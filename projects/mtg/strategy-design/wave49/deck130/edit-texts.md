# deck130 (Budde's Ponza) - wave-49 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (wave-48 revision, 36,845
chars). Seven edits. 36,845 -> 41,527 chars. Corpus: `matchups-20260827-094106`. Deck facts: 24
lands (the guide's own count), all producing {R}; primitives checked: Pyrite Spellbomb (`{R}{S}:
Damage:2 target(anytarget)` / `{1}{S}:draw:1`), Hammer of Bogardan, Siege-Gang Commander, Dwarven
Blastminer, Starstorm (from the option rows' `{card text:}`, which matched).

---

## D49-1 - MULLIGAN: the floors go; odds at every look; the bottoming order is satisfiable at any N (#119, boundary B2)
**Finding.** The live section opened "NEVER go below 6 cards ... Mulligan only a hand with zero
lands, or a hand that is all lands" and closed the bottoming with "never a land down to fewer than
three" - the floor text #119 retires. `...deck130-0x55cfc55149b0-vs-...deck123.jsonl` seq 1: a
one-land seven (`counted by the engine: 1 land, 6 spells`; `would not cover any spell in it`;
Starstorm x2, Stone Rain x2, Lay Waste, Rorix, Mountain) - the pilot shipped it, against the floor
and with the coverage rule, and the six it bought (five Mountains, Lay Waste, Molten Rain; bottomed
a Mountain) acted on turn 3. The two rules contradicted each other with no precedence (#97). Odds:
24/60 lands; a fresh seven holds 2+ lands ~86% (1 - C(36,7)/C(60,7) - 24*C(36,6)/C(60,7)).
**Before**: the three paragraphs quoted above.
**After**
```
THE ODDS, ONCE: twenty-four lands in sixty, every one of them making {R} ... A fresh seven from
this deck holds two or more lands about six times in seven; each mulligan hands you those same
odds with one card fewer, so a ship buys a look, never a fix.
READ TWO LINES BEFORE YOU ANSWER. ... "counted by the engine: N lands, M spells" ... "Playing
every land in this hand would cover the cost of: ..." ... a one-land seven of Starstorm,
Starstorm, Stone Rain, Stone Rain, Lay Waste, Rorix and a Mountain was shipped this corpus, and
the six it bought was four Mountains and two spells - a hand that acted on turn three.
AT EACH LOOK, price it: the ask prints "(keeping N)". A hand with two or more lands is a keep
at every N. A one-lander is a keep when the coverage line names a card, and a ship when it
does not - at (keeping 6) or (keeping 5) ... at (keeping 4) or lower even a one-lander that
acts by turn three beats the look you would buy. Zero lands, or all lands, ships at any N above 4.
THE BOTTOMING ASK IS WHERE THE HAND IS BUILT. ... PREFERENCE ORDER, which is satisfiable at any
N because it is an order and not a set of floors: the lands that cover the cheapest spells you
keep ...; then the one Starstorm ...; then your cheapest spells; then the rest by cost. ...
Whichever card is last in that order is the one that goes, even when it is a land or the Starstorm.
```
The two DECIDING mulligan lines re-written to match (no "never below 6", no "never fewer than
three"). Net +900 chars.

## D49-2 - Hammer of Bogardan: the upkeep return with a body in hand is a pass
**Finding.** `...deck130-0x562d3ed7c8d0-vs-...deck146.jsonl` seq 22 (turn 9, 6 mana, Siege-Gang
Commander in hand): `Put a card into hand with Hammer of Bogardan [cost: {2}{r}{r}{r}]` taken in
upkeep, 1 mana left, no Commander; seq 28 (turn 13, 6 mana, Commander in hand): taken again; between
them seq 24-27 cast two Hammers at Nadaar, the first from a target row already reading `{right now:
takes 3 damage - SURVIVES (toughness 4)}`. The Commander was never cast; dead turn 14. The guide's
"Never let it push a Siege-Gang ... out of the turn" was in the card paragraph, not keyed to the
upkeep row where the decision is made (#89 row-first; #112 the turn is decided before combat).
**After** (HAMMER paragraph gains)
```
THE RETURN IS ASKED IN YOUR UPKEEP, before your main-phase menu exists - so read "Your hand" on
that upkeep screen before you answer it. Siege-Gang Commander or Rorix Bladewing in hand, and
lands on your line enough to cast it this turn: answer "CHOICE: 0 (pass)" to the return ...
```
Plus a DECIDING line keyed to the row. +640 chars.

## D49-3 - Cast-order entry 1: Starstorm goes FIRST in the turn; a token-maker on their line is the target
**Finding.** `...deck130-0x55cfc55149b0-vs-...deck123.jsonl` seq 22-29, turn 11, 6 mana, their
line `Thraben Doomsayer, Human x2` + Intruder Alarm: the Starstorm row read `At X=4 - kills THEIRS:
Thraben Doomsayer, Human x2; YOURS: Dwarven Blastminer`. The pilot cast Molten Rain first (entry 3)
and then Starstorm at `max affordable X=1` (two Humans + own Blastminer); the Doomsayer lived and
by turn 13 had made 25 Humans; dead turn 14 to 26 attackers. Entry 1 outranked entry 3 and was
taken second, at a shrunken X (#70 rank, the same fact twice).
**After** (entry 1 gains)
```
    AND IT GOES FIRST IN THE TURN: the X you can afford shrinks with every spell you cast before
    it. With "Thraben Doomsayer, Human x2" on their line and Intruder Alarm beside them, at six
    mana, this deck cast Molten Rain first and then Starstorm at X=1 ... A token-MAKER on their
    line (Thraben Doomsayer 2/2, Bloodline Keeper 3/3) is the creature the sweep is for ...
```
Plus a DECIDING line. +720 chars.

## D49-4 - SURVIVAL FIRST: a sweep that leaves the killer standing is not survival
**Finding.** `...deck130-0x55723bab0220-vs-...deck152.jsonl` seq 17-19, turn 9, 4 life, their line
Moonrage Brute 6/6 + Luminarch Aspirant 2/2 (8 power): the only kill rows read `X=3 {kills THEIRS:
Luminarch Aspirant; YOURS: Dwarven Blastminer}` / `X=2 {same kills}`. The pilot cast X=2 (plan: "kills
the Brute (6 toughness)" - the row said otherwise), lost its only blocker, and died at -2 to the 6/6.
D48-3's SURVIVAL FIRST said "take the X whose THEIRS list is longest, WHATEVER the YOURS list names"
- correct when the sweep removes the lethal, wrong when it does not (#71's carve-out needs its own
condition).
**After** (inserted under SURVIVAL FIRST)
```
     BUT A SWEEP THAT LEAVES THE KILLER STANDING IS NOT SURVIVAL. Subtract the THEIRS list from
     their line: if the creatures the sweep does NOT kill still total your life or more, the
     Starstorm buys you nothing and costs you the blocker on its YOURS list. At 4 life against a
     6/6 and a 2/2 ... A 1/1 in front of the 6/6 was the extra turn. Keep the blocker.
```
Plus a DECIDING line. +640 chars.

## D49-5 - Rule #2: a row that names your own permanent is never the answer
**Finding.** Two self-hits. `...vs-...deck146.jsonl` seq 16 (turn 5 main 2): the window's only row
`1. Deal 2 damage with Pyrite Spellbomb targeting Dwarven Blastminer [your battlefield]` (plus `0.
Pass priority`); reply `CHOICE: 1`, plan "Activate Dwarven Blastminer to destroy Brightclimb
Pathway" - narration `Your Pyrite Spellbomb dealt 2 damage to Dwarven Blastminer / Your Dwarven
Blastminer died`. `...deck130-0x55a0f8e4de00-vs-...deck126.jsonl` seq 38 (turn 15): a window listing
only `Destroy with Dwarven Blastminer targeting Forgotten Cave #1/#2 [your battlefield]` + cycling;
reply took Cave #1 (plan "Destroy Forest with Blastminer") - `Your battlefield` 8 -> 7 at seq 39.
Same belief both times: the plan's target is executed by the nearest row (#106). The Blastminer
paragraph already said "never a row tagged [your battlefield]"; the Spellbomb/Hammer/Siege-Gang rows
had no such line, and neither had the all-rows-are-yours case.
**After** (new paragraph in rule #2, above CROSS-CHECK)
```
A ROW THAT NAMES YOUR OWN PERMANENT IS NEVER THE ANSWER. ... A row tagged [your battlefield]
on Pyrite Spellbomb, Hammer of Bogardan, Siege-Gang Commander or Dwarven Blastminer aims at
YOU, and when it is the only row on the window the answer is "CHOICE: 0 (pass)". ... A plan that
names a target the screen does not list is carried out by 0 (pass), not by the nearest-looking row.
```
Plus a DECIDING line. The engine half (why the window listed only that row; suppress or tag
self-only target rows) is seat item H3. +900 chars.

## D49-6 - Rule #2 point 4: the "chip away" sentence, refuted in its own numbers
**Finding.** Spellbomb at the face at 20 (`...vs-...deck162.jsonl` seq 9, plan "start chipping away
at their life total"), at 20 and at 26 (`...vs-...deck125.jsonl` seq 16, 35); Hammer at the face at 17
(`...vs-...deck126.jsonl` seq 59-60, 0 creatures on their line - the rule's exception). None decisive;
the 6-or-less rule stood in the text and the belief was never named (#106).
**After** (point 4 gains)
```
     "Start chipping away at their life total" is the sentence that spends it early: 2 off 20
     is not a start, it is the Spellbomb that would have killed their first 2-toughness
     creature. Three such casts this corpus, at 20, 20 and 26, changed nothing in any of those games.
```
+300 chars.

## D49-7 - DECIDING: land destruction with mana that would otherwise go unspent
**Finding.** `...deck130-0x55b7f7614d40-vs-...deck162.jsonl` seq 24 and 26 (turn 11, 5 mana, their
line 3 lands, `Cast Molten Rain` listed, Rorix {3}{r}{r}{r} in hand uncastable): `Cast nothing right
now` twice, plan "Cast Rorix Bladewing in Main Phase 2" (six mana never arrived that turn). Those
three lands cast Teferi's Puzzle Box and Master of the Feast; the draw steps took 18 -> 5 -> dead by
turn 14. Entry 3 was skipped for an entry 4 that was not castable.
**After** (new DECIDING line)
```
- Their board shows 4 or fewer lands, a land-destruction spell is listed, and nothing above it
  in the cast order is castable THIS turn (a body you cannot afford yet is not castable) ->
  cast it now. "Hold mana for Rorix next turn" left five mana unspent against three opposing
  lands, twice in one game, and those lands cast the draw engines that ended it.
```
+380 chars.

---

## Not edited, recorded
- **Blocking (P9)**: `...vs-...deck123.jsonl` seq 31 put three Goblins on one 2/2 under a LETHAL
  header (27 incoming at 18 life; no assignment survived it). The one-blocker-per-attacker rule
  exists; second corpus of the same shape, 0 decisive - recorded, not re-taught (#45).
- **Blastminer gate (P15)**: 4 activations; 3 inside the gate (vs123 seq 17 at 19 life / 3 lands;
  vs152 seq 13 at 12 / 4; vs126 seq 27 at 20 / 3), 1 at its own land (D49-5).
- **X announce (P12)**: 3 menus; the marked or lowest-of-run row taken 3/3 (vs123 seq 29 X=1 marked;
  vs152 seq 19 X=2 "same kills as X=3"; vs126 seq 52 X=4 "same kills as X=5").
- **Face Hammer under Elixir** (vs125 seq 87-123): the loop was the only clock the pilot had and the
  opponent's Elixir out-gained it; the CROSS-CHECK rule ("life went UP -> aim nothing at the face")
  was violated ten times without a better row on any menu - #109 says that is evidence against the
  absolute, but no edit: the games where it matters are the ones with a creature to aim at.
