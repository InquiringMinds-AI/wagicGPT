# Wave-68 lane BD — J8 and the render MEDs (the wrong side of the ledger)

Base: master `4837e76a1` (wave-68 step-one brief). Branch `w68-lane-BD`, worktree
`worktrees/lanes/w68-BD`. Files changed: `projects/mtg/src/AIPlayerGPT.cpp` (only).

Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); **no kill under the cap on
any run**. The gate ran DETACHED (`w68-BD-gate`) with a foreground until-loop wait on
`~/.gatelogs/W68_BD_DONE` (removed before launch); the binary was freshness-checked
(`ls -la bin/wagic`) after every make and no build was ever chained behind a counting grep.
Every edit was a python `rb`/`wb` byte splice; `git diff | /usr/bin/grep -c $'\357\277\275'`
= **0** after every one. Every comment tagged `#W68-BD (<item>)`.

Everything here is a RENDER change: none of it changes what the engine does with a card, an
option or an answer, so no suite fixture can be RED on base and none is claimed. RED was
produced the way lanes AW/AQ produced it — the mechanisms reverted to their wave-67 behaviour
with the new cases already in place, binary rebuilt and freshness-checked. See **RED-on-base**.

Nothing volatile entered `mPromptTail` or an ask key: no counter, clock or running total was
added anywhere, and the one transform over the composed narration
(`narrationFoldPaidSources`) is a pure function of `mNarration`, which two rebuilds of the same
window agree on (wave61/corpus-livelock.md). `req.optionTexts` (the staleness key), the row
ORDER and the answer INDEX are untouched by every row change below — each appends to the RENDER
copy only.

---

## What shipped

### J8 (a) — ONE edict floor, for mode rows and standalone rows (deck146 HIGH-1)

`146v123` s31, verbatim: `Opponent battlefield (39 permanents listed, of which 31 are
creatures ...)` and rows `4.`/`7.` reading only `{this mode has a legal object right now}`.
The seat's own plan one window earlier (s30) named the card it expected them to give up
(Thraben Doomsayer); it then spent BOTH Commands on that mode and killed two 1/1s of 31.
COUNTED on the corpus before designing: `this mode has a legal object right now` renders in
**15 prompts / 201 rows**; the standalone edict clause `they control N creatures - they choose
which one` renders in **17 prompts**. Two surfaces, one fact, and only one of them carried it.

* **The walk is now one function.** `edictFloorScan(them, theirCreatures, minToughness, atFloor,
  onlyOut)` is the #W62-X floor walk and the #W67-AW at-floor count, lifted out of
  `boardTurnOnClause` verbatim; the standalone cast row calls it, and so does the mode row. A
  named victim is returned ONLY at exactly one body, which is the only case where naming one is
  a claim.
* **The clause is `edictClause`'s, byte for byte.** `modeEdictFloorTag` composes
  `edictClause(N, name, 0, false, ...)` and adds the mode's own scope — which half of the pair
  this is — plus, at more than one body, the sentence the gain-bearing standalone row already
  carries in its own words: *the pick is THEIRS, so plan on their least valuable body, not the
  one you want gone*, with the at-floor count in parentheses. A mode edict grants no life, so
  edictClause's gain terms are off.
* **The mode is identified off the SCRIPT, never off the label.** `GptModalMode` gained
  `subOpponentEdict`, set in `modalChoiceModes` only when the grant handed to the OPPONENT
  carries a `sacrifice` verb over a spec that was rewritten from `mybattlefield` and names a
  creature. A controller-side sacrifice and an opponent-side bounce are pinned as
  MUST-NOT-MATCH.

### J8 (b) — a granted draw is a PRICE, on the mode row and in the bracket (deck146 HIGH-2)

`146v125` s16 row 2 led with a number good for the seat (`they LOSE 1 life`) and reported the
card as a bare fact (`they draw 1`); the cast row's DRAW GRANT bracket at s19 priced it only as
`those 1 draws take 0 off them`. The seat read both exactly as written, twice, and wrote *"a
2-for-1 in card advantage (I spend 1 card, they draw 1)"*. COUNTED: `take 0 off them` renders in
**39 prompts / 78 branches** — on this corpus the ONLY framing a granted opponent draw ever got
was the number zero.

* **The mode row** (`modeEffectPriceTag`): `you HAND THEM N cards - that is a PRICE this mode
  pays, not a gain of yours: a card in THEIR hand is theirs to use against you`. `#W47-R3`'s
  framing for life THEY gain, on the card they draw. The count still prints.
* **The mirror is NAMED**: `modalMirrorModeLabel` pairs `return creature and opponent draws` with
  `return creature and you draw` by an exact SCRIPT segment match after a pure actor-token swap
  (`controller` <-> `opponent`) — never by comparing the labels, which is precisely the
  comparison the model was already getting wrong. No match = no claim.
* **The bracket** (`oneShotDrawGrantTag`): the punisher arithmetic is untouched and
  `- and either way this HANDS THEM N cards: that is what this row COSTS you, and it is not
  measured by the punisher figure above` is appended. A branch that draws only the pilot says
  nothing about them (MUST-NOT-MATCH).

### J8 (c) — the crack-back cover on the row that REMOVES an attacker (deck126 MED)

`126v152` s14: the header read `CRACK-BACK NEXT TURN: ... for up to 18 - you would be at 0; that
would KILL you` and the Tribute to Hunger row — the one row that subtracts from that total —
carried its edict floor and no cover clause, while a creature row in `126v123` s12 carried
`{crack-back cover: ...}`. COUNTED: `CRACK-BACK NEXT TURN` renders in **358 prompts**;
`crack-back cover:` in **42** — the clause exists and reaches only rows that ADD bodies.

`crackBackRemovalRowTag` is a new pure function, not the blocker tag: an edict is not cover, and
the rules-correct arithmetic follows from WHO CHOOSES.

* **The deck-126 review's premise is not adopted as stated.** It says "they sacrifice their
  smallest, so the floor is computable". They sacrifice whatever they like: if ANY creature they
  control is not in the crack-back total, they hand over that one and the total does not move.
  So the clause says exactly that — `N of their M creatures are NOT in that total, so they can
  hand over a body that was not going to attack: the FLOOR of what this takes off the crack-back
  is 0 and the line above stands unchanged`.
* **Only when every creature they control is in that total** is a removal proven, and then it is
  the SMALLEST contribution: `18 - 2 = 16 -> you would be at 2, which you SURVIVE - and they can
  only give up MORE than 2, never less`.
* **#W65-AL (G4) is obeyed**: against a total the line above calls a FLOOR, no survival verdict
  and no KILL verdict is printed, and the clause says the figure is computed against a number
  that can only go up.
* The bodies in the total come from the SAME `crackBackBodyContribution` walk the LINE is built
  from, and the creature count from `edictFloorScan`, the walk the row's own floor came from; two
  walks that disagree (`attackerBodies > theirCreatures`) claim nothing. Gated through
  `crackBackScreenTotal`, so the row can never point at a line that is not above it. Wired on the
  plain-edict class only (`CastRowBoardAnswer::edictKind == 1`).

### MED — `NO LIVE CAST ROW` says the casting seam is not the turn (deck146 MED-2)

`146v125` s22/s24 are both CORRECT all-dead casting menus, both answered correctly — and on
those two turns the seat cast nothing AND animated nothing, with an untapped Hive on the board
against an opponent at zero creatures; across that game it declined the animation on an untapped
Hive **twelve** times. COUNTED: the header renders in **136 prompts**. It now closes with a
SCOPE statement (land drop, activated abilities incl. a manland animation, attack, planeswalker
loyalty — all asked at their own windows, none priced here) plus, when and only when the engine's
own `LegalActionsOracle::legalLandPlays` is non-empty, the count it holds. At 0 legal land plays
nothing is claimed about the land drop.

### MED — the band row costs one clause, not two (I9a, wave-67 adjudication)

The adjudication says I9a's byte saving is ZERO after AZ R4, 7 band windows at 2,514-2,737 B.
MEASURED why, on the corpus row (`152v146` s24): each band row is **103 bytes** —
`add 2 counters` plus `{this mode has a legal object right now}` (39 B) plus
`{identical in effect right now: adds 1 counter}` (46 B), nineteen times over. Every rung stays
on the list and stays answerable; the row is reset to the ENGINE's own label (only when it
provably still begins with it; otherwise wave 67's truncation point stands) plus
`{same effect right now: adds 1 counter}` — **54 bytes, 49 B smaller per row**, ~930 B per window
across 19 band rows. The band's LEGALITY is stated once, on the kept row's collapse tag ("any of
them is a legal answer"), instead of once per row. Both spellings stay on
`stripNarrationDecoration`'s list.

### MED — the opponent's library as a deck-out forecast (deck162 LOW)

`162v126` was won BY the opponent decking out while the seat sat at 10 life with no idea it was
coming. `opponentZoneCountsLine` now appends, at 3 cards or fewer only, the engine rule and the
count-down: they can survive at most N more draws and the draw after that loses them the game,
and any extra draw of theirs makes it sooner, never later. Outside three cards the line is
wave-67's byte for byte.

### MED — a stage-1 side gate says how many objects each side can reach (deck152 MED-4)

`152v125` s59, verbatim: `1. choose a creature / 2. Decline - do nothing` with the opponent's
board line reading `of which 0 are creatures`. The seat took row 1 and wrote *"the opponent has
no creatures on the battlefield, so the creature target for Teferi's +1 is Staff of Nin"* — an
artifact — and the stage-3 list (s60) held only its own three bodies, with no pass row.
`gateSideBranches` reads the gate's two branches off the SCRIPT (`may name(Choose a creature)
ability$! choice name(Choose your creature) target(creature|myBattlefield) untap _ choice
name(Choose opponent creature) target(creature|opponentbattlefield) tap!$`); anything but
exactly two branches with their own target specs yields nothing. `gateSideCensusTag` prints each
side's count from the engine's own TargetChooser (`modalSpecObjectCount`), and a count of 0 is
NAMED — `"Choose opponent creature" - NONE` — instead of being left for the pilot to infer from a
battlefield line forty lines up. **The gate is not suppressed and stays answerable**, which is
the doctrine's requirement; the brief's ask was "say none instead of asking", and this says NONE.

### MED — the narration census, and the one fold it justifies

MEASURED over the wave-67 corpus (42 seats, largest narration per seat: **479,148 B / 11,048
lines**, mean 11,408 B / 262 lines, max 23,885 B / 511 lines). What the census FALSIFIES:

| candidate fold | measured saving |
|---|---|
| collapse adjacent duplicate lines | **-279 B** (already bucketed: only 43 runs, all of length 2) |
| collapse within-turn duplicates | 9,965 B (2.1%) |
| drop phase markers with nothing after them | 89 B (5 lines of 11,048 — already lazy) |
| collapse draw-step-only turns | 710 B (50 of 952 turns) |

The narration is not full of structural filler. The largest byte class after the event lines is
one line shape: `- Paid {cost} for CARD with A; B; C` — **44,103 B over 513 lines, 9.2% of all
narration bytes, 85 B average**. The mana SOURCES of a payment made in a PAST turn are the one
part of it no later decision reads (which lands are untapped NOW is the CURRENT SITUATION
block's job, and it prints them), while the cost and the card are the history.
`narrationFoldPaidSources` folds that list to its COUNT on past turns only: **18,714 B, 3.9% of
all narration**, no fact class deleted (payment, cost, card and how many sources it took all
still print). The CURRENT turn is untouched, a bucketed `[xN]` line is left to the bucketer, and
a line that would grow is passed through. Ordered AFTER `narrationBucketRuns` so the two
transforms cannot fight.

Other byte classes, for the next lane that wants this: `- You/Opponent used:` 55,904 B / 780
lines (9 of them carry a `-> ` description tail averaging 318 B); turn headers 29,106 B; phase
markers 32,800 B; draw lines 33,603 B; `- You chose` 9,319 B; `- targeted` 8,274 B.

---

## Gate

Detached unit `w68-BD-gate`, hermetic `rm -f bin/wagic && make -f Makefile.sdl -j4`, binary
freshness-checked (`-rwxr-xr-x ... 58037944 Sep 6 12:50 bin/wagic`), MAKE_EXIT=0.

| gate | base (wave-68 brief) | this lane |
|---|---|---|
| PARSETEST | 5107 passed, 0 failed | **5163 passed, 0 failed** (+56) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1274 tests, 0 failed | **1274 tests, 0 failed** |
| AI tests | 67, 0 failed | **67, 0 failed** |
| `==Test Failed !==` / `==Test timed out` | 0 / 0 | **0 / 0** |
| U+FFFD in the diff | 0 | **0** |
| killed under the 4G cap | — | **none, on any run** |

Eight existing cases were UPDATED, not deleted, because a string they pin gained a clause
(`#W57-C D12`, `#W61-S C12` x2, `#W64-AG F6`, `#W65-AL G1`, `#W67-AZ R4` x3). Each keeps its
original assertion and gains the new one.

### RED-on-base — measured, not asserted

The ten mechanisms reverted to their wave-67 behaviour (the mode floor tag and the edict flag
off; the mode price back to `they draw N` with no mirror; the DRAW GRANT cost sentence
suppressed; the removal cover returning empty; the dead-cast scope sentence and land clause
suppressed; the band clause back to `{identical in effect right now}`; the deck-out clause off;
the gate census returning empty and its branch reader returning false; the narration fold a
pass-through), the new cases already in place, binary rebuilt and freshness-checked
(MAKE_EXIT=0, `12:49`).

**`=== self-test: 5132 passed, 31 failed ===`** — 31 RED, 0 with the mechanisms in:

| item | RED cases |
|---|---|
| J8 (a) mode edict floor + the script flag | 5 |
| J8 (b) draw-as-price (mode row, bracket, mirror) | 7 |
| J8 (c) crack-back removal cover | 6 |
| MED dead-cast header scope + land count | 3 |
| MED band row shortening | 3 |
| MED deck-out forecast | 2 |
| MED stage-1 gate census | 3 |
| MED narration payment fold | 2 |

---

## Falsifiable predictions for the wave-68 corpus

1. **J8 (a).** Every Silverquill-class sacrifice mode row carries
   `{this mode's SACRIFICE half is an EDICT: they control N creatures - they choose which one}`
   — 100% of such rows, 0 bare — and no reply names the specific creature it expects the
   opponent to sacrifice on a board where they control more than one (wave 67: `146v123` s30
   named Thraben Doomsayer against 31). FALSIFIER: one bare sacrifice-mode row, or one reply
   naming a determined victim at N > 1.
2. **J8 (b).** Zero replies describe a mode that draws the OPPONENT a card as card advantage for
   the seat (wave 67: 2, at `146v125` s16 and s20), and the take rate on `opponent draws` mode
   rows against a control deck falls below wave 67's 2 of 3. FALSIFIER: one reply calling a
   granted opponent draw a gain of its own.
3. **J8 (b), the mirror.** Every rendered `opponent draws` mode row whose script holds the
   pilot-side twin names it by label. FALSIFIER: a mirrored pair on the screen with no
   `is this same mode pointed at YOU` clause on the opponent-side row.
4. **J8 (c).** Every cast row for a plain edict rendered in a window whose prompt carries a
   `CRACK-BACK NEXT TURN` line also carries `{crack-back cover:` — and in the case where their
   bodies exceed the crack-back set, the clause reads `FLOOR ... is 0` rather than promising a
   removal. FALSIFIER: an edict row beside a crack-back line with no cover clause, or a cover
   clause promising a removal on a board where they hold a non-attacking body.
5. **MED, dead-cast header.** On turns whose casting menu carries `NO LIVE CAST ROW`, the seat's
   rate of ALSO taking no land drop and no activated ability falls below wave 67's rate on
   `146v125` (12 declined animations on an untapped manland across the game, 2 of them on
   all-dead-cast turns where it also cast nothing). FALSIFIER: the rate is unchanged across
   >= 6 such windows.
6. **MED, band rows.** Intrepid Adversary band windows drop from 2,514-2,737 B to under 1,900 B,
   every rung still printed and numbered, and no `named_row_reask` or `unparsed_reply` names a
   rung inside the band. FALSIFIER: a band window still above 2,400 B, or an answer landing on
   the wrong engine index.
7. **MED, deck-out.** Every prompt whose `Opponent library:` count is 3 or fewer carries
   `DECK-OUT IS IN RANGE`, and at least one seat in a deck-out-bound game states the deck-out in
   its PLAN before the game ends (wave 67: 0 — `162v126` won on it without seeing it).
   FALSIFIER: a library at <= 3 with no clause.
8. **MED, stage-1 gate.** Every Teferi-class stage-1 gate row carries the two-branch census, and
   zero replies assert a target of the wrong TYPE for that gate (wave 67: 1, `152v125` s59
   naming Staff of Nin, an artifact, as the creature target). Latency on those gates falls below
   the wave-67 39 / 357 / 31 s trio. FALSIFIER: one bare stage-1 gate row, or one wrong-type
   assertion.
9. **MED, narration fold.** Every `- Paid` line before the last turn header reads
   `(paid with N sources)` and none reads ` with `; mean narration bytes per seat fall by
   ~3.9% against wave 67's 11,408. FALSIFIER: a past-turn `- Paid ... with A; B` line surviving,
   or a CURRENT-turn payment folded.

---

## What I did NOT verify

* **No live probe was run, and no selfplay game.** Everything is proven at the PARSETEST seam
  with the corpus's own numbers (s31's 31 creatures, s16/s19's rows verbatim, s14's 18-from-4
  crack-back, s22/s24's header, s24's 103-byte band row, s59's zero-creature gate, the 42-seat
  narration census). No model call was made, so nothing here is verified END TO END against a
  live prompt.
* **The two callers that need a board are proven by construction, not by a test.** The mode
  edict floor and the gate census both run inside `chooseMenuAction`, which needs a
  DecisionRequest; what PARSETEST proves is the SCRIPT parse (`subOpponentEdict`,
  `gateSideBranches`), the pure clause, and the strip behaviour. Whether `opponent()` and the
  recovered subject pointer are the right objects at that seam is code-reviewed only — the
  census/`modalSpecObjectCount` path they reuse is #W66-AQ's, which the wave-67 corpus
  exercised. Predictions 1 and 8 are written to catch a mis-wire.
* **The crack-back removal cover is wired on the PLAIN edict class only.** The highest-MV edict
  (Soul Shatter class) returns before `ans->edictKind` is set, and a PILOT-chosen removal row
  (Devour Flesh, Vanishing Verse) gets no cover clause. Both are honest gaps, not bugs: the
  highest-MV victim is determined and would want a different sentence, and a pilot-chosen
  removal is a CEILING, not a floor. I did not write either branch, because a branch nothing in
  the corpus exercises is a branch nothing can falsify.
* **`edictFloorScan`'s equivalence to the code it replaced is by inspection plus the suite**, not
  by a differential test: the #W67-AW M4 cases still pass on the merged tree, which is the only
  evidence that the lifted walk returns what the inline walk returned.
* **The narration fold's saving is measured on the corpus's LARGEST narration per seat**, not on
  every prompt, and the 3.9% figure is that sample's. I did not measure the effect on decision
  quality, only on bytes; and I did not fold the `- You/Opponent used:` class (55.9 kB, the
  larger one) because its content is per-ability text with no provably-dead sub-part.
* **The band-row label reset is guarded but not proven against a decorated row.** If some future
  emitter prefixes an `add N counters` row before this block runs, the guard falls back to wave
  67's truncation point; I did not construct such a row.
* **I did not adjudicate any other lane's predictions**, did not read the deck reviews outside
  the sections my items cite, and did not touch any file other than `AIPlayerGPT.cpp`.
