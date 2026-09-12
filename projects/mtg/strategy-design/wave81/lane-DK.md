# Wave 81 - lane DK: holds, response windows and the send boundary (V1, V3, V4, V9, V15)

Base `4df71b19b`. Worktree `worktrees/lanes/w81-DK`, branch `w81-lane-DK`.
Corpus evidence: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final`
(21 games + the HUNG attempt-0 stderr), sources wave80/known-bugs.md V1/V3/V4/V9/V15
and wave80/engine-seat.md §2(b), §2(d), §4 HIGH-1/HIGH-2/HIGH-4/MED-5.

Live instrument: `projects/mtg/tools/w81-dk-window-probe.sh` - a GPT-vs-GPT one-shot on
a stacked probe deck (Nadaar, Selfless Paladin x12 for the legend rule, Essence Scatter
and Path to Exile x8 each for instant-speed response windows, and a stub that takes the
hold row) driven by the development-build offline reply stub `WAGIC_GPT_STUB`, so the
real callers (`chooseOrderedAction`, `askModel`, `chooseTarget`'s legend-rule branch,
the hold latch) run deterministically with no network. The suite CANNOT reach any of
them: `TestSuiteAI : public AIPlayerBaka` (`include/TestSuiteAI.h`) and `[AI]` only
flips `playMode` on those objects, so a fixture on any of these boards would exercise
the HEURISTIC - the component next to the defect - and be green before and after.

---

## V1 - the cached-replay bound, adjudicated; and a state-based action served from the ask cache

**Verdict: DI's bound REFUTED as the fix for the two runs named; the legend-rule half CONFIRMED and fixed.**

### Adjudication of lane DI against the step-one binary's runs

The two runs the brief names are **not** at the seam `#W81-DI` bounds. Both are ASK-seam
cache replays, and both replay a **decline**:

| run | seat log | record | question | answer | run length |
|---|---|---|---|---|---|
| 39 | `askreplay/1789216935-ai_baka_deck126-…-vs-ai_baka_deck125.jsonl` | `replayed_from` **96** | Casting decision (Upkeep, YOUR turn) | `3 (Cast nothing right now)` | `replay_run` 39 |
| 17 | `askreplay/1789216951-ai_baka_deck126-…-vs-ai_baka_deck162.jsonl` | `replayed_from` **33** | Casting decision | decline | `replay_run` 17 |

`w81CachedReplayMustReask` deliberately exempts a cached PASS or HOLD (`lastChoice <= 0`
returns false, and the decline row is the hold/decline row of a casting menu, not an
activation), and the priority seam's `unchanged` cache is the only cache it gates.
`menu is unchanged after action` is **0 in all 20 corpus games and 0 in the rerun** -
the string appears 3,655 times only in `game-162v123-1789216936-HUNG-attempt0.stderr`.
So the DI bound is neither engaged nor needed by those two runs; they sit inside the ask
seam's existing `askReplayRefuseScoped(..., kAskReplayRefuseMax = 64)`. **DI is not
falsified by them and is not credited with them.** DI stays UNTESTED live (the rerun's
`cached_replay_reasked` is 0 and `reask_reason` is on 0 records corpus-wide, because the
Doomsayer+Alarm board never formed again: across the rerun deck123 seat's 55 prompts
naming Intruder Alarm it is in HAND in 55 and on the battlefield in 0).

PARSETEST re-states this as the corpus's own arithmetic:
`#W81-DK V1 ADJUDICATION` drives 39 identical offers through
`AIPlayerGPT::askReplayRefuseScoped` and shows the run never reaches the bound.

### RED (the half that IS a defect)

`askreplay/1789216925-ai_baka_deck146-0x55be71935390-vs-ai_baka_deck162.jsonl`,
`seq 27`, `why: cache_replay`, `replayed_from: 24`, `choice 2 of 2`, question:

```
LEGEND RULE CHOICE for Nadaar, Selfless Paladin - this is the STATE-BASED ACTION of the
legend rule … Pick from the list below the copy that GOES TO ITS OWNER'S GRAVEYARD now
```

A cached answer that **destroys a permanent**, served with no round trip and no
`-> chose`. That is exactly the commit class `#W81-DI` bounds at the priority seam, and
the ask seam's only bound was 64 identical replays. The window is issued by the live
caller `AIPlayerGPT::chooseTarget` (`src/AIPlayerGPT.cpp:55068`, the `legendRuleSelect`
branch) into `askModel`.

### Fix

* `w81AskCacheUsable(bool stateBasedActionWindow)` (`src/AIPlayerGPT.cpp`, wave-81 helper
  block) - pure, both faces provable.
* `AIPlayerGPT::StateBasedActionAskScope` (`include/AIPlayerGPT.h`) taken around the
  legend-rule `askModel` call only (RAII).
* `askModel` then bypasses **both** re-serve paths for such a window (`mAskCache` and
  `repeatAskAnswerStands` - the `#W71-BS F4` rule: both or neither) and does not store
  the answer. Nothing is removed from the menu and nothing is auto-answered: the MODEL
  decides, every time the rule applies.
* Counter `sba_windows_cache_bypassed`; per-record field `cache_bypass_reason:
  state_based_action`, stamped at the SEND.

### GREEN

* PARSETEST `[#W81-DK V1]`: 3 pins (RED-on-base `w81AskCacheUsable(false)` = the base
  rule, GREEN `w81AskCacheUsable(true)` = false, ADJUDICATION over the 39-deep run).
* Live: probe on the fixed binary, seat logs
  `1789248457-ai_baka_deck196-…` / `…deck197-…`: `sba_windows_cache_bypassed` **1 / 2**,
  records carrying `cache_bypass_reason` **1 / 2**, stderr `a state-based-action window
  (the legend rule) - neither answer cache may serve it` **3**. On the base binary the
  same probe prints that line **0** times and the counter does not exist.

### Prediction (falsifiable, NON-ZERO)

`sba_windows_cache_bypassed > 0` on the next corpus, with the same number of records
carrying `cache_bypass_reason: state_based_action`. Floor from wave 80: 5 legend-rule
windows across 21 games (4 asked + 1 cache replay). **0 is a FAIL.** Negative clause:
`why: cache_replay` sidecar records whose `question` contains `LEGEND RULE CHOICE`
must be **0** (wave 80: 1).

---

## V3 - a `new_lethal` hold re-open is a logged no-op

**Verdict: CONFIRMED. Root cause found: the re-open works; the ANSWER CACHE re-serves the hold row.**

### RED

`1789216953-ai_baka_deck125-0x55ed042111c0-vs-ai_baka_deck162.jsonl` seqs **18, 20, 22,
24**: four `hold_event` `reopen_new_lethal`, `seam: cast`, `window_seq 14`, face
`[stack death verdict: the stack KILLS you]`; **no prompt-bearing record after seq 17**;
`gameend` seq 26, `won: false`, `my_life: 0`.
`game-125v162-1789216950.stderr:1289-1309` prints, four times:

```
AIPlayerGPT: a hold re-opened at the cast seam - the live [stack death verdict: the stack KILLS you] face is lethal over a DIFFERENT object set …
AIPlayerGPT: hold re-opened at the cast seam - a NEW lethal threat: …
AIPlayerGPT: the model took the hold row at the cast seam on turn 16 - later cast windows are held until one of these rows changes
```

with **no `-> chose`** between the re-open and the re-take. The re-open half of DH F3
DOES work - `src/AIPlayerGPT.cpp` `holdHonoured`'s `if (!stands)` branch erases
`mHoldRows[seam]`, `mHoldLatchRows[seam]` and both threat identities and returns false,
so the window is re-put. What re-closes it is `askModel`: the re-put window's key is
still in `mAskCache`, the hold row's index is served with no round trip and no record,
and `takeHold` runs again on the seat's own retained answer. Same family at deck125:
`1789216935-ai_baka_deck125-0x55e82004b1d0-vs-ai_baka_deck126.jsonl` records 90 and 96
(`reopen_rows_moved`, `window_seq 42`), stderr `game-125v126-1789216932.stderr:1445-1447`
- the hold row taken again with no `-> chose` between them.

### Fix

`AIPlayerGPT::w81InvalidateHeldAnswer(const char * seam)`:

* `takeHold` records `mHoldAskKey[seam] = mLastAskKeyBuilt` (the key of the window being
  put right now; the priority seam records the empty string because it retains its answer
  in `mLastChoice` under an unchanged key rather than in a map).
* Every re-open (`reopen_new_lethal`, `reopen_same_rank_new_threat`, `reopen_rows_moved`)
  calls it **before** `writeHoldEventRecord`, and it erases that key from `mAskCache`,
  `mAskCacheSeq` and `mAskReplayRuns`, clears the repeat latch (`#W71-BS F4`: both
  re-serve paths or neither), and for the priority seam clears `mLastAskKey`.
* Counter `hold_reopen_answer_invalidated`; per-event field `answer_invalidated` on the
  `hold_event` record (1 = invalidated, 0 = nothing was retained, -1 = not a re-open).

Nothing is removed, capped or auto-answered - the re-opened window is ASKED.

### GREEN

* PARSETEST `[#W81-DK V3]`: INSTRUMENT (the lethal face really does retire the latch),
  RED-ON-BASE (the key is still in the answer cache after the re-open), GREEN (the key is
  gone before the hold check runs again), MUST-NOT-MATCH (an unchanged screen still
  stands, so the invalidation cannot become a per-window cache flush). Held set built the
  way the live seam builds it - `holdActionKeyRow` over `mLastMenuRows` plus the verdict
  markers.
* Live: probe on the fixed binary, seat `…deck196-0x55e9ad9d83a0`: `hold_events` 2,
  `hold_reopen_answer_invalidated` **1**, one `hold_event` with `answer_invalidated: 1`,
  stderr `the retained answer for that window is invalidated` **1**. Base binary: the
  line is absent and the counter does not exist.

### Prediction (falsifiable, NON-ZERO)

`hold_reopen_answer_invalidated > 0` on the next corpus, and **every** `hold_event` whose
`what` starts `reopen_` carries `answer_invalidated` (1 or 0) - the field names the event
it counted. Negative clause: a `reopen_new_lethal` `hold_event` at record seq N followed
by no prompt-bearing record before the next `hold_event` on the same `window_seq` must be
**0** (wave 80: 4, on one window, at the seat that died).

---

## V4 - an instant in hand cannot be cast in response

**Verdict: CONFIRMED.**

### RED

`1789216920-ai_baka_deck152-0x55a5a97d7280-vs-ai_baka_deck125.jsonl` **seq 101**
(`kind: priority`, turn 37, Main phase 1, `fallback: unparsed_reply` - the corpus's only
one). The prompt's own frame:

```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): opponent's Essence Scatter {1}{u} (instant) [spell] targeting Briarbridge Tracker
  2: your Briarbridge Tracker {2}{g} (creature 2/3) [vigilance] [spell]
Mana available: 6 total …
Your hand (2 cards): … Fateful Absence {1}{w} [instant] [no cast row now: you already
answered this phase's Casting decision, so this window does not offer it - it is
otherwise castable and its own casting window this turn has passed]
```

Reply: `CHOICE: Cast Fateful Absence targeting Essence Scatter` - a row that did not
exist. Seq 99 of the same seat is the Casting decision the clause refers to, answered
`Cast nothing right now` **over an empty stack**. 250 prompts carry the clause corpus
wide, 200 gating an INSTANT, 61 with a non-empty stack.

Mechanism (`src/AIPlayerGPT.cpp`): the casting menu's key is
`w79AskScopeKey(turn, phase, w79ContinuationDigestCast(candidates)) + w77KeyTailOf(tail)`
and `w79ContinuationDigestCast` carried **the rows and the untapped-source count and
nothing about the stack**, so the decline given over an empty stack keys equal to the
window that now holds a counterspell and is replayed from `mAskCache`. The render half
(`kHandCastAnsweredTagText` at `src/AIPlayerGPT.cpp:30710`, gated by
`w72CastAnsweredFactApplies(mCastAskTurn == turn && mCastAskPhase == phase, …)`) made the
same claim in the same wrong scope.

### Fix

* `w81CastDigestStackTerm(bool anyInstantSpeedRow, const std::vector<std::string>&
  stackObjectHandles)` - pure. The term is admitted only when the menu offers at least
  one instant-speed row, and an EMPTY stack contributes nothing **byte for byte**, so a
  sorcery-speed menu and an instant menu over an empty stack key exactly as wave 79 keyed
  them; the only windows re-keyed are the ones V4 names.
* `AIPlayerGPT::w81StackObjectHandles()` - the live collector. Identity is the engine's
  own object identity plus the object's class and its source's name, **never the rendered
  text**: two copies of one counterspell are two objects (the case that costs the window),
  a re-render of the same object keys equal. Sorted, so stack ORDER is not the identity.
* `markCastDecisionAnswered()` also stamps `mCastAskStack`, and both render sites go
  through `w81CastAnsweredStampMatches(stampTurn, stampPhase, stampStack, …)` so the tag
  can only claim the decision is answered over the SAME stack.
* Counter `cast_decision_reopened_new_stack`; per-record field
  `cast_reopen_reason: new_stack_object`, stamped at the SEND.

### GREEN

* PARSETEST `[#W81-DK V4]`: 10 pins - GREEN (a new stack object re-keys), RED-ON-BASE
  (with an empty stack the digest IS the wave-79 digest), MUST-NOT-MATCH (sorcery-speed
  menus and empty-stack instant menus omit the term byte for byte), identity-not-text
  (same object equal, two copies unequal), the stamp's three faces, and **key stability**:
  `w77KeyTailOf(tail)` contains no `stack=` (so the ask key tail, the async slot key and
  the hold-check key are untouched), `holdStillStands` over a held set built via
  `holdActionKeyRow` from `mLastMenuRows` still stands across a stack change, and
  `optionSetKeyOf` is a function of the rows alone.
* Live: probe on the fixed binary, `cast_decision_reopened_new_stack` **1 / 1** on the two
  seats, one record each carrying `cast_reopen_reason`, and **0** prompts carrying the
  cast-answered clause together with `ON THE STACK`.

### Prediction (falsifiable, NON-ZERO)

`cast_decision_reopened_new_stack > 0` with the matching count of records carrying
`cast_reopen_reason: new_stack_object`; and the clause
`[no cast row now: you already answered this phase's Casting decision` on prompts whose
board block contains `ON THE STACK` → **0** (wave 80: 61). `unparsed_reply` on a
priority window whose reply names a hand instant → 0 (wave 80: 1).

---

## V9 - send-time counters over-count and faces under-stamp

**Verdict: CONFIRMED, one boundary with two halves.**

### RED (corpus census, wave80/engine-seat.md §2(d) HIGH-4)

| counter | value | prompts carrying the line | records carrying the face |
|---|---|---|---|
| `crackback_verdict_lines_rendered` | 382 | 329 | 291 |
| `own_loop_verdict_lines_rendered` | 20 | 18 | 18 |
| `crackback_lethal_blocked_away` | 26 | 24 | 14 |

38 prompts carry `[crack-back verdict:` with no `crackback_verdict` field, **36 of them
`priority` records**; no record carries the field without the line.

Two mechanisms, both in the code rather than in the data:

1. **`sent` was a caller's literal.** `chooseOrderedAction` called
   `w80ApplyVerdictFacesAtSend(true, …)` one statement *before*
   `pollCompletionRetry(userMsg, content, "priority")`, and `askModel` called
   `w80SendFaces.apply(true)` one statement before its own poll. `pollCompletion` returns
   `kChoicePending` **without sending anything** when a request is already in flight
   (`slot->status == 1`), and returns 0 with an empty reply when the worker thread is
   refused or a wedged request is reaped. None of those is a send, and the caller cannot
   tell them apart.
2. **A suppressed window CLEARED a neighbour's live face.** `w79ApplyLoopFaceAtSend(false, …)`
   and `w80ApplyVerdictFacesAtSend(false, …)` cleared the shared face strings on every
   non-send exit - and the suppressed exit of one seam routinely falls between another
   window's send and its record, which is the shape of the 36 unstamped `priority` prompts.

### Fix

* `mTransportHandedOff` is set by the TRANSPORT: in `pollCompletion`, at the development
  stub and immediately after a successful `gptSpawnWorker`. Those are the only two places
  in this seat where a prompt is handed over.
* `w81SendDisposition(bool transportHandedOff, bool pollReturnedPending)` - pure, three
  states: `kW81SendHandedOff` (count and stamp), `kW81SendInFlight` (touch nothing, so the
  face staged at the real send survives to the record), `kW81SendSuppressed` (the guard's
  destructor; count nothing, stamp nothing). Both seams read it AFTER the poll.
* A suppressed window no longer clears a face. What stops a later record consuming one is
  the window ORDINAL stamped beside it (`mVerdictFaceWindow = mWindowSeq` at the send),
  consumed in `writeTransLog` only by the record that closes that window (and never by a
  `wall_miss`, which rides the in-flight window's number). A face whose window never wrote
  a record is dropped and counted: `verdict_faces_dropped_unrecorded`, closing the census
  exactly - `<lines_rendered> == records carrying the face + verdict_faces_dropped_unrecorded`.
* Two wave-79/80 pins asserted the superseded "and it CLEARS the face" half and are
  AMENDED in place, with the supersession recorded at the pin.

### GREEN

* PARSETEST `[#W81-DK V9]`: the three dispositions, a MUST-NOT-MATCH on the synchronous
  stub (a send whose poll does not return pending), and the mixed fixture the brief asked
  for - a cached decline, a suppressed hold window, a real send and its own polling tick -
  asserting `counter == rendered == stamped == 1` and that the four dispositions are each
  distinguishable.
* Live: probe on the fixed binary, both seats:
  `crackback_verdict_lines_rendered` **24** vs records carrying `crackback_verdict` **24**,
  and **13** vs **13**, with `verdict_faces_dropped_unrecorded` **0**.

### Prediction (falsifiable, NON-ZERO)

On the next corpus, for each of `crackback_verdict`, `stack_death_verdict` and
`own_loop_verdict`: `<counter>_lines_rendered == (records carrying the field) +
verdict_faces_dropped_unrecorded`, per seat, exactly. All three counters and the record
field counts stay **> 0** (wave 80: 382 / 36 / 20 and 291 / 33 / 18). Negative clause: a
record carrying the field whose prompt does NOT carry the literal → 0 (wave 80: 0, and it
must stay there).

---

## V15 - six wave-80 counters have no per-record trace

**Verdict: CONFIRMED.**

### RED

`ask_key_continuation_differs` 112, `hold_windows_skipped` 1,051, `plan_cast_steps_closed`
376, `mana_only_windows_skipped` 78, `main_phase_windows_skipped` 70,
`own_turn_windows_skipped` 722 exist only on the 42 `gameend` records; the wave-80 engine
seat records three sampling requests it could not answer ("I could not sample it. The
counter has no per-record or sidecar trace at all"). Separately, `hold_event.window_seq`
is the WINDOW ordinal while `seq`, `replayed_from` and `recovers_seq` are RECORD seqs, so
a deck125 hold at record 89 reads `window_seq 42` - the wave-77 lesson again.

### Fix

* `W81SkipTrace` (`include/AIPlayerGPT.h`) - pending/total maps, `note()` and `drain()`.
  These are SKIP counters: they fire on windows that write no record, so the trace they
  can carry is the DELTA since the previous record. `writeTransLog` writes the non-zero
  deltas as `skips_since_last`; `logGameEnd` writes the tail as `skips_after_last_record`.
  Identity, by construction and pinned: **sum(record deltas) + tail == the gameend counter**.
* All six increment sites call `mSkipTrace.note(<the counter's own name>)`.
* `hold_event` gains `window_record_seq` - the RECORD seq of the last prompt-bearing
  record (`mLastWindowRecordSeq`, set in `writeTransLog` and never by a `wall_miss`);
  -1 = no window record yet at this seat.

### GREEN

* PARSETEST `[#W81-DK V15]`: RED-on-base (nothing between two records to read), GREEN
  (deltas, non-zero only), MUST-NOT-MATCH (a drained delta is not re-reported), the sum
  identity, and an empty event name counting nothing.
* Live: probe, both seats - `records with skips_since_last` 30 / 47, and the trace totals
  equal the gameend counters exactly:
  seat `deck197` `{plan_cast_steps_closed: 17, ask_key_continuation_differs: 1,
  hold_windows_skipped: 33}` == its gameend counters;
  seat `deck196` `{plan_cast_steps_closed: 12, hold_windows_skipped: 181,
  ask_key_continuation_differs: 1}` == its gameend counters.
  Both `hold_event` records carry `window_record_seq >= 0`.

### Prediction (falsifiable, NON-ZERO)

On the next corpus, for each of the six counters and each seat:
`sum(skips_since_last[c] over records) + skips_after_last_record[c] == gameend[c]`, and
the number of records carrying `skips_since_last` is **> 0** on every seat. Every
`hold_event` carries `window_record_seq`, and every non-negative value **joins** to a
record with that `seq` in the same seat log (0 orphans).

---

## Gate

| leg | result |
|---|---|
| `make -B -f Makefile.sdl -j8` | clean link, 0 errors |
| suite, THREADS=1, detached unit `w81-DK-suite` | **1296 tests (0 failed), 76 AI tests (0 failed)**; `==Test Failed !==` 0, `==Test timed out` 0 |
| PARSETEST | **7132 passed, 0 failed** (base 7102, +30) |
| `check-ctor-init.py` | OK (118 files); the 55 pre-existing scalars outside AIPlayerGPT are the carried V18 docket |
| `check-reply-instructions.py` | OK (1 source, 37 guides) |
| `corpus-stats.py --selftest` | OK |
| `selfplay-harness.sh --selftest` | 34 checks, 0 failed |
| `git diff \| grep -c U+FFFD` | 0 |
| `reply-protocol.txt` / `kReplyProtocol` | untouched (`git diff --stat` on the file is empty; no protocol string changed) |

Files: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h`,
`projects/mtg/tools/w81-dk-window-probe.sh` (new).

---

## Weakest evidence - what I could NOT drive through the live path

The V9 defect is the one I could not reproduce live. Its RED is the wave-80 corpus census
(382 / 329 / 291 and the 36 unstamped `priority` prompts) plus a reading of the two code
sites; my offline probe's base leg happens to produce an EXACT census (16/16/16 and 8/8/8)
because the stub transport never leaves a request in flight across a tick and the probe
game produced no stale async drop, so the base binary looks innocent there and the fixed
binary's exact census (24/24 and 13/13) is a GREEN with no matching RED beside it - only
the PARSETEST disposition pins separate the two. The same gap applies to the V1 legend-rule
replay: neither probe leg re-put a legend-rule window, so the cache hit itself
(`146v162` askreplay seq 27, `replayed_from` 24) exists only in the corpus, and what my
live leg proves is the narrower fact that the fixed binary marks such a window
cache-ineligible at the live caller (3 stderr lines, 3 records) where the base binary
prints nothing. Two further limits: the probe's stub is not the model, so none of these
legs says anything about whether the re-asked window gets a BETTER answer - only the next
corpus can; and the V3 live leg exercised exactly ONE invalidation, on a `reopen_rows_moved`
event, not on the `reopen_new_lethal` shape that killed the deck125 seat, because the probe
board never assembled a lethal stack under a standing hold. I also did not verify that
pointer-derived stack handles cannot collide after a free-and-reuse within one window
(`w81StackObjectHandles` mixes the address with the object's class and its source's name to
make that improbable, not impossible); a collision would make two different stacks key
equal, which is the wave-80 behaviour, so it fails in the direction the fix improves rather
than a new one.
