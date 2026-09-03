#ifndef _PLAY_RESTRICTIONS_H_
#define _PLAY_RESTRICTIONS_H_


class TargetChooser;
class MTGCardInstance;
class MTGGameZone;
class GameObserver;

class PlayRestriction 
{

public:
    
    enum
    {
        CAN_PLAY,
        CANT_PLAY,
        NO_OPINION
    };

    TargetChooser * tc;

    virtual int canPutIntoZone(MTGCardInstance * card, MTGGameZone * destZone) = 0;

    PlayRestriction(TargetChooser * tc);
    virtual ~PlayRestriction();
};

class MaxPerTurnRestriction: public PlayRestriction
{
public:
    enum
    {
        NO_MAX = -1,
    };
    int maxPerTurn;
    MTGGameZone * zone;
    MaxPerTurnRestriction(TargetChooser * tc, int maxPerTurn, MTGGameZone * zone);
    int canPutIntoZone(MTGCardInstance * card, MTGGameZone * destZone);
    //#W56-W (E-2): "is this restriction's quota still open?" - the SAME count
    //canPutIntoZone gates on, promoted to its own name so a caller that cannot
    //supply a matching card (the modal-DFC land, whose hand card is a Sorcery)
    //asks the identical question instead of re-deriving it. canPutIntoZone
    //calls this, so the two can never disagree.
    bool quotaLeft();
};


class PlayRestrictions
{
protected:
    vector<PlayRestriction *>restrictions;
public:
    MaxPerTurnRestriction * getMaxPerTurnRestrictionByTargetChooser(TargetChooser * tc);

    void addRestriction(PlayRestriction * restriction);
    void removeRestriction(PlayRestriction * restriction);
    int canPutIntoZone(MTGCardInstance * card, MTGGameZone * destZone);
    //#W56-W (E-2): "does this player still have a land drop this turn?", asked
    //about the LAND TYPE rather than about one card. canPutIntoZone answers
    //NO_OPINION for a card the land restriction's TargetChooser cannot target,
    //and a modal double-faced card in hand is a Sorcery whose BACK face is the
    //land - so asking it about that card silently reported a free drop. Finds
    //the lands-per-turn restriction the rules card registered (maxPlay(land)N)
    //and reads its quota. TRUE when no such restriction exists at all.
    bool landDropAvailable(GameObserver * observer, MTGCardInstance * contextCard);
    ~PlayRestrictions();

};
#endif
