# Wave-74 lane CG — the ask-cache key is unstable (O11 regression)

Base `cfcc37a8f` (gate 2: 1285/0, 76 AI/0, PARSETEST 5748/0), worktree
`worktrees/lanes/w74-CG`, branch `w74-lane-CG`.

Gate on this branch: `make -f Makefile.sdl -j8` clean, both link guards OK
(`check-ctor-init: OK (118 files)`, `check-reply-instructions: OK (1 source, 37 guides)`).
**Suite THREADS=1, detached unit `w74-CG-suite`: 1285 tests (0 failed), 76 AI tests
(0 failed)** — `==Test Failed !==` 0 and `==Test timed out` 0, no flake needed a solo
rerun. **PARSETEST 5,760 passed / 0 failed** (base 5,748 — **+12 cases**).
`bash -n tools/selfplay-harness.sh` OK; `--selftest` 4 + 13 + 7 + 30 checks, 0 failed.
`git diff | grep -c $'\357\277\275'` = **0**.

---

## Root cause

`#W74-CE` (O11) put the re-ask clause **on the decline row**:

```
3. Cast nothing right now (combat comes next this turn) {this same question will be asked
   again this turn: … you have already declined this exact list 295 times this turn}
```

That clause carries **a count that rises with every answer**, and both ask seams build
their cache key from the rendered option list the row sits in:

* priority seam — `askKey = boardKey + tailStr` (AIPlayerGPT.cpp, the `tail` the row 0
  clause is streamed into at the `kPassPriorityRowText` line);
* casting/ask seam — `askKey0 = situationPrefill + tailStr`, where `tailStr` is
  `joinNumberedRows(options)` over the menu the clause was appended to;
* and **the same bytes are handed to `assemblePrompt`'s `keyTail`**, which is one half of
  the async **slot** key (`asyncSlotKeyOf`).

So every answer minted a fresh question for a window that had not moved. `mAskCache`
could never hit again, `mAskAnswerReserved` was never set, the decline count therefore
kept incrementing, and the seat re-asked the same window until the wall.

The `#W74-CE` comment's premise — "a `{...}` group, stripped from every key by
`stripRenderAnnotationsLc`" — is true of the **option-set key** (`optionSetKeyOf`, which
is why the count did not reset itself) and **false of the ask key**, which is the raw
rendered tail. That is the whole defect: the right guard on the wrong key.

### Evidence (corpus, base binary)

Run `matchups-20260909-015553` (killed at 8 h under invariant 00; 11 of 21 games frozen
at turns 3–15), seat `1788936956-ai_baka_deck123-…`:

* 386 ask records, **384 of them the same turn-4 Main-1 casting decision**; every one a
  real model call (`latency_ms` > 0 on all 386, `latency_ms == -1` count **0**, no
  fallbacks), 60–100 s each;
* asks 300 vs 301 diff to **three lines**: the carried PLAN, the prompt-only
  `[you declined this exact list N …]` note, and — inside the numbered list — the decline
  row's tag, `295 → 296`. Only the third is in the key;
* replayed on the real record bytes: taking the numbered-row region of each prompt,
  `base keys equal: False`, `stripped keys equal: True`;
* gameend counters, summed over every seat: `ask_replays_reserved` **3** and
  `identical_ask_answers_reserved` **1** over 20 gameends. Wave-73's
  `matchups-20260907-225637-final`: **1262 / 999** over 42.

## Fix

`kDeclineReaskTagHead` (the clause's literal head, now shared by the builder so the two
cannot drift) + `stripDeclineReaskTags()`. Every **key** is computed from the rendered
tail with that one clause removed; the **render** is untouched, so O11's intent is intact
— the row still says the question comes back and still says how many times the model has
declined it. Applied at: the priority seam's `askKey`, the ask seam's `askKey0` (and its
re-ask variant), the `keyTail` both seams hand `assemblePrompt` (the async slot key), and
the two ask-seam retry launches, so a corrected leg's slot key matches the tail the next
tick rebuilds. Nothing is removed, capped or auto-answered.

## Deliverables

**(a) The clause leaves the ask key, O11's tag preserved** — done as above. PARSETEST
pins that the tag's own text is unchanged and that the row still parses by its short name
(the pre-existing `#W74-CE O11` cases all still pass).

**(b) PARSETEST pin: two windows differing only in the count are ONE askKey** — RED on
base (the function does not exist there; the base composition is `boardKey + tailStr`
with the tag in it, and the corpus replay above shows those bytes differ). GREEN:
8 new cases, including the seq-300/301 repro, the MUST-NOT-MATCH that a real change to
the row is still a fresh question, both seams' rows in one tail, a byte-identical return
on an untagged tail, and an unterminated clause truncating nothing.

**(c) Fixture: the casting decision reaches the model at most twice per identical state,
and the game reaches combat** — `projects/mtg/tools/window-loop-probe.sh` (new). **Not a
suite fixture, and cannot be one**: the shape needs a seat that asks a MODEL, and the test
suite has no endpoint, so its GPT seats fall back to the heuristic and the ask cache is
never exercised (the same reason lane CD's O2 has no suite fixture). The model is
`tools/gpt-stub-server.py --prefer "Cast nothing right now"` — it always takes the plain
decline row, which is exactly what the corpus seat answered 384 times. One pinned
selfplay game, deck123 vs deck126, `WAGIC_AI=gpt`, 150 s cap, five runs per binary:

| binary | runs that never finished | worst declined-list count | model calls |
|---|---|---|---|
| base `cfcc37a8f` | **2 of 5** (+1 of 1 at a 240 s cap) | 17, 17, 17, **2028**, **2440** (240 s: **2803**) | 1281–5324 |
| this branch | **0 of 5** (+2 of 2 at 240 s) | **8, 8, 9, 9, 9** | 783–1307 |

Every fixed run reached combat and played the game out (turn 102–106, `winner=1`); no base
RED run produced a `WAGIC_SELFPLAY_RESULT` line at all. The count ceiling is the sharp
discriminator: base **17–2803**, fixed **8–9**, wave-73 corpus **≤3**.

**(d) After `Cast nothing` with the land arm parked, `decisionPending()` is false and Act
passes — and WHICH cause is load-bearing.** `#W74-CD`'s pass gate (`decisionArmed()`) was
the second candidate. Its decision is now a pure helper, `w74RetryArmedHold()`, called by
`decisionArmed()` unchanged in behaviour, and pinned: a **parked** land-drop leg the seat
cannot re-enter holds the pass at 0 ms and 1999 ms and **stops holding at
`kRetryArmedHoldMs` (2000 ms)** — `decisionArmed()` false, `decisionPending(0)` already
false (the async slot is empty after the answer is consumed), so `AIPlayerBaka::Act`
passes. **The load-bearing cause is the unstable ask key, not the retry park.** The
A/B above isolates it: the binary that produced the GREEN column carried the key fix and
*nothing else* — CD's per-arm slots and the pass gate are byte-identical in both columns,
and the game finished. The park is bounded by construction and the pin proves the bound
fires.
One interaction is worth recording, and is pinned as a MUST-NOT-MATCH: the armed prompt
is built from the same rendered tail, and `w74RetryArmedHold` measures its bound from the
moment that prompt was **first seen** — so an armed prompt carrying the rising count
resets the bound every window and never releases (20 consecutive windows, never released
in the pin). Whenever a retry *did* arm on this seam, the unstable tail also disarmed CD's
time bound. That is a second, independent reason the count may not ride a keyed tail.

**(e) Harness WINDOW LOOP tripwire** — `window_loop_verdict` / `window_loop_sweep` /
`window_loop_selftest` in `tools/selfplay-harness.sh`, wired into the 45 s supervisor
beside `pilot_stall_sweep` and into `--selftest`. Predicate: for every **live** seat log
of this run (no `gameend`), read the **newest** decision record's prompt; if any carries
`declined this exact list N times` with **N ≥ 40** (`WAGIC_WINDOW_LOOP_N`), print the
`== WINDOW LOOP` banner, touch `$OUTDIR/WINDOW-LOOP` and SIGTERM the harness. It is the
complement of PILOT STALL, which reads endpoint *silence* and was correctly OK for the
whole 8 h: here the model answers every decision, on time, with reasoning — and the window
still never closes. Selftest: 7 checks (the wave-74 shape → `LOOP 295`; a wave-73 count →
`OK 3`; a finished game's tail → `OK 0`; a high count the seat has moved on from → `OK 0`;
an older run's log → `OK 0`; the threshold at exactly 40 and at 41). Real-data check:
against the killed run it reads `LOOP 382 1788936956-ai_baka_deck123-…jsonl`; against
`matchups-20260907-225637-final` it reads `OK 0`.

## Prediction (falsifiable, next corpus)

Summed over all gameends, `ask_replays_reserved` and `identical_ask_answers_reserved`
return to the wave-73 order of magnitude — **≥ 20 per gameend** on both (wave 74:
0.15 / 0.05) — and **no seat log carries a `declined this exact list N` above 15**
(wave 74: twelve seats at 100–382). If the counters recover but a seat still walks a
count past 40, the WINDOW LOOP tripwire stops the run inside a minute and names the seat;
the remaining cause is then the window's own re-open condition, not the key.

## Weakest evidence

* **The probe is stochastic.** The loop reproduced on 2 of 5 base runs at 150 s (3 of 7
  counting the 240 s run); the other base runs finished. A single GREEN probe run
  therefore proves nothing, and the honest discriminator is the **count ceiling** (base
  17–2803 vs fixed 8–9 across every run, including the base runs that finished), not the
  finish/no-finish column. There is no seed control for selfplay games, so the fixture
  cannot be made deterministic without adding one.
* **Base runs that finished still walked the count to 17.** The fix lowers that to 8–9,
  but neither number is wave-73's ≤3, so the underlying question — why the same casting
  window is re-put 8 times in one turn at all — is NOT closed by this lane. It is O10/O13
  territory (which arm re-opens the window), and the next corpus should be read for it.
* **The stub answers instantly.** It reproduces the CONTROL-FLOW loop faithfully but says
  nothing about the wall-clock arithmetic that actually killed the corpus (60–100 s per
  call); the 8 h figure is read off the corpus, not the probe.
* **No live-endpoint validation.** Nothing here was run against Spark; the claim that the
  next corpus's games finish rests on the stub A/B plus the counter arithmetic.
* **The `keyTail` change touches the async slot key on EVERY seam that routes through
  those two functions**, not only the looping one. It is strictly a removal of one
  volatile clause, and the suite and PARSETEST are green, but no corpus has yet exercised
  a stale-drop-heavy path with the new slot key.
