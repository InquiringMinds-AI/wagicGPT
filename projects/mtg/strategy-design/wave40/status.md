# Wave-40 status (post-corpus checkpoint, 2026-08-23)

Corpus matchups-20260823-110407 COMPLETE (binary 465422fcf, pool 146,139,152,125,126,162
+ Step-0 guideless 123): **21/21 NATURAL — first fully complete corpus on this pool.**
2073 decisions, 7 fallbacks (0.34%: unparsed x2, stale_echo x1, empty x3, reasoning_only x1).

Step-1 verdicts already measurable at harvest:
- **139v125 ended naturally t26** (139 won 18-0) — completeness invariant DISCHARGED;
  the cardsAbilities weak-registry crash fix held on the live matchup.
- `validation_reject_reask_exhausted`: **0** (was 5) — desirability-veto + stuck-line fixes.
- `all_assignments_illegal`: **0** (was 1) — unreachable-attacker reasons.
- `draws 0` clauses: **0** (was every Dictate/Puzzle Box line).
- `was COUNTERED` present corpus-wide (coverage fraction owed by review); `STOPS all` 76
  emissions (deck162 Rule-4 stopgap retirement condition now FIRES — retire per register).
- Captain may-asks present (ledger #22's owed arrival trace — seat-126 review verifies).

Deck126 note: FIRST corpus on the corrected 60-card list (owner fix) AND the new guide —
attribute deltas across BOTH (wave-35 rule; recorded in wave39/rotation-selection.md).

Wins (context only, never verdicts): 152:5/6, 146:4/6, 139:4/6, 126:3/6, 162:2/6,
123:1/6, 125:0/6.

NEXT (the loop): review step = deck123 Step-0 initial-guide author + validation seats
(125, 126, 162, 152+146) + cross-cutting fix-validation seat; then synthesis + rotation;
then wave-41 step-1 from the surviving ledger (#7 mutate-pile render, #19 library-mask
narration, #20 mass may accept/deny design, #21 remaining alias counters, #22 suite
may-menu wedge, + whatever review adds); then corpus. Standing serve FP8-35B :8084 —
NEVER cycle (owner ruling). Suite baseline now 1057/0 + 30/0; PARSETEST 671/0.
