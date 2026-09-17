# STEP ONE lane, 2026-09-17 — bug list items 11 and 12

Branch `step1-2026-09-17` (worktree `worktrees/lanes/step1`, seeded from master at 5ab472929).
Two commits, one per item: `001fd2112` (item 11, #W87-JA) and `3047da5e2` (item 12, #W87-JB).
Rules cited were read from `~/.gatelogs/engine-audit/cr.txt` by grep (700.2, 700.2b, 603.3c, 117.3d).

---

## 11. `ButtonPressedOnMultipleChoice` — sentinel and slot-0 semantics made explicit by construction

**What I found (base, `src/ActionLayer.cpp:989-1017`).** The multiple-choice dispatcher found the
MenuAbility to answer with `for (int i = mObjects.size()-1; i > 0; i--)` looking for a `MenuAbility`
with `triggered` set. Three separate facts rode that loop:

* `i > 0` — index 0 was never examined. What sits there is the first element `Rules::initGame`
  registers (a game rule; the wave-86 probe read `menuText='Ability' aType=-1`). Nothing enforces
  it: `ActionLayer::getAbility` (`src/ActionLayer.cpp:10-21`) and `ALoseAbilities::addToGame`
  (`src/AllAbilities.cpp:10658, 10669`, "0 is not a mtgability...hackish") skip it by the same
  convention.
* `int currentMenuObject = -1` was both "not found" and `kCancelMenuID` (`-1`), so the not-found
  case fell into `else if (currentMenuObject == kCancelMenuID)` and ran `endOfInterruption(false)`
  as if the player had cancelled.
* "Highest triggered MenuAbility" is not "the MenuAbility whose menu is armed". `triggered` is never
  cleared by an answer. A MenuAbility whose chosen mode is a MAY hands a `MayAbility` clone to the
  layer and LINGERS — processed, still triggered — until that clone is answered
  (`MenuAbility::testDestroy`, `src/AllAbilities.cpp:9460`: `getIndexOf(mClone) != -1`). If a second,
  lower MenuAbility then arms ITS menu, the scan finds the lingering one first and hands it the
  answer; `MenuAbility::reactToChoiceClick` re-clones the already-chosen mode, `processAbility`
  returns 0 on `processed` (`src/AllAbilities.cpp:9473`), the answer is eaten, and the armed menu's
  owner re-arms the same question on the next tick — the re-arming-menu shape the pass floor
  (bug list item 5) exists to survive. Two READERS used the same scan and skipped slot 0 the same way:
  `AIPlayerBaka::selectMenuOption` (`src/AIPlayerBaka.cpp:3001-3010`) ranks the modes of whatever
  MenuAbility the scan returns, and `DecisionContract.cpp:228-240` `currentMenuAbility` renders them
  for the LLM seat — so the modes ranked/rendered could belong to a different MenuAbility than the
  one the answer would execute on.

**Why the wave-86 fixture could not see it.** Two triggered MenuAbilities both re-arm on every tick
(`MenuAbility::Update`, `src/AllAbilities.cpp:9427-9432`), ascending, so the LAST one to Update
owns the displayed menu — and it is also the highest one the scan finds. The scan and the display
agree in that shape; they disagree only when a triggered MenuAbility ABOVE the armed one does not
re-arm, which is exactly the processed-and-lingering case above.

**RED.** New suite command `assertmultichoiceidentity` (`src/TestSuiteAI.cpp`) builds the shape out
of the engine's own objects: A (modes `counter(1/1,1)` / `counter(3/3,1)`, source and target the
host) and B (modes `may counter(0/1,1)` / `may counter(0/3,1)`, sourced by a second permanent so
its clone cannot arm over A's menu), both `resolve()`d, one `Update` (B's menu shows), B answered
with mode 0 (its clone is now live and keeps B in the layer), one more `Update` (A arms its own
menu below B), then A's menu answered with mode 0 through `ButtonPressedOnMultipleChoice(0)`.
Fixture `w87ja_multichoice_identity.txt`. On the base engine with this harness (counterfactual
build, engine files stashed):

```
TESTSUITE assertmultichoiceidentity: the answer to the ARMED menu (A) was not processed by A
(A processed=0, host 2/2 -> 2/2, expected +1/+1 from A's mode 0) - the lingering, already-processed
MenuAbility B above it took the answer, and A would re-arm the same question on the next tick
==Test Failed !==
```

**Fix (`001fd2112`).**
* `include/ActionLayer.h`: `static const int kNoLiveMenuElement = -4` — the dispatcher's
  "this row names no element the layer still holds", distinct from `kCancelMenuID` (-1), 0, and
  `kMenuRowIsMode` (-3). `setCustomMenuObject(..., ActionElement * owner = NULL)`;
  `ActionElement * armedMenuOwner()`.
* `src/ActionLayer.cpp` `setCustomMenuObject`: every row of a multiple-choice menu (each mode and
  the cancel row) records `owner` in the W58-F row-identity map `menuRowElements` — the map that
  `removeFromGame` and `forgetElement` already null when an element leaves the game. A menu armed
  with no owner traces it.
* `src/ActionLayer.cpp` `ButtonPressedOnMultipleChoice`: the answered row (the argument, or the
  menu's cursor for the human path) resolves to `menuRowElements[row]`; its index is looked up LIVE
  with `getIndexOf`; `reactToChoiceClick(menuObject, row, liveIndex)` goes to that element; the
  owner is re-resolved by identity before `removeMenu` is set (the click can reshape the layer). No
  scan, no raw slot, no dependence on what sits at `mObjects[0]`. A row with no live owner is
  `kNoLiveMenuElement`, a named branch: the menu is unanswerable (its owner left the game), so it is
  closed and the interrupt window released — the behaviour the unnamed `-1` branch always had, now
  with a trace naming the row and the menu.
* `armedMenuOwner()` is the one way to ask "which MenuAbility does this menu belong to";
  `AIPlayerBaka::selectMenuOption` and `DecisionContract.cpp` `currentMenuAbility` use it instead of
  their own top-down scans, so the modes ranked / rendered are the modes the answer executes on.
* Arming sites pass the owner: `MenuAbility::Update` (`src/AllAbilities.cpp:9430`) passes `this`;
  `MTGPutInPlayRule`'s X announcement (`src/MTGRules.cpp:581`, byte-spliced, CP1252) passes `a1`.
  The suite's `assertmultichoicestaleid` arms without one (it never dispatches).

**GREEN.** All three seats' entries into the shared dispatcher, each pinned:
* `w87ja_multichoice_identity.txt` — A processed, host 2/2 -> 3/3.
* `w87jb_multichoice_human_seat.txt` — the HUMAN entry. New command `humanmenu <row>` drives the
  real key path: `SimpleMenu::CheckUserInput(JGE_BTN_DOWN/UP)` moves the cursor, `Entering()` on the
  row (what hover/arrival does), `JGE_BTN_OK` -> `JGuiController::CheckUserInput` ->
  `ActionLayer::ButtonPressed(id, rowId)` -> `ButtonPressedOnMultipleChoice()` reading the cursor.
  Inquisitor Exarch's trigger (mode 0 gain 2 / mode 1 opponent loses 2), row 1: P1 20, P2 18.
* `w87jc_multichoice_ai_seat.txt` — the HEURISTIC entry (`AIPlayerBaka::computeActions` ->
  `ButtonPressedOnMultipleChoice(doThis)`; the GPT seat's fallback and `DecisionManager::
  applyMenuChoice` take the same call). FORCEABILITY cast of Inquisitor Exarch; the seat ranks the
  modes and takes mode 0: P1 22, P2 20 (pinned from the observed run).
* `w86ia_multichoice_mode_after_compaction.txt` (unchanged) — the script/replay entry
  (`doReactTo -> ButtonPressedOnMultipleChoice(N)`) selecting a surviving real mode after
  `shrinkactionlayer 3`; still green. `w82ef`, `w82eg`, `w58F`, `w86id` (the rest of the menu
  family) green.

Nothing removes, caps or auto-answers a legal option: the identity map only changes WHICH element
receives an answer (the one whose rows are displayed), never the row set.

---

## 12. `addToGame` after `moveToGarbage` — the guard stays, and it is observable

**Decision: keep the guard and make it observable; ship the enumeration as evidence, not as a proof.**

**Enumeration (`tools/enum-addtogame.py`, output in the commit message and
`~/.gatelogs/step1-enum-addtogame.txt`).** 170 `addToGame()` CALL sites under `src/` and `include/`
(the lane-86 "221" counted declarations, definitions and comments too). By receiver:

| class | count | why it is on a live element |
|---|---|---|
| FRESH-VAR, `var = NEW ...` | 33 | allocated in the same function; a live allocation can never equal a garbage pointer |
| FRESH-VAR, `var = ...->clone()` | 67 | same |
| FRESH-VAR, factory-parsed (`parseMagicLine`, `getAbilities`) | 16 | same |
| BASE-CHAIN, `X::addToGame()` inside an override | 31 | `this` — reduces to the override's caller, one of the other rows |
| OTHER (existing pointer or `this`), each read | 23 | below |

The 23 read one by one: `AACopier`/`AAFlip`/`AAMorph`/`AAFrozen`/`AEquip::equip`/`AEquip::mutate`/
`AbilityFactory::magicText` iterate a vector `af.getAbilities(&v, ...)` filled in the same function
(fresh; `AEquip::unequip` clears `currentAbilities` before `mutate` refills it, `src/AllAbilities.cpp:
12202`); `GenericInstantAbility::addToGame` re-adds its NESTED ability (`include/AllAbilities.h:3317`),
owned by a wrapper that is itself added exactly once by its creator (14 `NEW GenericInstantAbility`
sites, all immediately added); `ALord::_added`/`ATeach::_added` add `ability->clone()`; the three
`delayedA` sites and `ATransformer`'s `aNew` are `parseMagicLine` results; `AADynamic::
activateMainAbility`'s `toActivate` is `mainAbility`, `NEW`'d in `resolve` on every path
(`src/AllAbilities.cpp:7267-7317`); `MenuAbility::processAbility`'s `mClone` is `abilities[choice]->
clone()` from `reactToChoiceClick`, guarded once by `processed`; `MTGDredgeRule` adds the
`MenuAbility` it just `NEW`'d; `Rules::addExtraRules` adds a fresh parse; and `ALoseAbilities::
destroy` (`src/AllAbilities.cpp:10731`) re-adds abilities it took out with `al->removeFromGame`
(`:10702`) — the one existing-pointer round trip, and it never touches the garbage (`removeFromGame`
"does not move the element to garbage", `include/ActionLayer.h`), and an element out of `mObjects`
cannot be swept into it by `Update` or `purgeDeadReferences`, which walk `mObjects` only.

That leaves `MenuAbility::resolve` (`src/AllAbilities.cpp:9436-9441`, `this->addToGame()`): every
`resolve()` of a MenuAbility is either a creator's call on the object it just `NEW`'d (18 sites) or the
stack's call on a clone it owns (`StackAbility::resolve` -> `ability->resolve()`, the ability being the
clone `fireAbility` handed to `addAbility`). I believe that is complete, and I cannot show it
statically: `resolve()` is virtual, called through `MTGAbility*` from the stack, from triggers, from
`AbilityFactory`, and from card-script paths; a static walk sees the call, not the receiver's
provenance. A proof that stops at "I believe" is the thing the wave-86 lane refused to ship, and I
refuse it for the same reason.

**What ships instead (`3047da5e2`).** The guard is unchanged in effect and now REPORTS:
* `MTGAbility::garbageReaddRefused` (static, `include/MTGAbility.h`) counts every refusal.
* Under `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)` the refusal prints
  `WAGIC addToGame REFUSED a garbaged element: class=<RTTI name> source=<card> menu='<text>'
  (refusal #N)` on stderr — so a corpus stderr names the element's class and source, i.e. the path
  to read, on the day the enumeration turns out incomplete. Release builds carry the counter only.
* `assertgarbagereaddrefused` (the w86ib command) now also requires the counter to move by exactly
  one on the refusal; **GREEN** with the suite log carrying the line
  `class=23GenericActivatedAbility source=Grizzly Bears menu='re-add witness' (refusal #1)`.
  (There is no separate RED for the observability half: the counter is new, so "silent refusal" is
  the state the base binary is in by construction.)

---

## Gates (all on the `make -B` binary of `3047da5e2`)

* Suite THREADS=1 (detached unit `step1-suite`, `~/.gatelogs/step1-suite.log`): **1314 tests (1
  failed), 82 AI tests (0 failed)** — baseline 1312/0 + 81/0, +2 scripted (w87ja, w87jb) +1 AI
  (w87jc). Both failure strings counted: `==Test Failed !==` 1, `==Test timed out (game never
  ended)==` 0. The one failure is the known flake `intrepid_adversary_repeated_payment`
  (tap-source order in the narration line); solo reruns: **6/10 fail on the accepted
  `wagic-62a5733ec-accepted` binary, 6/10 fail on this binary** — the same rate, not this lane's.
* PARSETEST from `bin`: **7314 passed, 0 failed** (unchanged; no parse-relevant string changed).
* `python3 tools/check-ctor-init.py src`: OK (123 files). The one new header scalar is a `static`
  (`garbageReaddRefused`), defined in the .cpp; the new ActionLayer members are none (the owner
  rides the existing `menuRowElements`).
* `git diff | grep -c U+FFFD` = 0 after every edit (MTGRules.cpp byte-spliced).
* HEURISTIC A/B GATE: below.

## Heuristic A/B gate

`bash tools/baka-ab.sh <worktree>/projects/mtg/bin ~/.gatelogs/step1-baka 10 4` (detached unit), then
`python3 tools/baka-ab-compare.py archives/baka-ab/62a5733ec ~/.gatelogs/step1-baka`:

```
accepted: 630 games, 0 unfinished, turns med 17.0 max 87
candidate: 210 games, 0 unfinished, turns med 16.0 max 89
deck  acc  cand   z
 123   77/180    33/60   +1.65
 125   66/180    26/60   +0.92
 126  105/180    42/60   +1.61
 130   48/180    14/60   -0.51
 146   91/180    22/60   -1.87
 152  122/180    31/60   -2.25 FLAG
 162  121/180    42/60   +0.40
PASS (hard); 1 deck(s) flagged for explanation
```

**HARD: PASS — 210/210 finished, 0 timeouts, 0 crashes.** Deck 152 (Bant Midrange) FLAGGED at z = -2.25.
Commits since the accepted binary touching the named files: `001fd2112` (this lane: ActionLayer,
AIPlayerBaka, AllAbilities, DecisionContract) and `f1a722daf` (gpt-layer section A, on master
before this lane's seed: AIPlayerBaka +7/-1, DecisionContract +10/-1; no baka-ab was archived for
it). I did not accept the flag on one run. What I did to answer it:

1. **A second 210-game run of the lane binary** (`~/.gatelogs/step1-baka2`): 152 at 37/60, z = -0.87,
   no flag. Pooled 420 lane games vs the accepted 630: 152 at 68/120 = 56.7% vs 67.8%, z = -1.96.
   ```
accepted: 630 games, 0 unfinished, turns med 17.0 max 87
candidate: 420 games, 0 unfinished, turns med 17.0 max 89
deck  acc  cand   z
 123   77/180    62/120  +1.51
 125   66/180    54/120  +1.44
 126  105/180    79/120  +1.31
 130   48/180    31/120  -0.16
 146   91/180    48/120  -1.80
 152  122/180    68/120  -1.96
 162  121/180    78/120  -0.40
PASS
   ```
2. **The pre-lane seed binary (5ab472929, `make -B` in a throwaway worktree, run from a bin dir
   whose `Res` is this worktree's) played 2 x 210 games** the same way, so the comparison is
   same-day, same-harness, same-Res, differing only by this lane's two commits. Seed pooled 420 vs
   lane pooled 420:
   ```
accepted: 420 games, 0 unfinished, turns med 17.0 max 86
candidate: 420 games, 0 unfinished, turns med 17.0 max 89
deck  acc  cand   z
 123   52/120    62/120  +1.29
 125   48/120    54/120  +0.78
 126   71/120    79/120  +1.07
 130   31/120    31/120  +0.00
 146   53/120    48/120  -0.65
 152   90/120    68/120  -2.99 FLAG
 162   75/120    78/120  +0.40
PASS (hard); 1 deck(s) flagged for explanation
   ```
   152: 90/120 = 75.0% (seed) vs 68/120 = 56.7% (lane), z = -2.99 — a FLAG that does not go away
   with power, concentrated in 152 vs 123 (13/20 -> 4/20), vs 125 (17/20 -> 10/20), vs 126 (12/20 -> 7/20).
3. **Mechanism probes on the lane binary** (temporary `fprintf` instrumentation, reverted with
   `git checkout`, rebuilt, binary md5 identical to the gated one): over 126 heuristic games with
   219 multiple-choice dispatches, the element the OLD top-down scan would have answered and the
   element the identity map answers were the SAME in 219/219 (`W87-PROBE DIVERGE` = 0), and the
   post-click owner index was STABLE in 104/104 (the old code's stale-slot `removeMenu` write hit
   the owner every time). `AIPlayerBaka::selectMenuOption`'s `currentMenu` is the same object by
   the same argument; `DecisionContract`'s `currentMenuAbility` is never called by the heuristic
   seat (`src/AIPlayerBaka.cpp:3420`). No `kNoLiveMenuElement`, `armed with NO owner`, or
   `addToGame REFUSED` trace appeared in any of the 840 + 126 games' stderr.
4. **Seeded, paired games.** Selfplay seeds `srand(time(0))` (GameApp.cpp:103; the per-observer
   generator is then drawn from `rand()`), so an `LD_PRELOAD` shim that fixes `time()` makes a game
   reproducible: the lane binary played the same seed twice to the identical result and identical
   1335-line stderr. **18 seeded games (6 seeds x 152v123 / 123v152 / 152v125) on the two binaries:
   18/18 identical results (winner, both life totals, turn) and identical stderr except the
   `Time elapsed while loading` lines.** On every state those games visit, the two binaries are
   trace-identical.

**Verdict I can stand behind:** the two binaries play the same game on the same seed, and the only
code path the lane changed in heuristic play dispatched identically on every one of 219 observed
decisions. I therefore cannot explain the deck-152 shift as an intended play change, and I cannot
point at a line that causes it; by the gate's rule that leaves "treat it as a regression", and I
am NOT accepting the binary on my own authority — the orchestrator/owner decides with the evidence
above. My own reading (inference, not a finding): with 18/18 paired games identical the shift is
most likely sampling on unpaired time-seeded runs (the one-per-second seeds also collided 1-3
times per run), which the current gate cannot distinguish from a real 10-point move at n = 120.

**Harness finding for the bug list (not fixed here, outside scope):** `baka-ab.sh` games are seeded
by launch second, so the accepted and candidate runs never play the same games; a
`WAGIC_SELFPLAY_SEED` control would make the gate PAIRED (same seeds on both binaries, compare
game-by-game) — the 18 paired games above settled in minutes what 840 unpaired games could not.
The `time()` shim used (compiled with `gcc -shared -fPIC`), for reproduction:

```c
#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
static time_t fake(void){ const char * e = getenv("FAKE_TIME"); return e ? (time_t) atol(e) : 0; }
time_t time(time_t * t){ time_t v = fake(); if (t) *t = v; return v; }
int gettimeofday(struct timeval * tv, void * tz){ (void) tz; if (tv) { tv->tv_sec = fake(); tv->tv_usec = 0; } return 0; }
```
Run: `FAKE_TIME=1700000010 LD_PRELOAD=./libfaketime.so WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1
WAGIC_SELFPLAY_FAIRHAND=1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 WAGIC_SELFPLAY_DECK0=152
WAGIC_SELFPLAY_DECK1=123 WAGIC_AI=baka ./wagic` (under the memory-cap scope, from `bin`).

Results dirs: `~/.gatelogs/step1-baka`, `step1-baka2`, `step1-baka-pooled` (lane),
`step1-baka-seed`, `step1-baka-seed2`, `step1-baka-seed-pooled` (pre-lane seed binary),
`step1-probe-ab`, `step1-probe-ab2` (instrumented). The gated lane binary is at
`~/.gatelogs/step1-wagic-3047da5e2-gated` (md5 a8a1936d77eb...). Nothing was copied into
`archives/baka-ab/` — that is the acceptance step, which is not mine to take.

---

## Weakest evidence

**Item 11's RED shape is built by hand, and I have not named a shipped card that reaches it.** The
lingering MenuAbility needs a chosen mode that is itself a MAY (its clone keeps the owner alive)
AND a second MenuAbility below it that arms while the first lingers. `assertmultichoiceidentity`
assembles that from `parseMagicLine` and two `NEW MenuAbility`s; no corpus record or Vita report was
traced to it. What the fixture proves is the MECHANISM the item names — the top-down scan hands an
answer to an element that is not the menu's owner, and the answer is eaten — and that the identity
dispatch closes it; what it does not prove is that a real game has produced it. The slot-0 half is
closed by construction (the dispatcher and both readers no longer index or skip slot 0 at all); I did
NOT add an assertion reserving slot 0, and the two remaining `i > 0` scans (`getAbility`,
`ALoseAbilities::addToGame`) keep the convention untouched — they are outside this item's dispatch
path and changing them is a shared-path change with no witness.

**The not-found branch keeps its old behaviour on my judgement, not on a witness.** A row whose
owner has left the game now traces and closes the menu with `endOfInterruption(false)`, exactly
what the unnamed `-1 == kCancelMenuID` branch did. I chose that because a multiple-choice menu
whose owner is gone is unanswerable and `closeUnanswerableMandatoryMenu` deliberately skips
multiple-choice menus (`src/ActionLayer.cpp`), so leaving it armed would hold the layer; but no
fixture drives the branch, because nothing in the suite removes a MenuAbility from under its own
armed menu. If a corpus log ever shows the new trace line, that is the branch to read.

**Item 12 is a guard with a witness, still not a proof, and the enumeration's power is bounded by
its own classifier.** The 116 FRESH sites are classified by a 25-line look-back for `NEW` /
`clone()` / factory patterns and the BASE-CHAIN rule is "reduces to the caller"; I read the 23
OTHER sites but only spot-checked the pattern classes (the four with the most sites) by hand. The
dynamic `resolve()` class is where the argument rests on ownership conventions (the stack owns a
clone; creators add what they `NEW`), and those conventions are exactly what a 15-year-old engine
does not enforce. The counter and the stderr line turn "we would never know" into "the corpus
stderr says class and source" — that is the whole of the improvement, and it is inert until the
day it fires. The w87jc AI-seat pin is likewise a non-regression pin (the heuristic seat's mode
choice worked before this change): it holds the seat to the SAME dispatcher, it is not a RED.

**The A/B flag on deck 152 is the weakest point of this lane, and it is unexplained.** Two binaries
that are trace-identical on 18 paired seeded games and identical on 219/219 observed dispatches
should not differ by 18 points on one deck over 120 games each — yet the unpaired runs say they do
(z = -2.99 against the same-day seed binary). Either the shift is sampling that the unpaired,
time-seeded gate cannot rule out, or a state exists that neither the 18 paired games nor the
instrumented 126 games visited. I could not find such a state, and I am not claiming it does not
exist; I am reporting the flag as the gate requires and leaving the accept/reject call where it
belongs. The gate itself cannot see item 11's change unless a Baka game hits the lingering shape
(none of 966 games traced it), and item 12's diagnostic never fires in a clean run, so even a
clean table would have said "no regression on the shared paths", not "the fixed shape was
exercised".
