# deck130 - wave-57 guide edits, before -> after

Start point: the LIVE guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (67,966 bytes).
Result: `wave57/deck130/strategy.txt`, **69,606 bytes (+1,640)**: five additions against eleven
trims. Band 41-71 KB, measured.

All counts are from the RENDERED `prompt` of my six deck130 seat logs, never from `options`.

---

## D57-1 (RULE #2, CROSS-CHECK) - the latch reads the printed trend line; the face decision is at the CAST row
**Finding, and it is one turn.** `130v125` turn 38, 20 life, opponent battlefield
`(5 permanents listed, of which 0 are creatures, ... and 5 are lands)`:
- s88/s90 sacrificed a Pyrite Spellbomb into the face at their 28;
- s91 cast Hammer of Bogardan over its own cast-row clause `{no creature target - and 3 to the
  opponent at life 26 leaves them at 23}`, under `PLAN: ... 3 damage is not lethal, but it is
  the only active play`;
- s93, one window later, the frame read `Opponent life trend: turn 34: 28, turn 36: 28,
  turn 38: 28, now 31 (+3 since turn 34)` - they had gained 8 back - and it cast the SECOND
  Hammer with the identical sentence.

Two things this proves. (1) The wave-56 latch is written as a memory task ("if it is HIGHER than
when you last looked") and the render now prints the whole history on its own line; keying the
latch to that line's sign is free. (2) **The rule was on the wrong seam.** s92 and s94 - the two
takes that a letter-reading of wave-56's P7 scores as breaks - are TARGET menus with exactly two
rows, `The opponent (player, life N)` and `Yourself (player, life N)`. There is no declining row
there. The decision was s91 and s93.

**Before**
```
CROSS-CHECK: "Opponent life:" is printed on every decision. If it is HIGHER than when you last
looked, ask what put it there before you latch. ...
```
**After** (head of the paragraph; the ONE-OFF / REPEATS test and the latch sentence are unchanged)
```
CROSS-CHECK, AND YOU NO LONGER HAVE TO REMEMBER ANYTHING: the board frame prints the whole
history on its own line, "Opponent life trend: turn 34: 28, turn 36: 28, turn 38: 28, now 31
(+3 since turn 34)". Read the SIGN in the final parenthesis before you number any face row. ...
```
**Added after it**
```
A TREND THAT MOVED UP WHILE YOU WERE BURNING IS THE LATCH, PROVED. [the turn-38 repro]
AND EVERY ONE OF THOSE DECISIONS WAS MADE ONE SCREEN EARLIER THAN IT LOOKED. The floor, the
latch and the release are all answered at the CAST row, where "Cast nothing right now" and the
hold row are on the menu. Once you have taken the cast, the target screen for Hammer of
Bogardan, Pyrite Spellbomb and Spark Spray on a creatureless board is exactly two rows ... and
there is no third answer. The cast row already carries the arithmetic you need: "{no creature
target - and 3 to the opponent at life 26 leaves them at 23}". If that sentence does not end in
a number at or below 0, and the release below has not fired, the answer on THAT window is
"Cast nothing right now".
```
Note the release itself is UNCHANGED and was never wrong: the wave-56 release fires at
`0 creatures AND 2 or fewer lands`, and this board printed FIVE lands, so the floor bound. The
one face take that the release licensed this corpus - `130v123` s80, `{right now: takes 3 damage
- they would be at -2; THIS WINS THE GAME}` - is the lethal branch and won the game.

---

## D57-2 (RULE #2 step 2) - `SURVIVES (loyalty N, K left)` is progress, not waste
**Finding.** The only two `- SURVIVES` takes at this seat this corpus are **planeswalkers**:
`130v152` s36 `Deal 2 damage with Siege-Gang Commander targeting Teferi, Who Slows the Sunset
{right now: takes 2 damage - SURVIVES (loyalty 3, 1 left)}` and `130v126` s43 `Sorin, Lord of
Innistrad ... {right now: takes 3 damage - SURVIVES (loyalty 4, 1 left)}`. Loyalty is a counter
that stays gone; toughness heals at end of turn. **Creature-shape SURVIVES takes: 0 of 25
rendering windows** - wave-56's P10 target, met, third corpus of trying - and the old rule as
written called both of these breaks.

**Added under "SURVIVES is the answer 'not this row'"**
```
ONE VERDICT READS "SURVIVES" AND IS STILL A TAKE, AND THE ROW NAMES IT: a PLANESWALKER prints
"- SURVIVES (loyalty 3, 1 left)", not "(toughness N)". ... Read which word is inside the
parenthesis.
```

---

## D57-3 (RULE #1 Starstorm / RULE #2 step 0b) - two cast-row annotations that mean "do not cast at all"
**Findings, two.**
1. `130v162` s7, turn 4, two mana: `Cast Starstorm {r}{r}{x} {X pricing: your mana affords only
   X=0 right now, which deals 0 damage and kills nothing}` - TAKEN, under `PLAN: Sweep their
   board with Starstorm (X=0 is fine ...)`. The next screen (s8) had ONE row, `X = 0
   {X pricing: X=0 - this cast does NOTHING ...}`, so the X-menu "take" is forced, not a
   decision. The card was spent for nothing on turn 4.
2. `130v152` s31, turn 14, 19 life: `Cast Starstorm ... At X=4 - kills THEIRS: Sigarda, Champion
   of Light; YOURS: Dwarven Blastminer, Siege-Gang Commander, Goblin x3` with `[<- best X for
   this cast: X=4 - best trade: the most of THEIRS at the smallest cost to YOURS]` - TAKEN, in a
   reply whose own PLAN read `Sweep Sigarda with X=4. Attack with Siege-Gang Commander and 3
   Goblins.` The sweep killed the attack. Clause (a) of the existing rule names Siege-Gang
   Commander explicitly; it lost to the marker printed beside the row. `[<- best X for this
   cast:` is a NEW literal this corpus - the marker now prints on the CAST row, not only on the
   X menu, and there it reads as a recommendation to cast.

**Added at the end of step 0b's cast-row paragraph**
```
TWO CAST-ROW ANNOTATIONS MEAN "DO NOT CAST STARSTORM AT ALL", AND THEY ARE THE WHOLE TEST:
  "{X pricing: your mana affords only X=0 right now ...}"  -> [repro 1]
  any "YOURS:" list on the cast row naming RORIX BLADEWING or SIEGE-GANG COMMANDER -> (a) above,
   and it does not become false because a marker is printed beside it. [repro 2]
THE MARKER NOW PRINTS ON THE CAST ROW TOO: "[<- best X for this cast: X=4 - best trade: ...]".
It is still an answer to WHICH X. It is never an answer to WHETHER, and "best trade" is printed
even when every trade is bad - count the two lists yourself and compare their lengths before you
number that row.
```

## D57-5 (RULE #2 step 0b, the TAKE IT clause) - the trade test gets a comparison
**Before** `TAKE IT when the THEIRS list at an affordable X names TWO OR MORE creatures.`
**After** `TAKE IT when the THEIRS list at an affordable X names TWO OR MORE creatures, and the
YOURS list at that X is no longer than it. ... losing five of yours to kill one of theirs is not,
whatever is marked.`

---

## D57-4 (RULE #2) - both halves of their life loop: deploy nothing
**Finding.** `130v126`, turn 16. At s63 the pilot cast Siege-Gang Commander off a row that priced
the board for it: `{their converter: this body has toughness 2 and they control 1 life-to-damage
converter (Sanguine Bond) - any effect of theirs that gains them life equal to its toughness
takes 2 off YOU: life 18 -> 16}`, with the frame's block naming BOTH halves. The opponent then
cast two Tribute to Hunger; the Goblin's toughness fed Sanguine Bond, Exquisite Blood fed it
back, and the narration is eighteen lines of `Opponent gained 1 life (now N)` / `You lost 1 life
(now M)` - **18 to 0, one point at a time, in that one phase**. Game ended 0 to 38 on turn 16.
Primitives verified: Sanguine Bond (mtg.txt:99004) `auto=@lifeof(player) from(*[-lifefaker]|*):
life:-thatmuch opponent`; Exquisite Blood (mtg.txt:37966) `auto=@lifelostfoeof(player):life:
thatmuch controller`. Nothing in this sixty removes an enchantment.
**ADDED** a paragraph before the "NEVER TOUCH A LAND" block: on that board, no creature, no face
damage, nothing that gains them life; hold everything and take the hold row.

---

## Trims
| # | where | what came out | the rung's number this corpus |
|---|---|---|---|
| T1 | #0 RULE | the 109-window count -> "seventh corpus at zero" | 2 renders, 0 draws/cycles |
| T2 | #1 Lay Waste | the four-wrong-cycles anecdote | 19 cycles, 1 by the letter, and that one is the release |
| T3 | #1 Starstorm | the "sixty-plus windows" bookkeeping | 1 X=0 cast (the repro above) |
| T4 | #1 release | the fourteen-window Opponent-life-2 anecdote | the window did not arise (UNTESTED) |
| T5 | #2 floor | the six-casts-at-19/20/23 list and the 70-turn anecdote | 4 face takes: 1 lethal, 1 forced, 2 breaks |
| T6 | #2 step 1 | the Ob Nixilis worked example | - |
| T7 | #2 step 2 | the Master of the Feast / Nadaar anecdotes | 0 creature-SURVIVES takes |
| T8 | #2 step 0 | the "lost that game 0 to 1" lethal anecdote | 1 lethal face row, taken |
| T9 | #2 step 0b | the plan-device worked example | markers 5 menus / 3 marked-row takes |
| T10 | #2 step 0b | the Rorix-over-Battlement carve-out anecdote | - |
| T11 | #2 own-permanent | the two self-target anecdotes | 0 self-target takes |
| T12 | COMBAT race check | the 4-life and 89-power anecdotes | UNTESTED, ninth corpus |
