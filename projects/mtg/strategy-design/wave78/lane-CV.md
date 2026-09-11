# Wave-78 lane CV — the window/hold family (S3, S4, S6, S7, S9, S11)

Worktree `worktrees/lanes/w78-CV`, branch `w78-lane-CV`, base `1e465fff7`.
**Gates.** Seeded tree: suite 1291/0 + 76 AI/0, PARSETEST 6402/0.
After: **PARSETEST 6450/0**, **suite 1291/0 + 76 AI/0** (`~/.gatelogs/w78-CV-suite.log`,
`==Test Failed !==` 0 and `==Test timed out` 0, THREADS=1, detached unit). Files:
`src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`. `git diff | grep -c U+FFFD` = 0;
`check-reply-instructions.py` OK (1 source, 37 guides); both link guards OK.

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-010157-final`.

---

## S3 HIGH — a stop-reached row re-offered 12 times in one turn

**Repro (read, not assumed).** `126v123` deck123 seat, seqs 41-52 (turn 13, the
opponent's turn): TWELVE priority windows, each a two-row menu —
`Create human with Thraben Doomsayer [cost: Tap] {card text: ...} {if you pass here,
this option is not offered again until the board changes} {right now: M=31, your stated
stop=29, so this window would add to a count ALREADY AT OR PAST your own stop - past your
stop = a wasted window}` plus the hold row — and TWELVE `CHOICE: 0 (pass)` replies.
Latencies 84.1 s + 105.6 + 99.3 + 82.5 + 114.7 + 71.9 + 72.8 + 97.4 + 133.9 + 69.4 + 87.6
+ 98.7 ≈ **17 min of decode for zero decisions**. Seq 41 carries no
`{if you pass here...}` clause and seqs 42-52 do: the whole run is one list re-put.

**Why the shipped collapse is blind to it — the mechanism, not a guess.**
`w72StopReachedWindowCollapses` requires `everyBaseRowIsStopReached`, which the base-row
scan can only set for rows that pass `repeatRowEligible`; that predicate opens with
`aa->counters < kRepeatRowFloor` — **the activation count THIS TURN**. Turn 13 is a fresh
turn, the count is 0, no repeat row is offered (seqs 38/39 on turn 12 DO carry
`Create human with Thraben Doomsayer, repeated then stop`; seqs 41-52 do not), so the
scan sets `everyBaseRowIsStopReached = false` on the very row whose own printed verdict
says `ALREADY AT OR PAST your own stop`. The verdict renders; the collapse cannot see it.

**RED evidence (executed, through the live predicate).**
`#W78-CV S3 REPRO`: `w72StopReachedWindowCollapses(false, true, 29, 13, 13)` is **false** —
the base build's answer for this shape at every one of the twelve windows.

**Fix.** A second arm, exactly as wide as the ruling allows and no wider.
The base-row scan now also computes `everyBaseRowStopPriced` from the ROW'S OWN printed
verdict (token-maker, live count >= `carriedStop`) rather than from repeat-row
eligibility, and `w78StopReachedRePutCollapses(everyBaseRowStopPriced, anyStopReachedRow,
carriedStop, mStatedStopTurn, nowTurn, declinedN)` adds one clause to the wave-72 guard:
`declinedN >= 1` — **this exact option set has already been put to this seat this turn and
declined**. The wave-72 arm is byte-unchanged in every clause (it was only moved below
`declinedN`, which is measured from `optionSetKeyOf(shownLines)` — the ask-cache's own
board+question key). Consequences that make this the ruling's allowance and not a cap:
the FIRST put of any list is always asked; a window with any other live row fails the row
test; the moment the count falls back under the stop, or any row appears, the key is a
different key, `declinedN` is 0 for it, and the window is asked. Counter
`stop_reached_reputs_collapsed` on `gameend`, separate from
`stop_reached_windows_skipped`.

**GREEN.** 8 pins. The truth table (first put asked; re-put collapses; any other live row
refuses; no row at the stop refuses; no stop stated refuses; a stop stated on an earlier
turn refuses — the #W72-BX F2 guard intact), **the seq-41→52 shape walked end to end =
1 ask and 11 collapsed re-puts** (the base walk asks all twelve), and the negative's own
pin: a window with any other new row is asked **at every decline count 0-5**.

**Prediction (falsifiable).** Next corpus: no (turn, phase, seat) cell shows more than
**two** asks over a menu whose only acting rows print `ALREADY AT OR PAST your own stop`,
and `stop_reached_reputs_collapsed` is non-zero on deck123's seats.
**Pin for the negative clause:** if a seat shows a third such ask, the collapse is not
covering the re-put — read `declinedN` for that list (the `[you declined this exact list N
times already this turn]` note is on the prompt) before touching the predicate; if N >= 1
and the window was still asked, the option-set key moved between puts and the defect is in
`optionSetKeyOf`, not here. And `stop_reached_reputs_collapsed` must never exceed
`stop_reached_windows_skipped`.

---

## S4 HIGH — nine byte-identical windows while a 7-object trigger stack drained

**Repro.** `152v162` deck152 seat, seqs 55-63 (~15 min), one Draw step, the opponent's
seven-link trigger stack resolving one link at a time; Katilda's unused activation sits
above the hold row, so the guide correctly forbids a blind hold. Same family as R1:
nothing may be collapsed — the board genuinely moves between links.

**RED evidence.** `#W78-CV S4 GREEN the drain clause, verbatim` — the exact byte string
does not exist in the base binary (`w78StackDrainNote` is new); the base build prints no
sentence about the drain at any of the nine windows.

**Fix (annotation only).** `w78StackDrainNote(theirTriggers, rowsUnchangedSinceLastAsk,
holdRowShortName)` prints
`[their stack is draining N triggers - each link will put this same list to you; HOLD
(Hold priority) covers every link, the activation above stays available after]`,
gated on `theirTriggers >= 2` AND on the hold-check bracket's **own** comparison
(`w78RowsUnchangedSinceLastAsk` tests the bracket text for
`every row above was also on the menu`), so the clause and the hold-check line can never
disagree. `w78TheirDrainingTriggerCount()` walks the live stack for unresolved objects the
opponent controls, the same walk `w77OwnLoopStackState` uses.
`w78HoldRowShortName` cuts the hold row at its first ` - ` so the clause names the row and
copies none of its contract paragraph. Both seams (priority + cast), prompt-only channel.
Counter `stack_drain_windows_asked`, one per WINDOW (`mStackDrainCountedSeq == mWindowSeq`
guard, the `w77CountOwnLoopWindow` discipline).

**GREEN.** 10 pins. The verbatim clause; three MUST-NOT-MATCHes (one link is not a drain;
a menu that moved; no hold row). **The hold across the drain, through the LIVE latch and
the LIVE bracket**: a hold taken at link 1, seven links, **0 re-opens and 7 brackets
saying `every row above was also on the menu`** — which is what makes one hold the whole
answer. Key-stability set: the ask-key tail and the `asyncSlotKeyOf` slot key are built
from the option list only (the clause is spliced below `optionsEnd`), the clause's number
moves (7 → 3) while `optionSetKeyOf` does not, and `holdStillStands`/`holdActionKeyRow`
are untouched.

**Prediction.** Next corpus: on any seat whose prompt carries the drain clause, the number
of asks in that (turn, phase) cell is <= `theirTriggers`, and at least one such window is
answered with the hold row. **Pin for the negative:** `stack_drain_windows_asked` > 0 with
zero prompts carrying `their stack is draining` means the gate fired and the note did not
reach the prompt — check the splice at `optionsEnd`, not the predicate.

---

## S6 HIGH — the `[own loop verdict:]` marker never rendered

**Repro.** engine-seat §2(b): 0 occurrences of `[own loop verdict:` in 42 logs.
`w77OwnLoopVerdictKey` (`:30503`) feeds `kHoldVerdictMarkerHead` — it is a **synthetic
marker joined to the HELD SET**, never a rendered row, by CU F4's design. At the corpus's
one own-loop window (`126v130` deck126 seq 43, priority, turn 17 Main 1, life 7-14) the
RESOLVING verdict was TRUE against the live stack and invisible; the seat answered
`CHOICE: 1 (+1: create a 1/1 vampire with Sorin, Lord of Innistrad)` instead of the hold.

**RED evidence.** `#W78-CV S6 GREEN the seq-43 shape RENDERS a verdict line` — the string
does not exist in the base binary; the base build's only own-loop surface is the per-row
`[your loop is resolving on its own ...]` tag, and the VERDICT itself was never printed.

**Fix.** `w78OwnLoopVerdictLine(state, theirSpell, component)` renders the verdict as a
prompt line on the same prompt-only channel the annotation family uses; the key-side
marker is untouched, so the hold still re-opens when the verdict moves. RESOLVING states
the mechanism and the hold's coverage; THREATENED names **their spell** and **your
component** and says the hold does NOT cover the window; IDLE prints nothing, and a
threat with no spell to name prints nothing rather than half a sentence.

**GREEN.** 7 pins: the key marker unchanged; the RESOLVING line verbatim (asserting the
RENDERED BYTES, per the wave-77 lesson, not the key); THREATENED naming both cards;
two MUST-NOT-MATCHes; the key-stability set (rows, ask-key tail, option-set key and hold
key byte-identical with the line and without it); and the marker STILL re-opening a hold
when the verdict turns THREATENED.

**Prediction.** Next corpus: every record whose gameend carries `own_loop_windows_asked`
> 0 has `[own loop verdict:` in its prompt, and the count of rendered verdict lines equals
`own_loop_windows_asked` plus the THREATENED windows. **Pin for the negative:** a
non-zero `own_loop_windows_asked` with zero rendered lines is the wave-77 defect
un-fixed — the line is being built and dropped before the splice.

---

## S7 MED — `hold_check_ref_seq` is a window ordinal, not a record seq

**Repro.** Reconstructed the field over the corpus: it is `mWindowSeq`, which advances on
every `writeTransLog` record and not on `recovery`/`wall_miss`, so ~80% of the time it is
within 1-3 of the record index and **246 of 294 land at the wrong seam if read as a seq**.
Four deck seats and the engine seat could not join it. Second half: 18 records carry it
with no bracket in their prompt, contradicting the field's own documented contract
(`:19943`), and two of those (`146v126` records **46** and **55**) are worse — they
STAGED, COMMITTED to the cast seam, and rendered nothing, so records 47 and 56 named them
as "the last window I asked you at this seam".

**What records 46/55 actually are (checked, not assumed).** Both are DUNGEON-ROOM menus:
`kind: ask`, `log_window_kind: unclassified`, prompt tail `1. storeroom (room 4 of 7 ...)
/ 2. dark pool (room 5 of 7 ...)`, `grep -c 'hold check'` = **0**. `unclassified` is also
the class every casting window carries (1,627 of them corpus-wide), so the record's own
words cannot separate the two — the routing by `kind`/`mLogWindowKind` alone is
undecidable here. A window-kind-based routing change would have broken all 1,355 genuine
cast commits; the prompt is the only authority.

**Fix.** (a) `W76HoldMemory` gains `lastRecordSeq` / `measuredRefRecord`;
`w78HoldWindowRecordSeq(m, seam, windowSeq, recordSeq)` writes the committed window's
record seq once the record carries it (after the recovery flush), and refuses a stamp for
a window that is no longer the committed one. The record emits
**`hold_check_ref_seq` = the referent's RECORD seq** and **`hold_check_ref_window` = the
window ordinal**. (b) The contract is ENFORCED rather than asserted:
`holdBracketRendered = userMsg.find("[hold check:") != npos`, computed off the prompt
about to be logged; a record with no bracket carries neither field — all 18 violations
retire structurally. (c) The same test routes the COMMIT: if the staged window measured a
bracket (`w78StagedNoteIsEmpty` false) and this prompt carries none, this record is not
that window's, so the stage is DROPPED (`w76HoldWindowNotAsked` — the #W77-CR R2a policy)
and the next bracket still names the last window the model actually saw. A window that
measured NO bracket still commits, so the ordinary "rebuild with no run and no new row"
case is unchanged.

**GREEN.** 6 pins: both integers in the memory; the referent carrying window 4 AND record
37; a superseded stamp refused; `w78StagedNoteIsEmpty` on an empty seam and on a staged
one; and the dropped stage leaving the next bracket at `first window`.

**Prediction.** Next corpus: **0** records carry `hold_check_ref_seq` without a
`[hold check:` bracket in their prompt (wave 77: 18), `hold_check_ref_seq` joins to a real
record `seq` at 100% (wave 77: ~20% off by 1-3), and `hold_check_ref_window` reproduces
the wave-77 values exactly. **Pin for the negative:** if any bracket's referent record is
a `kind: ask` record whose prompt has no cast menu in it, (c) did not fire — check
`w78StagedNoteIsEmpty` before touching the routing.

---

## S9 MED — hold-check residuals: renumbering, and the rows that left

**Repro (a).** `152v130` deck130 seat, seq 6 (one acting row, `cycling with Forgotten
Cave`) → seq 10 (nine acting rows). Wagic disambiguates two copies by APPENDING an ordinal
and RENUMBERING the first: seq 10 prints `Pyrite Spellbomb #1` and `#2` where the
predecessor printed `Pyrite Spellbomb`. Every row of the copy that was already there then
carries a byte the key had never seen — the bracket said `8 rows above are new`.
Same shape at `126` 12→18, `123` 62→70, `125` 84→88.
**Repro (b).** `125` deck152 seqs 20/35: a row DISAPPEARED and the bracket said
`every row above was also on the menu` — true about the rows above, silent about the row
that left, which the hold row's own contract names as a re-opener.

**RED evidence (executed).** `#W78-CV S9a REPRO`: the base set-diff over the printed keys
for the deck130 pair counts **4** where **2** rows appeared. `#W78-CV S9b DISAPPEARANCE`
(the rewritten wave-77 R2c pin): the base build prints the run sentence and no gone
clause for the minusOne window, and counts that window into the unchanged run.

**Fix.** `w78StripHandleOrdinal` removes ` #<digits>` from the counting key (a RENDER
handle: it says which of two identical instances a row addresses and changes without the
action changing); `w78HoldRowKeys` re-attaches MULTIPLICITY as an occurrence index, so two
rows that normalise alike are two instances; `w78HoldRowDelta` returns **unseen and gone**
as a two-sided diff. `W76HoldMemory` gains `lastRows`/`pendingRows` (the committed
window's row keys in order) because a `std::set` cannot carry multiplicity; the LATCH
still reads `last` and is untouched. `holdReopenNoteText` gains a fourth argument
`goneRows` (defaulted 0, so every existing caller reads byte-identically) and states both
directions; a disappearance breaks the unchanged run.

**GREEN.** 12 pins. The ordinal stripper positive + two MUST-NOT-MATCHes (mana braces,
a bare `#`); the deck130 pair at 2 new / 0 gone against the base's 4; the five cases the
wave-76 lesson requires — **ZERO, ONE, REORDER, DISAPPEARANCE, RENUMBERING**; both printed
sentences (both-directions, pure-disappearance) and the empty string when nothing is
measured; and the run breaking on a disappearance through the live note builder.
The wave-77 `#W77-CR R2c DISAPPEARANCE the bracket says SEEN` pin is **superseded and
rewritten in place**, marked as such with why.

**Prediction.** Next corpus: no hold-check bracket's `N rows above are new` exceeds the
count of rows whose ordinal-stripped action keys are absent from the referent, and at
least one bracket prints the `... is gone and no row above is new` sentence.
**Pin for the negative:** a corpus with zero `is gone` sentences AND a seat that lost a
row means the delta is not being fed the referent's row list — check
`m.lastRows` is populated (the fallback to the wave-77 one-directional arithmetic fires
only when it is not).

---

## S11 MED — `forced_close_unrecorded` 5 → 10 with every arm-bound counter at 0

**Repro and the answer to "why does the bound never fire".** `force_close_arms_refused` /
`_deferred` / `_same_arm_deferred` / `_defer_bound_hits` are 0 for the fourth wave while
`forced_close_unrecorded` doubled; the identity `recovered 15 + missing 1 + unrecorded 10
= 26 stderr closes` holds corpus-wide. Reading the three increment sites:
`mForceCloseUnrecorded` rises at (i) `gptForceCloseArm` when a close is armed while this
arm's previous one is outstanding — reachable only past the 8-tick bound, so the deferral
counters would have fired FIRST; and (ii) **`:18367`, "decision changed under a pending
retry: drop it"** — which no arm bound watches at all. The ten are (ii). **The bound is
not broken; it was looking elsewhere.** That is also why six of the ten are in games with
no `reasoning_forced_close` record: the decode was bought and thrown away with the
question that moved.

**RED evidence (executed).** `#W78-CV S11 REPRO` walks the drop path through the live
`gptForceCloseArm` / `w76ForceCloseDecision`: `unrecorded == 1`, `same_arm_deferred == 0`.
On the base build this path additionally writes **no record**, so the ten are unnameable
from a corpus — three waves of briefs have asked reviewers to name them from a bare
integer.

**Fix.** `writeForceCloseRecord(outcome, landArm)` emits one record per EVENT —
`kind: forced_close`, `event` (the per-seat ordinal), `outcome`, `arm` (land/cast),
`window_seq`, `park_armed`, `defer_ticks`, `unrecorded_so_far`, turn, phase — at all four
sites: `dropped_decision_moved`, `deferred_same_arm_busy` / `deferred_park_busy`,
`armed` / `armed_superseding_outstanding`, and
`armed_after_defer_cleared` / `armed_at_defer_bound` / `armed_after_defer_superseding`.
It is telemetry about a window still in flight, so its kind takes no window-counter seq
(the `wall_miss` rule). Gameend field `forced_close_events`.

**GREEN / the fixture the brief requires.** `#W78-CV S11 GREEN` replays the caller's tick
loop through the LIVE `w76ForceCloseDecision` / `w76ForceCloseArmAllowed` /
`gptForceCloseArm` with **two closes earned on ONE arm and an EMPTY park**:
`force_close_same_arm_deferred == 1`, `arms_refused == 0`, `unrecorded == 0`,
`events == 2` — the counter that has read 0 for four waves reads **1**. A second pin walks
the deferral to the 8-tick ceiling and shows it arms anyway (bounded wait, never a
permanent pending).

**Prediction.** Next corpus: `forced_close_events == force_close_arms_refused +
force_close_same_arm_deferred + (armed events)`, the `forced_close` records sum to the
stderr `unclosed <think>` close lines per seat, and every one of the wave-77 "ten" carries
`outcome: dropped_decision_moved`. **Pin for the negative:** if any `forced_close` record
carries an outcome other than `dropped_decision_moved` while
`force_close_same_arm_deferred` is still 0, the same-arm bound has a live path this lane
did not find — the record names the arm and the window to look at.

---

## Weakest evidence

1. **S3's `declinedN` gate is inferred from one shape.** The twelve-window run is read out
   of the corpus, but the decline count at each of seqs 42-52 is NOT in the translog — the
   `[you declined this exact list N times already this turn]` note is prompt-only and I
   verified only that the option-set key is annotation-stripped (so the `{if you pass
   here...}` clause added at seq 42 does not mint a new key). If that clause were to
   survive `optionSetKeyOf`, `declinedN` would be 0 at seq 42 and the collapse would cover
   eleven windows instead of ten-plus-one. The pinned walk assumes the key is stable
   across it. Nothing is lost if the assumption is wrong — the window is asked.
2. **S4's `theirTriggers` count is untested against a live seven-link stack.** The walk is
   the same one `w77OwnLoopStackState` uses and the note is pinned pure, but no fixture
   builds a real 7-object opponent trigger stack; the hold-across-the-drain pin drives the
   latch and the bracket with rows held byte-identical BY CONSTRUCTION, which is what the
   corpus shows but not what a live drain guarantees (a trigger that changes a row's price
   would move the bracket and correctly suppress the clause — untested live).
3. **S6 is validated on one window in the whole corpus.** `own_loop_windows_asked` is 1.
   The THREATENED face has NEVER occurred in any corpus (engine-seat: "no THREATENED text
   anywhere"), so its rendered line is pinned and never observed.
4. **S7's record-seq stamp is only exercised when logging is on.** `w78HoldWindowRecordSeq`
   is called after the logging-off early return, so with the translog off
   `measuredRefRecord` stays -1 and the field is simply absent — correct, but it means the
   join is a telemetry-only guarantee and the suite cannot see it.
5. **S9's multiset delta is pinned on synthetic key lists.** The deck130 rows were
   transcribed from the corpus by hand into normalised key form; I did not run
   `holdActionKeyRow` over the real rendered rows to confirm the ordinal survives its
   stripping in exactly the ` #N` form the stripper expects. If some renderer emits `#1`
   without a preceding space, the stripper misses it and the bracket over-counts as before.
6. **What the translog cannot show at all for this lane:** (a) whether a window the
   collapse skipped would have been answered differently — a collapsed window writes no
   record, so S3's benefit is visible only as absence; (b) whether the model READ the drain
   clause or the verdict line — annotation-present-vs-outcome is a cross-tab, never a
   causal claim; (c) the four `forced_close` deferral outcomes, which no corpus has ever
   produced, so those three record shapes are unexercised outside PARSETEST.
