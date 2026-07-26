# deck148 -> strategy-writing-skill proposals (wave 29)

Seat: Kor Army equipment, STEP-0 guideless debut. Proposals for the synthesis agent to merge into
wave29/strategy-writing-skill.md. Convergence with the other two Step-0 seats (139 mutants, 122
lifegain) is the merge signal.

## PROPOSAL 1 (Step 0-bis, NEW face of rule 5 / attack-floor): the EQUIPMENT-AGGRO passivity trap -
write the attack floor AND the "gear-the-lord-first" sequence on the FIRST pass.
Equipment aggro is a THIRD archetype (after evasion decks and go-wide anthem decks) where the
attack-floor-does-not-transfer leak recurs, and it comes with a deck-specific SEQUENCING sub-leak.
deck148 durdled to turn 11 for its first attack with bodies down since turn 3 (vs136), AND it
under-used its lord because a BARE lord (Armament Master gear=0) is a blank in the engine. The
authoring move that generalizes: for an equipment/aura deck built around an "equip me to pump the
team" lord, the guide must state (a) the attack floor keyed on a board-readable fact, AND (b) the
explicit sequence "gear the LORD itself first (cheap equipment on the lord > on an attacker),
because the lord does NOTHING until it is equipped." This is Step-0-bis rule 5 (write the floor on
the first pass) fused with rule 3's engine-SEQUENCING clause (deck62 enchantress "cast her first").
Tag: equipment archetype, single-seat; promote to a named "gear-the-lord-first" face if a second
equipment/aura-lord deck reproduces the bare-lord blank.

## PROPOSAL 2 (verify-oracle rail, POSITIVE reinforcement + a new sub-case): the DOUBLED-vs-paper
lord line.
The wave-28 verify-oracle-at-drafting rail (headline 3) caught this seat's biggest fact trap:
Armament Master's engine script is +2/+2 per equipment (real oracle +1/+1), AND it only fires per
equipment attached to Armament Master ITSELF (no gear=0 line). A guide written from paper memory
would state +1/+1 and would miss the bare-lord-is-a-blank fact entirely - the two together are the
deck's whole payoff. Sub-case worth adding to the rail: for LORDS/anthems, verify BOTH the magnitude
AND the CONDITION/trigger of the buff off the `auto=` (the `this(gear = N)` gate here means the buff
scales AND has a floor of zero) - a lord's condition is as load-bearing as its number. Congruent
with wagicgpt-verify-oracle-text; a clean POSITIVE application of the wave-28 rail (no fabrication
shipped because the facts were pulled at draft time).

## PROPOSAL 3 (Step 0-bis, construction-decomposition refinement): the CLOSE-LOSS low-power shell.
deck148 is NOT construction-terminal like deck134 (a clockless control deck that literally cannot
win to the cap). It is a LOW-POWER AGGRO shell that loses CLOSE races (died at opp 2 life vs139;
lost 6-1 vs93; 12-6 vs136). This is a distinct construction sub-shape from deck134: the deck HAS a
clock and gets there, it just runs out of gas / lacks the last push. For this shape the 1/6
decomposition should read ~60/40 construction/decision (not 100% construction) and the guide's
attack-floor + close-fast teaches are legitimately expected to move 1-2 close games - the honest
call is "construction-limited but decision-improvable," not "construction-terminal." Suggest the
skill distinguish CONSTRUCTION-TERMINAL (deck134: guide validated, record cannot move) from
CONSTRUCTION-CEILINGED (deck148: low-power but decisions can flip the close ones) so the second gets
its ordinary validation corpus rather than being routed to the roster owner prematurely.

## PROPOSAL 4 (equipment RENDER - positive confirmation, no edit): the equip surface is CLEAN.
For any future equipment/aura deck reviewer: the equip surface renders well and needs NO
representation ledger work. Equip options bind the attach TARGET into the option with the equip cost
and full card text; the board shows `[attached to: X]` + modified [P/T]; and there is an "Artifacts
in play: you N | opponent N" line for count-based payoffs (Cranial Plating). No equip fall-off /
illegal-attach / double-equip bug across six seat logs. Record this so the next equipment seat does
not re-investigate the render from scratch. (Contrast deck146's venture render, which needed ledger
work - equipment is a solved render surface.)
