# wave-55 deck130 guide edits (before -> after)

Every edit below is applied to `wave55/deck130/strategy.txt`, which starts from the live
guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (wave-54 reviewer revisions
`5160eb1cb` + boundary pass `dd0cd2d74`). Tags beginning `A55-TRIM`/`D55-TRIM` are pure
length trims taken to keep the file inside the 41-71 KB pool band; they carry no new rule.

## D55-1
**Finding.** THE headline change. The X-menu marker rendered on 3 of 3 menus this corpus (wave 54: 0 renders in 42 logs) in TWO forms - '[<- most kills at any affordable X that costs you nothing ...]' (130v152 s46, 130v146 s40) and the new '[<- best trade: the most of THEIRS at the smallest cost to YOURS ...]' (130v146 s36). The old text told the pilot the marker 'appeared ZERO times' and to read past it; that sentence is now false and is exactly what 130v146 s40 did - answered X=3 with the marker on X=1 ('same kills as X=3, for 2 less mana'), two mana for an identical board. Also adds the WHICH-vs-WHETHER separation, because the best-trade marker fired on a row killing one of theirs for two of its own Goblins (130v146 s34/s36), which entry 1 of the cast order forbids.

BEFORE:
```
       STEP 1. If a row carries "[<- most kills at any affordable X that costs you nothing]",
               THAT row is the answer: answer its number and read nothing else - it is already
               the largest kill list at the lowest X that produces it. This deck answered X=6
               with that marker on the X=2 row four rows below (same kills, four mana less).
               THE MARKER IS OFTEN NOT THERE. It is printed only when some X kills something of
               theirs and nothing of yours; across a whole corpus of twelve games it appeared
               ZERO times. Do not wait for it and do not read its absence as "no row is good".
               With no marker printed: find the row with the LARGEST "kills THEIRS" list whose
               "YOURS:" reads "none". That is your X. Bigger is free when YOURS is none - the mana you save buys
               nothing if you have nothing else to cast this turn, and a creature you left
               alive because you paid one mana less is a creature that wins their game.
```
AFTER:
```
       STEP 1. THE MENU MARKS ONE ROW, AND THAT ROW IS THE ANSWER. Last corpus every X menu
               this deck reached carried a marker - three of three - in one of two forms:
                 "[<- most kills at any affordable X that costs you nothing - at X=N that is
                  THEIRS: ...; YOURS: none]"   (some X is free)
                 "[<- best trade: the most of THEIRS at the smallest cost to YOURS - at X=N
                  that is THEIRS: ...; YOURS: ...]"   (no X is free)
               Answer the marked row's number and read nothing else; it has already done both
               the kill comparison and the mana comparison, and it is already the BOTTOM of any
               collapsed run. This deck took the marked row twice and, on the third menu,
               answered "X = 3" with the marker sitting on "X = 1 {same kills as X=3, for 2 less
               mana}" two rows below - two mana for an identical board.
               A MARKER IS NOT A REASON TO CAST. It answers WHICH X, never WHETHER: the "best
               trade" form is printed even when the best trade is a bad one, and this deck took
               a marked X=2 that read "THEIRS: Silverquill Silencer; YOURS: Goblin x2" - one of
               theirs for two of yours, which entry 1 of the cast order forbids. WHETHER is
               decided one screen earlier, at the cast row, where "Cast nothing right now" still
               exists. If no marker is printed: the LARGEST "kills THEIRS" list whose "YOURS:"
               reads "none". Bigger is free when YOURS is none.
```

## D55-4
**Finding.** The only rung deck130 lost ground on. Four wrong Lay Waste cycles: 130v126 s22 and s23 (back to back, five own lands), 130v146 s28 (three lands), 130v152 s18 (three lands, their board on three). The old gate was a LAND COUNT ('8 or more', 'below THREE'); the real test is whether {3}{R} is affordable this turn, which the 'Mana available: N total' line prints. Re-keyed to that.

BEFORE:
```
  Lay Waste       -> CAST IT at a land. Cycle Lay Waste once you control 8 or more lands - or
                     below THREE lands with a Stone Rain or Molten Rain already in hand: then
                     Lay Waste is the land-kill you cannot cast for two turns and the card it
                     draws is the third land (it was, once - Molten Rain on turn 6 followed).
                     At four to seven lands it is castable and it is cast, never cycled.
```
AFTER:
```
  Lay Waste       -> CAST IT at a land. THE GATE IS THE MANA LINE, NOT A LAND COUNT: Lay Waste
                     costs {3}{R}, so read "Mana available: N total". At 4 or more it is
                     CASTABLE - cast it at one of their lands this turn or hold it for next
                     turn's drop; never cycle it. Cycle Lay Waste only when it is UNCASTABLE
                     this turn AND you already hold another land-kill (Stone Rain, Molten Rain),
                     or at 8 or more lands when the land plan is over. And never twice in a
                     turn. Last corpus this deck cycled Lay Waste FOUR times - twice
                     back-to-back at five lands, twice more at three with their board on three
                     and four lands - and it is the only rung this deck lost ground on.
```

## D55-5
**Finding.** Two face casts at a healthy opponent, both Spark Sprays whose only opponent-side row was the player: 130v152 s11 (opp 20, paid 1 life through a Talisman) and 130v125 s32 (opp 15). The cycling row is on the same card and the cast row now prints 'kills whichever you target: THEIRS - none' - the answer there is the cycle.

BEFORE:
```
  Spark Spray     -> cycle it when its cast option's "legal targets right now:" list shows no
                     opponent creature. If it shows one, cast it instead (rule #2).
```
AFTER:
```
  Spark Spray     -> cycle it when its cast option's "legal targets right now:" list shows no
                     opponent creature. If it shows one, cast it instead (rule #2). THE CYCLE
                     IS THE ANSWER THERE, NOT THE FACE. A Spark Spray whose only opponent-side
                     row is "The opponent (player, life N)" is a cycling row on the same card:
                     "{kills whichever you target: THEIRS - none; YOURS - Goblin x3 - and 1 to
                     the opponent at life 15 leaves them at 14}" is the engine telling you the
                     spell kills nothing, and 1 damage off 15 is not a clock. Last corpus this
                     deck cast it at a healthy face twice, once paying a Talisman life for the
                     privilege. One card for one point is the worst rate in the deck.
```

## D55-6
**Finding.** 130v152 s50 (7 life, their board SIX lands, 'Cast nothing right now' on the menu): cast Molten Rain under a plan that wrote the reason - 'Land count is 6, which is >4, so land destruction is generally low value, but this is the only playable card'. Both the five-land gate and the 8-life floor already existed as separate paragraphs; they are now one two-number line written before the cast. Also records the rung's real denominator (2 of 19 at five-plus lands) so it reads as nearly closed.

BEFORE:
```
COUNT THE LAND ROWS ON THEIR BATTLEFIELD LINE BEFORE EVERY ONE OF THESE CASTS. Collapsed handles
count in full: "Island #1-#4 x4; Plains #1-#3 x3; Seachrome Coast" is NINE lands, not three rows.
At FIVE or more the spell is dead for the rest of the game - it does not become live again because
you have nothing else to do with the mana. Six such casts in the last corpus went at boards of
nine, nine, ten and twelve-plus lands; the opponent's mana never moved and the game was lost at
turn 69. When the count is past four and nothing else is castable, the answer is "Cast nothing
right now" and the card is a Cleanup discard, not a play.
```
AFTER:
```
WRITE TWO NUMBERS BEFORE EVERY ONE OF THESE CASTS: "Their lands <N>; my life <L>". N comes off
their battlefield line's own tail ("... and N are lands"), and collapsed handles count in full -
"Island #1-#4 x4; Plains #1-#3 x3; Seachrome Coast" is NINE lands, not three rows. N at FIVE or
more, or L at 8 or less, and the spell is dead for the rest of the game; it does not become live
again because you have nothing else to do with the mana. "This is the only playable card" is the
sentence that spends it anyway - this deck wrote exactly that at 7 life against a SIX-land board,
with "Cast nothing right now" printed on the same menu, in the game it lost. When either number
says stop, the answer is "Cast nothing right now" and the card is a Cleanup discard, not a play.
Last corpus this rung otherwise held: 19 land-destruction casts, only 2 at five-plus lands.
```

## D55-7
**Finding.** 130v146 s17: sacrificed a Pyrite Spellbomb at 'Nadaar, Selfless Paladin ... {right now: takes 2 damage - SURVIVES (toughness 3)}' under 'PLAN: Kill Nadaar with Pyrite Spellbomb'. Adds the Spellbomb-specific reading (the artifact keeps at no cost) and the plan-verb-versus-row-verdict line.

BEFORE:
```
     "SURVIVES" is the answer "not this row": at 3 life this deck fired a Spellbomb at a row
     reading "SURVIVES (toughness 4)", paid the {r} with a Talisman, and went to 2 for
     nothing.
```
AFTER:
```
     "SURVIVES" is the answer "not this row", and for the Spellbomb it is the answer "keep the
     gun loaded": the artifact sits on the battlefield costing nothing until a 2-toughness
     creature arrives. At 3 life this deck fired one at a row reading "SURVIVES (toughness 4)",
     paid the {r} with a Talisman and went to 2 for nothing; last corpus it fired one at
     "Nadaar, Selfless Paladin ... SURVIVES (toughness 3)" while its own PLAN said "Kill Nadaar
     with Pyrite Spellbomb". The row's verdict outranks the plan's verb every time.
```

## D55-3
**Finding.** NEW rung, new card fact. deck130.txt runs Rorix Bladewing *2 and mtg.txt:96836 gives 'type=Legendary Creature'. 130v146 s62 (14 life vs 13, first Rorix on the battlefield attacking for 6 a turn) cast the second off a row reading '[legendary: you already control Rorix Bladewing - legend rule: casting this sends one copy to your graveyard (you choose which)]' - six mana for the identical board. The guide had no rule for the legend bracket.

BEFORE:
```
RORIX BLADEWING is a 6/5 with FLYING and HASTE - cast him and declare him as an attacker that same
turn, and every turn after. He is the fastest clock you own.
```
AFTER:
```
RORIX BLADEWING is a 6/5 with FLYING and HASTE - cast him and declare him as an attacker that same
turn, and every turn after. He is the fastest clock you own. HE IS ALSO LEGENDARY, AND YOU RUN
TWO. With one already on the battlefield the second one's row says so - "[legendary: you already
control Rorix Bladewing - legend rule: casting this sends one copy to your graveyard (you choose
which)]" - and casting it is six mana to end the turn with exactly the same board. That bracket
is the word DEAD: keep the second Rorix in hand for the turn the first one dies. Last corpus this
deck cast the second one at 14 life against 13 while the first was attacking for 6 a turn.
```

## D55-1b
**Finding.** DECIDING SITUATIONS X-menu bullet re-led with the marker (was: the no-marker fallback first).

BEFORE:
```
- You are on the "Announce the value of X" menu -> take the row with the LARGEST "kills THEIRS"
  list whose "YOURS:" reads "none", then walk DOWN to the bottom of any run of rows reading
  "same kills as X=N" and answer THAT row. Option 1 is the LARGEST X, never X=1.
- A row below the one you were about to answer reads "same kills as X=N, for 1 less mana" ->
  answer the lower row. Same board, one more mana in your pool.
```
AFTER:
```
- You are on the "Announce the value of X" menu -> find the row carrying a "[<- most kills at
  any affordable X that costs you nothing ...]" or "[<- best trade: the most of THEIRS at the
  smallest cost to YOURS ...]" marker and answer THAT row's number. It is already the bottom of
  any collapsed run. Only with no marker printed: the LARGEST "kills THEIRS" list whose "YOURS:"
  reads "none", then walk DOWN through any "same kills as X=N" rows. Option 1 is the LARGEST X.
- A row below the one you were about to answer reads "same kills as X=N, for 1 less mana" ->
  answer the lower row. Same board, one more mana in your pool.
```

## D55-4b
**Finding.** DECIDING SITUATIONS cycling bullet re-keyed to the mana line.

BEFORE:
```
- "cycling with Lay Waste" is offered and you control 7 or fewer lands -> pass, take your land drop,
  and cast Lay Waste at one of their lands this turn or next. Count YOUR lands on your own
  battlefield line before you answer; at 5 life on six lands this deck cycled one from a blockers
  window and drew a card it could not cast. Below THREE lands with a Stone Rain or Molten Rain
  already in hand is the only exception, and it is about finding the third land.
```
AFTER:
```
- "cycling with Lay Waste" is offered and "Mana available:" is 4 or more -> pass. It is
  CASTABLE; cast it at one of their lands this turn, or take your land drop and cast it next
  turn. Cycle it only when it is uncastable this turn AND another land-kill is already in your
  hand, or at 8+ lands with the land plan over - and never twice in one turn. Four wrong cycles
  last corpus, two of them back to back.
```

## D55-1c
**Finding.** KEY CARDS Starstorm line re-pointed at the marked row.

BEFORE:
```
let the Captain live for one mana. The X is chosen by rule #2 STEP 1 - the LARGEST "kills THEIRS"
list whose "YOURS:" reads "none" - and by nothing else. Because it
```
AFTER:
```
let the Captain live for one mana. The X is chosen by rule #2 STEP 1 - the MARKED row, and by
nothing else. Because it
```

## D55-TRIM1
**Finding.** Trim only: STEP 0's plan-is-stale anecdote is stated twice elsewhere; kept the rule, dropped the story. Paid for D55-1's added length.

BEFORE:
```
       STEP 0. IF YOUR CARRIED PLAN ALREADY NAMES AN X, IGNORE IT AND READ THE ROWS. The plan
               you wrote one screen ago was written before this menu existed, and the screen
               hands it back to you as "YOUR PLAN (as you last stated it, N windows ago on turn T)" - that
               stamp is how old it is, and an X written before this menu existed is older than
               the menu. This deck arrived
               here with "Cast Starstorm at X=3 to kill both Pride Guardians", answered X=3
               verbatim, and never read the X=4 row above it. An X in a plan is a guess; an X
               on a row comes with the kill list attached.
```
AFTER:
```
       STEP 0. IF YOUR CARRIED PLAN ALREADY NAMES AN X, IGNORE IT AND READ THE ROWS. The screen
               hands the plan back stamped with its age ("as you last stated it, N windows ago
               on turn T"); an X written before this menu existed is older than the menu. An X
               in a plan is a guess; an X on a row comes with the kill list attached.
```
