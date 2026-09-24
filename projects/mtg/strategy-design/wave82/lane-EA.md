# Wave-82 step one — lane EA (parser, transport, harness, tools)

Branch `w82-lane-EA` in `worktrees/lanes/w82-EA`, base 35e1e82dc. Six commits, one per item:

| item | commit | files |
|---|---|---|
| #14 early-check | dfeb9c77b | tools/corpus-early-check.py, tools/intent-collapse-metric.py |
| M13 guide match | 5212830f3 | tools/check-reply-instructions.py |
| #13 paired A/B | ac4598bd9 | src/GameStateDuel.cpp, tools/baka-ab.sh, tools/baka-ab-compare.py |
| H7 PLAN-line answer + re-ask | 90e9f892b | src/AIPlayerGPT.cpp, src/AIPlayerGPTSeams.cpp, AIPlayerGPTSelfTest.cpp, tools/w82-ea-unparsed-reask-probe.sh |
| H6 thinking stays on | 11267815c | AIPlayerGPT.h, AIPlayerGPTInternal.h, AIPlayerGPTSelfTestAccess.h, AIPlayerGPT.cpp, AIPlayerGPTSelfTest.cpp, AIPlayerGPTTranslog.cpp, AIPlayerGPTTransport.cpp, tools/gpt-stub-server.py, tools/regime-gate.py, tools/w82-ea-thinking-retry-probe.sh |
| M3 async_drop record | 0843ff9bd | AIPlayerGPT.h, AIPlayerGPTInternal.h, AIPlayerGPTSelfTestAccess.h, AIPlayerGPTSelfTest.cpp, AIPlayerGPTTranslog.cpp, AIPlayerGPTTransport.cpp, tools/corpus-stats.py |

**Gate (worktree binary at 0843ff9bd):** build clean, both link guards OK (`check-ctor-init: OK (123 file(s))`, `check-reply-instructions: OK (5 source file(s), 38 guide(s))`). PARSETEST **7332 passed / 0 failed** (baseline 7314 + 8 H7 + 6 H6 + 4 M3). Suite THREADS=1 (`w82-EA-suite`, `~/.gatelogs/w82-EA-suite.log`): **1314 tests (1 failed), 82 AI tests (0 failed)** — the one `==Test Failed !==` is `intrepid_adversary_repeated_payment.txt` (the known flake); `==Test timed out` 0. `git diff | grep -c $'\357\277\275'` = 0 on every commit. No CLAUDE.md, no pilot config, no corpus, no VPK.

---

## 1. H7 — `PLAN: 4 (Cast nothing right now)` is the answer; unreadable + deadline left = re-ask

**False before:** deck50 vs 125 seq41 (the corpus's only fallback) — a reply that was the single line `PLAN: 4 (Cast nothing right now)` was `fallback: unparsed_reply` and seq42 `executed_by: heuristic`, with 891 s of the 900 s deadline unspent (`deadline_pct` 8.9). Two defects: the reader did not read a PLAN line whose payload is exactly `<row> (<that row's short name>)`, and the plain-unparsed class was the one exit that went straight to Baka (the named-row and no-pass lanes already re-asked; #W71-BO R9 had deleted the label-missing re-ask).

**Now:** (a) `AIPlayerGPT.cpp` — the label-less reader accepts a labelled line whose content is exactly `<row> (<short name>)` (or bare `<row> (<short name>)`) matching exactly one row, when no action-label line exists; the ruling holds — no plan exists for the answer to precede. (b) `AIPlayerGPTSeams.cpp` — at the ask and priority seams, `choice < 0 && !content.empty() && !namedRowFail && !staleEcho && retryFitsInDeadline(...)` re-asks once with a `[RE-ASK] No answer line could be read in that reply. Write your PLAN line, then on its own line "CHOICE: <n> (<that row's name>)"` correction; fallback tag `unparsed_reask`, parse note `unparsed_reask_recovered` / `unparsed_reask_unanswered`; the heuristic answers only when the deadline is gone (same remainder rule as the transport retry).

**PARSETEST** (`[#W82-EA] H7`, 8 cases): GREEN the verbatim seq41 reply; GREEN case-insensitive label / indentation / CR / trailing courtesy line; MUST-NOT-MATCH a PLAN-line answer beside a bare answer line (two answers); two PLAN-line answers; an answer in the plan slot followed by a PLAN line (answer-before-plan, rejected); a PLAN payload that is prose around a parenthetical; the labelled line wins over a PLAN-line payload; a bare action line with nothing before it is still refused.

**Live (stub, `tools/w82-ea-unparsed-reask-probe.sh`, `WAGIC_GPT_STUB` offline transport, 123 vs 125, 240 s each, `~/.gatelogs/w82-EA-h7-probe.log`):**
- `recover` (stub alternates unreadable / unreadable / `PLAN: develop.\nCHOICE: 1 (x)`): ask 145 + priority 56 windows; fallbacks `unparsed_reask` 76+26, `unparsed_reply` 4+5; notes `unparsed_reask_recovered` 48+17, `_unanswered` 4+5; recovery records `reask` 48+17 vs `heuristic` 4+5. The re-ask fires and recovers; the heuristic gets only what the re-ask could not read.
- `exhaust` (every reply unreadable): 818+225 windows; `unparsed_reask` 598+169, then `unparsed_reply` 220+56 → heuristic 293+57. One re-ask per window, never a loop.

**Weakest evidence:** the live re-ask is proven through the offline stub transport, not the curl path (same seam code, different transport); the deadline-remaining predicate is exercised only with the stub's ~0 ms latency (always "fits"), so the "heuristic only when the deadline is gone" branch is proven by PARSETEST/logic, not by a live slow reply.

## 2. H6 — the reasoning-budget retry keeps thinking ON

**False before:** wave-81 corpus, 15 records `retry: 1` all of shape `thinking: on`, `max_tokens_reasoning: 0`, `reasoning_budget_hit: true`, `reasoning_forced_close: true` (deck125 vs 152 seq23 20,683 reasoning chars; vs 162 seq300 26,808). The phase-2 "forced close" re-asked as an assistant PREFILL of the truncated trace with `</think>` injected and `enable_thinking:false`, `max_tokens 400` — the native channel switched OFF under a regime of on, and the record said `thinking: on` (the regime, not the request: "a true statement in the wrong scope").

**Now** (`AIPlayerGPTTransport.cpp` / `AIPlayerGPT.cpp::buildRequestBody`): the default retry is the SAME question again with thinking on and `w82RetryReasoningBudget(phase1) = 2 × the allowance phase 1 hit` (6000 → 12000; unknown → 2 × kDefaultReasoningBudget, never 0); resolved like a phase-1 request (raised reasoning half + the seam's ordinary answer ceiling, not answer-locked). The prefill close survives ONLY behind `WAGIC_GPT_FORCECLOSE_PREFILL=1` (disable flag for an A/B) or a thinking-off regime. No third leg: `userMsg != mRetryDoneBase` blocks a re-arm, so a second budget hit hands to the heuristic. Records: `thinking` is now the flag the REQUEST carried (`mLastRequestThinking`; records with no round trip keep the regime), plus `retry_thinking: on|off` and `phase1_reasoning_chars` (the trace the thinking retry did not prefill back). `regime-gate.py` comment updated (the `max_tokens_reasoning 0` exemption now names the legacy close only). Stub: `--reasoning-only-first` answers the first request of each distinct prompt as a budget hit (empty content, reasoning trace, `finish_reason: length`), the retry normally.

**PARSETEST** (`[#W82-EA] H6`, 6): budget doubling; unknown allowance raises from the default never to 0; `w82RetryKeepsThinking(on, no flag)` true / (on, flag) false / (off, ·) false; the retry plan is 12000 + phase 1's answer ceiling; the request is sent with `enable_thinking:true`; the legacy L9 echo case re-pinned to the legacy path.

**Live (`tools/w82-ea-thinking-retry-probe.sh`, real curl transport to the local stub, 240 s per variant, `~/.gatelogs/w82-EA-h6-probe.log`):**
- default: every retry request SENT carried `enable_thinking: true`, no prefill, no `continue_final_message`, `max_tokens 12288` (= 12000 + 288); **117 retry records, all `thinking: on`, `retry_thinking: on`, `max_tokens_reasoning: 12000`, `reasoning_budget_hit`, `reasoning_forced_close`, `phase1_reasoning_chars > 0`, `reasoning_chars > 0`**; 112 with `choice >= 1`, 5 without (the stub's fixed answer row not legal on that window).
- legacy (`WAGIC_GPT_FORCECLOSE_PREFILL=1`): 222 requests `enable_thinking: false`, prefill, `max_tokens 400`; 221 records `thinking: off`, `retry_thinking: off`, `max_tokens_reasoning: 0` — the wave-81 shape, now truthfully labelled off.

**Weakest evidence:** the raised budget is a policy number (2×), chosen by the generous-limits ruling, not measured against the pilot — whether 2× clears the 20-27k-char traces is for the next corpus (`phase1_reasoning_chars` vs `reasoning_chars` on the retry records answers it). `gptForceCloseSupported(mEndpoint)` still gates the retry off hidden-trace endpoints (api.openai.com) — a thinking re-ask needs no trace, so that gate could be relaxed; not done (out of scope, no evidence).

## 3. M3 — `async_drop` side record

**Missing before:** gameend `async_drops` summed 52 over 42 seats, and **0 records in the corpus carry `async_drop_events`** — the D4 stamp rode "the next record of the drop's OWN window" (#W82-A L10), and a stale drop by definition means the seat moved past that window, so the stamp was cleared unwritten every time. The 52 could be classified only from the harness stderr: **37 `casting arm; the question (or turn/phase) moved) [outcome: re-asked]`, 15 `casting arm; the question and board moved) [outcome: re-asked]`, 0 land-arm, 0 give-ups** — every drop was an answer bought and re-bought after the decision moved on.

**Now:** `writeAsyncDropRecord` writes one `kind: async_drop` record AT the drop (`AIPlayerGPTTranslog.cpp`, body pure `asyncDropRecordJson`): `event` (= running `mAsyncDropsGame`), `arm`, `seam` (`mRequestSeam`), `why` (the slot-key drift kind), `outcome` (`re-asked` / `gave-up-to-heuristic`), `discarded_bytes`, `discarded_parsed`, `discarded_content` (head 600 of the paid-for reply's content — its answer line), `discarded_content_chars`, `discarded_reasoning_chars`, `window_seq`, `window_record_seq`, `turn`, `phase`, `seq`. Takes no window ordinal (like `hold_event`). The stamp list and `mAsyncDropStampsSeq` are removed; gameend `async_drops` == number of these records. `corpus-stats.py`: census selftest pins `async_drop` as a no-round-trip kind (explained with no edit, per the derived-census rule) and a new `ASYNC DROPS:` section lists records by arm/seam/why/outcome with discarded sizes and reconciles against the gameend sum — on the wave-81 corpus it prints `ASYNC DROPS: 0 records (gameend async_drops 52 - MISMATCH: a drop left no record)`, which is the defect stated; CENSUS CHECK still RECONCILES (2122 vs 2349 = hold_event 226 + recovery 1). `corpus-early-check.py` already lists `async_drop` among side-record kinds (from #14).

**PARSETEST** (`[#W82-EA] M3`, 4): the record's fields from a real envelope (content + 1234-char reasoning); empty body → size 0, `why` `unknown`, give-up outcome named; unparseable envelope never throws; long content keeps 600 + `...` and the full length.

**Weakest evidence:** no live record was produced — a stale drop needs the board to move under an in-flight request, which neither stub fixture induces; the writer is one call at the existing drop site with the pure body pinned, so the risk is a wrong `seam` (`mRequestSeam` is set at the top of `pollCompletionRetry` for the current decision). The next corpus's `ASYNC DROPS:` line either reconciles or says MISMATCH.

## 4. Bug-list #14 — early-check counts windows by seam

**False before:** on the wave-81 corpus the base tool printed `seats 42 asks 0 fallbacks {}` / `no window-loop signature` — it counted `kind == 'ask'` and every decision has been `kind: window` + `seam` since P11, so the loop tripwire was blind for a whole corpus.

**Now:** `seats 42 windows 2122 by seam {'ask': 1580, 'attackers': 79, 'blockers': 20, 'bottom': 10, 'discard': 42, 'order': 3, 'priority': 379, 'reveal': 9} fallbacks {'unparsed_reply': 1}` and `WINDOW-LOOP SUSPECT: 5 seat(s)` (deck123-0x565 26 windows in one phase, deck152-0x565 17, …) — real numbers, and it now names what the wave-81 late check would have wanted early. Selftest OK (`hold_event`/`forced_close`/`async_drop`/`menu_single_outcome` are side records, not decisions). `intent-collapse-metric.py` reads the window shape too. Grep of tools/ found no other reader keyed on the old kinds.

**Weakest evidence:** the 5 "suspects" are the tool's threshold on a corpus that ran to completion — the threshold's calibration (26 windows in one Draw phase) is the reviewer's call, not proven here.

## 5. M13 — `strategy.txt` is a guide

**False before:** `GUIDE_SUFFIX = ("_strategy.txt",)` — a staged `wave-<N>/deck<N>/strategy.txt` got zero coverage and the check passed vacuously. **Now:** `is_guide_name()` matches both spellings, and the guide count prints on every run, OK and FAILED (`check-reply-instructions: OK (5 source file(s), 38 guide(s))` at link time). **Weakest evidence:** no staged `strategy.txt` exists on this branch to fail against; the change is a two-line predicate.

## 6. Bug-list #13 — paired heuristic A/B

**False before:** `baka-ab.sh` seeded every game by launch second (process `srand(time(0))` + the observer's `mSeed = time(0)`), so accepted and candidate never played the same game and a 10-point shift at n = 120 was indistinguishable from sampling.

**Now:** `GameStateDuel.cpp` — `WAGIC_SELFPLAY_SEED=<n>` pins `initRand(n)` and `game->resetSeed(n)` before any draw (unset/0 = clock, unchanged); `baka-ab.sh --paired` draws one seed per game (records it in the `seed` column of results.tsv, `WAGIC_STALL_FLOOR=0`), `--paired=<accepted-outdir>` replays that run's seeds game for game; `baka-ab-compare.py --paired` joins on (d0, d1, rep) with equal seeds and judges per game (concordant / discordant, same turn), with selftest OK.

**Paired proof (`~/.gatelogs/w82-EA-paired`, ONE binary, 20 games, acc = fresh seeds, cand = replay):** results.tsv identical on 19/20 lines; the compare tool: `20 game(s) joined … equal seeds; 19 concordant (19 also on the same turn), 1 DISCORDANT` — `123v125 rep 1 seed 64990388: winner=0 turn=38 -> winner=1 turn=45`. Stderr traces, after stripping pointer addresses (`0x…`) and ms timings, are **identical for 19/20 pairs** — trace-identical, not merely same-outcome. The 20th replayed 3× on the same binary: outcomes 45/41, 38/75, 45/41 — **bimodal on its own seed**; first divergence is Baka's `Should I play Sphinx's Revelation? shouldPlayPercentage = 73` appearing in one run and not the other, i.e. a candidate-order/address residual in the heuristic, the class the compare tool's `--help` already names (and why it says replay 3× before calling a discordant game a regression). Not the seed pin: the pinned seed is honoured in all three runs.

**Weakest evidence:** that residual — ~1 in 20 seeds is bimodal on one binary, so a paired gate must replay discordant seeds (the tool says so) rather than treat each as signal; its root (which container iteration in Baka is address-ordered) is not chased here.

---

**Not done / for the orchestrator:** the full A/B gate (brief: 20 paired games is the proof); the `gptForceCloseSupported` relaxation noted under H6; the Baka address-order residual under #13.
