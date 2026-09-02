# wave-53 deck126 edit texts (Sanguine Blood, WGB wall prison)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` (49,293 bytes, 583 lines).
Result: `wave53/deck126/strategy.txt` (53,926 bytes, 635 lines; +4.6 KB / +9%). 14 edits.
Every edit cites a seq in `matchups-20260902-023342`.

## D83
**Finding.** **B4 obligation, discharged.** Lane O replaced `- it cannot attack this turn` with `- it cannot block on their turn` on defender-naming tap rows. THREE-NUMBER AUDIT (#168): old string on a cast row **0/27**; new string **27/27**; **8 takes**. The D69 paragraph existed only to translate the old words into the true consequence; with the render saying it outright the paragraph collapses to one sentence plus the take-it list. Takes were vs125 seq 24, vs146 seq 20 (x2 rows), vs162 seq 11 and siblings - each for a Bond, a Blood, a Sorin or a Tutor, so the rule is at its ceiling.

**Before**
```
THE CAST ROWS NAME THE SAME COST, AND YOU HAVE TO TRANSLATE IT. A cast row can carry
"{paying this taps: Overgrown Battlement - it cannot attack this turn}". Every creature in this
deck is a Defender and none of them could ever attack, so the words "cannot attack" cost you
nothing; what that clause is telling you is WHICH OF YOUR WALLS WILL BE TAPPED when you take this
row, and a tapped wall does not block on their next turn. Read it as "this block is not available
next turn" and weigh it against what the spell buys. Ninety-one such rows this corpus; the seven
you took each cost one wall for one combat, and each was worth it because the spell was a Sanguine
Bond, a Sorin or a Tribute. It would not be worth it for a wall or a Chromatic Lantern.
```

**After**
```
THE CAST ROW NAMES THE WALL IT WILL COST YOU, IN THOSE WORDS: "{paying this taps: Overgrown
Battlement - it cannot block on their turn}" - so take the row when the spell is a Sanguine Bond, an
Exquisite Blood, a Sorin or a Tribute, and not when it is another wall or a Chromatic Lantern.
Twenty-seven such rows last corpus, eight taken, every one of the eight for a spell off that list.
```

## D84
**Finding.** deck126 vs125 seq 73 and seq 74 (t42, 6 life): Tribute to Hunger cast TWICE off `{right now: they control 0 creatures - at 0 this does nothing}`, in two windows of one turn, breaking a three-corpus ceiling (39 rows, 2 casts). Both replies' PLAN lines said not to (`Avoid casting Tribute to Hunger as there are no creatures to target`; `The opponent has no creatures, so Tribute to Hunger does nothing. Pass the turn.`) while the CHOICE line read `CHOICE: 1` on a two-row menu whose row 2 was the decline. `answer_replaced` false, no `decision_reversed_in_prose` note - the engine half is seat item MED #4.

**Before**
```
THE ZERO STOP: the engine lists Tribute even when they have NO creature - the row says "at 0 this
does nothing". Never cast it there. It sacrifices nothing, gains nothing and taps you out for
{2}{B}. Forty such rows this corpus, zero casts - keep it that way.
```

**After**
```
THE ZERO STOP: the engine lists Tribute even when they have NO creature - the row says
"at 0 this does nothing". Never cast it there. It sacrifices nothing, gains nothing and taps you
out for {2}{B}. Thirty-nine such rows last corpus and you cast it TWICE, both at 6 life, in two
windows of the same turn, off a two-row menu whose other row was "Cast nothing right now".
BOTH TIMES YOUR OWN PLAN LINE SAID NOT TO - "Avoid casting Tribute to Hunger as there are no
creatures to target", and then "The opponent has no creatures, so Tribute to Hunger does nothing.
Pass the turn." THE ENGINE EXECUTES THE NUMBER ON YOUR CHOICE LINE, NOT THE SENTENCE UNDER IT. On a
two-row menu, "CHOICE: 1" is a cast; the decline is row 2. Before you write the number, check it
against the sentence you are about to write after it: if your PLAN says pass, the number is the
"Cast nothing right now" row.
```

## D85
**Finding.** deck126 vs125 seq 14 (t10) and vs130 seq 21 (t21): Idyllic Tutor cast off `[finds only an enchantment card - every enchantment left in your library is a copy of one you already control or hold: ...]` with BOTH halves already on the hand line. 22 Tutor rows, 10 taken; 8 of the 10 were the correct `still in your library, not on your battlefield or in your hand:` form. The dead form now has its own stop keyed to the literal (#168).

**Before**
```
 2. Idyllic Tutor, when either enchantment is missing from both your hand line and your
    battlefield line. A missing name that is on your HAND line with a "Cast" row is entry 1, not
    this. You cast an Overgrown Battlement over a Tutor with neither piece in hand; the Tutor came
    two turns later and the Bond it found four.
```

**After**
```
 2. Idyllic Tutor, when either enchantment is missing from both your hand line and your
    battlefield line. A missing name that is on your HAND line with a "Cast" row is entry 1, not
    this. You cast an Overgrown Battlement over a Tutor with neither piece in hand; the Tutor came
    two turns later and the Bond it found four.
    THE ROW TELLS YOU WHEN THE TUTOR IS DEAD, AND IT IS ONE STRING. "Cast Idyllic Tutor {2}{w}
    [finds only an enchantment card - every enchantment left in your library is a copy of one you
    already control or hold: ...]" means the search cannot bring you anything you do not already
    have. When that clause is printed, THIS ENTRY HAS NOT MATCHED - go down the list. You took that
    exact row twice last corpus, both times holding Sanguine Bond AND Exquisite Blood in hand, and
    both times what the turn needed was a wall or the enchantment itself. The other form -
    "still in your library, not on your battlefield or in your hand: <name>" - IS this entry:
    it names the half you are missing, and you took eight of those correctly.
```

## D86
**Finding.** deck126 vs125 seq 22 (t16, 20 life): a second Sanguine Bond cast off the BARE `[second copy: you already control Sanguine Bond; both stay on the battlefield - no legend rule]` on a two-row menu. Lane P's new partial form fixed the Lantern half completely (6 rows, 0 takes, was 5/5 last corpus) but the enchantments get the bare tag because their auto lines genuinely stack - so the guide has to name the bare tag itself.

**Before**
```
    THE ROW SAYS WHICH IT IS. A second copy prints "[second copy: you already control Exquisite
    Blood; both stay on the battlefield - no legend rule]" - that tag means the copy is legal, NOT
    that it is worth casting; a second Blood or a second Bond changes nothing about who is winning.
```

**After**
```
    THE ROW SAYS WHICH IT IS, AND FOR THESE TWO CARDS THE TAG IS THE BARE ONE: "[second copy: you
    already control Sanguine Bond; both stay on the battlefield - no legend rule]", with nothing
    after "rule". A Chromatic Lantern's second copy now prints a longer tag that tells you outright
    the effect is already on; YOURS DOES NOT, and the absence of that sentence is not permission.
    The tag means the copy is legal, NOT that it is worth casting; a second Blood or a second Bond
    changes nothing about who is winning. You cast a second Sanguine Bond off that bare tag at 20
    life on a two-row menu last corpus - the other row was "Cast nothing right now", and "Cast
    nothing right now" was the answer.
```

## D87a
**Finding.** deck126 vs146 seq 21 (t15, 12 life, pair out): the reply's PLAN read `Overgrown Battlement blocks the Goblin (2 damage) because it survives, stopping 2 damage and gaining 2 life (loop triggers)`. The battlefield line held no Perimeter Captain; Overgrown Battlement gains nothing on a block (`mtg.txt:83596` `{T}:foreach(creature[defender]|myBattlefield) add{G}`, `abilities=defender` - no life clause). Rule #2's entry needed its negative.

**Before**
```
 - Declare a block with ANY wall, if Perimeter Captain is on your battlefield (2 life).
 - Declare a block with Pride Guardian (3 life, Captain or no Captain).
```

**After**
```
 - Declare a block with ANY wall, if Perimeter Captain is on your battlefield (2 life). WITHOUT a
   Perimeter Captain, an Overgrown Battlement's or a Wall of Omens' block gains you NOTHING and
   starts nothing - only Perimeter Captain and Pride Guardian turn a block into life. You wrote
   "Overgrown Battlement blocks the Goblin ... gaining 2 life (loop triggers)" with no Captain on
   your line; the block stopped 2 damage and that was all it did.
 - Declare a block with Pride Guardian (3 life, Captain or no Captain).
```

## D87b
**Finding.** Rule #2 VALIDATED twice, and both wins are worth carrying: vs152 seq 36 (t16, 4 life vs 17) attacked one 1/1 Vampire into a 3/3 Luminarch Aspirant - the Vampire was BLOCKED and the game ended 21/0 in that combat; vs130 seq 33 (t29, 1 life vs 18) cast Tribute at Rorix Bladewing and ended 21/-2. The counts in the rule were two corpora stale.

**Before**
```
Any one of these wins from any life total, yours or theirs. You do not need a big number - 1 life is
enough, because the loop runs itself until the opponent is at 0.
```

**After**
```
Any one of these wins from any life total, yours or theirs. You do not need a big number - 1 life is
enough, because the loop runs itself until the opponent is at 0. IT DOES: last corpus you won twice
on it and both wins came from one point. At 4 life against their 17 you attacked with a single 1/1
Vampire into a 3/3 blocker - the Vampire was BLOCKED, lifelink paid you 1 for the damage it dealt
that blocker, and the game ended 21 to 0 in that combat. At 1 life against their 18 you cast Tribute
to Hunger at a Rorix Bladewing and the game ended 21 to -2. Blocked, tiny, or from a losing life
total: the size of the trigger does not matter.
```

## D87c
**Finding.** deck126 vs146 seq 22 and seq 23 (t16, 4 life, both Exquisite Blood and Sanguine Bond on the battlefield line, `Cast Wall of Omens` printed on both menus): answered `Cast nothing right now` twice. This is D75's exact failure a second corpus running, and the wall would have been a second blocker for the 14 damage that killed the seat at t17.

**Before**
```
 "Cast nothing right now" is
the wrong answer to five windows in a row with a Wall of Omens on the menu; that is exactly what
you answered at 9, 7, 5, 3 and 1 life in the game above.
```

**After**
```
 "Cast nothing right now" is
the wrong answer to five windows in a row with a Wall of Omens on the menu; that is exactly what
you answered at 9, 7, 5, 3 and 1 life in the game above - AND AGAIN LAST CORPUS, twice in one turn
at 4 life with the pair out and "Cast Wall of Omens" printed on both menus. You died the next turn
to four attackers with one blocker on your board. A wall you cast is a blocker you have; while both
names are on your battlefield line, "Cast nothing right now" is NEVER your answer to a menu that
prints any "Cast" row at all.
```

## D88
**Finding.** deck126 vs146 seq 24 (t17, 4 life, four attackers, an untapped 0/4 Overgrown Battlement on the B-line): `BLOCKS: none`, with the reply reading `I will concede this game`. Rule #5's own carve-out says the bench is OFF at `you would be at 8` or less. Substantively the block did not change the outcome; the rule and the refusal-to-play both needed naming.

**Before**
```
"BLOCKS: none" is right only when EVERY blocker the list offers you falls under STEP 1.
```

**After**
```
"BLOCKS: none" is right only when EVERY blocker the list offers you falls under STEP 1 - and at a
header reading "you would be at 8" or less STEP 1 benches nobody, so at 8 or less "BLOCKS: none" is
right only when the list offers no blocker at all. At 4 life against four attackers you answered
"BLOCKS: none" with an untapped 0/4 Battlement on the list and wrote "I will concede this game".
You do not concede, and you do not stop answering for a board you have decided is lost: a blocker
that dies still stops its attacker's whole damage this combat, and that is the only number still
in your hands.
```

## D90
**Finding.** deck126 vs125 seq 15 (t10): the `reveal` pick took Sanguine Bond with BOTH halves on the hand line. 9 reveal picks, 7 correct against `pick the OTHER one`; the two misses are both the dead-Tutor search D85 stops. Rule #3.4 had no branch for `you have both`.

**Before**
```
 4. The Tutor then shows you your whole library as a long numbered list. Before the PUT line, read
    your battlefield line AND your hand line: the name printed on either is the one you already
    have. Pick the OTHER one; if you have NEITHER, pick SANGUINE BOND - it is the half that wins on
    its own (Rule #3). You picked a Sanguine Bond with a Sanguine Bond on your battlefield and an
    Exquisite Blood in your hand.
```

**After**
```
 4. The Tutor then shows you your whole library as a long numbered list. Before the PUT line, read
    your battlefield line AND your hand line: the name printed on either is the one you already
    have. Pick the OTHER one; if you have NEITHER, pick SANGUINE BOND - it is the half that wins on
    its own (Rule #3). If you have BOTH already (that is the search Rule #7 entry 2 tells you not to
    make), the pick cannot help you - take SANGUINE BOND and cast a real card next turn. You picked
    a Sanguine Bond with a Sanguine Bond on your battlefield and an Exquisite Blood in your hand,
    and again with both halves sitting in your hand.
```

## D91a
**Finding.** #177 bullet-vs-prose sweep: the SITUATIONS list carried no bullet for D84's CHOICE-vs-PLAN guard, D85's dead-Tutor literal or D86's bare second-copy tag. Three compact bullets added.

**Before**
```
 - That row reads "at 0 this does nothing": never cast Tribute to Hunger. It does nothing and
   taps you out.
```

**After**
```
 - That row reads "at 0 this does nothing": never cast Tribute to Hunger. It does nothing and
   taps you out. On the two-row menu where it is the only cast row, the answer is the OTHER row -
   and check your CHOICE number against your own PLAN sentence before you send it.
 - "Cast Idyllic Tutor" carries "[finds only an enchantment card - every enchantment left in your
   library is a copy of one you already control or hold: ...]": the Tutor is dead this window. Skip
   it and take the next Rule #7 entry that appears.
 - A "Cast Sanguine Bond" or "Cast Exquisite Blood" row carries "[second copy: you already control
   <that same name>; ...]": that name is already on your battlefield line, so this is not Rule #7
   entry 1. Skip it, even when the only other row is "Cast nothing right now" - then that is your
   answer.
```

## D91b
**Finding.** #168 re-key. THREE-NUMBER AUDIT for the Lantern second copy: old string `[second copy: you already control Chromatic Lantern; both stay on the battlefield - no legend rule]` **0 renders**; new partial form (`..., but the effect it gives your OTHER permanents is already on - this copy adds only its own abilities`) **6 renders**; **0 takes** (was 5 takes of 5 last corpus, under the old string). The guide quoted the retired string in two places.

**Before**
```
   4 or less. A combo piece castable on the same menu, or a Lantern already out (its row reads
   "[second copy: you already control Chromatic Lantern; both stay on the battlefield - no legend
   rule]"): take the piece, or the wall. That tag says the copy is LEGAL, never that it is worth
   casting - a second Lantern is a bare {3} mana rock and this deck is never short of generic mana.
```

**After**
```
   4 or less. A combo piece castable on the same menu, or a Lantern already out (its row now reads
   "[second copy: you already control Chromatic Lantern; both stay on the battlefield - no legend
   rule, but the effect it gives your OTHER permanents is already on - this copy adds only its own
   abilities]"): take the piece, or the wall. Six such rows last corpus and you declined all six -
   keep it that way; a second Lantern is a bare {3} mana rock and this deck is never short of
   generic mana.
```

## D91c
**Finding.** #177: the `cast the Tutor` bullet contradicted D85 until it named the live-half form of the clause.

**Before**
```
 - One enchantment down, the other not in hand, "Cast Idyllic Tutor {2}{w}" listed: cast the Tutor
   and take the missing one.
```

**After**
```
 - One enchantment down, the other not in hand, "Cast Idyllic Tutor {2}{w}" listed and its row
   reads "still in your library, not on your battlefield or in your hand: <name>": cast the Tutor
   and take that name.
```

## D91d
**Finding.** Second occurrence of the retired Lantern literal, inside Rule #7 entry 7.

**Before**
```
game; the row now prints "[second copy: you already control Chromatic Lantern; both stay on
    the battlefield - no legend rule]". THAT TAG IS NOT PERMISSION.
```

**After**
```
game; the row now prints "[second copy: you already control Chromatic Lantern; both stay on
    the battlefield - no legend rule, but the effect it gives your OTHER permanents is already on -
    this copy adds only its own abilities]". THAT TAG IS NOT PERMISSION.
```

## D91e
**Finding.** Evidence refresh for the same entry: 6 declines of 6, including three windows in `vs162` (t11/t13/t15 at 18/10/1 life) where the second Lantern was the ONLY cast row on the menu and declining was right.

**Before**
```
    Read the NAME on your battlefield line: once "Chromatic Lantern" is printed there, this entry is
    finished for the rest of the game whatever tag the row carries.
```

**After**
```
    Read the NAME on your battlefield line: once "Chromatic Lantern" is printed there, this entry is
    finished for the rest of the game whatever tag the row carries. Last corpus, six such rows and
    six declines - including three at 18, 10 and 1 life in a game where it was the ONLY cast row on
    the menu. That was right: an unwinnable window is not a reason to spend three mana.
```
