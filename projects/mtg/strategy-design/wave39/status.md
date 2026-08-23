# Wave-39 status (pre-review checkpoint, 2026-08-23)

Corpus matchups-20260823-040744 HARVESTED (binary eabf463b1, rotated pool
146,139,152,36 + Step-0 guideless 125,126,162): 20/21 natural, 1436 decisions,
8 fallbacks (validation_reject_reask_exhausted x5 — NEW class, all-assignments x1,
unparsed x2). All anomalies cluster on NEW decks (rotation working as designed):

1. **CRASH #2 (139v125, t15, core dumped)**: stack = __dynamic_cast inside
   GameObserver::Affinity() <- gameStateBasedEffects <- Update. Affinity() iterating
   with a bad/stale pointer during SBA — deck125 (Revelation Fracture) triggered it;
   distinct from the wave-37 ALord crash. Stack + core in crash/ (core gitignored).
   NEEDS: dedicated step-1 lane (line-level frames from the core, repro fixture, fix).
   The 139v125 matchup needs a RERUN after the fix (completeness invariant).
2. **5 consecutive defers, deck125 seat, turn 5 seq 11-15** — the pilot deferring the
   same window repeatedly (control-deck priority shape, possibly counter-hold asks).
   Review must classify: ask-shape defect vs correct holds mis-recorded.
3. **validation_reject_reask_exhausted x5** — new fallback class firing; identify which
   ask shape rejects validation (likely a new-deck menu the c3 staleness gate churns on).

OWNER RULING (2026-08-23, binding, on the zero-slack-X anomaly): "casting a spell,
with x as zero is legal. not only that, there obviously times when a player should
do so." — X=0 casts fall UNDER the never-hard-suppress rule. Do NOT filter zero-slack
X spells out of the cast menu. The wave-40 fix targets the VALIDATOR and the replay
loop instead: (1) the GPT seat must be ABLE to announce and commit X=0 — Baka's
slack-penalty heuristic must not gate the model's explicit choice; the ask should
reach the model (this corpus it never did: latency -1, no model call at any of the
5 records); (2) the no-model-call reject/replay churn (cached answer replayed against
a rebuilt menu until phase end) is the bug to kill, without suppressing the offer.

NEXT (the loop, in order): review step = 3 Step-0 initial-guide authors (125, 126, 162
— guideless corpus data at the run dir) + hold-seat validations (139 mutate-header/
host-carry predictions in the W38 lane report; 36 crash-fix live check; 152/146 new
teaches) + the anomaly classifications above; then synthesis + rotation adjudication;
then step-1 = Affinity crash lane + whatever review ranks; then corpus. Standing serve:
FP8-35B :8084 (NEVER cycle — owner ruling). Pool/exits recorded in
wave39/rotation-selection.md. Suite baseline 1047/0+30/0, PARSETEST 627/0.
