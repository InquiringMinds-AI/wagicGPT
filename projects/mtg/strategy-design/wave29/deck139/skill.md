# What the MUTATE surface teaches (deck139, Step-0 debut) -- NEW RENDER-FACE MATERIAL

This is the campaign's first mutate deck. It adds a new class of multi-step, merge-into-one-
object mechanic. The durable lessons for the strategy-writing skill:

## RENDER-FACE 1 -- a merge mechanic renders in TWO inconsistent surfaces (battlefield vs A-lines)
The mutate PILE (a merged creature) renders CORRECTLY as ONE creature in combat attack/block
A-lines (single line, top card's name/P/T/keywords) but AMBIGUOUSLY in the "Your battlefield"
listing, where it appears as two adjacent lines: the top card with P/T + its own keywords, and
the under card as a BARE name + mana cost, no P/T, no tag. The header creature-count is correct
(the pile counts as 1). **Skill rule for any merge/attach/stacked-object mechanic (mutate,
and structurally the same shape as equipment/Auras/host-augment):** the CANONICAL render is the
one the decision is made on. Where the same object appears in two surfaces, verify BOTH; a
mechanic can be right in the combat lists and misleading in the board list at the same time.
When the board render splits one object into pieces, the fix is REPRESENTATION-LANE (group the
pile / tag the under card [under X] / show the merged keyword+ability set on the top line), NOT
guide prose -- but the guide must still carry a one-line "this is ONE creature" note because the
render ships before the fix (standing DECIDING-FACT-RIDES-THE-OBJECT gap).

## RENDER-FACE 2 -- a draw-then-discard (loot) mis-renders as a generic "TARGET CHOICE"
Pollywog Symbiote's mutate-cast loot (`_LOOT_` = draw 1, then discard 1) renders as
"TARGET CHOICE for Pollywog Symbiote ... Pick the ONE target it will affect" with the player's
HAND as the option list. Nothing says DISCARD. This is a two-way natural experiment inside one
corpus: the SAME render was DECODED by the model in one game (deck122 s18: "the game logic seems
to be triggering ... draw a card, then discard") and MISREAD in another (deck93 s20: "requires a
target creature you control to mutate" -> discarded a land by accident, reasoning about the
wrong thing). **Skill rule:** a cost/effect that REMOVES a card from the player's own hand
(discard, sacrifice, exile-from-hand) must be LABELED with the verb (DISCARD / SACRIFICE), not
framed as a neutral "target it will affect" -- the neutral frame invites the model to interpret
it as a beneficial target. This is a perception leak (the model misreads WHAT the choice is), so
per the wave-28 perception-vs-strategy rung it dies to a SURFACE ANNOTATION, not guide prose;
the guide carries a stopgap trap-warning while the render fix is routed to notes.

## RENDER-FACE 3 -- multi-ask FLOW ORDER for a compound cast is itself a comprehension variable
One mutate cast produces up to five sequential asks (cost-choice -> normal/mutate -> [loot
discard] -> over/under -> mutate-target -> mutate-ETB trigger). The over/under question is asked
BEFORE the target is chosen, and an unrelated triggered-ability choice (the loot) is interleaved
in the middle. Each ask re-sends the full board, so a scrambled/repetitive flow multiplies both
latency and truncation risk. **Skill rule:** for a compound mechanic, the guide should give the
player the WHOLE flow in order once ("cost -> mutate -> over -> pick host") so each individual
ask is recognized as a step of a known sequence rather than a fresh puzzle; and the flow ORDER
itself (target-before-over/under would be more natural) is an engine/representation note.

## RENDER-FACE 4 -- same mechanic, inconsistent COST LABEL
Two cards with an identical `other={..} name(Mutate)` cost render with different labels: Gemrazer
"with its mutate cost", Migratory Greathorn "with its alternative cost". A player who has learned
"mutate cost" does not recognize "alternative cost" as the same thing. **Skill rule:** when the
guide teaches a mechanic by the name the render uses, verify the render uses ONE name for it;
if the engine emits synonyms, name all synonyms in the guide ("labeled 'mutate cost' or
'alternative cost' -- both mean mutate") until the render is unified.

## STRATEGY-FACE -- a trigger-bearing alternate cast reframes "which cost" as "which payoff"
The Apex creatures' entire value is gated behind "Whenever this creature MUTATES" -- a hardcast
fires nothing. This is a recurring archetype (alt-cost-unlocks-the-payoff: mutate, kicker,
foretell-value, cast-from-graveyard). **Skill rule:** when a card's headline effect is
conditioned on a NON-default cast mode, the guide's #1 job is to make the payoff cast the DEFAULT
in the player's mind and name the exception (here: mutate the Apex unless you lack a host or need
a raw blocker). The model reads the option list correctly but has no prior that "mutate" is the
value line vs "cast normally" -- deciding-fact-rides-the-option applies to the CHOICE BETWEEN
CASTING MODES, not just to targets.

## VERIFY-ORACLE reconfirmation (hard rail held)
Every card fact in this guide was read off the primitive `mana=`/`text=`/`other=` line AND
cross-checked on Scryfall before assertion. One near-miss worth recording: memory said Migratory
Greathorn's land-search triggers "on enter OR mutate"; Oracle and the primitive both say MUTATE
ONLY. Asserting the memory version would have (a) falsely flagged the primitive as a bug and
(b) told the player a hardcast ramps when it does not. The rail caught it. Mutate costs also
must be read AFTER cost-reducers: Migratory Greathorn shows {1}{G} only because Pollywog reduces
its {2}{G} base -- verify the BASE cost from `other=`, not the reduced number in the option.
