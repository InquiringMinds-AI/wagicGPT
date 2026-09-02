# wave-53 deck125 edit texts (Revelation Fracture, UW draw-go)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` (50,417 bytes, 601 lines).
Result: `wave53/deck125/strategy.txt` (55,975 bytes, 661 lines; +5.6 KB / +11%). 27 edits, of which
6 are trims. Every edit cites a seq in `matchups-20260902-023342`.

## W77a
**Finding.** deck125 vs126 seq 294 (t49, 36/8) and seq 332 (t52, 20/19): Emrakul cast with `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` printed (353 renders in that game). The old recognition asked the pilot to scan the opponent battlefield line for an {effect:} substring; the prompt prints a dedicated one-line block. Re-keyed to that literal (#168).

**Before**
```
THE ONE BOARD WHERE THAT LINE IS OFF, and it outranks it. Read the OPPONENT battlefield line
before you take the Emrakul row, and read the game log. If that line prints an [enchantment] whose
{effect:} reads "Whenever an opponent loses life, you gain that much life" (Exquisite Blood) or
"Whenever you gain life, ... loses that much life" (Sanguine Bond), Emrakul is not a
finisher, it is a fifteen-life gift:
```

**After**
```
THE ONE BOARD WHERE THAT LINE IS OFF, and it outranks it. THE PROMPT PRINTS THIS BOARD AS ITS OWN
LINE, above the menu, and that line is the whole test - you do not read the battlefield for it:
   "LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond."
When that line says "theirs", Emrakul is not a
finisher, it is a fifteen-life gift:
```

## W77b
**Finding.** Same two seqs. Four earlier windows (seq 254/255/262 t45-47, plus seq 309) declined the row correctly on the SAME board, so this is recognition drift across repeats, not a missing rule. deck126 vs125 seq 106 is the cost: Tribute ate the Emrakul, deck126 gained 15, its Sanguine Bond drained 15, and 36/8 became 21/21.

**Before**
```
You cast it at 48 life with a Staff ticking them down; they answered Tribute to Hunger and
you were at -12. You cast it again at 33 life with "Exquisite Blood {4}{b} [enchantment]" printed
on their line and your own plan line saying not to. On that board answer "Cast nothing right now"
to the Emrakul row every time and let the Staff finish - it was finishing.
```

**After**
```
You cast it at 48 life with a Staff ticking them down; they answered Tribute to Hunger and
you were at -12. You cast it again at 33 life with "Exquisite Blood {4}{b} [enchantment]" printed
on their line and your own plan line saying not to. On that board answer "Cast nothing right now"
to the Emrakul row every time and let the Staff finish - it was finishing.
THE CONVERTER LINE WAS PRINTED IN ALL 353 OF YOUR PROMPTS IN ONE GAME LAST CORPUS AND YOU STILL TOOK
THE ROW TWICE. You were at 36 to their 8 with two Staffs ticking. You cast Emrakul; their Tribute to
Hunger ate it, they gained 15, their Sanguine Bond took 15 off you, and the game was 21-21. Four
windows earlier you had declined the same row on the same board, correctly. The rule does not get
weaker because the row keeps being offered: read the CONVERTER line at EVERY Emrakul row, including
the fifth one, and answer "Cast nothing right now" while it says "theirs".
```

## W77c
**Finding.** DECIDING-SITUATIONS bullet re-keyed to the same literal, and its broken seam repaired (the old bullet's edict-without-converter clause had lost its opening).

**Before**
```
- "Cast Emrakul, the Aeons Torn" is printed in your numbered list: TAKE IT - unless the opponent
  battlefield line prints an [enchantment] whose {effect:} says "you gain that much life" or
  "loses that much life" (Exquisite Blood, Sanguine Bond): then cast nothing and let the Staff
  finish.
```

**After**
```
- "Cast Emrakul, the Aeons Torn" is printed in your numbered list: TAKE IT - unless the prompt
  carries the line "LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - <name>": then cast
  nothing and let the Staff finish, at every window that offers the row.
```

## W78a
**Finding.** deck125 vs162 seq 78 (t18, 24 life): cast Essence Scatter off a row reading `[DRAW PRICE: ... deal you 14 - you would be at 10]`. Life fell 24 -> 10 in that window; the seat never cast another spell and died at t21. The old gate (`price below your life minus 1`) licensed paying 58% of the seat's life for a third punisher. Re-keyed to the row's own `- you would be at K` with a fixed floor of 15.

**Before**
```
   EVERY ROW ON THAT MENU PRINTS THE PRICE, THE COUNTER ROW INCLUDED: "[DRAW PRICE: casting this
   draws YOU 7 cards (their Forced Fruition), and their Underworld Dreams deals you 7]". Read that
   last number against YOUR LIFE before anything else on the row. When it is at or above your life,
   THAT ROW KILLS YOU - answer "Cast nothing right now" even though the thing on the stack is on
   your counter-on-sight list, because a threat that resolves can still be answered next turn and a
   row that kills you cannot.
```

**After**
```
   EVERY ROW ON THAT MENU PRINTS THE PRICE AND THE FINISHED SUBTRACTION, THE COUNTER ROW INCLUDED:
   "[DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition), and their Underworld
   Dreams, Ob Nixilis, the Hate-Twisted deal you 14 - you would be at 10]". THE NUMBER YOU READ IS
   THE ONE AFTER "you would be at" - call it K. Do no arithmetic of your own; the row did it.
   - K is 15 or more: the row is payable. Take it if the rule above wants the counter.
   - K is below 15: answer "Cast nothing right now", even though the thing on the stack is on your
     counter-on-sight list, and even when the row does NOT say "this KILLS you". A threat that
     resolves can still be answered next turn; fourteen life cannot be bought back.
   - The row ends "; this KILLS you": that is the floor, not the rule. Eight rows carried it last
     corpus and you declined all eight - correctly, and it changed nothing, because the row that
     lost you the game was the one that did NOT carry it.
   AT 24 LIFE, WITH TWO PUNISHERS ALREADY RESOLVED, YOU CAST ESSENCE SCATTER OFF A ROW READING
   "deal you 14 - you would be at 10" TO COUNTER A THIRD PUNISHER. The counter worked. A third
   punisher costs you 1 more life per card you draw; you paid fourteen for it in one window, went
   24 -> 10 on the spot, and were dead two turns later without another spell being cast. That is
   the whole loss.
```

## W78b
**Finding.** Same finding, DECIDING-SITUATIONS bullet.

**Before**
```
- "Forced Fruition" is on the opponent battlefield line beside Underworld Dreams, Fate Unraveler or
  Ob Nixilis: cast nothing on your turn, and cast a counter only when 7 x (Fruitions) x (punishers)
  is below your life minus 1. Activating an Elixir already on your battlefield is still free.
```

**After**
```
- "Forced Fruition" is on the opponent battlefield line beside Underworld Dreams, Fate Unraveler or
  Ob Nixilis: cast nothing on your turn, and cast a counter only when the row's own
  "- you would be at K" reads 15 or more. Do not compute the price; read K off the row.
  Activating an Elixir already on your battlefield is still free.
```

## W79a
**Finding.** deck125 vs126 seq 109/157/193 (t29/32/35, 37/47/45 life): three Paths spent on 1/1 lifelink Vampire tokens with `LIFE-TO-DAMAGE CONVERTER ... theirs - Sanguine Bond` printed. Correct play (each Vampire was 2 off the seat per combat) but forbidden by the guide's own textless-token rule. Carve-out added; also resolves the contradiction between the Path section's power-0/1 stop and the TARGET-ask power tiebreak (#177).

**Before**
```
Only when NO candidate has such a line do you use the power rule:
```

**After**
```
ONE MORE LINE OUTRANKS THE POWER RULE, AND IT IS THE PROMPT'S OWN: when the prompt carries
"LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - <name>", every point of life THEY gain comes
straight off YOUR total. A creature of theirs tagged [lifelink] is then a clock at ANY printed
power - a 1/1 lifelink Vampire token takes 2 off you every combat it connects - and it is the Path
target, ahead of a bigger ground creature and ahead of the "keep the Path" rule below. You Pathed
three 1/1 lifelink Vampires off exactly that line at 37, 47 and 45 life and won that game; without
that line those same tokens are the textless tokens the next paragraph tells you to leave alone.
Only when NO candidate has such a line do you use the power rule:
```

## W79b
**Finding.** Same, plus deck125 vs146 seq 97 (t26, 35 life): a Path spent on a lone 1/1 Goblin token with NO converter anywhere - the case the rule is for, kept.

**Before**
```
at any life total above 12, whatever else is on the board and however many turns you have been
holding it. A 0/4 wall is never a Path target, and neither is a token with no {target text:} at
all - "Goblin (creature 1/1)", a Spirit, a Human, a Wolf: you Pathed a lone Goblin token twice at
20 life with ten lands open.
```

**After**
```
at any life total above 12, whatever else is on the board and however many turns you have been
holding it - unless the CONVERTER line above is printed and the candidate has [lifelink]. A 0/4
wall is never a Path target, and neither is a token with no {target text:} at all - "Goblin
(creature 1/1)", a Spirit, a Human, a Wolf: you Pathed a lone Goblin token twice at 20 life with
ten lands open, and once more last corpus at 35 life on a board with no converter anywhere.
```

## W80
**Finding.** Eight tap-out-gate breaks last corpus: Elixir activations vs126 seq 10, vs123 seq 35/53, vs162 seq 23/70; Staffs vs126 seq 69, vs123 seq 27; Lightmine vs130 seq 25. Every one at `leaves 0`/`leaves 1` with an Essence Scatter or Fall of the Gavel on the hand line. Re-keyed to lane L's `{spends K of your M untapped mana sources this turn; <card> in your hand needs N}` clause, WITH the warning that the card the clause names is not necessarily a counter (vs162 seq 23 named a Path).

**Before**
```
THE GATE IS A PRICE, AND THE PRICE IS PRINTED IN YOUR HAND.
```

**After**
```
THE ROW DOES THE SUBTRACTION FOR YOU AND NAMES THE CARD IT IS SUBTRACTING FOR. Most spend rows -
a Staff, an Elixir ACTIVATION, a Lightmine Field, a Revelation - now end with
"{spends K of your M untapped mana sources this turn; <card> {cost} in your hand needs N}".
That clause names ONE card in your hand and what it needs. Work it in two steps and take neither
on trust: (1) M minus K is what is left; (2) the price is the cheapest COUNTERSPELL on your hand
line (Essence Scatter 2, Cancel or Dream Fracture 3, a lone Fall of the Gavel 5) - NOT necessarily
the card the clause happens to name, which is only the cheapest card you cannot currently pay for.
If M minus K is below that price, PASS. A row carrying no such clause at all is a row where M is far
above K; those are free. Eight rows broke this gate last corpus - five Elixir activations, two
Staffs and a Lightmine Field, every one at "leaves 0" or "leaves 1" with an Essence Scatter or a
Fall of the Gavel sitting on the hand line.
THE GATE IS A PRICE, AND THE PRICE IS PRINTED IN YOUR HAND.
```

## W81
**Finding.** 116 `max affordable X=1`/`X=2` rows, 2 taken: vs126 seq 59 (t19, 25 life) and vs123 seq 17 (t9, 20 life). On both menus every other row was one the guide forbids. Re-keyed to the literal and the empty-menu excuse named and refused.

**Before**
```
Sixteen windows at "max affordable X=1" or "X=2" one corpus, sixteen holds; the corpus after that,
two casts at 20 life from menus whose only other rows were "Cast nothing" and a sweeper at 0; this
corpus eighteen such rows and eighteen holds, and all ten Revelations you did cast came off rows
reading X=3 or more. The rule works - keep reading the row.
```

**After**
```
THE ROW IS THE TEST AND THE ROW IS ONE STRING: "{X pricing: max affordable X=1" and
"{X pricing: max affordable X=2" are both "Cast nothing right now", at any life above 10, on any
menu. One hundred and sixteen such rows last corpus and you held one hundred and fourteen.
THE TWO YOU TOOK ARE THE SAME EXCUSE TWICE, AND IT IS NOT AN ARGUMENT: on both menus every other
row was one this guide forbids - a Supreme Verdict at "destroys 0 of their creatures", a Path at a
candidate tagged [defender]. A menu on which nothing is worth casting is answered "Cast nothing
right now". That is a real answer, not a wasted turn, and it is the answer 155 sweeper windows
already got in the same six games.
```

## W82a
**Finding.** deck125 vs162 seq 87 (t19, hand 14, sent 7): the seat discarded its only Staff of Nin. The ordered cleanup list had no Staff entry. Also vs126 seq 61/268: Essence Scatters sent ahead of a Fall of the Gavel against deck126, whose every creature is a wall - the old carve-out said `when the opponent casts no creatures`, which does not fire on a wall deck. Both fixed.

**Before**
```
  6. Only then a counterspell, cheapest last: send a Fall of the Gavel before a Cancel, a Cancel
     before an Essence Scatter - and an Essence Scatter first when the opponent casts no creatures.
```

**After**
```
  6. A Staff of Nin, but ONLY when a Staff of yours is already on your battlefield line. The Staff
     is the only card in this deck that ends a game; a Staff in hand with none in play is kept
     ahead of every counterspell. You sent your only Staff from a fourteen-card hand at 6 life.
  7. Only then a counterspell, MOST EXPENSIVE FIRST: send a Fall of the Gavel before a Cancel, a
     Cancel before an Essence Scatter. THE ONE REVERSAL: send the Essence Scatters first when every
     creature the opponent has cast so far is a wall - printed power 0, or tagged [defender]. A
     Scatter counters creature spells only, and against a board of walls you will never spend one,
     so it is the dead card in that hand whatever it cost.
```

## W82b
**Finding.** Same, DECIDING-SITUATIONS bullet.

**Before**
```
- The cleanup ask ("PUT: n, m"): send Lightmine Field, spare lands, a second sweeper, Emrakul
  (below 13 lands), a spare Revelation - a counterspell last.
```

**After**
```
- The cleanup ask ("PUT: n, m"): send Lightmine Field, spare lands, a second sweeper, Emrakul
  (below 13 lands), a spare Revelation, a Staff of Nin only with a Staff already in play - a
  counterspell last, most expensive first, except that Essence Scatters go first against a deck
  whose every creature so far is a wall.
```

## W83
**Finding.** deck125 vs126 seq 67 (Fall of the Gavel on a Chromatic Lantern) and vs123 seq 22 (Dream Fracture on Lightning Greaves). 39 counters cast, 2 on artifacts the guide already tells the seat to let resolve; 0 on walls (fourth corpus at that ceiling).

**Before**
```
    nothing; all it does is let them cast the spell you actually need the counter for. In a game
    you lost you spent Dream Fracture on a Chromatic Lantern on turn 7; the Sanguine Bond it was
    fixing the mana for resolved on turn 12 with no counter left in your hand.
```

**After**
```
    nothing; all it does is let them cast the spell you actually need the counter for. In a game
    you lost you spent Dream Fracture on a Chromatic Lantern on turn 7; the Sanguine Bond it was
    fixing the mana for resolved on turn 12 with no counter left in your hand. You did it again
    last corpus - a Fall of the Gavel on a Chromatic Lantern and a Dream Fracture on a Lightning
    Greaves (an Equipment: no P/T, no damage, no card) - in two games whose real threats were the
    enchantment halves those counters were then not there for. An (artifact) stack line with no
    P/T is a decline unless its {target text:} damages, draws or makes tokens.
```

## W84
**Finding.** 479 opponent-turn windows on this seat in six games (35 End, 34 Cleanup, 37 Combat-begins, 34 Combat-ends, all with 0 casts). The guide asserted `You get a window on the opponent's turn only when they cast a spell` and `You are never offered an end-of-their-turn window` - both false against this render. Trust doctrine: the guide is corrected to the render. The HOLD row (476 offers, 3 takes) gets one sentence so the pilot knows what it is.

**Before**
```
=== YOUR MANA AND YOUR WINDOWS ===
You get a window on the opponent's turn only when they cast a spell, and the prompt shows it to you
under "ON THE STACK". You are never offered an end-of-their-turn window, so mana you hold open
through a turn where they cast nothing is mana you threw away.
```

**After**
```
=== YOUR MANA AND YOUR WINDOWS ===
YOU NOW GET A WINDOW AT EVERY PHASE OF THE OPPONENT'S TURN - their Upkeep, Draw, both main phases,
combat, End and Cleanup alike. Four hundred and seventy-nine of them in six games last corpus, and
you cast 47 spells across all of them. Most have NOTHING on the stack, and a window with nothing
under "ON THE STACK" has one right answer: decline it. Your counterspells answer a spell that is ON
THE STACK; there is nothing to spend them on at an empty End step, and nothing to hold them for
either - a spell they cast later opens its own window.
THE LAST ROW OF AN OPPONENT-TURN MENU reads "Hold priority - do not ask me again this turn unless
the board changes". It is a decline row like "Cast nothing right now", and it is safe: a new object
on the stack is a board change, so taking it never gives up a window on a spell they cast
afterwards. Take it or take "Cast nothing right now" - both are correct. What is never correct is
casting something off one of these windows because the window opened.
```

## W85a
**Finding.** Trim. The Elixir section's three stale activation anecdotes are superseded by W80's clause rule; replaced with the current count (5 of 8 breaks).

**Before**
```
Cast it the turn it is listed when you have a spare {1}. ACTIVATING it is the same spend as
casting a spell and the tap-out gate covers it: the {2} has to leave your "Mana available:" line at
or above the price of the cheapest counter in your hand (2 for a Scatter, 3 for Cancel or Fracture,
5 for a lone Gavel). Otherwise activate it only when your life is 15 or
less or your library is 16 or less - those two are worth a tapped-out turn and a spare {2} in a
full hand is not. On turn 2 of a game you activated it from "Mana available: 2" to 0 at 20 life
with a library of 53 and a Scatter and a Gavel in hand: nothing bought, a counter's mana gone.
You did it twice more this corpus - turn 5 from "Mana available: 2" to 0 at 20 life on a library of
51 with an Essence Scatter in hand, and turn 13 at 18 life on four sources with a Fall of the Gavel
as your only counter, leaving 2 against its price of 5. THE ROW DOES THAT SUBTRACTION FOR YOU: the
activation line prints "{spends 2 of your 4 untapped mana sources this turn; Fall of the Gavel
{3}{u}{w} in your hand needs 5}". When the number after "needs" is larger than what the row leaves,
the answer is pass.
```

**After**
```
Cast it the turn it is listed when you have a spare {1}. ACTIVATING it is the same spend as
casting a spell and the tap-out gate covers it - read the activation row's own
"{spends K of your M untapped mana sources this turn; ...}" clause and pass when M minus K is below
the cheapest counter on your hand line. Otherwise activate it only when your life is 15 or less or
your library is 16 or less; those two are worth a tapped-out turn and a spare {2} in a full hand is
not. THE ACTIVATION IS THE GATE'S WORST BREAK POINT: five of the eight gate breaks last corpus were
Elixir activations, at 20, 20, 25, 20 and 21 life, every one down to "leaves 0" or "leaves 1" with
an Essence Scatter or a Fall of the Gavel in hand.
```

## W85b
**Finding.** Trim. 0 `bottom` asks on this seat in three corpora; the worked example goes, the rule stays.

**Before**
```
The hand you keep should hold at least as many lands as it can play in its first two or three
turns and at least one card that answers a threat - a counterspell or a Path. When the ask makes
you send more cards than that leaves room for, keep the LANDS and ONE answer and let the rest go:
a counterspell you cannot pay for is not an answer, and a land with nothing to cast is not a hand.
You once sent an Island, a Fall of the Gavel and an Essence Scatter from a three-land four-spell
hand and kept two lands with a Path and an Elixir - three cards had to go, and the three that
should have gone were the Gavel, the Elixir and the Path, leaving three lands and a Scatter.
```

**After**
```
The hand you keep should hold at least as many lands as it can play in its first two or three
turns and at least one card that answers a threat - a counterspell or a Path. When the ask makes
you send more cards than that leaves room for, keep the LANDS and ONE answer and let the rest go:
a counterspell you cannot pay for is not an answer, and a land with nothing to cast is not a hand.
```

## W85c
**Finding.** Trim. One of three two-corpora-stale sweeper anecdotes removed.

**Before**
```
  You have cast a Supreme Verdict at a lone Fog Bank (0/2 defender) at 23 life, a Final Judgment
  at a lone Wall of Omens (0/4 defender) at 31 life, and a Supreme Verdict at a lone Spirit token
  (1/1) at 33 life with two Paths in hand; each cost you a card and a turn of open mana and
  killed nothing that mattered.
```

**After**
```
  You have cast a Supreme Verdict at a lone Fog Bank (0/2 defender) at 23 life and a Final Judgment
  at a lone Wall of Omens (0/4 defender) at 31 life; each cost a card and a turn of open mana and
  killed nothing that mattered.
```

## W85d
**Finding.** Trim + refresh. 15 X menus last corpus, 15 right answers (13 threes; two menus topping out at 1 and 2). The worked arithmetic examples were never needed.

**Before**
```
If "X = 3" is not on the menu, take the largest value the menu does offer that is below 3.
Worked examples, both from real games of yours. Library 31, one Staff, menu up to 13: 31-2-1 = 28,
which is not below 3, so X = 3 - not the 13 the menu topped out at. Library 20, two Staffs, menu up
to 13: 20-2-2 = 16, still not below 3, so X = 3 - the 8 you announced discarded five cards at end
of turn.
```

**After**
```
If "X = 3" is not on the menu, take the largest value the menu does offer that is below 3 - which
is what you correctly did on the two menus last corpus that topped out at 1 and at 2.
Fifteen X menus last corpus, fifteen right answers: thirteen 3s and those two. This rule is at its
ceiling; the arithmetic above only ever LOWERS it, and it has never yet had to.
```

## W85e
**Finding.** Trim + refresh. Land drop 85/85 last corpus; one anecdote is enough for a rule at its ceiling.

**Before**
```
Games of yours have been lost this way. In one you passed the land drop on six straight turns
holding a Plains and an Island, then could not pay for a second counterspell on the turn their
engine landed. In another you skipped three drops, hit seven cards, and DISCARDED a Cancel -
then the spell that killed you resolved four turns later with nothing in hand to stop it.
```

**After**
```
Games of yours have been lost this way: you passed the land drop on six straight turns holding a
Plains and an Island, then could not pay for a second counterspell on the turn their engine landed.
Eighty-five land windows last corpus, eighty-five lands played - this rule is at its ceiling and
there is nothing here to think about.
```

## W85f
**Finding.** Trim. Two-corpora-stale Peer into the Abyss anecdote; the rule above it is unchanged.

**Before**
```
    outranks the "(artifact) with no P/T: let it resolve" line above. Extra cards cost you life
    whenever they have Underworld Dreams or Fate Unraveler out, and cost you library either way.
    Peer into the Abyss under two Underworld Dreams drew you half your library at 2 life a card
    and killed you from 14.
```

**After**
```
    outranks the "(artifact) with no P/T: let it resolve" line above. Extra cards cost you life
    whenever they have Underworld Dreams or Fate Unraveler out, and cost you library either way.
```

## W86a
**Finding.** Repairs the seam W77c opened in the Emrakul bullet.

**Before**
```
  nothing and let the Staff finish, at every window that offers the row. The log shows they used Tribute to Hunger or Devour Flesh and NEITHER enchantment is on
  their line: take it only at their printed life 15 or less. Otherwise the game only prints a cast
  you can already pay for, and it ends the game faster than the Staff does.
```

**After**
```
  nothing and let the Staff finish, at every window that offers the row - the fifth one included.
  No CONVERTER line, but the log shows they used Tribute to Hunger or Devour Flesh: take it only at
  their printed life 15 or less. Neither of those: the game only prints a cast you can already pay
  for, and Emrakul ends the game faster than the Staff does.
```

## W86b
**Finding.** Refreshes the Staff bullet's counts to 2 of 7 and points it at the spends/needs clause (#168).

**Before**
```
- "Cast Staff of Nin" is in your list: read the counters on your hand line and their costs; take
  the Staff only from a row whose "leaves N" covers the cheapest one (Scatter 2, Cancel or
  Fracture 3, a lone Gavel 5), or with no counter in hand. Below that: cast nothing, and cast it
  next turn. "Leaves 1" with a Dream Fracture in hand is below the price - you took that row twice
  in the corpus where it cost two games, and Ob Nixilis and three creatures resolved on the next
  opponent turns with no window offered. You took it four more times this corpus ("leaves 1" and
  "leaves 0", an Essence Scatter in hand) and none of the four was punished; the rule stands on the
  corpus where it was.
```

**After**
```
- "Cast Staff of Nin" is in your list: read the counters on your hand line and their costs; take
  the Staff only from a row whose "leaves N" (or M minus K on its "{spends K of your M ...}"
  clause) covers the cheapest one (Scatter 2, Cancel or Fracture 3, a lone Gavel 5), or with no
  counter in hand. Below that: cast nothing, and cast it next turn. "Leaves 1" with a Dream
  Fracture in hand is below the price - you took that row twice in the corpus where it cost two
  games, and Ob Nixilis and three creatures resolved on the next opponent turns with no window
  offered. Two of seven Staff casts last corpus were still taken at "taps you out" with a counter
  in hand; neither was punished, and the rule stands on the corpus where it was.
```

## W86c
**Finding.** #177: the DECIDING-SITUATIONS Path bullet contradicted W79's carve-out. Fixed.

**Before**
```
- The only Path targets offered are defenders or 1-power creatures and your life is above 12: cast
  nothing and keep the Path.
```

**After**
```
- The only Path targets offered are defenders or 1-power creatures and your life is above 12: cast
  nothing and keep the Path - UNLESS the prompt carries "LIFE-TO-DAMAGE CONVERTER on the
  battlefield: theirs - <name>" and one of those candidates is tagged [lifelink]: then Path that
  one, at any life total. A 1/1 lifelink body under their converter takes 2 off you per combat.
```

## W86d
**Finding.** #177: the TARGET-ask bullet gets the lifelink/converter tier so the two Path passages agree.

**Before**
```
- The TARGET ask lists two or more candidates: the one whose text says "Whenever ... attacks",
  "{T}:", "At the beginning of" or "venture" first; then the one tagged [flying]; then the highest
  printed power. "When this creature enters" is none of those.
```

**After**
```
- The TARGET ask lists two or more candidates: with a "CONVERTER ... theirs" line printed, a
  [lifelink] candidate first, whatever its power; then the one whose text says "Whenever ...
  attacks", "{T}:", "At the beginning of" or "venture"; then the one tagged [flying]; then the
  highest printed power. "When this creature enters" is none of those.
```

## W86e
**Finding.** #177: the Elixir bullet re-keyed to the same clause as W80/W85a.

**Before**
```
- Your life is 15 or less, or your library is 16 or less, and Elixir of Immortality is on your
  battlefield untapped: activate it. At a life above 15 and a library above 16, activate it only if
  the {2} still leaves your "Mana available:" line at or above the price of a counter in your hand.
```

**After**
```
- Your life is 15 or less, or your library is 16 or less, and Elixir of Immortality is on your
  battlefield untapped: activate it. At a life above 15 AND a library above 16, activate it only
  when the activation row's own "{spends K of your M untapped mana sources this turn; ...}" leaves
  M minus K at or above the cheapest counter on your hand line. This is the gate's worst break
  point - five of eight breaks last corpus were this activation.
```

## W86f
**Finding.** #177: the Fruition bullet still carried the old `below your life minus 1` gate that W78a replaced.

**Before**
```
   Cast NOTHING on your own turn - no Elixir cast, no Staff, no sweeper, no Revelation - and on
   their turn cast a counter ONLY when that price is below your life minus 1. Read the price off
   the line before the CHOICE: one Fruition and one Dreams is 7; two Fruitions and one Dreams is
   14. At 15 life you cast an Elixir under one of each, drew seven and finished at 13 for the five
   life it gained; at 11 life you cast Cancel under two Fruitions and a Dreams, drew fourteen, and
   died on the spot.
```

**After**
```
   Cast NOTHING on your own turn - no Elixir cast, no Staff, no sweeper, no Revelation - and on
   their turn cast a counter ONLY when the row's own "- you would be at K" reads 15 or more. At 15
   life you cast an Elixir under one Fruition and one Dreams, drew seven and finished at 13 for the
   five life it gained; at 11 life you cast Cancel under two Fruitions and a Dreams, drew fourteen,
   and died on the spot.
```

## W86g
**Finding.** Trim/merge: the 3-life anecdote is kept but folded, since W78a now carries the decisive one.

**Before**
```
   the whole loss. The damage does not arrive as one hit you might survive: it arrives as
   seven separate 1-damage pings, one per card drawn. At 3 life, with "deals you 7" printed on the
   row, you cast Essence Scatter at a Fate Unraveler; the Unraveler was countered, the seven draws
   took you 3 -> 2 -> 1 -> 0, and you lost the game in that window.
```

**After**
```
   the whole loss, twice over: the corpus before, at 3 life with "deals you 7" on the row, you cast
   Essence Scatter at a Fate Unraveler and the seven 1-damage pings took you 3 -> 2 -> 1 -> 0 in
   that window. The damage never arrives as one hit you might survive - it arrives as one ping per
   card drawn, and no answer of yours interrupts it.
```

## W87
**Finding.** #131 completeness. deck125 vs126 seq 1: a ONE-land seven whose coverage line named only `Elixir of Immortality {1}` was kept (and the game won at t54). No branch of the mulligan section covered a one-land hand WITH coverage - the `would not cover any spell` branch needs no coverage and the `2 to 5 lands` branch needs two. Recorded as a completeness fix and an outcome, not as a punished break.

**Before**
```
  - Seven lands and no spell, or seven spells and no land, ships at any price - those hands do
    nothing however long you wait, and the odds of the next look beating "nothing" are as good as
    odds get. You shipped a one-land seven that covered nothing and kept the two-land six behind
    it (Scatter and Elixir covered) - and won.
```

**After**
```
  - ONE LAND, whatever the coverage line says. One land plays one spell and then stops; the branch
    above is about hands that need a colour, and this hand needs a second and a third land before
    anything else it holds is castable. On a seven and on a six, SHIP IT - at 26 lands in 60 the
    next look is better odds than three draws. At "would keep 5 cards" or smaller, keep it if the
    coverage line names a card. You kept a one-land seven whose coverage line named only an Elixir
    of Immortality {1} last corpus and won a 54-turn game with it; that keep was outside this line
    and the win does not move it - one land is one draw from a hand that does nothing.
  - Seven lands and no spell, or seven spells and no land, ships at any price - those hands do
    nothing however long you wait, and the odds of the next look beating "nothing" are as good as
    odds get. You shipped a one-land seven that covered nothing and kept the two-land six behind
    it (Scatter and Elixir covered) - and won.
```
