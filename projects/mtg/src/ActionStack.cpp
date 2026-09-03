/*
The Action Stack contains all information for Game Events that can be interrupted (Interruptible)
*/
#include "PrecompiledHeader.h"

#include "ActionStack.h"
#include "AIPlayer.h"
#include "LegalActions.h"
#include "CardGui.h"
#include "Damage.h"
#include "GameObserver.h"
#include "ManaCost.h"
#include "MTGAbility.h"
#include "Subtypes.h"
#include "TargetChooser.h"
#include "Translate.h"
#include "WResourceManager.h"
#include "ModRules.h"
#include "AllAbilities.h"
#include "CardSelector.h"
#include <typeinfo>
#include <chrono>

//#W54-R: the stall floor's DISABLE FLAG and its wall clock.
//WAGIC_STALL_FLOOR=0 turns the whole floor off - one env var, no rebuild, so
//"was it the watchdog?" is answerable on a shipped binary instead of by a
//build swap. (The wave-53 lane that introduced the floor shipped no flag; a
//wave-54 corpus then spent ~3.2 h of inference on answers the floor threw
//away and it could not be A/B'd.)
static bool stallFloorEnabled()
{
    static int cached = -1;
    if (cached < 0)
    {
        const char * v = getenv("WAGIC_STALL_FLOOR");
        cached = (v && (v[0] == '0') && !v[1]) ? 0 : 1;
    }
    return cached != 0;
}

//REAL elapsed time, never dt. steady_clock so a wall-clock adjustment cannot
//make a held window look ancient (or immortal).
static long long stallFloorNowMs()
{
    using namespace std::chrono;
    return (long long) duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

#ifdef VITA
#include <psp2/ctrl.h>
#endif

namespace
{
    float kGamepadIconSize = 0.5f;

    //Gamepad glyphs in iconspsp.png row 0: 0-3 d-pad, 4 CIRCLE, 5 triangle,
    //6 SQUARE, 7 CROSS.
    //
    //The fork binds Cross->OK / Circle->SEC / Square->PRI on every pad
    //platform it ships (JGE/src/main.cpp gDefaultBindings, Vitamain.cpp
    //gVitaButtonMap) - the defaults below follow that layout. But bindings
    //are RUNTIME state (Options > Controls remaps persist), so on the pad
    //platforms the prompt asks the live binding table which physical button
    //owns each action and draws THAT glyph. A hardcoded glyph told a PSP
    //player to press Cross to interrupt while Cross was bound to decline.
    const int kIconForOKDefault  = 7; //Cross
    const int kIconForSECDefault = 4; //Circle
    const int kIconForPRIDefault = 6; //Square

    int iconForBoundKey(JButton btn, int fallback)
    {
#if defined(PSP) || defined(VITA)
        JGE* engine = JGE::GetInstance();
        if (engine)
            for (JGE::keybindings_it it = engine->KeyBindings_begin(); it != engine->KeyBindings_end(); ++it)
            {
                if (it->second != btn)
                    continue;
                switch (it->first)
                {
#if defined(PSP)
                case PSP_CTRL_CIRCLE:   return 4;
                case PSP_CTRL_TRIANGLE: return 5;
                case PSP_CTRL_SQUARE:   return 6;
                case PSP_CTRL_CROSS:    return 7;
#else
                case SCE_CTRL_CIRCLE:   return 4;
                case SCE_CTRL_TRIANGLE: return 5;
                case SCE_CTRL_SQUARE:   return 6;
                case SCE_CTRL_CROSS:    return 7;
#endif
                default:
                    break; //bound to a key with no face-button glyph - keep looking
                }
            }
#endif
        return fallback;
    }

    std::string kInterruptMessageString("Interrupt?");
    std::string kInterruptString(": Interrupt");
    std::string kNoString(": No");
    std::string kNoToAllString(": No To All");
    static const float kIconVerticalOffset = 24;

#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    //WAGIC_PADLOG (Android: flag file User/padlog.on): interrupt-offer slice
    //of the input trace - same gate as the SimplePad/SDLmain halves.
    FILE * jgePadlogFile()
    {
        static FILE * out = NULL;
        static int state = 0;
        if (state == 0)
        {
            if (getenv("WAGIC_PADLOG"))
                { out = stderr; state = 1; }
#ifdef ANDROID
            else if (access("/sdcard/Wagic/User/padlog.on", F_OK) == 0)
                { out = fopen("/sdcard/Wagic/User/padlog.txt", "a"); state = out ? 1 : -1; }
#endif
            else state = -1;
        }
        return (state == 1) ? out : NULL;
    }
#else
    inline FILE * jgePadlogFile() { return NULL; }
#endif //_DEBUG || WAGIC_DEVLOGS

}

/*
NextGamePhase requested by user
*/
int NextGamePhase::resolve()
{
    //W36 lane-B item 6 (B-vs-105 t15): a phase-advance queued while the
    //defender's blockers decision was MASKED by an ability resolving on the
    //stack used to ride past the blockers step when it resolved - the
    //deferred, engine-issued declaration never got another settled tick, and
    //a real, lethal-preventing block window vanished with no ask and no
    //translog record. pendingCombatDecision's stack gate promises "deferred,
    //not dropped"; honoring that promise here means a queued pass yields to a
    //still-owed AI declaration (the declaring seat re-drives the flow and
    //advances the phase itself once committed). Scoped to AI defenders whose
    //declaration is genuinely outstanding (blockersDeclarationDue - the async
    //GPT seat; heuristic seats declare synchronously and never report due),
    //so a HUMAN defender's deliberate decline-by-advance is untouched.
    //The gates mirror pendingCombatDecision's, EXCEPT the stack gate: while
    //resolve() runs, THIS pass is itself still NOT_RESOLVED on the stack
    //(ActionStack::resolve sets the state after the call), so the oracle's
    //stack check would read the pass as unfinished business and never let the
    //hold fire. "Settled" here means nothing is pending BUT this pass.
    if (observer->getCurrentGamePhase() == MTG_PHASE_COMBATBLOCKERS
        && observer->combatStep == BLOCKERS && observer->currentPlayer)
    {
        Player * defender = observer->currentPlayer->opponent();
        AIPlayer * aiDefender = (defender && defender->isAI()) ? dynamic_cast<AIPlayer *>(defender) : NULL;
        ActionStack * stack = observer->mLayers->stackLayer();
        if (aiDefender && aiDefender->blockersDeclarationDue()
            && stack->count(0, NOT_RESOLVED) <= 1 //only this pass itself
            && !observer->mLayers->actionLayer()->menuObject
            && !observer->getCurrentTargetChooser()
            && !observer->mExtraPayment
            && observer->currentPlayer->game->inPlay->getNextAttacker(NULL)
            && LegalActionsOracle::hasLegalBlock(defender))
        {
            DebugTrace("NextGamePhase: held - defender's blockers declaration still due");
            return 1;
        }
    }
    observer->userRequestNextGamePhase(false, false);
    return 1;
}

const string NextGamePhase::getDisplayName() const
{
    std::ostringstream stream;
    stream << "NextGamePhase.  (Current phase is: " << observer->getCurrentGamePhaseName() << ")";

    return stream.str();
}

void NextGamePhase::Render()
{
    WFont * mFont = observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT);
    mFont->SetBase(0);
    mFont->SetScale(1.3f);
    char buffer[200];
    int playerId = 1;
    if (observer->currentActionPlayer == observer->players[1])
        playerId = 2;

    snprintf(buffer, sizeof(buffer), "%s %i :  %s", _("Player").c_str(), playerId, observer->getNextGamePhaseName().c_str());

    mFont->DrawString(buffer, x + 15, y+10, JGETEXT_LEFT);
    mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
}

NextGamePhase::NextGamePhase(GameObserver* observer, int id) :
Interruptible(observer, id)
{
    mHeight = 40;
    type = ACTION_NEXTGAMEPHASE;
}

ostream& NextGamePhase::toString(ostream& out) const
{
    out << "NextGamePhase ::: ";
    return out;
}

const string Interruptible::getDisplayName() const
{
    return typeid(*this).name();
}

float Interruptible::GetVerticalTextOffset() const
{
    static const float kTextVerticalOffset = (mHeight - observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT)->GetHeight()) / 2;
    return kTextVerticalOffset;
}

void Interruptible::Render(MTGCardInstance * source, JQuad * targetQuad, string alt1, string alt2, string action,
    bool bigQuad, int aType, vector<JQuadPtr> mytargetsQuad, vector<int> mytargetCounts)
{
    WFont * mFont = observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT);
    mFont->SetColor(ARGB(255,255,255,255));
    mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
    JRenderer * renderer = JRenderer::GetInstance();
    bool hiddenview = aType == MTGAbility::HIDDENVIEW?true:false;

    if (!targetQuad && !mytargetsQuad.size())
    {
        /*if(source->controller()->isHuman() && source->controller()->opponent()->isAI() && !alt2.size() && _(action).c_str() == source->name)
            mFont->DrawString("You play ", x + 35, y-15 + GetVerticalTextOffset(), JGETEXT_LEFT);
        else if(source->controller()->isAI() && source->controller()->opponent()->isHuman() && !alt2.size() && _(action).c_str() == source->name)
            mFont->DrawString("Opponent plays ", x + 35, y-15 + GetVerticalTextOffset(), JGETEXT_LEFT);*/
        mFont->DrawString(_(action).c_str(), x + 35, y + GetVerticalTextOffset(), JGETEXT_LEFT);
    }
    else
    {
        /*if(source->controller()->isHuman() && source->controller()->opponent()->isAI())
            renderer->DrawRect(x-2,y-16 + GetVerticalTextOffset(), 73, 43, ARGB(245,0,255,0));
        else
            renderer->DrawRect(x-2,y-16 + GetVerticalTextOffset(), 73, 43, ARGB(245,255,0,0));*/
        float xnadj = 0;
        int count = 1;
        if(mytargetsQuad.size())
        {
            //#W57-G (D42): the "(N)" in the header stays the TRUE number of
            //targets even when the icons were collapsed - a Fireball split over
            //3 copies of one token is three targets, and saying "(1)" would be
            //the kind of true-in-the-wrong-scope statement the trust doctrine
            //forbids.
            count = 0;
            for(unsigned int k = 0; k < mytargetsQuad.size(); k++)
                count += (k < mytargetCounts.size()) ? mytargetCounts[k] : 1;
            if (!count)
                count = (int) mytargetsQuad.size();
            for(unsigned int k = 0; k < mytargetsQuad.size(); k++)
            {
                if(k > 10)
                    break;
                xnadj+=4;
            }
        }

        ostringstream aa;
        aa << action << " " << "(" << count << ")";

        if(count > 1)
            xnadj -= 4;

        if(!hiddenview)
        {
            mFont->DrawString(">", x + 32, y + GetVerticalTextOffset(), JGETEXT_LEFT);
            if(count > 1)
            {
                mFont->DrawString(_(aa.str()).c_str(), x + 75 + xnadj, y + GetVerticalTextOffset(), JGETEXT_LEFT);
            }
            else
                mFont->DrawString(_(action).c_str(), x + 75 + xnadj, y + GetVerticalTextOffset(), JGETEXT_LEFT);
        }
        else
            mFont->DrawString(_(action).c_str(), x + 35, y + GetVerticalTextOffset(), JGETEXT_LEFT);

    }

    JQuadPtr quad = observer->getResourceManager()->RetrieveCard(source, CACHE_THUMB);
    JQuadPtr fakeborder = observer->getResourceManager()->GetQuad("white");
    if (!quad.get())
        quad = CardGui::AlternateThumbQuad(source);
    if (quad.get())
    {
        quad->SetColor(ARGB(255,255,255,255));
        float scale = mHeight / quad->mHeight;
        if (fakeborder.get())
        {
            fakeborder->SetColor(ARGB(255,15,15,15));
            renderer->RenderQuad(fakeborder.get(), x + (quad->mWidth * scale / 2), y + (quad->mHeight * scale / 2), 0, (29 * actZ + 1) / 16, 42 * actZ / 16);
        }
        
        renderer->RenderQuad(quad.get(), x + (quad->mWidth * scale / 2), y + (quad->mHeight * scale / 2), 0, scale, scale);
    }
    else if (alt1.size())
    {
        mFont->DrawString(_(alt1).c_str(), x, y + GetVerticalTextOffset());
    }

    if (bigQuad)
    {
        /*Pos pos = Pos(CardGui::BigWidth / 2, CardGui::BigHeight / 2 - 10, 1.0, 0.0, 220);
        CardGui::DrawCard(source, pos, observer->getCardSelector()->GetDrawMode());*/
        if(observer->gameType() == GAME_TYPE_MOMIR && aType == MTGAbility::FORCED_TOKEN_CREATOR)
        {
            Pos pos = Pos(CardGui::BigWidth / 2, CardGui::BigHeight / 2 - 10, 0.80f, 0.0, 220);
            pos.actY = 142;//adjust y a little bit
            CardGui::DrawCard(source, pos, observer->getCardSelector()->GetDrawMode());
        }
        else if (observer->gameType() != GAME_TYPE_MOMIR)
        {
            Pos pos = Pos(CardGui::BigWidth / 2, CardGui::BigHeight / 2 - 10, 0.80f, 0.0, 220);
            pos.actY = 142;//adjust y a little bit
            CardGui::DrawCard(source, pos, observer->getCardSelector()->GetDrawMode());
        }

    }

    if(mytargetsQuad.size() && !hiddenview)
    {
        float xadj = 0;
        for(unsigned int k = 0; k < mytargetsQuad.size(); k++)
        {
            if(k > 10)
                break;

            JQuadPtr multiQ = mytargetsQuad[k];
            if(multiQ.get())
            {
                float backupX = multiQ->mHotSpotX;
                float backupY = multiQ->mHotSpotY;
                multiQ->SetColor(ARGB(255,255,255,255));
                multiQ->SetHotSpot(multiQ->mWidth / 2, multiQ->mHeight / 2);
                float scale = mHeight / multiQ->mHeight;
                if (fakeborder.get())
                {
                    fakeborder->SetColor(ARGB(255,15,15,15));
                    renderer->RenderQuad(fakeborder.get(), x + 55 + xadj, y + ((mHeight - multiQ->mHeight) / 2) + multiQ->mHotSpotY, 0, (29 * actZ + 1) / 16, 42 * actZ / 16);
                }
                renderer->RenderQuad(multiQ.get(), x + 55 + xadj, y + ((mHeight - multiQ->mHeight) / 2) + multiQ->mHotSpotY, 0, scale, scale);
                multiQ->SetHotSpot(backupX, backupY);
                //#W57-G (D42): xN on a collapsed run of identical targets.
                if (k < mytargetCounts.size() && mytargetCounts[k] > 1)
                {
                    char cbuf[16];
                    sprintf(cbuf, "x%i", mytargetCounts[k]);
                    mFont->SetColor(ARGB(255,255,235,140));
                    mFont->DrawString(cbuf, x + 55 + xadj, y + mHeight - 8 + GetVerticalTextOffset(), JGETEXT_LEFT);
                    mFont->SetColor(ARGB(255,255,255,255));
                }
                xadj+=4;
            }
        }
    }
    else if(!hiddenview)
    {
        if (targetQuad)
        {
            float backupX = targetQuad->mHotSpotX;
            float backupY = targetQuad->mHotSpotY;
            targetQuad->SetColor(ARGB(255,255,255,255));
            targetQuad->SetHotSpot(targetQuad->mWidth / 2, targetQuad->mHeight / 2);
            float scale = mHeight / targetQuad->mHeight;
            renderer->RenderQuad(targetQuad, x + 55, y + ((mHeight - targetQuad->mHeight) / 2) + targetQuad->mHotSpotY, 0, scale, scale);
            targetQuad->SetHotSpot(backupX, backupY);
        }
        else if (alt2.size())
        {
            mFont->DrawString(_(alt2).c_str(), x + 35, y+15 + GetVerticalTextOffset());
        }
    }
}

//#W57-G (D42): collapse a stack entry's target icons the way the board
//collapses the permanents themselves. Members of one pile are, by the stack
//key's own definition, indistinguishable - name and every observable state are
//equal - so N identical thumbnails carry no more information than one
//thumbnail and the number N, and the row is 480 logical pixels wide.
//Entries that are not battlefield cards (players, spells, library cards) never
//merge: they get a unique key. Order is preserved; the first occurrence keeps
//the slot.
static void w57gCollapseTargetIcons(vector<JQuadPtr>& quads, vector<MTGCardInstance*>& cards,
                                    vector<int>& counts)
{
    counts.assign(quads.size(), 1);
    if (!wagicBoardGroupingEnabled() || quads.size() < 2)
        return;
    vector<JQuadPtr> outQ;
    vector<int> outC;
    vector<string> outK;
    for (size_t i = 0; i < quads.size(); ++i)
    {
        string key;
        if (i < cards.size() && cards[i] && cards[i]->isInPlay(cards[i]->getObserver()))
            key = wagicBoardStackKey(cards[i]);
        if (key.size())
        {
            size_t at = outK.size();
            for (size_t j = 0; j < outK.size(); ++j)
                if (outK[j] == key) { at = j; break; }
            if (at < outK.size()) { outC[at]++; continue; }
        }
        outQ.push_back(quads[i]);
        outC.push_back(1);
        outK.push_back(key);
    }
    quads = outQ;
    counts = outC;
}

/* Ability */
int StackAbility::resolve()
{
    return (ability->resolve());
}
void StackAbility::Render()
{
    string action = ability->getMenuText();
    MTGCardInstance * source = ability->source;
    string alt1 = source->getName();
    vector<JQuadPtr> mytargetQuads;
    vector<MTGCardInstance*> myClones;
    vector<MTGCardInstance*> myQuadCards; //#W57-G (D42): parallel to mytargetQuads
    vector<int> myTargetCounts;           //#W57-G (D42)

    int fmLibrary = 0;
    int force = 0;

    Targetable * _target = ability->target;
    if (ability->getActionTc())
    {
        Targetable * t = ability->getActionTc()->getNextTarget();
        if (t)
            _target = t;

        
    //test vector quads
        if(ability->getActionTc()->getTargetsFrom().size())
        {
            for(size_t i = 0; i < ability->getActionTc()->getTargetsFrom().size(); i++)
            {
                Targetable * tt = ability->getActionTc()->getTargetsFrom()[i];
                if(tt)
                {
                    MTGCardInstance* card = dynamic_cast<MTGCardInstance *>(tt);
                    if(!card && dynamic_cast<Spell *>(tt)) card = dynamic_cast<Spell *>(tt)->source; //Fixed crash on targeting a spell on stack by correctly casting variable tt.
                    if(((Damageable *)(tt))->type_as_damageable == Damageable::DAMAGEABLE_MTGCARDINSTANCE)
                    {
                        if(card)
                            myClones.push_back(card); //fill vector
                        if(source->has(Constants::HIDDENFACE) && card && !observer->isInLibrary(card))
                        {
                            mytargetQuads.push_back(card->getIcon()); //Fixed crash on targeting a spell on stack by correctly casting variable tt.
                            myQuadCards.push_back(card); //#W57-G (D42)
                        }
                        else if (!source->has(Constants::HIDDENFACE) && card)
                        {
                            mytargetQuads.push_back(card->getIcon()); //Fixed crash on targeting a spell on stack by correctly casting variable tt.
                            myQuadCards.push_back(card); //#W57-G (D42)
                        }
                        else
                            fmLibrary++;
                    }
                    else if(card)
                    {
                        mytargetQuads.push_back(card->getIcon()); //Fixed crash on targeting a spell on stack by correctly casting variable tt.
                        myQuadCards.push_back(card); //#W57-G (D42)
                    }
                }
            }
        }
    //end
    }
    Damageable * target = NULL;
    if (_target != ability->source && (dynamic_cast<MTGCardInstance *>(_target) || dynamic_cast<Player *>(_target)))
    {
        target = (Damageable *) _target;
    }

    JQuadPtr quad;
    string alt2 = "";
    if (target)
    {
        quad = target->getIcon();
        if (target->type_as_damageable == Damageable::DAMAGEABLE_MTGCARDINSTANCE)
        {
            alt2 = ((MTGCardInstance *) target)->name;
        }
    }

    //setborder test
    if(myClones.size())
    {
        source->forcedBorderB = 1;
        for(unsigned int kk = 0; kk < myClones.size(); kk++)
        {
            if(myClones[kk])
            {
                myClones[kk]->forcedBorderA = 1;
                //JRenderer::GetInstance()->DrawLine(myClones[kk]->view->actX,myClones[kk]->view->actY,source->view->actX,source->view->actY,0.5f,ARGB(120, 255, 0, 0));
            }
        }
    }

    if(source->has(Constants::HIDDENFACE) && fmLibrary)
        force = MTGAbility::HIDDENVIEW;

    w57gCollapseTargetIcons(mytargetQuads, myQuadCards, myTargetCounts); //#W57-G (D42)

    if(observer->gameType() == GAME_TYPE_MOMIR)
        Interruptible::Render(source, quad.get(), alt1, alt2, action, true, ability->aType, mytargetQuads, myTargetCounts);
    else
        Interruptible::Render(source, quad.get(), alt1, alt2, action, false, force, mytargetQuads, myTargetCounts);
}
StackAbility::StackAbility(GameObserver* observer, int id, MTGAbility * _ability) :
Interruptible(observer, id), ability(_ability)
{
    type = ACTION_ABILITY;
}

ostream& StackAbility::toString(ostream& out) const
{
    out << "StackAbility ::: ability : " << ability;
    return out;
}

const string StackAbility::getDisplayName() const
{
    std::ostringstream stream;
    if(ability->source)
    stream << "StackAbility.  (Source: " << ability->source->getDisplayName() << ")";
    else
    stream << "StackAbility.  (Source: " << ability->getMenuText() << ")";

    return stream.str();
}

/* Spell Cast */

Spell::Spell(GameObserver* observer, MTGCardInstance * _source) :
Interruptible(observer, 0)
{
    source = _source;
    mHeight = 40;
    type = ACTION_SPELL;
    cost = NEW ManaCost();
    cost->extraCosts = NULL;
    tc = NULL;
    from = _source->getCurrentZone();
    payResult = ManaCost::MANA_UNPAID;
    source->castMethod = Constants::NOT_CAST;
}

Spell::Spell(GameObserver* observer, int id, MTGCardInstance * _source, TargetChooser * tc, ManaCost * _cost, int payResult) :
Interruptible(observer, id), tc(tc), cost(_cost), payResult(payResult)
{
    if (!cost)
    {
        cost = NEW ManaCost();
        cost->extraCosts = NULL;
    }
    source = _source;
    mHeight = 40;
    type = ACTION_SPELL;
    from = _source->getCurrentZone();

    _source->backupTargets.clear();
    if (tc)
    {
        Targetable* t = NULL;
        for(size_t i = 0;i < tc->getNbTargets();i++)
        {
            t = tc->getNextTarget(t);
            _source->backupTargets.push_back(t);
        }
    }

    // fill information on how the card came into this zone. Right now the quickest way is to do it here, based on how the mana was paid...
    switch(payResult) {
        case ManaCost::MANA_UNPAID:
            source->castMethod = Constants::NOT_CAST;
            break;
        case ManaCost::MANA_PAID:
        case ManaCost::MANA_PAID_WITH_KICKER:
            source->castMethod = Constants::CAST_NORMALLY;
            break;
        default:
            source->castMethod = Constants::CAST_ALTERNATE;
            break;
    }
}

int Spell::computeX(MTGCardInstance * card)
{
    ManaCost * c = cost->Diff(card->getManaCost());
    int x = c->getCost(Constants::NB_Colors);
    delete c;
    return x;
}

bool Spell::FullfilledAlternateCost(const int &costType)
{
    bool hasFullfilledAlternateCost = false;

    switch (costType)
    {
    case ManaCost::MANA_UNPAID:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_UNPAID);
        break;
    case ManaCost::MANA_PAID:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID);
        break;
    case ManaCost::MANA_PAID_WITH_KICKER:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID_WITH_KICKER);
        break;
    case ManaCost::MANA_PAID_WITH_ALTERNATIVE:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID_WITH_ALTERNATIVE);
        break;
    case ManaCost::MANA_PAID_WITH_BUYBACK:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID_WITH_BUYBACK);
        break;
    case ManaCost::MANA_PAID_WITH_FLASHBACK:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID_WITH_FLASHBACK);
        break;
    case ManaCost::MANA_PAID_WITH_RETRACE:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID_WITH_RETRACE);
        break;
    case ManaCost::MANA_PAID_WITH_SUSPEND:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID_WITH_SUSPEND);
        break;
    case ManaCost::MANA_PAID_WITH_OVERLOAD:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID_WITH_OVERLOAD);
        break;
    case ManaCost::MANA_PAID_WITH_BESTOW:
        hasFullfilledAlternateCost = (payResult == ManaCost::MANA_PAID_WITH_BESTOW);
        break;
    }

    return hasFullfilledAlternateCost;
}

const string Spell::getDisplayName() const
{
    return source->getName();
}
Spell::~Spell()
{
    SAFE_DELETE(cost);
    SAFE_DELETE(tc);
}

int Spell::resolve()
{
    MTGCardInstance * oldStored = source->storedCard;
    Player * playerT = source->playerTarget;
    if (!source->hasType(Subtypes::TYPE_INSTANT) && !source->hasType(Subtypes::TYPE_SORCERY) && source->name.size())
    {
        Player * p = source->controller();
        int castMethod = source->castMethod;
        vector<Targetable*>backupTgt = source->backupTargets;
        if(from != source->currentZone)
        {
            from = source->currentZone;//this happens when casting spells that belong to another player or casting a copy of someone elses spell.
        }
        source = p->game->putInZone(source, from, p->game->battlefield);
        
        // We need to get the information about the cast method on both the card in the stack AND the card in play,
        //so we copy it from the previous card (in the stack) to the new one (in play).
        source->castMethod = castMethod; 
        source->backupTargets = backupTgt;
        from = p->game->battlefield;
    }
    source->playerTarget = playerT;
    source->storedCard = oldStored;
    //Play SFX
    if (options[Options::SFXVOLUME].number > 0)
    {

        if(observer->getResourceManager())
            observer->getResourceManager()->PlaySample(source->getSample());
    }
    if(this->cost && !source->getManaCost()->getManaUsedToCast())
    {
        source->getManaCost()->setManaUsedToCast(NEW ManaCost(this->cost));
    }
    AbilityFactory af(observer);
    af.addAbilities(observer->mLayers->actionLayer()->getMaxId(), this);
    return 1;
}

MTGCardInstance * Spell::getNextCardTarget(MTGCardInstance * previous)
{
    if (!tc)
        return NULL;
    return tc->getNextCardTarget(previous);
}
Player * Spell::getNextPlayerTarget(Player * previous)
{
    if (!tc)
        return NULL;
    return tc->getNextPlayerTarget(previous);
}
Damageable * Spell::getNextDamageableTarget(Damageable * previous)
{
    if (!tc)
        return NULL;
    return tc->getNextDamageableTarget(previous);
}
Interruptible * Spell::getNextInterruptible(Interruptible * previous, int type)
{
    if (!tc)
        return NULL;
    return tc->getNextInterruptible(previous, type);
}
Spell * Spell::getNextSpellTarget(Spell * previous)
{
    if (!tc)
        return NULL;
    return tc->getNextSpellTarget(previous);
}
Damage * Spell::getNextDamageTarget(Damage * previous)
{
    if (!tc)
        return NULL;
    return tc->getNextDamageTarget(previous);
}
Targetable * Spell::getNextTarget(Targetable * previous)
{
    if (!tc)
        return NULL;
    return tc->getNextTarget(previous);
}

int Spell::getNbTargets()
{
    if (!tc)
        return 0;
    return (int) (tc->getNbTargets());
}

void Spell::Render()
{
    string action = source->getName();
    string alt1 = "";

    string alt2 = "";
    Damageable * target = getNextDamageableTarget();
    JQuadPtr quad;
    if (target)
    {
        quad = target->getIcon();
        if (target->type_as_damageable == Damageable::DAMAGEABLE_MTGCARDINSTANCE)
        {
            alt2 = ((MTGCardInstance *) target)->name;
        }
    }

    //#W57-G (D42): the per-card BOARD marker for a spell's targets. Wagic
    //already draws one for a multi-target ABILITY (StackAbility sets
    //forcedBorderA on each of its targets, and CardGui renders that as the red
    //rim), but a SPELL on the stack set nothing - the owner's Fireball case,
    //where the whole question is "which of these copies did I aim at". The
    //answer was only in the stack entry's thumbnails. This closes the gap with
    //the language the game already speaks: red rim on every current target,
    //green rim on the source, cleared by GameObserver's existing sweep the
    //moment the stack empties. It is independent of the grouping option - a
    //pre-existing gap, fixed for everyone.
    vector<JQuadPtr> mytargetQuads;
    vector<MTGCardInstance*> myQuadCards;
    vector<int> myTargetCounts;
    if (tc)
    {
        vector<Targetable*> tfrom = tc->getTargetsFrom();
        for (size_t i = 0; i < tfrom.size(); ++i)
        {
            MTGCardInstance * c = dynamic_cast<MTGCardInstance *> (tfrom[i]);
            if (!c)
                continue;
            c->forcedBorderA = 1;
            myQuadCards.push_back(c);
            mytargetQuads.push_back(c->getIcon());
        }
        if (myQuadCards.size())
            source->forcedBorderB = 1;
    }
    //Only take the multi-icon path when there IS more than one target: a
    //single-target spell keeps the exact stack row it has always drawn.
    if (mytargetQuads.size() > 1)
    {
        w57gCollapseTargetIcons(mytargetQuads, myQuadCards, myTargetCounts);
        Interruptible::Render(source, quad.get(), alt1, alt2, action, true, 0, mytargetQuads, myTargetCounts);
        return;
    }
    Interruptible::Render(source, quad.get(), alt1, alt2, action, true);
}

ostream& Spell::toString(ostream& out) const
{
    out << "Spell ::: cost : " << cost;
    return out;
}

/* Put a card in graveyard */

PutInGraveyard::PutInGraveyard(GameObserver* observer, int id, MTGCardInstance * _card) :
Interruptible(observer, id)
{
    card = _card;
    removeFromGame = 0;
    type = ACTION_PUTINGRAVEYARD;
}

int PutInGraveyard::resolve()
{
    MTGGameZone * zone = card->getCurrentZone();
    if (card->basicAbilities[(int)Constants::EXILEDEATH] || card->basicAbilities[(int)Constants::GAINEDEXILEDEATH] || (card->basicAbilities[(int)Constants::HASDISTURB] && card->alternateCostPaid[ManaCost::MANA_PAID_WITH_RETRACE] == 1))
    {
        card->basicAbilities[(int)Constants::GAINEDEXILEDEATH] = 0;
        card->controller()->game->putInZone(card, zone, card->owner->game->exile);
        return 1;
    }
    if (card->basicAbilities[(int)Constants::DOUBLEFACEDEATH] || card->basicAbilities[(int)Constants::GAINEDDOUBLEFACEDEATH])
    {
        card->basicAbilities[(int)Constants::GAINEDDOUBLEFACEDEATH] = 0;
        card->controller()->game->putInZone(card, zone, card->owner->game->temp);
        return 1;
    }
    if (card->basicAbilities[(int)Constants::HANDDEATH] || card->basicAbilities[(int)Constants::GAINEDHANDDEATH])
    {
        card->basicAbilities[(int)Constants::GAINEDHANDDEATH] = 0;
        card->controller()->game->putInZone(card, zone, card->owner->game->hand);
        return 1;
    }
    if (card->basicAbilities[(int)Constants::INPLAYDEATH] || card->basicAbilities[(int)Constants::INPLAYTAPDEATH])
    {
        bool toTap = card->basicAbilities[(int)Constants::INPLAYTAPDEATH];
        bool addCounter = card->basicAbilities[(int)Constants::COUNTERDEATH];
        card = card->controller()->game->putInZone(card, zone, card->owner->game->graveyard);
        card = card->controller()->game->putInZone(card, card->owner->game->graveyard, card->owner->game->battlefield);
        if(toTap)
            card->tap(true);
        if(addCounter)
            card->counters->addCounter(1, 1, false);
        return 1;
    }
    if (zone == observer->players[0]->game->inPlay || zone == observer->players[1]->game->inPlay)
    {
        card->controller()->game->putInZone(card, zone, card->owner->game->graveyard);
        return 1;
    }
    return 0;
}

void PutInGraveyard::Render()
{
    WFont * mFont = observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT);
    mFont->SetBase(0);
    mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
    if (!removeFromGame)
    {
        mFont->DrawString(_("goes to graveyard").c_str(), x + 30, y, JGETEXT_LEFT);
    }
    else
    {
        mFont->DrawString(_("is exiled").c_str(), x + 30, y, JGETEXT_LEFT);
    }
    JRenderer * renderer = JRenderer::GetInstance();
    JQuadPtr quad = observer->getResourceManager()->RetrieveCard(card, CACHE_THUMB);
    if (quad.get())
    {
        quad->SetColor(ARGB(255,255,255,255));
        float scale = 30 / quad->mHeight;
        renderer->RenderQuad(quad.get(), x, y, 0, scale, scale);
    }
    else
    {
        mFont->DrawString(_(card->name).c_str(), x, y - 15);
    }
}

ostream& PutInGraveyard::toString(ostream& out) const
{
    out << "PutInGraveyard ::: removeFromGame : " << removeFromGame;
    return out;
}

/* Draw a Card */
DrawAction::DrawAction(GameObserver* observer, int id, Player * _player, int _nbcards) :
Interruptible(observer, id), nbcards(_nbcards), player(_player)
{
}

int DrawAction::resolve()
{
    for (int i = 0; i < nbcards; i++)
    {
            player->game->drawFromLibrary();
    }
    return 1;
}

void DrawAction::Render()
{
    WFont * mFont = observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT);
    mFont->SetBase(0);
    mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
    char buffer[200];
    int playerId = 1;
    if (player == observer->players[1])
        playerId = 2;
    snprintf(buffer, sizeof(buffer), _("Player %i draws %i card").c_str(), playerId, nbcards);
    mFont->DrawString(buffer, x + 35, y + GetVerticalTextOffset(), JGETEXT_LEFT);
}

ostream& DrawAction::toString(ostream& out) const
{
    out << "DrawAction ::: nbcards : " << nbcards << " ; player : " << player;
    return out;
}
//////
LifeAction::LifeAction(GameObserver* observer, int id, Damageable * _target, int amount) :
Interruptible(observer, id), amount(amount),target(_target)
{
}

int LifeAction::resolve()
{
target->life += amount;
    return 1;
}

void LifeAction::Render()
{
    WFont * mFont = observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT);
    mFont->SetBase(0);
    mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
    char buffer[200];
    if(amount > 0)
        snprintf(buffer, sizeof(buffer), _("Player gains %i life").c_str(), amount);
    else if(amount < 0)
        snprintf(buffer, sizeof(buffer), _("Player loses %i life").c_str(), amount);
    else
        snprintf(buffer, sizeof(buffer), "%s", _("Nothing happened").c_str());
    mFont->DrawString(buffer, x + 20, y, JGETEXT_LEFT);
}

ostream& LifeAction::toString(ostream& out) const
{
    out << "LifeAction ::: amount : " << amount << " ; target : " << target;
    return out;
}
/* The Action Stack itself */

int ActionStack::addPutInGraveyard(MTGCardInstance * card)
{
    PutInGraveyard * death = NEW PutInGraveyard(observer, mObjects.size(), card);
    addAction(death);
    return 1;
}

int ActionStack::addAbility(MTGAbility * ability)
{
    StackAbility * stackAbility = NEW StackAbility(observer, mObjects.size(), ability);
    int result = addAction(stackAbility);
    //Priority: no push-time suppression - whether each player is offered a
    //window is decided at offer time by wouldOfferWindow() (auto-pass with
    //stops), from a NOT_DECIDED start.
    return result;
}

int ActionStack::addDraw(Player * player, int nb_cards)
{
    DrawAction * draw = NEW DrawAction(observer, mObjects.size(), player, nb_cards);
    addAction(draw);
    return 1;
}

int ActionStack::addLife(Damageable * _target, int amount)
{
    LifeAction * life = NEW LifeAction(observer, mObjects.size(), _target, amount);
    addAction(life);
    return 1;
}

int ActionStack::addDamage(MTGCardInstance * _source, Damageable * _target, int _damage, bool noTrigger)
{
    Damage * damage = NEW Damage(observer, _source, _target, _damage);
    damage->noTrigger = noTrigger;
    addAction(damage);
    _source->thatmuch = _damage;
    _target->thatmuch = _damage;
    return 1;
}

int ActionStack::AddNextGamePhase()
{
    if (getNext(NULL, NOT_RESOLVED))
        return 0;

    NextGamePhase * next = NEW NextGamePhase(observer, mObjects.size());
    addAction(next);
    int playerId = (observer->currentActionPlayer == observer->players[1]) ? 1 : 0;
    interruptDecision[playerId] = DONT_INTERRUPT;
    mPriorityOn = next; //bound at push: the requester's pass IS the request
    return 1;
}

int ActionStack::AddNextCombatStep()
{
    if (getNext(NULL, NOT_RESOLVED))
        return 0;

    NextGamePhase * next = NEW NextGamePhase(observer, mObjects.size());
    addAction(next);
    return 1;
}

int ActionStack::setIsInterrupting(Player * player, bool log)
{
    askIfWishesToInterrupt = NULL;

    if (!gModRules.game.canInterrupt())
    {
        cancelInterruptOffer(DONT_INTERRUPT, log);
        return 0;
    }

     //Is it a valid interruption request, or is uninterruptible stuff going on in the game?
    if (observer->getCurrentTargetChooser())
    {
        DebugTrace("ActionStack: WARNING - We were asked to interrupt, During Targetchoosing" << endl
            << "source: " << (observer->getCurrentTargetChooser()->source ? observer->getCurrentTargetChooser()->source->name : "None" ) << endl );
        return 0;
    }

    int playerId = (player == observer->players[1]) ? 1 : 0;
    interruptDecision[playerId] = INTERRUPT;
    observer->isInterrupting = player;
    if(log)
        observer->logAction(player, "yes");
    return 1;
}

int ActionStack::addAction(Interruptible * action)
{
    for (int i = 0; i < 2; i++)
    {
        interruptDecision[i] = NOT_DECIDED;
    }
    Add(action);
    observer->bumpAbilityEpoch(); //#W54-H (A6b)
    lastActionController = observer->currentlyActing();
    DebugTrace("Action added to stack: " << action->getDisplayName());

    return 1;
}

Spell * ActionStack::addSpell(MTGCardInstance * _source, TargetChooser * tc, ManaCost * mana, int payResult,
    int storm, bool forcedinterrupt)
{
    DebugTrace("ACTIONSTACK Add spell");
    if (storm > 0)
    {
        mana = NULL;
    }
    Spell * spell = NEW Spell(observer, mObjects.size(), _source, tc, mana, payResult);
    addAction(spell);
    //Priority: push-time suppression removed (offer-time auto-pass decides);
    //forcedinterrupt still pre-commits the human to respond.
    if (forcedinterrupt && !observer->players[0]->isAI() && _source->controller() == observer->players[0])
        interruptDecision[0] = INTERRUPT;
    return spell;
}

Interruptible * ActionStack::getAt(int id)
{
    if (id < 0)
        id = mObjects.size() + id;
    if (id > (int)(mObjects.size()) - 1 || id < 0)
        return NULL;
    return (Interruptible *) mObjects[id];
}

ActionStack::ActionStack(GameObserver* game)
    : GuiLayer(game), currentTutorial(0)
{
    mPriorityOn = NULL;
    for (int i = 0; i < 2; i++)
        interruptDecision[i] = NOT_DECIDED;
    askIfWishesToInterrupt = NULL;
    mHoldOn = NULL;
    mHoldWho = NULL;
    mHoldTicks = 0;
    mHoldSeconds = 0.0f;
    mHoldStartMs = 0;
    timer = -1;
    currentState = -1;
    mode = ACTIONSTACK_STANDARD;
    checked = 0;
    lastActionController = NULL;
    interruptBtnXOffset = noBtnXOffset = noToAllBtnXOffset = interruptDialogWidth = 0;
    interruptDialogX = 0;
    interruptDialogHeight = 0;

    if(!observer->getResourceManager()) return;
    for (int i = 0; i < 8; ++i)
    {
        std::ostringstream stream;
        stream << "iconspsp" << i;
        pspIcons[i] = observer->getResourceManager()->RetrieveQuad("iconspsp.png", (float) i * 32, 0, 32, 32, stream.str(), RETRIEVE_MANAGE);
        //RETRIEVE_MANAGE returns an empty ptr when the texture cache cannot
        //promote (cache full) - a missing icon must degrade to text, not crash.
        if (pspIcons[i])
            pspIcons[i]->SetHotSpot(16, 16);
    }
}

int ActionStack::has(MTGAbility * ability)
{
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        if (mObjects[i] && (((Interruptible *) mObjects[i])->type == ACTION_ABILITY)) // Fixed a random crash occurring when mObjects[i] is null...
        {
            StackAbility * action = ((StackAbility *) mObjects[i]);
            if (action->state == NOT_RESOLVED && action->ability == ability)
                return 1;
        }
    }
    return 0;
}

int ActionStack::has(Interruptible * action)
{
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        if (mObjects[i] == action)
            return 1;
    }
    return 0;
}

int ActionStack::resolve()
{
    Interruptible * action = getLatest(NOT_RESOLVED);

    if (!action)
        return 0;
    mPriorityOn = NULL; //next stack top binds a fresh priority round

    DebugTrace("Resolving Action on stack: " << action->getDisplayName());
    if (action->resolve())
    {
        action->state = RESOLVED_OK;
    }
    else
    {
        action->state = RESOLVED_NOK;
    }
    observer->bumpAbilityEpoch(); //#W54-H (A6b): a resolution is a state change
    if (action->type == ACTION_DAMAGE)
        ((Damage *) action)->target->afterDamage();
    if (!getNext(NULL, NOT_RESOLVED))
    {
        for (int i = 0; i < 2; i++)
        {
                    if (interruptDecision[i] != 2)
            interruptDecision[i] = NOT_DECIDED;
        }
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            if (interruptDecision[i] != DONT_INTERRUPT_ALL)
                interruptDecision[i] = NOT_DECIDED;
        }
    }
    lastActionController = NULL;
    return 1;

}

Interruptible * ActionStack::getPrevious(Interruptible * next, int type, int state, int display)
{
    int n = getPreviousIndex(next, type, state, display);
    if (n == -1)
        return NULL;
    return ((Interruptible *) mObjects[n]);
}

int ActionStack::getPreviousIndex(Interruptible * next, int type, int state, int display)
{
    int found = 0;
    if (!next)
        found = 1;
    for (int i = (int)(mObjects.size()) - 1; i >= 0; i--)
    {
        Interruptible * current = (Interruptible *) mObjects[i];
        if (found && (type == 0 || current->type == type) && (state == 0 || current->state == state) && (display
            == -1 || current->display == display))
        {
            return i;
        }
        if (current == next)
            found = 1;
    }
    if (!found)
        return getPreviousIndex(NULL, type, state, display);
    return -1;
}

int ActionStack::count(int type, int state, int display)
{
    int result = 0;
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        Interruptible * current = (Interruptible *) mObjects[i];
        if ((type == 0 || current->type == type) && (state == 0 || current->state == state) && (display == -1
            || current->display == display))
        {
            result++;
        }
    }
    return result;
}

Interruptible * ActionStack::getActionElementFromCard(MTGCardInstance * card)
{

    if(!card)
    return 0;
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        Interruptible * current = (Interruptible *) mObjects[i];
        if (current->source == card)
        {
            return current;
        }
    }  
    return NULL;
}

Interruptible * ActionStack::getNext(Interruptible * previous, int type, int state, int display)
{
    int n = getNextIndex(previous, type, state, display);
    if (n == -1)
        return NULL;
    return ((Interruptible *) mObjects[n]);
}

int ActionStack::getNextIndex(Interruptible * previous, int type, int state, int display)
{
    int found = 0;
    if (!previous)
        found = 1;
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        Interruptible * current = (Interruptible *) mObjects[i];
        if (found && (type == 0 || current->type == type) && (state == 0 || current->state == state) && (display
            == -1 || current->display == display))
        {
            return i;
        }
        if (current == previous)
            found = 1;
    }
    if (!found)
        return getNextIndex(NULL, type, state, display);
    return -1;
}

Interruptible * ActionStack::getLatest(int state)
{
    for (int i = (int)(mObjects.size()) - 1; i >= 0; i--)
    {
        Interruptible * action = ((Interruptible *) mObjects[i]);
        if (action->state == state)
            return action;
    }
    return NULL;
}

int ActionStack::receiveEventPlus(WEvent * event)
{
    int result = 0;
    for (size_t i = 0; i < mObjects.size(); ++i)
    {
        Interruptible * current = (Interruptible *) mObjects[i];
        result += current->receiveEvent(event);
    }
    return result;
}

bool ActionStack::wouldOfferWindow(Player * p, Interruptible * action)
{
    //CR 502.4 / 117.3a: no player receives priority during the untap step -
    //never offer a response window there. The engine used to grant the ask
    //(hasInstantResponse saw untapped lands) while every tap/cast inside it
    //was correctly refused by the phase rules: a window no one can use
    //(owner incident 2026-08-07, Lightning Helix at opponent's untap). The
    //slot the player actually wants - resolve after untapping, before upkeep
    //effects resolve - is the upkeep ask: spells stack ABOVE the pending
    //upkeep triggers and resolve first.
    if (observer->getCurrentGamePhase() == MTG_PHASE_UNTAP)
        return false;

    //Own casts/abilities auto-yield by default; the INTERRUPTMY* options
    //opt back in to being offered windows on your own actions.
    if (action && action->source && action->source->controller() == p)
    {
        if (action->type == ACTION_SPELL && !options[Options::INTERRUPTMYSPELLS].number)
            return false;
        if (action->type == ACTION_ABILITY && !options[Options::INTERRUPTMYABILITIES].number)
            return false;
    }
    //UI modality: a human mid-display/reveal cannot answer a window.
    if (!p->isAI() && (observer->OpenedDisplay || p->game->reveal->cards.size()))
        return false;
    //The universal priority rule: a window exists iff the player could
    //actually respond right now. Everyone else auto-passes silently.
    //NOTE (wave-5, verified by probe runs): this gate is honest - a seat that
    //tapped out on its own turn stays tapped through the opponent's next turn
    //and gets NO window on the opponent's casts. "Died with counters in hand"
    //is therefore usually a mana-discipline leak (guide layer), not a missing
    //window: keep counter mana untapped or the window never exists.
    return LegalActionsOracle::hasInstantResponse(p);
}

void ActionStack::Update(float dt)
{
    //The game ends IMMEDIATELY when a player is dead (rule 104): stop
    //resolving stack objects - without this, a lethal trigger loop resolves
    //one extra iteration past the killing blow (life -1 / an extra drain).
    if (observer->didWin())
        return;
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    //#W57-S: the stall probe. Prints the whole priority/choice state vector
    //every tick under WAGIC_STALLPROBE so a game that stops advancing names
    //the flag that is stuck. Compile-time gated (never in a release build),
    //env-gated inside that.
    {
        static bool probeOn = (getenv("WAGIC_STALLPROBE") != NULL);
        if (probeOn)
        {
            ActionLayer * pal = observer->mLayers ? observer->mLayers->actionLayer() : NULL;
            TargetChooser * ptc = observer->getCurrentTargetChooser();
            fprintf(stderr, "[stallprobe] t=%d ph=%d cur=%s act=%s intr=%s ask=%s dec=%d/%d mode=%d modal=%d"
                            " stackNR=%d wait=%p menu=%p gtc=%p gtcOwner=%s hold=%d\n",
                    observer->turn, (int) observer->getCurrentGamePhase(),
                    observer->currentPlayer == observer->players[0] ? "p1" : "p2",
                    observer->currentActionPlayer ? (observer->currentActionPlayer == observer->players[0] ? "p1" : "p2") : "-",
                    observer->isInterrupting ? (observer->isInterrupting == observer->players[0] ? "p1" : "p2") : "-",
                    askIfWishesToInterrupt ? (askIfWishesToInterrupt == observer->players[0] ? "p1" : "p2") : "-",
                    (int) interruptDecision[0], (int) interruptDecision[1], (int) mode, modal,
                    count(0, NOT_RESOLVED, 0),
                    (void *) (pal ? pal->isWaitingForAnswer() : NULL),
                    (void *) (pal ? pal->menuObject : NULL),
                    (void *) observer->targetChooser,
                    (observer->targetChooser && observer->targetChooser->Owner)
                        ? (observer->targetChooser->Owner == observer->players[0] ? "p1" : "p2") : "-",
                    mHoldTicks);
            fflush(stderr);
        }
    }
#endif

    //This is a hack to avoid updating the stack while tuto messages are being shown
    //Ideally, the tuto messages should be moved to a layer above this one
    //No need for Tuto when no human in game
    if (getCurrentTutorial() && (observer->players[0]->isHuman() || observer->players[1]->isHuman() ) )
        return;

    //A pending decision menu (kicker/X/mode/resolution-time choice) holds
    //the priority round for EVERY controller - the old human-only gate let
    //testsuite/AI games settle passes and resolve the spell before its
    //controller's scripted/policy answer arrived (Prohibit resolving
    //targetless, Mystic Confluence wedging mid-modes).
    if (observer->mLayers->actionLayer()->menuObject)// || observer->LPWeffect) //test fix for hang for both legendary with action/reveal
        return;//dont do any of this if a menuobject exists.

    askIfWishesToInterrupt = NULL;
    //modal = 0;

    TargetChooser * tc = observer->getCurrentTargetChooser();
    int newState = observer->getCurrentGamePhase();
    currentState = newState;
    if (!tc)
        checked = 0;

    //Select Stack's display mode
    if (mode == ACTIONSTACK_STANDARD && tc && !checked)
    {
        checked = 1;

        for (size_t i = 0; i < mObjects.size(); i++)
        {
            Interruptible * current = (Interruptible *) mObjects[i];
            if (tc->canTarget(current))
            {
                if (mCurr < (int) mObjects.size() && mObjects[mCurr])
                    mObjects[mCurr]->Leaving(JGE_BTN_UP);
                current->display = 1;
                mCurr = i;
                mObjects[mCurr]->Entering();
                mode = ACTIONSTACK_TARGET;
                modal = 1;
            }
            else
            {
                current->display = 0;
            }
        }
        if (mode != ACTIONSTACK_TARGET)
        {
        }
    }
    else if (mode == ACTIONSTACK_TARGET && !tc)
    {
        mode = ACTIONSTACK_STANDARD;
        checked = 0;
    }

    if (mode == ACTIONSTACK_STANDARD)
    {
        modal = 0;
        if (getLatest(NOT_RESOLVED) && !tc)
        {
            Interruptible * currentSpell = (Interruptible *)getLatest(NOT_RESOLVED);
            MTGCardInstance * card = currentSpell->source;
            if(card && card->has(Constants::SPLITSECOND))
            {
                resolve();
            }
            else
            {
                int currentPlayerId = 0;
                int otherPlayerId = 1;
                if (observer->currentlyActing() != observer->players[0])
                {
                    currentPlayerId = 1;
                    otherPlayerId = 0;
                }
                if (currentSpell != mPriorityOn)
                {
                    //A NEW stack object holds the top: bind a fresh priority
                    //round to it and let one tick pass before anyone passes
                    //or anything resolves. That tick is when triggered
                    //abilities enter the stack and state-based actions are
                    //applied - resolving in the same tick lets spells "jump"
                    //their triggers (bushido never fires, Chalice counters
                    //nothing, a dead Soul Warden still gains life).
                    mPriorityOn = currentSpell;
                    for (int di = 0; di < 2; di++)
                        if (interruptDecision[di] != DONT_INTERRUPT_ALL)
                            interruptDecision[di] = NOT_DECIDED;
                }
                else
                {
                //Auto-pass settles within THIS tick: a silent pass must not
                //consume frames, or resolution timing becomes wall-clock
                //sensitive (scripted/suite play desyncs, and real play adds
                //frames of dead time per stack item).
                if (interruptDecision[currentPlayerId] == NOT_DECIDED
                    && !wouldOfferWindow(observer->players[currentPlayerId], currentSpell))
                    interruptDecision[currentPlayerId] = DONT_INTERRUPT;
                if (interruptDecision[currentPlayerId] == DONT_INTERRUPT
                    && interruptDecision[otherPlayerId] == NOT_DECIDED
                    && !wouldOfferWindow(observer->players[otherPlayerId], currentSpell))
                    interruptDecision[otherPlayerId] = DONT_INTERRUPT;

#ifdef WAGIC_TRANSCRIPT_ON
                if (observer->isLoading() && getenv("WAGIC_TRANSCRIPT_TRACE"))
                    DebugTrace("[transcript-trace] stack " << currentSpell->getDisplayName() << " decisions p1=" << interruptDecision[0] << " p2=" << interruptDecision[1]
                               << " current=" << currentPlayerId << " isInterrupting=" << (observer->isInterrupting ? (observer->isInterrupting == observer->players[0] ? "p1" : "p2") : "none")
                               << " ask=" << (askIfWishesToInterrupt ? "set" : "none") << " modal=" << modal);
#endif
                if (interruptDecision[currentPlayerId] == NOT_DECIDED)
                {
                    askIfWishesToInterrupt = observer->players[currentPlayerId];
                    observer->isInterrupting = observer->players[currentPlayerId];
                    modal = 1;
                }
                else if (interruptDecision[currentPlayerId] == INTERRUPT)
                {
                    observer->isInterrupting = observer->players[currentPlayerId];

                }
                else
                {
                    if (interruptDecision[otherPlayerId] == NOT_DECIDED)
                    {
                        askIfWishesToInterrupt = observer->players[otherPlayerId];
                        observer->isInterrupting = observer->players[otherPlayerId];
                        modal = 1;
                    }
                    else if (interruptDecision[otherPlayerId] == INTERRUPT)
                    {
                        observer->isInterrupting = observer->players[otherPlayerId];
                    }
                    else
                    {
                        resolve();
                    }
                }
                }
            }
        }
    }
    else if (mode == ACTIONSTACK_TARGET)
    {
        GuiLayer::Update(dt);
    }
    if (askIfWishesToInterrupt)
    {
        // WALDORF - added code to use a game option setting to determine how
        // long the Interrupt timer should be. If it is set to zero (0), the
        // game will wait for ever for the user to make a selection.
        if (options[Options::INTERRUPT_SECONDS].number > 0)
        {
            int extraTime = 0;
            //extraTime is a multiplier, it counts the number of unresolved stack actions
            //then is used to extend the time you have to interupt.
            //this prevents you from "running out of time" while deciding.
            //before this int was added, it was possible to run out of time if you had 10 stack actions
            //and set the timer to 4 secs. BUG FIX //http://code.google.com/p/wagic/issues/detail?id=464
            extraTime = count(0, NOT_RESOLVED, 0);
            if (extraTime == 0)
                extraTime = 1;//we never want this int to be 0.

            if (timer < 0)
                timer = static_cast<float>(options[Options::INTERRUPT_SECONDS].number * extraTime);
            timer -= dt;
            if (timer < 0)
                cancelInterruptOffer();
        }
    }

    //W53-AA - THE STALL FLOOR. An interrupt window held by a seat that never
    //answers stops the whole game: userRequestNextGamePhase refuses while
    //anything is NOT_RESOLVED, INTERRUPT_SECONDS is 0 by default (the timer
    //above never runs), and DuelLayers::CheckUserInput reads-and-discards
    //every human key while `isInterrupting` names the other seat - so the
    //player presses and nothing at all happens, with no message. That is the
    //owner's Vita softlock shape (2026-09-02, deck5 vs baka deck33: The Rack's
    //upkeep trigger on the stack at the human's own upkeep, isInterrupting=p2,
    //stackUnresolved=1, every recorded phase request refused from there on).
    //
    //Progress is defined narrowly: a DIFFERENT holder, a DIFFERENT top stack
    //object, or an explicit extendInterruptOffer (the LLM seat's "still
    //thinking" signal) all reset the count. Nothing else does - a genuine
    //wedge changes none of them. Only AI-owned windows are watched: a human's
    //own window is bounded by their own input, which the key road accepts
    //(ActionStack::CheckUserInput / MTGGamePhase), and yanking it would steal
    //a response they are in the middle of making.
    //
    //While the game is LOADING (a transcript replay or an undo) the threshold
    //is tiny: DuelLayers::Update does not call Act() at all in that state, so
    //NO seat can answer a window the record does not contain, and the replay
    //stops dead exactly where a live softlock would - which blinds the
    //transcript tool at the one moment it is needed. A recorded answer always
    //arrives inside the load loop's next re-issue (6 updates per attempt), so
    //12 ticks cannot pre-empt one.
    if (stallFloorEnabled())
    {
        Interruptible * top = getNext(NULL, 0, NOT_RESOLVED);
        Player * holder = askIfWishesToInterrupt ? askIfWishesToInterrupt : observer->isInterrupting;
        const bool loading = observer->isLoading();
        if (!top || !holder || (!loading && !holder->isAI()))
        {
            mHoldOn = NULL;
            mHoldWho = NULL;
            mHoldTicks = 0;
            mHoldSeconds = 0.0f;
            mHoldStartMs = 0;
        }
        else
        {
            if (top != mHoldOn || holder != mHoldWho)
            {
                mHoldOn = top;
                mHoldWho = holder;
                mHoldTicks = 0;
                mHoldSeconds = 0.0f;
                mHoldStartMs = 0;
            }
            else
            {
                ++mHoldTicks;
                mHoldSeconds += dt;
            }
            //#W54-R: arm/re-arm the wall clock (extendInterruptOffer zeroes it).
            if (!mHoldStartMs)
                mHoldStartMs = stallFloorNowMs();
            const float wallSeconds = (float)(stallFloorNowMs() - mHoldStartMs) / 1000.0f;
            //Budgets in BOTH ticks and elapsed time, so neither a slow frame
            //rate nor a fast headless pump can trip it early. Loading: 12
            //ticks only - dt is a synthetic counter inside the load loop, and
            //see above for why 12 cannot pre-empt a recorded answer.
            //#W54-R. Two corrections to the wave-53 budget, and nothing else
            //about the floor moves - the softlock it fixed is a seat that can
            //NEVER answer, and that case is untouched.
            //
            //(1) A seat with a request IN FLIGHT is making progress by
            //definition. `extendInterruptOffer` was supposed to say so, but it
            //only resets while `askIfWishesToInterrupt == who`; once the seat
            //has TAKEN the window the holder is `observer->isInterrupting` and
            //the keep-alive silently no-ops - the seat's only "still thinking"
            //signal was disconnected from the watchdog about to kill it. Ask
            //the holder directly instead, on every branch: no window is ever
            //released out from under an answer that is on its way.
            //
            //(2) The interactive-AI budget is denominated in WALL CLOCK. It
            //was `mHoldSeconds`, i.e. accumulated dt, and dt is synthetic: the
            //corpus harness feeds a fixed WAGIC_FASTCLOCK 0.1 s per tick, so
            //1,200 "seconds" was exactly 12,000 ticks - about twelve wall
            //seconds against a mean model latency of 23.9 s. Every release in
            //the wave-54 corpus (468 of them) read the same 12,002 ticks, and
            //the seats they fired on recorded ~zero opponent-turn decisions.
            //The heuristic seat keeps its dt budget: it answers in a tick or
            //two, 20 s of any denomination is slack, and changing it would
            //move the lane-AA softlock pin.
            const bool inFlight = holder->aiDecisionInFlight();
            const bool spent = !inFlight && (loading
                ? (mHoldTicks >= 12)
                : (holder->isInteractiveAI()
                       ? (mHoldTicks >= 300 && wallSeconds >= 1200.0f)
                       : (mHoldTicks >= 300 && mHoldSeconds >= 20.0f)));
            if (spent)
            {
                const string who = holder->getDisplayName();
                const string what = top->getDisplayName();
                DebugTrace("ActionStack: interrupt window held by " << who << " on '" << what
                           << "' for " << mHoldTicks << " ticks / " << wallSeconds
                           << " s wall with no progress - releasing"
                           << (loading ? " (loading: no seat can answer)" : ""));
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS) || defined(WAGIC_TRANSCRIPT_ON)
                fprintf(stderr, "wagic: interrupt window held by %s on '%s' for %d ticks"
                                " / %.1f s wall (turn %d phase %d%s) - releasing so the game advances\n",
                        who.c_str(), what.c_str(), mHoldTicks, wallSeconds, observer->turn,
                        (int) observer->getCurrentGamePhase(), loading ? ", loading" : "");
                fflush(stderr);
#endif
                //The codebase's own decline: DONT_INTERRUPT for this seat,
                //unlogged so a replay's action list is not rewritten by the
                //recovery. isInterrupting is set in both the "asked" and the
                //"took it" state, which is what cancelInterruptOffer keys on.
                cancelInterruptOffer(DONT_INTERRUPT, false, true); //#W57-F (D34): forced
                mHoldOn = NULL;
                mHoldWho = NULL;
                mHoldTicks = 0;
                mHoldSeconds = 0.0f;
                mHoldStartMs = 0;
            }
        }
    }
}

//#W57-F (D36): THE THIRD REPLAY DEFECT, root-caused. The stall floor's LOADING
//arm (12 ticks, "no seat can answer") is right about the seats - a replay never
//calls AIPlayer::Act, so no seat answers a window - but it is WRONG about the
//record, which is the answer. Measured on a fresh Baka-vs-Baka transcript
//(125v126, 2026-09-03): the record has p1 take the window on Overgrown
//Battlement and tap two lands inside it; the loader spent 12 ticks pumping the
//action BETWEEN those two taps, the floor released p1's window, the Battlement
//resolved, and p1's recorded Plains tap then came back `0plains` - refused for
//all 60 retries, because ActivatedAbility::isReactingToClick needs
//`source->controller() == game->currentlyActing()` and the window that made p1
//the acting seat had just been taken away. That is the `0<name>` shape lane E
//parked, in full.
//The loader consuming a recorded action IS progress: reset the no-progress
//budget, so the floor fires only when the loader is stuck on ONE action (the
//softlock lane AA's fix exists for), never merely because a replay is slow.
void ActionStack::noteReplayProgress()
{
    mHoldTicks = 0;
    mHoldSeconds = 0.0f;
    mHoldStartMs = 0;
}

void ActionStack::cancelInterruptOffer(InterruptDecision cancelMode, bool log, bool forcedRelease)
{
    int playerId = (observer->isInterrupting == observer->players[1]) ? 1 : 0;
    //#W56-Z: NO safety net here, deliberately. `observer->targetChooser` is
    //global to the observer, not per-seat, and cancelInterruptOffer runs on the
    //ORDINARY decline path - every time a seat waves off an offer while the
    //ACTIVE player is mid-cast. Cancelling there killed the caster's own
    //in-flight chooser: counter_unless_pay_x and spell_blast_counter_matching_mv
    //both went red. endOfInterruption is safe because it only runs once a seat
    //has TAKEN the window, so the pending choice is that seat's own.
    //#W57-F (D34): lane Z's residual, and the reason it was a residual. The
    //chooser IS per-seat - TargetChooser::Owner is stamped from
    //source->controller() at construction - the OBSERVER's pointer just never
    //carried the fact. With the owner read, the net lane Z had to remove can be
    //put back where its residual is: the stall watchdog takes a window away
    //from a seat that never answered, and a chooser that seat armed inside it
    //has no other route out (nothing else clears it, and the next window's
    //first click on any legal target completes the abandoned cast - the owner's
    //vpk15 Putrefy report). Scoped twice: the FORCED route only, and only a
    //chooser the RELEASED SEAT owns, so the ordinary decline still cannot touch
    //the active player's in-flight cast.
    Player * released = observer->isInterrupting ? observer->isInterrupting : askIfWishesToInterrupt;
    if (forcedRelease && released && observer->targetChooser
        && observer->targetChooser->Owner == released)
    {
        DebugTrace("ActionStack: releasing the chooser " << released->getDisplayName()
                   << " armed inside the window the watchdog is taking back");
        observer->releaseTargetChooser(); //#W57-F (D34)
    }
    interruptDecision[playerId] = cancelMode;
    askIfWishesToInterrupt = NULL;
    observer->isInterrupting = NULL;
    timer = -1;
    if(log) {
        stringstream stream;
        stream << "no " << cancelMode;
        observer->logAction(playerId, stream.str());
    }
}

//#W56-Z. A target/cost chooser armed inside an interrupt window used to
//OUTLIVE that window: MTGPutInPlayRule::reactToClick pays only AFTER
//game->targetListIsSet(card), so clicking a targeted spell arms
//GameObserver::targetChooser (and cardWaitingForTargets) with nothing paid,
//and nothing cleared it when the window closed. On the NEXT window the first
//click on any legal target ran GameObserver::cardClick's `if (targetChooser)`
//branch -> TARGET_OK_FULL -> cardClick(cardWaitingForTargets) and the spell
//resolved. The owner's Vita report (2026-09-03): Putrefy was armed, cancel was
//pressed eighteen times, and the only legal target was his own Thornweald
//Archer - which the engine then destroyed for him.
//This is the one cancel used by both fix legs. It mirrors the human's ordinary
//cancel path (CardSelector JGE_BTN_SEC -> GameObserver::cancelCurrentAction):
//the action layer's cancelCurrentAction() honours cantCancel, and the cast
//chooser is released only when nothing mandatory is pending.
int ActionStack::cancelPendingChoice()
{
    if (!observer || !observer->mLayers)
        return 0;
    ActionLayer * al = observer->mLayers->actionLayer();
    bool pendingAbility = (al && al->isWaitingForAnswer() != NULL);
    bool pendingCast = (observer->targetChooser != NULL);
    if (!pendingAbility && !pendingCast)
        return 0;
    if (pendingAbility && !al->cancelCurrentAction())
        return 0; //mandatory choice (cantCancel): leave it exactly as it was
    if (pendingCast)
        SAFE_DELETE(observer->targetChooser);
    return 1;
}

void ActionStack::endOfInterruption(bool log)
{
    int playerId = (observer->isInterrupting == observer->players[1]) ? 1 : 0;
    //#W56-Z safety net: whatever route closes the window (this includes the
    //test suite's `endinterruption` command and the abilities-menu Cancel id),
    //a non-AI seat's pending choice dies with the window. AI seats are left
    //byte-identical - they drive their own choosers through AIPlayerBaka and
    //never reach this by pressing a key (DuelLayers::CheckUserInput is gated
    //on !isAI).
    if (observer->isInterrupting && observer->isInterrupting->playMode != Player::MODE_AI)
        cancelPendingChoice();
    interruptDecision[playerId] = NOT_DECIDED;
    observer->isInterrupting = NULL;
    if(log)
        observer->logAction(playerId, "endinterruption");
}

JButton ActionStack::handleInterruptRequest( JButton inputKey, int& x, int& y )
{
    //Touch-first mapping (2026-08-10). The old hit test honored only the thin
    //strip of button-hint labels (y 10..40): tapping the "Interrupt?" title
    //text (y<10), a stack item, or anywhere near-but-not-on a label fell
    //through unmapped - the offer branch consumed the click and the tap's
    //trailing JGE_BTN_OK then DECLINED. On a touchscreen that read as "the
    //interrupt button declines". Now the No / No to All labels keep their
    //zones and the REST of the dialog - title, Interrupt label, stack items -
    //is one big Interrupt target; taps outside the dialog still decline.
    if (!gModRules.game.canInterrupt())
        return inputKey;

    if (y >= 10 && y < (kIconVerticalOffset + 16))
    {
        if (x >= noBtnXOffset && x < noToAllBtnXOffset)
            return JGE_BTN_OK;

        if (x >= noToAllBtnXOffset && x < interruptDialogWidth)
            return JGE_BTN_PRI;
    }

    int dialogRight = interruptDialogX + 224; //x0-7 .. x0+width+17, width=200
    if (interruptDialogHeight > 0 && x >= interruptDialogX && x < dialogRight && y >= 0 && y < interruptDialogHeight)
        return JGE_BTN_SEC;

    return inputKey;
}


bool ActionStack::CheckUserInput(JButton inputKey)
{
    JButton key = inputKey;
    JButton trigger = (options[Options::REVERSETRIGGERS].number ? JGE_BTN_NEXT : JGE_BTN_PREV);
    if (mode == ACTIONSTACK_STANDARD)
    {        
        if (askIfWishesToInterrupt)
        {
            int x,y;
            if(observer->getInput()->GetLeftClickCoordinates(x, y))
            {
                key = handleInterruptRequest(inputKey, x, y);
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
                if (FILE * f = jgePadlogFile())
                {
                    fprintf(f, "stack offer click x=%d y=%d in=%d -> key=%d (dialog x=%d..%d h=%d)\n",
                            x, y, (int)inputKey, (int)key, interruptDialogX, interruptDialogX + 224, interruptDialogHeight);
                    fflush(f);
                }
#endif //_DEBUG || WAGIC_DEVLOGS

            }
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
            else if (inputKey != JGE_BTN_NONE)
            {
                if (FILE * f = jgePadlogFile())
                    { fprintf(f, "stack offer bare key=%d\n", (int)inputKey); fflush(f); }
            }
#endif //_DEBUG || WAGIC_DEVLOGS


            if (JGE_BTN_SEC == key && gModRules.game.canInterrupt())
            {
                setIsInterrupting(askIfWishesToInterrupt);
                return true;
            }
            else if ((JGE_BTN_OK == key) || (trigger == key))
            {
                cancelInterruptOffer();
                return true;
            }
            else if ((JGE_BTN_PRI == key))
            {
                cancelInterruptOffer(DONT_INTERRUPT_ALL);
                return true;
            }
            return true;
        }
        else if (observer->isInterrupting)
        {
            //End the interruption (pass priority) on the interrupt button
            //(JGE_BTN_SEC = middle-click / 'k') OR on the "pass / next phase"
            //gesture (trigger = right-click by default). Previously only
            //JGE_BTN_SEC worked, so a player who right-clicked - which means
            //"next phase" everywhere else and is the natural "pass priority"
            //gesture - had no way out of an interrupt window they had nothing
            //to play in, and the game soft-locked. The offer branch above
            //already treats `trigger` as decline/pass; mirror it here.
            if (JGE_BTN_SEC == key || trigger == key)
            {
                if(observer->mExtraPayment)
                {
                    observer->mExtraPayment->action->CheckUserInput(JGE_BTN_SEC);
                    observer->mExtraPayment = NULL;
                }
                //#W56-Z. The stack layer sees the key BEFORE the action layer
                //and CardSelector (DuelLayers::CheckUserInput order), so while
                //this seat was interrupting the cancel button could never reach
                //the ordinary cast-cancel path - it always meant "end the
                //window" instead. If a choice is pending, cancel THAT and keep
                //the window open (nothing has been paid yet - the pool still
                //floats and the card stays in hand); a second press then ends
                //the interruption exactly as before. A mandatory choice
                //(cantCancel) falls through untouched.
                if (cancelPendingChoice())
                    return true;
                endOfInterruption();
                return true;
            }
        }
    }
    else if (mode == ACTIONSTACK_TARGET)
    {
        if (modal)
        {
            if (JGE_BTN_UP == key)
            {
                if (mObjects[mCurr])
                {
                    int n = getPreviousIndex(((Interruptible *) mObjects[mCurr]), 0, 0, 1);
                    if (n != -1 && n != mCurr && mObjects[mCurr]->Leaving(JGE_BTN_UP))
                    {
                        mCurr = n;
                        mObjects[mCurr]->Entering();
                        DebugTrace("ACTIONSTACK UP TO mCurr = " << mCurr);
                    }
                }
                return true;
            }
            else if (JGE_BTN_DOWN == key)
            {
                if( mObjects[mCurr])
                {
                    int n = getNextIndex(((Interruptible *) mObjects[mCurr]), 0, 0, 1);
                    if (n!= -1 && n != mCurr && mObjects[mCurr]->Leaving(JGE_BTN_DOWN))
                    {
                        mCurr = n;
                        mObjects[mCurr]->Entering();
                        DebugTrace("ACTIONSTACK DOWN TO mCurr " << mCurr);
                    }
                }
                return true;
            }
            else if (JGE_BTN_OK == key)
            {
                DebugTrace("ACTIONSTACK CLICKED mCurr = " << mCurr);

                observer->stackObjectClicked(((Interruptible *) mObjects[mCurr]));
                return true;
            }
            return true; //Steal the input to other layers if we're visible
        }
        if (JGE_BTN_CANCEL == key)
        {
            if (modal) modal = 0;
            else modal = 1;
            return true;
        }
    }
    return false;
}

//Cleans history of last turn
int ActionStack::garbageCollect()
{
    std::vector<JGuiObject *>::iterator iter = mObjects.begin();

    while (iter != mObjects.end())
    {
        Interruptible * current = ((Interruptible *) *iter);
        if (current->state != NOT_RESOLVED)
        {
            //#W54-H (L16): the dynamic_cast<AManaProducer*> branch that stood
            //here could never fire (no class derives from both) and erased
            //manaObjects with an iterator into mObjects if it ever did.
            iter = mObjects.erase(iter);
            SAFE_DELETE(current);
        }
        else
            ++iter;
    }
    return 1;
}

// Fizzle action and put it in targetZone
void ActionStack::Fizzle(Interruptible * action, MTGCardInstance * fizzler, FizzleMode fizzleMode)
{
    if (!action)
    {
        DebugTrace("ACTIONSTACK ==ERROR==: action is NULL in ActionStack::Fizzle");
        return;
    }
    if (action->type == ACTION_SPELL)
    {
        Spell * spell = (Spell *) action;
        MTGCardInstance * _target = NULL;
        unsigned int position = 0;
        //W35-narration: announce the COUNTER before the card moves. Every
        //fizzle mode below leaves the stack with an ordinary zone change that is
        //indistinguishable from a resolution ("stack -> graveyard" for both), so
        //an observer needs this marker to record the outcome. Raised first and
        //the observer's queue is FIFO, so it always precedes the move it
        //describes.
        if (spell->source && spell->source->getObserver())
            spell->source->getObserver()->receiveEvent(NEW WEventSpellCountered(spell->source, fizzler));
        switch (fizzleMode) {
        case PUT_IN_GRAVEARD:
            //Flashback-cast spells are exiled wherever they would leave
            //the stack - including when countered (CR 702.34a). The
            //resolved path already does this; the countered path sent
            //them back to the graveyard (Dread Return report in #1085).
            if (spell->source->alternateCostPaid[ManaCost::MANA_PAID_WITH_FLASHBACK] > 0
                || spell->source->basicAbilities[(int)Constants::TEMPFLASHBACK])
                spell->source->controller()->game->putInExile(spell->source);
            else
                spell->source->controller()->game->putInGraveyard(spell->source);
            break;
        case PUT_IN_HAND:
            spell->source->controller()->game->putInHand(spell->source);
            break;
        case PUT_IN_EXILE:
        case PUT_IN_EXILE_IMPRINT:
            _target = spell->source->controller()->game->putInExile(spell->source);
            if (_target && fizzler && fizzleMode == PUT_IN_EXILE_IMPRINT){
                fizzler->imprintedCards.push_back(_target);
                if (fizzler->imprintedCards.size()){
                    if (fizzler->imprintedCards.back()->getName().size()){
                        fizzler->currentimprintName = fizzler->imprintedCards.back()->getName();
                        fizzler->imprintedNames.push_back(fizzler->imprintedCards.back()->getName());
                    }
                }
            }
            break;
        case PUT_IN_LIBRARY_TOP:
        case PUT_IN_LIBRARY_SECOND:
        case PUT_IN_LIBRARY_BOTTOM:
            _target = spell->source->controller()->game->putInLibrary(spell->source);
            if (_target && fizzleMode == PUT_IN_LIBRARY_BOTTOM){
                MTGLibrary * library = _target->owner->game->library;
                vector<MTGCardInstance *>oldOrder = library->cards;
                vector<MTGCardInstance *>newOrder;
                newOrder.push_back(_target);
                for(unsigned int k = 0 ;k < oldOrder.size(); ++k)
                {
                    MTGCardInstance * rearranged = oldOrder[k];
                    if(rearranged != _target)
                        newOrder.push_back(rearranged);
                }
                library->cards = newOrder;
            } else if (_target && fizzleMode == PUT_IN_LIBRARY_SECOND){
                position = 2;
                MTGLibrary * library = _target->owner->game->library;
                vector<MTGCardInstance *>oldOrder = library->cards;
                vector<MTGCardInstance *>newOrder;
                if(position > oldOrder.size())
                    position = oldOrder.size(); //Avoid to exceed the library dimension.
                for(unsigned int k = 0; k < oldOrder.size() - position; ++k)
                {
                    MTGCardInstance * rearranged = oldOrder[k];
                    if(rearranged != _target)
                        newOrder.push_back(rearranged);
                }
                newOrder.push_back(_target);
                for(unsigned int k = oldOrder.size() - position ; k < oldOrder.size(); ++k)
                {
                    MTGCardInstance * rearranged = oldOrder[k];
                    if(rearranged != _target)
                        newOrder.push_back(rearranged);
                }
                library->cards = newOrder;
            }
            break;
        }
    }
    action->state = RESOLVED_NOK;
}

void ActionStack::Render()
{
    //This is a hack to avoid rendering the stack above the tuto messages
    //Ideally, the tuto messages should be moved to a layer above this one
    if (getCurrentTutorial())
        return;

    static const float kSpacer = 8;
    static const float x0 = 250;
    static const float y0 = 0;
    float width = 200;
    float height = 25;
    float currenty = y0 + 5;

    if (mode == ACTIONSTACK_STANDARD)
    {
        if (!askIfWishesToInterrupt || !askIfWishesToInterrupt->displayStack())
            return;
        /*observer->mExtraPayment = NULL*/;//end any payment request from extra cost as we open the stack to display items.
        if(observer->mExtraPayment)
        {
        observer->mExtraPayment->action->CheckUserInput(JGE_BTN_SEC);
        observer->mExtraPayment = NULL;
        }
        for (size_t i = 0; i < mObjects.size(); i++)
        {
            Interruptible * current = (Interruptible *) mObjects[i];
            if (current->state == NOT_RESOLVED)
                height += current->mHeight;
        }

        WFont * mFont = observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT);
        mFont->SetBase(0);
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
        mFont->SetColor(ARGB(255,255,255,255));
        JRenderer * renderer = JRenderer::GetInstance();

        //stack shadow
        //renderer->FillRoundRect(x0 - 7, y0+2, width + 17, height + 2, 9.0f, ARGB(128,0,0,0));
        //stack fill
        renderer->FillRect(x0 - 7, y0+2, width + 17, height + 14, ARGB(225,5,5,5));
        //top stack fill
        renderer->FillRect(x0 - 6, y0+37, width + 15, 40.5f, ARGB(20,135,206,235));
        //stack highlight
        renderer->FillRect(x0 - 6, y0+3, width + 15, 31.f, ARGB(255,89,89,89));
        //another border
        renderer->DrawRect(x0 - 6, y0+34.5f, width + 15, height - 19.5f, ARGB(255,89,89,89));
        //stack border
        renderer->DrawRect(x0 - 7, y0+2, width + 17, height + 14, ARGB(255,240,240,240));

        //remember the dialog rect for the touch hit test (see
        //handleInterruptRequest): the WHOLE dialog is the Interrupt target.
        interruptDialogX = static_cast<int>(x0 - 7);
        interruptDialogHeight = static_cast<int>(y0 + height + 16);
        
        std::ostringstream stream;
        // WALDORF - changed "interrupt ?" to "Interrupt?". Don't display count down
        // seconds if the user disables auto progressing interrupts by setting the seconds
        // value to zero in Options.

        // Mootpoint 01/12/2011: draw the interrupt text first, at the top.  Offset the rest of the 
        // unresolved stack effects down so that they don't collide with the interrupt text.
        if (options[Options::INTERRUPT_SECONDS].number == 0)
            stream << _(kInterruptMessageString);
        else
            stream << _(kInterruptMessageString) << " " << static_cast<int>(timer);

        mFont->DrawString(stream.str(), x0 + 5, currenty - 2);

//        static const float kIconVerticalOffset = 24;
        static const float kIconHorizontalOffset = 10;
        static const float kBeforeIconSpace = 12;
  
        //Render "interrupt?" text + possible actions
        {
            const int kIconForSEC = iconForBoundKey(JGE_BTN_SEC, kIconForSECDefault);
            const int kIconForOK  = iconForBoundKey(JGE_BTN_OK,  kIconForOKDefault);
            const int kIconForPRI = iconForBoundKey(JGE_BTN_PRI, kIconForPRIDefault);
            float currentx = x0 + 10;
            interruptBtnXOffset = static_cast<int>(currentx);

            if (gModRules.game.canInterrupt())
            {
                if (pspIcons[kIconForSEC])
                    renderer->RenderQuad(pspIcons[kIconForSEC].get(), currentx, kIconVerticalOffset - 2, 0, kGamepadIconSize, kGamepadIconSize);
                currentx+= kIconHorizontalOffset;
                mFont->DrawString(_(kInterruptString), currentx, kIconVerticalOffset - 8);
                currentx+= mFont->GetStringWidth(_(kInterruptString).c_str()) + kBeforeIconSpace;
            }

            noBtnXOffset = static_cast<int>(currentx);
            
            if (pspIcons[kIconForOK])
                renderer->RenderQuad(pspIcons[kIconForOK].get(), currentx, kIconVerticalOffset - 2, 0, kGamepadIconSize, kGamepadIconSize);
            currentx+= kIconHorizontalOffset;
            mFont->DrawString(_(kNoString), currentx, kIconVerticalOffset - 8);
            currentx+= mFont->GetStringWidth(_(kNoString).c_str()) + kBeforeIconSpace;

            noToAllBtnXOffset = static_cast<int>(currentx);
            if (mObjects.size() > 1)
            {
                if (pspIcons[kIconForPRI])
                    renderer->RenderQuad(pspIcons[kIconForPRI].get(), currentx, kIconVerticalOffset - 2, 0, kGamepadIconSize, kGamepadIconSize);
                currentx+= kIconHorizontalOffset;
                mFont->DrawString(_(kNoToAllString), currentx, kIconVerticalOffset - 8);
                currentx+= mFont->GetStringWidth(_(kNoToAllString).c_str()) + kBeforeIconSpace;
            }
            
            interruptDialogWidth = static_cast<int>(currentx);
        }

        currenty += kIconVerticalOffset + kSpacer;

        float totalmHeight = 0.f;
        for (size_t i = 0; i < mObjects.size(); i++)
        {
            Interruptible * current = (Interruptible *) mObjects[i];
            if (current && current->state == NOT_RESOLVED)
                totalmHeight += current->mHeight;
        }
        int sC = 0;//stack Count
        for (size_t i = 0; i < mObjects.size(); i++)
        {
            Interruptible * current = (Interruptible *) mObjects[i];
            if (current && current->state == NOT_RESOLVED)
            {
                /*
                current->x = x0;
                current->y = currenty;
                current->Render();

                currenty += current->mHeight;*/
                sC+=1;
                float cH = current->mHeight*sC;
                current->x = x0;
                current->y = (5+kIconVerticalOffset + kSpacer) + (totalmHeight - cH);
                //render the stack object
                current->Render();

                currenty += current->mHeight;
            }
        }
    }
    else if (mode == ACTIONSTACK_TARGET && modal)
    {
        for (size_t i = 0; i < mObjects.size(); i++)
        {
            Interruptible * current = (Interruptible *) mObjects[i];
            if (current->display)
                height += current->mHeight;
        }

        WFont * mFont = observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT);
        mFont->SetScale(DEFAULT_MAIN_FONT_SCALE);
        mFont->SetColor(ARGB(255,255,255,255));

        JRenderer * renderer = JRenderer::GetInstance();
        renderer->FillRect(x0, y0, width, height, ARGB(200,0,0,0));
        renderer->DrawRect(x0 - 1, y0 - 1, width + 2, height + 2, ARGB(255,255,255,255));

        for (size_t i = 0; i < mObjects.size(); i++)
        {
            Interruptible * current = (Interruptible *) mObjects[i];
            if (mObjects[i] != NULL && current->display)
            {
                ((Interruptible *) mObjects[i])->x = x0 + 5;
                if (i != mObjects.size() - 1)
                {
                    ((Interruptible *) mObjects[i])->y = currenty;
                    currenty += ((Interruptible *) mObjects[i])->mHeight;
                }
                else
                {
                    ((Interruptible *) mObjects[i])->y = currenty + 40;
                    currenty += ((Interruptible *) mObjects[i])->mHeight + 40;
                }
                current->mHasFocus = false;//fix stack display
                mObjects[i]->Render();
            }
        }
    }
}

#if defined (WIN32) || defined (LINUX)  || defined (IOS)

void Interruptible::Dump()
{
    string stype, sstate, sdisplay = "";
    switch (type)
    {
    case ACTION_SPELL:
        stype = "spell";
        break;
    case ACTION_DAMAGE:
        stype = "damage";
        break;
    case ACTION_DAMAGES:
        stype = "damages";
        break;
    case ACTION_NEXTGAMEPHASE:
        stype = "next phase";
        break;
    case ACTION_DRAW:
        stype = "draw";
        break;
    case ACTION_PUTINGRAVEYARD:
        stype = "put in graveyard";
        break;
    case ACTION_ABILITY:
        stype = "ability";
        break;
    default:
        stype = "unknown";
        break;
    }

    switch(state)
    {
    case NOT_RESOLVED:
        sstate = "not resolved";
        break;
    case RESOLVED_OK:
        sstate = "resolved";
        break;
    case RESOLVED_NOK:
        sstate = "fizzled";
        break;
    default:
        sstate = "unknown";
        break;
    }
    DebugTrace("type: " << stype << " " << type << " - state: " << sstate << " " << state << " - display: " << display);
}

void ActionStack::Dump()
{
    DebugTrace("=====\nDumping Action Stack=====");
    for (size_t i = 0; i < mObjects.size(); i++)
    {
        Interruptible * current = (Interruptible *)mObjects[i];
        current->Dump();
    }
}

#endif
