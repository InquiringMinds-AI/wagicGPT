# deck152 -> strategy-writing skill proposals (wave 46)

---

## S1 (STRONG) — TWO RULES THAT CAN BOTH FIRE ON ONE SCREEN NEED A STATED PRECEDENCE, AND THE
## MOST DANGEROUS PAIR IS A POSTURE RULE OVER A PRICE RULE

**Observation — the deciding loss.** deck152 carries a lifegain-wall rule ("at two or more, the
attack PAYS them ... send only what actually gets through") and a matchup-posture rule ("vs A
LIFE-TO-DAMAGE CONVERTER ... the correct posture is the most aggressive one available: attack with
everything that is not held home by a stop"). Both fired on the same screen for three consecutive
combats. The posture rule won, and the seat spent **fifteen life on its own attacks** (20 -> 11
-> 5) while the opponent gained the same amount, losing at -1 from a board it was ahead on.
A price rule tells the executor what a decision COSTS; a posture rule tells it what MOOD to be in.
A mood beats a price in prose every time, because the mood sentence is shorter and it feels like
a conclusion.

**Proposed amendment.** *A posture/matchup rule ("be aggressive", "go defensive", "race them") may
never be written so that it can be read as overriding a rule that prices an individual decision.
When you add a posture rule, name the price rules in the same guide it will be read against and
state the direction of the interaction — normally: the posture selects among the options the
prices allow; it never adds options the prices refuse. If the interaction is genuinely
multiplicative (a converter doubling every lifegain trigger), say the arithmetic, because a
doubled price is the one case where the price rule should get STRICTER under the aggressive
posture, which is the opposite of what the mood word suggests.*

**Falsifier.** A corpus in which a posture rule with a stated precedence produces measurably more
passivity than one without, on a seat that needed to race.

---

## S2 (STRONG) — AN ARITHMETIC STEP BELONGS IN THE FIRST SENTENCE OF THE ANSWER, NOT AT THE
## BOTTOM OF THE LADDER IT GOVERNS

**Observation.** deck152's block ladder has both the floor ("Take this block when N is 9 or less")
and an explicit arithmetic step ("read the header's 'you would be at N', compare N to 9, and
answer") — and both sit **below** a 25-line ladder. Second corpus running, the seat produced a
bare `BLOCKS: none` at N=8, and the reply printed the number without comparing it: *"I will take
the 6 damage, dropping to 8 life."* It was the margin it died by. Amendment #24 said place the
release condition near the answer; this sharpens it: the step that must run FIRST must be
written first, in imperative form, as an instruction about what to WRITE.

**Proposed amendment.** *When a decision has one governing number, the guide's instruction for
that decision opens with "the first line of your answer is <the number>". Placement near the
answer is not enough — the executor must be told to emit the number before it emits the choice,
because a number it did not write is a number it did not use. Then make the absence of that
number a falsifiable defect you can grep for in the next corpus.*

**Falsifier.** A corpus in which replies name N and still answer against it at the same rate.

---

## S3 (MEDIUM) — A COUNTER/RESOURCE CAP MUST BE KEYED TO A RENDERED STATE, NEVER TO A RUNNING
## TALLY THE EXECUTOR HAS TO KEEP

**Observation.** "From the fifth counter on, put it on a SECOND creature instead" requires
remembering how many counters have been placed across turns. Seven consecutive placements went to
the same body (Sigarda 4/4 -> 11/11) in one game. The option row prints each creature's CURRENT
size on every one of those asks — a state the executor cannot fail to see.

**Proposed amendment.** *Never write a cap over a quantity the executor must accumulate across
decisions; there is no memory between asks. Re-key every such cap to a value the current prompt
prints. "From the fifth counter on" becomes "if the row already reads 6/6 or larger". If no
rendered state carries the quantity, that is a render request, not a guide rule.*

**Falsifier.** A corpus in which a rendered-state cap is followed no better than a tally cap.

---

## S4 (MEDIUM) — A FLOOR WRITTEN FOR ONE REGIME MUST NAME ITS REGIME

**Observation.** "NEVER bottom a land unless the hand you keep would still hold at least FOUR" is
correct at six and five cards and absurd at four — where the seat correctly bottomed three lands
from a five-land keep and would have been left with a four-land hand if it had obeyed. The
guide's own ask prints "You will be left with a N-card hand", so the regime is on the screen.

**Proposed amendment.** *Every hard floor is written for a regime. State the regime in the same
sentence and key it to something the prompt prints, or the first window outside the regime will
produce an odds-favoured violation that a reviewer must then decide whether to score. A floor
whose regime is unstated cannot be falsified — every violation reads as either "the rule is wrong"
or "the pilot is wrong", and the corpus cannot tell you which.*

**Falsifier.** A corpus in which regime-stated floors are violated at the same rate as bare ones.

---

## S5 (SUPPORTING) — WHAT WORKED, RECORDED SO IT IS NOT UNDONE

The two hardest sub-clauses of the shared attack STOP both held on their first live windows:
the vigilance clause ("Briarbridge Tracker's vigilance does not change a 'your attacker dies'
line") and the lifelink clause ("Never write 'the Adversaries gain 3 life each AND deal 6 damage'
- that sentence counts the blocked outcome and the unblocked outcome at the same time"). Both are
the same construction: **quote the sentence the executor would otherwise write, then refute it.**
Amendment #30's rationale-loophole form now has two independent confirmations on this seat.
