# Strategy-writing-skill proposals from deck126 (wave 44)

Only proposals my own corpus evidence supports.

## S6 — Colour and mana-source claims need the primitive's subtype line beside them

deck126's live guide asserted *"Your Bayous, Scrublands, Savannahs and Urborg already make
black"*. Savannah is `type=Land / subtype=Forest Plains` — {G}/{W} only (Scryfall agrees).
The claim was load-bearing: it gates the Chromatic Lantern entry, and it argues against the
exact board (Savannah/Plains/Forest) on which the model correctly cast the Lantern.

A wrong colour claim is invisible in playtest logs — the model just follows a rule that is
subtly against it — so it can survive many waves.

Proposal: the skill should require that any statement of the form "these lands make colour
X" be written from the primitives' `subtype=` / `auto={T}:Add{...}` lines, enumerated card
by card, and that the reviewer re-check it every wave the way card texts are re-checked. A
dual land's colours are a card fact, not deck lore.

## S7 — "Both halves are useless alone" is a symmetry claim, and combo halves are rarely symmetric

The guide said of Exquisite Blood and Sanguine Bond: *"Neither does anything useful alone."*
Two of deck126's four wins this corpus were won with **no Exquisite Blood on the battlefield
at all** (turn 15 at 15 life with two Sanguine Bonds; turn 26 at 28 life with one). The
converse held in the third game: Exquisite Blood alone sat on the battlefield from turn 9 to
turn 38 against a creatureless opponent and never triggered once.

The asymmetry is mechanical and derivable from the deck: Sanguine Bond converts a resource
this deck manufactures on its own (life, from 16 defenders) into damage; Exquisite Blood
converts a resource only the opponent can manufacture.

Proposal: the skill should flag "neither/both" phrasings about a two-card combo as requiring
a per-half test — *for each half alone, does the DECK's own engine already produce the input
that half consumes?* If one half's input is deck-generated, that half is a standalone plan
and must be taught as one, with the pair taught as the accelerated version.

## S8 — A numbered priority list needs an explicit survival floor written in render terms

RULE #7 is "take the highest entry on this list that appears in your casting menu", with the
combo pieces at 1 and 2. At life 10, with the opponent's only creature draining 4 a turn and
mana for exactly one spell, the model took entry 1 (a Sanguine Bond with no partner in hand)
over the Tribute to Hunger that would have killed the drain and gained 4 life. It died the
next turn. RULE #4 already licensed the Tribute — but as prose, against a list written as
absolute, and the list won.

Proposal: whenever the skill produces a strict priority list, it should require a companion
floor entry that (a) is numbered into the same list so it inherits the list's authority,
(b) states its precedence over specific entries by number, and (c) is composed only of
conjuncts readable straight off the render (a life number, a header count, an option present
in the menu, a mana total). Prose elsewhere in the guide does not override a numbered list;
only a numbered entry does.
