# Wave-68 lane BA — J3 / J6 / J1: the reply that runs long, the reversal nobody honours, the stop nobody reads

Base: master `4837e76a1`. All measurement below is a read-only python pass over
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/` (64 seat JSONLs, 2,436 records, binary
`14977f3af`) and was done BEFORE any code was written. Nothing here is inferred from review prose.

---

## 1. MEASUREMENT (J3 — done first, and it re-sized the fix twice)

### (a) Reply generation, per seam

`true reply bytes` = the stored `reply` plus its own `reply_trimmed_bytes` (the record trims the
tail past the PLAN line, so `len(reply)` alone under-counts by up to 5.6 kB). `bytes BEFORE label`
= from the reply's first non-blank byte to the start of its first line-leading coded label (the
same offset every seam parses from, `answerHeadingSkip` included); `bytes AFTER` is the shipped
`post_answer_overrun`.

| seam | n | gen-seconds | share | p50 s | p90 s | max s | true bytes total | p50 | p90 | BEFORE label (total/med/max) | AFTER label (total/med/p90/max) |
|---|---|---|---|---|---|---|---|---|---|---|---|
| ask | 1643 | 61,849 | 69.8% | 19.3 | 75.9 | 866.3 | 541,451 | 241 | 462 | 29,829 / 0 / 8,635 | 447,849 / 209 / 400 / 10,916 |
| priority | 496 | 18,500 | 20.9% | 22.3 | 73.3 | 683.2 | 201,038 | 280 | 607 | 10,391 / 0 / 4,093 | 163,360 / 220 / 499 / 5,221 |
| attackers | 74 | 3,007 | 3.4% | 27.8 | 80.8 | 248.3 | 30,711 | 255 | 722 | 608 / 0 / 608 | 28,894 / 236 / 701 / 3,103 |
| blockers | 33 | 3,488 | 3.9% | 34.6 | 109.9 | 1233.4 | 19,141 | 283 | 810 | 8,313 / 0 / 4,572 | 10,242 / 238 / 484 / 2,711 |
| discard | 32 | 660 | 0.7% | 16.1 | 43.2 | 87.4 | 7,773 | 275 | 299 | 0 / 0 / 0 | 7,383 / 266 / 287 / 470 |
| reveal | 20 | 747 | 0.8% | 37.9 | 61.5 | 71.5 | 4,498 | 252 | 322 | 0 / 0 / 0 | 4,160 / 231 / 295 / 390 |
| bottom | 3 | 317 | 0.4% | 94.8 | 169.5 | 169.5 | 969 | 240 | 489 | 0 / 0 / 0 | 926 / 233 / 482 / 482 |
| **total** | **2,301** | **88,567 s (24.6 seat-hours)** | | **20.5** | **75.4** | **1233.4** | **805,581** | **246** | **499** | **49,141** | **662,814** |

Two facts the table settles:

* **The pre-label side is real and was invisible.** 49,141 B sit before a label, 8,313 of them at
  the BLOCKERS seam — where the two slowest replies of the whole corpus (825 s / 4,572 B and
  462 s / 3,741 B) score **`post_answer_overrun` = 0**. The meter could not see the shape that
  costs the most per record.
* **Bytes buy the wall time.** Pearson r between a reply's true byte length and its `latency_ms`
  is **0.727** over all 2,301 replies. Cutting bytes cuts seconds; that is what makes a token cap
  a latency fix rather than a cosmetic one.

### (b) The 20 slowest replies — what is in them

| # | seam | s | true bytes | before | after | seat s# | parse_note |
|---|---|---|---|---|---|---|---|
| 1 | blockers | 1233 | 2,725 | 0 | 2,711 | deck162-0x56442565e5c0 s13 | later_answer_ignored;long_reply |
| 2 | ask | 866 | 6,957 | 0 | 6,925 | deck146-0x56285ffe51b0 s19 | later_answer_ignored;long_reply |
| 3 | blockers | 825 | 4,585 | 4,572 | 0 | deck126-0x5621f03365b0 s10 | — |
| 4 | ask | 712 | 8,965 | 8,635 | 303 | deck130-0x560d564dc440 s27 | — |
| 5 | priority | 683 | 5,053 | 0 | 4,932 | deck130-0x560d564dc440 s25 | — |
| 6 | ask | 677 | 3,716 | 0 | 3,689 | deck130-0x55ddfae77580 s17 | later_answer_ignored |
| 7 | ask | 615 | 10,561 | 0 | 10,542 | deck146-0x55b565a661c0 s36 | later_answer_ignored |
| 8 | ask | 576 | 2,586 | 0 | 2,551 | deck162-0x556383994b20 s10 | — |
| 9 | ask | 570 | 7,237 | 0 | 7,204 | deck130-0x560051065510 s42 | later_answer_ignored;latched_row_mismatch |
| 10 | ask | 552 | 4,230 | 0 | 4,169 | deck162-0x55ba6aa4f4d0 s19 | later_answer_ignored |
| 11 | ask | 530 | 10,951 | 0 | 10,916 | deck126-0x560d551bfdf0 s16 | later_answer_ignored |
| 12 | blockers | 462 | 3,761 | 3,741 | 0 | deck126-0x55f22543ef20 s13 | — |
| 13 | ask | 453 | 1,363 | 0 | 1,336 | deck152-0x55e0bc7d26c0 s2 | — |
| 14 | priority | 445 | 4,974 | 0 | 4,922 | deck123-0x55cb3de4e5f0 s42 | — |
| 15 | ask | 441 | 3,624 | 0 | 3,587 | deck146-0x55e0bb49a2d0 s36 | later_answer_ignored |
| 16 | ask | 424 | 3,009 | 0 | 2,982 | deck130-0x560d564dc440 s21 | later_answer_ignored;latched_row_mismatch |
| 17 | ask | 416 | 3,095 | 0 | 3,058 | deck130-0x55ddfae77580 s34 | later_answer_ignored |
| 18 | priority | 402 | 1,400 | 0 | 1,339 | deck146-0x557714edd100 s6 | — |
| 19 | ask | 384 | 2,718 | 0 | 2,685 | deck130-0x55912c7d4a40 s31 | later_answer_ignored;latched_row_mismatch |
| 20 | priority | 378 | 1,717 | 0 | 1,597 | deck130-0x560051065510 s13 | — |

The 20 cost **11,646 s (13.1% of all reply generation)** for **93,227 B (11.6% of all bytes)**.
Read by hand, they are two populations and nothing else: **11 of the 20 carry
`later_answer_ignored`** — the model wrote its answer, kept deliberating, and wrote a SECOND coded
line that first-wins discarded (re-derivation, not new information: #2 re-enumerates a modal pair
table it had already been given, #7/#11 re-walk a combat count, #9/#16/#19 re-open a row they had
answered) — and **2 of the 20 (#3, #12, both blockers) put the whole deliberation BEFORE the
label**, which is why the shipped meter scored them 0. None of the 20 is a long reply *because the
decision was hard*: the median REQUIRED length (first byte through the end of the PLAN line) across
the whole corpus is 246 B and p90 is 499 B.

### (c) What a seam actually needs, and what a cap costs

Bytes from the reply's first byte through the end of its PLAN line:

| seam | n | p50 | p90 | p99 | p99.5 | max |
|---|---|---|---|---|---|---|
| ask | 1643 | 240 | 460 | 1,630 | 2,664 | 8,965 |
| priority | 496 | 280 | 564 | 2,041 | 3,500 | 4,348 |
| attackers | 74 | 255 | 722 | 2,161 | 3,110 | 3,110 |
| blockers | 33 | 283 | 810 | 4,585 | 4,585 | 4,585 |
| discard | 32 | 275 | 299 | 477 | 477 | 477 |
| reveal | 20 | 231 | 321 | 397 | 397 | 397 |
| bottom | 3 | 240 | 489 | 489 | 489 | 489 |

Candidate cap sets, replayed over the same 2,301 replies at ~3.5 B per token:

| set | ask/priority/attackers/blockers/small (tokens) | cap reached | bytes not generated | answer lost -> one re-ask | PLAN clipped | est. gen-seconds saved |
|---|---|---|---|---|---|---|
| A | 1024 / 1024 / 1024 / 1536 / 512 | 21 (0.9%) | 39,677 (4.9%) | 5 (0.22%) | 7 (0.3%) | 2,755 |
| **B (SHIPPED)** | **640 / 768 / 768 / 896 / 384** | **53 (2.3%)** | **83,720 (10.4%)** | **9 (0.39%)** | **19 (0.8%)** | **~6,060** |
| C | 512 / 640 / 640 / 768 / 320 | 62 (2.7%) | 109,651 (13.6%) | 9 (0.39%) | 24 (1.0%) | 8,030 |

Set A was written first and then re-sized: it clears p99.5 on every seam but only removes 4.9% of
the bytes, which does not answer the finding. **B** clears each seam's p99 need, is ~9x the median
reply at the ask seam, and doubles the saving for four more lost answers per corpus. C was not
taken: the extra 3 points of bytes buys no extra recovered answers and clips 5 more PLANs.

---

## 2. WHAT SHIPPED

Files: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h`. All comments
tagged `#W68-BA (J1|J3|J6)`. `git diff | /usr/bin/grep -c $'\357\277\275'` = **0** after every edit.

### J3a — a per-seam `max_tokens`, and one re-ask when it bites

* `gptSeamMaxTokens(seam, ceiling)` (pure, above `buildRequestBody`) returns the seam's budget,
  **never above** the configured value / `WAGIC_GPT_MAXTOKENS`; an unnamed seam keeps the ceiling.
  `WAGIC_GPT_SEAMTOKENS=0` disables the mechanism entirely (the disable flag, so "was it the cap?"
  is one env var). Skipped when the owner has set an explicit thinking budget — that dial is his.
* `pollCompletionRetry` takes a `seam` argument (defaulted NULL) and stores it in `mRequestSeam`;
  the 10 call sites name their seam. `buildRequestBody` records the resolved cap in
  `mLastRequestMaxTokens`, and every record now carries `max_tokens` plus `reply_truncated` when
  `finish_reason == length`.
* **`reply_truncated_reask`**: a reply that stopped at the cap with **no coded label at all** earns
  ONE re-ask at the ask and priority seams, quoting the number ("ran to its length limit of N
  tokens ... put the answer FIRST"), stamped `reply_truncated_reask_recovered|_exhausted|
  _unanswered` like every other re-ask class. Never Baka, no row withheld, no window closed.
* The protocol states the budget in words at the point of composition: *"Your working comes FIRST:
  THREE short sentences at most, and never more than five. That is a budget, not a style note -
  your reply has a hard length limit, and a reply that reaches it before it has written its answer
  line is thrown away and asked again. Do not restate the board, re-derive a number the decision
  already printed for you, or list options you have already ruled out."* First-wins, the answer
  order and the #W67-AV I3 section-naming fix are untouched and PARSETEST-pinned as untouched.

### J3b — the overrun meter counts both sides

`preAnswerOverrun(reply)` (pure), fed by a new `firstLabelStart` out-param on
`scanCodedAnswerLines` so the meter measures to the same offset every seam parses from. Records
carry `pre_answer_overrun` and `reply_overrun` (the sum) on EVERY record, present or zero.
Instrument only — nothing branches on it.

### J6 — one reversal predicate at every seam, and a budget-exempt no-op re-ask

* `planSaysPassThisWindow` gains `label` + `seamVocab` parameters; **`seamVocab == 0` is
  byte-identical to every shipped caller** (all its wave-51/52/56 cases still pass unmodified).
  `proseReversesDeclaration(reply, seam)` is the named wrapper: 0 = CHOICE, 1 = ATTACK, 2 = BLOCKS.
  Each seam gets its own PLAN cues and sentence openers; every guard is shared (post-think only,
  evidence from the latched line's region, negations and deferrals rejected), and the combat arms
  add `" with "` to the deferral list so *"I will not attack **with** A2"* is an exclusion of one
  attacker, not a reversal of the combat. The PLAN-cue path — which had no deferral test at all —
  now runs one at the combat seams.
* **ATTACKERS** (`mAttackReaskTurn` / `mAttackReaskLine`) and **BLOCKERS** (`mBlockRevReaskTurn` /
  `mBlockRevReaskLine`, deliberately separate from the gang and all-illegal latches) each get ONE
  prose-reversal re-ask per turn, appended to the prompt on the tick that re-polls — the ask/
  priority pattern. Stamps: `attack_reversed_in_prose_reask` / `blocks_reversed_in_prose_reask`
  plus `_recovered|_exhausted|_unanswered`.
* **ASK** gets the CHOICE arm it never had (all 15 corpus firings were at priority because nothing
  else called the predicate). The menu's own decline row is exempt — `rowIsDeclineRow`, the
  #W54-A D2b HOLD exemption stated once for every seam, with a word-boundary test so *"Passage of
  Time"* is not a pass.
* **PRIORITY: the narrowing no longer suppresses.** `proseNamesOtherMenuRow` removed **15 of 15**
  candidate re-asks in the corpus while 12 of the 20 J1 takes past a stated stop carried exactly
  that reversal and executed. A reply that says this window is a pass HAS named a rival answer -
  row 0, which is on every one of these menus. The re-ask now fires on the reversal; the
  rival-named half survives as a stamp, renamed `plan_choice_conflict_no_rival_named` (calling it
  "narrowed" would report a suppression that no longer happens). The wave-57 PARSETEST partition
  block is re-pinned to the new arithmetic, with a NEGATIVE that the retired name is gone.
* **The no-op re-ask is exempt from the one-per-board budget** at BOTH seams
  (`mAskNoopReaskKey`, `mPriorityNoopReaskBoard`): 123 s41 spent the board's re-ask on a
  `named_row` miss and s43 then cast a Devour Flesh its own row and its own PLAN both called dead,
  at 7 life. One extra ask per board, on its own latch, so it cannot loop; when the ordinary chain
  fires the no-op branch it spends the same latch.

### J1 — the stated stop applies to ANY activation of the same ability

`rowIsRepeatBase(repeatBaseRow, choice)` (pure) answers whether the taken row is the BASE row that
an appended `[repeat: ...]` row shadows. `repeatPastStop` is no longer gated on `namedCount >= 1`:
it tests `stopTestCount` = the count named on the repeat row, or **1** for a single activation of
the same ability. The re-ask has a second wording for that case ("activates row N once more, but
your own PLAN puts you at M with your stop at S"), and the #W67-AY clamp performs
`repeatStopClampCount(stopTestCount, ...)` — on this branch `stop - M <= 0`, so the number
performed is the model's own zero and the window resolves as the pass its own arithmetic demands.
No ceiling of the engine's invention, no row withheld, an activation inside the stated stop
untouched (`repeatStopClampCount` returns -1 and the take stands).

---

## 3. GATE

Built from `<wt>/projects/mtg` with `make -f Makefile.sdl -j4`; `ls -la bin/wagic` after every make
(final binary 12:56). Gate run detached as `systemd-run --user --unit=w68-BA-gate2
-p MemoryMax=4G -p MemorySwapMax=0`, waited on a foreground done-file loop.

| gate | baseline (14977f3af) | this branch |
|---|---|---|
| PARSETEST | 5107 passed, **0 failed** | **5140 passed, 0 failed** (+33) |
| suite `WAGIC_TESTSUITE_THREADS=1` | 1274 tests, 0 failed | **1274 tests, 0 failed** |
| AI tests | 67, 0 failed | **67, 0 failed** |
| `==Test Failed !==` / `==Test timed out` | 0 / 0 | **0 / 0** |
| `git diff \| grep -c U+FFFD` | — | **0** |

### RED on base

Verified by neutering ONLY the new mechanisms (`gptSeamMaxTokens` returns its ceiling,
`preAnswerOverrun` returns 0, the protocol budget sentence reverted verbatim, `rowIsRepeatBase`
returns false, `proseReversesDeclaration` refuses every non-CHOICE seam, `rowIsDeclineRow` returns
false), rebuilding, and re-running the corpus: **9 of the 33 new cases go RED**, and every
MUST-NOT-MATCH / NEGATIVE case passes in BOTH states (they pin no new expectation):

```
FAIL #W68-BA J3 POSITIVE each seam carries its own budget, blockers the largest (it bundles)
FAIL #W68-BA J3 POSITIVE the small seams, whose largest corpus reply is 489 B
FAIL #W68-BA J3 REPRO the 126v152 s10 shape: the shipped meter reads 0, the new side reads the working
FAIL #W68-BA J3 ECHO a #W67-AV I1 heading is measured as the 8 bytes it is
FAIL #W68-BA J3 POSITIVE the length budget is stated where the model composes the reply
FAIL #W68-BA J1 POSITIVE row 1 IS the base row the repeat row counts - one activation of the same ability
FAIL #W68-BA J6 REPRO 130v126 s24: the declaration contradicts the reply's own PLAN
FAIL #W68-BA J6 POSITIVE the BLOCKS arm reads the same shape in its own words
FAIL #W68-BA J6 POSITIVE the menu's own decline rows are recognised at every seam
=== self-test: 5131 passed, 9 failed ===
```
The file was restored byte-for-byte from the pre-neuter copy and rebuilt to 5140/0 before the gate
ran. The wider RED for all three items is the corpus itself, produced by the base binary:
**20 records executed past a stated stop** (J1), **5 of 74 attackers replies declared attackers
under prose naming a pass and 15 of 15 priority reversals were narrowed away** (J6), and
**662,814 B / 88,567 s of reply generation with 49,141 B of it unmeasurable** (J3).

The corpus replies are pinned VERBATIM: 123 s27 and s48 (`stop=23`, `M=24` and `M=35`, neither
naming a count), 130v126 s24 (`ATTACK: A1, A2, A3, A4` under "I must NOT attack ... I will pass
combat"), 126v152 s10's answer-last shape, and 123 s43's dead Devour Flesh row + reply.

---

## 4. PREDICTIONS (falsifiable, for the wave-68 corpus)

1. **J3a.** `reply_truncated` appears on **fewer than 4% of records** and
   `reply_truncated_reask` on fewer than **20**, while total reply generation falls by at least
   **6%** against 88,567 s at comparable concurrency. Falsifier: if `reply_truncated_reask`
   exceeds 40, or generation time does not fall, the cap is mis-sized and set A (or the disable
   flag) is the answer — not a tighter cap.
2. **J3a, the protocol half.** Median `reply_overrun` falls below 249 B. If the caps bite (>2.3%
   of records carry `reply_truncated`) while the median does not move, the WORDS did nothing and
   wave 69 should stop spending protocol text on length as wave 68 stopped spending it on order.
3. **J3b.** `pre_answer_overrun` is non-zero on **at least 20 records**, and at least one blockers
   record carries `pre_answer_overrun > 1000` with `post_answer_overrun == 0`. A corpus-wide zero
   means the meter is wired to a label offset no reply reaches — a dead counter, not good news.
4. **J6, attackers.** `attack_reversed_in_prose_reask` fires **at least twice** (5 windows in
   2,301 records is 0.2%, so a 21-game corpus should see 2-6), and **no** record executes an
   ATTACK declaration whose own reply prose names a pass without a re-ask beside it. If it fires
   more than 15 times the combat vocabulary is too loose and the `" with "` deferral is not
   holding.
5. **J6, priority.** `plan_choice_conflict` (the firing) is now **non-zero** and
   `plan_choice_conflict_no_rival_named` no longer implies suppression: broad == firings. If
   `plan_choice_conflict_exhausted` exceeds `plan_choice_conflict_recovered` the re-ask is not
   changing minds and the narrowing removal is buying round trips for nothing.
6. **J6, the no-op exemption.** Every record whose `parse_note` carries
   `plan_contradicts_noop_row` also carries a `plan_contradicts_noop_row_reask*` stamp — i.e. the
   detector is never again right and unheard. Falsifier: one bare `plan_contradicts_noop_row` on
   an executed row.
7. **J1.** `repeat_past_stop` and `repeat_clamped_to_own_stop` are **non-zero** (they were 0 in a
   corpus with 20 violations), and **no record executes an activation of a repeat-eligible ability
   from a reply whose own PLAN states both a stop and a current count with `stop - M <= 0`** —
   counted row or plain row. If the two counters are still 0, the base-row detection is not
   reaching the rows the model presses and `rowIsRepeatBase` is looking at the wrong map.
8. **Cost.** The extra re-asks this lane adds (J6 at four seams + the no-op exemption + J1's
   broadened refusal) total **fewer than 60 extra round trips** across the corpus. If the total
   fallback+re-ask count more than doubles from wave 67's 7 fallbacks, the predicates are too
   broad and the per-seam vocabularies need the corpus counted again before they are widened.

---

## 5. WHAT I DID NOT VERIFY

* **No live model probe was run, and no game was played.** Every claim about what the model will
  WRITE under the new protocol sentence or the new caps (predictions 1, 2, 4) is inference from
  the wave-67 corpus, not measurement of the new build. The corpus-level numbers in §1 are
  measurement.
* **The tokens-per-byte constant is an assumption.** The corpus records no token counts, so the
  cap sizing rests on ~3.5 B/token for this model's English output. §1(c) shows the effect at 3.0
  and 4.0 as well (6.5% / 3.8% of bytes), so the ORDER of the effect is robust, but the exact
  "10.4% of bytes" figure moves with that constant. `max_tokens` is now on every record, so wave
  68's corpus can calibrate it directly.
* **No suite fixture is RED on base for any of the three items**, because none of them is
  reachable from the `[AI]` harness: every changed branch lives inside the GPT seam and needs a
  model reply to enter. The RED evidence is the corpus plus the neutered-mechanism PARSETEST run;
  the suite gate here is a no-regression gate, not a proof. This is the honest gap in this lane.
* **The BLOCKERS reversal arm has ZERO corpus windows.** 33 blockers records, 0 of them the shape.
  It is wired because the predicate is one predicate, and its firing rate is a wave-68
  measurement, not a claim.
* **The PUT / reveal seam is NOT wired** for the reversal: a reveal has no decline answer to
  reverse to, so there is no second answer a re-ask could ask for. The brief names PUT; this is a
  deliberate omission, stated rather than silently skipped.
* **`reply_truncated_reask` is wired at the ask and priority seams only** — the same two seams
  that carry the one-re-ask-per-board machinery (#W67-AV I2 made the same choice for the same
  reason). A truncation at discard/reveal/bottom still reaches its existing class. Those three
  seams' largest corpus reply is 489 B against a 1,344 B cap, so no truncation is expected there.
* **The forced-close budget path was not touched.** When the owner sets `reasoningBudget` the seam
  caps stand down entirely, so the interaction between a seam cap and the two-phase thinking
  rescue is untested by construction rather than by measurement.
* **I did not measure parse cost.** `preAnswerOverrun` adds one more full scan per record and the
  reversal predicate now runs at four seams instead of one; both are O(reply) with early exits,
  and neither was timed.
* **The "what the 20 slowest replies contain" reading in §1(b) is mine**, from the replies' text.
  The counts (11 of 20 carrying `later_answer_ignored`, 2 of 20 answer-last) are measured; the
  characterisation "re-derivation, not new information" is a judgement over 20 replies.
* **I did not verify any card fact** against Scryfall or the primitives. No claim here rests on
  one; the card names appear only as quoted corpus text.
