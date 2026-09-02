# deck123 (Intruders of Thraben) - wave-53 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (wave-52 reviewer
revision `e68b7c7c6` + boundary pass `2d83e169f`), 67,302 bytes -> 70,230 bytes (**+2,928**),
ten edits (nine additions, one cut). Corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260902-023342/`; every seq below is a deck123 seat log.
ASCII only; no non-`[ -~\t]` byte in the file.

---

## A53-1 (RULE 2, WHICH LAND) - the anti-land-destruction basic carve-out cannot fix blue
**Finding:** 123v130 s36 (turn 13, 10 life). Hand held **two Intruder Alarms {2}{u}**; the mana
line read "colours you can make: {b}{w}"; the Marsh Flats menu listed seven rows, **6 "targeting
Tundra"** and **7 "targeting Underground Sea"** (Tundra `subtype=Plains Island`, Underground Sea
`subtype=Island Swamp`, both fetchable by Marsh Flats' `target(*[swamp;plains]|myLibrary)` and
both making {U}). The pilot answered **5 "targeting Swamp"**, cast neither Alarm for the rest of
the game and died at 1 life with both in hand (s64-s66). Its own reply names the problem
("I need a source that makes {U}") and takes the Swamp anyway - the sentence it was following is
the guide's own deck130 carve-out. Same shape, cheaper, twice more: 123v125 s8 (Swamp over
Scrubland with {W}/{B} already doubled) and 123v146 s9 (Plains against a deck with no land
destruction). Basic-over-dual **3 of 8 fetches**.
**BEFORE:** *"A basic only against a deck whose log shows Molten Rain, Lay Waste or Dwarven
Blastminer (deck130): those three hit NONBASIC lands only, so a Plains or Swamp fetched for the
colour you are short of is the land they cannot take - keep the Sea for the Alarm's blue."*
**AFTER:** *"A basic only against a deck whose log shows Molten Rain, Lay Waste or Dwarven
Blastminer (deck130), and only when the colour you are short of is {W} or {B}: those three spells
hit NONBASIC lands only. THE CARVE-OUT STOPS AT BLUE - a Plains and a Swamp make no {U}, so when
{U} is the pip missing from "colours you can make", the fetch is "targeting Tundra" or "targeting
Underground Sea" whatever their deck destroys: a land they blow up next turn still casts your
spell this turn. At 10 life against deck130, holding TWO Intruder Alarms with no {U} on the mana
line, this deck answered "targeting Swamp" over those two rows and died at 1 life with both
Alarms in hand."*

## A53-2 (RULE 4) - the Slip test re-keyed to lane O's kill count (#168)
**Finding:** 123v130 s64 (turn 18, 1 life). Cast row read
`Cast Tragic Slip {b} {right now: -1/-1 (no creature has died this turn, so Morbid does NOT
apply)} {leaves 1 of your 2 untapped mana sources untapped} {kills 0 of the 3 legal targets at
-1/-1}` - taken; s65's target ask then printed `- SURVIVES` on all three rows. The old sentence
asked the pilot to derive from a name list what the row now states.
**BEFORE:** *"So before you take the cast row, read the Morbid clause on it: ... plus a
legal-target list whose creatures all have toughness 2 or more is "Cast nothing right now"."*
**AFTER:** *"So before you take the cast row, read the KILL COUNT on it. "{kills 0 of the 3 legal
targets at -1/-1}" means every name on that row's list lives through the spell - the engine has
done the toughness arithmetic for you on every target at once, and that row is "Cast nothing
right now". A row naming a victim instead - "{kills: Elite Spellbinder}" - is the cast. At 1 life
this deck cast the Slip off a "{kills 0 of the 3 legal targets}" row and spent its last card on a
6/5 that survived."*

## A53-3 (RULE -1) - the PLAN header's age stamp, and what it settles (#168)
**Finding:** 123v130 s57 wrote a PLAN whose captured text ran to ~2,000 characters of
deliberation, including "Vampire died. So Morbid applies ... Rorix ... dies". The engine handed
that whole block back at s63 and s64 under `YOUR PLAN (as you last stated it, 1 window ago on
turn 17)`, beside a row saying Morbid does NOT apply. The pilot believed the plan (s64's own PLAN:
"Kill Rorix Bladewing with Tragic Slip (-13/-13 due to Morbid)") and threw the card away.
515 windows at this seat carried the stamped header; **0** carried the old unstamped form.
**BEFORE:** *"the ask shows you "YOUR PLAN (as you last stated it)", and the plan it shows is the
one you wrote about the hand you already threw away."*
**AFTER:** *"the ask shows you "YOUR PLAN (as you last stated it, N windows ago on turn T)" - the
stamp is the point: it is your own older text, about the hand you threw away, not a fact about
this one. THAT HOLDS IN EVERY LATER WINDOW TOO. Where a carried plan and a "{right now: ...}"
clause disagree, the clause is now and the plan is then: this deck was handed back "Morbid
applies, Rorix dies" beside a row reading "(no creature has died this turn, so Morbid does NOT
apply)" and "{kills 0 of the 3 legal targets}", believed the plan, and threw the card away at 1
life."*

## A53-4 (RULE 1 prose) - the A52-12 paragraph re-keyed to the HOLD row (B9/D2 obligation)
**Finding:** the HOLD row shipped and renders as the LAST row of every opponent-turn casting ask
(**249 offered rows** at this seat). It was taken **once in 240 opponent-turn asks** (123v125 s66).
In 123v125 alone, **110 of 128** opponent-turn asks sat inside runs of three or more consecutive
byte-identical declined menus (runs of 15, 13x4, 12, 11, 9, 8, 3).
**BEFORE:** *"The same ask reappears at Upkeep, ... with the same rows: when the only row reads
"{right now: they control 0 creatures - at 0 this does nothing}", the answer is "Cast nothing
right now" and it stays that answer for the whole turn - do not re-litigate it eleven times."*
**AFTER:** *"... with the same rows, and the LAST row of every one of them now says how to stop
that: "Hold priority - do not ask me again this turn unless the board changes". When the only cast
row is dead - "{right now: they control 0 creatures - at 0 this does nothing}", or "{kills 0 of
the N legal targets}" - decline once and take the Hold row on the very next ask of that turn. The
board changing brings the window straight back, so holding costs you no window you would have
used."*
Plus the matching DECIDING SITUATIONS bullet (new, cites the s63 -> s64 flip: the pilot declined
the dead Slip row, was shown `[you declined this exact list 1 time already this turn]`, and cast
it on the next window).

## A53-5 (RULE 3) - N = 0 is still N = 0 in your second main phase
**Finding:** 123v125 s83/s84 (turn 20, 15 life). s83 main 1: row
`Cast Damnation {2}{b}{b} {right now: destroys 0 of their creatures (0 without a restriction
against attacking), 0 of yours}`; the reply's coded line said "Cast Damnation", the prose
reversed it ("Casting Damnation is a waste of mana"), a second coded line landed and the engine
executed `Cast nothing right now` (`answer_replaced`). At s84, main 2, same board, same row -
cast. One of two Damnations, gone; the game ended 0 to 30 with no creature ever resolving.
**BEFORE:** *"...an empty board needs no clearing and the sweeper was not there when it mattered.
At N = 0 the Damnation stays in hand, whatever the plan says."*
**AFTER:** *"... At N = 0 the Damnation stays in hand, whatever the plan says - and your SECOND
main phase is the same board: this deck read "destroys 0 of their creatures ... 0 of yours",
answered "Cast nothing right now" in main phase 1, and cast that same Damnation off that same row
in main phase 2 of the same turn."*

## A53-6 (DECIDING SITUATIONS, new bullet) - the blockers hint names its own condition
**Finding:** 123v130 s55 (turn 16, **7 life vs 12**). The prompt printed
`Your life: 7. Unblocked, these attackers deal up to 6 - you would be at 1 - NOT lethal: block
only where the trade favors you; taking damage while ahead on LIFE is often correct (your
strategy guide's blocking rules override this general hint).` with `A1. Rorix Bladewing (6/5)`
and `B1. Vampire #2 (2/2) [flying] - may block A1 (your blocker dies, attacker lives)`.
`BLOCKS: none`. At 1 life the pilot then had two live Intruder Alarms and no {U}; it died on the
next swing. The guide had no blockers bullet for a non-lethal window.
**AFTER (new bullet):** *"The blockers prompt says "you would be at K - NOT lethal: block only
where the trade favors you; taking damage while ahead on LIFE is often correct": that hint names
its own condition - read it against the two life totals above it. While THEIR life is higher than
yours you are not the player it describes: a token goes in front of the biggest attacker, because
a 2/2 you can remake is not a cost and K is the life your combo has to live on next turn. At 7
against 12 this deck answered "BLOCKS: none" to a lone 6/5 with a Vampire untapped, went to 1,
and lost with the Intruder Alarm still in hand."*

## A53-7 (MULLIGAN, ONE LAND branch) - the (keeping 6) gap the boundary pass recorded
**Finding:** boundary-pass obligation 7 (the one-land / no-coverage hand was decided at
(keeping 7) and at (keeping 4), and silent at (keeping 6), while RULE -1 already said it ships at
(keeping 6)). No mulligan arose this corpus (6 looks, 6 keeps), so this closes a #131
self-consistency gap, not a corpus finding.
**BEFORE:** *""would not cover any spell in it" ships on the untouched seven: nothing happens
until a draw."*
**AFTER:** *""would not cover any spell in it" ships on the untouched seven AND at "(keeping 6)":
nothing happens until a draw, and one card fewer does not change that."*

## A53-8 (DECIDING SITUATIONS) - CUT, the duplicated mulligan bullet
Two bullets stated the same (keeping 5) rule in different words. The second - *"The mulligan
ask's price line says "(keeping 5)" or fewer and the hand holds a land: keep it; only a zero-land
hand buys a fresh look, at the price of one more card."* - is deleted; the first (which also
carries the carried-plan clause) stands.

## A53-9 (RULE 5, step 2) - "they choose which one" is the row saying N >= 2
**Finding:** 123v130, the lost game, three casts on that clause: **s27** (turn 12, 17 life)
`Cast Tribute to Hunger {2}{b} {right now: they control 3 creatures - they choose which one}`;
**s38** (turn 13, 9 life) the same row again; **s53/s54** (turn 16, 7 life)
`Cast Devour Flesh {1}{b} {right now: they control 2 creatures - they choose which one}` aimed at
`The opponent (player, life 11)`, who kept Rorix Bladewing and went to 12. Wave 52 had **0 such
casts in 33 windows**; this is a regression, and all three were in the game this deck lost at
-4. 238 dead edict rows were correctly declined in the same corpus, so the rung is not broken in
general - only the "they choose which one" branch of it.
**BEFORE:** *"(2) If N is not exactly 1, do not cast an edict - not at 0, not at 2 or more."*
**AFTER:** *"(2) If N is not exactly 1, do not cast an edict - not at 0, not at 2 or more.
"- they choose which one" is the row telling you N is 2 or more and that the body you want is the
one they will keep: this deck cast Tribute to Hunger twice and Devour Flesh once off that exact
clause in one game, against a board whose Rorix Bladewing was still there afterwards every time,
and lost it."*

---

**Totals:** nine additions and one cut (A53-1..A53-9 plus the A53-8 cut), `deck123_strategy.txt` 67,302 -> **70,230** bytes
(**+2,928**). Every edit cites at least one seq from this corpus except A53-7, which closes the
boundary pass's recorded #131 gap.
