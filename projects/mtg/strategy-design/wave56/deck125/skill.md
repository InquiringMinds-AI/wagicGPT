# deck125 -> strategy-writing skill proposals (wave 56)

Only what this seat's evidence supports. Amendment numbers continue from #261.

## P1 - A CEILING CLAIM IS SCOPED TO THE BOARDS IT WAS MEASURED ON, AND THE GUIDE MUST SAY WHICH

**Evidence.** deck125's Staff-ping line read `Otherwise it goes at the player.` and carried the
ceiling claim "keep taking it in every window it is offered, with EVERY Staff. That damage is how
the game ends." It went 78 of 78 at the face across six games - a perfect number - and eight of
those 78 lost the `vs162` game, because on that one board the thing killing the seat was option 1
on the very same menu (`125v162` seqs 87/88/108/110/137/139/164/166, Ob Nixilis at
`SURVIVES (loyalty 5, 4 left)` while `DRAW FORECAST:` read 3 life per draw step).

**Proposal (#262).** When a guide records a rule as "at its ceiling" or "N of N", the sentence must
name the BOARD CLASS the N was measured on, not just the count - "78 of 78 **on boards with no
DRAW PUNISHERS line**". A bare N/N reads to the pilot as "this rule has no exceptions", and the
denominator is exactly where a missing exception hides: a rule cannot be measured against a
carve-out that has not been written yet. This is the mirror of #188's three-number audit applied to
the GUIDE's own claims rather than to the render's literals.

## P2 - WHEN A RULE BREAKS A THIRD TIME UNDER MORE PROSE, THE PROSE IS NOT THE MISSING PART

**Evidence.** deck125's Emrakul/CONVERTER stop has grown every wave since 51 - it now runs ~25
lines with three worked anecdotes, an outranking clause and a "the fifth one included" sentence -
and it broke again this corpus at the least ambiguous board it will ever see (`125v126` seq 254:
37 life against 2, the converter line printed, two Staffs lethal next upkeep). Meanwhile every rule
in the same guide that is enforced by a PRICE ON THE ROW went to zero and stayed there: low-X
Revelation 0/86, sweeper-at-dead-header 0/124, Lightmine 0/91, cleanup land rule 0 breaks in three
corpora.

**Proposal (#263).** Add a stop condition to the skill: **a rule that has broken in three
consecutive corpora while its prose grew is evidence for a RENDER item, not for a fourth
rewrite.** The reviewer's obligation at that point is (a) file the render ask that would put the
number on the row, (b) COMPRESS the prose to the bare rule plus one cite, and (c) say in the seat
file that the guide half is now capped. Growing a section past ~20 lines has, at this seat,
correlated with breaking rather than holding.

## P3 - THE GUIDE MUST TEACH THE COLLAPSED-RANGE DECODE FOR ANY MENU IT GIVES AN ABSOLUTE ANSWER TO

**Evidence.** deck125's guide said "reply with the OPTION number whose line reads `X = 3`". This
corpus one of its three X menus was the collapsed form (`125v130` seq 72:
`2-5. X = 5 down to X = 2 - one option per X in that range, largest X first`), on which **no line
reads `X = 3`**. The model decoded the range and answered option 4 correctly - but from the
prompt's generic range paragraph, not from the guide, which would have been unanswerable as
written. `(copies k-m of n)` and `The same N options as a-b, with <source> #2` collapses appeared on
40 of my prompts.

**Proposal (#264).** Wherever a guide names a literal row text as the answer, it must also state
the answer in RANGE terms ("count into the range: option 2 is X = 5, option 3 is X = 4, option 4 is
X = 3"). Collapse is a property of the render that a guide cannot suppress, and an instruction that
only works on the uncollapsed form is a rule with a silent hole.

## P4 - A GAP IS A FINDING EVEN WHEN THE MODEL GUESSED RIGHT

**Evidence.** deck125 spent four counters on land destruction in the `vs130` matchup (Lay Waste,
Molten Rain, two Dwarven Blastminers) with **no rule in the guide covering "Destroy target land"**,
and won that game at turn 60 on fourteen lands. Under the previous wave's W103 predicate those
casts scored as BREAKS; under judgement they were all correct. The guide gained a rule this wave
only because the seat noticed the unruled cluster, not because anything failed.

**Proposal (#265).** Add to the reviewer method: after adjudicating breaks, list the decisions the
guide has **no rule for at all** - correct ones included - and write the rule that sanctions them.
An unruled correct decision is one draw away from being an unruled wrong one, and it costs one
bullet now versus one game later.
