# Lane S — Vita performance report (vpk11: "almost frozen when attempting to select cards in my hand")

Board reproduced from the owner's transcript
(`psp-work/logs/20260902-vpk11/transcript-1788330366-player_deck6-vs-ai_baka_deck109.txt`,
deck6 "Natural Selection" R/G, won turn 16): 19 permanents — 6 Mountain, 5 Forest,
Karn's Bastion (`{T}:Add{C}` + `{4}{T}:_PROLIFERATE_`, a utility land), Draconic Disciple
(five `{T}:Add<colour>` abilities + `{7}{T}{S}`), two Birds of Paradise standing in for the
two Meteorite tokens (five any-colour mana abilities each — the same shape Roxanne's lord
grants an artifact token), Bloodforged Battle-Axe, Boartusk Liege, Scab-Clan Mauler,
Swarm Shambler — against a 9-card hand (Shivan Dragon focused). Memlog on device was flat,
so this was CPU, not memory.

## How it was measured

A throwaway suite command `benchhand <card> <reps>` ran the exact body of
`GuiHandSelf::Update`'s 0.25 s refresh — `castableForDisplay`, the three per-permanent
availability oracles, and `selectAutoTapProducers` with and without option preservation —
300 times on that board, timing each leg with `clock_gettime(CLOCK_MONOTONIC)`.
`perf record` was also taken but is useless here: card-DB parsing dominates a suite process's
profile, so the direct per-leg timers are the instrument. **The bench command and the probe
fixture were REMOVED before commit** — nothing diagnostic ships (owner rule: no diagnostics in
release builds). `perf` and the timers can be reconstructed from this file if needed.

Each candidate was isolated with a temporary env kill-switch inside the suspect function, so
every number below is a paired A/B on ONE binary, same board, same run conditions.

## Measured cost centre (per refresh, desktop, 19 permanents / 9-card hand)

| leg | ms | share |
|---|---|---|
| `hasUsableAbility` × 19 battlefield cards | 1.49 | 57 % |
| `planPayment`'s utility ordering (`sourceHasNonManaTapAbility`) | 0.50 | 19 % |
| rest of `planPayment` | 0.43 | 16 % |
| `castableForDisplay` (all 9 hand cards) | 0.15 | 6 % |
| `refineForOptions` | 0.08 | 3 % |
| `canDeclareAttacker` + `canDeclareBlocker` × 19 | 0.002 | ~0 % |

Two findings overturn the recorded suspects:

1. **The wave-52 memo fixed the cheap part.** `mPreviewSig` guards only
   `selectAutoTapProducers`. The two legs that run on EVERY tick regardless of focus —
   `castableForDisplay` + the battlefield oracles, 1.64 ms — were never memoised, and the
   battlefield oracle alone is 57 % of the refresh.
2. **`refineForOptions` is a no-op on the owner's real board.** It bails at
   `sources.size() > kMaxOptSources` (14); his late board offers 16 candidate producers. The
   option-preserving autotap therefore never engaged in the game he reported — it cost him
   0.08 ms, not the freeze. (Raised for the docket, NOT changed here: raising the cap would
   make the owner's spec actually apply on late boards and needs its own cost measurement.)
3. `GuiCombat::reseatCursor` (the other recorded suspect) is discharged: it early-returns on
   `cursor_pos == NONE`, which is the state outside combat, and it is only reached from
   `shiftLeft`/`shiftRight`/`Update`'s combat path — not per `CheckUserInput` in the hand.

Why it feels like a freeze on the console: at the brief's ~20× factor the pre-fix refresh is
~57 ms of work per focus change and ~33 ms every 0.25 s while a card is merely focused, on top
of rendering — and if a frame ever costs more than 0.25 s, `dt` grows and the refresh fires
every frame, which is a positive feedback loop.

## The fix (both are algorithmic dedup — no verdict changes)

1. **`LegalActionsOracle::usableAbilityCards(Player*)`** (`LegalActions.cpp/.h`) — the
   `hasUsableActivatedAbility` loop gained a `collect` set: `return true` becomes "record this
   source and keep going". ONE action-layer walk and ONE `potentialManaPermissive` answer the
   question for the whole battlefield, where `GuiHandSelf::Update` previously asked per card
   (19 permissive potentials + 19 full walks of every ability object, 4×/s). There is still
   exactly ONE definition of "this ability is usable" — the batch is the same loop.
2. **`buildNonManaTapIndex` / `sourceHasNonManaTapAbility(..., idx)`** (`ManaEngine.cpp`) —
   `genericFillOrder` and `refineForOptions` each re-walked the whole action layer once per
   producer source to find its non-mana tap abilities, INSIDE every `planPayment`. They now
   build a card→abilities index in one walk and read it. The affordability pricing
   (`activationAffordableWithoutSource`'s strict `potentialMana`) is unchanged and still runs
   once per utility source, which after the index is only 0.12 ms of `planPayment`.

The owner's autotap spec is untouched: no ordering rule, no scoring term, no candidate filter
changed. `autotap_options_keeps_castable_card`, `autotap_options_keeps_ability` and
`autotap_options_unaffordable_utility_spends` are green in the gate below.

## Timings, before → after (median of 3 paired runs, reps=300)

| | before | after |
|---|---|---|
| battlefield ability oracle | 1.49 ms | 0.28 ms (5.3×) |
| utility ordering inside `planPayment` | 0.50 ms | 0.12 ms (4.2×) |
| **per-tick refresh** (focus held, static board) | **1.64 ms** | **0.44 ms (3.7×)** |
| **per-focus-change refresh** (hand navigation) | **2.84 ms** | **1.29 ms (2.2×)** |

At the brief's ~20× Vita factor: ~33 ms → ~9 ms per tick, ~57 ms → ~26 ms per focus change.

## Suite pin

`bin/Res/test/w53_usable_ability_batch_board.txt` (new, registered in `_tests.txt`): a mixed
board — Westvale Abbey `{5},{T}` and Karn's Bastion `{4},{T}` affordable over five other
lands, a vanilla Hill Giant and a Plains not — asserted through `assertusable`.
`assertusable` itself now cross-checks the per-card oracle against
`usableAbilityCards`, and fails the fixture on disagreement, so the eight existing
`assertusable` fixtures also pin the equivalence. The guard was proved non-silent: skipping
lands in the batch collector turned it RED with
`per-card oracle says 1 but the batch usableAbilityCards says 0`.

This fixture cannot be RED on the base commit — the batch API it pins does not exist there.
It is an equivalence pin, not a behaviour-change pin; the change is a pure perf refactor.

## Gate

Suite (single-threaded, ground truth): **1211 tests, 0 failed, 44 AI / 0 failed, 0 timeouts**
(baseline 1210 + this lane's 1 fixture; the two known concurrency-only failures do not appear
single-threaded). PARSETEST: **2121 passed / 0 failed** — exactly the baseline, no
parse-relevant string changed.

## Files

- `projects/mtg/src/LegalActions.cpp`, `projects/mtg/include/LegalActions.h`
- `projects/mtg/src/ManaEngine.cpp`
- `projects/mtg/src/GuiHand.cpp`
- `projects/mtg/src/TestSuiteAI.cpp` (assertusable cross-check)
- `projects/mtg/bin/Res/test/w53_usable_ability_batch_board.txt`, `bin/Res/test/_tests.txt`

## What I did NOT verify

- **Nothing was measured on the Vita.** I have no console here; every number is desktop, and
  the ~20× scaling is the brief's estimate, not a measurement. Whether the owner still feels a
  freeze is decided by his next session on a VPK built from this commit.
- The rendering side of "selecting cards in my hand" is unmeasured. `CardSelector` re-renders
  the focused card large, which on the Vita can pull a card texture through `WCache`/`LoadPNG`
  — the same path as the vpk7-era `std::bad_alloc` crash cluster. If per-card art loading is
  what stalls him, none of this lane's work touches it.
- No frame-level profile of the whole `DuelLayers::Update`/`Render` tick: I timed the engine
  work `GuiHandSelf::Update` performs, not the frame it sits in, so I cannot say what share of
  a Vita frame it was.
- The board is a reconstruction: Meteorite tokens are stood in for by Birds of Paradise (same
  ability shape, five single-colour mana abilities), and his exact hand contents are inferred
  from the transcript's castable set, not copied card-for-card.
- I did not re-run the suite under `WAGIC_TESTSUITE_THREADS=16`, so I cannot say whether the
  known lifeline/merrow_reejerey concurrency pair still churns at the usual rate.
