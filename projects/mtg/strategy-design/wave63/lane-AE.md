# Wave-63 lane AE — E8, E12, E17, E18

Branch `w63-lane-AE`, worktree `worktrees/lanes/w63-AE`, base master `41fbad879`.

Files touched: `projects/mtg/src/AIPlayerGPT.cpp` (E8 render, E18 render, PARSETEST),
`projects/mtg/bin/Res/sets/primitives/borderline.txt` (E12, card script only),
`projects/mtg/src/Player.cpp` + `projects/mtg/src/TestSuiteAI.cpp` +
`projects/mtg/include/TestSuiteAI.h` + `projects/mtg/src/GameStateDuel.cpp` (E12 fixture surface,
E17), `JGE/src/SDLmain.cpp` (E17), two new fixtures under `bin/Res/test/` registered in
`_tests.txt`.

---

## What shipped

### E8 — the resolved land-drop witness says whether the subtype is PRINTED or GRANTED

**The ledger's finding does not survive the corpus.** Engine-seat HIGH-3 called three
resolved-UNTAPPED clauses false because their witness "lacks the named subtype". Both cited
repros were re-read in the corpus and in both the witness IS a Swamp at the moment the row
renders, because **Urborg, Tomb of Yawgmoth is on the seat's own battlefield**
(`lord(land) transforms((swamp))` makes every land a Swamp):

* `matchups-20260905-133058/1788633073-…deck126-…-vs-…deck125` seq 152 — `Your battlefield`
  ends `…; Swamp; Urborg, Tomb of Yawgmoth; Vampire`. The engine seat's own note says it
  "checked: Bayou, Forest, Isolated Chapel ×2, Plains, Scrubland ×2, …" — the elision is where
  Urborg was.
* `…deck126-…-vs-…deck146` seq 64 — Urborg is on that battlefield too (present on this seat's
  own battlefield line for 40 consecutive seqs, 31-70).

So the verdict was right and the reason was right *by the rules*. What was wrong is that a
pilot who checks the reason against the card finds nothing: `Savannah` prints `Forest Plains`,
`Isolated Chapel` prints no subtype at all, and the row asserted "a Swamp" with no way to tell
where the type came from. The witness search matched by type id, which is correct; the RENDER
withheld the fact that decided it.

Two changes in `landEntersTappedTagResolved` / `landTapResolve`, and **no verdict moves**:

1. **A printed witness is preferred.** The walk now records, per gate slot, whether the
   candidate's own primitive (`c->model->data->hasType(id)`) carries the subtype, and a printed
   candidate replaces a granted-only one. On the seq-64 board that names `Bayou` (prints
   `Swamp Forest`) instead of `Savannah`. The choice between slots follows the same rule — a
   printed witness in slot 2 beats a granted one in slot 1, which is the shape that produced
   both false-looking rows.
2. **A granted-only witness says so:** `(you control Savannah, a Swamp - a type it has on the
   battlefield now, not one printed on the card)`. Nothing is deleted; the card's own sentence
   is still quoted beside it, and a caller that hands in no printed/granted information gets
   the wave-62 string byte for byte.

Render change only, so PARSETEST is the gate: a positive per branch, must-NOT-match negatives
(a printed witness never carries the note; no witness at all still reads TAPPED), the
printed-beats-granted pair in both slot orders, the no-information fallback pinned equal to the
wave-62 string, and the echo shape (the bracket binds `CHOICE: 1 (Play Woodland Cemetery)` and
strips to `Play Woodland Cemetery`).

### E12 — Nadaar, Selfless Paladin stops pumping itself (card script, no engine change)

Oracle verified directly at `https://api.scryfall.com/cards/named?exact=Nadaar,%20Selfless%20Paladin`
on 2026-09-05: `{2}{W}` Legendary Creature — Dragon Knight 3/3, "Vigilance — Whenever Nadaar
enters or attacks, venture into the dungeon. — **Other** creatures you control get +1/+1 as
long as you've completed a dungeon."

`borderline.txt:75387` read `this(variable{pdungeoncompleted}>0) lord(creature|myBattlefield) 1/1`
— `creature`, not `other creature`, so with a dungeon completed Nadaar was a 4/4 alone. Now
`lord(other creature|myBattlefield) 1/1`. One line, one card, no engine change.

Two fixtures, both at `bin/Res/test/`, both registered in `_tests.txt`:

* `w63ae_nadaar_other_creatures_only.txt` — Nadaar (card id 527314) alone with a completed
  dungeon, Lightning Bolt at it. **RED on base**, verified by reverting *only* the primitive
  line on the built binary and re-running: `==Card ID not the same. Didn't find 527314` /
  `==Card number not the same in player 0's battlefield==, expected 1, got 2` — the 4/4
  survived. GREEN with the script fixed.
* `w63ae_nadaar_still_pumps_others.txt` — the sibling guard: Grizzly Bears (printed 2/2) is
  3/3 beside Nadaar and survives Shock. GREEN on base and after; it fails only if the lord
  stops applying to OTHER creatures.

Fixture surface this needed: `dungeoncompleted:` is now a parsed player-state key
(`Player::parseLine`) and is carried into the live seat by `TestSuiteGame::initGame` beside
life and poison. Additive — an absent key leaves the constructor's 0, and nothing else reads
the key.

### E17 — the intermittent is the WALL CLOCK, and the suite no longer runs on it

Root-caused and fixed. The measurement chain:

| step | result |
|---|---|
| the fixture alone, 30 fresh processes, THREADS=1 | **30 pass, 0 fail** — "solo" hides it |
| the fixture alone, 8 full-suite runs at default threads | 0 fail (the worker path is already deterministic: `ThreadProc` feeds `Update` a synthetic dt 1,2,3,…) |
| 40 copies of the fixture in ONE process, THREADS=1, no seeds | **28 of 40 fail**, the FIRST always passes |
| 6 copies, all carrying the SAME `seed 12345`, two runs | `.FF..F` then `.FFFF.` — **same seed, different results**, so the RNG seed is not the variable |
| the same probe under `setarch -R` (ASLR off), three runs | still three different vectors — **not pointer/heap ordering** |
| 20 copies, THREADS=1, three runs at `WAGIC_FASTCLOCK=0.1` | `.F.FF.FF..FFFFFFFFFF` **three times identical** |

The divergence itself is a mana-payment ORDER divergence, not a rules divergence: the passing
run pays `plains #1;#2 / #3;#4 / #5;katilda`, the failing one `plains #3;#1 / #2;#5 /
#4;katilda`, and the fixture asserts the exact narration `paid {1}{w} with plains #5; katilda,
dawnhart prime`. The scripted command sequence is byte-identical in both; the first difference
in the trace is an `ActionLayer::doReactTo` index.

**Cause:** the suite's MAIN-THREAD path (`WAGIC_TESTSUITE_THREADS=1`, and the interactive
suite) drives `GameObserver::Update` with REAL wall-clock dt, and `AIPlayerBaka::Act` is
dt-throttled at ~0.07 s. How many times the AI acted between two scripted commands was
therefore a function of machine timing. The worker path has had a fixed dt all along; the path
a solo run takes did not.

**Fix** (`JGE/src/SDLmain.cpp`, `fastClockDt`): when `WAGIC_TESTSUITE` is set and
`WAGIC_FASTCLOCK` is not, the fixed 0.1 s tick is the default. An explicit `WAGIC_FASTCLOCK`
still wins; non-suite runs are untouched. After the fix the 20-copy probe gives the identical
vector on three consecutive runs **with no environment override**, and
`intrepid_adversary_repeated_payment.txt` passes 5 of 5 solo and in both gate suites.

A second, smaller determinism hole found on the way and closed with it: a fixture that names
no `seed` kept the `GameObserver` constructor's `time(0)`, and even a fixture that DOES name
one had it applied only *after* `startGame()` on the worker path — so the library shuffle was
always wall-clock random. `TestSuiteGame::suiteSeed()` now returns the declared seed, else a
stable djb2 of the fixture's filename, and it is applied before the game starts on both paths.

### E18 — D7's taps half, scoped to the payments the mana half priced

The clause had no corpus window (0 of 100 `{repeat cost:}` rows tapped a creature), and reading
the composed path found a real ambiguity rather than a missing render. On a CAST row
`{paying this taps: …}` is unambiguous — one payment. On a pay-repeat row the seat asked for
N counters and the engine pays only the K its mana covers, so the bare clause let "this" read
as all N: the row would name two creatures beside a `{repeat cost:}` half saying the mana stops
after 2 of 3.

`payRepeatTapsClause(names, restrictions, paid, counters)` (pure) now wraps
`paymentTapsClause` and, where `paid < counters`, appends `(that is the 2 payments your mana
covers, not all 3)`. Both halves are fed the SAME `paid` — the mana engine's answer for this
row — so they cannot disagree by construction. The `{paying this taps: ` prefix and the brace
shape are unchanged, so `stripNarrationDecoration` and every echo path are untouched.

PARSETEST: the positive on the composed Intrepid Adversary board (Katilda + Elite Spellbinder,
both untapped attackers on the seat's own pre-combat main, so the attack restriction renders);
the agreement case (the K in `{repeat cost: …pays for 2 of them and stops}` is the K in the
taps clause); must-NOT-match negatives (a fully affordable row is byte-identical to the
cast-row clause; no payment count claims no share; a payment tapping no creature prints
nothing); the singular/`cannot block` variant; and the echo shape with both tags on the row.

---

## Gate

Detached unit `w63-AE-gate`, memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`), foreground
until-loop wait (200 s). Build `rm -f bin/wagic && make -f Makefile.sdl -j4`, clean link. Every
probe run in this lane also ran under `systemd-run --user --scope -p MemoryMax=4G
-p MemorySwapMax=0`.

| leg | result |
|---|---|
| suite, default threads | **1256 tests (2 failed), 66 AI tests (0 failed)** |
| the 2 failures | `lifeline.txt`, `merrow_reejerey.txt` — the known concurrency-only pair |
| `==Test Failed !==` / `==Test timed out` | 2 / 0 |
| suite, `WAGIC_TESTSUITE_THREADS=1` | **1256 tests (0 failed), 66 AI tests (0 failed)**, 0 / 0 |
| PARSETEST | **4130 passed, 0 failed** |
| PARSETEST delta | base 4111 → 4130 = **+19**, exactly the 19 `CHECK(` lines this diff adds |
| suite count delta | base 1254 → 1256 = the two E12 fixtures |
| encoding | `git diff \| /usr/bin/grep -c $'\357\277\275'` = **0** |
| memory | no run killed under the 4G cap |

Base measured on the seeded binary before any edit: suite 1254 (2 failed at default threads,
the same pair), 66 AI tests, PARSETEST 4111/0. The brief's baseline says 65 AI tests; this
worktree's base binary reports 66. `intrepid_adversary_repeated_payment.txt` was **not** among
the base failures in any of the 8 base full-suite runs.

---

## Predictions for the wave-63 corpus (falsifiable, per item)

**E8.** Every `enters UNTAPPED - it makes mana this turn (you control X, a T)` clause will
satisfy one of exactly two shapes: either X's primitive carries subtype T (checkable against
`sets/primitives/*.txt`), or the clause carries `- a type it has on the battlefield now, not
one printed on the card`. Falsifier: one clause of neither shape. Secondary: **0** clauses will
name a granted-only witness on a prompt whose own battlefield line also holds a land whose
printed subtype is T (wave 62: 3 of 61, all three of that shape). Third: the count of
`enters UNTAPPED` clauses does not fall — this changes which permanent is named and what is
said about it, never the verdict.

**E12.** On any prompt where Nadaar, Selfless Paladin is on a battlefield with a completed
dungeon, Nadaar's own rendered power/toughness will read its printed `3/3` plus only effects
that are not this lord, and every OTHER creature that seat controls will read one higher than
its printed line. Falsifier: one board where Nadaar reads `(4/4)` with no other pump present.

**E17.** `intrepid_adversary_repeated_payment.txt` will not fail again in any suite run of this
build, at any thread count — the fixture is now a pure function of its own file. Stronger and
more useful: for ANY fixture, two runs of the same binary over the same `_tests.txt` will
produce the same per-file pass/fail vector. Falsifier: one fixture that passes in one run and
fails in the next with nothing else changed. (The two known concurrency failures are exempt:
they are a THREADS>1 race, not a clock effect, and this lane did not touch them.)

**E18.** Every `{paying this taps: …}` clause that sits on a row also carrying
`{repeat cost: …}` will either state the same payment count that row's `pays for K of them`
half states, or state no count at all — and in the latter case that row's cost half will read
`pays for N of them` with N equal to the row's own counter number. Falsifier: one repeat row
whose two halves name different numbers. Weaker second half: this window may again fail to
arise (wave 62: 0 of 100), in which case the item is UNTESTED, not passed.

---

## What I did NOT verify

* **No live model probe was run.** E8 and E18 are proved by PARSETEST over pure functions plus
  the suite's no-regression; neither has been observed in a real rendered prompt from this
  build.
* **E8's impure half is still untested at runtime.** `landEntersTappedTagResolved`'s battlefield
  walk — including the new `c->model->data->hasType(id)` printed test — has no suite coverage
  (the suite never opens a GPT land-drop ask). Specifically unverified: that `model->data` is
  non-NULL for every land instance the walk can meet (both dereferences are guarded, and a NULL
  model degrades to "granted", which is the honest reading, but I did not measure how often
  that path is taken); and that a card whose printed subtype list is grown at load time by
  something other than `subtype=` is classified the way this code assumes.
* **E8: I did not fix Urborg's invisibility on the battlefield line.** The line prints
  `Urborg, Tomb of Yawgmoth` as a bare land name with no statement that it is making every land
  a Swamp. The new note tells the pilot the type is granted; it does not say BY WHAT. Naming
  the granting permanent is a bigger change than this lane's finding supports, and it is the
  obvious next ask if the note reads as a dead end in the corpus.
* **E8: the engine seat's third cited row** (seq 175, the repeat of the Isolated Chapel clause)
  was not separately opened; it is the same seat and the same board as seq 152.
* **E12: the fixtures do not exercise the venture triggers.** Nadaar is placed in play at INIT,
  so neither its ETB nor its attack trigger fires, and no dungeon card is in the command zone.
  The `dungeoncompleted:` key sets the COUNT the payoff reads; it does not model a dungeon.
  I also did not check any other card in the primitives for the same `lord(creature)`-for-
  "other creatures" defect — the census is not in this lane's items.
* **E17: the fixture is now deterministic but not seed-PORTABLE.** Twenty copies of it under
  twenty different filenames (hence twenty seeds) give 6 passes and 14 failures, identically on
  every run. The failures are legal alternative permutations of five interchangeable Plains, so
  this is a fixture-robustness problem (the assertion names `plains #5`), not a live defect. I
  did NOT rewrite the assertion, because the item asked for the cause and not the test.
* **E17: I did not measure what the fixed suite dt costs in wall time**, nor whether any
  fixture's timing-sensitive behaviour (the `mAiPendingTicks` freeze, the stall floor's
  20-second release) is now reached differently. Both gate suites are green and the THREADS=1
  suite went from a hidden intermittent to 0 failures, but "green" is not "same".
* **E17: the two known concurrency failures are untouched and unexplained by this lane.**
  Two of the eight base full-suite runs at default threads also ended in a SEGFAULT after the
  results line; that crash is not in this lane's items and I did not diagnose it. It did not
  recur in either gate leg.
* **E18: the taps clause still has no corpus window.** Everything here is source reading plus
  PARSETEST over the pure composition. I did NOT prove that
  `ManaEngine::selectAutoTapProducers` names the same sources the payment will actually tap,
  nor that its answer agrees with the `planPayment` call that decided `paid` — the two are
  separate engine calls over the same board, and this lane assumes rather than measures their
  agreement.
* **No guide, no `_cards.dat`, and no deck file was edited.**
