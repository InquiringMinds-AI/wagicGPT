# deck162 - wave-55 edit texts (before -> after), each with its finding

Base = the LIVE guide `bin/Res/ai/baka/deck162_strategy.txt` (wave-54 reviewer revision 5160eb1cb;
byte-identical to `wave54/deck162/strategy.txt` - the boundary pass did not touch this deck).
Size 41,379 -> **45,365 bytes** (band 41-71 KB). Four edits. deck162 **5-1**, the best record in
the pool (W v130 15/-1 T10, v123 20/0 T13, v126 20/-6 T15, v125 20/0 T27, v152 6/0 T16;
L v146 -1/12 T20). 246 decision records at the six deck162 seats.

**The headline is a PASS.** Wave 54's 162-A lane - ten own-turn `Cast nothing right now` answers on
menus whose only live row printed **no** `{feeds:` clause - was re-keyed last wave from a category
("draw engines") to a row literal. This corpus: **3 such windows**, down from 10, in games the seat
won. The record went 2-4 -> 5-1. No rate is attributed to the edit (new-baseline rule); the WINDOW
COUNT is what moved and that is measured, not inferred.

---

## 162-A (CARRY, P7 FAIL 3 of N, was 10) - name the sentence that survives

**Finding.** Three residual windows, all with Liliana's Caress `{1}{b}` as the only live row and no
`{feeds:` clause on it: `162v152` **s4** (T4 Main 1, 2 untapped {B}), `162v152` **s5** (T4 Main 2,
same list, `[you declined this exact list 1 time already this turn]`), `162v130` **s6** (T5 Main 1,
3 untapped). All three replies carry the same plan sentence:
*"cast Underworld Dreams or Fate Unraveler as soon as possible"* - a punisher costing `{b}{b}{b}` or
`{3}{b}` that was **not a row on any of those menus**. The guide already forbids "keep mana open"
and the phrasing "cast Underworld Dreams first"; it does not name **"as soon as possible"**, and
that is the sentence that survived the re-key.
Classification: **STRATEGY**.

**Before** (end of the `{feeds:` brake block):
```
Six turns of mana bought nothing and the game ended 0 to 38. "Cast nothing right now" is the
answer to a menu of GIFTS, never to a menu of one cheap permanent that gives them nothing.
```
**After:** same two lines, then:
```
THE SENTENCE THAT KEEPS DOING THIS IS "as soon as possible". "Cast Underworld Dreams or Fate
Unraveler as soon as possible" is a plan for a LATER window; it is not a reason to decline the
row in front of you. A punisher you cannot pay for is not competing for these two mana - the
engine would have printed its row if it were. So when the only spell on the menu is a Liliana's
Caress, a Fog Bank or a Shield Sphere and your plan sentence names a punisher that is NOT one of
the numbered rows, cast the row and leave the sentence alone. WHAT THIS COST, three more windows
in this corpus after the six above: two-row asks whose only spell was "Cast Liliana's Caress
{1}{b}" answered "Cast nothing right now" on turns 4, 4 and 5, each under a plan reading "cast
Underworld Dreams or Fate Unraveler as soon as possible".
```

---

## 162-B (RULE STALENESS, not a pilot break) - the K-of-0 brake gets a release read off the hand line

**Finding.** `{feeds:` rows at deck162 seats: **195 rows in 153 windows**, of which **166 rows read
`converters on your battlefield: 0`**. Nine were taken. Four of the nine are the sanctioned
first-Howling-Mine carve-out (`162v146` s7, `162v123` s4, `162v126` s7, `162v125` s24) - **P8's
first-Mine half PASSES 4 of 4, and no `[second copy:` Mine row was taken at K of 0 (0 of 1)**.
The other five split, and the split is the finding:

| seq | game | row | punisher in HAND at that ask | result |
|---|---|---|---|---|
| s18 | `162v123` T8 | Dictate of Kruphix, K=0, Mine already out, two-row ask | **Underworld Dreams** | game won 20/0; that Dictate is why the T13 render read `their next draw step draws 5 cards` and the seat declined its own attack because 5 x 3 punisher damage was lethal |
| s23 | `162v126` T9 | Dictate of Kruphix, K=0, Mine out, two-row ask | **Ob Nixilis + Underworld Dreams** | game won 20/-6 |
| s98 | `162v125` T21 | Dictate of Kruphix, K=0, Mine out, two-row ask | **none** (3 Islands, Puzzle Box, Master of the Feast) | the real break |
| s7 | `162v130` T5 | Master of the Feast, K=0, `Cast Liliana's Caress` on the SAME menu | n/a | break, see 162-C |
| s31 | `162v146` T18 | Master of the Feast, K=0 (every punisher had been removed), their line `3 creatures` | n/a | 5/5 blocker at 10 life; the guide's own "Master goes at 1+ creatures on THEIR line" clause and its K-of-0 clause disagree here. Left unresolved deliberately - one window, and the game was lost to removal, not to this |

The brake as written is unconditional and two of the three engine takes were RIGHT. Rather than
loosen it by judgement, the edit gives it the exact release the corpus demonstrates: a punisher
NAME on the `Your hand (N cards):` line.
Classification: **STRATEGY / rule staleness** (all four facts - K, the Mine, the hand line, the
two-row menu - render).

**Before:**
```
  With a Howling Mine already on your
  battlefield line, a second row reading "converters on your battlefield: 0" is the brake: take
  a wall, a punisher, a Liliana's Caress, or "Cast nothing right now" instead; Master of the
  Feast, Dictate of Kruphix, Teferi's Puzzle Box and Forced Fruition all wait for the punisher.
```
**After:** same four lines, then:
```
  THE BRAKE HAS ONE RELEASE, AND IT IS READ OFF YOUR HAND LINE. Before you answer, scan "Your
  hand (N cards):" for UNDERWORLD DREAMS, FATE UNRAVELER or OB NIXILIS. If one of those three
  names is IN YOUR HAND, the converter is one turn away and the engine you cast now is drawing
  into it, not feeding a game you are not in: take the engine row. If none of the three is in
  your hand, the brake stands - "Cast nothing right now", every time, however much mana is idle.
  The hand line is the whole test; do not substitute "I will draw one".
  THIS IS WHAT THE THREE TAKES IN THIS CORPUS LOOKED LIKE. Two of them - Dictate of Kruphix at
  K of 0 on turn 8 with UNDERWORLD DREAMS in hand, and again on turn 9 with OB NIXILIS and
  UNDERWORLD DREAMS in hand - were RIGHT, and the first of them is why the draw forecast read
  "their next draw step draws 5 cards" five turns later and ended the game at 20 to 0. The third
  was the break: the same Dictate row on turn 21 with a hand of three Islands, a Puzzle Box and a
  Master of the Feast and NO punisher name in it.
```

---

## 162-C (NEW) - Caress outranks an engine on the SAME menu

**Finding.** `162v130` **s6** then **s7**, T5, 3 untapped sources. Menu:
`1. Cast Liliana's Caress {1}{b} {leaves 1 of your 3 ...}` /
`2. Cast Master of the Feast {1}{b}{b} (5/5) {leaves 0 ... - casting this taps you out}
{feeds: the opponent draws 1 extra card per turn; converters on your battlefield: 0 ...}` /
`3. Cast nothing right now`, with each row carrying the other row's fit clause
(`{leaves 1 source - row 2 needs more mana sources than the 1 this leaves}`). The seat answered
row 3 at s6 and then row 2 (Master of the Feast) at s7 - it declined the converter and then took
the gift, at K of 0, against an opponent line reading `0 creatures` so the 5/5 body blocked
nothing. The guide's cast order already puts Caress at 6 and the engines at 5, but the ordering is
stated as a list, not as a two-row menu test.
Classification: **STRATEGY**.

**Before** (cast order, entry 6): `... and BEFORE the engines, not after them.`
**After:** the same, then:
```
   WHEN A CARESS ROW AND AN ENGINE ROW ARE ON THE SAME MENU AND YOU CAN ONLY PAY FOR
   ONE, THE CARESS WINS - its row says so itself, because it prints no "{feeds:" clause and the
   engine's prints "converters on your battlefield: 0". WHAT THIS COST: a three-mana turn 5 menu
   of "1. Cast Liliana's Caress {1}{b}" and "2. Cast Master of the Feast {1}{b}{b} ... {feeds: ...
   converters on your battlefield: 0}" was answered first "Cast nothing right now" and then, on
   the very next window, Master of the Feast - the gift taken and the converter left in hand,
   against an opponent line reading "0 creatures" where the Master's 5/5 body blocked nothing.
```

---

## 162-D (NEW, cross-deck) - the HOLD-priority row

**Finding.** deck162 seats: **174 HOLD renders / 2 takes**; **65 windows carried
`[you declined this exact list N times already this turn]` at N>=2 with 0 takes**, and 42 of those
are `162v125` alone - turns 11 to 21, the SAME two-row `Cast Dictate of Kruphix` / `Cast nothing
right now` menu re-asked at Main 1, Combat begins, Attackers, Combat ends, Main 2, End and Cleanup,
eight to ten windows a turn, every one answered `Cast nothing right now` (the brake firing
correctly) while row 3 would have ended the turn's re-asks. That seat is 113 of deck162's 246
decisions for a game it won 20/0. Text inserted immediately above `=== DECIDING SITUATIONS ===`,
written so it cannot override the brake: the HOLD row is the pass the brake already chose.
Classification: **STRATEGY** (and the single largest inference saving available to this deck).

---

## NOT CHANGED, and why

- **The first-Mine carve-out** - 4 of 4 first Mines cast at K of 0, 0 tagged `[second copy:` rows
  taken at K of 0 (1 row offered). **P8 PASSES both halves for a second corpus.** Untouched.
- **The `[held back, it CANNOT block ANY of their creatures]` / lethal-check rules** -
  `162v123` **s30** is the corpus's best positive for this deck: `ATTACK: none` at T13 with a
  Fate Unraveler on the menu, reasoned from the render's own
  `DRAW FORECAST (theirs): their next draw step draws 5 cards` and
  `DRAW PUNISHERS ... Every card the OPPONENT draws costs them 3 life` -> 15 damage against their
  10 life. The seat waited and won on their draw step. Nothing to add.
- **The blocking rules** - 6 blocker windows, every one a wall block (`Shield Sphere blocks
  Silverquill Silencer` x3, `Fate Unraveler blocks Siege-Gang Commander`, `Master of the Feast
  blocks Augur of Autumn`), 0 `all_assignments_illegal`. Untouched.
- **Forced Fruition** - `162v146` s33/s34 at 10 life declined a lone Forced Fruition row at K of 0
  with 9 mana. Correct under the brake (no punisher on the battlefield AND none in hand), and the
  loss was caused by deck146 removing every punisher as it landed, not by that decline.
