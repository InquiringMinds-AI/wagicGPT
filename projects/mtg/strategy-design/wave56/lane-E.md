# Lane E — replay determinism (O8 / audit A35 + the equip auto-tap replay blocker)

Owner ruling O8 (2026-09-03): **"yes"** to a second RNG stream for the heuristic AI,
with the two accepted costs stated up front (pre-change transcripts stop replaying;
AI-draw-dependent seeded fixtures get re-baselined).

## 1. What shipped

### (a) A second, seeded RNG stream for the AI (`#W56-E`)

`GameObserver::aiRandomGenerator` — a `RandomGenerator` seeded from the game seed
(`aiSeedFrom(seed) = seed ^ 0x9E3779B9`, re-applied by `resetSeed()` and by the
`seed` directive in `load()`), reached through `getAIRandomGenerator()`. The class
comment in `utils.h` has always said this was the intent ("to be able to control the
AI randoms independently of the other game randoms"); it was never built.

Every AI-side draw now goes there:

- `AIPlayer::getRandomGenerator()` is no longer an accessor for the per-seat member —
  it returns `observer->getAIRandomGenerator()` (the member survives only as the
  fallback for an observer-less construction path). That member was **time+address
  seeded and never logged**, so it was neither deterministic nor recordable; one
  accessor change puts `AIPlayer::clickMultiTarget/clickSingleTarget/clickMultiAct`,
  every `owner->getRandomGenerator()` efficiency roll in `AIPlayerBaka`, and
  `AIMomirPlayer` on the one seeded AI stream.
- The three AI sites that drew from the **game** generator are re-pointed:
  `AIPlayerBaka.cpp` `getEfficiency`/`getRevealedEfficiency` (`... % 30` noise, both
  copies) and the random-avatar pick in `AIPlayerBaka::loadAvatar`.
- The member-direct draws (`randomGenerator.random()` in `AIPlayerBaka`'s 95 % gate,
  the two `chance` gates, `randomChance`, `AIPlayer`'s shuffle/pick and the `[AI]`
  fixture `rand values` loader) all route through the accessor.

**Disable flag**: `WAGIC_SINGLE_RNG=1` makes `getAIRandomGenerator()` return the game
generator — i.e. the exact pre-change single shared stream.

### (b) The four libc `rand()` sites (A35)

| site | before | after |
|---|---|---|
| `Rules.cpp:722` (random commander) | `std::srand(time(NULL))` **+** `std::rand()` — a *reseed* of the process-global stream on every commander load | `observer->getRandomGenerator()->random()` (falls back to `std::rand()` with no observer) |
| `WParsedInt.cpp:1144` (`cursedscrollresult`) | `std::rand()` | `card->getObserver()->getRandomGenerator()->random()`, same fallback shape as the `genrand` site above it |
| `TargetChooser.cpp:1255` (dummy subtype id) | `rand()` | `card->getObserver()->getRandomGenerator()->random()` |
| `GameStateDuel.cpp:3461` (`Tournament::leaveOutAIvsAIMatches` coin flip) | `rand()` | a file-static `RandomGenerator sTournamentRandom` |

The last one is a **deviation from the docket wording** and it is deliberate: a
`Tournament` has no `GameObserver`, and a bookkeeping coin flip for filler AI-vs-AI
matches must not consume a value out of the stream a transcript records either. It
gets its own generator, which is what "not the process-global libc stream" buys.

### (c) `WRand` deleted

`utils.h`/`utils.cpp`. A35 called it callerless; it was not — `AIPlayer.cpp:233`
(random AI deck pick) and three `Credits.cpp` sites called it. `WRand(bool)` was
literally `return rand();`, so the four call sites became `std::rand()` — a pure
rename, no behaviour change, and the misleading wrapper is gone.

### (d) The equip auto-tap replay blocker (wave-53 lane AA's open item)

The mechanism, read off both 2026-09-02 vpk12 dumps that carry a Bloodforged
Battle-Axe: **during a replay the engine acts on its own.** Equipping auto-taps the
payment and `logAction`s clicks the record never contained. Two failures come out of
that one fact:

1. the unrecorded entries land in `actionsList`, so the loader's
   `dumpAssert(cmdIndex == actionsList.size()-1)` invariant blows and the replay
   **aborts with a core dump** (this is what actually happened on both dumps — not
   the "refused 60x" shape lane AA described);
2. when the engine pays *first*, the recorded payment click is then refused for
   every one of the 60 retries (lane AA's shape).

Fix, both directions, `WAGIC_REPLAY`-gated only:

- `GameObserver::logAction` — under `WAGIC_REPLAY`, an action that does not match the
  expected record entry is **engine-generated**: park it in a new
  `mReplayEngineActions` list and keep it OUT of `actionsList`, so the index
  invariant holds.
- `GameObserver::processActions` — before the retry loop, a recorded action whose
  `transcriptActionKey` matches a parked entry is **consumed as already performed**
  (traced `REPLAY: '...' was already performed by the engine - consumed`) instead of
  being re-issued 60 times.

Inert outside a replay: `undo()` and the test suite take the old path (the `else`
arm is only reached when `getenv("WAGIC_REPLAY")` is set).

## 2. Replay evidence — action index reached, before / after

Before = `archives/wagic-b228b8648-w55step1` (the lane base binary), after = this
lane's build. Same command line in both arms
(`WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_SELFPLAY=1 WAGIC_AI=baka WAGIC_REPLAY=<dump>`,
under `MemoryMax=4G`).

| dump | actions | BEFORE | AFTER |
|---|---|---|---|
| vpk12b `...-1788393737-player_deck5-vs-ai_baka_deck33` (lane AA game 2) | 147 | **ABORT** — `dumpAssert` core dump inside the axe equip | **COMPLETE 147/147**, ends `turn 10 phase 1 life 16/23` = the dump header's `turn=10 life=16/23`, byte-exact |
| vpk12 `...-1788381501-player_deck5-vs-ai_baka_deck95` (lane W #2) | 314 | **ABORT** — `dumpAssert` core dump, same axe | **295/314**, `turn 14 phase 4 life 13/10` |
| vpk12 `...-1788378791-player_deck3-vs-ai_baka_deck164` (lane W #1) | 311 | diverged at **94** (turn 5) | diverged at **121** (turn 6) |
| vpk12b `...-1788390621-player_deck4-vs-ai_baka_deck54` (lane AA game 1) | 141 | COMPLETE (turn 10 phase 1, life 15/9) | **unchanged**, COMPLETE — no regression |

**Attribution is clean**: re-running all three failing dumps with
`WAGIC_SINGLE_RNG=1` gives byte-identical results to the flag-off run. Every replay
gain above is the (d) equip fix; the RNG split neither helps nor hurts these dumps,
because AI seats never `Act()` while `isLoading` and so consumed nothing from the
game stream during a replay in the first place.

## 3. Re-baselined fixtures: **NONE** (the accepted cost did not materialise)

Measured, not assumed. Single-threaded suite + PARSETEST run on the **base binary**
and on this build, same worktree, same Res:

| | base `wagic-b228b8648-w55step1` | this lane |
|---|---|---|
| suite (`THREADS=1`, ground truth) | 1229 tests, **0 failed**, 51 AI / 0, 0 timeouts | 1229 tests, **0 failed**, 51 AI / 0, 0 timeouts |
| PARSETEST | 2636 passed, 0 failed | 2636 passed, 0 failed |

Not one fixture changed verdict, so there is nothing to justify as rules-correct.
Why the predicted re-baseline did not happen: the seeded fixtures' AI draws already
came off `AIPlayer`'s own per-seat generator (which this lane only re-homed and
re-seeded), and the three game-stream AI sites this lane moved
(`getEfficiency` `%30` noise x2, the random-avatar pick) are not reached by any
fixture's assertion path. The docket's cost was correctly stated as a *risk*; the
measurement says it did not bite.

## 4. What I did NOT verify

- **A35's stated verification is a negative control with no discriminating power, and
  I am reporting it as such.** "Replay a Baka-vs-Baka transcript with one seat swapped
  to GPT — shuffles identical" was run (`tools/gpt-stub-server.py --port 8299`,
  isolated `HOME`, `WAGIC_AI=gpt`): the GPT-seat replay is byte-identical to the
  Baka-seat replay — **and so is the `WAGIC_SINGLE_RNG=1` run**. It passes on the base
  behaviour too, because AI seats are passive during a replay. The property the split
  actually buys (an AI-side draw cannot advance the recorded game stream) is verified
  **statically** — no `AIPlayer*`/`AIHints` path reaches `getRandomGenerator()` on a
  `GameObserver` any more — not dynamically. A dynamic test needs a seed-pinned LIVE
  game with a swapped seat, and there is no seed override for `WAGIC_SELFPLAY`; I did
  not add one.
- **Baka-vs-Baka self-replay is NOT whole, on either build.** A 6-matchup A/B
  (`~/.gatelogs/w56E/bvbab.tsv`, each binary replaying its OWN fresh transcript)
  diverged in 6/6 on base (actions 3, 3, 49, 90, 142, 169) and 6/6 after (3, 21, 73,
  115, 147, 246). Different seeds per game, so this is descriptive, not a powered
  comparison — but it refutes the standing "a 22-turn Baka-vs-Baka game replays to the
  identical end state" claim in the wagicgpt skill, which did not reproduce on the
  base binary either. The residual shape I looked at: a recorded mana click inside an
  interrupt window comes back `0<name>` (refused) and never becomes `1<name>`. That is
  a THIRD replay defect, separate from this lane's two, and it is what now bounds
  human-vs-Baka replay. It should be docketed.
- **No suite pin for either item.** The replay loader is `getenv("WAGIC_REPLAY")`-gated
  and unreachable from a fixture by construction (the same argument wave-53 lane T made
  for its name-resolution fix, and for the same reason). The RNG split's observable is
  the absence of cross-stream coupling, which the suite cannot see; the before/after
  gate above is the regression evidence and the transcript table is the positive
  evidence. Neither is a fixture and I am not claiming one.
- **No Vita/PSP build**; port builds are owner-initiated. Nothing here is
  platform-specific, but the console is the test.
- **No selfplay corpus shakedown.** The AI's roll sequence changes (a different, now
  deterministic, seed) so heuristic play differs game to game from the base; I did not
  measure whether Baka's win rate or decision mix moved. Suite + PARSETEST only.
- `Tournament`'s own generator is time-seeded, so tournament filler results are not
  reproducible run to run. They never were, and nothing records them.
- The `[AI]` fixture `rand values` directive remains **inert** (the per-seat generator
  is constructed with `doLog=false`, and `RandomGenerator::random()` only consumes
  loaded values when `log` is true). That was true before this lane and is still true
  after it; re-homing the accessor did not change it, and fixing it was not in scope.

## 5. Falsifiable predictions

1. Any future device transcript containing an equip (or any other auto-tapped
   activation cost) replays past it; the previous outcome was an abort, so a single
   dump settles this.
2. A transcript recorded by a build carrying this change replays identically whether
   its seats are re-created as Baka or as GPT — because the recorded `rvalues:` no
   longer interleave AI draws.
3. `WAGIC_SINGLE_RNG=1` reproduces the pre-change stream exactly: any regression
   attributed to this lane must disappear under the flag, and if it does not, this
   lane is not the cause.
