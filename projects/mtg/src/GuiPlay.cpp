#include "PrecompiledHeader.h"

#include "CardSelector.h"
#include "GameApp.h"
#include "GuiPlay.h"
#include "Subtypes.h"
#include "Trash.h"
#include "ModRules.h"
#include "DuelLayers.h"
#include "ActionLayer.h"
#include "ActionStack.h"
#include "GameObserver.h"
#include "Player.h"
#include <map>
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
#include <time.h>
#endif

#define CARD_WIDTH (31)

const float GuiPlay::HORZWIDTH = 300.0f;
const float GuiPlay::VERTHEIGHT = 80.0f;


void GuiPlay::CardStack::reset(unsigned total, float x, float y)
{
    this->total = total;
    this->x = 0;
    baseX = x;
    this->y = 0;
    baseY = y;
}

void GuiPlay::CardStack::RenderSpell(MTGCardInstance* card, iterator begin, iterator end, float x, float y)
{
    while (begin != end)
    {
        if ((*begin)->card->target == card)
        {
            RenderSpell(card, begin + 1, end, x, y - 10);
            (*begin)->x = x;
            (*begin)->y = y;
            (*begin)->Render();
            return;
        }
        ++begin;
    }
}

GuiPlay::HorzStack::HorzStack()
{
}
GuiPlay::VertStack::VertStack()
{
}

void GuiPlay::VertStack::reset(unsigned total, float x, float y)
{
    GuiPlay::CardStack::reset(total, x - CARD_WIDTH, y);
    count = 0;
}

void GuiPlay::HorzStack::Render(CardView* card, iterator begin, iterator end)
{
    RenderSpell(card->card, begin, end, card->x, card->y - 10);
    card->Render();
}

void GuiPlay::HorzStack::Enstack(CardView* card)
{
    card->x = x + baseX;
    card->y = y + baseY;
    if (total < 8)
        x += CARD_WIDTH;
    else if (total < 16)
        x += (SCREEN_WIDTH - 200 - baseX) / total;
    else
        x += (SCREEN_WIDTH - 50 - baseX) / total;
}

void GuiPlay::VertStack::Enstack(CardView* card)
{
    int modulus = total < 10 ? 3 : 5;
    {
        if (0 == count % modulus)
        {
            x += CARD_WIDTH;
            y = 0;
        }
    }

    card->x = x + baseX;
    card->y = y + baseY;
    y += 12;
    if (++count == total - 1 && y == 12)
        y += 12;
}

void GuiPlay::VertStack::Render(CardView* card, iterator begin, iterator end)
{
    RenderSpell(card->card, begin, end, card->x + 5, card->y - 10);
    card->Render();
}

inline float GuiPlay::VertStack::nextX()
{
    if (0 == count)
        return x + CARD_WIDTH;
    else
        return x;
}

GuiPlay::BattleField::BattleField() :
    attackers(0), height(0.0), red(0), colorFlow(0)
{
}
const float GuiPlay::BattleField::HEIGHT = 146.0f;
void GuiPlay::BattleField::addAttacker(MTGCardInstance*)
{
    ++attackers;
    colorFlow = 1;
}
void GuiPlay::BattleField::removeAttacker(MTGCardInstance*)
{
    --attackers;
}
void GuiPlay::BattleField::reset(float x, float y)
{
    HorzStack::reset(0, x, y);
    currentAttacker = 1;
}
void GuiPlay::BattleField::EnstackAttacker(CardView* card)
{
    if(card->card->getObserver() && ((card->card->getObserver()->getCurrentGamePhase() >= MTG_PHASE_COMBATDAMAGE) && (card->card->getObserver()->getCurrentGamePhase() < MTG_PHASE_ENDOFTURN)))
        return;
    //card->x = CARD_WIDTH + 20 + (currentAttacker * (HORZWIDTH) / (attackers+1));
    card->x = x + (CARD_WIDTH/2.5f) + baseX;
    if (attackers+1 < 8)
        x += CARD_WIDTH;
    else if (attackers+1 < 24)
        x += (SCREEN_WIDTH - 200 - baseX) / attackers+1;
    else
        x += (HORZWIDTH - baseX) / attackers+1;

    card->y = baseY + (card->card->getObserver()->getView()->getRenderedPlayer() == card->card->controller() ? 20 + y : -20 - y);
    ++currentAttacker;
    //  JRenderer::GetInstance()->RenderQuad(WResourceManager::Instance()->GetQuad("BattleIcon"), card->actX, card->actY, 0, 0.5 + 0.1 * sinf(JGE::GetInstance()->GetTime()), 0.5 + 0.1 * sinf(JGE::GetInstance()->GetTime()));
}
void GuiPlay::BattleField::EnstackBlocker(CardView* card)
{
    if(card->card->getObserver() && ((card->card->getObserver()->getCurrentGamePhase() >= MTG_PHASE_COMBATDAMAGE) && (card->card->getObserver()->getCurrentGamePhase() < MTG_PHASE_ENDOFTURN)))
        return;
    MTGCardInstance * c = card->card;
    if (!c)
        return;
    int offset = 0;
    if (c->defenser && c->defenser->view)
    {
        offset = c->defenser->getDefenserRank(c);
        card->x = c->defenser->view->x + 5 * offset;
    }
    card->y = baseY + (card->card->getObserver()->getView()->getRenderedPlayer() == card->card->controller() ? 20 + y + 6 * offset : -20 - y + 6 * offset);
}
void GuiPlay::BattleField::Update(float dt)
{
    if (0 == attackers)
        height -= 10 * dt * height;
    else
        height += 10 * dt * (HEIGHT - height);

    if (colorFlow)
    {
        red += static_cast<int> (colorFlow * 300 * dt);
        if (red < 0)
            red = 0;
        if (red > 70)
            red = 70;
    }
}
void GuiPlay::BattleField::Render()
{
    if (height > 3)
    {
        JRenderer::GetInstance()->FillRect(0, SCREEN_HEIGHT / 2 + 8.5f - height / 2, 480, height, ARGB(127, red, 0, 0));
        if(red > 1)
            JRenderer::GetInstance()->DrawRect(-2, SCREEN_HEIGHT / 2 + 8.5f - height / 2, 484, height, ARGB(255, 255, 165, 0));        
    }
}

const float GuiPlay::STACKFANPITCH = 18.0f; //#W57-G (D42)

GuiPlay::GuiPlay(DuelLayers* view) :
    GuiLayer(view)
{
    wave = 0;
    mLayoutDirty = true;
    mStacksPinned = false;   //#W57-G (D42)
    mStackSig = NULL;        //#W57-G (D42)
    end_spells = cards.end();
}

//#W57-G (D42): the "do not move anything under the player" predicate.
//A live TargetChooser, an ability waiting for an answer, or ANY unresolved
//stack entry all mean a decision is in flight whose memory aid is positional -
//the owner's words, 2026-09-03: "since all the cards maintain positioning, the
//user remembers what they have targeted". While any of those hold, the board
//renders exactly as it did before this lane existed: every permanent in its
//own slot. It re-collapses only once the stack is empty and no chooser is up.
bool GuiPlay::stacksPinnedNow()
{
    if (!observer)
        return true;
    if (observer->getCurrentTargetChooser())
        return true;
    if (observer->mLayers)
    {
        if (observer->mLayers->actionLayer() && observer->mLayers->actionLayer()->isWaitingForAnswer())
            return true;
        if (observer->mLayers->stackLayer() && observer->mLayers->stackLayer()->count(0, NOT_RESOLVED) > 0)
            return true;
    }
    for (iterator it = cards.begin(); it != cards.end(); ++it)
    {
        if (!(*it))
            continue;
        if ((*it)->mStackForceExpand)
            return true;
        //Declare-attackers and declare-blockers are live click prompts that need
        //ONE specific body, and the board is at its widest exactly then. The
        //first cut fanned the affected piles in place, which is cramped and -
        //worse - puts the bodies somewhere the player has not seen them before.
        //Pinning instead gives the familiar ungrouped board for the whole
        //window, which is the same answer the chooser case gets and for the
        //same reason. The flags are refreshed by GuiHandSelf::Update and are
        //only ever set inside those two windows, for the acting seat.
        if ((*it)->card && ((*it)->card->canAttackNow || (*it)->card->canBlockNow))
            return true;
    }
    return false;
}

void GuiPlay::computeStacks()
{
    mStackFollowers.clear();

    const bool pinned = stacksPinnedNow();
    mStacksPinned = pinned;
    const bool grouping = wagicBoardGroupingEnabled() && !pinned;

    //Baseline: everything draws itself, in its own slot. This IS the pre-D42
    //board, and it is also what an unpinned-but-option-off board must be, so
    //the reset runs unconditionally rather than in an else branch.
    for (iterator it = cards.begin(); it != cards.end(); ++it)
    {
        CardView * cv = *it;
        if (!cv) continue;
        cv->mStackCount = 1;
        cv->mStackHidden = false;
        cv->mStackFanIndex = 0;
        if (!pinned)
            cv->mStackForceExpand = false;
    }
    if (!grouping)
        return;

    //Group in cards[] order, so the drawn member of a pile is the one that was
    //already leftmost - the pile does not jump when its composition changes.
    std::map<std::string, CardView*> firstOf;
    std::map<CardView*, vector<CardView*> > members;
    for (iterator it = cards.begin(); it != cards.end(); ++it)
    {
        CardView * cv = *it;
        if (!cv || !cv->card)
            continue;
        //An attached aura/equipment has no slot of its own (RenderSpell draws
        //it on its host), so it never stacks - it follows whatever its host does.
        if (cv->card->target)
            continue;
        //Cards still fading in are click-invisible to the selector (closest()
        //drops actA < 32); stacking them would hide an arrival mid-animation.
        if (cv->actA < 32)
            continue;
        const std::string key = wagicBoardStackKey(cv->card);
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
        {
            static int dumpOn = -1;
            if (dumpOn < 0) { const char* e = getenv("WAGIC_BOARDGROUP_PROBE"); dumpOn = (e && atoi(e) >= 2) ? 1 : 0; }
            static int nth = 0;
            if (dumpOn && (++nth % 400) < 40)
                fprintf(stderr, "#W57-G key %-14s %s\n", cv->card->getName().c_str(), key.c_str());
        }
#endif
        std::map<std::string, CardView*>::iterator f = firstOf.find(key);
        if (f == firstOf.end())
            firstOf[key] = cv;
        else
            members[f->second].push_back(cv);
    }

    for (std::map<CardView*, vector<CardView*> >::iterator g = members.begin(); g != members.end(); ++g)
    {
        CardView * lead = g->first;
        vector<CardView*>& rest = g->second;
        if (rest.empty())
            continue;

        //A pile EXPANDS when the cursor is inside it (so any one member can be
        //reached and clicked) or when a live click prompt is aimed at that
        //class of permanent - declare-attackers and declare-blockers are
        //exactly the windows where the engine needs ONE specific body, and the
        //display flags that drive the orange halo answer that per group (they
        //are part of the stack key, so every member agrees).
        //Focus is the only in-place expansion left: combat windows and choosers
        //pin the whole board instead (see stacksPinnedNow).
        bool expand = lead->mHasFocus || lead->mStackForceExpand;
        for (size_t i = 0; i < rest.size() && !expand; ++i)
            if (rest[i]->mHasFocus || rest[i]->mStackForceExpand)
                expand = true;

        if (expand)
        {
            //Fanned IN PLACE: the pile keeps its one slot and its members are
            //drawn at increasing offsets from it. Nothing else on the row moves,
            //and the offsets are distinct so the d-pad walks members with the
            //same left/right presses that walk piles.
            for (size_t i = 0; i < rest.size(); ++i)
            {
                rest[i]->mStackFanIndex = (int) (i + 1);
                mStackFollowers.push_back(make_pair(rest[i], lead));
            }
        }
        else
        {
            lead->mStackCount = (int) rest.size() + 1;
            for (size_t i = 0; i < rest.size(); ++i)
            {
                rest[i]->mStackHidden = true;
                mStackFollowers.push_back(make_pair(rest[i], lead));
            }
        }
    }
}

#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
//#W57-G (D42): the before/after instrument. WAGIC_BOARDGROUP_PROBE=1 prints one
//stderr line every 120 GuiPlay::Render calls: wall time per frame in this
//layer, how many battlefield cards exist, and how many actually got drawn.
//Compile-time gated - it must not exist in a release build.
void GuiPlay::stackProbe(double ms)
{
    static int on = -1;
    if (on < 0) on = getenv("WAGIC_BOARDGROUP_PROBE") ? 1 : 0;
    if (!on) return;
    static int frames = 0;
    static double total = 0.0;
    static long drawn = 0, present = 0;
    int d = 0;
    for (iterator it = cards.begin(); it != cards.end(); ++it)
        if ((*it) && !(*it)->mStackHidden) ++d;
    drawn += d;
    present += (long) cards.size();
    total += ms;
    if (++frames >= 120)
    {
        fprintf(stderr, "#W57-G boardgroup: %d frames, %.4f ms/frame in GuiPlay::Render, "
                        "%.1f cards present, %.1f drawn, grouping=%d pinned=%d (tc=%d wait=%d stack=%d)\n",
                frames, total / frames, (double) present / frames, (double) drawn / frames,
                wagicBoardGroupingEnabled() ? 1 : 0, mStacksPinned ? 1 : 0,
                (observer && observer->getCurrentTargetChooser()) ? 1 : 0,
                (observer && observer->mLayers && observer->mLayers->actionLayer()
                    && observer->mLayers->actionLayer()->isWaitingForAnswer()) ? 1 : 0,
                (observer && observer->mLayers && observer->mLayers->stackLayer())
                    ? observer->mLayers->stackLayer()->count(0, NOT_RESOLVED) : -1);
        fflush(stderr);
        frames = 0; total = 0.0; drawn = 0; present = 0;
    }
}
#endif

//#W54-J (A25): one layout pass per frame instead of one per event. A mana
//payment or an untap step is an event STORM (every symbol, every permanent),
//and each event re-ran the four-pass Replace() over the whole battlefield -
//1.1 calls per event, 4.2 per frame in fast-clock self-play. Every event
//still marks the layout dirty (the catch-all the trailing Replace() was), the
//zone-change path still lays out synchronously before CardSelector::Add (the
//card's position is that selector's zone cue), and the disable switch
//(wagicRenderCacheOff) restores the eager call so a suspected layout defect
//is one env var to rule in or out.
void GuiPlay::relayoutIfDirty()
{
    if (mLayoutDirty)
        Replace();
}

GuiPlay::~GuiPlay()
{
    for (iterator it = cards.begin(); it != cards.end(); ++it)
    {
        delete (*it);
    }
}

bool isSpell(CardView* c)
{
    return c->card->isSpell() && !c->card->isCreature() && !c->card->hasType(Subtypes::TYPE_PLANESWALKER) && !c->card->hasType(Subtypes::TYPE_BATTLE);
}
//#W57-G (D42): a card only consumes a layout slot when it is the one that
//draws for its pile. Followers (hidden members, and the fanned members of an
//expanded pile) are positioned from their leader after the slot passes.
#define W57G_LAIDOUT(cv) (!(cv)->mStackHidden && (cv)->mStackFanIndex == 0)

void GuiPlay::Replace()
{
    mLayoutDirty = false;
    computeStacks(); //#W57-G (D42)
    unsigned opponentSpellsN = 0, selfSpellsN = 0, opponentLandsN = 0, opponentCreaturesN = 0, 
            battleFieldAttackersN = 0, battleFieldBlockersN = 0, selfCreaturesN = 0, selfLandsN = 0;

    end_spells = stable_partition(cards.begin(), cards.end(), &isSpell);

    for (iterator it = cards.begin(); it != end_spells; ++it)
        if (!(*it)->card->target && W57G_LAIDOUT(*it)) //#W57-G (D42)
        {
            if((!(*it)->card->hasSubtype(Subtypes::TYPE_AURA)|| ((*it)->card->hasSubtype(Subtypes::TYPE_AURA) && (*it)->card->playerTarget)) && !(*it)->card->hasType(Subtypes::TYPE_PLANESWALKER) && !(*it)->card->hasType(Subtypes::TYPE_BATTLE))
            {
                if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                    ++selfSpellsN;
                else
                    ++opponentSpellsN;
            }
        }
    for (iterator it = end_spells; it != cards.end(); ++it)
    {
        if (!W57G_LAIDOUT(*it)) //#W57-G (D42)
            continue;
        if ((*it)->card->isCreature())
        {
            if ((*it)->card->isAttacker())
                ++battleFieldAttackersN;
            else if ((*it)->card->isDefenser())
                ++battleFieldBlockersN;
            else if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                ++selfCreaturesN;
            else
                ++opponentCreaturesN;
        }
        else if ((*it)->card->isLand() || (*it)->card->hasType(Subtypes::TYPE_PLANESWALKER) || (*it)->card->hasType(Subtypes::TYPE_BATTLE))
        {
            if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                ++selfLandsN;
            else
                ++opponentLandsN;
        }
    }

    opponentSpells.reset(opponentSpellsN, 18, 60);
    selfSpells.reset(selfSpellsN, 18, 215);

    for (iterator it = cards.begin(); it != end_spells; ++it)
        if (!(*it)->card->target && W57G_LAIDOUT(*it)) //#W57-G (D42)
        {
            if((!(*it)->card->hasSubtype(Subtypes::TYPE_AURA)|| ((*it)->card->hasSubtype(Subtypes::TYPE_AURA) && (*it)->card->playerTarget)) && !(*it)->card->hasType(Subtypes::TYPE_PLANESWALKER) && !(*it)->card->hasType(Subtypes::TYPE_BATTLE))
            {
                if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                    selfSpells.Enstack(*it);
                else
                    opponentSpells.Enstack(*it);
            }
        }
    float x = 24 + opponentSpells.nextX();
    //seperated the variable X into 2 different variables. There are 2 players here!!
    //we should not be using a single variable to determine the positioning of cards!!
    float myx = 24 + selfSpells.nextX();
    opponentLands.reset(opponentLandsN,x, 50);
    opponentCreatures.reset(opponentCreaturesN, x, 95);
    battleField.reset(x, 145);//what does this variable do? I can comment it out with no repercussions...is this being double handled?
    selfCreatures.reset(selfCreaturesN, myx, 195);
    selfLands.reset(selfLandsN, myx, 240);

    for (iterator it = end_spells; it != cards.end(); ++it)
    {
        if (!W57G_LAIDOUT(*it)) //#W57-G (D42)
            continue;
        if ((*it)->card->isCreature())
        {
            if ((*it)->card->isAttacker())
                battleField.EnstackAttacker(*it);
            else if ((*it)->card->isDefenser())
                battleField.EnstackBlocker(*it);
            else if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                selfCreatures.Enstack(*it);
            else
                opponentCreatures.Enstack(*it);
        }
        else if ((*it)->card->isLand())
        {
            if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                selfLands.Enstack(*it);
            else
                opponentLands.Enstack(*it);
        }

    }
    //rerun the iter reattaching planes walkers to the back of the lands.
    for (iterator it = end_spells; it != cards.end(); ++it)
    {
        if (!W57G_LAIDOUT(*it)) //#W57-G (D42)
            continue;
        if (((*it)->card->hasType(Subtypes::TYPE_PLANESWALKER) || (*it)->card->hasType(Subtypes::TYPE_BATTLE)) && !(*it)->card->isCreature())
        {
            if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                selfLands.Enstack(*it);
            else
                opponentLands.Enstack(*it);
        }
    }

    //#W57-G (D42): followers take their leader's slot. A hidden member sits
    //exactly on it - the selector's directional tests need a strictly greater
    //coordinate, so a collapsed pile is one cursor stop, and a pointer click
    //on the pile resolves to the leader (closest() breaks ties in cards[]
    //order, which is the order the leader was chosen in). A fanned member
    //takes a distinct offset, so the same presses walk the members.
    for (size_t i = 0; i < mStackFollowers.size(); ++i)
    {
        CardView * f = mStackFollowers[i].first;
        CardView * lead = mStackFollowers[i].second;
        if (!f || !lead)
            continue;
        f->x = lead->x + STACKFANPITCH * f->mStackFanIndex;
        f->y = lead->y;
    }
}

void GuiPlay::Render()
{
    relayoutIfDirty(); //#W54-J (A25)
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    struct timespec t0; //#W57-G (D42)
    clock_gettime(CLOCK_MONOTONIC, &t0);
#endif
    battleField.Render();

    for (iterator it = cards.begin(); it != cards.end(); ++it)
    {
        //#W57-G (D42): a member another card stands in for is not drawn at
        //all - this is where the frame cost of a wide board goes away. Its
        //auras and equipment ride RenderSpell off the drawn member, so they
        //disappear with it and nothing is drawn twice at the same coordinates.
        if ((*it)->mStackHidden)
            continue;
        //#W57-G (D42): the pile look. Two dark plates behind the drawn card,
        //offset like a stack of physical cards, so a badge is not the only cue.
        if ((*it)->mStackCount > 1)
        {
            const int extra = ((*it)->mStackCount > 3) ? 2 : 1;
            for (int p = extra; p >= 1; --p)
                JRenderer::GetInstance()->FillRect((*it)->actX - 13.0f + 2.2f * p,
                                                   (*it)->actY - 19.0f - 2.2f * p,
                                                   27.0f, 38.0f, ARGB(150, 20, 20, 20));
        }
        //draw line when attacking planeswalker
        if((*it)->card && (*it)->card->isAttacker())
        {
            Damageable * dtarget = ((Damageable *)(*it)->card->isAttacking); 
            if(dtarget && dtarget->type_as_damageable == Damageable::DAMAGEABLE_MTGCARDINSTANCE)
            {
                MTGCardInstance * ctarget = ((MTGCardInstance *)(*it)->card->isAttacking);
                if((ctarget->hasType(Subtypes::TYPE_PLANESWALKER) || ctarget->hasType(Subtypes::TYPE_BATTLE)) && observer->isInPlay(ctarget) && observer->getCurrentGamePhase() < MTG_PHASE_COMBATEND)
                {
                    JRenderer::GetInstance()->DrawLine((*it)->actX,(*it)->actY,ctarget->view->actX,ctarget->view->actY,0.5f,ARGB(128 - wave, 255, 40, 40));
                }
            }
        }

        if ((*it)->card->isLand())
        {
            if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                selfLands.Render(*it, cards.begin(), end_spells);
            else
                opponentLands.Render(*it, cards.begin(), end_spells);
        }
        else if ((*it)->card->isCreature())
        {
            if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                selfCreatures.Render(*it, cards.begin(), end_spells);
            else
                opponentCreatures.Render(*it, cards.begin(), end_spells);
        }
        else if(!(*it)->card->hasType(Subtypes::TYPE_PLANESWALKER) && !(*it)->card->hasType(Subtypes::TYPE_BATTLE))
        {
            if (!(*it)->card->target)
            {
                if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                    selfSpells.Render(*it, cards.begin(), end_spells);
                else
                    opponentSpells.Render(*it, cards.begin(), end_spells);
            }
        }
        else
        {
            if (!(*it)->card->target)
            {
                if (mpDuelLayers->getRenderedPlayer() == (*it)->card->controller())
                    selfPlaneswalker.Render(*it, cards.begin(), end_spells);
                else
                    opponentPlaneswalker.Render(*it, cards.begin(), end_spells);
            }
        }
    }
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    {
        struct timespec t1; //#W57-G (D42)
        clock_gettime(CLOCK_MONOTONIC, &t1);
        stackProbe((t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_nsec - t0.tv_nsec) / 1000000.0);
    }
#endif
}
void GuiPlay::Update(float dt)
{
    //#W57-G (D42): the two inputs to stacking that arrive WITHOUT a game event -
    //the cursor moving into or out of a pile, and a chooser/stack window opening
    //or closing. Neither reaches receiveEventPlus, so without this the board
    //would keep the layout it had when the last game event fired: a focused pile
    //would never fan out, and targeting would never expand the board.
    {
        const void * sig = NULL;
        for (iterator it = cards.begin(); it != cards.end(); ++it)
            if ((*it) && (*it)->mHasFocus)
            {
                sig = (const void *) (*it);
                break;
            }
        const bool pinned = stacksPinnedNow();
        if (sig != mStackSig || pinned != mStacksPinned)
        {
            mStackSig = sig;
            mLayoutDirty = true;
        }
    }
    relayoutIfDirty(); //#W54-J (A25)
    battleField.Update(dt);
    for (iterator it = cards.begin(); it != cards.end(); ++it)
    {
        if((*it))
            (*it)->Update(dt);
    }
    wave = (wave + 2 * (int) (100 * dt)) % 255;
}

int GuiPlay::receiveEventPlus(WEvent * e)
{
    if (WEventZoneChange *event = dynamic_cast<WEventZoneChange*>(e))
    {
        if ((observer->players[0]->inPlay() == event->to) || (observer->players[1]->inPlay() == event->to))
        {
            CardView * card;
            if (event->card->view)
            {
                //fix for http://code.google.com/p/wagic/issues/detail?id=462.
                // We don't want a card in the hand to have an alpha of 0
                event->card->view->alpha = 255;

                card = NEW CardView(CardView::playZone, event->card, *(event->card->view));
            }
            else
                card = NEW CardView(CardView::playZone, event->card, 0, 0);
            cards.push_back(card);

            if (event->card->isTapped())
                gModRules.cards.activateEffect->doEffect(card);
            else
                gModRules.cards.activateEffect->undoEffect(card);

            card->alpha = 255;

            // Make sure that the card is repositioned before adding it to the CardSelector, as
            // the card's position is a cue for certain CardSelector variants as to what zone the card is placed in
            Replace();
            observer->getCardSelector()->Add(card);
            return 1;
        }
    }
    else if (WEventCreatureAttacker* event = dynamic_cast<WEventCreatureAttacker*>(e))
    {
        if (NULL != event->after)
            battleField.addAttacker(event->card);
        else if (NULL != event->before)
            battleField.removeAttacker(event->card);
        mLayoutDirty = true; //#W54-J (A25)
    }
    else if (dynamic_cast<WEventCreatureBlocker*> (e))
    {
        mLayoutDirty = true; //#W54-J (A25)
    }
    else if (WEventCardTap* event = dynamic_cast<WEventCardTap*>(e))
    {
        if (CardView* cv = dynamic_cast<CardView*>(event->card->view))
        {
            if (event->after)
                gModRules.cards.activateEffect->doEffect(cv);
            else
                gModRules.cards.activateEffect->undoEffect(cv);
            //cv->t = event->after ? M_PI / 2 : 0;
        }
        else if (event->card->view != NULL)
        {
            if (event->after)
                gModRules.cards.activateEffect->doEffect(event->card->view);
            else
                gModRules.cards.activateEffect->undoEffect(event->card->view);
            //event->card->view->actT = event->after ? M_PI / 2 : 0;
        }
        else
        {
            // this should never happen, if you have a consistent repro case, ping Wil please
            assert(false);
        }
        return 1;
    }
    else if (WEventPhaseChange *event = dynamic_cast<WEventPhaseChange*>(e))
    {
        if (MTG_PHASE_COMBATEND == event->to->id)
            battleField.colorFlow = -1;
    }
    else if (dynamic_cast<WEventCardChangeType*> (e))
        mLayoutDirty = true;
    else if (dynamic_cast<WEventCardUnattached*> (e))
        mLayoutDirty = true;
    else if (dynamic_cast<WEventCardEquipped*> (e))
        mLayoutDirty = true;
    else if (dynamic_cast<WEventCardControllerChange*> (e))
        mLayoutDirty = true;
    /*else if (dynamic_cast<WEventCardTransforms*> (e))
        Replace();
    else if (dynamic_cast<WEventCardCopiedACard*> (e))
        Replace();
    else if (dynamic_cast<WEventCardFaceUp*> (e))
        Replace();*/
    //#W54-J (A25): the catch-all. Every event that reaches the layer still
    //forces a relayout - it just happens once, on the next Update/Render,
    //instead of here for each of the frame's N events.
    mLayoutDirty = true;
    if (wagicRenderCacheOff())
        Replace();
    return 0;
}

int GuiPlay::receiveEventMinus(WEvent * e)
{
    if (WEventZoneChange *event = dynamic_cast<WEventZoneChange*>(e))
    {
        if ((observer->players[0]->inPlay() == event->from) || (observer->players[1]->inPlay() == event->from))
            for (iterator it = cards.begin(); it != cards.end(); ++it)
                if (event->card->previous == (*it)->card || event->card == (*it)->card)
                {
                    if (event->card->previous && event->card->previous->attacker)
                        battleField.removeAttacker(event->card->previous);
                    else if (event->card->attacker)
                        battleField.removeAttacker(event->card);
                    CardView* cv = *it;
                    observer->getCardSelector()->Remove(cv);
                    cards.erase(it);
                    observer->mTrash->trash(cv);
                    mLayoutDirty = true; //#W54-J (A25)
                    if (wagicRenderCacheOff())
                        Replace();
                    return 1;
                }
    }
    return 0;
}
