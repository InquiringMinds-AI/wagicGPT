# skill.md proposal — deck102 wave 24

Two proposals, both tagged single-seat with promotion thresholds. No change proposed to the
crutch-lifecycle, reveal pick-ONE, INDEX-WINS/echo, or attacker-scoring rungs — all held clean at
this seat (0 flip-thrash, correct Tergrid cast, 1 benign fallback).

## PROPOSAL 1 (small sharpening of method-3 commit-lock classification): a THIRD commit-lock sub-shape — the example-seeded / stale-plan-seeded OUT-OF-RANGE line-1 index.

Method-3 (wave-23) split failed replies into COMMIT-LOCK (a wrong coded line, self-corrected in
prose, TRUNCATION freezes the wrong snap -> guide/parser) vs DECODE-COLLAPSE (no coded line ->
sampling). deck102's s5 (vs133) is commit-lock family but fits NEITHER existing driver: the reply
COMPLETED (no truncation) and self-corrected fully in prose to the right answer (`choose 0 (pass)`),
yet still fell back — because the single coded line was an OUT-OF-RANGE index seeded by two prompt
artifacts, not by truncation:
1. a **stale PLAN naming an unaffordable action** (carried forward as authoritative intent), and
2. the **reply-protocol EXAMPLE `e.g. "CHOICE: 2 (Cast Fatal Push)"`** — the model pattern-filled the
   example's literal index 2 with its planned card before checking legality.

Reviewer ruling to add to method-3: when an unparsed reply carries a coded line that is OUT OF RANGE
(not merely wrong-among-valid), check TWO seeds before routing — (a) did the carried PLAN name an
action that is not in the current legal set / not affordable? and (b) does the emitted index match
the reply-protocol EXAMPLE'S index? If either, the owner is NOT the parser (a "prefer last coded
line" rule cannot help — the corrected answer lives in prose, never re-coded) and NOT the guide; it
is the HARNESS/CORE-PROMPT (plan-hygiene: do not feed a plan that names an illegal/unaffordable
action as intent; and/or an example that cannot produce an out-of-range index). One-line addition to
the commit-lock rung; caught a real, precisely-mechanized fork here. Promotion threshold: a 2nd seat
producing an out-of-range coded line traceable to the example index or a stale plan promotes this to
a named commit-lock sub-shape and escalates the core-prompt example fix (see general-suggestions).

## PROPOSAL 2 (NEW reviewer-method rung for the pregame surface): the mulligan HAND-READ diff — extend method-2's claimed-vs-actual-state diff to the pregame hand.

The pregame mulligan phase (first fleet outing this wave) adds a decision class the reviewer method
did not previously cover, and it exposed a failure the win/keep-count checks MISS. deck102 vs59 seq2:
the model mulliganed a KEEPABLE hand while its reply asserted "zero basic Swamps" — but the prompt's
`Your hand:` line plainly listed TWO Swamps. The keep/mull outcome alone reads as a normal mulligan;
only diffing the model's CLAIMED hand composition against the LITERAL listed hand exposes the
misplay. This is exactly method-2's decision-input-fidelity diff (claimed board-state vs actual
`[tapped]` tags), now applied to the pregame HAND. Proposed rung: for every mulligan/bottom decision,
run TWO diffs — (1) the model's claimed land/color/curve count vs the literal `Your hand:` line
(hand-read fidelity), never accept a "no lands / no black / all bombs" rationale without counting the
listed cards; and (2) bottom-pick quality — did the keep FLOOD (bottomed a payoff to keep an Nth
land) or ship the wrong cards. Also watch for stale-PLAN anchoring: a carried "mulligan to find X"
plan can drive a re-mulligan of a hand that already has X. Tag single-seat; promote to a standing
pregame-review step if a 2nd seat shows a mulligan/bottom driven by a hand-misread or a
flood-inducing bottom.

(No guide edit followed from either — the s5 confusion and the mulligan miscount are model/harness
faults the guide cannot reliably correct; per the crutch-lifecycle anti-pattern, do not add guide
text duplicating attention the model already has the information for. Both route to notes.md.)
