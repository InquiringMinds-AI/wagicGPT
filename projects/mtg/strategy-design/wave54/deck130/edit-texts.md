# deck130 (Budde's Ponza) — wave-54 edits, before -> after

Baseline: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (61,789 bytes,
wave-53 reviewer revision + boundary pass). Revised file: **64,849 bytes (+3,060, +5.0 %)**, well
inside the pool band. Five edits, each cited. Corpus:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/`, six deck130 seat logs, 338 decisions.

---

## D54-0 — MANDATORY RE-KEY (B1 REVERSE audit): the HOLD row's literal changed under lane A
**Finding.** The DECIDING SITUATIONS entry quoted `"Hold priority - do not ask me again this turn
unless the board changes"`. **0 renders** in this corpus. The emitted row is `"Pass priority, and
do not ask me again this turn unless the board changes (any change re-opens this window; you give
up no cast)"` — 11 renders at this seat, 286 across my twelve logs. deck130 had only 9
opponent-turn casting asks all corpus (wave 53: 72), so the rung barely fired, but the sentence
was false wherever it did.

BEFORE:
```
- It is the OPPONENT'S turn, you have already answered "Cast nothing right now" to this exact
  list, and the last row reads "Hold priority - do not ask me again this turn unless the board
  changes" -> take THAT row.
```
AFTER: the emitted string, plus "at the FIRST such ask of the turn, not after four declines."

---

## D54-1 — the X menu when NO row reads `YOURS: none`, and the marker that never printed
**Finding, and the game it cost.** `130v152` s17, turn 10, **9 life against 25**, six Starstorm
rows:
```
1. X = 4 {X pricing: kills THEIRS: Intrepid Adversary, Elite Spellbinder, Sigarda, Champion of Light; YOURS: Siege-Gang Commander, Goblin x3}
2. X = 3 {X pricing: kills THEIRS: Intrepid Adversary, Elite Spellbinder; YOURS: Siege-Gang Commander, Goblin x3}
3. X = 2 {X pricing: same kills as X=3, for 1 less mana}
4. X = 1 {X pricing: kills THEIRS: none; YOURS: Goblin x3}
5. X = 0 {X pricing: X=0 - this cast does NOTHING ...}
```
Answered **4**, under `PLAN: Kill Sigarda and Elite Spellbinder with Starstorm X=1` — a sentence
the row it named denies in its own words. It killed its own three Goblins and nothing of theirs,
then attacked its lone Siege-Gang Commander into a `(your attacker dies, their blocker lives)`
Sigarda (s18) and lost at -3 to 29. The cast row one screen earlier (s15) had said
`smallest X that kills an OPPONENT creature: X=2`, and its own PLAN there said X=2.
**Why the guide did not stop it.** STEP 1's no-marker fallback reads "find the row with the
LARGEST `kills THEIRS` list whose `YOURS:` reads `none`". On this menu **no row reads
`YOURS: none`** — the rule has no branch, and the pilot fell through it.
**And the marker itself.** `[<- most kills at any affordable X that costs you nothing]` rendered
**0 times in the entire 42-log corpus** (grepped across every `.jsonl`). The guide treats it as
the primary path.

BEFORE: `With no marker printed: find the row with the LARGEST "kills THEIRS" list whose "YOURS:"
reads "none". That is your X. ...`
AFTER: the same paragraph, preceded by `THE MARKER IS OFTEN NOT THERE` (0 renders in a whole
corpus; do not read its absence as "no row is good"), and followed by a new branch:
`AND WHEN NO ROW READS "YOURS: none" AT ALL ... NEVER TAKE A ROW WHOSE "kills THEIRS" READS
"none"` — with the six rows above reproduced verbatim, the plan sentence that contradicted them,
the outcome, and the closing rule: among rows that DO name an opponent creature take the longest
THEIRS list; the YOURS list is the price, and a price is not a reason to pick the row that buys
nothing; if every such row names Rorix or Siege-Gang, the decision belongs at the cast row where
"Cast nothing right now" still exists.

---

## D54-2 — STEP 2 (collapsed runs): third corpus, third break
**Finding.** `130v126` s58: took `X = 5 {kills THEIRS: Overgrown Battlement x2, Perimeter Captain,
Wall of Omens; YOURS: Dwarven Blastminer}` with the row directly below reading
`X = 4 {X pricing: same kills as X=5, for 1 less mana}` — one mana thrown away. The kills were
right; the row was not the cheapest of its class.
BEFORE: `... and this deck did it twice.`
AFTER: `... has now done it three times - the last on a menu whose X=5 row it took with the row
directly below reading "X = 4 {X pricing: same kills as X=5, for 1 less mana}".`

---

## D54-3 — the empty opposing board is the hardest case of the 6-life face floor
**Finding.** `130v146` s52, turn 24, **17 life against 15**: sacrificed a Pyrite Spellbomb at the
opponent's face (15 -> 13) with their battlefield line showing **0 creatures**. Twelve turns later
it lost at -2 to 14 with four of their creatures alive, several inside the Spellbomb's 2 damage.
This is the only face-damage break at this seat this corpus; the `130v123` chain (s72/s73/s76/s77
at opponent 10/8/6/4, then s87 at 2) is the licensed lethal form and closed the game, so it is
NOT counted as a break.
BEFORE: the LETHAL-THIS-TURN exception ended at "... is lethal, not a face-burn mistake."
AFTER: adds `AND THE EMPTY OPPOSING BOARD IS THE HARDEST VERSION OF THE FLOOR, BECAUSE THERE IS
NOTHING ELSE TO AIM AT`, quoting the row's own `legal targets right now: the opponent, you`, the
17-against-15 seq, and the closing contrast: "Two damage against a healthy opponent is a card;
two damage against a 2-toughness creature is a card AND a body."

---

## D54-4 — a one-row ATTACK list after your own sweeper
**Finding.** `130v152` s18, immediately after D54-1's Starstorm killed its own three Goblins: the
attackers screen printed exactly one row, `A1. Siege-Gang Commander (2/2) ... [their untapped
blockers: Sigarda, Champion of Light (4/4) (your attacker dies, their blocker lives)]`, and the
pilot sent it. The ATTACK FLOOR's THIRD case ("with any other creature listed, send EVERY creature
listed") swallowed the single-body case; its "ATTACK: none only when every listed attacker would
die and kill nothing" clause was true here and was not reached.
AFTER: adds `A ONE-ROW "ATTACK:" LIST AFTER YOUR OWN STARSTORM IS THE CASE THIS FLOOR GETS WRONG`
— read the tag on the last body; one attacker whose own tag says it dies and kills nothing is
"ATTACK: none", however few rows the screen prints.

---

## Compressions that partly paid for the above
| where | why safe | bytes |
|---|---|---|
| #0 draw-punisher anecdote | 0 draws/cycles under the line, **fourth corpus** (12 renders at this seat, 0 takes) | 69 |
| punisher-targeting anecdote | rung held (`130v162` s25 aimed the Spellbomb at Ob Nixilis) | 19 |
| Blastminer attack anecdote | sent correctly into "(neither dies)" two corpora running | -1 |
| land-plan "in range" anecdote | the rung held: 22 LD casts, 2 against a 5+-land line (wave 53: 6 of 26) | 42 |

## NOT edited, and why
- **Land destruction at a 2-land opponent** (`130v146` s44 t20, s47 t22, s60 t32). It looks like a
  rung break and is not: the hand line at each of those seqs reads `Your hand (1 card)` and the
  card was the only legal cast. No rule can improve a topdeck.
- **The 1-land opening keep** (`130v123` s1, coverage line naming Spark Spray and Pyrite Spellbomb;
  won at 20 to 0). The MULLIGAN section licenses it explicitly — "this deck kept a one-land seven
  whose coverage line named exactly one card and won that game". No floor was broken; nothing for
  the owner.
- **The `would not cover any spell` mulligan branch**: **0 renders at this seat, fourth corpus**.
  #132 stays UNTESTABLE-AT-THIS-SEAT (skill #200). Text unchanged.
