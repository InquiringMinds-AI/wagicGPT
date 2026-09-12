# Wave 81 - lane DI: the priority ask-cache replayed an ACTIVATION without bound

## The defect

`matchups-20260912-074153`, game `162v123`: HUNG for 3,641 s in an engine dead loop
and killed by the harness. Seat deck123's translog ends at **seq 35** and the game
kept running for another hour.

Seq 35 (turn 8, Main phase 2, kind `priority`) is a real, well-formed decision:

```
reply     "\n\nPLAN: Tap Doomsayer 9 times to build army, then pass. Next turn attack with all.
           \nCHOICE: 2 (Create human with Thraben Doomsayer, repeated then stop x9)"
repeat_n  9        protocol_deviation "compliant"      reasoning_chars 7772
```

The repeat plan ran its 8 remaining iterations (`repeat plan iteration N/9`, stderr
counts 8). Then the seat performed **3,674** `AIPlayer:Using Activated ability` on
Thraben Doomsayer, with **6** `model chose` lines in the entire game.

stderr `game-162v123-1789216936.stderr`, the loop's own two lines, 3,655 / 3,659 times:

```
AIPlayerGPT: the menu is unchanged after action 2 but the board moved - that
             activation progressed the game, so priority is not force-passed
AIPlayerGPT[ph10]: take action 2/3 (cached=1)
```

## The replay path (file:function)

`projects/mtg/src/AIPlayerGPT.cpp` : `AIPlayerGPT::chooseOrderedAction`

1. `askKey = w79AskScopeKey(turn, phase, w79ContinuationDigestPriority(shown)) + w77KeyTailOf(tail)`
2. `bool unchanged = (askKey == mLastAskKey);`
3. `w79ForcePassNoProgress(unchanged, mLastChoice, boardMoved)` - **#W79-DC (F1)**
   correctly declines to force-pass an activation that moved the board, and prints
   the first of the two lines above.
4. `if (unchanged) { choice = mLastChoice; }` - **no bound at all**, and the comment
   there says the cache half is *deliberately* not gated on the board (#W79-CZ T3:
   a life tick under an unchanged menu is not a new question).

The seat controls **Intruder Alarm** (whenever a creature enters, untap all
creatures). So after every token the Doomsayer is untapped and the SAME row is
re-offered: same label, same `instanceHandle`, no target chooser (empty target
token set), and `potentialColorReach` unchanged because the Alarm untapped
everything. The continuation digest is byte-identical, the key tail is
byte-identical, `unchanged` is true - forever. The model is never asked again.

The two halves are individually right and jointly unbounded: F1 removed the only
exit (the forced pass) from a path that had no other one.

The **ask/cast** seam (`askModel`) is not affected - it has had
`askReplayRefuseScoped(..., kAskReplayRefuseMax)` since #W71-BP/#W72-BT. Only the
priority seam's `unchanged` cache was unbounded.

## The fix

1. **`w81CachedReplayMustReask(keyUnchanged, lastChoice, replaysThisKey, rowPastStatedStop)`**
   (pure). An answer that ACTIVATES or CASTS is replayed **at most once per window
   key**; the second identical offer goes back to the model, which restates its
   plan, names a stop, or passes. A cached PASS or HOLD is untouched (it commits
   nothing, and re-asking those is the waste #W79-CZ T3 removed). The re-ask is
   **latched** (`mCachedReplayRuns = -1`) until the model really answers that key,
   or the round trip's own `kChoicePending` ticks would find a zeroed counter and
   fall straight back into the cache.
   *This is not a cap on a legal option*: the row stays on the menu at every one
   of those windows, nothing is removed and nothing is auto-answered - the MODEL
   decides again.
2. **The legal-continuation digest gains the seat's battlefield object count**, and
   only when some offered row's own resolution creates or moves objects
   (`w81RowCreatesOrMovesObjects`: `ATokenCreator` / `AAMover` / `AACopier`, one
   NestedAbility unwrap). A token the row itself made IS a new legal continuation
   for the next activation. **Justification against the standing rule** ("a board
   NUMBER that creates no row stays out of every key"): this number is admitted
   only for a row that MAKES an object, which is exactly the stated exception; a
   life total, a damage count or a clock still key EQUAL, and the term is omitted
   byte for byte when no such row is on the menu, so every wave-79 key is unchanged.
   It is a **digest** term only - `holdActionKeyRow`, `optionSetKeyOf`,
   `w79RePutCollapseIdentity` and the hold-check are untouched (pinned).
3. **The wave-72 stop binds the replay path.** `w81StopReachedRow[]` is filled from
   the same `rowStopPriced` verdict the row's own `{... ALREADY AT OR PAST your own
   stop}` clause prints, and a cached answer whose row is at or past the model's
   stated stop is refused outright - not even the one replay. (The existing
   `w72StopReachedWindowCollapses` / `w79StopReachedRePutCollapses` arms need EVERY
   base row stop-reached; a replay needs only its own row.)
4. **The breaker and the replay can no longer compound.** The unbounded loop needed
   `unchanged && boardMoved` to hold for ever. After (2) a board move that creates
   an object is not `unchanged`; after (1) any residual `unchanged && lastChoice>0`
   run is at most 1 before the model is asked. So the number of engine actions
   between two model calls at this seam is <= 2 - measured below as 226 activations
   against 365 model calls, versus 916 against 14 on base.
5. **Instrument**: counter `cached_replay_reasked` on the gameend record; per-record
   field `reask_reason` = `cached_replay_bound` | `cached_replay_past_stop`, stamped
   at the SEND (#W79-DC F10 discipline) and consumed by `writeTransLog`.

Also added, development builds only (`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`,
per the no-diagnostics-in-release rule): the **offline reply stub**
`WAGIC_GPT_STUB` - a `|`-separated reply list, one consumed per model call, the last
repeating - short-circuiting both `resolveEndpoint` and `pollCompletion`.

## RED evidence

### Live game, the real seam (`tools/w81-di-alarm-loop-probe.sh`)

A GPT-vs-GPT one-shot on stacked Doomsayer + Intruder Alarm decks, driven by the
stub (120 x `CHOICE: 1`, then pass for ever - so the seat activates a BOUNDED number
of times of its own accord). Same binary, same decks, same stub; the only difference
is the two fix sites.

| | stub replies | activations | `menu is unchanged after action` | exit |
|---|---|---|---|---|
| **BASE** (fix sites reverted) | **14** | **916** | **914** | **124 - the game never ended** |
| **FIXED** | 365 | 226 | 47 (23 bound re-asks) | **0 - winner=0, turn 20** |

The base leg's signature is the corpus's, scaled: a handful of model calls and
hundreds of activations under `menu is unchanged after action N but the board moved`.

**Why this is a script and not a SUITE fixture (the brief asked for one).** The test
harness's AI seat is `TestSuiteAI : public AIPlayerBaka` (`include/TestSuiteAI.h`
line 184), and the `[AI]` command only flips `playMode` on those objects - no suite
fixture can reach `AIPlayerGPT`'s seams at all. A Doomsayer+Alarm fixture would
exercise the HEURISTIC, the component next to the defect, and would be green before
and after the fix; that is exactly the instrument failure this project has a memory
about. The probe drives the real seam instead, offline and repeatably.

### PARSETEST (the predicate and the key, on the corpus's own rows)

`7086 -> 7102 passed, 0 failed` (+16). RED-on-base cases, both stated as the base
code's own rule:

* a 3,659-tick loop over `w81CachedReplayMustReask` - the base rule (`unchanged`
  alone) replays all 3,659 and asks the model none; the bound replays 1,830 and
  re-asks 1,829;
* `w79ContinuationDigestOf(labels, targets, 3)` before and after one activation is
  **equal** (the wave-79 digest cannot tell the two windows apart), and unequal once
  the object count is in it.

## GREEN

* PARSETEST **7102 passed, 0 failed**.
* Suite (single-threaded, detached unit): **1296 tests (0 failed), 76 AI tests
  (0 failed)**; `==Test Failed !==` 0, `Test timed out (game never ended)` 0.
* `check-reply-instructions.py` OK (1 source, 37 guides); `check-ctor-init.py` OK;
  `corpus-stats.py --selftest` OK.
* Probe leg FIXED: exit 0, `WAGIC_SELFPLAY_RESULT winner=0 ... turn=20`.

## Prediction (falsifiable)

* **0 HUNG games** in the wave-80 rerun and in the wave-81 corpus.
* `cached_replay_reasked > 0` on any seat whose deck runs a loop engine (deck123's
  Doomsayer + Intruder Alarm is the known one), with `reask_reason` on the
  corresponding records.
* On such seats, `menu_pass_no_progress_suppressed` falls sharply (the digest term
  makes most of those windows a fresh key rather than an unchanged one), while
  `ask_replays_cache` at the CAST seam is unchanged.
* No key-stability regression: `hold_events`, `hold_windows_skipped` and
  `stop_reached_re_puts_collapsed` stay in their wave-80 range.

## Weakest evidence

1. **The stub is not the model.** The live legs prove the ENGINE cannot loop without
   the model's consent; they do not prove qwen's next answer is a good one. Only the
   wave-81 corpus can say that.
2. **The probe game is shuffled**, so the combo does not form in every run - a BASE
   leg that exits 0 with 0 `menu is unchanged` never built the board and proves
   nothing. The RED above is one run in which it did form; it is reproducible but not
   deterministic.
3. **`w81RowCreatesOrMovesObjects` is a class list, not an effect analysis.** A row
   that creates or moves objects through some other ability class (a nested
   `MultiAbility`, a two-layer wrapper) is missed, and that window falls back on the
   replay bound alone - correct, but one round trip per two activations rather than
   one per activation.
4. **The bound is per KEY, not per phase.** The brief says "once per window/phase";
   a key is finer than a phase (it carries the seam scope plus the normalised rows),
   so a window that genuinely changes its rows starts a new allowance within the same
   phase. That is deliberate - a different question is a different question - but it
   is a reading, not the brief's literal word.
5. **No corpus was run** (brief: NO corpus, NO pilot requests), so the prediction is
   untested against live inference.
