# Wave-54 lane H — audit lane B: per-tick ability machinery (perf)

Branch `w54-lane-H`, worktree `worktrees/lanes/w54-H`, seeded from master b6205f88b (lane G's
ownership repair underneath). Items: A6 (a-e), A26, L15, L16, plus L13's `findMatchingAmount`
shipped FIRST in its own commit as the plan required. Every code comment is tagged `#W54-H (item)`.

## Commits (in order)

| commit | what |
|---|---|
| `4337061f9` | L13: `AAsLongAs::findMatchingAmount` sums every zone (was: the LAST zone only); the aslongas bound parser reads `<=N`/`>=N` (was: `atoi("=N")` = 0). Pins `isleback_spawn_compare_library` (+ `_neg`). |
| `462c63982` | A6(b) epoch gate, A6(c) single `updateTargets` per `AAsLongAs` tick + vector scratch, A6(d) `testDestroy` short-circuits the stack scan, A6(a) cheap half (stack `WParsedInt` for `limit` / `ThisVariable`), A26 erase fix, L15 Affinity memo, L16 dead branch. Disable flag `WAGIC_W54H_LEGACY=1`. |
| `d603b5b88` | A6(d) second half: `testDestroy` reads the two battlefield zones inline instead of the three-call `isInPlay` chain, per source and target. Exact rewrite. |

Archived binaries: `archives/wagic-462c63982`, `archives/wagic-d603b5b88`.

## Gate numbers (single-threaded, `WAGIC_TESTSUITE_THREADS=1`, `WAGIC_FASTCLOCK=0.1`, MemoryMax=4G)

| binary | suite | AI | timeouts | PARSETEST |
|---|---|---|---|---|
| seeded b6205f88b (baseline, this worktree) | 1224 / 0 failed | 51 / 0 | 0 | 2469 / 0 |
| `4337061f9` (L13 alone) | 1226 / 0 (1224 + 2 pins) | 51 / 0 | 0 | 2469 / 0 |
| `462c63982` (perf) | 1226 / 0 | 51 / 0 | 0 | 2469 / 0 |
| `d603b5b88` (perf, final) | 1226 / 0 | 51 / 0 | 0 | 2469 / 0 |

Verdicts identical at every step: no existing fixture changed state, and the two new pins are
RED on the seeded binary (positive: `expected 8/16 got 4/8`) and GREEN from `4337061f9` on. The
threaded suite's known concurrency-only pair (lifeline / merrow_reejerey) does not appear
single-threaded, as in every lane before.

## Perf — headless suite, `perf record -F 997 -g`, -O0 debug build (the audit's rig)

Inclusive shares of suite CPU (`perf report --children --no-inline`). "before" = the seeded
binary on the SAME registry (1226 fixtures, so the positive Isleback pin fails there — 1 red,
otherwise the baseline run); "after" = `d603b5b88`.

| symbol | before | after `462c63982` | after `d603b5b88` (final) |
|---|---|---|---|
| `GameObserver::Update` | 58.79% | 47.86% | 43.20% |
| `ActionLayer::Update` | 37.83% | 21.36% | 18.62% |
| `ListMaintainerAbility::updateTargets` | **17.73%** | 4.75% | **4.83%** |
| `MTGAbility::testDestroy` | **8.98%** | 4.00% | **3.43%** |
| `ActionStack::has(MTGAbility*)` | 1.36% | below 0.01% (absent from the report) | absent |
| `GenericTriggeredAbility::Update` | 8.14% | 7.38% | 6.70% |
| `__dynamic_cast` | 8.02% | 4.14% | 3.96% |
| `AbilityFactory::parseCastRestrictions` | 4.42% | 5.45% | 6.11% (same absolute samples; the pie shrank) |
| `WParsedInt::init` | 2.36% | 2.36% | 2.48% |
| `GameObserver::Affinity` | 0.82% | 0.49% | 0.45% |
| `AThis::resolve` / `ThisVariable::match` | 0.62% / 0.61% | 0.07% / 0.06% | 0.09% / 0.08% |
| `MTGAbility::conditionEpochDue` (new) | - | 0.08% | 0.04% |

Call counts (throwaway counters compiled in for the measurement only, removed before every
commit — `~/.gatelogs/w54H/apply_cnt.py`, logs `cnt-before.log` / `cnt-after.log`), whole
single-threaded suite of 1226 fixtures:

| counter | before (`4337061f9` + counters) | after (`462c63982` + counters) |
|---|---|---|
| `ListMaintainerAbility::updateTargets` calls | **1,237,054** | **253,503** (-79.5%) |
| `MTGAbility::testDestroy` calls | 15,297,798 | 15,297,779 (unchanged by design - the sweep is not gated) |
| `ActionStack::has(MTGAbility*)` calls (from testDestroy) | 15,139,211 | **1,140** |
| `AThis::resolve` evaluations | (not counted) | 1,152 |

Where the residual epoch bumps come from (a second throwaway build, `apply_sites.py`, log
`cnt-sites.log`): action-layer add 134,709 / remove 139,483, events 73,972 (top: EmptyManaPool
16,580, PhasePreChange 14,732, PhaseChange 10,270, ZoneChange 10,255, CreatureAttacker 3,088,
ConsumeMana 3,066, CombatStepChange 2,950), stack push 4,785 / resolve 5,261, stuffHappened
2,259, phase-change fallback 1,272, type changes 855. The 253K residual is (list maintainers
alive) x (ticks in which any of those fired) — the suite is action-dense (a scripted command
every ~40 ticks, phases auto-advancing), so one tick in five still carries a change. The
plan's "< 100,000" was not reached on this suite; it is not reachable by coalescing, only by
narrowing which changes count, which is exactly the risk the plan names.

Perf sample totals fell 69,507 -> 55,782 -> 33,779 across the three runs (task-clock samples of
the wagic process at 997 Hz); the machine was shared with other lanes' builds during the
first two, so treat the shares as the claim and the totals as indicative.

## -O2 offscreen frame profile (lane W's rig: `Makefile.sdlO2` + `objs-o2`, `WAGIC_FASTCLOCK=0.0333`, SDL offscreen driver, deck164 vs deck164 Baka one-shots, throwaway `WAGIC_FRAMEPROF` profiler in DuelLayers.cpp, applied and reverted, never committed)

Three one-shot games per binary, back to back on a quiet machine (load ~1.3), `U 11ActionLayer`
= `ActionLayer::Update` per tick, `UPDATE total` = the whole `DuelLayers::Update` (incl. the
AI's `Act`). "before" = an export of commit `4337061f9` (master b6205f88b + L13, i.e. the
engine before any perf change) built at -O2; "after" = `d603b5b88` at -O2.

| game | ticks | avg perms | `ActionLayer::Update` ms/tick | UPDATE total ms/tick | `[AI Act]` | update ms/tick at 16-19 perms | at 20-23 perms |
|---|---|---|---|---|---|---|---|
| before 1 | 742 | 11.6 | **0.0949** | 0.1974 | 0.0756 | 0.2197 (n=302) | 0.2135 (n=10) |
| before 2 | 613 | 10.0 | **0.0877** | 0.1807 | 0.0694 | 0.2165 (n=123) | - |
| before 3 | 663 | 10.5 | **0.0919** | 0.2000 | 0.0807 | 0.2313 (n=151) | 0.3591 (n=36) |
| after 1 | 902 | 12.2 | **0.0545** | 0.1580 | 0.0774 | 0.1836 (n=280) | 0.1797 (n=106) |
| after 2 | 827 | 12.0 | **0.0620** | 0.1739 | 0.0811 | 0.2138 (n=178) | 0.2372 (n=119) |
| after 3 | 869 | 12.4 | **0.0534** | 0.1544 | 0.0766 | 0.1816 (n=123) | 0.1723 (n=163) |

`ActionLayer::Update`: **0.092 -> 0.057 ms/tick** (mean of three, -38%) on boards averaging
10-12 permanents (max 20-25); its share of Update fell from 46-48% to 35%. The whole Update
tick went 0.19 -> 0.16 ms. Render is untouched (0.44-0.54 both sides; it scales with the
board, which the after games grew wider).

Calibration against lane W: lane W measured `ActionLayer::Update` at 0.26 ms/frame on
2026-09-02; today's BEFORE binary reads 0.09 on the same rig and deck. Two things changed
under it: lane W's own run shared the machine with a corpus (it quotes ~2.5x inflation for
its A/B rows), and lane G's ownership repair (orphaned clones no longer accumulate in the
layer) landed in between. So the plan's "0.26 -> <= 0.10" was already met by the seeded
binary; this lane's claim is the paired delta on the same day, same rig: -38% on the layer
tick. Bucket rows are whole-Update numbers (the profiler buckets only the totals); the
per-layer figure is the game-wide average.

## What shipped (by item)

**A6(b) — epoch gate.** `GameObserver::mAbilityEpoch` is a change counter; `bumpAbilityEpoch()`
is called from `receiveEvent` for every event except the per-tick `WEventGameStateBasedChecked`
marker, from `addObserver` and `ActionLayer::removeFromGame` (every ability add/remove), from
`ActionStack::addAction` and `::resolve`, from `GameObserver::Update` on a phase change, from
`ActionLayer::Update` when `stuffHappened` was raised (clicks, harness pokes), and from the
three sites that raise `mPropertiesChangedSinceLastUpdate` (`addType`, `removeType`, the
copy/transform path in AllAbilities.cpp). `MTGAbility::conditionEpochDue()` records the epoch
BEFORE the evaluation (a change the evaluation itself causes is seen next tick) and every
ability starts at 0 (a clone too), so the first tick always evaluates.
`ListMaintainerAbility::Update`, `AAsLongAs::Update`, `AThis::Update`, `AThisForEach::Update`
consult it. NOT gated: the `testDestroy` sweep (an ability that sets `forceDestroy` on itself
without any bump would otherwise be Updated again before removal — a re-fire hazard; the
sweep stays per-tick and was made cheap instead), `TriggeredAbility::Update` (phase triggers
poll `currentPhase != newPhase`; untouched), the click predicates.

**A6(c).** `AAsLongAs::Update` runs `updateTargets` once (SorterFunction's); its `added()` /
`removed()` are `return 1`, so the dropped first call only re-filled the list the second then
found unchanged — exact. `updateTargets`' two scratch `map<MTGCardInstance*,bool>` are
vectors; removals are collected walking `cards` (already pointer-ordered), additions are
`std::sort`ed, so `added()`/`removed()` fire in the same order as before (matters for lords
whose clones land in the action layer in that order).

**A6(d).** `testDestroy`: the zone tests run first; the linear `ActionStack::has` scan runs
only when they would return 1 (the scan can only turn a 1 into a 0) — 15.1M scans -> 1,140.
`target == source` skips the `dynamic_cast<Player*>`. Second commit: the source/target
battlefield tests compare `currentZone` against the two `inPlay` zones directly (what
`hasCard` does three calls down). Both exact.

**A6(a), the cheap half.** `ActivatedAbility::isReactingToClick` and `ThisVariable::match`
evaluate their `WParsedInt` on the stack (no NEW/delete per poll). NOT done: compiling
`restriction{}` / `this(...)` conditions to a struct, and a `limit` parsed once — every
`limit:` in the corpus is a literal except `hasabilitytwoboastplus1plusend` (37) and one
`type:swamp[snow]:mybattlefield`, so a per-instance cache would need a purity test per form;
with the epoch gate the `AThis` restriction evaluations dropped to ~1.1K per suite and the
remaining `parseCastRestrictions` samples are the click predicates and
`TriggerAtPhase::trigger`'s per-tick restriction check (~2,000 samples, unchanged in absolute
terms). That is the next target if A6(a) is pursued: memoise per (ability, epoch).

**A6(e).** NOT done. The `WParsedPT` at the P/T parse site is already cheap (its ctor is one
`find("/")` before anything else; the audit's "hoist" saves an allocation, not a parse). The
AI dry-mode `abilityEfficiency` cache per `(card model id, mode)` was not attempted: its
purity per branch was not established and it is output-affecting for the heuristic AI —
a lane of its own with a corpus gate, not a perf patch.

**A26.** The twelve `s.erase(pos, pos + len)` calls in `WParsedInt::init` are `erase(pos, len)`
(the four `other*` forms keep their deliberate 5-char prefix strip). This changes output only
for a rounding keyword that is neither leading nor trailing in its expression; PARSETEST and
the suite did not move. The compiled `WParsedIntExpr` was NOT built — `init` evaluates as it
parses (there is no parse/eval split to cache), so it is the same project as A6(a)'s compiled
conditions.

**L15.** `AffinityNeedsUpdate` is re-armed by the epoch bump (so by every real event, every
ability add/remove, stack change, phase change, click) instead of by every event including
the per-tick `WEventGameStateBasedChecked`; inside `Affinity()` the memo is tested before the
`cardsAbilities` `dynamic_cast` walk and the 20 keyword reads (pure reads — exact reorder).
The per-card extra-cost shadow reset and the Trinisphere block still run every tick.

**L16.** The `dynamic_cast<AManaProducer*>` branch in `ActionStack::garbageCollect` is gone.

**L13.** See commit 1. Isleback Spawn is the only shipped `compare` user; without the `<=`
parse fix the amount fix alone changes nothing observable (maxi stayed 0), so both went into
the same attributable commit with a RED-on-base pin.

## Disable flag

`WAGIC_W54H_LEGACY=1` (read once) restores: per-tick polling (every `conditionEpochDue()`
answers true), `AAsLongAs`' double `updateTargets`, and the every-event Affinity re-arm.
Everything else in the lane is an exact rewrite (same result by construction) and has no
switch: the `testDestroy` reorder/inline, the vector scratch, the stack `WParsedInt`s, the
erase fix, L16, L13.

## Predictions (falsifiable)

1. `updateTargets` share and calls fall by more than 4x on the same suite (met: 17.7% -> 4.8%,
   1.24M -> 254K).
2. `ActionStack::has` disappears from the profile (met: 15.1M -> 1.1K calls).
3. Suite verdicts identical at every commit (met: 1226/0, 51/0, 0 timeouts x3; PARSETEST 2469/0).
4. The `WAGIC_W54H_LEGACY=1` binary reproduces the "before" call counts — NOT run (see below).
5. On the Vita, `avg_upd` on lane W's per-turn telemetry line drops on wide boards during the
   HUMAN's turn (where `ActionLayer::Update` is most of Update-minus-Act); the AI's-turn number
   stays dominated by `Act`. Unverified (no device).

## What was NOT verified

- **The gate's completeness.** The epoch bumps at every state change the engine ANNOUNCES
  (events, layer add/remove, stack, phase, clicks, type changes). A card that mutates a field a
  `this(...)` / lord condition reads WITHOUT any of those (a direct `power =` / `life =` in an
  effect that emits no event and adds no ability) would be seen at the next bump instead of
  the next tick — bounded by the next phase change, never lost. The suite (1226 fixtures,
  identical verdicts) is the evidence that no tested card does this; untested cards are not
  covered. This is the risk the plan named and the flag exists for.
- `WAGIC_W54H_LEGACY=1` was not gated (the default-off path is the gated one); it compiles and
  is read once.
- The threaded suite (`THREADS=16`) was not run.
- No selfplay corpus or live GUI/Vita run; the -O2 frame profile is desktop, offscreen, Baka
  vs Baka, on the same Nazgul deck lane W used — not the owner's boards.
- `perf` shares are from -O0; ratios, not Vita milliseconds.
- The two `isInPlay` inlines assume `players[i]->game` non-NULL, exactly as `isInPlay` did.
- The L15 memo is only as good as the bump set (same caveat as the gate); a cost change with
  no bump would be recomputed at the next bump. `Affinity()`'s early rows (extra-cost shadows,
  Trinisphere) were deliberately left per-tick.
