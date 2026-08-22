# Wave-36 step-1 lane B — written verdicts (items 5 and 6)

## Item 5 — "empty-reply transport drop" (116-fp8-review, vs deck152 seq12): NOT a transport drop

Record inspected: `matchups-20260822-111102/1787415073-ai_baka_deck116-…-vs-ai_baka_deck152.jsonl`
seq 12 (t7, Farseek PUT reveal, 46 options).

The record carries `fallback: "stale_livelock"` and `latency_ms: -1`. `latency_ms: -1` means
NO fresh HTTP round trip was consumed on this record (cache/reuse), and `stale_livelock` is the
existing give-up class: `kStaleLivelockLimit` consecutive stale drops on one decision hand it to
the bounded heuristic, logging an empty reply BY DESIGN. The reviewer's "harness/API drop"
hypothesis is wrong — the reply and reasoning are empty because the model was never called on
the record that got written, not because a server 200 came back empty. The instrument already
classifies it (the review missed the `fallback` field). No retry is warranted; the class is
self-healing (seq13 re-asked and play continued). **No code change.**

## Item 6 — missing final-blockers ask (152-tier-review item 3, B-vs-105 t15): ENGINE SKIP, real decision lost — FIXED

Evidence, arm B `matchups-20260821-033000`:
- deck152 seat log ends seq24 (t14 main) → seq25 gameend t15. No blockers (or any) record at t15.
- deck105 seat seq32 t15 declared Ichorclaw Myr + Hand of the Praetors at opponent poison 5
  (2+3 infect = exactly lethal 10). deck152's board at that prompt: Wolf, Briarbridge Tracker
  (vigilance), Elite Spellbinder — three legal, untapped blockers; blocking either attacker
  prevents lethal. A real decision existed.
- `game-152v105-1787301018.stderr` tail: after "declared attack (2 of 2)" the DEFENDER activated
  an ability twice (Target: Wolf) with an "unclosed <think>; forcing the answer" between, then
  "Action added to stack: NextGamePhase (Current phase is: Blockers)" resolved straight into
  DamageStack and the game ended. While the ability sat on the stack, `pendingCombatDecision`
  (correctly) returned NONE — its comment promises "deferred, not dropped" — but the queued
  NextGamePhase resolved the moment the stack settled, so the defender never got another settled
  tick and the deferred blockers declaration was silently dropped. Healthy combats in the same
  log show the declaration landing BEFORE any pass is queued; the skip needs the
  ability-in-window + queued-pass interleaving, which is why it is rare.

Fix (this commit): `NextGamePhase::resolve` now yields — consumes the pass without advancing —
while the phase is COMBATBLOCKERS/BLOCKERS and an AI defender's declaration is still due
(`AIPlayer::blockersDeclarationDue`, overridden by AIPlayerGPT as "live endpoint, defending,
`mBlocksDoneTurn` not stamped this turn"). Every chooseBlockers exit (declared, declined,
heuristic fallback, vacuous builder) stamps `mBlocksDoneTurn`, so the hold cannot outlive the
decision; human defenders (isAI false) and heuristic seats (default false) are untouched, so a
human's decline-by-advance still works.

Falsifiable prediction for the validation corpus: every game in which the defending GPT seat has
a legal block when attackers are declared shows a `blockers` translog record for that combat (or
the combat ends before the blockers step for engine reasons visible in the narration); the
per-corpus count of defender combats with legal blocks and no blockers record drops to 0.
