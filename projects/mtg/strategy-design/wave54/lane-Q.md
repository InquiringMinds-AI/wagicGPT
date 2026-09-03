# Lane Q — D35 / owner decision O4: `kMaxOptSources` measured, then replaced

**Docket:** wave53/engine-ledger.md **D35** (raised by lane S, not changed there) and
`audit-20260902/10-owner-decisions.md` **O4** — "sure." Measure-then-decide.

**Headline: the premise of the ticket was wrong, and the measurement says so.**
The 14-source cap was never protecting anyone from an expensive search. The search costs
**6–15 ms at -O2 on every board it engages on — i.e. today, BELOW the cap** — and it gets
*cheaper*, not dearer, as the board widens. Lane S's "0.08 ms" was the cost of the **bail**,
measured on a board (16 producers) that was over the cap. Raising the cap is free; the search
itself was the expense.

---

## 1. How it was measured

A throwaway suite command `benchrefine <reps> <card>` (removed before commit, with the
instrumentation counters it read) called `ManaEngine::selectAutoTapProducers(..., preserveOptions=false)`
once for the baseline plan, then timed `ManaEngine::refineForOptions` over 100 reps with
`clock_gettime(CLOCK_MONOTONIC)`, reporting median/p90/max plus the search's own visit
counts (configurations scored, ability assignments tried, payability questions asked,
enumeration odometer steps).

Six throwaway fixtures (also removed) built boards of **14 / 16 / 20 / 24 / 32 candidate
producers** with a realistic mix — basic lands of five types, Selesnya/Gruul/Simic/Boros/Dimir
Guildgates (two mana abilities each), Karn's Bastion (affordable `{4},{T}` utility), Kher Keep
(unaffordable `{1}{R},{T}` utility), Mind Stone (rock, plus a sacrifice-draw activation),
Llanowar Elves and Birds of Paradise (five any-colour abilities), plus **Twitching Doll**, the
`&&`-wrapped producer of D36 — which, as D36 says, the reader cannot see and which therefore
never counts as a candidate. Hands were **7** castable cards (one variant at **3**), paying
Hill Giant `{3}{R}` (k = 4 sources tapped). Every producer was made usable and untapped with an
`eot`/`eot`/`goto firstmain` prelude, and the candidate count was confirmed by the probe's own
`sources=` read-back, not assumed.

Both builds: the tracked `Makefile.sdl` (**-O0**, what the desktop debug build is) and a
parallel **-O2** build (`Makefile.sdlO2` + `objs-o2`, lane W's recipe — both deleted
afterwards). The Vita CMake builds at -O2, so **-O2 is the column that matters**; the audit's
console factor is **×10–20** on top of it.

## 2. The measurement — `refineForOptions`, median per call

Cap raised via `WAGIC_OPT_SOURCES_CAP` so every board could be measured engaged.

| candidate producers | -O0 | **-O2** | -O2 × 10–20 (Vita estimate) | configs scored | payability questions |
|---|---|---|---|---|---|
| 14 (at the shipped cap) | 117 ms | **15.2 ms** | 152–304 ms | 514 | 11,280 |
| 16 (the owner's board)  | 104 ms | **12.3 ms** | 123–246 ms | 514 | 8,643 |
| 20 | 90 ms | **10.6 ms** | 106–212 ms | 514 | 9,217 |
| 24 | 72 ms | **8.3 ms** | 83–166 ms | 514 | 7,761 |
| 32 | 58 ms | **6.0 ms** | 60–120 ms | 514 | 4,921 |
| 24, 3-card hand | 47 ms | **5.6 ms** | 56–112 ms | 514 | 5,589 |

**Bail cost** (board over the cap — what lane S measured): 9 µs at 16 sources rising to
18 µs at 32, at -O2. Three orders of magnitude under the engaged cost.

Three things fall out of the counts:

1. **Cost is set by the flat `evaluated > 512` configuration cut-off, not by the source
   count.** Every board hits it. `payability questions ≈ configs × (hand options + ability
   options)`, and each question was an allocating `ManaCost::canAfford` (a `Diff`, its result,
   and for `anytypeofmana` a third object) inside a per-source `vector<vector<AManaProducer*>>`.
2. **Wider boards are CHEAPER.** With more producers left over, `payableFrom`'s assignment
   product passes `kMaxAssignments` (64) sooner and takes the permissive-sum short-cut
   (`payable_combos` falls 10,669 → 0 between 14 and 32 sources).
3. **The freeze the owner reported is on the CHEAP side of the cap.** His 16-producer late
   board bailed in 9 µs; the same deck four turns earlier, at 12–14 producers, paid
   ~15 ms × 10–20 = **150–300 ms per focus change**. That is a defensible mechanism for
   "almost frozen when attempting to select cards in my hand" that lane S's numbers hid,
   because lane S measured only the board that bailed.

## 3. What shipped

The recommendation is **not** a bigger `kMaxOptSources`. The cap is the wrong knob; it was
hiding an expensive search rather than bounding one. Three changes, in `ManaEngine.cpp` only:

**(a) The payability question is allocation-free.** `fastDiff` / `fastCanAfford` / `fastPay`
mirror `ManaCost::Diff`, `isPositive` and `pay` on a stack array (`PayVec`, a compile-time
12-int vector) for the shape the search already restricts itself to — a probed cost with no
hybrid symbols and no X. Anything else (hybrids, X, a colour count larger than the array)
falls through to the real `ManaCost` path, now `payableFromSlow`, byte for byte unchanged.
Each producer's output is converted once; each scored option's spend-restriction filter
(`ManaEngine::spendAllowed`, a `dynamic_cast` plus a `TargetChooser` build) is resolved once
into a `PayOption` instead of per source per question. `scoreConfig` keeps the remainder as
source indices on the stack (it built a heap vector of pointers and a `std::set` of chosen
cards per call). The class-prefix odometer keeps its sum incrementally.

*This half changes no verdict, and the probe proves it:* every search counter — configurations
scored, assignments, payability questions, odometer steps, and the `changed=` flag — was
**bit-identical** before and after on all six boards. It is the same search, done without
allocating. -O0 117 → 17.1 ms; -O2 15.2 → 3.2 ms at 14 sources.

**(b) The flat 512-configuration cut-off becomes a measured WORK budget.**
`optWorkBudget()` (default **4,000 payability questions**, `WAGIC_OPT_WORK_BUDGET`) divides by
the number of scored options on the board to get the configuration budget, floored at 64 and
**never above the 512 the old cut-off allowed** — so no board does more work than today. At
15 options that is 266 configurations. This *is* a behaviour change on boards that offered
more than 266 canonical configurations; the honest framing is that such boards were already
getting a truncated search at 512, and 512 was an unmeasured number while 4,000 questions is
the number the timings support.

**(c) The candidate cap: 14 → 32.** With (a) and (b) the search is cheapest on the widest
boards, so the cap now exists only to bound the setup legs and the 64-wide stack arrays.
32 covers every board seen in play (the owner's was 16; the widest measured self-play boards
are 24–27 permanents). **`WAGIC_OPT_SOURCES_CAP=14` restores the old reach** — the disable
flag, and the thing the new fixture's RED result is produced with.

### Cost after, -O2, same boards

| candidate producers | -O2 before | **-O2 after** | after × 10–20 | configs scored |
|---|---|---|---|---|
| 14 | 15.2 ms | **1.88 ms** | 19–38 ms | 268 |
| 16 | 12.3 ms | **2.06 ms** | 21–41 ms | 268 |
| 20 | 10.6 ms | **0.62 ms** | 6–12 ms | 268 |
| **24** | 8.3 ms | **0.47 ms** | **5–9 ms** | 268 |
| 32 | 6.0 ms | **0.44 ms** | 4–9 ms | 237 |
| 24, 3-card hand | 5.6 ms | **0.66 ms** | 7–13 ms | 365 |

**The gate the brief set — under 1 ms at -O2 on a 24-producer board — is met at 0.47 ms**, and
that number is an upper bound: it was taken on the probe build, which still carried the
instrumentation counters (one of them inside the innermost assignment loop). The 14/16-source
boards remain the worst case at ~2 ms (their leftovers are narrow enough that the assignment
enumeration still runs instead of short-cutting); that is a 7–8× improvement on the shape the
owner actually reported, not a solved problem, and it is named here rather than rounded away.

## 4. Suite pin

`bin/Res/test/w54q_autotap_options_16_sources.txt` (new, registered in `_tests.txt`) —
fourteen Forests and two Plains (**16** candidate producers, the owner's count), Grizzly Bears
`{1}{G}` to pay, White Knight `{W}{W}` in hand. The baseline plan spends a Forest for the pip
and an off-pip source for the generic — a Plains — stranding the Knight on one white source.

- **RED on the archived pre-lane binary** `archives/wagic-ef133ea2a-w54base`:
  `picks: Forest Plains`, test failed.
- **RED on the shipped binary with the disable flag** `WAGIC_OPT_SOURCES_CAP=14`:
  `picks: Forest Plains`, test failed.
- **GREEN after:** `picks: Forest Forest`, pool `{G}{G}`, both Plains up.

The three existing option-preserving fixtures (`autotap_options_keeps_castable_card`,
`autotap_options_keeps_ability`, `autotap_options_unaffordable_utility_spends`) and the whole
24-file `autotap`/`oracle_` family are green at the shipped settings.

## 5. Gate

- Suite, single-threaded (`WAGIC_TESTSUITE_THREADS=1`, ground truth):
  **1227 tests, 0 failed, 51 AI tests, 0 failed, 0 timeouts** — baseline 1226 + this lane's
  one fixture. Both `==Test Failed !==` and `==Test timed out` counted: 0 and 0.
- PARSETEST: **2489 passed, 0 failed** — exactly the baseline; no parse-relevant string changed.
- Every run under `systemd-run --user ... MemoryMax=4G MemorySwapMax=0`.

## 6. Falsifiable predictions

1. On a Vita build from this commit, a hand-focus change on a 10–16 producer board costs about
   **a fifth to an eighth** of what it costs on vpk11-era code — and the owner's reported
   "almost frozen" hand navigation should get better on the boards *below* 16 producers,
   where it was previously worst. If his report says the freeze is unchanged, the cost centre
   is not this function (see §7's first bullet — the card-art path is the standing alternative).
2. His late boards (16+ producers) will now show option-preserving taps where they previously
   showed plain planPayment order — a **visible behaviour change** on exactly the boards where
   the spec used to be dead. If a tap looks *worse* to him on a wide board, that is the
   work-budget truncation (b), not the reach change (c), and `WAGIC_OPT_WORK_BUDGET` is the
   dial that tests it.
3. No fixture in the `autotap`/`oracle_` family changes verdict at any
   `WAGIC_OPT_SOURCES_CAP` between 14 and 32 — their boards are all far under 14 sources.

## 7. What I did NOT verify

- **Nothing was measured on the Vita, or on any console.** Every number here is desktop
  (Voyager, i7-7700HQ). The **×10–20 is the audit's fixed factor applied to a desktop -O2
  measurement, not a measurement** — treat every "Vita estimate" column as arithmetic, not
  evidence. The owner's next session on a build from this commit is the test.
- **I did not re-profile the whole hand refresh.** Lane S's 0.44 ms per-tick / 1.29 ms
  per-focus-change figures were taken with `refineForOptions` *bailing*; on a board where it
  engages, the per-focus-change refresh is that plus the numbers in §2/§3. I did not re-run
  lane S's `benchhand` to state the new combined refresh figure.
- **The final -O2 numbers were taken on the probe build**, which carried the instrumentation
  counters (including one increment inside the innermost loop). The shipped code is the same
  code minus those increments, so the shipped cost is at or below the table — but I did not
  rebuild -O2 after stripping them to confirm the exact figure.
- **The boards are constructed, not the owner's.** They are a realistic mix at the right
  candidate counts, not a replay of his transcript. Meteorite tokens are again stood in for by
  Birds of Paradise (lane S's substitution).
- **The work-budget cut (512 → 266 at 15 options) is not covered by any fixture.** Every
  existing autotap fixture and the new one sit on boards with far fewer than 266
  configurations, so the suite cannot see that change at all. It is measured, reasoned and
  flagged — not pinned.
- **`fastDiff`/`fastPay` are a hand transcription of `ManaCost::Diff` and `ManaCost::pay`.**
  I verified equivalence *indirectly*: the search's counters and the `changed=` verdict were
  bit-identical on all six probe boards, and the 24-fixture autotap/oracle family plus the
  full suite stayed green. I did **not** ship a differential harness that runs both paths on
  every call and asserts agreement — a cost with hybrid symbols is routed to the old path by
  `payVecModels`, so the untested surface is a hybrid-cost card in hand while a wide board
  auto-taps, and I did not construct that case.
- No corpus, no self-play game, no GPT seat: this function is HUMAN-seat only
  (`preserveOptions` is false for the AI seat's forecasts), so a corpus could not exercise it.
