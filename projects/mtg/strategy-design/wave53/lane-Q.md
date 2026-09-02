# Wave-53 lane Q — observability (D10 MED, D24 LOW, D20 LOW)

Base: master `3dfd77d65`. Branch `w53-lane-Q`.
Baseline re-confirmed on this worktree BEFORE any edit: PARSETEST **2121/0**;
suite **1210 (2 failed: lifeline.txt + merrow_reejerey.txt, the known
concurrency-only pair)**, **44 AI / 0**, **0 timeouts**.

## D10 — MED — a 900 s non-answer is now its own class, says so on stderr, and gets one retry

**Root cause of the "the retry path exists and did not engage" half of the docket.**
The corpus's only `retry: 1` record (`125v152` seq 126, `latency_ms` 560,313, choice 3
executed) is the **decode-garbage** retry: its gate is
`!content.empty() && userMsg != mRetryDoneBase && isDecodeGarbage(content)`.
A deadline miss has **empty** content, so it fails the first conjunct — the wall was
structurally unreachable by every retry in the file (the force-close retry needs a
non-empty `mLastReasoning`, which a timeout also lacks). Nothing was misfiring; there
was no branch to fire.

**Changes**
1. `AsyncState::timedOut` — set by the HTTP worker under the same mutex as the body.
   The deadline is told from a refusal by the only fact every transport shares
   (curl, the Android JNI bridge, the Codex SSE reader all report failure as an empty
   body): **elapsed >= 95% of the configured deadline with an empty body**. A refusal
   returns in milliseconds. No transport file was touched.
2. `AIPlayerGPT::noAnswerClassFor(staleLivelock, timedOut, hasReasoning)` — the
   classification as a PURE function, with `noAnswerClass()` delegating.
   Precedence: `stale_livelock` > **`timeout`** > `reasoning_only` / `empty_reply`.
3. Two stderr lines through `DebugTrace` (the same channel that prints
   `-> chose -1 of N`): `AIPlayerGPT: no reply after Ns - one retry`, then
   `AIPlayerGPT: no reply after Ns - heuristic` on the handoff (printed both where the
   spent retry lands and where a retry could not run). N is the DEADLINE, not the
   summed round trip, so the line reads true on both attempts.
4. **ONE deadline retry**, in `pollCompletionRetry`, gated by `mRetryDoneBase` exactly
   like the other two, and only on an EMPTY reply (a reply that came back and merely
   failed to parse keeps its own classes and is not spent on a second round trip).
   Slot key `kTimeoutRetryTag` (a KEY, not prompt text — `buildRequestBody` strips it),
   so the retry re-sends a **byte-identical** ask: same body, same max_tokens
   (the answer-locked 512 cap is explicitly excluded), same deadline.
   *Nothing can be double-consumed:* the ask was never answered, and the staleness gate
   is untouched and still in front — `pollCompletion` keys on prompt equality, and
   `pollCompletionRetry` abandons the pending retry the moment `userMsg != mRetryBase`
   (a drifted board), which is the pre-existing rule for all three retries.
5. `WAGIC_GPT_TIMEOUT` and every default are **unchanged** — no dial moved. Docket
   item (d) (a per-ask deadline scaled to prompt size) was NOT implemented: it changes
   the owner's number in effect, and the docket says the item is observability.
   Also fixed in passing: the Codex `/responses` path sent the untagged `userMsg`
   where the chat path already sent the stripped `baseMsg`.

## D24 — LOW — an unanswered decision now writes a recovery record

`146v125` s282 (`reply: 'method4 * is_ k ind  *'`, `choice: -1`, `chosen_text` null)
said the model did not answer and then said nothing about what did.

`handedToHeuristic(choice, fallback)` (pure) latches at the end of `writeTransLog`
when **nothing from the reply executed** (`choice < 0`) **and** a fallback class was
stamped. The next `writeTransLog` — or `logGameEnd`, so the last decision of a game is
not lost — flushes a `kind: "recovery"` record carrying `recovers_seq`,
`recovers_kind`, `recovers_fallback`, turn/phase/life, and `recovered_by` = the
narration delta, whose head is the heuristic's own action. The delta is **copied, not
consumed**: the following record's `events` field is byte-unchanged.
`recovered_by` is ABSENT (not empty) when the heuristic produced no narration — a pass
— which is itself the finding and is not dressed up as an action.
Cost: one extra record per handoff (wave-52 corpus fallback rate ~2-3%), and
`mTransSeq` now advances over them, so a seq-arithmetic count of decisions must filter
`kind != "recovery"` (`corpus-compare.py` already filters by kind).

## D20 — LOW — MEASURED and DECLINED, not skipped

Measured over the whole `matchups-20260901-223430` corpus (adjacency test per record):
**zero** runs of >= 3 byte-identical ADJACENT event lines remain — the shipped
`NarrationCycleHolder` + `collapsedRunNarration` collapse is already firing on
everything it is entitled to, and `" drew a card"` / `" got a "/" lost a " counter` /
`" created a "` all have working conjugations plus the `(xN)` fallback.
All 110 residual `- Opponent drew a card` repeats are **non-adjacent and cross turn
boundaries** (`1788331...deck146...deck125` seq 330: three draws inside turn 51 — two
Staff of Nin upkeep triggers plus the draw step — separated by `- Phase: Draw`,
`=== Turn 52 ===`, casts and damage). Collapsing them would erase turn structure and
state a count across turns the log never had. The remaining same-line pairs
(`- Opponent discarded Sphinx's Revelation` x2) are BELOW the floor of 3 by design.
**Not cheap, and not correct at this shape** — the docket's "only if cheap" gate says
leave it. Re-open only if a corpus shows adjacent runs >= 3 surviving.

## Files

- `projects/mtg/include/AIPlayerGPT.h` — `noAnswerClassFor`, `mLastTimeout`,
  `mRecoverySeq/Class/Kind`, `flushRecoveryRecord`, `handedToHeuristic`.
- `projects/mtg/src/AIPlayerGPT.cpp` — `AsyncState::timedOut` + worker stamp;
  consume/spawn/refusal/livelock clearing; `noAnswerClassFor`; `kTimeoutRetryTag`;
  the deadline retry + the two stderr lines; `buildRequestBody` tag strip and cap
  exclusion; `flushRecoveryRecord` + the latch + the game-end flush; 9 PARSETEST cases.
- `projects/mtg/tools/gpt-stub-server.py` — `--hang-ms` / `--hang-every` and a
  threading server: the model-free, deterministic TIMEOUT fixture.

## Gates

- **PARSETEST 2130 passed / 0 failed** (2121 base + 9 new; 6 of the 9 are negatives).
- **Suite 1210 tests, 2 failed** (`lifeline.txt`, `merrow_reejerey.txt` — the known
  concurrency-only pair, unchanged), **44 AI tests / 0 failed**, **0 timeouts**.
  Byte-for-byte the wave-52 baseline.
- **Scoped pins for the timeout stamp** (deterministic, no model, `WAGIC_GPT_TIMEOUT=5`,
  memory-capped, logs under `~/.gatelogs/`):
  - *hang-always* (`--hang-ms 999000`, deck109 v deck133): stderr
    `AIPlayerGPT: no reply after 5s - one retry` **37**, `... - heuristic` **2**;
    translog records `fallback: "timeout"` with `retry: 1` and
    `latency_ms: 10004` (= both 5 s attempts summed). On base this record was
    `fallback: "empty_reply"`, no retry, no stderr line.
  - *hang-every-2* (first attempt hangs 6 s, retry answered): records with
    `retry: 1` and **no fallback** — the deadline retry RECOVERED a decision that on
    base was lost to the heuristic.
  - *prose-reply* (stub answers English with no coded line, full game to
    `winner=1 life0=0 life1=22 turn=25`): **242 `kind:"recovery"` records** across the
    two seats, 144 of them carrying `recovered_by`; `recovers_fallback` =
    `unparsed_reply` / `stale_livelock`, the D24 class.
- A live scoped run against `qwen36-35b-a3b` @ `:8084` with `WAGIC_GPT_TIMEOUT=5` did
  NOT produce a timeout (median decision latency 2 s at that prompt size; the harness's
  own feasibility watchdog then killed it, correctly). That is why the pin above is the
  hang fixture: the live path cannot be made to miss a deadline on demand without
  changing the owner's dial to something the corpus never uses.

## Falsifiable predictions

- **D10.** In the next corpus every empty reply whose round trip reaches >= 95% of
  `WAGIC_GPT_TIMEOUT` is stamped `fallback: "timeout"` and carries `retry: 1`, with one
  `no reply after Ns - one retry` and one `no reply after Ns - heuristic` line in that
  game's stderr: **N/N**. `fallback: "empty_reply"` records with `latency_ms` within 1 s
  of the deadline: **0** (was 3). Baka-executed opening-hand keeps caused by a wall
  miss: **0** (was 2) — a retry recovers the decision whenever the queue drains inside
  the second window.
- **D24.** Every record with `choice: -1` and a `fallback` field is followed by exactly
  one `kind: "recovery"` record naming it (`recovers_seq` = that record's `seq`):
  **N/N**, and no record with `choice >= 0` is followed by one: **0 / all**.
- **D20.** A re-measure of the next corpus finds **0** runs of >= 3 byte-identical
  ADJACENT event lines inside a single record (the collapse is not leaking); the
  non-adjacent repeat population persists and is not a defect.

## NOT verified

- **No live 900 s wall was reproduced.** The stamp, the retry and the stderr lines are
  proven against a hang fixture at a 5 s deadline; the arithmetic is deadline-relative
  (95% of `mTimeoutMs`), so it is scale-free, but no run at the owner's real number was
  made.
- **The retry's effect on queue pressure at `-j 21` is unmeasured.** A wall miss now
  costs up to 2x the deadline for that decision. The wave-52 timeouts all happened
  under server queueing, which is exactly when a retry adds load; whether the second
  ask is answered or joins the same queue is a corpus question, not a lane question.
- **`--hang-every`'s counter is global across seats**, so the hang-every-2 probe's
  interleaving between the two selfplay seats is not deterministic; it proved the
  recovered-by-retry shape exists, not a rate.
- **Recovery records were not exercised at a realistic fallback rate.** The prose probe
  is a 100%-fallback pathological run; the record's cost and readability at a 2-3%
  rate is inferred, not observed.
- **Non-curl transports.** The `timedOut` stamp was exercised only through the desktop
  curl path; the Android JNI and Codex SSE paths use the same elapsed-time test but
  were not run.
- D20's decision rests on the wave-52 corpus only; no new corpus was run.
