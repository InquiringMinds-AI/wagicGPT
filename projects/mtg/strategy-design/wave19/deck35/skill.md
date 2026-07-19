# deck35 -- proposals for the strategy-writing skill (Step 0-ter, revised-guide round 2)

All items single-seat; tagged. Layer-routing respected (engine items in notes.md; general Magic
reasoning items in general-suggestions.md). This wave's theme: a DIED class stayed dead, the tax
RELOCATED again, and a durdle-instrument count turned out to have a NON-model root -- which is itself
the sharpest lesson.

## S1 [SHARPEN Step 0-ter / the 0-attacker durdle instrument -- distinguish MODEL-durdle from ENGINE-non-issuance]

The durdle instrument counts "0-attacker games." This wave deck35 logged one (vs135, the adj loss).
Naively that is a re-opening of the FC1 durdle. Root-cause says NO: the model's own stated plan that
game was explicitly aggressive ("I am the aggressor in a race"), it had eligible untapped unsick
creatures, and the engine simply **never issued a declare-attackers decision** -- so the seat dealt
zero combat damage without ever choosing to. The SAME engine non-issuance recurred in a WON game
(vs27, two own-turns) where it cost only tempo. **Rung:** when the 0-attacker instrument fires, do
not score it as a durdle until you check the two disqualifiers -- (a) did the seat have an eligible
untapped/unsick attacker that turn, and (b) was an `attackers` decision actually ISSUED? A 0-attacker
game with an eligible board and NO attacker decision is an ENGINE gap, not a model choice; it belongs
in notes.md, not against the guide. The instrument measures OUTCOME (no attack happened); attributing
CAUSE requires the eligibility-and-issuance check. (This is the instrument's most important refinement
to date -- without it, an engine bug would have been logged as guide regression.)

## S2 [CONFIRM -- tax-relocation (rule 4) is a repeatable, MULTI-WAVE pattern, and it crosses seams]

Wave-18 predicted the block-CHOICE class dying would not take the block-TAX with it; it did not, and
the block tax produced that corpus's only fallback. This wave the block-TAX itself was tamed by the
wave-18 revision (0 block fallbacks; a 10k-char block at 1 life PARSED) -- but the tax did not leave,
it **jumped seams** to trivial LAND-DROP decisions, producing BOTH of this corpus's fallbacks
(a 15k-char unparsed and a stale_echo, both on "which basic land do I play"). **Rung:** tax-relocation
is not a one-time event to note once; it is a recurring dynamic where suppressing indecision at seam A
surfaces it at seam B. After you tame a tax at one seam, RE-SCAN the other low-stakes seams
(land drops, mulligan, "cast nothing") for the same oscillation signature next wave, because that is
where it goes. The gauge that catches it is reply-length-by-seam, not the choice-quality gauge.

## S3 [CONFIRM -- a "trust the rendered number" rule, made TWO-SIDED (wave-18 S3), is validated]

Wave-18 sharpened the trust rule to cover BOTH boards + any damage/effect annotation. This wave the
engine's new per-blocker trade tags ("you kill it, your blocker lives") gave the model a computed
answer to trust, and it trusted them (vs27 seq13 clean profitable first-strike block; vs49 seq17
survival block). No re-derivation spiral at the block seam recurred. **Rung (reinforced):** the durable
cure for a "the model distrusts a rendered number and re-derives it" tax is to make the representation
compute-and-print the very quantity being distrusted, then write the guide line to trust THAT specific
printed quantity by name. Prose alone ("trust the number") is weaker than prose pointing at a concrete
printed tag the engine now guarantees. Confirms the wave-18 S3 direction end-to-end.

## S4 [CONFIRM -- losses live in the off-case, and the off-case can be VARIANCE or ENGINE, not just construction]

Step 0-ter rule 6 again, now with a third off-case flavor. Wave-18's residual losses were
construction (blue screw) + matchup speed. This wave's three losses decompose as: one VARIANCE (49,
a near-dead-heat race lost by a hair -- model attacked every turn, opp reached 1 life), one ENGINE
(135, attack-step non-issuance), one MATCHUP-SPEED (110, dead turn 6 to turbo-Affinity before any
attack window). **Rung:** when a validated guide dips a win, decompose the flipped games individually
before touching the guide -- a race lost at opp-life-1 is variance, an engine non-issuance is a notes
item, a turn-6 kill is a construction/matchup wall. None of the three is guide-fixable, so the guide
should stay near-frozen. Do NOT invent guide prose to "recover" a win-rate dip whose flips are all
non-guide causes (the inverse of rule-3's over-broad-countermand risk).
