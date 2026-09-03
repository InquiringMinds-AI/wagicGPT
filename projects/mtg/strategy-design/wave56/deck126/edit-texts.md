# deck126 (Sanguine Blood) - wave-56 guide edits, before -> after

Base: live guide `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` (60,384 B).
Result: `wave56/deck126/strategy.txt` (**65,461 B**, inside the 41-71 KB pool band).
Eight additions, one compression; net +5,077 B.
Corpus `matchups-20260903-120952`, deck126 seat logs (6 games, 501 decisions).
**deck126 remains the zero-HOLD-text control (skill #233): the string "Hold priority", the
declined-note and the hold row are still absent from this guide - 0 occurrences, verified.**

---

## D-E1 - do not cast a second copy into your own unresolved first copy (RULE #4)

**Finding, 3 of 10 Tribute casts.** `126v146` **seq 18 -> seq 19** (t13, 20 life): seq 18 cast
Tribute at `{right now: they control 1 creature - Silverquill Silencer (3/2) is sacrificed, you
gain 2}`; seq 19's prompt then reads
`ON THE STACK ... 1 (top): ability: Tribute to Hunger's gain life equal to its toughness (aimed at
Silverquill Silencer) [from your Tribute to Hunger]` **and the row still prints the identical
verdict**, so the seat cast its SECOND Tribute at a creature its first had already eaten. Same
shape at `126v130` seqs 28 and 29 (Dwarven Blastminer, t15) and `126v125` seqs 261-262 (Emrakul).
The deck runs two Tributes; in the Silencer game it had none left at t25 when a nine-attacker
board arrived and it lost -10/10. STRATEGY on the guide side (the ON THE STACK section names the
answer already given), with a MED render item in the seat file for the verdict itself.

**Before** - Rule #4 ended `Otherwise hold it. It is an instant, it stays castable on their turn,
and it will be offered again.`
**After** - the same, plus `ONE TRIBUTE ANSWERS ONE CREATURE - AND THE SECOND WINDOW OF THE SAME
TURN IS NOT A SECOND CREATURE`, instructing a read of ON THE STACK before every Tribute row.
A mirror line was added to SITUATIONS.

## D-E2 - a DEAD IDYLLIC TUTOR is the third row that is not a cast row (RULE #2)

**Finding, 6 breaks in one game.** `126v125` (won 32/-8 but the menu discipline collapsed):
**seq 15** (t18, 20 life) took `Cast Idyllic Tutor {2}{w} [finds only an enchantment card - every
enchantment left in your library is a copy of one you already control or hold: Sanguine Bond]`
while `Cast Overgrown Battlement {1}{g}` - a live entry-4 row - sat on the same menu; **seq 30**
and **seq 32** (t22) took the same dead-Tutor row off menus whose only other Cast row was a
Tribute at `at 0 this does nothing`; **seq 46** (t24) took
`Cast Sanguine Bond {3}{b}{b} [second copy: you already control Sanguine Bond ...]`; **seq 167**
(t42, 12 life) and **seq 215** (t50, 8 life) took `Cast Chromatic Lantern {3} [second copy: ...]`.
Every one is the pull of Rule #2's absolute ("Cast nothing right now is NEVER your answer to a
menu that prints any Cast row"), whose exclusion list had two entries and needed three.
Control case: `126v152` **seq 28/29** (t20) - a lone dead-Tutor row, and the seat correctly
answered `Cast nothing right now`. Same shape, 1 right / 3 wrong.

**Before** - `TWO ROWS ARE NOT "A CAST ROW" FOR THIS LINE, AND BOTH ARE PRINTED SO YOU CAN SEE IT:`
(zero-Tribute, `[second copy:`).
**After** - `THREE ROWS ARE NOT "A CAST ROW" FOR THIS LINE, AND ALL THREE SAY SO IN THEIR OWN
TEXT:` as a three-item list adding the dead-Tutor clause, then
`WHEN EVERY Cast ROW ON THE MENU IS ONE OF THEM, THE MENU IS EMPTY` with the 1-right/6-wrong cite.

## D-E3 - with a converter of YOURS out, STEP 1 is off and every blocker blocks (RULE #5)

**Finding.** `126v146` **seq 37** (t25, 20 life). Battlefield line printed `Sanguine Bond {3}{b}{b}
[enchantment]`; the prompt printed
`LIFE-TO-DAMAGE CONVERTER on the battlefield: yours - Sanguine Bond` and
`INCOMING THIS COMBAT: 4 attackers, 10 unblocked damage - you would be at 10`; the one blocker's
B-line read `B1. Perimeter Captain (0/4) ... may block A3, A4 (all: your blocker dies, attacker
lives) {blocking trigger, this combat: you may gain 2, and if you do your converter takes 2 off
them}`. Answer: `no blockers`. That forfeited 2 damage stopped, 2 life and 2 off their total; the
seat lost -10 against **10**. Half-break two turns earlier at **seq 30** (t19): two blockers
available, one assigned, against a single 3/3 both survive. `{blocking trigger, this combat: you
may gain N, and if you do your converter takes N off them}` is a NEW wave-56 literal (lane B) -
45 rows at this seat - and the guide did not name it.

**Before** - STEP 1's bullet `THE PARENTHESIS NOW CARRIES THE LIFE GAIN INSIDE IT, AND THE DEATH
HALF STILL COMES FIRST.`
**After** - a new first bullet
`ONCE A CONVERTER OF YOURS IS ON YOUR BATTLEFIELD LINE, STEP 1 IS OFF AND EVERY BLOCKER BLOCKS`,
quoting the new clause and citing seq 37; the old bullet is kept, re-headed
`WITH NO CONVERTER OF YOURS OUT`, so the no-enchantment case is unchanged.

## D-E4 - at a "this KILLS you" header, only damage stopped matters (RULE #5)

**Finding.** `126v146` **seq 41** (t27, 8 life):
`INCOMING THIS COMBAT: 9 attackers, 26 unblocked damage - you would be at -18; this KILLS you`.
Two blockers, nine attackers; A1 Barrowin `deals 4`, A4 Nadaar `deals 4`, A9 Silencer `deals 4`
all with `may block` rows on both B-lines. The seat answered
`Perimeter Captain blocks Goblin; Wall of Omens blocks Goblin` - the two `deals 2` rows, chosen
because their parenthesis read `(neither dies)`. It stopped 4 where 8 was available. The game was
lost either way, but the rule is the one that will matter at a survivable header. `; this KILLS
you` is a NEW wave-56 literal - 3 renders at this seat, 0 maximised.

**Before** - RULE #5's `When every attacker offered to that blocker kills it, ...` paragraph.
**After** - a new paragraph inserted above it,
`WHEN THE HEADER SAYS "this KILLS you", NOTHING ABOVE APPLIES AND ONLY ONE NUMBER DOES`, with the
seq 41 cite. Mirrored in SITUATIONS.

## D-E5 - Rule 7b fires in the FIRST window (RULE #7 entry 7b)

**Finding.** 13 `they control 1 creature - <name> is sacrificed` rows, 8 taken. `126v130` t15
printed `Dwarven Blastminer (1/1) is sacrificed, you gain 1` four times (seqs 24, 26, 28, 29): the
first two were declined for `Cast nothing right now`, the third and fourth taken. `126v162` seq 13
`Fate Unraveler (3/4) is sacrificed, you gain 4` was taken in its first window - that is the shape
the rule wants. Primitive check: `mtg.txt` Dwarven Blastminer's repeating land destruction is why
a 1/1 is 7b material.

**Before** - 7b ended `... and whether or not you could also pay for the enchantment.`
**After** - the same, plus `IN THE FIRST WINDOW THAT PRINTS THE ROW, not the third`, with both
cites and the "printed power does not measure it" sentence.

## D-E6 - the defender-victim Tribute stop, second cite (RULE #7 entry 7b tail)

**Finding.** `126v162` seqs 8 and 10 (t7, t9, 20 life) correctly declined
`{right now: they control 1 creature - Shield Sphere (0/6) [defender] is sacrificed, you gain 6}`;
**seq 12** (t9, 19 life) took it. seq 13 then correctly ate the Fate Unraveler with the second
copy, and there was no third when Master of the Feast arrived. Two corpora, two Shield Spheres,
two lost games. D109's prediction "0" **FAILS 1 of 3**.

**Before** - `... unless (a) applies or both enchantments are out.`
**After** - the same, plus the 20/20/19 sequence and `the second Tribute you then had to spend on
the real threat was your last one`.

## D-E7 - one Vampire, one blocker that kills it, only the Bond out: keep it home (RULE #6)

**Finding.** `126v146` **seq 29** (t18, 25 life, Sanguine Bond out, Exquisite Blood not):
`A1. Vampire (1/1) [lifelink] [their untapped blockers: Barrowin of Clan Undurr (3/3) (your
attacker dies, their blocker lives ...)]`, sent. It bought 1 point of drain and cost the board its
only lifelink body; at **seq 37** (t25) the seat had one blocker left and answered `no blockers`
(D-E3), and it was dead by t27. Attackers windows this corpus: 11, all sends; this is the only
one the tag argued against. The win-button exception (both enchantments out) is preserved.

**Before** - Rule #6's second bullet began `- N is 1 and no GANG BLOCK clause: COUNT YOUR OWN
A-LINES FIRST.`
**After** - a new bullet above it for the one-Vampire / attacker-dies / Bond-only case.

---

## Compression

- **the second-Chromatic-Lantern paragraph** (-1,000 B): the two breaks this corpus are now
  covered structurally by Rule #2's three-row list (D-E2), so the paragraph keeps its rule, its
  "THAT TAG IS NOT PERMISSION" sentence and the six-declines cite, and loses the re-argument.

## D-E8 - the Idyllic Tutor pick reads YOUR OWN battlefield line, every time (RULE #3 step 4)

**Finding.** 5 reveals, **4 correct, 1 break**. `126v125` **seq 33**: the battlefield line printed
`Sanguine Bond {3}{b}{b} [enchantment]` and the reveal picked **Sanguine Bond**. Two turns later,
seq 46 took `Cast Sanguine Bond {3}{b}{b} [second copy: you already control Sanguine Bond ...]` -
so the bad search and the bad cast are one defect seen twice. The other four picks (`126v130`
seq 11, `126v162` seq 9, `126v146` seq 9 and seq 25) were all "neither half" looks and all
correctly took Sanguine Bond per Rule #3's split.

**Before** - `... Then you got ten of ten right in one corpus, including both "neither half"
looks. This step is at its ceiling: read the two lines, name the other card, and do not
re-derive it.`
**After** - the running record (10/10, 5/5, then 4/5) plus the seq 33 -> seq 46 chain and
`THE LINE YOU READ IS YOUR OWN BATTLEFIELD LINE, EVERY TIME, INCLUDING THE FIFTH SEARCH`.
The "at its ceiling" claim is withdrawn - it is no longer true.
