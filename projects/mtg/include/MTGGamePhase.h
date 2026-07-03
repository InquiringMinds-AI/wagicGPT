#ifndef _MTGGAMEPHASE_H_
#define _MTGGAMEPHASE_H_

#include "ActionElement.h"
#include "GameObserver.h"

#include <JGui.h>
#include "WFont.h"

class MTGGamePhase: public ActionElement
{
protected:
    float animation;
    int currentState;
    WFont * mFont;
    GameObserver* observer;
    //Pass-turn ("go"): while >= 0, auto-advance phases whenever advancing
    //is legal until observer->turn moves past this value. Armed by HOLDING
    //the phase-advance control ~0.7s (tap = one phase, hold = the rest of
    //the turn); tapping it again cancels.
    int mPassTurnUntil;
    float mTriggerHeld;

public:
    MTGGamePhase(GameObserver* g, int id);
    virtual void Update(float dt);
    virtual void Render();
    bool CheckUserInput(JButton key);
    virtual MTGGamePhase * clone() const;
    virtual ostream& toString(ostream& out) const;
    bool NextGamePhase();
};

#endif
