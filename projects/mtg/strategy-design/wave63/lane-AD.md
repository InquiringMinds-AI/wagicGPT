# Wave-63 lane AD — E6, E7, E10, E11, E14 (the record, the latch, the hold, the decline row)

Base: master `41fbad879`. Branch `w63-lane-AD`, worktree `worktrees/lanes/w63-AD`.
Files: `src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`, `include/GptPlanCaveat.h`. All three
byte-spliced with python `rb`/`wb`; `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.
Every wagic run under `systemd-run --user --scope -p MemoryMax=4G -p MemorySwapMax=0`; the gate
ran detached as `w63-AD-gate` with a foreground `until [ -f ~/.gatelogs/W63_AD_DONE ]` wait. No
kill under the cap on any run. No new sources, so qmake was not re-run.

Every item here is parse, render, record or the AI seat's own hold latch. **No game rule and no
engine behaviour reachable from the test suite changed**, so no item ships a suite fixture: the
suite has no network seat (`askModel` returns at `mEndpoint.empty()` before any of this code) and
no fixture can make a model reply happen. PARSETEST is the instrument, and the RED evidence below
is measured by reverting each fix in place on the finished tree and rebuilding — the counterfactual
method lanes Y and AA used, not an assertion.

---

## The finding that reorganised two of the items

Two of the five turned out to be one shape: **an engine that documents a rule and then does not
implement it at the seam that decides.**

* **E6(b).** `kReplyProtocol` has told the model since #W62-Z that *"an answer written inside your
  PLAN sentence is part of the plan, not your answer, and is NOT read as one"*, and D10 put that
  bound on `choiceRetractedNoReplacement`. But the retraction gate is **not** what latched the
  wave-62 overrides. `choiceRetractedNoReplacement` computes a valid region only when the PLAN
  marker follows the last well-formed CHOICE line, and returns **false outright** otherwise — which
  is exactly the shape of every one of these replies (answer, PLAN, then a coded line in the tail).
  The seam that actually selects the answer is `findAnswerLabelLine`, whose walk is last-wins over
  the **entire reply** and had no bound at all. The protocol was true about the engine's intent and
  false about the engine.
* **E10.** `takeHold`'s own comment says *"a hold at a DIFFERENT seam still keeps its own key and
  its own set"*, and `holdHonoured` then called `mHoldRows.clear()` — every seam — whenever any one
  seam's rows moved. A priority menu reprices itself on every life change, so a cast-seam hold was
  destroyed by the next priority window, and the same casting question came back inside the same
  turn with a byte-identical row set. That is the wave-62 engine seat's HIGH-2: 15 leaks, **all at
  the same seam**, against a row that had just promised they would not happen.

---

## What shipped

### E6 — the record, the bound, and the protocol

**(a) the latched line is on the record, whatever the trim cut.** `latched_coded_line` has said
WHICH ordinal the engine latched since #W53-N; `recordReplyTrimmed` (#W61-U C14) then cut the line
itself out of the stored reply, because the latching token lives in exactly the overrun the trim
exists to drop (146v162 seq 41 `reply_trimmed_bytes: 2337`; 146v126 seq 80 `2551` — 2 of 2
`answer_replaced` records at that seat). New pure `codedAnswerLineAt(reply, ordinal)` lifts the
line from the **full** reply, before the trim, and writes it as `latched_line`. Where no ordinal is
computable (a combat seam, a record with no `optionTexts`) and the reply still ended on a different
answer than it began with, the LAST coded line is kept for the same reason. The trim itself is
unchanged: nothing else about the record moved.

**(b) the answer is not read out of the plan block — at the seam that decides.**
`findAnswerLabelLine` gains an optional `[planStart, planEnd)` exclusion, the span
`planBlockEndOffset` defines (the same span `mCurrentPlan` carries), computed in `consumePlan` from
a new pure `firstLineLeadingPlanPos`. A label line that STARTS inside the block is skipped whole —
it neither answers nor anchors a run. **The exclusion only ever DEMOTES:** `consumePlan` runs the
bounded walk first and falls back to the unbounded one when the plan block holds the reply's only
coded line, so no reply that used to parse stops parsing and no window is lost. Which of the two
happened is signed on the record: `plan_answer_line_ignored` (an in-plan line was demoted) or
`plan_answer_line_only` (the fail-safe fired). Applies at both CHOICE seams and, through
`consumePlan`'s shared walk, to `ATTACK:`/`BLOCKS:`/`PUT:` with their own label filter.

The decision the brief asked for, stated: **a coded line inside the plan block may not override.**
Outside the block, last-wins is unchanged — it is what the protocol promises and what the wave-47
recovery (#W48-E1) depends on, and it is pinned MUST-NOT-MATCH here.

**(c) the system message is in the record.** New `recordSystemPrompt()`, called from both
`buildSystemPrompt()` exits, writes one `kind: system` record per seat-game carrying
`reply_protocol` **verbatim**, `reply_protocol_hash`, `system_bytes` and `system_hash` (FNV-1a 64,
16 hex). `ensureGameStartRecord()` is called first so the gamestart record stays first in the file.
Every record that carries a prompt now also carries `system_hash`, so any window can be resolved to
the edition it was answered under. No prompt changed and nothing reads these back.

### E7 — the X menu says why it carries no decline row
The engine is right and the render was silent. 125v146 seq 12's menu armed with the mana already
spent, so `object->checkCantCancel()` is true and there is genuinely no way back to the hand — the
same seat's seq 7 rendered the row because nothing was committed yet. Making the row unconditional
would be a false surface (a Cancel the engine will not honour), so `announceXHeader` takes the
contract's own `canDecline` flag and, when it is false, states the fact the predicate proved: the
costs are ALREADY PAID, the announcement cannot be cancelled, every listed value spends the card,
and X = 0 is the smallest commitment **and not a way out**. `canDecline == true` renders the
wave-23 header byte for byte (default argument), which is 17 of the corpus's 18 asks.

### E10 — the hold latch keeps the promise the row makes
1. **Per-seam retirement.** `holdHonoured` erases only `it` (this seam's entry) and clears
   `mHoldTurn` only when the map is empty. Each seam is still re-checked against its own rows at
   every window, so nothing is held over a screen the model has not answered.
2. **The phase clause is not a row change.** New pure `holdKeyRow` removes
   `" (combat comes next this turn)"` — and **nothing else** — from the latch's identity key; every
   price, bracket and row name is still compared byte for byte, so this is not the blind cache the
   latch must never become. `optionSetKeyLine` has excluded the same clause from the option-set key
   since #W55-A D2a for the same reason. `holdStillStands` normalises both sides, so a set stored
   raw compares the same.
3. **The row says what it forgives.** `holdRowBenefitClause` keeps #W62-Z's scope sentence byte for
   byte and adds *"A pass row that differs only by naming which step comes next is the same row"*.
   Still one `{...}` annotation, still no number in it (the row text is inside both ask keys).

### E11 — `choice` is only read as a row where it IS one
At the ask and priority seams `choice` is the row the model picked; at the subset seams (reveal,
attackers, blockers, discard, bottom) the value handed to `writeTransLog` is the SIZE of the
selection — which is why all 16 reveal records read `choice: 1`. Three row-index-derived stamps were
computed from it anyway (`latched_coded_line`, `latched_row_mismatch`, `plan_contradicts_noop_row`),
which is what made 126v146 seq 66 carry a false `latched_row_mismatch`. New pure
`recordChoiceIsRowIndex(kind)` gates all three; the subset kinds write
`choice_meaning: "selection size"` instead, so the number can never be read as a row again.
`latchedRowMismatch` itself is unchanged — the predicate was right, its inputs were wrong.

### E14 — the carried plan is a plan, and it is not a false board
1. **Bounded to the plan, not to the deliberation.** New pure `gptcaveat::planScratchpadCut` ends
   the carry at the first SENTENCE that opens with a self-correction marker (`wait`, `let me`,
   `let's`, `actually`, `hmm`, `re-read`, ...), and only when a complete sentence precedes it, so a
   plan that merely begins "Actually, cast the Bond" is never cut to nothing and a marker word
   mid-sentence never cuts. It runs INSIDE `planCarryCompose` (the #W60-Q lesson: two correct halves
   in the wrong order delete each other's marker) — the scratchpad cut chooses the text, the length
   bound bounds that text, and exactly one `planTruncationNote` states the drop **against the number
   of characters the model actually wrote**, not against the intermediate cut.
2. **A carried plan that asserts an absent permanent is withdrawn.** New pure
   `gptcaveat::planAssertsAbsentPermanent` is `planDeniesOwnPermanent` with the sign flipped, and
   the more dangerous half: the model plays to a board the prompt invented. Vocabulary is this
   pilot's own cards across **six** zones — library, hand, battlefield, graveyard, **exile and
   stack**, because the repro's two enchantments were EXILED and the four-zone walk every other plan
   caveat uses cannot see them. `inPlayNames` is BOTH battlefields, so a permanent anywhere in play
   contradicts nothing. The claim must be explicit (a window around "on the battlefield" / "on my
   battlefield" / "on the board" / "in play", or after "I control" / "I still control" /
   "I now control"); negated, conditional, future and zone-qualified clauses are excluded, and the
   clause the name sits in is checked before either window so "Once X is on the battlefield" cannot
   fire. On a hit the plan is withdrawn with `planAssertedAbsentBlock`, which names the claim and
   the zone fact — the same shape the denial branch uses.

---

## Gate

Detached unit `w63-AD-gate`, `MemoryMax=4G MemorySwapMax=0`, binary rebuilt from clean
(`rm -f bin/wagic && make -f Makefile.sdl -j4`).

| | base (`41fbad879`, the brief's figures) | this lane (measured) |
|---|---|---|
| PARSETEST | 4111 passed, 0 failed | **4177 passed, 0 failed** (+66 cases) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1254 tests, 0 failed / 66 AI, 0 failed | **1254 tests (0 failed), 66 AI tests (0 failed)** |
| suite, default threads | 1254, 2 known concurrency-only | **1254 tests (2 failed), 66 AI (0 failed)** — `lifeline.txt` + `merrow_reejerey.txt`, both 0 at THREADS=1 |
| `==Test Failed !==` / `==Test timed out` (THREADS=1) | 0 / 0 | **0 / 0** |

Logs: `~/.gatelogs/w63-AD-parsetest.log`, `-suite-t1.log`, `-suite-mt.log`.
No test file was added, so the 1254/66 counts are base's.

### RED evidence — measured, not asserted

Every fix was reverted **in place on the finished tree** to the shipped behaviour
(`codedAnswerLineAt` reading the TRIMMED record text; `findAnswerLabelLine`'s `inPlanBlock` forced
false; the E7 clause behind `if (false && ...)`; `holdKeyRow` returning the row unchanged;
`recordChoiceIsRowIndex` returning true; `planScratchpadCut` returning the plan;
`planAssertsAbsentPermanent` returning false), the binary rebuilt, and PARSETEST read
**`4161 passed, 16 failed`**. Restored and rebuilt from clean it reads `4177 passed, 0 failed`.
The 16 by item:

| item | RED cases |
|---|---|
| E6a | 1 — `the latched line is recovered VERBATIM from the full reply` |
| E6b | 2 — `POSITIVE the committed answer stands`, `the bounded walk finds nothing when the plan block holds the only line` |
| E6c | 0 — a new instrument only; there is no base behaviour to contradict (stated, not claimed around) |
| E7 | 2 — the two clause POSITIVEs |
| E10 | 3 — `a hold taken in main 1 survives main 2's pass row`, the `#W61-V R2` disappearing-row MUST-NOT-MATCH (it reads the normaliser), `the one clause the key forgives` |
| E11 | 2 — the subset-seam MUST-NOT-MATCH and the no-menu NEGATIVE |
| E14 | 6 — two `planScratchpadCut` POSITIVEs, the composed-denominator case, the one-marker echo shape, and the two `planAssertsAbsentPermanent` POSITIVEs |

The per-seam `mHoldRows.erase(it)` half of E10 has **no pure surface** and is source-verified only
(see "What I did NOT verify").

### Cases updated, none deleted
Two shipped literals pin the hold row's benefit tail — `#W61-U C14` and `#W57-A D4`. Both are
updated to the string the row now carries; #W62-Z's scope sentence is kept byte for byte inside it
and the wave-53 literal HEAD (`kHoldPriorityRowText`) is untouched, so #W57-A D4's "strict PREFIX"
assertion still holds and no corpus key is renamed away. Every other shipped case passes unchanged.

---

## Predictions (falsifiable, wave-63 corpus)

1. **E6a** — every record carrying `answer_replaced: true` also carries `latched_line`, and that
   string is a line-leading coded answer line of the model's own reply. Falsifier: an
   `answer_replaced` record with `reply_trimmed_bytes > 0` and no `latched_line`, or a
   `latched_line` that does not appear in the untrimmed reply.
2. **E6b** — 0 records execute a row named ONLY by a coded line inside the plan block. Concretely:
   every record with `latched_line_in_plan: true` also carries `plan_answer_line_only` (the
   fail-safe, i.e. that was the reply's only coded line); a record with
   `latched_line_in_plan: true` and no such note is the falsifier. **The seven wave-62 latches are
   NOT predicted to disappear** — see "What I did NOT verify": whether they fell inside the block is
   still unknown, and `latched_line_in_plan` is the field that will settle it. The weaker outcome
   prediction: `plan_answer_line_ignored` fires at least once, and no window that used to parse
   becomes an `unparsed_reply` (`unparsed_reply` count does not rise above wave 62's 1).
3. **E6c** — every seat translog opens `gamestart`, then `system`, and every record carrying a
   `prompt` carries `system_hash` equal to that record's `system_hash`. Grep
   `The LAST answer line you write is the one that runs` and it appears once per seat file rather
   than 0 times corpus-wide. D9(c) becomes adjudicable; that is the whole claim.
4. **E7** — 0 ANNOUNCE_X asks render `(this ask has no pass row)` without also stating that the
   spell's costs are already paid; and every ask that DOES carry the Decline row renders the
   wave-23 header with no such sentence. Falsifier: either shape crossed.
5. **E10** — 0 same-seam leaks. Concretely: after a hold is taken at a seam, no later window in the
   same turn at that seam re-asks a row set identical under `holdKeyRow` (the wave-62 count was 15).
   Cross-seam re-asks may still occur and are not leaks — the row does not promise them away.
   Second half: no hold is retired between a main-1 and a main-2 window whose only difference is the
   pass row's phase clause (the deck130 seat's 8 pairs). Falsifier: either count above 0.
   Outcome prediction, weaker: `hold_windows_skipped` rises materially against wave 62's 1,191 while
   the hold TAKE rate is unchanged — the latch, not the model, is what moved.
6. **E11** — 0 `reveal`/`attackers`/`blockers`/`discard`/`bottom` records carry `latched_row_mismatch`,
   `latched_coded_line` or `plan_contradicts_noop_row`, and every one carries
   `choice_meaning: "selection size"`. The corpus's `latched_row_mismatch` count on `ask`/`priority`
   records is the only population left, and each of those is checkable against its row.
7. **E14** — (a) 0 `YOUR PLAN` blocks contain a self-correction sentence ("Wait,", "Let me re-read")
   after a completed sentence; every plan that was cut states the drop against the size the model
   wrote, and the marker appears at most once per block. (b) 0 `YOUR PLAN` blocks assert a permanent
   on a battlefield while the same prompt's two battlefield lines carry neither — the deck126
   seq 29 → seq 61 shape (eight turns) does not recur. Falsifier for (b): any served plan naming a
   card under a presence phrase that is in exile, hand, graveyard or library on the same screen.
   Watch also the FALSE-POSITIVE direction: a withdrawal whose named card IS on a battlefield.

---

## What I did NOT verify

* **No live model probe and no corpus run.** Every parse, render, record and latch change here is
  PARSETEST and code-reading only; no prompt was put to Spark.
* **E6b does not resolve the seven wave-62 latches, and I say so plainly.** The deck146 seat wrote
  that the adjudication "needs the engine seat / a `planBlockEndOffset` probe" because the deciding
  line was trimmed out of every one of those records — which is E6a's whole point. I could not
  locate those seven relative to `planParagraphBound` because the data does not exist yet. What is
  measured is the MECHANISM: `findAnswerLabelLine` had no bound, it is the seam that selects the
  answer, and a coded line inside the plan block now loses to one outside it. Whether the real
  replies' latching lines fell inside the block is what `latched_line_in_plan` will say next wave.
  A shape the bound deliberately does NOT catch: a CoT paragraph whose sentences end in periods and
  wrap onto new lines ends the plan block at its second line, so a coded line further down is
  OUTSIDE and still wins by last-wins. That is the protocol's stated rule, not an oversight, but it
  means the bound's reach is narrower than "anything after PLAN:".
* **E6b's fail-safe is a real behaviour change I chose over a stricter one.** A reply whose only
  coded line sits inside the plan block is still answered by it. The strict alternative (refuse, and
  re-ask) would have removed a window, which the doctrine forbids; the cost is that the bound does
  nothing on exactly those replies. `plan_answer_line_only` counts them so the choice is auditable.
* **E6c writes the reply protocol, not the whole system message.** The deck guide, the rules primer
  and the deck lists are represented only by `system_bytes` + `system_hash`. A wave that needs to
  adjudicate a GUIDE wording change will need more than the hash; I judged the protocol block the
  one that three waves of findings are about, and bounded the record deliberately.
* **E7 is a render change on a shape I did not build.** I did not construct a board where an X
  announcement arms with costs committed and read the header off a running game; the clause is
  pinned pure and the flag it reads is the contract's own (`req.canDecline`, which lane AA's
  `assertxdecline` fixture already exercises on the cast route). I did NOT change which menus carry
  the row — the engine's `checkCantCancel` verdict is untouched, and the D5 repro shape (a burned
  card with no way out) is still reachable exactly as often as before.
* **E10's per-seam erase has no test.** `mHoldRows` is a member and the latch has no pure surface for
  the clear; the change is one line and the mechanism is read off the source (`holdHonoured` cleared
  the whole map while `takeHold`'s comment promised per-seam keys). I did not drive two seams in one
  turn and watch a cast hold survive a priority re-open — no fixture can, and the corpus is the
  instrument. I also did NOT widen or narrow the latch's re-check: set equality over the rendered
  rows, in both directions, is #W61-V R2's rule and is unchanged.
* **E10's normaliser is a shared-choke-point change and I checked what else reads it.**
  `holdKeyRow` is consumed only by `takeHold` and `holdStillStands`; `optionSetKeyLine` (the
  declined-count / deadlock key) is untouched, and nothing that varies between two rebuilds of the
  same window entered any key — the hold row's new sentence is a build constant, not a counter, so
  wave-61's `mPromptTail` livelock class cannot be re-armed by it. What I did NOT do is measure
  whether widening the latch by that phrase makes any hold outlive a board change it should not:
  the phase clause is the only string removed, and every priced annotation still retires the hold,
  but that is an argument, not a measurement.
* **E11 changes what the record SAYS, not what the reveal seam decides.** `choice` still receives
  `parseAttackerSet`'s return at those seams; I did not renumber the field or add the picked row
  indices, because `chosen_text` already names the cards taken and renaming the field would break
  every shipped harvest. A reader who wants the row a reveal took still has to map `chosen_text`
  back through `options_text`.
* **E14's markers are a closed word list**, like #W62-AA R2's negation list and with the same
  limitation: a self-correction phrased outside it ("Scratch the above", "That is wrong") is not a
  cut. I also did not measure how often a legitimate plan opens a later sentence with one of these
  words and loses its tail — the sentence-boundary + word-boundary + at-least-one-prior-sentence
  guards are what bound that risk, and the truncation note tells the model what was dropped, but the
  false-positive rate is unmeasured.
* **E14's assertion detector is narrow by construction and I did not sweep the corpus for the
  shapes it misses.** "Both are out", "the pair is assembled", "my Bond resolves the loop" — none of
  those is an explicit presence phrase and none fires. The vocabulary is this pilot's own cards, so a
  false claim about an OPPONENT's permanent is not caught. Nothing here reads the GAME LOG, so
  deck126's MED-1 ("nothing tells the model a permanent has LEFT") is untouched — that is a
  different surface and was not in this lane.
* **The 2 default-thread suite failures are `lifeline.txt` and `merrow_reejerey.txt`** — the brief's
  named known pair — asserted concurrency-only on the strength of the THREADS=1 run reading 0. I did
  not run either solo, and I did not run `intrepid_adversary_repeated_payment.txt` repeatedly (E17
  is lane AE's).
* **I did not measure base myself.** The 4111 / 1254 / 66 figures are the brief's, taken on
  `caa5d6ea3`; my base is `41fbad879`, which adds only wave-63 documents and revised deck guides.
  The RED counterfactual above is measured on this tree, which is what the per-item evidence rests on.
* **Concurrency of the gate.** Sibling lanes may have run their own suites against the same
  `~/.Wagic` during this one. The THREADS=1 result matches base on every counter, so nothing I can
  see was masked, but I did not re-run in isolation to prove it.
