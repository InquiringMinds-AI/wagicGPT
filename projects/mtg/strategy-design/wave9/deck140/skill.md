# Skill deltas from deck140 — wave 9 (final wave)

deck140 was in TURNAROUND/FREEZE mode this wave (count-anchored SWEEP TRIGGER validated a 2nd corpus).
No new core method headline; the wave contributes CONFIRMATIONS and two refinements to existing lines.

## Confirmations of existing method (report to synthesis as "held")
- **Re-anchor-to-a-count validation is durable across corpora.** Wave-8 validated the `(creatures: N)`
  SWEEP TRIGGER once (defect class -> 0 after the castability/intra-turn join). Wave 9 REPEATS it on a
  fresh corpus: 0 held-castable-sweeper overshoots except one wording-caused miss, and BOTH C=0 sweeps
  traced to the reply protocol, not the trigger. A validated re-anchor stays validated; keep it FROZEN,
  resist re-widening. (Extends wave-8 Method headline 3.)
- **Score both directions AFTER the join.** The raw tally this wave was "2 sweeps into empty boards" +
  "1 hold with creatures present" — read naively that reads as trigger failure in both directions. After
  joining to the PLAN text and the reply head, all three dissolve to reply-protocol/stale-plan/wording,
  NONE to the count trigger. Always join before scoring a frozen re-anchor. (Confirms confirm-before-accuse.)

## Refinement 1 — a BOUNDARY VALUE the executor mis-resolves is a WORDING defect, not a disobey wall
New shape of the representation-vs-instruction distinction. When the pilot READS a clearly-true condition
and acts against it, that is the read-then-disobey wall (do NOT escalate). But when the pilot mis-resolves
an AMBIGUOUS boundary — here "your life is 16 or less" at life exactly 16, computed as *"above the 16
threshold"* — the deciding condition was never unambiguously TRUE to the executor, so this is a wording
defect and DISAMBIGUATION is the correct rung, not escalation. Test to tell them apart: quote the pilot's
own PLAN. If it states the condition as satisfied and acts against it -> wall. If it states the condition
as UNsatisfied via a boundary/threshold reading -> disambiguate the number ("16 or LOWER - 16 itself
counts"). deck140 vs135 t13: the boundary misread was the pilot's PRIMARY stated reason; the tapped
rationalization was secondary cover the boundary error enabled. Fix the number and the escape closes.

## Refinement 2 — a demoted/compressed false-belief can resurrect in a NEW GUISE the exact wording
does not cover; name the invented anti-pattern, do not re-inflate the whole block
Wave-7/8 killed the tapped bridge ("tapped -> no threat"). This wave it resurrected as a DIFFERENT
phrase — *"it isn't attacking THIS turn"* — that the standing "Tapped is NEVER a reason to wait" line
does not literally name, and that the executor invents to satisfy a hold it already wants (here, off the
boundary error). The fix is NOT to demote further (the full-strength line was present and still failed =
evidence against demotion) and NOT to add a paragraph, but to NAME THE SPECIFIC INVENTED PHRASE inline
("'it isn't attacking THIS turn' is not a reason to hold — it hit you last turn and attacks next turn").
One clause, anchored to the tag text. (Extends wave-8 P1 "name the anti-pattern the executor will invent"
+ the per-belief demotion rule: a belief demoted against a representation can re-enter through a phrase
the representation does not surface.)

## Reply-protocol evidence contributed (route to the reply-protocol A/B, NOT the guide)
deck140 is the SECOND seat to witness answer-before-reasoning intent-collapse (deck135 was first, wave-8):
head token committed before the PLAN reasons, PLAN reaches a different same-window action, head never
revised. 2-3 instances this corpus, ALL self-corrected on the immediately-following priority window
(deck140's many priority windows make it largely self-healing). Distinct from plan/choice mismatch.
Two witnessing seats now clear the "single-instance" bar the wave-8 note set for the durable fix -> the
reply-protocol A/B (accept the LAST bare number / move CHOICE after the PLAN) is now DOUBLE-witnessed;
recommend scheduling it, judged by decision-quality on a symmetric core.

## Timeout-triage confirmation (method already in the skill)
The decision-density x latency triage separated all 3 timeouts (60-103 decisions, ~21-23 min cumulative
latency, deck140 ahead/even) from the 3 losses (18-37 decisions, 4-7 min, died). A raw timeout row is not
a play failure; the fix is infra (cap-by-decision-count / adjudicate-by-life-at-cap), not the guide.
Confirms the wave-4 timeout-triage line; results.tsv filling life/turn at the cap made this triage
possible without opening logs blind — keep that.
