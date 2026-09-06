# Wave-64 lane AJ — F13 + the MED list (the record, the hold, and the small lies)

Base: master `be2e88282`. Branch `w64-lane-AJ`, worktree `worktrees/lanes/w64-AJ`.
Files: `src/AIPlayerGPT.cpp`, `include/GptPlanCaveat.h`. Both byte-spliced with python `rb`/`wb`;
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. Every wagic run under
`systemd-run --user --scope -p MemoryMax=4G -p MemorySwapMax=0`; the gate ran detached as
`w64-AJ-gate` with a foreground `until [ -f ~/.gatelogs/W64_AJ_DONE ]` wait. No kill under the cap
on any run. No new sources, so qmake was not re-run.

Every item here is parse, render, record or the AI seat's own hold latch. **No game rule and no
engine behaviour reachable from the test suite changed**, so no item ships a suite fixture: the
suite has no network seat (`askModel` returns at `mEndpoint.empty()` before any of this code) and
no fixture can make a model reply happen. PARSETEST is the instrument, and the RED evidence below
is measured by reverting each fix in place on the finished tree and rebuilding — the counterfactual
method lanes Y, AA and AD used, not an assertion.

---

## F13 — the latch record was unauditable because the verdict was computed by a text search

The engine seat's HIGH-1 read: `130v126@1788653538` seq 29 carries `latched_line_in_plan: true` on
a column-0 `ATTACK: A4` that follows a blank line, with no `plan_answer_line_only`, and
`plan_answer_line_ignored` was written 0 times corpus-wide. Two separate defects sat under that
one record, and only one of them was the bound.

**(a) THE VERDICT WAS A FIRST-MATCH SEARCH.** `codedAnswerLineInPlanBlock` located the latched
line with `text.find(line)` — the FIRST occurrence of that string anywhere in the reply. The real
seq-29 reply (read from the corpus, not reconstructed) is a top-level `ATTACK: A2, A3, A4`, then a
single ~3 KB `PLAN:` line whose deliberation says *"So ATTACK: A4 is the most efficient"*, then a
blank line, then the top-level `ATTACK: A4` the engine latched. The plan block ends at the blank
line and always did; the finder matched the quote INSIDE the plan line and reported the wrong side
of the bound. The scan already knows where the line starts, so nothing has to search for it:
`codedAnswerLineAt` gains an offset out-param and new `codedAnswerLinePlanSpan` returns the
verdict plus all three offsets.

**(b) THE RECORD NOW CARRIES THE ARITHMETIC.** `recordLatchedLineSpan` writes
`latched_line_offset`, `plan_block_start` and `plan_block_end` beside `latched_line` /
`latched_line_in_plan`. All three are offsets into the reply **as stored** (a `</think>` prefix's
length is added back rather than reported in a second coordinate space), so a reader slices the
record's own `reply` and checks the verdict instead of trusting it. The two block offsets are
present only when the reply has a line-leading `PLAN:` marker; their absence is what
`latched_line_in_plan: false` then means. This is the engine seat's HIGH-2 ask ("a byte offset ...
would close it and cost nothing near the trim budget"), and it costs three integers.

**(c) THE BOUND IS NARROWED TO COLUMN 0.** `planParagraphBound` / `planBlockEndOffset` had exactly
two terminators — a blank line, and a sentence boundary whose next line opens no connective. A
plan whose last line does not close a sentence, followed at column 0 by `ATTACK: A4`, hits
neither, so that answer was INSIDE the block and #W63-AD's exclusion demoted it silently. New pure
`lineIsCodedAnswerLine` adds one rule: **a coded answer line is not plan prose.** It is
deliberately COLUMN-0 (markdown `*` / `#` furniture skipped, matching `findAnswerLabelLine`'s own
tolerance; INDENTATION is not) — the protocol's rule is that *an answer written inside your PLAN
sentence* is plan, and an indented or wrapped coded line is inside the deliberation. That
distinction is what keeps #W63-AD's exclusion alive rather than retiring it: the demotion still
governs the indented shape, and `plan_answer_line_ignored`'s condition (the bounded and unbounded
walks selecting different lines) is pinned on it in PARSETEST. The rule only ever moves lines OUT
of the block, so no reply that parsed stops parsing.

**Why `plan_answer_line_ignored` fired 0 times, stated rather than fixed blind:** the note is
already written at both seams and its condition is correct. It fired 0 times because no wave-63
reply had a coded line demoted out of the block — the seq-29 record is a FALSE `latched_line_in_plan`,
not a demotion. Nothing was added to make the counter fire; (a) makes the field it sits beside true,
and (c) changes which replies can reach it.

**Two wave-63 PARSETEST fixtures were re-indented, none deleted.** The `#W63-AD E6b` block's `r41`
reply and its fail-safe reply both wrote the in-plan coded line at column 0, which (c) now classes
as a second top-level answer; both fixtures are indented by three and two spaces and every
assertion under them is unchanged, byte for byte.

## MED items

* **E9's relief clause in `chosen_text` (engine seat MED-2, 9 records).** A blanket strip of that
  field is the WRONG repair and I did not ship one: #W57-A D4 makes `chosen_text` the RENDERED row
  byte for byte on purpose, and `stripNarrationDecoration` drops `{taking this row ...}` — which IS
  the HOLD row's benefit tail, the exact tail whose absence made a wave-56 exact-match census read
  208 HOLD takes where the truth was 320. The row is kept whole and the stripped identity is
  written beside it as **`chosen_text_core`**, present only where the two differ. A harvest that
  wants the tag-free name reads that field; nothing is deleted and the D4 census is untouched. The
  GAME LOG was already clean (0 leaks), so no decision was ever harmed by this.
* **E14a `(Wait,` (engine seat LOW-1, 1 of 2,801).** `planScratchpadCut`'s sentence-start walk
  skipped whitespace and quotes; a leading `(` put the marker out of reach of the word list.
  `(`, `[`, `*` and `` ` `` are now skipped as the quote characters are. The marker list itself is
  untouched, so #W63-AF R4's rule holds: *"Wait until their end step"* has no comma, is not a
  marker, and still does not cut — pinned as a MUST-NOT-MATCH with the bracket in front of it.
* **The hold broken by a transient bracket (deck130 MED-4; 125 seqs 72-74).**
  `[this cannot target the spell on the stack - battlefield permanents only]` is appended only
  while an OPPONENT SPELL IS ON THE STACK, so the same window rebuilt after that spell resolves
  printed the same rows minus one bracket and the latch retired against a byte-identical menu.
  `holdKeyRow` now removes that clause and **nothing else besides #W63-AD's phase clause** — every
  price, every other bracket and every row name is still compared byte for byte. The clause states
  what the row CANNOT reach, which is a fact about the stack, not about the row's content: the row
  offers the same objects with or without it. **The row says so** — `holdRowBenefitClause` names
  the second forgiven difference, and the two shipped literals that pin it (#W57-A D4, #W63-AF R2)
  are updated to the new string with #W62-Z's scope sentence kept byte for byte inside it.
  *Shared choke point, checked:* `holdKeyRow` is read by `takeHold` and `holdStillStands` and by
  nothing else; `optionSetKeyLine` (the declined-count / deadlock key) already drops every bracket
  through `stripRenderAnnotationsLc`; nothing that varies between two rebuilds of the same window
  entered any ask key or `mPromptTail` (the wave-61 livelock class), because this change REMOVES a
  varying string from a key rather than adding one.
* **E4b by the letter (engine seat MED-1).** `126v162@1788653525` seq 28 is the corpus's only
  multi-priced tag and it LED with `(1 if all block)` while the largest single price printed in the
  same tag was 4. The certain and "may" halves are still kept apart — a gain the script only MAY
  take is not a number the seat can rely on — but the ceiling is now stated in the same breath:
  `(N certain if all block, and up to N+M if they also take every "may")`. The seq-28 tag now reads
  `(1 certain ... up to 17 ...)`. Silent and single-figure cases are unchanged.
* **"N untapped creatures able to block" unfiltered by evasion (deck162 MED, vs123 seq 16).**
  `canBlock()` is the SOLO gate, so the header counted walls that every per-attacker tag in the
  same prompt had already ruled out with `[no creature they control can block this attacker]`. Two
  true statements, one in the wrong scope, and the header is what the model reads first.
  `attackerBlockerCountLine` takes the count of attackers no untapped creature of theirs may
  legally block — the SAME `rowNoLegalBlock` predicate the guaranteed-damage floor below it is
  built from, so header, floor and rows cannot disagree — and names the scope. The count is not
  replaced and no row is deleted. With no evasive attacker the wave-54 line is byte-identical.
* **`[enters TAPPED]` omits next-turn value (deck125 MED-1, 44/44 declined land drops).** The tag
  priced this turn and stopped, so the only fact on the row was a cost. `- it taps for mana from
  your next turn on` is appended LAST, after the verdict, its evidence and the card's own quoted
  sentence, so every wave-63 substring is byte-identical and only the missing half is added. It is
  a fact, not an instruction, and the RESOLVED-UNTAPPED branch does not carry it.

## Gate

Detached unit `w64-AJ-gate`, `MemoryMax=4G MemorySwapMax=0`, binary rebuilt from clean
(`rm -f bin/wagic && make -f Makefile.sdl -j4`).

| | base (`be2e88282`, the brief's figures) | this lane (measured) |
|---|---|---|
| PARSETEST | 4344 passed, 0 failed | **4387 passed, 0 failed** (+43 cases) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1268, 0 failed / 66 AI, 0 failed | **1268 tests (0 failed), 66 AI tests (0 failed)**; `==Test Failed !==` 0 / `==Test timed out` 0 |
| suite, default threads | 1268, 2 known concurrency-only | **1268 tests (2 failed), 66 AI (0 failed)** — `lifeline.txt` + `merrow_reejerey.txt`, the brief's known pair, both 0 at THREADS=1 |

Logs: `~/.gatelogs/w64-AJ-parsetest.log`, `-suite-t1.log`, `-suite-mt.log` (unit `w64-AJ-gate2`;
the first attempt, `w64-AJ-gate`, sat in the menu forever because the worktree's `bin/User` was
nearly empty — it looks exactly like a slow suite and is not one).
No test file was added, so the suite counts are base's.

### RED evidence — measured, not asserted

Every fix was reverted **in place on the finished tree** to the shipped wave-63 behaviour
(`codedAnswerLinePlanSpan` locating the line with `text.find`; `lineIsCodedAnswerLine` forced
false; `holdKeyRow`'s stack-clause erase disabled; `blockPriceSumTag`'s wave-63 render;
`attackerBlockerCountLine`'s `evasive` forced to 0; the enters-TAPPED clause behind `if (false)`;
`planScratchpadCut`'s opener list without the four bracket characters), the binary rebuilt, and
PARSETEST read **`4367 passed, 20 failed`**. Restored and rebuilt from clean it reads
`4387 passed, 0 failed`. The 20 by item:

| item | RED cases |
|---|---|
| F13 (a) the offset | 4 — the seq-29 REPRO, the first-match MUST-NOT-MATCH, the three-offset consistency check, the think-block coordinate case |
| F13 (c) the bound | 3 — the column-0 POSITIVE, the block terminator, the carried-plan terminator |
| E14a | 2 — the seq-35 REPRO and the three-opener POSITIVE |
| hold / transient bracket | 2 — the key POSITIVE and the `holdStillStands` re-ask case |
| E4b | 3 — the two updated #W63-AB pins and the seq-28 REPRO (+1 MUST-NOT-MATCH) |
| blocker count scope | 2 — the POSITIVE and the overflow clause |
| enters TAPPED | 4 — the two updated byte-equality pins (#W61-T C7, #W62-W D1) and the two new POSITIVEs |
| E9 / `chosen_text_core` | 0 — record-only; see "What I did NOT verify" |

---

## Predictions (falsifiable, wave-64 corpus)

1. **F13 (a).** 0 records carry `latched_line_in_plan: true` whose `latched_line_offset` is outside
   `[plan_block_start, plan_block_end)`; every record carrying `latched_line` also carries
   `latched_line_offset`, and `reply.substr(latched_line_offset, len(latched_line))` equals
   `latched_line` on every record whose `reply_trimmed_bytes` is absent or 0. Falsifier: any of the
   three crossed. The seq-29 shape specifically (a column-0 second answer after a blank line) reads
   `latched_line_in_plan: false`.
2. **F13 (c).** `plan_answer_line_ignored` and `plan_answer_line_only` between them account for
   every record with `latched_line_in_plan: true`, and `unparsed_reply` does not rise above wave
   63's 1. Weaker outcome prediction: `plan_answer_line_ignored` fires at least once — if it is
   still 0, the corpus is telling us the indented-in-plan shape does not occur and the exclusion
   can be retired next wave rather than kept on faith.
3. **E9.** 0 `chosen_text_core` values contain `{` or `[`; every record carrying one has a
   `chosen_text` that does, and the count of records with `chosen_text_core` is at most the count
   of records with `chosen_text`. `{removes ` in the GAME LOG stays 0.
4. **E14a.** 0 served `YOUR PLAN` blocks contain a bracketed self-correction opener followed by a
   marker (`(Wait,`, `[Actually,`, `*Wait,`) after a completed sentence; the wave-63 count was 1.
   Watch the false-positive direction: a plan cut at `(Wait until ...` would be a regression, and
   the MUST-NOT-MATCH says it should not happen.
5. **Hold / transient bracket.** 0 same-turn, same-seam re-asks whose only row difference is the
   stack bracket (wave 63: the 125 seqs 72-74 sequence). `hold_windows_skipped` does not fall.
   Falsifier: any such pair, or a hold honoured across a row set that differs by anything else.
6. **E4b.** Every multi-priced gang tag with a "may" half prints two numbers, the second not
   smaller than the first, and 0 tags print `plus up to N more`. Falsifier: either.
7. **Blocker count scope.** 0 attackers prompts render a `They have N untapped creatures able to
   block` header whose N exceeds the number of blockers any per-attacker tag in the SAME prompt
   admits, without the scope clause. Concretely: on every prompt where at least one A-line carries
   `[no creature they control can block this attacker]`, the header names how many.
8. **Enters TAPPED.** Every `[enters TAPPED ...]` and `[enters tapped UNLESS ...]` bracket ends
   with the next-turn sentence; 0 `[enters UNTAPPED ...]` brackets carry it. Outcome prediction,
   weaker: the declined-land-drop rate at the deck125 seat falls from 44/44.

---

## What I did NOT verify

* **No live model probe and no corpus run.** Every parse, render, record and latch change here is
  PARSETEST and code-reading only; no prompt was put to Spark.
* **`chosen_text_core` has no pure surface and is source-verified only.** The four PARSETEST cases
  under `#W64-AJ E9` pin `stripNarrationDecoration`'s behaviour on the leaking row and on the HOLD
  row — including the MUST-NOT-MATCH that shows why a blanket strip was refused — but
  `stripNarrationDecoration` itself is unchanged, so those cases are GREEN on base and are pins,
  not RED evidence. What is unverified is the record write: I did not run a game and read a record
  carrying the field. The condition is one `!=` in `writeTransLog`.
* **I re-scoped the E9 item rather than doing what the ledger's "Ask" said.** The brief's ask was
  "the chosen-text strip must drop the clause"; dropping it deletes the HOLD row's benefit tail
  from the field #W57-A D4 exists to keep byte-exact, which is a first-order regression of a
  measured wave-56 defect. I state the refusal here rather than burying it.
* **F13's narrowing changes which replies the E6b exclusion governs, and I re-pinned two wave-63
  fixtures to say so.** If the corpus shows `plan_answer_line_ignored` still at 0, the exclusion
  has no live population and next wave should decide whether it earns its place — I did not decide
  that here.
* **The offsets are into the STORED reply, and a trimmed reply is shorter than the reply the
  offsets were computed from.** `recordReplyTrimmed` runs after these fields are computed, so on a
  record with `reply_trimmed_bytes > 0` the offsets index a string the record no longer holds in
  full. That is the same limitation `latched_line` was added to work around and I did not widen the
  trim budget to close it; the offsets are still the only way to know WHERE the line stood, which
  is the engine seat's actual HIGH-2 ask.
* **The blocker-count scope clause is computed from `rowNoLegalBlock`, which is the per-row
  predicate at DECLARE-ATTACKERS.** It is not a claim about what will be blockable after the
  opponent's own responses, and I did not fold in evasion that only PARTIALLY restricts blocking
  (menace, "can't be blocked by creatures with power 2 or less"): those attackers still count as
  blockable in the header and their own rows carry the restriction. The header now names what it
  excludes; it does not claim to name everything.
* **The enters-TAPPED sentence is unconditional on untap.** "It taps for mana from your next turn
  on" is true of every land that survives to that turn and says nothing about a land that is
  destroyed, sacrificed or kept tapped by an effect. I judged the general fact worth more than a
  hedge (the trust doctrine forbids teaching doubt about the render), but it is a general fact.
* **The hold latch's second forgiven clause is an ARGUMENT, not a measurement.** I did not drive a
  window with an opponent spell on the stack, take a hold, let the spell resolve and watch the
  latch survive — no fixture can, and the corpus is the instrument. What is measured is that the
  clause is a bracket printed only under that condition and that `holdKeyRow` is read at exactly
  two sites.
* **I did not measure base myself.** The 4344 / 1268 / 66 figures are the brief's. The RED
  counterfactual above is measured on this tree, which is what the per-item evidence rests on.
* **MED items I did NOT take.** The `; converters` string (30x, lane AC's emitter, declared out of
  scope by that lane), the `{feeds:}` row whose two clauses both end `in your hand: N`, the Kaya
  `-3` self-target row collapse (deck146 MED-5), the "choose a card NAME" token-name and
  public-zone-visibility item (deck146 MED-3), and the Teferi's Puzzle Box mid-clause truncation
  (deck146 MED-4) are untouched. They are render-emitter changes in code this lane did not open,
  and I chose depth on F13 and on the six items above over breadth. Each still stands on the wave-64
  ledger.
* **Concurrency of the gate.** Sibling lanes ran their own suites against the same `~/.Wagic`
  during this one (three `wagic` processes were live during my suite leg). The THREADS=1 result is
  what I report as ground truth; I did not re-run in isolation to prove nothing was masked.
