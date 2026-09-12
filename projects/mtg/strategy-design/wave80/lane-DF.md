# Wave-80 lane DF — combat, cover and pricing truth (U3, U4, U6, U7, U14, U13)

Base: `70345356d` (master after the wave-79 known-bugs commit).
PARSETEST baseline **6902 passed / 0 failed → 6957 passed / 0 failed (+55)**.
Suite baseline "1295 tests (0 failed), 76 AI tests (0 failed)" →
**1296 tests (0 failed), 76 AI tests (0 failed)** (+1 fixture); both failure strings 0.
`check-reply-instructions.py`: OK (1 source file, 37 guides). U+FFFD in the diff: 0.

Every string quoted below was read off the wave-79 corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-000748-final`, from the `prompt`
field of the cited record — i.e. the exact bytes the model saw. Every population
figure is from a scan of all 42 seat logs, and the scan is named where it matters.

---

## U3 HIGH — the cover credited a body the row does not leave, named no mechanism, and priced the spent blocker only on kill rows

### (a) the ETB bounce

**RED:** `146v162` seq 32, row 2: `Cast Acererak the Archlich {2}{b} (5/5) … {crack-back
cover: … This adds 1 body - a creature that arrives this turn CAN block …}` — on a prompt
whose own narration nine lines above reads `Your Acererak the Archlich was returned to your
hand from the battlefield`. The card's own untriggered ETB line (`borderline.txt:442`,
`if type(Tomb of Annihilation[dungeoncompleted]|myzones)~equalto~0 then name(Return to hand)
moveTo(hand)`) is why. **7 corpus windows** carry a `{crack-back cover:` clause on an
Acererak row. Pinned RED: `castBodiesNetOfOwnText(1, true, false) == 1`.

**Fix:** `w80EtbSelfLeavesLine` recognises, from the effect SHAPE (untriggered or
`@movedto(this|battlefield):`, a `moveTo` to a non-battlefield zone or a `sacrifice`, and no
`target(`/`notatarget(`/`all(`/`foreach(` naming another object), a line that moves the cast
card straight back off the battlefield, and `castBodiesNetOfOwnText` nets that body out
alongside the legend rule. Where the netting leaves nothing, the row prints
`w80SelfLeavesNoCoverClause` instead of falling silent — the trust doctrine's
no-silent-omission rule.

**GREEN:** the predicate is true on Acererak's real line and false on a target-bounce, an
edict, a sweeper, a token ETB, a `moveTo(mybattlefield)` and a `@combat` trigger;
`castBodiesNetOfOwnText(1,true,false,true) == 0`, `(4,true,false,true) == 3`,
`(1,true,true,true) == 0`; the replacement clause's bytes are pinned in full.

**Prediction (wave-80 engine seat):** 0 `{crack-back cover:` clauses on a cast row whose card
has a self-leaving ETB line; `crackback_cover_self_leaves_dropped` (countable as renders of
`crack-back cover: NONE from this body`) **> 0** — 7 windows of the shape existed in the
wave-79 corpus, so a 0 is a FAIL, not "untested".

### (b) the mechanism

**RED:** `146v162` seq 35 (t15, 1 life): `{crack-back cover, STAY HOME: keeping all 3 of them
back covers 6 of that 6, leaving 0 -> you would be at 1, which you SURVIVE …}`. The seat's
reasoning body (~2,000 chars) argues with it seven times — *"If I block 2 attackers with 2
blockers, I take 6 damage. I die"*, *"Maybe the 'covers 6' means my blockers have enough
toughness or something?"*, and finally *"I'll just follow the prompt's guidance."* The clause
states a subtraction and never states the rule behind it, and the rule is not size, it is
redirection (CR 510.1a/c). Pinned RED: the shipped seq-35 bytes are reproduced by
`w77StayHomeCoverTag(6, 1, false, 3, 6)` up to its old closing brace.

**Fix:** one sentence, `w80CoverMechanismSentence`, on both emitters of the cover family
(`crackBackBlockerRowTag`, `w77StayHomeCoverTag`): a BLOCKED attacker deals its combat damage
to the blocker, not to you, all of it, whatever the sizes, which is why each blocker subtracts
its attacker's FULL power and why the blocker's toughness is not in this arithmetic.

**GREEN:** both clauses carry it, in the same place, and every wave-79 byte before it stands
(the seq-32 cover clause still reads `… you cover 3 of 6, leaving 3 -> you would be at -2,
which still KILLS you`).

**Prediction:** **129 clauses** of the wave-79 corpus (126 `{crack-back cover:` rows + 3 STAY
HOME rows) carry the sentence in a comparable wave-80 corpus;
`cover_mechanism_sentence_rendered` **> 0**, and 0 reasoning bodies that compute a cover from
blocker TOUGHNESS.

### (c) the spent blocker on every sacrifice-cost row

**RED:** `130v152` seqs 52 and 54 print nine `[cost: {1}{r}, Sacrifice]` rows under
`CRACK-BACK NEXT TURN: 1 of their creatures … for up to 4 … a FLOOR`, six of them aimed at the
seat's own bodies and two at a face, and not one prices a blocker — wave 79's give-back
sentence lives inside `crackBackReliefClause`, which is emitted only where the row's damage
kills a creature the OPPONENT controls. Corpus scan: **60 sacrifice-cost rows under a printed
CRACK-BACK line, 0 of them carrying the give-back sentence.**

**Fix:** `w80SacrificeSpendsBlockerClause` is emitted from the `[cost: …]` block of
`describeAction` on any row whose cost's `SacrificeCost` can take one of the seat's standing
blockers, using the same `w79SacrificeBlockerGiveBack` model and the same base the relief
clause uses (#W79-DC F4: the blocked best case, never the no-block projection, and no death
verdict). It is suppressed on a row that already says it, so no row states the fact twice.

**GREEN:** the positive clause's bytes are pinned; the FLOOR variant names the floor; the
MUST-NOT-MATCH pins hold that it states no death verdict and no no-block subtraction.

**Prediction — and its honest bound.** On the wave-79 corpus this fix would have rendered
**zero** times, and that is the correct answer, not a failure of the fix: of those 60 rows,
every one either sacrifices a non-creature (a Clue, a fetchland) or stands on a board where
the give-back is genuinely 0. `130v152` seq 52 is the exact case — one attacker of 4 power
against six standing bodies, so `w79CoveredByBodies(atk, 6) == w79CoveredByBodies(atk, 5) == 4`
— and that identity is pinned, together with the pin that a give-back of 0 prints nothing.
So the falsifiable prediction is a CONDITIONAL one: `sacrifice_row_blocker_priced > 0` **on any
corpus containing a window where a sacrifice cost can take a standing blocker whose loss
reduces the cover**, and — the half that is checkable on any corpus — **0 renders of the clause
on a row where the cost cannot take a creature**. This is the weakest item in the lane; it is
listed first in "weakest evidence" below.

---

## U4 HIGH — the tap bill split across two clauses

**RED:** `152v125` seq 38 row 8: `put 1/1 counters with Katilda, Dawnhart Prime [cost:
{4}{g}{w}, Tap] {paying this taps: Brutal Cathar, Briarbridge Tracker - they cannot attack this
turn} {paying this taps: Katilda, Dawnhart Prime - it cannot attack this turn}` — three
attackers spent by one row and no clause on the row naming more than two, while the deck-152
guide's LETHAL COUNT rule counts the names inside that clause. Both halves are reproduced byte
for byte from the two emitters that built them, and the composed row is pinned as carrying the
bill twice. Corpus: **3 rows** carry two `{paying this taps: ` clauses.

**Fix:** the row's own `{T}` cost is folded into the SAME name list before the single
`paymentTapsClause` is built (the predicate is hoisted above the mana-pick block and reused by
the old emitter, which now never fires when the merged list already names the source). And
because `Katilda, Dawnhart Prime` contains a comma, a list that cannot be counted by reading its
commas now states its own count.

**GREEN:** one clause, three names, `(that is 3 permanents of yours - one of the names in this
list contains a comma, so count them from this number, not from the commas)`; exactly one
`{paying this taps: ` on the row. A comma-free list and a one-name list are byte-identical to
wave 79 (pinned both ways).

**Prediction:** 0 rows in the wave-80 corpus carrying two `{paying this taps: ` clauses
(`tap_bill_split_rows` = 0), and `tap_bill_count_stated` **> 0** on the deck-152 seat, whose
Katilda rows are the population. 3 such rows existed in wave 79.

---

## U6 HIGH — the cycling row priced the draw half only

**RED (render):** `130v162` seq 35, the row the seat took: `cycling with Starstorm [cost: {3},
Cycle] … [DRAW PRICE: this draws 1 card, and the opponent's Underworld Dreams punishes every
draw, so taking it costs you 1 life right now - you would be at 18]`, on a screen whose own
board frame reads `DISCARD PUNISHERS on the battlefield: theirs - Liliana's Caress. Every card
YOU discard costs you 2 life`. Pinned byte for byte from `drawPriceRowTag(1, 1, "Underworld
Dreams", 19)`. Corpus scan: **33 cycling rows** rendered under a live discard punisher, all
priced at the draw alone.

**RED (engine — the live path, not the reviewer's arithmetic):** new suite fixture
`w80df_u6_cycling_is_discard_and_draw.txt`. `CycleCost::doPay` (ExtraCost.cpp) sends a
`WEventCardDiscard` *and* the card is drawn, so Liliana's Caress
(`@discarded(*|opponenthand):life:-2 opponent`) takes 2 and Underworld Dreams
(`@drawfoeof(player):damage:1 opponent`) takes 1. The fixture cycles Akroma's Vengeance with
both enchantments out and ends at **17**; asserting the render's own figure (19 → 18, i.e. 18
in the fixture's frame) FAILS with `==life problem for player 0. Expected 18, got 17==`. The
engine charges 3 where the row said 1.

**Fix:** `drawPriceRowTag` gains a discard half (count, per-discard rate, punisher names, and
whether the cost is a CYCLING cost), read at the emitter off the ability's own cost object
(`CycleCost` / `DiscardCost` / `DiscardRandomCost`) and the same `discardPunisherScan` the board
frame is built from. One bracket, one resulting life, the row's whole price.

**GREEN:** the seq-35 row now reads `… costs you 1 life right now; AND paying this row's cost
DISCARDS 1 card (a CYCLING cost is a discard as well as a draw …), and the opponent's Liliana's
Caress punishes every card you discard, for 2 more - 3 life from this row in total - you would
be at 16`. A discard with no draw punisher is priced on its own (wave 79 printed nothing at
all); the lethality tail reads off the folded total; no discard, no discard punisher, or an
unnamed one, and every wave-79 byte stands.

**Prediction:** `cycle_row_discard_priced` **> 0** (33 rows of the shape in wave 79), and 0
rows carrying a `Cycle]` cost under a named discard punisher whose `[DRAW PRICE:` bracket omits
the discard term. A 0 on the first is a FAIL.

---

## U7 HIGH — the `board sweep:` marker argued one side

**RED:** `123v162` seq 42 row 4 (Damnation): `[<- board sweep: THEIRS 2 (including Fate
Unraveler - a DRAW PUNISHER (it bills every card the other player draws)) / YOURS 1 - the only
row on this menu that prices a board sweep. THAT COUNT IS BODIES, NOT VALUE: what it takes of
THEIRS includes the engine(s) named above …]`. The row's own clause three brackets earlier
already names the seat's loss — `YOURS: Thraben Doomsayer (2/2)`, a token engine — and the
marker, the one surface the corpus shows the pilot obeys, reduces it to the integer 1. The seat
cast Damnation into its own engine and died two turns short. `123v162` seq 74 is the same shape
with Bloodline Keeper and no editorial at all. Both reproduced byte for byte. Corpus: **43
board-sweep markers, 6 of them with YOURS > 0.**

**Fix:** `boardCreatureCounts` collects `myEngines` from the same walk and the same
`engineKindForScript` predicate it already uses for THEIRS; `CastRowBoardAnswer` carries the
own-side roster and engine list; `boardSweepMarker` names them, and the BODIES-NOT-VALUE
sentence now fires when EITHER side loses an engine.

**GREEN:** seq 42 becomes `… / YOURS 1 (Thraben Doomsayer; of those, Thraben Doomsayer - a
TOKEN ENGINE … - losing that is not losing a body, it is losing what the body keeps making) …`
and its caution reads `what it takes of THEIRS includes the engine(s) named above, and what it
takes of YOURS includes the engine(s) named above …`; seq 74's bytes are pinned in full. With
no roster on either side every byte is wave 66's.

**Prediction:** `board_sweep_own_roster_rendered` **> 0** (6 markers of the shape in wave 79),
and 0 markers printing a non-zero `YOURS` count with no names after it.

---

## U14 MED — the blockers header's lethal verdict ignored compulsory non-combat damage

**RED:** `146v162` seq 29, at 4 life: `Your life: 4. Unblocked, these attackers deal up to 3 -
you would be at 1 - NOT lethal: block only where the trade favors you.` — eleven lines under
the same screen's `DRAW FORECAST: your next draw step draws 1 card = 1 x 3 = 3 life LOST BY YOU
to their punishers above … This draw step is COMPULSORY - no row on any menu declines it.`
Pinned byte for byte from `combatDamageForecast(4, 0, 3, 0, 13, false)`. Corpus scan over all
42 logs: **1 window** of this exact shape (a `NOT lethal` header whose own screen's
compulsory draw charge takes the post-combat figure to 0 or below).

**Fix:** the blockers seam computes the draw-step charge from the DRAW FORECAST's own two scans
(`drawStepExtrasScan` + `drawPunisherScan`, so the two lines cannot disagree — this seam is the
opponent's combat, so the seat's whole next draw step is ahead of it) and hands it to
`combatDamageForecast`, which withdraws the survival claim, says it is withdrawn, prints the
arithmetic, and drops the take-the-damage-while-ahead hint.

**GREEN:** `… - NOT lethal FROM THE COMBAT ALONE, and that is not a survival verdict: your next
draw step (1 card into their Underworld Dreams, Fate Unraveler, Ob Nixilis, the Hate-Twisted -
see the DRAW FORECAST line above) costs you 3 more life before you act again, and it is
COMPULSORY - no row on any menu declines it. 1 - 3 = -2, so taking this swing in full LOSES THE
GAME this turn cycle.` A charge that does not reach 0, an already-lethal swing, and a board with
no draw punisher are all byte-identical to wave 79 (pinned).

**Prediction:** `blockers_cycle_lethal_verdict` **≥ 1** in a comparable corpus, and 0 blockers
headers printing `NOT lethal` on a screen whose DRAW FORECAST figure takes the post-combat life
to 0 or below. The wave-79 population is 1 window, so this prediction is low-powered by
construction — the checkable half is the second clause, which is a 0 over every window.

---

## U13 MED — a 21-row menu with one outcome

**RED:** `152v130` seqs 6 and 9, `152v123` seq 8, `152v162` seq 10: `options: 21` on the
Intrepid Adversary counter ask with 0 spendable mana. The engine's own header already states
the identity in words — `With no spendable mana left, every option adds 0 counters` — and row
21's clause states it in numbers — `{repeat cost: 20 x {1}{w} = 40 mana for all 20; you have 0
spendable now, which pays for 0 of them and stops} {rows for 1 through 20 counters are all
identical in effect right now: your mana pays for 0 payments and stops …}`. Four of that seat's
207 model calls (≈2%) went to it at p50 ~100 s each.

**Fix:** `w80SingleOutcomeRepeatMenu` proves the identity ON THE ROW SET — every row must be
either an `add N counters` row the mana pays 0 of, or the engine's own `don't add any counter`
row; one unrecognised row, or one rung the mana reaches, and the menu is asked exactly as
before. Where it holds, `chooseMenuAction` answers the engine's own no-op row and writes a
`menu_single_outcome` record naming the menu, the row count, the row taken and the payment
figure that proved them one option.

**No legal option is lost.** The engine's option vector and the answer index space are
untouched, nothing is capped, hidden or reordered, and the rows are not *choices* that were
removed — they are one choice that was printed twenty-one times, which the engine's own two
annotations already say. The gate is proved, not assumed: the pins hold that `152v146` seq 13
(3 mana spendable, `pays for 1 of them`) is still ASKED, that a single reachable rung re-opens
the ask, that an unreadable row declines the gate, and that with no engine row to answer on the
seat asks rather than picking a rung of its own.

**GREEN:** the 21-row corpus menu, rebuilt from the engine's own labels, returns row 1; all
five MUST-NOT-MATCH shapes return "ask".

**Prediction:** `single_outcome_menus_answered` **> 0** (4 windows in wave 79), one
`menu_single_outcome` record per increment (the census reconciles or the counter is wrong), and
0 `ask` records whose `options` is a 21-row Adversary menu with `pays for 0 of them` in its row
text.

---

## Key stability (LESSON OF WAVE 74/78)

`[#W80-DF] KEY` builds the JOINED menu (one acting cast row and the hold row beside it) twice,
differing ONLY in this wave's bytes — the cover clause vs the self-leaves clause, the merged tap
bill, the folded draw/discard price, the spent-blocker clause — and asserts, through the LIVE
builders: `optionSetKeyOf(menu)` identical; `holdActionKeyRow(row)` identical and the whole
latched row SET identical (built the way the live seam builds it, over `mLastMenuRows`'s own
reducer); `w77KeyTailOf(tail)` identical — the ask key's AND the async slot's half;
MUST-NOT-MATCH that none of `crack-back cover`, `paying this taps`, `DRAW PRICE` or `SPENDS A
BLOCKER` survives into the key, while the action and its cost pips do. The board-sweep marker
is pinned the same way on its own pair.

The blockers header (U14) is a board-frame line, not a menu row: it lives in the seam's `tail`,
not in `optionTexts`, and carries no action key. U13 changes no rendered byte at all.

---

## Gate numbers

* PARSETEST: **6957 passed, 0 failed** (baseline 6902 — +55 checks).
* Suite, `WAGIC_TESTSUITE_THREADS=1`, detached unit `w80-DF-suite`:
  **1296 tests (0 failed), 76 AI tests (0 failed)**; `==Test Failed !==` 0,
  `Test timed out (game never ended)` 0. No known flake hit.
* `check-reply-instructions.py`: OK (1 source file, 37 guides).
* `git diff | grep -c U+FFFD`: 0. Both link guards (header + ctor initializer list): OK
  (`check-ctor-init: OK (118 files)`).

---

## Weakest evidence (read this before trusting the above)

1. **U3(c) renders zero on this corpus, by construction.** All 60 sacrifice-cost rows under a
   crack-back line in wave 79 sacrifice a non-creature or stand on a board where the give-back
   is genuinely 0. The fix is right for the general case and its silence on those boards is
   pinned as correct — but the wave-78 rule ("a counter that reads 0 after the fix is a FAIL")
   cannot be applied to it, and I have not manufactured a corpus window where it fires. Treat
   it as UNTESTED against live play until a corpus produces the eligible board.
2. **No corpus, no live seat (lane rule).** Every claim about the RENDERED prompt is proven at
   the emitter with corpus-derived inputs. The path from the live board walk into those
   emitters is unexercised for U3(a), U3(c), U4, U6's render half, U7 and U14 — the S6/S3 shape
   the wave-78 lesson names. Only U6 has an ENGINE-level proof (the suite fixture), and only
   U13's gate is a pure function of data the live seam already computes.
3. **U3(a) is conservative and the conservatism is invisible.** A self-leaving ETB line that is
   CONDITIONAL (Acererak's is: `if <dungeon not completed> then`) still drops the body, so on a
   board where the condition is false the row under-counts its own cover. Under-counting is the
   safe direction for a survival verdict, but the clause says "moves it straight back off the
   battlefield", not "can" — the condition is visible only in the card text printed on the same
   row. A card whose ETB bounce is conditional on something the seat controls would read as
   flatly wrong.
4. **U3(a)'s predicate is line-local.** It reads one script line at a time and asks whether
   that line names another object. A self-bounce expressed across two lines, or through a
   granted ability, or through a macro that expands to a target form, is missed (fails closed);
   a self-sacrifice whose `sacrifice` token belongs to a clause the line also targets with
   would be caught wrongly — no such shape exists in the primitives I read, but I did not scan
   all of them.
5. **The reviewer's Silverquill Command half of U3 does not reproduce.** U3 names "Silverquill
   Command's dead 'return creature' mode (`146v162` seq 32)" as credited with "adds 1 body". It
   is not: row 1 of that record carries no `{crack-back cover:` clause at all, and a scan of the
   whole corpus finds 0 cover clauses on any Silverquill Command row. `castBodiesAdded` counts
   only `create(` tokens and skips every `if `/`may `/`pay(` line, so a modal spell's
   battlefield-return mode has never been counted as a body. Nothing was changed for it.
6. **U4's count clause is scoped to the ambiguity, not to the fact.** It fires only when a
   name in the list contains a comma. A guide rule that counts names is still counting names on
   every other row; the merge (one clause per row) is what actually fixes the LETHAL COUNT
   rule's input, and the count is a belt on top of it.
7. **U6 prices the cost's discard, not every discard the row causes.** The count comes from the
   cost object (one per `CycleCost`/`DiscardCost`/`DiscardRandomCost`), so a `DiscardCost` whose
   TargetChooser takes more than one card is under-counted, and a discard caused by the
   ability's EFFECT (rather than its cost) is not priced here at all. Under-counting is the
   direction that cannot claim a cheaper row than the engine charges — except in the
   multi-card-discard case, where it can.
8. **U7 names the roster the sweeper walk already built.** `myNames` carries each victim's
   `sweeperVictimName` plus its regeneration tail; on a wide own-board the marker gets long. I
   did not cap it (a cap would be the render deciding what the seat may see), and the measured
   population is 6 markers, but a 10-creature own board would add a long list to one row.
9. **U14's population is one window.** The positive prediction is low-powered by construction;
   the load-bearing half is the negative (0 `NOT lethal` headers contradicted by their own DRAW
   FORECAST). The charge itself counts only the DRAW-STEP punisher loss — the one non-combat
   damage the same screen calls undeclinable. Upkeep triggers, queued stack damage and the
   opponent's own second main are all outside it, so the verdict can still be optimistic.
10. **U13 answers a menu the model never sees.** The four wave-79 windows are all
    zero-spendable-mana Adversary asks, and the gate's shape test is written to that card's
    engine labels (`add N counter(s)` / `don't add any counter`). A different card that reaches
    the same one-outcome state under different labels is asked, not answered — correct, but it
    means the gate's reach is exactly one card family until another is proven.
11. **The `menu_single_outcome` record is new telemetry and has never been harvested.** Its
    join to the gameend counters is pinned by construction (one increment, one record) but not
    by a corpus. The census rule (the record-kind sum must reconcile) needs this kind added.
