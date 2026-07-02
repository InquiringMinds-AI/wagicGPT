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
//into the settings; shows "Custom" when the URL matches no preset.
class OptionGptPreset: public WGuiItem
{
public:
    OptionGptPreset(GptSettings * cfg);
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
