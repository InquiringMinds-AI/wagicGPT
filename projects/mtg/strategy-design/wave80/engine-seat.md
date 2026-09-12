# Wave-80 engine-narration seat

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final`.
MIXED binary: 20 games on `archives/wagic-a7a6b8942-w80step1` (= source `cf72a002f`);
the `162v123` game is the RERUN on `archives/wagic-ba5cdc374-w80step1b` (= `337becbf2`,
seat logs `1789241587-*`, + lanes DI/DJ). Every finding below says which games it was
read from. Tooling: `corpus-stats.py` (output `~/.gatelogs/w80-stats.out`),
`prompt-byte-census.py`, and python over the 42 seat logs + 20 `game-*.stderr`;
`/usr/bin/grep` for stderr. Item (5) is RETIRED by the owner's 2026-09-12 ruling —
no lategame specimen is written.

---

## 1. Corpus census

| | wave 79 | wave 80 |
|---|---|---|
| games / clean seat logs | 21 / 42 | 21 / 42 |
| record-kind sum | 2,230 | **2,347** |
| `protocol_replies` (round trips) | 2,203 | **2,135** |
| decisions / results.tsv turn (x2 seats) | 1.86 | **1.92** |
| fallbacks, ask seam | 0 | **0** |
| fallbacks, all kinds | 1 (`reveal`/`engine_answered`) | **2** (1 `reveal`/`engine_answered`, 1 `priority`/`unparsed_reply`) |
| `reply_truncated` | 0 | **0** |
| `post_answer_overrun` > 0 | — | **0** |
| `async_drops` | 15 | **9** |
| `transport` | — | **2,135/2,135 `curl=0,http=200,empty=0`** |
| `deadline_pct` p95 / max | — | **21.4% / 46.4%**; `wall_miss_events` 0 in all 42 seats |
| reply shape (`protocol_deviation`) | compliant 2,104 / unlabelled_plan 87 / plan_absent 12 | **compliant 2,072 / unlabelled_plan 57 / plan_absent 5 / prose_outside_two_lines 1** |
| `plan_line_missing` | 99 (4.5%) | **62 (2.9%)** |
| `off_protocol_bytes` > 0 | 87 (3.9%) | **58 (2.7%)** |
| `reasoning_chars` median / max | 5,835 / 21,069 | **5,770 / 19,629**; `thinking` = `on` on 2,135/2,135 |
| `reasoning_degenerate` | — | max ratio 0.0086 — no degeneracy |
| latency p50 / ask p95 | 94.4 s / 196.3 s | **87.0 s / 185.5 s** |
| `askreplay/` files | 29 | **29** (1,172 `cache_replay` + 4 `crossphase_replay`) |
| `plan_steps_done` | — | 0 on 931, 1 on 1,200, 2 on **4** records |

Record-kind sum 2,347 = ask 1,661 + priority 290 + hold_event 180 + attackers 80 +
discard 52 + blockers 28 + forced_close 26 + reveal 20 + bottom 5 +
menu_single_outcome 3 + recovery 2. CENSUS CHECK RECONCILES (2,347 − 212 no-round-trip
records = 2,135).

**Reply discipline improved on every shape measured** (plan_line_missing 4.5% → 2.9%,
off_protocol 3.9% → 2.7%, `plan_absent` 12 → 5) while latency fell 7.4 s at p50.
`plan_steps_done` reaching 2 on only 4 of 2,135 records says the model restates a
fresh plan almost every window rather than walking one — the plan echo is being
rewritten, not followed.

---

## 2. Regression watch

### (a) Fallbacks — 0 on the ask seam; the robust parser's first live refusal

**The `priority`/`unparsed_reply`.** `1789216920-ai_baka_deck152-0x55a5a97d7280-vs-ai_baka_deck125.jsonl`
seq 101 (turn 37, Main phase 1, life 11 vs 36). Reply verbatim:

```
\n\nPLAN: Respond to Essence Scatter with Fateful Absence to save Briarbridge Tracker, then activate Teferi's -2 in main 2.\nCHOICE: Cast Fateful Absence targeting Essence Scatter
```

`protocol_deviation` on that record is **`compliant`**, not `prose_outside_two_lines`
— the brief's census joins two different records (the single
`prose_outside_two_lines` record is `1789216951-ai_baka_deck126…` seq 62, an
`attackers` reply with a prose paragraph BEFORE the plan, which the parser read
correctly as `ATTACK: A1, A2, A3` → choice 3; that one is a robustness WIN).

Adjudication under the owner's rule: **the refusal was correct and the answer was NOT
unambiguously readable.** The menu had exactly two rows — `1. +1: tap or untap
permanents with Teferi, Who Slows the Sunset` and `2. Hold priority`. "Cast Fateful
Absence" names no row, carries no row number, and is not a legal play (Fateful Absence
destroys a creature or planeswalker; Essence Scatter is an instant on the stack). The
plan PRECEDED the action, so the ordering rule did not fire either; `action_before_plan_rejected_replies` = 0
and `action_before_plan_replies` = 0. The window cost one decision to the heuristic —
the `recovery` record at seq 104 shows `executed_by: heuristic`, `executed_choice: 1`
(Teferi's +1). DH F2's own "real cost" warning is discharged: one window, one
heuristic play, in a game deck152 lost.

**But the window itself is a defect** — see HIGH-2 below. The prompt's own hand line
read `Fateful Absence {1}{w} [instant] [no cast row now: you already answered this
phase's Casting decision, so this window does not offer it - it is otherwise castable
and its own casting window this turn has passed]`.

**The `reveal`/`engine_answered`** — `1789216918-ai_baka_deck152…` seq 43, `options` 5,
`choice` −1, `chosen_text` `none (no legal target)`. Zero-eligible: yes. Genuine; the
seat had no legal pick and the engine answered. One `recovery` record (seq 44) joins it.

**`answer_label_absent_read` = 0** corpus-wide — the label-less shape did not recur.

**NEW, unflagged by any lane: `parse_note: multi_answer_first_taken`, 2 records.**
`1789216920-ai_baka_deck152…` seqs 217 and 237: the model answered a sacrifice-lands ask
with a SIX-row list (`CHOICE: 1, 2, 7, 8, 9, 11 (Boulderloft Pathway, Forest, …)`)
against Emrakul's Annihilator's sacrifice-6 trigger, and the parser silently took row 1.
The seam asks one land at a time and never says so; the model paid for that
misunderstanding twice in one game. MED-3 below.

### (b) The HUNG game and lane DI

`game-162v123-1789216936-HUNG-attempt0.stderr` (5.77 MB, not part of the reviewed
corpus): 3,674 `Using Activated ability` against 3,655 `menu is unchanged after action`
lines. The rerun on `337becbf2` (`game-162v123-1789241585.stderr`,
`1789241587-*` seat logs): **16 activations, 0 `menu is unchanged after action`, game
completed at turn 12, winner deck162.** 0 HUNG games in the reviewed set.

**But the DI fix is UNTESTED, not confirmed.** `cached_replay_reasked` = 0 corpus-wide
and 0 on the rerun seat, and the state never arose: across the rerun deck123 seat's 55
prompts naming Intruder Alarm, **Intruder Alarm is in HAND in all 55 and on the
battlefield in 0**; Thraben Doomsayer is on the battlefield in 54. The Doomsayer+Alarm
board the loop needs never formed. `reask_reason` appears on 0 records corpus-wide.
`menu_pass_no_progress_suppressed` 0 (wave 79: 1) is likewise uninformative.
The rerun seat's other counters stay in the wave-80 range (`hold_events` 5,
`hold_windows_skipped` 23, `stop_reached_*` 0, `ask_replays_cache` 66), so no
key-stability regression is visible.

### (c) U1 / DG + DH F5 — the cast step in the plan echo

`dropped after its X was announced`: **2 in the corpus, 0 in the HUNG stderr.** The
brief's attribution is wrong — neither hit is in `game-162v123-1789216936-HUNG-attempt0.stderr`
(that file greps 0). Both are corpus games:

* `game-125v123-1789216946.stderr:2313` — Sphinx's Revelation, **12 mana already paid,
  lost**; the model took option 11 of 11 (the decline rung).
* `game-152v125-1789216918.stderr:4677` — Sphinx's Revelation, **17 mana already paid,
  lost**; option 16 of 16.

Both decline rows **did** print the stranded mana, in full and correctly:
`{declining strands the 17 mana already paid: the card comes back to your hand, your
sources STAY TAPPED, and the 17 mana stays floating in your pool. No row on this list
can spend it … whatever is still floating when the step ends is lost}`. So the render
half of the docket item is discharged; the **model declines anyway**, which makes this
a guide/strategy surface, not a render gap. The `{X}`-announcement payment-loss docket
item stays OPEN (2 events, ~29 mana burned).

**Completing menus.** 18 prompts carry `this menu completes THAT cast`; 17 of them have
a preceding record with a PLAN. In **17/17 the leading clause survives into the echo**
— F5's first half holds and U1's own RED (`162v130` seq 31→32) does not recur.

**ANNOUNCE_X asks still drop it, 1 of 17.** `1789216941-ai_baka_deck130-0x557205aee960`
seqs 19 → 20: seq 19's reply is `PLAN: Cast Starstorm with X=2 at Thraben Doomsayer
now. In main phase 1, cast Talisman of Impulse to ramp, then hold for Rorix Bladewing
next turn.` + `CHOICE: 2 (Cast Card Normally)`. Seq 20 is the X announcement and its
echo reads `"In main phase 1, cast Talisman of Impulse to ramp, then hold for Rorix
Bladewing next turn."` — the cast clause deleted at the very window being asked to set
that cast's X, with `plan_step_count` 3 → 2. That is F5's RED verbatim, on the fixed
binary, at the second completing menu. DG's prediction measures exactly this shape
("or is an ANNOUNCE_X ask") and it FAILS 1/17.

`plan_cast_steps_closed` 376 > 0. Its equality-to-committed-casts half is not
checkable from the corpus: the counter has no per-record or per-event join — only the
stderr line — so I could not verify it. `plan_names_uncastable_zone_card` 29,
`plan_names_stranded_card` 1.

### (d) U2 / DE + DH F3/F4/F11 — holds and the two verdict lines

**`hold_events` reconciles exactly.** 180 `hold_event` records = 180 counter =
`verdict_clamped` 71 + `reopen_rows_moved` 102 + `reopen_new_lethal` 7 + `new_threat` 0.
F11 PASSES on both halves and on its ≥59 floor. Seam split: cast 167 / priority 13.

**The 71 clamps are a single, degenerate face pair.** Every one reads
`held [stack death verdict: you survive the stack] over live [stack death verdict:
nothing lethal on the stack]`. Ignoring was RIGHT in all 71 — the live face is strictly
safer than the held one, so nothing was owed the model. That is also the item's limit:
the sample the brief asked for (20 of the 71) contains no variety at all, so the clamp
is auditable but has not yet been exercised on a case where the answer could go the
other way.

**`hold_reopened_new_threat` = 0 and the state never arose.** The equal-rank re-open
needs an own-loop THREATENED face; `own_loop_verdict` is `proven win` on all 18
rendered lines and the THREATENED face rendered 0 times. UNTESTED, now from evidence.

**HIGH-1: four of the seven `new_lethal` re-opens produced no ask, and the seat died.**
`1789216953-ai_baka_deck125-0x55ed042111c0-vs-ai_baka_deck162.jsonl` seqs 18, 20, 22, 24
— four `reopen_new_lethal` events on the SAME `window_seq` 14, all
`[stack death verdict: the stack KILLS you]`, all at the cast seam, and the seat log has
**no record with a prompt after seq 17**: gameend at seq 26, `won: false`, life 0.
`game-125v162-1789216950.stderr` lines 1289-1309 show the pattern four times over:

```
AIPlayerGPT: a hold re-opened at the cast seam - the live [stack death verdict: the stack KILLS you] face is lethal over a DIFFERENT object set than the one the hold was taken over
AIPlayerGPT: hold re-opened at the cast seam - a NEW lethal threat: the objects behind it are not the ones this hold was taken over
AIPlayerGPT: the model took the hold row at the cast seam on turn 16 - later cast windows are held until one of these rows changes
```

There is no `-> chose` between the re-open and the re-take. The window re-opens, the
row set is unchanged, the hold latch immediately re-clamps from the seat's own retained
answer, and the model is never put the lethal board. F3's fix delivers the TELEMETRY it
promised and no behaviour: on this board the re-open is a logged no-op. The other 3
re-opens (`146v162` seq 23, `146v125` seq 214, the rerun `123v162` seq 136) did reach
the model, so the path works when the rows move.

**HIGH-3: the rendered crack-back verdict contradicts its own screen, 11 windows.**
The verdict line is computed off the raw combat total; the CRACK-BACK header's own
`ADD THOSE UP` clause then adds animated creature-lands and trigger power, and the
verdict is not recomputed.

* 6 windows say `you survive` on a screen whose header says `that would KILL you`.
  `1789216918-ai_baka_deck130-0x559161469ed0` seq 17, life 20: header
  `…for up to 15 from combat … Lair of the Hydra (best rung their mana pays for: 5
  power once animated …); triggers on their board add power before damage - Luminarch
  Aspirant - ADD THOSE UP: … the total to subtract from your life is 20 and you would
  be at 0; that would KILL you`, beside
  `[crack-back verdict: you survive - … you are still alive afterwards: the crack-back
  alone does not end the game]`. Same seat seqs 19, 22, 24, 25; `162v130` seq 43.
* 5 windows are the `blocked away` face flipped by the same addendum:
  `1789216925-ai_baka_deck162-0x55be72c6cdd0` seqs 21, 22 (`best block: 8 … leaving you
  at 2`, +3 animated = −1), 31, 32, 33 (`leaving you at 1`, +3 = −2). Each also says
  `passing this window does NOT hand them the game`.

Five `crackback_lethal_blocked_away` quotes, as asked:
`146v162` seq 53 (life 3: `with your best block: 0 damage still gets through, leaving
you at 3`) — TRUE, the seat's Angel 4/4 flier blocks the only attacker, a 5/5 flier;
`146v162` seqs 21/22 (`8 … at 2`), 31 (`6 … at 1`), 32/33 (`3 … at 1`) — the floor
arithmetic is right against the combat total and wrong against the screen. The
"with your best block" FLOOR is the right idea; it is computed over the wrong total.
The LETHAL-no-floor face states its own limit honestly
(`This figure subtracts no blockers (the engine could not prove a best block on this
board)`) and on `152v130` seq 27 that was correct — the seat controlled 0 creatures.

**HIGH-4: the send-time counters still over-count, and the record faces under-stamp.**
DH F10 predicted each counter EQUALS its literal's prompt count, exactly.

| counter | value | prompts carrying the line | records carrying the face | verdict |
|---|---|---|---|---|
| `crackback_verdict_lines_rendered` | **382** | **329** | **291** | FAIL both ways |
| `stack_death_verdict_lines_rendered` | 36 | 36 | 33 | counter PASS, face FAIL |
| `own_loop_verdict_lines_rendered` | **20** | **18** | 18 | FAIL |
| `stack_drain_windows_asked` | 30 | 30 | n/a | PASS |
| `crossphase_identical_reputs` | 129 | 129 | n/a | PASS |
| `crackback_lethal_blocked_away` | **26** | 24 | **14** | FAIL |

The over-count is spread across 20 of 21 seats (worst: `152v123` 44 vs 39, `152v125`
32 vs 27, `162v130` 26 vs 23), so a suppressing path still stamps at the cast seam —
DE U2's own negative clause. The under-stamp is seam-shaped: of the 38 prompts that
carry `[crack-back verdict:` with no `crackback_verdict` field, **36 are `priority`
records** (e.g. `1789216920-ai_baka_deck125…` seqs 37, 46, 258, 287). No record anywhere
carries the field WITHOUT the line, so the direction is one-way.

`hold_windows_skipped` 1,051 (cast 860 / priority 191) against wave 79's 1,598 — a 34%
fall, tracking the 102 rows-moved re-opens the new event kind now makes visible.
`hold_released_turn` 81.

### (e) U8 / U9 — cross-phase replay and the key

`crossphase_replayed` **4** (predicted band 10-40 — MISSED, > 0 met).
`crossphase_board_unchanged` **0** (wave 79: 26) and `crossphase_identical_reputs` 129
(wave 79: 172). **The population did not move into the replay — it did not arise.**
All 129 re-put brackets in this corpus render the *moved-board* wording
(`the board has moved since then, but this list has not`); the board-unchanged variant
renders **0 times**. So the replay predicate's board-key equality precondition was
almost never satisfiable, and the 4 replays are the whole eligible population, not the
surviving tail of 26.

All 4 replays traced in `askreplay/`, and each replayed answer is the DECLINE row
(`choice 3 of 3`, `Cast nothing right now`):
`Main phase 2 → End` x2 (`152v123` seq 62 from 57; `146v125` from 191),
`Main phase 1 → Combat begins` and `Main phase 1 → Attackers` (`152v125` seqs 204/205,
both `replayed_from` 203, `replay_run` 1 then 2). **No replay entered a sorcery-speed
window** (no `Draw → Main phase 1`, no `Untap/Upkeep → Main`), so Astra's
phase-gated-option risk did not materialise; and because every replayed answer was a
decline, no phase-gated legal option could have been lost by serving it.

**DH F12 PASSES cleanly**: all 4 `replayed_from` values resolve to an `ask` record with
that `seq` in the same seat log, and so do all 1,172 `cache_replay` sidecars (1,176/1,176
joins, 0 orphans).

**U9's identity holds exactly**: `ask_replays_cache` 1,172 + `_repeat_latch` 0 +
`crossphase_replayed` 4 = `ask_replays_reserved` 1,176.

`ask_key_continuation_differs` 112 — **I could not sample it. The counter has no
per-record or sidecar trace at all** (it exists only on the 42 `gameend` records), so
the wave-79 LESSON is not yet applied to it. Same for `hold_windows_skipped`,
`mana_only_windows_skipped`, `own_turn_windows_skipped`, `main_phase_windows_skipped`
and `plan_cast_steps_closed`.

### (f) U10 / DH F6 — the own-loop face

18 rendered lines, 18 stamped faces, **0 `[own loop verdict: none]`, 0 on a two-row
land-drop menu** — DE U10's negative clauses all PASS; the counter reads 20 (HIGH-4).
All 18 are `[own loop verdict: proven win]`; **`proven but no live starter` rendered 0
times**, so F6's predicted population shift is UNTESTED. Every line names a live
starter: Exquisite Blood (`126v123` seq 16), Perimeter Captain (`126v130` seqs 13-39,
10 windows), Pride Guardian (`126v162` seq 61).

**MED-1: the starter test admits triggers the seat cannot fire.** In
`1789216933-ai_baka_deck126-0x55559c0ca2d0` the loop (Sanguine Bond + Exquisite Blood,
both on the battlefield from seq 13) is real, but the named starter is **Perimeter
Captain (0/4 defender)** — a BLOCK trigger, live only if the opponent attacks into it —
and the seat was told `your life loop is a proven win … It starts from Perimeter
Captain` ten times and **lost the game at 0 life on turn 22**. Pride Guardian
(`Whenever Pride Guardian blocks, you gain 3 life`) is the same shape. F6 gates
ACTIVATED lines and ENTERS triggers and lets "any other trigger" stand as live; a
blocking trigger is not under the seat's control and is not an entry the seat can take.
I did NOT verify whether the opponent ever attacked into those blockers in that game.

`own_loop_windows_asked` 1.

### (g) U3 / U4 / U6 / U7 / DF + DH F7/F8/F9 — cover, pricing, sweeps

* **ETB self-leave (U3a / F7).** 89 `{crack-back cover…}` clauses; **3 read
  `crack-back cover: NONE from this body`, all Acererak the Archlich**
  (`146v162` seq 38, `146v123` seqs 13, 19), all correctly citing its untriggered ETB
  line. **0 clauses on any card whose leave line carries a cost prefix** — no Fleeting
  Image row anywhere carries the clause. F7's falsifier does not fire. **0 bodies are
  credited that the row does not leave.**
* **The mechanism sentence (U3b).** 37 of the 89 clauses carry it
  (`an attacker that is BLOCKED deals its combat damage to the blocker, not to you -
  all of it, whatever the blocker's size`); the other 52 are removal/edict covers where
  the blocking lesson has no referent. **`TRAMPLE, so blocking it does not remove its
  whole power` renders 0 times, and 0 of the 24 prompts carrying `a 1/1 in front of a
  5/5 stops the whole 5` have a trampler on the opponent's battlefield.** F8's
  falsifier is clean; its positive is UNTESTED — the trample board never coincided with
  a rendered cover lesson (31 prompts do have an opposing trampler under a CRACK-BACK
  line, but none of those printed the lesson).
* **Tap bill (U4).** 42 rows carry a `{paying this taps: ` clause, **0 carry two**
  (wave 79: 3). 10 rows state their own count (`count them from this number, not from
  the commas`). PASS on both halves.
* **Cycling (U6).** 9 cycling rows carry a `[DRAW PRICE:` bracket
  (`162v130` seqs 76, 83, 89, 91, 94, 96 x2, 103 x2; Starstorm, Lay Waste, Spark Spray),
  and **none carries a discard term** — because **no corpus window put a cycling row on
  a menu under a live discard punisher** (5 prompts carry a `DISCARD PUNISHERS on the
  battlefield` line; 0 of them offer a cycling row). The negative half PASSES vacuously;
  the positive is UNTESTED. `DISCARDS 1 card` and `a CYCLING cost is a discard` render
  0 times corpus-wide, so a cycling row under NO discard punisher still says nothing
  about discarding at all — LOW-1.
* **`board sweep:` markers (U7).** 38 markers, 8 with `YOURS > 0`. **2 name the roster
  (`152v123` seq 82: `THEIRS 2 / YOURS 2 (Bloodline Keeper (3/3) …`), 6 do not** — all
  6 in `1789216937-ai_baka_deck130-0x562b68836ca0` (seqs 49, 52, 55, 57, 69, 75),
  including `[<- board sweep: THEIRS 4 / YOURS 5 - the only row on this menu that
  prices a board sweep (it takes more of YOURS than of THEIRS)]` with no names at all.
  U7's negative prediction FAILS 6/8. Mitigation: on those rows the SAME row's
  `{X pricing: …}` clause does name them (`YOURS: Dwarven Blastminer, Siege-Gang
  Commander, Goblin x3`), so the fact is on the screen — the X-variable sweeper path
  simply was not wired into `boardSweepMarker`.
* **Blockers verdict (U14 / F9).** 28 `blockers` records, 26 `NOT lethal` headers,
  **0 contradicted by their own DRAW FORECAST** — the load-bearing negative PASSES.
  The withdrawal clause (`NOT lethal FROM THE COMBAT ALONE`) and F9's
  `if you CHOOSE` optional-draw clause each render **0 times**: no blockers screen in
  this corpus had a compulsory draw charge that reached 0, and none had a `may`-draw
  permanent. Both positives UNTESTED.

### (h) U13 / DF — the single-outcome menu

3 `menu_single_outcome` records, `single_outcome_rows_spared` 60 = 3 x 20 — the join is
exact. All three are the Intrepid Adversary counter menu with `rows: 21`,
`repeat_payments_paid: 0`, `answered: 0`, `answered_text: "don't add any counter"`
(`152v130` seq 6, `146v152` seqs 6 and 8). **Every row on each really did resolve to one
outcome**: the payment figure on the record is 0, so no rung was reachable and the mana
spent is 0 either way. **0 `ask` records remain whose `options` is 21 with
`pays for 0 of them` in the row text** — nothing eligible was left asked. PASS on all
three halves. Note the gate is still one card family (lane DF's own weakest-evidence 10).

### (i) U5 / DG — the neutral exemplar

The ask seam's exemplar is now the template on 1,644 prompts
(`e.g. "CHOICE: <row number> (<that row's short name>)"`), the priority seam's own
placeholder on 290, the ANNOUNCE_X template on 17.

**Row-1 pick rate, on DG's own measure** (records with ≥3 rows and a decline/hold row,
share with `choice == 1`):

| seam | wave 79 | wave 80 |
|---|---|---|
| ask | 213/1,139 = **18.7%** | 228/1,129 = **20.2%** |
| priority | 41/182 = 22.5% | 66/198 = 33.3% |

The ask-seam rate **did not fall — it rose 1.5 points**, and it was already below the
priority seam it was supposed to converge toward. DG's prediction ("a rate that does not
move is a FAIL") FAILS. Unfiltered, deck162's own figure — the lane's 62/139 citation —
does fall, 62/125 = 49.6% → 60/137 = 43.8%, and the corpus-wide unfiltered ask rate is
33.3% → 34.5%. A one-wave rate over a changed menu mix cannot separate the exemplar's
effect from the mix; I would not claim either direction from this.

DG's own stronger negative — `0` ask prompts whose format sentence contains a card name
— **FAILS**: 5 prompts quote a live row
(`e.g. "CHOICE: 19 (Create human with Thraben Doomsayer x<N>)"`,
`"CHOICE: 20 (Equip with Lightning Greaves targeting Human #1 x<N>)"`,
`"CHOICE: 2 (Create vampire with Bloodline Keeper x<N>)"`). The repeat-menu (`x<N>`)
exemplar seam was not converted. LOW-2.

### (j) U11 / U12 / DG — markers, the pointer, the six gloss fixes

* **Stack pointer.** 260 prompts carry `the stack is not empty (see ON THE STACK
  above)`; **0 of them lack an `ON THE STACK, waiting to resolve` block** (wave 79:
  135 did). PASS.
* **Own-spell marker.** `THIS IS YOUR OWN spell` renders once corpus-wide, and **0 rows
  anywhere name a card in `[your stack]`** — the `125v146` seq 104/105 shape (the seat
  countering its own spell) did not recur and could not have. UNTESTED, state absent.
  **0 seats countered their own spell.**
* **U12 (1) dangling connective before `(...more)`: 0 renders** (wave 79: 62). PASS.
* **U12 (2) `... (...more)`: 0 renders.** PASS.
* **U12 (3) Puzzle Box: 9 renders of `their whole hand:`, 0 `{feeds:` clauses reading
  "an amount that is not fixed" on a hand-counting script.** PASS.
* **U12 (4) `[legendary:` 24 renders — 16 carry `at N loyalty`** (Teferi x15, Ob Nixilis
  x1) **and the other 8 are Nadaar, Selfless Paladin, a legendary creature**, which
  correctly keeps the wave-79 bytes. PASS.
* **U12 (5) 84 renders of `(...more - this card also has …; their full text is in the
  decklist in your instructions)`.** PASS.
* **U12 (6) 37 `{modes live right now:` clauses, 20 carrying `(right now: …)`;** the
  other 17 are empty-object lists, which print nothing rather than an empty
  parenthesis. PASS (`≥1` predicted).

### (k) U16 / U17 — bytes and the per-seam cost table

`prompt-byte-census.py`, measured on both corpora:

| deck | w79 mean B | w80 mean B | w79 >20 KB | w80 >20 KB | hold apparatus w79 → w80 |
|---|---|---|---|---|---|
| 123 | 11,872 | 12,405 | 1/495 | 14/358 | 1,377 → **1,221** |
| 125 | 12,310 | **15,192** | 49/415 | **157/529** | 1,253 → **1,147** |
| 126 | 10,782 | 10,348 | 1/192 | 3/168 | 1,077 → **1,023** |
| 130 | 10,815 | 11,908 | 11/322 | 14/301 | 1,037 → **892** |
| **146** | 15,019 | **13,860** | 97/427 | 55/332 | **1,015 → 862** |
| 152 | 10,451 | **14,194** | 2/203 | **85/279** | 842 → **851** |
| 162 | 9,656 | 9,943 | 1/149 | 10/168 | 1,045 → **925** |
| **corpus** | **12,029** | **13,124** | **124 (5.6%)** | **287 (13.4%)** | |

**U16 delivered more than it promised**: deck146's hold apparatus fell 1,015 → **862
B/prompt (−153)** against the −91 predicted (the `[hold check:` bracket itself fell
239 → 148 = exactly the −91 fold; the row and the contract shrank a further −19/−42).
PASS.

**MED-2: the wave's own new render swamped it.** Corpus-wide the mean prompt ROSE
**+1,095 B (+9.1%)** and the share over 20 KB went **5.6% → 13.4%**, with max 25,471 →
27,189. deck152 (+3,743 B, 2 → 85 prompts over 20 KB) and deck125 (+2,882 B, 49 → 157)
carry it — the two decks whose boards feed the new crack-back/stack-death verdict lines,
the cover mechanism sentence and the completing-menu spent clause. U16's 91 B is 8% of
what wave 80 added.

**U17, reproduced on this corpus** (from `w80-stats.out`): latency p50 **87.0 s**
(ask 88.2, blockers 120.6, bottom 87.0 on 1.9 KB prompts, discard 60.9 on 17.0 KB
prompts), `ms per reasoning char` 10.6-23.5 across seams, and correlation with latency
over 2,135 round trips **r = −0.374 vs prompt bytes, r = +0.657 vs reasoning_chars**.
Wave 79 read −0.302 / +0.748. **The sign and the ordering replicate**: the wait is
bought by reasoning output, not by prompt size, and the seam with the smallest prompts
is not the fastest. What it means for the byte work: **byte reduction is not a latency
lever and should not be justified as one.** It remains justifiable on context-budget
grounds — and MED-2 says that budget moved backwards this wave — but the 87 s p50 will
only move on reasoning budget, seam mix or server throughput.

`plan_line_missing` 62 (2.9%, was 4.5%) — the best figure in the project's history.

### (l) DJ — the harness

CONFIRMED from the run's own files. `matchups-20260912-074153-rerun-01-162v123-attempt1-killed/REGIME-FAIL`
reads `the binary played 20 of 20 checked records with thinking=off while the launch
asked for on (first: 1789239545-ai_baka_deck198-0x556a69034850-vs-ai_baka_deck199.jsonl)`
— a development lane's stub fixture, exactly the shape DJ describes. The successful
rerun `matchups-20260912-074153-rerun-02-162v123/` has **`REGIME-GATE-PASSED`, no
`REGIME-FAIL`, and a `regime-gate-state.txt` listing exactly the run's own two seat
logs** (`1789241587-ai_baka_deck123-…` and `…deck162-…`), which are the two basenames
announced on `game-162v123-1789241585.stderr` at lines 123 and 149
(`WAGIC_GPT_TRANSLOG_FILE …`). Manifest keying works end to end; the game completed
(`162 123 0 26 0 12`). PASS.

### (m) Residuals

* `async_drops` **9** (wave 79: 15), all one class: `casting/question and board/re-asked`,
  on 7 `ask` and 2 `blockers` records. Every one was re-asked; none lost a decision.
* `forced_close_events` **26** / `forced_close_unrecorded` **13** — 26 records,
  13 `armed` + 13 `dropped_decision_moved`, arms cast 20 / land 6, max
  `unrecorded_so_far` 2 on any seat. Concentrated: 30 of the 42 seats logged
  `forced closes: 0 events`; 11 logged `2 events, 1 superseded`; one logged
  `4 events, 2 superseded`.
* `wall_miss_events` / `_no_retry` / `_unrecorded` **0 in all 42 seats**;
  `phase2_answer_missing` / `_recovered` **0**. `deadline_pct` never exceeded 46.4%
  against WAGIC_GPT_TIMEOUT=900 — the wall is not near.
* **Stop chains: `stop_reached_windows_skipped` 1, `_reputs_collapsed` 0, and almost no
  stop arose** — 2 prompts carry `ALREADY AT OR PAST your own stop` and 2 answers name
  a `repeated then stop` row corpus-wide. The population is real but tiny; neither
  collapse arm can be adjudicated this wave.
* `mana_only_windows_skipped` **78** (wave 79: 129); `main_phase_windows_skipped` 70;
  `own_turn_windows_skipped` 722; `identical_option_asks_resolved` 13;
  `reserve_decline_windows_noted` 9 / `_skipped` 1.
* Wins: deck152 4/6, deck130 4/6, deck126 3/6, deck125 3/6, deck162 3/6, deck146 3/6,
  deck123 1/6 (wave 79: 152 5/6; 162/146/130 4/6; 126 3/6; 125 1/6; 123 0/6). deck125
  +2 and deck123 +1 are the movement; deck146 −1, deck162 −1, deck152 −1. Hands are
  real; I read none of this as guide effect.

### (n) The harness's own behaviour

* **HUNG tripwire: one true positive, no false positives.** Attempt 0 was killed at
  3,641 s and its stderr preserved; no other game tripped it; 21/21 games in the final
  set carry `gameend` on both seats and 0 are timeout-tainted.
* **No pilot stall.** All 2,135 round trips report `curl=0,http=200,empty=0`;
  0 deadline misses; latency p95 185.5 s against a 900 s wall.
* **One harness FALSE POSITIVE, on the first rerun attempt** — the foreign-log regime
  gate DJ fixed. That kill cost a full rerun attempt (`…attempt1-killed`, ~33 min of
  wall between the two `game-*.stderr` files in it).
* Timing I can measure: all 21 games in the main pool started within **40 s** of one
  another (`results.tsv` start epochs 1789216914-1789216958), i.e. the pool is fully
  parallel, and the rerun started **24,671 s (6 h 51 m)** after the pool's first game.
  I could NOT measure the 5 h 58 m to the 20th game — no per-game end timestamp is
  recorded in `results.tsv` and I did not have the harness's own run log.

---

## 3. Prediction adjudication

### Lane DE (wave80/lane-DE.md)

| # | prediction | verdict | evidence |
|---|---|---|---|
| U15 | `stack_drain_windows_asked` > 0 and == prompts with `OF THIS STACK`, band 15-40 | **PASS** | 30 == 30, in band |
| U15 | `crossphase_identical_reputs` == prompts with `[this exact list was put to you` | **PASS** | 129 == 129 |
| U2 | `hold_safer_ignored_face` / `hold_reopen_reason` on > 0 records | **PASS (superseded by DH F11)** | fields deleted; 180 `hold_event` records carry `face`+`reason`, 71 clamps + 109 re-opens |
| U2 | `crackback_verdict_lines_rendered` > 0 and == its literal's prompt count (band 100-600) | **FAIL** | 382 vs 329 prompts; in band but the equality is the prediction |
| U2 | `stack_death_verdict_lines_rendered` > 0 and == prompt count (band 50-400) | **PASS on equality** (36 == 36), band MISSED low | 36 |
| U2 | `hold_reopened_new_threat` > 0 conditional on a THREATENED own-loop window | **UNTESTED** | 0 THREATENED faces rendered; all 18 own-loop lines are `proven win` |
| U8 | `crossphase_replayed` > 0, band 10-40 | **PASS on > 0, band MISSED** | 4; population did not arise (0 board-unchanged brackets rendered) |
| U8 | Draw → Main phase 1 still asked | **PASS** | 0 replays into a sorcery-speed window |
| U8 | negative: replayed = 0 while board_unchanged large | **N/A** | `crossphase_board_unchanged` 0 |
| U9 | cache + repeat_latch + crossphase == reserved; cache > 0 band 400-1,400 | **PASS** | 1,172 + 0 + 4 = 1,176 |
| U10 | `own_loop_verdict_lines_rendered` > 0 (band 2-80) | **PASS** | 20 |
| U10 | ... and == prompts carrying `[own loop verdict:` | **FAIL** | 20 vs 18 |
| U10 | 0 records carrying `[own loop verdict: none]`; 0 on a land-drop menu | **PASS** | 0 and 0 |

*DE: 13 predictions — PASS 9 (one of them superseded-but-satisfied by DH F11; two met ">0" but missed their band) · FAIL 2 · UNTESTED 1 · N/A 1.*

### Lane DF (wave80/lane-DF.md)

| # | prediction | verdict | evidence |
|---|---|---|---|
| U3a | 0 cover clauses on a self-leaving-ETB card | **PASS** | 0/89 |
| U3a | `crack-back cover: NONE from this body` > 0 | **PASS** | 3 (all Acererak) |
| U3b | mechanism sentence rendered > 0 | **PASS** | 37 clauses |
| U3b | 0 reasoning bodies computing cover from blocker TOUGHNESS | **NOT CHECKED** | I did not read the 2,135 reasoning bodies |
| U3c | `sacrifice_row_blocker_priced` > 0 on an eligible board | **UNTESTED** | no eligible board arose (lane's own weakest-evidence 1) |
| U3c | 0 renders where the cost cannot take a creature | **PASS** | 0 |
| U4 | 0 rows with two `{paying this taps: ` | **PASS** | 0/42 (wave 79: 3) |
| U4 | `tap_bill_count_stated` > 0 | **PASS** | 10 rows |
| U6 | `cycle_row_discard_priced` > 0 | **UNTESTED** | 9 cycling rows, 0 under a live discard punisher |
| U6 | 0 `Cycle]` rows under a named discard punisher omitting the discard term | **PASS** (vacuous) | 0 |
| U7 | `board_sweep_own_roster_rendered` > 0 | **PASS** | 2 markers |
| U7 | 0 markers with non-zero YOURS and no names | **FAIL** | 6 of 8 (`162v130` seqs 49/52/55/57/69/75) |
| U14 | `blockers_cycle_lethal_verdict` ≥ 1 | **UNTESTED** | 0 windows of the shape |
| U14 | 0 `NOT lethal` headers contradicted by their own DRAW FORECAST | **PASS** | 0 of 26 |
| U13 | `single_outcome_menus_answered` > 0, one record per increment | **PASS** | 3 / 3, 60 rows spared |
| U13 | 0 leftover 21-row Adversary asks with `pays for 0 of them` | **PASS** | 0 |

*DF: 16 predictions — PASS 11 · FAIL 1 · UNTESTED 3 · NOT CHECKED 1.*

### Lane DG (wave80/lane-DG.md)

| # | prediction | verdict | evidence |
|---|---|---|---|
| U1 | `dropped after its X was announced` = 0 | **FAIL** | 2 (`125v123`, `152v125`; 12 and 17 mana lost) |
| U1 | 0 leading-clause losses at completing menus / ANNOUNCE_X asks | **FAIL** | 0/17 at completing menus, **1/17 at ANNOUNCE_X** (`123v130` seq 20) |
| U5 | ask-seam row-1 pick rate falls toward the priority rate | **FAIL** | 18.7% → 20.2% (rose); priority 22.5% → 33.3% |
| U5 | 0 ask prompts whose format sentence contains a card name | **FAIL** | 5 (`x<N>` repeat-menu exemplar) |
| U11 | 0 prompts with the stack pointer and no ON THE STACK block | **PASS** | 0/260 (wave 79: 135) |
| U11 | every own-stack target row carries the marker | **UNTESTED** | 0 rows named a card in `[your stack]` |
| U12.1 | 0 renders of a dangling connective before `(...more)` | **PASS** | 0 (wave 79: 62) |
| U12.2 | 0 renders of `... (...more)` | **PASS** | 0 |
| U12.3 | 0 `{feeds:` "amount that is not fixed" on a hand-counting script | **PASS** | 0; 9 `their whole hand:` renders |
| U12.4 | every planeswalker `[legendary:` carries `at N loyalty` | **PASS** | 16/16; 8 creature twins correctly plain |
| U12.5 | every multi-loyalty cut names its dropped labels | **PASS** | 84 renders |
| U12.6 | ≥ 1 `(right now:` inside `{modes live right now:` | **PASS** | 20 of 37 |
| U16 | deck146 hold apparatus 1,015 → 924 B/prompt | **PASS (exceeded)** | 862 B/prompt measured |
| U17 | measure only | **reproduced** | r = −0.374 bytes / +0.657 reasoning_chars |

*DG: 14 predictions — PASS 8 · FAIL 4 · UNTESTED 1 (+1 measure-only, reproduced).*

### Lane DH (wave80/lane-DH.md — the twelve Astra findings)

| # | prediction | verdict | evidence |
|---|---|---|---|
| F1 | re-applying the merge damage fails the build | **UNTESTED** | not corpus-observable |
| F2 | `action_before_plan_rejected_replies` == records with the flag; will read 0 | **PASS** | 0 and 0; `action_before_plan_replies` 0; the one refusal (seq 101) is an off-menu answer, not an ordering violation |
| F3 | `hold_reopened_new_lethal` > 0, one `hold_event` record per increment | **PASS (counter)** | 7 and 7 |
| F3 | ...and the seat gets the window | **FAIL (behaviour)** | 4 of 7 re-opens produced no ask (`125v162` seqs 18/20/22/24) — HIGH-1 |
| F4 | `crackback_lethal_blocked_away` > 0, ≤ lines_rendered, per-record trace on each | **FAIL** | 26 > 0 and ≤ 382, but only 14 records and 24 rendered lines |
| F5 | `plan_cast_steps_closed` > 0 and == committed casts | **PASS on > 0; equality UNTESTED** | 376; no per-record join exists |
| F5 | DG's X-drop = 0 must hold at ANY completing menu | **FAIL** | 2 X-drops; 1 ANNOUNCE_X echo truncation |
| F6 | `proven but no live starter` faces appear where wave 80 named a tapped permanent | **UNTESTED** | 0 such faces; 18/18 are `proven win` |
| F7 | falsifier: any `NONE from this body` over a cost-prefixed leave line | **PASS** | 0; all 3 are Acererak's untriggered ETB |
| F8 | non-zero `TRAMPLE, so blocking…`; zero `stops the whole 5` on those prompts | **UNTESTED positive / PASS negative** | 0 trample sentences; 0 of 24 lesson prompts have an opposing trampler |
| F9 | `if you CHOOSE` clause appears; falsifier: a lethal figure including a `may` draw | **UNTESTED positive / PASS negative** | 0 renders; 0 contradicted headers |
| F10 | four counters each EXACTLY equal their literal's prompt count | **FAIL 2 of 4** | crackback 382≠329, own_loop 20≠18; stack_death 36=36, stack_drain 30=30 |
| F10 | the `*_verdict` field present on exactly the records whose prompt carried the line | **FAIL** | 38 crack-back prompts + 3 stack-death prompts carry the line with no field (36 of the 38 are `priority`) |
| F11 | `hold_events` == records, and its four parts sum to it; ≥ 59 | **PASS** | 180 == 180 = 71 + 102 + 7 + 0 |
| F12 | every `crossphase_replay` `replayed_from` resolves to an ask record | **PASS** | 4/4 (and 1,172/1,172 cache sidecars) |

*DH: 15 predictions — PASS 8 (F8 and F9 counted on their checkable negative half) · FAIL 5 · UNTESTED 4 (F1, F6, and the positive halves of F8/F9; F5's equality half also unverifiable).*

### Lane DI (wave81/lane-DI.md — rerun game only)

| prediction | verdict | evidence |
|---|---|---|
| 0 HUNG games in the rerun | **PASS** | rerun completed turn 12; 16 activations vs attempt 0's 3,674; 0 `menu is unchanged after action` vs 3,655 |
| `cached_replay_reasked` > 0 on a loop-engine seat, with `reask_reason` | **UNTESTED** | Intruder Alarm in HAND on all 55 prompts that name it, on the battlefield 0; the combo never formed. 0 `reask_reason` records corpus-wide |
| `menu_pass_no_progress_suppressed` falls sharply on such seats | **UNTESTED** | 0 in both waves' rerun seats |
| `ask_replays_cache` at the cast seam unchanged | **PASS** | 66 on the rerun deck123 seat, in range |
| no key-stability regression (`hold_events`, `hold_windows_skipped`, `stop_reached_*`) | **PASS** | 5 / 23 / 0 / 0 on the rerun seat |

*DI: PASS 3 · UNTESTED 2.*

### Lane DJ (wave81/lane-DJ.md — rerun game only)

| prediction | verdict | evidence |
|---|---|---|
| the rerun passes on the manifest-keyed harness | **PASS** | `rerun-02/REGIME-GATE-PASSED`, no `REGIME-FAIL`, `regime-gate-state.txt` = exactly the run's own 2 announced basenames; both announced at `game-162v123-1789241585.stderr:123,149` |
| a foreign log never names our run's verdict | **PASS** | attempt 1's `REGIME-FAIL` names `1789239545-ai_baka_deck198-…` — the RED, reproduced on disk |

*DJ: PASS 2.*

### Carried UNTESTED docket

| item | verdict this wave |
|---|---|
| **{X}-announcement payment loss** | **STILL LIVE — 2 events** (`game-125v123-1789216946.stderr:2313` 12 mana; `game-152v125-1789216918.stderr:4677` 17 mana). The decline row printed the stranded mana correctly both times; the model declined anyway |
| BS F2 Tribute second edict | **NOT ADJUDICATED.** Tribute to Hunger is live in this corpus (77 own casts, 98 opposing) but the brief carries no repro definition for F2 and I did not have `wave*/lane-BS.md` in scope; I did not read it |
| BS F5 blockers ceiling | **UNTESTED** — 28 blockers records, only 1 with ≥12 options |
| BS F6 distinct-index re-ask | **NOT ADJUDICATED** (no repro definition in this brief) |
| CF F1 pending-deadlock | **UNTESTED** — 0 prompts carry a pending/deadlock clause |
| F2 / F5-nonlifelink / F8 haste / F9 (CF-era) | **NOT ADJUDICATED** — no repro definitions carried into this brief; haste appears in 246 prompts and lifelink in 205, so the populations exist |
| reasoning-body halves of N14a / N14d / O20 | **NOT CHECKED** — I did not read reasoning bodies this wave |

---

## 4. Engine / render / narration defects

### HIGH-1 — a `new_lethal` hold re-open is a logged no-op: the window re-opens and the retained hold answer immediately re-closes it

Repro: `1789216953-ai_baka_deck125-0x55ed042111c0-vs-ai_baka_deck162.jsonl`, four
`hold_event` records at seqs **18, 20, 22, 24**, all `what: reopen_new_lethal`,
`seam: cast`, `window_seq: 14`, `face: [stack death verdict: the stack KILLS you]`.
The seat log has **no prompt-bearing record after seq 17**; `gameend` at seq 26,
`won: false`, `my_life: 0`. `game-125v162-1789216950.stderr` lines 1289-1309 print, four
times:

```
AIPlayerGPT: hold re-opened at the cast seam - a NEW lethal threat: the objects behind it are not the ones this hold was taken over
AIPlayerGPT: the model took the hold row at the cast seam on turn 16 - later cast windows are held until one of these rows changes
```

with no `-> chose` between them. DH F3 shipped the event; the seat is still never asked.
Fix direction: a re-open that is immediately re-clamped by the same latch is not a
re-open — the re-open must invalidate the retained answer for that key before the hold
check runs, the way lane DI bounds the priority cache.

### HIGH-2 — an instant in hand cannot be cast in response, and the seat is told so 200 times

Repro: `1789216920-ai_baka_deck152-0x55a5a97d7280…` seq 101, hand line verbatim:

```
Fateful Absence {1}{w} [instant] [no cast row now: you already answered this phase's Casting decision, so this window does not offer it - it is otherwise castable and its own casting window this turn has passed]
```

on a window whose own frame reads `ON THE STACK … 1 (top): opponent's Essence Scatter
{1}{u} (instant) [spell] targeting Briarbridge Tracker` with `Mana available: 6 total`.
**250 prompts carry that clause; on 200 of them the gated card is an INSTANT; on 61 of
those the stack is non-empty.** The once-per-phase casting decision therefore removes
instant-speed interaction — including responses to a counterspell on the stack — from
the seat for the rest of the phase. This is the window that produced the corpus's only
unparsed reply: the model tried to respond and there was no row to respond with. I did
NOT determine whether this is a deliberate seam bound or a defect in
`FindCardToPlay`'s phase gating; the render is honest about the restriction, the
restriction itself is not MTG.

### HIGH-3 — the crack-back verdict line contradicts its own screen on 11 windows

The verdict is computed off the raw combat total; the header's own `ADD THOSE UP`
clause then adds animated creature-lands and trigger power and the verdict is not
recomputed. 6 `you survive` verdicts on screens whose header says `that would KILL you`
(`152v130` seqs 17, 19, 22, 24, 25; `162v130` seq 43) and 5 `blocked away` verdicts
whose floor is flipped by the same addendum (`146v162` seqs 21, 22, 31, 32, 33).
Quote, `152v130` seq 17 (life 20):

```
… ADD THOSE UP: the numbers named there come to 5 more damage, so the total to subtract from your life is 20 and you would be at 0; that would KILL you, and lower still
[crack-back verdict: you survive - … you are still alive afterwards: the crack-back alone does not end the game]
```

This is the class the brief names: a new render clause stating a false fact. Fix
direction: `w80CrackBackBestBlockFloorNow` and the survival face must take the same
total the CRACK-BACK line's own `ADD THOSE UP` sentence publishes, or the verdict must
say which total it used.

### HIGH-4 — the send-time counters over-count and the record faces under-stamp

`crackback_verdict_lines_rendered` 382 vs 329 prompts carrying the literal (excess in
20 of 21 seats) and `own_loop_verdict_lines_rendered` 20 vs 18;
`crackback_lethal_blocked_away` 26 vs 24 rendered lines vs 14 stamped records; 38
prompts carry `[crack-back verdict:` with no `crackback_verdict` field, **36 of them
`priority` records**. DH F10 claimed exact equality for all four counters and exact
co-presence for the faces; two counters and both face families miss. The direction is
consistent — a suppressed path still counts, and the priority seam does not stamp — so
this is one boundary, not four bugs.

### MED-1 — a block trigger is accepted as a live loop starter

`1789216933-ai_baka_deck126-0x55559c0ca2d0` seqs 13, 15, 21, 22, 23, 29, 32, 38, 39 (and
`126v162` seq 61): `[own loop verdict: your life loop is a proven win … It starts from
Perimeter Captain: one point of life THEY lose or YOU gain closes it]` — Perimeter
Captain is a 0/4 defender whose trigger fires only when it blocks, so the entry is not
one the seat can take. The seat lost that game at 0 life on turn 22. F6's "any other
trigger belongs to the turn cycle and stays live" is too wide for triggers that need the
OPPONENT to act. I did NOT verify whether the opponent attacked into those blockers.

### MED-2 — the prompt budget moved backwards

Corpus-wide mean prompt 12,029 → **13,124 B (+9.1%)**; share over 20 KB
**5.6% → 13.4%** (124 → 287 prompts); max 25,471 → 27,189. deck152 +3,743 B/prompt
(2 → 85 over 20 KB) and deck125 +2,882 B/prompt (49 → 157). U16's −91 B is 8% of what
the wave's new render added. Per U17 this costs nothing in latency, but it is a real
context cost and the next byte item should be scoped against the +1,095, not against
the hold apparatus.

### MED-3 — a multi-row answer is silently reduced to its first row

`1789216920-ai_baka_deck152-0x55a5a97d7280…` seq 217 (`CHOICE: 1, 2, 7, 8, 9, 11
(Boulderloft Pathway, Forest, Plains #1, Plains #2, Plains #3, Hengegate Pathway)`) and
seq 237 (`CHOICE: 5, 6, 7, 8, 9, 10 (…)`), both `parse_note: multi_answer_first_taken`,
both on Emrakul's Annihilator's sacrifice-6 trigger. The parser takes row 1 and the
window says nothing; the model plainly believes it is choosing all six at once. The seam
asks one card per window and the prompt never says so. This is new telemetry that no
lane predicted and no counter reports — `multi_answer_first_taken` is not in the
gameend census.

### MED-4 — six board-sweep markers price the seat's own losses as a bare integer

`1789216937-ai_baka_deck130-0x562b68836ca0` seqs 49, 52, 55, 57, 69, 75 — e.g.
`[<- board sweep: THEIRS 4 / YOURS 5 - the only row on this menu that prices a board
sweep (it takes more of YOURS than of THEIRS)]` with no roster. U7's fix covers the
fixed-effect sweeper; the X-variable path (`{X pricing: …}`) builds its own marker and
was not wired in. Mitigated by the same row's X-pricing clause naming the victims.

### MED-5 — six wave-80 counters have no per-record trace

`ask_key_continuation_differs` (112), `hold_windows_skipped` (1,051),
`plan_cast_steps_closed` (376), `mana_only_windows_skipped` (78),
`main_phase_windows_skipped` (70), `own_turn_windows_skipped` (722) exist only on the
42 `gameend` records. The wave-79 LESSON ("every counter has a per-record trace") is
applied to the new hold/menu/forced-close families and not to these; three of the
brief's own sampling requests could not be answered because of it.

### LOW-1 — a cycling row under no discard punisher says nothing about discarding

All 9 priced cycling rows (`162v130` seqs 76-103) read `[DRAW PRICE: this draws 1 card
…]` with no discard term. U6's GREEN says a discard with no draw punisher is priced on
its own; corpus-wide `DISCARDS 1 card` and `a CYCLING cost is a discard` render 0 times,
so the discard half appears to be gated on a PUNISHER being present rather than on the
cost being a discard. Harmless here (no punisher, no life cost) but it means a model
reading the row still cannot tell that cycling discards.

### LOW-2 — the repeat-menu (`x<N>`) exemplar still quotes a live row

5 prompts: `e.g. "CHOICE: 19 (Create human with Thraben Doomsayer x<N>)"`,
`"CHOICE: 20 (Equip with Lightning Greaves targeting Human #1 x<N>)"`,
`"CHOICE: 2 (Create vampire with Bloodline Keeper x<N>)"`. U5 converted the ask seam
and the ANNOUNCE_X template; this seam was missed.

### LOW-3 — `prompt-byte-census.py` crashes after printing its tables

`TypeError: 'int' object is not subscriptable` at line 297 (`tot["n"]` where `tot` is an
int), on both the wave-79 and wave-80 corpora. The per-deck and HOLD APPARATUS blocks
print first, so the tool is usable, but its corpus-total line never prints and its exit
status is 1 — a harness step that gates on exit status would read this as a failure.

---

## 5. RETIRED

No lategame specimen is written (owner ruling 2026-09-12: "Stop producing these.").

---

## What I did NOT check

* **No reasoning bodies were read.** DF U3(b)'s "0 reasoning bodies that compute a cover
  from blocker TOUGHNESS" and the reasoning-body halves of N14a / N14d / O20 are
  unadjudicated.
* **BS F2 / BS F6 / CF-era F2 / F5-nonlifelink / F8-haste / F9** — this brief carries no
  repro definitions for them and I did not open the BS/CF lane files, so I recorded
  their populations and did not grade them.
* **The 5 h 58 min pool runtime** — `results.tsv` records only start epochs and I did
  not have the harness run log; I measured only that the 21 games started within 40 s
  and that the rerun started 6 h 51 m later.
* **`plan_cast_steps_closed` = committed casts** — no per-record or sidecar join exists;
  only the stderr line does, and I did not count stderr cast events across 20 files.
* **Whether HIGH-2 is a deliberate seam bound or a defect** — I did not read
  `AIPlayerGPT.cpp` or `FindCardToPlay`; the finding is from the rendered prompts and
  the one refused reply.
* **Whether the opponent ever attacked into Perimeter Captain / Pride Guardian**
  (MED-1) — I read the own-loop lines and the battlefield frames, not the combat log.
* **I ran no game, no build and no git command**, and wrote only this file.
