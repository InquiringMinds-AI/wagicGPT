# Strategy-writing skill proposals from the deck146 seat (wave 47)

Only what this seat's corpus supports. Two proposals, one of them a correction of my own
wave-46 edit.

---

## PROPOSAL S-1: a rule whose threshold counts printed items must state what happens when the
## surface collapses that list.
**Evidence:** deck146's lifegain-wall rule said *"if TWO OR MORE blockers on this creature's tag
print a '(blocking trigger:' clause"*. Lane M's ranged collapse and the existing wide-board
collapse mean a wide tag prints **exactly one** blocker (`they have 5 untapped creatures that
could block this one, biggest Vampire (1/1)`). The threshold was therefore unsatisfiable on
precisely the boards it was written for, and at `146 vs126` seq 50 the seat paid **eleven life in
one swing** to a rule that could not fire. The guide-side fix was to drop the count; the general
lesson is upstream of that.
**Proposed amendment text:** *A rule may not be keyed to a COUNT of items the render is allowed to
collapse. Before writing "two or more X on the tag", check whether that list has a collapsed form;
if it does, key the rule to the PRESENCE of one X plus the collapse's own count word ("they have N
untapped creatures"), never to the number of X you can see. When the surface summarises, a count
you can read is a lower bound, and a rule that treats it as the truth is a rule that silently
switches off on big boards.*
**Falsifiable:** any guide rule in the pool phrased "two or more ... print" that governs a list
with a collapsed form is a defect; a sweep should return zero after this amendment.

## PROPOSAL S-2: a permission clause with no observable becomes the default answer within one
## corpus. Ship permissions with a WRITE-THE-NUMBERS gate.
**Evidence, and it is my own edit failing:** wave-46 edit 146-C added CHECK 0 to the Hive rule -
*"read the opponent's printed life, then add the power of every creature of yours that is
UNTAPPED ... if that total is at or above their printed life, ANIMATE NOW ... whatever phase"* -
above an absolute *"If it says Upkeep, the answer is PASS. There is no exception to this."* It was
paid for by a real loss (wave 46, opponent at 1 life in upkeep). This corpus: Hive offers fell
46 -> 27 but takes rose **3 -> 12**, and **10 of the 12 are Upkeep animations at life totals CHECK
0 cannot reach** (opponents at 15, 12, 12, 6, 6, 5, 5 - and one at own-life 2 against 19). The
absolute did not survive the arrival of an exception with a soft trigger.
**Proposed amendment text:** *When a rule adds an EXCEPTION above an absolute, the exception must
carry an observable the reply has to emit, not merely a condition the model has to feel. Write it
as "compute X and Y and put both numbers in your reply; if the numbers are not in the reply the
exception did not fire". An arithmetic exception with no written-out arithmetic converts the
absolute it sits above into a preference, and the corpus will show the exception firing on the
majority of windows within one wave.*
**Falsifiable:** deck146 non-lethal Upkeep animations return to 0-1 next corpus, with the
"WRITE THAT SUM" clause shipped; if they stay high, the gate is not the fix and CHECK 0 should be
cut back to a main-phase-only permission.

---

## Not proposed
- I have no evidence for a general amendment about `Cast nothing right now`. deck146 took it 20
  times with a live option, but deck152's identical-looking 26 takes are almost all licensed by
  its own exception clauses, and deck162 took it 6 times. The failure is deck146-specific (its
  entry 9 has no accountability sentence) and is fixed at the guide, not the skill.
