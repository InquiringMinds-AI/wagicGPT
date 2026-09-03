# Wave-54 lane R — the stall floor stops releasing windows a seat is answering

Base: master `e7e22bd04`. Worktree `worktrees/lanes/w54-R`, branch `w54-lane-R`.
Docket: seat-engine-narration HIGH #1 and seat-146-152-162 headline 1 / E-1.
Amends wave-53 lane AA (`37b7951d3`); AA's softlock fix survives intact.

## The defect, as verified in code

`ActionStack::Update` (`src/ActionStack.cpp`) accumulated the hold budget as
`mHoldSeconds += dt` and compared it to 1,200 for an interactive-AI holder.
**dt is synthetic in every regime this engine actually runs in**, which I
confirmed by reading each producer rather than assuming one:

| regime | dt per tick | 1,200 "seconds" is |
|---|---|---|
| corpus harness (`WAGIC_FASTCLOCK=0.1`, `JGE/src/SDLmain.cpp` `fastClockDt`) | fixed 0.1 | **12,000 ticks** |
| suite worker path (`TestSuiteAI.cpp:2067`, `observer->Update(counter++)`) | 1, 2, 3, … | ~50 ticks (so only the 300-tick conjunct is left) |
| suite main-thread / live game | real wall dt | genuinely 1,200 s |

So the corpus's 468 releases at one distinct value, `12002 ticks`, are the
arithmetic, not a coincidence — and I reproduced that exact number in the
harness (below). The second half is `extendInterruptOffer`
(`include/ActionStack.h:236`): `AIPlayerGPT::decisionPending` calls it every
tick a request is in flight, but it only resets the counters while
`askIfWishesToInterrupt == who`. Once the seat has TAKEN the window the holder
is `observer->isInterrupting`, the keep-alive no-ops, and the seat's only
"still thinking" signal is disconnected from the watchdog about to kill it.

## What shipped

1. **`Player::aiDecisionInFlight()`** (`include/Player.h`), false by default;
   `AIPlayerGPT` overrides it as `!mEndpoint.empty() && asyncBusy()`. A pure
   const query — deliberately NOT `decisionPending(dt)`, which is the Act-loop
   policy hook and has side effects (think-time accounting, keep-alive).
   The floor now asks the holder directly, on **every** branch: no window is
   released out from under an answer that is on its way. This is the fix for
   the disconnected keep-alive, and it needs no keep-alive call at all.
2. **The interactive-AI budget is `std::chrono::steady_clock`**, taken at hold
   acquisition (`mHoldStartMs`, re-armed when `extendInterruptOffer` zeroes it)
   — never dt. The heuristic-AI floor (300 ticks AND 20 dt-seconds) and the
   12-tick loading floor are **unchanged**: that is lane AA's softlock class, a
   seat that can never answer, and moving it would move AA's pin.
3. **`WAGIC_STALL_FLOOR=0`** disables the whole floor (env, read once). Wave 53
   shipped this floor with no disable flag, so the corpus could not A/B it.
4. The release diagnostic (compile-time gated, `_DEBUG || WAGIC_DEVLOGS ||
   WAGIC_TRANSCRIPT_ON`, as before) now prints **wall** elapsed beside the tick
   count: `for 1979 ticks / 20.0 s wall (turn 0 phase 4)`.
5. Suite arm (dev/test code only): `aipending <ticks> <inflight> <interactive>`
   latches one scripted seat as an interactive AI and/or a seat with a call in
   flight and freezes the shared command pump until `ActionStack::mHoldTicks`
   reaches `<ticks>` — or the window is taken away, so a base binary reports
   instead of wedging. `assertinterrupting <p1|p2|none>` pins who owns the open
   window; nothing else in the harness can see it, which is precisely why this
   defect was invisible to 1,226 tests (a released window and a held one leave
   identical zones).

Nothing else in `ActionStack` was touched.

## Fixtures — RED/GREEN evidence

`bin/Res/test/w54R_ai_pending_window_held.txt` (registered, in the gate).
p2 takes a real response window on Hill Giant, then holds it 2,400 ticks with
a call in flight.

* **RED on base** (seeded tree, this fixture + its two suite commands built in,
  `ActionStack.cpp`/`.h` #W54-R hunks reverted, `~/.gatelogs/w54R-base-probe2.log`):
  `wagic: interrupt window held by Player 2 on 'Hill Giant' for 1979 ticks (turn 0 phase 4) - releasing so the game advances`
  then `TESTSUITE assertinterrupting: expected p2 got none` → `==Test Failed !==`.
* **GREEN after** (`w54R-fix-probe4.log`): 2 tests, 0 failed; no release line in
  this test at all.

`bin/Res/test/probe/w54R_ai_wallclock_budget.txt` (probe, **not** registered —
it needs `WAGIC_FASTCLOCK=0.1`, following the existing `probe/` convention for
env-dependent fixtures). An interactive seat holds a window 12,100 ticks with
NO call in flight, i.e. the corpus's own clock and the corpus's own shape.

* **RED on base** (`w54R-base-fastclock.log`): `interrupt window held by
  Player 2 on 'Hill Giant' for **12002 ticks** - releasing` — the corpus
  signature, byte for byte, with no model and no endpoint — then
  `assertinterrupting: expected p2 got none`.
* **GREEN after** (`w54R-fix-fastclock.log`): 1 test, 0 failed, no release line.

**Disable-flag positive control** (`w54R-flagoff-aa.log`): the fixed binary with
`WAGIC_STALL_FLOOR=0` running lane AA's softlock pin reproduces the owner's
softlock outright — 0 release lines, `eot` re-queued 349 times, killed at the
timeout. The flag genuinely turns the floor off.

**Lane AA's pin stays GREEN** and still fires: `w53AA_ai_interrupt_window_stall.txt`
passes, releasing at `1979 ticks / 20.0 s wall`. Across the whole 1,227-test
gate suite there is exactly **1** release line — AA's own. No other test comes
near the floor.

## Gate

Hermetic incremental build, clean link. From `projects/mtg/bin`, detached unit
`w54-R-gate`, memory-capped:

* Suite (`WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 ./wagic`): **1227 tests, 2 failed,
  0 timed out, 51 AI tests / 0 failed**. The 2 are `lifeline.txt` and
  `merrow_reejerey.txt` — the known concurrency-only pair named in the lane
  brief's baseline. 1226 + 1 = this lane's fixture.
* PARSETEST: **2489 passed, 0 failed** (unchanged — this lane changes no parse
  or render string).
* `git diff | /usr/bin/grep -c $'\357\277\275'` = 0.

## Falsifiable prediction for the wave-55 corpus

With this binary and `WAGIC_FASTCLOCK=0.1`, `releasing so the game advances`
falls to **0 on GPT seats** and `dropping stale async answer` returns to the
wave-53 baseline (~63 corpus-wide, i.e. under ~5 per game and uncorrelated with
release lines). The per-seat tell: no seat should record ≤3 opponent-turn
decisions while its counterpart records dozens in the same game (wave 54:
`126v123` 1 vs 71). If releases persist on GPT seats, the floor is exonerated
and lane M's ask cache / lane L's board index are next — `WAGIC_STALL_FLOOR=0`
now makes that A/B a one-env-var run instead of a build swap.

## What I did NOT verify

* **No live corpus, no model call.** Both fixtures arm the seat state
  synthetically; no `AIPlayerGPT` with a real endpoint held a window under this
  binary. The wave-55 corpus is the real gate for the prediction above.
* **No Vita/PSP build** (port builds are owner-initiated). Desktop only.
* **The registered fixture is regime-scoped, honestly.** It is RED on base on
  the suite's main-thread path (real dt, release at ~1,980 ticks / 20 s) and on
  the worker path (dt-as-counter, release at 300 ticks). It never reports a
  false red, but under a hypothetical regime where the base binary would need
  1,200 real seconds it would be inert. The FASTCLOCK probe covers the corpus
  regime; neither fixture covers a real wall-clock 1,200 s expiry, which no
  test can afford to sit through.
* **I did not measure the wave-54 corpus myself.** The 468/488/12002 numbers,
  the per-seat opponent-turn counts and the 23.9 s mean latency are the two
  seat reviews'; I verified the mechanism they name in the code and reproduced
  the `12002` arithmetic in the harness.
* **The 1,200 s allowance itself is unchanged and still unmeasured** — it is now
  1,200 REAL seconds, which is what lane AA intended, but no live slow-endpoint
  run has tested that boundary.
* I did not audit whether any OTHER dt-denominated timer in the engine has the
  same wall-decoupling bug; the brief scoped me to this floor.
