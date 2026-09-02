# deck162 - wave-53 edits (before -> after, each with its finding)

Live guide under test: `bin/Res/ai/baka/deck162_strategy.txt` (36,004 B). Revised: 39,649 B
(+3,645). Corpus `matchups-20260902-023342`. Card facts verified with `/usr/bin/grep` against
`bin/Res/sets/primitives/*.txt`.

---

## G1 - survival arithmetic exists only at the BLOCK window; the mana is spent at the CAST
## window (162-A, HIGH - the one loss with a window)

**Finding.** `162v152` (L 0/11, T11), s11, T10, **4 life**, five untapped sources. Printed:
`Your battlefield (8 permanents listed, of which 0 are creatures, 0 of them able to attack right
now, and 5 are lands)`; `Opponent battlefield (10 permanents listed, of which 4 are creatures,
4 of them without a restriction against attacking, and 5 are lands)`. Menu:
```
1. Cast Liliana's Caress {1}{b} {leaves 3 of your 5 untapped mana sources untapped}
2. Cast Ob Nixilis, the Hate-Twisted {3}{b}{b} {leaves 0 of your 5 ... - casting this taps you out}
3. Cast Master of the Feast {1}{b}{b} (5/5) {leaves 2 of your 5 untapped mana sources untapped}
   {feeds: ... converters on your battlefield: 1 - Underworld Dreams ...}
4. Cast Shield Sphere {0} (0/6) {leaves 5 of your 5 untapped mana sources untapped}
5. Cast nothing right now (combat comes next this turn)
```
Answer: row 2. Its own PLAN said *"Activate Ob Nixilis's -2 ... Cast Master of the Feast. Attack
with Master of the Feast."* - and casting Ob Nixilis tapped out, so s12's menu had two rows left
(Shield Sphere / nothing). At s14 (T11) three attackers dealt 2+2+3 = 7 into 4 life with ONE
blocker; the seat's own reply reads *"There is no way to survive this combat."*
The rows print the counterfactual: **Master of the Feast leaves 2 of 5, Shield Sphere costs 0** -
both were affordable in the same window, giving a 5/5 flier and a 0/6 against three attackers.
Master at K=1 (Underworld Dreams out) is a legal cast under RULE 2 and sits at entry 5 of the
cast order; Ob Nixilis is entry 1b, so the guide as written produced the losing answer.

**Why.** The deck's survival arithmetic is real and correct but lives under `RULE 4 - BLOCKING`
("SURVIVAL COMES FIRST, AND IT IS ARITHMETIC - RUN IT BEFORE THE NUMBERED RULES BELOW"), i.e. it
is scoped to the block window, where the mana is already gone. #158: the fix is ORDER - the check
becomes the first case of the imperative it lost to.

**AFTER** - new section inserted immediately above `=== RULE 1 ... ===`:
```
=== RULE 0c - COUNT THE BODIES BEFORE YOU CAST ANYTHING. THIS OUTRANKS RULE 1 ===
RULE 4's survival arithmetic is run at the BLOCK window, and by then the mana is gone. Run this
one at every "which card do you cast now" ask, before you read Rule 1, off three numbers that are
already printed:
  L  = your printed life.
  N  = your own battlefield line's "of which N are creatures".
  M  = their battlefield line's "M of them without a restriction against attacking".
If M is greater than N and L is 10 or less, THIS TURN IS A BODY TURN. Take every creature row on
the menu you can afford in ONE window, cheapest per body first, before any noncreature row -
Shield Sphere {0} first because it is free, then Fog Bank, then Master of the Feast. A five-mana
planeswalker that taps you out is the wrong card on a body turn however loudly Rule 1 reads: the
punisher is damage you collect over turns you are still alive for.
READ THE "{leaves N of your M untapped mana sources untapped}" CLAUSE ON EACH ROW AND SPEND THE
WHOLE WINDOW. Two bodies out of one ask is the answer that survives; one body and a walker is not.
WHAT THIS COST: [the s11 record, as above]
```
**G1b** - and the check becomes numbered step 0 of THE WHOLE CAST ORDER (#165: an absolute keyed
to printed numbers goes INSIDE the numbered procedure, never beside it):
```
0. RULE 0c's body turn - M greater than N and your life 10 or less: every creature row you can
   afford this window, cheapest per body first, and nothing else. Then stop.
```
plus a matching DECIDING SITUATIONS bullet above the punisher-first bullet, and the punisher-first
bullet gains "(RULE 0c's body turn is the one thing that outranks this)".

---

## G2 - the #185 two-row-ask sentence swallowed the FIRST Howling Mine (162-B, HIGH regression)

**Finding.** Eleven windows across three games declined an untapped-first Howling Mine on a
two-row menu at `converters on your battlefield: 0`:
- `162v125` s7, s8 (T4), taken at s10 (T6) - 2 turns late.
- `162v130` s9, s10 (T11), s12, s13 (T13), taken at s15 (T15) - **4 turns late**; that game was
  won at **1 life**.
- `162v146` s6, s7 (T4), s10, s11 (T6), s13 (a `named_row_reask`), s15 (T8), taken at s16 (T8) -
  **4 turns late**; that game was lost at -1.
Every one of those rows is a FIRST Mine: the battlefield line shows no Howling Mine and the row
carries **no** `[second copy:` tag. Wave 52's over-brake count was 1 window; it is 11 here.

**Why.** Wave 52 shipped #185's fix - *"A two-row ask whose only spell is a second engine at K of
0 is the brake firing, not the brake failing: row 2 IS the answer"* - as a loud paragraph with
three imperatives, while the first-Mine exemption remained a half-sentence with none (#183's
diagnostic: strip the headings and read only the imperatives). The pilot pattern-matched the loud
branch. The discriminator is on the row and is exact: THE THREE-NUMBER AUDIT for
`[second copy: you already control Howling Mine; both stay on the battlefield - no legend rule]`
at deck162 - **11 rows rendered, 4 taken, and every one of the 4 was at K of 1 or more** (P8
PASS). Second Mines were declined 3/3 at K=0. The rule works on the tagged row; it is the
UNTAGGED row it eats.

**BEFORE**
```
- K OF 0: one Howling Mine is enough, and the FIRST Howling Mine is cast at K of 0 - its row
  reads "converters on your battlefield: 0" too, and that is fine for that one card: it draws
  you into the punisher and is the engine you convert later.
```
**AFTER**
```
- K OF 0: one Howling Mine is enough, and the FIRST Howling Mine is cast at K of 0 - its row
  reads "converters on your battlefield: 0" too, and that is fine for that one card: it draws
  you into the punisher and is the engine you convert later.
  THE ROW TELLS YOU WHICH MINE IT IS, AND IT IS THE ONLY THING YOU HAVE TO READ. A SECOND Mine's
  row carries the tag "[second copy: you already control Howling Mine; both stay on the
  battlefield - no legend rule]". A FIRST Mine's row does not carry it. So: a "Cast Howling Mine"
  row with NO "[second copy:" tag on it is your first Mine - CAST IT THIS ASK. At K of 0. On a
  two-row menu whose other row is "Cast nothing right now". On your turn 4 and on your turn 11.
  The brake below is about the tagged row and nothing else, and "converters on your battlefield:
  0" on an untagged Mine row is not a brake, it is the reason you are casting it.
  WHAT THIS COST, eleven windows across three games in one corpus: [the record, as above]
```
plus the matching DECIDING SITUATIONS bullet:
```
- A "Cast Howling Mine" row carries NO "[second copy:" tag and your battlefield line shows no
  Howling Mine: that is your FIRST Mine. Cast it this ask, even at "converters on your
  battlefield: 0", even when the menu has only two rows.
```

---

## Considered and NOT made

- **RULE 2's K-string (162-I / P8).** PASS. 9 engine casts at K >= 1, all legal; 0 engine casts
  at K = 0 with a Mine on the line (was 2/2); 5 first Mines at K = 0, all exempt; the Master of
  the Feast carve-out was met twice (`162v126` s7 at K=1, s10 at K=2) and declined at K=0 with
  their line at "of which 0 are creatures" (`162v126` s6). No edit - and per #186 the rule earned
  a citation, not just an absence of breaks.
- **`{this hits YOUR permanent}`.** 12 windows offered at deck162, **0 taken**. No edit.
- **Blocking.** 2 windows, both correct (`162v146` s8 Shield Sphere on the Adventurer; `162v152`
  s14 Shield Sphere on the biggest "deals N", which was the only surviving-shaped answer and did
  not survive - the loss was decided at s11). No edit.
- **`162v146` (L -1/17).** No window: every menu in that game was two rows (Mine / nothing, then
  Ob Nixilis / nothing) and the hand held no wall at any point. Reported as no-window per #167.
