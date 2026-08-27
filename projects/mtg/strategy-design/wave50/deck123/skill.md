# deck123 - proposals for the strategy-writing skill (wave 50)

Each proposal names its evidence; synthesis owns adoption.

**S1 - Under an answer-first reply protocol, a dictated plan shape's blanks are filled BEFORE the
first line, and the guide says so.** #120 put the three blanks in the PLAN line and it worked: 8/8
repeat-row replies carried a stop number (wave 49: 0/15) and the overshoot fell from 300 to 28. The
residual is structural: the CHOICE line is written first, the subtraction second, and four times
the first line named a count (x0, x1, x3, x25) that the plan beneath it then refused ("this window:
pass" - 123v162 seq 35, 123v130 seq 31/46, 123v126 seq 32). Rule: where the protocol commits on
line one, the guide's plan shape is introduced with "do the subtraction before you write the
first line; the CHOICE line is the plan's `this window:` blank, not a guess", and the reviewer's
metric is "replies whose CHOICE count disagrees with the PLAN line's answer" (0/N is the pass).
Companion for the engine seat: a `plan_choice_conflict` re-ask (seat M1).

**S2 - A named count of zero is a pass, and every guide that teaches a count teaches that "x0" is
not one.** 123v162 seq 35: `x0` executed once (`ran 1 time (you named 0)`, `parse_note:
repeat_count_under_two`). The pilot used the row's own grammar to say "none" and the engine turned
it into a take. Until the engine half ships (seat H1), a repeated-action rule states the pass row
literally ("CHOICE: 0 (pass) - never x0") - a one-line addition wherever #104's count grammar is
taught.

**S3 - Two sentences that disagree on the same creature are found by the #131 method applied to
RULE-level pairs, not only to mulligan sections.** RULE 5 carried "N = 1 is the cast" and "power
0 or 1 - not hurting you - keep the edict"; at 123v152 seq 6 they disagreed on a 1/1 lord (Katilda,
whose card text gives Humans mana and counters) and the pilot held; she was a 3/3 by turn 10 with a
board behind her. Rule: for every pair of rules keyed to overlapping conditions (N = 1 / power <= 1;
K = 0 / walls named by another check; hand size / an edict on the same menu) the reviewer
constructs the boundary case and confirms one verdict. This corpus found three such pairs in one
guide (A50-4's K-0-on-Captains, A50-10, A50-12).

**S4 - When a lane turns an engine choice into an ask, the guide sentence that described the
engine's behaviour is retired the same wave, and the rule that avoided the decision becomes a rule
that ANSWERS it.** A49-8 ("the engine - not you - chooses what you discard ... cast Skeins at N of
5 or fewer") was written for wave-49 H2; lane W shipped the `Cleanup step` ask and the pilot chose
sanely 7/7 (Alarm copy 1 of 2, Damnation copy 1 of 2, Skeins). The floor-shaped half failed 3/3
anyway. #127 said the mitigation is written as a fact about the cards; the corollary: when the ask
arrives, the guide's job moves from "avoid the window" to "what to PUT" (A50-9). The reviewer's
checklist item at each lane merge: "which guide sentences describe an engine behaviour this lane
changed?" (the #128 grep finds them - "the engine discards" had zero matching prompts this corpus).

**S5 - A rule keyed to a card class is re-keyed to the check that names the class.** RULE 3's "K
of 0 - keep the sweeper" was written for two 0/4 walls; at 123v126 seq 17 the walls were Perimeter
Captain x2 + Overgrown Battlement - the exact names CHECK 1 forbids attacking into - and the pilot
broke the rule correctly (won -208). The right key was already in the guide, in another section.
Rule (extends #121's "soften an absolute with the row's count"): when two sections of one guide
name the same cards, the exception is written as a cross-reference to the other section's names
("K = 0 on Perimeter Captain or Pride Guardian: CHECK 1's wall - sweep them"), never as a new
prose class.

**S6 - A third-corpus recurrence of a quoted-misbelief rule routes to the render, and the guide
keeps one line.** Idyllic Tutor with the Alarm owned: wave 49 3 casts, wave 50 2 (123v125 seq
118, 139), the reply's own words matching the guide's quoted anti-example each time, "Tutor: dead"
written 0 times across both corpora. #132 (a test that cannot be validated twice -> row annotation
regardless) applies to a rule that FAILS the same way twice as well: the seat file carries the
render ask (M2: `{Intruder Alarm is already on your battlefield / in your hand}`) and the guide's
rule is left at one restatement (A50-5), not a fourth paragraph.
