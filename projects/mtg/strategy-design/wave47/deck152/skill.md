# Strategy-writing skill proposals from the deck152 seat (wave 47)

---

## PROPOSAL S-3: a prediction of the form "the reply will name N" is not a usable observable.
Score behaviour, not vocabulary.
**Evidence:** wave-46 prediction P7 had two halves. The behavioural half - *deck152 answers
`BLOCKS: none` while the header's "you would be at N" is 9 or less: 0* - **PASSED, 0 of 4**. The
vocabulary half - *every deck152 blocks reply names the value of N* - **failed 0 of 4**: not one
of the four replies contains the number, and yet every one of the four block assignments was
correct (including the arithmetically forced one at `146` seq 45 and the correct refusal at
`vs146` seq 38 where the only offer was a `your blocker dies, attacker lives` row).
The model reads the header and acts on it without quoting it. A reviewer who had written only the
vocabulary half would have recorded a total failure of a rule that was working perfectly.
**Proposed amendment text:** *Predictions and rules must be scored on the CHOICE the model made,
not on words in its reply. "The reply names the number / states the rule / cites the entry" is
evidence when it appears and is NOT evidence of failure when it does not - the model routinely
acts on a surface it never quotes. Where a written token really is wanted, make emitting it part
of the RULE (an instruction the model is told to obey) rather than part of the PREDICTION (a
measurement of a rule about something else), and score the two separately.*
**Caveat, deliberately kept:** this cuts against wave-47 edits 146-B and 146-C, which both
DEMAND written work. That is consistent: those are rules that require the token, and they will be
scored on whether the token appears AND on whether the underlying choice improved - two numbers,
reported separately, next wave.

## PROPOSAL S-4: the carried PLAN block is a decision input, and every guide rule that a stale
plan can override needs the same refusal RULE -1 already carries for mulligans.
**Evidence:** lane L removed the carried plan from PREGAME asks (R9), and the pregame result is
the cleanest in the project's history - **8 mulligans corpus-wide, every one of them a single
look, zero chains** (wave 46 had a 3-chain and two 6-chains). But the same mechanism is alive one
layer down: at `152 vs146` seq 18 the block reads `YOUR PLAN (as you last stated it): Cast
Tovolar's Huntmaster. Attack with Hydra and Katilda.` and the reply is that sentence again, sending
two 1/1 CARDS into a tag that says `your attacker dies, their blocker lives` on both of them.
The plan was written before the tag existed.
**Proposed amendment text:** *Wherever a guide states a per-window STOP that is computed fresh by
the engine (attacker tags, blocker rows, target legality), the STOP must explicitly outrank the
carried PLAN block, in one sentence, at the STOP - not in a general "trust the render" preamble.
The plan is the model's own older reasoning re-presented as context; it competes with the render
on equal footing unless a rule says it does not.*
**Falsifiable:** zero deck152 attacks next corpus in which the reply restates a carried plan naming
an attacker whose current A-line reads "your attacker dies, their blocker lives".
