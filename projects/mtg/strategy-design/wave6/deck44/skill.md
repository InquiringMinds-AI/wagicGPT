# Proposed skill revisions — deck44 wave 6

## P1 (ADD to Step 2 harness-defect sweep) — the VALIDATION-DROP shape: chosen != happened, visible only in stderr

The current sweep (DESYNC, ENGINE NO-OP, NUMERIC-OFFSET, PLAN/CHOICE MISMATCH) is resolvable from the
JSONL alone. This wave surfaced a defect the JSONL **cannot** show: a pick that was recorded but never
executed because it failed target-validation and the engine **silently deferred to the heuristic**.

- deck44 F1: the JSONL logs `chosen_text = "Cast Go for the Throat"` at 7 decisions across two games,
  but the game-`*.stderr` shows `model chose Go for the Throat but it fails validation; deferring to
  heuristic` followed by `NextGamePhase` — the model's pick was dropped and a heuristic pass played.
  A reviewer working from the JSONL alone would score these as real GFTT casts and mis-diagnose them
  as bad targeting or passivity.

Proposed method addition (one bullet in the harness-defect sweep): **VALIDATION-DROP (chosen recorded,
heuristic played).** When a chosen action is a targeted spell/ability and the same decision's board
shows no legal target, cross-reference the run's `game-*.stderr` for `fails validation; deferring to
heuristic`; those records are harness defects (the engine offered a targetless action), not decisions —
do not score the recorded `chosen_text` as what happened. This is a sibling of ENGINE NO-OP: no-op =
pick executes nothing repeatedly with mana unspent; validation-drop = pick is illegal-target and is
replaced by a heuristic move the JSONL does not record. Digest guidance: when a decision's board has no
legal target for the chosen targeted spell, flag it and check stderr.

## P2 (CONFIRMATION, no method change) — representation-beats-instruction, vindicated at the stack seam

Wave 5 routed deck44's #1 P0 (counters 2/27, fabricated resolutions over the log tail) to REPRESENTATION
and predicted the shipped ON THE STACK section would fix it. **It did: 2/27 -> 13/14, zero
hallucinated resolutions, sweeper-first obeyed, and the two matchups deck44 lost to un-countered
sweepers (vs140, vs110) flipped to wins.** No method change — this is the third corpus-scale
confirmation of the pattern (after deck44's own log-tail case and deck110's mana line) that a
first-class CURRENT-SITUATION line beats any amount of louder wording. Recommend the wave-6 synthesis
record it as a closed prediction: the counter-tell retirement was correct, and the "fact living only in
the log tail is functionally hidden" rung is now empirically validated end-to-end (bug -> route ->
ship -> win).

## P3 (no change) — the convergence-watch mechanism fired correctly

Reflexive high-life blocking is now a SECOND seat (deck109 wave 5, deck44 wave 6). The
convergence-watch bullet the wave-5 synthesis added did its job. Note for the synthesis: the correct
promotion here is NOT a new core sentence — the core ALREADY carries the high-life blocking floor
("blocking is optional and 'no blockers' is often the right answer... block only when you can name the
gain"), and the pilot is DISOBEYING it at distance (blocking at 20 life while asserting a false
"prevent lethal"). Per the skill's own layer-routing ("a rule already stated and still bypassed routes
to representation, never a second paragraph of the same instruction"), this promotes to a
REPRESENTATION intervention (surface the pre-block life / a "this does not prevent lethal" cue at the
blocker decision) plus per-deck reinforcement — the pattern the skill predicts, no rewrite of the core
sentence. Detailed in general-suggestions.md.

No other method changes proposed. The tell audit, harness sweep, turnaround/obeyed-but-losing mode,
threshold reuse, and the negative-framing ruling all held at this seat.
