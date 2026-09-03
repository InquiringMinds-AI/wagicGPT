/*
* CardGui.cpp
* This class is in charge of rendering Cards on the screen
*/

#include "PrecompiledHeader.h"

#if defined(WAGIC_AUTODEMO) || defined(WAGIC_HWPROBE)
#include <stdarg.h>
static void cardProbe(const char* fmt, ...)
{
    FILE* f = fopen("User/wagic-probe.log", "a");
    if (!f) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fprintf(f, "\n");
    fclose(f);
}
#else
#define cardProbe(...) ((void)0)
#endif

#include "JGE.h"
#include "CardGui.h"
#include "ManaCostHybrid.h"
#include "ExtraCost.h"
#include "Subtypes.h"
#include "Translate.h"
#include "MTGDefinitions.h"
#include "GameObserver.h"
#include <Vector2D.h>
#include "Counters.h"
#include "ModRules.h"
#include "CardDescriptor.h"
#include "GameApp.h"

const float CardGui::Width = 28.0;
const float CardGui::Height = 40.0;
const float CardGui::BigWidth = 200.0;
const float CardGui::BigHeight = 285.0;

const float kWidthScaleFactor = 0.8f;

map<string, string> CardGui::counterGraphics;

namespace
{
    inline float SineHelperFunction(const float& value)
    {
        return sinf(2 * M_PI * (value) / 256.0f);
    }

    inline float CosineHelperFunction(const float& value)
    {
        return cosf(2 * M_PI * (value - 35) / 256.0f);
    }

}

CardGui::CardGui(MTGCardInstance* card, float x, float y)
    : PlayGuiObject(Height, x, y, 0, false), mStackCount(1), mStackHidden(false), mStackFanIndex(0),
      mStackForceExpand(false), card(card) //#W57-G (D42)
{
}
CardGui::CardGui(MTGCardInstance* card, const Pos& ref)
    : PlayGuiObject(Height, ref, 0, false), mStackCount(1), mStackHidden(false), mStackFanIndex(0),
      mStackForceExpand(false), card(card) //#W57-G (D42)
{
}

float CardView::GetCenterX()
{
    bool largeCard = mHeight == BigHeight;

    float centerX = x + (largeCard ? BigWidth : Width) * 0.5f * zoom;
    return centerX;
}

float CardView::GetCenterY()
{
    bool largeCard = mHeight == BigHeight;

    float centerY = y + (largeCard ? BigHeight : Height) * 0.5f * zoom;
    return centerY;
}


CardView::CardView(const SelectorZone owner, MTGCardInstance* card, float x, float y)
    : CardGui(card, x, y), owner(owner)
{
    const Pos* ref = card->view;
    while (card)
    {
        if (ref == card->view)
            card->view = this;
        card = card->next;
    }
}

CardView::CardView(const SelectorZone owner, MTGCardInstance* card, const Pos& ref)
    : CardGui(card, ref), owner(owner)
{
    const Pos* r = card->view;
    while (card)
    {
        if (r == card->view)
            card->view = this;
        card = card->next;
    }
}

CardView::~CardView()
{
    if (card)
    {
        const Pos* r = this;
        while (card)
        {
            if (r == card->view)
                card->view = NULL;
            card = card->next;
        }
    }
}

void CardGui::Update(float dt)
{
    PlayGuiObject::Update(dt);
}

//==== audit-J (W54-J, A5) ====
//Per-frame render-path caches. CONTRACT for every JQuadPtr held here across
//frames: only MANAGED quads (GetQuad(name) -> the deadbolted managed map) are
//kept. A managed texture is never evicted (deadbolt), and a theme/profile
//Refresh repoints its tracked quads' texture IN PLACE (WCachedTexture::
//Refresh), so the pointer stays valid for the life of the resource manager;
//the cache is keyed on the manager pointer so a different manager re-resolves
//(the singleton is only ever torn down at GameApp exit). Evictable
//(RETRIEVE_NORMAL / RetrieveTempQuad) quads are NEVER cached here: WCache::
//isLocked() ignores tracked quads, so a held JQuadPtr would outlive its
//texture after RemoveOldest/ClearUnlocked. Card art goes through RetrieveCard
//every frame for the same reason (and because that call is what keeps the
//on-screen textures' LRU stamp fresh); it was made cheap instead.
bool wagicRenderCacheOff()
{
#if defined(WAGIC_RENDER_NOCACHE)
    return true;
#elif defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    static int state = -1;
    if (state < 0) state = getenv("WAGIC_RENDER_NOCACHE") ? 1 : 0;
    return state == 1;
#else
    return false;
#endif
}

//==== #W57-G (D42): Arena-style board stacking ====
//The stack predicate. Two battlefield permanents draw as one pile IFF this
//string matches byte-for-byte, so EVERY observable difference has to be in it:
//the owner's ruling names tapped / summoning-sick / equipped explicitly and
//then generalises - "only items with identical names and states should stack."
//An over-inclusive key costs one un-merged pile; an under-inclusive one tells
//the player a lie about the board, so when in doubt a field goes IN.
bool wagicBoardGroupingEnabled()
{
    //Env override first, and only in a development build - the shipping switch
    //is the profile option, which defaults ON (GameOptions.cpp mints the 1;
    //an unset option would otherwise read 0 and ship the feature dead).
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    static int state = -1;
    if (state < 0)
    {
        const char * e = getenv("WAGIC_BOARD_GROUPING");
        state = e ? (atoi(e) ? 1 : 0) : -2;
    }
    if (state >= 0)
        return state == 1;
#endif
    return options[Options::BOARDGROUPING].number != 0;
}

static void appendStackAttachments(std::ostringstream& k, MTGCardInstance * card)
{
    //Auras, equipment and anything else that points its `target` at this
    //permanent. Sorted so two identically-outfitted creatures whose attachments
    //arrived in a different order still stack, and stamped with the attachment's
    //own visible state (a tapped Equipment reads differently on the board).
    GameObserver * g = card->getObserver();
    if (!g)
        return;
    std::vector<std::string> att;
    for (int p = 0; p < 2; ++p)
    {
        if (!g->players[p] || !g->players[p]->game)
            continue;
        MTGGameZone * z = g->players[p]->game->inPlay;
        if (!z)
            continue;
        for (int i = 0; i < z->nb_cards; ++i)
        {
            MTGCardInstance * a = z->cards[i];
            if (!a || a == card)
                continue;
            if (a->target != card && a->auraParent != card)
                continue;
            std::ostringstream one;
            one << a->getName() << ':' << a->isTapped() << ':'
                << (a->counters ? a->counters->mCount : 0) << ':'
                << (const void *) a->controller();
            att.push_back(one.str());
        }
    }
    std::sort(att.begin(), att.end());
    for (size_t i = 0; i < att.size(); ++i)
        k << att[i] << ';';
}

std::string wagicBoardStackKey(MTGCardInstance * card)
{
    if (!card)
        return "";
    std::ostringstream k;
    //printed identity
    k << card->getName() << '|' << card->getMTGId() << '|'
      << (card->model && card->model->data ? card->model->data->name : card->getName()) << '|';
    //who controls it, and who owns it
    k << (const void *) card->controller() << '|' << (const void *) card->owner << '|';
    //tap / untap / sickness / phasing / freeze / exert
    k << card->isTapped() << ',' << card->isUntapping() << ','
      << card->hasSummoningSickness() << ',' << (card->isPhased ? 1 : 0) << ','
      << card->phasedTurn << ',' << card->frozen << ',' << (card->exerted ? 1 : 0) << '|';
    //printed and live P/T, every bonus channel, and the marked-damage residue
    //(the small card renders power/life, so both have to be in the key)
    k << card->power << '/' << card->life << '/' << card->toughness << '/'
      << card->getCurrentPower() << '/' << card->getCurrentToughness() << '/'
      << card->origpower << '/' << card->origtoughness << '/'
      << card->basepower << '/' << card->basetoughness << '/'
      << card->pbonus << '/' << card->tbonus << '/'
      << (card->isSwitchedPT ? 1 : 0) << '/'
      //swapP/swapT are only ever written by MTGCardInstance::switchPT - nothing
      //initialises them - so on a card that never switched they hold stack
      //garbage that differs per instance. Reading them unconditionally made
      //EVERY pair of otherwise-identical permanents look different and the
      //whole feature never fired once (found live, 2026-09-03). They are only
      //observable while the switch is active, so only read them there. The
      //uninitialised members themselves are an engine finding for the ledger,
      //not something a render lane should be quietly patching around.
      << (card->isSwitchedPT ? card->swapP : 0) << '/'
      << (card->isSwitchedPT ? card->swapT : 0) << '/'
      << card->wasDealtDamage << '|';
    //combat assignment: role, WHICH attacker/defender, damage order rank
    k << card->isAttacker() << ',' << (card->isDefenser() ? 1 : 0) << ','
      << (card->isBlocked() ? 1 : 0) << ',' << card->didattacked << ','
      << card->didblocked << ',' << card->notblocked << ','
      << card->willattackplayer << ',' << card->willattackpw << ','
      << (const void *) card->defenser << ',' << (const void *) card->isAttacking << ','
      << (const void *) card->banding << ',' << card->blockers.size() << ','
      << (card->defenser ? card->defenser->getDefenserRank(card) : 0) << '|';
    //face / flip / morph / transform state
    k << (card->isFacedown ? 1 : 0) << ',' << (card->morphed ? 1 : 0) << ','
      << (card->isMorphed ? 1 : 0) << ',' << (card->turningOver ? 1 : 0) << ','
      << card->isFlipped << ',' << (card->hasCopiedToken ? 1 : 0) << ','
      << card->copiedID << ',' << card->copiedSetID << ',' << (card->blinked ? 1 : 0) << ','
      << (card->isACopier ? 1 : 0) << ',' << card->MeldedFrom << '|';
    //counters, by kind and count
    if (card->counters)
        for (int i = 0; i < card->counters->mCount; ++i)
        {
            Counter * c = card->counters->counters[i];
            if (c)
                k << c->name << ',' << c->nb << ',' << c->power << ',' << c->toughness << ';';
        }
    k << '|';
    //the LIVE keyword set (until-EOT grants included) and the live type list
    k << card->basicAbilities.to_string() << '|';
    for (size_t i = 0; i < card->types.size(); ++i)
        k << card->types[i] << ',';
    k << '|' << (int) card->colors << '|';
    //every display flag the render turns into a border or a dim - two cards
    //wearing different borders are not the same board object to the player
    k << card->castableNow << ',' << card->willPayForFocused << ','
      << card->canAttackNow << ',' << card->hasUsableAbilityNow << ','
      << card->canBlockNow << ',' << card->forcedBorderA << ',' << card->forcedBorderB << ','
      << (card->isExtraCostTarget ? 1 : 0) << ',' << (card->has(Constants::NECROED) ? 1 : 0) << '|';
    //identity riders and pending-value state
    k << (card->isToken ? 1 : 0) << ',' << card->isCommander << ',' << card->isRingBearer << ','
      << (card->isBestowed ? 1 : 0) << ',' << (card->suspended ? 1 : 0) << ','
      << (card->miracle ? 1 : 0) << ',' << (card->isDefeated ? 1 : 0) << ','
      << (card->isCascaded ? 1 : 0) << ',' << (card->isDualWielding ? 1 : 0) << '|';
    k << card->X << ',' << card->castX << ',' << card->setX << ',' << card->kicked << ','
      << card->sunburst << ',' << card->mutation << ',' << card->auras << ','
      << card->equipment << ',' << card->regenerateTokens << ',' << card->flanked << ','
      //MaxLevelUp: same story - assigned only by the leveler parser, so it is
      //garbage on every non-leveler.
      << (card->isLeveler ? card->MaxLevelUp : 0) << ','
      << card->chooseacolor << ',' << card->chooseasubtype << ','
      << card->chooseaname << '|';
    //linked permanents: a paired / shackled / imprinted card is unique
    k << (const void *) card->myPair << ',' << (const void *) card->shackled << ','
      << (const void *) card->seized << ',' << (const void *) card->storedCard << ','
      << card->parentCards.size() << ',' << card->childrenCards.size() << ','
      << card->imprintedCards.size() << ',' << (const void *) card->hauntedCard << '|';
    //granted-ability population: two otherwise identical creatures with a
    //different number of live abilities attached are different objects
    k << card->cardsAbilities.size() << '|';
    appendStackAttachments(k, card);
    return k.str();
}
//==== end #W57-G ====

namespace
{
enum HudSlot { HUD_WHITE, HUD_SHADOW, HUD_EXTRACOSTSHADOW, HUD_C_WHITE, HUD_C_BLACK, HUD_C_GREEN, HUD_C_RED, HUD_C_BLUE, HUD_SLOTS };
const char* const kHudNames[HUD_SLOTS] = { "white", "shadow", "extracostshadow", "c_white", "c_black", "c_green", "c_red", "c_blue" };
struct HudQuad
{
    WResourceManager* owner;
    JQuadPtr quad;
    HudQuad() : owner(NULL) {}
};

//The managed HUD quads CardGui::Render draws with ("white" up to 7x per card
//per frame before this), resolved once per resource manager.
JQuadPtr hudQuad(WResourceManager* rm, int slot)
{
    if (!rm) return JQuadPtr();
    if (wagicRenderCacheOff()) return rm->GetQuad(kHudNames[slot]);
    static HudQuad cache[HUD_SLOTS];
    HudQuad& h = cache[slot];
    if (h.owner != rm || !h.quad.get())
    {
        h.quad = rm->GetQuad(kHudNames[slot]);
        h.owner = rm;
    }
    return h.quad;
}

//String-keyed type lookups take the subtype-list mutex per call
//(MTGAllCards::findType). Resolve the handful CardGui::Render tests to ids
//once; the list is append-only so an id never changes meaning.
struct RenderTypeIds
{
    int eldrazi, plains, swamp, forest, mountain, island;
    RenderTypeIds()
    {
        eldrazi  = MTGAllCards::findType("eldrazi");
        plains   = MTGAllCards::findType("plains");
        swamp    = MTGAllCards::findType("swamp");
        forest   = MTGAllCards::findType("forest");
        mountain = MTGAllCards::findType("mountain");
        island   = MTGAllCards::findType("island");
    }
};
const RenderTypeIds& renderTypeIds()
{
    static RenderTypeIds ids;
    return ids;
}
}
//==== end audit-J ====

void CardGui::DrawCard(const Pos& inPosition, int inMode, bool thumb, bool noborder, bool gdv)
{
    DrawCard(card, inPosition, inMode, thumb, noborder, gdv);
}

void CardGui::DrawCard(MTGCard* inCard, const Pos& inPosition, int inMode, bool thumb, bool noborder, bool gdv)
{
    switch (inMode)
    {
    case DrawMode::kNormal:
        RenderBig(inCard, inPosition, thumb, noborder, gdv);
        break;
    case DrawMode::kText:
        AlternateRender(inCard, inPosition);
        break;
    default:
        break;
    }
}

void CardGui::Render()
{
    GameObserver * game = card->getObserver();
    //#W54-J (A5): one resource-manager pointer for the whole render; every
    //managed HUD quad below comes from hudQuad() (see the audit-J block).
    WResourceManager * rm = game ? game->getResourceManager() : WResourceManager::Instance();
    WFont * mFont = rm->GetWFont(Fonts::MAIN_FONT);
    JRenderer * renderer = JRenderer::GetInstance();
    TargetChooser * tc = NULL;

    if (game)
        tc = game->getCurrentTargetChooser();

    bool alternate = true;
    JQuadPtr quad = game? game->getResourceManager()->RetrieveCard(card, CACHE_THUMB):WResourceManager::Instance()->RetrieveCard(card, CACHE_THUMB);
    if(card && !card->isToken && card->name != card->model->data->name)
    {
        //W53-V (owner's Vita: a transformed Heliod showed the OTHER printing's
        //front art): the instance's OWN id is authoritative when it already
        //names this face - a flip rewrote it (AAFlip/AATurnSide setMTGId) and it
        //carries the right PRINTING. Re-resolving by NAME threw the printing
        //away and took whichever same-named printing sorts lowest. Keep the
        //name path only for copy/clone effects, where the id genuinely still
        //points at the original card.
        MTGCard * self = MTGCollection()->getCardById(card->getMTGId());
        if (!(self && self->data && self->data->name == card->name))
        {
            MTGCard * fcard = MTGCollection()->getCardByName(card->name);
            quad = game? game->getResourceManager()->RetrieveCard(fcard, CACHE_THUMB)
                       : WResourceManager::Instance()->RetrieveCard(fcard, CACHE_THUMB);
        }
    }
    if (game && card->hasCopiedToken && !quad.get())
    {
        MTGCard * tcard = MTGCollection()->getCardById(abs(card->copiedID));
        quad = game->getResourceManager()->RetrieveCardToken(tcard, CACHE_THUMB, 1, abs(card->copiedID));
    }
    if (quad.get())
        alternate = false;
    else
        quad = AlternateThumbQuad(card);

    float cardScale = quad ? 38 / quad->mHeight : 1;
    //I want the below for melded cards but I dont know how to adjust everything else
    //to look neat and clean. leaving this here incase someone else wants to pretty up the p/t box
    //and line up the position.
   /* if (card->MeldedFrom.size())
        cardScale = cardScale + (10 / quad->mHeight);*/
    float scale = actZ * cardScale;

    JQuadPtr shadow;
    if (actZ > 1)
    {
        shadow = hudQuad(rm, HUD_SHADOW);
        if (shadow) 
        {
            shadow->SetColor(ARGB(static_cast<unsigned char>(actA)/2,255,255,255));
            renderer->RenderQuad(shadow.get(), actX + (actZ - 1) * 15, actY + (actZ - 1) * 15, actT, 28 * actZ / 16, 40 * actZ / 16);
        }
    }

    JQuadPtr extracostshadow;
    if (card->isExtraCostTarget)
    {
        extracostshadow = hudQuad(rm, HUD_EXTRACOSTSHADOW);
        if (extracostshadow) 
        {
            extracostshadow->SetColor(ARGB(static_cast<unsigned char>(actA)/2,100,0,0));
            renderer->RenderQuad(extracostshadow.get(), actX + (actZ - 1) * 15, actY + (actZ - 1) * 15, actT, 28 * actZ / 16, 40 * actZ / 16);
        }
    }

    if(game && game->connectRule)
    {
        // Am I a parent of a selected card, or am I a parent and myself being selected?
        bool isActiveConnectedParent = mHasFocus && card->childrenCards.size();
        if (!isActiveConnectedParent)
        {
            for (size_t i = 0; i < card->childrenCards.size(); ++i)
            {
                MTGCardInstance * child = card->childrenCards[i];
                if (CardView* cv = dynamic_cast<CardView*>(child->view))
                {
                    if (cv->mHasFocus)
                    {
                        isActiveConnectedParent = true;
                        break;
                    }
                }
            }
        }
        if (isActiveConnectedParent)
        {
            JQuadPtr white = hudQuad(rm, HUD_WHITE);
            if(white)
            {
                white->SetColor(ARGB(255,230,50,50));
                renderer->RenderQuad(white.get(), actX, actY, actT, 30 * actZ / 16, 42 * actZ / 16);
                white->SetColor(ARGB(255,255,255,255));//shared quad - restore
            }
        }

        // Am I a child of a selected card, or am I a child and myself being selected?
        bool isActiveConnectedChild = mHasFocus && card->parentCards.size();
        if (!isActiveConnectedChild)
        {
            for (size_t i = 0; i < card->parentCards.size(); ++i)
            {
                MTGCardInstance * parent = card->parentCards[i];
                if (CardView* cv = dynamic_cast<CardView*>(parent->view))
                {
                    if (cv->mHasFocus)
                    {
                        isActiveConnectedChild = true;
                        break;
                    }
                }
            }
        }
        if (isActiveConnectedChild)
        {
            JQuadPtr white = hudQuad(rm, HUD_WHITE);
            if(white)
            {
                white->SetColor(ARGB(255,0,0,255));
                renderer->RenderQuad(white.get(), actX, actY, actT, 30 * actZ / 16, 42 * actZ / 16);
                white->SetColor(ARGB(255,255,255,255));//shared quad - restore
            }
        }
    }
    if (quad)
    {
        //castability dim: a hand card its owner cannot play right now
        //renders faded (castableNow is refreshed by GuiHandSelf::Update
        //and only ever set on the owner's hand cards)
        float renderA = actA;
        if (game && card->castableNow < 0 && card->controller()
            && card->controller()->game->hand->hasCard(card))
            renderA = actA * 0.45f;
        quad->SetColor(ARGB(static_cast<unsigned char>(renderA),255,255,255));
        //fake border...
        JQuadPtr fakeborder = hudQuad(rm, HUD_WHITE);
        JQuadPtr highlightborder = fakeborder; //the same managed "white" quad
        if(fakeborder)
        {
            if(game)
            {
                if(card->has(Constants::PAYZERO) ||
                    ((card->has(Constants::CANPLAYFROMGRAVEYARD) || card->has(Constants::TEMPFLASHBACK) || card->getManaCost()->getFlashback() || card->getManaCost()->getRetrace()) && game->isInGrave(card)) ||
                    (((card->has(Constants::FORETELL) && card->foretellTurn > -1 && game->turn > card->foretellTurn) || card->has(Constants::CANPLAYFROMEXILE)) && game->isInExile(card)))
                    fakeborder->SetColor(ARGB((int)(actA),7,235,7)); //green border
                else if(card->isCommander)
                    fakeborder->SetColor(ARGB((int)(actA),255,255,255)); //white border for commanders
                else
                    fakeborder->SetColor(ARGB((int)(actA),15,15,15));
            }
            else
                fakeborder->SetColor(ARGB((int)(actA),15,15,15));

            renderer->RenderQuad(fakeborder.get(), actX, actY, actT, (31 * actZ + 1) / 16, 42 * actZ / 16);
        }
        //draw border for highlighting
        if (game)
        {
            if (card && card->forcedBorderA && highlightborder)
            {
                highlightborder->SetColor(ARGB(95,255,0,0));
                renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (32 * actZ + 1) / 16, 43 * actZ / 16);
            }
            if (card && card->forcedBorderB && highlightborder)
            {
                highlightborder->SetColor(ARGB(95,0,245,0));
                renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (32 * actZ + 1) / 16, 43 * actZ / 16);
            }
            //castability glow: GOLD border behind a hand card its owner
            //could legally play right now (drawn here so it shows with
            //card art AND when another layer re-renders the card)
            if (card && card->castableNow > 0 && highlightborder && card->controller()
                && card->controller()->game->hand->hasCard(card))
            {
                highlightborder->SetColor(ARGB(220,250,205,60));
                renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (32 * actZ + 1) / 16, 43 * actZ / 16);
            }
            //availability on the battlefield, same language as the gold hand
            //glow: a border means "you can act with this right now".
            //ORANGE = can be declared an attacker, GREEN = has an activated
            //ability you can use and afford.
            //NESTED, and the nesting MUST GO OUTWARD. These quads are drawn
            //BEHIND the card art, so the only part a player ever sees is the
            //rim that protrudes past it: the art is 38 units tall and every
            //visible border here is 43, i.e. a ~2.5-unit rim per side. An
            //INSET ring is not a subtler signal, it is an invisible one - a
            //first attempt at 39 left half a unit of rim and never appeared on
            //hardware at all, which read as "the predicate is broken" when the
            //predicate was fine.
            //
            //So: ability keeps the standard 43 rim, and attack takes a wider
            //46 halo OUTSIDE it. A card with both shows orange around green;
            //a card with one looks like every other signal in the game.
            if (card && highlightborder && card->controller()
                && card->controller()->game->inPlay->hasCard(card))
            {
                if (card->canAttackNow)
                {
                    highlightborder->SetColor(ARGB(220,255,120,60));
                    renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (35 * actZ + 1) / 16, 46 * actZ / 16);
                }
                //Blocking gets the SAME orange halo as attacking: both mean
                //"this creature can enter combat right now", and the two can
                //never appear at the same time (attack ring = your combat,
                //block ring = the opponent's), so one colour carries one idea.
                if (card->canBlockNow)
                {
                    highlightborder->SetColor(ARGB(220,255,120,60));
                    renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (35 * actZ + 1) / 16, 46 * actZ / 16);
                }
                if (card->hasUsableAbilityNow)
                {
                    //42, not the standard 43: a 2-unit rim reads clearly on
                    //hardware (the invisibility floor was ~39) while keeping
                    //the green visually inside the 46 attack halo.
                    highlightborder->SetColor(ARGB(210,80,235,170));
                    renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (31 * actZ + 1) / 16, 42 * actZ / 16);
                }
            }
            //tap preview: BLUE border on the battlefield producers the
            //auto-tap plan would activate for the focused hand card. Drawn
            //AFTER the availability borders so the answer to "what does this
            //cast cost me" wins over the standing state underneath it.
            if (card && card->willPayForFocused && highlightborder && card->controller()
                && card->controller()->game->inPlay->hasCard(card))
            {
                highlightborder->SetColor(ARGB(200,80,170,255));
                renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (32 * actZ + 1) / 16, 43 * actZ / 16);
            }
            if(card->myPair && card->myPair->isInPlay(game) && highlightborder)
            {
                if(mHasFocus)
                {
                    highlightborder->SetColor(ARGB(200,7,98,248));
                    renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (32 * actZ + 1) / 16, 43 * actZ / 16);
                }
                if(CardView* cv = dynamic_cast<CardView*>(card->myPair->view))
                {
                    if(cv->mHasFocus)
                    {
                        highlightborder->SetColor(ARGB(200,57,28,248));
                        renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (32 * actZ + 1) / 16, 43 * actZ / 16);
                    }
                }
            }
            if(card->shackled && card->shackled->isInPlay(game) && highlightborder)
            {
                if(mHasFocus)
                {
                    if(card->has(Constants::SHACKLER))
                        highlightborder->SetColor(ARGB(200,7,98,248));
                    else
                        highlightborder->SetColor(ARGB(200,57,28,248));

                    renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (32 * actZ + 1) / 16, 43 * actZ / 16);
                }
                if(CardView* cv = dynamic_cast<CardView*>(card->shackled->view))
                {
                    if(cv->mHasFocus)
                    {
                        if(!card->shackled->has(Constants::SHACKLER))
                            highlightborder->SetColor(ARGB(200,7,98,248));
                        else
                            highlightborder->SetColor(ARGB(200,57,28,248));

                        renderer->RenderQuad(highlightborder.get(), actX, actY, actT, (32 * actZ + 1) / 16, 43 * actZ / 16);
                    }
                }
            }
        }
        //draw the card image
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
        //WAGIC_ALTLOG (Android: flag file User/altlog.on, output next to it):
        //log every art-less battlefield render with the id/set/image the
        //load wanted - turns "some cards have no art" into an exact list.
        {
            static FILE * altOut = NULL;
            static int altState = 0;
            if (alternate && altState >= 0)
            {
                if (altState == 0)
                {
                    if (getenv("WAGIC_ALTLOG"))
                        { altOut = stderr; altState = 1; }
#ifdef ANDROID
                    else if (access("/sdcard/Wagic/User/altlog.on", F_OK) == 0)
                        { altOut = fopen("/sdcard/Wagic/User/altlog.txt", "a"); altState = altOut ? 1 : -1; }
#endif
                    else altState = -1;
                }
                if (altState == 1)
                {
                    fprintf(altOut, "altlog id=%d set=%s img=%s card=%s x=%.1f y=%.1f z=%.3f a=%.0f\n",
                            card->getMTGId(),
                            (card->setId >= 0) ? setlist[card->setId].c_str() : "?",
                            card->getImageName().c_str(), card->name.c_str(), actX, actY, actZ, actA);
                    fflush(altOut);
                }
            }
        }
#endif //_DEBUG || WAGIC_DEVLOGS

        renderer->RenderQuad(quad.get(), actX, actY, actT, scale, scale);
    }

    if (alternate)
    {
        mFont->SetColor(ARGB(static_cast<unsigned char>(actA), 0, 0, 0));
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE * 0.5f * actZ);
        mFont->DrawString(_(card->getName()), actX - actZ * Width / 2 + 1, actY - actZ * Height / 2 + 1);
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);

        JQuadPtr icon;
        const RenderTypeIds& tid = renderTypeIds();
        if (card->hasSubtype(tid.plains))
            icon = hudQuad(rm, HUD_C_WHITE);
        else if (card->hasSubtype(tid.swamp))
            icon = hudQuad(rm, HUD_C_BLACK);
        else if (card->hasSubtype(tid.forest))
            icon = hudQuad(rm, HUD_C_GREEN);
        else if (card->hasSubtype(tid.mountain))
            icon = hudQuad(rm, HUD_C_RED);
        else if (card->hasSubtype(tid.island))
            icon = hudQuad(rm, HUD_C_BLUE);
        if (icon.get())
        {
            icon->SetColor(ARGB(static_cast<unsigned char>(actA),255,255,255));
            renderer->RenderQuad(icon.get(), actX, actY, actT);
            icon->SetColor(ARGB(255,255,255,255)); //Putting color back as this quad is shared
        }

    }
    JQuadPtr mor;
    if((card->isMorphed||(card->name == "Morph" && card->isACopier)) && !alternate)
    {
        mor = card->getObserver()->getResourceManager()->RetrieveTempQuad("morph.jpg");
        if (mor &&  mor->mTex) {
            mor->SetHotSpot(static_cast<float> (mor->mTex->mWidth / 2), static_cast<float> (mor->mTex->mHeight / 2));
            mor->SetColor(ARGB(255,255,255,255));
            renderer->RenderQuad(mor.get(), actX, actY, actT,scale, scale);
        }
    }

    //draw line
    if (game)
    {
        //choose attacker mask. #W54-J (A5): the same predicates, cheapest
        //first - Player::hasPossibleAttackers() is a whole-battlefield scan
        //(canAttack x2 per permanent) and ran for EVERY card EVERY frame; it
        //now runs only for a creature of the acting human inside its combat
        //window, after every per-card test has already passed. All of these
        //are pure reads, so the reorder cannot change what is drawn.
        {
            const int phase = game->getCurrentGamePhase();
            if(phase > MTG_PHASE_FIRSTMAIN && phase < MTG_PHASE_SECONDMAIN
                && card->controller() == game->currentPlayer
                && card->controller()->isHuman()
                && card->isInPlay(game) && card->isCreature()
                && !card->isPhased && !card->didattacked
                && ((!card->canAttack() || (card->attackCost > 0)) && (!card->canAttack(true) || (card->attackPlaneswalkerCost > 0)))
                && game->currentPlayer->hasPossibleAttackers())
            {
                JQuadPtr ssMask = hudQuad(rm, HUD_WHITE);
                if(ssMask)
                {
                    ssMask->SetColor(ARGB(170,64,64,64));
                    renderer->RenderQuad(ssMask.get(), actX, actY, actT, (27 * actZ + 1) / 16, 40 * actZ / 16);
                }
            }
        }

        if (card && card->forcedBorderA)
        {
            if(card->isTapped())
            {
                if(mHasFocus)
                    renderer->DrawRoundRect(actX - (scale * quad->mWidth / 2)-10,actY - (scale * quad->mHeight / 2)+6.5f, (scale * quad->mHeight)-0.02f, (scale * quad->mWidth)-0.02f, 1.8f,ARGB(250,255,0,0));
                else
                    renderer->DrawRoundRect(actX - (scale * quad->mWidth / 2)-8,actY - (scale * quad->mHeight / 2)+4, (scale * quad->mHeight)-0.02f, (scale * quad->mWidth)-0.02f, 1.8f,ARGB(250,255,0,0));
            }
            else
                renderer->DrawRoundRect(actX - (scale * quad->mWidth / 2)-2,actY - (scale * quad->mHeight / 2)-2, (scale * quad->mWidth)-0.02f, (scale * quad->mHeight)-0.02f, 1.8f,ARGB(250,255,0,0));
        }
        if (card && card->forcedBorderB)
        {
            if(card->isTapped())
            {
                if(mHasFocus)
                    renderer->DrawRoundRect(actX - (scale * quad->mWidth / 2)-10,actY - (scale * quad->mHeight / 2)+6.5f, (scale * quad->mHeight)-0.02f, (scale * quad->mWidth)-0.02f, 1.8f,ARGB(250,0,255,0));
                else
                    renderer->DrawRoundRect(actX - (scale * quad->mWidth / 2)-8,actY - (scale * quad->mHeight / 2)+4, (scale * quad->mHeight)-0.02f, (scale * quad->mWidth)-0.02f, 1.8f,ARGB(250,0,255,0));
            }
            else
                renderer->DrawRoundRect(actX - (scale * quad->mWidth / 2)-2,actY - (scale * quad->mHeight / 2)-2, (scale * quad->mWidth)-0.02f, (scale * quad->mHeight)-0.02f, 1.8f,ARGB(250,0,255,0));
       }
    }

    // Render a mask over the card, if set
    if (mask && quad)
        JRenderer::GetInstance()->FillRect(actX - (scale * quad->mWidth / 2),actY - (scale * quad->mHeight / 2), scale * quad->mWidth, scale* quad->mHeight, mask);

    if ((tc && tc->alreadyHasTarget(card)) || (game && card == game->mLayers->actionLayer()->currentActionCard))//paint targets red.
    {
        JQuadPtr rMask = hudQuad(rm, HUD_WHITE);
        rMask->SetColor(ARGB(128,255,0,0));//red
        renderer->RenderQuad(rMask.get(), actX, actY, actT, (27 * actZ + 1) / 16, 40 * actZ / 16);
        rMask->SetColor(ARGB(255,255,255,255));//"white" is ONE shared cached JQuad (GameApp.cpp registers it
        //from shadows.png). SetColor mutates that shared object permanently, so every later user of
        //GetQuad("white") - in this screen or any other - inherited this tint until something else
        //overwrote it. Restore it.
    }
    if(tc && tc->source && tc->source->view && tc->source->view->actZ >= 1.3 && card == tc->source)//paint the source green while infocus.
    {
        JQuadPtr gMask = hudQuad(rm, HUD_WHITE);
        gMask->SetColor(ARGB(128,0,255,0));//green
        renderer->RenderQuad(gMask.get(), actX, actY, actT, (27 * actZ + 1) / 16, 40 * actZ / 16);
        gMask->SetColor(ARGB(255,255,255,255));//see the red mask above: shared quad, restore the tint.
    }

    //draws the numbers power/toughness
    if (card->isCreature())
    {
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
        char buffer[200];
        sprintf(buffer, "%i/%i", card->power, card->life);
         //move up the p/t box by increasing ymody
         float ymody = ((card->isAttacker()||card->isDefenser())&&card->isTapped())?-6.0f:0.0f;
        renderer->FillRect(actX - (13 * actZ), actY + ymody + 4 * actZ, 25.5f * actZ, 14 * actZ,
            ARGB(((static_cast<unsigned char>(actA))/2),0,0,0));
        renderer->DrawRect(actX - (13 * actZ), actY + ymody + 4 * actZ, 25.5f * actZ, 14 * actZ,
            ARGB(((static_cast<unsigned char>(actA))),20,20,20));
        //damaged or buffed or powered down        
        if(card->wasDealtDamage > 0 && card->life <= 2)
            mFont->SetColor(ARGB(static_cast<unsigned char>(actA),255,0,0));//red critical and damaged
        else if(card->wasDealtDamage == 0 && card->pbonus < 0)
            mFont->SetColor(ARGB(static_cast<unsigned char>(actA),216,191,216));//thistle powered down
        else if(card->wasDealtDamage == 0 && card->pbonus >= 3)
            mFont->SetColor(ARGB(static_cast<unsigned char>(actA),255,255,0));//yellow buff
        else if(card->hasType(Subtypes::TYPE_LEGENDARY) && card->hasType(renderTypeIds().eldrazi) && !card->has(Constants::CHANGELING)) //#W54-J (A5): ids, not strings
            mFont->SetColor(ARGB(static_cast<unsigned char>(actA),238,130,238));//violet legendary eldrazi
        else
            mFont->SetColor(ARGB(static_cast<unsigned char>(actA),255,255,255));//white default
        mFont->SetScale(actZ);
        mFont->SetScale(actZ);
        float halfbufferW = (mFont->GetStringWidth(buffer))/2;
        mFont->DrawString(buffer, actX - halfbufferW, actY + ymody + 7 * actZ);
        mFont->SetScale(1);
    }

    //#W54-J (A5): the token/copy/exerted tag, built without std::string
    //temporaries (three per card per frame for a usually-empty mark).
    char buff[16];
    {
        const char* tag = card->isToken ? (card->isACopier ? "CT" : "T") : (card->isACopier ? "C" : "");
        snprintf(buff, sizeof(buff), "%s%s", card->exerted ? "exerted" : "", tag);
    }
    //if(card->has(Constants::PAYZERO))
        //buff += "Z";
    if(card->chooseacolor >= 1)
    {
        if(card->chooseacolor == 1)
        {
            renderer->FillRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(255,0,255,0));
            renderer->DrawRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(180,10,10,10));
        }
        else if(card->chooseacolor == 2)
        {
            renderer->FillRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(255,0,206,209));
            renderer->DrawRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(180,10,10,10));
        }
        else if(card->chooseacolor == 3)
        {
            renderer->FillRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(255,255,0,0));
            renderer->DrawRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(180,10,10,10));
        }
        else if(card->chooseacolor == 4)
        {
            renderer->FillRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(255,20,20,20));
            renderer->DrawRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(180,210,210,210));
        }
        else if(card->chooseacolor == 5)
        {
            renderer->FillRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(255,250,235,215));
            renderer->DrawRect(actX - 10 * actZ, actY - (1 * actZ), 6.f,6.f,ARGB(180,10,10,10));
        }
    }
    if(card->chooseasubtype.size() && !alternate && game)
    {
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
        char buffer[200];
        sprintf(buffer, "%s", card->chooseasubtype.c_str());
        mFont->SetColor(ARGB(static_cast<unsigned char>(actA),255,215,0));//Gold indicator
        mFont->SetScale(0.8f);
        mFont->DrawString(buffer, actX - 10 * actZ, actY - (25.3f * actZ));
        mFont->SetScale(1);
    }
    if(card->chooseaname.size() && !alternate && game)
    {
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
        char buffer[200];
        sprintf(buffer, "%s", card->chooseaname.c_str());
        mFont->SetColor(ARGB(static_cast<unsigned char>(actA),255,215,0));//Gold indicator
        mFont->SetScale(0.8f);
        mFont->DrawString(buffer, actX - 10 * actZ, actY - (25.3f * actZ));
        mFont->SetScale(1);
    }
    if(!alternate && buff[0] && game)
    {
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
        mFont->SetColor(ARGB(static_cast<unsigned char>(actA),255,215,0));//Gold indicator
        mFont->SetScale(actZ);
        mFont->DrawString(buff, actX - 10 * actZ, actY - (18.3f * actZ));
        mFont->SetScale(1);
    }
#if !defined (PSP)
    if(game && game->gameType() == GAME_TYPE_MOMIR)
    {
        if(game->isInHand(card) && !card->controller()->isAI())
        {
            if ((game->currentPlayer != card->controller()) || (card->controller()->game->playRestrictions->canPutIntoZone(card, card->controller()->game->inPlay) == PlayRestriction::CANT_PLAY))
            {
                mFont->SetScale(0.4f);
                mFont->SetColor(ARGB(static_cast<unsigned char>(actA),255,0,0));
                mFont->SetScale(actZ);
                mFont->DrawString("+", actX - 12 * actZ, actY - (18.8f * actZ));
                mFont->SetScale(1);
            }
            else
            {
                mFont->SetScale(0.4f);
                mFont->SetColor(ARGB(static_cast<unsigned char>(actA),0,255,0));
                mFont->SetScale(actZ);
                mFont->DrawString("+", actX - 12 * actZ, actY - (18.8f * actZ));
                mFont->SetScale(1);
            }
        }
    }
#endif
    if (card->counters->mCount > 0)
    {
        unsigned c = -1;
        for (int i = 0; i < card->counters->mCount; i++)
        {
            if (card->counters->counters[i]->name != "")
                c = i;
            break;
        }
        if (c + 1)
        {
            mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
            char buffer[200];
            sprintf(buffer, "%i", card->counters->counters[0]->nb);
            mFont->SetColor(ARGB(static_cast<unsigned char>(actA),255,255,255));
            mFont->SetScale(actZ);
            mFont->DrawString(buffer, actX - 10 * actZ, actY - (10.8f * actZ));
            mFont->SetScale(1);
        }
    }

    //shadow that covers the whole card for targetchooser...
    if (tc && !tc->canTarget(card))
    {
        if (!shadow)
            shadow = hudQuad(rm, HUD_SHADOW);
        if (shadow)
        {
            shadow->SetColor(ARGB(190,255,255,255));
            renderer->RenderQuad(shadow.get(), actX, actY, actT, (28 * actZ + 1) / 16, 40 * actZ / 16);
        }
    }
    
    //for necro
    if(game)
    {
        if (!shadow)
            shadow = hudQuad(rm, HUD_SHADOW);
        if (shadow)
        {
            int myA = 0;
            if(game && card->has(Constants::NECROED))//no peeking...
                myA = 255;
            else
                myA = 0;
            
            shadow->SetColor(ARGB(myA,255,255,255));
            if(myA > 0)
                renderer->RenderQuad(shadow.get(), actX, actY, actT, (28 * actZ + 1) / 16, 40 * actZ / 16);
        }
    }

    //#W57-G (D42): the pile badge. Drawn last so nothing overpaints it, and
    //only on the one member that stands in for a group - a lone card never
    //shows it, so the badge itself means "there are more of these here".
    if (mStackCount > 1)
    {
        char sbuf[32];
        sprintf(sbuf, "x%i", mStackCount);
        //dark plate first: at 480x272 white glyphs over card art are unreadable
        //on half the printings.
        //In the GUTTER off the card's top-right corner, not on the face. A
        //28x40 card at 480x272 has no free corner: the top-left is the printed
        //name, the bottom the P/T box - a badge over either HIDES information
        //(the first placement clipped the toughness digit, which is worse than
        //no badge at all). Slots are 31 apart, so this lands in the ~3px gap
        //plus the neighbouring pile's left rim, next to the offset plates that
        //already read as "there is more than one here".
        JRenderer::GetInstance()->FillRect(actX + 12.0f * actZ, actY - 21.0f * actZ,
                                           13.0f * actZ, 9.0f * actZ, ARGB(225, 10, 10, 10));
        mFont->SetColor(ARGB(255, 255, 235, 140));
        mFont->SetScale(actZ);
        mFont->DrawString(sbuf, actX + 13.0f * actZ, actY - (20.6f * actZ));
        mFont->SetScale(1);
        mFont->SetColor(ARGB(255, 255, 255, 255));
    }

    PlayGuiObject::Render();
}

JQuadPtr CardGui::AlternateThumbQuad(MTGCard * card)
{
    // Called once per visible art-less card per frame (and for every card when DISABLECARDS is
    // set), so the per-call cost has to stay near zero on PSP/Vita.
    const vector<ModRulesBackGroundCardGuiItem *>& items = gModRules.cardgui.background;
    int numItems = (int)items.size();
    if (!numItems) return JQuadPtr();

    // The rendered output is determined solely by the colour bucket, and a permanent's colours
    // are mutable during a duel (CardPrimitive::setColor/removeColor), so the bucket is
    // recomputed every call and is the cache key. Keying by card identity would show stale art
    // after a colour-changing effect.
    int index = (card->data->countColors() > 1) ? numItems - 1 : card->data->getColor();
    if (index < 0 || index >= numItems) index = 0;

#if defined(PSP)
    // NO cache and NO pinning here. RETRIEVE_MANAGE holds a texture in the resource manager's
    // managed map, where ClearUnlocked/RemoveOldest can never reclaim it - and on PSP
    // allocation pressure is the mechanism behind very nearly every behaviour that differs
    // from the other platforms. An unevictable texture is the wrong trade on the one target
    // that has no headroom to spend, however small it looks: what gets pinned is the whole
    // decoded texture, not the 28x40 region the quad draws from. The temp quad is what this
    // function used before the cache, it stays evictable, and the lookup it costs is cheaper
    // than the memory it saves.
    JQuadPtr q = WResourceManager::Instance()->RetrieveTempQuad(items[index]->mDisplayThumb);
#else
    // One entry per background item (8 in modrules.xml), filled on demand: bounded by the mod
    // rules, not by the number of cards seen, so it cannot grow over a long session.
    static vector<JQuadPtr> sAlternateThumbs;
    if ((int)sAlternateThumbs.size() != numItems)
        sAlternateThumbs.assign(numItems, JQuadPtr());

    JQuadPtr q = sAlternateThumbs[index];
    if (!q.get())
    {
        // RETRIEVE_MANAGE deadbolts the texture the way GameApp pins back_thumb.jpg and the mana
        // icons: the entry is held in the resource manager's managed map rather than the
        // evictable cache, so ClearUnlocked/RemoveOldest cannot free it under this quad, and
        // WCachedTexture::Refresh (theme or profile change) repoints the quad's texture in
        // place.
        const string& thumbFile = items[index]->mDisplayThumb;
        q = WResourceManager::Instance()->RetrieveQuad(thumbFile, 0, 0, 0, 0, "altthumb_" + thumbFile, RETRIEVE_MANAGE);
        if (!q.get()) return q;
        sAlternateThumbs[index] = q;
    }
#endif

    if (q && q->mTex)
        q->SetHotSpot(static_cast<float> (q->mTex->mWidth / 2), static_cast<float> (q->mTex->mHeight / 2));
    return q;
}

void CardGui::AlternateRender(MTGCard * card, const Pos& pos)
{
#if defined(WAGIC_AUTODEMO) || defined(WAGIC_HWPROBE)
    static MTGCard * lastAlt = NULL;
    if (card != lastAlt)
    {
        lastAlt = card;
        cardProbe("altrender: %s", (card && card->data) ? card->data->getName().c_str() : "?");
    }
#endif
    // Draw the "unknown" card model
    JRenderer * renderer = JRenderer::GetInstance();
    JQuadPtr q;
    MTGCardInstance * thiscard = dynamic_cast<MTGCardInstance*> (card);
    int zpos = 0;
    float x = pos.actX;
   
    const vector<ModRulesBackGroundCardGuiItem *>& items = gModRules.cardgui.background; //#W54-J (A43): in place, per frame
    ModRulesBackGroundCardGuiItem * item;
    int numItems = (int)items.size();
    if (card->data->countColors() > 1)
    {
         item = items[numItems-1];
    }
    else
    {
        item = items[card->data->getColor()];
    }
    
    q = WResourceManager::Instance()->RetrieveTempQuad(item->mDisplayImg,TEXTURE_SUB_5551);

    if (q.get() && q->mTex)
    {
        if(thiscard && thiscard->getObserver())
        {
            zpos = thiscard->zpos;
        }

        q->SetHotSpot(static_cast<float> (q->mTex->mWidth / 2), static_cast<float> (q->mTex->mHeight / 2));

        float scale = pos.actZ * 250 / q->mHeight;
        q->SetColor(ARGB((int)pos.actA,255,255,255));
        //new border
        renderer->FillRoundRect(pos.actX - (scale * q->mWidth / 2)-5.8f,pos.actY - (scale * q->mHeight / 2)-5.8f, (scale * q->mWidth)-0.02f, (scale * q->mHeight)-0.02f, 5.8f,ARGB(255,5,5,5));
        renderer->DrawRoundRect(pos.actX - (scale * q->mWidth / 2)-5.8f,pos.actY - (scale * q->mHeight / 2)-5.8f, (scale * q->mWidth)-0.02f, (scale * q->mHeight)-0.02f, 5.8f,ARGB(50,240,240,240));
        //end
        renderer->RenderQuad(q.get(), x, pos.actY, pos.actT, scale, scale);
    }

    const vector<ModRulesRenderCardGuiItem *>& Carditems = gModRules.cardgui.renderbig; //#W54-J (A43)
    
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::MAGIC_FONT);
    float backup_scale = font->GetScale();
    font->SetColor(ARGB((int)pos.actA, 0, 0, 0));
    string sFormattedData = "";
    
    for( size_t i =0 ; i < Carditems.size(); i ++)
    {
        ModRulesRenderCardGuiItem * Carditem = Carditems[i];
        if (Carditem->mFilter.length() == 0 || FilterCard(card,Carditem->mFilter.c_str()))
        {

           if (Carditem->mFont) 
            {
                font->SetColor(Carditem->mFontColor);
                font->SetScale(((float)Carditem->mFontSize / 100) * pos.actZ);
                
            }
            else
            {
                font->SetColor(ARGB((int)pos.actA, 0, 0, 0));
                font->SetScale(kWidthScaleFactor * pos.actZ);

            }
            
            if (Carditem->mName == "description")
            {

                const std::vector<string>& txt = card->data->getFormattedText(); //#W54-J (A43)

                unsigned i = 0;
                unsigned h = neofont ? 14 : 11;
                for (std::vector<string>::const_iterator it = txt.begin(); it != txt.end(); ++it, ++i)
                    font->DrawString(_(it->c_str()), x + (Carditem->mPosX - BigWidth / 2) * pos.actZ, pos.actY + (-BigHeight / 2 + Carditem->mPosY + h * i) * pos.actZ);
            }
            else if (Carditem->mName == "mana")
            {
                // Mana
                // Need Create a realy generic struct for mana render
                ManaCost* manacost = card->data->getManaCost();
                ManaCostHybrid* h;
                ExtraCost* e;
                unsigned int j = 0;
                unsigned int z = 0;
                unsigned char t = (JGE::GetInstance()->GetTime() / 3) & 0xFF;
                unsigned char v = t + 127;
                float yOffset = (float)Carditem->mPosY;
                
                JQuadPtr quad = WResourceManager::Instance()->RetrieveQuad("menuicons.png", 0, 0, 0, 0, "", RETRIEVE_NORMAL, TEXTURE_SUB_5551, 1);
                if (quad.get())
                    if (quad->mHeight >= 78)
                    while ((e = manacost->getExtraCost(z)))
                    {
                            if(e->mCostRenderString == "Phyrexian Mana")
                            {
                                float _color = (float)card->data->getColor() -1;
                                JQuadPtr ExtraManas = WResourceManager::Instance()->RetrieveQuad("menuicons.png", 2 + _color * 36, 76, 32, 32, "c_extra", RETRIEVE_MANAGE);
                                ExtraManas->SetHotSpot(16, 16);
                                renderer->RenderQuad(ExtraManas.get(), x + (-12 * j + Carditem->mPosX) * pos.actZ, pos.actY + (yOffset) * pos.actZ, 0, 0.4f
                                * pos.actZ, 0.4f * pos.actZ);
                            }
                   
                        ++j;
                        ++z;
                    }
               
                z=0;
                while ((h = manacost->getHybridCost(z)))
                {
                    float scale = pos.actZ * 0.05f * cosf(2 * M_PI * ((float) t) / 256.0f);

                    if (scale < 0)
                    {
                        renderer->RenderQuad(manaIcons[h->color1].get(), x + (-12 * j + Carditem->mPosX + 3 * SineHelperFunction((float) t)) * pos.actZ,
                            pos.actY + (yOffset + 3 * CosineHelperFunction((float) t)) * pos.actZ, 0, 0.4f + scale, 0.4f
                            + scale);
                        renderer->RenderQuad(manaIcons[h->color2].get(), x + (-12 * j + Carditem->mPosX + 3 * SineHelperFunction((float) v)) * pos.actZ,
                            pos.actY + (yOffset + 3 * CosineHelperFunction((float) v)) * pos.actZ, 0, 0.4f - scale, 0.4f
                            - scale);
                    }
                    else
                    {
                        renderer->RenderQuad(manaIcons[h->color2].get(), x + (-12 * j + Carditem->mPosX + 3 * SineHelperFunction((float) v)) * pos.actZ,
                            pos.actY + (yOffset + 3 * CosineHelperFunction((float) v)) * pos.actZ, 0, 0.4f - scale, 0.4f
                            - scale);
                        renderer->RenderQuad(manaIcons[h->color1].get(), x + (-12 * j + Carditem->mPosX + 3 * SineHelperFunction((float) t)) * pos.actZ,
                            pos.actY + (yOffset + 3 * CosineHelperFunction((float) t)) * pos.actZ, 0, 0.4f + scale, 0.4f
                            + scale);
                    }
                    ++j;
                    ++z;
                }
                for (int i = Constants::NB_Colors - 2; i >= 1; --i)
                {
                     int cost;
                    for (cost = manacost->getCost(i); cost > 0; --cost)
                    {
                        renderer->RenderQuad(manaIcons[i].get(), x + (-12 * j + Carditem->mPosX) * pos.actZ, pos.actY + (yOffset) * pos.actZ, 0, 0.4f
                            * pos.actZ, 0.4f * pos.actZ);
                        ++j;
                    }
                    
                }
                // Colorless mana
                if (int cost = manacost->getCost(0))
                {
                    char buffer[10];
                    sprintf(buffer, "%d", cost);
                    renderer->RenderQuad(manaIcons[0].get(), x + (-12 * j + Carditem->mPosX) * pos.actZ, pos.actY + (yOffset) * pos.actZ, 0, 0.4f * pos.actZ,
                        0.4f * pos.actZ);
                    float w = font->GetStringWidth(buffer);
                    font->DrawString(buffer, x + (-12 * j + (Carditem->mPosX +1) - w / 2) * pos.actZ, pos.actY + (yOffset - 5) * pos.actZ);
                    ++j;
                }
                //Has X?
                if (manacost->hasX())
                {
                    char buffer[10];
                    sprintf(buffer, "X");
                    renderer->RenderQuad(manaIcons[0].get(), x + (-12 * j + Carditem->mPosX) * pos.actZ, pos.actY + (yOffset) * pos.actZ, 0, 0.4f * pos.actZ,
                        0.4f * pos.actZ);
                    float w = font->GetStringWidth(buffer);
                    font->DrawString(buffer, x + (-12 * j + (Carditem->mPosX + 1) - w / 2) * pos.actZ, pos.actY + (yOffset - 5) * pos.actZ);
                }
    
            }
            else if (Carditem->mName == "icon")
            {
                float yOffseticon = (float)Carditem->mPosY;
                JQuadPtr ExtraIcons = WResourceManager::Instance()->RetrieveQuad(Carditem->mFileName.c_str(), 2 + (float)(Carditem->mIconPosX - 1) * 36, (float)(Carditem->mIconPosY -1) * 38 , 32, 32, "", RETRIEVE_MANAGE);
                ExtraIcons->SetHotSpot(16,16);
                renderer->RenderQuad(ExtraIcons.get(), x + (Carditem->mPosX) * pos.actZ, pos.actY + (yOffseticon) * pos.actZ, 0, (float)Carditem->mSizeIcon * 0.4f
                * pos.actZ, (float)Carditem->mSizeIcon* 0.4f * pos.actZ);

            }
            else 
            {
                string formattedfield = Carditem->mFormattedData;
                size_t found = Carditem->mName.find("title"); // Write the title
                if (found != string::npos)
                {
                    stringstream st;
                    st << _(card->data->name);
                    formattedfield = FormattedData(formattedfield, "title", st.str());
                
                }

                found = Carditem->mName.find("cardid"); // Write the cardid
                if (found != string::npos)
                {
                    stringstream st;
                    st << "id:" << card->getMTGId() << " zpos:" << zpos;
                    formattedfield = FormattedData(formattedfield, "mtgid", st.str());
                }

                found = Carditem->mName.find("power"); // Write the strength
                if (found != string::npos)
                {
                    stringstream st;
                    st << card->data->power;
                    formattedfield = FormattedData(formattedfield, "power", st.str());
                }
                found = Carditem->mName.find("life"); // Write the toughness
                if (found != string::npos)
                {
                    stringstream st;
                    st << card->data->toughness;
                    formattedfield = FormattedData(formattedfield, "life", st.str());

                }

                found = Carditem->mName.find("types"); //types
                if (found != string::npos)
                {
                    string s = "";
                    if (card->data->basicAbilities[(int)Constants::CHANGELING])
                    {
                        // this avoids drawing the list of subtypes on changeling cards.
                        if (card->data->types.size()){
                            s = _(MTGAllCards::findType(card->data->types[0])) + _(" - Shapeshifter");
                        } else {
                            s = _("Shapeshifter");
                        }
                    } else {
                        for (unsigned int i = 0; i < card->data->types.size() - 1; i++)
                        {
                            s += _(MTGAllCards::findType(card->data->types[i]));
                            if(!strcmp(_(MTGAllCards::findType(card->data->types[i])).c_str(),"Creature") || !strcmp(_(MTGAllCards::findType(card->data->types[i])).c_str(),"Land"))
                                s += _(" - ");
                            else
                                s += _(" ");
                        }
                        s += _(MTGAllCards::findType(card->data->types[card->data->types.size()-1]));
                    }
                    if (!card->data->types.size()){
                        DebugTrace("Typeless card: " << setlist[card->setId].c_str() << card->data->getName() << card->getId());
                    }
                    formattedfield = FormattedData(formattedfield, "types", s);
                }

                found = Carditem->mName.find("rarity");
                if (found != string::npos)
                {
                    
                    string sRarity;
                    switch(card->getRarity())
                    {
                    case Constants::RARITY_M:
                        sRarity =_("Mythic");
                        break;
                    case Constants::RARITY_R:
                        sRarity =_("Rare");
                        break;
                    case Constants::RARITY_U:
                        sRarity =_("Uncommon");
                        break;
                    case Constants::RARITY_C:
                        sRarity =_("Common");
                        break;
                    case Constants::RARITY_L:
                        sRarity =_("Land");
                        break;
                    case Constants::RARITY_T:
                        sRarity =_("Token");
                        break;
                    default:
                    case Constants::RARITY_S:
                        sRarity =_("Special");
                        break;
                    }
                    formattedfield = FormattedData(formattedfield, "rarity", sRarity);
                }

                 found = Carditem->mName.find("expansion");
                if (found != string::npos)
                {
                    formattedfield = FormattedData(formattedfield, "expansion", setlist[card->setId].c_str());
                }

                if (!Carditem->mFont) 
                {          
                    float w = font->GetStringWidth(formattedfield.c_str()) * kWidthScaleFactor * pos.actZ;
                    if (w > BigWidth - 30)
                        font->SetScale((BigWidth - 30) / w);
                }
                font->DrawString(formattedfield.c_str(), x + (Carditem->mPosX  - BigWidth / 2) * pos.actZ, pos.actY + (Carditem->mPosY - BigHeight / 2) * pos.actZ);
            
            }
         
        }
    }


    
    font->SetScale(backup_scale);

    RenderCountersBig(card, pos, DrawMode::kText);
}

void CardGui::TinyCropRender(MTGCard * card, const Pos& pos, JQuad * quad)
{
    if (!quad)
        return;

    JRenderer * renderer = JRenderer::GetInstance();
    JQuadPtr q;

    float x = pos.actX;
    float displayScale = 250 / BigHeight;
   
    const vector<ModRulesBackGroundCardGuiItem *>& items = gModRules.cardgui.background; //#W54-J (A43): in place, per frame
    ModRulesBackGroundCardGuiItem * item;
    int numItems = (int)items.size();
    if (card->data->countColors() > 1)
    {
         item = items[numItems-1];
    }
    else
    {
        item = items[card->data->getColor()];
    }
    
    q = WResourceManager::Instance()->RetrieveTempQuad(item->mDisplayImg,TEXTURE_SUB_5551);
    if (q.get() && q->mTex)
    {
        q->SetHotSpot(static_cast<float> (q->mTex->mWidth / 2), static_cast<float> (q->mTex->mHeight / 2));

        float scale = pos.actZ * displayScale * BigHeight / q->mHeight;
        q->SetColor(ARGB((int)pos.actA,255,255,255));
        renderer->RenderQuad(q.get(), x, pos.actY, pos.actT, scale, scale);
    }
    
    std::vector<string> txt = card->data->getFormattedText();
    size_t nbTextLines = txt.size();

    //Render the image on top of that
    quad->SetColor(ARGB((int)pos.actA,255,255,255));
    float imgScale = pos.actZ * (displayScale * (BigWidth - 15)) / quad->mWidth;
    float imgY = pos.actY - (20 * imgScale);
    if (nbTextLines > 6)
    {
        imgY -= 10 * imgScale;
        imgScale *= 0.75;
    }
    renderer->RenderQuad(quad, x, imgY, pos.actT, imgScale, imgScale);



    const vector<ModRulesRenderCardGuiItem *>& Carditems = gModRules.cardgui.rendertinycrop; //#W54-J (A43)
    
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::MAGIC_FONT);
    float backup_scale = font->GetScale();
    string sFormattedData = "";
    for( size_t i =0 ; i < Carditems.size(); i ++)
    {
        ModRulesRenderCardGuiItem * Carditem = Carditems[i];
        if (Carditem->mFilter.length() == 0 || FilterCard(card,Carditem->mFilter.c_str()))
        {

           if (Carditem->mFont) 
            {
                font->SetColor(Carditem->mFontColor);
                font->SetScale(((float)Carditem->mFontSize / 100) * pos.actZ);
                    
            }
            else
            {
                font->SetColor(ARGB((int)pos.actA, 0, 0, 0));
                font->SetScale(kWidthScaleFactor * pos.actZ);

            }
            
            if (Carditem->mName == "description")
            {

                const std::vector<string>& txt = card->data->getFormattedText(); //#W54-J (A43)
                float imgBottom = imgY + (imgScale * quad->mHeight / 2);

                unsigned i = 0;
                unsigned h = neofont ? 14 : 11;
                for (std::vector<string>::const_iterator it = txt.begin(); it != txt.end(); ++it, ++i)
                    font->DrawString(it->c_str(), x + (Carditem->mPosX - BigWidth / 2) * pos.actZ, imgBottom + (Carditem->mPosY + h * i) * pos.actZ);
            }
            else if (Carditem->mName == "mana")
            {
                // Mana
                // Need Create a realy generic struct for mana render
                ManaCost* manacost = card->data->getManaCost();
                ManaCostHybrid* h;
                ExtraCost* e;
                unsigned int j = 0;
                unsigned int z = 0;
                unsigned char t = (JGE::GetInstance()->GetTime() / 3) & 0xFF;
                unsigned char v = t + 127;
                float yOffset = (float)Carditem->mPosY;
                
                JQuadPtr quad = WResourceManager::Instance()->RetrieveQuad("menuicons.png", 0, 0, 0, 0, "", RETRIEVE_NORMAL, TEXTURE_SUB_5551, 1);
                if (quad.get())
                    if (quad->mHeight >= 78)
                    while ((e = manacost->getExtraCost(z)))
                    {
                            if(e->mCostRenderString == "Phyrexian Mana")
                            {
                                float _color = (float)card->data->getColor() -1;
                                JQuadPtr ExtraManas = WResourceManager::Instance()->RetrieveQuad("menuicons.png", 2 + _color * 36, 76, 32, 32, "c_extra", RETRIEVE_MANAGE);
                                ExtraManas->SetHotSpot(16, 16);
                                renderer->RenderQuad(ExtraManas.get(), x + (-12 * j + Carditem->mPosX) * pos.actZ, pos.actY + (yOffset) * pos.actZ, 0, 0.4f
                                * pos.actZ, 0.4f * pos.actZ);
                            }
                   
                        ++j;
                        ++z;
                    }
               
                z=0;
                while ((h = manacost->getHybridCost(z)))
                {
                    float scale = pos.actZ * 0.05f * cosf(2 * M_PI * ((float) t) / 256.0f);

                    if (scale < 0)
                    {
                        renderer->RenderQuad(manaIcons[h->color1].get(), x + (-12 * j + Carditem->mPosX + 3 * SineHelperFunction((float) t)) * pos.actZ,
                            pos.actY + (yOffset + 3 * CosineHelperFunction((float) t)) * pos.actZ, 0, 0.4f + scale, 0.4f
                            + scale);
                        renderer->RenderQuad(manaIcons[h->color2].get(), x + (-12 * j + Carditem->mPosX + 3 * SineHelperFunction((float) v)) * pos.actZ,
                            pos.actY + (yOffset + 3 * CosineHelperFunction((float) v)) * pos.actZ, 0, 0.4f - scale, 0.4f
                            - scale);
                    }
                    else
                    {
                        renderer->RenderQuad(manaIcons[h->color2].get(), x + (-12 * j + Carditem->mPosX + 3 * SineHelperFunction((float) v)) * pos.actZ,
                            pos.actY + (yOffset + 3 * CosineHelperFunction((float) v)) * pos.actZ, 0, 0.4f - scale, 0.4f
                            - scale);
                        renderer->RenderQuad(manaIcons[h->color1].get(), x + (-12 * j + Carditem->mPosX + 3 * SineHelperFunction((float) t)) * pos.actZ,
                            pos.actY + (yOffset + 3 * CosineHelperFunction((float) t)) * pos.actZ, 0, 0.4f + scale, 0.4f
                            + scale);
                    }
                    ++j;
                    ++z;
                }
                for (int i = Constants::NB_Colors - 2; i >= 1; --i)
                {
                     int cost;
                    for (cost = manacost->getCost(i); cost > 0; --cost)
                    {
                        renderer->RenderQuad(manaIcons[i].get(), x + (-12 * j + Carditem->mPosX) * pos.actZ, pos.actY + (yOffset) * pos.actZ, 0, 0.4f
                            * pos.actZ, 0.4f * pos.actZ);
                        ++j;
                    }
                    
                }
                // Colorless mana
                if (int cost = manacost->getCost(0))
                {
                    char buffer[10];
                    sprintf(buffer, "%d", cost);
                    renderer->RenderQuad(manaIcons[0].get(), x + (-12 * j + Carditem->mPosX) * pos.actZ, pos.actY + (yOffset) * pos.actZ, 0, 0.4f * pos.actZ,
                        0.4f * pos.actZ);
                    float w = font->GetStringWidth(buffer);
                    font->DrawString(buffer, x + (-12 * j + (Carditem->mPosX +1) - w / 2) * pos.actZ, pos.actY + (yOffset - 5) * pos.actZ);
                    ++j;
                }
                //Has X?
                if (manacost->hasX())
                {
                    char buffer[10];
                    sprintf(buffer, "X");
                    renderer->RenderQuad(manaIcons[0].get(), x + (-12 * j + Carditem->mPosX) * pos.actZ, pos.actY + (yOffset) * pos.actZ, 0, 0.4f * pos.actZ,
                        0.4f * pos.actZ);
                    float w = font->GetStringWidth(buffer);
                    font->DrawString(buffer, x + (-12 * j + (Carditem->mPosX + 1) - w / 2) * pos.actZ, pos.actY + (yOffset - 5) * pos.actZ);
                }
    
            }
            else if (Carditem->mName == "icon")
            {
                float yOffseticon = (float)Carditem->mPosY;
                JQuadPtr ExtraIcons = WResourceManager::Instance()->RetrieveQuad(Carditem->mFileName.c_str(), 2 + (float)(Carditem->mIconPosX - 1) * 36, (float)(Carditem->mIconPosY -1) * 38 , 32, 32, "", RETRIEVE_MANAGE);
                ExtraIcons->SetHotSpot(16,16);
                renderer->RenderQuad(ExtraIcons.get(), x + (Carditem->mPosX) * pos.actZ, pos.actY + (yOffseticon) * pos.actZ, 0, (float)Carditem->mSizeIcon * 0.4f
                * pos.actZ, (float)Carditem->mSizeIcon* 0.4f * pos.actZ);

            }
            else 
            {
                string formattedfield = Carditem->mFormattedData;
                size_t found = Carditem->mName.find("title"); // Write the title
                if (found != string::npos)
                {
                    stringstream st;
                    st << card->data->name;
                    formattedfield = FormattedData(formattedfield, "title", st.str());
                
                }

                found = Carditem->mName.find("power"); // Write the strength
                if (found != string::npos)
                {
                    stringstream st;
                    st << card->data->power;
                    formattedfield = FormattedData(formattedfield, "power", st.str());
                }
                found = Carditem->mName.find("life"); // Write the toughness
                if (found != string::npos)
                {
                    stringstream st;
                    st << card->data->toughness;
                    formattedfield = FormattedData(formattedfield, "life", st.str());

                }

                found = Carditem->mName.find("types"); //types
                if (found != string::npos)
                {
                    string s = "";
                    if (card->data->basicAbilities[(int)Constants::CHANGELING])
                    {
                        // this avoids drawing the list of subtypes on changeling cards.
                        if (card->data->types.size()){
                            s = _(MTGAllCards::findType(card->data->types[0])) + _(" - Shapeshifter");
                        } else {
                            s = _("Shapeshifter");
                        }
                    } else {
                        for (unsigned int i = 0; i < card->data->types.size() - 1; i++)
                        {
                            s += _(MTGAllCards::findType(card->data->types[i]));
                            if(!strcmp(_(MTGAllCards::findType(card->data->types[i])).c_str(),"Creature") || !strcmp(_(MTGAllCards::findType(card->data->types[i])).c_str(),"Land"))
                                s += _(" - ");
                            else
                                s += _(" ");
                        }
                        s += _(MTGAllCards::findType(card->data->types[card->data->types.size()-1]));
                    }
                    if (!card->data->types.size()){
                        DebugTrace("Typeless card: " << setlist[card->setId].c_str() << card->data->getName() << card->getId());
                    }
                    formattedfield = FormattedData(formattedfield, "types", s);
                }

                found = Carditem->mName.find("rarity");
                if (found != string::npos)
                {
                    
                    string sRarity;
                    switch(card->getRarity())
                    {
                    case Constants::RARITY_M:
                        sRarity ="Mythic";
                        break;
                    case Constants::RARITY_R:
                        sRarity ="Rare";
                        break;
                    case Constants::RARITY_U:
                        sRarity ="Uncommon";
                        break;
                    case Constants::RARITY_C:
                        sRarity ="Common";
                        break;
                    case Constants::RARITY_L:
                        sRarity ="Land";
                        break;
                    case Constants::RARITY_T:
                        sRarity ="Token";
                        break;
                    default:
                    case Constants::RARITY_S:
                        sRarity ="Special";
                        break;
                    }
                    formattedfield = FormattedData(formattedfield, "rarity", sRarity);
                }

                 found = Carditem->mName.find("expansion");
                if (found != string::npos)
                {
                    formattedfield = FormattedData(formattedfield, "expansion", setlist[card->setId].c_str());
                }

                if (!Carditem->mFont) 
                {          
                    float w = font->GetStringWidth(formattedfield.c_str()) * kWidthScaleFactor * pos.actZ;
                    if (w > BigWidth - 30)
                        font->SetScale((BigWidth - 30) / w);
                }
                font->DrawString(formattedfield.c_str(), x + (Carditem->mPosX  - BigWidth / 2) * pos.actZ, pos.actY + (Carditem->mPosY - BigHeight / 2) * pos.actZ);
            
            }
         
        }
    }


    
    font->SetScale(backup_scale);

    RenderCountersBig(card, pos);
}

//Renders a big card on screen. Defaults to the "alternate" rendering if no image is found
void CardGui::RenderBig(MTGCard* card, const Pos& pos, bool thumb, bool noborder, bool gdv)
{
#if defined(WAGIC_AUTODEMO) || defined(WAGIC_HWPROBE)
    static MTGCard * lastBig = NULL;
    if (card != lastBig)
    {
        lastBig = card;
        cardProbe("renderbig: %s", (card && card->data) ? card->data->getName().c_str() : "?");
    }
#endif
    JRenderer * renderer = JRenderer::GetInstance();
    //GameObserver * game = GameObserver::GetInstance();
    //if((MTGCard*)game->mLayers->actionLayer()->currentActionCard != NULL)
    //    card = (MTGCard*)game->mLayers->actionLayer()->currentActionCard;
    //i want this but ai targets cards so quickly that it can crash the game.
    float x = pos.actX;
    JQuadPtr alphabeta = WResourceManager::Instance()->RetrieveTempQuad("alphabeta.png");
    JQuadPtr quad = thumb ? WResourceManager::Instance()->RetrieveCard(card, RETRIEVE_THUMB)
                          : WResourceManager::Instance()->RetrieveCard(card);
    MTGCardInstance * kcard =  dynamic_cast<MTGCardInstance*>(card);
    GameObserver * game = NULL;
    //TargetChooser * tc = NULL;
    bool myA = true;
    if(kcard)
    {
        game = kcard->getObserver();
        if(game)
        {
            if(kcard->has(Constants::NECROED))
                myA = false;
            else
                myA = true;
        }
    }
    if(kcard && !kcard->isToken && kcard->name != kcard->model->data->name)
    {
        //W53-V (owner's Vita: a transformed Heliod showed the OTHER printing's
        //front art): the instance's OWN id is authoritative when it already
        //names this face - a flip rewrote it (AAFlip/AATurnSide setMTGId) and it
        //carries the right PRINTING. Re-resolving by NAME threw the printing
        //away and took whichever same-named printing sorts lowest. Keep the
        //name path only for copy/clone effects, where the id genuinely still
        //points at the original card.
        MTGCard * self = MTGCollection()->getCardById(kcard->getMTGId());
        if (!(self && self->data && self->data->name == kcard->name))
        {
            MTGCard * fcard = MTGCollection()->getCardByName(kcard->name);
            quad = thumb ? WResourceManager::Instance()->RetrieveCard(fcard, RETRIEVE_THUMB)
                         : WResourceManager::Instance()->RetrieveCard(fcard);
        }
    }
    if (kcard && kcard->hasCopiedToken && !quad.get())
    {
        MTGCard * tcard = MTGCollection()->getCardById(abs(kcard->copiedID));
        quad = thumb ? WResourceManager::Instance()->RetrieveCardToken(tcard, RETRIEVE_THUMB, 1, abs(kcard->copiedID))
                     : WResourceManager::Instance()->RetrieveCardToken(tcard, RETRIEVE_NORMAL, 1, abs(kcard->copiedID));
    }
    if (quad.get() && myA)
    {
        if (quad->mHeight < quad->mWidth)
        {
            return TinyCropRender(card, pos, quad.get());
        }
        quad->SetColor(ARGB(255,255,255,255));
        float scale = pos.actZ * 250.f / quad->mHeight;
        //init setname
        string cardsetname = setlist[card->setId].c_str();
        /*if(!noborder)
        {
            if(cardsetname == "2ED"||cardsetname == "RV"||cardsetname == "4ED"||cardsetname == "5ED"||cardsetname == "6ED"||cardsetname == "7ED"||cardsetname == "8ED"||cardsetname == "9ED"||cardsetname == "S00"||cardsetname == "S99"||cardsetname == "PTK"||cardsetname == "BTD"||cardsetname == "ATH"||cardsetname == "BRB"||cardsetname == "CHR"||cardsetname == "DM")
            {//Draw white border
                renderer->FillRoundRect((pos.actX - (pos.actZ * 84.f))-11.5f,(pos.actY - (pos.actZ * 119.7f))-14.f,pos.actZ * 168.f + 6.5f,pos.actZ * 239.4f + 12.f,8.f,ARGB(255,248,248,255));
                renderer->DrawRoundRect((pos.actX - (pos.actZ * 84.f))-11.5f,(pos.actY - (pos.actZ * 119.7f))-14.f,pos.actZ * 168.f + 6.5f,pos.actZ * 239.4f + 12.f,8.f,ARGB(150,20,20,20));
            }
            else
            {
                if(cardsetname == "LEA")
                {//BETA HAS REGULAR BORDER
                    //Draw more rounder black border
                    renderer->FillRoundRect((pos.actX - (pos.actZ * 84.f))-10.f,(pos.actY - (pos.actZ * 119.7f))-14.f,pos.actZ * 168.f - 0.5f,pos.actZ * 239.4f + 8.f,10.f,ARGB(255,5,5,5));
                    renderer->DrawRoundRect((pos.actX - (pos.actZ * 84.f))-10.f,(pos.actY - (pos.actZ * 119.7f))-14.f,pos.actZ * 168.f - 0.5f,pos.actZ * 239.4f + 8.f,10.f,ARGB(50,240,240,240));
                }
                else
                {//draw black border
                    renderer->FillRoundRect((pos.actX - (pos.actZ * 84.f))-11.5f,(pos.actY - (pos.actZ * 119.7f))-14.f,pos.actZ * 168.f + 6.5f,pos.actZ * 239.4f + 12.f,8.f,ARGB(255,5,5,5));
                    renderer->DrawRoundRect((pos.actX - (pos.actZ * 84.f))-11.5f,(pos.actY - (pos.actZ * 119.7f))-14.f,pos.actZ * 168.f + 6.5f,pos.actZ * 239.4f + 12.f,8.f,ARGB(50,240,240,240));
                }
            }
        }*///disabled this for universal border across game, deck editor, etc...
        //universal border
        if(options[Options::SHOWBORDER].number && !noborder)
        {
            if((cardsetname == "2ED"||cardsetname == "RV"||cardsetname == "4ED"||cardsetname == "5ED"||cardsetname == "6ED"||cardsetname == "7ED"||cardsetname == "8ED"||cardsetname == "9ED"||cardsetname == "S00"||cardsetname == "S99"||cardsetname == "PTK"||cardsetname == "BTD"||cardsetname == "ATH"||cardsetname == "BRB"||cardsetname == "CHR"||cardsetname == "DM")
                && !options[Options::BLKBORDER].number)
            {//white border
                renderer->FillRoundRect(pos.actX - (scale * quad->mWidth / 2)-6.f,pos.actY - (scale * quad->mHeight / 2)-5.8f, (scale * quad->mWidth)-0.02f, (scale * quad->mHeight)-0.02f, 5.8f,ARGB(255,248,248,255));
                renderer->DrawRoundRect(pos.actX - (scale * quad->mWidth / 2)-6.f,pos.actY - (scale * quad->mHeight / 2)-5.8f, (scale * quad->mWidth)-0.02f, (scale * quad->mHeight)-0.02f, 5.8f,ARGB(150,20,20,20));
            }
            else
            {//black border
                renderer->FillRoundRect(pos.actX - (scale * quad->mWidth / 2)-6.f,pos.actY - (scale * quad->mHeight / 2)-5.8f, (scale * quad->mWidth)-0.02f, (scale * quad->mHeight)-0.02f, 5.8f,ARGB(255,5,5,5));
                renderer->DrawRoundRect(pos.actX - (scale * quad->mWidth / 2)-6.f,pos.actY - (scale * quad->mHeight / 2)-5.8f, (scale * quad->mWidth)-0.02f, (scale * quad->mHeight)-0.02f, 5.8f,ARGB(50,240,240,240));
            }
            //end new border
            //draw inner border
            if(cardsetname == "LEA"||cardsetname == "LEB")
            {
                if(alphabeta.get())
                {
                    alphabeta->SetHotSpot(static_cast<float> (alphabeta->mWidth / 2), static_cast<float> (alphabeta->mHeight / 2));
                    float myscale = pos.actZ * 255 / alphabeta->mHeight;
                    alphabeta->SetColor(ARGB((int)pos.actA,255,255,255));
                    renderer->RenderQuad(alphabeta.get(), x, pos.actY+0.2f, pos.actT, myscale, myscale);
                }
            }
        }
        float modxscale = (cardsetname =="UNH")?0.015f:0.0f;
        float modyscale = (cardsetname =="UNH")?0.010f:0.0f;
        float gdvadd = gdv?0.008f:0.0f;//scale add grid deck view
        renderer->RenderQuad(quad.get(), x, pos.actY, pos.actT, (scale-0.005f)+modxscale+gdvadd, (scale-0.005f)+modyscale+gdvadd);

        RenderCountersBig(card, pos);
        return;
    }

    //DebugTrace("Unable to fetch image: " << card->getImageName());

    // If we come here, we do not have the picture.
    if(myA)
        AlternateRender(card, pos);
}

string CardGui::FormattedData(string data, string replace, string value)
{
    size_t found = data.find(replace.c_str());
    if (found != string::npos)
    {
        size_t len = replace.length();
        string teste = data.replace(found,len,value);
        return teste;
    }
    else
    {
        return value;
    }

}

bool CardGui::FilterCard(MTGCard * _card,string filter)
{
    CardDescriptor  cd;
    //#W54-J (A43): `_card->data` is a CardPrimitive*, and CardDescriptor::
    //match reads MTGCardInstance fields past it (foretellTurn, kicked,
    //counters, zpos, name...). Match the REAL instance when we were handed
    //one (every duel-time render); a bare MTGCard (deck viewer, shop) only
    //carries a primitive, so its match is restricted to the primitive-safe
    //comparisons (types/colors) below.
    MTGCardInstance * card = dynamic_cast<MTGCardInstance*>(_card);
    const bool primitiveOnly = (card == NULL);
    if (primitiveOnly)
        card = (MTGCardInstance*) _card->data;
    cd.init();
    cd.mode = CardDescriptor::CD_OR;
    while (filter.size())
    {
        
        string typeName;
        //Advanced cards caracteristics ?
         size_t found = filter.find("[");
        if (found != string::npos)
        {
            int nbminuses = 0;
            int end = filter.find("]");
            string attributes = filter.substr(found + 1, end - found - 1);
            
            while (attributes.size())
            {
                size_t found2 = attributes.find(";");
                size_t foundAnd = attributes.find("&");
                string attribute;
                if (found2 != string::npos)
                {
                    cd.mode = CardDescriptor::CD_OR;
                    attribute = attributes.substr(0, found2);
                    attributes = attributes.substr(found2 + 1);
                }
                else if (foundAnd != string::npos)
                {
                    cd.mode = CardDescriptor::CD_AND;
                    attribute = attributes.substr(0, foundAnd);
                    attributes = attributes.substr(foundAnd + 1);
                }
                else
                {
                    attribute = attributes;
                    attributes = "";
                }
                int minus = 0;
                if (attribute[0] == '-')
                {
                    minus = 1;
                    nbminuses++;
                    attribute = attribute.substr(1);
                }
                int comparisonMode = COMPARISON_NONE;
                int comparisonCriterion = 0;
                if (attribute.size() > 1)
                {
                    size_t operatorPosition = attribute.find("=", 1);
                    if (operatorPosition != string::npos)
                    {
                        string numberCD = attribute.substr(operatorPosition + 1, attribute.size() - operatorPosition - 1);
                        
                        switch (attribute[operatorPosition - 1])
                        {
                        case '<':
                            if (minus)
                            {
                                comparisonMode = COMPARISON_GREATER;
                            }
                            else
                            {
                                comparisonMode = COMPARISON_AT_MOST;
                            }
                            operatorPosition--;
                            break;
                        case '>':
                            if (minus)
                            {
                                comparisonMode = COMPARISON_LESS;
                            }
                            else
                            {
                                comparisonMode = COMPARISON_AT_LEAST;
                            }
                            operatorPosition--;
                            break;
                        default:
                            if (minus)
                            {
                                comparisonMode = COMPARISON_UNEQUAL;
                            }
                            else
                            {
                                comparisonMode = COMPARISON_EQUAL;
                            }
                        }
                        attribute = attribute.substr(0, operatorPosition);
                    }
                }

                //Attacker
                if (attribute.find("attacking") != string::npos)
                {
                    if (minus)
                    {
                        cd.attacker = -1;
                    }
                    else
                    {
                        cd.attacker = 1;
                    }
                }
                //Blocker
                else if (attribute.find("blocking") != string::npos)
                {
                    if (minus)
                    {
                        cd.defenser = &MTGCardInstance::NoCard;
                    }
                    else
                    {
                        cd.defenser = &MTGCardInstance::AnyCard;
                    }
                }
                //Tapped, untapped
                else if (attribute.find("tapped") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetTapped(-1);
                    }
                    else
                    {
                        cd.unsecureSetTapped(1);
                    }
                }
                //Has been foretold
                else if (attribute.find("foretold") != string::npos)
                {
                    cd.foretellTurn = comparisonCriterion;
                    cd.foretoldComparisonMode = comparisonMode;
                }
                //Has been kicked
                else if (attribute.find("kicked") != string::npos)
                {
                    cd.kicked = comparisonCriterion;
                    cd.kickedComparisonMode = comparisonMode;
                }
                //Has kicker cost
                else if (attribute.find("haskicker") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetHasKickerCost(-1);
                    }
                    else
                    {
                        cd.unsecureSetHasKickerCost(1);
                    }
                }
                //Has convoke cost
                else if (attribute.find("hasconvoke") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetHasConvokeCost(-1);
                    }
                    else
                    {
                        cd.unsecureSetHasConvokeCost(1);
                    }
                }
                //Has flashback cost
                else if (attribute.find("hasflashback") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetHasFlashbackCost(-1);
                    }
                    else
                    {
                        cd.unsecureSetHasFlashbackCost(1);
                    }
                }
                //Has backside
                else if (attribute.find("hasbackside") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetHasBackSide(-1);
                    }
                    else
                    {
                        cd.unsecureSetHasBackSide(1);
                    }
                }
                //Is modified
                else if (attribute.find("modified") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetModified(-1);
                    }
                    else
                    {
                        cd.unsecureSetModified(1);
                    }
                }
                //Has toxic
                else if (attribute.find("hastoxic") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetHasToxic(-1);
                    }
                    else
                    {
                        cd.unsecureSetHasToxic(1);
                    }
                }
                //Has partner
                else if (attribute.find("haspartner") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetHasPartner(-1);
                    }
                    else
                    {
                        cd.unsecureSetHasPartner(1);
                    }
                }
                //Is a permanent
                else if (attribute.find("permanent") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecureSetIsPermanent(-1);
                    }
                    else
                    {
                        cd.unsecureSetIsPermanent(1);
                    }
                }
                //Token
                else if (attribute.find("token") != string::npos)
                {
                    if (minus)
                    {
                        cd.isToken = -1;
                    }
                    else
                    {
                        cd.isToken = 1;
                    }
                }
                //Has been flipped
                else if (attribute.find("isflipped") != string::npos)
                {
                    if (minus)
                    {
                        cd.isFlipped = -1;
                    }
                    else
                    {
                        cd.isFlipped = 1;
                    }
                }
                //Card is commander
                else if (attribute.find("iscommander") != string::npos)
                {
                    if (minus)
                    {
                        cd.isCommander = -1;
                    }
                    else
                    {
                        cd.isCommander = 1;
                    }
                }
                //Card is Ring bearer
                else if (attribute.find("ringbearer") != string::npos)
                {
                    if (minus)
                    {
                        cd.isRingBearer = -1;
                    }
                    else
                    {
                        cd.isRingBearer = 1;
                    }
                }
                //Has x in cost
                else if (attribute.find("hasx") != string::npos)
                {
                    if (minus)
                    {
                        cd.hasXCost = -1;
                    }
                    else
                    {
                        cd.hasXCost = 1;
                    }
                }
                //has been discarded
                else if (attribute.find("discarded") != string::npos)
                {
                    if (minus)
                    {
                        cd.CDdiscarded = -1;
                    }
                    else
                    {
                        cd.CDdiscarded = 1;
                    }
                }
                //put in its zone this turn
                else if (attribute.find("fresh") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecuresetfresh(-1);
                    }
                    else
                    {
                        cd.unsecuresetfresh(1);
                    }
                }
                else if (attribute.find("recent") != string::npos)
                {
                    if (minus)
                    {
                        cd.unsecuresetrecent(-1);
                    }
                    else
                    {
                        cd.unsecuresetrecent(1);
                    }
                }
                else if (attribute.find("geared") != string::npos || attribute.find("equipped") != string::npos)
                {
                    if (minus)
                    {
                        cd.CDgeared = -1;
                    }
                    else
                    {
                        cd.CDgeared = 1;
                    }
                }
                else if (attribute.find("attached") != string::npos)
                {
                    if (minus)
                    {
                        cd.CDattached = -1;
                    }
                    else
                    {
                        cd.CDattached = 1;
                    }
                }
                //creature is a level up creature
                else if (attribute.find("leveler") != string::npos)
                {
                    if (minus)
                    {
                        cd.isLeveler = -1;
                    }
                    else
                    {
                        cd.isLeveler = 1;
                    }
                }
                //creature is enchanted
                else if (attribute.find("enchanted") != string::npos)
                {
                    if (minus)
                    {
                        cd.CDenchanted = -1;
                    }
                    else
                    {
                        cd.CDenchanted = 1;
                    }
                }
                //creature was damaged
                else if (attribute.find("damaged") != string::npos)
                {
                    if (minus)
                    {
                        cd.CDdamaged = -1;
                    }
                    else
                    {
                        cd.CDdamaged = 1;
                    }
                }
                //creature dealt damage to opponent
                else if (attribute.find("opponentdamager") != string::npos)
                {
                    if (minus)
                    {
                        cd.CDopponentDamaged = -1;
                    }
                    else
                    {
                        cd.CDopponentDamaged = 1;
                    }
                }
                //creature dealt damage to controller
                else if (attribute.find("controllerdamager") != string::npos)
                {
                    if (minus)
                    {
                        cd.CDcontrollerDamaged = -1;
                    }
                    else
                    {
                        cd.CDcontrollerDamaged = 1;
                    }
                }
                //creature dealt damage to anything
                else if (attribute.find("damager") != string::npos)
                {
                    if (minus)
                    {
                        cd.CDdamager = -1;
                    } 
                    else 
                    {
                        cd.CDdamager = 1;
                    }
                }
                else if (attribute.find("multicolor") != string::npos)
                {
                    //card is multicolored?
                    if (minus)
                    {
                        //Kept byte-for-byte in step with the TargetChooser.cpp
                        //`-multicolor` branch, or the card browser's rendering
                        //of a predicate disagrees with what the game enforces.
                        //The two SetExclusionColor calls removed here passed
                        //colour INDICES that resolve to TYPE bits (artifact,
                        //and WASTE which shares the land bit), so they rejected
                        //every artifact and every land regardless of colour
                        //(N-146j). See the long note at the TargetChooser site.
                        cd.setisMultiColored(-1);
                    }
                    else
                    {
                        cd.setisMultiColored(1);
                    }

                }
                else if (attribute.find("numofcols") != string::npos)
                {
                    //Number of color restrictions
                    cd.numofColors = comparisonCriterion;
                    cd.numofColorsComparisonMode = comparisonMode;
                }
                else if (attribute.find("power") != string::npos)
                {
                    //Power restrictions
                    cd.setPower(comparisonCriterion);
                    cd.powerComparisonMode = comparisonMode;
                }
                else if (attribute.find("toughness") != string::npos)
                {
                    //Toughness restrictions
                    cd.setToughness(comparisonCriterion);
                    cd.toughnessComparisonMode = comparisonMode;
                }
                else if (attribute.find("zpos") != string::npos)
                {//using > or < don't have effect unless like this: >= or <= or =
                    //zpos restrictions
                    cd.zposition = comparisonCriterion;
                    cd.zposComparisonMode = comparisonMode;
                }
                else if (attribute.find("manacost") != string::npos)
                {
                    //Manacost restrictions
                    cd.convertedManacost = comparisonCriterion;
                    cd.manacostComparisonMode = comparisonMode;
                }
                else
                {
                    int attributefound = 0;
                    ////Colors - remove Artifact and Land from the loop
                    
                    for (int cid = 1; cid < Constants::NB_Colors - 1; cid++)
                    { 
                        if (attribute.find(Constants::MTGColorStrings[cid]) != string::npos)
                        {
                            attributefound = 1;
                            if (minus)
                                cd.SetExclusionColor(cid);
                            else
                                cd.setColor(cid);
                        }
                    }
                    if (!attributefound)
                    {
                        //Abilities
                        for (int j = 0; j < Constants::NB_BASIC_ABILITIES; j++)
                        {
                            if (attribute.find(Constants::MTGBasicAbilities[j]) != string::npos)
                            {
                                attributefound = 1;
                                if (minus)
                                    cd.mAbilityExclusions.set(j);
                                else
                                    cd.basicAbilities.set(j);
                            }
                        }
                    }

                    if (!attributefound)
                    {
                        //Subtypes
                        if (minus)
                        {
                            cd.setNegativeSubtype(attribute);
                        }
                        else
                        {
                            cd.setSubtype(attribute);
                        }
                    }
                }
            }
            if (nbminuses)
                cd.mode = CardDescriptor::CD_AND;
            filter = filter.substr(0, found);
        }
        else
        {
            
            found = filter.find(",");
            
            if (found != string::npos)
            {
                cd.mode = CardDescriptor::CD_OR;
                typeName = filter.substr(0, found);
                filter = filter.substr(found + 1);
            }
            else
            {
                typeName = filter;
                filter = "";
            }
            
             cd.setSubtype(typeName);
        }

        
     } 
     //#W54-J (A43): see above - a primitive cannot answer these, so a
     //filter that needs them does not match a bare MTGCard rather than read
     //past the object.
     if (primitiveOnly && (cd.powerComparisonMode || cd.toughnessComparisonMode || cd.manacostComparisonMode
                           || cd.zposComparisonMode || cd.nameComparisonMode || cd.foretoldComparisonMode
                           || cd.kickedComparisonMode || cd.counterComparisonMode))
         return false;
     if(cd.match(card))
         return true;
    return false;

}

void CardGui::RenderCountersBig(MTGCard * mtgcard, const Pos& pos, int drawMode)
{
    MTGCardInstance * card = dynamic_cast<MTGCardInstance*> (mtgcard);
    if (!card)
        return;

    int ringTemptations = 0;
    if(card->controller())
        ringTemptations = card->controller()->ringTemptations;

    if (!card->counters && !ringTemptations)
        return;
    if (!card->counters->mCount && !ringTemptations)
        return;

    // Write Named Counters
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::MAGIC_FONT);
    font->SetColor(ARGB((int)pos.actA, 0, 0, 0));
    font->SetScale(kWidthScaleFactor * pos.actZ);

    unsigned i = 0; 
    if (drawMode == DrawMode::kText)
    {
        std::vector<string> txt = card->data->getFormattedText();
        i = txt.size() + 1;
    }
    
    if (ringTemptations > 0 && card->name == "The Ring") // Added a label on Ring to show the number of temptations.
    {
        char buf[512];
        bool renderText = true;
        string gfx = "";
        if (counterGraphics.find("temptations") == counterGraphics.end())
        {
            string gfxRelativeName = "counters/";
            gfxRelativeName.append("temptations");
            gfxRelativeName.append(".png");
            string _gfx = WResourceManager::Instance()->graphicsFile(gfxRelativeName);
            if (!fileExists(_gfx.c_str()))
                _gfx = "";
            counterGraphics["temptations"] = _gfx;
        }
        gfx = counterGraphics["temptations"];
        if (gfx.size())
            renderText = false;
           
        if (renderText)
        {
            std::string s = "temptations";
            s[0] = toupper(s[0]);
            sprintf(buf, _("%s: %i").c_str(), s.c_str(), ringTemptations);
        }
        
        if (!gfx.size())
        {
            gfx = "counters/default.png";
        }
        
        float x = pos.actX + (22 - BigWidth / 2) * pos.actZ;
        float y =  pos.actY + (-BigHeight / 2 + 80 + 11 * i + 21 * 0) * pos.actZ;
        if (y > pos.actY + 105) 
        {
           y =  (-BigHeight / 2 + 80 + 11 * i) * pos.actZ + (y - 105 - 21);
           x +=  (BigWidth / 2) * pos.actZ;
        }

        if (gfx.size())
        {
            JQuadPtr q = WResourceManager::Instance()->RetrieveTempQuad(gfx);

            if (q.get() && q->mTex)
            {
                float scale = 20.f / q->mHeight;
                if (renderText)
                {
                    float scaleX = (font->GetStringWidth(buf) + 20) / q->mWidth;
                    JRenderer::GetInstance()->RenderQuad(q.get(), x, y, 0, scaleX, scale);
                }
                else
                {
                    JRenderer::GetInstance()->RenderQuad(q.get(), x + (scale * q->mWidth * 0), y, 0, scale, scale);
                }
            }
        }

        if (renderText)
        {
            font->SetColor(ARGB(255,0,0,0));
            font->DrawString(buf, x + 5, y + 5);
        }

        return; // No need to check counters on The Ring.
    }

    for (size_t t = 0; t < card->counters->counters.size(); t++)
    {
        Counter * c = card->counters->counters[t];

        if (!c || c->nb <= 0)
            continue;

        char buf[512];
        bool renderText = true;
        string gfx = "";
        //TODO cache the gfx fetch results?
        if (c->name.size()) 
        {
            if (c->nb < 6) //we only render a counter's specific quad if there are 5 counters of this type or less. Otherwise we will use the generic one
            {
                if (counterGraphics.find(c->name) == counterGraphics.end())
                {
                    string gfxRelativeName = "counters/";
                    gfxRelativeName.append(c->name);
                    gfxRelativeName.append(".png");
                    string _gfx = WResourceManager::Instance()->graphicsFile(gfxRelativeName);
                    if (!fileExists(_gfx.c_str()))
                        _gfx = "";
                    counterGraphics[c->name] = _gfx;
                }
                gfx = counterGraphics[c->name];
                if (gfx.size())
                    renderText = false;
            }

            if (renderText)
            {
                std::string s = c->name;
                s[0] = toupper(s[0]);
                sprintf(buf, _("%s: %i").c_str(), s.c_str(), c->nb);
            }
        }
        else
        {
            sprintf(buf, _("%s%i/%s%i").c_str(), ((c->power > 0) ? "+": ""), c->power * c->nb, ((c->toughness > 0) ? "+": ""),c->toughness* c->nb);
        }

        if (!gfx.size())
        {
            gfx = "counters/default.png";
        }
        
        float x = pos.actX + (22 - BigWidth / 2) * pos.actZ;
        float y =  pos.actY + (-BigHeight / 2 + 80 + 11 * i + 21 * t) * pos.actZ;
        if (y > pos.actY + 105) 
        {
           y =  (-BigHeight / 2 + 80 + 11 * i) * pos.actZ + (y - 105 - 21);
           x +=  (BigWidth / 2) * pos.actZ;
        }

        if (gfx.size())
        {
            JQuadPtr q = WResourceManager::Instance()->RetrieveTempQuad(gfx);

            if (q.get() && q->mTex)
            {
                float scale = 20.f / q->mHeight;
                if (renderText)
                {
                    float scaleX = (font->GetStringWidth(buf) + 20) / q->mWidth;
                    JRenderer::GetInstance()->RenderQuad(q.get(), x, y, 0, scaleX, scale);
                }
                else
                {
                    for (int j = 0; j < c->nb; ++j)
                    {
                        JRenderer::GetInstance()->RenderQuad(q.get(), x + (scale * q->mWidth * j), y, 0, scale, scale);
                    }
                }
            }
        }

        if (renderText)
        {
            font->SetColor(ARGB(255,0,0,0));
            font->DrawString(buf, x + 5, y + 5);
        }
    }
    
}

MTGCardInstance* CardView::getCard()
{
    return card;
}

TransientCardView::TransientCardView(MTGCardInstance* card, float x, float y)
    : CardGui(card, x, y)
{
}

TransientCardView::TransientCardView(MTGCardInstance* card, const Pos& ref)
    : CardGui(card, ref)
{
}
;

ostream& CardView::toString(ostream& out) const
{
    return (CardGui::toString(out) << " : CardView ::: card : " << card << ";  actX,actY : " << actX << "," << actY << "; t : "
        << t << " ; actT : " << actT);
}
ostream& CardGui::toString(ostream& out) const
{
    return (out << "CardGui ::: x,y " << x << "," << y);
}


SimpleCardEffectRotate::SimpleCardEffectRotate(float rotation): mRotation(rotation)
{
}
    
void SimpleCardEffectRotate::doEffect(Pos * card)
{
    card->t = mRotation;
}

void SimpleCardEffectRotate::undoEffect(Pos * card)
{
    card->t = 0;
}

SimpleCardEffectMask::SimpleCardEffectMask(PIXEL_TYPE mask): mMask(mask)
{
}
    
void SimpleCardEffectMask::doEffect(Pos * card)
{
    card->mask = mMask;
}

void SimpleCardEffectMask::undoEffect(Pos * card)
{
    card->mask = 0;
}

