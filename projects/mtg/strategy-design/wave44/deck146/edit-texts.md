# deck146 — wave-44 edit texts (before -> after, each with its finding)

Baseline = the LIVE guide `projects/mtg/bin/Res/ai/baka/deck146_strategy.txt` (wave-43 S1/S2/S3/S4
already applied). 5 edits, 0 deletions. Corpus: 6 games, 319 decisions, 1 fallback.

---

## EDIT 146-A (HIGH) — attack-side LIFEGAIN WALLS defeat the outnumber clause

FINDING. deck146 vs deck126 (file `1787761215-ai_baka_deck146-...-vs-ai_baka_deck126.jsonl`),
attackers seq 37 / 44 / 51 / 56 / 65 / 73. Their board was Perimeter Captain (0/4) plus one or two
Pride Guardians (0/3). Every Goblin token's tag read "(neither dies)"; the guide's outnumber clause
("send it only when the attackers you are sending OUTNUMBER their untapped blockers") licensed
sending everything, and the model did, 6 turns running. Measured cost, read off the game log:
turn 17 the opponent gained 3+2+3+2+2 = 12 life and took 6; turn 19 they gained 2+2+3+2+3+1 = 13
and netted +10 on the turn. Opponent life went 13 (T16) -> 19 -> 31 -> 42 (T22) while deck146 went
21 -> 18 and then lost at turn 26. The attacks WERE the opponent's life engine.
Card facts verified against the primitive (mtg.txt): Perimeter Captain 0/4, `auto=@combat(blocking)
source(creature[defender]|mybattlefield):may life:2 controller` — 2 life whenever ANY defender they
control blocks, not only itself. Pride Guardian 0/3, `auto=@combat(blocking) source(this):life:3
controller` — 3 life whenever it blocks. Neither gain is printed in the attack tag.
CLASSIFICATION: STRATEGY (the render was true; the guide's rule was wrong), plus a MED render item
(see the seat file, E-1: block-triggered life gain absent from the attack tag).

BEFORE (end of the outnumber bullet, after the Shield Sphere exception):
```
  ONE named exception: SHIELD SPHERE. It takes a permanent -0/-1 counter every time it
  blocks, so a "(neither dies)" line against Shield Sphere is not a wasted swing - it is
  how you kill it. Send into a Shield Sphere whether or not you outnumber them, and read
  the proof off their battlefield line as its printed toughness falls: 0/6, 0/5, 0/4.
```
AFTER — the same text, then this new bullet:
```
- OUTNUMBERING THEM DOES NOT MAKE A LIFEGAIN WALL FREE. Some 0/N defenders PAY THEM for
  blocking, and every extra body you send is one more payment: PERIMETER CAPTAIN gives them
  2 life every time ANY defender they control blocks, and PRIDE GUARDIAN gives them 3 more
  whenever it blocks. The tag will still read "(neither dies)" - it does not print their
  life gain. So before you send a "(neither dies)" body into their board, count the 0/N
  defenders on their battlefield line: if there are two or more, the swing PAYS them and
  the outnumber clause does not apply. Send only the attackers that get through (flying,
  menace, or a body whose tag kills something), keep the rest home, and win with removal,
  Kaya and the dungeon drains instead of feeding the wall.
```

---

## EDIT 146-B (HIGH) — the block ladder needs a LETHAL SURVIVAL override

FINDING. The window is on deck152's board this corpus (deck152 vs deck146 seq 20, the deciding
decision of that game) but the ladder text is WORD-FOR-WORD the same in deck146's guide, so the
same trap is armed here. At 6 life facing A1 Triumphant Adventurer deals 2, A2 Nadaar deals 7,
A3 Goblin deals 2, with exactly ONE blocker (Sigarda 4/4 flying trample) whose line offered
A1 (blocker dies), A2 (blocker dies), A3 (you kill it, blocker lives). The header said LETHAL.
Rung 1 says "TAKE THAT BLOCK, always, at every life total and every forecast number... No number
on this screen can make this block wrong." The free kill on the 2/2 Goblin was taken; 2+7 = 9
came through into 6 life and the game ended. Blocking A2 (a chump on the 7/7) survives at 2.
The ladder as written makes the losing answer the correct answer. deck146 had 1 block window this
corpus (vs152 seq 18, answered correctly) so the trap is untested HERE, but it is the same text.
CLASSIFICATION: STRATEGY (guide defect).

BEFORE:
```
BLOCK LADDER - the block window prints one parenthesis per offered block. Read it and answer from
this ladder, top down. The parenthesis is the answer; do not re-derive the combat.
```
AFTER:
```
BLOCK LADDER - the block window prints one parenthesis per offered block. Read it and answer from
this ladder, top down. The parenthesis is the answer; do not re-derive the combat.
 SURVIVAL COMES BEFORE THE LADDER. When the header says the attack IS LETHAL, the first
 question is not "which block is free" - it is "which assignment leaves me alive". Read the
 "deals N" number on each attacker, add them up, and assign your blockers so that the damage
 still coming through is LESS than your printed life. Block the BIGGEST "deals N" attackers
 first, whatever their parentheses say. A rung-1 free kill on a 2/2 while a 7/7 walks past
 you into lethal is a LOSS you chose: at lethal, the biggest number on the board outranks
 every parenthesis. Only once you are alive do you spend spare blockers by the ladder below.
```

---

## EDIT 146-C (MED) — Hive re-animation and upkeep animation

FINDING. deck146 vs deck125 (47-turn grind): 23 `becomes beholder` activations on Hive #1 and 5 on
Hive #2, at {3}{B} = 4 mana each. On turns 16, 18, 20, 22, 30, 34, 36, 40 the SAME Hive was
animated twice in one upkeep; on turns 44 and 46 four animations fired in a single upkeep.
Repro: seq 126 -> 127 (turn 44). At seq 127 the battlefield line already reads
`Hive of the Eye Tyrant #1 (3/3) [menace...]` and "of which 1 is a creature", mana 16 -> 12, and
the model answered `CHOICE: 1 (becomes beholder with Hive of the Eye Tyrant #1)` again; seq 128
shows mana down to 8 with the same Hive still the only creature. The render is HONEST here - it
prints the animated state - so this is a pure model/guide gap, not an engine item. Two Silverquill
Commands sat uncast in hand through those turns.
CLASSIFICATION: STRATEGY.

BEFORE (CLOSE THE GAME, the Hive paragraph):
```
Hive of the Eye Tyrant is a manland: pay {3}{B} - FOUR mana, not two - to make it a 3/3 menace
attacker until end of turn. That is a whole turn's mana in the mid-game, so animate it only when
you have nothing better to cast; it is a late-game mana sink, not a cheap threat.
```
AFTER — the same paragraph, then:
```
- ONE ANIMATION PER HIVE PER TURN, AND NEVER IN YOUR UPKEEP. Your battlefield line tells you
  which Hives are already animated: an animated one is printed "Hive of the Eye Tyrant #1 (3/3)
  [menace]" and is counted in the "of which N are creatures" number. Activating a Hive that
  ALREADY reads (3/3) buys you NOTHING - it is a second four-mana payment for a creature you
  already have, and it is the single most expensive mistake available to this deck. Before you
  answer "becomes beholder", find that Hive on your battlefield line: if it already shows
  (3/3), the answer is pass. Animate a SECOND Hive only if you still have four spare mana after
  the first.
- Do it in your MAIN PHASE, not in your upkeep. The animation lasts until end of turn either
  way, and spending {3}{B} before your draw step is spending it before you know what you drew.
  In upkeep, pass.
```

---

## EDIT 146-D (MED) — S1's per-creature STOP gets a TOKEN carve-out

FINDING (this is the S1 adjudication, see the seat file). deck146 vs deck123 seq 20, turn 8,
opponent at 11. Four attackers, ONE untapped blocker (Thraben Doomsayer 2/2): A1 Silencer (both
die), A2 Nadaar (you kill it), A3 Goblin token (your attacker dies, their blocker lives),
A4 Silencer (both die). S1 says leave A3 home. The model sent all four - a violation - and the
violation was CORRECT: the Doomsayer blocked the Goblin token, the token died and the three real
cards connected for 3+3+3, taking the opponent 11 -> 1 (with dark pool). Had the token stayed
home the Doomsayer blocks a Silencer instead ("both die"), deck146 loses a 3/2 card for a 2/2
and the opponent ends the turn at 4 rather than 1. S1 as written is right about CARDS and wrong
about TOKENS when you outnumber their blockers.
CLASSIFICATION: STRATEGY (guide over-reach).

BEFORE (S1 bullet, unchanged) — AFTER: the same bullet plus:
```
  THE STOP IS ABOUT CARDS, NOT TOKENS. A Goblin token from the dungeon costs you nothing
  when it dies, and their one untapped blocker can only block ONE attacker: send the token
  WITH the team and their blocker spends itself on the token instead of on a Silencer or a
  Nadaar. So when the stop-tagged body is a TOKEN and the attackers you are sending
  outnumber their untapped blockers, send it - it is bait, and the bait is the point. The
  stop stands for every real card in your deck.
```

---

## EDIT 146-E (LOW) — three new DECIDING SITUATIONS rows

FINDING. Recognition rows for edits A, B and C, in the shape the rest of that list uses.

BEFORE:
```
- You need combat math -> read the (P/T) already printed on each creature; do not rebuild it.
```
AFTER:
```
- You need combat math -> read the (P/T) already printed on each creature; do not rebuild it.
- The block header says the attack IS LETHAL -> assign blockers to the biggest "deals N"
  attackers until the damage still coming through is below your life, THEN use the ladder.
- Their battlefield shows two or more 0/N defenders (Perimeter Captain, Pride Guardian) ->
  a "(neither dies)" swing PAYS them life. Send only what gets through; hold the rest.
- The "becomes beholder" option is offered and that Hive already reads (3/3) [menace] on your
  battlefield line -> it is already animated. Pass. And never animate during your upkeep.
```

---

## NOT EDITED, and why (all four earned their tokens this corpus)
- Dungeon choice = LOST MINE by name: 16/16 CONFIRMED.
- Room branch (body-or-drain over Treasure/counter/scry): 29/29 CONFIRMED.
- "DO NOT CAST AGADEEM'S AWAKENING - EVER": 14/14 windows, never cast. CONFIRMED.
- Vanishing Verse "the only legal targets are YOUR OWN" -> do not cast: 23/23 CONFIRMED.
- Acererak-loses-to-removal: 2/2 in windows where the opponent actually controlled a creature
  (15 further windows had an empty opposing board, where the rule is not engaged). CONFIRMED.
- The MULLIGAN section is NOT touched: see the OWNER QUESTION in the seat file (deck146
  mulliganed a one-land seven against its own STEP 2 floor, and won that game).
