# skill.md proposal — deck102 wave 23

## VALIDATION (not a new rung): the crutch-lifecycle DEMOTE terminus fired a FOURTH annotation shape, cleanly.
Wave-21/22 established the demotion terminus (RETIRE if the rule is gone; DEMOTE if the rule is intact and
the engine now carries it, verified by (i) misplay-count -> 0 AND (ii) the FIRED signal visible in the
prompt) across marker-string, option-annotation, and CAUTION-line/battlefield-tag shapes. deck102 this wave
adds the **DISPLAY-TOGGLE option annotation** shape: the wave-22 guide paragraph teaching "don't sit
flipping between faces, it does nothing" is now redundant with the engine's own option text ("DISPLAY
TOGGLE only... It does NOT cast anything... the Cast menu is where you cast"), and the misplay count fell
11 -> 1. Same terminus, verified the same way. No new rung needed — this is confirmation the terminus
generalizes across annotation shapes, and it retires this guide's mechanical crutch (executed, strategy
edit 1).

## PROPOSAL (small sharpening of the wave-22 "audit for now-FALSE clauses" sub-rung).
Wave-22 added: when you DEMOTE a crutch, audit its text for clauses the fix made factually FALSE (an
absence-of-signal narration the fix now contradicts) and DELETE them. deck102 exposes an adjacent case that
deserves one sentence: **separate a crutch's ORACLE-truth clauses from its ENGINE-BEHAVIOR-narration
clauses when you demote.** The wave-22 Tergrid crutch mixed both — a mechanical "the flip toggle does
nothing / cast a face" (ENGINE-behavior narration, now carried by the annotation -> DEMOTE) and a
"the Lantern is a repeatable {T}: drain you cast when mana-light" (ORACLE description of the card). This
wave surfaced an engine defect where casting the Lantern back-face actually deploys the God front-face
(R-DFC-BACKFACE-RESOLVES-FRONT), which means the oracle clause may not match what the engine executes.
Ruling for the skill: DEMOTE/trim the engine-behavior narration (the fix carries it), but do NOT rewrite an
oracle-truth clause to chase a suspected engine BUG on a single observation — route the bug to notes, keep
the guide oracle-accurate-but-generic (I softened "Lantern = repeatable drain" to "Lantern = cheaper face"
rather than asserting the bug), and only correct the oracle clause once the engine's actual behavior is
confirmed durable. I.e. guides describe correct play; engine bugs get fixed in the engine, not encoded into
the guide from one game. One-line addition to the demotion rung; caught a real fork here.

## PROPOSAL (validates the wave-22 keystone-sequencing proposal — fold in if the probe-deck rung is revisited).
Wave-22 proposed: a rare keystone blocked by a representation defect needs the defect fixed BEFORE probing
the seam. This wave VALIDATES the mechanism WITHOUT a probe deck: once batch (b) fixed the DFC-flip
representation, the random pool's single Tergrid draw converted all the way through cast -> steal-seam
fire in ONE game (vs27). Lesson to record on the probe-deck rung: **fixing the representation defect on the
path to a keystone can unblock the keystone in the RANDOM pool too — a probe deck is the guaranteed path,
but it is not always required once the representation is clean.** Optional; only if that rung is edited.

(No change proposed to the reveal pick-ONE, answer-first, or INDEX-WINS/echo rungs — all held clean at this
seat this wave, 0 fallbacks.)
