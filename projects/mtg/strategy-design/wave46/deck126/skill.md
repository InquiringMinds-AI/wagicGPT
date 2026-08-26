# Strategy-writing skill proposals from the deck126 seat (wave 46)

## S5 (proposed amendment) - A BLANKET IMPERATIVE PRINTED ABOVE ITS OWN EXCEPTION WINS
**Claim.** When a section opens with an unconditional imperative and states its exception later
under a heading of its own, the imperative is what executes. The exception must be a numbered STEP
ABOVE the imperative, and the imperative must be worded as conditional on it.
**Evidence.** deck126's RULE #5 opens `Assign EVERY blocker the list offers you, in every combat:`
and puts the survival carve-out thirty lines below under "The one blocker you leave out". Both
blocks in the game this seat lost violated the carve-out and obeyed the imperative
(`...vs-ai_baka_deck146.jsonl` seq 5 and seq 8, turns 4 and 6; both walls died; turn 8 had no
blockers left and took 10 damage; dead turn 10). The MULLIGAN section has the identical shape -
`Bottom your most expensive spell first` as the first sentence, `NEVER bottom a land when the hand
you keep would hold fewer than THREE` as the last - and the one bottoming ask this corpus produced
violated both halves in one pick.
**The control is in the same corpus.** At `...vs-ai_baka_deck162.jsonl` seq 15 the SAME carve-out
fired correctly and benched two Vampires. What separates the cases is a competing printed tag: the
wall's B-line carries `[deals 0 - this block kills nothing, but it STOPS all N damage from reaching
you]` and the Vampire's does not. So the exception loses only when the render supplies a rival
number - which is exactly when a guide most needs to state precedence outright, in the render's own
vocabulary ("that parenthesis outranks the STOPS-all-N tag beside it").
**Falsifier.** A section whose exception is stated after a blanket imperative and which conforms
above 90% across a wave with a competing rendered tag in the window.

## S6 (proposed amendment) - RE-VERIFY A GUIDE'S CARD-FACT LIST AGAINST THE RENDER, NOT ONLY
## AGAINST THE PRIMITIVE THAT WROTE IT
**Claim.** A by-name enumeration of cards satisfying a property is a STATIC claim in a game where
permanents change each other's properties. Verifying it against each named card's own primitive is
not enough; a lord/transform effect on the battlefield can falsify it, and the render already knows.
**Evidence.** deck126's Lantern gate said `the lands of yours that make black are Bayou, Scrubland,
Woodland Cemetery, Isolated Chapel, the Swamp and Urborg. SAVANNAH AND SUNPETAL GROVE DO NOT`.
Every one of those facts is right about the card in isolation and the list is WRONG on a board with
Urborg in play: primitive `name=Urborg, Tomb of Yawgmoth / auto=lord(land) transforms((swamp))` -
Urborg makes EVERY land a Swamp, so Savannah and Sunpetal Grove do make {b}. Both of that guide's
worst Lantern casts happened on an Urborg board, and the render's per-colour clause (`{B} 3`,
`{B} 6`) was right in both windows while the guide's "2" was wrong. The guide had put its own
warning on the wrong target too - `Do not use the "colours you can make:" line for this test` was
correct about the colour SET and got carried over onto the per-colour COUNT that shipped this wave.
**Deployment.** Where the engine renders a COUNT of the property, key the rule to it and delete the
enumeration. Where it does not, write the enumeration with the modifier class named ("...unless a
permanent is changing land types").
**Falsifier.** A by-name property list that survives a corpus containing a lord/transform effect
over that property.

## S7 (observation) - AN AUTO-PASS CHANGES A RULE'S DENOMINATOR, NOT ITS RATE, AND A GUIDE THAT
## MENTIONS THE AUTO-PASS SHOULD SAY WHAT SEEING THE WINDOW NOW MEANS
This seat's `Add N green mana` discipline reads 1/16 this wave against 2/39 last wave - but 180 of
the old windows never reached the model, so the two fractions are over different populations and
neither reviewer should report them as a trend. The guide-side move the evidence supports is small
and new: once the engine suppresses a window class, SEEING one of those windows is itself
information ("something else was on the menu too"), and the guide can say so. Recorded so a later
wave does not read 1/16 vs 2/39 as a regression.
