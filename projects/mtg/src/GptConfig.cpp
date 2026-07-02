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

using std::string;
using std::vector;

GptSettings::GptSettings()
    : enabled(0), thinking(-1), hints(-1), maxTokens(-1), timeoutSecs(120)
{
}

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
    }
    if (cfg.timeoutSecs < 5)
        cfg.timeoutSecs = 5;
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
        f << "key=" << key << "\n";
    if (thinking >= 0)
        f << "thinking=" << thinking << "\n";
    if (hints >= 0)
        f << "hints=" << hints << "\n";
    if (maxTokens > 0)
        f << "maxtokens=" << maxTokens << "\n";
    f << "timeout=" << timeoutSecs << "\n";
    return f.good();
}

bool GptSettings::operator==(const GptSettings& o) const
{
    return enabled == o.enabled && urls == o.urls && model == o.model && key == o.key
        && thinking == o.thinking && hints == o.hints && maxTokens == o.maxTokens
        && timeoutSecs == o.timeoutSecs;
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
