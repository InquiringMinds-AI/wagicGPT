/*
 *  Wagic, The Homebrew ?! is licensed under the BSD license
 *  See LICENSE in the Folder's root
 *
 *  The "GPT" options tab: in-game configuration for the LLM opponent (and
 *  the future home of other transformer-backed features). The tab edits a
 *  working copy of GptSettings and, on Save, writes the user config file
 *  ($HOME/.Wagic/ai/gpt/endpoints.txt) - the same file hand editors use,
 *  so the GUI and config-as-code stay coherent. Nothing is written when
 *  nothing changed, so hand-maintained comments survive visits to other
 *  option tabs.
 */

#ifndef _OPTIONGPT_H_
#define _OPTIONGPT_H_

#ifdef WITH_GPT_AI

#include <JGui.h>
#include <vector>
#include <string>
#include "GameStateOptions.h"
#include "WFilter.h"
#include "WDataSrc.h"
#include "WGui.h"
#include "GptConfig.h"
#include <memory>

class GptOptionsList: public WGuiList
{
public:
    GptOptionsList();
    virtual void setData();  //children write into cfg; then persist if changed
    virtual void Reload();

    GptSettings cfg;     //working copy the rows bind to
    GptSettings loadedCfg; //snapshot for only-write-when-changed

    //Set by the Model row; consumed by GameStateOptions, which owns the
    //modal picker menus (screen-level UI lives at the screen, like the
    //telemetry consent ask).
    bool modelPickerWanted;

    //Telemetry consent negotiation: asked once, on saving with a newly
    //set-up endpoint, never re-asked after a decision. The modal itself is
    //owned by GameStateOptions (screen-level menus live there).
    bool wantsTelemetryConsent() const
    {
        return cfg.enabled && cfg.telemetry < 0
            && (cfg.urls != loadedCfg.urls || !loadedCfg.enabled);
    }
    void setTelemetryConsent(int decision)
    {
        cfg.telemetry = decision;
    }
};

//Tri-state consent row: "not decided" until the user answers (or clicks it),
//then Yes/No, freely changeable afterwards.
class OptionGptConsent: public WGuiItem
{
public:
    OptionGptConsent(int * bind, string label);
    virtual void Render();
    virtual void updateValue();

protected:
    int * mBind;
};

//On/off row bound to an int flag.
class OptionGptBool: public WGuiItem
{
public:
    OptionGptBool(int * bind, string label, string offText = "Off", string onText = "On");
    virtual void Render();
    virtual void updateValue();

protected:
    int * mBind;
    string mOff, mOn;
};

//Numeric row bound to an int (wraps from max back to min).
class OptionGptNumber: public WGuiItem
{
public:
    OptionGptNumber(int * bind, string label, int mn, int mx, int step, string zeroText = "");
    virtual void Render();
    virtual void updateValue();

protected:
    int * mBind;
    int mMin, mMax, mStep;
    string mZeroText; //what to display for 0 (e.g. "Auto")
};

//Free-text row: OK opens the on-screen keyboard editing the bound string
//in place. Secrets render masked except a short identifying tail - the
//full value is visible only while it is being typed (the keyboard shows
//its buffer in clear); once confirmed it can never be displayed again.
class OptionGptText: public WGuiItem
{
public:
    OptionGptText(string * bind, string label, string emptyText = "", bool secret = false);
    virtual void Render();
    virtual void updateValue();

protected:
    string * mBind;
    string mEmptyText;
    bool mSecret;
};

//Provider preset row: cycles the curated endpoint list, writing the URL
//into the settings; shows "Custom" when the URL matches no preset. Crossing
//the subscription-preset boundary also clears a model id that does not
//belong on the new provider - a carried-over foreign id passes no probe but
//would fail every live decision.
class OptionGptPreset: public WGuiItem
{
public:
    OptionGptPreset(GptSettings * cfg);
    virtual void Render();
    virtual void updateValue();

protected:
    GptSettings * mCfg;
};

//Model row: pressing it asks the screen for the model picker - a polled
///v1/models listing presented vendor-first (the subscription preset lists
//its verified static roster instead). Manual keyboard entry survives as a
//picker item, not as this row's default.
class OptionGptModel: public OptionGptText
{
public:
    OptionGptModel(GptOptionsList * list);
    virtual void Render();
    virtual void updateValue();

protected:
    GptOptionsList * mList;
    GptSettings * mCfg;
};

//Reasoning row, preset-aware: the subscription backend takes an explicit
//effort tier (the server's own set), every other provider family a boolean
//thinking toggle. One knob, rendered in whichever vocabulary the current
//provider actually speaks.
class OptionGptReasoning: public WGuiItem
{
public:
    OptionGptReasoning(GptSettings * cfg);
    virtual void Render();
    virtual void updateValue();

protected:
    GptSettings * mCfg;
};

//Free-text row that a given preset does not consume: renders its value
//greyed to "(not used with this preset)" and refuses to open the keyboard
//there, instead of pretending the field means something.
class OptionGptTextUnlessCodex: public OptionGptText
{
public:
    OptionGptTextUnlessCodex(GptSettings * cfg, string * bind, string label,
                             string emptyText = "", bool secret = false);
    virtual void Render();
    virtual void updateValue();

protected:
    GptSettings * mCfg;
};

//"Test connection": probes the configured URL/key on a worker thread and
//renders the outcome inline.
class OptionGptTest: public WGuiItem
{
public:
    OptionGptTest(GptSettings * cfg);
    virtual void Render();
    virtual void updateValue();

    struct ProbeState;

protected:
    GptSettings * mCfg;
    std::shared_ptr<ProbeState> mProbe;
};

#endif //WITH_GPT_AI

#endif //_OPTIONGPT_H_
