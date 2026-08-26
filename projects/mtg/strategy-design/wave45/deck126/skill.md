# Strategy-writing skill proposals — from the deck126 seat (wave 45)

## Proposal S-E — A "YOU CAN AFFORD MORE THAN THE LINE SAYS" TEACH MUST SAY WHICH HALF OF A COST IT COVERS.

deck126's Rule #1 taught, correctly, that the `Mana available:` count under-reports an Overgrown
Battlement ("when a spell looks one or two mana out of reach and you have three or four walls out,
it is probably affordable — take it and let the engine tap for you"). The Battlement makes {G} and
only {G} (primitive `auto={T}:foreach(creature[defender]|myBattlefield) add{G}`). The teach is true
of the generic half of a cost and false of every coloured pip — and the deck's win condition costs
{3}{B}{B}. Following it, the model tapped two Battlements for ten green, could not cast Sanguine
Bond, passed, and lost the two walls' blocks; life 6 -> 1 that turn.

Proposed text: *A rule that tells the pilot a rendered mana number is a FLOOR must name which part
of a cost the extra mana pays. Generic and coloured mana are different resources and the render
counts them in different places — the total on one line, the per-source colours on another. Write
the floor for the generic part and, in the same rule, point the pilot at the per-source colour
sub-list for the pips. A ramp teach without a colour clause is a licence to tap out for nothing.*

## Proposal S-F — AN EVASION KEYWORD IS A DIFFERENT KIND OF THREAT FROM A BIG CREATURE, AND A DECK WITH ONE ANSWER TO IT NEEDS THAT NAMED.

deck126 has sixteen defenders and no flying or reach anywhere. Its guide taught blocking as the
whole defensive plan and gated its one removal spell on a LIFE TOTAL (10 or less). Against a lone
Master of the Feast (5/5 [flying]) at 13 life, with the removal spell and the combo piece both in
hand and mana for one, it cast the combo piece and wrote a plan that said "block Master of the Feast
with all walls". Dead two turns later. The life gate is a proxy for "am I losing"; the fact that
decided it was "can my board interact with this at all", which the render prints as a tag.

Proposed text: *When a deck's defence is a body type (walls, chump blockers, a single large
blocker), enumerate the keywords that body type CANNOT answer — flying, trample, menace,
unblockable, protection — and route each to the deck's actual answer, by name. Trigger the routing
on the TAG on the board line, not on a life total: a life threshold fires after the damage, and the
tag is visible before it.*

## Proposal S-G — WHEN A PRE-REGISTERED PREDICTION FAILS AND THE DEVIATIONS WERE RIGHT, THE RULE IS THE DEFECT.

D13 ("Chromatic Lantern cast 0 times when {b} is already on the `colours you can make:` line")
failed 2/14. Both casts were correct play: the line answers "any black at all", the deck's key
spell needs TWO black sources, and the Lantern turns every land into one. Reported as a model
failure this becomes a re-teach of a rule that is against the deck. This is amendment #47 applied
to a RENDER-KEYED rule rather than a mulligan: the same discipline, but the failure mode is
different — the rule pointed at a rendered field that does not carry the quantity the decision
needs.

Proposed text (extends #47): *Before recording a behavioural prediction as FAILED, check whether
the rendered field the rule points at actually carries the quantity the decision turns on. A
presence line ("colours you can make: {g}{b}{w}") does not answer a count question ("do I have two
black sources"). A rule keyed to the wrong field will be violated by correct play, and the review
that records those violations as model error entrenches the defect for another wave.*
