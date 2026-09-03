# Wave 56, lane Z (hotfix) — cancelling a spell inside an interrupt window

## The defect

Owner Vita play report, vpk15, 2026-09-03, tag `bug`, his words:

> "engine forced me to kill my own creature, not allowing me to cancel casting a
> spell that i hadnt declared targets for. this is rules noncompliant, and also
> not fun."

Transcript `psp-work/logs/20260903-vpk15/transcript-1788460441-player_deck1-vs-ai_baka_deck100.txt`,
lines ~118-172. On the AI's turn 9 he accepted the interrupt offer (`p1.yes`),
tapped Swamp + Forest + Forest by hand, clicked Putrefy (`p1.hand[2] 0putrefy`),
then pressed cancel **eighteen** times — each logged as `p1.endinterruption` —
and finally clicked `p1.battlefield[2] 1thornweald archer`, his own creature and
the only legal target (the AI's board was five Plains). Putrefy destroyed it.

## Mechanism (verified by reading, then by a RED fixture on the pre-fix binary)

1. `MTGPutInPlayRule::reactToClick` (src/MTGRules.cpp) pays only **after**
   `game->targetListIsSet(card)`. Clicking a targeted spell therefore arms
   `GameObserver::targetChooser` + `cardWaitingForTargets` with **nothing paid** —
   the pool still floats and the card is still in hand.
2. The cancel button is `JGE_BTN_SEC`. `DuelLayers::CheckUserInput` dispatches
   stack -> combat -> avatars -> action -> hand -> card selector, so
   `ActionStack::CheckUserInput` sees the key **first**. While
   `observer->isInterrupting`, its branch treated every SEC (and every `trigger`
   press) as "end the window": it called `endOfInterruption()` and returned true.
   The key never reached `CardSelector`'s `JGE_BTN_SEC -> observer->cancelCurrentAction()`,
   which is the ordinary human cast-cancel.
3. Nothing cleared the chooser when the window closed, by any route. So the armed
   chooser **outlived its window**. On the next window the first click on a legal
   target ran `GameObserver::cardClick`'s `if (targetChooser)` branch ->
   `toggleTarget` -> `TARGET_OK_FULL` -> `cardClick(cardWaitingForTargets)`, and
   the spell resolved on a target the player never declared.

`cantCancel` is not involved (only `setMenuObject`/`setCustomMenuObject` set it).

## The fix (engine, human-seat behaviour only)

New `int ActionStack::cancelPendingChoice()` (src/ActionStack.cpp). It mirrors the
human's ordinary cancel path (`CardSelector` -> `GameObserver::cancelCurrentAction`):

* nothing pending (no `ActionLayer::isWaitingForAnswer()`, no
  `observer->targetChooser`) -> return 0, caller behaves exactly as before;
* an ability choice pending whose `ActionLayer::cancelCurrentAction()` returns 0
  (the `cantCancel` mandatory contract) -> return 0, caller falls through
  untouched;
* otherwise cancel the waiting action and `SAFE_DELETE(observer->targetChooser)`,
  return 1.

**(a) The cancel button.** In the `observer->isInterrupting` SEC/trigger branch,
`cancelPendingChoice()` is consulted before `endOfInterruption()`. If it cancels,
the branch returns true **without ending the interruption**: the spell is
cancelled, the card stays in hand, the floated mana is untouched, and the window
stays open. A second press then ends the interruption exactly as before. Extra
payment (`observer->mExtraPayment`) is released on the same press, as it already
was.

**(b) Safety net.** `ActionStack::endOfInterruption` calls
`cancelPendingChoice()` before clearing `observer->isInterrupting`, so an armed
chooser cannot outlive a window that was actually TAKEN — including via the
suite's `endinterruption` DSL command and `ActionLayer`'s two `kCancelMenuID`
paths.

`ActionStack::cancelInterruptOffer` deliberately does **not** get the net, and
this was found empirically, not assumed: `observer->targetChooser` is global to
the observer, not per-seat, and `cancelInterruptOffer` runs on the ordinary
DECLINE path — every time a seat waves off an offer while the ACTIVE player is
mid-cast. With the net there, the decline killed the caster's own in-flight
chooser and `counter_unless_pay_x.txt` + `spell_blast_counter_matching_mv.txt`
went red in the full gate. Removing it restored both. `endOfInterruption` is safe
because it runs only once a seat has taken the window, so the pending choice is
that seat's own. Residual: the stall watchdog releases a held window through
`cancelInterruptOffer`, so a chooser armed by a seat that then goes silent for
the whole watchdog period can still be orphaned. Not fixed here — a per-seat
chooser owner is what would fix it properly.

**AI seats are byte-identical.** Leg (a) is human-only by construction:
`DuelLayers::CheckUserInput` is gated on `(!isAI)`, so no AI seat ever reaches the
key branch. Leg (b) is guarded on `observer->isInterrupting->playMode != Player::MODE_AI`
— `AIPlayer` sets `MODE_AI`, a human is `MODE_HUMAN`, a scripted suite seat is
`MODE_TEST_SUITE`. So the net covers the human seat and fixtures and skips every
autonomous AI seat.

The pay-after-targets order is unchanged; `cantCancel` semantics are unchanged.

## New suite DSL command: `cancelbutton`

`cancelbutton` feeds `JGE_BTN_SEC` to the stack layer (the first layer in the duel's
input order, and the owner of the branch under test). It was necessary because
the real key path is human-only and a scripted suite seat always reports
`isAI()`, so leg (a) was otherwise unreachable from the DSL. It is registered in
both `cannotAnswer` and `keyword` guard lists in TestSuiteAI.cpp.

It is `cancelbutton` and not `cancel` because fixture actions are lowercased and
the CARD named **Cancel** is clicked by name in `countered_spell_narration.txt` —
the short name shadowed that click and took the fixture red in the full gate.

## Fixtures (Res/test/, registered in _tests.txt)

RED proven on the archived pre-fix binary `archives/wagic-d0515fae3-w56step1`
(base `d61dad89c` adds only `strategy-design/wave56/review-carry.md` on top of it),
GREEN on the lane build:

| fixture | base | lane |
|---|---|---|
| `w56Z_interrupt_cancel_orphan_chooser.txt` — the report: leg (b), `endinterruption` route | FAILED | Successful |
| `w56Z_interrupt_cancel_button_keeps_window.txt` — leg (a), `cancelbutton` route, `assertinterrupting` around each press | FAILED | Successful |
| `w56Z_interrupt_targeted_cast_resolves.txt` — NEGATIVE: declared target still resolves after `endinterruption` | Successful | Successful |

Caveat on the second row: on the base binary `cancelbutton` does not exist, so its RED
comes from the zone assert (Putrefy in the graveyard, Thornweald Archer dead,
pool spent) rather than from a differing `cancel` semantic. The zone assert is
the defect signal; the `assertinterrupting p2` / `assertinterrupting none` pair
is what discriminates "cancelled the spell" from "cancelled the window" on the
lane build, and `assertinterrupting: expected none got p2` is what the base
reports.

The existing `blue_elemental_blast_*` interrupt-cast fixtures stay green (full
suite below) — that is the second, pre-existing negative control.

## NOT verified

* **The `cantCancel` / must-choice fixture the brief asked for was not written.**
  A mandatory menu or must-target armed *inside an interrupt window* is not a
  cheap idiom in this harness and no existing fixture has that shape. The
  protection is structural — `cancelPendingChoice()` returns 0 whenever
  `ActionLayer::cancelCurrentAction()` refuses (`cantCancel && validTargetsExist()`),
  and every caller then behaves exactly as it did before — plus whatever coverage
  the full suite's existing must-menu fixtures give. **This is a real gap**, and
  it is the first thing to write if a mandatory-choice regression ever shows up.
* **No Vita/PSP build and no GUI drive.** The whole verification is the desktop
  headless suite. The owner's actual defect surface (a real cancel press on the
  Vita's mapped SEC button) is untested on hardware; the `cancel` DSL command
  drives the same `ActionStack` branch the real key reaches, but not through JGE
  or the Vita input map.
* X spells: a cancel after `setX` was announced clears the chooser but does not
  reset `card->setX`. That matches the pre-existing ordinary human cancel path
  (which does not reset it either), so nothing regressed — but it was not made
  right, and `MTGPutInPlayRule::reactToClick` has its own `setX` cleanup for the
  un-castable case.
* Extra-cost (`mExtraPayment`) casts cancelled by leg (a) were not fixture-pinned;
  the existing release idiom was left in place ahead of the new cancel.

## Gate

Worktree `worktrees/lanes/w56-Z`, branch `w56-lane-Z`, base `d61dad89c`.

| leg | baseline | this lane |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1234 / 0 failed | **1237 / 0 failed** (+3 = the three new fixtures) |
| AI tests (same run) | 51 / 0 | **51 / 0** |
| `timed out` occurrences | 0 | **0** |
| PARSETEST (`WAGIC_GPT_PARSETEST=1`) | 2736 / 0 | **2736 / 0** (unchanged — no parse or render string touched) |

The first full gate ran 1237 / **3 failed**; both causes are recorded above (the
`cancelInterruptOffer` net, and the `cancel` command name shadowing the Cancel
card) and both were fixed rather than accommodated. The numbers in the table are
the re-run after those fixes.
