# Wave-54 lane D — chooser asks + collapse (D3, D6, D8, D25)

Base: master `42b3bf5da`. Baseline re-confirmed on this worktree's seeded binary before any
edit (`~/.gatelogs/w54-D-base-*.log`): PARSETEST **2258 / 0 failed**; suite **1219 tests
(2 failed — `lifeline.txt` + `merrow_reejerey.txt`, the known concurrency-only solo-green
pair) + 49 AI (0 failed), 0 timeouts** — exactly the brief's numbers.

Final gate on this branch, **single-threaded (`WAGIC_TESTSUITE_THREADS=1`, ground truth)`**:
PARSETEST **2288 / 0 failed** (+30 cases); suite **1219 tests (0 failed) + 51 AI tests
(0 failed), 0 timeouts** (+2 AI fixtures; the concurrency-only pair is green solo, as
expected). Every wagic invocation ran under `MemoryMax=4G / MemorySwapMax=0`; no kill.
`git diff | grep -c $'\357\277\275'` = 0.

## What changed

### D3 (HIGH, = R163) — the framing is gated on the DESTINATION ZONE, not the source name
`src/AIPlayerGPT.cpp`

`handRemovalVerb` matched the lowercased chooser-source-name + ability-verb string in order,
and `"path to exile"` contains `exile`, so the verb was decided by the SOURCE CARD'S NAME
before anything read `moveTo(mybattlefield)`. All 8 corpus renders told the beneficiary of a
compensation search to pick the card it could best AFFORD TO LOSE.

* `unwrapMoverAbility()` — walks the wrapper nesting (`NestedAbility`, `MultiAbility`) to the
  `AAMover` the chooser feeds, the way `unwrapDamagerAbility` does; two movers = ambiguous,
  no answer.
* `moverGainDestination()` — **pure over the script's own `moveTo(...)` argument string**
  (never `destinationZone(NULL)`, which dereferences the target on `previousbattlefield`):
  `battlefield` / `hand` when the card ends up under the CHOOSER's control, `""` for
  exile/graveyard/library and for any `opponent…` destination.
* `handRemovalVerb(lc, relocate, gainDest = "")` — a gain destination answers `""`. The
  default argument keeps every wave-29 call byte-identical.
* `buildHandGainAsk()` — the new framing, gated so it can ONLY fire where the loss detection
  would have fired (the verb table matches, every candidate is the chooser's own card):
  `CHOOSE A CARD TO PUT ONTO THE BATTLEFIELD: Path to Exile's effect puts a card you choose
  from the list below onto YOUR battlefield - each option is a card you GAIN, NOT a card you
  lose … Pick the card you MOST NEED right now (with basic lands, the colour your hand and
  your board are shortest of).` The answer instruction is the loss ask's, unchanged.

### D6 (MED, = R166) — `selfInflicted` resolves through `storedSourceCard`
`src/AIPlayerGPT.cpp`

`selfInflicted = tc->source->controller() == this` read TRUE for the OPPONENT's annihilator,
because an ability granted to a player is parsed onto a **nameless dummy card owned by the
victim** (`ATargetedAbilityCreator::resolve`, `AllAbilities.h:4470`). The chooser source now
takes the same `storedSourceCard` hop the stack line, token creation and mana production
inside that keyword already take, when the source's display name is empty. `effectName` was
already correct (it goes through `resolveOwningCardName`); only the controller test was wrong.

### D8(a) (MED, = R168) — the collapse reaches rows with NO instance ordinal
`src/AIPlayerGPT.cpp` (`groupNumberedRows`, `joinNumberedRows`, `kOptionRangeNote`)

**Mechanism this lane pinned on disk, which changes what D8(a) is.** `askModel` already calls
`groupNumberedRows` (lane P shipped that in wave 53) and the ability / forced-sacrifice menus
already collapse — `130v125` seq 181 in the wave-53 corpus renders
`1-14. Mountain #1-#14 [land] [your battlefield] - "R" x14`. The seam that fails is the one
whose rows carry **no `#N` handle at all**: `splitRowHandle` returns false, so the row was
unkeyable and 17 byte-identical `Mountain [land] [your library] - "R"` printed 17 times
(`130v125` seq 69), as did the twelve identical `Vampire (2/2) …` damage-order rows
(`152v123` s23-s37).

* `groupNumberedRows` now keys handle-less rows on the ROW itself in its own namespace
  (`\x03` prefix) with rank 0, so the existing `(rank, index)` sort falls through to list
  order for them; scattered identical rows gather at the first member and unique rows
  (declines) still never move.
* `joinNumberedRows` collapses a run of byte-identical handle-less rows to the label range
  alone — `1-17. Mountain [land] [your library] - "R" x17` — with **no invented `#a-#b`**.
* `kOptionRangeNote` gains one clause for that form (additive; the ordinal sentence is
  unchanged): *"When the collapsed row carries NO #N ordinal, the options in the range are
  interchangeable copies of the same thing and any number in the range picks one of them."*

### D8(b) (MED) — a de-duplicated option list of length 1 resolves internally
`src/AIPlayerGPT.cpp` (`identicalInterchangeableRows`, `askModel`), `include/AIPlayerGPT.h`

Gated on (a) having shown the choice vacuous, and narrower than the docket allows: the rows
must be **byte-identical AND ordinal-free**, so the render itself says there is exactly one
distinguishable outcome. This is the existing "one option = no decision" rule (askModel
already returns 0 without a model call) applied to N indistinguishable outcomes. If the engine
prints anything that tells the rows apart — an ordinal, a decline row, one differing tail —
the model keeps the ask. Counted onto the gameend record as
`identical_option_asks_resolved`, the way `mana_only_windows_skipped` is, so a corpus can
still see the window; the caller's per-option narration for the taken row is still consumed.

### D25 (LOW, = R185) — the forced-loss list is ordered cheapest-first
`src/AIPlayerGPT.cpp`

`sacrificeCostRank`: lands rank below everything (the header's own example of the right pick),
then ascending mana value; the sort is **stable**, so battlefield order survives inside each
tier. `targets` / `opts` / `narrOpts` are permuted together and the trailing
`Done - no further targets` escape stays last; picks are pointers, so the apply path
(`DecisionManager::applyChooseTarget`) is untouched.

## Suite pins (RED with only the fix logic reverted, GREEN after — verified by rebuild)

Both defects live entirely in the WORDS over an unchanged zone layout, so — as with lane P's
wave-53 fixtures — they need a render surface the suite cannot otherwise see. `TestSuiteAI::Act`
now records a third register, `ownCardChooserRegister()` (`src/TestSuiteAI.cpp`,
`include/TestSuiteAI.h`), which runs the **production builders** (`handRemovalVerb`,
`buildHandRemovalAsk`, `buildHandGainAsk`) over the live chooser and emits nothing unless the
chooser is a removal/move over the seat's own cards.

* `bin/Res/test/w54_path_search_is_framed_as_a_gain.txt` — P1 Paths P2's Serra Angel; P2's
  basic-land search chooser. Pre-fix P2's register reads `EXILE ONE OF YOUR OWN CARDS`;
  post-fix `CHOOSE A CARD TO PUT ONTO THE BATTLEFIELD` + `Pick the card you MOST NEED`, with
  both loss strings as `nonarration:` negatives.
* `bin/Res/test/w54_granted_sacrifice_names_the_opponent.txt` — Innocent Blood: its own half is
  genuinely self-inflicted (P1, source = the sorcery) and its granted half is the opponent's
  (P2, source = the dummy), so one fixture pins **both** branches from the two chairs.
  Pre-fix P2's register reads `your own effect (Innocent Blood)`.

Both appended to `bin/Res/test/_tests.txt`. **The RED was taken with the register PRESENT and
only the two fix branches defeated** (destination gate short-circuited, `storedSourceCard` hop
deleted at both sites), rebuilt and re-run: 5 narration assertions failed across the two
fixtures. The file was then restored byte-for-byte from a pre-revert copy and rebuilt.

## Falsifiable predictions (wave-55 corpus)

* **D3** — asks whose candidate zone is `mylibrary` (or any own zone) and whose destination is
  the battlefield or the hand, framed as a loss: **0** (was 8/8). No ask that is genuinely a
  removal loses its loss framing: the three wave-29 loss headers still render.
* **D6** — sacrifice/edict asks sourced from a permanent the seat does not control and framed
  as `your own effect`: **0** (was 18 of 26). `your own effect` still renders on the seat's
  own loot/Path/cost sacrifices.
* **D8(a)** — `ask` records whose printed option block repeats one byte-identical ordinal-free
  row more than twice: **0** (was 17 rows in one render, 14 in another, 12 in a third). Every
  such render carries the `x N` label range and the range note.
* **D8(b)** — asks whose de-duplicated option list has length 1: **0** (was 16 asks / 147 rows
  / 2.9 min); `identical_option_asks_resolved` on the gameend records is **> 0** and its sum
  is within a few of that 16. No out-of-range fallback on a damage-assignment-order menu.
* **D25** — forced-loss lists of >= 2 rows in which a land appears below a non-land: **0**.
  Row 1 of every forced-sacrifice ask is a land whenever the seat controls one.

## What I did NOT verify

* **No live model probe was run.** Every prediction is about the next corpus; nothing here was
  measured against a model this lane.
* **D8(a) does NOT close the docket's ability-menu half.** The docket's 28-row specimen
  (`125v126` seq 220) is **two identical Staff of Nin sources x 14 targets**: the per-target
  collapse already fires (`1-3. … Vampire #1-#3 … x3`), and what remains uncollapsed is the
  duplication across the SOURCE ordinal (`Staff of Nin #1` vs `#2`). Collapsing that needs a
  **two-dimensional** range (`Staff #1-#2 targeting Vampire #1-#3`), and the printed-range
  grammar's decode rule ("5 is the first instance, 6 the next") cannot describe a
  cross-product truthfully — an undecodable range is worse than 28 honest rows under the trust
  doctrine. I did NOT ship it; it should be carried as its own item with that constraint
  stated. The docket's "> 12 rows over one verb+source" prediction is therefore **not**
  discharged by this lane for the two-source case.
* **The forced-sacrifice mega-menu was already collapsing before this lane.** D8's
  "fourteen byte-identical Mountains" bullet reads as a render failure; on the corpus binary
  those rows print as a single `x14` row. The real cost in those prompts is the ~99% game-log
  share (D32, the owner's item), not the option block.
* **D8(b)'s interchangeability test is the RENDER's, not the engine's.** Two objects that
  differ in some way the engine never prints will be treated as interchangeable. That is
  deliberate under the trust doctrine (a difference that matters owes the model a token), but
  it means a future render omission could silently make a real choice vacuous. The docket's
  "the engine's own handle map says the members are interchangeable" was implemented as
  "the printed row carries no ordinal and every row is byte-identical".
* **D25's rank was not exercised by a suite fixture.** `sacrificeCostRank` needs live
  `MTGCardInstance`s, so it is neither PARSETEST-pure nor covered by the two fixtures; only
  the reordering's index-safety is argued by construction (pointer-based apply, parallel
  permutation, `Done` kept last). Its first real evidence is the next corpus.
* **`ownCardChooserRegister` does not apply the `allMine` candidate test** the live ask does
  (enumerating candidates needs the decision contract and an AI seat). It can therefore fire
  in a fixture where the live ask would not; it emits only for removal/move choosers, and no
  existing fixture's narration changed.
* **No hermetic `make -B`.** The gate ran on an incremental build over the seeded `objs-sdl`.
  The seeded binary's baseline matched the brief's numbers exactly (2258 / 1219+2 / 49), which
  is the evidence that the seed is not stale; a shipping gate should still rebuild.
* Baseline was measured multi-threaded (2 concurrency-only failures) and the final gate
  single-threaded (0 failures). The comparable invariant is 0-failed solo; I did not re-run
  the baseline single-threaded.
