# deck146 -> strategy-writing skill proposals (wave 56)

Only proposals my own corpus evidence supports. Amendment numbering left to synthesis.

## S1 (STRONG) - #238's P10 shape is a REUSABLE INSTRUMENT: every workaround rule ships with a
detector prediction AND a retire clause in the same wave.
EVIDENCE. deck146 carried "EMERIA'S CALL AND AGADEEM'S AWAKENING ARE NOT LANDS FOR YOU" for three
waves. It was true when written, false the moment lane D shipped, and it was retired in the FIRST
review after the fix - because wave-55 P10 was written as *"still 0/N unless the engine changes -
this prediction exists to detect the FIX, and if it ever becomes non-zero the block must be deleted
in the same wave."* That sentence is what made the retirement automatic instead of a rediscovery.
PROPOSE: make it a named rule of the skill - a guide rule that exists BECAUSE of an engine defect
is written with (a) the render literal that will change when the defect is fixed, (b) a prediction
whose non-zero reading is the retire trigger, and (c) a `REMOVE THIS RULE WHEN ...` line inside the
guide text itself, so the next reviewer does not have to hold the history.
COST OF NOT HAVING IT, measured here: the block did not merely go stale, it went ACTIVELY HARMFUL -
it is cited in this deck's mulligan rung, its bottom rung, its cleanup scan (two numbered entries)
and one DECIDING SITUATION, and all five had to be re-keyed in one pass.

## S2 (STRONG) - a workaround rule METASTASISES; count its citation sites before writing it.
EVIDENCE. One false premise ("those cards are spells only") had propagated to NINE edit sites in
64 KB of guide. The skill currently treats a rule as a local object.
PROPOSE: when a rule states a FACT about a card class (rather than a decision), the guide states it
ONCE in a named block and every other rung REFERS to that block by name. Then a retirement is one
edit, not nine, and a reviewer can count the blast radius before shipping.

## S3 (MED) - "ADD ONE TO THE RENDERED NUMBER" is inside the trust doctrine; "the number may be
wrong" is not. Give the doctrine this worked example.
EVIDENCE. The pregame hand header under-counts land-backed cards (seat file E-1). The doctrine
forbids teaching doubt of a surface. The rule that shipped is
`your land count is that number PLUS one for each <three names> on the hand line` - an arithmetic
rule whose two inputs (the header number, the hand card list) are BOTH true surfaces, plus a
removal condition. Nothing in it says the render lies.
PROPOSE: record this as the doctrine's canonical "route around a false surface" example, and state
the discriminator explicitly - a legal route uses only true surfaces as inputs and names the
condition that deletes it; an illegal route asks the pilot to distrust a printed value.

## S4 (MED) - an over-braking rule reads as a FAIL and must be adjudicated by the PLAY, not the key.
EVIDENCE. wave-55 P1 fired 2 of 2 at deck146 seats and both flagged decisions were correct
(`146v123` s12 and s21: a vigilance attacker under a `[doesn't untap]` tag, into a header reading
`They have 0 untapped creatures able to block`, in a game won 21/-2). A reviewer reading the key
alone would have hardened the brake and cost the deck its free attacks.
PROPOSE: the skill's prediction template gains a required field - **"a hit is a lane break only if
<condition>"** - and the reviewer instrument says: when every hit of a prediction is a correct play,
the verdict is RULE OVER-BRAKES, and the edit is a carve-out, never a tightening.
