# Wave-70 lane BM — every PARSER and tolerance, rebuilt around the two-line reply

Branch `w70-lane-BM`, base master 52ab71610. Files: `projects/mtg/src/AIPlayerGPT.cpp`,
`projects/mtg/include/AIPlayerGPT.h`, `projects/mtg/include/GptPlanCaveat.h`.
Comment tag on every change: `#W70-BM (E1..E6)`.

## The mechanism, in one paragraph

The reply is TWO LABELLED LINES: `PLAN:` first (a sequence of intended actions across the
coming windows), then the action line for THIS window (`CHOICE:` / `ATTACK:` / `BLOCKS:` /
`PUT:`), and nothing else. The action line is STEP ONE of the plan. Everything this lane did
follows from that: the reader locates the two lines and measures what sits outside them; the
parsers that assumed the answer came FIRST and the plan LAST were fixed; every tolerance that
existed only because the model used to reason in the reply was DELETED, with its PARSETEST
cases; and the plan is carried as a SEQUENCE whose first step is consumed when the action runs.

## Per-row disposition (audit Table B)

| Row | Verdict in the audit | What shipped |
|---|---|---|
| B1.1-B1.3 | KEEP | untouched |
| B1.4 | VERIFY | KEPT + gameend census `reasoning_tail_answers` |
| B1.5 | VERIFY | KEPT + gameend census `phase2_answer_recovered` / `phase2_answer_missing` |
| B1.6 | CHANGE (cap) | **NOT MINE** - cap arithmetic is lane BK (audit section C item 4); flagged below |
| B2.1 | CHANGE | markdown-decoration skip reduced to whitespace; heading skip gone |
| B2.2 | KEEP (strip skips) | skips stripped with B2.1 |
| B2.3 | KEEP (strip skips) | `lineIsCodedAnswerLine` skips stripped; the literal `PLAN: ...\nCHOICE: N` shape is now PINNED (four seams) |
| B2.4 | DELETE | `answerHeadingSkip` DELETED (header) + all 14 call sites |
| B2.5 | CHANGE | `findAnswerLabelLine` rewritten: first line-leading label wins; run-fold, correction cues, three-line announcement window, plan-block demotion all DELETED; new out-param `extraAnswerLines` |
| B2.6 | DELETE (arm) | `gptSelectAnswerIndex` collapsed to (usable, clean) - supersession arm DELETED |
| B2.7 | DELETE | `gptAnswerCorrectionCue` DELETED |
| B2.8 | DELETE | `correctionHeaderCue` + `textHasCorrectionHeader` DELETED; `planTruncationNote`'s `droppedCorrection` clause DELETED |
| B2.9 | CHANGE | an unclean answer line still loses to a clean one and is counted (`rejected_line_skipped`); it is no longer a ranking input beyond that |
| B2.10 | CHANGE | combat drivers: single-line; prose-restatement candidate and correction arms DELETED; `extra_answer_line` replaces `later_answer_ignored` |
| B2.11 | DELETE | `restatedCombatDirective` DELETED (+ both live call sites) |
| B2.12 | DELETE | `salvageLoopedChoice` DELETED (+ 3 live call sites, incl. the no-pass sibling) |
| B2.13 | DELETE (salvage) | `salvageLabelMissingChoice`, `sentenceNegatesBefore`, `gptRowShortName` DELETED; `replyLabelMissing` KEPT as the R4 re-ask trigger |
| B2.14 | DELETE | `choiceRetractedNoReplacement` DELETED (+ both live gates; `retracted` survives only as a fallback class name) |
| B2.15 | KEEP | untouched (record only) |
| B3.1 | CHANGE | both 300-byte legacy head paths DELETED; plan-block demotion + `plan_answer_line_*` stamps DELETED; `kPlanAnswerExtraLine` is the one shape left to sign |
| B3.2 | KEEP | untouched |
| B3.3 | VERIFY | KEPT + gameend census `plan_paragraph_bound_cuts` |
| B3.4 | KEEP (minus clause) | clause DELETED; the carry is now step-bounded (below) |
| B3.5 | KEEP | untouched |
| B4.1 | KEEP | untouched |
| B4.2 | CHANGE | the 200-char PROSE-DECLINE sniff DELETED - only the `none`/`nobody` token declines; the PUT-seam "no PUT: line -> any labeled answer" fallback DELETED (both sites) |
| B4.3-B4.4 | KEEP | untouched |
| B4.5 | CHANGE | `replyTerminatedNaturally(content, finishLength)`; the PLAN-as-terminal rule and the legacy short-answer arm DELETED; `truncatedBlockCommitmentAbandoned` takes the same flag |
| B4.6 | CHANGE + VERIFY | PUT fallback DELETED (with B4.2); gloss strip KEPT + gameend census `put_gloss_stripped` |
| B4.7-B4.8 | KEEP | untouched |
| B5.1 | VERIFY/CHANGE | `planSaysPassThisWindow` now reads the PLAN LINE wherever it sits (the `fromStripped` origin that started at the latched CHOICE line is DELETED) + census `plan_choice_conflict_seen` |
| B5.2 | DELETE | the sentence-opener half DELETED (openers/leads/deferral tables gone) |
| B5.3 | DELETE | `proseReversesInCorrectionZone`, `correctionZoneEnd`, `reversalScanText`, `proseReversesDeclaration` DELETED |
| B5.4 | DELETE | `proseNegatesTakenRow` DELETED (+ both seams) |
| B5.5 | DELETE | `proseNamesOtherMenuRow` DELETED (+ `plan_choice_conflict_no_rival_named`) |
| B5.6 | VERIFY | `planArguesAgainstRow`'s WHOLE-REPLY fallback DELETED (no PLAN marker -> false) + census `plan_argues_against_row_seen` |
| B5.7 | KEEP | untouched |
| B5.8 | DELETE | `decision_reversed_in_prose` KEPT for the one surviving shape (the PLAN line's verdict); `reversal_names_taken_row` DELETED |
| B6.1 | KEEP | `post_answer_overrun` untouched |
| B6.2 | CHANGE | `preAnswerOverrun` DELETED -> `offProtocolBytes` (bytes outside the two permitted lines, both sides) |
| B6.3 | CHANGE | `postPlanOverrun` and `post_plan_overrun` DELETED; the record trim is keyed on the TRAILING off-protocol bytes |
| B6.4 | KEEP | `answer_replaced` / `coded_answers` untouched |
| B6.5 | CHANGE | `later_answer_ignored` -> `extra_answer_line` (a violation, stamped; the first line still runs) |
| B6.6 | DELETE | `multi_answer_first_taken`, `plan_answer_line_ignored`, `plan_answer_line_only` DELETED |
| B6.7-B6.9 | KEEP | untouched |
| B6.10 | DELETE | both 300-byte ramble guards DELETED with B3.1 |
| B6.11 | KEEP | untouched |
| B7.1-B7.10 | (section C) | **NOT MINE** - lane BK owns the regime and the cap arithmetic |

Beyond the audit's named rows, one more prose-as-answer tolerance was deleted under the same
rule and is called out here rather than buried: the BOTTOM seam's `salvageProsePutList`
reconciliation, which preferred a sentence ("So I bottom 5, 6, and 7.") over the reply's own
`PUT:` line whenever the reply terminated naturally. Its gate was `replyTerminatedNaturally`,
which this lane redefined, so leaving it would have changed how often it fired rather than
whether it should.

## The plan is a SEQUENCE (invariant 000(e))

- `gptcaveat::planStepEnds` / `planStepCount` / `planStepsAfter` / `planStepBound` /
  `planCarryComposeSteps` (new, in GptPlanCaveat.h). A step ends at `; , . ! ?` or a newline
  FOLLOWED BY whitespace or end-of-string, so "1,000" and "3.5" do not split a step. The
  helpers return OFFSETS, so every join is the original text byte for byte.
- The carry is bounded by STEPS (`kPlanCarryMaxSteps = 12`) and the character ceiling
  (`kPlanCarryMaxChars`, UNCHANGED at 400 so the protocol's stated number stays true) can only
  cut AT a step end. The truncation note now names how many LATER STEPS the cut dropped -
  the failure the brief names (dropping step 3 for bytes) is now impossible in silence.
- STEP ONE IS CONSUMED BY THE ACTION. `mPlanStepsDone` resets to 0 when a reply states a
  CHANGED plan and advances by one when a reply carries the same plan (or omits PLAN, which
  carries it), because that window's action executed the step the window was served. The
  prompt is served `planStepsAfter(mCurrentPlan, mPlanStepsDone)` - the REMAINDER. The pointer
  is clamped to the last step so a fully-walked plan carries its final step rather than
  vanishing, and it is cleared with every `mCurrentPlan.clear()` expiry (4 sites).
- `mPlanStepsDone` cannot change between two rebuilds of the SAME window (it moves only when a
  reply is consumed), so nothing volatile enters `mPromptTail` or the ask key.

## Census counters added (gameend record)

`protocol_replies` (the denominator - replies that carried a body), `action_before_plan_replies`,
`reasoning_tail_answers` (B1.4), `phase2_answer_recovered` / `phase2_answer_missing` (B1.5),
`plan_paragraph_bound_cuts` (B3.3), `put_gloss_stripped` (B4.6), `plan_choice_conflict_seen`
(B5.1), `plan_argues_against_row_seen` (B5.6). Per-record: `off_protocol_bytes` (every record),
`action_before_plan` (present only when true). Removed from the record: `post_plan_overrun`,
`pre_answer_overrun`, `reply_overrun`.

The six VERIFY rows keep their mechanism and get a counter each, so the probe corpus decides
them from counts rather than from code reading. Every counter is written beside a denominator
that is also written, so a zero can be told from silence.

## action_before_plan

A reply that writes its action line ABOVE its plan line is still PARSED - a decision is never
thrown away over line order - but it is stamped `action_before_plan` on the record and counted
in the gameend census. It is never treated as the intended shape.

## Gate numbers

- PARSETEST: **5248 passed, 0 failed** (base on this worktree: 5420/0). 203 cases deleted with
  the mechanisms they pinned, 39 added. Log `~/.gatelogs/w70-BM-pt7.log`.
- Suite `WAGIC_TESTSUITE_THREADS=1`: **1279 tests (0 failed), 67 AI tests (0 failed), 0
  timeouts** - identical to the base run on this worktree (`~/.gatelogs/w70-BM-base-suite.log`
  vs `~/.gatelogs/w70-BM-suite2.log`).
- Build: clean link, binary freshness-checked after every make (`ls -la bin/wagic` in each
  build log; the stale-binary trap fired once during this lane - `w70-BM-build3.log` had 4
  errors and PARSETEST re-ran the PREVIOUS binary, which is why the check is in every gate).
- `git diff | grep -c $'\357\277\275'` = 0.

## RED evidence

The mechanism change is proven by 40 SHIPPED PARSETEST cases that were GREEN on the base
binary and went RED the moment the tolerances were deleted (log `~/.gatelogs/w70-BM-pt2.log`,
built from this worktree's base). Every one of them pins a behaviour invariant 000 forbids -
a heading before the label, a correction superseding an answer, a prose sentence read as a
declaration, a pass verdict read out of prose, a run of answer lines folded into one:

- B retraction followed by a NEW choice takes the new one
- N9-W23 (a) -> #W48-E1: a trailing prose re-answer REPLACES the resolved answer (was -1 before wave 48)
- W25-1 genuine -> #W48-E1: an unquoted trailing recode contradicting line 1 is taken as the decision (was -1 before wave 48)
- W36-B3 POSITIVE a consecutive CHOICE block answers with its FIRST line (run reported)
- #W65-AO G8 POSITIVE a MARKED correction still supersedes
- #W65-AO G8 POSITIVE the marker on the correcting line itself also binds
- #W65-AO G8 the first BLOCKS line is the answer (the rule is label-wide)
- #W65-AO G8 ...and a marked BLOCKS correction still supersedes
- W36-B3 a blank line does not break a pick-list block
- #W48-D2 echo NEGATIVE the word 'all' inside a decline never alpha-strikes
- #W65-AO G8 POSITIVE the same shape MARKED supersedes (2)
- #W50-Y D7 REGRESSION the run-head rule (W36) is untouched: the first of an adjacent run
- #W52-J D14 deck146 vs125 seq 282: 'We must pass and hope' after CHOICE: 1 (Lolth 0) is the pass verdict -> plan_choice_conflict re-ask, quoting that sentence
- #W52-J D14 'So we pass.' opener (lead word) is the verdict
- #W52-J D14 'The answer is pass' on an unlabelled line after the CHOICE is the verdict
- #W52-J D14 'I must pass' opener is the verdict
- #W56-C D3 the pass verdict is BEFORE the latched line, so the region test clears it (this is the Starstorm cycle the wave-55 notice destroyed)
- #W62-Z D10 ...and the shape is signed plan_prose_recode_ignored rather than silent
- #W64-AK R5 ...and the bounded walk now AGREES with the unbounded one, which is the disagreement the finding is about
- #W65-AO G8 ...and the answer that stands is the LEAD line, the later one being unmarked
- #W65-AO G8 146v130 s93 the refused later line is reported, so the seam can sign it
- #W65-AP R3 POSITIVE the marked correction on the very next line is the answer
- #W65-AP R3 MUST-NOT-MATCH an adjacent ranked list still answers with its head
- #W65-AP R3 NEGATIVE a cue in the FIRST line's own tail does not promote the line under it to a correction
- #W66-AR H2b REPRO 123v126 seq 36 - the announced correction two lines up is read, and the retracted x30 no longer runs
- #W66-AR H2c the refused line is recorded VERBATIM, from the full reply
- #W66-AR MED-2b the note says the cut took a retraction with it
- #W67-AV I1 162v123 s13 `ANSWER: CHOICE: 3 (Cast nothing right now)` is an answer
- #W67-AV I1 ...and it resolves to row 3 through the ordinary parse
- #W67-AV I1 130 s52 `CORRECTION: CHOICE: 3` is a MARKED correction, and it wins
- #W67-AV I1 126v162 s10 `ANSWER: PUT: 44` is an answer at the reveal seam
- #W67-AV I1 the ATTACK seam skips the heading too
- #W67-AV I1 collectLabeledLines (the combat/PUT drivers) skips the heading
- #W67-AV I1 `The answer is CHOICE: 2 ...` is an answer
- #W67-AV I1 the record's coded-line counters and the latched line agree with the seams
- #W67-AV I1 ECHO the stored line starts at the label, at its true offset, unannotated
- #W67-AV I3 123v126 s114 the announced correction wins over the retracted Damnation
- #W67-AY I9b POSITIVE a reply with no PLAN label is still a reply with an argument
- #W68-BA J6 MUST-NOT-MATCH the CHOICE arm still answers exactly as #W52-J shipped it
- #W69-BF K3 REPRO 125v162 s19: the unbounded predicate still sees the plan body's "I will pass" - nothing was deleted

Those 40 are the inverse pins of the new MUST-NOT-MATCH cases: `#W67-AV I1` asserted that
`ANSWER: CHOICE: 3 (...)` IS an answer and passed on base; `#W70-BM E2` now asserts it is a
violation. All 40 were deleted with their mechanisms.

**Weakest evidence / what I did NOT verify.** A cases-only-on-base build is not compilable -
the new pins call helpers that do not exist on base (`offProtocolBytes`, `replyActionBeforePlan`,
`planStepsAfter`, `replyTerminatedNaturally(content, bool)`, `findAnswerLabelLine`'s new
arity) - so the new two-line pins were never RUN against the old mechanism. Their RED evidence
is the inverse-pin argument above, not a red run. Nothing in this lane was validated against a
LIVE model: the step-consumption pointer (`mPlanStepsDone`) is the least evidenced piece -
its advance rule is inferred from "the action line is step one", and a window that re-asks
calls `consumePlan` twice, which advances the pointer twice; the clamp in `planStepsAfter`
bounds the damage to serving a later step early, never to an empty plan, but the true rate is
a corpus question. The step SPLIT grammar (`; , . ! ?` + newline) is measured against no
corpus of thinking-on PLAN lines, because none exists yet. `off_protocol_bytes` is pinned on
synthetic strings, not on corpus replies.

## Predictions for the wave-70 probe corpus

1. `off_protocol_bytes` is 0 on the large majority of records; where it is non-zero the bytes
   sit BEFORE the PLAN line (a preamble), not between the two lines.
2. `action_before_plan_replies` is small but non-zero, and falls between the first and last
   games of a run as the narration's own examples accumulate.
3. `coded_answers != 1` and `extra_answer_line` are near zero; if either is not, the
   instruction side (lane BL) is what moves it, not this lane.
4. `plan_choice_conflict_seen` is NON-zero where wave 69's `decision_reversed_in_prose` would
   have read 0 for the wrong reason - that is the silent-instrument fix landing.
5. `reasoning_tail_answers` is non-zero on the vLLM reasoning parser (audit B1.4's open
   question); if it is, the PLAN written inside the reasoning field is being dropped and B1.4
   becomes a CHANGE next wave.

## For the merge

- I edited NO instruction string. Two touch the same lines lane BL will edit and are flagged:
  (a) the priority-seam plan-conflict re-ask lost its "your reply then says you should NOT do
  it" ARM (the predicate that selected it is deleted) - the surviving wording is unchanged;
  (b) the ask-seam equivalent likewise. If BL rewrites either, take BL's text.
- The reply protocol's "the FIRST 400 CHARACTERS of your PLAN line" claim is still TRUE:
  `kPlanCarryMaxChars` is unchanged; the cut now lands at a step boundary at or below it.
- Lane BK owns every B7 row (the regime, `gptSeamMaxTokens`, `WAGIC_GPT_MAXTOKENS`, the
  answer-locked retry cap) and audit B1.6. This lane changed none of them.
