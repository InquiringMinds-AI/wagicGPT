# deck130 (Budde's Ponza) - wave-53 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (wave-52 reviewer
revision `e68b7c7c6` + boundary pass `2d83e169f`), 59,011 bytes -> 61,746 bytes (**+2,735**),
six edits (five additions, one compression). Corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260902-023342/`; every seq below is a deck130 seat log.
ASCII only; no non-`[ -~\t]` byte in the file.

---

## D53-1 (#2 RULE, new step 0 + header) - LETHAL FIRST
**Finding, and the game it lost:** 130v162, dead at **0 to 1** on turn 24.
- **s62** (turn 22, 7 life, **Opponent life: 1**, ten untapped sources): row 1 read
  `Cast Hammer of Bogardan {1}{r}{r} {leaves 7 of your 10 untapped mana sources untapped}
  {kills 0 of the 2 legal targets at 3 damage} - legal targets right now: Shield Sphere ...`.
  Taken.
- **s63**: the target ask listed `1. The opponent (player, life 1)`, `2. Shield Sphere (0/6)
  ... SURVIVES`, `3. Ob Nixilis, the Hate-Twisted [planeswalker]`, `4. Fate Unraveler (3/4) ...
  SURVIVES`, `5. Yourself`. **Answered 3.** Hammer of Bogardan is `target=anytarget auto=damage:3`
  (mtg.txt:52046) - row 1 was the game.
- **s65**: `Cast Pyrite Spellbomb {1}` on the menu with seven sources open (`{R}, Sacrifice:
  2 damage to any target`, mtg.txt:90168) - declined as "no direct damage spells in hand".
- Two more turns of the same: **s46/s51** (turn 20, Opponent life 1) spent on Lay Waste and
  Molten Rain against two of their lands.
The kill-count literal was actively misleading here: lane O records that the cast summary
"counts only creature targets ... player targets ... are excluded from the denominator", so a
lethal face row prints `{kills 0 of the 2 legal targets}`.
**BEFORE (header):** *"#2 RULE - DAMAGE GOES AT THEIR CREATURES AND PLANESWALKERS, NOT THE FACE."*
**AFTER (header):** *"#2 RULE - LETHAL FIRST; OTHERWISE DAMAGE GOES AT THEIR CREATURES AND
PLANESWALKERS, NOT THE FACE."*
**AFTER (new step 0, above the existing step 1):** *"0. READ "Opponent life:" FIRST, BEFORE ANY
OF THE STEPS BELOW. It is printed on every single decision. If one row on this menu deals that
number or more to a player - Hammer of Bogardan 3, Pyrite Spellbomb 2, Spark Spray 1, each Goblin
you can sacrifice 2 - then the face row, written "The opponent (player, life N)", WINS THE GAME
IN THIS WINDOW. Take it now. Nothing below this line applies, the CROSS-CHECK latch does not
apply, and neither does anything that is killing you: a game you win this window cannot be lost
next window. THE KILL COUNT ON THE CAST ROW DOES NOT SEE THIS. "{kills 0 of the 2 legal targets
at 3 damage}" counts only CREATURES; the player is never in that count, so a row that says it
kills nothing is still lethal when their life is at or below its damage. At "Opponent life: 1",
holding Hammer of Bogardan and Pyrite Spellbomb with ten mana, this deck read "{kills 0 of the 2
legal targets at 3 damage}", aimed the Hammer at a planeswalker instead of at the row reading
"The opponent (player, life 1)", declined the Spellbomb next window as "no direct damage", and
lost that game 0 to 1."*

## D53-2 (DECIDING SITUATIONS) - the same rung on the recognise->do surface (#177)
The planeswalker bullet is what the pilot followed at s63 (Ob Nixilis at `[counters: 3x loyalty]`,
Hammer 3). A lethal bullet is inserted ABOVE it and the planeswalker bullet gains the exception.
**AFTER (new first bullet):** *""Opponent life: N" and one row on this menu deals N or more to a
player -> take the face row, "The opponent (player, life N)", THIS window. Ahead of the
planeswalker bullet below, ahead of the 6-life floor, ahead of the CROSS-CHECK latch, ahead of
whatever is killing you. The cast row's "{kills 0 of the K legal targets}" counts creatures only
and never counts the player, so it says "kills 0" on a row that wins the game."*
**BEFORE (planeswalker bullet tail):** *"-> cast it at the planeswalker, before any creature you
could cast this turn."*
**AFTER:** *"... before any creature you could cast this turn - unless the bullet above fires: at
"Opponent life: 1" this deck put a 3-damage Hammer into a 3-loyalty Ob Nixilis and lost 0 to 1."*

## D53-3 (HOW THE DECK WINS) - the land plan ends inside lethal range
**Finding:** 130v162 s46 (`Cast Lay Waste`, opponent at 1, six of their lands) and s51
(`Cast Molten Rain`, opponent at 1). Also the land gate itself: **6 of 26** land-destruction casts
this corpus went against a battlefield line printing five or more lands (130v123 s29 at 5;
130v125 s131 at 7, s150 at 10; 130v162 s41 cycle at 5, s46 at 6, s51 at 5) - wave 52: 8 of 19.
**AFTER (appended to the tempo sentence):** *"AND THE PLAN ENDS THE MOMENT THEY ARE IN RANGE:
when "Opponent life:" is at or below the damage you can point at a player this turn, the land
spells are dead cards and rule #2's step 0 is the whole turn. At "Opponent life: 1" this deck
spent turn 20 casting Lay Waste and Molten Rain on two lands, and turn 22 on a planeswalker, and
lost at 0 to 1."*

## D53-4 (DECIDING SITUATIONS) - the HOLD row
**Finding:** the HOLD row was offered **91** times at this seat and taken **3** (all in 130v125,
s14/s35/s37, where `hold_windows_skipped` then read **14** - the row working as designed).
72 opponent-turn casting asks, 69 plain declines, **0 casts**. The row is free: a board change
re-opens the window.
**AFTER (new bullet):** *"It is the OPPONENT'S turn, you have already answered "Cast nothing right
now" to this exact list, and the last row reads "Hold priority - do not ask me again this turn
unless the board changes" -> take THAT row. It is the same decision said once for the whole turn,
a board change re-opens the window, and you give up nothing. "[you declined this exact list N
times already this turn]" is not new information and is not a reason to cast."*
The CROSS-CHECK bullet under it also gains *"Unless step 0 of rule #2 fires: a row that is lethal
right now outranks the latch."*

## D53-5 (#2 RULE, X-menu STEP 0) - the plan stamp re-keyed (#168)
443 windows at this seat printed `YOUR PLAN (as you last stated it, N windows ago on turn T)`;
**0** printed the unstamped form the guide quoted.
**BEFORE:** *"the screen hands it back to you as "YOUR PLAN (as you last stated it)"."*
**AFTER:** *"the screen hands it back to you as "YOUR PLAN (as you last stated it, N windows ago
on turn T)" - that stamp is how old it is, and an X written before this menu existed is older
than the menu."*

## D53-6 (MULLIGAN) - compression, no rule change
Third corpus with **0 ships, 0 bottom asks** at this seat, and the `would not cover any spell in
it` branch has **0 renders in three corpora** (6 of 6 looks this corpus printed `would cover the
cost of:`; all six kept, all six correct by the header). Per the boundary pass's own condition the
section is #132 UNTESTABLE-AT-THIS-SEAT and the check moves off the guide; the RULE is untouched,
only its anecdote is shortened.
**BEFORE:** *"a one-land seven of Starstorm, Starstorm, Stone Rain, Stone Rain, Lay Waste, Rorix
and a Mountain was shipped this corpus, and the six it bought was four Mountains and two spells -
a hand that acted on turn three."*
**AFTER:** *"a one-land seven of Starstorm, Starstorm, Stone Rain, Stone Rain, Lay Waste, Rorix
and a Mountain ships, and the six it buys acts on turn three."*
