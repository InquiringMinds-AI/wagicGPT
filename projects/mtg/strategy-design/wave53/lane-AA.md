# Lane AA — the Vita softlock ("one of the quit to menu games was a softlock")

## Which game

The two vpk12b transcripts the owner flagged are the ONLY two of the 88 in
`psp-work/logs/20260902-vpk12b/` whose header tag is `turn` rather than `note`:
every other game got a post-match note (`#result` / `#classification`), so these
two are exactly the games he abandoned. Both files end at the same structural
place — the transcript is rewritten at each untap, so the last write is the untap
of the human's turn 11 and NOTHING from that turn is on disk in either file. The
file endings therefore do not discriminate; the replays do.

**Game 1 — `transcript-1788390621-player_deck4-vs-ai_baka_deck54` is the QUIT.**
It replays byte-perfect to turn 10 phase 1 (life 15/9, the header). Appending the
human's whole turn as synthetic `p1.next 1 <phase>` requests replays the ENTIRE
turn without a single refusal and lands at the AI's next untap (turn 11 phase 1,
life 14/9 — Sleeper Agent -2, Nyx-Fleece Ram +1, exactly right). No interrupt
window was offered to the AI anywhere in that turn. Nothing in that board wedges.

**Game 2 — `transcript-1788393737-player_deck5-vs-ai_baka_deck33` is the SOFTLOCK
turn.** Same treatment stops dead at the first phase of the human's turn:

```
REPLAY DIVERGED at action 144 expected 'p1.next 1 2' got 'p1.next 1 1'
  turn 10 phase 2 | chooser=none menu=none menuCard=none
  extraPayment=no stackUnresolved=1 isInterrupting=p2
```

Phase 2 is the human's UPKEEP. The unresolved stack object is **The Rack's**
upkeep trigger (`@each targetedplayer upkeep:damage:3minus...`), and the AI seat
holds the response window. Every phase request from there on is refused. Same on
the exact vpk12b binary (`archives/wagic-859186536-w53corpus`).

(Game 2's raw transcript will not replay past turn 8 on ANY build: the engine
auto-taps for the Bloodforged Battle-Axe equip and the recorded equip-target
click is then never accepted, 60 retries. That is a separate replay-fidelity
defect, recorded below, not this lane's fix. Dropping the four equip lines lets
the replay reach the stall.)

## The sequence, in plain words

Turn 11, the human's own upkeep. The Rack (the AI's, cast turn 2, chosen player =
the human) puts its damage trigger on the stack. Priority goes round: the human
auto-passes, and the AI is offered a response window — it has untapped Swamps and
instants, so `wouldOfferWindow` says yes, which is why this turn and not turn 9.
The AI never closes that window. From that instant the game is over as a game:

- `GameObserver::userRequestNextGamePhase` returns immediately while anything on
  the stack is `NOT_RESOLVED`, so every press of the trigger does nothing;
- `Options::INTERRUPT_SECONDS` is 0 unless the player has saved it, so
  `ActionStack::Update`'s own interrupt timer never runs ("wait for ever");
- and `DuelLayers::CheckUserInput(isAI)` **reads and discards every key** while
  the player passed down from `GameObserver::Update` is the other seat — which it
  is, because `player = isInterrupting`. The human's input never reaches a layer.

So: the board renders, the menu still works (he quit to it), and the trigger is
dead. That is his softlock, and nothing anywhere printed a word about it.

## The seam

`ActionStack::Update` — the priority round. An interrupt window is offered or
taken by a seat and there is no bound on how long it may hold. The engine's three
existing escape hatches all miss this case: the INTERRUPT_SECONDS timer is off by
default, `ActionStack::CheckUserInput`'s trigger-as-decline never gets the key,
and `MTGGamePhase`'s phase-bar-tap decline **deliberately excludes AI-owned
windows** ("a tap must not close the window an LLM opponent is still thinking
in"). The AI-owned window is the one hole, and it is the one the owner fell into.

## The fix

1. **`ActionStack::Update` — a stall floor (the main change).** Track the holder
   (`askIfWishesToInterrupt`, else `observer->isInterrupting`) and the top
   unresolved stack object. A different holder, a different top object, or an
   explicit `extendInterruptOffer` (the LLM seat's "still thinking" signal) all
   count as progress and reset. When an **AI-owned** window survives 300 ticks
   AND 20 s of game time with none of those, print one stderr line naming the
   holder, the stack object, the turn and the phase, and decline the window
   (`cancelInterruptOffer(DONT_INTERRUPT, false)`) so the stack resolves and the
   game advances. Budgets: an interactive-AI seat gets 1200 s (past the whole
   900 s request timeout) instead of 20; a LOADING game gets 12 ticks, because
   `DuelLayers::Update` calls `Act()` only `if (isAI && !isLoading)` — **no seat
   can answer a window during a replay at all**, which is why the transcript tool
   died precisely where the live game did. Human-owned windows are not watched:
   their bound is the player's own input, which the key road already accepts, and
   yanking one would steal a response he is in the middle of making.
2. **`AIPlayerBaka::Act` — one concrete live cause removed.** In the interrupting
   branch, an extra payment that is NOT an `ExtraManaCost` (a sacrifice, discard,
   exile cost) fell through to `return 0` with `mExtraPayment` still armed and
   this seat still holding the window — every tick, for ever. It now declines the
   payment the way `chooseTarget` already does. (The neighbouring `isLoading`
   decline branch is dead code from the duel loop; it is annotated as such — the
   working release is the floor in (1), which needs no seat to be updated.)
3. **`TestSuiteActions::add` — a bounds check.** `actions[MAX_TESTSUITE_ACTIONS]`
   had none; a fixture one line over 100 wrote past the array and SEGV'd inside
   the string allocator with nothing naming the test (hit while writing this
   lane's fixture). It now names the test and drops the overflow.

## Suite pin

`bin/Res/test/w53AA_ai_interrupt_window_stall.txt` (registered in `_tests.txt`).
p1 casts Hill Giant; p2 is offered the response window (Shock + untapped
Mountains make it real), takes it with `yes`, and never ends the interruption —
then 80 `next` presses (the owner holding his trigger) and a trailing `eot`,
which re-queues itself until Cleanup so the assert does not depend on the
driver's command cadence.

- **RED on base** (lane base `cd005b06e`, rebuilt with the fix stashed out): the
  game never leaves Main phase 1, Hill Giant never resolves, `eot` re-queued
  6,020 times and the run was still spinning when it was killed at 10 minutes.
  It is a hang, not a fast failure — which is the point of the pin.
- **GREEN after**: the floor releases the window at 300 ticks with
  `wagic: interrupt window held by Player 2 on 'Hill Giant' for 300 ticks
  (turn 0 phase 4) - releasing so the game advances`, Hill Giant resolves and the
  turn runs out to the next untap.

Replay evidence, same binaries: game 2's continuation goes from stopping at
turn 10 phase 2 (base) to completing the human's whole turn to phase 12 (fixed);
game 1 is unchanged in both.

## Gate

Baseline re-confirmed on this worktree before the change: suite **1219 tests, 0
failed, 0 timed out**, 47 AI tests / 0; PARSETEST **2258 passed, 0 failed**.
(Single-threaded is ground truth, so the two known concurrency-only names pass.)

After: suite **1220 tests, 0 failed, 0 timed out**, 47 AI tests / 0 failed;
PARSETEST **2258 passed, 0 failed**. +1 test = this lane's fixture. The watchdog
fires exactly twice in the whole suite — both from that fixture; no other test
comes near the budget.

## What I did NOT verify

- **No Vita build.** Port builds are owner-initiated; this is desktop only.
- **I did not prove WHICH live cause held game 2's window.** What is proved is
  the shape (an AI-owned window at that exact upkeep, with the stack pinned and
  the human's input discarded) and one concrete live cause that produces it (the
  non-mana extra payment, fix 2). The replay path's cause — `Act()` never being
  called while loading — is proved outright but is replay-only. The floor is
  deliberately cause-agnostic for that reason: it bounds the state, whatever
  reached it.
- The stall floor's live behaviour has not been observed in a real game, only in
  the fixture and in the two replays. It has never fired in the 1220-test suite
  outside its own pin, which is the only evidence I have that it does not fire on
  legitimate windows.
- The 20 s / 300-tick and 1200 s budgets are chosen, not measured against a real
  slow-AI-speed setting or a live LLM opponent. If an LLM window is ever yanked,
  the stderr line names it.
- **Open, not fixed: game 2's transcript will not replay past its turn-8 equip.**
  The engine auto-taps two lands for Bloodforged Battle-Axe's `{2}:equip` and the
  recorded equip-target click is then refused 60 times; the ability goes on the
  stack with no target from the record. Reproduced identically on
  `wagic-859186536-w53corpus` and on this lane's build, so it is not a wave-53
  regression. It matters because it blinds the transcript tool on any human game
  containing an equip.
- Not swept: whether other holders (a target chooser, an open menu, `mExtraPayment`
  on its own) can pin the ring the same way with no bound. Only the interrupt
  window is floored here.
