#include "PrecompiledHeader.h"

#include "ManaEngine.h"
#include "Player.h"
#include "GameObserver.h"
#include "AllAbilities.h"
#include "ManaCostHybrid.h"
#include "ExtraCost.h"
#include <sstream>

int ManaEngine::FreeProducerPolicy::canHandle(MTGAbility * producer)
{
    //Auto-activation on a player's behalf is only safe for producers whose
    //activation carries no REAL extra cost (nothing to sacrifice/discard/
    //choose). Tapping/untapping the source itself is how a mana ability
    //works, not a cost to protect the player from - a plain {T} parses as
    //a TapCost ExtraCost, so an all-or-nothing extraCosts check would
    //reject every basic land.
    ManaCost * cost = producer->getCost();
    if (!cost || !cost->extraCosts)
        return 1;
    for (size_t i = 0; i < cost->extraCosts->costs.size(); i++)
    {
        ExtraCost * ec = cost->extraCosts->costs[i];
        if (!dynamic_cast<TapCost*>(ec) && !dynamic_cast<UnTapCost*>(ec))
            return 0;
    }
    return 1;
}


namespace
{
    //Player-parameterized replica of AManaProducer::isReactingToClick.
    //The real method evaluates game->currentlyActing(), which is the WRONG
    //subject when the engine probes a non-acting responder (priority
    //windows) or renders castability. checkCost=false mirrors the one
    //legacy call site that passed the producer's own cost as the pool.
    bool producerUsable(Player * p, AManaProducer * amp, MTGCardInstance * card, bool checkCost)
    {
        MTGCardInstance * source = amp->source;
        if (card != source)
            return false;
        if (amp->castRestriction.size())
        {
            AbilityFactory af(p->getObserver());
            if (!af.parseCastRestrictions(card, card->controller(), amp->castRestriction))
                return false;
        }
        if (amp->tap && (source->isTapped() || source->hasSummoningSickness()))
            return false;
        if (!p->game->inPlay->hasCard(source))
            return false;
        if (!(source->hasType(Subtypes::TYPE_LAND) || !amp->tap || !source->hasSummoningSickness()))
            return false;
        if (source->isPhased)
            return false;
        //Mirror the click layer's disable refusals (ActivatedAbility::
        //isReactingToClick, enforced on mana taps via AManaProducer::
        //reactToClick's base-class guard). Counting a producer the click
        //will refuse makes the cast oracle offer uncompletable casts: the
        //plan taps the real sources, stalls on the disabled one, floats
        //the mana, and the still-miscounted option re-arms every poll
        //(wave-20 deck102 Witch-of-the-Moors loop).
        if (source->has(Constants::NOACTIVATED) || source->has(Constants::NOMANA))
            return false;
        if (source->mutation && source->parentCards.size() > 0)
            return false;
        if (source->has(Constants::NOACTIVATEDTAP))
        {
            ManaCost * dcost = amp->getCost();
            if (amp->tap)
                return false;
            if (dcost && dcost->extraCosts)
                for (size_t k = 0; k < dcost->extraCosts->costs.size(); k++)
                    if (dynamic_cast<TapCost*>(dcost->extraCosts->costs[k]))
                        return false;
        }
        if (checkCost)
        {
            ManaCost * cost = amp->getCost();
            if (cost && !(p->getManaPool()->canAfford(cost, card->has(Constants::ANYTYPEOFMANAABILITY))
                          && (!cost->extraCosts || cost->extraCosts->canPay())))
                return false;
        }
        return true;
    }

    //Damage this mana producer deals to ITS OWN CONTROLLER when activated:
    //an "and!( damage:N controller )!" rider (Ancient Tomb 2, painlands 1).
    //0 when the rider damages someone else or is absent. A "rand" amount is
    //left unevaluated (it would draw the game RNG) and reported as 0.
    int producerSelfDamage(AManaProducer * amp)
    {
        AADamager * dmg = dynamic_cast<AADamager*>(amp->andAbility);
        if (!dmg)
            return 0;
        //who resolves the AADamager's victim: CONTROLLER/OWNER == the tapping
        //player (see ActivatedAbilityTP::getTarget). Any other value damages
        //someone else and is not a self-cost to warn about.
        if (dmg->who != TargetChooser::CONTROLLER && dmg->who != TargetChooser::OWNER)
            return 0;
        if (dmg->d.find("rand") != string::npos)
            return 0;
        int n = dmg->getDamage();
        return n > 0 ? n : 0;
    }

    //Greedy colored-pip assignment defers a FLEXIBLE source. A source that can
    //also pay a DIFFERENT still-needed color must not be spent on color k when
    //another (as-yet-unused) source can cover k instead. Without this, a dual
    //land scripted as two single-color mana abilities (Shineshadow Snarl:
    //`{T}:add{B}` THEN `{T}:add{W}`) has its FIRST-listed color claim the whole
    //source (used[] is per-source), and a mono source of that same color then
    //leaves the dual's OTHER color unpayable - so {W}{B} over Snarl + a black
    //source was wrongly judged unaffordable, denying the seat every gold cast
    //(N-146a; the same collapse the render showed as N-146d, wave-29). Deferring
    //the flexible source frees it for the color only it can supply. Order-
    //independent: whichever of the source's colors is still needed AND unique to
    //it gets taken; the redundant color goes to the dedicated source.
    bool deferFlexibleSource(Player * p, ManaEngine::ManaPolicy & policy,
                             ManaCost * cost, ManaCost * result,
                             map<MTGCardInstance*, bool> & used,
                             MTGCardInstance * card, int k)
    {
        ActionLayer * al = p->getObserver()->mLayers->actionLayer();
        bool sourceHasOtherNeeded = false;
        bool otherUnusedCoversK = false;
        for (size_t i = 0; i < al->manaObjects.size(); i++)
        {
            AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility*) al->manaObjects[i]);
            if (!amp || !policy.canHandle(amp) || !producerUsable(p, amp, amp->source, true))
                continue;
            if (amp->output->getConvertedCost() < 1)
                continue;
            if (amp->source == card)
            {
                for (int c = 1; c < Constants::NB_Colors; c++)
                    if (c != k && cost->hasColor(c) && amp->output->hasColor(c)
                        && result->getCost(c) < cost->getCost(c))
                        sourceHasOtherNeeded = true;
            }
            else if (!used[amp->source] && amp->output->hasColor(k))
            {
                //Only rely on the other source for k if it is DEDICATED to k for
                //this cost - it must NOT itself produce another still-needed
                //color. Otherwise two flexible sources could MUTUALLY defer and
                //strand the payment (two {W}/{B} duals paying {B}{W} must not
                //both defer: one takes B, the other W).
                bool competesElsewhere = false;
                for (int c = 1; c < Constants::NB_Colors; c++)
                    if (c != k && cost->hasColor(c) && amp->output->hasColor(c)
                        && result->getCost(c) < cost->getCost(c))
                        competesElsewhere = true;
                if (!competesElsewhere)
                    otherUnusedCoversK = true;
            }
        }
        return sourceHasOtherNeeded && otherUnusedCoversK;
    }
}

ManaCost * ManaEngine::potentialMana(Player * p, ManaPolicy & policy, MTGCardInstance * target)
{
    ManaCost * result = NEW ManaCost();

    map<MTGCardInstance *, bool> used;
    for (size_t i = 0; i < p->getObserver()->mLayers->actionLayer()->manaObjects.size(); i++)
    { 
        //Make sure we can use the ability
        MTGAbility * a = ((MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[i]);
        AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
        GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility*>(a);
        if(gmp && policy.canHandle(gmp))
        {
            //skip for each mana producers.
            AForeach * fmp = dynamic_cast<AForeach*>(gmp->ability);
            if(fmp)
            {
                amp = dynamic_cast<AManaProducer*> (fmp->ability);
                if(amp)
                {
                    used[fmp->source] = true;
                    continue;
                }
            }
        }
        if (amp && policy.canHandle(amp))
        {
            MTGCardInstance * card = amp->source;
            if (card == target)
                used[card] = true; //http://code.google.com/p/wagic/issues/detail?id=76
            if (!used[card] && producerUsable(p, amp, card, true) && amp->output->getConvertedCost() == 1)
            {//ai can't use cards which produce more than 1 converted while using the old pMana method.
                result->add(amp->output);
                used[card] = true;
            }
        }
    }
    return result;
}

vector<MTGAbility*> ManaEngine::planPayment(Player * p, ManaPolicy & policy, MTGCardInstance * target, ManaCost * cost, int anytypeofmana)
{
    if(!cost || (cost && !cost->getConvertedCost()) || !target)
        return vector<MTGAbility*>();
    map<MTGCardInstance*, bool> usedCards;

    return planPayment(p, policy, target, cost, anytypeofmana, usedCards, false);
}

vector<MTGAbility*> ManaEngine::planPayment(Player * p, ManaPolicy & policy, MTGCardInstance * target, ManaCost * _cost, int anytypeofmana, map<MTGCardInstance*,bool> &used, bool searchingAgain)
{
    ManaCost * cost = _cost;

    if(!cost->getConvertedCost())
        return vector<MTGAbility*>();
    ManaCost * result = NEW ManaCost();

    vector<MTGAbility*>payments = vector<MTGAbility*>();
    if (p->getManaPool()->getConvertedCost())
    {
        //adding the current manapool if any.
        result->add(p->getManaPool());
    }

    if(anytypeofmana){
        int convertedC = cost->getConvertedCost();
        cost = NEW ManaCost(ManaCost::parseManaCost("{0}", NULL, target));
        for (int jj = 0; jj < convertedC; jj++)
            cost->add(Constants::MTG_COLOR_ARTIFACT, 1);
    }

    int needColorConverted = cost->getConvertedCost() - int(cost->getCost(0)+cost->getCost(7));
    int fullColor = 0;
    //Generic mana spends OFF-PIP sources first. The walk is layer-ordered
    //(earliest-played lands first), so a {4}{u} cost used to swallow every
    //blue source into its generic fill before anything else was considered,
    //stranding exactly the colors a held reaction needs ("hold {u}{u} for
    //the counterspell" was unexecutable; wave-11 deck44 vs131 s30). Pass 0
    //lets the generic fill take only producers that pay NONE of the cost's
    //colored pips, and only up to the generic amount itself (an off-color
    //source that turns superfluous once the pips fill would be an overpay);
    //pass 1 takes anything, as before. The colored/hybrid branches are
    //guarded by used[] and result-vs-cost limits, so re-walking is a no-op
    //for them.
    int offColorFilled = 0;
    int genericAmount = int(cost->getCost(0) + cost->getCost(7));
    //Spare would-be ATTACKERS from the mana bill - PARTIALLY. Tapping a creature
    //that could attack this turn (a sliver under Gemhide, a mana dork) to pay a
    //cast that lands/rocks could have covered leaves the board with no untapped
    //attacker at COMBATATTACKERS, and the engine then correctly never offers the
    //declare-attackers step (live-observed, corpus 20260719 deck35 vs49: casting
    //Might Sliver {4}{g} over four lands tapped all three flyers and skipped the
    //swing). Build the SWING-NEUTRAL pool (mana pool + lands/rocks/sick/tapped
    //creatures), then draw in would-be attackers ONE AT A TIME, weakest first,
    //only while the bill is still short of affordable - and mark every attacker
    //the bill did not reach as used so the payment walk below never taps it. The
    //prior form was all-or-nothing (spare EVERY attacker iff neutrals covered the
    //WHOLE cost, else spare NONE), so a cost a single mana over the neutral supply
    //emptied the board of attackers when tapping one of them would have sufficed.
    //An {X} spell is EXCLUDED: there the AI wants to spend everything to maximize X,
    //so holding a mana-attacker back would shrink X (Death Wind {X}{B} over three
    //Leaden Myr must tap all three to reach X=2, not spare one and settle for X=0).
    if (!searchingAgain && !cost->hasX())
    {
        ManaCost * neutral = NEW ManaCost();
        neutral->add(p->getManaPool());
        map<MTGCardInstance*, bool> counted;
        vector<AManaProducer*> attackerProd; //deferred: only tapped if the bill needs it
        for (size_t z = 0; z < p->getObserver()->mLayers->actionLayer()->manaObjects.size(); z++)
        {
            MTGAbility * za = (MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[z];
            AManaProducer * zamp = dynamic_cast<AManaProducer*>(za);
            if (!zamp || !policy.canHandle(zamp) || !producerUsable(p, zamp, zamp->source, true))
                continue;
            MTGCardInstance * zsrc = zamp->source;
            if (zsrc == target || counted[zsrc])
                continue;
            counted[zsrc] = true;
            if (zsrc && zsrc->isCreature() && zsrc->canAttack())
                attackerProd.push_back(zamp);
            else
                neutral->add(zamp->output);
        }
        //Tap the WEAKEST would-be attackers first, so the strongest swingers are
        //the ones held back for combat.
        std::sort(attackerProd.begin(), attackerProd.end(),
            [](AManaProducer * a, AManaProducer * b) { return a->source->power < b->source->power; });
        size_t atkNeed = 0;
        while (!neutral->canAfford(cost, anytypeofmana) && atkNeed < attackerProd.size())
        {
            neutral->add(attackerProd[atkNeed]->output);
            ++atkNeed;
        }
        bool coversWithoutRest = neutral->canAfford(cost, anytypeofmana);
        SAFE_DELETE(neutral);
        //Spare exactly the attackers the bill never reached. When even every
        //attacker cannot cover the cost, spare none - correctness first.
        if (coversWithoutRest)
        {
            for (size_t k = atkNeed; k < attackerProd.size(); k++)
                used[attackerProd[k]->source] = true;
        }
    }
    for (int pass = 0; pass < 2; pass++)
    {
    for (size_t i = 0; i < p->getObserver()->mLayers->actionLayer()->manaObjects.size(); i++)
    {
        MTGAbility * a = ((MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[i]);
        AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
        if(amp && (amp->getCost() && amp->getCost()->extraCosts && !amp->getCost()->extraCosts->canPay()))
            continue;
        if((fullColor == needColorConverted || cost->hasColor(0) || cost->hasColor(7)) && result->getConvertedCost() < cost->getConvertedCost()) // Fixed a bug on colorless mana calculation for AI.
        {
            if((cost->hasColor(0) || cost->hasColor(7)) && amp)//find colorless after color mana.
            {
                if(result->canAfford(cost,0))
                    continue;
                if (pass == 0)
                {
                    if (offColorFilled >= genericAmount)
                        continue;
                    bool onPip = false;
                    for (int k = 1; k < Constants::NB_Colors && !onPip; k++)
                        if (k != Constants::MTG_COLOR_ARTIFACT && cost->getCost(k) && amp->output->hasColor(k))
                            onPip = true;
                    if (onPip)
                        continue; //keep it for the colored pips / pass 1
                }
                if (policy.canHandle(amp))
                {
                    MTGCardInstance * card = amp->source;
                    if (card == target)
                        used[card] = true; //http://code.google.com/p/wagic/issues/detail?id=76
                    if (!used[card] && producerUsable(p, amp, card, true) && amp->output->getConvertedCost() >= 1)
                    {
                        if(!(result->canAfford(cost,0)))//if we got to this point we should be filling colorless mana requirements.
                        {
                            payments.push_back(amp);
                            result->add(amp->output);
                            used[card] = true;
                            if (pass == 0)
                                offColorFilled++;
                        }
                    }
                }
            }
            continue;
        }
        GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility*>(a);
        if(gmp && policy.canHandle(gmp))
        {
            //for each mana producers.
            AForeach * fmp = dynamic_cast<AForeach*>(gmp->ability);
            if(fmp)
            {
                amp = dynamic_cast<AManaProducer*> (fmp->ability);
                if(amp)
                {
                    MTGCardInstance * fecard = gmp->source;
                    if (fecard == target)
                        used[fecard] = true; //http://code.google.com/p/wagic/issues/detail?id=76
                    if(gmp->getCost() && gmp->getCost()->getConvertedCost() > 0)
                    {//ai stil can't use cabal coffers and mana abilities which require mana payments effectively;
                        used[fecard];
                        continue;
                    }
                    if (!used[fecard] && gmp->isReactingToClick(fecard) && amp->output->getConvertedCost() >= 1 && (cost->getConvertedCost() > 1 || cost->hasX()))//wasteful to tap a potential big mana source for a single mana.
                    {
                        int outPut = fmp->checkActivation();
                        for(int k = 0;k < outPut;k++)
                            result->add(amp->output);
                        payments.push_back(gmp);
                        used[fecard] = true;
                    }
                }
            }
        }
        else if (amp && policy.canHandle(amp) && producerUsable(p, amp, amp->source, false))
        {
            for (int k = Constants::NB_Colors-1; k > 0 ; k--)//go backwards.
            {
                if (cost->hasColor(k) && amp->output->hasColor(k) && result->getCost(k) < cost->getCost(k))
                {
                    MTGCardInstance * card = amp->source;
                    if (card == target)
                        used[card] = true; //http://code.google.com/p/wagic/issues/detail?id=76
                    if (!used[card] && producerUsable(p, amp, card, true) && amp->output->getConvertedCost() >= 1)
                    {
                        //Don't burn a dual/flexible source on a color a dedicated
                        //source can pay - hold it for the color only it supplies.
                        //Only a single-color ability is deferrable; an add-both
                        //producer ({W}{B} at once) pays both pips in one tap.
                        if (amp->output->getConvertedCost() == 1
                            && deferFlexibleSource(p, policy, cost, result, used, card, k))
                            continue;
                        ManaCost * check = NEW ManaCost();
                        check->add(k,cost->getCost(k));
                        ManaCost * checkResult = NEW ManaCost();
                        checkResult->add(k,result->getCost(k));
                        if(!(checkResult->canAfford(check,0)))
                        {
                            payments.push_back(amp);
                            result->add(k,amp->output->getCost(k));
                            used[card] = true;
                            fullColor++;
                        }
                        SAFE_DELETE(check);
                        SAFE_DELETE(checkResult);
                    }
                }
            }
        }
    }
    //pass 1 only runs when the off-color-first pass left the total short.
    if (result->canAfford(cost, 0))
        break;
    }
    ManaCostHybrid * hybridCost;
    hybridCost = cost->getHybridCost(0);
    if(hybridCost)
    {
        int hyb = 0;
        while ((hybridCost = cost->getHybridCost(hyb)) != NULL)
        {
            //here we try to find one of the colors in the hybrid cost, it is done 1 at a time unfortunately
            //{rw}{ub} would be 2 runs of this.90% of the time ai finds it's hybrid in pMana check.
            bool foundColor1 = false;
            bool foundColor2 = false;
            for (size_t i = 0; i < p->getObserver()->mLayers->actionLayer()->manaObjects.size(); i++)
            {
                MTGAbility * a = ((MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[i]);
                AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
                if (amp && policy.canHandle(amp))
                {
                    foundColor1 = amp->output->hasColor(hybridCost->color1)?true:false;
                    foundColor2 = amp->output->hasColor(hybridCost->color2)?true:false;
                    if ((foundColor1 && result->getCost(hybridCost->color1) < hybridCost->value1)||
                        (foundColor2 && result->getCost(hybridCost->color2) < hybridCost->value2))
                    {
                        MTGCardInstance * card = amp->source;
                        if (card == target)
                            used[card] = true; //http://code.google.com/p/wagic/issues/detail?id=76
                        if (!used[card] && producerUsable(p, amp, card, true) && amp->output->getConvertedCost() >= 1)
                        {
                            ManaCost * check = NEW ManaCost();
                            check->add(foundColor1?hybridCost->color1:hybridCost->color2,foundColor1?hybridCost->value1:hybridCost->value2);
                            ManaCost * checkResult = NEW ManaCost();
                            checkResult->add(foundColor1?hybridCost->color1:hybridCost->color2,result->getCost(foundColor1?hybridCost->color1:hybridCost->color2));
                            if(((foundColor1 && !foundColor2)||(!foundColor1 && foundColor2)) &&!(checkResult->canAfford(check,0)))
                            {
                                payments.push_back(amp);
                                result->add(foundColor1?hybridCost->color1:hybridCost->color2,amp->output->getCost(foundColor1?hybridCost->color1:hybridCost->color2));
                                used[card] = true;
                                fullColor++;
                            }
                            SAFE_DELETE(check);
                            SAFE_DELETE(checkResult);
                        }
                    }
                }
            }
            hyb++;
        }
    }
    else if(!hybridCost && result->getConvertedCost())
    {
        ManaCost * check = NEW ManaCost();
        ManaCost * checkResult = NEW ManaCost();
        for (int k = 1; k < Constants::NB_Colors; k++)
        {
            check->add(k,cost->getCost(k));
            checkResult->add(k,result->getCost(k));
            if(!(checkResult->canAfford(check,0)))
            {
                SAFE_DELETE(check);
                SAFE_DELETE(checkResult);
                SAFE_DELETE(result);
                payments.clear();
                return payments;//we didn't meet one of the color cost requirements.
            }
        }
        if(cost->getKicker() && !searchingAgain)
        {

            ManaCost * withKickerCost= NEW ManaCost(cost->getKicker());
            vector<MTGAbility*>kickerPayment;
            bool keepLooking = true;
            while(keepLooking)
            {
                kickerPayment = planPayment(p, policy, target, withKickerCost, target->has(Constants::ANYTYPEOFMANA), used, true);
                if(kickerPayment.size())
                {
                    for(unsigned int w = 0;w < kickerPayment.size();++w)
                    {
                        if(used[kickerPayment[w]->source])
                        {
                            payments.push_back(kickerPayment[w]);
                        }
                    }
                    keepLooking = cost->getKicker()->isMulti;
                }
                else
                    keepLooking = false;
            }
            SAFE_DELETE(withKickerCost);
        }
        SAFE_DELETE(check);
        SAFE_DELETE(checkResult);
    }
    if(cost->hasX())
    {
        //if we decided to play an "x" ability/card, lets go all out, these effects tend to be game winners.
        //add the rest of the mana.
        for (size_t i = 0; i < p->getObserver()->mLayers->actionLayer()->manaObjects.size(); i++)
        { 
            MTGAbility * a = ((MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[i]);
            AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
            if (amp && policy.canHandle(amp))
            {
                if (!used[amp->source] && producerUsable(p, amp, amp->source, true) && amp->output->getConvertedCost() >= 1)
                {
                    payments.push_back(amp);
                }
            }
        }
    }
    if(!result->canAfford(cost,0))
        payments.clear();
    SAFE_DELETE(result);
    if(anytypeofmana)
        SAFE_DELETE(cost);
    return payments;
}


ManaCost * ManaEngine::potentialManaPermissive(Player * p, ManaPolicy & policy)
{
    ManaCost * result = NEW ManaCost();
    for (size_t i = 0; i < p->getObserver()->mLayers->actionLayer()->manaObjects.size(); i++)
    {
        MTGAbility * a = ((MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[i]);
        AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
        if (amp && policy.canHandle(amp) && producerUsable(p, amp, amp->source, true)
            && amp->output->getConvertedCost() >= 1)
            result->add(amp->output);
    }
    return result;
}

int ManaEngine::potentialColorReach(Player * p, ManaPolicy & policy, ManaCost * outColors)
{
    map<MTGCardInstance *, bool> seen;
    ActionLayer * al = p->getObserver()->mLayers->actionLayer();
    for (size_t i = 0; i < al->manaObjects.size(); i++)
    {
        AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility*) al->manaObjects[i]);
        if (!amp || !policy.canHandle(amp) || !producerUsable(p, amp, amp->source, true))
            continue;
        if (amp->output->getConvertedCost() < 1)
            continue;
        seen[amp->source] = true;
        if (outColors)
            for (int c = 0; c < Constants::NB_Colors; c++)
                if (amp->output->hasColor(c) && outColors->getCost(c) < 1)
                    outColors->add(c, 1);
    }
    return (int) seen.size();
}

vector<MTGAbility*> ManaEngine::selectAutoTapProducers(Player * p, MTGCardInstance * target, ManaCost * cost, int anytypeofmana)
{
    vector<MTGAbility*> picks;
    if (!cost || !cost->getConvertedCost())
        return picks;
    ManaCost * sim = NEW ManaCost(p->getManaPool());
    if (sim->canAfford(cost, anytypeofmana))
    {
        delete sim;
        return picks;
    }
    FreeProducerPolicy freePolicy;
    vector<MTGAbility*> plan = planPayment(p, freePolicy, target, cost, anytypeofmana);
    //Two passes over the plan: producers whose single-color output pays a
    //COLORED symbol the simulated pool does not cover yet go first, generic
    //fillers after. The raw plan is layer-ordered and can front-load
    //wrong-color fillers - consuming it blindly overpaid ({1}{G} tapping
    //two Mountains before the Forest, floating the extra red).
    bool covered = false;
    for (int pass = 0; pass < 2 && !covered; pass++)
    {
        for (size_t i = 0; i < plan.size() && !covered; i++)
        {
            if (!plan[i])
                continue;
            if (sim->canAfford(cost, anytypeofmana))
            {
                covered = true;
                continue;
            }
            AManaProducer * amp = dynamic_cast<AManaProducer *>(plan[i]);
            if (pass == 0)
            {
                int color = 0;
                if (amp && amp->output)
                    for (int k = 1; k < Constants::NB_Colors && !color; k++)
                        if (amp->output->getCost(k))
                            color = k;
                if (!color || sim->getCost(color) >= cost->getCost(color))
                    continue; //not a still-needed colored symbol; retry as filler
            }
            picks.push_back(plan[i]);
            if (amp && amp->output)
                sim->add(amp->output);
            else
                sim->add(Constants::MTG_COLOR_ARTIFACT, 1); //wrapped producer: approximate
            plan[i] = NULL; //selected; never pick a producer twice
        }
    }
    delete sim;
    return picks;
}

void ManaEngine::autoTapForCost(Player * p, MTGCardInstance * target, ManaCost * cost, int anytypeofmana)
{
    if (!cost || !cost->getConvertedCost())
        return;
    if (p->getManaPool()->canAfford(cost, anytypeofmana))
        return;
    vector<MTGAbility*> picks = selectAutoTapProducers(p, target, cost, anytypeofmana);
    for (size_t i = 0; i < picks.size(); i++)
    {
        if (p->getManaPool()->canAfford(cost, anytypeofmana))
            break;
        p->getObserver()->cardClick(picks[i]->source, picks[i]);
    }
}

int ManaEngine::maxAnnounceableX(Player * p, ManaCost * baseWithX, int anytypeofmana)
{
    if (!p || !baseWithX)
        return 0;
    struct AnyProducerPolicy : ManaPolicy
    {
        int canHandle(MTGAbility *) { return 1; }
    } anyPolicy;
    ManaCost * potential = potentialMana(p, anyPolicy, NULL);
    potential->add(p->getManaPool());
    int avail;
    if (baseWithX->xColor > 0 && !anytypeofmana)
        avail = potential->getCost(baseWithX->xColor) - baseWithX->getCost(baseWithX->xColor);
    else
        avail = potential->getConvertedCost() - baseWithX->getConvertedCost();
    delete potential;

    //CONVOKE credits the X cap. Per CR 702.51, each creature tapped for convoke
    //pays {1} of GENERIC mana or one mana of that creature's colour, and generic
    //mana pays the {X} portion of an X-spell. So an X-spell with convoke can
    //reach an X ABOVE what the mana pool alone affords, by the number of
    //untapped creatures available to convoke. Convoke::offerable() already
    //credits this when deciding to OFFER the cast; if the X cap does NOT, the
    //engine offers a convoke X-spell but then builds only the X=0 option, so no
    //ANNOUNCE_X ask ever fires and the offered cast silently no-ops (deck137
    //March of the Multitudes: OFFERED every game, resolved to tokens only when
    //the mana pool alone happened to reach X>=1). The downstream payment already
    //supports it: after X is announced the cost is resolved to {base}+{X generic}
    //and Convoke::getReduction/doPay reduce the generic pips with tapped
    //creatures (vs152 t12 X=3 -> 3 Soldiers). Only a GENERIC X benefits: a strict
    //coloured X (xColor>0, !anytypeofmana) is not helped by off-colour convoke
    //mana, so leave that branch uncredited (never OVER-offer an unpayable X).
    if ((baseWithX->xColor <= 0 || anytypeofmana) && baseWithX->extraCosts && p->game && p->game->inPlay)
    {
        bool hasConvoke = false;
        for (size_t i = 0; i < baseWithX->extraCosts->costs.size(); i++)
        {
            if (dynamic_cast<Convoke*>(baseWithX->extraCosts->costs[i]))
            {
                hasConvoke = true;
                break;
            }
        }
        if (hasConvoke)
        {
            int convokers = 0;
            MTGGameZone * z = p->game->inPlay;
            for (int c = 0; c < z->nb_cards; c++)
            {
                MTGCardInstance * cc = z->cards[c];
                if (cc && cc->isCreature() && !cc->isTapped() && !cc->isPhased)
                    convokers++;
            }
            avail += convokers;
        }
    }

    if (avail < 0)
        avail = 0;
    if (avail > 20)
        avail = 20;
    return avail;
}

int ManaEngine::selfDamageOnTap(MTGCardInstance * card)
{
    if (!card)
        return 0;
    Player * p = card->controller();
    if (!p)
        return 0;
    ActionLayer * al = p->getObserver()->mLayers->actionLayer();
    int worst = 0;
    for (size_t i = 0; i < al->manaObjects.size(); i++)
    {
        AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility *) al->manaObjects[i]);
        if (!amp || amp->source != card)
            continue;
        if (!producerUsable(p, amp, card, true))
            continue;
        int n = producerSelfDamage(amp);
        if (n > worst)
            worst = n;
    }
    return worst;
}

vector<string> ManaEngine::selfDamageManaSources(Player * p)
{
    vector<string> out;
    if (!p)
        return out;
    ActionLayer * al = p->getObserver()->mLayers->actionLayer();
    map<MTGCardInstance *, int> worst; //per source card, largest self-damage
    for (size_t i = 0; i < al->manaObjects.size(); i++)
    {
        AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility *) al->manaObjects[i]);
        if (!amp)
            continue;
        MTGCardInstance * card = amp->source;
        if (!card || card->controller() != p)
            continue;
        if (!producerUsable(p, amp, card, true))
            continue;
        int n = producerSelfDamage(amp);
        if (n <= 0)
            continue;
        map<MTGCardInstance *, int>::iterator it = worst.find(card);
        if (it == worst.end() || n > it->second)
            worst[card] = n;
    }
    for (map<MTGCardInstance *, int>::iterator it = worst.begin(); it != worst.end(); ++it)
    {
        std::ostringstream o;
        o << it->first->getDisplayName() << ": " << it->second << " damage";
        out.push_back(o.str());
    }
    return out;
}
