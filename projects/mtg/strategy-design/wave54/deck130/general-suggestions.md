# deck130 — core-prompt / render proposals (wave 54)

Layer routing: text the engine EMITS, or the reply contract. Engine-behaviour defects are in the
seat file's engine list.

---

## G6 (HIGH, render) — the X menu must always mark exactly one row
**Why, and it lost a game.** `[<- most kills at any affordable X that costs you nothing]` rendered
**0 times in the whole 42-log corpus** (checked by grepping every `.jsonl`). The marker is printed
only when some affordable X kills something of theirs and nothing of yours — a condition that did
not hold on either X menu at this seat, nor anywhere else in the corpus. On the unmarked menu at
`130v152` s17 (9 life against 25) the pilot took `X = 1 {X pricing: kills THEIRS: none; YOURS:
Goblin x3}` while `X = 3` killed two of theirs, swept its own board, and lost the game two windows
later. A marker that never fires is not a marker.
**Ask.** Always mark ONE row, and say what the mark means:
- when some X costs nothing: the current marker, unchanged;
- otherwise: `[<- best trade: the most of THEIRS at the smallest cost to YOURS]` on the cheapest
  row whose `kills THEIRS` list is maximal.
**Falsifier.** X menus rendering with no marker: 2 of 2 this corpus -> 0. Takes of a
`kills THEIRS: none` row while another affordable row names an opponent creature: 1 -> 0.

## G7 (MED, render) — `same kills as X=N` hides the kill list from the row that should be taken
**Why.** The de-duplication is correct and it removes the decisive fact from the correct row. At
`130v152` s17 row 3 read `X = 2 {X pricing: same kills as X=3, for 1 less mana}` — the row the
guide's STEP 2 wants taken, and the only row on the menu with no kill list on it. At `130v126`
s58 the pilot took the TOP of a collapsed run (X=5) with `X = 4 {same kills as X=5, for 1 less
mana}` directly below.
**Ask.** Either restate the kill list on every row, or move the mark: put the
`[<- cheapest X with these kills]` marker on the BOTTOM row of each collapsed run, so the row that
should be answered is the row that carries the annotation.

## G8 (MED, render) — a single-target `{kills:}` list does not say whose permanents those are
**Why.** Stone Rain and Lay Waste rows print `{kills: Plains #1, Plains #2, Mountain #1, Mountain
#2, Mountain #3, Mountain #4, Mountain #5}` (`130v146` s23) — a list of all *legal targets*, of
which five are the pilot's own Mountains, under a label that reads as a consequence. The target
menu one screen later marks ownership on every row (`Plains #1 [land] [opponent's battlefield]`,
`Mountain #1 [land] [your battlefield] [tapped]`, `130v146` s24); the cast row does not. Lane C's
D4 fixed the same omission for player targets in damage summaries; this is the land-destruction
sibling.
**Ask.** For a single-target destroy row, either mark ownership inline —
`{kills whichever ONE you target: theirs - Plains #1, Plains #2; yours - Mountain #1-#5}` — or
name only the opponent's permanents and state the count of your own.

## G9 (MED, render) — ability rows that damage a player still print no life total
Carried from wave-53 L1 and still open. `130v162` s25 `Deal 2 damage with Pyrite Spellbomb
targeting Ob Nixilis, the Hate-Twisted` sat beside a menu whose player row printed
`the opponent (player, life 12)`, and the ability row itself printed no life arithmetic. The cast
rows have carried `- and 1 to the opponent at life 20 leaves them at 19` since lane C
(`130v126` s66); the ABILITY rows have not. Same clause, other emitter.

## G10 (LOW, render) — the DRAW PRICE tail shipped and is clean
8 `[DRAW PRICE:` rows at this seat, **8 of 8** carrying `- you would be at K` (wave 53: 0 of 58),
0 casts from a DRAW PRICE row, 0 `this KILLS you` renders (no window reached damage >= life).
Recorded as a PASS; no ask.
