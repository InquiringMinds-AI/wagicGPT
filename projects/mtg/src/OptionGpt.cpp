#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "OptionGpt.h"
#include "GameOptions.h"
#include "WFont.h"
#include "WResourceManager.h"

#include <mutex> //std::lock_guard; the mutex itself is GptMutex (Vita seam)

namespace
{
//Right-aligned value cell, truncated from the left so the significant tail
//(ports, model suffixes, key endings) stays visible.
void drawValue(WFont * font, const string& value, float x, float y, float width)
{
    string shown = value;
    const size_t kMax = 42;
    if (shown.size() > kMax)
        shown = "..." + shown.substr(shown.size() - (kMax - 3));
    font->DrawString(shown.c_str(), x + width - 10, y + 3, JGETEXT_RIGHT);
}
} //namespace

//--- the tab ---------------------------------------------------------------

GptOptionsList::GptOptionsList()
    : WGuiList("GPT")
{
    cfg = GptSettings::load();
    if (cfg.urls.empty())
        cfg.urls.push_back("http://127.0.0.1:8080");
    if (cfg.thinking < 0) cfg.thinking = 0;
    if (cfg.maxTokens < 0) cfg.maxTokens = 0;
    loadedCfg = cfg;

    Add(NEW WGuiHeader("Language-Model Opponent"));
    Add(NEW OptionGptBool(&cfg.enabled, "LLM opponent", "Off (heuristic AI)", "On"));
    Add(NEW OptionGptPreset(&cfg));
    Add(NEW OptionGptText(&cfg.urls[0], "Endpoint URL"));
    Add(NEW OptionGptText(&cfg.model, "Model", "(auto-detect)"));
    //OpenRouter routing pin: comma-separated provider names, sent as
    //provider:{only:[...],allow_fallbacks:false}. Config-file-only until the
    //owner's ruling that provider control must be reachable from the couch.
    Add(NEW OptionGptText(&cfg.providerOnly, "Provider pin (OpenRouter)", "(any provider)"));
    Add(NEW OptionGptText(&cfg.key, "API key", "(none)", true));
    Add(NEW OptionGptBool(&cfg.thinking, "Thinking mode (stronger, slower)"));
    //Generous by design: the patience prompt is what bounds how long a PERSON
    //waits, so this only has to be long enough that "keep waiting" can still
    //land an answer, and short enough to eventually release a dead socket.
    Add(NEW OptionGptNumber(&cfg.timeoutSecs, "Call timeout (seconds)", 30, 600, 30));
    Add(NEW OptionGptNumber(&cfg.patienceSecs, "Ask after waiting (seconds)", 0, 300, 15, "Never"));
    Add(NEW OptionGptTest(&cfg));
    Add(NEW OptionGptConsent(&cfg.telemetry, "Contribute anonymized game data"));
    //Full disclosure for the consent above (and its save-time ask):
    WDecoStyled * disc = NEW WDecoStyled(NEW WGuiHeader("Telemetry shares only in-game decisions: board states, choices, model replies. Never keys or personal data."));
    disc->mStyle = WDecoStyled::DS_STYLE_ALERT;
    Add(disc);
    WDecoStyled * note = NEW WDecoStyled(NEW WGuiHeader("Applies to the next duel. Environment variables override."));
    note->mStyle = WDecoStyled::DS_STYLE_ALERT;
    Add(note);
}

void GptOptionsList::setData()
{
    WGuiList::setData();
    //Persist only when something actually changed, so opening the options
    //screen for unrelated settings never rewrites (and de-comments) a
    //hand-maintained config file.
    if (cfg != loadedCfg)
    {
        if (cfg.save())
            loadedCfg = cfg;
        else
            DebugTrace("OptionGpt: could not write the GPT config file");
    }
}

void GptOptionsList::Reload()
{
    GptSettings fresh = GptSettings::load();
    if (fresh.urls.empty())
        fresh.urls.push_back("http://127.0.0.1:8080");
    if (fresh.thinking < 0) fresh.thinking = 0;
    if (fresh.maxTokens < 0) fresh.maxTokens = 0;
    cfg = fresh;
    loadedCfg = fresh;
    WGuiList::Reload();
}

//--- rows ------------------------------------------------------------------

OptionGptBool::OptionGptBool(int * bind, string label, string offText, string onText)
    : WGuiItem(label), mBind(bind), mOff(offText), mOn(onText)
{
}

void OptionGptBool::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    drawValue(font, *mBind ? mOn : mOff, x, y, width);
}

void OptionGptBool::updateValue()
{
    *mBind = *mBind ? 0 : 1;
}

OptionGptNumber::OptionGptNumber(int * bind, string label, int mn, int mx, int step, string zeroText)
    : WGuiItem(label), mBind(bind), mMin(mn), mMax(mx), mStep(step), mZeroText(zeroText)
{
    if (*mBind < mMin && !(mZeroText.size() && *mBind == 0))
        *mBind = mMin;
    if (*mBind > mMax)
        *mBind = mMax;
}

void OptionGptNumber::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    char buf[32];
    if (mZeroText.size() && *mBind == 0)
        drawValue(font, mZeroText, x, y, width);
    else
    {
        sprintf(buf, "%d", *mBind);
        drawValue(font, buf, x, y, width);
    }
}

void OptionGptNumber::updateValue()
{
    *mBind += mStep;
    if (*mBind > mMax)
        *mBind = mZeroText.size() ? 0 : mMin;
    else if (*mBind < mMin && !(mZeroText.size() && *mBind == 0))
        *mBind = mMin;
}

OptionGptConsent::OptionGptConsent(int * bind, string label)
    : WGuiItem(label), mBind(bind)
{
}

void OptionGptConsent::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    const char * v = (*mBind < 0) ? "(not decided)" : (*mBind ? "Yes" : "No");
    drawValue(font, v, x, y, width);
}

void OptionGptConsent::updateValue()
{
    *mBind = (*mBind == 1) ? 0 : 1;
}

OptionGptText::OptionGptText(string * bind, string label, string emptyText, bool secret)
    : WGuiItem(label), mBind(bind), mEmptyText(emptyText), mSecret(secret)
{
}

void OptionGptText::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    if (mBind->empty())
        drawValue(font, mEmptyText, x, y, width);
    else if (mSecret)
    {
        //Identify-not-expose: the majority of the secret stays masked; the
        //trailing characters let the owner recognize WHICH key is set. Keys
        //too short for a safe tail render fully masked.
        string shown(MIN(mBind->size(), (size_t) 16), '*');
        if (mBind->size() > 12)
            shown += mBind->substr(mBind->size() - 4);
        drawValue(font, shown, x, y, width);
    }
    else
        drawValue(font, *mBind, x, y, width);
}

void OptionGptText::updateValue()
{
    //The on-screen keyboard edits the bound string in place: SimplePad
    //writes *dest on confirm (the New Profile / network address pattern).
    SimplePad * pad = options.keypadStart(*mBind, mBind);
    if (pad)
        pad->title = displayValue;
}

OptionGptPreset::OptionGptPreset(GptSettings * cfg)
    : WGuiItem("Provider preset"), mCfg(cfg)
{
}

void OptionGptPreset::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    size_t count = 0;
    const GptPreset * presets = gptPresets(count);
    drawValue(font, presets[gptPresetForUrl(mCfg->primaryUrl())].name, x, y, width);
}

void OptionGptPreset::updateValue()
{
    size_t count = 0;
    const GptPreset * presets = gptPresets(count);
    size_t next = (gptPresetForUrl(mCfg->primaryUrl()) + 1) % count;
    //"Custom" keeps whatever URL is entered; a named preset writes its URL.
    if (next != 0)
        mCfg->setPrimaryUrl(presets[next].url);
    else if (gptPresetForUrl(mCfg->primaryUrl()) == count - 1)
        mCfg->setPrimaryUrl(""); //wrap past the last preset into editable Custom
}

//--- test connection --------------------------------------------------------

struct OptionGptTest::ProbeState
{
    GptMutex mtx; //NOT std::mutex - a no-op on Vita, and a real worker
                  //without a real lock is a data race on this struct
    int status; //0 idle, 1 running, 2 done
    string result;
    ProbeState() : status(0) {}
};

namespace
{
//Context handed to the probe worker; owned by ProbeMain (deleted there).
struct ProbeCtx
{
    std::shared_ptr<OptionGptTest::ProbeState> state;
    string url;
    string key;
};

void ProbeMain(void * p)
{
    ProbeCtx * ctx = static_cast<ProbeCtx *>(p);
    string model;
    bool ok = gptProbeEndpoint(ctx->url, ctx->key, model, 6000);
    {
        std::lock_guard<GptMutex> g(ctx->state->mtx);
        ctx->state->status = 2;
        ctx->state->result = ok ? ("OK - serving " + model) : "unreachable / no usable reply";
    }
    delete ctx;
}
} //namespace

OptionGptTest::OptionGptTest(GptSettings * cfg)
    : WGuiItem("Test connection"), mCfg(cfg), mProbe(std::make_shared<ProbeState>())
{
}

void OptionGptTest::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    string status;
    {
        std::lock_guard<GptMutex> g(mProbe->mtx);
        status = (mProbe->status == 1) ? "testing..." : mProbe->result;
    }
    if (status.empty())
        status = "(press to probe the endpoint)";
    drawValue(font, status, x, y, width);
}

void OptionGptTest::updateValue()
{
    {
        std::lock_guard<GptMutex> g(mProbe->mtx);
        if (mProbe->status == 1)
            return; //already running
        mProbe->status = 1;
        mProbe->result.clear();
    }
    //gptSpawnWorker, NOT std::thread: this was the site the Vita seam port
    //missed - std::thread construction throws there, so "Test connection"
    //reported "cannot test: no worker thread" on the one platform where the
    //user most needs to check an endpoint from the couch.
    ProbeCtx * ctx = new ProbeCtx(); //plain new: ProbeMain deletes it, possibly on the worker
    ctx->state = mProbe;
    ctx->url = mCfg->primaryUrl();
    ctx->key = mCfg->key;
    if (!gptSpawnWorker(&ProbeMain, ctx))
    {
        //No worker thread on this platform: probe synchronously. The options
        //screen stalls for up to the 6s probe timeout, but it comes back with
        //a real answer - better than refusing to test at all.
        ProbeMain(ctx);
    }
}

#endif //WITH_GPT_AI
