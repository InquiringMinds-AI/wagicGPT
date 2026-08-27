# deck130 (Budde's Ponza) - wave-50 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (wave-49 revision, 41,527
chars). Nine edits, applied by exact-string replacement on a copy; 41,527 -> 48,682 chars. Corpus:
`matchups-20260827-115759`. Every number is read off a prompt, a reply, a narration line or a
primitive. Files cited by short form; full names in `wave50/seats/seat-123-130.md`.

---

## D50-1 - Hammer return: the third-corpus specimen added
**Finding.** 130v123 seq 18 (turn 10 upkeep, `Mana available: 5`, `Siege-Gang Commander` in hand):
`CHOICE: 5 (Put a card into hand with Hammer of Bogardan)` under `PLAN: ... wait, I have 5 mana,
Siege-Gang is {3}{R}{R}, so I can cast it`. No Commander that turn (seq 25: Molten Rain at 4).
D49-2's rule existed; P11 FAIL 1/1; third corpus of the shape -> the render half is seat M3.
**After** (the Hammer paragraph gains)
```
The corpus after that one it did it again: five sources, the Commander in hand, "CHOICE: 5
(Put a card into hand with Hammer of Bogardan)" over a plan that said, in its own words,
"wait, I have 5 mana, Siege-Gang is {3}{R}{R}, so I can cast it". The upkeep ask comes FIRST;
the answer to it is the hand line, not the Hammer.
```
+330 chars.

## D50-2 - cast order entry 2: a token-maker beside an Intruder Alarm dies to single-target damage THIS window, before any body
**Finding.** 130v123 seq 32 (turn 14, 6 mana): their line `Thraben Doomsayer {1}{w}{w} (2/2)` +
`Intruder Alarm`; menu rows 1-6: Rorix, Siege-Gang, Spark Spray, `Cast Hammer of Bogardan {1}{r}{r}
... legal targets right now: Thraben Doomsayer`, Molten Rain, Lay Waste -> `Cast Siege-Gang
Commander` (plan "use Hammer of Bogardan to kill Thraben Doomsayer if needed"). Their turn 15:
`Opponent used: Create human with Thraben Doomsayer (x18)`. Dead turn 18 at -34. The guide's only
maker-under-Alarm line was Starstorm-specific (D49-3); no Starstorm was in hand.
**After** (entry 2 gains)
```
    A TOKEN-MAKER BESIDE AN INTRUDER ALARM IS THE CREATURE THIS ENTRY EXISTS FOR, and it
    outranks entry 4. Thraben Doomsayer is a 2/2: Hammer of Bogardan kills it, so does a
    Spellbomb. With "Thraben Doomsayer" and "Intruder Alarm" on their line, six mana, and
    "Cast Hammer of Bogardan ... legal targets right now: Thraben Doomsayer" on the menu,
    this deck cast Siege-Gang Commander ... The maker dies THIS window; the body comes after.
```
The DECIDING Starstorm-first line gains "No Starstorm: Hammer of Bogardan (3) or a Spellbomb (2) at
the Doomsayer (2/2) THIS window, before Siege-Gang, before Rorix." +640 chars.

## D50-3 - BLOCKING: the one (both die) the Commander takes - a token-maker attacker
**Finding.** 130v123 seq 39 (blockers, 20 life): `A1. Thraben Doomsayer (2/2) deals 2`, `B1.
Siege-Gang Commander (2/2) - may block A1 (both die)`, `B2-B4. Goblin #1-#3 ... (your blocker dies,
attacker lives) x3` -> `BLOCKS: B1:A1`; both died. The guide forbids the Commander's (both die)
block; the Doomsayer had made 19 Humans and was the loop's engine - the trade was right (#109: a
violation whose alternative was worse -> the rule gains its exception, keyed to the names).
**After** (BLOCKING gains)
```
THE ONE "(both die)" THE COMMANDER DOES TAKE: the attacker is THRABEN DOOMSAYER or BLOODLINE
KEEPER - a token-MAKER. That creature is their engine and it is attacking into you; the
Commander for the maker is the trade of the game, and this deck made it, correctly, at 20
life against a Doomsayer that had already made nineteen Humans.
```
Plus a DECIDING line. +420 chars.

## D50-4 - Starstorm: "NOTHING on the board dies" / "affords only X=0" rows are never cast; "Decline - do nothing" is the way back
**Finding.** 130v162 seq 10 (turn 6, 4 mana): `Cast Starstorm {r}{r}{x} {X pricing: max affordable
X=2 (4 mana total); even at X=2 NOTHING on the board dies}` taken, plan "kill Shield Sphere" (0/6);
seq 11 `Cast Card Normally`; seq 12 `X = 1 {same kills as X=2, for 1 less mana}` on rows reading
`kills THEIRS: none; YOURS: none` - the only sweeper spent on nothing in a game lost on turn 11 to
Fate Unraveler + Ob Nixilis. #128: the strings `NOTHING on the board dies` and `affords only X=0`
were on the row and in no guide sentence. The `Choose an option` menu's third row `Decline - do
nothing` (seen at 130v125 seq 19, 130v126 seq 59) is the exit the guide never named.
**After** (the #1 RULE Starstorm entry gains)
```
                     THE CAST ROW TELLS YOU BEFORE YOU COMMIT. "{X pricing: your mana affords
                     only X=0 right now, which deals 0 damage and kills nothing}" and "{X
                     pricing: max affordable X=N ...; even at X=N NOTHING on the board dies}"
                     both mean: not this window. This deck read "even at X=2 NOTHING on the
                     board dies", wrote "Cast Starstorm at X=2 to kill Shield Sphere" (a 0/6) ...
                     AND IF YOU ARE ALREADY ON THE "Choose an option" MENU BY MISTAKE, its
                     third row reads "Decline - do nothing": that row is the way back. On the
                     X menu itself there is no way back - every row spends the card.
```
Plus a DECIDING line. +1,030 chars.

## D50-5 - rule #2 point 2: ability rows print no DIES/SURVIVES - read the toughness / loyalty yourself
**Finding.** 130v162 seq 23 (turn 12 upkeep, 4 life, two punishers): `Deal 2 damage with Siege-Gang
Commander targeting Fate Unraveler [opponent's battlefield]` taken; the Unraveler is (3/4); the
Goblin died, the Unraveler did not, the draw step killed the seat. seq 17: `Deal 2 damage with
Pyrite Spellbomb targeting Ob Nixilis, the Hate-Twisted` at `[counters: 3x loyalty]` - lived. 0 of
49 ability rows at opposing creatures carried `{right now: takes N damage - DIES/SURVIVES}`; spell
target menus do (130v146 seq 17 `Goblin (1/1) {right now: takes 3 damage - DIES}`, 130v123 seq 38
`Rorix ... SURVIVES (toughness 5)`). Seat H2 asks for the annotation; the guide teaches the read
until it ships (#124's pattern: the guide rule stays correct after the render half lands).
**After** (point 2 gains)
```
     A SPELL'S target list prints the result - "Goblin (1/1) {right now: takes 3 damage - DIES}",
     "Rorix Bladewing (6/5) {right now: takes 3 damage - SURVIVES (toughness 5)}" - and you
     read it. An ABILITY'S rows ("Deal 2 damage with Siege-Gang Commander targeting Fate
     Unraveler", ...) print NO result: there you do the read yourself, off the (P/T) or the
     "[counters: Nx loyalty]" on their battlefield line. 2 damage kills toughness 2 and loyalty
     2, and nothing bigger. ...
```
Plus a DECIDING line. +900 chars.

## D50-6 - BLOCKING: a GANG BLOCK that kills a draw punisher (or the only creature hitting you) at low life
**Finding.** 130v162 seq 22 (blockers, 7 life; `DRAW PUNISHERS ... Fate Unraveler, Ob Nixilis ...
2 life` per draw; Howling Mine on their line): `A1. Fate Unraveler (3/4) deals 3 [GANG BLOCK: your
3 biggest together deal 4, enough to kill this attacker; each B-line result below is a LONE blocker
...]`, B1 Commander + B2-B4 Goblins each `(your blocker dies, attacker lives)` -> `BLOCKS: none`,
7 -> 4; the next draw step (2 punishers x 2 draws) ended the game. The guide's "one blocker stops
the whole of one attacker's damage; a second and a third stop nothing more" is true of damage and
was read as a ban on the kill the header priced.
**After** (BLOCKING gains)
```
A GANG BLOCK THAT KILLS THE ATTACKER IS A DIFFERENT QUESTION FROM DAMAGE. The header prints
it when it exists: "[GANG BLOCK: your 3 biggest together deal 4, enough to kill this
attacker; ...]". When that attacker is a DRAW PUNISHER named on the "DRAW PUNISHERS" line,
or the only creature hitting you, and your life minus its "deals N" is 5 or less, put every
blocker the clause counts on it - the Goblins you lose are the draw steps you keep. ...
```
Plus a DECIDING line. +780 chars.

## D50-7 - COMBAT: a WALL CHECK before the attack floor (Perimeter Captain / Pride Guardian)
**Finding.** 130v126 seq 49 (turn 14 attackers, 20 vs 21): `A1. Siege-Gang Commander (2/2) [their
untapped blockers: Perimeter Captain (0/4) (neither dies (blocking trigger: they may gain 2));
Pride Guardian (0/3) (neither dies ...)]`, same on both Goblins -> `ATTACK: Siege-Gang Commander,
Goblin, Goblin`; narration `Opponent gained 2 (23) ... 3 (26) ... 2 (28) ... 2 (30)` - nine life
in one combat, with Exquisite Blood on their line. deck123's guide carries this check; deck130's
attack floor had no lifegain-wall case at all.
**After** (new paragraph before ATTACK FLOOR)
```
WALL CHECK, BEFORE THE FLOOR: their battlefield line names PERIMETER CAPTAIN or PRIDE
GUARDIAN. Their tags say what a block costs you - "(neither dies (blocking trigger: they may
gain 2))" - and every one of your attackers they block is life for them ... "ATTACK: none"
while either name is on that line; they die to a Spellbomb, a Goblin sacrifice or a Starstorm
first, and THEN the floor applies.
```
Plus a DECIDING line. +650 chars.

## D50-8 - rule #2 STEP 3: decide (a) and (b) at the cast row; the X menu has no exit
**Finding.** 130v126 seq 58-60 (turn 16, Commander on the line): the cast row's pricing showed the
kills; `Cast Card Normally`; X menu `X = 6 {kills THEIRS: Pride Guardian, Overgrown Battlement;
YOURS: Siege-Gang Commander}`, X=5/4 same, `X = 3 {kills THEIRS: Pride Guardian; YOURS: Siege-Gang
Commander}`, `X = 2 {none; YOURS: Siege-Gang Commander}`, `X = 1 {none; none}`; answered X=3 under
a plan reading "without killing Siege-Gang Commander" - the Commander died for a 0/3. seq 65-67
(turn 18): the second Starstorm at X=4 for a lone `Overgrown Battlement (0/4) [defender]` - rule
(b) to the letter. Both rules existed (D48/D49); second corpus of (a). #118: re-key to where the
decision is still open - the cast row - and name the exit.
**After** (STEP 3 gains)
```
               DECIDE (a) AND (b) AT THE CAST ROW, NOT ON THE X MENU. The cast row's own
               "{X pricing: ... At X=N - kills THEIRS: ...; YOURS: ...}" prints the same two
               lists; once you have answered "Cast Card Normally" the X menu has no "cast
               nothing" row and every row spends the card. ...
```
Plus two DECIDING lines (YOURS names the Commander at every killing X -> no `Cast Card Normally`;
`Decline - do nothing` on a menu reached by mistake). +900 chars.

## D50-12 - the life floor gains the Talisman payment specimen
**Finding.** 130v123 seq 55-56 (turn 18, 2 life): `Cast Molten Rain` at Arcane Sanctum; receipt
`- Paid {1}{r}{r} for Molten Rain with Mountain #1, Talisman of Impulse #1, Talisman of Impulse #2`;
`me` 2 -> 1. The 8-or-less floor existed; the CAUTION line the prompt prints was not keyed.
**After** (the floor gains)
```
At 2 life this deck cast Molten Rain off two Talismans of Impulse - the "Paid" line read
"Talisman of Impulse #1, Talisman of Impulse #2" - and went to 1 for a land that changed
nothing. When the screen prints "CAUTION - some usable mana sources DAMAGE YOU when tapped
for mana" and your life is 3 or less, a spell that needs the Talisman costs life you do not
have.
```
Plus a DECIDING line. +420 chars.

---

## Not edited, recorded
- **130v152 seq 15-16**: Pyrite Spellbomb cast over Molten Rain / Stone Rain at 3 lands (their board
  3 lands), then `cycling with Lay Waste` at 4 lands with 2 mana - both rules exist ("3 lands: STONE
  RAIN or MOLTEN RAIN - do not cast a Spellbomb first"; "Cycle Lay Waste only once you control 8 or
  more lands"); second corpus, one window each; the cycling row's `{if you pass here, this option is
  not offered again this turn}` tag is seat L1 (inference).
- **Goblins into a lone `(your attacker dies, their blocker lives)` blocker** (130v152 seq 31 three,
  seq 39 two; one died each time): the "a GOBLIN goes anyway" clause and the floor's "none when every
  attacker would die and kill nothing" meet here; two connected each time; not decisive; recorded.
- **Spellbomb at the face at 20 with 0 creatures on their line** (130v146 seq 7): the 6-or-less floor
  broken once in a won game; the P15 shape (a creature on their line) had no violation.
- **Spellbomb draw mode with two defenders out** (130v126 seq 26): the rule says 0 creatures; minor.
- **Forgotten Cave cycled with lands in hand** (130v123 seq 7, 130v162 seq 3): the land drop was
  already used both times; harmless.
- **Bottoming at 130v126 seq 3**: a Mountain bottomed from 3 lands + 4 spells, leaving two
  enters-tapped Caves; the order says the Commander (rest by cost) goes; non-decisive.
