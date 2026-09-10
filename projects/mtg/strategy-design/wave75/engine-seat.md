# Wave 75 — engine-narration seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-003556-final`, **SINGLE BINARY**
`archives/wagic-7a04cd6da-w75step1` (source `14fea99f3`, lanes CI+CJ+CK+CL+CM).
21 games / 42 seats / 984 turns, `--thinking on`, `WAGIC_GPT_TIMEOUT=900`, no operator token
ceiling. Read-only: no build, no run, no git, nothing written outside `wave75/`.

**What I did NOT check.** No deck-guide reading and no strategy verdicts (deck seats own those).
No Scryfall/Gatherer calls — card facts come from the rendered prompt and, where a mechanism
mattered, from `src/AIPlayerGPT.cpp` / `src/ManaEngine.cpp` / `include/MTGDefinitions.h`.
PARSETEST/suite numbers are quoted from the lane files, never re-run; no lane fixture or probe
was re-run; no `archives/` binary was run. I did not read 2,919 reasoning bodies — every
prediction whose subject is a reasoning-body claim is marked UNTESTED. I did not re-derive
`serializeGameState()`. **I could not adjudicate the `[hold check: …]` bracket exactly**: the
engine measures it against the last window it BUILT (`mLastMenuRows`, `holdReopenNote`), and
3,402 windows this corpus were built without ever reaching the model (1,601 hold-skipped +
1,801 replayed), so the translog does not contain the bracket's own reference set — §2(a) gives
a bounded reading and says which direction the bound runs. I did not measure the F1
`other!creature` animated-Sorin path, F7's infect path, or F5's two-arm interleave directly:
none of the three occurred in this corpus. `askreplay/` sidecar contents were counted, not read
line by line. I did not diff every hold-bracket render for drift classes beyond the one I name.

---

## 1. Corpus census

| | wave 75 | wave 74 |
|---|---|---|
| seats / games | 42 / 21 | 42 / 21 |
| completed naturally | 42 gameend, 21 `results.tsv` rows, **0 hang, 0 crash, 0 timeout, 0 rerun** | 42 / 21 |
| **decisions (records carrying a `prompt`)** | **2,919** | 2,396 |
| by kind | ask 2,118 · priority 603 · attackers 102 · discard 53 · blockers 27 · reveal 10 · bottom 6 | ask 1,866 · priority 367 · attackers 93 · discard 40 · blockers 17 · reveal 12 · bottom 1 |
| `kind: recovery` (no prompt) | 0 | 4 |
| summed gameend turns / **decisions per turn** | 984 / **2.97** | 914 / 2.62 |
| same, ask seam only | **2.15** | 2.04 |
| **fallbacks, all kinds** | **0** | 5 |
| `transport` | `curl=0,http=200,empty=0` on **2,919 / 2,919** | 2 records `http=400` |
| `http_error_body` | 0 records | field did not exist |
| `async_drops` | **70** (`answer_replaced` 0 on all 2,919) | 84 |
| `deadline_pct` | p50 8.6 · p95 20.6 · max 48.4 | p50 8.9 · p95 21.1 · max 54.4 |
| latency | p50 **77.8 s** · p95 186.3 · max 439.0 | p50 80.5 · p95 190.3 · max 489.7 |
| `reasoning_chars` | min 571 · p50 5,734 · mean 6,156 · p95 10,289 · max 26,686 | 1,387 / 5,980 / 6,404 / — / 24,734 |
| `reply_truncated` | **0** | 1 |
| game length (turns) | mean 23.4, median 19, **max 83** | mean 21.8, median 19, max 52 |
| wins | 152 5/6 · 146 5/6 · 162 3/6 · 125 3/6 · 126 2/6 · 130 2/6 · 123 1/6 | 152 5/6 · 146 5/6 · 162 4/6 · 123 3/6 · 126 2/6 · 130 1/6 · 125 1/6 |

**⚠ The brief's census line is the ASK SEAM again — third wave running.** `2,118 decisions`,
`71 off_protocol_bytes>0 (3.4%)`, `83 plan_line_missing (3.9%)` and `decisions/turn 2.15` are,
to the digit, the statistics of the 2,118 `kind: ask` records alone. Corpus-wide: **2,919**
decisions, **92** off-protocol (3.2%), **104** plan-line-missing (3.6%), **2.97** decisions/turn.
The gameend counters already say so: `protocol_replies` 2,919, `plan_line_missing` 104.

**Reasoning (invariant 000).** 2,919 / 2,919 records carry `thinking: on` and a non-empty native
`reasoning`; 0 empty. The harness's own gate agrees (`REGIME GATE PASSED: thinking=on: 210
records, all carry reasoning. seats: 42 shapes: two_line=210`). **The corpus is valid under 000.**
Rescue markers: `reasoning_forced_close` 15 = `reasoning_budget_hit` 15 = `retry: 1` 15, and
`phase2_answer_recovered` 15 + `phase2_answer_missing` 0 = 15. All 15 recovered a coded answer
(`coded_answers` 1 on every one); none carries a fallback.

**Reply shape.** `protocol_replies` 2,919. `plan_line_missing` **104 (3.6%)**,
`off_protocol_bytes>0` **92 (3.2%)**, overlap 89. `protocol_deviation` partitions them exactly:
`compliant` 2,812 · `unlabelled_plan` 89 · `plan_absent` 15 · `prose_outside_two_lines` 3 —
**107 non-compliant = `protocol_deviation_replies` 107, to the record.** By seam (plm / offp):
ask 83 / 71, attackers 16 / 15, priority 3 / 4, discard 2 / 2, blockers + reveal + bottom 0 / 0.
Attackers is again the worst rate (**15.7% plm**, wave 74: 10.8%) — the one seam that got worse.
Off-protocol byte weight: n=92, sum 8,497 B, median 73, max 838. `action_before_plan_replies` 0,
`answer_replaced` 0, `post_answer_overrun>0` on **1** record (`125v126` deck125 seq 345,
`extra_answer_line`, two `CHOICE:` lines for a one-answer window; the parser took the first).

**Ceilings / truncation.** `max_tokens_answer`: ask 288 (2,106) / 400 (12 retries); priority 288
(601) / 400 (2); attackers 256 (100) / 288 (1) / 400 (1); blockers 256 (27); discard 256 (52) /
400 (1); reveal 256; bottom 512. **`reply_truncated` 0** (wave 74: 1). `dropped_assignments` 0 on
all 27 blockers records, so BS F5's doubled allowance is **UNTESTED for the fifth wave**.

**`askreplay/` sidecar.** 33 files, **1,801 records** = summed `ask_replays_reserved` 1,801,
exactly. `why`: `repeat_ask_reserved` **1,332** · `cache_replay` **469**. `ask_replays_refused` 0;
max `replay_run` 19 (`repeat_ask_reserved`) and 11 (`cache_replay`), cap 64 — the refusal path
was never reached and no `refusing the …` line appears in any stderr.

**`plan_steps_done`.** `(done,count)`: (0,1) 1,301 · (1,2) 1,119 · (1,3) 391 · (1,4) 83 ·
(1,5) 11 · (0,0) 7 · (2,3) 3 · (1,6) 3 · (2,4) 1. The pointer reaches ≥2 on **4** of 2,919
(wave 74: 11 of 2,396). Unchanged and slightly worse: the model re-states its plan far more
often than it walks it.

**Prompt size.** p50 13,229 B · p95 27,434 · p99 30,695 · **max 80,805** (`125v123` deck125
seq 394, discard, t77). Five prompts exceed wave 74's whole-corpus maximum of 34,315 B. See
HIGH-2 — this is a new, unbounded enumeration, not the O18 row population.

---

## 2. REGRESSION WATCH — the wave-75 changes

### (a) The loop family — dead, and the hold-check bracket is the one thing I cannot certify

**Asks per (turn, rendered phase), every seat.** The `phase` field is not the rendered phase
(wave-59 method note), so the phase is read out of each prompt's own
`Phase: X | It is your/the opponent's turn.` header. **18 windows exceed 8 asks; 6 exceed 10;
the maximum is 19.**

| asks | seat | (turn, phase) | genuine chain? |
|---|---|---|---|
| 19 | `125v123` deck123 | (77, Upkeep, opponent's) | **YES** — 7 of the 19 are priority actions that MOVE the board (Create vampire ×2 with Bloodline Keeper, Transform:backside, Create vampire ×2 with Lord of Lineage, a repeat-batch row); the interleaved cast windows re-put because the board moved under them. `events` non-empty on 13 of 19 |
| 14 | `146v125` deck146 | (21, Main 1, own) | **YES** — five distinct casts, a dungeon venture chain, a land drop, two modal menus. Every window a different `options_text` hash |
| 14 | `125v123` deck123 | (76, Main 2, own) | **YES** — Equip, Create vampire ×2, a repeat batch; 4 distinct priority takes |
| 13 | `152v126` deck152 | (11, Main 1, own) | **YES** — an MDFC land drop, two casts, and a Teferi `+1` tap/untap chain walking land → creature → artifact, each a different menu |
| 12 | `146v123` deck146 | (19, Main 1, own) | **YES** — land drop, two casts, a Lolth `-3`, one hold taken |

No re-put window (board unchanged, list unchanged, count climbing) appears anywhere. **The CG
and CH loop families are dead**, and the corpus's own tripwires never fired (§2(n)).

**The O11 in-row tag is gone.** `declined this exact list` appears on **0 option rows**
corpus-wide (wave 74: on every plain decline row, taken 284/561 = 50.6%). CI P1a **PASS**.

**The fact still rides the prompt-only note**, 783 renders, N distribution 1:426 · 2:149 · 3:65 ·
4:43 · 5:30 · 6:25 · 7:19 · 8:17 · 9:7 · 10:2. **Per-seat maximum is 10** (`125v123` deck123);
8 of 32 seats that render it at all exceed 5. CI predicted "no seat above 5" — that clause
**FAILS**, but against wave 74's 23 / 13 / 12 / 10 it is a 2.3× improvement, and CI's own
diagnostic threshold ("past 10 with an empty `events`") is not crossed.

**The `[hold check: …]` bracket.** 2,026 renders: `every row above was also on the menu …
(N windows in a row now)` **1,454**, `N rows above are new` **572** (1:315, 2:144, 3:55, 4:34,
5:15, 6-23:8). The CH/CI carve-out sentence — *"a row that changes only in its annotations (a
price, a forecast, a clock, a count) is the SAME row"* — is present in **both** halves of every
render. Wave 74's ratio was inverted (368 "seen" vs 1,210 "new"); it is now 72% / 28%.

**The bound I cannot close.** `holdReopenNote` compares the menu against `mLastMenuRows[seam]`,
which is updated at every window whose prompt is BUILT — including the 1,601 hold-skipped and
1,801 replayed windows, none of which produced a record. So the bracket's referent is frequently
a window the model never saw. Re-implementing `holdActionKeyRow` in Python (strip `[...]`, strip
non-mana `{...}`, drop `(land…)`, lower-case + collapse, then append the `[cost: …]` groups per
`#W75-CM` F2) and comparing each window against **the previous RECORD at the same seam**:

* **6 of 2,000** say `N rows above are new` where the previous record's key set already held every
  row. All six are single-window overshoots.
* **214 of 2,000 (10.7%)** say `every row above was also on the menu at the last window at this
  seam` while the previous record at that seam shared **no** key with one or more current rows.
  The cleanest instance, verbatim — `126v146` deck126, priority seam, seq 28 (t14) → seq 33 (t16):

  ```
  seq 28 menu: 11 x "Deal 1 damage with Staff of Nin targeting <...>" + the hold row
  seq 33 menu: "+1: create a 1/1 vampire with Sorin, Lord of Innistrad [cost: Counters]"
               "-2: emblem: \"creatures get +1/+0\" with Sorin, Lord of Innistrad targeting
                Sorin, Lord of Innistrad [your battlefield] [cost: Counters]"
               "-6: destroy up to three and reanimate with Sorin, Lord of Innistrad targeting
                Silverquill Silencer [opponent's battlefield] [cost: Counters]"
               + the hold row
  seq 33 bracket: [hold check: every row above was also on the menu at the last window at this
                   seam (1 window in a row now) - a hold taken here holds until one of them
                   appears, disappears, or names a different card, cost or target; ...]
  ```

  Not one Sorin row was on the seq-28 menu. `holdNoteSameWindow` cannot explain it (it reuses a
  cached note only when `unseenRows == 0`), so the engine genuinely measured 0 unseen — against
  a menu built at an intervening window the model was never shown.

  The hold row was taken at **9** of those 214 windows, so the measured cost is small. Whether
  each of the 214 is a false ENGINE fact or a true fact about an invisible referent, the sentence
  the model reads — *"the last window at this seam"* — names something the model does not have.
  See MED-1. CI's P1b prediction is therefore **PARTIAL/UNTESTED**, with the direction of the
  residual bounded above by 214.

### (b) `hold_released_turn` = 115 — complete enumeration, **no hold outlived its holder's untap**

All 115 `the hold is RELEASED at this seat's untap` traces parsed. Span from take-turn to
release-turn: **their-turn takes → +1 (76 of 76), own-turn takes → +2 (39 of 39)**. Both are the
minimum possible span. **0 off-minimum.** Hold takes: 253 at the cast seam +… (stderr splits
213 cast / 61 priority takes = 274 arms, against 253 recorded takes; the difference is takes on
windows served from the replay path). `hold_windows_skipped` 1,601 = the exact count of
`holding priority at the … seam` stderr lines (cast 1,075 / priority 526).

### (c) `forced_close_unrecorded` = 9 — the accounting identity holds in **every one of the 21 games**

`unclosed <think> (budget/truncation); forcing the answer` appears **24** times in the stderr.
Per game, `phase2_answer_recovered + phase2_answer_missing + forced_close_unrecorded` vs the
game's own close count:

| stderr | recov | miss | unrec | closes |
|---|---|---|---|---|
| `146v126` | 2 | 0 | 0 | 2 |
| `125v123` | 0 | 0 | **2** | 2 |
| `126v130` | 1 | 0 | 0 | 1 |
| `123v130` | 1 | 0 | 0 | 1 |
| `125v130` | 1 | 0 | **2** | 3 |
| `125v162` | 1 | 0 | 0 | 1 |
| `146v130` | 1 | 0 | 0 | 1 |
| `125v126` | 2 | 0 | 0 | 2 |
| `162v123` | 0 | 0 | **1** | 1 |
| `152v123` | 0 | 0 | **1** | 1 |
| `126v123` | 5 | 0 | 0 | 5 |
| `162v130` | 1 | 0 | **3** | 4 |
| the other 9 games | 0 | 0 | 0 | 0 |
| **total** | **15** | **0** | **9** | **24** |

**`recovered + missing + unrecorded == closes` in 21 of 21 games, and 15 + 0 + 9 = 24 corpus-wide.**
CJ P2c's identity — untestable until a corpus ran — **PASS, exactly**. CM F5's per-arm flag is
what makes it hold.

**Was a decision lost on any of the 9?** No. Reading the stderr around each of the nine closes:
every one is followed either by `dropping stale async answer (casting arm; the question (or
turn/phase) moved) [outcome: re-asked]` or by the seat's own re-serve/`consuming an in-flight
answer whose prompt text drifted` line, and the corpus carries **0 fallbacks of any kind** and
**0 `phase2_answer_missing`**. The 9 are closes whose rescued answer arrived after the window it
belonged to had moved on — a wasted round trip (≈ 9 × 80 s), not a lost decision. Named arms:
`125v123` casting ×2, `125v130` casting ×2, `162v123` land-drop ×1 (the trace names
`land-drop ask NOT issued` immediately around it), `152v123` casting ×1, `162v130` casting ×3.
**MED-2**, not HIGH. CJ predicted 0 and CM F5 predicted 0-where-the-identity-holds; the identity
holds *with* the 9, so CM F5's own reading ("a non-zero value now means a genuinely lost close")
is the half that is wrong — a parked-then-superseded close is counted and is not a loss.

### (d) P2 — the HTTP 400 is gone

`transport` is `curl=0,http=200,empty=0` on **2,919 / 2,919** records. **0** records carry
`http_status: 400`; **0** carry `http_error_body`; the string `continue_final_message` appears in
no record and no stderr. `phase2_answer_missing` 0 (wave 74: 2). CJ P2 and CM F6 **PASS** — with
the caveat that the fix's own diagnostic (the logged 400 body) had nothing to log, so the Unicode
half of F6 is proven only by construction.

### (e) P21 / F4 — the arm fired 15 times and never on a truncation

**0 `reply_truncated` records**, so the literal prediction ("every `reply_truncated` record
carries `retry` + `reasoning_forced_close`") is vacuous. The arm itself is live: 15 records carry
`reasoning_forced_close` + `retry: 1` + `reasoning_budget_hit`, `reasoning_chars` 18,414-26,686,
and **all 15 recovered a coded answer** (`coded_answers` 1). None is a fallback; none carries
`unparsed_reply`. The wave-74 64-character cut does not recur. **PASS on the behaviour, the
`reply_truncated` clause UNTESTED.**

### (f) P9 / F3 — the inline `CHOICE:` never occurred

**0** replies in 2,919 put a `CHOICE:` label on a `PLAN:`-leading physical line. The gate cannot
be adjudicated from this corpus in either direction: CJ's P9 prediction and CM's F3 prediction are
both **UNTESTED**. (Nothing was mis-consumed either: `coded_answers` ≥ 2 on 0 records outside the
one `post_answer_overrun`.)

### (g) P13 — `main_phase_windows_skipped` is still 36% false, and the pair signature is back

All **94** skip traces come from the single arm CI identified
(`the instant-speed response arm answered it`). Cross-referencing every counted `(turn, phase)`
against `Casting decision (Main phase N, YOUR turn)` headers in that seat's own translog
(phase 4 = `MTG_PHASE_FIRSTMAIN`, phase 10 = `MTG_PHASE_SECONDMAIN`, `include/MTGDefinitions.h`;
the header occurs at most once per record, verified):

* **34 of 94 (36%) are FALSE** — a Casting decision WAS put at that turn and that main phase.
  Wave 74: 28 of 72 (39%). Worked example, `146v125` deck146: stderr counts
  `own main phase (turn 3, phase 4) ENDED with NO casting window` while **seq 5** of that seat's
  translog is `Casting decision (Main phase 1, YOUR turn)` at turn 3. The same seat is counted
  falsely at turns 3, 5, 7, 9, 11, 13.
* **The (turn,4)/(turn,10) pair signature — the signature CI named as the overwrite's
  fingerprint — is present on 46 of the 47 turns that carry any count**, and the phase split is
  exactly 47 / 47.

**CI's P13 prediction ("0 false counts … and no (turn,4)/(turn,10) pair") FAILS on both halves.**
The two new pure predicates are pinned; the six lines of member plumbing that call them are the
part CI flagged as untested, and the corpus says the plumbing is where the defect lives. MED-3.

### (h) P18 — the land drop is one window, with a row per face

**28** land-drop menus render a `{TWO FACES, ONE CARD: …}` row. Parsing each into
(FRONT row → back-face name) and (BACK row → front-face name) pairs: **28 of 28 menus match
exactly** — every two-faced land in hand contributes both a FRONT row and a BACK row, and no
menu offers one face without the other. **0 rows carry `OTHER FACE of` anywhere in the corpus.**
**0 `Choose an option for <card>:` prompts are a face menu**: the 51 such prompts are
Teferi (29), Emeria/Agadeem's pay-3-life ETB choice (8), Lay Waste (5), Silverquill Command (3),
Spark Spray (3), and one each of five more — none with a face row. CI P18 **PASS**.
`declined_face_latches` 0 for the fifth wave (BS F3 **UNTESTED**).

### (i) P19 — the hold row is cut, true, and the paragraph is exactly once per prompt

2,076 of 2,919 windows render the hold row. **Hold row bytes: min 489, median 593, p95 593,
max 724** (was 1,074-1,287). **6 windows** exceed CI's 650 B claim (723-724 B, at `123v125`
seq 1276, `125v126` seqs 111/112, `126v125` seq 228, `130v146` seqs 49/50) — the prediction's
"under 650 B at every window" **FAILS by 6 of 2,076**; the substance passes.

`[HOW A HOLD ENDS:` appears **exactly once in every one of the 2,076 prompts that render the row,
and in none of the 843 that do not** — 0 anomalies. The first-draft miss CI caught on the probe
did not recur at either seam. CI P19 **PASS**.

**Hold takes did not fall.** 253 takes over 2,076 hold-row windows (12.2%). On CI's own
comparator (deck146, wave 74: 11 of 322 decisions): deck146 this corpus takes **12 of 330**
decisions — flat. The truth-and-size change cost no persuasion; the item does not reopen.

### (j) New / changed render clauses stating a FALSE fact

| item | measured | verdict |
|---|---|---|
| **P5** `you would be at 0 - this row feeds their chain` | 22 rows across 22 records, **all** on `125v126` deck125, t25-t49. Every one carries **both** `LOOP COMPLETE:` and `[LOOP RUNNING:` in the same prompt, so the loop is proven live by the row's own predicate. **0 rows** anywhere render `{right now: … you would be at N}` with N > 0 while a loop paragraph is live (wave 74: 16). The seat took the self-ping **0 times** and **won that game 38-0 on turn 49** (wave 74's same matchup lost it 42-7 to exactly this row) | **PASS** |
| **P6** `CRACK-BACK NEXT TURN` | own-turn present/absent: Main 1 **317/927**, Upkeep **103/346**, Attackers 46/56, Main 2 19/224, Draw **12/20**, Blockers 10/29, End 10/115, Cleanup 6/48, Combat begins 6/6. Opponent's turn: **0 renders on all 12 phases**. Wave 74: Upkeep 0 of 384, Draw 0 of 54 | **PASS**, falsifier not triggered |
| **P10** own-clock | **174** `reaches 0 in N more turns` renders. **0** sit in a prompt whose `ON THE STACK:` header says `KILL` (wave 74: 3). 57 render under a live loop paragraph; 19 of those also carry a `feeds their chain` row — but that row is an OPTIONAL payment the seat may decline, not an incoming loss, and the seat's life is 28-38 in all 19, so the clock is TRUE. The engine gate reads `pendingStackLifeLossToSeat`, and no live-loop window in this corpus states an incoming nonzero loss | **PASS** on the stack half; the loop arm **UNTESTED** (its trigger never occurred) |
| **P11** keep-X | **88** clauses of the exact `… in your hand needs N: no X on this row leaves it payable` shape. **0** name the row's own card without `another copy of`. The new wording DOES fire — 37 prompts carry `another copy of Starstorm {r}{r}{x} in your hand needs 2 …` (deck130) | **PASS** |
| **P15** X ladders | The range paragraph now states **both** walks in all 105 prompts that carry it (`"down to" counts down and "up to" counts up`, both worked examples present). **0** monotone-X range ROWS occurred this corpus (21 prompts carry a bare `X = N` option row, none a collapsed range), so no pairing could be wrong | **PASS on the note; the pairing UNTESTED** |
| **P16** short names | **92** rows carry `, repeated then stop`. **0** printed short names end inside an unclosed parenthesis. **0** repeat-run RANGE lines rendered at all (no menu this corpus reached a board big enough to fold a run), so the #9→#10 split could not recur and the fold-key fix is **UNTESTED** | **PASS (vacuous) / UNTESTED** |
| **P7 + F8** crack-back cover | **41** rows carry `{crack-back cover: …}`, **7** carry `[legendary: you already control …]`, and **0 rows carry both**. The wave-74 Rorix shape does not recur; the first copies still price their body | **PASS** |
| **P3** payability | **7** prompts have an Overgrown Battlement in `Those sources, one per untapped card:` while Exquisite Blood is in hand. **0** of them tag Exquisite Blood `[cannot pay now]`. Corpus-wide, **0** `cannot pay now` tags appear in a prompt that lists a Battlement among the untapped sources. Exquisite Blood cast rows were chosen 15 times. CK predicted 0 | **PASS** |
| **P4 + F1** Sorin | **6** `-6: destroy up to three and reanimate` target rows offered, targeting Silverquill Silencer ×4, Kaya ×1, own Wall of Omens ×1 — **none targets Sorin**. The `-6` was consumed once (`126v146` deck126 seq 39). **0** logs contain `Sorin … was put into your graveyard` → `entered the battlefield from your graveyard`. **0** seats take two Sorin loyalty abilities in one turn (12 `+1` takes, 1 `-6`, all on distinct turns). No animated planeswalker occurred, so F1's `other!creature` exemption is **UNTESTED** | **PASS on P4; F1's animated half UNTESTED** |
| **P22** parser noise | **0** `ABILITYFACTORY ERROR` lines and **0** `Parser returned NULL` lines of any kind in any stderr (wave 74: six false `{t}:add{g}` / `{t}:add{w}` indictments per pathway). What prints instead, 16 times: `INFO ABILITYFACTORY: line not available in this zone (by design): {0}:doubleside(<back face>)` for Grimclimb (5), Mistgate (6), Tidechannel (3), Boulderloft (2) | **PASS** |
| **F7** infect | **0** prompts mention infect or poison; 0 rows print `infect damage` or a `POISON COUNTERS` forecast. Astra's board is constructed and no corpus game contains an infect source | **UNTESTED** |
| ghostform gloss (O1 carry) | **919** ghostform lines across 264 prompts, **all** the full gloss; **0** render `a marker only` | **PASS**, third wave |
| `[cost:` on reveal rows | 10 reveal records, **0** rows carry a `[cost:` group | clean |

### (k) `async_drops` = 70 — complete enumeration, **no decision lost**

70 gameend-summed drops = **70** `dropping stale async answer` stderr lines, and every one carries
`[outcome: re-asked]`:

| arm / reason | count | outcome |
|---|---|---|
| casting arm; the question and board moved | 62 | re-asked |
| (question or turn/phase) moved | 5 | re-asked |
| casting arm; the board moved | 2 | re-asked |
| land-drop arm; the question and board moved | 1 | re-asked |

**0 to the heuristic**, `answer_replaced` 0 on all 2,919 records, 0 fallbacks. Wave 74: 84, same
result. CF F2's decode-garbage retry path is **UNTESTED for the third wave** (all 15 retries this
corpus are `reasoning_forced_close`).

### (l) `identical_ask_answers_reserved` 1,332 / `ask_replays_reserved` 1,801 — the CG-era replay, restored and then some

Read off `src/AIPlayerGPT.cpp` (~:39215 and ~:39255): the ask seam consults **two** caches in
order. (1) `mAskCache`, keyed on `askKey` = board key + rendered tail — a hit increments
`mAskReplaysReserved` only and logs `cache_replay`. (2) only if (1) misses,
`repeatAskAnswerStands(mRepeatAskKey …)`, keyed on turn + phase + question + **rows** and gated on
`mRepeatAskPlan == mCurrentPlan` — a hit increments **both** `mAskReplaysReserved` **and**
`mRepeatAskAnswersReserved`, and logs `repeat_ask_reserved`.
So **`ask_replays_reserved` = both paths; `identical_ask_answers_reserved` = path (2) only.**
The sidecar splits **1,332 / 469**, which matches the two counters exactly, and the stderr carries
1,332 `the same ask again, unchanged - re-serving this seat's own answer` lines.

**1,332 is the CG-era replay restored on path (2), and CI P1a is why.** Path (2)'s key contains
the ROWS. In wave 74 every plain decline row carried `you have already declined this exact list N
times this turn`, whose N moved on every answer, so path (2)'s key was byte-different at every
window and it hit **once in the whole corpus**. Removing the in-row tag makes the rows byte-stable
within a turn, and the latch now catches the drain loops it was built for. It is not a new window
being re-put: the corpus's own `(turn, phase)` census (§2(a)) shows no unchanged-board re-put run
anywhere, and `ask_replays_refused` is 0 with max run 19 against a cap of 64. Combined with the
1,601 hold-skipped windows, **3,402 windows never reached the model against 2,919 that did** —
the single largest saving of the wave, and the direct payoff of the O11 revert.

### (m) One deviation class, one upper-bound counter, two independent degeneracy meters

**`protocol_deviation_replies` 107 vs `plan_line_missing` 104 / `off_protocol_bytes>0` 92.**
Cross-tab over all 2,919 records:

| `plan_line_missing` | `off_protocol_bytes > 0` | records |
|---|---|---|
| true | true | **89** (`unlabelled_plan`) |
| true | false | **15** (`plan_absent`) |
| false | true | **3** (`prose_outside_two_lines`) |
| false | false | 2,812 (`compliant`) |

`protocol_deviation_replies` **107 = 89 + 15 + 3**, exactly the count of non-`compliant` records —
CL P23d's identity **PASS**. All three classes occur, so the partition is real. **The overlap is
83%, not the 96%+ the brief carries forward** (that figure is wave 74's 96/108 = 89%): the
`plan_absent` class grew from 10 to 15 while `unlabelled_plan` shrank 96 → 89, so reporting the
two fields separately now double-counts less and mis-classes more. Residue, quoted:

* `plan_absent` (15) — a bare answer line, no plan of any kind:
  `'\n\nCHOICE: 3 (Cast Silverquill Silencer)'`, `'\n\nATTACK: none'`,
  `'\n\nCHOICE: 3 (Cast nothing right now)'` (deck152 ×3 in one game), `'\n\nATTACK: A1, A2'`.
  Owner-ruled report-only.
* `prose_outside_two_lines` (3) — `126v146` deck126 seq 22 (**a forced-close record**: the phase-2
  prefill sends the model's own trace back, and the continuation re-states 823 B of it before the
  PLAN/ATTACK pair; the answer was consumed correctly), `125v126` deck125 seq 111 (838 B),
  `125v126` deck125 seq 345 (69 B — the two-`CHOICE:` overrun). **1 of the 3 is an artefact of the
  rescue, not a model deviation** — worth excluding from the class before it is read as a trend.

**`plan_names_uncastable_zone_card` 68 — an upper bound, and CL was right to say so.**
Re-deriving the trigger name (a name in the seat's own graveyard/exile, named in the plan, with no
card of that name in hand or on the battlefield) accounts for 47 of the 68:
Hammer of Bogardan 12 · Staff of Nin 11 · Thraben Doomsayer 9 · Triumphant Adventurer 5 ·
Sphinx's Revelation 3 · Bloodline Keeper 2 · Idyllic Tutor 2 · Devour Flesh 1 ·
Silverquill Command 1 · Siege-Gang Commander 1. **Hammer of Bogardan is not a defect** — it
returns *itself* from the graveyard to hand, so `PLAN: Return Hammer of Bogardan to hand to cast
it in main phase 1` (`130v125` seq 161, hand empty, 19 cards in the graveyard) is a correct plan.
That is ≈18% of the counter. Of a 10-record random sample, 3 were Hammer of Bogardan (false), 7
were plans naming a card in no castable zone (genuine). **Treat 68 as ≈55 genuine, and give the
counter a self-recursion exemption before any wave acts on it.**

**`plan_names_stranded_card` 5, all five quoted** (`126v162` deck126 seq 17 t11 / `126v123`
deck126 seq 22 t13 / `123v162` deck123 seqs 30 t10 and 39 t12 / `152v146` deck152 seq 13 t8):

1. `PLAN: Cast Tribute to Hunger now to remove a Shield Sphere and gain 6 life. Play a land in main phase 1. Cast Exquisite Blood to complete the loop. Attack with Vampires.`
2. `PLAN: Cast Tribute to Hunger to remove Bloodline Keeper and gain 3 life, then pass main phase until I can cast Sanguine Bond.`
3. `PLAN: Cast Tribute to Hunger to remove Fog Bank and reduce hand size for cleanup. In Main Phase 1, play Marsh Flats, cast Bloodline Keeper, tap it for a 2/2 Vampire token, cast Intruder Alarm to unta…`
4. `PLAN: Sacrifice the Vampire with Devour Flesh to gain life and reduce hand size, surviving the upcoming stack damage and mandatory cleanup discards; proceed to main phase to play a land and cast Bloo…`
5. `PLAN: Cast Fateful Absence on TA#1 now to reduce next turn's incoming damage. In Main 1, play Plains, cast Brutal Cathar, and attack with Wolf and Cathar.`

**`reasoning_ngram_repeat` vs `reasoning_degenerate` — CJ P20 PASS.**
`reasoning_degenerate`: median **0.0031**, max 0.107. `reasoning_ngram_repeat`: median **0.1230**,
max 0.986 — **40× higher at the median**, exactly the blindness CJ described. Spearman rank
correlation over all 2,919 paired values: **0.242** — not rank-correlated. On deck146 seats
specifically (CJ's named population): degenerate median 0.0034 vs ngram median 0.1393.
Nothing gates on either; both ride every record.

**P12 — the casting-seam chain census is live.** `chain_windows_only_selfharm` 11 /
`chain_selfharm_rows` 1,108 / `chain_acting_rows` 2,355 at the priority seam, and
`_cast` **482 / 692 / 1,445** at the casting seam. The cast triple is non-zero on its first
corpus, so the "the gate, not the seam, is wrong" branch of CI's prediction is closed.
CI's ~20% numerical claim compares ROWS to WINDOWS and is not adjudicable as written; the closest
comparable I can build off `options_text` is 353 windows whose every acting row carries a
decision-empty annotation, against `chain_windows_only_selfharm` + `_cast` = **493** (+40%).
8.2% of all acting rows (704 of 8,550) carry one. **PARTIAL.**

### (n) The harness

`~/.gatelogs/w75-corpus.log`: `== REGIME GATE PASSED: thinking=on: 210 records, all carry
reasoning. seats: 42 shapes: two_line=210 ==`, then `21 games, 0 timeouts/draws,
0 life-adjudicated at cap, 0 CRASHED, 0 HUNG - no winner, rerun owed`, then
`none of those classes appears in any seat log - every decision was answered by the model`,
`exit=0`. **No `PILOT STALL` and no `WINDOW LOOP` banner anywhere in the log or the `-final`
dir**, and `wall_miss_events` / `wall_miss_no_retry` / `wall_miss_unrecorded` are 0 on all 42
gameends, so CM F9's and CI P24(b)'s positive direction is **UNTESTED** — no false fire is the
half the corpus proves. The early check's 42-seat report is the brief's; the corpus log does not
carry it, so CI P24(a) is adjudicated from the brief, not re-derived by me. The log's
"3045 decisions" is all record kinds including `gamestart`/`system`/`gameend`; the decision count
is 2,919.

---

## 3. Prediction adjudication

### lane CI — window / hold
| # | prediction | verdict | evidence |
|---|---|---|---|
| P1a | the in-row O11 tag is gone from every render | **PASS** | 0 rows corpus-wide carry `declined this exact list` |
| P1a | no seat shows a prompt-only `declined this exact list N` above 5 | **FAIL** | 8 of 32 seats exceed 5; max **10** (`125v123` deck123). 2.3× better than wave 74's 23; CI's own past-10-with-empty-`events` diagnostic is not crossed |
| P1b | consecutive same-seam windows differing only inside brackets print `every row above was also on the menu`, N advancing | **PARTIAL / UNTESTED** | the form now dominates 1,454 / 572 (wave 74: 368 / 1,210) with runs advancing, and the carve-out sentence is in both halves of every render — but the bracket's referent is a window that produces no record, so §2(a)'s 214 disagreements bound the residual rather than resolving it |
| P8 | every own-main window following a countered spell with no re-ask carries the new trace with too few sources | **PASS** | 23 own-turn own-spell counter events across the corpus; **every one of those 23 turns carries at least one `Casting decision (Main phase N, YOUR turn)` record** (1-5 of them). The wave-74 shape does not recur, so the trace's negative case never had to be read |
| P13 | 0 false counts; no (turn,4)/(turn,10) pair in one turn | **FAIL / FAIL** | 34 of 94 false (36%); the pair appears on 46 of 47 counted turns; phase split exactly 47/47 — §2(g) |
| P18 | 0 `Choose an option for <card>:` face menus; both faces as rows on the drop | **PASS / PASS** | 0 of 51; 28 of 28 menus front+back matched |
| P19 | hold row under 650 B at every window | **FAIL by 6 of 2,076** | median 593, max 724 |
| P19 | `[HOW A HOLD ENDS:` exactly once per hold-row prompt, never otherwise | **PASS** | 2,076 / 2,076, 0 anomalies |
| P19 | hold takes do not fall below wave 74's 11/322 | **PASS** | deck146 12 of 330; corpus 253 of 2,076 hold-row windows |
| P12 | selfharm rows within ~20% of the all-dead window count; the cast triple is non-zero | **PARTIAL / PASS** | cast triple 482/692/1,445; the numeric claim mixes rows and windows — closest comparable +40% |
| P24a | the early check reports 2× the games started, no probe log | **NOT RE-DERIVED** | the corpus log carries no early-check block; taken from the brief |
| P24b | PILOT STALL does not fire while any seat answers | **PASS (no false fire) / UNTESTED (no wedge)** | §2(n) |

### lane CJ — transport / reply
| # | prediction | verdict | evidence |
|---|---|---|---|
| P2 | 0 records with `http_status: 400`; no `http=400` on any forced-close record | **PASS** | 2,919/2,919 `http=200` |
| P2 | `phase2_answer_missing` falls to 0 | **PASS** | 0 (wave 74: 2) |
| P2c | `recovered + missing + unrecorded == stderr closes` per game | **PASS, 21 of 21 games and 15+0+9 = 24 corpus-wide** | §2(c) |
| P2c | `forced_close_unrecorded` is 0 | **FAIL, 9** — but no decision was lost | §2(c); MED-2 |
| P2a | if a 400 recurs, `http_error_body` names it | **UNTESTED** | no 400 recurred; 0 `http_error_body` |
| P21 | every `reply_truncated` record carries `retry` + `reasoning_forced_close`, none a fallback | **UNTESTED (0 such records)** — the arm fired 15×, all recovered a coded answer, 0 fallbacks | §2(e) |
| P9 | 0 `unparsed_reply` whose reply has one `CHOICE:` on a PLAN-leading line | **UNTESTED (0 such replies; also 0 `unparsed_reply`)** | §2(f) |
| P9 | `post_answer_overrun` and `coded_answers >= 2` counts unchanged | **PASS** | overrun 1, no multi-answer consumption |
| P14 | every `repeat_count_missing` is followed by a `repeat_count_reask` on the same board | **UNTESTED** | **0** `repeat_count_missing` and 0 `repeat_count_reask` in the corpus; 92 `, repeated then stop` rows rendered and none was answered by a bare name. `repeat_annotated_takes` 2, both round-tripping (`repeat_annotated_take(n=1)`) |
| P20 | ngram median materially higher; the two not rank-correlated | **PASS** | 0.1230 vs 0.0031; Spearman 0.242 |

### lane CK — engine truth
| # | prediction | verdict | evidence |
|---|---|---|---|
| P3 | `cannot pay now` tags naming a cost the board's VARIABLE producers cover drop to zero | **PASS** | 0 of 28 Battlement-source prompts; 0 on Exquisite Blood in the 7 prompts where both conditions hold; 15 Exquisite Blood cast rows taken |
| P4 | no `-6` consumption is followed by a Sorin graveyard→battlefield return pair; no seat takes two Sorin loyalty abilities in one turn | **PASS** | 6 `-6` rows offered, none self-targeting; 1 consumed; 0 return pairs; 12 `+1` takes all on distinct turns |
| P7 | no row carries both `[legendary: you already control …]` and `{crack-back cover: …}`; first copies keep the cover | **PASS** | 0 of 41 cover rows / 7 legendary rows overlap |
| P22 | no stderr contains `ABILITYFACTORY ERROR: Parser returned NULL {t}:add{` | **PASS** | 0 `Parser returned NULL` of any form, 0 `ABILITYFACTORY ERROR`; 16 by-design INFO lines instead |

### lane CL — render truth
| # | prediction | verdict | evidence |
|---|---|---|---|
| P5 | 0 rows render a positive `you would be at N` under a live loop; each instead carries `this row feeds their chain` | **PASS** | 0 violations across 327 loop prompts; 22 rewritten rows, loop proven live on all 22; the seat declined every one and won 38-0 |
| P6 | the line renders on own-turn Upkeep/Draw wherever an able attacker exists; never on an opponent-turn window | **PASS / PASS** | Upkeep 103, Draw 12 (wave 74: 0 and 0); 0 opponent-turn renders on 12 phases |
| P10 | 0 renders in a prompt whose `ON THE STACK:` says KILL | **PASS** | 0 of 174 (wave 74: 3) |
| P10 | 0 renders under a live loop that states a nonzero loss landing on the seat | **UNTESTED** | 57 renders under a live loop; none states an INCOMING nonzero loss (the 19 co-occurring `feeds their chain` rows are optional payments, and the clock is true at 28-38 life) |
| P11 | 0 keep-X clauses name the row's own card without `another copy of` | **PASS** | 0 of 88; the new wording fires in 37 prompts |
| P15 | 0 prompts pair an `up to X =` row with a down-only paragraph | **PASS (vacuous)** | the note states both walks in all 105 prompts; 0 range rows occurred |
| P16 | one full row + one range line across a digit boundary; no short name ends inside a parenthesis | **UNTESTED / PASS** | 0 repeat-run range lines rendered; 0 of 92 short names unclosed |
| P17 | non-zero counter; the `125v152` seq-28/29 shape appears | **PASS on non-zero (68); the counter is a loose upper bound** | Sphinx's Revelation is 3 of the 47 I could re-derive; Hammer of Bogardan (12) is a self-recursion false positive — §2(m) |
| P23d | `protocol_deviation_replies` equals the non-compliant count; `unlabelled_plan` is the large majority | **PASS / PASS** | 107 = 107; 89 of 107 = 83%. The brief's carried "96%+ overlap" is wave 74's figure and does not hold at 83% |

### lane CM — the nine Astra findings
| # | prediction | verdict | evidence |
|---|---|---|---|
| F1 | a scoped `other!creature` keeps an animated Sorin legal while forbidding the plain self-target | **UNTESTED (animated) / PASS (plain)** | no animated planeswalker on any board; 0 of 6 `-6` rows self-target |
| F2 | no menu with two `[cost: …]` variants of one activation is served without the bracket reporting a new row; the `146v125` oscillation does not reappear | **UNTESTED / PASS** | no two-cost-variant menu occurred; no byte-identical-menu oscillation anywhere. `[cost: …]` groups ARE in the key (verified in source) and 1,955 of the corpus's rows carry one |
| F3 | no consumed `choice` sits inside a sentence that negates it | **UNTESTED** | 0 inline-CHOICE replies |
| F4 | `recovered + missing` falls relative to `coded_answers >= 1`; no forced close beside a phase-1 reply that already had an inline answer | **UNTESTED / PASS (vacuous)** | 15 closes, 0 inline answers |
| F5 | `forced_close_unrecorded` is 0 wherever the identity holds; non-zero means a genuinely lost close on one arm | **the identity holds WITH 9 unrecorded, so the second clause is wrong** | §2(c) — the 9 are superseded arms, all re-asked, 0 lost |
| F6 | no `http_error` record carries the `continue_final_message` body | **PASS (vacuous)** | 0 http errors |
| F7 | no infect row prints `you would be at`; no `feeds their chain` on an infect source | **UNTESTED** | 0 infect/poison anywhere in 2,919 prompts |
| F8 | no cover clause claims a body for a cast whose name already stands, token or not; bracket and cover agree | **PASS** | 0 of 41 |
| F9 | no corpus stopped with a PILOT-STALL marker while a live seat is answering | **PASS (no false fire)** | §2(n) |

### Carried docket
* **{X}-announcement payment loss.** `dropped after its X was announced` prints **0** times, and
  **0** records carry a `chosen_text` beginning `Decline - do not cast this after all`. The
  identity O23 discharged in wave 74 (5 = 5, 53 mana) holds trivially: the shape did not occur.
  **UNTESTED this wave, not a regression.**
* **BS F2 (Tribute's second edict).** 4 `FORCED SACRIFICE` renders, no two-edict turn traced.
  **UNTESTED, fifth wave.**
* **BS F3 / the land latch.** `declined_face_latches` **0** on all 42 gameends. **UNTESTED,
  fifth wave** — and P18 has now removed the second face window entirely, so the latch this
  docket item watches may no longer be reachable; the next wave should decide whether to retire it.
* **BS F5 (blockers ceiling).** `dropped_assignments` 0 on all 27 blockers records; no
  `ceiling re-ask` line in any stderr. **UNTESTED.**
* **BS F6 (distinct-index re-ask).** no `distinct index` line in any stderr. **UNTESTED.**
* **CF F1 (pending deadlock).** 0 hung, 0 crashed, 0 timeouts, 21 natural finishes, and **0**
  `[combatentry]` lines. Proxy only, as before. **UNTESTED directly.**
* **CF F2 (decode-garbage retry on the land arm).** all 15 retries are `reasoning_forced_close`;
  1 of 70 async drops is on the land arm and it was re-asked. **UNTESTED, third wave.**
* **CF F5 (non-lifelink converter).** `Cliffhaven Vampire` in 0 prompts; every converter in the
  pool is proportional (Sanguine Bond). **UNTESTED.**
* **CF F8 (haste granter).** no haste-GRANTING permanent on any board (the 816 `haste` mentions
  are printed keyword tags and card text). **UNTESTED.**
* **CF F9 (thousands separator in a carried plan).** 0 plans write one. **UNTESTED.**
* **Reasoning-body halves of N14a / N14d / O20.** **UNTESTED** — I did not read reasoning bodies.

### Other counters, for the record
`sibling_window_asks_skipped` **0** for the fourth wave. `chain_windows_collapsed` **0** —
`chainAutoPassApplies` requires `!anyLegalAction`, and 0 windows had an empty acting-row set, so
the collapse stays unreachable, which is the safe direction. `stop_reached_windows_skipped` 19
(wave 74: 30) · `mana_only_windows_skipped` 232 (136) · `own_turn_windows_skipped` 650 (599) ·
`reserve_decline_windows_noted` 21 / `_skipped` 3 · `identical_option_asks_resolved` 11 (2) ·
`repeat_annotated_takes` 2 · `menu_pass_no_progress` 0 · `put_gloss_stripped` 0 ·
`engine_reveal_floor_picks` 0 · `ask_replays_refused` 0. Blocker forecast: rows 132, multi 76,
gang **18** (wave 74: 6), collapsed 28. Combat entry: **102 `declared attack` traces = 102
attackers records**; 438 suppressions, all classified (`no-legal-attacker` 382,
`unresolved-stack` 56). Nine own turns show an able-attacker count > 0 on some rendered board and
no attackers record (deck123 ×4, deck130 ×5, all n=1-2) — a weak signal, since the count is read
at the window that printed it and the body can tap or die before combat; 9 of ~111 such turns.

---

## 4. Engine / render / narration defects

### HIGH-1 — the `[hold check: …]` bracket measures against a window the model never saw
`holdReopenNote` (`src/AIPlayerGPT.cpp` ~:28729) updates `mLastMenuRows[seam]` at every window
whose prompt is BUILT. This corpus built 3,402 windows the model never received (1,601 suppressed
by the hold latch, 1,801 answered from the two replay caches). The bracket then tells the model
*"every row above was also on the menu **at the last window at this seam**"* about a menu the
model has no memory of. Repro, verbatim, `126v146` deck126 priority seam:
seq 28 (t14) offers eleven `Deal 1 damage with Staff of Nin targeting …` rows; seq 33 (t16)
offers three Sorin loyalty rows and nothing else, and prints
`[hold check: every row above was also on the menu at the last window at this seam (1 window in a
row now) - a hold taken here holds until one of them appears, disappears, or names a different
card, cost or target; a row that changes only in its annotations is the same row]`.
**214 of 2,000** record-adjacent windows show this shape (deck123 73, deck130 48, deck125 32,
deck146 23, deck126 22, deck152 11, deck162 5); the hold row was taken at 9 of them. Under the
trust doctrine the bracket is an instruction, and its subject is invisible to its reader.
Fix direction: either name the referent the model can check ("since the last window I asked you
about at this seam") and key the memory on windows that were ASKED, or state the run length in
asked windows. The 214 is an upper bound — my `holdActionKeyRow` is a Python re-implementation and
the engine's own reference set is not in the translog; the direction of the bound is that some of
the 214 are true-about-an-invisible-window rather than false, which does not make the sentence
readable.

### HIGH-2 — an unbounded per-body enumeration, replicated once per identical copy row, produced an 80,805-byte prompt
`125v123` deck125 **seq 394** (discard, t77): the prompt is **80,805 B**, 2.4× the largest prompt
in wave 74's whole corpus (34,315 B) and 6× this corpus's p50. 47 KB of it is five rows —
`26.`/`27.`/`28. Supreme Verdict (copy 1|2|3 of 3 in your hand)` and
`24.`/`25. Final Judgment (copy 1|2 of 2 in your hand)` — each **9,450-9,497 B**, each carrying
its own copy of `{right now: destroys 100 of their creatures (100 without a restriction against
attacking), 0 of yours - THEIRS: Lord of Lineage (5/5) [flying, haste, shroud, doesn't untap
during its controller's untap step], Vampire #1 (4/4) (printed 2/2) [flying, doesn't untap during
its controller's untap step], Vampire #2 …}` enumerating all 100 bodies. Two defects compose:
the enumeration has no cap, and O18's identical-row collapse does not reach copy rows of one card
in a discard menu (they differ only by `(copy N of M in your hand)`). 74 prompts carry a
`- THEIRS:` enumeration (deck123 41, deck125 33); 562 prompts exceed 20 KB. Fix direction: cap the
enumeration (a count plus the distinct shapes, the way the board header already summarises), and
fold identical copy rows at the discard seam.

### MED-1 — `main_phase_windows_skipped` is 36% false and the overwrite signature is unchanged
See §2(g). 34 of 94 counted `(turn, phase)` pairs DID put a `Casting decision`; the
(turn,4)/(turn,10) pairing appears on 46 of the 47 counted turns. CI's two pure predicates
(`w75MainPhaseSkipArms` / `w75MainPhaseSkipCancels`) are pinned; the member plumbing that calls
them is not, and the rate is statistically unchanged from wave 74 (36% vs 39%). The meter must not
be cited until this is closed. Repro: `146v125` deck146 stderr line for `(turn 3, phase 4)` against
that seat's translog seq 5.

### MED-2 — nine forced closes were armed, superseded, and paid for a second round trip
See §2(c). Named arms: `125v123` casting ×2, `125v130` casting ×2, `162v123` land-drop ×1,
`152v123` casting ×1, `162v130` casting ×3. No decision was lost (0 fallbacks, 0
`phase2_answer_missing`, every neighbouring async drop `[outcome: re-asked]`), but each cost a
~20 KB-reasoning decode whose answer was discarded because the question had moved — ≈12 minutes of
wall clock across the corpus. The accounting is now honest enough to see it; the arm is what needs
bounding (do not arm a close on a window whose seam already has one outstanding at a different arm
unless the first is still live).

### MED-3 — the `plan_names_uncastable_zone_card` counter has no self-recursion exemption
See §2(m). Hammer of Bogardan is 12 of the 47 triggers I could re-derive from 68, and its plan
(`Return Hammer of Bogardan to hand to cast it in main phase 1`) is CORRECT — the card returns
itself from the graveyard. A card whose own text moves it out of the graveyard/exile is not a
stranded plan. CL flagged flashback/retrace/cast-from-exile; self-recursion is the fourth case and
it is the single largest contributor.

### MED-4 — the attackers seam is the one reply seam that got worse
`plan_line_missing` 16 of 102 = **15.7%** at the attackers seam (wave 74: 10.8%; ask seam this
wave: 3.9%). `off_protocol_bytes>0` 15 of 102 = 14.7%. Two of the corpus's three
`prose_outside_two_lines` records and both `plan_absent` ATTACK replies (`'\n\nATTACK: none'`,
`'\n\nATTACK: A1, A2'`) are here. The attackers prompt is the one seam whose answer label differs
from `CHOICE:`, and it is the seam that drifts.

### LOW-1 — the `prose_outside_two_lines` class counts the rescue as a deviation
`126v146` deck126 seq 22 is a `reasoning_forced_close` record whose 823 off-protocol bytes are the
phase-2 prefill's own trace echoed back before a correctly-formed `PLAN:` / `ATTACK:` pair. The
answer was consumed correctly (`ATTACK: Vampire #1` → `chosen_text` `Vampire`). Excluding
forced-close records would make the class mean what it says: 2 of 3, not 3.

### LOW-2 — six hold rows exceed CI's stated 650 B ceiling
723-724 B at `123v125` seq 1276, `125v126` seqs 111/112, `126v125` seq 228, `130v146` seqs 49/50,
against a median of 593. Small, and named only because CI's prediction was categorical.

---

## 5. The owner's lategame specimen (standing rule, alert-only)

`wave75/lategame-specimen.txt` — `125v123` deck123 seat, **seq 1252, turn 82**, rendered header
`Casting decision (Main phase 1, YOUR turn)`, life 1 vs 97, 7 options, prompt 10,669 B,
latency 43.5 s, reasoning 7,960 chars, full `prompt` field verbatim under a 3-line header.
Chosen for the deepest turn in the corpus's longest game (83 turns), a board of 94 Vampire tokens
against 1 life, and a game log that has been through the trim-and-batch compressor
(`[x94 - this exact line 94 times in this batch]`).
