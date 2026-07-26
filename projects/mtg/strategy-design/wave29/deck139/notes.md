# Engine / representation / harness items -- deck139 (mutate) wave-29
# All card facts verified against primitives (bin/Res/sets/primitives/borderline.txt) + Scryfall.
# Step-0 seat: items are FIRST-SIGHTING of the mutate surface. Step-0 never rotates.

## N-139a -- mutate multi-ask FLOW ORDER (representation)
One mutate cast emits up to five sequential asks; the OVER/UNDER choice is asked BEFORE the
mutate TARGET, and a triggered-ability sub-choice (Pollywog's loot) is interleaved in the
middle. Repro deck93 (1785093513-...-deck93.jsonl): s19 normal/mutate -> s20 loot-discard ->
s21 over/under -> s22 mutate-target -> s23 land fetch. Repro without Pollywog deck146
(1785087615-...-deck146.jsonl): s16 normal/mutate -> s17 over/under -> s18 mutate-target.
Each ask re-sends the full board. More natural order: pick the mutate TARGET first, then
over/under (over/under is only meaningful relative to a known host). Cost: multiplies latency
and truncation exposure on an already-expensive decision class. Representation-lane.

## N-139b -- draw-then-discard (loot) MIS-RENDERED as "TARGET CHOICE" (representation; perception leak)
Pollywog Symbiote's `@movedTo(creature[mutate]|mystack):_LOOT_` (macro `_LOOT_` = draw:1 then
transforms(...reject) = discard 1) renders as:
  "TARGET CHOICE for Pollywog Symbiote (this spell/ability is already on the stack and needs a
   target ...). Pick the ONE target it will affect ..."
with the player's HAND as options and NO "discard" verb. Model MISREAD it as the mutate target
(deck93 s20, discarded Island by accident, 194,660 ms / 14,181-char reply) and DECODED it
correctly only by inference in another game (deck122 s18). Fix: label draw-then-discard (and any
sacrifice/exile-from-own-hand) with the VERB -- "Choose a card to DISCARD" -- not "target it
will affect." Per the wave-28 perception-vs-strategy rung, this dies to the surface annotation,
not to guide prose (guide carries only a stopgap trap-warning). Repro pair: deck93 s20 (missed)
vs deck122 s18 (decoded).

## N-139c -- mutate PILE rendered as two battlefield lines; combined abilities hidden (representation)
After a mutate resolves, the "Your battlefield" listing shows the pile as two adjacent lines:
top card with P/T + its own keywords, under card as a BARE name + mana cost (no P/T, no tag).
Header creature-count is CORRECT (pile = 1). Combat A-lines render it CORRECTLY as one creature.
Repro deck146 s19+: "... Gemrazer {2}{g} (4/4) [trample, reach, mutate]; Dryad of the Ilysian
Grove {3}{g}" = ONE creature (count says 3, and both were 2 pre-merge Grazers + this pile).
Two sub-gaps: (i) ambiguous "is this one creature or two?"; (ii) the merged creature's UNDER-card
abilities (Dryad's ramp/omnilands, a host's static ability) are shown NOWHERE the model reads --
only the top card's keywords render, so the model cannot see the merged creature's full ability
set. Fix: group/tag the pile ([under Gemrazer]) and render the merged keyword+ability union on
the top line. Representation-lane.

## N-139d -- inconsistent mutate cost LABEL (representation, minor)
Same `other={..} name(Mutate)` mechanic renders two labels: Gemrazer "with its mutate cost"
(deck146 s15, deck136 s22); Migratory Greathorn "with its alternative cost" (deck122 s16,
deck93 s18). Should both be "mutate cost". The "alternative cost" label kept the model from
recognizing the mutate line up front (deck122 s17 detoured into mana math). Low effort fix.

## N-139e -- Migratory Greathorn land-search is mutate-ONLY: CORRECT, do not "fix" (verify closed)
Oracle (Scryfall) and the primitive both scope the basic-land search to "Whenever this creature
MUTATES" (primitive has `@mutated(this)` / `@mutated(mytgt)` only, NO `@movedTo(this|battlefield)`
ETB). A normal cast correctly ramps NOTHING. Recording so a future reviewer does not re-open it
as a "missing ETB" bug. (Model memory that it triggers "on enter or mutate" is WRONG.)

## N-139g -- OPEN engine-verify: do UNDER-card static abilities function on the merged creature?
CR 725 gives the merged creature ALL abilities of all cards in the pile. Not observable this
corpus (no game reached a state that exercised a buried static ability, e.g. whether Dryad's
extra-land-drop still worked while under Gemrazer, deck146). The move-together `@movedTo(mytgt|
zone):all(this) moveTo(...)` autos keep the pile intact through zone changes (verified: pile
counted as 1 through combat and death). Whether buried STATIC abilities apply is UNVERIFIED ->
route as an engine-correctness probe for a future corpus, do not assert either way.

## N-139-f6 -- ATTRIBUTION: the corpus's single truncated_abandoned is deck148's, not deck139's
Brief f6 asked whoever owns the one blockers/truncated_abandoned record to decompose it. It is
NOT this seat. Located: game-139v148 stderr "AIPlayerGPT: truncated-abandoned block commit ->
safe no-blocks default"; the truncated reply is deck148's SEQ31 (T12, 12,676 chars, ends
mid-sentence "...If the blocking creature is", choice -> no blockers). deck139's own blockers in
that game (SEQ10/17/20/25) all terminated cleanly with a PLAN line. Ownership -> deck148 seat.
Note for that reviewer: the deck139 seat came within one char-class of the SAME cliff
(deck93 s20 = 14,181 chars, parsed) -- the truncation cap is being pressured by mutate/combat
rules-lawyering at more than one seat, so the f6 safety and the latency tail (N-139 findings s5)
are the same underlying pressure, not two unrelated events.

## Steady state (f-batch) at this seat
- 0 fallbacks / 0 retries / 0 unparsed / 0 truncation-abandons in 155 decisions.
- f3 summoning-sick tag: rendered on the merged creature ("[summoning sick - cannot attack this
  turn]" the turn it mutated; "[tapped - untaps and can attack next turn]" after; correct).
- f2 dual/flex mana render: the model briefly misread "Mana available: {g}{u}{r}{b}{w} from 1
  untapped source" as 5 available mana (deck122 s17) before self-correcting to "types not count."
  Shared f2 surface, not mutate-specific; flagged for the f2 owner as a residual comprehension
  wobble on the color-reach line, low severity.
