# Wave-67 lane AV — I1/I2/I3: the reply rule, measured on the corpus before it was designed

Corpus measured: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/` (64 files, 2317 records,
2094 of them carrying a `reply`). Every number below is produced by a read-only python pass over the
`reply` field of every record; nothing here is inferred from the wave-66 review prose.

## 1. MEASUREMENT (done BEFORE any code was written)

### (a) A heading word before the answer label — per seam

A "heading word" is a short line-leading phrase that ends in `:`/`,`/space and is IMMEDIATELY followed
by one of the four answer labels with a payload. Measured with the CLOSED SET the fix ships (below),
so this table is exactly what the shipped predicate sees.

| seam | replies | replies with a heading-word-before-label line | of those, replies the engine found NO answer in (the rescue) |
|---|---|---|---|
| ask | 1440 | 27 | 24 |
| priority | 475 | 20 | 19 |
| reveal | 14 | 3 | 3 |
| attackers | 80 | 2 | 1 |
| blockers | 19 | 2 | 2 |
| discard | 57 | 2 | 2 |
| bottom | 9 | 0 | 0 |
| **total** | **2094** | **56** | **51** |

Heading tokens actually written by the model, corpus-wide (this IS the vocabulary — nothing else occurs):

| count | token |
|---|---|
| 51 | `ANSWER:` |
| 4 | `So ` |
| 1 | `CORRECTION:` |
| 1 | `The answer is ` |
| 1 | `Therefore,` |

Context: 83 records carry `fallback: unparsed_reply`; 81 of them contain zero coded answer lines.
**50 of the 83 carry a heading-word-before-label line** — i.e. the answer was written, in the exact
label syntax, and the scanner walked past it. 46 of those 50 are the single token `ANSWER:`, which is
the protocol's own section name. The reply corpus also shows the sibling transcription: **380 replies
(18.1%) OPEN with a line-leading `REASONING:` heading**, and every one of the 37 replies that contain
no label anywhere begins that way.

Shapes cited by the brief, all confirmed present verbatim: `ANSWER: CHOICE: 3 (Cast nothing right now)`
(162v123 s13), `CORRECTION: CHOICE: 3` (130 s52), `So PUT: 1, 8.` (126 s258), `ANSWER: PUT: 44`
(126v162 s10, the reveal that voided an Idyllic Tutor).

Prefixes that DO occur before a label and MUST NOT be skipped (all measured, all rejected by the
closed set): `PLAN: Turn 10` (2), `REASONING: I have Sorin on the battlefield...` (4), `4.`,
`Also, "`, `However, the Branch B rule says "...`, `But wait, I have Intruder Alarm...`, `Wait, look
at the board. "..."`, `"`.

### (b) A PLAN but no coded line — the salvage rate

92 replies contain zero coded answer lines (ask 47, priority 30, discard 8, reveal 3, blockers 2,
attackers 2); 87 of them carry a line-leading `PLAN:`; 81 became `unparsed_reply`.
After the (a) fix rescues 51 of them, **41 remain** (ask 23, priority 11, discard 6, attackers 1),
36 with a PLAN, 34 previously `unparsed_reply`. 37 contain no answer label anywhere.

Salvage measured two ways on the 37 label-free replies, matching option rows by exact substring of the
row's short name (row text up to the first `{`):

| rule | exact single-row match | ambiguous (>1 row) | no match -> re-ask |
|---|---|---|---|
| the LAST action sentence before `PLAN:` (the brief's rule) | **7** | 2 | 28 |
| any sentence before `PLAN:`, scanning backwards | 22 | 2 | 13 |

The backwards scan is NOT shipped. Hand-reading its extra 15 hits shows it salvages hypotheticals:
125v123 s43's last matching sentence is *"Even if I cast Tribute to Hunger to remove Fate Unraveler..."*
while the reply's conclusion two sentences later is *"Therefore, I cast nothing."* A wrong row executed
is worse than a re-ask, so the shipped rule is the strict one: ~19% salvage, the rest one re-ask.

### (c) Which anchor is the model's real conclusion — first / last / the line before `PLAN:`

38 replies carry more than one coded answer line (ask 21, priority 15, attackers 2; 36 of them exactly
two). 35 have a line-leading `PLAN:`. **Corpus-wide only 1570 of 2094 replies (75.0%) carry a
line-leading `PLAN:` — not 98%**, so a PLAN-relative anchor has no anchor at all in a quarter of
replies, and in 3 of the 38 multi-line replies.

Of the 38: **17 write the SAME row twice** (answer, PLAN, then a restatement) — every anchor agrees.
6 are two Staff-of-Nin activations the model wants BOTH of, written adjacently (the engine already
folds those into one head with a run length, so no anchor sees two candidates). 4 are label-restricted
(a `CHOICE:` and an `ATTACK:` line at a `CHOICE:` seam) and 2 more resolve because the first line is a
rejection line the scanner already skips. That leaves **9 replies where the anchors actually disagree**;
each was judged by reading the prose:

| # | seq | first coded | last coded | line before last `PLAN:` | model's real conclusion (from prose) | engine ran |
|---|---|---|---|---|---|---|
| 1 | 130v146 s24 | `CHOICE: 0 (pass)` | `CHOICE: 1 (Cast Hammer of Bogardan)` | first | **pass** (Hammer is a main-phase intent; this is upkeep) | pass ✓ |
| 2 | 125v146 s34 | `CHOICE: 2 (Cast Cancel)` | `CHOICE: 4 (Hold priority)` | *no PLAN* | **Hold** ("countering a mana rock is a waste") | Hold ✓ |
| 3 | 130v125 s60 | `CHOICE: 3 (Cast Stone Rain)` | `CHOICE: 2 (Cast Talisman)` | last | **Talisman** ("Stone Rain is dead") | Stone Rain ✗ |
| 4 | 123v125 s38 | `CHOICE: 2 (Devour Flesh)` | `CHOICE: 0 (Pass)` | last | **pass** (PLAN says "this window: pass") | Devour Flesh ✗ |
| 5 | 123v126 s96 | `CHOICE: 3 (x200 humans)` | `CHOICE: 0 (pass)` | first | **pass** ("Correction: M (100) > stop") | pass ✓ |
| 6 | 123v126 s114 | `CHOICE: 3 (Cast Damnation)` | `CHOICE: 6 (Cast nothing)` | last | **Cast nothing** ("Correct Plan:") | Damnation ✗ |
| 7 | 126v162 s60 | `CHOICE: 1 (Tribute to Hunger)` | `CHOICE: 3 (Hold priority)` | first | **Hold** ("I will hold") | Hold ✓ |
| 8 | 126v162 s83 | `CHOICE: 1 (Tribute to Hunger)` | `CHOICE: 4 (Cast nothing)` | first | **Cast nothing** ("I will cast nothing, then activate Sorin -2") | Tribute ✗ |
| 9 | 126v162 s84 | `CHOICE: 1 (Tribute to Hunger)` | `CHOICE: 3 (Cast nothing)` | *no PLAN* | **Cast nothing** | Tribute ✗ |

Scores on the 9 decisive replies:

| anchor | correct |
|---|---|
| first coded line, raw | 1 / 9 |
| **first coded line + the shipped correction rule (what the engine does today)** | **4 / 9** |
| the coded line immediately before the last line-leading `PLAN:` (fallback first when absent) | **4 / 9** |
| last coded line, raw | 8 / 9 |

**The brief's candidate anchor does not beat first-wins: 4/9 against 4/9.** It wins #3, #4 and #6 and
loses #2, #5, #7 and #9 (two of them because the reply has no PLAN at all, two because the retraction
is announced BELOW the plan the anchor points at). It is therefore NOT shipped — see §2.3 for what is.

Last-wins scores 8/9 here but is not adoptable from this corpus: this corpus was produced under a
protocol that TELLS the model first-wins, so its later lines are disproportionately real corrections.
Wave 65 measured the other regime directly (a coded line 6 KB down the deliberation executing instead
of the led answer, 3 of 3 flipping the row) and wave-62 R2 pinned the negated-candidate case. The
corpus cannot measure the counterfactual, so the doctrine holds: no last-wins over a negated candidate,
`combatLineIsClean` and the negation test stay.

### (d) The retraction vocabulary

Counted over the up-to-three non-blank lines since the previous coded line, above every LATER coded
line in the 38 multi-line replies (this is exactly the window `findAnswerLabelLine` reads):

| count | text in the window | in the announced-header set today? |
|---|---|---|
| 17 | `So ` | no (deliberation) |
| 14 | `wait` (10 of them line-leading `Wait`) | no — deliberately |
| 13 | `but ` | no |
| 9 | `actually` (7 line-leading `Actually`) | **yes** |
| 3 | `however` | no |
| 2 | `therefore` | no |
| 1 | `re-evaluat` | **yes** |
| 1 | `correction` | **yes** |
| 1 | **`correct plan`** | **NO — and it decided a game** |
| 1 | `instead` | no |

Two findings:

* `Correct Plan:` is absent from `gptcaveat::correctionHeaderCue`'s head list. It is the announcement
  in 123v126 s114 (row 6 of the table above): the retracted `CHOICE: 3 (Cast Damnation)` executed and
  killed the seat's own 102 creatures. One measured token, one line of code.
* `Wait,` is the model's most common line-leading word above a later coded line (10 windows) and it is
  the strongest argument for NOT admitting it. Row 1 of the table above is the counter-example the
  corpus supplies: 130v146 s24 answers `CHOICE: 0 (pass)`, then rambles *"Wait,, Lay Waste targets a
  land..."* and writes `CHOICE: 1 (Cast Hammer of Bogardan)` — a main-phase intent at an UPKEEP window.
  Admitting `Wait,` flips that correct answer to a wrong one. It stays out, now with corpus evidence
  rather than only the wave-65 argument.

### (e) Reply shape, for the wave-67 predictions

| metric | wave-66 corpus |
|---|---|
| coded line on reply line 1 (no reasoning above it) | 1679 / 2094 = **80.2%** |
| replies opening with a `REASONING:` heading | 380 = 18.1% |
| replies with a line-leading `PLAN:` | 1570 = 75.0% |
| `post_answer_overrun` | 1390 non-zero, median 177 B, mean 301 B, p90 409 B, max 12360 B, **total 630,880 B** |
| `post_plan_overrun` | 200 non-zero, mean 105 B, max 11582 B, total 219,885 B |
| `fallback: unparsed_reply` | **83** (3.6% of decisions) |

## 2. WHAT SHIPPED

Files: `projects/mtg/include/GptPlanCaveat.h`, `projects/mtg/src/AIPlayerGPT.cpp`.
All comments tagged `#W67-AV (I1|I2|I3)`.

### 2.1 I1 — one heading predicate, at every seam

`gptcaveat::answerHeadingSkip(text, s, end)` (GptPlanCaveat.h, beside `correctionHeaderCue` so
every consumer shares one definition). After a caller has skipped markdown decoration, it advances
past a line-leading heading phrase **only when a real answer label with a payload follows it**, and
returns the offset the label compare should use. Two closed head tables:

* `kHeadsPhrase` (a space may close it — the phrase NAMES the answer): `the answer line is`,
  `the answer is`, `my answer is`, `my choice is`, `the chosen row is`.
* `kHeadsClosed` (must be closed by `:` or `,`): `the answer line`, `final answer`, `my answer`,
  `the answer`, `answer`, `correction`, `corrections`, `corrected`, `correct plan`,
  `revised answer`, `revised`, `conclusion`, `decision`.

**Deliberately excluded, with the reason in the code:** `PLAN` and `REASONING` (a plan line and a
reasoning line are not answers at any seam); bare connectives `So` / `Therefore` / `Thus` (the corpus
writes them 5 times, but that shape is owned and PARSETEST-pinned by three shipped rules — #W48-E1's
post-plan prose recode, `restatedCombatDirective`'s clean restatement, and its MUST-NOT-MATCH for
`So BLOCKS: whatever keeps me alive longest.`; admitting the connective promotes that prose to a
line-leading answer ahead of the cleanliness tests those rules exist to apply — it made 12 shipped
cases fail, and re-pinning them would have been re-pinning a wrong expectation); bare `Final:`
(#W62-AA R2 pins `Final: BLOCKS: B1:A2.` as a post-answer restatement).
The shipped set matches **51 of the 56** measured corpus lines and nothing else in 2094 replies.

Called at **14 sites** — every place in the engine that skips decoration and then compares an answer
label: `codedAnswerCount`/`scanCodedAnswerLines`, `codedAnswerLineAt`, `findAnswerLabelLine`,
`lineIsCodedAnswerLine`, the five `kLabel = "CHOICE:"` walks (ordinal, retraction anchor, salvage,
`firstLabelledLine`, the plan-prose recode), the two `BLOCKS:` walks, `collectLabeledLines` (the
ATTACK/BLOCKS/PUT drivers and both loop salvages) and `restatedCombatDirective`. One predicate, every
seam — the rule cannot be true at one seam and false at another.

### 2.2 I2 — a complete reply with no label is salvaged or re-asked, never Baka'd

* `replyLabelMissing(reply)` — the trigger: `codedAnswerCount == 0` **and** a line-leading `PLAN:`
  exists (a reply with no PLAN is truncated/spiralling and keeps every path it has today).
* `salvageLabelMissingChoice(reply, optionCount, optionTexts)` — walks sentences backwards from the
  PLAN marker, stops at the **first (i.e. last-written) action sentence**, and matches each row's
  short name (option text up to the first `{` or `[`, and also its head before a ` (` gloss) as a
  case-insensitive substring. Exactly one distinct row -> that row; zero or >1 -> `-1`.
  It does **not** keep scanning past that sentence (see §1(b): the wide scan salvages hypotheticals).
* Wired at the **ask** seam and the **priority** seam (77 of the 92 label-free replies), inside the
  existing one-re-ask-per-board machinery: salvage stamps `label_missing_salvaged` and executes;
  otherwise `labelMissing` joins the re-ask condition with fallback class `label_missing_reask`,
  a notice, and the re-ask line
  `[RE-ASK] Your reply has no answer line. Answer again with one line that BEGINS with "CHOICE: " ...
  and nothing before the label on that line.` (the priority form adds `or 0 (pass)`).
  The second answer executes as given and is stamped
  `label_missing_reask_recovered|_exhausted|_unanswered`, exactly like every other re-ask kind.
  No cap, no window removed, no row withheld — one extra question.

### 2.3 I3 — the anchor is NOT moved; the wording that invited the shape is

**The anchor stays first-clean-wins with announced corrections superseding.** §1(c) is the reason:
the brief's candidate ("the coded line immediately before `PLAN:`") scores 4/9 against the shipped
rule's own 4/9 on the nine replies where the anchors disagree, and 25.0% of replies carry no
line-leading `PLAN:` for it to anchor on. It is not shipped, and a PARSETEST NEGATIVE pins that the
protocol still says first-wins. Last-wins scores 8/9 on this corpus but is unadoptable from it: this
corpus was produced under a protocol that TELLS the model first-wins, so its later lines are
disproportionately real corrections — wave 65 measured the other regime and it was worse, and
wave-62 R2's negated-candidate rule (`combatLineIsClean` + the negation test) is untouched.

What did change, all measured:

1. **`kReplyProtocol` no longer names a section with a word the model writes as a label.**
   `Write your REASONING first, then your ANSWER, then your PLAN` ->
   `Reply in three parts, in this order: your working, then one answer line, then your PLAN line -
   and nothing after the PLAN line. Do not put a heading over any part of it.`
   `REASONING comes FIRST:` -> `Your working comes FIRST:`.
   `THE ANSWER LINE comes next, on a line of its own, using exactly the label...` ->
   `THE ANSWER comes next, on a line of its own that BEGINS with the label the decision asks for.
   Nothing may come before that label on that line - not a heading, not "ANSWER:", not "CORRECTION:",
   not "So" or "Therefore,": the line's first characters are the label itself.`
   `ANSWER LINE` is gone from the protocol entirely (two other mentions lowercased). `PLAN` keeps its
   name because `PLAN:` IS a label the engine reads. The shape asked for is unchanged and is still
   reasoning -> ONE coded line -> PLAN -> stop.
2. **The one engine tail that WROTE the invited shape.** `discardSpareDefaultLine` printed a
   line-leading `DEFAULT ANSWER: discard the spare land.` — the engine teaching the model the exact
   `ANSWER:` prefix. It now reads `BY DEFAULT, discard the spare land.`; the claim is byte-identical
   otherwise and a PARSETEST NEGATIVE pins that no engine tail writes a line-leading `ANSWER:`.
   (Checked: this was the ONLY emitted string of that shape; the other 7 per-decision tails name no
   section and needed no change.)
3. **`correct plan` / `corrected plan` added to `gptcaveat::correctionHeaderCue`.** One measured
   token (§1(d)); it is the announcement in 123v126 s114, where the unheard retraction cast a
   Damnation over the seat's own 102 creatures and decided the game. **`wait` is still NOT a header**,
   and now has corpus evidence for staying out: it is the most frequent word in the window (10
   line-leading), and the one case the corpus lets us adjudicate (130v146 s24) would flip a correct
   `CHOICE: 0 (pass)` to a main-phase Hammer at an upkeep window.

## 3. GATE

Built from `<wt>/projects/mtg`: `rm -f bin/wagic && make -f Makefile.sdl -j4`, binary freshness
confirmed with `ls -la bin/wagic` after every make. Gate run detached under
`systemd-run --user --unit=w67-AV-gate -p MemoryMax=4G -p MemorySwapMax=0`.

| gate | baseline | this branch |
|---|---|---|
| PARSETEST | 4906 passed, **0 failed** | **4940 passed, 0 failed** (+34) |
| suite (`WAGIC_TESTSUITE_THREADS=1`) | 1273 tests, 0 failed | **1273 tests, 0 failed** |
| AI tests | 67, 0 failed | **67, 0 failed** |
| `==Test Failed !==` / `==Test timed out` lines | 0 / 0 | **0 / 0** |
| `git diff \| grep -c U+FFFD` | — | **0** after every edit |

### RED on base

Verified by neutering ONLY the two new mechanisms in `GptPlanCaveat.h` (`answerHeadingSkip` returns
its input unchanged; `correct plan` removed from the header set), rebuilding, and re-running the
suite of new cases: **11 of the new PARSETEST cases go RED**, and every MUST-NOT-MATCH / NEGATIVE
case passes in BOTH states (they pin no new expectation):

```
FAIL #W67-AV I1 162v123 s13 `ANSWER: CHOICE: 3 (Cast nothing right now)` is an answer
FAIL #W67-AV I1 ...and it resolves to row 3 through the ordinary parse
FAIL #W67-AV I1 130 s52 `CORRECTION: CHOICE: 3` is a MARKED correction, and it wins
FAIL #W67-AV I1 126v162 s10 `ANSWER: PUT: 44` is an answer at the reveal seam
FAIL #W67-AV I1 the ATTACK seam skips the heading too
FAIL #W67-AV I1 collectLabeledLines (the combat/PUT drivers) skips the heading
FAIL #W67-AV I1 `The answer is CHOICE: 2 ...` is an answer
FAIL #W67-AV I1 the record's coded-line counters and the latched line agree with the seams
FAIL #W67-AV I1 ECHO the stored line starts at the label, at its true offset, unannotated
FAIL #W67-AV I3 `Correct Plan:` is an announced retraction header
FAIL #W67-AV I3 123v126 s114 the announced correction wins over the retracted Damnation
=== self-test: 4929 passed, 11 failed ===
```
The header was restored from an unmodified copy and rebuilt to 4940/0 before the gate ran.

The wider RED for I1 and I2 is the corpus itself, produced by the base binary 9b797fb81: **83
`unparsed_reply` records, 49 of them carrying a line this predicate now parses, and 34 more that are
complete replies with a PLAN and no label at all** — every one of which reached Baka. I2's helpers did
not exist on base, so their RED is those 34 records rather than a synthetic fixture.

## 4. PREDICTIONS (falsifiable, for the wave-67 corpus)

1. **`unparsed_reply` < 5 per corpus** (wave 66: 83). I1 removes 49 of the 83 mechanically; I2 turns
   34 more into `label_missing_salvaged` or `label_missing_reask` rather than a fallback class; the
   protocol rewrite should suppress most of the remainder at source.
2. **No record carries a line-leading `ANSWER:` or `REASONING:` heading over its answer.** If the
   corpus still shows `ANSWER: CHOICE:` lines the protocol rewrite failed and only the parser saved
   them — the counts still fall, but the diagnosis is different, so count them separately.
3. **`label_missing_salvaged` ≈ 1 in 5 of the label-free replies, and every salvaged row matches the
   reply's own last action sentence.** If `label_missing_salvaged` exceeds `label_missing_reask` the
   matcher has become loose and the hypothetical case (§1(b)) is being salvaged.
4. **Answer-first share falls from 80.2%.** The protocol still asks for working first; if line 1 is
   still the coded line in >70% of replies, the ORDER instruction is not the lever and wave 68 should
   stop spending words on it.
5. **`post_answer_overrun` total falls below 630,880 B** (median below 177 B). The added
   "nothing before the label" sentence costs ~30 tokens per window; if the total does not fall, the
   protocol grew for nothing.
6. **`later_answer_ignored` falls from 48**, and `answer_anchor_override` is NOT recorded — because
   the anchor did not move; the only new supersession path is the `correct plan` header.
7. **No new `plan_answer_line_ignored` / `latched_row_mismatch` growth.** The heading skip moves the
   label, never the line, so the plan-block spans and the latched-line offsets are unchanged.

## 5. WHAT I DID NOT VERIFY

* **No live model probe was run.** Every claim about what the model will WRITE (predictions 2, 4, 5)
  is inference from the wave-66 corpus, not measurement of the new protocol. The corpus-level claims
  (§1) are measurement.
* **`answer_anchor_override` is not implemented**, because the anchor did not move. If a later wave
  adopts a different anchor it will need that record field.
* **The `So PUT: 1, 8.` shape (2 discard records) is NOT parsed** — it is excluded on purpose (§2.1).
  I predict I2's re-ask catches both, but the discard seam is NOT wired to I2 (see below), so on the
  wave-67 corpus those two windows will still reach Baka. That is a known, deliberate gap.
* **I2 is wired at the ask and priority seams only** (77 of 92 label-free replies). The discard (8),
  reveal (3), blockers (2) and attackers (2) seams do not have the one-re-ask-per-board machinery
  those two share, and building it at four more seams was outside this lane's items.
* **The judgment in §1(c) of "the model's real conclusion" is mine, read from the prose of 9 replies.**
  Rows 3, 5, 6 and 7 are unambiguous (the reply states the retraction in words); rows 8 and 9 are
  reads of a 7.8 KB and a 5.4 KB reply and could be argued the other way. If both were reversed the
  PLAN-anchor score would be 4/9 vs first-wins' 6/9 — the decision not to move the anchor gets
  stronger, never weaker, so the conclusion does not turn on those two.
* **The 400-char plan carry, the ramble guard at `:18137`, and `consumePlan`'s 300-byte bound were not
  touched.** I2's trigger reads the reply directly, so the guard that the wave-66 reorder pushed
  `PLAN:` past is bypassed rather than re-tuned; whether that guard is now dead weight is untested.
* **No performance measurement.** `answerHeadingSkip` runs per line at 14 seams; it is O(heads × line)
  with an early exit on the first character, but I did not measure the parse cost.
