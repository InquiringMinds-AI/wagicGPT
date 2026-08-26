# deck130 - wave-46 guide edits, before -> after, each with its finding

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` as of the wave-45
reviewer revision + the reviewer boundary pass (3ca0b2622). Full revised file: `strategy.txt`
beside this one. Every card fact asserted in a new sentence was read off
`projects/mtg/bin/Res/sets/primitives/mtg.txt` with `/usr/bin/grep`: Underworld Dreams
`auto=@drawfoeof(player):damage:1 opponent` / `type=Enchantment`, Fate Unraveler the same auto
line / `type=Enchantment Creature` (3/4), Starstorm `auto=damage:X all(creature)` +
`autohand=__CYCLING__({3})`, Thraben Doomsayer `auto={T}:token(Human,...,1/1,white)`, Rorix
Bladewing 6/5 `{3}{R}{R}{R}`, Siege-Gang Commander `{3}{R}{R}`, Talisman of Impulse
`auto={T}:Add{1}`. File is pure ASCII; no line I added exceeds 100 chars.

---

## EDIT D46-1 - a new #0 RULE above the cycling table: read their board for a draw punisher

FINDING, and it is the seat's whole loss. vs162 seq 27, turn 11, life 3. The opponent's
battlefield line named Underworld Dreams, Fate Unraveler AND Ob Nixilis, the Hate-Twisted, each
with its trigger text rendered in `{effect:}` on its own entry, each dealing 1 damage per card
the pilot draws. The pilot's PLAN, one sentence before its answer, names all three and says
"I need to draw cards to find a way to break the lock". It answered `cycling with Starstorm`.
One card drawn is exactly 3 damage. Gameend: 0 life, same turn.
It is simultaneously a violation of the guide's own Starstorm cycling gate ("do not cycle
Starstorm while the Opponent battlefield line names ANY creature" - Shield Sphere and Fate
Unraveler were both listed), so the specific rule and the general instinct both failed to a class
the guide had never named. deck123's guide has carried a draw-punish clause for Vision Skeins
since wave 43; deck130, which draws far more often, had none.

BEFORE: the file opened on `#1 RULE - THE CYCLING TRAP`.
AFTER: a new `#0 RULE - BEFORE ANY DRAW, READ THEIR BATTLEFIELD FOR A DRAW PUNISHER` above it,
stating that it OVERRIDES the cycling rule, keyed to the rendered `{effect:}` text rather than to
a memorised card list (the three names in this pool are given as examples, with the instruction
to read the line), covering all three draw shapes this deck has (a cycling row, Forgotten Cave,
Pyrite Spellbomb's draw mode), with two branches: one punisher, cycle only above 5 life; two or
more, do not draw at all. The loss is named in numbers.
WHY A NEW RULE AND NOT A ROW IN THE CYCLING TABLE: the table is per-card and this is per-board;
a per-card table entry would have to be repeated four times and would still miss the Spellbomb.

## EDIT D46-2 - rule #2 step 0: a three-step procedure for choosing X

FINDING (a), the decisive one. vs123 seq 16 -> 18, turn 7. The cast row printed
`{X pricing: max affordable X=3 (5 mana total); smallest X that kills an OPPONENT creature: X=1
(3 mana total) - kills THEIRS: Human; YOURS: none. At X=3 - kills THEIRS: Thraben Doomsayer,
Human; YOURS: none}` and the announce menu printed `1. X = 3 {kills THEIRS: Thraben Doomsayer,
Human; YOURS: none}` / `2. X = 2 {same kills as X=3, for 1 less mana}` / `3. X = 1 {kills THEIRS:
Human}` / `4. X = 0`. It answered option 3. The seat controlled ZERO creatures, so `YOURS: none`
was true at every row and the bigger number was free. Thraben Doomsayer lived, made twenty-one
more tokens behind an Intruder Alarm, and the game ended 19 to -31.
FINDING (b). vs123 seq 23 answered option 1 (X=4) with option 2 reading "same kills as X=4, for
1 less mana"; vs126 seq 31 answered option 1 (X=2) with option 2 reading "same kills as X=2, for
1 less mana". Two mana thrown away for identical board results - #W46-8's own prediction,
failing 2 of 4, from the guide side.
The guide is complicit in (a): "Pick the smallest X that reaches the toughness you must kill" and
the render's lead clause "smallest X that kills an OPPONENT creature" both push down.

BEFORE: "AND WHEN YOU DO CAST IT, READ THE X MENU'S OWN SENTENCE ... Decide your number first,
then find the row whose text reads "X = " followed by that number".
AFTER: a numbered procedure that reads the rows instead of deciding a number in advance.
STEP 1, take the row with the LARGEST `kills THEIRS` list whose `YOURS:` reads `none` - bigger is
free when YOURS is none, and the seat's own loss is named. STEP 2, then walk DOWN to the BOTTOM
of any run of rows reading "same kills as X=N", because that run is the same board for less mana.
STEP 3, if a row's `YOURS:` names Rorix or Siege-Gang, that row and everything above it is out
and you do not cast at all (the existing carve-out (a), unchanged, now reachable from the
procedure). Closes by naming the cast row's "smallest X" clause as a FACT about the cheapest
kill rather than a recommendation, and pointing at the "At X=N" clause beside it.
The DECIDING SITUATIONS announce entry is rewritten to the same three steps, plus two new
recognise-lines for the collapsed-run case and the both-YOURS-none case (skill 51).
The `Pick the smallest X that reaches the toughness you must kill` sentence in the STARSTORM card
line is left alone deliberately - it is the right instruction when YOURS is non-empty, and the
procedure above now outranks it by first-match.

## EDIT D46-3 - the "Choose an option for Starstorm:" menu, re-formed rather than restated

FINDING. vs125 seq 60/61/62, turn 23, both boards at 0 creatures. seq 60's plan: "Cast Starstorm
at X=0 to cycle it ... per Strategy Guide Rule #1". seq 61: it answered "1. Cast Card Normally"
over "2. cycling". seq 62: option 9, X = 0. The guide already carries `CASTING IT AT X=0 IS NOT
CYCLING`, the two-row menu described by name, and `NEVER announce X=0` - all added last wave -
and the pilot cited the rule by number while breaking it. This is the SECOND corpus with this
exact three-window sequence at this seat.

BEFORE: "If your reason for casting is "spend the card" or "cycle it legally", the answer you
want is the cycling row."
AFTER: the same fact re-keyed to two things the pilot can check about ITSELF rather than about
its reasons - (1) if the word "cycle" or "cycling" appears anywhere in the plan being carried
into this menu, the only row that carries it out is the row whose text is the single word
"cycling"; (2) a tripwire: reaching the "Announce the value of X for Starstorm" screen is PROOF
you took the wrong row, because cycling never asks for an X.
STATED LIMIT, recorded here rather than in the guide: if this fails a third time the guide lane
is exhausted and ENGINE H2 (annotating the two bare menu rows) is the only remaining remedy. A
fourth restatement should not be written.

## EDIT D46-4 - Dwarven Blastminer: the same land gate everything else carries, plus a life floor

FINDING. vs152 seq 19 (turn 9, life 11) and seq 23 (turn 11, life 5): both spend `{2}{R}, Tap`
destroying a land while deck152's board held Elite Spellbinder, Katilda and, by seq 23, an
untapped Sigarda, Champion of Light (4/4 trample flying). Opponent land count seven and eight.
Dead at -2 on turn 12. The guide's own gate is FOUR OR FEWER lands, stated in HOW THE DECK WINS
and in cast-order entry 3 - but the DWARVEN BLASTMINER card line said "Activate him every turn
you have three spare mana and their board shows a non-basic land", ungated, and that is the line
the pilot followed. An internal contradiction in my own guide.

BEFORE: "Activate him every turn you have three spare mana and their board shows a non-basic
land."
AFTER: the same FOUR-OR-FEWER-lands gate that every other land-destruction line here carries,
with the two losing windows named, plus a LIFE FLOOR on the whole land plan: at 8 life or less,
stop destroying lands and spend the mana on the creature that is killing you, on a body, or not
at all. Two new DECIDING SITUATIONS entries to match.
WHY A LIFE FLOOR AND NOT ONLY THE LAND GATE: the land gate alone would still have permitted
seq 19 if the opponent had held four lands, and the thing that killed this seat was spending its
last three turns on a plan that does not affect the board that was killing it.

## EDIT D46-5 - RACE CHECK: the carve-out the pilot found and the rule did not have

FINDING (skill 47, pilot right, letter wrong). vs123 seq 37: life 11, one attacker offered
(Rorix), opponent battlefield 22 untapped fliers totalling 89 printed power. The RACE CHECK's
arithmetic says their turns-to-kill (1) is smaller than yours (4), so hold. The pilot sent Rorix
with the reply "We are losing the race but have no better plays" - and it is right: one blocker
stops one attacker, so holding subtracts 4 damage from 89 and changes nothing.

BEFORE: "Only when you WIN the race, or when their untapped creatures total 0 power, go on to
the floor."
AFTER: one carve-out above that line - if their untapped total power exceeds your life PLUS your
one creature's toughness, holding does not move the turn you die, so send it. The vs123 numbers
are named. The wave-45 shape the rule was written for (4 life against a single 4/4 with
vigilance) did NOT recur this corpus, so the rule's core is UNTESTED and is not touched.

---

# THRESHOLD WHYS (skill amendment 61) - every numeric threshold this guide now ships
- T = 2 draw punishers (the #0 RULE hard stop) because two of them make every draw cost 2 or more
  and this deck gains no life at all; at 3 life against three of them one cycle was exactly
  lethal. T = 5 life for the single-punisher branch because one punisher costs 1 per draw and a
  draw step is already one of them, so 5 leaves room for the draw you cannot decline.
- T = "YOURS: none" (the X step-1 gate) is not a number but the whole condition: with nothing of
  yours on the board a bigger X has no cost at all, and the seat lost a game paying one less mana.
- T = FOUR OR FEWER opponent lands (the land-destruction gate, now on Blastminer too) unchanged
  from wave 44: land destruction is tempo, and past four lands the card does not change what they
  can cast.
- T = 8 life (the new land-plan floor) because the two windows that lost vs152 were at 11 and 5,
  the deck's own removal costs one to three mana, and below 8 a single unanswered attacker on
  their board ends the game before a destroyed land matters.
- T = 6 life (the face-burn floor) unchanged and HELD this corpus: no damage spell fired at a
  player above 6.
- T = your life + your creature's toughness (the new RACE CHECK carve-out) because that is the
  most one blocker can subtract from an incoming swing; above it, holding cannot change the turn
  you die.
- T = 2 (the collapsed tag's N) unchanged.
- T = 6 cards (the mulligan floor) unchanged and untested: 6 straight keeps, 0 mulligans.

# WHAT THIS FILE DOES NOT CHANGE
- The mulligan section, including the boundary pass's coverage-line branch (B7): 6 of 6 keeps,
  two of them one-land sevens taken on that line, one win and one loss. Working as designed;
  skill 66 forbids moving it on this evidence.
- The Starstorm HOLD carve-outs (a) and (b): not armed this corpus (no window put Rorix or
  Siege-Gang on a `YOURS:` list, and no all-`[defender]` THEIRS list arose at an affordable X).
- The face-burn floor and the LD targeting rules: both HELD, nothing to add.
- The lethal-chump legality qualifier (EDIT 45-5): 2 blockers windows, neither LETHAL-flagged.
  UNTESTED for a second corpus; left exactly as written.
