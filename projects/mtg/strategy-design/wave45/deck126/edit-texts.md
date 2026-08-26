# wave45 deck126 - guide edits, before -> after
Base: the LIVE guide `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` as of this wave.
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-131833/`. Every card fact below is
read off `projects/mtg/bin/Res/sets/primitives/*.txt`.

## F1 land-drop rule added as RULE #0
**Finding.** 2 missed turn-level land drops in 41 offered turns, BOTH in the vs146 LOSS (turns 7 and 13); 8 declines across 47 land windows. At turn 13 (seq 20/21) the hand was Scrubland, Forest, Overgrown Battlement, Swamp, Woodland Cemetery on a four-land board of Scrublands + Isolated Chapel - no green source, so the Battlement in hand was uncastable, and the Forest that fixed it was declined twice. Stated reason in the reply: 'I will pass this turn to preserve mana.' Dead on turn 14. STRATEGY: the render prints 'Land drop: NOT yet used this turn' and each hand land's colour.

**BEFORE**
```
=== RULE #1 - AT YOUR UPKEEP, PASS. THIS IS THE MOST IMPORTANT LINE IN THIS GUIDE ===
```

**AFTER**
```
=== RULE #0 - PLAY THE LAND. EVERY TURN. ===
When the prompt asks "Land drop:" and lists "Play <land>" options, TAKE ONE. Every turn, at any
life total, with any board. "Play no land right now" is never your answer while a land is listed,
and "I will hold it to preserve mana" is not a thought that means anything - playing a land costs
nothing and taps nothing.
Which land: take the one whose colour your hand needs and your battlefield does not already have.
The hand line prints it for you ("Forest (land: taps for {G})"), and the mana line prints, one per
untapped card, what each source you already control can make.
You lost a game to this. Holding a Forest and a Woodland Cemetery in hand, you passed the land drop
on two turns, stayed on four lands that made only {W} and {B}, never cast the Overgrown Battlement
in your hand because you had no green source, and died on turn 14 with five lands in hand.

=== RULE #1 - AT YOUR UPKEEP, PASS. THIS IS THE MOST IMPORTANT LINE IN THIS GUIDE ===
```

## F2 coloured pips come from lands/Lantern, never from the Battlement
**Finding.** vs130, turn 14, seq 30-34. Tutored Sanguine Bond ({3}{B}{B}; primitive mana={3}{B}{B}), then took 'Add 5 green mana with Overgrown Battlement' TWICE (seq 32, 33) with the mana sub-list reading 'Scrubland {w} or {b}; Overgrown Battlement #1 {g} (VARIABLE output...)' x3 - one black source. With 10 floating green it passed at seq 34 without casting anything, and cast the Bond only two turns later after playing a Swamp. The two tapped Battlements could not block on the opponent's turn; life went 6 -> 1. Primitive: Overgrown Battlement auto={T}:foreach(creature[defender]|myBattlefield) add{G} - green only. The existing 'it is probably affordable, let the engine tap for you' line is true of generic costs and false of pips, and it is the line that licensed this.

**BEFORE**
```
number is a FLOOR: your real mana is that number plus one for every defender past the first. When
a spell looks one or two mana out of reach and you have three or four walls out, it is probably
affordable - take it and let the engine tap for you.
```

**AFTER**
```
number is a FLOOR for the GENERIC part of a cost: your real generic mana
is that number plus one for every defender past the first. When the part of a cost that is a plain
number ({3} in {3}{B}{B}, the {2} in {2}{W}) looks one or two out of reach and you have three or
four walls out, it is probably affordable - take it and let the engine tap for you.
THE COLOURED PIPS ARE A DIFFERENT COUNT, AND THE BATTLEMENT PAYS NONE OF THEM. Overgrown
Battlement makes {G} and only {G}. It cannot pay the {B} in Sanguine Bond {3}{B}{B}, the {B} in
Exquisite Blood {4}{B}, the {B} in Tribute to Hunger {2}{B} or the {W} in Idyllic Tutor {2}{W}.
Those come from your LANDS, and from Chromatic Lantern.
Before you plan any spell with a coloured pip, read the sub-list under the mana line - "Those
sources, one per untapped card: Bayou {b} or {g}; Scrubland {w} or {b}; Overgrown Battlement {g}
(VARIABLE output ...)" - and COUNT the untapped cards whose colour list contains the pip you need.
Sanguine Bond needs TWO of them to say {b}. One Scrubland is one, not two.
This is not theory. In a game you lost you tutored up Sanguine Bond, tapped two Battlements for ten
floating green mana with one black source on the board, could not cast it, passed - and the two
walls you tapped could not block on their turn. You went from 6 life to 1 that turn.
```

## F3 two-question stop before any Battlement tap
**Finding.** 39 mana-only Battlement windows this corpus (vs125 14, vs162 8, vs130 17), passed 37/39. The 2 takes are the vs130 incident in F2. Rule #1's existing test ('right only when the very next thing you do in that same decision is cast a spell with that mana') was satisfied in the model's own reasoning - it INTENDED to cast the Bond - and still failed, because the missing check was the colour one. Making the stop two explicit questions, the second pointed at the pip count, closes it.

**BEFORE**
```
The ONE time you take it: you are casting Tribute to Hunger in that same window and need the mana
to pay for it right now.
```

**AFTER**
```
The ONE time you take it: you are casting Tribute to Hunger in that same window and need the mana
to pay for it right now.
Before you take a "Add N green mana with Overgrown Battlement" option in ANY window, answer these
two out loud, and pass unless BOTH are yes:
  1. Can you NAME the spell you are about to cast with it, in this same decision?
  2. Are that spell's COLOURED pips covered by your untapped LANDS (Rule #1's pip count) without
     the Battlement's green?
If either answer is no, the green is wasted and you have turned a 0/4 blocker into a tapped card.
```

## F4 Tribute takes an unblockable-by-your-board lone creature at any life total
**Finding.** vs162 LOSS, turn 10. At seq 18 (turn 8, life 13, 6 mana) the opponent's ONLY creature was Master of the Feast (primitive: abilities=flying, 5/5, 'At the beginning of your upkeep, each opponent draws a card') alongside Underworld Dreams ('Whenever an opponent draws a card, deals 1 damage') - 5 combat damage plus 2 per turn cycle. The menu offered both Sanguine Bond {3}{B}{B} and Tribute to Hunger {2}{B}, and 6 mana pays for one. The model cast the Bond, and its stated PLAN read 'Block Master of the Feast with all walls' - an impossible block: no card in this deck has flying or reach. Dead on turn 10. Rule 7b's life<=10 gate did not fire at 13. STRATEGY; the [flying] tag was on the board line.

**BEFORE**
```
 - The opponent controls exactly ONE creature - then their choice is your choice.
 - You need the life right now to survive the turn.
```

**AFTER**
```
 - The opponent controls exactly ONE creature - then their choice is your choice.
 - THE OPPONENT'S ONLY CREATURE IS ONE YOUR BOARD CANNOT BLOCK. None of your sixteen defenders
   and none of your Vampire tokens has FLYING or REACH, so a creature tagged [flying] on the
   opponent battlefield line connects for its full power every single turn no matter how many
   walls you have. Your walls do not answer it; Tribute to Hunger is the only card in this deck
   that does. When it is their ONLY creature, Tribute takes it - at ANY life total, in the first
   window Tribute is offered, ahead of Exquisite Blood and ahead of Sanguine Bond.
   The thought "I will block it with all my walls" is ALWAYS FALSE against a [flying] attacker.
   You lost a game on turn 10 to exactly this: their only creature was Master of the Feast (5/5
   [flying]), you were at 13 life, you had six mana and both Tribute to Hunger and Sanguine Bond
   in hand, and you cast the Bond. Tribute would have eaten the Master - their only creature, so
   no choice for them - and gained you 5.
 - You need the life right now to survive the turn.
```

## F5 survival floor 7b gains the unblockable-creature branch
**Finding.** Same window as F4. D12 (life<=10 + one creature + Tribute + mana for one) produced ZERO windows this corpus - the floor is written on a condition that does not arise. The condition that DID arise, twice-shaped (vs162's flier; vs146's Nadaar/Adventurer board), is 'their clock is something my walls do not stop'.

**BEFORE**
```
7b. THE SURVIVAL FLOOR, and it outranks entries 1 and 2 of this list. When ALL of these are true -
    your life is 10 or less; the opponent battlefield line shows exactly ONE creature; "Cast Tribute
    to Hunger" is in this menu; and you cannot pay for BOTH Tribute and the combo piece out of the
    "Mana available:" line - then cast TRIBUTE TO HUNGER first, even over Exquisite Blood or
    Sanguine Bond.
```

**AFTER**
```
7b. THE SURVIVAL FLOOR, and it outranks entries 1 and 2 of this list. When the opponent battlefield
    line shows exactly ONE creature, "Cast Tribute to Hunger" is in this menu, you cannot pay for
    BOTH Tribute and the combo piece out of the "Mana available:" line, and EITHER of these holds -
      (a) your life is 10 or less, or
      (b) that one creature is tagged [flying] (or anything else your walls cannot block - none of
          them has flying or reach), whatever your life total is
    - then cast TRIBUTE TO HUNGER first, even over Exquisite Blood or Sanguine Bond.
```

## F6 Lantern gate becomes a two-black-lands test, not a colours-line test
**Finding.** D13 predicted 0 Lantern casts with {b} already on the 'colours you can make:' line; 2 of 14 windows cast it anyway (vs125 seq 10, vs130 seq 15, both at colours {g}{b}{w}). Graded by odds (amendment #47) both casts were RIGHT: the deck's key spell is {3}{B}{B} and one black source pays half of it, while the colours line answers only 'any black at all'. Primitive: Chromatic Lantern auto=lord(land|mybattlefield) transforms(... add{B} ...) plus its own {T}:Add{B} - it makes every land a black source. Savannah re-verified as type=Land subtype=Forest Plains (no black); Sunpetal Grove auto={T}:Add{G} / Add{W} (no black). The rule was the defect, not the play.

**BEFORE**
```
 7. Chromatic Lantern - only when BOTH are true: the "colours you can make:" line does NOT include
    {b}, AND no Chromatic Lantern is already on your battlefield line. The lands of yours that make
    black are Bayou, Scrubland, Woodland Cemetery, Isolated Chapel, the Swamp and Urborg. SAVANNAH
    AND SUNPETAL GROVE DO NOT - they are {G}/{W} only - so an opening of Savannahs, Sunpetal Groves
    and Plains is exactly the board where this card earns its slot. When {b} is already on the
    "colours you can make:" line the Lantern does nothing for 3 mana. A SECOND Lantern does nothing
    at any time - once one is on your battlefield this entry is finished for the rest of the game,
    so take a wall or a combo piece instead.
```

**AFTER**
```
 7. Chromatic Lantern - only when BOTH are true: FEWER THAN TWO of the LANDS on your battlefield
    can make {b}, AND no Chromatic Lantern is already on your battlefield line. Two, not one:
    Sanguine Bond costs {3}{B}{B} and one black land pays half of it. Count them off the battlefield
    line by name - the lands of yours that make black are Bayou, Scrubland, Woodland Cemetery,
    Isolated Chapel, the Swamp and Urborg. SAVANNAH AND SUNPETAL GROVE DO NOT - they are {G}/{W}
    only - and Overgrown Battlement is not a land and makes only {G}. So an opening of Savannahs,
    Sunpetal Groves and Plains is exactly the board where this card earns its slot, and so is a
    board with one lone Scrubland.
    Do not use the "colours you can make:" line for this test. That line says whether you can make
    {b} AT ALL, not how many black sources you have, and your key spell needs two.
    A SECOND Lantern does nothing at any time - once one is on your battlefield this entry is
    finished for the rest of the game, so take a wall or a combo piece instead.
```

## F7 block tiebreak restated as a read-the-number step
**Finding.** vs152 seq 15, turn 12, life 12. Offered A1 Luminarch Aspirant (3/3) deals 3, A3 Wolf (2/2) deals 2, both '(neither dies)' on the single Wall of Omens B-line; A2 Sigarda carried 'NONE of your available blockers can block this attacker'. The model blocked the Wolf and took 3. Rule #5's tiebreak already said to take the biggest 'deals N' but said it inside a 90-word sentence. Blocks otherwise: 5 windows, 8/8 offered blockers assigned, zero 'BLOCKS: none'.

**BEFORE**
```
When two or more attackers all let that blocker live, take the one with the
BIGGEST "deals N" number on its A-line that no other blocker of yours is already assigned to -
one blocker per attacker first, as above; this tiebreak only chooses among attackers
still unblocked.
```

**AFTER**
```
When two or more attackers all let that blocker live, READ THE "deals N" NUMBER OFF EACH OF THEIR
A-LINES AND TAKE THE BIGGEST - not the one whose name sounds smaller, not the one you blocked last
turn. One blocker per attacker first, as above; this tiebreak only chooses among attackers still
unblocked. You have got this wrong: offered "A1. Luminarch Aspirant (3/3) deals 3" and
"A3. Wolf (2/2) deals 2", both reading "(neither dies)", you blocked the Wolf and took 3.
```

## F8 situations list: land drop, mana-only pass, flying-creature Tribute
**Finding.** Evidence as F1, F3, F4. The wave-44 pattern (a rule absent from the situations list does not fire) is why each of the three is repeated here.

**BEFORE**
```
 - Your Upkeep, and the only offered action taps Overgrown Battlement for mana: answer CHOICE: 0 (pass)
   and keep the wall untapped for their turn.
```

**AFTER**
```
 - The prompt lists "Play <land>" options: play one. Every turn, no exceptions.
 - Your Upkeep, and the only offered action taps Overgrown Battlement for mana: answer CHOICE: 0 (pass)
   and keep the wall untapped for their turn.
 - ANY window whose only offered actions are "Add N green mana with Overgrown Battlement": pass,
   unless you can name the spell you are casting with it in this same decision AND its coloured
   pips come from your untapped lands.
 - The opponent's only creature is tagged [flying] and Tribute to Hunger is listed: cast Tribute,
   at any life total. Your walls cannot block it.
```

## F8b situations list: pip counting
**Finding.** Evidence as F2.

**BEFORE**
```
 - The opponent has exactly one creature and you need life: cast Tribute to Hunger.
```

**AFTER**
```
 - The opponent has exactly one creature and you need life: cast Tribute to Hunger.
 - You are about to plan a spell with a {b} or {w} in its cost: count the untapped LANDS whose
   colour list on the mana line contains that symbol. The Battlements' green pays none of it.
```
