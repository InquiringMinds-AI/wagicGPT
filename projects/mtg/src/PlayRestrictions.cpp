#include "PrecompiledHeader.h"

#include "PlayRestrictions.h"
#include "TargetChooser.h"
#include "MTGCardInstance.h"


PlayRestriction::PlayRestriction(TargetChooser * tc): tc(tc)
{
    tc->setAllZones(); // This is to allow targetting cards without caring about the actual zone
    tc->targetter = NULL;
};

PlayRestriction::~PlayRestriction()
{
    SAFE_DELETE(tc);
};


MaxPerTurnRestriction::MaxPerTurnRestriction(TargetChooser * tc, int maxPerTurn, MTGGameZone * zone): 
    PlayRestriction(tc), maxPerTurn(maxPerTurn), zone(zone)
    {}

//#W56-W (E-2): the quota test, alone. ONE definition, two callers -
//canPutIntoZone (a card is being put somewhere) and landDropAvailable (no card
//of the right type exists to ask about).
bool MaxPerTurnRestriction::quotaLeft()
{
    if (maxPerTurn == NO_MAX) return true;
    if (!zone) return true;
    return zone->seenThisTurn(tc, Constants::CAST_ALL) < maxPerTurn;
}

int  MaxPerTurnRestriction::canPutIntoZone(MTGCardInstance * card, MTGGameZone * destZone)
{
    if (destZone != zone)
        return PlayRestriction::NO_OPINION;

    if (!tc->canTarget(card))
        return PlayRestriction::NO_OPINION;

    if (!quotaLeft())
        return PlayRestriction::CANT_PLAY;

    return PlayRestriction::CAN_PLAY;
};


MaxPerTurnRestriction * PlayRestrictions::getMaxPerTurnRestrictionByTargetChooser(TargetChooser * tc)
{
    TargetChooser * _tc = tc->clone();
     _tc->setAllZones(); // we don't care about the actual zone for the "equals" check

    for (vector<PlayRestriction *>::iterator iter = restrictions.begin(); iter != restrictions.end(); ++iter)
    {
        MaxPerTurnRestriction * mptr = dynamic_cast<MaxPerTurnRestriction *> (*iter);
        if (mptr && mptr->tc->equals(_tc))
        {
            delete _tc;
            return mptr;
        }
    }

    delete _tc;
    return NULL;
}

//#W56-W (E-2). See the header: the land-drop counter, asked by TYPE. The probe
//TargetChooser is built from the same "land" spec the rules card's
//maxPlay(land)1 registered, so getMaxPerTurnRestrictionByTargetChooser finds
//exactly the restriction MTGPutInPlayRule consults for a normal land drop.
bool PlayRestrictions::landDropAvailable(GameObserver * observer, MTGCardInstance * contextCard)
{
    if (!observer)
        return true;
    TargetChooserFactory tcf(observer);
    TargetChooser * probe = tcf.createTargetChooser("land", contextCard);
    if (!probe)
        return true;
    MaxPerTurnRestriction * r = getMaxPerTurnRestrictionByTargetChooser(probe);
    delete probe;
    if (!r)
        return true;
    return r->quotaLeft();
}

void PlayRestrictions::addRestriction(PlayRestriction * restriction)
{
    //TODO control that the id does not already exist?
    restrictions.push_back(restriction);

}

void PlayRestrictions::removeRestriction(PlayRestriction * restriction)
{
    for (vector<PlayRestriction *>::iterator iter = restrictions.begin(); iter != restrictions.end(); ++iter)
    {
        if(*iter == restriction)
        {
            restrictions.erase(iter);
            return;
        }
    }
}

int  PlayRestrictions::canPutIntoZone(MTGCardInstance * card, MTGGameZone * destZone)
{
    if (!card)
        return PlayRestriction::CANT_PLAY;

    for (vector<PlayRestriction *>::iterator iter = restrictions.begin(); iter != restrictions.end(); ++iter)
    {
            if ((*iter)->canPutIntoZone(card, destZone) == PlayRestriction::CANT_PLAY)
                return PlayRestriction::CANT_PLAY;
    }

    return PlayRestriction::CAN_PLAY;
}

PlayRestrictions::~PlayRestrictions()
{
    for (vector<PlayRestriction *>::iterator iter = restrictions.begin(); iter != restrictions.end(); ++iter)
    {
        SAFE_DELETE(*iter);
    }
    restrictions.clear();
}
