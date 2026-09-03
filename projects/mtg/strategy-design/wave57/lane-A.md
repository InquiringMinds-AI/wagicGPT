# Wave-57 lane A — the second async slot and record fidelity: D5, D4, D16, D31

Base: `master 4d4ea0bdf` (>= `7ee3a51e8`; gate binary seeded from `4366e8eec`), worktree
`worktrees/lanes/w57-A`, branch `w57-lane-A`. Two files: `projects/mtg/src/AIPlayerGPT.cpp`
and `projects/mtg/include/AIPlayerGPT.h`, both spliced as bytes (python `rb`/`wb`).
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. Every comment tagged `#W57-A (Dn)`.

**Base gate re-run on THIS worktree** (`~/.gatelogs/w57A-base-{parsetest,suite}.log`, seeded
binary, 4 G memory cap): PARSETEST **2736 / 0**; suite **1241 tests, 1 failed
(`lifeline.txt`), 55 AI / 0 failed, 0 timeouts** — inside the known concurrency-only pair,
so the brief's baseline is confirmed and no third failure is carried in.

---

## Corpus arithmetic this lane re-derived (wave-56 corpus, read-only, python over JSONL +
## the 21 stderr)

Every number below was re-derived on disk before any code was written; two of them **change
the docket's own remedy**, so they are stated first.

| fact | on disk |
|---|---|
| `dropping stale async answer` | **65** |
| next ask after the drop | **44** `Land drop:` / **21** `Casting decision` |
| line immediately before the drop = `AIPlayerBaka: Mana cost is NULL.` | **32** |
| **of those 32, next ask = `Land drop:`** | **32 of 32** |
| D18 rescue (`prompt text drifted`) | **8** |
| single-row takes (`priority` + `ask`) with `options_text` in range | **2,416** |
| takes whose `chosen_text` != the row it names | **124**, *all at the `priority` seam* |
| of those, `chosen_text` a strict PREFIX of the row | **112** (the tail-less HOLD row) |
| `ask`-seam mismatches | **0** |
| records with `choice: -1` and NO `chosen_text` | **4** decision records + 2 `wall_miss` = **6** |
| stderr lines carrying `N windows held this game` | **1,236** |
| sum of `hold_windows_skipped` over all 42 gameend records | **1,236** (30 non-zero, max 153) |

**Finding 1 (D5).** The opponent-fetch-land displacement class is *entirely inside* the
cross-arm class: **all 32** drops preceded by the Baka opponent's own land auto-tap are drops
whose next ask is a `Land drop:`. So the arm split covers them, and the docket's alternative —
widening the board half of `slotKey` by excluding an opponent's non-combat land arrival —
**is not needed for this class and is not shipped**. That is the conservative direction: the
docket's own Ask says the second slot "is cheaper than widening the board half of `slotKey`
and does not risk consuming a genuinely stale answer", and widening the board half is the
only one of the two that can consume an answer computed against a board that has moved. The
21 same-arm residual is instrumented instead (below) rather than guessed at.

**Finding 2 (D31).** The docket's premise is false: the stderr companion **already exists**
and is exact — 1,236 lines carrying a running `N windows held this game`, equal to the sum of
`hold_windows_skipped` across all 42 gameend records. The harvest that reported "no stderr
companion" keyed on the line's PREFIX (`AIPlayerGPT: holding priority at the <seam> seam`)
and never read the parenthetical. What was genuinely missing is stated below.

---

## D5 (HIGH) — a SECOND async slot: the land-drop arm and the casting arm

`mAsyncLandState` joins `mAsyncState`. `pollCompletion` reads the arm off `mPromptTail` — the
question-and-options tail `assemblePrompt` wrote for THIS ask, in this same tick — through the
pure `asyncLandArm(seamTail)` (`"Land drop: "` as a **prefix**, not a search), and works
against that arm's own slot for the whole poll: consume, rescue, drop, streak, launch.

**Concurrency is unchanged.** `asyncBusy()` is now true while EITHER arm has `status == 1`,
and the launch path returns `kChoicePending` when it is. The second slot buys *storage*, never
a second spend: at most one round trip is ever in flight, exactly as before, and every reader
of `asyncBusy()` (the decision gate, the stall watchdog) sees the same thing it saw.

**No guarantee is weakened.** A stored answer is still only consumed when its slot key —
`(force-close tag, turn, phase, the byte-exact seam tail, the board serialization)` — matches
at the poll; the D18 rescue is untouched and still fires on the same condition. What changed
is only that a land-drop ask no longer *reaches into* the casting arm's storage to throw its
answer away. No window is removed, no cap is added, nothing is deleted.

**The drop's own classification (the instrument for the residual 21).** The existing
`AIPlayerGPT: dropping stale async answer` line keeps its leading literal byte-for-byte — every
existing census still counts the same events — and gains `(<arm> arm; the <what> moved)`, where
`<what>` comes from the pure `asyncSlotDriftKind(oldKey, newKey)`: `same slot` /
`question (or turn/phase)` / `board` / `question and board` / `unknown`. The wave-56 residual
had to be classified by reading the NEXT line of a 40 KB stderr; the next corpus reads it off
the drop. (The label says "question (or turn/phase)" and not "seam" because turn and phase sit
in the same half of the key as the tail — the honest name for what that half proves.)

**Prediction.** `dropping stale async answer` **< 20** corpus-wide (was 65); **0** drops whose
next ask is a `Land drop:` (was 44 of 65, 32 of them the Baka land auto-tap class); the D18
consume line still fires. **Falsifier:** drops stay >= 40 with the `Land drop:` next-ask class
intact -> the arm is not what displaced them and the item becomes the board half after all.
**Second falsifier, specific to the split:** a rise in `AIPlayerGPT: ... pending` stalls or a
game that parks -> the global `asyncBusy()` launch gate is starving one arm and the two slots
need two round trips, which this deliberately does not buy.

## D4 (HIGH) — `chosen_text` IS the rendered row, byte for byte

The priority seam keeps two parallel lists: `shownLines` (the PURE line — the de-dup, decline
and hold key, deliberately without the volatile clauses) and `renderRows` (what
`joinNumberedRows` actually printed). Wave 56 logged the first. Three consequences, all
measured above: the HOLD row lost its `{taking this row skips ...}` tail (112 of the 124), the
last-offer and upkeep-animation clauses were dropped from the take (the other 12), and a repeat
row recorded its BASE action's text with `" xN"` glued on.

`renderRows` is now extended through the repeat rows and the hold row, so it is parallel to
`shownLines` for every row the model can press, and **both** `chosen_text` and the record's
`options_text` are taken from it at both `priority` write sites. `shownLines` itself is
untouched — the decline allowance, the hold key and the prose-rival extractor all still read
the pure line, which is what they were built to read. The repeat count moves to its own field,
`repeat_n`, so the row text stays byte-equal.

Second face: `writeTransLog` now writes `chosen_text` on **every** record, `choice: -1`
included, and `refusedChosenText` gained a `-1` arm (`<refused: no answer executed>`) so a
refusal is never an absent field. `choice == 0` still records `pass`: it is the pass, not a
numbered row, and it is outside the census.

**Scope stated, not silently narrowed.** The MULTI-SELECT seams (`attackers`, `blockers`,
`reveal`, `bottom`, `discard`) record a comma-joined list of the cards they actually sent —
their `choice` is a count/mask, not a row index, so "the rendered row" has no referent there.
They are unchanged and are outside the prediction's N.

**Prediction.** Single-row takes (`priority` + `ask`) whose `chosen_text` is not byte-equal to
`options_text[choice-1]`: **0 of N** (was 124 of 2,416); records with `choice: -1` and no
`chosen_text`: **0** (was 6); an exact-match HOLD-take census reads **320 of 320**, not 208.
**Falsifier:** any residual mismatch at the `priority` seam -> a third list is being rendered
that neither vector holds.

## D16 (MED) — the BROAD census is stamped on the BROAD condition

The wave-56 code stamped `decision_reversed_in_prose` on `planChoiceConflict` — the NARROWED
condition — so the broad column read **0** while `plan_choice_conflict_narrowed` read **14**,
and "0 conflicts" was indistinguishable from "0 counted" (silent-instrument class, skill #263).
One condition changed: the broad stamp now fires on `passVerdictInProse`. The three stamps
partition the shape exactly, and the arithmetic is pinned:

* `decision_reversed_in_prose` = a taken row under a pass verdict (broad)
* `plan_choice_conflict_narrowed` = that shape with **no** rival row named
* the re-ask itself = that shape **with** a rival row named
* therefore **broad == narrowed + firings**.

**Prediction.** `decision_reversed_in_prose` non-zero on any corpus in which
`plan_choice_conflict_narrowed` is non-zero, and `broad == narrowed + plan_choice_conflict`
firings on every corpus. **Falsifier:** broad != narrowed + firings -> a fourth path reaches
one of the stamps.

## D31 (LOW) — `hold_windows_skipped`: discharged on the record, and the real gap closed

The running saving was already on the hold line (see Finding 2). What was missing:

1. **Whose.** Both seats of a matchup write into ONE stderr with byte-identical wording, so
   two interleaved running counts could not be attributed. The line now leads
   `AIPlayerGPT[<deckFileSmall>]:` — the same seat token the translog filename and `my_deck`
   carry.
2. **A closing total.** `logGameEnd` now emits a per-seat stderr line with the same members the
   `gameend` record serialises: windows held, mana-only windows auto-passed, interchangeable-
   option asks resolved without a call, and the deadline-miss pair. A reviewer cross-tabbing
   holds against savings never has to open the JSON to reconcile the two surfaces.

No prompt change, nothing removed, report only.

**Prediction.** On the next corpus, `/usr/bin/grep -c 'windows held this game'` per stderr
equals the sum of `hold_windows_skipped` over that file's two seats, and each seat's closing
line reproduces its own gameend fields exactly; hold lines are separable by seat token.
**Falsifier:** a stderr whose closing lines disagree with its gameend records -> a seat is
ending without `logGameEnd` (which is itself the finding).

---

## Tests

**PARSETEST 2736 -> 2762 (+26), 0 failed.** Three new sections, all against the pure helpers
the seams call (each RED on base — two of the helpers do not exist there, and the third pins
the opposite of what base asserts):

* **`[#W57-A D5]`** — the arm classifier: both land-drop question shapes positive; four
  must-NOT-match NEGATIVES (a casting window; an unset tail; a cast row *naming* a land
  (`Cast Land Tax`); a land question that is not the land DROP; and a shifted tail, pinning
  that the test is a prefix and not a search). Then `asyncSlotDriftKind` on all five outcomes
  including `unknown` for a never-written key, and an ECHO SHAPE case pinning that the drop
  line's leading literal is unchanged so existing censuses keep counting.
* **`[#W57-A D16]`** — the three stamps at all four input combinations, a NEGATIVE that no pass
  verdict stamps nothing, and the partition arithmetic (`broad == narrowed + firings`) asserted
  as a count over four cases.
* **`[#W57-A D4]`** — that the wave-56 value was a strict PREFIX of the rendered HOLD row (the
  exact shape the census missed), the rendered row's byte content, the rendered/pure divergence
  on the last-offer clause with the parallel-size invariant, and the `-1` refusal values
  including a NEGATIVE that the value echoes no row of its own menu.

**No suite fixture, and the reason is structural** (unchanged from wave-54 lanes A/N, wave-55
lane A and wave-56 lane A): every behaviour here lives inside `AIPlayerGPT` behind
`mEndpoint`/`askModel` — i.e. behind a LIVE MODEL ENDPOINT. The suite's AI fixtures run the
Baka heuristic with no endpoint, so a fixture pinning the async slot, the translog field, the
census stamp or the stderr companion would be **GREEN ON BASE**, which is the opposite of a
pin. The diff touches two files, both `AIPlayerGPT`, and no non-GPT engine path. The PARSETEST
cases are the pins; the full suite is the regression gate.

## Gate (this worktree, 4 G memory cap, single run)

| | base (this worktree, seeded binary) | after |
|---|---|---|
| PARSETEST | 2736 passed / **0 failed** | **2762 passed / 0 failed** (+26) |
| suite | 1241 tests / 1 failed (`lifeline.txt`) | **1241 tests / 2 failed** (`lifeline.txt`, `merrow_reejerey.txt` — the known concurrency-only pair) |
| AI suite | 55 / **0 failed** | **55 / 0 failed** |
| timeouts | 0 | **0** |
| memory kills under the 4 G cap | 0 | **0** |

Logs: `~/.gatelogs/w57A-base-{parsetest,suite}.log`, `~/.gatelogs/w57A-post-{parsetest,suite}.log`.

## What I did NOT verify

* **No live-model probe, no live game, no stub-server run.** Nothing here was exercised against
  `qwen36-35b-a3b`, and no `WAGIC_GPT_DRIFT=1` dump was taken on this binary. Every claim about
  what the second slot DOES to the drop count is an argument from the wave-56 classification,
  not an observation; the next corpus is the measurement.
* **The second slot has never been observed holding an answer across an arm switch.** The arm
  classifier and the drift labeller are pinned as pure functions only. In particular I did NOT
  verify that `mPromptTail` is the tail of the prompt actually in flight at every one of the
  ~14 `pollCompletionRetry` call sites — wave 56 flagged the same gap for `asyncSlotKey`, and
  the arm now rides the same member. The failure mode if a seam polls with a `userMsg` it did
  not assemble in the same tick is that the poll reads the WRONG arm's slot: it would find that
  slot idle and launch, i.e. a MISSED rescue plus one extra round trip, not a wrong answer
  consumed (a wrong consume still requires a byte-identical tail, turn, phase and board).
  I did not enumerate those call sites to prove it.
* **The global `asyncBusy()` launch gate is unmeasured under load.** If one arm holds a long
  request, the other now returns `kChoicePending` where before it would have dropped and
  relaunched. I argue that is strictly better (the old behaviour paid a round trip to throw an
  answer away) but I did not measure the added pending-tick count, and a starvation mode would
  show up as a park, not as a wrong answer.
* **The board-half narrowing was NOT implemented**, on the strength of the 32-of-32 measurement
  above. If the next corpus's residual drops are `board`-labelled and same-arm, the item is
  still open and the measurement is now on the drop line rather than needing a fresh census.
* **`chosen_text` at the multi-select seams is unchanged** (see the D4 scope paragraph); those
  records are outside the prediction's N and I did not audit whether their name lists are
  byte-recoverable from `options_text`.
* **`repeat_n` is a new field with no consumer.** I did not update any harvest that read the
  `" xN"` suffix off `chosen_text` — a wave-57 harvest reading repeats must read the field.
* **D16's prose-name extractor is untouched**, so the wave-56 caveat stands: a rival row the
  model paraphrases is still not matched, and the narrowed column may contain genuine conflicts.
  I re-read none of the 14 records.
* **The suite was run once, threaded**, and produced exactly the known pair; I did not run it
  single-threaded to re-confirm the pair is solo-green, and I did not run it twice.
