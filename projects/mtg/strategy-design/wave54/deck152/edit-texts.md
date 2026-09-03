# deck152 - wave-54 edits (before -> after), each with its finding

Live guide 57,075 B -> 59,215 B (+2,140). Three edits: one mulligan re-key (the adjudication the
brief asked for), two that replace an ABSOLUTE with arithmetic the engine already prints.

---

## 152-B (RE-KEY) - THE MULLIGAN FLOOR, ADJUDICATED

**The break, in this corpus.** `152v146` seq 1. Header: `Your hand (7 cards), counted by the
engine: 3 lands, 4 spells.` / `Mana sources among those lands, counted by the engine: {W} 3,
{U} 1.` / `Playing every land in this hand would not cover any spell in it (you have 3 lands; no
spell in it is castable off {W}{U} alone).` Hand: two Teferis, two Katildas, three lands.
Answer: **Mulligan**. Reply verbatim: *"Mulligan for a hand with at least one green mana source
to cast Katilda on turn 2. Keep 6 if the new hand has 1+ lands and covers a spell, or 2+ lands
regardless."* The guide's floor reads `TWO OR MORE LANDS AND AT LEAST ONE SPELL: KEEP. That is
the floor and it is not a close call.` This is a THREE-land seven - three rungs clear - shipped
with a COLOUR named as the reason, i.e. THE FORBIDDEN THOUGHT, written in the same reply that
correctly restates the floor for the NEXT look.

**Mulligan census, deck152, 4 asks over 6 games:** `152v123` s1 (1 land, count-family clause) MULL
- right; s2 (1 land, count-family) MULL - right; s3 (4 lands) KEEP, bottomed 2 - right.
`152v146` s1 (**3 lands, colour-family clause**) **MULL - WRONG**; s2 (5 lands) KEEP, bottomed
Teferi - right. `152v125`/`v126`/`v130`/`v162` s1: 3, 4, 4, 3 lands, KEEP x4 - right.
**Mulligans at two or more lands: 1 of 4 (wave 53: 1 of 3; wave 52: 1 of 3).** No mull-to-zero.
No repeated mulligan against a floor (`152v123`'s chain was 1-land at both looks, floor-compliant),
so **no OWNER QUESTION is owed this wave** - the new-baseline rule's trigger did not fire.

**The adjudication.** The floor is not the problem and is not touched. What changed is the
TRIGGER, and it changed in the ENGINE, this corpus:

| string | wave-53 corpus | wave-54 corpus |
|---|---|---|
| `Playing every land in this hand would not cover any spell in it.` (bare, period) | **10** | **0** |
| `... in it (N land(s) is/are not enough for your cheapest spell at mana value M)` | 0 | **4** |
| `... in it (you have N lands; no spell in it is castable off {X} alone)` | 0 | **7** |

Lane E's cause clause is a PASS by its own falsifier (0 renders without a cause clause). But
7 of the 11 renders are the COLOUR family, and the colour family is the guide's FORBIDDEN
THOUGHT restated by the engine in the engine's voice, one line above the ask - a surface the
trust doctrine says the pilot must believe. Three corpora of "at two or more lands that sentence
is not a ship" were written against a literal that no longer exists. So the edit is a RE-KEY of
the same ruling onto the two clause families the engine now prints, plus the missing predicate:
the colour clause is a statement about TURN ONE, not a verdict on the hand. The render half is
filed to the engine list (E-4) - the clause should carry its own scope, not a bare colour claim.

BEFORE:
```
  "Playing every land in this hand would not cover any spell in it" IS A ONE-LAND TEST AND
  NOTHING ELSE. Read the engine's land count FIRST and let it decide which rung you are on. At
  TWO OR MORE lands that sentence is not a ship and never was, and it is printed most often for a
  reason that is not about land count at all: your two lands make one colour and your spells need
  the other. That is the ordinary two-colour opener, it is a KEEP, and the coverage line does not
  say otherwise - it only says nothing is castable on turn ONE, which was already true of a hand
  whose cheapest card costs TWO.
```
AFTER:
```
  "Playing every land in this hand would not cover any spell in it" IS A ONE-LAND TEST AND
  NOTHING ELSE. Read the engine's land count FIRST and let it decide which rung you are on. At
  TWO OR MORE lands that sentence is not a ship and never was.
  THAT LINE NOW PRINTS ITS OWN REASON IN BRACKETS, AND THE TWO REASONS ARE NOT THE SAME
  DECISION. Read the parenthetical, not just the sentence in front of it.
    COUNT FAMILY - "(1 land is not enough for your cheapest spell at mana value 2)",
    "(2 lands are not enough for your cheapest spell at mana value 3)". This is the engine doing
    arithmetic on the land COUNT, and at ONE land it is the whole one-land test.
    COLOUR FAMILY - "(you have 3 lands; no spell in it is castable off {W}{U} alone)". This is
    THE FORBIDDEN THOUGHT below, printed by the engine in its own voice. It is a statement about
    TURN ONE and nothing else: it says your lands make one pair of colours and your spells want
    another, which is the ordinary two-colour opener from a 25-land deck that splits 14 green and
    15 white. It is a KEEP at THREE lands, at four, at two - at any land count the floor clears.
    The engine is telling you a true fact; the ship is a conclusion you added to it.
  WHAT THIS COST, a fourth corpus running: a SEVEN with THREE lands ({W} 3, {U} 1) holding two
  Teferis and two Katildas was shipped under "(you have 3 lands; no spell in it is castable off
  {W}{U} alone)", the reply reading "Mulligan for a hand with at least one green mana source to
  cast Katilda on turn 2" - a colour named as the ship reason, on a hand three rungs above the
  floor, in the one game this deck lost.
```

---

## 152-A (REPLACE AN ABSOLUTE WITH THE PRINTED ARITHMETIC)
**Finding.** `152v126` s28 (T11, 20 v 23), s34 (T13, 20 v 13), s44 (T15, 20 v 2): 5, then 7,
then 9 A-lines; `their untapped blockers` names ONE creature, `Pride Guardian (0/3)`; **8
A-lines whose tag contains `(you kill it, your attacker lives ...)` were held** across the three
windows, and only the two `[no creature they control can block this attacker]` fliers were sent
each time. Under the wave-53 rule ("never for a creature whose tag contains even one entry
reading (you kill it, your attacker lives)" + DECLARE THEM ALL) this reads as **P4 FAIL 3
windows / 8 A-lines**. It is not a simple failure, and the reason is a clause the engine printed
that the wave-53 rule was written without: every one of those tags reads
`(you kill it, your attacker lives (blocking trigger: they gain 3, and their converter takes 3
off you))`, and s44's flier rows add `(their life LOOP is in play: any life they gain or you
lose in a loop ...)`. With Sanguine Bond on their line each point they gain costs the seat one
too, so one extra 2/2 past B=1 buys 2 damage for 3 gained and 3 lost. The pilot's fliers-only
line took them **23 -> 13 -> 2** and the seat never left 20 life; it won 20/-10 on turn 15.
**Adjudication: the rule's direction is right and its absoluteness is wrong at a one-name
blocker line under a life-loop converter.** The edit makes the surplus call arithmetic on the
number the tag prints instead of adding a fourth restatement, and it exempts the unblockable
row from B (which the wave-53 text left implicit and which is where all six sent A-lines were).

BEFORE (tail of the "converter of THEIRS on the line" paragraph):
```
  So with a converter of THEIRS on the line and only ONE of the two names printed: send
  every creature whose A-line prints no "(blocking trigger:" and no stop - your fliers first -
  then keep sending until the number of attackers you have declared is LARGER than B, taking the
  cheapest printed price you are offered; and hold only the creature whose whole blocker list
  stops it AND pays them.
```
AFTER (same text, plus):
```
  DO THAT ARITHMETIC WITH THE PRINTED NUMBER, BECAUSE UNDER SANGUINE BOND EVERY POINT THEY GAIN
  COSTS YOU ONE TOO. The tag prints both halves - "(blocking trigger: they gain 3, and their
  converter takes 3 off you)". So one extra attacker past B is worth declaring when the power it
  adds to the unblocked total is MORE THAN TWICE the gain number in the tag it walks into; at or
  below that, the surplus is a wash and the fliers alone are the cleaner line. WHAT THIS BOUGHT,
  three combats in one game: a "Pride Guardian (0/3) (you kill it, your attacker lives (blocking
  trigger: they gain 3, and their converter takes 3 off you))" list of ONE name, five then seven
  then nine A-lines, and two "[no creature they control can block this attacker]" fliers sent
  alone every time. 2/2s and 5/4s held behind a gain of 3 are inside the noise; the fliers took
  them 23 -> 13 -> 2 and the seat never dropped below 20.
```

## 152-A(b) - the matching DECIDING SITUATION, re-keyed the same way
BEFORE:
```
- Their line shows ONE converter name, you have more attackers than there are distinct names in
  "their untapped blockers", and the surplus tags read "(you kill it, your attacker lives)" ->
  DECLARE THEM ALL. The surplus cannot be blocked and the price on the rest is the 1 or 3 life
  the tag prints.
```
AFTER:
```
- Their line shows ONE converter name, you have more attackers than there are distinct names in
  "their untapped blockers", and the surplus tags read "(you kill it, your attacker lives)" ->
  DECLARE every one whose power beats TWICE the gain number printed in its own tag; the surplus
  cannot be blocked and that is the price you are buying it at. An A-line reading "[no creature
  they control can block this attacker]" is always sent and never counts against B.
```

---

## NOT EDITED, and why
- **`{paying this taps:` (152-C, wave-53 P6).** 6 rows / 1 take this corpus, the take a ONE-name
  clause. The count re-key holds; nothing to say.
- **`[legendary:` (P6).** 8 rows, **0 casts**. `152v146` s39/s44/s51/s61 are four consecutive
  own-main `Cast nothing` answers whose only live row was
  `Cast Teferi, Who Slows the Sunset {2}{u}{w} [legendary: you already control Teferi ...]` -
  the rule firing exactly as written, four times, in a losing game. Correct and left alone.
- **Blocks (P7).** 6 block windows, 2 `BLOCKS: none` (both above the life floor), 0 repeated
  B-numbers, 0 `multiblock_reask`. One `all_assignments_illegal` fallback (`152v146` s58) is an
  engine item, not a guide one - see the seat file, E-5.

---

## 152-D (NEW) - two menace attackers, two blockers: gang one or block nobody
**Finding.** `152v146` seq 58, T19, the corpus's only `all_assignments_illegal` fallback.
`A3. Spider #1 (2/1) deals 2 [reach, indestructible, menace ...] [menace - only a block by TWO
OR MORE of your creatures counts; one creature alone does not block it at all]`, A4 identical;
`B1 ... - may block ... A3 (no 1-on-1 result exists - only a block by TWO OR MORE of your
creatures counts; this one alone does not block it at all), A4 (same)`, B2 the same. The seat
answered `BLOCKS: B1:A3, B2:A4`. **This is a pure STRATEGY miss, not perception**: the render
states the menace rule three times (on the A-row, and inside both B-rows' may-block entries), and
the guide already carries the affordance sentence keyed to the exact literal ("two different
B-numbers on one A-number is the answer"). What the guide never states is the FAILURE SHAPE - the
answer that spreads one blocker onto each menace attacker, which blocks nothing AND voids the
whole line. Per amendment #181 the fix is not a restatement of the affordance but a
recognise->do entry for the shape that broke, in DECIDING SITUATIONS where the ask-kind step 0
puts it.

BEFORE (DECIDING SITUATIONS):
```
- Each B-number appears at most once in your BLOCKS line; several B-numbers on ONE A-number is
  legal, and is what "only a block by TWO OR MORE of your creatures counts" is asking for.
```
AFTER (same entry, plus a new one):
```
- TWO OR MORE attackers carry that menace clause and you have exactly TWO blockers -> put BOTH
  B-numbers on ONE of them, or write "BLOCKS: none". One B-number on each is the trap, and it is
  the WORST of the three answers: neither Spider is blocked, and the whole line is illegal, so
  the engine throws your answer away and blocks for you with all of it. "BLOCKS: B1:A3, B2:A4"
  against two menace 2/1s was written in this corpus - both assignments illegal, an
  all_assignments_illegal fallback, and the seat lost that game at -11. Count how many A-numbers
  print "no 1-on-1 result exists" BEFORE you spend a blocker on any of them: n menace attackers
  need 2n blockers, and if you have fewer, you are choosing WHICH ONE to stop, not how to spread.
```
