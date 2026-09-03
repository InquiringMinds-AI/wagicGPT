/*
*  Wagic, The Homebrew ?! is licensed under the BSD license
*  See LICENSE in the Folder's root
*  http://wololo.net/wagic/
*/

#ifndef _SPELLSTACK_H_
#define _SPELLSTACK_H_

#define MAX_SPELL_TARGETS 10


#define ACTION_SPELL 10
#define ACTION_DAMAGE 11
#define ACTION_DAMAGES 12
#define ACTION_NEXTGAMEPHASE 13
#define ACTION_DRAW 14
#define ACTION_PUTINGRAVEYARD 15
#define ACTION_ABILITY 16

#define NOT_RESOLVED -2
#define RESOLVED_OK 1
#define RESOLVED_NOK -1

#include "PlayGuiObject.h"
#include "GuiLayers.h"
#include "TargetsList.h"
#include "Targetable.h"

#include "WResource_Fwd.h"

class GuiLayer;
class PlayGuiObject;
class MTGCardInstance;
class GameObserver;
class Player;
class Damageable;
class MTGAbility;
class Targetable;
class DamageStack;
class ManaCost;
class TargetChooser;

class Interruptible: public PlayGuiObject, public Targetable
{
public:
    int state, display;
    MTGCardInstance * source;
    virtual void Entering()
    {
        mHasFocus = true;
    }
    
    virtual bool Leaving(JButton)
    {
        mHasFocus = false;
        return true;
    }

    virtual bool ButtonPressed()
    {
        return true;
    }

    virtual int resolve()
    {
        return 0;
    }
    
    virtual void Render()
    {
    }
    
    Interruptible(GameObserver* observer, int inID = 0, bool hasFocus = false)
        : PlayGuiObject(40, 0.0f, 0.0f, inID, hasFocus), Targetable(observer), state(NOT_RESOLVED), display(0), source(NULL)
    {
    }
    
    virtual const string getDisplayName() const;
    //#W57-G (D42): targetCounts is parallel to targetIcons. When several of a
    //stack entry's targets are state-identical members of one board pile the
    //caller collapses them to ONE icon carrying their count - identical members
    //are interchangeable, so only the count is information. Empty = one each.
    void Render(MTGCardInstance * source, JQuad * targetQuad, string alt1, string alt2, string action, bool bigQuad = false, int aType = 0, vector<JQuadPtr> targetIcons = vector<JQuadPtr>(), vector<int> targetCounts = vector<int>());
    
    virtual int receiveEvent(WEvent *)
    {
        return 0;
    }

#if defined (WIN32) || defined (LINUX) || defined (IOS)
    virtual void Dump();
#endif

protected:
    float GetVerticalTextOffset() const;
};

class NextGamePhase: public Interruptible
{
public:
    int resolve();
    bool extraDamagePhase();
    void Render();
    virtual ostream& toString(ostream& out) const;
    virtual const string getDisplayName() const;
    NextGamePhase(GameObserver* observer, int id);
};

class Spell: public Interruptible
{
protected:

public:
    MTGGameZone * from;
    TargetChooser * tc;
    ManaCost * cost;
    int payResult;
    int computeX(MTGCardInstance * card);
    Spell(GameObserver* observer, MTGCardInstance* _source);
    Spell(GameObserver* observer, int id, MTGCardInstance* _source, TargetChooser *_tc, ManaCost * _cost, int payResult);
    ~Spell();
    int resolve();
    void Render();
    bool FullfilledAlternateCost(const int &costType);
    const string getDisplayName() const;
    virtual ostream& toString(ostream& out) const;
    MTGCardInstance * getNextCardTarget(MTGCardInstance * previous = 0);
    Player * getNextPlayerTarget(Player * previous = 0);
    Damageable * getNextDamageableTarget(Damageable * previous = 0);
    Interruptible * getNextInterruptible(Interruptible * previous, int type);
    Spell * getNextSpellTarget(Spell * previous = 0);
    Damage * getNextDamageTarget(Damage * previous = 0);
    Targetable * getNextTarget(Targetable * previous = 0);
    int getNbTargets();
};

class StackAbility: public Interruptible
{
public:
    MTGAbility * ability;
    int resolve();
    void Render();
    virtual ostream& toString(ostream& out) const;
    virtual const string getDisplayName() const;
    StackAbility(GameObserver* observer, int id, MTGAbility * _ability);
};

class PutInGraveyard: public Interruptible {
public:
    MTGCardInstance * card;
    int removeFromGame;
    int resolve();
    void Render();
    virtual ostream& toString(ostream& out) const;
    PutInGraveyard(GameObserver* observer, int id, MTGCardInstance * _card);
};


class DrawAction: public Interruptible
{
public:
    int nbcards;
    Player * player;
    int resolve();
    void Render();
    virtual ostream& toString(ostream& out) const;
    DrawAction(GameObserver* observer, int id, Player *  _player, int _nbcards);
};

class ATutorialMessage;
class LifeAction: public Interruptible
{
public:
    int amount;
    Damageable * target;
    int resolve();
    void Render();
    virtual ostream& toString(ostream& out) const;
    LifeAction(GameObserver* observer, int id, Damageable * _target, int amount);
};

class ActionStack :public GuiLayer
{
public:
    enum ActionStackMode{
        ACTIONSTACK_STANDARD = 0,
        ACTIONSTACK_TARGET = 1
    };

    typedef enum
    {
        NOT_DECIDED = 0,
        INTERRUPT = -1,
        DONT_INTERRUPT = 1,
        DONT_INTERRUPT_ALL = 2
    } InterruptDecision;

    typedef enum
    {
        PUT_IN_GRAVEARD,
        PUT_IN_HAND,
        PUT_IN_LIBRARY_TOP,
        PUT_IN_EXILE,
        PUT_IN_LIBRARY_BOTTOM,
        PUT_IN_LIBRARY_SECOND,
        PUT_IN_EXILE_IMPRINT
    } FizzleMode;

protected:
    JQuadPtr pspIcons[8];
    InterruptDecision interruptDecision[2];
    float timer;
    int currentState;
    ActionStackMode mode;
    int checked;
    ATutorialMessage* currentTutorial;
    int interruptBtnXOffset, noBtnXOffset, noToAllBtnXOffset, interruptDialogWidth;
    int interruptDialogX, interruptDialogHeight;

    JButton handleInterruptRequest( JButton inputKey, int& x, int& y );

public:

    Player * lastActionController;
    int setIsInterrupting(Player * player, bool log = true);
    int count( int type = 0 , int state = 0 , int display = -1);
    Interruptible * getActionElementFromCard(MTGCardInstance * card);
    Interruptible * getPrevious(Interruptible * next, int type = 0, int state = 0 , int display = -1);
    int getPreviousIndex(Interruptible * next, int type = 0, int state = 0 , int display = -1);
    Interruptible * getNext(Interruptible * previous, int type = 0, int state = 0 , int display = -1);
    int getNextIndex(Interruptible * previous, int type = 0, int state = 0 , int display = -1);
    void Fizzle(Interruptible * action, MTGCardInstance* fizzler = NULL, FizzleMode fizzleMode = PUT_IN_GRAVEARD);
    Interruptible * getAt(int id);
    void cancelInterruptOffer(InterruptDecision cancelMode = DONT_INTERRUPT, bool log = true);
    void endOfInterruption(bool log = true);
    //#W56-Z: an armed target/cost chooser must never outlive the interrupt
    //window it was armed in. Cancels a pending choice held by a NON-AI seat
    //(human, or a scripted test-suite seat); returns 1 if one was cancelled,
    //0 if there was nothing pending or the choice is mandatory (cantCancel).
    int cancelPendingChoice();
    //Keep an open interrupt offer to `who` from timing out: an asynchronous
    //decision maker (the LLM player) can need longer than the configured
    //interrupt seconds to answer.
    void extendInterruptOffer(Player * who, float seconds = 2.0f)
    {
        if (askIfWishesToInterrupt == who && timer >= 0 && timer < seconds)
            timer = seconds;
        //The stall watchdog must not count an asynchronous decision maker's
        //thinking time as a wedge: every tick it keeps its window alive is
        //progress.
        if (askIfWishesToInterrupt == who)
        {
            mHoldTicks = 0;
            mHoldSeconds = 0.0f;
            mHoldStartMs = 0; //#W54-R: re-arm the wall clock on the next tick
        }
    }
    Interruptible * getLatest(int state);
    //W53-AA (owner Vita softlock, 2026-09-02): an interrupt window owned by
    //an AI seat has NO escape hatch. DuelLayers::CheckUserInput drops every
    //human key while `isInterrupting` names the other seat, INTERRUPT_SECONDS
    //defaults to 0 ("wait forever"), and userRequestNextGamePhase refuses on
    //anything NOT_RESOLVED - so if the AI's Act ever stops answering, the
    //human's game stops dead with no message and no way out but the menu.
    //These three fields bound that: the same holder sitting on the same stack
    //object for mHoldTicks consecutive ActionStack updates with no progress is
    //a wedge, and Update releases it (loudly) instead of hanging the game.
    Interruptible * mHoldOn;
    Player * mHoldWho;
    int mHoldTicks;
    //dt-accumulated GAME seconds. Still the budget for a HEURISTIC AI seat
    //(the softlock class: it answers within a tick or two of its own
    //throttle, so any denomination is orders of magnitude of slack).
    float mHoldSeconds;
    //#W54-R: REAL WALL CLOCK since this hold began (steady_clock ms; 0 = not
    //armed yet). The interactive-AI budget is denominated here and nowhere
    //else. dt is a synthetic counter by design - the headless harness feeds
    //a fixed WAGIC_FASTCLOCK dt and the suite feeds an INCREMENTING one -
    //so a watchdog that rides it measures ticks while claiming to measure
    //seconds. Wave-54 corpus: 1,200 "seconds" cashed out as exactly 12,000
    //ticks, 468 releases all at the same tick count, every one of them a
    //window whose owner had a model call in flight.
    long long mHoldStartMs;
    //#W57-U (the vpk16 in-flight softlock): REAL WALL CLOCK since this holder
    //went IN FLIGHT on this stack object (0 = not in flight). Deliberately a
    //second anchor rather than a reuse of mHoldStartMs: extendInterruptOffer
    //zeroes that one on every tick the seat is being OFFERED the window, and
    //the seat calls it from decisionPending on every tick it is thinking - so
    //the only clock the old floor had could be reset for ever by a seat whose
    //request was already dead. Nothing resets this one but a change of holder,
    //a change of the stack object, or the call actually landing.
    long long mHoldInFlightSinceMs;
    //The stack object the current priority round refers to. When the top
    //of the stack changes (new spell/trigger/phase item), decisions reset
    //and one tick passes before anyone can pass or anything resolves -
    //giving triggered-ability placement and state-based actions their
    //window between resolutions.
    Interruptible * mPriorityOn;
    Player * askIfWishesToInterrupt;
    //Priority auto-pass: whether a response window should be OFFERED to p
    //for the given unresolved stack action, or silently passed. Replaces
    //the old push-time interruptDecision seeding.
    bool wouldOfferWindow(Player * p, Interruptible * action);
    int garbageCollect();
    int addAction(Interruptible * interruptible);
    Spell * addSpell(MTGCardInstance* card, TargetChooser * tc, ManaCost * mana, int payResult, int storm, bool forcedinterrupt = false);
    int AddNextGamePhase();
    int AddNextCombatStep();
    int addPutInGraveyard(MTGCardInstance * card);
    int addDraw(Player * player, int nbcards = 1);
    int addLife(Damageable * _target,int amount = 0);
    int addDamage(MTGCardInstance * _source, Damageable * target, int _damage, bool noTrigger = false);
    int addAbility(MTGAbility * ability);
    void Update(float dt);
    bool CheckUserInput(JButton key);
    virtual void Render();
    ActionStack(GameObserver* game);
    int resolve();
    int has(Interruptible * action);
    int has(MTGAbility * ability);
    int receiveEventPlus(WEvent * event);
#if defined (WIN32) || defined (LINUX) || defined (IOS)
    void Dump();
#endif
    void setCurrentTutorial(ATutorialMessage* message) {currentTutorial = message;};
    ATutorialMessage* getCurrentTutorial() {return currentTutorial;};
    bool isCalm() {return interruptDecision[0] == NOT_DECIDED && interruptDecision[1] == NOT_DECIDED;};
};

#endif
