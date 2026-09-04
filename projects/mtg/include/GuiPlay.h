#ifndef _GUIPLAY_H_
#define _GUIPLAY_H_

#include "GuiLayers.h"
#include "CardGui.h"

class GuiPlay: public GuiLayer
{
public:
    static const float HORZWIDTH;
    static const float VERTHEIGHT;
    typedef vector<CardView*>::iterator iterator;

protected:
    class CardStack
    {
    protected:
        unsigned total;
        float baseX, baseY;
        float x, y;

    public:
        void reset(unsigned total, float x, float y);
        void Enstack(CardView*);
        void RenderSpell(MTGCardInstance*, iterator begin, iterator end, float x, float y);
    };

    class HorzStack: public CardStack
    {
    public:
        HorzStack();
        void Render(CardView*, iterator begin, iterator end);
        void Enstack(CardView*);
    };
    class VertStack: public CardStack
    {
    protected:
        unsigned count;
    public:
        VertStack();
        void reset(unsigned total, float x, float y);
        void Render(CardView*, iterator begin, iterator end);
        void Enstack(CardView*);
        inline float nextX();
    };
    class BattleField: public HorzStack
    {
        static const float HEIGHT;
        unsigned attackers;
        unsigned currentAttacker;
        float height;

    public:
        int red;
        int colorFlow;

        void addAttacker(MTGCardInstance*);
        void removeAttacker(MTGCardInstance*);
        void reset(float x, float y);
        BattleField();
        void EnstackAttacker(CardView*);
        void EnstackBlocker(CardView*);
        void Update(float dt);
        void Render();
    };

    class Lands: public HorzStack {};
    class Creatures: public HorzStack {};
    class Planeswalker: public HorzStack {};
    class Spells: public VertStack {};

protected:
    Creatures selfCreatures, opponentCreatures;
    BattleField battleField;
    Lands selfLands, opponentLands;
    Spells selfSpells, opponentSpells;
    Planeswalker selfPlaneswalker,opponentPlaneswalker;
    iterator end_spells;

    vector<CardView*> cards;
    //#W54-J (A25): set by every game event that reaches this layer (the old
    //unconditional trailing Replace() kept the same coverage); consumed by
    //relayoutIfDirty() at most once per Update and once per Render.
    bool mLayoutDirty;
    void relayoutIfDirty();

    //#W57-G (D42): Arena-style stacking of state-identical permanents.
    //computeStacks() runs at the head of every Replace() and decides, for each
    //battlefield CardView, whether it draws (and with what xN badge), whether
    //it is a follower another card stands in for, or whether it is fanned out
    //of an expanded pile. Layout then allocates slots ONLY to the drawing
    //cards, so a board of 8 identical tokens costs one slot and one
    //CardGui::Render instead of eight.
    //Two hard rules from the owner's rulings live here:
    //  - only identical NAME AND STATE stack (wagicBoardStackKey is the
    //    predicate; any difference splits the pile);
    //  - while a chooser is live or anything on the stack is still unresolved,
    //    the whole battlefield expands to its ungrouped layout and STAYS
    //    there, because "the user remembers what they have targeted" by
    //    position and nothing may move under them mid-decision.
    static const float STACKFANPITCH;
    bool mStacksPinned;
    const void * mStackSig;
    vector<pair<CardView*, CardView*> > mStackFollowers; //follower, drawn card
    void computeStacks();
    bool stacksPinnedNow();
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    void stackProbe(double ms);
    void churnProbe(); //#W58-E (D42)
#endif

public:
    int wave;
    GuiPlay(DuelLayers*);
    ~GuiPlay();
    virtual void Render();
    //#W58-E (D42): the xN pile badges, drawn by DuelLayers AFTER every layer so
    //nothing can paint over them (the owner's "indicators are not remaining on
    //top"). Public because DuelLayers::Render is the only caller.
    void RenderStackBadges();
    void Replace();
    void Update(float dt);
    virtual int receiveEventPlus(WEvent * e);
    virtual int receiveEventMinus(WEvent * e);
};

#endif // _GUIPLAY_H_
