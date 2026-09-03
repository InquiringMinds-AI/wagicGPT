# Strategy-writing skill proposals from the deck152 seat (wave 54)

## S4 - WHEN A CORRECTLY-KEYED RULE BREAKS ANYWAY, THE MISSING TEXT IS THE FAILURE SHAPE
Amendment #181 gates restatement on whether a rule protected anything. This wave gives the case
one rung past that: a rule that RENDERS, is keyed to the engine's exact literal, is stated twice
in the guide, and still breaks - because the guide states the AFFORDANCE and the pilot's wrong
answer is not the affordance's negation.
`152v146` s58: two menace attackers, two blockers. The guide says "several of YOUR blockers may
be assigned to the SAME attacker ... two different B-numbers on one A-number is the answer",
keyed verbatim to `no 1-on-1 result exists - only a block by TWO OR MORE of your creatures
counts`. The reply was `BLOCKS: B1:A3, B2:A4` - one blocker on each menace attacker. That answer
does not contradict the affordance sentence; it simply never reaches it. Both assignments were
illegal, the whole line was discarded (`all_assignments_illegal`, the corpus's only one), and
the engine blocked for the seat.
**Proposal:** when a corpus shows a break under a rule that already renders and is already keyed,
the edit that is allowed is a recognise->do entry naming the WRONG ANSWER and what it costs -
never a third statement of the right one. Restating an affordance a pilot never disputed is the
#181 failure with the sign flipped.

## S5 - AN ABSOLUTE WRITTEN BEFORE A RENDER CLAUSE EXISTED IS OWED AN ARITHMETIC REWRITE, NOT A FOURTH RESTATEMENT
deck152's attack rule said "never for a creature whose tag contains even one entry reading
'(you kill it, your attacker lives)'". This corpus the engine prints the price INSIDE that same
parenthesis - `(you kill it, your attacker lives (blocking trigger: they gain 3, and their
converter takes 3 off you))` - and under a life-loop converter that makes the surplus attack a
wash, not a gain. Scored against the old absolute the seat "failed" 3 windows / 8 A-lines; it
won the game 20/-10 having never dropped below 20 life.
**Proposal:** when a lane adds a NUMBER to a clause a guide rule treats as a binary flag, the
rule is rewritten as arithmetic over that number in the same wave. A "FAIL" scored against a
binary rule whose render has since become quantitative is reported as a rule-staleness verdict,
not a pilot break - and the seat file must say which of the two it is before the number goes in
a prediction table.

## S6 - A CLOSED LANE'S "WHAT THIS COST" RECITAL IS THE BYTE BUDGET'S NATURAL DONOR
Pool guides run 39.6-74.7 KB and every wave adds. deck162's first-Mine lane passed both halves of
its prediction this corpus (untagged declines 1 of 4, and that one a punisher collision; tagged
takes at K of 0: 0 of 1). Its eleven-window recital had nothing left to buy, and retiring it paid
for the two new findings in that guide almost exactly.
**Proposal:** make retirement mechanical - a WHAT-THIS-COST passage whose prediction has passed
both halves in a corpus is collapsed to one clause or deleted in the SAME wave, and the seat file
lists it under the edit that spent the bytes. The rule it justifies always stays.
