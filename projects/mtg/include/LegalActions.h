#ifndef _LEGALACTIONS_H_
#define _LEGALACTIONS_H_

#include <set>
#include <string>
#include <vector>

#include "ManaEngine.h"

class ManaCost;
class MTGCardInstance;
class Player;

/*
 * LegalActionsOracle - the single authority for "what can this player
 * legally do right now".
 *
 * First increment: the CAST action set (rules-legal + affordable spells),
 * promoted out of AIPlayerGPT so AI policies, the human castability
 * display, and the coming priority engine all read one computation.
 * Grows toward the full decision surface (land drops, ability activations,
 * attacker/blocker sets, PassPriority) as the redesign lands - see
 * strategy-design/priority-mana-ux-redesign.md.
 */
class LegalActionsOracle
{
public:
    struct Cast
    {
        MTGCardInstance * card;
        std::string zoneLabel;  //"" (hand), " [from your graveyard]", ...
        bool viaAlternative;    //this entry casts via the alternative cost
        bool normalPayable;     //the normal cost is affordable
    };

    //Enumerate every rules-legal, affordable cast for p right now: zone
    //gates (hand/graveyard/exile/command), the legendary rule, play
    //restrictions, affordability against pMana (pool + potential mana, with
    //specific-producer fallback plans via ManaEngine), and the 601.2c
    //target-validity filter (a spell REQUIRING targets with none available
    //is not castable). instantSpeedOnly restricts to instant-speed casts
    //(the priority-window filter). pMana is borrowed, not owned. A card
    //affordable both normally and via its alternative cost yields TWO
    //entries (they are different plays).
    static std::vector<Cast> legalCasts(Player * p, ManaEngine::ManaPolicy & policy,
                                        ManaCost * pMana, bool instantSpeedOnly);

    //Every land p could put onto the battlefield right now: zone gates
    //(hand always; graveyard/exile/command need the matching playfrom
    //ability), the legendary rule, and play restrictions against the
    //battlefield (which carry the lands-per-turn count). Lands are not
    //casts, so legalCasts excludes them; this is their enumeration.
    static std::vector<Cast> legalLandPlays(Player * p);

    //Payability across every cost variant (normal / alternative / morph /
    //flashback / retrace, plus specific-producer payment plans).
    static bool payable(Player * p, ManaEngine::ManaPolicy & policy,
                        MTGCardInstance * card, ManaCost * pMana);

    //TRUE when p could actually RESPOND at a priority window right now: an
    //instant-speed legal cast, or a usable non-mana activated ability.
    //Pure (no state mutation). Deliberately PERMISSIVE on abilities (extra
    //costs are not target-validated): a spurious window costs one
    //auto-answered decision; a missed window costs a game. This is the
    //auto-pass predicate of the priority engine.
    static bool hasInstantResponse(Player * p);

    //The castability display set: every card in p's hand a click could
    //legally play right now (spells via legalCasts; lands when a land drop
    //is available at sorcery speed). PURE - unlike the rules layer's
    //isReactingToClick walk, which mutates (Leyline auto-resolution) and is
    //not safe to probe from rendering code.
    static std::set<MTGCardInstance*> castableForDisplay(Player * p);

    //Combat declaration sets (W3b). Pure predicates the engine consults to
    //decide whether a declare-attackers / declare-blockers decision even
    //EXISTS for a player - an empty set means the step auto-advances
    //instead of asking anyone.
    //hasLegalAttacker: any creature of p's that canAttack() right now.
    static bool hasLegalAttacker(Player * p);
    //hasLegalBlock: any creature of the defender's that canBlock() AND can
    //legally block at least one currently-declared attacker. Only
    //meaningful during the blockers step (attackers already declared).
    static bool hasLegalBlock(Player * defender);

    //W43-1 (CR 509.1c): the SET-level half of block legality. Returns the first
    //attacker whose current declaration is under-filled (menace blocked by one,
    //"three or more" blocked by two), or NULL when the declaration as a whole is
    //legal. This is the predicate the declare-blockers step is ACCEPTED against
    //- pairwise canBlock() cannot express it, because the first blocker of a
    //legal pair is individually legal. Consumers: the human's refused phase
    //advance, AIPlayerBaka's end-of-declaration sweep, AIPlayerGPT's reply
    //validator, and the rules-layer safety net that must now never fire.
    static MTGCardInstance * illegalBlockDeclaration(Player * defender);

    //hasAnyLegalAction: can p do ANYTHING at all in the current phase? This
    //is the predicate behind "a window where nothing is possible is not a
    //window" - the engine skips such a stop regardless of the player's
    //ASPHASES posture, because there is nothing there to decide.
    //
    //Phase-aware, because "an action" is not one thing: at a sorcery-speed
    //window of your own turn it includes lands, sorcery-speed casts and
    //sorcery-only abilities; in combat it includes declaring attackers or
    //blockers; everywhere else it is an instant-speed response.
    //
    //DELIBERATELY PERMISSIVE, and the asymmetry is the reason: a false "no
    //action" skips a window the player needed and can lose them the game,
    //while a false "has an action" costs one keypress. When in doubt this
    //answers true.
    static bool hasAnyLegalAction(Player * p);

    //PER-CARD display predicates. The set-level predicates above answer "does
    //a decision exist"; these answer "is THIS card one of the reasons", which
    //is what a border on a single card has to mean. Consumed by
    //GuiHandSelf::Update to refresh the card's display-only flags.
    //
    //Unlike the permissive set predicates, these must be EXACT: a border that
    //promises an action the engine then refuses is worse than no border, so
    //canDeclareAttacker mirrors MTGAttackRule::isReactingToClick rather than
    //approximating it. If that rule changes, this must follow.
    static bool canDeclareAttacker(MTGCardInstance * card);
    //hasUsableAbility: this permanent has an activated ability its controller
    //could legally use AND afford right now. Making mana does not count - a
    //land is not "doing something" in the sense a player cares to be shown.
    static bool hasUsableAbility(MTGCardInstance * card);
    //canDeclareBlocker: defending player's creature that can block AND has
    //at least one attacker it could legally block, in the declare-blockers
    //window. Mirrors MTGBlockRule::isReactingToClick plus the has-a-target
    //requirement. Display-only consumer, like the two above.
    static bool canDeclareBlocker(MTGCardInstance * card);
};

#endif
