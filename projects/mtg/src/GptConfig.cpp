#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "GptConfig.h"
#include "JFileSystem.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstring>

using std::string;
using std::vector;

GptSettings::GptSettings()
    : enabled(0), thinking(-1), hints(-1), maxTokens(-1), timeoutSecs(120), translog(0), telemetry(-1), peek(0)
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

string saltPath()
{
    const char * home = getenv("HOME");
    if (!home)
        return "";
    return string(home) + "/.Wagic/ai/gpt/keysalt";
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
    const char * home = getenv("HOME");
    if (home)
    {
        string dir = string(home) + "/.Wagic";
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

string gptReadAsset(const char * filename)
{
    if (const char * home = getenv("HOME"))
    {
        string path = string(home) + "/.Wagic/ai/gpt/" + filename;
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
        else if (k == "hints") cfg.hints = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "maxtokens") cfg.maxTokens = atol(v.c_str());
        else if (k == "timeout") cfg.timeoutSecs = atoi(v.c_str());
        else if (k == "translog") cfg.translog = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "telemetry") cfg.telemetry = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "peek") cfg.peek = (v != "0" && v != "off") ? 1 : 0;
    }
    if (cfg.timeoutSecs < 5)
        cfg.timeoutSecs = 5;
    cfg.key = deobfuscateKey(cfg.key);
    return cfg;
}

bool GptSettings::save() const
{
    const char * home = getenv("HOME");
    if (!home)
        return false;
    string dir = string(home) + "/.Wagic";
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
    if (hints >= 0)
        f << "hints=" << hints << "\n";
    if (maxTokens > 0)
        f << "maxtokens=" << maxTokens << "\n";
    f << "timeout=" << timeoutSecs << "\n";
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
        && thinking == o.thinking && hints == o.hints && maxTokens == o.maxTokens
        && timeoutSecs == o.timeoutSecs && translog == o.translog && telemetry == o.telemetry
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
} //namespace

string gptHttpGet(const string& url, long timeoutMs, const string& bearer)
{
    return httpRequestImpl(url, "", timeoutMs, bearer);
}

string gptHttpPost(const string& url, const string& body, long timeoutMs, const string& bearer)
{
    return httpRequestImpl(url, body, timeoutMs, bearer);
}

bool gptProbeEndpoint(const string& url, const string& key, string& modelOut, long timeoutMs)
{
    modelOut.clear();
    string body = gptHttpGet(url + "/v1/models", timeoutMs, key);
    if (body.empty())
        return false;
    try
    {
        nlohmann::json models = nlohmann::json::parse(body);
        //A real /v1/models reply carries a non-empty "data" array; an auth
        //error ({"error":"Unauthorized"}) parses fine but is not usable.
        if (!models.contains("data") || !models["data"].is_array() || models["data"].empty())
            return false;
        modelOut = models["data"][0]["id"].get<string>();
        return true;
    }
    catch (nlohmann::json::exception&)
    {
        return false;
    }
}

#endif //WITH_GPT_AI
