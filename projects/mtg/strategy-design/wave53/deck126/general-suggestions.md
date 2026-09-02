# Core-prompt / render proposals from the deck126 seat (wave 53)

Layer-routed; engine defects are in the seat file's engine list, not here.

## G5 - The `LIFE-TO-DAMAGE CONVERTER` block is the best surface either of my seats has, and it
should say who it is bad for

Rendered 353 times to deck125 (`theirs - Sanguine Bond`) and 147 times to deck126 (`yours -
Sanguine Bond`), it is a dedicated, single-purpose line and it is what I re-keyed deck125's Emrakul
stop and Path carve-out onto (W77, W79). Its text ends: "This is not a trade: when you gain N, your
total goes UP by N and theirs goes DOWN by N in the same event."

One gap. The block describes the effect symmetrically and leaves the pilot to work out the
consequence for the CARD IN FRONT OF IT. The two decisions it should have decided on my seats were
both about a specific row: an Emrakul that an edict turns into 15 life for the converter's owner,
and a 1/1 lifelink token that is a 2-per-combat clock. Ask: when the block says `theirs`, append one
clause naming the class most affected - e.g. `- any creature of theirs with lifelink is worth double
its power against you, and any creature of YOURS they can make you sacrifice pays them its
toughness`. Not a per-row annotation; one sentence on the block that already exists.

## G6 - `[finds only an enchantment card - every enchantment left in your library is a copy of one you
already control or hold: ...]` is accurate and is being read as an inventory, not as a stop

Twenty-two Idyllic Tutor rows, all clauses accurate. Ten takes, of which **two** were on this dead
form (`vs125` seq 14 t10, `vs130` seq 21 t21), both with BOTH combo halves already on the hand line.
The clause states a fact from which the pilot must derive "so this search cannot help me". The
sibling live form does derive it - `still in your library, not on your battlefield or in your hand:
Exquisite Blood` - and it went 8 for 8.

Ask: close the dead form the same way the lane-P Lantern tag was closed - end it with the
consequence, e.g. `... - this search cannot bring you a card you do not already have`. The Lantern
precedent is the argument: adding the consequence clause to that tag took the same rule from 5 takes
of 5 to **0 takes of 6** on this seat with no guide change at all.

## G7 - The bare `[second copy: ... no legend rule]` now reads as the "safe" tag by contrast

Lane P gave the Lantern a longer tag ending "this copy adds only its own abilities" and the seat
declined 6 of 6. On the same seat, `[second copy: you already control Sanguine Bond; both stay on
the battlefield - no legend rule]` - correct, because two Bonds really do both trigger - was taken
at `vs125` seq 22. With one form now carrying a "this does nothing extra" tail, the ABSENCE of that
tail is being read as "this does something extra", which for a Sanguine Bond is technically true and
strategically false.

Ask, in preference order: (a) leave the tag alone and let the guide carry it (I did this, D86); or
(b) if the engine can cheaply tell that the seat's win condition does not scale with copies, say so
- but this is a judgment the render should probably not be making, so (a) is my recommendation and
G7 is recorded as a wording risk created by a good change, not as a defect.

## G8 - The `SACRIFICE ONE OF YOUR OWN CARDS` list should be ordered, or should carry the seat's own
value hint

Routed here rather than to the engine list because the framing bug (`your own effect` for the
OPPONENT's Emrakul) is the engine item (seat file MED #3) and this is separate. At `vs125` seq 111
the list was 23 rows in battlefield order: 15 lands, an enchantment, a planeswalker, two artifacts,
a wall, and the seat's only lifelink Vampire last. The header says "usually your LEAST valuable: a
spare land or a redundant/dead card". The pilot picked row 23. At seq 112, on the identical list, it
reasoned correctly and picked lands - so the information was sufficient but the ordering fought it
on the first of six picks.

Ask: sort the list cheapest-first (lands, then duplicates, then the rest) or tag the obvious
candidates (`[spare land]`). Six sequential picks from a 23-row list is the widest ask either of my
decks faces and it arrives at the moment a game is being decided.
