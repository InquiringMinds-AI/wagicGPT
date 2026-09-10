# Wave 74 — engine-narration seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260909-104713-final`, **MIXED BINARY**:
18 games (36 seats, 706 turns) on `archives/wagic-60e9fe2ad-w74step1b` (source `1d1d3744d`,
CC+CD+CE+CF+CG) — called **CG** below; 3 rerun games (152v125, 125v126, 125v123; 6 seats,
208 turns; `start_epoch >= 1788998000`, seat-log prefixes `1788998133/1788998153/1788998173`)
on `archives/wagic-76c5fab24-w74step1c` (source `aca5e06f8`, + CH's action-identity hold key) —
called **CH**. `--thinking on`, `WAGIC_GPT_TIMEOUT=900`, no operator token ceiling.
Every hold, chain and loop measurement below is split by binary. Read-only: no build, no run,
no git, nothing written outside `wave74/`.

**What I did NOT check.** No deck-guide reading and no strategy verdicts (deck seats own those).
No Scryfall/Gatherer calls — card facts come from the rendered prompt and, where a mechanism
mattered, from `src/AIPlayerGPT.cpp` / `src/AIPlayerBaka.cpp`. PARSETEST/suite numbers are quoted
from the lane files, never re-run; no lane fixture or probe was re-run. I did not read 2,396
reasoning bodies — every prediction whose subject is a reasoning-body claim is marked UNTESTED.
I did not measure the F1 "pending deadlock" hazard directly (no per-tick trace exists in the
translog); the proxy is 0 hung games and 0 `[combatentry]` lines. I did not re-derive
`serializeGameState()`, so the sibling-window verdict is code-read, not measured. I did not
diff every hold-bracket render for drift classes beyond the two I name. The mixed binary means
**no hold/chain figure in this corpus is a 21-game figure**; the CH sample is 3 games.
`archives/` binaries were not run.

---

## 1. Corpus census

| | wave 74 (all) | CG (18 games) | CH (3 games) | wave 73 |
|---|---|---|---|---|
| seats / games | 42 / 21 | 36 / 18 | 6 / 3 | 42 / 21 |
| completed naturally | 42 gameend, 21 `results.tsv` rows, 0 hang, 0 crash | 36 / 18 | 6 / 3 | 42 / 21 |
| **decisions (records carrying a `prompt`)** | **2,396** | 1,803 | 593 | 2,338 |
| by kind | ask 1,866 · priority 367 · attackers 93 · discard 40 · blockers 17 · reveal 12 · bottom 1 | — | — | ask 1,757 · priority 416 · attackers 83 · discard 46 · blockers 23 · reveal 10 · bottom 3 |
| `kind: recovery` (no prompt) | 4 | 4 | 0 | — |
| summed gameend turns / **decisions per turn** | 914 / **2.62** | 706 / 2.55 | 208 / 2.85 | 928 / 2.52 |
| same, ask seam only | 2.04 | — | — | 1.89 |
| fallbacks | **5** | 5 | 0 | 0 |
| `transport` | `curl=0,http=200,empty=0` on 2,394; `curl=0,http=400,empty=1` on **2** | — | — | clean on 2,338 |
| `async_drops` | 84 (`answer_replaced` 0 on all 2,396) | 70 | 14 | 43 |
| `deadline_pct` | p50 8.9 · p95 21.1 · max 54.4 | — | — | p50 7.1 · p95 19.1 · max 39.8 |
| latency | p50 **80.5 s** · p95 190.3 · max 489.7 | — | — | p50 64.0 · p95 172.4 · max 358.6 |
| `reasoning_chars` | min 1,387 · p50 5,980 · mean 6,404 · max 24,734 | — | — | 737 / 5,931 / 6,343 / 24,849 |
| game length (turns) | mean 21.8, median 19, max 52 | — | — | mean 22.1, median 18, max 77 |
| wins | 152 5/6 · 146 5/6 · 162 4/6 · 123 3/6 · 126 2/6 · 130 1/6 · 125 1/6 | — | — | — |

**⚠ The brief's census line is the ASK SEAM again** (as wave 73 flagged, and the note did not
take). `1,866 decisions`, `86 off_protocol_bytes>0 (4.6%)`, `93 plan_line_missing (5.0%)`,
`decisions/turn 2.04` are, to the digit, the statistics of the 1,866 `kind: ask` records alone.
Corpus-wide: **2,396** decisions, **98** off-protocol (4.1%), **106** plan-line-missing (4.4%),
**2.62** decisions/turn. Also: the brief says "1 http_error" — there are **2** (both quoted below),
and both cost a decision.

**Reasoning (invariant 000).** 2,396 / 2,396 records carry `thinking: on` and a non-empty native
`reasoning`. **The corpus is valid under 000.** Rescue markers: `reasoning_forced_close` 12 =
`reasoning_budget_hit` 12 = `retry: 1` 12, and `phase2_answer_recovered` 10 +
`phase2_answer_missing` 2 = 12 — the wave-71 identity (`recovered + missing == closes`) holds
**against the records**, but the stderr issued **17** `unclosed <think>` closes (see HIGH-3).

**Reply shape.** `protocol_replies` 2,396. `plan_line_missing` **106 (4.4%)**, `off_protocol_bytes>0`
**98 (4.1%)**, overlap 96. By seam (plm / offp): ask 93 / 86, attackers 10 / 9, discard 2 / 2,
blockers 1 / 0, priority 0 / 1. Attackers is again the worst rate (10/93 = **10.8%**, improved
from wave 73's 14.5%). Off-protocol byte weight: n=98, sum 7,104 B, median 58, max 621.
**Shape**: of the 108 distinct offending replies, **76** are the wave-72/73 class (a bare plan
sentence with the `PLAN:` label dropped, e.g. `Play the Mountain to increase mana, then pass as
hand is empty.`) and **31** are a NEW and worse class — the answer line alone, no plan of any
kind (wave 73: 4). `action_before_plan_replies` 0, `answer_replaced` 0 on all 2,396.
`post_answer_overrun>0` on **1** record.

**Ceilings / truncation.** `max_tokens_answer`: ask 288 (1,856) / 400 (10 retries); priority 288
(366) / 400 (1); attackers 256 (92) / 400 (1); blockers 256 (16) / 400 (1); discard, reveal 256;
bottom 512 (1). `reply_truncated` on **1** record, quoted below. `dropped_assignments` 0 on all
17 blockers records, so BS F5's doubled allowance is **UNTESTED** for the fourth wave.

**The five fallbacks, verbatim.**

1. `1788968842-…deck123-…vs-deck152` **seq 64** (ask, t14) — `http_error`, `http_status 400`,
   `transport curl=0,http=400,empty=1`, `reasoning_chars 20,039`, latency 448.9 s,
   `reasoning_forced_close true`, `max_tokens_answer 400`. `reply` = `''`,
   `chosen_text` = `<refused: http_error>`. **Genuine, and it is O27 recurring**: this is the
   phase-2 forced-close prefill being rejected. It is one of the two `phase2_answer_missing`.
2. `1788968865-…deck126-…vs-deck152` **seq 48** (**attackers**, t20) — identical shape
   (`http 400`, `reasoning_chars 19,605`, latency 322.7 s, forced close). **Genuine, O27.**
   This one put an ATTACK DECLARATION on the heuristic in a corpus game.
3. `1788968863-…deck130-…vs-deck146` **seq 16** (ask, t6) — `unparsed_reply`,
   `reply_truncated 1`, `reasoning_chars 23,318`, latency 489.7 s (the corpus max).
   Reply verbatim: `"\n\nPLAN: Hold priority, then activate Talisman for {R}, cast Siege"`.
   **Genuine**: the reasoning budget ate the answer line. (This record also carries the one
   `async_drop_events` that coincides with a fallback; the drop was re-asked, the truncation is
   the cause.)
4. `1788998153-…deck126-…vs-deck125` **seq 154** (ask, t52) — `unparsed_reply`. Reply verbatim:
   `"\n\nPLAN: Play a land to expand mana base and prepare to cast Chromatic Lantern. CHOICE: 1 (Play Sunpetal Grove)"`.
   **NOT genuine — this is a parser defect (MED-1).** The model wrote both labelled parts; it
   put them on ONE line. The answer is unambiguous, the label appears exactly once, and the
   decision was thrown to the heuristic anyway.
5. `1788968869-…deck125-…vs-deck162` **seq 148** (ask, t27) — `noop_row_zero_reask`, reply
   `"PLAN: Survive the draw step at 6 life. Do not cast Sphinx's Revelation due to active draw punishers.\nCHOICE: 10 (X = 0)"`.
   **Genuine.** The re-ask line is well formed (`[RE-ASK] You chose row 10 ("X = 0"), whose own
   note on this list says it does nothing right now. Answer again: that row if you meant it
   anyway, or the number of the row you want instead.`) and the model was free to repeat itself;
   at seq 149 it answered `CHOICE: 7 (X = 7)` (`parse_note: plan_contradicts_noop_row_recovered`).
   The seat's answer was not overridden. See LOW-2 for the ordering problem behind it.

**The `post_answer_overrun` record.** `1788968851-…deck146-…vs-deck125` seq 46 (priority, t19),
`parse_note extra_answer_line;latched_row_mismatch`, overrun 54 B. Reply verbatim:
`"\n\nPLAN: Animate both Hives to attack for lethal.\nCHOICE: 1 (becomes beholder Hive of the Eye Tyrant #1)\nCHOICE: 2 (becomes beholder Hive of the Eye Tyrant #2)"`
— two answer lines for a one-answer window; the parser took the first.

**`askreplay/` sidecar.** 26 files, **584 records** = summed `ask_replays_reserved` 584, exactly.
`why`: `cache_replay` **583**, `repeat_ask_reserved` **1**. `ask_replays_refused` 0.
max `replay_run` 23 (cap 64); `replays_this_game` non-decreasing in all 26 files.

**`plan_steps_done`.** `(done,count)`: (0,1) 1,118 · (1,2) 920 · (1,3) 257 · (1,4) 67 · (1,5) 12 ·
(2,3) 9 · (0,0) 7 · (0,2) 3 · (2,4) 2 · (1,6) 1. The pointer reaches ≥2 on **11** of 2,396
(wave 73: 9 of 2,338). Unchanged: the model re-states its plan far more often than it walks it.

---

## 2. REGRESSION WATCH — the wave-74 changes

### (a) The CG/CH loop family

**Asks per (turn, rendered phase), every seat.** Only **4** phases in the whole corpus exceed 10:

| binary | seat | turn / phase | asks | verdict |
|---|---|---|---|---|
| **CG** | `1788968869-deck125` (125v162) | t27 **Draw** | **35** | **RE-PUT WINDOW — HIGH-1** |
| CG | `1788968851-deck146` (146v125) | t21 Main 1 | 13 | genuine chain (rows 6→3→12→4→5, cards cast, walkers activated, opp life 5→10) |
| CH | `1788998173-deck123` (123v125) | t22 Main 2 | 13 | genuine chain (Greaves cast + equipped, Doomsayer tapped, rows 7→2→81) |
| CH | `1788998173-deck123` (123v125) | t24 Main 1 | 13 | genuine chain (Bloodline Keeper cast, tokens made, rows 7→41→45) |

Per-seat maximum asks in one (turn, phase): **CG** 35, 22, 14, 13, 13, 13, 11, then ≤9 (median 3);
**CH** 17*, 13, 10, 7, 7, 5, 2 (*aggregated across un-headered phases). The wave-74 41-ask and
38-ask phases do **not** recur on the CH binary. On the CG binary the 38-ask shape **does**.

**`declined this exact list N times` maxima per seat** (wave 73: ≤3):
**CG** 23, 13, 12, 10, 7, 7, 6, 5, 4, 4, 4, … (median 1); **CH** 7, 5, 4, 4, 3, 0.
CG's own prediction ("no seat log carries a count above 15") **FAILS** on one seat (23).

**HIGH-1 — the CG binary still re-opens a held window on annotation alone, and the seat nearly
killed itself doing it.** `1788968869-ai_baka_deck125-…-vs-…deck162.jsonl`, turn 27 Draw step,
**35 model calls in one phase**, `Hold priority` **taken 18 times** and re-opened every time.
The seat's own life falls **26 → 2** one point per link (Ob Nixilis + Underworld Dreams punishing
its own draws). Diffed byte for byte off the seat's own `options_text`, the three/four rows are
the same rows at every window, and the ONLY deltas are annotations:

* seq 266-class delta (seq 138 → 139): the O11 tag ARRIVES —
  `{this same question will be asked again this turn: … you have already declined this exact list 1 time this turn}`
  — and the O8 bracket restates the stack: `counted from the 8 life the 18 damage ALREADY ON THE STACK leaves you on` → `the 17 damage`;
* seq 152 → 153: the O11 count `2 times` → `3 times`, nothing else;
* seq 171 → 172: J9's `{answers the stack: NO - 4 damage is ALREADY ON THE STACK … puts you at -1 and KILLS you}` appears.

This is CH Instance 2 reproducing on the un-fixed binary, which is the cleanest possible
confirmation of CH's diagnosis; it is HIGH because **18 of the 21 corpus games are on that
binary** and the shape is therefore live in the corpus under review.

**Cost/benefit of the CH key, split by binary** (stderr trace counts; per-seat figures in brackets):

| | CG (36 seats) | CH (6 seats) |
|---|---|---|
| `took the hold row` | 282 (7.8/seat) | 124 (20.7/seat) |
| `hold re-opened …` | 249 (0.88 per hold taken) | 78 (**0.63** per hold taken) |
| `hold_windows_skipped_cast` | **141** (3.9/seat) | **927** (154.5/seat) |
| `hold_windows_skipped_priority` | 114 | 80 |
| windows suppressed per hold taken | **0.9** | **8.1** |

CH's prediction ("`hold_windows_skipped_cast` rises well above 141") **PASSES categorically** —
a ~40× rise per seat.

### (b) `hold_released_turn` = 70 — complete enumeration, **no hold outlived its holder's untap**
All 70 release lines parsed out of the stderr (each names the turn taken, whose turn it was, and
the turn now). Every one released at the predicate's minimum span:

| binary | own-turn hold, released at heldTurn+2 | their-turn hold, released at heldTurn+1 |
|---|---|---|
| CG | 13 | 15 |
| CH | 18 | 24 |

Zero releases with a span above the minimum → zero holds survived an untap. **BY N2a / CB F2 /
CH's untap bound: PASS.** Note the skew: 42 of the 70 releases are in the 3 CH games — the CH
key makes holds actually last long enough to need releasing.

### (c) O2 — the declare-attackers window
Method: for every own turn on which some prompt's board header reads
`N of them able to attack right now` with N > 0, does an `attackers` record exist for that turn?

* **76** such own turns corpus-wide. **5** have no attackers record. **4 of the 5 are the game's
  final turn** (the game ended in that turn: deck123 t12, deck126 t19, deck130 t20 — all `game
  ended t = that turn`). The remaining two are legitimate no-attack turns, traced record by
  record: `1788968874-deck152` t19 — the header reads `able=2` at Upkeep/Main-1, then the seat
  casts Katilda and Elite Spellbinder and the same header reads **`able=0`** at seqs 37/38 (mana
  tapped, bodies summoning sick); `1788998173-deck123` (CH) t22 — `able=0` at every window before
  combat, `able=1` only at Main-2 windows *after* combat, on a token Doomsayer had just made.
* **`[combatentry] … PASSING declare-attackers` fires 0 times** in all 21 stderr files. The trace
  exists in the shipped binary (`src/AIPlayerBaka.cpp:5921`, dev-gated) and the binary is a dev
  build (its sibling DEVLOGS traces print in the same files).

**Verdict: 0 lost declare-attackers windows, 0 traces, against wave 73's three named misses.
O2 / F1 PASS on the outcome.** The pending-deadlock (F1) half is UNTESTED directly: 0 hung games
and 0 traces is consistent with it but the translog has no per-tick record to prove it.

### (d) O19 — the ascending X ladder
**33 ANNOUNCE_X rung menus** (≥2 `X = N` rows) across deck125 (Sphinx's Revelation) and deck130
(Starstorm). Every one renders `X = 1, 2, … capX`, then `X = 0`, then `Decline`. On **33 / 33**
the option number equals the X it announces for every rung above zero, and on **33 / 33** the
`chosen_text` is the row the reply's number names — with no name rescue needed
(e.g. `CHOICE: 9 (X = 9)` → `X = 9 …`, `CHOICE: 12 (X = 12)` → `X = 12 …`).
**`echo_index_conflict` on an ANNOUNCE_X record: 0** (wave 73: 2, both on this menu).
The corpus's two remaining `echo_index_conflict` notes are ordinary menus
(`1788968851-deck146` seq 49 and `1788968865-deck126` seq 66, the latter a `CHOICE: 0 (Hold priority)`).
`smallest X first` (the climbing range row) renders 14 times.
**O19 PASS, 33/33 — the lane's own highest-risk change is the cleanest result in the wave.**

### (e) O4 — the MDFC land-drop menu
**340** dedicated land-drop menus (a `Play no land right now` row). **44** carry an MDFC pathway
row. **44 / 44** carry `{TWO FACES, ONE CARD: … the next window asks which face enters the
battlefield … Choose the face there, not here.}` — no misses. **32** of the 44 are followed by a
face menu, i.e. the double-ask persists exactly as CD said it would. **PASS on the render.**
The reasoning-body half of CD's prediction ("stops treating the first answer as a face
commitment") is **UNTESTED** — I did not build a reliable classifier over 32 reasoning bodies.

### (f) O11 — the tagged decline row
**561** windows render the O11-tagged plain decline/pass row. The tagged row was **taken 284
times = 50.6%** (wave 73: 40% of 136; waves 70/71/72 ≈ 20/26/24%). CE predicted **< 30%**.
**FAIL, and the metric moved further the wrong way.** The tag does damp with N —
takes by N: N=1 134/239 (56%), N=2 65/115, N=3 36/73, N=5 8/23, N≥9 6/23 (26%) — but the
headline rate is up. Confound worth stating: the population is 4× wave 73's, and much of the
growth is the HIGH-1 loop, so the two rates are not measured over comparable boards.
Set against its costs — O11's rising count is the byte that killed the first wave-74 run (CG) and
the byte that re-opens the hold on the CG binary (HIGH-1) — the tag is a net negative so far.

### (g) O7 / O7b / O7c — the stop-and-plan family
* **O7 PASS.** deck123 carries whose ORIGINATING `PLAN:` line carried a `stop=`/`M=` pair: **20**;
  carries whose rendered `THE PLAN YOU LAST STATED` still contains the numbers: **20 = 100%**
  (CE's bar was ≥95%; wave 73: 0 of 50).
* **O7's own discharge condition fires.** CE wrote: if the intra-turn stop still oscillates by
  more than ±5 while every carry shows its counts, the defect is the pilot's arithmetic.
  `1788968875-deck123` turn 11 states, in order, `stop=25, 46, 46, 25, 26, 26, 26, 22` — a spread
  of 24. **O7 is discharged engine-side and routes to the guide/pilot.** (The CH game
  `1788998173-deck123` is stable: `40` ×9, one `80`, then `40`, then `43` ×7.)
* **O7b PASS.** 38 NON-repeat Create rows carry `{right now: M=…, your stated stop=…}`
  (wave 73: 0); 8 repeat rows carry it. First example `1788968846-deck123` seq 61.
* **O7c PASS.** 6 renders, all in `1788998173-deck123` seqs 196-210:
  `your stated stop was set when their life was 37; it is now 40` — and the record's own
  `opp_life` is 40 at every one. Arithmetic correct 6/6.
* **`stop_reached_windows_skipped` 30 vs wave 73's 366 — the stop STILL PARSES; the answer is
  "fewer loops".** The 30 collapses carry exactly 4 distinct stated stops, all traced in the
  stderr as the seat's own count: `stated stop (23)` ×13, `(40)` ×9, `(43)` ×4, `(24)` ×4 = 30
  exactly. Wave 73's 366 was one turn (294 of them). The parse is alive: 46 `your stated stop=`
  renders in 38 prompts, and the counters split CG 17 / CH 13.

### (h) O10 — `main_phase_windows_skipped` 72 is NOT the honest figure
All 72 skip traces parsed out of the stderr (`own main phase (turn N, phase P) ENDED with NO
casting window - the instant-speed response arm answered it`), resolved to a seat by deck name,
and cross-referenced against that seat's own translog for a
`Casting decision (Main phase 1|2, YOUR turn)` header on the same turn:

* **44 honest, 28 FALSE (39%)** — the phase DID put a Casting decision to the model.
  Worked example: `1788968851-deck146` t3 Main-1 is counted skipped, and seq 6 of that seat's
  translog carries `Casting decision (Main phase 1, YOUR turn): which card do you cast now`.
  The 28 fall on deck152 (16), deck146 (9), deck123 (2), deck162 (1).
* Wave 73 was 233 of 375 (62%). Absolute cost down 8×, rate down to 39%. **CD's prediction of
  0 FAILS**; the deck126 half of it (a hold-heavy seat no longer carrying a proportional skip
  count) **PASSES** — deck126 seats contribute **0** of the 72.

### (i) New render clauses stating a FALSE fact

| clause | renders | true? |
|---|---|---|
| **O1 ghostform gloss** | `1x ghostform` on **81** prompts; the FULL gloss (`killing it is not removal`) **162** (event line + board line); `(a marker only - the ability that returned it is gone)` **0** | **TRUE 81/81 and INVERTED from wave 73's 56/56 false.** No ability-removal effect exists anywhere in the corpus (`loses all abilities` / `Sudden Spoiling` / `Turn to Frog` / `Merfolk Trickster` = 0 hits in all 21 stderr; the 89 `Kaya the Inexorable` hits are the grantor), so the grant was live on every render. The marker branch remains **unreachable and untested**, exactly as CC's own weakest-evidence says. |
| **O16 `(keeping 7)`** | 43 pre-game mulligan prompts, **42** carry it | **PASS 42/42** — 21 games × 2 seats = 42 first windows; the 43rd is a genuine post-mulligan window, which correctly carries the wave-47 sentence instead. Wave 73: 0. |
| **O5 converter arithmetic** | 93 ATTACK TOTAL records; **8** on a board with a converter, **2** of those with the seat's OWN converter *and* a lifelink attacker offered | **PASS 2/2.** `1788968865-deck126` seq 64: `1 of that damage is LIFELINK, and your LIFE-TO-DAMAGE CONVERTER … so with none of them blocked they are at 3` (combat-only figure 4). seq 106, opponent at 1: `… they are at -1` — the converter turns a "NOT lethal" 1-power swing into the kill, which is the exact inversion of the wave-73 repro, and deck126 won that game on turn 25. The 6 `theirs`-converter records correctly print nothing. **F5's fixed-rate half is UNTESTED**: `Cliffhaven Vampire` appears in 0 prompts; every converter in the corpus is a proportional Sanguine Bond. |
| **O9 own-clock tag** | **325** renders; the stated rate and `reaches 0 in K more turns` agree with `ceil(opp_life / rate)` on **325 / 325** | **Arithmetic PASS; the F7 gate FAILS in the non-combat direction — MED-2.** 7 renders print while the seat is at ≤5 life, and 112 of 325 (4 games) are in games the seat then LOST — F7's own stated falsification. Worst case is HIGH-1's phase: at `1788968869-deck125` seqs 169-175 the seat is at 5→2 life with lethal on the stack and the decline row still reads `your Staff of Nin deal 1 damage a turn … the opponent reaches 0 in 18 more turns, with no card spent`, in the SAME prompt as `{answers the stack: NO - 4 damage is ALREADY ON THE STACK … puts you at -1 and KILLS you}`. F7 gates on latched COMBAT damage only; this is stack damage, which its weakest-evidence #6 named. |
| **O8 keep-X line** | 238 X-cast rows carry it; 100 print the categorical `no X on this row leaves it payable this turn, not even X=0`; 0 print a negative keep-X | **Arithmetically defensible, editorially broken on 27 of 238 (11%) — MED-3.** On those 27 the clause names the SAME CARD as the row being cast: `Cast Sphinx's Revelation {u}{u}{w}{x} … Sphinx's Revelation {u}{u}{w}{x} in your hand needs 3: no X on this row leaves it payable this turn, not even X=0` (`1788968838-deck125` seq 6). It is a *second copy* in hand (the same prompt's hand line reads `copy 1 of 3`), so the arithmetic is true — but nothing in the clause says so, and the sentence reads as the row forbidding its own cast. F6's own class (a categorical refusal reversed by colourless mana) did not recur. |
| **O7c stop-vs-life note** | 6 | TRUE 6/6 (above). |
| **O15 loyalty consequences** | `the body this makes arrives summoning sick` 10 · `an emblem is permanent - nothing in the game removes one once you have it` 5 · `you already control 1 emblem - a new emblem is ADDED to them` 8 | **TRUE on this corpus.** No walker row carries two `{right now: ` brackets. **F8 UNTESTED**: no haste granter (Fervor/Anger/Hall of the Bandit Lord) appears on any board in the corpus, so the false-clause class F8 fixed never had a chance to fire. |
| **O20 casting-hold clause** | `that is a different question at a different seam and you will still be asked it` on **1,252** prompts | Renders as designed. The reasoning-body half ("no body treats the next priority window as a bug") is **UNTESTED**. |
| **O21(b) worked example** | **2,424** worked examples, **0** containing `(menu text:` | **PASS.** (The 844 `(menu text:` renders in the corpus are real rows, not examples.) |
| **O21(c) `[second copy:`** | 25 renders in 3 shapes: the *stacking* shape 9/9 with the price half; `Intruder Alarm` "a second copy changes nothing" 2; `Chromatic Lantern` "adds only its own abilities" 14 | **PASS** — the price half is on 100% of the class it was written for. |
| **O14 mana bill** | rows carrying separate adjacent `{leaves …}` + `{paying this taps: …}` groups: **0** | **PASS.** |
| **the O6 hold-check bracket** | `[hold check: N row(s) above are new … a row moving is what re-opens a hold, so a hold taken here lasts only until one moves again]` **1,210** (CG 859 / **CH 351**); `every row above was also on the menu …` 368 (CG 252 / CH 116); longest run `15 windows in a row now` | **On the CH binary this sentence is FALSE — MED-4.** CH deliberately left `holdReopenNote` on the RENDER key while the latch moved to the ACTION key, so on 351 CH renders the bracket tells the model a row moved and the hold will therefore break, while the latch will in fact hold. This is CH's own weakest-evidence #7 realised in a rendered sentence. |

### (j) `async_drops` = 84 — complete enumeration, **no decision lost**
84 records carry `async_drop_events` (`answer_replaced` false on all 2,396). Reasons:
`casting/question and board/re-asked` **75**, `casting/board/re-asked` 5,
`casting/question (or turn/phase)/re-asked` 3, `land-drop/question and board/re-asked` **1**.
**All 84 end `/re-asked`**, and all 84 records carry a real model answer with a positive latency.
Exactly **1** sits on a record that also took a fallback — `1788968863-deck130` seq 16, and its
fallback cause is the reasoning-budget truncation (fallback 3 above), not the drop. **0 of 84
decisions fell to the heuristic because of a drop.** The near-absence of land-arm drops (1 of 84)
is consistent with F1's per-arm slots; wave 73's 43 drops were on the shared slot.

---

## 3. Prediction adjudication

### lane CC (render truth)
| # | prediction | verdict | evidence |
|---|---|---|---|
| O1 | every live-grant ghostform line renders the FULL gloss; 0 render `a marker only`; the marker form appears only after an abilities wipe | **PASS (first half), UNTESTED (second)** | 81 prompts, 162 full-gloss renders, 0 marker renders, 0 ability-removal effects in the corpus |
| O3 | `FORCED SACRIFICE …` appears only on edict records; Silverquill Command's pump mode renders `TARGET CHOICE for Silverquill Command` | **PASS, thin** | 4 FORCED SACRIFICE renders, all `Tribute to Hunger` (deck152 seq 23, deck162 seqs 11/30, deck146 seq 33). Exactly **1** Silverquill Command target picker occurred (`1788968855-deck146` seq 24) and it carries `TARGET CHOICE for Silverquill Command - its "creature gains 3/3 and sacrifice creature" ability`. n=1 |
| O5 | every ATTACK TOTAL on a seat with its own converter + a lifelink attacker carries the LIFELINK clause; no seat told "NOT lethal" over a converter-lethal swing | **PASS, 2/2** | §2(i); the seq-106 window is the inversion of the wave-73 repro |
| O12 | on an Intruder Alarm board the able-count is strictly less than the creature count | **PASS, indirect** | 143 prompts render `doesn't untap during its controller's untap step`; **0** board headers count a tagged tapped-no-untap body as able. The 103-creature Intruder Alarm board itself did not recur, so the extreme case is untested |
| O15 | every `+N:` body row says summoning sick; every emblem row says cumulative/permanent; no walker row carries two `{right now: ` brackets | **PASS, 3/3** | 10 / 13 / 0 |
| O16 | `(keeping ` on 100% of pre-game mulligan prompts | **PASS, 42/42** | wave 73: 0 |
| O19 | `index_name_conflict` on ANNOUNCE_X drops to 0; `CHOICE: N (X = N)` resolves without a name rescue | **PASS, 33/33** | §2(d) |
| O20 | no reasoning body treats the following priority window as a bug | **UNTESTED** | render present on 1,252 prompts; reasoning bodies not read |
| O21 | 0 examples containing `(menu text:`; 0 bodies calling the example contradictory; the `[second copy:` price half on 100% of stacking renders | **PASS / UNTESTED / PASS** | 0 of 2,424; not read; 9 of 9 |

### lane CD (window model)
| # | prediction | verdict | evidence |
|---|---|---|---|
| O2 | `phase2_answer_recovered + missing` ≥ 80% of the `unclosed <think>` count | **FAIL (71%), 12× better than wave 73** | 10 + 2 = 12 records against **17** stderr closes. Wave 73: 2 of 33 = 6%. See HIGH-3 |
| O2 | no seat log shows a turn with a legal attacker, no declaration, and a `[combatentry] … armed=1` line | **PASS** | 0 lost windows of 76, 0 trace lines — §2(c) |
| O4 | every land-drop menu whose row is followed by a face menu carries `TWO FACES, ONE CARD`; the reasoning stops anchoring | **PASS / UNTESTED** | 44 of 44 tagged (32 followed by a face menu); reasoning half not measured |
| O6 | runs of consecutive priority windows differing only by projected life print `every row above was also on the menu …`, N advancing; the deck130 11-window chain collapses to ≤2 | **PARTIAL** | the `every row above …` form renders 368 times with runs up to `15 windows in a row now`; but the `N rows above are new` form still dominates 1,210 renders, and the deck130 chain shape did not recur in this deck pool, so the specific collapse claim is **UNTESTED**. The bracket's own sentence is now false on the CH binary — MED-4 |
| O10 | 0 counted `(turn, phase)` offered a Casting decision; a hold-heavy seat no longer carries a proportional skip count | **FAIL (28 of 72) / PASS** | §2(h) |
| O13 | `chain_acting_rows` non-zero wherever `chain_windows_collapsed` is 0; `chain_selfharm_rows / chain_acting_rows` ≥ 0.8 on punisher windows; `chain_windows_only_selfharm` stays low | **PASS on the first, FAIL on the ratio, PASS on the last — and the instrument is at the wrong seam (MED-5)** | `chain_acting_rows` 1,123 (CG 989 / CH 134), `chain_selfharm_rows` 560 (CG 558 / **CH 2**), ratio 0.50 overall, `chain_windows_only_selfharm` 6. Measured directly off `options_text` instead: of 2,396 windows carrying ≥1 acting row, **174 (7%)** offered nothing but acting rows whose own annotation says `this does nothing` — **CG 61 (3%), CH 113 (19%)**, by deck 126:85, 123:58, 146:31. The CH binary's 113 such windows produced `chain_selfharm_rows` **2**, confirming CH's structural finding: the O13 instrument runs at the PRIORITY seam while these windows are at the CASTING seam |
| O23 | `dropped after its X was announced` non-zero and equal to the count of `Decline - do not cast this after all` records | **PASS, exactly 5 = 5** | see the carried docket below |
| O26 | the tripwire does not fire on a healthy run; a wedged seat trips it | **PASS (no false fire) / UNTESTED (no wedge)** | the `-final` dir carries no `PILOT-STALL` marker; `wall_miss_events` / `wall_miss_no_retry` / `wall_miss_unrecorded` are **0** on all 42 gameends, so the silence classes never occurred and the predicate was never exercised in the positive direction |

### lane CE (stop/plan + pricing)
| # | prediction | verdict | evidence |
|---|---|---|---|
| O7 | ≥95% of deck123 carries keep both numbers | **PASS, 100% (20/20)** | §2(g); the ±5 oscillation clause **fires**, so O7 is discharged to the guide lane |
| O7b | the `{right now: M=` clause on ≥1 non-repeat Create row | **PASS, 38** | wave 73: 0 |
| O7c | the `was set when their life was` sentence appears when the totals differ | **PASS, 6/6 correct** | |
| O8 | the `needs N` clause renders on every X row with a castable hand card; announced X exceeding the printed keep-X falls below 5 of 5 | **PASS on the render / UNTESTED on the second half** | 238 rows carry it; I did not build the announced-X-vs-keep-X cross-tab (the keep-X is only printed when a keep candidate exists, and 100 of the 238 print the categorical refusal instead). See MED-3 |
| O9 | the clause renders on every decline row of a game where the seat controls a `{T}:damage:` source and leads on life; deck125 takes 0 Emrakul casts under a printed LOOP COMPLETE | **PASS on the render / PASS on the behaviour** | 325 renders; `Emrakul` cast under a printed `LOOP COMPLETE`: 0 this corpus (wave 73: 1). But see MED-2 — the clause also renders where the seat is dying |
| O11 | takes of the tagged decline row fall below 30% | **FAIL — 50.6%** | §2(f). Per CE's own rule the item now goes to the guide lane with this measurement |
| O14 | 0 rows carry both groups separately | **PASS, 0** | |
| O17 | `repeat_count_missing` from a verbatim short-name copy falls to 0 | **FAIL, 1 of 1** | `repeat_count_missing` fell 4 → **1**, and the survivor is exactly the shape the item was written to kill: `1788968875-deck123` seq 58, reply `CHOICE: 3 (Create vampire with Bloodline Keeper, repeated then stop)` — the row's NEW short name, copied verbatim, still refused and re-asked. `repeated N times, then stop` renders 0 times, so the rename landed; the answerability did not. Also 2 `repeat_count_under_two` notes on `x1` answers |
| O18 | the largest deck123 priority prompt falls below 40,000 B with the option count unchanged; 0 answers fail to resolve inside a collapsed range | **PASS on the size / UNTESTED on the count** | largest prompt corpus-wide **34,315 B** (`1788968865-deck126` seq 67, 39 options), wave 73: 104,131 B. The 411-option window did not recur (largest option count this corpus is 81), so the "411 stays 411" half is untested; 0 unresolved answers |
| O25 | the counter is non-zero on deck162's seat | **PASS, 3, all deck162** | all three quoted: `1788968846-deck162` seq 10 (`Master of the Feast`, plan `Cast Dictate of Kruphix in upkeep, play land, cast Master of the Feast in main phase 1`); `1788968848-deck162` seq 20 (`Teferi's Puzzle Box`, plan `… Next turn: cast Master of the Feast and Teferi's Puzzle Box, attack with Master`); `1788968867-deck162` seq 27 (`Teferi's Puzzle Box`, plan `Win on their draw step with Dictate.`). All three took a `Cast Dictate of Kruphix … {reserve: TAKE THIS ROW …}` row |

### lane CG (the ask-cache key)
| # | prediction | verdict | evidence |
|---|---|---|---|
| CG | `ask_replays_reserved` and `identical_ask_answers_reserved` both return to ≥20 per gameend | **PASS / FAIL** | `ask_replays_reserved` 584 = **13.9 per gameend** (wave 74 pre-fix: 0.15) — the replay path is restored but still below the bar; `identical_ask_answers_reserved` **1** |
| CG | no seat log carries a `declined this exact list N` above 15 | **FAIL** | one CG seat at **23**; CH max 7 |
| CG | if a count walks past 40 the WINDOW LOOP tripwire stops the run in a minute | **UNTESTED, correctly** | no count reached 40; the `-final` dir carries no `WINDOW-LOOP` marker (the parent `matchups-20260909-104713` does — that is the false fire CH fixed) |

**Which replay path each counter names, and whether 1 is a regression.** From
`src/AIPlayerGPT.cpp` (~:38189 and ~:38242): the ask seam consults **two** caches in order.
(1) `mAskCache`, keyed on `askKey` = board key + rendered tail — a hit increments
`mAskReplaysReserved` and logs `cache_replay`. (2) only if (1) misses,
`repeatAskAnswerStands(mRepeatAskKey …)`, keyed on turn + phase + question + rows **and gated on
`mRepeatAskPlan == mCurrentPlan`** — a hit increments **both** `mAskReplaysReserved` **and**
`mRepeatAskAnswersReserved`, and logs `repeat_ask_reserved`.
So `ask_replays_reserved` = both paths, `identical_ask_answers_reserved` = path (2) only.
The sidecar splits 583 / 1, which matches the two counters exactly.
**1 is not a regression**: wave 73's 999/263 split was path (2) doing the work because path (1)
kept missing; with the CG key stripped of the O11 clause path (1) now hits first (263 → 583) and
returns before path (2) is reached, and path (2)'s plan-equality gate rarely holds anyway (1,118
of 2,396 records carry a brand-new one-step plan). The *total* fell 1,262 → 584 chiefly because
1,262 windows were suppressed by the hold latch before an ask was ever built. **This is a
code-read inference plus the sidecar split, not an instrumented trace.**

### lane CH (the action-identity hold key)
| # | prediction | verdict | evidence |
|---|---|---|---|
| CH | `hold_windows_skipped_cast` rises well above 141 | **PASS, categorically** | 927 in 3 games (154.5/seat) vs 141 in 18 games (3.9/seat) |
| CH | no seat log shows more than ~5 asks in one (turn, phase) while a life total moves monotonically; the 41-ask and 38-ask phases do not recur | **PASS on the CH binary, FAIL on the CG binary (which is 18 of 21 games)** | CH's three >5 phases (13, 13, 10) all move the BOARD, not just life. CG's `1788968869-deck125` t27 is 35 asks with life falling monotonically 26→2 — HIGH-1 |
| CH | `hold_released_turn` stays non-zero | **PASS, 70** | §2(b) |
| CH | the WINDOW LOOP tripwire does not fire on a chain | **PASS** | no marker in the `-final` dir; the 35-ask CG phase moves `my_life` every window, so the K-unchanged-board term would not fire on it either |
| CH | O13 needs the casting seam before its number means anything | **CONFIRMED** | see O13 above: CH binary, 113 all-do-nothing casting windows, `chain_selfharm_rows` 2 |

### lane CF (the Astra fixes)
| # | prediction | verdict | evidence |
|---|---|---|---|
| F1 | forced closes ARMED and phase-2 requests SENT converge | **PARTIAL — 12 of 17 (71%)** | HIGH-3; wave 73 was 2 of 33 |
| F2 | a decode-garbage retry armed from a land-drop window launches on the land arm's next poll | **UNTESTED** | 0 `isDecodeGarbage` retries in the corpus (all 12 retries are `reasoning_forced_close`); 1 of 84 async drops is on the land arm and it was re-asked |
| F3 | a run whose seats all answer never trips the banner; a silent seat trips it within K | **PASS / UNTESTED** | no false fire; `wall_miss_*` and `timeout` fallbacks 0, so the positive direction never occurred |
| F4 | the banner's classes and the tripwire's classes agree | **UNTESTED** | no silent-class fallback occurred; the banner had nothing to count |
| F5 | an ATTACK TOTAL on a board with a FIXED converter no longer claims a kill it cannot deliver | **UNTESTED** | `Cliffhaven Vampire` in 0 prompts; every converter in the corpus is proportional (Sanguine Bond) |
| F6 | X rows that previously refused categorically now name a keep-X; the seq-308 strand shape stops being licensed | **PARTIAL** | 138 of 238 rows name a keep-X, 100 still print the categorical refusal; 0 print a negative keep-X, so F6's arithmetic inversion did not recur. The strand behaviour is a reasoning-body claim, **UNTESTED** |
| F7 | the tag appears only beside a source that will untap, and never where the seat faces latched lethal | **PASS on the untap half / FAIL on the lethal half** | 0 clock tags name a `DOESNOTUNTAP` source; 7 print at ≤5 life and 112 of 325 are in games the seat lost — MED-2 |
| F8 | a seat with a haste granter prints no summoning-sick clause | **UNTESTED** | no haste granter on any board in the corpus |
| F9 | a carried plan's stated number is byte-identical across windows regardless of formatting | **UNTESTED** | 0 plans in the corpus write a thousands separator; the largest stated count is `stop=106`-class (max seen: 80) |
| O19 | the render→answer→apply round trip holds | **PASS, 33/33** | §2(d) |

### Carried docket
* **The abandoned-{X} payment loss — DISCHARGED, first fire in four waves.**
  `dropped after its X was announced` prints **5** times, and exactly **5** records carry a
  `chosen_text` beginning `Decline - do not cast this after all` — an exact match, which is
  O23's own falsification test passed. The figure the docket wanted, quoted verbatim from the
  five lines: **10, 11, 11, 7 and 14 mana** already paid, left floating and lost when the step
  ended — **53 mana across the corpus**, all five on `Sphinx's Revelation`, all deck125.
* **BS F1 (death-trigger lifegain).** Exquisite Blood renders in 15 seat logs, Sanguine Bond
  throughout; `125v126` ended 48-0 on turn 52 and `125v130` ran to 59 life on turn 48 — the
  enchantments move life. **PASS, indirect.**
* **BS F2 (Tribute's second edict).** 4 FORCED SACRIFICE renders, no two-edict turn traced.
  **UNTESTED**, fourth wave.
* **BS F3 / the land latch.** `declined_face_latches` **0** on all 42 gameends. **UNTESTED**,
  fourth wave.
* **BS F5 (blockers ceiling).** `dropped_assignments` 0 on all 17 blockers records; no
  `ceiling re-ask` line in any stderr. **UNTESTED.**
* **BS F6 (distinct-index re-ask).** no `distinct index` line in any stderr. **UNTESTED.**
* **N16's `main_phase_windows_skipped`.** adjudicated at §2(h) and MED-6: 28 of 72 still false.

### Other counters, for the record
`sibling_window_asks_skipped` **0** for the third wave — `gptHoldCoversSiblingWindow` still
requires the now-rows to be a subset of the held rows plus a matching board key, and no window
met both bars. `chain_windows_collapsed` **0** — `chainAutoPassApplies` requires
`!anyLegalAction` (`src/AIPlayerGPT.cpp:29207`), and **0** windows in the corpus had an empty
acting-row set, so the collapse is unreachable, which is the safe direction; nothing was lost.
`reserve_decline_windows_noted` 11 / `_skipped` 1 over 58 rendered `{reserve: ` clauses — same
healthy direction as wave 73. `repeat_annotated_takes` 2 (both `1788968863-deck130`, n=1 and
n=2 — the annotation round-trips). `identical_option_asks_resolved` 2. `menu_pass_no_progress` 0,
`put_gloss_stripped` 0, `engine_reveal_floor_picks` 0, `own_turn_windows_skipped` 599,
`mana_only_windows_skipped` 136. Blocker forecast: rows 118, multi 82, gang 6, collapsed 28.

---

## 4. Engine / render / narration defects

### HIGH-1 — the CG binary re-opens a taken hold on annotation alone; 35 model calls in one draw step while the seat drained itself 26 → 2
`1788968869-ai_baka_deck125-0x55a3814190b0-vs-ai_baka_deck162.jsonl`, turn 27, `Draw, YOUR turn`,
seqs 138-175. Same 3-4 rows at every window; `Hold priority` taken 18 times; every re-open is an
annotation, never an action (the three deltas are quoted at §2(a)). This is CH Instance 2
reproducing, and CH's fix is present in only **3 of the 21 corpus games**.
**Repro line (seq 152 → 153, the whole delta):**
`… you have already declined this exact list 2 times this turn}` → `… 3 times this turn}`.
**Action:** the CH binary is the one to run the next corpus on; nothing further is needed
engine-side, but no conclusion about hold behaviour may be drawn from the 18 CG games.

### HIGH-2 — two decisions, one of them an ATTACK DECLARATION, were served by the heuristic on an HTTP 400 from the phase-2 forced-close prefill (O27 recurring, now with a cost)
`1788968842-deck123` seq 64 (ask, t14) and `1788968865-deck126` seq 48 (**attackers**, t20).
Both: `reasoning_forced_close true`, `retry 1`, `max_tokens_answer 400`,
`transport curl=0,http=400,empty=1`, `reply ''`, `chosen_text <refused: http_error>`,
`reasoning_chars` 20,039 / 19,605, latency 448.9 s / 322.7 s. These are the two
`phase2_answer_missing`. Wave 73 logged this as O27 LOW with "mechanism untested"; it is now
2 of 12 forced closes (17%) and one of them decided a combat. The prefill request that the pilot
template rejects (`continue_final_message is set but the final message does not appear in the
chat after applying the chat template`) has no stderr trace in this corpus — **the 400 body is
not logged**, which is itself worth fixing before the mechanism can be diagnosed.

### HIGH-3 — 5 of 17 forced closes (29%) leave no decision record at all
Matched per game by deck pair: stderr `unclosed <think>` count vs records carrying
`reasoning_forced_close`.

| game | closes | records |
|---|---|---|
| `game-125v130-1788968835` | 1 | **0** |
| `game-126v123-1788968872` | 1 | **0** |
| `game-152v130-1788968870` | 1 | **0** |
| `game-126v130-1788968855` | 2 | **1** |
| `game-152v126-1788968861` | 3 | **2** |
| (the other 7 games) | 8 | 8 |
| **total** | **17** | **12** |

The record-side identity holds (`recovered` 10 + `missing` 2 = `closes` 12 = `retry` 12), so the
gap is 5 closes that armed a leg and left no trace on any record — CD's "third path" warning,
still open. It is why CD's ≥80% prediction lands at 71%. No attackers window was lost to it
(§2(c)), so the cost is unmeasured rather than known to be zero.
**Repro:** `/usr/bin/grep -c 'unclosed <think>' game-125v130-1788968835.stderr` = 1, and no
record in either seat log of that game carries `reasoning_forced_close`.

### MED-1 — a reply that carries both labelled parts on one line is thrown away
`1788998153-…deck126-…vs-deck125.jsonl` seq 154 (ask, t52), `fallback: unparsed_reply`.
Verbatim: `"\n\nPLAN: Play a land to expand mana base and prepare to cast Chromatic Lantern. CHOICE: 1 (Play Sunpetal Grove)"`.
The `CHOICE:` label appears exactly once, its number and short name are both legal, and the
decision still fell to the heuristic. The protocol's "two labelled lines" is a formatting rule;
this is the parser declining to find an unambiguous answer that is present. Invariant 000 forbids
telling the model it may write prose — it does not forbid the parser recognising a label that is
there. 1 of 2 `unparsed_reply` fallbacks in the corpus.

### MED-2 — the O9 clock promises 18 turns to a seat that is about to die to a non-combat source
`1788968869-deck125` seqs 169-175 (and 7 renders corpus-wide at ≤5 life; 112 of 325 renders are
in games the seat then lost). The decline row reads
`{the clock you already control: your Staff of Nin deal 1 damage a turn between them - at that
rate alone the opponent reaches 0 in 18 more turns, with no card spent}` in the same prompt as
`{answers the stack: NO - 4 damage is ALREADY ON THE STACK, and taking this row lets it resolve:
that puts you at -1 and KILLS you}`. F7's gate reads `mIncomingCombatTurn` only; the lethal here
is on the stack. F7's weakest-evidence #6 named this class; the corpus now has it.

### MED-3 — the O8 keep-X clause names the row's own card on 27 of 238 renders
`1788968838-deck125` seq 6:
`Cast Sphinx's Revelation {u}{u}{w}{x} … {no {leaves ...} count on this row: what it spends
depends on the X you announce at the next window, and your mana affords only X=0 right now.
Sphinx's Revelation {u}{u}{w}{x} in your hand needs 3: no X on this row leaves it payable this
turn, not even X=0}`. The keep candidate is a *second copy* (`copy 1 of 3 in your hand`), so the
number is true — the sentence is not. Nothing distinguishes the copy being cast from the copy
being priced, and the clause reads as the row forbidding itself. Cheap fix: say `another copy of`
when the keep candidate shares the row's card name.

### MED-4 — the hold-check bracket now contradicts the latch on the CH binary
`[hold check: 1 row above is new since the last window at this seam - a row moving is what
re-opens a hold, so a hold taken here lasts only until one moves again]` renders **351** times on
the CH binary (1,210 corpus-wide). Under the CH action key a rendered row can move without
re-opening anything, so on those 351 windows the second half of that sentence is false. CH left
`holdReopenNote` on the render key on purpose (its weakest-evidence #7); the consequence is a
rendered non-fact and it should get a scoped clause (`a row moving is what re-opens a hold — a
row that changes only in its annotations is the same row`).

### MED-5 — the O13 chain instrument measures the wrong seam, and now has corpus-scale evidence
On the CH binary, **113** windows offered nothing but acting rows annotated `this does nothing`
(19% of that binary's 593 windows carrying an acting row), and `chain_selfharm_rows` reads **2**
and `chain_windows_only_selfharm` **0** for the same three games. Corpus-wide the honest figure is
**174 / 2,396 = 7%** (deck126 85, deck123 58, deck146 31) against `chain_selfharm_rows` 560 /
`chain_acting_rows` 1,123. The counters cannot be read as the quantity they are named for until
the instrument runs at the CASTING seam as well.

### MED-6 — `main_phase_windows_skipped` is still 39% false
28 of 72 counted phases put a Casting decision to the model (worked example at §2(h)). Better
than 62%, still not a meter anyone can read off a gameend record. The residue is concentrated on
deck152 (16) and deck146 (9), which suggests a third arm that issues a casting ask without
calling `noteMainPhaseCastingOffered()`.

### MED-7 — the repeat row's short name is still un-answerable by itself
`1788968875-deck123` seq 58, reply `CHOICE: 3 (Create vampire with Bloodline Keeper, repeated then stop)`
→ `parse_note: repeat_count_missing`. O17 removed the `N` placeholder (`repeated N times, then
stop` renders 0 times) but the row's printed short name still cannot be copied as the footer
instructs. 1 of 4 wave-73 cases survives, and it is the exact one the item was written for.

### LOW-1 — 31 replies carry an answer line and no plan at all
Wave 73's class-G residue was 4 records; it is 31 here (of 108 offending replies; the other 76 are
the familiar bare-plan-sentence shape). `action_before_plan_replies` is 0 because there is no plan
to come before. Worth one line in the reply protocol only if the owner wants it — O22 is
owner-ruled report-only.

### LOW-2 — the X ladder's economics arrive after the cast is committed
`1788968869-deck125` seqs 147 → 148: the seat takes the cast row, and only the ANNOUNCE_X menu
then prints `the opponent's Ob Nixilis, the Hate-Twisted, Underworld Dreams punish every draw, so
those draws cost you 2 life - NET -1 life for this cast`. The seat's plan at 148 was
`Do not cast Sphinx's Revelation due to active draw punishers` — a decision it could no longer
take except through `Decline` (row 11), which it did not find. The `noop_row_zero_reask` refusal
was correct and recoverable; the ordering is the interface problem.

### LOW-3 — 2 `echo_index_conflict` notes survive outside the X family
`1788968851-deck146` seq 49 (`CHOICE: 3 (Cast Nadaar, Selfless Paladin)` resolved to index 4 by
name) and `1788968865-deck126` seq 66 (`CHOICE: 0 (Hold priority)`,
`hold_row_named;index_name_unique_name`). Both rescued by the name match, both cost nothing.
O19 removed the X-menu class; this is the residue.

---

## 5. The owner's lategame specimen

`wave74/lategame-specimen.txt` — `1788968865-ai_baka_deck126-…-vs-…deck152.jsonl` **seq 67**,
kind `priority`, **turn 26**, deck126 at 24 life vs 3, 39 options, prompt **34,315 B** (the
largest prompt in the corpus), `reasoning_chars` 5,493, latency 66.0 s, `deadline_pct` 7.3,
thinking on, CG binary. The seat chose
`Deal 1 damage with Staff of Nin #1 targeting the opponent {right now: takes 1 damage - they
would be at 2}`. Three-line header, then the full `prompt` field verbatim, nothing elided.
