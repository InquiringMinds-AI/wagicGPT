# deck136 (Yarok of Pranks) - wave30 skill.md (strategy-writing-skill proposals)
# For the synthesis agent. Convergence with other seats is the merge signal.

## PROPOSAL 1 (STRONG, cross-cutting): "validated-fix != landed-fix" - re-verify the fix
## batch EMPIRICALLY at the seat, per reveal PATH, before declaring a class dead.
The wave-29->30 batch listed g2 ("PUT-predicate filter; tutor reveals offer only
predicate-legal cards; empty -> no ask") as under live validation, and the brief PREDICTED
deck136's "tutor windows should be clean; the phantom-tutor class dead." Empirically at the
deck136 seat the class is NOT dead: all four Mausoleum Secrets reveals still offered
42/38/43/39 options (near-whole-library) and the silent no-op fired 2 of 4. The tell was
cheap and objective: the translog `options` INTEGER (whole-library count) and the stderr
"reveal put 1 of N" line. LESSON for the skill: a fix-batch prediction is a HYPOTHESIS to
FALSIFY at the seat, never a fact to confirm in prose. Concretely, verify tutor/reveal fixes
by (a) the offered option COUNT (does it drop to the predicate-legal set, or stay
whole-library?), AND (b) whether the tutored card actually REACHES HAND in a later
"Your hand:" line - because the seam can offer illegal options AND the model can pick a
legal one by luck, masking the bug in win-rate. Also: fixes are PATH-SCOPED - the same
corpus showed a FILTERED reveal ("1 of 5") at another seat while Mausoleum Secrets'
revealzone path stayed unfiltered; "the reveal seam is fixed" must be qualified by WHICH
reveal path.

## PROPOSAL 2 (record-read discipline): a soft adjudicated win over a THEN-WEAK debut deck
## is expected to REVERSE when that deck is later guided/tuned - do not read the reversal as
## a regression.
deck136 went 5/6 -> 4/6, and both flips (vs122, vs148) were w29 adjudicated wins over decks
that were Step-0 guideless debuts in w29. This wave deck122 is the 6/6 first-guided perfecta
(strongest in the pool) and correctly out-grinds deck136 - the guide's own "you can be
OUT-GROUND by another value deck" warning firing as designed. The skill should tell
reviewers: when decomposing a win-rate change, TAG each prior win/loss by the OPPONENT'S
pipeline stage AT THE TIME, and treat "soft adj-win vs a weak debut deck -> loss vs the same
deck once tuned" as a WASH (pool hardening), not a deck-quality change. The honest strength
signal is decision quality + correct-shape losses, not the W-L delta.

## PROPOSAL 3 (rotation doctrine sharpening): the "unexercised surface" that holds a seat
## open is RESOLVED the moment it fires correctly ONCE - then apply the exhaustion gate
## immediately, do not grant an extra "steady-state" wave.
deck136's seat was held open across w28-w29 solely by Ugin being uncast. It cast (and
correctly -3'd) this wave. The doctrine should be explicit that a single correct exercise of
the last-held surface FLIPS the seat to rotate-eligible in the SAME wave - the guide has now
demonstrably taught everything and nothing remains at the guide/skill/core layers. Engine
bugs observed at the seat (here N-136a) do NOT reset the exhaustion clock: they route to the
ledger and are validated by probe decks, which do not need a live rotation slot. Add this as
a boundary case to the "new-work exhaustion" section so reviewers don't reflexively grant
"one more validation wave" when the only live item is engine-lane.

## BOUNDARY (keep): guide-can't-fix-it class remains a hard skill boundary.
N-136a persisting confirms the w29 skill entry (some misplays are seam defects a guide
cannot fix). The guide correctly stays FROZEN - it already articulates the constraint and
the model demonstrably understands it; further guide sharpening is diminishing returns while
the seam offers illegal options. The skill's verify-oracle rail and construction categories
(terminal/ceilinged) need no change from this seat.
