# Wave-54 seat review — ENGINE / NARRATION

Corpus `matchups-20260903-034355` (binary = hermetic `make -B` of master **caf023dbc**; engine lanes
A e21f81de5, B 351ad0098, C a2bc290d0, D 6c1555bb1, E 5fe998414, F 3329c40f1; **wave-53 lane AA
37b7951d3 — first corpus exposure**; audit lanes G-N; owner O1/O2 1afe6c8f6). Model qwen36-35b-a3b on
Spark :8084. **42 seat logs, 2,306 records** = 42 `gamestart` + 42 `gameend` + 10 `recovery` +
**2,212 decisions** (ask 1,683 / priority 332 / attackers 99 / blockers 36 / discard 34 / reveal 20 /
bottom 8). **21/21 natural — D1 discharged**, 0 timeouts, 0 crashes, 0 hangs, the lane-F no-progress
supervisor never fired. Max seat 312 (`deck123-vs-deck125`). Max prompt **32,215**
(`deck146-vs-deck126` seq 86, priority, turn 31). Total consumed inference **14.67 h** over 2,211
timed decisions.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL
(`prompt`, `options_text`, `reply`, `chosen_text`, `events`) and `/usr/bin/grep` over the `.stderr`
files. **`choice` is 1-BASED against `options_text`** (verified: `options_text[choice-1] ==
chosen_text` on 1,803 of the 1,813 single-select records that have both; the 10 "exact" hits are
1-row menus, and the 343 non-matches are all multi-select kinds whose `chosen_text` is a comma list).
On `priority` menus index **0** is the virtual `pass` row, which is NOT in `options_text`. Card facts
checked against `projects/mtg/bin/Res/sets/primitives/*.txt`: Devour Flesh, Pelakka Predation,
Silverquill Silencer, Intruder Alarm, Silverquill Command. None contradicted a render except where
called out (D-2). Read-only throughout: no build, no run, no git, no edit under `bin/Res` or `src/`.

---

## HIGH #1 — `dropping stale async answer` 488 (wave 53: 63, **+675 %**). Pinned: lane AA's stall floor is denominated in GAME seconds, and `WAGIC_FASTCLOCK` makes 1,200 of them cost ~12 s of wall clock.

### The mechanism, in code

`AIPlayerGPT::pollCompletion` (`src/AIPlayerGPT.cpp:9042-9214`) drops an answer when the worker has
finished (`status == 2`) but `mAsyncState->prompt != userMsg` — i.e. **the prompt was rebuilt
differently while the request was in flight**. The drop line is `AIPlayerGPT.cpp:9214`.

What rebuilds the prompt is the phase advancing, and what advances the phase under an in-flight ask is
**wave-53 lane AA's stall floor**, `ActionStack::Update` (`src/ActionStack.cpp:1386-1470`):

```
const bool spent = loading ? (mHoldTicks >= 12)
                           : (mHoldTicks >= 300 && mHoldSeconds >= (holder->isInteractiveAI() ? 1200.0f : 20.0f));
…
cancelInterruptOffer(DONT_INTERRUPT, false);
```

`mHoldSeconds += dt`. Under the corpus recipe `WAGIC_FASTCLOCK=0.1` every tick feeds a **fixed,
wall-decoupled dt of 0.1 s** (`JGE/src/SDLmain.cpp:110-131`: "decouple game time from wall time…
every tick feeds a FIXED dt (default 0.1 s) and the event wait shrinks 10 ms -> 1 ms"). So the
interactive-AI budget of 1,200 *game* seconds is **exactly 12,000 ticks**, and every single release
in this corpus reads `for 12002 ticks` — 936/936 lines, one distinct value. Twelve thousand ticks of a
headless pump is seconds of wall clock; the mean model latency this corpus is **23.9 s**. The floor
therefore fires on essentially every GPT interrupt window before its answer can land.

The lane's own comment states the assumption that fails: *"An INTERACTIVE AI … legitimately holds a
window for as long as its model takes … so its budget sits past the whole request timeout."* That is
true in wall time (1,200 s > the 900 s `WAGIC_GPT_TIMEOUT`) and false in dt time. The second half of
the same comment names the other half of the bug: *"once it has TAKEN the window
`extendInterruptOffer` no longer reaches it"* — and indeed `AIPlayerGPT::decisionPending`
(`AIPlayerGPT.cpp:20659-20675`) calls `extendInterruptOffer(this)` on every tick a request is in
flight, but `ActionStack::extendInterruptOffer` (`include/ActionStack.h:236-248`) only resets
`mHoldTicks/mHoldSeconds` when `askIfWishesToInterrupt == who`. Once the seat has taken the window the
holder is `observer->isInterrupting`, the keep-alive silently no-ops, and **the seat's only "I am
still thinking" signal is disconnected from the watchdog that is about to kill it.**

### The evidence

| measurement | value |
|---|---|
| `dropping stale async answer` | **488** (wave 53: 63) |
| ` - releasing so the game advances` | **468** |
| drops within ±8 stderr lines of a release | **434 / 488 (89 %)** |
| distinct tick counts across all 468 releases | **1** (`12002`) |
| release holders | Player 1 **154**, Player 2 **314** |
| games with 0 releases | 4 (`152v130`, `152v162`, `146v152`, `146v123`) — drops there 0, 0, 1, 6 |
| games with ≥30 releases | 8 — drops 64, 59, 48, 44, 39, 37, 33, 30 |

Per-game the counts track 1:1 (`126v123` 64 releases / 64 drops; `162v130` 37/37; `125v162` 48/48;
`152v123` 15/15). The four zero-release games carry the wave-53 baseline rate.

**Repro, exact.** `game-126v123-1788425076.stderr:306-341`, turn 2, all on Player 2:

```
306 wagic: interrupt window held by Player 2 on 'NextGamePhase.  (Current phase is: Main phase 1)' for 12002 ticks (turn 2 phase 4) - releasing so the game advances
307 Resolving Action on stack: NextGamePhase.  (Current phase is: Main phase 1)
308 Action added to stack: NextGamePhase.  (Current phase is: Combat begins)
309 AIPlayerGPT: dropping stale async answer
310/311 … held by Player 2 on 'NextGamePhase.  (Current phase is: Combat begins)' for 12002 ticks (turn 2 phase 5) - releasing
```
…and so on through Attackers, Combat ends, Main 2, End, Cleanup. **Seven consecutive phase steps of
one turn, seven windows the seat was actively answering, seven answers thrown away, zero records
written.**

### Which seam, whose turn, and what it costs

The seam is the **opponent-turn / stack-response INTERRUPT window** — the only windows the floor
watches (`!holder->isAI()` bails out; a seat's own main-phase ask is not an interrupt offer). The
seat's own ask key did not change under it; **the engine advanced the phase while the window was
held**, and the rebuilt prompt no longer matched.

The cost is not just wasted calls, it is **lost decisions**. In `126v123`, all 64 releases fall on
Player 2 = the deck126 seat, and that seat records **27 own-turn decisions and exactly 1
opponent-turn decision** across an 18-turn game, while its counterpart `deck123-vs-deck126` records
66 own-turn and **71** opponent-turn decisions. The releases are not re-asked; the window is gone
(`cancelInterruptOffer(DONT_INTERRUPT, false)`).

Corpus-wide: **24 of 42 seats recorded ≤3 opponent-turn decisions and 16 recorded zero.** I do NOT
claim the floor caused all of that — the per-game correlation runs the other way at the aggregate
(games with ≥15 releases average 1.64 opponent-turn decisions per turn, games with ≤2 releases
average 0.20), because a release can only happen where a window was offered in the first place. The
defensible claim is the per-seat one, and it is stark: **where the floor fires it fires on one seat,
and that seat's opponent-turn surface goes to ~zero.**

Inference cost: the 488 drops are model calls whose latency never entered the 14.67 h (only consumed
answers are timed). At the corpus mean of 23.9 s that is **~3.2 h of additional inference bought and
discarded — 22 % on top of the recorded total.**

### It also poisons every lane-A number in this corpus

The HOLD row is offered on the opponent's turn only (`AIPlayerGPT.cpp:18305-18311` and
`20391-20397`: `if (observer->currentPlayer != this)`). Wave 53 rendered it 1,173 times; wave 54
renders it **426**. Lane A's headline "43 takes = 10.1 % vs 29 = 2.5 %" is measured against a
denominator the stall floor cut by 64 %. The take **count** is up (29 → 43) and that is real; the
**rate** is not a clean comparison and should not be carried forward as one.

### Lane and flag

**Lane AA (37b7951d3), `ActionStack.cpp` stall floor.** It is the only wave-54-corpus change that
touches this seam, it is the only one whose signature (`12002 ticks`, `NextGamePhase`, per-seat
clustering) matches, and it is the only one absent from wave 53's binary. The other candidates are
refuted: lane A's hold keys never fire on a released window (a hold take writes a record; these
windows have none), lane K's Baka `Act` change is heuristic-seat only (Baka executed 2 decisions all
corpus), lane M's ask cache emits nothing in stderr and the 4 zero-release games carry the wave-53
drop rate with lane M active, and lane L's board index would change the prompt on every tick, not
only after `Action added to stack`.

**AA ships no disable flag** — a silent-instrument-rule violation in its own right, and the reason
this cannot be A/B'd from the flag table. The A/B that needs no rebuild is the clock:

> **Flip `WAGIC_FASTCLOCK` first.** Re-run one high-drop game (`126v123`) with `--realtime`
> (`FASTCLOCK=0`, `selfplay-harness.sh:79/94`). Prediction: `dropping stale async answer` falls to
> the wave-53 baseline (≤3 for that game) and `releasing so the game advances` to 0, with no other
> change. If it does not, the floor is exonerated and lane M/L are next.

Fix shape (wave-55 docket): (a) denominate the interactive-AI budget in **wall clock**
(`steady_clock` on `mHoldOn` acquisition), not `dt` — dt is a synthetic counter by design and no
watchdog should ride it; (b) make `extendInterruptOffer` reset the counters on the
`observer->isInterrupting == who` branch as well as the `askIfWishesToInterrupt == who` branch, so a
seat with a request in flight is progress in both states; (c) ship `WAGIC_W53AA_LEGACY=1`.

---

## HIGH #2 — the HOLD row's own-turn exclusion leaves the corpus's longest dead run uncovered: 50 byte-identical windows in ONE Blockers step, 242.8 s of inference, no HOLD row on the menu.

**Repro: `deck126-vs-deck125` seq 72 → 121, turn 28, phase Blockers, 2-row menu, 50 consecutive
decisions.** The prompt on the last one reads `[you declined this exact list **49** times already
this turn]`. Every reply is `CHOICE: 2 (Cast nothing right now)`. Latency sum across the run:
**242.8 s**. The menu is `1. Cast Tribute to Hunger … {right now: they control 0 creatures - at 0
this does nothing} / 2. Cast nothing right now` — and **the HOLD row is not on it**, because
`observer->currentPlayer == this` (the seat had declared its own attack that turn).

The lane's rationale for the exclusion is stated at `AIPlayerGPT.cpp:18296-18304`: *"on the seat's OWN
turn the board changes with every land drop and cast, so a hold there would be taken and retired in
the same breath."* This run refutes it directly: after the seat's main phase is spent, its own
Blockers / Combat-damage sequence produces 50 byte-identical windows with no board change at all. The
hold's own key (`holdBoardKeyOf`, `AIPlayerGPT.cpp:13849+`) already guarantees retirement on any real
change, so nothing about the guarantee needs weakening to extend the offer.

Distribution of the runs (wave-53 method, key = `(turn, options_text tuple)` on a declined
`chosen_text`, runs of length ≥3): **356 of 2,212 decisions = 16.1 %** (wave 53: 26.2 %; target
< 15 % **NOT met**). 45 runs, max length **50**. Including the new HOLD-row text in the declined
predicate: 396 = 17.9 %. **I could not reproduce the harvest's 12.2 %** with either predicate and
report my own number with its method stated; the discrepancy is worth one line in synthesis.

Second-largest runs, all `Upkeep`, all 3-row menus, all in the two long games: `deck123-vs-deck125`
seq 284-297 (14), 256-268 (13), 198-209 (12), 241-251 (11). Those DO carry the HOLD row and the
pilot did take it repeatedly (17 takes in that seat) — the mechanism works where it is offered.

**Ask:** offer the HOLD row on own-turn windows once the seat's land drop is spent and it is past
Main 1, or simply offer it always. Falsifier: if the row is offered on own-turn windows and the 50-run
class does not shrink, the row is not the remedy.

---

## HIGH #3 — `all_assignments_illegal` declares NO blocks at a lethal window and does not re-ask. New fallback class, first firing, cost the game.

**Repro: `deck152-vs-deck146` seq 58, turn 19, Blockers, my_life 6 / opp_life 7, latency 2,483 ms.**

The render was correct and emphatic. The header: `Your life: 6. Unblocked, these attackers deal up to
6 - you would be at 0 - LETHAL if it all connects (at 0 life you LOSE - 0 is not survival): block
enough to survive.` Both Spider attackers carry `[menace - only a block by TWO OR MORE of your
creatures counts; one creature alone does not block it at all]`, and **both** blocker rows repeat it
per-attacker: `A3 (no 1-on-1 result exists - only a block by TWO OR MORE of your creatures counts;
this one alone does not block it at all)`. The model replied `BLOCKS: B1:A3, B2:A4` — one blocker on
each menace attacker.

The engine pruned both (`pruned_pairs: "Elite Spellbinder -> Spider (needs 2 blockers, only 1
assigned); Intrepid Adversary -> Spider (needs 2 blockers, only 1 assigned)"`), stamped
`fallback: all_assignments_illegal`, and **declared no blocks at all**. The seat took 6 at 6 life and
lost the game (`results.tsv`: `146 152 → winner 0`, life1 −11).

This is a legality-enforcement seam behaving exactly opposite to the loop's stated purpose. Compare
`named_row_reask`, which re-asks once when the model names an off-menu row: 7 firings this corpus,
5 recovered. The blockers path has no equivalent arm. A window where **every** assignment was pruned
is precisely the window that should be re-asked with the rejection quoted back
(`rejectedSoFar` already exists on the cast path, `AIPlayerGPT.cpp:20374`).

**Ask:** give `all_assignments_illegal` a one-shot re-ask carrying the pruned pairs as text, exactly
as `named_row_reask` carries the rejected name. Falsifier for wave 55: `all_assignments_illegal`
records with no follower re-ask **0/N**.

Not an engine defect on the same record: the model was told three times, in three places, that a lone
blocker does not block menace. That half is a deck-seat / skill item, and it is the strongest
"the render was right and the pilot did not read it" datum in the corpus.

---

## HIGH #4 — the reveal ask's prompt is still unstable on an unchanged board: 6 consecutive stale drops → the heuristic, and lane F's force-close does not cover the shape.

**Repro: `deck146-vs-deck123` seq 15, turn 9, Main phase 1, kind `reveal`, 4 rows
(`Lightning Greaves / Marsh Flats / Thraben Doomsayer / Damnation`), `choice: -1`, `latency_ms: -1`,
`fallback: stale_livelock`, empty reply.** stderr: `game-146v123-1788425054.stderr:1335-1341` — six
`AIPlayerGPT: dropping stale async answer` lines back to back with **no stack action and no release
between them**, then `AIPlayerGPT: 6 consecutive stale drops - giving this decision to the
heuristic`. This is the ONE handoff in the corpus and it is a different class from HIGH #1: the
breaker only counts drops on the **same** `serializeGameState()`, so the board did not move — the
prompt did.

The card (primitive, verified): `name=Pelakka Predation` …
`auto=name(reveal hand) reveal:type:*:opponenthand revealzone(opponenthand) optionone name(Choose a
card) target(*[manacost>=3]|reveal) reject optiononeend optiontwo name(put back) all(*|reveal)
moveto(hand) optiontwoend revealend`. Option one is **predicate-gated** (`manacost>=3`) — the same
family as wave-53's HIGH #1 hang — and here it had live targets (Thraben Doomsayer mv 3, Damnation
mv 4). The recovery record (seq 16) shows what the heuristic did instead:
`- You used: choose a card with Pelakka Predation / - Opponent put the revealed Lightning Greaves
into their hand / …` — **option TWO ran; nothing was discarded; the Predation did nothing.**

Lane F's new guard does not reach this: its fixture is *"option one predicate-gated to zero targets
with NO option two"*, and Pelakka Predation has an option two. `reveal force-close` renders **0**
times corpus-wide, so the force-close is **UNTESTED (N = 0)**, not PASS.

Also on this path: `game-146v123-1788425054.stderr:1250-1334` is 85 lines of the reveal display
cycling `AttemptNew failed to load (404) → Destroying WCachedResource → Can't locate` over the same
four card images. Corpus-wide there are **1,927** 404 lines from **118 distinct** resources
(`mana.wav` alone 105 times). Lane J's resource-miss memo is not covering this loop — set-C watch
item, flag `WAGIC_RENDER_NOCACHE=1` for the A/B.

**Ask:** (a) extend the force-close budget to any interactive reveal whose driver makes no progress,
not only the no-option-two shape; (b) find the per-tick instability in the reveal menu build (the
wave-52 "deterministic-order fix" evidently does not cover the `revealzone(opponenthand)` path);
(c) first flag to flip is `WAGIC_GPT_AUDIT_M_OFF=1` (lane M's situation prefill is the only wave-54
change that rewrites prompt text per tick), then `WAGIC_GPT_BOARDINDEX=0`.

---

## Health — the 23 fallbacks, each classified

`fallback` **23 / 2,212 = 1.04 %** (wave 53: 15 / 3,483 = 0.43 %). The rise is entirely
`plan_choice_conflict` (8 → 12) and `named_row_reask` (3 → 7), both concentrated in one seat.
**Baka actually executed 2 decisions** (`chose -1 of N` in stderr, `game-125v123-1788425036.stderr:1655`
and `:2121`) = **0.09 %** of decisions — under the ≤0.20 % carry (wave 53: 0.11 %).
`giving this decision to the heuristic` **1**. NO `empty_reply`. NO `timeout` class.

| class | n | root cause | Baka? | recovered |
|---|---|---|---|---|
| `plan_choice_conflict` | 12 | all 12 stamped `decision_reversed_in_prose`; the reply's header line takes a live row and the PLAN prose then reasons to a pass. 5 in `deck123-vs-deck126` (Thraben Doomsayer token counting), 2 in `deck123-vs-deck146`, 1 each in `deck126-vs-deck125`, `deck130-vs-deck123`, `deck130-vs-deck126`, `deck146-vs-deck126`, `deck152-vs-deck146`. **0 of the 12 executed the HOLD row** — lane A's D2b collision is closed | no (all `choice != -1`) | 6 `plan_choice_conflict_recovered`, 6 `plan_choice_conflict_exhausted`; no `recovery` record (correct — `choice != -1`) |
| `named_row_reask` | 7 | in-range index + off-menu name, **all 7 in `deck123-vs-deck125`** (seq 37, 56, 100, 105, 150 and two more), every one naming `Cast Bloodline Keeper` or `Cast Vision Skeins` from a stale plan. 5 stamped `stale_echo_in_range`, 2 `named_row_not_offered` | no | 5 `named_row_reask_recovered`, **2 `named_row_reask_exhausted`** (seq 58, 107) → those two are the 2 Baka executions |
| `named_row_not_offered` | 2 | seq 58 and 107 of the same seat, the exhausted tail of the above | **yes ×2** | `recovery` seq 59 shows Baka cast Lightning Greaves; seq 108's `recovered_by` is empty |
| `stale_livelock` | 1 | HIGH #4 | **yes** | `recovery` seq 16, option two ran |
| `all_assignments_illegal` | 1 | HIGH #3 | no (`choice: 2`) | none — and that is the defect |

**Lane Q's D24 recovery rule: PASS 10/10.** Exactly 10 records carry `choice: -1` **with** a
`fallback`, and exactly 10 `recovery` records exist, each at `seq+1` with `recovers_seq` pointing
back and carrying `recovers_kind` / `recovers_fallback`. The 12 `plan_choice_conflict` and the 1
`all_assignments_illegal` have `choice != -1` and correctly get none.

**Lane Q's timeout arm FIRED — first live exercise (wave 53: UNTESTED, N = 0).**
`AIPlayerGPT: no reply after 900s - one retry` appears **twice**.
1. `game-125v123-1788425036.stderr:148` — `deck123-vs-deck125` **seq 1**, the opening mulligan.
   Record: `retry: 1`, `long_reply: 1`, `deadline_pct: 165.4`, `latency_ms: 1,488,661`. **It
   answered on the retry** (`Keep this hand`). Full PASS on the arm.
2. `game-146v123-1788425054.stderr:1607` — retried, and the window was then auto-passed
   (`AIPlayerGPT[ph7]: only display-toggle (Flip Side) options; auto-passing without a model call`).
   **No decision record exists for it at all.** That is a 900 s wall miss with zero translog trace —
   an observability gap worth a field (`wall_miss_unrecorded`) rather than a fix.

`parse_note` 61: `decision_reversed_in_prose` 18, **`pass_hold_ambiguous` 16**,
`named_row_not_offered` 6, `plan_choice_conflict_exhausted` 6, `plan_choice_conflict_recovered` 6,
`stale_echo_in_range` 5, `named_row_reask_recovered` 5, `named_row_reask_exhausted` 2,
**`latched_row_mismatch` 2**, `blocks_last_line_taken` 2, `long_reply` 1, `echo_index_conflict` 1,
`name_over_index` 1, `repeat_count_under_two` 1, `multiblock_first_wins` 1. `hold_row_named` **0**,
`plan_contradicts_noop_row` **0**.

stderr: `auto-passing without a model call` 2,061, `all actions pass-declined` 474, `land-drop ask NOT
issued` 892, `cast ask NOT issued` 319, `repeat plan iteration` 89, `combat decision suppressed`
397 (no-legal-attacker 334, unresolved-stack 63), `hold re-opened` 21, `with no source tapped` **0**,
`index_name_conflict` **0**, `plan_missing` **0**, `http_error` **0**, `engine_answered` **0**,
`force-close` **0**.

### `pass_hold_ambiguous` — the docket item, corrected

The harvest note says all 16 are a plain `CHOICE: 0 (pass)` executing row 0 and calls the stamp a
false positive. **13 of 16 are; 3 are not.** Read off the records:

| shape | n | executed | verdict |
|---|---|---|---|
| `CHOICE: 0 (pass)` on a menu whose last row is the HOLD row | **13** | row 0 (the virtual pass) — correct every time | **false positive of the reword.** `pass` is an EXACT match for row 0's own label and merely a case-insensitive PREFIX of `Pass priority, and do not ask me again…`; the stamp is prefix-matching without an exact-match tiebreak. `deck152-vs-deck125` 29/30/45/46, `deck125-vs-deck162` 21/23, `deck125-vs-deck146` 20/21, `deck146-vs-deck126` 55, `deck123-vs-deck126` 72/74/101/110 |
| `CHOICE: 3 (Pass priority)` on a 3-row menu whose row 3 IS the HOLD row | **3** | row 3 = the HOLD row | **TRUE positive.** `deck146-vs-deck126` seq **54, 56, 57** (turn 24, Upkeep / Main 1 / Combat ends). `Pass priority` is a genuine prefix of BOTH labels; the engine broke the tie on the index, which is the right call, and all three are counted in the 43 HOLD takes |

**Ask (cheap, no reword):** an EXACT match against a row label wins over a prefix match — that kills
13 of 16 stamps with no behaviour change. The residual 3 are a real ambiguity created by leading the
HOLD row with the same verb as the pass row; if the owner wants them gone too it needs a reword, and
that is lane A's call, not mine.

### `latched_row_mismatch` — both firings adjudicated, both correct

Lane B's prediction is *"a stamp on a legitimate name-over-index remap is a FAIL"*. Neither is one.

1. `deck123-vs-deck125` seq **147** (turn 36, Main 1, 4 rows). Coded lines: `CHOICE: 3 (Cast Intruder
   Alarm)` then, after 400 words of prose, `CHOICE: 2 (Cast Bloodline Keeper)`. Bloodline Keeper is
   **not on the menu** (rows are Damnation / Tribute to Hunger / Intruder Alarm / Cast nothing).
   `coded_answers: 2`, `latched_coded_line: 1`, executed row 3 = Intruder Alarm — the first line,
   whose index and name agree. The last line's index (2) and name both differ from the executed row,
   which is the stamp's definition. Correct: this is the D13 latch refusing an off-menu last line,
   and it is co-stamped `stale_echo_in_range;named_row_not_offered`.
2. `deck146-vs-deck152` seq **63** (turn 21, Main 2, 5 rows). Header `CHOICE: 3 (Cast Silverquill
   Command)`; the prose then reasons to lethal-by-combat and ends `So, CHOICE: 5 (Cast nothing right
   now).` Executed row 5. The header's index and name both differ from the executed row → stamp.
   Correct, and the executed row matches the model's final intent.

**Verdict: PASS 2/2, 0 stamps on a legitimate name-over-index remap.**

### Reply-vs-executed-row audit

**1,828** replies carried a `CHOICE: n (name)` parenthetical on the last coded line; **25** disagree
with the executed row's label = **1.37 / 1,000**. Classified: 23 are cosmetic short-forms the row
label contains in longer form (`Animate Lair of the Hydra` vs `becomes a 1/1 hydra with Lair of the
Hydra`; `Level Ranger Class` vs `Put a level counter with Ranger Class`; `Swamp - "B"` vs
`Swamp [land] [your library] - "B"`; the `Create human with Thraben Doomsayer xN` repeat rows vs
`Create human with Thraben Doomsayer, repeated N times, then …`); 1 is the seq-147 latch above; 1 is
seq 63. **Genuine mis-executions: 0** (wave 53: 2). The wave-53 carry ceiling of ≤1/3,000 is met with
room.

### Mulligans (new-baseline record)

**34 of 42 seats kept 7.** 7 seats mulliganed once, 1 seat twice (`deck152-vs-deck123`, bottomed
`Barkchannel Pathway, Tovolar's Huntmaster`). Eight `bottom` records, all the model's — no
Baka-executed opening keep, no mull-to-zero, and **no repeated mulligan against a guide floor**, so
nothing here is an OWNER QUESTION. Bottomed cards: `Chromatic Lantern`, `Master of the Feast`,
`Island`, `Vision Skeins`, `Peer into the Abyss`, `Teferi, Who Slows the Sunset`, `Tragic Slip`,
plus the two above. **deck130 shipped 0 mulligans again** — its mulligan section stays
**#132-UNTESTED (four corpora)**.

---

## VERDICT TABLE — Set B, adjudicated by the emitter's actual strings

Three-number audit (#188) is given as **old renders / new renders / takes** in the count column for
every literal lanes A-E changed.

| Lane | Prediction | Verdict | Count / citation |
|---|---|---|---|
| **A** (D2 a-d, D12) | `CHOICE: 0 (Hold priority)`-shape echoes execute the hold row or are stamped N/N (was 0/2) | **PASS (16/16 stamped)** | The old string `Hold priority - do not ask me again…` renders **0**; the shape now arrives as `CHOICE: 0 (pass)` / `CHOICE: 3 (Pass priority)` and all 16 carry `pass_hold_ambiguous`. `hold_row_named` renders 0 |
| A | HOLD takes tripping `plan_choice_conflict` **0** (was 5) | **PASS (0/12)** | All 12 `plan_choice_conflict` records' executed rows checked against `options_text[choice-1]`; none is the HOLD row |
| A | `hold_windows_skipped > 0` on every gameend where the row was taken N/N (was 2/6) | **PASS 7/8, the 8th explained** | 8 seats took the row; 7 have `hold_windows_skipped > 0` (31, 16, 9, 5, 5, 3, 3; sum **72**). The exception is `deck123-vs-deck130`, whose single take is seq 17 at **Cleanup** — the last phase of the turn, so there was nothing left to skip. No seat has skips without a take |
| A | HOLD takes in declined-note N≥3 windows **≥ 20 %** (was 0.4 %) | **FAIL (22 / 212 = 10.4 %)** | Big improvement (0.4 % → 10.4 %) but half the target. The row is still last on the menu and still competes with `Cast nothing right now` directly above it |
| A | identical-declined runs **< 15 %** (was 26.2 %) | **FAIL (16.1 %)** | 356 / 2,212 by the wave-53 method (key = `(turn, options_text)`, run ≥3); 396 = 17.9 % if the new HOLD text counts as a decline. 45 runs, max **50** (HIGH #2). **I could not reproduce the harvest's 12.2 %** with either predicate |
| A | opponent-turn casts **≥ 90 %** of 58 | **FAIL (26 = 44.8 %)** | 403 opponent-turn casting windows (wave 53: 1,052), 26 casts. By phase: Main 1 22, Combat ends 1, Main 2 1, Combat begins 1, Attackers 1. **Zero casts at Upkeep (65 windows), Draw (31), End (26), Cleanup (25), Combat damage (31), Blockers (4) — six phases, 182 windows, no cast.** The window collapse is HIGH #1's doing and the prediction's denominator is not comparable |
| A | row text is the new wording | **PASS (426/426)** | `Pass priority, and do not ask me again this turn unless the board changes (any change re-opens this window; you give up no cast)`. **0 / 426 / 43.** LAST row in 426/426. By kind: ask 403, priority 23 |
| A | served PLAN blocks > one paragraph **0** | **PASS (0 / 2,036)** | Every served block is single-paragraph; every one carries the age stamp `, N window(s) ago on turn N` (2,036/2,036). Max `plan_echo_count` **37** (wave 53: 43) |
| A | stale-plan decisions with no `"<card>" is no longer on your menu` clause **0** (was 2) | **PASS on renders (357)** | The clause renders in 357 prompts. I cannot enumerate the ground-truth set of stale-plan decisions from the log, so this is a render count, not a completeness proof — stated honestly |
| A | WATCH: `pass_hold_ambiguous` has no field evidence | **16 firings, 13 false / 3 true** | See above. Docketed |
| **B** (D13/D14/D15/D9) | `latched_row_mismatch` on every record whose index AND parenthetical both differ from the executed row; a stamp on a legitimate name-over-index remap is a FAIL | **PASS (2/2, 0 FAILs)** | Both adjudicated above. `name_over_index` fires separately once and is not co-stamped |
| B | `plan_contradicts_noop_row` on no-op-row + contrary-PLAN records N/N (was 0/4) | **UNTESTED (0 renders)** | The note renders 0 times. 12 `decision_reversed_in_prose` records take a LIVE row, not a no-op row, so no window arose. Old 0 / new 0 / takes 0 |
| B | replies naming a card that occurs only inside an annotation and executing a row **0** (was 1) | **PASS (0)** | The 25 parenthetical disagreements are 23 cosmetic + the 2 latch cases; none names an annotation-only card |
| B | answered replies at ≥95 % of the deadline without `long_reply`/`deadline_pct` **0** (was 1 of 6) | **PASS (0/1)** | Exactly one reply passed 95 %: `deck123-vs-deck125` seq 1 at **165.4 %**, and it carries `long_reply: 1`, `deadline_pct: 165.4` and `retry: 1` |
| B | watch `stale_echo_in_range` / `named_row_not_offered` for a rise on legitimate names | **RISE, and it is one seat** | `stale_echo_in_range` 1 → **5**, `named_row_not_offered` 4 → **6**. 10 of the 11 are `deck123-vs-deck125`, every one an off-menu `Cast Bloodline Keeper` / `Cast Vision Skeins` echoed from a stale plan. Not a parser regression — a deck-seat plan-staleness signature |
| **C** (D4/D10/D11/D18/D5) | rows whose enumeration includes a player and whose `{kills …}` summary does not say so **0** (was 26) | **PASS (0 / 586)** | 586 rows enumerate a player in `legal targets right now:`; none carries a `{kills…}`/`{removes…}` summary that omits the player |
| C | `[DRAW PRICE:` rows of either form with no `- you would be at K` **0** (was 58/85) | **PASS (43/43)** | **0 / 43 / 5.** Every DRAW PRICE row carries the life tail |
| C | the lethal tail must stay 16/16 (Forced Fruition) | **UNTESTED — 0 Forced Fruition windows** | `this KILLS you` renders **9** times and 0 are Forced Fruition: Barrowin of Clan Undurr 2, Acererak the Archlich 2, Nadaar Selfless Paladin 2, Silverquill Silencer 2, Emeria's Call 1. The tail works on the family that DID arise; the named card never appeared |
| C | casts from a `KILLS you` row **0** | **PASS (0/9)** | 0 / 9 / **0** |
| C | unconditional-removal rows with `{kills:}` / `{removes:}` N/N (was 0/244), none on an INDESTRUCTIBLE target | **PASS on renders (113); indestructible sub-claim UNTESTED** | `{kills: <names>}` **0 / 75 / 13**; `{removes: <names>}` **0 / 38 / 11**; the conditional form `{kills N of the …}` **0 / 12 / 0**. No `{kills:}`/`{removes:}` row landed on an indestructible target this corpus, but no indestructible removal target arose either, so the guard is unexercised |
| C | priced rows on a multi-priced menu with no fit clause **0** | **PASS (1,197/1,197)** | **0 / 1,203 / 218.** Forms, all counted: `{leaves N sources - no other row on this menu needs more than N}` 579, `{leaves N sources - row(s) … need more mana sources than the N this leaves}` 221, `{leaves N source - …}` 164, `{taps you out - …}` 197+. A first pass that only looked for `{leaves N sources` scored 392 misses; the emitter's actual strings include the singular and the taps-you-out form. D18's fit clause is a COUNT claim and reads as one |
| C | modal cast rows with `{modes live right now: …; Dead: …}` N/N (was 0/28) | **PASS (11/11 marked; 17 row-renders)** | **0 / 17 / 7.** Every cast row whose `{card text:}` says "Choose one/two" carries the clause, e.g. Silverquill Command's 9-mode list with `dead (no legal object right now): none` |
| C | CHOOSE_MODE sub-menus unmarked **0** | **FAIL (9/9) — the carried `chooseaname` item, confirmed** | The header renders 9 times, verbatim `Choose one mode for Silverquill Silencer:`, with no annotation. Primitive: `name=Silverquill Silencer / auto=chooseanameopp transforms((…)) forever chooseend nonland`, text *"As Silverquill Silencer enters, choose a nonland card name."* It is a **card-NAME menu, not a mode menu**, and the header names the wrong thing. Already carried to wave 55 by lane C; this corpus confirms it at 9/9 |
| C | REFUTED-on-disk D4 repro B | **holds** | Not re-litigated |
| **D** (D3/D6/D8/D25) | loss-framed asks whose destination is the battlefield/hand **0** (was 8/8) | **PASS (0/3)** | The new gain header renders **2** times, both Path to Exile, both `CHOOSE A CARD TO PUT ONTO THE BATTLEFIELD: Path to Exile's effect puts a card you choose from the list below onto YOUR battlefield - each option is a card you GAIN, NOT a card you lose … Pick the card you MOST NEED right now (with basic lands, the colour your hand and your board are shortest of).` (`deck162-vs-deck125` seq 26, `deck126-vs-deck125` seq 33). Old string `EXILE ONE OF YOUR OWN CARDS: your own effect (Path to Exile)` renders **0**. The one surviving `each option is a card YOU will LOSE` is a genuine forced sacrifice to the graveyard (`deck126-vs-deck146` seq 33, Silverquill Command's sac mode) and is correctly framed. **N is 2 this corpus vs 8 last — a thin but clean pass** |
| D | `your own effect` on a permanent the seat does not control **0** (was 18/26) | **PASS (0 renders)** | The literal `your own effect` renders **0** times corpus-wide. The opponent-driven branch now reads `FORCED sacrifice OF YOUR OWN CARD: the opponent's effect (<source>) forces YOU to sacrifice…` |
| D | renders repeating one ordinal-free row > 2× **0** | **PASS on ordinal-free (0); the ordinal-BEARING class is HIGH-cost** | Every repeat carries `#N`. But see D-3 below: 41 ask/priority records carry **425 redundant `#N` rows** (~153,591 prompt chars), led by 190 `Equip with Lightning Greaves targeting Human #N` and 93 `… Vampire #N` |
| D | de-duplicated-length-1 asks **0** (was 16) with `identical_option_asks_resolved > 0` on the gameend | **PASS (5 resolutions on 2 gameends)** | `identical_option_asks_resolved`: `deck146-vs-deck126` 4, `deck130-vs-deck125` 1. 110 length-1 `priority` menus exist and all are genuine single-ability windows with the virtual pass at index 0 |
| D | a land below a non-land in a forced-loss list **0** | **PASS (0)** | Checked every discard / bottom / AFFORD-TO-LOSE list |
| D | NOT SHIPPED: 2-D ability-menu collapse | **carried, and now the corpus's dominant mega-menu class** | See D-3 |
| **E** (D21/D17/D16/D20/D19) | `while ahead on LIFE` at a record where `my_life <= opp_life` **0** — the corpus's only render FALSEHOOD | **PASS (0 / 18)** | 18 renders, life margins +3, +16, +2, +2, +5, +2 … ; **0** at `my_life <= opp_life`. The gate works. **The corpus's render falsehood has moved, not vanished — see D-2** |
| E | attackers prompts without `They have N untapped creatures able to block …` **0** | **PASS (99/99)** | Every one of the 99 `attackers` records carries it, e.g. `They have 0 untapped creatures able to block: every attacker you declare this turn is unblocked unless something changes first.` |
| E | `would not cover any spell` renders without a cause clause **0** | **PASS (11/11)** | All 11 carry a parenthetical cause, in two forms: `(you have N lands; no spell in it is castable off {W}{B} alone)` and `(N lands are not enough for your cheapest spell at mana value M)` |
| E | colourless fetch rows **0** except lands whose colour flags read empty | **PASS (0)** | No land row renders an empty `- ""` colour string anywhere |
| E | `ability: X's X` name-twice stack lines **0** (was 53) | **PASS (0 / 401)** | 401 `ability:` stack-line occurrences over 51 distinct forms, all `ability: <source>'s <effect> [from your/their <source>] [triggered/activated ability]`. Wave-53's `Ob Nixilis, the Hate-Twisted's Ob Nixilis, the Hate-Twisted` is now `Ob Nixilis, the Hate-Twisted's **triggered ability**` (9 lines) — the duplicate is gone, the generic fallback label remains (LOW, D-6) |
| **F** (D7 a-c, D38) | records with `choice: -1` and no `fallback` **0** (was 1; class `engine_answered`) | **PASS (0 / 2,212)** | Every `choice: -1` record carries a fallback. `engine_answered` never needed to render; **0 / 0 / 0** |
| F | any game whose two seats write nothing for > 60 min is stopped with verdict `hang` (0 expected) | **PASS (0) — and the supervisor's silence is corroborated** | 21/21 natural, `corpus-log.txt` reports 0 HUNG. D1 discharged |
| F | reveal force-close after 20,000 no-progress ticks AND 600 s wall | **UNTESTED (N = 0)** | `force-close` renders 0 times. And HIGH #4 shows a reveal that DID stall and the force-close did not cover its shape (option two present). Not a PASS |
| F | SECOND reveal hang fixed; fixture `w54_reveal_stall_forceclose.txt` RED on base | **UNTESTED in corpus** | No zero-eligible-option-one-with-no-option-two reveal arose. The 4 Sigarda Coven `reveal:5` windows (`deck152-vs-deck126` seq 20/29/35/45) all found a Human, asked the model and resolved — wave-53's HIGH #1 does **not** recur |
| **AA** | 0 stall-floor declines on GPT seats; a decline within 1,200 s is a FAIL to investigate | **FAIL — 468 declines, all on GPT seats** | HIGH #1. Every one at `12002 ticks`, i.e. 1,200 *game* seconds under `WAGIC_FASTCLOCK=0.1` |
| **Q** carry | timeout arm (`fallback: "timeout"` + `retry: 1` + two stderr lines) | **PARTIAL PASS — first live firing** | 2 `no reply after 900s - one retry` lines; 1 produced a full record (`retry: 1`, `deadline_pct: 165.4`, answered on retry); the other produced **no record at all**. `fallback: "timeout"` itself renders 0 — the retry succeeded, which is the arm working |
| **P** carry | no ask menu > 12 rows collapsing to ≤ 2 shapes | **FAIL (13; wave 53: 3)** | 47 menus exceed 12 rows. The 13 low-shape ones: `deck123-vs-deck126` seq 88 (**58** attacker rows, 2 shapes) and seq 36 (29 rows, **1** shape); `deck123-vs-deck162` seq 26 and 24 (**23 rows, 1 shape**); plus 9 more at 23-31 rows. See D-3 |
| **carry** | Baka-executed ≤ 0.20 % | **PASS (2 / 2,212 = 0.09 %)** | Both `game-125v123-1788425036.stderr:1655` and `:2121`, both the `named_row_reask_exhausted` tail of `deck123-vs-deck125` |
| **carry** | 21/21 natural | **PASS** | corpus-results.tsv, 0 timeouts / draws / crashes / hangs |

---

## Set C — audit-lane watch, with the flag to flip

| lane | what the corpus shows | verdict |
|---|---|---|
| **G** ability ownership | 0 crashes, 0 ASAN signatures, 21/21 natural, no dangling-ability trace in any stderr | **clean; nothing to isolate** |
| **H** ability epoch gate | No verdict drift I can observe from logs: `{right now:}` renders 1,025 times and every sampled verdict matched its own enumeration. No aslongas/lord card in the pool exercised a delayed-condition bump visibly | **not observable from this corpus — say so rather than claim clean** |
| **I** ManaCost / garbage / cleanup | No RSS telemetry in the corpus. Dead-token dangle signature: `- Your/Opponent's Goblin (token) ceased to exist and left …` renders normally (9 duplicated instances inside single records, the D-6 class). `In GetCost Seems ManaCost was not properly initialized` appears in stderr (e.g. `game-125v123:146`) — pre-existing noise, not new | **no regression visible; RSS untested** |
| **J** render caches / resource-miss memo | **WATCH ITEM.** 1,927 `AttemptNew failed to load (404)` lines over 118 distinct resources; `mana.wav` 105×, `land.wav` 105×, `creature.wav` 96×. The reveal display in `game-146v123:1250-1334` re-cycles the same four card images ~20 times in 85 lines. The memo is not covering the audio loader or surviving `Destroying WCachedResource` | **flag `WAGIC_RENDER_NOCACHE=1` for the A/B; desktop/Vita visual parity NOT eyeballed by me either** |
| **K** Baka `selectAbility` | Baka received 2 decisions + 1 heuristic handoff. Both executions are ordinary (`ai is doing a combo:Lightning Greaves` → cast; the other passed). No planner anomaly in stderr | **behaviour identical as far as 3 samples can say — effectively UNTESTED** |
| **L** narration / board index / http classes | `http_error` **0** on the 35B — **PASS**. Prompt shapes are recognisably wave-53's; mean prompt is DOWN 28 % on `ask` (14,245 → 10,269) and `>25,000` prompts fall 588 → 37, which is the 24 KB trim v2 working | **PASS on the falsifiable half** |
| **M** ask cache / prefill / combat window cache | No cache-hit counter reaches stderr, so "cache hits per game with turn-boundary clears" is **unmeasurable from this corpus** — ask for a counter on the gameend record. A22's cached blocker helpers got their first live exercise: 36 blockers records, 36 `declared blocks from N assignment(s)` lines 1:1, `one-blocker-many-attackers reply -> re-asking once` fired once and was followed by its declaration, `multiblock_first_wins` fired once, `blocks_last_line_taken` twice — **and `all_assignments_illegal` fired for the first time and lost a game (HIGH #3)** | **instrument gap; flag `WAGIC_GPT_AUDIT_M_OFF=1` is also the first A/B for HIGH #4** |
| **N** Vita textures | No desktop exposure | **UNTESTED here** |

**Behaviour with no engine-lane explanation → candidate audit-lane regression:** exactly one, and it
is HIGH #4's per-tick prompt instability on the reveal menu. Flag order: `WAGIC_GPT_AUDIT_M_OFF=1`,
then `WAGIC_GPT_BOARDINDEX=0`, then `WAGIC_GPT_SCRIPTMEMO=0`. HIGH #1 is NOT an audit-lane regression
— it is lane AA, which has no flag at all.

---

## NEW / CARRIED DEFECTS (record + emitter string), ranked

**D-1 HIGH — see HIGH #1** (stale-drop storm / lane AA stall floor).
**D-2 HIGH (correctness of the surface) — `Cast Devour Flesh` enumerates BOTH players as legal
targets and then prices only one of them. 178 renders, 76 of them with the self-target live.**

Emitter string, `deck123-vs-deck125` seq 8 row 3:
`Cast Devour Flesh {1}{b} {right now: they control 0 creatures - at 0 this does nothing} {leaves 1 of
your 3 untapped mana sources untapped} - legal targets right now: the opponent, you {card text:
"Target player sacrifices a creature, then gains life equal to that creature's toughness."} {leaves 1
source - rows 1 and 2 need more mana sources than the 1 this leaves}`

Primitive (verified): `name=Devour Flesh / target=player / auto=ability$!name(sacrifice)
notaTarget(creature|mybattlefield) dynamicability<!mytgt toughnesslifegain targetcontroller!>
sacrifice!$ targetedplayer`. The TARGETED player sacrifices, so **self-targeting is a real,
frequently-relevant play** — and `{right now: they control 0 creatures - at 0 this does nothing}` is
FALSE for half the row's own enumerated target list.

Counts: **178** Devour Flesh rows list `you` as a legal target; **173** of those carry a
`{right now: they …}` verdict that speaks only of the opponent; **76** are windows where the seat
itself controls ≥1 creature, i.e. the unpriced half is live. Takes: 0.

Cost, measured: `deck123-vs-deck126` seq **104** (turn 19, Combat begins, 58 Humans on board). The
model spends roughly 900 words of its reply working out whether choosing the row lets it pick a
target at all — `"The menu item 1 is 'Cast Devour Flesh'. It doesn't specify target yet. Usually, you
select the card, then the target… If I cannot target myself with this menu item, I cannot trigger the
loop."` — then abandons the line and takes the HOLD row. With Sanguine Bond + Exquisite Blood on the
board that self-target was a genuine win attempt, correctly identified and abandoned **because the
render did not say which target the row commits to.**

Fix shape: (a) when a `target=player` row enumerates both players, give the verdict per player
(`{right now: they control 0 creatures - at 0 this does nothing; YOU control 58 - you would sacrifice
one and gain its toughness}`); (b) state whether the row commits a target or opens a follow-up ask —
the model asked this question explicitly and got no answer anywhere in the prompt.

**D-3 MED — the one-source × N-identical-target ability menu is uncollapsed and is now the corpus's
biggest render waste. 41 records, 425 redundant rows, ~153,591 prompt chars.**

Repro: `deck123-vs-deck162` seq **26** (turn 10, priority, 23 rows, 9,207 chars). Every row is
`Equip with Lightning Greaves targeting Vampire #N [your battlefield] (Lightning Greaves is ALREADY
attached to Bloodline Keeper - this MOVES it to Vampire #N, and Bloodline Keeper loses what it
gave…)`, N = 1…23, on 23 identical 2/2 tokens. `chosen_text: pass`. Same shape at seq 23, 24, 34;
`deck123-vs-deck146` seq 22, 23, 24, 25.

Families: `Equip with Lightning Greaves targeting Human #N` 190 rows, `… Vampire #N` 93,
`Mountain #N [land] …` 54, `Human #N (1/1) …` 29, `Deal 2 damage with Siege-Gang Commander targeting
Goblin #N` 21, `Destroy with Dwarven Blastminer #N targeting Underground Sea` 8. Wave 53's Staff of
Nin exhibit is the same class; this is lane D's explicitly-not-shipped 2-D collapse, and it now
carries a measured price. Adjacent: `deck123-vs-deck126` seq **88** is a **58-row** attackers list
and seq **106** a **61-row** blockers list of interchangeable `Human #N` tokens — the standing
"hard caps on legal choices are rejected on sight" rule means the answer is a collapse
(`Human #1-#58 x58, all identical`), never a cap.

**D-4 MED — see HIGH #3**: `all_assignments_illegal` has no re-ask arm.
**D-5 MED — see HIGH #2**: the HOLD row's own-turn exclusion.
**D-6 LOW — the `(xN)` log collapse still misses phase lines and draws, and the generic ability
label survives.** 471 duplicate event lines inside single records (wave 53: 571 — improving):
`- Phase: Draw` 217, `- Opponent drew a card` 89, `- Phase: Main phase N` 79, `- Phase: Combat
damage` 5, token-ceased lines 9, `- Opponent's Ob Nixilis, the Hate-Twisted lost a loyalty counter`
3. Separately, 9 stack lines still read `ability: Ob Nixilis, the Hate-Twisted's triggered ability` —
the name-twice bug is fixed (E/D19 PASS) but the effect label still falls back to a generic when the
ability has no name.

**D-7 LOW — `- Paid` receipts with no payee: 3, still the Intrepid Adversary class.**
`deck152-vs-deck125` seq 36: `- Paid {1}{w} with Hengegate Pathway; Barkchannel Pathway #1` ×3, from
`- You chose add 19 counters for Intrepid Adversary`. Corpus-wide **15,170 `- Paid` renders across
all prompts, 0 with no source.** The no-payee class runs 7 → 3 → 2 → **3** across four waves.

**D-8 LOW — a 900 s wall miss with no translog record.** `game-146v123-1788425054.stderr:1607`
(above). Ask: a `wall_miss_unrecorded` counter on the gameend record, or write a zero-choice record.

**D-9 LOW — repeated resource 404s.** 1,927 lines / 118 resources. Set-C lane-J watch item.

**Observed, not defects.** `answer_replaced` 12 (`deck123-vs-deck126` seq 73 and 104 are the two most
interesting: `coded_answers: 2`, `latched_coded_line: 2`, the last clean line wins and it is the HOLD
row, over a header line that named a different row). `multiblock_first_wins` 1, `blocks_last_line_taken`
2, `repeat_count_under_two` 1, `echo_index_conflict` 1 — all behaved. `commit_retracted` 0 this corpus.

---

## Prompt economics for the owner's per-wave prompt review (invariant 0: decision value per token)

| kind | n | mean | p50 | p90 | max | p90 lat (s) | max lat (s) |
|---|---|---|---|---|---|---|---|
| ask | 1,683 | 10,269 | 9,403 | 18,301 | 28,776 | 48.5 | 1,488.7 |
| priority | 332 | 12,514 | 11,469 | 21,699 | **32,215** | 64.6 | 346.8 |
| attackers | 99 | 11,264 | 9,729 | 20,636 | 28,108 | 63.2 | 99.5 |
| blockers | 36 | 12,547 | 10,350 | 24,354 | 26,764 | 82.5 | 150.2 |
| discard | 34 | 12,333 | 10,940 | 22,011 | 25,767 | 27.7 | 54.5 |
| reveal | 20 | 15,692 | 14,912 | 24,609 | 27,183 | 53.9 | 61.5 |
| bottom | 8 | 2,196 | 2,126 | 2,634 | 2,634 | 182.9 | 182.9 |

**The tail is fixed and the body shrank.** Prompts over 30,000: **2** (wave 53: 12). Over 25,000:
**37** (wave 53: 588). `ask` mean 14,245 → **10,269 (−28 %)**. Total prompt volume 23.76 M chars over
2,212 decisions. Lane L's 24 KB trim v2 is the visible win.

**Per-decision inference cost went UP.** 14.67 h / 2,212 = **23.9 s per decision**, against wave 53's
20.96 h / 3,483 = **21.7 s** — **+10.2 %**, on a corpus whose mean prompt is 28 % smaller. That is
the number to watch: the prompt got cheaper and the decision did not. And the 488 stale drops are
model calls whose time is **not in the 14.67 h at all** — at the corpus mean that is **~3.2 h (+22 %)
of inference bought and thrown away**, which makes the true per-decision cost ≈ **29.1 s**. Closing
HIGH #1 is the single largest inference win available this wave, worth more than any prompt edit.

**Log share.** Across the 2,153 prompts carrying a `CURRENT SITUATION` marker the GAME LOG is
**56.1 %** of the prompt on average. On the shipped specimen it is **80.6 %** (20,758 of 25,767
chars) — see below.

### Specimen recommendation

The harvest default `wave54/lategame-specimen.txt` = `125v123` seq **188**, turn 63, Cleanup,
25,767 chars, 9 rows, is a **`discard`** record: a hand-size cull whose nine rows are bare card names
with no pricing, no kill summary, no fit clause, no HOLD row, no modes. It is the corpus's best
exhibit of one thing only — **80.6 % of a 25.8 KB prompt is log, to choose two cards to pitch** — and
it is a poor exhibit of everything the owner would actually be reviewing.

**Recommend shipping TWO specimens, and making the vocabulary-rich one the primary:**

1. **PRIMARY — `1788425073-ai_baka_deck146-…-vs-ai_baka_deck162.jsonl` seq 46**, turn 25, Main
   phase 2, **23,237 chars, 7 rows, 6.9 s latency, 8 life vs 8 life, 14-card hand, 4 mana.** Log
   share **64.4 %**. It carries `[DRAW PRICE:` with the life tail, `{modes live right now: …}` on
   Silverquill Command's nine-mode list, three distinct `{leaves N source(s) - …}` fit forms, the
   dungeons-completed line, the `{castable from exile - …}` cause clause, the counters/summoning-sick
   battlefield vocabulary — and the seat still answered `Cast nothing right now` at 8-8 with a full
   grip. That is the invariant-0 question in one file: **23 KB bought a decline at parity.**
2. **SECONDARY — keep `125v123` seq 188** as the pure log-cost exhibit, relabelled as such.

If only one file is wanted, ship #1.

---

## Falsifiable predictions for wave 55

1. **AA/HIGH #1.** With the interactive-AI stall budget moved to wall clock (or `WAGIC_FASTCLOCK=0`),
   `dropping stale async answer` returns to ≤ 80 corpus-wide and `releasing so the game advances` to
   0 on GPT seats. **Falsifier:** drops stay ≥ 300 with the floor disarmed — then the cause is a
   prompt rebuild, and `WAGIC_GPT_AUDIT_M_OFF=1` is next.
2. **AA/HIGH #1, second half.** With the floor fixed, the count of seats recording **zero**
   opponent-turn decisions falls from 16/42 to ≤ 4/42, and opponent-turn casting windows rise above
   700 (from 403).
3. **HOLD row on own turns (HIGH #2).** Identical-declined runs (wave-53 method) fall below 12 % and
   the maximum run length falls below 20 (from 50). **Falsifier:** the 50-run class recurs with the
   row on the menu — then the row is not the remedy and the repeat-suppression question re-opens.
4. **`pass_hold_ambiguous` (exact-match tiebreak).** Firings fall from 16 to ≤ 4, all of the
   `CHOICE: n (Pass priority)`-on-the-HOLD-row shape; `CHOICE: 0 (pass)` never stamps again.
5. **`all_assignments_illegal` (HIGH #3).** Every firing is followed by exactly one re-ask carrying
   the pruned pairs; records with no follower **0/N**. A second consecutive all-illegal reply may
   still declare no blocks — that is acceptable and must be logged as `..._exhausted`.
6. **Reveal (HIGH #4).** `stale_livelock` on a `reveal` record **0**, and any reveal driver that
   makes no progress writes a `reveal_stall` field. **Falsifier:** a 6-drop handoff recurs on a
   reveal with the force-close armed — then the instability is in the menu build, not the driver.
7. **D-2 Devour Flesh.** Rows enumerating both players whose `{right now:}` verdict names only one
   **0/N** (this corpus: 173). Some take of the self-target with SB+EB on board is NOT predicted —
   that is a deck-seat call.
8. **D-3 collapse.** Ask/priority menus containing > 3 rows of one `#N` shape **0**; redundant row
   chars < 20,000 (from ~153,591). Menus over 40 rows survive only for attackers/blockers, and those
   carry an `x N, all identical` collapse line.
9. **Per-decision cost.** With HIGH #1 closed, recorded inference per decision falls below 24 s AND
   uncounted (dropped-call) inference falls below 0.5 h. **Falsifier:** per-decision cost stays above
   26 s with drops at 0 — then the model, not the engine, owns the regression.
10. **Timeout arm.** `no reply after 900s - one retry` events each produce exactly one decision record
    (this corpus: 2 events, 1 record).

---

## What I did NOT check

- **I did not build, run, or A/B anything.** Every flag recommendation is a proposal for the lane, not
  a measurement. In particular I could not confirm HIGH #1 by disarming lane AA; the code-plus-log
  argument is strong (single tick value 12002, dt pinned at 0.1, 4 zero-release games carry the
  wave-53 drop rate) but it is inference until someone flips `WAGIC_FASTCLOCK`.
- **Lane G/H/I/N are largely unobservable from this corpus.** No RSS, no ASAN, no per-tick verdict
  trace, no Vita exposure. I reported "not observable" rather than "clean".
- **Lane M's cache-hit counters do not reach any log**, so its central watch item is unmeasured.
  Asking for a `cache_hits` field on the gameend record is a cheap fix.
- **I did not diff prompts across polls** (only the final prompt is recorded), so HIGH #4's
  instability is identified but not localised.
- **I did not read any deck guide or measure guide sizes** — this seat owns no deck, and the
  39.6-70.2 KB band belongs to the deck seats and synthesis.
- **I could not reproduce the harvest's 12.2 % identical-declined figure** with either the wave-53
  predicate or a HOLD-inclusive one; I report 16.1 % / 17.9 % with the method stated and flag the
  discrepancy rather than adopting either number silently.
- Card facts verified: Devour Flesh, Pelakka Predation, Silverquill Silencer, Intruder Alarm,
  Silverquill Command (via `_cards` primitives). I did **not** cross-check any of them against
  Scryfall — none of my claims turns on a printing detail the primitive does not carry.
