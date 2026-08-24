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
    //#W43-6. Bumped by every setMenuObject / setCustomMenuObject. A menu's
    //IDENTITY cannot be read off menuObject (the alternative-cost menu and the
    //{X} announcement it chains into are both armed on the SAME card) nor off
    //the abilitiesMenu pointer (freed and re-allocated, so the address can
    //repeat). ButtonPressed compares this serial across the rule callback to
    //tell "the menu I answered is finished" from "the callback armed a NEW
    //decision on the same card" - the second must not be thrown away.
    unsigned int menuArmedSerial;
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

protected:
    ActionElement * currentWaitingAction;
    int cantCancel;
    std::set<ActionElement*> mReactions;
};

#endif
