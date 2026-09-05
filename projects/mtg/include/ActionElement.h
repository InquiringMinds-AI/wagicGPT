/*
 *  Wagic, The Homebrew ?! is licensed under the BSD license
 *  See LICENSE in the Folder's root
 *  http://wololo.net/wagic/
 */

#ifndef _ACTIONELEMENT_H_
#define _ACTIONELEMENT_H_
#include <JGui.h>
#include "MTGDefinitions.h"

class MTGCardInstance;
class ManaCost;
class Targetable;
class TargetChooser;
class WEvent;

class ActionElement: public JGuiObject
{
protected:
    enum Activity{
        Inactive,
        ActionRequested,
        Active
    };

    Activity activity;
    TargetChooser * tc;
public:
    //#W61-R (C4): read-only access to the chooser an element is holding. A
    //TargetAbility keeps the pilot's picks here until `TargetAbility::resolve`
    //copies them into the payload ability's `target`, so an ability sitting on
    //the stack has its chosen targets HERE and nowhere else - which is what the
    //prompt's ON THE STACK total needs to price a `target(anytarget)` ping.
    //Const-correct and non-owning; nothing outside may replace or free it.
    TargetChooser * chosenTargets() const { return tc; }
    GamePhase currentPhase;
    GamePhase newPhase;
    int modal;
    int waitingForAnswer;
    virtual void Update(float){}
    virtual void Render(){}
    virtual int testDestroy()
    {
        return 0;
    }
    virtual int destroy()
    {
        return 0;
    }
    virtual bool CheckUserInput(JButton)
    {
        return false;
    }
    ActionElement(int id);
    ActionElement(const ActionElement& copyFromMe);
    TargetChooser * getActionTc(){return tc;}
    virtual void setActionTC(TargetChooser * newTc = NULL){this->tc = newTc;}
    virtual ~ActionElement();
    virtual int isReactingToTargetClick(Targetable * card);
    virtual int reactToTargetClick(Targetable * card);
    virtual int reactToChoiceClick(Targetable *,int,int)
    {
        return 0;
    }
    virtual int isReactingToClick(MTGCardInstance *, ManaCost * = NULL)
    {
        return 0;
    }
    virtual int stillInUse(MTGCardInstance *)
    {
        return 0;
    }
    virtual int receiveEvent(WEvent *)
    {
        return 0;
    }
    virtual int reactToClick(MTGCardInstance *)
    {
        return 0;
    }
    virtual const string getMenuText()
    {
        return "Ability";
    }
    virtual ActionElement * clone() const = 0;

};

#endif
