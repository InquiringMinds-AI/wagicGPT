# Wave-80 lane DE — windows, holds and keys (U2, U8, U9, U10, U15)

Base `70345356d` (= master). Worktree `worktrees/lanes/w80-DE`, branch `w80-lane-DE`.
Evidence: wave79/known-bugs.md §U (U2, U8, U9, U10, U15), wave79/engine-seat.md
(HIGH-2, 2(c), MED-2, MED-3, T3a/T3b), wave79/deck125/review.md (`125v162` seq 199),
wave79/deck123/review.md (`123v130` seqs 72-80), wave79/deck126/review.md.
Repairs lane-DC F1/F2/F10.

**Gate.** PARSETEST **6934 passed / 0 failed** (base 6902 / 0; **+32**). Suite
THREADS=1 **1295 tests (0 failed), 76 AI tests (0 failed)** — `==Test Failed !==` 0,
`Test timed out (game never ended)` 0, no flake hit. `check-reply-instructions` OK
(1 source, 37 guides); `corpus-stats.py --selftest` OK; `check-ctor-init` OK
(118 files); `git diff | grep -c U+FFFD` = **0**. No corpus was run and no request
was sent to the pilot. The reply protocol text and the parser are byte-identical.

---

## U15 — MED: the render counters count BUILDS, not sends

**RED.** Wave-79 census: `own_loop_verdict_lines_rendered` 63 against **56** prompts
carrying the literal, `crossphase_identical_reputs` 172 against **170** rendered
notes, `stack_drain_windows_asked` 30 against **18** rendered clauses. Each counter
was incremented where its clause was *composed*, upstream of the honoured hold, the
reservation decline, the loop auto-pass, the deadlock-breaker pass and the ask-cache
replay — and each re-implemented its own once-per-window guard, which is why DC F10
repaired one of the three and left the other two standing. Pinned as the wave-79
ORDER (`#W80-DE U15 RED-ON-BASE`): a build-site counter increments for a window the
gates below it discard.

**Fix.** One predicate, `w80CountRenderedAtSend(sent, rendered, windowSeq,
countedSeq, counter)`, counts only when the prompt is handed to the model AND the
clause is in it, once per window; every seam now calls it through the member
`w80ApplyVerdictFacesAtSend`, which also stamps the record faces on exactly the same
condition and CLEARS them when the window is suppressed.

**GREEN.** Five pins: suppressed-with-clause counts 0; sent-without-clause counts 0
(this is the 63-vs-56 gap at its source); sent-with-clause counts exactly 1; a later
polling tick on the same window counts nothing more; the next window counts again.

**Prediction (wave-80 census).** `stack_drain_windows_asked` **> 0** and EQUAL to the
number of prompts containing `OF THIS STACK`, and `crossphase_identical_reputs`
EQUAL to the number of prompts containing `[this exact list was put to you`.
Expected band 15-40 for the drain counter (wave 79 rendered 18 clauses).
Negative clause: a counter still above its literal's prompt count means a seam
composes the clause somewhere that is not routed through the one predicate — which
is now one function to inspect rather than three call sites.

---

## U2 — HIGH: the hold counters are unauditable and two verdicts render nowhere

**RED.** `hold_verdict_safer_ignored` 59 and `hold_reopened_new_threat` 0 carried no
DebugTrace and no record field, and the two markers the clamp arbitrates
(`[crack-back verdict:`, `[stack death verdict:`) appear **0 times in all 2,203
wave-79 prompts** — they are latch KEYS only. So neither the window, nor the held
face, nor the live face was recoverable from any of the 42 seat logs, and DC F2's 0
had to be called UNTESTED by default. Pinned through the live composer's own inputs
(`#W80-DE U2 RED-ON-BASE`): the wave-79 priority-seam note composer is exactly
`w79OwnLoopVerdictLineFor` + `w78StackDrainNote`, and neither can ever carry either
verdict's bytes.

**Fix.** Both verdicts now RENDER, built by `w80CrackBackVerdictLine` /
`w80StackDeathVerdictLine` off the SAME two key builders the latch compares
(`crackBackVerdictNow` / `stackDeathVerdictNow`), on the prompt-only channel at the
priority and casting seams; and the clamp and every re-open stamp
`hold_safer_ignored_face` / `hold_reopen_reason` on the record of the window they
happened at, plus a stderr trace naming both faces.

**GREEN.** Fixture from `125v162` seq 199 (deck125, turn 28 Upkeep, life 28 vs 20,
their battlefield 0 creatures) and its own next window seq 200 (the stack carrying 2
damage to the seat): seq 199 owes NO crack-back line (0 able attackers — a silent
face is not a claim); the same board with 4 attackers renders the survivable face
verbatim and at 30 damage the LETHAL face; seq 200's stack renders
`you survive the stack` verbatim, an empty stack renders nothing, and 30-into-28
renders `the stack KILLS you`. **Key stability (wave-74 rule):** two windows whose
only difference is the rendered verdict line key IDENTICAL through the live latch
builder (`w78HoldLatchKeys` + `holdActionKeyRow`), and the seq-199 hold still STANDS
over the same rows once the verdicts are rendered — the render retires no hold and
adds none.

**Prediction.** `hold_safer_ignored_face` present on **> 0** records (expected band
30-80 against wave 79's 59 clamp events) and `hold_reopen_reason` present on **> 0**
records (band 60-150 against wave 79's 100 stderr re-opens), each naming the held and
the live face; `crackback_verdict_lines_rendered` **> 0** and EQUAL to the prompts
containing `[crack-back verdict:` (band 100-600 — wave 79 had 111 windows whose
crack-back total was non-zero); `stack_death_verdict_lines_rendered` **> 0** and
EQUAL to the prompts containing `[stack death verdict:` (band 50-400 against 360
`ON THE STACK` records). `hold_reopened_new_threat` **> 0** *conditional on the
corpus containing an own-loop THREATENED window* — that face rendered 0 times in
wave 79, so this counter's population did not arise; with `hold_safer_ignored_face`
now on the record the next seat can classify a 0 as FAIL-or-not-arisen from evidence
instead of by default, which is the whole point of the item.
Negative clause: a rendered count ABOVE its literal's prompt count means a
suppressing path still stamps.

---

## U8 — HIGH: a dead list crosses every phase at full price

**RED.** `crossphase_board_unchanged` 4 -> 26 — full model calls over a board the
bracket ITSELF certifies unchanged. Acceptance board `123v130` seqs 72-80: one 3-row
list (`Cast Devour Flesh {1}{b}` + Hold + `Cast nothing right now`) walked across
Upkeep, Draw, Main 1, Combat begins, Attackers, Combat ends, Main 2, End and Cleanup
of turn 12 — nine windows, **746 s** of inference, no board effect. Pinned through
the live key builders (`#W80-DE U8 RED-ON-BASE`): `w76CrossPhaseListKey` already keys
seqs 79 and 80 as ONE list at one turn, while `w79AskScopeKey(12, 9)` !=
`w79AskScopeKey(12, 10)` — the ask key carries the phase, so no cache hit is possible
across one however dead the list is.

**Fix.** `w80CrossPhaseReplayable` admits a replay only when the seat's own last
answer to this list DECLINED it, the board key the bracket compares is EQUAL, the
option list is **byte-identical** (which is what proves the legal option set is the
one the seat already read — nothing is removed, capped or auto-answered; the seat is
served its own answer), and the transition does not enter a sorcery-speed window.
The replay logs a per-event line in `askreplay/` naming the answer, the source
window and both phases, and increments `crossphase_replayed`.

**GREEN.** Seven pins: End -> Cleanup replays; Main phase 2 -> End replays (the
corpus's largest population, 14 of the 26); **Draw -> Main phase 1 is still ASKED**
(Astra wave-79 F1: entering a sorcery-speed window is where a phase-gated legal
option first becomes legal); a moved board is asked; rows that are not byte-identical
are asked; an ACTING answer is never replayed; the same phase is not a cross-phase
replay.

**Prediction.** `crossphase_replayed` **> 0**, expected band 10-40. Measured on the
wave-79 corpus's own 26 board-unchanged re-puts, the rule admits **25** and asks the
1 `Draw -> Main phase 1` case; the trace field is the `askreplay/` line
`why: crossphase_replay from <phase> to <phase>` carrying the replayed choice and
the source window seq. Negative clause: `crossphase_replayed` = 0 while
`crossphase_board_unchanged` is still large means the rows are not reaching the
comparison byte-identical at the live seam — i.e. `W76CrossPhaseAsk::rows` is being
populated from a different vector than the one the next window is asked over.

---

## U9 — MED: the replay counters are not comparable across waves

**RED.** The wave-79 census read `ask_replays_reserved` 1,344 -> 1,084 and
`identical_ask_answers_reserved` 953 -> 0, which reads as a 260-replay drop. It was
not a drop: the population MOVED path (953 latch / 391 cache -> 0 latch / 1,084
cache). The two counters name different unions of the same paths and the cache figure
was recoverable only by subtracting them, a relationship documented nowhere on the
record. Pinned as that arithmetic (`#W80-DE U9 RED-ON-BASE`).

**Fix.** Three named counters — `ask_replays_cache`, `ask_replays_repeat_latch`,
`crossphase_replayed` — whose sum IS `ask_replays_reserved`, so a wave-to-wave
comparison reads path movement instead of an invented drop.

**GREEN.** The sum identity is pinned.

**The 22 residual pairs' differing bytes** (T3a/T3b, measured on this corpus with a
rows-only instrument that reproduces the engine seat's **3** byte-identical
survivors): of the strip-identical pairs' differing annotation groups, **22 are
`{leaves N of your M untapped mana sources untapped}`** — the untapped-source figure,
which is the `sources=` term of the F1 continuation digest — 2 are `{right now: ...}`
target/verdict changes and 2 are `{crack-back cover ...}`. So the residual is not a
key defect: it is the digest working as designed over a menu whose *rendered* rows
moved only inside their braces. The remaining 3 byte-identical survivors are the
fetchland crack row on the priority seam already named in engine-seat 2(c)/LOW-2.

**Prediction.** `ask_replays_cache + ask_replays_repeat_latch + crossphase_replayed`
== `ask_replays_reserved` exactly, with `ask_replays_cache` **> 0** (band 400-1,400).
Negative clause: a sum that does not match means a fourth re-serve path exists that
no counter names.

---

## U10 — MED: the own-loop residuals

**RED.** Three defects, all corpus-located.
(a) `[own loop verdict: your life loop is a proven win]` printed 4x on a board with
no live starter (`126v130` seqs 31-36, the game deck126 lost) — `lifeLoopProvenWin`
proves the CHAIN and says nothing about an ENTRY.
(b) **61 of the corpus's 63 stamped record faces read `[own loop verdict: none]`**
while the prompt carried a proven-win paragraph, because the stamp was
`w77OwnLoopVerdictKey(state, "", "")` and that key is `none` for the state that
renders the proven-win line; **7 of those 63 had no verdict line in the prompt at
all**, and I ran them down — all seven are two-row **LAND DROP** menus
(`...deck126` seqs 20/30/32/44/61, `1789189686-...deck126` seq 30,
`1789189713-...deck126` seq 23), a seam that calls `askModel` directly and never
touched the own-loop face, so a face stamped by a cast window still in flight was
consumed by the next record written. That is the seam DC F10 missed.
(c) The RESOLVING verdict recommended a hold (`the HOLD row covers every remaining
link`) beside a hold row whose own text gives up every other row on the menu — at
`126v125` seq 46 that was two live Sorin activations.
Pinned (`#W80-DE U10 RED-ON-BASE`): the stamped face and the printed line were two
different facts about one window.

**Fix.** The record face is read off the LINE THAT PRINTED
(`w80LoopFaceOfLine`), so a window with no line stamps nothing and `none` is never
stamped; the land-drop seam clears the staged faces at its own send; the proven-win
claim is gated on a LIVE starter found by `w80LiveLoopStarterName` (a permanent I
control whose script can move life, or a hand card the mana engine can plan a payment
for, excluding the loop's own halves) and NAMES it, and with none the line prints the
GAP instead of the win; and the RESOLVING line states the hold row's own cost in the
hold row's own terms.

**GREEN.** The face names what the model read at every shape (proven win / proven but
no live starter / resolving / THREATENED); a window with no verdict line stamps no
face; the proven-win line names its starter; the starter-less board prints
`no live way to start it` / `NOT yet a win` and never the words `proven win`; the
starter script test reads the engine's own `life:` / `damage:` terms and rejects a
counterspell; the RESOLVING line carries `gives up every other row on this menu`.
Two wave-79 pins are SUPERSEDED in place and rewritten (`#W79-CZ T4 GREEN the idle
face, verbatim` and `#W78-CV S6 GREEN the seq-43 shape`).

**Prediction.** `own_loop_verdict_lines_rendered` **> 0** (band 2-80) and EQUAL to
the number of prompts containing `[own loop verdict:`, with **0** records carrying
`own_loop_verdict` == `[own loop verdict: none]` and 0 records carrying the field on
a two-row land-drop menu. Negative clause: any `none` face on a record means the
stamp is not being read off the line.

---

## Weakest evidence

1. **The GREEN pins for every new helper cannot be run on the base binary** — the
   functions do not exist there. The RED-ON-BASE pins therefore assert the base
   defect through base-era functions that are still callable (the composer that
   cannot carry the verdict bytes; the key that already matches while the ask key
   does not; the `none` stamp beside a non-empty line). That is the wave-79 lane-DC
   convention, but it is weaker than a counterfactual run.
2. **U8's yield is modelled, not observed.** The 25-of-26 figure is my replay
   predicate evaluated over the wave-79 corpus's own `nothing on the board has
   changed` brackets and each record's phase. The live seam additionally requires the
   option vectors to compare byte-equal; if the engine re-renders an annotation
   between the store and the compare, the real yield is lower. Nothing in this lane
   observes a live cross-phase replay.
3. **`hold_reopened_new_threat` may read 0 again.** Its branch needs an own-loop
   THREATENED face, and that face rendered 0 times corpus-wide in wave 79. I did NOT
   widen the equal-rank yield to the crack-back or stack-death families, because
   giving either an identity would put a board fact that creates no row back into the
   latch key — the exact defect #W79-CZ removed 194 re-opens by fixing. The item is
   discharged as *auditability* (two new record fields), not as a re-open rate.
4. **`w80LiveLoopStarterName` is an approximation on the liveness axis.** A
   battlefield permanent counts as a starter on its SCRIPT alone — affordability of
   its activation is not checked (only hand cards go through `ManaEngine::planPayment`).
   So the gap line can still be withheld on a board whose only starter is an
   unaffordable activation. The direction of the error is conservative (it claims a
   win less often than wave 79 and never claims a gap that is not there), but
   `126v130`'s own board — a gain source that exists and cannot be used — is only
   partly covered.
5. **`w80LoopStarterScript` is a substring test over the engine script.** It will
   accept a `life:` term that moves life in the direction that does NOT enter the
   chain. Deliberate: an over-broad script test can only make the engine claim less.
6. **The U9 residual-byte analysis used my own rows-only instrument**, which
   reproduces the engine seat's 3 byte-identical survivors exactly but finds 10
   strip-identical pairs against its 19 — my strip is a single non-nested pass, not
   `holdActionKeyRow`. The *distribution* (the untapped-source figure dominating)
   is the finding; the count is not comparable to the seat's.
7. **No live render was observed for either new verdict line.** Both are pinned on
   corpus-derived board numbers through their own builders and through the live latch
   key, but the wave-80 corpus is the first place they can be seen in a prompt.
