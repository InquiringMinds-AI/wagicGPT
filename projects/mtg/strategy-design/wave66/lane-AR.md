# Lane AR — H2 (a/b/c), H8, and the two MED items

Base `bdd0ec6e5` (wave-66 lane brief on master). Branch `w66-lane-AR`, worktree
`worktrees/lanes/w66-AR`. Three files: `projects/mtg/src/AIPlayerGPT.cpp`,
`projects/mtg/include/AIPlayerGPT.h`, `projects/mtg/include/GptPlanCaveat.h`. Every code comment is
tagged `#W66-AR (H2a|H2b|H2c|H8|MED)`. `git diff | /usr/bin/grep -c U+FFFD` = **0**.

The corpus records cited below were read FIRST, verbatim, out of
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/` (read-only), and every claim about what
the shipped engine did is a claim about what those records rendered.

---

## What shipped, item by item

### H2a — the protocol asks for the reasoning FIRST and the coded answer LAST

`kReplyProtocol`. #W65-AO made the FIRST coded line binding at every seam; the protocol still told
the model to put it on line 1. Those two compose into the wave-65 hazard: `post_answer_overrun` on
**1598 of 2209 (72.3%)** decisions, i.e. on three windows in four the model kept reasoning past the
line that had already decided the game. The reorder makes the first coded line and the reasoned line
the SAME line, so the rule and the reasoning stop fighting:

- `HOW TO REPLY` now reads **REASONING -> ANSWER LINE -> PLAN**, "and nothing after the PLAN line".
- The reasoning block is explicitly not read as an answer and not kept, and the model is told to
  write the answer LABEL **exactly once**, on the answer line and never inside the reasoning —
  the one new hazard the reorder creates (a coded line inside the reasoning would now be FIRST).
- First-wins is unchanged and still stated ("If more than one answer line is written, the FIRST one
  is the one that runs"), so a legacy answer-first reply parses byte for byte as it did. The parser
  was not touched for order: `gptSelectAnswerIndex` is order-agnostic and both orders resolve to a
  single coded line when the model complies.
- Eight per-decision tails that said **"On the FIRST line write CHOICE:/ATTACK:/BLOCKS:/PUT:"** now
  say "On a line of its own write ..." — under the new order that sentence would have been a false
  surface contradicting the system prompt at every window.

### H2b — a retraction announced up to THREE non-blank lines above the corrected answer

`gptcaveat::correctionHeaderCue` (new, in `GptPlanCaveat.h` so ONE definition serves both consumers),
`findAnswerLabelLine`, `collectLabeledLines` (new optional `windowOut`), and both combat drivers.

123v126 **seq 36** wrote `CHOICE: 2 (... x30)`, then `Correction: M=41. Stop = L+C+3 = 26+4+3 = 33.
M (41) is already above stop (33).`, then `CHOICE: 0 (pass)` — the announcement two non-blank lines
up, outside #W65-AO's one-line window, and **30 activations ran**. The window is now the up-to-three
non-blank lines written **since the last coded line** (`recentNonBlank` / `windowOut` are CLEARED at
every matched label line, so "no other coded line intervenes" is structural, not a second test).

At that distance only an **announced header** counts: the cue at the START of the line (markdown
decoration tolerated) closed immediately by `:` `,` `.` `!` or end-of-line — `Correction:`,
`Re-evaluating:`, `Actually,`, `On second thought,`. A substring test over three lines of prose would
arm on any paragraph containing the word ("no correction is needed"). Deliberation vocabulary
(`wait`, `hmm`, `let me re-read`) is excluded, exactly as #W65-AO excluded it. The adjacent case is
still exempt (#W65-AP R3: there the line above IS the answer being corrected).

All three seams read the same window — the CHOICE selector, `gptAttackLineFromReply` and
`gptBlocksLineFromReply` — so this is one rule in one place, not two that happen to agree.

### H2c — the record keeps the coded line the rule refused

`gptLineAtOffset` (new), `consumePlan`'s new `laterIgnoredLine` out-param, both combat drivers'
new `ignoredLine` out-param, `mLastIgnoredAnswerLine`, and `rec["ignored_line"]`.

**21 of the 33** wave-65 `later_answer_ignored` records had their `reply` trimmed past the PLAN
(`[+N bytes ... trimmed from this record]`), and `latched_line` holds the line that RAN — so the
refused line, the only thing the stamp is about, was exactly the byte range the record did not keep.
It is now recorded verbatim, by the selector that refused it, from the FULL reply. Consumed and
cleared in `writeTransLog` like `mLastParseNote`, so it can never leak onto a later record.

### H8 — the worked example is no longer an answer attractor, and the no-op stamp re-asks

`askExemplar` + the sentence around it; a new `noopPlanConflict` gate at BOTH the ask seam and the
priority seam.

All 3 `named_row_reask` windows in the corpus recovered to the exemplified row **verbatim**, and
twice that row was `Cast Tribute to Hunger {right now: they control 0 creatures - at 0 this does
nothing}` (126v125 s34, s47) — the engine's own dead-row verdict printed inside the sentence that
shows the model how to answer. `askExemplar` now builds the example from the first row
`rowSaysNoOp` does **not** flag, returns which row it used, and the surrounding sentence names that
row's number ("written out from row N of this list") instead of claiming "the first option" — the
number and the name in an "e.g." must point at the same row. Every row dead, or an empty menu, keeps
row 1: withholding the example would leave the FORMAT unstated, which is the worse surface.

Second half: `plan_contradicts_noop_row` (#W54-B D14) was a RECORD stamp only, and s34 stamped it and
cast anyway. The same conjunction — the row's own zero-verdict AND the reply's own plan arguing
against that row — now earns the ONE re-ask every other self-contradicting reply gets, at both seams,
with the row's verdict quoted back; the second answer executes as given, whatever it is
(`plan_contradicts_noop_row_reask` -> `_recovered` / `_exhausted` / `_unanswered`).

### MED (engine MED-1) — every number/name conflict routes through `index_name_conflict`

`parseChoice`, both `name_over_index` sites (head branch and trailing-scan branch). #W65-AO G8/6
wired the stamp into the reserved-name branches only, so on an ordinary list the name still won
silently: 130v125 **s97/s98** executed `Mountain #2` / `Mountain #3` off a coded 2 and 3 with
`echo_index_conflict;name_over_index`, no conflict note and no re-ask, while the same disagreement on
a hold/pass row got one. The ANSWER is unchanged (the named row still wins); what changes is that the
disagreement is stamped, so both seams route it to the one re-ask the protocol already promises, and
the named row stands as the answer of last resort on the exhausted pass.

### MED (deck126) — a carried plan naming a card in no visible zone

`gptcaveat::planNamesLibraryOnlyCard` (new) + `planLibraryOnlyNote`, emitted in `assemblePrompt`'s
carried-plan branch; and `planTruncationNote`'s new `droppedCorrection` clause.

126v125 **s45-s57**: "On my next turn, cast Idyllic Tutor to find Exquisite Blood" was re-served
verbatim for fifteen turns while the hand held one Chromatic Lantern and no Idyllic Tutor was
anywhere the prompt renders. `planActionsStale` could not say so twice over: it is a claim about
THIS MENU, it is silenced by a clause about a later turn, and its vocabulary includes the LIBRARY.
The new note is a different claim, made separately: a card the plan affirmatively acts on is in the
seat's library and in none of the zones the prompt shows (hand, either battlefield, either graveyard,
either exile, the stack). Future clauses count here — that is the shape that goes stale unseen — a
negated clause never does. Restriction-first, a zone fact and not a legality ruling, ending in the
one action it wants ("Re-state your plan if it has gone out of date").

The item's first half — the carry keeps the premise and drops the correction (29 of 183 deck126
windows) — is addressed at the truncation NOTE, not by carrying more: when the dropped span contains
an announced retraction (the same `correctionHeaderCue`, which is why it lives in the header), the
note adds "and what was dropped included a line correcting what you see above". The bound itself is
untouched at the owner's 400 characters; the default argument is false, so every shipped note is
byte-identical.

---

## RED evidence

The same tree was rebuilt with all six mechanisms reverted to their shipped semantics — the wave-65
protocol strings, the one-line correction window at all three seams, no ignored-line reporting,
`askExemplar` pinned to row 1, no `index_name_conflict` beside `name_over_index`, and both MED-2
halves silenced — and the new cases run against it:

- **base semantics: `4726 passed, 16 failed`** — H2a x5 (3 new + the 2 re-pins that reverse with the
  wording, plus `#W65-AO G8`'s block pin), H2b x4 (protocol + the s36 repro + both combat drivers),
  H2c x2 (both combat drivers), H8 x1, MED-1 x1, MED-2 x3.
- **this tree: `4742 passed, 0 failed`** (base 4698; **+44**).

Three shipped cases were RE-PINNED, each with a `#W66-AR` comment saying which expectation reversed
and why: `W35-plan` (the bound line is no longer "LINE 1"), `#W62-Z D9` and `#W65-AO G8` (first-wins
is unchanged and still pinned; the answer-first instruction it was worded against is gone). No shipped
case's OUTCOME was weakened — each re-pin still asserts the same claim about the same rule.

No suite fixture ships: nothing here is an engine-behaviour change (the GPT seams have no endpoint
under the suite), so the instrument is PARSETEST, and the H2b/H2c cases call the DRIVER functions the
live seams execute rather than the helpers beside them.

## Gate

Detached unit `w66-AR-gate`, `MemoryMax=4G MemorySwapMax=0`, binary rebuilt clean, suite at
`WAGIC_TESTSUITE_THREADS=1` (ground truth).

| leg | result | base |
|---|---|---|
| build | clean link, 0 errors | — |
| PARSETEST | **4742 passed, 0 failed** | 4698 (+44) |
| suite | **1271 tests (0 failed), 67 AI tests (0 failed)** | 1271 / 67 |
| `==Test Failed !==` | 0 | 0 |
| `==Test timed out` | 0 | 0 |

Nothing was killed under the memory cap.

## Key safety (wave-61/62/63 class)

Nothing added here enters a key. `ignored_line` is a RECORD field only. The exemplar row number and
the two new prompt notes ride the rendered tail; `askKey0` is built from the situation plus `tailStr`
exactly as before, and neither note is derived from a counter, a clock or a running total — the
library-zone note is a function of the zones, and the truncation clause is a function of the plan the
model itself wrote, so two rebuilds of the same window produce the same bytes.

## Predictions for the wave-66 corpus

1. **H2a** — `post_answer_overrun` falls sharply from 1598/2209 (72.3%), and the median overrun on
   the records that still carry it is smaller. Falsifier: the rate holds at or above 65% while the
   protocol hash is the new one. Counter-measurement (the cost this buys): the number of records whose
   FIRST coded line sits inside a reasoning block — measurable as `later_answer_ignored` records whose
   `ignored_line` is the LAST line of the reply and whose executed line is followed by prose. If that
   class appears at all, the "write the label once" instruction is not carrying, and the exemplar in
   each per-decision tail is the next dial.
2. **H2a/2** — `coded_answers` is 1 on a higher fraction of records than wave 65's, and
   `later_answer_ignored` falls from 33/2209. Falsifier: `later_answer_ignored` rises.
3. **H2b** — no record executes a coded line that a header-form announcement within the three
   non-blank lines above a LATER coded line retracted. Falsifier: a reply containing
   `Correction:` / `Re-evaluating:` / `Actually,` / `On second thought,` at the start of one of the
   three non-blank lines before its last coded line, with `later_answer_ignored` on the record.
   Positive signal: `answer_replaced: true` on records whose marker is prose-separated by two or
   three lines (wave 65 had 2 `answer_replaced` in 2209, both adjacent-ish).
4. **H2c** — every `later_answer_ignored` record carries an `ignored_line` field, and it is never
   equal to `latched_line`. Falsifier: a record with the note and no field, or with the two equal.
5. **H8** — no `named_row_reask` (or any ask) prompt renders a worked example naming a row that the
   same prompt's `{right now:}` annotation flags as doing nothing. Falsifier: an `e.g. "CHOICE: N
   (...)"` whose row N carries `at 0 this does nothing` / `does not apply` in the same prompt.
6. **H8/2** — no record carries `plan_contradicts_noop_row` and an executed choice without a
   `plan_contradicts_noop_row_reask` first. Falsifier: the stamp on a record whose `fallback` is
   empty and whose parse note has no `_recovered` / `_exhausted` / `_unanswered` sibling.
   Counter-measurement: the re-ask VOLUME this adds (wave 65 would have fired it once, at 126v125
   s34); if it fires more than ~5 times per corpus the predicate is broader than the finding.
7. **MED-1** — no record executes a row whose number and name disagree without an
   `index_name_conflict` note. Falsifier: `name_over_index` on a record with no conflict note.
   Counter-measurement: the extra re-asks (wave 65 would have added 2). The engine seat should count
   `index_name_conflict` firings on UNIQUE names against deck123's MED complaint about the same
   re-ask — this change makes that class bigger, deliberately, and if the seat judges the churn worse
   than the silence the dial is the re-ask, not the stamp.
8. **MED-2** — no carried plan naming a card that is only in the seat's library is served without the
   library note, and the note never renders for a card visible in any rendered zone. Falsifier:
   either half. Positive signal: the 126-shape (a plan naming the same absent card across >5
   consecutive windows) does not recur, or recurs with the note printed each time.
9. **MED-2b** — every truncation notice whose dropped span opens a line with an announced retraction
   carries the new clause, and no other notice does. Falsifier: either half.

## What I did NOT verify / did NOT do

- **No live model probe and no corpus run.** Every change is verified as a STRING or as a pure
  function over recorded bytes, never as a decision a model made differently. In particular H2a is a
  bet about MODEL BEHAVIOUR — the reorder cannot be tested by any instrument I have; the wave-66
  corpus is its first measurement, and prediction 1's counter-measurement is where it can go wrong.
- **H2a's new hazard is real and unmitigated except by instruction.** With the reasoning BEFORE the
  answer, a coded line written inside the reasoning is now the FIRST one and will run. The protocol
  forbids it in as many words; nothing structural prevents it. If the corpus shows the class, the fix
  is a parser bound (a coded line before a `REASONING`/`PLAN` boundary), not more prose.
- **H2b does NOT reach 130v162 seq 61**, the specimen the ledger leads with. Its `Re-evaluating:`
  stands EIGHT non-blank lines above the later answer; the window is three, by the brief. That case
  is pinned as a LIMIT so the claim is not overstated: s61 is H2a's window, not H2b's.
- **The header cue is a closed list and a shape, not a grammar.** A genuine retraction announced in
  words outside it, or written mid-line, still loses. I did not enumerate what the pool actually
  writes; the wave-65 corpus contained exactly two retractions and both are covered.
- **MED-1's re-ask cost is asserted, not measured.** Wave 65 would have added 2 firings; I did not
  model what a corpus with more target lists does, and the same corpus already flagged
  `index_name_conflict` re-asks on unique names as a MED churn complaint (deck123). The stamp is what
  the brief asked for; if the churn is worse than the silence, the dial is the re-ask's condition.
- **MED-1 is verified on the LABEL-STRIPPED segment**, which is what `consumePlan` hands the seams
  (#W52-J D6's own case is written the same way and says so). On the RAW labelled line the parse
  reaches the trailing-scan salvage path and the note is dropped before the return — that is
  pre-existing wave-52 behaviour on a path the live seams do not use for this, and I did not change
  it or pin it.
- **The `plan_contradicts_noop_row` re-ask is pinned at its CONDITION, not at its routing.** The
  conjunction PARSETEST asserts is the exact expression both seams evaluate, on the verbatim s34
  reply and row; the gate itself needs an endpoint and a game and cannot be reached from the
  self-test. Same limitation as every other re-ask in this file.
- **The H2c CHOICE-seam field is pinned as the composition** (`findAnswerLabelLine`'s refused head
  offset -> `gptLineAtOffset`), not through `consumePlan`, which is a non-static member the self-test
  cannot call. The two combat drivers ARE pinned end to end, and they are the two that went RED.
- **`planNamesLibraryOnlyCard`'s visible set is the zones I enumerate**, not the zones the prompt
  actually prints. I chose a SUPERSET of what is rendered (both graveyards and exiles, the stack),
  so the note can only be more conservative than the render, never less — but I did not diff it
  against `serializeGameStateImpl` line by line. A card in the OPPONENT's hand is not checked and
  cannot be; the note claims nothing about it.
- **The library note's cost per window is unmeasured.** It scans the library once per carried-plan
  window (deck libraries are ~30-60 cards); I did not profile it, and it is not cached.
- **Three synthetic PARSETEST boilerplates still carry the old "On the FIRST line write ..." shape**
  (the `#W44-7` and `#W45-4` caveat fixtures at ~49120/49670 and the `#W64-AH F2` composed-window
  case). They are fixtures for `planActionsStale` / `declineFactForMenu`, not for the protocol, and
  changing their wording could change the predicate verdicts they exist to pin. Left as they are,
  deliberately.
- **`planTruncationNote`'s new clause now also fires on the SCRATCHPAD cut** (`planScratchpadCut`
  removes a self-argument sentence, and its marker set overlaps mine on "correction" / "actually" /
  "scratch that" / "on second thought"). I judged that in scope — it is the same defect — but it was
  not what the ledger measured, and the notice text changes on more windows than the 29/183 the
  ledger counted.
- **Nothing was checked against the two known concurrency-only failures** (`lifeline`,
  `merrow_reejerey`) or the intermittent `intrepid_adversary_repeated_payment`: the gate ran
  single-threaded and read 0 failures.
- **Nothing outside my items was touched.** No refactors, no style edits, no work on H1/H3/H4/H5/H6/
  H7/H9/H10 or the other lanes' MED items.
