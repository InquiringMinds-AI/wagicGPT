# Wave-72 lane BT — harness, the no-op class, the window budget (M1, M2, M10, M13, M14, M22)

Base `0e4e6b9b8`. Worktree `worktrees/lanes/w72-BT`, branch `w72-lane-BT`.
Base binary kept at `~/.gatelogs/w72-BT-base-wagic`, base harness at
`~/.gatelogs/w72-BT-base-harness.sh` (both used for the counterfactuals below).

**Gates.** Suite `1280 tests (0 failed), 73 AI tests (0 failed)` — 0 `==Test Failed !==`,
0 `==Test timed out`, `WAGIC_TESTSUITE_THREADS=1`, detached unit `w72-BT-suite`, log
`~/.gatelogs/w72-BT-suite.log` (the three known single-thread flakes did not fire this run).
PARSETEST **5330 → 5362 passed, 0 failed**: +34 new checks, −2 removed (the two Lightmine
Field iterations of the wave-58 `#W58-A D3` paired loop, whose POSITIVE half is exactly the
verdict M2 flips). Both link guards print OK. `git diff | grep -c U+FFFD` = 0.

Verdicts: **M1 confirmed+fixed · M2 confirmed+fixed · M10 confirmed+fixed · M13
confirmed+fixed · M14 REFUTED as worded, instrumented instead · M22 confirmed (with a
different mechanism than diagnosed) + fixed.**

---

## M1 — the harness never harvests the `askreplay/` sidecar — CONFIRMED, fixed

**Repro.** `~/.Wagic/ai/gpt/logs/askreplay/` holds 29 files / 725 records from the wave-71
run (`cache_replay` 657 + `repeat_ask_reserved` 68 = `ask_replays_reserved` 725 exactly);
`matchups-20260907-085638/` has none. `tools/selfplay-harness.sh` harvested with a
non-recursive `ls "$LOGDIR"/*.jsonl` glob — which is the same property W71-BS F4 relies on
to keep the sidecar out of the no-progress watchdog's `max(mtime)`. The engine half was
right, the harness half was never written.

**Fix.** `harvest_translogs()` (a named function now) copies, for each of THIS run's
translog basenames only, `$LOGDIR/askreplay/<basename>` into `$OUTDIR/askreplay/`. The
sidecars are not in `$BEFORE_LIST` and accumulate across runs, so the basename restriction
is what keeps an old run's spin out of a new corpus.

**Selftest.** `--selftest` now runs `harvest_selftest` before the regime-gate selftest and
fails the flag if it fails. It builds a throwaway LOGDIR with (a) a pre-existing log, (b) a
new log + its sidecar, (c) a sidecar with no matching new log, runs the real
`harvest_translogs`, and asserts on all four facts.

**RED on base.** `~/.gatelogs/w72-BT-M1-red.sh` grafts the new selftest onto the BASE
harvest block verbatim:
```
harvest-selftest FAIL: the ask_replay sidecar was not harvested (wave-71 HIGH-1)
exit=1
```
Green after: `harvest-selftest: 4 checks, 0 failed`.

**Prediction.** The next corpus's outdir contains `askreplay/` with one file per seat log
that produced a replay, and the engine seat's `ask_replays_reserved` can be reconciled
against it inside the corpus.

---

## M2 — a permanent's PROSPECTIVE zero is not a no-op — CONFIRMED, fixed (both halves)

**Repro (3 of the corpus's 4 fallbacks).** `125v152` seq 32 and `125v123` seq 7:
`Cast Lightmine Field {2}{w}{w} {right now: they control 0 creatures able to attack - deals
0 until they have an attacker - ...}`; `123v152` seq 10: `Cast Lightning Greaves {2} {right
now: you control 0 creatures - this equips nothing}`. The magnitude scanner
(`rightNowComputedMagnitudesAreZero`) reads only the operative core BEFORE the first `" - "`,
so in both cases what it read was the **precondition**, never an effect of the row. The one
TRUE fire is `125v152` seq 64 (Supreme Verdict into an empty board).

**Fix.** `rowSaysNoOp`'s magnitude path is rescued by `w72ProspectiveZeroOnAPermanent(row)`,
which requires BOTH halves of that shape and nothing else:
(a) every digit-bearing segment of the operative core is a SUBJECT-LED BOARD COUNT
(`they control 0 …`, `you control 0 …`) — a fact about the board, not a magnitude the row
produces; and (b) a segment after the dash carries a PROSPECTIVE cue (`until`, `when`,
`whenever`, or the equipment renderer's `equips nothing`). Supreme Verdict fails (a) on both
of its segments (`destroys 0 …`, `0 of yours`) and stays the no-op it is. A row with an
unqualified zero effect is untouched, and so is every row that reaches the phrase path
(`does nothing` / `does not apply`) above it.

**The header follows for free.** `allCastRowsDeadNote` folds through
`verdictReadsZero → rowSaysNoOp`, so the `NO LIVE CAST ROW ON THIS MENU` claim (292 windows)
stops being made about these rows without a second predicate. Pinned both ways.

**The re-ask (deck125 A-1).** The refusal quoted the row through
`stripNarrationDecoration`, which strips the `{right now: …}` clause — so the prompt stated
a conclusion and never showed the words behind it (`seq 33`: `[RE-ASK] You chose row 1
("Cast Lightmine Field {2}{w}{w}"), whose own note on this list says it does nothing right
now.`), and 2 of 3 re-asks came back wrong. `noopReaskLine()` now emits, at all four
emission sites (2 priority, 2 ask), one labelled line that QUOTES the row's own verdict
clause. Still one line, still nothing read outside the reply's two lines (invariant 000
untouched); a row with no `{right now:}` clause keeps the old wording.

**RED on base.** Base's own PARSETEST case `#W58-A D3 POSITIVE every computed-zero verb
emitted by the renderer is a no-op` runs over an array CONTAINING the Lightmine Field row
and passes on `~/.gatelogs/w72-BT-base-wagic` (8 iterations) — base asserts the exact
verdict M2 refutes. `strategy-design/wave72/lane-BT-m2-red.py` transcribes the base
predicate and prints `base refuses 2 row(s) it should not (M2 RED)` (exit 1).

**Prediction.** Next corpus: 0 `noop_row_zero_reask` fires on a row whose verdict core is a
subject-led board count plus an `until`/`when`/`equips nothing` tail; the Supreme-Verdict
shape still fires; every surviving fire's re-ask prompt contains
`whose own verdict on this list reads zero: {right now:`; and `NO LIVE CAST ROW ON THIS
MENU` no longer appears on a menu whose only cast row is a prospective-zero permanent.

---

## M10 — a repeat row at its stated stop is re-offered every phase seam — CONFIRMED, fixed

**Repro.** `1788789409-ai_baka_deck123-…-vs-ai_baka_deck152.jsonl` seqs **28–38**: eleven
consecutive `priority` windows on turn 11 (opponent's main 1 → combat begins → attackers →
blockers ×4 → combat damage → combat ends → main 2 ×2), each carrying the same two Thraben
Doomsayer rows over `{right now: M=29, your stated stop=29, so this window would add to a
count ALREADY AT OR PAST your own stop}` and each answered `CHOICE: 0 (pass)` —
**1,287 s of wall clock and 11 model calls to advance nothing.** Both existing re-serve
caches miss it: `mAskCache` is not on this seam, and `repeatAskAnswerStands` refuses a held
choice of 0 (`heldChoice < 1`), which a pass always is.

**Fix.** In `chooseOrderedAction`, when EVERY base row is either mana-only or a
repeat-eligible family whose rendered stop clause reads reached, the window is auto-passed
without a model call and counted `stop_reached_windows_skipped` (new gameend field) — the
same treatment a mana-only window already gets. This is not a removal and not an
auto-answer of an open question: the model stated the stop, the engine's own count reached
it, and no other live row is on the menu, so the answer is the one it already wrote. The
guard is the pure `w72StopReachedWindowCollapses(...)`, pinned face by face: one live row of
any other kind, or a count still short of the stop, or no stated stop, or a stop stated on
an EARLIER turn (a persisted stop can never silence a later turn) → the window is asked.

**Evidence.** Corpus repro above (11 windows, 1,287 s) plus 9 PARSETEST pins over the
guard and over `repeatRowStopClause`'s reached/short/absent faces. **No suite fixture:** the
collapse lives in `AIPlayerGPT::chooseOrderedAction`, which needs a live endpoint, so the
scripted harness cannot reach it. This is the lane's weakest evidence — see §Weakest.

**Prediction.** Next corpus: `stop_reached_windows_skipped` is non-zero in any game that
runs a repeat engine to a stated stop; no seat log shows more than ONE `priority` window per
turn carrying `ALREADY AT OR PAST your own stop` as its only live row; the wave-71
eleven-window / 1,287 s shape does not recur.

---

## M13 — `[castable now]` at a window whose casting decision is already answered — CONFIRMED, fixed

**Repro.** `146v125` seq 150 (deck146 MED-4). Corpus-wide, measured over every prompt that
carries `kCastAnsweredFact`: **123 of 199 such windows still stamp `[castable now]` on the
hand line** (247 stamps), while the legal-action list does not offer the card. The truthful
explanation was in the header, but the statement ON THE CARD won and the model reasoned to
"this is a contradiction or a specific UI quirk".

**Fix.** New verdict `kHandCastableAnswered`, chosen from the SAME pair the header is
printed on (`mCastAskTurn == observer->turn && mCastAskPhase == phase`) — one condition, one
source. The tag is RESTRICTION FIRST per the annotation-wording rule:
`[no cast row now: you already answered this phase's Casting decision, so this window does
not offer it - it is otherwise castable and its own casting window this turn has passed]`.
Nothing is deleted (a silent omission is what the trust doctrine says the model
confabulates into).

**Second half, found while fixing it:** `mCastAskTurn/mCastAskPhase` were stamped BEFORE
`askModel`, so a cast-menu RE-ASK (`attempt > 0`, which re-renders the situation with no
prefill) would have printed "you already answered this phase's Casting decision" inside the
casting window itself — a true statement in the wrong scope. They are now set after the
window is answered, which is also when "answered" becomes true for the header's own use.

**RED on base.** Base has no such verdict: `handCastabilityTag(kHandCastableNow, …)` is the
only castable branch and returns `" [castable now]"` unconditionally; the corpus census
above (123/199) is the live measurement. Pinned: the open window still stamps
`[castable now]`; the answered one never contains `castable now`, opens with the
restriction, still names the reason, and never claims an affordability failure.

**Prediction.** Next corpus: 0 prompts contain both `already answered this phase's Casting
decision` and `[castable now]`; the count of `[no cast row now: you already answered` tags
is in the same order as this wave's 247.

---

## M14 — "the L4 branch issues an ask with no instant-speed candidate" — **REFUTED as worded**

**Measurement (the whole corpus, not one seat).** Every `ask`-kind record whose prompt is a
`Casting decision` outside a main phase — 341 windows across all 42 seat logs, own-turn and
opponent-turn — carries **at least one Cast row**. There is no window in the corpus where
the seam asked with nothing castable. The mechanism is already in place:
`AIPlayerGPT::FindCardToPlay` passes `instantWindow = true` to
`DecisionManager::buildCastSpell` and returns before any model call when the legal set is
empty ("nothing castable: only one outcome, no model call"), and Baka's L4 arm falls through
to `selectAbility()`.

So deck146 MED-5's 32.7% no-op rate (corpus-wide: **792 of 2,243 decisions, 35.3%**) is NOT
an engine defect of this shape. Those windows offered a legal instant-speed cast and the
model declined it — which is a decision, and collapsing it would be removing a legal option.

**What shipped instead.** The class MED-5 could not see is now countable:
`own_turn_windows_skipped` (new gameend field) counts own-turn instant-speed windows the L4
arm entered and left WITHOUT reaching the cast search at all (no instant/flash card in any
castable-from zone), via a new no-op virtual `AIPlayerBaka::noteOwnTurnWindowSkipped()`
overridden by the GPT seat. Report-only; no behaviour change.

**Prediction.** Next corpus: `own_turn_windows_skipped` is non-zero, and
(`own_turn_windows_skipped` + own-turn non-main casting asks) accounts for the own-turn
instant-speed windows the L4 arm admits — so the next budget review can price a window that
cost a round trip against one that cost nothing.

---

## M22 — the replay refusal is defeated by interleaving — CONFIRMED (different mechanism), fixed

**What the diagnosis said** ("`ask_replays` longest run 38 and growing… the cap will trip on
long games — scope the refusal to a WINDOW, not the game") is not what the code does: the
run counter is already keyed on state+question and is already dropped at the turn boundary
with the ask cache, and the longest `replay_run` anywhere in the wave-71 sidecar is **11**,
not 38 (38 was the length of a same-question STREAK, not the counter).

**The real defect, from the sidecar.**
`askreplay/1788789439-ai_baka_deck123-0x5629c69e3090-…jsonl`, turn 36, seqs 363–383: an
**18-record streak of one byte-identical question whose `replay_run` restarted at 1 halfway
through** (…, 8, 9, then 1, 2, 3, …). Cause: `mAskReplayKey`/`mAskReplayRun` are ONE slot,
and `askModel` clears them on **any** real model answer — so three unrelated windows
answered in between wiped the run. A livelock that alternates between two windows can
therefore never reach the cap, which is the one thing the cap exists for (W71-BP L1c).

**Fix.** `askReplayRefuseScoped(key, runs, maxRun)` — one counter per state+question in a
`std::map`. Interleaving cannot reset a run; the map is cleared with the ask cache at the
turn boundary (the keys embed the turn header, so they are dead anyway); a real answer for a
window erases only THAT window's entry. The sidecar's `replay_run` now reports the window's
own run. `askReplayRefuse` (the single-slot form) is KEPT and still pinned — it is what the
new pin uses to state the defect.

**RED on base, inside the binary.** New pin
`#W72-BT M22 the wave-71 shape, stated`: over 128 interleaved replays of two windows the
BASE predicate `askReplayRefuse` never fires, while `askReplayRefuseScoped` does. Both run
in the same PARSETEST.

**Prediction.** Next corpus: no sidecar streak of one question shows `replay_run` decreasing
mid-streak; if a two-window loop occurs, `ask_replays_refused` is non-zero instead of 0.

---

## Files

- `projects/mtg/tools/selfplay-harness.sh` — M1 (harvest + `harvest_selftest`)
- `projects/mtg/src/AIPlayerGPT.cpp` — M2, M10, M13, M22 + 34 PARSETEST checks (−2)
- `projects/mtg/include/AIPlayerGPT.h` — M10/M14 counters, M22 map + predicate, M14 override
- `projects/mtg/src/AIPlayerBaka.cpp`, `projects/mtg/include/AIPlayerBaka.h` — M14 hook
- `projects/mtg/strategy-design/wave72/lane-BT-m2-red.py` — M2 RED-on-base transcription
- `projects/mtg/strategy-design/wave72/lane-BT.md` — this file

## Weakest evidence (in order)

1. **M10 has no live-path test.** The collapse runs only in `AIPlayerGPT::chooseOrderedAction`,
   which requires an endpoint, so nothing but the next corpus exercises it. What IS pinned is
   the guard's truth table and the clause it reads; what is NOT observed is a real game
   collapsing a window. If `carriedStop` is ever populated from a plan whose "stop" number
   means something else, this silently passes a window the model wanted — the turn guard and
   the every-row guard are what bound that, and both are reasoned, not measured.
2. **M2's rescue can under-refuse.** The predicate deliberately errs toward the model's own
   answer standing: a genuine no-op whose core happens to be a board count AND whose tail
   happens to contain `when`/`until` would no longer be refused. I chose that direction
   because the measured harm is a FALSE refusal (a spent round trip that argued the pilot off
   its own plan); a missed refusal leaves a legal row the model chose. I did not enumerate the
   renderer's full emitter set to bound how many shapes that is.
3. **M14 is a refutation resting on one corpus.** 341 windows is the whole corpus, but only
   this corpus; a card whose instant-speed legality the oracle computes differently could
   still produce the shape MED-5 hypothesised. The new counter is what would show it.
4. **M13's second half (the stamp move) is unobserved.** Moving `mCastAskTurn` past
   `askModel` is reasoned from the code path (`attempt > 0` re-renders with no prefill); no
   test drives a cast-menu re-ask, and the suite has no coverage of it.
5. **M22's fix is pinned but not lived.** No livelock occurred in wave 71 (0 refusals), so
   the new counter path is proven only by PARSETEST, exactly as the wave-71 version was.
