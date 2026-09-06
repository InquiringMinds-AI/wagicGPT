# Wave-69 lane BJ — the eight findings of `wave69/codex-review.md` (Fable seat)

Base: master 8110ea2de (= 02201b130 after the BF–BI merge, plus the review commits).
Worktree `worktrees/lanes/w69-BJ`, branch `w69-lane-BJ`. One file touched:
`projects/mtg/src/AIPlayerGPT.cpp`.

## Verdicts

| # | Sev | Verdict | What shipped | RED evidence |
|---|-----|---------|--------------|--------------|
| F1 | HIGH | **CONFIRMED** | one builder for `{paying this taps: }`; dev-gated duplicate census on the composed row | source: two builders at 31076 / 31081-82 on base; no runtime path can see it (see "did NOT verify") |
| F2 | HIGH | **CONFIRMED** | standing bodies fold into the crack-back cover only where an attack of the seat's own cannot tap them | code + caller phase; no fixture (see below) |
| F3 | MED | **CONFIRMED** | the `+1 this card` term requires `moveto(mylibrary) all(this)` in the script | 2 PARSETEST cases FAIL on the reverted helper |
| F4 | MED | **CONFIRMED** | the material ALTERNATIVE names itself as lethal when it is | 1 PARSETEST case FAILs on the reverted helper |
| F5 | MED | **CONFIRMED, half-fixed by design** | library surfaces count parked-out-of-library cards; stranding NOT root-caused — ledgered | BG's own fixture proves the parked state; the stranding has no fixture |
| F6 | MED-LOW | **CONFIRMED** | the negation must govern the verb phrase that names the row | 2 PARSETEST cases FAIL on the reverted predicate |
| F7 | LOW | **CONFIRMED** | the floating pool is folded into the "0 mana available" reach term | call-site only; the helper is unchanged, so its cases pass on base too |
| F8 | LOW | **CONFIRMED** | the second take of an invited no-op row stamps `noop_row_retaken` | stamp rename; no runtime test |

Nothing was REFUTED. Every finding reproduced against the code at the line it named.

## Mechanisms

**F1 — one row, one tap clause.** The wave-69 merge kept BG's context copy of
`out << paymentTapsClause(taps, tapRestrict);` above BI's de-duplicated builder
(`paidTapsClause = paymentTapsClause(...); out << paidTapsClause;`), so every
affordable-by-tap cast row whose auto-tap picks include a creature or the source
itself printed the guide-keyed clause twice. The builder is the copy kept — the
self-tap clause below it reads `paidTapsClause`'s text to avoid a third spelling,
so deleting the builder instead would have re-opened BI's K7 item. The
`cannotPayNowClause` call keeps its position after the taps clause, so the
composed row is byte-identical to the intended single-clause row.
**String stability**: the printed literals `{paying this taps: ` and
`{you cannot pay this right now: ` are unchanged; only the second copy is gone.

Because the defect lived at the COMPOSITION seam and not inside any emitter, a
per-helper PARSETEST case cannot see it. Added instead: a pure
`clauseOccurrences(row, literal)` (PARSETEST-covered, positive/negative) and a
`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)` census at the end of
`describeAction` that DebugTraces any row stating `{paying this taps: `,
`{tapping `, `{right now: `, `{crack-back cover: ` or `{reserve: ` more than
once. Every suite run and every corpus game runs a `_DEBUG` build, so a
re-introduction of this merge shape is loud rather than silent.

**F2 — the cover the seat has not spent yet.** `crackBackCoverFacts` counted
every untapped creature of the seat's as certain crack-back cover, and the row
renders in MAIN 1 as well as MAIN 2. A body untapped in main 1 that attacks this
turn is tapped through the whole of the opponent's turn, so on the lethal screen
the clause was stating a survival number against bodies the same turn's plan
sends away. New `attacksSettled` argument: true exactly when it is the seat's own
turn AND the phase is at or past `MTG_PHASE_COMBATATTACKERS` — i.e. the attack
declaration is behind it and the untapped bodies are the ones it did not send.
Before that, and on the opponent's turn (where the seat's own turn still comes
first), a standing body counts only if attacking cannot tap it (`VIGILANCE`) or
it cannot be sent at all (`DEFENDER` / `CANTATTACK`). A `MUSTATTACK` body without
vigilance is excluded by the same test, which is what the rule demands of it.
The clause's wording is untouched; only the integer it is built from changed, and
it can now only UNDER-state cover, never over-state it.

**F3 — the exiling refill.** `abilityShufflesGraveyardIntoLibrary` gated on
`moveto(mylibrary)` + `all(*|mygraveyard)` and the clause then asserted
"and this card ... L to L+N+1". Feldon's Cane (`{T}{E}:...`, mtg.txt:39576),
Campfire (`{2}{T}{E}`, borderline.txt:15766) and Archangel's Light (a sorcery,
mtg.txt:5199) all match the gate and none of them ends in the library. The
predicate now reports `selfToLibrary` = the script contains the adjacency
`moveto(mylibrary) all(this)` — which is Elixir of Immortality's own line
(mtg.txt:34776) and nothing else in the lexicon; the clause drops the
"and this card" term and the +1 when it is absent. Elixir's string is
byte-identical to what wave 68 shipped (PARSETEST pins it).

**F4 — the alternative that kills you.** `blockKeepAlternativeClause` was pure
over two damage figures and never took life, so at 6 life it could print
"lets in 7 combat damage instead of 4 ... Both lines are legal" on a lethal
screen. It now takes `lifeAfterGains` — the SAME effective life the header's own
lethal verdict is computed from (`life - blockTriggerGain - blockLifelinkGain`),
so the two authorities in the window cannot disagree — and states
"7 KILLS you at 6 life: this line LOSES THE GAME and the line above does not."
when `keepDamage >= lifeAfterGains`. The clause is now built at the call site
AFTER those gains are counted (the pairing and its damage are collected where
they were); `-1` claims nothing. The non-lethal wording is byte-identical to
wave 68 (PARSETEST pins the full string).

**F5 — the stranded payload, and the surfaces that read 0.** BG's K1 fix stops
the false deck-out, so the game now CONTINUES with `library->nb_cards == 0` and
the whole library parked in `reveal`. The board frame already folded those cards
(`yourLibraryLine`'s `myLibraryInReveal`); the numeric consumers did not. New
`libraryCountWithParked(Player*)` uses the same `previousZone` stamp and now
feeds `xLibraryReserve` (so `xLibraryCeilingX` / `xLibraryReserveWhy` and the X
ceiling stop reading 0) and K4b's refill clause. `yourLibraryLine` is unchanged.

The stranding itself is NOT root-caused and is NOT fixed. **Ledger line for the
next corpus:** a `library 0` line, or a game continuing with a `StackAbility.
(Source: <a tutor>)` resting on the stack, is THIS known defect and not a new
one. Code-grounded HYPOTHESIS, unverified: `MTGRevealingCards::toResolve`
`addObserver`s the option-one payload and then calls `fireOneShot`, which returns
immediately unless `a->oneShot` — the file's own comment at
`AllAbilities.cpp:445` states that an `addObserver`'d ability of the
ActivatedAbility family is never resolved by anything. Idyllic Tutor's option one
is a targeted `transforms((...))` payload. I did not build a fixture that arms
the real chooser (BG's fixture deliberately reaches the parked state by a route
where option one never arms), so this is a reading, not a diagnosis.

**F6 — the negation that governs.** `proseNegatesTakenRow` was a bag of words
within a sentence: any cue followed by the row's short name anywhere later.
New `negationGovernsRowName`: the bridge between the cue and the name must carry
no clause boundary (`,` `;` ` so ` ` because ` ` but ` ` while ` ` since `
` unless ` ` and ` ` if ` ` when `), be at most three words, and — when non-empty
— contain a verb a menu row is taken with. All three corpus firings keep their
shape ("NOT cast Damnation", "should not cast Soul Shatter", "cannot cycle Stone
Rain"); the two approving shapes the review named are now MUST-NOT-MATCH cases.
A failed governance test `continue`s to the sentence's next cue rather than
abandoning the sentence, so no true positive is lost to cue ordering.

**F7 — floating mana is mana available.** `cannotPayNowClause(windowReach())`
became `cannotPayNowClause(windowReach() + getManaPool()->getConvertedCost())`.
`potentialColorReach` counts untapped producers only, so 1 floating after a
ritual with no untapped source made the row state "0 mana available" over a
non-empty pool — a false surface the pilot is instructed to believe. The clause
literal is unchanged; it simply stops firing when the pool is not empty.

**F8 — retaken, not exhausted.** The no-op re-ask's own words are "that row if
you meant it anyway", so the same answer coming back is the offer being ACCEPTED.
Both seams now record the pre-re-ask choice (`mPriorityReaskPriorChoice` /
`mAskReaskPriorChoice`, which the noop branches did not set) and stamp
`noop_row_retaken` when the second answer takes the same row; every other outcome
keeps its existing stamp, so `plan_contradicts_noop_row_exhausted` again means
what it says.

## Gate

Hermetic build from `<wt>/projects/mtg` (`rm -f bin/wagic && make -f Makefile.sdl -j4`),
binary freshness checked with `ls -la bin/wagic` after every make; every run under
`MemoryMax=4G MemorySwapMax=0`, detached unit + foreground done-file wait.

- PARSETEST: **5420 passed, 0 failed** (base 5402 → +18: F1 4, F3 4, F4 3, F6 5, F7 2).
- Suite `WAGIC_TESTSUITE_THREADS=1`: **1279 tests, 0 failed; 67 AI tests, 0 failed;
  0 timeouts** (`==Test Failed !==` 0, `==Test timed out` 0).
- `git diff | /usr/bin/grep -c $'\357\277\275'` = 0.

**RED probe** (one build with the behavioural halves of F3/F4/F6 reverted, the new
cases in place): `5415 passed, 5 failed` —
`F3 REPRO an exiling refill states L+N`, `F3 MUST-NOT-MATCH no self term`,
`F4 POSITIVE at 6 life a 7-damage material line is named as lethal`,
`F6 MUST-NOT-MATCH approving prose whose negation governs another verb phrase`,
`F6 MUST-NOT-MATCH approving prose across a ", so" clause boundary`.
Log: `~/.gatelogs/w69-BJ-parsetest-RED.log`.

**One flake, cleared by counterfactual.** An intermediate full-suite run failed
`intrepid_adversary_repeated_payment.txt` (the intermittent the wave-69 brief
already names). Scoped 12× on THIS tree: 8/12 failed. Scoped 12× on the SAME tree
with `src/AIPlayerGPT.cpp` reverted to base: **8/12 failed, identical**. It is
pre-existing and unrelated to this lane. The gate above is the restored-source
build.

## Predictions for the wave-69 corpus (falsifiable)

1. No prompt contains a row with two `{paying this taps: ` clauses, and no
   `W69-BJ F1` line appears in any game's stderr.
2. No `{crack-back cover:` clause counts a standing body in a main-1 window; the
   `You ALSO already control` sentence appears only at/after declare-attackers,
   or for vigilant / cannot-attack bodies.
3. No refill clause says "and this card" for a card whose script pays `{E}`;
   Feldon's Cane and Campfire render `from L to L+N`.
4. Every `ALTERNATIVE, chosen for MATERIAL` line whose damage is >= the seat's
   effective life carries the `KILLS you at` sentence.
5. `plan_choice_conflict` re-asks bought by `decision_reversed_in_prose` fall
   relative to wave 68 on approving-prose shapes; the s34 shape still fires.
6. No row states `0 mana available` in a window where the seat's mana pool is
   non-empty.
7. `plan_contradicts_noop_row_exhausted` no longer appears on a record whose
   second answer equals its first; those records read `noop_row_retaken`.
8. `Your library: 0` may still appear with cards parked in `reveal` (F5's
   unfixed half) — but the X ceiling and the refill clause no longer read 0.

## What I did NOT verify

- **F1 has no runtime RED.** The duplicate lives in `describeAction`, which
  PARSETEST cannot instantiate and which no Baka suite game calls (it is an
  `AIPlayerGPT` method). The evidence is the source diff plus the new dev census;
  I did not run a live GPT game against Spark to watch a row render.
- **F2 has no fixture.** It is a count change gated on board state inside a
  helper that needs two players, a battlefield and a phase. I verified the caller
  renders in main 1 by reading `FindCardToPlay`'s call site, not by observing it.
  The PARSETEST corpus pins the clause's echo shape only.
- **F5's stranding is not diagnosed.** See the hypothesis above; no fixture arms
  the real Idyllic Tutor chooser, and I did not run a replay or gdb session.
- **F7 and F8 have no failing test.** F7's helper is unchanged (the defect was
  the argument), and F8 renames a stamp; neither is observable from PARSETEST.
  Their cases pass on base as well as after, and are documentation, not RED.
- No live model probe was run this lane; no corpus was run.
- I did not re-measure the wave-68 clause byte sizes, so I make no claim about
  F1's effect on the BD-6 row-size item beyond "one clause instead of two".
