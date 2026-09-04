# deck162 -> strategy-writing skill: proposals from wave-57 evidence only

## S3 (deck162's evidence, cross-filed from deck146/skill.md) - PROSE BRAKES DO NOT BIND AT THIS TIER; A BRAKE MUST BE AN OPERATION WITH A VERDICT

`162v125` seq 57 is the cleanest instance the pool has produced: the reply states the brake in the
guide's own terms ("with no converters on the battlefield, we cannot safely stack more draw
engines") and takes the braked row in the same record. Four more in the same game, three of them
inside an exception's stated exclusion. The rule was 40 lines of correct, evidenced prose with the
right literal in it.

Proposal: the skill should distinguish two rule shapes and say when each is used.
- A **heuristic** ("prefer X to Y when Z") may be prose; it competes with other considerations and
  losing sometimes is expected.
- A **brake** (a rule whose whole purpose is to veto an otherwise attractive row) must be written as
  a check with three parts: (1) the literal to look for, on the ROW that has already been chosen;
  (2) the release conditions, read off named lines of the prompt; (3) an explicit verdict -
  "your answer is wrong, change it before you send it" - and an instruction to run it LAST.
The five wave-57 breaks all had the rule stated EARLY in the reply and the wrong row named LATE,
which is what "run it last" is aimed at, and which is a falsifiable design claim: if 162-A still
fails in wave 58, ordering is not the mechanism and the item belongs to the core loop.

## S8 - AN EXCEPTION IS WHERE A BRAKE LEAKS; COUNT ITS EXCLUSION, NOT JUST ITS TRIGGER

deck162's brake has exactly one exception (Master of the Feast is a body when they have a creature)
and three of this corpus's five breaks were Master casts **at `of which 0 are creatures`** - that
is, inside the exception's exclusion, taken as if the exception were unconditional. The rule stated
the exclusion clearly; the model took the exception's existence as the permission.

Proposal: when a guide adds an exception to a brake, the skill should require the exception to be
written with its exclusion in the SAME breath as its permission and to be counted separately in the
next wave's audit ("exception taken correctly: N; exception taken outside its condition: M"). An
exception whose M exceeds its N is not a carve-out, it is a hole, and the reviewer should be
counting for that.
