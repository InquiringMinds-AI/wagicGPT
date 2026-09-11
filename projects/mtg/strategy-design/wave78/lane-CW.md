# Wave-78 lane CW — render truth and pricing (S2, S5, S8, S12, S13, S14, S15)

Worktree `worktrees/lanes/w78-CW`, branch `w78-lane-CW`, base `1e465fff7`.
Gates: **PARSETEST 6402 -> 6473, 0 failed**; **suite THREADS=1 1291 tests (0 failed) + 76 AI
tests (0 failed)**; both link guards OK; `git diff | grep -c U+FFFD` = **0**. One file changed
(`projects/mtg/src/AIPlayerGPT.cpp`), commit `6028c3a96`.

**RED-on-base method.** The wave-77 source (`git show HEAD:.../AIPlayerGPT.cpp`) was staged into
the lane tree with a RED block appended to the PARSETEST corpus asserting the wave-78
expectations against the wave-77 code, built, and run through the same `WAGIC_GPT_PARSETEST=1`
caller: **6402 passed, 8 failed** — S8 x4, S12, S13, S14(b), S15. Those eight are GREEN in the
lane tree. S2's RED is a pin, not a build (`#W78-CW S2 RED the wave-77 pairing (repeat row +
'card name only') DISAGREES`); S5, S14(a) and S14(c) name surfaces that did not exist on base,
so their RED is definitional and is stated as such below.

---

## S2 (HIGH) — the repeat row's instruction contradicted the window's closing sentence

**Repro.** `123v126` deck123 **seq 38**, the corpus's only `repeat_count_reask`: the row's
bracket demands `CHOICE: 2 (<short name> x<N>)` and refuses the bare name, while the sentence
that closes the SAME prompt says the short name is "the action and card name only - copy
nothing from the {...} annotations". 339 s of decode, `reasoning_ngram_repeat` 0.626, reasoning
budget hit. This is wave-74 O17's defect one surface over (two instructions pointing opposite
ways on one row family).

**RED evidence.** `#W78-CW S2 RED the wave-77 pairing (repeat row + 'card name only')
DISAGREES` — `w78RepeatSurfacesAgree(rowsWithRepeat, w78ShortNameParenthetical(false))` is
false, which is exactly the pairing wave 77 shipped.

**Fix.** ONE instruction. `w78ShortNameParenthetical(hasRepeatRow)` is the closing sentence's
parenthetical; on a window whose rows carry `", repeated then stop"` it carves the row out by
name and quotes the row's own literal form once (`"<short name> x<N>"`). Read off `shownLines`
at the render site, so the two surfaces are computed from the same list the model is looking
at. The golden protocol text (`reply-protocol.txt` / `kReplyProtocol`) is **untouched** —
invariant 000 — and a window with no repeat row is byte-identical to wave 77 (pinned).

**GREEN.** 6 pins: recognition; the RED pairing; both agreeing pairings; the MUST-NOT-MATCH
(a menu with no repeat row never licenses a count); the byte-identical ordinary sentence; and
the echo shape — `CHOICE: 2 (Create human with Thraben Doomsayer x3)` still resolves to row 2
with count 3, and the `x<N>` placeholder still names no count (the wave-50 Z rule).

**Prediction (falsifiable).** Next corpus: **zero `repeat_count_reask` events**, and on every
window that renders a repeat row the closing sentence contains `<short name> x<N>`.
**Negative clause, own pin:** on every window with no repeat row the closing parenthetical is
the wave-77 string byte for byte — `#W78-CW S2 MUST-NOT-MATCH the ordinary sentence is
unchanged, byte for byte`.

---

## S5 (HIGH) — a counter row offered a spell the seat's own counter already targets

**Repro.** `125v146` deck125 **seq 125**: both counterspell rows print `can target on the stack:
Silverquill Silencer` while the seat's own Essence Scatter sits above it on the stack aimed at
that spell. The seat burned its second Scatter at 8 life, took 7, lost.

**RED evidence.** The clause did not exist on base (no surface said it). The join it performs is
over engine state the model cannot do: the stack line names objects, the row names what it can
hit, and nothing crossed the two.

**Fix.** In the counter row's stack-hit loop, scan the live `ActionStack` for an unresolved
`ACTION_SPELL` of THIS seat's, not this row's own card, whose script contains `fizzle` and whose
target list holds the candidate object; `w78RedundantCounterTag(name)` then rides the hit. The
row is still offered and still answerable — a second counter is right when theirs resolves first
or is itself countered, and the clause says so.

**GREEN.** Positive (exact bytes), negative (no pending counter -> ""), the key-stability set —
`holdActionKeyRow`, `optionSetKeyLine`, `w77KeyTailOf` and `asyncSlotKeyOf` all unmoved — and
the narration strip (`stripNarrationDecoration` drops it, so it cannot enter history). The tag
is a `{...}` annotation group **deliberately**: `w76StripBalancedAnnotationGroups` strips `{}`
and not `[]`, so a bracketed form would have entered the ask key.

**Prediction.** Next corpus: **every counter row whose named stack target is already targeted by
one of that seat's own unresolved counters carries the clause** (wave-77 rate: 0), and
**no seat casts a second counter onto a spell its own counter already targets** while that first
counter is unresolved.

---

## S8 (MED) — the gloss cut mid-clause into a false statement

**Repro + population.** Over the 21-game corpus, **761 of 10,759 glossed bodies (7.1%) end
mid-clause** (39 distinct shapes). The three the docket names: Howling Mine
`"...if Howling Mine is untapped, that player..."` **55 renders**; Brutal Cathar
`"exile target creature an..."` / `"...until this..."` **50 + 48 renders** (the cut drops the
"until this creature leaves the battlefield" return clause and turns a temporary exile into a
permanent one); Lightning Greaves `"...stays on the battlefield if..."` **45 renders**.

**RED evidence (base build).** All four S8 CHECKs fail on the wave-77 source.

**Fix.** `textSnippetCore` prefers a **clause** boundary: the largest position at or below the
budget that ends a clause (` -- `, or a full stop followed by a capital or an opening
parenthesis), and marks the omission ` (...more)`. A text with **no interior boundary that ends
at one** is a single clause end to end and prints whole inside a bounded stretch
(`kClauseCompleteStretch` = 80 B, and only on tiers at least that wide — a narrow per-clause
budget cannot pay for a stretch it never measured). The stretch is **withheld** from
`boardEffectSnippet`'s protected last clause and its hard-cap re-truncation, because that is the
budget the wave-47 R6 guarantee is shared around (pinned: `#W47-R6 every earlier loyalty ability
is still on the line` stays green). Past the stretch the wave-48 D5 word-boundary rule stands
unchanged (pinned).

**GREEN through the live caller path (`WAGIC_GPT_RENDERPROBE`, GPT seats, dead endpoint at
`127.0.0.1:9`, decks 152 vs 126):**

| | base `1e465fff7` | lane |
|---|---|---|
| glossed bodies rendered | 272 | 414 |
| **mid-clause cuts** | **20** | **0** |
| marked omissions `(...more)` | 0 | 12 |

Different deals (fair hands), so the counts are not paired — the categorical 20 -> 0 is the
load-bearing half. Byte cost is bounded analytically at **<= 90 B per glossed body**
(80 stretch + 10 marker); the probe's max section grew 32,138 -> 37,732 B across four renders
per section on a longer game.

**Prediction.** Next corpus: **zero glossed bodies end in a mid-clause `...`** (wave 77: 761 of
10,759), and **every Brutal Cathar board gloss contains "leaves the battlefield"**.
**Negative clause, own pin:** a text with no clause end inside the stretch still takes the
wave-48 word cut and never gains a `(...more)` — `#W78-CW S8 MUST-NOT-MATCH`.

---

## S12 (MED) — `Opponent life trend:` netted the seat's own damage against their gain

**Repro.** deck130 MED-1: a deck gaining 7 a turn while taking 12 prints `-5`, and every "are
they gaining" rule in deck130's guide keys on that flickering figure.

**RED evidence (base build).** `#W78-CW S12 RED the life trend names the gain and the loss`
fails on wave-77 code.

**Fix.** The net stays (it is the figure that says who is winning the race); the two halves that
make it print beside it — the sum of the rises and the sum of the falls between the samples
already on the line. Arithmetic over numbers the line already prints, so the three figures
cannot disagree. A trend that only ever moved one way prints no split.

**GREEN.** `turn 4: 20, turn 5: 27, turn 6: 15, now 22 (+2 since turn 4; they GAINED +14 and
LOST -12 across those turns - the figure before this is the two netted).` Three MUST-NOT-MATCH
pins keep the monotone-rise, monotone-fall and flat faces **byte-identical to wave 77**.

**Prediction.** Next corpus: **every trend line whose samples move in both directions carries
both halves**, and no line carrying a split has `gained == 0` or `taken == 0`.

---

## S13 (MED) — a shared card's text printed once per row

**Repro.** `146v162` deck146 **seq 27**, a 25,218 B prompt: fifteen rows all act with Kaya the
Inexorable and each repeats her whole 450 B `{card text: "..."}` — **6,750 B of one prompt is
one paragraph**. Corpus-wide the shape fires on **138 windows**.

**RED evidence (base build).** `#W78-CW S13 RED the shared card text is printed ONCE, not once
per row` fails through `joinNumberedRows`.

**Fix.** `w78HoistSharedCardText` runs inside `joinNumberedRows` (the one function all four
option seams render their lists with): a `{card text: "..."}` body of at least 100 B that
appears on 2+ rows is removed from those rows and printed ONCE above the list, naming the option
numbers it belongs to (`Card text shared by options 1-4 ...`) — a referent the reader can see,
per the wave-75 lesson. Every row keeps its number, label, cost, target and every other
annotation; the blob was already stripped from every key, so no key moves (pinned).

**Byte measurements.**

| prompt | before | after |
|---|---|---|
| `146v162` deck146 seq 27 (the repro) | 25,218 B | **18,823 B** (-6,395, -25.4%) |
| **deck130 max prompt** (`130v125` seq 173, the brief's target) | 30,125 B | **30,125 B (0)** |
| whole corpus (2,872 prompts) | 41,648,166 B | **41,576,134 B** (-72,032 over 138 windows) |

The deck130 max prompt is measured and **unchanged**: its five rows carry three DIFFERENT card
texts, and 21,918 of its 30,125 B is narration, not option text. S13's cause as docketed
("repeated `{card text:}` per row on shared-source target menus") is real and is the deck146
shape; it is **not** what makes the deck130 prompt the corpus's second largest. Said plainly
because the docket attributes that prompt to this cause.

**Prediction.** Next corpus: **no prompt contains the same `{card text: "..."}` body twice in
its option block**, and every window that would have contains exactly one `Card text shared by
option...` line. **Negative clause, own pins:** a single row, two different texts, or a body
under 100 B hoist nothing.

---

## S14 (MED) — pricing and wording, three items

### (a) the keep-X clause named the cheapest CARD, never the cheapest COUNTERSPELL

**Repro.** `162` deck125 **seq 180**: X=9, tapped out, opponent resolved four permanents the
following turn with Essence Scatters in hand. The wave-74 O8 clause answers "what is the largest
X that leaves the cheapest CARD payable" — on a control seat that is routinely a sorcery-speed
body it never meant to hold.

**RED evidence.** The figure did not exist on base (definitional).

**Fix.** A second scan, same counting rule as O8's, for the cheapest castable **instant** in
hand; `xCastRemainderScopeTag` prints `Holding up an instant: <name> <cost> needs N: the largest
X that still leaves it castable this turn is X=K`, in the same one-currency arithmetic wave-74
CF fixed. Printed only when the instant is a DIFFERENT card from the keep candidate — otherwise
the clause above has already said it and saying it twice reads as two holds.

**GREEN.** The O8 figure is unchanged (`X=8`) beside the new `X=11`; MUST-NOT-MATCH for "no
instant in hand" and for "the cheapest card already is the instant"; and the **negative clause
has its own pin** (`no X on this row leaves it castable this turn, not even X=0`).

### (b) `[second copy:` priced a waste in the vocabulary of a payoff

**Repro.** `125` deck126 **seq 51**: the row read `...this window's cast spent DOUBLING an
effect you already have...` and the model's plan came back as *"Cast Exquisite Blood to double
the life gain trigger, maximizing future Sanguine Bond damage"* — on a board that already held
one. The wave-74 O21 clause was the COST half and was read as an upside.

**RED evidence (base build).** `#W78-CW S14 RED the second-copy price carries no capitalised
verb of increase` fails on wave-77 code.

**Fix.** Same two halves, emphasis neutralised: the price leads, no capitalised verb of
increase. The **stacking fact is unchanged and still stated** — verdict 0 still says a second
instance repeats the lines, which is TRUE of Sanguine Bond and Staff of Nin, and turning it into
"changes nothing" would have been the wave-69 K8 defect restored.

**GREEN.** The wave-52 head stays byte-identical; the dead and partial verdicts are untouched;
a new MUST-NOT-MATCH pins the absence of `DOUBLING`/`doubl`.

### (c) loyalty rows printed `[cost: Counters]` with no figure

**Repro.** deck146: `CounterCost`'s render string is the bare word "Counters" for every counter
cost in the engine, so every planeswalker row in the corpus reads `[cost: Counters]` and the two
numbers a loyalty decision turns on — what this row spends, and what the walker has to spend it
from — appear on no surface at all.

**RED evidence.** The figure did not exist on base (definitional).

**Fix.** `w78CounterCostBody` reads the cost's own `Counter` and the source's own counter pile:
`{counter cost: spends 3 loyalty counters - Kaya the Inexorable has 5 now, leaving 2}`. It rides
its **own annotation group and NOT the `[cost: ...]` body** — `w75CostGroupsKey` puts cost groups
back into the hold key byte for byte, so a loyalty total inside `[cost:]` would have made every
row of the walker read as NEW on every activation.

**GREEN.** Minus and plus faces; MUST-NOT-MATCH for an unreadable pile (claims no total), a
zero cost and a nameless counter; the key-stability set (two loyalty totals -> one hold key, one
option-set key, one ask tail, one async slot key); and the narration strip.

**Prediction for S14.** Next corpus: **every `{X pricing:}` block on a seat holding a castable
instant carries the `Holding up an instant:` figure**; **no `[second copy:` render contains
"DOUBLING"**; **every loyalty row carries a `{counter cost: ...}` group naming the walker's
current total** (wave-77 rate: 0), and **no hold-check bracket counts a walker row as new
because its loyalty moved**.

---

## S15 (LOW) — a 21-row menu whose rows 2-20 are annotated no-ops

**Repro.** `152v123` deck152 **seq 7** (the docket's "`123` seq 7"): an Intrepid Adversary
repeat-pay menu, rows 2-20 each reading `add N counter(s) {same effect right now: adds 0
counters}`. Corpus-wide: **4 windows, ~3,964 B**.

**RED evidence (base build).** `#W78-CW S15 RED the 21-row no-op band folds to fewer than 21
lines` fails on wave-77 code.

**Fix.** A fourth `form` in `joinNumberedRows`, tried LAST so none of the three existing forms
changes: a row with a count in its label plus the band's own `{same effect right now: adds ...}`
clause, climbing by one with digit-erased text identical (the plural normalised out so `add 1
counter` joins its neighbours). A run at or above `kMonotoneXCollapseFloor` prints as ONE
labelled range naming both ends verbatim, the count, and that **every number in that range is
still on this list and any of them is a legal answer**. This is DISPLAY only — `shown`, the
option vector, `req.optionTexts` and the answer INDEX are untouched, which is also why the
wave-72 codex finding 6 hazard (a display permutation applied twice, once to the model's answer
and once to the heuristic's) cannot arise here: nothing is permuted.

**GREEN.** The 21-row menu prints fewer than 21 lines with `2-20. add 1 counter ...`; the rows
either side keep their own numbers and text; the **answer -> engine index mapping is pinned**
positively (`CHOICE: 7 (add 6 counters)` -> row 7, engine index 6), at both edges
(`CHOICE: 1`, `CHOICE: 21`), and negatively (a LIVE rung is never folded; a row with no count in
its label is not a rung; a band under the floor prints in full).

**Prediction.** Next corpus: **no prompt prints four or more consecutive `{same effect right
now: adds ...}` rows**, and every folded window's `chosen_text` still names a single rung.
**Negative clause, own pin:** a rung whose `{right now:}` verdict is LIVE is never folded.

---

## Weakest evidence

1. **The S8 stretch is a judgment call with a measured, not derived, bound.** 80 bytes is chosen
   because it completes the three named cards' clauses and is narrower than every whole-text
   tier; it is not read off a distribution of clause lengths. A card whose first clause runs
   more than 80 B past its tier still takes the word cut — I did not enumerate how many such
   cards the pool holds, only that the corpus's 761 mid-clause cuts fall to 0 in the live probe.
2. **The two render-probe runs are different games.** Fair hands are dealt per run and the
   corpus is not replayable from its prompts, so base's 20 mid-clause cuts and the lane's 0 are
   not a paired measurement; 414 vs 272 glossed bodies is mostly a longer game. The categorical
   claim (0) is safe; the byte delta is not a clean before/after.
3. **The deck130 max-prompt measurement is a NULL result and the docket expected otherwise.**
   S13 saves 0 B on the prompt the brief named and 6,395 B on the prompt where the shape
   actually lives. If the wave-78 reviewer wants the deck130 prompt smaller, the lever is its
   21,918 B of narration, not this item.
4. **S5's join was not observed firing in a live game.** No corpus game replays, and the probe
   run's seats never had an unresolved own counter aimed at a stack object a second counter
   could also hit. The scan is pinned pure and the engine reads are the same ones the stack line
   already makes, but "it renders when the shape occurs" rests on the code, not on a sighting.
5. **S14(a) uses `TYPE_INSTANT` as the proxy for "an answer you hold mana up for".** A flash
   creature or an instant-speed activation is not counted, and a cheap instant that is not a
   counter (a combat trick) is. The docket asked for "the cheapest COUNTERSPELL"; instant is the
   honest engine-readable generalisation and is stated as such on the row ("Holding up an
   instant"), not as a claim about counters.
6. **The first full-suite run exited cleanly after 1,304 of 1,367 tests with no summary line.**
   Exit 0, 92 MB peak (nowhere near the 4 G cap), no crash marker. The two tests that follow the
   stop point pass solo, and a clean rerun produced the exact baseline (1291/0 + 76 AI/0). I
   could not reproduce the truncation and do not have an explanation for it; the gate reported
   above is the rerun.
7. **S14(c) reads `counter->name` verbatim, lowercased.** Primitives spell it both `Loyalty` and
   `loyalty`; both render as "loyalty counters". A non-loyalty counter cost (a Ghostform-style
   removal cost) gets the same clause, which is correct but untested against a real such card.
