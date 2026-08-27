# Strategy-writing skill proposals from the deck162 seat (wave 47)

---

## PROPOSAL S-5: an "ALWAYS, at every life total, on every board" rule needs a stated tiebreak for
the case where two of its own members are on the same menu.
**Evidence:** deck162's RULE 1 is the strongest rule in the pool - *"Your three punishers are
UNDERWORLD DREAMS, FATE UNRAVELER, OB NIXILIS ... If ANY of the three is in the list, CAST IT THIS
ASK ... It goes ahead of every other card in your deck, at every life total, on every board."* Its
compliance is excellent and the deck went 4-1. Its only loss was scored **inside** the rule: at
`162 vs146` seq 6 the menu was *two of the three punishers and nothing else*, so the rule was
satisfied by either answer, and it took the one that is not a creature. The seat then produced
**zero blockers records for the whole game** and died at turn 12.
A rule with an exhaustive "always" and a set of interchangeable members is silent exactly when the
set collides with itself, and that silence is invisible in the compliance metric - the rule scores
100% on the window that lost the game.
**Proposed amendment text:** *When a rule elevates a SET of cards over everything else, write what
happens when two members of the set are on the same menu. Rank them by a property the prompt
already prints, not by strength-in-the-abstract. A rule that is satisfied by every available answer
is not doing any work at that window, and the compliance count will not tell you - audit every
"always/any of these" rule for the two-members-offered case before shipping it.*
**Falsifiable:** zero deck162 asks next corpus where both a bodiless and an embodied punisher are
offered, the seat controls no creature, the opponent controls an unrestricted attacker, and the
bodiless one is taken.

## PROPOSAL S-6 (observation, not yet an amendment): three of deck162's six sections have now been
UNTESTED for three consecutive corpora, and untested is now the norm for a WINNING deck.
**Evidence:** the mulligan section (1 mulligan, no chain), the GANG BLOCK rung (**0** live
`GANG BLOCK:` clauses on this seat - the 2 corpus occurrences are the standing footnote text, not
an A-line), and the collapsed-tag rung (0 collapsed attacker tags). deck162 also had **5 attack
windows in 5 games**. The deck wins by not attacking, so its combat sections may be structurally
unreachable rather than merely unlucky.
**What I am NOT proposing:** deleting them (amendment 21). **What I would want the skill to say
eventually:** that a section unreachable for N corpora at a seat whose ARCHETYPE explains the
unreachability should be moved behind a one-line pointer rather than carried inline, so the bytes
the model reads every window are the bytes its archetype actually uses. I do not have the
evidence to write that amendment yet - it needs a measurement of whether guide length affects
compliance, which no wave has run.
