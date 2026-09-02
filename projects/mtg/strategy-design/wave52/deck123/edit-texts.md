# deck123 (Intruders of Thraben) - wave-52 edit texts

Base: live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (61,173 bytes, wave-51
reviewer revisions 5d0c24d28 + boundary pass 6686392d3). Revision: `wave52/deck123/strategy.txt`
(67,217 bytes; +6.0 KB net after five trims).
Corpus: `matchups-20260901-223430`, deck123 seat logs (6 games, 306 decisions, 2 fallbacks).
Every edit below names the seq that paid for it. Model this corpus: qwen36-35b-a3b.

---

## A52-1 - RULE 4 / Tragic Slip: the guide's literal is retired; the decision moved to the cast row
**Finding.** Lane L now prints the Slip's result on every target row. Renders at this seat:
`{right now: -1/-1 - DIES}` x1, `{right now: -1/-1 - SURVIVES (toughness N, -1/-1 more kills it)}`
x4, `{right now: -13/-13 - DIES}` x4. The guide sentence "The Slip's target rows print no
'DIES / SURVIVES'; you read the number" is now FALSE text in a live guide.
**The misplay it did not stop (v146 s18 -> s19, 6 life).** The cast row read
`{right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)}`; the pilot cast
it, and the target ask that followed offered three rows - `Nadaar, Selfless Paladin (4/4) ...
SURVIVES`, `Triumphant Adventurer (2/2) [deathtouch] ... SURVIVES`, `Goblin (2/2) ... SURVIVES` -
with the prompt's own words "this ask has no pass row". The Slip was spent for nothing at 6 life
in a game lost on turn 11. STRATEGY (the surface was complete and correct).

BEFORE (RULE 4):
```
Pick the target with toughness 1 - the SECOND number in the "(P/T)" on the target row. The
Slip's target rows print no "DIES / SURVIVES"; you read the number. A "(4/2)" and a "(2/2)" both
have toughness 2 and both live: this deck wrote "it is 3/1, -1/-1 kills it" at a row reading
"Elite Spellbinder (4/2)", and Slipped a "Siege-Gang Commander #2 (2/2)" the game after; both
Slips were spent and both creatures attacked the next turn.
If several toughness-1 targets are listed, take the one with the biggest power.
```
AFTER: replaced with the DIES/SURVIVES read plus a new paragraph making the CAST row the decision
point ("the target ask that follows it HAS NO PASS ROW"). Full text in strategy.txt.

BEFORE (DECIDING SITUATIONS):
```
- "Cast Tragic Slip {b}" is listed: a target whose "(P/T)" second number is 1, or any target if
  a "died" line sits under this turn's header. A second number of 2 or more and no "died"
  line: "Cast nothing right now" - the target rows print no DIES/SURVIVES for the Slip.
```
AFTER: re-keyed to the Morbid clause on the cast row and the `- DIES` / `- SURVIVES` verdict on
the target row.

## A52-2 - `[from exile]` re-keyed to the lane-K form (guide-literal drift, carry item)
**Finding.** The emitter's actual string at 123v152 s39 is
`Cast Damnation {4}{b}{b} [from exile] {castable from exile - a legal cast, not a dead row; it
costs {2} more than printed, already counted in the cost shown}`. The wave-51 guide cited the
bare `[from exile]` three times. 1 render this corpus; the wave-51 defect it was written for
(4 PLAN lines saying "exiled ... I cannot cast it") did NOT recur - 0 occurrences. PASS.

BEFORE (RULE 0, last paragraph): the "Cast Intruder Alarm {4}{u} [from exile]" story.
AFTER: quotes the full new clause and re-points the anecdote at s39's real failure (the Damnation
was entry 0 and was passed over for a Doomsayer at 4 life).

BEFORE (DECIDING SITUATIONS):
```
- A "Cast <card> ... [from exile]" row (Elite Spellbinder took it): a real cast, two mana more,
  at the card's own cast-list entry.
```
AFTER: quotes the emitted clause verbatim.

## A52-3 - `[already owned:` is retired; three new Tutor brackets (guide-literal drift, carry item)
**Finding.** `[already owned:` renders this corpus: **0**. The lane-K replacements at this seat:
`[finds only an enchantment card - none left in your library]` 9 renders,
`[finds only an enchantment card - every enchantment left in your library is a copy of one you
already control or hold: Intruder Alarm]` 25 renders,
`[finds only an enchantment card - still in your library, not on your battlefield or in your
hand: Intruder Alarm]` 2 renders (the LIVE form).
**Meetings.** The live form was met twice and cast correctly both times (v146 s8 -> s9 found the
Alarm; v125 s22 -> s23 same). The dead middle form was met and CAST once (v126 s46, turn 12, an
Alarm already on the line).
BEFORE / AFTER: the single-bracket paragraph under INTRUDER ALARM AND IDYLLIC TUTOR replaced with
a three-case table; the DECIDING SITUATIONS bullet re-keyed to the same three strings.

## A52-4 - the `[second copy:` bracket reads as permission (4 dead Alarms)
**Finding.** Lane K replaced `[already owned:` on a second Intruder Alarm with
`[second copy: you already control Intruder Alarm; both stay on the battlefield - no legend rule]`
(14 renders at this seat). The new clause answers "is this legal", where the old one answered
"is this dead". The pilot cast a second Alarm under it **4 times**: v126 s74 (t24, 8 life),
v162 s39 (t12), v125 s41 (t20, 15 life), v125 s59 (t28, 6 life). Primitive check
(`mtg.txt:58850`): `auto=lord(creature) doesnotuntap` + two `@movedTo(creature|...):untap all` -
a second copy changes no number on any screen. Classified PERCEPTION-adjacent (a true but
misleading surface) -> a render item is filed in the seat file; the guide edit is the local fix.
ADDED to INTRUDER ALARM AND IDYLLIC TUTOR: "THE SECOND-COPY BRACKET IS NOT PERMISSION" paragraph.

## A52-5 - Damnation entry 0, third corpus running; the PLAN line was written 0/2 times
**Finding.** 2 Damnation windows. v126 s65 (t18): `destroys 5 of their creatures (all of them
carry a restriction against attacking), 0 of yours` on the Perimeter Captain / Pride Guardian /
Overgrown Battlement wall board - CAST, correct by the CHECK-1 carve-out, and the game ended
5 to -108. v152 s39 (t12, **4 life**): `destroys 3 of their creatures (3 without a restriction
against attacking), 0 of yours` = M 0, K 3 = entry 0 exactly; the pilot cast Thraben Doomsayer
and was dead at -10 on turn 13. Neither reply carried the required `Damnation: N <>, K <>, M <>`
plan line (0/2).
EDIT: the entry-0 paragraph in RULE 3 now names three corpora and makes the plan line's absence
the thing to notice.

## A52-6 - an edict into a LIFE-TO-DAMAGE CONVERTER, on a defender (v126 s66)
**Finding.** At 14 life vs 26 the screen printed
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond. While it is in play, life
ITS CONTROLLER gains also makes the other player lose that much life.` Their only creature was
`Overgrown Battlement {1}{g} (0/4) [defender]`. The row read `they control 1 creature - Overgrown
Battlement is sacrificed, they gain 4`. The pilot cast Devour Flesh: 14 -> 10 and 26 -> 30 in one
window (an 8-point swing) for a wall that had never attacked. Two guide rules broken at once
(defender/power-0 = keep the edict; a lifegain-rewarding deck = cast nothing). Primitives
verified: Overgrown Battlement `abilities=defender` 0/4; Sanguine Bond "Whenever you gain life,
target opponent loses that much life".
BEFORE:
```
Both edicts give life to the player who sacrifices. If the OPPONENT'S deck list contains a card
that rewards them for gaining life, "Cast nothing right now" beats either edict at the
opponent.
```
AFTER: quotes the CONVERTER block verbatim and makes the two conditions together a named mistake.

## A52-7 - Lightning Greaves at "of which 0 are creatures", twice more
**Finding.** v146 s15 (t10, 7 life, 2 mana, the Greaves took both); v125 s40 (t20, 15 life, six
sources). Guide entry 5 already forbids it; the DECIDING SITUATIONS bullet gets the counts.

## A52-8 - cleanup discard: the only Damnation went 3 times with a spare on the screen
**Finding.** 8 discard/bottom asks at this seat. Three sent the deck's ONLY Damnation while a
duplicate sat in the same listed hand: v162 s5 (Thraben Doomsayer copy 1 of 2 AND copy 2 of 2 in
hand), v162 s15 (Scrubland copy 1 of 2 AND copy 2 of 2), v130 s45 (Devour Flesh x3 and Bloodline
Keeper x2). The guide's never-list covered the maker and the Alarm only.
EDIT: "NEVER YOUR ONLY DAMNATION" added, and the first step made "scan for `copy 2 of 2`".
(Wave-51 P7 was PASS 4/4 on this rung; it is FAIL 3/8 now.)

## A52-9 - bottoming a mana-producing land (v146 s3)
**Finding.** At (keeping 6), hand = Tundra x2, Marsh Flats, Idyllic Tutor, Lightning Greaves,
Tragic Slip, Bloodline Keeper. The pilot bottomed a Tundra. Marsh Flats makes no mana, so the
keep was a ONE-source six; the Keeper it was built around was never cast and the game ended -2
on turn 11. Lightning Greaves is not on the preference order at all - it was the leftover.
EDIT: a sentence added above "Bottom a way to find or make a creature only after..."

## A52-10 - MULLIGAN: the two-lands floor and the coverage line disagreed on the hand dealt
**Finding (recorded, not routed).** v130 s1, opening seven: `counted by the engine: 2 lands,
5 spells` (two Underground Seas), `Playing every land in this hand would not cover any spell in
it`, hand = Doomsayer x2, Intruder Alarm, Damnation, Bloodline Keeper. STEP 2's "TWO OR MORE
LANDS: keep" says keep; the coverage clause immediately after it invites a ship. The pilot
shipped. s2 (1 land, no coverage) was a correct ship by the guide. s3 (3 lands, coverage naming
four cards) TIMED OUT at 900,000 ms with an empty reply (`fallback: empty_reply`) and the engine
kept at 5. The deck then never cast a spell all game and died on turn 18 at -2.
This is the same shape as wave 51's deck146 mull-to-2 seam: a floor and a qualifier that
disagree on one specific hand. Under the carry's new-baseline rule it is FLAGGED (a repeated
mulligan against a guide floor); the guide edit below removes the seam without moving the floor.
BEFORE:
```
 - TWO OR MORE LANDS: keep. ... Colour is not a reason to ship: read the "Mana
   sources among those lands" and "Playing every land in this hand would cover the cost of:"
   lines, and if either token-maker or Intruder Alarm is on that cover list the hand is fine.
```
AFTER: "keep. Full stop, at every (keeping N). ... NEITHER COLOUR NOR THE COVERAGE LINE IS A
REASON TO SHIP A TWO-LANDER ... The coverage line decides ONE-land hands and nothing else."

## A52-11 - the repeat row: one over-take, and the plan line contradicted its own arithmetic
**Finding.** 5 repeat-row takes. v126 s81 `x20` (M 3 -> 23) and s83 `x11` (M 23 -> **34**, the
stop L+C+3 = 29+2+3 = 34 hit exactly) then pass at s84, then `ATTACK: A1-A34` for -108: perfect.
v162 s31 `x16` (M 4 -> 20) and s32 `x3` (M 20 -> 23) correct; then **s33** with the row reading
`[repeat: you control 23 creatures ...]` and the stop at 25, the reply wrote
`CHOICE: 2 (Create human with Thraben Doomsayer x17)` under a PLAN line that printed
"Stop creating humans at M=40 (L+C+3 = 20+2+3=25)" - the sentence and its own arithmetic
disagree by 15 in the same line.
EDIT: the PLAN shape now carries L and C separately, and a "recompute the stop from THIS
window's row" instruction with the s33 contradiction as its example.

## A52-12 - opponent-turn instants are real now (lane I), and the dead row repeats
**Finding.** 140 casting asks on the opponent's turn at these two seats (wave 51: 0 with a Cast
row in 51 windows). Three casts resolved on an opponent's turn; two were this seat's best plays
of their games (v152 s9 Tragic Slip on Elite Spellbinder in their main 1; v152 s14 Tribute to
Hunger in their upkeep, `they control 1 creature - Wolf is sacrificed, you gain 2`). The cost
side: 36 of the 140 offered ONLY a row carrying `at 0 this does nothing`, re-asked at every
phase step (26 of them in one game, v130 turns 4-10).
EDIT: a paragraph added at the head of RULE 1's Timing block teaching the window and closing the
dead-row loop in one sentence.

---

## Trims paid for the additions (net +5.8 KB, not +7.1)
- **A52-T1** RULE 2's six-window Marsh Flats story compressed; the lane-L clause
  `{this land makes no mana - crack it for a land: your untapped sources go from N to N+1}`
  (69 renders) now carries the reasoning and the rung PASSED this corpus.
- **A52-T2** RULE 3's lone-1/1-Vampire anecdote deleted (duplicate of the K=0 sentence above it).
- **A52-T3** CHECK 1's "Why, in the numbers" paragraph deleted (restates the two lines above it).
- **A52-T4** RULE 0's "CHOICE names a numbered row" anecdote compressed to one clause.
- **A52-T5** MULLIGAN STEP 1 compressed (it restated STEP 2's odds in full).

## A52-4b (post-check addendum)
A DECIDING SITUATIONS bullet was added for the `[second copy:` bracket after a composition check
found the rule lived only in prose - the same seam D52-3 documents at deck130. Final revision
size 67,217 bytes (+6.0 KB net over the live guide).
```
- "Cast Intruder Alarm {2}{u} [second copy: you already control Intruder Alarm; both stay on the
  battlefield - no legend rule]": that bracket says LEGAL, not USEFUL. A second Alarm changes no
  number on any screen - "Cast nothing right now" or a maker instead.
```
