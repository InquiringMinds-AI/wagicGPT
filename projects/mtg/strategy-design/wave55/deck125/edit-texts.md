# wave-55 deck125 edit texts (Revelation Fracture)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` at the boundary pass
dd0cd2d74 (61,248 bytes). Revised guide 66,855 bytes - inside the 41-71 KB pool band.
Corpus `matchups-20260903-074354`; every seq below is a record in
`1788439xxx-ai_baka_deck125-...-vs-ai_baka_<opp>.jsonl`, named by the short tag (`125v162` = the
deck125 seat of the game against deck162). Twelve edits (edits 11-12 are consistency follow-ons required by edit 2).

## Edit 1 - W96

**Finding.** STRATEGY W-1 (deck125). `125v162` seq 180 (t28, 30 life): the seat spent its ONLY counter row (Cancel) on a Howling Mine; Peer into the Abyss was cast in the same phase (seq 181), no counter row remained, and the resolution drew 19 cards and halved the seat 30 -> 15 with 19 Underworld Dreams triggers behind it (30 -> 0 that turn). Also `125v123` seq 31/88/99: three counters on Vision Skeins ("Each player draws two cards") with counters still in hand - correct, and the reason the new rule is gated on ONE counter row, not on the card.

**Before:**
```
    outranks the "(artifact) with no P/T: let it resolve" line above. Extra cards cost you life
    whenever they have Underworld Dreams or Fate Unraveler out, and cost you library either way.
```

**After:**
```
    outranks the "(artifact) with no P/T: let it resolve" line above. Extra cards cost you life
    whenever they have Underworld Dreams or Fate Unraveler out, and cost you library either way.
COUNT THE COUNTER ROWS ON THE MENU BEFORE YOU SPEND ONE. Every counterspell you can pay for right
now has its own "Cast ..." row, so the menu tells you how many answers you still have. When exactly
ONE counter row is printed, that row is your LAST answer. Spend a last counter only on a
planeswalker, on a creature of printed power 2 or more, or on a permanent that hits you or grows
them EVERY turn. These two are never worth a last counter:
  - a draw permanent that adds ONE card per draw step (Howling Mine, Dictate of Kruphix) while your
    printed life is 15 or more - it costs you 1 life a turn per punisher and you have more turns
    than that;
  - a one-shot draw spell, whose {target text:} reads only "Each player draws two cards" or "Draw a
    card" with no "At the beginning of" and no "Whenever" (Vision Skeins) - it draws once, then it
    is gone.
At 30 life against 20 you spent your only Cancel on a Howling Mine. In the SAME phase they cast
Peer into the Abyss - "Target player draws cards equal to half the number of cards in their library
and loses half their life. Round up each time." - your menu no longer printed a counter row, you
drew nineteen cards under an Underworld Dreams, and the game went 30 to 0 on that one turn. Three
cards in this pool end a game in one resolution and each is worth your last counter at any price:
Peer into the Abyss, Forced Fruition, Teferi's Puzzle Box. With TWO counter rows printed the
ordinary list decides and a Howling Mine or a Vision Skeins is a fine target - you countered three
Vision Skeins in one game with counters still in hand and it cost you nothing.
```

## Edit 2 - W101a

**Finding.** STRATEGY W-2 (deck125). Tap-out gate LIVE breaks 2 of 4 raw: `125v123` seq 49 (t19, 24 life, Staff of Nin at "casting this taps you out" with Dream Fracture in hand) and `125v126` seq 49 (t13, 23 life, same row with Essence Scatter in hand). BOTH were the FIRST Staff (no Staff on the battlefield line), both resolved, both pinged on the next upkeep, both games won 32-0 and 44-0. The gate is the guide's central rule and its only breaks in two corpora are first-Staff casts that were right; shape (d) makes the rule match the evidence and keeps the second-Staff / Elixir-activation / Lightmine cases gated.

**Before:**
```
    (c) the cheapest counter in your hand costs MORE than the row's own "M" total - if you could
        not have cast it this turn at any N, the gate has nothing to protect.
    Four of the five rows scored against this gate last corpus were one of those three, and the
    fifth was the real one: a Staff of Nin at "casting this taps you out" at 20 life with a Cancel
    in hand, against the combo deck whose two enchantments are exactly what a Cancel answers.
```

**After:**
```
    (c) the cheapest counter in your hand costs MORE than the row's own "M" total - if you could
        not have cast it this turn at any N, the gate has nothing to protect;
    (d) the row is "Cast Staff of Nin" and NO Staff of Nin is on your battlefield line. THE FIRST
        STAFF IS EMRAKUL-CLASS. It is the only card in this deck that ends a game, and a turn spent
        holding mana for a counter is a turn its clock has not started. You took that row at
        "casting this taps you out" twice this corpus - at 24 life with a Dream Fracture in hand,
        and at 23 life with an Essence Scatter in hand against the combo deck - and both Staffs
        resolved, both pinged on the very next upkeep, and both games were won 32-0 and 44-0. A
        SECOND Staff, with one already printed on your battlefield line, is NOT this shape: the
        clock is already running and the gate binds that row exactly as written.
    Four of the five rows scored against this gate one corpus were shapes (a) to (c), and the fifth
    was a FIRST Staff of Nin at "casting this taps you out" at 20 life with a Cancel in hand -
    which shape (d) now makes a take as well. What this gate still has to stop is a SECOND Staff,
    an Elixir ACTIVATION and a Lightmine Field at "leaves 0" or "leaves 1".
```

## Edit 3 - W101b

**Finding.** Same finding as W101a - the bullet in THE CLOCK section had to move with the gate or the two would disagree.

**Before:**
```
- A STAFF ROW WHOSE N IS BELOW THE PRICE OF A COUNTER IN YOUR HAND IS ANSWERED "Cast nothing right
  now".
```

**After:**
```
- A SECOND-STAFF ROW WHOSE N IS BELOW THE PRICE OF A COUNTER IN YOUR HAND IS ANSWERED "Cast
  nothing right now"; with no Staff on your battlefield line, shape (d) opens the gate and the row
  is a take.
```

## Edit 4 - W-A1

**Finding.** STRATEGY W-3 (deck125), the corpus's clearest lost game. `125v146` seq 32 (t13, Main 1, 10 life) and seq 36 (t13, Blockers, 10 life): the opponent line read "3 are creatures, 3 of them are attacking right now" with Barrowin of Clan Undurr (5/5), Nadaar, Selfless Paladin (4/4) and a Goblin (2/2) = 11 power; the only cast row was "Cast Sphinx's Revelation ... max affordable X=2". Declined both times, took 11, died at -1. X=2 was 12 life. The old carve-out was keyed to "life 5 or less" and to a bare header COUNT, neither of which fires at 10 life against 11 power.

**Before:**
```
THE ONE TIME THE ROW'S SIZE RULE IS OFF: your life is 5 or less and the opponent battlefield
header's second number is 1 or more. Then Sphinx's Revelation is a LIFE spell before it is a draw
spell: take its row at whatever "max affordable X" it prints, and on the X menu announce the
LARGEST X offered - the cards you discard at end of turn cost nothing next to the damage you
survive. At 1 life with a 3/1 lifelink attacker on their line and "max affordable X=7" on the
menu, you cast a second Staff of Nin instead and died to the 3/1 next turn; X=7 was 8 life.
```

**After:**
```
THE ONE TIME THE ROW'S SIZE RULE IS OFF, AND YOU READ IT OFF THE OPPONENT BATTLEFIELD LINE: add up
the POWER of the creatures that line says are attacking or able to attack. The line prints the
count and then every creature with its (P/T) - "3 are creatures, 3 of them are attacking right now"
followed by "Barrowin of Clan Undurr (5/5) [tapped - attacking]; Goblin (2/2) [tapped - attacking];
Nadaar, Selfless Paladin (4/4) [attacking]" is eleven power. If that total is at or above YOUR
printed life, Sphinx's Revelation is a LIFE spell before it is a draw spell: take its row at
whatever "max affordable X" it prints - X = 1 and X = 2 INCLUDED - and on the X menu announce the
LARGEST X offered. The cards you discard at end of turn cost nothing next to the damage you
survive, and this line outranks the max-affordable-3 floor, the full-hand line and the tap-out gate
alike.
You died to exactly this. At 10 life against those eleven power your menu read "Cast Sphinx's
Revelation ... max affordable X=2", "Cast nothing right now" and the hold row; you answered "Cast
nothing right now" in the main phase and then AGAIN in the blockers window with the same three rows
in front of you, and eleven damage took you from 10 to -1. X = 2 was 12 life and the game.
It is off for the same reason at 5 life or less whenever their header's second number is 1 or more,
whatever the attackers add up to. At 1 life with a 3/1 lifelink attacker on their line and "max
affordable X=7" on the menu, you cast a second Staff of Nin instead and died to the 3/1 next turn;
X=7 was 8 life.
```

## Edit 5 - W-A2

**Finding.** STRATEGY W-4 (deck125). `125v123` seq 37 (t15, M2, 20 life) and seq 42 (t17, M2, 22 life): "max affordable X=2" taken with only a dead Lightmine Field ("they control 0 creatures able to attack") beside it - the #194 "nothing else was castable" excuse, recurring. Low-X rows 92, takes 2 (was 0/51). Counted from the rendered prompt with the regex `max affordable X=(\d+)`; a substring match on "X=1" also hits "X=12" and inflates this count by one.

**Before:**
```
candidate tagged [defender]. A menu on which nothing is worth casting is answered "Cast nothing
right now". That is a real answer, not a wasted turn, and it is the answer 155 sweeper windows
already got in the same six games.
```

**After:**
```
candidate tagged [defender]. A menu on which nothing is worth casting is answered "Cast nothing
right now". That is a real answer, not a wasted turn, and it is the answer 155 sweeper windows
already got in the same six games.
IT CAME BACK, IN YOUR SECOND MAIN PHASE, TWICE IN ONE GAME. At 20 life and again at 22 the only
other row was "Cast Lightmine Field ... {right now: they control 0 creatures able to attack - deals
0 until they have an attacker}", and you took "max affordable X=2" both times. A DEAD LIGHTMINE
FIELD IS NOT ANOTHER CASTABLE ROW, and neither is a sweeper at 0 or a Path at a defender: each of
those is a row this guide has already answered, so a menu made only of them is an EMPTY menu, and
the number of lines printed on it is not an argument. Ninety-two low-X rows this corpus and you
held ninety.
```

## Edit 6 - W-E1

**Finding.** STRATEGY W-5 (deck125). `125v126` seq 255 (t40, 44 life): Dream Fracture cast at a Pride Guardian (creature 0/3) [defender] on the stack. First counter-on-a-wall in three corpora; game already won 44-0, so not consequential - recorded to keep the ceiling honest rather than to claim a loss.

**Before:**
```
counters on walls for two corpora running - keep it that way.
```

**After:**
```
counters on walls for three corpora, and then one: at 44 life in a game you went on to win
    44-0 you spent a Dream Fracture on a Pride Guardian (creature 0/3) [defender]. Winning is not a
    reason and neither is spare mana - put it back at zero.
```

## Edit 7 - W-E2

**Finding.** STRATEGY W-5 (deck125), same game. `125v126` seq 234 (t38, 44 life): Dream Fracture at a Chromatic Lantern whose {target text:} reads only "Lands you control have {T}: Add one mana of any color." The mana-rock line already names Chromatic Lantern; the cite is added so the paragraph carries this corpus's instance.

**Before:**
```
    enchantment halves those counters were then not there for. An (artifact) stack line with no
    P/T is a decline unless its {target text:} damages, draws or makes tokens.
```

**After:**
```
    enchantment halves those counters were then not there for. You did it again this corpus - a
    Dream Fracture on a Chromatic Lantern at 44 life, two turns after the wall above - and the
    stack row said so itself: {target text: "Lands you control have {T}: Add one mana of any
    color."} and nothing more. An (artifact) stack line with no
    P/T is a decline unless its {target text:} damages, draws or makes tokens.
```

## Edit 8 - W-F1

**Finding.** STRATEGY W-6 (deck125). Four cleanups sent a LONE sweeper ahead of the counters with the opponent header at 0 - `125v152` seq 216, `125v162` seq 64, `125v123` seq 45, `125v126` seq 166 - and none cost anything; against deck126, whose every creature is a wall, a sweeper is simply dead. The ordered list had no step for it, so the pilot was breaking the list to make the right send. The clause is gated on "every creature they have cast so far is a wall" so a lone sweeper is still kept against a creature deck.

**Before:**
```
  3. A second Supreme Verdict or Final Judgment (one sweeper in hand is enough).
```

**After:**
```
  3. A second Supreme Verdict or Final Judgment (one sweeper in hand is enough) - and a LONE
     sweeper too when the opponent header's second number reads 0 AND every creature they have cast
     so far is a wall (printed power 0, or tagged [defender]). Against that deck a sweeper is as
     dead as the Lightmine Field of step 1; you sent one ahead of your counters four times this
     corpus and it cost you nothing. Against a deck that has cast a real creature, a lone sweeper
     stays.
```

## Edit 9 - W-F2

**Finding.** MEASUREMENT (deck125), wave-54 HIGH #2 / W99 / W100. 35 cleanup asks. W100 (lands sent == min(lands on list, cards demanded) at 8+ lands) 27/27 PASS. W99 (counterspell sent while a land is on the list at 6+ lands) 1 apparent break, `125v123` seq 76, and it is a FALSE POSITIVE - one land on the list, two demanded, steps 3-6 empty, so step 7 was legitimately reached. Real breaks 0 (was 3 of 4). The "TWO OF THEM ARE TWO OF YOUR LOSSES" sentence was now false and had to go. Step 1 (Lightmine first at header 0) is 4 of 12 by the letter, and every one of the 8 misses sent a spare land or a dead sweeper instead - a rank quibble with no cost, which is why steps 1-3 are relabelled a TIER rather than given more text.

**Before:**
```
You sent a Cancel from a hand holding a Lightmine Field at 44 life against an empty board, and two
Essence Scatters from a hand of six counters with Emrakul at 34 life on sixteen lands; that second
one was fine, the first was a counter thrown away. THIS ASK IS WHERE YOU LOSE GAMES: eighteen
cleanups last corpus, three sends that broke the order, and TWO OF THEM ARE TWO OF YOUR LOSSES.
```

**After:**
```
STEPS 1 TO 3 ARE A TIER, NOT A SEQUENCE: when several of their cards sit on the numbered list, send
them in any order, and only when that tier and step 2's lands are exhausted do you read on to 4, 5,
6 and 7. THIS ASK USED TO BE WHERE YOU LOST GAMES: eighteen cleanups one corpus, three sends that
broke the order, two of them two of your losses. Thirty-five cleanups this corpus and the step-2
land rule went TWENTY-SEVEN OF TWENTY-SEVEN - no counterspell left your hand past an unsent land,
and no loss this corpus was decided at this ask. That is the ceiling; hold it.
```

## Edit 10 - MULL

**Finding.** NEW-BASELINE record (deck125). 6 offers, 0 mulligans. `125v123` seq 1 kept a ONE-land seven (Plains; Essence Scatter, 2 Supreme Verdict, Fall of the Gavel, Elixir, Path) against the guide's explicit "on a seven and on a six, SHIP IT" - and won at t39, which is why the branch is reordered rather than rewritten: the imperative now ENDS the branch instead of being followed by a win story, and the coverage line's pull is named. No rule change, no mulligan floor, no owner question.

**Before:**
```
  - ONE LAND, whatever the coverage line says. One land plays one spell and then stops; the branch
    above is about hands that need a colour, and this hand needs a second and a third land before
    anything else it holds is castable. On a seven and on a six, SHIP IT - at 26 lands in 60 the
    next look is better odds than three draws. At "would keep 5 cards" or smaller, keep it if the
    coverage line names a card. You kept a one-land seven whose coverage line named only an Elixir
    of Immortality {1} last corpus and won a 54-turn game with it; that keep was outside this line
    and the win does not move it - one land is one draw from a hand that does nothing.
```

**After:**
```
  - ONE LAND, whatever the coverage line says. Two one-land sevens have now been kept against this
    line and one of them won a long game; the win does not move it, and THE COVERAGE LINE IS WHAT
    MAKES SUCH A KEEP LOOK RIGHT. This corpus it read "would cover the cost of: Elixir of
    Immortality {1}, Path to Exile {w}" - it names those two because they cost one mana, not
    because the hand does anything, and a coverage line on a ONE-land hand is measuring a single
    turn of mana. One land plays one spell and then stops; the branch above is about hands that
    need a colour, and this hand needs a second and a third land before anything else it holds is
    castable. So, last and outranking both stories: on a seven and on a six, SHIP IT - at 26 lands
    in 60 the next look is better odds than three draws. At "would keep 5 cards" or smaller, keep
    it if the coverage line names a card.
```

## Edit 11 - W101c

**Finding.** Consistency with edit W101a: with gate shape (d) in place, Emrakul is no longer the only row that outranks the tap-out gate, and #1 said it was. Same evidence as W101a (`125v123` seq 49, `125v126` seq 49).

**Before:**
```
IT ALSO OUTRANKS THE TAP-OUT
GATE, and it is the only card in this deck that does:
```

**After:**
```
IT ALSO OUTRANKS THE TAP-OUT
GATE, as the FIRST Staff of Nin does under that gate's shape (d):
```

## Edit 12 - W101d

**Finding.** Consistency with edits W101a and W-A1: the DECIDING SITUATIONS bullet listed Emrakul as the single exception to the gate; there are now three, and a bullet that contradicts its own section is the shape S9 of the skill file is about.

**Before:**
```
- A counter is in your hand and the row of the spell you are about to cast on your own turn reads
  a "leaves N" below its price: do not cast it - and that includes a Sphinx's Revelation row. Emrakul
  is the one exception; a counter answering the stack is not covered at all.
```

**After:**
```
- A counter is in your hand and the row of the spell you are about to cast on your own turn reads
  a "leaves N" below its price: do not cast it - and that includes a Sphinx's Revelation row. The
  exceptions are Emrakul, a FIRST Staff of Nin with no Staff on your battlefield line (gate shape
  (d)), and a Revelation on a board whose attackers add up to your life or more; a counter
  answering the stack is not covered at all.
```

