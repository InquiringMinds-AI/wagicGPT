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
};

#endif
