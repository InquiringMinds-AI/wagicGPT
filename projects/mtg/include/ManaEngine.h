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
#include <string>

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

    //SPEND-RESTRICTED MANA (CR 106.6b). May `producer`'s mana pay for
    //`payee`? True for every unrestricted producer. A producer carrying a
    //`manarestriction{<tc spec>}` clause ("Spend this mana only to cast
    //creature spells") answers true only when payee is a CARD BEING CAST
    //(not a battlefield permanent, i.e. not an ability activation) that
    //matches the spec. A NULL payee is refused: the restricted mana is real,
    //but it is not spendable on an unknown cost.
    static bool spendAllowed(MTGAbility * producer, MTGCardInstance * payee);

    //Sum of everything the player's untapped producers could add to the
    //pool right now (single-mana producers, matching the historical AI
    //behavior). Caller owns the returned ManaCost.
    static ManaCost * potentialMana(Player * p, ManaPolicy & policy, MTGCardInstance * target = NULL);

    //Plan which producer abilities to activate to pay `cost`. Returns the
    //ability list in activation order; empty when the cost cannot be met.
    //Mirrors the historical AIPlayerBaka::canPayMana semantics (color-first
    //fill, hybrid handling, kicker lookahead, X = go all out).
    //#W52-L (D13, planner half): a LAND that is a creature right now but was
    //not printed one - a `becomes` animation (Mutavault, Lair of the Hydra,
    //Hive of the Eye Tyrant) paid for THIS turn. The attacker-sparing walk
    //draws would-be attackers into a mana bill weakest first; an animated
    //land is the last of them whatever its power, because its body is the
    //thing the seat just paid mana for (corpus 20260827 deck152 vs125
    //seq36->39: Lair animated at Upkeep, tapped for the Main-1 cast). A
    //printed land creature (Dryad Arbor) is NOT animated and keeps its
    //power-ordered place.
    static bool isAnimatedLand(MTGCardInstance * c);

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

    //Render-facing color reach: the SET of colors the player's untapped
    //sources can produce (a dual source contributes EACH of its colors) plus
    //the count of distinct untapped SOURCE cards. potentialMana counts one
    //ability per card, so a dual land (Snarl: add{B}/add{W}) collapsed to a
    //single color in the "Mana available" line and made the model believe a
    //second-color pip unpayable (N-146d). This shows the true options without
    //over-reporting the ability count as total mana. `outColors` (may be NULL)
    //gets each producible color set to 1; returns the distinct source count.
    static int potentialColorReach(Player * p, ManaPolicy & policy, ManaCost * outColors);

    //The producers auto-tap WOULD activate to pay `cost` from the current
    //pool: producers whose single-color output pays a still-uncovered
    //COLORED symbol first, generic fillers after, stopping the moment the
    //simulated pool covers the cost - planPayment's raw list is
    //layer-ordered and can front-load wrong-color fillers, which overpaid
    //(a {1}{G} cost tapping two Mountains before the Forest). Pure: no
    //clicks. Empty when the pool already covers the cost.
    //`preserveOptions` (#W55-OPT, default on): after the plan is built, swap
    //it for a same-size set of sources that leaves the player MORE options
    //(castable cards, payable activations, attackers, colours, held utility
    //lands) - see refineForOptions. The AI seat's own payment runs planPayment
    //and does not refine, so its forecasts pass false to stay honest.
    static std::vector<MTGAbility*> selectAutoTapProducers(Player * p, MTGCardInstance * target,
                                                           ManaCost * cost, int anytypeofmana,
                                                           bool preserveOptions = true);

    //#W55-OPT: the option-preserving tap search. `baseline` is a payment plan
    //(one bare single-mana producer per source, as selectAutoTapProducers
    //yields); the result taps the same NUMBER of sources and pays the same
    //cost, chosen to leave the most options open, or `baseline` unchanged
    //when nothing beats it strictly or the board is outside the search's
    //model (variable producers, X, >14 candidate sources). Pure: no clicks.
    static std::vector<MTGAbility*> refineForOptions(Player * p, MTGCardInstance * target, ManaCost * cost,
                                                     int anytypeofmana, const std::vector<MTGAbility*> & baseline);

    //Auto-tap on a human's behalf: activate selectAutoTapProducers' picks
    //until the player's POOL covers `cost`, and no further. Clicks route
    //through GameObserver::cardClick so events/animations stay identical
    //to manual taps. No-op when the pool already covers the cost.
    static void autoTapForCost(Player * p, MTGCardInstance * target, ManaCost * cost, int anytypeofmana);

    //Largest X the player could announce for an X-bearing cost (CR 601.2b),
    //judged permissively against pool + every untapped producer (ANY
    //producer - overstating only risks an unpayable announcement, which the
    //payment machinery releases). Specific-X costs ({X:red}) are bounded by
    //that color's availability. Capped at 20, the engine's own X-menu bound.
    static int maxAnnounceableX(Player * p, ManaCost * baseWithX, int anytypeofmana);

    //Self-harming mana sources (R-PAINLAND, wave-21 deck102). A source that
    //deals damage to its controller when tapped for mana (Ancient Tomb: 2,
    //painlands: 1 per colored tap) is priced NOWHERE on the decision surface,
    //and a pilot self-killed paying life through a source it did not know
    //about (vs27: cast through Ancient Tomb at 3 life, died at -1). These
    //surface the life the auto-tap can spend.
    //
    // selfDamageOnTap: the largest damage `card` would deal to ITS CONTROLLER
    //   for one usable mana tap right now, 0 if none/none currently usable.
    //   For the per-permanent battlefield line.
    // selfDamageManaSources: "<Name>: <N> damage" fragments (de-duped by card
    //   name) for every usable self-harming source player `p` controls, for
    //   the AI "Mana available" caution line.
    // Both gate on the SAME producerUsable check as potentialMana's counting
    //   (wave-21 disabled/tapped exclusion) so a spent or disabled painland is
    //   not reported. Only constant damage amounts are evaluated (a "rand"
    //   rider is never evaluated - it would draw the game RNG).
    static int selfDamageOnTap(MTGCardInstance * card);
    static std::vector<std::string> selfDamageManaSources(Player * p);
    // producerSelfDamageOf: the self-damage rider of ONE producer ability (as
    //   selectAutoTapProducers picks them), so a payment PLAN can be priced in
    //   life per pick: a Talisman tapped for {1} costs nothing, tapped for {R}
    //   costs 1. 0 for a non-producer or a rider that hits someone else.
    static int producerSelfDamageOf(MTGAbility * producer);

    //N-166k (wave-34 audit, b1 F-10 / b2 R1 / b3 F1 / b4 F5): one usable mana
    //SOURCE, as the render needs to name it. potentialColorReach returns a bare
    //count and a colour set, and an unexplained colour is a colour the model
    //INVENTS a mechanism for ("why {B}? Ah, the opponent has Urborg") - while a
    //creature mana source it cannot see sent one trace on a 33k-char hunt for a
    //fourth land. `variable` marks a producer whose output is a per-something
    //count (Tolarian Academy's "add {U} for each artifact you control"): it is
    //ONE source but not one mana, and the flat count silently understated it.
    class ManaSourceView
    {
    public:
        MTGCardInstance * card;
        std::string colors;  //what this source alone can add, e.g. "{u}"
        bool variable;       //output scales with a count; more than one mana
        ManaSourceView() : card(0), variable(false) {}
    };

    //potentialColorReach, plus the per-source breakdown. `outSources` (may be
    //NULL) receives one entry per distinct usable source card, in action-layer
    //order. Variable (foreach-wrapped) producers are counted here - the plain
    //dynamic_cast in the 3-argument form misses them entirely, so an untapped
    //Academy was invisible to the count as well as unnamed.
    static int potentialColorReach(Player * p, ManaPolicy & policy, ManaCost * outColors,
                                   std::vector<ManaSourceView> * outSources);
};

#endif
