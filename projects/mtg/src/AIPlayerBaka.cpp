#include "PrecompiledHeader.h"

#include "AIPlayerBaka.h"
#include "DecisionContract.h"
#include "ManaEngine.h"
#include "CardDescriptor.h"
#include "AIStats.h"
#include "AllAbilities.h"
#include "ExtraCost.h"
#include "GuiCombat.h"
#include "AIHints.h"
#include "ManaCostHybrid.h"
#include "MTGRules.h"

//
// AIAction
//


Player * OrderedAIAction::getPlayerTarget()
{
    if (playerAbilityTarget)
        return (Player *)playerAbilityTarget;

    return NULL;
}

int OrderedAIAction::getEfficiency(AADamager * aad)
{
    Player * playerTarget = getPlayerTarget();
    GameObserver * g = owner->getObserver();
    Player * p = g->currentlyActing();

    MTGCardInstance * dTarget = target ? target : dynamic_cast<MTGCardInstance *>(aad->getTarget());

    if(!target && !playerTarget && dTarget)
    {
        //no action target, but damage has a target...this is most likely a card like pestilence.
        return int(p->opponent()->game->battlefield->countByType("creature") - p->game->battlefield->countByType("creature")) * 25 % 100;
    }

    if(playerTarget)
    {
        TargetChooser * checkT = g->getCurrentTargetChooser();
        int otherTargets = checkT ? checkT->countValidTargets() : 0;
        if (playerTarget == p->opponent())
            return 90 - otherTargets;
        return  0;
    }

    if(p && target)
        if(p == target->controller())
            return 0;

    if (dTarget && aad && (aad->getDamage() == dTarget->toughness))
        return 100;
    else if (dTarget && aad && (aad->getDamage() > dTarget->toughness))
        //A kill with slight overkill must still outrank chip damage to the
        //player's face (90 - otherTargets above); waste-heavy overkill
        //decays below it (upstream issue #1079 / ai/goblin_artillery.txt).
        return 95 - 10 * (aad->getDamage() - dTarget->toughness - 1);
    else
        return 10;

    return 0;
}

//Damage a card's own script promises on resolution, read the way the engine
//will read it. magicText is the auto= lines joined by '\n' (CardPrimitive::
//addMagicText), so the shapes that can be trusted here are the untriggered
//single-amount ones: a '@'-prefixed line fires on some LATER event rather than
//when the spell resolves, the damage:type: forms are mass damage with no single
//target, "x" is only meaningful after announcement, and an expression naming
//rand would draw the game RNG just by being evaluated. Everything else is
//measured with the same WParsedInt the resolution uses. Skipping a shape costs
//an answer the AI would have had; guessing one wastes a real card, so every
//uncertain shape is skipped.
static int scriptedDamageOnResolve(MTGCardInstance * source)
{
    if (!source)
        return 0;
    string mt = source->magicText;
    for (size_t i = 0; i < mt.size(); i++)
        mt[i] = (char) tolower((unsigned char) mt[i]);

    int total = 0;
    size_t lineStart = 0;
    while (lineStart <= mt.size())
    {
        size_t lineEnd = mt.find('\n', lineStart);
        if (lineEnd == string::npos)
            lineEnd = mt.size();
        string line = mt.substr(lineStart, lineEnd - lineStart);
        lineStart = lineEnd + 1;

        size_t firstChar = line.find_first_not_of(" \t\r");
        if (firstChar == string::npos || line[firstChar] == '@')
            continue;

        size_t pos = line.find("damage:");
        if (pos == string::npos)
            continue;
        string amount = line.substr(pos + 7);
        size_t stop = amount.find_first_of(" \t\r");
        if (stop != string::npos)
            amount = amount.substr(0, stop);
        if (amount.empty() || amount.find(':') != string::npos
            || amount.find("rand") != string::npos || amount == "x")
            continue;

        if (amount.find_first_not_of("0123456789") == string::npos)
        {
            total += atoi(amount.c_str());
            continue;
        }
        WParsedInt parsed(amount, NULL, source);
        int value = parsed.getValue();
        if (value > 0)
            total += value;
    }
    return total;
}

//Damage already on the stack and aimed at this card.
//
//The regenerate and prevent scoring below only ever looked at combat, so a burn
//spell about to resolve on a creature raised neither - Baka sat on a
//regeneration that would have saved the card. The gap became reachable when
//priority moved engine-side: the AI now reliably holds priority while a spell
//waits on the stack, which is exactly the window these answers are for.
//
//Two shapes are counted, because they are the two whose magnitude is knowable
//before resolution: an ability on the stack whose core is an AADamager aimed
//here (the engine hands over the exact number), and a spell on the stack that
//targets this card and whose script carries an untriggered damage clause.
static int pendingStackDamageTo(GameObserver * observer, MTGCardInstance * card)
{
    if (!observer || !card || !observer->mLayers)
        return 0;
    ActionStack * stack = observer->mLayers->stackLayer();
    if (!stack)
        return 0;

    int total = 0;
    Interruptible * item = NULL;
    while ((item = stack->getNext(item, 0, NOT_RESOLVED)))
    {
        if (StackAbility * stacked = dynamic_cast<StackAbility *>(item))
        {
            MTGAbility * core = AbilityFactory::getCoreAbility(stacked->ability);
            AADamager * damager = dynamic_cast<AADamager *>(core);
            if (damager && dynamic_cast<MTGCardInstance *>(damager->getTarget()) == card)
                total += damager->getDamage();
            continue;
        }

        Spell * spell = dynamic_cast<Spell *>(item);
        if (!spell || !spell->source)
            continue;
        bool aimedHere = false;
        MTGCardInstance * spellTarget = NULL;
        while ((spellTarget = spell->getNextCardTarget(spellTarget)))
        {
            if (spellTarget == card)
            {
                aimedHere = true;
                break;
            }
        }
        if (aimedHere)
            total += scriptedDamageOnResolve(spell->source);
    }
    return total;
}

//A regeneration shield this card is already about to get.
//
//regenerateTokens only rises when the ability RESOLVES, so between activation
//and resolution the card still reads as unshielded. getEfficiency's own
//`stack has this ability` guard covers the ability that was activated, but not
//a SECOND regeneration aimed at the same card - a card with two regenerate
//abilities, or a second permanent offering one - and one threat only ever
//needs one shield, so the second is a wasted card and wasted mana.
static bool regenerationPendingFor(GameObserver * observer, MTGCardInstance * card)
{
    if (!observer || !card || !observer->mLayers)
        return false;
    ActionStack * stack = observer->mLayers->stackLayer();
    if (!stack)
        return false;

    Interruptible * item = NULL;
    while ((item = stack->getNext(item, 0, NOT_RESOLVED)))
    {
        StackAbility * stacked = dynamic_cast<StackAbility *>(item);
        if (!stacked)
            continue;
        MTGAbility * core = AbilityFactory::getCoreAbility(stacked->ability);
        if (core && core->aType == MTGAbility::STANDARD_REGENERATE
            && dynamic_cast<MTGCardInstance *>(core->target) == card)
            return true;
    }
    return false;
}

// In this function, target represents the target of the currentAIAction object, while coreAbilityCardTarget is the target of the ability of this AIAction object
// I can't remember as I type this in which condition we use one or the other for this function, if you find out please replace this comment
int OrderedAIAction::getEfficiency()
{
    //the below is required for CMPAbilities operator override, without it the effs trip a debug assert. we need to find a better way to do it.
    if (efficiency > -1)
        return efficiency;
    if (!ability)
        return 0;
    GameObserver * g = owner->getObserver();
    ActionStack * s = g->mLayers->stackLayer();
    int currentPhase = g->getCurrentGamePhase();

    Player * p = g->currentlyActing();
    if (s->has(ability))
        return 0;
    MTGAbility * a = AbilityFactory::getCoreAbility(ability);
    MTGAbility * transAbility = NULL;
    if(ATransformerInstant * atia = dynamic_cast<ATransformerInstant *>(a))
    {
        if(atia->newAbilityFound)
        {
            AbilityFactory af(g);
            transAbility = af.parseMagicLine(atia->newAbilitiesList[atia->newAbilitiesList.size()-1], 0, NULL, atia->source);
            transAbility->target = ability->target;
            a = transAbility;
        }
    }
    if (!a)
    {
        DebugTrace("FATAL: Ability is NULL in AIAction::getEfficiency()");
        return 0;
    }

    if (!((AIPlayerBaka *)owner)->canHandleCost(ability))
    {
        SAFE_DELETE(transAbility);
        return 0;
    }
    MTGCardInstance * coreAbilityCardTarget = dynamic_cast<MTGCardInstance *>(a->target);

    //CoreAbility shouldn't return a Lord, but it does.
    //When we don't have a target for a lord action, we assume it's the lord itself
    if (!target && dynamic_cast<ALord*> (a))
    {
        target = a->source;
    }

    AACastCard * CC = dynamic_cast<AACastCard*> (a);
    if (CC)
        return 99;

    switch (a->aType)
    {
    case MTGAbility::DAMAGER:
        {
            efficiency =  getEfficiency ((AADamager *) a);
            break;
        }
    case MTGAbility::STANDARD_REGENERATE:
        {
            efficiency = 0;
            if (!coreAbilityCardTarget)
                break;

            if (!coreAbilityCardTarget->regenerateTokens && currentPhase == MTG_PHASE_COMBATBLOCKERS
                && (coreAbilityCardTarget->defenser || coreAbilityCardTarget->blockers.size())
                && !regenerationPendingFor(g, coreAbilityCardTarget)
                )
            {
                efficiency = 95;
            }
            else if (!coreAbilityCardTarget->regenerateTokens
                     && !regenerationPendingFor(g, coreAbilityCardTarget))
            {
                //A regeneration shield replaces the destruction that lethal
                //damage causes, so it answers a burn spell exactly as it
                //answers a lethal blocker - and the stack is the only place
                //that damage is visible before it lands. life is the card's
                //REMAINING toughness (damage is subtracted from it), so lethal
                //is measured against that, not the printed value.
                int pendingDamage = pendingStackDamageTo(g, coreAbilityCardTarget);
                if (pendingDamage > 0 && pendingDamage >= coreAbilityCardTarget->life)
                    efficiency = 95;
            }
            break;
        }
    case MTGAbility::STANDARD_PREVENT:
        {
            efficiency = 0;//starts out low to avoid spamming it when its not needed.

            if (!target)
                break;

            if (currentPhase == MTG_PHASE_COMBATBLOCKERS)
            {
                bool NeedPreventing = false;
                MTGCardInstance * nextOpponent = target->getNextOpponent();
                if(!nextOpponent)
                    break;
                if ((target->defenser || target->blockers.size()) && target->preventable < nextOpponent->power)
                    NeedPreventing = true;
                if (p == target->controller() && target->controller()->isAI() && NeedPreventing  && !(nextOpponent->has(Constants::DEATHTOUCH) || nextOpponent->has(Constants::PERPETUALDEATHTOUCH) || nextOpponent->has(Constants::WITHER)))
                {
                    efficiency = 20 * (target->DangerRanking());//increase this chance to be used in combat if the creature blocking/blocked could kill the creature this chance is taking into consideration how good the creature is, best creature will always be the first "saved"..
                    if (target->toughness == 1 && nextOpponent->power == 1)
                        efficiency += 15;
                    //small bonus added for the poor 1/1s, if we can save them, we will unless something else took precidence.
                    //note is the target is being blocked or blocking a creature with wither or deathtouch, it is not even considered for preventing as it is a waste.
                    //if its combat blockers, it is being blocked or blocking, and has less prevents the the amount of damage it will be taking, the effeincy is increased slightly and totalled by the danger rank multiplier for final result.
                    if((target->defenser || target->blockers.size()) && target->controller() == p)
                    {
                        int damages = nextOpponent->power;
                        int calculateAfterDamage = target->toughness - damages;
                        if((calculateAfterDamage + target->preventable) > 0)
                        {
                            efficiency = 0;
                            //this is to avoid wasting prevents on creatures that will already survive.
                            //this should take into account bushido and flanking as this check is run after every trigger.
                        }
                    }
                }
            }
            else if (p == target->controller() && target->controller()->isAI())
            {
                //Off-combat answer: damage already on the stack aimed at this
                //card. Prevention only earns its card if it turns lethal into
                //survivable - preventing 2 of a 5-damage bolt on a 3-toughness
                //creature loses the creature AND the prevention.
                int pendingDamage = pendingStackDamageTo(g, target);
                if (pendingDamage > 0 && (pendingDamage - target->preventable) >= target->life)
                {
                    AADamagePrevent * prevention = dynamic_cast<AADamagePrevent *>(a);
                    int wouldPrevent = prevention ? prevention->preventing : 0;
                    if (wouldPrevent > 0
                        && (target->life + target->preventable + wouldPrevent - pendingDamage) > 0)
                        efficiency = 20 * (target->DangerRanking());
                }
            }
            break;
        }
    case MTGAbility::STANDARD_EQUIP:
        {

            efficiency = 0;
            if (!target)
                break;

            unsigned int equips = p->game->battlefield->countByType("Equipment");
            unsigned int myArmy = p->game->battlefield->countByType("Creature");
            // when can this ever be negative?
            int equalized = myArmy ? equips / myArmy : 0;

            if (p == target->controller() && target->equipment <= 1 && !a->source->target)
            {
                efficiency = 20 * (target->DangerRanking());
                if (target->hasColor(Constants::MTG_COLOR_WHITE))
                    efficiency += 20;//this is to encourage Ai to equip white creatures in a weenie deck. ultimately it will depend on what had the higher dangerranking.
                if (target->power == 1 && target->toughness == 1 && target->isToken == 0)
                    efficiency += 10; //small bonus to encourage equipping nontoken 1/1 creatures.
            }

            if (p == target->controller() && !a->source->target && target->equipment < equalized)
            {
                efficiency = 15 * (target->DangerRanking());
                efficiency -= 5 * (target->equipment);
            }

            if ( efficiency < 20 && efficiency > 0 )
                efficiency += target->controller()->getObserver()->getRandomGenerator()->random() % 30;
            break;
        }
    case MTGAbility::STANDARD_LEVELUP:
        {
            efficiency = 0;
            Counter * targetCounter = NULL;
            int currentlevel = 0;

            if (!coreAbilityCardTarget)
                break;

            if (coreAbilityCardTarget->counters && coreAbilityCardTarget->counters->hasCounter("level", 0, 0))
            {
                targetCounter = coreAbilityCardTarget->counters->hasCounter("level", 0, 0);
                currentlevel = targetCounter->nb;
            }
            if (currentlevel < coreAbilityCardTarget->MaxLevelUp)
            {
                efficiency = 85;
                //increase the efficeincy of leveling up by a small amount equal to current level.
                efficiency += currentlevel;

                if (p->game->hand->nb_cards > 0 && p->isAI())
                {
                    efficiency -= (10 * p->game->hand->nb_cards);//reduce the eff if by 10 times the amount of cards in Ais hand.
                    //it should always try playing more cards before deciding
                }

                if (g->getCurrentGamePhase() == MTG_PHASE_SECONDMAIN)
                {
                    efficiency = 100;
                    //in 2nd main, go all out and try to max stuff.
                }
            }

            break;
        }
    case MTGAbility::COUNTERS:
        {
            MTGCardInstance * _target = target ? target : coreAbilityCardTarget;
            efficiency = 0;

            if (!_target)
                break;

            if(AACounter * cc = dynamic_cast<AACounter*> (a))
            {
                if(_target->controller() == p && cc->toughness>=0)
                {
                    efficiency = 90;

                }
                if(_target->controller() != p && ((_target->toughness + cc->toughness <= 0 && _target->toughness) || (cc->toughness < 0 && cc->power < 0)))
                {
                    efficiency = 90;

                }
                if(_target->counters && _target->counters->hasCounter(cc->power,cc->toughness) && _target->counters->hasCounter(cc->power,cc->toughness)->nb > 15)
                {
                    efficiency = _target->counters->hasCounter(cc->power,cc->toughness)->nb;
                }
                if(cc->maxNb && _target->counters && _target->counters->hasCounter(cc->power,cc->toughness)->nb >= cc->maxNb) 
                    efficiency = 0;
                if(a->target == a->source && a->getCost() && a->getCost()->hasX())
                    efficiency -= 10 * int(p->game->hand->cards.size());
            }
            break;
        }
    case MTGAbility::STANDARD_PUMP:
        {
            efficiency = 0;
            if(!coreAbilityCardTarget)
                break;
            if(!target && !dynamic_cast<ALord*> (a) && (((MTGCardInstance *)a->source)->hasSubtype(Subtypes::TYPE_AURA) || ((MTGCardInstance *)a->source)->hasSubtype(Subtypes::TYPE_EQUIPMENT)))
            {
                if(a->source->target)
                    coreAbilityCardTarget = a->source->target; //TODO use intermediate value?
                target = a->source;
            }
            else //if(how to know cards like Basking Rootwalla that pump themselves)
            {
                target = a->source;
            }
            if (!target && !dynamic_cast<ALord*> (a))
                break;
            if(dynamic_cast<ALord*> (a) && !target)
            {
                target = a->source;
            }

            AbilityFactory af(g);
            int suggestion = af.abilityEfficiency(a, p, MODE_ABILITY);
            //i do not set a starting eff. on this ability, this allows Ai to sometimes randomly do it as it normally does.
            int currentPhase = g->getCurrentGamePhase();
            if ((currentPhase == MTG_PHASE_COMBATBLOCKERS) || (currentPhase == MTG_PHASE_COMBATATTACKERS))            
            {
                if (suggestion == BAKA_EFFECT_GOOD && target->controller() == p)                 
                {
                    if(coreAbilityCardTarget->defenser || coreAbilityCardTarget->blockers.size())
                    {
                        MTGCardInstance * opponent = coreAbilityCardTarget->getNextOpponent();
                        if (!opponent)
                            break;

                        if (coreAbilityCardTarget->power < opponent->toughness ||( coreAbilityCardTarget->toughness < opponent->power) || (coreAbilityCardTarget->has(Constants::TRAMPLE)))
                        {
                            //this pump is based on a start eff. of 20 multiplied by how good the creature is.
                            efficiency = 20 * coreAbilityCardTarget->DangerRanking();
                        }
                    }
                    if (coreAbilityCardTarget->isAttacker() && !coreAbilityCardTarget->blockers.size())
                    {
                        //this means im heading directly for the player, pump this creature as much as possible.
                        efficiency = 100;
                        if(coreAbilityCardTarget->power > 20) // to be realistic
                            efficiency -= coreAbilityCardTarget->power;//we don't need to go overboard. better to not put all your eggs in a single basket.
                    }
                }
            }
            if (suggestion == BAKA_EFFECT_BAD && target->controller() != p && target->toughness > 0)
            {
                efficiency = 100;
            }
            break;
        }
    case MTGAbility::STANDARD_BECOMES:
        {
            if(!coreAbilityCardTarget)
                break;
            // It used to skip most effects, with no other condition efficiency is -1
            // Becomes is generally good so setting a value, but don't want to spam it
            if (coreAbilityCardTarget && !coreAbilityCardTarget->isLand())
            {
                // Bonus if almost no cards in hand
                if (p->game->hand->nb_cards <= 1)
                {
                    efficiency = 50;
                }
                else efficiency = 30;
            }
            //nothing huge here, just ensuring that Ai makes his noncreature becomers into creatures during first main, so it can actually use them in combat.
            if (coreAbilityCardTarget && !coreAbilityCardTarget->isCreature() && !coreAbilityCardTarget->isTapped() && currentPhase == MTG_PHASE_FIRSTMAIN)
            {
                efficiency = 50;
            }            
            break;
        }
    case MTGAbility::MANA_PRODUCER://only way to hit this condition is nested manaabilities, ai skips manaproducers by defualt when finding an ability to use.
        {
            AManaProducer * manamaker = dynamic_cast<AManaProducer*>(a);
            GenericActivatedAbility * GAA = dynamic_cast<GenericActivatedAbility*>(ability);
            if(GAA)
            {
                AForeach * forMana = dynamic_cast<AForeach*>(GAA->ability);
                if (manamaker && forMana)
                {
                    int outPut = forMana->checkActivation();
                    if (ability->getCost() && outPut > int(ability->getCost()->getConvertedCost() +1) && currentPhase == MTG_PHASE_FIRSTMAIN && ability->source->controller()->game->hand->nb_cards > 1)
                        efficiency = 60;//might be a bit random, but better than never using them.
                }
            }
            else
                efficiency = 0;
            break; 
        }
    case MTGAbility::STANDARDABILITYGRANT:
        {
            efficiency = 0;

            if (!target)
                break;

            //ensuring that Ai grants abilities to creatures during first main, so it can actually use them in combat.
            //quick note: the eff is multiplied by creatures ranking then divided by the number of cards in hand.
            //the reason i do this is to encourage more casting and less waste of mana on abilities.
            AbilityFactory af(g);
            int suggestion = af.abilityEfficiency(a, p, MODE_ABILITY);

            int efficiencyModifier = (25 * target->DangerRanking());
            if (p->game->hand->nb_cards > 1)
            {
                efficiencyModifier -= p->game->hand->nb_cards*3;
            }
            if (suggestion == BAKA_EFFECT_BAD && p != target->controller() && !target->has(a->abilitygranted))
            {
                efficiency += efficiencyModifier;
            }

            if (!target->has(a->abilitygranted) && g->getCurrentGamePhase() == MTG_PHASE_COMBATBEGIN
                && p == target->controller()
                )
            {
                efficiency += efficiencyModifier;
            }

            if (suggestion == BAKA_EFFECT_GOOD && target->has(a->abilitygranted))
            {
                //trying to avoid Ai giving ie:flying creatures ie:flying twice.
                efficiency = 0;
            }

            if ((suggestion == BAKA_EFFECT_BAD && p == target->controller()) 
                || (suggestion == BAKA_EFFECT_GOOD && p != target->controller())
                )
            {
                efficiency = 0;
                //stop giving trample to the players creatures.
            }
            break;
        }

    case MTGAbility::UNTAPPER:
        //untap things that Ai owns and are tapped.
        {
            efficiency = 0;
            if (!target)
                break;

            if (target->isTapped() && target->controller() == p)
            {
                efficiency = target->isCreature()? (20 * target->DangerRanking()) : 100;
            }
            break;
        }

    case MTGAbility::TAPPER:
        //tap things the player owns and that are untapped.
        {
            if (!target)
                break;

            if (target->controller() != p)
                efficiency = (20 * target->DangerRanking());

            if (target->isTapped())
                efficiency = 0;

            break;
        }

    case MTGAbility::LIFER:
        {
            //use life abilities whenever possible. Well yes, but actually no
            //limits mana and in case of Zuran Orb it just sacrifices all lands
            AALifer * alife = (AALifer *) a;
            Targetable * _t = alife->getTarget();

            efficiency = 80;
            AbilityFactory af(g);
            int suggestion = af.abilityEfficiency(a, p, MODE_ABILITY);

            if(MTGCardInstance * cTarget = dynamic_cast<MTGCardInstance *>(_t))
            {
                if((suggestion == BAKA_EFFECT_BAD && (cTarget)->controller() == p) || (suggestion == BAKA_EFFECT_GOOD && (cTarget)->controller() != p))
                    efficiency = 0;
            }
            else if ((suggestion == BAKA_EFFECT_BAD && _t == p) || (suggestion == BAKA_EFFECT_GOOD && _t != p))
            {
                efficiency = 0;
            }

            break;
        }
    case MTGAbility::STANDARD_DRAW:
        {
            AADrawer * drawer = (AADrawer *)a;
            //adding this case since i played a few games where Ai litterally decided to mill himself to death. fastest and easiest win ever.
            //this should help a little, tho ultimately it will be decided later what the best course of action is.
            //eff of drawing ability is calculated by base 20 + the amount of cards in library minus the amount of cards in hand times 7.
            //drawing is never going to return a hundred eff because later eff is multiplied by 1.3 if no cards in hand.
            efficiency = int(20 + p->game->library->nb_cards) - int(p->game->hand->nb_cards * 7);
            if (p->game->hand->nb_cards > 8)//reduce by 50 if cards in hand are over 8, high chance ai cant play them.
            {
                efficiency -= 70;
            }
            if ((drawer->getNumCards() >= p->game->library->nb_cards && (Targetable*)p == drawer->getTarget()) || (p->game->hand->nb_cards > 10 && (Targetable*)p == drawer->getTarget()))
            {
                //if the amount im drawing will mill me to death or i have more than 10 cards in hand, eff is 0;
                efficiency = 0;
            }
            break;
        }
    case MTGAbility::CLONING:
        {
            efficiency = 0;
            if(!target)
                efficiency = 100;//a clone ability with no target is an "clone all("
            else if (p == target->controller())
            {
                efficiency = 20 * target->DangerRanking();
            }
            break;
        }
    case MTGAbility::STANDARD_FIZZLER:
        {
            efficiency = 0; 

            if(!target)
                break;

            Interruptible * action = g->mLayers->stackLayer()->getAt(-1);
            if (!action)
                break;

            Spell * spell = dynamic_cast<Spell *>(action);
            if (!spell)
                break;

            Player * lastStackActionController = spell->source->controller();   
            if(p != target->controller() && lastStackActionController != p)
                efficiency = 60;//we want ai to fizzle at higher than "unknown" ability %.

            break;
        }
    default:
        if (target)
        {
            AbilityFactory af(g);
            int suggestion = af.abilityEfficiency(a, p, MODE_ABILITY,NULL,target);
            if (AADynamic * ady = dynamic_cast<AADynamic *>(a))
            {
                if(ady)
                {
                    //not going into massive detail with this ability, its far to complex, just going to give it a general idea.
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_COUNTERSONEONE)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_DEPLETE)
                        suggestion = BAKA_EFFECT_BAD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_DRAW)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_LIFEGAIN)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_LIFELOSS)
                        suggestion = BAKA_EFFECT_BAD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_PUMPBOTH)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_PUMPTOUGHNESS)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_PUMPPOWER)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_STRIKE)
                        suggestion = BAKA_EFFECT_BAD;
                }
            }
            if ((suggestion == BAKA_EFFECT_BAD && p == target->controller())
                || (suggestion == BAKA_EFFECT_GOOD && p != target->controller()))
            {
                efficiency = 0;
            }
            else
            {
                //without a base to start with Wrand % 5 almost always returns 0.
                efficiency = 10 + (owner->getRandomGenerator()->random() % 20); //Small percentage of chance for unknown abilities
            }
        }
        else
        {
            efficiency = 10 + (owner->getRandomGenerator()->random() % 30);
        }
        break;
    }
    if(AUpkeep * auk = dynamic_cast<AUpkeep *>(ability))
    {
        //hello, Ai pay your upcost please :P, this entices Ai into paying upcost, the conditional isAi() is required strangely ai is able to pay upcost during YOUR upkeep.
        if (auk && g->getCurrentGamePhase() == MTG_PHASE_UPKEEP && g->currentPlayer == p && p == a->source->controller())
        {
            efficiency = 100;
        }
    }
    else if (AAMover * aam = dynamic_cast<AAMover *>(a))
    {
        MTGGameZone * z = aam->destinationZone(target);
        if (target)
        {
            if (target->currentZone == p->game->library|| target->currentZone == p->opponent()->game->inPlay||target->currentZone == p->game->hand)
            {
                if (z == p->game->hand || z == p->game->inPlay || z == target->controller()->game->hand)
                    efficiency = 100;
            }
            else if( target->currentZone == p->game->inPlay && (MTGCardInstance*)target == a->source)
            {
                if (z == p->game->hand)
                    efficiency = 10 + (owner->getRandomGenerator()->random() % 10);//random chance to bounce their own card;
            }
            // We don't want to return cards in play to own hand, save rare combos
            else if(target->currentZone == p->game->inPlay)
            {
                if (z == p->game->hand || z == p->game->library)
                    efficiency = (owner->getRandomGenerator()->random() % 10);//random chance to bounce their own card;
            }
            else
            {
                efficiency = 10 + (owner->getRandomGenerator()->random() % 5);
            }
        }
        else
        {
            // We don't want to return the ability source cards that are in play to own hand, save rare combos
            // cards like Blinking Spirit used to be auto lose for AI
            if(z == p->game->hand || z == p->game->library)
                efficiency = 1;
            else efficiency = 50;
            //may abilities target the source until thier nested ability is activated, so 50% chance to use this
            //mover, until we can come up with something more elegent....
        }
    }
    else if (dynamic_cast<AAProliferate *>(a))
    {
        if (playerAbilityTarget && playerAbilityTarget != p)
        {
            efficiency = 60;//ai determines if the counters are good or bad on menu check.
        }
        else
            efficiency = 90;
    }
    else if (dynamic_cast<AAAlterPoison *>(a))
    {
        if (playerAbilityTarget && playerAbilityTarget != p)
        {
            efficiency = 90;
        }
    }
    else if (dynamic_cast<AAAlterEnergy *>(a))
    {
        if (playerAbilityTarget && playerAbilityTarget == p)
        {
            efficiency = 90;
        }
    }
    else if (dynamic_cast<AATurnSide *>(a))
    {
        // Doubleside (modal-DFC "Flip Side" in hand) is scored 0: the AI does
        // not proactively toggle which face a hand card DISPLAYS. The old code
        // here toggled ats->source->isFlipped via std::rand() as a "randomly
        // consider the other face" heuristic - but SCORING must never mutate
        // game state (getEfficiency runs every ranking pass, on both the Baka
        // and the GPT seat), and this toggle desynced the isFlipped FLAG from
        // the displayed face: paired with AATurnSide::resolve's AI flag-reset
        // it let the flag oscillate forever without ever flipping the display
        // or casting (the flip-thrash livelock, wave-27). It also never
        // produced a correct back-face cast - a flag-only flip does not swap
        // the card's name/cost, so nothing downstream actually cast the back.
        // The back face is cast directly via its alternative-cost option in the
        // cast menu (the modal-DFC alt-cost path), so the display toggle is not
        // needed to reach it. No mutation, no rand: efficiency stays 0.
        efficiency = 0;
    }
    else if (ATokenCreator * atc = dynamic_cast<ATokenCreator *>(a))
    {
        efficiency = 80;
        if(atc->name.length() && atc->sabilities.length() && atc->types.size() && p->game->inPlay->findByName(atc->name))
        {
            for (list<int>::const_iterator it = atc->types.begin(); it != atc->types.end(); ++it)
            {
                if(*it == Subtypes::TYPE_LEGENDARY)//ai please stop killing voja!!! :P
                    efficiency = 0;
            }
        }
        if(p->game->battlefield->countByType("token") >= 25)
            efficiency = 0;

    }
    else if (GenericRevealAbility * grA = dynamic_cast<GenericRevealAbility *>(a))
    {
        if(grA->source->getAICustomCode().size() && grA->source->alias != 185709)//Sphinx of Jwar Isle so the ai will ignore it
        {
            //efficiency = 45 + (owner->getRandomGenerator()->random() % 50);
            AbilityFactory af(g);
            MTGAbility * parsedAICC = af.parseMagicLine(cReplaceString(grA->source->getAICustomCode(),"activate",""),0,NULL,grA->source);
            efficiency = getRevealedEfficiency(parsedAICC);
            SAFE_DELETE(parsedAICC);
        }
        else // this is why the AI never chooses any card at all? reveal is used to get cards so it should be at better value
            efficiency = 60;
    }
    else if (GenericScryAbility * grA = dynamic_cast<GenericScryAbility *>(a))
    {
        if(grA->source->getAICustomCode().size())
        {           
            AbilityFactory af(g);
            MTGAbility * parsedAICC = af.parseMagicLine(cReplaceString(grA->source->getAICustomCode(),"activate",""),0,NULL,grA->source);
            efficiency = getRevealedEfficiency(parsedAICC);
            SAFE_DELETE(parsedAICC);
        }
        else // this is why the AI never chooses any card at all? scry is used to get cards so it should be at better value
            efficiency = 60;
    }
    //At this point the "basic" efficiency is computed, we further tweak it depending on general decisions, independent of theAbility type

    MayAbility * may = dynamic_cast<MayAbility*>(ability);
    if (!efficiency && may)
    {
        AIPlayer * chk = (AIPlayer*)p;
        if(may->ability && may->ability->getActionTc() && chk->chooseTarget(may->ability->getActionTc(),NULL,NULL,true))
            efficiency = 50 + (owner->getRandomGenerator()->random() % 50);
    }
    if (p->game->hand->nb_cards == 0)
        efficiency = (int) ((float) efficiency * 1.3); //increase chance of using ability if hand is empty
    ManaCost * cost = ability->getCost();
    if (cost)
    {
        ExtraCosts * ec = cost->extraCosts;
        if (ec)
        {
            for(unsigned int i = 0; i < ec->costs.size();i++)
            {
                ExtraCost * tapper = dynamic_cast<TapCost*>(ec->costs[i]);
                ExtraCost * sacrifice = dynamic_cast<SacrificeCost*>(ec->costs[i]);
                if(tapper)
                    continue;                
                else if(sacrifice)
                    efficiency = efficiency / 10;
                else
                    efficiency = efficiency / 2;
            }
            //Decrease chance of using ability if there is an extra cost to use the ability, ignore tap
        }
    }
    if (dynamic_cast<MTGPutInPlayRule *>(a))
    {
        efficiency += 65;
    }
    else if (dynamic_cast<MTGAlternativeCostRule *>(a))
    {
        efficiency += 55;
    }
    else if (dynamic_cast<MTGSuspendRule *>(a))
    {
        efficiency += 55;
    }

    if (ability->source)
    {
        if(ability->source->hasType(Subtypes::TYPE_PLANESWALKER) || ability->source->hasType(Subtypes::TYPE_BATTLE))
            efficiency += 50;
        else if(ability->source->hasType(Subtypes::TYPE_LAND))
        { // probably a shockland, don't pay life if hand is empty
            if (p->life<=2)
                // check that's not a manland(like Celestial Colonnade)
                if(efficiency < 50)
                    efficiency = 0;
        } 
    }

    SAFE_DELETE(transAbility);
    return efficiency;
}

int OrderedAIAction::getRevealedEfficiency(MTGAbility * ability2)
{
    int eff2 = 0;
    if (!ability2)
        return 0;
    GameObserver * g = owner->getObserver();
    ActionStack * s = g->mLayers->stackLayer();
    int currentPhase = g->getCurrentGamePhase();

    Player * p = g->currentlyActing();
    if (s->has(ability2))
        return 0;
    MTGAbility * a = AbilityFactory::getCoreAbility(ability2);
    MTGAbility * transAbility = NULL;
    if(ATransformerInstant * atia = dynamic_cast<ATransformerInstant *>(a))
    {
        if(atia->newAbilityFound)
        {
            AbilityFactory af(g);
            transAbility = af.parseMagicLine(atia->newAbilitiesList[atia->newAbilitiesList.size()-1], 0, NULL, atia->source);
            transAbility->target = ability2->target;
            a = transAbility;
        }
    }
    if (!a)
    {
        DebugTrace("FATAL: Ability is NULL in AIAction::getEfficiency()");
        return 0;
    }

    if (!((AIPlayerBaka *)owner)->canHandleCost(ability2))
    {
        SAFE_DELETE(transAbility);
        return 0;
    }
    MTGCardInstance * coreAbilityCardTarget = dynamic_cast<MTGCardInstance *>(a->target);

    //CoreAbility shouldn't return a Lord, but it does.
    //When we don't have a target for a lord action, we assume it's the lord itself
    if (!target && dynamic_cast<ALord*> (a))
    {
        target = a->source;
    }

    AACastCard * CC = dynamic_cast<AACastCard*> (a);
    if (CC)
        return 99;

    switch (a->aType)
    {
    case MTGAbility::DAMAGER:
        {
            eff2 =  getEfficiency ((AADamager *) a);
            break;
        }
    case MTGAbility::STANDARD_REGENERATE:
        {
            eff2 = 0;
            if (!coreAbilityCardTarget)
                break;

            if (!coreAbilityCardTarget->regenerateTokens && currentPhase == MTG_PHASE_COMBATBLOCKERS
                && (coreAbilityCardTarget->defenser || coreAbilityCardTarget->blockers.size())
                && !regenerationPendingFor(g, coreAbilityCardTarget)
                )
            {
                eff2 = 95;
            }
            else if (!coreAbilityCardTarget->regenerateTokens
                     && !regenerationPendingFor(g, coreAbilityCardTarget))
            {
                //A regeneration shield replaces the destruction that lethal
                //damage causes, so it answers a burn spell exactly as it
                //answers a lethal blocker - and the stack is the only place
                //that damage is visible before it lands. life is the card's
                //REMAINING toughness (damage is subtracted from it), so lethal
                //is measured against that, not the printed value.
                int pendingDamage = pendingStackDamageTo(g, coreAbilityCardTarget);
                if (pendingDamage > 0 && pendingDamage >= coreAbilityCardTarget->life)
                    eff2 = 95;
            }
            break;
        }
    case MTGAbility::STANDARD_PREVENT:
        {
            eff2 = 0;//starts out low to avoid spamming it when its not needed.

            if (!target)
                break;

            if (currentPhase == MTG_PHASE_COMBATBLOCKERS)
            {
                bool NeedPreventing = false;
                MTGCardInstance * nextOpponent = target->getNextOpponent();
                if(!nextOpponent)
                    break;
                if ((target->defenser || target->blockers.size()) && target->preventable < nextOpponent->power)
                    NeedPreventing = true;
                if (p == target->controller() && target->controller()->isAI() && NeedPreventing  && !(nextOpponent->has(Constants::DEATHTOUCH) || nextOpponent->has(Constants::PERPETUALDEATHTOUCH) || nextOpponent->has(Constants::WITHER)))
                {
                    eff2 = 20 * (target->DangerRanking());//increase this chance to be used in combat if the creature blocking/blocked could kill the creature this chance is taking into consideration how good the creature is, best creature will always be the first "saved"..
                    if (target->toughness == 1 && nextOpponent->power == 1)
                        eff2 += 15;
                    //small bonus added for the poor 1/1s, if we can save them, we will unless something else took precidence.
                    //note is the target is being blocked or blocking a creature with wither or deathtouch, it is not even considered for preventing as it is a waste.
                    //if its combat blockers, it is being blocked or blocking, and has less prevents the the amount of damage it will be taking, the effeincy is increased slightly and totalled by the danger rank multiplier for final result.
                    if((target->defenser || target->blockers.size()) && target->controller() == p)
                    {
                        int damages = nextOpponent->power;
                        int calculateAfterDamage = target->toughness - damages;
                        if((calculateAfterDamage + target->preventable) > 0)
                        {
                            eff2 = 0;
                            //this is to avoid wasting prevents on creatures that will already survive.
                            //this should take into account bushido and flanking as this check is run after every trigger.
                        }
                    }
                }
            }
            else if (p == target->controller() && target->controller()->isAI())
            {
                //Off-combat answer: damage already on the stack aimed at this
                //card. Prevention only earns its card if it turns lethal into
                //survivable - preventing 2 of a 5-damage bolt on a 3-toughness
                //creature loses the creature AND the prevention.
                int pendingDamage = pendingStackDamageTo(g, target);
                if (pendingDamage > 0 && (pendingDamage - target->preventable) >= target->life)
                {
                    AADamagePrevent * prevention = dynamic_cast<AADamagePrevent *>(a);
                    int wouldPrevent = prevention ? prevention->preventing : 0;
                    if (wouldPrevent > 0
                        && (target->life + target->preventable + wouldPrevent - pendingDamage) > 0)
                        eff2 = 20 * (target->DangerRanking());
                }
            }
            break;
        }
    case MTGAbility::STANDARD_EQUIP:
        {

            eff2 = 0;
            if (!target)
                break;

            unsigned int equips = p->game->battlefield->countByType("Equipment");
            unsigned int myArmy = p->game->battlefield->countByType("Creature");
            // when can this ever be negative?
            int equalized = myArmy ? equips / myArmy : 0;

            if (p == target->controller() && target->equipment <= 1 && !a->source->target)
            {
                eff2 = 20 * (target->DangerRanking());
                if (target->hasColor(Constants::MTG_COLOR_WHITE))
                    eff2 += 20;//this is to encourage Ai to equip white creatures in a weenie deck. ultimately it will depend on what had the higher dangerranking.
                if (target->power == 1 && target->toughness == 1 && target->isToken == 0)
                    eff2 += 10; //small bonus to encourage equipping nontoken 1/1 creatures.
            }

            if (p == target->controller() && !a->source->target && target->equipment < equalized)
            {
                eff2 = 15 * (target->DangerRanking());
                eff2 -= 5 * (target->equipment);
            }

            if ( eff2 < 20 && eff2 > 0 )
                eff2 += target->controller()->getObserver()->getRandomGenerator()->random() % 30;
            break;
        }
    case MTGAbility::STANDARD_LEVELUP:
        {
            eff2 = 0;
            Counter * targetCounter = NULL;
            int currentlevel = 0;

            if (!coreAbilityCardTarget)
                break;

            if (coreAbilityCardTarget->counters && coreAbilityCardTarget->counters->hasCounter("level", 0, 0))
            {
                targetCounter = coreAbilityCardTarget->counters->hasCounter("level", 0, 0);
                currentlevel = targetCounter->nb;
            }
            if (currentlevel < coreAbilityCardTarget->MaxLevelUp)
            {
                eff2 = 85;
                //increase the efficeincy of leveling up by a small amount equal to current level.
                eff2 += currentlevel;

                if (p->game->hand->nb_cards > 0 && p->isAI())
                {
                    eff2 -= (10 * p->game->hand->nb_cards);//reduce the eff if by 10 times the amount of cards in Ais hand.
                    //it should always try playing more cards before deciding
                }

                if (g->getCurrentGamePhase() == MTG_PHASE_SECONDMAIN)
                {
                    eff2 = 100;
                    //in 2nd main, go all out and try to max stuff.
                }
            }

            break;
        }
    case MTGAbility::COUNTERS:
        {
            MTGCardInstance * _target = target ? target : coreAbilityCardTarget;
            eff2 = 0;

            if (!_target)
                break;

            if(AACounter * cc = dynamic_cast<AACounter*> (a))
            {
                if(_target->controller() == p && cc->toughness>=0)
                {
                    eff2 = 90;

                }
                if(_target->controller() != p && ((_target->toughness + cc->toughness <= 0 && _target->toughness) || (cc->toughness < 0 && cc->power < 0)))
                {
                    eff2 = 90;

                }
                if(_target->counters && _target->counters->hasCounter(cc->power,cc->toughness) && _target->counters->hasCounter(cc->power,cc->toughness)->nb > 15)
                {
                    eff2 = _target->counters->hasCounter(cc->power,cc->toughness)->nb;
                }
                if(cc->maxNb && _target->counters && _target->counters->hasCounter(cc->power,cc->toughness)->nb >= cc->maxNb) 
                    eff2 = 0;
                if(a->target == a->source && a->getCost() && a->getCost()->hasX())
                    eff2 -= 10 * int(p->game->hand->cards.size());
            }
            break;
        }
    case MTGAbility::STANDARD_PUMP:
        {
            eff2 = 0;
            if(!coreAbilityCardTarget)
                break;
            if(!target && !dynamic_cast<ALord*> (a) && (((MTGCardInstance *)a->source)->hasSubtype(Subtypes::TYPE_AURA) || ((MTGCardInstance *)a->source)->hasSubtype(Subtypes::TYPE_EQUIPMENT)))
            {
                if(a->source->target)
                    coreAbilityCardTarget = a->source->target; //TODO use intermediate value?
                target = a->source;
            }
            if (!target && !dynamic_cast<ALord*> (a))
                break;
            if(dynamic_cast<ALord*> (a) && !target)
            {
                target = a->source;
            }

            AbilityFactory af(g);
            int suggestion = af.abilityEfficiency(a, p, MODE_ABILITY);
            //i do not set a starting eff. on this ability, this allows Ai to sometimes randomly do it as it normally does.
            int currentPhase = g->getCurrentGamePhase();
            if ((currentPhase == MTG_PHASE_COMBATBLOCKERS) || (currentPhase == MTG_PHASE_COMBATATTACKERS))
            {
                if (suggestion == BAKA_EFFECT_GOOD && target->controller() == p)
                {
                    if(coreAbilityCardTarget->defenser || coreAbilityCardTarget->blockers.size())
                    {
                        MTGCardInstance * opponent = coreAbilityCardTarget->getNextOpponent();
                        if (!opponent)
                            break;

                        if (coreAbilityCardTarget->power < opponent->toughness ||( coreAbilityCardTarget->toughness < opponent->power) || (coreAbilityCardTarget->has(Constants::TRAMPLE)))
                        {
                            //this pump is based on a start eff. of 20 multiplied by how good the creature is.
                            eff2 = 20 * coreAbilityCardTarget->DangerRanking();
                        }
                    }
                    if (coreAbilityCardTarget->isAttacker() && !coreAbilityCardTarget->blockers.size())
                    {
                        //this means im heading directly for the player, pump this creature as much as possible.
                        eff2 = 100;
                        if(coreAbilityCardTarget->power > 50)
                            eff2 -= coreAbilityCardTarget->power;//we don't need to go overboard. better to not put all your eggs in a single basket.
                    }
                }
            }
            if (suggestion == BAKA_EFFECT_BAD && target->controller() != p && target->toughness > 0)
            {
                eff2 = 100;
            }
            break;
        }
    case MTGAbility::STANDARD_BECOMES:
        {
            if(!coreAbilityCardTarget)
                break;

            //nothing huge here, just ensuring that Ai makes his noncreature becomers into creatures during first main, so it can actually use them in combat.
            if (coreAbilityCardTarget && !coreAbilityCardTarget->isCreature() && currentPhase == MTG_PHASE_FIRSTMAIN)
            {
                eff2 = 70;
            }
            break;
        }
    case MTGAbility::MANA_PRODUCER://only way to hit this condition is nested manaabilities, ai skips manaproducers by defualt when finding an ability to use.
        {
            AManaProducer * manamaker = dynamic_cast<AManaProducer*>(a);
            GenericActivatedAbility * GAA = dynamic_cast<GenericActivatedAbility*>(ability2);
            AForeach * forMana = dynamic_cast<AForeach*>(GAA->ability);
            if (manamaker && forMana)
            {
                int outPut = forMana->checkActivation();
                if (ability2->getCost() && outPut > int(ability2->getCost()->getConvertedCost() +1) && currentPhase == MTG_PHASE_FIRSTMAIN && ability2->source->controller()->game->hand->nb_cards > 1)
                    eff2 = 60;//might be a bit random, but better than never using them.
            }
            else
                eff2 = 0;
            break;
        }
    case MTGAbility::STANDARDABILITYGRANT:
        {
            eff2 = 0;

            if (!target)
                break;

            //ensuring that Ai grants abilities to creatures during first main, so it can actually use them in combat.
            //quick note: the eff is multiplied by creatures ranking then divided by the number of cards in hand.
            //the reason i do this is to encourage more casting and less waste of mana on abilities.
            AbilityFactory af(g);
            int suggestion = af.abilityEfficiency(a, p, MODE_ABILITY);

            int eff2Modifier = (25 * target->DangerRanking());
            if (p->game->hand->nb_cards > 1)
            {
                eff2Modifier -= p->game->hand->nb_cards*3;
            }
            if (suggestion == BAKA_EFFECT_BAD && p != target->controller() && !target->has(a->abilitygranted))
            {
                eff2 += eff2Modifier;
            }

            if (!target->has(a->abilitygranted) && g->getCurrentGamePhase() == MTG_PHASE_COMBATBEGIN
                && p == target->controller()
                )
            {
                eff2 += eff2Modifier;
            }

            if (suggestion == BAKA_EFFECT_GOOD && target->has(a->abilitygranted))
            {
                //trying to avoid Ai giving ie:flying creatures ie:flying twice.
                eff2 = 0;
            }

            if ((suggestion == BAKA_EFFECT_BAD && p == target->controller()) 
                || (suggestion == BAKA_EFFECT_GOOD && p != target->controller())
                )
            {
                eff2 = 0;
                //stop giving trample to the players creatures.
            }
            break;
        }

    case MTGAbility::UNTAPPER:
        //untap things that Ai owns and are tapped.
        {
            eff2 = 0;
            if (!target)
                break;

            if (target->isTapped() && target->controller() == p)
            {
                eff2 = target->isCreature()? (20 * target->DangerRanking()) : 100;
            }
            break;
        }

    case MTGAbility::TAPPER:
        //tap things the player owns and that are untapped.
        {
            if (!target)
                break;

            if (target->controller() != p)
                eff2 = (20 * target->DangerRanking());

            if (target->isTapped())
                eff2 = 0;

            break;
        }

    case MTGAbility::LIFER:
        {
            //use life abilities whenever possible.
            AALifer * alife = (AALifer *) a;
            Targetable * _t = alife->getTarget();

            eff2 = 100;
            AbilityFactory af(g);
            int suggestion = af.abilityEfficiency(a, p, MODE_ABILITY);

            if(MTGCardInstance * cTarget = dynamic_cast<MTGCardInstance *>(_t))
            {
                if((suggestion == BAKA_EFFECT_BAD && (cTarget)->controller() == p) || (suggestion == BAKA_EFFECT_GOOD && (cTarget)->controller() != p))
                    eff2 = 0;
            }
            else if ((suggestion == BAKA_EFFECT_BAD && _t == p) || (suggestion == BAKA_EFFECT_GOOD && _t != p))
            {
                eff2 = 0;
            }

            break;
        }
    case MTGAbility::STANDARD_DRAW:
        {
            AADrawer * drawer = (AADrawer *)a;
            //adding this case since i played a few games where Ai litterally decided to mill himself to death. fastest and easiest win ever.
            //this should help a little, tho ultimately it will be decided later what the best course of action is.
            //eff of drawing ability is calculated by base 20 + the amount of cards in library minus the amount of cards in hand times 7.
            //drawing is never going to return a hundred eff because later eff is multiplied by 1.3 if no cards in hand.
            eff2 = int(20 + p->game->library->nb_cards) - int(p->game->hand->nb_cards * 7);
            if (p->game->hand->nb_cards > 8)//reduce by 50 if cards in hand are over 8, high chance ai cant play them.
            {
                eff2 -= 70;
            }
            if ((drawer->getNumCards() >= p->game->library->nb_cards && (Targetable*)p == drawer->getTarget()) || (p->game->hand->nb_cards > 10 && (Targetable*)p == drawer->getTarget()))
            {
                //if the amount im drawing will mill me to death or i have more than 10 cards in hand, eff is 0;
                eff2 = 0;
            }
            break;
        }
    case MTGAbility::CLONING:
        {
            eff2 = 0;
            if(!target)
                eff2 = 100;//a clone ability with no target is an "clone all("
            else if (p == target->controller())
            {
                eff2 = 20 * target->DangerRanking();
            }
            break;
        }
    case MTGAbility::STANDARD_FIZZLER:
        {
            eff2 = 0; 

            if(!target)
                break;

            Interruptible * action = g->mLayers->stackLayer()->getAt(-1);
            if (!action)
                break;

            Spell * spell = dynamic_cast<Spell *>(action);
            if (!spell)
                break;

            Player * lastStackActionController = spell->source->controller();   
            if(p != target->controller() && lastStackActionController != p)
                eff2 = 60;//we want ai to fizzle at higher than "unknown" ability %.

            break;
        }
    default:
        if (target)
        {
            AbilityFactory af(g);
            int suggestion = af.abilityEfficiency(a, p, MODE_ABILITY,NULL,target);
            if (AADynamic * ady = dynamic_cast<AADynamic *>(a))
            {
                if(ady)
                {
                    //not going into massive detail with this ability, its far to complex, just going to give it a general idea.
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_COUNTERSONEONE)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_DEPLETE)
                        suggestion = BAKA_EFFECT_BAD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_DRAW)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_LIFEGAIN)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_LIFELOSS)
                        suggestion = BAKA_EFFECT_BAD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_PUMPBOTH)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_PUMPTOUGHNESS)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_PUMPPOWER)
                        suggestion = BAKA_EFFECT_GOOD;
                    if(ady->effect == ady->DYNAMIC_ABILITY_EFFECT_STRIKE)
                        suggestion = BAKA_EFFECT_BAD;
                }
            }
            if ((suggestion == BAKA_EFFECT_BAD && p == target->controller())
                || (suggestion == BAKA_EFFECT_GOOD && p != target->controller()))
            {
                eff2 = 0;
            }
            else
            {
                //without a base to start with Wrand % 5 almost always returns 0.
                eff2 = 10 + (owner->getRandomGenerator()->random() % 20); //Small percentage of chance for unknown abilities
            }
        }
        else
        {
            eff2 = 10 + (owner->getRandomGenerator()->random() % 30);
        }
        break;
    }
    if(AUpkeep * auk = dynamic_cast<AUpkeep *>(ability2))
    {
        //hello, Ai pay your upcost please :P, this entices Ai into paying upcost, the conditional isAi() is required strangely ai is able to pay upcost during YOUR upkeep.
        if (auk && g->getCurrentGamePhase() == MTG_PHASE_UPKEEP && g->currentPlayer == p && p == a->source->controller())
        {
            eff2 = 100;
        }
    }
    else if (AAMover * aam = dynamic_cast<AAMover *>(a))
    {
        MTGGameZone * z = aam->destinationZone(target);
        if (target)
        {
            if (target->currentZone == p->game->library|| target->currentZone == p->opponent()->game->inPlay||target->currentZone == p->game->hand)
            {
                if (z == p->game->hand || z == p->game->inPlay || z == target->controller()->game->hand)
                    eff2 = 100;
            }
            else if( target->currentZone == p->game->inPlay && (MTGCardInstance*)target == a->source)
            {
                if (z == p->game->hand)
                    eff2 = 10 + (owner->getRandomGenerator()->random() % 10);//random chance to bounce their own card;
            }
            else
            {
                eff2 = 10 + (owner->getRandomGenerator()->random() % 5);
            }
        }
        else
        {
            eff2 = 50;
            //may abilities target the source until thier nested ability is activated, so 50% chance to use this
            //mover, until we can come up with something more elegent....
        }
    }
    else if (dynamic_cast<AAProliferate *>(a))
    {
        if (playerAbilityTarget && playerAbilityTarget != p)
        {
            eff2 = 60;//ai determines if the counters are good or bad on menu check.
        }
        else
            eff2 = 90;
    }
    else if (dynamic_cast<AAAlterPoison *>(a))
    {
        if (playerAbilityTarget && playerAbilityTarget != p)
        {
            eff2 = 90;
        }
    }
    else if (dynamic_cast<AAAlterEnergy *>(a))
    {
        if (playerAbilityTarget && playerAbilityTarget == p)
        {
            eff2 = 90;
        }
    }
    else if (ATokenCreator * atc = dynamic_cast<ATokenCreator *>(a))
    {
        eff2 = 80;
        if(atc->name.length() && atc->sabilities.length() && atc->types.size() && p->game->inPlay->findByName(atc->name))
        {
            for (list<int>::const_iterator it = atc->types.begin(); it != atc->types.end(); ++it)
            {
                if(*it == Subtypes::TYPE_LEGENDARY)//ai please stop killing voja!!! :P
                    eff2 = 0;
            }
        }
        if(p->game->battlefield->countByType("token") >= 25)
            eff2 = 0;

    }
    //At this point the "basic" eff2 is computed, we further tweak it depending on general decisions, independent of theAbility type

    MayAbility * may = dynamic_cast<MayAbility*>(ability2);
    if (!eff2 && may)
    {
        AIPlayer * chk = (AIPlayer*)p;
        if(may->ability && may->ability->getActionTc() && chk->chooseTarget(may->ability->getActionTc(),NULL,NULL,true))
            eff2 = 50 + (owner->getRandomGenerator()->random() % 50);
    }
    if (p->game->hand->nb_cards == 0)
        eff2 = (int) ((float) eff2 * 1.3); //increase chance of using ability if hand is empty
    ManaCost * cost = ability2->getCost();
    if (cost)
    {
        ExtraCosts * ec = cost->extraCosts;
        if (ec)
        {
            for(unsigned int i = 0; i < ec->costs.size();i++)
            {
                ExtraCost * tapper = dynamic_cast<TapCost*>(ec->costs[i]);
                if(tapper)
                    continue;
                else
                    eff2 = eff2 / 2;
            }
            //Decrease chance of using ability if there is an extra cost to use the ability, ignore tap
        }
    }
    if (dynamic_cast<MTGPutInPlayRule *>(a))
    {
        eff2 += 65;
    }
    else if (dynamic_cast<MTGAlternativeCostRule *>(a))
    {
        eff2 += 55;
    }
    else if (dynamic_cast<MTGSuspendRule *>(a))
    {
        eff2 += 55;
    }
    SAFE_DELETE(transAbility);
    return eff2;
}

int AIPlayerBaka::getEfficiency(OrderedAIAction * action)
{
    return action->getEfficiency();
}

//
// Abilities/Target Selection
//


MTGCardInstance * AIPlayerBaka::chooseCard(TargetChooser * tc, MTGCardInstance * source, int)
{
    MTGPlayerCards * playerZones = source->controller()->game;
    if (comboHint && comboHint->cardTargets.size())
    {
        tc = GetComboTc(observer,tc);
    }
    for(int players = 0; players < 2;++players)
    {
        MTGGameZone * zones[] = { playerZones->hand, playerZones->library, playerZones->inPlay, playerZones->graveyard, playerZones->stack, playerZones->exile, playerZones->commandzone };
        for (int j = 0; j < 7; j++)
        {
            MTGGameZone * zone = zones[j];
            for (int k = 0; k < zone->nb_cards; k++)
            {
                MTGCardInstance * card = zone->cards[k];
                if (card != source && card != tc->source && !tc->alreadyHasTarget(card) && tc->canTarget(card))
                {
                    return card;
                }
            }
        }
        //switch player->zones to the other player and check there if we haven't found one yet.
        playerZones = source->controller()->opponent()->game;
    }
    return NULL;
}

bool AIPlayerBaka::payTheManaCost(ManaCost * cost, int anytypeofmana, MTGCardInstance * target,vector<MTGAbility*>gotPayments)
{
    DebugTrace("AIPlayerBaka: AI attempting to pay a mana cost." << endl
        << "-  Target: " << (target ? target->name : "None" ) << endl
        << "-  Cost: " << (cost ? cost->toString() : "NULL") );

    if (!cost)
    {
        DebugTrace("AIPlayerBaka: Mana cost is NULL.  ");
        return false;
    }

    ExtraCosts * ec = cost->extraCosts;
    if(!ec && observer->mExtraPayment)
        ec = observer->mExtraPayment;
    if (ec)
    {
        for (size_t i = 0; i < ec->costs.size(); ++i)
        {
            if (ec->costs[i]->tc)
            {
                ec->costs[i]->setSource(target);
                if(!ec->costs[i]->tc->countValidTargets())
                    return false;
                //chooseCostTarget is the DECISION seam (model-overridable);
                //NULL = no choice yet or none possible - feeding NULL into
                //tryToSetPayment would false-match an unset cost target.
                MTGCardInstance * costTarget = chooseCostTarget(ec->costs[i]->tc,target);
                //The seam can be ASYNCHRONOUS: AIPlayerGPT issues a model call
                //and returns NULL until it answers. But payTheManaCost is
                //synchronous and its callers commit-or-abandon this tick, so an
                //unanswered pick aborted the whole cast - the alternative-cost
                //spell (Force of Negation via "exile a blue card") never went
                //onto the stack and the countered spell resolved, while the
                //cast re-offered every priority window (2 seats, corpus
                //20260715). The which-card-to-exile choice is minor next to the
                //already-made decision to cast; fall back to the synchronous
                //heuristic so the extra cost is PAID this tick and the cast
                //completes rather than looping forever.
                if (!costTarget)
                    costTarget = AIPlayerBaka::chooseCostTarget(ec->costs[i]->tc, target);
                if (!costTarget)
                    return false;
                int checkTarget = 0;
                //After a payment is set, tryToSetPayment still returns nonzero,
                //so the loop fetches ANOTHER cost target. A single-target extra
                //cost (e.g. Force of Negation's "exile a blue card") has no
                //second legal pick once its lone target is taken, so this used
                //to abort a COMPLETED payment. Break instead and let the
                //isPaymentSet check below reject genuinely-unpaid costs.
                while (ec->tryToSetPayment(costTarget))
                {
                    costTarget = chooseCostTarget(ec->costs[i]->tc,target);
                    if (!costTarget)
                        break;
                    if(checkTarget == 20)
                        break;
                    checkTarget++;
                }
                if(!ec->costs[i]->isPaymentSet())
                {
                    //CONVOKE is a REDUCTION, not a stand-alone cost: the tapped
                    //creatures cover part of the printed cost and the REMAINDER
                    //is paid from mana. Convoke::isPaymentSet checks whether the
                    //LIVE pool affords that reduced remainder, but the AI pays
                    //synchronously here with an EMPTY pool (its mana taps are
                    //queued clicks that fire on later ticks), so any convoke cast
                    //not fully covered by creatures failed this gate and aborted -
                    //the card reverted to hand and was re-picked for turns
                    //(Venerated Loxodon 2/23, March of the Multitudes 0/17,
                    //corpus 20260725). Float the reduced remainder from lands NOW:
                    //queue the producer clicks BEFORE the caller queues the card
                    //click, so by the time MTGAlternativeCostRule::reactToClick
                    //re-checks isExtraPaymentSet the pool holds the reduced mana
                    //and the cast completes (creatures tap via Convoke::doPay).
                    Convoke * conv = dynamic_cast<Convoke*>(ec->costs[i]);
                    if (conv && ec->costs[i]->tc && ec->costs[i]->tc->getNbTargets() > 0)
                    {
                        ManaCost * reduced = conv->getReduction();
                        bool floated = false;
                        if (reduced)
                        {
                            int anyt = target->has(Constants::ANYTYPEOFMANAABILITY);
                            if (target->controller()->getManaPool()->canAfford(reduced, anyt))
                                floated = true; //creatures + existing pool already cover it
                            else
                            {
                                vector<MTGAbility*> landPlan = canPayMana(target, reduced, anyt);
                                if (landPlan.size())
                                {
                                    for (size_t lp = 0; lp < landPlan.size(); ++lp)
                                    {
                                        if (AManaProducer * amp = dynamic_cast<AManaProducer*>(landPlan[lp]))
                                            clickstream.push(NEW AIAction(this, amp, amp->source));
                                        else if (GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility*>(landPlan[lp]))
                                            clickstream.push(NEW AIAction(this, gmp, gmp->source));
                                    }
                                    floated = true;
                                }
                            }
                            SAFE_DELETE(reduced);
                        }
                        if (!floated)
                            return false;
                        continue; //remainder queued; convoke targets are set on the tc
                    }
                    return false;
                }
            }
        }
    }

    if(!cost->getConvertedCost())
    {
        //N-122b sub-item: this trace used to sit HERE, above the test it
        //describes, so it fired for EVERY zero-mana payment - every land drop
        //the AI successfully makes, and every free ability - announcing a
        //land-drop refusal that had not been decided and usually was not
        //happening (the very next lines normally return true). It is the
        //`AIPlayerBaka: Card was a land and ai cant play any more lands this
        //turn.` line the deck122 notes flagged as firing immediately after
        //every GPT land pick. Benign to the engine, but it is a false signal in
        //a log that reviewers read as ground truth, so it moves inside the
        //branch that actually refuses.
        //
        //The land-per-turn restriction applies only to PLAYING a land from
        //hand. A fetchland crack (and any activated ability whose source is a
        //land already on the battlefield) is NOT a land play - its {T}/Sac/Life
        //cost carries no mana, so it reaches this zero-cost branch, but gating
        //it on canPutIntoZone silently DROPPED the whole activation whenever the
        //turn's land drop was spent (ENGINE-F1: the fetch was chosen + narrated,
        //but payTheManaCost returned false so the AIAction was never queued, so
        //the crack fizzled with no event and re-armed next turn). Only refuse a
        //land that is NOT yet in play (i.e. an actual play-from-hand).
        if (target && target->isLand() && !target->isInPlay(observer)
            && game->playRestrictions->canPutIntoZone(target, game->battlefield) == PlayRestriction::CANT_PLAY)
        {
            DebugTrace("AIPlayerBaka: Card was a land and ai cant play any more lands this turn.  ");
            return false;
        }
        DebugTrace("AIPlayerBaka: Card or Ability was free to play.  ");
        if(!cost->hasX())//don't return true if it contains {x} but no cost, locks ai in a loop. ie oorchi hatchery cost {x}{x} to play.
            return true;
        //return true if cost does not contain "x" becuase we don't need to do anything with a cost of 0;
    }
    if(gotPayments.size())
    {
        DebugTrace("AIPlayerBaka: Ai had a payment in mind.");
        ManaCost * paid = NEW ManaCost();
        vector<AIAction*>clicks;

        for(size_t k = 0; k < gotPayments.size(); ++k)
        {
            if ( AManaProducer * amp = dynamic_cast<AManaProducer*> (gotPayments[k]))
            {
                AIAction * action = NEW AIAction(this, amp,amp->source);
                clicks.push_back(action);
                paid->add(amp->output);
            }
            else if(GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility*>(gotPayments[k]))
            {
                AIAction * action = NEW AIAction(this, gmp,gmp->source);
                clicks.push_back(action);
                if(AForeach * fmp = dynamic_cast<AForeach*>(gmp->ability))
                {
                    amp = dynamic_cast<AManaProducer*> (fmp->ability);
                    int outPut = fmp->checkActivation();
                    for(int k = 0; k < outPut; ++k)
                        paid->add(amp->output);
                }
            }
            if(k == gotPayments.size()-1)//only add it once, and at the end.
                paid->add(this->getManaPool());//incase some of our payments were mana already in the pool/.
            if(paid->canAfford(cost, anytypeofmana))
            {
                if((!cost->hasX() && !cost->hasAnotherCost()) || k == gotPayments.size()-1)
                {
                    SAFE_DELETE(paid);
                    for(size_t clicking = 0; clicking < clicks.size(); ++clicking)
                        clickstream.push(clicks[clicking]);
                    return true;
                }
            }
        }
        //clean up temporary "clicks" structure if its content wasn't used above
        for(size_t i = 0; i< clicks.size(); ++i)
            SAFE_DELETE(clicks[i]);
        clicks.clear();
        SAFE_DELETE(paid);
        return false;
    }

    // Didn't have a payment in mind if we reach this point
    //pMana is our main payment form, it is far faster then direct search.
    DebugTrace("AIPlayerBaka: the Mana was already in the manapool or could be Paid with potential mana, using potential Mana now.");

    ManaCost * pMana = getPotentialMana(target);
    if (!pMana)
        return false;

    pMana->add(this->getManaPool());

    if(!cost->getConvertedCost() && cost->hasX())
    {
        cost = pMana;//{x}:effect, set x to max.
    }

    if(!pMana->canAfford(cost, 0))
    {
        delete pMana;
        return false;
    }
    map<MTGCardInstance *, bool> used;
    //Spare would-be ATTACKERS from the mana bill - PARTIALLY. This loop taps every
    //usable producer except the surplus (diff); a creature that also makes mana (a
    //sliver under Gemhide, a mana dork) tapped to cast a spell while lands sit idle
    //leaves no untapped attacker at COMBATATTACKERS - so the engine correctly never
    //offers the declare-attackers step (live-observed, corpus 20260719 deck35 vs49).
    //Build the SWING-NEUTRAL pool (lands, rocks, sick/tapped creatures + the mana
    //pool), then draw in would-be attackers ONE AT A TIME, weakest first, only
    //while the bill is still short; base the surplus (diff) on that set and pre-mark
    //every attacker the bill did not reach as used, so the loop taps only what it
    //needs and holds the strongest swingers back. The prior form was all-or-nothing
    //(spare EVERY attacker iff neutrals covered the WHOLE cost, else spare NONE), so
    //a cost a single mana over the neutral supply emptied the board of attackers.
    //An {X} spell is EXCLUDED: there the AI wants to spend everything to maximize X,
    //so holding a mana-attacker back would shrink X (Death Wind {X}{B} over three
    //Leaden Myr must tap all three to reach X=2, not spare one and settle for X=0).
    bool spareAttackers = false;
    size_t atkNeed = 0;
    vector<AManaProducer*> attackerProd; //deferred: only tapped if the bill needs it
    ManaCost * diff = NULL;
    if (!cost->hasX())
    {
        ManaCost * neutral = NEW ManaCost();
        neutral->add(this->getManaPool());
        {
            map<MTGCardInstance*, bool> counted;
            for (size_t z = 1; z < observer->mLayers->actionLayer()->mObjects.size(); ++z)
            {
                AManaProducer * zamp = dynamic_cast<AManaProducer*>((MTGAbility *) observer->mLayers->actionLayer()->mObjects[z]);
                if (!zamp || !canHandleCost(zamp))
                    continue;
                MTGCardInstance * zsrc = zamp->source;
                if (!zsrc || zsrc == target || counted[zsrc] || !zamp->isReactingToClick(zsrc) || zamp->output->getConvertedCost() < 1)
                    continue;
                counted[zsrc] = true;
                if (zsrc->isCreature() && zsrc->canAttack())
                    attackerProd.push_back(zamp);
                else
                    neutral->add(zamp->output);
            }
        }
        std::sort(attackerProd.begin(), attackerProd.end(),
            [](AManaProducer * a, AManaProducer * b) { return a->source->power < b->source->power; });
        while (!neutral->canAfford(cost, 0) && atkNeed < attackerProd.size())
        {
            neutral->add(attackerProd[atkNeed]->output);
            ++atkNeed;
        }
        spareAttackers = neutral->canAfford(cost, 0);
        diff = spareAttackers ? neutral->Diff(cost) : pMana->Diff(cost);
        SAFE_DELETE(neutral);
    }
    else
    {
        diff = pMana->Diff(cost);
    }
    delete (pMana);
    if (spareAttackers)
    {
        for (size_t k = atkNeed; k < attackerProd.size(); k++)
            used[attackerProd[k]->source] = true;
    }
    for (size_t i = 1; i < observer->mLayers->actionLayer()->mObjects.size(); ++i)
    { //0 is not a mtgability...hackish
        //Make sure we can use the ability
        MTGAbility * a = ((MTGAbility *) observer->mLayers->actionLayer()->mObjects[i]);
        AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
        if (amp && canHandleCost(amp))
        {
            MTGCardInstance * card = amp->source;
            if (card == target)
            {
                used[card] = true;
            } //http://code.google.com/p/wagic/issues/detail?id=76
            if (!used[card] && amp->isReactingToClick(card) && amp->output->getConvertedCost() >= 1)
            {
                used[card] = true;
                int doUse = 1;
                for (int i = Constants::NB_Colors - 1; i >= 0; i--)
                {
                    if (diff->getCost(i) && amp->output->getCost(i))
                    {
                        diff->remove(i, 1);
                        doUse = 0;
                        break;
                    }
                }
                if (doUse)
                {
                    AIAction * action = NEW AIAction(this, amp, card);
                    clickstream.push(action);
                }
            }
        }
    }
    delete (diff);
    return true;
}

//Mana affordability/payment planning now lives in the player-agnostic
//ManaEngine; Baka supplies its willingness check (canHandleCost, which may
//pre-choose extra-cost payments) as the policy.
namespace
{
    class BakaManaPolicy : public ManaEngine::ManaPolicy
    {
    public:
        BakaManaPolicy(AIPlayerBaka * _ai) : ai(_ai) {}
        int canHandle(MTGAbility * producer) { return ai->canHandleCost(producer); }
    private:
        AIPlayerBaka * ai;
    };
}

ManaCost * AIPlayerBaka::getPotentialMana(MTGCardInstance * target)
{
    BakaManaPolicy policy(this);
    return ManaEngine::potentialMana(this, policy, target);
}

vector<MTGAbility*> AIPlayerBaka::canPayMana(MTGCardInstance * target, ManaCost * cost, int anytypeofmana)
{
    BakaManaPolicy policy(this);
    return ManaEngine::planPayment(this, policy, target, cost, anytypeofmana);
}

vector<MTGAbility*> AIPlayerBaka::canPayMana(MTGCardInstance * target, ManaCost * _cost, int anytypeofmana, map<MTGCardInstance*,bool> &used ,bool searchingAgain)
{
    BakaManaPolicy policy(this);
    return ManaEngine::planPayment(this, policy, target, _cost, anytypeofmana, used, searchingAgain);
}

//N-116g: the specific-mana plan for ONE (ability, source) pair, by the same
//rule the ability scans use - only probe for a specific plan when the pooled
//potential mana cannot already cover the cost. The scans compute this into a
//shared variable that is never cleared between pairs, so the plan that reached
//payment belonged to an arbitrary other ability; callers ask for the plan of
//the action they are about to take instead.
vector<MTGAbility*> AIPlayerBaka::planPaymentForAction(MTGAbility * ability, MTGCardInstance * click,
                                                       ManaCost * totalPotentialMana)
{
    vector<MTGAbility*> plan;
    if (!ability || !click || !ability->getCost())
        return plan;
    int anyType = click->has(Constants::ANYTYPEOFMANAABILITY);
    if (!ability->isReactingToClick(click, totalPotentialMana))
        plan = canPayMana(click, ability->getCost(), anyType);
    return plan;
}

vector<MTGAbility*> AIPlayerBaka::canPaySunBurst(ManaCost * cost)
{
    //in canPaySunburst we try to fill the cost with one of each color we can produce, 
    //note it is still possible to use lotus petal for it's first mana ability and not later for a final color
    //a search of true sunburst would cause the game to come to a crawl, trust me, this is the "fast" method for sunburst :)
    ManaCost * result = NEW ManaCost();
    map<MTGCardInstance *, bool> used;
    vector<MTGAbility*>payments = vector<MTGAbility*>();
    int needColorConverted = 6;
    int fullColor = 0;
    result->add(this->getManaPool());
    for (size_t i = 0; i < observer->mLayers->actionLayer()->manaObjects.size(); i++)
    { 
        //Make sure we can use the ability
        if(fullColor == needColorConverted || fullColor == cost->getConvertedCost())
        {
            break;
        }
        MTGAbility * a = ((MTGAbility *) observer->mLayers->actionLayer()->manaObjects[i]);
        AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
        if(amp && amp->getCost() && amp->getCost()->extraCosts && !amp->getCost()->extraCosts->canPay())
            continue;//pentid prism, has no cost but contains a counter cost, without this check ai will think it can still use this mana.
        if (amp && canHandleCost(amp) && amp->isReactingToClick(amp->source,amp->getCost()))
        {
            for (int k = Constants::NB_Colors-1; k > 0 ; k--)
            {
                if (amp->output->hasColor(k) && result->getCost(k) < 1 && result->getConvertedCost() < cost->getConvertedCost())
                {
                    MTGCardInstance * card = amp->source;
                    if (!used[card] && amp->isReactingToClick(card) && amp->output->getConvertedCost() >= 1)
                    {
                        ManaCost * check = NEW ManaCost();
                        check->add(k,1);
                        ManaCost * checkResult = NEW ManaCost();
                        checkResult->add(k,result->getCost(k));
                        if(!(checkResult->canAfford(check,card->has(Constants::ANYTYPEOFMANA))))
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

    for(int i = fullColor;i < cost->getConvertedCost();i++)
    {
        for (size_t i = 0; i < observer->mLayers->actionLayer()->manaObjects.size(); i++)
        { 
            MTGAbility * a = ((MTGAbility *) observer->mLayers->actionLayer()->manaObjects[i]);
            AManaProducer * amp = dynamic_cast<AManaProducer*> (a);
            if (amp && canHandleCost(amp))
            {
                MTGCardInstance * card = amp->source;
                if (!used[card] && amp->isReactingToClick(card) && amp->output->getConvertedCost() >= 1)
                {
                    if(!(result->canAfford(cost,card->has(Constants::ANYTYPEOFMANA))))//if we got to this point we should be filling colorless mana requirements.
                    {
                        payments.push_back(amp);
                        result->add(amp->output);
                        used[card] = true;
                    }
                }
            }
        }
    }
    if(!result->canAfford(cost,0))
        payments.clear();
    SAFE_DELETE(result);
    return payments;
}


//can handle extra cost to some extent, tho not wisely.
int AIPlayerBaka::CanHandleCost(ManaCost * cost, MTGCardInstance * card)
{
    if (!cost)
        return 1;

    ExtraCosts * ec = cost->extraCosts;
    if (!ec)
        return 1;

    for (size_t i = 0; i < ec->costs.size(); ++i)
    {
        if (ec->costs[i]->tc)
        {
            ec->costs[i]->setSource(card);
            if(!ec->costs[i]->tc->countValidTargets())
                return 0;
            if(!chooseCard(ec->costs[i]->tc,card))
                return 0;
        }
    }
    return 1;
}

int AIPlayerBaka::canHandleCost(MTGAbility * ability)
{
    return CanHandleCost(ability->getCost(),ability->source);
}




int AIPlayerBaka::createAbilityTargets(MTGAbility * a, MTGCardInstance * c, RankingContainer& ranking)
{
    if (!a->getActionTc())
    {
        OrderedAIAction aiAction(this, a, c, NULL);
        ranking[aiAction] = 1;
        return 1;
    }
    if (comboHint && comboHint->cardTargets.size())
    {
        a->setActionTC(GetComboTc(observer,a->getActionTc()));
    }
    vector<Targetable*>potentialTargets;
    for (int i = 0; i < 2; i++)
    {
        Player * p = observer->players[i];
        MTGGameZone * playerZones[] = { p->game->graveyard, p->game->library, p->game->hand, p->game->inPlay, p->game->stack, p->game->exile, p->game->commandzone, p->game->sideboard, p->game->reveal };
        if(a->getActionTc()->canTarget((Targetable*)p))
        {
            if(a->getActionTc()->maxtargets == 1)
            {
                OrderedAIAction aiAction(this, a, p, c);
                ranking[aiAction] = 1;
            }
            else
                potentialTargets.push_back(p);
        }
        for (int j = 0; j < 9; j++)
        {
            MTGGameZone * zone = playerZones[j];
            for (int k = 0; k < zone->nb_cards; k++)
            {
                MTGCardInstance * t = zone->cards[k];
                if (a->getActionTc()->canTarget(t))
                {
                    if(a->getActionTc()->maxtargets == 1)
                    {
                        OrderedAIAction aiAction(this, a, c, t);
                        ranking[aiAction] = 1;
                    }
                    else
                    {
                        potentialTargets.push_back(t);
                    }
                }
            }
        }
    }
    vector<Targetable*>realTargets;
    if(a->getActionTc()->maxtargets != 1)
    {
        if(a->getActionTc()->getNbTargets() && a->getActionTc()->attemptsToFill > 4)
        {
            a->getActionTc()->done = true;
            a->getActionTc()->autoChoice = false;
            return 0;
        }
        while(potentialTargets.size())
        {
            OrderedAIAction * check = NULL;

            MTGCardInstance * cTargeting = dynamic_cast<MTGCardInstance*>(potentialTargets[0]);
            if(cTargeting)
                check = NEW OrderedAIAction(this, a,c,cTargeting);

            Player * pTargeting = dynamic_cast<Player*>(potentialTargets[0]);
            if(pTargeting)
                check = NEW OrderedAIAction(this, a,pTargeting,c);

            int targetThis = getEfficiency(check);
            if(targetThis && pTargeting)
            {
                OrderedAIAction aiAction(this, a,pTargeting,c);
                ranking[aiAction] = 1;
            }
            if(targetThis)
                realTargets.push_back(potentialTargets[0]);
            potentialTargets.erase(potentialTargets.begin());
            SAFE_DELETE(check);
        }
        if(!realTargets.size() || (int(realTargets.size()) < a->getActionTc()->maxtargets && a->getActionTc()->targetMin))
            return 0;
        OrderedAIAction aiAction(this, a, c,realTargets);

        aiAction.target = dynamic_cast<MTGCardInstance*>(realTargets[0]);
        aiAction.playerAbilityTarget = dynamic_cast<Player*>(realTargets[0]);
        ranking[aiAction] = 1;
    }
    return 1;
}

TargetChooser * AIPlayerBaka::GetComboTc( GameObserver * observer,TargetChooser * tc)
{
    TargetChooserFactory tcf(observer);
    for(map<string, string>::iterator it = comboHint->cardTargets.begin();it != comboHint->cardTargets.end();++it)
    {
        TargetChooser *gathertc = tcf.createTargetChooser(it->first.c_str(),tc->source);
        gathertc->setAllZones();
        if(gathertc->canTarget(tc->source))
        {
            MTGCardInstance * cardBackUp = tc->source;
            Player * Oowner = tc->Owner;
            TargetChooser * testTc = tcf.createTargetChooser(it->second.c_str(),cardBackUp);
            if(testTc->countValidTargets())
            {
                tc = testTc;
                tc->Owner = Oowner;
                tc->other = true;
            }
            //I know I shouldn't redefine a passed variable,
            //if anyone knows a way that doesn't add a major function for this that does this correctly
            //then feel free to change this redefine. I do it this way becuase the method is the
            //fastest I could find that doesn't produce a noticible lag on ai.
            //recreate the targetchooser for this card becuase we planned to use it in a combo
        }
        SAFE_DELETE(gathertc);
    }
    return tc;
}

int AIPlayerBaka::selectHintAbility()
{
    if (!hints)
        return 0;

    ManaCost * totalPotentialMana = getPotentialMana(); 
    totalPotentialMana->add(this->getManaPool());
    AIAction * action = hints->suggestAbility(totalPotentialMana);
    if (action && ((randomGenerator.random() % 100) < 95)) //95% chance
    {
        if (!clickstream.size())
        {
            DebugTrace("AIPlayer:Using Activated ability");
            if (payTheManaCost(action->ability->getCost(), action->click->has(Constants::ANYTYPEOFMANAABILITY), action->click))
            {
                clickstream.push(action);
                SAFE_DELETE(totalPotentialMana);
                return 1;
            }
        }
    }
    SAFE_DELETE(action);
    SAFE_DELETE(totalPotentialMana);
    return 0;
}

int AIPlayerBaka::selectAbility()
{
    if(observer->mExtraPayment && observer->mExtraPayment->source && observer->mExtraPayment->source->controller() == this)
    {
        ExtraManaCost * check = NULL;
        check = dynamic_cast<ExtraManaCost*>(observer->mExtraPayment->costs[0]);
        if(check)
        {
            vector<MTGAbility*> CostToPay = canPayMana(observer->mExtraPayment->source,check->costToPay,check->source->has(Constants::ANYTYPEOFMANAABILITY));
            if(CostToPay.size())
            {
                payTheManaCost(check->costToPay,check->source->has(Constants::ANYTYPEOFMANAABILITY),check->source,CostToPay);
            }
            else
            {
                observer->mExtraPayment->action->CheckUserInput(JGE_BTN_SEC);
                observer->mExtraPayment = NULL;
            }
        }
    }
    // Try Deck hints first
    if (selectHintAbility())
        return 1;

    if(observer->mLayers->stackLayer()->lastActionController == this)
    {
        //this is here for 2 reasons, MTG rules state that priority is passed with each action.
        //without this ai is able to chain cast {t}:damage:1 target(creature) from everything it can all at once.
        //this not only is illegal but cause ai to waste abilities ei:all damage:1 on a single 1/1 creature.
        return 1;
    }

    RankingContainer ranking;
    list<int>::iterator it;
    vector<MTGAbility*>abilityPayment = vector<MTGAbility*>();
    //This loop is extrmely inefficient. TODO: optimize!
    ManaCost * totalPotentialMana = getPotentialMana();
    totalPotentialMana->add(this->getManaPool());
    for (size_t i = 1; i < observer->mLayers->actionLayer()->mObjects.size(); i++)
    { //0 is not a mtgability...hackish
        MTGAbility * a = ((MTGAbility *) observer->mLayers->actionLayer()->mObjects[i]);
        //Skip mana abilities for performance
        if (dynamic_cast<AManaProducer*> (a))
            continue;
        //Make sure we can use the ability with card in play
        for (int j = 0; j < game->inPlay->nb_cards; j++)
        {
            MTGCardInstance * card = game->inPlay->cards[j];
            if(a->getCost() && !a->isReactingToClick(card, totalPotentialMana))//for performance reason only look for specific mana if the payment couldnt be made with potential.
            {
                abilityPayment = canPayMana(card,a->getCost(),card->has(Constants::ANYTYPEOFMANAABILITY));
            }
            if (a->isReactingToClick(card, totalPotentialMana) || abilityPayment.size())
            { //This test is to avoid the huge call to getPotentialManaCost after that
                if(a->getCost() && a->getCost()->hasX() && totalPotentialMana->getConvertedCost() < a->getCost()->getConvertedCost()+1)
                    continue;
                //don't even bother to play an ability with {x} if you can't even afford x=1.
                if (abilityPayment.size())
                {
                    ManaCost *fullPayment = NEW ManaCost();
                    for(int ch = 0; ch < int(abilityPayment.size());ch++)
                    {
                        AManaProducer * ampp = dynamic_cast<AManaProducer*> (abilityPayment[ch]);
                        if(ampp)
                            fullPayment->add(ampp->output);
                    }
                    if (fullPayment && a->isReactingToClick(card, fullPayment))
                        createAbilityTargets(a, card, ranking);
                    delete fullPayment;
                }
                else
                {
                    ManaCost * pMana = getPotentialMana(card);
                    pMana->add(this->getManaPool());
                    if (a->isReactingToClick(card, pMana))
                    {
                        createAbilityTargets(a, card, ranking);
                    }
                    delete (pMana);
                }     
            }
        }
        //Make sure we can use the ability with card in commandzone
        for (int j = 0; j < game->commandzone->nb_cards; j++)
        {
            MTGCardInstance * card = game->commandzone->cards[j];
            if(a->getCost() && !a->isReactingToClick(card, totalPotentialMana))//for performance reason only look for specific mana if the payment couldnt be made with potential.
            {
                abilityPayment = canPayMana(card,a->getCost(),card->has(Constants::ANYTYPEOFMANAABILITY));
            }
            if (a->isReactingToClick(card, totalPotentialMana) || abilityPayment.size())
            { //This test is to avoid the huge call to getPotentialManaCost after that
                if(a->getCost() && a->getCost()->hasX() && totalPotentialMana->getConvertedCost() < a->getCost()->getConvertedCost()+1)
                    continue;
                //don't even bother to play an ability with {x} if you can't even afford x=1.
                if (abilityPayment.size())
                {
                    ManaCost *fullPayment = NEW ManaCost();
                    for(int ch = 0; ch < int(abilityPayment.size());ch++)
                    {
                        AManaProducer * ampp = dynamic_cast<AManaProducer*> (abilityPayment[ch]);
                        if(ampp)
                            fullPayment->add(ampp->output);
                    }
                    if (fullPayment && a->isReactingToClick(card, fullPayment))
                        createAbilityTargets(a, card, ranking);
                    delete fullPayment;
                }
                else
                {
                    ManaCost * pMana = getPotentialMana(card);
                    pMana->add(this->getManaPool());
                    if (a->isReactingToClick(card, pMana))
                    {
                        createAbilityTargets(a, card, ranking);
                    }
                    delete (pMana);
                }     
            }
        }
        //Make sure we can use the ability with card in hand
        for (int j = 0; j < game->hand->nb_cards; j++)
        {
            MTGCardInstance * card = game->hand->cards[j];
            if(a->getCost() && !a->isReactingToClick(card, totalPotentialMana))//for performance reason only look for specific mana if the payment couldnt be made with potential.
            {
                abilityPayment = canPayMana(card,a->getCost(),card->has(Constants::ANYTYPEOFMANAABILITY));
            }
            if (a->isReactingToClick(card, totalPotentialMana) || abilityPayment.size())
            { //This test is to avoid the huge call to getPotentialManaCost after that
                if(a->getCost() && a->getCost()->hasX() && totalPotentialMana->getConvertedCost() < a->getCost()->getConvertedCost()+1)
                    continue;
                //don't even bother to play an ability with {x} if you can't even afford x=1.
                if (abilityPayment.size())
                {
                    ManaCost *fullPayment = NEW ManaCost();
                    for(int ch = 0; ch < int(abilityPayment.size());ch++)
                    {
                        AManaProducer * ampp = dynamic_cast<AManaProducer*> (abilityPayment[ch]);
                        if(ampp)
                            fullPayment->add(ampp->output);
                    }
                    if (fullPayment && a->isReactingToClick(card, fullPayment))
                        createAbilityTargets(a, card, ranking);
                    delete fullPayment;
                }
                else
                {
                    ManaCost * pMana = getPotentialMana(card);
                    pMana->add(this->getManaPool());
                    if (a->isReactingToClick(card, pMana))
                    {
                        createAbilityTargets(a, card, ranking);
                    }
                    delete (pMana);
                }     
            }
        }
        //Make sure we can use the ability with card in graveyard
        for (int j = 0; j < game->graveyard->nb_cards; j++)
        {
            MTGCardInstance * card = game->graveyard->cards[j];
            if(a->getCost() && !a->isReactingToClick(card, totalPotentialMana))//for performance reason only look for specific mana if the payment couldnt be made with potential.
            {
                abilityPayment = canPayMana(card,a->getCost(),card->has(Constants::ANYTYPEOFMANAABILITY));
            }
            if (a->isReactingToClick(card, totalPotentialMana) || abilityPayment.size())
            { //This test is to avoid the huge call to getPotentialManaCost after that
                if(a->getCost() && a->getCost()->hasX() && totalPotentialMana->getConvertedCost() < a->getCost()->getConvertedCost()+1)
                    continue;
                //don't even bother to play an ability with {x} if you can't even afford x=1.
                if (abilityPayment.size())
                {
                    ManaCost *fullPayment = NEW ManaCost();
                    for(int ch = 0; ch < int(abilityPayment.size());ch++)
                    {
                        AManaProducer * ampp = dynamic_cast<AManaProducer*> (abilityPayment[ch]);
                        if(ampp)
                            fullPayment->add(ampp->output);
                    }
                    if (fullPayment && a->isReactingToClick(card, fullPayment))
                        createAbilityTargets(a, card, ranking);
                    delete fullPayment;
                }
                else
                {
                    ManaCost * pMana = getPotentialMana(card);
                    pMana->add(this->getManaPool());
                    if (a->isReactingToClick(card, pMana))
                    {
                        createAbilityTargets(a, card, ranking);
                    }
                    delete (pMana);
                }     
            }
        }
        //Make sure we can use the ability with card in exile
        for (int j = 0; j < game->exile->nb_cards; j++)
        {
            MTGCardInstance * card = game->exile->cards[j];
            if(a->getCost() && !a->isReactingToClick(card, totalPotentialMana))//for performance reason only look for specific mana if the payment couldnt be made with potential.
            {
                abilityPayment = canPayMana(card,a->getCost(),card->has(Constants::ANYTYPEOFMANAABILITY));
            }
            if (a->isReactingToClick(card, totalPotentialMana) || abilityPayment.size())
            { //This test is to avoid the huge call to getPotentialManaCost after that
                if(a->getCost() && a->getCost()->hasX() && totalPotentialMana->getConvertedCost() < a->getCost()->getConvertedCost()+1)
                    continue;
                //don't even bother to play an ability with {x} if you can't even afford x=1.
                if (abilityPayment.size())
                {
                    ManaCost *fullPayment = NEW ManaCost();
                    for(int ch = 0; ch < int(abilityPayment.size());ch++)
                    {
                        AManaProducer * ampp = dynamic_cast<AManaProducer*> (abilityPayment[ch]);
                        if(ampp)
                            fullPayment->add(ampp->output);
                    }
                    if (fullPayment && a->isReactingToClick(card, fullPayment))
                        createAbilityTargets(a, card, ranking);
                    delete fullPayment;
                }
                else
                {
                    ManaCost * pMana = getPotentialMana(card);
                    pMana->add(this->getManaPool());
                    if (a->isReactingToClick(card, pMana))
                    {
                        createAbilityTargets(a, card, ranking);
                    }
                    delete (pMana);
                }     
            }
        }
    }
    const OrderedAIAction * chosenAction = chooseOrderedAction(ranking);
    if (chosenAction)
    {
        const OrderedAIAction & action = *chosenAction;
        if (!clickstream.size())
        {
            //N-116g: the payment plan must belong to the action being taken.
            //abilityPayment above is assigned inside the scan for WHATEVER
            //(ability, card) pair last failed the potential-mana probe - which
            //is nearly every pair, since the probe runs for every ability
            //against every card and only the ability's own source can react -
            //and it is never cleared between pairs. So the plan handed to
            //payTheManaCost was, in general, a plan for a DIFFERENT ability's
            //cost. When that foreign plan cannot afford the chosen cost,
            //payTheManaCost prints "Ai had a payment in mind" and returns
            //false: no AIAction is queued, nothing is tapped, nothing is paid,
            //and the whole activation disappears with no fallback, no defer
            //and no counter recording it (wave-33 N-116g: 3 of 11 cheat-into-
            //play activations silently no-op'd, one of them the game). The
            //complementary failure - an empty leftover plan forcing the strict
            //one-ability-per-card potential-mana path, which cannot see a dual
            //land's second colour - dropped the {g},Tap Elvish Piper
            //activation off a Tropical Island the same corpus.
            //Recompute here, for THIS action, by the same rule the scan uses.
            vector<MTGAbility*> chosenPayment = planPaymentForAction(action.ability, action.click,
                                                                    totalPotentialMana);
            if (chosenPayment.size())
            {
                DebugTrace(" Ai knows exactly what mana to use for this ability.");
            }
            DebugTrace("AIPlayer:Using Activated ability");
            if (payTheManaCost(action.ability->getCost(),action.click->has(Constants::ANYTYPEOFMANAABILITY),action.click,chosenPayment))
                clickstream.push(NEW AIAction(action));
        }
    }
    delete totalPotentialMana;

    abilityPayment.clear();
    return 1;
}

const OrderedAIAction * AIPlayerBaka::chooseOrderedAction(RankingContainer& ranking)
{
    if (!ranking.size())
        return NULL;
    OrderedAIAction action = ranking.begin()->first; //copy: getEfficiency() is not const
    int chance = 1;
    if (!forceBestAbilityUse)
        chance = 1 + randomGenerator.random() % 100;
    int actionScore = action.getEfficiency();
    if(action.ability->getCost() && action.ability->getCost()->hasX() && this->game->hand->cards.size())
        actionScore = actionScore/int(this->game->hand->cards.size());//reduce chance for "x" abilities if cards are in hand.
    if (actionScore < chance)
        return NULL;
    return &(ranking.begin()->first);
}

int AIPlayerBaka::doAbility(MTGAbility * Specific, MTGCardInstance * withCard)
{
    if (observer->mExtraPayment && observer->mExtraPayment->source && observer->mExtraPayment->source->controller() == this)
    {
        ExtraManaCost * check = NULL;
        check = dynamic_cast<ExtraManaCost*>(observer->mExtraPayment->costs[0]);
        if (check)
        {
            vector<MTGAbility*> CostToPay = canPayMana(observer->mExtraPayment->source, check->costToPay, check->source->has(Constants::ANYTYPEOFMANAABILITY));
            if (CostToPay.size())
            {
                payTheManaCost(check->costToPay, check->source->has(Constants::ANYTYPEOFMANAABILITY), check->source, CostToPay);
            }
            else
            {
                observer->mExtraPayment->action->CheckUserInput(JGE_BTN_SEC);
                observer->mExtraPayment = NULL;
            }
        }
    }
    if (observer->mLayers->stackLayer()->lastActionController == this)
    {
        return 1;
    }

    RankingContainer ranking;
    list<int>::iterator it;
    vector<MTGAbility*>abilityPayment = vector<MTGAbility*>();
    MTGCardInstance * card = withCard;
    ManaCost * totalPotentialMana = getPotentialMana();
    totalPotentialMana->add(this->getManaPool());
    for (size_t i = 1; i < observer->mLayers->actionLayer()->mObjects.size(); i++)
    {
        MTGAbility * a = ((MTGAbility *)observer->mLayers->actionLayer()->mObjects[i]);
        if (Specific && Specific != a)
            continue;
        //Make sure we can use the ability
        if (a->getCost() && !a->isReactingToClick(card, totalPotentialMana))//for performance reason only look for specific mana if the payment couldnt be made with potential.
        {
            abilityPayment = canPayMana(card, a->getCost(), card->has(Constants::ANYTYPEOFMANAABILITY));
        }
        if (a->isReactingToClick(card, totalPotentialMana) || abilityPayment.size())
        { //This test is to avoid the huge call to getPotentialManaCost after that
            if (a->getCost() && a->getCost()->hasX() && totalPotentialMana->getConvertedCost() < a->getCost()->getConvertedCost() + 1)
                continue;
            //don't even bother to play an ability with {x} if you can't even afford x=1.
            if (abilityPayment.size())
            {
                ManaCost *fullPayment = NEW ManaCost();
                for (int ch = 0; ch < int(abilityPayment.size()); ch++)
                {
                    AManaProducer * ampp = dynamic_cast<AManaProducer*> (abilityPayment[ch]);
                    if (ampp)
                        fullPayment->add(ampp->output);
                }
                if (fullPayment && a->isReactingToClick(card, fullPayment))
                    createAbilityTargets(a, card, ranking);
                delete fullPayment;
            }
            else
            {
                ManaCost * pMana = getPotentialMana(card);
                pMana->add(this->getManaPool());
                if (a->isReactingToClick(card, pMana))
                {
                    createAbilityTargets(a, card, ranking);

                    if (!Specific->getCost())
                    {
                        //attackcost, blockcost
                        if (a->aType == MTGAbility::ATTACK_COST)
                        {
                            ManaCost * specificCost = NEW ManaCost(ManaCost::parseManaCost("{0}", NULL, NULL));
                            specificCost->add(0, card->attackCostBackup);
                            abilityPayment = canPayMana(card, specificCost, card->has(Constants::ANYTYPEOFMANAABILITY));
                            SAFE_DELETE(specificCost);
                        }
                        else if (a->aType == MTGAbility::BLOCK_COST)
                        {
                            ManaCost * specificCost = NEW ManaCost(ManaCost::parseManaCost("{0}", NULL, NULL));
                            specificCost->add(0, card->blockCostBackup);
                            abilityPayment = canPayMana(card, specificCost, card->has(Constants::ANYTYPEOFMANAABILITY));
                            SAFE_DELETE(specificCost);
                        }
                    }
                }
                delete (pMana);
            }
        }
    }
    if (ranking.size())
    {
        OrderedAIAction action = ranking.begin()->first;
        int chance = 1;
        if (!forceBestAbilityUse)
            chance = 1 + randomGenerator.random() % 100;
        int actionScore = 95;
        if (action.ability->getCost() && action.ability->getCost()->hasX() && this->game->hand->cards.size())
            actionScore = actionScore / int(this->game->hand->cards.size());//reduce chance for "x" abilities if cards are in hand.
        if (actionScore >= chance)
        {
            if (!clickstream.size())
            {
                //N-116g, same leak as in selectAbility(): the scan's
                //abilityPayment belongs to whichever pair last failed the
                //potential-mana probe, not to the action being taken. The
                //attack/block-cost branches below carry a plan the scan built
                //for THIS card's specific cost, so they keep it; the ordinary
                //cost path recomputes for the chosen action.
                vector<MTGAbility*> chosenPayment = planPaymentForAction(action.ability, action.click,
                                                                        totalPotentialMana);
                if (abilityPayment.size() || chosenPayment.size())
                {
                    DebugTrace(" Ai knows exactly what mana to use for this ability.");
                }
                DebugTrace("AIPlayer:Using Activated ability");

                if (!Specific->getCost())
                {
                    //attackcost, blockcost
                    if (action.ability->aType == MTGAbility::ATTACK_COST)
                    {
                        ManaCost * specificCost = NEW ManaCost(ManaCost::parseManaCost("{0}", NULL, NULL));
                        specificCost->add(0, action.click->attackCostBackup);
                        if (payTheManaCost(specificCost, action.click->has(Constants::ANYTYPEOFMANAABILITY), action.click, abilityPayment))
                            clickstream.push(NEW AIAction(action));
                        SAFE_DELETE(specificCost);
                    }
                    else if (action.ability->aType == MTGAbility::BLOCK_COST)
                    {
                        ManaCost * specificCost = NEW ManaCost(ManaCost::parseManaCost("{0}", NULL, NULL));
                        specificCost->add(0, action.click->blockCostBackup);
                        if (payTheManaCost(specificCost, action.click->has(Constants::ANYTYPEOFMANAABILITY), action.click, abilityPayment))
                            clickstream.push(NEW AIAction(action));
                        SAFE_DELETE(specificCost);
                    }
                }
                else
                {
                    if (payTheManaCost(action.ability->getCost(), action.click->has(Constants::ANYTYPEOFMANAABILITY), action.click, chosenPayment))
                        clickstream.push(NEW AIAction(action));
                }
            }
        }
    }
    delete totalPotentialMana;
    abilityPayment.clear();
    return 1;
}

int AIPlayerBaka::interruptIfICan()
{
    if (observer->mLayers->stackLayer()->askIfWishesToInterrupt == this)
    {
        if (!clickstream.empty())
            observer->mLayers->stackLayer()->cancelInterruptOffer();
        else
            return observer->mLayers->stackLayer()->setIsInterrupting(this);
    }
    return 0;
}

int AIPlayerBaka::effectBadOrGood(MTGCardInstance * card, int mode, TargetChooser * tc)
{
    if(hints && hints->HintSaysCardIsGood(observer,card))
    {
        return BAKA_EFFECT_GOOD;
    }
    if(hints && hints->HintSaysCardIsBad(observer,card))
    {
        return BAKA_EFFECT_BAD;
    }
    int id = card->getMTGId();
    AbilityFactory af(observer);
    int autoGuess = af.magicText(id, NULL, card, mode, tc);
    if (autoGuess)
        return autoGuess;
    return BAKA_EFFECT_DONTKNOW;
}

int AIPlayerBaka::chooseTarget(TargetChooser * _tc, Player * forceTarget,MTGCardInstance * chosenCard,bool checkOnly)
{
    if(observer->mExtraPayment)
    {
        observer->mExtraPayment->action->CheckUserInput(JGE_BTN_SEC);
        observer->mExtraPayment = NULL;
    }
    //there should never be a case where a extra cost target selection is happening at the same time as this..
    //extracost uses "chooseCard()" to determine its targets.
    vector<Targetable *> potentialTargets;
    TargetChooser * tc = _tc;
    if (!(observer->currentlyActing() == this))
        return 0;
    if (!tc)
    {
        tc = observer->getCurrentTargetChooser();
    }
    if (!tc || !tc->source || tc->maxtargets < 1)
        return 0;
    assert(tc);
    if (comboHint && comboHint->cardTargets.size())
    {
        tc = GetComboTc(observer,tc);
    }
    if(!checkOnly && tc->maxtargets > 1)
    {
        tc->initTargets();//just incase....
        potentialTargets.clear();
    }
    //Make sure we own the decision to choose the targets
    assert(tc->Owner == observer->currentlyActing());
    if (tc->Owner != observer->currentlyActing())
    {
        observer->currentActionPlayer = tc->Owner;
        //this is a hack, but if we hit this condition we are locked in a infinate loop
        //so lets give the tc to its owner
        //todo:find the root cause of this.
        DebugTrace("AIPLAYER: Error, was asked to chose targets but I don't own the source of the targetController\n");
        return 0;
    }
    Player * target = forceTarget;
    int playerTargetedZone = 1;
    if (!target)
    {
        target = this;
        int cardEffect = effectBadOrGood(tc->source, MODE_TARGET, tc);
        if(tc->belongsToAbility.size())
        {
            AbilityFactory af(observer);
            MTGAbility * withoutGuessing = af.parseMagicLine(tc->belongsToAbility,0,NULL,tc->source);
            cardEffect = af.abilityEfficiency(withoutGuessing,this,MODE_TARGET,tc,NULL);
            delete withoutGuessing;
        }
        // Don't really like it but green mana producing auras targeting the player is one of the most reported bugs
        if(cardEffect == BAKA_EFFECT_DONTKNOW && tc->source->hasSubtype(Subtypes::TYPE_AURA) && tc->source->hasColor(Constants::MTG_COLOR_GREEN))
            cardEffect = BAKA_EFFECT_GOOD;

        if (cardEffect != BAKA_EFFECT_GOOD)
        {
            target = this->opponent();
        }
        if(dynamic_cast<ProliferateChooser*> (tc))
            playerTargetedZone = 2;
    }
    while(playerTargetedZone)
    {
        if (!tc->alreadyHasTarget(target) && tc->canTarget(target) && potentialTargets.size() < 50)
        {
            for (int i = 0; i < 3; i++)
            { //Increase probability to target a player when this is possible
                potentialTargets.push_back(target);
            }
        }
        MTGPlayerCards * playerZones = target->game;
        MTGGameZone * zones[] = { playerZones->hand, playerZones->library, playerZones->inPlay, playerZones->graveyard, playerZones->stack, playerZones->exile, playerZones->commandzone, playerZones->sideboard, playerZones->reveal };
        for (int j = 0; j < 9; j++)
        {
            MTGGameZone * zone = zones[j];
            for (int k = 0; k < zone->nb_cards; k++)
            {
                MTGCardInstance * card = zone->cards[k];
                if (!tc->alreadyHasTarget(card) && tc->canTarget(card) && potentialTargets.size() < 50)
                {
                    int multiplier = 1;
                    if (getStats() && getStats()->isInTop(card, 10))
                    {
                        multiplier++;
                        if (getStats()->isInTop(card, 5))
                        {
                            multiplier++;
                            if (getStats()->isInTop(card, 3))
                            {
                                multiplier++;
                            }
                        }
                    }
                    for (int l = 0; l < multiplier; l++)
                    {
                        if(tc->maxtargets != 1 && tc->belongsToAbility.size())
                        {
                            AbilityFactory af(observer);
                            MTGAbility * withoutGuessing = af.parseMagicLine(tc->belongsToAbility,0,NULL,tc->source);
                            OrderedAIAction * effCheck = NEW OrderedAIAction(this, withoutGuessing,(MTGCardInstance*)tc->source,card);
                            if(effCheck->getEfficiency())
                            {
                                potentialTargets.push_back(card);
                            }
                            SAFE_DELETE(effCheck);
                            SAFE_DELETE(withoutGuessing);
                        }
                        else
                        {
                            potentialTargets.push_back(card);
                        }
                    }
                }
            }
        }
        if(playerTargetedZone > 1)
            target = target->opponent();
        playerTargetedZone--;
    }
    if (potentialTargets.size())
    {
        if((!forceTarget && checkOnly)||(tc->maxtargets != 1))
        {
            sort(potentialTargets.begin(), potentialTargets.end());
            potentialTargets.erase(std::unique(potentialTargets.begin(), potentialTargets.end()), potentialTargets.end());
            //checking actual amount of unique targets.
            //multitargeting can not function the same as single target, 
            //a second click on the same target causes it to detoggle target, which can lead to ai lockdowns.
            if(!checkOnly)
                return clickMultiTarget(tc, potentialTargets);
            return int(potentialTargets.size());//return the actual amount of targets ai will atempt to select.
        }
        return clickSingleTarget(tc, potentialTargets, chosenCard);
        //click single target contains nbtargets to keep it as it was previously designed
        //shoving 100 targets into potential, then selecting one of them at random.
    }
    if(checkOnly)return 0;//it wasn't an error if we couldn't find a target while checkonly
    //Couldn't find any valid target for the preferred player,
    //usually that's because we played a card that has bad side effects (ex: when X comes into play, return target land you own to your hand)
    //so we try again to choose a target in the other player's field...
    //
    //OWNERSHIP (core 2623136): GameObserver::cancelCurrentAction() unconditionally
    //SAFE_DELETEs observer->targetChooser - the spell-CAST chooser. For a cast, `tc`
    //IS that chooser, so the old code (cancel here, then recurse with `tc`) was a
    //use-after-free: the GPT seat declined Fatal Push's mandatory target -> Baka
    //fallback -> empty preferred field -> the cancel freed `tc` -> the retry
    //dereferenced the freed chooser in TargetsList::alreadyHasTarget -> SIGSEGV.
    //Only the ability-layer waiting action is cleared here (no chooser teardown),
    //so `tc` stays live for the opposite-field retry below. The full cancel that
    //releases the casting chooser runs solely on the true give-up branch, once we
    //are done using `tc`.
    int cancel = observer->mLayers->actionLayer()->cancelCurrentAction();
    if (!cancel && !forceTarget)
        return chooseTarget(tc, target->opponent(), NULL, checkOnly);
    //ERROR!!! - already retried the opposite field (forceTarget set), or the
    //ability-layer cancel took. Abandon the action for good; this releases the
    //casting chooser (observer->targetChooser) too, and we no longer touch `tc`.
    observer->cancelCurrentAction();
    DebugTrace("AIPLAYER: ERROR! AI needs to choose a target but can't decide!!!");
    return 1;
}

//Returns -1 if error, a number between 0 and 100 otherwise
int AIPlayerBaka::getEfficiency(MTGAbility * ability)
{
    if (!ability)
        return -1;

    OrderedAIAction * check = NULL;

    if(MTGCardInstance * cTarget = dynamic_cast<MTGCardInstance *>(ability->target))
        check = NEW OrderedAIAction(this, ability, ability->source, cTarget);
    else if(Player * pTarget = dynamic_cast<Player *>(ability->target))
        check = NEW OrderedAIAction(this, ability, pTarget, ability->source);
    else
        check = NEW OrderedAIAction(this, ability, ability->source);

    if (!check)
        return -1;

    int result = getEfficiency(check);
    SAFE_DELETE(check);
    return result;
}

int AIPlayerBaka::selectMenuOption()
{
    ActionLayer * object = observer->mLayers->actionLayer();
    int doThis = 0; // The AI just passes on things if set to -1, getEfficiency should be improved
    if (object->menuObject)
    {
        int checkedLast = 0;
        if(object->abilitiesMenu->isMultipleChoice && object->currentActionCard)
        {
            MenuAbility * currentMenu = NULL;
            for(size_t m = object->mObjects.size()-1;m > 0;m--)
            {
                MenuAbility * ability = dynamic_cast<MenuAbility *>(object->mObjects[m]);
                if(ability && ability->triggered)
                {
                    currentMenu = (MenuAbility *)object->mObjects[m];
                    break;
                }
            }
            if(currentMenu)
                for (unsigned int mk = 0; mk < currentMenu->abilities.size(); mk++)
                {
                    if (dynamic_cast<AAWhatsX*>(currentMenu->abilities[0]))
                    {
                        if (currentMenu->announcing && currentMenu->announceCost)
                        {
                            //pay[[{X}]] announcement round: options enumerate the
                            //affordable range (index == X); announce the biggest X
                            //the pool + producers cover. The pool-based legacy
                            //formula below assumes the cast flow, where payment
                            //was already floated - here the pool is still empty.
                            int mx = ManaEngine::maxAnnounceableX(this, currentMenu->announceCost,
                                        currentMenu->source ? currentMenu->source->has(Constants::ANYTYPEOFMANAABILITY) : 0);
                            if (mx > int(currentMenu->abilities.size()) - 1)
                                mx = int(currentMenu->abilities.size()) - 1;
                            if (mx < 0)
                                mx = 0;
                            return mx;
                        }
                        //N-146h. The legacy formula below is pool-minus-printed-cost,
                        //which assumes the payment was already FLOATED. On the
                        //normal cast path (MTGPutInPlayRule::reactToClick) it is
                        //not: measured on Agadeem's Awakening {X}{B}{B}{B} over
                        //five Vault of Whispers, the announce menu was built with
                        //three options (X = 0,1,2) and this branch returned
                        //`0 - 3 = -3`. A NEGATIVE menu index is not an answer -
                        //nothing consumes it, card->setX stays -1, the cast never
                        //advances, and the floated mana is stranded. That is the
                        //deck146 signature exactly: mana spent, the face-chooser
                        //answer consumed, and the whole turn narrating one line,
                        //with the card still in hand and re-offered next window
                        //(corpus 20260727 seq30/31 t14, seq32/33 t16, seq34 t18;
                        //~21 mana burned with the opponent on 5 life). The
                        //ledger's hypothesis - that no ANNOUNCE_X step is reached
                        //and X defaults to 0 - is WRONG: the step IS reached, and
                        //X is never set at all.
                        //
                        //Price it the way the `announcing` branch above already
                        //does: ManaEngine::maxAnnounceableX counts the pool PLUS
                        //untapped producers, so it is correct whether or not the
                        //payment has been floated yet. Take the MAX so a path
                        //that answers correctly today cannot shrink, then CLAMP
                        //into the menu's real index range - an out-of-range index
                        //is silently dropped, and "it must not be silent" is this
                        //item's standing requirement.
                        int potent = manaPool->getConvertedCost();
                        int aftercost = potent - currentMenu->abilities[0]->source->getManaCost()->getConvertedCost();
                        MTGCardInstance * xsrc = currentMenu->abilities[0]->source;
                        int credited = ManaEngine::maxAnnounceableX(this, xsrc->getManaCost(),
                                            xsrc->has(Constants::ANYTYPEOFMANAABILITY));
                        if (credited > aftercost)
                            aftercost = credited;
                        if (aftercost > int(currentMenu->abilities.size()) - 1)
                            aftercost = int(currentMenu->abilities.size()) - 1;
                        if (aftercost < 0)
                            aftercost = 0;
                        return  aftercost;
                    }
                    int checked = getEfficiency(currentMenu->abilities[mk]);
                    if (checked > 60 && checked > checkedLast)
                    {
                        doThis = mk;
                        checkedLast = checked;
                    }
                }
        }
        else
        {
            for(unsigned int k = 0;k < object->abilitiesMenu->mObjects.size();k++)
            {
                if(object->abilitiesMenu->mObjects[k]->GetId() <= 0)
                    continue;

                MTGAbility * checkEff = (MTGAbility *)object->mObjects[object->abilitiesMenu->mObjects[k]->GetId()];
                int checked = getEfficiency(checkEff);
                if(checked > 60 && checked > checkedLast)
                {
                    doThis = k;
                    checkedLast = checked;
                }
            }
        }
    }
    return doThis;
}

//c5c: the four near-duplicate zone rungs (graveyard/exile/command/hand,
//~790 lines) collapsed into ONE evaluation loop over the
//LegalActionsOracle's enumeration. The oracle owns the RULES half: zone
//gates, the legendary rule, play restrictions, affordability across every
//cost variant (with specific-producer payment plans) and 601.2c target
//validity. This loop owns the POLICY half: combo hints, the residual
//per-card gates the oracle deliberately doesn't know, payment-variant
//choice (the gotPayments / payAlternative side-channel the commit stanza
//consumes), shouldPlayPercentage scoring, cast restrictions and the dice.
//
//type: "land" = land drops only; "" = the interrupt window (instant-speed
//casts only); "*" = anything including lands, most expensive (the hint
//wildcard, and AIPlayerGPT's aiForcedCandidate validation pass);
//"commander" = commander cards; a single type = that type; a
//comma-separated list = a whole priority ladder in one call (earlier
//types dominate, converted cost breaks ties within a type).
MTGCardInstance * AIPlayerBaka::FindCardToPlay(ManaCost * pMana, const char * type)
{
    int maxCost = -1;
    MTGCardInstance * nextCardToPlay = NULL;
    if(comboCards.size())
    {
        nextCardToPlay = comboCards.back();
        gotPayments.clear();
        if((!pMana->canAfford(nextCardToPlay->getManaCost(),0) || nextCardToPlay->getManaCost()->getKicker() || nextCardToPlay->getManaCost()->getBestow()))
            gotPayments = canPayMana(nextCardToPlay,nextCardToPlay->getManaCost(),nextCardToPlay->has(Constants::ANYTYPEOFMANA));
        DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
        comboCards.pop_back();
        if(!comboHint->cardTargets.size() && !comboCards.size())
            comboHint = NULL;//becuase it's no longer needed.
        return nextCardToPlay;
    }

    //parse the type request
    vector<string> typeRanks;          //priority order; empty = no type filter
    bool wantLands = false;
    bool wantSpells = true;
    bool commanderOnly = false;
    bool instantWindow = (*type == 0); //"": the interrupt-window search
    if (!strcmp(type, "land"))
    {
        wantLands = true;
        wantSpells = false;
    }
    else if (!strcmp(type, "*") || instantWindow)
    {
        wantLands = !instantWindow; //a window responds with spells, not land drops
    }
    else if (!strcmp(type, "commander"))
    {
        commanderOnly = true;
    }
    else
    {
        string t = type;
        size_t start = 0;
        while (start < t.size())
        {
            size_t comma = t.find(',', start);
            if (comma == string::npos)
                comma = t.size();
            if (comma > start)
                typeRanks.push_back(t.substr(start, comma - start));
            start = comma + 1;
        }
    }

    BakaManaPolicy policy(this);
    vector<LegalActionsOracle::Cast> candidates;
    if (wantSpells)
        candidates = LegalActionsOracle::legalCasts(this, policy, pMana, instantWindow);
    if (wantLands)
    {
        vector<LegalActionsOracle::Cast> lands = LegalActionsOracle::legalLandPlays(this);
        candidates.insert(candidates.end(), lands.begin(), lands.end());
    }

    payAlternative = NONE;
    gotPayments = vector<MTGAbility*>();
    int bestRank = INT_MAX; //lower = higher priority type

    for (size_t ci = 0; ci < candidates.size(); ci++)
    {
        MTGCardInstance * card = candidates[ci].card;
        bool viaAlt = candidates[ci].viaAlternative;

        if (aiForcedCandidate && card != aiForcedCandidate)
            continue;
        if (commanderOnly && !card->isCommander)
            continue;

        int rank = 0;
        if (typeRanks.size())
        {
            rank = -1;
            for (size_t r = 0; r < typeRanks.size(); r++)
            {
                if (card->hasType(typeRanks[r]))
                {
                    rank = (int) r;
                    break;
                }
            }
            if (rank < 0)
                continue; //not one of the requested types
        }

        ManaCost * baseCost = card->getManaCost();
        //A card castable both normally and via its alternative cost is two
        //oracle entries. Baka's historical policy pays the normal cost when
        //it can; the alternative entry only competes when the normal cost
        //is unpayable, or when the consumer (AIPlayerGPT) explicitly forced
        //the alternative mode for this candidate.
        bool forceAlt = (aiForcedCandidate == card) && aiForcedAlternative && baseCost && baseCost->getAlternative();
        if (viaAlt && candidates[ci].normalPayable && !forceAlt)
            continue;
        if (!viaAlt && forceAlt)
            continue; //the forced-alternative pick competes as its alt entry only
        if (viaAlt && !forceAlt && baseCost->getKicker())
            continue; //historical guard: kicker cards never auto-picked the alternative

        //Offering/Delve alternative extras still lack a working AI payment
        //walk (it hangs/crashes on them), so they stay blocked. CONVOKE is no
        //longer blocked: payTheManaCost now taps the convoke creatures and
        //floats the reduced remainder from lands, so the convoke alternative
        //resolves for the AI (both Baka's own short-of-mana casts and the GPT
        //seat's forced "cast with convoke" pick). Degrading a forced convoke to
        //normal pricing was the root of the corpus-wide non-resolution (the
        //creatures never tapped; Loxodon 2/23, March 0/17, corpus 20260725) -
        //keep viaAlt=true for convoke so the alternative cost is actually paid.
        bool altBlocked = card->basicAbilities[Constants::OFFERING];
        if (baseCost && baseCost->getAlternative() && baseCost->getAlternative()->extraCosts)
        {
            ExtraCosts * ec = baseCost->getAlternative()->extraCosts;
            for (unsigned int i = 0; i < ec->costs.size() && !altBlocked; i++)
            {
                if (dynamic_cast<Offering*> (ec->costs[i])
                    || dynamic_cast<Delve*> (ec->costs[i]))
                    altBlocked = true;
            }
        }
        if (viaAlt && altBlocked)
        {
            if (!forceAlt)
                continue;
            viaAlt = false; //forced pick degrades to normal pricing, like the old rung
        }

        bool fromGrave = game->graveyard->hasCard(card);
        bool fromHand = game->hand->hasCard(card);
        bool hasFlashback = baseCost && baseCost->getFlashback();
        bool hasRetrace = baseCost && baseCost->getRetrace();

        //willingness to satisfy the cost's extras (sacrifice/discard/...)
        if (!CanHandleCost(baseCost, card))
            continue;
        if (fromGrave && hasFlashback && !CanHandleCost(baseCost->getFlashback(), card))
            continue;
        if (fromGrave && hasRetrace && !CanHandleCost(baseCost->getRetrace(), card))
            continue;

        //residual per-card gates the oracle deliberately does not know:
        //glimmervoid alias to avoid ai stalling the game as the hint combo
        //is stuck (galvanic blast clashes with glimmervoid on activate)
        if ((card->alias == 48132) && (card->controller()->game->inPlay->countByType("artifact") < 1))
            continue;
        if (card->has(Constants::TREASON) && observer->getCurrentGamePhase() != MTG_PHASE_FIRSTMAIN)
            continue;
        //Same-subtype planeswalker/battle dupes: a stale-rules POLICY gate
        //(the 2017 rules change retired planeswalker-subtype uniqueness; the
        //engine's own MTGNewLegend enforces the modern by-NAME legend rule,
        //so a second different Liliana is a legal, often correct cast). An
        //explicit model pick must not be vetoed by it - the oracle offered
        //the cast, and a silent veto re-offers forever (wave-7: Liliana of
        //the Veil rejected 4x while Liliana, the Last Hope was in play).
        if (card != aiForcedCandidate)
        {
            if (card->hasType(Subtypes::TYPE_PLANESWALKER) && card->types.size() > 0 && game->inPlay->hasTypeSpecificInt(Subtypes::TYPE_PLANESWALKER,card->types[1]))
                continue;
            if (card->hasType(Subtypes::TYPE_BATTLE) && card->types.size() > 0 && game->inPlay->hasTypeSpecificInt(Subtypes::TYPE_BATTLE,card->types[1]))
                continue;
        }

        //Combo-hold hints are TIMING scaffolding for the heuristic ("hold
        //Pyroclasm until 2+ small creatures"). An explicit model pick
        //(aiForcedCandidate) already carries its own timing judgment - the
        //hint must not silently veto it, or the pick no-ops while the menu
        //keeps offering the card (wave-4/5 "cast no-op": Hellrider on an
        //empty board, Pyroclasm rejected 111 times in one corpus).
        if(hints && card != aiForcedCandidate && hints->HintSaysItsForCombo(observer,card))
        {
            if(hints->canWeCombo(observer,card,this))
            {
                AbilityFactory af(observer);
                int canPlay = af.parseCastRestrictions(card,card->controller(),card->getRestrictions());
                if(!canPlay)
                    continue;
                nextCardToPlay = card;
                gotPayments.clear();
                if((!pMana->canAfford(nextCardToPlay->getManaCost(),0) || nextCardToPlay->getManaCost()->getKicker()))
                    gotPayments = canPayMana(nextCardToPlay,nextCardToPlay->getManaCost(),nextCardToPlay->has(Constants::ANYTYPEOFMANA));
                return activateCombo();
            }
            else
            {
                //combo piece we can't fire yet: reserved. (Historical rung
                //behavior: this also drops any winner found before it.)
                nextCardToPlay = NULL;
                continue;
            }
        }

        int currentCost = baseCost->getConvertedCost();
        int hasX = baseCost->hasX();
        gotPayments.clear();
        int localpayAlternative = NONE;
        ManaCost * manaToPay = baseCost;

        if (viaAlt)
        {
            localpayAlternative = OTHER;
            manaToPay = baseCost->getAlternative();
            if (!pMana->canAfford(manaToPay, 0))
                gotPayments = canPayMana(card, manaToPay, card->has(Constants::ANYTYPEOFMANA));
        }
        else if (fromGrave && !card->has(Constants::CANPLAYFROMGRAVEYARD) && !card->has(Constants::TEMPFLASHBACK)
                 && !baseCost->getKicker() && (hasFlashback || hasRetrace))
        {
            //flashback / retrace pricing when the card has no direct
            //play-from-graveyard permission
            manaToPay = hasFlashback ? baseCost->getFlashback() : baseCost->getRetrace();
            gotPayments = canPayMana(card, manaToPay, card->has(Constants::ANYTYPEOFMANA));
        }
        else
        {
            if (!pMana->canAfford(baseCost, 0) || baseCost->getKicker())
                gotPayments = canPayMana(card, baseCost, card->has(Constants::ANYTYPEOFMANA));
            //morph as the last-resort pricing (hand only, like the old rung)
            if (fromHand && baseCost->getMorph() && !gotPayments.size() && !pMana->canAfford(baseCost, 0)
                && !baseCost->getKicker() && !baseCost->getAlternative())
            {
                localpayAlternative = MORPH;
                manaToPay = baseCost->getMorph();
                if (!pMana->canAfford(manaToPay, 0))
                    gotPayments = canPayMana(card, manaToPay, card->has(Constants::ANYTYPEOFMANA));
            }
        }

        //winner ordering: type rank dominates, then most expensive; an {X}
        //spell always re-competes (it scales to the pool)
        bool better = (rank < bestRank) || (rank == bestRank && (currentCost > maxCost || hasX));
        //for preformence reason we only look for specific mana if the payment couldn't be made with pmana.
        if (better && (gotPayments.size() || pMana->canAfford(manaToPay, card->has(Constants::ANYTYPEOFMANA))))
        {
            TargetChooserFactory tcf(observer);
            TargetChooser * tc = tcf.createTargetChooser(card);
            int shouldPlayPercentage = 0;
            if (tc && card == aiForcedCandidate)
            {
                //Model pick: validate targets by LEGALITY (both players,
                //the oracle's own 601.2c check), not the policy probe below
                //- chooseTarget picks ONE preferred player (opponent, for a
                //bad effect) and scans only that player's zones, so Fatal
                //Push with the only cmc<=2 creature on the CASTER's board
                //was offered by the oracle then rejected here forever
                //(wave-6/7 no-legal-target loop). The GPT target seam
                //chooses from the full legal set once the cast commits.
                bool castable = true;
                if (tc->maxtargets == 1 && !tc->validTargetsExist())
                    castable = false;
                if (tc->targetMin && !tc->validTargetsExist(tc->maxtargets))
                    castable = false;
                SAFE_DELETE(tc);
                if (!castable)
                    continue;
                shouldPlayPercentage = 90;
            }
            else if (tc)
            {
                int hasTarget = chooseTarget(tc,NULL,NULL,true);
                if(
                    (tc->maxtargets > hasTarget && tc->maxtargets > 1 && !tc->targetMin && tc->maxtargets != TargetChooser::UNLITMITED_TARGETS) ||//target=<3>creature
                    (tc->maxtargets == TargetChooser::UNLITMITED_TARGETS && hasTarget < 1)//target=creatures
                    )
                    hasTarget = 0;
                if (!hasTarget)//single target covered here.
                {
                    SAFE_DELETE(tc);
                    continue;
                }
                shouldPlayPercentage = 90;
                if(tc->targetMin && hasTarget < tc->maxtargets)
                    shouldPlayPercentage = 0;
                if(tc->maxtargets > 1 && tc->maxtargets != TargetChooser::UNLITMITED_TARGETS && hasTarget <= tc->maxtargets)
                {
                    int maxA = hasTarget-tc->maxtargets;
                    shouldPlayPercentage += (10*maxA);//reduce the chances of playing multitarget if we are not above max targets.
                }
                if(tc->maxtargets == TargetChooser::UNLITMITED_TARGETS)
                {
                    shouldPlayPercentage = 40 + (10*hasTarget);
                    int totalCost = pMana->getConvertedCost()-currentCost;
                    int totalTargets = hasTarget+hasTarget;
                    if(hasX &&  totalCost <= totalTargets)// {x} spell with unlimited targeting tend to divide damage, we want atleast 1 damage per target before casting.
                    {
                        shouldPlayPercentage = 0;
                    }
                }
                SAFE_DELETE(tc);
            }
            else
            {
                // Refactor to not check effect of lands since it always returned BAKA_EFFECT_DONTKNOW
                // If it is a land, play it
                if (card->isLand())
                {
                    shouldPlayPercentage = 90;
                }
                else {
                    int shouldPlay = effectBadOrGood(card);
                    if (shouldPlay == BAKA_EFFECT_GOOD)    {
                        shouldPlayPercentage = 90;
                    }
                    else if (BAKA_EFFECT_DONTKNOW == shouldPlay) {
                        //previously shouldPlayPercentage = 80;, I found this a little to high
                        //for cards which AI had no idea how to use.
                        shouldPlayPercentage = 60;
                    }
                    else {
                        // shouldPlay == baka_effect_bad giving it a 10 for odd ball lottery chance.
                        shouldPlayPercentage = 10;
                    }
                }
            }
            //Reduce the chances of playing a spell with X cost if available mana is low
            if (hasX)
            {
                //The X-slack penalty must be measured against the X that is
                //actually ANNOUNCEABLE, not raw pool-minus-printed-cost
                //(N-137c). For a CONVOKE X-spell the two differ by the whole
                //point of convoke: March of the Multitudes {X}{G}{W}{W} over
                //three green sources plus two white convokers has pool 3 and
                //printed cost 3, so the old xDiff was 0 and the penalty ran
                //`90 - (90*1.9)/1 = -81`. shouldPlayPercentage then went
                //NEGATIVE, and because FORCEABILITY / an explicit model pick
                //only zero the random roll `if (shouldPlayPercentage > 0)`,
                //`chance > shouldPlayPercentage` was true for every roll and
                //the candidate was silently `continue`d - no click, no
                //clickstream entry, no defer record. That is the deck137
                //silent-abort signature: offered every window, zero Soldier
                //tokens across six games (corpus 20260727 s22-24, s29-30,
                //s36-38, s41-42), and it fires BEFORE any of the convoke
                //payment machinery the wave-28/30/31 fixes hardened, which is
                //why those fixes never showed up in the arrival trace.
                //
                //ManaEngine::maxAnnounceableX is the engine's own answer to
                //"how big can X be" and already credits convoke creatures
                //(and leaves a strictly-coloured X uncredited so we never
                //over-offer an unpayable announcement). Price the heuristic
                //off the SAME number the announce menu will be built from.
                int xDiff = ManaEngine::maxAnnounceableX(this, manaToPay,
                                                         card->has(Constants::ANYTYPEOFMANA));
                if (xDiff < pMana->getConvertedCost() - currentCost)
                    xDiff = pMana->getConvertedCost() - currentCost; //never SHRINK the historical slack
                if (xDiff < 0)
                    xDiff = 0;
                shouldPlayPercentage = shouldPlayPercentage - static_cast<int> ((shouldPlayPercentage * 1.9f) / (1 + xDiff));
            }
            if(card->getManaCost() && card->getManaCost()->getKicker() && card->getManaCost()->getKicker()->isMulti)
            {
                shouldPlayPercentage = 10* size_t(gotPayments.size())/int(1+(card->getManaCost()->getConvertedCost()+card->getManaCost()->getKicker()->getConvertedCost()));
                if(shouldPlayPercentage <= 10)
                    shouldPlayPercentage = shouldPlayPercentage/3;
            }
            DebugTrace("Should I play " << (card ? card->name : "Nothing" ) << "?" << endl
                <<"shouldPlayPercentage = "<< shouldPlayPercentage);
            //cast restrictions: an alternative/morph play checks the OTHER
            //restrictions first and may fall back to the normal cost; a
            //normal play may conversely fall forward onto alternative/morph
            //when only those restrictions allow the cast
            if(localpayAlternative != NONE){
                if(card->getOtherRestrictions().size())
                {
                    AbilityFactory af(observer);
                    int canPlay = af.parseCastRestrictions(card,card->controller(),card->getOtherRestrictions());
                    if(!canPlay){
                        localpayAlternative = NONE;
                        canPlay = true;
                        if(card->getRestrictions().size())
                            canPlay = af.parseCastRestrictions(card,card->controller(),card->getRestrictions()); //Check if card can be casted at least with normal cost.
                    }
                    if(!canPlay)
                        continue;
                }
            } else{
                if(card->getRestrictions().size())
                {
                    AbilityFactory af(observer);
                    int canPlay = af.parseCastRestrictions(card,card->controller(),card->getRestrictions());
                    if(!canPlay && (card->getManaCost()->getAlternative() || card->getManaCost()->getMorph())){
                        canPlay = true;
                        if(card->getOtherRestrictions().size())
                            canPlay = af.parseCastRestrictions(card,card->controller(),card->getOtherRestrictions()); //Check if card can be casted at least with alternative costs (e.g. other or morph).
                        if(canPlay) {
                            if(card->getManaCost()->getAlternative() && !altBlocked)
                                localpayAlternative = OTHER;
                            else if(card->getManaCost()->getMorph())
                                localpayAlternative = MORPH;
                            else
                                canPlay = false;
                        }
                    }
                    if(!canPlay)
                        continue;
                }
            }
            int randomChance = randomGenerator.random();
            int chance = randomChance % 100;
            //FORCEABILITY tests: any card worth playing at all is played,
            //so scripted AI tests don't depend on the (process-global,
            //thread-shared) rand() stream. Deliberate zeros still skip.
            if ((forceBestAbilityUse || aiForcedCandidate) && shouldPlayPercentage > 0)
                chance = 0;
            if (chance > shouldPlayPercentage)
                continue;
            if(shouldPlayPercentage <= 10)
            {
                DebugTrace("shouldPlayPercentage was less than 10 this was a lottery roll on RNG");
            }
            nextCardToPlay = card;
            payAlternative = localpayAlternative;
            bestRank = rank;
            maxCost = currentCost;
            if (hasX)
                maxCost = pMana->getConvertedCost();
        }
    }
    if(nextCardToPlay)
    {
        if(game->graveyard->hasCard(nextCardToPlay) && !nextCardToPlay->has(Constants::CANPLAYFROMGRAVEYARD) && !nextCardToPlay->has(Constants::TEMPFLASHBACK)){ //Now AI can play cards with flashback and retrace costs.
            if(nextCardToPlay->getManaCost()->getFlashback()){
                if(!pMana->canAfford(nextCardToPlay->getManaCost()->getFlashback(),0))
                    gotPayments = canPayMana(nextCardToPlay,nextCardToPlay->getManaCost()->getFlashback(),nextCardToPlay->has(Constants::ANYTYPEOFMANA));
            } else if(nextCardToPlay->getManaCost()->getRetrace()){
                if(!pMana->canAfford(nextCardToPlay->getManaCost()->getRetrace(),0))
                    gotPayments = canPayMana(nextCardToPlay,nextCardToPlay->getManaCost()->getRetrace(),nextCardToPlay->has(Constants::ANYTYPEOFMANA));
            }
        } else if(payAlternative == OTHER){
            if(!pMana->canAfford(nextCardToPlay->getManaCost()->getAlternative(),0)) // Now AI can cast cards using alternative cost.
                gotPayments = canPayMana(nextCardToPlay,nextCardToPlay->getManaCost()->getAlternative(),nextCardToPlay->has(Constants::ANYTYPEOFMANA));
        } else if(payAlternative == MORPH){
            if(!pMana->canAfford(nextCardToPlay->getManaCost()->getMorph(),0)) // Now AI can cast cards using morph cost.
                gotPayments = canPayMana(nextCardToPlay,nextCardToPlay->getManaCost()->getMorph(),nextCardToPlay->has(Constants::ANYTYPEOFMANA));
        } else {
            if(!pMana->canAfford(nextCardToPlay->getManaCost(),0) || nextCardToPlay->getManaCost()->getKicker())
                gotPayments = canPayMana(nextCardToPlay,nextCardToPlay->getManaCost(),nextCardToPlay->has(Constants::ANYTYPEOFMANA));
        }
        DebugTrace(" AI wants to play card." << endl
            << "- Next card to play: " << (nextCardToPlay ? nextCardToPlay->name : "None" ) << endl );

        if(hints && nextCardToPlay != aiForcedCandidate && hints->HintSaysItsForCombo(observer,nextCardToPlay))
        {
            DebugTrace(" AI wants to play a card that belongs to a combo.");
            nextCardToPlay = NULL;
        }

    }
    return nextCardToPlay;
}

MTGCardInstance * AIPlayerBaka::activateCombo()
{
    if(!comboHint)
        return NULL;
    TargetChooserFactory tfc(observer);
    ManaCost * totalCost = ManaCost::parseManaCost(comboHint->manaNeeded);
    for(unsigned int k = 0;k < comboHint->casting.size(); k++)
    {
        TargetChooser *hintTc = tfc.createTargetChooser(comboHint->casting[k],nextCardToPlay);
        int combohand = game->hand->cards.size();
        for(int j = 0; j < combohand;j++)
        {
            if(!hintTc)
                break;
            if(hintTc->canTarget(game->hand->cards[j]))
            {
                comboCards.push_back(game->hand->cards[j]);
                SAFE_DELETE(hintTc);
            }
        }
        SAFE_DELETE(hintTc);
    }
    //Affordability CHECK only - no payment. This used to payTheManaCost the
    //whole totalmananeeded here, and then the normal cast path paid the piece
    //AGAIN, queueing duplicate producer clicks; the second click on an
    //already-tapped producer refuses, and a refused payment click aborts the
    //rest of the plan (the wave-20 stall guard) - so the combo card was never
    //cast and the pre-floated mana sat in the pool every turn (live-observed:
    //deck15 floating {B}{B} for Fear, Fear never cast). The cast path is the
    //single payer; each combo piece pays its own cost when it is cast.
    bool comboAffordable = !totalCost->getConvertedCost();
    if (!comboAffordable)
    {
        ManaCost * avail = getPotentialMana();
        avail->add(this->getManaPool());
        comboAffordable = avail->canAfford(totalCost, 0) != 0;
        SAFE_DELETE(avail);
    }
    if(comboAffordable)
    {
        if(comboCards.size())
        {
            nextCardToPlay = comboCards.back();

            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());
            DebugTrace("ai is doing a combo:" << nextCardToPlay->getName());

            if (game->playRestrictions->canPutIntoZone(nextCardToPlay, game->stack) == PlayRestriction::CANT_PLAY)
                return NULL;
            comboCards.pop_back();
        }
    }
    SAFE_DELETE(totalCost);
    return nextCardToPlay;
}

void AIPlayerBaka::initTimer()
{
    if (mFastTimerMode)
        timer = 0.07f; //0 or 1 is as fast as possible and will generate bad side effects in the game engine (phases getting skipped...), use wisely
    else
        timer = 0.1f;
}

int AIPlayerBaka::computeActions()
{
    /*Zeth fox:TODO:rewrite this entire function, It's a mess.
    I made it far to complicated for what it does and is prone to error and inefficiency.
    Ai run's certain part's when it doesn't need to and run's certain actions when it shouldn't, 
    and it is far to easy to cripple the ai even with what appears to be a minor change to this function;
    reasoning:I split this from 2 to 3 else statements, leaving chooseblockers in the 3rd else,
    the 2nd else is run about 90% of the time over the third, this was causing ai to miss the chance to chooseblockers()
    when it could have blocked almost 90% of the time.*/
    Player * p = this;
    Player * currentP = observer->currentlyActing();
    if (!(currentP == p))
        return 0;
    ActionLayer * object = observer->mLayers->actionLayer();
    if (object->menuObject)
    {
        int doThis = selectMenuOption();

        // FIXME, action logging is broken in the multiplechoice case.
        if(doThis >= 0)
        {
            if(object->abilitiesMenu->isMultipleChoice)
                observer->mLayers->actionLayer()->ButtonPressedOnMultipleChoice(doThis);
            else
                observer->mLayers->actionLayer()->doReactTo(doThis);
        }
        else if(doThis < 0 || object->checkCantCancel())
            observer->mLayers->actionLayer()->doReactTo(object->abilitiesMenu->mObjects.size()-1);
        return 1;
    }
    TargetChooser * currentTc = observer->getCurrentTargetChooser();
    if(currentTc)
    {
        int targetResult = currentTc->Owner == this? chooseTarget():0;
        if (targetResult)
            return 1;
    }

    //Engine-issued combat decisions (W3b): the ENGINE decides when a
    //declaration is due (right phase/step, stack settled - attack triggers
    //resolve BEFORE blocks - nothing else in flight, legal set non-empty).
    //Consulted ahead of the branch maze below, whose reach conditions
    //(zero NOT_RESOLVED on the stack, isInterrupting not latched) silently
    //dropped block decisions for years. The choosers themselves converge:
    //re-invocation on later ticks is a no-op once the declaration stands.
    switch (observer->pendingCombatDecision(this))
    {
    case GameObserver::COMBAT_DECISION_ATTACKERS:
        chooseAttackers();
        return 1;
    case GameObserver::COMBAT_DECISION_BLOCKERS:
        chooseBlockers();
        selectAbility(); //parity with the old blockers case: the defender
                         //may still activate abilities after declaring
        return 1;
    default:
        break;
    }

#ifndef AI_CHANGE_TESTING
    static bool findingCard = false;
    //this guard is put in place to prevent Ai from
    //ever running computeActions() function WHILE its already doing so.
    // Break if this happens in debug mode. If this happens, it's actually a bug
    assert(!findingCard);
    if (findingCard)
    {//is already looking kick me out of this function!
        return 0;
    } 
#endif //AI_CHANGE_TESTING

    Interruptible * action = observer->mLayers->stackLayer()->getAt(-1);
    Spell * spell = dynamic_cast<Spell *>(action);
    Player * lastStackActionController = spell ? spell->source->controller() : NULL;         
    if (observer->isInterrupting == this
        && this == currentP 
        //and i am the currentlyActivePlayer
        && ((lastStackActionController && lastStackActionController != this) || (observer->mLayers->stackLayer()->count(0, NOT_RESOLVED) == 0)))
        //am im not interupting my own spell, or the stack contains nothing.
    {
        bool ipotential = false;
        if(p->game->hand->hasType("instant") || p->game->hand->hasAbility(Constants::FLASH) || p->game->hand->hasAbility(Constants::ASFLASH) ||
            p->game->graveyard->hasType("instant") || p->game->graveyard->hasAbility(Constants::FLASH) || p->game->graveyard->hasAbility(Constants::ASFLASH) ||
            p->game->exile->hasType("instant") || p->game->exile->hasAbility(Constants::FLASH) || p->game->exile->hasAbility(Constants::ASFLASH) || 
            p->game->commandzone->hasAbility(Constants::FLASH) || p->game->commandzone->hasAbility(Constants::ASFLASH)) //Now AI will not search just for instant cards.
        {
#ifndef AI_CHANGE_TESTING
            findingCard = true;
#endif //AI_CHANGE_TESTING
            ManaCost * icurrentMana = getPotentialMana();
            icurrentMana->add(this->getManaPool());
            if (icurrentMana->getConvertedCost())
            {
                //if theres mana i can use there then potential is true.
                ipotential = true;
            }
            if (!nextCardToPlay)
            {
                nextCardToPlay = FindCardToPlay(icurrentMana, ""); //Now AI will not search just for instant cards.
                bool canPlay = false;
                if(nextCardToPlay && p->game->hand->hasCard(nextCardToPlay)){
                    if(nextCardToPlay->hasType(Subtypes::TYPE_INSTANT) || nextCardToPlay->has(Constants::FLASH) || nextCardToPlay->has(Constants::ASFLASH))
                        canPlay = true;
                } else if(nextCardToPlay && p->game->graveyard->hasCard(nextCardToPlay)){
                    if((nextCardToPlay->hasType(Subtypes::TYPE_INSTANT) || nextCardToPlay->has(Constants::FLASH) || nextCardToPlay->has(Constants::ASFLASH)) && nextCardToPlay->has(Constants::CANPLAYFROMGRAVEYARD))
                        canPlay = true;
                    else if((nextCardToPlay->hasType(Subtypes::TYPE_INSTANT) || nextCardToPlay->has(Constants::FLASH) || nextCardToPlay->has(Constants::ASFLASH)) && nextCardToPlay->has(Constants::TEMPFLASHBACK))
                        canPlay = true;
                    else if((nextCardToPlay->hasType(Subtypes::TYPE_INSTANT) || nextCardToPlay->has(Constants::FLASH) || nextCardToPlay->has(Constants::ASFLASH)) && nextCardToPlay->getManaCost()->getFlashback())
                        canPlay = true;
                    else if((nextCardToPlay->hasType(Subtypes::TYPE_INSTANT) || nextCardToPlay->has(Constants::FLASH) || nextCardToPlay->has(Constants::ASFLASH)) && nextCardToPlay->getManaCost()->getRetrace())
                        canPlay = true;
                } else if(nextCardToPlay && p->game->exile->hasCard(nextCardToPlay)){
                    if((nextCardToPlay->hasType(Subtypes::TYPE_INSTANT) || nextCardToPlay->has(Constants::FLASH) || nextCardToPlay->has(Constants::ASFLASH)) && nextCardToPlay->has(Constants::CANPLAYFROMEXILE))
                        canPlay = true;
                } else if(nextCardToPlay && p->game->commandzone->hasCard(nextCardToPlay)){
                    if(nextCardToPlay->has(Constants::FLASH) || nextCardToPlay->has(Constants::ASFLASH))
                        canPlay = true;
                }
                if(!canPlay)
                    nextCardToPlay = NULL;
                if (nextCardToPlay && game->playRestrictions->canPutIntoZone(nextCardToPlay, game->stack) == PlayRestriction::CANT_PLAY)
                    nextCardToPlay = NULL;
            }
            //A response whose selected cast mode needs NO mana (Force of
            //Negation's free "exile a blue card" alternative, and other
            //extra-cost-only pitches) is payable on a fully tapped board, but
            //ipotential above only tracks raw mana existence - so a zero-mana
            //seat that legitimately holds the free counter never reached the
            //cast branch below (corpus 20260716: (no untapped sources)
            //correlated 3/3 with fizzled answered Force casts). FindCardToPlay
            //(via the oracle's legalCasts) already proved this exact mode is
            //payable - it only returns a card it can pay for, and legalCasts
            //requires the alternative's targeted extra costs to have a legal
            //target. Admit the mode when its MANA part is empty; everything
            //that actually needs mana still rides the ipotential gate.
            if (nextCardToPlay && !ipotential)
            {
                ManaCost * selCost = nextCardToPlay->getManaCost();
                ManaCost * modeCost = selCost ?
                    (payAlternative == OTHER ? selCost->getAlternative()
                     : payAlternative == MORPH ? selCost->getMorph()
                     : selCost) : NULL;
                if (modeCost && !modeCost->getConvertedCost())
                    ipotential = true;
            }
            SAFE_DELETE (icurrentMana);
        }
        if (!nextCardToPlay)
        {
            selectAbility();
        }
        if (nextCardToPlay)
        {
            if (ipotential)
            {
                if(game->graveyard->hasCard(nextCardToPlay) && !nextCardToPlay->has(Constants::CANPLAYFROMGRAVEYARD) && !nextCardToPlay->has(Constants::TEMPFLASHBACK)){ //Now AI can play cards with flashback and retrace costs.
                    if(nextCardToPlay->getManaCost()->getFlashback()){
                        if(payTheManaCost(nextCardToPlay->getManaCost()->getFlashback(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                        {
                            AIAction * a = NEW AIAction(this, nextCardToPlay);
                            clickstream.push(a);
                            gotPayments.clear();
                        }
                    } else if(nextCardToPlay->getManaCost()->getRetrace()){
                        if(payTheManaCost(nextCardToPlay->getManaCost()->getRetrace(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                        {
                            AIAction * a = NEW AIAction(this, nextCardToPlay);
                            clickstream.push(a);
                            gotPayments.clear();
                        }
                    }
                } else if(payAlternative == OTHER){ // Now AI can cast cards using other cost.
                    if(payTheManaCost(nextCardToPlay->getManaCost()->getAlternative(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                    {
                        AIAction * a = NEW AIAction(this, nextCardToPlay);
                        clickstream.push(a);
                        gotPayments.clear();
                    }
                } else if(payAlternative == MORPH){ // Now AI can cast cards using morph cost.
                    if(payTheManaCost(nextCardToPlay->getManaCost()->getMorph(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                    {
                        AIAction * a = NEW AIAction(this, nextCardToPlay);
                        clickstream.push(a);
                        gotPayments.clear();
                    }
                } else {
                    //c5a: plain casts (hand zone, normal cost, no extra
                    //costs/X/kicker) are validated and payment-planned by
                    //the contract manager; the clicks stay on the AI
                    //clickstream's one-per-tick cadence (see the
                    //planCastSpell header note on the ability-GC latent
                    //double-destroy). Everything else (and any cast the
                    //manager declines) keeps the legacy path below.
                    bool committed = false;
                    {
                        BakaManaPolicy castPolicy(this);
                        ManaCost * castMana = getPotentialMana(nextCardToPlay);
                        DecisionRequest castReq;
                        if (castMana)
                            castMana->add(this->getManaPool());
                        if (castMana && DecisionManager::buildCastSpell(this, castPolicy, castMana, false, castReq))
                        {
                            DecisionAction castAct;
                            for (size_t ci = 0; ci < castReq.casts.size(); ci++)
                                if (castReq.casts[ci].card == nextCardToPlay && !castReq.casts[ci].viaAlternative)
                                {
                                    castAct.choice = (int) ci;
                                    vector<MTGAbility*> castProducers;
                                    if (DecisionManager::planCastSpell(castReq, castAct, castPolicy, castProducers))
                                    {
                                        for (size_t pk = 0; pk < castProducers.size(); pk++)
                                        {
                                            if (AManaProducer * amp = dynamic_cast<AManaProducer*>(castProducers[pk]))
                                                clickstream.push(NEW AIAction(this, amp, amp->source));
                                            else if (GenericActivatedAbility * gmp = dynamic_cast<GenericActivatedAbility*>(castProducers[pk]))
                                                clickstream.push(NEW AIAction(this, gmp, gmp->source));
                                        }
                                        clickstream.push(NEW AIAction(this, nextCardToPlay));
                                        committed = true;
                                    }
                                    break;
                                }
                        }
                        delete castMana;
                        if (committed)
                            gotPayments.clear();
                    }
                    if(!committed && payTheManaCost(nextCardToPlay->getManaCost(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                    {
                        AIAction * a = NEW AIAction(this, nextCardToPlay);
                        clickstream.push(a);
                        gotPayments.clear();
                    }
                }
            }
#ifndef AI_CHANGE_TESTING
            findingCard = false;
#endif //AI_CHANGE_TESTING
            nextCardToPlay = NULL;
            return 1;
        }
        nextCardToPlay = NULL;
#ifndef AI_CHANGE_TESTING
        findingCard = false;
#endif //AI_CHANGE_TESTING
        return 1;
    }
    else if(observer->mLayers->stackLayer()->count(0, NOT_RESOLVED) == 0)
    { //standard actions
        switch (observer->getCurrentGamePhase())
        {
        case MTG_PHASE_UPKEEP:
            selectAbility();
            break;
        case MTG_PHASE_FIRSTMAIN:
        case MTG_PHASE_SECONDMAIN:
            {
                ManaCost * currentMana = getPotentialMana();
                currentMana->add(this->getManaPool());

                nextCardToPlay = FindCardToPlay(currentMana, "land");
                if (nextCardToPlay && nextCardToPlay->isLand() && game->playRestrictions->canPutIntoZone(nextCardToPlay, game->battlefield) == PlayRestriction::CANT_PLAY)
                    nextCardToPlay = NULL;//look for a land, did we find one we can play..if not set to null now.
                if(hints && hints->mCastOrder().size())
                {
                    vector<string>findType = hints->mCastOrder();
                    for(unsigned int j = 0;j < findType.size();j++)
                    {
                        if(nextCardToPlay) 
                            continue;//if there is a card to play on first run of this, it is most likly a land.
                        if(clickstream.size())
                        {
                            SAFE_DELETE(currentMana);
                            return 0;
                        }
                        nextCardToPlay = FindCardToPlay(currentMana, findType[j].c_str());
                        if (game->playRestrictions->canPutIntoZone(nextCardToPlay, game->stack) == PlayRestriction::CANT_PLAY)
                            nextCardToPlay = NULL;
                        if (nextCardToPlay && nextCardToPlay->isLand() && game->playRestrictions->canPutIntoZone(nextCardToPlay, game->battlefield) == PlayRestriction::CANT_PLAY)
                            nextCardToPlay = NULL;
                    }
                }
                else
                {
                    //c5c: the old per-type rescan ladder is one prioritized
                    //call now - FindCardToPlay treats the comma list as
                    //rank-dominant weights (most expensive within a type).
                    //"battle" was unreachable in the old ladder (count < 6
                    //off-by-one); it is deliberately included here.
                    if (!nextCardToPlay)
                    {
                        if(clickstream.size()) //don't find cards while we have clicking to do.
                        {
                            SAFE_DELETE(currentMana);
                            return 0;
                        }
                        nextCardToPlay = FindCardToPlay(currentMana, "planeswalker,creature,enchantment,artifact,sorcery,instant,battle");
                        if (game->playRestrictions->canPutIntoZone(nextCardToPlay, game->stack) == PlayRestriction::CANT_PLAY)
                            nextCardToPlay = NULL;
                        if (nextCardToPlay && nextCardToPlay->isLand() && game->playRestrictions->canPutIntoZone(nextCardToPlay, game->battlefield) == PlayRestriction::CANT_PLAY)
                            nextCardToPlay = NULL;
                    }

                    if(nextCardToPlay == NULL)//check if there is a free card to play, play it....
                    {//TODO: add potential mana if we can pay if there is a cost increaser in play
                        CardDescriptor cd;
                        if (game->hand->hasAbility(Constants::PAYZERO))
                        {
                            //Attempt to put free cards into play from hand
                            cd.init();
                            cd.SetExclusionColor(Constants::MTG_COLOR_LAND);
                            MTGCardInstance *freecard = cd.match(game->hand);
                            int canCastCard = game->playRestrictions->canPutIntoZone(freecard, game->inPlay);
                            if (freecard && (canCastCard == PlayRestriction::CAN_PLAY) && freecard->has(Constants::PAYZERO) && (freecard->getIncreasedManaCost()->getConvertedCost() < 1))
                            {
                                MTGAbility * castFreeCard = observer->mLayers->actionLayer()->getAbility(MTGAbility::PAYZERO_COST);
                                AIAction * aa = NEW AIAction(this, castFreeCard, freecard); //TODO putinplay action
                                clickstream.push(aa);
                                break;
                            }
                        }
                        if (game->graveyard->hasAbility(Constants::PAYZERO) && game->graveyard->hasAbility(Constants::CANPLAYFROMGRAVEYARD))
                        {
                            //Attempt to put free cards into play from graveyard
                            cd.init();
                            cd.SetExclusionColor(Constants::MTG_COLOR_LAND);
                            MTGCardInstance *freecard = cd.match(game->graveyard);
                            int canCastCard = game->playRestrictions->canPutIntoZone(freecard, game->inPlay);
                            if (freecard && (canCastCard == PlayRestriction::CAN_PLAY) && freecard->has(Constants::PAYZERO) && freecard->has(Constants::CANPLAYFROMGRAVEYARD) && (freecard->getIncreasedManaCost()->getConvertedCost() < 1) && (!freecard->isCDA))
                            {
                                MTGAbility * castFreeCard = observer->mLayers->actionLayer()->getAbility(MTGAbility::PAYZERO_COST);
                                AIAction * aa = NEW AIAction(this, castFreeCard, freecard); //TODO putinplay action
                                clickstream.push(aa);
                                break;
                            }
                        }
                        if (game->exile->hasAbility(Constants::PAYZERO) && game->exile->hasAbility(Constants::CANPLAYFROMEXILE))
                        {
                            //Attempt to put free cards into play from exile
                            cd.init();
                            cd.SetExclusionColor(Constants::MTG_COLOR_LAND);
                            MTGCardInstance *freecard = cd.match(game->exile);
                            int canCastCard = game->playRestrictions->canPutIntoZone(freecard, game->inPlay);
                            if (freecard && (canCastCard == PlayRestriction::CAN_PLAY) && freecard->has(Constants::PAYZERO) && freecard->has(Constants::CANPLAYFROMEXILE) && (freecard->getIncreasedManaCost()->getConvertedCost() < 1) && (!freecard->isCDA))
                            {
                                MTGAbility * castFreeCard = observer->mLayers->actionLayer()->getAbility(MTGAbility::PAYZERO_COST);
                                AIAction * aa = NEW AIAction(this, castFreeCard, freecard); //TODO putinplay action
                                clickstream.push(aa);
                                break;
                            }
                        }
                        if (game->commandzone->hasAbility(Constants::PAYZERO))
                        {
                            //Attempt to put free cards into play from commandzone
                            cd.init();
                            cd.SetExclusionColor(Constants::MTG_COLOR_LAND);
                            MTGCardInstance *freecard = cd.match(game->commandzone);
                            int canCastCard = game->playRestrictions->canPutIntoZone(freecard, game->inPlay);
                            if (freecard && (canCastCard == PlayRestriction::CAN_PLAY) && freecard->has(Constants::PAYZERO) && (freecard->getIncreasedManaCost()->getConvertedCost() < 1) && (!freecard->isCDA))
                            {
                                MTGAbility * castFreeCard = observer->mLayers->actionLayer()->getAbility(MTGAbility::PAYZERO_COST);
                                AIAction * aa = NEW AIAction(this, castFreeCard, freecard); //TODO putinplay action
                                clickstream.push(aa);
                                break;
                            }
                        }
                    }//end
                }

                SAFE_DELETE(currentMana);
                if (nextCardToPlay)
                {
                    if(nextCardToPlay->has(Constants::SUNBURST))
                    {
                        //had to force this on Ai other wise it would pay nothing but 1 color for a sunburst card.
                        //this does not teach it to use manaproducer more effectively, it simply allow it to 
                        //use the manaproducers it does understand better on sunburst by force.
                        vector<MTGAbility*>checking = canPaySunBurst(nextCardToPlay->getManaCost());
                        if(payTheManaCost(nextCardToPlay->getManaCost(),0,NULL,checking))
                        {
                            AIAction * a = NEW AIAction(this, nextCardToPlay);
                            clickstream.push(a);
                            return 1;
                        }
                        nextCardToPlay = NULL;
                        gotPayments.clear();//if any.
                        return 1;
                    }
                    if(game->graveyard->hasCard(nextCardToPlay) && !nextCardToPlay->has(Constants::CANPLAYFROMGRAVEYARD) && !nextCardToPlay->has(Constants::TEMPFLASHBACK)){ //Now AI can play cards with flashback and retrace costs.
                        if(nextCardToPlay->getManaCost()->getFlashback()){
                            if(payTheManaCost(nextCardToPlay->getManaCost()->getFlashback(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                            {
                                AIAction * a = NEW AIAction(this, nextCardToPlay);
                                clickstream.push(a);
                                gotPayments.clear();
                            }
                        } else if(nextCardToPlay->getManaCost()->getRetrace()){
                            if(payTheManaCost(nextCardToPlay->getManaCost()->getRetrace(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                            {
                                AIAction * a = NEW AIAction(this, nextCardToPlay);
                                clickstream.push(a);
                                gotPayments.clear();
                            }
                        }
                    } else if(payAlternative == OTHER){ // Now AI can cast cards using other cost.
                        if(payTheManaCost(nextCardToPlay->getManaCost()->getAlternative(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                        {
                            AIAction * a = NEW AIAction(this, nextCardToPlay);
                            clickstream.push(a);
                            gotPayments.clear();
                        }
                    } else if(payAlternative == MORPH){ // Now AI can cast cards using morph cost.
                        if(payTheManaCost(nextCardToPlay->getManaCost()->getMorph(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                        {
                            AIAction * a = NEW AIAction(this, nextCardToPlay);
                            clickstream.push(a);
                            gotPayments.clear();
                        }
                    } else {
                        if(payTheManaCost(nextCardToPlay->getManaCost(),nextCardToPlay->has(Constants::ANYTYPEOFMANA),nextCardToPlay,gotPayments))
                        {
                            AIAction * a = NEW AIAction(this, nextCardToPlay);
                            clickstream.push(a);
                            gotPayments.clear();
                        }
                    }
                    return 1;
                }
                else
                {
                    if(observer->mExtraPayment)
                    {
                        //no extra payment should be waiting before selecting an ability.
                        observer->mExtraPayment->action->CheckUserInput(JGE_BTN_SEC);
                        observer->mExtraPayment = NULL;
                    }
                    //this is a fix for a rare bug that somehow ai trips over an extra payment without paying
                    //then locks in a loop of trying to choose something different to do and trying to pay the extra payment.
                    selectAbility();
                }
                break;
            }
        case MTG_PHASE_COMBATATTACKERS:
            {
                //declaration is engine-issued now (pendingCombatDecision
                //consult at the top of this function)
                break;
            }
        case MTG_PHASE_COMBATBLOCKERS:
            {
                //declaration engine-issued; abilities still selectable
                selectAbility();
                break;
            }
        case MTG_PHASE_COMBATDAMAGE:
        case MTG_PHASE_ENDOFTURN:
            selectAbility();
            break;
        default:
            break;
        }
    }
    else
    {
        switch (observer->getCurrentGamePhase())
        {
        case MTG_PHASE_UPKEEP:
        case MTG_PHASE_FIRSTMAIN:
        case MTG_PHASE_COMBATATTACKERS:
        case MTG_PHASE_COMBATBLOCKERS:
        case MTG_PHASE_COMBATDAMAGE:
        case MTG_PHASE_SECONDMAIN:
            {
                selectAbility();
                break;
            }
        default:
            break;
        }
        return 1;
    }
    return 1;
};


//
// Combat //
//

int AIPlayerBaka::getCreaturesInfo(Player * player, int neededInfo, int untapMode, int canAttack)
{
    int result = 0;
    CardDescriptor cd;
    cd.init();
    cd.setType("Creature");
    cd.unsecureSetTapped(untapMode);
    MTGCardInstance * card = NULL;
    while ((card = cd.nextmatch(player->game->inPlay, card)))
    {
        if (!canAttack || card->canAttack())
        {
            if (neededInfo == INFO_NBCREATURES)
            {
                result++;
            }
            else
            {//AI should consider COMBATTOUGHNESS for attackers and blockers
                result += card->has(Constants::COMBATTOUGHNESS) ? card->toughness : card->power;
            }
        }
    }
    return result;
}

int AIPlayerBaka::chooseAttackers()
{
    int myCreatures = getCreaturesInfo(this, INFO_NBCREATURES, -1, 1);
    if (myCreatures < 1)
        return 0;
    //Attack with all creatures
    //How much damage can the other player do during his next Attack ?
    int opponentForce = getCreaturesInfo(opponent(), INFO_CREATURESPOWER);
    int opponentCreatures = getCreaturesInfo(opponent(), INFO_NBCREATURES);
    int myForce = getCreaturesInfo(this, INFO_CREATURESPOWER, -1, 1);
    if(opponent()->life < 5)
        agressivity += 31;

    bool attack = ((myCreatures > opponentCreatures) || (myForce > opponentForce) || (myForce > 2 * opponent()->life));
    if (agressivity > 80 && !attack && life > opponentForce)
    {
        opponentCreatures = getCreaturesInfo(opponent(), INFO_NBCREATURES, -1);
        opponentForce = getCreaturesInfo(opponent(), INFO_CREATURESPOWER, -1);
        attack = (myCreatures >= opponentCreatures && myForce > opponentForce)
            || (myForce > opponentForce) || (myForce > opponent()->life) || ((life - opponentForce) > 30) ;
    }
    printf("Choose attackers : %i %i %i %i -> %i\n", opponentForce, opponentCreatures, myForce, myCreatures, attack);

    CardDescriptor cd;
    cd.init();
    cd.setType("creature");
    MTGCardInstance * card = NULL;
    while ((card = cd.nextmatch(game->inPlay, card)))
    {
        if (shouldAIForceAttack(card, attack))
        {
            if (card->attackCost)
            {
                MTGAbility* a = observer->mLayers->actionLayer()->getAbility(MTGAbility::ATTACK_COST);
                doAbility(a, card);
                observer->cardClick(card, MTGAbility::ATTACK_COST);
            }
            observer->cardClick(card, MTGAbility::MTG_ATTACK_RULE);
        }
    }

    if (attack)
    {
        CardDescriptor cd;
        cd.init();
        cd.setType("creature");
        MTGCardInstance * card = NULL;
        while ((card = cd.nextmatch(game->inPlay, card)))
        {
            if(hints && hints->HintSaysDontAttack(observer,card))
                continue;
            if (!card->isAttacker())
            {
                if (card->attackCost)
                {
                    MTGAbility * a = observer->mLayers->actionLayer()->getAbility(MTGAbility::ATTACK_COST);
                    doAbility(a, card);
                    observer->cardClick(card, MTGAbility::ATTACK_COST);
                }
                observer->cardClick(card, MTGAbility::MTG_ATTACK_RULE);
            }
        }
    }
    return 1;
}

bool AIPlayerBaka::shouldAIForceAttack(MTGCardInstance* card, bool globalAttack)
{
    if (globalAttack)
        return true;

    if (!card || card->isAttacker())
        return false;

    if (hints)
    {
        if (hints->HintSaysDontAttack(observer, card))
            return false;
        if (hints->HintSaysAlwaysAttack(observer, card))
            return true;
    }

    if (card->has(Constants::UNBLOCKABLE))
        return true;

    // Flags for opponent defenses
    bool oppHasShadow = false;
    bool oppHasAirDefense = false;
    bool oppHasHorsemanship = false;
    bool oppHasBlackOrArtifact = false;
    bool oppHasMatchingColorOrArtifact = false;

    MTGCardInstance* oppCard = NULL;
    CardDescriptor desc;
    desc.init();
    desc.setType("creature");

    while ((oppCard = desc.nextmatch(opponent()->game->inPlay, oppCard)))
    {
        if (oppCard->isTapped())
            continue;

        if (oppCard->has(Constants::SHADOW))
            oppHasShadow = true;
        if (oppCard->has(Constants::FLYING) || oppCard->has(Constants::REACH))
            oppHasAirDefense = true;
        if (oppCard->has(Constants::HORSEMANSHIP))
            oppHasHorsemanship = true;

        if (oppCard->hasColor(Constants::MTG_COLOR_BLACK) || oppCard->hasType("Artifact"))
            oppHasBlackOrArtifact = true;

        // Intimidate check: artifact or shares color
        if (oppCard->hasType("Artifact") || (oppCard->colors & card->colors))
            oppHasMatchingColorOrArtifact = true;
    }

    // Decision logic based on evasion
    if ((card->has(Constants::SHADOW) && !oppHasShadow) ||
        (card->has(Constants::FLYING) && !oppHasAirDefense) ||
        (card->has(Constants::HORSEMANSHIP) && !oppHasHorsemanship) ||
        (card->has(Constants::FEAR) && !oppHasBlackOrArtifact) ||
        (card->has(Constants::INTIMIDATE) && !oppHasMatchingColorOrArtifact))
        return true;

    // Landwalk abilities
    if ((card->has(Constants::SWAMPWALK) && opponent()->game->inPlay->hasType("Swamp")) ||
        (card->has(Constants::ISLANDWALK) && opponent()->game->inPlay->hasType("Island")) ||
        (card->has(Constants::FORESTWALK) && opponent()->game->inPlay->hasType("Forest")) ||
        (card->has(Constants::MOUNTAINWALK) && opponent()->game->inPlay->hasType("Mountain")) ||
        (card->has(Constants::PLAINSWALK) && opponent()->game->inPlay->hasType("Plains")))
        return true;

    return false;
}

/* Can I first strike my oponent and get away with murder ? */
int AIPlayerBaka::canFirstStrikeKill(MTGCardInstance * card, MTGCardInstance *ennemy)
{
    if(hints && hints->HintSaysAlwaysBlock(observer,ennemy))
        return 1;
    if (ennemy->has(Constants::FIRSTSTRIKE) || ennemy->has(Constants::DOUBLESTRIKE))
        return 0;
    if (!(card->has(Constants::FIRSTSTRIKE) || card->has(Constants::DOUBLESTRIKE)))
        return 0;
    if (!(card->power >= ennemy->toughness))
        return 0;
    if (!(card->power >= ennemy->toughness + 1) && ennemy->has(Constants::FLANKING))
        return 0;
    return 1;
}

int AIPlayerBaka::chooseBlockers()
{
    //Should not block during my own turn...
    if (observer->currentPlayer == this)
        return 0;

    map<MTGCardInstance*, int> opponentsToughness;

    //Initialize the list of opponent's attacking cards toughness
    CardDescriptor cdAttackers;
    cdAttackers.init();
    cdAttackers.setType("Creature");
    MTGCardInstance* card = NULL;

    // Gather all attacking creatures and store their toughness
    while ((card = cdAttackers.nextmatch(opponent()->game->inPlay, card)))
    {
        if (card->isAttacker())
            opponentsToughness[card] = card->toughness;
    }

    //A Descriptor to find untapped creatures in our game
    CardDescriptor cd;
    cd.init();
    cd.setType("Creature");
    cd.unsecureSetTapped(-1);
    card = NULL;

    // First pass: auto-block top 3 threats if can be killed
    while ((card = cd.nextmatch(game->inPlay, card)))
    {
        if (hints && hints->HintSaysDontBlock(observer, card))
            continue;

        //A 0-power creature can never contribute to KILLING an attacker (this
        //pass's whole purpose), yet blocking with one here left the attacker's
        //tracked remaining toughness unchanged - so it still read as "needs a
        //blocker" and pulled a real blocker on top, gang-chumping a valuable
        //0-power engine (Argothian Enchantress 0/1) that a single real blocker
        //already killed (deck62 wave-20 N8: Yavimaya 4/6 + Argothian 0/1 on a
        //2/2; two Argothians on a 2/2). Skip 0-power creatures in the kill pass;
        //a lone 0-power chump to survive lethal is still reached in pass 3.
        if (card->power == 0)
            continue;

        observer->cardClick(card, MTGAbility::MTG_BLOCK_RULE);
        int set = 0;
        while (!set)
        {
            if (!card->defenser)
            {
                set = 1;
            }
            else
            {
                MTGCardInstance* attacker = card->defenser;
                map<MTGCardInstance*, int>::iterator it = opponentsToughness.find(attacker);
                if (it == opponentsToughness.end())
                {
                    opponentsToughness[attacker] = attacker->toughness;
                    it = opponentsToughness.find(attacker);
                }
                if (opponentsToughness[attacker] > 0 && getStats() && getStats()->isInTop(attacker, 3, false))
                {
                    opponentsToughness[attacker] -= card->power;
                    set = 1;
                }
                else
                {
                    if (card->blockCost)
                    {
                        MTGAbility* a = observer->mLayers->actionLayer()->getAbility(MTGAbility::BLOCK_COST);
                        doAbility(a, card);
                        observer->cardClick(card, MTGAbility::BLOCK_COST);
                    }
                    observer->cardClick(card, MTGAbility::MTG_BLOCK_RULE);
                }
            }
        }
    }

    // Second pass: unassign if attacker is not expected to die
    card = NULL;
    while ((card = cd.nextmatch(game->inPlay, card)))
    {
        if (hints && hints->HintSaysDontBlock(observer, card))
            continue;

        if (card->defenser && opponentsToughness[card->defenser] > 0)
        {
            while (card->defenser)
            {
                observer->cardClick(card, MTGAbility::MTG_BLOCK_RULE);
            }
        }
    }

    // Third pass: intelligent blocking
    card = NULL;
    while ((card = cd.nextmatch(game->inPlay, card)))
    {
        if (hints && hints->HintSaysDontBlock(observer, card))
            continue;
        if (card->defenser)
            continue;

        MTGCardInstance* bestAttacker = NULL;
        int bestScore = -1;

        for (map<MTGCardInstance*, int>::iterator it = opponentsToughness.begin(); it != opponentsToughness.end(); ++it)
        {
            MTGCardInstance* attacker = it->first;
            if (!attacker)
                continue;

            int currentBlockers = (int)attacker->blockers.size();
            int totalAssignedDamage = 0;

            std::list<MTGCardInstance*>::iterator itb;
            for (itb = attacker->blockers.begin(); itb != attacker->blockers.end(); ++itb)
            {
                MTGCardInstance* blocker = *itb;
                if (blocker)
                    totalAssignedDamage += blocker->power;
            }

            int maxBlockers = 1;
            if (attacker->basicAbilities[Constants::MENACE]) maxBlockers = 2;
            if (attacker->basicAbilities[Constants::THREEBLOCKERS]) maxBlockers = 3;

            if (totalAssignedDamage >= attacker->toughness || currentBlockers >= maxBlockers)
                continue;

            bool canKill = (card->power >= attacker->toughness);
            bool survives = (card->toughness > attacker->power);

            // Always block if can kill, regardless of survivability or damage
            if (canKill)
            {
                int score = attacker->power * 2 + attacker->toughness;
                if (getStats() && getStats()->isInTop(attacker, 3, false))
                    score += 100;

                if (score > bestScore)
                {
                    bestScore = score;
                    bestAttacker = attacker;
                }
            }
            // Block even if can't kill, but we survive and reduce damage
            else if (survives && attacker->power < life)
            {
                int score = attacker->power;
                if (getStats() && getStats()->isInTop(attacker, 3, false))
                    score += 50;

                if (score > bestScore)
                {
                    bestScore = score;
                    bestAttacker = attacker;
                }
            }
            // Block to prevent lethal damage, even if we die
            else if (!survives && attacker->power >= life)
            {
                int score = attacker->power;
                if (getStats() && getStats()->isInTop(attacker, 3, false))
                    score += 75;

                if (score > bestScore)
                {
                    bestScore = score;
                    bestAttacker = attacker;
                }
            }
        }

        if (bestAttacker)
        {
            int requiredBlockers = 1;
            if (bestAttacker->basicAbilities[Constants::MENACE]) requiredBlockers = 2;
            if (bestAttacker->basicAbilities[Constants::THREEBLOCKERS]) requiredBlockers = 3;

            int currentBlockers = (int)bestAttacker->blockers.size();
            int currentBlockPower = 0;

            std::list<MTGCardInstance*>::iterator itb;
            for (itb = bestAttacker->blockers.begin(); itb != bestAttacker->blockers.end(); ++itb)
            {
                MTGCardInstance* blocker = *itb;
                if (blocker)
                    currentBlockPower += blocker->power;
            }

            if (currentBlockers >= requiredBlockers || currentBlockPower >= bestAttacker->toughness)
                continue;

            //Deprioritize a 0-power creature as a SECOND+ blocker: if the
            //attacker already has a blocker assigned, piling a 0-power engine on
            //adds no lethality and only loses the engine (deck62 N8). A LONE
            //0-power chump (no other blocker) stays allowed - it can still
            //absorb damage to prevent lethal.
            if (card->power == 0 && currentBlockers >= 1)
                continue;

            vector<MTGCardInstance*> extraBlockers;
            if (requiredBlockers > 1)
            {
                CardDescriptor cd2;
                cd2.init();
                cd2.setType("Creature");
                cd2.unsecureSetTapped(-1);
                MTGCardInstance* c2 = NULL;
                while ((c2 = cd2.nextmatch(game->inPlay, c2)))
                {
                    if (c2 == card || c2->defenser || (hints && hints->HintSaysDontBlock(observer, c2)))
                        continue;

                    int combinedPower = c2->power + card->power;
                    bool combinedCanKill = (combinedPower >= bestAttacker->toughness);

                    if (combinedCanKill)
                    {
                        extraBlockers.push_back(c2);
                        if ((int)extraBlockers.size() + currentBlockers + 1 >= requiredBlockers)
                            break;
                    }
                }
            }

            if (currentBlockers + (int)extraBlockers.size() + 1 >= requiredBlockers)
            {
                if (card->blockCost)
                {
                    MTGAbility* a = observer->mLayers->actionLayer()->getAbility(MTGAbility::BLOCK_COST);
                    doAbility(a, card);
                }
                observer->cardClick(card, MTGAbility::MTG_BLOCK_RULE);
                opponentsToughness[bestAttacker] -= card->power;

                for (size_t i = 0; i < extraBlockers.size(); ++i)
                {
                    MTGCardInstance* extra = extraBlockers[i];
                    if (extra->blockCost)
                    {
                        MTGAbility* a = observer->mLayers->actionLayer()->getAbility(MTGAbility::BLOCK_COST);
                        doAbility(a, extra);
                    }
                    observer->cardClick(extra, MTGAbility::MTG_BLOCK_RULE);
                    opponentsToughness[bestAttacker] -= extra->power;
                }
            }
        }
    }

    return 1;
}

void AIPlayerBaka::heuristicDamageOrder(AttackerDamaged * atk)
{
    if (!atk || !atk->card || atk->blockers.size() < 2)
        return;
    int budget = atk->card->power;
    if (budget < 1)
        return; //no damage to order
    const bool deathtouch = atk->card->has(Constants::DEATHTOUCH) || atk->card->has(Constants::PERPETUALDEATHTOUCH);

    //Damage needed to kill each blocker, per autoaffectDamage's own math
    //(MIN(damage, toughness); deathtouch kills at 1). Blockers damage
    //cannot kill go last.
    struct Candidate
    {
        DefenserDamaged * blocker;
        int cost;
    };
    vector<Candidate> killable;
    vector<DefenserDamaged *> unkillable;
    for (size_t i = 0; i < atk->blockers.size(); i++)
    {
        DefenserDamaged * b = atk->blockers[i];
        int cost = 0;
        if (b->card && !b->card->has(Constants::INDESTRUCTIBLE)
            && !b->card->protectedAgainst(atk->card))
        {
            cost = deathtouch ? 1 : b->card->toughness;
            if (cost < 1)
                cost = 1;
        }
        if (cost)
        {
            Candidate c = { b, cost };
            killable.push_back(c);
        }
        else
            unkillable.push_back(b);
    }
    //Greedy max-kill subset: cheapest kills first buy the most deaths;
    //among equal costs, kill the biggest power first.
    std::stable_sort(killable.begin(), killable.end(),
                     [](const Candidate& a, const Candidate& b)
                     {
                         if (a.cost != b.cost)
                             return a.cost < b.cost;
                         int pa = a.blocker->card ? a.blocker->card->power : 0;
                         int pb = b.blocker->card ? b.blocker->card->power : 0;
                         return pa > pb;
                     });
    vector<DefenserDamaged *> kills;
    vector<DefenserDamaged *> rest = unkillable;
    for (size_t i = 0; i < killable.size(); i++)
    {
        if (killable[i].cost <= budget)
        {
            kills.push_back(killable[i].blocker);
            budget -= killable[i].cost;
        }
        else
            rest.push_back(killable[i].blocker);
    }
    if (kills.empty())
        return; //nothing dies whatever the order - keep declaration order
    kills.insert(kills.end(), rest.begin(), rest.end());
    atk->blockers = kills;
}

int AIPlayerBaka::orderBlockers()
{
    if (ORDER == observer->combatStep && observer->currentPlayer == this)
    {
        DebugTrace("AIPLAYER: order blockers");
        GuiCombat * gc = observer->mLayers->combatLayer();
        for (size_t a = 0; a < gc->attackers.size(); a++)
        {
            AttackerDamaged * atk = gc->attackers[a];
            if (atk->card && atk->card->controller() == this)
                heuristicDamageOrder(atk);
        }
        observer->userRequestNextGamePhase();
        return 1;
    }

    return 0;
}

int AIPlayerBaka::affectCombatDamages(CombatStep step)
{
    GuiCombat * gc = observer->mLayers->combatLayer();
    for (vector<AttackerDamaged*>::iterator attacker = gc->attackers.begin(); attacker != gc->attackers.end(); ++attacker)
        gc->autoaffectDamage(*attacker, step);
    return 1;
}

//TODO: Deprecate combatDamages
int AIPlayerBaka::combatDamages()
{
    int currentGamePhase =  observer->getCurrentGamePhase();

    if (currentGamePhase == MTG_PHASE_COMBATBLOCKERS)
        return orderBlockers();

    if (currentGamePhase != MTG_PHASE_COMBATDAMAGE)
        return 0;

    return 0;

}


//
// General
//

AIStats * AIPlayerBaka::getStats()
{
    if (!stats)
    {
        char statFile[512];
        sprintf(statFile, "ai/baka/stats/%s.stats", opponent()->deckFileSmall.c_str());
        stats = NEW AIStats(this, statFile);
    }
    return stats;
}


void AIPlayerBaka::Render()
{
#ifdef RENDER_AI_STATS
    if (getStats()) getStats()->Render();
#endif
}

int AIPlayerBaka::receiveEvent(WEvent * event)
{
    if (getStats())
        return getStats()->receiveEvent(event);
    return 0;
}


AIPlayerBaka::AIPlayerBaka(GameObserver *observer, string file, string fileSmall, string avatarFile, MTGDeck * deck) :
AIPlayer(observer, file, fileSmall, deck)
{

    nextCardToPlay = NULL;
    aiForcedCandidate = NULL;
    aiForcedAlternative = false;
    stats = NULL;

    //Initialize "AIHints" system
    hints = NULL;
    comboHint = NULL;
    if (mDeck && mDeck->meta_AIHints.size())
    {
        hints = NEW AIHints(this);
        for (size_t i = 0; i <  mDeck->meta_AIHints.size(); ++i)
            hints->add(mDeck->meta_AIHints[i]);
    }


    if(avatarFile != "")
    {
        if(!loadAvatar(avatarFile, "bakaAvatar"))
        {
            avatarFile = "baka.jpg";
            loadAvatar(avatarFile, "bakaAvatar");
        }
        mAvatarName = avatarFile;
    }
    else //load a random avatar.
    {
        avatarFile = "avatar";
        char buffer[4];
        sprintf(buffer, "%i", int(observer->getRandomGenerator()->random()%200));
        avatarFile.append(buffer);
        avatarFile.append(".jpg");
        if(!loadAvatar(avatarFile, "bakaAvatar"))
        {
            avatarFile = "baka.jpg";
            loadAvatar(avatarFile, "bakaAvatar");
        }
        mAvatarName = avatarFile;
    }

    if (fileSmall == "ai_baka_eviltwin")
        mAvatar->SetHFlip(true);

    initTimer();
}

int AIPlayerBaka::Act(float dt)
{
    //an out-of-band decision (async model call) is still in flight:
    //neither act nor pass until it lands (no-op for Baka itself)
    if (decisionPending(dt))
        return 0;

    if (!(observer->currentlyActing() == this))
    {
        return 0;
    }

    int currentGamePhase = observer->getCurrentGamePhase();

    oldGamePhase = currentGamePhase;

    if (mFastTimerMode)
        timer -= dt*3;
    else 
        timer -= dt;
    if (timer > 0)
    {
        return 0;
    }
    initTimer();

    if (combatDamages())
    {
        return 0;
    }
    interruptIfICan();

    //computeActions only when i have priority
    if (!(observer->currentlyActing() == this))
    {
        DebugTrace("Cannot interrupt");
        return 0;
    }
    if (clickstream.empty())
        computeActions();
    if (clickstream.empty())
    {
        //a decision started during computeActions and is not made yet:
        //neither pass priority nor decline the interrupt
        if (decisionPending(0))
            return 0;
        if (observer->isInterrupting == this)
        {
            if(observer->mExtraPayment && observer->mExtraPayment->source->controller() == this)
            {
                ExtraManaCost * check = NULL;
                check = dynamic_cast<ExtraManaCost*>(observer->mExtraPayment->costs[0]);
                if(check)
                {
                    vector<MTGAbility*> CostToPay = canPayMana(observer->mExtraPayment->source,check->costToPay,check->source->has(Constants::ANYTYPEOFMANAABILITY));
                    if(CostToPay.size())
                    {
                        payTheManaCost(check->costToPay,check->source->has(Constants::ANYTYPEOFMANAABILITY),check->source,CostToPay);
                    }
                    else
                    {
                        observer->mExtraPayment->action->CheckUserInput(JGE_BTN_SEC);
                        observer->mExtraPayment = NULL;
                    }
                }
                return 0;
            }
            observer->mLayers->stackLayer()->cancelInterruptOffer(); //endOfInterruption();
        }
        else
        {
            if (observer->currentActionPlayer == this)//if im not the action player why would i requestnextphase?
            {
                //Human-paced blockers: while a HUMAN defender still has a
                //legal block to declare, the attacking AI must not push the
                //game past the blockers step - block clicks only react while
                //the game SITS there with a settled stack, so the AI's
                //request was silently ending combat before the human could
                //block (live-observed: pump your blocker in the attack
                //window, then never get to block with it). The defender
                //advances the phase when done. AI or blockless defenders
                //keep the engine-paced flow (pendingCombatDecision / the
                //W3b empty-block auto-skip).
                Player * defender = observer->currentPlayer == this ? opponent() : NULL;
                bool humanBlockHold = defender && !defender->isAI()
                    && observer->getCurrentGamePhase() == MTG_PHASE_COMBATBLOCKERS
                    && observer->combatStep == BLOCKERS
                    && !observer->mLayers->stackLayer()->getNext(NULL, 0, NOT_RESOLVED)
                    && LegalActionsOracle::hasLegalBlock(defender);
                if (!humanBlockHold)
                    observer->userRequestNextGamePhase();
            }
        }
    }
    else
    {
        bool completedPlay = !clickstream.empty();
        while(clickstream.size())
        {
            AIAction * action = clickstream.front();
            int acted = action->Act();
            //a refused payment click (mana producer / wrapped producer)
            //means the rest of this plan can no longer complete - clicking
            //on would float partial mana and dead-end at the cast click
            //(wave-20 deck102 stall loop). Abort the remaining plan.
            bool paymentClickRefused = !acted && action->ability
                && !action->target && !action->playerAbilityTarget
                && action->mAbilityTargets.empty()
                && (dynamic_cast<AManaProducer*>(action->ability)
                    || dynamic_cast<GenericActivatedAbility*>(action->ability));
            SAFE_DELETE(action);
            clickstream.pop();
            if (paymentClickRefused)
            {
                while(clickstream.size())
                {
                    AIAction * rest = clickstream.front();
                    SAFE_DELETE(rest);
                    clickstream.pop();
                }
            }
        }
        //Spectator pacing: the endless demo develops faster than a human can
        //read the board, so hold this seat's next act after each completed
        //play. Priority passes keep the normal quick timer, and harness
        //contexts (suite/selfplay/headless, or WAGIC_DEMO_FAST=1 for live GUI
        //engine tests) keep full engine speed.
        if (completedPlay && observer && observer->gameType() == GAME_TYPE_DEMO
            && !observer->mSuiteGame)
        {
            bool harness = false;
#if !defined (PSP)
            harness = getenv("WAGIC_DEMO_FAST") || getenv("WAGIC_TESTSUITE")
                || getenv("WAGIC_HEADLESS") || getenv("WAGIC_SELFPLAY");
#endif
            if (!harness)
                timer = 1.5f;
        }
    }
    return 1;
};

AIPlayerBaka::~AIPlayerBaka() {
    if (stats)
    {
        stats->save();
        SAFE_DELETE(stats);
    }
    SAFE_DELETE(hints);
}

//---- Pre-game (opening-hand) heuristics -------------------------------------

int AIPlayerBaka::pregameMulliganDecision(int mullsTaken)
{
    //London floor: once two mulligans have been taken a keep would already be
    //down to five cards, so stop (never mulligan below a 5-card hand).
    if (mullsTaken >= 2)
        return 0;
    int lands = 0;
    MTGGameZone * h = game->hand;
    for (int i = 0; i < h->nb_cards; i++)
        if (h->cards[i]->isLand())
            lands++;
    //A hand with too few or too many lands is worth a mulligan.
    if (lands < 2 || lands > 5)
        return 1;
    return 0;
}

MTGCardInstance * AIPlayerBaka::pregameChooseBottom(int need, int chosenSoFar, int & status)
{
    (void) need; (void) chosenSoFar;
    status = 0;
    //Bottom the highest-converted-cost card currently in hand (keep the cheap
    //spells and the lands).
    MTGGameZone * h = game->hand;
    MTGCardInstance * best = NULL;
    int bestCost = -1;
    for (int i = 0; i < h->nb_cards; i++)
    {
        MTGCardInstance * c = h->cards[i];
        int cost = c->getManaCost() ? c->getManaCost()->getConvertedCost() : 0;
        if (cost > bestCost)
        {
            bestCost = cost;
            best = c;
        }
    }
    return best;
}

int AIPlayerBaka::pregameLeylineDecision(MTGCardInstance * card)
{
    (void) card;
    return 1; //always begin the game with a leyline on the battlefield
}
