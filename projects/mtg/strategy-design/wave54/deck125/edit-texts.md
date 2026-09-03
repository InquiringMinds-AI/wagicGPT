# deck125 (Revelation Fracture) - wave-54 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` (55,975 B, wave-53
revisions baf36b936 + boundary pass a0067bae5). Revised: `wave54/deck125/strategy.txt`
(61,190 B). Pool band 39.6-70.2 KB: in band. Twelve edits, all from `matchups-20260903-034355`.

Corpus root `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/`; seq references are the
deck125 seat log of the named matchup.

## 1. W99/W100 cleanup steps 1-2

**Finding.** W99/W100 - `125v162` seq 47 (t17, 8 life, 9 lands in play, sent both Fall of the Gavel and kept the Island); `125v126` seq 73 (t23, 10 lands, sent both Gavels, their Sanguine Bond resolved uncountered at t24); `125v126` seq 80 (t27, 12 lands, 4 lands on the list, sent an Essence Scatter). Control: `125v123` seq 112 sent two Islands. **3 of the 4 windows where a land was on the list.** Step 2 was a derivation over two hand-counts; it is now the battlefield header's printed `and N are lands`.

BEFORE:
```
  1. Lightmine Field, unless the header second number reads 3 or more.
  2. A land beyond the second in hand.
```

AFTER:
```
  1. Lightmine Field, unless the header second number reads 3 or more.
  2. EVERY LAND ON THE NUMBERED LIST, while the numbers still need sending, whenever your own
     battlefield line reads "and N are lands" with N of 6 or more. READ N OFF THAT LINE; do not
     count the lands in your hand and do not subtract two. At six lands in play this deck casts
     everything it owns except an Emrakul, so a land in hand is the cheapest card you will ever
     hold, and it is cheaper than the counterspell you would send instead. Three of the four
     cleanups where a land sat on this list sent a counterspell past it: at 8 life on NINE lands
     you sent both Fall of the Gavel and kept an Island, a Lightmine Field and a Final Judgment
     against a board with zero creatures, and died two turns later with nine untapped sources and
     no counter in hand; on TEN lands you sent both Fall of the Gavel again and their Sanguine
     Bond resolved uncountered the next turn; on TWELVE lands you sent an Essence Scatter with
     four lands on the list.
     THE ABSOLUTE THAT FOLLOWS FROM IT: while any land, or a Lightmine Field that step 1 wants,
     is still on the numbered list, a counterspell is NEVER part of your answer. Steps 3 to 6
     come after step 2 for the same reason and none of them is ever skipped to reach step 7.
```

## 2. W99b cleanup anecdote

**Finding.** W99b - same three seqs. Adds the sizing sentence (#198: 3 breaks, 3 distinct game/turns, 2 of them losses) and names the render gap (no `{right now:}` on any discard row) so the pilot knows to read the battlefield lines instead. Engine half is seat-file HIGH #2.

BEFORE:
```
You sent a Cancel from a hand holding a Lightmine Field at 44 life against an empty board, and two
Essence Scatters from a hand of six counters with Emrakul at 34 life on sixteen lands; that second
one was fine, the first was a counter thrown away.
```

AFTER:
```
You sent a Cancel from a hand holding a Lightmine Field at 44 life against an empty board, and two
Essence Scatters from a hand of six counters with Emrakul at 34 life on sixteen lands; that second
one was fine, the first was a counter thrown away. THIS ASK IS WHERE YOU LOSE GAMES: eighteen
cleanups last corpus, three sends that broke the order, and TWO OF THEM ARE TWO OF YOUR LOSSES.
The list is bare card names with no {right now:} verdict on any row, so nothing on the ask itself
will stop you - the numbers you need are on the battlefield lines above it: YOUR "and N are lands",
and THEIRS, whose second number is the count that makes a Lightmine Field and a sweeper dead.
```

## 3. W96/W97 counter-vs-optional-draw

**Finding.** W96/W97 - `125v162` seq 29 (K=14), seq 48 (K=6, Howling Mine `mtg.txt:55935-37`), seq 49 (K=4, Teferi's Puzzle Box `mtg.txt:118332-34`). All three scored as breaks of the old flat floor and all three were correct. The section said "the same goes for any other optional draw" three lines above "your answer to those permanents is a counterspell on the stack" (#177/#190 diff). Threshold re-shaped per #192 into a comparison against a printed literal (`{target text:}` draw text) rather than a constant that is wrong in one direction.

BEFORE:
```
Answer "Cast nothing right now" and hold it. The same goes for any other optional draw. Your
answer to those permanents is a counterspell on the stack, never a bigger hand afterwards.
```

AFTER:
```
Answer "Cast nothing right now" and hold it. The same goes for any other draw you choose to take
for the cards.
A COUNTERSPELL IS NOT AN OPTIONAL DRAW, AND THIS STOP NEVER REACHES ONE. Dream Fracture draws you
a card as it counters; that draw is a side effect of the answer, not a reason you cast it, and
your answer to these permanents is a counterspell on the stack, never a bigger hand afterwards.
When the counter's row carries "[DRAW PRICE: ... - you would be at K]", K is the price of the
answer and you compare it to what the thing on the stack costs you PER TURN, not to a fixed floor:
 - The spell on the stack is itself a DRAW ENGINE - its own {target text:} contains "draws an
   additional card", "draws that many cards", "draws seven cards" or "draws two cards" - then it
   costs you 1 life per punisher per extra draw for the REST OF THE GAME. Counter it at any K above
   1. At 8 life you countered a Howling Mine at K=6 and at 6 life a Teferi's Puzzle Box at K=4, with
   two punishers out and a Dictate of Kruphix already resolved: both were the cheapest life you
   spent all game, and the reason you still lost is the cleanup ask, not those two casts.
 - The spell on the stack is a THIRD punisher or another permanent that only adds 1 per draw: it
   is worth K only while K stays above 15. At 24 life you paid 14 for exactly that and were dead
   two turns later.
 - The spell on the stack is a creature, a mana rock or anything with no draw text: the ordinary
   counter-on-sight list decides it and the DRAW PRICE is just the receipt.
```

## 4. W101 tap-out gate exemptions

**Finding.** W101 - `125v152` seq 22 (Final Judgment exiling 5 at 5 life), `125v130` seq 14 (Supreme Verdict destroying 4), `125v130` seq 18 (Elixir at 2 life), `125v162` seq 3 (turn 1, a 5-mana Gavel no board could cast), `125v126` seq 22 (**the live one**: Staff at taps-you-out with a Cancel in hand vs the combo deck). #201: 5 raw, 1 live. Writing the exemptions in is what makes the next corpus's count mean something.

BEFORE:
```
  - No counter in your hand at all: the gate is open at any N.
```

AFTER:
```
  - No counter in your hand at all: the gate is open at any N.
  - THE GATE IS ALSO OPEN, whatever N reads, in these three shapes and only these three:
    (a) the row is a sweeper whose "{right now: destroys/exiles K of their creatures}" reads K of
        2 or more - a sweeper is the answer, so holding mana for a counter instead of casting it
        is holding an answer to protect an answer;
    (b) your life is 5 or less and the row gains life (an Elixir of Immortality activation, a
        Sphinx's Revelation under the life branch);
    (c) the cheapest counter in your hand costs MORE than the row's own "M" total - if you could
        not have cast it this turn at any N, the gate has nothing to protect.
    Four of the five rows scored against this gate last corpus were one of those three, and the
    fifth was the real one: a Staff of Nin at "casting this taps you out" at 20 life with a Cancel
    in hand, against the combo deck whose two enchantments are exactly what a Cancel answers.
```

## 5. W98 Path keyed to target text

**Finding.** W98 - 4 Path casts: `125v162` seq 53 (Master of the Feast 5/5), `125v126` seq 74 (lifelink Vampire under the converter line - the carve-out), `125v126` seq 66 (lifelink Vampire, no converter), `125v130` seq 12 (Dwarven Blastminer, printed 1/1, `{target text:}` = `{2}{R}, {T}: Destroy target nonbasic land`). #188/#189: the rule was keyed to printed power, a number that measured none of the four; both strings it now reads (`{removes:}` and `{target text:}`) render on the row being answered.

BEFORE:
```
A 0/4
wall is never a Path target, and neither is a token with no {target text:} at all - "Goblin
(creature 1/1)", a Spirit, a Human, a Wolf: you Pathed a lone Goblin token twice at 20 life with
ten lands open, and once more last corpus at 35 life on a board with no converter anywhere.
```

AFTER:
```
A 0/4
wall is never a Path target, and neither is a token with no {target text:} at all - "Goblin
(creature 1/1)", a Spirit, a Human, a Wolf: you Pathed a lone Goblin token twice at 20 life with
ten lands open, and once more at 35 life on a board with no converter anywhere.
THE TEST IS THE PRINTED TEXT, NOT THE PRINTED POWER, AND BOTH STRINGS ARE ON THE ROW YOU ARE
ANSWERING: the cast row prints "{removes: <name>}" and the candidate prints its own
"{target text: ...}". A 1-power body WITH a {target text:} that repeats - "{T}:", "Whenever ...
attacks", "At the beginning of", "venture" - or a [lifelink] body of any power is a Path target;
a 1-power body whose {target text:} is empty or reads only "When this creature enters" is not.
Every Path you cast last corpus passed that test and none passed a power test, which is why the
power rule is now the LAST thing you read and not the first.
```

## 6. W91 low-X ceiling refresh

**Finding.** W91 - 51 `max affordable X=1`/`X=2` rows, **0 taken** (wave 53: 2 of 116). #194's "nothing else was castable" excuse did not return. The line now records a ceiling instead of re-arguing a break.

BEFORE:
```
One hundred and sixteen such rows last corpus and you held one hundred and fourteen.
```

AFTER:
```
One hundred and sixteen such rows one corpus and you held a hundred and fourteen; FIFTY-ONE the
next and you held all fifty-one. This rule is at its ceiling - it does not need re-arguing, only
obeying.
```

## 7. W97b Fruition anecdote compressed (#200c)

**Finding.** W97b - Forced Fruition rendered **0 times** this corpus (checked across all 12 of my logs). #200(c): the rule stays, the anecdote compresses. Saves ~5 lines and removes the stale "eight rows carried it last corpus" count (0 renders now).

BEFORE:
```
   - The row ends "; this KILLS you": that is the floor, not the rule. Eight rows carried it last
     corpus and you declined all eight - correctly, and it changed nothing, because the row that
     lost you the game was the one that did NOT carry it.
   AT 24 LIFE, WITH TWO PUNISHERS ALREADY RESOLVED, YOU CAST ESSENCE SCATTER OFF A ROW READING
   "deal you 14 - you would be at 10" TO COUNTER A THIRD PUNISHER. The counter worked. A third
   punisher costs you 1 more life per card you draw; you paid fourteen for it in one window, went
   24 -> 10 on the spot, and were dead two turns later without another spell being cast. That is
   the whole loss, twice over: the corpus before, at 3 life with "deals you 7" on the row, you cast
   Essence Scatter at a Fate Unraveler and the seven 1-damage pings took you 3 -> 2 -> 1 -> 0 in
   that window. The damage never arrives as one hit you might survive - it arrives as one ping per
   card drawn, and no answer of yours interrupts it.
```

AFTER:
```
   - The row ends "; this KILLS you": that is the floor, not the rule. You have declined every row
     that carried it, and the row that lost you a game was the one that did NOT carry it - at 24
     life you cast Essence Scatter off "deal you 14 - you would be at 10" to counter a THIRD
     punisher, went 24 -> 10 in one window and were dead two turns later without casting again.
     The damage never arrives as one hit you might survive; it arrives as one ping per card drawn,
     and no answer of yours interrupts it. The DRAW ENGINE carve-out in the draw-is-damage section
     above is the only thing that lowers this 15, and Forced Fruition is not a draw engine of
     theirs - it is a draw engine pointed at YOU.
```

## 8. W97c deciding-situations draw bullet re-keyed to the dedicated line

**Finding.** W97c - #193: `DRAW PUNISHERS on the battlefield: theirs - <names>` rendered on **49 deck125 records** as a dedicated single-purpose line, while the bullet told the pilot to read the composite battlefield listing for `{effect:}` substrings. Keyed to the dedicated line, with the negative written in.

BEFORE:
```
- The opponent battlefield line shows Underworld Dreams, Fate Unraveler or Ob Nixilis the
  Hate-Twisted - anything whose {effect:} says "Whenever an opponent draws" - do NOT cast Sphinx's
  Revelation and do not take any optional draw, at any mana and any life total. Each card you draw
  costs you 1 life per such permanent; the row's [DRAW PRICE:] prints the total.
```

AFTER:
```
- The prompt carries "DRAW PUNISHERS on the battlefield: theirs - <names>" (read that dedicated
  line; you do not read the battlefield listing for it) - do NOT cast Sphinx's Revelation and do
  not take any draw you are taking for the cards, at any mana and any life total. Each card you
  draw costs you 1 life per such permanent; the row's [DRAW PRICE:] prints the total. A COUNTERSPELL
  IS NOT ONE OF THOSE DRAWS: cast it by the ladder in the draw-is-damage section - at any
  "- you would be at K" above 1 when the spell on the stack has draw text of its own, above 15
  otherwise.
```

## 9. W101b deciding-situations elixir bullet

**Finding.** W101b - #177/#190: the DECIDING SITUATIONS bullet had to gain the same three shapes or it would keep contradicting the section. Also drops the stale "five of eight breaks last corpus" count.

BEFORE:
```
- Your life is 15 or less, or your library is 16 or less, and Elixir of Immortality is on your
  battlefield untapped: activate it. At a life above 15 AND a library above 16, activate it only
  when the activation row's own "{spends K of your M untapped mana sources this turn; ...}" leaves
  M minus K at or above the cheapest counter on your hand line. This is the gate's worst break
  point - five of eight breaks last corpus were this activation.
```

AFTER:
```
- Your life is 15 or less, or your library is 16 or less, and Elixir of Immortality is on your
  battlefield untapped: activate it. At a life above 15 AND a library above 16, activate it only
  when the activation row's own "{spends K of your M untapped mana sources this turn; ...}" leaves
  M minus K at or above the cheapest counter on your hand line - unless one of the gate's three
  open shapes applies (sweeper at 2+, your life at 5 or less, or a counter the row's M could never
  have paid for anyway).
```

## 10. W96b deciding-situations fruition bullet

**Finding.** W96b - #190 diff: the Fruition bullet's flat 15 had to gain the draw-engine exception or the two bullets would disagree.

BEFORE:
```
- "Forced Fruition" is on the opponent battlefield line beside Underworld Dreams, Fate Unraveler or
  Ob Nixilis: cast nothing on your turn, and cast a counter only when the row's own
  "- you would be at K" reads 15 or more. Do not compute the price; read K off the row.
```

AFTER:
```
- "Forced Fruition" is on the opponent battlefield line beside a DRAW PUNISHERS line: cast nothing
  on your turn, and cast a counter only when the row's own "- you would be at K" reads 15 or more -
  or at any K above 1 when the spell you are countering has draw text of its own (a Howling Mine, a
  Teferi's Puzzle Box, a Dictate of Kruphix, another Fruition). Do not compute the price; read K
  off the row.
```

## 11. W99c deciding-situations cleanup bullet

**Finding.** (see seat file)

BEFORE:
```
- The cleanup ask ("PUT: n, m"): send Lightmine Field, spare lands, a second sweeper, Emrakul
  (below 13 lands), a spare Revelation, a Staff of Nin only with a Staff already in play - a
  counterspell last, most expensive first, except that Essence Scatters go first against a deck
  whose every creature so far is a wall.
```

AFTER:
```
- The cleanup ask ("PUT: n, m"): send Lightmine Field (unless their header second number is 3+),
  then EVERY LAND on the list while your own battlefield line reads 6 or more lands, then a second
  sweeper, Emrakul (below 13 lands), a spare Revelation, a Staff of Nin only with a Staff already
  in play - a counterspell LAST, most expensive first, except that Essence Scatters go first
  against a deck whose every creature so far is a wall. While a land or a dead Lightmine Field is
  still on the numbered list, a counterspell is not your answer: that send has now cost you two
  games.
```

## 12. W98b deciding-situations path bullet

**Finding.** W98b - #190 diff: the bullet still said "defenders or 1-power creatures", which the new prose contradicts.

BEFORE:
```
- The only Path targets offered are defenders or 1-power creatures and your life is above 12: cast
  nothing and keep the Path - UNLESS the prompt carries "LIFE-TO-DAMAGE CONVERTER on the
  battlefield: theirs - <name>" and one of those candidates is tagged [lifelink]: then Path that
  one, at any life total.
```

AFTER:
```
- The only Path targets offered are defenders, or bodies whose own "{target text:}" is empty or
  reads only "When this creature enters", and your life is above 12: cast nothing and keep the
  Path - UNLESS a candidate is tagged [lifelink], or the prompt carries "LIFE-TO-DAMAGE CONVERTER
  on the battlefield: theirs - <name>": then Path that one, at any life total and at any printed
  power.
```
