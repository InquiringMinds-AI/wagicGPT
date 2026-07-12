#include "PrecompiledHeader.h"

#include "LegalActions.h"
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
    if (cost->getAlternative() && pMana->canAfford(cost->getAlternative(), 0))
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
                && !card->has(Constants::ASFLASH))
                continue;
            if (zone == p->game->graveyard
                && !card->has(Constants::CANPLAYFROMGRAVEYARD) && !card->has(Constants::TEMPFLASHBACK)
                && !(card->getManaCost() && card->getManaCost()->getFlashback())
                && !(card->getManaCost() && card->getManaCost()->getRetrace()))
                continue;
            if (zone == p->game->exile && !card->has(Constants::CANPLAYFROMEXILE))
                continue;
            if (card->hasType(Subtypes::TYPE_LEGENDARY) && p->game->inPlay->findByName(card->name))
                continue;
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
            bool altOk = zone == p->game->hand && cost && cost->getAlternative()
                && (pMana->canAfford(cost->getAlternative(), 0)
                    || ManaEngine::planPayment(p, policy, card, cost->getAlternative(), 0).size());

            if (normalOk || !altOk)
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
                c.normalPayable = normalOk;
                result.push_back(c);
            }
        }
    }

    return result;
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
            if (card->hasType(Subtypes::TYPE_LEGENDARY) && p->game->inPlay->findByName(card->name))
                continue;
            if (p->game->playRestrictions->canPutIntoZone(card, p->game->inPlay) == PlayRestriction::CANT_PLAY)
                continue;
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

    for (size_t i = 1; !any && i < g->mLayers->actionLayer()->mObjects.size(); i++)
    {
        MTGAbility * a = (MTGAbility *) g->mLayers->actionLayer()->mObjects[i];
        ActivatedAbility * aa = dynamic_cast<ActivatedAbility*>(a);
        if (!aa || !aa->source)
            continue;
        if (aa->source->controller() != p)
            continue;
        if (aa->source->isPhased)
            continue;
        if (isWrappedManaProducer(aa))
            continue; //making mana is not a response
        //instant-speed usability only: phase- and sorcery-scoped
        //activations are not responses at a priority window
        if (aa->restrictions == MTGAbility::PLAYER_TURN_ONLY && g->currentPlayer != p)
            continue;
        if (aa->restrictions == MTGAbility::OPPONENT_TURN_ONLY && g->currentPlayer == p)
            continue;
        if (aa->restrictions != MTGAbility::NO_RESTRICTION && aa->restrictions != MTGAbility::PLAYER_TURN_ONLY
            && aa->restrictions != MTGAbility::OPPONENT_TURN_ONLY)
            continue;
        if (aa->needsTapping && (aa->source->isTapped() || aa->source->hasSummoningSickness()))
            continue;
        ManaCost * cost = aa->getCost();
        if (cost && cost->getConvertedCost() && !pMana->canAfford(cost, 0))
            continue;
        any = true;
    }
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
