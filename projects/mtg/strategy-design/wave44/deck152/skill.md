# deck152 — proposals for the strategy-writing skill (wave-44 evidence only)

## S-F. SELECTION vs EVALUATION: a guide must say which question the rule answers
deck152 died holding a rule that evaluates blocks one at a time (the ladder) while the board asked
a selection question: ONE blocker, THREE attackers, header LETHAL. The ladder ranked the options
correctly and still produced the losing answer, because ranking is not allocating.
PROPOSAL: the skill should make authors label each combat rule as EVALUATION ("is this option
good?") or SELECTION ("which of these do I spend my scarce piece on?"), and require a SELECTION
rule wherever the render can offer more attackers than the deck has blockers. Every guide in the
pool currently has only evaluation rules for blocking.

## S-G. TRANSPLANT A TEACH THAT IS 23/23 ON A SIBLING DECK BEFORE INVENTING A NEW ONE
deck146's guide carries "the only legal targets are YOUR OWN right now -> DO NOT CAST IT" and
honoured it 23/23 this corpus. deck152's guide lacks it, and deck152 destroyed its own only
creature with Fateful Absence on exactly that rendered line (vs deck125 seq 33), after twice
declining the same option. The render's phrasing is identical across decks because it is engine
text, not deck text.
PROPOSAL: when a rule is keyed to a RENDER STRING rather than to a card, the skill should treat it
as pool-wide: any deck whose removal can reach that string needs the rule. A cross-guide index of
render-string-keyed rules would have caught this in one pass. Candidate strings already in use:
"the only legal targets are YOUR OWN right now", "(neither dies)", "you would be at N",
"they have N untapped creatures that could block this one".

## S-H. A RESOURCE-PLACEMENT RULE NEEDS A CONNECTION TEST, NOT A SIZE TEST
Eight consecutive +1/+1 counters went onto one Luminarch Aspirant (1/1 -> 10/10) whose every
attack tag read "Fog Bank (0/2) (neither dies (no combat damage is dealt either way - prevented))".
The guide said "keep it alive" and said nothing about whether the body could connect. Size grew;
damage dealt stayed at zero; deck152 lost.
PROPOSAL: for any recurring "choose a target for your own buff" ability, the guide must ask a
connection question ("did this body's tag get through last combat?") before a size question. The
skill should flag any buff-placement rule that mentions only stats as incomplete.

## S-I. "DECLINED TWICE THEN TOOK IT" IS A MEASURABLE DRIFT CLASS WORTH TARGETING
The Fateful Absence self-kill was the THIRD time that identical option came up (seq 31 declined,
seq 32 declined, seq 33 taken). The failure mode is not a misread - it is a repeated window where
a fabricated justification eventually wins.
PROPOSAL: the skill should ask reviewers to cross-tab REPEATED identical option rows against the
decision taken, and to write the stop for such a window with a named forbidden justification
(the wording the model actually produced), because a bare prohibition has already survived two
correct answers and lost on the third.

## S-J. VIGILANCE (and any "costs you nothing" keyword) needs an explicit non-licence line
The model attacked a vigilant Briarbridge Tracker into a "your attacker dies, their blocker lives"
tag. Vigilance removes the TAPPING cost the S1 stop is usually justified by, and the guide's own
justification for S1 mentions being tapped out of blocking - so the keyword reads as an exemption.
PROPOSAL: when a stop's stated RATIONALE is a cost that some keyword removes, the guide must say
the stop survives the keyword. Otherwise the rationale becomes the loophole.
