# deck152 - wave-53 edits (before -> after, each with its finding)

Live guide under test: `bin/Res/ai/baka/deck152_strategy.txt` (53,727 B). Revised: 57,075 B
(+3,348). Corpus `matchups-20260902-023342`. Card facts verified with `/usr/bin/grep` against
`bin/Res/sets/primitives/*.txt`.

---

## F1 - the converter attack rule holds attackers that cannot be blocked (152-A, HIGH)

**Finding.** `152v126` (L 0/21, T16), three consecutive combats:
- s26 (T11, **31 life vs 17**): four A-lines. `A1. Luminarch Aspirant (4/4) [their untapped
  blockers: Wall of Omens (0/4) (you kill it, your attacker lives); Vampire (1/1) (you kill it,
  your attacker lives (lifelink: they gain 1, and their converter takes 1 off you))]`;
  `A2. Intrepid Adversary #1 (7/5) [lifelink] [...]`; `A3. Sigarda (7/7) [no creature they
  control can block this attacker]`; `A4. Brutal Cathar (4/4) [...]`. **Two** distinct untapped
  blockers were named. Answer: `ATTACK: A3, A4` - the 4/4 and the 7/5 held. Reply: *"Intrepid #1
  and Luminarch are blocked by Walls/Vampires that trigger Sanguine Bond via lifelink/blocking
  triggers, draining us while dealing no damage"*.
- s32 (T13, 25 vs 16): same shape, Luminarch (4/4) and Intrepid (5/3) held, two blockers named.
- s40 (T15, 17 vs 16): Luminarch (3/3) and Brutal Cathar (3/3) held, two blockers named.

Their life went 17 -> 16 -> 16 across those three combats; the seat's went 31 -> 25 -> 17 -> 0.
With four attackers against two named blockers, at least two are unblocked whatever they choose,
and the price the tags actually print is 1 or 3 life.

**Why.** The rule as written is an absolute keyed to a substring: *"send every creature whose
A-line prints no `(blocking trigger:` and no stop ... and hold every creature whose tag prints
one."* The tag lists EVERY potential blocker, so one gain clause anywhere in it holds the whole
attacker - even when the same tag's other entry reads `(you kill it, your attacker lives)`, and
even when the defender has fewer blockers than the pilot has attackers. Wave 52's fix closed the
`ATTACK: none` half (P5 PASS this wave: 0 `ATTACK: none` answers at deck152 in 22 windows,
26 empty-tag A-lines, 0 missed) and left this one.

**BEFORE**
```
  RACE MEANS DAMAGE THROUGH, NOT ATTACKERS DECLARED, AND THE CONVERTER LINE NEVER LICENSES A
  SWING THE TAGS PRICE AGAINST YOU. A creature you send into a "(blocking trigger:" wall deals
  ZERO to their life and hands them N, which the converter takes off you as well - it moves the
  race backwards in both directions at once. So with a converter of THEIRS on the line: send
  every creature whose A-line prints no "(blocking trigger:" and no stop - your fliers first -
  and hold every creature whose tag prints one.
```
**AFTER** (the first three sentences are kept verbatim; the arithmetic and the closing
instruction are new):
```
  RACE MEANS DAMAGE THROUGH, NOT ATTACKERS DECLARED, AND THE CONVERTER LINE NEVER LICENSES A
  SWING THE TAGS PRICE AGAINST YOU. A creature you send into a "(blocking trigger:" wall deals
  ZERO to their life and hands them N, which the converter takes off you as well - it moves the
  race backwards in both directions at once.
  BUT A GAIN CLAUSE INSIDE A TAG IS A PRICE, NOT A PROHIBITION, AND ONE NAME IN THE TAG CANNOT
  BLOCK EVERYTHING. Each A-line lists "their untapped blockers: <name>; <name>; ...". COUNT THE
  DISTINCT NAMES IN THAT LIST - call it B. That is how many of your attackers they can block AT
  ALL this combat. If you declare more than B attackers, at least (your attackers minus B) of
  them are UNBLOCKED and their full power lands on the opponent's life, and the gain clauses on
  the other tags never fire at all. So the hold is for the creature whose EVERY listed blocker
  prints a gain clause AND stops its damage - never for a creature whose tag contains even one
  entry reading "(you kill it, your attacker lives)", which is a blocker traded away for a
  printed price of 1 or 3 life.
  WHAT THIS COST, one game, three combats: [the s26/s32/s40 record, as above]
  So with a converter of THEIRS on the line and only ONE of the two names printed: send
  every creature whose A-line prints no "(blocking trigger:" and no stop - your fliers first -
  then keep sending until the number of attackers you have declared is LARGER than B, taking the
  cheapest printed price you are offered; and hold only the creature whose whole blocker list
  stops it AND pays them.
```

**F1b - the matching DECIDING SITUATIONS bullet (#177).** New bullet inserted above the existing
"Their line shows ONE of the two names" bullet:
```
- Their line shows ONE converter name, you have more attackers than there are distinct names in
  "their untapped blockers", and the surplus tags read "(you kill it, your attacker lives)" ->
  DECLARE THEM ALL. The surplus cannot be blocked and the price on the rest is the 1 or 3 life
  the tag prints.
```

---

## F2 - the coverage literal has no scope on the TWO-OR-MORE rung (152-B, HIGH, third corpus)

**Finding.** `152v162` s1: seven cards, `counted by the engine: 2 lands, 5 spells`,
`Mana sources among those lands, counted by the engine: {G} 2`, hand = 2 Pathways + 2 Luminarch
Aspirants + 2 Brutal Cathars + Elite Spellbinder, header line `Playing every land in this hand
would not cover any spell in it`. Answer: **MULLIGAN**, reply *"Mulligan for a hand with at least
one white mana source to cast Luminarch Aspirant, Elite Spellbinder, or Brutal Cathar on curve."*
That is THE FORBIDDEN THOUGHT written word for word, against a floor that reads *"TWO OR MORE
LANDS AND AT LEAST ONE SPELL: KEEP. That is the floor and it is not a close call."* Same shape as
wave 52's `152v126` s1 (2 lands {G}2, colour reason) and wave 51's - third corpus.

**Why the existing text does not hold it.** #171(a): the guide quotes the coverage literal ONLY
under its ONE LAND rung. The TWO-OR-MORE rung never mentions the literal at all, so the most
concrete sentence on the screen has no scope attached to it anywhere near the rung it is being
read on. deck146's guide carries exactly that scoping sentence and shipped **0 of N** two-and-
three-land sevens this corpus (its two mulligans were legal one-landers); deck152's does not and
shipped one. Also note WHY the literal fires here: the two lands make {G} and every spell needs
{W}, so `would not cover` is a COLOUR statement wearing a land-count sentence's clothes, and it
points the same way as the forbidden thought.

**BEFORE**
```
- TWO OR MORE LANDS AND AT LEAST ONE SPELL: KEEP. That is the floor and it is not a close call. If
  the "would cover the cost of" line names even one card, it is not close at all. ALL LANDS: ship -
  it casts nothing.
```
**AFTER** (floor kept verbatim; the scoping paragraph is new, ported from deck146 and given this
deck's colour reason)
```
- TWO OR MORE LANDS AND AT LEAST ONE SPELL: KEEP. That is the floor and it is not a close call. If
  the "would cover the cost of" line names even one card, it is not close at all. ALL LANDS: ship -
  it casts nothing.
  "Playing every land in this hand would not cover any spell in it" IS A ONE-LAND TEST AND
  NOTHING ELSE. Read the engine's land count FIRST and let it decide which rung you are on. At
  TWO OR MORE lands that sentence is not a ship and never was, and it is printed most often for a
  reason that is not about land count at all: your two lands make one colour and your spells need
  the other. That is the ordinary two-colour opener, it is a KEEP, and the coverage line does not
  say otherwise - it only says nothing is castable on turn ONE, which was already true of a hand
  whose cheapest card costs TWO.
  WHAT THIS COST, in two corpora running: a seven of two Pathways ({G} 2) plus two Luminarch
  Aspirants, two Brutal Cathars and an Elite Spellbinder was shipped under that header line, the
  reply reading "Mulligan for a hand with at least one white mana source" - THE FORBIDDEN
  THOUGHT below, written word for word, on a hand this floor keeps.
```

---

## F3 - the `{paying this taps:` stop is scoped by the number of names in the clause (152-C)

**Finding.** THREE-NUMBER AUDIT for this literal at deck152: rows carrying
`{paying this taps: <names> - they cannot attack this turn}` **14** (12 windows), rows carrying
lane O's new defender form `- it cannot block on their turn` **0** (deck152 controls no
defenders; the new form renders 27 times corpus-wide, all on deck126's Overgrown Battlement, and
the old form renders **0** times on a defender - lane O PASS), takes **2**. Both takes are in
**Main phase 1**, so wave 52's P4 ("taken in Upkeep or Main 1: 0/N") is a FAIL of 2 - down from
3 of 10. But read them:
- `152v130` s11 (T9): took `Cast Ranger Class {1}{g} ... {paying this taps: Katilda, Dawnhart
  Prime - it cannot attack this turn}` - ONE name, and Ranger Class makes a 2/2 Wolf.
- `152v130` s14 (T11): took `Cast Sigarda, Champion of Light ... {paying this taps: Katilda,
  Dawnhart Prime - it cannot attack this turn}` - ONE name, for a 4/4 flying trample lord.
The seat WON that game 11 to -4. The rows it correctly passed were the two- and three-name ones
(`Luminarch Aspirant, Intrepid Adversary`, `Katilda, Dawnhart Prime, Elite Spellbinder`) and the
one-name ones attached to a level counter or a removal spell - `152v162` s29/s32/s43/s44 passed a
Katilda counter activation four times, which is the exact break wave 52 was written against.
Per #157: every break shares a printed fact the rule did not name - the clause holds ONE name and
the row puts a bigger body on the board than the one being tapped.

**BEFORE**
```
THAT CLAUSE IS AN
  UPKEEP-AND-MAIN-1 STOP. Every name inside it is a creature you are giving up this combat, and
  one more 1/1 body is never worth two attackers removed. In Upkeep or Main 1 a row carrying
  that clause is PASS: attack first, then take the same row in Main 2, when the clause costs you
  nothing.
```
**AFTER**
```
COUNT THE NAMES INSIDE THAT
  CLAUSE - THAT COUNT IS THE WHOLE STOP. Every name inside it is a creature you are giving up
  this combat. In Upkeep or Main 1, a row whose clause names TWO OR MORE creatures is PASS,
  always, whatever the row is: attack first, then take the same row in Main 2, when the clause
  costs you nothing. With exactly ONE name inside the clause the trade is real and you read it -
  taking the row is right when the row puts a creature onto the battlefield whose printed power
  is 2 or more (Sigarda, a Ranger Class Wolf, a Tracker), because you are trading one tapped
  1/1 for a bigger body; it is PASS when the row is a level, an activation, a removal spell or
  anything else that leaves no body behind.
```

---

## Considered and NOT made

- **The Spellbinder duplicate pick.** `152v123` s40: the exile list held `Lightning Greaves`,
  `Tribute to Hunger`, **three** identical `Idyllic Tutor` rows and `Damnation`. The guide's
  ordered rule (neither Bond nor Blood on the list -> take IDYLLIC TUTOR) was cited and followed;
  exiling one of three copies changes nothing, but the seat was facing twenty flying 2/2 Vampires
  and was dead the following turn either way. One window, no punishment attributable to the pick;
  recorded under #118, not written.
- **The block floor.** P7 PASS: 9 block windows across the three seats, `BLOCKS: none` twice
  (both at "you would be at" 15 and 19, i.e. above the floor), 0 repeated B-numbers, 0
  `multiblock_reask`, 0 replies claiming first strike beats a blocking trigger. No edit.
- **The deploy floor.** No `Cast nothing right now` on an own main phase with an untagged
  creature row this corpus (19 such answers, every one a Fateful Absence held as an instant -
  branch (a) of the rule). No edit.
