#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "OptionGpt.h"
#include "GameOptions.h"
#include "WFont.h"
#include "WResourceManager.h"

#include <thread>
#include <mutex>

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
    : WGuiList("GPT"), showKey(0)
{
    cfg = GptSettings::load();
    if (cfg.urls.empty())
        cfg.urls.push_back("http://127.0.0.1:8080");
    if (cfg.thinking < 0) cfg.thinking = 0;
    if (cfg.hints < 0) cfg.hints = 0;
    if (cfg.maxTokens < 0) cfg.maxTokens = 0;
    loadedCfg = cfg;

    Add(NEW WGuiHeader("Language-Model Opponent"));
    Add(NEW OptionGptBool(&cfg.enabled, "LLM opponent", "Off (heuristic AI)", "On"));
    Add(NEW OptionGptPreset(&cfg));
    Add(NEW OptionGptText(&cfg.urls[0], "Endpoint URL"));
    Add(NEW OptionGptText(&cfg.model, "Model", "(auto-detect)"));
    Add(NEW OptionGptText(&cfg.key, "API key", "(none)", &showKey));
    Add(NEW OptionGptBool(&showKey, "Show API key"));
    Add(NEW OptionGptBool(&cfg.thinking, "Thinking mode (stronger, slower)"));
    Add(NEW OptionGptBool(&cfg.hints, "Heuristic hints in prompt"));
    Add(NEW OptionGptNumber(&cfg.timeoutSecs, "Call timeout (seconds)", 15, 300, 15));
    Add(NEW OptionGptTest(&cfg));
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
    if (fresh.hints < 0) fresh.hints = 0;
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

OptionGptText::OptionGptText(string * bind, string label, string emptyText, int * maskOff)
    : WGuiItem(label), mBind(bind), mEmptyText(emptyText), mMaskOff(maskOff)
{
}

void OptionGptText::Render()
{
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::OPTION_FONT);
    font->SetColor(getColor(WGuiColor::TEXT));
    font->DrawString(_(displayValue).c_str(), x + 2, y + 3);
    if (mBind->empty())
        drawValue(font, mEmptyText, x, y, width);
    else if (mMaskOff && !*mMaskOff)
        drawValue(font, string(MIN(mBind->size(), (size_t) 24), '*'), x, y, width);
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
    std::mutex mtx;
    int status; //0 idle, 1 running, 2 done
    string result;
    ProbeState() : status(0) {}
};

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
        std::lock_guard<std::mutex> g(mProbe->mtx);
        status = (mProbe->status == 1) ? "testing..." : mProbe->result;
    }
    if (status.empty())
        status = "(press to probe the endpoint)";
    drawValue(font, status, x, y, width);
}

void OptionGptTest::updateValue()
{
    {
        std::lock_guard<std::mutex> g(mProbe->mtx);
        if (mProbe->status == 1)
            return; //already running
        mProbe->status = 1;
        mProbe->result.clear();
    }
    string url = mCfg->primaryUrl();
    string key = mCfg->key;
    std::shared_ptr<ProbeState> state = mProbe;
    std::thread([state, url, key]() {
        string model;
        bool ok = gptProbeEndpoint(url, key, model, 6000);
        std::lock_guard<std::mutex> g(state->mtx);
        state->status = 2;
        state->result = ok ? ("OK - serving " + model) : "unreachable / no usable reply";
    }).detach();
}

#endif //WITH_GPT_AI
