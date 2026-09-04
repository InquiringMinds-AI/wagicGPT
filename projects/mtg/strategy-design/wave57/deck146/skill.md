# deck146 -> strategy-writing skill: proposals from wave-57 evidence only

## S1 - A RULE'S OWN REMOVAL CONDITION IS A COMMITMENT, AND THE REVIEWER MUST CHECK IT FIRST

Evidence: deck146's guide carried *"REMOVE THIS RULE when the mulligan header's land number already
counts those three names"*. Wave 57 met that condition (D2, 6 of 6 prompts), and had I not read the
line I would have shipped a rule instructing the pilot to add one to a number that already counts
it - a guaranteed off-by-one on every mulligan. deck152's Flip Side paragraph carried the same kind
of condition and it too came due this wave (0 rows corpus-wide).

Proposal: make "scan every REMOVE-WHEN / RE-VERIFY line in your decks' guides and adjudicate it
before writing anything new" an explicit first step of the seat method, alongside the prediction
sets. Two of my three decks had one come due in a single wave, and both would have become active
misinformation.

## S2 - WHEN A RULE'S KEY MEASURES SOMETHING THE PILOT CANNOT ACT ON, RE-KEY IT; DO NOT SHARPEN IT

Evidence: deck152's 152-B was written against `[you declined this exact list N times already this
turn]`. It failed 66 of 73 this wave - but 0 of the 66 windows had an unchanged situation block, so
the row the rule pushes would have been re-opened by the very next change and bought nothing. A
sharper 152-B would have driven the pilot to pass live windows for no gain.

Proposal: add a step to the seat method between "count the key" and "write the edit" - **check what
the rule could have bought if obeyed.** If the answer is nothing, the finding is a render item, not
a guide edit, and the rule's evidence paragraph must be deleted rather than restated. The skill
currently teaches to cite what a break COST; it should equally require citing what obedience would
have PAID, because a rule with a zero ceiling is worse than no rule (it spends the pilot's attention
and it teaches distrust of a true surface).

## S3 - A BRAKE THE MODEL RESTATES AND VIOLATES IN THE SAME REPLY IS A BINDING PROBLEM, AND PROSE IS THE WRONG SHAPE

Evidence: `162v125` seq 57. The reply reads *"With no converters on the battlefield, we cannot
safely stack more draw engines ... We must wait for a converter"* and its CHOICE is a draw engine.
Four more like it in the same game. The rule is 40 lines of correct prose with the right literal in
it; the model knows it and does not bind it to the row it is naming.

Proposal: for this model tier, express a brake as a **last-pass check on the text of the row already
chosen**, with an explicit "your answer is wrong, change it" verdict, rather than as a condition to
be carried through the deliberation. The re-shaped 162-A is written that way. If it still fails in
wave 58 the honest conclusion is that no prose brake binds at this tier and the item belongs to the
core loop (a row-level annotation), which is a finding the skill should be able to reach.

## S4 - "WHAT THIS COST" NUMBERS AGE FASTER THAN THE RULE THEY DEFEND

Evidence: deck152's HOLD paragraph carried three stacked scoreboards (wave 55's 190 windows, wave
56's 33 of 35, wave 56's "2 of 35 - the rule is working"). All three were about a key that turned
out to be invalid, and together they were ~10 lines defending a null. deck146's SCAN list carried a
wave-55 anecdote one screen above a fresher, seq-cited one saying the same thing.

Proposal: **one evidence paragraph per rule, always the newest, always seq-cited.** A superseded
scoreboard should be deleted by the reviewer who supersedes it, not appended to. This is also the
cheapest source of bytes when a deck is near the top of the pool band - my three edits this wave
cost 2.0 KB net on deck146 and 20 bytes on deck152 precisely because the retirements paid for them.
