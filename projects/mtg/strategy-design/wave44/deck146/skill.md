# deck146 — proposals for the strategy-writing skill (wave-44 evidence only)

## S-A. A COUNTING RULE MUST NAME THE THING THAT DEFEATS IT
The outnumber clause ("send it only when the attackers you are sending OUTNUMBER their untapped
blockers") is a pure counting test. It is correct against ordinary blockers and catastrophically
wrong against blockers that PAY the defender for blocking - and nothing in the sentence hints that
a class of exception exists. deck146 fed 25+ life to two 0/N walls over six turns and lost a game
it was winning at turn 16, following the guide exactly.
PROPOSAL: when a guide rule reduces a decision to counting bodies, the same paragraph must name
at least one board shape that breaks the count, in the vocabulary the render actually prints
("count the 0/N defenders on their battlefield line"). A counting rule with no stated exception
class reads as exhaustive, and the model will apply it to the boards it was never tested on.

## S-B. A LADDER NEEDS AN ARITHMETIC GATE ABOVE IT, NOT INSIDE IT
The block ladder's rungs are ordered by QUALITY of trade. At lethal, the deciding variable is not
trade quality but the SUM of the "deals N" numbers you fail to stop. Because the arithmetic lived
only inside rung 3 ("take this block when N is 10 or less, or when the header says the attack IS
lethal"), a rung-1 free kill outranked survival and the sibling seat died to it (deck152 seq 20).
PROPOSAL: any ordered ladder that answers a resource-ALLOCATION question (which blocker to which
attacker) needs a survival/arithmetic gate stated ABOVE rung 0, because the ladder only ranks
options - it never sums them. State the gate as a sum-and-compare over numbers the render prints.
More generally: ladders answer "is this option good?"; they cannot answer "which of these options
do I spend my one resource on?" A guide that uses a ladder for the second question needs a
separate selection rule.

## S-C. "ALWAYS / NO NUMBER CAN MAKE THIS WRONG" IS A LOAD-BEARING ABSOLUTE - AUDIT EVERY ONE
Rung 1's absolute ("TAKE THAT BLOCK, always, at every life total and every forecast number...
No number on this screen can make this block wrong") is the sentence that produced the loss. The
absolute is doing real work in the ordinary case - it stops the model re-deriving combat - so it
should not simply be softened. It needs a scope it cannot escape.
PROPOSAL: for every absolute in a guide, the skill should require the author to write down the
board on which it is false, and either add that board as a named exception or record it as
unreachable. An absolute with no recorded falsifier is untested prose, not a verified teach.

## S-D. THE RENDER PRINTS STATE; TEACH THE MODEL TO READ ITS OWN STATE BEFORE PAYING FOR IT
23 redundant Hive animations happened while the battlefield line said, in plain text,
"Hive of the Eye Tyrant #1 (3/3) [menace]" and "of which 1 is a creature". The render was honest;
no rule told the model to look at it before re-buying the effect.
PROPOSAL: any guide teaching a REPEATABLE activated ability should carry a one-line
already-on check written as an instruction to read a specific rendered field ("find it on your
battlefield line; if it already reads (3/3), pass"). This is cheaper than a mana-budget rule and
it is verifiable from the transcript.

## S-E. TOKENS AND CARDS ARE DIFFERENT NOUNS IN A COMBAT STOP
S1's per-creature STOP was written about "a creature". Against one blocker and four attackers, the
right play is to send the token INTO the stop so the block is spent on the free body. The teach
was right about cards and wrong about tokens, and the model's violation was better than the rule.
PROPOSAL: combat prohibitions should say CARD or TOKEN explicitly. "Creature" is under-specified
in a deck that makes tokens, and the cost side of every stop rule (what you lose when it dies)
differs by an order of magnitude between the two.
