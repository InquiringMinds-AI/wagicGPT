# deck125 -> core-prompt / render proposals (wave 54)

Layer-routed per the wave-47 doctrine: engine BUGS are in the seat file's engine list
(`wave54/seats/seat-125-126.md`, HIGH #1-#2, MED #3-#4, LOW #5-#6) and are not repeated here.
What follows is prompt text and render shape only.

## G1 - THE DISCARD ASK IS THE ONLY ASK KIND WHOSE ROWS CARRY NO VERDICT, AND IT IS THIS SEAT'S
## MOST EXPENSIVE ASK.

Every cast row in this engine now carries a live/dead verdict the pilot can read without
arithmetic: `{right now: destroys 0 of their creatures}`, `{kills: <name>}`, `{removes: <name>}`,
`at 0 this does nothing`, `[second copy: ...]`, `[DRAW PRICE: ... you would be at K]`,
`{leaves N of your M untapped mana sources}`, `[finds only an enchantment card - ...]`. The
cleanup-discard ask carries **nothing**: nine rows of bare card names.

Repro: `1788425042-ai_baka_deck125-...-vs-ai_baka_deck162.jsonl` seq 47 (t17, 8 life, `kind:
"discard"`) - options are exactly
`['Fall of the Gavel', 'Lightmine Field', 'Final Judgment', 'Dream Fracture', 'Path to Exile',
'Fall of the Gavel', 'Dream Fracture', 'Island', 'Path to Exile']`. On the same prompt the engine
already prints `Your battlefield (9 permanents ... and 9 are lands)` and
`Opponent battlefield (13 permanents listed, of which 0 are creatures ...)`. It therefore already
knows the Island is the ninth land of a nine-land board, and that a Lightmine Field and a Final
Judgment do nothing against zero creatures. It said neither.

Proposal - three annotations, all of them facts the emitter already computes for other asks:
 - on a LAND row: `{spare: you control N lands}` (N from the same header line);
 - on a row whose card's own gate is the opponent creature count: `{dead right now: they control 0
   creatures}` - the same string family as the cast row's `{right now: destroys 0 of their
   creatures}`, which this seat has declined 40 of 40 times, so the string is known to work;
 - on a row naming a card already on the seat's battlefield line: `{you already control one}`.
Cost is one pass over a list the ask already has. The payoff is measured: this seat sent a
counterspell past a listed land three times of four, and two of those three are two of its five
losses. Guide half shipped as W99/W100; the guide can only tell the pilot to look somewhere else
on the prompt, which is exactly the crowding failure #193 names.

## G2 - THE OPPONENT-TURN WINDOW AT A PHASE THAT HAS NEVER PRODUCED A CAST (carried from wave 53
## MED #6, now smaller and now partly answered).

deck125's six games: opponent's End 7, Cleanup 8, Combat begins 12, Combat ends 6, Attackers 8 -
**41 windows, 0 casts**, fourth consecutive corpus at zero. Lane A's hold row is doing part of the
job (5 takes suppressed 13 windows across 3 games) but only after the pilot spends a model call to
take it. The narrow ask is unchanged and is a companion to `mana_only_windows_skipped`: an
opponent-turn window with an EMPTY `ON THE STACK` section, at a phase where this seat has never
cast, is auto-passed. This is a decision-quality and inference-cost item, not a legality change -
lane I's windows are why this seat can counter at all, and nothing here should narrow a window
where something is on the stack.

## G3 - THE HOLD ROW IS NOW CORRECT AND STILL BARELY TAKEN; THE MISSING SENTENCE IS WHAT IT SAVES.

132 renders on my seats, 6 takes (4.5%), and **1 of 102** in windows already carrying
`[you declined this exact list N times already this turn]` at N >= 3. The row's text is accurate
and reassuring about the RISK ("any change re-opens this window; you give up no cast") and says
nothing about the BENEFIT, so a pilot that is not worried about the risk has no reason to press
it. Proposal: append the count the engine already tracks, e.g.
`(you have declined this same list N times this turn; taking this row skips the rest of their
turn's identical windows)`. The declined-count literal is already computed and already printed
elsewhere in the prompt - this is a re-siting, not a new fact. Predicted effect is measurable
against `hold_windows_skipped`, which now works.

## G4 - `pass_hold_ambiguous` IS A PARSER STAMP, NOT A PROMPT ITEM, BUT ITS CAUSE IS PROMPT TEXT.

4 firings on my seats, 4 false positives, and the corpus's 16 are all the same shape. Both row 0
and the hold row now begin with the word `Pass`, so a bare `CHOICE: 0 (pass)` is ambiguous by
construction. Either the stamp matches more than one word, or the pass row's short name is made
distinct (`Pass this window` vs `Pass the rest of this turn`). The second is a prompt change and
is the cheaper one; it also makes the two rows distinguishable to the pilot, which is the point.
