# wave-56 deck130 guide edits (before -> after, each with its finding)
Start point: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt`. Every edit below is a
single exact-string replacement applied to that file, in this order.

## D56-1
**Finding.** THE WAVE'S HEADLINE EDIT. 8 face-damage takes at this seat, 5 of them above the 6-life floor - and every one of those 5 was into a battlefield line reading `of which 0 are creatures` where the ONLY other row on the menu was `Yourself (player, life N)`. `130v123` s76 put a Hammer into a face at 16 across an opponent battlefield of `(0 permanents listed)` and that game was WON at -3 on turn 34: burn was the only clock the board allowed. The old rule made that play a break. The release is keyed to two numbers the render already prints (0 creatures AND 2 or fewer lands).

**BEFORE**
```
  AND THE EMPTY OPPOSING BOARD IS THE HARDEST VERSION OF THE FLOOR, BECAUSE THERE IS NOTHING
  ELSE TO AIM AT. A Pyrite Spellbomb with "legal targets right now: the opponent, you" and no
  creature on their line is a HOLD, not a play: last corpus this deck sacrificed one at the face
  at 17 life against 15, moving them to 13, and lost that game at -2 to 14 twelve turns later
  with two of their creatures alive that the Spellbomb would have killed. Two damage against a
  healthy opponent is a card; two damage against a 2-toughness creature is a card AND a body.
```

**AFTER**
```
  AND THE EMPTY OPPOSING BOARD IS THE HARDEST VERSION OF THE FLOOR, BECAUSE THERE IS NOTHING
  ELSE TO AIM AT. A Pyrite Spellbomb with "legal targets right now: the opponent, you" and no
  creature on their line is normally a HOLD: two damage against a healthy opponent is a card;
  two damage against a 2-toughness creature is a card AND a body.
  THE FLOOR RELEASES WHEN THEIR BOARD IS GONE AND HAS NOTHING TO COME BACK WITH, AND THE
  RELEASE IS TWO NUMBERS OFF THEIR OWN BATTLEFIELD LINE. "Opponent battlefield (N permanents
  listed, of which 0 are creatures, ... and L are lands)". At 0 creatures AND L of 2 or fewer,
  you have already won the land war: they are not deploying the creature you were saving the
  card for, the card you are holding will never find a better target, and the face row IS the
  clock. Take it, at any life total. Last corpus this deck put a Hammer of Bogardan into a face
  at 16 life across a battlefield line reading "(0 permanents listed)" and won that game at -3
  on turn 34; it was the only route the board allowed. At L of 3 or more the floor still binds -
  they still have mana for a threat, and the card is still removal.
  THE RELEASE IS NOT A LICENCE AGAINST A LIFEGAIN ENGINE. If their line names Elixir of
  Immortality, a lifelink creature or a Sanguine Bond / Exquisite Blood half, compare per turn:
  three points a turn from a recurring Hammer does not beat five a turn from an Elixir, and the
  CROSS-CHECK latch below is what tells you which side of that you are on.
```

## D56-2
**Finding.** The CROSS-CHECK latch was a one-way latch on ANY life gain. In `130v125` it fired on a single Elixir of Immortality activation and then forbade the deck's only remaining line for 30 turns. Rewritten as a per-turn comparison (their gain per turn vs your damage per turn) - which still latches against that Elixir (5 a turn beats a recurring Hammer's 3) and does not latch against a one-off lifelink block.

**BEFORE**
```
CROSS-CHECK, AND IT IS A ONE-WAY LATCH: "Opponent life:" is printed on every decision. If it is
HIGHER than when you last looked, face damage is not a clock - they gain faster than you burn.
Aim nothing at the face for the rest of that game; spend every point on their creatures and
planeswalkers. This latch fired on turn 14 of the last corpus's longest game (20 -> 23 between two
windows) and was then broken four times over the next thirty-six turns, at 23, 19, 16 and 21 life.
Once it has fired, "The opponent (player, life N)" is not an answer again in that game unless the
LETHAL-THIS-TURN chain below adds up on the screen in front of you.
```

**AFTER**
```
CROSS-CHECK: "Opponent life:" is printed on every decision. If it is HIGHER than when you last
looked, ask what put it there before you latch. A ONE-OFF gain - a lifelink blocker connecting
once, a single Elixir activation - is not a lifegain engine and does not stop the burn. A gain
that REPEATS, and whose size per turn is at or above the damage you can produce per turn, does:
against that, face damage is not a clock and every point belongs on their creatures and
planeswalkers for the rest of the game. Write the comparison once, on the window it happens:
"they gained N this turn; I make M a turn" - N at or above M is the latch, N below M is not.
Once the latch has fired, "The opponent (player, life N)" is not an answer again in that game
unless the LETHAL-THIS-TURN chain above adds up on the screen in front of you.
```

## D56-3
**Finding.** Cycling. 137 windows offered a cycle row at this seat and 16 were taken; at `Opponent life:` 6 or less, 17 offered and 3 taken. `130v125` s52-s73 is 14 consecutive windows at Opponent life 2 with 12-14 untapped sources whose entire menu was a Lay Waste, a cycle and a pass - one cycle taken, and the two damage that would have ended the game never arrived. The existing gate ('Mana available 4 or more -> cast it, never cycle') is right while the land war is live and wrong once it is over; the release is 10+ of your mana against 5+ of their lands.

**BEFORE**
```
AND: when the line reads "Phase: Upkeep | It is your turn", answer CHOICE: 0 (pass) unless the option
in front of you destroys a land or kills a creature.
```

**AFTER**
```
AND THE OTHER WAY ROUND, ONCE THE LAND PLAN IS OVER, THE CYCLE IS THE ONLY LINE YOU HAVE. When
"Mana available:" is 10 or more, their land count is 5 or more, and the only row on this window
is a cycle or a pass, destroying one of nine lands changes nothing and the card you draw is the
only thing that can. CYCLE. Last corpus this deck spent fourteen consecutive windows at
"Opponent life: 2" with twelve to fourteen untapped sources and a cycling row on the screen,
took the cycle ONCE, and never found the two damage that would have ended it: 17 windows offered
a cycle at an opponent on 6 or less and 3 were taken. The gate above ("cast it, never cycle it")
is about a land war you are still winning; at 10+ mana against 5+ of their lands you are not in
one any more.
AND: when the line reads "Phase: Upkeep | It is your turn", answer CHOICE: 0 (pass) unless the option
in front of you destroys a land or kills a creature.
```

## D56-4
**Finding.** SURVIVES, second corpus. `130v162` s19: Pyrite Spellbomb into `Master of the Feast (5/5) ... {right now: takes 2 damage - SURVIVES (toughness 5)}` under `PLAN: Kill Master of the Feast with Pyrite Spellbomb`. That 5/5 flier then dealt every point of the damage that ended the game (20 -> -6 by turn 11). 33 SURVIVES rows offered, 1 taken. The break's shape is identical to wave-55's Nadaar break: a plan verb written before the row was read.

**BEFORE**
```
     creature arrives. At 3 life this deck fired one at a row reading "SURVIVES (toughness 4)",
     paid the {r} with a Talisman and went to 2 for nothing; last corpus it fired one at
     "Nadaar, Selfless Paladin ... SURVIVES (toughness 3)" while its own PLAN said "Kill Nadaar
     with Pyrite Spellbomb". The row's verdict outranks the plan's verb every time.
```

**AFTER**
```
     creature arrives. THIS IS NOW THE ONLY REMOVAL RUNG THIS DECK IS LOSING, AND IT BREAKS THE
     SAME WAY EVERY TIME: a PLAN line that says "Kill X with Y" written before the row was read.
     Last corpus a Pyrite Spellbomb went into "Master of the Feast (5/5) ... {right now: takes 2
     damage - SURVIVES (toughness 5)}" under "PLAN: Kill Master of the Feast with Pyrite
     Spellbomb"; the 5/5 flier then dealt every point of the 26 damage that ended the game, and
     the Spellbomb was the deck's only card that could have killed the 1-toughness body it drew
     next. The corpus before it was Nadaar at toughness 3. THE ROW'S VERDICT OUTRANKS THE PLAN'S
     VERB EVERY TIME: read "- DIES" or "- SURVIVES" on the row you are about to number, and if
     every row on the target list says SURVIVES, the answer one screen earlier was
     "Cast nothing right now". A creature this deck cannot kill is not a target; it is a fact
     about the game you have to play around.
```

## D56-5
**Finding.** NEW RENDER (lane B D6). `INCOMING THIS COMBAT` on 20 of 20 opponent-combat windows. For a deck with almost no blockers the value is on the NON-blockers windows: it is the number the lethal chain is racing.

**BEFORE**
```
=====================================================================
DECIDING SITUATIONS (recognise -> do)
```

**AFTER**
```
THE INCOMING TOTAL IS PRINTED ON EVERY WINDOW OF THEIR COMBAT. "INCOMING THIS COMBAT: 3
attackers, 11 unblocked damage - you would be at -1; this KILLS you" sits in the board frame from
the moment they declare, on the upkeep, main-phase and priority windows as well as the blockers
ask. It is the number the LETHAL-THIS-TURN chain races: when it says "this KILLS you", the only
answers that matter this turn are a lethal chain of your own and a block that gets the total
under your life. Everything else - a land destroyed, a card cycled, a Talisman cast - is a play
you will not be alive to use.

=====================================================================
DECIDING SITUATIONS (recognise -> do)
```

## D56-6
**Finding.** DECIDING SITUATIONS' face bullet, carrying D56-1's release so the checklist and rule #2 do not disagree.

**BEFORE**
```
- A damage spell's only targets are "the opponent, you" and the opponent is above 6 life -> do
  NOT fire it at the face. Hold the card. Nine face-burn casts in one game moved an opponent
  from 25 to 24 while they gained back more than they lost - that is not a clock, it is a card
  spent. Cast it the turn a creature or a planeswalker appears on their side, or the turn their
  life is 6 or less. The one exception is the LETHAL-THIS-TURN chain in rule #2.
```

**AFTER**
```
- A damage spell's only targets are "the opponent, you" and the opponent is above 6 life -> do
  NOT fire it at the face. Hold the card; cast it the turn a creature or a planeswalker appears
  on their side, or the turn their life is 6 or less. TWO exceptions, both in rule #2: the
  LETHAL-THIS-TURN chain, and their battlefield line reading "of which 0 are creatures" with
  TWO OR FEWER lands - a board that cannot come back is a board with nothing to hold the card
  for, and then the face row is the clock.
```

## D56-7
**Finding.** TRIM. X marker: 6 menus, 20 rows, every row carried `{X pricing:`, a marker printed on all 6 and the MARKED ROW WAS TAKEN 6 OF 6 (wave 55: 2 of 3; wave 54: 0 renders). Wave-55's P9 - the prediction I said I cared about most at this seat - PASSES N/N. Two worked examples retired.

**BEFORE**
```
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

**AFTER**
```
               Answer the marked row's number and read nothing else; it has already done both
               the kill comparison and the mana comparison, and it is already the BOTTOM of any
               collapsed run. Last corpus the marker printed on six of six menus and this deck
               answered it six times out of six - this rung is DONE, keep it there.
               A MARKER IS NOT A REASON TO CAST. It answers WHICH X, never WHETHER: the "best
               trade" form is printed even when the best trade is a bad one. WHETHER is decided
               one screen earlier, at the cast row, where "Cast nothing right now" still exists.
               If no marker is printed: the LARGEST "kills THEIRS" list whose "YOURS:" reads
               "none". Bigger is free when YOURS is none.
```

## D56-8
**Finding.** TRIM. The five-row worked menu under 'no row reads YOURS: none'. 0 takes of a `kills THEIRS: none` row this corpus and last.

**BEFORE**
```
               AND WHEN NO ROW READS "YOURS: none" AT ALL - every X on the menu costs you a body -
               the rule is the one this deck broke at 9 life against 25: NEVER TAKE A ROW WHOSE
               "kills THEIRS" READS "none". That row spends the card, kills only your own
               creatures, and leaves theirs untouched. The menu read
                 1. X = 4 {kills THEIRS: Intrepid Adversary, Elite Spellbinder, Sigarda; YOURS: Siege-Gang Commander, Goblin x3}
                 2. X = 3 {kills THEIRS: Intrepid Adversary, Elite Spellbinder; YOURS: Siege-Gang Commander, Goblin x3}
                 3. X = 2 {same kills as X=3, for 1 less mana}
                 4. X = 1 {kills THEIRS: none; YOURS: Goblin x3}
                 5. X = 0
               and this deck answered 4, under a plan that said "Kill Sigarda and Elite
               Spellbinder with Starstorm X=1" - a sentence the row it named denies in its own
               words. It killed its own three Goblins, attacked its lone Commander into a 4/4
               flier and lost. Among rows that DO name an opponent creature, take the one whose
               THEIRS list is longest; the YOURS list is the price, and a price is not a reason
               to pick the row that buys nothing. If every such row names RORIX BLADEWING or
               SIEGE-GANG COMMANDER in YOURS, do not cast Starstorm at all - that decision
               belongs at the cast row, one screen earlier, where "Cast nothing right now" still
               exists.
               This deck answered X=1 against Thraben Doomsayer and a Human token when X=2 was
               affordable and killed BOTH for "YOURS: none". The Doomsayer lived, made
               twenty-one more tokens, and the game ended 19 to -31.
```

**AFTER**
```
               AND WHEN NO ROW READS "YOURS: none" AT ALL - every X on the menu costs you a body -
               NEVER TAKE A ROW WHOSE "kills THEIRS" READS "none". That row spends the card,
               kills only your own creatures, and leaves theirs untouched. Among rows that DO
               name an opponent creature, take the one whose THEIRS list is longest; the YOURS
               list is the price, and a price is not a reason to pick the row that buys nothing.
               If every such row names RORIX BLADEWING or SIEGE-GANG COMMANDER in YOURS, do not
               cast Starstorm at all - that decision belongs at the cast row, one screen
               earlier, where "Cast nothing right now" still exists.
```

## D56-9
**Finding.** TRIM. STEP 2's top-of-collapsed-run anecdote. 1 collapsed run offered (`130v126` s70), the BOTTOM row taken - second corpus at zero.

**BEFORE**
```
       STEP 2. Then walk DOWN from that row while the next row says "same kills as X=N".
               A row that says "same kills" is the same board result for less mana; take the
               LOWEST row in that collapsed run, not the top of it. Answering the top of a
               collapsed run is a mana thrown away for nothing, and this deck has now done it
               three times - the last on a menu whose X=5 row it took with the row directly
               below reading "X = 4 {X pricing: same kills as X=5, for 1 less mana}".
```

**AFTER**
```
       STEP 2. Then walk DOWN from that row while the next row says "same kills as X=N".
               A row that says "same kills" is the same board result for less mana; take the
               LOWEST row in that collapsed run, not the top of it. Two corpora at zero here.
```

## D56-10
**Finding.** TRIM. The X=0 anecdote. 0 `X = 0` answers this corpus.

**BEFORE**
```
                     This deck answered "Cast Card Normally"
                     and then X=0 against an empty board while stuck on four lands with two Rorix
                     and two Siege-Gang uncastable in hand - the cycling row was on the same
                     screen, it costs {3}, and the card it draws was the land that game needed.
                     NEVER announce X=0. If the only X you can afford is 0, you are not casting
                     this spell.
```

**AFTER**
```
                     NEVER announce X=0. If the only X you can afford is 0, you are not casting
                     this spell.
```

## D56-11
**Finding.** TRIM. Two survival-first anecdotes. No sweep-vs-survival window arose this corpus (UNTESTED); the rule is kept, the stories are not.

**BEFORE**
```
     THEIRS list is longest, WHATEVER the YOURS list names. At 8 life against 5/5 + 5/3 + 4/5 +
     3/3 (17 power) this deck held Starstorm because the X=5 row named its own Siege-Gang
     Commander, and was dead at -2 on the next swing with the Commander still on the table.
     Rorix or the Commander on the table beats the annotation only while you are alive to
     attack with him.
     BUT A SWEEP THAT LEAVES THE KILLER STANDING IS NOT SURVIVAL. Subtract the THEIRS list from
     their line: if the creatures the sweep does NOT kill still total your life or more, the
     Starstorm buys you nothing and costs you the blocker on its YOURS list. At 4 life against a
     6/6 and a 2/2, the only affordable rows read "kills THEIRS: Luminarch Aspirant; YOURS:
     Dwarven Blastminer"; this deck cast it, the 6/6 attacked into no blocker, and the game
     ended at -2. A 1/1 in front of the 6/6 was the extra turn. Keep the blocker.
```

**AFTER**
```
     THEIRS list is longest, WHATEVER the YOURS list names. Rorix or the Commander on the table
     beats the annotation only while you are alive to attack with him.
     BUT A SWEEP THAT LEAVES THE KILLER STANDING IS NOT SURVIVAL. Subtract the THEIRS list from
     their line: if the creatures the sweep does NOT kill still total your life or more, the
     Starstorm buys you nothing and costs you the blocker on its YOURS list. Keep the blocker.
```

## D56-12
**Finding.** TRIM. #0 rule anecdote replaced by the current denominator: 109 `DRAW PUNISHERS` renders, 0 draws or cycles taken under one - sixth corpus at zero.

**BEFORE**
```
At 3 life against Underworld Dreams, Fate Unraveler and Ob Nixilis on one line, this deck cycled
a Starstorm to "dig for an answer" and dealt itself exactly its remaining 3 life. Its plan named
all three punishers in
the sentence before it drew. There is no answer worth finding at the cost of the game you are
still in.
```

**AFTER**
```
SIXTH CORPUS AT ZERO: 109 windows carried that line last corpus and no draw or cycle was taken
under one. There is no answer worth finding at the cost of the game you are still in.
```

## D56-13
**Finding.** Spark Spray's cycle-not-face line, made consistent with D56-1's release.

**BEFORE**
```
                     "{kills whichever you target: THEIRS - none; YOURS - Goblin x3 - and 1 to
                     the opponent at life 15 leaves them at 14}" is the engine telling you the
                     spell kills nothing, and 1 damage off 15 is not a clock. Last corpus this
                     deck cast it at a healthy face twice, once paying a Talisman life for the
                     privilege. One card for one point is the worst rate in the deck.
```

**AFTER**
```
                     "{kills whichever you target: THEIRS - none; YOURS - Goblin x3 - and 1 to
                     the opponent at life 15 leaves them at 14}" is the engine telling you the
                     spell kills nothing, and 1 damage off 15 is not a clock. One card for one
                     point is the worst rate in the deck - unless rule #2's release has fired
                     (0 creatures, 2 or fewer lands), and then it is the clock.
```
