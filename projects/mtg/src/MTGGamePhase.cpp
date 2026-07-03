#include "PrecompiledHeader.h"

#include "MTGGamePhase.h"
#include "GuiPhaseBar.h"
#include "Translate.h"

MTGGamePhase::MTGGamePhase(GameObserver* g, int id) :
    ActionElement(id), observer(g)
{
    animation = 0;
    currentState = -1;
    mPassTurnUntil = -1;
    mTriggerHeld = 0;
    mFont = WResourceManager::Instance()->GetWFont(Fonts::MAIN_FONT);
    if(mFont)
    mFont->SetBase(0); // using 2nd font
}

void MTGGamePhase::Update(float dt)
{
    int newState = observer->getCurrentGamePhase();
    if (newState != currentState)
    {
        activity = Active;
        animation = 4;
        currentState = newState;
    }

    if (animation > 0)
    {
        animation--;
    }
    else
    {
        activity = Inactive;
        animation = 0;
    }

    //Hold-to-pass: holding the phase-advance control (Q / right mouse
    //button) ~0.7s arms pass-turn; the initial press already advanced one
    //phase (and cancelled any armed pass), so tap behavior is unchanged.
    JButton trigger = (options[Options::REVERSETRIGGERS].number ? JGE_BTN_NEXT : JGE_BTN_PREV);
    if (observer->getInput() && observer->getInput()->GetButtonState(trigger)
        && observer->currentlyActing() && !observer->currentlyActing()->isAI())
    {
        mTriggerHeld += dt;
        if (mTriggerHeld > 0.7f && mPassTurnUntil < 0)
            mPassTurnUntil = observer->turn;
    }
    else
        mTriggerHeld = 0;

    //Pass-turn: keep requesting the next phase whenever advancing is legal.
    //NextGamePhase()'s own guards (activity cooldown, someone else acting)
    //and userRequestNextGamePhase's deeper ones (open target chooser, extra
    //payments, interrupt offers) pace and interrupt the fast-forward - any
    //decision that genuinely needs the player still surfaces normally.
    if (mPassTurnUntil >= 0)
    {
        if (observer->turn != mPassTurnUntil)
            mPassTurnUntil = -1; //turn passed: done
        else if (observer->currentlyActing() && !observer->currentlyActing()->isAI())
            NextGamePhase();
    }
}

void MTGGamePhase::Render()
{
    if (mPassTurnUntil < 0 || !mFont)
        return;
    mFont->SetColor(ARGB(255,255,255,255));
    mFont->DrawString(_("Passing turn...").c_str(), SCREEN_WIDTH / 2, SCREEN_HEIGHT - 14, JGETEXT_CENTER);
}

bool MTGGamePhase::NextGamePhase()
{
    if (activity == Inactive)
    {
        if (observer->currentActionPlayer == observer->currentlyActing())
        {
            activity = Active;
            observer->userRequestNextGamePhase();
            return true;
        }
    }
    return false;
}


bool MTGGamePhase::CheckUserInput(JButton key)
{
    JButton trigger = (options[Options::REVERSETRIGGERS].number ? JGE_BTN_NEXT : JGE_BTN_PREV);
    if (trigger == key)
    {
        mPassTurnUntil = -1; //a fresh press cancels an armed pass-turn
        return NextGamePhase();
    }
    return false;
}

MTGGamePhase * MTGGamePhase::clone() const
{
    return NEW MTGGamePhase(*this);
}

ostream& MTGGamePhase::toString(ostream& out) const
{
    return out << "MTGGamePhase ::: animation " << animation << " ; currentState : " << currentState;
}
