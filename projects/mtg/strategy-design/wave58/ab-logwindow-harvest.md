# Log-window A/B harvest (owner ruling D43 'measure'; three sequential arms 2026-09-03 19:54 -> 09-04 01:50; harvested by Codex gpt-5.6-sol read-only from the translogs; numbers unverified by a second reader)

# Log-window A/B harvest

I parsed all **126 JSONL files**—42 per arm—with Python 3, treating records containing `prompt` as model decisions. The harness’s broader “decisions” totals include `gamestart`, `gameend`, and `recovery` records; both totals are shown below. Inputs were the [harvest specification](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave57/lane-H.md), [launch script](/home/magi/.gatelogs/ab-logwindow-launch.sh), [arm markers](/home/magi/.gatelogs/ab-logwindow.log), driver logs, JSONL, `results.tsv`, and all 63 game stderr files. No files were written.

## 1. Arm identity and health

| Arm | Record-verified setting | Contradictions | Prompt decisions / all records | Natural games | SEGV / abnormal | Wall time |
|---|---:|---:|---:|---:|---:|---:|
| `unset` | `full` on **2,532 of 2,532** | **0 of 2,532** | **2,532 / 2,632** | **21 of 21** | **0 / 0** | **1:49:38** |
| `kind:3` | `kind:3` on **3,194 of 3,194** | **0 of 3,194** | **3,194 / 3,281** | **21 of 21** | **0 / 0** | **2:12:41** |
| `4` | `4` on **2,681 of 2,681** | **0 of 2,681** | **2,681 / 2,775** | **20 of 21** | **0 / 1** | **1:53:40** |

The abnormal `4` game was deck146 versus deck152: `rc=134`, SIGABRT after a C++ vector-bounds assertion, with a core dump. It was **not a SEGV**. All arm drivers exited 0; the harness recorded the crash explicitly and awarded neither deck a win. There were **0 game-cap timeouts/draws in every arm**.

`log_window_kind` remained mostly `unclassified`: **1,664/2,532**, **2,108/3,194**, and **1,740/2,681**, respectively. Therefore the actual per-record elision field, not this label, is used below.

## 2. Decisions, fallbacks, stale drops, and timeouts

| Arm | Heuristic-class handoffs | All fallback-stamped records | Stale async drops | `fallback:"timeout"` | Other deadline failures |
|---|---:|---:|---:|---:|---:|
| `unset` | **14 of 2,532** | **22 of 2,532** | **78**: casting 50, land-drop 28 | **0 of 2,532** | **0** |
| `kind:3` | **3 of 3,194** | **3 of 3,194** | **58**: casting 34, land-drop 24 | **0 of 3,194** | **0** |
| `4` | **9 of 2,681** | **13 of 2,681** | **58**: casting 25, land-drop 33 | **0 of 2,681** | **2** `wall_miss_unrecorded` at approximately 900 seconds |

“Heuristic-class” excludes records explicitly initiating one more model re-ask. Breakdown:

- `unset`: 13 `unparsed_reply`, 1 `stale_echo`; additionally 5 `named_row_reask` and 3 `no_pass_reask`.
- `kind:3`: 3 `unparsed_reply`.
- `4`: 3 `unparsed_reply`, 2 `wall_miss_unrecorded`, and one each of `empty_reply`, `stale_echo`, `retracted_choice`, and `all_assignments_illegal`; additionally 4 `named_row_reask`.

The `4` arm’s one `empty_reply` lasted 2.326 seconds and was not marked as a timeout. No arm recorded `abandoned_in_flight`.

## 3. Prompt composition and actual elision

No prompt-token field exists, so sizes are Unicode character counts.

| Arm | Prompt chars mean / median / max | GAME LOG chars / all prompt chars | Narration share | Turns elided: 0 / 1–5 / 6+ |
|---|---:|---:|---:|---:|
| `unset` | **11,284.2 / 10,084.5 / 31,703** | **17,601,770 / 28,571,531** | **61.6%** | **2,532 / 0 / 0** |
| `kind:3` | **12,590.7 / 10,216.5 / 32,436** | **25,253,320 / 40,214,569** | **62.8%** | **2,331 / 140 / 723** |
| `4` | **6,449.6 / 6,317 / 15,696** | **5,620,991 / 17,291,346** | **32.5%** | **224 / 424 / 2,033** |

The elision field was present on **0/2,532**, **863/3,194**, and **2,457/2,681** decisions; absence means zero by the instrument contract. Maximum turns elided were **0**, **56**, and **58**. Thus `kind:3` engaged, but its aggregate corpus prompt size and narration share did not fall below control; the all-kind `4` arm did.

## 4. Inference time

| Arm | Latency observations | Mean | Median |
|---|---:|---:|---:|
| `unset` | **2,532 of 2,532** | **6.243 s** | **3.659 s** |
| `kind:3` | **3,194 of 3,194** | **6.477 s** | **3.604 s** |
| `4` | **2,681 of 2,681** | **7.095 s** | **4.300 s** |

The `4` mean includes two approximately 900-second wall misses. These arms ran sequentially against one Spark at `-j 3`; server load is not controlled across time, so latency differences cannot be attributed solely to the window.

## 5. Deck wins and the win-rate band

Each entry is wins/games; every six-game estimate has **SE ≈ ±10.9 percentage points**.

| Deck | `unset` | `kind:3` | `4` |
|---|---:|---:|---:|
| 123 | **2/6 (33.3%)** | **2/6 (33.3%)** | **3/6 (50.0%)** |
| 125 | **2/6 (33.3%)** | **3/6 (50.0%)** | **4/6 (66.7%)** |
| 126 | **3/6 (50.0%)** | **1/6 (16.7%)** | **2/6 (33.3%)** |
| 130 | **0/6 (0.0%)** | **3/6 (50.0%)** | **1/6 (16.7%)** |
| 146 | **5/6 (83.3%)** | **3/6 (50.0%)** | **4/6 (66.7%)** |
| 152 | **4/6 (66.7%)** | **4/6 (66.7%)** | **3/6 (50.0%)** |
| 162 | **5/6 (83.3%)** | **5/6 (83.3%)** | **3/6 (50.0%)** |

Numerically, every deck’s max–min difference exceeds 10.9 points; the largest is deck130 at **50 points**. However, one game is already **16.7 points**, and the lane’s approximate two-arm resolution threshold is about **30 points**. On that stronger guardrail, decks 125, 126, 130, 146, and 162 exceed it; decks 123 and 152 do not. These unpaired six-game samples do not establish that an arm improved play.

## 6. Other recorded differences

| Measure | `unset` | `kind:3` | `4` |
|---|---:|---:|---:|
| Exact HOLD takes / rendered HOLD rows | **337/1,796** | **396/2,423** | **344/1,853** |
| HOLD re-open stderr events | **166** | **134** | **156** |
| Decisions with `plan_echo_count > 0` | **1,061/2,532** | **1,456/3,194** | **1,266/2,681** |
| Maximum plan-echo counter | **92** | **88** | **227** |
| Explicit re-ask records | **8/2,532** | **0/3,194** | **4/2,681** |
| Answer-locked `retry` records | **2/2,532** | **0/3,194** | **0/2,681** |

HOLD-take proportions are similar (**18.8%, 16.3%, 18.6%**). Re-asks are rare in every arm. Plan-echo record shares differ modestly, while the `4` maximum is a single long-streak extreme. With events clustered by game and different game lengths, these secondary counts do **not differ beyond noise** in a way attributable to elision.

## Plain-language summary

The measurement clearly shows that a four-turn window greatly reduces prompt size and narration share, while `kind:3` only elides part of the traffic and did not reduce the aggregate prompt corpus in these particular games. It does **not** show whether either form of elision helps or hurts decision quality: only 21 unpaired games ran per arm, each deck played six times, one `4` game crashed, and ordinary deal and matchup variance moves a deck by 16.7 points per game. The win shifts, fallback counts, HOLD behavior, plan echoes, and re-asks are therefore guardrails and follow-up signals, not evidence that one setting makes better decisions.