# deck162 - wave-52 edits (before -> after), each with its finding

Corpus `matchups-20260901-223430/`, deck162 seats (6 logs, 163 records, 0 fallbacks).
Record **5-1**. RULE 1 punisher-first held **20 of 21** windows, RULE 3 (walls) 100%, the block
ladder 0 breaks, Ob Nixilis's -2 3/3 on their side with 8 `{this hits YOUR permanent}` rows
offered and 0 taken. ONE edit. Nothing else in the guide is touched.

---

## 162-I - the two-row window is where RULE 2's brake breaks
**Finding.** deck162 took 20 `{feeds:` engine casts; 7 printed
`converters on your battlefield: 0`. Five of the seven are permitted by the guide as written
(three FIRST Howling Mines - `v130` s6, `v146` s4, `v123` s5 - and two Master of the Feast casts
under the named exception, `v146` s17 with their line at "of which 1 is a creature" and `v123`
s10 at "of which 3 are creatures"; the s10 reply cites *"as per Rule 2 exception"* by name).
The two real breaks are both a SECOND Howling Mine at K of 0:
- `162v130` s11 (T7, 19/20): row 1 `Cast Howling Mine {2} [second copy: you already control
  Howling Mine; both stay on the battlefield - no legend rule] ... converters on your
  battlefield: 0`; row 2 `Cast nothing right now`. **Two rows.** Taken.
- `162v123` s8 (T7, 20/20): the same two rows, same tags. Taken. That game is deck162's only
  loss (-18/17 T12) - deck123 drew off two Mines into a Thraben Doomsayer token board while the
  first punisher did not land until T11.
Both replies wrote a plan sentence the guide already forbids ("Establish double Howling Mine
pressure... Once the punisher is on the battlefield, begin stacking draw engines"). What is new
is the SHAPE: in both windows the ONLY alternative was "Cast nothing right now". The rule's
prose lists four alternatives ("a wall, a punisher, a Liliana's Caress, or Cast nothing") and
reads as though one of the first three will be there; when they are not, the seat treats the
menu as forcing the Mine.

**Before** (inside RULE 2, K OF 0):
```
  WHAT THIS COST, both ways: a second Mine went down on turn 6 under "converters on your
  battlefield: 0" - ...
```
**After** - the same passage, preceded by:
```
  THE WINDOW THIS BREAKS IN HAS EXACTLY TWO ROWS, AND THE OTHER ONE IS "Cast nothing right now".
  Both breaks in the last corpus looked like this: "1. Cast Howling Mine {2} [second copy: you
  already control Howling Mine ...] {feeds: ... converters on your battlefield: 0 ...}" and
  "2. Cast nothing right now" - nothing else on the menu. That is not a menu with no answer on
  it; row 2 IS the answer, and it is the answer this rule was written to produce. A two-row ask
  whose only spell is a second engine at K of 0 is the brake firing, not the brake failing:
  "Cast nothing right now" costs you a turn of mana you had no better use for, and the second
  Mine costs you the game. Take row 2 and say so in one clause.
```
and a matching DECIDING SITUATIONS line:
```
- That row is one of only TWO on the ask and the other is "Cast nothing right now": take
  "Cast nothing right now". A menu with no good spell on it is not a menu that forces the bad
  one; this seat took the second Mine at K of 0 twice from exactly that two-row ask.
```

---

## Considered and NOT made
- **The one RULE-1 deviation, `162v130` s22** (T15, 20/8): Fate Unraveler #2 and Teferi's Puzzle
  Box on the same list; the seat took the Box. By the letter that is a break. On the board it
  was the kill: two punishers were already out, two Howling Mines were untapped, their hand was
  full, and the Box turned their hand size into damage on the next draw step - the game ended
  20/0 on turn 16. RULE 1's absoluteness is what makes it work at this tier and one winning
  deviation is not evidence to soften it; recorded, not patched.
- **The first-Mine over-brake** (wave-51's 4-window shape at `v130`): 1 window this corpus
  (`v130` s5, T3 Main 1, passed; the Mine went down at s6, Main 2, same turn). Not a lane.
- **`v126` s7-s18: twelve consecutive `Cast nothing right now` asks in one opponent turn**, all
  offering the same single `Cast Dictate of Kruphix` flash row, all declined, 452 s of inference.
  Every answer was correct. Routed to the seat file as engine item E-2 (HIGH, efficiency) - no
  prompt text can fix a window that should not have been opened.
- **Guide-literal drift**: `/usr/bin/grep -c "already owned:\|from exile]"` returns 0 for all
  three of my guides; the wave-52 re-key obligation is a deck123/deck126 item, not mine.
- The Master-of-the-Feast exception, the walls, RULE 1 and the Ob Nixilis maker-first order all
  held at 100% - no restatement added to any of them.
