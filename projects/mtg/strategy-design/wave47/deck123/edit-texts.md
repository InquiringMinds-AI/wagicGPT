# deck123 (Intruders of Thraben) - wave-47 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (= wave-46 revision,
byte-identical, verified). Three edits. 38,144 -> 40,084 chars.
Corpus: `matchups-20260826-182155`. Every number below is read off a prompt or an option row.

---

## EDIT A47-1 - RULE 1: the token-loop stop, rewritten on a number the activation moves

**Finding.** deck123 took the token line **3,743 times in 3,763 offers** this corpus, and
**3,436 of those takes are in three games with Intruder Alarm on the battlefield**:
- `...deck123-0x55e867ceaa40-vs-...deck162.jsonl` - **1,868 activations in a single turn**
  (turn 10, the opponent's Main phase 1). The option row is the ONLY row on the screen and
  reads `Create human with Thraben Doomsayer [cost: Tap] [repeat: activated this turn 1867
  times already; you control 1896 creatures]`. Its plan at seq 1920 reads "Continue tapping ...
  On my next main phase, attack with all creatures to win the game." It never reached that main
  phase: the orchestrator killed the game (no gameend record).
- `...vs-...deck126.jsonl` - 1,540 activations across turns 9/10/11 (434 on its own turn 9,
  1,102 on the opponent's turn 10). It won on turn 11.
- `...vs-...deck146.jsonl` - 214 activations. Won on turn 13.

**Why the existing rule could not stop it - this is the mechanical part.** RULE 1's stop was
"add up the printed power of the creatures that do NOT read summoning sick; call that P; stop
when P is at or above the opponent's life". Every token the loop makes IS summoning sick, so
**taking the line does not change P**. The own-turn branch was therefore unreachable inside a
loop, and the opponent's-turn branch said, in my own words, *"take it anyway ... it costs
nothing"* - with no bound at all. Both branches were unbounded; the guide is the loop's guide
half. (Card facts verified in `Res/sets/primitives/mtg.txt`: Intruder Alarm
`auto=@movedTo(creature|myBattlefield):untap all(creature)` + `lord(creature) doesnotuntap`;
Thraben Doomsayer `auto={T}:token(Human,Creature Human,1/1,white)`.)

**Before**
```
Then, and only then - and the test is POWER, not a headcount:
  - Add up the printed power of the creatures on your battlefield line that do NOT read
    "summoning sick". Call that number P, and read the "Opponent life:" number.
  - P is less than the opponent's life: TAKE THE LINE. Take it again every time it comes back.
  - P is at or above the opponent's life AND it is your own turn: stop taking the line and go
    to the ATTACK section - you already have the game and another token does not add to it.
  - P is at or above the opponent's life and it is the OPPONENT'S turn: take it anyway. A token
    made on their turn is a blocker now and an attacker on your next turn, and it costs nothing.
THERE IS NO CREATURE-COUNT CAP, and the old one at 12 and then at 20 is GONE. [...4 more lines]
The render carries no width cost that would argue for a lower stop - [...3 lines]
"I have tapped it 0 times this turn" is not a reason and never was; the count on the battlefield
line is the state, not the "[repeat:]" note.
```

**After** (full text is in `strategy.txt`; the operative parts)
```
THE STOP IS A HEADCOUNT AFTER ALL, BECAUSE POWER CANNOT MEASURE THIS LOOP.
[...the summoning-sick explanation and the three counts above...]
Use the two numbers that DO move, both printed in every window:
  - M = the number after "of which" on YOUR battlefield line [...]  Every activation adds one.
  - L = "Opponent life:", and C = the number after "of which" on the OPPONENT'S battlefield line
  - M is BELOW L + C + 3: TAKE THE LINE.
  - M reaches L + C + 3: STOP. Answer "CHOICE: 0 (pass)".
  - HARD TRIPWIRE [...] If [repeat:] N is 20 OR MORE, answer "CHOICE: 0 (pass)" and stop reading.
PASSING IS ALWAYS AVAILABLE, EVEN WHEN THE LOOP IS THE ONLY ROW ON THE SCREEN. [...] Pass is not
a row; it is the number 0, and the reply instructions on that same screen say so.
WHY STOPPING MATTERS EVEN WHEN YOU ARE WINNING: [...] 233,662 character prompt with 1,525
separate attacker rows to name by hand.
```
`L + C + 3` is chosen so it is provable off the printed numbers: every token is at least 1/1, so
at L+C+3 bodies they cannot block enough of them to survive the next attack. Worked against the
corpus: vs126 turn 10 (L=15, C=3) stops at 21 instead of 1,541; vs162 turn 10 (L=20, C=3) stops
at 26 instead of 1,897.
The `[repeat:]` tripwire exists because it is the ONE number that is monotone inside a single
turn and is printed on the option row itself, so it needs no board reading at all.
The pass-affordance sentence is paid for by the row itself: 1,894 of the 1,896 replies in the
dead-looped game took the only listed option, and the two that did not (seq 21, seq 49) answered
`CHOICE: 0 (pass)` - so pass is legal and the pilot has used it; it just is not a row.

---

## EDIT A47-2 - RULE 5: an edict tripwire on the pilot's own sentence, and 6 lines of duplication cut

**Finding.** Two edict casts in the corpus, both outside the rule, plus a third false reading
inside a blockers reply:
- `...vs-...deck152.jsonl` seq 23, opponent battlefield `of which 5 are creatures`.
  PLAN: *"Use Tribute to Hunger to remove Sigarda (toughness 4) to gain 4 life"*.
- `...vs-...deck126.jsonl` seq 408, opponent battlefield `of which 3 are creatures`.
- `...vs-...deck152.jsonl` seq 27 (blockers, 8.7 minutes of latency, ~1,800 words): its whole
  survival line is *"Cast Tribute to Hunger on Briarbridge Tracker ... surviving the combat
  with 5 life"*.
The option row printed the card in full, on screen, at every one of them:
`Cast Tribute to Hunger {2}{b} {leaves 1 of your 4 untapped mana sources untapped} - legal
targets right now: the opponent {card text: "Target opponent sacrifices a creature of their
choice. You gain life equal to that creature's toughness."}` - verified against the primitive
(`target=opponent ... notaTarget(creature|mybattlefield) sacrifice`). So this is not a render
gap; it is the pilot writing a plan the row refutes.
RULE 5 already said "they hand over their worst one", "Both cards let the SACRIFICING player
pick", and "If it is not exactly 1, do not cast an edict" - **three separate paragraphs, third
corpus running, still violated.** More prose is not the lever. The edit moves the check onto
the pilot's OWN sentence (which is the thing that is wrong) and deletes one of the duplicates
so the rule gets shorter, not longer.

**Added at the head of RULE 5**
```
IF YOUR PLAN NAMES WHICH CREATURE AN EDICT WILL REMOVE, THE PLAN IS ALREADY WRONG. Check your
own sentence before you check the board: "Tribute to Hunger on Sigarda", "Tribute to Hunger on
Briarbridge Tracker", "Tribute to Hunger to remove Sorin" are all sentences this deck has
written, and all three are impossible. You NEVER pick. [...] The only thing you choose is
whether to cast it at all, and the answer to that is the count of their creatures.
```
**Deleted** the whole "Both cards let the SACRIFICING player pick, so with TWO OR MORE ..."
paragraph (5 lines) - it is a third statement of the same fact.
**Amended** the operative paragraph to carry this corpus's counts: `... This deck cast it at
FIVE of their creatures and at THREE of their creatures in the same corpus, and got a body it
did not want both times.`

---

## EDIT A47-3 - RULE 0: the affordability check reaches combat

**Finding.** `...vs-...deck152.jsonl` seq 27, blockers at 9 life. The screen reads
`Mana available: 1 total (one untapped source ...)`; the pilot's line is Tribute to Hunger,
`{2}{b}` = three mana. It computed the survival arithmetic correctly and built it on a spell it
could not pay for. RULE 0 already teaches "READ THE COSTS IN MANA, NOT IN SYMBOLS", but every
example under it is a main-phase cast; the failure moved to combat.

**Added, 4 lines, at the end of RULE 0**
```
This applies hardest in COMBAT. On a blockers screen at 9 life this deck worked out, correctly,
that Tribute to Hunger after blockers would let it survive - with "Mana available: 1 total" on
the same screen and Tribute costing {2}{b}, three mana. A spell you cannot pay for is not a
survival plan; check "Mana available: N total" BEFORE you build a line around a card in hand.
```

---

## NOT edited, and why

- **RULE -1 (mulligan).** PASSED this corpus: 8 mulligan asks at this seat, longest chain ONE
  mulligan, no chain reached a third look (wave 46: chains of 7 and of 6). The engine also
  shipped the priced row and dropped the carried plan from pregame asks in the same binary, so
  the corpus cannot separate guide from engine - which is exactly why I am not touching a rung
  that passed. Sunset test in the seat file's predictions.
- **The deleted 12/20 creature cap.** Take rate 3,743/3,763 = 99.5%; the below-cap declines of
  wave 46 are gone. The cap stays deleted; the runaway is handled by A47-1, which is a stop on
  sufficiency, not a cap on width.
- **RULE 3 (Damnation).** 23 offers, **0 casts**, 0 plans claiming it removes an enchantment.
  Held; no change.
- **Blockers STEP 1/STEP 2, RULE 4 (Tragic Slip), RULE 6 (Greaves).** No violation found.
- **RULE 2 (Marsh Flats): 5 fetches, 4 onto Underground Sea and ONE onto Tundra
  (`...vs-...deck126.jsonl` seq 10) with an `Underground Sea` row listed on the same screen -
  a violation of the printed order, 1 of 5. NOT patched: the rule's whole point is "a dual, not
  a basic", both taken lands are duals that make blue, and no game turned on it. Recorded as a
  denominator, re-checked next wave.
