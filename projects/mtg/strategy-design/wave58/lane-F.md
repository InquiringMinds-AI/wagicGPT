# Wave-58 lane F — F1: a stale menu id indexed past `ActionLayer::mObjects`

## Mechanism (established, not guessed)

A `SimpleMenu` row built by `ActionLayer::setMenuObject` stores **only the row's
position in `ActionLayer::mObjects`** (`abilitiesMenu->Add(i, ...)`, where `i` is the
loop index over the action layer). That position is not an identity.
`ActionLayer::removeFromGame` erases from the **middle** of `mObjects`
(`mObjects.erase(mObjects.begin() + i)`), so every ability that leaves the game while a
menu is armed shifts the meaning of every later row:

* if the removed element sat at the highest index a row names, that row's id becomes
  exactly `mObjects.size()` — one past the end. That is the F1 abort: index 175, size 175,
  `DecisionContract.cpp:367`, reached from `AIPlayerGPT::computeActions` on the tick after
  the Lost Mine of Phandelver venture abilities were removed.
* if it sat lower, the row's id stays **in range and names a different ability**. That is
  the silent half of the same bug and is the more dangerous one — bounds-checking alone
  would have left it in place.

So the fix is not a bounds check on a position; it is to stop treating the position as the
identity. Every id -> `mObjects` mapping now resolves through the **ActionElement the row
was actually built from**.

## What shipped

`include/ActionLayer.h`, `src/ActionLayer.cpp`
* New `vector<ActionElement *> menuRowElements` — one entry per row of the armed menu, in
  row order, filled in `setMenuObject` (both the plain and the `abilitiesTriggered` menu,
  whichever is kept) and `setCustomMenuObject`; `NULL` for the cancel row and for
  multiple-choice rows (whose ids are not `mObjects` indices at all).
* `removeFromGame` nulls every `menuRowElements` entry naming the departing element
  **before** the erase. Pointer-identity scan, no dereference — safe whatever `destroy()`
  just did. This also closes the ABA hazard (a new element reusing the freed address).
* `bool getMenuControlId(int menuIndex, int & slot)` — row index -> the ability's
  **current** index. Returns the stored id when nothing moved, **re-points** to
  `getIndexOf(armed)` when the vector was compacted, and returns **false** when the row's
  ability has left the game. Ids `<= 0` pass through untouched (-1 = cancel; id 0 keeps the
  engine's own long-standing "not a selectable option" convention that every reader already
  applies), so no existing menu shape changes meaning.
* `bool getLiveMenuSlot(int controlid, int & slot)` — the same mapping from the other end,
  because `SimpleMenu` hands `JGuiListener::ButtonPressed` a row **id**, not a row index.
* `doReactTo` validates the row and does nothing when it is stale (the menu stays armed;
  the engine re-asks next tick). It passes the row's original id to `ButtonPressed`, which
  re-resolves it, so the two ends cannot disagree.
* `ButtonPressed` (the human path) resolves `controlid` through `getLiveMenuSlot`. A stale
  id falls into the pre-existing "an id we don't recognize — do nothing, don't clear the
  menu" branch. Nothing legal is removed and no window closes.

`src/DecisionContract.cpp`
* `buildMenuChoice` (the abort site) resolves each row through `getMenuControlId` and
  **skips** a stale row instead of indexing it. Surviving rows remain legal choices.
  `applyMenuChoice`'s existing `optionTexts` staleness gate then drops an answer that was
  built against the old row set and the consumer's next poll gets a fresh request — the
  re-ask is already in the contract, this just feeds it correct rows.
* `inspectMayBatch` resolves the same way plus an explicit `slot < mObjects.size()` check.

`src/AIPlayerBaka.cpp`
* `selectMenuOption` skips stale rows (it used to index them to call `getEfficiency`).

`src/AllAbilities.cpp`
* `MenuAbility::reactToChoiceClick` bounds-checks its `control` parameter before
  `mObjects[control]` — the last unchecked caller-supplied index into the action layer.

## Fixture

`bin/Res/test/w58F_stale_menu_compaction.txt` (+ `_tests.txt`), test-only card
`W58 Stale Menu Witness` in `bin/Res/test/lexicon/test_primitives.txt` (two `{0}` activated
rows), driven by a new suite command `assertstalemenu <card>` in `src/TestSuiteAI.cpp`.

Nadaar + Lost Mine of Phandelver could NOT be driven in a fixture: the abort needs the
dungeon-venture abilities to be added and removed across two AI ticks, which the suite
script DSL cannot sequence. The synthetic card reproduces the *shape* exactly — arm the
menu, remove the **highest-index** reacting ability, poll `buildMenuChoice` — so the stale
id lands one past the compacted vector, which is the F1 signature (index == size).

The command asserts the surviving option count and that the removed ability's label is no
longer offered, then disarms the probe menu so the rest of the script runs normally.

**RED verified by rebuild, not by inference**: the five engine files were reverted to base
`e663e3f56` with the new test command kept, rebuilt, and the fixture run —
`Aborted (rc=134)`, `stl_vector.h:1253 ... Assertion '__n < this->size()' failed` for
`_Tp = JGuiObject*`, i.e. byte-for-byte the F1 assertion. Log
`~/.gatelogs/w58-F-red2.log`. The fix was then restored and rebuilt; GREEN in
`~/.gatelogs/w58-F-green2.log`.

## Gate (worktree `w58-F`, hermetic build, MemoryMax=4G)

* Suite, `WAGIC_TESTSUITE_THREADS=1`: **1246 tests, 0 failed, 0 timed out**; **61 AI tests,
  0 failed**. (Baseline 1245 + this lane's fixture.)
* PARSETEST: **3455 passed, 0 failed** — unchanged; no AIPlayerGPT strings were touched.
* `git diff | grep -c $'\357\277\275'` = 0.

## Predictions (falsifiable, wave-58 corpus)

1. Zero SIGABRTs with `DecisionManager::buildMenuChoice` in the backtrace across the whole
   wave-58 corpus, including dungeon/venture decks (deck146, deck152). One such abort
   refutes the fix.
2. Any game that previously would have aborted now shows, in stderr, one or more
   `ActionLayer: menu row N re-pointed A -> B` lines (or a silently skipped row) and then
   continues to a normal end-of-game record — i.e. the abort is replaced by progress, not
   by a hang or a dropped turn.
3. No new stall/softlock class: the count of games ending in a timeout does not rise
   relative to the wave-57 A/B arms. (A rise would mean the "no decision this tick"
   path fails to make progress and is a refutation.)

## What I did NOT verify

* No live model probe was run; this is fixtures + PARSETEST only.
* The **re-point** branch (`getIndexOf` finds the ability at a new index) is not exercised
  by the fixture — the fixture removes the highest-index row, which is the out-of-range
  case. The in-range-but-wrong-ability case is reasoned from the erase semantics and
  covered by the same code path, but it is unproven by a test.
* The human `ButtonPressed` path was not driven through a real stale menu in a windowed
  run; it is covered by code inspection and by the suite's ordinary menu traffic
  (1246 tests, 0 failures) proving no regression on the live path.
* Whether the Nadaar / Lost Mine sequence itself is now clean end-to-end — that needs the
  wave-58 corpus (prediction 1).
* Vita/PSP/Android builds were not compiled; the change is C++14 and adds no includes, but
  only the SDL build was exercised.
