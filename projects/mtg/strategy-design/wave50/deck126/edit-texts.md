# deck126 - wave-50 edits (each: finding -> before -> after)

Numbering continues from wave 49 (F10-F15). Live guide = `bin/Res/ai/baka/deck126_strategy.txt`
at ce3c7a90c; the full revised guide is `wave50/deck126/strategy.txt` (36.8 KB -> ~40.5 KB: F16
rewrites in place, F17/F18/F19 are the additions; the old "exactly one creature" bullets fold
into F16's row-keyed wording).

## F16 - Tribute-at-one re-keyed to the row string `<name> is sacrificed, you gain N` (D46: 3/4, #118)
Finding: the engine now prints the victim and the gain on the row (`{right now: they control 1
creature - Thraben Doomsayer is sacrificed, you gain 2}`); the guide keyed the rule to the
opponent header's "of which 1 is a creature". Three of four such rows taken in their first window
(`vs123` seq 8, 12; `vs162` seq 11); the miss (`vs162` seq 6) declined it for a Battlement with
the plan "only if the opponent has exactly one creature". The row is the cheaper key and it also
carries the name the [defender] carve-out (F17) needs.
BEFORE (Rule 4): "THE OPPONENT CONTROLS EXACTLY ONE CREATURE. ... You do not read this off the
names: the opponent battlefield header prints it - "Opponent battlefield (6 permanents listed, of
which 1 is a creature, ...)". When that creature count reads exactly 1 and "Cast Tribute to
Hunger" is in the menu, CAST IT IN THAT WINDOW ... READ "1" AS "ONLY" ..."
AFTER: "THE ROW TELLS YOU WHAT HAPPENS. With two or more of their creatures it reads "{right now:
they control 4 creatures - they choose which one}". With exactly one it NAMES the victim and the
life: "{right now: they control 1 creature - Thraben Doomsayer is sacrificed, you gain 2}". With
none it reads "they control 0 creatures - at 0 this does nothing"." + "THE ROW NAMES THE VICTIM
("<name> is sacrificed, you gain N") and that creature is NOT tagged [defender] ... CAST IT IN THAT
WINDOW ... The row already did the counting: "is sacrificed" means it IS their only creature, right
now." History condensed to this corpus's three takes and one miss. Rule 7b, Rule 2's first bullet
and both SITUATIONS bullets re-keyed to the same string; the 0-stop cites `at 0 this does nothing`
and the count (40 rows, 0 casts).

## F17 - NEW: a [defender] victim is entry 8, not 7b (`vs162` seq 6/8, then seq 23-24)
Finding: Tribute #1 spent on `Shield Sphere is sacrificed, you gain 6` at 18 life (t6), #2 on Fate
Unraveler (t8, right); at t14 BOTH enchantments were out against two Fog Banks (`2 creatures, 0
without a restriction`), the seat at 3, no Tribute in hand - a Tribute wins that window outright
(Fog Bank sacrificed, +2, loop). The literal "1 = only creature = cast at any life" absolute had
no wall carve-out; a defender never attacks, so the edict removes nothing and spends the deck's
only own-turn win button. Also cited: the same seq 6 window declined an Idyllic Tutor (entry 2)
for the Battlement (D45-adjacent) - folded into Rule 7 entry 2's text.
AFTER (Rule 4, new bullet): "THE ROW NAMES A VICTIM TAGGED [defender] (Shield Sphere, Fog Bank, a
wall): it was never going to attack you, so the edict removes nothing that hurts you - it buys N
life and spends the ONE card in this deck that presses the win button on your own turn. Hold it:
it is entry 8 of Rule #7, not 7b, unless your life is 10 or less or both enchantments are out
(then it wins). You spent one on a Shield Sphere for 6 life on turn 6 ..." Rule 7b gets the
one-line exception, entry 8 the [defender] clause, Rule 2's Tribute bullet the sentence "It works
on a wall of theirs too: a Fog Bank sacrificed is 2 life gained, and 2 is enough", SITUATIONS a
bullet. #131-style check on the Tribute sentences: {non-defender lone creature} -> cast (7b);
{defender lone creature, life 20, no pair} -> hold (entry 8 at most); {defender, life 8} -> cast;
{defender, both out} -> cast (Rule 2); {0 creatures} -> never; {4 creatures, no pair} -> entry 8.
No two sentences disagree.

## F18 - NEW section: CLEANUP DISCARD (`vs123` seq 7 -> the -208 loss)
Finding: the new lane-W `discard` ask had no guide text. Seq 7 (t4, hand 8): `PUT: 1` = the third
Tribute to Hunger from `Tribute x3, Chromatic Lantern, Wall of Omens, Perimeter Captain x2,
Sanguine Bond`. Tributes #1/#2 ate Thraben Doomsayer (t6) and Bloodline Keeper (t11); Keeper #2
resolved t14 under Intruder Alarm with no Tribute left; t16 upkeep 57 Vampires; -208. The
bottoming order already says "Lantern or Staff first"; the cleanup ask needed the same order
plus the one hard line the loss proves.
AFTER: the section as written (Lantern, Staff, duplicate wall, spare land, Sorin, single wall;
"A Tribute to Hunger goes under only when nothing else is left to send, and NEVER while the
opponent's line or the log shows a "{T}: Put a ... token" / "{T}: Create" creature") + a
SITUATIONS bullet.

## F19 - NEW: a hand card with no `Cast` row is not castable (the three `stale_echo` fallbacks)
Finding: `vs125` seq 14, `vs162` seq 16, `vs130` seq 13 - `CHOICE: 1 (Cast Sanguine Bond)` /
`(Cast Exquisite Blood)` with the enchantment on the hand line and no row for it (pips unpayable:
`{B} 1` on the mana line at seq 14); the engine discarded the reply, Baka cast a Battlement / a
Lantern / nothing. PERCEPTION (a hand card read as a menu row). Rule 7 said "do not go outside
this list" about the ENTRY list, not about the menu.
AFTER (Rule 7 tail): "AND A CARD IN YOUR HAND WITH NO "Cast" ROW IS NOT CASTABLE THIS WINDOW. The
engine prints a row for every spell you can pay for right now; when Sanguine Bond is on your hand
line and no "Cast Sanguine Bond" row is in the menu, its {b}{b} is not there (count the {B}
sources on the mana line - one Bayou is one). Your CHOICE names a numbered row, never a card from
the hand line. Three times this corpus ..." + a SITUATIONS bullet. Engine side: seat file HIGH #1.

## F20 - Lantern counts updated (D47: 1 + 1, third corpus; no restatement, #102)
BEFORE: "three times this corpus it was the only real row ... you cast it, and two of those games
you won ... you cast a Lantern over a Wall of Omens and another over a Pride Guardian this corpus
... You cast four second Lanterns this corpus with the first still in play."
AFTER: "as the only real row beside "Cast nothing right now" it has been the right cast in game
after game ... you cast a Lantern over a Pride Guardian this corpus, the wall one entry higher ...
You cast a second Lantern this corpus after writing "I do not have a CL on the battlefield" under
a battlefield line that printed one; read the line, not the memory of it." (`vs130` seq 6; `vs146`
seq 37.)

## Recorded, NOT edited beyond a clause
- Rule 5 at `you would be at 8` or less: the all-dies blocker goes on the biggest `deals N` -
  `vs152` seq 20 put both walls on the 2-power "neither dies" attackers over the 4-power lifelink
  one at `you would be at -11` (dead to two fliers either way). One clause added to the existing
  sentence ("the 4-power lifelink one that kills the wall, not the 2-power one that does not ...")
  and a flier sentence to STEP 2 ("An A-line reading "[NONE of your available blockers can block
  this attacker ...]" is not blockable by anyone; its damage is coming through, count it" - the
  seq 20 plan wrote "now at 4 life" while 11 flying damage was on the A-lines).
- Rule 1: "none reached you this corpus" added to the engine-passes sentence (D50 0/0).
- Rule 3: a parenthesis for the countered-copy case (`vs125` seq 24/26 - D44 legal).
- Rule 6: the `[no creature they control can block this attacker]` tag named (7/7 sends).
- Rule 7a: the `vs125` Sorin -> Staff -> ping win cited (the wave-49 "45 turns" shape did not
  recur).
- Mulligan: counts updated (five 2+-land keeps; the one-land ship and its win); bottoming: the
  Staff send cited. #131 twelve-hand check: 0 lands / 1 land no coverage / 1 land coverage / all
  lands at keep-7, keep-6, keep-4 - every sentence returns ship / ship / (7,6: ship; 4: keep) /
  ship; the "would keep 5 or smaller" sentence and the zero-or-one sentence agree at keep-4 because
  the former is stated as the exception to the latter. No contradiction.
