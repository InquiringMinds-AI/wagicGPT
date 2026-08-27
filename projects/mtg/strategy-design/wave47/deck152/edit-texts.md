# deck152 - wave-47 edits (2), before -> after, each with its finding

Baseline: the LIVE guide `bin/Res/ai/baka/deck152_strategy.txt` (35,941 bytes = wave-46 reviewer
revision + boundary-pass C1/C2/C4). After: 37,060 bytes (**+1,119**). 0 deletions.

---

## 152-A (HIGH) - RULE -1's M=6 branch kept a one-land six and produced a game with ZERO casts
**Finding:** `152 vs130` seq **1-3**. The opening seven was 1 land / 6 spells with the coverage
line `Playing every land in this hand would not cover any spell in it` - shipped, correctly
(MULLIGAN section's ONE LAND branch). The second look was **1 land / 6 spells again**, same
coverage line, cheapest spell `Luminarch Aspirant {1}{w}`, with `Tovolar's Huntmaster {4}{g}{g}`
and `Sigarda {1}{g}{w}{w}` in it. The price row read `Mulligan (a keep after this one would keep
5 cards)`, i.e. **M = 6**, and RULE -1's M=6 branch is `keep unless the hand is zero lands or all
lands` - so **the guide mandated this keep**, and the deeper MULLIGAN section's correct one-land
test never got read (RULE -1 says "you are DONE; do not read another rule anywhere in this file").
Cost: land drops at turns 0, 10, 12, 14, 22, 24, 28, 32 and **not one cast spell in thirty-three
turns**. Final: **-7 to 17**. It is the only game in this corpus with no cast in it.
CLASSIFICATION: **STRATEGY** - the render printed the land count, the coverage line and the price
row; the guide's own branch overrode all three.
- **before:** `  - M is 6: keep unless the hand is zero lands or all lands.`
- **after:** `  - M is 6: keep unless the hand is zero lands, all lands, or ONE land under the
  header line "Playing every land in this hand would not cover any spell in it". That coverage
  line is the engine's own arithmetic and it is the whole test at M=6 ... If the header instead
  reads "would cover the cost of:" and names a card, KEEP.` plus the WHAT THIS COST record.
- **Deliberately narrow.** M=5 and lower are untouched (the floor stands; skill 66). M=6 with two
  or more lands is untouched. The new clause fires only on a one-land six whose own header says
  it covers nothing - an observable already printed on every pregame ask.

## 152-B (MED) - the carried plan overrode the per-creature STOP
**Finding:** `152 vs146` seq **18** (turn 9, life 26 vs 20). Both offered attackers read
`[their untapped blockers: Nadaar, Selfless Paladin (3/3) (your attacker dies, their blocker
lives); Goblin (1/1) (both die)]`. `Lair of the Hydra` was a 1/1 (`auto={1}{G}:name(Becomes a 1/1
hydra)`, verified) and `Katilda, Dawnhart Prime` is a printed 1/1 - both **non-token CARDS**, the
lethal check unmet (opponent at 20, total power sent 2). The prompt's carried block read
`YOUR PLAN (as you last stated it): Cast Tovolar's Huntmaster. Attack with Hydra and Katilda.`
and the reply is that plan re-stated verbatim. The plan predates the tag. Both were sent; both
were stopped. These are the only two STOP violations on this seat this corpus (see the seat file's
cross-deck table: 2 sends of 3 card-windows, and the third - `vs146` seq 36's Luminarch Aspirant -
was correctly HELD while three Wolf TOKENS on the identical tag were sent, which is the rule
working exactly as written).
CLASSIFICATION: **STRATEGY**. RULE -1 already carries "THE CARRIED PLAN IS NOT EVIDENCE ABOUT THIS
HAND" for the mulligan ask; this generalises the same refusal to the attackers ask, where the
corpus now shows the same mechanism.
- **before:** the STOP block ends with `VIGILANCE IS NOT A LICENCE EITHER.`
- **after:** a new paragraph immediately above it - `YOUR CARRIED PLAN IS NOT A LICENCE EITHER ...
  the A-lines are computed fresh for this combat; the plan is not. When the plan names an attacker
  whose A-line now reads "your attacker dies, their blocker lives", the plan is out of date - say
  so in one clause and leave that creature home.`

---

## NOT edited, and why
- **#3 RULE DEPLOY FLOOR: 44/45**, and the single miss (`vs146` seq 39, opponent at **2**, lethal
  on board, won that turn) is exception (b) written into the rule itself. Effectively **45/45**,
  third consecutive corpus. Nothing to fix. The 26 `Cast nothing right now` answers on this seat
  are almost all Fateful-Absence-only menus (exception (a)) or menus with no creature on them -
  I checked each before writing this line.
- **Fateful Absence `the only legal targets are YOUR OWN` -> do not cast: 6/6** (`vs125` seq
  93/94/95/97/98/99). PASS.
- **Block floor (wave-46 edit 152-B): PASS 0/4.** No `BLOCKS: none` at a header "you would be at
  N" of 9 or less. The one `BLOCKS: none` (`vs146` seq 38) is at N=15 with the only offered block
  reading `your blocker dies, attacker lives` - the rung that says never. The prediction's
  SECONDARY ("every blocks reply names the value of N") is **0/4** - the model never quotes it -
  which is a lesson about the observable, not about the rule; see `skill.md`.
- **Counter spreading (wave-46 edit 152-D): 2 breaks of 13**, both a counter onto a Sigarda whose
  own option row already printed `(6/6)` while five other creatures were listed (`vs126` seq 39,
  `vs146` seq 35). Both games were **won**, Sigarda is the flier that closed them, and the rule is
  a heuristic about spreading, not an arithmetic. Per amendments 45/64 a rule violated-and-
  unpunished at n=2 is neither loosened nor re-worded: **measured, carried to wave 48.**
- **Land drops 16/16** (was 51/56). Wave-46 edit 152-E holds; so does lane L's R13. Not re-touched.
