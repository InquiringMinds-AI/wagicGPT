# Strategy-writing skill proposals from the deck123 seat, wave 47

Only what this corpus's evidence supports. Two proposals, one of them a correction to an
amendment I authored.

---

## S47-1 - A STOP RULE MUST BE WRITTEN ON A QUANTITY THE ACTION CHANGES

**Evidence.** deck123's RULE 1 stopped the token engine on "the printed power of creatures that
do NOT read summoning sick, versus opponent life". Every token the engine makes IS summoning
sick, so the quantity in the test does not move when the action is taken. The rule was therefore
unreachable from inside the loop it governed, and the seat activated the same ability **1,868
times in one turn** while its own plan said it would stop and attack.
(`...deck123-0x55e867ceaa40-vs-...deck162.jsonl`, turn 10.)

**Proposed amendment text.**
> When you write a rule of the form "keep doing X until N", check that **taking X changes N**.
> A stop written on a quantity the action leaves untouched is not a weak stop, it is not a stop
> at all - the pilot can obey it forever. Prefer the number on the screen that the action
> increments (a count in a header, a `[repeat: N times already]` tag) over the number that is
> strategically more meaningful but static (power that excludes what you just made, mana you
> did not spend, damage you have not dealt). If the meaningful number really is the right one,
> pair it with a monotone tripwire that fires on the mechanical one.

**Why it generalises.** The same defect shape - a threshold on a quantity outside the action's
effect - is available to any guide that gates a repeatable ability, a repeated mulligan, or a
repeated decline. The deck123 case is the extreme, but wave 46's mulligan chain was the same
error in miniature: "keep until you have a keepable hand" tested a property of the hand, and
mulliganing does change the hand, which is why that one merely ran to seven and not to 1,868.

---

## S47-2 - THIRD-FAILURE ROUTING: SHRINK THE RULE, MOVE THE CHECK ONTO THE PILOT'S OWN SENTENCE

**Evidence.** deck123's RULE 5 states three times, in three separate paragraphs, that the
OPPONENT chooses which creature an edict takes. The option row prints the card text verbatim on
the same screen. Third corpus running, the pilot cast the edict at 5 creatures and at 3
creatures and wrote a survival plan around choosing a specific body. Adding a fourth statement
of a fact that is already on the screen has now been tried and measured.

**Proposed amendment text.**
> A rung that has failed in three corpora with the fact rendered correctly on the option row is
> not under-explained, and a fourth restatement is not the lever. Do two things instead:
> (a) DELETE one of the existing restatements, so the rule gets shorter rather than longer -
> length is itself a failure mode at 30-40 KB; and (b) re-point the check from the BOARD to the
> pilot's OWN PLAN SENTENCE, in the imperative, naming the exact wrong sentences the corpus
> contains: "IF YOUR PLAN NAMES WHICH CREATURE AN EDICT WILL REMOVE, THE PLAN IS ALREADY WRONG."
> A plan-text check is self-applied at the moment of writing; a board check competes with
> everything else on a 25 KB screen. Then predict the plan-text metric, not the outcome metric,
> so the next wave can tell which half moved.

**Boundary.** This is a routing rule for the THIRD failure, not the first. A first failure is
usually a missing fact and prose is the right answer; a second is usually a contradiction
elsewhere in the file (see deck130's D47-1 this wave, and D46-4 last wave). Only at the third,
with the fact rendered, is restatement measurably exhausted.

---

## Amendment I am NOT proposing, and why
The corpus tempts a "guides should teach the model to pass priority" amendment. I did not write
one: the pilot demonstrably knows how to pass (`CHOICE: 0 (pass)` twice in the dead-looped file),
and the general lesson is S47-1's, not a new doctrine about passing. The affordance half is an
engine item (H1 in the seat file), not skill text.
