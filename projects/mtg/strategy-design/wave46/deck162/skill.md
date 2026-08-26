# deck162 -> strategy-writing skill proposals (wave 46)

---

## S1 (STRONG) — A DECK THAT LOSES ON MECHANICS THAT ITS SIBLINGS' GUIDES COVER IS MISSING A
## SECTION, NOT A NUANCE; RUN A COVERAGE DIFF BEFORE TUNING ANYTHING

**Observation.** deck162 went 5-1 -> 3-3 while every wave-45 edit it received held or improved
(Rule 1 punisher-first 17/20 -> **20/21**; the LETHAL survival gate PASS on its first window).
All three losses trace to sections the guide has never contained, and in each case a sibling
guide in the same pool has the section:
- **No land rule.** 35/39 land drops taken, three declines in the game it lost (turns 3 and 5,
  holding three lands). deck146, which has one, is 43/43.
- **No lifegain / converter price.** It attacked into three 1/1 lifelink blockers with a
  `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs` line printed and went 19 -> 0 in one
  combat. deck152 has both rules.
- **No "your last blocker stays home" rule beyond one named card.** It swung its only untapped
  creature at 3 life and died at -2.
The wave-45 pass on this deck concluded "nothing to add" because everything it *had* was in good
shape. That is a different question from what it lacks.

**Proposed amendment.** *Before revising a guide, run a COVERAGE DIFF against the other guides in
the same pool: list the decision surfaces the engine can present (land drop, mana line, cast
order, attack tags, blocking ladder, mulligan, bottoming, each named opposing mechanic) and mark
which guides have a rule for each. A blank cell is a finding on its own, and it outranks tuning a
cell that is already full. A guide that scores well on every rule it has can still be the worst
guide in the pool.*

**Falsifier.** A pool in which a coverage diff produces mostly rules that never get windows, i.e.
the blanks were blank because the deck cannot reach those surfaces.

---

## S2 (STRONG) — A RULE SET KEYED TO A RENDERED STRING MUST HANDLE THE ABSENCE OF THAT STRING

**Observation.** Every rung of deck162's ATTACKING section is keyed to what the "their untapped
blockers" tag SAYS. Twice in one game the A-line carried **no tag at all** (the opponent's
battlefield line read `(0 permanents listed)`), which is free damage by construction — and the
guide matched no rung, so the executor fell back on a mood ("preserve Fate Unraveler as a
blocker") against a player with no creatures. Attack rate fell from 12/12 windows to 6/8, and
both declines are this shape.

**Proposed amendment.** *For every rule keyed to a rendered token, write the ABSENT case
explicitly. The executor does not treat "the tag is missing" as "the tag is empty of dangers" —
absence matches no rung, and an unmatched decision is decided by narrative fit. This is the
mirror of the render-side trust doctrine: the surface owes the model truth, and the guide owes
the model a branch for every state that surface can be in, including not being there.*

**Falsifier.** A corpus in which absent-token branches are never reached, or are followed no
better than the mood they replace.

---

## S3 (MEDIUM) — AN UNCONDITIONAL RUNG AT THE TOP OF A LADDER SILENTLY DELETES EVERY CONDITION
## BELOW IT

**Observation.** deck162's ladder opens "1. A line ending '(you kill it, your blocker lives)' -
take that block. Free kill, every time." Rung 3 carries the life-8 clause that would have saved
the game. Because rung 1 matched first and unconditionally, rung 3 was never reachable on that
screen: the seat took a free kill on a 2/2 while a 4/4 walked past, went 12 -> 4, and died two
turns later. Both rungs are individually correct. The ladder is what is wrong.

**Proposed amendment.** *In a top-down ladder, an unconditional early rung is a DELETION of every
conditioned rung beneath it for the cases where both match. Before shipping one, ask which lower
rung it can pre-empt; if any lower rung carries a survival condition, that condition belongs
ABOVE the unconditional rung as its own numbered step, not below it. Reuse the number the lower
rung already uses so the guide ships one threshold, not two.*

**Falsifier.** A corpus in which the hoisted survival step fires on boards that were never at
risk, costing free kills the seat needed.

---

## S4 (MEDIUM) — A RULE NAMED FOR ONE CARD DOES NOT COVER THE ROLE

**Observation.** "At 2 life your Fate Unraveler is also your blocker" is exactly right about the
role and names only one card. The card that lost the game in that role was Master of the Feast,
which the same guide separately calls "your fastest clock ... swing with it every single turn".

**Proposed amendment.** *When a rule is really about a ROLE (your last blocker, your only mana
source, your only evasive threat), define the role by something the prompt prints — "count the
untapped, non-summoning-sick creatures on YOUR battlefield line; if that count is one" — and then
name the cards as examples. A card-named rule is a rule with a coverage hole the moment the board
changes, and the executor will read the OTHER card's own enthusiastic line instead.*

**Falsifier.** A corpus in which role-defined rules over-fire, holding bodies home in races the
seat needed to win.

---

## S5 (SUPPORTING) — DO NOT LOOSEN A RULE THAT IS VIOLATED-AND-UNPUNISHED AT SMALL n

Rule 2's engine brake ran 5/7, and both breaks are the exact shape the rule names (a second
Howling Mine with a Mine out and no punisher). Both games were survivable. Per amendments 45/64
this is not evidence to loosen or re-word; it is carried forward as a prediction. Recording it
because the temptation to "fix" a 5/7 by softening the rule is what a 22/26 in wave 45 already
survived.
