# Wave-67 lane AY — I6, I8, I9b + render MEDs

Base: master `ff06cc432` (worktree `worktrees/lanes/w67-AY`, branch `w67-lane-AY`). Two files:
`projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h`. Every code comment is
tagged `#W67-AY (I6|I8|I9b|MED)`. `git diff | /usr/bin/grep -c U+FFFD` = **0**. Every wagic run
under `MemoryMax=4G / MemorySwapMax=0`.

Corpus read read-only: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/`. Every claim below
about what the shipped engine rendered or answered is a claim about bytes in those records.

No live model probe and no corpus run — PARSETEST is the instrument (nothing here is an
engine-behaviour change the suite can drive: all five mechanisms are GPT-seam render/parse code and
the suite has no model seat).

---

## I6 — the repeat row: the model's own stop is performed, and one source states it

**Two halves, both keyed on numbers the MODEL wrote.**

**(a) The stop is executed on the second answer** (`repeatStopClampCount`, new pure; wired at the
priority seam's exhausted branch). Wave 66's `repeat_past_stop` re-ask executed the second answer as
given, and `123v126` s84/s85 is the price: the re-ask quoted *"your own PLAN puts you at 66 with
your stop at 26"* and the reply came back with the **identical** `x34` under the **identical**
contradicting PLAN, and it ran (M 66 -> 100). Same shape at s81/s82. 3 of 20 re-asks on that seat
did this and produced 59 extra bodies, driving M to 102.

Now: when the SECOND answer repeats a counted take past the stop the model itself stated, the engine
performs **the stop the model stated** — `max(0, stop - M)`, the model's own subtraction over the
model's own two numbers — and records `repeat_clamped_to_own_stop(named=..,stated_M=..,stated_stop=..,
executed=..)` with a narration receipt naming all of it. It is **not a cap**: no ceiling of the
engine's invention exists in the predicate, `kRepeatRowMax` is untouched, no row is withheld, a count
INSIDE the stated stop is untouched (`repeatStopClampCount` returns -1), a reply that states a NEW
stop above its count is not clamped at all, and nothing fires until the model has been asked again
and has restated a count past its own number. On every past-stop pair `stop - M <= 0`, so the
performed number is 0 and the window resolves as the pass the model's own arithmetic demands; the
receipt is narrated so the clamp is not invisible to the arrival trace.

**(b) One source for the clause and the guard** (`mStatedStop` / `mStatedStopCount`, new members).
deck123 HIGH-4: the `{right now: M=.., your stated stop=..}` verdict reached **1 of the 20** windows
it was built for, because it read the CARRIED plan (`mCurrentPlan`) while the guard read the reply's
own PLAN — and the carry is cleared by a refusal and by every caveat gate (5 of 31 carried after a
refusal vs 288 of 337 otherwise). Repro in the corpus: s76 answered
`PLAN: L=17, C=6, stop=26; M=40 now; this window: pass (stop reached)` and s77 rendered no `YOUR
PLAN` block and no clause, so the model wrote `x20` into a window carrying no verdict.

The two numbers are now persisted wherever a reply's plan is read (`consumePlan`, and the priority
seam before the guard — s76 was a PASS, so the guard's own short-circuited scan would never have seen
it) and never cleared; the row renders from the store, the refusal still rests on THIS reply's own
PLAN (a refusal must quote words the model just wrote). `repeatPlanScanNumber` /
`repeatPlanStopAndCurrent` were MOVED UP in the file, body byte-identical, so `consumePlan` can call
them.

Livelock check (wave61/corpus-livelock.md): the clause is a `{...}` group — `stripRenderAnnotationsLc`
drops it from the option-set key (pinned), `holdKeyRow` normalises digits inside braces, and neither
member ever reaches `mPromptTail` or an ask key. Both numbers are fixed for the window.

**Prediction (falsifiable, wave-67 corpus):** 0 records carry a `repeat_past_stop_exhausted` stamp
without a `repeat_clamped_to_own_stop` stamp beside it, and no record executes a repeat count greater
than `stop - M` from a reply whose own PLAN states both. Second: the share of repeat-row windows
whose rendered row carries `your stated stop=` rises from 43/91 (deck123 vs126, wave 66) toward the
number of windows that follow any reply that ever stated a stop — and specifically, a window
immediately after a refusal carries the clause.

## I8 — the graveyard is a line of the frame, and an exiled half is not pending

**(a) `Your graveyard (N cards): ...`** (`graveyardZoneLine` + `graveyardEntriesOf`, new pure;
emitted in `serializeGameState` beside the battlefield lines). deck146 HIGH-1: every
graveyard-dependent surface this deck runs is priced off a zone the CURRENT SITUATION block did not
contain, so at `146v130` s29/31/33 the seat spent 165 s re-deriving the census from fifteen turns of
log, disagreed with the row, wrote "contradiction", declined at 3 life and died — while its own guide
(`deck146_strategy.txt:203`) forbids exactly that re-derivation. The frame now carries the seat's own
graveyard on every window **including when it is empty** ("your graveyard is empty" is the fact that
stops a re-derivation), and the opponent's when non-empty. Each entry is name + mana cost + printed
body for a creature — the mana value is what `creature[manacost<=2]` and every other graveyard price
is read against. Identical entries collapse (`Name xN`) the way the battlefield line collapses;
beyond 12 distinct names the REMAINDER IS COUNTED IN THE LINE (`+K more cards under J further names
not listed here`), never silently dropped — an omission the model cannot see is the confabulation
surface the trust doctrine forbids.

Choke-point check: the line is in `serializeGameState`, which IS the board key (`boardKey`,
`:30755`) and the async slot's state half — so a card entering a graveyard now re-opens the one
re-ask per board. That is a real board change (it changes what is castable and targetable), it is
stable within a window, and it is NOT in `mPromptTail`, the ask key's question half, `holdKeyRow` or
the option-set key.

**(b) The exiled half** (`pendingLoopWarningText` gains `halfCanReturn`; the caller's zone table
carries the flag). `146v126` s21-s33: the seat spent its own removal exiling Exquisite Blood and the
banner kept saying *"the pair is one resolution from closing"* — the three windows carrying that
wording are the three slowest of the game (256.5 + 197.8 + 220.1 = **674 s**), all re-deriving whether
the loop closes, and at s32 the model invented a Sanguine Bond it does not have. The zone was named
correctly and then contradicted. The closing claim is now gated on the half sitting in a zone it
returns from (hand / library / graveyard); in exile the banner says so and says the pair is BROKEN,
and keeps the consequence conditionally ("IF it ever does return..."). The returnable face is
byte-identical to the shipped banner (pinned).

**Prediction:** 0 windows in the wave-67 corpus render `is one resolution from closing` about a half
in an exile zone; and no seat re-derives a graveyard census in prose (the `146v130` s29/31/33 shape —
a reply naming graveyard cards the frame did not list) in any window whose frame now carries
`Your graveyard (`.

## I9b — why the no-op conjunction missed its textbook case

`126v125` s83 is the shape AR H8 was built for: row `Cast Tribute to Hunger {right now: they control
0 creatures - at 0 this does nothing}`, answered `CHOICE: 1`, over `PLAN: Cast Tribute to Hunger.
Opponent has 0 creatures, so this does nothing. This is a waste of mana and cards.` It did not fire
for **two independent reasons, both inside `planArguesAgainstRow`** (the seam and `rowSaysNoOp` were
innocent — `rowSaysNoOp` on that exact row is pinned true):

1. **The anaphor.** The sentence that names the row carries no negative and the sentence that carries
   the negative names no row — it says "this". The per-sentence test cannot see a pronoun, and the
   model writes one *whenever it has just named the card*. A negative sentence whose subject is a
   pronoun is now attributed to the row named by the sentence IMMEDIATELY before it, and to nothing
   else: one sentence of reach, a word-bounded pronoun required. A negative about a different card
   with no pronoun ("Damnation does nothing here") still misses; a pronoun whose antecedent is
   another row's card still misses; a plan that never names the row still misses.
2. **s84 has no `PLAN:` label at all** in 5 354 bytes of prose, and the predicate returned false
   before reading a word of it. A reply with no plan label is not a reply with no argument, so the
   whole reply is the span when no marker exists. (s84's *other* defect — first-wins executing row 1
   while the reply's conclusion was `CHOICE: 3 (Cast nothing right now)`, stamped
   `later_answer_ignored;latched_row_mismatch` — is lane AV's answer anchor, not this predicate's.)

Also: `"a waste of mana and cards"` — the corpus's own words — was not in the negative vocabulary
("wasted" was). Both spellings are now in it.

**Prediction:** every wave-67 record whose chosen row carries `this does nothing` / `does not apply`
AND whose reply argues against that row in prose (by name or by an immediately-following pronoun)
carries `plan_contradicts_noop_row_reask`; 0 records repeat the `126v125` s83/s84 shape — the same
dead row taken twice in one turn with the reply saying so — without a re-ask.

## MED — the worked example does not point at a half-dead row

Extends AU R5 (a format example may not put a row the engine has already priced down into the answer
slot) from dead to HALF DEAD (engine MED; `146` s22). `askExemplar` prefers a row that is neither
`rowSaysNoOp` nor `rowSaysHalfDead`; a half-dead row is still exemplified when nothing better exists,
because withholding the FORMAT is the worse surface — the same trade AU R5 made. An ordinary menu's
example is byte-identical.

**Prediction:** 0 `named_row_reask` windows in the wave-67 corpus recover to a row the same window
tagged `HALF DEAD`.

## MED — the game log buckets a death batch the way the board line buckets the board

`123v126` s115 / `126v123` s21 (t15) carry a **208-line contiguous death batch** — 96 byte-identical
`... ceased to exist and left your graveyard` lines and 94 `... died (that Human was N of 96 copies
...; the other K are still there)` lines — about **14 KB of a 25 KB prompt**, while the same 96 tokens
occupy ONE collapsed entry on the battlefield line. Root cause, measured: neither shipped collapser
can reach it. `collapseAdjacentDuplicate` (#W57-D) needs the repeated line ADJACENT and the pair
ALTERNATES; the cycle holder (#W48-D11) needs the repeating block byte-identical and the death line's
two ordinals move on every copy.

`narrationBucketRuns` therefore buckets at RENDER, over the composed body, after `logWindowApply` —
a pure transform of the text about to be printed: `mNarration`, the translog's `events` delta, the
trim marker and every key are untouched. The rule is `joinZoneEntries`': within one contiguous run of
`- ` event lines (a header or blank line ends the batch), a SHAPE (the line with digit runs
normalised) occurring at or above the floor of 4 is printed ONCE, verbatim, at the position of its
first occurrence, with an exact count beside it (`[x96 - this exact line 96 times in this batch]` /
`[x96 - 96 lines of this shape in this batch; only the numbers in them differ]`); every other line
keeps its place and its order, and a batch with no repetition renders byte-identically. Idempotent
(pinned), so two rebuilds of one window agree.

**Prediction:** no wave-67 prompt contains more than 4 lines whose normalised shape is equal inside
one contiguous event batch; the t15-class prompt on deck123/deck126 falls from ~25 KB to under 12 KB.

## MED — hold churn: the census, and what it does NOT prove

Censused `123v126` t13, the 41-window drained turn (the wave's worst): 41 priority windows, whose
answers were **20 plain passes, 11 `<refused: repeat_past_stop>`, 7 executions, 1 hold**. Of the 40
window-to-window transitions, **11 present a byte-identical option set** and 29 a word delta. The
word deltas are dominated by two clauses that are TRUE and decision-relevant: the repeat row's
`[repeat: you control N creatures; activated this turn K times already ...]` (which moves whenever the
model acts) and `{if you pass here, this option is not offered again until the board changes}`.

So the census proves the largest single cause of extra windows in that turn is the **`repeat_past_stop`
refusal pair** (11 of 41 = 27%), which is I6's item — my clamp ends each pair on the second answer
instead of leaving the model free to restate the same count. It does NOT prove a false or stale
surface, so I changed nothing else: I did not touch the activation counter (a true fact the repeat
row is priced on), and I did not change the plain-decline-vs-hold ordering at the priority seam (AS
H7b's territory, and the 20 plain passes there are the model declining to take an offered hold row,
not a missing row).

---

## Gate

Clean rebuild in this worktree (`rm -f bin/wagic && make -f Makefile.sdl -j4`; qmake untouched, no new
sources). Detached unit `w67-AY-gate2`, `MemoryMax=4G` / `MemorySwapMax=0`, foreground until-loop on
`~/.gatelogs/W67_AY_DONE`. Binary freshness-gated after every make.

| leg | result | base (brief) |
|---|---|---|
| build | clean link, `bin/wagic` re-dated | — |
| PARSETEST | **4951 passed, 0 failed** | 4906 (**+45**) |
| suite, `WAGIC_TESTSUITE_THREADS=1` (ground truth) | **1273 tests (0 failed), 67 AI tests (0 failed)** | 1273 / 67 |
| `==Test Failed !==` / `==Test timed out` (T1) | **0** / **0** | 0 / 0 |
| suite, default threads | 1273 tests (2 failed), 67 AI/0, 0 timeouts | — |
| the default-threads failures | `lifeline.txt`, `merrow_reejerey.txt` — the brief's known concurrency-only pair | — |

`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. No scratch registry was written.

### RED-on-base — measured, not asserted

Every mechanism reverted to its wave-66 behaviour with the new cases in place, rebuilt, re-run
(`~/.gatelogs/w67-AY-red.log`): `repeatStopClampCount` returning -1 always; `graveyardZoneLine`
returning ""; the `halfCanReturn` branch disabled; `planArguesAgainstRow` back to marker-required,
no anaphor, no "waste" tokens; the exemplar's half-dead preference removed; `narrationBucketRuns`
returning its input.

**`4929 passed, 22 failed`** — 4929 + 22 = 4951, so **no pre-existing case goes RED under the
reversion and none was deleted**. The RED split: I6 4, I8 8, I9b 3, exemplar MED 2, narration MED 5.
The other 23 new cases are MUST-NOT-MATCH, NEGATIVE, ECHO and KEY-safety pins that pass on base too,
which is what they are for.

## What I did NOT verify

* **No live model probe and no corpus run.** Every model-facing claim (that the pilot reads the
  graveyard line instead of re-deriving, that the broken-pair wording ends the 674 s re-derivation,
  that the bucketed log is as legible as the 208-line one) is unmeasured. The wave-67 corpus is the
  instrument; the predictions above are written to be checked against it.
* **The I6 clamp has no pure RED surface for its WIRING.** `repeatStopClampCount` is pinned on the
  corpus's own numbers, but that the exhausted branch calls it, sets `choice = 0` and narrates the
  receipt is proved only by reading the seam and by the suite's 0-failed — the suite cannot drive a
  GPT seam. The `x0 -> pass` precedent (`repeat_count_zero_pass`, `:31352`) sets the same three
  variables the same way, which is why I set them the same way.
* **The clamp changes what runs on a legal, offered row.** I judge it inside the doctrine (it
  performs the model's own stated number after one re-ask that quoted it, and records both numbers),
  but it is the one change in this lane that alters an outcome rather than a surface. If the wave-67
  corpus shows a window where the model genuinely intended to pass its own stop and the receipt
  reads as a false surface, the dial is the predicate, not the doctrine.
* **The graveyard line's board-key sensitivity is reasoned, not measured.** A card entering a
  graveyard now re-opens the one-re-ask-per-board latch. I argue that is a real board change; I did
  not count how many extra re-asks that buys in a corpus.
* **The graveyard entry text is not zone-aware beyond creature/non-creature.** No `[flashback]`,
  no recursion tags, no exile line — the seat's exile zone is still unrendered, and deck146's Kaya -7
  and Agadeem's X are priced on counts this line supports but does not compute.
* **The 12-name cap is a judgement.** A 40-name graveyard prints 12 names and a counted remainder; I
  did not measure the largest graveyard in the wave-66 corpus to check the cap is never reached in
  practice.
* **`narrationBucketRuns` reorders within a batch.** Later copies of a repeated shape are pulled up
  to the first occurrence, exactly as `joinZoneEntries` does for the board. Lines of other shapes keep
  their order, and a header or blank line bounds the batch, but I did not audit every emitter for a
  batch in which the ORDER of two same-shape lines carries information.
* **I9b's anaphor reach is ONE sentence, deliberately.** A retraction two sentences after the naming
  sentence still misses. I did not measure how many corpus replies put the negative further away.
* **Two MEDs are NOT fixed, with root causes recorded instead of a rushed change:**
  (1) **152 MED-3** (an opposing static tagged on a creature without naming its source). The tag
  comes from `legibleKeywordName("doesnotuntap")` — a LIVE KEYWORD on the card, not an ability with a
  source. The grantor machinery that exists (`stackAbilityLine`, `:4023`) is stack-only; attributing a
  continuous static to its granting permanent needs a walk of every permanent's abilities, and naming
  the WRONG source is a false surface worse than naming none. Docketed, not attempted.
  (2) The **`plan_contradicts_noop_row` ask-seam gate on a reply with no coded line** is untouched —
  that is lane AV's I2.
* **No cross-lane interaction was tested.** Lane AV owns the reply protocol and the answer anchor and
  touches `parseChoice`/`findAnswerLabelLine`; my I9b change is inside `planArguesAgainstRow`, which
  AV's anchor feeds (a different coded line adopted as the answer changes which row the conjunction is
  evaluated against — the two compose, but the composition is untested). Lane AW owns row MEDs that
  print into the same option rows my I6 clause rides; lane AX owns `mLastLatencyMs` ordering and the
  fallback record, whose new field pair lands beside my `repeat_clamped_to_own_stop` stamp in
  `mLastParseNote`. All four lanes append their PARSETEST blocks at the corpus END — the merge conflict
  is the tail, and the merged count must equal exactly 4906 + the sum of the four lanes' additions
  (mine: **+45**).
