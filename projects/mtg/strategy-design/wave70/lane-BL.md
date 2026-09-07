# Wave-70 lane BL — every INSTRUCTION the model sees

Scope: audit Table A (all CHANGE + all DELETE rows) plus the golden-file move the brief ordered.
Nothing in Table B (parsers, tolerances, meters) was touched — those are lane BM's, and the two
lanes meet only at the golden file's text.

The ruling this serves (owner, verbatim, via invariant 000): reasoning happens ONLY in the native
reasoning channel; "a plan had to precede the action"; the reply is the PLAN line then the action
line and NOTHING else; "the model CANNOT be told to writing nonsense OR IT FUCKING WILL WRITE
NONSENSE"; the PLAN is a scratchpad for SEQUENCED DECISIONS, not reasoning.

---

## 1. The golden file

`projects/mtg/bin/Res/ai/gpt/reply-protocol.txt` (3,457 bytes, `git add -f`), loaded once by
`gptReplyProtocolText()` and appended to the system prompt exactly where `kReplyProtocol` was
appended before (after the user-editable `system_prompt.txt` template, so a stale override still
cannot drop it). `kReplyProtocol` is now the byte-identical compiled fallback used when the asset
is missing; PARSETEST pins the two together and pins their `gptTextHash` equal, so the record's
`reply_protocol_hash` names ONE edition. `gptReadAsset` still prefers a user override in
`~/.Wagic/ai/gpt/`, which is the pre-existing contract for every prompt asset.

### The golden text, verbatim

```
(the file opens with one blank line — the leading "\n" kReplyProtocol always carried)
HOW TO REPLY (every decision):
Every reply is exactly TWO labelled lines, in this order, and nothing else - nothing before them, nothing between them, nothing after them. Do not put a heading over any part of it.
LINE ONE is your PLAN: line. It is a SEQUENCE of the actions you mean to take, in the order you mean to take them, across this window and the windows to come - for example "PLAN: swing all, Giant Growth after blockers are declared, Rhino in the second main". Write one in every reply: while the sequence you carry still stands, write it out again word for word; write a new sequence when what you intend has changed. It is a private note to your future self, not an instruction to the game and not a second answer, and it is NOT checked against this decision's option list: it may name any card in your deck or hand and any future turn, whether or not that card is among today's choices.
LINE TWO is your action for THIS window: step one of the sequence you just wrote, on a line of its own that BEGINS with the label the decision asks for. Nothing may come before that label on that line - not a heading, not "ANSWER:", not "So" or "Therefore,": the line's first characters are the label itself, and the label appears exactly ONCE in the whole reply. Only the answer line has to come from the list.
Use exactly the label the decision asks for (CHOICE: for numbered choices, ATTACK: for attack declarations, BLOCKS: for block assignments, PUT: for the card-number seams). For CHOICE: write the label, then the NUMBER of your choice FROM THE LIST, then that option's SHORT NAME in parentheses - CHOICE: <number> (<short name>). The SHORT NAME is the action and card name only, i.e. the option text up to the first "{": never copy the {mana cost}, the {right now: ...} note or the {card text: "..."} blob - they are annotations for your reading, not part of the name, and the card text is often cut off mid-sentence. So an option that renders as "3. Cast Example Card {1}{b} {right now: drains 2} {card text: "You draw a card and..."}" is answered in full by "CHOICE: 3 (Cast Example Card)" - nothing more is wanted or matched (Example Card is a placeholder - always copy the real number and short name from the options in front of you, never this example's). On a TARGET menu the short name is the target's name. ATTACK: and BLOCKS: are different: they take the A#/B# LABELS only, with no names and no parentheses ("ATTACK: A1, A3", "BLOCKS: B1:A2, B3:A1").
Write the PLAN as INTENT - the actions you mean to take - not as a description of the board. The line you write is stored and re-served to you VERBATIM at later decisions, unchanged and unchecked, so any board fact you put in it ("both combo pieces are on the battlefield") will still read as true after that has stopped being true. The board you act on is the CURRENT SITUATION block, never your own plan.
Nothing you write is kept except that PLAN line. At your next decision you will see only the game log, the current board, your last PLAN line, and the new choices. So every PLAN line must be complete and self-contained: state the whole sequence as it now stands. Never write a fragment like "continue as before".
Only the FIRST 400 CHARACTERS of your PLAN line are carried to your next decision, cut back to the end of a sentence; anything past that is dropped and you are told how much. Put the steps that matter earliest, and keep the sequence short enough to survive the cut.
```

What survived from the old protocol, byte for byte (each is still pinned by its original PARSETEST
case): the label-purity sentence, the whole CHOICE:/short-name format paragraph with its fake
"Cast Example Card" example, the combat A#/B# exemption, "Write the PLAN as INTENT", the verbatim
re-serve warning, "The board you act on is the CURRENT SITUATION block, never your own plan.",
"NOT checked against this decision's option list", "Only the answer line has to come from the
list.", and the 400-character carry cap (still pinned to `kPlanCarryMaxChars`).

What is gone: the three-part reply, the three-sentence "working" budget, the "answer comes next"
order, the correction/announcement grammar, the "Put any correction BEFORE your PLAN line"
zone, the first-wins/last-wins anchor sentence, the three-case PLAN-omission test, and
"Keep the reasoning SHORT".

---

## 2. Every string changed — before -> after

### 2a. Protocol clauses (audit P-rows)

| Row | Before (abridged) | After |
|---|---|---|
| P2 | `Reply in three parts, in this order: your working, then one answer line, then your PLAN line - and nothing after the PLAN line. Do not put a heading over any part of it.` | `Every reply is exactly TWO labelled lines, in this order, and nothing else - nothing before them, nothing between them, nothing after them. Do not put a heading over any part of it.` |
| P3 | the whole "Your working comes FIRST: THREE short sentences at most ... Think the decision through here" paragraph | **DELETED**. Its one keepable clause (write the label exactly once) survives as `the label appears exactly ONCE in the whole reply`, moved onto LINE TWO. |
| P4 | `THE ANSWER comes next, on a line of its own that BEGINS with the label...` | `LINE TWO is your action for THIS window: step one of the sequence you just wrote, on a line of its own that BEGINS with the label...` (label-purity sentence unchanged) |
| P5 | label list `CHOICE:`/`ATTACK:`/`BLOCKS:` | same paragraph, `PUT: for the card-number seams` added (the missing fourth label the audit flagged) |
| P6 | the correction/announcement grammar (`CORRECTION:`, `Re-evaluating:`, `Actually,`, the three-line window, "read as thinking-out-loud") | **DELETED**. The keepable half (an answer inside the PLAN is not an answer) is folded into LINE ONE's `not a second answer`. |
| P7 | `Put any correction BEFORE your PLAN line ... the lines between your first answer and your PLAN line` | **DELETED** (there is no region between the two lines). |
| P8 | `THE PLAN comes LAST` + the (a)/(b)/(c) omission test | LINE ONE, first: `Write one in every reply: while the sequence you carry still stands, write it out again word for word; write a new sequence when what you intend has changed.` Also re-specified as a SEQUENCE of actions with the owner's own worked example. |
| P9 | `Write the PLAN as INTENT - what you mean to do next, and why` | `Write the PLAN as INTENT - the actions you mean to take` (the brief forbids "and why") |
| P11 | `Keep the reasoning SHORT and put nothing after the PLAN line...` | folded into P2's `nothing after them`; the sentence is gone. |
| P12 | `...your thinking and your earlier plans will have dropped out of context. ... state your full current plan, or your full revised plan if the situation changed.` | `...and the new choices. So every PLAN line must be complete and self-contained: state the whole sequence as it now stands.` (the "your thinking" clause is gone; the never-write-"continue as before" rule stands) |
| P13 | `...a plan that needs more than 400 characters is deliberation, and deliberation is not carried.` | `Put the steps that matter earliest, and keep the sequence short enough to survive the cut.` (the cap sentence itself is byte-identical) |

### 2b. Per-seam tails (audit T-rows)

`planRequestClause(bool)` is **DELETED** with the conditional PLAN it asked for (both faces:
`; then a PLAN: line only if the reply rules call for one (...)` and `; this decision needs no PLAN
line.`). In its place, one file-scope constant every seam leads with:

```
const char * kPlanFirstLead = "Write your PLAN: line first, then ";
```

| Row | Seam | Before (the reply-form part only) | After |
|---|---|---|---|
| T1 | priority | `Which action do you take? On a line of its own write CHOICE: ...; then a PLAN: line only if the reply rules call for one (no plan shown yet, or part of yours is now done or false). Write nothing else.` | `Which action do you take? Write your PLAN: line first, then on a line of its own CHOICE: ... . Write nothing else.` |
| T2 | ask (cast menus, targets, modes, mulligan, X announce, land drop) | `On a line of its own write CHOICE: ...` + `planRequestClause(...)` + ` Write nothing else.` | `Write your PLAN: line first, then on a line of its own CHOICE: ...` + ` Write nothing else.` (`declineFactForMenu` and `exemplarSentence` unchanged) |
| T9 | attackers | `On a line of its own write ATTACK: ...; ... then a PLAN: line only if the reply rules call for one. Write nothing else.` | `Write your PLAN: line first, then on a line of its own ATTACK: ... . Write nothing else.` (the trailing `;` moved onto the planeswalker clause so the sentence still reads) |
| T10 | blockers | `...\nOn a line of its own write BLOCKS: ...; then a PLAN: line only if...` | `...\nWrite your PLAN: line first, then on a line of its own BLOCKS: ... . Write nothing else.` |
| T11/T12 | reveal (single pick / subset) | `On a line of its own write PUT: ...; then a PLAN: line only if...` | `Write your PLAN: line first, then on a line of its own PUT: ... . Write nothing else.` |
| T14 | bottom (London mulligan) | as T11 | as T11 |
| T15 | discard (cleanup) | as T11 | as T11 |
| T18 | decode-garbage retry (`kAnswerLockPrefix`, hoisted to file scope) | `Reply with ONLY the required coded answer line (CHOICE: / ATTACK: / BLOCKS:) and nothing else - no reasoning, no PLAN.` | `Reply with your PLAN: line and then the required answer line (CHOICE: / ATTACK: / BLOCKS: / PUT:), and nothing else.` |
| T5 | search/move target ask (x2 call sites) | `answer with the chosen card's name.` | `answer with the chosen card's row number.` |
| T6 | mutate host ask | `Pick the ONE creature to mutate onto, and answer with its name.` | `... and answer with its row number.` |
| T7 | target ask | `answer with the chosen TARGET's name (not "<effect>")` | `answer with the chosen TARGET's row number (not the row for "<effect>")` |
| T8 | sacrifice / cost target ask | `answer with the chosen creature's name.` | `answer with the chosen creature's row number.` |

The T5-T8 rewrites are the audit's N-152i finding: those asks prescribed a NAME-form answer while
the protocol's contract is number + short name, and a numberless `CHOICE: Cast ...` scores
`unparsed_reply`. Nothing about the menu, its rows or its ordering changed.

### 2c. Re-ask texts (audit R-rows)

| Row | Before | After |
|---|---|---|
| R1 (priority + ask, 2 sites) | `... Answer again and put the answer FIRST: one line beginning with "CHOICE: " - ... - with at most one short sentence of working before it.` | `... Answer again with exactly two lines: your PLAN: line, then one line beginning with "CHOICE: " - ... . Nothing else.` |
| R2 (`smallSeamTruncationReaskLine`, discard/reveal/bottom) | `... put the answer FIRST: one line beginning with "PUT: " naming ... - with at most one short sentence of working before it.` | `... Answer again with exactly two lines: your PLAN: line, then one line beginning with "PUT: " naming ... from the list above. Nothing else.` |
| R3 (`combatTruncationReaskLine`, attackers + blockers) | `... Answer again and write the ATTACK:/BLOCKS: line FIRST - ... - then stop.` | `... Answer again with exactly two lines: your PLAN: line, then the ATTACK:/BLOCKS: line - ... - then stop.` |
| R4 (priority + ask, 2 sites) | `Your reply has no answer line. Answer again with one line that BEGINS with "CHOICE: " ...` | `Your reply has no answer line. Answer again with your PLAN: line and then one line that BEGINS with "CHOICE: " ...` |
| R10 (priority, repeat rows) | `Answer again with the CHOICE line and a PLAN line stating your stop count ...` | `Answer again with a PLAN line stating your stop count, the count you are at now, and how many you perform this window, and then the CHOICE line; or 0 (pass).` |
| R8/R9 (priority `plan_choice_conflict`) | three wordings quoting the reply's prose back (`but your reply then says you should NOT do it ("...")`) | **DELETED**, with the `planChoiceConflict` term removed from the re-ask trigger. |
| R8 (ask, terminal `else`) | `but your reply then says you should NOT do it / but your reply says this window is a pass` | **DELETED**, `askReversedInProse` removed from the trigger; `else if (noopRowZero)` becomes the terminal `else`, and the fallback-class ternary's dead `plan_choice_conflict` arm goes with it. |
| R14 (attackers) | `Your ATTACK line declares attackers, but your reply says this combat is a pass ("...")` | **DELETED**, with its `proseReversesInCorrectionZone` guard and its `attack_reversed_in_prose_*` parse notes. |
| R15 (blockers) | `Your BLOCKS line assigns blockers, but your reply says you are not blocking this turn ("...")` | **DELETED**, same shape. |

R5-R7, R11-R13, R16-R18 are unchanged (they fire on real protocol violations and quote no prose).

**Coordination note for the merge:** deleting R8/R9/R14/R15 leaves `planSaysPassThisWindow`,
`proseReversesInCorrectionZone`, `proseNegatesTakenRow` and their local flags computed and unused
in this lane's tree (warnings only; `-Werror` is off). Lane BM owns those predicates (Table B
B5.1-B5.5) and deletes them. If BM keeps B5.1 after its census, the residual re-ask must be
written fresh against the PLAN LINE — the wordings deleted here quoted "your reply".

### 2d. Guides — STAGED, not installed

Edited copies in `strategy-design/wave70/guides/`; the live `bin/Res/ai/baka/deckN_strategy.txt`
files are untouched, for the orchestrator to install. All six are <= 20,000 bytes with an
Amendment-332 grep of 0 and no U+FFFD.

| Guide | Bytes (was -> now) | Change |
|---|---|---|
| deck123 | 19,977 -> 19,729 | ANSWER FORMAT: `Reasoning, CHOICE line, PLAN.` -> `PLAN line, then CHOICE line; nothing else.` **DELETED** the whole correction-grammar bullet (`THE FIRST CHOICE LINE RUNS AND PROSE NEVER TAKES IT BACK ... "Wait," and "Correct Plan:" do not count.`). Gate bullet: `EVERY GATE ... IS READ BEFORE THE CHOICE LINE, NEVER AFTER IT ... A refusal you write after the CHOICE line, or in the PLAN, does not run` -> `EVERY GATE ... IS APPLIED BEFORE YOU WRITE ... The row your CHOICE line names is what runs.` Blocking: `at LETHAL your FIRST line is "BLOCKS: ..."` -> `your BLOCKS line is`. PLAN template rewritten from a four-number ledger to a sequence (`PLAN: this window <x<N> or pass>, then <the next action you intend>, then <the one after>; stop <L+C+3>, M is <M> now, their life <L>, their creatures <C>`). |
| deck125 | 19,987 -> 19,970 | `Your plan line, when asked for one, is this sentence and nothing else:` -> `Your plan line is this sequence of actions and nothing else:`, and the sentence itself re-cast as ordered steps (`Hold the counter ... then Staff of Nin ... then an Elixir row ... then ping their face each turn`). |
| deck126 | 19,963 -> 19,971 | `ONE "CHOICE:" LINE PER REPLY, AND THE ENGINE RUNS YOUR LAST ONE. Decide first, write one line, stop.` -> `ONE "CHOICE:" LINE PER REPLY, AND IT IS THE ONE THAT RUNS. Write your PLAN line, then one CHOICE line, stop.` (the old claim was false since #W65-AO made the engine first-wins, and is now moot: one line per reply). |
| deck130 | 19,993 -> 19,991 | Starstorm PLAN template from a blank-filling ledger to a sequence: `PLAN: <Starstorm now / hold it>, then <next action>; THEIRS non-defenders <n>, YOURS names Rorix/Commander <yes/no>`, and the follow-up sentence re-anchored (`THEIRS 0, or YOURS yes, means "Cast nothing right now".`) because "the first blank" moved. |
| deck146 | 19,966 -> 19,954 | CHECK 0: `write that sum and their printed life into your reply` -> `weigh that sum against their printed life`. |
| deck152 | 19,997 -> 19,999 | `FIRST LINE OF EVERY BLOCK ANSWER:` -> `FIRST STEP OF EVERY BLOCK DECISION:`. |

deck162 has no answer-format section and is not staged.

**Render-string stability (Amendment 332):** this lane changes no rendered OPTION row, tag,
annotation or header — only the question tails, the re-ask lines and the protocol. Two printed
strings a guide could key on DID move and both are handled in the staged guides above: the
seam tails now open `Write your PLAN: line first, then on a line of its own <LABEL>:` instead of
`On a line of its own write <LABEL>:`, and the T5-T8 asks now say `row number` instead of `name`.
No guide in the pool quotes either (checked by grep over all seven live guides).

---

## 3. The guard

Two halves, because half the instruction strings are reachable at runtime and half are literals
inside emitters that need a live game.

1. **Runtime (PARSETEST, `#W70-BL E6`):** a registry of all 10 reachable instruction strings —
   `kReplyProtocol`, `kPlanFirstLead`, `kAnswerLockPrefix`, both truncation re-ask builders in
   three/two shapes each, and both faces of `exemplarSentence` — scanned case-insensitively for
   `working`, `reasoning`, `first line`, `correction`, `announce`, `then a plan`, `only if`.
   The case asserts the registry SIZE too, so a new instruction constant has to be added to be
   exempt. It carries a positive control (a sham string the scan must see), per the
   silent-instrument rule.
2. **Link time (`tools/check-reply-instructions.py`, wired as a second `QMAKE_PRE_LINK` in
   `wagic-SDL.pro`):** strips comments from `src/AIPlayerGPT.cpp`, cuts the file at
   `#if defined(WAGIC_GPT_PARSETEST_BUILD)` (the corpus legitimately quotes the deleted wordings as
   MUST-NOT-MATCH), splits into statements on semicolons OUTSIDE string literals, concatenates each
   statement's literals, and fails on any statement whose text carries a reply LABEL
   (`CHOICE:`/`ATTACK:`/`BLOCKS:`/`PUT:`/`PLAN:`) and one of the same seven words. Identifier-shaped
   literals (translog classes, parse notes, json keys) are excused by SHAPE, not by name.

   **RED on base, verified:** run against `git show 52ab71610:.../AIPlayerGPT.cpp` it reports
   **24 instruction strings** (the protocol's `Reply in three parts` clause, `kAnswerLockPrefix`,
   both R1 sites, and all seven seam tails). On this tree it reports 0. A second positive control
   (temporarily re-adding `then a PLAN: line only if you like, with one sentence of working` to
   `kPlanFirstLead`) fires 3 hits and exits 1.

   ⚠ **`Makefile.sdl` must be regenerated for the guard to run**: `qmake6 wagic-SDL.pro
   CONFIG+=debug -o Makefile.sdl` (done in this worktree; the merged master tree and every other
   lane worktree carries a stale generated makefile that will link WITHOUT it). Verified running:
   the link line reads `check-ctor-init.py src && check-reply-instructions.py src` and both print
   OK. The `&&` is deliberate — qmake joins multiple `QMAKE_PRE_LINK` entries with a SPACE, so a
   second entry without its own shell separator silently becomes arguments to the first (seen and
   fixed here).

---

## 4. Gate numbers

Hermetic `rm -f bin/wagic && make -f Makefile.sdl -j4`, binary freshness-checked
(`bin/wagic` mtime 21:06, both gate legs after it). Every run under
`MemoryMax=4G MemorySwapMax=0`; no kill under cap.

| Gate | Baseline | This lane |
|---|---|---|
| PARSETEST | 5420 passed, 0 failed | **5438 passed, 0 failed** |
| Suite (`WAGIC_TESTSUITE_THREADS=1`, `WAGIC_FASTCLOCK=0.1`) | 1279 (0 failed), 67 AI (0 failed) | **1279 (0 failed), 67 AI (0 failed)** |
| `==Test Failed !==` / `==Test timed out` counts | 0 / 0 | **0 / 0** |
| `git diff \| grep -c U+FFFD` | 0 | **0** |
| `check-reply-instructions.py src` | 24 hits (FAILED) | **0 hits (OK)** |

PARSETEST arithmetic: 5420 - 3 (the `#W47-R13` `planRequestClause` cases, retired with the
function) + 2 (their `kPlanFirstLead` replacements) + 19 (the new `#W70-BL` block) = 5438.

### PARSETEST cases RETIRED or INVERTED (every one listed, with what replaced it)

| Case | Why it went | Replacement |
|---|---|---|
| `W35-plan case (a)` / `(b)` / `(c)` | pinned the three-case PLAN-omission test | `#W70-BL E1` — the PLAN line is written in EVERY reply; MUST-NOT-MATCH `OMIT the PLAN line`; the PLAN is a SEQUENCE and the answer is its step one |
| `#W47-R13 the default ask keeps the conditional PLAN request verbatim` | `planRequestClause` deleted | `#W70-BL E2` — `kPlanFirstLead` is exactly `Write your PLAN: line first, then ` |
| `#W47-R13 the suppressed form states that no plan is owed here` | same | folded into the same case |
| `#W47-R13 NEGATIVE the suppressed form neither cites the rules nor bans a plan` | same | `#W70-BL E2` MUST-NOT-MATCH — the lead is neither conditional nor waivable |
| `#W62-Z D9 the reply protocol states the rule the three seams now share` | pinned `Reply in three parts` + first-wins | `#W70-BL E1` — the two-line shape and its exclusivity; MUST-NOT-MATCH on both anchors and on `Reply in three parts` |
| `#W66-AR H2a the protocol asks for reasoning first, ONE labelled answer, plan last` | the order it pinned is inverted | `#W70-BL E1` — one label line per reply, with no clause about what precedes it |
| `#W66-AR H2b the protocol names the announcement form...` | pinned the correction grammar | INVERTED — MUST-NOT-MATCH `Re-evaluating` / `CORRECTION` / `three lines just above` / `hmm` |
| `#W65-AO G8 the recorded protocol block carries the first-wins rule` | same | rewritten — the recorded block carries the two-line shape and no `thinking-out-loud` |
| `#W67-AV I3 NEGATIVE the answer anchor is unchanged` | the anchor sentence is gone from the protocol | INVERTED — neither anchor rule is stated to the model any more (the SELECTOR's behaviour is BM's and is still pinned by its own cases) |
| `#W68-BA J3 POSITIVE the length budget is stated where the model composes the reply` | pinned the three-sentence prose budget | INVERTED — no prose budget is stated, because no prose is asked for |
| `#W68-BA J3 MUST-NOT-MATCH first-wins is untouched` + `... section-naming fix is untouched` | both pinned deleted text | folded into the inverted case above |
| `#W69-BF K2` (3 protocol cases: the correction side, the WHY, the region) | pinned P7's correction zone | INVERTED — no correction zone anywhere; the 400-character carry pin is KEPT unchanged |
| `#W54-D D3 echo shape` | pinned `answer with the chosen card's name.` | updated to `answer with the chosen card's row number.` (still byte-identical between the gain and loss asks) |
| `#W68-BE R4` (2 combat correction cases) | pinned `write the ATTACK: line FIRST` | updated to the two-line wording, plus a new MUST-NOT-MATCH that neither correction says `FIRST` or `working` |
| `#W69-BF K2 POSITIVE it says the window is still open` | pinned `put the answer FIRST` | updated to require `exactly two lines: your PLAN: line` and MUST-NOT-MATCH `put the answer FIRST` |
| `[W45-4]` fixture protocol string | a synthetic tail in the old shape (not shown to the model) | rewritten to the tail the seams now emit |

### New PARSETEST cases (19 CHECKs in 4 blocks, at the corpus end)

`#W70-BL E1` golden-file identity (asset present; file == `kReplyProtocol` byte for byte; equal
hashes; a trailing byte is a difference) — read through `JFileSystem` from Res, NOT through
`gptReadAsset`, so a user override on the dev box cannot make the pin pass falsely.
`#W70-BL E1` golden text (shape stated first and exclusively; the two lines named in order; the
owner's worked SEQUENCE example verbatim; `PUT:` in the label list; and the seven-word scan).
`#W70-BL E6` the registry guard + its positive control + the registry-size pin.
`#W70-BL E4` the re-ask texts verbatim, their MUST-NOT-MATCHes, and the echo shape
(`PLAN: ...\nCHOICE: 1 (Cast Damnation)` — PLAN at offset 0, one coded answer line,
`parseChoice` binding row 1 off the answer segment).

---

## 5. Falsifiable predictions for the wave-70 corpus (thinking ON)

1. `post_answer_overrun` > 0 on fewer than 5% of records: nothing in any instruction now asks for
   text after the action line, and the action line is last.
2. `coded_answers != 1` on fewer than 1% of records, and `answer_replaced` ~0: no instruction
   offers a second answer line or a way to supersede one.
3. Replies whose stripped content is exactly two line-leading labelled lines (`PLAN:` then the
   action label) > 90%. The wave-69 corpus's answer-first compliance was 98.9% under an
   instruction that asked for answer-first, so the shape follows the instruction; what this
   predicts is that the ORDER follows it too.
4. `reply_truncated_reask*` falls at the PUT seams relative to wave 69: the reply the cap must fit
   is now two lines, not a paragraph plus two lines.
5. Zero records stamped with any `*_reversed_in_prose_*` note, for the honest reason that the
   stamps no longer exist — NOT as evidence that reversals stopped. Lane BM owns the census that
   can say anything about that.

## 6. What I did NOT verify

- **No live model call.** No probe corpus, no game, no `reasoning_content` observed. Every claim
  about what the model WRITES under this protocol is a prediction; the wave-70 corpus is the gate.
  This is the weakest evidence in the lane: wave 66 shipped a protocol change whose composition
  cost (the model transcribing section names as labels) no PARSETEST case could see, and the same
  class of failure is possible here — e.g. the model writing the literal words `LINE ONE` /
  `LINE TWO` as labels. I chose `LINE ONE`/`LINE TWO` over named sections precisely because
  #W67-AV showed named sections (`REASONING:`, `ANSWER:`) get transcribed, and `LINE ONE` is not a
  plausible line label; that reasoning is untested.
- **The composed prompt was not read end to end.** I changed the protocol block and seven tails
  but never rendered a full system prompt + user turn together, so I have not seen the tails and
  the protocol side by side as the model does. The prompt SHAPE changed (the protocol block is
  ~1,300 bytes shorter and the tails' reply-form clause moved from the end to the front) — per
  invariant 0 that is an ALERT-worthy shape change for the owner, and I am flagging it rather than
  waiting on him.
- **The per-seam tails are guarded at link time over the SOURCE, not at runtime over the emitted
  string.** The emitters need a live game, so no PARSETEST case renders an actual priority,
  attackers, blockers, reveal, bottom or discard tail. If a tail's reply-form wording is ever built
  from a runtime value rather than a literal, the link-time guard will not see it.
- **The guard's link-time half depends on a regenerated `Makefile.sdl`** (see §3). On any tree
  that links from a stale makefile, only the PARSETEST half runs.
- **`planMissing` (R10) may now be unreachable.** It fires when a repeat-row answer carries no PLAN
  line; with the PLAN line asked for in every reply it should approach zero. I reordered its
  wording rather than deleting it because "should be inert" is not evidence — the corpus decides.
- **Guides are staged, never installed, and never play-tested.** I did not run a game with them.
  The deck130 template lost `<the one after>` to the 20,000-byte ceiling; the deck125 sequence
  re-casts standing rules as ordered steps, which is a reading of the deck's engine I did not
  verify against a game.
- **Nothing in Table B was measured.** Whether the surviving parsers read the two-line shape
  correctly at every seam is lane BM's finding, not mine.
