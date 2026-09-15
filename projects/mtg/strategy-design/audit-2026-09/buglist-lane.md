# Bug-list lane — engine items 2, 3, 4, 5 and 8

Branch `buglist-2026-09`, base `982394151`. Items 1, 6, 7, 9 and 10 of
`audit-2026-09/bug-list.md` were not this lane's.

Every CR line quoted below was grepped out of the official CR on disk
(`~/.gatelogs/engine-audit/cr.txt`); every card fact was read off the card's own
`text=`/`auto=` line in `bin/Res/sets/primitives/`.

**On RED.** Item 2's RED is measured on the lane's BASE binary (built from
`982394151`), because its fixture uses only commands that already existed. Items 3
and 5 need suite commands this lane adds, which the base binary silently ignores,
so their RED is measured with the ENGINE (or placement) HUNK ALONE reverted and
everything else in place — the fix-lane-2 convention. Item 8's RED is the gate
itself failing when one initializer is removed.

---

## 2. MULTIPLE-CHOICE AFTER COMPACTION — why the scripted `choice 0` never landed

**The question.** fix-lane-2 item 4 built a Heartless Act fixture with a new
`shrinkactionlayer` command and dropped it: *"the scripted `choice 0` did not reach
`doReactTo` at all on either binary once the layer had been shrunk, so it failed on
the fixed build and passed on base — the opposite of the property under test ...
something else in the scripted modal path is disturbed by compaction, and it may be
a real defect this lane has not found."*

**The answer: the fixture's own click routing, not the mode-menu dispatch.**
`TestSuiteAI.cpp:521-562` holds the suite's menu-default guard: while a menu is
armed, any PENDING command that is not one of a named list of exemptions causes the
suite to answer the menu itself — Cancel if the menu is cancelable, otherwise
**option 0** — and then `suite->currentAction--` re-queues the command for the next
tick (`src/TestSuiteAI.cpp:584-589`). `shrinkactionlayer` was never added to that
list. So the order a fixture wrote as

```
<cast>            -> the modal menu arms
shrinkactionlayer -> compact the layer under it
choice N          -> select a surviving mode
```

actually ran as: the default answered the menu with **option 0** (a modal
`MenuAbility` menu is `must`, so it has no Cancel row), the shrink then ran with
`menuObject == 0`, and the script's own `choice N` reached `ActionLayer::doReactTo`
with no menu armed — `doReactTo`'s body is wholly inside `if (menuObject)`
(`src/ActionLayer.cpp:887-913`), so it returned having done nothing. That is exactly
"never reached `doReactTo`", and it explains the inverted signal too: on BASE the
default's option 0 happened to be the mode the old fixture asserted.

**Measured, on the base binary**, with a fixture that casts Inquisitor Exarch
(`mtg.txt:58485`, `auto=choice life:2 controller` / `auto=choice life:-2 opponent`)
and then asks for mode 1:

```
TESTSUITE command: shrinkactionlayer 3 [w86ia_multichoice_mode_after_compaction.txt]
TESTSUITE menu default: first option pending='shrinkactionlayer 3' [...]
ActionLayer::doReactTo 104
TESTSUITE command: shrinkactionlayer 3 [...]
TESTSUITE shrinkactionlayer: removed 3 element(s); layer is now 101
TESTSUITE command: choice 1 [...]
TESTSUITE choice !!!
==life problem for player 0. Expected 20, got 22==
==life problem for player 1. Expected 18, got 20==
==Test Failed !==
```

Mode 0 resolved (P1 22), mode 1 never did (P2 20) — the default picked the mode, the
shrink ran on an idle layer and `choice 1` was a no-op.

**Fix.** `shrinkactionlayer ` joins the menu-default exemption list
(`src/TestSuiteAI.cpp:547-557`) and the `keyword` list that suppresses the card
lookup (`src/TestSuiteAI.cpp:578`). Harness only; no engine behaviour changes.

**GREEN** `bin/Res/test/w86ia_multichoice_mode_after_compaction.txt` — the fixture
the fix-review asked for and fix-lane-2 could not deliver: it **selects a surviving
real mode after compaction**. The shrink removes 3 elements under the armed menu
(`layer is now 103`), `choice 1` is dispatched through `getMenuControlId` →
`kMenuRowIsMode` → `ButtonPressedOnMultipleChoice(1)`, and P2 ends on 18 with P1 on
20. Both the wave-82 refusal and the pre-wave-82 stale-slot form are still pinned by
`w82ef_multichoice_stale_row_id.txt`, which is unchanged; this fixture adds the
end-to-end half — an AI's/script's legal answer is not merely *accepted*, it is
*executed*, after the layer moved.

**One latent observation, not repaired, for the bug list.**
`ActionLayer::ButtonPressedOnMultipleChoice` (`src/ActionLayer.cpp:977-996`) scans
`for (int i = mObjects.size()-1; i > 0; i--)` — index 0 is never examined — and when
the scan finds nothing it leaves `currentMenuObject == -1`, which the next branch
tests against `kCancelMenuID` (also -1) and treats as a cancel, calling
`endOfInterruption(false)`. A triggered `MenuAbility` at `mObjects[0]` would
therefore be answered as a cancel. I probed `mObjects[0]` in a live suite game
(temporary instrumentation, removed): `menuText='Ability' aType=-1
isMenuAbility=0` — slot 0 is the first rule registered at game init and never
leaves, and `ActionLayer::getAbility` starts its own scan at 1 for the same reason.
So it is unreachable today, by a positional convention nothing enforces. I did not
change it: the only behaviour that would move is the not-found case losing its
`endOfInterruption`, and I have no witness that says which way that should go.

---

## 3. forgetElement RE-ADD CAVEAT — guarded, not proved away

**The caveat** (fix-review-3-fable E2/E5): *"X's OWN slot is nulled by
`SAFE_DELETE(garbage[i])`, but a second slot for X would not be — possible only if
an element is `addToGame`d again after `moveToGarbage`; the pre-W83 code nulled all
of X's slots. Not shown reachable ... worth a `getIndexOf`-style guard in
`moveToGarbage` if it ever is."*

**Prove or guard: guarded.** `addToGame` has 221 call sites across `src/` and
`include/`, and most of them are inside ability constructors, clones and resolve
paths that can be reached from any card script in a 26,000-card pool. A proof over
that set would be a claim I could not stand behind, and the guard costs one pointer
scan on a path that already walks the same vector.

**Fix, two independent guards.**
* `src/MTGAbility.cpp:7894` — `MTGAbility::addToGame()` REFUSES (returns 0, traces)
  when the action layer already holds this element in `garbage`. The element is
  scheduled for deletion; putting it back in `mObjects` leaves storage the next
  `cleanGarbage()` frees inside the live layer, which is the `#W82-EH` / core-474128
  shape the whole facility exists to close.
* `src/ActionLayer.cpp:69` — `moveToGarbage` never pushes a duplicate slot. This one
  holds whatever route put the element back, including one that does not go through
  `addToGame`. They are deliberately not one guard.

**RED fixture** `bin/Res/test/w86ib_garbage_readd_refused.txt`, with both engine
hunks reverted and everything else in place:

```
TESTSUITE assertgarbagereaddrefused: addToGame ACCEPTED an element already in the
layer's garbage (returned 1, mObjects index 104) - the sweep is about to free it and
the layer would keep updating freed storage
TESTSUITE assertgarbagereaddrefused: the element holds 2 garbage slot(s);
cleanGarbage would delete it 2 time(s)
==Test Failed !==
```

**GREEN**: refused, one slot, swept once. The new command
(`src/TestSuiteAI.cpp`, `assertgarbagereaddrefused`) builds the shape out of the
engine's own parser and layer calls and repairs the layer on the RED path so the
fixture reports rather than double-frees.

---

## 4. mRevealAbove IDENTITY — a stable id instead of an address

**The residual**, named by fix-lane-4's own weakest-evidence paragraph:
*"`mRevealAbove` holds raw card pointers compared against the live library, so a
freed instance whose address is later reused by a card that IS in the library would
read as 'still above' and hold the parked card down for one draw ... it is an
identity assumption, not a proof."* CR 121.1: *"A player draws a card by putting the
top card of their library into their hand."* CR 701.20b: *"Revealing a card doesn't
cause it to leave the zone it's in."*

**What identity was available.** `MTGCard::mtgid` is the PRINTING id — every copy of
a card shares it, and a library of copies is the normal case, so it cannot key an
above-set at all. `MTGCardInstance` had no per-instance id and `Targetable` carries
none. So the fix adds one: `MTGCardInstance::mInstanceId`
(`include/MTGCardInstance.h:197`), stamped once in `initMTGCI`
(`src/MTGCardInstance.cpp:391`, the single path every constructor runs through) from
a monotonic counter bumped with `__sync_fetch_and_add` (the suite builds games on
several worker threads; two instances sharing an id would be the very false positive
this removes), never reused and never written again. In-class initialiser `= 0`, so
0 stays reserved for "no id".

**Fix.** `mRevealAbove` is `vector<unsigned int>`
(`include/MTGCardInstance.h:206`); the selective-reveal walk records
`toMove->mInstanceId` (`src/AllAbilities.cpp:321`); the draw compares
`library->cards[k]->mInstanceId == parked->mRevealAbove[a]`
(`src/MTGGameZones.cpp:472-480`). A card whose storage was recycled is no longer
found, because the new occupant has its own serial. A zone move that COPIES a card
produces a fresh instance and a fresh id, which is the right answer here too — the
copy is not the card that was recorded.

**GREEN.** The behavioural pins from fix-lane-4 and its predecessors are unchanged
and all pass on the new binary: `w85hb_mill_above_parked_card.txt` (the milled-over-
parked-card shape the brief named), `w84gd_selective_reveal_logical_top.txt`,
`w83fc_partial_library_logical_top.txt`, `w69bg_reveal_parked_library_no_deckout.txt`.
New pin `bin/Res/test/w86ic_reveal_above_identity.txt` +
`assertinstanceidentity`: over a library of four Mind Stones, one printing id
(4436), four distinct non-zero instance ids. It fails if anyone goes back to the
printing id, and it fails if `initMTGCI` stops stamping.

---

## 5. PASS-FLOOR ARM — produced on demand, and observed FIRING

**The item.** *"The wave-71 re-arming-menu shape has not been produced on demand in
four lanes; the floor's firing is pinned by predicate only."* CR 117.3d: *"If a
player has priority and chooses not to take any actions, that player passes."*

**Why four lanes could not produce it — the mechanism, not the board.** The suite
already has the shape's ingredients: `aideclineface <card>` (wave-71) arms the
model's own "Decline — do nothing" answer through
`DecisionManager::applyMenuChoice(choice = -1)`, unbounded, and the heuristic then
re-proposes the same card and re-arms the identical menu. What the suite did NOT
have is the answer given in the right PLACE. `AIPlayerBaka::Act` samples
`menuOpenBefore` at its own entry (`src/AIPlayerBaka.cpp:5941`) and counts a tick as
menu-answering only if the menu stood at that sample and is gone after
`computeActions` (`:6004`). The wave-71 arm answered from `TestSuiteAI::Act`, BEFORE
the base `Act` ran — so every tick read as "had nothing to do", `mMenuPassHold` reset
to 0 every tick, and the hold run could never reach 24, let alone the 200 identical
ticks after it. The live seam (`AIPlayerGPT::chooseMenuAction`) answers from INSIDE
`computeActions`, which is why the wave-70 hang logged `menu pass floor reached ...
not passing` **2,584,166** times and no fixture ever logged one.

**Fix (harness only; no engine change).** `aideclineface <card> <budget> seam`:
`TestSuiteAI::computeActions()` (new override) gives the decline where the model seam
gives it, and `TestSuiteAI::usesDeclinedFaceLatch()` returns false while seam mode is
armed — the pre-`#W71-BP` engine, which is the engine that hangs. The ordinary
(pre-`Act`, latched) placement is untouched, so `ai_mdfc_decline_livelock_w71bp.txt`
and `ai_mdfc_decline_sticky_w71bp.txt` are unchanged and still green.

**RED** `bin/Res/test/w86id_pass_floor_fires_on_rearming_menu.txt` with the ` seam`
token dropped (the four-lane arrangement), same board, same 21 filler ticks:

```
TESTSUITE assertpassfloorfired: player 1 expected at least 1 NO-PROGRESS forced
pass(es), got 0 (forced passes of any kind: 0) - the seat answered the same re-armed
menu over an unchanged board and never passed
==Test Failed !==
```

**GREEN — the floor firing, observed for the first time:**

```
AIPLAYER: menu pass floor fired at 224 on NO PROGRESS (a legal action remains and
has been refused every tick of the stall) (forced pass #1, no-progress 1)
```

400 `menu pass floor reached ... not passing` ticks in the run, then the arm fires at
tick 224 (24 hold + 200 identical), twice over the game, over a board that never
moved with a legal land drop and a castable Raging Goblin both offered and both
refused every tick. The seat reaches `end` instead of standing still in main phase 1.
New command `assertpassfloorfired <seat> <min>` reads `mMenuPassNoProgress` — the
NO-PROGRESS subset, not any forced pass, so an empty-seat close cannot satisfy it.
The predicate pins from fix-lane-4 (`w85ha_pass_floor_cleanup_window.txt`,
`w85ha_pass_floor_blockers_window.txt`) are left in place as asked.

---

## 8. The uninitialised scalars

`check-ctor-init.py --notes` reported **55**. They were three different things, and
only the third was real.

* **27 were mis-attributed across classes.** `header_scalar_members` took everything
  from `class X` to END OF FILE, so in a header declaring several classes
  (`TestSuiteAI.h` declares `TestSuiteAI`, `TestSuite` and `TestSuiteGame`;
  `MTGAbility.h` declares 20+) every later class's members were reported against the
  first class's constructor — a finding no edit to that constructor can clear,
  because the members are not its own. `TestSuiteGame::mAiPendingTicks`,
  `Trigger::mOnce`, `VerticalTextScroller::mNbItemsShown` and 24 others were all
  already initialised in their own constructors. FIX: match the class's own braces.
* **A few were a base class matched by name prefix.** `raw.find('class CardSelector')`
  also matches `class CardSelectorBase`, whose `mDrawMode` IS initialised (default
  argument `DrawMode::kNormal`). FIX: `\bclass\s+<name>\b`.
* **40 were real**, once the scan was honest — including 8 the old scan had been
  missing entirely (`ATutorialMessage::mSH/mSW`, `Tournament::mSpeed`,
  `TransitionBase::mElapsed`, `StoryDialog`, `TaskList::mState`,
  `NetworkGameObserver::mForwardAction`), because a helper class whose own
  `<class>.h` does not exist was skipped rather than looked for in the header named
  after the FILE. FIX: that fallback, so tightening the scope did not cost coverage.

All 40 are now initialised **in their constructors' initializer lists** (the two
`GameObserver.cpp` ones byte-spliced — that file is CP1252 + CRLF):
`ATutorialMessage` ×2, `DeckMetaData` ×2, `GameObserver::mAbilityEpoch`,
`NetworkGameObserver::mForwardAction`, `GameStateAwards` ×2,
`GameStateDuel::mTranscriptMenuDone`, `Tournament::mSpeed`, `GameStateMenu` ×4,
`GameStateOptions::mState`, `GameStateShop` ×2, `TransitionBase::mElapsed`,
`IconButton` ×9, `MTGCardInstance::mPropertiesChangedSinceLastUpdate`,
`SimpleButton` ×8, `SimplePad` ×2, `SimplePopup::mClosed`, `TaskList::mState`.
Three of those (`mCreditsYPos`, `Tournament::mSpeed`, `ATutorialMessage::mSH/mSW`)
are written NOWHERE in the tree and read unconditionally; the rest are written in a
helper (`IconButton::init`) or on a later branch. `IconButton`'s nine are stated as
defaults even though `init()` sets them from the arguments — a helper call is not an
initializer, and the object is indeterminate between the two.

**The enforced set.** With the debt at zero, the gate is now the WHOLE tree
(`ENFORCE_ALL_CLASSES = True`), not a named-class list: a count printed on every
build is a number people stop reading; an empty set that FAILS the build the day it
stops being empty is not. **RED, measured**: with `SimplePad`'s one initializer
removed, `check-ctor-init.py src` prints both members and exits 1. **GREEN**:
`check-ctor-init: OK (118 file(s))`, no note, exit 0; `--selftest` OK.

---

## Gates

* **Suite, `THREADS=1`, detached unit, `make -B` binary**: `1312 tests (0 failed),
  81 AI tests (0 failed)`; `==Test Failed !==` 0, `==Test timed out` 0. Baseline
  1309 + 80; this lane adds three scripted fixtures (`w86ia`, `w86ib`, `w86ic`) and
  one AI fixture (`w86id`). No rerun was needed — none of the three known flakes
  (`lifeline`, `merrow_reejerey`, `intrepid_adversary_repeated_payment`) failed.
* **PARSETEST**: `=== self-test: 7275 passed, 0 failed ===` (baseline 7275 — engine
  and harness work, no parse-relevant string changed).
* **`check-ctor-init.py`**: OK (118 files), 0 outside the enforced set, which is now
  every class. `--selftest` OK.
* **Encoding**: `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.
  `GameObserver.cpp` was byte-spliced; no primitives were touched.
* **Heuristic A/B gate**: see the table below.

### Heuristic A/B gate

`tools/baka-ab.sh <worktree>/projects/mtg/bin ~/.gatelogs/buglist-baka 30 4`
(630 games, `WAGIC_AI=baka`, no LLM) vs `archives/baka-ab/badbba7d3`:

```
accepted: 630 games, 0 unfinished, turns med 17.0 max 88
candidate: 630 games, 0 unfinished, turns med 17.0 max 93
deck  acc  cand   z
 123   89/180    85/180  -0.42
 125   77/180    86/180  +0.95
 126  119/180   117/180  -0.22
 130   41/180    42/180  +0.13
 146   79/180    75/180  -0.43
 152  108/180   105/180  -0.32
 162  117/180   120/180  +0.33
PASS
```

**PASS, exit 0**: 630/630 games finished — no crash, no hang, no unfinished game —
median turn count identical, and no per-deck shift reaches |z| >= 2, so there is no
FLAG to explain. The largest is deck 125 at +0.95.

**THE GATE CAUGHT A DEFECT I INTRODUCED, AND IT IS WORTH NAMING.** The FIRST run of
this A/B, on the first candidate binary, flagged deck 126 at **z = -2.16** (119/180
-> 99/180). It was not noise and it was not one of items 2-5: it was item 8. I had
initialised `GameObserver::mAbilityEpoch` to **0**, and `GameObserver::cleanup()`
states the invariant in its own comment — *"never equals a fresh ability's 0"* —
because `MTGAbility::mConditionEpoch` starts at 0 (`src/MTGAbility.cpp:7797`) and
`MTGAbility.cpp:8789` treats a match as a CACHE HIT. An epoch of 0 made every
ability in every game SKIP its first condition evaluation. The suite was green on
that binary (1312/0, 81/0) and PARSETEST was 7275/0; only the A/B saw it. Corrected
to 1 (`src/GameObserver.cpp:116`) — which is also the honest fix, since the
constructor never set the field at all and `cleanup()` is not on the construction
path, so before this lane the value was whatever the stack held.

---

## Weakest evidence

**Item 3's guard is a guard because I could not do the proof, and that is a real
gap.** The brief offered "prove no code path does that (cite every `addToGame`
caller)" as the alternative, and I did not attempt it: there are 221 call sites, a
large share of them inside ability constructors, clone paths and `resolve` bodies
reachable from arbitrary card script, and an enumeration I could not complete
honestly is worse than a guard. So what ships is a refusal plus a de-duplication,
with a fixture that builds the shape synthetically out of the engine's own parser
and layer calls — and NOTHING in this lane shows a shipped card reaching it. If the
path is unreachable the guards are inert; if it is reachable I have not named the
card that reaches it. The one thing the fixture does prove is the CONSEQUENCE: on
the reverted binary the element really does come back into `mObjects` and really
does hold two garbage slots, so the caveat's mechanism is confirmed even though its
trigger is not.

**Item 4 pins the identity, not the collision.** The defect is an ADDRESS REUSE —
a freed `MTGCardInstance` whose storage the allocator hands to a card that is in the
library — and I cannot drive the allocator from a script, so there is no fixture in
which the old code fails and the new code passes on that specific event. What is
measured is (a) the behavioural family is unchanged (`w85hb`, `w84gd`, `w83fc`,
`w69bg` all green) and (b) the identity the comparison now uses is per-INSTANCE and
non-zero, distinct across copies that share a printing id (`w86ic`). The claim
"a recycled address can no longer read as still-above" rests on ids never being
reused, which is a property of a monotonic counter, not an observation.

**Item 5's fixture proves the floor fires in the shape; it does not prove the shape
is what hung the wave-70 game.** The wave-70 evidence is a 1.23 GB stderr and a
2,584,166-line counter, not a repro, and the shape I built is assembled from the
suite's own decline arm rather than from a model. It has the same board, the same
card class (a modal double-faced land), the same choke point
(`DecisionManager::applyMenuChoice(-1)`) and the same engine (latch off), and it
produces the same log line at the same tick counts — but a fixture that MATCHES a
hang is not the hang. What is new and solid is the negative half: with the decline
given from the pre-`Act` placement, `mMenuPassForced` is 0 over the same run, which
is a sufficient explanation for why four lanes saw nothing and means the four-lane
absence of evidence was an instrument artefact, not evidence of absence.

**Item 2's explanation is complete; its fix is a harness fix, and a possible engine
defect is left open.** The `choice 0` mystery is closed with a measured trace, and
the fixture now selects a surviving mode. But the `ButtonPressedOnMultipleChoice`
scan that never examines `mObjects[0]`, and its conflation of "not found" (-1) with
`kCancelMenuID` (-1), are real and unfixed. I probed one live game and found slot 0
holding a game rule, and the engine's own `getAbility` starts at 1 — so it is
unreachable by convention, not by construction, and I chose not to change a shared
dispatch path on a convention argument with no witness. It belongs on the next bug
list.

**Item 8's 55 became 40, and the arithmetic deserves scrutiny rather than applause.**
Fifteen of the original findings were never real; I did not verify every one of the
27 cross-class re-attributions individually, I verified the mechanism (end-of-file
scan, prefix match) and spot-checked four (`Trigger::mOnce`,
`VerticalTextScroller::mNbItemsShown`, `TestSuiteGame::mAiPendingTicks`,
`CardSelectorBase::mDrawMode`). Of the 40 that were real, most are UI classes that a
headless corpus never constructs, so the A/B could not have caught a wrong VALUE in
them — and the one engine-relevant value I did get wrong (`mAbilityEpoch`) it caught
only because the class is constructed once per game. `IconButton`'s nine and
`SimpleButton`'s eight are stated defaults that `init()` immediately overwrites, so
their values are untested by anything.
