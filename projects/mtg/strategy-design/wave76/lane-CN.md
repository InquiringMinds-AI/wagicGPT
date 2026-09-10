# Wave-76 lane CN — the window/hold and meter family (Q1, Q7, Q8, Q13, Q9)

Base: master `77b920612`, worktree `worktrees/lanes/w76-CN`, branch `w76-lane-CN`.
Corpus for every repro: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-003556-final`.

**Gates.** Seeded tree: PARSETEST 5954/0, suite 1289/0 + 76 AI/0.
After the lane: **PARSETEST 6008 passed, 0 failed** (+54), **suite 1289 tests (0 failed),
76 AI tests (0 failed)** single-threaded, detached unit, both failure strings counted
(`==Test Failed !==` 0, `==Test timed out` 0; none of the three known flakes fired).
`check-ctor-init` OK, `check-reply-instructions` OK (1 source, 37 guides).
`git diff | grep -c $'\357\277\275'` = **0**. Files touched: `src/AIPlayerGPT.cpp`,
`include/AIPlayerGPT.h` (both ASCII/LF — no byte-splice needed), plus the SUPERSEDED
rewrite in `wave75/lane-CM.md`.

Tools written this lane take the run dir as an argument and echo it on their first output
line (LESSON OF WAVE 75 (2)): `~/.gatelogs/w76cn/p13check.py`, `~/.gatelogs/w76cn/q13measure.py`.

---

## Q1 HIGH — the hold-check bracket measured against a window the model never saw

### Repro
`126v146` deck126, priority seam. seq 28 (t14) offers eleven
`Deal 1 damage with Staff of Nin targeting <...>` rows; seq 33 (t16) offers three Sorin
loyalty rows and nothing shared, and prints
`[hold check: every row above was also on the menu at the last window at this seam
(1 window in a row now) ...]`. 214 of 2,000 record-adjacent windows show that shape.
Second half, `162v152` deck162 Upkeep seqs 16 and 24: both print `1 row above is new`
over ONE Dictate of Kruphix row whose only delta is inside its `{reserve: ...}` group
(`leaves 2 sources` -> `leaves 3 sources`, `2 sources cannot pay 4` -> `3 sources cannot
pay 4`) — verified by reading both `options_text` lists out of the JSONL.

### Root causes (two, and the second was not the one the review named)
1. `holdReopenNote` updated `mLastMenuRows[seam]` at every window whose prompt was
   **BUILT**. The corpus built 3,402 windows the model never received (1,601 hold-skipped,
   1,801 replayed), so the bracket's subject was routinely an invisible menu.
2. `stripRenderAnnotationsLc` finds the **first** `}` after a `{`. An annotation that
   quotes a COST inside itself — `{reserve: ... Teferi's Puzzle Box {4} in your hand needs
   4 ... 3 sources cannot pay 4}` — is therefore stripped only as far as that `{4}`, and
   the rest of the sentence (which carries the board's untapped-source count) stays in the
   ACTION key. So the residual false-NEW was **not** "the key does not strip `{leaves}` /
   `{spends}` / `{reserve}`" (it does — they are stripped when they contain no nested
   brace); it was nesting.

### RED evidence on the seeded tree
* `holdActionKeyRow(seq16row) == holdActionKeyRow(seq24row)` — **FAILS on base**, the two
  keys differ by the un-stripped `{reserve:` residue. Now GREEN.
* The memory policy: `W76HoldMemory` carries the wave-75 policy verbatim as
  `commitAtBuild = true`, so the same three-window sequence is pinned side by side
  (`#W75-CI` P1b's `HoldRowKeyFn` idiom). Under the wave-75 policy the third window prints
  `every row above was also on the menu`; under the shipped policy it prints
  `1 row above is new`. Both assertions live in PARSETEST, so the defect cannot be
  reintroduced silently.

### Fix
* `W76HoldMemory` (header) replaces the four loose maps. `w76HoldReopenNote` **stages** the
  window's keys; `w76HoldWindowAsked` commits them, `w76HoldWindowNotAsked` drops them.
* `AIPlayerGPT::writeTransLog` commits at the top — before the logging-off early return —
  because whether the bracket's referent is true must not depend on whether a translog is
  configured. That needed a new `mWindowSeq`, a record counter that moves with or without
  logging; `holdNoteSameWindow`'s `#W62-fix` guard now reads it instead of `mTransSeq`
  (with logging off `mTransSeq` never moved, so the note froze after one window — a
  pre-existing defect this closes).
* The three cast-seam and two priority-seam no-ask exits call `w76HoldWindowNotAsked`.
* Wording: `at the last window at this seam` -> `at the last window I asked you at this
  seam`, both halves (LESSON OF WAVE 75 (1)).
* `w76StripBalancedAnnotationGroups` strips BALANCED `{...}` groups (mana pips kept by
  `isManaSymbolBody`) before `stripRenderAnnotationsLc`, **only inside `holdActionKeyRow`**.
  `[cost: ...]` still survives (`#W75-CM` F2).

### GREEN
PARSETEST +20 for Q1. Key-stability pin set, all in the block:
held set built the way the LIVE seam builds it (`holdActionKeyRow` over the same rows the
seam feeds `mLastMenuRows`) identical across the two windows; `holdStillStands` therefore
stands; async slot key is a pure function of the ask tail; the two windows' option-set keys
still differ (see residual); `optionSetKeyOf` deterministic; MUST-NOT-MATCH on a different
card, on a different `[cost:]`, and on an unbalanced brace.

### Prediction (falsifiable, with a pin for the negative)
Next corpus: **0** windows print `every row above was also on the menu at the last window I
asked you at this seam` where the previous RECORD at that seam shares no key with a current
row (wave 75: 214 of 2,000), and **0** print `N rows above are new` where the previous
record's key set already held every row (wave 75: 6). NEGATIVE clause and its pin: the fix
must not silence the bracket on the unsuppressed path — a seat every one of whose windows
is asked must produce byte-identical brackets under both policies, which is pinned
(`#W76-CN Q1 NEGATIVE every window asked -> the two policies are byte-identical, run
included`), and the run must still climb (pinned at `(1 window in a row now)` for the
second asked window). If the next corpus shows the bracket rendering but the "seen" /
"new" ratio collapsing to ~0% "seen", the fix has over-corrected and the staging is being
dropped on windows that WERE asked.

---

## Q7 MED — `main_phase_windows_skipped` is NOT 36% false; the instrument was one turn out

### Repro and the correction
`146v125` deck146 stderr: `own main phase (turn 3, phase 4) ENDED with NO casting window`,
against that seat's translog **seq 5** = `Casting decision (Main phase 1, YOUR turn)` at
turn 3. The stderr printed `mMainSkipPendTurn`, i.e. **`observer->turn` raw**; every
translog record prints `translogTurn(observer->turn)` = `observer->turn + 1`
(`AIPlayerGPT::translogTurn`, src:28082). The cross-reference was therefore one turn out.

`~/.gatelogs/w76cn/p13check.py` reproduces the wave-75 numbers exactly and then re-runs the
same comparison with the correct mapping:

```
counted skip traces: 94
FALSE under raw-turn match      : 34 (36.2%)      <- wave-75 engine-seat MED-1, reproduced
FALSE under translogTurn=raw+1  : 0  (0.0%)
turns carrying a count: 48 ; (4,10) pair turns: 46
```

**0 of 94 are false.** The `(turn,4)/(turn,10)` pair signature (46 of 47/48) is therefore
not the overwrite's fingerprint any more: it is the expected shape of a seat whose
instant-speed arm answers BOTH of its own main phases on a turn.

### Fix
* The plumbing is lifted into `W76MainPhaseSkipState` + five free functions
  (`w76MainSkipNeedsFlush`, `w76MainSkipArm`, `w76MainSkipCastingOffered`,
  `w76MainSkipHoldSuppressed`, `w76MainSkipFlush`); the three member methods now call
  exactly those, so a fixture drives the member's own code rather than a copy.
* The trace prints the RECORD's turn first and the observer's turn beside it.

### GREEN
PARSETEST drives two main phases of one turn through the real calls: arm main 1, offer,
arm main 2, offer, flush -> **counted 0**; a main phase that offers nothing -> counted 1;
a phase the seat's own hold latch closed -> unchanged; an offer at main 2 does NOT exempt
main 1 -> counted. Plus the instrument pin `translogTurn(3) == 4`.

### Prediction
Next corpus: cross-referencing the stderr's new `turn N as the records number it` against
the translog gives **0 false counts of 100%** of the traces, and `main_phase_windows_skipped`
is quotable for the first time. NEGATIVE clause + pin: the meter must not be silenced —
if the next corpus reports `main_phase_windows_skipped: 0` in every game, the arm has been
broken, not fixed; the pin `#W76-CN Q7 REPRO a main phase that ends having offered nothing
is still counted once` is the guard.

---

## Q8 MED — nine forced closes armed, superseded, and paid a second decode

### Repro
Nine `forced_close_unrecorded` (`125v123` casting x2, `125v130` casting x2, `162v123` land
x1, `152v123` casting x1, `162v130` casting x3), ~80 s of ~20 KB decode each, no decision
lost (0 fallbacks, 0 `phase2_answer_missing`, every neighbouring async drop
`[outcome: re-asked]`).

### Fix
`w76ForceCloseArmAllowed(parkArmed)` — a close is not armed while the **other** arm's close
is still outstanding in the park (the brief's literal bound: "unless the first is still
live"). Refusals are counted on the gameend record as `force_close_arms_refused`. The live
arm's own outstanding close is NOT refused: that is this window superseding its
predecessor, counted as unrecorded exactly as before, and the accounting identity is
untouched.

### GREEN
PARSETEST replays the interleaving through the PRODUCTION swap: land arms -> casting takes
the live slot and the land close PARKS -> the casting arm's close is refused ->
`gptForceCloseOutstanding` is still 1 (the identity is untouched) -> the land arm comes back,
its close is live again and the arm is allowed. NEGATIVE: with the park empty the arm is
always allowed — this is a bound, not a cap on rescues.

### Prediction
Next corpus: `forced_close_unrecorded` falls and `force_close_arms_refused` accounts for the
difference, with the gameend identity
`recovered + missing + unrecorded == closes` still holding in every game. NEGATIVE clause
and its pin: the bound must not cost a decision — if any game shows a `reasoning_forced_close`
fallback or a non-zero `phase2_answer_missing` alongside a non-zero
`force_close_arms_refused`, the refusal is eating rescues and must be reverted; the
`#W76-CN Q8 NEGATIVE with the park empty the arm is allowed again` pin is the guard on the
common path.

### The CM F5 rewrite
`wave75/lane-CM.md` §F5's prediction second clause ("a non-zero value now means a genuinely
lost close") is struck through and marked **SUPERSEDED** with the evidence: the identity
held in 21 of 21 games *with* 9 unrecorded, so a non-zero value is a WASTED decode, not a
lost decision.

---

## Q13 MED — a three-row menu re-put ten times across one opponent turn

### Repro
`152v125` deck152 seqs 25-34 (t12, Upkeep -> Cleanup): rows byte-identical, each a full
model call, ~13 min. `identical_ask_answers_reserved` fired **0** times inside it because
`repeatAskKey` carries the PHASE.

### Fix (measure + annotation; no window collapsed, no option removed)
* `w76CrossPhaseListKey(turn, decision, rows)` = the repeat key with the phase taken out.
* `mCrossPhaseAsks` records ONE ASKED window per key (phase name, `mWindowSeq`, whether the
  answer declined) — written where the model's own answer is latched, so the bracket refers
  to ASKED history (LESSON OF WAVE 75 (1)). Turn-scoped: the map is dropped when the turn
  changes, so it cannot grow through a 77-turn game.
* On a byte-identical re-put at a DIFFERENT phase after a decline:
  `crossphase_identical_reputs` increments and the prompt-only bracket
  `[this exact list was put to you N windows ago at <phase> and you declined; nothing on the
  board has changed]` is appended to `promptOnlyNote` — the same channel as the declined
  count and the hold check, inserted into the user message after the option list and never
  into `keyTailStr`/`askKey0`.

### Offline measure of the population (upper bound)
`~/.gatelogs/w76cn/q13measure.py` over the wave-75 corpus: **273** cross-phase
byte-identical re-puts answered after a decline across the 21 games; worst seat
`125v123` deck123 **108**, and `152v125` deck152 **13** (the named run).

### GREEN
PARSETEST: key determinism; MUST-NOT-MATCH on a different turn and on one differing row;
the REPRO pin that `w76CrossPhaseListKey != repeatAskKey(with phase)` — the difference that
made the run invisible; the bracket's exact bytes; singular/plural; two negatives (nothing
measured, nothing claimed). **Key-stability pin set** (LESSON OF WAVE 74 — the bracket
carries a board-derived number): the note is a `\n[` bracket, `stripNarrationDecoration`
drops it, the async slot key WOULD move if the bracket reached the ask tail (poison test)
and the tail carries no part of it, and the option set is unchanged.

### Prediction
Next corpus reports `crossphase_identical_reputs` non-zero on the deck123 and deck152 seats
(offline upper bound 273; the live counter is scoped to `askModel`'s seams and to windows
the seat answered, so expect a fraction of that), and the bracket appears on exactly those
windows. NEGATIVE clause + pin: no window is removed — every seat's option counts and the
per-window answerable row set must be unchanged from wave 75, pinned by
`#W76-CN Q13 KEY PIN the option set is untouched: three rows in, three rows answerable out`
and by the async-slot poison pin (if the ask key moves, the bracket has leaked into a key
and the deadlock breaker will re-ask answered questions).

---

## Q9 MED — the counter's exemptions, its per-record field, and the stranded widening

### What the wave-75 seat asked for, and what the corpus says
* **"The counter is gameend-only so no seat can sample it" — REFUTED.**
  `grep -ho '"plan_names_uncastable_zone_card": *"[^"]*"' *.jsonl | wc -l` = **68**
  per-record string fields against **110** gameend integers (one per seat-game). It has
  been per record since `#W75-CL` (P17); the request is already paid.
* **"`plan_names_stranded_card` UNDER-counts; widen the match" — REFUTED at the extractor.**
  All **67** option rows corpus-wide that carry `STRANDS` use the exact marker
  `VERDICT: taking this row STRANDS <name> this turn` — 0 rows use another shape. Of the 10
  TAKEN rows carrying it, 5 fired and the 5 that did not are **correct non-fires**: the
  plan does not name the stranded card (`146v126` seq 20/36, `123v146` seq 35, `146v123`
  seq 12, `123v130` seq 140 — each quoted plan read and checked). The reviewer's
  "in three of the four the PLAN's next step names the very card" does not survive reading
  the plans: `123v146` seq 35 names "tutor for Intruder Alarm", not Idyllic Tutor.
* **The REAL under-count is structural and no corpus could see it:** the two measures were
  an `if`/`else`, so a window whose taken row DID carry a reserve verdict was never
  examined for the wider class at all. That is now fixed — the two are independent, on
  their own fields, and are still never summed.

### Fix (measure only)
* `w76SelfRecursiveZoneScript` — a card whose own zone script moves it back to a castable
  zone (`moveto(hand|myhand|ownerhand|battlefield|mybattlefield)`, `flashback`, `retrace`)
  is not a stranded plan. Hammer of Bogardan's `autograveyard={2}{R}{R}{R}:moveto(hand)
  myUpkeepOnly` (verified in `bin/Res/sets/primitives/mtg.txt:52046-52052`, with the
  `text=` line) is 12 of the 47 re-derived triggers.
* `w76CastableFromDeadZone` — `CANPLAYFROMEXILE` in exile; `CANPLAYFROMGRAVEYARD` /
  `TEMPFLASHBACK` in the graveyard (the `Cast Sphinx's Revelation [from exile]` class).
* Both exemptions consulted inside `w75PlanNamesUncastableZoneCard`; the `else` removed.

### GREEN
PARSETEST: the Hammer line, flashback and retrace exempt; an ordinary `{t}:damage` ability,
an empty script, and `moveto(graveyard)`/`moveto(exile)` do NOT (four MUST-NOT-MATCHes, so
the counter keeps the population it exists for); `NULL` card exempts nothing; the marker
shape; and the `146v123` seq 12 correct non-fire pinned as a MUST-NOT-MATCH.

### Prediction
Next corpus: `plan_names_uncastable_zone_card` loses the whole self-recursion population —
**0 records name Hammer of Bogardan** (wave 75: 12 of the 47 re-derivable) and 0 name a card
the seat may cast from exile — while the counter stays non-zero (wave-75 estimate ~55
genuine of 68). NEGATIVE clause + pin: the exemptions must not empty the counter — if the
next corpus reports 0 per-record `plan_names_uncastable_zone_card` fields across all seats,
the predicate is over-matching; the four MUST-NOT-MATCH pins are the guard, and the
independence change should make the field appear on windows it could never appear on before
(a record carrying BOTH `plan_names_stranded_card` and `plan_names_uncastable_zone_card`
was structurally impossible in wave 75 and is now possible).

---

## Weakest evidence

1. **Q1's 214 is still an upper bound and the engine's own reference set is still not
   logged.** My repro reads the same `options_text` the wave-75 seat read and the same
   bracket text; neither of us can see what `mLastMenuRows` actually held at seq 33, because
   the engine does not record it. **Recommendation, as the brief anticipated: log the
   hold-check referent's seq on the record** (`hold_check_ref_seq`) — one integer per record
   makes the whole class decidable from the translog instead of re-implemented in Python.
   Until then the wave-76 prediction for Q1 is verified against a Python re-implementation
   of `holdActionKeyRow`, which is exactly the instrument the wave-75 seat flagged.
2. **Q1's live path is not observed.** The staging/commit is pinned as a pure unit and the
   suite is green, but no corpus has run: whether `writeTransLog`'s kind->seam mapping
   (`priority`->priority, `ask`->cast) commits the right pending on every real path is
   argued, not measured. The known residual: a cast window closed with no record, followed
   by an unrelated `ask` record before any other record, would commit the cast pending. The
   three cast-seam no-ask exits are cleared explicitly to bound it; the remaining hole is
   the replay caches inside `askModel`, which leave the pending staged (harmless — it is
   overwritten by the next build at that seam) unless such an unrelated `ask` intervenes.
3. **Q7 rests on a reading of one instrument against another.** The 0-of-94 result is only
   as good as my `Casting decision (Main phase N, YOUR turn)` header extraction and the
   phase mapping (4 = FIRSTMAIN, 10 = SECONDMAIN, `include/MTGDefinitions.h`). I reproduced
   the wave-75 number exactly (34 of 94, 46 pair turns) before applying the correction,
   which is the strongest available check that the two runs differ only in the turn offset —
   but a second, independent confirmation of the meter would be a live game whose stderr and
   translog are read together under the NEW trace.
4. **Q8's bound may never fire.** The nine wave-75 arms are named by arm ("casting x3") but
   the stderr does not say whether the PARK held a close at each. If all nine were same-arm
   re-arms, `force_close_arms_refused` will read 0 next corpus and the waste will be
   unchanged — which is itself the finding, and points the next lane at the same-arm case
   (which cannot be refused without costing the rescue).
5. **Q13's 273 is an offline upper bound over a different code path.** It counts every seat
   record of kind `ask`; the live counter only sees `askModel`'s windows and only after the
   seat answered one. The two numbers will not match and should not be read as a regression
   if the live one is smaller.
6. **The option-set key still moves with a nested annotation.** `optionSetKeyOf` uses the
   same `stripRenderAnnotationsLc` early-terminate, so the two `162v152` rows key
   differently there. I did NOT widen the shared stripper — that would move the ask key and
   is out of this lane's scope — and pinned the current behaviour instead
   (`#W76-CN Q1 KEY PIN the OPTION-SET key moves with the RENDER`). It is a live candidate
   for a later lane, with the pin as the tripwire.
