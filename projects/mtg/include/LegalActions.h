#ifndef _LEGALACTIONS_H_
#define _LEGALACTIONS_H_

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

    //Payability across every cost variant (normal / alternative / morph /
    //flashback / retrace, plus specific-producer payment plans).
    static bool payable(Player * p, ManaEngine::ManaPolicy & policy,
                        MTGCardInstance * card, ManaCost * pMana);
};

#endif
