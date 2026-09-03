# deck123 — proposals for the strategy-writing skill (wave 54)

Only what this seat's evidence supports. Numbering continues from the current edition (#208).

## S1 — A GUIDE MAY NOT QUOTE AN ENGINE STRING WITHOUT AN AUDIT SLOT
**Evidence.** Both of my guides quoted `"Hold priority - do not ask me again this turn unless the
board changes"`. Lane A reworded that row; the old string rendered **0 times** in this corpus and
the new one **286 times**. Two guides were describing a row that no longer exists, and the only
reason it was caught is that the reviewer brief mandates the B1 REVERSE audit.
**Proposal.** The skill already has #188 (three-number literal audit) as a *reviewer* obligation.
Add the *author-side* half: every guide sentence that quotes an engine literal is written so the
literal is the only thing that has to change — quoted on its own, never paraphrased into the
sentence's grammar. A guide sentence that dissolves a literal into prose cannot be audited by
grep, and this pool now has 12 guides in the 39-70 KB range.

## S2 — A GUIDE'S REPLY TEMPLATE IS PART OF THE PARSER'S INPUT SURFACE
**Evidence.** 7 of my seats' 16 fallbacks are `plan_choice_conflict` firing on the PLAN shape the
deck123 guide itself mandates (`stop <L+C+3>; M is <M> now; this window: ...`). The model writes
"(stop reached)" as arithmetic commentary, the parser reads a reversal, four re-asks return the
identical reply and the windows are spent (`123v126` s29/s30, s58/s59; `123v146` s23/s24, s26/s27).
**Proposal.** When a guide prescribes a PLAN/reply template, the author must check it against the
parser's reversal and latch checks, and the template must name the action in exactly ONE field.
Vocabulary that also names a row ("pass", "stop", "hold", "none") may not appear anywhere else in
the template. This is a composition-seam defect: the guide was right, the parser was right, and
the pair was wrong.

## S3 — "THE GUIDE LANE IS EXHAUSTED" NEEDS A NAMED EXIT, AND THIS CORPUS PROVIDES ONE
**Evidence.** Wave 53's P7 said: if the `Damnation: N, K, M` line is written 0 times again, the
lane is dead and the ask becomes a row annotation. This corpus it was written — `123v125` s176
`PLAN: Damnation: N 0, K 0, M 0; cast.` — and the dead Damnation was cast on the same line. The
guide had the rule, the row had the verdict, the PLAN had the numbers, and the play was still
wrong. Four corpora.
**Proposal.** Make the exit test two-sided, not one-sided: a rung is exhausted when EITHER the
mandated artefact is never produced, OR it is produced and the decision does not change. In both
cases the next edit is a render/prompt ask, and the guide's remaining job is to shrink to the
rule plus one citation. Without the second clause a rung can absorb prose forever by "improving"
its compliance rate.

## S4 — A RULE MAY NOT CONTRADICT A TRUE RENDER, AND THE REVIEWER MUST CHECK WHICH IS TRUE
**Evidence.** `123v126` s88. The guide's Branch B arithmetic (A >= 3B + 3) and its lethal override
both said ATTACK with 58 creatures against 6 blockers at 28 life; the render said
`their life LOOP is in play: any life they gain or you lose in this combat chains without limit -
fatal to you, not a trade`. The pilot obeyed the render and was RIGHT — Sanguine Bond
(mtg.txt:99004) plus Exquisite Blood (mtg.txt:37966) is a closed loop and the blocking triggers
resolve before combat damage. I nearly filed this as the corpus's second render falsehood.
**Proposal.** Reinforce the trust doctrine with its author-side obligation: where a guide rule and
an emitted clause can disagree on the same board, the guide must say which wins and why, in the
rule. And on the reviewer side — before writing "the render is false", check the primitive AND
the timing of the trigger, not just the numbers. Two of my four candidate HIGH render items
dissolved on that check.
