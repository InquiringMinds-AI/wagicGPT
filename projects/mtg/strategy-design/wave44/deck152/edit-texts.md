# deck152 — wave-44 edit texts (before -> after, each with its finding)

Baseline = the LIVE guide `projects/mtg/bin/Res/ai/baka/deck152_strategy.txt` (wave-43 S1/S2/S3
already applied). 5 edits, 0 deletions. Corpus: 6 games (2-4), 250 decisions, 3 fallbacks.

---

## EDIT 152-A (HIGH) — the block ladder needs a LETHAL SURVIVAL override

FINDING — this is the single decision that lost deck152 vs deck146.
File `1787761199-ai_baka_deck152-0x561b794e74e0-vs-ai_baka_deck146.jsonl`, kind `blockers`, seq 20,
turn 12. Header: "Your life: 6. Unblocked, these attackers deal up to 11 - you would be at -5 -
LETHAL if it all connects... block enough to survive."
  A1 Triumphant Adventurer (2/2) deals 2 [first strike, deathtouch]
  A2 Nadaar, Selfless Paladin (7/7) deals 7 [flying, vigilance]
  A3 Goblin #2 (2/2) deals 2
  B1 Sigarda, Champion of Light (4/4) [trample, flying] - may block A1 (your blocker dies,
     attacker lives), A2 (your blocker dies, attacker lives), A3 (you kill it, your blocker lives)
Answer: `BLOCKS: B1:A3`. 2 + 7 = 9 came through into 6 life and the game ended (gameend seq 21,
my_life -3). `BLOCKS: B1:A2` survives at 2 life with a 4/4 still on board.
The render was complete and correct - every number needed was printed. The guide made the wrong
answer mandatory: rung 1 reads "TAKE THAT BLOCK, always, at every life total and every forecast
number... No number on this screen can make this block wrong", and the ladder is ordered top-down
so rung 1 outranks the rung-3 chump that was the only survival. The ladder has no rule for the
one-blocker-many-attackers case and no arithmetic step at LETHAL.
CLASSIFICATION: STRATEGY (guide defect). Same text lives in deck146's guide -> mirrored there.

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
 question is not "which block is free" - it is "which assignment leaves me alive". Each
 attacker prints its own "deals N". Add them up and assign your blockers so the damage still
 getting through is LESS than your printed life, taking the BIGGEST "deals N" attackers first,
 whatever their parentheses say. A rung-1 free kill on a 2/2 while a 7/7 walks past you is a
 LOSS you chose: with ONE blocker against three attackers you do not get to pick the pretty
 block, you pick the one you survive. Only once you are alive does the ladder below decide
 anything.
```

---

## EDIT 152-B (HIGH) — "the only legal targets are YOUR OWN" is a STOP on Fateful Absence

FINDING. deck152 vs deck125, ask seq 33, turn 17. Opponent at 11 life with ZERO creatures on
board. The only option: `Cast Fateful Absence {1}{w} - the only legal targets are YOUR OWN right
now: Luminarch Aspirant {1}{w} (creature 2/2)`. The model cast it and destroyed its own only
creature, with the stated reasoning "Remove Luminarch Aspirant to prevent Path to Exile from
fetching a land... we deny the opponent the land fetch benefit". At seq 34 its battlefield reads
"of which 0 are creatures" and its hand is empty. It had DECLINED the same option twice (seq 31,
seq 32) before taking it - a repeated-window drift, not a one-off.
The surface named the trap in plain words; deck146's guide has exactly this stop for Vanishing
Verse and honoured it 23/23 this corpus. deck152's guide has no equivalent line.
CLASSIFICATION: STRATEGY (missing teach; the identical teach is 23/23 on the sibling deck).

BEFORE:
```
- FATEFUL ABSENCE ({1}{W}, instant): destroys a creature or planeswalker (they get a Clue). Save
  it for their best creature or a planeswalker. This is the ONE card worth holding with mana up.
```
AFTER — the same two lines, then:
```
  THE OPTION LINE'S TARGET LIST IS THE ONLY TRUTH ABOUT WHAT IT CAN HIT, and it has one reading
  that is always a STOP: when the line says "the only legal targets are YOUR OWN right now",
  DO NOT CAST IT. There is nothing of theirs to kill; casting it destroys your own creature and
  hands them a Clue for the privilege. Do something else this turn and keep the card - the
  option comes back the moment they play a threat. Never build a reason to fire it at your own
  board ("it dies to their removal anyway", "it denies them the fetch") - those are always
  wrong, and a board you emptied yourself is a clock you no longer have.
```

---

## EDIT 152-C (HIGH) — counters go on a body that can CONNECT

FINDING. deck152 vs deck162 (a loss). Luminarch Aspirant's combat trigger fired 8 times and the
model aimed every single one at Luminarch Aspirant #1: seq 8 (2/2), 9 (3/3), 14 (4/4), 15 (5/5),
24 (6/6), 25 (7/7), 32 (8/8), 33 (9/9), reaching 10/10 by turn 11. From seq 10 onward its own
attack tag read `Fog Bank (0/2) (neither dies (no combat damage is dealt either way - prevented))`
every combat. Fog Bank primitive (mtg.txt): 0/2, `abilities=flying,defender`,
`auto=preventAllCombatDamage to(this)` + `from(this)` - size is irrelevant to it. The 10/10 dealt
ZERO damage in the game; deck152 lost at turn 12. The same all-in-on-one-body pattern ran in the
vs126 and vs130 games (Briarbridge Tracker to 8/9). The option list showed the alternatives every
time (Luminarch Aspirant #2, Wolf, Intrepid Adversary, Lair of the Hydra).
CLASSIFICATION: STRATEGY (the render printed the prevention clause; the guide had no placement rule).

BEFORE:
```
- LUMINARCH ASPIRANT ({1}{W}, 1/1): free +1/+1 counter each of your combats. Keep it alive -
  but "keep it alive" is about not ATTACKING it into death, never about declining a block the
  ladder tells you to take. On the block ladder the ladder wins.
```
AFTER — the same bullet, then:
```
  PUT THE COUNTER ON A CREATURE THAT CAN CONNECT, NOT ON THE BIGGEST ONE. A counter is damage
  only if the body carrying it gets through. Before you answer, read the attack tags you were
  shown last combat: if your biggest creature is being stopped every turn by the SAME blocker -
  a FOG BANK (prevents all combat damage to and from itself, so any size is stopped dead), a
  Shield Sphere, or any wall whose line reads "(neither dies)" - then every further counter on
  that creature is worth exactly zero. Grow a different body instead, preferring one they cannot
  block at all: Sigarda and Elite Spellbinder FLY. Stacking nine counters onto one grounded
  attacker facing a Fog Bank builds a 10/10 that never deals a point.
```

---

## EDIT 152-D (MED) — S1's per-creature STOP: vigilance is not a licence; tokens are bait

FINDING (this is the S1 adjudication, see the seat file). Two windows, split.
HONOURED: deck146-side seq 17 equivalent - deck152 vs deck162 seq 26/34 aside, the clean case is
deck152 vs deck146 attackers seq 16, turn 9: sole attacker `A1. Briarbridge Tracker (2/3)
[vigilance] [their untapped blockers: Goblin (2/2) (you kill it, your attacker lives);
Nadaar, Selfless Paladin (4/4) (your attacker dies, their blocker lives)]`. Opponent at 21, total
power sent 2 - the lethal check does not clear, so S1 says leave it home. The model attacked.
Nadaar blocked; game log: "Nadaar, Selfless Paladin dealt 4 damage to Briarbridge Tracker /
Your Briarbridge Tracker died". deck152 lost its only creature and the game three turns later.
S1's prediction is CONFIRMED as a rule and FAILED as behaviour; the reasoning the model reached
for was vigilance ("attacking costs nothing").
The token half comes from the sibling seat (deck146 vs deck123 seq 20) and is mirrored here.
CLASSIFICATION: STRATEGY.

BEFORE: the S1 bullet, unchanged. AFTER: the same bullet, plus:
```
  VIGILANCE IS NOT A LICENCE. "It has vigilance, so attacking costs me nothing" is FALSE
  against that tag: a vigilant attacker that dies is gone from your board for the rest of the
  game, which is far worse than being tapped for one turn. Briarbridge Tracker's vigilance
  does not change a "your attacker dies, their blocker lives" line - keep it home.
  THE STOP IS ABOUT CARDS, NOT TOKENS. A Wolf token costs you nothing when it dies, and one
  untapped blocker can only block ONE attacker: when the stop-tagged body is a TOKEN and your
  attackers outnumber their untapped blockers, send it as bait so their block is spent on the
  token instead of on a lord.
```

---

## EDIT 152-E (MED) — attack-side lifegain walls

FINDING. deck152 vs deck126, turn 11: `You declared attackers: Briarbridge Tracker` (7/8) into
Perimeter Captain; log: "Opponent used: Life with Perimeter Captain - Opponent gained 2 life
(now 17) - You lost 2 life (now 18)". Also deck152 vs deck126 attackers seq 21 with the same
Captain. Primitive verified: Perimeter Captain 0/4, 2 life whenever ANY defender they control
blocks; Pride Guardian 0/3, 3 life whenever it blocks. Neither is printed in the attack tag,
which reads only "(neither dies)" or "(you kill it, your attacker lives)".
Small in this deck's games (deck152's swings usually kill the wall) but the same wording carries
the deck146 finding, where it cost the game; shipped on both guides for one lexicon.
CLASSIFICATION: STRATEGY + MED render item (see seat file E-1).

BEFORE: the Shield Sphere exception bullet, unchanged. AFTER: the same, plus:
```
- OUTNUMBERING THEM DOES NOT MAKE A LIFEGAIN WALL FREE. PERIMETER CAPTAIN gives them 2 life
  every time ANY defender they control blocks; PRIDE GUARDIAN gives them 3 whenever it blocks.
  The tag still reads "(neither dies)" - it does not print their life gain. Count the 0/N
  defenders on their battlefield line before you swing: at two or more, the attack PAYS them
  and the outnumber clause does not apply. Send only what actually gets through (Sigarda and
  Elite Spellbinder FLY) and grind with removal instead of feeding the wall.
```

---

## NOT EDITED, and why
- #3 RULE deploy floor: 45/47 windows CONFIRMED (both misses were an Elite Spellbinder held on
  turns 12 and 14 vs deck125). Earning its tokens; left alone.
- #1 RULE mana / "if a Cast option is listed you CAN pay for it": 0 observed re-derivation
  failures this corpus.
- WEREWOLVES / DAY-NIGHT: 31/31 daybound-on-battlefield frames carried a day/night line (engine
  lane #W44-4 PASS for this seat). Guide text already says "read the line" - unchanged.
- The MULLIGAN section is NOT touched (fair-hand baseline rule): deck152 mulliganed one 0-land
  seven and kept 2/3/3/3/5-land hands otherwise, all inside its own floor.
