# Strategy-writing skill proposals from the deck123 seat, wave 56

Only what this corpus's evidence supports. Each is a proposed amendment to
`wave55/strategy-writing-skill.md`; the synthesis owns whether it lands.

## S1 (NEW). A guide cannot raise a deck above its board-presence floor - measure the floor
before you spend a wave writing rungs.
deck123 has now been 1/6 for three waves. This corpus the decisive measurement was not a misplay
census, it was one line of arithmetic over the prompts: **in its five losses deck123 controlled at
least one creature in 2 of 194 windows, and in three of those games in 0 of 156.** Every rung in a
71 KB guide is a rule about what to do with a board; there was no board for five games out of six.
**Proposal:** when a deck's win rate is flat across three or more waves, the seat's FIRST
obligation is a floor measurement of this shape - a count, over every window, of how often the
deck had the resource its own wincon needs - and the finding "the ceiling is the deck" is a
legitimate, terminal seat verdict that redirects the wave's effort to TRIMMING the guide rather
than extending it. The skill currently has no sanctioned way for a reviewer to say "there is
nothing here for me to fix", and the absence of one produces guides that grow while the deck's
result does not move.

## S2 (NEW). When a render ships the number a guide rung was teaching the pilot to derive, the
guide text must be re-keyed to the emitter's literal - and the rung's classification changes.
Wave 55's E6 asked for a `{right now: ...}` verdict on the Lightning Greaves cast row. It shipped:
62 of 64 rows now read `{right now: you control 0 creatures - this equips nothing}`. The wave-55
guide's paragraph on that rung opens "The Greaves is the one card here whose row carries NO
`{right now: ...}` verdict" - **a sentence that is now false in the guide the model is reading**,
and the break happened anyway (`123v125` s45). **Proposal:** the seat's literal audit should carry
a third column beyond old-renders / new-renders / takes - **"guide sentences that quote or assume
the OLD string"** - and any guide sentence naming a render's absence must be re-checked against the
corpus that first carries the fix. A guide that describes the render inaccurately teaches doubt of
the render by accident, which the trust doctrine forbids on purpose.

## S3 (NEW). A rule keyed to a conditional annotation is silent when the annotation does not print
- and the seat must supply the untagged branch.
RULE 2's fetch-land rule is built entirely on `(you cannot make {W} right now)`. That parenthesis
only prints for a colour the seat cannot currently make, so on a window where both colours are
available NO row carries it and the rule answers nothing. The corpus's one Marsh Flats break
(`123v126` s6) is exactly that window. **Proposal:** amend the skill's render-keyed-rule guidance -
whenever a rule's trigger is an annotation that prints CONDITIONALLY, the guide must state what to
do when it does not print, and the seat's audit must count the windows in which the trigger was
absent as a separate denominator from the windows in which it was present and ignored. This corpus
those are 1 and 4; without the split the rung reads as "4 of 5 correct" and the actual defect is
invisible.

## S4 (AMEND #208). A trim is a claim, and it needs its own falsifiable prediction.
This seat removed ~2.5 KB of worked examples from rungs passing on large denominators (Damnation
25/25, edicts 238/0, `[second copy:` 20/0, `plan_choice_conflict` 6->0). That is a bet that the
prose was carrying no load. **Proposal:** the skill should require, for every trim, a named
prediction of the form "rung X stays at N of M next corpus, and a rise falsifies the trim and
restores the text" - and the next wave's seat must adjudicate it in Set A alongside the additions.
Trims are currently invisible to the prediction machinery, so a guide can be cut badly and the
damage will be attributed to variance.

## S5 (AMEND, the LETHAL blocking rule). A "block the biggest attacker" rule needs its arithmetic
gate, or it orders plays that cannot change the outcome.
`123v152` s23: 8 life, `INCOMING THIS COMBAT: 4 attackers, 16 unblocked damage ... this KILLS you
(of that, 10 from 2 attackers none of your creatures can block)`, one blocker whose only legal
assignment removes 3. The seat answered `BLOCKS: none` and was RIGHT; the guide's LETHAL rule as
written ordered a block. **Proposal:** the skill's combat-rule pattern should require the
subtraction, not only the ordering - "assign to the largest, THEN check that the remaining total is
below your life; if it is not, the assignment is not the decision the header is asking for". The
render now supplies both numbers on the same line, so this costs one sentence.
