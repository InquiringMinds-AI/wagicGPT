#ifndef _DECISIONCONTRACT_H_
#define _DECISIONCONTRACT_H_

#include <string>
#include <utility>
#include <vector>

#include "LegalActions.h"

class GameObserver;
class MTGCardInstance;
class Player;
class Targetable;
class TargetChooser;

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
        //menu family (c3): a menu is up on the action layer and the seat
        //must answer. Option TEXTS are snapshotted at build time - the menu
        //owns its option objects and frees unchosen ones on answer, so a
        //request must never hand out pointers into it.
        CHOOSE_MENU,   //regular action-layer menu (may yes/no, pay/kicker, ability pick)
        CHOOSE_MODE,   //"choose one" modal (MenuAbility::abilities[])
        ANNOUNCE_X,    //X announcement (AAWhatsX; option index == X value)
        CHOOSE_TARGET, //an open TargetChooser needs its target(s)
        CAST_SPELL,    //a casting window: which legal cast (if any) to make
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
    //menu family: snapshotted option texts; the card the menu is about
    //(may be NULL); whether a decline/cancel answer is legal (CHOOSE_MENU).
    //menuIndices maps each option to its SimpleMenu item index (CHOOSE_MENU).
    std::vector<std::string> optionTexts;
    std::vector<int> menuIndices;
    MTGCardInstance * contextCard;
    bool canDecline;
    //CHOOSE_TARGET: the chooser's currently-legal candidates (players and
    //cards across all zones, capped), the source card, and the selection
    //bounds (targetMin: a minimum must be met before stopping; maxTargets
    //straight from the chooser, TargetChooser::UNLITMITED_TARGETS included)
    std::vector<Targetable*> targetCandidates;
    MTGCardInstance * sourceCard;
    int targetMin;
    int maxTargets;
    //CAST_SPELL: the oracle's legal cast set (card + zone + cost variant);
    //declining (casting nothing) is always legal at a casting window
    std::vector<LegalActionsOracle::Cast> casts;

    DecisionRequest() : player(NULL), contextCard(NULL), canDecline(false),
                        sourceCard(NULL), targetMin(0), maxTargets(0) {}
};

class DecisionAction
{
public:
    //DECLARE_ATTACKERS: the chosen attacker set
    std::vector<MTGCardInstance*> attackers;
    //DECLARE_BLOCKERS: blocker -> attacker assignments; unlisted blockers
    //stay out of combat
    std::vector<std::pair<MTGCardInstance*, MTGCardInstance*> > blocks;
    //menu family: index into optionTexts (ANNOUNCE_X: the X value itself);
    //-1 = decline (CHOOSE_MENU with canDecline)
    int choice;
    //CHOOSE_TARGET: the chosen target(s), in chosen order
    std::vector<Targetable*> targets;

    DecisionAction() : choice(-1) {}
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

    //Menu family (c3). buildMenuChoice classifies the live action-layer
    //menu into CHOOSE_MENU / CHOOSE_MODE / ANNOUNCE_X and snapshots its
    //options; false when no menu is up or it has no answerable shape.
    //applyMenuChoice re-runs the build and drops the action if the menu
    //changed since the request was issued (stale answers re-ask, never
    //misclick); a decline against a can't-cancel menu clicks the last
    //real option (the engine's own convention for that key).
    static bool buildMenuChoice(Player * p, DecisionRequest & req);
    static void applyMenuChoice(const DecisionRequest & req, const DecisionAction & act);

    //Targets (c4). buildChooseTarget enumerates the chooser's live legal
    //candidates (false when none exist); applyChooseTarget re-validates
    //every pick against the LIVE chooser (stale or illegal picks drop) and
    //performs the click choreography the AI base classes used to own:
    //source-first confirming click, player clicks, then the card batch
    //with done/autoChoice set on the final click. skipCardClick preserves
    //the historical chosenCard semantics (the card was already clicked by
    //the caller; only player targets still need a click).
    static bool buildChooseTarget(Player * p, TargetChooser * tc, DecisionRequest & req);
    static void applyChooseTarget(const DecisionRequest & req, const DecisionAction & act,
                                  bool skipCardClick = false);

    //Casts (c4, issuance half). buildCastSpell wraps the oracle's
    //legalCasts as a typed request; false when nothing is castable (the
    //only outcome is passing - no decision exists). There is deliberately
    //NO applyCastSpell yet: committing a cast means payment planning plus
    //the put-in-play clickstream, which today lives in AIPlayerBaka's
    //pricing machinery (aiForcedCandidate validation, gotPayments,
    //payAlternative). That apply moves into the manager with c5
    //(Baka-as-policy), when payment planning rides ManaEngine end to end.
    static bool buildCastSpell(Player * p, ManaEngine::ManaPolicy & policy, ManaCost * pMana,
                               bool instantSpeedOnly, DecisionRequest & req);
};

#endif
