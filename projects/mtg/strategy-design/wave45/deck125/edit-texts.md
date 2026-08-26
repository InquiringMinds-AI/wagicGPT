# wave45 deck125 - guide edits, before -> after
Base: the LIVE guide `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` as of this wave.
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-131833/`. Every card fact below is
read off `projects/mtg/bin/Res/sets/primitives/*.txt`.

## W1 land-drop rule added as #0
**Finding.** 14/81 turn-level land drops declined across 6 games (vs146 turns 9,11,15,17,19,21,54; vs126 20,22,28; vs162 14,20; vs123 42,43). In the vs126 LOSS the declines pushed the hand to 7+ and forced a Cancel discard on turn 23; the opponent's Sanguine Bond then resolved uncountered on turn 28 with the hand empty of answers. In the vs162 LOSS drops were declined on turns 14, 18 and 20; on turn 20 the model spent its only counter on Ob Nixilis and had no mana left for Teferi's Puzzle Box, which resolved and (with Underworld Dreams) took 12 life in one turn. Windows: 105 land-drop windows, 38 declines. STRATEGY - the render prints 'Land drop: NOT yet used this turn' and lists every playable land.

**BEFORE**
```
=== #1 - STAFF OF NIN IS YOUR WIN CONDITION. EMRAKUL IS NOT YOUR PLAN. ===
```

**AFTER**
```
=== #0 - PLAY THE LAND. EVERY TURN. BEFORE ANYTHING ELSE. ===
When the prompt asks "Land drop:" and lists "Play <land>" options, TAKE ONE. Every turn, at
any life total, with any hand, however many lands you already have. "Play no land right now"
is never your answer while a land is listed.
A land in play is a counterspell you can pay for on their turn; a land in your hand is a card
you discard at seven. The land drop does not carry over - a turn you skip it is a turn of mana
you never get back, and it is a card that stays in your hand and pushes something else out at
end of turn.
Which land: take the one whose colour you have fewest of. 18 of your lands make {W} and 20 make
{U}; the "Mana available:" line names, one per untapped card, what each source can make. If both
colours are already covered, any land will do - just take one.
Games of yours have been lost this way. In one you passed the land drop on six straight turns
holding a Plains and an Island, then could not pay for a second counterspell on the turn their
engine landed. In another you skipped three drops, hit seven cards, and DISCARDED a Cancel -
then the spell that killed you resolved four turns later with nothing in hand to stop it.

=== #1 - STAFF OF NIN IS YOUR WIN CONDITION. EMRAKUL IS NOT YOUR PLAN. ===
```

## W2 counter: [defender] is never a counter target
**Finding.** 33/33 counter windows taken (0 declines), but 4 of the 33 were spent on creatures the guide's own printed-power rule says to let resolve, all of them [defender]: Pride Guardian 0/3 (vs126 seq 52), Wall of Omens 0/4 (vs126 seq 53), Shield Sphere 0/6 twice (vs162 seq 9, 36). Primitives: Pride Guardian auto=@combat(blocking) ... life:3; Wall of Omens auto=draw:1 on ETB; Shield Sphere auto=@combat(blocking) counter(0/-1) - none carries {T}: / 'Whenever ... attacks' / 'At the beginning of' / 'venture'. The vs126 pair were the last two counters in hand.

**BEFORE**
```
  - (creature P/T) with printed power 0 or 1: LET IT RESOLVE - unless its {target text: ...} on
    that same option line contains "{T}:", "Whenever ... attacks", "At the beginning of" or
    "venture".
```

**AFTER**
```
  - (creature P/T) tagged [defender], at ANY printed power: LET IT RESOLVE. A defender cannot
    attack, so it can never take a point off you, and you have no creature for it to block.
    Pride Guardian (0/3), Wall of Omens (0/4), Fog Bank (0/2) and Shield Sphere (0/6) are this
    shape. This line has no exception: a counter spent on a wall is a counter you do not have
    for the enchantment that kills you. In a game you lost you spent Dream Fracture on a Pride
    Guardian and Fall of the Gavel on a Wall of Omens on the same turn; four turns later the
    opponent tutored up Sanguine Bond and resolved it with your hand empty of answers.
  - (creature P/T) with printed power 0 or 1 and NOT tagged [defender]: LET IT RESOLVE - unless
    its {target text: ...} on
    that same option line contains "{T}:", "Whenever ... attacks", "At the beginning of" or
    "venture".
```

## W3 sweeper: lone [defender]/0-power is a hard no-cast
**Finding.** 2 of the 9 sweeper casts this corpus hit a board whose only creature was a defender: vs162 seq 17 (Supreme Verdict, lone Fog Bank 0/2 [flying, defender], life 23) and vs126 seq 66 (Final Judgment, lone Wall of Omens 0/4, life 31). Both are already forbidden by the printed-power-4 clause; neither creature could ever attack. Primitives confirm Fog Bank abilities=flying,defender power=0 and Wall of Omens power=0 toughness=4.

**BEFORE**
```
- exactly 1 creature, and Path to Exile is NOT in your list: cast the sweeper when that creature's
  printed power is 4 or more, otherwise answer "Cast nothing right now" and keep it - unless your
  life is 10 or less and nothing else in your list answers that creature: then cast the sweeper
  whatever its printed power.
```

**AFTER**
```
- exactly 1 creature, and that creature is tagged [defender] or shows printed power 0: cast
  NOTHING. It cannot attack you and you have no creature for it to block, so a sweeper aimed at
  it kills a card that was never going to deal you damage - and leaves you tapped out. This is
  the same rule the Path section runs ("a 0/4 wall is never a Path target"); it holds at any
  life total. You have cast a Supreme Verdict at a lone Fog Bank (0/2 defender) at 23 life and a
  Final Judgment at a lone Wall of Omens (0/4 defender) at 31 life; both cost you a card and a
  turn of open mana and killed nothing that mattered.
- exactly 1 creature, and Path to Exile is NOT in your list: cast the sweeper when that creature's
  printed power is 4 or more, otherwise answer "Cast nothing right now" and keep it - unless your
  life is 10 or less and nothing else in your list answers that creature: then cast the sweeper
  whatever its printed power.
```

## W4 0-creature sweeper gate names main phase 2
**Finding.** W10 predicted zero sweeper casts at a '0 are creatures' header; 1 occurred - vs126 seq 73, turn 28, MAIN PHASE 2, header 'Opponent battlefield (13 permanents listed, of which 0 are creatures)'. 42 zero-creature sweeper windows, 1 violation (wave 44: 3/14 casts). The single slip was in the second main phase, which the section's prose never names.

**BEFORE**
```
IF THAT NUMBER IS 0, SUPREME VERDICT AND FINAL JUDGMENT ARE NOT CASTABLE PLAYS THIS TURN.
```

**AFTER**
```
IF THAT NUMBER IS 0, SUPREME VERDICT AND FINAL JUDGMENT ARE NOT CASTABLE PLAYS THIS TURN -
in your FIRST main phase and in your SECOND main phase alike. The header does not change
between them, and neither does this answer.
```

## W5 X sizing collapsed to one subtraction (8 minus the hand line)
**Finding.** W11: the library ceiling (N-2-Staffs) bound in 0/12 windows and was conformed 12/12; the HAND ceiling was violated 6/12 (vs162 seq 15 hand 8 -> X 3; vs123 seq 57 hand 6 -> X 3; vs123 seq 66 hand 8 -> X 3; vs146 seq 39 hand 7 -> X 4; vs146 seq 63 hand 8 -> X 6; vs146 seq 72 hand 5 -> X 6). The model took the MAXIMUM offered X in only 4/12 windows (wave 44: 14/14), so the 'largest X' pull is gone - what remains is arithmetic error across two subtractions. 7 - (handline - 1) == 8 - handline; one subtraction off one rendered number removes the off-by-one entirely and reproduces every verdict above.

**BEFORE**
```
WORK OUT YOUR X BEFORE YOU LOOK AT THE MENU. The menu's own sentence tells you the largest X you can
AFFORD; affordable is not the same as correct, and the largest affordable X is almost never your
answer. Compute two ceilings from the render and take the SMALLER of them:
  CEILING 1 - your hand. Count the cards on your "Your hand:" line and SUBTRACT 1: the Sphinx's
  Revelation you are casting right now is still listed there, and it is leaving. Call that H.
  Your X ceiling is 7 minus H. Everything above 7 cards is discarded at end of turn - cards you
  paid mana for and threw away. If the opponent battlefield line shows Liliana's Caress, Underworld
  Dreams or Fate Unraveler, each discarded card ALSO costs you life on the way to the graveyard.
  CEILING 2 - your library. Read "Your library: N cards". Your X ceiling is N minus 2, minus one
  more for EACH Staff of Nin on your battlefield - every Staff draws you an extra card at your next
  upkeep on top of your draw step.
Then reply with the OPTION number whose line reads "X = " followed by the SMALLER of those two
ceilings, or the largest X the menu offers if every offered value is below both ceilings. If both
ceilings come out at 0 or less, take "X = 0" - a Revelation that draws nothing still costs you
nothing but the card. Worked example from a real game of yours: hand 8, library 13, one Staff of
Nin, menu offering up to 12. H = 7, so ceiling 1 = 0; ceiling 2 = 13 - 2 - 1 = 10; you announce
X = 0, not X = 12 - and better still, a hand of 8 means you should not have taken "Cast Sphinx's
Revelation" in the first place. Announcing 12 there drew 12 cards and then discarded 12 -
including your Elixir of Immortality, a Path to Exile, a Final Judgment and two Fall of the Gavel.
```

**AFTER**
```
WORK OUT YOUR X BEFORE YOU LOOK AT THE MENU. The menu's own sentence tells you the largest X you
can AFFORD; affordable is not the same as correct, and the largest affordable X is almost never
your answer.
ONE SUBTRACTION, off one rendered number:
   COUNT the cards listed on your "Your hand:" line. Call that number N.
   YOUR X IS 8 MINUS N.
That is the whole calculation. Do not compute anything else first; the Sphinx's Revelation you
are casting is one of the N and it is leaving, which is why the number is 8 and not 7. Everything
you hold above seven cards at end of turn is discarded - cards you paid mana for and threw away -
and if the opponent battlefield line shows Liliana's Caress, Underworld Dreams or Fate Unraveler,
each discarded card ALSO costs you life on the way to the graveyard.
Two checks on that number, in this order:
  - If 8 minus N is 0 or less (your hand line reads 8 or more), you should not be casting this
    spell at all - answer "Cast nothing right now" back at the casting menu and hold it. It is an
    instant; it will still be there when your hand is smaller.
  - Read "Your library: N cards". If that library number minus 2, minus one more for EACH Staff of
    Nin on your battlefield, is SMALLER than 8 minus your hand count, announce that smaller number
    instead. Every Staff draws you an extra card at your next upkeep on top of your draw step, and
    drawing from an empty library loses the game outright.
Then reply with the OPTION number whose line reads "X = " followed by that number. If the menu's
values are all above it, take the smallest one offered.
Worked examples, both from real games of yours. Hand line 8, library 13, one Staff, menu up to 12:
8 - 8 = 0, so X = 0 - and a hand line of 8 means you should not have taken "Cast Sphinx's
Revelation" in the first place. Announcing 12 there drew 12 cards and then discarded 12, including
your Elixir of Immortality, a Path to Exile, a Final Judgment and two Fall of the Gavel. Hand line
5, library 32, no Staff: 8 - 5 = 3, so X = 3 - not the 6 you announced, which discarded three
cards at end of turn.
```

## W6 Revelation hand gate restated at 8 to match the sizing rule
**Finding.** 6 of 12 Revelation casts were taken with the hand line at 7 or more (vs126 seq 57 hand 8, seq 63 hand 10; vs162 seq 14 hand 8; vs123 seq 65 hand 8; vs146 seq 38 hand 7, seq 62 hand 8). The old text forbade casting at hand>=7 while the ceiling formula allowed X=1 at hand line 7 - a contradiction inside the same section. Restating both at the same threshold (hand line 8 => X ceiling 0 => do not cast) makes one number govern.

**BEFORE**
```
If your "Your hand:" line already lists 7 or more cards, do NOT take "Cast Sphinx's Revelation" at
all this turn: answer "Cast nothing right now" and keep it. Every card it draws on top of a full
hand is discarded at end of turn, and it is an instant - it will still be there when your hand is
smaller. At N of 6 or more and a hand below 7, cast it as above and announce the X the two ceilings
below give you.
```

**AFTER**
```
If your "Your hand:" line already lists 8 or more cards, do NOT take "Cast Sphinx's Revelation" at
all this turn: answer "Cast nothing right now" and keep it. At 8 cards the sizing rule below gives
you X = 0, and a Revelation for X = 0 gains nothing and draws nothing. It is an instant - it will
still be there when your hand is smaller. At a "Mana available:" of 6 or more and a hand line of 7
or fewer, cast it and announce the X the one subtraction below gives you.
```

## W7 floating mana pool read + it cannot hold a counter up
**Finding.** 2 of 7 Staff of Nin casts happened at 'Mana available: 0 total (no untapped sources) | Already in pool: N mana ALREADY produced and floating' (vs123 seq 67, 9 floating; vs146 seq 73, 6 floating). Every mana rule in the guide points at the 'Mana available:' number, which reads 0 in those windows; the model used the pool anyway. The pool is emptied on phase change (AllAbilities.cpp manaPool->Empty() in the phase-change branch), so mana in the pool cannot be held for a counterspell on the opponent's turn - the guide's tap-out subtraction is unsatisfiable there and should say so.

**BEFORE**
```
=== YOUR MANA AND YOUR WINDOWS ===
```

**AFTER**
```
=== READING THE MANA LINE ===
Every rule in this guide that says "your Mana available: line" means the number on that line -
with one exception the render tells you about itself. When the line reads
"Mana available: 0 total (no untapped sources) | Already in pool: {u}{w}{w}... (N mana ALREADY
produced and floating right now)", the number you have to spend IN THIS DECISION is N, the
floating one. Use N wherever a rule below asks for "Mana available:".
But floating mana is spent-or-lost: it is produced mana, not an untapped land, and it does not
survive to the opponent's turn. Mana you are "keeping open" for a counterspell has to be an
UNTAPPED SOURCE, never a pool. So when the whole of your mana is floating, the counterspell
subtraction below cannot be satisfied at all - spend the pool this turn on whatever the rules
name, and do not tell yourself you are holding a counter up.

=== YOUR MANA AND YOUR WINDOWS ===
```

## W8 mulligan rewritten as an odds comparison (skill amendments #46/#47)
**Finding.** 2 mulligans in 6 games. vs123: a 1-land 6-spell seven, shipped - guide-legal and right. vs126: a THREE-land seven, shipped, against a guide floor that keeps 2-5 lands. That hand was Glacial Fortress, Island, Plains, Lightmine Field {2}{W}{W}, Final Judgment {4}{W}{W}, Emrakul {15}, Staff of Nin {6} - render: 'Cheapest spell in this hand: Lightmine Field (mana value 4)' and 'Playing every land in this hand would not cover any spell in it.' Zero interaction and nothing castable before turn 4. Graded by odds (amendment #47), the ship was favoured; the flat land-count floor is the thing that was wrong. Rewritten as the odds comparison amendment #46 requires, naming the deck's land count, the reactive role, and the render's coverage line as the discriminator. NOT routed to the owner as a floor question: #46 already settled that floors are not the form.

**BEFORE**
```
=== MULLIGAN ===
KEEP any 7-card opening hand with 2, 3, 4 or 5 lands. The prompt counts them for you - "Your hand (7
cards), counted by the engine: 3 lands, 4 spells" - so read that number and do not count the hand
yourself.
Your cheapest cards cost ONE mana (Path to Exile {W}, Elixir of Immortality {1}), so "no early play"
is never a reason to ship a hand, and the prompt's "Playing every land in this hand would cover the
cost of:" line names exactly what this hand can already cast - if it names even one card, this hand
can act.
Tiebreakers between keepable hands only, never a reason to mulligan on their own: prefer the hand
with a land that makes both colours (Tundra, Glacial Fortress, Seachrome Coast), and prefer the hand
with at least one counterspell. Colour is only a tiebreaker: 18 of your lands make {W} and 20 make
{U}, so a one-colour opener still casts spells.
Any hand with 2 to 5 lands is a keep. A two-land hand whose lands make one colour is STILL a keep: your
odds of drawing the third land and the second colour before you need them beat the odds of a better
six-card hand (a six after shipping a two-lander is most often a two-lander again).

When a keep makes you bottom cards, bottom Emrakul first, then your lands beyond the fifth.
```

**AFTER**
```
=== MULLIGAN ===
The question is never "does this hand meet a number". It is: what are the odds THIS hand does
something in time, against the odds a six-card hand does something in time? Two facts about this
deck set that comparison, and the prompt gives you the rest:
  - 26 of your 60 cards are lands, so a two-land seven sees its third land inside two or three
    draws and its fourth by about turn five. Waiting for lands is a bet you usually win.
  - Your job is REACTIVE. Nothing here has to happen on turn two; the cost of a slow start is only
    the cost of the threats that land before you can answer them.
The prompt hands you the three numbers the comparison needs, and you read them rather than counting
the hand yourself:
  "Your hand (7 cards), counted by the engine: N lands, M spells."
  "Mana sources among those lands, counted by the engine: {W} n, {U} n."
  "Playing every land in this hand would cover the cost of: ..." - or "would not cover any spell
  in it."
KEEP any hand with 2 to 5 lands whose coverage line names at least one card. That hand can already
act on their first threat, and the odds of improving it by shipping a card are against you.
THINK AGAIN when the coverage line reads "would not cover any spell in it". That sentence means
this hand does nothing until you draw both a land and a colour, and it is the one shape where
seven cards can be worse than six - most sharply when the spells left in the hand are your
expensive ones (Final Judgment {4}{W}{W}, Staff of Nin {6}, Emrakul {15}) rather than a Path to
Exile {W} or an Elixir {1}. Weigh: how many draws until this hand casts ANYTHING, and can you
afford those turns against the deck across the table? Ship it when the answer is "four or more
turns and they have a creature on turn two"; keep it when one land makes it live.
A one-land seven ships. A hand of 2 to 5 lands with a Path or an Elixir in it never does.
Tiebreakers between keepable hands, never a reason to ship on their own: prefer the hand with a
land that makes both colours (Tundra, Glacial Fortress, Seachrome Coast), and prefer the hand with
at least one counterspell. Colour alone is not a reason to mulligan - 18 of your lands make {W} and
20 make {U}, so a one-colour opener is usually two draws from the other.

When a keep makes you bottom cards, bottom Emrakul first, then your lands beyond the fifth.
```

## W9a DECIDING SITUATIONS leads with the land drop
**Finding.** Same evidence as W1; the wave-44 review found that a rule absent from DECIDING SITUATIONS is a rule that does not fire (that was the E1 finding for sweepers, which then held 41/42 this corpus).

**BEFORE**
```
=== DECIDING SITUATIONS ===
- The plan you were given talks about ramping to Emrakul or about attacking: write the plan "answer
  their threats, resolve Staff of Nin, ping their face every turn" instead.
```

**AFTER**
```
=== DECIDING SITUATIONS ===
- The prompt lists "Play <land>" options: play one. Every turn, no exceptions.
- The plan you were given talks about ramping to Emrakul or about attacking: write the plan "answer
  their threats, resolve Staff of Nin, ping their face every turn" instead.
```

## W9b DECIDING SITUATIONS: lone defender is a no-cast
**Finding.** Same evidence as W3.

**BEFORE**
```
- The header says exactly 1 creature: Path to Exile it and keep the sweeper.
```

**AFTER**
```
- The header says exactly 1 creature and it is tagged [defender] or shows printed power 0: cast
  NOTHING - no sweeper, no Path. It cannot attack and you have no creature for it to block.
- The header says exactly 1 creature: Path to Exile it and keep the sweeper.
```

## W9c DECIDING SITUATIONS: defenders never get a counter
**Finding.** Same evidence as W2.

**BEFORE**
```
- Their spell on the stack prints (creature P/T) with printed power 0 or 1, or prints no P/T at
  all: let it resolve and keep the counter - unless its {target text:} contains "{T}:",
  "Whenever ... attacks", "At the beginning of" or "venture": then counter it now.
```

**AFTER**
```
- Their spell on the stack prints (creature P/T) tagged [defender]: let it resolve, whatever the
  numbers say and whatever its text says. Walls never get a counter.
- Their spell on the stack prints (creature P/T) with printed power 0 or 1, or prints no P/T at
  all: let it resolve and keep the counter - unless its {target text:} contains "{T}:",
  "Whenever ... attacks", "At the beginning of" or "venture": then counter it now.
```

## W9d DECIDING SITUATIONS: X line matches the one-subtraction rule
**Finding.** Same evidence as W5/W6; the situation line previously restated the two-ceiling arithmetic and would have contradicted the new rule.

**BEFORE**
```
- "Mana available:" reads 6 or more and Sphinx's Revelation is listed: cast it, then on the X menu
  announce the SMALLER of (7 minus the cards in your hand after removing the Revelation itself) and
  (your library minus 2 minus one per Staff of Nin) - not the largest number offered.
```

**AFTER**
```
- "Mana available:" reads 6 or more, your hand line reads 7 cards or fewer, and Sphinx's Revelation
  is listed: cast it, then on the X menu announce 8 MINUS THE NUMBER OF CARDS ON YOUR HAND LINE -
  not the largest number offered. If your hand line reads 8 or more, do not cast it at all.
```

---

## THRESHOLD WHYS (skill amendment 61) - reviewer boundary pass, wave-45 step 4
- T = 8 (Sphinx's Revelation X sizing, `8 - handline`) because the hand line is the one printed
  number the executor never miscounts, and folding the "minus one" correction into the constant
  removes the intermediate value that lost the rule 6 of 12 windows. The 8 is the 7-card hand
  limit plus the one card the Revelation itself has already left the hand.
- T = 8 (the "do not cast Revelation at hand N or more" gate) because it is the SAME boundary as
  the sizing formula above; two independently-stated numbers for one boundary is a contradiction
  the executor resolves by whichever it read last, and the earlier pair (7 and 8) was cast
  through 6 of 12 times.
- T = "(keeping 5)" (the mulligan chain stop) because the London mulligan already shaves a seven
  to six for one card, so a third look buys a fresh seven at the price of two; two seats in the
  wave-45 corpus reached four cards and both lost.
- T = 3 lands (the bottoming land floor) because this deck's answers cost {1}{U} to {1}{W}{W}{U}
  and a two-land keep cannot hold a counter up on the turn the first real threat lands.
