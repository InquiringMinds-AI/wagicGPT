# Wave 70 — engine-narration seat

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849` (binary master `14c82df37`,
`--thinking on`, `WAGIC_GPT_TIMEOUT=900`). 21 games launched, 20 completed naturally, **1 HUNG**,
0 crashes, 0 timeouts. 42 seat logs, 2,246 records, **2,119 model decisions**, 2,119 with native
reasoning (100%), 9 fallbacks (0.42%), 1 transport failure.
Comparison corpus (wave 69, thinking OFF): `matchups-20260906-134120`, 42 seats, 2,282 decisions.
All counts below are from the JSONL and the per-game stderr; I did not build or run wagic.

---

## 0. HIGH-0 — the HUNG game, reconstructed: a declined MDFC face menu is an unbreakable livelock

**Repro (deterministic, no model needed once the state is reached):** a seat holds an unused land
drop, its hand holds a modal double-faced card whose back face is a land, the land-drop seam picks
that card, and the face menu that follows is answered with its **decline** row.

**Files.** `game-152v126-1788752967.stderr` (1,234,251,264 bytes — 1.23 GB, 31,012,527 lines);
seat logs `1788752973-ai_baka_deck152-0x558943c19f30-vs-ai_baka_deck126.jsonl` (ends at seq 55) and
`1788752973-ai_baka_deck126-0x558944f528a0-vs-ai_baka_deck152.jsonl` (ends at seq 17, a reveal).
`results.tsv` row: `152 126 hang 20 1 15`.

**The last consumed decision.** deck152 seat, **seq 55, turn 15, Main phase 1, my_life 20 /
opp_life 1**, kind `ask`, 3 options, rendered as:

```
Choose an option for Hengegate Pathway:
1. Hengegate Pathway (menu text: Play Land) [PLAY THIS AS A LAND: ... taps for {W} ...]
2. Mistgate Pathway [PLAY THIS AS A LAND: ... taps for {U} ...]
3. Decline - do nothing
```

reply: `PLAN: Attack for lethal with all creatures. Decline land drop.` / `CHOICE: 3 (Decline - do
nothing)`. The board shows five attackers able to attack and the opponent at 1 life, so the answer
is *correct play* — the seat wanted combat, not a land. **This is a pure engine defect: a legal,
sensible answer is a hard hang.**

**What that choice executed.** `AIPlayerGPT.cpp:39920-39921` — `if (req.canDecline && pick ==
(int) fullCount - 1) act.choice = -1;` → `applyMenuChoice` clicks the menu's cancel item. The menu
closes; the land is **not** played; the land drop stays unused; Hengegate Pathway stays in hand;
nothing about the game state changes. Nothing records that this card's face menu was declined.

**The loop (verbatim from the stderr, the cycle that repeats 2,584,190 times):**

```
AIPlayerGPT: Land drop: which land do you play now, if any? ... -> chose 1 of 3
Should I play Hengegate Pathway?
shouldPlayPercentage = 90
 AI wants to play card.
- Next card to play: Hengegate Pathway
AIPlayerBaka: AI attempting to pay a mana cost.
-  Target: Hengegate Pathway
-  Cost: 
AIPlayerBaka: Card or Ability was free to play.  
AIPlayerGPT: dropped 1 display-toggle (Flip Side) row(s) from the menu; the back-face land row is offered
AIPlayerGPT: Choose an option for Hengegate Pathway: -> chose 3 of 3     <- ONCE, at the model
ActionLayer::doReactTo -1
AIPLAYER: menu answered this tick - deferring the priority pass (1/24)
```

**Mechanism hypothesis (mechanism-level, four closed links):**

1. **The decline is not sticky.** Declining the face menu leaves the proposing intent completely
   intact. `AIPlayerBaka`'s land-play heuristic re-evaluates the same card on the very next tick
   (`Should I play Hengegate Pathway? shouldPlayPercentage = 90 → AI wants to play card`) and
   re-opens the identical menu. Contrast the LAND-DROP seam one level up, which *does* handle its
   own decline (`AIPlayerGPT.cpp:36181-36186`, `DebugTrace("AIPlayerGPT: held the land drop");
   gotPayments.clear(); return NULL;`) — it returns "nothing to play", so Baka proposes nothing.
   The MDFC face menu has no equivalent: its decline returns to a caller that has already decided
   the card is being played.
2. **The re-ask is silent.** `askModel`'s state-plus-question cache (`AIPlayerGPT.cpp:34978-34983`,
   `mAskCache`) keys on `serializeGameState() + tailStr`. Because link 1 changes nothing, both
   halves are byte-identical every iteration, so the cached answer `3` is replayed with
   `mAskAnswerReserved = true` — no HTTP call, no `DebugTrace`, and **no translog record**. That is
   why the seat log simply stops at seq 55 with no error: from the engine's point of view the
   window is being answered correctly, forever, for free. (The `Choose an option for Hengegate
   Pathway: -> chose 3 of 3` line appears exactly ONCE in 1.23 GB.)
3. **The livelock breaker cannot fire.** `AIPlayerBaka.cpp:5569-5590`. `menuAnswered` is true every
   tick, so `mMenuPassHold` climbs to `kMenuPassHoldMax` and then enters the W64-AK arm:
   `else if (LegalActionsOracle::hasAnyLegalAction(this))` → `DebugTrace("AIPLAYER: menu pass floor
   reached at " << mMenuPassHold << " but a legal action remains - not passing"); return 0;`.
   Playing the land IS a legal action and always will be, so the floor never reaches its forcing
   arm. Measured over the whole 1.23 GB log: `menu pass floor reached … not passing` **2,584,166**
   times, `menu pass floor fired` **0** times. Across the other 20 games both strings are 0 — the
   guard has never been exercised except here, where it is the thing preventing recovery.
   The W64-AK comment's own premise ("it fires only where passing removes NOTHING") is exactly
   what makes it inert in a livelock: a livelock is *defined* by a legal action that never executes.
4. **No watchdog covers it.** 3,641 s of wall with zero translog writes and zero HTTP calls; the
   deadline/`deadline_pct` machinery only measures a decision that is in flight, and none was.
   The harness's per-game kill was the only thing that stopped it.

**Fix directions (proposals, not the owner's position).** (a) Make the face-menu decline sticky for
the window that proposed it — latch `(card, land-drop, turn)` as declined so Baka's land selection
skips it until the board moves, mirroring `held the land drop`. (b) Give the pass floor a
**no-progress** arm alongside the no-legal-action arm: N consecutive ticks in which the same menu
key is answered with the same answer and the serialized state does not change must force the pass,
whether or not a legal action remains — that is the only shape that can break link 3. (c) Cheap
tripwire independent of both: `mAskCache` should count consecutive reserved replays of the same
key and, past a threshold, refuse to serve and write a record, so the corpus can *see* a livelock
instead of inferring it from a 1.2 GB file.

**Status of the replacement run:** `matchups-20260907-065922` was in flight at the time of writing
(`REGIME-GATE-PASSED` present, `game-152v126-1788782363.stderr` 30 KB, `menu pass floor reached` 0).
Not adjudicated here.

---

## 1. COMPENSATION CENSUS — the revert list's evidence

Denominators: 2,119 decisions / 40 completed seats (w70) vs 2,282 / 42 (w69). "REMOVED" means lane
BK/BL/BM/BN deleted the field or its writer in step one, so the *mechanism* cannot fire; the column
that matters there is whether the decision it guarded went wrong anyway.

| mechanism | w69 (thinking OFF) | w70 (reasoning ON) | verdict |
|---|---|---|---|
| `off_protocol_bytes` (reply shape) | field absent | **103 of 2,119 (4.9%)**, all one class (§2) | **KEEP** as a meter — it is the only surviving reply-shape signal, and it is measuring a label omission, not prose |
| `post_answer_overrun` / `reply_overrun` | 1,588 of 2,282 (**69.6%**) nonzero | **2 of 2,119 (0.09%)** | the *behaviour* is gone; keep the counter, **DELETE** anything that reacts to it |
| `pre_answer_overrun` | 13 nonzero | REMOVED, and 0 preambles observed | **DELETE** |
| `post_plan_overrun` | 54 nonzero | REMOVED | **DELETE** |
| `reply_trimmed_bytes` | 28 nonzero | REMOVED | **DELETE** |
| `action_before_plan` | n/a (protocol was answer-first) | `action_before_plan_replies` **0 of 40 games**, 0 of 2,119 replies | **WATCH** (new; one clean corpus is not proof, but it is 0 of 2,119) |
| `reply_truncated`, `reply_truncated_reask`, `reply_truncated_answer_changed` | `reply_truncated` **58**, `reply_truncated_reask` fallback **5**, `reply_truncated_reask_recovered` **5** | fields REMOVED; **0** truncation of any kind, and no ANSWER cap cut a two-line reply: max PLAN line 422 B, p95 191 B, against answer ceilings 768/896/512 | **DELETE**. The 768-token answer cap is now ~10x oversized for the shape it must hold |
| `truncated_abandoned*`, `prose-salvaged` | present | 0 occurrences (classes unwritable) | **DELETE** the dead classifier branches (`commitRetracted`, `salvageProsePutList`) |
| `decision_reversed_in_prose` / `blocks_reversed_in_prose_reask` / reversal predicates | 22 notes + 1 fallback | REMOVED; nothing reversed an answer in 2,119 replies | **DELETE** |
| `later_answer_ignored` / `latched_row_mismatch` / multi-answer latching | 11 + 4; `latched_coded_line` 16 | `extra_answer_line` **1**, `multi_answer_first_taken` **1**, `latched_coded_line` **1** | **KEEP** (thin but non-zero — §2 class C) |
| `plan_choice_conflict` (fallback + re-ask) | fallback **25**, `plan_choice_conflict_recovered` 24, `_no_rival_named` 5, `_exhausted` 1 | `plan_choice_conflict_seen` **0 of 40 games** | **DELETE** — this was the single largest wave-69 fallback class and it is dead under reasoning-on |
| `plan_argues_against_row_seen` | n/a | **0 of 40** | **DELETE** |
| `index_name_conflict` / `distinct_index_reask` | 3 + 3 recovered / 1 | fallbacks **0**; notes `echo_index_conflict` 2, `index_name_unique_name` 2, `name_over_index` 1 — all *recovered without a re-ask* | **KEEP the note, DELETE the re-ask** |
| `repeat_count_reask` / `repeat_past_stop` | 2 / 1 | **0 / 0**; `repeat_annotated_take(n=1)` note fires once | **DELETE the re-asks**, KEEP the annotation |
| `label_missing_reask`, `noop_plan`, `attack_*`/`blocks_*` re-ask kinds | — | string absent from all 42 logs | **DEAD / already removed** |
| `noop_row_zero_reask` (+ `plan_contradicts_noop_row`) | fallback `plan_contradicts_noop_row_reask` **5** | fallback `noop_row_zero_reask` **6 of 2,119**, `plan_contradicts_noop_row_recovered` **4**, `noop_row_retaken` **2** | **KEEP** — the only re-ask that still earns its keep. It is now the corpus's largest fallback class (6 of 9) |
| `reasoning_chars` | 0 on all 2,282 | **min 330, p05 3,088, p50 5,883, p95 10,901, max 23,787; zero on 0 records** | **KEEP** |
| `reasoning_hidden` | — | **0** | **KEEP** (gate-critical, untested by absence) |
| `reasoning_budget_hit` / `reasoning_forced_close` | — | **19 records each**, always together; 4 escalated to an answer-locked retry (`max_tokens_reasoning=0`, `max_tokens_answer=400`) which recovered in 178–2,623 ms | **KEEP the retry**, **FIX the marker** — see MED-1 |
| phase-2 recovery counters (`phase2_answer_missing` / `_recovered`) | — | **0 / 0 across 40 games** *while 4 answer-locked retries demonstrably happened* | **WATCH — the counter and the mechanism disagree** |
| `reasoning_tail_answers` | — | **0 of 40** | **DELETE** unless the parser changes (BM prediction 5 FAILs) |
| PLAN carry (`kPlanCarryMaxChars = 400`) | PLAN lines were number ledgers (BK measured p99.5 = 2,286 B at ask) | **2,005 PLAN lines: p50 94 B, p95 191 B, max 422 B; 2 records ≥ 400 B**; `plan_paragraph_bound_cuts` **0 of 40** | **KEEP the cut** (it now costs nothing), **DELETE the paragraph-bound machinery** |
| PLAN step counts | BN measured ~8.25 steps | comma/`then` step count **p50 2, p95 5** | BN-4 **PASS** |
| plan restated verbatim vs changed | — | consecutive PLAN lines identical **139**, changed **1,824** (**7.1% verbatim**) | **WATCH** — the protocol says "write it out again word for word"; the model rewrites 92.9% of the time (§3, MED-3) |
| ledger-shaped PLAN lines (board facts, no intended action) | the wave-69 norm | rare; clearest cases `…deck130-0x56465b4f3e00…` seq 106 ("Opponent controls 16 lands and 37 life … no path to victory exists") and seq 108 | **WATCH**, no action |
| `plan_echo_count` | 650 records | **163 records** | KEEP |
| `hold_windows_skipped` (HOLD take) | 2,727 | **3,218** (cast 2,640 / priority 578). HOLD row rendered in **1,353** windows, taken **386** (prefix-match on `Hold priority:`) | **KEEP** |
| `mana_only_windows_skipped` | 153 | **260** | KEEP |
| `identical_ask_answers_reserved` | 52 | **90** | **KEEP but instrument** — the same replay path is link 2 of HIGH-0 |
| `identical_option_asks_resolved` | 2 | 4 | KEEP |
| `async_drops` | 49 | **30** (`async_drop_events` 0) | KEEP |
| `reserve_decline_windows_skipped`, `engine_reveal_floor_picks`, `wall_miss_*`, `commit_retracted`, `blocker_forecast_collapsed`, `put_gloss_stripped`, `draw` | 0 | **0 of 40** | **UNTESTED** (no window arose) — do not revert on a zero |
| `answer_replaced` | 9 | **1** (`…deck152-0x55875eb9c120…` seq 25) | KEEP |
| latency / `deadline_pct` | p50 **10,578 ms**, p95 64,936, max 315,224; deadline p50 1.1%, p95 7.2%, max 35.0% | p50 **80,689 ms**, p95 185,632, max 476,650; deadline p50 8.9%, p95 20.6%, **max 52.9%, 0 over 100%** | **ratio p50 7.6x, p95 2.9x.** 900 s timeout holds with ~2x headroom. Total decision wall 192,834 s across 21 concurrent seats |
| fallbacks by kind | **44 of 2,282 (1.93%)** across 9 kinds | **9 of 2,119 (0.42%)** across 3 kinds: `noop_row_zero_reask` 6, `unparsed_reply` 2, `http_error` 1 | reasoning-on cut the fallback rate **4.6x** |

**Reading of the table:** every wave-66..69 mechanism built to cope with in-band reasoning is dead
(prose overrun, reversal, truncation, plan/choice conflict, index/name conflict, repeat re-asks) —
0 firings between them where wave 69 had 44 fallbacks and 1,588 overruns. The mechanisms that
survive are the ones that were never about reasoning: the HOLD/mana window skips, the noop-row
re-ask, the ask/repeat caches, and the async-drop guard. **The revert list is the "DELETE" rows
above**; the "UNTESTED" rows are not evidence and must not be reverted on this corpus.

---

## 2. The 103 `off_protocol_bytes > 0` records — full classification

Shape over all 2,119 replies (`PLAN:` label case-insensitive; action labels `CHOICE:` / `ATTACK:` /
`BLOCKS:` / `PUT:`):

| class | n | `off_protocol_bytes > 0` |
|---|---|---|
| exactly `PLAN:` line + action line | **2,004 (94.6%)** | 0 |
| **A — plan sentence written with NO label** | **92** | 92 |
| G — action line only, PLAN line omitted entirely | 11 | 0 |
| **B — label written as `YOUR PLAN:`** | **7** | 7 |
| C — extra / wrong-seam answer line | 3 | 3 |
| F — empty reply (the HTTP 400) | 1 | 0 |

`off_protocol_bytes` equals **exactly the byte length of the unlabelled first line** in every A/B
case (verified: `'Play a land every turn.'` = 23 chars → `off_protocol_bytes: 23`). So **all 103 are
a LABEL defect, not prose**: the model wrote the plan it was asked for and omitted or renamed the
label. Nothing in the corpus is prose *in addition to* the two lines. Distribution: 6–289 bytes,
p50 55, one outlier at 289. Per seam (rate): `discard` 18/67 (26.9%), `reveal` 5/20 (25.0%),
`attackers` 7/78 (9.0%), `ask` 64/1,523 (4.2%), `priority` 9/411 (2.2%), `blockers` 0/20.
Concentrated by seat: **50 of 103 come from four deck125 seats** (the Staff-of-Nin ping loop).

**Class A, three representative quotes** (`…` = file suffix):

- `…deck125-0x56465a1d9370-vs-deck130` seq 43 (`off 40`):
  `Path to Exile targeting Rorix Bladewing.` / `CHOICE: 1 (Rorix Bladewing (6/5))`
- `…deck146-0x55ac89a9e110-vs-deck152` seq 32 (`off 118`):
  `Cast Acererak the Archlich to venture into the dungeon, activate Kaya's +1 on a Spider, and attack with all creatures.` / `CHOICE: 2 (Cast Acererak the Archlich)`
- `…deck125-0x564b817b51f0-vs-deck162` seq 78 (`off 289`, the largest):
  `then Staff of Nin from a row whose leaves N covers that counter's cost, then an Elixir row before any ping while my library reads 16 or less, then ping their face each turn - the punisher instead whenever a DRAW PUNISHERS line is printed; no Emrakul while a converter of theirs is printed.` / `PUT: 4, 7, 8`

Note the seq-78 quote: it begins mid-sentence with "then" — the model is continuing a plan it
believes it already started, i.e. the label is being dropped as part of a *carry*, not as a
formatting slip. 20 of the 92 class-A lines begin with `then`/`Continue`/`Win the game by`.

**Class B, all 7** are the literal string `YOUR PLAN:` — the model is echoing the prompt's own
second-person heading back as its label. Every one is at a `priority` or `ask` seam whose rendered
prompt contains the words `YOUR PLAN` as a section header. Quote:
`…deck125-0x56533d01b8d0-vs-deck146` seq 49 (`off 25`): `YOUR PLAN: Pass priority.` /
`CHOICE: 0 (Pass priority)`. **This is the wave-66 "the model transcribes the section name as the
label" failure that lane BL flagged as untested — it recurred, at a rate of 7 in 2,119.**

**Class C, all 3:**
- `…deck152-0x55875eb9c120-vs-deck123` seq 25 (`off 6`, the only `answer_replaced: true` in the
  corpus): a **reveal** seam answered `BLOCKS: Sigarda, Champion of Light blocks Vampire.` then
  `PUT: 1` — a *previous seam's* answer label leaked into this seam's plan slot and the parser had
  to latch the later line (`latched_line: "PUT: 1"`, `latched_line_offset: 53`).
- `…deck125-0x564b817b51f0-vs-deck162` seq 105 (`off 69`, `parse_note: extra_answer_line`,
  `post_answer_overrun: 69`): two `CHOICE:` lines; the first was taken, `ignored_line` records the
  second.
- `…deck125-0x564b817b51f0-vs-deck162` seq 104 (`off 107`): the "plan" line is a verbatim copy of
  the HOLD row's own rendered text.

**Class G (11)** — no `PLAN:` line at all, only the action line. These carry
`off_protocol_bytes: 0`, i.e. **a missing plan is invisible to the meter** while a mislabelled one
is counted. Examples: `…deck146-0x56533bce32f0…` seq 31 `CHOICE: 3 (Cast nothing right now)`;
`…deck162-0x5586fd1e16a0…` seq 22 `BLOCKS: B1:A1`. See MED-2.

**Does the label defect cost a decision?** No — the parser recovered the choice in 102 of 103
(the exception is class D/G's `HOLD PRIORITY: 2 (Hold priority)` at
`…deck125-0x564b817b51f0-vs-deck162` seq 110, which took `fallback: unparsed_reply` and handed
Sphinx's Revelation to the heuristic at seq 111). But the unlabelled line is *not* being consumed
as the plan carry, so 92 decisions carried no plan forward. And in a visible minority the
unlabelled line **contradicts the answer taken** — e.g. `…deck125-0x56465a1d9370…` seq 60
`Ping opponent with Staff of Nin.` / `CHOICE: 2 (Play Island)` — which under wave 69 would have
been a `plan_choice_conflict`. Under wave 70 nothing sees it, because the conflict predicate only
reads a labelled `PLAN:` line.

---

## 3. Corpus census

- **Games:** 21 launched, 20 natural, 1 hung (§0), 0 crashed, 0 SEGV/abort. Wave-58/59 hang-class
  signature **recurs once** (§0). `empty_reply` **0**, `transport_error` **0**, `bad_reply` **0**,
  `reveal_stall_forced` **0**, `deadline_pct > 100` **0** (max 52.9), no translog gap past the
  watchdog other than the hang's total silence.
- **Decisions:** 2,119. By seam: ask 1,523, priority 411, attackers 78, discard 67, blockers 20,
  reveal 20. Plus 42 `gamestart`, 42 `system`, 40 `gameend`, 3 `recovery`.
- **Fallbacks:** 9 (0.42%) — `noop_row_zero_reask` 6, `unparsed_reply` 2, `http_error` 1.
  3 produced `recovery` records; all 3 were executed by the heuristic
  (`executed_by: heuristic`): Elixir of Immortality life-gain, Sphinx's Revelation, and a "cast
  nothing".
- **Transport:** 2,118 × `curl=0,http=200,empty=0`; **1** × `curl=0,http=400,empty=1`
  (`…deck126-0x556e0f994e80-vs-deck125` seq 16, on the answer-locked retry leg — see MED-1).
  0 `transport_error` retries. `retry: 1` on 4 records, all budget-driven.
- **Stale drops:** `async_drops` 30 across 40 seats; `async_drop_events` 0.
- **Wall:** 0 timeouts, `wall_miss_events`/`wall_miss_no_retry`/`wall_miss_unrecorded` all 0 of 40.
- **HOLD:** rendered in 1,353 of 2,119 windows (63.9%), taken 386 (28.5% of windows offering it);
  `hold_windows_skipped` 3,218 (cast 2,640 / priority 578).
- **Reveal waits:** 20 records, 23–124 s (p50 ~73 s), `reveal_wait_ticks` 1,747–19,557,
  `reveal_wait_unexplained_secs` **0 on all 20**, `reveal_wait_driver_secs` 0 on all 20.
- **Caps observed:** `('ask'|'priority'|'attackers', 6768/768/6000)`, `('blockers', 6896/896/6000)`,
  `('discard'|'reveal', 6512/512/6000)`, and 4 answer-locked retries at `(400/400/0)`.
- **`reasoning_tokens` is absent on all 2,119 records** — the server never returns its own token
  count, so BK's owed budget re-fit can only be done from `reasoning_chars`.
  Per-seam `reasoning_chars` p50/p95/p99/max: ask 5,919/10,941/15,168/23,787 ·
  priority 6,219/10,334/14,423/15,103 · discard 6,373/12,089/13,446/13,446 ·
  blockers 6,188/11,762/11,762/11,762 · attackers 4,055/6,205/11,323/11,323 ·
  reveal 3,971/6,812/6,812/6,812.
- **`reasoning_degenerate`:** 1 record > 0.02, 0 > 0.05. The worst
  (`…deck126-0x556e0f994e80-vs-deck125` seq 10, 0.0254, 18,947 chars, **411 s**) repeats the line
  `I can tap Savannah {w}, Bayou #1 {b}, Bayou #2 {g}? No.` dozens of times until the budget closes.

---

## 4. Prediction adjudication

### lane BK
1. **PASS with one exception.** All 2,119 records carry `thinking: "on"` and
   `reasoning_chars > 0`; `max_tokens` is exactly 6000 + the seam answer ceiling on 2,115 records
   (6768 ask/priority/attackers, 6896 blockers, 6512 discard/reveal). The exception is the
   prediction's last clause: **4 records have `max_tokens: 400` (≤ 896) and
   `max_tokens_reasoning: 0`** — the answer-locked retry leg, which is legitimate (each is stamped
   `reasoning_forced_close`) but was not carved out of the prediction. 4 of 2,119.
2. **PASS.** `regime-gate-state.txt` lists all 42 seat logs; results exist; `--thinking on` in the
   launch. (I did not see the launch line itself; inferred from the stamps and the gate file.)
3. **UNTESTED.** No seat returned zero reasoning, so the FAIL arm never armed.
4. **PASS.** p50 10,578 → 80,689 ms (7.6x), p95 64,936 → 185,632 (2.9x); the timeout floor held —
   `empty_reply` 0 in both corpora, `deadline_pct` max 52.9%.
5. **UNTESTED.** No `--thinking off` probe in this run.

### lane BL
1. **PASS.** `post_answer_overrun > 0` on **2 of 2,119 (0.09%)** vs a 5% bar.
2. **PASS.** `coded_answers != 1` on 4 of 2,119 (0.19%: two records at 2, two at 0);
   `answer_replaced` **1**.
3. **PASS.** Exactly-two-labelled-lines = **94.6%** vs a 90% bar. (Order: 0 action-before-plan.)
4. **PASS by removal.** Truncation fields are gone and **no truncation of any kind occurred at any
   seam**; the discard/reveal PUT seams' 512-token answer ceiling was never approached
   (max reply 422 B).
5. **PASS**, for the stated honest reason (stamps removed). BM's census now says the reversals
   themselves are gone too: 2 of 2,119 records have any bytes after the action line.

### lane BM
1. **HALF-PASS / instructive FAIL.** "0 on the large majority" — **PASS** (95.1%). "Where non-zero
   the bytes sit BEFORE the PLAN line (a preamble)" — **FAIL**: in 99 of 103 the off-protocol bytes
   ARE the plan line (label omitted or renamed), and in 2 they sit AFTER the action line. The
   predicted shape — a preamble *in addition to* a PLAN line — occurred **0** times.
2. **FAIL (in the good direction).** `action_before_plan_replies` is **0 of 40 games**, not
   "small but non-zero", so the within-run trend could not be measured.
3. **PASS.** `coded_answers != 1` = 4; `extra_answer_line` = 1.
4. **FAIL.** `plan_choice_conflict_seen` = **0 of 40 games**. The instrument is live but the
   behaviour is gone under reasoning-on. Caveat that weakens this as a clean pass: the predicate
   only reads a labelled `PLAN:` line, and 92 replies had none — a plan/choice contradiction inside
   an *unlabelled* plan line (observed, e.g. `…deck125-0x56465a1d9370…` seq 60) is invisible to it.
5. **FAIL.** `reasoning_tail_answers` = **0 of 40**. B1.4's open question resolves the other way:
   the vLLM reasoning parser is not stranding a PLAN written inside the reasoning field.

### lane BN
1. **PASS.** 0 `truncated_abandoned*`, 0 `prose-salvaged` in all 42 logs.
2. **PASS.** `plan_contradicts_noop_row_reask` as a *fallback* is **0**; the class now appears only
   as `plan_contradicts_noop_row_recovered` (4) behind `noop_row_zero_reask` (6). The argument is
   arriving on the PLAN line as intended.
3. **PASS.** Every record has `max_tokens_reasoning > 0` except exactly the 4 stamped
   `reasoning_forced_close`; the gate passed and the corpus ran.
4. **PASS.** `plan_paragraph_bound_cuts` **0 of 40**; step counts p50 2 / p95 5 against the
   predicted ~4.7 mean (old ~8.25). PLAN lines p50 94 B vs BK's wave-69 p99.5 of 2,286 B at ask.
5. **UNTESTED.** No seat returned no reasoning.

### carried wave-69 items
- **Idyllic Tutor payload stranding / "never decrements the printed library after the reveal":**
  **does NOT recur as stated.** Traced `…deck123-0x560b901fade0-vs-deck125` seq 50–58 (turn 28):
  reveal at `Your library: 35 cards`, seq 51 still 35 with the card not yet in hand, **seq 52 = 34
  with Intruder Alarm in hand**. The count is correct once the search resolves; the residual is a
  **one-window lag** in which the tutored card is in neither zone. Same lag at
  `…deck146-0x556fe4060fc0-vs-deck126` seq 15 and `…deck126-0x556e0f994e80-vs-deck125` seq 41.
  Downgrade to LOW-2. The narration does name the payload (`You used: choose card with Idyllic
  Tutor targeting Intruder Alarm`).
- **146v123 replay divergence:** **UNTESTED.** Both 146v123 seats completed naturally at turn 12
  (`146 123 0 20 0 12`), no stranded `StackAbility`, no `index_name_conflict`. The window did not
  arise and I cannot run a replay from this seat.

---

## 5. Engine / render / narration defects

**HIGH-0 — declined MDFC face menu is an unbreakable livelock.** §0. Repro, files, seqs and the
four code sites there. 1 of 21 games lost; 1.23 GB of stderr; the pass floor provably cannot fire.

**HIGH-1 — the silent replay path hides a total game stop.** `mAskCache`
(`AIPlayerGPT.cpp:34978-34983`) and the repeat-ask re-serve (`:34992-35002`) answer a window with
no HTTP call, no `DebugTrace` on the cache path, and **no translog record**. Corpus-wide these
paths served 90 reserved answers legitimately; in the hung game the same path served ~2.58 M and
the seat log simply ends, so from the JSONL alone the game is indistinguishable from a seat that
stopped being asked. Repro:
`1788752973-ai_baka_deck152-0x558943c19f30-vs-ai_baka_deck126.jsonl` ends at seq 55 with no
`gameend`. Cross-deck: no per-deck reader can see this, because the evidence is an *absence*.

**MED-1 — `reasoning_budget_hit` fires ~4x too often, and its retry leg carries the corpus's only
transport failure.** 19 records are stamped `reasoning_budget_hit` **and**
`reasoning_forced_close` (they never appear apart, though the code comment at
`AIPlayerGPT.cpp:16874-16879` says the non-budget shape is `forced_close` alone). On **15 of 19**
the completed reasoning is 4,218–15,625 chars — roughly 1.1k–4.0k tokens against a 6,000-token
budget — and the reasoning ends cleanly with a well-formed two-line reply. Repro:
`…deck130-0x56465b4f3e00-vs-deck125` seq 93, `reasoning_chars 5880`,
`max_tokens_reasoning 6000`, reasoning tail `"…Everything is correct.\nProceeds."`, reply is a
clean `PLAN:`/`CHOICE:` pair. Only the **4** records that escalated (rc 18,947–23,787, i.e. at the
real cap) look budget-bound; their retry leg worked (178–2,623 ms second attempt) except
`…deck126-0x556e0f994e80-vs-deck125` seq 16, where the retry returned **HTTP 400**
(`transport: curl=0,http=400,empty=1`) and the decision fell to the heuristic at seq 17. Two
consequences: (a) a budget re-fit read off `reasoning_budget_hit` rate would be wrong by 4x;
(b) `phase2_answer_missing`/`phase2_answer_recovered` are **0 of 40** while four phase-2 closes
demonstrably happened, so at least one of the two counters is not wired to the path that ran.

**MED-2 — a missing PLAN line is invisible; a mislabelled one is counted.** 11 replies carry only
the action line and record `off_protocol_bytes: 0`, while 99 replies that *did* write a plan are
charged for it. Repro: `…deck146-0x556fe4060fc0-vs-deck126` seq 25,
reply `CHOICE: 2 (Hold priority: pass now, and do not ask me again)`, `off_protocol_bytes 0`,
no PLAN line. The meter that the wave-70 revert list depends on therefore under-reports the
protocol's *primary* requirement. Suggest a `plan_line_missing` stamp so "no plan" and "clean
two-line reply" stop looking alike (the same silent-instrument shape lane BK fixed for
`reasoning_chars`).

**MED-3 — the plan carry is not being carried.** The protocol asks the model to restate a standing
plan "word for word". Consecutive PLAN lines are byte-identical on **139** transitions and changed
on **1,824** (7.1%). Combined with the 92 unlabelled plan lines, the carry is mostly a fresh
sentence each window. Not a defect on its own — but every mechanism keyed on "the plan still
stands" (`repeatAskAnswerStands` compares `mRepeatAskPlan` to `mCurrentPlan`, `AIPlayerGPT.cpp:34994`)
is therefore firing on a plan string that changes almost every window, which is the likely reason
`identical_ask_answers_reserved` is only 90 across 40 seats.

**MED-4 — the prompt's own section heading is being transcribed as a reply label.** 7 records
answer with `YOUR PLAN:` instead of `PLAN:` (all quoted in §2, class B). This is exactly the
composition-seam failure lane BL flagged as its weakest untested claim. The fix is on the render
side (the heading), not the parser: accepting `YOUR PLAN:` would be tolerating prose-shaped drift.
Repro: `…deck125-0x56533d01b8d0-vs-deck146` seq 49; `…deck125-0x564b817b51f0-vs-deck162` seq 63.

**MED-5 — a seam's answer label leaks into the next seam.** `…deck152-0x55875eb9c120-vs-deck123`
seq 25 (reveal): `BLOCKS: Sigarda, Champion of Light blocks Vampire.` / `PUT: 1`. The parser
latched the second line (`answer_replaced: true`, `latched_line_offset: 53`,
`post_answer_overrun: 6`) and got the right answer, but the model believed it was in a blockers
window. 1 of 20 reveal records.

**LOW-1 — a render self-diagnostic fires on a legitimate row.** `W69-BJ F1: row states
' {paying this taps: ' more than once` appears **29** times across two games
(`game-152v162-1788752957.stderr` 24, `game-152v126-1788752967.stderr` 5). The row it flags is
correct — two distinct tap groups on one activation:
`put 1/1 counters with Katilda, Dawnhart Prime [cost: {4}{g}{w}, Tap] {paying this taps: Luminarch
Aspirant, Intrepid Adversary - they cannot attack this turn} {paying this taps: Katilda, Dawnhart
Prime - it cannot attack this turn}`. Either the diagnostic should allow disjoint tap groups or the
render should merge them; as shipped it is log noise that will be read as a defect.

**LOW-2 — Idyllic Tutor's one-window library lag.** §4, carried items. The tutored card is in
neither the library count nor the hand for exactly one decision window.

**LOW-3 — the answer ceilings are ~10x oversized for the two-line reply.** Max observed reply is
one 422-byte PLAN line plus an action line, against 768/896/512-token answer ceilings. This is
BK's owed re-fit and this corpus is the first that can pay for it: p99 of (PLAN + action) bytes is
well under 300 B at every seam.

---

## 6. Lategame specimen

`wave70/lategame-specimen.txt` — `1788752968-ai_baka_deck125-0x556e0e67a490-vs-ai_baka_deck126.jsonl`
seq 171, turn 50, Main phase 1, 141 vs 33 life, 5 options, model chose 1 (Cast Final Judgment),
29,418-byte prompt. Chosen because it is the deciding decision of the corpus's longest game (54
turns) *and* the clearest cap event: `reasoning_budget_hit` + `reasoning_forced_close`, 23,787
reasoning chars, then a 1,356 ms answer-locked retry at `max_tokens 400 / reasoning 0`.

---

## 7. What I did NOT check

- **I did not build or run wagic** (per the brief), so every code claim is a source reading of
  `src/AIPlayerGPT.cpp` and `src/AIPlayerBaka.cpp` at the tree's current state, not an observed
  execution. In particular the HIGH-0 mechanism is reconstructed from the stderr cycle plus those
  two sites; I did not confirm by instrumented replay which of the two links (the non-sticky
  decline vs the ask cache) would break the loop first if fixed alone.
- **I did not read the 1.23 GB stderr in full** — only the head, the transition region
  (lines ~1,985–2,175), a mid-file sample, the tail, and `grep -c` over the whole file.
- **I did not adjudicate the replacement run** `matchups-20260907-065922` (in flight).
- **I did not verify card facts against primitives or Scryfall** — no defect in this report turns
  on a card's rules text; the two card-shaped items (Hengegate Pathway as an MDFC land, Idyllic
  Tutor's search) are engine-behaviour items, not card-script items.
- **I did not re-read wave66..69 `lane-*.md` in full**, only wave 70's four lanes' Predictions and
  "did not verify" sections plus wave69's `known-bugs.md` head. Mechanisms named in the brief but
  absent from both corpora's fields (`label_missing_reask`, `noop_plan`, `attack_*`/`blocks_*`
  re-asks) are reported as string-absent, not as source-verified deletions.
- **`reasoning_content` was read selectively**, not corpus-wide: the mulligan exemplar, the 19
  budget-hit records, and the degenerate outlier. A systematic read of 2,119 reasoning traces for
  perception failures is the per-deck seats' work, not this one's.
- **`plan_steps_done`** is named in the brief but appears on no record in either corpus; I could
  not measure plan-step advance vs execution, only PLAN-line step *counts*.
