# Wave-55 lane E — reveal + observability + mulligan (D5 a/b, D23, D12)

Base: master `8a1768d1c`. Branch `w55-lane-E`, worktree `worktrees/lanes/w55-E`.
Baseline re-confirmed on this worktree BEFORE any edit: suite **1228 tests, 2 failed**
(`lifeline.txt` + `merrow_reejerey.txt`), **51 AI / 0**, **0 timeouts**; PARSETEST
**2489 / 0**. Byte-identical to the brief's numbers.

---

## D5(a) — the force-close now covers the POLL-CHURN stall, and every reveal record says how long the driver was parked

**The window lane F's guard did not have.** `MTGRevealingCards::revealProgressSignature()`
folds `mAIPollTicks` in, and `mAIPollTicks` is incremented on **every phase-0 tick with a
model call in flight**. So the one hang shape whose driver *is* doing something — asking,
being answered, having the answer dropped, asking again — resets the budget on every single
tick and can never reach it. That is exactly `146v123` s15: six `dropping stale async answer`
lines back to back, no release line, no phase step, no stack action, the driver sitting in
phase 0 the whole time. `force-close` renders 0 corpus-wide because the guard was blind to it.

**Shipped** (`src/AllAbilities.cpp`, `include/AllAbilities.h`, `include/Player.h`):

* `revealProgressSignature(bool withPolls)`. The **structural** signature is the same value
  with the poll counter left out — it does not move while the driver only polls.
* Both budgets are advanced **every tick** (the structural one cannot sit behind an early
  return on the full test, because the poll-churn shape is precisely the one whose full
  signature changes on every tick). `forceCloseStalledReveal(why)` names which ran out:
  `driver` (nothing moved at all, the wave-54 shape) or `poll-churn`.
* The poll-churn wall floor is read off the **seat's own deadline**
  (`Player::decisionDeadlineMs`, `AIPlayerGPT` returns `mTimeoutMs`):
  `revealStallStructSecsFor()` = max(1800 s, 3 × deadline) — one deadline, plus lane Q's one
  retry, plus a full deadline of margin. Raising `WAGIC_GPT_TIMEOUT` therefore cannot turn a
  legitimately slow decision into a force-close. Pure, pinned in PARSETEST (4 cases).
* `Player::noteRevealStall(ticks, secs, phase)` is called by the driver **before** the step
  runs — the step is where `decideReveal` writes its record, so a decision taken out of a
  parked driver carries the park. `AIPlayerGPT` stamps `reveal_stall`, `reveal_stall_secs`,
  `reveal_stall_phase` on `kind: reveal` records, present only when the driver was parked,
  consumed so it can never leak onto a later record. Report only — nothing in the engine
  reads it.

**Pin**: `bin/Res/test/w55_reveal_stall_pollchurn.txt`. `Lexicon Hurkyl Wizard` (the existing
test primitive, reveal WITH an option two) plus **Icy Manipulator** in the top five, so option
one is built, its chooser arms, and the driver reaches phase 0 with a real decision;
`revealasyncticks 100000` makes the stub answer **never**, so the driver polls forever.
`revealstallbudget 5` shrinks ONLY the budget — the churn is entirely real.
**RED on the archived base `8a1768d1c`** (which already carries lane F's guard AND lane X's
fix): `Card number not the same in player 0's library, expected 6, got 1` — the five revealed
cards stranded and the display holding every phase-advance path open.
**GREEN after**, with the trace
`reveal from Lexicon Hurkyl Wizard made no progress (poll-churn) for 0 ticks / 0s (structural
5 ticks / 0s, 6 polls) with 5 card(s) still revealed - FORCE-CLOSING it (driver phase 0)`.
The wave-54 fixture `w54_reveal_stall_forceclose` stays green and still reports `driver`.

## D5(b) — the A/B: **none of the three flags**, and the drift dump names the mechanism

**The instrument.** Rather than guess between three flags on a two-line symptom, a stale drop
now dumps **where** the prompt moved: `GPT_DRIFT_TRACE` prints the first byte at which the
rebuilt prompt diverges from the one in flight, both neighbourhoods and both lengths.
Compile-time gated (`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`) and env-armed
(`WAGIC_GPT_DRIFT=1`), so release builds do not contain it and dev builds are silent.

**Reproduction** (stub server, no model): `tools/gpt-stub-server.py --prefer "Pelakka
Predation"`, `WAGIC_SELFPLAY_ONESHOT=1 DECK0=146 DECK1=123 WAGIC_AI=gpt`, isolated `HOME`.
One full 16-turn game produced **30 stale drops, 30 drift dumps**.

**Result.** The drops are NOT a reveal-menu text instability. 24 of 30 are one shape, and the
dump states it outright — the prompt alternates between **two different questions on an
unchanged board**:

```
[DRIFT] first diff at 5490 of 6079 -> 8102
[DRIFT] inflight: |... Your library: 45 cards / Land drop: which land do you play now, if any? ...|
[DRIFT] rebuilt : |... Your library: 45 cards / Casting decision (Main phase 1, YOUR turn): ...|
```

and the very next drop is the same pair with the lengths swapped (`8102 -> 6079`), tick after
tick. The **land-drop ask and the casting ask are contending for the one async slot**: each
tick the seam that runs first replaces the pending prompt, so whichever answer comes back is
stale by construction. "The board did not move; the prompt did" is literally true, and the
mover is the SEAM CHOICE, not the rendered text. 3 of 30 were genuine board movement
(`Opponent battlefield (6 permanents … of which 2 are cr` -> `1 is a c`) — the async design
working. The rest are the same alternation at other windows.

**The flag A/B, run anyway** (one full game each, same deck pin, drift armed):

| run | flag | exit | turns | stale drops |
|---|---|---|---|---|
| control | — | 0 | 16 | 30 |
| A | `WAGIC_GPT_AUDIT_M_OFF=1` | 124 (500 s cap) | 11 | 9 |
| B | `WAGIC_GPT_BOARDINDEX=0` | **139 (SIGSEGV)** | 12 | 18 |
| C | `WAGIC_GPT_SCRIPTMEMO=0` | 0 | 16 | 39 |

**No flag removes the drops**, and the games diverge (a stub reply changes the line of play),
so the counts are not a comparison — the drift dump is what carries the verdict, and it puts
the cause outside all three. **Answer: none.** The wave-54 situation prefill, the board index
and the script memo are not the mover in this reproduction.

**Not fixed here, deliberately.** The fix belongs in the priority/cast seam — lane A owns
every priority-seam item this wave (D2/D4/D11/D19/D21) and two lanes editing that region in
one wave is the collision the merge order exists to avoid. It is written up as a docket
candidate below rather than patched.

**A separate, PRE-EXISTING crash found by run B.** `WAGIC_GPT_BOARDINDEX=0` segfaults —
and the **archived base binary `8a1768d1c` segfaults on the same flag** (exit 139, turn 10),
so the legacy per-host-scan path is broken independently of this lane. It matters because
that flag is the disable lever for lane L's board index: today "was it the index?" cannot be
answered, because turning the index off crashes the game.

## D23 — a 900 s wall miss always leaves a record

`146v123`:1607 missed the wall, launched its one retry, and the window then auto-passed
(`only display-toggle (Flip Side) options`) before the retry could be consumed. The decision
was abandoned and **nothing wrote it down**: two `no reply after 900s` events, one record.

**Shipped** (`src/AIPlayerGPT.cpp`): the deadline-retry site arms a wall-miss account on the
prompt that missed (`mWallMissPending` / `mWallMissBase`, `mWallMissEvents++`). Exactly one
of two things then closes it:

* the record that consumes that prompt carries `wall_miss: 1` (`writeTransLog`, matched on
  the prompt itself, not on a counter); or
* the ask is **abandoned** — `wallMissAbandoned(pending, missedBase, nowBase)`, pure and
  pinned with 4 PARSETEST cases — and `flushWallMissRecord()` writes a zero-choice record
  `kind: "wall_miss"`, class `wall_miss_unrecorded`, carrying the prompt the model never
  answered. Checked at the top of `pollCompletionRetry` (the seat has moved to another
  decision) and in `logGameEnd` before the recovery flush, so the last decision of a game is
  covered too.

The `gameend` record additionally carries `wall_miss_events` and `wall_miss_unrecorded`,
written always, present or zero, so a seat review divides rather than infers. Additive
throughout: no window removed, no retry spent, nothing in the engine reads any of it.

## D12 — the mulligan coverage clause's colour family now says its scope

11 renders, 11 with a cause, two families that call for opposite decisions. The COUNT family
is a hand-level verdict and is **byte-identical to wave 54** (pinned). The COLOUR family is
not a hand-level verdict at all — it is a statement about the lands in this hand, before a
single draw — and `152v146` s1 mulliganed a three-land seven under it, naming the colour as
its reason, for the fourth corpus running.

Nothing is deleted and nothing is hedged (the trust doctrine forbids teaching doubt about a
true render): the scope the clause always implied is now **said**.

* one land: `(you have 1 land; no spell in it is castable off {G} alone **on turn one**)`
* two or more: `(you have 2 lands; no spell in it is castable off {G} alone **even with every
  land in this hand in play**)`

The docket's literal (`on turn one`) is only true on the one-land branch — above that the
union of colours the clause computes assumes every land in the hand is in play, and saying
"turn one" over three lands would be a second false scope. Both branches, and both negatives,
are pinned; the existing wave-54 D16 case was updated to the new literal (a render change
ships its cases).

---

## Gate (this worktree, after `make -B` and a clean link)

| gate | base (`8a1768d1c`) | after |
|---|---|---|
| suite | 1228 tests, **2 failed** (`lifeline`, `merrow_reejerey`), 51 AI / 0, 0 timeouts | **1229 tests, 2 failed (the same two names), 51 AI / 0, 0 timeouts** |
| PARSETEST | 2489 / 0 | **2502 / 0** (2489 + 13) |

+1 test = `w55_reveal_stall_pollchurn`. +13 PARSETEST = 5 (D12) + 4 (D23) + 4 (D5a); the
wave-54 D16 case was **modified**, not added. All runs under `MemoryMax=4G MemorySwapMax=0`;
no kill under the cap. `git diff | grep -c $'\357\277\275'` = **0**.

**A build lesson worth the next lane's attention.** This lane adds a virtual to `Player.h`,
which changes the vtable layout of every `Player` subclass. The seeded worktree's copied
`objs-sdl` have fresh mtimes, so the incremental `make` left stale objects linked against the
old layout and the first three stub games **segfaulted at turn 8** — a crash that looked
exactly like a bug in this lane's diff and was not. `make -B` removed it and the same game
then ran to a natural end (16 turns, exit 0). Any lane touching a widely-derived header
should take the skill's `make -B` rule as mandatory, not advisory.

## Falsifiable predictions for the next corpus

* **D5(a)** Reveal records written while the driver was parked carry `reveal_stall` /
  `reveal_stall_secs` / `reveal_stall_phase`: **N/N**, so a poll-churn park is a NUMBER
  instead of an archaeology pass over stderr. `stale_livelock` on a `reveal` record with no
  `reveal_stall` field: **0**. Force-closes remain expected **0** on a healthy corpus; any
  that fire name their budget (`driver` / `poll-churn`) and their driver phase. Falsifier: a
  seat pair with no `gameend`, or a reveal that parks and writes neither.
* **D5(b)** Runs of ≥ 3 consecutive `dropping stale async answer` lines with no release
  between them are **still expected** — this lane did not fix the cause. What is predicted is
  that any such run in a `WAGIC_GPT_DRIFT` dev run names its first divergence, and that the
  dominant shape is a land-drop/cast ask alternation rather than a reveal-menu text change.
* **D23** `no reply after` events each produce exactly one decision record: **N/N** (this
  corpus: 2 events, 1 record). Every `gameend` carries `wall_miss_events` and
  `wall_miss_unrecorded`; `wall_miss_events - (records carrying wall_miss) - wall_miss_unrecorded`
  = **0**.
* **D12** Mulligans taken under a COLOUR-family coverage clause at 2+ lands: **0** (was 1 of
  4 for four corpora). Colour-family renders carrying no scope clause: **0**. Count-family
  renders differing by one byte from wave 54: **0**.

## What I did NOT verify

* **No live model run.** Everything here was proven against fixtures, the archived base
  binary, and stub-server selfplay games. Nothing touched `qwen36-35b-a3b` at :8084.
* **`reveal_stall` was never stamped in a live game.** The stub games' reveals resolved
  without a park, so what is proven is that the field is absent when it should be absent and
  present in the fixture; it has not been observed on a real livelock.
* **`wall_miss_unrecorded` was never observed firing.** The stub server's `--hang-ms` run
  produced no deadline miss inside the probe window, so the abandonment branch is proven by
  its pure predicate and by construction, not by a live wall miss. The `wall_miss` stamp on a
  consuming record is likewise unobserved live.
* **The D5(b) verdict is scoped to ONE reproduction.** It is a stub-answered
  deck146-vs-deck123 game, not the corpus board, and no reveal in it livelocked. It shows
  that a per-tick prompt instability of this class exists and is a seam alternation; it does
  NOT prove the `revealzone(opponenthand)` reveal at `146v123` s15 had the same cause, and it
  does not exonerate the reveal menu build on a board this game never reached.
* **The `WAGIC_GPT_BOARDINDEX=0` segfault is reported, not diagnosed.** It reproduces on the
  base binary, so it is pre-existing; no backtrace was taken and no fix attempted.
* **The A/B counts are not comparable across runs** — a stub reply changes the line of play,
  so the three flag games diverged. Only the drift dump carries the verdict.
