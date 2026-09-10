# Wave-75 lane CL — render truth (P5, P6, P10, P11, P15, P16, P17, P23)

Base `207a7f410` (PARSETEST 5,788 / 0 on the seeded tree — measured on this
branch before any case was added, with the fixes in: **5,786 passed, 2 failed**,
and those two failures are P6's RED, see below). Worktree
`worktrees/lanes/w75-CL`, branch `w75-lane-CL`, commit `a464f4a67`.

Gate on this branch: `make -f Makefile.sdl -j8` clean, both link guards OK
(`check-ctor-init: OK (118 file(s))`, `check-reply-instructions: OK (1 source
file(s), 37 guide(s))` — run after the render-string changes). **Suite
THREADS=1, detached unit `w75-CL-suite`: 1285 tests (0 failed), 76 AI tests
(0 failed)** — `==Test Failed !==` **0** and `==Test timed out` **0**; none of
the three known flakes fired, so no solo rerun was needed. **PARSETEST 5,835
passed / 0 failed** (base 5,788 — **+47 cases**). `git diff | grep -c
$'\357\277\275'` = **0**. Every wagic invocation ran under `systemd-run --user
--scope -q -p MemoryMax=4G -p MemorySwapMax=0 --`; nothing was killed under the
cap. No legal option is removed, capped or auto-answered anywhere in this lane.

Corpus for every repro and every census below:
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260909-104713-final`
(2,526 decision records over 21 games).

---

## P5 HIGH — a row that feeds the opponent's chain printed its own arithmetic

### Repro (`125v126` deck125 seq 267, turn 53 Upkeep, life 42 vs 7)

Verbatim, off the record's own `options_text`:

```
1. Deal 1 damage with Staff of Nin targeting the opponent {right now: takes 1 damage - they would be at 6} [cost: Tap] ...
2. Deal 1 damage with Staff of Nin targeting you {right now: takes 1 damage - you would be at 41} [cost: Tap] ...
```

and, on the same screen, twelve lines above:

```
LOOP COMPLETE: BOTH halves of a life LOOP (Sanguine Bond + Exquisite Blood) are on THEIR battlefield right now. Any life YOU lose, and any life THEY gain, chains until you are at 0 - so every life payment on this screen is fatal rather than expensive.
[LOOP RUNNING: both halves of the opponent's life LOOP are on the battlefield and the chain is live ...]
```

Reply: `PLAN: Ping myself to avoid triggering the Sanguine Bond + Exquisite Blood
infinite loop.` / `CHOICE: 2 (Deal 1 damage with Staff of Nin targeting you)`.
Two statements on one screen and one of them false; the model followed the row,
because the row is what the guides teach it to read. The game was 42-7 in its
favour and it lost it.

### RED on base

* **Census, corpus-wide**: the shape rendered **16 times**, every one on the
  deck125 seat of that game (seqs 87, 110, 123, 140, 150, 159, 163, 165, 182,
  186, 196, 211, 225, 242, 254, **267**), at life 37 and then 42. Seq 267 is the
  one taken.
* **PARSETEST, on the record's own bytes** (`#W75-CL P5 RED-ON-BASE`): the
  shipped row contains `you would be at 41`, and two windows of that run
  differing ONLY in that number produce **different ask keys**
  (`stripDeclineReaskTags(joinNumberedRows(rows))` differs) — the wave-74 shape,
  reproduced here as a standing assertion rather than a description.

### Fix

`w75ChainFeedRow` / `w75ApplyChainFeed` (pure). While the opponent's loop is
PROVEN live — `lifeLoopProvenWin(opponent())`, the same predicate the LOOP
RUNNING bracket is rendered from, so the row and the paragraph cannot disagree —
every ` {right now: ...}` group on a row is rewritten on the two shapes whose
direction is unambiguous:

* `you would be at N` with **N > 0** → `you would be at 0 - this row feeds their chain`
* `they gain N` with **N > 0** → `they gain N - you would be at 0 - this row feeds their chain`

`you would be at 0` / a negative projection is left byte-identical (the row
already says it kills the seat); `they would be at N` off damage is untouched
(the header already says "life THEY lose does not enter it"); anything outside a
`{right now: ...}` group is untouched. Applied at the **priority** seam (both
`shownLines` and `renderRows`), the **casting** seam (`menu`, before every tag
and every key) and the **target** seam (`opts`), each time before any key is
taken, so the rows the model reads, the rows the hold latch remembers, the rows
the ask key is built from and the rows the translog records are one set of bytes.

### GREEN + the key-stability pin (the lesson of wave 74)

+17 cases on the P5 block. The pin: the seq-267 rows at life 42 and at life 41
(the same window one link later) are, once the override has run, **byte-identical
rows**, and therefore identical on all five keys — `optionSetKeyOf`,
`holdActionKeyRow` (hold latch), `holdKeyRow` and `w74HoldUnseenRows == 0`
(hold check), `stripDeclineReaskTags(joinNumberedRows(...))` (ask key) and
`asyncSlotKeyOf(...)` (async slot key). The direction matters and is worth
naming: this change **removes** a board-derived number from the rendered row, so
under a live loop every one of those keys becomes strictly MORE stable than it
was — the opposite of the two regressions that killed wave 74's first corpus.
MUST-NOT-MATCH cases: no loop → byte-identical; the opponent-damage row →
untouched; `you would be at 0` and `-1` → untouched; `[DRAW PRICE: ... you would
be at 12]` (outside the group) → untouched; an unterminated `{right now:` →
returned whole; the `{card text: "... {T}: ..."}` group's own braces are never
entered.

### Prediction (falsifiable, next corpus)

Zero rows render `{right now: ... you would be at N}` with N > 0 while a
`LOOP COMPLETE:` or `[LOOP RUNNING:` paragraph is in the same prompt (wave 74:
16), and every such row instead carries `this row feeds their chain`. If a seat
still pays life into a proven opponent loop, the surface is no longer the cause
and the residue is a guide teach or a row shape this override does not reach
(a `{right now:}` that states a life move with no projection at all).

---

## P6 HIGH — CRACK-BACK NEXT TURN was phase-gated to the main phases

### Repro (`125v123` deck125 seqs 68-79, turn 23)

Six turn-23 **Upkeep** asks at 37 life with forty able attackers across the
table, no crack-back line in any of them; seq 79, the Main-1 window six asks
later, prints

```
CRACK-BACK NEXT TURN: 40 of their creatures will be able to attack (tapped ones untap first), for up to 41 - you would be at -1; that would KILL you - you control NO creature, so every point of that i...
```

deck125 is a draw-go control deck: it commits its mana in its own upkeep, which
is exactly the window the gate kept silent.

### RED on base

* **Census of own-turn windows, wave-74 corpus** (crack-back line present /
  absent): Main 1 **159 / 473**, Main 2 **20 / 116**, and **Upkeep 0 / 384**,
  **Draw 0 / 54**, **End 0 / 43**, **Blockers 0 / 29**, Untap 0 / 6, Combat
  begins 0 / 2, Combat damage 0 / 3, Combat ends 0 / 1.
* **Two PARSETEST cases on the seeded tree FAILED against the new rule and are
  the base's encoded behaviour**: `#W59-J K8 NEGATIVE the seat's other windows
  stay silent` and `#W63-AC E9 MUST-NOT-MATCH no clause where no CRACK-BACK line
  is printed`. Measured: base **5,786 passed / 2 failed**. Both are rewritten to
  the rule that supersedes them, with the old assertion preserved as a comment.

### Fix

`crackBackNextTurnDue` drops the phase whitelist: on the seat's OWN turn every
window is due (instants, activations and the mana that pays for them are live in
upkeep, draw, combat and the end step alike). On the **opponent's** turn it is
still silent on every phase — D9's "a line on every window is a line nobody
reads" is kept where it earns its keep, because the crack-back it prices is a
whole turn cycle away and the seat's own untap comes first.

### GREEN

+2 rewritten cases and their MUST-NOT-MATCH: every own-turn phase (untap,
upkeep, draw, blockers, end, cleanup) is due; every opponent-turn phase is not;
a creatureless or powerless board still claims nothing.

### Prediction

`CRACK-BACK NEXT TURN` renders on own-turn Upkeep and Draw windows wherever an
able attacker exists (wave 74: 0 of 384 and 0 of 54), and the deck125-vs-123
turn-23 shape — six upkeep windows at 37 life against a lethal board with no
line — does not recur. Falsifier: the line appearing on an OPPONENT-turn window,
or an own-turn window where `crackBackTotalOver` is 0.

---

## P10 MED — the own-clock tag promised turns to a seat with a lethal on the stack

### Repro (`125v162` deck125 seqs 169-175, turn 27 Draw)

seq 172 (life 3), seq 173 (life 2) and seq 175 (life 2) each carry

```
ON THE STACK: 4 damage to you - you would be at -1; that would KILL you
```

and, on the decline row of the same prompt,

```
3. Cast nothing right now {the clock you already control: your Staff of Nin deal 1 damage a turn between them - at that rate alone the opponent reaches 0 in 18 more turns, with no card spent}
```

### RED on base

* **325 own-clock renders** in the corpus; **3** of them sit in a prompt that
  states a stack lethal on its own `ON THE STACK:` header (the three above);
  **7** are at 5 life or less; **93** are under a live loop paragraph. F7's gate
  reads `mIncomingCombatTurn` only, so none of the three was suppressed.
* PARSETEST `#W75-CL P10 REPRO/GREEN` asserts the two repro shapes
  (`(life 3, stack 4)` and `(life 2, stack 3)`) suppress — both are false on base
  by construction, because the base gate has no `stackDamage` parameter.

### Fix

`w75OwnClockSuppressed(myLife, incomingDamage, stackDamage, oppLoopLive)` (pure,
lifted out of `ownClockTagFor` so every branch is walkable). It takes the
**worst** stated incoming loss — combat declaration or the pending stack, via
`pendingStackLifeLossToSeat` at both call sites (priority row 0 and the cast
decline row) — and, under a **proven opponent loop**, treats a stated loss of ANY
size as the lethal, which is the prompt's own LOOP COMPLETE rule verbatim ("ANY
nonzero payment on a tag above is fatal, not merely expensive").

**The loop alone suppresses nothing.** An earlier draft of this lane suppressed
the sentence on `oppLoopLive` outright, which would have deleted the clock from
**93** renders including the deck125-vs-126 game where the seat genuinely held a
7-turn clock at 42 life. The trust doctrine forbids deleting a true token
(silent omissions are worse than wrong text), so the loop arm requires a stated
loss.

### GREEN

+8 cases, including the two repros, the "worst of the two, whichever surface
states it" pin, and two MUST-NOT-MATCH: a SURVIVABLE stack (5 life, 4 damage)
keeps the clock, and an unknown life total (`-1`) claims nothing.
**Suppression census under the shipped rule: 3 of 325 renders, all three
repros; 322 kept.**

### Prediction

`reaches 0 in N more turns` renders 0 times in a prompt whose `ON THE STACK:`
header says `KILL`, and 0 times under a live loop paragraph that also states a
nonzero loss landing on the seat. Falsifier: a seat losing a game it could have
answered because a TRUE clock was deleted — the 322-kept figure is what that
would move.

---

## P11 MED — the keep-X clause named the row's own card

### Repro (engine-seat MED-3)

`Cast Sphinx's Revelation ... {... Sphinx's Revelation {u}{u}{w}{x} in your hand
needs 3: no X on this row leaves it payable ...}` — the keep candidate is a
SECOND COPY in hand, and the clause read as a contradiction with its own row.

### RED on base

Census over the corpus's cast rows carrying the keep-X clause: **297** clauses,
**10** of them naming the row's own card by name. (The engine-seat figure was
27 of 238 over a differently-scoped denominator; this lane's regex counts only
rows where both names are extractable from one prompt, so 10 is a floor.) The
scan already excludes the row's own INSTANCE (`hc == card`), so the arithmetic
was right and only the NAME was ambiguous.

### Fix

`xCastRemainderScopeTag` gains `keepIsAnotherCopy`; the caller sets it when
`keep->getDisplayName() == card->getDisplayName()`, and the clause reads
`another copy of Sphinx's Revelation {u}{u}{w}{x} in your hand needs 3: ...`.
The keep candidate is NOT changed — a second copy is a real thing an X strands.

### GREEN

+3 cases: the repro shape, a MUST-NOT-MATCH that a different card is named byte
for byte as before, and a pin that the default argument reproduces the shipped
string.

### Prediction

0 keep-X clauses name the row's own card without `another copy of`. Falsifier:
a reasoning body that still reads the clause as self-contradictory.

---

## P15 MED — the range paragraph decoded only the descending X ladder

### Repro (`125v162` deck125 seq 148)

The row: `2-8. X = 2 up to X = 8 - one option per X in that range, **smallest X
first** ...`. The paragraph, ten lines below it: `A row reading "X = 12 down to X
= 2" is one option per X in that range, **largest X first**: its first number is
X = 12, the next X = 11, and so on to X = 2.` `#W74-CC` (O19) made the ladder
ascending and this decode was not moved with it.

### RED on base

**91** prompts carry a monotone-X range row; **14** of them pair an ASCENDING row
with the descending-only paragraph. (The known-bugs figure, 14 of 28 X menus, is
the same 14 counted per menu rather than per prompt.)

### Fix

`kOptionRangeNote` states both walks and says which words select which: the
wave-56 descending sentence is byte-identical, and the ascending twin is added
beside it with an explicit `"down to" counts down and "up to" counts up`.
Constant text, no board number, so it cannot move any key.

### GREEN

+3 cases: both sentences present in the note, and both row wordings still
reachable from `monotoneXRangeRow` so neither half of the decode is dead text.

### Prediction

0 prompts pair an `up to X =` row with a paragraph that only decodes `down to`.
Falsifier: an `echo_index_conflict` or an off-by-N X answer on an ascending
ladder — which is the population P23's residue item (b) also watches.

---

## P16 MED — the repeat block split into four rows, and its short name was a fragment

### Repro (`123v125` deck123 seq 186, turn 22 Main 2, 81 options)

The reviewer read this as "O18 not applied at the Equip seam". **It is applied**
— verified on the corpus: the BASE rows collapse correctly
(`2-40. Equip with Lightning Greaves targeting Human #1-#39 ... x39`), and so do
seqs 222 and 242. What actually goes wrong is one row lower down:

```
42. Equip with Lightning Greaves targeting Human #1 (Lightning G, repeated then stop [...]
43-50. the same "repeated then stop" shortcut, one row for each of the remaining targets ...
51. Equip with Lightning Greaves targeting Human #10 (Lightning , repeated then stop [...]
52-80. the same "repeated then stop" shortcut, one row for each of the remaining targets ...
```

FOUR rows where two would do. `repeatShortName` cuts the name at exactly 60
bytes; `repeatRowFoldKey` erases digits and compares; `Human #1 (Lightning G`
erases to `Human # (Lightning G` and `Human #10 (Lightning ` erases to
`Human # (Lightning ` — one trailing byte apart, so the run splits at the
#9 → #10 boundary and would split again at #99 → #100. The same cut is why the
name the footer tells the model to COPY ends in a dangling `(Lightning G`, a
name no answer can be built from (P14's shape, from the other direction).

### RED on base

* Corpus: **1** prompt carries a repeat-run range line and **1** of those (the
  same one) is split into more than one run — the population is small because
  only one game reached a 40-token board, and it is the game the reviewer read.
* PARSETEST `#W75-CL P16 RED-ON-BASE`: the two SHIPPED 60-byte names
  (`... Human #1 (Lightning G` and `... Human #10 (Lightning `) produce
  **different fold keys** — a standing assertion of the split.

### Fix

The 60-byte cut backs off to a word boundary, and never leaves an unclosed
parenthetical: if the truncated text has a `" ("` with no `)` after it, the cut
moves to that `" ("`. The cut point becomes a property of the WORDS, not of how
many digits the ordinal has. Nothing is removed: every repeat row is still in
`shown`/`shownLines`, still numbered, still separately answerable, and the option
set still reaches every target.

### GREEN

+6 cases: `#1`, `#10` and `#100` all yield whole names; no unclosed parenthesis
survives; the three fold keys are equal (so the block is ONE run); the shipped
fragments are shown to be two keys; a short name under the cut is untouched; and
a DIFFERENT target (`Bloodline Keeper`) is still its own run — the fold erases
digits, never names.

### Prediction

A menu whose repeat block covers ordinals spanning a digit-count boundary prints
exactly ONE full row plus ONE range line (seq 186's shape becomes `42.` +
`43-80.`), and no printed short name ends inside a parenthesis. Falsifier: a
`repeat_count_missing` or a named-row re-ask quoting a truncated equip name.

---

## P17 MED (measure only) — `plan_names_stranded_card` was blind to a card in no castable zone

### Repro (`125v152` deck125 seqs 28-29, turn 26)

seq 28's reply: `PLAN: Cast Sphinx's Revelation for X=5 to stabilize and draw
cards.` The same prompt: `Your hand (4 cards): Cancel; Plains; Island; Tundra`
and `Your graveyard (6 cards): ... Sphinx's Revelation {u}{u}{w}{x}`. seq 29
carries the plan forward and writes it again. No verdict, no counter, no record —
the existing measure needs the TAKEN row to carry a reserve verdict naming the
card, which a land-drop row never does.

### Fix (instrument only, nothing rendered, refused or re-asked)

`w75PlanNamesUncastableZoneCard(plan, game)`: a card in the seat's own GRAVEYARD
or EXILE (both PUBLIC zones — the measure reads only what the seat could read)
whose name the plan names, while NO card of that name is in hand or on the
battlefield. Same name test and same four-character floor as
`planNamesStrandedCard`. New record field `plan_names_uncastable_zone_card` and
gameend counter of the same name, kept SEPARATE from
`plan_names_stranded_card` so the two are never summed by accident, and computed
only when the existing measure did not fire.

### GREEN

+3 cases on the shared name predicate (the repro plan/name pair matches; a plan
that does not name it does not; the four-character floor is unchanged).

### Prediction

The next corpus reports a nonzero `plan_names_uncastable_zone_card` and the
`125v152` seq-28/29 shape appears in it. **Deliberately not claimed**: whether
the named card is castable FROM that zone — flashback, retrace and
cast-from-exile are real, so this counter is an UPPER bound and the reviewer must
sample it before treating it as a defect count.

---

## P23 LOW (d) — `off_protocol_bytes` and `plan_line_missing` are one event

### RED on base

Corpus cross-tab over all 2,526 records:

| `plan_line_missing` | `off_protocol_bytes > 0` | records |
|---|---|---|
| true | true | **96** |
| true | false | 10 |
| false | true | 2 |
| false | false | 2,288 |

96 of the 108 deviating replies are the SAME event — the reply wrote its plan as
an unlabelled sentence, so the label is missing AND the sentence is bytes outside
the two permitted lines. A review that reports the two fields separately
double-counts that one defect and cannot tell it from the two other shapes that
live in the same fields.

### Fix (instrument only)

`w75ProtocolDeviationClass(planLineMissing, offProtocolBytes)` (pure) →
`compliant` / `unlabelled_plan` / `plan_absent` / `prose_outside_two_lines`, on
every record as `protocol_deviation`, plus ONE gameend counter
`protocol_deviation_replies`. Both underlying measures are KEPT: they are what
the class is computed from, and deleting a measure to add one is how instruments
lose their audit trail.

### GREEN

+4 cases, one per class.

### Prediction

`protocol_deviation_replies` on the next corpus equals the count of records whose
`protocol_deviation` is not `compliant`, and the `unlabelled_plan` class is the
large majority of them (wave 74 would have read 96 / 10 / 2). Falsifier: a class
that never occurs, which would mean the three shapes are not the partition this
lane claims.

### P23 residue NOT closed by this lane

* the 2 `echo_index_conflict` outside the X family (`146` seq 49, `126` seq 66) —
  both name-rescued, no mechanism found in this lane's reading;
* the X ladder's draw-punisher economics printing AFTER the cast is committed
  (`125v162` seqs 147 → 148) — this is an ORDER-of-windows defect, not a render
  string, and belongs with the seam that issues the two windows;
* the hold row taken on menus printing a live activated row (deck126 LOW-4) —
  `#W72-BV` M6 already renders the activation-aware hold sentence; the third-wave
  recurrence needs a corpus count of hold-takes over `activationLive` menus,
  which is lane CI's instrument, not this one;
* the 31 replies carrying an answer and no plan are owner-ruled report-only and
  are now the `plan_absent` class.

---

## Weakest evidence

* **Nothing here was validated against a live model.** Every claim is a
  PARSETEST assertion over corpus bytes plus a census. The P5 override in
  particular changes what the model READS, and whether a model that is told
  `you would be at 0 - this row feeds their chain` actually declines the row is
  the next corpus's question, not this lane's.
* **P5's scope is `{right now: ...}` only.** A row that pays life in a
  `[NAMED BY THEIR ...]`, `[DRAW PRICE: ...]` or `{paying this costs you N life}`
  bracket still prints its own subtraction under a live loop. Those brackets were
  left alone deliberately (P5's text names the `{right now:}` clause), but they
  are the same lie in a different group and the residue is real. The corpus
  census found 16 rows in the `{right now:}` population and did not census the
  others.
* **P5's direction test is textual, not semantic.** It asserts that every
  `you would be at N` inside a `{right now:}` group is a `life - <positive
  amount>` subtraction. That is true of all five emitters read in this lane
  (`damagePlayerVerdict`, `drawPriceRowTag`, `castDrawPriceRowTag` and the two
  X-price paths), but a future emitter that prints a GAIN in that phrasing would
  be mis-rewritten. There is no assertion in the tree that forbids it.
* **P10's suppression census (3 of 325) is measured with a regex on the
  `ON THE STACK: N damage to you` header**, not with the engine's own
  `pendingStackLifeLossToSeat`. The engine walk may find pending losses the
  header does not print, so the true suppression count is ≥ 3 — bounded above by
  the 93 renders under a live loop, and I have not measured how many of those
  carry a nonzero stated loss.
* **P16's live population is ONE prompt.** The fold split is proven by
  construction (the two shipped fragments key differently) and by that single
  seq-186 render; no second game in the corpus reached a board big enough to show
  it. The #99 → #100 boundary is pinned but has never occurred.
* **P17's counter is an upper bound** (castability from graveyard/exile is not
  checked) and its PARSETEST coverage is the shared NAME predicate only — the
  zone walk itself has no fixture, because it needs a game.
* **P11's corpus figure (10 of 297) is lower than the engine-seat's (27 of 238)**
  because the two denominators are built differently; I did not reconcile them,
  and the fix is keyed on the engine's own name comparison rather than on either
  count.
* **P6 grows the prompt.** The crack-back line now renders on up to 384 more
  own-turn windows per corpus, at roughly 200-400 bytes each, and it carries
  board-derived numbers into the BOARD SNAPSHOT (hence the ask key, though not
  the hold latch, which reads rows only). That was already true of the main-phase
  windows it rendered on; the extra windows widen the exposure and no lane has
  measured the resulting re-ask rate.
* **The suite cannot exercise any of this.** All eight items live on the model-
  facing render, and the test suite has no endpoint, so the 1285/76 green says
  only that nothing else broke.
