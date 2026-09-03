# Wave-54 audit lane L — GPT seat region 1 + transport (plan "Lane F")

Branch `w54-lane-L`, seeded from master 6fc0e601f. Items from `audit-20260902/09-synthesis.md`
section 3 Lane F: A18, A19 (`assemblePrompt` overload half), A20, A23, A24, A49, L1, L2, L3,
L4, L10, L11, L12. Files: `src/AIPlayerGPT.cpp` (region 1 + the PARSETEST tail),
`include/AIPlayerGPT.h` (one `audit-L` block), `src/GptConfig.cpp`, `include/GptConfig.h`,
`src/OptionGpt.cpp`, `include/OptionGpt.h`, `JGE/src/SDLmain.cpp`, and two lines in
`src/GameStateDuel.cpp` (the ONESHOT `exit(0)` path — the harness's exit, same hazard as
SDLmain's; flagged here because it is outside the lane's file list).

## The falsifiable gate: the render is the ask key

The audit's measure for this lane is byte-identical prompts on a `WAGIC_REPLAY` corpus
record. A replayed game makes NO model calls (the seats are passive while loading), so the
base binary could not emit a prompt to compare. Instrument, compile-gated
(`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`, never read in release): `renderProbeDump()`,
called at every turn header in `receiveEvent`, with `WAGIC_GPT_RENDERPROBE=<file>` appends per
seat per turn the narration, `serializeGameState(&tail)` (a fake option block naming the
first own permanents so the #W47 own-effect skip engages), `serializeGameState()`, the A18
delta size, and a self-check line comparing the pair path (`serializeGameStatePair`,
`assemblePrompt(tail, &situation)`) against the one-shot renders. A mirror probe (direct
renders only) was spliced onto a throwaway worktree at 6fc0e601f (removed after).

Result over ALL 42 recorded transcripts in `bin/User/transcripts`, GPT seats
(`WAGIC_AI=gpt`, endpoint refused), base binary vs lane binary:
- **0 of 42 dumps differ** (`[pair-check]`/`[pending-bytes]` lines excluded); 488 render
  points; every replay reaches the same end state on both binaries.
- `[pair-check]`: 488/488 `key same prompt same assemble same` (47 with the own-effect
  skip active, 441 without) — the pair render and the splice overload equal the one-shot
  renders on every board seen.
- Lane binary with every disable flag set (`WAGIC_GPT_BOARDINDEX=0 WAGIC_GPT_SCRIPTMEMO=0
  WAGIC_GPT_TRIM_V1=1 WAGIC_GPT_TRANSLOG_STREAM=0`) vs default: 0 of 42 differ.
- Caveat: no replay reaches the 24 KB narration trim, so L10's byte effect is pinned only in
  PARSETEST (it is the one change that alters bytes, in trim regimes only, behind a flag).

## What shipped (item by item)

- **A18** `mNarrationPending` with the translog off: `writeNarration` passes a NULL delta
  when `mTransLogPath` is empty; the two direct writers (opening hand, turn header) are
  gated the same way; the `writeTransLog` early return clears it. Measure (probe
  `[pending-bytes]`, replay, `WAGIC_GPT_TRANSLOG=0`): base max **7,480 bytes** at the
  turn header, lane **0** on every render point. Translog-on records unchanged (the delta
  is fed exactly as before when the path is set).
- **A19** (the sharing half; the seams are lane M's): `serializeGameStatePair(tail,
  &key)` renders the situation ONCE and returns the prompt variant while writing the key
  variant — the own-battlefield block is the only difference and is re-rendered only when
  the skip set is non-empty (splice by `tellp()` offsets); `assemblePrompt(tail,
  &situation)` splices a pre-rendered situation. Declared in the header's `audit-L` block;
  one-argument forms unchanged. Lane M was told to call these if present. The trivial
  `reasked` half (`:17398`) is in lane M's region — not touched here.
- **A20** `BoardRenderIndex`: one pass over both battlefields per battlefield render
  builds host→attachments (aura via `auraParent`, equipment/fortification via `target`,
  same (player, array) order as the old pair scan, both lists when the two pointers differ
  exactly as the pair test matched both) and card→handle (rank among same-named cards on
  the controller's battlefield, vouched only when `controller()` owns that battlefield, else
  the scan). `describeAttachments`, the entry handle, the `[attached to:]` host and the
  blocked-attacker name read the index. Fortification id resolved once
  (`isEquipmentOrFortification`). Flag `WAGIC_GPT_BOARDINDEX=0`.
- **A23** `ScriptMemo<V>`: nine pure-over-text scanners (`amassCountersFromScript`,
  `counterAddPlusFromScript`, `annihilatorTag`, `drawPunisherClause`,
  `castTriggerDrawCount`, `opponentExtraDrawPerTurn`, `discardPunisherClause`,
  `scriptAbilityDrawCount` (keyed on both inputs), `scriptSelfDrawCount`) memoised by input
  string under a `GptMutex`; the originals are the `...Uncached` bodies, untouched. Flag
  `WAGIC_GPT_SCRIPTMEMO=0`. NOT memoised: `lifeToDamageConverterScript` /
  `lifeLossMirrorScript` (defined at ~22400, lane M's region) and the board scans
  (`drawStepExtrasScan`, `drawPunisherScan`) which are not pure over text.
- **A24** `httpRequestImpl` returns the status (`long * codeOut`) and the first 160 bytes
  of a non-200 body; `gptHttpGet/Post` overloads carry them; `WorkerMain` logs
  `http error <code> from <url>: <body>` once per distinct (url, code) via
  `gptNoteHttpFailure`, stamps `AsyncState::httpStatus`; `pollCompletion` consumes it into
  `mLastHttpStatus`; `noAnswerClassFor(stale, timedOut, hasReasoning, httpStatus)` returns
  `http_error` for any status other than 0/200 (the breaker still wins); the record carries
  `http_status`; the endpoint PROBE logs a rejected `/v1/models` the same way (a failing
  probe disables the seat, so a bad key at the probe used to be silent). Also
  `CURLOPT_MAXREDIRS 3` + `CURLOPT_POSTREDIR CURL_REDIR_POST_ALL` on both curl paths (a
  redirected POST became a bodiless GET). `CURLOPT_XOAUTH2_BEARER` NOT adopted (unverified
  on vitasdk's curl; the header form stands). PARSETEST: 7 rows.
  Bad-key test (stub: public `/v1/models`, 401 on chat — the OpenRouter shape): gpt-log
  `http error 401 from http://127.0.0.1:8298/v1/chat/completions: {"error":{"message":
  "Incorrect API key provided: sk-bad." ...}}` once; 64 records, **29 with `fallback:
  http_error` + `http_status: 401`** (ask 20, attackers 7, blockers 2); the game completed.
- **A49** `gptCurlInit()` (once, game thread: AIPlayerGPT ctor, `gptSpawnWorker`, the
  probes); worker accounting (`gptWorkerStarted/Finished` around every spawn on the desktop
  and Vita branches); `gptShutdownWorkers(2000)` — 50 ms polls, `curl_global_cleanup` only
  when no worker remains — before `main` returns (SDLmain) and before the ONESHOT `exit(0)`;
  the worker's function-local `logMtx`/`lastErr` replaced by `gptLogLineOnce` whose state is
  first-touched on the game thread by `gptCurlInit`. Kill-mid-decision test: see the
  section below.
- **L1** `tribesConsumedByScript`: per-script memo (rebuilt when the type vocabulary
  grows, since `findType(forceAdd)` can add types); `collectTribalTypes` is a union of
  lookups. Shares the `WAGIC_GPT_SCRIPTMEMO=0` flag.
- **L2** `noteChosenNames` builds its key without an `ostringstream`. The event-type gate
  the ledger suggested was DECLINED: `AASetNameChosen::resolve` fires no event of its own
  (the #W50-X comment says so), so any gate moves the narration line to a later event — a
  byte change on the narration for a 0.11% scan.
- **L3** header comment (the "re-entered every tick" wording that steered two audits)
  reworded; `dynamicMagnitudes` guards NULL before dereferencing; the WorkerCtx doc comment
  reunited (padlog block moved above it); `collectMutatePile` walked once
  (`mutatePileTextFor`).
- **L4** `transLogWrite`: one `std::ofstream mTransLog` per seat, opened on the first
  record (after the `-vs-` rename), flushed per record, closed by the game-end record. Flag
  `WAGIC_GPT_TRANSLOG_STREAM=0`. gpt-log's per-line open left as is (rare lines).
- **L10** `narrationTrimKeep(markerLen)` keeps `20000 - marker - 1` so the post-trim log
  is <= 20 KB whatever the digest weighs (was: a >3.9 KB digest re-trimmed on every line);
  `narrationTrimNear(log, line, pending)` counts the pending-phase line and the decoration
  so a cap crossing caused by them no longer trims behind an EMPTY marker. Flag
  `WAGIC_GPT_TRIM_V1=1`. PARSETEST: 7 rows incl. the composed 4 KB-marker case (trims
  fire, none leaves the log above 20,000, one per ~4 KB not one per line).
- **L11** keysalt: `sceKernelGetRandomNumber` (VITA) / `rand_s` (`_WIN32`) before the
  `rand()` fallback; `writeSecretFile` opens `O_CREAT|0600` (POSIX) for keysalt, the Codex
  token store and the sign-in document (Windows keeps stream+chmod).
- **L12** `OptionGptText::rebind`; `GptOptionsList::mUrlRow` re-pointed at `&cfg.urls[0]`
  after `cfg = fresh` in `Reload`.

## Gate numbers (lane binary, `~/.gatelogs/w54-L-*.log`)

- PARSETEST **2483/0** (base 2469 + 14: A24 x7, L10 x7).
- Suite **1221 tests, 2 failed = lifeline.txt + merrow_reejerey.txt** (the known
  concurrency pair, `WAGIC_TESTSUITE_THREADS=8`), **51 AI / 0 failed**, 0 timeouts.
- Replay render diff: 0/42 differ (above). 401 test: above. A18: 7,480 -> 0 bytes.
- Kill-mid-decision (A49): stub endpoint answering every chat request after a 1.5 s
  hang, GPT-vs-GPT selfplay, SIGTERM at 12 s (7 workers started / 6 published at the
  signal, i.e. one inside libcurl), x30 per binary: base **30/30 exit 0**, lane **30/30
  exit 0**. The rare event did not manifest on the base binary in 30 reps, so this run
  does NOT discriminate the guard (rare-event power); what IS observed: the lane binary
  exits ~1.5 s after the signal (it waits out the in-flight worker), the base in ~0.25 s
  (it does not). Harness lessons recorded for the next runner: SIGINT is SIG_IGN for a
  background job of a non-interactive script and SDL installs its handler only over
  SIG_DFL, so the quit must be SIGTERM; find the pid with `pidof <path>` (`pgrep -x`
  truncates comm at 15 chars, `pgrep -f` matches the env/systemd-run wrapper).

## Predictions (falsifiable, for the wave-54 corpus review)

- Any `fallback: empty_reply` record whose seat's gpt-log carries an `http error` line is a
  lane-L bug; a rejected server now always reads `http_error` with `http_status`.
- `describeAttachments` perf share on a fake-endpoint selfplay drops below 0.5% (not
  measured here — see below).
- Corpus prompts are byte-identical to what 6fc0e601f would have rendered except in games
  past the 24 KB narration trim, where the trim marker's tail is up to `marker+1` bytes
  shorter and the marker is never empty.

## What I did NOT verify

- No perf measurement (the audit's 4.6% -> <0.5% claim for `describeAttachments`): the
  change is argued from complexity (O(N) index vs O(N^2) scans), verified for bytes, not
  timed.
- Vita/Windows/Android compile of the new code (`sceKernelGetRandomNumber`, `rand_s`, the
  Vita `gptWorkerEntry` accounting, the JNI `httpRequestImpl` signature): desktop g++ only.
- The `WAGIC_GPT_TRIM_V1` counterfactual in-process (env is read once); the L10 byte effect
  on a real >24 KB game (no replay reaches it).
- The replay gate covers `serializeGameState`, `describeZoneCards`, the scanners, the
  narration and `assemblePrompt`; it does NOT exercise the ask-key path in region 2 (lane
  M) or combat/target prompts that never render outside a live ask.
- L12 under ASAN with a live URL-count change (reasoned fix; no GUI run).
- The Codex (subscription) path never sets `httpStatus` (its errors log via
  `gptLogLineOnce` as before); its `http_error` class is not reachable.
- Two `stale_livelock` records appeared in the 401 game (instant failures re-asking
  under a drifting prompt) — a pre-existing class, noted, not investigated.
