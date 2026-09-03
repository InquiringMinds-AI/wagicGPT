# wave-55 deck126 edit texts (Sanguine Blood)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` at the boundary pass
dd0cd2d74 (56,650 bytes). Revised guide 60,384 bytes - inside the 41-71 KB pool band.
Corpus `matchups-20260903-074354`; seq numbers are records in the deck126 seat log named by the
short tag (`126v146` = the deck126 seat of the game against deck146). Six edits.

## Edit 1 - D-A

**Finding.** STRATEGY D-1. `126v130` seq 14 (t11, 23 life): both halves were missing from the battlefield line and BOTH rows were printed at "casting this taps you out"; the seat took Exquisite Blood. Sanguine Bond landed t13. From t13 the pair sat out for nine turns with no starter - deck130's only attacker was Rorix Bladewing (6/5, `mtg.txt:96836-96843` abilities=flying,haste), which none of sixteen defenders can block, so no blockers window was ever offered (correctly), and no Tribute, Staff or Sorin was ever castable. The seat lost 23 -> 0. Entry 1 had no tiebreak; Rule #3 already says which half is the clock, so the tiebreak is one sentence pointing at it. Entry 1 itself is 9 takes of 9 windows - PASS - so nothing else in the entry moves.

**Before:**
```
 1. Exquisite Blood or Sanguine Bond - whichever NAME is missing from your battlefield line, at any
    "leaves N", including "casting this taps you out". A copy of a name already on that line is
    not this entry; skip it. Nine of nine one corpus - the turn the row appeared, it was cast.
```

**After:**
```
 1. Exquisite Blood or Sanguine Bond - whichever NAME is missing from your battlefield line, at any
    "leaves N", including "casting this taps you out". A copy of a name already on that line is
    not this entry; skip it. Nine of nine one corpus and nine of nine this one - the turn the row
    appeared, it was cast.
    BOTH NAMES MISSING AND BOTH ROWS PRINTED ON THE SAME MENU: TAKE SANGUINE BOND. That is Rule
    #3's split - the Bond alone is already a clock and the Blood alone does nothing you can start -
    so the Bond is the half that is worth something if the second one never arrives. At 23 life you
    had both rows at "casting this taps you out" and took Exquisite Blood; the Bond landed two
    turns later, their only threat was a 6/5 [flying] Rorix Bladewing your walls cannot block, and
    the pair sat on your battlefield for nine turns with no Tribute, no Staff and no Vampire to
    start it while you went 23 to 0. Two turns of Bond clock was the whole difference you could
    still have made.
```

## Edit 2 - D-B

**Finding.** STRATEGY D-2. `126v125` seq 169 (t34, 7 life) and seq 185 (t36, 5 life): with both enchantments out and the opponent at 44 with zero creatures, the only "Cast" rows were a Tribute reading "at 0 this does nothing" and a "[second copy: you already control Chromatic Lantern" row. The seat correctly answered "Cast nothing right now" - against the letter of Rule #2's absolute. Then at seq 200 (t38, 3 life) it took the second Lantern off the same menu: the corpus's only second-copy break, 1 of 40 rows (was 0 of 24). The absolute contradicted Rule #4's zero stop and entry 7's second-copy stop; the exclusion removes the contradiction rather than weakening the passivity rule.

**Before:**
```
to four attackers with one blocker on your board. A wall you cast is a blocker you have; while both
names are on your battlefield line, "Cast nothing right now" is NEVER your answer to a menu that
prints any "Cast" row at all.
```

**After:**
```
to four attackers with one blocker on your board. A wall you cast is a blocker you have; while both
names are on your battlefield line, "Cast nothing right now" is NEVER your answer to a menu that
prints any "Cast" row at all.
TWO ROWS ARE NOT "A CAST ROW" FOR THIS LINE, AND BOTH ARE PRINTED SO YOU CAN SEE IT: a Tribute to
Hunger row reading "{right now: they control 0 creatures - at 0 this does nothing}", and any row
carrying "[second copy: you already control <that same name>". Those two cast nothing and change
nothing, and this line does not override Rule #4's zero stop or entry 7's second-copy stop - it
sits under them. When a menu prints only those, "Cast nothing right now" IS your answer. You read
that menu right twice at 7 and 5 life and then, on the same menu at 3 life, took the second
Chromatic Lantern - one break in forty second-copy rows, and it bought nothing.
```

## Edit 3 - D-C

**Finding.** STRATEGY D-3. `126v146` seq 6 (t5, 20 life, no enchantment out): the ONLY attacker was Triumphant Adventurer (1/1) [first strike, deathtouch]; both B-lines read "your blocker dies, attacker lives (blocking trigger: you gain 3 and may gain 2 more)" / "(blocking trigger: you may gain 2)". STEP 1 benches both. The seat blocked with both, gained 7 (20 -> 27) and Pride Guardian died. At seq 16 (t11) one blocker faced five attackers (2+5+2+2+4 = 15) and the seat lost at -14 on t13. The "(blocking trigger: ...)" clause is NEW this corpus and it renders INSIDE the same parenthesis as "your blocker dies", which is exactly the pull STEP 1 exists to resist - the rule had to name the string.

**Before:**
```
 - If a B-line offers you a "dies" attacker and a "lives" attacker, take the LIVES one, even when
   the dies one has the bigger "deals N".
```

**After:**
```
 - THE PARENTHESIS NOW CARRIES THE LIFE GAIN INSIDE IT, AND THE DEATH HALF STILL COMES FIRST. A
   B-line reads "may block A1 (your blocker dies, attacker lives (blocking trigger: you gain 3 and
   may gain 2 more))". The inner clause is TRUE and it is not permission: it prices ONE combat,
   the way the STOPS-N tag does, while the wall it costs you would have blocked in every combat
   after this one. At 20 life with no enchantment out you put BOTH walls on a lone Triumphant
   Adventurer (1/1) [first strike, deathtouch] whose every parenthesis read "your blocker dies";
   you banked 7 life, Pride Guardian died, and two combats later ONE blocker faced five attackers
   for 15 and you lost that game at -14. Read "your blocker dies" first, then STEP 1's three
   carve-outs, and only then the life the block would pay.
 - If a B-line offers you a "dies" attacker and a "lives" attacker, take the LIVES one, even when
   the dies one has the bigger "deals N".
```

## Edit 4 - D-D

**Finding.** STRATEGY D-4. `126v146` seq 11 (t8, "Mana available: 4", Sanguine Bond {3}{b}{b} on the hand line, "Cast Chromatic Lantern {3}" printed beside "Cast Overgrown Battlement {1}{g}"): the seat cast the Battlement. Entry 7's carve-out names this shape exactly and it was not applied. The Bond became castable on t12; the game ended -14 on t13. Twelve menus matched the shape (combo card on the hand line AND "Mana available:" 4 or less): 5 Lantern takes, 5 declines for an entry ABOVE the Lantern (4 live Idyllic Tutors, 1 Rule-7b Tribute) which are correct, and 2 declines for a wall (`126v146` seq 11 and `126v162` seq 26, the latter at 2 life where the wall is defensible). One live break; the edit names the wall as the row the carve-out beats.

**Before:**
```
    hand and the Bond came down the turn after; both games were won. With no combo piece in hand,
    or at 5 or more sources, the wall comes first.
```

**After:**
```
    hand and the Bond came down the turn after; both games were won. With no combo piece in hand,
    or at 5 or more sources, the wall comes first.
    THE ROW IT BEATS IN THAT SHAPE IS A WALL, AND A WALL IS WHAT YOU TAKE INSTEAD. At 4 sources
    with Sanguine Bond {3}{b}{b} on your hand line and "Cast Chromatic Lantern {3}" printed beside
    "Cast Overgrown Battlement {1}{g}", you cast the Battlement; the Bond became castable four
    turns later, on turn 12, and that game ended -14 on turn 13. The Battlement's green cannot pay
    either {b}; the Lantern can pay both and makes every land of yours pay them too. Twelve menus
    this corpus matched this shape: you took the Lantern on five, an entry ABOVE it (a live Idyllic
    Tutor, a 7b Tribute) on five - all correct - and a wall on two. The wall is the only wrong
    answer here.
```

## Edit 5 - D-E

**Finding.** STRATEGY D-5. `126v162` seq 8 (t9, 20 life): Tribute to Hunger cast at "Shield Sphere (0/6) [defender] is sacrificed, you gain 6" - the exact shape Rule #4 says to hold. The seat's second Tribute then correctly ate Master of the Feast (5/5) [flying] at seq 19 (t13) and there was no third. 1 of 1 defender-victim rows taken. No new rule - the existing line gets this corpus's instance so the count is not a fiction.

**Before:**
```
 - That row names a [defender] victim (Shield Sphere, Fog Bank): hold the Tribute and take the
   next entry of Rule #7, unless you are at 10 or less or both enchantments are out.
```

**After:**
```
 - That row names a [defender] victim (Shield Sphere, Fog Bank): hold the Tribute and take the
   next entry of Rule #7, unless you are at 10 or less or both enchantments are out. You spent one
   again this corpus - "Shield Sphere (0/6) [defender] is sacrificed, you gain 6" at 20 life on
   turn 9 - and four turns later their Master of the Feast (5/5) [flying] arrived; your SECOND
   Tribute ate it, and there was no third. A 0/6 that cannot attack is not what is killing you,
   and 6 life is not worth the only removal this deck owns.
```

## Edit 6 - D-F

**Finding.** NEW-BASELINE record. 6 offers, 0 mulligans (wave 54: 1 taken). `126v162` seq 1 kept a ONE-land seven - Forest; Sorin, 2x Tribute to Hunger, Perimeter Captain, Pride Guardian, Sanguine Bond - whose coverage line read "would not cover any spell in it (you have 1 land; no spell in it is castable off {G} alone on turn one)", against the branch's explicit "ship it on a seven and on a six". Second land t3, third t7; lost -6 on t16. The branch already held a WIN story for a one-land keep, which reads as permission, so the branch is reordered to end on the imperative and both stories are labelled as not moving it. No floor, no rule change, no owner question (no mulligan was taken anywhere, so there is no chain and no mull-to-zero to flag).

**Before:**
```
  - Zero lands, or one land: the hand casts nothing until it draws lands, and a fresh seven at
    24-in-60 beats waiting - ship it on a seven and on a six. You kept a one-land seven whose
    coverage line named two Perimeter Captains and won on Battlement mana: the keep was outside
    this line and the win does not move it - one land is one draw from nothing. You shipped a one-land seven this
    corpus, kept the two-land six behind it, and won.
```

**After:**
```
  - Zero lands, or one land: the hand casts nothing until it draws lands, and a fresh seven at
    24-in-60 beats waiting. Two one-land sevens have been kept against this line: one won on
    Battlement mana, and one this corpus - Forest alone, under Sorin, two Tribute to Hunger, two
    walls and a Sanguine Bond, with the coverage line reading "would not cover any spell in it (you
    have 1 land; no spell in it is castable off {G} alone on turn one)". That hand played its
    second land on turn 3 and its third on turn 7, and the game was lost at -6 on turn 16. A win
    with one land does not move this line and neither does a loss; the reason does. One land is one
    draw from nothing, and a Perimeter Captain you can cast on turn one is still one spell and then
    a stop. SHIP IT on a seven and on a six. You shipped a one-land seven one corpus, kept the
    two-land six behind it, and won.
```
