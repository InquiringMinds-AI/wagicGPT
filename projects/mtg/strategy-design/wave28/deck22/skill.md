# deck22 (Giants!) skill proposals - wave-28 (2nd validation + rotation)

## Headline for the skill: a representation<->parser coupling fix VALIDATED and GENERALIZED

Wave-27 surfaced the first case of a representation lever inducing a parser fallback: an
in-band render annotation (`[changeling: counts as Giant]`) echoed by the model into its
`CHOICE:` line, tripping the c4 name-anchored answer check (1/146, benign). Wave-28 is the
clean validation of the e5 fix AND a generalization signal worth a standing skill line:

- The model echoes annotations MORE once they are useful to it (5 echoes this corpus vs 1 in
  wave-27) - confirming the wave-27 prediction that "an in-band annotation WILL be echoed."
  Annotation-echo is not a rare accident; it is the expected behavior of a read annotation.
- The e5 tolerance parsed a SHAPE IT WAS NOT MOTIVATED BY: vs136 s11 echoed `[land] [your
  hand]` (a different annotation family than `[changeling...]`) and it parsed clean. This is
  the payoff of fixing the parser at the STRUCTURAL level (strip/allow bracketed tails) rather
  than special-casing the one shape that produced the bug.
- STANDING LESSON (promote/confirm): when you add ANY in-band render annotation as a
  representation lever, ship it WITH an annotation-tolerant answer parser - match on choice
  NUMBER + card-name PREFIX and tolerate trailing `[...]` in echoed names. Verify the
  tolerance against a DIFFERENT annotation shape than the one you built it for; the fix is
  only proven when it survives a shape it was not tuned to.

## The KEYWORD-TYPE-CONSEQUENCE face: stays SOLVED, stays single-seat (no change)

Still one seat. deck93/134/136/137/146 have no changeling / type-matters payoff, so no second
deck reproduced the inference gap. The face remains "SOLVED via double-lever (in-band render
annotation + guide rule-#1 identity line, carried cross-phase to mulligan)" - the corrective
template is validated, not hypothesized. Keep documented and single-seat-tagged pending a
second changeling/type-devotion/type-restricted deck.

## CONFIRMS the rotation doctrine's UNSENTIMENTAL test (this seat is the clean witness)

deck22 is a textbook application of "the record drop alone does not hold a seat - only a
guide-addressable decision class does." The record fell 5/6 -> 3/6, which a sentimental read
would treat as regression demanding guide work. Decomposed honestly, BOTH losses are
opponent-strengthening (deck93 +2/6; deck137's payoffs revived by the e1 convoke fix) plus
draw variance, with every deck22 decision correct. There is NO new guide-addressable class.
The two model deviations (value-positive Mogg attack; correct all-Stinkdrinker mulligan) are
handled correctly WITHOUT guide change - non-binding, so editing them would only reset the
freeze clock. Rotation is therefore correct DESPITE the falling record. This is the discipline
working: it separates "the win column moved" from "there is work the guide layer can do," and
rotates on the latter. Add as a worked example of new-work-exhaustion-not-record as the
rotation trigger.

## No new rung
The representation<->parser coupling lesson is a CONFIRM+GENERALIZE of the wave-27 line, not a
new rung. Attack-floor and cross-phase-deciding-fact rungs re-witnessed, no new corner.
