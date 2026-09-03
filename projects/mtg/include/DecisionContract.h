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
    //#W48 D6: the menu subject's NAME as the action layer captured it when the
    //menu armed. contextCard is a POINTER and every consumer validates it
    //against the game's zones before dereferencing (the dangle rail); a menu
    //armed by a resolving spell's `choice` payload is armed on a card that is
    //in no zone, so the pointer is correctly refused and the name went with it.
    //This string is snapshotted while the object is provably alive, so it names
    //the subject when no pointer can. Empty when the object had no name.
    std::string contextName;
    //#W55-D (D22): CHOOSE_MODE is also how a card-NAME choice arrives - the
    //engine builds `chooseaname`'s list of names as a MenuAbility of
    //AASetNameChosen options, indistinguishable at this seam from a modal
    //"choose one". Nine corpus records rendered Silverquill Silencer's name
    //list under the header "Choose one mode for Silverquill Silencer:" over
    //bare card names. True of the menu OBJECT, false of the decision: the rows
    //are card names, not modes. The kind stays CHOOSE_MODE (the apply path is
    //identical); this flag says what the rows MEAN so the consumer can name
    //the choice correctly.
    bool nameChoiceMenu;
    bool canDecline;
    //CHOOSE_MENU, triggered/activated "may"-ability ask (the all(trigger[to])
    //moveto class - Tergrid's steal): the card-script option label is a
    //static, SUBJECT-LESS string ("Put onto battlefield under your control")
    //and contextCard is the TRIGGER SOURCE, so with no object named the model
    //reads the verb as acting on the source itself and declines a free effect
    //(w26 deck199 re-probe: threw away a stolen Bog Wraith). When the option's
    //underlying ability has a single trigger-supplied target object, its NAME
    //and ORIGIN zone are recovered here so the seat can name it. Both empty
    //for every other menu (ETB pay/tap, transform, mode, generic) and for
    //may-asks whose object is not resolvable at ask time - those keep their
    //current text. Snapshotted strings, never a live pointer.
    std::string mayObjectName;    //e.g. "Bog Wraith"
    std::string mayObjectOrigin;  //e.g. "opponent's graveyard"
    //CHOOSE_MENU, single-option "may" ask (#W41-14 label truth): the engine's
    //own option label for a may-gain-life trigger is the bare verb "Life" - no
    //magnitude, no source, and the SAME string for any life-granting may. The
    //deciding fact must ride the option (P1/P2/P4), so the magnitude is
    //evaluated here at build time ("Gain 2 life") and the seat renders it.
    //Empty whenever the effect's magnitude is not safely derivable - then the
    //seat keeps the engine label untouched.
    std::string mayEffectLabel;   //e.g. "Gain 2 life"
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

    DecisionRequest() : player(NULL), contextCard(NULL), nameChoiceMenu(false), canDecline(false),
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

    //Mass accept/deny for repeated identical "may" triggers (#W41-7).
    //
    //One `may` ability can fire N times inside a single resolution window -
    //Perimeter Captain's "whenever a creature you control with defender
    //blocks, you may gain 2 life" raised EIGHT identical asks in one combat
    //(wave-40 seat126: 22 asks, 22 accepts, 1,377s of inference, 132k
    //reasoning characters for 22 identical answers). The engine pushes every
    //sibling trigger onto the stack BEFORE the first one resolves, so at the
    //FIRST ask the size of the whole group is already knowable: this
    //inspector counts the still-unresolved siblings so a seat can ask once
    //instead of N times.
    //
    //"Identical" is deliberately narrow, and every condition must be
    //POSITIVELY verified or the group is not offered as batchable:
    //  - the live menu is a single-option may with a legal decline;
    //  - the option's ability is a MayAbility with no target chooser (no
    //    per-instance targets to choose) and no cast-restriction condition
    //    (which could evaluate differently per instance);
    //  - each counted sibling is an unresolved StackAbility whose nested
    //    ability is a MayAbility with the SAME source card, the SAME
    //    controller, the SAME rendered menu text, and likewise no targets.
    //Anything unverifiable simply does not join the batch and keeps its own
    //ask. The batch is an AFFORDANCE, never a replacement for the `may`:
    //a consumer offering it must always keep a "decide individually" answer,
    //so the batched question can never make an answer impossible.
    struct MayBatch
    {
        bool batchable;        //true only when totalInWindow >= 2 and all checks passed
        int pendingSiblings;   //unresolved identical triggers still on the stack
        int totalInWindow;     //pendingSiblings + this ask
        std::string groupKey;  //stable within one window; changes with source/effect/turn
        std::string sourceName;
        MayBatch() : batchable(false), pendingSiblings(0), totalInWindow(1) {}
    };
    static bool inspectMayBatch(Player * p, const DecisionRequest & req, MayBatch & out);

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

    //c5a: validate and plan the commit of a chosen cast (act.choice indexes
    //req.casts) for the PLAIN case only - hand zone, normal cost, no extra
    //costs / X / kicker. Re-validates against a fresh build (the same
    //card+variant must still be offered) and plans payment via ManaEngine
    //with the caller's policy. Fills `producers` with the producer
    //abilities to activate, in order, before clicking the card. Returns
    //false when the cast is outside the plain case or no longer legal -
    //the caller keeps its legacy commit path for those.
    //DELIBERATELY NO CLICKS HERE: the engine's ability-GC sweep has a
    //latent double-destroy (ALord::removed -> removeObserver cascade,
    //SIGSEGV observed 2026-07-11) that a synchronous click burst can
    //trigger by changing which abilities co-die in one ActionLayer::Update
    //sweep. Cast commits must keep the AI clickstream's one-click-per-tick
    //cadence until that GC bug is fixed; the consumer enqueues the clicks.
    static bool planCastSpell(const DecisionRequest & req, const DecisionAction & act,
                              ManaEngine::ManaPolicy & policy,
                              std::vector<MTGAbility*> & producers);
};

#endif
