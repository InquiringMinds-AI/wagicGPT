# Wave-57 lane T — softlock escape + diagnostics capture

Owner request (verbatim, 2026-09-03): *"We need a softlock exit to main menu option, for
capturing diagnostics from softlocks."*
Owner correction the same day (verbatim): *"No, the menu button works. I used it."*

That correction is what shaped this lane. During the vpk16 freeze the **game loop was alive** —
the in-game menu opened and he quit through it; the selector was frozen because the layers did
not name his seat as the acting one, not because a loop was spinning. So the state that mattered
was readable the whole time and simply went unrecorded. The primary escape is therefore a **menu
item on the game thread**, not a thread and not a watchdog.

Scope as shipped:

* **(A′) IN-GAME MENU ITEM — primary, always on, every platform.** *"Softlock: dump diagnostics
  and quit to menu"*, added next to *"Back to main menu"* in the in-game (START) menu.
* **(A) IN-THREAD HANG GUARD — secondary, DEFAULT OFF** (`WAGIC_HANG_GUARD=1`). A per-tick wall
  clock checked at eight judged loop sites; on trip it throws `SoftlockAbort`, which the duel
  screen catches, writes the same capture, and leaves by the same door.
* **(B) WATCHDOG THREAD + PAD CHORD — DROPPED** on the coordinator's instruction. No thread is
  created; nothing on the Vita is touched by this lane except one `strncpy` in the memlog writer.

---

## The menu path that was reused

`GameStateDuel::ButtonPressed`, `DUEL_MENU_GAME_MENU`:

```
case MENUITEM_MAIN_MENU:                 case MENUITEM_SOFTLOCK_DUMP:
    menu->Close();                           wagicSoftlockDump(game, "menu");
    setGamePhase(DUEL_STATE_BACK_TO_MAIN_MENU);   menu->Close();
    break;                                   setGamePhase(DUEL_STATE_BACK_TO_MAIN_MENU);
                                             break;
```

The exit is **byte-for-byte the existing quit** — the same `menu->Close()`, the same
`DUEL_STATE_BACK_TO_MAIN_MENU`, therefore the same `tournament->setOpLevel(OPLEVEL_NEW)` +
`tournament->End()` + `PlayerData::taskList->passOneDay()` + `DoTransition(TRANSITION_FADE,
GAME_STATE_MENU)` + `GameStateDuel::End()` teardown of the observer and the layers. Nothing about
the teardown is re-implemented, so it cannot drift from the plain quit.

The capture runs **before** `menu->Close()`: it reads live state and mutates nothing, so there is
no ordering hazard, while running it after would read a half-closed menu.

Menu item is added inside the same `if (!menu)` construction block guarded by
`taskList->getState() != TASKS_ACTIVE`, i.e. it appears exactly where and when
*"Back to main menu"* appears.

**Hang-guard exit, one honest difference:** when the guard fires there is usually **no open
menu**, so `DUEL_STATE_BACK_TO_MAIN_MENU`'s `if (menu) { ... }` block is skipped and only
`DoTransition(TRANSITION_FADE, GAME_STATE_MENU)` runs. The transition, `GameStateDuel::End()`,
`SAFE_DELETE(game)` and `tournament->End()` all still happen (End() calls it unconditionally);
what is skipped is `setOpLevel(OPLEVEL_NEW)` and the task-list day tick. Shared code was left
alone rather than hoisted, because hoisting changes the normal quit for a path that only the
opt-in guard reaches.

---

## The diagnostics file

**Path**

| platform | path |
|---|---|
| Vita | `ux0:data/Wagic/softlock-<epoch>.txt` |
| everything else | `User/softlock-<epoch>.txt` (override: `WAGIC_SOFTLOCK_DIR`) |

Deviation to state plainly: the brief said `~/.Wagic/softlock-<epoch>.txt` for desktop. The engine
has no `~/.Wagic` concept — every desktop/handheld artefact it writes (`User/transcripts/`,
`User/wagic-probe.log`, `User/wagic-memprobe.log`) goes to the cwd-relative `User/` directory that
`JFileSystem` already owns, and the suite fixture needs a path it can read back. `User/` is what
shipped; `WAGIC_SOFTLOCK_DIR` covers anyone who wants it elsewhere.

A second capture inside the same wall-clock second gets `-1`, `-2`, … before `.txt` rather than
silently overwriting the first.

**Layout** (real capture, taken by the fixture with a target chooser armed):

```
#softlock dump
build=wagic-v0.25.5 compiled=Sep  3 2026 15:56:01      (+ " id=<...>" if -DWAGIC_BUILD_ID is set)
trigger=menu | testsuite | hangguard site=<site> iterations=<n> elapsed_ms=<n>
epoch=1788468987

[game]
turn= phase=<name>(<id>) nextphase= combatStep= settled(turn/phase/step)= phaseTicks=
currentPlayer= currentActionPlayer= currentlyActing= isInterrupting= gameOver=
pregame= pregameDone= loading= suiteGame= cardWaitingForTargets= extraPayment=
observer.targetChooser: class= source= targetter= owner= targets= maxtargets= valid=
                        done= full= other= targetMin= autoChoice= attempts= ability=
currentTargetChooser:   <same shape>
openedDisplay= humanDisplayOpen=

[actionlayer]
cantCancel= stuffHappened= menuArmedSerial= menuObject= menuObjectName= currentActionCard=
abilitiesMenu= abilitiesTriggered= garbage= destroying=
currentWaitingAction=<class> source= waitingForAnswer= modal=
waitingAction.tc: <TargetChooser shape>
mObjects=<n>
  i=<k> class=<rtti> source=<card> tcTargets=<n>          (one line per entry, all of them)

[stack]
askIfWishesToInterrupt= interruptDecision=[p1,p2] timer= state= calm=
priorityOn= holdOn= holdWho= holdTicks= holdSeconds= holdStartMs= lastActionController=
items=<n>
  i=<k> class= state= display= source= name='' targets=<n> [<names>] tc: <TargetChooser shape>

[players]
p1 name= deck= life= ai= interactiveAI= decisionInFlight= manapool=
   library= hand= inplay= graveyard= exile= stack= commandzone= reveal= sideboard=
   garbage= garbageLastTurn= temp=
p1 gpt:                                        (only when the seat is an LLM seat)
  arm=main status=<0 idle|1 in-flight|2 done-unconsumed> inflight_ms= http= timedout=
      prompt_bytes= response_bytes= seam='<slotKey>'
  arm=land <same>
  endpoint_set= in_flight= think_s= timeout_ms= patience_s= last_latency_ms=
      fallbacks= degraded_ticks= notice=''
p2 ... (same)

[frames]
frames turn= n= avg_ms= max_ms= last_ms= dtsum_s=      (memlog-style, every platform)
memlog_last=<the last Vita memlog frame/frames line>   ("(not built)" off-console)

[transcript]
path=<the game's transcript file, or (none)>
actions=<total> showing_last=<n>
  <the last 40 recorded actions, one per line>
#end
```

Everything the brief asked for is present except **the AI seat's "last stderr line"**, which has
no ring buffer to read; `notice='...'` (the seat's standing in-duel notice — "model timed out",
"no endpoint reachable") is the closest live substitute and is what shipped. Stated, not hidden.

**Transcript stamping.** The capture appends three notes to the game's transcript:
`#classification=softlock`, `#softlock file=<path>`, `#softlock trigger=<...>`, and records the
path on the observer (`GameObserver::mSoftlockDumpPath` — on the *object*, not a static, per the
W53-U lesson). If the owner later classifies the match **"bug"** on the victory screen, that path
is appended again next to the classification, so the verdict and the capture are one grep apart.

**Pointer discipline.** Every `MTGCardInstance` is resolved through
`GameObserver::validateCardPointer` (pointer comparison, never a deref). An unmatched pointer
prints `(unverified)`, **not** `(dangling)` — the engine's own rule abilities carry sources
outside every tracked zone (ExtraRules) and come back unmatched while perfectly alive. Calling a
live pointer dead in a diagnostics file would send the next reader hunting a use-after-free that
is not there.

---

## Half A — the in-thread hang guard (default OFF)

`GameObserver::hangTickBegin()` re-arms a wall-clock budget at the top of every
`GameObserver::Update`; `GameObserver::hangCheck(site)` is the per-iteration probe.

* **Off unless `WAGIC_HANG_GUARD=1`.** Explicit opt-in, resolved once per observer. That covers
  the suite and the selfplay/PARSETEST harnesses without a special case: a suite tick under load
  can legitimately take seconds, and a guard that fires on a loaded machine is a flaky red.
* **Budget: 5 s desktop / 8 s Vita+PSP**, `WAGIC_HANG_GUARD_MS` overrides.
* **Cost:** `if (!on) return;` in the off case. On: an increment and a mask; the clock is read
  once per 64 calls (`(++iter & 0x3F) != 0`).
* **The AI's async wait cannot trip it.** The budget is per **tick**, not per decision. A seat
  waiting on a model round trip is not inside the tick at all — its seam returns `kChoicePending`
  and the frame completes normally — so an endpoint taking two minutes burns zero budget. This is
  a structural property, not a special case.
* **Disarms before throwing**, so the capture the unwind writes cannot itself be aborted.

### Loop-site table

| # | site string | file | loop | why guarded |
|---|---|---|---|---|
| 1 | `GameObserver::Update/stuffHappened` | GameObserver.cpp | `while (actionLayer()->stuffHappened)` | the classic re-entrant in-tick spin; a click that re-arms itself never returns from the tick |
| 2 | `GameObserver::receiveEvent/eventsQueue` | GameObserver.cpp | `while (eventsQueue.size())` | trigger dispatch; a self-requeuing event never drains |
| 3 | `GameObserver::nextGamePhase/discardToHandSize` | GameObserver.cpp |  `while (hand->nb_cards > handmodified && !nomaxhandsize)` | spins if the discard is refused |
| 4 | `GameObserver::gameStateBasedEffects/flanked` | GameObserver.cpp | `while (c->flanked)` | decrements a counter that a replacement effect can restore |
| 5 | `AIPlayer::Act/clickstream` | AIPlayer.cpp | `while (clickstream.size())` | an action that re-pushes itself |
| 6 | `AIPlayerBaka::selectAbility/potentialTargets` | AIPlayerBaka.cpp | `while (potentialTargets.size())` | shrinks only when a target is consumed |
| 7 | `AIPlayerBaka::chooseTarget/playerTargetedZone` | AIPlayerBaka.cpp | `while (playerTargetedZone)` | zone walk with an early-exit condition that a full chooser can starve |
| 8 | `AIPlayerBaka::declareBlocker/setDefenser` | AIPlayerBaka.cpp | `while (!set)` | exits only when the click takes |

**Judged out, with reasons** (loops enumerated by grepping `while (` / `for (;;)` across the
engine): `ActionLayer::Update`'s two `for` loops and `ActionStack::Update`'s scans are bounded by
container size and cannot spin on their own; `ActionLayer::~ActionLayer` and `AIPlayer::~AIPlayer`
both contain `while (container.size())` loops but are **destructors** — throwing from one during
unwinding is `std::terminate`, so **no destructor is guarded, ever**; `ActionStack::garbageCollect`
is an iterator walk over a fixed range. `MayAbility::Update` (AllAbilities.cpp) is a state machine
with no loop of its own and was left alone.

### Would it have caught the reported shape?

**No — and it was never going to.** The owner's own correction settles it: his loop was alive.
The guard exists for a *different* failure (a tick that never returns) that the first brief
assumed and the correction ruled out. The menu item is what covers the reported shape, and it
covers it by capturing exactly the fields the report was missing: `currentActionPlayer`,
`isInterrupting`, `askIfWishesToInterrupt`, `interruptDecision[]`, `mHoldOn`/`mHoldWho`/
`mHoldTicks`, and each seat's async status — the disagreement between "who the layers think is
acting" and "who can actually act" is a difference these fields make visible and nothing else does.

---

## Gate

Baselines on `1c7ba720d`: PARSETEST 3216/0, suite 1241/0 at `WAGIC_TESTSUITE_THREADS=1`,
55 AI/0, 0 timeouts.

Results on this branch (hermetic re-run against the exact committed source,
`~/.gatelogs/w57T-parsetest.log` / `w57T-suite.log`, all under the 4G memory cap):

| gate | baseline (1c7ba720d) | this branch |
|---|---|---|
| PARSETEST | 3216 / 0 | **3216 / 0** |
| suite (`WAGIC_TESTSUITE_THREADS=1`) | 1241 / 0 | **1242 / 0** (+1 = the new fixture) |
| AI games | 55 / 0 | **55 / 0** |
| timeouts | 0 | **0** |

The prediction holds: every existing count is unchanged and the only movement is the
one new green fixture. No stray `User/softlock-*.txt` survived the run (the fixture
removes its own captures).

New fixture **`bin/Res/test/softlock_dump.txt`** (registered in `_tests.txt`), RED on base — the
base build cannot even parse its `softlockdump` / `hangspin` commands. It pins both halves:

* `softlockdump` — runs the **same** `wagicSoftlockDump` the menu item runs, at two moments a
  real softlock report comes from (a target chooser armed and waiting; and after the spell has
  gone through the stack), then **reads the file back** and asserts 26 required field markers
  (`#softlock dump`, `[game]`, `observer.targetChooser:`, `[actionlayer]`, `currentWaitingAction=`,
  `mObjects=`, `[stack]`, `interruptDecision=[`, `[players]`, `library=`, `[frames]`,
  `memlog_last=`, `[transcript]`, `#end`, …) plus that the observer recorded the path. Reading it
  back is the point: "the function returned a path" would pass with an empty file.
* `hangspin <ms>` — arms the guard with a 20 ms budget (`hangGuardForTest`, the programmatic twin
  of the env pair, because a registered fixture runs under the threaded suite and must not depend
  on process-wide state), spins, and asserts the guard tripped, named `testsuite.spin`, and wrote
  a capture whose `trigger=` line carries the site. The abort is caught **in the command**, not at
  the duel screen: the suite has no duel screen and must not be dragged to the main menu
  mid-fixture.

The fixture removes its own captures (`WAGIC_SOFTLOCK_KEEP=1` leaves them for reading).

**Prediction (falsifiable):** with the guard off — the default, and the state of every gate run —
this lane changes zero engine behaviour; suite, AI and PARSETEST counts equal the base baselines
exactly, plus one new green fixture. Any movement in an existing count falsifies it.

---

## Vita

Compiles under `-std=c++14` with vitasdk **in principle only — NOT BUILT, NOT RUN on the console.**
Everything Vita-specific stays behind the existing `#ifdef VITA` pattern and mirrors
`Vitamain.cpp`'s idioms:

* the dump's path branch uses `sceIoMkdir("ux0:data")` / `sceIoMkdir("ux0:data/Wagic")` — the same
  two calls `GameObserver::resetStartupGame` already makes for `transcripts/`, from a file that
  already includes `<psp2/io/stat.h>` on that platform;
* one addition to `JGE/src/Vitamain.cpp`: `memlogAppend` keeps a copy of the line it just wrote in
  a 352-byte static, exposed as `extern "C" const char * vitaFrameLastLine()`, so a capture can
  quote the last memlog frame line without an I/O round trip on a wedged console. Guarded by the
  existing `WAGIC_VITAMEMLOG` block. Cost: one `strncpy` per memlog line, and memlog lines are
  per-turn plus at most five slow frames — never per frame.
* No new source file, so `CMakeLists.txt`, `Android.mk` and `Makefile.sdl` are untouched. The dump
  and the guard are defined in `GameStateDuel.cpp` (commented in place) rather than in their own
  translation unit precisely to avoid three build-file edits mid-wave.
* No thread is created anywhere. The `std::thread`-refused-on-Vita constraint (wave54 lane L) is
  not approached.

---

## What was NOT verified

1. **Anything on the console.** The vpk was not built; `vitaFrameLastLine`, the `ux0:` path branch
   and the menu item's appearance on the Vita are unverified. Desktop only.
2. **The menu item pressed by a human.** The fixture calls the dump function directly; it cannot
   press a `SimpleMenu` entry. What is verified is that the handler compiles and that its two exit
   statements are literally the ones `MENUITEM_MAIN_MENU` uses.
3. **The return to the main menu.** Nothing automated observes `GAME_STATE_MENU` after the item.
   The claim rests on path reuse, not on an assertion.
4. **The hang guard against a real engine hang.** `hangspin` proves budget → throw → capture; no
   fixture wedges a real engine loop. The eight sites are judged, not exercised.
5. **The `SoftlockAbort` unwind through real engine frames.** The fixture's throw unwinds a bare
   `for` loop. Unwinding out of `GameObserver::Update` through the layers has not been run; if any
   engine frame holds raw ownership across a call, the unwind could leak. (It cannot double-free:
   the unwind runs no destructors that a normal return would not.) This is why the guard is off by
   default.
6. **Per-frame cost of the heartbeat, measured only indirectly.** `wagicSoftlockFrameTick` is one
   `JGEGetTime()` call and five integer ops per duel frame. It was not profiled on the 1060 or on
   the console; the suite's wall time was unchanged, which bounds it but does not measure it.
7. **The classifier link.** The `"bug"` → `#softlock file=` append is code-reviewed only; reaching
   it needs a human on the victory screen after a capture, and the capture normally quits the game.
   It fires only in the case where a capture was taken and the match later ended normally.
