# Wave-54 audit lane M — GPT seat, region 2 of AIPlayerGPT.cpp (plan "Lane G")

Base: master 6fc0e601f (wave-54 engine lanes A-F merged; the plan's line numbers are ~+1160
off — every site below is named by function, not line). Branch `w54-lane-M`.
Items: A17, A19 (caller side, against the EXISTING `assemblePrompt` signature — the overload is
lane L's), A21, A22, A33's stderr line, L5, L6, L7 (+PARSETEST), L8. L9 NOT done (see below).
Header members sit in the `// audit-M` block of `include/AIPlayerGPT.h`. Every lane-M change is
disabled at runtime by `WAGIC_GPT_AUDIT_M_OFF=1` (the "was it lane M?" flag; the A/B gate below
is exactly that flag on vs off on one binary).

## What shipped (all `#W54-M (...)`-tagged)

- **A17** `askModel`: `mAskCache.clear()` when `observer->turn` moves (member `mAskCacheTurn`).
  The key embeds the turn header + board, so nothing that could still hit is dropped; same-turn
  re-polls stay cached (measured: cache hits 6 vs 6 on the seeded A/B game, 79 hits on the live
  35B game with 28 clears dropping 104 dead keys).
- **A19 (caller side)**: (a) the `:17398` one-liner — the re-ask key is `askKey0 + "\n" +
  mAskReaskLine` (same bytes, no second `serializeGameState`); (b) `FindCardToPlay` hands its
  already-rendered `boardNow` to `askModel` through `mAskSituationPrefill` (swapped out on entry
  like the narration/note members, so it can never leak onto a later ask; attempt 0 only). The
  per-tick base-render memo and the `assemblePrompt(tail, const string*)` overload are lane L's
  (region 1) — not touched here. Cast decision: 3 renders -> 2 per poll (assemblePrompt still
  renders on send).
- **A21** `describeAction`: the mana planner (`selectAutoTapProducers`) runs ONCE per row (a lazy
  `rowAutoTapPicks` lambda feeds both the `{paying this taps:}` and the `{strands...}` clauses);
  `potentialColorReach` runs ONCE per render window (`windowReach()` memo armed by
  `chooseOrderedAction` around its describeAction loop; direct call outside a window).
- **A22** combat: `CombatWindowCache` (per-window memo of `combatStatOf` incl. the blocking-
  trigger walk, and of `playerHasLifeToDamageConverter` per player); `combatBlockOutcome` /
  `combatAttackOutcome` take the cache; `chooseAttackers` asks `combatPreventionKind(c, attacker)`
  once per pairing and reuses it for the gang/price tests; `playerHasLifeLoop(opponent())` hoisted
  out of the per-row loops in both windows.
- **A33** the unconditional `fprintf(stderr, "menu on %s could not be put to the model ...")` is
  now a translog `defer` record (`fallback = menu_not_askable`, the reviewer's release channel)
  + the same text under `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- **L5** `tail.str()` copies: `tellp()` for the two options-end offsets; one `tailStr` copy in
  `chooseOrderedAction` (was 5 copies of the option list per render on the wide boards).
- **L6** `mListDeclineCount` and `mStuckCastLines` hold `std::hash<string>` of the menu text
  instead of the text (`listKeyHash`). `mHoldRows` left as strings (its rows feed
  `holdStillStands`; not worth the churn at LOW).
- **L7** `readDigits(s, pos, end)` — one bounded digit reader (saturates at 100000) replacing all
  12 unbounded `n = n*10 + ...` accumulators in `nameOrdinal`, `becomesBlockedSelfPump`,
  `proseAttackerOrdinal`, `salvageProseAttackers`, `salvageProsePutList`, `parseAttackerSet` (x3),
  `parseBlockAssignments` (x3). PARSETEST +6 cases (2469 -> 2475): `B4294967297:A1` -> 0 pairs,
  `B1:A4294967298` -> 0 pairs, `ATTACK: A4294967297` sends nobody, `#4294967297` never reads #1,
  plus two POSITIVE controls (`B2:A1`, `#2`).
- **L8** `mRepeatClickId` / `mLoopClickId` (mtgid captured when the plan/loop is noted) are
  compared alongside the raw pointers, so an address reused by a foreign card can no longer
  dispatch a queued repeat-N plan or count a foreign loop. Compared via the LIVE row's card only.

## Gate numbers (final binary = the committed source, hermetic rebuild)

- PARSETEST: base 2469/0 -> **2475/0** (+6 = the L7 pins; all six PASS).
- Suite (THREADS=8): **1221 tests, 2 failed (lifeline.txt + merrow_reejerey.txt = the known
  concurrency-only pair, identical on the seeded base binary), 0 timed out, 51 AI / 0 failed.**
- **`options_text` byte-diff — the falsifiable gate.** The engine is NOT replay-deterministic for
  a GPT game (a fixed RNG seed reproduces the deal, but tick-count-dependent timing diverges
  the games; two base-binary runs with the same seed diverged at record 11 — a probe added
  `WAGIC_SEED_PROBE` to the GameObserver ctor for these runs only and is reverted). So the gate
  is STATE-MATCHED: key = `--- CURRENT SITUATION ---` block + question (everything before the
  first option row), value = the option rows + tail; every state seen in both runs must render
  the identical bytes. Runs: one binary, flag ON vs `WAGIC_GPT_AUDIT_M_OFF=1`, same seed, stub
  endpoint (`tools/gpt-stub-server.py`, real prompt/parse/translog path):
  - deck123 vs deck126, seed 424242: 50 states each, **50 common, 0 mismatched** (39 ask, 7
    priority, 2 reveal, 1 attackers, 1 discard); the games end identically (turn 16, 50 life).
  - deck125 vs deck146, seed 4242: 91/91 states, **87 common, 0 mismatched** (53 ask, 18 priority,
    14 discard, 1 attackers, 1 reveal); identical end state (turn 37, 29 / -14).
  - deck125 vs deck146, seed 9191: 111/106 states, **83 common, 0 mismatched** (55 ask, 21
    priority, 5 discard, 2 attackers).
  - Control (same binary, flag OFF twice): 25/25 common, 0 mismatched.
  (One earlier "mismatch" was a library-reveal listing the same cards in a different hidden
  order between two runs — a state difference, not a render difference; checked as a
  permutation.)
- **Runtime self-check probe** (compiled in for the gate runs, REMOVED before commit — the
  committed file has no `W54M` text): on every prefilled `askModel` the prefilled situation was
  compared with a fresh `serializeGameState()` — **prefill_match=1 on all 43 + 175 + 107 + 502
  (live) prefilled asks, 0 mismatches**; the `windowReach` memo vs a fresh
  `potentialColorReach` — 0 mismatches; the per-row `picks` reuse vs a fresh
  `selectAutoTapProducers` — 0 mismatches; RNG-draw deltas across every memoised function — 0
  everywhere (the memos cannot shift the game's random stream).
- **Counters (probe, removed)** — planner runs per render window at identical rows:
  seed 4242: 622 rows / 392 windows, planner_runs **16 -> 8**, reach_calls 8 -> 8 (the reach
  hoist only pays on multi-row windows); seed 9191: planner_runs **42 -> 23**, reach_calls
  21 -> 23 (rows 773 vs 815: the seeded games drift by one window late); seed 424242 (11-row
  windows x2): reach_calls **22 -> 2**. The 432-row Staff-of-Nin window was not reproduced —
  the ledger's ~860 -> ~432 figure is the ratio these numbers show (2 planner runs per priced
  row -> 1, N reach scans -> 1), not a measurement on that board.
  `serializeGameState` per cast poll: 2 (FindCardToPlay's boardNow + assemblePrompt) — askModel
  itself rendered 0 times on 502 prefilled live asks. NOTE for lane L / the ledger: the live 35B
  game entered `askModel` **3,397 times for ~110 answered asks** — the seams re-poll askModel
  every tick while an answer is in flight and each poll re-renders the key on the non-prefilled
  seams; that is where the per-tick base-render memo (A19 main half) pays, far more than the
  per-decision count suggested.
- **Live 35B translog** (qwen36-35b-a3b at :8084, deck123 vs deck126, uncapped, probe binary with
  the probe env set): completed (turn 19, winner 1), 133 records, fallbacks only
  `named_row_reask` 1 / `plan_choice_conflict` 1, **cache hits 79 with 28 turn-boundary clears**
  (same-turn re-polls replay from the cache — the A17 boundary holds). **`latency_ms -1`
  re-poll records: NONE** — and none exist in the pre-change corpus either (60 wave-53 translogs,
  5,200 records: one `reveal` record at -1, zero decision records): a cache HIT returns from
  askModel BEFORE `writeTransLog`, so cache replays never produced a record. The tripwire as
  written cannot fire on either binary; the cache-hit counter above is the working tripwire.

## Predictions (falsifiable)

1. Any `ask`/`priority` state rendered by this binary and by master for the same board+question
   is byte-identical (the A/B above; a corpus reviewer diffing wave-54 prompts against wave-53
   for the same board will find no lane-M drift).
2. A wave-54 corpus translog will show NO `menu_not_askable` defer records unless a menu really
   had no answerable shape; when it does, the record carries the menu card + the heuristic's
   option (previously stderr only, invisible on the Vita).
3. A reply `B4294967297:A1` (or any 6+-digit label) never declares a block/attack.
4. GPT seat-game heap: the ask cache holds at most one turn's keys (median key 1.4-7.8 KB).

## NOT verified / left open

- **A22 at runtime**: no stub or live game in this lane produced a BLOCKERS window, and the
  attackers windows had no untapped potential blockers, so `combatBlockOutcome` /
  `combatAttackOutcome` (cache versions) never executed under the probe; they are verified by
  compile + reading only (pure inputs, same call order, the memo returns the struct the
  pairwise call produced). The struct-only PARSETEST `combatTradePreviewStats` cases are unchanged.
- **L8 live check** (sacrifice the token maker mid-plan) not run.
- **A33 defer record** not observed live (no un-askable menu occurred).
- The L7 "was B1:A1 by int wrap" pre-fix claim is by reading (ledger 2-F7); the base binary
  has no such PARSETEST case to turn red.
- **L9** (duplicate-logic clusters) NOT done — lowest priority, and every cluster is a
  parse-path rewrite whose byte-identity this lane could not gate beyond PARSETEST.
- Measured on stub games and one 35B game only; no Vita numbers.

## Artifacts (under ~/.gatelogs/, not committed)

`w54-M-cmp2.py` (state-matched comparer), `w54-M-run.sh` / `w54-M-run2.sh` (stub game runner),
`w54-M-home-{p3-on,p3-off,c2-on1,c2-off1,c2-on2,c2-off2,live2}/` (translogs + probe stderr),
`w54-M-{base,final}-{suite,parsetest}.log`.
