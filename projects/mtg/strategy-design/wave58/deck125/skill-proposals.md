# deck125 — proposal for the strategy-writing skill (wave-58)

Read against `wave57/strategy-writing-skill.md`, amendments 1-329. One proposal.

## S1. A guide FLOOR expressed as a CONSTANT is a proxy, and it is re-derived against the card's own threshold whenever the render prints both operands.

deck125's Lightmine Field rung read: *"Do not cast Lightmine Field while that count is below 3 -
not at 0, not at 1, not at 2."* It was a clean, obeyable, ceiling-held rule — 43 rows, 1 cast, no
argument at any window — and it was **wrong on the card**. Lightmine Field deals damage equal to
the number of attackers, so the count is also the damage, and it kills any attacker whose printed
toughness is at or below it. The constant 3 was standing in for "enough damage to kill something",
and the render prints both halves of the real test on the same screen: the row prints N, the
opponent battlefield line prints `Spider #1 (2/1)`. In `125v146` the seat declined a two-count row
four times (s38, s39, s41, s42) against two 1-toughness bodies with six spare mana, at 27 then 20
life, and lost that game at -4.

The failure mode is not disobedience and it is not a perception error — high compliance is exactly
what made it expensive, which is #248's point turned inward: **a floor the pilot obeys perfectly is
worth nothing if the floor is not the card's threshold.** #319 says a *render* marker gets the
guide's own arithmetic; this is the same demand pointed at the guide's own constants. #321 asks
whether a key measures something the pilot can act on; this asks whether it measures the right
thing at all.

**The rung:** when a guide states a numeric floor for a card, the writer must name what the number
is a proxy FOR, and check whether the render prints the proxy's own inputs at the decision window.
If it does, the floor is written as the test (`N >= the printed toughness of at least one counted
creature`), not as the constant. If it does not, the constant stays and the guide says so in one
clause, so the next reviewer knows it is a proxy and not a measurement.

**How a reviewer catches it:** a constant floor whose compliance is at its ceiling is the shape to
audit, not the shape to leave alone. Take the card's printed text, derive the threshold it actually
has, and ask whether the constant and the threshold ever disagree on a board in the corpus. Where
they do, the declines under the constant are the evidence, and they will be filed as "rule held" by
every count-based pass.

**What this does not say:** it is not a licence to lower every floor. deck125's Revelation
`max affordable X >= 3` floor is a *value* judgement with no printed threshold behind it, and it
stays a constant. The test is whether the card itself defines the number.
