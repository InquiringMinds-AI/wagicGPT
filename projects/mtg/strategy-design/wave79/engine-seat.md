# Wave-79 engine-narration seat

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-000748-final`
(single binary `wagic-5af6f7a95-w79step1` = source b1e1db7be, one run, 21 concurrent, 5 h 16 min,
21 games, 42 seat logs, 0 HUNG, 0 timeout-tainted). Item (5) is RETIRED by the owner's
2026-09-12 ruling: no lategame specimen is written. Everything below is measured off the
corpus, the 21 `game-*.stderr` files and `askreplay/`; the source was read but nothing was
edited and neither git nor wagic was run.

---

## 1. Corpus census

| figure | wave 79 | wave 78 |
|---|---|---|
| games / seat logs | 21 / 42 | 21 / 42 |
| record-kind sum | 2,230 | 2,670 |
| model decisions (`protocol_replies`) | 2,203 | 2,629 |
| results.tsv game turns (sum) | 456 | 487 |
| decisions per seat-turn | 2.42 | 2.70 |
| decisions/turn as the census prints it | 1.86 | 1.88 |
| fallbacks, ask seam | **0** | 7 |
| fallbacks, all kinds | 1 (`reveal`/`engine_answered`) | 1 |
| `reply_truncated` / `action_before_plan` | 0 / 0 | 0 / 0 |
| `off_protocol_bytes>0` | 87 (3.9%) | 96 (3.7%) |
| `plan_line_missing` | 99 (4.5%) | 103 (3.9%) |
| latency p50 / p95 (ask seam) | 96.2 s / 196.3 s | 78.6 s / 179.8 s |
| `reasoning_chars` median / max | 5,835 / 21,069 | 5,871 / 17,847 |
| `async_drops` | 15 | 46 |
| `askreplay/` files | 29 | 30 |
| corpus mean prompt bytes | 12,034 | 13,300 (DB's measure) |

* **transport**: `curl=0,http=200,empty=0` on all 2,203 round trips — one value, no variance.
* **deadline_pct**: n=2,203, p50 10.4, p95 21.6, max 47.6. **Zero** windows at or above 80%.
  The 900 s wall was never approached; no wall-miss, no no-retry (stderr sums confirm 0/0/0).
* **reply shape**: `protocol_deviation` = compliant 2,104 / `unlabelled_plan` 87 /
  `plan_absent` 12 / `answer_label_absent` **0**. 100% of 1,696 asks carried native
  reasoning with `thinking: on` — invariant 000 holds for the tenth corpus.
* **`plan_steps_done`**: median 1 (1,153 windows at 1, 1,045 at 0, 5 at 2) against a
  `plan_step_count` median of 2 (1,037 one-step plans, 858 two-step, 240 three-step,
  8 zero-step). Roughly half of all windows report no step of the standing plan done.
* **Why fewer AND slower.** Turns fell 6% (487 -> 456) and decisions per seat-turn fell 10%
  (2.70 -> 2.42): the total window population (model calls + every engine-suppressed class)
  fell from 7,939 to 5,620, -29%. Latency is **not** a prompt-bytes effect: over all 2,203
  round trips `corr(latency, prompt_bytes) = -0.30` (negative) while
  `corr(latency, reasoning_chars) = +0.75`. But the median reasoning length barely moved
  (5,871 -> 5,835), so the +18 s p50 is **not explained by any field this corpus carries** —
  the most likely remainder is server-side/concurrency, which a single un-replicated run
  cannot separate. I did not check the serving host's state during the run; UNDETERMINED.

---

## 2. Regression watch on the wave-79 changes

### (a) FALLBACKS — the ask seam is clean

Every `fallback`-named field in all 2,230 records was enumerated. Exactly two non-empty
values exist, and they are the same event:
`1789189673-ai_baka_deck152-...-vs-ai_baka_deck126.jsonl` seq 24 `reveal`
`fallback: engine_answered`, `latency_ms: -1`, `choice: -1`,
`chosen_text: "none (no legal target)"`, `options: 5`, and seq 25 `recovery`
`recovers_fallback: engine_answered`. **Zero-eligible and genuine**: the window had no
legal target, so there was no answer for a model to give; the engine answering it is the
right behaviour and it is not a degraded decision. First corpus since wave 76 with **0 ask-seam
fallbacks** (wave 78: 7 `unparsed_reply`).

`answer_label_absent` = 0. In wave 78 it was 6, and re-measuring the wave-78 corpus
(`matchups-20260911-125420-final`) shows **all 6 were deck125 seats and no other deck**.
deck125's wave-78 guide edit 4 (pairing the label with the form) is the only variable that
changed on that path this wave — no engine or parser change touched it — so the **guide was
the cause**, on the strength of an exact deck-localised before/after. Lane DD's parser fix
(merged at 5bc856d84 AFTER this corpus) is therefore **UNTESTED by this corpus**.

### (b) T2/CZ — the stop-reached collapse

`stop_reached_reputs_collapsed` 5, `stop_reached_windows_skipped` 8 (wave 78: 0 / 44).
All 8 are in ONE game, on ONE seat: `1789189682-ai_baka_deck123-...-vs-ai_baka_deck162`
(gameend 8 / 5). stderr `game-162v123-1789189679.stderr`: 3 wave-72-arm skips
(lines 1964/1966/1968, ph10/ph11) + 5 re-put collapses (lines 1983/1988/1994/2034/2064,
ph3 x3, ph4 x2), all on stated stop 24, all with `declined 1 time(s) this turn`.
Sum 3+5 = 8 = the gameend figure exactly.

**Every stop-reached chain in the corpus, found.** Only **3 asked windows corpus-wide render
a stop clause at all** (`your stated stop=`), all on that same seat: seq 115 (t12 M1,
`M=4, stop=24`), seq 127 (t12 M2, `M=4, stop=24`, the seat took the repeat row at N=20 and
went to 24), seq 131 (t13 Upkeep, `M=24, stop=24, ALREADY AT OR PAST`, declined). The
collapses are the re-puts after that decline. **The arm fired on every eligible re-put**;
0 eligible-but-refused (the only asked at-stop window, seq 131, was the turn's FIRST put,
where `declinedN` is 0 by design).

**No collapse is a LOST LEGAL OPTION.** The arm requires every base row to be stop-priced;
on each of the 5, the only base rows were the seat's own Bloodline Keeper repeat family
already at its own stated stop of 24, the identical list had already been put and declined
that same turn, and the immediately preceding stderr line reads `cast ask NOT issued - the
oracle finds no legal cast in this instant-speed window`. The only hidden action is making a
25th vampire past the seat's own stop.

**Why 5 and not CZ's 20-35: the state barely arose, not the arm refusing.** Three asked
stop-clause windows this wave against wave 78's 44 skipped stop-reached windows. deck123 went
4/6 -> 0/6 and its long token-loop games did not recur. The fix reached the live caller
(0 -> 5 is a first non-zero) — the band was set against a population that did not repeat.

### (c) T3/CZ + DC F1 — the CG shape

Measured on the OPTION ROWS (the full prompt is never byte-identical because the narration
moves; the same instrument on the wave-78 corpus is the positive control):

| same seam, same turn, same phase | wave 79 | wave 78 (same instrument) |
|---|---|---|
| option-row **byte-identical** re-asks | **3** | 192 |
| identical after stripping `{…}`/`[…]` | **19** | 235 |

A **98.4% collapse**, but not the predicted 0. The 3 byte-identical survivors are all the
same shape — a fetchland crack row on the **priority** seam, both windows answered `pass`:
`1789189698-ai_baka_deck123-...` seq 277->280 (t37 M1), and
`1789189711-ai_baka_deck123-...` seq 98->100 (t19 Upkeep) and 102->104 (t19 M1), all
`Put a card onto the battlefield with Marsh Flats targeting Plains [cost: Tap, Life, Sacrifice]`.
In each, a legal continuation genuinely moved (a new object entered the stack, or the
already-answered-this-phase casting note changed), so the engine was right to ask — but the
menu the model saw was byte-identical to one it had just declined, which is the shape CZ set
out to remove.

**`ask_key_continuation_differs` 118 (NEW).** Replicating the population from the records
(group by kind/turn/phase over action-normalised rows) yields 52 recoverable pairs — a lower
bound, since a window the engine then suppressed leaves no record. Verified on all 52:
- 37 show the `{leaves N of your M untapped mana sources}` figure moving (the digest's
  `sources=` term — every X ceiling and every cost figure is priced in it);
- 7 more show a `{right now: …}` target/verdict change;
- the remaining 10 were read line-by-line and **every one has a moved continuation**: e.g.
  `1789189684-ai_baka_deck162-...` seq 46->48 (opponent's `Fall of the Gavel` arrives on the
  stack, targetable), `1789189698-ai_baka_deck123-...` seq 277->280 (`Staff of Nin` resolves
  and puts 1 damage on the stack), `1789189706-ai_baka_deck125-...` seq 62->64 (`Path to
  Exile` paid, a row gone). **0 of 52 are spurious.**

**The replay swap explained.** `identical_ask_answers_reserved` = `mRepeatAskAnswersReserved`
counts ONLY the second re-serve path (the repeat latch); `ask_replays_reserved` counts both.
The repeat latch is only reached when the ask CACHE misses. The T3 key change (turn+phase+
continuation digest instead of the serialised board) made the cache hit on exactly the windows
that used to fall through to the latch: wave 78 = 1,344 total of which 953 latch / 391 cache;
wave 79 = 1,084 total of which **0 latch / 1,084 cache**. Replays did not stop; they moved
path. Confirmed independently: `askreplay/` holds 1,084 lines, **all** `why: cache_replay`,
0 `repeat_ask_reserved`, 0 refusals; and the 42 stderr game-end lines sum
`repeated identical asks re-served from the seat's own answer: 0`.

**Wrong replays.** None found. The longest chain (`replay_run` 33,
`1789189698-ai_baka_deck123-...`) is a dead menu — `NO LIVE CAST ROW ON THIS MENU: all 1
cast row below carry a verdict computed from the board that reads zero` — declined once and
replayed. Run distribution: 300 at run 1, tailing to 2 at run 12, max 33.
**Residual aliasing risk, stated honestly**: the digest carries target identities plus the
untapped-source count and nothing else — no life total, and a stack object only when a row
can target it. A replay that is wrong *because the life total moved under it* is outside the
digest's stated scope and cannot be detected from these records. I did not attempt that.

### (d) The hold latch

`hold_windows_skipped` 1,598 (cast 1,080 / priority 518; wave 78: 2,816) — confirmed to the
unit by the 42 stderr game-end lines (`windows held by the model's own hold row` sums 1,598).
stderr `hold re-opened` = **100** total: 85 `a printed row changed or is newly available`
(wave 78: 194) + 15 `a printed row it was held over is gone`.

**`hold_verdict_safer_ignored` 59 — CANNOT BE SAMPLED.** The increment at AIPlayerGPT.cpp:35476
carries **no DebugTrace and no record field**, and the three verdict markers it compares
(`[crack-back verdict:`, `[stack death verdict:`, `[own loop verdict:`) are latch keys, not
prompt text — `[crack-back verdict:` and `[stack death verdict:` appear **0 times in all 2,203
prompts**. So neither the window, nor the held face, nor the live face is recoverable from the
corpus. The brief's "sample 15 — was the ignored safer verdict truly safer" is **not
answerable from this corpus**; this is a MED instrumentation defect (see 4/MED-2), not a
finding about the clamp. What *is* observable: the clamp fired 59 times and the re-open rate
fell from 194 to 85, which is the direction T3 argued for.

**`hold_reopened_new_threat` 0 — UNTESTED, not FAIL.** The branch is an `else if` under the
clamp and needs a held marker whose live counterpart differs at the SAME danger rank. The
`NEW threat at the same rank` DebugTrace fired **0 times in all 21 stderr files**, and the
rank of any window's held/live marker is not recorded anywhere. Only two distinct
`[own loop verdict:` faces were ever rendered corpus-wide (both benign: "proven win, no link
on the stack" and "RESOLVING"); no THREATENED and no UNPROVEN face appears in any prompt.
I therefore **cannot show a new equal-rank threat ever arose**, and by the owner's stated
condition that makes it UNTESTED. It becomes a FAIL only when a corpus is shown to contain
one, which requires the recording gap above to be closed first.

**`crossphase_board_unchanged` 26 (wave 78: 4), `crossphase_identical_reputs` 172.** 170
cross-phase re-put notes are present in prompts (2 counted windows left no record — see
4/LOW-1). 26 of the 170 carry `nothing on the board has changed`, matching the counter
exactly. Five, all from `1789189692-ai_baka_deck123-...`:
- seq 75: `[this exact list was put to you 1 window ago at Main phase 1 and you declined; nothing on the board has changed]`
- seq 76: `… 1 window ago at Combat begins and you declined; nothing on the board has changed`
- seq 79: `… 1 window ago at Main phase 2 and you declined; nothing on the board has changed`
- seq 80: `… 1 window ago at End and you declined; nothing on the board has changed`
- seq 93: `… 1 window ago at Main phase 2 and you declined; nothing on the board has changed`

The 6.5x rise over wave 78 is expected: the wave-77 R8 rewrite made the negative branch state
the row guarantee, and the positive branch now fires wherever the phase-free board key is
literally equal. All 26 are one seat walking one unchanged board across five phases of a turn.

### (e) T4/CZ + DC F10 — the own-loop verdict

`own_loop_verdict_lines_rendered` 63, `own_loop_windows_asked` 4, and **56** prompts contain
`[own loop verdict:`. The 7-line gap is a **counter defect, found and explained**: seven
records carry the record field with the value `[own loop verdict: none]` — i.e. no verdict and
no rendered line — and the counter still incremented. Repro, all `ask`, all Main phase 1:
`1789189686-...deck126` seq 30; `1789189698-...deck126` seqs 20, 30, 32, 44, 61;
`1789189713-...deck126` seq 23. Per-seat, the record-field count equals the gameend counter
exactly (5/4/42/12 = 63), so the miscount is at the field write, not the record.

All **56 rendered lines are on ASKED prompts** (they are in records by construction), so
DC F10's "none on a suppressed window" holds for everything observable. Only two faces ever
rendered:
- `[own loop verdict: your life loop is a proven win, and no link of it is on the stack right now - nothing of it is waiting to resolve, so a hold here covers no link of it]` (`1789189686-...deck126` seq 28) — TRUE against that window's stack (empty of loop links).
- `[own loop verdict: RESOLVING - a piece of your own proven life loop is on the stack and nothing of theirs targets it: the loop closes without another action from you, and the HOLD row covers every remaining link]` (`1789189713-...deck126` seq 45) — TRUE; `ON THE STACK` on that prompt carries the loop link.
No THREATENED and no UNPROVEN face rendered anywhere: **those two faces are UNTESTED.**
Of the 4 `own_loop_windows_asked`, the seat took the HOLD row where the verdict said the hold
covers the remaining links (seq 45) — i.e. it acted consistently with the line.

`menu_pass_no_progress_suppressed` 1, quoted from stderr:
`AIPlayerGPT: cast pick made no progress, suppressing: Cast Starstorm {r}{r}{x} {X pricing: max affordable X=1 (3 mana total); even at X=1 NOTHING on the board dies} [<- no X on this menu kills anything of THEIRS]`.

### (f) T5/T6/T9/DA + DC F3/F4/F5 — the combat headers

The corpus carries **16 `blockers` windows**, and every one of them prints a
`best case with every blocker assigned` figure (the header literal is `best case`, not
`AT BEST`; `AT BEST` appears 0 times). I checked all 16 by hand against the header's own
attacker damages and the engine's own per-pair verdicts:

**All 16 best-case figures are correct and none is beaten by a legal assignment.** Spot list:
#3 (`1789189673-...deck126` seq 21) 19 life, Sigarda 5 unblockable-by-you + Tracker 3, one
Wall blocks the Tracker -> 14 ✓. #7 (`...deck162` seq 20) 4 life, 3+3+7=13, one legal
blocker, best is to eat the 7 -> -2, printed with `no block saves you` ✓. #11
(`...deck126` seq 25) 1 life, 3+2+4+2=11, one blocker, best blocks the 4 -> -6 before
triggers, -3 after the 3 the block gains ✓. #8 (`...deck152` seq 15) 19, lifelink blocker
-> `19 before your blocking triggers and your blockers' lifelink, 23 after the 4 those blocks
gain you` ✓. **No header prints two disagreeing verdicts**; two headers carry both
`this KILLS you` and `you SURVIVE at 1 if you block`, and both qualify it as
`this KILLS you ONLY if you decline every block` — one verdict, two branches.

First/double strike **is** modelled correctly across both steps: `...deck162` seq 10,
A1 `Triumphant Adventurer (2/2) [first strike, deathtouch]` vs B1 `Fate Unraveler (3/4)`
reads `your blocker dies, attacker lives` ✓ (first-strike deathtouch kills before the blocker
swings).

**Not exercised this corpus, therefore UNTESTED, not passed:**
- **trample spill** — the only trample attacker (Sigarda) was unblockable by the seat;
- **menace** — no attacker carried menace in any of the 16;
- **past 12x12 drop** — max board was 4 attackers; the claim-drop path never ran (0 windows);
- **out-of-model ability drop** — 1 of 16 windows had a shroud attacker; `blocker_forecast_collapsed` = 0 and `no assignment of your blockers does better` renders 0 times, so DC F3's honest-unsearched wording never fired.

`blocker_forecast_rows` 72 / multi 26 / gang 14 / collapsed 0. Only **4** rendered
`[GANG BLOCK: …]` brackets exist, 2 distinct:
`[GANG BLOCK: your 2 biggest together deal 3, enough to kill this attacker; each B-line result below is a LONE blocker only]` and the 3-biggest/4 variant. Both are true against their
boards. **Neither states a spill, because neither attacker had trample** — the spill clause
is UNTESTED.

**T6 crack-back cover — PASS, measured.** 126 `{crack-back cover:` renders in 95 prompts;
111 parse to the full `is N from K of their creatures and puts you at L` form, and in
**111 of 111 all three figures equal the `CRACK-BACK NEXT TURN` line on the same prompt**.
0 mismatches. 54 carry the `OR LOWER … FLOOR, not a ceiling` extra.

**T9 sacrifice rows — UNTESTED.** 140 distinct `[cost: …, Sacrifice]` rows were rendered, and
**every one of them sacrifices its own source** (fetchlands: Marsh Flats; Clue tokens). Not a
single row in the corpus has a sacrifice cost that can take an untapped blocker, so
"prices the spent blocker without double-counting" had no board to run on. There is nothing to
quote and nothing to fault.

**T9 ATTACK TOTAL — the fold rendered once.** 84 `ATTACK TOTAL` lines, 68 distinct. Exactly
**1** prompt carries `power survives the field` (`1789189690-...deck152` seq 42):
`…That kills 4 of them (…), so 12 of the 12 above never gets dealt: 0 power survives the field
and goes to the declaration. That is the power BEFORE blocking - what reaches THEM is whatever
their blockers do…` — **no life projection**, exactly as DC F5 required. `reaches them and
they would be at` renders **0** times. So the fold's shape is right in its one instance, and
its rate is one window.

**DC F4 relief clause — did not render.** `off your best case WITH blockers` appears **0**
times in 2,203 prompts; `count it back and you would be at` also 0. The negative half holds,
the positive half never fired.

### (g) T7/DA + DC F8 — the `UNLESS … untaps them` clause

**2 distinct `UNLESS` clauses corpus-wide, 46 occurrences in 31 records.** Only one is the
tap-bill family:
`1789189698-...deck126` seq 22: `…UNLESS Intruder Alarm (THEIRS) untaps it first: that untaps
creatures OUTSIDE the untap step, so a body tapped for mana here can be standing again before
the declaration this clause is about…`
Intruder Alarm is a **both-sides live trigger** (it untaps on any creature ETB, either
player's) and it is explicitly marked `(THEIRS)`. **0 clauses name a spent ETB.** The other
`UNLESS` (`1789189688-...deck146` seq 3) is an unrelated land clause
(`UNLESS its own condition holds … "If you don't, Shineshadow Snarl enters tapped."`).
82 `{paying this taps:` clauses render in 42 records; the one that sits on an untap-all board
carries the `UNLESS`. **PASS on its single instance.**

### (h) T8/DB + DC F6 — HELD and keep-X

`HELD: … castable the moment a legal target appears` renders **1,009 times in 402 records**,
and the two literals co-occur 1,009/1,009 — every `HELD:` note on a target-less instant
carries the clause.

The keep-X clause is the clean result of this wave: **`Holding up a counterspell:` renders 62
times in 62 records; `Holding up an instant:` renders 0 times.** Every card it names is a real
counterspell — Dream Fracture (25), Essence Scatter (20), Fall of the Gavel (14), Cancel (3).
**No Path to Exile, no Ertai-class card, and no card whose fizzle is behind a colon.** The
wave-78 shape (10 of 47 naming a non-counter) is gone.

### (i) T10/DB — prompt bytes

`prompt-byte-census.py` over this corpus (the corpus is already post-trim, so its
`now -> after` columns are identical by construction):

| deck | n | mean | max | >20 KB | >20 KB share |
|---|---|---|---|---|---|
| deck123 | 495 | 11,872 | 20,286 | 1 | 0.2% |
| deck125 | 415 | 12,310 | 25,471 | 49 | **11.8%** (wave 78: 44.4%) |
| deck126 | 192 | 10,782 | 20,359 | 1 | 0.5% |
| deck130 | 322 | 10,815 | **22,357** | 11 | 3.4% |
| deck146 | 427 | 15,019 | 23,712 | 97 | 22.7% |
| deck152 | 203 | 10,451 | 20,328 | 2 | 1.0% |
| deck162 | 149 | 9,656 | 20,199 | 1 | 0.7% |
| TOTAL | 2,203 | **12,034** | — | 162 | 7.4% |

deck130's max landed at 22,357 against DB's predicted ~22,000 (wave 78: 29,641), and its
over-24-KB count is **0**. deck125's over-20-KB share fell 44.4% -> 11.8%, past the predicted
~34%. Corpus mean 12,034 B is below DB's predicted 13.8 KB. **deck146 is now the heaviest deck**
(mean 15,019, 97 prompts over 20 KB) — the byte problem moved, it did not go away.

**The hoist fired**: `Card text shared by option…` renders **136 times in 132 prompts**
(wave 78: 0). Example `1789189671-...deck123` seq 66:
`Card text shared by options 1-4 (they all act with the one card this text belongs to, so it
is printed once here instead of on each of them): "{T}, Pay 1 life, Sacrifice Marsh Flats: …"`.

**Did the trim lose the seat's own last PLAN, a stated stop, or the loop state?** No, on
everything measurable: `THE PLAN YOU LAST STATED` is present on every prompt I sampled
including the largest; all 3 stop-clause windows kept their clause; all 56 own-loop verdict
lines are present on their prompts (the 7 missing ones are `none`, i.e. never rendered —
see (e), that is a counter bug, not a trim loss). I did **not** diff pre-trim against
post-trim prompts — the corpus only stores what was sent, so a trim loss that removed a whole
section leaves no trace here. That check is not possible from this corpus.

### (j) T11/DB + DC F7 — the glosses

4,851 `{card text:}` / `{target text:}` / `{effect:}` renders, **141 distinct bodies**, all of
them read:
- **0 renders end on a bare `...`** ✓
- **0 renders have an unbalanced `(` or `"`** ✓
- 34 distinct bodies carry `(...more)`.

**2 of the 34 cut MID-CLAUSE** (DC F7 / T11 fail, see 4/MED-1):
- `1789189688-...deck146` seq 22: `…{3}{B}: Until end of turn, Hive of the Eye Tyrant becomes a 3/3 black Beholder creature with menace and (...more)` — cut after `and`.
- `1789189711-...deck146` seq 54: `… -8: You get an emblem with (...more)` — cut after `with`.

The Chrome Mox case DC F7 names never arose: `Chrome Mox` and
`of any of the exiled card's colors` render **0** times. That half is UNTESTED.

### (k) T12/T13/T14/CZ + DC F9 — brackets, drain, force-close

**Hold-check brackets: 1,814 rendered, every one on a record carrying
`hold_check_ref_seq`.** For the 1,693 whose ref seq resolves to a record in the same log I
recomputed the new/gone sets from the option rows (decline/hold/pass rows normalised the way
the prompt itself says they compare): **1,690 of 1,693 claims are TRUE** (99.8%).
The 3 exceptions are ref-window kind mismatches or a row whose TARGET LIST is part of its name
(`1789189711-...deck146` seq 103, `Cast Vanishing Verse - legal targets right now: …`), not a
systematic miscount. `N gone` is true in all 1,690.

**Rank shifts are not counted as new** ✓ — 15 rows across the corpus differ from a prior row
only by an ordinal, and none of them appears in a bracket's `is new` count. `(a price,` renders
**0** times ✓. Brackets printing BOTH `is new` and `is gone` exist in quantity (246 + 82 + 51
+ … of the 1,814) ✓.

**Drain clause: 18 windows, and all 18 carry BOTH `OF THIS STACK` and `released at your next
untap`** ✓. `stack_drain_windows_asked` 30 vs 18 rendered clauses — 12 counted windows left no
rendered clause in a record (same family as 4/LOW-1). Two same-turn drain pairs exist
(`1789189682-...deck123` seq 39->40 t8 Draw and 71->72 t12 Draw) — **but both cross a seam**
(ask/cast -> priority), which the engine's own scoping treats as a different question, and in
both the seat took the hold at the first. Literal prediction violated; the promise was not.

**Force-close identity closes to the byte.** The 42 stderr game-end lines sum: forced closes
**25 events / 12 superseded (dropped_decision_moved)**, deadline misses 0 (0 wall-miss
unrecorded, 0 no-retry), phase-2 answer recovery **1 recovered / 0 missing** — identical to
`forced_close_events` 25, `forced_close_unrecorded` 12, `phase2_answer_recovered` 1,
`phase2_answer_missing` 0. Same lines also reconcile `hold_windows_skipped` 1,598,
`mana_only_windows_skipped` 129, `identical_option_asks_resolved` 7 and
`identical_ask_answers_reserved` 0. **`declined_face_latches` is absent from every gameend
record** ✓ (retired).

### (l) S12/T15 — the life trend

`Opponent life trend:` renders on **2,139 records**, 369 distinct forms. **922 are split
gained/lost lines**, and in **922 of 922 the arithmetic closes**: `gained - lost == net`, 0
wrong. The `+0` face is printed when one side is zero — 568 records carry a `+0`, e.g.
`Opponent life trend: turn 13: 20, turn 15: 19, turn 17: 18, now 16 (-4 since turn 13; over
those turns life-gain EVENTS put +0 on them and life-loss EVENTS took -4 off …)`.

deck162 specifically (DB's T15 prediction: split lines 9 -> ~48, net-only -> 0): **87 split
lines** and **52 net-only lines, every one of which is `unchanged at N since turn M`** — i.e. a
window where life genuinely did not move and there is nothing to split. **0 residual net-only
lines over a changed life.** PASS, above the predicted band.

### (m) The rest

- **`async_drops` 15 (wave 78: 46) — all re-asked.** 15 stderr lines, all identical:
  `dropping stale async answer (casting arm; the question and board moved) [outcome: re-asked]`.
  0 dropped without a re-ask.
- **`mana_only_windows_skipped` 129** (wave 78: 367), confirmed against stderr.
- **Latency / decision count**: see §1. The rise is tied to reasoning length per call
  (r = +0.75), not prompt bytes (r = -0.30); median reasoning length is flat, so the corpus
  does not explain the +18 s.
- **`plan_line_missing` 99 (4.5%)**, concentrated: deck146 45, deck130 18, deck152 17,
  deck162 11, deck125 8, deck123 0, deck126 0. `off_protocol_bytes>0` tracks it almost
  one-for-one per deck (42/17/14/6/8/0/0) — the same windows.
- **`plan_names_stranded_card` 3 / `plan_names_uncastable_zone_card` 23** (wave 78: 2 / 35).
- **Wins: deck123 0/6 (was 4/6), deck125 1/6 (was 3/6).** Hands are real and I did not trace
  either deck's deciding decisions — that is the per-deck seats' work. One wave-79-visible
  item does sit inside deck125's losses: **both** of the corpus's `{X}`-announcement payment
  losses are deck125 seats (see 4/HIGH-1), 5 mana and 4 mana burned for nothing in
  `game-152v125` and `game-125v126`. For deck123 I found **nothing** — its seats carry 0
  `plan_line_missing`, 0 `off_protocol_bytes`, and its only wave-79 render exposure is the
  stop-reached collapse, which hid no legal option.
- **Harness (n)**: no tripwire fired, 0 HUNG, 0 crashed, all 21 games finished naturally in
  5 h 16 min; 42/42 seat logs carry a gameend; census reconciles
  (`CENSUS CHECK RECONCILES.`, gap 42 = 42 seat logs x the +1 narration offset).

---

## 3. Prediction adjudication

**LESSON OF WAVE 78 applied**: a 0 is a FAIL unless the state is shown never to have arisen.

**Scoreboard: 45 PASS / 8 FAIL / 7 UNTESTED** (CZ 12/5/0, DA 8/0/3, DB 13/1/0, DC 12/2/3, DD 0/0/1).

### lane-CZ

| # | prediction | verdict | evidence |
|---|---|---|---|
| T2 | `stop_reached_reputs_collapsed` > 0 on deck123's seats (band 20-35) | **PASS (band missed)** | 5, all on `1789189682-...deck123`. >0 met; band not — only 3 stop-clause windows arose corpus-wide |
| T2b | no (turn, phase, seat) cell shows more than two asks over a stop-reached menu | **PASS** | max 1 asked at-stop window per cell (seq 131) |
| T3a | same-phase byte-identical re-asks -> 0 | **FAIL** | 3 (192 on the same instrument in wave 78) |
| T3b | strip-identical same-phase re-asks -> 0 | **FAIL** | 19 (235 in wave 78) |
| T3c | `ask_replays_reserved` up by >= 135 over 1,344 | **FAIL** | 1,084, DOWN 260 — but the population moved path (all cache, 0 latch); see 2(c) |
| T3d | `hold_verdict_safer_ignored` > 0 (>= 20) | **PASS** | 59 |
| T4 | `own_loop_verdict_lines_rendered` >= 2 AND equal to prompts carrying the literal | **PARTIAL FAIL** | 63 vs 56; >= 2 met, equality not (7 `none` writes) |
| T4b | >= 1 record carries `own_loop_verdict` on deck126's seats | **PASS** | 63 records, all deck126 seats |
| T13a | every drain clause carries `OF THIS STACK` and `released at your next untap` | **PASS** | 18/18 |
| T13b | `stack_drain_windows_asked` > 0 with >= 1 rendered clause | **PASS** | 30 counted / 18 rendered |
| T13c | 0 drain-hold pairs re-asked inside the SAME turn | **FAIL (literal)** | 2 pairs, both CROSS-SEAM (39->40, 71->72) |
| T12a | >= 1 bracket prints both `is new` and `is gone` | **PASS** | 379+ brackets |
| T12b | 0 brackets print `no row that was on it is gone` over a menu whose rows gained a `#2` | **PASS** | 15 ordinal-only rows, none counted as new |
| T12c | 0 prompts contain `(a price,` | **PASS** | 0 |
| T14a | every game-end stderr line carries wall-miss + superseded clauses | **PASS** | 42/42 |
| T14b | 0 gameend records carry `declined_face_latches` | **PASS** | 0/42 |
| T14c | force-close identity closes to the byte | **PASS** | stderr 25/12/1/0 == gameend 25/12/1/0 |

### lane-DA

| # | prediction | verdict | evidence |
|---|---|---|---|
| T5a | 0 headers whose best-case figure a legal assignment beats | **PASS** | 16/16 verified by hand |
| T5b | `exact_block_search_ran` > 0 | **PASS (by proxy)** | all 16 print a named optimal assignment; the field itself is not on the records |
| T5c | 0 headers with both `no assignment of your blockers survives` and `and the line above does not` | **PASS** | 2 / 0 |
| T5d | trample/menace headers' figure >= the life actually held | **UNTESTED** | no blockable trample or menace attacker in any of the 16 |
| T6 | 0 windows where cover and `CRACK-BACK NEXT TURN` state different totals | **PASS** | 111/111 agree, 0 mismatch |
| T6b | `crackback_cover_with_floor_extra` > 0 | **PASS** | 54 |
| T7 | every `{paying this taps:` on an untap-all board contains `UNLESS`; 0 false clauses | **PASS (n=1)** | 82 tap-bill clauses, the one untap-all board carries `UNLESS Intruder Alarm (THEIRS)` |
| T9a | `sacrifice_row_blocker_priced` > 0; 0 unpriced sacrifice rows | **UNTESTED** | 140 sacrifice rows, every one sacrifices its own source; no blocker-taking cost arose |
| T9b | `attack_total_punisher_folded` > 0; 0 bad declarations | **PASS (n=1)** | 1 `power survives the field` render, correct, no life projection |
| T16a | 0 prompts where `ON THE STACK:` and `DRAW FORECAST` disagree | **UNTESTED** | 360 `ON THE STACK` + 300 `DRAW FORECAST` records; I did not reconcile the two figures window-by-window |
| T16b | `untapped_face_rendered` > 0 | **PASS** | `[untapped]` renders 345 times in 181 records |

### lane-DB

| # | prediction | verdict | evidence |
|---|---|---|---|
| T8a | keep-X names a counterspell in 0 of N non-counter cases | **PASS** | `Holding up a counterspell:` 62, `Holding up an instant:` 0 |
| T8b | the two-copy `[no cast row now …]` shape at deck125 carries `HELD:` on every instant | **PASS** | 1,009 `HELD:` renders, all paired with the castable-the-moment clause |
| T10a | deck125 over-20-KB share 44.4% -> ~34% | **PASS (beaten)** | 11.8% |
| T10b | deck130 max 29,641 -> ~22,000; over-24-KB 57 -> 0 | **PASS** | 22,357; 0 |
| T10c | corpus mean falls ~10% | **PASS** | 13.3 KB -> 12.0 KB (-9.5%) |
| T10d | repeated-`{card text:}` bodies that PAY to hoist fall 43 -> 0 | **PASS (by proxy)** | hoist header renders 136x; I did not re-run the pay-to-hoist model over this corpus |
| T10e | hoist-header count of 0 on a menu with >= 3 identical bodies = FAIL | **PASS** | header present on 132 prompts, incl. 4-row fetchland menus |
| T11a | 0 glosses ending on a bare `...` | **PASS** | 0/141 |
| T11b | 0 renders with an unbalanced `(` or `"` | **PASS** | 0/141 |
| T11c | `(...more)` after whole units / dependent clauses kept together | **FAIL** | 2 of 34 cut mid-clause (deck146 seq 22, seq 54) |
| T15 | deck162 split lines 9 -> ~48, net-only -> 0 | **PASS (beaten)** | 87 split; 52 net-only, all genuine `unchanged at N` |
| T17a | `corpus-stats.py` prints `CENSUS CHECK RECONCILES.` | **PASS** | it does |
| T17b | engine-answered on its own line, 0 in the silent fallback tally | **PASS** | `engine_answered 1` printed separately; ask-seam fallbacks 0 |
| T1 | `answer_label_absent` drops to 0 at deck125 -> the guide's edit 4 was the cause | **PASS** | 6 (all deck125) -> 0 |

### lane-DC (the eleven Astra findings)

| # | prediction | verdict | evidence |
|---|---|---|---|
| F1a | `ask_key_continuation_differs` > 0 (band 30-200) | **PASS** | 118 |
| F1b | `menu_pass_no_progress_suppressed` > 0 | **PASS** | 1 |
| F1c | no replayed answer wrong for the new board | **PASS (within the digest's scope)** | 52/52 replicated pairs had a moved continuation; life-driven aliasing is outside the digest and unmeasurable here |
| F2 | `hold_reopened_new_threat` > 0 | **UNTESTED** | 0; no ranked verdict face renders anywhere and the branch has no trace — the state cannot be shown to have arisen |
| F2b | `hold_verdict_safer_ignored` stays > 0 | **PASS** | 59 |
| F3a | `blocker_forecast_rows` > 0 | **PASS** | 72 |
| F3b | `blocker_forecast_collapsed` > 0 with `no block saves you` absent from those windows | **UNTESTED** | 0 collapsed; only 1 of 16 blockers windows had any out-of-model ability (shroud, on an attacker) |
| F4 | >= 1 clause carries `off your best case WITH blockers`; 0 `count it back and you would be at` | **FAIL (positive half)** | 0 and 0 — the new string never rendered |
| F5 | >= 1 `power survives the field`; 0 `reaches them and they would be at` | **PASS** | 1 and 0 |
| F6 | >= 1 keep-X clause names a counter; 0 name a colon-fizzle card | **PASS** | 62 and 0 |
| F7a | 0 prompts with `of any of the exiled card's colors` without `exile a nonartifact` | **PASS (vacuous)** | both strings render 0 times |
| F7b | >= 1 prompt renders the Chrome Mox snippet with `(...more)` | **UNTESTED** | Chrome Mox never appeared in the corpus |
| F8 | >= 1 tap-bill clause names `Intruder Alarm`; 0 name a bare-resolution untapper | **PASS** | 1 and 0 |
| F9a | `hold_windows_skipped` > 0 and stderr `hold re-opened … changed or is newly available` < 194 | **PASS** | 1,598 and 85 |
| F9b | 0 brackets print `is new` over a descending-ordinal-only change | **PASS** | 15 ordinal-only rows, none counted |
| F10 | `own_loop_verdict_lines_rendered` > 0 AND equal to the prompt count AND to the record count | **FAIL** | 63 = record count but != 56 prompts; 7 `[own loop verdict: none]` writes |
| F11 | census prints `gap N = N seat logs x the +1 narration offset`, N = clean seat logs | **PASS** | `gap 42 = 42 seat logs x the +1 narration offset` |

### lane-DD

Merged at 5bc856d84 **after** this corpus ran. Its prediction (a robust parser reading the
answer wherever it unambiguously is) is **UNTESTED** — this corpus ran without the fix, and in
any case `answer_label_absent` read 0 here, so the corpus carries no window the fix would have
changed.

### Carried UNTESTED docket

| item | verdict | evidence |
|---|---|---|
| BS F2 Tribute second edict | **UNTESTED** | Tribute to Hunger resolves 10 times in the corpus, but no second-edict window was recorded; not re-derived |
| BS F3 | retired | — |
| BS F5 blockers ceiling | **UNTESTED** | max 4 attackers x 5 blockers; the ceiling path never ran |
| BS F6 distinct-index re-ask | **UNTESTED** | `identical_option_asks_resolved` 7, `repeat_annotated_takes` 2; no distinct-index re-ask observed |
| CF F1 pending-deadlock | **PASS (no occurrence, state shown absent)** | deadline_pct max 47.6%, 0 windows >= 80%, 0 wall-misses, 0 HUNG, 21/21 games ended naturally |
| CF F2 | **UNTESTED** | not observable from the record fields this corpus carries |
| CF F5 non-lifelink | **UNTESTED** | every lifelink window in the corpus was a lifelink one (`...deck152` seq 15, `...deck126` seq 9/25) |
| CF F8 haste | **UNTESTED** | haste attackers appear (Rorix, Bloodline Keeper) but no haste-specific clause was under test |
| CF F9 | **UNTESTED** | — |
| N14a / N14d / O20 reasoning-body halves | **UNTESTED** | I did not read the 2,203 reasoning bodies; only the reply/prompt channels were audited |
| `{X}`-announcement payment loss | **CONFIRMED, STILL LIVE** | 2 events, both deck125: 5 mana and 4 mana burned (see 4/HIGH-1) |

---

## 4. Engine / render / narration defects

### HIGH-1 — an announced X payment is still burned when the seat takes the X menu's decline row

Two events this corpus, both on deck125 seats, both Sphinx's Revelation:

```
AIPlayerGPT: cast of Sphinx's Revelation dropped after its X was announced (the model took the
X menu's own decline row - 5 mana already paid stays floating and is lost when the step ends);
announcement cancelled, card stays in hand
```
(`game-152v125-1789189687.stderr`; the second is 4 mana in `game-125v126-1789189709.stderr`.)

The seat's own prompt then confirms the loss to it:
`1789189690-...deck125` seq 11 — `- Sphinx's Revelation was NOT cast: you declined after the
payment above, so it is back in your hand and the 4 mana that paid for it is still floating,
unspent`. The narration is honest; the **engine still lets an X menu offer a decline row after
the payment is committed**, and deck125 (1/6 this wave, 3/6 last) paid 9 mana across two games
for a card it did not cast. This has been on the docket as untested since before wave 78; it is
now confirmed live with two reproductions. Either the decline row should not be on a
post-payment X menu, or the payment should be rolled back with the announcement.

### HIGH-2 — `hold_verdict_safer_ignored` and `hold_reopened_new_threat` are unauditable

The clamp at `AIPlayerGPT.cpp:35476` increments with **no DebugTrace and no record field**, and
the three markers it arbitrates (`[crack-back verdict:`, `[stack death verdict:`,
`[own loop verdict:`) are latch keys — the first two render **0 times in all 2,203 prompts**.
Consequence: a reviewer asked to "sample 15 and say whether the ignored safer verdict was truly
safer" has nothing to sample, and DC F2's `hold_reopened_new_threat = 0` cannot be classified
FAIL-or-UNTESTED from evidence — it has to be called UNTESTED by default, which is exactly the
outcome the LESSON OF WAVE 78 was written to prevent. This wave, 59 clamp events and a re-open
rate that fell 194 -> 85 are invisible at the window level. The minimum fix is a record field
carrying the held marker, the live marker and both danger ranks on any window where either
branch runs. Repro: any of the 42 seat logs — grep for a verdict marker and find none.

### MED-1 — `(...more)` cuts mid-clause

2 of 34 distinct `(...more)` glosses end inside a clause, which is the exact shape T11 set out
to remove:
- `1789189688-ai_baka_deck146-0x55db6e5d7460-vs-ai_baka_deck152.jsonl` seq 22 —
  `… {3}{B}: Until end of turn, Hive of the Eye Tyrant becomes a 3/3 black Beholder creature with menace and (...more)`
- `1789189711-ai_baka_deck146-0x55ab3761fd90-...` seq 54 —
  `… -3: Create two 2/1 black Spider creature tokens with menace and reach. -- -8: You get an emblem with (...more)`

Both leave a dangling conjunction/preposition, so the model reads "with menace and …" with no
idea whether the missing half is another keyword or a whole ability. The fallback should back
up to the last `--` or sentence boundary, not the last token that fits.

### MED-2 — `own_loop_verdict_lines_rendered` counts writes, not lines

63 increments, 56 rendered lines. The 7 extra are windows whose record field reads
`[own loop verdict: none]` — the field was written, no line was rendered, the counter moved.
Repro: `1789189698-ai_baka_deck126-0x560f527d3300-vs-ai_baka_deck123.jsonl` seqs 20, 30, 32,
44, 61 (plus `...deck126` seq 30 in `1789189686-…` and seq 23 in `1789189713-…`). The counter's
own name promises the invariant DC F10 tried to verify, and it does not hold. Gate the
increment on a non-`none` verdict.

### MED-3 — three counters exceed their rendered population by a fixed small margin

Same family, three sites: `own_loop_verdict_lines_rendered` 63 vs 56 prompts (above),
`crossphase_identical_reputs` 172 vs 170 rendered notes, `stack_drain_windows_asked` 30 vs 18
rendered clauses. In each case the counter is incremented on a path that does not always reach
a rendered prompt, so a reviewer comparing the census to the corpus finds an unexplained gap
and has to go to the source to decide whether it is a bug. The drain gap (30 vs 18) is the
largest and was **not** run down — I did not determine whether the 12 uncounted windows render
a different clause, are suppressed before render, or are a miscount.

### MED-4 — the byte problem moved to deck146

deck146 is now the heaviest deck by a wide margin (mean 15,019 B against a corpus mean of
12,034; 97 prompts over 20 KB, 22.7% of its windows, against deck125's 11.8%). It also carries
45 of the corpus's 99 `plan_line_missing` and 42 of its 87 `off_protocol_bytes>0` — the two
travel together per deck almost one-for-one. Whether the bytes cause the protocol drift or
both follow from deck146's board complexity is not settled by this corpus, but deck146 is where
the next byte pass should look.

### LOW-1 — the stderr game-end summary does not name the cache-replay population

Every game-end line prints `repeated identical asks re-served from the seat's own answer: 0`,
which is true of the repeat latch and reads as "no answers were re-served" — while 1,084 ask
answers were re-served from the cache in the same games. A reader working from stderr alone
would conclude the corpus made 2,203 fresh decisions over 2,203 windows. Add the cache figure
to the same line.

### LOW-2 — three byte-identical menus were still put twice

`1789189698-...deck123` seq 277->280 (t37 M1) and `1789189711-...deck123` seq 98->100 /
102->104 (t19), all the same fetchland crack menu on the **priority** seam, all answered `pass`
both times. In each a legal continuation had genuinely moved, so the ask is defensible — but
the rows the model reads are byte-for-byte the ones it just declined, with nothing in the
prompt telling it what changed. A one-line note ("a legal continuation moved since the last
window at this seam: …") would make the re-ask legible instead of looking like a loop.

---

## 5. RETIRED

No lategame specimen is written (owner ruling 2026-09-12: "Stop producing these.").

---

## What I did NOT check

- **Reasoning bodies.** I read no `reasoning` field. Every N14a/N14d/O20 reasoning-body half
  stays UNTESTED, and any claim about what the model *thought* is outside this report.
- **Pre-trim prompts.** The corpus stores only what was sent, so a narration trim that removed
  a whole section leaves no trace. My "no PLAN/stop/loop-state was lost" statement covers only
  what is checkable: the presence of those sections in the prompts as sent.
- **`ON THE STACK` vs `DRAW FORECAST` reconciliation (DA T16a).** 360 + 300 records; I did not
  compare the two post-stack life figures window-by-window. Marked UNTESTED, not PASS.
- **The DP's trample/menace/12x12 arms.** Never exercised by the corpus; I verified the DP only
  on the boards it actually saw (including one correct first-strike-deathtouch result).
- **The pay-to-hoist model (DB T10d).** I confirmed the hoist header renders 136 times; I did
  not re-run DB's byte model to confirm the residual is 0.
- **`stack_drain_windows_asked` 30 vs 18 rendered.** Gap observed, not run down.
- **Per-deck deciding decisions for deck123 (0/6) and deck125 (1/6).** That is the per-deck
  seats' work; I looked only for wave-79 render changes in their games and report the one I
  found (HIGH-1, deck125).
- **Replay aliasing driven by life totals.** Out of the continuation digest's stated scope and
  not detectable from these records.
