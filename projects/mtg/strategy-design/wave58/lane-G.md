# Wave-58 lane G — F2 (engine parked after an AI menu answer of -1 on Sigarda, Champion of Light)

Branch `w58-lane-G`, base `e663e3f56`. Master has moved on (742324b5a); this lane is NOT rebased.

## The mechanism (established, not assumed)

The `-1 of 2` in the corpus stderr is the ask's own number, not the click. The heuristic
fallback below it maps its pick back into option space, so a real option was clicked
(`ActionLayer::doReactTo 178`) and Sigarda's Coven trigger resolved. The trigger is a
REVEAL (`auto=@combat(attacking) ... reveal:5 optionone name(Get a human) ... optiontwo
name(put on bottom) ...`), so an LLM seat resolves it through the interactive reveal
driver (`MTGRevealingCards::driveInteractiveReveal`). The parked game is downstream of
the click, inside that driver.

Reproduced headlessly with lane S's forced-async seam (`revealasync`), the empty-reply
value (-1 = decline, no Human taken) and `WAGIC_REVEAL_DEBUG=1`:

```
phase0 decided r=1 picks=0 zone=5 tc=0x..f2c0
CheckUserInput NEXT: nbTargets=0 abilitySecond=0 -> DECLINE(all option-two)
phase3 enter: tc=0x..f2c0 second=0x..7620 zone=5 onLayer=-1     <- option two is NOT on the layer
phase4:      tc=0x..f2c0 idx=0/5 zone=5 wait=0x..8520           <- the waiting action is neither
phase4:      tc=0x..f2c0 idx=1/5 zone=5 wait=0x..8520              option one nor option two
phase4:      tc=0        idx=2/5 zone=5 wait=0
phase5:      tc=0        zone=5 display=0x..a790                <- nothing moved, display still open
```

The chain:

1. `CheckUserInput`'s DECLINE branch removes `abilityFirst` from the stack and from the
   game, but option one's NESTED target ability is the action layer's
   `currentWaitingAction`, and neither removal reaches it. (`ActionLayer::removeFromGame`
   only clears `currentWaitingAction` for the element it is handed, and returns early
   when that element is already out of `mObjects`.)
2. The option two built beside that corpse is added to the layer but never ARMS.
3. `ownChooser()` proves only that the armed chooser belongs to this reveal's SOURCE - it
   cannot tell option one's chooser from option two's, and the two are not even
   distinguishable by pointer (option two is constructed the instant option one dies, so
   the allocator hands back the same address). Phase 3 accepts the corpse as option two's
   chooser, phase 4 spends two clicks on it, the third finds it gone.
4. **Phase 5 then sets `mAIDriveDone = true` unconditionally.** That is the fatal step:
   `mAIDriveDone` is the first line of `driveInteractiveReveal`, so setting it retires the
   reveal's stall guard - the only thing in the engine that can force-close a reveal
   nothing else will finish.
5. The reveal display is left open over a full reveal zone, and `GameObserver::Update`
   returns early on every subsequent tick for as long as that is true
   (`if (OpenedDisplay && (...reveal->cards.size()...)) return;`, GameObserver.cpp ~1265).
   The game stops advancing, for ever, with no receipt. That is F2.

## What shipped

`projects/mtg/src/AllAbilities.cpp` — `MTGRevealingCards::driveInteractiveRevealStep`,
phase 5: DONE now means the reveal is OVER, not that the driver ran out of steps.
`mAIDriveDone` is set only when the reveal actually finished (`zone` empty, or the display
already closed). An unfinished reveal stays accountable to the stall guard, which
force-closes it, returns the revealed cards to the zone they came from, and writes the
existing `reveal_stall_forced` receipt. Nothing is capped and no window is removed: the
decline still stands and the menu is untouched - what changes is that a reveal that
cannot finish now ENDS and SAYS so instead of parking the game.

Three `REVEAL_DBG` probes (phases 3/4/5) ship with it, compile-time gated
(`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`) and runtime-gated on
`WAGIC_REVEAL_DEBUG` like the driver's existing traces. They are what located this and
what will locate the next one - phases 3-5 had no unconditional trace at all.

`projects/mtg/include/TestSuiteAI.h` — FIXTURE INFRASTRUCTURE, not the fix: a suite seat
that has opted into the forced-async reveal seam (`revealasync`) now reports
`isInteractiveAI()`. Without it the seat is not interactive, so `AllAbilities.cpp:69` lets
the AICODE substitute pre-empt the interactive driver and the fixture races between two
different code paths run to run (measured: the same fixture gave `battlefield 4` on one
run and `hand 1` on the next). This is present on BOTH sides of the RED/GREEN.

`projects/mtg/bin/Res/test/w58G_reveal_decline_advances.txt` (+ `_tests.txt`) — the fixture.

## RED / GREEN

Both measured with `WAGIC_TESTSUITE_FILE=test/_probe.txt`, `timeout 200`, under the 4G cap.

* **RED on base** (base + the TestSuiteAI.h fixture infrastructure, AllAbilities.cpp at
  base): `==Card number not the same in player 0's library==, expected 7, got 2` -
  five of the seven library cards stranded in an open reveal zone, no force-close line.
  2/2 runs identical.
* **GREEN after**: `==Test Successful !==`, preceded by
  `MTGRevealingCards: reveal from Sigarda, Champion of Light made no progress (driver)
  for 10 ticks ... with 5 card(s) still revealed - FORCE-CLOSING it (driver phase 6)`.

**The RED is a stranded-state RED, not a timeout.** I could not make this shape read
`==Test timed out` and I did not fake one. The suite's only hang detector is a
1,000,000-update cap on `while(!didWin())`, and a fixture reaches its `[ASSERT]` from the
script pump, which keeps draining regardless: an `[ASSERT] ENDOFTURN` fixture therefore
always asserts. I checked this directly - a variant whose whole pump was replaced by
`goto end` completed in 7 s over the parked reveal, because the suite advances phases
through a path `GameObserver::Update`'s early return does not gate. The freeze itself is
the code path in step 5 above, read out of GameObserver.cpp, plus the measured stranded
state; it is not a wall-clock measurement.

## The translog question

The menu is NOT missing its receipt. The deck126 review found it at the deck152 seat's
seq 30 with choice -1 and class `empty_reply` - the `writeTransLog("ask", ...)` at the end
of `askModel` writes it on the fallback path too. What had no receipt was the
CONSEQUENCE: the reveal that then parked wrote nothing, because the only code that writes
a reveal receipt is `forceCloseStalledReveal`, and phase 5 had already retired it. After
this change that state emits `reveal_stall_forced` through the existing
`logEngineResolution` channel. No new record kind is needed, and none was added.

## Predictions (falsifiable, wave-58 corpus)

1. No game in the next corpus ends with a seat translog gap longer than the harness
   watchdog while the process is still alive AND the last stderr activity is a reveal
   source. Any remaining no-tick hang will have a different last-activity signature.
2. Every reveal that fails to complete in the next corpus now leaves a
   `reveal_stall_forced` record naming its source card, and the count of such records is
   >= 1 and small (single digits over ~20 games); a large count means the decline path is
   parking routinely and the CAUSE below needs the fix, not just the floor.
3. The `(driver phase 6)` figure appears in at least one force-close line. Phase 6 is
   reachable only through the hole this lane closed, so its absence over a full corpus
   would mean the corpus never hit the F2 shape, not that the fix is inert.

## What I did NOT verify / did not do

* **I did not fix the CAUSE, only guaranteed the exit.** Option two still fails to arm
  after a decline when option one's nested target ability is still the layer's waiting
  action. Four attempts to fix it at the source all failed and were reverted: releasing
  the waiting action in the DECLINE branch (never fires - the layer reads free at that
  instant and the corpse re-arms afterwards); requiring option two to be on the action
  layer before accepting its chooser (fires the W54-X rebuild too early, and the rebuilt
  ability still does not arm); deferring option-two construction while the layer is
  waiting (same - the layer reads free at that instant); discriminating the chooser by
  `abilitySecond->getActionTc()` (returns NULL even in the working case - the wrapper is
  not the layer's element). The correct cause fix needs the identity of the nested
  waiting element, which `MTGRevealingCards` does not hold. Consequence: on this path the
  revealed cards now go back to the LIBRARY via the force-close instead of to the BOTTOM
  of the library via option two. For Sigarda those are the same zone but not the same
  order; for another card with a different option two they would be the same zone as the
  reveal came from, which is the force-close's stated least-harm contract. **This is the
  next item.**
* **A second, separate bug is proven and NOT fixed here.** With the decision delayed two
  ticks (`revealasyncticks 2`), the seat's own generic action pass consumes option one's
  chooser before the driver sees it: after a DECLINE the AI still takes "Get a human" and
  Devoted Retainer goes to HAND (`library 6, hand 1`, deterministic 3/3). This is the
  W50-W (D2) double-consumer shape producing a wrong OUTCOME rather than a hang. The two
  Codex runs on this lane fixed exactly this (a `GameObserver::mInteractiveRevealDriver`
  ownership stamp plus early returns in `AIPlayerBaka::Act` and `TestSuiteAI::Act`, and an
  `acceptInteractiveRemainder()` that auto-clicks option two's MayAbility). I discarded
  that machinery: an unconditional early return in a seat's `Act` is a no-advance hazard
  of exactly the class this lane is closing, and auto-answering a may-menu through
  `doReactTo` without asking is a window removed. It wants its own item with its own
  design.
* No live model probe was run. No PSP/Vita build was attempted (this lane touches no
  platform code).
* The corpus hang itself was not re-run under the fixed binary - the fix is verified by
  the fixture and by the code path, not by a replay of game 152v126.
