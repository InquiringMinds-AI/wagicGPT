# deck152 - core-prompt / render proposals (wave 50), layer-routed

G-1 (engine, MED - listed as E-3 in the seat file). **Mana payment taps dual lands for generic pips
while a basic sits idle.** `152 vs146` s6 `Paid {1}{w} for Intrepid Adversary with Deserted Beach
#1, Deserted Beach #2` with Plains untapped; same shape s9 (Aspirant) and s16 (Adversary) in the
same game; `152 vs123` s16, s24. Lane Z's prediction ("generic pips paid from a multi-colour source
while a mono source could pay: 0/N by string") reads 33/146 receipts on my three seats by a
land-only check (creature sources excluded). No strand was observed this corpus (the idle basic
was always the right colour for what came next), but the receipt line now makes the
payment order visible and it is wrong by string on a fifth of all payments.

G-2 (render, LOW). **The Lair row already carries every tag the guide needs** (`[this land is
TAPPED: animated, it still cannot attack this turn]`, `[repeat: activated this turn N times
already ...]`, `{paying this taps: ...}` on Katilda's row). Nothing to add; the guide half was
missing and is now written (152-E). Offers-by-phase for the engine seat: Lair 19 Upkeep / 13 Main 1
/ 1 Main 2 / 1 Blockers; Katilda pump 3 Upkeep / 1 Main 1 - same Upkeep skew as deck146's Hive
(see deck146 G-1).

G-3 (render, LOW). **The combat-damage-order ask is read as a block ask.** `152 vs146` s13 (header
`Combat damage order: your attacker Luminarch Aspirant (2/2) is blocked by 2 creatures ... Choose
the blocker dealt damage in position 1.`) was answered `CHOICE: 1 (Block with Elite Spellbinder:
Spider #1)`; `146 vs152` s42 the same ask was answered with a deathtouch story for a creature
without deathtouch. The choice numbers were fine both times. If the order ever matters (a
first-striker, a deathtouch attacker), a one-word `[damage ORDER, not a block]` prefix on the row
would cost nothing.

G-4 (engine, MED - E-2 in the seat file). **`- Paid` receipt missing on some activations**: `152
vs126` s25 `You used: becomes a 1/1 hydra with Lair of the Hydra` carries no `- Paid {1}{g}` line
while the six Lair activations in `152 vs125` (s17, s45, s59, s60, s73, s74) each do.
