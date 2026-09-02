# deck152 -> strategy-writing skill proposals (wave 53)

## S-1. AN ABSOLUTE KEYED TO A SUBSTRING INSIDE A RENDERED **LIST** MUST CARRY THE LIST'S
## CARDINALITY, OR IT FIRES ON EVERY MEMBER OF THE LIST AT ONCE.

deck152's converter rule read *"send every creature whose A-line prints no `(blocking trigger:`
and no stop ... and hold every creature whose tag prints one."* An A-line's tag is not one fact,
it is an enumeration: `their untapped blockers: Wall of Omens (0/4) (you kill it, your attacker
lives); Vampire (1/1) (you kill it, your attacker lives (lifelink: they gain 1, and their
converter takes 1 off you))`. One gain clause anywhere in that list held the whole attacker -
including a 4/4 and a 7/5 whose FIRST listed blocker traded itself away for free. `152v126` s26
(31 life vs 17), s32 (25 vs 16), s40 (17 vs 16): four attackers, **two** distinct blockers named
every time, two declared every time; their life moved 17 -> 16 -> 16 and the seat lost 0-21.

**Rule.** When a guide rule keys on a substring that the emitter prints once per LIST MEMBER, the
sentence states what the list's LENGTH means as well: here, "count the distinct names in `their
untapped blockers` - that is how many of your attackers they can block at all; declare more than
that and the surplus is unblocked." Companion to #178 (quote the collapsed handle and state the
arithmetic) and #183 (the asymmetric pair): the correction to an over-aggressive absolute is a
price comparison, not a mirrored prohibition, or the next corpus breaks the other way - which is
exactly what happened here after wave 52 closed the `ATTACK: none` half (P5 PASS 0/N, 26 empty-tag
A-lines, 0 missed).

## S-2. #171'S SCOPING SENTENCE IS PART OF THE STANDING PORT LIST (#182), AND THE POOL SUPPLIES
## ITS OWN CONTROL.

Same corpus, same literal, two guides, opposite results. deck146's STEP 2 carries *"THAT COVERAGE
LINE IS A ONE-LAND TEST AND NOTHING ELSE. Read the engine's land count FIRST. At TWO OR MORE
lands the sentence ... is not a ship and never was"* and shipped **0** two-or-three-land sevens
(its two mulligans were legal one-landers). deck152 carries a stronger floor (*"TWO OR MORE LANDS
AND AT LEAST ONE SPELL: KEEP. That is the floor and it is not a close call"*) and an explicit
FORBIDDEN THOUGHT naming the colour argument, quotes the coverage literal only under its ONE LAND
rung, and shipped `152v162` s1 - 2 lands {G}{G}, five white spells - with the reply
*"Mulligan for a hand with at least one white mana source"*. Third corpus of that exact shape.

**Rule.** #182's standing list of engine-literal absolutes gains the coverage line's negative
scope, and #144's cross-guide grep is run as a MATCHED PAIR: when one guide holds a shared
literal at 0 breaks and another breaks on it, the passing guide's sentence is the fix and the
port is a copy, not a redesign. Second half, new here: the coverage line collapses two different
causes into one string - too few lands, and enough lands of the wrong colour - and the second is
the case a two-colour deck meets most often, so the scoping sentence names the colour cause
explicitly or the pilot reads the literal as agreement with the thought the guide forbids.

## S-3. #157'S RE-DERIVATION APPLIES TO A RULE'S **SURVIVING** BREAKS EVEN WHEN THE BREAK COUNT
## IS FALLING, AND THE MISSING CONDITION CAN BE A COUNT INSIDE THE LITERAL ITSELF.

deck152's `{paying this taps:` stop went 3 takes (wave 52) -> 2 takes (wave 53) out of 12
windows. Both survivors share a printed fact the rule does not name: the clause holds exactly
ONE name and the row puts a strictly bigger body on the board (`152v130` s11 Ranger Class -> a
2/2 Wolf, s14 Sigarda 4/4, tapping one 1/1 Katilda; that game was won 11 to -4). Every row the
seat correctly passed either named two or three creatures or attached its one name to a level
counter, an activation or a removal spell. The rule's own WHAT-THIS-COST is a three-name case.

**Rule.** When a violated rule's remaining breaks all look like good play, #157's shared-fact
step is run before any restatement, and the condition it finds may be a CARDINALITY read off the
literal the rule already quotes - which costs the guide a clause, not a paragraph. Companion to
#184: a floor whose surviving "breaks" are correct is over-broad, not under-stated.
