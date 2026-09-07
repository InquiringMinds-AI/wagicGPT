# Wave-72 lane BX — the Astra step-one review's nine findings

Base `32913244b` (master after the review commit; merged step one 32ab3855c,
PARSETEST 5457/0), branch `w72-lane-BX`. Files touched: `src/AIPlayerGPT.cpp`,
`include/AIPlayerGPT.h`, `src/AllAbilities.cpp`, `include/AllAbilities.h`.

Gates on this tree: suite **1281 tests, 0 failed / 76 AI tests, 0 failed**
(THREADS=1, detached unit `w72-BX-suite`; both `==Test Failed !==` and
`==Test timed out` counted, both 0 — the three permitted flakies passed too);
PARSETEST **5487 passed, 0 failed** (base 5457 → **+30**); both link-time guards
OK (ctor-init 118 files, reply-instructions 1 source / 37 guides);
`git diff | grep -c U+FFFD` = 0. No corpus launched, no VPK.

## RED-on-base evidence (how it was taken)

Six of the nine fixes change a function SIGNATURE (a new parameter or a new
argument), so the new pins cannot be compiled against the base tree. The RED was
therefore taken as a **behavioural revert**: with every pin in place, each fix's
BODY was mutated back to the base rule (F1 subset test → always true; F2
ownership → always own; F3 open-flag → ignored; F4 token filter → `!isToken`;
F5 → the wave-72 wording verbatim; F6 → permute the fallback too; F7 → every
land is a source; F8 → the categorical branch only; F9 → header regardless), the
tree rebuilt, and PARSETEST run:

```
=== self-test: 5475 passed, 12 failed ===
```

the 12 being exactly this lane's REPRO/MUST-NOT-MATCH cases (F1, F2-ownership,
F3, F4, F5 ×3 incl. the two reworded M7 pins, F6, F7, F8, F9). The tree was then
restored byte-for-byte and rebuilt to the green numbers above. This proves the
pins fail against BASE BEHAVIOUR; it is not a build of the base tree.

## Per finding

### F1 HIGH — a casting HOLD suppressed unseen legal activations — CONFIRMED, FIXED
`gptHoldCoversSiblingWindow` checked (seam, turn, phase, board) and never asked
whether the held menu had CONTAINED the sibling's rows, so a hold taken over the
casting rows closed a priority window carrying cycling/activated rows the cast
menu never printed — removal of legal options by a latch.
**Fix.** The predicate now takes BOTH row sets: the held seam's recorded keys
(`mHoldRows[mHoldWindowSeam]`) and the keys of the rows this window is about to
print. The collapse stands only when every ACTING row of the new menu is in the
held set; `w72RowIsDeclineOrHold` drops the two seams' differently-worded
declines and the three hold spellings from the comparison (they are not plays).
`holdHonoured` builds both sets before the call, so `sibling_window_asks_skipped`
and `hold_windows_skipped` now count only collapses under the new rule.
**RED.** `#W72-BX F1 REPRO` (a priority menu carrying `Cycle Starstorm {2}` over
a held cast menu) fails with the subset test reverted; the pin at ~77340 was
rewritten to carry both option sets, positives and negatives alike.
**Prediction.** Next corpus: no `sibling asks skipped` stderr line on a window
whose printed rows include a row absent from the held seam's menu; the counter
drops well below wave 71's 44 identical pairs.

### F2 HIGH — an old stop acquired the date of an unrelated new plan — CONFIRMED, FIXED
`mPlanSetTurn` is stamped by EVERY plan line; `mStatedStop` is only replaced when
a reply states one. A turn-N stop therefore acquired turn N+1's date and could
auto-pass a repeat window. `repeatPlanStopAndCurrent` also had no ownership
discriminator (`PLAN: Their stop=29; their M=29` populated the seat's own stop).
**Fix.** New member `mStatedStopTurn`, set ONLY where a stop is parsed out of
this reply's PLAN line (both store sites: `consumePlan` and the reply-fold), and
only when `repeatPlanStopIsOwn` says the seat is talking about its own stop; the
collapse reads `mStatedStopTurn`, not `mPlanSetTurn`. An opponent-attributed stop
populates neither the stop nor its date. `repeatPlanScanNumberAt` reports the
label position so ownership reads the word in front of it; the old
`repeatPlanScanNumber` is an unchanged wrapper, so no existing reader moved.
**RED.** `#W72-BX F2 REPRO` (ownership) fails with the discriminator reverted;
`w72StopReachedWindowCollapses(true,true,29,-1,12)` pins the undated stop.
**Weakness (declared).** The DATING half is a wiring change (which member is
passed) and is not expressible as a pure pin; it is verified by reading, not by
a red test.
**Prediction.** Next corpus: every `stop_reached_windows_skipped` window has a
reply on the SAME turn whose PLAN states `stop=` in the seat's own voice.

### F3 MED — an open casting decision claimed casting was already answered — CONFIRMED, FIXED
Two halves. (a) The turn/phase stamp was read as "no cast is open", so a SECOND
casting menu in the same phase (one spell cast and resolved, mana and another
legal spell left) told its own window the decision was answered. (b) The stamp
was set immediately after `askModel` returned, so a pick the engine then rejected
re-rendered the same window with `attempt > 0` carrying the stamp.
**Fix.** `mCastDecisionOpen` (RAII `CastOpenGuard` over the whole cast loop) is
non-zero while the casting seam is assembling or re-asking; both consumers
(`kCastAnsweredFact` and the hand tag) go through the pure
`w72CastAnsweredFactApplies(stampMatches, open)`. The stamp itself moved to
`markCastDecisionAnswered()`, called at the four exits that actually CLOSE the
window (decline, hold, a validated cast, and both heuristic paths) and at none of
the re-ask iterations.
**RED.** `#W72-BX F3 REPRO` fails with the open-flag ignored.
**Prediction.** Next corpus: no prompt whose own question is `Casting decision`
contains `already answered this phase's Casting decision`, and no `[RE-ASK n]`
cast prompt carries the answered hand tag.

### F4 HIGH — token copies lost legitimate card-name options — CONFIRMED, FIXED
Wave 72's `isToken` filter dropped every token unconditionally. CR 201.4: a token
that is a copy of a card HAS that card's name and naming it is legal, so a token
copy of a real card removed a legal name whenever no nontoken copy sat in a
scanned zone (human and heuristic menus too, not just GPT).
**Fix.** The test is now on the NAME, not the object: a token is filtered only
when `MTGCollection()->getCardByName(name)` finds no card of that name. Rule as
the pure `w72TokenNameChoosable(isToken, nameIsACardName)` in `AllAbilities.h`.
A bare "Goblin" token is still removed; a token copy of a real card stays.
**RED.** `#W72-BX F4 REPRO` fails with the predicate reverted to `!isToken`.
**Weakness (declared).** PARSETEST runs from `main()` before the card database
loads, so the pin is on the RULE, not on a live database lookup; the lookup call
itself is verified only by the clean build and a green suite (which exercises
`GenericChooseTypeColorName` through the existing name-choice fixtures).

### F5 MED — public copies declared permanently uncastable — CONFIRMED, FIXED
The naming header equated "currently public" with "can never be cast again"
("evidence the name is SPENT") and the row tag claimed "only a copy that is not
already public can still be cast". A graveyard is a public zone and this engine
casts out of it (Hammer of Bogardan returns itself; flashback, cast-from-exile,
bounce).
**Fix.** The counts stay — they were the deciding number M7 added — but the
exclusivity claim goes. The header now says the tag says WHERE the public copies
are and that a copy in a graveyard or exile can still be cast or returned if a
card says so; the tag reads `- copies not visible in any public zone`. The M7
pins at ~77414 were rewritten (the false pin flipped).
**RED.** `#W72-BX F5 REPRO`/`MUST-NOT-MATCH` plus the two reworded M7 pins fail
against the wave-72 wording.
**Prediction.** Next corpus: no naming prompt contains `SPENT` or `can never be
cast again`.

### F6 MED — the name-menu fallback was permuted twice — CONFIRMED, FIXED
`AIPlayerBaka::selectMenuOption()` and the out-of-range clamp answer in the
ENGINE's index space; `nameOrder` was then applied to them, committing a
different name than the fallback chose (engine [A,B] displayed [B,A], endpoint
down → heuristic picks A, engine is handed B).
**Fix.** `pickIsDisplayIndex` tracks whose space the answer is in; the pure
`w72NameMenuEngineIndex(pick, fromModel, order)` permutes only the model's.
**RED.** `#W72-BX F6 REPRO` (reversed order + failed endpoint) fails with the
flag ignored.

### F7 MED — a non-mana land erased a real reserve warning — CONFIRMED, FIXED
The M9 land-drop fold counted any legal land not detected as entering tapped, so
an unused Dark Depths (or a fetchland, or Maze of Ith) contributed a "source"
that produces nothing and suppressed the STRANDS clause.
**Fix.** The land must actually make mana: `w72LandDropIsManaSource(basicMana,
script)` — the basic-land route (Basic Land, or a Plains/Island/Swamp/Mountain/
Forest subtype, which is how the duals tap) or a script carrying the engine's
three mana spellings (`Add{`, `add(`, `out{`). Scanned over the whole primitive
pool: 31 nonbasic lands answer false, and they are the fetchlands, Maze of Ith,
Dark Depths, Glacial Chasm, Urborg and friends — every one genuinely mana-less.
**RED.** `#W72-BX F7 REPRO` (Dark Depths' own two `auto=` lines) fails with the
predicate reverted; Ancient Tomb and Reflecting Pool pin the positives.

### F8 MED — "every point is unblocked" over an available creature-land — CONFIRMED, FIXED
The no-bodies sentence converts THIS turn's creature count into a categorical
claim about NEXT turn's combat; an untapped Mutavault or an instant-speed token
maker falsifies it before blockers are declared.
**Fix.** `crackBackNextTurnLine` takes `blockerMakers`; with one, the sentence
becomes "you control NO creature right now, so nothing on your battlefield blocks
any of it as the board stands - unless you first make a blocker: <names>". With
none it is byte-identical to wave 72. `w72BlockerMakers` names at most three:
untapped noncreature permanents with an activated `becomes(creature ...)` rung
(the same predicate the animatable census uses) and instant/flash creature-token
makers in hand. No affordability claim is made — the rung arithmetic belongs to
the FLOOR clause.
**RED.** `#W72-BX F8 REPRO` fails with the conditional branch removed; the pin
at ~77596 keeps the categorical face for a seat that really has nothing.

### F9 MED — LOOP COMPLETE ignored a live lifegain prohibition — CONFIRMED, FIXED
Two script-shaped halves on a battlefield were enough to announce inevitable
lethal chaining. With Erebos (`nolifegainopponent`) opposite Sanguine Bond +
Exquisite Blood the chain cannot restart, and the header still called every life
payment fatal.
**Fix.** `closedLoopHeaderText` takes `loopCanOperate` and is SILENT when false;
the call site passes `lifeLoopProvenWin(pl)` — the same predicate (CANTCHANGELIFE
/ CANTLIFELOSE / CANTLOSE / NOLIFEGAIN / NOLIFEGAINOPPONENT) the loop-half
warning and the attacker-row tag already consult.
**RED.** `#W72-BX F9 REPRO` fails with the gate ignored; `lifeLoopWinnable`'s
truth table is pinned beside it.

## Doctrine check

No legal option is removed, capped or auto-answered by anything in this lane. F1
and F4 RESTORE options the merged step one had removed; F2 and F3 narrow two
window-collapses to the states that actually justify them; F5, F7, F8 and F9
withdraw claims the board does not support without deleting the underlying
counts (the trust doctrine's "render the true token, never delete it"); F6
commits the fallback's own answer. No protocol text, thinking regime or harness
flag was touched (invariant 000 (c)).

## Weakest evidence

1. **F2's dating half and F3's stamp move are wiring**, verified by reading the
   call sites, not by a red test — no pure pin can say which member a call site
   passes. A live GPT corpus is the only instrument that would.
2. **The RED is a behavioural revert, not the base tree** (six signature
   changes); it proves the pins reject base BEHAVIOUR, which is one step weaker
   than a base build.
3. **F4's database lookup is unpinned** (PARSETEST runs before the card database
   loads); only the rule is pinned.
4. **F8's blocker-maker scan is untested against a live board** — no fixture
   drives a creature-land seat through the crack-back render; the pure line is
   pinned, the scan that feeds it is not.
5. **F1's decline/hold filter is a name-prefix list.** A future decline row worded
   outside it would be read as an acting row — which only makes the collapse
   FAIL SAFE (the window is asked), never the reverse.
