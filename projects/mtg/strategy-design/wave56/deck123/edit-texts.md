# wave-56 deck123 guide edits (before -> after, each with its finding)
Start point: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt`. Every edit below is a
single exact-string replacement applied to that file, in this order.

## A56-1
**Finding.** RULE -1 record refresh. 12 mulligan looks at my two seats, 12 keeps, every one at two or more lands; 0 ships, 0 bottom asks, 0 repeated mulligans. The old sentence quoted a record that no longer exists. (123v125/126/146/152/130/162 s1.)

**BEFORE**
```
This rule is first because the ask it governs is first. Last corpus it held at every look -
ten looks, four ships, six keeps, three bottoms, no repeated mulligan and no hand shipped at
two lands - and none of the six games was decided here. Keep it that way by reading the header.
```

**AFTER**
```
This rule is first because the ask it governs is first. Last corpus it held at every look -
twelve looks across both this deck and its stablemate, twelve keeps, every one at two or more
lands, no ship, no bottom ask and no repeated mulligan - and none of the six games was decided
here. Keep it that way by reading the header.
```

## A56-2
**Finding.** RULE 6. Wave-55's E6 SHIPPED: the Greaves cast row now prints `{right now: you control 0 creatures - this equips nothing}` on 62 of 64 rows at this seat. The old paragraph's premise ('the one card whose row carries NO verdict') is now false, and the break happened anyway - `123v125` s45, a six-row menu, 11 lands, every cast row priced dead. The rung is now pure STRATEGY and the text is keyed to the emitter's own string.

**BEFORE**
```
BEING THE ONLY CAST ROW ON THE MENU IS NOT A REASON. The Greaves is the one card here whose row
carries NO "{right now: ...}" verdict, so on a two-land turn the screen can read "1. Cast Lightning
Greaves / 2. Cast nothing right now / 3. Hold priority ..." and look like a menu with one play on
it. It is not: last corpus this deck took that row at "of which 0 are creatures", tapped out, and
still had no creature nine turns later. When the Greaves is the only cast row and your battlefield
line reads 0 creatures, the answer is row 2. A one-row menu is a menu you decline.
```

**AFTER**
```
THE ROW NOW SAYS IT, SO THERE IS NOTHING LEFT TO WORK OUT. The Greaves cast row carries its own
verdict: "Cast Lightning Greaves {2} {right now: you control 0 creatures - this equips nothing}".
Sixty-two of the sixty-four Greaves rows this deck saw last corpus printed that clause, and it is
the whole rule - a row that says it equips nothing is "Cast nothing right now", and being the ONLY
cast row on the menu does not change it. Last corpus this deck read that exact clause on a six-row
menu with eleven lands out and answered it anyway, on the turn its Bloodline Keeper had just been
countered, writing "next window cast Bloodline Keeper" about a card that was in its graveyard.
A one-row menu is a menu you decline.
```

## A56-3
**Finding.** CL5, same finding as A56-2, keyed to the same literal instead of to a battlefield-line count the pilot has to re-derive.

**BEFORE**
```
 CL5. "Cast Lightning Greaves {2}" - ONLY when your own battlefield line reads "of which 1 is
    a creature" or more AND that creature is a token-maker. Read that clause before you take
    this line: "of which 0 are creatures" means the Greaves equips nothing and is a wasted turn -
    keep it for the turn a maker lands. This deck has now cast it at "of which 0 are creatures"
    four times, the latest on turn 15 against deck130 with three lands left standing.
```

**AFTER**
```
 CL5. "Cast Lightning Greaves {2}" - ONLY when the row does NOT carry
    "{right now: you control 0 creatures - this equips nothing}". That clause is the answer;
    this deck has now cast it over its own printed verdict five times.
```

## A56-4
**Finding.** RULE 1's hold paragraph. Lane A's D1 landed: 119 HOLD takes at my seats, 8 followed by a byte-identical same-turn re-ask (was 79 of 170); longest single (turn,phase) run 9 windows (was 26); identical-declined runs 10.1% of decisions, max run 5. The old text taught the pilot to expect a run that no longer happens.

**BEFORE**
```
THE ROW IS NOT KEPT WHEN A LIFE TOTAL IS TICKING, AND THAT IS NOT YOUR PROBLEM TO SOLVE. A drain
- their Sanguine Bond loop, an Underworld Dreams draw trigger, a Staff of Nin ping - moves a life
number between windows, and any change re-opens the window: this deck took the row and was asked
the byte-identical list again fifteen times in one Draw step, and twenty-six times in one Upkeep.
Answer the SAME way every time. The one thing that must not happen in such a run is a cast: the
menu that was dead at the first ask is dead at the twentieth, and "[you declined this exact list
N times already this turn]" is a counter, not new information. Last corpus this deck answered
every one of those runs with a decline or the hold row and spent nothing - that is the standard.
```

**AFTER**
```
THE ROW IS NOW KEPT. Last corpus this deck took the hold row 119 times and the byte-identical
list came back inside the same turn 8 times, against 79 of 170 the corpus before: the hold holds.
Take it at the first dead window of the turn and stop reading the menu. When a run does still
come back - a drain moves a life number and the price on a row moves with it - answer the SAME
way every time. The one thing that must not happen in such a run is a cast: the menu that was
dead at the first ask is dead at the twentieth, and "[you declined this exact list N times
already this turn]" is a counter, not new information.
```

## A56-5
**Finding.** NEW RENDER (lane B D6). `INCOMING THIS COMBAT: ...` on 20 of 20 opponent-combat windows at my seats (was 0 of 50). `123v152` s23 carried `4 attackers, 16 unblocked damage - you would be at -8; this KILLS you (of that, 10 from 2 attackers none of your creatures can block)` and the seat answered `BLOCKS: none` - CORRECT, because the only legal block was 3 of the 16. The guide's LETHAL rule as written ordered a block that could not save it; this adds the arithmetic that makes the decline legible.

**BEFORE**
```
Blocking, in this order. Step 0 decides whether you HAVE a blocker; steps 1 and 2 pick WHICH
creature blocks; the rest decide whether to block at all.
```

**AFTER**
```
THE INCOMING TOTAL IS NOW PRINTED ON EVERY WINDOW OF THEIR COMBAT, NOT ONLY ON THE BLOCKERS ASK.
"INCOMING THIS COMBAT: 4 attackers, 16 unblocked damage - you would be at -8; this KILLS you
(of that, 10 from 2 attackers none of your creatures can block)" appears in the board frame from
the moment they declare, so an instant, an edict or a token line you take in their upkeep or
main phase can be priced against it before the blockers ask ever arrives. Read the SECOND number
in the parenthesis: damage from attackers you cannot block is damage no assignment removes.
AND WHEN THE HEADER SAYS "this KILLS you" AND NO ASSIGNMENT GETS YOU ABOVE 0, "BLOCKS: none" IS
THE RIGHT ANSWER. At 8 life against 16 - 10 of it from two fliers this deck could not block -
one 2/2 in front of a 3-power attacker still ends at -5. Blocking to lose one card fewer is the
same loss. Check the arithmetic before you spend the maker: subtract the largest "deals N" you
can legally block from the header's total, and if the result still reaches your life, decline.

Blocking, in this order. Step 0 decides whether you HAVE a blocker; steps 1 and 2 pick WHICH
creature blocks; the rest decide whether to block at all.
```

## A56-6
**Finding.** NEW RENDER. `[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life and draws them a card]` - 4 prompts, 1 take (`123v146` s7, an Intruder Alarm at 14 life). UNTESTED for two corpora, now TESTED; the guide had no sentence for it.

**BEFORE**
```
=== KEY CARDS ===
```

**AFTER**
```
A ROW TAGGED "[NAMED BY THEIR ...]" IS A SURCHARGE, NOT A BAN. Silverquill Silencer and its kind
name a card in your hand and the row then prints the price in full - "Cast Intruder Alarm {2}{u}
[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life and draws them a card]". The
card is still castable and the number is already counted for you: pay it when the card is the
engine (a maker, the Alarm) and your life is above 8; decline it for a Greaves, a Skeins or a dead
removal spell. This deck paid 3 life for an Alarm at 14 last corpus, which was right, and the
Alarm was answered by a Vanishing Verse two turns later, which was not something the row could
have told it.

=== KEY CARDS ===
```

## A56-7
**Finding.** TRIM. RULE 3's Damnation prose. 25 Damnation rows offered at this seat, ALL 25 priced `destroys 0 of their creatures`, 0 casts - third corpus at zero. Two worked examples removed; the arithmetic kept.

**BEFORE**
```
AND WRITING THE THREE NUMBERS IS NOT THE SAME AS OBEYING THEM. This deck has twice written the
line and then contradicted it in the same breath - "Damnation: N 0, K 0, M 0; cast." into an empty
board, and "wipe their single creature, saving my own 5 creatures" above a row that says it
destroys five of yours. The numbers are the answer; the sentence after them is not.

```

**AFTER**
```
AND WRITING THE THREE NUMBERS IS NOT THE SAME AS OBEYING THEM. The numbers are the answer; the
sentence after them is not.

```

## A56-8
**Finding.** RULE 2. 5 Marsh Flats windows, 5 cracked at the first offer (crack rung PASSES 5/5), 4 duals + 1 basic. The one break, `123v126` s6, is a window where the pilot could already make BOTH its colours, so NO row carried `(you cannot make X right now)` and the guide's rule - which keys entirely on that parenthesis - gave no answer. The untagged tie-break is new text and is the only line added to RULE 2.

**BEFORE**
```
A colour at 0 on the mana line comes first (Intruder Alarm {2}{u} and Vision Skeins {1}{u} are
the reason: only 10 of 24 lands make blue).
```

**AFTER**
```
WHEN NO ROW CARRIES THAT PARENTHESIS AT ALL, TAKE A DUAL ANYWAY - NEVER THE BASIC. The tag only
prints for a colour you cannot currently make, so on a turn when you can already make both, every
row is untagged and the rule above gives you nothing. The tie-break is the row's own "and it adds"
clause: "adds {W} or {B}" beats "adds {B}", for the same one life and the same one card. Last
corpus's only miss here was exactly that window - "adds {B}" taken over "adds {W} or {B}" on turn
4 with one source of each colour on the line and both a {b} and a {1}{b} spell in hand.
A colour at 0 on the mana line comes first (Intruder Alarm {2}{u} and Vision Skeins {1}{u} are
the reason: only 10 of 24 lands make blue).
```

## A56-9
**Finding.** TRIM. RULE 5. 113 rows at `at 0 this does nothing` and 125 at `their choice`, 0 taken - second corpus at zero on both. Numbers refreshed, one worked example removed.

**BEFORE**
```
(2) If N is not exactly 1, do not cast an edict - not at 0, not at 2 or more. Both are held
rungs: last corpus this deck was offered 171 rows reading "they control 0 creatures - at 0 this
does nothing" and 52 reading "- they choose which one", and took NONE of them. Keep that at zero.
(3) A creature
listed under "ON THE STACK, waiting to resolve" is NOT on the battlefield and does not change
N. Naming a creature spell on the stack in your PLAN is the mistake itself: at N=0 the opponent
sacrifices nothing and you have thrown away the card.
AT N = 1 THE EDICT IS THE CAST, NOT THE SLIP. Offered "they control 1 creature - Silverquill
Silencer is sacrificed" twice at two mana, this deck answered "Cast nothing", then shrank the
3/2 with a Tragic Slip and took nine more damage from it in a game lost at -2. A toughness-2
creature is the edict's job, not the Slip's.
```

**AFTER**
```
(2) If N is not exactly 1, do not cast an edict - not at 0, not at 2 or more. Both are held
rungs, two corpora running: 113 rows reading "at 0 this does nothing" and 125 reading "their
choice" last corpus, and NONE of them taken. Keep that at zero. (3) A creature listed under
"ON THE STACK, waiting to resolve" is NOT on the battlefield and does not change N.
AT N = 1 THE EDICT IS THE CAST, NOT THE SLIP. A toughness-2 creature is the edict's job, not
the Slip's.
```

## A56-10
**Finding.** RULE 5's defender carve-out, re-cited to this corpus's evidence: `Wall of Omens (0/4) [defender] is sacrificed, they gain 4` was offered 5 times in two turns (`123v126` s8-s12) and declined 5 times. Wave-55's A55-3 is TESTED and PASSES.

**BEFORE**
```
its toughness for something that was never going to kill you. The row prints the body's own tags
inside its verdict, so the test is on the screen: "they control 1 creature - Fog Bank (0/2)
[flying, defender] is sacrificed, you gain 2" is the row you decline, and last corpus this deck
took exactly that one, tapping out for it on turn 10 against a deck whose threats were an
Underworld Dreams and an Ob Nixilis that no edict can touch. Two life is not a card. A 1-power creature is NOT in that class when its "{card text:}" gives the rest of their
```

**AFTER**
```
its toughness for something that was never going to kill you. The row prints the body's own tags
inside its verdict, so the test is on the screen: "they control 1 creature - Wall of Omens (0/4)
[defender] is sacrificed, they gain 4" is the row you decline, and last corpus this deck was
offered exactly that one five times in two turns and declined all five. Keep that at five of
five. A 1-power creature is NOT in that class when its "{card text:}" gives the rest of their
```

## A56-11
**Finding.** TRIM. Converter-block worked example. `LIFE-TO-DAMAGE CONVERTER` rendered on 54 prompts, 0 edicts cast under it.

**BEFORE**
```
With that block on the screen an edict at the opponent is a two-way swing against you: the row's
own "they gain N" becomes N on their total AND N off yours: at 14 against 26, with that block
printed and their only creature an "Overgrown Battlement (0/4) [defender]", a Devour Flesh made
it 10 against 30 in one window, for a wall that had never attacked.
```

**AFTER**
```
With that block on the screen an edict at the opponent is a two-way swing against you: the row's
own "they gain N" becomes N on their total AND N off yours.
```

## A56-12
**Finding.** TRIM. RULE 1's plan-contradiction paragraph. `plan_choice_conflict` fired 0 times at my seats (was 6); 0 `named_row_reask` on a token menu. Lane A's D16 narrowing plus wave-55's A55-TRIM closed it.

**BEFORE**
```
    THE PLAN LINE MAY NOT CONTAIN THE WORD "PASS", OR "STOP REACHED", ON A WINDOW WHERE YOUR
    CHOICE LINE TAKES A ROW. "this window:" is the ONLY place the action is named, and what it
    names must be the same action the CHOICE line above it named. A plan reading "creating 2
    humans to reach M=30 (stop reached)" beside "CHOICE: 2 (... x2)", or "Stop reached. Pass
    token creation." beside "CHOICE: 1 (Create human ...)", is read by the engine as a reply
    that reverses itself: it is stamped, thrown away and asked again. That cost this deck FIVE
    re-asks in one game last corpus and two more in another, and in four of the seven the
    re-ask came back with the identical answer, so the only thing spent was the window. If the
    stop IS reached, the CHOICE line is "CHOICE: 0 (pass)" and the plan says pass. If it is not,
    neither line says pass.
```

**AFTER**
```
    THE PLAN LINE MAY NOT CONTAIN THE WORD "PASS", OR "STOP REACHED", ON A WINDOW WHERE YOUR
    CHOICE LINE TAKES A ROW. "this window:" is the ONLY place the action is named, and what it
    names must be the same action the CHOICE line above it named; a reply that reverses itself
    is stamped, thrown away and asked again. This cost seven re-asks two corpora ago and ZERO
    last corpus - keep it there. If the stop IS reached, the CHOICE line is "CHOICE: 0 (pass)"
    and the plan says pass. If it is not, neither line says pass.
```

## A56-13
**Finding.** TRIM. Sweeper-upkeep anecdote. The rule is kept; the 323-Vampire story is three corpora old and the rung has not broken since.

**BEFORE**
```
YOUR OWN UPKEEP IS THE WORST WINDOW AGAINST A SWEEPER. Tokens made in your own upkeep sit
through their whole turn before they attack: the 323 Vampires this deck made in its own turn-9
upkeep were swept on turn 10 and the game ended 0 to 62. Against a deck whose log shows
Starstorm, Supreme Verdict or Damnation (deck130, deck125, the mirror), the answer in your upkeep
is "CHOICE: 0 (pass)" and the chain fires in THEIR turn. Against a deck with no sweeper the
own-turn chain has won four games running at the L + C + 3 stop.
```

**AFTER**
```
YOUR OWN UPKEEP IS THE WORST WINDOW AGAINST A SWEEPER. Tokens made in your own upkeep sit
through their whole turn before they attack. Against a deck whose log shows Starstorm, Supreme
Verdict or Damnation (deck130, deck125, the mirror), the answer in your upkeep is
"CHOICE: 0 (pass)" and the chain fires in THEIR turn.
```

## A56-14
**Finding.** TRIM. Alarm-before-tap anecdote; the mechanism sentence is kept.

**BEFORE**
```
WITH THE ALARM IN HAND AND CASTABLE THIS TURN, THE MAKER WAITS UNTAPPED UNTIL THE ALARM HAS
RESOLVED. Under Intruder Alarm a tapped creature untaps only when a creature ENTERS; nothing
of theirs may enter for a whole turn. This deck tapped its Doomsayer in its own upkeep, cast
the Alarm in the main phase after it, and then went through the opponent's entire next turn
with no "Create human" line on any screen - the Doomsayer sat tapped, Brutal Cathar exiled
it, and the game ended two turns later at -10. Land, Alarm, THEN the first tap - or, with
the Alarm not castable this turn, tap freely: a maker with no Alarm untaps on its own.
```

**AFTER**
```
WITH THE ALARM IN HAND AND CASTABLE THIS TURN, THE MAKER WAITS UNTAPPED UNTIL THE ALARM HAS
RESOLVED. Under Intruder Alarm a tapped creature untaps only when a creature ENTERS; nothing of
theirs may enter for a whole turn, so a maker tapped before the Alarm resolves sits tapped through
their entire turn. Land, Alarm, THEN the first tap - or, with the Alarm not castable this turn,
tap freely: a maker with no Alarm untaps on its own.
```

## A56-15
**Finding.** TRIM + refresh. All-dead menus: 85 of 197 casting menus at this seat, ONE cast off them (the Greaves of A56-2). Wave 55 was 2 of 108, wave 54 was 5. The five-card story is retired for the current number.

**BEFORE**
```
Read the {right now: ...} and [second copy: ...] clauses on every row before you pick: a menu
whose rows all read "destroys 0 of their creatures ... 0 of yours", "they control 0 creatures -
at 0 this does nothing" and "[second copy: ... a second copy changes nothing]" is a menu with
nothing on it, and "Cast nothing right now" is the only row that keeps a card. Last corpus this
deck, holding a full grip against a counter deck, spent FIVE cards off such menus in one game -
two dead Intruder Alarms at turns 42 and 58, a third at turn 36, a Damnation at "destroys 0" on
turn 40 and a Tribute to Hunger at "they control 0" on turn 44 - and wrote the reason in its own
plan: "cast second Intruder Alarm (dead, but only way to proceed)". There is no rule in this file
```

**AFTER**
```
Read the {right now: ...} and [second copy: ...] clauses on every row before you pick: a menu
whose rows all read "destroys 0 of their creatures ... 0 of yours", "they control 0 creatures -
at 0 this does nothing" and "[second copy: ... a second copy changes nothing]" is a menu with
nothing on it, and "Cast nothing right now" is the only row that keeps a card. Last corpus 85 of
this deck's 197 casting menus were that menu and it spent ONE card off them - the Greaves above.
Take that to zero. There is no rule in this file
```

## A56-16
**Finding.** TRIM. DECIDING SITUATIONS' Greaves entry re-keyed to the render string (duplicate of A56-2's finding).

**BEFORE**
```
- "Cast Lightning Greaves {2}" is listed and your battlefield line reads "of which 0 are
  creatures": do not cast it. Removal, or "Cast nothing right now". This is the single most
  repeated wasted cast in this deck's history - twice again in the last corpus, once at 7 life
  with two mana and the Greaves taking both, once at 15 life with six sources open and a
  Bloodline Keeper in hand. The clause to read is on YOUR battlefield line, before the row.
```

**AFTER**
```
- The Greaves row reads "{right now: you control 0 creatures - this equips nothing}": do not
  cast it, whatever else is or is not on the menu. Removal, or "Cast nothing right now".
```

## A56-17
**Finding.** TRIM. Vision Skeins anecdote; the rule is kept. 6 Skeins casts this corpus, none under a `DRAW PUNISHERS` line.

**BEFORE**
```
  {1}{u}" is listed: cast it, this window. Twelve consecutive "Cast nothing right now" answers
  with eight to eleven mana open and a Skeins in hand were twelve turns of not looking for the
  maker, in a game that ended 0 to 43 without one.
```

**AFTER**
```
  {1}{u}" is listed: cast it, this window. A turn spent not looking for the maker is a turn
  this deck does not have.
```
