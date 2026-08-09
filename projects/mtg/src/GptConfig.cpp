#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "GptConfig.h"
#include "JFileSystem.h"

#ifndef WAGIC_NO_CURL
#include <curl/curl.h>
#endif
#if defined(PSP)
//The PSP has no always-on IP stack: sockets only exist after the net modules
//are loaded and the access point association (an XMB connection profile) is
//brought up - see pspEnsureNetwork below.
#include <pspsdk.h>
#include <pspkernel.h>
#include <psputility.h>
#include <pspnet.h>
#include <pspnet_apctl.h>
//Set by netStackBoot in JGE/src/main.cpp: 0 never attempted, 1 up, -1 failed.
extern "C" int gWagicPspNetStack;
#endif
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstring>
//std::lock_guard for the codex auth mutex. Header-only, so safe on Vita too,
//where GptMutex is a kernel mutex precisely because std::mutex is a no-op
//there - lock_guard is just RAII over whatever BasicLockable it is given.
#include <mutex>
//Sign-in worker pacing on non-Vita platforms (Vita sleeps via the kernel).
#include <thread>
#include <chrono>

using std::string;
using std::vector;

GptSettings::GptSettings()
    : enabled(0), thinking(-1), maxTokens(-1), repetitionPenalty(1.0), timeoutSecs(600), patienceSecs(60), translog(0), telemetry(-1), peek(0)
{
}

namespace
{
//--- API key at-rest obfuscation -------------------------------------------
//NOT encryption (this is open source; the algorithm is public): the goal is
//that the config file never contains the key in the clear, so someone
//scrolling their files on a stream or screenshot leaks nothing readable.
//The key is XORed against a per-install random salt kept in a SEPARATE
//file (keysalt, mode 0600) - the config file alone is not decodable even
//with this code in hand. Cost: configs copied to another machine need the
//key re-entered.

const char * kObfPrefix = "obf1:";
const size_t kSaltBytes = 32;

const char kB64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string b64encode(const string& in)
{
    string out;
    int val = 0, bits = -6;
    for (size_t i = 0; i < in.size(); i++)
    {
        val = (val << 8) + (unsigned char) in[i];
        bits += 8;
        while (bits >= 0)
        {
            out += kB64[(val >> bits) & 0x3F];
            bits -= 6;
        }
    }
    if (bits > -6)
        out += kB64[((val << 8) >> (bits + 8)) & 0x3F];
    while (out.size() % 4)
        out += '=';
    return out;
}

string b64decode(const string& in)
{
    int T[256];
    for (int i = 0; i < 256; i++) T[i] = -1;
    for (int i = 0; i < 64; i++) T[(unsigned char) kB64[i]] = i;
    string out;
    int val = 0, bits = -8;
    for (size_t i = 0; i < in.size(); i++)
    {
        int c = T[(unsigned char) in[i]];
        if (c == -1)
            break; //padding or garbage
        val = (val << 6) + c;
        bits += 6;
        if (bits >= 0)
        {
            out += (char) ((val >> bits) & 0xFF);
            bits -= 8;
        }
    }
    return out;
}

//(gptUserRoot moved above the namespace - it is part of the public seam now;
//see GptConfig.h.)
static string gptUserRootImpl()
{
#ifdef VITA
    return "ux0:data/Wagic";
#elif defined(ANDROID)
    //An Android app process has no useful HOME, so $HOME/.Wagic would put the
    //endpoint config somewhere unwritable and the LLM opponent would look
    //unconfigurable. The engine already resolved the real user root through
    //SDLActivity (internal storage, or the SD card when the user picked it) -
    //every other Wagic file lives there, so the GPT config does too.
    {
        string root = JFileSystem::GetInstance()->GetUserRoot();
        while (root.size() && root[root.size() - 1] == '/')
            root.erase(root.size() - 1);
        return root;
    }
#elif defined(PSP)
    //No HOME on the PSP - an empty root here silently disabled config saves
    //AND gpt-log.txt. The writable per-user tree is User/ next to the EBOOT
    //(cwd), the same place the rest of the game writes its user files.
    return "User";
#else
    const char * home = getenv("HOME");
    return home ? string(home) + "/.Wagic" : string();
#endif
}

string saltPath()
{
    const string root = gptUserRootImpl();
    if (root.empty())
        return "";
    return root + "/ai/gpt/keysalt";
}

string loadOrCreateSalt()
{
    string path = saltPath();
    if (path.empty())
        return "";
    {
        std::ifstream f(path.c_str(), std::ios::binary);
        if (f)
        {
            std::ostringstream ss;
            ss << f.rdbuf();
            if (ss.str().size() >= kSaltBytes)
                return ss.str().substr(0, kSaltBytes);
        }
    }
    //First use: create the per-install salt.
    string salt;
    std::ifstream ur("/dev/urandom", std::ios::binary);
    if (ur)
    {
        char buf[kSaltBytes];
        ur.read(buf, kSaltBytes);
        if (ur.gcount() == (std::streamsize) kSaltBytes)
            salt.assign(buf, kSaltBytes);
    }
    if (salt.empty())
    {
        //Weak fallback; still beats plaintext for the shoulder-surf case.
        for (size_t i = 0; i < kSaltBytes; i++)
            salt += (char) (rand() & 0xFF);
    }
    //The directory chain may not exist yet on a first run.
    string dir = gptUserRootImpl();
    if (!dir.empty())
    {
        mkdir(dir.c_str(), 0755);
        dir += "/ai"; mkdir(dir.c_str(), 0755);
        dir += "/gpt"; mkdir(dir.c_str(), 0755);
    }
    std::ofstream f(path.c_str(), std::ios::binary | std::ios::trunc);
    if (f)
    {
        f.write(salt.data(), salt.size());
        f.close();
        chmod(path.c_str(), 0600);
    }
    return salt;
}

string xorWithSalt(const string& data, const string& salt)
{
    if (salt.empty())
        return data;
    string out = data;
    for (size_t i = 0; i < out.size(); i++)
        out[i] = out[i] ^ salt[i % salt.size()];
    return out;
}

string obfuscateKey(const string& plain)
{
    if (plain.empty())
        return "";
    string salt = loadOrCreateSalt();
    if (salt.empty())
        return plain; //no HOME: store as-is rather than lose the key
    return string(kObfPrefix) + b64encode(xorWithSalt(plain, salt));
}

string deobfuscateKey(const string& stored)
{
    if (stored.compare(0, strlen(kObfPrefix), kObfPrefix) != 0)
        return stored; //legacy/hand-entered plaintext keys keep working
    string salt = loadOrCreateSalt();
    string plain = xorWithSalt(b64decode(stored.substr(strlen(kObfPrefix))), salt);
    //A blob that decodes to non-printable bytes means the salt is gone or
    //foreign (config copied between installs): treat as no key.
    for (size_t i = 0; i < plain.size(); i++)
        if ((unsigned char) plain[i] < 0x20 || (unsigned char) plain[i] > 0x7E)
            return "";
    return plain;
}
} //namespace

string gptUserRoot()
{
    return gptUserRootImpl();
}

void gptLogLine(const string& line)
{
    const string root = gptUserRootImpl();
    if (root.empty())
        return; //nowhere writable; nothing useful to do

    const string dir = root + "/ai/gpt";
    //The directory chain only has to be built once per run. It used to be three
    //mkdir syscalls on EVERY line, which is invisible on a desktop and is not
    //on a console: this runs on the game thread, and on the handhelds storage
    //I/O per game event is a measured cause of lag.
    static bool dirsReady = false;
    if (!dirsReady)
    {
        string d = root;
        mkdir(d.c_str(), 0755);
        d += "/ai";  mkdir(d.c_str(), 0755);
        d += "/gpt"; mkdir(d.c_str(), 0755);
        dirsReady = true;
    }

    std::ofstream f((dir + "/gpt-log.txt").c_str(), std::ios::app);
    if (f)
        f << line << "\n";
}

string gptReadAsset(const char * filename)
{
    const string root = gptUserRootImpl();
    if (!root.empty())
    {
        string path = root + "/ai/gpt/" + filename;
        std::ifstream f(path.c_str(), std::ios::binary);
        if (f)
        {
            std::ostringstream ss;
            ss << f.rdbuf();
            string c = ss.str();
            if (!c.empty())
                return c;
        }
    }
    string content;
    JFileSystem::GetInstance()->readIntoString(string("ai/gpt/") + filename, content);
    return content;
}

GptSettings GptSettings::load()
{
    GptSettings cfg;
    string content = gptReadAsset("endpoints.txt");
    if (content.empty())
        return cfg;
    std::istringstream stream(content);
    string line;
    while (std::getline(stream, line))
    {
        size_t hash = line.find('#');
        if (hash != string::npos)
            line = line.substr(0, hash);
        size_t eq = line.find('=');
        if (eq == string::npos)
            continue;
        string k = line.substr(0, eq);
        string v = line.substr(eq + 1);
        k.erase(0, k.find_first_not_of(" \t")); k.erase(k.find_last_not_of(" \t\r") + 1);
        v.erase(0, v.find_first_not_of(" \t")); v.erase(v.find_last_not_of(" \t\r") + 1);
        if (v.empty())
            continue;
        if (k == "url") cfg.urls.push_back(v);
        else if (k == "model") cfg.model = v;
        else if (k == "key") cfg.key = v;
        else if (k == "enabled") cfg.enabled = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "thinking") cfg.thinking = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "maxtokens" || k == "max_reply_tokens") cfg.maxTokens = atol(v.c_str());
        else if (k == "repetition_penalty") cfg.repetitionPenalty = atof(v.c_str());
        else if (k == "provider_only") cfg.providerOnly = v;
        else if (k == "reasoning_effort") cfg.reasoningEffort = v;
        else if (k == "timeout") cfg.timeoutSecs = atoi(v.c_str());
        else if (k == "patience") cfg.patienceSecs = atoi(v.c_str());
        else if (k == "translog") cfg.translog = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "telemetry") cfg.telemetry = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "peek") cfg.peek = (v != "0" && v != "off") ? 1 : 0;
    }
    if (cfg.timeoutSecs < 5)
        cfg.timeoutSecs = 5;
    //0 means "never ask", any other too-small value would prompt faster than
    //a normal decision completes and turn the dialog into the annoyance.
    if (cfg.patienceSecs < 0)
        cfg.patienceSecs = 0;
    else if (cfg.patienceSecs > 0 && cfg.patienceSecs < 10)
        cfg.patienceSecs = 10;
    cfg.key = deobfuscateKey(cfg.key);
    return cfg;
}

bool GptSettings::save() const
{
    string dir = gptUserRootImpl();
    if (dir.empty())
        return false;
    mkdir(dir.c_str(), 0755);
    dir += "/ai";
    mkdir(dir.c_str(), 0755);
    dir += "/gpt";
    mkdir(dir.c_str(), 0755);

    std::ofstream f((dir + "/endpoints.txt").c_str(), std::ios::trunc);
    if (!f)
        return false;
    f << "# LLM opponent configuration. Managed by the in-game GPT options\n"
         "# tab; hand edits are honored, but the GUI rewrites this file on\n"
         "# save (comments are not preserved). Environment variables\n"
         "# (WAGIC_AI, WAGIC_GPT_URL/MODEL/KEY/...) override these values.\n";
    f << "enabled=" << enabled << "\n";
    for (size_t i = 0; i < urls.size(); i++)
        f << "url=" << urls[i] << "\n";
    if (!model.empty())
        f << "model=" << model << "\n";
    if (!key.empty())
        f << "key=" << obfuscateKey(key) << "\n";
    if (thinking >= 0)
        f << "thinking=" << thinking << "\n";
    if (maxTokens > 0)
        f << "max_reply_tokens=" << maxTokens << "\n";
    if (repetitionPenalty != 1.0)
        f << "repetition_penalty=" << repetitionPenalty << "\n";
    if (!providerOnly.empty())
        f << "provider_only=" << providerOnly << "\n";
    if (!reasoningEffort.empty())
        f << "reasoning_effort=" << reasoningEffort << "\n";
    f << "timeout=" << timeoutSecs << "\n";
    f << "patience=" << patienceSecs << "\n";
    if (translog)
        f << "translog=1\n";
    if (telemetry >= 0)
        f << "telemetry=" << telemetry << "\n";
    if (peek)
        f << "peek=1\n";
    return f.good();
}

bool GptSettings::operator==(const GptSettings& o) const
{
    return enabled == o.enabled && urls == o.urls && model == o.model && key == o.key
        && thinking == o.thinking && maxTokens == o.maxTokens
        && repetitionPenalty == o.repetitionPenalty
        && providerOnly == o.providerOnly
        && reasoningEffort == o.reasoningEffort
        && timeoutSecs == o.timeoutSecs && patienceSecs == o.patienceSecs
        && translog == o.translog && telemetry == o.telemetry
        && peek == o.peek;
}

bool gptPeekOpponentHand()
{
    static int cached = -1;
    if (cached < 0)
    {
        const char * env = getenv("WAGIC_GPT_PEEK");
        if (env)
            cached = (*env && *env != '0') ? 1 : 0;
        else
            cached = (GptSettings::load().peek == 1) ? 1 : 0;
    }
    return cached == 1;
}

string GptSettings::primaryUrl() const
{
    return urls.empty() ? string("") : urls[0];
}

void GptSettings::setPrimaryUrl(const string& u)
{
    if (urls.empty())
        urls.push_back(u);
    else
        urls[0] = u;
}

//Provider base URLs are long-lived: these have been stable for years.
//The game appends /v1/models and /v1/chat/completions - except the
//"OpenAI subscription" preset, whose base is the Codex backend: the request
//path is /responses, auth comes from the device-code token file rather than
//key=, and the whole exchange is handled by gptCodexComplete.
static const GptPreset kPresets[] = {
    { "Custom", "" },
#if !defined(PSP) && !defined(VITA)
    //Handhelds can't host the model: 127.0.0.1 is the device itself, so the
    //local-server presets only make sense where a server can actually run.
    { "Local llama.cpp", "http://127.0.0.1:8080" },
    { "Local Ollama", "http://127.0.0.1:11434" },
    { "LM Studio", "http://127.0.0.1:1234" },
#endif
#if !defined(PSP)
    //PSP transport is plain-HTTP to a LAN companion bridge only: no TLS rules
    //out every https:// provider too - so Custom (the user's bridge URL) is
    //the only connection type that can work there. The Vita keeps these: its
    //curl/TLS stack drives them for real.
    { "OpenRouter", "https://openrouter.ai/api" },
    { "OpenAI", "https://api.openai.com" },
    { "OpenAI subscription", "https://chatgpt.com/backend-api/codex" },
    { "Anthropic", "https://api.anthropic.com" },
    { "Groq", "https://api.groq.com/openai" },
    { "Mistral", "https://api.mistral.ai" },
    { "DeepSeek", "https://api.deepseek.com" },
    { "xAI", "https://api.x.ai" },
#endif
};

const GptPreset * gptPresets(size_t& count)
{
    count = sizeof(kPresets) / sizeof(kPresets[0]);
    return kPresets;
}

size_t gptPresetForUrl(const string& url)
{
    for (size_t i = 1; i < sizeof(kPresets) / sizeof(kPresets[0]); i++)
        if (url == kPresets[i].url)
            return i;
    return 0;
}

#ifdef WAGIC_HTTP_JNI
//--- Android transport ------------------------------------------------------
//
//Android has no libcurl port in this tree, and does not need one: the platform
//already ships a maintained TLS stack behind java.net.HttpURLConnection, which
//also means system trust anchors and OS-level updates rather than a bundled CA
//bundle we would have to keep fresh. The whole transport is therefore ONE
//static Java method (SDLActivity.gptHttpRequest), called over JNI.
//
//Two JNI facts shape this code:
// 1. FindClass on a thread ATTACHED FROM NATIVE resolves against the system
//    class loader, which cannot see app classes - it would fail with
//    ClassNotFoundException. The model call runs on exactly such a thread
//    (AIPlayerGPT's detached worker), so the class reference is captured ONCE
//    during JNI_OnLoad, where the loader is still the app's, and kept as a
//    global ref.
// 2. A natively-attached thread MUST detach before it exits or the VM aborts
//    the process. We attach and detach around each call.
#include <jni.h>

namespace
{
JavaVM * gGptJvm = NULL;
jclass gGptActivityClass = NULL;
} //namespace

void gptAndroidCacheClass(JNIEnv * env)
{
    if (!env || gGptActivityClass)
        return;
    env->GetJavaVM(&gGptJvm);
    jclass local = env->FindClass("org/libsdl/app/SDLActivity");
    if (!local)
    {
        env->ExceptionClear();
        gptLogLine("android transport: SDLActivity not found at load time");
        return;
    }
    gGptActivityClass = (jclass) env->NewGlobalRef(local);
    env->DeleteLocalRef(local);
}

namespace
{
string httpRequestImpl(const string& url, const string& postBody, long timeoutMs, const string& bearer)
{
    if (!gGptJvm || !gGptActivityClass)
        return "";

    JNIEnv * env = NULL;
    bool attached = false;
    jint status = gGptJvm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if (status == JNI_EDETACHED || status == JNI_ERR)
    {
        if (gGptJvm->AttachCurrentThread(&env, NULL) != JNI_OK)
        {
            gptLogLine("android transport: could not attach the calling thread");
            return "";
        }
        attached = true;
    }
    if (!env)
        return "";

    string result;
    jmethodID mid = env->GetStaticMethodID(
        gGptActivityClass, "gptHttpRequest",
        "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)Ljava/lang/String;");
    if (!mid)
    {
        env->ExceptionClear();
        gptLogLine("android transport: SDLActivity.gptHttpRequest is missing");
    }
    else
    {
        jstring jUrl = env->NewStringUTF(url.c_str());
        jstring jBody = env->NewStringUTF(postBody.c_str());
        jstring jKey = env->NewStringUTF(bearer.c_str());
        jstring reply = (jstring) env->CallStaticObjectMethod(
            gGptActivityClass, mid, jUrl, jBody, (jint) timeoutMs, jKey);
        //An exception thrown across JNI stays pending and poisons every later
        //call on this thread; the Java side already returns "" for failures, so
        //anything pending here is a bug worth naming rather than swallowing.
        if (env->ExceptionCheck())
        {
            env->ExceptionDescribe();
            env->ExceptionClear();
            gptLogLine("android transport: java side threw");
        }
        else if (reply)
        {
            const char * cstr = env->GetStringUTFChars(reply, NULL);
            if (cstr)
            {
                result = cstr;
                env->ReleaseStringUTFChars(reply, cstr);
            }
        }
        //A silent empty reply is indistinguishable from a wrong IP, and the
        //most likely causes on Android are platform policy (blocked cleartext)
        //or an auth failure - both worth naming in the log a user can send us.
        if (result.empty())
        {
            jmethodID errId = env->GetStaticMethodID(gGptActivityClass, "gptLastError",
                                                     "()Ljava/lang/String;");
            if (!errId)
                env->ExceptionClear();
            else
            {
                jstring why = (jstring) env->CallStaticObjectMethod(gGptActivityClass, errId);
                if (env->ExceptionCheck())
                    env->ExceptionClear();
                else if (why)
                {
                    const char * cwhy = env->GetStringUTFChars(why, NULL);
                    if (cwhy && *cwhy)
                        gptLogLine(string("android transport failed: ") + cwhy);
                    if (cwhy)
                        env->ReleaseStringUTFChars(why, cwhy);
                }
                if (why) env->DeleteLocalRef(why);
            }
        }
        if (reply) env->DeleteLocalRef(reply);
        env->DeleteLocalRef(jKey);
        env->DeleteLocalRef(jBody);
        env->DeleteLocalRef(jUrl);
    }

    if (attached)
        gGptJvm->DetachCurrentThread();
    return result;
}
#elif defined(WAGIC_NO_CURL)
namespace
{
//Platforms without any wired transport: the request reports failure, which
//every GPT seam already treats as "fall back to Baka" - the same behavior as
//an unreachable endpoint.
string httpRequestImpl(const string&, const string&, long, const string&)
{
    return "";
}
#else
namespace
{
size_t curlWriteToString(void * contents, size_t size, size_t nmemb, void * userp)
{
    static_cast<string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

#if defined(PSP)
//Bring the PSP's network up once per launch: the stack (modules + inet pool)
//comes up at APP START while the heap is empty - sceNetInit's pool
//allocation fails with NO_MEMORY (80020190, proven from the options menu)
//once the game has loaded, the same carve-early law as the texture pool.
//Association with XMB connection profile 1 stays lazy on the first request
//(the access point may not exist at boot). One association attempt per
//launch - a failed one costs one bounded wait, not one per decision.
static int gPspStackState = 0; //0 untried, 1 up, -1 failed
static int gPspNetState = 0;   //0 untried, 1 up, -1 failed

//The stack itself comes up first thing in main() (JGE/src/main.cpp
//netStackBoot - loading the net modules any later hangs the boot); this just
//reads the verdict it left (declared at file scope above - an extern "C"
//inside the anonymous namespace picks up internal linkage).
static void pspNetStackInit()
{
    if (gPspStackState) return;
    gPspStackState = (gWagicPspNetStack > 0) ? 1 : -1;
    if (gPspStackState < 0)
        gptLogLine("psp net: stack failed at boot (see User/wagic-netboot.log)");
}
static bool pspEnsureNetwork()
{
    if (gPspNetState) return gPspNetState > 0;
    gPspNetState = -1;
    if (gPspStackState == 0)
        pspNetStackInit(); //fallback; the real call is at app start
    if (gPspStackState < 0)
        return false; //stack never came up; reason already logged
    int rc = sceNetApctlConnect(1);
    if (rc != 0)
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "psp net: apctl connect(1) failed %08x", (unsigned) rc);
        gptLogLine(buf);
        return false;
    }
    int state = 0;
    for (int i = 0; i < 600; i++) //30s ceiling, 50ms per poll
    {
        if (sceNetApctlGetState(&state) != 0)
        {
            gptLogLine("psp net: apctl state poll failed");
            return false;
        }
        if (state == PSP_NET_APCTL_STATE_GOT_IP)
        {
            gPspNetState = 1;
            gptLogLine("psp net: associated, got IP");
            return true;
        }
        sceKernelDelayThread(50 * 1000);
    }
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "psp net: association timeout at state %d", state);
        gptLogLine(buf);
    }
    return false;
}
#endif

string httpRequestImpl(const string& url, const string& postBody, long timeoutMs, const string& bearer)
{
#if defined(PSP)
    if (!pspEnsureNetwork())
        return "";
#endif
    CURL * curl = curl_easy_init();
    if (!curl)
        return "";

    string response;
    struct curl_slist * headers = NULL;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 2500L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    if (!bearer.empty())
        headers = curl_slist_append(headers, ("Authorization: Bearer " + bearer).c_str());
    if (!postBody.empty())
    {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postBody.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postBody.size());
    }
    if (headers)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    if (headers)
        curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK || httpCode != 200)
        return "";
    return response;
}
#endif //WAGIC_NO_CURL
} //namespace

#if defined(PSP)
//GameApp calls this at app start (heap empty - see pspNetStackInit above).
void wagicGptNetStackInit()
{
    pspNetStackInit();
}
#endif

string gptHttpGet(const string& url, long timeoutMs, const string& bearer)
{
    return httpRequestImpl(url, "", timeoutMs, bearer);
}

string gptHttpPost(const string& url, const string& body, long timeoutMs, const string& bearer)
{
    return httpRequestImpl(url, body, timeoutMs, bearer);
}

//--- Full-control POST (ChatGPT-subscription transport) ---------------------
//The Codex backend needs caller-supplied headers and the caller needs the HTTP
//status and response headers back (401 drives a token refresh; the x-codex-*
//headers carry the plan-usage gauge). Curl-only: on platforms without curl the
//call reports transport failure, which degrades to Baka exactly like an
//unreachable endpoint - the subscription preset simply is not available there.
namespace
{
#if !defined(WAGIC_HTTP_JNI) && !defined(WAGIC_NO_CURL)
size_t curlHeaderToString(void * contents, size_t size, size_t nmemb, void * userp)
{
    static_cast<string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

string httpRequestFull(const string& url, const string& postBody, long timeoutMs,
                       const vector<string>& reqHeaders, long& httpCode, string& respHeaders)
{
    httpCode = 0;
    respHeaders.clear();
    CURL * curl = curl_easy_init();
    if (!curl)
        return "";

    string response;
    struct curl_slist * headers = NULL;
    for (size_t i = 0; i < reqHeaders.size(); i++)
        headers = curl_slist_append(headers, reqHeaders[i].c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, curlHeaderToString);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &respHeaders);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 5000L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (!postBody.empty())
    {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postBody.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) postBody.size());
    }
    if (headers)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    if (headers)
        curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        httpCode = 0;
        return "";
    }
    //Unlike httpRequestImpl, the body comes back on EVERY status: the error
    //payload ({"detail": ...}) is how the caller tells a wrong route from an
    //expired token from a rate limit.
    return response;
}
#else
string httpRequestFull(const string&, const string&, long,
                       const vector<string>&, long& httpCode, string& respHeaders)
{
    httpCode = 0;
    respHeaders.clear();
    return "";
}
#endif
} //namespace

//=== ChatGPT-subscription backend (Codex Responses API) =====================
//
//The "OpenAI subscription" preset talks to the SAME backend Codex CLI uses:
//POST https://chatgpt.com/backend-api/codex/responses, authorized by the OAuth
//tokens a ChatGPT plan account mints through the device-code flow. Everything
//below was verified live 2026-08-09 (Voyager probes) and against the
//openai/codex source, because earlier research digests were wrong twice:
// - The endpoint takes ONLY Codex-shaped fields. stream:false is REJECTED
//   ("Stream must be set to true") and max_output_tokens is REJECTED
//   ("Unsupported parameter"), so replies are always SSE and there is no
//   decode-side truncation cap on this preset.
// - The minimal header set suffices - no attestation headers. originator and
//   the User-Agent are Codex-fingerprint-coupled constants; if the backend
//   ever starts challenging, they are the first thing to revisit.
// - Refresh: POST https://auth.openai.com/oauth/token, JSON body
//   {client_id, grant_type:"refresh_token", refresh_token} (codex-rs
//   login/src/auth/manager.rs). Refresh tokens ROTATE, and reusing a stale one
//   is a hard error (refresh_token_reused) - which is why every auth mutation
//   here is serialized under one mutex and persisted immediately.
// - An HTML body on any of these calls means the WEB tier answered: wrong
//   route or blocked, never a model reply.
//
//Auth material lives in oai-auth.json (shape: {"tokens":{access_token,
//refresh_token,id_token,expires_in,...},"minted_at":N,"chatgpt_account_id":S})
//under <user root>/ai/gpt/, with a desktop fallback to ~/.config/wagic-oai/
//(auth.json + sibling account.json) where the device-code scripts mint it.
//No in-client device-code flow yet: the file is provisioned externally.

namespace
{
const char * kCodexClientId = "app_EMoamEEZ73f0CkXaXp7hrann";
const char * kCodexTokenUrl = "https://auth.openai.com/oauth/token";
const char * kCodexOriginator = "codex_cli_rs";
const char * kCodexUserAgent = "codex_cli_rs/0.45.0 (Ubuntu 24.04.2 LTS; x86_64) WindowsTerminal";

//All auth state (file cache + rotation) behind one mutex: two AI seats can
//complete concurrently, and a double-refresh with the same rotated-out token
//would invalidate the whole login.
GptMutex& codexMutex()
{
    static GptMutex m;
    return m;
}

struct CodexAuth
{
    bool loaded;
    nlohmann::json doc;   //the whole auth file, mutated in place on refresh
    string path;          //where doc came from / gets written back
    string access;
    string refresh;
    string accountId;
    long mintedAt;        //epoch seconds; 0 unknown
    long expiresIn;       //seconds; 0 unknown
    CodexAuth() : loaded(false), mintedAt(0), expiresIn(0) {}
};

CodexAuth& codexAuth()
{
    static CodexAuth a;
    return a;
}

//Pull the useful fields out of whatever auth-file shape we were handed.
void codexReadDoc(CodexAuth& a)
{
    a.access.clear();
    a.refresh.clear();
    a.mintedAt = 0;
    a.expiresIn = 0;
    try
    {
        const nlohmann::json& d = a.doc;
        const nlohmann::json& t = d.contains("tokens") ? d["tokens"] : d;
        if (t.contains("access_token") && t["access_token"].is_string())
            a.access = t["access_token"].get<string>();
        if (t.contains("refresh_token") && t["refresh_token"].is_string())
            a.refresh = t["refresh_token"].get<string>();
        if (t.contains("expires_in") && t["expires_in"].is_number())
            a.expiresIn = t["expires_in"].get<long>();
        if (d.contains("minted_at") && d["minted_at"].is_number())
            a.mintedAt = d["minted_at"].get<long>();
        if (d.contains("chatgpt_account_id") && d["chatgpt_account_id"].is_string())
            a.accountId = d["chatgpt_account_id"].get<string>();
    }
    catch (nlohmann::json::exception&)
    {
    }
}

bool codexLoadFile(CodexAuth& a, const string& path)
{
    std::ifstream f(path.c_str(), std::ios::binary);
    if (!f)
        return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    try
    {
        a.doc = nlohmann::json::parse(ss.str());
    }
    catch (nlohmann::json::exception&)
    {
        return false;
    }
    a.path = path;
    codexReadDoc(a);
    return !a.access.empty() || !a.refresh.empty();
}

//Locate + parse auth material. Caller holds codexMutex().
bool codexEnsureLoaded(string& whyNot)
{
    CodexAuth& a = codexAuth();
    if (a.loaded)
    {
        if (a.access.empty() && a.refresh.empty())
        {
            whyNot = "no usable tokens in " + (a.path.empty() ? string("any auth file") : a.path);
            return false;
        }
        return true;
    }

    vector<string> candidates;
    string root = gptUserRoot();
    if (!root.empty())
        candidates.push_back(root + "/ai/gpt/oai-auth.json");
    if (const char * home = getenv("HOME"))
        candidates.push_back(string(home) + "/.config/wagic-oai/auth.json");

    for (size_t i = 0; i < candidates.size(); i++)
    {
        if (!codexLoadFile(a, candidates[i]))
            continue;
        //The device-code scripts keep the account id in a SIBLING account.json.
        if (a.accountId.empty())
        {
            size_t slash = candidates[i].find_last_of('/');
            if (slash != string::npos)
            {
                std::ifstream acc((candidates[i].substr(0, slash) + "/account.json").c_str());
                if (acc)
                {
                    std::ostringstream ss;
                    ss << acc.rdbuf();
                    try
                    {
                        nlohmann::json j = nlohmann::json::parse(ss.str());
                        if (j.contains("chatgpt_account_id") && j["chatgpt_account_id"].is_string())
                            a.accountId = j["chatgpt_account_id"].get<string>();
                    }
                    catch (nlohmann::json::exception&)
                    {
                    }
                }
            }
        }
        a.loaded = true;
        if (a.accountId.empty())
        {
            whyNot = "auth file found but no chatgpt_account_id (need account.json or the field inline)";
            return false;
        }
        return true;
    }
    a.loaded = true; //remember the miss; a new file takes a restart, like peek
    whyNot = "no oai-auth.json found (checked ";
    for (size_t i = 0; i < candidates.size(); i++)
        whyNot += (i ? ", " : "") + candidates[i];
    whyNot += ")";
    return false;
}

//Persist the (rotated) tokens back where they came from. Caller holds the mutex.
void codexSave(CodexAuth& a)
{
    if (a.path.empty())
        return;
    std::ofstream f(a.path.c_str(), std::ios::binary | std::ios::trunc);
    if (f)
    {
        f << a.doc.dump(1) << "\n";
        f.close();
        chmod(a.path.c_str(), 0600);
    }
}

//Refresh the access token. Caller holds the mutex. Rotates and persists.
bool codexRefresh(CodexAuth& a, string& err)
{
    if (a.refresh.empty())
    {
        err = "no refresh token";
        return false;
    }
    nlohmann::json body = {
        {"client_id", kCodexClientId},
        {"grant_type", "refresh_token"},
        {"refresh_token", a.refresh},
    };
    vector<string> headers;
    headers.push_back("Content-Type: application/json");
    long code = 0;
    string respHeaders;
    string resp = httpRequestFull(kCodexTokenUrl, body.dump(), 30000, headers, code, respHeaders);
    if (code != 200)
    {
        err = "token refresh failed (HTTP " + std::to_string(code) + "): " + resp.substr(0, 200);
        return false;
    }
    try
    {
        nlohmann::json j = nlohmann::json::parse(resp);
        nlohmann::json& t = a.doc.contains("tokens") ? a.doc["tokens"] : a.doc;
        if (j.contains("access_token") && j["access_token"].is_string())
            t["access_token"] = j["access_token"];
        if (j.contains("refresh_token") && j["refresh_token"].is_string())
            t["refresh_token"] = j["refresh_token"];
        if (j.contains("id_token") && j["id_token"].is_string())
            t["id_token"] = j["id_token"];
        a.doc["minted_at"] = (long) time(NULL);
        if (!a.accountId.empty())
            a.doc["chatgpt_account_id"] = a.accountId;
        codexReadDoc(a);
        codexSave(a);
        return !a.access.empty();
    }
    catch (nlohmann::json::exception&)
    {
        err = "token refresh returned unparseable JSON";
        return false;
    }
}

//Last plan-usage percent the backend reported (x-codex-primary-used-percent).
string& codexUsedPercentRef()
{
    static string pct;
    return pct;
}

//Case-insensitive single-header lookup in a raw response-header blob.
string headerValue(const string& respHeaders, const string& name)
{
    string low = respHeaders;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    string needle = name + ":";
    size_t pos = 0;
    while ((pos = low.find(needle, pos)) != string::npos)
    {
        //Header names start a line.
        if (pos != 0 && low[pos - 1] != '\n')
        {
            pos += needle.size();
            continue;
        }
        size_t vs = pos + needle.size();
        size_t ve = respHeaders.find('\n', vs);
        string v = respHeaders.substr(vs, ve == string::npos ? string::npos : ve - vs);
        while (!v.empty() && (v[0] == ' ' || v[0] == '\t'))
            v.erase(0, 1);
        while (!v.empty() && (v[v.size() - 1] == '\r' || v[v.size() - 1] == ' '))
            v.erase(v.size() - 1);
        return v;
    }
    return "";
}
} //namespace

//The model the preset answers with when the user has not configured one.
const char * const kGptCodexDefaultModel = "gpt-5.6-luna";

//Established by probing 2026-08-09: these four answer; everything else
//(-fast/-codex variants included) is refused for ChatGPT accounts. Peer
//options for the end user - listed in probe order, no ranking implied.
static const char * const kCodexModelIds[] = {
    "gpt-5.6-luna",
    "gpt-5.6-terra",
    "gpt-5.6-sol",
    "gpt-5.5",
};

const char * const * gptCodexModels(size_t& count)
{
    count = sizeof(kCodexModelIds) / sizeof(kCodexModelIds[0]);
    return kCodexModelIds;
}

bool gptCodexEffortValid(const string& s)
{
    static const char * const kTiers[] = { "none", "low", "medium", "high", "xhigh", "max" };
    for (size_t i = 0; i < sizeof(kTiers) / sizeof(kTiers[0]); i++)
        if (s == kTiers[i])
            return true;
    return false;
}

bool gptCodexEndpoint(const string& url)
{
    return url.find("chatgpt.com/backend-api") != string::npos;
}

bool gptCodexAuthPresent(string& whyNot)
{
    std::lock_guard<GptMutex> g(codexMutex());
    return codexEnsureLoaded(whyNot);
}

string gptCodexUsedPercent()
{
    std::lock_guard<GptMutex> g(codexMutex());
    return codexUsedPercentRef();
}

string gptCodexExtractText(const string& sse)
{
    //The completed event's output array arrives EMPTY on this backend (probed
    //2026-08-09): the text rides response.output_text.done (one per content
    //part), with the .delta stream as the incremental form. Prefer the done
    //events, fall back to accumulated deltas, then to the completed event's
    //output array in case the backend ever starts populating it.
    string dones, deltas, completed;
    std::istringstream ss(sse);
    string line;
    while (std::getline(ss, line))
    {
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        if (line.compare(0, 5, "data:") != 0)
            continue;
        size_t s = 5;
        while (s < line.size() && line[s] == ' ')
            s++;
        try
        {
            nlohmann::json j = nlohmann::json::parse(line.substr(s));
            string type = j.value("type", "");
            if (type == "response.output_text.done")
                dones += j.value("text", "");
            else if (type == "response.output_text.delta")
                deltas += j.value("delta", "");
            else if (type == "response.completed" && j.contains("response"))
            {
                const nlohmann::json& out = j["response"].value("output", nlohmann::json::array());
                for (nlohmann::json::const_iterator it = out.begin(); it != out.end(); ++it)
                {
                    if (it->value("type", "") != "message" || !it->contains("content"))
                        continue;
                    const nlohmann::json& content = (*it)["content"];
                    for (nlohmann::json::const_iterator c = content.begin(); c != content.end(); ++c)
                        if (c->value("type", "") == "output_text")
                            completed += c->value("text", "");
                }
            }
        }
        catch (nlohmann::json::exception&)
        {
            //Non-JSON data lines (or a truncated tail) are skipped, not fatal.
        }
    }
    if (!dones.empty())
        return dones;
    if (!deltas.empty())
        return deltas;
    return completed;
}

string gptCodexComplete(const string& url, const string& requestBody, long timeoutMs, string& errOut)
{
    errOut.clear();
    string access, accountId;
    {
        std::lock_guard<GptMutex> g(codexMutex());
        if (!codexEnsureLoaded(errOut))
            return "";
        CodexAuth& a = codexAuth();
        //Proactive refresh an hour before known expiry - cheaper than paying a
        //401 round trip on a game decision.
        if (a.mintedAt > 0 && a.expiresIn > 0
            && (long) time(NULL) > a.mintedAt + a.expiresIn - 3600 && !a.refresh.empty())
        {
            string rerr;
            if (!codexRefresh(a, rerr) && a.access.empty())
            {
                errOut = rerr;
                return "";
            }
        }
        access = a.access;
        accountId = a.accountId;
    }
    if (access.empty())
    {
        errOut = "no access token";
        return "";
    }

    vector<string> headers;
    headers.push_back("Authorization: Bearer " + access);
    headers.push_back("chatgpt-account-id: " + accountId);
    headers.push_back("Content-Type: application/json");
    headers.push_back(string("originator: ") + kCodexOriginator);
    headers.push_back("OpenAI-Beta: responses=experimental");
    headers.push_back(string("User-Agent: ") + kCodexUserAgent);
    headers.push_back("Accept: text/event-stream");

    long code = 0;
    string respHeaders;
    string body = httpRequestFull(url, requestBody, timeoutMs, headers, code, respHeaders);

    if (code == 401)
    {
        //Expired or revoked access token: refresh once and retry. Another seat
        //may have refreshed while we were in flight - only refresh if the token
        //we used is still the current one.
        {
            std::lock_guard<GptMutex> g(codexMutex());
            CodexAuth& a = codexAuth();
            if (a.access == access)
            {
                string rerr;
                if (!codexRefresh(a, rerr))
                {
                    errOut = "authorization rejected and " + rerr;
                    return "";
                }
            }
            access = a.access;
        }
        headers[0] = "Authorization: Bearer " + access;
        body = httpRequestFull(url, requestBody, timeoutMs, headers, code, respHeaders);
    }

    if (code != 200)
    {
        if (code == 0)
            errOut = "transport failure (no connection, or no TLS transport on this platform)";
        else if (!body.empty() && body[0] == '<')
            errOut = "HTTP " + std::to_string(code) + " from the web tier (wrong route or blocked)";
        else if (code == 429)
            errOut = "rate limited by the plan window: " + body.substr(0, 200);
        else
            errOut = "HTTP " + std::to_string(code) + ": " + body.substr(0, 200);
        return "";
    }

    string pct = headerValue(respHeaders, "x-codex-primary-used-percent");
    if (!pct.empty())
    {
        std::lock_guard<GptMutex> g(codexMutex());
        codexUsedPercentRef() = pct;
    }

    string text = gptCodexExtractText(body);
    if (text.empty())
        errOut = "no output text in the response stream";
    return text;
}

//--- Platform threading seam (see GptConfig.h) -----------------------------
#if defined (VITA)

#include <psp2/kernel/threadmgr.h>

GptMutex::GptMutex()
{
    mId = sceKernelCreateMutex("gpt_mutex", 0, 0, NULL);
}

GptMutex::~GptMutex()
{
    if (mId >= 0)
        sceKernelDeleteMutex(mId);
}

void GptMutex::lock()
{
    if (mId >= 0)
        sceKernelLockMutex(mId, 1, NULL);
}

void GptMutex::unlock()
{
    if (mId >= 0)
        sceKernelUnlockMutex(mId, 1);
}

namespace
{
struct SpawnArgs
{
    void (*fn)(void *);
    void * ctx;
};

int gptWorkerEntry(SceSize args, void * argp)
{
    //sceKernelStartThread copied SpawnArgs onto this thread's stack.
    SpawnArgs a = *reinterpret_cast<SpawnArgs *>(argp);
    a.fn(a.ctx);
    //Detached semantics: the thread frees itself on exit.
    sceKernelExitDeleteThread(0);
    return 0;
}
} //namespace

bool gptSpawnWorker(void (*fn)(void *), void * ctx)
{
    if (getenv("WAGIC_GPT_NOTHREAD"))
        return false;
    //Priority 0x10000100 = the process default; the worker spends its life
    //blocked in curl I/O, so it does not contend with the render loop.
    //64KB stack: curl + OpenSSL handshake depth, measured generously.
    SceUID id = sceKernelCreateThread("gpt_worker", gptWorkerEntry, 0x10000100, 0x10000, 0, 0, NULL);
    if (id < 0)
        return false;
    SpawnArgs a;
    a.fn = fn;
    a.ctx = ctx;
    if (sceKernelStartThread(id, sizeof(a), &a) < 0)
    {
        sceKernelDeleteThread(id);
        return false;
    }
    return true;
}

#elif defined (PSP)

//No worker until task #6 wires sceNet bring-up; when it does, implement this
//with sceKernelCreateThread like the Vita branch (PSP's libstdc++ may not even
//declare std::thread - its gthreads layer is absent, same as Vita's).
bool gptSpawnWorker(void (*)(void *), void *)
{
    return false;
}

#else //desktop and everything with a working std::thread

#include <thread>

bool gptSpawnWorker(void (*fn)(void *), void * ctx)
{
    if (getenv("WAGIC_GPT_NOTHREAD"))
        return false;
    try
    {
        std::thread(fn, ctx).detach();
        return true;
    }
    catch (const std::exception&)
    {
        //Platform refused a thread (resource limits, inactive gthreads
        //layer). The caller degrades to its synchronous path.
        return false;
    }
}

#endif //platform threading seam

//=== In-client ChatGPT sign-in (device-code flow) ===========================
//
//Runs the same beta flow Codex CLI uses (paths verified against openai/codex
//and proven live 2026-08-09 - note the /api/accounts/ segment the early
//research digest dropped):
//  1. POST {AUTH}/api/accounts/deviceauth/usercode  {"client_id"}
//     -> {device_auth_id, user_code|usercode, interval}
//  2. show user_code + a QR of {AUTH}/codex/device; the user approves from
//     any modern browser (their phone - consoles cannot render that SPA)
//  3. poll POST {AUTH}/api/accounts/deviceauth/token {device_auth_id,
//     user_code}; 403/404 = still pending
//     -> {authorization_code, code_challenge, code_verifier} (the PKCE pair
//     is SERVER-generated in this flow)
//  4. exchange: POST {AUTH}/oauth/token, form-encoded, redirect_uri =
//     {AUTH}/deviceauth/callback (device flow - NOT the localhost callback)
//  5. persist tokens + the account id (from the id_token's JWT claims) as
//     oai-auth.json, then drop the in-memory auth cache so the next
//     completion (and Test connection) reads the fresh login.

#if !defined(WAGIC_HTTP_JNI) && !defined(WAGIC_NO_CURL)
namespace
{
const char * kOaiAuthBase = "https://auth.openai.com";

void oaiSleepMs(int ms)
{
#if defined (VITA)
    sceKernelDelayThread(ms * 1000);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

string urlEncode(const string& s)
{
    static const char * hex = "0123456789ABCDEF";
    string out;
    for (size_t i = 0; i < s.size(); i++)
    {
        unsigned char c = (unsigned char) s[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            out += (char) c;
        else
        {
            out += '%';
            out += hex[c >> 4];
            out += hex[c & 15];
        }
    }
    return out;
}

//JWT payload: base64URL (-_ alphabet, unpadded) - translate and reuse the
//standard decoder above.
nlohmann::json jwtPayload(const string& token)
{
    size_t a = token.find('.');
    size_t b = (a == string::npos) ? string::npos : token.find('.', a + 1);
    if (a == string::npos || b == string::npos)
        return nlohmann::json();
    string part = token.substr(a + 1, b - a - 1);
    for (size_t i = 0; i < part.size(); i++)
    {
        if (part[i] == '-') part[i] = '+';
        else if (part[i] == '_') part[i] = '/';
    }
    while (part.size() % 4)
        part += '=';
    try
    {
        return nlohmann::json::parse(b64decode(part));
    }
    catch (nlohmann::json::exception&)
    {
        return nlohmann::json();
    }
}

struct OaiSignInCtx
{
    std::shared_ptr<GptOaiSignIn> st;
};

void oaiFail(std::shared_ptr<GptOaiSignIn>& st, const string& why)
{
    gptLogLine("sign-in failed: " + why);
    std::lock_guard<GptMutex> g(st->mtx);
    st->error = why;
    st->status = 3;
}

void OaiSignInMain(void * p)
{
    OaiSignInCtx * ctx = static_cast<OaiSignInCtx *>(p);
    std::shared_ptr<GptOaiSignIn> st = ctx->st;
    delete ctx;

    vector<string> jsonHdr;
    jsonHdr.push_back("Content-Type: application/json");
    long code = 0;
    string respHeaders;

    //1. user code
    nlohmann::json req = {{"client_id", kCodexClientId}};
    string body = httpRequestFull(string(kOaiAuthBase) + "/api/accounts/deviceauth/usercode",
                                  req.dump(), 30000, jsonHdr, code, respHeaders);
    if (code != 200)
    {
        oaiFail(st, "could not get a sign-in code (HTTP " + std::to_string(code) + "): " + body.substr(0, 160));
        return;
    }
    string deviceAuthId, userCode;
    long intervalSecs = 5;
    try
    {
        nlohmann::json j = nlohmann::json::parse(body);
        deviceAuthId = j.value("device_auth_id", "");
        userCode = j.contains("user_code") ? j.value("user_code", "") : j.value("usercode", "");
        //codex tolerates number-or-string here; so do we.
        if (j.contains("interval"))
            intervalSecs = j["interval"].is_number() ? j["interval"].get<long>()
                                                     : atol(j["interval"].get<string>().c_str());
    }
    catch (nlohmann::json::exception&)
    {
    }
    if (deviceAuthId.empty() || userCode.empty())
    {
        oaiFail(st, "sign-in code reply had no code: " + body.substr(0, 160));
        return;
    }
    if (intervalSecs < 2) intervalSecs = 2;
    if (intervalSecs > 30) intervalSecs = 30;
    {
        std::lock_guard<GptMutex> g(st->mtx);
        st->userCode = userCode;
        st->verifyUrl = string(kOaiAuthBase) + "/codex/device";
        st->status = 1;
    }

    //3. poll until approved, cancelled, or the 15-minute code expiry
    string authorizationCode, codeVerifier;
    long waitedMs = 0;
    const long kExpiryMs = 16L * 60L * 1000L;
    for (;;)
    {
        for (long s = 0; s < intervalSecs * 1000; s += 250)
        {
            oaiSleepMs(250);
            std::lock_guard<GptMutex> g(st->mtx);
            if (st->cancel)
                return; //silent: the user backed out on purpose
        }
        waitedMs += intervalSecs * 1000;
        if (waitedMs > kExpiryMs)
        {
            oaiFail(st, "the sign-in code expired (15 minutes) - start again");
            return;
        }
        nlohmann::json poll = {{"device_auth_id", deviceAuthId}, {"user_code", userCode}};
        body = httpRequestFull(string(kOaiAuthBase) + "/api/accounts/deviceauth/token",
                               poll.dump(), 30000, jsonHdr, code, respHeaders);
        if (code == 403 || code == 404)
            continue; //still waiting on the phone
        if (code != 200)
        {
            oaiFail(st, "sign-in poll failed (HTTP " + std::to_string(code) + "): " + body.substr(0, 160));
            return;
        }
        try
        {
            nlohmann::json j = nlohmann::json::parse(body);
            authorizationCode = j.value("authorization_code", "");
            codeVerifier = j.value("code_verifier", "");
        }
        catch (nlohmann::json::exception&)
        {
        }
        if (authorizationCode.empty() || codeVerifier.empty())
        {
            oaiFail(st, "approval reply was missing the exchange material: " + body.substr(0, 160));
            return;
        }
        break;
    }

    //4. PKCE exchange (form-encoded, device-flow redirect)
    string form = string("grant_type=authorization_code")
        + "&code=" + urlEncode(authorizationCode)
        + "&redirect_uri=" + urlEncode(string(kOaiAuthBase) + "/deviceauth/callback")
        + "&client_id=" + urlEncode(kCodexClientId)
        + "&code_verifier=" + urlEncode(codeVerifier);
    vector<string> formHdr;
    formHdr.push_back("Content-Type: application/x-www-form-urlencoded");
    body = httpRequestFull(string(kOaiAuthBase) + "/oauth/token", form, 30000, formHdr, code, respHeaders);
    if (code != 200)
    {
        oaiFail(st, "token exchange failed (HTTP " + std::to_string(code) + "): " + body.substr(0, 160));
        return;
    }
    nlohmann::json tokens;
    try
    {
        tokens = nlohmann::json::parse(body);
    }
    catch (nlohmann::json::exception&)
    {
        oaiFail(st, "token exchange returned unparseable JSON");
        return;
    }
    if (!tokens.contains("access_token"))
    {
        oaiFail(st, "token exchange reply had no access token");
        return;
    }

    //5. account id + plan from the id_token claims, then persist
    string accountId, plan;
    if (tokens.contains("id_token") && tokens["id_token"].is_string())
    {
        nlohmann::json claims = jwtPayload(tokens["id_token"].get<string>());
        if (claims.contains("https://api.openai.com/auth"))
        {
            const nlohmann::json& auth = claims["https://api.openai.com/auth"];
            accountId = auth.value("chatgpt_account_id", "");
            plan = auth.value("chatgpt_plan_type", "");
        }
    }
    if (accountId.empty())
    {
        oaiFail(st, "signed in, but the id token carried no chatgpt_account_id");
        return;
    }

    string root = gptUserRoot();
    if (root.empty())
    {
        oaiFail(st, "nowhere writable to store the login");
        return;
    }
    string dir = root;
    mkdir(dir.c_str(), 0755);
    dir += "/ai"; mkdir(dir.c_str(), 0755);
    dir += "/gpt"; mkdir(dir.c_str(), 0755);
    string path = dir + "/oai-auth.json";
    nlohmann::json doc = {
        {"tokens", tokens},
        {"minted_at", (long) time(NULL)},
        {"chatgpt_account_id", accountId},
    };
    {
        std::ofstream f(path.c_str(), std::ios::binary | std::ios::trunc);
        if (!f)
        {
            oaiFail(st, "could not write " + path);
            return;
        }
        f << doc.dump(1) << "\n";
    }
    chmod(path.c_str(), 0600);

    //Drop the in-memory auth cache: it may be remembering "no auth file" (or
    //the OLD login) from before this sign-in, and it is only ever read under
    //the same mutex.
    {
        std::lock_guard<GptMutex> g(codexMutex());
        CodexAuth& a = codexAuth();
        a.loaded = false;
        a.doc = nlohmann::json();
        a.path.clear();
        a.access.clear();
        a.refresh.clear();
        a.accountId.clear();
        a.mintedAt = 0;
        a.expiresIn = 0;
    }

    gptLogLine("signed in to ChatGPT (plan: " + (plan.empty() ? string("unknown") : plan) + ")");
    std::lock_guard<GptMutex> g(st->mtx);
    st->plan = plan;
    st->status = 2;
}
} //namespace

bool gptOaiSignInStart(std::shared_ptr<GptOaiSignIn> state)
{
    OaiSignInCtx * ctx = new OaiSignInCtx(); //plain new: the worker deletes it
    ctx->st = state;
    if (!gptSpawnWorker(&OaiSignInMain, ctx))
    {
        delete ctx;
        //The flow waits on a human for up to 15 minutes - it cannot run
        //synchronously on the game thread, so no thread means no sign-in.
        std::lock_guard<GptMutex> g(state->mtx);
        state->error = "this platform cannot run the sign-in in the background";
        state->status = 3;
        return false;
    }
    return true;
}
#else
bool gptOaiSignInStart(std::shared_ptr<GptOaiSignIn> state)
{
    std::lock_guard<GptMutex> g(state->mtx);
    state->error = "no TLS transport on this platform";
    state->status = 3;
    return false;
}
#endif //sign-in transport gate

namespace
{
//The name of a model in a listing reply. OpenAI-shaped servers use "id";
//Ollama-shaped ones use "name" (and sometimes "model"); some list bare strings.
string modelNameOf(const nlohmann::json& entry)
{
    if (entry.is_string())
        return entry.get<string>();
    if (!entry.is_object())
        return "";
    const char * keys[] = { "id", "name", "model" };
    for (size_t i = 0; i < 3; ++i)
        if (entry.contains(keys[i]) && entry[keys[i]].is_string())
            return entry[keys[i]].get<string>();
    return "";
}
} //namespace

bool gptProbeEndpoint(const string& url, const string& key, string& modelOut, long timeoutMs, const string& modelHint)
{
    modelOut.clear();
    //The Codex backend has no /v1/models. The honest probe is a minimal
    //completion: it proves auth, route and the SSE path end-to-end for a few
    //tokens (~25 total against a 7-day plan window). It runs against the
    //CONFIGURED model when one is set - probing a default here once masked a
    //stale foreign model id that would have failed every live decision.
    if (gptCodexEndpoint(url))
    {
        string why;
        if (!gptCodexAuthPresent(why))
        {
            gptLogLine("subscription probe: " + why);
            return false;
        }
        string model = modelHint.empty() ? string(kGptCodexDefaultModel) : modelHint;
        nlohmann::json ping = {
            {"model", model},
            {"instructions", "Reply with the word ok."},
            {"input", nlohmann::json::array({
                {{"type", "message"}, {"role", "user"},
                 {"content", nlohmann::json::array({{{"type", "input_text"}, {"text", "ok?"}}})}},
            })},
            {"reasoning", {{"effort", "none"}}},
            {"store", false},
            {"stream", true},
        };
        string err;
        string text = gptCodexComplete(url + "/responses", ping.dump(), timeoutMs, err);
        if (text.empty())
        {
            gptLogLine("subscription probe failed: " + err);
            return false;
        }
        modelOut = model;
        return true;
    }
    string body = gptHttpGet(url + "/v1/models", timeoutMs, key);
    if (body.empty())
        return false;
    try
    {
        nlohmann::json models = nlohmann::json::parse(body);
        //A usable reply carries a non-empty list of models; an auth error
        //({"error":"Unauthorized"}) parses fine but is not usable. The list
        //lives under "data" on OpenAI-shaped servers and under "models" on
        //Ollama-shaped ones (current llama-server emits both, "models" first),
        //so accept either rather than betting the probe on one local server's
        //choice of key. A listing entry names its model with "id", "name" or
        //"model" depending on the same lineage, and some servers list plain
        //strings.
        const char * arrays[] = { "data", "models" };
        for (size_t i = 0; i < 2; ++i)
        {
            if (!models.contains(arrays[i]) || !models[arrays[i]].is_array())
                continue;
            const nlohmann::json& list = models[arrays[i]];
            for (nlohmann::json::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                string name = modelNameOf(*it);
                if (name.size())
                {
                    modelOut = name;
                    return true;
                }
            }
        }
        return false;
    }
    catch (nlohmann::json::exception&)
    {
        return false;
    }
}

bool gptListModels(const string& url, const string& key, vector<string>& out, long timeoutMs)
{
    out.clear();
    //The Codex backend advertises nothing; its roster is the verified
    //constant list (see gptCodexModels).
    if (gptCodexEndpoint(url))
    {
        size_t n = 0;
        const char * const * roster = gptCodexModels(n);
        for (size_t i = 0; i < n; i++)
            out.push_back(roster[i]);
        return true;
    }
    string body = gptHttpGet(url + "/v1/models", timeoutMs, key);
    if (body.empty())
        return false;
    try
    {
        nlohmann::json models = nlohmann::json::parse(body);
        //Same two listing shapes as the probe above; here EVERY id is
        //collected, order preserved (a provider's ordering is often newest
        //or most-used first - information a re-sort would destroy).
        const char * arrays[] = { "data", "models" };
        for (size_t i = 0; i < 2 && out.empty(); ++i)
        {
            if (!models.contains(arrays[i]) || !models[arrays[i]].is_array())
                continue;
            const nlohmann::json& list = models[arrays[i]];
            for (nlohmann::json::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                string name = modelNameOf(*it);
                if (name.empty())
                    continue;
                bool dup = false;
                for (size_t k = 0; k < out.size() && !dup; k++)
                    dup = (out[k] == name);
                if (!dup)
                    out.push_back(name);
            }
        }
        return !out.empty();
    }
    catch (nlohmann::json::exception&)
    {
        return false;
    }
}

#endif //WITH_GPT_AI
