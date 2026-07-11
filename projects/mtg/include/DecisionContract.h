#ifndef _DECISIONCONTRACT_H_
#define _DECISIONCONTRACT_H_

#include <utility>
#include <vector>

class GameObserver;
class MTGCardInstance;
class Player;

/*
 * DecisionRequest / DecisionAction - the frontend-agnostic gameplay
 * contract (redesign 2.3). The engine issues a typed request carrying the
 * legal options; a consumer (LLM policy, heuristic policy, eventually the
 * human UI) answers with a typed action; the manager APPLIES the action.
 * Click choreography (defenser-cycling, attack-rule toggles) lives in the
 * manager and nowhere else - consumers deal in cards, not clicks.
 *
 * c1 scope: the combat declarations W3b made engine-issued. Casts,
 * targets, and modal choices join in later increments (see
 * strategy-design/priority-mana-ux-redesign.md 2.3.1).
 */
class DecisionRequest
{
public:
    enum Kind
    {
        DECLARE_ATTACKERS,
        DECLARE_BLOCKERS,
    };
    Kind kind;
    Player * player;
    //DECLARE_ATTACKERS: creatures that may be declared (not yet attacking)
    std::vector<MTGCardInstance*> candidates;
    //DECLARE_BLOCKERS: the declared attackers, and each available blocker
    //with the attackers it may legally block (parallel vectors)
    std::vector<MTGCardInstance*> attackers;
    std::vector<MTGCardInstance*> blockers;
    std::vector<std::vector<MTGCardInstance*> > legalPerBlocker;
};

class DecisionAction
{
public:
    //DECLARE_ATTACKERS: the chosen attacker set
    std::vector<MTGCardInstance*> attackers;
    //DECLARE_BLOCKERS: blocker -> attacker assignments; unlisted blockers
    //stay out of combat
    std::vector<std::pair<MTGCardInstance*, MTGCardInstance*> > blocks;
};

class DecisionManager
{
public:
    //Build the request for a pending combat decision (the engine's
    //pendingCombatDecision says WHEN; these say WHAT). Return false when
    //the request would be empty - nothing to declare, no decision exists.
    static bool buildDeclareAttackers(Player * p, DecisionRequest & req);
    static bool buildDeclareBlockers(Player * p, DecisionRequest & req);

    //Apply an answer. Entries are re-validated against the live board:
    //illegal or stale picks are dropped, never errors - the game must
    //stand whatever a consumer replies. Attack-COST payment (rare "pay to
    //attack" creatures) is NOT applied here yet: an AI policy pre-pays via
    //its own mana planner before applying (folds into the contract with
    //Baka-as-policy/c5, when payment planning rides ManaEngine).
    static void applyDeclareAttackers(const DecisionRequest & req, const DecisionAction & act);
    static void applyDeclareBlockers(const DecisionRequest & req, const DecisionAction & act);
};

#endif
