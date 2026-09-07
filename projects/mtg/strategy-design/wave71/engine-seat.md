# Wave 71 — engine-narration seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-085638` (binary `b31f66cf3`, `--thinking on`,
`WAGIC_GPT_TIMEOUT=900`, no operator token ceiling). Baseline for every ratio: wave 70's
`matchups-20260906-224849` + `wave70/engine-seat.md`. Read-only: no build, no run, no git, nothing
written outside `wave71/`.

**What I did NOT check:** no deck-guide reading and no strategy verdicts (deck seats own those); no
per-game narrative tracing beyond the decisions cited; card facts verified against
`bin/Res/sets/primitives/mtg.txt` only where a verdict depended on them (Teferi's Puzzle Box,
Tribute to Hunger) — no Scryfall calls were made this session; PARSETEST/suite numbers are quoted
from the lane files, not re-run.

---

## 1. Corpus census

| | wave 71 | wave 70 |
|---|---|---|
| seats / games | 42 seats, 21 games | 42 seats, 21 games |
| completed naturally | 42 gameend records (21 games, `results.tsv` has no `hang` row) | 40 gameend, **1 HUNG** |
| crashes / harness timeouts | 0 / 0 | 0 / 0 |
| model decisions (records carrying a `prompt`) | **2,159** | 2,119 |
| decisions by kind | ask 1,606 · priority 376 · attackers 81 · discard 50 · blockers 30 · reveal 12 · bottom 4 | ask 1,523 · priority 411 · attackers 78 · discard 67 · blockers 20 · reveal 20 · recovery 3 |
| fallbacks | **4**, all `noop_row_zero_reask` (0.19%) | 9 (0.42%): 6 noop, 2 `unparsed_reply`, 1 `http_error` |
| `transport` | `curl=0,http=200,empty=0` on 2,159/2,159 | one `http=400,empty=1` |
| `async_drops` | 17 (8 question, 6 question+board, 2 land-drop, 1 board — all `re-asked`) | 33 |
| `deadline_pct` | p50 8.8 · p95 20.9 · max 52.8 | p50 8.9 · p95 20.6 · max 52.9 |
| latency | p50 79.3 s · p95 188.6 s · max 479.2 s | p50 80.7 s · p95 185.6 s · max 476.6 s |
| HOLD rows rendered / taken | 1,352 / **253** (ask 976/119, priority 376/134) | 1,353 / 386 (ask 942/288, priority 411/98) |
| `hold_windows_skipped` (cast / priority) | 1,777 (**774** / 1,003) | 3,218 (2,640 / 578) |
| `mana_only_windows_skipped` | 270 | 260 |
| game length (turns) | mean 20.8, median 15, max 70 | mean 22.1, median 16, max 58 |

**Reply shape.** `protocol_replies` 2,159. Exactly-PLAN+action = 2,159 − 90 `plan_line_missing`
(4.2%) with 83 records at `off_protocol_bytes > 0` (3.8%; wave 70 4.9%, 103/2,119). The two classes
overlap: of the 90 missing-PLAN records, **83 also carry off-protocol bytes (class A/B: a mislabelled
plan line) and 7 carry `off_protocol_bytes == 0`** — the class G that was byte-invisible before
lane BO's L10 (0.32% of the corpus). `plan_line_missing` by seam: ask 59, attackers 11, priority 10,
discard 7, blockers 2, reveal 1. Off-protocol byte weight: n=83, sum 5,475 B, median 62, p95 125,
max 294 (wave 70: n=103, sum 6,212, median 56, max 289) — the shape is unchanged, the count fell.
`action_before_plan_replies` **0** (both waves). `answer_replaced` 0; `coded_answers` 1 on every
record.

**Reasoning.** 2,159/2,159 records carry native `reasoning` (`reasoning_chars` min **1,229**, p25
4,413, p50 5,561, p75 7,140, p95 10,313, max 20,925, mean 5,997; wave 70 min 330, p50 5,883, mean
6,308). `reasoning_degenerate` (a repetition RATIO, not a flag) p50 0.003, max 0.014 — nothing at or
near degeneracy (wave 70 max 0.025). Invariant 000 held: reasoning ON, native channel, 100% coverage.

**Rescue markers, after lane BO L9.** `reasoning_forced_close` **3** = `reasoning_budget_hit` **3** =
records with `max_tokens_reasoning == 0` **3** = `phase2_answer_recovered` 3 + `phase2_answer_missing`
0. All three identities hold exactly (wave 70: 19 stamps against 4 real closes). The three are the
answer-locked retries (`retry: 1`) at `126v152` seq 20 (blockers), `126v162` seq 8 and `126v125`
seq 13 (ask), each issued with `max_tokens 400`.

**Ceilings.** `max_tokens_answer` observed: ask 288 (1,604) / 400 (2 retries); priority 288 (376);
attackers 256 (81); discard 256 (50); blockers 256 (29) / 400 (1 retry); reveal 256 (12); bottom 512
(4, unfitted as designed). **`reply_truncated`: 0 records at any seam** — nothing refutes lane BO's
288/256 fit.

**Cardinality floor (BS F5).** Largest declarations offered: reveal 50 items, blockers 29, attackers
28, discard 22, bottom 7. `gptDeclarationAnswerFloorTokens` at 50 PUT slots is 182 tokens and at 29
block pairs 197 — both under the 256 floor, so **the floor never raised a ceiling in this corpus**
and the doubled-allowance re-ask never fired (no `finish_reason=length` at a declaration seam).
`dropped_assignments` 0 on all 30 blockers records.

**Re-asks and replays.** `identical_ask_answers_reserved` 68, `identical_option_asks_resolved` 3,
`repeat_annotated_takes` 3, `reserve_decline_windows_skipped` 3, `engine_reveal_floor_picks` 0,
`put_gloss_stripped` 0, `wall_miss_*` 0, `declined_face_latches` **0**, `menu_pass_no_progress` **0**,
`ask_replays_refused` **0**, `ask_replays_reserved` **725**.

**`plan_steps_done` advance.** Non-zero on at least one record of **42 of 42 seats**, and 42 of 42
seats wrote a multi-step plan. Distribution of `(done, count)`: (0,1) 969, (1,2) 763, (1,3) 304,
(1,4) 74, (0,0) 16, (1,5) 16, (2,3) 7, (1,6) 4, (2,4) 2. The pointer advances, but it reaches 2 on
only 9 records — a plan is re-stated far more often than it is walked.

**`ask_replay` sidecar — NOT empty; the harness never harvested it.** See HIGH-1.

---

## 2. REGRESSION WATCH — wave 71's shared-path changes

### BQ L4 — own-turn instant windows in the shared heuristic path
**No stall, no ping-pong, cost priced.** Decisions rose 2,119 → 2,159 while turns fell 940 → 899
summed over 42 seats: **2.40 decisions/turn vs 2.25** (+6.7%); per-seat median 1.92 → 2.15 (+12%).
Latency p50/p95 unchanged, so the extra windows cost inference time in proportion, not
disproportion. Games completed 21/21 with no hang; median game length fell 16 → 15 turns.

Where the volume went (own-turn, non-main, `ask` seam): Upkeep 93 → 122, End 5 → 39, Draw 15 → 26,
Blockers 44 → 46, Combat damage 0 → 5; own-turn non-main `priority` windows FELL 236 → 181.
**Every card offered at a new window is instant-speed or flash** (Devour Flesh, Tribute to Hunger,
Vanishing Verse, Soul Shatter, Path to Exile, Starstorm, Sphinx's Revelation, Tragic Slip, Spark
Spray, Vision Skeins, Fateful Absence, Dictate of Kruphix) — no sorcery-speed leak.

**Priority ping-pong / double-ask: none.** Consecutive windows identical in (kind, turn, phase,
option list) and *actually asked of the model*: **7 pairs in wave 71 vs 6 in wave 70**, every run of
length exactly 2. No run of 3+, no seat alternating with its opponent.

Outlier worth naming: `1788789439-ai_baka_deck123-…-vs-…deck125` at **5.69 decisions/turn**
(279 decisions over 49 turns) — the highest in either corpus (wave 70 max 3.91). Same seat holds 284
of the 725 ask replays.

### BP pass-floor no-progress arm and cache refusal
Both **UNTESTED, correctly**: `menu_pass_no_progress` 0 and `ask_replays_refused` 0 across 42 seats,
and no forced pass ever auto-answered a legal decision (no `menu pass floor fired` line in any
stderr). The refusal's own threshold was approached but not reached — the longest replay run is
**38** against `kAskReplayRefuseMax = 64`, in
`askreplay/1788789439-ai_baka_deck123-0x5629c69e3090-…jsonl` turn 47. In that seat the run length
grows monotonically with the turn count (11, 12, 14, 16, 18, 21, 23, 25, 27, 29, 34, 38 at turns
14…47), i.e. the wave-70 drain-loop shape is still there and is O(turn); a game ~15 turns longer
would have crossed 64 and exercised the refusal. Reported, not a defect.

### BS F3 land latch
**UNTESTED — the trigger never occurred.** 63 face/mode menus were served (31 of them MDFC pathway
menus: Brightclimb 9, Hengegate 8, Barkchannel 7, Branchloft 7) and **not one was answered with
"Decline - do nothing"**: exactly one `Decline` pick exists in the corpus, and it is a cast-cancel at
`125v123` seq 169, not a face menu. Consistent with `declined_face_latches == 0`. No land drop was
suppressed: every `land-drop ask NOT issued` line in every stderr gives the pre-existing reason
("N land(s) in hand, none playable right now (restriction or play-restriction)"), 960 lines vs
wave 70's 1,057, and none names the latch. No land offered in main 2 after a main-1 decline, because
there was no main-1 decline.

### BO's restored / narrowed re-asks
`distinct_index_reask` (BS F6): **0 fires** — the shape (a `PUT:` line repeating an index on a
multi-card discard) did not recur; 50 discard windows, max 22 items. The doubled-allowance re-ask
(BS F5): **0 fires** (no truncation to trigger it). Both remain untested paths.

---

## 3. Prediction adjudication

### lane BO
| # | prediction | verdict | evidence |
|---|---|---|---|
| 1 | fallback rate ≤ 0.42%, kinds ⊆ {noop_row_zero_reask, unparsed_reply, http_error, named_row, no_pass, plan_missing} | **PASS** | 4/2,159 = 0.19%; all four `noop_row_zero_reask` |
| 2 | `off_protocol_bytes > 0` stays 4.9% ± 1.5 pp | **PASS** | 83/2,159 = 3.84% (−1.02 pp) |
| 3 | ceilings do not bite; any `reply_truncated: 1` at ask/priority refutes the 288 fit | **PASS** | 0 `reply_truncated` records; max seam ceiling honoured on 2,156 records |
| 4 | `reasoning_forced_close` == records with `max_tokens_reasoning == 0`, and `phase2_answer_recovered + _missing` == that count | **PASS** (3 = 3 = 3 + 0) | the three `retry: 1` records, §1 |
| 5 | `plan_line_missing` ≈ 5%, its `off_protocol==0` subset nameable; `plan_steps_done` non-zero on some record of every multi-step-plan game | **PASS** | 90/2,159 = 4.2%; class G = 7 records (0.32%); 42/42 seats advance the pointer |

### lane BP
| # | prediction | verdict | evidence |
|---|---|---|---|
| 1 | zero HUNG games; no stderr near 100 MB | **PASS** | `results.tsv` 21 rows, no `hang`; total stderr 2.75 MB, **largest single file 557 KB** (`game-125v123-1788789436.stderr`) against wave 70's 1.23 GB |
| 2 | `ask_replays_reserved` non-zero and per-window visible; `ask_replays_refused` 0 | **PASS on the counters, FAIL on delivery** | 725 reserved / 0 refused in the gameend census; the per-window records exist but are not in the corpus — HIGH-1 |
| 3 | `declined_face_latches` small and non-zero; `menu_pass_no_progress` 0 | **UNTESTED / PASS** | latches 0 because zero declines occurred (§2), not because the latch failed; `menu_pass_no_progress` 0 |

### lane BQ
| # | prediction | verdict | evidence |
|---|---|---|---|
| L3 | `noop_row_zero_reask` fires 0 times on a row with a non-empty victim tag or a conditional-only "does nothing"; the re-ask never argues against a printed kill; no row refused in one window and accepted unchanged in the next | **PASS** | all 4 fires are on rows with NO `{kills:`/`{removes:` tag and no conditional (quoted in §4 HIGH-2). 41 Tragic Slip + 4 Tribute rows that fired in wave 70 fired 0 times here. The `131/132/133` signature did not recur |
| L4 | in any game where an instant prints `[castable now]`, at least one own-turn **priority** window outside a main phase renders a `Cast <that instant>` row; the 23-of-23 shape does not recur | **PASS on substance, FAIL as worded** | grouping every (seat, turn, non-main own-turn phase, castable-now instant): **209 of 243 groups get a Cast row — Upkeep 112/112, Draw 14/14, Blockers 14/14, End 65/66, Combat damage 4/5, Attackers 0/14 (excluded by design), Cleanup 0/18**. But the row is rendered at a *sibling `ask`* window, never at a `priority` one: **0 of 376 `priority` windows in the corpus carry a Cast row.** Repro of the pair: `1788789427-…deck130-…-vs-…deck125` seq 24 (ask, Upkeep, `Cast Starstorm {r}{r}{x} {X pricing: max affordable X=4 …}`) and seq 25 (priority, same turn/phase, rows = `cycling with Spark Spray` / `cycling with Starstorm` / hold only) |
| L5 | of the modal cast-mode menus that follow a model-answered `Cast X`, `Cast Card Normally` is taken in a clear majority | **PASS** | 11 cast-mode menus, **11 carry the commitment note**, **10 took `Cast Card Normally`** (91%) vs base 4 of 21 (19%). The one exception (`…deck130-…vs-…deck126` seq 39, cycling with Spark Spray) had the note in its prompt, so it is separable from a misread |
| L12 | 0 reveal records whose reply carries `BLOCKS:` or `ATTACK:` | **PASS, weakly** | 0 of 12 reveal records (base 1 of 20). N=12 cannot separate the fix from the shrunken seam |

### lane BR
| item | prediction | verdict | evidence |
|---|---|---|---|
| L6 | zero games where two edicts of the Tribute family credit one death twice | **UNTESTED** | 4 Tribute to Hunger casts total, never two in one window. The single-cast payment is now correct: `126v162` seq 14→15, `Shield Sphere (0/6) is sacrificed, you gain 6`, life 20 → 26 with "Opponent's Shield Sphere died" in the log |
| L7 | a control seat no longer reads `destroys 0` as an empty board; the animatable count is non-zero in every window that lists one | **PASS** | the tail `… N noncreature permanent(s) … can animate into a creature and is not in that count` renders **430 times** across headers, sweeper rows and Lightmine rows. But see MED-1: it now feeds a false read of its own |
| L8 | `162v152` seq 21's shape (a `{reserve:}` verdict taken with a Puzzle Box trigger on the stack) does not recur without the seat being told | **FAIL** | the shape recurred and the seat was NOT told — see HIGH-3 |
| L11 | `YOUR PLAN:` openings go to 0; class A falls but does not vanish | **PASS** | **0 prompts in the whole corpus contain `YOUR PLAN:`** (the heading is now `YOUR PLAN (as you last stated it, N windows ago on turn T) reads "…"`); mislabelled-plan records 103 → 83 |
| L13 | no reasoning trace spends itself on "the log says it already resolved?" | **UNTESTED** | not measurable from the record set without reading 2,159 reasoning bodies; I did not do that and will not claim it |
| L14 | no animation plan written at a window whose sources cannot pay | **UNTESTED** | the permanent-side `manlandBoardTag` renders (e.g. `…deck146-…vs-…deck125` seq 65/98 carry `{rung ceiling: Hive of the Eye Tyrant's OWN mana is not counted toward …}`); whether a plan was written against an unpayable rung is a reasoning-body claim I did not measure |
| L15 | zero bare mode rows; no invented mode content | **PASS on the render half** | **0 of 306 menu/mode rows render without an annotation.** The invented-content half is a reasoning claim, UNTESTED |
| L16 | no `[DRAW PRICE]` on a cast row for a card that draws only from an activation | **PASS** | 51 `[DRAW PRICE]` rows; all 13 Pyrite Spellbomb ones are on `Draw 1 with Pyrite Spellbomb` **activation** rows; no `Cast Pyrite Spellbomb` row carries the tag |
| L17 | `hold_windows_skipped_cast` falls sharply; a token loop costs ~2 decisions, not 7 | **PASS, both halves** | cast-seam hold skips 2,640 → **774** (−71%), cast-seam hold takes 288 → 119 (30.6% → 12.2% of cast menus). Token loop: `123v152` seq 26 (one-at-a-time), seq 27 offers the batch row after **one** activation and the model answers `repeat_n = 26`, seq 28 shows 29 creatures and passes — **3 decisions** where wave 70's `123v130` seq 18→23/24 took 5 windows before the row appeared |
| L18a | the summed crack-back line | **UNTESTED** | no window in this corpus reproduced the `125v152` seq 38 shape with an unsizeable contributor; I did not find a case to adjudicate |
| L18c | `HOLD PRIORITY:` label-shaped row head (docketed, not fixed) | **no recurrence** | 0 of 2,159 replies used `HOLD PRIORITY:` as a label (base 1). The head is still `Hold priority: …` on all 1,352 rendered hold rows, so the cause is live and the docket stands |
| L19 | disjoint tap groups stop tripping the F1 diagnostic | **PASS** | `W69-BJ F1` in stderr: **29 (wave 70) → 0 (wave 71)**, and DebugTrace is demonstrably live in this build (574 `AIPlayerGPT: chose to cast nothing` lines in one stderr) |

### lane BS
| # | prediction | verdict | evidence |
|---|---|---|---|
| F1 | a game with Proper Burial / Grim Feast / Death Watch shows life move on each death | **UNTESTED** | none of the three cards appears in any seat log; the deck pool does not contain them |
| F2 | two edicts in one turn kill two bodies and pay both toughnesses | **UNTESTED** | no turn cast two edicts (see L6). The new floor render *is* live: `126v146` seq 37 `they control 3 creatures - they choose which one - you gain at least 1 (their smallest toughness …)` |
| F3 | `declined_face_latches` per game rises; a land-drop window after a board change re-offers the declined pathway | **UNTESTED** | 0 declines in 31 pathway menus (§2) |
| F4 | a wave-70-shaped spin trips the fast arm at ~64 MB; the sidecar holds the replay run | **PASS on the sidecar's content, FAIL on its location in the corpus** | HIGH-1. No spin occurred, so the fast arm is UNTESTED |
| F5 | wide-board blockers windows stop producing partial declarations; a ceiling re-ask appears only on genuine truncation | **UNTESTED** | the floor never exceeded the fitted ceiling (largest window 50 reveal slots → 182 tokens < 256); 0 truncations, 0 re-asks, `dropped_assignments` 0/30 |
| F6 | (restored path) | **UNTESTED** | 0 `distinct_index_reask` fires |
| F7 | the Puzzle Box warning names the right hand | **FAIL** | HIGH-3 |
| F8 | damage modes no longer declared incapable of changing life | **PASS, weakly** | 0 of 306 mode rows carry the "changes no life total and draws no cards" negative over a damage mode; but no Jeskai-Charm-shaped card was cast, so this is absence-of-the-defect, not a positive |
| F9 | branch-aware `rowSaysNoOp` | **PASS** | the Devour Flesh shape (`…; YOU control 0 creatures - targeting yourself does nothing`) rendered 289 times and was **never** refused; `123v152` seq 11 took it and the game proceeded to the target menu at seq 12 |

### Carried items
- **Idyllic Tutor one-window library lag (wave 70 LOW-2):** **UNTESTED.** 6 Idyllic Tutor takes in
  the corpus; I did not trace the library/hand counts across the following window for any of them.
- **`HOLD PRIORITY:` row head:** see L18c above — cause live, effect did not recur.
- **Dungeon room header (lane BR L15 "ALSO FOUND"):** **still open.** 3,312 dungeon lines render;
  the room-level header path (`dungeonRoomHeader`, N-146n) is not visible in any prompt. The mode
  rows themselves are now annotated (`146v125` seq 78, `goblin lair (room 2 of 7 in Lost Mine of
  Phandelver's printed room list) {room effect: …}`), so the annotation gap L15 fixed is closed and
  only the header remains.

---

## 4. Engine / render / narration defects

### HIGH-1 — the `ask_replay` sidecar is written outside the corpus and the harness never harvests it
The 725 replay records **exist**: `~/.Wagic/ai/gpt/logs/askreplay/`, 29 files, 608 KB, all from this
run (`why`: `cache_replay` 657, `repeat_ask_reserved` 68 — matching `ask_replays_reserved` 725
exactly). They are not in `matchups-20260907-085638/` because
`AIPlayerGPT::askReplaySidecarPath()` (`src/AIPlayerGPT.cpp:16334`) derives the directory from the
**live** `mTransLogPath` (`$HOME/.Wagic/ai/gpt/logs/`), while the harness harvests with
`comm -13 "$BEFORE_LIST" <(ls "$LOGDIR"/*.jsonl) | while read -r f; do cp "$f" "$OUTDIR/"; done`
(`tools/selfplay-harness.sh:610`) — a non-recursive top-level glob. The engine half of F4 is
correct (a subdirectory is invisible to the sweep's own non-recursive glob, which is the point);
the harness half was not written. Consequences: the corpus is missing its loop evidence, the
sidecars accumulate across runs in the shared log dir (they are not snapshotted by `$BEFORE_LIST`),
and a future reviewer reading only the corpus concludes "0 re-serves" — the false conclusion this
wave's own CORPUS line reached. Fix is one line in the harvest block (`cp -r "$LOGDIR/askreplay"`
restricted to this run's basenames).

### HIGH-2 — `noop_row_zero_reask` fires on permanents whose zero verdict is prospective (3 of 4 fires)
All four fallbacks, quoted from `options_text`:

1. `125v152` **seq 32**, turn 18 Main 1 — reply `CHOICE: 1 (Cast Lightmine Field)`:
   `Cast Lightmine Field {2}{w}{w} {right now: they control 0 creatures able to attack - deals 0 until they have an attacker - 1 noncreature permanent of theirs can animate into a creature and is not in that count} …`
   **FALSE FIRE.** Lightmine Field is an enchantment; "deals 0 **until** they have an attacker" is a
   statement about a future trigger, not about the cast. Re-ask consumed; the model switched to
   `Cast Staff of Nin` at seq 33 (`parse_note: plan_contradicts_noop_row_recovered`).
2. `125v152` **seq 64**, turn 24 Main 2 — reply `CHOICE: 2 (Cast Supreme Verdict)`:
   `Cast Supreme Verdict {1}{u}{w}{w} {right now: destroys 0 of their creatures (0 without a restriction against attacking), 0 of yours - 1 noncreature permanent of theirs can animate into a creature and is not in that count} …`
   **GENUINE.** A one-shot sweeper resolving into an empty board really does nothing. Outcome
   correct: seq 65 `Cast nothing right now`.
3. `123v152` **seq 10**, turn 4 Main 2 — reply `CHOICE: 2 (Cast Lightning Greaves)`:
   `Cast Lightning Greaves {2} {right now: you control 0 creatures - this equips nothing} …`
   **FALSE FIRE.** An Equipment cast with no creature out is the standard set-up play, and the
   model's own PLAN said so ("Cast Lightning Greaves now, then next turn cast Thraben Doomsayer,
   equip Greaves"). The re-ask line read
   `[RE-ASK] You chose row 2 ("Cast Lightning Greaves {2}"), whose own note on this list says it does nothing right now.`
   and the model abandoned the plan (seq 11 `Cast Devour Flesh`).
4. `125v123` **seq 7**, turn 7 Main 1 — reply `CHOICE: 1 (Cast Lightmine Field)`:
   `Cast Lightmine Field {2}{w}{w} {right now: they control 0 creatures able to attack - deals 0 until they have an attacker} …`
   **FALSE FIRE.** Same class as (1). The model re-took the row at seq 8 and was allowed through
   (`parse_note: noop_row_retaken`) — one wasted round trip.

The class is not "Morbid/quoted/conditional" (BQ L3's three rules, all of which held): it is **a
PERMANENT whose zero is about a downstream trigger or attachment**. `verdictReadsZero` fires on
"deals 0 …", "this equips nothing", "destroys 0 …" without asking whether the row puts a lasting
object on the battlefield. Exposure: 759 rendered cast rows carry a zero verdict, of which **54 are
permanents** (Lightmine Field 32, Lightning Greaves 22) and 705 are one-shot spells (Devour Flesh
289, Tribute to Hunger 249, Soul Shatter 70, Supreme Verdict 52, Damnation 45). The same falsehood
rides the menu header — `NO LIVE CAST ROW ON THIS MENU: … not one of them changes a number on the
board as it stands` — on 292 windows. Suggested separator: a row whose card type is a permanent
never earns the zero verdict from a trigger/attachment clause, only from a clause about the
permanent failing to enter.

### HIGH-3 — the Puzzle Box hand-replacement line still does not fire (lane BR L8 / lane BS F7)
`123v162` seq **43, 44, 45**, turn 12, the seat's own Draw step. The stack block renders:
```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): ability: Howling Mine's Draw 1 [from their Howling Mine] [triggered/activated ability]
  2: ability: Teferi's Puzzle Box's Ability [from their Teferi's Puzzle Box] [triggered/activated ability]
  3: ability: Fate Unraveler's deal 1 damage [from their Fate Unraveler] [triggered/activated ability]
ON THE STACK: 1 damage to you - you would be at 1
```
and **no `EVERY COUNT TAKEN FROM YOUR HAND ON THIS SCREEN IS ABOUT TO BE VOID` line follows**, though
the call site (`AIPlayerGPT.cpp:24299`) sits directly under that block. `mtg.txt:118332-118335`
confirms the shape: `auto=@each opponent draw:name(recycle draw) count(type:*:opponenthand) && bottomoflibrary all(*|opponenthand) && draw:countedamount opponent` on a Puzzle Box the OPPONENT
controls — so the hand about to be bottomed is this seat's, and the trigger was live on the stack.
`stackHandReplacerFor` (`:4636`) returned empty, i.e. `abilityEmptiesHandOf` (`:4615`) found no node
whose `getActionTc()` is a `TargetZoneChooser` over the seat's hand while walking `NestedAbility` /
`MultiAbility` from `StackAbility::ability`. This is BS's own weakest-evidence #1 ("no fixture and
no PARSETEST pin … source-reasoned and suite-green, nothing more") coming back FAIL on its first
live shape. Whether the `bottomoflibrary all(*|opponenthand)` lord is reachable through those two
casts is the thing to check; the shape gate above it also still reads `src->magicText` alone, the
exact narrowness lane BR fixed elsewhere (L15) with `scriptAllZones`.

### MED-1 — the animatable-land tail (L7) reads as "the sweeper answers it"
The tail is factually correct and it is doing work (430 renders), but at `125v152` seq 64 it sat
inside a Supreme Verdict row whose own verdict was `destroys 0 of their creatures … 0 of yours` and
the model's reply was `PLAN: Cast Supreme Verdict to remove the impending Lair of the Hydra threat`.
The seat read "1 noncreature permanent of theirs can animate into a creature and is not in that
count" as a reason the sweeper was worth casting, when a Verdict resolving now cannot touch an
un-animated land. The tail says what is NOT in the count; it does not say the row cannot reach it.
One clause on the sweeper row ("this resolves before any animation, so it destroys none of them")
would close it. This is the only case in the corpus where a wave-71 render addition appears to have
steered a decision the wrong way.

### MED-2 — the ask-replay drain loop grows with the turn count
`askreplay/1788789439-ai_baka_deck123-0x5629c69e3090-…jsonl`: consecutive silent re-serves of the
same casting window run 11, 12, 14, 16, 18, 21, 23, 25, 27, 29, 34, **38** at turns 14 → 47, all
`why: cache_replay` on `Casting decision (Upkeep, opponent's turn)`. That seat spent 279 decisions
over 49 turns (5.69/turn, the corpus maximum) with 284 replays behind them. Nothing broke — the
refusal is at 64 — but the growth is monotone in turn count, so the bound is the game ending, not
the mechanism. Worth a per-turn cap or a fingerprint that includes the tick count.

### LOW-1 — own-turn Cleanup windows are outside L4's admitted set
18 (seat, turn, Cleanup) groups had a `[castable now]` instant in hand and no cast row. Under the
rules a player usually gets no priority in the cleanup step, so this is probably correct behaviour
rather than a gap — but L4's admitted list does not mention Cleanup either way, and the seat *is*
being asked its `discard` there (50 windows), so the asymmetry should be written down.

### LOW-2 — `plan_steps_done` reaches 2 on 9 records of 2,159
42 of 42 seats advance the pointer at least once, so BN's F10 mechanism fires; but 969 records carry
`(0, 1)` — a one-step plan with nothing done — and only 9 ever show `done >= 2`. The instrument is
now visible (lane BO L10 delivered that); what it shows is that the model re-states rather than
walks. Not an engine defect; a measurement for whoever owns the protocol.

---

## 5. Lategame specimen

`wave71/lategame-specimen.txt` —
`1788789421-ai_baka_deck146-0x55d62b5dc0b0-vs-ai_baka_deck125.jsonl` seq 74, turn 27, phase Blockers,
17 vs 21 life, 7 options, 54.2 s, 5,520 reasoning chars. The seat chose option 6, `Final Judgment
[sorcery] [opponent's graveyard]`, with `PLAN: Exile Final Judgment to remove a board wipe, then
proceed to combat.` — and won the game 13 to −3 on turn 34. Full `prompt` verbatim (15,803 B).
