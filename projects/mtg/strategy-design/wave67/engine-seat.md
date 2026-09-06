# Wave-67 engine-narration seat

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/` (binary master `14977f3af`).
Everything below is measured from the 42 seat JSONLs with python and `/usr/bin/grep`; no file under
`bin/Res` or `src/` was touched, no git, no wagic run. Counts are stated as N of M with file+seq.
Seat files are cited by their short handle (`deck123-0x55f22675b`) — the full name is
`<epoch>-ai_baka_<handle>-vs-ai_baka_<opp>.jsonl`.

---

## 1. CORPUS CENSUS

| quantity | value |
|---|---|
| games | **21 natural, 0 crashed, 0 hung, 0 timeouts/draws** (21 `WAGIC_SELFPLAY_RESULT` lines, 42 `gameend` records, 21 rows in results.tsv) |
| seat translogs | 42 |
| records | **2436**; 2305 carry a rendered `prompt` |
| kinds | ask 1643, priority 496, attackers 74, blockers 33, discard 32, reveal 21, bottom 3, wall_miss 3, recovery 5, gamestart/system/gameend 42 each |
| fallbacks | **7 total**: `wall_miss_unrecorded_wall` 3, `index_name_conflict` 2, `engine_answered` 1, `named_row_reask` 1 |
| wave-58/59 failure signatures | `unparsed_reply` **0** (was 83), `empty_reply` **0**, `bad_reply` **0**, `reasoning_only` **0**, `degenerate_decode` **0**, `reveal_stall_forced` **0**, `transport_error` **0** |
| stale drops (`async_drops`) | **22** events on 22 records, every one `casting/question (or turn/phase)/re-asked`; 0 livelock give-ups (`kStaleLivelockLimit` never reached) |
| transport failures | 4 records carry `transport` = `curl=28,http=0,empty=1,connect_ms=20000,phase=wall`; `wall_miss_events` 4, `wall_miss_unrecorded` 3, 1 recovered by the wall-miss retry |
| deadline | `deadline_pct >= 95` on 5 records; **`deadline_pct > 100` on 1** (137.0, see §3 HIGH-2) |
| latency (n=2304 round trips) | p50 **20.5 s**, p90 **75.4 s**, max **1233.4 s** |
| HOLD (prefix-matched on the rendered prompt) | a `Hold priority:` row printed in **1455** prompts, **taken 435** times; `hold_windows_skipped` **3160** |
| other gameend counters | `mana_only_windows_skipped` 343, `identical_ask_answers_reserved` 48, `identical_option_asks_resolved` 3, **`reserve_decline_windows_skipped` 0**, **`engine_reveal_floor_picks` 0** |
| SEGV / abort / assertion | none — no `.stderr` matches `Segmentation`, `Aborted`, `terminate called` or `Assertion` |
| translog gap past the watchdog | none — no record carries an abandonment stamp; the only >600 s wait is HIGH-2's, which published |

### The nine lane-flagged questions

1. **`unparsed_reply`** — **0 of 2436** (wave 66: 83). AV's prediction met with room. No residual shapes:
   the `So PUT:` discard shape AV deliberately left unwired **did not occur** (0 matches of
   `^\s*(So|Therefore,?)\s+(PUT|CHOICE|BLOCKS|ATTACK):` anywhere in the corpus), and all **32 discard
   records parsed** with 0 fallbacks.
2. **Answer-first and overrun** — answer-first **2275 of 2301 = 98.9%**, up from 80.2%. Replies opening
   with a `REASONING:` heading fell from 380 (18.1%) to **0**; line-leading `PLAN:` 1735/2301 = 75.4%
   (was 75.0%). `post_answer_overrun` **1732 nonzero, 662,814 B total, median-nonzero 249 B** — up from
   1390 / 630,880 B / 177 B. **No new heading was transcribed as a label**: the 7 line-leading heading
   phrases in the corpus are all cues AV wired — 6 × `Correction:` (`deck123-0x55ab261b61b` s31,
   `deck130-0x55912c7d4` s31/s45, `deck123-0x55cb3de4e` s39, `deck123-0x55b566d9e` s17,
   `deck123-0x55f9b7597` s36) and 1 × `So:` (`deck126-0x5621f0336` s10) — and every one parsed to a
   choice.
3. **`repeat_clamped_to_own_stop` 0 / `repeat_past_stop` 0** — and **20 takes are still past a stated
   stop**. See HIGH-1: the guard is gated on `namedCount >= 1`, and all 20 rode the single-activation row.
4. **`reveal_fallback_pick` 0**, `reveal_stall_forced` **0**, `engine_reveal_floor_picks` **0**. 21 reveal
   records: 20 with `choice >= 1` and a named card, 1 (`deck152-0x5559d82f8` s16) `engine_answered` on
   `none (no legal target)` — a window with nothing to pick. `reveal_wait_unexplained_secs` is **0 on all
   20** measured reveals; max `reveal_wait_driver_secs` **0**.
5. **The X ceiling** — **10 X windows, all 10 carry `{library:}`**, and **no seat decked out**.
   `deck125-0x55912b4ba23` s90 (t43) is the fix working live: library 4, option 1 = X=9 tagged
   `9 of your 4 library cards - 0 left, which is 5 MORE than the library holds`, and the seat answered
   **row 6 (X=4)** instead of option 1. I checked every `{library:}` clause against the same prompt's
   `Your library: N cards` line — **0 wrong**. Residual (STRATEGY, not a false surface): the badge on
   that window said `X=1 is the largest listed X the library pays for` (reserving 1 draw step + 2 Staff
   of Nin upkeeps — verified against the battlefield line, **two** Staffs, so the doubled clause is
   correct) and the model took X=4, emptying its library; it won anyway on turn 46.
6. **The reservation-decline latch never held** — `reserve_decline_windows_skipped` is **0 on all 42
   seats**, so it never held at all, and therefore never held across a changed board. AZ R3's full-board
   key did what R3 said it would; AX's own prediction that it would fire on a deck-162 seat is falsified.
7. **I9a band answers** — 7 collapsed-band windows (all deck152 seats). Every one printed **all 21 rungs**;
   answers were row 1 (`don't add any counter`, ×5) and row 2 (`add 1 counter`, ×2). **No answer landed
   inside the collapsed band**, so the `shownToFull` round-trip is still unexercised — 0 mis-maps, but
   also 0 tests. 0 `named_row_reask` and 0 `unparsed_reply` on any of the 7.
8. **The graveyard line** — rendered in **2256 of 2304** prompts (`Your graveyard (` or `your graveyard is
   empty`); `Their graveyard` in 1855. **77 replies reference a graveyard** and read it as given; only 2
   replies are census-shaped and neither re-derives a count the frame already prints, so the
   `146v130` s29/31/33 shape did not recur. boardKey re-opens did **not** rise into anything visible:
   `async_drops` 22 (all re-asked), `identical_ask_answers_reserved` 48, and the median seat is 49 records
   (min 15, max 144).
9. **`label_missing_salvaged` 0 / `label_missing_reask` 0** — **no label-free reply occurred in the whole
   corpus**, so the salvage matcher and the negation refusal are both UNTESTED (never a negated salvage,
   because never a salvage).

### The three stats-line flags

**(a) The 1233 s record vs the 900 s deadline — mechanism.**
`deck162-0x56442565e5c` **seq 13** (`blockers`, t13), `latency_ms` 1233411, `deadline_pct` 137.0,
`transport` `curl=28,http=0,empty=1,connect_ms=20000,phase=wall`, `wall_miss: 1`, `retry: 1`, and a
consumed 1853-byte reply. **No single request exceeded the deadline.** The mechanism is the two-attempt
sum, and it is by design in two places:

* `AIPlayerGPT.cpp:15180-15205` — a wall miss (`mLastTimeout`) sets `mRetryBudgetMs = 0`, which the
  comment states means *"the full deadline again"*; only the transport arm gets
  `remainingTransportRetryMs(mTimeoutMs, mLastLatencyMs)`. So AX's "the 900 s curl-28 wall has zero retry
  budget by arithmetic" is true of the *transport* arm and false of the *wall-miss* arm, which the same
  record rides.
* `AIPlayerGPT.cpp:15090-15091` — when the retry finishes, `mLastLatencyMs += mRetryFirstLatencyMs`.
  900035 + 333376 = 1233411 exactly.

`CURLOPT_TIMEOUT_MS` is set to `timeoutMs` per request (`GptConfig.cpp:872`), so each leg is bounded at
900 s; the seat's worst-case wait on ONE decision is **2 × the deadline**, not one. Two consequences are
defects rather than design (§3 HIGH-2 and MED-1): `deadlineTenthsPct(mLastLatencyMs, mTimeoutMs)`
(`:14276`, called at `:16047`) divides the *summed* latency by *one* deadline, so the field reports a
deadline overrun that did not happen; and no p90/max latency consumer can tell a 1233 s round trip from a
900 s miss plus a 333 s success.

**(b) `post_answer_overrun` 71% vs AV's protocol predictions.** 1732 of 2436 (71.1%), 662,814 B, up from
1390 / 60% / 630,880 B. AV predictions 4 and 5 are both **FAIL** and they fail in the same direction and
for the same reason: the rewritten `kReplyProtocol` moved the coded line to the front (answer-first went
80.2% → 98.9%, `REASONING:` openings 380 → 0), and *every byte of reasoning the model still writes is now
post-answer by construction*. The protocol did not shorten replies; it moved them past the label. The
lever AV said to stop paying for (prediction 4's own falsifier — "if line 1 is still the coded line in
>70%, the ORDER instruction is not the lever") is now at 98.9%: **wave 68 should stop spending protocol
words on ORDER and start spending them on LENGTH**, because the overrun meter is now measuring the whole
reasoning budget and nothing else. `post_plan_overrun` is 68 nonzero / 115,340 B (was 200 / 219,885).

**(c) The two lopsided finishes — both traced, neither is an engine defect.**
`152v123` **-119/8, t18**: `deck152-0x55cb3cb15e6` s43 is a bundled blockers window against **34
attackers for 138 damage** (A1 Lord of Lineage 5/5 flying, A2 Bloodline Keeper 5/5 flying, A3-A34 Vampire
#1-#32 4/4 flying) with **one** blocker, Sigarda, who can only block a 4/4 for an even trade. The prompt
states it exactly: `Unblocked, these attackers deal up to 138 - you would be at -119 - LETHAL if it all
connects`. The seat answered `no blockers`; blocking would have made it -115. The -119 is deck123's
Bloodline Keeper/Lord of Lineage engine going critical, correctly rendered and correctly read.
`146v125` **0/51, t46**: a 46-turn control grind, not a blowout — `deck146-0x56285ffe5` sat at ≤3 life
for its last 19 records while deck125 climbed via Sphinx's Revelation lifegain (opp_life trace 20 → 28 →
45 → 51) and countered its outs; s140 and s141 both end `... was COUNTERED by Dream Fracture`. The
spread is the lifegain, not a one-hit swing.

---

## 2. PREDICTION LEDGER (36 adjudications: 22 PASS, 8 FAIL, 6 UNTESTED)

Split rows (AW4, AX5, AY-I6c, AY-I9b, AZ4) are counted by their PRIMARY half — the one the lane wrote
as its falsifier — and the other half is stated in the same row.

### lane AV — the reply rule

| # | prediction | verdict | evidence |
|---|---|---|---|
| AV1 | `unparsed_reply` < 5 | **PASS** | **0 of 2436**; every seam parsed, incl. 32/32 discard |
| AV2 | no line-leading `ANSWER:`/`REASONING:` heading over an answer | **PASS** | `REASONING:` openings **0 of 2301** (w66: 380). The 7 heading-lines present are the wired cues (6 `Correction:`, 1 `So:`), all parsed — enumerated in Q2 above |
| AV3 | `label_missing_salvaged` ≈ 1 in 5 of label-free replies | **UNTESTED** | **0 label-free replies** in the corpus; both counters 0. No window arose |
| AV4 | answer-first share falls from 80.2% | **FAIL** | **2275/2301 = 98.9%**, +18.7 pts. AV's own falsifier (">70%") is met three times over |
| AV5 | overrun total < 630,880 B, median-nz < 177 B | **FAIL** | **662,814 B** (+5.1%), median-nz **249 B** (+41%) |
| AV6 | `later_answer_ignored` falls from 48; no `answer_anchor_override` | **PASS** | **38** (−21%); `answer_anchor_override` **0** — the anchor did not move |
| AV7 | no new `plan_answer_line_ignored` / `latched_row_mismatch` growth | **PASS** | `plan_answer_line_ignored` **0**; `latched_row_mismatch` **5** on 55 latched records; the heading skip moved no line |

### lane AW — the X ceiling, the bands, the four M items

| # | prediction | verdict | evidence |
|---|---|---|---|
| AW1 | no deck-out on a recommended X | **PASS** | 0 `gameend` where the loser's library hit 0. The one seat that emptied its library (`deck125-0x55912b4ba23` s91/92, t43) **won** (125 beat 130, 23-0, t46) |
| AW2 | `{library:}` on 100% of drawing-X rows; max-X take rate below 14/14 | **PASS** | **10 of 10** X windows carry it, 0 bare; `deck125-0x55912b4ba23` s90 answered **row 6 (X=4)** over option 1 (X=9). Sample is 10, above AW's own ≥6 bar |
| AW3 | ceiling menus carry the header sentence; median latency > 3.3 s | **PASS** | **3 of 3** `SECOND CEILING` windows carry `YOUR LIBRARY IS THE SECOND CEILING`; their latencies 3283 / 8244 / 11928 ms, median **8.2 s** |
| AW4 | ≤ `paid+2` add-N rows; menu bytes 2.7 kB → under 0.6 kB; no mis-map | **FAIL (bytes/rows), UNTESTED (mapping)** | All 7 band windows print **21 of 21** rungs at **2514-2737 B** — a byte saving of ≈0%. This is AZ R4 overriding AW I9a inside the same binary (§3 MED-2). No answer landed in the band, so the `shownToFull` round-trip is untested; 0 `named_row_reask`, 0 `unparsed_reply` on the 7 |
| AW5 | zero replies write "lethal" of a NOT-lethal ATTACK TOTAL | **PASS** | 74 prompts carry `ATTACK TOTAL`, **61** carry `That is NOT lethal`; 3 candidate replies inspected by hand (`deck130-0x55ddfae7758` s27, `deck152-0x55cb3cb15e6` s34, `deck123-0x55b566d9e` s28) — all three are about a FUTURE turn or say "does not deal lethal damage". **0 violations** |
| AW6 | 0 `unparsed_reply` on a sorcery-bracket window; no >3000-char argument with the bracket | **PASS** | **674** windows carry `[no cast row now: sorcery speed`; **0** unparsed. 5 replies exceed 3000 chars on such windows; I read all 5 — none argues with the bracket (they argue combat math, a life loop, and a counterspell). The wave-66 7212-char shape did not recur |
| AW7 | 0 replies assert a tapped arrival for a Pathway-class MDFC row | **PASS** | The new tag `{it enters UNTAPPED - it makes mana this turn}` renders in **28** prompts; 4 reply sentences pair "Pathway" with "tapped" and all 4 are about a *different* land entering tapped or a Pathway already tapped for mana. **0** arrival assertions |
| AW8 | tribute-class rows at >5 opposing creatures taken at a lower rate | **UNTESTED** | **0 windows** offered an edict at >5 opposing creatures. 8 edict rows were taken (at 0, 2, 4 and 5 creatures). No window arose |

### lane AX — reveal, fallback receipts, transport

| # | prediction | verdict | evidence |
|---|---|---|---|
| AX1 | 0 `reveal_stall_forced`; every reveal a real card or a `reveal_fallback_pick` | **PASS** | 0 force-closes; **20 of 21** reveals carry `choice >= 1` and a named card. The 21st (`deck152-0x5559d82f8` s16) is `choice -1` / `engine_answered` on `none (no legal target)` — its own window had no legal pick, so it is not AX's falsifier |
| AX2 | no `reveal_wait_driver_secs >= 600`; total unexplained 0 | **PASS** | max driver **0**; `reveal_wait_unexplained_secs` **0 on all 20**. The 600 s stall is gone |
| AX3 | residual 0 on every reveal with `latency_ms > 0` | **PASS** | **20 of 20**; the largest reveal wait is 72 s (`deck126-0x564424341f2` s10) against a 71.5 s round trip |
| AX4 | every `ask`/`priority` recovery carries `executed_seam`/`_choice`/`_text` | **FAIL** | 1 recovery has `recovers_kind: ask` — `deck123-0x55f9b7597` **s42** (recovering s41 `named_row_reask`) — and it carries **neither** `executed_seam` nor `recovered_by`. AX's falsifier is literally met. See §3 MED-3 for why: the `named_row_reask` path opens a recovery record but never reaches a `noteHeuristicExecuted` call site (only 3 exist: `:33057` priority, `:34568` land, `:35664` cast) |
| AX5 | 0 recurring reserve flips; `reserve_decline_windows_skipped > 0` on a deck-162 seat | **FAIL (fire), PASS (flips)** | `reserve_decline_windows_skipped` is **0 on all 42 seats** — the latch never held once, so no flip could recur and no deck-162 seat shows a firing. AX said its own scope was "the dial, not the doctrine"; the measurement is that the AZ R3 key makes it inert on this pool |
| AX6 | transport unchanged: `wall_miss` iff the full deadline is spent, `transport_error` only below the band | **PASS** | 4 transport-stamped records, **all** `phase=wall` at `deadline_pct >= 100`; **0** `transport_error`; 0 wall misses under 95% |

### lane AY — the own stop, the graveyard, the no-op conjunction, the render MEDs

| # | prediction | verdict | evidence |
|---|---|---|---|
| AY-I6a | 0 `repeat_past_stop_exhausted` without a clamp beside it | **UNTESTED** | 0 of each; the guard never fired, so the pairing was never exercised |
| AY-I6b | no record executes a repeat count greater than `stop − M` from a reply whose PLAN states both | **FAIL** | **20 records** do. Full list in §3 HIGH-1 |
| AY-I6c | the `your stated stop=` share rises; a window right after a refusal carries the clause | **PASS (share), UNTESTED (after-refusal)** | **41** windows render `your stated stop=` against 55 windows carrying a `[repeat:` row — the persisted `mStatedStop` survives, incl. across passes (`deck123-0x55f22675b` s17-s32 all render it after a pass). **0 refusals occurred**, so the refusal half never arose |
| AY-I8a | 0 seats re-derive a graveyard census in prose where the frame carries `Your graveyard (` | **PASS** | The line is in **2256 of 2304** prompts; 77 replies read it, 2 are census-shaped and neither re-derives a printed count. The 165-second `146v130` s29/31/33 shape did not recur |
| AY-I8b | 0 windows render `is one resolution from closing` about a half in an exile zone | **PASS** | 136 `LOOP HALF PENDING` banners: 56 positive, 70 negated. All **10** exile-half banners read `A card in exile does not come back unless another card says so, so the pair is BROKEN` (`deck126-0x557716215` s34/s47, `deck146-0x557714edd` s39-s42, …). The invented-Sanguine-Bond window shape did not recur |
| AY-I9b | every noop-row take argued against in prose carries `plan_contradicts_noop_row_reask`; 0 repeats of the s83/s84 shape | **FAIL (the re-ask), PASS (the detector)** | **1** noop-row take in the corpus: `deck123-0x55f9b7597` **s43**, `Cast Devour Flesh {right now: they control 0 creatures - at 0 this does nothing}`, over a reply that says `Devour Flesh at 0 creatures does nothing`. The anaphora/no-label predicate **fired** — `parse_note: named_row_reask_recovered;plan_contradicts_noop_row` — but the stamp is the bare note, **not** `..._reask`, and the dead row was executed. The seam's one-re-ask-per-board budget had already been spent on s41's `named_row_reask` (§3 MED-4). The s83/s84 *pair* (the same dead row twice in one turn) did not recur |
| AY-MED-exemplar | 0 `named_row_reask` windows recover to a row the same window tagged `HALF DEAD` | **PASS** | 1 `named_row_reask` window (`deck123-0x55f9b7597` s41→s43); its exemplar named **row 4, the Hold row**, and the recovery landed on a DEAD row the model chose itself, not a half-dead exemplar |
| AY-MED-bucket | no prompt has >4 same-shape lines inside one contiguous event batch; the t15-class prompt falls under 12 KB | **PASS (the rule), UNTESTED (the 12 KB)** | **0 of 2304** prompts violate the floor-of-4 rule (measured by normalising digit runs inside each contiguous `- ` run). **292** bucket tags rendered, largest collapse `[x15 …]` (`deck130-0x560d564dc44` s66). The 208-line / 96-token death batch never arose in this corpus, so the 25 kB → 12 kB claim has no case |

### lane AZ — the Codex review's seven

| # | prediction | verdict | evidence |
|---|---|---|---|
| AZ1 | `reserve_decline_windows_skipped` drops to 0 on most seats; no hold across a changed board | **PASS** | **0 on all 42 seats.** R3's full-board key made the latch inert; the corpus pairs cannot latch, as R3 predicted |
| AZ2 | no salvage of a negated sentence; `label_missing_reask` rises correspondingly | **UNTESTED** | 0 label-free replies, 0 salvages, 0 re-asks. No window arose |
| AZ3 | every `repeat_clamped_to_own_stop(...,executed=1)` is followed by an executed activation | **UNTESTED** | **0** clamp records. The clamp is unreachable on this corpus for the same reason as HIGH-1 |
| AZ4 | every Intrepid-class menu prints all its rungs; bytes fall ~half; a mid-band answer resolves to its rung | **PASS (rungs), FAIL (bytes), UNTESTED (mid-band)** | **21 of 21** rungs printed in all 7 windows — R4's row-preservation is live and correct. Bytes 2514-2737 B vs wave 66's ~2.7 kB: **no fall**. No mid-band answer arose |
| AZ5 | no `xLibraryReserveWhy` says "cannot decline" about a `MayAbility` stack draw | **PASS** | The 3 ceiling badges name only mandatory draws. `deck125-0x55912b4ba23` s90: `3 draws are already owed that you cannot decline (your next draw step … plus Staff of Nin's upkeep draw, plus Staff of Nin's upkeep draw)` — verified against the battlefield line, which lists **Staff of Nin #1 and #2**. The doubled clause is a true count, not a double-count |
| AZ6 | no banner claims "one resolution from closing" for a graveyard half with no recursion | **PASS** | Every graveyard-half banner reads `A card in a graveyard does not come back without a recursion effect, and nothing that could return it is on that battlefield or in that hand, so the pair is NOT one resolution from closing` (`deck162-0x56442565e5c` s13, `deck125-0x562c0f3213f` s62, …) |
| AZ7 | every `reveal_fallback_pick` names the highest-cost eligible card, indexed into the printed list | **UNTESTED** | **0** `reveal_fallback_pick` and **0** `engine_reveal_floor_picks`. No reveal was refused |

---

## 3. ENGINE / RENDER / NARRATION DEFECTS

### HIGH-1 — the own-stop guard is blind to the single-activation row: 20 takes past a stated stop, 12 of them contradicting the reply's own prose

**Repro.** `deck123-0x55f22675b` (123 vs 126) **seq 22**, t9, `kind: priority`.
Rendered clause (verbatim from the prompt):

> `{right now: M=27, your stated stop=25, so this window would add to a count ALREADY AT OR PAST your own stop …}`

Reply (verbatim): `CHOICE: 1 (Create human with Thraben Doomsayer)` / `PLAN: L=20, C=2, stop=25; M=27
now (stop reached). This window: pass. Next window: pass.`
Executed: `choice 1`, `chosen_text: Create human with Thraben Doomsayer [cost: Tap] [repeat: you control 27
creatures; activated this turn 2 times already…]`, `repeat_n` **absent**, `parse_note:
decision_reversed_in_prose;plan_choice_conflict_narrowed`, no fallback. M went 27 → 28.

**Mechanism** (`AIPlayerGPT.cpp:32639-32642`):

```
const bool repeatPastStop =
    repeatRowTaken && namedCount >= 1 && replyHasPlanLine(content)
    && repeatPlanStopAndCurrent(content, &planStop, &planCurrent)
    && planStop - planCurrent <= 0;
```

`namedCount >= 1` scopes the whole I6/H3 guard to the **counted** row (`… repeated N times, then stop`).
Only **4 records in the corpus** carry `repeat_n` at all; every one of the 20 violations rode the **plain
single-activation row**, where `namedCount` is 0, so `repeatPastStop` is false by construction and the
clamp at `:32817` is unreachable. AZ R2's `allowed==1` boundary is therefore also unreachable — which is
why AY-I6a and AZ3 both read UNTESTED. The wave shipped a guard for a row the model does not use.

**The second half.** 12 of the 20 also carry `decision_reversed_in_prose` — the reply's own prose states a
pass — and every one of those 12 also carries `plan_choice_conflict_narrowed`, i.e. the re-ask was
suppressed by `proseNamesOtherMenuRow` (`:32594`) because "This window: pass" names no *rival row by name*
on the menu. Across the whole corpus `decision_reversed_in_prose` is 15 and `plan_choice_conflict_narrowed`
is 15: **the narrowing removed 100% of the candidate re-asks**. Wave 55 narrowed this to stop false
firings; on this corpus it has narrowed to zero firings while the shape it was built for executed 12 times.

**Full list** (seat handle, seq, turn, the reply's own numbers):

```
deck123-0x55f22675b  s19 t9  stop=25 M=25 | s20 t9  stop=25 M=26 | s22 t9  stop=25 M=27
                     s29 t10 stop=25 M=29 | s30 t10 stop=25 M=30
deck123-0x55cb3de4e  s27 t17 stop=23 M=24 | s28 t17 stop=23 M=24 | s30 t17 stop=23 M=25
                     s31 t17 stop=23 M=26 | s33 t17 stop=23 M=27 | s34 t17 stop=23 M=28
                     s35 t17 stop=23 M=29 | s36 t17 stop=23 M=30 | s37 t17 stop=23 M=31
                     s38 t17 stop=23 M=32 | s40 t17 stop=23 M=33 | s46 t18 stop=23 M=34
                     s48 t18 stop=23 M=35
deck123-0x55b566d9e  s31 t15 stop=29 M=29 | s32 t15 stop=29 M=30
```

**Cost.** 20 full round trips, plus the passes interleaved between them; `deck123-0x55cb3de4e` spent
**13 windows in turns 17-18** alone walking M from 24 to 35 against a stop of 23. 20 of the 41 windows
that rendered `your stated stop=` (**49%**) executed past it.

**Fix shape** (mechanism, not an owner position): drop `namedCount >= 1` from `repeatPastStop` and treat a
take on any repeat row as `namedCount = 1` for the stop test, so the single activation is refused on the
same terms as a counted one; and reconsider whether a bare `this window: pass` verdict beside a take should
buy the re-ask that `proseNamesOtherMenuRow` currently withholds.

### HIGH-2 — `deadline_pct` reports a deadline overrun that did not happen, and one decision can burn 2× the deadline

**Repro.** `deck162-0x56442565e5c` **seq 13**, t13, `kind: blockers`: `latency_ms 1233411`,
`deadline_pct 137.0`, `transport curl=28,http=0,empty=1,connect_ms=20000,phase=wall`, `wall_miss 1`,
`retry 1`, and a consumed answer (`BLOCKS: B3:A1`, `later_answer_ignored;long_reply`, 872 bytes trimmed).

Two separate problems live in that one record:

1. **The meter lies.** `AIPlayerGPT.cpp:15090-15091` sums both attempts into `mLastLatencyMs`; `:16047`
   then calls `deadlineTenthsPct(mLastLatencyMs, mTimeoutMs)` (`:14276`), which divides the **sum** by
   **one** deadline. `deadline_pct > 100` is the brief's own wave-58/59 signature for "a call ran past its
   deadline" — here it fires on a record where *neither* leg did. Every downstream consumer (this census
   included, and the harness's p90/max) reads a 1233 s round trip. A `retry: 1` record needs either its
   own `first_latency_ms` / `retry_latency_ms` split, or a `deadline_pct` computed per attempt.
2. **The budget is 2× on the wall arm.** `:15196` — `mRetryBudgetMs = mLastTimeout ? 0 : transportBudgetMs`
   and the comment at `:15187-15189` states 0 means "the full deadline again". The transport arm's comment
   at `:15170-15176` promises "keeps the seat's total wait at one deadline"; that promise is scoped to the
   transport arm only, and the wall arm — which is the arm every one of this corpus's 4 misses took —
   spends up to 1800 s on one decision. On a 21-game corpus that is 3 unrecovered misses (heuristic
   answered) plus one 20.5-minute window; on a human seat it is a 20-minute freeze on a blockers
   declaration. Whether the wall retry should get a *remainder* rather than a fresh deadline is an
   engine call, but the record and the comment currently disagree about what it does.

### MED-1 — a `recovery` record can carry no evidence at all

`deck123-0x55f9b7597` **seq 42** recovers s41 (`named_row_reask`) and carries **neither** `executed_seam`/
`executed_choice`/`executed_text` **nor** `recovered_by` — the record names a failure and says nothing
about what happened. AX's I7 wired 3 call sites (`noteHeuristicExecuted` at `:33057` priority, `:34568`
land, `:35664` cast); a **re-ask** class opens a recovery record on a path that reaches none of them, and
`flushRecoveryRecord` (`:15843`) correctly stays silent rather than claim an unknown row. That is the right
default and the wrong outcome: 1 of 5 recovery records in this corpus is fully blind. (The other 4: 3
`wall_miss` recoveries also carry no `executed_seam` — the wall_miss seam is not wired either — and 1
carries a rich `recovered_by`.) The narrow fix is to stamp the re-ask path, or to suppress the recovery
record on a class that recovers by re-asking rather than by the heuristic.

### MED-2 — I9a's byte saving is zero after AZ R4, and the two lanes shipped in one binary

7 collapsed-band windows, all deck152. Every one prints **21 of 21** rungs at **2514-2737 B** against wave
66's ~2.7 kB for the same menu. AW I9a was justified by "5 occurrences on the corpus, ~13.5 kB of menu"
(`:36452-36457`) and predicted "under 0.6 kB"; AZ R4 then correctly restored the printed list (hiding a
legal row is the render removing a decision), and what remains is a description swap that saves nothing:
19 rows lose a long price/taps clause and gain `{identical in effect right now: adds 1 counter}`, while the
kept row gains the band tag. Verbatim from `deck152-0x5621eeffddb` s16:

```
 2 | add 1 counter {this mode has a legal object right now} {repeat cost: 1 x {1}{w} = 2 mana for all 1; …}
 3 | add 2 counters {this mode has a legal object right now} {identical in effect right now: adds 1 counter}
 …
20 | add 19 counters {this mode has a legal object right now} {identical in effect right now: adds 1 counter}
21 | add 20 counters {…} {repeat cost: 20 x {1}{w} = 40 mana for all 20; …you have 2 spendable now, which pays for 1 of them and stops}
```

The information is now honest and the bytes are unchanged. If the byte cost is the item, the remaining
lever is the *range* form the corpus already uses elsewhere (`2-5. X = 8 down to X = 5 - one option per X
in that range …`, seen on the X menus) — which keeps every row answerable AND collapses the print. Flagging
it because a per-deck reader sees one deck's menu and cannot see that the wave's stated 13.5 kB saving did
not land.

### MED-3 — the no-op re-ask loses to the one-re-ask-per-board budget

`deck123-0x55f9b7597` **s41 → s43** (t27, 7 life). s41 answers `CHOICE: 2 (Cast Bloodline Keeper)` for a
card not on the list → `named_row_reask`, one re-ask spent. s43 answers the corrected question with
`CHOICE: 2 (Cast Devour Flesh)` — a row whose own note reads `{right now: they control 0 creatures - at 0
this does nothing}` — over a plan that says `Devour Flesh at 0 creatures does nothing`. AY's I9b predicate
**fired** (`parse_note: named_row_reask_recovered;plan_contradicts_noop_row`), and the dead spell was cast
anyway at 7 life, because `mPriorityReaskBoard` / the ask seam's one-re-ask-per-board budget was already
consumed by s41. The detector is right and unheard. Either the budget should be per *class* rather than per
board, or a second, different self-contradiction should be allowed to spend the board's re-ask once.

### MED-4 — the narration batch still reaches 484 lines / 30 KB after bucketing

`deck125-0x56286131d4c` **seq 101** (t41): a 30,503-byte prompt containing **484** `- ` event lines. The
run-bucketer works exactly as specified (0 prompts violate the floor-of-4 rule; 292 tags rendered), but it
only collapses **within one contiguous run of ≥4 equal shapes** — a long game's log is mostly *unique*
lines, and nothing in the wave addresses volume. Largest prompts in the corpus: 35,417 B
(`deck126-0x557716215` s64, blockers), 34,712 B (`deck146-0x557714edd` s95, attackers), 33,369 B, 32,426 B,
31,519 B. Naming it because the wave-66 finding was framed as "a 208-line batch inside a 25 KB prompt" and
the batch is fixed while the 25 KB is not.

### LOW-1 — `answer_replaced` fires 7 times and `commit_retracted` never

7 records carry `answer_replaced: true` and **0** carry `commit_retracted: true` across 2305 windows.
Worth one line in a future census because the pair is the instrument for AV's anchor question; a permanent
zero on one half is either good news or a dead counter, and this corpus cannot tell them apart.

### LOW-2 — `reply_trimmed_bytes` 78,840 B over 31 records

31 replies were trimmed, 78.8 kB total, max on `deck162-0x56442565e5c` s13 (872 B). Two records carry
`long_reply`. Small, but it is bytes the model spent that the seat threw away, and it moves with the
overrun finding in (b): the trim is now cutting *post-answer* text almost exclusively.

---

## 4. WHAT I DID NOT CHECK

* **No file under `bin/Res` or `src/` was modified, no git command was run, no wagic invocation.** Every
  engine claim above is read from `AIPlayerGPT.cpp` / `GptConfig.cpp` source and from the corpus records;
  nothing was reproduced by running the binary, so the two HIGH items are diagnosed, not re-demonstrated.
* **I did not verify any card's Oracle text against Scryfall or the primitives.** No finding above rests on
  a card fact except the Staff of Nin count (read off the prompt's own battlefield line) and the Pathway
  arrival (read off AW's PARSETEST case and the rendered tag) — neither was checked against
  `borderline.txt` or Scryfall by me.
* **I did not read the seven per-deck reviews**, and none of the above is reconciled against them.
* **The 98.9% answer-first figure counts a coded label on reply line 1** across every seam
  (`CHOICE|BLOCKS|ATTACK|PUT|REVEAL|X|BOTTOM|MULLIGAN|KEEP`); AV's 80.2% baseline used its own definition
  and I did not re-derive wave 66 with mine, so the +18.7 pt delta could carry a definition component. The
  `REASONING:`-openings comparison (380 → 0) uses AV's own definition and is clean.
* **The `{library:}` audit compared each clause to the same prompt's `Your library: N cards` line**, not to
  an independent zone count — a shared upstream error would be invisible to it.
* **I did not measure parse cost**, did not run PARSETEST or the suite, and did not check the two known
  concurrency-only failures.
* **AW6's "no argument with the bracket" verdict is my read of 5 replies**, not a predicate.
