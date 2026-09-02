# deck162 -> core-prompt / render proposals (layer-routed)

## G9 (prompt, HIGH) - the lane-I opponent-turn window is right; its cadence is not
deck162's `162v126` turn 7 is the corpus's clearest specimen of the seat file's E-2: twelve asks
in one opponent turn, all offering `1. Cast Dictate of Kruphix {1}{u}{u} ... 2. Cast nothing right
now`, all declined, **452 s** of inference for zero state change. The lane itself is a win - a
flash draw engine genuinely can be cast at the opponent's end step and this deck should be asked.
What is wrong is that the offer is repeated at Upkeep, Draw, Main 1 (x4), Combat begins,
Attackers, Combat ends, Main 2, End and Cleanup with nothing between them. If a full suppression
is too invasive, the cheapest correct cadence for a flash-only menu is **one ask per opponent
turn, at their end step** - that is where holding a flash spell is actually worth something, and
it is the only stop where the information the pilot is waiting for (what they did this turn) is
complete.

## G10 (render) - the `{feeds: ... converters on your battlefield: K - <names>}` clause is the
best emitter string this pool has, and its 0-case could carry one more word
64 rows across deck162 this corpus, K correct on every one I checked against the battlefield
line, and it is the whole arithmetic of the deck's central rule. The two breaks
(`162v130` s11, `162v123` s8) were both a **second** Howling Mine, and the row already knew that:
it printed `[second copy: you already control Howling Mine; both stay on the battlefield - no
legend rule]` in the same line as `converters on your battlefield: 0`. Proposal: when a
`{feeds:` row's K is 0 **and** the card named is already on the pilot's battlefield line, say so
inside the feeds clause rather than leaving it to the `[second copy:` tag two annotations away -
`{feeds: the opponent draws 1 extra card per turn; converters on your battlefield: 0 (nothing of
yours punishes their draws yet) - and you already control one of these}`. The two facts are one
decision; they are currently rendered as two independent annotations.

## G11 (render, positive) - `{this hits YOUR permanent}` is doing its job
23 rows across deck146 and deck162, **0 taken**, including a five-row Ob Nixilis -2 menu
(`162v126` s25) whose rows 1 and 5 pointed at the pilot's own Fate Unraveler and Shield Sphere.
Wave 51's 146-N break (a Kaya -3 taken at 1 life on an all-`[your battlefield]` list) has no
successor. Recording it so the clause is not shortened in a prompt-size pass; it is four words
and it closed a lane.

## G12 (prompt, small) - the reply format's worked example uses option 1, which is worth keeping
Noted because it is invisible when it works: `e.g. "CHOICE: 1 (Cast Master of the Feast)" (a
worked example of the format using the first option - choose the option YOU want)`. Across 161
deck162 decisions there is not one instance of the example being copied as an answer, and the
seat answered `2` or `3` far more often than `1`. No change asked.
