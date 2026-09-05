# Wave-63 lane AC — E2, E5, E9, E13, E15, E16 (prices and rows that lie by omission)

Base: master `41fbad879` (wave-62 close). Branch `w63-lane-AC`, worktree
`worktrees/lanes/w63-AC`. **One file changed: `projects/mtg/src/AIPlayerGPT.cpp`.**
Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); no kill under the cap on any
run. The gate ran detached as `w63-AC-gate` with a foreground until-loop wait.
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0** after every edit (all splices python rb/wb).

---

## What shipped

### E2 — the CLEANUP PRICE undercounted the hand and priced the floor off the wrong life
`125v162` seq 89 priced a Sphinx's Revelation row *"up to 7 discards ... at worst you would be at
4"*; seq 94 billed 10 discards, 20 life, 25 -> 5. Two independent errors that happened to cancel on
that row and would not cancel again.

*(1) the count.* `forcedCleanupInputs` computed the hand as "cards in hand, minus this cast" and
nothing else, so the four draw triggers listed in that same prompt's own `ON THE STACK` block
(Howling Mine ×2, Dictate of Kruphix ×2) were not in it — 14 claimed against 17 actual. New
`scanStackAbilityDraws` / `stackPendingDrawsFor` walk the UNRESOLVED stack for `AADrawer` payloads
aimed at the seat, on exactly the rails the life walk already uses (the RNG is never drawn — a
`rand` amount is not counted; only `NOT_RESOLVED` objects; only cards drawn by this seat), and
`exclude` drops the object the caller is itself pricing so a spell whose own draws the row already
adds is never double-counted. `forcedCleanupInputs` folds them into `handAfterCast`, so **every**
consumer of that number moves together — the cast row, the ANNOUNCE_X rows, and `xNetLifeForX`'s
NET. `cleanupDiscardPriceClause` reports how many of the number came from there, so the aggregate
says what it folded rather than leaving a reader to reconcile it against the prompt's own stack
block.

*(2) the floor.* `- at worst you would be at N` was computed off PRE-cast life. Sphinx's Revelation
gains X: the seat that read "at 4" was going to be standing at 18 + 7 when the bill arrived.
`castLifeDelta` is the cast's own signed life movement, passed from the cast row (`sv.maxX *
lifePerX`), and it is NAMED in the clause (`your 18 life plus the 7 this cast gains you, minus the
20 above`) rather than folded into a number the reader cannot reconstruct. The repro's own numbers
now come out at **5**, which is what the game actually charged.

### E5 — ONE manland is ONE entry, at its best affordable rung, with a price
`130v152` seq 17/18/19 printed `Lair of the Hydra, Lair of the Hydra, Lair of the Hydra, Lair of
the Hydra, Lair of the Hydra` against a battlefield line listing **one**. The wave-62 walk appended
`c->getDisplayName()` on every script line that matched inside the per-line loop, so the count
tracked the opponent's MANA, not their board (the corpus's own control case — two physical Hives,
one animate ability each, printing twice — proves it was not a listing convention).

The animate half is lifted out of the per-line loop into `crackBackBestAnimateRung`, **one pure
call per permanent**, which folds every rung to one answer: the best one their next-turn reach
pays for. `crackBackAnimatorEntry` renders it. Three things the clause never said now ride it:
the rung's POWER (MED-10), the rung's COST (MED-10 — the number the gate already had to compute in
order to admit the rung), and why that cost is priced against everything *but* this permanent's own
mana.

`crackBackAnimatedPower` reads BOTH script forms. The wave-61 walk read only `becomes(creature^N/M)`;
every one of Lair's twenty rungs is written `becomes(Creature Hydra,1/1,green)` — the COMMA form
(`borderline.txt:64293-64312`), which `MTGAbility.cpp:5468` itself normalises to a comma before
parsing. That is why the clause named the Lair five times and never once said how hard it hits. The
read is bounded to the `becomes(...)` group, so a caret elsewhere on the line is not a power, and a
number not followed by `/` is not a power.

**LOW-11 (the rung ceiling) is a RENDER fix, not an engine one, and the reason is rules-grounded.**
`152v126` seq 15 offered `{1}{g}`, `{2}{g}`, `{3}{g}` at `Mana available: 5 total`. The missing
`{4}{g}` is not a cap this lane lifted: `ManaEngine::potentialMana` excludes the ability's own
source from the mana it may be paid with (`if (card == target) used[card] = true;`,
`ManaEngine.cpp:607+`, upstream issue 76), and for THIS ability class the exclusion is
rules-load-bearing — a Lair tapped for its own `{g}` is a tapped permanent, and a tapped creature
can neither attack nor block, which is the entire reason to animate it. The engine is right and the
page did not say so. `animateRungCeilingClause` states the rule on the row, gated to a
self-animation offered by a permanent that is itself a mana source (`unwrapSelfAnimate` walks the
`ATransformer` / `ATransformerInstant` nest the same way the damage rider's is walked). No legal
rung is withheld and no shared choke point was touched.

### E9 — D19's relief clause fired 0 times because it was wired to a seam the corpus never uses
`removes N from the CRACK-BACK total above`: **0** occurrences in 2,251 windows, including both
windows lane X built it for. The gate was never the suspect it looked like: `crackBackNextTurnDue`
is the same predicate the LINE is rendered from, and in both repros the line was demonstrably on
the screen. **The rows never reached the emitter.** Lane X wired the clause into
`AIPlayerGPT::chooseTarget` only, and recorded that it had "deliberately left alone" the other
`damageTargetVerdict` call site as "the LOG-narration call site". That site is
`AIPlayerGPT::describeAction` — not narration at all, but the DECISION-ROW builder
(`renderOrder.push_back(std::make_pair(describeAction(*candidates[c]), ...))`). Both repro rows —
`125v146` seq 15 row 3 and `126v146` seq 38, `Deal 1 damage with Staff of Nin targeting Goblin ...
{right now: takes 1 damage - DIES} [cost: Tap]` — are rows it built. An activated ability whose
target is already bound never opens a chooseTarget ask, so the clause had no window to fire in.

`crackBackScreenTotal` packages the screen's figure for the row builder, gated on exactly the
render's own conditions (same walk, same `crackBackNextTurnDue`, same seat and phase), and
`describeAction` now carries the clause under the same three guards the ask seam uses: a DIES
verdict, a body the OPPONENT controls, and a line actually printed above.

Second half, unasked but owed: `stripNarrationDecoration`'s drop list carried `"{removes: "` (the
wave-54 victim list, with a colon) and therefore did NOT cover `{removes N from the CRACK-BACK
total above: ...}`. Nothing leaked in the wave-62 corpus only because the clause never rendered.
The entry is widened to `"{removes "`, which subsumes both; the colon form has its own regression
pin.

### E13 — the punisher paragraph's direction, the loser of each number, the counter's class
*(a) MED-8.* Lane Y's D3 bound the two forecast LINES to the clause that is true for the number
each carries, and left `drawPunisherSummaryText` alone; the paragraph then stood eight lines above
a corrected forecast (`162v126` seq 13) carrying the chaining caution over BOTH its numbers. The
paragraph states two numbers with opposite owners — *"every card YOU draw costs you N"* is life the
pilot loses, *"every card the OPPONENT draws costs them M"* is life they lose — and a loop held by
side S is entered only by life S's OPPONENT loses (or life S gains). Each sentence now carries the
clause true for ITS number, from the same `loopCautionForLine` D3 built. Nothing the model relies
on is deleted: where both numbers are on the page both clauses print; the only string that
disappears is a false one. `loopCautionForBoards` (one sentence for the whole block) is no longer
called from this path.

*(b) MED-9.* `= N life to you from your punishers above` reads as life GAINED, and no draw punisher
in the pool gives the pilot a point of life (Underworld Dreams, Fate Unraveler and Ob Nixilis are
all `damage:N` to the DRAWER). Both forecast lines now name their loser: `life LOST BY THEM to your
punishers above (that is life removed from their total; you gain none of it)` and `life LOST BY YOU
to their punishers above`.

*(c) deck162 MED.* The `{feeds:}` tag's first counter was labelled `converters on your
battlefield:` and its sibling `discard punishers (a different class ...)`; 0 of 63 of the first
list ever named a discard punisher, so the render was right and the LABEL invited the conflation.
It now reads `draw converters (they fire when the opponent DRAWS) on your battlefield: N` /
`draw converters in your hand: N`, and the zero-case sentence no longer says "draws or discards".

### E15 — the generic-cost rule on the ZERO-mana line
`mana of ANY colour pays a generic cost like {2}` rendered on 1922/1922 nonzero-mana lines and
0/177 zero-mana lines. `123v130` seq 26 printed the bare zero line; seq 27 wrote *"Colored mana
cannot be used for generic costs ... This deck is uncastable"* and burned 10,799 bytes of overrun
on it; seq 28, back at one source, carried the clause again. The payment rule is a fact about MANA,
not about this board, so it is stated on both faces — and the zero line also carries the cheaper
truth the review asked for (what would make mana: an untap step, a land drop, a mana ability). It
still claims no source, no colour and no source list.

### E16 — `{spare:}` stops being hostage to one uncastable card
`125v162` seq 94: eleven lands down, and all four land rows read `{you control 11 lands already;
the most expensive card in your hand costs 15}` with no `{spare:` prefix, because Emrakul, the
Aeons Torn `{15}` was on the list — and the guide's cleanup ordering keys on that literal string.
A card the seat cannot reach cannot be what the next land drop is FOR, so it is not the demand the
verdict is about. The caller's cost scan now caps the comparison at `myLands + 2` (the review's own
suggestion) and hands the excluded card to the clause, which NAMES it with its cost and states the
cap — nothing is hidden and the arithmetic is checkable. With no card past the cap the clause is
what wave 62 printed, but for C11's own wording (`the most expensive card in your hand you could
still reach costs N`).

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), qmake untouched, no new sources.
Detached unit `w63-AC-gate`, `MemoryMax=4G` / `MemorySwapMax=0`; no cap kill.

| run | result |
|---|---|
| PARSETEST | **`=== self-test: 4179 passed, 0 failed ===`** (baseline 4111/0; **+68 cases**) |
| suite, default threads | **1254 tests (2 failed), 66 AI tests (0 failed)**; `==Test Failed !==` **2**, `==Test timed out` **0** |
| the 2 failures | `lifeline.txt` and `merrow_reejerey.txt` — the brief's known concurrency-only pair, named from the log |
| suite, `WAGIC_TESTSUITE_THREADS=1` | **1254 tests (0 failed), 66 AI tests (0 failed)**, 0 timeouts — **0 new failures** |
| `intrepid_adversary_repeated_payment.txt` | passed in both runs of this gate (it is lane AE's item; not exercised for its rate here) |

Test count 1254 -> 1254: this lane ships **no suite fixture**, and claims none. No item here changes
what the ENGINE does with a card, an option or an answer — all six are render/classification
changes, so no fixture can be RED. Baseline AI count is 66 on `41fbad879` (the brief's 65 is the
`caa5d6ea3` figure); this lane added no AI test.

### RED-on-base evidence
RED was produced the way lane X produced it: every mechanism reverted to its wave-62 behaviour with
the new cases in place, and the binary rebuilt. Reverted: the zero-mana line's clause; the
`{feeds:}` label (all three strings); both forecast loser-namings; the paragraph's per-sentence loop
clauses (restored to one trailing clause); `cleanupDiscardPriceClause`'s stacked-draw sentence and
`castLifeDelta`; `crackBackAnimatedPower`'s comma form; `crackBackAnimatorEntry`'s power/cost tail;
`animateRungCeilingClause` emptied; the widened `"{removes "` strip entry; and
`discardSpareLandClause`'s reach wording and exclusion clause.

Result: **`4132 passed, 47 failed`**. **24 of the 47 are this lane's own new cases** — E2 ×3,
E5 ×6, E9 ×1, E13 ×9, E15 ×2, E16 ×3 — and the other 23 are pre-existing wave-51/55/57/60/61/62
cases that were UPDATED (not deleted) to the strings this lane changed, which is why they go RED
under the reversion too. The remaining 44 new cases are NEGATIVE / must-NOT-match / echo-shape /
defaulted-call pins that pass on base as well, which is what they are for.

---

## Predictions (falsifiable, for the wave-63 corpus)

1. **E2** — every `CLEANUP PRICE` clause rendered on a prompt whose own `ON THE STACK` block lists
   one or more pending draw triggers for the seat carries `that count INCLUDES N card(s) from draw
   triggers already ON THE STACK`, and the number in the following cleanup ask's *"your hand has K
   cards"* equals the clause's own `leaves K cards in hand` figure minus whatever the seat spent in
   between. Every `CLEANUP PRICE at X=` clause on a life-gaining X spell carries `(your L life plus
   the G this cast gains you, minus the C above)`. **Falsifier**: one priced Revelation row whose
   `at worst you would be at` figure is below the life the following cleanup ask actually starts
   from, other than by cards the seat spent.
2. **E5** — in every `CRACK-BACK` clause corpus-wide, each permanent name appears at most ONCE in
   the `noncreature permanents of theirs that can animate and attack are not in that count` list,
   and the count of names in that list never exceeds the number of matching permanents on the
   prompt's own opponent battlefield line. Every entry carries either `best rung their mana pays
   for: N power once animated` or `this render will not claim its size`, and (where the cost head
   parsed) a `for {...}` figure. **Falsifier**: one repeated name, or one entry with neither a power
   figure nor the no-claim wording.
3. **E5 / LOW-11** — every menu offering two or more `becomes a K/K` rows off a mana-producing
   permanent carries `{rung ceiling:` on those rows, and 0 such clauses appear on a permanent that
   makes no mana. **Falsifier**: a multi-rung manland menu with no ceiling clause.
4. **E9** — `removes N from the CRACK-BACK total above` occurrence count is **> 0**. Every row
   reading `DIES` for an opponent-controlled creature, in a record that also renders a `CRACK-BACK
   NEXT TURN` line, carries the clause — whether the row came from a target ask or from an
   activation row with a bound target — and 0 such clauses appear in a record with no crack-back
   line or on a body the seat controls. 0 narrated records (`chosen_text`, history) contain the
   substring `{removes `. **Falsifier**: another 0-occurrence census, or one clause on a
   seat-controlled body.
5. **E13** — 0 prompts carry a `LOOP CAUTION` sentence attached to the `Every card the OPPONENT
   draws costs them` sentence; where a completed pair sits on the opponent's board and both
   punisher sides are present, the paragraph carries `LOOP CAUTION` on the YOU-draw sentence and
   `LOOP SCOPE` on the OPPONENT-draw sentence, and the two no longer contradict the forecast eight
   lines below. `life to you from your punishers above` and `life to the punishers above`: **0**
   occurrences; `life LOST BY THEM` / `life LOST BY YOU`: every forecast that prices a draw.
   `; converters on your battlefield:` (unqualified): **0**. **Falsifier**: one of any.
6. **E15** — every `Mana available: 0 total` line carries `mana of ANY colour pays a generic cost
   like {2}`, and 0 replies in the corpus assert that coloured mana cannot pay a generic cost.
   **Falsifier**: one bare zero line, or one recurrence of the uncastable-deck conclusion.
7. **E16** — in every deck125 window whose hand holds Emrakul and whose land count exceeds the next
   most expensive reachable card plus one, the land rows carry `{spare:`; every land row whose hand
   holds a card past `lands + 2` names that card with `is NOT counted here`. **Falsifier**: an
   11-land Emrakul cleanup with no `{spare:` on any land row.

---

## What I did NOT verify

- **No live model probe was run, and no game was driven.** PARSETEST, the suite and source reading
  are the whole gate. Nothing here has been seen by a model; every prediction above is a
  prediction, not a measurement.
- **E9's WIRING is not covered by a RED case.** The new PARSETEST composes the row as
  `describeAction` now builds it and pins the shape, the ordering after the verdict, the echo and
  the strip — but `describeAction` itself takes a live `OrderedAIAction` and a board, so no pure
  case proves the call site fires. The RED evidence for E9 is the strip half only. That the clause
  now reaches the rows is argued from the call graph (`renderOrder` -> `describeAction`) and from
  the two repro rows' shape, and it is prediction 4 that will settle it.
- **E9's other verdict families are still bare.** Lane X's own note applies unchanged: destroy-effect
  target rows, sacrifice rows and Siege-Gang's own sacrifice carry no verdict emitter to hang the
  clause on and were not given one. The shrink-verdict (`ptDropTargetVerdict`) path in
  `describeAction` was NOT given the clause either — that row builder has no `ptDrop` computation,
  and adding one is a different item.
- **E5's dedupe is proven at the FOLD, not on a board.** `crackBackBestAnimateRung` is pinned pure
  (five affordable rungs -> one answer; the ceiling tracks the reach; the colour gate both ways),
  and the emitter appends once per card because the call sits outside the per-line loop. I did not
  render `crackBackFloorSources` against a live two-Lair board, so the "two physical copies still
  print twice" half is argued from the loop structure, not observed.
- **E5's rung-ceiling clause is not measured for reach.** `unwrapSelfAnimate` was verified by
  reading `MTGAbility.cpp:5464-5498` (the `becomes(` parse) and by the `dynamic_cast` nest; I did
  NOT enumerate which other cards in the pool arm it. A permanent that both makes mana and animates
  itself by some route this cast does not catch will render no clause, and one that animates a
  DIFFERENT permanent should not — that direction is untested against a board.
- **I did NOT change `ManaEngine::potentialMana`.** LOW-11's ceiling is left where upstream put it,
  deliberately: it is a shared choke point every Baka payment plan reads, the exclusion is
  rules-correct for this ability class, and two corpus-killing regressions in two waves came from
  exactly this kind of edit. If a later wave decides the exclusion is wrong for a NON-tap ability on
  a mana source, the fix belongs there and needs its own suite evidence.
- **E2's stack walk covers `AADrawer` only.** A draw delivered by some other ability class on the
  stack (a replacement effect, a `MultiAbility` payload this walk's nest does not reach, a draw
  written as something other than `draw:N`) is not counted, and the clause will then under-report
  rather than over-report. I read `AADrawer::resolve` and `getNumCards`; I did not enumerate the
  pool for other stack-borne draw shapes. `WParsedInt` is re-evaluated by `getNumCards()` at render
  time — a count that depends on a board that changes before resolution can move, which is the same
  modality the clause's "unless you spend cards before then" wording already carries.
- **E2's `castLifeDelta` is supplied at the CAST row only.** The ANNOUNCE_X menu rows
  (`xLifeDrawRowCore`) print no `you would be at` figure at all, so they take the stacked-draw
  sentence and no life delta; if a later item gives those rows a life total, it must pass the delta
  too.
- **The `{spare:}` reach cap is a HEURISTIC**, not a derived number: `lands + 2` is the review's
  suggestion, not a measured curve, and I did not check it against the pool's actual land counts. A
  hand holding a card at exactly `lands + 3` will read as out of reach on a board that could in fact
  get there.
- **E13(a) leaves `converterSummaryText`, `pendingLoopWarningText` and `oneShotDrawGrantTag`
  untouched.** The last of those carries its own `converters on your battlefield:` string (it says
  "punishes their draws" explicitly, so it is not the conflation deck162 named) and was not
  relabelled; if a later reviewer wants one vocabulary across all three surfaces, that is a
  separate change.
- **`loopCautionForBoards` is now called from nowhere.** It is left in place rather than deleted
  (deleting is not this lane's business and the compiler's unused-static warning is not fatal here),
  but a reader should know the paragraph no longer uses it.
- **No cross-lane interaction was tested.** Lanes AB, AD and AE touch adjacent emitters; the
  combined diff has not been built here. Two of this lane's changes sit near shared choke points and
  are worth a merge-time look: `stripNarrationDecoration`'s drop list (widened) and
  `forcedCleanupInputs`'s `handAfterCast` (now larger for every consumer, including `xNetLifeForX`'s
  NET and the `xMonotoneMarker` badge that folds it).
- **Key safety.** Everything added here is derived from the board or from the script, never from a
  counter, a clock or a running total of asks, so nothing new can differ between two rebuilds of the
  same window. The one figure that MOVES with the game state rather than the window —
  `stackPendingDrawsFor` — is a fact about the stack as it stands, which is part of the state the
  key is supposed to distinguish, and it is excluded from the narrated record by
  `stripNarrationDecoration` like every other decision-time tag.
