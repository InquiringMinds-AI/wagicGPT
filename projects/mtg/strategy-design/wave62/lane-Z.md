# Wave-62 lane Z — D9, D10, D11, D12, D13, D14, D16, D18

Base: master `0d9b2d52e`. Branch `w62-lane-Z`.
Files: `src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`, `include/GptPlanCaveat.h`,
`src/TargetChooser.cpp`, `include/TargetChooser.h`, `bin/Res/test/_tests.txt`, and one new
fixture `bin/Res/test/w62Z_designation_marker_not_a_permanent.txt`.

Seven of the eight items live inside `AIPlayerGPT` (parse, re-ask, render, record). One - D16's
second half - is an engine rule and is the only item with a suite fixture; it is RED on base,
measured. The rest are PARSETEST-gated for the reason lanes H/M/Q/U/V state and this lane
re-confirms: the suite has no network seat, `askModel` returns at `mEndpoint.empty()` before any
of this code, and no fixture can make a model reply happen.

---

## The finding that reorganised the lane

D9 and D10 arrived as opposite complaints — D9 wants a later derived answer honoured, D10 wants a
committed answer not thrown away — and they turned out to be one defect with two faces.

`choiceRetractedNoReplacement` documents its own search region: *"the search region ends at the
first line-leading PLAN: marker"*, and it computed `regionEnd` for exactly that. **`regionEnd` was
never read.** Wave 24 retired branch (b), which had been its only consumer, and the bound went
with it — leaving the scan for a contradictory `CHOICE: n` token running over the ENTIRE reply,
including the PLAN block the engine itself carries forward and quotes back to the model as
`YOUR PLAN`. A dead variable, holding the proof that the intended bound had been lost.

Measured on this build before anything was changed (temporary probe, since removed):

```
PROBE parseChoice("CHOICE: 4 (Cast Teferi's Puzzle Box)", 6 rows) = 4   (162v126 seq 49's own answer line)
PROBE answerLine -> [ 4 (Cast Teferi's Puzzle Box)]  runLen=1 rejected=0
PROBE choiceRetractedNoReplacement(the recorded reply)      retracted=0 reanswer=-1
PROBE same reply + one appended "So CHOICE: 5 (...)"        retracted=1 reanswer=5
```

So the parser never mis-read the answer line at all — the answer was CORRECT and was then
overwritten, silently (no parse note is stamped on the `reanswer` path), by a coded token in the
model's own deliberation. `latched_row_mismatch` was the symptom stamp doing its job.

The rule the three seams now share, and the invariant behind it:
**the engine never reads an answer out of the text it carries forward as your plan.** The span is
not "everything after `PLAN:`": it is `planParagraphBound`'s own paragraph — the same span
`mCurrentPlan` keeps — computed by a new pure `planBlockEndOffset`. So the parser and the carry
agree on where the plan is.

## What shipped

**D9 — one answer rule at three seams, and it is written down.**
(a) The CHOICE seam's mid-prose recode is bounded to outside the PLAN block (above). (b) The two
COMBAT seams gain the mid-prose self-correction the CHOICE seam has had since #W48-E1, with the
same bound: `restatedCombatDirective` (pure) returns the LAST restated `BLOCKS:`/`ATTACK:` payload
that is outside the plan block, is not quoted or backticked, is not one of the protocol's own
worked examples, and is CLEAN by `combatLineIsClean`'s roster grammar. It is appended to the line
list so the shipped last-clean rule judges it exactly like a coded line — it is never trusted for
being later, only for being clean. Stamped `blocks_restated_prose_taken` /
`attack_restated_prose_taken`. (c) `kReplyProtocol` now states the rule: *"If you change your mind
while writing, write the corrected ANSWER LINE again, on a line of its own... The LAST answer line
you write is the one that runs. An answer written inside your PLAN sentence is part of the plan,
not your answer, and is NOT read as one."*

**D10 — the parser bug.** Above. Additionally, a contradictory coded index found INSIDE the plan
block is now measured and signed `plan_prose_recode_ignored` at both CHOICE seams, so the corpus
can count the shape instead of inferring it.

**D11 — the re-ask names the step it was asked about.** The existing `named_row_reask` already
quoted the offending name and the legal range (`146v125` s44 → s46 recovered on that line); what
it never said is that the action named belongs to a LATER decision this turn. `laterStepRouteClause`
(pure) fires only when the offending name LEADS with a combat-step verb AND this menu carries the
decline row that itself states combat is next, and then quotes that row by number and label. It is
silent otherwise, so it can never point at a row that does not exist. Both re-ask seams carry it.

**D12 — the survivable assignment is named.** `assignableRemainderDamage` now hands back the
`matchOfBlocker` it already computed (`matchOut`), so the printed pairing and the printed life come
out of one walk and cannot disagree. `blockAssignmentClause` (pure) renders it BY CARD NAME, not by
`B#`/`A#` label — this header is printed on windows that carry no B#/A# enumeration, and a label
that is not on the screen would be a false surface. Entries are separated by `; ` (#W61-U C10b's
rule: a card name can contain a comma). It rides ONLY the proven branch (`bestCaseOptimal`) and only
when the best case is survivable: on the floor branch no single assignment reaches the figure, so
naming one would claim more than the number does.

**D13 — the hold row's promise, and the two notes that contradicted each other.** The latch is per
SEAM (`holdHonoured`/`takeHold` are called with `"priority"` and with `"cast"`), so the row's
`{taking this row skips every later window whose rows are identical to these}` was a promise the
engine does not keep — nine same-turn leaks in six games, each a hold at one seam followed by the
identical row set re-asked at the other. The clause is now
`{taking this row skips every later window that asks THIS SAME question with rows identical to
these; a different question is still asked}` — scoped without naming an engine seam, and still
carrying no number (the row text is inside both ask keys). Separately, `[you declined this exact
list N times already this turn]` and `[hold check: 1 row above is new since the last window at this
seam]` are both true over DIFFERENT populations; the declined note now says which one it counted
(`- counted over every window this turn, not only the ones asked at this same seam`), with its
wave-53 literal head kept byte for byte because the deck guides key on it.

**D14 — the `{X}` cast row stops reading as a free row.** `leavesUntappedTag` is suppressed for
every `{X}` cost for a good reason (X is announced at the next window), but silence is not neutral
against a guide that teaches "a row carrying no such clause at all is a row where M is far above K".
`xCastRemainderScopeTag` (pure) replaces the silence with two facts that are exactly true at render
time and depend on no unmade payment: that the spend depends on the X announced next, and — since
`maxAnnounceableX` derives the largest X from the whole potential mana minus the base cost — that at
the X the row's own badge names no larger X is payable, so that cast leaves nothing more to spend on
X, and every step below it frees exactly one mana (of the X pip's colour, for a coloured X). Same
affordability call the `{X pricing:}` block makes, so the two brackets on the row cannot disagree.

**D16 — (a) the plan cut states its size.** `planTruncationNote` keeps the wave-60 literal as its
HEAD and adds the measured cut: `[...the rest of your plan was not carried: N further characters, of
M you wrote - restate it in a sentence or two if you still mean it]`. That is the fact that tells the
model its PLAN line was a deliberation stream rather than a plan (deck152 G3 seq 115's ~3 KB, quoted
back at seq 120/121). **(b) a designation marker is not a permanent.** `Night`/`Day` are
`type=Emblem` marker cards parked on a battlefield so the daybound machinery can find them
(`borderline.txt:76946`); CR 114.1 says an emblem is not a permanent and is in no zone, and
`TestSuiteAI`'s `isDesignationMarker` has treated them as invisible to every zone assertion since
wave 43. `TargetChooser::canTarget` — the universal choke point, so the rule holds for sacrifice,
destroy, exile, bounce and count alike — now rejects an Emblem-typed card unless the chooser says it
accepts one (`acceptsDesignationMarkers`: true for `TypeTargetChooser` when its own type list names
Emblem, true for `CardTargetChooser`, which is bound to one named object, false everywhere else).
`notaTarget(<6>*|mybattlefield)` bypasses the shroud branch, which is why the marker's own
protections never stopped Emrakul.

**D18 — the abandoned wall miss carries the phase its stamp proved.** `wallMissClassFor(phase)` +
`transportStampPhase` (both pure): a record whose #W61-U C13 stamp proves a phase is classed
`wall_miss_unrecorded_wall` / `_connect` / `_midflight`; one that proves none keeps
`wall_miss_unrecorded` byte for byte. The old name is a strict PREFIX of the new ones, so no corpus
key is renamed away. Note the correction the deck-130 seat's reading needs: `connect_ms=20000` is the
connect BUDGET (#W60-M B4), not evidence the connect failed — only elapsed time separates the two,
and at 100% of the deadline this is a wall miss. The class now repeats that verdict instead of
contradicting it.

---

## Gate

Detached unit `w62-Z-gate2`, `MemoryMax=4G MemorySwapMax=0`, binary rebuilt from clean
(`rm -f bin/wagic && make -f Makefile.sdl -j4`), every run memory-capped.

| | base (`0d9b2d52e`, measured on this worktree) | this lane |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1249 tests, 0 failed, 65 AI / 0 failed | **1249 tests, 0 failed, 66 AI / 0 failed** |
| `==Test Failed !==` / `==Test timed out` | 0 / 0 | **0 / 0** |
| PARSETEST | 3915 passed, 0 failed | **3969 passed, 0 failed** (+54) |

`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.
The 66th AI test is this lane's new fixture; nothing else moved.

## RED evidence per item

* **D16(b) — a real suite fixture, RED on base, measured.**
  `w62Z_designation_marker_not_a_permanent.txt`: Eldrazi Ravager (annihilator 1) attacks a defender
  whose battlefield is one `Night` marker and one `Forest`, so the sacrifice has exactly one legal
  object. Verified by reverting ONLY the `TargetChooser::canTarget` guard (`if (false && ...)`),
  rebuilding, and running the fixture scoped through `test/_probe.txt`:

  ```
  ==Card number not the same in player 1's graveyard==, expected 1, got 0
  ==Card ID not the same. Didn't find 288
  ==Card number not the same in player 1's battlefield==, expected 0, got 1
  ==Test Failed !==
  ```

  i.e. on base the marker absorbs the sacrifice and the Forest is still in play — the deck152 G3
  under-count, reproduced. The guard was then restored, the binary rebuilt from clean, and the
  fixture passes (`==Test Successful !==`, and inside the full suite).
* **D9 / D10 / D13 / D16(a) — RED produced by shipped base expectations that CONTRADICT the change.**
  The first build of this lane failed **14** pinned cases: three #W48-E1 and three #W49-S D2 cases on
  the mid-prose recode, two #W53-N D2 declined-note literals, the #W61-U C14 and #W57-A D4 hold-row
  benefit literals, and four #W60-M B13a / #W60-Q R8 truncation-marker cases. That is measured, not
  asserted. Their disposition is under "Cases updated" below.
* **D11, D12, D14, D18** add new functions and a new default argument; their cases cannot compile
  against base (`laterStepRouteClause`, `blockAssignmentClause`, the four-argument
  `assignableRemainderDamage`, the eleven-argument `incomingCombatLine`, `xCastRemainderScopeTag`,
  `wallMissClassFor`, `transportStampPhase`). Their old behaviour was a silent omission, so there is
  no base string to contradict; each ships an explicit NEGATIVE that the pre-change rendering is
  byte-identical wherever the new fact is absent (`incomingCombatLine(...)` with no assignment ==
  the wave-61 line; `wallMissClassFor("")` == the wave-55 string).
* **No suite fixture is possible for D9-D14, D16(a), D18** — no network seat, no model reply. Stated,
  not claimed around.

## Cases updated, none deleted

* The two synthetic #W49-S D2 POSITIVEs (`rOk`, `rRemap`) put their prose recode on the line straight
  after a one-line `PLAN: pass.`, and by the engine's OWN definition of the plan a line opening with a
  connective after a sentence end is still that paragraph. They are **re-sited** past a blank line, so
  the behaviour they pin is preserved, and a new NEGATIVE pins that the SAME recode left inside the
  plan block is not honoured and is signed. Every case pinned on a REAL corpus record is unchanged
  and still passes — #W48-E1's `r34` (deck123 vs146 seq 34, the wave-47 recovery) and #W49-S D2's
  `r25` (deck126 vs130 seq 25) both keep their verdicts, and `r34` has its own MUST-NOT-MATCH in the
  D10 block proving the wave-47 recode was not paid for.
* The #W53-N D2 declined-note literals, the #W61-U C14 / #W57-A D4 hold-row literals and the
  #W60-M/#W60-Q truncation cases are updated to the strings the engine now keeps; the truncation
  cases now read the marker's HEAD, which is unchanged.

## Predictions (falsifiable, next corpus)

1. **D10** — 0 records whose `choice` differs from the row named by an unambiguous first coded
   `CHOICE: n (name)` line while `coded_answers` is 1. Falsifier: any record with `coded_answers 1`,
   a `latched_row_mismatch` note and a `choice` that is not the first line's index. The new note
   `plan_prose_recode_ignored` appears on the windows that used to be silently overwritten; grep for
   it and check each one's executed row IS its line-1 answer.
2. **D9** — every `blocks_restated_prose_taken` / `attack_restated_prose_taken` record's executed
   declaration equals the LAST clean directive in the reply, and none of them is inside a `PLAN:`
   block. Falsifier: such a stamp on a record whose reply's only restatement is the protocol's worked
   example, or whose executed set matches neither the coded line nor a clean restatement.
   **Stated plainly: the parser change does NOT re-decide `146v152` seq 54.** That reply's
   `So BLOCKS: B2:A1.` is inside its PLAN sentence and stays plan text under the invariant above; what
   addresses that window is the protocol clause (c). The measurable prediction is therefore a
   BEHAVIOUR one: of the ~3.9% of replies that deliberate past the answer line, the share that state
   their final answer on a line of its own rises above 0, and `blocks_restated_prose_taken` /
   `attack_restated_prose_taken` fire at least once. Falsifier: the rate of replies whose PLAN
   contradicts their executed combat answer is unchanged.
3. **D11** — every `named_row_reask` whose offending name leads with an attack/block verb, on a menu
   carrying the `(combat comes next this turn)` row, quotes that row's number; and the deck126
   seq 44/65/82 shape recovers on the first re-ask rather than costing a further window.
4. **D12** — every blockers-seam header printing `best case with every blocker assigned: you would be
   at N` with N > 0 also prints `one legal assignment that reaches it:`, and every blocker named in
   it is on the same screen's battlefield. Falsifier: a header with the number and no assignment, or
   an assignment naming a creature that cannot legally block the attacker it is paired with. Outcome
   prediction, weaker: a reply saying "I have no other blockers" on a window whose header names one
   does not recur.
5. **D13** — 0 prompts carry `[you declined this exact list N times already this turn]` without its
   scope clause; and the hold row's `every later window whose rows are identical to these` string
   appears 0 times. The 9-leak class stays (the latch is unchanged), but no leak is now a broken
   promise: grep the leaks and check each re-ask is at the OTHER seam.
6. **D14** — 0 `Cast <X spell>` rows print no remainder clause at all: every one carries either a
   `{leaves ...}` count or the `{no {leaves ...} count on this row: ...}` scope tag. Falsifier: an
   `{X pricing:}` row with neither. Outcome prediction, weaker: deck125's three X-floor breaks on
   badged Revelation rows do not recur at the same rate.
7. **D16(a)** — every carried plan that was cut states the byte count it dropped; 0 `YOUR PLAN`
   blocks end in the bare wave-60 marker. **D16(b)** — 0 sacrifice/destroy/exile/target menus offer
   `Night`, `Day` or any `[emblem]`-typed object, and 0 annihilator resolutions name more objects
   than they put into a graveyard.
8. **D18** — every `wall_miss` record whose `transport` carries `phase=` is classed
   `wall_miss_unrecorded_<that phase>`, and no record is classed `wall_miss_unrecorded_connect` while
   its `deadline_pct` is >= 95.

## What I did NOT verify

* **No live model probe and no corpus run.** Every parse, render, re-ask and record change here is
  PARSETEST and code-reading only; no prompt was put to Spark.
* **The 2,433 trimmed bytes of `162v126` seq 49 are not recoverable.** `recordReplyTrimmed` dropped
  them, so I could not read the actual overriding token. What IS measured is that the visible reply
  parses to row 4 and does not retract, and that appending ONE mid-prose `CHOICE: 5` to that exact
  reply produces `reanswer=5` — the mechanism, not that record's literal tail. The `latched_row_mismatch`
  stamp, `coded_answers 1`, and `choice 5` are consistent with nothing else I can construct, but I am
  inferring the tail's content.
* **The plan-block boundary is `planParagraphBound`'s rule, and that rule is permissive** — its
  connective list includes `this`, `it`, `they`, `that`, so a new sentence opening with one of those
  after a sentence end stays inside the plan block and its coded tokens are not read as answers. That
  is deliberate (it is the same span the carry keeps) but it is a WIDER exclusion than "the PLAN
  sentence", and I did not measure how many wave-47-class recoveries fall inside it in a real corpus.
* **D9's combat restatement has no board-level test.** `restatedCombatDirective` is pinned pure; that
  the two seams feed it the right rosters and that `parseAttackerSet` / the last-clean walk then
  consume the appended line correctly is verified by reading, not by a running game.
* **D12's assignment is the matching the greedy found**, which is optimal in TOTAL prevented damage,
  not unique: an equally good pairing may exist and is not shown. The clause says "one legal
  assignment", not "the assignment". It also inherits every limitation the number has — no
  first-strike ordering, no damage already marked, no trample/menace (those take the floor branch,
  which prints no assignment at all).
* **D14's tag is not a source count.** It states mana, which is what `maxAnnounceableX` is denominated
  in; it deliberately does NOT claim how many SOURCES the cast taps, because that is the figure the
  suppressed `{leaves N of your M}` would have had to invent.
* **D16(b)'s blast radius is bounded by the suite, not by a sweep.** I did not enumerate every
  primitive that might legitimately want to choose an Emblem-typed object; the 1249-test suite and 66
  AI tests are the instrument, and they are clean. `CardTargetChooser` and an explicit
  `TypeTargetChooser` naming Emblem are exempted by construction. The monarch / initiative / ring
  markers are the same class as day/night and are covered by the same rule, but only day/night was
  exercised.
* **D18 has never been written by a real curl failure on this build.** The class is pinned pure; the
  emission site reads `mLastTransportOutcomes` before `writeTransLog` consumes it, which I verified by
  reading, not by producing a 900-second wall miss.
* **Concurrency of the gate.** Sibling lanes may have run their own suites against the same
  `~/.Wagic` during this one. The single-threaded result matches base exactly on every counter, so
  nothing I can see was masked, but I did not re-run in isolation to prove it.
* **I did not read the seat translogs beyond the seqs the wave-61 reviews quote**, except for
  `162v126` seq 49 and `126v125` seqs 43-46, which I read in full from the corpus.
