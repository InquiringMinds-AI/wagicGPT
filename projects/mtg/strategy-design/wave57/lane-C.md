# Wave-57 lane C — cast-row and discard prices (D7, D8, D11, D30, D21, D12)

Branch `w57-lane-C`, worktree `worktrees/lanes/w57-C`, base master `4d4ea0bdf`
(wave-57 lane brief; src at wave-56 close). Every change is a RENDER change:
no legality, no answer index, no staleness key, no engine semantics move.

## Gate

| gate | base (this worktree, first run) | after |
|---|---|---|
| PARSETEST | 2736 passed / 0 failed | **3146 / 0** (+410, all `#W57-C`) |
| suite (default threads) | 1241 tests, 2 failed (`lifeline.txt`, `merrow_reejerey.txt` — the known concurrency-only pair), 55 AI / 0, 0 timeouts | **1241, the SAME 2, 55 AI / 0, 0 timeouts** |
| suite (`WAGIC_TESTSUITE_THREADS=1`, ground truth) | — | **1241 tests (0 failed), 55 AI / 0, 0 timeouts** |

Both gates ran on a hermetic `make -B` under `MemoryMax=4G/6G, MemorySwapMax=0`
(no kill). Logs: `~/.gatelogs/w57-C-base-{build,parsetest,suite}.log` (base),
`~/.gatelogs/w57-C-{build,parsetest,suite,suite-st}.log` (after).
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

No suite fixture is added: nothing in this lane changes what the engine DOES.
The +410 PARSETEST cases are the pin, and 360 of them are the D7 arithmetic
walked over every toughness 1-12 × life 1-30 (skill #259 — the number checked
against its inputs, not the clause checked for presence).

---

## D7 (HIGH) — the creature cast row under THEIR converter

`src/AIPlayerGPT.cpp`: `theirConverterScan` (the opponent half of
`converterSituationLine`'s own scan, so the row and the paragraph cannot
disagree about who controls what) + `theirConverterBodyTag`, called on every
CREATURE cast row.

```
Cast Emrakul, the Aeons Torn {15} (15/15) {their converter: this body has toughness 15
and they control 2 life-to-damage converters (Sanguine Bond #1, Sanguine Bond #2) - any
effect of theirs that gains them life equal to its toughness (a sacrifice-for-toughness
edict, a drain of that size) takes 30 off YOU: life 37 -> 7}
```

Two deliberate departures from the docket's suggested wording, both about
truth-scope: (a) the loss is multiplied by the converter COUNT (two Sanguine
Bonds are two triggers — the docket's single-`toughness` form under-states the
repro game by half); (b) the sentence is a CONDITIONAL on the effect class
("any effect of theirs that gains them life equal to its toughness"), because
the engine cannot see their hand and asserting they hold such an effect would
be a wrong-scope claim. It forbids nothing — PARSETEST pins that the tag
contains no "cannot" / "do not cast" / "they will".

**Prediction.** Creature cast rows rendered while a converter of theirs is on
the battlefield and carrying no converter price: **0 of N** (wave-56: all of
them). The lethal branch (`; that KILLS you`) fires at least once in a corpus
where the pair recurs.

## D8 (HIGH) — the discard menu's verdicts become ONE family

`discardBoardVerdictTag` runs the cast row's own evaluator (`dynamicMagnitudes`)
over every discard row and re-badges its output through the engine's own
zero-predicate (`AIPlayerGPT::rowSaysNoOp`, now public — three surfaces share one
definition of "dead"): a verdict that reads zero prints under the SAME
`{dead right now: ...}` tag the wave-55 target-count clause already used; a
verdict stating a real magnitude prints as `{right now: ...}`.

The `123v130` seq 22 asymmetry, as it renders now:

```
1. Tragic Slip {b} (instant) {dead right now: 0 legal targets on the board for it}
2. Tribute to Hunger {2}{b} (instant) {dead right now: they control 0 creatures - at 0 this does nothing}
3. Damnation {2}{b}{b} (sorcery) {dead right now: destroys 0 of their creatures (0 without a restriction against attacking), 0 of yours}
```

`discardVerdictLegend` prints once under the rows when any verdict rendered.
Its last sentence is the load-bearing half and is pinned: **"A row carrying
NEITHER tag is one this engine could not price here - that is not a statement
that the card is dead."** Without it the family rule the guide wants
("the unannotated row is the keep") would be a lie about every card the
evaluator does not model.

**Prediction.** Discard rows whose card has a computable dead-verdict and
carries none: **0 of N** (wave-56: 357 of 426 bare; `{MV` 0 renders).
Sweeper/edict cards sent from a cleanup menu while the opponent's creature
count is 0: below wave-56's 4-in-one-game.

## D11 (MED) — the edict row counts the seat's own answer on the stack

The wave-53 `edictAlreadyOnStackClause` was wired to the highest-MV branch
(Soul Shatter class) ONLY; the plain edict branch (`notaTarget(creature|
mybattlefield) ... targetedplayer` — Tribute to Hunger, Devour Flesh) never
asked the stack. At N == 1 the victim is DETERMINED and named on the row, so
the stack can be asked about it exactly as the other branch asks about its tied
tops; at N > 1 the row names no victim, so there is nothing to contradict and
nothing is emitted.

```
Cast Tribute to Hunger {2}{b} {right now: they control 1 creature - Silverquill Silencer
is sacrificed, you gain 2 - but an effect already on the stack is aimed at that same
creature; if it is gone when this resolves they control 0 creatures and this does nothing}
```

Worded as a conditional, and NOT as the highest-MV clause's "this one would
find their next-highest" — at N=1 there is no next-highest. It does not claim
the removal has happened (the stack object can still be countered or fizzle)
and it does not claim ownership of that object (the predicate does not check a
controller).

**Prediction.** Cast rows naming a victim already targeted by an object on the
stack, with no such tail: **0 of N** (wave-56: 3 of 3). Second Tributes spent
on an already-answered creature: 0.

## D30 (LOW) — two different cards, one verdict, two prices

`applyDuplicateEffectTags` now also finds the cross-card match and marks the
dearer row with a NARROWER claim than D15's (which keys on the card name and
rendered 0 times all corpus):

```
2. Cast Tribute to Hunger {2}{b} {right now: they control 0 creatures - at 0 this does nothing}
   {identical verdict right now to row 1 (Devour Flesh), which costs 1 less mana - a
   different card, the same priced outcome on this board}
```

D15's same-card tag stays and WINS the row when both match — the stronger
statement is not diluted by a second tag about a third row. Pinned negatives:
different verdicts are never called identical, equal costs mark nothing,
unpriced rows are never compared, and the cheaper row is never tagged.

**Prediction.** Menus carrying two rows with byte-identical `{right now:}`
verdicts at different prices and no mark on either: **0 of N** (wave-56: the
`123v125` seq 45 pair, unmarked). The tag renders at least once (D15 never did).

## D21 (MED) — the cycle row prices the cast, and the cast row the cycle

The cast-mode menu offers two mutually exclusive uses of one card and neither
row said what the other was worth. The pass that already computes each row's
own cost/clause/draw now also cross-prices:

```
cycling with Lay Waste [cost: {3}, discard this card] {the cast this replaces: cost {2}{r}
  - Destroy target land. (4 legal targets on the board right now)}
Cast Card Normally [cost: {2}{r}, Destroy target land.] {the alternative this replaces:
  cycling for {3}, discard this card - draws 1 card}
```

The cast side carries a NUMBER (the same chooser count every other row gets),
and `0` is printed as `0` — a dead cast is exactly the fact that makes the
cycle correct, and a silent omission gets confabulated over. A path that draws
nothing says nothing about drawing rather than the false "draws 0". Append-only:
the answer index and `req.optionTexts` (the staleness key) are untouched.

**Prediction.** Cycle takes at `Opponent life:` ≤ 6 rise above wave-56's
**3 of 17**; cast-mode rows carrying no clause about the path they displace:
**0 of N**.

## D12 (MED) — the menu-level no-live-cast-row token

`everyCastRowDead` + `allCastRowsDeadNote`, computed over the CAST rows only
(before the decline and hold rows join the menu, since neither is a cast and
neither carries a board verdict) and emitted as a HEADER TOKEN under the
question:

```
NO LIVE CAST ROW ON THIS MENU: all 3 cast rows below carry a verdict computed from the
board that reads zero - not one of them changes a number on the board as it stands.
Nothing is withheld and no row is capped: casting one is still legal and still your
choice, and the decline and hold rows are on the menu as always.
```

The predicate reads the ROW VERDICT ONLY (`rowVerdictClause`, brace-balanced),
never the text around it — a `{card text: "..."}` blob quoting "does nothing"
or "deals 0" can never make a live row read as dead (pinned). And EVERY cast
row must carry a verdict: one unpriced row and the token does not fire, because
silence is not a dead verdict. No window is removed, no row is capped, and the
token says so in its own words.

**Prediction.** Menus where every cast row carries a dead marker and no
menu-level line prints: **0 of N** (wave-56: 85 of deck123's 197). Spends off
an all-dead menu below deck126's six-in-one-game.

---

## What I did NOT verify

* **No live corpus, no live game.** Nothing in this lane has been observed
  rendering into a real prompt. Every claim above is a PARSETEST claim about
  pure functions plus a reading of the call sites; the next corpus is the gate.
  Specifically un-observed: whether `dynamicMagnitudes` yields a non-empty
  clause for the D8 sweeper/edict class *on a hand card during the cleanup
  step* (it is the same object the cast menu evaluates, and the cleanup row
  already calls `modalSpecObjectCount` on the same card, so the shape should
  hold — but "should" is not an observation); whether `ctx->spellTargetType` is
  populated for the cast-mode menu's context card (D21's target count is
  omitted, not wrong, if it is empty); and whether D12's token ever fires in a
  live game, since it requires EVERY cast row to carry a `{right now:}` verdict
  and deck126's other dead markers (`[second copy: ...]`,
  `[finds only an enchantment card ...]`) are bracketed annotations the
  predicate deliberately does not read. That last one is a known, deliberate
  under-fire: the docket's 85-of-197 deck123 figure counts "all-dead" by a
  looser predicate than mine, so my N will be smaller than the docket's.
* **The D7 conditional is not a claim that the effect exists.** If the corpus
  shows the tag rendering on boards where no such effect is ever available, the
  tag is noise at that seat and the next wave should scope it (e.g. to boards
  where the opponent has cast such an effect before) rather than widen it.
* **No performance measurement.** D8 adds one `dynamicMagnitudes` call per
  discard row (which can build and delete a `TargetChooser` through
  `spellCanTargetSelf`) and D21 adds one `modalSpecObjectCount`. Neither was
  timed; both ride paths the cast menu already runs per row.
* **Prompt-size cost not measured.** The five new annotations were not weighed
  against D13's narration-share budget (lane D's item this wave).
* I did not re-derive the docket's corpus counts from
  `matchups-20260903-120952` — I read the ledger records as filed and built
  against the call sites they name.
