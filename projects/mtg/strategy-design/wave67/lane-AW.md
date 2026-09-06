# Wave-67 lane AW — I4, I9a and the row MEDs (the numbers a row must carry)

Base: master `ff06cc432` (wave-67 step-one brief). Branch `w67-lane-AW`, worktree
`worktrees/lanes/w67-AW`. Files changed: `projects/mtg/src/AIPlayerGPT.cpp` (only).
Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); **no kill under the cap on any
run**. Gates ran detached (`w67-AW-gate`, `w67-AW-red2`, `w67-AW-gate2`) with a foreground
until-loop wait; the binary was freshness-checked (`ls -la bin/wagic`) after every make, and no
build was ever chained behind a counting grep. Every edit was a python `rb`/`wb` byte splice;
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0** after every one. Every comment tagged
`#W67-AW (<item>)`.

Everything here is a RENDER change: none of it changes what the engine does with a card, an
option or an answer, so no suite fixture can be RED on base and none is claimed. RED was produced
the way lanes AL/AQ produced it — the six mechanisms reverted to their wave-66 behaviour with the
new cases already in place, binary rebuilt. See **RED-on-base**.

I9a is the one item that changes which rows the PROMPT LISTS. It does not change the engine's
menu, `req.optionTexts` (the staleness key), or the answer index: the pick is mapped back through
`shownToFull` before anything is clicked, which is the discipline the Flip-Side filter has used
since wave 57. Nothing volatile entered `mPromptTail` or an ask key — no counter, clock or running
total was added anywhere.

---

## What shipped

### I4 — the library ceiling folds the draws already on the stack, and the number rides the row and the ask
(engine HIGH-3; deck125 HIGH-2 and HIGH-3.)

The wave-66 ceiling reserved a flat **1** card and the rows that spend the library said nothing
about it. `125v123` s130 (T59, the lategame specimen) printed two Staff of Nin `Draw 1` triggers in
its OWN `ON THE STACK` block, reserved 1, recommended `X=13` from a library of 14 — the seat obeyed
exactly, drew 13, the triggers finished the library, and it lost at **44 life against 3**.
`125v126` s273 is the same shape without a stack (library 22, ceiling 19, the seat took X=21).
14 of 14 X windows on that seat took the maximum X.

Three surfaces now carry it, all built from the SAME reserve, so they cannot name different values:

* **the reserve** — `xLibraryReserveCount(drawStepSize, stackDraws)` and
  `xLibraryReserveWhy(...)`, both pure. The step's own SIZE comes from `drawStepExtrasScan` (the
  DRAW FORECAST's own scan — a Howling Mine makes the next undeclinable draw more than one card,
  and reserving 1 for it under-counts in the direction that decks the seat); the stack term comes
  from `stackPendingDrawsFor` (wave-63 R8), with the announcing card's own objects excluded so a
  spell never reserves against itself. `xLibraryReserveCount(1, 0) == 1` and
  `xLibraryReserveWhy(1, 0)` is wave 66's sentence byte for byte — the reserve moves only when
  there is something to fold.
* **the row** — `xLibraryRowClause(cards, library, stackPending)` appends
  `{library: this draws 13 of your 14 library cards - 1 left; 2 more draws already pending on the
  stack, which you cannot decline}` to every drawing X row. Its own brace channel, so the
  `{X pricing: ...}` block above it is byte-identical; added to `stripNarrationDecoration` so it
  never enters history.
* **the ask** — `xAnnounceLibraryNote(...)` puts the ceiling sentence in the ANNOUNCE_X header
  (`YOUR LIBRARY IS THE SECOND CEILING on this menu: ... X=19 is the largest value on this menu the
  library pays for`). This is 125 HIGH-3's fix: the cast window got 47,976 ms and 2,700 characters
  of correct library arithmetic (s272) and the window that commits the irreversible number got
  3,335 ms and one line (s273). The fact now lives in the second window too. Empty whenever the
  library does not bind, so the wave-66 header is byte-identical there.

Nothing is capped and no X is removed from the menu: the affordable ceiling still prints, the row
for every X still exists, and what is added is the number that decides.

### I9a — the unreachable repeat-pay rungs collapse to one row
(deck152 HIGH-1.)

`152v146` s11: the Intrepid Adversary ETB renders 21 rows (~2.7 kB) — `borderline.txt:57863-57883`
hard-codes 21 `auto=choice` lines — while the seat had **2 spendable mana**. Rows 3..21
(`add 2`..`add 20`) were behaviourally identical: each says `you have 2 spendable now, which pays
for 1 of them and stops`, i.e. each adds the same ONE counter. 5 occurrences on the corpus,
~13.5 kB.

`payRepeatCollapse(optionTexts, paid, hide, bandLow, bandHigh, bandPaid)` is pure over the engine's
own labels and the per-row payment counts the mana engine already computes for the `{repeat cost:}`
tag (`repeatPaid`). It collapses **only** when every add-N row's `paid` is KNOWN and IDENTICAL — an
undecided (-1) or a varying count means the rows are not proven identical and the full list prints
exactly as wave 66 printed it. The row kept is the band's LARGEST rung (the biggest ask the card
offers, and the one that is never worse if the mana moves before the payment: the header's own
partial-pay note says an over-ask never fails), and it carries
`{rows for 2 through 20 counters are collapsed into this one: your mana pays for 1 payment and
stops, so every one of them adds the same 1 counter - this row is the largest ask the card offers
and stands for all of them}` — so the band is NAMED and an answer reaching for a number inside it
has a row to land on. The last row is never hidden (it is the decline the index map reads), and a
menu that would be left with fewer than two shown rows keeps its full list.

### MEDs

* **M1 (deck146 MED) — the ATTACK TOTAL states the lethal verdict it already computes.** `130` s20
  printed `... puts them at 3.` and the reply wrote "which is lethal"; same word over 4 (`162` s21)
  and 6 (`126` s23). The line now closes the subtraction it just printed. The NEGATIVE verdict
  (`That is NOT lethal: they survive at 3 even with none of them blocked.`) needs no gate — a
  block, a blocking trigger and a life loop can only make a survivable total MORE survivable, so it
  is true whatever else is on the board. The POSITIVE one is scoped to the assumption it is
  computed under and rides the attack-punisher gate every other kill claim on this line rides. It
  is deliberately NOT gated on the opposing life loop: the line describes the unblocked
  declaration, and a loop of theirs is started by a block or a gain, neither of which happens on
  that line. (This is why `#W62-X D2 NEGATIVE blockGain 0` still passes untouched.)
* **M2 (deck130 MED-4) — the sorcery-speed bracket names the half that failed.** `130v125` s52
  printed the bracket under `Casting decision (Main phase 1, YOUR turn)` with a cycling ability on
  the stack four lines above: both halves true, the pair self-contradictory at the point of use;
  7,212 characters of argument, no coded line. The full rule is still printed and
  `; right now the stack is not empty (see ON THE STACK above)` is APPENDED. EVERY failing half is
  named, in the order the rule reads them — two can be false at once and naming only the first
  would be a true statement in the wrong scope.
* **M3 (deck152 MED-2) — the MDFC land row carries an arrival status.** `152v130` s22's Hengegate
  Pathway row stated none, and the model imported Overgrown Farmland's condition onto it. Ground
  truth checked BEFORE the edit: `borderline.txt:52388` (`auto={T}:add{W}`, no tap-on-ETB line) and
  Scryfall (Pathway MDFCs always enter untapped) — the render was wrong by omission, not the card.
  `mdfcLandArrivalTagFrom` asks the back face's own script through the SAME `landTapGateScan` the
  ordinary land row runs, so the two rows in one prompt cannot disagree: no gate found = `{it
  enters UNTAPPED - it makes mana this turn}`; a gate found in this class is conditional and gets
  what the ordinary row would say. The pay-or-tap and unconditional-tap classes are untouched.
* **M4 (deck126 MED-3) — the edict floor says how many of their bodies sit on it.**
  ⚠ **The review's premise is WRONG and was checked against the corpus before any edit.** deck126
  MED-3 says the Tribute row "prints no floor" at 20-23 creatures. It does: `126v123` s44 renders
  `they control 23 creatures - they choose which one - you gain at least 1 (their smallest
  toughness)` and s55 the same at 20. What was missing is how many of their bodies are AT the
  floor, which is what makes the floor the number to plan on rather than a lower bound to beat. The
  count comes from the same battlefield walk the floor came from. With no count supplied the
  wave-62 string is byte-identical.

## NOT shipped — deck125 MED-1, the third source of "max affordable X"

The ask was "find the third source", and it is found; no render change is shipped for it, for the
reason below.

`125v126` s272 (cast row) printed `{X pricing: max affordable X=20 (23 mana total)}` while the same
prompt's board line read `Mana available: 24 total` and the X menu one window later (s273) offered
`X = 21` with `24 floating mana`. **Three different quantities, three numbers:**

1. the board line counts **distinct untapped SOURCE CARDS** — `ManaEngine::potentialColorReach`
   under `FreeProducerPolicy` (`AIPlayerGPT.cpp`, the `Mana available:` emitter). It said 24.
2. the cast row's cap is **`ManaEngine::maxAnnounceableX`** (`ManaEngine.cpp:2195`), whose base is
   `potentialMana(p, AnyProducerPolicy)->getConvertedCost()` **plus the floating pool**. At s272 the
   pool was empty, so the base was `potentialMana` alone: 23. `potentialMana` is the
   one-ability-per-card sum — it is documented to UNDER-count against what the payment planner
   actually assembles, and 23-vs-24 is exactly that gap.
3. the menu's ceiling is the same `maxAnnounceableX` asked **after** the autotap, when the 24
   sources are in the pool as 24 mana: `24 - 3 = 21`.

So the disagreement is not a bug in either number — it is `potentialMana`'s per-card limit measured
before the tap against a source count measured as cards. I did not ship a change because the two
honest repairs both fail a standing rule: hedging the cast row ("the menu may offer one more")
teaches doubt about the render, which the trust doctrine forbids; and re-basing the cap on
`potentialManaPermissive` would OVER-promise an X the menu will not offer, which is the one error
direction this family cannot tolerate. The real repair is to base `maxAnnounceableX` on
`planPayment` (the truth), which is a `ManaEngine` change outside this lane's files and needs its
own fixture. **Docketed for a lane that owns ManaEngine.** 1 of 17 pairs on that seat disagreed.

---

## Gate

Detached unit `w67-AW-gate2`, hermetic `rm -f bin/wagic && make -f Makefile.sdl -j4`, binary
freshness-checked (`-rwxr-xr-x ... 57696824 Sep 6 09:35 bin/wagic`), MAKE_EXIT=0.

| gate | base (wave-67 brief) | this lane |
|---|---|---|
| PARSETEST | 4906 passed, 0 failed | **4966 passed, 0 failed** (+60) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1273 tests, 0 failed | **1273 tests, 0 failed** |
| AI tests | 67, 0 failed | **67, 0 failed** |
| timeouts | 0 | **0** |
| U+FFFD in the diff | 0 | **0** |
| killed under the 4G cap | — | **none, on any run** |

Three wave-60/62 PARSETEST cases were UPDATED (not deleted) because M1 adds a sentence to the
string they pin: `#W60-L B11 the ceiling and the proven floor`, `... a fully coverable attack
promises nothing`, `... NEGATIVE an uncomputed floor claims no floor`. Each keeps its original
assertion and gains the verdict clause; the `#W62-X D2` loop cases are untouched and still pass.

### RED-on-base — measured, not asserted

Unit `w67-AW-red2`: the six mechanisms reverted to their wave-66 behaviour (reserve back to a flat
1 with the flat sentence; the row clause, the ask note, the collapse and its tag returning empty;
the ATTACK TOTAL verdict and the sorcery-speed reason suppressed; the MDFC plain class silent; the
edict floor count suppressed), the new cases already in place, binary rebuilt and freshness-checked
(MAKE_EXIT=0, `09:33`/`09:34`).

**`=== self-test: 4938 passed, 28 failed ===`** — 28 RED, 0 with the mechanisms in:

| item | RED cases |
|---|---|
| I4 (reserve, row clause, ask note) | 11 |
| I9a (collapse + tag) | 5 |
| M1 (ATTACK TOTAL verdict; incl. the 3 updated wave-60/62 cases) | 6 |
| M2 (sorcery-speed half) | 2 |
| M3 (MDFC arrival) | 2 |
| M4 (edict floor count) | 2 |

The first RED build aborted inside the I9a block on an out-of-range `vector<bool>` read, because
the stub returned before sizing `hide`. That is an instrument failure, not a finding: the green
path always sizes it. The green test now asserts `hide.size() == labels.size()` on every path
before indexing, and the RED run was redone with a stub that sizes the vector first.

---

## Falsifiable predictions for the wave-67 corpus

1. **I4 (ceiling).** Zero seats deck themselves out of a won board on an X they were recommended.
   Concretely: no `gameend` record where the losing seat's library reached 0 within 3 turns of an
   ANNOUNCE_X answer whose row printed a `{library: ...}` clause naming a remainder BELOW the
   reserve the badge stated. FALSIFIER: one such record.
2. **I4 (rows).** `{library:` renders on every ANNOUNCE_X row of a drawing X spell — 100% of X
   windows on a `draw:X` card, 0 bare. And the max-X take rate on drawing X spells falls below
   14/14: at least one X window at a library the ceiling binds answers BELOW option 1. FALSIFIER:
   the take rate stays at 100% of maximum across ≥6 such windows.
3. **I4 (ask).** Every ANNOUNCE_X window whose badge carries a LIBRARY CEILING also carries
   `YOUR LIBRARY IS THE SECOND CEILING` in its header, and the median latency of those windows rises
   above the wave-66 3.3 s figure (the ask now has something to weigh). FALSIFIER: a ceiling-bearing
   menu with no header sentence.
4. **I9a.** No CHOOSE_MENU prompt in the corpus contains more than `paid + 2` `add N counters` rows
   on a card whose engine menu offers more, and every collapsed window's `chosen_text` maps to a row
   the engine actually holds (no `named_row_reask` and no `unparsed_reply` attributable to a hidden
   rung). Menu bytes on Intrepid Adversary windows drop from ~2.7 kB to under 0.6 kB.
   FALSIFIER: any collapsed window whose answer lands on the wrong engine index, or a
   `named_row_reask` naming a rung inside a collapsed band.
5. **M1.** Zero replies write "lethal" of an ATTACK TOTAL line whose own sentence says
   `That is NOT lethal` (wave 66: 3 on deck146's seat alone). FALSIFIER: one.
6. **M2.** Zero `unparsed_reply` records on a window whose hand line carries
   `[no cast row now: sorcery speed`, and no reply above 3,000 characters arguing with that bracket
   (wave 66: 7,212 characters at `130v125` s52, twice). FALSIFIER: either.
7. **M3.** Zero replies assert a tapped arrival for a Pathway-class MDFC land row (wave 66: 1, at
   `152v130` s22). FALSIFIER: one.
8. **M4.** Tribute-class rows at >5 opposing creatures with the floor at 1 are taken at a lower rate
   than wave 66's 2 of 2. FALSIFIER: every such row still taken.

---

## What I did NOT verify

* **No live probe was run.** Everything is proven at the PARSETEST seam with the corpus's own
  numbers (s130's library 14 / reserve 3, s273's 21-of-22, s11's 21 rows and `paid`=1, s20's
  4/11/14, s52's bracket, s22's Pathway script, s44's 23 creatures). No model call was made and no
  selfplay game was run, so nothing here is verified END TO END against a live prompt.
* **The two scans behind I4's reserve are not pinned by a new case.** `drawStepExtrasScan` and
  `stackPendingDrawsFor` are the prompt's own (the DRAW FORECAST's scan and the wave-63 R8 walk,
  each with existing coverage); what this lane proves is the FOLD — the arithmetic, the sentence,
  the ceiling and the three surfaces built from them. If either scan is wrong on a board, the
  reserve inherits it. The `gptStackPendingDrawsFor` suite door exists for a fixture that would
  close this; I did not write one.
* **I9a's answer mapping is proven by construction, not by a test.** `shownToFull` is the same
  vector the Flip-Side filter has used since wave 57 and the two filters now share one build loop,
  but no PARSETEST case exercises the index round-trip (it lives inside `chooseMenuAction`, which
  needs a DecisionRequest). The corpus is the gate: prediction 4 is written to catch a mis-map.
* **I9a's interaction with a mid-menu mana change is untested.** `repeatPaid` is computed once, when
  the rows are annotated; if something changed the seat's mana between that and the pick, the band
  would be stale. Nothing in the engine can do that inside one menu render, but I did not prove it.
* **M3's conditional branch never fired in any test with a real board.** A plain-class MDFC back
  face that DOES carry a tap gate is handled (it renders what the ordinary row renders) but no such
  card was found in the pool, so that branch is code-reviewed only.
* **deck125 MED-1 is diagnosed, not fixed** — see above. I did not identify WHICH of the 24 sources
  `potentialMana` credited with 0; the per-source list was not in the s272 prompt slice I read, and
  the mechanism does not depend on which card it was.
* **I did not adjudicate any other lane's predictions**, did not read the deck reviews outside the
  sections my items cite, and did not touch any file other than `AIPlayerGPT.cpp`.
