# deck146 -> core-prompt / render proposals (layer-routed)

Engine bugs are in the seat file's E-list, not here. These are prompt/render proposals only.

## G1 (render) - print the edict's post-resolution board, or stop printing the pre-resolution one
`146v125` s328 and s329 offered `Cast Soul Shatter {2}{b} {right now: they sacrifice Emrakul, the
Aeons Torn (MV 15, their highest)}` on an opponent battlefield line that still listed Emrakul
after the previous copy had already "resolved". Whatever the engine fix for E-1 turns out to be,
the render side has its own lever: when a `{right now: they sacrifice <name>}` clause names a
permanent that a resolution earlier in **this same priority window** already targeted, the clause
should say so - e.g. `{right now: they sacrifice Emrakul, the Aeons Torn (MV 15, their highest) -
a Soul Shatter has already resolved against this permanent this window}`. That single clause
would have stopped two of three casts even with the engine unchanged, and it is a clause about
what the log says, not about what the rules say.

## G2 (prompt) - the "Cast nothing" ask needs its own no-op form
84 of deck146's own-turn answers were `Cast nothing right now`, and in ~63 of them the menu was
two rows: "Cast nothing" plus a row the engine had already annotated dead (`{right now: they
control 0 creatures or planeswalkers - at 0 this does nothing}`, `the only legal targets are YOUR
OWN right now`). The core prompt asks for a CHOICE line and a conditional PLAN line for each of
those. Proposal: when **every** non-pass row on a menu carries a dead-row annotation the engine
itself computed, do not open the ask - resolve it as the pass. This is a strictly-safe
suppression: the engine already knows the rows do nothing, and no guide in the pool ever wants
one taken. (This is the deck146 half of the seat file's E-3; it pairs with E-2's throttle.)

## G3 (prompt) - name the X-cost row's outcome, not just its price
`Cast Agadeem's Awakening {b}{b}{b}{x} {X pricing: max affordable X=8 (11 mana total)}` was
offered 8 times in `146v125` and correctly declined 8 times, but only because deck146's guide
spends a whole paragraph on it ("Its text returns CREATURE CARDS ... Kaya and Lolth are
PLANESWALKERS"). The row already computes affordability; it could as cheaply compute the effect:
`{right now: returns 0 cards - your graveyard holds no creature cards at mana value <= X}`. The
engine prints exactly this shape for Barrowin (`{right now: returns NOTHING - your graveyard has
no creature cards}`) and for Soul Shatter. Extending it to X-cost returns would let six guides
delete a paragraph each and would make the row self-describing for any future deck.

## G4 (render, small) - the `{spends K of your M untapped mana sources this turn; <card> in your
hand needs N}` clause is landing well
Recorded as a positive: 150 prompts on my seats carried it, and deck146's Main-1/Main-2 sequencing
(cast the cheap body, keep the removal's N sources) held everywhere it appeared. No change asked
for; noting it so it is not traded away in a size pass.
