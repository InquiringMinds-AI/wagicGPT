# deck123 -> strategy-writing skill proposals (wave 55)

Only what this seat's twelve logs support. Each is stated as the rule I would want a future
reviewer to apply, with the evidence that produced it and the counter-case that limits it.

## S1. A GUIDE SENTENCE THAT NARRATES A CORPUS BECOMES FALSE, AND FALSE NARRATION IS WORSE THAN NO NARRATION - BECAUSE THE PILOT ACTS ON IT.
`deck130`'s X-menu STEP 1 carried, in the live guide, the sentence "THE MARKER IS OFTEN NOT THERE.
It is printed only when some X kills something of theirs and nothing of yours; across a whole
corpus of twelve games it appeared ZERO times. Do not wait for it and do not read its absence as
'no row is good'." That sentence was TRUE when wave 54 wrote it and is FALSE now: the marker
rendered on **3 of 3** X menus this corpus. And it did not fail silently - at `130v146` s40 the
pilot answered `X = 3` with the marker sitting on `X = 1 {same kills as X=3, for 2 less mana}` two
rows below, which is exactly what a reader who has been told "do not wait for the marker" does.
The same pattern is visible at deck123 in a milder form: RULE -1 opened "Two of six games were
decided here", which was true of wave 53 and describes nothing in this corpus (ten looks, zero
games turned).
**Proposed rule.** A guide may cite a count as EVIDENCE for a rule ("this deck cast it at 0
creatures four times"), but it may not state a count as a FACT ABOUT THE RENDER ("the marker
appeared ZERO times", "this rendered 0 of 42 logs"). Facts about the render belong in the seat
file's literal audit, which is re-derived every wave; a guide is read by a pilot who cannot check
it. Where a reviewer wants to warn that a surface may be absent, the wording is conditional and
carries its own escape - "if no marker is printed, then ...". The mechanical check: grep the
finished guide for a numeral adjacent to a render string, and for the words "never printed",
"zero times", "not there", "does not exist"; each hit is either deleted or turned into a
conditional.
**Limit.** This is NOT the trust doctrine. Nothing here licenses hedging against a surface that IS
printed; it forbids the guide asserting that a surface is not.

## S2. WHEN A RENDER ITEM CLOSES, THE GUIDE PARAGRAPH BUILT ON ITS ABSENCE MUST BE RE-KEYED IN THE SAME WAVE, AND THE REVIEWER WHO CLOSED IT IS THE ONE WHO KNOWS.
Three wave-54 render items closed at my seats this corpus and every one of them had a guide
paragraph shaped around its absence:
- the X marker (deck130 STEP 1, above);
- fetch rows printing no colour, wave-54 E7/G4 - now `and it adds {W} or {B} (you cannot make {W}
  right now)` on **60 rows**, while deck123's RULE 2 still taught the pilot to derive the missing
  colour from the mana line by hand;
- single-target `{kills:}` lists not marking ownership, wave-54 E6/G8 - now
  `{kills whichever you target: THEIRS - ...; YOURS - ...}` on **71 rows**, while deck130's land
  section still warned about rows that "name your own permanent" without naming the split that now
  distinguishes them.
Each of those paragraphs was several sentences of hand-derivation replaced, this wave, by one
sentence naming the printed clause (A55-8, D55-6). The saving is real: A55-8 is shorter than what
it replaced and strictly more mechanical.
**Proposed rule.** The seat file's "Closed from wave N" list and the guide edit list are the same
pass, not two passes. For every item a reviewer closes, they state which guide paragraph was
written against its absence and either re-key it to the new string or record explicitly that no
paragraph depended on it. A closed render item with no corresponding guide line is a reviewer who
did not look.

## S3. A RULE WHOSE TRIGGER IS A NUMBER THE ROW PRINTS SHOULD NAME THE NUMBER'S POSITION IN THE STRING, NOT ITS MEANING.
deck123's RULE 3 said "if YOUR 'of which N are creatures' number is 4 or more, do not cast
Damnation at all" - a correct rule, keyed to a number on the BATTLEFIELD line. The row itself
prints the same number in a different place: `{right now: destroys 1 of their creature (1 without
a restriction against attacking), 5 of yours}`. At `123v152` s104 the pilot read the row, wrote a
plan about "saving my own 5 creatures", and did not connect that 5 to the rule two screens away
that forbids it. The rewrite (A55-1b) keys the stop to "the number after the final comma in the
row you are about to take", and the pilot then never has to hold two surfaces at once.
The same shape recurs: RULE 1's stop is keyed to `M` on the battlefield line while the repeat row
prints `[repeat: you control M creatures ...]` (23 renders); deck130's land plan was keyed to a
count of land rows while the line prints `... and N are lands`.
**Proposed rule.** When the decision is made ON a row, key the rule to the number that row prints,
in the row's own words and in its own position. A rule keyed to a header the pilot must scroll back
to is a rule with an extra step, and the extra step is where the corpus shows it breaking.

## S4. A CARD CLASS WITH NO `{right now: ...}` VERDICT IS THE CLASS THE GUIDE KEEPS FAILING TO FIX, AND THE REVIEWER SHOULD SAY SO RATHER THAN WRITE A FIFTH PARAGRAPH.
Lightning Greaves cast at `of which 0 are creatures` is now the **fifth** corpus. The live guide
attacks it in FOUR places - RULE 6, cast-list entry CL5, a DECIDING SITUATIONS bullet, and the
CL5 gloss - and it broke again (`123v130` s11). Every other repeat offender at this seat that DID
close closed when the row started carrying a verdict: the edict at N=0 (`at 0 this does nothing`)
went 1 -> 0 of 171 rows; the Tragic Slip on a dead row (`kills 0 of the N legal targets`) is 0 for
three corpora; the second Alarm (`a second copy changes nothing`) is 0 this corpus. The Greaves
row is the one that has never been priced.
**Proposed rule.** When a guide has attacked the same break in three or more separate paragraphs
across three or more corpora and it recurs, the reviewer stops adding guide text and files a
RENDER item asking for the verdict clause that the analogous, now-closed rungs have. The seat file
should state the comparison explicitly (which sibling rungs closed, and what closed them), because
that comparison is the argument. deck123 wave 55 does this at E6 and pairs it with a prediction
(P3) that names the exit: if it breaks a sixth time on a menu where the Greaves is the only cast
row, the rung belongs to the render and no further guide text is written for it.

## S5. THE PILOT'S CORRECT BEHAVIOUR INSIDE A KNOWN ENGINE DEFECT BELONGS IN THE GUIDE AS A STANDARD, NOT AS A WARNING.
`123v126` s57-s87 is 26 identical asks in one Upkeep step and `123v162` s36-s50 is fifteen in one
Draw step; across both, plus `123v125`'s four Upkeep runs, the pilot cast **nothing**. The live
guide's HOLD paragraph reads as a rebuke ("this deck took it 32 times last corpus and took it LATE
almost every time"), which was the wave-54 truth and is not this one (106 takes at deck123, taken
early). A rebuke aimed at behaviour the pilot has already corrected spends its lines telling the
model it is bad at something it is now good at, and - worse - the paragraph's remedy ("take it at
the FIRST dead window") is already being followed, so a reader who is following the rule and still
seeing the window return has nowhere to go.
**Proposed rule.** Where the corpus shows a rung HELD, the guide sentence changes register from
"this deck did X and lost" to "this deck did Y last corpus - keep doing it", and the reviewer adds
the shape of the remaining engine-side residue so the pilot recognises it as not-its-problem
(A55-7: "the row is not kept when a life total is ticking, and that is not your problem to solve;
answer the SAME way every time, and the one thing that must not happen is a cast"). This is not
praise-writing: it is the difference between a pilot that reads a run of identical windows as
evidence it has misplayed and one that reads it as noise.

## S6. TWO ROWS FOR THE SAME CARD AT TWO PRICES IS A GUIDE HOLE THAT NO SINGLE-CARD SECTION CATCHES.
`123v152` s104's menu carried `Cast Damnation {2}{b}{b}` and `Cast Damnation {4}{b}{b} [from
exile]` with byte-identical `{right now: ...}` verdicts. Nothing in the live guide - not the
Damnation rule, not the exile paragraph, not the cast list - said "prefer the cheaper row", because
each of those sections is written about ONE card and this is a fact about a MENU. The same hole
would swallow a second Talisman-shaped case at deck130 and any future flashback/adventure/exile
row in the pool.
**Proposed rule.** A guide needs a small MENU-LEVEL section (three or four lines) for facts that
are about the shape of the list rather than about any card in it: two rows naming the same card,
a row that is a strict subset of another, the collapsed-run rule the X menu already has. deck130
has one of these and it works (the `same kills as X=N` walk-down is 2 for 2 this corpus); deck123
did not, and the missing line cost the game. Filed as E5 too, because the render can also solve it
with one string.

## S7. A DECK RUNNING TWO COPIES OF A LEGENDARY CARD NEEDS A LINE FOR THE SECOND ONE, AND THE DECKLIST IS WHERE THE REVIEWER LEARNS THAT.
`130v146` s62 cast a second Rorix Bladewing off `[legendary: you already control Rorix Bladewing -
legend rule: casting this sends one copy to your graveyard (you choose which)]` - six mana for the
identical board. The information needed to predict this was available before the corpus ran:
`Res/ai/baka/deck130.txt` says `Rorix Bladewing *2` and `mtg.txt:96836` says
`type=Legendary Creature`. No reviewer had crossed those two files.
**Proposed rule.** Once per deck, the reviewer runs the decklist against the primitives for the
three structural properties that create a dead second copy - `Legendary`, a non-stacking `lord(...)`
effect, and a unique-effect enchantment - and writes a line for each one the deck runs 2+ of. It is
a mechanical pass over ~20 distinct cards and it pre-empts a whole class. (The mirror check matters
as much: deck130's four Talismans DO stack - three separate `{T}:Add` lines at `mtg.txt:117529` -
and its `[second copy:` takes are correct, 4 for 4.)
