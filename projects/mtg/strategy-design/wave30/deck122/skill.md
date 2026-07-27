# deck122 -> strategy-writing skill proposals (wave 30)

Context: FIRST-GUIDED corpus of the campaign's first alt-win-condition deck. Record 2/6 ->
6/6. This is the **4th Step-0 -> first-guided validation arc** (after the deck59/18 shape).
The wave-29 skill proposals (alt-win-condition teach, life-as-engine resource reframe,
build-loyalty value walker, conditional-static-P/T-renders-live positive control) are now
LIVE-VALIDATED, not just proposed. Proposals below are for the synthesis agent; the load-
bearing signal is that a well-worn teach category PREDICTED the win and the corpus confirmed.

## PROPOSAL 1 (VALIDATE + PROMOTE) -- the ALT-WIN-CONDITION teach category WORKS.

Wave-29 proposed: for a deck whose engine implements an alternate win condition, the guide
MUST elevate the win line to a named first-class plan (exact threshold + "this WINS, not a
bonus" framing + a bridge from the normal game to the threshold), because the model will NOT
infer it from option/board text. Wave 30 confirms the mechanism: Step-0 had ZERO plan
mentions of the 40-life line; after the guide added guide-rule #2 ("FELIDAR SOVEREIGN CAN
JUST WIN THE GAME ... get to 40 life, survive to your next upkeep, WIN"), the model verbalized
the line in ALL 6 games (6-21 reply mentions/game) and climbed to 51 life pursuing it. The
teach demonstrably installed the plan. Promote from proposal to an ESTABLISHED skill rung
(name it the alt-win-threshold rung), citing deck122 as the live proof.

Caveat worth encoding: the teach installs PURSUIT, but converting the alt-win still needs the
enabling card in play, which the guide cannot force. Skill note: alt-win teaches should be
validated for PURSUIT (does the model plan toward the threshold?) separately from CONVERSION
(does the win actually fire?) -- conversion often depends on draw variance and belongs to a
probe-deck engine check, not the corpus. Do not read a non-converted alt-win as a failed
teach if the pursuit is present.

## PROPOSAL 2 (VALIDATE) -- the LIFE-AS-ENGINE resource reframe is a reusable pattern.

Wave-29 generalized: for any deck where a resource the model defaults to treating as a cushion
is actually the win engine, lead with an explicit identity reframe + per-threshold callouts.
Wave 30 confirms: the reframe killed the Step-0 "race own life down" loss class outright (no
recurrence in 6 games, including two aggro matchups where the model correctly held back
attackers at low life). This is now a validated cross-deck pattern (name the resource, name
that spending it is the mistake here, name the thresholds). Keep in the skill as an
established resource-reframe rung with deck122 as the worked example.

## PROPOSAL 3 (VALIDATE) -- build-loyalty value-walker teach generalizes.

Sorin, Lord of Innistrad piloted per the wave-29 teach (default +1 every turn, gate the
minuses on target/board quality): vs146 cast t15 + +1 chain t15/17/19, no -2/-6 misfire. Second
live confirmation after the wave-29 read; the loyalty teach (146-Lolth partial -> Sorin) is
solid. Codify as established, not proposal.

## PROPOSAL 4 (NEW skill note) -- the ADJUDICATION-WIN-METRIC alignment caveat.

deck122 surfaces a skill-relevant reading hazard: when a deck's win metric IS the harness
adjudication tiebreaker (a lifegain deck adjudicated on life totals), a high win-rate is
partially structural -- the deck is favored at the cap by construction. A perfecta on such a
deck must be decomposed into clean closes vs adjudication-ahead grinds before it is read as
dominance (deck122: 2 kills + 4 adj-ahead). Add to the skill's evidence-reading rung: when
auditing a record, check whether the deck's plan aligns with the adjudication rule and
discount adj wins accordingly. This protects the pipeline from over-crediting a guide when
the pool composition (grindy field, many adj-at-cap) did part of the work.
