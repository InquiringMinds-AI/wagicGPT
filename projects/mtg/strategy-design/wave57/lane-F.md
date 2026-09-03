# Wave-57 lane F — engine hardening (no render, no prompt change)

Branch `w57-lane-F`, worktree `worktrees/lanes/w57-F`, seeded from master `4d4ea0bdf`
(wave-57 step-1 lane brief on top of the wave-56 close). Docket items **D25, D34, D38, D36**
from `wave56/engine-ledger.md`, plus the **initMTGCI** item handed over mid-lane from lane G's
live key dump. Nothing in this lane touches a render string, a prompt, or a card script:
`PARSETEST` moves only by the two engine self-test cases lane G's item ships.

Baseline re-confirmed on this worktree before any edit (hermetic build, `THREADS=1`):
**suite 1241 tests / 0 failed, 55 AI / 0, 0 timeouts; PARSETEST 2736 / 0.** (The brief's
"1241/2-known" pair is a concurrency artefact; at `THREADS=1` the ground truth is 1241/0,
and that is the number this lane is measured against.)

---

## D25 — `MTGAbility::target` / `::source` OWNERSHIP

Wave 56 lane C made the AI's five **derefs** safe (`liveCardTarget`); it said in its own
"NOT verified" that the **ownership** was untouched and that `a->source` was still read raw
in the same loop. Both halves are now closed.

**(a) The sweep, at the zone delete.** `MTGPlayerCards::beforeBeginPhase` frees last turn's
garbage zone (`MTGGameZone::~MTGGameZone` deletes its cards). Before that `SAFE_DELETE` — with
every doomed card still ALIVE, which is what makes the sweep able to compare and able to run
the engine's own removal contract — `GameObserver::purgeDeadReferences(zone)` runs:

* observer level: `cardWaitingForTargets` naming a doomed card releases the chooser waiting on
  it, a `targetChooser` whose `source` is doomed is freed, `mExtraPayment` on a doomed source
  is dropped;
* `ActionLayer::purgeDeadReferences(zone)`: every `MTGAbility` in `mObjects` **and** in the
  layer's `garbage` list whose `target` is a doomed card has that back-pointer **cleared**
  (every consumer already handles NULL); every ability whose **`source`** is doomed is
  **evicted** through `moveToGarbage` (it cannot function without a source, and `destroy()`
  is safe here precisely because the card is not freed yet); `currentActionCard` and
  `menuObject` naming a doomed card are cleared.

All of it is pointer identity — `hasCard()` is deliberately NOT used, because it dereferences
`card->currentZone` and a stale pointer is what the sweep exists to prevent.

**(b) The reader-side net.** `AIPlayerBaka::rankActivations` built
`cands[0] = a->source` and then asked `zones[z]->hasCard(cands[0])` — and `MTGGameZone::hasCard`
**dereferences** its argument (`card->currentZone`). That is a second use-after-free in the very
loop wave 56 fixed, on the other pointer. `cands[0]` now goes through `liveCardTarget` like
`cands[1]`, so a dead source yields NULL and the existing `!cands[c]` guard drops the pair.

**Disable flag:** `WAGIC_NO_DEADREF_SWEEP=1` restores the pre-wave-57 behaviour exactly (no
sweep, raw `a->source`), so "was it the sweep?" is one environment variable — and so ONE binary
can be run in both arms.

**Prediction (falsifiable).** No `WAGIC_GPT_BOARDINDEX=0` stub game reports a
heap-use-after-free in `AIPlayerBaka::rankActivations`, `MTGGameZone::hasCard` or
`__dynamic_cast` reached from an `ActionLayer::mObjects` walk; with `WAGIC_NO_DEADREF_SWEEP=1`
the same binary and the same matchup does.

## D34 — the per-seat target-chooser owner

The chooser has **always** carried its seat: `TargetChooser::Owner` is stamped from
`source->controller()` in the constructor (`TargetChooser.cpp:1584`). What was missing is any
consumer of that fact — `GameObserver::targetChooser` is one global pointer, and lane Z had to
REMOVE its safety net from `ActionStack::cancelInterruptOffer` because an ordinary decline by
one seat was killing the ACTIVE player's in-flight cast (`counter_unless_pay_x`,
`spell_blast_counter_matching_mv` went red).

With the owner read, lane Z's residual is closed where it lives:
`cancelInterruptOffer` gains a `forcedRelease` argument, passed **only** by the stall
watchdog's release in `ActionStack::Update`. On that route — and only there — a chooser whose
`Owner` is the seat being released is freed with the window. Scoped twice (forced route AND
owner match), so the ordinary decline path is byte-identical to wave 56 and both of lane Z's
red fixtures stay green.

The same `Owner` is what the D38 breaker below reads to decide whether anyone can answer a
chooser, and that path IS fixture-pinned.

**Prediction.** No further "the engine cast my spell on a target I never chose" report of the
vpk15 Putrefy shape arising from a window the WATCHDOG released; the ordinary-decline shape was
already fixed by lane Z and its two fixtures stay green.

## D38 — the phase-advance LIVELOCK, root-caused at the advance

Lane C's stub repro sat in phase 7 for ever, both seats emitting `only display-toggle (Flip
Side) options; auto-passing without a model call` — 65 MB of one line. The row is the
**symptom**: lane D removing it from the AI option list hides this instance and no other.

The mechanism is in `GameObserver::userRequestNextGamePhase`:

```
if (getCurrentTargetChooser())
    return;
```

unconditional, and **nothing in the engine clears a chooser no seat will ever answer**. One
armed chooser therefore refuses EVERY advance for the rest of the game, and every seat
re-enters the same window for ever — with any display-only row, or none at all.

The fix is the codebase's own interrupt-window idiom (ActionStack's stall floor) applied to the
advance: count CONSECUTIVE refusals of the SAME chooser (a changed pointer is progress), and
release it only when the seat that OWNS it (D34's `TargetChooser::Owner`) demonstrably cannot
answer — an AI seat with no decision in flight. A **human** seat is never touched (they are
thinking, and the window is theirs to hold); a seat with a model call in flight is never
touched. Floor: 300 consecutive refusals in a real game (the interrupt watchdog's number),
12 in a suite game (`mSuiteGame`, ActionStack's loading number) because a fixture cannot pump
300 ticks inside one script and a scripted seat declares its targets in the very next command.
The release is traced, and the stderr line is compile-time gated
(`_DEBUG || WAGIC_DEVLOGS`).

**Prediction.** A priority window in which every seat auto-passes cannot hold a phase: no
`timeout`-capped stub game exits 124 with a solid run of one auto-pass line and an armed
chooser in the layer.

## D36 — the THIRD replay defect, root-caused and fixed

Lane E parked the shape: *a recorded mana click inside an interrupt window comes back
`0<name>` refused and never becomes `1<name>`*. Measured here on a fresh Baka-vs-Baka
transcript (125v126, base binary): **60** consecutive
`REPLAY MISMATCH expected 'p1.battlefield[0] 1plains' got 'p1.battlefield[0] 0plains'`,
then divergence. The log line immediately before the first refusal is the whole answer:

```
ActionStack: interrupt window held by Player 1 on 'Overgrown Battlement' for 12 ticks /
0.014 s wall with no progress - releasing (loading: no seat can answer)
```

The stall floor's LOADING arm (12 ticks) is right that no SEAT answers during a replay
(`DuelLayers::Update` never calls `Act()` while loading) and wrong that nobody does: **the
record is the answer**. The record has p1 take the window and tap two lands inside it; the
loader spent 12 ticks on the action BETWEEN those taps, the floor took p1's window away, the
Battlement resolved, and p1's recorded Plains tap was then refused for all 60 retries —
`ActivatedAbility::isReactingToClick` requires `source->controller() == currentlyActing()`, and
the window that made p1 the acting seat had just been removed.

Fix, `WAGIC_REPLAY`-gated only (lane E's idiom): the loader reaching a new recorded action IS
progress — `ActionStack::noteReplayProgress()` resets the no-progress budget at the top of each
outer iteration of `processActions`. The floor then fires only when the loader is stuck on ONE
action, which is exactly the softlock lane AA's fix exists for. Inert outside a replay (undo
and the suite take the old path).

**Prediction.** A recorded click inside an interrupt window is issued while that window is
still held: the `0<name>` refusal shape disappears from replays that carry one (60 -> 2 below),
and no dump that completed before completes less.

## initMTGCI (handed over from lane G's live key dump)

Lane G observed `swapP=1999648119, swapT=1818587231, MaxLevelUp=1919249251` on two otherwise
identical Memnites. `MTGCardInstance::initMTGCI()` never wrote them. A mechanical audit of the
122 scalar members declared in `MTGCardInstance.h` against `initMTGCI` and both constructors
found **six** that no constructor path writes: `swapP`, `swapT`, `MaxLevelUp`, `handEffects`,
`castX`, `nb_damages`. (The other 21 that `initMTGCI` misses are all written by the
`MTGCard *` constructor, so only the default constructor could see them stale; they were left
alone rather than given a second, competing default.) What each cost, in the code that reads
them before their only writer runs:

* `swapP` / `swapT` — only `switchPT()` writes them, and its switch-BACK branch reads them
  first: an un-switched card told to switch back gets a random power and toughness.
* `MaxLevelUp` — only the leveler parser writes it, and `AIPlayerBaka` reads
  `currentlevel < MaxLevelUp` for EVERY card: a garbage ceiling makes every creature look
  level-uppable.
* `handEffects` — only `MTGAbility.cpp` ~6731/6738 writes it; ~7877 reads it to decide a card
  in hand has an active effect.
* `castX` — `MTGCardInstance::init()` zeroes it, but `init()` is not on the `initMTGCI` path.
* `nb_damages` — carried by the header, written nowhere.

All six are now initialised in `initMTGCI` to the value their writers imply.

**Prediction.** Two freshly constructed cards of the same id compare equal on all six fields,
on every platform and every allocator state.

---

## Gates

| leg | seeded base (`4d4ea0bdf`, rebuilt in this worktree) | w57-lane-F |
|---|---|---|
| build | clean | clean (rc 0) |
| suite (`WAGIC_TESTSUITE_THREADS=1`) | **1241 tests / 0 failed** | **1242 / 0 failed** (+1 = the new D38 fixture) |
| AI fixtures (same run) | 55 / 0 | **55 / 0** |
| suite `==Test timed out` | 0 | **0** |
| PARSETEST | 2736 / 0 | **2738 / 0** (+2 = the two initMTGCI engine cases) |

`git diff | /usr/bin/grep -c $'\357\277\275'` = **0** (no U+FFFD; `GameObserver.cpp` is CP1252+CRLF
and every edit to it was byte-spliced with python `rb`/`wb`, CRLF preserved).

### RED -> GREEN, per item

| item | RED (pre-fix) | GREEN (this lane) |
|---|---|---|
| **D38** | `Res/test/w57F_orphan_chooser_phase_livelock.txt` on the base binary: `==phase problem. Expected [ Combat begins ](5), got [ Main phase 1 ](4)==` — twelve `next` commands, no advance | **Test Successful** — the breaker releases the orphaned chooser and the phase moves |
| **initMTGCI** | the two `#W57-F` PARSETEST cases built with the initialiser removed: `2736 passed, 2 failed` — a fresh card inherits the poisoned card's bytes, and two fresh cards differ | **2738 passed, 0 failed** |
| **D36** | 4 fresh Baka-vs-Baka transcripts replayed on the base binary (below) — 60 `0<name>` refusals on two of them | 2 refusals, both replays reach further |
| **D25** | ASAN build, `WAGIC_NO_DEADREF_SWEEP=1`: **heap-use-after-free**, 2 of 2 stub games (exit 1) | **0 ASAN errors**, 146v152 exit 0 |
| **D34** | not fixture-reachable (the watchdog floor is 300 ticks / 20 s); code + full-suite regression only | — |

### D36 — replay indices, before / after

`WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_SELFPLAY=1 WAGIC_AI=baka WAGIC_REPLAY=<dump>`,
`MemoryMax=4G`, 300 s cap. `refused` counts `got 'pN.zone[i] 0<name>'` mismatches.

**Lane E's four device dumps** (the ones the brief names) — unchanged, no regression:

| dump | base | lane F |
|---|---|---|
| vpk12b `...-1788393737-...deck33` (147 actions) | COMPLETE | **COMPLETE** |
| vpk12 `...-1788381501-...deck95` (314) | diverged at **295** | **295** (unchanged) |
| vpk12 `...-1788378791-...deck164` (311) | diverged at **121** | **121** (unchanged) |
| vpk12b `...-1788390621-...deck54` (141) | COMPLETE | **COMPLETE** |

None of the four carries the `0<name>` shape at all (0 such mismatches on either binary): both
divergences are `choice`-ordering, a FOURTH defect (dump 164: expected
`p1.zone[36] 1bloodforged battle-axe`, got an engine-generated `p1.choice 0` with a `fabricate`
chooser armed; dump 95: expected `p1.choice 0`, got a land click). **Docket it.** The dumps
therefore cannot show D36 either way, which is why the measurement below was taken on
transcripts that DO carry it.

**Four fresh Baka-vs-Baka transcripts, recorded by the base binary, replayed by both**
(base column re-run after the fix to prove the base numbers are deterministic — they are):

| matchup | actions | base | lane F | `0<name>` refusals base -> lane |
|---|---|---|---|---|
| 125v126 | 1522 | diverged at **44** | **46** | **60 -> 2** |
| 123v125 | 1574 | diverged at **228** | **231** | **60 -> 2** |
| 146v130 | 465 | 16 | 16 | 1 -> 1 |
| 152v162 | 638 | 3 | 3 | 0 -> 0 |

The two dumps that carried the defect lose it (60 refusals -> 2) and each replays further; the
new divergence on 125v126 is the fourth defect above (a stack-target click logged without its
result digit: `expected 'p2.stack[0] 1overgrown battlement' got 'p2.stack[0]'`). The two that
never carried it are byte-unchanged. **Whole-replay completeness is NOT claimed** — this fixes
one named defect, and the record still diverges on others.

### D38 — the stub arm, and what it actually showed

The brief's RED for D38 is "the stub command with a 240 s timeout is exit 124". Ten stub
self-play games on the base binary (`tools/gpt-stub-server.py --answer "CHOICE: 1"`,
`WAGIC_SELFPLAY_ONESHOT`, `WAGIC_AI=gpt`, `WAGIC_GPT_BOARDINDEX=0`, `timeout -k 10 240`), plus
seven earlier games on lane C's own 146v152 matchup: **16 of 17 exit 0**. Lane C's phase-7
shape did NOT reproduce on the current base — consistent with the ledger's own re-scoping of
D38 to "unreproduced stub condition".

The one exit 124 (146v123, 9,688 lines) is a **different** livelock and is reported as such:
max consecutive run of the auto-pass line is **3**, and the tail is an Intruder Alarm +
Thraben Doomsayer untap loop the AI re-activates without bound — a genuine unbounded
activation loop, not a phase-advance failure. It is NOT this item's RED and the fix is not
claimed to cure it. **Docket it** (an AI-side loop bound, in the shape of the repeat-plan
`mRepeatNoProgress` guard).

So D38's RED/GREEN is the fixture above, which pins the MECHANISM (the unconditional chooser
gate) rather than one matchup's symptom — which is what "so it cannot recur with any other
display-only row" asks for.

---


### D25 — the ASAN arm (the docket's own ask)

An ASAN build of THIS lane's source (`-fsanitize=address -O1`, `-j1` under the 4 G cap; a
`-j4 -B` ASAN build was OOM-killed by the cap and is the reason for the serial rebuild), run in
both arms of the disable flag, same binary, same matchups, `timeout -k 10 900`:

| arm | 146v152 | 146v123 |
|---|---|---|
| `WAGIC_NO_DEADREF_SWEEP=1` (pre-wave-57 behaviour) | **heap-use-after-free**, exit 1 | **heap-use-after-free**, exit 1 |
| sweep on (shipped) | **0 ASAN errors**, exit 0 | 0 ASAN errors, exit 124 (the Intruder Alarm loop above, not a memory error) |

The report is D25's prediction verbatim:

```
ERROR: AddressSanitizer: heap-use-after-free ... READ of size 8
    #0 MTGGameZone::hasCard(MTGCardInstance*)          src/MTGGameZones.cpp:1096
    #1 AIPlayerBaka::rankActivations(...)              src/AIPlayerBaka.cpp:2735
    #2 AIPlayerBaka::selectAbility()                   src/AIPlayerBaka.cpp:2419
...
freed by thread T0 here:
    #1 MTGCardInstance::~MTGCardInstance()             src/MTGCardInstance.cpp:340
    #2 MTGGameZone::~MTGGameZone()                     src/MTGGameZones.cpp:1005
    #4 MTGPlayerCards::beforeBeginPhase()              src/MTGGameZones.cpp:252
```

Read site: `hasCard` dereferencing `a->source`. Free site: the garbage-zone delete in
`beforeBeginPhase`. Both halves of D25, named by the ledger, confirmed by the instrument and
closed by the fix — and the wave-56 `liveCardTarget` deref fix was NOT enough on its own,
because this is the OTHER pointer.

## What I did NOT verify

* **No live model ran anywhere in this lane.** Every game was the stub server
  (`--answer "CHOICE: 1"`). No prompt, row or verdict was produced by a real reply, and none
  was changed by this lane.
* **D34's watchdog leg is not fixture-pinned.** The stall floor is 300 ticks / 20 s of
  accumulated dt for a heuristic seat; a suite script cannot pump that inside one fixture, and
  I did not add an env knob to shorten it (the D38 floor is `mSuiteGame`-keyed instead, which
  needs no knob). The evidence for the forced-release leg is: the code path, the ownership
  scope argument, and the full suite staying green — including lane Z's
  `counter_unless_pay_x` / `spell_blast_counter_matching_mv`, which are exactly the fixtures
  that went red when lane Z tried this net WITHOUT an owner. That is a regression control, not
  a positive proof.
* **D34's second half is NOT done.** The docket also asks for "a whose-board term in
  `AbilityFactory::abilityEfficiency`". Lane Y did the proliferate case in
  `ProliferateChooser::canTarget`; the general scorer term is untouched here and is still open.
* **The per-seat chooser is an OWNER READ, not a per-seat pointer.** `GameObserver::targetChooser`
  is still one global pointer; what changed is that its `Owner` is now consulted by the two
  paths that were guessing. A seat-indexed chooser (two pointers, or a chooser stack) is a
  bigger change and was not attempted; if a case appears where two seats must hold choosers
  simultaneously, this does not cover it.
* **D38's stub RED is a fixture, not the stub command.** Lane C's phase-7 shape did not
  reproduce on the current base in 17 stub games (16 exit 0; the one 124 is a different
  mechanism, above), so the exit-124 -> exit-0 pair the brief asked for could not be taken on
  that matchup. The fixture pins the mechanism instead. If lane C's exact shape ever returns,
  the first thing to check is whether the chooser gate is the refusing gate — the release is
  traced under `_DEBUG || WAGIC_DEVLOGS`.
* **The D38 breaker has never fired in a real (non-suite) game.** The 300-refusal floor was
  not reached in any stub game run here, by construction: nothing armed an orphaned chooser.
  Its live behaviour is argued from the fixture (which exercises the same code at floor 12)
  and from the guards, not observed.
* **D36 fixes ONE replay defect.** The `0<name>` shape is gone from the two transcripts that
  carried it (60 -> 2 refusals) but neither replay COMPLETES; both now diverge a few actions
  later on a fourth defect (a stack-target click logged without its result digit,
  `expected 'p2.stack[0] 1overgrown battlement' got 'p2.stack[0]'`), and the vpk12 dumps
  diverge on a fifth (engine-generated `choice` ordering). Neither is fixed here.
* **No suite pin for D36.** The replay loader is `getenv("WAGIC_REPLAY")`-gated and unreachable
  from a fixture, the same argument lane E and wave-53 lane T made. The before/after table is
  the evidence.
* **The initMTGCI RED depends on allocator reuse.** The PARSETEST case poisons a card, frees
  it, and allocates the next one; glibc returns the same block for the same size class, so the
  pre-fix binary inherits the poison (verified: `2736 passed, 2 failed` with the initialiser
  removed). Under a different allocator the RED could pass vacuously — the GREEN (all six
  fields read 0, and two fresh cards compare equal) does not depend on it.
* **The other 21 unset-in-initMTGCI members were left alone.** They are written by the
  `MTGCard *` constructor, so only the default constructor could see them stale, and giving
  them a second default in `initMTGCI` risks disagreeing with the constructor's. They are
  listed in this lane's audit and are a candidate docket item, not a fix.
* **No Vita/PSP build, no GUI drive, no corpus.** Desktop headless only; port builds are
  owner-initiated.
* **The suite's known concurrency pair** (`lifeline.txt`, `merrow_reejerey.txt`) was not
  exercised: every run here is `THREADS=1`, where they are green.

