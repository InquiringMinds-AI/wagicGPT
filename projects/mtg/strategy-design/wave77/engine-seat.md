# Wave 77 — engine-narration seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-010157-final`, **SINGLE BINARY**
`archives/wagic-42c41fc66-w77step1` (source `8e394a077`, lanes CR+CS+CT+CU). 42 seat logs /
21 games / 990 turns, `--thinking on`, `WAGIC_GPT_TIMEOUT=900`, no operator token ceiling.
**One `reply_protocol_hash` (`3871ee53c96bb848`) on all 42 seats — the single-binary claim is
verified from the data, not the launch line.** 20 of the 21 games are reruns after pilot wedge
#3; the first-run game is `126v162` (epoch 1789106557). Read-only: no build, no run, no git;
nothing written outside `wave77/engine-seat.md` and `wave77/lategame-specimen.txt`.

**What I did NOT check.** No deck-guide reading, no strategy verdicts (deck seats own those).
No Scryfall/Gatherer calls — card facts come from the rendered prompt and, where a mechanism
mattered, from `src/AIPlayerGPT.cpp`. PARSETEST/suite numbers are quoted from the lane files,
never re-run; no lane fixture, probe or `archives/` binary was run; no endpoint was called.
I did not read 2,740 reasoning bodies — every prediction whose subject is a reasoning-body claim
stays UNTESTED (the N14a/N14d/O20 reasoning halves; CS R3's "cost 0 … safe" clause was grepped,
not read). I did not enumerate the 132 `hold_released_turn` events against their holders' untaps.
I did not re-derive `serializeGameState()`; §2(d)'s board verdict is a diff of the rendered
SITUATION block with the phase and life-trend lines removed, not of the engine's key. My
`holdActionKeyRow` is a **Python re-implementation** (balanced `{…}`/`[…]` strip + the wave-56
land parenthetical + a decline/hold-row filter); it does NOT transcribe `w75CostGroupsKey`, and
§2(c)'s six disagreements are all traceable to that gap (see below). I did not hand-verify the
arithmetic of the 40 crack-back clauses that carry no machine-parsable triple, nor every one of
the 218 blocker-forecast rows. `askreplay/` records were counted and their `why` fields read, not
line-by-line compared to their parents. I did not read the deck-seat review files.

---

## 1. Corpus census

| | wave 77 | wave 76 |
|---|---|---|
| seats / games / summed turns | 42 / 21 / **990** | 42 / 21 / 920 |
| completed naturally | 42 gameend, 21 `results.tsv` rows, **0 hang, 0 crash, 0 timeout-tainted** | same |
| **decision records (carry a `prompt`)** | **2,740** | 2,188 |
| by kind | ask 1,999 · priority 567 · attackers 82 · discard 51 · reveal 21 · blockers 18 · bottom 2 | ask 1,594 · priority 412 · attackers 94 · discard 40 · blockers 29 · reveal 13 · bottom 6 |
| non-decision new kinds | **recovery 5 · defer 1** (neither carries a model reply — see §2(a)) | — |
| decisions per turn, corpus-wide | **2.77** | 2.38 |
| same, ask seam only | **2.02** | 1.73 |
| **fallbacks, all kinds** | **7 stamps**: 5 `unparsed_reply` + 1 `noop_row_zero_reask` + 1 `repeat_count_reask`; **5 heuristic answers** | 1 stamp, 0 heuristic answers |
| `transport` | `curl=0,http=200,empty=0` on **2,740 / 2,740** | same on 2,188 |
| `http_error_body` | 0 records | 0 |
| `async_drops` | **53**, every one `[outcome: re-asked]`; `answer_replaced` 0 on all 2,740 | 37 |
| `deadline_pct` | p50 9.1 · p95 20.5 · max 50.7 | 10.0 / 21.5 / 49.6 |
| latency | p50 **82.5 s** · p95 185.0 · max 462.7 | 90.2 / 194.3 / 449.4 |
| `reasoning_chars` | min **734** · p50 5,854 · mean 6,276 · p95 10,698 · max **35,886** | 376 / 5,639 / 6,126 / 10,457 / 22,793 |
| `reply_truncated` | **0** | 0 |
| prompt bytes | p50 13,759 · p95 29,019 · p99 31,690 · **max 36,211** · **712 over 20 KB, 0 over 40 KB** | 11,994 / 23,321 / 28,460 / 30,375 / 282 over 20 KB |
| wins | 152 6/6 · 146 5/6 · 162 3/6 · 125 3/6 · 123 2/6 · 126 1/6 · 130 1/6 | 152 5 · 130 4 · 162 3 · 126 3 · 146 2 · 123 2 · 125 2 |

**⚠ The brief's census arithmetic does not close.** `ask 1,999 + priority 567 + attackers 82 +
discard 51 + reveal 21 + blockers 18 + recovery 5 + bottom 2 + defer 1` is **2,746**, not 2,740.
`protocol_replies` = 2,740 = that sum **minus the 5 `recovery` and 1 `defer` records**, which are
not protocol replies: neither carries a `reply` from the model (`defer`'s `reply` field holds the
engine's own explanation, `recovery` has no reply field at all). The census kind-list and the
2,740 figure are two different populations and should not be summed to each other.

**Reasoning (invariant 000).** 2,740 / 2,740 decision records carry `thinking: on` and a
non-empty native `reasoning`; **0 empty**. The harness gate agrees on the first run
(`REGIME GATE PASSED: thinking=on: 210 records, all carry reasoning. seats: 42 shapes:
two_line=210`) and on all 20 rerun pools. **The corpus is valid under 000** — the second
reasoning-on corpus since the wave-76 ruling, and the first where no seat launched with the
harness default.

**Reply shape.** `protocol_replies` 2,740. `plan_line_missing` **141 (5.1%)**,
`off_protocol_bytes>0` **120 (4.4%)**, overlap 115. `protocol_deviation` partitions exactly:
`compliant` 2,594 · `unlabelled_plan` 115 · `plan_absent` 26 · `prose_outside_two_lines` 5 —
**146 non-compliant = `protocol_deviation_replies` 146**. The brief's identity guess is right to
the record: **146 = 141 `plan_line_missing` + the 5 `prose_outside_two_lines`**, and those 5 are
exactly the 5 `unparsed_reply` fallbacks (they DID write a PLAN line; what they lost was the
`CHOICE:` label). The 26 `plan_absent` replies carry 0 off-protocol bytes. Off-protocol byte
weight: n=120, sum 7,319 B, median 56, max 223. `action_before_plan_replies` 0,
`answer_replaced` 0, `post_answer_overrun` 0 on every record.

**Ceilings.** `max_tokens_answer`: ask 288 (1,991) / 400 (8 retries); priority 288 (556) / 400
(11); attackers 256 (82); blockers 256 (18); discard 256 (51); reveal 256 (21); bottom 512 (2).
`reply_truncated` 0. `dropped_assignments` **0 on all 18 blockers records** — BS F5's doubled
allowance is UNTESTED for the **seventh** wave.

**Degeneracy meters.** `reasoning_degenerate` p50 0.0031 · p95 0.0059 · max 0.0444.
`reasoning_ngram_repeat` p50 0.128 · p95 0.263 · **max 0.913**; 428 records above 0.2. The 0.913
is new (wave 76 max 0.555) and sits on a very long trace; it is a ratio, not a flag, and nothing
in either distribution looks like a collapse, but the tail moved.

**`askreplay/` sidecar — 6 records are missing from the consolidated corpus.** 29 files, **1,328
records** against summed `ask_replays_reserved` **1,334**; `why`: `repeat_ask_reserved` **1,068**
against `identical_ask_answers_reserved` **1,073**; `cache_replay` 260. The whole gap is the
FIRST-RUN game `126v162`: its two seats report 5+1 reserved / 5+0 identical and have **no
askreplay file in `-final` at all**. The wave-77 manifest harvest names seat logs
(`WAGIC_GPT_TRANSLOG_FILE`) — it does not name their `askreplay/` sidecars, and the hand
consolidation after the wedge did not carry them. `ask_replays_refused` 0; max `replay_run` 11,
cap 64 — the refusal path was never reached and no `refusing the …` line appears in any stderr.

**`plan_steps_done`.** `(done,count)`: (0,1) 1,286 · (1,2) 1,019 · (1,3) 314 · (1,4) 79 · (1,5)
15 · (0,0) 14 · (2,3) 6 · (0,2) 5 · (1,6) 1 · (0,4) 1. The pointer reaches ≥2 on **6** of 2,740
(wave 76: 5 of 2,188). Unchanged for the fourth wave: the model re-states its plan far more often
than it walks it.

---

## 2. REGRESSION WATCH — the wave-77 changes

### (a) FALLBACKS ARE BACK — 5 heuristic answers, and all five are ONE parser gap at ONE seat

Two corpora at zero before this. All 6 ask-seam stamps and the 7th (`repeat_count_reask`) are
below, verbatim, with their classification.

**All five `unparsed_reply` replies are two lines, PLAN-first, with the action line carrying an
unambiguous row number and the row's short name — and no `CHOICE:` label.** Every one is at the
**deck125 seat** (4 of the 5 in a two-game cluster against deck126/deck123).

1. `125v123` seq 315, turn 49 End, life 42-5, `off_protocol_bytes` 17:
   `"\n\nPLAN: Opponent is at 5 life, my two Staffs will deal 2 damage per turn, securing the win in 3 turns. Stop 3 prohibits Sphinx's Revelation, and there are no threats to answer. I will hold priority to close the turn.\n2 (Hold priority)"`
2. `125v126` seq 140, turn 29 Upkeep, life 43-10, 17 B:
   `"\n\nPLAN: Let Staff pings resolve and hold priority to stop the menu loop.\n2 (Hold priority)"`
3. `125v126` seq 201, turn 34 Main phase 2, life 48-6, 26 B:
   `"\n\nPLAN: Wait for Staff of Nin to ping the opponent down to 0.\n3 (Cast nothing right now)"`
4. `125v126` seq 219, turn 35 Main phase 2, life 48-4, 26 B:
   `"\n\nPLAN: Exquisite Blood converts my damage to their life gain, so I will avoid dealing damage and hold priority to survive.\n5 (Cast nothing right now)"`
5. `125v162` seq 49, turn 15 Main phase 1, life 20-19, 26 B:
   `"\n\nPLAN: Cast nothing right now.\n4 (Cast nothing right now)"`

**Classification: HIGH — a reply shape the parser should read.** The protocol's letter is not met
(`LINE TWO … BEGINS with the label the decision asks for`), but nothing here is prose: the reply
is exactly the two lines the protocol asks for, the second line is `<number> (<short name>)` —
the protocol's own `CHOICE: <number> (<short name>)` with only the six-character label absent —
the number is in range on every one of the five, and the short name matches the named row
byte-for-byte in all five. There is no second coded answer to disambiguate against
(`coded_answers: 0`). Reading a bare leading `<n> (<name>)` on the answer line when it is the
only coded answer in the reply is not prose tolerance and licenses no narration: it recovers an
answer the model gave. **This is the owner's call, not mine** — but the alternative is that a
correct answer is discarded and the heuristic plays the window, which is what happened five times
here. Related MED: all five carry `protocol_deviation: prose_outside_two_lines`, which is
**false** — there is no prose outside the two lines. The stamp should distinguish "label absent"
from "prose present"; as written it mis-describes every instance of this class.

**Did the heuristic's play change the game?** The 5 `recovery` records name what it did:
- seq 315 → `executed_by heuristic, seam cast, choice 0, "cast nothing"`. Model wanted the HOLD
  row; the heuristic declined instead. Both are non-actions; the run re-opened. Game **won** 42-0.
- seq 140 → `choice 1, "cast Path to Exile"` (`recovered_by: "- Paid {w} for Path to Exile with
  Plains #1"`). **Divergent and material**: the model wanted to hold; the heuristic spent a
  removal spell on a 0/4 Perimeter Captain at 43 life. Game won 48-0.
- seq 201 → `choice 1, "cast Path to Exile"` on a Pride Guardian. **Divergent and material** —
  same shape, same game, five turns later.
- seq 219 → `choice 0, "cast nothing"`. The model asked for "Cast nothing right now". **Agreed.**
- seq 49 → `choice 0, "cast nothing"`. The model asked for "Cast nothing right now". **Agreed.**

So: 2 of 5 spent a card the model was explicitly holding; 2 of 5 the heuristic and the model
agreed; 1 of 5 differed only between two flavours of not-acting. **No game turned on it** — the
deck125 seat won three of the four affected games and lost `125v162` 0-10 at turn 35, twenty
turns after its only fallback, which the heuristic answered the way the model asked. The cost
here is the loss of two Path to Exiles, not a loss column.

**The 6th stamp is not a heuristic answer.** `125v130` seq 116, turn 31, `noop_row_zero_reask`,
`coded_answers: 1`, `choice: 2`, reply
`"\n\nPLAN: Cast Supreme Verdict to reduce hand to 7, then hold priority.\nCHOICE: 2 (Cast Supreme Verdict)"`.
The row's own verdict reads `destroys 0 of their creatures … 0 of yours`; the engine re-asked
(`[RE-ASK] You chose row 2 …, whose own verdict on this list reads zero`) and the model answered
the re-ask. **No recovery record; the model kept the decision.** Same for the 7th,
`123v126` seq 38 `repeat_count_reask` (`"CHOICE: 2 (Create human with Thraben Doomsayer, repeated
then stop)"` naming no count; re-asked and answered). **The honest fallback line for wave 77 is
5 heuristic answers + 2 model re-asks, not 6 + 1.**

**`phase2_answer_missing` 1.** `126v162` (the first-run game) seq 20, kind `ask`, turn 5,
`reasoning_forced_close: true`, `reasoning_budget_hit: true`, `retry: 1`, `latency_ms` 302,899,
`reasoning_chars` 22,331, `coded_answers: 1`, **no fallback stamp, `protocol_deviation:
compliant`, `off_protocol_bytes: 0`** — the phase-2 decode returned a usable answer but the
recovery bookkeeping did not see it. **The CU F5 wall-miss/seq change is NOT involved:**
`wall_miss_events` / `wall_miss_no_retry` / `wall_miss_unrecorded` are **0 on every one of the 42
seats**, no `wall_miss` record exists anywhere in the corpus, and no stderr carries a wall-miss
line. F5's defect requires a wall-miss record; the corpus contains none, so F5's live path is
UNTESTED (see §3) and this record is an unrelated accounting miss. The window cost nothing.

**The new record kinds.** Read from source, not inferred:
- **`recovery` (5)** — `AIPlayerGPT.cpp:19439`, written by `flushRecovery` **directly**, not
  through `writeTransLog`. It trails a fallback record and states what the heuristic actually
  did: `recovers_seq` / `recovers_kind` / `recovers_fallback`, plus `executed_seam`,
  `executed_choice` (1-based printed row, 0 = a pass or a no-cast), `executed_text`,
  `executed_by`, and `recovered_by` (the narration delta — **absent, not empty**, when the
  heuristic's answer produced no narration; 2 of the 5 here). All 5 are `executed_by: heuristic`,
  all 5 `executed_seam: cast`. **None lost a decision by existing** — the decision was already
  lost at the fallback; `recovery` is the record that makes the loss auditable, and it is the
  reason §2(a) above can say what the two Path to Exile casts were.
- **`defer` (1, `menu_not_askable`)** — `AIPlayerGPT.cpp:44794`, in `selectMenuOption`'s
  no-answerable-shape branch: `if (DecisionManager::buildMenuChoice(...))` fails, the base
  `AIPlayerBaka::selectMenuOption()` clicks, and the engine writes a `defer` record so a reviewer
  can tell "not asked" from "asked and answered". The one instance: `152v130` seq 21, turn 15
  Main phase 1, life 16-13, `choice: 0`, `chosen_text: "Starstorm"`,
  `reply: "menu on Starstorm could not be put to the model (no answerable shape - e.g. the
  announced X is no longer affordable); the heuristic took option 0"`, `options_text` **empty**.
  It sits between the seat casting Tovolar's Huntmaster (seq 20) and declaring attackers (seq
  22); the seat won that game 16 to -7. **It did lose a decision** — a menu was answered by the
  heuristic with the model never asked — but the record cannot say WHICH decision: `options` is
  0 and `options_text` is empty, so the menu's rows are unrecoverable. **MED: a `defer` record
  should carry the menu it declined to ask, or a reviewer can only report that something
  happened.** This is also the corpus's only `{X}`-announcement payment-loss event (§3 docket).

### (b) The loop family — the wave-75/76 storms are gone; the skip counter absorbed them

Asks per (turn, rendered phase, seat), phase read out of each prompt's own `Phase:` line, seam
from `log_window_kind`: the maximum anywhere in the corpus is **13** (`146v125`, deck146 seat,
turn 27, Main phase 1, cast seam) and **exactly one** (turn, phase, seat, seam) cell exceeds 10.
That cell is **not a loop**: its 15 records at that turn walk a real sequence — Triumphant
Adventurer, Acererak the Archlich, a dungeon-room menu (`veils of fear`), a discard mode, a
target menu, and a graveyard-cast menu — each with a different option set. Next highest cells are
10, four of them. No seat shows the wave-75 shape.

**`own_loop_windows_asked` 1 — R1/CU F4 quoted in full.** The single window is `126v130`, deck126
seat, **record seq 43**, kind `priority`, turn 17 Main phase 1, life 7-14. The guidance clause is
on all three Sorin rows:
`[your loop is resolving on its own - this row is not needed to win it; the HOLD row on this menu
covers every link of it until the loop ends or something else changes]`.

- **The `[own loop verdict:]` MARKER ROW did not print, and must not.** `w77OwnLoopVerdictKey`
  (`AIPlayerGPT.cpp:30503`) feeds `kHoldVerdictMarkerHead` (`:31135`) — it is a **synthetic marker
  joined to the HELD SET**, not a rendered row ("A synthetic verdict marker is not a row the model
  can take"). It is invisible in every prompt in the corpus, by design; the brief's question
  presumes a render that the fix never had. What is observable is its effect: the verdict is
  recomputed per window and a change in it re-opens the hold.
- **Verdict: RESOLVING** (the resolving wording; no THREATENED text anywhere in the corpus).
- **The verdict is TRUE against the live stack.** The prompt's own stack block reads
  `1 (top): ability: Sanguine Bond's Life Loss [from your Sanguine Bond] [triggered/activated
  ability]` — a loop component of the seat's, unresolved, with no opponent object on the stack.
  The discrimination is real: the immediately preceding window (seq 42, same turn, same phase)
  had `ability: Tribute to Hunger's gain life equal to its toughness … [from your Tribute to
  Hunger]` on the stack — not a loop component — and printed **no** clause (verdict IDLE).
- **The seat did NOT hold.** It answered `CHOICE: 1 (+1: create a 1/1 vampire with Sorin, Lord of
  Innistrad)`. The game ended on the same turn (gameend at seq 44, won). Nothing was lost: the
  clause said the row was not needed, and the model took it anyway; the loop closed regardless.

`own_loop_windows_asked` 1 against wave 76's uncounted 35-window deck126 storm: the storm did not
recur, but it did not recur because this corpus's only completed loop won on the turn it
completed. CR R1's prediction is adjudicated in §3 on that basis.

**`hold_windows_skipped` 2,128 (cast 1,433 / priority 695) against wave 76's 1,280 — +66%.**
What grew is the CAST arm: the stderr confirms the split to the line
(`holding priority at the cast seam …` **1,433** lines, `… at the priority seam` **695**), and
the growth tracks the corpus's 25% more decisions plus a much longer tail of grindy games
(990 turns vs 920, and three games past turn 50). `hold_released_turn` 132 (wave 76: 89) rises
with it — the latch is being retired every turn as designed, not accumulating.
`own_turn_windows_skipped` 596, `mana_only_windows_skipped` 69, `stop_reached_windows_skipped` 7,
`main_phase_windows_skipped` 89, `sibling_window_asks_skipped` 0, `menu_pass_no_progress` 0.

### (c) `[hold check: …]` after R2 + CU F5/F6 — 2,174 comparing brackets, 0 demonstrably false

**2,292 records carry a bracket** (cast 1,309 · priority 535 · land 330): **116 "first window"
sentences + 2,176 comparing sentences**, of which 2,174 have a resolvable predecessor.

- **Referent audit.** `hold_check_ref_seq` is a WINDOW sequence (`mWindowSeq`), not the record
  `seq`; it advances on every `writeTransLog` record (`w77ConsumeWindowSeq`, `:19628`) and
  **not** on `recovery` records (written outside `writeTransLog`) or on `wall_miss` (F5's fix).
  Reconstructing it that way and taking "the previous record at this seam **that staged a note**"
  as the expected referent (`w76HoldWindowAsked` rejects a commit when `pendingSeq[s] !=
  windowSeq`, so a sub-menu that stages nothing never becomes the referent): **2,174 of 2,174
  comparing brackets name exactly the expected window.** The two apparent exceptions
  (`146v126` seq 47 and 56) resolve to records 46 and 55 — `ask` records that staged, committed,
  and then printed no bracket (see the stamp-leak item below); the referent is correct, the
  render is not.
- **Truth of the claim.** Diffing each record's `options_text` against its referent's under the
  Python `holdActionKeyRow`: **2,168 TRUE, 6 disagree — and all 6 disagreements are my
  transcription, not the engine's claim.** Hand-checked, all six: `152v123` 31→34 and
  `152v130` 12→17 changed a row's `[cost: {1}{g}]` to `[cost: {3}{g}]` (the engine calls that a
  new row, which is the ruling; my stripper drops `[cost:]`); `152v125` 23→25, `152v146` 27→29,
  `146v125` 204→205 changed only a digit or a keyword inside a brace/bracket group (the engine
  calls that the same row, which is the ruling; my stripper mis-parsed an unbalanced `}` inside a
  `{card text: "…"}` blob); `146v125` 103→107 added a real new row (Acererak) which the engine
  counted as 1 new, and my measure said 2 because a mana digit also moved.
  **Against wave 76's 22 false brackets, this corpus has none.**
- **The land seam.** 330 land-drop brackets, **0 of them naming a casting menu as referent**, and
  all 330 arithmetically true. Wave 76's entire false population (22 of 338, all cast/land pairs)
  is gone. R2a delivered.
- **"First window asked" sentences: 116, and 0 of them follow an asked predecessor at that seam.**
  CU F5's timeout case does not occur anywhere in the corpus — consistent with 0 wall-miss events.
- **MED — 18 records carry `hold_check_ref_seq` with NO bracket in their prompt.** The field's
  documented contract (`:19943`, "absent = this window printed no bracket") is violated 18 times:
  14 `discard`/`cleanup_discard`, 2 `ask`/`unclassified`, 1 `attackers`, 1 `blockers`. The
  discard/attackers/blockers cases are a stale stamp consumed by a record that never printed a
  bracket (they do not commit, so the seam history is unharmed, but a reviewer reading the field
  is told a bracket exists). The **2 `ask` cases are worse**: `146v126` records 46 and 55 staged a
  note, COMMITTED to the cast seam (the next window's referent is them) and rendered no bracket —
  so the model was asked two cast windows with no hold-check line while the bracket run continued
  counting them. Neither cost a decision here; the class is a silent surface loss.

### (d) `crossphase_identical_reputs` 119 / `crossphase_board_unchanged` 2

**Both positive clauses, verbatim**, and both are at the deck125 seat in `125v126`:
- seq 199: `[this exact list was put to you 1 window ago at Combat begins and you declined; nothing on the board has changed]`
- seq 269: `[this exact list was put to you 1 window ago at Main phase 2 and you declined; nothing on the board has changed]`

**The clause renders 115 times, not 119** (113 "moved" + 2 "unchanged"); 4 counted re-puts
printed nothing. LOW, but the counter and the surface are not the same population.

**Are the others genuinely moved?** Diffing each record's rendered SITUATION block against the
prompt of the window its own clause cites, with the `Phase:` and life-trend lines removed (the
engine compares below the phase line): **112 of 113 "the board has moved" claims are true.** One
is false — `125v126` **seq 198**, whose cited predecessor one window back has a byte-identical
board; the clause tells the model the board moved when it did not. The two "nothing has changed"
claims are both true. Net: 114/115 true, one false in the conservative direction. R8's positive
clause is alive and is not over-firing.

### (e) `forced_close_unrecorded` 10 — the bound STILL never fires, and the counter DOUBLED

`force_close_arms_refused` **0** / `_deferred` **0** / `_same_arm_deferred` **0** /
`_defer_bound_hits` **0** — **fourth wave running with every one at zero.** Meanwhile
`forced_close_unrecorded` went **5 → 10**. CR R11(a) predicted the opposite (§3: FAIL).

**The reconciliation closes exactly.** The stderr carries the close line —
`AIPlayerGPT: unclosed <think> (budget/truncation); forcing the answer` — **26 times across the
42 seats**. Against `phase2_answer_recovered` 15 + `phase2_answer_missing` 1 +
`forced_close_unrecorded` 10 = **26**. The identity `recovered + missing + unrecorded == closes`
**holds corpus-wide**. (There is no `force_close` / `same arm` / `refusing` line in any stderr —
those paths never ran, consistent with the four zeros.)

**Naming the 10.** The counter is a bare integer on `gameend` with no per-event record, so the arm
and what superseded it are **not recoverable from the corpus**. What is recoverable is the game
and the count, from the 26 close lines minus the 16 records carrying `reasoning_forced_close`:

| game / seat | unrecorded | recovered | records carrying `reasoning_forced_close` |
|---|---|---|---|
| `123v130` deck130 | 2 | 1 | seq 13 (priority) |
| `125v162` deck125 | 1 | 1 | seq 187 (ask) |
| `126v130` deck130 | 1 | 0 | — |
| `146v123` deck123 | 1 | 0 | — |
| `146v125` deck125 | 1 | 0 | — |
| `146v126` deck146 | 1 | 0 | — |
| `152v125` deck125 | 1 | 1 | seq 28 (priority) |
| `152v126` deck126 | 1 | 0 | — |
| `152v126` deck152 | 1 | 0 | — |

Six of the ten are in games where the arm fired and **no** record carries a close at all — the
decode was bought and thrown away with nothing to show. **MED: `forced_close_unrecorded` needs the
same treatment `recovery` just got — a record naming the arm and the window it was dropped on.**
Without it the brief's own question ("name each of the 10, and what superseded it") is
unanswerable from a corpus, and has been for three waves.

### (f) R3 — the event-naming clause on life-gain-on-sacrifice rows and both edict branches

**`{their converter: …}` — 16 clauses on 13 records, and every one names the EVENT**, e.g.
`{their converter: this body has toughness 5 and they control 1 life-to-damage converter
(Sanguine Bond) - any effect of theirs that gains them life equal to its toughness (a
sacrifice-for-toughness edict, a drain of that size) takes 5 off YOU: life 20 -> 15}`.
**0 of 16 contain `ANY nonzero payment`** (wave 76: every loop-branch render). The arithmetic is
true on all 16 (toughness → life delta matches the seat's stated life on each). **0 reasoning
bodies on a converter window match `cost[s]? 0.*safe`** (wave 76: 1, and it lost the game).

**Edicts — both branches, on all 11 rows.** `Cast Devour Flesh` renders
`{right now: they control 9 creatures - they choose which one - they gain that creature's
toughness: THEIR pick, so between 1 and 6 across their creatures; YOU control 11 creatures -
targeting yourself sacrifices one of them, your choice, and you gain its toughness - you pick
which, so up to 2 (your largest toughness)}`. **`THEIR pick, so` is present on all 11**, the
range is a real min/max across their bodies on every one I checked, and the **OWN-converter case
is stated in the same clause** with the opposite quantifier (`you pick which, so up to N`), which
is the correct asymmetry. **1 `[you SACRIFICE this; you gain N life (its toughness)]` row exists
in the whole corpus** (`123v162` seq 116) — too few to test the "and with your `<name>` that is
N off THEIRS" clause as a population; it is the shape CS R3 was written for and it barely occurs.
**The PROHIBITION case never occurs:** `cantchangelife` / `nolifegain` / `nolifegainopponent` /
`life gain is PREVENTED` appear in **0** prompts. F7's prohibition branch is UNTESTED.

### (g) R4 — blockers headers: the AT BEST folded figure, and the CU F7 gate

**5 prompts carry `AT BEST (no assignment of your blockers does better)`.** Cross-tab:
3 carry BOTH `may gain` and `OPTIONAL gain`; 2 carry **neither**. **The forbidden cell —
`may gain` present × `OPTIONAL gain` absent — is 0.** The folded figure is stated as a distinct
second number, not merged into the certain one:
`you would be at 11 AT BEST (no assignment of your blockers does better), 13 AT BEST if you also
take every OPTIONAL gain the BLOCKING THIS COMBAT line below meters (2 more; those are "may"
gains and yours to take)` — and the `2 more` matches the `BLOCKING THIS COMBAT: each of your 2
blockers that blocks gains you 0 and may gain 2 more - up to 4 life for you` line below it on
each of the three. The death-verdict withdrawal is stated separately and correctly on the one
lethal board (`146v126` seq 28: `-5 AT BEST …, -3 AT BEST if you also take every OPTIONAL gain …;
no block saves you` — the gain does not reach, so the verdict STANDS, which is the right way
round). **The prohibition gate (CU F7) is UNTESTED — no board in the corpus forbids life gain.**
Also: `AT BEST once the` — the wave-65 certain-gain-only line CS pinned as its negative — appears
**0 times**, so that pin's population is empty here and the pin is UNTESTED, not failed.

### (h) R5 — the creature `{effect:}` gloss

**2,977 gloss renders across 20 distinct (name, gloss) pairs / 11 distinct cards.** Sampled 30
(all 20 distinct pairs plus repeats); every gloss is a faithful transcription of the card's
`text=` for its own seat, and the foreign-voice wrapper is correct on THEIR cards
(`{effect (THEIR card, written from THEIR seat: "you"/"your" in this text means THEM, and
"opponent" in it means YOU): "…"}`) and the copy wrapper on duplicates
(`{effect (each copy of this card does this): "…"}`).

**MED — one gloss is cut mid-clause into a statement with no content, 55 times.**
`Howling Mine #1 {2} [artifact] {effect (each copy of this card does this): "At the beginning of
each player's draw step, if Howling Mine is untapped, that player..."}`. The same card renders
its full text 443 times elsewhere: `"… that player draws an additional card."` The cut lands on a
conditional's subject and drops the entire consequent — a model reading the short form learns
that something happens at each draw step and nothing about what. It is `textSnippetCore`'s
**character** truncation, not the clause-level give-back loop CU F9 built: the prompt where it
occurs is 26,995 B, nowhere near any whole-prompt bound, so the tier is per-name and the cut is
unconditional at that length. Two further mid-word cuts of the same class on Lightning Greaves
(31 + 4 renders, cut inside `stays on the bat…`). **Recommendation: truncate at a clause boundary
(`-- ` or `. `) and count the omission, never mid-sentence — the give-back loop already has the
vocabulary for it.**

**Prompt bytes: max 36,211 B**, against CT's post-F9 bound of **38,657 B** and the 40,000 B
ceiling. The bound holds with 3,789 B of headroom. **712 prompts over 20 KB** (wave 76: 282) —
the 20 KB population more than doubled, which is where the gloss and the cover clauses went.

### (i) R6/R7 — `{crack-back cover:}`, X sub-menu rows, the stay-home line

**81 `{crack-back cover…}` clauses on 69 records** (median 508 B, **max 1,365 B**); 475 records
carry a `CRACK-BACK NEXT TURN:` line; 4 `{crack-back cover, STAY HOME:}` on attackers windows.

- **Arithmetic: 41 of the 81 carry a machine-checkable triple (`line above is T … takes P off …
  leaving L -> you would be at R`, or the `you cover C of T, leaving L -> at R` form). 0 are
  wrong** — `P + L == T` and `R == at + P` on every one. The other 40 state a floor or a
  THEY-choose bound with no closed arithmetic to check.
- **CU F2 (single-target vs sweep): PASS, and the wording is the fix.** Single-target rows render
  `This row removes ONE body and the target is YOUR choice: Barrowin of Clan Undurr takes 3 off
  that total, leaving 4; Spirit Cai, Goblin each take 1 off that total, leaving 6. The most any
  ONE of them takes off is 3, leaving 4` — the BEST SINGLE victim, never the sum. Sweep rows
  render the other family (`This row REMOVES 4 of those 4 attackers … takes 12 off the total`).
  **No clause in the corpus sums multiple victims on a single-target row.**
- **CU F8 (roster fold): PASS.** Victims group by what they take (`Spirit Cai, Goblin each take 1
  off that total`), the total is stated once above, largest contribution first. Largest clause
  1,365 B, on a board of 31 attackers; nothing approaches the 6,212 B shape F8 measured.
- **CU F3 (regeneration/ghostform/persist/undying not subtracted): UNTESTED.** No clause in the
  corpus subtracts a body, and no board in the corpus carries a regenerating, ghostformed,
  indestructible or persist/undying blocker in a crack-back total. (`ghostform` appears in 256
  prompts, all in the general rules block, never on a crack-back victim.)
- **R7 legality filter: live.** Clauses state the filtered count explicitly —
  `1 of their 1 could not legally be blocked by it at all (evasion, protection, or a block needing
  more bodies than this row adds). Counting only the checked bodies you cover 0 of 6` — and the
  aggregate `You ALSO already control N untapped creatures that can block on their turn without
  casting anything, and this clause counts them FIRST: on their own they cover C of T (each of
  them blocks at most ONE attacker, and none is counted against two)` appears 4 times, each with
  C ≤ T and the per-attacker one-body rule stated. CS's cross-tab on over-filtering has too small
  a population (4) to move.
- **CS R7 ATTACK TOTAL: PASS.** 82 attackers prompts carry an `ATTACK TOTAL` line; 51 of them
  also carry `[no creature they control can block this attacker]` A-rows. **0 of the 82 assert
  full cover** — the line's only verdict is lethality (`That is NOT lethal: they survive at 15
  even with none of them blocked. At least 5 damage lands whatever they block`).
- **STAY HOME: 4 clauses, all on windows that also carry a `CRACK-BACK NEXT TURN:` line**, and 0
  `STAY HOME` on a window without one. Arithmetic true on all four
  (`keeping all 4 of them back covers 16 of that 22, leaving 6`).

### (j) R10 — the ANNOUNCE_X plan echo and the neutral example

**16 X sub-menu windows** in the corpus (rows of the shape `X = N {X pricing: …}`), all at the
deck125 seat (Sphinx's Revelation) plus deck130 (Starstorm).

- **Plan echo: 16 of 16** carry `THE PLAN YOU LAST STATED …`, the same rate as the rest of the
  seat. (Wave 76: 0 of 3.)
- **Neutral example: 16 of 16**, and **0 of 16 name a row**: the closing instruction reads
  `e.g. "CHOICE: <n> (X = <n>)" (a worked example of the FORMAT only - <n> is a placeholder, not
  a recommendation: read the rows below and choose the X YOU want)`.
- **Negative pin holds:** `written out from row N of this list` is present on **1,983
  non-X records** and on **0 of the 16 X windows** — the neutral branch did not leak off its seam.
- **0 X replies chose X=1** on the ground that an example named row 1 — the example names no row.
- 3 of the 16 also carry a `{crack-back cover:}` clause (CS R6's third clause, live on the X menu).
`identical_option_asks_resolved` **8**, `repeat_annotated_takes` **2**, `put_gloss_stripped` 0,
`engine_reveal_floor_picks` 0, `declined_face_latches` **0** (fifth wave — see §3 docket).

### (k) R9 — `plan_names_uncastable_zone_card` 52, `plan_names_stranded_card` 4

52 corpus-wide against wave 76's 102 — **down 49%**, and still non-zero, which is what CR R9
predicted. (56 decision records carry the card name against a gameend sum of 52; the 4-record gap
is a counting rule I did not chase.)

Sampled 14 by hand. The population is now dominated by the shape the counter exists for — a plan
naming a card that is not castable from where it sits, with the note correctly scoped to the menu
(`(note: this decision's list does not contain the actions your plan names. That is about this
menu, not about what is legal for you …)`, and `"Bloodline Keeper" is no longer on your menu`).

**One residual false positive in the sample.** `126v130` seq 17: the plan reads *"then next turn
cast Idyllic Tutor to find Exquisite Blood and complete the combo"* and the note fires on
**Exquisite Blood** (`no "Exquisite Blood" is in your hand, on either battlefield, in a graveyard,
in exile or on the stack right now - a copy is still in your LIBRARY`). The verb governing
Exquisite Blood is **find**, not cast — this is precisely the tutor-target population R9's
cast-intent restriction was supposed to exempt, and the restriction reads the plan's cast verb
without following it to the object it governs. The note is not harmful (it is accurate about the
zone) but it is noise on a correct plan. `plan_names_stranded_card` 4 (8 records carry a name).

### (l) R11 — the harvest by manifest

**PASS, verified end to end.** All 20 rerun pool logs report `== harvested 2 player-game logs ==`
(20 of 20, with decision counts 35-387). The `-final` dir holds exactly 42 `.jsonl` files.
Every stderr announces exactly **2** `WAGIC_GPT_TRANSLOG_FILE <basename>` lines (42 announcements
across 21 stderr files, **2 per file with no duplicates**), and **the set of announced basenames
is byte-identical to the set of harvested seat logs** (`diff` clean). The deck+tolerance fallback
never fired — no `WARN`/fallback line in any pool log. **The same-arm force-close defer never
fired either** (`force_close_same_arm_deferred` 0, §2(e)).

The one gap the manifest does not cover: **`askreplay/` sidecars are not named by it**, and the
first-run game's two sidecars are absent from `-final` (§1). If the hand consolidation is ever
replaced by a scripted one, the sidecar is the file it will drop.

### (m) `async_drops` 53, replays, and the deviation split

**`async_drops` 53** (wave 76: 37). Every one carries an `[outcome: re-asked]` event and
`answer_replaced` is **0 on all 2,740 records** — nothing was answered from a stale slot. By
reason: `casting/question and board` 37 · `casting/question (or turn/phase)` 7 ·
`casting/board` 6 · `land-drop/question and board` 2 · `land-drop/question (or turn/phase)` 1.
The land-drop reasons are new this wave and are R2a's seam split showing up in the drop
classifier, not a new drop class.

`ask_replays_reserved` **1,334** / `identical_ask_answers_reserved` **1,073**; the sidecar holds
1,328 / 1,068 (the 6-record first-run gap, §1). `ask_replays_refused` 0.

`protocol_deviation_replies` **146 (5.3%**, wave 76: 4.2%) **= `plan_line_missing` 141 + 5**, and
the 5 are the `prose_outside_two_lines` stamps on the 5 `unparsed_reply` fallbacks (§2(a)) —
the brief's arithmetic, confirmed. Classification of the 141: **115 `unlabelled_plan`** (a plan
line is there but not labelled `PLAN:` — these carry the off-protocol bytes, median 56 B) and
**26 `plan_absent`** (no plan line, 0 off-protocol bytes). The deviation rate rose a point;
the shape did not change.

**`main_phase_windows_skipped` 89** — the stderr trace carries the record's turn on every one:
`own main phase (turn 13 as the records number it; observer turn 12, phase 4) ENDED with NO
casting window - the instant-speed response arm answered it; no sorcery-speed casting window was
offered (1 this game)`. Count of trace lines matches the gameend sum exactly. R-item satisfied.
`reserve_decline_windows_noted` 28 / skipped 4.

### (n) CU F1 — the ask/async keys under the balanced strip

Re-asks at the **same (seat, turn, phase, seam)** whose option lists are **byte-identical after
the balanced strip but not before**: **117**. These are the CG shape by construction — but they
are not a key defect: on all of them the model took a DECLINE row (`Cast nothing right now`),
which by the rendered contract closes only that window, so the re-ask is the design. The key's
job is to suppress a window after a HOLD, and there is no case in the corpus of a held window
re-opening on an annotation-only change. `[you declined this exact list N times already this
turn]` rises with N inside a bracket group, as intended, and never re-opens a hold.

**CU F1's prediction fails on its `>10 asks` clause, once.** `146v125`, deck146 seat, turn 27,
Main phase 1, cast seam: **13 asks**. Inspected: not a loop (Triumphant Adventurer → Acererak →
a dungeon room menu → a discard mode → a target menu → a graveyard cast, six distinct option
sets). Exactly one cell in the corpus exceeds 10. The falsifier CU named — "a window loop whose
two prompts differ only inside `{…}`" — **did not occur at that cell or anywhere else as a hold
re-open**. I record this as a FAIL on the letter with the mechanism the prediction was aimed at
absent; the owner may want the clause re-stated as "no (turn, phase, seam) shows >10 asks
**over an unchanged option set**".

### (o) The harness — PILOT STALL fired once, true positive

The first run (`matchups-20260911-010157`) hit the Spark pilot's silent engine wedge at ~03:01
CDT with 1 game finished; `== PILOT STALL: the last records of every live seat are timeout
fallbacks (6 of 6).` fired at 03:16 — a **true positive**, fired once, on a real freeze, and it
fired on the evidence (every live seat's last record a timeout fallback) rather than on a clock.
The container was restarted and the 20 stalled matchups reran as 20 concurrent two-deck pools,
all of which finished naturally with `REGIME GATE PASSED` and a 2-log harvest. No stall fired in
any rerun pool. The tripwire cost 15 minutes of a wedged engine; without it the run would have
burned the full 900 s timeout on every remaining window.

---

## 3. Prediction adjudication

### Lane CR

| # | Prediction | Verdict |
|---|---|---|
| R2a/R2b | 0 land-drop records whose `[hold check:]` referent is a casting menu; every bracket's `hold_check_ref_seq` names a record at the SAME seam class; brackets RISE, not fall | **PASS.** 330 land brackets, **0** with a cast referent; 2,174/2,174 comparing brackets name the expected same-seam window; bracket count 2,292 vs wave 76's ~1,033 — it rose. The first window at every seam prints one (116 sentences, 0 following an asked predecessor). §2(c) |
| R1 | `own_loop_windows_asked` non-zero on any seat that closes its own life loop; longest own-loop run at one seat/phase well below 32; nothing removed or auto-answered | **PASS, on a population of one.** The corpus's only completed loop (`126v130` deck126, turn 17) produced `own_loop_windows_asked` 1 and won on the same turn; the longest run is 1. No window was removed — the seat was asked and took a row. The wave-76 35-window storm did not recur, but this corpus never gave it the chance. §2(b) |
| R8 | `crossphase_board_unchanged` non-zero and small relative to `crossphase_identical_reputs`; no bracket says `nothing on the board has changed` over two asks whose boards differ | **PASS.** 2 vs 119 (1.7%; wave 76's offline split was 5 of 109). Both positive clauses quoted; both true against the boards. 112 of 113 negatives verified genuinely moved. §2(d) |
| R9 | `plan_names_uncastable_zone_card` falls by roughly the tutor-target + non-cast population (~57 of 102) while staying non-zero | **PASS.** 102 → **52** (50 removed, against ~57 predicted), non-zero, and the residual population is dominated by the shape it exists for. One tutor-target false positive survives (`126v130` seq 17). §2(k) |
| R11a | `forced_close_unrecorded` FALLS and `force_close_same_arm_deferred` accounts for the difference; identity `recovered + missing + unrecorded == closes` holds | **FAIL on the first clause, PASS on the second.** `forced_close_unrecorded` **rose 5 → 10**; `force_close_same_arm_deferred` is **0**, as are refused / deferred / defer-bound-hits — **fourth wave with the bound never firing once**. The identity holds exactly: 15 + 1 + 10 = 26 = the stderr's 26 `unclosed <think>` lines. §2(e) |
| R11b | Each one-game pool's harvest count equals exactly 2; no `-final` dir rebuilt by hand | **PASS on the count, PARTIAL on the rebuild.** 20/20 pools report `harvested 2 player-game logs`; announced basenames == harvested files, byte-identical. The `-final` dir still needed a hand merge for the first-run game, and that merge dropped its two `askreplay/` sidecars. §2(l) |

### Lane CS

| # | Prediction | Verdict |
|---|---|---|
| R3 converter | 0 `{their converter:` clauses contain `ANY nonzero payment`; 0 reasoning bodies match `cost[s]? 0.*safe`; every self-sacrifice row under a converter carries the `off THEIRS` clause; every edict row at 2+ bodies carries `THEIR pick, so` | **PASS on three clauses, UNTESTED on one.** 0/16 `ANY nonzero payment`; 0 matching reasoning bodies; 11/11 edict rows carry `THEIR pick, so` with a true range and the own-converter branch. The self-sacrifice-row clause has a population of **1** in the whole corpus — too few to test. §2(f) |
| R4 | 0 prompts carry `AT BEST (no assignment …)` while a `may gain` line on the same prompt is excluded from the figure | **PASS.** 5 AT BEST prompts; the forbidden cell (`may gain` × no `OPTIONAL gain`) is **0**; 3 fold it and state the fold; 2 have no may-gain at all. The negative pin (`AT BEST once the` present × `OPTIONAL gain` absent must not fall to zero) is **UNTESTED** — that line appears 0 times this corpus. §2(g) |
| R6 | Every `{kills whichever you target: THEIRS -` row under a crack-back line also carries `{crack-back cover:`; every ANNOUNCE_X menu under a crack-back line has ≥1 covered row; every attackers window printing `CRACK-BACK COST OF ATTACKING` also prints `{crack-back cover, STAY HOME:` | **PASS.** 81 cover clauses (wave 76: 0 on 13 such rows); 3 of the 16 X menus carry one; 4 STAY HOME clauses, all on windows carrying a crack-back line, 0 on windows without. Negative pin (a) — no clause says `This row removes ONE body` about victims outside the total: held; the THEY-choose form states a FLOOR of 0 instead. §2(i) |
| R7 | 0 `ATTACK TOTAL` lines assert full cover on a window with `[no creature they control can block this attacker]` rows; 0 `You ALSO already control N untapped creatures` with N over the distinct bodies named | **PASS on the first (0 of 82, with 51 no-block windows in the population). The second is a population of 4** — all four internally consistent (C ≤ T, one-attacker-per-body stated), too few to move the cross-tab. §2(i) |
| R10 | `plan_echo` on ANNOUNCE_X records at the same rate as the rest of the seat; 0 X replies choosing X=1 because the example named row 1; non-X asks keep `written out from row N` | **PASS on all three.** 16/16 plan echo (wave 76: 0 of 3); the example names no row on any of the 16; `written out from row N` present on 1,983 non-X records and 0 X windows. §2(j) |

### Lane CT

| # | Prediction | Verdict |
|---|---|---|
| R5 | Every eligible creature battlefield entry carries an `{effect:}` gloss; no prompt exceeds 40,000 B; no `{effect: ""}`; no already-printed-keyword body glossed | **PASS with a MED defect inside it.** 2,977 gloss renders (wave 76: 0 of 3,892); max prompt **36,211 B** against the 40,000 B ceiling and CT's post-F9 38,657 B bound; **0** empty braces; no keyword-only body glossed. The defect is not eligibility but truncation: 55 renders of Howling Mine and 35 of Lightning Greaves are cut MID-CLAUSE and state a condition with no consequent. §2(h) |
| R11-row | Largest `{kills whichever you target:}` / `{removes:}` clause under 300 B; no option row containing one over 1,600 B; no body disappears from a roster | **PASS on the fold, the byte figure needs re-reading.** 66 `kills whichever you target:` and 174 `{removes:}` clauses; rosters fold by shared contribution with the residue counted. Largest crack-back cover clause is **1,365 B** and largest option row containing one is well under 1,600 B; I did not isolate the `{kills…}`/`{removes:}` substring alone against the 300 B figure, so that exact number is **UNTESTED**. Nothing in the corpus names a `chosen_text` the row never listed. |

### Lane CU (the ten Astra findings)

| # | Prediction | Verdict |
|---|---|---|
| F1 | No `declined this exact list N times` clause with N rising on an unchanged `serializeGameState`; **no (turn, phase, seat) shows >10 asks**; falsifier = a window loop whose two prompts differ only inside `{…}` | **FAIL on the >10 clause (one cell: `146v125` deck146 turn 27 Main phase 1, 13 asks — inspected, six distinct option sets, not a loop). PASS on the falsifier**: no held window re-opened on an annotation-only change; the 117 identical-after-strip re-asks are all post-DECLINE, which the rendered contract permits. §2(n) |
| F5 | No seat log contains a "first window I have asked you" sentence following an asked predecessor at that seam | **PASS as a count (0 of 116), UNTESTED as a fix.** The defect requires a `wall_miss` record; `wall_miss_events` / `_no_retry` / `_unrecorded` are 0 on all 42 seats and no `wall_miss` record or stderr line exists. The corpus cannot distinguish "fixed" from "never triggered". §2(c) |
| F4 | `own_loop_windows_asked` non-zero only on seats whose own loop has a link on the stack; no seat holds through a window in which an opponent's stack object targets a loop component | **PASS.** The one counted window has `Sanguine Bond's Life Loss` unresolved on the stack; the window one earlier, with a non-component ability on the stack, printed nothing. **No THREATENED verdict anywhere in the corpus** — that half is UNTESTED. §2(b) |
| F3 | No `{crack-back cover…}` clause claims removed power for a body the prompt marks regenerating, ghostformed, indestructible or persist/undying | **UNTESTED.** No such body appears in any crack-back total in the corpus. §2(i) |
| F2 | No X sub-menu row of a single-target spell claims more removed power than its own best single victim; sweep rows unchanged | **PASS.** All single-target cover clauses render the BEST SINGLE victim (`The most any ONE of them takes off is 3`); sweep rows render the removal family. 0 counterexamples in 81 clauses. **On X sub-menus specifically** the population is 3 (all sweeps — Starstorm), so the single-target-X case is UNTESTED. §2(i) |
| F7 | On any board with `cantchangelife` / `nolifegain` / `nolifegainopponent` in force, no INCOMING THIS COMBAT line meters a gain and no edict row states one | **UNTESTED.** 0 prompts in the corpus carry any of the three, and `life gain is PREVENTED` renders 0 times. The GATE is observably live in the allowed direction (3 AT BEST prompts fold an allowed gain and 11 edict rows state one), which is the MUST-NOT-MATCH half. §2(f), §2(g) |
| F6 | Target-preview drift no longer counts as an action change | **PASS.** 2,174/2,174 comparing brackets true; the 6 disagreements with my Python are all my stripper's cost/brace handling, hand-checked. No re-open in the corpus is attributable to a P/T preview move. §2(c) |
| F8 | The targeted cover annotation is bounded | **PASS.** Largest cover clause 1,365 B on a 31-attacker board; grouping by contribution with the residue counted is visible in the renders. Nothing near the 6,212 B base shape. §2(i) |
| F9 | CT's snippet bound re-derived and enforced | **PASS as a bound, FAIL as a render contract.** Max prompt 36,211 B < 38,657 B < 40,000 B. But the enforced ceiling truncates mid-clause (§2(h)) — the bytes are bounded and the meaning is not. |
| F10 | Harvest membership is game identity | **PASS, verified two ways.** 42 announcements, 2 per stderr, announced set == harvested set byte-identical; 20/20 pools `harvested 2 player-game logs`; fallback never warned. §2(l) |

### Carried UNTESTED docket

| item | wave 77 |
|---|---|
| **BS F2** Tribute second edict | **UNTESTED** — 1 self-sacrifice row in the corpus, no second-edict shape. |
| **BS F3 / land latch** `declined_face_latches` | **0 for the FIFTH wave.** 1,825 stderr land-face auto-pass lines, 0 latches. The counter has never been non-zero; either the shape does not occur in this deck pool or the counter is dead. Worth one look at the increment site. |
| **BS F5** blockers ceiling | **UNTESTED, seventh wave.** `dropped_assignments` 0 on all 18 blockers records; `max_tokens_answer` 256 never approached. |
| **BS F6** distinct-index re-ask | **UNTESTED.** No distinct-index re-ask occurred; the only re-asks were `noop_row_zero_reask` (1) and `repeat_count_reask` (1). |
| **CF F1** pending-deadlock | **UNTESTED.** 0 hangs, 0 crashes, 0 timeout-tainted seats, `menu_pass_no_progress` 0. |
| **F2 / F5-nonlifelink / F8 haste / F9** | **UNTESTED** — none of the four shapes occurs in this corpus's boards. |
| **N14a / N14d / O20 reasoning halves** | **UNTESTED** — I did not read 2,740 reasoning bodies. Both grep-shaped clauses I could machine-check (CS R3's `cost 0 … safe`, CU F1's declined-count) came back clean. |
| **{X}-announcement payment loss** — count `dropped after its X was announced` | **0 renders of that phrase.** But the loss shape occurred **once**, under a different surface: the `defer` / `menu_not_askable` record (`152v130` seq 21, Starstorm), whose own text is "the announced X is no longer affordable". The docket item should be re-pointed at the `defer` record, which is where the engine now voices it. §2(a) |

---

## 4. Engine / render / narration defects

### HIGH

**H1 — A correct answer missing only its six-character label is discarded, and the heuristic plays
the window. 5 times, all at one seat.** `125v123` seq 315, `125v126` seq 140 / 201 / 219,
`125v162` seq 49 (replies quoted verbatim in §2(a)). Each is two lines, PLAN first, with
`<n> (<short name>)` on the second — an in-range row number and a byte-exact short name, and
`coded_answers: 0` so there is nothing to disambiguate against. Two of the five cost a Path to
Exile the model was explicitly holding. Repro: `125v126` seq 140, reply
`"\n\nPLAN: Let Staff pings resolve and hold priority to stop the menu loop.\n2 (Hold priority)"`,
`fallback: unparsed_reply`, followed by `recovery` seq 141
`executed_text: "cast Path to Exile"`. **The parser decision is the owner's**; what the corpus
establishes is that this is not prose, it is the protocol's own answer line with the label
elided, and that it is the first heuristic play in three corpora.

**H2 — `protocol_deviation: prose_outside_two_lines` is false on every record that carries it.**
All 5 instances are H1's replies, and all 5 are exactly two lines with no prose anywhere. Repro:
any of the five above; the reply string contains one `\n\n` prefix, a `PLAN:` line, and one
answer line. A reviewer or a future gate reading this stamp is told the model rambled. It needs
its own class (`answer_label_absent`), or the next wave will chase a rambling that never
happened.

### MED

**M1 — `forced_close_unrecorded` doubled to 10 and the bound built to stop it never fired.**
`force_close_arms_refused` 0 / `_deferred` 0 / `_same_arm_deferred` 0 / `_defer_bound_hits` 0 for
the fourth consecutive wave, while the counter went 5 → 10 (26 stderr closes, 15 recovered, 1
missing). Six of the ten are in games with **no** `reasoning_forced_close` record at all. Repro:
`123v130` deck130 seat gameend (`forced_close_unrecorded: 2`, one record at seq 13) against that
game's stderr close lines. **The counter needs the treatment `recovery` just got**: a record
naming the arm and the window whose decode was dropped. Three waves of briefs have asked reviewers
to "name each" from a bare integer.

**M2 — a `{effect:}` gloss cut mid-clause states a condition with no consequent, 55 times.**
`Howling Mine … {effect (each copy of this card does this): "At the beginning of each player's
draw step, if Howling Mine is untapped, that player..."}` — repro `125v162` seq 183, prompt
26,995 B. The same card's full text renders 443 times elsewhere. Also Lightning Greaves,
35 renders, cut inside `stays on the bat…`. The cut is `textSnippetCore`'s character truncation,
not F9's clause-level give-back, and it fires on prompts nowhere near any byte bound. Truncate on
`-- ` / `. ` and count the omission.

**M3 — `hold_check_ref_seq` is present on 18 records whose prompt carries no `[hold check:]`
bracket, contradicting the field's own documented contract.** 14 `discard`, 2 `ask`, 1
`attackers`, 1 `blockers`. The two `ask` cases are the load-bearing ones: `146v126` records **46**
and **55** staged a note, COMMITTED to the cast seam (records 47 and 56 name them as referent)
and rendered **nothing** — two cast windows put to the model with the hold-check line silently
dropped while the run kept counting. Repro: `146v126` seq 46 (`hold_check_ref_seq: 37`,
`/usr/bin/grep -c 'hold check' ` on its prompt = 0), then seq 47 (`hold_check_ref_seq: 38`).

**M4 — the `defer` record cannot say what was deferred.** `152v130` seq 21: `options: 0`,
`options_text: []`, `chosen_text: "Starstorm"`, `choice: 0`. The engine knows the menu it is
about to click (`object->menuObject`) and prints the card's name, but not the rows. A reviewer
can see that the model was not asked and cannot see what it was not asked. It is also the corpus's
only X-announcement payment loss (§3 docket), which is exactly the decision class that most needs
the rows. Repro: the record above; compare a `recovery` record, which now names seam, row and text.

**M5 — the first-run game's `askreplay/` sidecars did not survive consolidation.** 1,328 sidecar
records against `ask_replays_reserved` 1,334; 1,068 against `identical_ask_answers_reserved`
1,073. The gap is exactly `126v162`'s two seats (5+1 / 5+0), which have no sidecar file in
`-final`. The wave-77 manifest names seat logs and not their sidecars. Wave 76's sidecar identity
was exact; this wave's is not, and the reason is the hand merge.

### LOW

**L1 — one false "the board has moved" clause.** `125v126` seq 198: the cited predecessor one
window back has a byte-identical rendered board (phase and life-trend lines excluded), and the
clause says `the board has moved since then, but this list has not`. 1 of 115; the other 114 are
true, both directions.

**L2 — the crossphase counter and the crossphase surface are different populations.**
`crossphase_identical_reputs` 119 against 115 rendered `[this exact list was put to you …]`
clauses. 4 counted re-puts printed nothing.

**L3 — `plan_names_uncastable_zone_card` still fires on a tutor TARGET.** `126v130` seq 17:
the plan reads "cast Idyllic Tutor to find Exquisite Blood"; the note fires on Exquisite Blood.
The cast-intent restriction reads the verb without following it to the object it governs. The
note's content is accurate; it is noise on a correct plan, and it is the exact population R9 set
out to remove.

**L4 — record-kind census fields are not summable.** `protocol_replies` (2,740) excludes
`recovery` and `defer`; the record-kind counts (2,746) include them. The gameend counter block has
no field for "records written", so a reviewer cross-checking a census has to discover the
exclusion by arithmetic. One `window_records` counter would end it.

**L5 — `declined_face_latches` is 0 for the fifth wave** while 1,825 land-face auto-pass lines
fire. Either the shape does not occur in this deck pool or the increment is unreachable; one look
at the site would retire a standing docket entry.

---

## 5. The owner's lategame specimen (standing rule, alert-only)

Written to **`wave77/lategame-specimen.txt`** (34,632 B): the `prompt` field verbatim, nothing
elided, with a three-line header. Seat
`1789114834-ai_baka_deck125-0x562523fd1ba0-vs-ai_baka_deck162.jsonl`, record **seq 214**, kind
`ask`, **turn 33** (Upkeep), life 14 vs 10 — the largest prompt at any turn ≥ 25 in the corpus at
**34,324 B**, against a corpus max of 36,211 B. The seat answered row 3 (Hold priority).
