# deck125 - skill proposals (wave 51)

S1 (#102 exhaustion reached on a MANA gate - the plan-text half failed twice; route to a row
clause that already exists on the ability path). The tap-out gate's own sentence was in the plan
line of both breaking replies (`vs152` seq 24, `vs162` seq 24: `plan_echo_count` 5 / 4) and the
Staff was cast at `leaves 1` with a 3-cost counter in hand anyway. Lane E already prints `{spends K
of your M untapped mana sources this turn; <card> in your hand needs K}` on ABILITY rows (`vs130`
seq 8). Proposed amendment: when a guide's gate is a comparison of two printed numbers (row's
`leaves N` vs a hand card's cost) and the plan-text metric fails, the remedy is the SAME clause the
engine already emits on the sibling row family - a gate the emitter can compute is moved onto the
row, not restated; the reviewer names the existing clause and the row family that lacks it (G1).

S2 (a derived exception is re-tested against the deck's FULL card list before it ships). W22's
"Scatter counts only against a creature deck" was derived from one unpunished break; deck162 runs
Master of the Feast and Fate Unraveler, and the exception cost `vs162`. Proposed rule: an exception
that narrows a safety gate by opponent behaviour ("only when they have shown X") is checked
against every pool deck's primitive list for X before it ships (#145's grep, applied to the
opponent side); if any pool deck runs X the exception does not ship. Evidence: `vs162` seq 10-13.

S3 (a size rule for a scalable spell carries its life-critical branch). "X is always 3" was
written from discard-pile evidence and had no branch for the turn the spell is the seat's life
total (`vs152` seq 39: X=7 offered at 1 life, not taken). Proposed: any rule fixing the size of a
scalable effect states the one board (life <= the printed power facing it) on which the size is
the largest offered, keyed to the header number.

S4 (the #143 metric needs its class word). 3/15 PUT lines named a card the PLAN mentioned, but
two of the three mentioned it as a RULE ("do not cast Supreme Verdict unless ...") not as a play.
Proposed refinement: the metric counts a PUT card the PLAN names as an ACTION (cast/play/activate),
and the reviewer reports both numbers.
