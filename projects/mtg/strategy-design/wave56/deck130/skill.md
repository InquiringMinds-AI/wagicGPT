# Strategy-writing skill proposals from the deck130 seat, wave 56

## S6 (NEW, and the most important one from this seat). A "hold the resource" rule needs a
RELEASE keyed to a printed number, or it becomes a rule against ever winning.
deck130's #2 rule holds every point of damage for a creature that may arrive. In two of its six
games no creature ever arrived: `130v123` finished with the opponent on **zero permanents**, and
`130v125` against a creatureless control deck. Five of the seat's eight face-damage takes broke
the floor by the letter, **and two of them are in the game it WON** - the burn was the only clock
the board allowed. A hold rule with no release converts "this card has a better use later" into
"this card has no use", and the seat cannot tell the difference from a rung count: both read as a
break.
**Proposal:** the skill should require every HOLD-the-resource rule to name its release in terms
of a number the render already prints, and the seat's audit to report hold-rule breaks split into
two columns - **broken while a better target existed** and **broken while no better target could
exist**. Only the first is a misplay. This corpus the split is 0 and 5, and reported unsplit it
looks like the worst regression at the seat.

## S7 (NEW). A one-way latch is a rule that cannot be re-entered; it needs a rate, not an event.
deck130's CROSS-CHECK latch fires on ANY rise in `Opponent life:` and then forbids face damage for
the rest of the game. In `130v125` it fired on a single Elixir of Immortality activation and
governed the next thirty turns. The distinction that matters is not "did their life go up" but
"does their gain per turn exceed my damage per turn" - which is why the same latch is CORRECT in
that game (5 a turn against a recurring Hammer's 3) and would be wrong after one lifelink block.
**Proposal:** the skill should treat one-way latches as a smell. Where a guide latches, the seat
must be able to state the latch as a comparison of two rates both readable off the screen, and
must report the number of windows the latch governed - a latch that governs 30 windows on one
event is a rule the pilot cannot recover from when the event was noise.

## S8 (NEW). The plan verb written before the row is read is a recurring defect class across decks
and belongs in the skill, not in each deck's guide.
`130v162` s19: `PLAN: Kill Master of the Feast with Pyrite Spellbomb` above a row reading
`{right now: takes 2 damage - SURVIVES (toughness 5)}`. Wave 55 had the identical shape on Nadaar.
deck123's `123v125` s45 is the same defect wearing different clothes: `next window cast Bloodline
Keeper` about a card that had been countered two windows earlier. In all three the PLAN line is a
statement about a board that the current window's own text contradicts.
**Proposal:** the skill's PLAN-line guidance should carry one general rule instead of a per-deck
one - **the PLAN line is written AFTER the CHOICE line and may name only objects that appear on
this window's rendered text**. Guides currently re-teach this per card (deck123's Damnation three
numbers, deck130's Starstorm two lists, the SURVIVES rung); one general statement would let three
guides drop their local copies, which is bytes back in the pool.

## S9 (AMEND #259). Report UNTESTED with its structural reason, not just its zero.
`PLAY THIS AS A LAND` rendered 0 times at my seats - not because lane D failed, but because neither
deck123 nor deck130 runs a modal DFC. That is a different fact from "the emitter did not fire", and
a synthesis reading "0 renders at seat 123-130" without the reason could mis-adjudicate D8.
**Proposal:** the skill should require an UNTESTED verdict to name which of three causes it is:
(a) the window never arose in this corpus, (b) the window arose and the string did not print, or
(c) the deck structurally cannot produce the window. Only (b) is evidence against a lane.

## S10 (AMEND, marker-rung retirement). A rung at N/N for two corpora on a marked row should
collapse to the marker's own sentence.
The X marker was taken **6 of 6** this corpus and 2 of 3 last. The guide carried ~2 KB of worked
menus teaching a comparison the marker now performs. **Proposal:** the skill should sanction
"retire the derivation, keep the pointer" as a standard trim once a render-supplied answer has been
followed at N/N across two corpora - with the S4 falsification requirement attached.
