# Wave-74 lane CE — the stop/plan family + cast pricing

Base `c0551632e` (worktree `worktrees/lanes/w74-CE`, branch `w74-lane-CE`).
Items: O7, O7b, O7c, O8, O9, O11, O14, O17, O18, O25.

Gate: `make -f Makefile.sdl -j8` clean, both link guards OK
(`check-ctor-init: OK (118 files)`, `check-reply-instructions: OK (1 source, 37 guides)`).
PARSETEST **5,649 passed / 0 failed** (archived w73 gate binary `wagic-4b90ed7d7-w73step1`:
5,593 / 0 — **+56 cases**). Suite THREADS=1, detached unit `w74-CE-suite`:
**1,283 tests (0 failed), 76 AI tests (0 failed)**; `==Test Failed !==` 0 and
`==Test timed out` 0 — no flakes needed a solo rerun. `git diff` U+FFFD count **0**.

---

## O7 (HIGH) — the `stop=`/`M=` clause is STATE, not a step

**Repro.** `123v130` deck123 seat **seq 141** wrote
`PLAN: stop=29; M=2; this window create 27 vampires with Bloodline Keeper, then attack next turn.`
and **seq 142** was served
`THE PLAN YOU LAST STATED … reads "M=2; this window create 27 vampires with Bloodline Keeper, then attack next turn."`
`123v125` **seq 251** wrote `PLAN: stop=46, M=1; this window tap Bloodline Keeper x45, …` and
**seq 252** carried `"this window tap Bloodline Keeper x45, …"` — both numbers gone.
50 carries in that seat lost their counts; the stated stop then oscillated
56 → 46 → 51 → 106 → 51 inside turn 52 with no screen memory to correct it.

**RED on base.** `planStepEnds` (GptPlanCaveat.h) opens a step at any `; `, so the counts
clause is step one and `planStepsAfter` drops it with the executed action. Compiled the
BASE header (`git show HEAD:…/GptPlanCaveat.h`) standalone against the two quoted plans:

```
A steps=4 carry1=[M=2; this window create 27 vampires with Bloodline Keeper, then attack next turn.]
B steps=3 carry1=[this window tap Bloodline Keeper x45, then attack next turn.]
RESULT A=RED B=RED     (exit 1)
```

Byte-for-byte the two carries the review quotes.

**Fix (engine-side; the guide's `stop=N; M=N` template is untouched).** New
`planStatePrefixEnd` reads the LEADING run of `label=number` clauses (`stop`, `m`, and the
`L=`/`C=` pair the corpus writes beside them; a label, a filler, a number, at most a short
unit word). `planStepEnds` refuses to open a step at any offset inside that prefix, and
`planStepsAfter` re-prepends the prefix to every carry — so the state rides forward while
the executed step is still consumed. The prefix is returned as an OFFSET, so every join is
the model's own bytes.

**GREEN.**
```
A steps=2 carry1=[stop=29; M=2; then attack next turn.]
B steps=2 carry1=[stop=46, M=1; then attack next turn.]
```
Plus 16 pre-existing `planStepCount`/`planStepsAfter` expectations re-run unchanged
(thousands comma, `3.5`, the Thalia comma rule, the `then` boundary, the clamp).

**Prediction (falsifiable).** Next corpus: on deck123's seat, of the carries whose ORIGINATING
`PLAN` line carried a `stop=`/`M=` pair, ≥95% of the rendered `THE PLAN YOU LAST STATED` lines
contain both numbers (wave 73: 0 of 50 after the first step was consumed). If the intra-turn
stop still oscillates by more than ±5 while every carry shows its counts, the defect is the
pilot's arithmetic, not the carry, and O7 is discharged.

## O7b (MED) — the stop clause on EVERY Create row of the seam

**Repro.** `123v162` seq 16 and `123v125` seq 624: the single `[cost: Tap]` Create row sat
beside an appended repeat row that carried `{right now: M=…, your stated stop=…}` — the base
row was an unpriced bypass past the seat's own stop.

**RED on base.** `repeatRowStopClause` was called at exactly one site: the appended repeat row
(`AIPlayerGPT.cpp`, the repeat loop). No base row could ever carry it.

**Fix.** `carriedStop` (and, for O7c, the stored life and the live life) are computed BEFORE
the rows are rendered; every `renderRows[i]` whose action `makesCreatureToken` gets the same
builder's clause. `shownLines` — the decline key, the option-set key and the translog's pure
row — is untouched, and the clause is a `{...}` group, stripped by `stripRenderAnnotationsLc`
from every key regardless.

**GREEN.** Build + suite green; the clause's own shape and its key-invisibility are pinned in
PARSETEST (`O7c ECHO the whole clause is furniture to every key`).

**Prediction.** Next corpus: the `{right now: M=` clause appears on ≥1 non-repeat Create row
in deck123's seat (wave 73: 0), and single-activation Create takes past a stated stop fall
below the wave-73 rate.

## O7c (MED) — the stop reconciled against the life it was set on

**Repro.** `123v125` **seq 686**: `stop=103` was stated off `Opponent life: 100`; their life ran
100 → 98 → **108** while the stop stood, and the 102-attacker swing the stop had sized left them
at 6 (`ATTACK TOTAL: 102 attackers … puts them at 6. That is NOT lethal`).

**RED on base.** Nothing stored the opponent's life at stop-set time; `repeatRowStopClause` took
two arguments and could not have printed the sentence.

**Fix.** New `mStatedStopOppLife`, written at both sites that persist a stated stop (the priority
seam and `consumePlan`), beside `mStatedStopTurn`. `repeatRowStopClause` gains two optional
arguments and appends `; your stated stop was set when their life was N; it is now L` — only when
the two differ, inside the same single `{…}` group.

**GREEN.** PARSETEST: the seq-686 numbers render; an unchanged total prints nothing; a missing
stored life prints nothing; the two-argument call still reads exactly as before.

**Prediction.** Next corpus: on any repeat row rendered ≥2 turns after its stop was stated, the
`was set when their life was` sentence appears whenever the totals differ; a stop restated after
that sentence tracks the LIVE total within 5.

## O8 (HIGH) — the X cast row prices what the X forecloses

**Repro.** `125v126` deck125 seat **seq 308** (t49, 91–9, 17 untapped sources, Staff of Nin in
hand). The row printed only
`{no {leaves ...} count on this row: what it spends depends on the X you announce at the next window}`;
the PLAN read *"Cast Sphinx's Revelation for X=14 now. In main phase, cast Staff of Nin"*.
X=14 spent all 17; the Staff was discarded at cleanup at seq 317. Same shape at 211/215,
250/253, 294/297, 429/432.

**RED on base.** `xCastRemainderScopeTag(maxX, baseCMC, colouredX)` has no hand argument and no
call site passes one — the `<card> needs N` grammar every other cast row prints was structurally
unreachable on this row.

**Fix.** The caller now finds the CHEAPEST castable non-land hand card (same converted-cost /
sources counting rule as `strandsHandCardTag`, so the two clauses on one menu cannot disagree)
and the tag prints `…, <card> {cost} in your hand needs N: the largest X that still leaves it
payable this turn is X=<maxX − N>`, or `no X on this row leaves it payable this turn, not even
X=0` when it cannot be kept.

**GREEN.** PARSETEST pins the seq-308 arithmetic (maxX 11, Staff needs 6 → X=5), the
cannot-keep branch, and both must-not-match branches (no hand card → the wave-62 text byte for
byte; X=0 → the old sentence, group still closed).

**Prediction.** Next corpus: on every Sphinx's Revelation row with a castable hand card, the
`needs N` clause renders; and of the X values ANNOUNCED at the following window, the share that
exceed the printed keep-X falls below wave 73's 5 of 5.

## O9 (HIGH) — the decline rows price the clock the seat already owns

**Repro.** `125v126` **seq 453**, t63, **131 life vs 2**, a Staff of Nin pinging every turn, both
the `LIFE-TO-DAMAGE CONVERTER` and `LOOP COMPLETE` paragraphs printed and quoted back four times
in the reasoning — Emrakul cast anyway (*"I have no other win condition"*), Tribute chain, dead.
The pass/decline/hold rows said nothing about the seat's own clock.

**RED on base.** No emitter reads the seat's own repeatable damage; the decline row is
`castDeclineRow(...) + passRowCleanupPriceTag(...)` and nothing else, and priority row 0 is a
bare literal.

**Fix.** `w74TapOnlyDamagePerActivation` reads the rate off the SCRIPT: a damage ability whose
whole cost is `{T}` and whose target can reach a face. A mana-gated ping (`{2}{T}:damage:2`) is
deliberately NOT counted — its rate depends on a payment the window has not made, and an
over-counted clock is a false surface in the dangerous direction. `ownClockTagFor` sums the
battlefield and `ownClockTag` divides: `{the clock you already control: your <name> #1-#N deal D
damage a turn between them - at that rate alone the opponent reaches 0 in K more turns, with no
card spent}`. Appended to the casting seam's decline row and to priority row 0. No row is added,
removed, capped or auto-answered.

**GREEN.** PARSETEST: Staff of Nin's real primitive line reads 1; the three must-not-match script
shapes read 0; the division, the singular/plural, the single-copy form and the key-invisibility
are pinned.

**Prediction.** Next corpus: in any game where the seat controls a `{T}:damage:` source and is
ahead on life, the `clock you already control` clause renders on every decline row of that game,
and deck125 takes 0 Emrakul casts under a printed `LOOP COMPLETE` (wave 73: 1, and it lost a won
game). A recurrence WITH the clause printed re-routes A-2 from perception to strategy.

## O11 (MED) — the declined-count menus tag the plain decline row (row NEVER removed)

**Repro.** `125v123` **seq 377** (clause reads `[you declined this exact list 3 times already this
turn …]`, answer `Cast nothing right now`); also `125v146` seqs 44 and 82. 55 of 136 windows (40%)
— waves 70/71/72/73 ≈ 20 / 26 / 24 / **40**%, moving the wrong way.

**RED on base.** The override is a note ABOVE the list only; the decline row itself carries no
consequence text at any N.

**Fix.** `declineRowReaskTag(N)` — `{this same question will be asked again this turn: taking this
row closes this window only, and you have already declined this exact list N times this turn}` —
appended to the casting seam's decline row and to priority row 0. **The row is never removed.**
It is a `{...}` group and it is appended AFTER `listKey` is taken, so the key the window counts
under is the key of the UNTAGGED menu and the count can never reset itself at N=1.

**GREEN.** PARSETEST: the seq-377 text, the singular, the N=0 negative, the brace shape, the
key-invisibility, and a `parseChoice` echo proving the tagged row is still answerable by its own
short name.

**Prediction.** Next corpus: takes of the plain decline row on a menu carrying the clause fall
below 30% (wave 73: 40%). If they do not, the clause is not the lever and the item goes to the
guide lane with the measurement.

## O14 (MED) — one mana bill, one bracket

**Repro.** `152v130` deck152 seat **seq 20**:
`{leaves 0 of your 3 untapped mana sources untapped - casting this taps you out}` immediately
followed by `{paying this taps: Intrepid Adversary, Katilda … they cannot attack this turn}`.
The reasoning quoted the first twice, never the second, and planned the attack the second had
ruled out.

**RED on base.** Two adjacent `{...}` groups; no emitter or composer merges them.

**Fix.** `foldManaBillClauses` — pure over the row text — merges an ADJACENT
`{leaves …}` + `{paying this taps: …}` pair into one group, `…taps you out; paying this taps: …`.
Applied at `opts.push_back` in the cast-row builder (the site the repro rides). Nothing is added
or dropped: both bodies survive byte for byte, joined by `"; "`.

**GREEN.** PARSETEST: the fold, the brace count before/after, the merged text, and three
must-not-match cases (only the count, only the tap list, and the two groups separated by a third).

**Prediction.** Next corpus: 0 rows carry both ` {leaves ` and ` {paying this taps: ` as separate
adjacent groups, and the share of reasoning bodies that quote the count while planning an attack
the tap list forbids drops from wave 73's 1-of-1 repro.

## O17 (MED) — the repeat row's short name carries no placeholder

**Repro.** 4 `parse_note: repeat_count_missing` records, all deck123, all the same shape: the
reply copies the row's own printed short name `Create human with Thraben Doomsayer, repeated N
times, then stop`. The row's bracket refused exactly that string while the answer footer says to
copy the SHORT NAME (`125v123` seqs 620, 628, 639; `126v123` seq 74).

**RED on base.** `repeatRowLine` emits `<name>, repeated N times, then stop [`.

**Fix (row text, not the protocol — invariant 000).** The name is now
`<name>, repeated then stop`; the bracket says what the copied name is MISSING
(`copying this row's name alone, with no x<count> after it, names no count and is refused and
re-asked`) rather than refusing the name the footer asks for. The count grammar is untouched.
`bin/Res/ai/baka/deck123_strategy.txt` line 71 quoted the old spelling and was updated to match
(`check-reply-instructions.py` re-run: OK, 37 guides).

**GREEN.** PARSETEST: the new head, the absence of `repeated N times` anywhere on the row, the
new bracket sentence, and `parseRepeatCount` unchanged. Five pre-existing expectations that
quoted the old name were updated to the new one.

**Prediction.** Next corpus: `repeat_count_missing` from a verbatim short-name copy falls to 0
(wave 73: 4).

## O18 (MED) — a row generator with no cardinality budget

**Repro.** `125v123` deck123 turn 67-68: priority windows of 411 and 409 options at **104,131 B**
and 102,927 B (seqs 658, 664; wave 72's largest prompt was 37,917 B). Read the record directly:
the BASE rows were already collapsed by `joinNumberedRows` into `2-103. Equip with Lightning
Greaves #1 targeting Human #1-#102`; the ~80 KB is the **appended REPEAT rows**, streamed one per
base row straight to the tail, each carrying ~700 B of identical contract bracket (rows 407-410
in the record are byte-identical apart from their digits).

**RED on base.** The repeat loop ends `tail << index << ". " << rline << "\n";` — the block never
reaches `joinNumberedRows` and has no collapse of any kind.

**Fix.** The repeat rows are buffered and printed through a fold: `repeatRowFoldKey` (the row with
every DIGIT erased — the only numerics on these rows are the worked example's index and the target
ordinal) groups consecutive rows, and a run at or above `kBattlefieldCollapseFloor` prints its
FIRST row in full plus one `repeatRunRangeLine` covering the rest, naming the row range, the base
row range it decodes against, and the row whose bracket states the count rule.
**The option SET is untouched**: `shown`, `shownLines` and `renderRows` still hold every row, so
every target is still reachable by index AND by name. Nothing is dropped or capped.

**GREEN.** PARSETEST: two equip-target repeat rows share a fold key, a different action does not,
and the range line names its decode block and states that every row in the range is answerable.

**Prediction.** Next corpus: the largest priority prompt on the deck123 seat falls below 40,000 B
(wave 73: 104,131 B) with the option COUNT unchanged at its wave-73 level (411 stays 411), and 0
answers name a row inside a collapsed range that fails to resolve.

## O25 (MED, instrument only) — `plan_names_stranded_card`

**Repro.** `162v130` deck162 seat **seq 11**: row 1 carried
`{reserve: … VERDICT: taking this row STRANDS Master of the Feast this turn …}`; the seat took the
row and wrote `PLAN: Cast Dictate of Kruphix, then play a land and cast Master of the Feast in main
phase 1`. Master of the Feast was never cast in that game.

**RED on base.** No counter, no record field; the shape was invisible to every harvest.

**Fix.** `reserveStrandedCardName` reads the card out of the taken row's own verdict;
`planNamesStrandedCard` asks whether the plan names it. In `writeTransLog`, where `chosenText` and
`mCurrentPlan` are both in hand, a hit writes `rec["plan_names_stranded_card"] = <card>` and
increments the new gameend counter `plan_names_stranded_card`. **No behaviour change** — nothing
is refused, re-asked or withheld.

**GREEN.** PARSETEST: the seq-11 row and plan, a plan that does not name it, a row with no verdict,
and the four-character name floor.

**Prediction.** Next corpus: the counter is non-zero on deck162's seat (the wave-73 shape recurs and
is now measurable). A corpus-wide 0 means the verdict is holding and the item closes.

---

## Weakest evidence — what I did NOT test

- **No live game, no corpus.** Every GREEN here is PARSETEST + the full suite. The suite has no
  fixture for the GPT render seam, so **O7b, O9, O11 and O18 are proven only as pure builders and
  a clean build at their call sites** — that a live priority window actually renders them is
  UNVERIFIED. O18 in particular: I verified the fold logic, not that a 411-row window prints
  correctly end to end.
- **O18's index arithmetic is untested against a real menu.** The range line's base-row numbers
  come from `repeatBaseRow` + 1; if `groupNumberedRows` permuted the base rows before the repeat
  loop ran, the "same order as rows A-B above" sentence could name the wrong block. The permutation
  happens BEFORE the repeat loop, so I believe it is consistent, but no test exercises a permuted
  menu.
- **O7b changes `renderRows` before the collapse runs.** All Create rows on one menu share a
  controller and therefore an identical clause, so the base-row range collapse should be unaffected
  — verified by reading, not by a rendered menu.
- **O8 conflates mana with SOURCES.** `maxAnnounceableX` is mana-denominated; the hand card's
  `need` is converted cost compared against `untappedSources`. That is the same conflation
  `strandsHandCardTag` already makes, so the two clauses agree with each other — but the keep-X can
  be off by one on a board of colour-restricted or multi-mana sources. Untested.
- **O9's clock scan is a SCRIPT scan.** It finds `{t}:damage:<n>` with a face-reaching target and
  nothing else: an upkeep-triggered ping, a `damage:` behind a non-tap extra cost, or a granted
  ability from another permanent are all missed (under-count, the safe direction). It does not check
  that the source is untapped, unsick or legal to activate — the clause states a RATE, not a
  window, and that wording distinction is untested against a pilot.
- **O9 renders unconditionally.** The review scopes the clause to "no lethal incoming"; I did not
  implement that gate (no cheap true predicate at that seam), so the clause can print on a window
  where the seat is about to die. It is still a true statement, but it is not the reviewer's exact
  scope.
- **O14 folds only ADJACENT pairs at the cast-row site.** The activated-ability composer also emits
  `{paying this taps: }` and is NOT folded (it emits no `{leaves }`, so the fold would be a no-op —
  verified by grepping the two `leavesUntappedTag` call sites, not by rendering).
- **O7's state-label set is closed** (`stop`, `m`, `l`, `c`). A pilot that invents a new label
  (`target=`, `goal=`) gets the wave-70 behaviour, i.e. the defect. No corpus scan was run to
  confirm those four are the whole population.
- **O11's key argument rests on reading, not measurement.** I reasoned that appending after
  `listKey` keeps the count stable; no test drives two consecutive windows through the seam.
- **PARSETEST base delta** is measured against `archives/wagic-4b90ed7d7-w73step1`, not a binary
  built from `c0551632e` exactly.
