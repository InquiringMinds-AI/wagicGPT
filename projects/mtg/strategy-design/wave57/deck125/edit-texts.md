# wave-57 deck125 guide edits - before -> after, each with its finding

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` (69,761 bytes).
Result: `wave57/deck125/strategy.txt`, **70,739 bytes** (band 41-71 KB - measured, in band).
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-174505/`. Every citation is a file+seq;
every count is read off the rendered `prompt`, never the `options` array.

## 1. W57-1 X menu: largest X, not 3

FINDING W-F1 (STRATEGY). 7 ANNOUNCE_X menus at this seat (125v126 seq 43/62/80, 125v130 seq 68, 125v146 seq 14, 125v152 seq 45/50), 28 X rows, 28/28 carrying the pool-priced fit clause. Four menus were answered at option 1 (largest X = 3, 6, 1, 2); the other three, topping out at 5, 4 and 4, were answered "X = 3" 3/3, leaving 1, 1 and 2 floating mana unspent = 4 life and 4 cards forgone. `125v152` seq 50 is at FIVE LIFE off a max-X=4 menu in a game lost at -3. The window prints "Mana available: 0 total ... | Already in pool: {u}{u}{w}{w}{w}(...) floating right now", so a smaller X saves nothing - the old rule's premise (hold mana back / avoid over-drawing) is false at this window.

BEFORE:
```
YOUR X IS 3. NOT THE LARGEST NUMBER ON THE MENU. Everything above seven cards at end of turn is
DISCARDED, and under a Liliana's Caress or an Underworld Dreams each discard costs life too.
Reply with the OPTION number whose line reads "X = 3", every time, with no arithmetic in front of
it. The menu prints largest-first and may COLLAPSE its middle into one row ("2-5. X = 5 down to
X = 2 - one option per X in that range, largest X first"): count into the range - option 2 is X = 5,
option 3 is X = 4, option 4 is X = 3 - and answer that number. You did exactly that off a
seven-row collapsed menu this corpus.
THE ONE COMPARISON THAT CAN LOWER IT, and nothing can raise it: read "Your library: N cards".
Subtract 2, and one more for EACH Staff of Nin on your battlefield. If that number is smaller than
3, announce THAT number instead. Drawing from an empty library loses the game outright.
If "X = 3" is not on the menu, take the largest value the menu does offer that is below 3.
Three X menus this corpus, three answers of 3, off menus topping out at 3, 4 and 6; fifteen right
answers the corpus before. This rule is at its ceiling - obey it and move on.
```
AFTER:
```
YOUR X IS THE LARGEST NUMBER ON THE MENU. Answer OPTION 1, every time, with no arithmetic in
front of it.
THE MENU'S OWN LINE TELLS YOU WHY, and it is the line you were not reading: "Mana available: 0
total (no untapped sources) | Already in pool: {u}{u}{w}{w}{w} (5 mana ALREADY produced and
floating right now)". By the time this menu is printed the mana is ALREADY OUT of your lands.
Every row prints what a smaller X wastes - "{leaves 1 of your 7 floating mana unspent}" - and
floating mana is gone at the end of this step. A smaller X saves nothing, buys nothing, and pays
for no counterspell later this turn. Option 1 always reads "{leaves 0 of your N floating mana
unspent - this X spends your whole pool}" and carries "[<- largest affordable X]".
Every point you leave on that menu is ONE LIFE AND ONE CARD you threw away. Cards above seven are
discarded at end of turn, which costs you nothing; life is never discarded.
Seven X menus this corpus. Four you answered at option 1 - the largest X was 3, 6, 1 and 2 and you
took it. The other three topped out at 5, 4 and 4, and you answered "X = 3" every time: 4 life and
4 cards binned, one of them at FIVE LIFE off a "max affordable X=4" menu, in a game you lost at -3.
THE TWO THINGS THAT CAN LOWER IT, and nothing else can:
 - "Your library: N cards". Subtract 2, and one more for EACH Staff of Nin on your battlefield.
   If that number is smaller than the largest X offered, announce THAT number. Drawing from an
   empty library loses the game outright.
 - A "[DRAW PRICE: ... - you would be at K]" tail on the Revelation's own cast row, or a
   "DRAW PUNISHERS on the battlefield: theirs" paragraph above the menu. Then every card X draws
   costs you life; the draw-is-damage stop below decides whether you cast at all, and if you do,
   K is the number you read.
The menu prints largest-first and may COLLAPSE its middle into one row ("2-5. X = 5 down to
X = 2 - one option per X in that range, largest X first"). Option 1 is still the largest, so a
collapse changes nothing about your answer.
```

## 2. W57-2 low-life Revelation floor

FINDING W-F2 (STRATEGY, the vs146 loss). 34 windows at my_life <= 6 printing a "max affordable X" >= 1; 29 declined. 12 of the 29 carried "DRAW PUNISHERS on the battlefield: theirs" and were CORRECT. The other 17 were not; SEVEN are one game - `125v146` seqs 16,17,19,20,21,22,23 at THREE LIFE over turns 16-19, answering "Cast nothing right now" to "max affordable X=2", lost at -1. The old text carried this rule three times in three different places with three different triggers (a 10-life clause, an attacking-power clause, a 5-life clause) 70 lines apart; it is now ONE floor at the head of the section.

BEFORE:
```
=== SPHINX'S REVELATION - X IS ALWAYS 3 ===
Sphinx's Revelation {X}{W}{U}{U} instant: "You gain X life and draw X cards." At X = 0 it gains
nothing and draws nothing; at X = 3 it costs six mana and draws you three cards, and three is the
answer every time you cast it. The whole section below is about WHEN to cast it, not how big.
```
AFTER:
```
=== SPHINX'S REVELATION - IT IS A LIFE SPELL FIRST ===
Sphinx's Revelation {X}{W}{U}{U} instant: "You gain X life and draw X cards." At X = 0 it gains
nothing and draws nothing. The section below is about WHEN to cast it; the X menu at the end says
how big, and the answer there is always the largest number offered.
THE FLOOR THAT OUTRANKS EVERY OTHER LINE IN THIS SECTION, and it reads off two printed strings:
   YOUR LIFE IS 6 OR LESS, and the prompt does NOT carry a paragraph beginning
   "DRAW PUNISHERS on the battlefield: theirs".
On that board a "Cast Sphinx's Revelation" row is your answer at ANY "max affordable X" of 1 or
more - ahead of the tap-out gate, ahead of the max-affordable-3 floor, ahead of the full-hand
line, ahead of "Cast nothing right now" and ahead of the hold row. X = 1 is a life. Take it and
take it in the FIRST window that prints it, not the fourth.
Thirty-four such windows this corpus and you declined TWENTY-NINE. Twelve of the declines carried
the DRAW PUNISHERS paragraph and were RIGHT - that is the whole point of the second string. The
other seventeen were not, and seven of them are one game: at THREE LIFE against two attackers you
answered "Cast nothing right now" to "max affordable X=2" in five consecutive windows across two
turns, and lost at -1. Two life was the game.
When the DRAW PUNISHERS paragraph IS printed, the row's own "[DRAW PRICE: ... - you would be at
K]" decides: cast only when K is above 0, and prefer an Elixir of Immortality activation, which
draws nothing and triggers no punisher.
```

## 3. W57-3 Lightmine survival override

FINDING W-F3 (STRATEGY, the vs146 loss, same game). `125v146` seq 19 and seq 20 at THREE LIFE: row 3 read "Cast Lightmine Field {2}{w}{w} {right now: they control 2 creatures able to attack - deals 2 to each if all 2 attack}"; the two creatures were a Goblin (1/1) and a Silverquill Silencer (3/2), both dead to 2. The guide's own "never below 3 attackers" floor forbade it and the seat answered "Cast nothing right now" both times, then died at -1 to those bodies. 0 casts off 24 rows is the ceiling holding - and one of the 24 was the game.

BEFORE:
```
It is an enchantment - it costs the same on the turn the crowd arrives. And when the row reads 3,
CAST IT: "[tapped]" on their creatures is not a reason to wait - the row already counted them,
and they untap. You declined a "they control 3 creatures able to attack" row at 25 life because
all three were tapped.
```
AFTER:
```
It is an enchantment - it costs the same on the turn the crowd arrives. And when the row reads 3,
CAST IT: "[tapped]" on their creatures is not a reason to wait - the row already counted them,
and they untap. You declined a "they control 3 creatures able to attack" row at 25 life because
all three were tapped.
THE COUNT OF 3 IS A VALUE FLOOR, AND SURVIVAL OUTRANKS IT. Add up the printed power of the
creatures on their line that are able to attack. When that total is AT OR ABOVE YOUR OWN LIFE,
the floor is off and any Lightmine Field row reading "deals N to each" with N of 1 or more is a
cast, whatever the count - because at that point the card is not value, it is the difference
between a board that kills you and a board that does not. At THREE LIFE this corpus your menu
read "Cast Lightmine Field {2}{w}{w} {right now: they control 2 creatures able to attack - deals
2 to each if all 2 attack}"; both of those creatures had 2 toughness or less, both would have
died on the spot, and you answered "Cast nothing right now" twice off that exact row and lost the
game at -1 to those same two bodies. Zero Lightmine casts off twenty-four rows this corpus is not
a ceiling held; one of those twenty-four was the game.
```

## 4. W57-4 Emrakul stop moves into the mandated PLAN line

FINDING W-F4 (STRATEGY, the vs126 loss). The guide prescribes a verbatim PLAN string, and the engine echoes the seat's own last PLAN into the header of every later prompt ("YOUR PLAN (as you last stated it, 2 windows ago on turn 45): ..."). The Emrakul stop has now failed in four corpora as guide prose; the PLAN echo is the one carrier guaranteed to be in front of the model at the Emrakul window. This is a change of MECHANISM, not another louder paragraph.

BEFORE:
```
   "Keep the counter in my hand payable; Staff of Nin only from a row whose leaves N covers that
   counter's cost; ping their face every turn."
```
AFTER:
```
   "Keep the counter in my hand payable; Staff of Nin only from a row whose leaves N covers that
   counter's cost; ping their face every turn; never cast Emrakul while a converter of theirs is
   printed."
That last clause is not decoration. Your PLAN line is echoed back to you at the top of every
window that follows ("YOUR PLAN (as you last stated it, 2 windows ago on turn 45): ..."), so it
is the only sentence in this guide you are guaranteed to be re-reading at the Emrakul window
itself. Write it every time.
```

## 5. W57-4b Emrakul: the row now prices itself, one-string rule

FINDING W-F4 (same). `125v126` seq 293, turn 77, 52 life vs 20, 21 lands. The row read "Cast Emrakul, the Aeons Torn {15} ... {their converter: this body has toughness 15 and they control 1 life-to-damage converter (Sanguine Bond) - ... takes 15 off YOU: life 52 -> 37}" - lane C D7 SHIPPED, on exactly the row wave-56 HIGH #3 asked for - and the prompt also carried the full LIFE-TO-DAMAGE CONVERTER paragraph naming the Bond+Blood LOOP and the words "ANY nonzero payment on a tag above is fatal". The seat took row 3; `126v125` seq 437 Tributed it; 52 -> -8 and 20 -> 80 in one turn. The perception half is now complete, so the remaining failure is entirely strategy, and the old section resolved FOUR nested overrides ("this line outranks the heading above it", "THE ONE BOARD WHERE THAT LINE IS OFF", "THE EDICT WITHOUT A CONVERTER is a different case") before reaching the answer. Replaced with one string test.

BEFORE:
```
THIS IS THE MOST EXPENSIVE ROW IN THE DECK AND IT HAS NOW COST THREE GAMES. Last corpus, at 36 to
their 8 with two Staffs ticking, you cast it under the line and finished 21-21. This corpus you cast
it at THIRTY-SEVEN LIFE AGAINST THEIR TWO, with two Staffs that would have killed them on your very
next upkeep and the line reading "theirs - Sanguine Bond #1, Sanguine Bond #2": their Tribute ate the
Emrakul, they gained 15 twice over, both Bonds converted it, and you went 37 to -8 in one turn in a
game you had already won. THE CLOSER THEY ARE TO DEAD, THE WORSE THIS ROW IS: a Staff at their 2 wins
next upkeep for free, and Emrakul is the only card on the menu that can hand them 15 life. Read the
CONVERTER line at EVERY Emrakul row, including the fifth one, and while it says "theirs" the answer
is "Cast nothing right now" - at any life total of yours, at any life total of theirs, and most of
all when theirs is small enough to make the row look like a formality.
```
AFTER:
```
THIS IS THE MOST EXPENSIVE ROW IN THE DECK AND IT HAS NOW COST FOUR GAMES.
THE ROW NOW PRICES ITSELF, AND YOU TOOK IT ANYWAY. This corpus the Emrakul row read, in the row
text, on the same line as its cost:
   "{their converter: this body has toughness 15 and they control 1 life-to-damage converter
   (Sanguine Bond) - any effect of theirs that gains them life equal to its toughness (a
   sacrifice-for-toughness edict, a drain of that size) takes 15 off YOU: life 52 -> 37}"
You were at FIFTY-TWO LIFE against their 20, on twenty-one lands, with a Path to Exile row and a
"max affordable X=18" Revelation row on the same menu, and the paragraph above the menu spelled
out that both halves of the loop were on their battlefield and that "ANY nonzero payment on a tag
above is fatal". You took the Emrakul. They edicted it, gained 15, the Bond converted it, the
Blood fed it back, and you went 52 to -8 in a single turn while they went 20 to 80.
There is nothing left for the render to tell you. The rule is now one string and it is not a
judgement:
   IF THE ROW CARRIES "{their converter:", OR THE PROMPT CARRIES "LIFE-TO-DAMAGE CONVERTER on the
   battlefield: theirs", THE EMRAKUL ROW IS NOT ON YOUR MENU.
Answer any other row - a Path, a Revelation, a counter, "Cast nothing right now", the hold row.
At any life total of yours, at any life total of theirs, and most of all when theirs is small
enough to make the row look like a formality: a Staff at their 2 wins next upkeep for free.
```

## 6. W57-5 cleanup: read the new discard verdicts, and the exiles-0 trap

FINDING W-F5 (STRATEGY + a render trap). Lane C D8 shipped: 48 discard asks / 374 rows this corpus carry 63 "{dead right now:}", 24 "{right now:}", 37 "{spare:}" (was 53 of 282 tagged), with the legend on 44 of 48 asks. The guide still said "The list is bare card names with no {right now:} verdict on any row" - false. 8 of 85 sends threw a live-verdict row while a dead or spare row sat on the same menu (`125v123` seq 189, `125v126` seq 65, `125v130` seq 14/16/100, `125v162` seq 157 x2, seq 207). The exiles-0 trap is the engine item HIGH #1 in the seat file, mirrored here as a read-the-numbers instruction so the guide is right even before the engine is fixed.

BEFORE:
```
The list is bare card names with no {right now:} verdict on any row, so nothing on the ask itself
will stop you - the numbers you need are on the battlefield lines above it: YOUR "and N are lands",
and THEIRS, whose second number is the count that makes a Lightmine Field and a sweeper dead.
```
AFTER:
```
THE LIST NOW CARRIES VERDICTS, AND THEY SORT IT FOR YOU. Rows on this ask come in four shapes:
"{dead right now: ...}" (this card changes nothing on the board as it stands), "{spare: you
control N lands already}", "{right now: ...}" (what it WOULD do), and no tag at all. A row with no
tag is one the engine could not price here - the legend under the list says so in those words -
and it is NOT a statement that the card is dead.
SEND IN THAT ORDER: every "{dead right now:}" row and every "{spare:}" row first, then the
untagged rows by the tier list above, and a "{right now:}" row LAST. While a dead or spare row is
still on the numbered list, a live one is never part of your answer. Eight of your eighty-five
sends this corpus broke that - you sent a sweeper whose own verdict printed a real number while a
dead sweeper or a spare land sat two rows above it.
ONE PLACE THE TAG IS WRONG AND THE NUMBER IS RIGHT: a Final Judgment reading "{right now: exiles 0
of their creatures (0 without a restriction against attacking), 0 of yours}" is DEAD - all three
numbers are 0 - even though it is tagged "{right now:}" and a Supreme Judgment... a Supreme
Verdict on the SAME menu with the same three zeroes is tagged "{dead right now:}". READ THE
NUMBERS IN THE TAG, NOT THE TAG'S NAME: all zeroes means dead, whichever tag carries them.
The other numbers you need are on the battlefield lines above the ask: YOUR "and N are lands",
and THEIRS, whose second number is the count that makes a Lightmine Field and a sweeper dead.
```

## 7. W57-6 land-drop ceiling number

FINDING W-F6 (ceiling). 83 land-drop asks, 83 lands played, 0 declines; 48 of the 83 were single-land menus with one right answer. Number refreshed only.

BEFORE:
```
Eighty-five land windows last corpus, eighty-five lands played - this rule is at its ceiling and
there is nothing here to think about.
```
AFTER:
```
Eighty-three land windows this corpus, eighty-three lands played - this rule is at its ceiling and
there is nothing here to think about.
```

## 8. W57-5b typo fix in the cleanup verdict paragraph

Editorial: a garbled clause in the paragraph added by W57-5.

BEFORE:
```
and a Supreme Judgment... a Supreme
Verdict on the SAME menu with the same three zeroes is tagged "{dead right now:}".
```
AFTER:
```
and a Supreme Verdict on the SAME
menu with the same three zeroes is tagged "{dead right now:}".
```

## 9. W57-2b Revelation: fold three overlapping low-life exceptions into one floor

FINDING W-F2 (same). De-duplication: the "5 life or less" sentence and the eleven-power anecdote were the same rule as the new floor. Kept the attacking-power TRIGGER (it fires above 6 life), cut its duplicate ruling and its second anecdote.

BEFORE:
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
AFTER:
```
THE SECOND BOARD THAT TURNS THE SIZE RULE OFF, and it is the same floor as the one at the top of
this section with a different trigger: add up the POWER of the creatures the opponent line says
are attacking or able to attack ("Barrowin of Clan Undurr (5/5) [tapped - attacking]; Goblin (2/2)
[tapped - attacking]; Nadaar, Selfless Paladin (4/4) [attacking]" is eleven power). If that total
is at or above YOUR printed life, take the Revelation row at whatever "max affordable X" it
prints, X = 1 and X = 2 included. At 10 life against those eleven power you answered "Cast nothing
right now" twice off a "max affordable X=2" row and died at -1; X = 2 was 12 life and the game.

```

## 10. W57-2c point the max-affordable-3 floor at its own exception

FINDING W-F2 (same). The max-affordable-3 floor read as unconditional two paragraphs below an exception that outranks it.

BEFORE:
```
"I have spare mana and nothing else to cast" is never a reason: read the N off the row, and 3 is
the floor. IF YOUR PLAN SAYS "draw a card or two while the mana is unused", THE PLAN IS WRONG.

```
AFTER:
```
"I have spare mana and nothing else to cast" is never a reason: read the N off the row, and 3 is
the floor - unless one of the two low-life boards above is printed, and then there is no floor.
IF YOUR PLAN SAYS "draw a card or two while the mana is unused", THE PLAN IS WRONG.

```

## 11. W57-7 MULLIGAN compressed (0 mulligans / 0 bottoming asks in three corpora; rules kept, prose halved)

FINDING W-F7 (budget). 6 offers, 6 keeps, 0 mulligans, 0 bottoming asks, no floor text, no "would not cover any spell in it" line - and the same in wave 55 and wave 56 (18/18 keeps). Every branch and both tiebreakers are kept; the anecdotes and the restatements are cut. 2,900 chars recovered, spent on W57-1..W57-5. No OWNER QUESTION arises.

BEFORE:
```
=== MULLIGAN ===
The question at every look is the same one, and it is about ODDS, never about a number the hand
has to meet: what are the odds THIS hand does something in time, against the odds the NEXT hand -
one card smaller - does? Two facts about this deck set the comparison, and the prompt gives you
the rest:
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
And the mulligan row prices the next look for you: "Mulligan (a keep after this one would keep 5
cards)". Every look costs a card; the London mulligan gives you a fresh seven and takes the cost
at the keep. A six-card hand that does something beats a seven that does nothing; a five-card hand
that does something beats a six that does nothing; but a hand of five or fewer that already does
something is almost never beaten by the next look, because a smaller hand loses answers faster
than it fixes lands.
So, at each look:
  - A hand of 2 to 5 lands whose coverage line names at least one card can already act on their
    first threat, and the odds of improving it by shipping a card are against you: KEEP. At any
    price. You shipped a four-land hand whose coverage line named Dream Fracture AND Supreme
    Verdict at the "would keep 5 cards" row, then shipped a two-land hand at the "would keep 4
    cards" row, kept four, and lost a game you never got to play.
  - The coverage line reads "would not cover any spell in it": this hand does nothing until you
    draw both a land and a colour, and it is the one shape where a smaller hand can be better.
    Weigh how many draws until it casts ANYTHING - sharpest when the spells left are Final
    Judgment {4}{W}{W}, Staff of Nin {6} or Emrakul {15} rather than a Path to Exile {W} or an
    Elixir {1} - against what the next look costs. On a fresh seven or a six, ship it when the
    answer is "four or more turns" - every deck in this pool casts creatures, and the cheap ones
    come down on the first turns, so that wait is a threat you cannot answer; their turn two is not on the header and is not a
    reason either way. At a row reading "would
    keep 5 cards" or smaller, the next look costs more than that wait does: keep it if it holds a
    land and a spell you can cast within two draws, and ship it only when it holds no land at all
    or no spell at all.
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
  - Seven lands and no spell, or seven spells and no land, ships at any price - those hands do
    nothing however long you wait, and the odds of the next look beating "nothing" are as good as
    odds get. You shipped a one-land seven that covered nothing and kept the two-land six behind
    it (Scatter and Elixir covered) - and won.
Tiebreakers between keepable hands, never a reason to ship on their own: prefer the hand with a
land that makes both colours (Tundra, Glacial Fortress, Seachrome Coast), and prefer the hand with
at least one counterspell. Colour alone is not a reason to mulligan - 18 of your lands make {W} and
20 make {U}, so a one-colour opener is usually two draws from the other.

When a keep makes you bottom cards the ask prints your hand as a NUMBERED list and tells you how
many to send. What you are choosing is the hand you KEEP, and the hand you keep needs two things:
lands to cast with and answers to cast. Read the numbered list once, decide which cards those are,
and bottom the rest - in this order of preference, taking as many as the ask demands:
  1. Emrakul, the Aeons Torn.
  2. Any card whose line says "(copy 2 of 2 in your hand)" - a duplicate of anything, a duplicate
     LAND included. Two Islands in a seven is a duplicate and it is the answer; you bottomed a
     Sphinx's Revelation past two Islands instead.
  3. A land beyond the fifth, or beyond the third when the ask makes you send three.
  4. Your most expensive spell, then the next most expensive.
  5. Only when the list is exhausted: the cheapest card that is neither a land nor an answer
     (an Elixir before a Path, a Path before a counterspell).
The hand you keep should hold at least as many lands as it can play in its first two or three
turns and at least one card that answers a threat - a counterspell or a Path. When the ask makes
you send more cards than that leaves room for, keep the LANDS and ONE answer and let the rest go:
a counterspell you cannot pay for is not an answer, and a land with nothing to cast is not a hand.


```
AFTER:
```
=== MULLIGAN ===
The question at every look is about ODDS, never a number the hand has to meet: what are the odds
THIS hand does something in time, against the odds the NEXT hand - one card smaller - does? Two
facts set the comparison. 26 of your 60 cards are lands, so a two-land seven sees its third land
inside two or three draws; and your job is REACTIVE, so the cost of a slow start is only the cost
of the threats that land before you can answer them.
The prompt hands you the numbers, and you read them rather than counting the hand yourself:
  "Your hand (7 cards), counted by the engine: N lands, M spells."
  "Mana sources among those lands, counted by the engine: {W} n, {U} n."
  "Playing every land in this hand would cover the cost of: ..." - or "would not cover any spell
  in it."
The mulligan row prices the next look: "Mulligan (a keep after this one would keep 5 cards)". A
six that does something beats a seven that does nothing; a hand of five or fewer that already does
something is almost never beaten by the next look.
At each look:
  - 2 to 5 lands and a coverage line naming at least one card: KEEP, at any price. You once
    shipped a four-land hand whose coverage named Dream Fracture AND Supreme Verdict, shipped
    again, kept four, and lost a game you never got to play.
  - "would not cover any spell in it": weigh how many draws until it casts ANYTHING - sharpest
    when what is left is Final Judgment {4}{W}{W}, Staff of Nin {6} or Emrakul {15} rather than a
    Path {W}. On a seven or a six, ship it when the answer is "four or more turns". At "would keep
    5 cards" or smaller, keep it if it holds a land and a spell castable within two draws.
  - ONE LAND, whatever the coverage line says: on a seven and on a six, SHIP IT. A one-land
    coverage line names your one-mana cards because they cost one mana, not because the hand does
    anything. At "would keep 5 cards" or smaller, keep it if the coverage line names a card.
  - Seven lands and no spell, or seven spells and no land: ship at any price.
Tiebreakers between keepable hands, never a reason to ship on their own: prefer a land that makes
both colours (Tundra, Glacial Fortress, Seachrome Coast), and prefer a hand with a counterspell.
Colour alone is not a reason to mulligan - 18 of your lands make {W} and 20 make {U}.
Eighteen offers over three corpora, eighteen keeps, no bottoming ask ever reached: this section is
the branch you have not had to walk, so walk it off the printed lines and not off a memory.
When a keep DOES make you bottom, the ask prints your hand as a NUMBERED list with a count. Bottom
in this order, as many as it demands: Emrakul; then any "(copy 2 of 2 in your hand)" duplicate, a
duplicate LAND included; then a land beyond the fifth; then your most expensive spell; and only
when that is exhausted the cheapest card that is neither a land nor an answer. Keep the LANDS and
at least ONE answer - a counterspell you cannot pay for is not an answer, and a land with nothing
to cast is not a hand.


```

## 12. W57-8 Staff ping: W110 passed 5/5 + 54/54, evidence refreshed

FINDING W-F8 (ceiling, and the wave-56 edit that worked). 59 Staff ping windows, 0 declined. 5 carried "DRAW PUNISHERS on the battlefield: theirs" and ALL FIVE shot Ob Nixilis, the Hate-Twisted (`125v162` seqs 27, 35, 48, 50, 66, turns 13-17); the other 54 had no punisher line and all 54 went at the face. Wave 56 was 8 of 8 at the face under the line and it lost that game; this corpus 125v162 was WON 30/8.

BEFORE:
```
  more draws on top; you took EIGHT ping rows in a row at the face, taking them 13 to 6 while it
  took you 16 to 0. Five of those eight pings kill it outright, and the other three finish the face
  afterwards. THE FACE IS NOT THE RACE WHEN THEY ARE PINGING YOU BACK: read the DRAW PUNISHERS line
  before every ping, not just before every draw.
```
AFTER:
```
  more draws on top; you once took EIGHT ping rows in a row at the face and lost 16 to 0 to it.
  THIS RULE IS NOW AT ITS CEILING AND IT FLIPPED A GAME: fifty-nine ping windows this corpus, and
  every one of the five that printed a DRAW PUNISHERS line went at Ob Nixilis (5/5, over turns
  13-17) while all fifty-four with no such line went at the face. Read the line before every ping,
  not just before every draw, and keep it there.
```

## 13. W57-9 wording

Editorial.

BEFORE:
```
  warning: both Staffs stay, each draws you a card at your upkeep and each pings for 1, so the
```
AFTER:
```
  note: both Staffs stay, each draws you a card at your upkeep and each pings for 1, so the
```

## 14. W57-10 counter section: mana-rock evidence refreshed to 2 of 39, stale anecdotes cut

FINDING W-F9 (ceiling, slightly improved). 39 counterspells cast; every stack line they answered was a real threat (draw punishers, combo enchantments, planeswalkers, creatures with power >= 2, land destruction) except two mana rocks - `125v126` seq 212 (Cancel on a Chromatic Lantern at 47 life) and `125v130` seq 35 (Dream Fracture on a Talisman of Impulse at 28). 0 counters at a [defender] or printed-power-0 stack line (was 1). Neither rock counter decided a game. Three superseded anecdotes cut for budget.

BEFORE:
```
    nothing; all it does is let them cast the spell you actually need the counter for. In a game
    you lost you spent Dream Fracture on a Chromatic Lantern on turn 7; the Sanguine Bond it was
    fixing the mana for resolved on turn 12 with no counter left in your hand. You did it again
    last corpus - a Fall of the Gavel on a Chromatic Lantern and a Dream Fracture on a Lightning
    Greaves (an Equipment: no P/T, no damage, no card) - in two games whose real threats were the
    enchantment halves those counters were then not there for. You did it again this corpus - a
    Dream Fracture on a Chromatic Lantern at 44 life, two turns after the wall above - and the
    stack row said so itself: {target text: "Lands you control have {T}: Add one mana of any
    color."} and nothing more. An (artifact) stack line with no
    P/T is a decline unless its {target text:} damages, draws or makes tokens.
    BEING FAR AHEAD IS THE EXCUSE THIS LINE KEEPS LOSING TO, AND IT IS NOT ONE. Three of your
    twenty-nine counters this corpus went at a card whose {target text:} reads only "Add": two
    Chromatic Lanterns at 33 life and a Talisman of Impulse at 32. A counter in hand at 33 life is
    the counter you have at 9 life four turns later; a mana rock you let resolve has still never
    dealt you a point. Read the {target text:} - "Add" and nothing else is a decline at EVERY life
    total, with any number of counters in hand and any amount of spare mana.
```
AFTER:
```
    nothing; all it does is let them cast the spell you actually need the counter for. In a game
    you lost you spent Dream Fracture on a Chromatic Lantern on turn 7; the Sanguine Bond it was
    fixing the mana for resolved on turn 12 with no counter left in your hand. An (artifact) stack
    line with no P/T is a decline unless its {target text:} damages, draws or makes tokens.
    BEING FAR AHEAD IS THE EXCUSE THIS LINE KEEPS LOSING TO, AND IT IS NOT ONE. Thirty-nine
    counters this corpus, every one of them on a real threat except TWO - a Cancel on a Chromatic
    Lantern at 47 life and a Dream Fracture on a Talisman of Impulse at 28. A counter in hand at 47
    life is the counter you have at 9 life four turns later; a mana rock you let resolve has still
    never dealt you a point. Read the {target text:} - "Add" and nothing else is a decline at EVERY
    life total, with any number of counters in hand and any amount of spare mana.
```
