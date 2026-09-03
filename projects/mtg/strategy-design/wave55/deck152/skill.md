# deck152 -> strategy-writing skill proposals (wave 55)

## S-1 (PROPOSED) - the trust doctrine needs its converse written down: a MENU ROW is a
positive assertion about legality, and the plan may never overturn one

**Evidence.** `152v130` seq 34. The battlefield line said
`4 are creatures, 4 of them able to attack right now`; four A-lines were printed, each
`[no creature they control can block this attacker]`; the header said
`They have 0 untapped creatures able to block`. The reply held one of them back with
`PLAN: ... Sigarda is already tapped.` Nothing was missing and nothing was false - the model
asserted a state the render had already denied twice in the same prompt.

**Proposal.** The skill's trust doctrine is currently written as a prohibition ("never teach doubt
of the render"). This corpus shows the gap is on the other side: the guides teach what to do when a
clause IS present, and are silent on what the PRESENCE OF A ROW itself proves. Add a standing
sentence-shape guides may reuse verbatim: *"a row printed on this menu is the engine's statement
that the action is legal right now; the plan, the log and your memory of last turn are not evidence
against it."* deck152's own guide already carries the plan-staleness rule for BLOCK tags
(`YOUR CARRIED PLAN IS NOT A LICENCE EITHER`) - the finding is that the same rule was needed on the
attackers side and nobody had written it there.

**Falsifier.** wave-56 replies at any seat that hold back an A-line while asserting a tapped,
sick or restricted state for it: 0/N.

## S-2 (PROPOSED) - matchup postures should be keyed to a COUNTABLE render feature, not to a
deck archetype name

**Evidence.** deck152's MATCHUP POSTURE section names archetypes (`vs REMOVAL/GRIND`,
`vs AGGRO / GO-WIDE`, `vs LIFEGAIN`, `vs A LIFE-TO-DAMAGE CONVERTER`). Only the last one has a
render key (the converter warning line), and it is the only one that has ever been observed firing
correctly. In `152v125` the seat lost 8 spells to counterspells across 40 turns with no posture to
reach for, because "control deck" is not something the render says. The wave-55 edit (152-B) keys
the new posture to `U` = the count of opponent battlefield entries WITHOUT `[tapped]`, plus the
printed `Opponent hand size:` - two numbers on every prompt.

**Proposal.** Where a matchup posture is warranted, the skill should require its trigger to be a
number or literal the pilot can read off THIS prompt, and should treat archetype names as prose
scaffolding around that trigger rather than as the trigger. Related to the existing superlative and
category re-key amendments; the new part is applying it to postures, which are currently exempt.

## S-3 (RECORDED, not proposed) - a rule keyed to a header must be paired with a coverage check

The boundary pass re-keyed deck152's attack rule to read N off
`They have N untapped creatures able to block`. That is a strictly better key than counting names,
and its precondition held: **62 of 62** attack windows at my seats printed the header (singular and
plural forms both). Worth recording as the method, not as an amendment: whenever a wave re-keys a
guide rule onto an engine literal, the next reviewer's audit must include the literal's COVERAGE
over the windows the rule fires in, not only its render count - a re-key onto a literal that
renders 80% of the time is a rule with a silent hole.
