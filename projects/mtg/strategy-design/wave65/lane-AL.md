# Wave-65 lane AL — G1, G4, G5, G9 (prices that never reached the seam, or lied about the floor)

Base: master `537070ac2` (wave-64 close). Branch `w65-lane-AL`, worktree
`worktrees/lanes/w65-AL`. Files changed: `src/AIPlayerGPT.cpp`, `src/ActionLayer.cpp`,
`include/ActionLayer.h`, `src/DecisionContract.cpp`, `include/DecisionContract.h`,
`src/TestSuiteAI.cpp`. Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); **no
kill under the cap on any run**. The gate ran detached as `w65-AL-gate2` with a foreground
until-loop wait. `git diff | /usr/bin/grep -c $'\357\277\275'` = **0** after every edit (all
splices python `rb`/`wb`). Every comment tagged `#W65-AL (Gn)`.

All four items are RENDER / classification changes: none of them changes what the engine does
with a card, an option or an answer, so no suite fixture can be RED on base and none is claimed.
RED was produced the way lanes AG/AH/AK produced it — mechanisms reverted to their wave-64
behaviour with the new cases already in place, binary rebuilt. See **RED-on-base evidence**.

---

## What shipped

### G1 — the modal price reaches the seam the corpus exercises (engine HIGH-1, deck146 HIGH-3)

Wave 64's F6 rendered **0 of 23** mode rows over 3 real Silverquill Command windows, and so did
the wave-60 liveness tag and the wave-60 tap/untap tag. The seat took
`draw:1 controller && life:-1 controller && ability$!…!$ opponent` at **5 life** and again at
**3** (`146v130` seq 57/74) and lost that game at 0. The engine seat located the silence at the
`ctx`-gated family and could not run the binary. The code says why, and one half is now MEASURED.

**(a) `ctx` is NULL on exactly this card class.** `req.contextCard` comes from
`nameableCardPointer(validatedCardPointer(currentActionCard))`. An `auto=choice name(...)` list
arms while the SPELL RESOLVES, so its subject is in no game zone, the dangle rail refuses the
pointer, and the header falls back to `req.contextName` — which is precisely why `#W48 D6` added
an arm-time NAME. The annotations needed `ctx->magicText`, and no string carried it.

**(b) These rows never arrive as `CHOOSE_MODE` at all — measured, not inferred.**
`DecisionManager::buildMenuChoice` enters its multiple-choice branch only
`if (abilitiesMenu->isMultipleChoice && currentActionCard)`. A temporary `DebugTrace` at both
`ActionLayer` arm sites (added, run, removed — it is not in the diff) over a scripted
Silverquill Command cast reports, six times:

```
[W65AL-PROBE] setMenuObject name='Silverquill Command' textlen=1717 cac=1 multi=0
```

`isMultipleChoice = 0`, so the list falls through to the REGULAR menu path and arrives as
**CHOOSE_MENU** under `Choose an option for <card>:` — the header the wave-64 records print byte
for byte, while `Choose one mode for <card>:` appears in none of them. Wave 64's F6 tag was
wired to the CHOOSE_MODE branch alone and could not have reached those windows with any pointer.
The same probe shows `menuObjectText` populated at that site (**1717 bytes** of the card's
script), so the snapshot below is a measurement, not a hypothesis.

Both halves are fixed, in the general form:

* `ActionLayer::menuObjectText` — the subject's SCRIPT, captured at the same two arm sites and
  cleared at the same three clear sites as `menuObjectName` (plus `TestSuiteAI`'s reset). A
  string captured while the object is provably alive cannot dangle.
* `DecisionRequest::contextText` — carried onto the contract beside `contextName`.
* `modeRowAnnotations(script, label, myLife, oppLife, canBlockTapped, doNotUntap)` — ONE pure
  function holding the tap/untap tag and the F6 price tag, called by **both** menu seams over
  `ctx ? ctx->magicText : req.contextText`. The two kinds can no longer drift, and a null
  pointer no longer silences either tag.
* At the CHOOSE_MENU seam the pass runs over the ENGINE's own labels (`req.optionTexts`), so a
  row whose label is not a `name(...)` in that script matches nothing and is left exactly as the
  engine wrote it — this is silent on every menu that is not modal. A row already priced by
  `lifePaymentVerdict` (the shockland pay-or-tap class, whose `life:-N controller` clause both
  emitters would read) is skipped: one price per row.
* `{this mode right now: ` joins `stripNarrationDecoration`'s drop list. Wave 64's tag never
  reached a rendered row, so it never reached that list either; now that it rides CHOOSE_MENU
  rows it would otherwise land in `chosen_text_core` and the GAME LOG.

Presentation only throughout: `req.optionTexts` (the staleness key) and the row ORDER are
untouched, so `act.choice` still means what `applyMenuChoice` thinks it means.

### G4 — no SURVIVE verdict on a FLOOR, and the cover gate stops keying on one (engine HIGH-4, deck123 HIGH-3, deck162 HIGH-2)

`123v152` seq 9: the line said `for up to 13 … you would be at -4 or lower … and that number is
a FLOOR, not a ceiling: triggers on their board add power before damage - Luminarch Aspirant`,
and three lines below the cast row said `you cover 6 of 13, leaving 7 -> you would be at 2, which
you SURVIVE - and more blockers can only lower that, so nothing uncounted here overturns it.`
The monotonicity argument is sound in the BLOCKER dimension and says nothing about a total that
is a lower bound. Next window read **17**, not 13; the seat died at -8.

`crackBackScreenTotal` already reports `isFloor` — the same board fact the LINE is rendered from
— and it was computed at the call site and thrown away. It is now passed into
`crackBackBlockerRowTag`, with two consequences:

1. **No survival verdict on a floor.** Whichever side of 0 the figure falls, the row prints
   `THIS IS NOT A SURVIVAL VERDICT: the total it is subtracted from is a FLOOR, so a larger
   crack-back than N is on the table and this row does not say whether you survive`, and says
   what the cover DOES establish (the bodies remove K of it, and up to K' if every unchecked body
   could also block). Nothing is deleted — the whole arithmetic still prints under the withdrawn
   verdict, and the head clause names the floor in the same words the line above uses.
2. **The gate moves off the floor's lethality.** `myLife - total > 0` returned "" — no clause at
   all — and on a floor that asks whether the CEILING is lethal using a lower bound.
   `162 vs146` seq 12 read `up to 7 - you would be at 2 or lower` at 9 life beside two named
   3-power Hives, and two castable 5/5 bodies got no cover clause because `9 - 7 > 0`. The gate
   is now `myLife - total > 0 && !totalIsFloor`: on a CEILING it is unchanged; on a FLOOR the
   screen cannot say the crack-back is survivable, so the row prices the cover it adds and says
   plainly that it is pricing a floor.

### G5 — the compulsory-draw sentence is charged to the right chair (engine HIGH-5)

`Your DRAW STEP is COMPULSORY … the cost above is charged whether you act or not` rendered on
191 windows; **159 under `punishers: theirs` (correct), 32 under `punishers: yours`** (`162v125`
seq 14, the seat's own Underworld Dreams), where the only cost above is one the OPPONENT pays,
and the sentence then told the seat to price its own optional draws against it.

The compulsory-draw FACT is true of the seat either way, so it is never withdrawn — F10's whole
point. What is scoped is the half that names a cost: with a punisher of THEIRS on the board the
wave-64 sentence stands byte-identically (including the both-sides board, where the seat really
is billed); with only the seat's OWN punishers it reads `No punisher above bills YOUR draws,
though: every number above is life the OPPONENT pays, and their draw step is compulsory for them
in the same way, so that life is collected whether they act or not. Your own draws cost you
nothing on this board.` — the true token the wrong chair was standing in for.

### G9 — the pass row carries the cleanup price it does not decline (deck125 HIGH-1)

`125v162` seq 72/73 (turn 17, own turn, 12 life, hand 10, 7 open mana, Final Judgment and Path
both castable) answered `Cast nothing right now` in main 1 and again in main 2; the cleanup at
seq 74 billed 3 discards × Liliana's Caress = **6 of its 12 life**, and the seat was dead at seq
97. **18 of 18** windows with a DISCARD PUNISHERS paragraph, a hand over the maximum and a pass
row priced the pass row at nothing; **18 of 18** were passed.

Nothing new is computed. The engine already renders this arithmetic twice on the same board —
on the cast row (`CLEANUP PRICE at X=4`, `cleanupDiscardPriceClause`) and one window later at the
cleanup ask (`cleanupDiscardHeaderText`). `passRowCleanupPriceTag` calls the SAME clause with the
label `CLEANUP PRICE OF PASSING` and puts it on the cast menu's decline row, where declining is
the act that incurs it, closing with the F10 shape: `Taking this row does not decline that: the
cleanup step is CR 514.1 and no row on any menu declines it. Only cards LEAVING your hand before
then … make it smaller.` Inputs come from `forcedCleanupInputs(NULL, …)` — the existing walk, so
hand, maximum, stack-pending draws and the punisher rate are the same numbers the cast rows use.
Stated only on the seat's OWN turn: on the opponent's turn the seat's cleanup is a turn away.

**Key safety** (wave61/corpus-livelock.md): the clause is a `{…}` group, so
`stripRenderAnnotationsLc` keeps it out of the option-set key the declined-list count and the
hold latch are built from — pinned by a case asserting the priced row keys identically to both
faces of `castDeclineRow`. `{CLEANUP PRICE OF PASSING: ` is on the narration drop list. Every
input is a board fact of THIS window, so nothing can differ between two rebuilds of it.

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), qmake untouched, no new sources.
Detached unit `w65-AL-gate2`, `MemoryMax=4G` / `MemorySwapMax=0`, foreground until-loop wait.

| leg | result |
|---|---|
| build | 0 errors |
| PARSETEST | **4573 passed, 0 failed** (base 4536/0 — **+37 cases**) |
| suite, `WAGIC_TESTSUITE_THREADS=1` (ground truth) | **1269 tests (0 failed), 67 AI tests (0 failed)** |
| `==Test Failed !==` / `==Test timed out` (T1) | **0** / **0** |
| suite, default threads | **1269 tests (2 failed), 67 AI tests (0 failed)**, 0 timeouts |
| the default-threads failures | `lifeline.txt`, `merrow_reejerey.txt` — the brief's known concurrency-only pair |

**0 new failures.** `intrepid_adversary_repeated_payment.txt` (the brief's genuine intermittent)
passed in both runs; its rate was not exercised here.

### RED-on-base evidence — measured, not asserted

The four mechanisms were reverted to their wave-64 behaviour with the new cases in place and the
binary rebuilt: `modeRowAnnotations` returning "" at entry (wave 64 had no shared annotator and
the CHOOSE_MENU seam annotated nothing), `crackBackBlockerRowTag`'s gate and verdict restored to
the `myLife - total > 0` / unconditional-SURVIVE forms, `drawPunisherSummaryText`'s compulsory
sentence unconditional, `passRowCleanupPriceTag` emptied.

Result: **`4555 passed, 18 failed`** (log `~/.gatelogs/w65-AL-red.log`) — **G1 ×6, G4 ×5, G5 ×3,
G9 ×4, and nothing else**: 4555 + 18 = 4573, so **no pre-existing case goes RED under the
reversion** and none was deleted or re-pinned. The other 19 new cases are MUST-NOT-MATCH,
NEGATIVE, CONTROL and echo-shape pins that pass on base too, which is what they are for.

---

## Predictions (falsifiable, for the wave-65 corpus)

1. **G1** — every rendered row of a modal `auto=choice name(...)` menu (the
   `Choose an option for <card>:` family) whose matched script segment carries an explicit
   `life:<N> controller|opponent` or `draw:<N> controller|opponent` OUTSIDE any `ability$!…!$`
   block carries `{this mode right now:`; **0** such rows render bare. On Silverquill Command
   specifically, 6 of the 9 rows carry the tag and rows 1/4/7 (grant-only) carry none.
   `{this mode right now:` occurrence count corpus-wide is **> 0** (wave 64: 0 over 23 rows).
   **Falsifier**: one bare priced mode row, or one seat-facing kill tag on an
   `opponent`-directed cost.
2. **G4** — **0** `{crack-back cover:` clauses containing `which you SURVIVE` appear on a window
   whose `CRACK-BACK NEXT TURN` line contains `is a FLOOR, not a ceiling`; every clause on such a
   window carries `THIS IS NOT A SURVIVAL VERDICT`. And a `{crack-back cover:` tag now appears on
   windows whose printed crack-back total is survivable **only** when that total is a floor.
   **Falsifier**: one survival claim on a floor (the `123v152` seq 9 shape), or a lethal-floor
   window with a body-adding cast row and no cover clause (the `162 vs146` seq 12 shape).
3. **G5** — `Your DRAW STEP is COMPULSORY` still renders on every window carrying a
   `DRAW PUNISHERS on the battlefield` paragraph; **0** windows whose paragraph names only
   `yours` carry `the cost above is charged whether you act or not` or
   `Count that cost before choosing an OPTIONAL draw` (wave 64: 32). Every such window carries
   `Your own draws cost you nothing on this board.` **Falsifier**: one of either.
4. **G9** — every window on the seat's own turn that prints a `DISCARD PUNISHERS … theirs`
   paragraph, a hand above the maximum hand size and a `Cast nothing right now` row carries
   `{CLEANUP PRICE OF PASSING:` on that row (wave 64: 0 of 18), and its discard count and life
   figure equal the ones the cleanup ask prints for the same hand. **0** such tags appear on the
   opponent's turn or on a hand at or under the maximum. **Falsifier**: one of any.
   Secondary (behaviour, not a render fact): deck125's pass rate over that window class falls
   below wave 64's 18 of 18.

---

## What I did NOT verify

- **No live model probe was run and no game was driven for the render items.** PARSETEST, the
  suite and source reading are the whole gate for G4, G5 and G9; nothing in them has been seen by
  a model, so every behavioural prediction is a prediction, not a measurement.
- **G1's seam claim is measured only on the CAPTURE side.** The probe proves the arm site
  (`setMenuObject`, `isMultipleChoice = 0` → the CHOOSE_MENU path) and that `menuObjectText` is
  populated there. It does NOT prove `req.contextCard` is NULL at ask time, because
  `DecisionManager::buildMenuChoice` has exactly one caller (`AIPlayerGPT::chooseMenuAction`) and
  the suite has no endpoint — the same unfixturable boundary lane AK hit on R1. The shipped code
  is correct either way (`ctx ? ctx->magicText : req.contextText`), but which of the two rungs
  actually feeds the corpus is unmeasured, and the wave-65 corpus settles it.
- **The composed CHOOSE_MENU ask is pinned in PARSETEST as a composed ROW SET, not through
  `askModel`.** `askModel` takes a live board, so no pure case proves the call site; the wiring
  is argued from the single edit at the one place the pass is emitted, plus the probe above.
- **`modeEffectPriceTag` is still proven against ONE real primitive** (Silverquill Command, all
  nine lines) plus synthetic shapes, and still reads only `life:` and `draw:` with an explicit
  player. Cards writing the same effects in other forms render as wave 64 did — silently, never
  wrongly. Widening the parse was out of scope for this lane.
- **The one-price-per-row skip is proven by reading.** `rowAlreadyLifePriced` needs the seam's
  own `opts`/`req` pair, so there is no pure case for it; I read
  `payLifeAmountFromOption`/`lifePaymentVerdict` and the shockland arm path, and did NOT
  enumerate every menu on which both emitters could match the same row.
- **G4's floor branch asserts nothing in EITHER direction.** Where the total is a floor a seat
  that would in fact survive is told only that nothing is established. That is the honest read of
  a lower bound, and it is stricter than the ledger's "gates on the ceiling": I did not compute a
  crack-back CEILING number, because `crackBackFloorSources` returns prose (animator rungs,
  pinger rates, pump names) and no arithmetic that could be summed without re-deriving each
  source's magnitude. The clause says what it cannot say instead of guessing — the brief's other
  allowed answer — and the growth sources are still named on the line above.
- **G4's new gate widens where the clause appears.** Any body-adding cast row on a floor window
  now carries it, including floors far from lethal. I did not measure how many windows that is;
  wave 64 saw 300 `CRACK-BACK NEXT TURN` windows of which 32 were lethal, and the floor share is
  unknown to me. If the corpus shows the clause on a large share of cast rows, the honest
  follow-up is a magnitude gate, not a return to keying on the floor.
- **G5's split is on the paragraph's two lists, not on a per-permanent actor test.** A permanent
  that punishes BOTH players' draws would appear in both lists and take the theirs-side sentence,
  which is correct for the seat; I did not look for such a card in the pool.
- **G9 prices the DECLINE row only, not the HOLD row.** Both are passes and the price is a fact
  about the window, so the clause is worded to say that taking it does not decline the cleanup
  and that only cards leaving the hand reduce it — but a seat that passes via `Hold priority`
  sees the number on the row above rather than on the row it takes. Putting it on
  `holdRowLine()` would reach four other seams that do not compute these inputs, which is a
  wider change than this lane was given.
- **G9's forecast is a CEILING, as the shared clause already says** ("unless you spend cards
  before then"). It does not model the cards the seat may still cast this turn, and it counts
  stack-pending draws through `forcedCleanupInputs`, which is the same walk the cast rows use —
  so any error in that walk is inherited, not added.
- **No cross-lane interaction was tested.** Lanes AM, AN and AO touch adjacent emitters (AN owns
  the loop clause that shares the crack-back screen; AO owns the reply rule and the plain
  `Cast nothing` row this lane annotates). The combined diff has not been built here. Two
  merge-time looks are worth it: `stripNarrationDecoration`'s drop list (two entries added at the
  same chain AH widened) and the cast-menu decline-row push, which is now a block rather than a
  single `push_back`.
