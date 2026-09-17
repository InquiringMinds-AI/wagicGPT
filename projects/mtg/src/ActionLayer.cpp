#include "PrecompiledHeader.h"

#include "ActionLayer.h"
#include "GameObserver.h"
#include "Targetable.h"
#include "WEvent.h"
#include "AllAbilities.h"
#include "MTGRules.h"

MTGAbility* ActionLayer::getAbility(int type)
{
    for (size_t i = 1; i < mObjects.size(); i++)
    {
        MTGAbility * a = ((MTGAbility *) mObjects[i]);
        if (a->aType == type)
        {
            return a;
        }
    }
    return NULL;
}

int ActionLayer::removeFromGame(ActionElement * e)
{
    observer->bumpAbilityEpoch(); //#W54-H (A6b): an ability leaving is a state change
    mReactions.erase(e);
    int i = getIndexOf(e);
    if (i == -1)
        return 0;

    //The cascade circled back to an element already mid-destroy higher up
    //this call stack (mutual lord/grant death in one sweep): running
    //destroy() again would double-destroy it. The outer call owns the
    //removal; this request is a no-op.
    for (size_t k = 0; k < mDestroying.size(); k++)
        if (mDestroying[k] == e)
            return 0;

    if (isWaitingForAnswer() == e)
        setCurrentWaitingAction(NULL);
    assert(e);
    mDestroying.push_back(e);
    e->destroy();
    mDestroying.pop_back();

    i = getIndexOf(e); //the destroy event might have changed the contents of mObjects, so we get the index again
    if (i == -1)
        return 0; //Should not happen, it means we deleted thesame object twice?
    AbilityFactory af(observer);

    //Unconditionally purge the manaObjects index (see GuiLayer::Remove for
    //the dangling-pointer failure the conditional purge caused).
    for (size_t k = 0; k < manaObjects.size(); k++)
        if (manaObjects[k] == e)
        {
            manaObjects.erase(manaObjects.begin() + k);
            break;
        }
    //#W58-F (F1): an armed menu row that names this element must stop naming
    //it BEFORE the erase compacts the vector - a pointer-identity scan, no
    //dereference, so it is safe whatever destroy() just did to the element.
    for (size_t k = 0; k < menuRowElements.size(); k++)
        if (menuRowElements[k] == e)
            menuRowElements[k] = NULL;
    mObjects.erase(mObjects.begin() + i);
    return 1;
}

bool ActionLayer::moveToGarbage(ActionElement * e)
{
    if (removeFromGame(e))
    {
        //#W86-IB (audit-2026-09 bug list item 3): ONE ELEMENT, AT MOST ONE SLOT.
        //cleanGarbage deletes every slot it walks, and forgetElement's per-object
        //exemption (#W84-GB) only nulls the slot the sweep is on - so a SECOND slot
        //for the same element would be deleted a second time, on storage already
        //freed. A duplicate slot can only exist if the element came BACK into the
        //layer after being garbaged (addToGame refuses that, below, but the two
        //guards are independent on purpose: this one holds whatever route put it
        //back). Pushing nothing is the whole fix - the element is already scheduled.
        if (!isInGarbage(e))
            garbage.push_back(e);
        else
            DebugTrace("ActionLayer::moveToGarbage: element is ALREADY in garbage -"
                       " keeping its single slot rather than scheduling a second delete");
        return true;
    }
    return false;

}

bool ActionLayer::isInGarbage(ActionElement * e)
{
    for (size_t i = 0; i < garbage.size(); ++i)
    {
        if (garbage[i] == e)
            return true;
    }
    return false;
}

//#W57-F (D25): sweep every back-pointer into a zone that is about to be freed.
//`zone`'s cards are still valid when this runs (the caller is
//MTGPlayerCards::beforeBeginPhase, immediately before the SAFE_DELETE), which is
//what makes eviction safe: an ability whose SOURCE is dying is removed through
//the layer's own removeFromGame/destroy contract, exactly as it would have been
//had the card left play in the ordinary way. An ability whose TARGET is dying
//keeps working - every consumer already handles a NULL target - so that
//back-pointer is simply cleared. Returns the number of elements evicted.
int ActionLayer::purgeDeadReferences(MTGGameZone * zone)
{
    if (!zone)
        return 0;
    int evicted = 0;
    //A pointer-identity scan, no dereference of the ability's own pointers.
    for (int j = 0; j < zone->nb_cards; j++)
    {
        MTGCardInstance * doomed = zone->cards[j];
        if (!doomed)
            continue;
        //TARGET first, for the whole layer including the garbage list: an
        //element already removed from mObjects is still deleted later, and its
        //destructor (or a consumer walking the garbage list) can read it.
        for (size_t i = 0; i < mObjects.size(); i++)
        {
            MTGAbility * a = dynamic_cast<MTGAbility *>(mObjects[i]);
            if (a && a->target == (Targetable *) doomed)
                a->target = NULL;
        }
        for (size_t i = 0; i < garbage.size(); i++)
        {
            MTGAbility * a = dynamic_cast<MTGAbility *>(garbage[i]);
            if (a && a->target == (Targetable *) doomed)
                a->target = NULL;
        }
        //SOURCE: the ability cannot function without it (every method reads
        //source), so it is evicted rather than nulled. Restart the walk after
        //each removal - destroy() can cascade into further removals.
        bool again = true;
        while (again)
        {
            again = false;
            for (size_t i = 0; i < mObjects.size(); i++)
            {
                MTGAbility * a = dynamic_cast<MTGAbility *>(mObjects[i]);
                if (!a || a->source != doomed)
                    continue;
                if (moveToGarbage(a))
                    evicted++;
                again = true;
                break;
            }
        }
    }
    //Pointer identity again - hasCard() would dereference `currentActionCard`,
    //and a stale one is exactly what this sweep exists to prevent.
    for (int j = 0; j < zone->nb_cards; j++)
    {
        MTGCardInstance * doomed = zone->cards[j];
        if (!doomed)
            continue;
        if (currentActionCard == doomed)
            currentActionCard = NULL;
        if (menuObject == (Targetable *) doomed)
            menuObject = NULL;
    }
    return evicted;
}

//#W82-EH (audit-2026-09 item 8): the per-CARD form of the sweep above. Same
//pointer-identity discipline (no dereference of the doomed card), same
//ownership contract; see GameObserver::purgeDeadReferencesForCard for why the
//zone-scoped sweep could not catch this class.
int ActionLayer::purgeDeadReferencesForCard(MTGCardInstance * doomed)
{
    if (!doomed)
        return 0;
    int evicted = 0;
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        MTGAbility * a = dynamic_cast<MTGAbility *>(mObjects[i]);
        if (a && a->target == (Targetable *) doomed)
            a->target = NULL;
    }
    for (size_t i = 0; i < garbage.size(); i++)
    {
        MTGAbility * a = dynamic_cast<MTGAbility *>(garbage[i]);
        if (a && a->target == (Targetable *) doomed)
            a->target = NULL;
    }
    //#W83-FF (fix-review item 7): RETRY ONLY ON PROGRESS. The first cut set
    //`again = true` whether or not moveToGarbage succeeded, and removeFromGame
    //returns false for an element already in `mDestroying` - a re-entrant sweep
    //meeting such an element retried it for ever and the outer removal could
    //never finish. The walk restarts only when an element actually LEFT (destroy()
    //can cascade into further removals, so the indices are stale after one); an
    //element that refuses to leave is skipped and the walk continues past it.
    for (size_t i = 0; i < mObjects.size(); )
    {
        MTGAbility * a = dynamic_cast<MTGAbility *>(mObjects[i]);
        if (!a || a->source != doomed)
        {
            i++;
            continue;
        }
        if (moveToGarbage(a))
        {
            evicted++;
            i = 0; //the cascade may have reshaped the vector - restart
            continue;
        }
        DebugTrace("ActionLayer::purgeDeadReferencesForCard: an element sourced by a"
                   " dying card could not be evicted (mid-destroy) - skipping it");
        i++;
    }
    if (currentActionCard == doomed)
        currentActionCard = NULL;
    if (menuObject == (Targetable *) doomed)
        menuObject = NULL;
    return evicted;
}

//#W82-EH (audit-2026-09 item 8, crash B). A DELETED ELEMENT MUST NOT STAY IN
//THE LAYER.
//
//ActionLayer.h already names the hazard: "registration and ownership are
//decoupled (a parent's destructor deletes children that are still registered)".
//That is the crash: core 474128's MayAbility is at mObjects[i] in
//ActionLayer::Update, it is NOT in `garbage`, and its own fields are ASCII
//rubble ('0x3030303030303030', a fragment of another object's string) - freed
//memory that the layer is still updating, whose `source` pointer therefore
//reads as a card that is gone (LegalActionsOracle::canPlayLandNow ->
//MTGCardInstance::StackIsEmptyandSorcerySpeed on freed storage). The two master
//cores (393716 / 395840) are the same frame.
//
//Ownership is not fixable here without a refactor of who deletes whom, but
//REGISTRATION is: whatever deletes an ability, ~MTGAbility calls this, and the
//element leaves every index the layer holds before its storage is reused. No
//destroy() is run (the object is already being destroyed and its owner has that
//contract); this is pointer bookkeeping only. mObjects and manaObjects are
//ERASED because their readers assume live non-null entries; garbage and
//menuRowElements are NULLED because their readers index by position.
void ActionLayer::forgetElement(ActionElement * e)
{
    if (!e)
        return;
    //#W83-FF (fix-review item 7) / #W84-GB (review-2 item 1): BOUND THE SCANS
    //WITHOUT SUPPRESSING THE BOOKKEEPING.
    //
    //The wave-83 bound was a GLOBAL flag held for the whole of cleanGarbage(),
    //and that was a new lifetime hazard, not an optimisation. cleanGarbage runs
    //during ordinary play (GameObserver::Update), and a garbage entry's
    //destructor RECURSIVELY DELETES ABILITIES IT OWNS - MayAbility::~MayAbility
    //frees its clone, GenericTargetAbility::~GenericTargetAbility its nested
    //ability. Those children are not the entry being swept: one can still be in
    //mObjects (its freed address then stays in the LIVE layer), can occupy a
    //LATER garbage slot (which the sweep would then delete a second time), or can
    //be the menu's row, a mana producer, or the waiting action. Suppressing their
    //deregistration is precisely the class this facility exists to close.
    //
    //So the exemption is PER-OBJECT and covers exactly one container: while the
    //sweep is deleting entry X, X itself skips the `garbage` walk - its own slot
    //is being nulled by SAFE_DELETE on the very next statement and the vector is
    //cleared straight after. That is the whole theta(G^2) term. Everything else
    //X touches, and EVERYTHING a recursively deleted child touches, is bookkept
    //normally.
    //
    //The second bound is the common case: an element that was never registered in
    //any layer (every parse-time template, every clone deleted without being
    //added) has nothing to un-register from, and says so in O(1). The wave-83
    //version tested "all containers empty", which is never true during play.
    if (!e->mEverRegistered)
        return;
    const bool isSweptEntry = (e == mGarbageEntryBeingDeleted);
    for (size_t k = 0; k < menuRowElements.size(); k++)
        if (menuRowElements[k] == e)
            menuRowElements[k] = NULL;
    for (size_t k = 0; k < mDestroying.size(); k++)
        if (mDestroying[k] == e)
            mDestroying[k] = NULL;
    if (!isSweptEntry)
        for (size_t k = 0; k < garbage.size(); k++)
            if (garbage[k] == e)
                garbage[k] = NULL;
    for (size_t k = 0; k < manaObjects.size(); k++)
        if (manaObjects[k] == e)
        {
            manaObjects.erase(manaObjects.begin() + k);
            break;
        }
    bool found = false;
    for (size_t k = 0; k < mObjects.size(); k++)
        if (mObjects[k] == e)
        {
            mObjects.erase(mObjects.begin() + k);
            found = true;
            break;
        }
    if (found)
    {
        mReactions.erase(e);
        if (currentWaitingAction == e)
            currentWaitingAction = NULL;
        DebugTrace("ActionLayer: an element was DELETED while still registered - "
                   "dropped it from the layer before its storage could be reused");
    }
}

void ActionLayer::cleanGarbage()
{
    //#W84-GB (review-2 item 1): see forgetElement. Only the entry CURRENTLY being
    //deleted skips the `garbage` walk - its slot is nulled by the SAFE_DELETE on
    //this line and the vector is cleared below. Anything its destructor deletes
    //recursively is a different object and is bookkept in full, including its own
    //later garbage slot, which is what stops the sweep deleting it twice.
    for (size_t i = 0; i < garbage.size(); ++i)
    {
        mGarbageEntryBeingDeleted = garbage[i];
        SAFE_DELETE(garbage[i]);
        mGarbageEntryBeingDeleted = NULL;
    }
    garbage.clear();
}

int ActionLayer::reactToClick(ActionElement * ability, MTGCardInstance * card)
{
    int result = ability?ability->reactToClick(card):0;
    if (result)
        stuffHappened = 1;
    return result;
}

int ActionLayer::reactToTargetClick(ActionElement* ability, Targetable * card)
{
    int result = ability->reactToTargetClick(card);
    if (result)
        stuffHappened = 1;
    return result;
}

bool ActionLayer::CheckUserInput(JButton key)
{
    if (observer->mExtraPayment && key == JGE_BTN_SEC)
    {
        for (size_t i = 0; i < mObjects.size(); i++)
        {
            if (mObjects[i] != NULL)
            {
                ActionElement * currentAction = (ActionElement *) mObjects[i];
                currentAction->CheckUserInput(key);
                //check first with a mock up to see if any abilities will care about the extra payment
                //being cancelled. currently only menuability and paidability will care.
            }
        }
        if (observer->mExtraPayment->costs.size() && observer->mExtraPayment->costs[0]->tc)
        {
            //if we cancel, clear the targets list so that when you try again you dont already have targets from before.
            observer->mExtraPayment->costs[0]->tc->initTargets();
        }
        observer->mExtraPayment = NULL;
        return 1;
    }
    if (menuObject)
    {
        return false;
    }
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        if (mObjects[i] != NULL)
        {
            ActionElement * currentAction = (ActionElement *) mObjects[i];
            if (currentAction->CheckUserInput(key))
                return true;
        }
    }
    return false;
}

void ActionLayer::Update(float dt)
{
    if (stuffHappened)
        observer->bumpAbilityEpoch(); //#W54-H (A6b): a click or a harness poke landed
    stuffHappened = 0;
    if (menuObject)
    {
        closeUnanswerableMandatoryMenu(); //#W82-EG / #W83-FE
        if (!menuObject)
            return;                    //the menu had no legal row left and closed
        abilitiesMenu->Update(dt);
        return;
    }
    modal = 0;
    for (int i = (int)(mObjects.size()) - 1; i >= 0; i--)
    {
        //a dirty hack, there might be cases when the mObject array gets reshaped if an ability removes some of its children abilites
        if ((int) mObjects.size() <= i)
        {
            i = (int) (mObjects.size()) - 1;
            if (i<0)
                break;
        }

        if (mObjects[i] != NULL)
        {
            ActionElement * currentAction = (ActionElement *) mObjects[i];
            if (currentAction->testDestroy())
                observer->removeObserver(currentAction);
        }
    }
    GamePhase newPhase = observer->getCurrentGamePhase();
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        if (mObjects[i] != NULL)
        {
            ActionElement * currentAction = (ActionElement *) mObjects[i];
            currentAction->newPhase = newPhase;
            currentAction->Update(dt);
            currentAction->currentPhase = newPhase;
        }
    }

    if (cantCancel)
    {
        ActionElement * ae = isWaitingForAnswer();
        if(ae && ae->getActionTc())
        {
            if (!ae->getActionTc()->validTargetsExist())
            {
                cantCancel = 0;
                cancelCurrentAction();
                return;
            }
            int countTargets = ae->getActionTc()->countValidTargets();
            int maxTargets = ae->getActionTc()->maxtargets;
            if (countTargets < maxTargets)
            {
                /*
                @movedto(this|mygraveyard) from(mybattlefield):moveto(mybattlefield) 
                target(<2>creature[elf]|opponentgraveyard)
                and there were 3 in the grave, you have the valid amount needed, this function should not trigger
                ...however if you had only 1 in the grave, then the max targets is reset to the maximum you CAN
                use this effect on...in line with "up to" wording found on the cards with such abilities.
                without this, the game locks into a freeze state while you try to select the targets and dont have enough to
                fill the maxtargets list.
                */
                if (int(ae->getActionTc()->getNbTargets()) == countTargets && !ae->getActionTc()->autoChoice)//if the amount of targets is equal the all we can target
                {
                    ae->getActionTc()->done = true;//were done
                    ae->getActionTc()->source->getObserver()->cardClick(ae->getActionTc()->source, 0, false);//click source.
                }
            }
        }
    }
}

void ActionLayer::Render()
{
    if (menuObject)
    {
        abilitiesMenu->Render();
        return;
    }

    for (size_t i = 0; i < mObjects.size(); i++)
    {
        if (mObjects[i] != NULL)
        {
            ActionElement * currentAction = (ActionElement *) mObjects[i];
            currentAction->Render();
        }
    }
}

void ActionLayer::setCurrentWaitingAction(ActionElement * ae)
{
    assert(!ae || !currentWaitingAction);//this assert causes crashes when may abilities overlap each other on ai. this conidiation is preexsiting.
    currentWaitingAction = ae;
    if (!ae)
        cantCancel = 0;
}

TargetChooser * ActionLayer::getCurrentTargetChooser()
{
    if (currentWaitingAction && currentWaitingAction->waitingForAnswer)
        return currentWaitingAction->getActionTc();
    return NULL;
}

int ActionLayer::cancelCurrentAction()
{
    ActionElement * ae = isWaitingForAnswer();
    if (!ae)
        return 0;
    if (cantCancel && ae->getActionTc()->validTargetsExist())
        return 0;
    ae->waitingForAnswer = 0; //TODO MOVE THIS IN ActionElement
    setCurrentWaitingAction(NULL);
    return 1;
}

ActionElement * ActionLayer::isWaitingForAnswer()
{
    if (currentWaitingAction && currentWaitingAction->waitingForAnswer)
        return currentWaitingAction;
    return NULL;
}

int ActionLayer::stillInUse(MTGCardInstance * card)
{
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        ActionElement * currentAction = (ActionElement *) mObjects[i];
        if (currentAction->stillInUse(card))
            return 1;
    }
    return 0;
}

int ActionLayer::receiveEventPlus(WEvent * event)
{
    int result = 0;
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        ActionElement * currentAction = (ActionElement *) mObjects[i];
        result += currentAction->receiveEvent(event);
    }
    return result;
}

int ActionLayer::isReactingToTargetClick(Targetable * card)
{
    int result = 0;

    if (isWaitingForAnswer())
        return -1;

    for (size_t i = 0; i < mObjects.size(); i++)
    {
        ActionElement * currentAction = (ActionElement *) mObjects[i];
        result += currentAction->isReactingToTargetClick(card);
    }
    return result;
}

int ActionLayer::reactToTargetClick(Targetable * card)
{
    int result = 0;

    ActionElement * ae = isWaitingForAnswer();
    if (ae)
        return reactToTargetClick(ae, card);

    for (size_t i = 0; i < mObjects.size(); i++)
    {
        ActionElement * currentAction = (ActionElement *) mObjects[i];
        result += currentAction->reactToTargetClick(card);
    }
    return result;
}

bool ActionLayer::getMenuIdFromCardAbility(MTGCardInstance *card, MTGAbility *ability, int& menuId)
{
    int ctr = 0;
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        ActionElement * currentAction = (ActionElement *) mObjects[i];
        if (currentAction->isReactingToClick(card))
        {
            //Position of `ability` among EVERY ability reacting to this card -
            //the index setMenuObject's menu would give it. The counter used to
            //advance only on the matching ability, so it never passed 1 and
            //an AI click choosing one of several reacting abilities was
            //recorded without its "choice N" (a transcript replay then armed
            //the menu and had no answer for it - Living Lands forests, 2026-09-01).
            if(currentAction == ability)
                menuId = ctr;
            ctr++;
        }
    }

    if(ctr == 0 || ctr == 1)
    {
        return false;
    }
    else
        return true;
}

//TODO Simplify with only object !!!
int ActionLayer::isReactingToClick(MTGCardInstance * card)
{
    int result = 0;

    if (isWaitingForAnswer())
        return -1;

    for (size_t i = 0; i < mObjects.size(); i++)
    {
        ActionElement * currentAction = (ActionElement *) mObjects[i];
        if (currentAction->isReactingToClick(card))
        {
            ++result;
            mReactions.insert(currentAction);
        }
    }

    return result;
}

int ActionLayer::reactToClick(MTGCardInstance * card)
{
    int result = 0;

    ActionElement * ae = isWaitingForAnswer();
    if (ae)
        return reactToClick(ae, card);

    std::set<ActionElement*>::const_iterator iter = mReactions.begin();
    std::set<ActionElement*>::const_iterator end = mReactions.end();
    for (; iter !=end; ++iter)
    {
        result += reactToClick(*iter, card);
        if (result)
            break;
    }

#ifdef WIN32
    // if we hit this, then something strange has happened with the click logic - reactToClick()
    // should never be called if isReactingToClick() previously didn't have an object return true
    assert(!mReactions.empty());
#endif

    mReactions.clear();
    return result;
}

void ActionLayer::setMenuObject(Targetable * object, bool must)
{
    if (!object)
    {
        DebugTrace("FATAL: ActionLayer::setMenuObject");
        return;
    }
    menuObject = object;
    menuArmedSerial++;

    SAFE_DELETE(abilitiesMenu);
    abilitiesTriggered = NULL;

    abilitiesMenu = NEW SimpleMenu(observer->getInput(), observer->getResourceManager(), 10, this, Fonts::MAIN_FONT, 100, 100, object->getDisplayName().c_str());
    abilitiesTriggered = NEW SimpleMenu(observer->getInput(), observer->getResourceManager(), 10, this, Fonts::MAIN_FONT, 100, 100, object->getDisplayName().c_str());
    //#W58-F (F1): one entry per row of the menu that is finally kept, in the
    //same order the rows are added.
    vector<ActionElement *> plainRowElements;
    vector<ActionElement *> triggeredRowElements;
    menuRowElements.clear();
    currentActionCard = (MTGCardInstance*)object;
    menuObjectName = object->getDisplayName();
    //#W65-AL (G1), same capture as the name. `object` is a Targetable; only a
    //card carries a script, and a menu armed on anything else simply has none.
    if (MTGCardInstance * menuCard = dynamic_cast<MTGCardInstance *>(object))
        menuObjectText = menuCard->magicText;
    else
        menuObjectText.clear();
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        ActionElement * currentAction = (ActionElement *) mObjects[i];
        if (currentAction->isReactingToTargetClick(object))
        {
            if(dynamic_cast<MTGAbility*>(currentAction)->getCost()||dynamic_cast<PermanentAbility*>(currentAction))
            {
                abilitiesMenu->Add(i, currentAction->getMenuText());
                plainRowElements.push_back(currentAction); //#W58-F (F1)
#ifdef WAGIC_TRANSCRIPT_ON
                if (observer->isLoading() && getenv("WAGIC_TRANSCRIPT_TRACE"))
                    DebugTrace("[transcript-trace] menu " << menuObjectName << " + " << currentAction->getMenuText());
#endif
            }
            else
            {
                //the only time this condiation is hit is when we are about to display a menu of abilities
                //which were triggered through a triggered ability or abilities such as multiple target(
                //and may abilities appearing on cards ie: auto=may draw:1
                //this prevents abilities activated otherwise from displaying on the same menu as "triggered" and
                //"put in play" abilities. an activated ability of a card should never share a menu with
                //a triggered or may ability as it leads to exploits.
                //only exception is perminent abilities such as "cast card normally" which can share the menu with autohand=
                abilitiesTriggered->Add(i, currentAction->getMenuText());
                triggeredRowElements.push_back(currentAction); //#W58-F (F1)
#ifdef WAGIC_TRANSCRIPT_ON
                if (observer->isLoading() && getenv("WAGIC_TRANSCRIPT_TRACE"))
                    DebugTrace("[transcript-trace] menu(triggered) " << menuObjectName << " + " << currentAction->getMenuText());
#endif
            }
        }
    }
    if(abilitiesTriggered->mCount)
    {
        SAFE_DELETE(abilitiesMenu);
        abilitiesMenu = abilitiesTriggered;
        menuRowElements = triggeredRowElements; //#W58-F (F1)
    }
    else
    {
        SAFE_DELETE(abilitiesTriggered);
        menuRowElements = plainRowElements; //#W58-F (F1)
    }
    if (!must)
    {
        abilitiesMenu->Add(kCancelMenuID, "Cancel");
        menuRowElements.push_back(NULL); //#W58-F (F1): the cancel row names no ability
    }
    else
        cantCancel = 1;
    modal = 1;
}

void ActionLayer::setCustomMenuObject(Targetable * object, bool must,vector<MTGAbility*>abilities,string customName, ActionElement * owner)
{
    if (!object)
    {
        DebugTrace("FATAL: ActionLayer::setCustomMenuObject");
        return;
    }
    menuObject = object;
    menuArmedSerial++;
    menuObjectName = object->getDisplayName(); //#W48 D6, same capture as setMenuObject
    //#W65-AL (G1), same capture as the name. `object` is a Targetable; only a
    //card carries a script, and a menu armed on anything else simply has none.
    if (MTGCardInstance * menuCard = dynamic_cast<MTGCardInstance *>(object))
        menuObjectText = menuCard->magicText;
    else
        menuObjectText.clear();
    SAFE_DELETE(abilitiesMenu);
    abilitiesMenu = NEW SimpleMenu(observer->getInput(), observer->getResourceManager(), 10, this, Fonts::MAIN_FONT, 100, 100, customName.size()?customName.c_str():object->getDisplayName().c_str());
    menuRowElements.clear(); //#W58-F (F1): the rows of a multiple-choice menu
    //carry no mObjects index at all (every row is added with the same id); the
    //slot resolvers short-circuit on isMultipleChoice before reading this.
    //#W87-JA: what they DO carry is the menu's OWNER, one entry per row, so the
    //answer is dispatched to the element that armed the menu by identity.
    currentActionCard = NULL;
    abilitiesMenu->isMultipleChoice = false;
    if(abilities.size())
    {
        abilitiesMenu->isMultipleChoice = true;
        for(int w = 0; w < int(abilities.size());w++)
        {
            ActionElement* currentAction = (ActionElement*)abilities[w];
            currentActionCard = (MTGCardInstance*)abilities[0]->target;
            abilitiesMenu->Add(mObjects.size()-1, currentAction->getMenuText(),"",false);
            menuRowElements.push_back(owner); //#W87-JA: this mode row belongs to `owner`
        }
        if (!owner)
            DebugTrace("ActionLayer::setCustomMenuObject: a multiple-choice menu on '"
                       << menuObjectName << "' was armed with NO owner - no element can"
                       " answer it (its rows resolve to kNoLiveMenuElement)");
    }
    if (!must)
    {
        abilitiesMenu->Add(kCancelMenuID, "Cancel");
        //#W87-JA: a multiple-choice menu's cancel row is answered by the owner too
        //(MenuAbility::reactToChoiceClick treats an out-of-range mode as the
        //decline); an ordinary custom menu's cancel row names no ability.
        menuRowElements.push_back(abilitiesMenu->isMultipleChoice ? owner : NULL); //#W58-F (F1)
    }
    else
        cantCancel = 1;
    modal = 1;
}

//#W87-JA (audit-2026-09 bug list item 11): the owner of the armed multiple-choice
//menu, by identity. The W58-F map holds it per row (nulled by removeFromGame /
//forgetElement the moment the owner leaves the game); the index is looked up
//LIVE, so a layer that compacted under the menu cannot mislead this.
ActionElement * ActionLayer::armedMenuOwner()
{
    if (!menuObject || !abilitiesMenu || !abilitiesMenu->isMultipleChoice)
        return NULL;
    ActionElement * owner = menuRowElements.empty() ? NULL : menuRowElements[0];
    if (!owner || getIndexOf(owner) < 0)
        return NULL;
    return owner;
}

//#W58-F (F1) / #W83-FD (fix-review item 4): menu row -> the ability it names,
//resolved by IDENTITY. FOUR DIFFERENT THINGS ride this one integer, and the
//wave-82 bounds check conflated two of them. They are, in full:
//
//  ROW IDENTITY - which ABILITY an ordinary row was built from. Never the id:
//      `menuRowElements[menuIndex]`, captured at arm time and nulled when that
//      element leaves the game. This is what makes a compacted layer safe.
//  LAYER SLOT   - the ability's index in mObjects RIGHT NOW. This is what `slot`
//      returns for an ordinary row, re-pointed when the vector moved under the
//      armed menu, and it is the ONLY value a caller may index mObjects with.
//  MODE INDEX   - which OPTION of a MenuAbility a multiple-choice row is. That is
//      `menuIndex` itself, never the id: setCustomMenuObject gives every row the
//      SAME id (`mObjects.size()-1`, captured at arm time) and the answer is
//      dispatched by ButtonPressedOnMultipleChoice, which resolves the row to
//      the element that OWNS the menu through menuRowElements (#W87-JA) - by
//      identity, at its live index, never by scanning mObjects. A multiple-choice
//      row is therefore ALWAYS answerable while its menu stands and its owner is
//      in the game, whatever the layer did to that stale id, and `slot` comes
//      back as kMenuRowIsMode so no caller can mistake it for a position.
//  SENTINEL     - kCancelMenuID (-1) is the cancel row and 0 is the engine's
//      long-standing "not a selectable option"; both are handed back untouched.
//      Note the honest scope of that: AIPlayerBaka::selectMenuOption and
//      DecisionManager skip `slot <= 0`, but ActionLayer::ButtonPressed's ordinary
//      branch does accept slot 0 as a layer index, and always has.
//
//False = this row names nothing the layer still holds: the caller must make no
//decision from it (readers skip the row, the act path does nothing this tick and
//the menu stays armed to be re-asked).
bool ActionLayer::getMenuControlId(int menuIndex, int & slot)
{
    if (!abilitiesMenu || menuIndex < 0 || (size_t) menuIndex >= abilitiesMenu->mObjects.size())
        return false;
    //#W83-FD (fix-review item 4): a MULTIPLE-CHOICE row is a MODE INDEX, and it is
    //answerable for as long as its menu stands. The wave-82 bounds check treated
    //its stale arm-time id as a slot and REFUSED the row once the layer shrank
    //past it - which made every legal mode of a live MenuAbility unreachable to
    //doReactTo (the AI's only route; the human's ButtonPressed dispatches multiple
    //choice BEFORE slot resolution, so the two seats disagreed). Answer first,
    //before the id is looked at at all.
    if (abilitiesMenu->isMultipleChoice)
    {
        slot = kMenuRowIsMode;
        return true;
    }
    slot = abilitiesMenu->mObjects[menuIndex]->GetId();
    if (slot <= 0)
        return true;
    ActionElement * armed = ((size_t) menuIndex < menuRowElements.size())
                            ? menuRowElements[menuIndex] : NULL;
    if (!armed)
        return false; //the ability this row was built from has left the game
    if ((size_t) slot < mObjects.size() && mObjects[slot] == armed)
        return true;
    //Still in the game, at a new index: re-point rather than answer with
    //whatever ability happens to sit at the stale position now.
    int live = getIndexOf(armed);
    if (live < 0)
        return false;
    DebugTrace("ActionLayer: menu row " << menuIndex << " re-pointed " << slot << " -> " << live);
    slot = live;
    return true;
}

//#W58-F (F1): the same mapping from the other end, for the human path -
//SimpleMenu hands JGuiListener::ButtonPressed a row ID, not a row index.
bool ActionLayer::getLiveMenuSlot(int controlid, int & slot)
{
    if (abilitiesMenu)
    {
        for (size_t i = 0; i < abilitiesMenu->mObjects.size(); i++)
            if (abilitiesMenu->mObjects[i]->GetId() == controlid)
                return getMenuControlId((int) i, slot);
    }
    //An id that names no row of the current menu (an engine call rather than a
    //menu answer): accept it only as a live index into the layer.
    if (controlid >= 0 && (size_t) controlid < mObjects.size())
    {
        slot = controlid;
        return true;
    }
    return false;
}

//#W82-EG (audit-2026-09 item 4; Astra F07, Fable G8) / #W83-FE (fix-review item
//6). A MANDATORY MENU WHOSE ROWS HAVE ALL VANISHED IS CLOSED.
//
//The W58-F row-identity map is the real fix for a real SIGABRT (an armed menu's
//stored positions naming a different ability, or none, after removeFromGame
//compacted the vector) and it stays. What it did not answer is the case where
//EVERY row has gone: getMenuControlId then refuses every row, so no answer can
//be given, and the menu is noncancelable by construction (`must`) - an
//unanswerable decision that holds the action layer, and with it the phase.
//Astra F07: "If all rows of a noncancelable menu expire, skipping/re-asking them
//does not reconstruct the underlying menu. The code leaves it armed."
//
//#W83-FE: THE WAVE-82 ANSWER REBUILT THE MENU, AND THAT WAS WORSE. setMenuObject
//gathers every ability reacting to the subject NOW and keeps the triggered/free
//rows if there are any, otherwise the ordinary cost-bearing ones. So an expired
//mandatory TRIGGER on a permanent that also carries a voluntary activated ability
//was rebuilt as "activate this other ability", WITH NO CANCEL ROW - a voluntary
//action turned into a mandatory one, which is exactly the removed-legal-option
//failure this whole lane exists to avoid. "Same card" is not "same decision".
//
//The decision that expired cannot be re-asked, because the options it offered
//have left the game. So it is CLOSED, and nothing is put in its place. That
//removes nothing (the rows were already gone) and it releases the layer, which is
//the completion path Astra F07 asked for. CR 117.3d - "If a player has priority
//and chooses not to take any actions, that player passes" - does not make a
//voluntary activation mandatory, and nothing here makes one.
//Ordinary (cancelable) menus are untouched: they already have an answer, Cancel.
//Multiple-choice menus are untouched too, and no longer need to be excluded for
//safety: since #W83-FD their rows stay answerable however the layer moved.
bool ActionLayer::closeUnanswerableMandatoryMenu()
{
    if (!menuObject || !abilitiesMenu || !cantCancel)
        return false;
    if (abilitiesMenu->isMultipleChoice)
        return false; //a mode row is answerable for as long as its menu stands
    for (size_t i = 0; i < abilitiesMenu->mObjects.size(); i++)
    {
        int slot = 0;
        //A row is an ANSWER only if it resolves to an ability the layer still
        //holds. Slot 0 and the cancel sentinel are not answers to a `must` menu
        //(a mandatory menu has no cancel row at all).
        if (getMenuControlId((int) i, slot) && slot > 0)
            return false;
    }
    DebugTrace("ActionLayer: a MANDATORY menu on '" << menuObjectName
               << "' has no answerable row left - closing it rather than leaving an"
               " unanswerable decision holding the layer");
    menuObject = 0;
    currentActionCard = NULL;
    menuObjectName.clear();
    menuObjectText.clear();
    cantCancel = 0;
    return true;
}

void ActionLayer::doReactTo(int menuIndex)
{

    if (menuObject)
    {
        //An out-of-range index (a test script's stray "choice N", an AI
        //picking a stale option) crashed on the unchecked vector access.
        if (!abilitiesMenu || menuIndex < 0 || (size_t)menuIndex >= abilitiesMenu->mObjects.size())
        {
            DebugTrace("ActionLayer::doReactTo ignoring out-of-range menu index " << menuIndex);
            return;
        }
        int slot = 0;
        if (!getMenuControlId(menuIndex, slot)) //#W58-F (F1): stale row, no decision this tick
        {
            DebugTrace("ActionLayer::doReactTo ignoring stale menu index " << menuIndex);
            return;
        }
        int controlid = abilitiesMenu->mObjects[menuIndex]->GetId();
        DebugTrace("ActionLayer::doReactTo " << controlid);
        if (abilitiesMenu && abilitiesMenu->isMultipleChoice)
        {
            return ButtonPressedOnMultipleChoice(menuIndex);
        }
        ButtonPressed(0, controlid);
    }
}

void ActionLayer::ButtonPressed(int, int controlid)
{
    stringstream stream;
    for(size_t i = 0; i < abilitiesMenu->mObjects.size(); i++)
    {   // this computes the reverse from the doReactTo method
        if(abilitiesMenu->mObjects[i]->GetId() == controlid)
        {
            stream << "choice " << i;
            observer->logAction(observer->currentActionPlayer, stream.str());
            break;
        }
    }

    if(this->abilitiesMenu && this->abilitiesMenu->isMultipleChoice)
    {
        return ButtonPressedOnMultipleChoice();
    }
    //#W58-F (F1): the id is a POSITION captured when the menu was armed, so it
    //is only meaningful while the row's ability is still in the layer. A stale
    //row falls through to the "id we don't recognize" branch below: no
    //decision this tick, and the menu is deliberately NOT cleared.
    int slot = -2;
    if (getLiveMenuSlot(controlid, slot) && slot >= 0 && slot < static_cast<int>(mObjects.size()))
    {
        ActionElement * currentAction = (ActionElement *) mObjects[slot];
        //#W43-6. A rule callback may CHAIN a new decision instead of finishing:
        //answering "Cast Card Normally" for an {X} spell runs
        //MTGPutInPlayRule::reactToClick, which arms the X-ANNOUNCEMENT menu and
        //returns 0. Clearing menuObject/currentActionCard unconditionally after
        //the callback threw that fresh menu away for a tick, so the action layer
        //read IDLE while a cast was mid-flight - whoever held priority advanced
        //the phase, the end-of-phase rule emptied the mana pool, and the payment
        //floated for THAT cast was gone. The announcement then arrived over an
        //empty pool (no affordable X), was answered 0, and the cast died at
        //isReactingToClick with no event and no record: the silent cast drop
        //(corpus 20260824 deck130 s293-298; probe tools/silent-cast-probe.sh).
        //Only retire the menu the callback actually consumed.
        unsigned int armedBefore = menuArmedSerial;
        currentAction->reactToTargetClick(menuObject);
        if (menuArmedSerial == armedBefore)
        {
            menuObject = 0;
            currentActionCard = NULL;
            menuObjectName.clear();
            menuObjectText.clear(); //#W65-AL (G1)
        }
    }
    else if (controlid == kCancelMenuID)
    {
        observer->mLayers->stackLayer()->endOfInterruption(false);
        menuObject = 0;
        currentActionCard = NULL;
        menuObjectName.clear();
        menuObjectText.clear(); //#W65-AL (G1)
    }
    else
    {
        // fallthrough case. We have an id we don't recognize - do nothing, don't clear the menu!
        //assert(false);
    }
}

//#W87-JA (audit-2026-09 bug list item 11; buglist-lane.md item 2's residual).
//THE ANSWER GOES TO THE ELEMENT THAT OWNS THE MENU, RESOLVED BY IDENTITY.
//
//This is the ONE dispatcher every seat's mode answer passes through: the human's
//SimpleMenu OK press (ButtonPressed -> here with no row, reading the menu's
//cursor), the script's / replay's `choice N` (doReactTo -> here with the row),
//the heuristic seat (AIPlayerBaka::computeActions), the LLM seat
//(AIPlayerGPT seams, and DecisionManager::applyMenuChoice for CHOOSE_MODE /
//ANNOUNCE_X). It used to find the MenuAbility to answer by scanning mObjects from
//the top for one with `triggered` set: slot 0 was never examined (a convention
//that the first element registered is a game rule, enforced by nothing), a
//scan that found nothing left the index at -1 and the next branch read that -1
//as kCancelMenuID, and `triggered` is never cleared by an answer - a MenuAbility
//whose chosen mode is a MAY lingers in the layer, processed and still
//triggered, until its clone is answered; a second MenuAbility below it that arms
//ITS menu then had its answer handed to the lingering one (reactToChoiceClick
//re-clones the chosen mode, processAbility returns on `processed`, the answer is
//eaten, the armed menu re-asks every tick) - w87ja is that shape.
//
//Now: the answered row's identity is the menu's owner, captured when the menu
//was armed (setCustomMenuObject's `owner`, one entry per row in the W58-F map,
//nulled when the owner leaves the game); its index is looked up live. No scan,
//no raw slot, no dependence on what sits at mObjects[0]. A row with no live
//owner is kNoLiveMenuElement - a named case, distinct from cancel: the menu is
//unanswerable (its owner has left the game), so it is closed and the interrupt
//window it held is released, exactly what the unnamed -1 branch always did.
void ActionLayer::ButtonPressedOnMultipleChoice(int choice)
{
    const int row = (choice > -1) ? choice : (abilitiesMenu ? abilitiesMenu->getmCurr() : -1);
    ActionElement * owner = (row >= 0 && (size_t) row < menuRowElements.size())
                            ? menuRowElements[row] : NULL;
    int live = kNoLiveMenuElement;
    if (owner)
    {
        const int index = getIndexOf(owner);
        if (index >= 0)
            live = index;
    }
    if (live >= 0)
    {
        ActionElement * currentAction = (ActionElement *) mObjects[live];
        currentAction->reactToChoiceClick(menuObject, row, live);
        //The click may have reshaped the layer (a processed mode resolves at
        //once); re-resolve the owner by identity before touching it.
        const int again = getIndexOf(owner);
        MenuAbility * ma = (again >= 0) ? dynamic_cast<MenuAbility *>(mObjects[again]) : NULL;
        if (ma)
            ma->removeMenu = true;//we clicked something, close menu now.
    }
    else
    {
        DebugTrace("ActionLayer::ButtonPressedOnMultipleChoice: row " << row << " of the menu on '"
                   << menuObjectName << "' names no live element (kNoLiveMenuElement) - closing the"
                   " unanswerable menu and releasing its interrupt window");
        observer->mLayers->stackLayer()->endOfInterruption(false);
    }
    menuObject = 0;
    currentActionCard = NULL;
    menuObjectName.clear();
    menuObjectText.clear(); //#W65-AL (G1)
}

ActionLayer::ActionLayer(GameObserver *observer)
    : GuiLayer(observer)
{
    menuObject = NULL;
    abilitiesMenu = NULL;
    abilitiesTriggered = NULL;
    menuArmedSerial = 0;
    stuffHappened = 0;
    currentWaitingAction = NULL;
    cantCancel = 0;
    mGarbageEntryBeingDeleted = NULL; //#W84-GB (review-2 item 1)
}

ActionLayer::~ActionLayer()
{
    while(mObjects.size())
        moveToGarbage((ActionElement *) mObjects[mObjects.size() - 1]);
    SAFE_DELETE(abilitiesMenu);
    //A GenericInstantAbility wrapper ALSO addToGame's its nested ability
    //(see GenericInstantAbility::addToGame), so at abrupt game teardown -
    //when its destroy() never ran to detach them - BOTH the wrapper and
    //the nested element sit in this garbage list, and the wrapper's
    //destructor would double-delete the nested one (seen as a teardown
    //segfault when a lord's per-card may was still alive at suite end -
    //Mind's Dilation). Detach such pairs before the final sweep; order
    //of deletion then no longer matters.
    for (size_t i = 0; i < garbage.size(); ++i)
    {
        GenericInstantAbility * gia = dynamic_cast<GenericInstantAbility *>(garbage[i]);
        if (!gia || !gia->ability)
            continue;
        for (size_t j = 0; j < garbage.size(); ++j)
        {
            if ((ActionElement *) gia->ability == garbage[j])
            {
                gia->ability = NULL;
                break;
            }
        }
    }
    cleanGarbage();
}
