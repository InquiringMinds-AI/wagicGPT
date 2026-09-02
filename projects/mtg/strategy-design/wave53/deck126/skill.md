# Strategy-writing skill proposals from the deck126 seat (wave 53)

Only what this seat's evidence supports.

## S6 - THE FIX FOR A "MISSING BRANCH" FAILURE IS OFTEN A NEGATIVE SENTENCE, NOT ANOTHER BRANCH

deck126's Rule #2 listed five ways to press the win button. In `vs146` seq 21 the pilot invented a
sixth: it wrote that an Overgrown Battlement's block would gain 2 life and start the loop. The rule
was not missing a branch - it was missing the sentence that says the other walls gain nothing
(`mtg.txt:83596`: Overgrown Battlement has no life clause; only Perimeter Captain and Pride Guardian
do). A list of what works reads, to this tier, as an incomplete list of what works.

Proposed rule for the skill: **wherever a guide lists the members of a mechanical class, name at
least one plausible non-member and say what it does instead.** The negative is what stops the pilot
generalising the class outward under pressure. This is the same shape as the existing
"the thought X is ALWAYS FALSE" device, applied to card lists rather than to plans.

## S7 - WHEN A REPLY'S PLAN CONTRADICTS ITS OWN CHOICE, THE GUIDE SENTENCE THAT FIXES IT IS ABOUT THE
FORMAT, NOT ABOUT THE CARD

`vs125` seq 73 and seq 74: two consecutive replies whose PLAN lines say "Avoid casting Tribute to
Hunger as there are no creatures to target" and "The opponent has no creatures, so Tribute to Hunger
does nothing. Pass the turn.", and whose CHOICE lines both read `CHOICE: 1 (Cast Tribute to Hunger)`
on a two-row menu whose row 2 was the decline. The card rule was known, stated, and being recited at
the moment it was broken. No card-level sentence could have helped.

What the guide can carry is a **format check**: before sending, read the number you wrote against the
sentence you are about to write after it. I added this to D84. Proposed rule: **when the corpus shows
a decision whose prose is right and whose coded answer is wrong, the guide edit belongs in the
answer-format section, and the detector belongs in the engine list** (this seat's MED #4 - lane J's
`decision_reversed_in_prose` reads prose BEFORE the CHOICE line and cannot see a PLAN line after it).

## S8 (supports #168 with a measured result) - A RENDER RE-KEY CAN CLOSE A RULE THE GUIDE COULD NOT

The cleanest result on either of my seats needed no guide text at all. Wave 52 recorded a rule that
went from 9-of-9 declines to 0-of-5 when its recognition string was retired underneath it (the
Chromatic Lantern second copy). This corpus lane P printed a *partial-redundancy* tag that states the
consequence - `..., but the effect it gives your OTHER permanents is already on - this copy adds only
its own abilities` - and the seat declined **6 of 6**, including three windows where the second
Lantern was the only cast row on the menu at 18, 10 and 1 life.

Proposed rule: **when a tag answers a rules question the pilot did not ask, the fix is to make the
tag answer the question the pilot IS asking ("does this copy DO anything?"), and the guide sentence
then only has to point at it.** Worth stating in the skill because it inverts the usual direction: the
cheapest guide edit is sometimes a render change plus a two-word pointer.

## S9 - REPORT A CEILING'S BREAK WITH ITS DENOMINATOR AND ITS TURN CLUSTERING, NOT JUST ITS COUNT

deck126's zero-stop broke "2 of 39" - which reads like a 5% slip. Both breaks are in the **same turn
of the same game**, two windows apart, at the same life total, off byte-identical menus. That is one
failure with two records, and a guide edit written for a 5% slip would be the wrong size. Proposed
addition to the seat-file conventions the skill describes: **when reporting a break rate, say how
many distinct turns the breaks occupy.** Wave-52 seat files already do this informally ("28
consecutive windows"); making it a convention would have changed how I sized D84.
