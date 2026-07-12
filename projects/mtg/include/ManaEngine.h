#ifndef _MANAENGINE_H_
#define _MANAENGINE_H_

/*
 * ManaEngine - player-agnostic mana affordability and payment planning.
 *
 * Extracted from AIPlayerBaka (getPotentialMana / canPayMana) so that every
 * consumer of "what mana COULD this player make, and which producers should
 * be activated to pay this cost" shares one implementation:
 *   - AI players (Baka keeps thin wrappers, policy = its canHandleCost)
 *   - the human auto-tap path (policy = free producers only; never
 *     auto-activate a producer whose activation carries extra costs)
 *   - the legal-action oracle (castability = pool + potential mana)
 *
 * A ManaPolicy decides which producers may be auto-activated. The engine is
 * mechanical; willingness (e.g. choosing WHAT to sacrifice for a producer
 * with extra costs) belongs to the policy owner.
 */

#include <map>
#include <vector>

class ManaCost;
class MTGAbility;
class MTGCardInstance;
class Player;

class ManaEngine
{
public:
    class ManaPolicy
    {
    public:
        virtual ~ManaPolicy() {}
        //May this mana producer be auto-activated to pay costs?
        virtual int canHandle(MTGAbility * producer) = 0;
    };

    //Only producers whose activation cost carries NO extra costs (no
    //sacrifice/discard/etc.) - the safe default for auto-tapping on a
    //human's behalf.
    class FreeProducerPolicy : public ManaPolicy
    {
    public:
        int canHandle(MTGAbility * producer);
    };

    //Sum of everything the player's untapped producers could add to the
    //pool right now (single-mana producers, matching the historical AI
    //behavior). Caller owns the returned ManaCost.
    static ManaCost * potentialMana(Player * p, ManaPolicy & policy, MTGCardInstance * target = NULL);

    //Plan which producer abilities to activate to pay `cost`. Returns the
    //ability list in activation order; empty when the cost cannot be met.
    //Mirrors the historical AIPlayerBaka::canPayMana semantics (color-first
    //fill, hybrid handling, kicker lookahead, X = go all out).
    static std::vector<MTGAbility*> planPayment(Player * p, ManaPolicy & policy, MTGCardInstance * target,
                                                ManaCost * cost, int anytypeofmana);
    static std::vector<MTGAbility*> planPayment(Player * p, ManaPolicy & policy, MTGCardInstance * target,
                                                ManaCost * cost, int anytypeofmana,
                                                std::map<MTGCardInstance*, bool> & used, bool searchingAgain);

    //PERMISSIVE potential: every color every untapped producer could make,
    //counting EACH ability of a card (a dual land contributes both colors).
    //potentialMana's one-ability-per-card semantics can HIDE payable casts
    //(the "dual-land second-color hole") - wrong for the priority-window
    //predicate, where a missed window costs a game and overcounting the
    //total only risks a spurious auto-answered ask. Caller owns the result.
    static ManaCost * potentialManaPermissive(Player * p, ManaPolicy & policy);

    //The producers auto-tap WOULD activate to pay `cost` from the current
    //pool: producers whose single-color output pays a still-uncovered
    //COLORED symbol first, generic fillers after, stopping the moment the
    //simulated pool covers the cost - planPayment's raw list is
    //layer-ordered and can front-load wrong-color fillers, which overpaid
    //(a {1}{G} cost tapping two Mountains before the Forest). Pure: no
    //clicks. Empty when the pool already covers the cost.
    static std::vector<MTGAbility*> selectAutoTapProducers(Player * p, MTGCardInstance * target,
                                                           ManaCost * cost, int anytypeofmana);

    //Auto-tap on a human's behalf: activate selectAutoTapProducers' picks
    //until the player's POOL covers `cost`, and no further. Clicks route
    //through GameObserver::cardClick so events/animations stay identical
    //to manual taps. No-op when the pool already covers the cost.
    static void autoTapForCost(Player * p, MTGCardInstance * target, ManaCost * cost, int anytypeofmana);
};

#endif
