# deck162 - wave-54 edits (before -> after), each with its finding

Live guide 39,649 B -> 41,379 B (+1,730; still the pool's smallest). Three edits: one new key,
one gate reversal, one recital retired because its lane closed.

---

## 162-C (CLOSE THE LANE) - THE EXEMPT-CASE OVER-BRAKE, ADJUDICATED

**Every `Cast Howling Mine` row on this deck's six seats, with its tag and its K, read off the row:**

| game | seq | turn | tag | K | taken | what was taken instead |
|---|---|---|---|---|---|---|
| v123 | s4 | 3 | FIRST (untagged) | 0 | **YES** | - |
| v123 | s5 | 5 | `[second copy:` | 0 | no | Liliana's Caress (the brake's own answer) |
| v123 | s7 | 7 | `[second copy:` | 1 | no | Master of the Feast |
| v123 | s9 | 9 | `[second copy:` | 1 | no | Underworld Dreams (punisher first) |
| v123 | s11 | 11 | `[second copy:` | 2 | no | Fate Unraveler (punisher first) |
| v125 | s5 | 6 | FIRST (untagged) | 0 | no | **Underworld Dreams** (3 sources; both unaffordable) |
| v125 | s8 | 8 | FIRST (untagged) | 1 | **YES** | - |
| v125 | s13/s15/s18/s19 | 12-16 | `[second copy:` | 1-2 | no | Master, Ob Nixilis, Dictate, Fate Unraveler |
| v125 | s16, s21 | 14, 18 | `[second copy:` | 2 | **YES x2** | - |
| v130 | s5 | 5 | FIRST (untagged) | 0 | **YES** | - |
| v130 | s13 | 11 | `[second copy:` | 1 | no | Underworld Dreams (punisher first) |
| v130 | s14 | 11 | `[second copy:` | 2 | **YES** | - |

**Verdict: the wave-53 edit worked and the lane CLOSES.**
- Untagged first-Mine rows **declined: 1 of 4** (wave 53: **11 across three games**). The one
  decline, `162v125` s5, is not the over-brake at all: the same three-source menu offered
  `Cast Underworld Dreams {b}{b}{b} {leaves 0 ... - casting this taps you out}`, the punisher
  RULE 1 says goes first, and the two were not both payable.
- Tagged second-Mine rows at `converters on your battlefield: 0`: **1 window, 0 takes** - the
  brake fired and took a Caress, exactly as the rule is written. Second Mines at K >= 1: 4 takes.
- Wave-53 P8, both halves: **PASS**.

The eleven-window recital has therefore stopped paying and is replaced by the ONE case the data
still shows is undecided: the first Mine colliding with a punisher for the same sources.

BEFORE:
```
  WHAT THIS COST, eleven windows across three games in one corpus: an untagged first Howling Mine
  was declined on a two-row menu twice on turn 4 and again on turn 6 in one game, four times
  across turns 11 and 13 in another, and five times across turns 4 to 8 in a third - the engine
  arriving four turns late twice, in a game that was lost at -1 and a game won at 1 life. With a Howling Mine already on your
```
AFTER:
```
  ONE COLLISION, AND THE PUNISHER WINS IT: when the same ask offers your first Mine AND an
  Underworld Dreams, Fate Unraveler or Ob Nixilis, and the row prices say you cannot pay for
  both this turn, take the PUNISHER and the Mine next window. "Cast it this ask" is about a
  menu where nothing better is competing for the same sources, and RULE 1 outranks it.
  With a Howling Mine already on your
```

---

## 162-A (NEW KEY, THE WAVE'S HEADLINE) - the brake is about rows that print `{feeds:`
**Finding.** 16 own-turn `Cast nothing right now` answers on menus holding a live row - **10 of
them are the same shape**, and it is the mirror image of the first-Mine lane that just closed:

- `162v126` **s7, s8, s9, s10, s11, s12** (turns 4, 4, 6, 6, 8, 8). Two-row menu, every time:
  `1. Cast Liliana's Caress {1}{b} {leaves 0 of your 2 untapped mana sources untapped - casting
  this taps you out}` / `2. Cast nothing right now`. Answered row 2 six times. The plan sentence
  each time: *"cast Underworld Dreams or Fate Unraveler to establish a punisher first."*
  Underworld Dreams costs `{b}{b}{b}`; the seat had TWO lands and did not reach a third black
  source until turn 10. Six turns of mana bought nothing. Board went 20/20 to 20/26; final 0/38.
- `162v146` **s7, s8, s10, s11** (turns 4, 4, 6, 6). Same two-row shape with
  `1. Cast Fog Bank {1}{u} (0/2)`; s15 (T10, 4 sources) adds Master of the Feast and is declined
  with *"Keep mana open for Underworld Dreams, Fate Unraveler, or Ob Nixilis."*

**Neither Liliana's Caress nor Fog Bank hands the opponent a card.** The engine says so on the
row: every gift row prints `{feeds: the opponent draws N extra card(s) per turn; converters on
your battlefield: K ...}` and neither of these rows prints that clause at all. RULE 2's whole
brake is about `{feeds:` rows, and the guide never said so - it said "draw engines", a category
the pilot widened to "anything I could cast before my punisher". Worse, the gate is backwards for
Caress specifically: RULE 2's own qualifier counts a Caress as a converter (at opponent hand size
7+), so Caress on the battlefield is the thing that makes a LATER Mine's K read 1.
The second half of the edit is the sentence that turns those windows from a judgment into an
error of fact: **you cannot hold mana on your own main phase for your own sorcery-speed card.**
Every punisher, Mine, Dictate and Caress in this deck is sorcery speed; if one were castable the
engine would have printed it as a row on that same menu.

BEFORE (RULE 2, immediately before the "drawing more cards is the priority" line):
```
The thought "drawing more cards is the priority, I need to find my win condition" is ALWAYS FALSE
```
AFTER (new block inserted above it):
```
THE BRAKE IS ABOUT ROWS THAT PRINT "{feeds:", AND ONLY THOSE. Every card that hands the opponent
a card prints "{feeds: the opponent draws N extra card(s) per turn; converters on your
battlefield: K ...}" on its own row. A row WITHOUT that clause gives them nothing, so no K, no
punisher and no "wait for a converter" sentence has anything to say about it: Liliana's Caress,
Fog Bank, Shield Sphere, a punisher and a land are all free of this rule. Cast them when they
are offered. WHAT THIS COST, six windows in one game: a two-row ask whose only spell was
"Cast Liliana's Caress {1}{b}" was answered "Cast nothing right now" on turns 4, 4, 6, 6, 8 and
8 - two lands untapped every time, no third black source until turn 10, and the plan sentence
reading "cast Underworld Dreams first" about a spell that needed a source the seat did not have.
Six turns of mana bought nothing and the game ended 0 to 38. "Cast nothing right now" is the
answer to a menu of GIFTS, never to a menu of one cheap permanent that gives them nothing.
AND YOU CANNOT HOLD MANA ON YOUR OWN MAIN PHASE FOR YOUR OWN SORCERY-SPEED CARD. Underworld
Dreams, Fate Unraveler, Ob Nixilis, the Mines, the Dictates and the Caress are all sorcery
speed: if one of them were castable this window the engine would have printed it as a row.
It is not on the menu, so no amount of untapped mana reaches it this turn - "keep mana open
for Underworld Dreams" on your own main is a sentence that costs a turn and buys nothing.
```

---

## 162-B (GATE REVERSAL) - Liliana's Caress moves to the front of the cast order
**Finding.** Same 6 windows. The cast order's item 6 gated Caress behind a Mine/Dictate/Box,
which is a FEED gate on a card that feeds nothing, and which inverts RULE 2's own converter
arithmetic. The row is the key: no `{feeds:` clause, so no gate.

BEFORE:
```
6. Liliana's Caress, when a Howling Mine, Dictate or Puzzle Box is on your battlefield.
```
AFTER:
```
6. Liliana's Caress - at K of 0 exactly as readily as at K of 1, and BEFORE the engines, not
   after them. Its row prints no "{feeds:" clause because it hands them nothing; RULE 2's brake
   is about rows that DO print one. A Caress already on the battlefield is what lets a later
   Mine's row read "converters on your battlefield: 1" at all.
```

---

## NOT EDITED, and why
- **Forced Fruition declines at K of 0** (`162v146` s24 T16, s29 T20, two-row menus). Forced
  Fruition's row DOES print `{feeds:`; K was 0 both times (Dictate of Kruphix is an engine, not
  a converter). The brake fired correctly. Left alone - and note 162-A's new `{feeds:` key keeps
  these two windows braked, which is the control on the edit.
- **`162v123` (L -32/13) is a NO-WINDOW loss.** Every punisher landed on schedule (Mine T3, Caress
  T5, Master T7, Underworld Dreams T9, Fate Unraveler T11) and the opponent still went from 1
  creature at T9 to 24 creatures at T11 behind Bloodline Keeper + Intruder Alarm. At s11, 17
  life, the alternatives to Fate Unraveler were Fog Bank (stops one of 24) and a second Caress.
  This is the deck feeding a token engine, which is a matchup, not a decision.
- **`162v152` (L 0/18 T9) is a NO-WINDOW loss.** 8 decisions, mulligan to 6, first punisher on
  turn 6 at three lands, dead on turn 9 to a curve-out. Nothing on any menu changes it.
