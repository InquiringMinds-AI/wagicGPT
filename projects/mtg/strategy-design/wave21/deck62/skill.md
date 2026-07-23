# deck62 wave-21 — proposed additions to the strategy-writing skill (Step 0)

deck62 is now a FIVE-guided-corpus seat (revised round 5, edit D live). Two rungs this wave, both grounded at
this seat and both about the REVIEWER INSTRUMENT rather than guide authoring — because the guide has converged
and the wave's signal is a parser/answer-first interaction, not a guide edit.

## 1. (STRONG, reviewer instrument) When a new PARSER SAFETY ROUTE ships (retracted_choice, prose-salvage, etc.), audit it ADVERSARIALLY for OVER-FIRING against the seat's own known over-deliberation signature — a fallback logged against a reply that CONTAINS a correct, uncontested, first-line coded answer is a FALSE POSITIVE, and the seat whose behavior most resembles the route's trigger is the one that will expose it.
The wave-21 batch added `retracted_choice`: a CHOICE "retracted with no replacement" routes to heuristic. deck62's
signature (established over waves 19-20 as N6) is over-deliberation-of-a-SETTLED-decision: reach the right answer,
then keep deliberating a DOWNSTREAM step using self-correction language ("Wait", "Correction:"). Wave-21 answer-first
put the correct answer on line 1 — and then the detector read the downstream self-correction as a retraction of the
line-1 CHOICE. All 3 of deck62's retracted_choice fallbacks (vs27 s10, vs135 s5, vs102 s3) are this over-fire: each
has exactly ONE well-formed CHOICE (the correct `CHOICE: 1 (Play Forest)`), zero second/contradictory CHOICE, zero
disavowal — the retraction phrases live entirely in the PLAN body (Fists-can't-target-Shroud, mana math). LESSON:
a parser SAFETY route (built to CATCH a bad reply) must be scored for FALSE POSITIVES with the same rigor a SALVAGE
route is scored for correct no-fires (wave-20 rung #1). The test: does the fallback-flagged reply contain a
correct, in-range, UNCONTESTED coded answer the parser SHOULD have taken? If yes, it is an over-fire (a false
positive), attributed to the HARNESS, and it does NOT count against the guide or model. Corollary — pick the
over-fire's WITNESS SEAT by matching the route's trigger to a seat's documented behavioral signature: the
retracted_choice route triggers on "self-correction language after the answer," and deck62's over-deliberation
signature IS self-correction-after-the-answer, so deck62 was predictably the seat that surfaced it (owns 3 of the
corpus's 5 retracted_choice, all over-fires). This makes the affected seat the ideal fix-VALIDATOR, which bears on
rotation (rung #3).

## 2. (STRONG, NEW) ANSWER-FIRST induces snap-then-REVISE, and for a reasons-then-revises model the SNAP answer is often the WORSE option — so "last well-formed CHOICE/BLOCKS wins" is not a mere parse convenience, it is a QUALITY-PRESERVING mechanism, and answer-first's real residual risk is a truncation BETWEEN the snap answer and the correction.
Answer-first asks the model to emit the coded line FIRST, then reason. At deck62 the model repeatedly emits an
inferior snap answer and then REASONS ITS WAY to a better one, re-emitting a corrected coded line: vs14 s26 (ask)
snap `CHOICE: 4` (a redundant 2nd Yavimaya) -> reasoned `CHOICE: 2` (Fists — draws + makes tokens); vs135 s21
(block) snap `BLOCKS: B2:A1` (block the deathtouch 1/1, the "both die" instinct) -> reasoned `BLOCKS: B2:A2` (block
the 13/13 trampler, saving 8 life, 14 vs 6). In both the parser's last-well-formed-wins rule took the REASONED
answer, so there was NO answer-quality regression. LESSON: when validating answer-first, do NOT stop at "the coded
line is first and it parsed" — check whether the FIRST answer equals the FINAL answer, and when they differ, confirm
the parser took the LATER (reasoned) one. Two consequences: (a) last-well-formed-wins is load-bearing for QUALITY,
not just parseability — a naive "take the FIRST coded line" reading of answer-first would have executed the worse
snap answer both times; (b) the residual answer-first risk is a reply that TRUNCATES after the snap answer but
before the correction — the worse snap answer then executes with no rescue. Score that exposure by checking whether
any truncated reply's parsed answer differs from where its (cut-off) reasoning was heading. Distinguish this cleanly
from the N9 over-fire (rung #1): the revise case emits a SECOND coded line (last-wins fires correctly); the over-fire
case has only ONE coded line and prose self-correction (the detector must not touch it).

## 3. (refinement, rotation discipline) A guide can be CONVERGED (byte-freeze earned) while the seat is HELD in the pool purely as the WITNESS/VALIDATOR for an open HARNESS defect it uniquely exposes — separate "guide needs work" from "seat is useful to keep," and name which one is holding the rotation.
deck62's guide is converged this wave: edit D validated (its target spiral class dead), every taught class held,
no guide contradiction or #1-leak, strategy.txt byte-frozen. On GUIDE grounds it would rotate. The only reason to
keep it is that it is the corpus's prime witness for the N9 retracted_choice over-fire (owns 3 of 5) and the ideal
validator for that fix. LESSON: the rotation test's two gates (guide-mod? new-work-signal?) can DISAGREE when the
new-work-signal is HARNESS-lane rather than guide-refinement. When they disagree, state it explicitly: "guide =
freeze/rotation-eligible; held only as harness-witness for <item>." This prevents two errors — (a) recording a
harness open-item as a reason the GUIDE isn't done (it is), and (b) rotating out the best witness for a live
harness defect before its fix is validated. The seat becomes a clean rotation candidate the moment the harness
item ships and is confirmed at that seat.

## 4. (confirmation, cross-wave) The computed-P/T distrust pocket stays DEAD at round 5 — cheap negative-signature grep, one line.
Fifth consecutive corpus with zero recount-of-already-printed-P/T (tight-signature grep = 0). Every aura/Blanchwood
mention is prospective cast-value math. Reinforces the wave-18/19/20 verdict; no new method. (Retained per the
established rung: a multi-wave-retired pocket is verified by a null grep reported in one line, not re-audited.)
