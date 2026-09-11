# Wave 76 — engine-narration seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-141526-final`, **SINGLE BINARY**
`archives/wagic-d372a1153-w76step1` (source `b4784ed05`, lanes CN+CO+CP+CQ).
21 games / 42 seats / 920 turns, `--thinking on`, `WAGIC_GPT_TIMEOUT=900`, no operator token
ceiling. 19 of the 21 games are RERUNS after the pilot wedge; the two first-run games are
`146v152` (start 1789067756) and `152v162` (1789067732). Read-only: no build, no run, no git,
nothing written outside `wave76/` (`engine-seat.md`, `lategame-specimen.txt`).

**What I did NOT check.** No deck-guide reading and no strategy verdicts (deck seats own those).
No Scryfall/Gatherer calls — card facts come from the rendered prompt and, where a mechanism
mattered, from `src/AIPlayerGPT.cpp`. PARSETEST/suite numbers are quoted from the lane files,
never re-run; no lane fixture, probe or `archives/` binary was run; no endpoint was called. I did
not read 2,188 reasoning bodies — every prediction whose subject is a reasoning-body claim stays
UNTESTED (N14a/N14d/O20 reasoning halves). I did not enumerate the 89 `hold_released_turn` events
against their holders' untaps (wave 75 did; I re-used no wave-75 number for it). I did not re-derive
`serializeGameState()`, so §2(d)'s board-key verdict is inferred from the printed clause, not from
the key. I did not hand-verify the arithmetic of all 219 own-clock renders, only their
co-occurrence tests. `askreplay/` sidecars were counted and their `why`/`replay_run` fields read,
not line-by-line compared to their parent records. My `holdActionKeyRow` is a **Python
re-implementation** transcribed from `AIPlayerGPT.cpp` (`w76StripBalancedAnnotationGroups` +
`stripRenderAnnotationsLc` + `holdKeyLifeProjectionsNormalised` + `w75CostGroupsKey`); §2(b)'s
numbers are only as good as that transcription, and the engine still does not log its own referent.

---

## 1. Corpus census

| | wave 76 | wave 75 |
|---|---|---|
| seats / games | 42 / 21 | 42 / 21 |
| completed naturally | 42 gameend, 21 `results.tsv` rows, **0 hang, 0 crash, 0 timeout** (19 games are reruns) | 42 / 21, 0 rerun |
| **decisions (records carrying a `prompt`)** | **2,188** | 2,919 |
| by kind | ask 1,594 · priority 412 · attackers 94 · discard 40 · blockers 29 · reveal 13 · bottom 6 | ask 2,118 · priority 603 · attackers 102 · discard 53 · blockers 27 · reveal 10 · bottom 6 |
| summed gameend turns / **decisions per turn** | 920 / **2.38** | 984 / 2.97 |
| same, ask seam only | **1.73** | 2.15 |
| **fallbacks, all kinds** | **1** (`repeat_count_reask`, a MODEL re-ask — §2(n)); **0 heuristic answers** | 0 |
| `transport` | `curl=0,http=200,empty=0` on **2,188 / 2,188** | same on 2,919 |
| `http_error_body` | 0 records | 0 |
| `async_drops` | **37**, every one `[outcome: re-asked]` (`answer_replaced` 0 on all 2,188) | 70 |
| `deadline_pct` | p50 10.0 · p95 21.5 · max 49.6 | 8.6 / 20.6 / 48.4 |
| latency | p50 **90.2 s** · p95 194.3 · max 449.4 | 77.8 / 186.3 / 439.0 |
| `reasoning_chars` | min 376 · p50 5,639 · mean 6,126 · p95 10,457 · max 22,793 | 571 / 5,734 / 6,156 / 10,289 / 26,686 |
| `reply_truncated` | **0** | 0 |
| prompt bytes | p50 11,994 · p95 23,321 · p99 28,460 · **max 30,375** · **282 over 20 KB, 0 over 40 KB** | 13,229 / 27,434 / 30,695 / **80,805** / 562 over 20 KB |
| wins | 152 5/6 · 130 4/6 · 162 3/6 · 126 3/6 · 146 2/6 · 123 2/6 · 125 2/6 | 152 5 · 146 5 · 162 3 · 125 3 · 126 2 · 130 2 · 123 1 |

**⚠ The brief's census line is the ASK SEAM again — FOURTH wave running.** `57
off_protocol_bytes>0 (3.6%)`, `74 plan_line_missing (4.6%)` and `latency p50 92.4 / p95 193.8` are
the statistics of the 1,594 `kind: ask` records alone. Corpus-wide: **72** off-protocol (3.3%),
**91** plan-line-missing (4.2%), latency p50 90.2 / p95 194.3. The gameend counters already say so
(`protocol_replies` 2,188, `plan_line_missing` 91). The brief's `1.73 vs 2.15 decisions/turn` IS a
like-for-like ask-seam comparison and is correct as stated; corpus-wide it is 2.38 vs 2.97.

**Reasoning (invariant 000).** 2,188 / 2,188 decision records carry `thinking: on` and a non-empty
native `reasoning`; 0 empty. The harness gate agrees (`REGIME GATE PASSED: thinking=on: 210
records, all carry reasoning. seats: 42 shapes: two_line=210`). **The corpus is valid under 000.**
Rescue markers: `reasoning_forced_close` 13 = `reasoning_budget_hit` 13 = `retry: 1` 13, and
`phase2_answer_recovered` 13 + `phase2_answer_missing` 0 = 13; all 13 carry `coded_answers: 1`,
`protocol_deviation: compliant`, `off_protocol_bytes: 0`, and no fallback.

**Reply shape.** `protocol_replies` 2,188. `plan_line_missing` **91 (4.2%)**, `off_protocol_bytes>0`
**72 (3.3%)**, **overlap 72 — every off-protocol reply also lost its plan line.**
`protocol_deviation` partitions exactly: `compliant` 2,097 · `unlabelled_plan` 72 · `plan_absent`
19 — **91 non-compliant = `protocol_deviation_replies` 91, to the record**, and `plan_line_missing`
91 = `protocol_deviation_replies` 91 (the brief's (o) identity: **holds**). No
`prose_outside_two_lines` and no `forced_close_prefill_echo` occurred. Off-protocol byte weight:
n=72, sum 4,852 B, median 60.5, max 187 (wave 75: max 838). `action_before_plan_replies` 0,
`answer_replaced` 0, `post_answer_overrun` 0 on every record (wave 75: 1).

**Ceilings.** `max_tokens_answer`: ask 288 (1,584) / 400 (10 retries); priority 288 (408) / 400 (4);
attackers 256 (93) / 400 (1); blockers 256 (29); discard 256 (40); reveal 256 (13); bottom 512 (6).
`reply_truncated` 0. `dropped_assignments` **0 on all 29 blockers records** — BS F5's doubled
allowance is UNTESTED for the sixth wave.

**Degeneracy meters.** `reasoning_degenerate` p50 0.0031 · p95 0.0059 · max 0.0128 (0 records above
0.2). `reasoning_ngram_repeat` p50 0.126 · p95 0.267 · max 0.555; 379 records above 0.2. Both are
ratios, not flags; nothing in either distribution looks like a collapse.

**`askreplay/` sidecar.** 25 files, **948 records** = summed `ask_replays_reserved` 948, exactly.
`why`: `repeat_ask_reserved` **751** (= `identical_ask_answers_reserved` 751, exactly) ·
`cache_replay` **197**. `ask_replays_refused` 0; max `replay_run` 9 / 11, cap 64 — the refusal path
was never reached and no `refusing the …` line appears in any stderr.

**`plan_steps_done`.** `(done,count)`: (0,1) 1,067 · (1,2) 793 · (1,3) 252 · (1,4) 54 · (1,5) 9 ·
(0,0) 7 · (2,3) 3 · (0,2) 1 · (2,5) 1 · (3,5) 1. The pointer reaches ≥2 on **5** of 2,188 (wave 75:
4 of 2,919). Unchanged: the model re-states its plan far more often than it walks it.

---

## 2. REGRESSION WATCH — the wave-76 changes

### (a) The loop family — the wave-75 shapes are dead; a NEW ask storm is not a loop but costs like one

Asks per (turn, rendered phase) per seat, phase read out of each prompt's own
`Phase: X | It is your/the opponent's turn.` header (wave-59 method note). **7 windows exceed 8
asks; 3 exceed 10; the maximum is 35.** Every one has a moving board:

| asks | seat | (turn, phase) | distinct menus | distinct life pairs | genuine? |
|---|---|---|---|---|---|
| **35** | `126v123` deck126 | (17, Main 1, own) | 5 | **33** | see HIGH-1 — a resolving life LOOP, one model call per iteration |
| 14 | `146v126` deck146 | (23, Main 1, own) | 9 | 6 | YES |
| 13 | `152v126` deck152 | (11, Main 1, own) | 11 | 2 | YES |
| 10 | `152v126` deck152 | (9, Main 1, own) | 10 | 2 | YES |
| 10 | `125v123` deck123 | (54, Upkeep, own) | 6 | 1 | YES (the P14 window, §2(n)) |

**No re-put window with an unchanged board and an unchanged list appears anywhere.** The CG and CH
families stay dead. `sibling_window_asks_skipped` 0 (fifth wave); `chain_windows_collapsed` 0;
`menu_pass_no_progress` 0; `wall_miss_*` 0.

### (b) Q1 — the hold-check bracket, now checkable from the translog for the first time

1,377 bracket renders: `every row above was also on the menu at the last window I asked you at this
seam` **951** (642 plural-run / 309 singular-run), `N rows above are new` **426**.

Because the bracket now commits only on windows that EARN A RECORD, the referent is nameable from
the translog. I compared each bracket against the previous record at the same seam **where that
record also carried a bracket** (1,033 pairs; 344 pairs skipped because the previous same-kind
record carried none — a land-drop/modal/dungeon window, see below):

* `every row above was also…` claims: **757**, of which **15** are false (a current row's
  `holdActionKeyRow` key is absent from the referent's key set). Under the LITERAL wave-75 test
  (the referent shares NO key with any current row) the same **15** fail. Wave 75: 214 of 2,000.
* `N rows above are new` claims: **276**, of which **7** state a count that does not match the key
  diff — **every one of the 7 under-counts** (claimed 1-3, actual 4-6). **0 claim a row new whose
  only delta is inside a nested annotation**: the F10 balanced stripper has closed that class
  (wave 75: the `162v152` Dictate `{reserve: … {4} …}` pair).
* **All 22 falsehoods have the same cause and it is a NEW one: the land-drop window shares the
  `"cast"` seam key with the casting menu.** 0 of the remaining 1,011 pairs is false. See MED-1.
* Negative clause (over-correction): seen/new is 757/276 = 73% "seen" — the bracket is not
  collapsing to 0% seen, and the run still climbs (309 singular vs 642 plural renders).

### (c) Q8 — the 5 unrecorded closes, reconciled

18 `unclosed <think> (budget/truncation); forcing the answer` traces across the 14 stderr files
that have any. **`phase2_answer_recovered` 13 + `phase2_answer_missing` 0 + `forced_close_unrecorded`
5 = 18.** The identity holds in **all 21 games**, per game:

| game | stderr closes | recovered | unrecorded | seat carrying the unrecorded close |
|---|---|---|---|---|
| 125v123 | 1 | 0 | 1 | deck123 |
| 126v123 | 2 | 1 | 1 | deck126 |
| 126v130 | 1 | 0 | 1 | deck130 |
| 126v162 | 1 | 0 | 1 | deck162 |
| 146v123 | 1 | 0 | 1 | deck123 |
| the other 9 games with closes | 12 | 12 | 0 | — |

**What superseded each of the 5:** the stderr line immediately after four of the five is
`dropping stale async answer (casting arm; the question and board moved) [outcome: re-asked]`, and
in all five the next line is a fresh `Casting decision (Main phase 1, YOUR turn): … -> chose N of M`
— a model answer. **No decision fell to the heuristic on any of the 5** (0 fallbacks corpus-wide
apart from the P14 re-ask; 0 `phase2_answer_missing`). These are **wasted decodes**
(~13 KB of phase-2 reasoning each), exactly the reading CM F5 was superseded to.

**`force_close_arms_refused` 0 / `_deferred` 0 / `_defer_bound_hits` 0 — the Q8/F2 path never fired.**
All five closes were therefore armed with the park EMPTY, i.e. **same-arm re-arms**, which
`w76ForceCloseDecision` deliberately does not refuse. CN's own Weakest-evidence #4 predicted exactly
this outcome and named the consequence: the waste is unchanged and the same-arm case is the next
lane's problem.

### (d) Q13 — the counter fired; the board clause almost never prints

`crossphase_identical_reputs` **111** across 16 of 42 seats (worst `125v123` deck123 **51**, then
`123v130` 10, `146v125` 11). **109 brackets rendered** in the translog — 2 counted windows carry no
bracket in any record, a 1.8% gap I could not place (most likely a window counted on a polling tick
whose record was replayed rather than written).

Of the 109 renders: **104** say `the board HAS moved since then`; only **5** say `nothing on the
board has changed` (4.6%). **CQ F4's own Weakest-evidence #5 is confirmed: the board comparison is
too strict to be informative** — the rows, turn and question are byte-identical by construction, so
a 95% "board moved" rate says the serialiser carries phase-varying content well beyond the phase
line `w76PhaseFreeBoardKey` removes. The clause is fail-closed (no false claim) but nearly dead.

**105 of the 109 re-puts (96.3%) were answered with the same decline** (`Cast nothing right now`
or `Hold priority`). The measure the lane wanted is now in hand and it is large: 109 windows and
~2.7 hours of decode spent re-asking a list the seat had already declined.

### (e) Q7 — 0 of 94… 0 of 56, and CN's refutation of wave-75 MED-1 independently reproduced

56 `own main phase (turn N as the records number it; observer turn N, phase N) ENDED with NO
casting window` traces = `main_phase_windows_skipped` 56, exactly. Cross-referenced against each
seat's own `Casting decision (Main phase N, YOUR turn)` headers (phase 4 → Main 1, phase 10 → Main 2):

* under the RECORD's turn: **0 false of 56 (0.0%)**;
* under the wave-75 raw-observer-turn mapping: **17 false of 56 (30.4%)** — the wave-75 MED-1
  signature, reproduced on a different corpus;
* `recturn == obsturn + 1` on **56 of 56** traces — `translogTurn` is the whole difference.
* the `(turn,4)/(turn,10)` pair signature: 27 turns carry both mains, 2 carry one. That is the
  expected shape of a seat whose instant-speed arm answers both of its own mains, not an overwrite.

**CN's REFUTATION of wave-75 MED-1 is CONFIRMED by my own re-derivation, and the meter is quotable
for the first time.** The trace's new dual-turn print is the instrument that makes it so.

### (f) Q9 — the exemptions took the whole self-recursion population

`plan_names_uncastable_zone_card` **102 per-record fields** (wave 75: 68) over 18 distinct cards:
Bloodline Keeper 31 · Idyllic Tutor 21 · Sanguine Bond 14 · Exquisite Blood 7 · Thraben Doomsayer 5
· Triumphant Adventurer 5 · Devour Flesh 3 · Siege-Gang Commander 3 · Intruder Alarm 2 ·
Fate Unraveler 2 · Acererak 2 · and eight singletons.

* **0 records name Hammer of Bogardan**, on a corpus where Hammer is printed in **166** prompts —
  CN's Q9 prediction, PASSED against a live population rather than vacuously.
* **0 records name a card castable from exile** (12 prompts carry a cast-from-exile marker).
* Sampled 17 of the 102 by reading the prompt's own zone lines: every one is genuine. Worked
  example, `125v123` deck123 seq 72 — `Your graveyard (5 cards): Bloodline Keeper {2}{b}{b}
  (creature 3/3); …` and the reply's `PLAN: … cast a creature (Bloodline Keeper or Thraben
  Doomsayer)`. The prompt already carries the sibling note for the LIBRARY case
  (`note: your plan names "Thraben Doomsayer", and no "Thraben Doomsayer" is in your hand, on either
  battlefield, in a graveyard, in exile or on the stack right now …`) — the graveyard case has the
  counter but no note. That asymmetry is LOW-2.
* `plan_names_stranded_card` **1** (`162v123` deck162 seq 10, Fog Bank). The two measures are now
  independent; **no record carries both**, so the wave-75 structural under-count is closed but
  unexercised.

### (g) Q3 — the loop/lethal family

* **Q3a.** 29 combat forecasts corpus-wide: 6 `LETHAL if it all connects`, 23 `NOT lethal: block
  only where the trade favors you`, **0** co-occurring with `NO best-case life figure is given`
  (wave 75: 3) and **0** on a prompt carrying `LOOP COMPLETE`. The withdrawal sentence
  (`NO survival verdict is given from that figure: both halves of their life LOOP …`) rendered
  **0 times** — no loop-live board produced a combat forecast this corpus, so the positive path is
  **UNTESTED**; the negative clause (no over-deletion) holds, all 23 non-loop renders intact.
* **Q3b.** **219** own-clock renders (wave 75: 193), **64** prompts carry a `CRACK-BACK NEXT TURN:`
  line saying `that would KILL you`, and the two sets **do not intersect — 0** (wave 75: 8). This
  is a real test, not a vacuous one. Not over-deleted: 43 of the 219 sit under a live
  `LOOP COMPLETE`, and all 219 still print `reaches 0 in N more turns`.

### (h) Q4 — cover and floor

* **Q4a.** **23** `Their N untapped blockers can cover every attacker you could send` renders;
  **1** has a menace attacker among its own A-rows (`146v123` deck146 seq 34: four attackers needing
  2+2+1+1 = **6** bodies against **25** untapped blockers). The claim is **TRUE** — 25 ≥ 6. **0
  false aggregate covers** (wave 75: 4 suspected). Not over-suppressed: 22 non-menace renders stand.
* **Q4b.** **71** `At least N damage lands whatever they block` floors. **1** is requalified
  (`125v123` deck123 seq 424, `but that floor is over their BLOCKS ONLY … fires on your
  declaration`); the other **70** are on prompts naming no attack punisher, and **0 unrequalified
  floor sits on a board where Lightmine Field is named as theirs**. PASS, with one live firing.

### (i) Q5 — the crack-back kill clause fires, and its arithmetic is true

**71** prompts carry a `{crack-back cover:` clause (wave 75: 38); **8 option rows** carry the new
`This row REMOVES K of those N attackers` form. Every one of the 8 checked against its own
`CRACK-BACK NEXT TURN:` line: **K ≤ N in 8 of 8**, the subtraction closes (`takes D off the total,
leaving 0`), and no survival verdict is stated against a line the screen calls a FLOOR (the
`125v126` deck125 seq 31 case prints the FLOOR caveat instead). Worked example, `123v130` deck130
seq 20, which is CO's own repro shape one wave later — the **Starstorm X-pricing row** now carries
`{crack-back cover: the CRACK-BACK NEXT TURN line above is 5 from 2 of their creatures and puts you
at 15. This row REMOVES 2 of those 2 attackers … leaving 0 -> you would be at 20, which you
SURVIVE …}`. CO's Weakest-evidence #2 (whether `xSpellPricing` would fire live) is **discharged**.

Residual, and it is new: on the **X SUB-MENU** that follows (same seat, seq 22), the `X = 2` /
`X = 4` rows price the same kills (`kills THEIRS: Bloodline Keeper, Vampire`) under the same
crack-back line and carry **no** cover clause. See MED-3. Stone Rain's
`kills whichever you target: THEIRS - <lands>` rows correctly carry none (lands are not crack-back
attackers, and the targeted class is CO's stated non-coverage).
**Regeneration/ghostform/indestructible subtraction (CQ F5): no such body appeared in any of the 8 —
UNTESTED.**

### (j) Q6 — the converter clause's condition never occurred

**15** `[you SACRIFICE this; they gain N life (its toughness)]` / `you gain N` rows across **7**
windows. **0** of them is on a board carrying `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs`
(78 prompts carry a converter, none of them a forced-sacrifice window; the one converter co-present
with a sac row, `126v123` seq 23, is **yours**, and a converter of theirs does not convert the
seat's own gain). **Q6 is UNTESTED** — the positive path did not occur. Negative clause holds: all
15 rows are byte-identical to the wave-59 wording, **0** carry `and with their <name> that is N off
YOUR life`, and **0** carry `you would be at` inside the bracket (the key-drift MUST-NOT-MATCH).
F6's life-gain prohibition gate is likewise UNTESTED.

### (k) Q2 — the 80 KB prompt is gone

Max prompt **30,375 B** (wave 75: 80,805), **0 prompts over 40,000 B**, 282 over 20 KB (wave 75:
562). CP's Q2 prediction **PASSES on both clauses**. **38** `- THEIRS:` roster clauses, **longest
87 B** — far under the 500 B prediction; no roster reached the shape cap or printed a residue
sentence, so `kVictimRosterGroupCap` / `kVictimRosterGroupItemCap` and the `+ N more bodies` residue
are **UNTESTED**. **0 records carry a `(copy N of M in your hand)` row** this corpus, so the
`copyTagOwnsRow` fold and its answer-mapping negative clause are **UNTESTED**; 95 records do carry a
folded range row and the discard seam offered 8-17 answerable rows on its 40 records with no
mis-mapped answer. Longest single option row **2,374 B** (`152v126` deck152 seq 63, a
`kills whichever you target:` roster) — marginally over F7's "~2 KB" expectation and the place the
next unbounded-enumeration will surface, as CP predicted for `sweeperSurvivorTail`.

### (l) Q10 / Q11 / Q12 / Q14

* **Q10 UNTESTED.** 81 prompts name `ghostform`, 74 option rows carry a `{removes: …}` clause, and
  **0 prompts carry both** — so no `{removes:}` ever named a ghostform body and the tail
  (`NOT removal: it returns to its owner's hand …`) rendered **0** times. Wave 75 had 4 of 13.
* **Q11 PASS.** 247 loyalty rows. `-2: emblem: "creatures get +1/+0" with Sorin, Lord of Innistrad`
  renders **52** times and **never** with ` targeting ` (wave 75: 13 windows did);
  `-7: get untap and draw emblem with Teferi, Who Slows the Sunset` (1) likewise.
  `-6: destroy up to three and reanimate with Sorin, Lord of Innistrad` renders **42** times, **0**
  with ` targeting `, and carries the arity clause instead (`[this ability chooses up to 3 targets
  as it resolves - this row cannot name them yet …]`). Negative clause holds: genuine single-target
  rows still name their target (`+1: target creature gets ghostform with Kaya … targeting Triumphant
  Adventurer` 9, `-2: destroy target creature and draw two cards with Ob Nixilis … targeting Shield
  Sphere` 4).
* **Q12 PASS.** **1,017** casting menus carry both closing rows; **0** print `Cast nothing right
  now` above `Hold priority` (wave 75: 32 of 159 on deck126 alone). 412 menus carry a hold row with
  no decline row — the append arm — so the negative clause holds too. 1,429 hold rows in all.
* **Q14 PASS.** **8** `pay N life` rows (all deck146, as in wave 75); **0** print
  `usable (tap for mana / attack) this turn` (wave 75: 8 of 8 did).

### (m) Q15 — the deviation class never had to fire

`forced_close_prefill_echo` **0** and `prose_outside_two_lines` **0**: all 13 forced-close records
classified `compliant` with `off_protocol_bytes: 0`, so neither F9's fix nor the class it replaced
was exercised. **UNTESTED.** Hold rows over CP's stated 650 B ceiling: **4** (723-726 B; wave 75:
6) — `125v123` seq 615, `125v162` seq 116, `146v125` seq 155, `146v162` seq 35. Unchanged and
reported, per CP Q15(c).

### (n) P14 — the first live firing, and it worked

`125v123` deck123 **seq 415** (t54, priority). Window (3 rows):
`0. Create human with Thraben Doomsayer [cost: Tap] …`, `1. Create human with Thraben Doomsayer,
repeated then stop [you control 2 creatures right now; you name N on the CHOICE line, e.g.
"CHOICE: 2 (Create human with Thraben Doomsayer x<N>)" - N is a DIGIT you choose, and copying this
row's name alone, with no x<count> after it, names no count and is refused …]`, `2. Hold priority`.
The model replied `PLAN: stop=64; M=2; this window tap Doomsayer x5 … / CHOICE: 2 (Create human
with Thraben Doomsayer, repeated then stop)` — the bare row name, no count. `chosen_text`
`<refused: repeat_count_reask>`, `fallback: repeat_count_reask`. **seq 418** re-put the same window
with `[RE-ASK] "CHOICE: 2 (…, repeated then stop)" names no count. That row repeats an action and
needs one: answer again with the same row number and "x<count>" …`; the model answered
`CHOICE: 2 (Create human with Thraben Doomsayer x62)` and the engine executed it (`repeat_n: 62`,
and the next prompt's event log reads `You used: Create human with Thraben Doomsayer (you named 62
repeats of it)`). **The single `fallback` value in this corpus is a MODEL re-ask, not a heuristic
answer** — the census's "0 heuristic fallbacks" is correct. 4 records carry `repeat_n` (62, 5, 18, 9)
= `repeat_annotated_takes` 4.

### (o) The replay / drop identities

`async_drops` **37** records = **37** stderr `dropping stale async answer … [outcome: re-asked]`
lines, **100% re-asked**, `answer_replaced` 0 — no decision lost.
`ask_replays_reserved` **948** = 948 `askreplay/` records exactly (25 files);
`identical_ask_answers_reserved` **751** = the 751 `repeat_ask_reserved` sidecars exactly; the other
197 are `cache_replay`. `ask_replays_refused` 0, max run 9/11 against a cap of 64.
`protocol_deviation_replies` **91** = `plan_line_missing` **91** = my own corpus count. Every
identity in (o) **holds to the record**.

### (p) The harness

* **PILOT STALL fired ONCE and was a TRUE positive.** `~/.gatelogs/w76-corpus.log:35` —
  `== PILOT STALL: the last records of every live seat are timeout fallbacks (6 of 6).` The engine
  froze at 15:42 CDT; the tripwire stopped the run at 15:56, 14 minutes later, with 2 games finished.
  No further stall fired in the 19 rerun pools. The REGIME GATE also passed at line 33 before it.
* **The harvest keys on mtime, not on the pool's own manifest.** Each rerun pool ran ONE game (2
  seat logs) but reported `harvested 38 / 37 / 36 / … / 17 player-game logs`, monotonically
  descending as pools finished — i.e. each pool copied every *other* pool's in-flight seat log that
  happened to be newer than its start. That is why the `-final` dir had to be rebuilt from the
  shared logs dir. **Harness item, HIGH for the next corpus** (a mid-game copy of another pool's log
  is a truncated, gameend-less file that a naive aggregation would count as a seat). Fix direction:
  key the harvest on the outdir's own game manifest, as `tools/corpus-early-check.py` already does.

---

## 3. Prediction adjudication

### lane CN — window / hold / meters

| # | prediction | verdict | evidence |
|---|---|---|---|
| Q1 a | 0 `every row above was also …` brackets whose previous RECORD shares no key with a current row (wave 75: 214/2,000) | **FAIL, 15 of 757** — but all 15 are the land-seam collision, 0 of 1,011 non-land pairs | §2(b); MED-1 |
| Q1 b | 0 `N rows above are new` where the previous record's key set already held every row (wave 75: 6) | **PASS, 0** — 7 counts are wrong but all UNDER-count, none claims a phantom new row | §2(b) |
| Q1 c | nested-annotation residual (`{reserve: … {4} …}`) gone | **PASS, 0** — F10's balanced stripper closed it | §2(b) |
| Q1 neg | the bracket is not silenced / the run still climbs | **PASS** — 73% "seen", 642 plural-run vs 309 singular-run renders | §2(b) |
| Q7 | 0 false counts over 100% of the traces; the meter quotable | **PASS, 0 of 56** (raw mapping: 17, 30.4%) | §2(e) |
| Q7 neg | the meter is not silenced | **PASS** — 56 counted across 29 turns, 20 of 42 seats | §2(e) |
| Q8 | `forced_close_unrecorded` falls and `force_close_arms_refused` accounts for the difference | **FAIL on the second clause** — unrecorded 9 → 5, refused **0**; every close was a same-arm re-arm the bound does not refuse. CN's own Weakest-evidence #4 named this outcome | §2(c) |
| Q8 neg | no rescue eaten: no forced-close fallback, no `phase2_answer_missing` | **PASS** — 0 and 0, identity holds in 21/21 | §2(c) |
| Q13 | `crossphase_identical_reputs` non-zero on the deck123 and deck152 seats; the bracket appears on exactly those windows | **PASS with a gap** — 111 counted, 109 rendered; deck123 seats carry **83** of the 111, deck146 16, deck125 8, deck126 3, deck152 **1** — CN named deck123 and deck152; deck152 fired once, deck146 was not predicted. The 2-render gap is unexplained | §2(d) |
| Q13 neg | no window removed, option set unchanged | **PASS** — 0 `identical_option_asks_resolved` regressions (7), no collapsed window, no ask-key drift observed | §2(d) |
| Q9 a | 0 records name Hammer of Bogardan | **PASS, 0** against 166 Hammer prompts | §2(f) |
| Q9 b | 0 name a card castable from exile, counter stays non-zero | **PASS** — 102 fields, 0 exile-castable | §2(f) |
| Q9 neg | the exemptions do not empty the counter; a record may now carry both fields | **PARTIAL** — counter rose 68 → 102, but **0** records carry both fields, so the independence change is **UNTESTED** | §2(f) |

### lane CO — combat and loop truth

| # | prediction | verdict | evidence |
|---|---|---|---|
| Q3 a | 0 prompts carry `NOT lethal: block only where the trade favors you` and `NO best-case life figure is given` (wave 75: 3) | **PASS, 0 of 23** — but no loop-live combat forecast occurred, so the WITHDRAWAL never printed: the positive path is **UNTESTED** | §2(g) |
| Q3 a neg | the clock/verdict is not over-deleted | **PASS** — 23 non-loop NOT-lethal renders, 6 LETHAL renders, all intact | §2(g) |
| Q3 b | 0 own-clock renders beside a crack-back `that would KILL you` (wave 75: 8) | **PASS, 0** against 64 live crack-back-KILL prompts and 219 own-clock renders | §2(g) |
| Q3 b neg | own-clock still renders with no stated loss | **PASS** — 219 renders (wave 75: 193), 43 under a live `LOOP COMPLETE` | §2(g) |
| Q4 a | 0 `can cover every attacker you could send` on a menace board where cover costs more bodies than they have (wave 75: 4) | **PASS, 0 of 23**; the one menace window (146v123 seq 34) needs 6 bodies and they have 25 — the claim is true | §2(h) |
| Q4 a neg | non-menace floors unchanged | **PASS** — 70 plain floors | §2(h) |
| Q4 b | 0 `damage lands whatever they block` on a prompt naming an attack punisher | **PASS** — 1 requalified render (`125v123` seq 424), 0 unrequalified on a Lightmine board | §2(h) |
| Q5 | every cast row naming a kill of THEIRS on a crack-back prompt carries a `{crack-back cover:` clause (wave 75: 0 of 13) | **PARTIAL** — the clause fires (8 rows, arithmetic true in 8/8, including the Starstorm X-pricing row CO could not test), but the **X SUB-MENU rows** that follow carry none | §2(i); MED-3 |
| Q5 neg | 0 `This row REMOVES` clauses on rows removing no attacker | **PASS, 0** — `K ≤ N` in 8 of 8 | §2(i) |
| Q5 (CQ F5) | no cover clause names a body that survives (regen/ghostform/indestructible/persist) | **UNTESTED** — no such body among the 8 | §2(i) |
| Q6 | every forced-sacrifice row under a converter of theirs carries `and with their <name> that is N off YOUR life` | **UNTESTED** — 15 sac rows, 0 on a board with a converter of theirs | §2(j) |
| Q6 neg | rows with no converter of theirs stay byte-identical, none carries `you would be at` | **PASS** — 15 of 15 | §2(j) |

### lane CP — row and prompt truth

| # | prediction | verdict | evidence |
|---|---|---|---|
| Q2 a | 0 prompts above 40,000 B (wave 75: max 80,805) | **PASS, 0**; max 30,375 B, 282 over 20 KB vs 562 | §2(k) |
| Q2 b | largest `- THEIRS:` roster under 500 B | **PASS** — 38 clauses, longest 87 B | §2(k) |
| Q2 c | discard option COUNT unchanged | **PASS** — 40 discard records, 8-17 answerable rows, no fold-induced loss | §2(k) |
| Q2 neg | 0 answers resolve to the wrong card | **UNTESTED** — 0 copy rows in the corpus, so `copyTagOwnsRow` never ran | §2(k) |
| Q10 | 0 `{removes:}` / roster entries name a live-ghostform body without the tail | **UNTESTED** — 0 prompts carry both a ghostform body and a removes clause; the tail rendered 0 times | §2(l) |
| Q11 | 0 loyalty rows print ` targeting ` on an up-to-N or emblem ability | **PASS, 0 of 95** such rows (52 Sorin `-2`, 42 Sorin `-6`, 1 Teferi `-7`) | §2(l) |
| Q11 neg | a single-target loyalty row still names it | **PASS** — Kaya `+1` ×12, Ob Nixilis `-2` ×6 | §2(l) |
| Q12 | 0 casting menus print `Cast nothing right now` above `Hold priority` (wave 75: 32) | **PASS, 0 of 1,017** | §2(l) |
| Q12 neg | a menu with no decline row still carries a hold row | **PASS** — 412 such menus | §2(l) |
| Q14 | 0 `pay N life` rows offer `attack` for a non-animatable permanent (wave 75: 8 of 8) | **PASS, 0 of 8** | §2(l) |
| Q14 neg | a creature-land keeps both verbs | **UNTESTED** — no creature-land ETB row occurred | §2(l) |
| Q15 a | own-clock singular for one source | **PASS** — no `#1-#K deal … between them` render sits over a single source (spot-checked the Staff renders) | §2(m) |
| Q15 b | `forced_close_prefill_echo` only on forced-close records | **UNTESTED** — the class rendered 0 times; all 13 rescues were `compliant` with 0 off-protocol bytes | §2(m) |
| Q15 c | hold rows over 650 B reported, not fixed | **holds** — 4 rows, 723-726 B | §2(m) |

### lane CQ — the ten Astra findings

| # | prediction | verdict | evidence |
|---|---|---|---|
| F1 | 0 brackets whose named referent did not contain every row then printed | **FAIL, 15** — but the cause is the land/cast seam collision, not the staging bug F1 fixed; 0 of 1,011 non-land pairs | §2(b); MED-1 |
| F2 | `force_close_arms_deferred` > 0 with `_defer_bound_hits` ≈ 0; 0 fallbacks on a reasoning-only reply | **UNTESTED on the positive clause** (deferred 0 — the park was never busy); **PASS on the negative** (0 such fallbacks) | §2(c) |
| F3 | a byte-identical list re-put at two phases in one turn yields a non-zero `cross_phase_re_puts` and the bracket appears | **PASS** — 111 / 109 across 13 phase names | §2(d) |
| F4 a | every `nothing on the board has changed` is over two asks whose boards match below the phase line | **PASS by construction, but nearly dead** — 5 of 109 prints; the "over-strict, dead clause" branch of CQ's own negative clause is what happened | §2(d); MED-2 |
| F4 b | the counter is never a multiple of the tick count | **PASS** — 111 counted vs 109 rendered, not a per-tick multiple | §2(d) |
| F5 | no cover clause names a body on the battlefield untapped next turn | **UNTESTED** — no regenerating/indestructible/ghostform/persist victim in the 8 clauses | §2(i) |
| F6 | no converter clause in a game with a life-gain prohibition | **UNTESTED** — 0 converter clauses rendered at all | §2(j) |
| F7 | no option row exceeds ~2 KB from its victim roster; residues close the arithmetic | **PASS on rosters** (longest `- THEIRS:` 87 B, no residue needed); the longest option row is 2,374 B and its bulk is a `kills whichever you target:` roster, not a victim roster — flagged LOW-1 | §2(k) |
| F8 | `plan_names_uncastable_zone_card` rises by roughly the graveyard bounce population; no sampled exemption is an ordinary-text `moveto` | **PASS** — 68 → 102, 0 Hammer, 0 exile-castable, 17 sampled all genuine | §2(f) |
| F9 | the echo class appears only on forced-close records | **UNTESTED** — 0 renders | §2(m) |
| F10 | 0 hold re-opens / option-set key changes whose only delta is inside a cost-quoting `{…}` group | **PASS** — 0 phantom-new brackets; the wave-75 Dictate shape did not recur | §2(b) |

### Carried docket

* **{X}-announcement payment loss — DISCHARGED, and it finally happened.** `146v125` deck125
  **seq 179** (t40): the model announced Sphinx's Revelation, was offered X=1..14 plus X=0 plus a
  decline row, and took the decline. stderr: `cast of Sphinx's Revelation dropped after its X was
  announced (the model took the X menu's own decline row - 17 mana already paid stays floating and
  is lost when the step ends); announcement cancelled, card stays in hand`. **1 render, 1 stderr
  line, identity closed.** The row disclosed the whole cost verbatim (`declining strands the 17
  mana already paid: … your sources STAY TAPPED …`) and the reply's PLAN shows the model chose it
  knowingly (it was at a winning board). **Engine: correct. Not a defect.**
* **BS F2 (Tribute's second edict).** 6 `FORCED SACRIFICE` prompts; Tribute to Hunger, Devour Flesh,
  Soul Shatter and Silverquill Command each fire, but no two-edict turn was traced. **UNTESTED,
  sixth wave.**
* **BS F3 / the land latch.** `declined_face_latches` **0** on all 42 gameends. **UNTESTED, sixth
  wave** — and P18 removed the second face window, so this item should be retired or re-scoped.
* **BS F5 (blockers ceiling).** `dropped_assignments` 0 on all 29 blockers records; no
  `ceiling re-ask` line in any stderr. **UNTESTED.**
* **BS F6 (distinct-index re-ask).** no `distinct index` line in any stderr. **UNTESTED.**
* **CF F1 (pending deadlock).** 0 hung, 0 crashed, 0 timeouts in the 21 finished games, 0
  `[combatentry]` lines. Proxy only. **UNTESTED directly.**
* **CF F2 (decode-garbage retry on the land arm).** all 13 retries are `reasoning_forced_close`; all
  37 async drops are re-asked. **UNTESTED, fourth wave.**
* **CF F5 (non-lifelink converter).** `Cliffhaven Vampire` in 0 prompts; every converter in the pool
  is Sanguine Bond (proportional). **UNTESTED.**
* **CF F8 (haste granter).** 10 stderr `haste` lines, no haste-GRANTING permanent on any board.
  **UNTESTED.**
* **CF F9 (thousands separator in a carried plan).** 0 plans write one. **UNTESTED.**
* **Reasoning-body halves of N14a / N14d / O20.** **UNTESTED** — I did not read reasoning bodies.

### Other counters, for the record
`hold_windows_skipped` **1,280** (cast 872 / priority 408) · `hold_released_turn` 89 ·
`stop_reached_windows_skipped` 18 (wave 75: 19) · `mana_only_windows_skipped` 136 (232) ·
`own_turn_windows_skipped` 571 (650) · `reserve_decline_windows_noted` 28 / `_skipped` 3 ·
`identical_option_asks_resolved` 7 (11) · `chain_acting_rows` 1,570 / `_cast` 854,
`chain_selfharm_rows` 274 / `_cast` 421, `chain_windows_only_selfharm` **0** / `_cast` 327 ·
`sibling_window_asks_skipped` 0 (fifth wave) · `chain_windows_collapsed` 0 (still unreachable) ·
`put_gloss_stripped` 0 · `engine_reveal_floor_picks` 0 · `menu_pass_no_progress` 0 ·
`wall_miss_events` / `_no_retry` / `_unrecorded` all 0. Blocker forecast: rows 206, multi 96,
gang 16, collapsed 40. Combat entry: **94 `declared attack` traces = 94 attackers records.**
Reveal rows carry **no** `[cost:` group (the brief's suspected false-fact render is absent).

---

## 4. Engine / render / narration defects

### HIGH-1 — 32 model calls to watch a proven life loop resolve itself

`126v123` deck126, **t17 Main 1, seqs 30-61**. The seat casts Staff of Nin (seq 28) and pings the
opponent (seq 29) under Sanguine Bond + Exquisite Blood. The loop then resolves one iteration at a
time, and the engine puts a **priority window to the model on every iteration**: 32 consecutive
records, two alternating 4-row menus (Sorin `+1` / `-2` / `-6` / hold), `chosen_text` **`pass` on
all 32**, opponent life 17 → 1 and seat life 24 → 40, one point per record. The game ended at t17.

This is NOT the wave-73/74 re-put loop — the board genuinely moves between windows (33 distinct
life pairs), the hold latch cannot close it (the rows' life-derived annotations move), and
`crossphase_identical_reputs` cannot see it (same phase). But the decision is the same decision 32
times and the model made it 32 times: **~48 minutes of decode, ~35% of that game's wall time, on a
board where `LOOP COMPLETE` was already on the screen.** The engine has the predicate
(`lifeLoopProvenWin`) and already renders from it. Suppressing the ask while a proven-win loop is
mid-resolution — the same class as `chain_windows_only_selfharm` — would have cost this game nothing.
**Repro:** `1789074103-ai_baka_deck126-0x56537b124da0-vs-ai_baka_deck123.jsonl` seqs 30-61;
option-set hashes alternate between exactly two values; every `chosen_text` is `pass`.

### HIGH-2 — the land-drop window and the casting menu share one hold-check seam

All **22** false hold-check brackets in this corpus (15 false `every row above was also on the menu
at the last window I asked you at this seam`, 7 wrong `N rows above are new` counts) sit on a
`cast`-seam pair where one of the two windows is the **land-drop menu**. `writeTransLog` maps every
`kind: ask` record to the `"cast"` seam and `holdReopenNote("cast", menu)` is called from the
casting builder, so a land-drop window's rows overwrite the casting menu's referent and vice versa.
**0 of the 1,011 pairs where neither window is a land drop is false.**

**Repro:** `125v162` deck162 **seq 20** prints `every row above was also on the menu at the last
window I asked you at this seam`; the previous `ask` record at that seat (seq 19) is the land-drop
menu whose key set is `{play swamp, play island, play no land right now}` and shares nothing with
seq 20's casting rows. Also `125v130` deck130 seq 7/8, `146v125` deck146 seqs 59/61 and 97/98,
`146v126` deck126 seqs 11/12.
**Fix direction:** the land drop is its own question; give it its own seam string (`"land"`), or
have `writeTransLog` route the commit by the window's own builder rather than by record kind. This
is the last member of the wave-75 HIGH-1 family and it is small.

### MED-1 — 109 byte-identical re-puts, 96% re-declined, and the note that should explain them is inert

`crossphase_identical_reputs` 111 is the measure Q13 was built to get and it is large: 109 rendered
brackets, **105 answered with the same decline**, worst seat `125v123` deck123 at 51. But **104 of
the 109 print `the board HAS moved since then`** and only **5** print `nothing on the board has
changed`. The rows, the turn and the question are byte-identical by construction, so a 95%
"board moved" rate means `w76PhaseFreeBoardKey` (which removes only the serialiser's leading
`Phase: … | It is …` line) leaves other phase-varying content in the key. CQ's own
Weakest-evidence #5 named this as the fail-closed-but-dead outcome. The seat is told a list it
already declined is being re-put AND that the board moved — which invites it to re-read rather than
re-decline, the opposite of the intent. **Fix direction:** either extend the phase-free board key to
the remaining phase-shaped lines (stack line, combat-next tag, mana-pool line) with a pin per line
removed, or state the true weaker fact (`the rows are identical; the board has moved`) rather than
implying the re-put is uninformative.

### MED-2 — the X SUB-MENU prices kills it does not cover

The Q5 kill clause reaches the **parent** cast row but not the X sub-menu that follows it.
`123v130` deck130 **seq 20** row `Cast Starstorm {r}{r}{x}` carries
`{crack-back cover: … This row REMOVES 2 of those 2 attackers … leaving 0 -> you would be at 20,
which you SURVIVE …}`. **seq 22**, the X announcement for that same cast under the same
`CRACK-BACK NEXT TURN: 2 of their creatures … for up to 5 - you would be at 15` line, offers
`X = 1 {X pricing: kills THEIRS: none…}`, `X = 2 {… kills THEIRS: Vampire…}`,
`X = 4 {… kills THEIRS: Bloodline Keeper, Vampire…}` — and **no row carries a cover clause**, so the
window where the seat actually chooses how many attackers to remove is the one that does not price
it. Same shape at the other X menus. 2 rows in this corpus; the class is the X seam.

### MED-3 — five forced closes still pay a full phase-2 decode for nothing, and the Q8 bound cannot reach them

`forced_close_unrecorded` 5 with `force_close_arms_refused` / `_deferred` / `_defer_bound_hits` all
**0**. Every one of the five armed with the park EMPTY — a **same-arm re-arm**, which
`w76ForceCloseDecision` returns `kW76CloseArm` for by design. The window then moved under the close
(`dropping stale async answer (casting arm; the question and board moved) [outcome: re-asked]` on 4
of the 5) and the phase-2 answer was discarded. Cost: 5 × ~13 KB of reasoning, ~80 s each. No
decision was lost. This is precisely CN's Q8 Weakest-evidence #4: the bound is correct and
irrelevant, and the same-arm case is where the waste lives. **Fix direction:** before buying the
phase-2 decode, check the window key the close was armed on is still the live one (the async slot
already carries it) — a same-arm re-arm on a moved question should drop the close, not pay for it.

### LOW-1 — a 2,374-byte option row, and the roster class has moved next door

Q2/F7 bounded the `- THEIRS:` victim roster (longest 87 B this corpus, from 9,250 B) but the
longest single option row is now **2,374 B** — `152v126` deck152 seq 63,
`Cast Fateful Absence {1}{w} … {kills whichever you target: THEIRS - <long roster>}`. Same
unbounded-enumeration shape, different emitter, and CP's Weakest-evidence already named
`sweeperSurvivorTail` as the next one. Not urgent at 2.4 KB; worth the same `joinVictimRoster`
treatment before a wide board finds it.

### LOW-2 — the plan-names-a-dead-card measure has a counter but no note

`plan_names_uncastable_zone_card` fires 102 times and the prompt says nothing to the seat. The
sibling case already has a note: when the plan names a card still in the LIBRARY the prompt prints
`(note: your plan names "Thraben Doomsayer", and no "Thraben Doomsayer" is in your hand, on either
battlefield, in a graveyard, in exile or on the stack right now - a copy is still in your LIBRARY,
so you would have to draw or search it out before you can act on it. Re-state your plan if it has
gone …)`. A plan naming a card **in the graveyard** — 31 renders of Bloodline Keeper alone, on
`125v123` deck123 seqs 72/102/129/267 among others — gets the counter and no note. This is a
measure-to-render promotion, not a new mechanism; it belongs to a lane that can pin the wording.

### LOW-3 — HARNESS: the rerun harvest keys on mtime

Nineteen one-game pools each reported harvesting 38, 37, 36 … 17 `player-game logs`, descending
with completion order: the harvest copies every seat log in the shared dir newer than the pool's
start, including other pools' **in-flight** (gameend-less) logs. The `-final` dir had to be rebuilt
by hand. `tools/corpus-early-check.py` already keys on the outdir's game manifest; the harvest
should do the same. Nothing in this corpus is tainted (42 clean seat logs, all with gameend, verified
against `results.tsv`), but a future aggregation that trusted the harvest count would be.

---

## 5. The owner's lategame specimen (standing rule, alert-only)

`wave76/lategame-specimen.txt` — `125v123` deck123 **seq 615**, casting decision at **turn 67**,
1 life vs 66, 20,258-byte prompt, 27.5 s, 5,191 reasoning chars, answered
`Cast nothing right now`. Written verbatim with a three-line header. (Longest game this corpus:
`125v123`, 66 turns on `results.tsv`, 67 as the records number it.)
