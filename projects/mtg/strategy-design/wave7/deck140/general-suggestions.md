# deck140 wave-7 — core-prompt / layer-routing proposals

**Recommendation: NO core-prompt edit this wave (PASS).** Every leak found was either single-seat
(only deck140 exercised sweepers in this corpus) or deck/infra. Per the skill's epistemics, a
single seat cannot add or cut a core line; the two candidate mechanics facts below are routed
PER-DECK now and flagged as convergence WATCHES for core.

## Routed PER-DECK this wave (in strategy.txt)
- The three false-belief kills (tapped-still-threatens; artifacts-don't-shield-creatures;
  shroud-doesn't-stop-non-targeted-sweeps/Edict) — deck140-specific instantiations tied to its
  named sweepers. Kept in the guide.
- Sweeper selection by castable color (red-light hand routes to Wrath/Damnation/BSZ) — deck140
  manabase-specific.
- The countable SWEEP TRIGGER (C>=1 and life<=16-or-attacked) — reuses the core's existing 8/16-life
  threshold family per the threshold-reuse rule (I set 16 as the "under pressure" line; the 8-life
  burn-at-face core number stays distinct — flag for the synthesis agent to reconcile if a second
  deck proposes a nearby number).

## Convergence WATCHES for core (do NOT edit core on this evidence)
1. **"A TAPPED attacker is still a threat — a sorcery-speed removal/sweeper kills it the same, and
   it untaps and attacks again next turn."** This is a deck-agnostic mechanics fact and the general
   prompt already carries a mechanics-facts block (attacking taps creatures not lands; summoning
   sickness; first strike). It plausibly belongs there IF a second control/removal deck reproduces
   the "they're tapped, I'll wait" hold. deck140 is the first seat. WATCH; promote only on a second
   seat.
2. **"Shroud/hexproof does not stop effects that do not target — all-creature sweeps
   (destroy/damage all creatures) and 'sacrifice a creature' still work."** Deck-agnostic rules
   fact; single seat (deck140). WATCH; promote only if a second deck's replies show shroud-conflation
   suppressing a non-targeted spell.

Both watches are the same shape as the wave-6 reflexive-high-life-blocking watch: a mechanics fact
present-and-plausibly-core but gated on cross-deck agreement.

## Representation ask (route to the engine/representation ledger, not the prompt)
The cleanest end-state fix for Finding 1 is representation, which would make the per-deck
false-belief kills unnecessary: surface a **"creatures: N"** count on the opponent-battlefield line
(and/or mark a tapped attacker as "(tapped - untaps and attacks next turn)"). The pilot's whole
error is mis-reading the creature count off a cluttered board line studded with artifacts and
[tapped] flags; a surfaced integer is the representation that instruction is standing in for. This
is the wave-6 method headline (representation beats instruction) applied to the sweep trigger.
Filed in notes.md.
