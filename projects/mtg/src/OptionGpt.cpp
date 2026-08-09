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
    modelPickerWanted = false;
    signInWanted = false;
    cfg = GptSettings::load();
    if (cfg.urls.empty())
#if defined(PSP)
        //127.0.0.1 is the PSP itself; the endpoint is always a LAN bridge the
        //user types in, so seed the row empty rather than with a self-address.
        cfg.urls.push_back("");
#else
        cfg.urls.push_back("http://127.0.0.1:8080");
#endif
    if (cfg.thinking < 0) cfg.thinking = 0;
    if (cfg.maxTokens < 0) cfg.maxTokens = 0;
    loadedCfg = cfg;

    Add(NEW WGuiHeader("Language-Model Opponent"));
    Add(NEW OptionGptBool(&cfg.enabled, "LLM opponent", "Off (heuristic AI)", "On"));
    Add(NEW OptionGptPreset(&cfg));
    Add(NEW OptionGptText(&cfg.urls[0], "Endpoint URL"));
    Add(NEW OptionGptModel(this));
    //OpenRouter routing pin: comma-separated provider names, sent as
    //provider:{only:[...],allow_fallbacks:false}. Config-file-only until the
    //owner's ruling that provider control must be reachable from the couch.
#if !defined(PSP)
    //Both rows govern providers the PSP transport can never reach (no TLS):
    //the OpenRouter routing pin and the ChatGPT-subscription sign-in.
    Add(NEW OptionGptTextUnlessCodex(&cfg, &cfg.providerOnly, "Provider pin (OpenRouter)", "(any provider)"));
#endif
    Add(NEW OptionGptTextUnlessCodex(&cfg, &cfg.key, "API key", "(none)", true));
#if !defined(PSP)
    Add(NEW OptionGptSignIn(this));
#endif
    Add(NEW OptionGptReasoning(&cfg));
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
    //URLs and API keys need digits and punctuation - show the numpad row.
    SimplePad * pad = options.keypadStart(*mBind, mBind, true, true);
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
    //A single-entry table (PSP: Custom only) has nothing to cycle to, and the
    //wrap branch below would blank the user's typed URL on every press.
    if (count <= 1)
        return;
    bool wasCodex = gptCodexEndpoint(mCfg->primaryUrl());
    size_t next = (gptPresetForUrl(mCfg->primaryUrl()) + 1) % count;
    //"Custom" keeps whatever URL is entered; a named preset writes its URL.
    if (next != 0)
        mCfg->setPrimaryUrl(presets[next].url);
    else if (gptPresetForUrl(mCfg->primaryUrl()) == count - 1)
        mCfg->setPrimaryUrl(""); //wrap past the last preset into editable Custom

    //Crossing the subscription boundary invalidates the model id: the codex
    //roster exists nowhere else, and a foreign id (deepseek/..., a local
    //gguf name) fails every live decision there while the probe - which
    //would use the default - reports OK. Seen live on the Vita: an
    //OpenRouter model id rode a preset switch and only the probe's default
    //masked it. Clearing to auto-detect is the honest reset; entering the
    //codex preset auto-detect means the default model.
    bool isCodex = gptCodexEndpoint(mCfg->primaryUrl());
    if (wasCodex != isCodex)
    {
        bool inRoster = false;
        size_t n = 0;
        const char * const * roster = gptCodexModels(n);
        for (size_t i = 0; i < n; i++)
            if (mCfg->model == roster[i])
                inRoster = true;
        if ((isCodex && !inRoster) || (wasCodex && inRoster))
            mCfg->model.clear();
    }
}

//--- model row (preset-aware) -----------------------------------------------

OptionGptModel::OptionGptModel(GptOptionsList * list)
    : OptionGptText(&list->cfg.model, "Model", "(auto-detect)"), mList(list), mCfg(&list->cfg)
{
}

void OptionGptModel::Render()
{
    if (!gptCodexEndpoint(mCfg->primaryUrl()))
    {
        OptionGptText::Render();
        return;
    }
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    drawValue(font, mCfg->model.empty() ? string("(") + kGptCodexDefaultModel + ")"
                                        : mCfg->model, x, y, width);
}

void OptionGptModel::updateValue()
{
    //The screen owns the picker (modal menus are screen-level UI); this row
    //only raises its hand. The picker polls /v1/models where a listing
    //exists, shows the verified roster on the subscription preset, and
    //keeps manual keyboard entry as an item for endpoints that misreport.
    mList->modelPickerWanted = true;
}

//--- reasoning row (preset-aware) --------------------------------------------

OptionGptReasoning::OptionGptReasoning(GptSettings * cfg)
    : WGuiItem("Thinking mode (stronger, slower)"), mCfg(cfg)
{
}

void OptionGptReasoning::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    if (gptCodexEndpoint(mCfg->primaryUrl()))
    {
        font->DrawString(_("Reasoning effort").c_str(), x + 2, y + 3);
        drawValue(font, mCfg->reasoningEffort.empty() ? "(low)" : mCfg->reasoningEffort,
                  x, y, width);
        return;
    }
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    drawValue(font, mCfg->thinking ? "On" : "Off", x, y, width);
}

void OptionGptReasoning::updateValue()
{
    if (!gptCodexEndpoint(mCfg->primaryUrl()))
    {
        mCfg->thinking = mCfg->thinking ? 0 : 1;
        return;
    }
    //The server's own tier enumeration, plus the empty default slot. All six
    //are offered even where the latency cost is severe - the plan window and
    //the wait are the user's to spend.
    static const char * const kTiers[] = { "none", "low", "medium", "high", "xhigh", "max" };
    const size_t n = sizeof(kTiers) / sizeof(kTiers[0]);
    size_t at = n;
    for (size_t i = 0; i < n; i++)
        if (mCfg->reasoningEffort == kTiers[i])
            at = i;
    size_t next = (at + 1) % (n + 1);
    mCfg->reasoningEffort = (next == n) ? "" : kTiers[next];
}

//--- sign-in row (subscription preset) ---------------------------------------

OptionGptSignIn::OptionGptSignIn(GptOptionsList * list)
    : WGuiItem("ChatGPT sign-in"), mList(list)
{
}

void OptionGptSignIn::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    if (!gptCodexEndpoint(mList->cfg.primaryUrl()))
    {
        drawValue(font, "(subscription preset only)", x, y, width);
        return;
    }
    //Presence, not validity: an expired login still refreshes itself on the
    //next call, so "signed in" is the honest summary of having material.
    string why;
    drawValue(font, gptCodexAuthPresent(why) ? "signed in (press to sign in again)"
                                             : "press to sign in", x, y, width);
}

void OptionGptSignIn::updateValue()
{
    if (gptCodexEndpoint(mList->cfg.primaryUrl()))
        mList->signInWanted = true;
}

//--- rows a preset does not consume ------------------------------------------

OptionGptTextUnlessCodex::OptionGptTextUnlessCodex(GptSettings * cfg, string * bind,
                                                   string label, string emptyText, bool secret)
    : OptionGptText(bind, label, emptyText, secret), mCfg(cfg)
{
}

void OptionGptTextUnlessCodex::Render()
{
    if (!gptCodexEndpoint(mCfg->primaryUrl()))
    {
        OptionGptText::Render();
        return;
    }
    //The subscription preset authenticates from the device-code token file
    //and routes first-party - neither an API key nor an OpenRouter provider
    //pin ever reaches the wire. Saying so beats hiding the row or letting an
    //edit pretend to matter.
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    drawValue(font, "(not used with this preset)", x, y, width);
}

void OptionGptTextUnlessCodex::updateValue()
{
    if (!gptCodexEndpoint(mCfg->primaryUrl()))
        OptionGptText::updateValue();
    //under the codex preset the row is informational; no keyboard
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
    string modelHint; //the configured model - the probe must test what a
                      //duel would use, or it masks a bad model id
};

void ProbeMain(void * p)
{
    ProbeCtx * ctx = static_cast<ProbeCtx *>(p);
    string model;
    bool ok = gptProbeEndpoint(ctx->url, ctx->key, model, 6000, ctx->modelHint);
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
    ctx->modelHint = mCfg->model;
    if (!gptSpawnWorker(&ProbeMain, ctx))
    {
        //No worker thread on this platform: probe synchronously. The options
        //screen stalls for up to the 6s probe timeout, but it comes back with
        //a real answer - better than refusing to test at all.
        ProbeMain(ctx);
    }
}

#endif //WITH_GPT_AI
