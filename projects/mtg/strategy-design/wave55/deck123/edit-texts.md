# wave-55 deck123 guide edits (before -> after)

Every edit below is applied to `wave55/deck123/strategy.txt`, which starts from the live
guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (wave-54 reviewer revisions
`5160eb1cb` + boundary pass `dd0cd2d74`). Tags beginning `A55-TRIM`/`D55-TRIM` are pure
length trims taken to keep the file inside the 41-71 KB pool band; they carry no new rule.

## A55-9
**Finding.** RULE -1's opener claimed 'Two of six games were decided here'. FALSE this corpus: deck123 took 10 mulligan looks (4 ships, 6 keeps, 3 bottoms), every one agreeing with the header rule, and no game turned on it. Retuned to the current record so the rung reads as HELD, not as a live bleed. Evidence: 123v125 s1, 123v126 s1, 123v130 s1-s3, 123v146 s1-s3, 123v152 s1-s3, 123v162 s1.

BEFORE:
```
This rule is first because the ask it governs is first. Two of six games were decided here,
before turn one, and neither of them was close.
```
AFTER:
```
This rule is first because the ask it governs is first. Last corpus it held at every look -
ten looks, four ships, six keeps, three bottoms, no repeated mulligan and no hand shipped at
two lands - and none of the six games was decided here. Keep it that way by reading the header.
```

## A55-7
**Finding.** The HOLD narration was wave-54's ('32 takes, taken LATE'). This corpus: 337 renders / 106 takes at deck123, taken EARLY, and 0 casts inside any repeated dead run. The paragraph now says so, and adds the residual the engine cannot yet fix: a ticking life total re-opens the byte-identical window however many times the row is taken. Evidence: 123v162 s36-s50 (15 identical asks in one Draw step, HOLD taken at s36 and s42), 123v126 s57-s87 (26 in one Upkeep, HOLD taken 6 times), 123v125 s104-s109.

BEFORE:
```
this deck took it 32 times last corpus and took it LATE almost every time - turn 41 declined ten
identical menus and then held on the Cleanup ask, turn 55 declined eleven and held at End, turns
43 and 57 declined twelve and thirteen and never held at all. A hold taken in the Cleanup step
saves nothing; a hold taken in their upkeep saves the whole turn. The board changing brings the
window straight back, so holding early costs you no window you would have used.
```
AFTER:
```
this deck took it 106 times last corpus, and it now takes it EARLY - the row is answered on the
first or second dead window of most turns, and it saved this deck from every dead cast it was
offered in a counter-deck game it still lost on the clock. Keep doing that. A hold taken in the
Cleanup step saves nothing; a hold taken in their upkeep saves the whole turn.
THE ROW IS NOT KEPT WHEN A LIFE TOTAL IS TICKING, AND THAT IS NOT YOUR PROBLEM TO SOLVE. A drain
- their Sanguine Bond loop, an Underworld Dreams draw trigger, a Staff of Nin ping - moves a life
number between windows, and any change re-opens the window: this deck took the row and was asked
the byte-identical list again fifteen times in one Draw step, and twenty-six times in one Upkeep.
Answer the SAME way every time. The one thing that must not happen in such a run is a cast: the
menu that was dead at the first ask is dead at the twentieth, and "[you declined this exact list
N times already this turn]" is a counter, not new information. Last corpus this deck answered
every one of those runs with a decline or the hold row and spent nothing - that is the standard.
```

## A55-6
**Finding.** NEW rung. 123v126 s53: 37 creatures on the battlefield, opponent at 20, and the attack screen printed FOUR rows - with Intruder Alarm out nothing untaps in the untap step, so only creatures made on the OPPONENT'S turn can attack. The guide justified 'fire on their turn' by sweepers alone; this is the stronger reason and it is the shape of the loss.

BEFORE:
```
Timing: the tokens you make CANNOT attack until your next turn, so making them early in your own
turn buys you nothing this turn and leaves them standing through the opponent's whole turn. Fire
the chain on the OPPONENT'S turn - the lines are offered there, in their upkeep, their main phase
and their attack step. A token made then blocks that same turn and attacks on your very next turn,
and a sweeper cast on their turn catches fewer of them.
```
AFTER:
```
Timing: the tokens you make CANNOT attack until your next turn, so making them early in your own
turn buys you nothing this turn and leaves them standing through the opponent's whole turn. Fire
the chain on the OPPONENT'S turn - the lines are offered there, in their upkeep, their main phase
and their attack step. A token made then blocks that same turn and attacks on your very next turn,
and a sweeper cast on their turn catches fewer of them.
THE ALARM IS ALSO WHY A BIG M IS NOT A BIG ATTACK. Nothing untaps in your untap step while
Intruder Alarm is out, so the creatures on "Your creatures that can attack:" are the ones you made
on the OPPONENT'S turn and have not spent - never the ones you made this turn. Last corpus this
deck stood on 37 creatures with the opponent at 20 and the attack screen printed FOUR rows, then
died before its next turn. The chain fired in its own main phase; every token it made was
summoning sick when the attack step arrived. M is the stop's number. The attack list is the
attack's number, and only the opponent's turn grows it.
```

## A55-8
**Finding.** The fetch row now prints its colours AND marks the ones you cannot currently make ('and it adds {W} or {B} (you cannot make {W} right now)') - wave-54's E7/G4 render item is closed. The rule is now pinned to that printed clause. Evidence of the break it fixes: 123v125 s4 fetched a basic Swamp off a menu listing Scrubland, Tundra and Underground Sea each carrying the parenthesis, against deck125 (no land destruction).

BEFORE:
```
A colour at 0 on the mana line comes first (Intruder Alarm {2}{u} and Vision Skeins {1}{u} are
the reason: only 10 of 24 lands make blue). After that, the hand decides: with Tundra and
Underground Sea out and a Doomsayer plus a Keeper in hand, Scrubland is the fetch - it is the
second W AND the second B. This deck fetched a second Sea there; the Doomsayer sat in hand
from turn 1 to turn 14 with "{W} 1" on the mana line, and the game was lost at 0.
```
AFTER:
```
THE ROW NOW NAMES THE COLOURS AND MARKS THE MISSING ONE. Every fetch row ends
"...and it adds {W} or {B}", and a colour you cannot currently produce is tagged in the row
itself: "and it adds {W} or {B} (you cannot make {W} right now)". Take a row carrying that
parenthesis, and among several take the one that supplies the pip your hand needs twice. That
clause is the whole rule and it is free. Last corpus this deck answered "targeting Swamp" - a
basic, one colour, no parenthesis - off a menu that listed Scrubland, Tundra and Underground Sea
each marked "(you cannot make {W} right now)", against a deck with no land destruction.
A colour at 0 on the mana line comes first (Intruder Alarm {2}{u} and Vision Skeins {1}{u} are
the reason: only 10 of 24 lands make blue). After that, the hand decides: with Tundra and
Underground Sea out and a Doomsayer plus a Keeper in hand, Scrubland is the fetch - it is the
second W AND the second B.
```

## A55-1a
**Finding.** Compressed the wave-54 anecdote and added this corpus's second instance of the same failure shape (the plan contradicting its own numbers). Evidence: 123v125 s87, 123v152 s104.

BEFORE:
```
AND WRITING THE THREE NUMBERS IS NOT THE SAME AS OBEYING THEM. Last corpus this deck finally wrote
the line - "PLAN: Damnation: N 0, K 0, M 0; cast." - and cast it anyway, on turn 40, into an empty
board, against a deck that had countered everything else it owned. N of 0 is "hold" no matter what
the second half of your own sentence says.
```
AFTER:
```
AND WRITING THE THREE NUMBERS IS NOT THE SAME AS OBEYING THEM. This deck has twice written the
line and then contradicted it in the same breath - "Damnation: N 0, K 0, M 0; cast." into an empty
board, and "wipe their single creature, saving my own 5 creatures" above a row that says it
destroys five of yours. The numbers are the answer; the sentence after them is not.
```

## A55-1b
**Finding.** THE decisive misplay at deck123 this corpus. 123v152 s104 (turn 28, 2 life vs 18): took 'Cast Damnation {4}{b}{b} [from exile] {right now: destroys 1 of their creature (1 without a restriction against attacking), 5 of yours}' under 'PLAN: ... saving my own 5 creatures'. Its board was empty in the next window. The M>=4 stop already existed; it is now anchored to the row's LAST number and given the repro. The same menu carried an identical-verdict 'Cast Damnation {2}{b}{b}' at two mana less (row 1), which is the second half of the edit.

BEFORE:
```
THE STOP, and why (this rule was broken at 14 against 6): if YOUR "of which N are creatures"
number is 4 or more, do not cast Damnation at all, whatever theirs says. Four bodies is a board
you win with; there is no opposing board this deck needs to trade four of its own creatures to
answer. Take a token activation, a removal spell or "Cast nothing right now" instead.
```
AFTER:
```
THE STOP, AND IT IS THE LAST NUMBER ON THE ROW: if M - "M of yours", the number after the final
comma in "{right now: destroys N of their creatures (K ...), M of yours}" - is 4 or more, do not
cast Damnation at all, whatever N says. Four bodies is a board you win with; there is no opposing
board this deck needs to trade four of its own creatures to answer. Take a token activation, a
removal spell or "Cast nothing right now" instead. READ THAT LAST NUMBER OUT LOUD BEFORE YOU
ANSWER, because the sentence you are about to write will contradict it: at 2 life against 18,
holding three Humans, a Bloodline Keeper and a Thraben Doomsayer, this deck answered a row reading
"destroys 1 of their creature (1 without a restriction against attacking), 5 of yours" under the
plan "wipe opponent's Katilda and their single creature, saving my own 5 creatures". Its board was
empty in the next window and it never made another creature. One of theirs is never worth five of
yours, and "saving" is not something a sweeper does.
AND WHEN THE SAME CARD IS ON THE MENU TWICE, TAKE THE CHEAPER ROW. Elite Spellbinder's exile row
is a second, more expensive copy of a card you may also hold: "Cast Damnation {4}{b}{b} [from
exile]" and "Cast Damnation {2}{b}{b}" printed identical "{right now:}" clauses on one menu and
this deck answered the {4}{b}{b} row - two mana for the same board. Same card, same verdict:
the smaller cost.
```

## A55-2
**Finding.** Fifth corpus with a Lightning Greaves cast at 'of which 0 are creatures'. 123v130 s11: a THREE-row menu (Greaves / Cast nothing / Hold), tapping out, with Thraben Doomsayer stuck in hand. The new clause names the trap that produced it - the Greaves row is the one cast row here that carries no '{right now: ...}' verdict, so a menu with it as the only cast row looks like a menu with one play.

BEFORE:
```
AND DO NOT CAST THE GREAVES ONTO AN EMPTY BOARD. On a battlefield line reading "of which 0 are
creatures" the Greaves equips nothing and the {2} is a token-maker you did not deploy; this deck
cast it there on turn 15 of a game it lost with two lands left.
```
AFTER:
```
AND DO NOT CAST THE GREAVES ONTO AN EMPTY BOARD. On a battlefield line reading "of which 0 are
creatures" the Greaves equips nothing and the {2} is a token-maker you did not deploy; this deck
cast it there on turn 15 of a game it lost with two lands left.
BEING THE ONLY CAST ROW ON THE MENU IS NOT A REASON. The Greaves is the one card here whose row
carries NO "{right now: ...}" verdict, so on a two-land turn the screen can read "1. Cast Lightning
Greaves / 2. Cast nothing right now / 3. Hold priority ..." and look like a menu with one play on
it. It is not: last corpus this deck took that row at "of which 0 are creatures", tapped out, and
still had no creature nine turns later. When the Greaves is the only cast row and your battlefield
line reads 0 creatures, the answer is row 2. A one-row menu is a menu you decline.
```

## A55-3
**Finding.** 123v162 s18: 'Cast Tribute to Hunger {2}{b} {right now: they control 1 creature - Fog Bank (0/2) [flying, defender] is sacrificed, you gain 2}', taken, tapping out, against a deck whose actual threats (Underworld Dreams, Ob Nixilis) no edict touches. Primitive check: Fog Bank mtg.txt:42416 is 0/2 defender with preventAllCombatDamage both ways - it could never have dealt a point. The rule existed; it now quotes the row shape that fires it.

BEFORE:
```
Do not spend an edict on a creature that is not hurting you: if their only creature reads
"[defender]" or has power 0, keep the edict - you would pay a card and hand them life equal to
its toughness for something that was never going to kill you.
```
AFTER:
```
Do not spend an edict on a creature that is not hurting you: if their only creature reads
"[defender]" or has power 0, keep the edict - you would pay a card and hand them life equal to
its toughness for something that was never going to kill you. The row prints the body's own tags
inside its verdict, so the test is on the screen: "they control 1 creature - Fog Bank (0/2)
[flying, defender] is sacrificed, you gain 2" is the row you decline, and last corpus this deck
took exactly that one, tapping out for it on turn 10 against a deck whose threats were an
Underworld Dreams and an Ob Nixilis that no edict can touch. Two life is not a card.
```

## A55-4
**Finding.** 123v125 s83: cast a dead Idyllic Tutor ('every enchantment left in your library is a copy of one you already control') writing 'dead, but only way to dig for Bloodline Keeper against counter deck'. The counter-deck bait licence was being read as blanket permission. It now has one mechanical test: the maker must be a numbered row on the SAME window's menu.

BEFORE:
```
a maker a turn into the graveyard. THIS IS THE ONLY REASON TO CAST A ROW YOUR OWN READING CALLS
DEAD, IT NEEDS THE MAKER BEHIND IT IN THE SAME WINDOW, AND IT IS NOT A LICENCE FOR THE REST OF
THE GAME: last corpus, in this exact matchup, this deck spent three Alarms, a Damnation and a
Tribute off dead rows on turns 36 to 58 with NO maker in hand to follow any of them.
```
AFTER:
```
a maker a turn into the graveyard. THIS IS THE ONLY REASON TO CAST A ROW YOUR OWN READING CALLS
DEAD, AND IT HAS ONE TEST: THE MAKER MUST BE A ROW ON THIS SAME WINDOW'S MENU. Not in your
library, not "next turn", not a card you hope Vision Skeins finds - a numbered "Cast Bloodline
Keeper" or "Cast Thraben Doomsayer" line you can answer the moment the bait resolves. Without
that line the bait is just the dead cast, and "dig for a maker" is the sentence that disguises it:
last corpus this deck cast a Tutor its own row priced dead, writing "dead, but only way to dig for
Bloodline Keeper against counter deck", with no creature in hand and none on the menu.
```

## A55-TRIM
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
THESE LABELS ARE "CL0" TO "CL8" AND THEY ARE NOT CHOICE NUMBERS. They are a PRIORITY ORDER over
cards; the number you write on your CHOICE line is the number printed beside that card ON THIS
WINDOW'S MENU, and a card that is not on this window's menu has no number and cannot be named.
Last corpus this deck wrote "CHOICE: 1 (Cast Bloodline Keeper)" three times and "CHOICE: 7 (Cast
Vision Skeins)" twice with neither card on the menu - those are this list's old entry numbers,
answered as if they were rows. Eight windows were spent re-asking, and one re-ask came back
"CHOICE: 7 (Cast Bloodline Keeper)", which is neither. Find the card ON THE MENU first; the CL
label only tells you WHICH of the cards that ARE on the menu you take.
```
AFTER:
```
THESE LABELS ARE "CL0" TO "CL8" AND THEY ARE NOT CHOICE NUMBERS. They are a PRIORITY ORDER over
cards; the number you write on your CHOICE line is the number printed beside that card ON THIS
WINDOW'S MENU, and a card that is not on this window's menu has no number and cannot be named.
The remaining leak is not the labels, it is naming a card or an ability that is not on the screen
at all: "CHOICE: 2 (Cast Thraben Doomsayer)" on a menu whose rows were a Greaves and two passes,
and "CHOICE: 2 (Create human with Thraben Doomsayer)" on a CASTING menu - the token line is its
own ask and never appears among the Cast rows. Both cost a re-ask and then the whole window.
Read the row you are numbering before you name it.
```

## A55-5a
**Finding.** STALE LITERAL. RULE 1 quoted the wave-54 HOLD row ('Pass priority, and do not ask me again this turn...') which renders ZERO times in this corpus; the live string is 'Hold priority for the rest of this turn: pass now, ... {taking this row skips the rest of this turn's identical windows}' (655 renders at my seats). Also records that the row is now offered on the pilot's OWN turn (42 own-turn takes at deck123).

BEFORE:
```
Cleanup with the same rows, and the LAST row of every one of them now says how to stop that, in these words:
"Pass priority, and do not ask me again this turn unless the board changes (any change re-opens
this window; you give up no cast)". When the only cast row is dead
```
AFTER:
```
Cleanup with the same rows - and so does the same ask on YOUR OWN turn now - and the LAST row of
every one of them says how to stop that, in these words: "Hold priority for the rest of this turn:
pass now, and do not ask me again unless the board changes (any change re-opens this window; you
give up no cast) {taking this row skips the rest of this turn's identical windows}".
When the only cast row is dead
```

## A55-5b
**Finding.** Same stale literal in DECIDING SITUATIONS, plus the bullet compressed and re-aimed at the drain case.

BEFORE:
```
- It is the OPPONENT'S turn, you have already answered "Cast nothing right now" to this exact
  list, and the last row reads "Pass priority, and do not ask me again this turn unless the board
  changes (any change re-opens this window; you give up no cast)": take THAT row at the FIRST
  such ask of the turn, instead of declining again. It is the same decision said once for the
  whole turn, and a board change re-opens it - you give up nothing. "[you declined this exact
  list N times already this turn]" is not new information and is not a reason to cast: this deck
  saw that line at 1 life and cast the dead Tragic Slip it had just declined.
```
AFTER:
```
- The last row reads "Hold priority for the rest of this turn: pass now, ...", on either
  player's turn: take THAT row at the FIRST dead ask of the turn instead of declining again. A
  life total ticking between windows re-opens it however many times; answer the same way, and
  never let the count become a cast. "[you declined this exact list N times already this turn]"
  is a counter, not new information.
```

## A55-TRIM2
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
- "Cast Damnation {2}{b}{b}" is listed: read its row, "{right now: destroys N of their
  creatures (K without a restriction against attacking), M of yours}" - "(K able to attack
  right now)" on their turn, "(all of them carry a restriction against attacking)" = K of 0.
  Write "Damnation: N <>, K <>, M <>" in your PLAN first. M at 0 and K at 2 or more - cast it,
  above everything else on the menu, a maker, the Tutor and the Alarm included. N bigger than M and K at 1 or more - cast it. K at 0 on Perimeter Captain or
  Pride Guardian - cast it, they are CHECK 1's wall. Any other K at 0, or N exactly 1 with a
  Tragic Slip or an edict on the menu that kills it - the one-for-one, and keep the Damnation.
  M bigger than N - take another action and keep it. N at 0 - never, whatever the plan says.
```
AFTER:
```
- "Cast Damnation {2}{b}{b}" is listed: write "Damnation: N <>, K <>, M <>" from the row first
  ("(all of them carry a restriction against attacking)" = K of 0). M at 4 or more - never.
  M at 0 and K at 2 or more - cast it, above a maker, the Tutor and the Alarm. N bigger than M
  and K at 1 or more - cast it. K at 0 on Perimeter Captain or Pride Guardian - cast it, they
  are CHECK 1's wall. Any other K at 0, or N exactly 1 with a Tragic Slip or an edict on the
  menu that kills it - the one-for-one, and keep the Damnation. M bigger than N, or N at 0 -
  never, whatever the plan says. Two Damnation rows on one menu: the cheaper number.
```

## A55-TRIM3
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
    THE NUMBER ON YOUR CHOICE LINE IS THE PLAN LINE'S ANSWER, NOT A GUESS WRITTEN BEFORE IT.
    Your reply's first line is the CHOICE and the subtraction comes after it, so do the
    subtraction BEFORE you write that line: if the row's "you control M creatures right now" is
    already at or past your stop, the first line is "CHOICE: 0 (pass)" - not "x1", not "x3", and
    never "x0", which the engine carried out ONE time. Windows that wrote "this window: pass" in
    the PLAN under a CHOICE naming x0, x1, x3 and x25 were each carried out, and the x25 put 58
    creatures on a board whose stop was 30. The last leak is the first line disagreeing with the
    plan beneath it - and, last corpus, taking x17 and x13 at M of 45 against a stop of 34.
```
AFTER:
```
    THE NUMBER ON YOUR CHOICE LINE IS THE PLAN LINE'S ANSWER, NOT A GUESS WRITTEN BEFORE IT.
    Do the subtraction BEFORE you write the first line: if the row's "you control M creatures
    right now" is already at or past your stop, that line is "CHOICE: 0 (pass)" - not "x1", not
    "x3", never "x0". A CHOICE naming a number under a PLAN that says "pass" is carried out as
    the number, every time.
```

## A55-TRIM4
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
BRANCH B - exactly ONE of SANGUINE BOND or EXQUISITE BLOOD is on the line: the gain is finite, so
you are never removing it and you attack on ARITHMETIC instead of on the name. Count two numbers off the screen: A, the creatures
on your "Your creatures that can attack:" list, and B, their UNTAPPED creatures on their
battlefield line. Every one of their bodies that blocks gains them at most 3 (Pride Guardian) or
2 (Perimeter Captain, once per blocking defender) or 1 (a lifelink blocker), and Sanguine Bond
turns each of those gains into that much off YOUR life.
  - Attack when A is at least 3 times B plus 3. At that width the unblocked damage beats every
    life they can gain from the blocks, and it keeps beating it every turn after.
  - Below that, answer "ATTACK: none", and spend the window making more tokens. A is the number
    you grow; B is the number they have to keep paying for.
  - Lethal overrides both branches: if the total power of what you can send is at or above their
    printed life, send it all - a life gain that happens after they are dead is not a life gain.
```
AFTER:
```
BRANCH B - exactly ONE of SANGUINE BOND or EXQUISITE BLOOD is on the line: the gain is finite, so
you attack on ARITHMETIC instead of on the name. A is the length of your "Your creatures that can
attack:" list - the LIST, never your creature count; B is their UNTAPPED creatures. Attack when A
is at least 3 times B plus 3; below that answer "ATTACK: none" and make more tokens on their turn.
  - Lethal overrides both branches: if the total power of what you can send is at or above their
    printed life, send it all - a life gain that happens after they are dead is not a life gain.
```

## A55-TRIM5
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
- The mulligan ask prints "(keeping 5)" or a smaller number and the hand holds one land: keep
  it - a fresh seven shaved to four is the same land odds with one card fewer. The plan shown
  above that ask was written about a hand you already threw away; it is not evidence about
  this one.
- Your plan names a spell and "Mana available: N total" is smaller than that spell's mana count
  ({2}{u} = 3, {2}{b}{b} = 4): play a land, do not re-state the plan.
```
AFTER:
```
- The mulligan ask prints "(keeping 5)" or lower and the hand holds one land: keep it. The plan
  above that ask was written about a hand you already threw away.
- Your plan names a spell and "Mana available: N total" is smaller than that spell's mana count
  ({2}{u} = 3, {2}{b}{b} = 4): play a land, do not re-state the plan.
```

## A55-TRIM6
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
- A "Cast <card> ... [from exile] {castable from exile - a legal cast, not a dead row; it costs
  {2} more than printed, already counted in the cost shown}" row (Elite Spellbinder took it):
  a real cast at the card's own cast-list entry, priced by the number already printed on it.
```
AFTER:
```
- A "Cast <card> ... [from exile] {castable from exile ...}" row (Elite Spellbinder took it):
  a real cast at that card's own cast-list entry, priced by the number printed on it - but if
  the same card is ALSO on the menu from your hand, take the cheaper row.
```

## A55-TRIM7
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
- A sacrifice ask that lists YOUR permanents one row each ("Sacrifice with Emrakul, the Aeons
  Torn" - annihilator, six picks): send LANDS, tapped ones first; Intruder Alarm and any
  creature go last, after every land. With eleven lands and one Alarm on the screen this deck
  sent the Alarm second, twice in one game.

```
AFTER:
```
- A sacrifice ask that lists YOUR permanents one row each (annihilator): send LANDS, tapped ones
  first; Intruder Alarm and any creature go last, after every land.

```

## A55-10
**Finding.** Cleanup discard rows now carry partial verdicts ('{dead right now: 0 legal targets on the board for it}', '{spare: you control N lands}'). Told the pilot to use them, and warned that their ABSENCE is not a live verdict - only 21 of 146 rendered discard rows carried one at my seats (see the seat file's engine list, item E4).

BEFORE:
```
  Never your only maker, never your only Intruder Alarm, NEVER YOUR ONLY DAMNATION: this deck
  discarded its single Damnation in three separate windows while holding a spare Thraben
  Doomsayer, a spare Scrubland, and three Devour Flesh plus two Bloodline Keepers respectively.
  Read the number back against the card your PLAN names next.
```
AFTER:
```
  Never your only maker, never your only Intruder Alarm, NEVER YOUR ONLY DAMNATION. Some rows
  carry the verdict for you - "{dead right now: 0 legal targets on the board for it}",
  "{spare: you control N lands}" - and a row that carries one is the send; a row that carries
  none is not thereby live. Read the number back against the card your PLAN names next.
```

## A55-11
**Finding.** The DECIDING SITUATIONS Sanguine Bond bullet still printed the Branch-B arithmetic for the BOTH-halves case, contradicting Branch B2 above it. Split into the one-half and both-halves cases and keyed to the render's own 'their life LOOP is in play' line (6 renders, 123v126).

BEFORE:
```
- Their battlefield line names Sanguine Bond or Exquisite Blood: you cannot remove either one -
  attack when your attacker count is at least 3 times their untapped creature count plus 3, or
  when your total power is at or above their printed life. Otherwise make more tokens.
```
AFTER:
```
- Their battlefield line names ONE of Sanguine Bond / Exquisite Blood: you cannot remove it -
  attack when your ATTACK LIST is at least 3 times their untapped creature count plus 3, or when
  your total power is at or above their printed life. BOTH of them, with the render printing
  "their life LOOP is in play": "ATTACK: none" while any untapped creature they control can gain
  them a point, whatever your creature count is. Make tokens on their turn and block.
```

## A55-TRIM8
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
- The blockers prompt says "- NOT lethal: block only where the trade favors you." and stops
  there. That is the form it takes whenever their life is at or above yours: the engine does not
  print the "taking damage while ahead on LIFE is often correct" half unless you are the player
  it describes. A token goes in front of the biggest attacker, because a 2/2 you can remake is
  not a cost and the "you would be at" number is the life your combo has to live on next turn.
  At 7 against 12 this deck answered "BLOCKS: none" to a lone 6/5 with a Vampire untapped, went
  to 1, and lost with the Intruder Alarm still in hand.
```
AFTER:
```
- The blockers prompt says "- NOT lethal: block only where the trade favors you." and stops
  there: a TOKEN still goes in front of the biggest attacker. A 2/2 you can remake is not a cost,
  and the "you would be at" number is the life your combo has to live on next turn.
```

## A55-TRIM9
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
IF YOUR PLAN NAMES WHICH CREATURE AN EDICT WILL REMOVE, THE PLAN IS ALREADY WRONG. Check your
own sentence before you check the board: "Tribute to Hunger on Sigarda", "Tribute to Hunger on
Briarbridge Tracker", "Tribute to Hunger to remove Sorin" are all sentences this deck has
written, and all three are impossible. You NEVER pick. The row itself prints the whole card -
```
AFTER:
```
IF YOUR PLAN NAMES WHICH CREATURE AN EDICT WILL REMOVE, THE PLAN IS ALREADY WRONG. "Tribute to
Hunger on Sigarda", "... on Briarbridge Tracker", "... to remove Sorin" are all sentences this
deck has written and all three are impossible. You NEVER pick. The row prints the whole card -
```

## A55-TRIM10
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
AN EDICT CANNOT TOUCH A PLANESWALKER. Both rows say "sacrifices a creature". A planeswalker on
their battlefield line - it prints "[planeswalker] [counters: Nx loyalty]" - is not a creature
and is not a legal outcome of either card. "Use Tribute to Hunger to remove Sorin" is a sentence
this deck wrote on a board with SEVEN of their creatures listed; the opponent handed over a 1/1
token and Sorin kept ticking up. If a planeswalker is what is beating you, an edict is not the
answer and neither is any other card in this deck: build the board and attack it.
```
AFTER:
```
AN EDICT CANNOT TOUCH A PLANESWALKER. Both rows say "sacrifices a creature"; a line printing
"[planeswalker] [counters: Nx loyalty]" is not a creature and is not a legal outcome of either
card. If a planeswalker is what is beating you, no card in this deck answers it: build the board
and attack it.
```

## A55-12
**Finding.** Replaced two wave-52/53 anecdotes with the current denominators, which are the strongest possible statement of the rung: 171 'at 0 this does nothing' rows and 52 'they choose which one' rows offered, 0 taken.

BEFORE:
```
(2) If N is not exactly 1, do not cast an edict - not at 0, not at 2 or more. The N = 0 row
prints its own verdict - "{right now: they control 0 creatures - at 0 this does nothing}" - and
this deck answered that row's number on turn 44 of a game it was losing, off a menu whose other
two rows were a dead Damnation and "Cast nothing right now". A row that says it does nothing does
nothing. "- they choose
which one" is the row telling you N is 2 or more and that the body you want is the one they will
keep: this deck cast Tribute to Hunger twice and Devour Flesh once off that exact clause in one
game, against a board whose Rorix Bladewing was still there afterwards every time, and lost it. (3) A creature
```
AFTER:
```
(2) If N is not exactly 1, do not cast an edict - not at 0, not at 2 or more. Both are held
rungs: last corpus this deck was offered 171 rows reading "they control 0 creatures - at 0 this
does nothing" and 52 reading "- they choose which one", and took NONE of them. Keep that at zero.
(3) A creature
```

## A55-13
**Finding.** 123v146 s22: sent a lone Thraben Doomsayer into '[their untapped blockers: Triumphant Adventurer (1/1) (both die); Goblin (1/1) (you kill it, your attacker lives); Silverquill Silencer (3/2) (both die)]' with 25 summoning-sick Humans at home, under a PLAN reading only 'stop reached'. Survived on the opponent's block choice.

BEFORE:
```
One hold-back only: do not send a LONE Thraben Doomsayer or Bloodline Keeper - or your ONLY
creature of any kind - into a board that can block and kill it. Its own row says when:
"[their untapped blockers: ... (both die)]" or "(your attacker dies)" on that line means
"ATTACK: none". This deck traded a lone Doomsayer for a Siege-Gang Commander with twenty Humans
summoning sick behind it, and at 4 life sent its only Human into "(your attacker dies, their
blocker lives)" under a plan that said its death would "untap all creatures" - nothing untaps
for a creature LEAVING. Send the maker once tokens are attacking beside it.
```
AFTER:
```
One hold-back only: do not send a LONE Thraben Doomsayer or Bloodline Keeper - or your ONLY
creature of any kind - into a board that can block and kill it. Its own row says when:
"[their untapped blockers: ... (both die)]" or "(your attacker dies)" on that line means
"ATTACK: none", and a hundred summoning-sick tokens behind it change nothing - last corpus this
deck sent a lone Doomsayer into a tag naming TWO "(both die)" blockers with twenty-five sick
Humans at home, and it survived only because they chose the third blocker. Send the maker once
tokens are attacking beside it.
```

## A55-TRIM11
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
STEP 1 - WHAT DOES THE NEXT LOOK COST? Read the ask's own price line for the "(keeping N)"
number. Each look is a fresh seven you must shave by one more card at the SAME land odds, so a
mulligan buys a look, never a fix. At (keeping 5) or lower, ONE land in front of you is a keep:
the bottom ask sheds the uncastable cards and leaves you the land plus your cheapest spells.
Colour is not a reason to ship at any N. Zero lands is the one hand a fresh look is worth the
card for; a hand of nothing but lands is the other. WHAT THIS COST: three mulligans in one
game, a four-card keep, both ways to find a token-maker bottomed, no creature ever on the
battlefield, lost 42 to 0.
STEP 2 - on your untouched opening seven, or at "(keeping 6)", a mulligan is a fair trade only
by the comparison below.
```
AFTER:
```
STEP 1 - WHAT DOES THE NEXT LOOK COST? Read the ask's "(keeping N)". Each look is a fresh seven
shaved by one more card at the SAME land odds, so a mulligan buys a look, never a fix. At
(keeping 5) or lower, ONE land in front of you is a keep. Colour is not a reason to ship at any
N. Zero lands is the one hand a fresh look is worth the card for; all lands is the other. WHAT
THIS COST once: three mulligans in one game, a four-card keep, lost 42 to 0.
STEP 2 - on the untouched seven, or at "(keeping 6)", use the comparison below.
```

## A55-TRIM12
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
ENTRY 0 HAS NO EXCEPTIONS FOR YOUR OWN CARDS: at 13 life this deck cast Idyllic Tutor over
"destroys 2 ... 0 of yours", at 9 life cast Intruder Alarm over "destroys 4 ... 0 of yours", and
cast the Damnation at 5. Eight life for two cards that could wait.

```
AFTER:
```
ENTRY 0 HAS NO EXCEPTIONS FOR YOUR OWN CARDS: at 13 life this deck cast Idyllic Tutor over
"destroys 2 ... 0 of yours" and at 9 cast Intruder Alarm over "destroys 4 ... 0 of yours".

```

## A55-TRIM13
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
"Cast Idyllic Tutor {2}{w}" searches for an ENCHANTMENT, and the only enchantment in this sixty
is Intruder Alarm; every other row on the reveal list reads "[does NOT qualify]". IT CANNOT
FIND A CREATURE. The row's own bracket tells you which of three cases you are in, and only one
of them is a cast:
```
AFTER:
```
"Cast Idyllic Tutor {2}{w}" searches for an ENCHANTMENT; the only enchantment in this sixty is
Intruder Alarm and every other reveal row reads "[does NOT qualify]". IT CANNOT FIND A CREATURE.
Its bracket names which of three cases you are in, and only one is a cast:
```

## A55-TRIM14
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
THE ALARM YOU TUTOR MAY NOT BE CASTABLE THIS TURN. The engine taps your mana for you, and it
paid a Tutor's {2}{w} with BOTH Underground Seas when a Swamp and two Scrublands would have done
it; the tutored Alarm then sat in hand for a whole turn cycle with "{U} 0" on the mana line.
Read "sources that can make each: ... {U} N" before you cast the Tutor: at {U} 2 or less, plan
the Alarm for NEXT turn, and spend what the Tutor leaves on a maker - "Cast Thraben Doomsayer
{1}{w}{w}" was on the menu with three W/B sources open, twice, and was not taken.
```
AFTER:
```
THE ALARM YOU TUTOR MAY NOT BE CASTABLE THIS TURN. The engine taps your mana for you and has
paid a Tutor's {2}{w} with BOTH Underground Seas; the tutored Alarm then sat in hand for a whole
turn cycle with "{U} 0" on the mana line. Read "sources that can make each: ... {U} N" first: at
{U} 2 or less, plan the Alarm for NEXT turn and spend what the Tutor leaves on a maker.
```

## A55-14
**Finding.** '[second copy:' Alarm rows: 2 offered, 0 taken; Talisman-shaped '[second copy:' rows 24 offered, 4 taken (correct). The paragraph now records the rung as HELD so the next reviewer does not read it as a live bleed, and is shorter.

BEFORE:
```
THE SECOND-COPY BRACKET IS NOT PERMISSION. A second Alarm's row reads "Cast Intruder Alarm {2}{u}
[second copy: you already control Intruder Alarm; both stay on the battlefield - no legend rule]".
The bracket now ends in the verdict itself: "... no legend rule, but its effect is already on the
battlefield and a second copy changes nothing". That clause answers "is this LEGAL" - it is - and
then tells you it does nothing. It does not: the Alarm's two lines ("creatures don't untap",
"whenever a creature enters, untap all creatures") are already true from the first copy, so the
second changes no number on any screen. This deck has now cast a second Alarm under that bracket
SEVEN times across four games - three of them last corpus in one game, at turns 36, 42 and 58,
the last of those at 7 life - and each time its own plan said the row was dead first: "second
copy is dead", "dead, but only way to proceed", "legal but redundant". Writing DEAD next to the
row and then answering its number is the whole failure. Read the bracket as the word DEAD and
then answer a DIFFERENT number.
```
AFTER:
```
THE SECOND-COPY BRACKET IS NOT PERMISSION. A second Alarm's row reads "[second copy: you already
control Intruder Alarm; both stay on the battlefield - no legend rule, but its effect is already
on the battlefield and a second copy changes nothing]". That answers "is this LEGAL" - it is -
and then says it does nothing, and it does not: both of the Alarm's lines are already true from
the first copy. This deck cast a second Alarm under that bracket seven times across four games,
each time writing DEAD beside the row first. Read the bracket as the word DEAD and answer a
DIFFERENT number. It held last corpus - 24 "[second copy:" rows, 0 Alarms - keep it there.
```

## A55-TRIM15
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
THE STOP IS A HEADCOUNT, BECAUSE POWER CANNOT MEASURE THIS LOOP. With Intruder Alarm out the
line comes back after every activation and the new tokens are summoning sick, so the "power of
creatures that are not summoning sick" number does NOT MOVE when you take it. A stop written on
that number can never be reached: this deck took the line 1,868 times in ONE turn (1,540 and 214
in two others) writing "on my next main phase, attack with all creatures to win the game" every
time, and never reached that main phase.
```
AFTER:
```
THE STOP IS A HEADCOUNT, BECAUSE POWER CANNOT MEASURE THIS LOOP. With Intruder Alarm out the
line comes back after every activation and the new tokens are summoning sick, so the "power of
creatures that are not summoning sick" number does NOT MOVE when you take it, and a stop written
on it can never be reached - this deck once took the line 1,868 times in ONE turn.
```

## A55-TRIM16
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
CHECK 3 - THE COLLAPSED TAG AND THE "GANG BLOCK:" CLAUSE. On a wide board their side stops
being listed blocker by blocker and the tag reads instead "they have N untapped creatures that
could block this one, biggest X (you kill it, your attacker lives)", sometimes with a clause
"- GANG BLOCK: their 6 biggest together deal 7, enough to kill this attacker; each result above
is a LONE blocker only".
That parenthesis prices ONE blocker. The GANG BLOCK clause prices what actually happens: any
number of them may block the same attacker together, and your attacker's toughness is measured
against the SUM of their bodies.
```
AFTER:
```
CHECK 3 - THE COLLAPSED TAG AND THE "GANG BLOCK:" CLAUSE. On a wide board the tag reads "they
have N untapped creatures that could block this one, biggest X (you kill it, your attacker
lives)", sometimes with "- GANG BLOCK: their 6 biggest together deal 7, enough to kill this
attacker; each result above is a LONE blocker only". The parenthesis prices ONE blocker; the
GANG BLOCK clause prices what actually happens - any number of them may block the same attacker
and your attacker's toughness is measured against the SUM.
```

## A55-15
**Finding.** The exile-cast paragraph gained the cheaper-copy rule (see A55-1b): 123v152 s104 took the {4}{b}{b} exile row over an identical-verdict {2}{b}{b} hand row on the same menu.

BEFORE:
```
A CARD ELITE SPELLBINDER EXILED FROM YOUR HAND IS STILL YOURS TO CAST, AND THE ROW SAYS SO IN
WORDS. It comes back as "Cast Damnation {4}{b}{b} [from exile] {castable from exile - a legal
cast, not a dead row; it costs {2} more than printed, already counted in the cost shown}" - the
same card at the same cast-list entry, with the surcharge already inside the number you compare
to "Mana available". Nothing about that row is a reason to skip it: this deck read one for four
windows writing "Intruder Alarm is exiled, I cannot cast it", and at 4 life cast a Thraben
Doomsayer over an exiled Damnation reading "destroys 3 ... 0 of yours". An exiled card at entry
0 is still CL0.
```
AFTER:
```
A CARD ELITE SPELLBINDER EXILED FROM YOUR HAND IS STILL YOURS TO CAST, AND THE ROW SAYS SO IN
WORDS: "Cast Damnation {4}{b}{b} [from exile] {castable from exile - a legal cast, not a dead
row; it costs {2} more than printed, already counted in the cost shown}" - the same card at the
same cast-list entry, surcharge already inside the number. An exiled card at entry 0 is still
CL0. But it is the EXPENSIVE copy: when the same card is also on the menu from your hand, the
hand row is the one you take.
```

## A55-TRIM17
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
READ THE COSTS IN MANA, NOT IN SYMBOLS. {2}{u} is THREE mana - a generic 2 plus one blue.
{2}{b}{b} is FOUR. {1}{w}{w} is THREE. Compare that number to the "Mana available: N total"
line, which already counts every untapped source for you. Two untapped lands do NOT cast
Intruder Alarm; a two-land board held for seven turns with three lands in hand, under a plan
that re-stated "cast Intruder Alarm using Tundra and Underground Sea" every window, lost that
game 42 to 0 with no token ever made. If your plan names a spell you cannot afford this window,
the fix is the land drop, not another pass.
```
AFTER:
```
READ THE COSTS IN MANA, NOT IN SYMBOLS. {2}{u} is THREE mana, {2}{b}{b} is FOUR, {1}{w}{w} is
THREE. Compare that number to "Mana available: N total", which already counts every untapped
source. If your plan names a spell you cannot afford this window, the fix is the land drop, not
another pass.
```

## A55-TRIM18
**Finding.** Length trim only - no rule changed; text was a restatement or a wave-52/53 anecdote already made elsewhere. Taken to pay for the edits above.

BEFORE:
```
WHICH LAND: when a spell in your hand is castable THIS turn off the land you play, play the land
that enters UNTAPPED - Underground Sea, Tundra, Scrubland, Plains, Swamp. Arcane Sanctum enters
tapped and Isolated Chapel enters tapped unless you control a Plains or Swamp: play those on a
turn you were not going to spend all your mana. This deck once played a Sanctum over an
Underground Sea with Intruder Alarm {2}{u} castable off three, and passed twice with two mana.
This applies hardest in COMBAT: at 9 life this deck built a survival line on Tribute to Hunger
with "Mana available: 1 total" on the same screen and Tribute costing three. Check "Mana
available: N total" BEFORE you build a line around a card in hand.
```
AFTER:
```
WHICH LAND: when a spell in your hand is castable THIS turn off the land you play, play the one
that enters UNTAPPED - Underground Sea, Tundra, Scrubland, Plains, Swamp. Arcane Sanctum enters
tapped, and Isolated Chapel does unless you control a Plains or Swamp: play those on a turn you
were not going to spend all your mana. Check "Mana available: N total" BEFORE you build any line
around a card in hand.
```
