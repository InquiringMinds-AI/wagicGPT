#ifndef _GAME_STATE_OPTIONS_H_
#define _GAME_STATE_OPTIONS_H_

#include <JGE.h>
#include <JGui.h>
#include "GameState.h"

class GameApp;
class WGuiTabMenu;
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
        SHOW_TELEMETRY_CONSENT
    };
    float timer;
    bool mReload;
    KeybGrabber* grabber;
#ifdef WITH_GPT_AI
    //Telemetry consent: asked once, on saving with a newly set-up endpoint
    //(never re-asked after a decision). Owned by this screen because modal
    //menus live at the screen level.
    GptOptionsList * gptTab;
    SimpleMenu * telemetryMenu;
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
