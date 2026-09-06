# Wave-66 lane AS — H3, H7 + the two MED items

Base: master `bdd0ec6e5` (worktree `w66-lane-AS`, branch `w66-lane-AS`). Everything below is code
reading, PARSETEST cases written on the corpus's own rendered lines, two suite fixtures, and a
measured RED counterfactual on this tree. **No live model probe and no corpus run.**

Corpus read (read-only): `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/`.

---

## H3a — the repeat row prints a verdict, and the stop it demands is read back

**Mechanism.** Two halves, both keyed on the contract the row ALREADY states ("a count on this row
REQUIRES a PLAN line stating your stop count, the count you are at now, and how many you perform
this window").

* **Render** (`src/AIPlayerGPT.cpp`): new pure `repeatPlanScanNumber` / `repeatPlanStopAndCurrent`
  read `stop` and `M` out of a PLAN in every spelling the corpus wrote them (`stop=20`, `stop 24`,
  `stop: 18`, `stop (33)`; `M=68`, `M 24 now`, `M is 101 now`, `M (41)`), and new pure
  `repeatRowStopClause(creatureCount, statedStop)` renders
  `{right now: M=68, your stated stop=20, so this window would add to a count ALREADY AT OR PAST
  your own stop - past your stop = a wasted window}` (or, below the stop, the remaining headroom).
  The stop comes from the CARRIED plan (`mCurrentPlan`), which is the only plan text the prompt
  carries forward, so the row can never state a stop the model cannot see one line above it. Both
  numbers are facts of THIS window — the engine's own creature count and a plan fixed for the
  window — so two rebuilds of one window render identical bytes (wave-61 rule). It is a `{...}`
  group, so `stripRenderAnnotationsLc` keeps it out of the option-set key, the declined count and
  the deadlock key, exactly like the cleanup price.
* **Refusal**: a counted take whose own PLAN says `stop - current <= 0` earns ONE re-ask,
  `repeat_past_stop`, quoting both of the reply's own numbers — the same lane `plan_missing` uses,
  worded for this shape, with `repeat_past_stop_recovered/_exhausted/_unanswered` stamps. The
  second answer executes as given, whatever it is. **This is not a cap**: no count is clamped, no
  row is withheld, `kRepeatRowMax` is untouched, and the refusal rests entirely on a number the
  model itself wrote in the same reply.

**The corpus this is measured against.** `123v162`: `stop=20` written in the PLAN and then x33 at
M=68, x21 at M=111, x20 at M=132, x47 at M=153, x30 at M=200, x21 at M=232, x13 at M=253, x200 at
M=230, x2 at M=301 — sixteen windows in that game whose CHOICE line names a count while the same
reply's PLAN says the stop is passed (several of them even close `x33 / pass (stop reached)`).

**Prediction (falsifiable, wave-66 corpus):** no window in the corpus takes a counted repeat whose
own PLAN line names a stop at or below the count it names as current. Falsifier: any record with
`repeat_n >= 1` whose reply contains both a `stop` number and an `M` number with `M >= stop` and no
`repeat_past_stop` stamp. Second, narrower: every repeat row rendered on a token-making ability in
a window whose carried plan names a stop carries a `{right now: M=..` clause.

## H3b — the loop that bills a round trip per link

**Mechanism.** Two parts, split on what the oracle says the seat can do.

* **No legal action at all**: new `AIPlayerGPT::loopAutoPassApplies(oppLoopProven, anyLegalAction)`
  = `oppLoopProven && !anyLegalAction`, read off the board by `loopAutoPassFor(p)`
  (`lifeLoopProvenWin(p->opponent())` first — it is the cheap read and this runs every AI tick —
  then `LegalActionsOracle::hasAnyLegalAction(p)`, the engine's own phase-aware predicate, which
  covers land drops and sorcery-speed abilities, not just instants). Both seams (priority and cast)
  unwind through the SAME return the hold row already uses. A run is counted and narrated ONCE as
  a receipt when it ends (`Their life LOOP ran N links while you had no legal action; the engine
  passed each one for you.`) with one translog record stamped `loop_autopass`. Fails closed on
  every doubt.
* **A legal answer in hand**: nothing is auto-passed — the window is a real decision and is still
  asked, every link. What was missing is the fact that makes ONE answer serve the run, so
  `loopChainingNote` states it (`[LOOP RUNNING: ... this same question is put to you again for
  every link of it, with only its numbers moved. Nothing on this list stops the chain. If your
  answer would be the same for every link, the HOLD row answers all of them at once ...]`).
  PROMPT-ONLY, spliced into the same channel as the declined count and the hold check
  (`promptNotes` / `mNextAskPromptNote`), constant text with no digit in it, so it can reach
  neither the ask key nor `mPromptTail`.

**Instrument (new).** No fixture could reach the seam (the suite has no model seat), so
`src/TestSuiteAI.cpp` gains `assertloopautopass <0|1> [1|2]`, which calls the same predicate with
the same two inputs the seam reads, against a REAL board. Fixtures
`bin/Res/test/w66as_loop_autopass.txt` (player 1 faces Sanguine Bond + Exquisite Blood and can do
nothing -> 1; player 2 holds the loop and faces none -> 0) and
`w66as_loop_autopass_has_action.txt` (same loop, Shock payable from the pool -> 0), both in
`_tests.txt`.

**Prediction:** 0 windows in the wave-66 corpus are put to a seat that `hasAnyLegalAction` says has
no action while the opponent's proven life loop is on the board; every `loop_autopass` record is
followed in the narration by exactly one "Their life LOOP ran N links" sentence. Falsifier: a
`loop_autopass` record with no matching narration line, or a run of `Cast nothing` answers on a
menu whose only rows are the two declines while both loop halves stand. Second: the 19-window
casting run of `130v126`'s shape does not recur — see H7's prediction, which is the half that
actually pays for it.

## H7a — the hold key stops breaking on a number that moved

**Mechanism** (`holdKeyRow`, via new pure `holdKeyNumbersNormalised` + `holdKeyManaCostBraceAt`):
digit runs are normalised to `#` **only inside a `[...]` or `{...}` annotation**, never in the
row's own text; an instance ordinal (`#2`) and a mana cost (`{2}`, wherever it sits, including
inside a bracket) keep their digits, because those name WHICH card and WHAT it costs. Every WORD of
every annotation is still compared byte for byte.

**Measured on the corpus's own rows.** `152v162` T15: nine consecutive no-op priority windows,
334 s, over ONE Clue row whose only delta between windows was `you would be at 4` -> `3` -> `2`
-> `1`; the model took the hold row at s41/s43/s47/s48 and was re-asked each time.
`130v126` seqs 37-55: NINETEEN casting windows on one Spark Spray row whose only delta was
`at life 17 leaves them at 16` -> `at life 18 leaves them at 17`, while the seat died 19 -> 1 and
took the hold row at s36/s41/s43/s53. Both pairs are pinned verbatim as PARSETEST cases.
**What is not forgiven** is pinned on the same rows: `; this KILLS you` arriving at the zero
crossing (`152v162` s44 -> s45) is a WORD, and it still re-opens the window. That is the line
between forgiving drift and the blind cache the latch must not become — and the hold row's own
clause now names the forgiveness and its limit, as waves 63 and 64 did for their two clauses.

Shared choke point checked: `holdKeyRow` is read by `takeHold` and `holdStillStands` and by nothing
else. The option-set key drops every bracket already (`stripRenderAnnotationsLc`), and the ask key
and the async slot key are built from prompt text, not from this — nothing this lane adds enters
`mPromptTail`, the ask key, the hold-note channel or the option-set key.

**Three pre-existing cases were AMENDED, not deleted**, because they pinned the behaviour this item
changes: `#W56-A D1`'s reprice case (now pins BOTH sides — a digit-only reprice keeps the hold, a
WORD reprice re-opens it), the `at26/at25` life-tick pair (the two keys now AGREE on that repro),
and `#W63-AD E10`'s "prices stay in it" case (now pins that every WORD and every mana cost stay in
it). The two clause literals (`#W61-U C14`, `#W57-A D4`) were updated in place.

**Prediction:** no run of >= 3 consecutive windows in the wave-66 corpus at one seam presents a row
set that differs from the previous window's only in digits inside brackets or braces. Falsifier:
any two consecutive records at the same seam whose `options_text` differ only in digits inside
`[...]`/`{...}` where the earlier reply took the hold row. Second: `152v162`'s draw-step shape (9
windows, one Clue row) does not recur at any seat.

## H7b — which decline is the default

**Mechanism** (cast seam, `FindCardToPlay`): the two decline rows are not equivalent — the plain
decline closes ONE window, the hold closes the run — and the wave-65 corpus answered with the first
one it read (deck123: 182 plain declines to 4 holds; deck125: 213 to 29). Where
`LegalActionsOracle::hasInstantResponse(this)` is true the engine WILL put this question again, so
`holdRowLine()` is INSERTED ahead of `Cast nothing right now` and the plain decline stays last.
No row is removed and nothing is renumbered blind: the decline's index is now the variable
`declineRowIdx` assigned by that block and read by the consumer (which previously assumed
`pick == candidates.size()`), `holdRow` is assigned the same way, and `holdRowIndexOf` finds the
hold row by its own reserved text rather than by position. The option-set key (`listKey`) is still
computed over casts + decline BEFORE either insertion, so the declined-list count is unaffected by
the ordering.

**Prediction:** the hold/plain-decline ratio at the casting seam rises on the seats that hold
instant-speed rows (deck123, deck125), and total priority+cast decisions per game do not rise.
Falsifier: a corpus in which `Cast nothing right now` still outnumbers the hold row by more than
20:1 on a seat whose windows are dominated by runs of byte-identical menus.

## MED (deck123) — `index_name_conflict` re-asked on a UNIQUE name

**Mechanism** (`parseChoice`, the reserved-name branches): where the named row is UNIQUE on the
menu — which the reserved HOLD row and row 0's own pass label always are — the divergence is
stamped `index_name_unique_name` and the named row is taken, with no re-ask. All three
`index_name_conflict` re-asks in the wave-65 corpus were this shape (`123v162` s29/s68/s111, every
one `CHOICE: 2 (Hold priority)` over a three-row menu) and all three recovered to the row the reply
had already named unambiguously. The genuinely ambiguous shape — the coded index names the HOLD row
while the echo names row 0 (`pass_hold_ambiguous`) — keeps `index_name_conflict` and its re-ask, and
so does `echo_index_conflict_ambiguous`. This is not wave 64's silent name-wins: the conflict is
still recorded on every window it happens on.

**Prediction:** 0 `index_name_conflict` re-asks in the wave-66 corpus name a row that appears once
on the menu; every such window instead carries `index_name_unique_name` and the row the reply named.

## MED (deck123) — the casting menu says it has no row 0

**Mechanism**: `kCastNoRowZeroFact`, a constant appended to the casting question — *"This menu has
no row 0: to decline, take the numbered "Cast nothing right now" row on the list below. (A priority
menu does have a row 0; this one does not.)"* `123v162` s45 answered `CHOICE: 0 (pass)` on a casting
menu and paid the corpus's only `no_pass_reask`; the re-ask already says this sentence, and saying
it before the answer costs one line and no round trip. Constant text, so it cannot move between two
rebuilds of a window.

**Prediction:** 0 `no_pass_reask` records in the wave-66 corpus. Falsifier: any `CHOICE: 0` answer
to an `ask`-kind (casting) window.

---

## Gate

Clean rebuild in this worktree (`rm -f bin/wagic && make -f Makefile.sdl -j4`, qmake untouched, no
new sources). Detached unit `w66-AS-gate2`, `MemoryMax=4G` / `MemorySwapMax=0`, foreground
until-loop wait on `~/.gatelogs/W66_AS_DONE`. Every wagic run under the memory cap.

| leg | result | base (brief) |
|---|---|---|
| build | clean link | — |
| PARSETEST | **4744 passed, 0 failed** | 4698 (**+46**) |
| suite, `WAGIC_TESTSUITE_THREADS=1` (ground truth) | **1273 tests (0 failed), 67 AI tests (0 failed)** | 1271 / 67 (+2 fixtures) |
| `==Test Failed !==` / `==Test timed out` (T1) | **0** / **0** | — |
| suite, default threads | 1273 tests (2 failed), 67 AI/0, 0 timeouts | — |
| the default-threads failures | `lifeline.txt`, `merrow_reejerey.txt` — the brief's known concurrency-only pair | — |

`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. The scratch registry `test/_probe_as.txt` was
deleted.

### RED-on-base — measured, not asserted

Every mechanism was reverted to its wave-65 behaviour with the new cases in place and the binary
rebuilt (`holdKeyRow` returning `core`; the hold row's new sentence removed; `repeatRowStopClause`
returning ""; `repeatPlanStopAndCurrent` reading nothing; `loopAutoPassApplies` returning false;
`loopChainingNote` returning ""; the unique-name stamp back to `index_name_conflict`;
`kCastNoRowZeroFact` emptied):

**`4720 passed, 24 failed`** (log `~/.gatelogs/w66-AS-red.log`). 4720 + 24 = 4744, so **no
pre-existing case goes RED under the reversion and none was deleted** — the other 22 new cases are
MUST-NOT-MATCH, NEGATIVE, CONTROL, KEY-safety and echo pins that pass on base too, which is what
they are for. The 24 split: H3 render/parse 7, H3b 2, H7 8 (4 of them the amended pre-existing
pins), MED unique-name 3, MED no-row-0 2, plus the two amended clause literals.

Fixture RED, same reverted binary: `w66as_loop_autopass.txt` **FAILS** —
`assertloopautopass: player 1 expected 1 got 0 (anyLegalAction=0)`. GREEN after. The negative
fixture (`_has_action`) is green on both sides, which is what a MUST-NOT-MATCH is for.

---

## What I did NOT verify

* **No live model probe and no corpus run.** Every model-facing claim (that the pilot reads the new
  `{right now: M=..}` verdict, that it presses the hold row more often now that it is the first
  decline, that the LOOP RUNNING note changes an answer) is unmeasured. The wave-66 corpus is the
  instrument; the predictions above are written to be checked against it.
* **H7b has no pure RED surface.** Its PARSETEST cases pin that the reordered menu is read
  correctly (finder, parser, key), not that the ordering ships — the ordering lives in
  `FindCardToPlay`, which the suite cannot drive. I read the consumer chain (`pick ==
  declineRowIdx`, `pick == holdRow`, `candidates[pick]`, the per-attempt `opts`/`rowUses`/
  `rowSweep*` erasures) and the indices are assigned rather than assumed, but the only proof that
  no consumer still assumes a position is the suite's 0-failed and my reading.
* **`hasInstantResponse` is the gate for H7b and `hasAnyLegalAction` for H3b.** I chose the broader
  oracle for the auto-pass deliberately (a land drop is legal at sorcery speed and
  `hasInstantResponse` would not see it), but I did not construct a board where the two disagree
  under a proven loop and confirm the seam's behaviour there.
* **The auto-pass may fire rarely or never in the corpus.** `GameObserver` already auto-passes a
  seat with no instant response in most phases, so the class of window this catches is narrow; the
  19-window run it was docketed against is the OTHER half (the seat held an instant), and that half
  is paid for by H7a and the LOOP RUNNING note, not by the auto-pass. If the wave-66 corpus shows 0
  `loop_autopass` records that is consistent with the fix, not evidence for it.
* **The hold key's numeric normalisation is a JUDGEMENT about where drift ends.** A row whose
  MAGNITUDE genuinely changed while every word stayed the same (`{right now: drains 3}` ->
  `{drains 5}`) is now forgiven by the latch. I argue that is correct — the row offers the same
  card against the same objects, the model already declined it, and the LATCH still retires on any
  new or vanished row — but I did not sweep the emitters for a clause whose whole decision content
  is a bare number with no word around it. If the wave-66 corpus shows a hold surviving a window
  where a magnitude flip should have changed the answer, the dial is the class of forgiven group,
  not the doctrine.
* **`repeatPlanScanNumber` reads the FIRST occurrence** of each label. Every corpus PLAN states its
  stop once and its M once, but a reply that restates them in a different order mid-line (or writes
  a third number in a `stop`-shaped phrase) could be misread. The failure mode is one wasted re-ask
  or one missed refusal, never a wrong execution: the refusal only ever routes to the same one
  re-ask the row already promised, and the second answer executes as given.
* **The `{right now: M=..}` clause's stop is the CARRIED plan's**, which may be older than this
  window (the plan-age clause is elsewhere and I did not tie the two together). A stale stop states
  a true fact about what the pilot last said, not about what it now intends.
* **The two known concurrency-only failures** (`lifeline`, `merrow_reejerey`) were reproduced at
  default threads and read 0 single-threaded, as the brief describes;
  `intrepid_adversary_repeated_payment.txt` passed in both runs and its rate was not exercised.
* **No cross-lane interaction was tested.** Lane AR owns `gptSelectAnswerIndex`, the reply protocol
  and `name_over_index`; my MED change touches `parseChoice`'s reserved-name branches, which is the
  same function. Merge-time looks worth taking: the `index_name_conflict` stamp (AR routes MORE
  conflicts through it; I route the unique-name subset AWAY from it — those two must be reconciled
  deliberately, not by whichever hunk lands last), and lane AL's `modeRowAnnotations` if it grows a
  `{...}` group whose only content is a number (the hold key now forgives those).
