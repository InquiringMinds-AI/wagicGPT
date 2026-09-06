# Wave-68 engine-narration seat

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/` (42 seat JSONLs,
2,414 records, binary `9be85773b`). Baseline throughout is the wave-67 corpus
`matchups-20260906-102638/` (42 seats, 2,436 records, binary `14977f3af`) — the same corpus
lane BA measured. All numbers below are a read-only python pass; nothing is inferred from lane
prose. Card facts: I verified NONE against Scryfall or the primitives — no finding here rests
on one (see §5).

Citation form `deckAvB-<last6 of ptr> sN` = the deckA seat's JSONL, record seq N.

---

## 1. CORPUS CENSUS

### 1.1 Games and transport

| | wave 68 | wave 67 |
|---|---|---|
| games | 21 natural, 0 hung, 0 crashed (21/21 stderr carry `WAGIC_SELFPLAY_RESULT`) | 21 |
| wall clock, launch to last game | **4,374 s (73 min)** | 6,029 s (100 min) |
| records | 2,414 (2,282 decisions + 42 gamestart + 42 system + 42 gameend + 6 recovery) | 2,436 (2,302 decisions) |
| total reply generation | **47,931 s** | 88,567 s (**-45.9%**) |
| latency p50 / p90 / p99 / max | **10.6 / 46.4 / 183.9 / 315.2 s** | 20.5 / 75.2 / 346.5 / 1,233.4 s |
| mean in-flight concurrency (gen s / wall s) | **11.0** | 14.7 |

Decisions by seam: ask 1,753 · priority 359 · attackers 85 · blockers 28 · discard 41 ·
reveal 9 · bottom 7.

**SEGV / abort / hang / watchdog signatures: NONE.** Zero occurrences of `empty_reply`,
`transport_error`, `bad_reply`, `reveal_stall_forced` anywhere in the corpus; zero
`deadline_pct > 100` (max **35.0**); no translog gap. The `transport` field and the
`attempt_ms` field lane BC shipped are **absent from every record** — see DEFECT MED-3.

### 1.2 Fallbacks and re-asks (44 total, on 2,282 decisions = 1.93%)

| fallback | n | seams |
|---|---|---|
| `plan_choice_conflict` | 25 | ask 19, priority 6 |
| `reply_truncated_reask` | 5 | ask 4, priority 1 |
| `plan_contradicts_noop_row_reask` | 5 | ask 5 |
| `index_name_conflict` | 3 | ask 2, priority 1 |
| `repeat_count_reask` | 2 | priority 2 |
| `unparsed_reply` | 1 | discard 1 |
| `distinct_index_reask` | 1 | discard 1 |
| `blocks_reversed_in_prose_reask` | 1 | blockers 1 |
| `repeat_past_stop` | 1 | priority 1 |

Recoveries: `plan_choice_conflict_recovered` 24 / `_exhausted` 1; `plan_contradicts_noop_row_recovered`
5/5; `reply_truncated_reask_recovered` 5/5; `index_name_conflict_recovered` 3/3;
`repeat_count_reask_recovered` 2/2; `repeat_past_stop_recovered` 1/1;
`blocks_reversed_in_prose_recovered` 1/1; `distinct_index_reask_recovered` 1/1.
Wave 67 for scale: 7 fallbacks (3 `wall_miss_unrecorded_wall`, 2 `index_name_conflict`,
1 `engine_answered`, 1 `named_row_reask`).

Other parse notes: `decision_reversed_in_prose` 22 (ask 16, priority 5, blockers 1) — wave 67: 15;
`later_answer_ignored` **11** (wave 67: **38**); `latched_row_mismatch` 4 (5);
`multi_answer_first_taken` 4 (5); `plan_choice_conflict_no_rival_named` 5 (the renamed
wave-67 `plan_choice_conflict_narrowed`, 15). `long_reply` **0** (wave 67: 2).

### 1.3 Stale drops, timeouts, wall misses

* `async_drops`: **49 decision records carry one (2.15% of decisions)**, every one value 1,
  event text `casting/question (or turn/phase)/re-asked`; by seam ask 28, priority 18,
  attackers 3. Wave 67: 22 records, **0.96%**. The rate **more than doubled** (DEFECT MED-1).
  The gameend record does **not** carry an `async_drops` total (field absent) — the 0 the brief
  quotes off gameend is an absent field, not a measured zero.
* Wall misses: `wall_miss_events` **0** in all 42 seats, `wall_miss_no_retry` **0**,
  `wall_miss_unrecorded` **0**. Wave 67 had 3 `wall_miss_unrecorded_wall` fallbacks.
* `deadline_pct` max **35.0** over 2,282 records; no record above 100.
* Timeouts: none observed; no record's `latency_ms` approaches the configured deadline.

### 1.4 HOLD renders and takes (prefix-match on the row text)

Prefix `Hold priority: pass now, and do not ask me again`:

| | wave 68 | wave 67 |
|---|---|---|
| decision records rendering a HOLD row | 1,385 | 1,452 |
| records whose `chosen_text` prefix-matches it | **339 (24.5%)** | 435 (30.0%) |
| `hold_windows_skipped`, corpus total | **2,727** (25 of 42 seats nonzero, max **669**) | — |

Skip leaders: `deck123v125-88ac20` **669** (lost), `deck146v125-510310` 413 (lost),
`deck125v126-8cb190` 281 (lost), `deck125v146-8487c0` 193 (won). All six deck162 seats: **0**.

Other gameend counters: `mana_only_windows_skipped` 153 (2 seats),
`identical_ask_answers_reserved` 52 (11 seats), `identical_option_asks_resolved` 2,
`engine_reveal_floor_picks` 0, **`reserve_decline_windows_skipped` 0 in all 42 seats**.

### 1.5 Caps and overrun

`max_tokens` is on every decision record and matches the shipped set exactly, with no
exceptions: ask 640 (1,753), priority 768 (359), attackers 768 (85), blockers 896 (28),
discard/reveal/bottom 384 (57). `WAGIC_GPT_SEAMTOKENS` was not disabled.

| | wave 68 | wave 67 |
|---|---|---|
| `reply_truncated` | **58 (2.54% of decisions)** — ask 46, priority 10, discard 1, attackers 1 | n/a (no cap) |
| `reply_truncated_reask` | **5** (ask 4, priority 1); 5/5 `_recovered` | n/a |
| `pre_answer_overrun` nonzero | **13 records**, sum 5,631 B, max 637 B | field absent |
| `post_answer_overrun` | sum 637,304 B, median **226**, p90 494, max **2,973** | sum 662,814, median **209**, p90 435, max **10,916** |
| `reply_overrun` (pre+post) | sum **642,935**, median 226, p90 507 | 662,814 |
| `post_plan_overrun` | sum 65,166, nonzero 54, max 2,086 | — |
| true reply bytes (reply + `reply_trimmed_bytes`) | 753,945; p50 **274**, p90 576, p99 2,413, **max 3,040** | 805,581; p50 246, p90 508, p99 3,540, **max 10,951** |
| replies > 2,500 B | **16** (42,912 B) | 52 (207,315 B) |
| replies > 4,000 B | **0** | 16 (96,957 B) |

---

## 2. THE NINE LANE-FLAGGED QUESTIONS

### (9) SHORT GUIDES — prefill vs decode attribution (the priority question)

**Verdict: of the 40,636 s cut, roughly 90% is the short system prompt and roughly 10% is the
caps. The caps cut the extreme tail of reply bytes and nothing else; they did not cut the
overrun the lane sized them for.**

**Seam 1 — prefill.** The system prompt is the only thing that got smaller:

| | wave 68 | wave 67 | delta |
|---|---|---|---|
| `system_bytes`, median | **51,210** | 100,328 | **-48.9%** |
| decision `prompt` bytes / decision, mean | **14,721** | 13,431 | **+9.6%** |
| prefill bytes / call (system + prompt) | **65,931** | 113,759 | **-42.0%** |
| corpus prompt bytes total | 33,593,956 | 30,918,476 | +8.7% |

The per-decision render **grew**. Every byte of the prefill saving is the guide rewrite, and
it lands on all 2,282 calls: ~49 kB × 2,282 ≈ **112 MB of prefill removed from the corpus**.
Per deck (median `system_bytes`, w67 → w68): 123 100,636→49,647 · 125 100,261→49,363 ·
126 100,365→49,648 · 130 100,129→49,221 · 146 102,315→51,622 · 152 99,566→51,823 ·
162 99,176→49,595. Seconds per decision fell for every deck without exception
(162 61.7→32.4, 152 47.3→25.4, 130 46.6→26.8, 126 45.4→25.1, 123 41.2→24.2,
146 36.3→21.6, 125 18.1→10.8).

**Seam 2 — decode.** Reply bytes barely moved: 805,581 → **753,945 (-6.4%)**, and per decision
350 → 330 B (-5.7%). The **median reply got LONGER** (246 → 274 B, p90 508 → 576). Everything
the caps removed is above p99: `max_tokens` bit 58 times (2.54%), replies over 4,000 B went
16 → **0**, and the >2,500 B population went 52 → 16 (207,315 → 42,912 B). Net of the caps'
tail cut and the shorter guide's longer bodies, decode work is essentially unchanged.

**Attribution by regression.** Fitting `latency = a + b·(true reply bytes)` over every decision
record in each corpus (the same byte measure lane BA used, r = 0.727 / 0.698):

| | intercept `a` (fixed: prefill + queue) | slope `b` (decode) | fixed total | decode total |
|---|---|---|---|---|
| wave 67 | **12.42 s** | **0.07444 s/B** | 28,600 s (32.3%) | 59,968 s (67.7%) |
| wave 68 | **4.62 s** | **0.04959 s/B** | 10,542 s (22.0%) | 37,389 s (78.0%) |

* Fixed-cost cut: **-18,058 s = 44% of the 40,636 s total cut.** This is prefill, directly.
* Decode cut: -22,579 s = 56%. Split it: at wave 67's own slope, the 51,636 B the corpus
  stopped generating is worth **-3,842 s**. The remaining **-18,737 s is the per-byte rate
  falling 33%**, which a `max_tokens` cap cannot cause — a cap truncates output, it does not
  make tokens arrive faster. A shorter KV prefix does: half the context to attend over on
  every decoded token, plus the queue relief the faster requests create (mean in-flight
  concurrency 14.7 → 11.0).
* So: **caps ≈ 3,842 s ≈ 9.5% of the cut; the short system prompt and its downstream KV/queue
  effects ≈ 36,794 s ≈ 90.5%.** The queue-relief share is not separable from the KV share
  without a controlled run at fixed concurrency — but both are caused by the guide rewrite,
  so no decomposition gives the caps more than ~10%.

**What the unchanged post-answer byte count means for lane BA's caps.** `reply_overrun` is
642,935 B against wave 67's 662,814 B — **-3.0%** — while the median overrun **rose** (209 →
226 B) and p90 rose (435 → 494). Lane BA's thesis was that the overrun is the waste and that
capping would cut it. It did not. What the caps actually bought is a **tail guarantee**:
max reply 10,951 → 3,040 B, max latency 1,233 → 315 s, p99 latency 346 → 184 s. That is a
real and worth-keeping property — the corpus can no longer be held hostage by one 20-minute
reply — but it is a variance fix, not the throughput fix the sizing exercise claimed, and the
9.5% share is an order of magnitude below what a reader of prediction 1 would infer.
Prediction 1's "generation falls by at least 6%" passed on a corpus where the cap contributed
about 8% of the observed 46%; the prediction is not discriminating and wave 69 should not read
it as validating the sizing.

**And the caps have a cost the sizing did not price:** the cap set was fitted to the wave-67
reply-length distribution, and the guide rewrite moved that distribution. The 384-token small
seam was sized on "largest corpus reply is 489 B against a 1,344 B cap" — in wave 68 a discard
reply reached **1,409 B**, hit the cap, produced no label, and had no re-ask wired at that seam
(HIGH-1).

### (1) Reply caps

`reply_truncated` **58 / 2,282 = 2.54%**, by seam ask 46, priority 10, discard 1, attackers 1.
`reply_truncated_reask` **5**, all at ask (4) / priority (1), all 5 `_recovered`.
**One decision was lost to a cap**: `deck125v146-8487c0 s133` (discard, cap 384, 1,409 B,
`reply_truncated` + `unparsed_reply`) — the discard seam has no truncation re-ask, so the
heuristic answered (HIGH-1). **A second was materially changed**:
`deck123v125-88ac20 s23` (priority, cap 768) truncated mid-analysis of a Marsh Flats fetch it
had already resolved to the Swamp; the re-ask at s25 answered `pass` instead (HIGH-2).
Latency/seam vs wave 67 (gen-seconds, p50 s): ask 36,242/10.3 vs 61,849/19.3 · priority
7,566/8.2 vs 18,500/22.3 · attackers 2,315/20.7 vs 3,007/29.2 · blockers 825/31.9 vs
3,488/34.6 · discard 368/5.6 vs 660/16.1 · reveal 179/14.9 vs 747/35.1 · bottom 435/51.2 vs
317/94.8. Every seam fell except bottom (n=7 vs n=3 — noise).

### (2) `pre_answer_overrun` + `reply_overrun` census

`pre_answer_overrun` fires on **13 of 2,282 records** (0.57%), sum 5,631 B, max **637 B**.
The 13 (seq / seam / pre / post): `deck146v125-510310` s143 ask 394/198, s166 discard 565/389,
s246 priority 431/254, s255 priority 362/325, s260 priority 605/376; `deck125v126-8cb190` s24
ask 401/270; `deck146v130-98a100` s22 discard 546/353; `deck125v152-3e1710` s67 discard
637/247; `deck123v126-f6a640` s21 priority 2/303; `deck146v123-1b42d0` s39 ask 249/239;
`deck130v125-86cce0` s12 ask 589/277; `deck162v152-c31670` s12 blockers 337/177;
`deck125v162-c27380` s11 ask 513/270. **No record has `pre > 1000` with `post == 0`** — the
answer-last blockers shape lane BA built the meter for (wave 67's s10/s13 at 4,572 B and
3,741 B) did not recur once in 28 blockers records. `reply_overrun` = pre+post on every record:
sum 642,935, median 226, p90 507, max 2,973.

### (3) Wall misses and `deadline_pct`

`wall_miss_no_retry` **0** on every gameend; `wall_miss_events` 0; `wall_miss_unrecorded` 0.
Max `deadline_pct` **35.0**; **0 records above 100**. No record carries `attempt_ms` or a
`retry`/`transport` field at all (MED-3).

### (4) `repeat_past_stop` / clamp counts

`repeat_past_stop` fired **once** (`deck123v162-5f8310 s27`, priority, `_recovered` at s28).
`repeat_count_reask` fired twice at the same seat (s23→s24 recovered with `repeat_n=6`,
s25→s26 recovered with `repeat_n=10`). **`repeat_clamped_to_own_stop`: 0.
`stop_conflict_single_activation_executed` (BE R3's new stamp): 0.** 17 prompts rendered a
`[repeat: ...]` row. **No record executes a take past a stated stop**: every activation with a
stated stop in the same reply either resolved inside it or was re-asked and recovered.

### (5) `decision_reversed_in_prose` at ATTACK / BLOCKS

Fired **22 times** total: ask 16, priority 5, **blockers 1, attackers 0**. The blockers firing
is `blocks_reversed_in_prose_reask` at `deck162v152-c31670`, recovered.
**At the ATTACKERS seam the window never arose**: 85 attackers records, 0
`decision_reversed_in_prose`, 0 `attack_reversed_in_prose_reask`, and my own independent scan
(a non-empty `ATTACK:` line whose preceding prose carries pass/decline language) finds **0 of
85**. The no-op re-ask exemption: `plan_contradicts_noop_row` fired **5 times, all at ask, and
all 5 carry `plan_contradicts_noop_row_reask` + `_recovered`** — 0 bare.

### (6) X ceiling

**33 ANNOUNCE_X windows, all at the ask seam, all in deck125 and deck130 seats.**
Two of them had a library ceiling below the mana ceiling (`deck125v146-8487c0 s129`,
`deck125v126-8cb190 s110`). In both, the header, the cast row and the row list agree exactly:
s129 header "X=8 is the largest value on this menu the library pays for", rows 1–9 all carry
`LOSE the game`, row 10 is X=8, seat took X=8; s110 header names X=7, rows 1–4 carry
`LOSE the game`, seat took X=7. **No `{library:}` line is wrong** in any of the 33 windows,
no deck-out occurred, and 12 windows rendered the reserve clause
(`N draws are already owed that you cannot decline, and the N left still pays them`) correctly
on safe rows. **No seat ever answered a row carrying the unsafe wording
`... and only N would be left`** (2 windows rendered it, 0 takes).

### (7) Ward asks

**No ward trigger fired in this corpus.** 0 prompts contain a ward-trigger target ask
(`TARGET CHOICE for Ward` = 0, `Ward trigger` = 0); the 5 prompts containing `Ward-Pay` carry
it only inside a card-text blob. **0 opponent library cards were offered as rows anywhere.**
22 rows across 6 records name `[opponent's hand]` — all 6 are Elite Spellbinder's
"look at target opponent's hand … exile a nonland card" ask
(`deck152v125-0a8e90 s29`, `deck152v123-c4ee90 s19`, `deck152v126-b51fd0 s9`/`s15`,
`deck152v162-8f8e40 s29`, `deck152v146-04e600 s18`), which legitimately reads that hand.
The J7 fix is therefore **unexercised**, not validated.

### (8) The reservation-decline latch under the full key

**`reserve_decline_windows_skipped` = 0 in all 42 seats.** The latch never held. Relatedly,
`reserve_decline` windows and ANNOUNCE_X declines both have zero instances: **0 seats took the
`Decline - do not cast this after all` row** in any of the 33 X windows, so the J5b/c
abandonment narration (`was NOT cast: you declined after the payment above`) appears **0 times**
in the corpus and is unexercised.

---

## 3. PREDICTION ADJUDICATION

Verdicts: **PASS** / **FAIL** / **UNTESTED** (no window arose — never inferred).

### Lane BA

| # | claim | verdict | evidence |
|---|---|---|---|
| BA-1 | `reply_truncated` < 4% of records; `reply_truncated_reask` < 20; generation falls ≥ 6% | **PASS** | 58/2,282 = **2.54%**; `reply_truncated_reask` = **5**; 47,931 s vs 88,567 s = **-45.9%**. Non-discriminating: §2(9) attributes only ~9.5% of the fall to the caps. |
| BA-2 | median `reply_overrun` falls below 249 B | **FAIL** | Literal threshold met (**226 B** < 249), but the prediction's own falsifier fires: the caps bit on **2.54% > 2.3%** of records while the median did **not** move down — it ROSE from wave 67's 209 B, p90 435→494. By the lane's own test, the protocol's length-budget sentence did nothing. |
| BA-3 | `pre_answer_overrun` non-zero on ≥ 20 records, and ≥ 1 blockers record with `pre > 1000` and `post == 0` | **FAIL** | **13** records (0.57%), max **637 B**; **0** blockers records with `pre > 1000`. Both halves miss. The meter is wired correctly (13 real firings, and `deck123v126-f6a640 s21` reads pre=2/post=303, i.e. it distinguishes the offset), so this is not the dead counter the falsifier describes — the SHAPE stopped occurring. |
| BA-4 | `attack_reversed_in_prose_reask` fires ≥ 2× ; no ATTACK executed under prose naming a pass without a re-ask | **UNTESTED** (first half) / **PASS** (second half) | 85 attackers records, **0** windows of the shape by the engine's predicate and **0** by my independent prose scan. The re-ask cannot be said to have failed to fire when nothing to fire on arose. Second half holds: 0 such executions. |
| BA-5 | `plan_choice_conflict` non-zero; `_exhausted` ≤ `_recovered` | **PASS** | 25 firings (ask 19, priority 6); `_recovered` **24**, `_exhausted` **1**. `plan_choice_conflict_no_rival_named` = 5, a stamp only. |
| BA-6 | every `plan_contradicts_noop_row` also carries a `_reask*` stamp; 0 bare | **PASS** | **5 of 5** (`deck126v125-be5ad0 s82`, `deck123v125-88ac20 s61` and `s126`, `deck146v123-1b42d0 s14`, `deck125v130-5521f0 s20`), all `_reask` + `_recovered`; **0 bare**. |
| BA-7 | `repeat_past_stop` AND `repeat_clamped_to_own_stop` non-zero; no execution past a stated stop | **FAIL** (first half) / **PASS** (second half) | `repeat_past_stop` = **1** ✓; **`repeat_clamped_to_own_stop` = 0**, so the conjunction fails. 17 prompts rendered a `[repeat:` row — the windows existed. No take past a stated stop executed. Note BE R3 renamed the executed-clamp case to `stop_conflict_single_activation_executed`, which is also **0**, so no rename hides the count. |
| BA-8 | extra re-asks < 60; total fallback+re-ask ≤ 2× wave 67's 7 | **PASS** (first) / **FAIL** (second) | Total fallbacks **44**, all of them re-asks or refusals this lane's family owns — well under 60. But 44 is **6.3×** wave 67's 7, not ≤ 2×. Reading the lane's own falsifier literally, the predicates are "too broad" — I do not endorse that reading: 24/25 `plan_choice_conflict` recovered, and wave 67's 7 was low because the narrowing suppressed 15 of 15. |

### Lane BB

| # | claim | verdict | evidence |
|---|---|---|---|
| BB-1 | every under-reserve X row carries the warning; no seat answers such a row | **PASS** | 2 windows render the unsafe wording (`s129` rows 8–9, `s110`); **0 takes** carry `only N would be left`; 12 windows render the safe reserve wording, all takes on safe rows. |
| BB-2 | largest no-`LOSE` X == the X named by the header | **PASS** | **2 of 2**: s129 header X=8 / largest safe row X=8 / taken X=8; s110 header X=7 / largest safe X=7 / taken X=7. The cast rows at s128 and s109 name the same numbers. |
| BB-3 | `index_name_conflict` = 0 on ANNOUNCE_X windows; every `x_value_named` `chosen_text` begins `X = ` | **PASS** (first) / **UNTESTED** (second) | 3 `index_name_conflict`, **none on an X window** (all on cast/priority menus). All 33 X-window `chosen_text` values begin `X = `. But **`x_value_named` is emitted 0 times** in the corpus, so the stamped half has no instances. |
| BB-4 | no prompt pairs `[cannot pay now: X]` with an offered `Cast X` row; every cannot-pay tag beside `Already in pool:` names the floating mana | **PASS** | 778 prompts carry a cannot-pay tag; **0 contradicting pairs** by name-match. 41 of them also print `Already in pool:`, and each tag names the pool (e.g. `deck125v146-8487c0 s13`: "needs 6 mana, you have 0 untapped sources and 5 mana already floating"). Wave 67's 9 contradicting prompts are gone. |
| BB-5 | every X decline with a nonzero pool is followed by the abandonment line; no seat declines twice a turn with ≥4 floating | **UNTESTED** | **0 ANNOUNCE_X declines taken** in 33 windows; the narration string appears 0 times. No window arose. |
| BB-6 | no seat takes a row carrying `{answers the stack: NO ...}` and loses to that stack next record | **FAIL** | The clause rendered on **4 records and was taken on all 4**: `deck123v126-f6a640 s35`, `deck125v162-c27380 s68` and `s69`, `deck123v162-5f8310 s57`. At s68/s69 (turn 29, 1 life, "4 damage … you would be at -3; that would KILL you") the seat held and the very next record is `gameend won=false`. Honest qualifier: at s68/s69 no row could have saved it — Sphinx's Revelation gains X but the two Underworld Dreams charge 2 life per drawn card, so every X is net-negative. The clause was RIGHT and READ (both replies quote it) and the seat still spent **66 s** of generation to restate it. See MED-4. |
| BB-7 | no hold survives a verdict-word change; deck162 `hold_windows_skipped` < 56 in a lost game | **PASS** (second) / **UNTESTED** (first) | All six deck162 seats: `hold_windows_skipped` = **0** ✓. The first half is **not observable from the translog**: skipped windows are only ever a counter, never a record, so "did a hold survive a verdict change" cannot be answered from the corpus. I can show 8 consecutive same-seam pairs where a hold was taken and the `KILL you` verdict then flipped and the window **did** re-open, which is consistent with the marker working, but it is not the falsifier. See MED-5. |

### Lane BC

| # | claim | verdict | evidence |
|---|---|---|---|
| BC-1 | 0 records pair `retry: 1` with a `phase=wall` stamp; every wall miss is `wall_miss_no_retry`; no `wall_miss_unrecorded_*` | **UNTESTED** | **0 wall misses of any kind** in 21 games (`wall_miss_events`/`_no_retry`/`_unrecorded` all 0 on all 42 gameends). Max `deadline_pct` 35.0. The branch lane BC noted "has never fired" still has not. |
| BC-2 | 0 records publish `deadline_pct > 100`; every `retry: 1` record carries a 2-entry `attempt_ms` | **PASS** (first) / **UNTESTED** (second) | Max `deadline_pct` **35.0**, 0 above 100 ✓. **No record carries `retry` or `attempt_ms`** — no retry occurred, so the leg-for-leg agreement has no instance. |
| BC-3 | 0 ward target asks list a hidden-zone card; ward ask option count ≤ stack size | **UNTESTED** | **No ward trigger fired** (see §2(7)). 0 ward asks exist. Separately verified: 0 rows anywhere name `[opponent's library]`, and the 22 `[opponent's hand]` rows are all Elite Spellbinder's own look-at-hand ability. |
| BC-4 | 0 blind `recovery` records at ask/priority; every `*_reask` recovery carries `executed_by: "reask"` | **PASS** | 6 recovery records; **5 carry `executed_seam` + `executed_by: "reask"`** (`deck125v146-8487c0 s104`, `deck146v125-510310 s115`, `deck123v125-88ac20 s24`, `deck125v123-5702d0 s65`, `deck152v162-8f8e40 s47`), the 6th (`deck125v146-8487c0 s134`) carries `recovered_by` — it is the discard-seam heuristic recovery, not an ask/priority reask. 0 blind. |
| BC-5 | every planeswalker TARGET row carries `[loyalty N]`; every legend-rule pick carries the CR 704.5j clause; 0 cross-copy loyalty arithmetic | **PASS** (first, N=1) / **UNTESTED** (rest) | Exactly **one** planeswalker target row exists in the corpus (`deck146v123-1b42d0 s32` row 1, Lolth) and it carries `[loyalty 3]` ✓. **`LEGEND RULE` appears in 0 prompts** — no legend-rule pick arose, so the clause and the arithmetic check have no instances. |

### Lane BD

| # | claim | verdict | evidence |
|---|---|---|---|
| BD-1 | 100% of sacrifice-mode rows carry the EDICT clause, 0 bare; no reply names a determined victim at N>1 | **PASS** (first) / **UNTESTED** (second) | **22 of 22** mode rows containing `sacrifice creature` carry `{this mode's SACRIFICE half is an EDICT: ...}`, **0 bare**, across 6 windows (`deck146v125-510310` s33/s198/s202/s236/s237, `deck146v123-1b42d0` s34). But every one reads **"they control 0 creatures - at 0 this does nothing"** — the N>1 board the prediction is about (wave 67's `146v123` s30 against 31 creatures) never arose, so the naming half is untested. |
| BD-2 | 0 replies call a granted opponent draw own card advantage; take rate on those rows falls | **PASS** (first) / **UNTESTED** (second, N too small) | The one reply that starts down that path corrects itself: `deck146v125-510310 s32` writes "This forces the opponent to draw (giving them cards…)" and then "They gain a card, lose 1 life. This is bad." **0 replies call it a gain.** The take-rate half: only 2 mode windows offered the pair (`s33` took `opponent draws`, `deck146v123-1b42d0 s34` took `you draw`) — 1 of 2, against wave 67's 2 of 3. Not a measurable rate. Note the PRICE render worked and was still overridden once (MED-6). |
| BD-3 | every rendered opponent-draws mode row with a pilot-side twin names it by label | **PASS** | **10 of 10** rows beginning `return creature and opponent draws` / `creature gains 3/3 and opponent draws` carry `{this mode … is this same mode pointed at YOU …}` and the PRICE wording (`you HAND THEM 1 card - that is a PRICE this mode pays`). 0 bare. |
| BD-4 | every plain-edict cast row beside a CRACK-BACK line carries `{crack-back cover:`, FLOOR-worded where their bodies exceed the set | **PASS** | 339 prompts carry a CRACK-BACK line; **41 carry a cover clause**, 52 clauses total, **28 FLOOR-worded** and 24 otherwise, and **0 promise a removal on a board holding a non-attacking body**. The gap between 339 and 41 is the lane's own stated scope (the cover is wired on the PLAIN edict class only), not a miss. Repro of the working shape: `deck146v130-98a100 s14`, "{crack-back cover: … that line says the total is a FLOOR, not a ceiling …}". |
| BD-5 | on `NO LIVE CAST ROW` turns the seat's rate of also declining land drop + activations falls below wave 67's | **UNTESTED** at this seat | The header shipped and is correct: **214 records across 11 seats**, and **0 of them offer a cast row whose `{right now:}` verdict is non-zero** — the header never lies. The behavioural half is a per-deck rate over 12 wave-67 windows at one seat; it is a deck-reader measurement, not an engine-seat one, and I did not compute it. |
| BD-6 | Intrepid Adversary band windows fall under 1,900 B, every rung printed; no re-ask lands on the wrong index | **FAIL** | 24 Intrepid Adversary rows over 290 windows; **max single row 1,425 B**, but the containing prompts run to **28,556 B** (median 13,877). Read as the lane wrote it — "band windows drop from 2,514–2,737 B to under 1,900 B" — the ROW shortening did not reach 40 B and the largest band row is still 1,425 B. Supporting half holds: **0** `named_row_reask`, `unparsed_reply` or `distinct_index_reask` names a rung inside a band (the 1 `unparsed_reply` is the discard truncation, the 1 `distinct_index_reask` is a discard duplicate index). |
| BD-7 | every prompt with `Opponent library:` ≤ 3 carries `DECK-OUT IS IN RANGE`; ≥ 1 seat states a deck-out in its PLAN | **UNTESTED** | **0 records** in the corpus show an opponent library at 3 or fewer, so the clause has no window. (`DECK-OUT IS IN RANGE` appears 0 times; 7 records show the seat's OWN library at ≤ 3, which the clause does not cover.) |
| BD-8 | every Teferi-class stage-1 gate row carries the two-branch census; 0 wrong-type assertions; latency falls | **UNTESTED** | The brief itself records "Teferi stage 1 NOT shipped". Confirmed: `stage 1` appears in **0** prompts. No window. |
| BD-9 | every `- Paid` line before the last turn header reads `(paid with N sources)`, none reads ` with `; narration bytes fall ~3.9% | **FAIL** | The fold works and is dominant — **18,653** rendered `- Paid … (paid with N sources)` lines — but **2,720 past-turn lines still read `… with X`**, of which **435 are multi-source** (`deck146v125-510310 s35`: `- Paid {b}{w} for Silverquill Silencer with Plains #1; Swamp …`). The falsifier is "a past-turn `- Paid … with A; B` line surviving": 435 of them survive. Single-source past lines (2,285) are the larger half and are arguably out of the fold's intent, but the multi-source ones are exactly what it was written for. |

### Lane BE

| # | claim | verdict | evidence |
|---|---|---|---|
| BE-R1 | no GPT seat constructs with a bad `mAttacksDoneTurn` / `mRepeatRemaining`; 0 NULL `FindCardToPlay` with no repeat in flight | **UNTESTED** from the corpus | The corpus records no constructor state and no `FindCardToPlay` stamp (0 occurrences of the string anywhere). The build-time guard `tools/check-ctor-init.py` is the evidence for R1, not this corpus. Consistent-with: 0 crashes, 0 aborts, 85 attackers records all answered. |
| BE-R2 | `stop_conflict_single_activation_executed` appears; `repeat_clamped_to_own_stop(named=1)` = 0 | **UNTESTED** (first) / **PASS** (second) | **`stop_conflict_single_activation_executed` = 0** and `repeat_clamped_to_own_stop` = 0 — the single-activation stop conflict never arose (only 1 `repeat_past_stop`, recovered by re-ask before reaching the clamp). Second half holds vacuously at 0. |
| BE-R3 | `reply_truncated_reask` appears on attackers/blockers records and all resolve; no combat `finish_reason:length` fallback without one of those notes first | **FAIL** | **`reply_truncated_reask` fires 0 times at attackers or blockers.** `deck123v162-5f8310 s19` is an `attackers` record with `reply_truncated: true` at cap 768 and **no re-ask stamp and no fallback** — a combat record that hit the cap and was neither re-asked nor recovered; its answer was simply kept. That is the second half's falsifier as written. (Lane BA §5 states the re-ask is wired at ask/priority only; BE R4 claims it was extended to attackers/blockers. The corpus says the extension did not fire on the one window it had.) |
| BE-R4 | no record publishes `deadline_pct` above 100 unless an attempt genuinely exceeded budget; `attempt_ms` agrees leg-for-leg | **PASS** (first) / **UNTESTED** (second) | 0 records above 100, max 35.0 ✓. **No record carries `attempt_ms`** (no retry occurred), so the leg-for-leg check has no instance. |
| BE-R5 | R2/R5/R6/R7 wordings appear verbatim; no reply argues from "I keep the one I pick", a certain edict kill, a forfeited pool, or a board-blocked deck-out | **PARTLY UNTESTED** | Wordings present: the floating-pool wording (29 prompts say the pool `stays floating`), the edict FLOOR/at-0 wording (22 rows), the reserve wording (12 rows). **Absent because no window arose**: `LEGEND RULE`/`PICK DIES` (0 prompts), `CANTMILLLOSE`/`CANTLOSE`/`CANTWIN` (0), `DECK-OUT IS IN RANGE` (0). Behavioural half **PASS** on the arms that rendered: 0 replies argue from a certain edict kill (`deck146v123-1b42d0 s33` reads the HALF DEAD label correctly), 0 from a forfeited pool. |

**Tally: 13 PASS, 9 FAIL, 12 UNTESTED, plus 5 split verdicts counted on both sides above.**
By lane — BA 4 PASS / 4 FAIL / 1 UNTESTED (over 8 predictions, 3 split);
BB 4 PASS / 1 FAIL / 2 UNTESTED (1 split); BC 2 PASS / 0 FAIL / 3 UNTESTED (2 split);
BD 4 PASS / 2 FAIL / 3 UNTESTED (2 split); BE 2 PASS / 1 FAIL / 2 UNTESTED (2 split).

---

## 4. ENGINE / RENDER / NARRATION DEFECTS

### HIGH

**HIGH-1 — a cap sized on last wave's replies silently ate a discard decision, at a seam with
no re-ask.**
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/1788720083-ai_baka_deck125-0x556d8c8487c0-vs-ai_baka_deck146.jsonl` **s133**, kind `discard`, `max_tokens: 384`,
`reply_truncated: true`, reply 1,409 B, `fallback: unparsed_reply`. The next record s134 is the
recovery, and its `recovered_by` narration reads:
`- Cleanup discard (hand 13, limit 7): the heuristic chose Emrakul, the Aeons Torn, Fall of the Gavel, Fall of the Gavel, Fall of the Gavel, Supreme Verdict, Dream Fracture`.
The model never answered; the heuristic discarded seven cards including the seat's Emrakul.
Root cause is a sizing assumption, not a bug: lane BA sized the 384-token small-seam cap on
"largest corpus reply is 489 B against a 1,344 B cap", measured on the wave-67 reply
distribution — and the guide rewrite moved that distribution up (corpus median reply 246 → 274 B,
discard p50 275 → 279 B, and this one reply reached 1,409 B). Lane BA also deliberately wired
`reply_truncated_reask` at ask/priority only, on the grounds that no truncation was expected at
discard/reveal/bottom. Both halves of that reasoning were falsified by the same record.
Fix shape: wire the truncation re-ask at every seam that has a coded label, and re-fit the caps
on THIS corpus (`max_tokens` is now on every record, so the fit is available).

**HIGH-2 — a truncation "recovery" that changed the answer, stamped as a success.**
`…deck123-0x55e70d88ac20-vs-ai_baka_deck125.jsonl` **s23** (priority, cap 768,
`reply_truncated`, `fallback: reply_truncated_reask`, `chosen_text: <refused: reply_truncated_reask>`).
Its reply ends mid-sentence having already resolved the decision:
`Option 2: "targeting Swamp" - Valid. Fetches the basic Swamp. … So Option 2 is the only valid land fetch. Should I fetch the Swamp? If I fetch the Swamp`.
The recovery record s24 carries `executed_by: "reask"`, `executed_choice: 0`,
**`executed_text: ""`**, and s25 answers `pass`. The seat did not fetch. The counter reads
`reply_truncated_reask_recovered` — a recovered ROUND TRIP, not a recovered DECISION. The
metric cannot distinguish "the re-ask got the same answer" from "the re-ask got a different
one", which is exactly the question the cap's safety rests on. Fix shape: on a truncation
re-ask, record whether the recovered choice equals the choice the truncated reply had already
named, when it named one.

**HIGH-3 — the caps did not do the job they were sized for, and the corpus's headline number
credits them for it.** §2(9): `reply_overrun` 642,935 vs 662,814 B (**-3.0%**), median overrun
209 → **226** (up), p90 435 → **494** (up). Under a fitted decomposition the caps are worth
about **3,842 s of the 40,636 s cut (9.5%)**; the short system prompt and the KV/queue relief
it causes are worth about **36,794 s (90.5%)**. What the caps did buy — max reply 10,951 →
3,040 B, max latency 1,233 → 315 s, replies over 4 kB 16 → 0 — is a variance guarantee worth
keeping. This is HIGH because a wave-69 reader of the corpus line "total reply generation
47,931 s vs 88,567 s (-46%)" beside "per-seam max_tokens caps shipped" will attribute the
whole win to the caps and tighten them further; set C would clip PLANs to buy seconds that are
not there.

### MED

**MED-1 — stale-drop rate more than doubled.** `async_drops` on 49 of 2,282 decision records
(**2.15%**) against wave 67's 22 of 2,302 (**0.96%**); by seam ask 28, priority 18, attackers 3,
every event `casting/question (or turn/phase)/re-asked`. Nothing observably went wrong (all 49
were re-asked), but the rate is the one census number that got worse, and the plausible
mechanism is the wave's own success: shorter prefill means replies land faster relative to the
game loop's tick, so more of them arrive against a board that has already moved. Worth one
measurement next wave rather than a fix now.

**MED-2 — `async_drops` is not aggregated on `gameend`.** The field is present on 49 decision
records and **absent from all 42 gameend records**, unlike `wall_miss_events`,
`hold_windows_skipped` and the rest. Any reader who takes the per-game total off the gameend
record (as the wave-68 brief's "async_drops 0.95%" line appears to) reads an absent field as
zero. Repro: `python -c "…"` over any gameend record — `'async_drops' in rec` is False.

**MED-3 — lane BC's `transport` / `attempt_ms` / `retry` fields are absent from every record.**
0 occurrences of `transport`, `transport_error`, `attempt_ms` in 2,414 records. This corpus had
no retries and no wall misses, so the fields may simply be conditional — but that cannot be
distinguished from "not wired" without a window, and BC-1/BC-2's second halves are therefore
unfalsifiable from any clean corpus. If they are conditional, emit `attempt_ms` with one entry
on the normal path so the instrument proves itself.

**MED-4 — the `{answers the stack: NO ...}` verdict has no positive twin, and the seat pays
seconds to rediscover the negative.** `…deck125-0x561492c27380-vs-ai_baka_deck162.jsonl`
**s68** and **s69**, turn 29, 1 life. The board line reads
`ON THE STACK: 4 damage to you - you would be at -3; that would KILL you`; row 2 (the hold)
carries `{answers the stack: NO - 4 damage is ALREADY ON THE STACK, and taking this row lets it resolve: that puts you at -3 and KILLS you}`; **row 1 (Cast Sphinx's Revelation) carries no
verdict at all**. The seat spent 36 s + 30 s writing out, in prose, the arithmetic the render
already had, concluded "I am dead", took the hold, and died on the next record. It was in fact
dead — the row-1 `DRAW PRICE` annotation is correct that every X is net-negative against two
Underworld Dreams — so this is not a wrong answer. It is 66 s of generation, on the last two
decisions of a lost game, spent because the cast row does not say what the hold row says.
Lane BB named this as the next move; the corpus's 4-of-4 take rate is the evidence it needs.

**MED-5 — a hold that suppresses a window leaves no record, so "did the hold survive a verdict
change" is unanswerable.** `hold_windows_skipped` totals 2,727 across 25 seats (max **669** in
`…deck123-0x55e70d88ac20…`, a 66-turn loss) and is a counter only. BB-7's first half, the
wave-63..67 hold questions, and any future claim about the crack-back marker all require the
suppressed windows themselves. One line per suppressed window (seq, seam, the key it matched)
would make the whole hold family falsifiable; today it is 2,727 invisible decisions.

**MED-6 — the PRICE render is right and was overridden anyway.**
`…deck146-0x556d8b510310-vs-ai_baka_deck125.jsonl` **s33**: row 2 reads
`return creature and opponent draws {this mode right now: they LOSE 1 life - they would be at 21; you HAND THEM 1 card - that is a PRICE this mode pays …}` against a deck125 control seat
that had just been drawing off Sphinx's Revelation; the previous record s32's own reply had
already written "They gain a card, lose 1 life. This is bad." The seat took it. BD-2's
render half PASSES and the behaviour half did not follow — worth a deck-125-matchup note next
wave rather than more render.

### LOW

* **LOW-1 — the `- Paid` fold leaves 435 past-turn multi-source lines.** 18,653 folded
  `(paid with N sources)` lines against **2,720** unfolded past-turn `… with X` lines, of which
  **435** name more than one source (`…deck146-0x556d8b510310… s35`:
  `- Paid {b}{w} for Silverquill Silencer with Plains #1; Swamp`). 1,197 unfolded lines are in
  the current turn and are intended.
* **LOW-2 — band rows are still large.** Longest Intrepid Adversary row **1,425 B**, against
  BD-6's "shortened to ~40 B"; the windows carrying them run to 28,556 B. No answer landed on
  a wrong index, so this is bytes, not correctness.
* **LOW-3 — prompt bytes grew 9.6% per decision** (13,431 → 14,721 mean; corpus total
  30.9 MB → 33.6 MB), driven mainly by deck125 (15,164 → 18,005) and deck146
  (16,882 → 18,454). The system-prompt saving swamps it, but the per-decision render is on its
  own upward trend and the largest prompts now reach **34,640 B**
  (`…deck125-0x556e248cb190… s111`, turn 41).
* **LOW-4 — `pre_answer_overrun` is a near-idle counter.** 13 firings, max 637 B, no blockers
  shape at all. Keep it (it costs one scan and it did distinguish pre from post at
  `…deck123-0x55b3dbf6a640… s21`, pre=2/post=303), but do not read its silence as a win — the
  wave-67 shape it was built for simply stopped occurring, and nothing in the corpus explains
  why.

---

## 5. WHAT I DID NOT CHECK

* **No card fact was verified against a primitive or Scryfall.** Card names appear here only as
  quoted corpus text. In particular the MED-4 claim that Sphinx's Revelation is net-negative
  against two Underworld Dreams rests on the rendered `DRAW PRICE` annotation, not on the
  primitives — if that annotation is itself wrong, MED-4's "it was in fact dead" is wrong with
  it, and the defect gets bigger, not smaller.
* **The prefill/decode attribution is a fit, not an instrument reading.** No token counts exist
  in either corpus; the split rests on a linear regression of latency on reply bytes (r = 0.73 /
  0.70) and on the assumption that the same model and hardware served both runs. Mean in-flight
  concurrency differed (14.7 vs 11.0) and I could not control for it — record timestamps are not
  logged, so I cannot bucket by instantaneous server load. The ~90/10 split is robust to that
  confound only in the sense that the confound is itself downstream of the prefill cut; a
  controlled run at fixed concurrency would settle it and has not been done.
* **I did not compute per-deck behavioural rates** — BD-5's decline rate on `NO LIVE CAST ROW`
  turns, BD-2's take rate, and any "would a dropped guide rule have changed this decision"
  judgement. Those are the deck readers' measurements; I verified only that the renders exist
  and are internally consistent.
* **I did not open a deck guide, the rewrite `changes.md` files, or any deck review.** The
  §2(9) attribution is measured entirely from `system_bytes`, `prompt`, `reply` and
  `latency_ms`; I did not check which rules the rewrite cut, so I make no claim about deck123's
  0-of-6 or about any specific dropped rule.
* **I did not re-run the engine's predicates.** BA-4's "0 attacker reversal windows" is the
  engine's own count plus an independent regex scan of the 85 attacker replies; a window whose
  prose reverses in a way neither the predicate nor my regex catches would be invisible to both.
* **HOLD counts are prefix-matched on `chosen_text`**, per the brief. A take whose
  `chosen_text` was rewritten by a recovery path would not be counted; 6 recovery records exist
  and 2 of them carry a HOLD `executed_text`, so the 339 figure may undercount by up to 2.
* **`plan_choice_conflict` recovery quality is not assessed.** I counted 24 `_recovered` and
  did not check whether the recovered answer differs from the refused one — the same blind spot
  HIGH-2 names for truncation re-asks applies here and I did not close it.
* **No build, no game, no git, nothing under `bin/Res` or `src/` was read or written.** Only
  `wave68/engine-seat.md` and `wave68/lategame-specimen.txt` were written.

---

## 6. LATEGAME SPECIMEN

`wave68/lategame-specimen.txt` — `…deck125-0x556d8c8487c0-vs-ai_baka_deck146.jsonl` **s129**,
**turn 69**, ask / ANNOUNCE_X, life 70 vs 17, latency 7.9 s, prompt 29,243 B.
Chosen for being the deepest turn in the corpus at which one render decided the game: 20 mana
floating, X affordable to 17, library 10 cards, 2 undeclinable draws owed. Rows 1–9 carry
`LOSE the game`; the header names X=8 as the largest value the library pays for; the seat took
X=8 and won 80 to -1. Option 1 (X=17) is what a pure life-and-cards reading picks, and it
decks the seat on the spot.
