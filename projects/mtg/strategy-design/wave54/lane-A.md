# Wave-54 lane A — the HOLD row's three defects and its wording; the PLAN block's shape bound

Base: `master 42b3bf5da` (worktree `w54-A`, branch `w54-lane-A`). Baseline re-confirmed on this
worktree BEFORE any edit, on the seeded binary, single-threaded (ground truth):
**PARSETEST 2258 / 0 failed**; suite **1219 tests, 0 failed**, **49 AI tests, 0 failed**,
**0 timeouts** — `WAGIC_TESTSUITE_THREADS=1` produced no failures at all, so the brief's
"2 known concurrency-only failures" (`lifeline.txt`, `merrow_reejerey.txt`) did not appear on
this run; there was no third failure, no timeout and no memory kill under the 4 G cap.
Logs: `~/.gatelogs/w54-A-base-parsetest.log`, `~/.gatelogs/w54-A-base-suite.log`.

Files touched: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/GptPlanCaveat.h`.
Nothing else. Both spliced as bytes (python `rb`/`wb`);
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

---

## D2 (HIGH) — all four parts

### (d) the wording — done FIRST because (a) depends on it

`kHoldPriorityRowText` is now:

> `Pass priority, and do not ask me again this turn unless the board changes (any change re-opens this window; you give up no cast)`

Two additive changes, both the docket's: lead with the VERB the pilot already writes (its own
PLAN prose says *"I must pass priority"* while it takes the row — which is precisely what the
(b) collision measures, so "hold" and "pass" are not two acts to this reader), and put the
GUARANTEE on the row, which it never stated. Nothing is withheld, nothing is advised, no window
is removed. The row still renders LAST, opponent's turn only, at both seams.

**Consequence I had to design for, and did:** after the reword, `Pass priority` is a
word-boundary head of BOTH row 0 (`Pass priority (take no action this window)`) and the HOLD
row. Left alone that would have MINTED a fresh instance of the very defect (a) repairs. It is
handled explicitly below and pinned with cases in both directions.

### (a) the HOLD row's short name is reserved, and the conflict never executes a third thing

`isReservedHoldEcho` (exact match on the trimmed, lowercased echo — never a substring) accepts:
the row's full text; any echo that carries the row's disambiguating prefix through its own comma
(`pass priority, and do not ask me again this turn…`); and the short names `hold`,
`hold priority`, plus the pre-D2d literal, which the model may still write.
`holdRowIndexOf(optionTexts)` says whether this menu carries the row at all — so the binding
works at BOTH seams (`chooseOrderedAction`'s priority menu, which has a row 0, and `askModel`'s
casting menu, which has none) and is inert on every menu without the row.

A reserved HOLD echo binds the HOLD row **ahead of every index verdict**, which is the whole
repair: the index-wins branch refuses to remap a coded `0`, so `CHOICE: 0 (Hold priority)`
executed a plain pass both times it was written (`125v146` seq 74, `146v125` seq 372). Where the
index disagrees with the name, the NAMED row is taken and the divergence is STAMPED
(`hold_row_named;echo_index_conflict`) — never silent, never a third row.

The pass/hold ambiguity: a reserved PASS echo that is also a word-boundary head of the hold
row's text (`(pass)`, `(pass priority)`) no longer forces row 0. The coded index breaks the tie,
and it can only break it between those two rows; the shape is stamped `pass_hold_ambiguous`
either way. `(pass)` with no index, or with index 0, is row 0 — the docket's own NEGATIVE.

### (b) the HOLD row is exempt from the pass-verdict conflict test

`planChoiceConflict` at the priority seam gains `&& !(holdRow > 0 && choice == holdRow)`. A pass
verdict CONFIRMS a hold. This narrows the detector for **no other shape** — every other row, and
every other verdict, is untouched; the wave-52 rejection of a general narrowing stands (the R118
reconciliation in `general-strategy.md`). Removes 5 of wave 53's 8 firings, and with them two
degradations-to-plain-pass, three `plan_choice_conflict_exhausted`, and a full extra model call
each.

### (c) the hidden-zone counters are out of the hold key

`holdBoardKeyOf` now drops, in addition to the leading phase line, any line beginning
`Opponent hand size:` (which also carries `| Opponent library: M cards`) or `Your library: `.
Those are the only two lines wave 53 identified, and they are exactly what the OPPONENT'S OWN
DRAW moves — which is why `125v126` seq 128 (hold taken at their Upkeep) was re-asked at seq 130
(their Draw) on a byte-identical menu, and why `hold_windows_skipped` was 0 on 38 of 40 gameends.
A card moving into a hidden zone is not a board change the hold was about, and the opponent's
hand CONTENTS are not in this block at all, so nothing actionable is dropped. **Every other
re-opener is untouched and pinned as such:** life, the stack top-first, either battlefield, a
newly affordable row, the turn ending.

## D12 (MED) — the PLAN block's shape bound and the menu diff

**(a) SHAPE, not bytes.** `planParagraphBound` runs on the extracted plan before the old
1,600-byte trim (kept as a backstop for a single runaway paragraph). Two cuts, in the docket's
order: the first BLANK LINE, and the first line that STARTS A NEW SENTENCE (the previous line
ended in `.` `!` or `?`) and does not open with a connective. So an ordinary wrapped sentence and
a `Then attack.` continuation both survive; a second paragraph of deliberation does not. **This
bounds the pilot's own prose only — no menu, no row, no window.**

**(b) the one-clause diff.** `gptcaveat::planAbsentActionName` (sibling of `planActionsStale`,
same affirmation test: verb present, not negated, not a future clause, action menu only) returns
the FIRST card the served plan commits to acting on that the CURRENT option list does not carry.
The header then reads

> `YOUR PLAN (as you last stated it, 1 window ago on turn 10; "Master of the Feast" is no longer on your menu): …`

`planActionsStale`'s existing whole-plan note is unchanged and still fires on its own condition;
this clause is the finer instrument that fires when the plan keeps ONE offered card and loses
another — which is the `162v152` s11 -> s12 repro, where `planActionsStale` could not fire.

---

## Tests

**PARSETEST: +37 cases, four new sections** (`#W54-A D2a`, `D2c`, `D12a`, `D12b`) plus two added
assertions inside the amended wave-53 D2 literal pin — **2258 -> 2295, 0 failed**. They pin:

* **D2a** the repro verbatim (`CHOICE: 0 (Hold priority)` on a 2-row menu whose last row is the
  HOLD row -> the hold row, `hold_row_named` + `echo_index_conflict`); the docket's NEGATIVE
  (`CHOICE: 0 (pass)` on the same menu still binds row 0); a real cast on a hold-bearing menu
  unchanged; an AGREEING index earns no stamp; the whole row echoed back (nested parentheses and
  all) resolving to its own row; the ECHO SHAPE of the row's disambiguating prefix; both
  directions of the new `(Pass priority)` ambiguity (index 2 -> hold, index 0 -> pass, stamped);
  and two NEGATIVEs — no hold row on the menu means nothing changes and nothing is stamped, and
  a card name merely CONTAINING "hold" (`Stronghold Assassin`) is not the hold row.
* **D2c** the `125v126` seq 128 -> 130 repro as two situation blocks differing only in the
  opponent's hand size and both library counts: same key, hold survives; the counters are absent
  from the key while life and the stack are present; and four NEGATIVEs — a life change, a new
  stack object, a permanent arriving, and a newly affordable row all still re-open the window
  across the draw step.
* **D12a** the paragraph bound: unchanged one-paragraph plan; blank-line cut; new-sentence-on-a-
  new-line cut; and NEGATIVEs for a wrapped sentence, a `Then …` continuation, a lower-case
  connective line, and the empty plan; plus trimming.
* **D12b** the clause literal; the empty case; the WHOLE header line at the repro's own age; the
  repro through `planAbsentActionName`; and four NEGATIVEs — a card the menu offers, a HELD card,
  a `Next turn, …` clause, and a target sub-menu (not a cast menu).

**Suite: no fixture added, and the reason is unchanged from lane N's.** Every behaviour this lane
changes lives behind `AIPlayerGPT`'s `mEndpoint.empty()` guard and behind `askModel` — i.e.
behind a LIVE MODEL ENDPOINT. The suite's AI fixtures run the Baka heuristic with no endpoint, so
a fixture pinning the hold key, the hold echo, the plan bound or the menu diff would be GREEN ON
BASE, which is the opposite of a pin. No non-GPT engine path is touched by this diff (checked
file by file: the only non-`AIPlayerGPT` file is the header-only `GptPlanCaveat.h`, which nothing
but `AIPlayerGPT.cpp` includes). The regression gate is therefore the FULL suite, and the pins
are the 37 PARSETEST cases against the pure helpers the seams call.

## Gate (this worktree, hermetic incremental build, single-threaded suite)

| | base (42b3bf5da seeded binary) | after |
|---|---|---|
| PARSETEST | 2258 passed / **0 failed** | **2295 passed / 0 failed** (+37) |
| suite | 1219 tests / **0 failed** | **1219 tests / 0 failed** |
| AI suite | 49 / **0 failed** | **49 / 0 failed** |
| timeouts | 0 | **0** |
| memory kills under the 4 G cap | 0 | **0** |

Logs: `~/.gatelogs/w54-A-base-{parsetest,suite}.log`,
`~/.gatelogs/w54-A-post-{parsetest,suite}.log`, build `~/.gatelogs/w54-A-build.log`.

## Falsifiable predictions (per docket item)

* **D2a** — `CHOICE: 0 (Hold priority)` (or any reserved hold name) executes the HOLD row or is
  stamped: **N/N** (was 0/2). No record whose parenthetical is a reserved hold echo carries
  `choice: 0` on a menu whose hold row is not row 0. Falsifier: any such record.
* **D2b** — `plan_choice_conflict` records whose executed row IS the hold row: **0** (was 5 of 8).
  The remaining 3 firings are unchanged in kind. Falsifier: a hold take that still trips it.
* **D2c** — `hold_windows_skipped > 0` on every gameend where the row was taken: **N/N** (was 2
  of 6); no `hold re-opened … the board changed` stderr line whose only board delta is a draw.
  Falsifier: a hold taken at an opponent's upkeep that is re-asked at their draw step on an
  otherwise unchanged board.
* **D2d** — HOLD takes in windows carrying the declined-note at N >= 3: **>= 20%** (was
  3/703 = 0.4%); identical-declined runs **< 15%** of decisions (was 26.2%); opponent-turn casts
  **>= 90% of 58**. Falsifier: takes stay under 5% -> the wording is not the lever and the
  remaining barrier is the pilot's, not the render's.
* **D12a** — served PLAN blocks longer than one paragraph: **0**.
* **D12b** — decisions taken on a served plan naming a card absent from that window's option list
  with no contradiction clause: **0** (was 2).

## What I did NOT verify

* **No live-model probe was run.** Nothing here was exercised against `qwen36-35b-a3b`. Every
  claim about what the model DOES with the reworded row (whether the D2d verb change moves the
  2.5% take rate at all) is an untested prediction; the wave-54 corpus is the measurement.
* **The new `pass_hold_ambiguous` class has no field evidence.** It is a shape I created by
  rewording the row, and I chose its tie-break (index decides; index 0 or no index -> the pass
  row) from the doctrine, not from records. If the pilot answers `(Pass priority)` with a WRONG
  index the answer is now that index's row — the same risk the index carries everywhere else,
  but it is new for this echo. Watch `pass_hold_ambiguous` on the next corpus.
* **`hold_row_named` / `pass_hold_ambiguous` were not read back off a real translog** — they are
  written by paths that need an endpoint; only the note strings and the parse results were
  verified.
* **The D2c key change was not observed retiring/surviving in a live game.** It is pinned against
  the pure `holdBoardKeyOf`/`holdStillStands` helpers with the repro's own two blocks, using
  situation lines I composed to match `serializeGameState`'s emitters
  (`opponentZoneCountsLine`, `yourLibraryLine`) — I did not diff a captured prompt.
* **`planParagraphBound`'s connective list is a judgement, not a measurement.** I did not audit
  the corpus's PLAN lines for how often a second paragraph is load-bearing; the bound can only
  shorten the pilot's own prose, never a menu, so the downside is bounded by construction.
* **`planAbsentActionName` shares `shortName`'s head-matching, so a plan naming a card whose head
  collides with an offered row's head will not fire the clause.** Same known limitation as
  `planActionsStale`; not re-litigated here.
* I did not re-derive any wave-53 corpus number; they are quoted from the ledger and
  `general-strategy.md`.
