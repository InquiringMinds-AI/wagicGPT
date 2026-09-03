# Wave-56 lane A — the HOLD latch and the priority/async seams: D1, D18, D16, D11, D4

Base: `master 41d1c15ef` (src identical to `b228b8648`), worktree `worktrees/lanes/w56-A`,
branch `w56-lane-A`. One source file and its header: `projects/mtg/src/AIPlayerGPT.cpp`,
`projects/mtg/include/AIPlayerGPT.h`, both spliced as bytes (python `rb`/`wb`).
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. Every comment tagged `#W56-A (Dn)`.

**Base numbers are INHERITED, not re-derived on this worktree.** I built over the seeded
binary before running a base gate; lane B's base gate on the identical seed
(`~/.gatelogs/w56-B-base-{parsetest,suite}.log`) reads PARSETEST **2636 / 0** and suite
**1229 tests (2 failed — the known concurrency-only pair), 51 AI (0 failed)**, which matches
the brief. That is the baseline this lane is measured against.

---

## D1 (HIGH) — the hold's re-open predicate is the PRINTED ROWS, and nothing else

`holdBoardKeyOf` and `holdKeyDroppedLine` are **deleted**, `mHoldBoard` with them.
`holdStillStands(heldRows, nowRows, why)` is now the whole predicate: every row printed at
this window must be, **byte for byte**, a row that was printed when the model took the hold —
name, cost, ordinals and each row's `{right now: ...}` price included. `takeHold` stores the
rendered rows (it stored `optionSetKeyLine` keys; wave 55). A row DISAPPEARING is still not a
re-opener (the model already declined it). `holdHonoured`/`takeHold` no longer take a
situation argument, so the four call sites at the priority and cast seams pass rows only.

The declined-note key (`optionSetKeyOf`, wave-55 D2a) is **untouched** and now deliberately
diverges from the hold key: the note still reads 21 rendered tuples of a repricing menu as ONE
list, while the hold re-opens on the reprice. Both statements are true of different questions
("how many times have you declined this list" vs "is this the screen you closed").

**Stated risk, unchanged from the docket and not mitigated here:** a board change that alters
the answer's VALUE without altering any printed row — an incoming lethal attack under an
unchanged menu — no longer re-opens that seam until a printed row moves or the turn ends.
Bounded (every cast, every combat declaration and every price that tracks the board changes a
printed row) but real, and it is why this is a predicate change and not a suppression. **No
cap, no window removed:** a held window is one the model closed by pressing a row, and a seat
that has not pressed one is never held.

**Prediction.** Decisions inside a single-phase mandatory life-loss loop **< 5** (was 13, 15,
16); HOLD takes followed by a byte-identical same-turn re-ask **< 20 of N** (was 113 of 282);
identical-declined runs **< 15%** of decisions, max run **< 10** (was 25.2% / 16); inference
inside those runs **< 6%** (was 12.4%). **Falsifier:** the runs do not shrink -> the re-opener
was never the key and the item becomes the loop's own priority grant.
**Second falsifier, specific to this predicate:** a corpus loop whose rows carry a
life-tracking price still spends >= 20 windows -> the hold key must drop annotations after all
and the divergence above was the wrong call.

## D18 (MED) — the async slot is `(seam, turn, phase, boardKey)`

`AsyncState` gains `slotKey`, written at launch from `asyncSlotKeyOf(forceClose, turn, phase,
seamTail, board)`; `assemblePrompt` records its `tail` (the question AND its numbered option
list) in `mPromptTail`, which is the seam half. On poll, an answer whose prompt text no longer
matches is **consumed anyway when the slot still matches** — the reply's indices address
exactly the rows it was shown, since the tail is byte-identical — and dropped as before when
the slot changed (the land-drop/casting alternation across the turn flip, 20 + 16 of the 43).
The force-close retry is its own slot. The livelock breaker and the stale-drop streak are
untouched. New stderr line on the rescue: `consuming an in-flight answer whose prompt text
drifted (same seam, turn, phase and board)`.

**Prediction:** `dropping stale async answer` **< 20** corpus-wide (was 43), with the
difference appearing as the new consume line rather than vanishing; **0** of the remaining
drops preceded by a `10DrawAction`/`NextGamePhase` resolution on an unchanged seam.

## D16 (MED) — `plan_choice_conflict` needs a rival row

The re-ask now also requires `proseNamesOtherMenuRow(reply, rows, choice)`: the reply's PROSE
(post-`</think>`, after the first coded line, every further coded line removed) must name a
row of THIS menu that is not the row the CHOICE took. Rows with no prose name — the decline,
pass and hold rows — are never rivals; a name contained in the taken row's own text is not a
rival (a kicked/unkicked pair). **The census is kept broad:** `decision_reversed_in_prose` is
still stamped on the old condition, and the narrowed-away shape is stamped
`plan_choice_conflict_narrowed`, so the item is measurable rather than merely quieter.

**Prediction:** `plan_choice_conflict` **<= 4**, none of them a reply whose prose names no
other row; `plan_choice_conflict_narrowed` accounts for the difference (wave 55: 9 firings,
5 `_exhausted`).

## D11 (MED) — the carried plan is withdrawn, not served under its own retraction

Two pure rules in `assemblePrompt`: (a) `planHardAged(windows)` at **40** windows, and (b)
`planRetractionServedAlone(windows, noteFires)` when the menu-diff note fires on a plan more
than **4** windows old. Either withdraws the plan: one sentence,
`YOUR LAST PLAN is withdrawn (you stated it, N windows ago on turn T; "<card>" is no longer on
your menu): state a fresh PLAN with this window's answer.`, and `mCurrentPlan` /
`mPlanEchoCount` / `mPlanSetSeq` are cleared, so the reply rules' own "no plan shown yet"
clause asks for a new one. A withdrawal is a TRUE token, not a silent deletion — the pilot is
told the plan is gone; it rules on nothing and advises nothing.

**Prediction:** `plan_echo_count` max **< 60** (was 143); records serving a plan over 40
windows old **0** (was 328); no record serves a plan block and its retraction together.

## D4 (MED) — `chosen_text` is never empty while `choice >= 0`

`writeTransLog` fills the field from `refusedChosenText(choice, fallback, optionTexts)` when
the caller had nothing executed to name: `<refused: plan_choice_conflict>` (the class that
stopped it), else `<refused: not executed: <row text>>`, else `<refused: not executed: row N>`.
Every value is marked, so a harvest can never mistake a refusal for an executed row.

**Prediction:** records with `choice >= 0` and empty/absent `chosen_text`: **0** (was 9); every
re-ask record instead carries a `<refused: ...>` string naming its class.

---

## Tests

**PARSETEST 2636 -> 2653 (+17), 0 failed.** Four new sections (`#W56-A D4`, `D11`, `D16`,
`D18`) and two amended wave-53/54/55 blocks. The delta is net: the wave-53/54/55
`holdBoardKeyOf` line-by-line pins are **deleted with the function** and replaced by the D1
block, so +17 is (new cases) - (retired board-key cases).

* **#W56-A D1** (rewrote `[#W53-N D2] the hold's re-openers` and `[#W54-A D2c]`) — the pin the
  brief asked for, on composed **situation + rows pairs**: a stack-top change (the 123v126
  Sanguine Bond / Exquisite Blood alternation), a phase step, a life tick, a poison counter, an
  arriving permanent and the opponent's draw all leave the hold STANDING while no printed row
  moves; a `{right now:}` price change re-opens it and says why; a newly available row
  re-opens it; a disappearing row does not; and a must-NOT-match NEGATIVE pins that
  `optionSetKeyOf` (the declined-note's key) still collapses the two priced menus that the
  hold key now separates. The wave-55 D2a/D2b loop block is amended to assert the new
  behaviour explicitly rather than being deleted.
* **#W56-A D4** — the three value shapes, a NEGATIVE that every value is marked `<refused:`,
  and the ECHO SHAPE (the string echoes no row of the menu it was written for).
* **#W56-A D11** — both thresholds at their boundaries (39/40, 4/5), the 152v125 seq-101 repro
  rendered as ONE sentence, the hard-age wording, and a NEGATIVE that the withdrawal rules on
  nothing and carries no plan block.
* **#W56-A D16** — the prose-name extractor (positive + the three no-name rows), the 9 firings'
  own shape now NOT firing, a genuine rival still firing and naming the row, and four
  NEGATIVEs (the taken row itself, a name inside `<think>`, a second coded line, a name
  contained in the taken row).
* **#W56-A D18** — the same seam/turn/phase/board is one slot; lane E's two drift questions are
  two slots; turn flip, phase step, moved board and the force-close tag each separate slots.

**No suite fixture, and the reason is structural** (same as wave-54 lanes A/N and wave-55 lane
A): every behaviour here lives inside `AIPlayerGPT` behind `mEndpoint`/`askModel`, i.e. behind
a LIVE MODEL ENDPOINT. The suite's AI fixtures run the Baka heuristic with no endpoint, so any
fixture pinning the hold predicate, the async slot, the plan withdrawal, the conflict
narrowing or the translog field would be GREEN ON BASE — the opposite of a pin. The diff
touches no non-GPT engine path (two files, both `AIPlayerGPT`). The PARSETEST cases against
the pure helpers the seams call are the pins (each RED on base: the helpers do not exist there,
and the amended cases assert the opposite of what base asserts), and the full suite is the
regression gate.

## Gate (this worktree, hermetic `make -B -j4`, single-threaded suite, 4 G memory cap)

| | base (lane B's gate on the identical seed) | after |
|---|---|---|
| PARSETEST | 2636 passed / **0 failed** | **2653 passed / 0 failed** (+17) |
| suite | 1229 tests / 2 failed (known concurrency pair) | **1229 tests / 0 failed** |
| AI suite | 51 / **0 failed** | **51 / 0 failed** |
| timeouts | 0 | **0** |
| memory kills under the 4 G cap | 0 | **0** |

Logs: `~/.gatelogs/w56-A-build.log` (build rc=0), `~/.gatelogs/w56-A-post-parsetest.log`,
`~/.gatelogs/w56-A-post-suite.log`; base `~/.gatelogs/w56-B-base-{parsetest,suite}.log`.

## What I did NOT verify

* **No live-model probe and no live game.** Nothing here was exercised against
  `qwen36-35b-a3b`, no stub-server run, no `WAGIC_GPT_DRIFT=1` dump on this binary. Every
  claim about what the pilot DOES with a held window, a withdrawn plan or a narrowed re-ask is
  an untested prediction; the wave-56 corpus is the measurement.
* **D18's rescue has never been observed firing.** The slot key is pinned as a pure function
  only. I did not reproduce a drift on a stub game and watch the answer be consumed, so the
  claim "the drop count falls and the consume line appears" is an argument from the wave-55
  classification, not an observation. In particular I did NOT verify that `mPromptTail` is the
  tail of the prompt actually in flight at every one of the ~14 `pollCompletionRetry` call
  sites; it is set by `assemblePrompt`, and a seam that polls with a userMsg it did not
  assemble in the same tick would compute a slot key from another seam's tail. The failure
  mode if that happens is a MISSED rescue (slot mismatch -> the existing drop), not a wrong
  answer consumed — but a wrong-answer consume would require two seams to share turn, phase,
  board AND tail, which is the same question by construction.
* **D1's stated risk is unmeasured.** I did not enumerate the engine paths that can change the
  answer's value without changing a printed row; the argument (casts, combat declarations and
  board-tracking prices all print) is reasoning, not a proof, and the incoming-lethal case has
  no test.
* **D16's prose-name extractor is text-derived, not card-derived.** It reads names off the
  rendered row, so a row whose prose name the model writes differently (an abbreviation, a
  repeat row's "create vampire with X" shape written as "X") will not match and the re-ask will
  not fire. That is the conservative direction for this item (fewer firings) but it means the
  narrowing may also suppress a genuine conflict whose prose paraphrases the rival row. Not
  measured against the corpus's 9 records — I did not re-read them; the classification is the
  ledger's.
* **D11's thresholds (40 and 4) are the docket's numbers, not fitted ones.** I did not test
  what fraction of the corpus's 328 over-age records the hard age reaches, nor whether
  withdrawing at 40 costs a seat a plan it was still executing.
* **Base numbers were inherited from lane B**, not re-run on this worktree (see the header).
* The two known concurrency-only failures did not appear single-threaded and no third failure
  or timeout appeared; I did not run the threaded suite at all.
