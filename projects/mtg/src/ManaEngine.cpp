#include "PrecompiledHeader.h"

#include "ManaEngine.h"
#include "Player.h"
#include "GameObserver.h"
#include "AllAbilities.h"
#include "ManaCostHybrid.h"
#include "ExtraCost.h"
#include "TargetChooser.h"
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

//SPEND-RESTRICTED MANA (CR 106.6b), the honor point for the card-script
//`manarestriction{<tc spec>}` clause: "Spend this mana only to cast <spec>
//spells". `payee` is the card the mana is about to pay for.
//
//Before this existed the restriction had NO engine representation - the card
//scripts approximated it with an ACTIVATION gate
//(`this(variable{type:creature:myrestrictedcastingzone}>0)`, "only tap me if a
//creature is castable"), which is a different claim: once the mana reached the
//pool it was ordinary mana and paid for anything. Live play, 2026-08-24:
//Beastcaller Savant's mana paid for Captain's Claws, an Equipment.
//
//"Only to cast a SPELL" is decided here by where the payee lives: a spell is a
//card that is not (yet) a permanent, so a payee already on the battlefield is
//an ABILITY ACTIVATION and never a legal use of this mana. A NULL payee (the
//context-free "what could I make" estimate) is likewise refused - understating
//an unrestricted pool is safe, because every payment path re-asks with the
//actual payee and gets the restricted producer back if it qualifies.
bool ManaEngine::spendAllowed(MTGAbility * producer, MTGCardInstance * payee)
{
    AManaProducer * amp = dynamic_cast<AManaProducer*>(producer);
    if (!amp || amp->spendRestriction.empty())
        return true;
    if (!payee)
        return false;
    Player * pc = payee->controller();
    if (pc && pc->game && pc->game->inPlay->hasCard(payee))
        return false;
    GameObserver * g = payee->getObserver();
    if (!g)
        return false;
    TargetChooserFactory tcf(g);
    TargetChooser * tc = tcf.createTargetChooser(amp->spendRestriction, amp->source);
    if (!tc)
        return false;
    //The payee is in hand / graveyard / exile / command zone / on the stack,
    //never the battlefield the spec would default to.
    tc->setAllZones();
    bool ok = tc->canTarget(payee, true);
    delete tc;
    return ok;
}


namespace
{
    //Player-parameterized replica of AManaProducer::isReactingToClick.
    //The real method evaluates game->currentlyActing(), which is the WRONG
    //subject when the engine probes a non-acting responder (priority
    //windows) or renders castability. checkCost=false mirrors the one
    //legacy call site that passed the producer's own cost as the pool.
    //`payee` + `enforceSpend`: the SPEND-RESTRICTION honor point. Every
    //producer-SELECTION walk (potentialMana, planPayment) passes the card the
    //mana would pay for and enforces; the display/reach walks
    //(potentialManaPermissive, potentialColorReach, selfDamage*) do not - they
    //report what the board can make, not what a given cost may spend.
    bool producerUsable(Player * p, AManaProducer * amp, MTGCardInstance * card, bool checkCost,
                        MTGCardInstance * payee = NULL, bool enforceSpend = false)
    {
        MTGCardInstance * source = amp->source;
        if (card != source)
            return false;
        if (enforceSpend && !ManaEngine::spendAllowed(amp, payee))
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

    //Can this SOURCE CARD, across EVERY usable mana ability printed on it,
    //still supply a color this cost needs OTHER than k? Both halves of the
    //deferral test below ask exactly this question, so both must ask it of the
    //whole CARD. Asking it of a single ability is what deadlocked N-152f: a
    //dual scripted as two separate single-color abilities answers "no" through
    //whichever ability the caller happens to be holding, and so looks dedicated
    //to one color when the card is nothing of the kind.
    bool cardCoversOtherNeededColor(Player * p, ManaEngine::ManaPolicy & policy,
                                    ManaCost * cost, ManaCost * result,
                                    MTGCardInstance * sourceCard, int k,
                                    MTGCardInstance * payee)
    {
        ActionLayer * al = p->getObserver()->mLayers->actionLayer();
        for (size_t i = 0; i < al->manaObjects.size(); i++)
        {
            AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility*) al->manaObjects[i]);
            if (!amp || amp->source != sourceCard)
                continue;
            if (!policy.canHandle(amp) || !producerUsable(p, amp, amp->source, true, payee, true))
                continue;
            if (amp->output->getConvertedCost() < 1)
                continue;
            for (int c = 1; c < Constants::NB_Colors; c++)
                if (c != k && cost->hasColor(c) && amp->output->hasColor(c)
                    && result->getCost(c) < cost->getCost(c))
                    return true;
        }
        return false;
    }

    //Does this SOURCE CARD, through any usable mana ability printed on it,
    //make colour k?
    bool cardMakesColour(Player * p, ManaEngine::ManaPolicy & policy,
                         MTGCardInstance * sourceCard, MTGCardInstance * payee, int k)
    {
        ActionLayer * al = p->getObserver()->mLayers->actionLayer();
        for (size_t i = 0; i < al->manaObjects.size(); i++)
        {
            AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility*) al->manaObjects[i]);
            if (!amp || amp->source != sourceCard || !amp->output)
                continue;
            if (!policy.canHandle(amp) || !producerUsable(p, amp, amp->source, true, payee, true))
                continue;
            if (amp->output->hasColor(k))
                return true;
        }
        return false;
    }

    //How many distinct colours (colourless excluded) can this SOURCE CARD make
    //across every usable mana ability printed on it? 1 = a mono source.
    int cardColourCount(Player * p, ManaEngine::ManaPolicy & policy,
                        MTGCardInstance * sourceCard, MTGCardInstance * payee)
    {
        ActionLayer * al = p->getObserver()->mLayers->actionLayer();
        int mask = 0;
        for (size_t i = 0; i < al->manaObjects.size(); i++)
        {
            AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility*) al->manaObjects[i]);
            if (!amp || amp->source != sourceCard || !amp->output)
                continue;
            if (!policy.canHandle(amp) || !producerUsable(p, amp, amp->source, true, payee, true))
                continue;
            for (int c = 1; c < Constants::NB_Colors && c < 32; c++)
                if (c != Constants::MTG_COLOR_LAND && c != Constants::MTG_COLOR_WASTE
                    && amp->output->hasColor(c))
                    mask |= (1 << c);
        }
        int n = 0;
        for (int c = 0; c < 32; c++)
            if (mask & (1 << c))
                n++;
        return n;
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
                             MTGCardInstance * card, int k,
                             MTGCardInstance * payee)
    {
        ActionLayer * al = p->getObserver()->mLayers->actionLayer();
        //#W51-B (wave-50 D-2, the colour-only shape): a MULTI-colour source
        //must not pay colour k while an unused MONO source of k can. The
        //original test below only defers a source that covers another colour
        //THIS cost still needs, so a lone {w} pip over Glacial Fortress + Plains
        //took the Fortress whenever it came first in layer order (`Paid {w} for
        //Path to Exile with Seachrome Coast`, five Plains/Islands untapped -
        //deck125 vs152 seq35; `Paid {b} for Bloodline Keeper with Underground
        //Sea`, Plains untapped - deck123 seq44). A mono source of k is
        //DOMINATED by any multi source that makes k: whatever the rest of the
        //cost or the rest of the turn needs, the multi source can still supply
        //it and the mono source cannot, so spending the mono first never costs
        //an option. Deadlock-free by construction: the source we defer TO is
        //mono, and a mono source has nothing to defer to. Leaves every earlier
        //protection intact - on an all-flexible manabase (N-152f) there is no
        //mono source, so this branch never fires and the N-146a test decides
        //exactly as before.
        if (cardColourCount(p, policy, card, payee) > 1)
        {
            for (size_t i = 0; i < al->manaObjects.size(); i++)
            {
                AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility*) al->manaObjects[i]);
                if (!amp || amp->source == card || used[amp->source])
                    continue;
                if (!policy.canHandle(amp) || !producerUsable(p, amp, amp->source, true, payee, true))
                    continue;
                if (amp->output->getConvertedCost() != 1 || !amp->output->hasColor(k))
                    continue;
                if (cardColourCount(p, policy, amp->source, payee) == 1)
                    return true;
            }
        }
        //SELF test: this source can also pay a different still-needed color, so
        //spending it on k might be the wrong use of it.
        bool sourceHasOtherNeeded = cardCoversOtherNeededColor(p, policy, cost, result, card, k, payee);
        if (!sourceHasOtherNeeded)
            return false;
        bool otherUnusedCoversK = false;
        for (size_t i = 0; i < al->manaObjects.size(); i++)
        {
            AManaProducer * amp = dynamic_cast<AManaProducer*>((MTGAbility*) al->manaObjects[i]);
            if (!amp || !policy.canHandle(amp) || !producerUsable(p, amp, amp->source, true, payee, true))
                continue;
            if (amp->output->getConvertedCost() < 1)
                continue;
            if (amp->source == card)
                continue;
            if (!used[amp->source] && amp->output->hasColor(k))
            {
                //Only rely on the other source for k if it is DEDICATED to k for
                //this cost - it must NOT itself produce another still-needed
                //color. Otherwise two flexible sources could MUTUALLY defer and
                //strand the payment (two {W}/{B} duals paying {B}{W} must not
                //both defer: one takes B, the other W).
                //
                //N-152f: this used to be judged on amp->output alone, i.e. on
                //the ONE ability the loop was holding, while the self test above
                //was judged over the whole card. On an all-flexible manabase the
                //asymmetry inverted the guard into the very deadlock it was
                //written to prevent: four Deserted Beach ({T}:add{W} THEN
                //{T}:add{U}) reached through their add{W} ability each reported
                //hasColor(U) == false, were each judged "dedicated to W", and so
                //every Beach deferred every pip to every other Beach. Nothing
                //claimed a pip, the walk returned empty, and Teferi {2}{U}{W}
                //was never offered across six windows (deck152 vs116, s17-s30).
                //Judging the whole CARD makes the two halves symmetric. It is
                //strictly LESS deferral, so it cannot resurrect the over-offer
                //shape, and it leaves the N-146a case that motivated deferral
                //untouched - a genuine mono source has one ability, and
                //aggregating over one ability changes nothing.
                if (!cardCoversOtherNeededColor(p, policy, cost, result, amp->source, k, payee))
                    otherUnusedCoversK = true;
            }
        }
        return sourceHasOtherNeeded && otherUnusedCoversK;
    }

    //#W54-F: does this permanent have a tap ability that is NOT mana production?
    //Tapping such a card for mana costs its controller the OTHER ability too, so
    //a payment plan that can pay with a plain source must not spend it (owner
    //Vita report 2026-09-01: "With 1 plains, 3 forests, and a squirrel nest
    //enchanted to a forest, autotap doesn't give a fuck, and all tap that
    //squirrel nest forest to cast a yavimaya enchantress, leaving me a useless
    //forest that cannot tap to make a squirrel").
    //
    //The abilities counted are the ones the CLICK layer would offer: any
    //ActivatedAbility whose source is this card, is not a mana producer in any
    //of its three wrapper shapes, and whose cost taps the source. The tap test
    //reads BOTH needsTapping and a TapCost in extraCosts - a taught ability
    //(`teach(land) {T}:_SQUIRRELTOKEN_`) arrives as needsTapping=0 with a
    //zero-converted cost carrying a TapCost, so needsTapping alone misses
    //exactly the case the owner reported.
    //
    //Only a TAP ability counts: an ability the card can still use while tapped
    //loses nothing when the card is tapped for mana, so it must not bias the
    //plan.
    bool sourceHasNonManaTapAbility(Player * p, MTGCardInstance * card)
    {
        if (!p || !card)
            return false;
        ActionLayer * al = p->getObserver()->mLayers->actionLayer();
        for (size_t i = 0; i < al->mObjects.size(); i++)
        {
            ActivatedAbility * aa = dynamic_cast<ActivatedAbility*>((MTGAbility *) al->mObjects[i]);
            if (!aa || aa->source != card)
                continue;
            if (aa->source->next) //superseded instance (flipped / re-entered)
                continue;
            //mana production in each of the three wrapper shapes the engine
            //builds (bare, GenericActivatedAbility, foreach-wrapped) - the same
            //set LegalActionsOracle::isWrappedManaProducer skips.
            if (dynamic_cast<AManaProducer*>((MTGAbility *) aa))
                continue;
            bool mana = false;
            if (GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility*>(aa))
            {
                if (dynamic_cast<AManaProducer*>(gmp->ability))
                    mana = true;
                else if (AForeach * fmp = dynamic_cast<AForeach*>(gmp->ability))
                    if (dynamic_cast<AManaProducer*>(fmp->ability))
                        mana = true;
            }
            if (mana)
                continue;
            bool taps = aa->needsTapping != 0;
            ManaCost * acost = aa->getCost();
            if (!taps && acost && acost->extraCosts)
                for (size_t k = 0; k < acost->extraCosts->costs.size() && !taps; k++)
                    if (dynamic_cast<TapCost*>(acost->extraCosts->costs[k]))
                        taps = true;
            if (taps)
                return true;
        }
        return false;
    }

    //#W49-D4: the generic-fill order (see planPayment). Every usable producer
    //ability of the paying player, stably sorted by its SOURCE CARD's key:
    //UTILITY ASC (#W54-F: a source whose only tap ability is mana production
    //taps before one that also has a non-mana tap ability), then scarcity DESC
    //(min over the card's colours of how many of the player's source cards make
    //that colour, tapped or not; colourless = unbounded), then colour count ASC,
    //then layer order. Abilities of one card stay adjacent, most-abundant
    //colour first (#W51-B).
    std::vector<MTGAbility*> genericFillOrder(Player * p, ManaEngine::ManaPolicy & policy,
                                              MTGCardInstance * payee)
    {
        ActionLayer * al = p->getObserver()->mLayers->actionLayer();
        std::map<MTGCardInstance*, int> colourMask; //per source card, bit k = makes colour k
        std::vector<MTGAbility*> order;
        for (size_t i = 0; i < al->manaObjects.size(); i++)
        {
            MTGAbility * a = (MTGAbility *) al->manaObjects[i];
            AManaProducer * amp = dynamic_cast<AManaProducer*>(a);
            if (!amp || !amp->source || !amp->output || !policy.canHandle(amp))
                continue;
            if (amp->source->controller() != p)
                continue;
            int & mask = colourMask[amp->source];
            for (int k = 1; k < Constants::NB_Colors; k++)
                if (k != Constants::MTG_COLOR_LAND && k != Constants::MTG_COLOR_WASTE
                    && amp->output->hasColor(k))
                    mask |= (1 << k);
            order.push_back(a);
        }
        int perColour[32] = { 0 };
        for (std::map<MTGCardInstance*, int>::iterator it = colourMask.begin(); it != colourMask.end(); ++it)
            for (int k = 1; k < Constants::NB_Colors && k < 32; k++)
                if (it->second & (1 << k))
                    perColour[k]++;
        struct Key
        {
            int utility;  //#W54-F: 1 = the card has a non-mana tap ability; tap it LAST
            int scarcity; //higher = safer to tap
            int colours;  //fewer = less flexible = tap first
            size_t layer;
        };
        std::map<MTGCardInstance*, Key> keyOf;
        for (size_t i = 0; i < order.size(); i++)
        {
            MTGCardInstance * src = ((AManaProducer *) order[i])->source;
            if (keyOf.find(src) != keyOf.end())
                continue;
            Key key;
            key.utility = sourceHasNonManaTapAbility(p, src) ? 1 : 0;
            key.scarcity = 1 << 20; //colourless: nothing to strand
            key.colours = 0;
            key.layer = i;
            int mask = colourMask[src];
            for (int k = 1; k < Constants::NB_Colors && k < 32; k++)
                if (mask & (1 << k))
                {
                    key.colours++;
                    if (perColour[k] < key.scarcity)
                        key.scarcity = perColour[k];
                }
            keyOf[src] = key;
        }
        //Within one card, the ability whose colour is most ABUNDANT goes
        //first, so a dual spent on generic floats/produces the colour the
        //board has most of and never its scarce one (Scrubland paying {1}
        //over a black-heavy board adds {B}, not {W}). Colourless output
        //ranks as abundant.
        struct Less
        {
            std::map<MTGCardInstance*, Key> * keys;
            const int * perColour;
            int abundance(MTGAbility * a) const
            {
                int best = 1 << 20;
                AManaProducer * amp = (AManaProducer *) a;
                bool any = false;
                for (int k = 1; k < Constants::NB_Colors && k < 32; k++)
                    if (k != Constants::MTG_COLOR_LAND && k != Constants::MTG_COLOR_WASTE
                        && amp->output->hasColor(k))
                    {
                        any = true;
                        if (perColour[k] < best)
                            best = perColour[k];
                    }
                return any ? best : (1 << 20);
            }
            bool operator()(MTGAbility * a, MTGAbility * b) const
            {
                const Key & ka = (*keys)[((AManaProducer *) a)->source];
                const Key & kb = (*keys)[((AManaProducer *) b)->source];
                //#W54-F: outranks scarcity - a utility land's tap is worth more
                //than a mana, so hold it back even when its colour is abundant.
                if (ka.utility != kb.utility)
                    return ka.utility < kb.utility;
                if (ka.scarcity != kb.scarcity)
                    return ka.scarcity > kb.scarcity;
                if (ka.colours != kb.colours)
                    return ka.colours < kb.colours;
                if (ka.layer != kb.layer)
                    return ka.layer < kb.layer;
                return abundance(a) > abundance(b);
            }
        } less;
        less.keys = &keyOf;
        less.perColour = perColour;
        std::stable_sort(order.begin(), order.end(), less);
        (void) payee;
        return order;
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
            if (!used[card] && producerUsable(p, amp, card, true, target, true) && amp->output->getConvertedCost() == 1)
            {//ai can't use cards which produce more than 1 converted while using the old pMana method.
                result->add(amp->output);
                used[card] = true;
            }
        }
    }
    return result;
}

bool ManaEngine::isAnimatedLand(MTGCardInstance * c)
{
    return c && c->isLand() && c->isCreature() && c->model && c->model->data
        && !c->model->data->isCreature();
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
            if (!zamp || !policy.canHandle(zamp) || !producerUsable(p, zamp, zamp->source, true, target, true))
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
        //#W52-L (D13): an animated land sorts LAST - it is tapped only when
        //every other attacker-source and the neutral pool cannot cover the
        //bill (see isAnimatedLand; pinned by aipay_animated_land_last_resort).
        std::sort(attackerProd.begin(), attackerProd.end(),
            [](AManaProducer * a, AManaProducer * b)
            {
                bool aa = ManaEngine::isAnimatedLand(a->source), ba = ManaEngine::isAnimatedLand(b->source);
                if (aa != ba)
                    return !aa;
                return a->source->power < b->source->power;
            });
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
    //HYBRID PIPS FIRST (N-116h, wave-36). Hybrid pips are COLOURED requirements,
    //and the old post-walk hybrid pass had two compounding defects: (a) it ran
    //AFTER the generic fill had already swallowed sources in layer order, so a
    //dual whose either colour a pip needed was often burned on its FIRST-listed
    //colour for generic; (b) its per-producer gate compared the accumulated
    //colour against a SINGLE pip's value (`result->getCost(c1) < value1`, i.e.
    //"< 1"), so a cost with TWO identical hybrid pips ({1}{g/w}{g/w}, Kitchen
    //Finks) could never receive a second source of the same colour - the first
    //{g} payment made the comparison false for the second pip forever. Because
    //LegalActionsOracle::payable's last resort IS this walk (potentialMana is
    //one-ability-per-card and under-reports duals), the cast was then never
    //offered at all - silently (deck116 vs105 s17, corpus 20260820: Forest +
    //Tropical Island + 2 Glimmerpost on the table, "Mana available: 4", Kitchen
    //Finks {1}{g/w}{g/w} absent from the cast list). Pay each pip UP FRONT, one
    //unused producer per pip, either colour side, preferring a source that pays
    //none of the cost's PLAIN coloured pips (hold those for the coloured walk
    //below); the final result->canAfford(cost) validation still arbitrates
    //global correctness, so a shape this walk cannot complete stays unpayable
    //exactly as before.
    {
        ManaCostHybrid * hc;
        int hyb = 0;
        while ((hc = cost->getHybridCost(hyb++)) != NULL)
        {
            if (result->canAfford(cost, 0))
                break; //everything, this pip included, is already covered
            bool paid = false;
            for (int pref = 0; pref < 2 && !paid; pref++)
            {
                for (size_t i = 0; i < p->getObserver()->mLayers->actionLayer()->manaObjects.size() && !paid; i++)
                {
                    MTGAbility * a = ((MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[i]);
                    AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
                    if (!amp || !policy.canHandle(amp))
                        continue;
                    if (amp->getCost() && amp->getCost()->extraCosts && !amp->getCost()->extraCosts->canPay())
                        continue;
                    MTGCardInstance * src = amp->source;
                    if (src == target)
                        used[src] = true; //http://code.google.com/p/wagic/issues/detail?id=76
                    if (used[src] || !producerUsable(p, amp, src, true, target, true) || amp->output->getConvertedCost() < 1)
                        continue;
                    //only true colour sides; a {2/w}-style generic side is out of
                    //this walk's scope (unchanged from the old pass's reach).
                    bool c1 = hc->color1 > 0 && hc->value1 && amp->output->hasColor(hc->color1);
                    bool c2 = hc->color2 > 0 && hc->value2 && amp->output->hasColor(hc->color2);
                    if (!c1 && !c2)
                        continue;
                    if (pref == 0)
                    {
                        //don't burn a source a PLAIN coloured pip of this cost
                        //also needs - hold it for the coloured walk below.
                        bool onPlainPip = false;
                        for (int k = 1; k < Constants::NB_Colors && !onPlainPip; k++)
                            if (k != Constants::MTG_COLOR_ARTIFACT && cost->getCost(k) && amp->output->hasColor(k))
                                onPlainPip = true;
                        if (onPlainPip)
                            continue;
                    }
                    int col = c1 ? hc->color1 : hc->color2;
                    payments.push_back(amp);
                    result->add(col, amp->output->getCost(col));
                    used[src] = true;
                    fullColor++;
                    paid = true;
                }
            }
        }
    }
    //#W49-D4: the GENERIC fill spends the SCARCE colour. Pass 0 above took
    //off-pip sources in LAYER order (earliest-played first), so a {2}{w} cost
    //over Underground Sea, Swamp, Underground Sea, Scrubland x3 paid its {2}
    //from BOTH Underground Seas - the seat's only two blue sources - when
    //Swamp + a spare Scrubland paid it just as well; the tutored Intruder
    //Alarm {2}{u} then sat in hand a full turn cycle and the game was lost by
    //that turn (corpus 20260827 deck123 vs162 seq17->19: "{U} 0" after the
    //cast). Traced payment order on that board: the coloured walk pays {w}
    //with the first Scrubland; the generic fill then takes the first two
    //off-pip producers in layer order, which are the two Seas because the
    //Swamp is listed between them and the fill stops the moment the total
    //is covered - the Swamp was never reached.
    //The walk is now THREE passes: (0) the coloured walk in layer order,
    //exactly as before (deferFlexibleSource, wave-32; the hybrid walk above,
    //wave-36; the foreach/Coffers branch; nothing here changes); (1) the
    //generic fill from OFF-PIP sources only, capped at the generic amount, in
    //a SCARCITY order; (2) the generic fill from anything, same order. The
    //order key, per SOURCE CARD: the colour it makes that the controller has
    //the FEWEST sources of (tapped or not) - the source whose every colour is
    //abundant taps first, the source that is one of the last makers of some
    //colour taps last; ties break mono-colour before dual before tri
    //(fewer colours = less flexible = tap first), then layer order as before.
    //A colourless source has no colour to strand and goes first of all. The
    //key is read from the board, never from the hand: at this cast the hand
    //held no blue card - the blue need was the tutor TARGET - so a hand-
    //weighted rule would have tapped the Seas exactly as the old walk did.
    //Same order for the human seat (selectAutoTapProducers consumes this plan
    //in plan order for its filler pass) so the human's auto-tap and the AI's
    //payment agree.
    std::vector<MTGAbility*> genericOrder = genericFillOrder(p, policy, target);
    std::vector<MTGAbility*> layerOrder;
    for (size_t i = 0; i < p->getObserver()->mLayers->actionLayer()->manaObjects.size(); i++)
        layerOrder.push_back((MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[i]);
    //#W54-F: the COLOURED walk gets the same utility preference, stably - a
    //source with a non-mana tap ability sinks to the end of the layer order,
    //everything else keeps the order this walk has always used. Without it the
    //owner's board pays its {G} pip with the FIRST Forest in layer order, and
    //that is the Squirrel Nest one; the generic fill's preference below can no
    //longer save it. This is ordering ONLY: the same producers are considered,
    //the plan's final result->canAfford(cost) arbitration is untouched, so a
    //cost payable only by the utility source is still paid by it.
    {
        std::map<MTGCardInstance*, bool> utilityOf;
        for (size_t i = 0; i < layerOrder.size(); i++)
        {
            AManaProducer * amp = dynamic_cast<AManaProducer*>(layerOrder[i]);
            MTGCardInstance * src = amp ? amp->source : NULL;
            if (src && utilityOf.find(src) == utilityOf.end())
                utilityOf[src] = sourceHasNonManaTapAbility(p, src);
        }
        struct UtilityLast
        {
            std::map<MTGCardInstance*, bool> * u;
            bool rank(MTGAbility * a) const
            {
                AManaProducer * amp = dynamic_cast<AManaProducer*>(a);
                if (!amp || !amp->source)
                    return false; //wrappers keep their place
                std::map<MTGCardInstance*, bool>::iterator it = u->find(amp->source);
                return it != u->end() && it->second;
            }
            bool operator()(MTGAbility * a, MTGAbility * b) const
            {
                return rank(a) < rank(b);
            }
        } utilityLast;
        utilityLast.u = &utilityOf;
        std::stable_sort(layerOrder.begin(), layerOrder.end(), utilityLast);
    }
    for (int pass = 0; pass < 3; pass++)
    {
    const bool colourWalk = (pass == 0);
    const bool offPipOnly = (pass == 1);
    const std::vector<MTGAbility*> & walk = colourWalk ? layerOrder : genericOrder;
    for (size_t i = 0; i < walk.size(); i++)
    {
        MTGAbility * a = ((MTGAbility *) walk[i]);
        AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
        if(amp && (amp->getCost() && amp->getCost()->extraCosts && !amp->getCost()->extraCosts->canPay()))
            continue;
        //COLOURED PIPS FIRST (N-152c). The generic-fill branch below used to
        //open on `cost->hasColor(0) || cost->hasColor(7)` alone, i.e. for ANY
        //cost carrying a generic component - and it `continue`s past the
        //colour-aware branch, so the colour-aware branch never ran for such a
        //cost at all. Producers were then swallowed in layer order, ONE ability
        //per card, until canAfford happened to pass. A dual source (Scrubland's
        //Plains+Swamp subtypes -> add{W} THEN add{B}; Overgrown Farmland's
        //add{G}/add{W}) therefore only ever contributed its FIRST-listed colour,
        //and a multi-pip cost with a generic component ({1}{B}{B} over Scrubland
        //+ Vault of Whispers + Darksteel Citadel) came back UNPAYABLE. Because
        //LegalActionsOracle::payable's last resort IS this walk, the cast was
        //then never offered at all - silently: no defer record, no ask, the
        //window burned (deck152 Sigarda seq21/seq26, corpus 20260727).
        //
        //Gate the generic fill on the coloured pips being COVERED instead.
        //`fullColor` counts only what THIS walk assigned, so it misses colour
        //already sitting in the mana pool (pre-added to `result` above) - read
        //the requirement off `result` vs `cost` directly and keep the historical
        //counter as a belt-and-braces disjunct. Colour 0 (generic) and colour 7
        //(land) are the generic side of `needColorConverted` and are excluded.
        bool coloredSatisfied = true;
        for (int k = 1; k < Constants::NB_Colors && coloredSatisfied; k++)
        {
            if (k == Constants::MTG_COLOR_LAND)
                continue;
            if (result->getCost(k) < cost->getCost(k))
                coloredSatisfied = false;
        }
        if((coloredSatisfied || fullColor == needColorConverted) && result->getConvertedCost() < cost->getConvertedCost()) // Fixed a bug on colorless mana calculation for AI.
        {
            if((cost->hasColor(0) || cost->hasColor(7)) && amp)//find colorless after color mana.
            {
                if(result->canAfford(cost,0))
                    continue;
                if (colourWalk)
                    continue; //#W49-D4: generic is filled by passes 1-2, in scarcity order
                if (offPipOnly)
                {
                    if (offColorFilled >= genericAmount)
                        continue;
                    //#W51-B: judge on-pip by the SOURCE CARD, not by the one
                    //ability in hand. A dual scripted as two single-colour
                    //abilities (Overgrown Farmland: add{G} THEN add{W}) reached
                    //through its off-colour half read as an off-pip source for a
                    //{1}{W} cost, so this pass spent the Farmland's {G} on the
                    //generic while a second Plains sat untapped (corpus 20260827
                    //deck152 vs123 seq16, `Paid {1}{w} for Intrepid Adversary
                    //with Plains #2, Overgrown Farmland #2`) - the seat's only
                    //green source gone for a pip any Plains could pay. A card
                    //that makes a colour the cost needs is held for pass 2,
                    //where the scarcity order decides.
                    bool onPip = false;
                    for (int k = 1; k < Constants::NB_Colors && !onPip; k++)
                        if (k != Constants::MTG_COLOR_ARTIFACT && cost->getCost(k)
                            && cardMakesColour(p, policy, amp->source, target, k))
                            onPip = true;
                    if (onPip)
                        continue; //keep it for the colored pips / pass 2
                    //...and only a MONO (or colourless) source rides this pass.
                    //An off-pip DUAL still strands its other colour; whether
                    //that is cheaper than spending an on-pip source is exactly
                    //the scarcity question pass 2 answers, so let it. ({2}{W}
                    //over Underground Sea x2, Swamp, Scrubland x3: the Seas are
                    //off-pip but the seat's only blue; the generic is Swamp +
                    //a spare Scrubland, never a Sea.)
                    if (cardColourCount(p, policy, amp->source, target) > 1)
                        continue;
                }
                if (policy.canHandle(amp))
                {
                    MTGCardInstance * card = amp->source;
                    if (card == target)
                        used[card] = true; //http://code.google.com/p/wagic/issues/detail?id=76
                    if (!used[card] && producerUsable(p, amp, card, true, target, true) && amp->output->getConvertedCost() >= 1)
                    {
                        if(!(result->canAfford(cost,0)))//if we got to this point we should be filling colorless mana requirements.
                        {
                            payments.push_back(amp);
                            result->add(amp->output);
                            used[card] = true;
                            if (offPipOnly)
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
                    //#W52-H: tap a variable source only while it still HELPS. This
                    //branch used to take every foreach producer it walked past, so a
                    //{2}{b} over three Overgrown Battlements + Woodland Cemetery (layer
                    //order: walls first, the black land last) tapped ALL THREE walls -
                    //each one already making {g}{g}{g}{g}{g}{g} - because the {b} pip
                    //stayed short until the land came up, and nothing here asked whether
                    //the wall's output was needed (corpus 20260827-155545 deck126 vs152
                    //seq18->19: "Paid {2}{b} for Tribute to Hunger with Overgrown
                    //Battlement #1; #2; #3; Woodland Cemetery", 13 {g} floated; seq26->27
                    //the same three walls tapped into an 18-life combat). A source helps
                    //when it makes a coloured pip the plan is still short of, or the
                    //TOTAL is still short (it is then a generic filler - the first wall
                    //covers the {2} with mana to spare, the next two do not help). The
                    //walk is layer-ordered, so the land that pays the pip is still
                    //reached after the one wall the cost needed.
                    bool helps = result->getConvertedCost() < cost->getConvertedCost();
                    for (int k = 1; k < Constants::NB_Colors && !helps; k++)
                        if (k != Constants::MTG_COLOR_LAND && amp->output->hasColor(k)
                            && result->getCost(k) < cost->getCost(k))
                            helps = true;
                    if (!helps)
                        continue;
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
        else if (amp && policy.canHandle(amp) && producerUsable(p, amp, amp->source, false, target, true))
        {
            for (int k = Constants::NB_Colors-1; k > 0 ; k--)//go backwards.
            {
                if (cost->hasColor(k) && amp->output->hasColor(k) && result->getCost(k) < cost->getCost(k))
                {
                    MTGCardInstance * card = amp->source;
                    if (card == target)
                        used[card] = true; //http://code.google.com/p/wagic/issues/detail?id=76
                    if (!used[card] && producerUsable(p, amp, card, true, target, true) && amp->output->getConvertedCost() >= 1)
                    {
                        //Don't burn a dual/flexible source on a color a dedicated
                        //source can pay - hold it for the color only it supplies.
                        //Only a single-color ability is deferrable; an add-both
                        //producer ({W}{B} at once) pays both pips in one tap.
                        if (amp->output->getConvertedCost() == 1
                            && deferFlexibleSource(p, policy, cost, result, used, card, k, target))
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
    //the next pass only runs when this one left the total short.
    if (result->canAfford(cost, 0))
        break;
    }
    ManaCostHybrid * hybridCost;
    hybridCost = cost->getHybridCost(0);
    if(hybridCost)
    {
        //hybrid pips are paid by the HYBRID-PIPS-FIRST walk above (N-116h),
        //before the generic fill can swallow their sources; nothing left to do
        //here. The final result->canAfford(cost) validation below arbitrates.
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
        //
        //ONE ABILITY PER SOURCE (W39-DOUBLEASK). This tail is the only walk in
        //the planner that did not mark `used[amp->source]` after taking a
        //producer, so a card carrying more than one mana ability - every DUAL
        //land (Seachrome Coast add{W} + add{U}, Tundra, Volcanic Island) -
        //entered the plan TWICE. The plan is executed as one click per entry
        //and the first click taps the card, so the second click on the same
        //(now tapped) card is refused; AIPlayerBaka::Act treats a refused
        //payment click as a broken plan and DISCARDS THE WHOLE REMAINDER,
        //including the cast click. The seat is then left with partial mana
        //floating and the spell still in hand - which is what the GPT seat
        //re-asked over: the identical cast question a second time, now
        //rendering the POST-TAP `Mana available:` figure (corpus 20260823
        //deck125 vs126 seq16 "Mana available: 6" -> seq17 "Mana available: 1,
        //Already in pool: 5"; the second ask is a full extra model call).
        //Every other walk above already enforces one-ability-per-card through
        //this same `used` map, and so does ManaEngine::selectAutoTapProducers
        //on the human side; this is the outlier, not a deliberate exception.
        for (size_t i = 0; i < p->getObserver()->mLayers->actionLayer()->manaObjects.size(); i++)
        {
            MTGAbility * a = ((MTGAbility *) p->getObserver()->mLayers->actionLayer()->manaObjects[i]);
            AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
            if (amp && policy.canHandle(amp))
            {
                if (!used[amp->source] && producerUsable(p, amp, amp->source, true, target, true) && amp->output->getConvertedCost() >= 1)
                {
                    payments.push_back(amp);
                    used[amp->source] = true;
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
    return potentialColorReach(p, policy, outColors, NULL);
}

int ManaEngine::potentialColorReach(Player * p, ManaPolicy & policy, ManaCost * outColors,
                                    std::vector<ManaSourceView> * outSources)
{
    map<MTGCardInstance *, size_t> seen; //source card -> index in outSources
    vector<ManaSourceView> sources;
    ActionLayer * al = p->getObserver()->mLayers->actionLayer();
    for (size_t i = 0; i < al->manaObjects.size(); i++)
    {
        MTGAbility * a = (MTGAbility *) al->manaObjects[i];
        AManaProducer * amp = dynamic_cast<AManaProducer*>(a);
        bool variable = false;
        //N-166k: a foreach-wrapped producer (Tolarian Academy: `{T}: foreach
        //(artifact|myBattlefield) add{U}`) is a GenericActivatedAbility holding
        //an AForeach holding the AManaProducer, so the plain cast above returns
        //NULL and the source was counted NOWHERE - "Mana available: 3 total" on
        //a board with three lands AND an untapped Academy. potentialMana already
        //unwraps this shape; do the same here, and mark the source variable so
        //the render can say the one source is not one mana.
        if (!amp)
        {
            GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility*>(a);
            if (gmp)
                if (AForeach * fmp = dynamic_cast<AForeach*>(gmp->ability))
                {
                    amp = dynamic_cast<AManaProducer*>(fmp->ability);
                    variable = (amp != NULL);
                }
        }
        if (!amp || !policy.canHandle(a) || !amp->source)
            continue;
        //The foreach wrapper carries the tap cost, so amp->tap can be 0 on a
        //producer whose source is spent - refuse a tapped source outright
        //rather than trusting the inner ability's flag.
        if (variable && (amp->source->isTapped() || amp->source->hasSummoningSickness()))
            continue;
        if (!producerUsable(p, amp, amp->source, true))
            continue;
        if (amp->output->getConvertedCost() < 1)
            continue;
        std::ostringstream mine;
        for (int c = 0; c < Constants::NB_Colors; c++)
            if (amp->output->hasColor(c))
            {
                if (outColors && outColors->getCost(c) < 1)
                    outColors->add(c, 1);
                mine << "{" << Constants::MTGColorChars[c] << "}";
            }
        map<MTGCardInstance *, size_t>::iterator at = seen.find(amp->source);
        if (at == seen.end())
        {
            ManaSourceView v;
            v.card = amp->source;
            v.colors = mine.str();
            v.variable = variable;
            seen[amp->source] = sources.size();
            sources.push_back(v);
        }
        else
        {
            //A dual scripted as two single-colour abilities: ONE source, both
            //colours (the N-146d collapse, at the per-source line this time).
            ManaSourceView & v = sources[at->second];
            if (v.colors.find(mine.str()) == string::npos)
                v.colors += " or " + mine.str();
            v.variable = v.variable || variable;
        }
    }
    if (outSources)
        *outSources = sources;
    return (int) sources.size();
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
            //#W52-H: a foreach-wrapped VARIABLE producer (Overgrown Battlement,
            //Tolarian Academy) is a GenericActivatedAbility around an AForeach
            //around the AManaProducer; the plain cast above is NULL for it and
            //the simulation used to credit it ONE colourless mana, so the row
            //forecast for a {2}{b} over three Battlements named TWO walls while
            //one tap of one wall made six {g}. Unwrap it and credit what the
            //tap actually makes (output x the foreach count), so the forecast
            //(`{paying this taps: ...}`, `{leaves N of M}`) and the executed
            //plan (planPayment, same fix) name the same sources.
            int repeat = 1;
            if (!amp)
                if (GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility *>(plan[i]))
                    if (AForeach * fmp = dynamic_cast<AForeach *>(gmp->ability))
                        if (AManaProducer * inner = dynamic_cast<AManaProducer *>(fmp->ability))
                        {
                            amp = inner;
                            repeat = fmp->checkActivation();
                        }
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
                for (int r = 0; r < repeat; r++)
                    sim->add(amp->output);
            else
                sim->add(Constants::MTG_COLOR_ARTIFACT, 1); //unknown wrapper: approximate
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

int ManaEngine::producerSelfDamageOf(MTGAbility * producer)
{
    AManaProducer * amp = dynamic_cast<AManaProducer*>(producer);
    return amp ? producerSelfDamage(amp) : 0;
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
