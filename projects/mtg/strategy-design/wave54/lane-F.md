# Wave-54 lane F — observability + hang guard (D7 a/b/c, D38; D1 carried)

Base: master `42b3bf5da`. Branch `w54-lane-F`, worktree `worktrees/lanes/w54-F`.
Baseline re-confirmed on this worktree BEFORE any edit: suite **1219 tests, 2 failed**
(`lifeline.txt` + `merrow_reejerey.txt`, the known concurrency-only pair), **49 AI / 0**,
**0 timeouts**; PARSETEST **2258 / 0**. Byte-identical to the brief's numbers.

---

## D7(a) — a tick budget per reveal phase, and a SECOND hang the budget caught

An open reveal display is held by `GameObserver::OpenedDisplay` and blocks every
phase-advance path, `userRequestNextGamePhase` included. So a reveal-driver phase that
cannot make progress does not merely lose the reveal: it freezes the whole game, with no
tick and no stderr. That is the 13 hours wave-53 lost game `152v125` to.

Lane X removed the KNOWN way in. Writing the budget turned up a **second, still-live one**,
and it is not a variant of lane X's:

> A reveal whose option one is predicate-gated to zero legal targets **and which has no
> option two at all**. `toResolve()` takes the `countValidTargets() == 0` branch and
> constructs `abilityTwo` — which is the empty string, so `contructAbility` returns NULL.
> Neither option exists. No chooser ever arms. `driveInteractiveReveal` phase 0 waits on
> `!tc` forever, and lane X's `option one consumed elsewhere` branch cannot rescue it
> because that branch is gated on `abilityFirst` being non-NULL, and here it is NULL.

**Shipped** (`src/AllAbilities.cpp`, `include/AllAbilities.h`):
`driveInteractiveReveal` is now a stall-guard WRAPPER around the state machine
(`driveInteractiveRevealStep`). Each tick it hashes a **progress signature** and compares
it with the previous tick's. Unchanged for both budgets → `forceCloseStalledReveal()`:
the options are taken out of the action layer, every still-revealed card goes back to
`RevealFromZone` in order, an **ungated** `DebugTrace` names the source, the tick count,
the wall seconds and the driver phase, and the seat is asked to log a record
(`Player::logEngineResolution`, no-op on every seat that keeps no decision log;
`AIPlayerGPT` writes kind `reveal`, class `reveal_stall_forced`).

Two design points worth the next lane's attention:

* **The signature is DRIVER-LOCAL on purpose.** The first draft also folded in the game's
  turn, phase and layer depths, so a reveal waiting on a still-resolving stack item could
  never be counted as stalled. It made the guard **unreliable** — unrelated action-layer
  churn reset the counter on most ticks, and the same fixture passed alone and failed
  beside two other tests. The wall-clock floor is the honest way to buy that safety.
* **Two floors, both required: 20000 no-progress ticks AND 600 s of wall clock.** Ticks
  are frame-rate relative (minutes windowed, a heartbeat under `WAGIC_FASTCLOCK`), so a
  tick floor alone cannot tell a slow game from a dead one. A model call in flight bumps
  `mAIPollTicks` and therefore counts as progress — its deadline belongs to the async
  layer (lane Q's `timeout` class), not here. Non-interactive seats never enter the
  driver at all, so human and Baka games are untouched.

**Pin**: `bin/Res/test/w54_reveal_stall_forceclose.txt` (+ the test-only primitive
`Lexicon Deadend Wizard`, id 2100000987, and the fixture directive `revealstallbudget N`
which shrinks ONLY the budget — the stall itself is entirely real).
**RED on base** (the archived `42b3bf5da` binary, which already carries lane X's fix):
five cards stranded in the reveal zone, library **1 of 6** — the engine stopped ticking.
**GREEN after**: force-closed, library 6 of 6 in original order, game continues.
Re-run 3x under concurrency and 1x solo, stable.

## D7(b) — a fallback class on ANY engine-answered `-1`

`152v125` seq 28 — kind `reveal`, `prompt` 0, `reply` `""`, `choice` -1, `latency_ms` -1,
**no `fallback` field** — is the single record that marked the 13-hour park, and it read
exactly like a cache hit. Lane Q's recovery contract latches on `choice < 0` **and** a
class, so it could not see it.

The shape is structural, not site-specific: no prompt and no reply means no model call was
made, and `choice < 0` means nothing from a reply executed — so the ENGINE answered.
Shipped as one gate inside `writeTransLog` (`engineAnsweredNoModel`, pure), which covers
every present and future such seam rather than the one site the corpus happened to catch:

```
if (engineAnsweredNoModel(choice, fallback, userMsg.empty(), reply.empty()))
    fallback = "engine_answered";
```

An already-stamped class is never overwritten; a record carrying a prompt (asked, not
answered) or a reply (answered, unparsed) keeps its own account; an executed choice is
never a handoff. **9 PARSETEST cases**, 6 of them negatives, including the two that state
the point: `handedToHeuristic(-1, NULL)` is false (the pre-fix 152v125 shape latched
nothing) and `handedToHeuristic(-1, "engine_answered")` is true.

**Live**: a full 23-turn stub-server selfplay game (`tools/gpt-stub-server.py`, prose
answers, no model) wrote **343 records** — `choice < 0` with no `fallback`: **0**; the 149
`unparsed_reply` and 13 `stale_livelock` records kept their own classes; **162 handoffs,
162 recovery records**. The new gate was evaluated on every record in that game and
mislabelled none.

## D7(c) — a harness-side NO-PROGRESS watchdog

The existing feasibility watchdog measures **latency**, and a hang produces none — the
wave-53 process wrote nothing for 13 h and nothing fired. Worse, that watchdog **returns
immediately on an uncapped run** (`-T 0`), which is exactly the corpus configuration the
owner's "no cap" ruling mandates.

`tools/selfplay-harness.sh`: `feasibility_watchdog` becomes `supervisor`, which now runs
in EVERY configuration. Each 45 s cycle it sweeps the in-flight games (each game now runs
as a tracked child with a `.inflight-` marker carrying its pid, start and decks) and asks
one question: how long since **either seat of this game** last wrote a translog record?
Past `NO_PROGRESS_S` (default **3600 s**, `WAGIC_NO_PROGRESS_S`) that game — and only that
game — is stopped, loudly, by matchup, with its stderr and seat logs named. The rest of the
corpus continues. The latency projection still runs, and only when `-T` is set.

Invariant 00 is the whole design:
* It is a **dead-loop detector, not a cap**. A game deciding slowly writes; a game hung
  writes nothing. The default clears the worst legitimate silence by a wide margin — a
  decision can cost the full deadline twice (deadline + lane Q's one retry) and 420 s is
  the largest deadline the harness ever sets.
* **stderr is deliberately NOT the progress signal**: the 13-hour park kept printing
  `AttemptNew failed to load (404)` the whole time, so its stderr grew while the engine
  did not tick.
* A stopped game takes verdict `hang`, credits **no seat** and is excluded from the
  timeout bucket (life-adjudicating it would repeat the wave-39 crash-row mistake).
* The run ends with a `CORPUS INCOMPLETE` banner listing the hung matchups and **exits 1**
  — the good games are still harvested and are still evidence, but the run is not a
  complete corpus and cannot be read as one.

**Pin**: `strategy-design/wave54/lane-F-watchdog-test.sh` extracts `no_progress_sweep`
verbatim from the shipped harness and runs it against synthetic in-flight games. 5/5:
quiet 2 h → stopped, marked, flagged; quiet 900 s → left alone; quiet 1799 s (one second
inside the limit) → left alone; **no seat log yet → exempt** (a game still loading its card
DB, and a Baka run that writes none, ever); the report names the matchup and says it owes
a rerun.

## D38 — `assertmtgid`, and the test-suite flip skip lifted

Lane V's `MTGAllCards::getOtherFaceCard` decides WHICH printing's back face the console
asks art for, and no harness could observe it: PARSETEST runs before the card database
loads, and the suite SKIPPED `setMTGId` on flip (`playMode != MODE_TEST_SUITE`) in both
`AAFlip::resolve` and `AATurnSide::resolve`.

Both skips are **lifted** (the real game never had them, so the suite was proving the
wrong thing), and `assertmtgid <id> <card name>` pins the current face's id.

**Two consequences, both handled:**
* The zone matcher's strict id match now misses a flipped card and falls through to its
  existing name-level count — all 15 existing transform fixtures stay green.
* `getCard` (the `[DO]` click resolver) lost a transformed card entirely: a script names
  the PHYSICAL card by the face it was played as, and that used to resolve by the retained
  front id. `lexicon/trigger_transformed_equipment.txt` silently stopped declaring its
  attacker. Fixed with a last-resort `nameOrig` pass — only after an exact id AND exact
  current-name match have both failed, so a fixture holding both faces still binds the one
  it named. This restores the permissive click resolution the `[DO]` section always had.

**Pin**: `bin/Res/test/w54_assertmtgid_transform.txt` — Delver of Secrets **ISD 226749**
transforms on its first upkeep and must become **ISD 226755**, the same printing's back
face. Discrimination shown both ways: expecting MID's back face (**534808**) fails with
`expected id 534808 got 226755 (set 138)`. On the base binary the command is **swallowed
as an unmatched card name and the test passes vacuously** — that is the docket's "observed
by no harness", demonstrated rather than asserted.

## D1 — carried verification item (context only)

Nothing shipped. D1 is discharged only by a 21/21 wave-54 corpus. Lane X's fix is intact
in this branch and its fixture stays green. This lane's contribution to D1 is the floor
under it: the second hang shape above shows the class was not closed by lane X's fix
alone, and D7(c) means the next corpus reports a hang within an hour instead of losing 13.

---

## Gate (this worktree, after a clean link)

| gate | base (`42b3bf5da`) | after |
|---|---|---|
| suite | 1219 tests, **2 failed**, 49 AI / 0, 0 timeouts | **1221 tests, 2 failed, 49 AI / 0, 0 timeouts** |
| PARSETEST | 2258 / 0 | **2267 / 0** (2258 + 9) |

+2 tests = this lane's two new fixtures. The 2 failures are the concurrency-only class:
this run's names were `lifeline.txt` + `merfolk_looter.txt` (the pair churns names run to
run); all three of `lifeline`, `merrow_reejerey`, `merfolk_looter` pass **solo,
single-threaded**. All runs under `MemoryMax=4G MemorySwapMax=0`; no kill under the cap.
`git diff | grep -c $'\357\277\275'` = **0**.

## Falsifiable predictions for the next corpus

* **D7(b)** Records with `choice: -1` and no `fallback` field: **0** (was 1). Every
  engine-answered decision (`prompt` 0, `reply` empty, `choice` -1) carries
  `fallback: "engine_answered"` and is followed by exactly one `kind: "recovery"` record
  naming it: **N/N**. Records with `latency_ms < 0` that are the LAST record of a seat:
  **0**.
* **D7(a)** Seat pairs with no `gameend`: **0**. Any reveal that does park writes a
  `MTGRevealingCards: ... FORCE-CLOSING it` line to that game's stderr and a
  `reveal_stall_forced` record — so the count of hangs becomes a NUMBER instead of an
  archaeology exercise. Expected count on a healthy corpus: **0**; a non-zero count is a
  new hang path and names its driver phase.
* **D7(c)** Games whose two seats write nothing for > `NO_PROGRESS_S` while the harness
  runs and which are NOT reported: **0**. Games killed by the sweep that were in fact
  still deciding (a seat record written inside the window): **0** — falsifier: any `hang`
  row whose seat log has a record newer than kill-time minus `NO_PROGRESS_S`.
* **D38** `w54_assertmtgid_transform` stays green, and any future change that makes
  `getOtherFaceCard` pick a different printing's face is RED at the suite rather than
  visible only on the console.

## What I did NOT verify

* **No live model run.** Everything here was proven against fixtures, the archived base
  binary, and a stub-server game. Nothing touched `qwen36-35b-a3b`.
* **The `engine_answered` class was never STAMPED in a live game.** The 343-record stub
  game contains no reveal self-decline (the shape that produces it), so what is proven
  live is that the gate fires on nothing it should not — not that it fires when it should.
  Its positive case rests on the PARSETEST pin and on the code path being the one
  `writeTransLog` takes for every record.
* **The force-close's translog record was not observed at all.** The suite's seat is a
  `TestSuiteAI`, which keeps no decision log, so the fixture pins the ENGINE half
  (cards returned, game continues) and the record half is pinned only through
  `handedToHeuristic("reveal_stall_forced")` in PARSETEST.
* **The shipped budgets (20000 ticks / 600 s) were never reached by anything.** Every
  force-close observed here ran at `revealstallbudget 5`. The arithmetic is
  budget-relative, but no run at the real numbers was made, and the 600 s wall floor is a
  JUDGEMENT about the longest legitimate freeze this engine can produce — the one case it
  is buying against (a human holding priority while an AI reveal waits on the stack) was
  reasoned about, not measured.
* **The no-progress watchdog was never exercised against a real hung game.** Its logic,
  its thresholds, its exemption and its report text are pinned against synthetic in-flight
  games; the marker/kill plumbing inside `run_one_game` (backgrounding the timeout child,
  `wait`, the `.hung` handoff) ran only in that harness's own syntax check and a real
  corpus has not been launched since.
* **`NO_PROGRESS_S` was not measured against a `-j 21` corpus.** Under heavy queueing a
  seat's silence is longer than at `-j 3`; 3600 s should still clear it (>4x the largest
  deadline+retry) but the distribution is a corpus question.
* **Lifting the flip skip was validated against the SUITE, not the whole engine.** All 15
  transform fixtures and the full 1221-test suite are green, but `setMTGId` on flip now
  runs in suite games where it never did, and any suite-only consumer of a flipped card's
  id that no fixture covers is unproven. (Real games always ran it.)
* **The reveal-driver phases other than 0 and 3 were not driven into a stall.** The guard
  is phase-agnostic by construction; only phase 0's stall is pinned.
