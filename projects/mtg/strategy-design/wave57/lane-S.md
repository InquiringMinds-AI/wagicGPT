# Wave 57, lane S (BLOCKING hotfix) — the Vita softlock after a countered spell

## Verdict up front

**NOT REPRODUCED. No engine change shipped.** Ten scripted/AI variations of the
reported shape (human-role seat casts into the AI seat's window, AI counters it)
all complete and advance the game, on the current master binary. Per the lane
rule, a speculative fix was not shipped. What DID ship is the instrumentation and
the harness capability that make the next attempt cheap, two negative-control
fixtures, and the analysis below, which narrows the softlock to two structurally
watchdog-blind states and names the one ownership hole that can produce them.

## The report and the physical evidence

Owner, vpk16 (= master `4366e8eec`), 2026-09-03: *"in the last match, i
softlocked after the opponent canceled my spell."* Follow-ups: *"my screen was
frozen, unable to move the selector, and the turn indicator was opponent, despite
being my turn"* and *"No, the menu button works. I used it."*

* `psp-work/logs/20260903-vpk15/memlog-vpk16.txt`: `untap turn=6` at t=465, one
  324 ms update frame at t=475, one 1104 ms update frame at t=495, then nothing —
  and **no `untap turn=7`**. The slow-frame emitter (`JGE/src/Vitamain.cpp`,
  `vitaFrameSample`) caps at **5 lines per turn** and only logs frames over
  100 ms; turn 6 used 2 of its 5. So the silence after t=495 is *not* the cap:
  either every later frame was fast, or none completed.
* The in-game menu opened and he quit through it ⇒ **the game loop was alive**.
  This is a LOGIC STALL, not a pegged-CPU hang.
* `transcript-1788467289-player_deck2-vs-ai_baka_deck131.txt`: the last recorded
  action is the AI seat's own `p2.next 1 12`. `userRequestNextGamePhase` logs
  only EFFECTIVE requests, so that request was ACCEPTED — a `NextGamePhase` went
  on the stack and **never resolved**. No human action is recorded after it.
* `gpt-log-vpk16.txt` for that session contains exactly one non-art line:
  `signed in to ChatGPT (plan: plus)`. There is **no** `worker thread refused`
  line, and that line is written once per process on the refusal path
  (`AIPlayerGPT.cpp`, `gptSpawnWorker` failure). So on this build the Vita seat
  was **not** on the thread-refusal fallback the lane brief assumed — it had a
  live endpoint. (Recorded as a correction, not as a cause: see "Not ruled out".)

## Where the game can stop, mechanically

`ActionStack::Update` is the only thing that can resolve that `NextGamePhase`.
It reaches its priority block only if it gets past three gates. Two of them are
the softlock's only structural homes:

1. **`observer->mLayers->actionLayer()->menuObject` is non-NULL** — Update
   returns EARLY, before `askIfWishesToInterrupt` is recomputed and before the
   W53-AA stall floor at the bottom of the function. So a menu that its owner
   cannot answer freezes the game with **no watchdog running at all**.
2. **a live target chooser** — the priority block is guarded
   `if (getLatest(NOT_RESOLVED) && !tc)`, so nothing is offered and nothing
   resolves while `observer->getCurrentTargetChooser()` is non-NULL. Worse,
   `GameObserver::Update` then *pins* the acting seat to the chooser's owner:

   ```
   if(getCurrentTargetChooser() && ->Owner && player != ->Owner)
       if(->Owner != currentlyActing()) { player = ->Owner; isInterrupting = player; }
   ```

   An orphaned chooser therefore re-asserts `isInterrupting` **every tick**, which
   the stall floor's only remedy (`cancelInterruptOffer`, which merely NULLs
   `isInterrupting`) cannot undo.

Both states produce exactly the reported symptom set: the phase/turn indicator
names the pinned seat, `DuelLayers::CheckUserInput` is called with `isAI=1` and
**discards every human key** (so the selector will not move), the game loop keeps
running (so the pause menu still works), and no seat ever advances the phase.

## The ownership hole that can create an orphan (unproven as the cause)

`ActionStack::cancelPendingChoice()` (wave-56 lane Z, `3b1eb7f8e`) deletes
`observer->targetChooser` and cancels `ActionLayer::currentWaitingAction` **with
no ownership check**. Lane Z found this empirically on one path — installing the
same net in `cancelInterruptOffer` killed the ACTIVE caster's chooser and took
`counter_unless_pay_x` and `spell_blast_counter_matching_mv` red — and removed it
there, reasoning that `endOfInterruption` is safe "because it only runs once a
seat has TAKEN the window, so the pending choice is that seat's own."

That reasoning does not hold for two of `endOfInterruption`'s callers:
`ActionLayer::doReactTo` and `ActionLayer::ButtonPressedOnMultipleChoice` both
call `stackLayer()->endOfInterruption(false)` on the **`kCancelMenuID`** branch —
an abilities-menu Cancel, which is not "a seat took the window" and is reachable
from the AI's own menu handling and from the suite's unanswered-menu default.
The chooser is observer-GLOBAL; `isInterrupting` at that moment need not name the
seat that owns it. This is the lane-Z residual and the docket's **D34**, and it
is the only mechanism found that can strand a chooser owned by seat A while seat
B's window closes.

**It is not shipped as a fix here because no fixture could be built that reaches
it.** The reason is itself informative: while a chooser is live, the priority
block is skipped, so the *other* seat can never be offered a window in the first
place — every scripted route to "owner != window holder" is closed by the same
guard. The state is reachable only if something ELSE (a menu, an AI click
choreography spanning ticks, a stale `isInterrupting`) breaks that invariant
first. Finding that "something else" is the next step, and D34 (lane F) is where
the per-seat chooser owner belongs.

## What was tried (all on the current master binary, `1c7ba720d` + this lane)

| # | shape | result |
|---|---|---|
| A | fully scripted: seat takes window on a spell, casts targetless instant (Fog), opponent counters (Counterspell), game marched to the next turn's first main | **advances**, green |
| B | same, TARGETED instant (Giant Growth) armed and resolved, window entered at the END step (`goto end`) | **advances**, green |
| C | both seats Baka (`ai`): P2 casts Terror in P1's phase-boundary window, P1 counters | **advances** (assert numbers differ; no stall) |
| D | **mixed seats** (`aiseat 1`): AI seat active + Counterspell, scripted seat responds with Giant Growth in the AI's window, AI counters | **advances**, green — kept as a fixture |
| E | as D with **Dissipate** (counter + exile, deck131's card) | **advances**, green — kept as a fixture |
| F | as D with **Essence Scatter** on a flash creature (Ambush Viper) | advances, no stall |
| G | as D, window taken at the AI's **END step** | advances, no stall |
| H | **counter war**: scripted seat counters the AI's counter | advances, no stall |
| I | scripted seat's spell targets the AI **player** (Lava Spike) | advances, no stall |
| J | `cancelbutton` pressed inside the window after the counter resolved (lane Z leg (a) crossed with the counter) | advances, no stall |
| K | `WAGIC_SELFPLAY` one-shots, deck131 (the owner's opponent deck) vs decks 2/5/7, **both seats AIPlayerGPT under `WAGIC_GPT_NOTHREAD=1` with a configured endpoint** | all three games ran to a natural result (turn 15/21/15) |
| L | `WAGIC_REPLAY` of the owner's own transcript from the softlocked match | **diverges at action 25** (turn 2, upkeep: expected `p1.next 1 3`, got `p1.next 1 2`, `stackUnresolved=1 isInterrupting=p2`) and stops there — the desktop card pool does not carry the Vita's card ids, so the replay never reaches turn 6. Not a repro; recorded because the divergence is real and unexplained. |

Not one produced `==Test timed out (game never ended)==` or a hung process.

## What shipped

1. **`aiseat <1|2>` suite DSL** (`src/TestSuiteAI.cpp`). Puts ONE seat on the
   heuristic AI and leaves the other reading the script. The blanket `ai` command
   hands BOTH seats to the AI, so until now the suite could not express the shape
   every human play report has — a script-driven (non-`MODE_AI`, i.e. lane Z's
   "human" branch) seat responding inside an AI seat's window. Registered in both
   the `cannotAnswer` and `keyword` guard lists. Variations D-J above exist only
   because of it.
2. **`WAGIC_STALLPROBE`** (`src/ActionStack.cpp`, `ActionStack::Update`).
   Compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)` and
   env-gated inside that; prints the whole priority/choice state vector per tick —
   turn, phase, `currentPlayer`, `currentActionPlayer`, `isInterrupting`,
   `askIfWishesToInterrupt`, both `interruptDecision`s, `mode`/`modal`, the
   unresolved-stack count, `ActionLayer::currentWaitingAction`, `menuObject`,
   `observer->targetChooser` and its `Owner`, and `mHoldTicks`. The next report of
   this class is one run away from naming the stuck flag instead of a week of
   reading.
3. **Two negative-control fixtures**, `Res/test/` and `_tests.txt`:
   `w57S_human_seat_cast_countered_advances.txt` and
   `w57S_human_seat_cast_dissipated_advances.txt`. They pin that the reported
   shape ADVANCES, so if a later change breaks it the suite says so. Both are AI
   fixtures (AI seat count 55 -> 57).

## Gate (worktree `worktrees/lanes/w57-S`, branch `w57-lane-S`, base `1c7ba720d`)

| leg | baseline | this lane |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1241 / 0 failed | **1241 / 0 failed** |
| AI tests (same run) | 55 / 0 | **57 / 0** (+2 = the two new fixtures) |
| `timed out` occurrences | 0 | **0** |
| PARSETEST | 3216 / 0 | **3216 / 0** (unchanged — no parse or render string touched) |

## NOT verified / what remains

* **The softlock itself is not reproduced and not fixed.** Everything above is
  elimination plus a reading of the two watchdog-blind states.
* The lane brief's premise that the Vita seat was on the thread-refusal fallback
  is **contradicted by that session's own gpt log** (no refusal line; a signed-in
  ChatGPT endpoint). That matters for the watchdog arithmetic: with a non-empty
  endpoint `Player::isInteractiveAI()` is true, so the W53-AA stall floor's budget
  for a window that seat holds is `mHoldTicks >= 300 && wallSeconds >= 1200` —
  **twenty wall-clock minutes** — and while `aiDecisionInFlight()` is true it is
  **unbounded**, because `decisionPending` calls `extendInterruptOffer` every tick.
  A wedged HTTP call therefore freezes the duel with no bound at all. Against
  that: the patience prompt (`aiPatiencePromptDue`, default 60 s) should have
  offered him the way out and he reports no such prompt. **Unresolved, and the
  first thing to check with the owner: did the seat show "opponent is thinking"?**
  If it did, the fault is the unbounded in-flight exemption, not the priority
  machinery, and the fix is to bound the exemption by the request's own
  `mTimeoutMs` rather than by nothing.
* **D34 (per-seat chooser owner) is not done** and is the right home for the
  ownership hole described above; it is already laned to F this wave.
* The two `kCancelMenuID -> endOfInterruption(false)` call sites in
  `ActionLayer.cpp` are unpinned by any fixture, on this lane or lane Z's.
  Note also that this pre-dates lane Z: cancelling an abilities MENU has always
  ENDED THE INTERRUPT WINDOW, which is a separate rules/UX question nobody has
  put to the owner.
* No Vita/PSP build and no GUI drive. Everything here is the desktop headless
  suite plus selfplay.
