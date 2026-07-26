# deck93 general-suggestions (wave 29) - CORE / HARNESS layer

## VERDICT: PASS (24th consecutive)

Nothing at the deck93 seat this wave points to a core Baka/harness defect. The one issue found
(the `[tapped - untaps and can attack next turn]` wording ambiguity that produced the single
residual phantom) is an ENGINE render-wording item, logged in notes.md as a cross-deck ledger
entry, not a core-suggestions escalation - it is a phrasing polish on an existing annotation,
scoped and low severity, and it is affirmatively better handled at the render layer than by any
core change.

Steady-state confirmations (all clean, no core action):
- Parser/binder: 0 fallbacks across 109 decisions at the 36-Rat stress seat (round 3). The
  first-wins/drop-illegal precedence handled the one over-list cleanly. No parser contradiction.
- Instance handles: contiguous #1..#6 held; A-line/B-line binding correct at scale.
- Structured logging integrity: kinds/seqs coherent; results.tsv matches per-seat outcomes.
- The f3 annotation was added at the render layer (correct layer) and consumed without latency
  or noise - re-affirms that state annotations are the right mechanism for perception leaks and
  that the harness surfaces them cleanly.

The core layer continues to hold. 24 consecutive PASSes.
