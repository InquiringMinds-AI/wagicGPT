# wave-52 strategy-writing-skill proposals - deck126 seat

## S4 (NEW amendment) - a card's TRIGGER DIRECTION must be written as a sentence the pilot can fail,
## not left to the card text the render already prints

**Evidence.** `deck126 vs125` seq 152-216. The battlefield line printed Exquisite Blood's own text
verbatim (`{effect: "Whenever an opponent loses life, you gain that much life."}`) in every one of
those prompts, and the pilot still wrote, and carried for nine turns, "their Staff of Nin ping deals
1 damage to me, triggering Exquisite Blood, causing me to gain 1 life". The render was correct and
complete; the failure was reading a symmetric-looking two-card loop in the direction that made the
current board a win. The guide had a section on the pair and no sentence that named the wrong
direction.

**Proposal.** For any card whose text is a conditional the pilot could read backwards (drain loops,
"whenever you gain"/"whenever an opponent loses", reflexive damage, punisher enchantments), the
guide must carry an explicit FALSE SENTENCE in the same voice as the deck's other "ALWAYS FALSE"
lines. The skill already has the always-false-thought construction; this extends its trigger list
from *plans* ("ramp to Emrakul") to *rules readings*.

## S5 (NEW amendment) - a win condition must be written as a list of STARTERS the pilot owns, and
## the guide must say what to do when the list is empty

**Evidence.** Same game. Rule #2 listed five ways to press the win and every one of them requires a
card of the pilot's (a Tribute with a victim, a block, a Staff ping, a lifelink attack). On turns
30-38 none existed - opponent board empty of creatures, the seat's Staff countered, no Sorin, no
Vampire - and the guide's only remaining instruction was the win-button section, which reads as
"you have already won". The pilot answered `Cast nothing right now` five times with a Wall of Omens
(draws a card) on the menu, at 9, 7, 5, 3 and 1 life.

**Proposal.** Every combo guide gets an explicit *no-starter* branch beside its win button: name the
state (assembled but unable to trigger), name the digging action, and forbid the pass. Without it,
"you win the moment X happens" degrades into "wait for X".

## S6 (NEW amendment) - the PLAN line is an INPUT to the next decision, so the reply rules must
## constrain what may be written into it

**Evidence.** `deck126 vs146` seq 11 -> 12 -> 13. The seq-11 reply wrote a 1,500-character
self-arguing PLAN whose last assertion was "Both combo pieces (Exquisite Blood and Sanguine Bond)
are now on the battlefield" (Sanguine Bond had been exiled by Vanishing Verse at t11). That exact
text was echoed back as `YOUR PLAN (as you last stated it)` at seq 12 and seq 13; at seq 12 the
pilot cast a redundant second Exquisite Blood on it. Three windows corrupted by one sentence.

**Proposal.** A guide-level and core-prompt-level pair: (a) guides that name a board state in their
plan must be told to read the battlefield LINE at write time, and (b) the reply rules should ask for
a plan that states an INTENT for the next turn rather than a claim about the current board, since a
claim is what survives to become false. deck126's Rule #3 already has half of (a) ("Read both names
off your battlefield line before you write 'both enchantments'"); it did not stop this, which is why
(b) belongs in the skill/core prompt rather than in another guide sentence.

## S7 (evidence, no amendment) - `[second copy:` shows that a tag answering a RULES question reads
## as permission to a pilot who was asking a VALUE question

The literal is accurate ("both stay on the battlefield - no legend rule") and it flipped a 9/9
decline into a 5/5 take. Filed here because the skill's guidance on quoting engine literals assumes
the literal's meaning is the pilot's question; this is a counterexample. The render half is proposed
in `general-suggestions.md`; the guide half is edit D65.
