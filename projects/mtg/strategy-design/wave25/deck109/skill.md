# Skill feedback — deck109 wave-25

**One PROPOSED refinement** (below), plus confirmation the crutch-lifecycle rung fired correctly.

## Confirmation: the 2-corpora crutch-validation clock worked exactly as designed.
The wave-23 rung ("one clean corpus is insufficient evidence for a COST/COMPREHENSION crutch;
require a 2-corpora validation clock") gated deck109 correctly across both ticks: wave-24 = corpus
1 (clean, kept the seat), wave-25 = corpus 2 (clean, trigger present, closes the clock). The
doctrine predicted the right cadence — do not shorten or lengthen it; it retired a cost crutch on
exactly the evidence it demanded. No edit to that rung.

## PROPOSED refinement: the "harness-blocked compliance" reviewer-discipline check.
This corpus produced a decision that, read naively from the choice log, looks like *"the model
ignored a guide-mandated on-sight kill"* (vs131 s10 recorded Cast nothing; s11 recorded pass — both
while Young Pyromancer, a guide-named on-sight-kill target, sat on the board). A reviewer scanning
`chosen_text` alone would score these as burn-discipline / exception-class FAILURES and route a
guide revision. **Reading the REPLY text reversed both verdicts**: at s10 the model concluded "cast
Searing Spear at Young Pyromancer" in prose (a parse mismatch swallowed it), and at s11 it declined
only because a truncated target preview hid Pyromancer ("I cannot target a target that isn't
listed"). The model's JUDGMENT was correct in both; the MECHANISM (parser + target truncation)
blocked it.

Proposed rung (for the "how to read a declined guide-mandated action" section): **Before
attributing a declined or wrong-looking guide-mandated action to model judgment or a guide gap,
read the REPLY and confirm the harness actually OFFERED the action cleanly — check (a) the parsed
`choice` matches the reply's concluding answer/PLAN (not a swallowed prose reversal), and (b) the
model's intended target was actually VISIBLE in the option/target line (not truncated behind
"(+N more)"). A misplay caused by harness rendering or parsing routes to notes.md, never to the
deck guide — the guide can't fix a target the model was shown as illegal, or an answer the parser
couldn't read.** This generalizes the existing "read the translog before theorizing" discipline to
the specific trap where the log's `choice` field itself is the misleading artifact.

## Carry-forward (unchanged from wave-24): the mulligan surface is now 2 corpora unexercised.
The aggressive-one-land keep/mull judgment has gone two full deck109 corpora without a marginal
hand dealt (all 12 hands trivial 3-land keeps). If the skill wants per-seat mulligan validation, a
"surface-unexercised" carry-forward flag (so an untested mulligan/bottom surface is not silently
read as "confirmed clean") is now supported by two data points, not one. Still left as an
observation — no rung forced on draw-variance evidence.
