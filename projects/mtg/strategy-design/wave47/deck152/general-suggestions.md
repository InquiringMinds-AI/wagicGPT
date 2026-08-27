# deck152 seat - core-prompt / render proposals (wave 47)

---

## G-5 (HIGH) - the blockers screen collapses the B-list and not the A-list, and the A-list is
## what makes it big.
`146 vs123` `blockers` seq **26** is a **74,063-character** prompt - 4.4x the next largest decision
on this seat and the largest single prompt in the corpus outside the loop game. Its composition:
**198 attacker rows, of which 196 are byte-identical modulo `#N`**
(`Vampire #N (4/4) deals 4 [flying, doesn't untap during its controller's untap step] [NONE of your
available blockers can block this attacker - it has flying and none of your available blockers has
flying or reach]`), against **2 blocker rows**. Lane M's ranged collapse (`B2-B22. Vampire
#1-#21 ... x21`) fired **0 times on this seat**, and it could not have: it is on the B-list, and
this seat had two blockers. Meanwhile the battlefield line in the SAME prompt collapses correctly
(`Mountain #1-#5 x5`), which shows the mechanism exists and is simply pointed at the wrong list.
**Proposal:** mirror the identical-run collapse onto the ATTACKER rows of the blockers screen, on
the same rule the battlefield line uses (identical text after `#N` normalisation), keeping the
first and last index (`A2-A197. Vampire #1-#196 (4/4) deals 4 ... x196`). Nothing is deleted: the
count and the per-row text are both still true, and the "deals N" total the header already prints
(`up to 791`) is unaffected. Cost saved on this one decision is roughly 70 KB.
**Why the collapse fired so rarely elsewhere** (the wave-47 question): on my three seats **every
blockers window had 3 or fewer B-rows** (max B=3, at `162 vs152` seq 13). The floor of 3 is not the
binding constraint - the B-list simply never got wide, because a deck with a wide board is
attacking, not blocking. The collapse as shipped is aimed at a window this pool does not produce.

## G-6 (MED) - the mulligan price row is doing its job; the pregame plan removal is doing more.
Reported as a positive control so nobody re-opens it. R9's priced row renders exactly as designed -
`2. Mulligan (a keep after this one would keep 6 cards)` - and the pregame asks carry **no carried
PLAN block**. Corpus-wide result: **8 mulligans, every single one a single look, zero chains, zero
seats reaching (keeping 4)**, against wave 46's three-chain at this very seat and two six-chains at
deck123. Prediction "chains reaching a third look with a land in hand 0/N - was 9": **0.**
No further change wanted; the remaining mulligan defects this corpus are guide-side (see
`deck152/edit-texts.md` 152-A).

## G-7 (LOW) - `dropped_assignments` returned once, on a `Bn:none` pad.
`152 vs123` `blockers` seq 26 answered `BLOCKS: B1:A1, B2:none, B3:none` on a two-blocker screen;
the record carries `dropped_assignments: 1`. The dropped item is a no-op assignment for a blocker
that does not exist, so nothing was lost - but the counter is the instrument the engine seat uses
to detect real drops, and a `Bn:none` pad inflates it. **Proposal:** exclude `:none` assignments
from the counter (wave-46 established the convention; it is not applied here), or state the B-range
on the ask the way the A-range is stated.
