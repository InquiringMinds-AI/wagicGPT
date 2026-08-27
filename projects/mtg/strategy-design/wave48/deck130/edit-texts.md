# deck130 (Budde's Ponza) - wave-48 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (wave-47 revision +
boundary pass, 32,412 chars). Seven edits. 32,412 -> 36,615 chars.
Corpus: `matchups-20260827-072004`. Every number is read off a prompt, a reply or a primitive.

---

## EDIT D48-1 - Rule #2 point 1 + DECIDING SITUATIONS: a planeswalker whose loyalty a spell reaches dies to that spell before any creature is cast
**Finding.** `1787833213-ai_baka_deck130-0x55ce8937fa00-vs-ai_baka_deck162.jsonl` seq 16 (turn 9,
9 life, 6 mana): `Ob Nixilis, the Hate-Twisted ... [counters: 3x loyalty]` on their line and on
`Cast Hammer of Bogardan`'s `legal targets right now:` list; reply `Cast Siege-Gang Commander`.
Turn 11: `Opponent used: -2 ... targeting Siege-Gang Commander / Your Siege-Gang Commander died
/ You drew Stone Rain / You drew Mountain / ... dealt 1 damage (now 5)` - the second time in two
turns. Point 1 ("aim there") and cast-order entry 2 already outrank entry 4; what was missing is
the loyalty-vs-damage read and the price of ignoring it. Primitive: `Ob Nixilis ... auto=
counter(0/0,5,loyalty)`, `{C(0/0,-2,Loyalty)}: destroy target(creature) && draw:2
targetcontroller`; Hammer `damage:3`.
**Before**
```
  1. An opponent PLANESWALKER on the list -> aim there. A live planeswalker wins their game.
```
**After**
```
  1. An opponent PLANESWALKER on the list -> aim there. A live planeswalker wins their game.
     This outranks casting a creature. Its line prints "[counters: Nx loyalty]": when that N
     is at or below the damage of a spell on your menu, the spell kills it, and that cast comes
     before any body. Ob Nixilis, the Hate-Twisted at "[counters: 3x loyalty]" was on Hammer of
     Bogardan's "legal targets" list while this deck cast Siege-Gang Commander instead ...
```
Plus the first DECIDING SITUATIONS line (`"[planeswalker] [counters: Nx loyalty]" and a damage
spell on your menu deals N or more ... -> cast it at the planeswalker, before any creature`).
+900 chars.

## EDIT D48-2 - BLOCKING: one blocker per attacker, keyed to the header's "you would be at N"
**Finding.** Same file, seq 13 (13 life, header `you would be at 10 - NOT lethal`, `B1-B3. Goblin
#1-#3 (1/1) - may block A1 (your blocker dies, attacker lives) x3`, reply `BLOCKS: B1:A1, B2:A1,
B3:A1`) and seq 20 (5 life, same triple). Six Goblins for 6 life; each Goblin is 2 Siege-Gang
damage. The live line "Block where only your creature dies when the damage otherwise puts you at
5 life or less" was violated at 13 and over-applied at 5 (one Goblin stops the same 3).
**Before**
```
Block where only your creature dies when the damage otherwise puts you at 5 life or less. A Goblin
is worth more as a blocker than as 2 points of face damage.
```
**After**
```
ONE BLOCKER STOPS THE WHOLE OF ONE ATTACKER'S DAMAGE. A second and a third of yours on the same
attacker stop nothing more - they only die with the first. At 13 life this deck put all three
Goblins in front of a single 3/4 ... Read the header's "you would be at N" FIRST and write it
down:
  - N above 5 and every row on that attacker reads "(your blocker dies, attacker lives)":
    "BLOCKS: none". ...
  - N at 5 or below: ONE Goblin on the attacker with the largest "deals N"; a second Goblin only
    on a DIFFERENT attacker. Never two on one unless the row says "(both die)" for the pair.
```
+560 chars.

## EDIT D48-3 - Rule #2 step 0: SURVIVAL FIRST, a numbered carve-out above (a)
**Finding.** `1787833246-ai_baka_deck130-0x560785bc5fb0-vs-ai_baka_deck152.jsonl` seq 26 (turn
13, 8 life, 7 mana): `At X=5 - kills THEIRS: Briarbridge Tracker, Katilda, Dawnhart Prime,
Sigarda, Champion of Light, Intrepid Adversary; YOURS: Goblin x4, Siege-Gang Commander, Dwarven
Blastminer`; their line 5/5 + 5/3 + 4/5 + 3/3 = 17 power. Reply `Cast nothing` *"it kills my own
Siege-Gang Commander"* - (a) obeyed; dead at -2 next combat with the Commander alive. Amendment
#71 (an absolute that can forbid survival needs the carve-out as a numbered check above it).
**Before**: (a) was the first clause under DO NOT TAKE IT.
**After** (inserted above it)
```
     SURVIVAL FIRST, above (a) and (b) below: add the printed power - the first number of each
     "(P/T)" - of every creature on THEIR battlefield line. If that sum is at or above "Your
     life:", you do not live through their next attack without the sweep; take the X whose
     THEIRS list is longest, WHATEVER the YOURS list names. At 8 life against 5/5 + 5/3 + 4/5 +
     3/3 (17 power) this deck held Starstorm ... and was dead at -2 on the next swing ...
```
+640 chars.

## EDIT D48-4 - ATTACK FLOOR: the blocker tag in EITHER form; count the blockers it names
**Finding.** Same file, seq 24 (turn 11, 13 life): `A1. Siege-Gang Commander #1 ... [their
untapped blockers: Briarbridge Tracker (3/4) (your attacker dies, their blocker lives); Sigarda
(4/4) (your attacker dies ...); Intrepid Adversary (4/2) (both die (lifelink: they gain 4))]`;
reply `ATTACK: A1, A2-A4, A5-A7`; the Commander died, 13 -> 8, 16 -> 19. The keep-home rule was
keyed only to the collapsed `they have N untapped creatures` form (#82: branch on the token's
other state).
**Before**: "THE COLLAPSED TAG, INSIDE THE FLOOR. On a wide board ... - The tag prints a GANG
BLOCK clause, or N is 2 or more: that creature stays home ..."
**After**
```
THE BLOCKER TAG, IN EITHER FORM, INSIDE THE FLOOR. On a narrow board an attacker's tag lists
their blockers one by one - "[their untapped blockers: Briarbridge Tracker (3/4) (...); ...]";
on a wide board it collapses to "they have N untapped creatures ..." COUNT THE BLOCKERS THE TAG
NAMES, in either form - a name-by-name list of three is N = 3. ... This deck sent its Commander
into a tag naming THREE blockers ... and lost it for nothing ...
  - The tag prints a "GANG BLOCK: ..." clause, or names 2 or more blockers: ...
```
+520 chars.

## EDIT D48-5 - Rule #2 STEP 1: the `[<- most kills ...]` marker IS the answer
**Finding.** `1787833220-ai_baka_deck130-0x556f484876d0-vs-ai_baka_deck146.jsonl` seq 47: rows
`1. X = 6 {kills THEIRS: Silverquill Silencer; YOURS: none}` ... `5. X = 2 {same kills as X=6,
for 4 less mana} [<- most kills at any affordable X that costs you nothing]`; reply `X = 6`. The
marker (lane Q) was on 4 of 4 announce menus this corpus and followed on 3; STEP 2 (walk down
the collapsed run) was the rule violated. #49: key the rule to the rendered token.
**Before**: STEP 1 opened "Find the row with the LARGEST 'kills THEIRS' list whose 'YOURS:'
reads 'none'."
**After**
```
       STEP 1. If a row carries "[<- most kills at any affordable X that costs you nothing]",
               THAT row is the answer: answer its number and read nothing else ... With no
               marker printed: find the row with the LARGEST "kills THEIRS" list ...
```
+330 chars.

## EDIT D48-6 - DECIDING SITUATIONS: Talisman when a body is in hand; "hold mana" is not a play
**Finding.** `1787833231-ai_baka_deck130-0x55ee92a5bc00-vs-ai_baka_deck126.jsonl` seq 23 and 25
(turn 11, 4 lands, Siege-Gang + Rorix in hand, `3. Cast Talisman of Impulse` listed): `Cast
nothing right now` twice, *"Hold mana for opponent's turn to answer potential threats"*. The
Commander landed turn 17 instead of 13; Sanguine Bond + Exquisite Blood landed turns 17/19.
Cast-order entry 5 covers Talisman; the pilot took entry 7 with a mood sentence.
**After** (new DECIDING line)
```
- "Cast Talisman of Impulse" is listed, nothing above entry 5 of the cast order matched, and
  Siege-Gang Commander or Rorix Bladewing is in your hand -> cast the Talisman. "Hold mana for
  their turn" is not a play this deck has: nothing in the sixty but Starstorm is an instant ...
```
+430 chars.

## EDIT D48-7 - DECIDING SITUATIONS: never "Yourself"; a PLAN names the next action
**Finding.** `1787833215-ai_baka_deck130-0x5589601133c0-vs-ai_baka_deck125.jsonl` seq 97 (9 life):
Hammer of Bogardan target menu `1. The opponent (player, life 26) / 2. Yourself (player, life
9)` answered `2 (Yourself)`; seq 111-112: Starstorm cast at X=12 with the row reading `there is
no creature on the battlefield for it to damage`; both under a PLAN reading *"The game is lost
... Passing is the only legal action with no impact"* carried unchanged from seq 84 to seq 131.
**After** (two new DECIDING lines)
```
- A damage spell's target menu lists "Yourself (player, life N)" -> never. The opponent, or a
  creature or planeswalker of theirs ...
- There is no "the game is lost". A PLAN line names the NEXT ACTION - a land to destroy, a
  creature to kill, a body to add - and a plan with no action in it is handed back to you every
  window until the game ends ...
```
+720 chars. The render half (retiring an action-less carried plan) is general-suggestions R3.

---

## NOT edited, and why
- **Blastminer at 8 life** (vs152 seq 27, plan "land destruction is off" then activated): 1
  instance; the floor "8 or less" is one printed number already. Watch.
- **Rorix into Forced Fruition at 1 life** (vs162 seq 23): the next draw step was lethal
  regardless; the misread of "that player" is a render item (general-suggestions R1).
- **The vs126 endgame** (Sanguine Bond + Exquisite Blood): nothing in the sixty removes an
  enchantment; the guide's job there is tempo (D48-6), not a matchup section.
