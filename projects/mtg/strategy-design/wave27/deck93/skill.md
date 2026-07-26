# deck93 skill proposals (Step-0 debut) - for the synthesis agent

Context: deck93 is a degenerate single-card deck (24 Swamp / 36 Relentless Rats). It is
an unusually PURE test of two authoring shapes the skill already tracks. Proposals below;
convergence-gate as usual.

## S1 (STRONG) - The cross-phase deciding-fact rung applies to BASIC-LAND identity, and
the deciding phase is the MULLIGAN. This is a fresh, clean witness for the standing rung
(headline 1: "a deck FACT stated for one phase does not reach the phase where it decides;
restate it at the deciding seam"). deck93 supplies the purest case yet: the pilot mulled
the identical 3-Swamp opening 5/6 games calling it "zero lands" while HOLDING three Swamps
- and in vs18 it even wrote "Swamps produce black mana" in the SAME reply before shipping.
The fact "Swamp is a land that taps for B" is basic-Magic knowledge the model HAS, yet it
does not reach the mulligan land-count. Authoring consequence, proposed as a standing note
for mono-basic decks: state the land identity as a Rule-#1 flat line tied to the MULLIGAN
seam explicitly ("a Swamp in hand IS a land; a hand with N Swamps has N lands; NEVER call
a Swamp hand 'zero lands'"), and give the mulligan rule in terms of LAND COUNT with the
Swamp-counts-as-land fact repeated inside the mulligan section, not just at the top. This
is the same shape as the deck59 mulligan-veto and deck27 bottom-hybrid mana witnesses, now
generalized to basic-land counting.

## S2 (MODERATE) - "Only the listed A./B. creatures can act" is a recurring combat-authoring
line worth a standing template for go-wide / token / swarm decks. The pilot reads the
board's N creatures and assumes all N attack/block, hallucinating slots (A3/A4 with only
A1/A2 offered; B2/B3 with only B1 offered). It is safe (engine drops them) but corrupts the
model's own lethal math and PLAN. Proposed template line for any deck that fields many
identical bodies: "Count your legal attackers from the A-lines, not from the board; a Rat
you just cast (summoning sick) and a [tapped] Rat are not on the list and cannot act."
Tag PROVISIONAL/single-seat until a second swarm/token deck reproduces the phantom-slot
math error.

## S3 (MODERATE) - "Losing a body shrinks the rest" is a distinct anthem/lord authoring
face. For symmetric-self-anthem decks (Relentless Rats, and any "each other X gives +1/+1"
lord-of-its-own-kind), the standing "don't overcommit / don't chump" advice needs an
EXTRA clause: each creature lost weakens ALL survivors, so trades are doubly costly and
board preservation outranks removing a single blocker. This differs from the generic
gang-block trap (which is about coordination) - here the cost is the shrink. Pair with the
existing "take damage while ahead" core line, which the pilot IGNORED at vs136 seq20 even
though it rendered verbatim. Provisional/single-seat.

## Boundary note
No skill boundary is crossed by deck93. Everything above is deck-guide + representation-
ledger routing; nothing proposes a core-prompt body edit (the "take damage while ahead"
line is already in core and rendered correctly - the failure was model adherence, a guide
lever, not a core gap).
