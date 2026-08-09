#ifndef _GAME_STATE_OPTIONS_H_
#define _GAME_STATE_OPTIONS_H_

#include <JGE.h>
#include <JGui.h>
#include "GameState.h"
#include <memory>
#include <string>
#include <vector>

class GameApp;
class WGuiTabMenu;
class WGuiHeader;
class SimpleMenu;
class SimplePad;
#ifdef WITH_GPT_AI
class GptOptionsList;
#endif

struct KeybGrabber
{
    virtual void KeyPressed(LocalKeySym) = 0;
};

class GameStateOptions: public GameState, public JGuiListener
{
private:
    enum
    {
        SHOW_OPTIONS,
        SHOW_OPTIONS_MENU,
        SAVE,
        SHOW_TELEMETRY_CONSENT,
        SHOW_MODEL_PICKER,
        SHOW_OAI_SIGNIN
    };
    float timer;
    bool mReload;
    KeybGrabber* grabber;
    //Label of the "Reset tutorial messages" action row. Owned by the button
    //that decorates it (and so by optionsTabs); held only to swap the text for
    //an acknowledgement, and dropped in End() with the rest of the tab tree.
    WGuiHeader * mResetTutorialsLabel;
#ifdef WITH_GPT_AI
    //Telemetry consent: asked once, on saving with a newly set-up endpoint
    //(never re-asked after a decision). Owned by this screen because modal
    //menus live at the screen level.
    GptOptionsList * gptTab;
    SimpleMenu * telemetryMenu;

    //Model picker: raised by the GPT tab's Model row; the model list is
    //polled from the endpoint on a worker thread, then presented vendor-first
    //(the id prefix before '/') so a 300-model catalog stays navigable on a
    //d-pad. Menus are (re)built only from Update - never from their own
    //ButtonPressed (the codebase's delete-in-callback UAF trap).
    std::shared_ptr<struct GptModelFetch> modelFetch;
    SimpleMenu * modelPickerMenu;
    std::vector<std::string> pickerModels;               //fetched ids, provider order
    std::vector<std::string> pickerVendors;              //bucket labels
    std::vector<std::vector<int> > pickerVendorModels;   //bucket -> indexes into pickerModels
    int pickerPhase;      //fetching / vendor list / model list
    int pickerBuildFor;   //menu to (re)build next Update; kPickerBuildNone when current
    int pickerVendorAt;   //bucket the model list is showing
    void startModelPicker();
    void updateModelPicker(float dt);
    void buildPickerIndex();
    void buildPickerMenu();

    //In-client ChatGPT sign-in: full-panel QR + user code + poll status.
    //The worker owns its state via shared_ptr (leaving the screen mid-poll
    //just sets cancel and walks away).
    std::shared_ptr<struct GptOaiSignIn> oaiSignIn;
    void startOaiSignIn();
    void updateOaiSignIn(float dt);
    void renderOaiSignIn();
#endif

public:
    SimpleMenu * optionsMenu;
    WGuiTabMenu * optionsTabs;
    int mState;

    GameStateOptions(GameApp* parent);
    virtual ~GameStateOptions();

    virtual void Start();
    virtual void End();
    virtual void Update(float dt);
    virtual void Render();
    virtual void GrabKeyboard(KeybGrabber*);
    virtual void UngrabKeyboard(const KeybGrabber*);
    void ButtonPressed(int controllerId, int ControlId);

    string newProfile;

};

#endif
