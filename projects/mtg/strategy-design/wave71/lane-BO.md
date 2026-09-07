# Wave 71 — lane BO: the revert list + the meters

Base `77d82bbe6`. Evidence: `wave70/known-bugs.md` §R, L9, L10, LOW-3 and
`wave70/engine-seat.md` §1 (the compensation census) and §3 (the corpus census).
Corpus re-measured directly: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849`
(2,119 model decisions, 40 completed seats, reasoning ON).

Nothing in this lane touches the reply protocol, the thinking regime or the harness
flags (invariant 000(c)). Nothing here caps a CHOICE: no row is withheld, no window
is closed, no answer is clamped — the four re-asks the census kept still fire.

---

## R1-R9 — what was deleted, counted

| row | verdict | deleted |
|---|---|---|
| R1 `pre_answer_overrun` / `post_plan_overrun` / `reply_trimmed_bytes` | DONE | 1 function (`recordReplyTrimmed`) + its 2 constants, 1 record field (`reply_trimmed_bytes`), the `trailingOut` parameter of `offProtocolBytes` and its two state variables, 1 PARSETEST case. The writers were already gone in BM; this removes the reader, the trim and the re-key. |
| R2 anything REACTING to `post_answer_overrun` / `reply_overrun` | **already clean** | audited: `postAnswerOverrun` has exactly two callers, the record writer and PARSETEST — no branch reads it. **0 deletions.** The counter is KEPT (2 of 2,119). |
| R3 truncation machinery | DONE | 5 functions (`commitRetracted`, `salvageProsePutList`, `proseNegatedBefore`, `smallSeamTruncationReaskLine`, `combatTruncationReaskLine`), the `reply_truncated_reask` arm at **all 7 seams** (ask, priority, attackers, blockers, reveal, bottom, discard), 4 state members (`mAttackTruncReaskTurn`, `mBlockTruncReaskTurn`, `mSmallTruncReaskKey`, `mSmallTruncReaskLine`), 1 record field (`commit_retracted`), 12 parse-note strings (`reply_truncated_reask_{exhausted,recovered,unanswered,no_prior_answer}`, `reply_truncated_answer_{changed,unchanged}` × 2 seams), 1 dead `if (false)` stub at the bottom seam, and 24 PARSETEST cases in 4 blocks. |
| R3b the owed ANSWER-ceiling re-fit | DONE | see **the ceilings** below. |
| R4 reversal remains | DONE | 4 functions (`planSaysPassThisWindow`, `planArguesAgainstRow`, `replyProseAfterChoice`, `negationGovernsRowName`), 2 census members + 2 gameend fields (`plan_choice_conflict_seen`, `plan_argues_against_row_seen`), 1 record note (`plan_contradicts_noop_row`), the `decision_reversed_in_prose` stamp at both seams, the `plan_choice` re-ask-kind arm at both seams (`plan_choice_conflict_{exhausted,recovered,unanswered}`, `reversal_reask_same_answer`), and 36 PARSETEST cases. |
| R5 `index_name_conflict` / `distinct_index_reask` RE-ASKS | DONE | the index/name re-ask at both seams (trigger, branch, `index_name_conflict_{exhausted,recovered,unanswered}` arm) and the discard seam's `distinct_index_reask` + its 2 stamps. **KEPT**: the parser notes `echo_index_conflict`, `index_name_unique_name`, `name_over_index`, `index_name_conflict`, and the `duplicate_index_deduped` / `duplicate_index_short` stamps — all 5 corpus occurrences recovered without a re-ask. |
| R6 `repeat_count_reask` / `repeat_past_stop` re-asks | DONE | both branches, the `repeatBaseTaken` / `stopTestCount` / `repeatPastStop` predicates, the `repeat_past_stop` re-ask-kind arm **including the stated-stop CLAMP** (`repeat_clamped_to_own_stop`, `stop_conflict_single_activation_executed`, the narration receipt) and the `repeat_count_reask_{recovered,exhausted}` stamp. **KEPT**: `repeat_annotated_take`, the stated-stop STORE (`mStatedStop`/`mStatedStopCount`) and everything it renders. |
| R7 `reasoning_tail_answers` | DONE | 1 member, 1 gameend field, 1 increment. `answerTailFromReasoning` itself is KEPT — it is the mechanism, not the counter. |
| R8 plan paragraph-bound machinery | DONE | 1 function (`planParagraphBound`), 1 member + 1 gameend field (`plan_paragraph_bound_cuts`), the call site, 11 PARSETEST cases. **KEPT**: the 400-character carry cut (`kPlanCarryMaxChars`) — max PLAN 422 B, 2 records ≥ 400. |
| R9 dead re-ask kinds | DONE | `label_missing_reask` at both seams (trigger `replyLabelMissing`, both branches, both exhausted arms). `truncated_abandoned*` / `attack_*` / `blocks_*` re-ask kinds: **confirmed already removed** (W70-BN F1), residue was 3 comment blocks. `noop_plan` is NOT a logged re-ask kind — it is the internal tag of the KEPT `noop_row_zero_reask`; no residue to delete. |

**KEEP rows, verified untouched:** `noop_row_zero_reask` (still the only re-ask left at the ask seam beside `named_row`/`no_pass`, and at priority beside `named_row`/`plan_missing`), HOLD / mana window skips, `identical_ask_answers_reserved`, `identical_option_asks_resolved`, async drops, `answer_replaced`, multi-answer latching, `off_protocol_bytes`, every `reasoning_*` field, the answer-locked retry. **UNTESTED rows untouched:** `reserve_decline_windows_skipped`, `engine_reveal_floor_picks`, `wall_miss_*`, `blocker_forecast_collapsed`, `put_gloss_stripped`, `draw`, and the `bottom` seam's answer ceiling (0 windows).

**Totals:** 10 functions, 7 state members, 5 record/gameend fields, ~25 parse-note strings, 7 seams' worth of re-ask branches, **~87 PARSETEST cases** deleted; 1,473 lines removed against 396 added.

---

## The ceilings (R3's second half / LOW-3), re-fitted from THIS corpus's bytes

Re-parsed every reply of `matchups-20260906-224849` into its PLAN line + its action
line — which is the whole reply for 94.6% of them — and took the bytes per seam.
Script: `lane-BO-ceiling-red.py` (RED on base, exit 1).

```
seam           n    p99  p99.9      max   fitted   base   ratio
ask         1522    317    417      457      288    768   2.67x
priority     411    317    451      451      288    768   2.67x
attackers     78    231    231      231      256    768   3.00x
blockers      20    195    195      195      256    896   3.50x
discard       67    302    302      302      256    512   2.00x   (full-reply p99.9)
reveal        20    161    161      161      256    512   2.00x
bottom         0      -      -        -        -    512   UNTESTED
```

**The rule, pinned in PARSETEST as arithmetic and not as seven hand-picked numbers:**
`ceiling = ceil(p99.9 × 2 / 3.15 B-per-token)`, rounded up to a multiple of 32,
**floored at 256 tokens**.

- `3.15 B/token` is #W69-BF K2's own MEASURED worst case (p10 over the replies that
  reached a cap), not the median 3.61 — a cap sized on the median is a cap that bites
  the verbose tail.
- **The floor, justified:** 256 tokens buys 806 B at 3.15 B/tok. The largest single
  reply at ANY seam in the whole corpus is **457 B**, so the floor covers the global
  worst case 1.76× over and covers the largest seam p99.9 (451 B) 1.79× over. No seam
  whose fitted value falls under the floor can be cut by it. The two seams above the
  floor (ask, priority → 288 = 907 B) carry 2.2× their own p99.9 and 1.98× the global
  worst reply.
- `bottom` had **0 windows** in this corpus. A zero is not evidence (the census's own
  rule), so it is NOT re-fitted and keeps 512.
- This is an ANSWER ceiling only. `gptResolveMaxTokens` ADDS the reasoning budget to
  it, so no number here can bound a thinking window (invariant 000(d)); the
  `WAGIC_GPT_SEAMTOKENS=0` disable flag is untouched, and the configured/operator
  ceiling still clips from above.

RED on base: base ships 768/768/768/896/512/512 and PARSETEST pins those values, so
the lane's `gptSeamMaxTokens("ask", 6000) == 288` case FAILS on `77d82bbe6` by
construction; `lane-BO-ceiling-red.py` exits 1 on the base numbers with the measured
ratios above.

---

## L9 — the rescue markers were leaking onto the wrong decision

**Confirmed, with a different mechanism than the seat proposed.** The seat read
`reasoning_budget_hit` as "over-stamped ~4x, a char estimate". It is not a char
estimate — it was already `finish_reason == "length"`. The defect is **binding**:
both markers were SET where the forced close was *issued* and cleared only where a
record was *written*, so any decision whose phase-2 answer never became a record (a
stale/async drop — 30 in this corpus — or an abandoned retry) left them set and the
NEXT decision's record wore them.

Repro, from the data (`lane-BO-marker-red.py`, RED on base, exit 1):

```
L9  reasoning_forced_close stamped ..... 19
L9  reasoning_budget_hit   stamped ..... 19
L9  phase-2 closes actually sent ....... 4   (the only requests with max_tokens_reasoning == 0)
```

19 stamps against 4 closes — and the 4 real ones are exactly the 4 records the seat
found genuine (18.9k-23.8k reasoning chars); the other 15 are the leak.

Fix: both markers are cleared on **every** consume that is not the phase-2 poll, and
set **only** on the phase-2 consume. `reasoning_forced_close` now means a phase-2
close HAPPENED; `reasoning_budget_hit` carries phase 1's own `finish_reason`, remembered
in the new `mForceClosePhase1Length` across the two legs.

**`phase2_answer_missing` / `_recovered` wired to the path that runs.** They sat inside
the `content.empty() && reasoning non-empty` branch, which a NORMAL phase-2 recovery
(content present) never enters — which is why they read 0/0 across 40 games while 4
answer-locked retries demonstrably happened. They are now counted once per phase-2
consume, whatever shape it came back in.

Pins: `#W71-BO L9` (4 cases) — the binding rule as a pure `Bind` struct (a decision with
no phase-2 consume wears neither marker, whatever a previous decision's phase 1 did),
plus the echo that a phase-2 request is the only one with `max_tokens_reasoning == 0`,
which is what the red witness counts.

---

## L10 — a missing PLAN line, and the step pointer

`off_protocol_bytes` charges a MISLABELLED plan (class A, 92 records: the unlabelled
line's exact byte length; class B, 7: `YOUR PLAN:`) and charges an ABSENT one nothing.
Measured on the corpus with the engine's own definition of the label
(`lane-BO-marker-red.py`): **114 replies wrote no line-leading `PLAN:`, and 12 of them
carry `off_protocol_bytes == 0`** — byte-identical to a perfect two-line reply. No
field on the base record separates them.

- `plan_line_missing` is written on **every** record that carried a round trip, present
  or false (the silent-instrument discipline lane BK applied to `reasoning_chars`), and
  totalled in the gameend census over the same `protocol_replies` denominator.
- `plan_steps_done` and `plan_step_count` ride every such record too — BN's F10
  step-advance claim was unmeasurable because neither number ever reached the data.

Pins: `#W71-BO L10` (6 cases) — the four classes (compliant / A / B / G) against both
`firstLineLeadingPlanPos` and `offProtocolBytes`, with the exact byte counts of the two
corpus quotes (40 and 25), and the explicit statement that `plan_line_missing` is the
field that tells class G from a perfect reply where `off_protocol_bytes` (0 for both)
never could.

---

## Gates

| gate | result |
|---|---|
| build | clean link, both link-time guards OK (`check-ctor-init: OK (118 files)`, `check-reply-instructions: OK (1 source, 37 guides)`) |
| PARSETEST | **5227 passed, 0 failed** (base `77d82bbe6`: 5302/0 — **-75**: ~87 cases deleted with their mechanisms, 12 added) |
| suite (THREADS=1, ground truth) | **1279 tests (0 failed), 67 AI tests (0 failed)** — `~/.gatelogs/w71-BO-suite.log`; `==Test Failed !==` 0, `==Test timed out` 0 |
| encoding | `git diff \| grep -c U+FFFD` = **0** |
| RED-on-base witnesses | `lane-BO-ceiling-red.py` exit 1, `lane-BO-marker-red.py` exit 1 |

---

## Predictions for the next corpus (falsifiable)

1. **Fallback rate does not rise.** The kept re-asks are `noop_row_zero_reask`,
   `named_row_reask`, `no_pass_reask`, `plan_missing`. The deleted ones contributed
   **0 of 9** fallbacks in wave 70, so the fallback rate stays at or below 0.42%
   (9/2,119) and its kinds stay ⊆ {`noop_row_zero_reask`, `unparsed_reply`,
   `http_error`, `named_row_reask`, `no_pass_reask`, `plan_missing`}.
2. **Off-protocol rate is unchanged.** Nothing in this lane touches a prompt string
   the model reads except the two deleted correction lines, which were sent 0 times.
   `off_protocol_bytes > 0` stays near 4.9% (103/2,119) ±1.5pp; if it moves more, the
   cause is not this lane.
3. **The ceilings do not bite.** `reply_truncated` (still written from
   `finish_reason == "length"`) stays at 0 records, and no seam's PLAN+action bytes
   exceed its new ceiling × 3.15. A single record with `reply_truncated: 1` at
   ask/priority REFUTES the 288-token fit and is the signal to raise that seam.
4. **The markers become countable.** `reasoning_forced_close` count == the count of
   records whose `max_tokens_reasoning == 0`, exactly; `phase2_answer_recovered +
   phase2_answer_missing` == that same count. Any inequality means the binding is
   still wrong.
5. **The plan classes become visible.** `plan_line_missing` is true on ~5% of records
   (114/2,119 in wave 70) and its `off_protocol_bytes == 0` subset (~0.6%) is now
   nameable; `plan_steps_done` is non-zero on some record of every game that carries a
   multi-step plan (if it is 0 everywhere, BN's F10 advance never fires and that is the
   finding).

---

## Weakest evidence

1. **The ceiling re-fit is the biggest behavioural change and the one with the least
   direct proof.** It rests on ONE corpus of 2,119 replies produced under the OLD
   caps — an uncensored sample only because nothing was truncated in it. If the next
   wave changes the reply shape (a longer PLAN, a new annotation the model echoes),
   288 tokens is 2.2× today's p99.9 but only ~2.0× the largest reply ever observed,
   and the truncation re-ask that used to catch a cut reply is deleted in the same
   commit. The mitigations are that `reply_truncated` is still recorded (prediction 3
   is the tripwire), the answer-locked / forced-close rescue is untouched, and
   `WAGIC_GPT_SEAMTOKENS=0` turns the whole table off. `discard` is fitted on n=67 and
   `blockers`/`reveal` on n=20 each — the floor, not the fit, is what protects those.
2. **`commit_retracted` was deleted against a mixed signal.** R3 names it; the census
   §1 lists it among the UNTESTED zeros ("do not revert on a zero"). R3's stated reason
   is only partly true — two of its three inputs are gone, but `retracted_choice` is
   still writable. It was deleted because the field only restated the record's own
   `fallback` string, so no information is lost; if a reviewer disagrees, that is the
   one row to re-litigate.
3. **The `distinct_index_reask` deletion (R5) changes a real path**, not just a dead
   one: a discard reply that repeats an index and comes up short now goes to the
   count-enforcing fill instead of buying one re-ask. It fired 0 times in this corpus,
   but the shape it guards (125v126 seq 246) was a real 16-card discard.
4. **L9's mechanism is inferred, not observed live.** The 19-vs-4 arithmetic proves the
   markers are not bound to the close; that the carrier is specifically a dropped
   phase-2 answer is the only mechanism consistent with the code and the 30 async
   drops, but no single record was traced end-to-end to a leaking predecessor.
5. **PARSETEST coverage lost as collateral.** Removing whole CHECK statements that
   *combined* a deleted predicate with a live one also removed pins on
   `latchedCodedChoiceLine` (2), `rowSaysNoOp` (2 of 33 remaining), `planBlockEndOffset`
   (1) and `parseRepeatCount` (1). Those helpers survive; their pins are thinner.
6. **Three pre-existing dead functions were left in place** (`loopCautionForBoards`,
   `combatDirectiveNegatedOnLine`, `combatDirectiveExampleEcho`) — they were already
   unused on `77d82bbe6` (verified by a base build) and are not named in R1-R9, so
   deleting them was out of scope. Two of the three are prose scanners and are
   plausible R4-family residue for a later sweep.
