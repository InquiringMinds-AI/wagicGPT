/*
 *  Wagic, The Homebrew ?! is licensed under the BSD license
 *  See LICENSE in the Folder's root
 *  http://wololo.net/wagic/
 */

#ifndef _ACTIONLAYER_H_
#define _ACTIONLAYER_H_

#include "GuiLayers.h"
#include "ActionElement.h"
#include "SimpleMenu.h"
#include "MTGAbility.h"

#include <set>

class GuiLayer;
class Targetable;
class WEvent;

class ActionLayer: public GuiLayer, public JGuiListener
{
public:
    vector<ActionElement *> garbage;
    Targetable * menuObject;
    SimpleMenu * abilitiesMenu;
    SimpleMenu * abilitiesTriggered;
    MTGCardInstance * currentActionCard;
    //#W48 D6: the NAME of the object this menu was armed on, captured at arm
    //time. A menu armed by a resolving spell's `choice` payload (Silverquill
    //Command, Peer into the Abyss) holds a currentActionCard that names itself
    //perfectly but is in NO game zone, so every consumer-side pointer validation
    //(the dangle rail) refuses it and the prompt header fell back to the
    //subject-less "A choice is required". A string captured while the object is
    //provably alive cannot dangle, so the name survives the validation the
    //pointer cannot. Empty when the object had no name.
    std::string menuObjectName;
    //#W65-AL (G1). The same rail as menuObjectName, for the same reason and at
    //the same three sites. Wave 64 priced modal rows off `ctx->magicText`, and
    //`ctx` is the validated menu-subject POINTER: a menu armed by a resolving
    //spell's `auto=choice` payload (Silverquill Command, Peer into the Abyss)
    //is armed on a card in NO game zone, so the dangle rail refuses the pointer
    //and every ctx-gated annotation went silent while the header - built from
    //the captured NAME - kept naming the card. 23 mode rows over 3 windows in
    //the wave-64 corpus rendered bare, and the seat took `life:-1 controller`
    //at 5 life and again at 3. A SCRIPT captured while the object is provably
    //alive cannot dangle either. Empty when the object had no script.
    std::string menuObjectText;
    //#W43-6. Bumped by every setMenuObject / setCustomMenuObject. A menu's
    //IDENTITY cannot be read off menuObject (the alternative-cost menu and the
    //{X} announcement it chains into are both armed on the SAME card) nor off
    //the abilitiesMenu pointer (freed and re-allocated, so the address can
    //repeat). ButtonPressed compares this serial across the rule callback to
    //tell "the menu I answered is finished" from "the callback armed a NEW
    //decision on the same card" - the second must not be thrown away.
    unsigned int menuArmedSerial;
    //#W58-F (F1): a SimpleMenu row stores ONLY its position in mObjects. That
    //position is not an identity: removeFromGame erases from the middle of the
    //vector, so an ability leaving the game between the moment a menu is armed
    //and the moment it is answered shifts every later row's meaning - the id
    //can land past the end (the F1 SIGABRT) or, worse, still in range and
    //naming a DIFFERENT ability. This vector captures, per menu row, the
    //ActionElement the row was built from, so every id -> mObjects mapping can
    //be resolved by identity instead of position. Entries are nulled when their
    //element leaves the game; a row whose element is gone yields no decision.
    vector<ActionElement *> menuRowElements;
    int stuffHappened;
    //destroy()-in-progress stack: an element's destroy() can cascade into
    //more removals (a lord's destroy removes the abilities it granted -
    //ALord::removed -> removeObserver -> removeFromGame again). When the
    //cascade circles back to an element already mid-destroy (mutual
    //lord/grant death in one sweep), running destroy() a second time is
    //the double-destroy crash class - the outer call owns the removal.
    //NOTE deferring nested removals wholesale is UNSAFE here: registration
    //and ownership are decoupled (a parent's destructor deletes children
    //that are still registered), so a deferred element can be freed while
    //it still sits in mObjects (teardown crash, core 4068495).
    vector<ActionElement *> mDestroying;
    virtual void Render();
    virtual void Update(float dt);
    bool CheckUserInput(JButton key);
    ActionLayer(GameObserver *observer);
    ~ActionLayer();
    int cancelCurrentAction();
    ActionElement * isWaitingForAnswer();
    int isReactingToTargetClick(Targetable * card);
    int receiveEventPlus(WEvent * event);
    int reactToTargetClick(Targetable * card);
    int isReactingToClick(MTGCardInstance * card);
    bool getMenuIdFromCardAbility(MTGCardInstance *card, MTGAbility *ability, int& menuId);
    int reactToClick(MTGCardInstance * card);
    int reactToClick(ActionElement * ability, MTGCardInstance * card);
    int reactToTargetClick(ActionElement * ability, Targetable * card);
    int stillInUse(MTGCardInstance * card);
    void setMenuObject(Targetable * object, bool must = false);
    void setCustomMenuObject(Targetable * object, bool must = false,vector<MTGAbility*>abilities = vector<MTGAbility*>(),string customName = "");
    void ButtonPressed(int controllerid, int controlid);
    void ButtonPressedOnMultipleChoice(int choice = -1);
    void doReactTo(int menuIndex);
    //#W58-F (F1): the two directions of the menu-id mapping. Both return false
    //when the row's ability has left the game (no decision this tick, the menu
    //stays armed and the engine re-asks); on success `slot` is the ability's
    //CURRENT index in mObjects, re-pointed if the vector was compacted.
    bool getMenuControlId(int menuIndex, int & slot);
    bool getLiveMenuSlot(int controlid, int & slot);
    TargetChooser * getCurrentTargetChooser();
    void setCurrentWaitingAction(ActionElement * ae);
    MTGAbility * getAbility(int type);
    int checkCantCancel(){return cantCancel;};
    
    //Removes from game but does not move the element to garbage. The caller must take care of deleting the element.
    int removeFromGame(ActionElement * e);
    
    bool moveToGarbage(ActionElement * e);

    //True if e already sits in the garbage list (it left mObjects via its
    //own testDestroy). Owners with a delete-fallback (GenericInstantAbility
    //::destroy) must check this: the layer will delete it at cleanup, and
    //deleting it again dangles the garbage slot.
    bool isInGarbage(ActionElement * e);

    void cleanGarbage();

    //#W57-F (D25): the OWNERSHIP half of the dangling-target class. Cards in a
    //zone that is about to be freed (MTGPlayerCards::beforeBeginPhase deletes
    //last turn's garbage zone, MTGGameZone::~MTGGameZone deletes its cards) are
    //still named by abilities sitting in mObjects: MTGAbility::target is a raw
    //Targetable* with no clearing contract and MTGAbility::source is a raw
    //MTGCardInstance*. Wave 56 lane C made the AI's five DEREFS safe
    //(liveCardTarget); nothing made the POINTERS safe, and a->source is read by
    //MTGGameZone::hasCard (`card->currentZone`) on the very next ranking pass.
    //Called with the doomed cards still ALIVE, so an evicted ability's
    //destroy() contract runs exactly as it would have at any other time.
    int purgeDeadReferences(MTGGameZone * zone);

protected:
    ActionElement * currentWaitingAction;
    int cantCancel;
    std::set<ActionElement*> mReactions;
};

#endif
