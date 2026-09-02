# deck146 -> strategy-writing skill: proposals from wave-52 evidence

Only what my corpus supports. Three proposals; the first is the load-bearing one.

## S1 - A TEST NAMED UNDER ONE BRANCH GETS APPLIED TO THE NEIGHBOURING BRANCHES
**Evidence (4 windows, two guides, one wave).** deck146's mulligan STEP 2 names three ships and
puts the engine's `"Playing every land in this hand would not cover any spell in it"` line inside
the ONE-LAND ship only. deck152's MULLIGAN section does the same under `- ONE LAND:`. Across my
seats, four of six mulligans taken were on hands with **two or three** lands, every one under
that header line: `146v123` s1 (2 lands), `146v162` s1 (2 lands), `146v126` s2 (3 lands, at
(keeping 6)), `152v126` s1 (2 lands). Both guides also contain an explicit keep floor for those
hands ("KEEP a 7 or a 6 with 2-5 lands", "TWO OR MORE LANDS ... KEEP. That is the floor and it is
not a close call") and both floors lost to the coverage sentence.

**Proposal.** When a guide names an ENGINE-PRINTED STRING as a test, the skill should require the
sentence to carry its own scope **and** the negative case, in the same sentence, not in a
neighbouring branch: not *"ONE land under 'would not cover any spell in it' ships"* but *"ONE
land under 'would not cover any spell in it' ships; at two or more lands that line is not a
ship"*. A printed literal is a strong attractor - it is the most concrete thing on the screen -
and a branch heading two lines above it is not strong enough to fence it. Rule of thumb: **any
guide sentence quoting an engine literal states what the literal does NOT decide.**

## S2 - AN ABSOLUTE THAT DEMANDS A FIXED SENTENCE IS ~25% OBSERVED, AND OBSERVED WHERE IT MATTERS
**Evidence.** Wave 51 added to deck146: *"when you answer 'Cast nothing right now', your reply
names the highest entry that was on this menu ... in the shape 'nothing right now: entry <number>
<card name> was on this menu; it did not match because <a fact printed on this screen>'"*, plus
*"A bare CHOICE line with no such sentence is the same"*. This corpus: **20 of 84** own-turn
`Cast nothing` answers carried it. But the DECISION the ceremony was protecting did not break at
all - 0 misroutes in 21 windows with a live cast row - and the ceremony fired in exactly the
windows where it had work to do (`146v125` s138/s140, full form, correct verdicts).

**Proposal.** The skill should distinguish two things it currently treats alike: a **routing
rule** (which row to take) and a **reporting ritual** (write this sentence). At this tier the
routing rule is followed and the ritual is not, so a ritual is worth its tokens only when it is
scoped to the windows where it changes an answer. Recommended shape: gate the ritual on an
observable in the prompt ("AND the menu holds at least one other 'Cast ...' row"), never on the
answer alone. Corollary: a ritual restated a fourth time is not the fix - `#138`'s no-fourth-
restatement rule already covered this and the wave-51 addition was a first statement, so the 24%
is the ritual's natural rate, not decay.

## S3 - WHEN A DECK-SPECIFIC ABSOLUTE IS RIGHT IN ONE GUIDE AND ABSENT IN A SIBLING, PORT IT
**Evidence.** deck162's guide carries *"NO 'their untapped blockers' TAG AT ALL ON A LINE MEANS
NOTHING THEY CONTROL CAN BLOCK IT ... the answer is SEND, every time, at every life total"* and
deck162 sent 2 of 2 such attackers. deck146's guide had no such line and missed 1 of 39
(`146v125` s107). deck152's guide had no such line and missed 5 of 19 - all five inside a rule
(the converter section) whose text reads as a general prohibition. The empty tag is a
cross-deck engine literal, not a deck fact.

**Proposal.** The skill should keep a short list of **engine-literal absolutes that belong in
every guide** - the empty blocker tag, the `[repeat:`/`[Upkeep offer:` pass forms, the
`[legendary:` non-body, `{this hits YOUR permanent}` - and treat their absence from a guide as a
defect found at review time, not a deck choice. They are statements about what the render means,
and the render is the same for all six seats.
