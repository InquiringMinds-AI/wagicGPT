# deck133 wave-10 — strategy-writing-skill proposals

Corpus: `matchups-20260715-031825` (answer-after-plan protocol). One method proposal, grounded in a
finding. It extends existing rungs; it is not a new headline.

## Proposal 1 (ADOPT-candidate, extends wave-9 headline 2 point 2 "reasoning-vs-outcome split" and the
## wave-9 headline 4 option-line-annotation terminus)

**An option-line annotation can be OBEYED-IN-REASONING yet leak at the number/parse layer — verify
annotation obedience against the REPLY'S REASONING, reported separately from the recorded CHOICE,
because under answer-after-plan the two can still diverge.**

Wave 9 established the option-line annotation as the last representation rung before the number is
emitted, verified by mis-cast->0 and a reasoning-tax drop. Wave 10 adds the failure mode that survives
it: at deck133 the own-targets Fatal Push annotation (`the only legal targets are YOUR OWN right now`)
was read and obeyed in the PLAN prose at **all** 29 windows ("Fatal Push has no valid targets... I will
not cast it"), yet the recorded choice cast the self-Fatal-Push in 2 of them. The divergence lives
DOWNSTREAM of the annotation:
- **deck140 seq19**: the plan's desired card (Phyrexian Obliterator) was UNOFFERED, and the model mapped
  its fallback digit onto the annotated-bad option (`CHOICE: 1` = self-Fatal-Push) instead of the safe
  "Cast nothing". The annotation fired and was believed; the NUMBER contradicted the belief.
- **deck110 seq20**: a formatless, truncated reply concluded "Cast nothing" in prose but was int-grabbed
  to the self-Fatal-Push by the parser.

Authoring consequence: when scoring a shipped option-line annotation, **score it by whether the REPLY
REASONED to the annotation's verdict, NOT by whether the recorded choice matched** — a mis-cast count
that treats the recorded choice as the annotation's obedience metric will misattribute a downstream
number/parse leak to the annotation (concluding it "failed" and inviting louder wording, when the
annotation actually succeeded and the leak is a harness item). This is the annotation-shaped instance of
the wave-9 "the fix fired in the reasoning; the leak moved to a lower layer" split, now applied to the
representation rung itself (previously applied only to guide/gate fixes). Route the downstream residual
to the reply-protocol/harness layer, never re-word the annotation.

Convergence status: **single seat (deck133), 2 instances.** Per the epistemics rule it does not cut or
add a core method line on its own; it is offered as a SHARPENING of an existing rung's verification
signal (the confirmed-fired-representation terminus's "mis-cast -> 0" measure gains an explicit "measured
in the REASONING, not the recorded choice" qualifier). Adopt only if a second seat shows an
annotation obeyed-in-reasoning / contradicted-by-number this wave; otherwise WATCH.

## No other skill changes.
The freeze/demotion doctrine, the pause-aware precondition, the conditional-surface demotion granularity,
and the confirmed-fired-representation terminus all held cleanly this corpus and required no revision
from deck133's evidence. The empty-hand annotation validated the wave-9 demotion caveat exactly as
written (annotation surface < guide-stopgap surface -> KEEP), which is a confirmation, not a change.
