# deck162 - wave-57 edits (before -> after), each with its finding

Base = `bin/Res/ai/baka/deck162_strategy.txt` (46,607 bytes, 564 lines).
Result = `wave57/deck162/strategy.txt` (**47,921 bytes**, +1,314). Pool band 41-71 KB: inside, and
this is the smallest guide in the pool.

---

## 162-A - the zero-converter brake becomes a last-pass CHECK on the chosen row, not a condition carried through deliberation

**Finding.** Seat file HEADLINE 5. Five takes of a `{feeds:}` row at
`converters on your battlefield: 0` with no punisher on the hand line, all in `162v125`, the long
game this deck lost (wave 56: 1). Two are Dictate of Kruphix (turn 21, seqs 40 and 41) and three
are Master of the Feast (turns 26 and 28, seqs 57, 64, 65) - and all three Master takes are at an
opponent line reading `of which 0 are creatures`, i.e. inside the exclusion the guide's own Master
exception already states. The decisive evidence is seq 57's reply, which reads *"With no converters
on the battlefield, we cannot safely stack more draw engines ... We must wait for a converter"* and
whose CHOICE is a draw engine. Wave-56 P8 fails 5 of 20; its second half (replies naming Forced
Fruition as the reason) passes 0 of N. The rule is known and restated and not bound to the row.

**BEFORE** (6 lines) - "CHECK YOUR OWN PLAN SENTENCE BEFORE YOU ANSWER. If your PLAN says 'stack
draw engines' ... the plan is describing a board you do not have ... Count the names; then answer."

**AFTER** (22 lines) - the same rule expressed as an operation with a verdict, run last:
```
THE BRAKE IS A CHECK ON THE ROW YOU HAVE ALREADY CHOSEN, RUN BEFORE YOU SEND IT. Having picked a
row, read that row's own text one more time:
  if it contains "converters on your battlefield: 0"
  and your hand line names none of UNDERWORLD DREAMS, FATE UNRAVELER, OB NIXILIS
  and (for Master of the Feast) their battlefield line reads "of which 0 are creatures"
  -> YOUR ANSWER IS WRONG. Change it before you send: a wall, a punisher, a Liliana's Caress, a
  land, or "Cast nothing right now".
Run that check on the ROW, not on your memory of the board, and run it last.
```
plus the plan-sentence check with **the exact words this seat wrote while breaking the rule**
quoted into it, and a WHAT THIS COST paragraph naming all five takes by card and turn, including
the fact that Dictate of Kruphix has no body at all and draws the opponent a card every one of
their draw steps.

Three deliberate choices: (a) the check operates on the row's printed text, which is the surface
that was on screen in all five breaks; (b) it produces a verdict ("YOUR ANSWER IS WRONG, change
it") rather than a consideration; (c) it is placed at the end of the deliberation, because the five
breaks all had the rule EARLY in the reply and the wrong row LATE.

---

## 162-B - Fog Bank, when it is the only non-pass row and casting it taps you out

**Finding.** Wave-56 P7's one hit: `162v152` seq 4, turn 4, the only non-pass row was
`Cast Fog Bank {1}{u} ... {leaves 0 of your 2 untapped mana sources untapped - casting this taps
you out}`, the opponent's line showed creatures, and the seat answered `Cast nothing right now`.
The game was won, so this is a small hit, not a headline - but the wall rule is one of this deck's
two defences and the `taps you out` clause is what deflected it.

**BEFORE**
```
- "Cast Fog Bank {1}{u}": if the opponent's battlefield line shows ANY creature, cast Fog Bank the
  first ask it is offered.
```
**AFTER**
```
- "Cast Fog Bank {1}{u}": if the opponent's battlefield line shows ANY creature, cast Fog Bank the
  first ask it is offered - INCLUDING when it is the only non-pass row on the menu and casting it
  taps you out. A wall that taps you out on your own main phase costs nothing you were going to
  spend; "Cast nothing right now" there buys a turn of nothing (`vs152` seq 4, turn 4).
```
No new key is added for P7; the carve-out lives on the rule it belongs to.
