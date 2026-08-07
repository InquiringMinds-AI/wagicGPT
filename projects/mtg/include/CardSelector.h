#ifndef _CARDSELECTOR_H_
#define _CARDSELECTOR_H_

#include <vector>
#include <stack>
#include "CardGui.h"
#include "GuiLayers.h"
#include "Pos.h"

using std::vector;

class PlayGuiObject;
class DuelLayers;
class MTGCardInstance;

// The X lib annoyingly defines True to be 1, leading to
// hard to understand syntax errors. Not using it, so it's
// safe to undefine it.
#ifdef True
#undef True
#endif

template<typename T>
struct LimitorFunctor
{
    virtual bool select(T*) = 0;
    virtual bool greyout(T*) = 0;
    typedef T Target;
};

class CardSelectorBase: public GuiLayer
{
public:

    CardSelectorBase(GameObserver *observer, int inDrawMode = DrawMode::kNormal) :
        GuiLayer(observer), mDrawMode(inDrawMode)

    {
    }
    ;
    virtual void Add(PlayGuiObject*) = 0;
    virtual void Remove(PlayGuiObject*) = 0;
    virtual bool CheckUserInput(JButton key) = 0;
    virtual void PushLimitor() = 0;
    virtual void PopLimitor() = 0;
    virtual void Limit(LimitorFunctor<PlayGuiObject>* inLimitor, CardView::SelectorZone inZone) = 0;
    virtual void Push() = 0;
    virtual void Pop() = 0;
    //Move the cursor to the next candidate in a zone, in stable reading order.
    //This exists so game rules can advance the selection WITHOUT firing a
    //synthetic direction press: that route measures screen distance, and a rule
    //that also re-lays-out the board (declaring an attacker does) ends up
    //measuring against coordinates its own click just moved. Returns false and
    //leaves the cursor alone when there is no candidate.
    virtual bool SelectNextInZone(LimitorFunctor<PlayGuiObject>*, CardView::SelectorZone,
                                  MTGCardInstance* = NULL)
    {
        return false;
    }
    virtual int GetDrawMode()
    {
        return mDrawMode;
    }

protected:
    int mDrawMode;
};

class CardSelector: public CardSelectorBase
{
public:
    struct SelectorMemory
    {
        PlayGuiObject* object;
        float x, y;
        SelectorMemory(PlayGuiObject* object);
        SelectorMemory();
    };

protected:
    vector<PlayGuiObject*> cards;
    PlayGuiObject* active;
    DuelLayers* duel;
    LimitorFunctor<PlayGuiObject>* limitor;
    Pos bigpos;
    map<const CardView::SelectorZone, SelectorMemory> lasts;
    stack<pair<LimitorFunctor<PlayGuiObject>*, CardView::SelectorZone> > limitorStack;
    stack<SelectorMemory> memoryStack;

    PlayGuiObject* fetchMemory(SelectorMemory&);
    float timer;

public:
    CardSelector(GameObserver *observer, DuelLayers*);
    void Add(PlayGuiObject*);
    void Remove(PlayGuiObject*);
    bool CheckUserInput(JButton key);
    void Update(float dt);
    void Render();
    void Push();
    void Pop();
    bool SelectNextInZone(LimitorFunctor<PlayGuiObject>* limitor, CardView::SelectorZone zone,
                          MTGCardInstance* skip = NULL);

    void Limit(LimitorFunctor<PlayGuiObject>* limitor, CardView::SelectorZone);
    void PushLimitor();
    void PopLimitor();

    typedef PlayGuiObject Target;
};

typedef LimitorFunctor<CardSelector::Target> Limitor;

struct Exp
{
    static inline bool test(CardSelector::Target*, CardSelector::Target*);
};

#endif
