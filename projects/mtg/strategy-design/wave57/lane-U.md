# Wave 57, lane U (BLOCKING hotfix) — the unbounded in-flight model call

## Verdict up front

**The defect is real, reproduced on the desktop, and fixed.** Lane S eliminated
the counter-spell shapes and named two structurally watchdog-blind states plus
one unbounded one. This lane takes the unbounded one: while the LLM seat has a
model request in flight, the interrupt window it holds had **no wall at all**,
so a round trip that never comes back freezes the duel for ever — the owner's
vpk16 report shape exactly (frozen screen, immovable selector, turn indicator on
the opponent during his own turn, pause menu still alive). The exemption is now
**bounded by the request's own deadline**, at two layers, with a disable flag.

## The mechanism, end to end

1. `AIPlayerGPT::decisionPending` (`src/AIPlayerGPT.cpp` ~23852) calls
   `stackLayer()->extendInterruptOffer(this)` on **every tick** the seat is
   `asyncBusy()`.
2. `ActionStack::extendInterruptOffer` (`include/ActionStack.h` ~245) zeroes
   `mHoldTicks`, `mHoldSeconds` **and `mHoldStartMs`** whenever
   `askIfWishesToInterrupt == who` — i.e. the W54-R stall floor's wall clock is
   re-armed from zero by the very seat it is watching.
3. `ActionStack::Update` (~1636) reads `holder->aiDecisionInFlight()` and, when
   true, exempts the holder from BOTH budgets (`mHoldTicks >= 300` and the
   1,200 s interactive wall).
4. `aiDecisionInFlight()` is `!mEndpoint.empty() && asyncBusy()`, and
   `asyncBusy()` is `status == 1` — **a request was LAUNCHED**. It never says
   the request will come back. `AsyncState::status` only leaves 1 when the
   worker publishes.

So a worker that never publishes — a socket the console's network stack never
fails, a server that accepts and never answers past curl's own timeout, a native
`sceKernel` thread that dies mid-round-trip — pins `status == 1` for the rest of
the process. Steps 1-3 then hold the window for ever, the game loop keeps running
(hence a working pause menu), `DuelLayers::CheckUserInput` discards every human
key while `isInterrupting` names the AI (hence the dead selector and the wrong
turn indicator), and no seat ever advances the phase. The owner's session was
signed in to a live endpoint (`gpt-log-vpk16.txt`: `signed in to ChatGPT (plan:
plus)`), which is exactly the configuration in which this path is live.

## The fix, and the bound's arithmetic

The exemption is CORRECT for a live call and had to stay: wave-55 corpora hold
legitimate 900 s decisions, and wave-54 measured what cutting them costs (468
windows released out from under in-flight calls, the affected seats' opponent-turn
decision surface at ~zero). So the bound is **derived from the request's own
deadline**, never a fixed number.

`AIPlayerGPT::inFlightAbandonMsFor(D)` (pure; pinned by PARSETEST):

```
grace  = clamp(D / 2, 1 s, 30 s)
bound  = D + grace          # ONE request
```

| deadline `D` | grace | one request abandoned at | window backstop (`2D + grace`) |
|---|---|---|---|
| 600 s (`GptSettings` default, and the Vita's) | 30 s | **630 s** | 1,230 s (20.5 min) |
| 900 s (wave-55 corpora) | 30 s | 930 s | 1,830 s |
| 420 s (the thinking-mode floor) | 30 s | 450 s | 870 s |
| 5 s (the configured floor) | 2.5 s | 7.5 s | 12.5 s |

Two layers carry it:

**(a) `AIPlayerGPT::reapWedgedRequests` — the seat abandons the request.**
Called at the top of `pollCompletion`, over **both** async arms (the launch guard
is `asyncBusy()`, so a wedged land-drop request blocks every casting ask and
would never be looked at by a poll of the casting arm). For an arm past `bound`:

* `++state->gen`, then status 0 / response, prompt, slotKey cleared, `timedOut`
  and `httpStatus` reset — **exactly the shape the refused-worker branch leaves
  behind** (`AIPlayerGPT.cpp` ~10430);
* ONE line to the gpt log, per event:
  `model call abandoned after N s (deadline D s): answering this decision with
  the heuristic AI`, plus a dev-gated stderr companion naming the arm and the
  bound;
* `mAbandonedInFlightSecs` latched, so the translog record for that decision
  carries `fallback: abandoned_in_flight` and `abandoned_after_s: N` (it
  overwrites the caller's class deliberately — the caller only knows the reply
  was empty, and `empty_reply` already means five other things);
* a player-visible notice (`no reply from the model - playing this one with the
  built-in AI`);
* returns 0 with empty `content`, so THAT decision is answered by the seam's
  existing heuristic fallback in this tick, and the window releases normally.
  The deadline retry does **not** fire (it is gated on `mLastTimeout`, which an
  abandonment clears): an abandoned call is not a wall miss.

**The late write is safe, on both platforms.** `WorkerCtx` already owns its own
`shared_ptr<AsyncState>`, so abandoning frees nothing and clears nothing the
worker can be writing; the worker's publish takes the same mutex and now checks
`state->gen != ctx->gen` first, dropping the body. This matters most on Vita,
where the worker is a native `sceKernelCreateThread` the game can neither join
nor cancel (`GptMutex` is used rather than `std::mutex` for the same reason —
`std::mutex` is a no-op there). Without the generation stamp the abandonment
would have been a real hazard: the slot returns to idle, the next decision
re-arms it with a NEW prompt, and the old worker's late publish would land under
that prompt and be consumed as its answer — a stale answer against a moved
board, which is precisely what the existing stale-drop gate cannot catch (it
keys on prompt equality).

**(b) `ActionStack::Update` — the backstop.** `2 * deadline + grace` (the
request plus its one retry, each with its own launch time, plus the grace),
measured on a NEW anchor `mHoldInFlightSinceMs` set when the holder first goes
in flight on this stack object and reset only by a change of holder, a change of
the top stack object, or the call landing. It is a second field on purpose:
`extendInterruptOffer` zeroes `mHoldStartMs` on every tick the seat is being
offered the window, and the seat calls it from `decisionPending` on every tick it
is thinking — so **the only wall clock the old floor had could be reset for ever
by a seat whose request was already dead**. That is the `extendInterruptOffer`
audit the brief asked for: the zeroing itself is right (it is the keep-alive for
a seat being offered a window, and W54-R put it there for a measured reason), and
nothing about it changes; what changes is that the in-flight bound no longer
rides a clock the seat can rewind. A seat reporting `decisionDeadlineMs() == 0`
(every non-LLM seat) keeps the old unbounded exemption — there is no arithmetic
to bound it with, and it has no transport that can wedge.

**Disable flag**: `WAGIC_INFLIGHT_BOUND=0` turns off BOTH halves and restores the
pre-lane behaviour, so "was it the new bound?" is one env var on a shipped
binary, and the defect stays reproducible on the same binary as a positive
control (used below). Each translation unit keeps its own static copy of the
flag reader — not every platform build compiles both against a shared one.

## Which repro I could reach

| # | setup | result |
|---|---|---|
| 1 | **Suite fixture** `w57U_ai_inflight_call_abandoned.txt`, base `0a5668074` | **RED**: frozen at `aipending`, **zero further output for 40 minutes**, no progress of any kind. The suite's runaway cap (1,000,000 updates) would eventually report `==Test timed out (game never ended)==`, but the loop runs at ~100 updates/s in this state, so that verdict is hours away; the freeze itself is the RED, and it was killed rather than waited out. |
| 2 | same fixture, **fixed binary** | **GREEN in 20 s**: `interrupt window held by Player 2 on 'Hill Giant' for 239 ticks / 2.431 s wall with no progress - releasing`, the game advances to the declared UNTAP. |
| 3 | same fixture, **fixed binary + `WAGIC_INFLIGHT_BOUND=0`** (positive control on ONE binary) | frozen again, 0 releases, killed at 90 s. The flag really turns the bound off. |
| 4 | **desktop selfplay**, `tools/gpt-stub-server.py` + `WAGIC_AI=gpt`, deck131 vs deck2, `WAGIC_GPT_TIMEOUT=5`, `WAGIC_GPT_WEDGE=1` (the worker publishes NOTHING), fixed binary | 4 abandonments at 7 s each, each answered `-> chose -1 of 2` by the heuristic, pre-game completes, **turn 1 begins**. |
| 5 | as 4 **+ `WAGIC_INFLIGHT_BOUND=0`** | **0 abandonments, the pre-game phase never completes**, killed at 100 s. The duel is dead at the first decision. |
| 6 | **desktop selfplay against a listener that ACCEPTS and never answers** (`gpt-stub-server.py --hang-ms 999000`, `WAGIC_GPT_TIMEOUT=5`), fixed binary | 0 abandonments, **19 `no reply after 5s`** lines, pre-game completes. |

Run 6 answers the brief's question directly: **a TCP listener that accepts and
never responds does NOT reach this defect.** curl honours `CURLOPT_TIMEOUT`, so
the worker publishes an empty body at the wall and the round trip lands on the
existing deadline path (one retry, then the heuristic). A *refused* connection is
further still from it — that is the synchronous transport-failure path. The
in-flight abandonment is reachable only when the transport fails to honour its
own deadline or the worker never publishes at all, which on the desktop needs an
emulator: `WAGIC_GPT_WEDGE` (dev builds only, `#if defined(_DEBUG) ||
defined(WAGIC_DEVLOGS)`), the exact counterpart of the existing
`WAGIC_GPT_NOTHREAD`. The suite cannot reach it at all — it builds `TestSuiteAI`
seats, never an `AIPlayerGPT`, so no suite fixture can own a real transport;
fixture 1 therefore pins the ActionStack backstop through `aipending`, which now
takes a fourth argument (the per-request deadline in ms) so a fixture can state a
tiny deadline instead of waiting out an LLM seat's 20 minutes.

## What shipped

* `src/AIPlayerGPT.cpp` / `include/AIPlayerGPT.h` — `AsyncState::gen` +
  worker-side publish guard; `inFlightAbandonMsFor` (pure) /
  `inFlightAbandonMs` / `reapWedgedRequests`; the `pollCompletion` hook; the
  `abandoned_in_flight` translog stamp with `abandoned_after_s`;
  `WAGIC_GPT_WEDGE` (dev-gated); the `WAGIC_INFLIGHT_BOUND` flag reader.
* `src/ActionStack.cpp` / `include/ActionStack.h` — `mHoldInFlightSinceMs` and
  the `2D + grace` backstop on the in-flight exemption; the same disable flag.
* `src/TestSuiteAI.cpp` / `include/TestSuiteAI.h` — `aipending`'s fourth
  argument (`deadline_ms`) and `TestSuiteAI::decisionDeadlineMs`.
* `bin/Res/test/w57U_ai_inflight_call_abandoned.txt` + `_tests.txt` registration.
* 7 PARSETEST cases pinning the bound's arithmetic (including two NEGATIVE ones:
  no deadline -> no bound, and the bound is always strictly past the deadline).

## Prediction (falsifiable)

If the owner's vpk16 softlock was the unbounded in-flight window, the next
console build cannot freeze on it for longer than 630 s per request / 1,230 s per
window at the shipped 600 s deadline; the seat will play that decision on the
heuristic and the duel will advance, and `gpt-log.txt` will carry a
`model call abandoned after N s` line with the translog record stamped
`abandoned_in_flight`. If a report of the same shape arrives with **no** such
line in the log, the freeze is NOT this defect and the remaining suspects are
lane S's two watchdog-blind states (a menu its owner cannot answer, or a stranded
target chooser — docket D34, lane F).

## Gate (worktree `worktrees/lanes/w57-U`, branch `w57-lane-U`, base `0a5668074`)

| leg | baseline (measured on this worktree) | this lane |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1241 / 0 failed | **1242 / 0 failed** (+1 = the new fixture) |
| AI tests (same run) | 57 / 0 | **57 / 0** (the fixture is not an `[AI]` test) |
| `Test timed out` occurrences | 0 | **0** |
| PARSETEST | 3305 / 0 | **3312 / 0** (+7 = the bound's arithmetic) |

## NOT verified

* **No Vita or PSP build, and no console run.** The memory-safety argument for
  the late worker write is a reading of the ownership (`WorkerCtx` holds a
  `shared_ptr`; the publish is under `GptMutex`; the abandonment frees nothing
  and only bumps an integer) plus the desktop wedge repro — it is NOT a Vita
  observation. The one Vita-specific claim that would need hardware to falsify:
  that a `sceKernel` worker whose round trip is wedged is still able to take
  `GptMutex` when it eventually returns.
* **The owner's softlock is still not reproduced on its own hardware**, and this
  lane does not prove it was this defect. Lane S's open question stands and is
  the cheapest discriminator: **did the seat show "opponent is thinking", and did
  the patience prompt (60 s default) ever appear?** If it did not appear, the
  seat was not in flight and this fix does not address his freeze.
* The abandonment of the **non-polled** arm gets a gpt-log line but no translog
  record — the decision it belonged to is long gone by then. Deliberate; noted so
  a harvest that counts `abandoned_in_flight` records does not read it as the
  total.
* `WAGIC_GPT_WEDGE` was only exercised with the wedge on for EVERY request. A
  mixed run (some wedge, some answer) is not tested; the generation stamp is
  what that would exercise, and it is pinned only by the code path, not by a run
  in which a late publish actually raced a re-armed slot.
* Nothing here touches lane S's two watchdog-blind states (`menuObject` non-NULL,
  a live target chooser). If the owner's report is one of those, this lane's
  bound never gets a chance to fire — `ActionStack::Update` returns before the
  stall floor on the `menuObject` branch.
