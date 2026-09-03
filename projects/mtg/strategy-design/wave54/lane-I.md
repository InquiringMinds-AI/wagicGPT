# Lane I (audit Lane C) — engine-core memory: ManaCost, zones, instances

Branch `w54-lane-I`, worktree `worktrees/lanes/w54-I`, seeded from master dab9f72f4.
Items from `audit-20260902/09-synthesis.md` §3 Lane C: A7, A11, A12 (engine sites),
A13, A15(a), A27, A34, L17, L18. A15(b) untouched (next wave).

## What shipped (all tagged `#W54-I (An)` in code)

| item | file:region | change | disable flag |
|---|---|---|---|
| A7 | `ManaCost.cpp` pointer ctor + copy ctor | sub-costs (kicker/Retrace/BuyBack/alternative/FlashBack/morph/suspend/Bestow) are copied only when the source HAS them; absent sub-costs stay NULL (same shape `copy()` already produced). Copy ctor now also carries `isMulti`/`instantSpeed`/`alternativeName` (they were never initialised on a by-value copy). | `WAGIC_MANACOST_EAGER_SUBCOSTS=1` restores the eight eager empties |
| L17 | `ManaCost.cpp` `operator=` | deep copy over `copy()` (was a shallow copy of nine owned pointers that skipped the X slot); `manaUsedToCast` deep-copied | — (latent double-free removed; no caller assigns by value today) |
| A12 | `ManaCost.cpp` `canAfford` (anytypeofmana), `ManaEngine.cpp` `planPayment` (anytypeofmana), `MTGRules.cpp` sunburst `:367`, PayZero is/reactToClick, attack-cost is/reactToClick, block-cost is/reactToClick | `NEW ManaCost(ManaCost::parseManaCost("{0}",…))` → `NEW ManaCost()` (identical: `{0}` adds nothing, xColor -1 either way); sunburst site uses a stack `ManaCost emptyCost` (changeCostTo copies); `canAfford` frees the first `diff` it used to overwrite; PayZero `isReactingToClick` frees its probe cost when the rule did not retain an extraCosts list | — (pure leak removal) |
| A11 | `MTGGameZones.cpp` `beforeBeginPhase`, `~MTGPlayerCards` | `garbageLastTurn` no longer aliases `garbage`: the zone that was `garbage` becomes `garbageLastTurn` and lives one more turn; dtor frees both when distinct (init keeps them aliased until the first boundary — the `!=` guard covers it). Dead tokens (and their `previous` chains) are now freed TWO turn boundaries after death. | `WAGIC_GARBAGE_ONE_TURN=1` restores the old one-turn free |
| A13 | `MTGGameZones.cpp` `initDeck` | one `MTGCardInstance` per commander built once before the deck loop and freed after (was one per deck card per commander, never freed); PLUS the sibling leak found by LSan in the same branch — a deck card rejected by the colour-identity/singleton rule was never freed | — |
| A15(a) | `Player.cpp` `cleanupPhase` | end-of-turn `cleanup()` sweep extended from inPlay+graveyard to hand, library, exile, commandzone, reveal (reaps `previous` chains; `stillInUse()` guard unchanged) | `WAGIC_CLEANUP_BATTLEFIELD_ONLY=1` restores the two-zone sweep |
| A27 | `MTGGameZones.cpp` `operator<<(MTGPlayerCards)` | `exile=` now serialises `removedFromGame` (was the HAND) | — |
| A34 | `MTGRules.cpp` `MoveLegend` / `MovePW` | the per-event `myClone` (source for the share!name! chooser and the menu abilities; owned by nobody) is parked in the controller's `garbage` zone (the AACopier dummy idiom, AllAbilities.cpp:11637) and freed by the zone two boundaries later, after the one-shot abilities have been garbage-collected | — |
| L18 | `Player.cpp` ctor (`deckId = 0`), `DecisionContract.cpp:118` (`size()-1` underflow → `m-- > 1` walk, same indices), `GameObserver.cpp` imprint sweep (reverse walk; the forward erase skipped the next entry), `DamagerDamaged.cpp` (`buf[16]` + `snprintf`) | one-liners | — |

NOT changed: `GameObserver.h:281` `if(this)` (UB but the only NULL-observer flow —
`Rules.cpp:1082 Player(NULL,"","")` → `parseLine` → `loadAvatar` — already guards `observer`
first; removing the guard buys nothing in -O0 and GCC already folds it in release; left for the
header-owning lane). `ExtraCost.cpp` SnowCost/Offering parse-and-drop sites belong to lane
A12-abilities (`ExtraCost.cpp` is not in this lane's file list). `AIPlayerBaka.cpp:2671-2729`
(lane E region). `GameObserver.cpp:1747` is commented-out code.

Audit of everything that scans the garbage zones (A11 blast radius): `GameObserver::
validateCardPointer` (scans both — now genuinely two zones, the intended semantics);
`MTGTokensCleanup::receiveEvent` (tests `to == garbage` only; nothing ever moves INTO
`garbageLastTurn`); `AIPlayerGPT.cpp:11260` (narration: `to == garbage || to == garbageLastTurn`,
unchanged meaning); `AllAbilities.cpp:10911/11637` (add to `garbage` — now the live zone, as
before); `MTGPlayerCards::setOwner` sets both; `MTGGameZone::beforeBeginPhase` runs on `garbage`
(the fresh zone) as before. No reader depended on the two pointers being equal.

A7 blast radius (readers of copied costs): every `getKicker()/getAlternative()/getMorph()/…->`
deref in `src/` was checked — all guarded (`CardPrimitive.cpp:112-118`, `AllAbilities.cpp:5812-
5846` (sets its own sub-cost first), `GameObserver.cpp:1833-1872`, `LegalActions.cpp:64/164`,
`ExtraCost.cpp:1298`, `AIPlayerBaka.cpp:1819/3225/3462`, `AIPlayerGPT.cpp:17690/17987`,
`ManaEngine.cpp:1136`). `hasAnotherCost()` (`AIPlayerBaka.cpp:1819`) was TRUE for every pointer-
copied cost; it now answers truthfully — the [AI] fixtures (49/0) and the ASAN suite are the pin.

## Gate numbers (this worktree; every wagic run under the memory cap)

- Base re-confirm on the seeded binary: PARSETEST 2258/0; suite 1219 tests / 0 failed
  (THREADS=1 makes the lifeline+merrow pair green, as the brief predicts), 49 AI / 0.
- After (shipping binary, hermetic rebuild — the seeded objs were older than the checkout so
  make rebuilt everything): PARSETEST 2258/0 (no parse-relevant change; no cases added); suite
  **1221 tests / 0 failed** (registry +2: `generic/token_dies_two_turns_w54i.txt`,
  `generic/attackcost_blockcost_probe_w54i.txt`), **49 AI / 0**, 0 timeouts.
  (`~/.gatelogs/w54-I-gate3-*.log`; gate2 on the pre-A13-sibling binary read the same.)
- Single-fixture RSS (slice 6's sampler, `generic/gift_of_the_gargantuan_i1085.txt` alone,
  `ru_maxrss` of the child): **468,324 KB before → 133,600 KB after** (target < 250 MB: met).
- ASAN suite (bin-asan, MemoryMax=8G, THREADS=1): first run with `timeout 900` reached 307
  fixtures / 0 failed / **0 AddressSanitizer errors** before the cap (the laptop was carrying three
  other lanes' gates); second run with `timeout 3600` (binary = everything but the A13 sibling
  delete, which is commander-only): **1221 tests / 0 failed, 49 AI / 0, 0 AddressSanitizer
  errors**, LSan 1,044,672 B leaked in 4,891 allocations at exit (`~/.gatelogs/w54-I-asan-suite2.log`).
- LSan, attack-cost fixture (`_probe_I_attack` — Ghostly Prison + Archangel of Tithes + a bears
  click at attackers): base 6 `ManaCost::parseManaCost` frames (412,453 B / 3,055 allocs) → after
  **0** (411,877 B / 3,049). Snow legs (`drift_of_the_dead`, `arcums_weathervane`, two snowlandwalk
  fixtures): 0 parseManaCost frames on BOTH binaries — none of them pays an `{S}` cost, so
  `SnowCost::isPaymentSet` (ExtraCost.cpp, not this lane) is not exercised here.
- LSan, legend registry (`animated_legendaries1_i221`, `brothers_yamazaki_i1085`,
  `legend_second_copy_castable`, `basic_nolegend_…`): `MoveLegend/MovePW` frames base **5 → after
  0** (A34 verified leaked on base, gone after). Byte totals are NOT comparable across these two
  reports: the base run's report omits the 16 `AutoLineMacro::AddMacro` startup-leak records every
  other run (base and after) shows — an instrument variance I did not diagnose; the frame count
  is the signal.
- LSan, commander game (Baka vs Baka, probe decks = `deck1` + `#CMD:Ihsan's Shade`, one-shot):
  base **44** `initDeck` frames (747,608 B / 4,103 allocs) → after the hoist 28 frames, all at
  the rejected-card `NEW` (the sibling leak, fixed in the same commit) → final binary **0**
  `initDeck` frames (407,804 B / 3,033 allocs = the startup baseline every non-commander run
  shows). 0 AddressSanitizer errors in all three games (15-17 turns each, completed).

## Predictions (falsifiable)

- A7: `ManaCost::init` share of suite CPU halves (5.0% → ~2.5%); not measured this lane (no
  perf run — the box was shared); the RSS fixture is the proxy and it fell 3.5x.
- A11: dead-token UAF signatures (`validateCardPointer` cores 3266478 / 3151670) do not recur
  in an ASAN selfplay corpus; on the Vita the Aug-28 `_malloc_r` corruption class should stop.
  Cost: one extra turn of dead-token instances per player.
- A15(a): `TRACK_OBJECT_USAGE` `MTGCardInstance` count at end of turn over a 30-turn selfplay
  drops for library/hand-cycling decks; not measured this lane.
- A27: any transcript/undo/dumpAssert taken with a non-empty exile now round-trips the exile
  zone. Verified directly with a temporary env-gated dump (removed before commit): a fixture
  with `hand:hill giant,grizzly bears` / `exile:llanowar elves` serialises `hand=205,155` and
  `exile=166` (before: `exile=205,155`).

## What I did NOT verify

- A11 RED-on-base pin: the suite compares eight fixed zones (`TestSuiteAI.cpp:1456`) and
  `garbage`/`garbageLastTurn` are not among them, and TestSuiteAI.cpp is another lane's file, so
  no [ASSERT] can observe the dead-token deferral. `token_dies_two_turns_w54i.txt` is GREEN on
  both binaries in the plain suite; it exercises the class under ASAN (0 errors both ways — no
  engine path in that fixture reads a dead token one turn later). A 3-line `garbagelastturn:`
  assert key in TestSuiteAI's endstate arrays would make it RED on base — offered to the
  harness lane.
- PayZero (`zerocast` is only ever granted by `transforms(newability[zerocast])`), the
  anytypeofmana paths (`canAfford`, `planPayment`), and the sunburst site were not exercised by
  a fixture; the edits are the mechanical `{0}` → `NEW ManaCost()` substitution.
- No perf run; no Vita build; no selfplay corpus (invariant 00 forbids a truncated one).
- `WAGIC_CLEANUP_BATTLEFIELD_ONLY` / `WAGIC_GARBAGE_ONE_TURN` / `WAGIC_MANACOST_EAGER_SUBCOSTS`
  disable flags were compiled, not exercised in a gate.
- Lane W's two vpk12 transcripts (bonus, not gate): replayed on this binary, both still fail the
  same way — `…deck3-vs-ai_baka_deck164` `REPLAY DIVERGED at action 94 expected 'p2.no 1' got
  'p2.choice 0' turn 5 phase 4`; `…deck5-vs-ai_baka_deck95` aborts in `dumpAssert` at load.
  Neither dump has an `exile=` line, so A27 was not their cause.
