/* Graphical representation of a Card Instance, used in game */

#ifndef _CARD_GUI_H_
#define _CARD_GUI_H_

#include <hge/hgeparticle.h>
#include <JGui.h>
#include "Pos.h"
#include "PlayGuiObject.h"
#include "WResource_Fwd.h"

class MTGCard;
class MTGCardInstance;
class PlayGuiObject;

namespace DrawMode
{
    enum
    {
        kNormal  = 0,
        kText,
        kHidden
    };
    const int kNumDrawModes = 3;
}

//==== audit-J (W54-J) ====
//"Was it me?" switch for every render-path cache this lane added (the HUD
//quad statics and resolved type ids in CardGui.cpp, the lazy battlefield
//relayout in GuiPlay.cpp): compile with -DWAGIC_RENDER_NOCACHE, or in a
//development build (_DEBUG / WAGIC_DEVLOGS) set env WAGIC_RENDER_NOCACHE=1.
//Either routes every lookup back through the original string-keyed path and
//every event back to an eager Replace().
bool wagicRenderCacheOff();
//==== end audit-J ====

//==== #W57-G (D42): Arena-style board stacking ====
//Owner ruling 2026-09-03: "it should seperate out any unique copies, for
//instance if they have summoning sickness, or if an equipment is attached, or
//if some are tapped. only items with identical names and states should stack."
//wagicBoardStackKey is that "identical name and state" predicate, expressed as
//a string: two battlefield permanents stack IFF their keys are byte-equal.
//The key is deliberately over-inclusive - any field that the board render, the
//rules or the click surface can distinguish is in it, because a false stack is
//a lie about the board and the trust doctrine forbids that. It is render-side
//only; no engine code reads it.
bool wagicBoardGroupingEnabled();
std::string wagicBoardStackKey(MTGCardInstance * card);
//==== end #W57-G ====

struct CardGui: public PlayGuiObject
{
protected:

    static map<string, string>counterGraphics;

    /*
    ** Tries to render the Big version of a card picture, backups to text version in case of failure
    */
    static void RenderBig(MTGCard * card, const Pos& pos, bool thumb = false, bool noborder = false, bool gdv = false);

    static void RenderCountersBig(MTGCard * card, const Pos& pos, int drawMode = DrawMode::kNormal);
    static void AlternateRender(MTGCard * card, const Pos& pos);
    static void TinyCropRender(MTGCard * card, const Pos& pos, JQuad * quad);
    static string FormattedData (string data, string replace, string value);
    static bool FilterCard (MTGCard * card,string filter);

public:
    static const float Width;
    static const float Height;
    static const float BigWidth;
    static const float BigHeight;

    PIXEL_TYPE mMask;

    //#W57-G (D42): stacking state, owned by GuiPlay's layout pass.
    //mStackCount > 1 on the one member that DRAWS for a group (it carries the
    //xN badge); mStackHidden on the members it stands in for - those are not
    //drawn and share the drawn member's slot, so the cursor cannot reach them
    //until the group expands. Both are 1/false for an ungrouped card, which is
    //exactly what the pre-D42 board is.
    int mStackCount;
    bool mStackHidden;
    //Fan offset of an expanded group's member: 0 = this card owns a layout
    //slot, >0 = it is drawn at the group's slot plus mStackFanIndex pitches.
    //Fanning IN PLACE is deliberate - focusing a pile must not shove the rest
    //of the row sideways while the player is reading it.
    int mStackFanIndex;
    //Set by CardSelector when a click lands on a collapsed group while a
    //chooser is live: that press EXPANDS the group instead of targeting,
    //because the player must see what they are choosing among.
    bool mStackForceExpand;

    MTGCardInstance* card;
    CardGui(MTGCardInstance* card, float x, float y);
    CardGui(MTGCardInstance* card, const Pos& ref);
    virtual void Render();
    virtual void Update(float dt);

    void DrawCard(const Pos& inPosition, int inMode = DrawMode::kNormal, bool thumb = false, bool noborder = false, bool gdv = false);
    static void DrawCard(MTGCard* inCard, const Pos& inPosition, int inMode = DrawMode::kNormal, bool thumb = false, bool noborder = false, bool gdv = false);

    static JQuadPtr AlternateThumbQuad(MTGCard * card);
    virtual ostream& toString(ostream&) const;
};

class CardView: public CardGui
{
public:

    typedef enum
    {
        nullZone, handZone, playZone
    } SelectorZone;

    const SelectorZone owner;

    MTGCardInstance* getCard(); // remove this when possible
    CardView(const SelectorZone, MTGCardInstance* card, float x, float y);
    CardView(const SelectorZone, MTGCardInstance* card, const Pos& ref);
    virtual ~CardView();

    void Render()
    {
        CardGui::Render();
    }
    
    void Render(JQuad* q)
    {
        Pos::Render(q);
    }
    
    virtual ostream& toString(ostream&) const;

    float GetCenterX();
    float GetCenterY();
};

class TransientCardView: public CardGui
{
public:
    TransientCardView(MTGCardInstance* card, float x, float y);
    TransientCardView(MTGCardInstance* card, const Pos& ref);
};


class SimpleCardEffect 
{
public:
    virtual void doEffect(Pos * card) = 0;
    virtual void undoEffect(Pos * card) = 0;
    virtual ~SimpleCardEffect(){};
};

class SimpleCardEffectRotate:public SimpleCardEffect 
{
protected:
    float mRotation;
public:
    SimpleCardEffectRotate(float rotation);
    void doEffect(Pos * card);
    void undoEffect(Pos * card);
};

class SimpleCardEffectMask:public SimpleCardEffect 
{
protected:
    PIXEL_TYPE mMask;
public:
    SimpleCardEffectMask(PIXEL_TYPE mask);
    void doEffect(Pos * card);
    void undoEffect(Pos * card);
};


#endif
