# deck123 (Intruders of Thraben) - wave-50 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (wave-49 revision, 49,946
chars). Thirteen edits, all applied by exact-string replacement on a copy; 49,946 -> 56,413 chars.
Corpus: `matchups-20260827-115759`. Every number is read off a prompt, a reply, a narration line or
a primitive. Files cited by short form; full names in `wave50/seats/seat-123-130.md`.

---

## A50-1 - RULE 1: the CHOICE line's number IS the plan line's answer; x0 / x1 are takes; M is the row's count
**Finding.** Four repeat-row windows whose PLAN line said "pass" under a CHOICE line naming a count:
123v162 seq 35 (`x0` -> `ran 1 time (you named 0)`), 123v130 seq 31 (`x1` under "this window:
pass"), seq 46 (`x3` under "M is 31 now; this window: pass (stop reached)"), 123v126 seq 32 (`x25`
under "M is 33 now. This window: pass." - 58 creatures on a stop of 30). Every reply's PLAN carried
a stop number (8/8, wave 49: 0/15) and the row's new `[you control M creatures right now; ...]`
held the count within 28 of the stop (wave 49: 300). The residual is the answer-first protocol:
the count is written before the subtraction.
**Before**
```
BEFORE you take one, read your own battlefield line - "Your battlefield (K permanents listed, of
which M are creatures)" - which prints in every window. The option line's "[repeat: ... you
control M creatures]" note carries the same M; ...
    When (L + C + 3) - M is 0 or less the repeat row is not yours: CHOICE: 0 (pass). This deck
    did the subtraction once - x19 at M = 3 against a stop of 23 - and then answered x2, x25,
    x50, x120 and x100 ...
```
**After**
```
BEFORE you take one, read M: the repeat row prints it first - "[you control M creatures right
now; ...]" - and your own battlefield line prints the same number after "of which". ...
    THE NUMBER ON YOUR CHOICE LINE IS THE PLAN LINE'S ANSWER, NOT A GUESS WRITTEN BEFORE IT.
    Your reply's first line is the CHOICE and the subtraction comes after it, so do the
    subtraction in your head BEFORE the first line: if the row's "you control M creatures right
    now" is already at or past your stop, the first line is "CHOICE: 0 (pass)" - not "x1",
    not "x3", and never "x0", which the engine carried out ONE time. Four windows this corpus
    wrote "this window: pass" in the PLAN under a CHOICE line that named x0, x1, x3 and x25 ...
```
The stale x2/x25/x50/x120/x100 anecdote (wave 48's game) is cut; the DECIDING repeat-row line gains
"the CHOICE line agrees with the PLAN line's 'this window:' or it is wrong - 'x0' and 'x1' are
takes, not passes". Net +430 chars.

## A50-3 - RULE 1: with the Alarm in hand and castable, the maker waits untapped until the Alarm resolves
**Finding.** 123v152 seq 14 (turn 10, OWN upkeep, Alarm in hand, 4 mana): `Create human with Thraben
Doomsayer` taken; seq 16 Alarm cast in main 1; seq 17-19: the Doomsayer `[tapped]` under the fresh
Alarm through the opponent's whole turn 11 - no token line offered (seq 19's menu: four Marsh
Flats rows); seq 21 Brutal Cathar exiled it; dead turn 12 at -10. Primitive: Intruder Alarm
`lord(creature) doesnotuntap` + `@movedTo(creature|...):untap all(creature)` - a tapped creature
untaps only when a creature enters.
**After** (new paragraph before "YOUR OWN UPKEEP IS THE WORST WINDOW OF ALL")
```
WITH THE ALARM IN HAND AND CASTABLE THIS TURN, THE MAKER WAITS UNTAPPED UNTIL THE ALARM HAS
RESOLVED. Under Intruder Alarm a tapped creature untaps only when a creature ENTERS; nothing
of theirs may enter for a whole turn. This deck tapped its Doomsayer in its own upkeep, cast
the Alarm in the main phase after it, and then went through the opponent's entire next turn
with no "Create human" line on any screen ... Land, Alarm, THEN the first tap ...
```
Plus a DECIDING line. +720 chars.

## A50-4 - RULE 3 + entry 0 + DECIDING: the K string re-keyed (lane X); walls exception; N = 0 never; entry 0 outranks Tutor/Alarm
**Finding.** (a) #128: the guide quoted `(K able to attack)`; the emitter now prints `(K without a
restriction against attacking)` on the pilot's turn, `(K able to attack right now)` on theirs, and
`(all of them carry a restriction against attacking)` when every creature is a wall (123v126 seq
17). `(0 able to attack)` appears 0 times. (b) 123v126 seq 17: Damnation cast at K 0 on two
Perimeter Captains + a Battlement with `0 of yours` - RULE 3's K-0 hold broken, and right: CHECK 1
forbids attacking into those names, the sweep is the removal; won -208. (c) 123v125 seq 130, 141,
144: Damnation cast on `destroys 0 of their creatures ... 0 of yours` three times ("clear board
(currently empty)"). (d) 123v146 seq 12 (13 life): `destroys 2 of their creatures (2 without a
restriction against attacking), 0 of yours` -> Idyllic Tutor; seq 15 (9 life): `destroys 4 ... 0
of yours` -> Intruder Alarm; seq 20 (5 life) Damnation. Eight life for two windows.
**After** (RULE 3 gains, after the row-count paragraph)
```
... "(K without a restriction against attacking), M of yours}" on your turn and "(K able to
attack right now)" on theirs; when every one of theirs is a wall it prints "(all of them carry
a restriction against attacking)" instead, and that is K = 0. ...
THE ONE K = 0 BOARD YOU DO SWEEP: walls named PERIMETER CAPTAIN or PRIDE GUARDIAN. CHECK 1 in
COMBAT forbids you to attack while they stand, and Damnation is the removal ...
"DESTROYS 0 OF THEIR CREATURES" IS A CARD THROWN AWAY. This deck cast Damnation three times
in one game on rows reading "destroys 0 of their creatures ... 0 of yours" ...
ENTRY 0 HAS NO EXCEPTIONS FOR YOUR OWN CARDS. At 13 life this deck read "destroys 2 of their
creatures (2 without a restriction against attacking), 0 of yours" and cast Idyllic Tutor ...
```
Entry 0 re-keyed to the new K string with "not the Tutor, not the Alarm"; the DECIDING Damnation
line re-keyed and gains the wall exception and "N at 0 - never". +1,450 chars.

## A50-5 - cast-list entry 4: read both lines; a second Alarm is entry nothing
**Finding.** 123v125 seq 118 and 139: Idyllic Tutor cast with Intruder Alarm on the battlefield,
plan "Cast Idyllic Tutor to find Intruder Alarm (already on battlefield, but must cast it to dig for
Bloodline Keeper)" - the guide's own quoted misbelief, third corpus (#126: the failing window is
the guide's worked example -> re-key to the printed line, and the render half is seat item M2).
seq 156: a second Intruder Alarm cast with one on the line (#125: a copy of a name on your line is
entry nothing).
**After** (entry 4 gains)
```
    Read BOTH lines before you take it. "Cast Idyllic Tutor to find Intruder Alarm (already on
    battlefield, but must cast it to dig for Bloodline Keeper)" is a sentence this deck wrote
    this corpus with the Alarm on its line, and the reveal list marked every creature "[does
    NOT qualify]" again. A second "Cast Intruder Alarm" with one on your line is entry
    nothing: it does nothing the first one is not already doing.
```
+430 chars.

## A50-7 - CHECK 2 Lightmine Field: the answer as the ATTACK line
**Finding.** 123v125 seq 39 (`ATTACK: Vampire, Vampire` into Lightmine Field, both 2/2s died) and seq
63 (same, under a PLAN reading "Both Vampires have 2 toughness, so they will die. This is a bad
trade" - `parse_note: attack_last_line_taken`). CHECK 2 existed with the arithmetic; the reply line
was never stated (#126).
**After** (CHECK 2 gains)
```
With 2/2 Vampires that is ONE: the line you write is "ATTACK: Vampire #1" and nothing after
the name. This deck wrote "ATTACK: Vampire, Vampire" under a plan that said "both Vampires
die - this is a bad trade", twice in one game, and lost four tokens to a field its own plan
had priced.
```
Plus a DECIDING line. +330 chars.

## A50-8 - RULE 0: the land that enters untapped when a spell is castable off it
**Finding.** 123v162 seq 6 (turn 6, two lands out, `Arcane Sanctum`, `Marsh Flats`, `Underground
Sea` in hand, Intruder Alarm {2}{u} in hand): `Play Arcane Sanctum` (enters tapped); seq 7-8 `Cast
nothing right now` twice with `Mana available: 2`. An Underground Sea would have been the third
untapped source and the Alarm the cast. Primitive text on the row: "Arcane Sanctum enters tapped".
**After** (RULE 0 gains)
```
WHICH LAND: when a spell in your hand is castable THIS turn off the land you play, play the
land that enters UNTAPPED - Underground Sea, Tundra, Scrubland, Plains, Swamp. Arcane Sanctum
"enters tapped" (its own text says so) and Isolated Chapel enters tapped unless you control a
Plains or Swamp: play those on a turn you were not going to spend all your mana. ...
```
Plus a DECIDING line. +560 chars.

## A50-9 - Vision Skeins: A49-8 shrunk and re-keyed to the cleanup ask (lane W)
**Finding.** A49-8's "at N of 5 or fewer" failed 3/3 (123v152 seq 6 hand 6, 123v162 seq 4 hand 7,
123v126 seq 4 hand 7 - every one a turn-3/4 "dig"). The harm it guarded against is now a decision:
lane W's cleanup ask (`Cleanup step (CR 514.1): your hand has 8 cards ... PUT:`) and the pilot chose
Intruder Alarm copy 1 of 2, Damnation copy 1 of 2, Vision Skeins - sane picks, 7/7 this corpus.
#118 (recurs -> shrink, re-key) and #127 (the mitigation is written as a fact about the cards).
**Before**
```
  COUNT "Your hand (N cards)" FIRST. Your hand goes back to seven at the end of your turn, and
  the engine - not you - chooses what you discard: at N of 7 on turn 3 this deck cast Skeins,
  could cast nothing it drew, and the engine discarded Intruder Alarm. Cast the Skeins at N of 5
  or fewer, or on a turn with the mana to cast what it draws.
```
**After**
```
  COUNT "Your hand (N cards)" FIRST. Your hand goes back to seven at the end of your turn:
  at N of 6 or more a Skeins you cannot follow with a cast ends in a "Cleanup step" ask that
  makes you discard. Three turn-3 Skeins this corpus ended that way. If you cast it anyway,
  answer that ask by naming the SPARE - the second copy of a card ("copy 2 of 2"), a Devour
  Flesh at "they control 0 creatures", a Damnation with a second in hand - never your only
  maker and never your only Intruder Alarm. And an edict at "they control 1 creature" on the
  same menu is the cast before the Skeins (Rule 5).
```
The stale "the engine - not you - chooses" sentence (false since lane W) is gone. Plus a DECIDING
line for the cleanup ask. Net +330 chars.

## A50-10 - RULE 5: "not hurting you" narrowed to [defender] / power 0; a 1-power engine is a target
**Finding.** 123v152 seq 6 (turn 4, 2 mana): `Cast Devour Flesh {right now: they control 1
creature - Katilda, Dawnhart Prime is sacrificed, they gain 1 - the sacrificing player gains, not
you}` -> `Cast Vision Skeins`. RULE 5 said "N = 1 is the cast" and "power 0 or 1 - keep the edict";
the two sentences disagreed on this hand and the pilot held. Primitive: Katilda `lord(creature
[human&...]|myBattlefield) {T}:add{...}` + `{4}{G}{W}{T}: counter(1/1) all(creature|
mybattlefield)`; her line read (2/2) by turn 8 and (3/3) by turn 10. Contrast 123v126 seq 21: N = 1
on Perimeter Captain (0/4 defender, they gain 4) - declined, right.
**Before**
```
Do not spend an edict on a creature that is not hurting you. If their only creature is a
defender, or has power 0 or 1, or its line says it cannot attack, keep the edict ...
```
**After**
```
Do not spend an edict on a creature that is not hurting you: if their only creature reads
"[defender]" or has power 0, keep the edict and take another action ... A 1-power creature is
NOT in that class when its "{card text:}" gives the rest of their board something - mana,
counters, +1/+1 - it is the engine and it grows: this deck answered "Cast nothing" to "they
control 1 creature - Katilda, Dawnhart Prime is sacrificed, they gain 1" on turn 3 ...
```
Plus a DECIDING line. +520 chars.

## A50-11 - CHECK 5: the lone maker's own tag decides
**Finding.** 123v130 seq 34: `A1. Thraben Doomsayer (2/2) ... [their untapped blockers: Siege-Gang
Commander (2/2) (both die); ...]`, twenty Humans summoning sick behind it -> `ATTACK: Thraben
Doomsayer`; the Doomsayer and the Commander died. The hold-back existed without its trigger string.
**After** (CHECK 5's hold-back gains)
```
Its own row says when: "[their untapped blockers: ... (both die)]" or "(your attacker dies)"
on the maker's line means "ATTACK: none". This deck sent a lone Doomsayer into "(both die)"
with twenty Humans summoning sick behind it and traded the engine for a Siege-Gang Commander.
```
Plus a DECIDING line. +330 chars.

## A50-12 - RULE -1: the (keeping 6) line deferred to the comparison (#131)
**Finding.** The twelve-hand self-check: a one-land no-coverage hand at (keeping 6) read "keep
unless zero lands or all lands" in RULE -1 and "ONE LAND ... does nothing for four turns and
ships" in the MULLIGAN comparison that STEP 2 routes (keeping 6) to. Two sentences, two verdicts.
No corpus window (the two sixes kept held 5 and 3 lands).
**Before**
```
  - M is 6: keep unless the hand is zero lands or all lands.
  - M is 7 (your untouched opening seven): the comparison in the MULLIGAN section applies.
```
**After**
```
  - M is 6 or 7 (your opening seven, or the first look after it): the comparison in the
    MULLIGAN section applies - a one-land seven with "would not cover any spell in it" ships at
    (keeping 6) exactly as it does at 7; two lands is a keep at either.
```
+110 chars. Odds-based, no floor.

## A50-13 - BOTTOMING: five lands and two spells at N of 6
**Finding.** 123v152 seq 3: hand Tundra, Scrubland, Devour Flesh, Marsh Flats, Tundra, Arcane
Sanctum, Devour Flesh (5 lands, 2 spells) at N of 6 -> `PUT: 3` (Devour Flesh). The order's item 1
("three or four lands and the spells they cover, up to FOUR mana") already said the fifth land goes.
**After** (BOTTOMING gains)
```
Five lands and two Devour Flesh at N of 6: the fifth land goes under, not a Devour - four
lands cover everything this deck casts. This deck bottomed the Devour.
```
+170 chars.

---

## Not edited, recorded
- **Own-upkeep chain firing** (A49-2 / P2): 123v130 seq 23-28 (turn 15 YOUR) and 123v126 seq 26-32
  (turn 16 YOUR) fired in the pilot's own upkeep; both games were won (no sweeper on the other
  side), while the one chain fired on the opponent's turn (123v162) lost to a draw step. The rule
  keeps its wave-49 evidence; no restatement (#118 needs a loss to re-key on).
- **Marsh Flats never fetched** at 123v152 (seq 17-25) and 123v162 (seq 14-41): no window where the
  missing land cost a cast was found; RULE 2 stays about WHICH dual.
- **Tribute at N = 4 with the opponent at 1** (123v130 seq 54): harmless, a Goblin and +1.
- **Blockers P9 shape**: no window.
- **The v162 draw-step death** (Howling Mine x2 + Puzzle Box + Underworld Dreams + Fate Unraveler):
  no decision after seq 30 could change it; the guide's "do not cast Vision Skeins" line for that
  board held (0 Skeins after the Mines landed).
