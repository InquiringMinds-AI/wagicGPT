#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "GptConfig.h"
#include "JFileSystem.h"

#ifndef WAGIC_NO_CURL
#include <curl/curl.h>
#endif
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstring>

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
//The game appends /v1/models and /v1/chat/completions.
static const GptPreset kPresets[] = {
    { "Custom", "" },
    { "Local llama.cpp", "http://127.0.0.1:8080" },
    { "Local Ollama", "http://127.0.0.1:11434" },
    { "LM Studio", "http://127.0.0.1:1234" },
    { "OpenRouter", "https://openrouter.ai/api" },
    { "OpenAI", "https://api.openai.com" },
    { "Anthropic", "https://api.anthropic.com" },
    { "Groq", "https://api.groq.com/openai" },
    { "Mistral", "https://api.mistral.ai" },
    { "DeepSeek", "https://api.deepseek.com" },
    { "xAI", "https://api.x.ai" },
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

string httpRequestImpl(const string& url, const string& postBody, long timeoutMs, const string& bearer)
{
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

string gptHttpGet(const string& url, long timeoutMs, const string& bearer)
{
    return httpRequestImpl(url, "", timeoutMs, bearer);
}

string gptHttpPost(const string& url, const string& body, long timeoutMs, const string& bearer)
{
    return httpRequestImpl(url, body, timeoutMs, bearer);
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

bool gptProbeEndpoint(const string& url, const string& key, string& modelOut, long timeoutMs)
{
    modelOut.clear();
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

#endif //WITH_GPT_AI
