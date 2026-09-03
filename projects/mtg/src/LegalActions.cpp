#include "PrecompiledHeader.h"

#include "LegalActions.h"
#include "ExtraCost.h"
#include "Player.h"
#include "GameObserver.h"
#include "AllAbilities.h"
#include "TargetChooser.h"
#include "PlayRestrictions.h"
#include "ActionStack.h"

bool LegalActionsOracle::payable(Player * p, ManaEngine::ManaPolicy & policy, MTGCardInstance * card, ManaCost * pMana)
{
    ManaCost * cost = card->getManaCost();
    if (!cost)
        return true;
    if (!cost->getConvertedCost() && !cost->extraCosts)
        return true;
    if (pMana->canAfford(cost, card->has(Constants::ANYTYPEOFMANA)))
        return true;
    if (cost->getAlternative() && Delve::alternativeIsDelve(cost))
    {
        if (Delve::offerable(card, pMana)) //W48-DELVE: never the {0} shell
            return true;
    }
    else if (cost->getAlternative() && pMana->canAfford(cost->getAlternative(), 0))
        return true;
    if (cost->getMorph() && pMana->canAfford(cost->getMorph(), 0))
        return true;
    if (cost->getFlashback() && pMana->canAfford(cost->getFlashback(), 0))
        return true;
    if (cost->getRetrace() && pMana->canAfford(cost->getRetrace(), 0))
        return true;
    //specific-producer payments (e.g. sac-for-mana rocks) that potential
    //mana does not cover
    if (ManaEngine::planPayment(p, policy, card, cost, card->has(Constants::ANYTYPEOFMANA)).size())
        return true;
    return false;
}

vector<LegalActionsOracle::Cast> LegalActionsOracle::legalCasts(Player * p, ManaEngine::ManaPolicy & policy, ManaCost * pMana, bool instantSpeedOnly)
{
    vector<LegalActionsOracle::Cast> result;
    std::set<string> seen; //same name+zone = same decision; list it once

    struct ZoneScan { MTGGameZone * zone; const char * label; };
    ZoneScan scans[] = {
        { p->game->hand, "" },
        { p->game->graveyard, " [from your graveyard]" },
        { p->game->exile, " [from exile]" },
        { p->game->commandzone, " [from your command zone]" },
    };
    for (size_t s = 0; s < sizeof(scans) / sizeof(scans[0]); s++)
    {
        MTGGameZone * zone = scans[s].zone;
        for (int i = 0; i < zone->nb_cards; i++)
        {
            MTGCardInstance * card = zone->cards[i];
            if (card->isLand())
                continue; //the land rung handles those
            if (instantSpeedOnly
                && !card->hasType(Subtypes::TYPE_INSTANT) && !card->has(Constants::FLASH)
                && !card->has(Constants::ASFLASH)
                && !(card->getManaCost() && card->getManaCost()->getAlternative()
                     && card->getManaCost()->getAlternative()->instantSpeed)) //W53-SPLIT
                continue;
            if (zone == p->game->graveyard
                && !card->has(Constants::CANPLAYFROMGRAVEYARD) && !card->has(Constants::TEMPFLASHBACK)
                && !(card->getManaCost() && card->getManaCost()->getFlashback())
                && !(card->getManaCost() && card->getManaCost()->getRetrace()))
                continue;
            if (zone == p->game->exile && !card->has(Constants::CANPLAYFROMEXILE))
                continue;
            //N-152g: the "legend rule" is NOT a casting restriction. CR 704.5j
            //is a state-based action checked after the spell RESOLVES, and
            //MTGNewLegend implements it (the controller picks which copy to
            //keep, the rest go to the graveyard). Suppressing the cast hid a
            //legal and often correct play - recasting a legend to re-trigger
            //its ETB, or to replace a shrunken/enchanted copy.
            if (p->game->playRestrictions->canPutIntoZone(card, p->game->stack) == PlayRestriction::CANT_PLAY)
                continue;
            if (!payable(p, policy, card, pMana))
                continue;
            //Rules-valid action set (MTG 601.2c): a spell that REQUIRES a
            //target cannot legally be cast with none available - offering
            //it wastes a model call AND the failed validation hands the
            //whole cast decision to the heuristic. Untargeted spells pass
            //untouched, and a castable spell with a null-looking effect
            //stays offered (casting for another card's trigger is
            //legitimate play).
            {
                TargetChooserFactory tcf(p->getObserver());
                TargetChooser * tc = tcf.createTargetChooser(card);
                if (tc)
                {
                    bool castable = true;
                    if (tc->maxtargets == 1 && !tc->validTargetsExist())
                        castable = false; //mandatory single target, none exist
                    if (tc->targetMin && !tc->validTargetsExist(tc->maxtargets))
                        castable = false; //"exactly N" with fewer than N
                    SAFE_DELETE(tc);
                    if (!castable)
                        continue;
                }
            }
            string key = card->getDisplayName() + scans[s].label;
            if (!seen.insert(key).second)
                continue;

            //A card affordable BOTH normally and through its alternative
            //cost (evoke vs hardcast, Force-of-Will-style pitches) is two
            //different plays: list each mode so the model chooses it
            //explicitly. Alternative casting only exists on the hand path.
            ManaCost * cost = card->getManaCost();
            bool normalOk = cost
                && (pMana->canAfford(cost, card->has(Constants::ANYTYPEOFMANA))
                    || ManaEngine::planPayment(p, policy, card, cost, card->has(Constants::ANYTYPEOFMANA)).size());
            //Convoke models its alternative as a {0}(+{X}) shell plus a
            //Convoke extra cost, so the plain canAfford below is TRIVIALLY true
            //(zero mana is always affordable) - the convoke variant was offered
            //regardless of whether the tapped creatures + floatable mana could
            //ever complete the PRINTED cost (deck137: unaffordable March
            //convoke picked then deferred, unaffordable Loxodon convoke picked
            //then silently no-op'd; corpus 20260726). The legality-oracle
            //invariant is that unpayable choices are structurally impossible, so
            //compute real convoke payability BEFORE offering: max creature-tap
            //reduction + floatable mana must complete the cost (and for an
            //X-spell, reach X>=1 - an X=0 March is a donothing). This is NOT
            //over-suppression: a convoke affordable ONLY via creatures (too few
            //lands for the normal cost - convoke's whole point, the wave-28
            //Venerated-Loxodon witness) still passes, because offerable() adds
            //the creature reduction the plain canAfford ignores. Non-convoke
            //alternatives (offering/delve/flashback/pitch) keep the mana check.
            bool altIsConvoke = false;
            if (cost && cost->getAlternative() && cost->getAlternative()->extraCosts)
            {
                ExtraCosts * aec = cost->getAlternative()->extraCosts;
                for (size_t ei = 0; ei < aec->costs.size(); ei++)
                    if (dynamic_cast<Convoke *>(aec->costs[ei]))
                    {
                        altIsConvoke = true;
                        break;
                    }
            }
            bool altOk = zone == p->game->hand && cost && cost->getAlternative()
                && (altIsConvoke
                    ? Convoke::offerable(card, pMana)
                    : Delve::alternativeIsDelve(cost)
                    ? Delve::offerable(card, pMana) //W48-DELVE
                    : (pMana->canAfford(cost->getAlternative(), 0)
                       || ManaEngine::planPayment(p, policy, card, cost->getAlternative(), 0).size()));

            //A targeted EXTRA cost on the alternative (Force of Negation's
            //"exile another blue card from hand", `other={E(other
            //*[blue]|myhand)}`) is part of what makes the mode payable:
            //MTG 601.2f-h, you may only choose the alternative cost if you
            //CAN pay it. canAfford/planPayment above weigh only the mana
            //part, so the mode was offered with no legal card to exile - the
            //"other" targeter excludes the source, so with the source as the
            //lone blue card in hand the payment finds nothing and the
            //committed cast aborts silently, re-offered every window
            //(corpus 20260715). Require every targeted extra cost of the
            //alternative to have at least one legal target before offering.
            if (altOk && cost->getAlternative()->extraCosts)
            {
                ExtraCosts * aec = cost->getAlternative()->extraCosts;
                for (size_t ei = 0; altOk && ei < aec->costs.size(); ei++)
                {
                    if (!aec->costs[ei]->tc)
                        continue;
                    aec->costs[ei]->setSource(card);
                    if (!aec->costs[ei]->tc->countValidTargets())
                        altOk = false;
                }
            }

            //Cast restrictions gate WHICH modes are legal (Arcum's
            //Astrolabe: restriction=never + other={i} = alternative-only).
            //They used to live only in FindCardToPlay's policy dance, so
            //the menu offered unexecutable modes - the model picking one
            //re-cast into a silent no-op every tick (the 135v133 turn-2
            //livelock, 903k re-picks of the same entry). Gate each ENTRY
            //on its own restriction set, as the engine does at cast time.
            //normalEntry stays the representative for flashback/morph
            //pricing (payable() passed with neither flag), like before.
            //
            //The OLD proxy "normalOk || !altOk" silently re-admitted the
            //printed-cost mode whenever the alternative was knocked out - its
            //MANA affordable but its extra-cost TARGET missing. Force of
            //Negation with a blue-less hand and only {g} in the pool is the
            //case: payable() (line 20) passed on the alternative's {0} mana
            //alone, the extra-cost check above set altOk=false (no blue card
            //to exile), and "!altOk" then re-offered the unpayable {1}{U}{U}
            //hardcast - chosen, then silently deferred/fizzled (deck135
            //wave-16 s8/s16). Gate the normal entry on a mode that is ACTUALLY
            //payable ignoring the alternative: the printed cost (normalOk
            //already weighs canAfford/planPayment) or a non-alternative
            //special mode (flashback/morph/retrace, which payable() accepts
            //but normalOk does not separately track). A null-cost card is free.
            bool normalEntry = normalOk || !cost
                || (cost->getMorph() && pMana->canAfford(cost->getMorph(), 0))
                || (cost->getFlashback() && pMana->canAfford(cost->getFlashback(), 0))
                || (cost->getRetrace() && pMana->canAfford(cost->getRetrace(), 0));
            {
                AbilityFactory af(p->getObserver());
                if (normalEntry && card->getRestrictions().size()
                    && !af.parseCastRestrictions(card, p, card->getRestrictions()))
                    normalEntry = false;
                if (altOk && card->getOtherRestrictions().size()
                    && !af.parseCastRestrictions(card, p, card->getOtherRestrictions()))
                    altOk = false;
                if (!normalEntry && !altOk)
                    continue;
            }

            if (normalEntry)
            {
                LegalActionsOracle::Cast c;
                c.card = card;
                c.zoneLabel = scans[s].label;
                c.viaAlternative = false;
                c.normalPayable = normalOk;
                result.push_back(c);
            }
            if (altOk)
            {
                LegalActionsOracle::Cast c;
                c.card = card;
                c.zoneLabel = scans[s].label;
                c.viaAlternative = true;
                //When the normal MODE is restricted away (no normal entry),
                //the alternative must compete on its own: consumers use
                //normalPayable to prefer normal pricing, and a true here
                //with no normal entry would leave the card uncastable for
                //the heuristic (alternative-only cards like Astrolabe).
                c.normalPayable = normalOk && normalEntry;
                result.push_back(c);
            }
        }
    }

    return result;
}

//#W56-W (E-2). See LegalActions.h. Pure: no zone, ability or counter is
//mutated, so it is safe from rendering and from an isReactingToClick poll.
bool LegalActionsOracle::canPlayLandNow(MTGCardInstance * card, Player * actor)
{
    if (!card)
        return false;
    Player * ctrl = card->controller();
    if (!ctrl)
        return false;
    //Only the card's controller plays it; `actor` is whoever is clicking.
    if (actor && actor != ctrl)
        return false;
    //Own turn + a main phase + empty stack + no foreign interrupt. This is
    //MTGPutInPlayRule's own gate for a land, called on the same object.
    if (!card->StackIsEmptyandSorcerySpeed())
        return false;
    if (!ctrl->game || !ctrl->game->playRestrictions)
        return false;
    return ctrl->game->playRestrictions->landDropAvailable(card->getObserver(), card);
}

vector<LegalActionsOracle::Cast> LegalActionsOracle::legalLandPlays(Player * p)
{
    vector<LegalActionsOracle::Cast> result;
    std::set<string> seen;

    struct ZoneScan { MTGGameZone * zone; const char * label; };
    ZoneScan scans[] = {
        { p->game->hand, "" },
        { p->game->graveyard, " [from your graveyard]" },
        { p->game->exile, " [from exile]" },
        { p->game->commandzone, " [from your command zone]" },
    };
    for (size_t s = 0; s < sizeof(scans) / sizeof(scans[0]); s++)
    {
        MTGGameZone * zone = scans[s].zone;
        for (int i = 0; i < zone->nb_cards; i++)
        {
            MTGCardInstance * card = zone->cards[i];
            if (!card->isLand())
                continue;
            if (zone == p->game->graveyard
                && !card->has(Constants::CANPLAYFROMGRAVEYARD) && !card->has(Constants::TEMPFLASHBACK))
                continue;
            if (zone == p->game->exile && !card->has(Constants::CANPLAYFROMEXILE))
                continue;
            //N-152g: same as the spell rung above - CR 704.5j is a state-based
            //action, not a restriction on playing the land. A second Tolarian
            //Academy is a legal land drop; the SBA then bins one of them.
            if (p->game->playRestrictions->canPutIntoZone(card, p->game->inPlay) == PlayRestriction::CANT_PLAY)
                continue;
            //#W43-12. The card's OWN cast restrictions gate a land drop exactly
            //as they gate a spell (MTGPutInPlayRule::isReactingToClick calls
            //allowedToCast before anything else), and legalCasts above already
            //applies them - this rung did not, so it offered lands the engine
            //would never accept. The live case is the modal-DFC land: Hengegate
            //Pathway carries restriction=compare(isflipped)~equalto~0, so once
            //the pilot flips the display to Mistgate Pathway the card can no
            //longer be PLAYED as a land at all - its back face reaches the
            //battlefield through the autohand flip ABILITY instead. The seat
            //nevertheless offered "Play Mistgate Pathway", the model (which is
            //instructed to trust the legal list) took it, and the commit path
            //refused: two deferred_to_heuristic records for one land drop and
            //the drop still unused (corpus 20260824 deck152 vs146 s31-33).
            //An option the engine cannot honour is a lie to the pilot; do not
            //offer it.
            if (card->getRestrictions().size())
            {
                AbilityFactory af(p->getObserver());
                if (!af.parseCastRestrictions(card, p, card->getRestrictions()))
                    continue;
            }
            string key = card->getDisplayName() + scans[s].label;
            if (!seen.insert(key).second)
                continue;
            LegalActionsOracle::Cast c;
            c.card = card;
            c.zoneLabel = scans[s].label;
            c.viaAlternative = false;
            c.normalPayable = true;
            result.push_back(c);
        }
    }
    return result;
}

namespace
{
    bool isWrappedManaProducer(MTGAbility * a)
    {
        if (dynamic_cast<AManaProducer*>(a))
            return true;
        if (GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility*>(a))
        {
            if (dynamic_cast<AManaProducer*>(gmp->ability))
                return true;
            if (AForeach * fmp = dynamic_cast<AForeach*>(gmp->ability))
                if (dynamic_cast<AManaProducer*>(fmp->ability))
                    return true;
        }
        return false;
    }

    //Any activated ability of p's that could be used right now. Shared by
    //hasInstantResponse and hasAnyLegalAction so there is ONE definition of
    //"this ability is usable"; sorcerySpeedOk relaxes only the instant-speed
    //restriction filter, and every other test is identical either way.
    //#W53-S perf: `collect` turns the same single definition into a BATCH pass -
    //every source with a usable ability, in ONE walk of the action layer and ONE
    //potential-mana computation. GuiHandSelf::Update used to ask this predicate
    //once per battlefield permanent, so a 19-permanent board re-walked every
    //ability object 19 times and rebuilt the permissive potential 19 times, four
    //times a second (measured 1.57 ms/refresh on desktop = ~30 ms on the Vita's
    //444 MHz ARM). Collecting does not change ANY verdict: it is the same loop,
    //with `return true` replaced by "record this source and keep going".
    bool hasUsableActivatedAbility(Player * p, ManaCost * pMana, bool sorcerySpeedOk,
                                   MTGCardInstance * only = NULL,
                                   std::set<MTGCardInstance*> * collect = NULL)
    {
        GameObserver * g = p->getObserver();
        for (size_t i = 1; i < g->mLayers->actionLayer()->mObjects.size(); i++)
        {
            MTGAbility * a = (MTGAbility *) g->mLayers->actionLayer()->mObjects[i];
            ActivatedAbility * aa = dynamic_cast<ActivatedAbility*>(a);
            if (!aa || !aa->source)
                continue;
            //`only` narrows the same loop to one card so the per-card display
            //predicate cannot drift from the set-level one - there is still
            //exactly ONE definition of "this ability is usable".
            if (only && aa->source != only)
                continue;
            if (aa->source->controller() != p)
                continue;
            if (aa->source->isPhased)
                continue;
            //W50-MORPH: an ability whose source instance has been superseded
            //(the card flipped / re-entered: `next` points at the live copy) is
            //not on the board any more; and a morph "Face Up" ability is only
            //usable while its permanent is face-down.
            if (aa->source->next)
                continue;
            //W53-TRICKSTER: `{U}:morph` parses as a GenericActivatedAbility
            //WRAPPING the AAMorph (cost + inner), so the bare cast missed the
            //very ability the border was showing (Coral Trickster, face up).
            {
                AAMorph * am = dynamic_cast<AAMorph *>(aa);
                if (!am)
                    if (GenericActivatedAbility * gw = dynamic_cast<GenericActivatedAbility *>(aa))
                        am = dynamic_cast<AAMorph *>(gw->ability);
                if (am && !am->sourceIsFaceDown())
                    continue;
            }
            if (isWrappedManaProducer(aa))
                continue; //making mana is not a response
            //turn-scoped activations are wrong-turn regardless of speed
            if (aa->restrictions == MTGAbility::PLAYER_TURN_ONLY && g->currentPlayer != p)
                continue;
            if (aa->restrictions == MTGAbility::OPPONENT_TURN_ONLY && g->currentPlayer == p)
                continue;
            //instant-speed usability only: sorcery-scoped activations are not
            //responses at a priority window, but they ARE actions at a
            //sorcery-speed window of the player's own main phase
            if (!sorcerySpeedOk
                && aa->restrictions != MTGAbility::NO_RESTRICTION && aa->restrictions != MTGAbility::PLAYER_TURN_ONLY
                && aa->restrictions != MTGAbility::OPPONENT_TURN_ONLY)
                continue;
            if (aa->needsTapping && (aa->source->isTapped() || aa->source->hasSummoningSickness()))
                continue;
            ManaCost * cost = aa->getCost();
            if (cost && cost->getConvertedCost())
            {
                //W48-SELFTAP (owner live-play report, Vita, 2026-08-27: Westvale
                //Abbey's {5},{T} showed activatable as one of FIVE untapped
                //lands). `pMana` is the whole board's potential, the source's
                //own {T}: Add {1} included - but an ability that TAPS its
                //source cannot also tap it for mana. Subtract the source's own
                //producer output before pricing a tap ability.
                bool affordable;
                bool tapsSource = aa->needsTapping != 0;
                if (!tapsSource && cost->extraCosts)
                    for (size_t k = 0; k < cost->extraCosts->costs.size() && !tapsSource; k++)
                        if (dynamic_cast<TapCost *>(cost->extraCosts->costs[k]))
                            tapsSource = true; //{T} in an ability cost is a TapCost extra
                //W50-PERMISSIVE (owner live-play report, Vita, 2026-08-27: Ironroot
                //Warlord's {3}{G}{W} bordered usable over FOUR lands). The caller's
                //`pMana` is potentialManaPermissive, which SUMS every mana ability of
                //every card - a dual land (Selesnya Guildgate add{G} + add{W}) counts as
                //TWO mana, so four lands with one dual read as five. Price the ability
                //the way casts are priced: the strict one-ability-per-card potential
                //(the source itself excluded when the ability taps it), and when that
                //under-counts a dual's second colour, the colour-aware payment planner
                //(which excludes the source - conservative for a non-tap ability).
                {
                    ManaEngine::FreeProducerPolicy strictPolicy;
                    ManaCost * strict = ManaEngine::potentialMana(p, strictPolicy,
                        tapsSource ? aa->source : NULL);
                    strict->add(p->getManaPool());
                    affordable = strict->canAfford(cost, 0) != 0;
                    SAFE_DELETE(strict);
                    if (!affordable)
                        affordable = ManaEngine::planPayment(p, strictPolicy, aa->source, cost, 0).size() > 0;
                }
                if (!affordable)
                    continue;
            }
            //Non-mana costs gate usability too: removing counters the card
            //does not have, sacrificing with no legal fodder. Each cost's
            //canPay carries the ENGINE's semantics, not the CR's - LifeCost,
            //for one, allows paying down to (even past) zero, and the ring's
            //only contract is "the click will be accepted". The engine's
            //click gate asks ExtraCost::canPay after setExtraCostsAction, but
            //that binding NULLs storedCard on the source - a mutation a
            //display probe must not make - so bind only the source pointer
            //each cost's canPay reads (idempotent for an ability's own
            //source; canPay itself is read-only).
            if (cost && cost->extraCosts)
            {
                bool extrasPayable = true;
                for (size_t k = 0; k < cost->extraCosts->costs.size(); k++)
                {
                    ExtraCost * ec = cost->extraCosts->costs[k];
                    ec->setSource(aa->source);
                    if (!ec->canPay())
                    {
                        extrasPayable = false;
                        break;
                    }
                }
                if (!extrasPayable)
                    continue;
            }
            if (collect)
            {
                collect->insert(aa->source);
                continue;
            }
            return true;
        }
        return false;
    }
}

bool LegalActionsOracle::hasInstantResponse(Player * p)
{
    GameObserver * g = p->getObserver();
    ManaEngine::FreeProducerPolicy freePolicy;
    //PERMISSIVE potential: strict potentialMana counts one ability per
    //card, so a dual land only ever offered its FIRST color and payable
    //responses were invisible - a missed window costs a game, a spurious
    //one costs a single auto-answered ask (this predicate's documented
    //bias).
    ManaCost * pMana = ManaEngine::potentialManaPermissive(p, freePolicy);
    pMana->add(p->getManaPool());

    bool any = !legalCasts(p, freePolicy, pMana, true).empty();
    if (!any)
        any = hasUsableActivatedAbility(p, pMana, false);
    delete pMana;
    return any;
}

std::set<MTGCardInstance*> LegalActionsOracle::castableForDisplay(Player * p)
{
    std::set<MTGCardInstance*> out;
    GameObserver * g = p->getObserver();
    int phase = g->getCurrentGamePhase();
    bool sorcerySpeed = g->currentPlayer == p
        && (phase == MTG_PHASE_FIRSTMAIN || phase == MTG_PHASE_SECONDMAIN)
        && g->mLayers->stackLayer()->count(0, NOT_RESOLVED) == 0;

    ManaEngine::FreeProducerPolicy freePolicy;
    ManaCost * pMana = ManaEngine::potentialMana(p, freePolicy, NULL);
    pMana->add(p->getManaPool());
    vector<Cast> casts = legalCasts(p, freePolicy, pMana, !sorcerySpeed);
    delete pMana;
    for (size_t i = 0; i < casts.size(); i++)
        if (casts[i].zoneLabel.empty()) //display covers the hand only
            out.insert(casts[i].card);

    if (sorcerySpeed)
    {
        for (int i = 0; i < p->game->hand->nb_cards; i++)
        {
            MTGCardInstance * card = p->game->hand->cards[i];
            if (card->isLand()
                && p->game->playRestrictions->canPutIntoZone(card, p->game->inPlay) != PlayRestriction::CANT_PLAY)
                out.insert(card);
        }
    }

    //legalCasts dedupes by name (same name+zone = one DECISION), which is
    //right for the model's menu and wrong for a per-card display: only the
    //first of duplicate copies came back, and its twin rendered faded - a
    //false "uncastable" on a castable card. Same name in the same zone is
    //the same cast, so spread each verdict across the copies. Lands are
    //skipped: the loop above already judged every land instance itself.
    for (int i = 0; i < p->game->hand->nb_cards; i++)
    {
        MTGCardInstance * card = p->game->hand->cards[i];
        if (card->isLand() || out.count(card))
            continue;
        for (std::set<MTGCardInstance*>::iterator it = out.begin(); it != out.end(); ++it)
        {
            if (!(*it)->isLand() && (*it)->getDisplayName() == card->getDisplayName())
            {
                out.insert(card);
                break;
            }
        }
    }
    return out;
}

bool LegalActionsOracle::hasLegalAttacker(Player * p)
{
    //The engine already owns this predicate (the phase auto-skip uses it);
    //the oracle is the naming authority, not a second implementation.
    return p->hasPossibleAttackers();
}

bool LegalActionsOracle::hasLegalBlock(Player * defender)
{
    GameObserver * g = defender->getObserver();
    Player * attackerP = g->currentPlayer;
    if (attackerP == defender)
        return false;
    MTGGameZone * mine = defender->game->inPlay;
    for (int i = 0; i < mine->nb_cards; i++)
    {
        MTGCardInstance * blocker = mine->cards[i];
        if (!blocker || blocker->isPhased || !blocker->canBlock())
            continue;
        MTGCardInstance * attacker = NULL;
        while ((attacker = attackerP->game->inPlay->getNextAttacker(attacker)))
        {
            if (blocker->canBlock(attacker))
                return true;
        }
    }
    return false;
}

MTGCardInstance * LegalActionsOracle::illegalBlockDeclaration(Player * defender)
{
    if (!defender)
        return NULL;
    GameObserver * g = defender->getObserver();
    if (!g)
        return NULL;
    Player * attackerP = g->currentPlayer;
    if (!attackerP || attackerP == defender || !attackerP->game || !attackerP->game->inPlay)
        return NULL;
    MTGGameZone * z = attackerP->game->inPlay;
    for (int i = 0; i < z->nb_cards; i++)
    {
        MTGCardInstance * c = z->cards[i];
        if (c && c->isAttacker() && c->blockDeclarationIllegal())
            return c;
    }
    return NULL;
}

bool LegalActionsOracle::canDeclareBlocker(MTGCardInstance * card)
{
    //Mirrors MTGBlockRule::isReactingToClick (the click gate) plus the
    //owner's display requirement: a ring means "this creature can block AND
    //there is an attacker it could legally block" - a blocker with no legal
    //assignment would open a menu that cycles to nothing.
    Player * p = card->controller();
    GameObserver * g = p->getObserver();
    if (g->getCurrentGamePhase() != MTG_PHASE_COMBATBLOCKERS)
        return false;
    Player * attackerP = g->currentPlayer;
    if (attackerP == p)
        return false; //only the defending seat declares blockers
    if (g->isInterrupting)
        return false;
    //attack triggers resolve before blocks are declared - same stack gate
    //as the click rule
    if (g->mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED))
        return false;
    if (card->isPhased || !card->canBlock() || card->blockCost >= 1)
        return false;
    MTGCardInstance * attacker = NULL;
    while ((attacker = attackerP->game->inPlay->getNextAttacker(attacker)))
    {
        if (card->canBlock(attacker))
            return true;
    }
    return false;
}

bool LegalActionsOracle::hasAnyLegalAction(Player * p)
{
    GameObserver * g = p->getObserver();
    const int phase = g->getCurrentGamePhase();
    const bool myTurn = (g->currentPlayer == p);

    //A hand over the limit at cleanup is a MANDATORY discard - the most
    //load-bearing action there is, and it is neither a cast nor an ability.
    if (phase == MTG_PHASE_CLEANUP && p->game->hand->nb_cards > 7)
        return true;

    //Combat declarations are actions in their own right, not casts. "No
    //blocks" is a real declaration the owner makes deliberately, so a
    //defender with any legal block still gets the step.
    if (phase == MTG_PHASE_COMBATATTACKERS && myTurn && hasLegalAttacker(p))
        return true;
    if (phase == MTG_PHASE_COMBATBLOCKERS && !myTurn && hasLegalBlock(p))
        return true;

    //CR 509.2 / 510.1a: ordering the blockers of a multi-blocked attacker is
    //the ATTACKING player's choice, and like the two declarations above it is
    //neither a cast nor an activated ability - so this oracle used to answer
    //"nothing to do here" for an attacker with an empty hand, and
    //GameObserver's no-legal-action phase skip stepped straight over the ORDER
    //step. The blockers then kept their DECLARATION order, which means the
    //DEFENDER silently chose which of its own creatures the attacker's damage
    //killed. Live-observed on Vita (2026-08-24, "sometimes it's not allowing me
    //to assign damage order, and other times it lets me" - the two cases are
    //exactly "empty hand" vs "holding an instant", because an instant-speed
    //response was the only thing that used to keep the window open).
    //The condition mirrors GuiCombat's own ORDER arm (any of our attackers with
    //more than one blocker) so the oracle cannot disagree with the UI that
    //armed the step - GuiCombat already advances the step itself when nothing
    //is multi-blocked, and that skip stays exactly as it is.
    if (phase == MTG_PHASE_COMBATBLOCKERS && g->combatStep == ORDER && myTurn)
    {
        MTGGameZone * inPlay = p->game->inPlay;
        for (int i = 0; i < inPlay->nb_cards; i++)
        {
            MTGCardInstance * c = inPlay->cards[i];
            if (c && c->attacker && c->blockers.size() > 1)
                return true;
        }
    }

    //A sorcery-speed window of the player's own turn opens up everything the
    //rest of the game forbids: lands, sorceries, creatures, and abilities
    //that are only usable at sorcery speed.
    const bool sorcerySpeed = myTurn
        && (phase == MTG_PHASE_FIRSTMAIN || phase == MTG_PHASE_SECONDMAIN)
        && g->mLayers->stackLayer()->count(0, NOT_RESOLVED) == 0;
    if (sorcerySpeed)
    {
        if (!castableForDisplay(p).empty())
            return true;
        ManaEngine::FreeProducerPolicy freePolicy;
        ManaCost * pMana = ManaEngine::potentialManaPermissive(p, freePolicy);
        pMana->add(p->getManaPool());
        const bool ability = hasUsableActivatedAbility(p, pMana, true);
        delete pMana;
        if (ability)
            return true;
    }

    //Everything else reduces to "can this player respond at instant speed".
    return hasInstantResponse(p);
}

bool LegalActionsOracle::canDeclareAttacker(MTGCardInstance * card)
{
    //Mirrors MTGAttackRule::isReactingToClick's DECLARE branch exactly. Kept
    //deliberately verbatim rather than simplified: this border promises the
    //player that pressing the button will declare this creature, so the two
    //must not be able to disagree. An already-declared attacker is excluded -
    //it is no longer an available action, it is a state, and the engine
    //already shows that state.
    if (!card || !card->isCreature())
        return false;
    Player * p = card->controller();
    if (!p)
        return false;
    GameObserver * g = p->getObserver();
    if (!g || g->getCurrentGamePhase() != MTG_PHASE_COMBATATTACKERS)
        return false;
    if (p != g->currentPlayer || p != g->currentlyActing())
        return false;
    if (card->isPhased || card->isAttacker() || card->willattackpw)
        return false;
    return card->canAttack() && card->attackCost < 1;
}

bool LegalActionsOracle::hasUsableAbility(MTGCardInstance * card)
{
    if (!card)
        return false;
    Player * p = card->controller();
    if (!p)
        return false;
    ManaEngine::FreeProducerPolicy freePolicy;
    //Same permissive potential the response predicate uses: a dual land must
    //offer both colours or an affordable ability reads as unaffordable.
    ManaCost * pMana = ManaEngine::potentialManaPermissive(p, freePolicy);
    pMana->add(p->getManaPool());
    //sorcerySpeedOk is decided by the window, not the card: at the player's
    //own sorcery-speed window a sorcery-scoped activation IS available.
    GameObserver * g = p->getObserver();
    const bool sorcerySpeedOk = g && g->currentPlayer == p
        && !g->mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED)
        && (g->getCurrentGamePhase() == MTG_PHASE_FIRSTMAIN
            || g->getCurrentGamePhase() == MTG_PHASE_SECONDMAIN);
    bool any = hasUsableActivatedAbility(p, pMana, sorcerySpeedOk, card);
    delete pMana;
    return any;
}

std::set<MTGCardInstance*> LegalActionsOracle::usableAbilityCards(Player * p)
{
    std::set<MTGCardInstance*> out;
    if (!p)
        return out;
    ManaEngine::FreeProducerPolicy freePolicy;
    ManaCost * pMana = ManaEngine::potentialManaPermissive(p, freePolicy);
    pMana->add(p->getManaPool());
    GameObserver * g = p->getObserver();
    const bool sorcerySpeedOk = g && g->currentPlayer == p
        && !g->mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED)
        && (g->getCurrentGamePhase() == MTG_PHASE_FIRSTMAIN
            || g->getCurrentGamePhase() == MTG_PHASE_SECONDMAIN);
    hasUsableActivatedAbility(p, pMana, sorcerySpeedOk, NULL, &out);
    delete pMana;
    return out;
}
