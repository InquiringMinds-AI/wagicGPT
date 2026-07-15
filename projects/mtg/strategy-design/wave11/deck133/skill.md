# deck133 wave-11 — strategy-writing-skill proposals

Corpus: `matchups-20260715-072254` (wave-10 engine batch: 4096 cap + parse ramble guards; answer-after-plan).
One WATCH-level proposal, grounded in a new finding. It refines an existing rung's verification; it is not a
new headline and (single-seat) it cannot add or cut a core line.

## Proposal 1 (WATCH — refines wave-10 headline 2 point 2 / the confirmed-fired-representation terminus)

**An own-targets/legal-targets option-line annotation has TWO distinct leak mechanisms, and a reviewer must
name WHICH one fired before crediting or re-diagnosing the annotation. Wave-11 introduces the second:
REASONING-LEVEL disobedience via a model rules-misconception (targeting a creature SPELL ON THE STACK),
distinct from wave-10's NUMBER/PARSE leak (reasoning obeyed, the emitted index leaked).**

Wave-10 established (deck133 P1) that an annotation can be obeyed-in-reasoning yet leak at the number/parse
layer, so annotation obedience must be scored against the REASONING, not the recorded choice. Wave-11
supplies the counterpart and closes the loop on wave-10's reactivation trigger:

- **The wave-10 number/parse mechanism did NOT recur at this seat.** The formatless-truncation int-grab is
  killed by the ramble guard (every fallback now falls to the heuristic with choice=-1). The
  unoffered-desired-card case now emits an OUT-OF-RANGE index (vs44 seq29: wanted Geralf's, emitted
  `CHOICE: 3` into a 2-option list) that is REJECTED, not int-grabbed. So the wave-10 reactivation trigger
  ("a 2nd seat / recurrence of annotation-obeyed-but-number-leaked") is NEGATIVE — the harness batch closed
  that mechanism.
- **A DIFFERENT mechanism appeared: the reasoning itself disobeys the annotation.** Twice (vs110 seq25,
  vs135 seq45) the model read `the only legal targets are YOUR OWN right now` and reasoned straight past it,
  fabricating that Fatal Push could destroy an opponent creature that was still ON THE STACK mid-cast
  ("The Steel Overseer is on the stack... Fatal Push can destroy it immediately"). `CHOICE:` matched the
  (wrong) reasoning; the cast resolved onto the model's own creature. This is the read-then-disobey WALL in
  a rules-misconception shape — the annotation states WHICH targets are legal but not WHY the on-stack
  creature is not one (stack-vs-battlefield; destroy-vs-counter conflation).

**Authoring consequence (refines the terminus's verification signal):** when scoring a shipped
own-targets/legal-targets annotation, classify each self/own-target cast by MECHANISM — (a) reasoning
DISOBEYED the annotation (model belief; representation may need to name WHY the excluded target is illegal;
a louder guide line is still the wrong lever — representation-beats-instruction) vs (b) reasoning OBEYED,
the NUMBER/PARSE leaked (harness/reply-protocol). The two have different owners and different fixes, and a
raw "self-cast count" conflates them. The annotation's obey-rate held at ~95% (39/41 real-choice windows,
matching wave-10's 27/29); the residual is now a MODEL rules gap, not a number leak.

Convergence status: **single seat (deck133), 2 instances, both WON/non-deciding.** Per the epistemics rule
it does not add or cut a core method line; it is offered as a SHARPENING of the confirmed-fired-representation
terminus (the mis-cast->0 measure gains a "classify the leak MECHANISM — reasoning-disobey vs number/parse"
qualifier). Adopt into the method only if a second seat shows a legal-targets annotation DISOBEYED in
reasoning this wave; otherwise WATCH.

## No other skill changes.
The freeze/demotion doctrine, the pause-aware precondition, the conditional-surface demotion granularity
(the empty-hand annotation surface < guide-stopgap surface -> KEEP, re-confirmed exactly), the bare-N /
shown==resolved terminus, and the reply-protocol layer-routing all held cleanly this corpus and required no
revision from deck133's evidence.
