# Wave-73 lane CB — the Astra step-one review (F1-F7)

Base `1d8c44b47` (master after the review commit), worktree `worktrees/lanes/w73-CB`,
branch `w73-lane-CB`. Files touched: `src/AIPlayerGPT.cpp`, `src/AIPlayerBaka.cpp`,
`src/LegalActions.cpp`, `include/AIPlayerGPT.h`, `include/AIPlayerBaka.h`,
`include/LegalActions.h`, plus two new suite fixtures under `bin/Res/test/`.

**Gates on the final binary** (mtime 22:43, built after the base-probe round trip):
suite `WAGIC_TESTSUITE_THREADS=1` **1283 tests / 0 failed, 76 AI tests / 0 failed**,
both failure strings counted at 0 (`==Test Failed !==` 0, `==Test timed out` 0; none of
the three known flakes fired). PARSETEST **5593 passed / 0 failed** against base
**5574 / 0** — delta **+19**, exactly this lane's additions. Both link guards OK
(`check-ctor-init` 118 files, `check-reply-instructions` 1 source / 37 guides).
`git diff | grep -c U+FFFD` = 0. No corpus, no VPK, no protocol text touched.

**RED on base** — two channels, both re-run on `1d8c44b47` itself:
* the two new suite fixtures, on the base binary: **2 tests / 2 failed, 3 assert
  failures** (`~/.gatelogs/w73-CB-f1-red.log`);
* a temporary PARSETEST probe block written with BASE symbols only, compiled and run on
  `1d8c44b47`: **5574 passed, 5 failed** (`~/.gatelogs/w73-CB-red-on-base.log`), then
  removed and the lane's work restored. The five are quoted per finding below.

Nothing in this lane removes, caps or auto-answers a legal option. Two of the changes
(F1, F2) make the engine ASK where it used to suppress; F3 makes a stated stop bind
LESS often; the rest are render/instrument truth.

---

## F1 HIGH — the chain collapse trusted an oracle that omits legal payments and activations. CONFIRMED (both blind spots). FIXED.

**(a) The policy mismatch.** `loopAutoPassFor` asked
`LegalActionsOracle::hasAnyLegalAction`, which prices casts under
`ManaEngine::FreeProducerPolicy` — the policy written for the HUMAN auto-tap seat, which
may not sacrifice a permanent on a player's behalf — while the model's own cast menu runs
`GptManaPolicy` (`AIPlayerBaka::canHandleCost`, which accepts the sacrifice). Two Lotus
Petals and a castable instant therefore read as "no legal action at all" to the collapse
and as a cast row to the menu.
*Fix:* the oracle's three set-level predicates (`hasAnyLegalAction`, `hasInstantResponse`,
`castableForDisplay`) and the shared ability scan take an optional `ManaEngine::ManaPolicy *`.
NULL keeps the historical free-producer behaviour byte for byte (every human-display caller
is unchanged); `loopAutoPassFor` passes the seat's OWN `GptManaPolicy` when the seat is an
`AIPlayerBaka`, so the collapse asks the question the menus ask. A non-AI seat keeps the
conservative default.

**(b) The phase-restricted activation.** `LegalActions.cpp`'s shared usability scan dropped
every activation whose restriction was not `NO_RESTRICTION` / `PLAYER_TURN_ONLY` /
`OPPONENT_TURN_ONLY` whenever the window was not sorcery-speed — so an "only during your
upkeep" ability was invisible DURING ITS OWN UPKEEP, to the collapse **and to the human's
ability border** (this is a live display bug beyond the review's scope, and the fixture pins
it as one). *Fix:* `restrictionUsableNow` mirrors `ActivatedAbility::isReactingToClick`'s own
switch — the MY_*/OPPONENT_*/bare phase ranges compare against the current phase and turn
ownership, `CAN_PLAY_LAND` routes to `canPlayLandNow`, `AS_SORCERY` and any restriction the
table does not know keep the old conservative answer.

**RED on base** (`bin/Res/test/w73cb_f1_petal_response.txt`,
`w73cb_f1_upkeep_activation.txt`, both run through the existing `assertloopautopass` /
`assertusable` suite commands, so the predicate is pinned against a REAL board, not assumed
inputs — which is exactly the review's complaint about PARSETEST 79392):
* petal fixture: `assertloopautopass: player 1 expected 0 got 1 (anyLegalAction=0)` —
  Shock in hand over a lone Lotus Petal, opponent's Sanguine Bond + Exquisite Blood standing;
* upkeep fixture: `assertusable: 'balduvian hydra' expected 1 got 0` **and**
  `assertloopautopass: player 1 expected 0 got 1`.
Both green after; the whole suite is green after.

**Prediction (next corpus).** `chain_windows_collapsed` never rises on a turn whose next
record offers a cast row over the same board; a seat holding only sacrifice-cost mana
(Petal/Lion's Eye Diamond shapes) is ASKED inside a chain; a "usable only during your
upkeep/end step" activation renders with its border and reaches the menu in its own phase.

## F2 HIGH — a hold could survive past the holder's next untap. CONFIRMED. FIXED at the event AND in the predicate.

The wave-73 BY release asked "is it MY turn NOW and later than the turn the hold was taken
on", and that question is only ever put when a menu reaches `holdHonoured`. The review's
sequence — hold on the opponent's turn 14, an own turn 15 with no castable card and no
non-mana activation (casting returns before the hold check with no candidates; mana-only
priority menus return before it too), a call on opponent turn 16 — never released it, and the
suppression could then run for turns.

*Fix, two halves.* (1) The predicate is re-stated so its answer cannot depend on WHEN it is
asked: `w73HoldExpiredByUntap(heldTurn, ownTurnAtHold, nowTurn)` — this is a two-player
game, so the holder's next untap is the next turn for a hold taken on the opponent's turn and
the turn after that for one taken on the holder's own. `takeHold` records whose turn it was
(`mHoldOwnTurnAtTake`). (2) `releaseHoldIfUntapPassed()` fires it EAGERLY at the untap
event — called from `receiveEvent` on any `WEventPhaseChange` (turn ownership read off
`observer->currentPlayer`, never the event's inverted `to->player`), from `decisionPending`
(every tick this seat acts), and from `holdHonoured` itself, so a missed event cannot
resurrect a dead latch. The release is counted and traced where it happens.

**RED on base:** `w73HoldExpiresAtUntap(14, 16, false)` = false —
`FAIL #W73-CB F2 RED a hold taken on THEIR turn 14 whose holder's turn 15 asked nothing is
still standing on their turn 16`. BY's five own pins were re-signed to the new argument and
still pass unchanged in meaning.

**Prediction.** `hold_released_turn` rises to at least one per hold that spans an untap,
including holds taken on the opponent's turn; no seat shows a suppressed-window run spanning
more than one of its own untaps; the release trace names which turn the hold was taken on.

## F3 HIGH — natural-language thresholds became binding stops. CONFIRMED (both triggers). FIXED.

**(a) The number's unit.** N3's filler tolerance made `PLAN: Stop at 3 life; M=10 creatures`
parse as stop=3, and a creature repeat row then compared its live creature count against a
LIFE total. `w73StopUnitIsCountable` reads the word that follows the number and refuses
`life/lives/mana/card/cards/turn/turns/damage/points/poison/loyalty/land/lands`; applied to
BOTH labels. A bare number, `now`, or a count noun (`24 creatures`) binds exactly as before.
Failing the test costs an ASK, never a suppressed row.

**(b) The ownership scope.** `repeatPlanStopIsOwn` read only the word immediately before the
label, so `PLAN: Their intended stop at 24` passed on "intended". It now scans every word from
the start of the CLAUSE the label sits in (the PLAN label, or the last `; , . :`) and voids the
stop on any of `their/theirs/opponent('s)/opponent/they/them/his/her/its/enemy('s)`. An
attribution in an EARLIER clause does not void this seat's own stop.

**RED on base:** two of the five probe failures —
`FAIL ... a LIFE threshold binds as the creature repeat stop` and
`FAIL ... the opponent's stop passes the ownership test`. BZ's accepted-form pins
(`stop 24; M 24 now`, `stop count`, `stop at`) and BX's `their stop=29` refusal all stay green.

**Prediction.** `stop_reached_windows_skipped` never fires on a turn whose PLAN line states its
stop in a non-count unit; no window is collapsed against a number the reply attributed to the
opponent.

## F4 MED — the LOOP RUNNING bracket declared chain-starting rows unrelated. CONFIRMED. FIXED per row.

N5's replacement sentence was chosen off a BOOLEAN (does any acting row exist), so a row whose
cost is life — Phyrexian Reclamation's "pay 2 life" against a live Sanguine Bond / Exquisite
Blood pair — was declared "NOT part of the chain" while paying it feeds the chain. The
categorical sentence is gone: `loopChainingNote` now takes a fourth fact, whether an acting row
TOUCHES life (`w73RowTouchesLife`, read off the same rows the menu is about to print, at both
seams). With a life row the bracket names the life term and says to weigh that row WITH the
chain; with none it says only that the rows above are still available; with no acting row at all
the wave-66 hold-answers-all sentence stands, unchanged. No row is removed on any face.

**RED on base:** `FAIL #W73-CB F4 RED an acting row is declared unrelated to the chain on a
boolean`. BY's own pins were carried over to the four-argument form and still pass.

**Prediction.** 0 prompts contain "NOT part of the chain"; a prompt whose menu carries a
life-paying row carries the life sentence instead, and the reasoning bodies stop treating a
life-cost activation inside a live drain loop as free of it.

## F5 MED — the ghostform gloss promised a return after the granted ability was removed. CONFIRMED. FIXED.

A ghostform counter is a REMINDER (Wizards' own Kaldheim release notes say the counters have no
functional significance); Kaya's +1 grants the death/exile trigger and marks it with a counter,
and anything that removes the creature's abilities (Sudden Spoiling) leaves the counter with
nothing behind it — while the board kept printing "killing it is not removal" in exactly the
state where the trade it discourages is right. The gloss is now conditional:
`legibleCounterNameForGrant(name, grantedTriggerLive)` prints the full gloss only when the
granted trigger is live and otherwise prints `ghostform (a marker only - the ability that
returned it is gone)`. `w73GhostformTriggerLive` answers from the ACTION LAYER (the granted
trigger objects are what `ALoseAbilities` removes), matching the trigger the card script itself
names. Every other counter name is untouched.

**RED on base:** `FAIL #W73-CB F5 RED the ghostform gloss promises the return unconditionally`.
CA's N14b pins were re-pointed at the grant-live face and a new pair pins both states.

**Prediction.** A board line for a ghostform creature whose abilities were removed prints the
marker wording, and no reasoning body treats such a creature as un-killable.

## F6 MED — `main_phase_windows_skipped` counted ordinary stack processing. CONFIRMED. FIXED.

The hook incremented on every main-phase tick that took the unresolved-stack branch, so a spell
cast normally in main 1 and then resolving counted as a "skipped casting window" once per tick.
The meter is now a PHASE meter: `noteMainPhaseWindowSkipped` records a PENDING candidate for
(turn, phase) instead of counting; the new paired hook `noteMainPhaseCastingOffered` (fired in
`AIPlayerBaka::computeActions` where the sorcery-speed main-phase branch is actually reached)
cancels it; `flushMainPhaseSkip` counts at most ONCE, when that phase ends — driven by the
phase-change event, and by `logGameEnd` so a phase pending at game over is still counted.
`w73MainPhaseSkipCounts` states the rule in one pure place and is pinned both ways.

**RED on base: DOCUMENTARY, not a failing assertion.** The base has no per-phase state to fail
a pure check against; the defect is visible in the base source (`AIPlayerBaka.cpp:4379` and
`:3949` call an unconditional `mMainPhaseWindowsSkipped++`) and in the review's trigger. This is
this lane's weakest RED — see below.

**Prediction.** `main_phase_windows_skipped` equals the number of own main phases that ended
with no casting window, and is 0 in a game where every main phase cast normally — which makes
N16's proposed comparison against turns with no main-1 record meaningful for the first time.

## F7 LOW — `replays_this_game` still changed meaning between paths. CONFIRMED. FIXED structurally.

`logAskReplay` TOOK a game total, and two call sites passed the combined re-serve counter while
the repeat path passed its own subtotal (ten cache replays -> 10, then the first repeat replay
-> 1, for eleven re-served answers). The parameter is GONE: the record reads
`mAskReplaysReserved` — the one combined counter — inside `logAskReplay`, so no call site can
disagree with another. The per-path subtotal is still on the gameend record as
`identical_ask_answers_reserved`.

**RED on base: compile-time / structural.** There is no runtime assertion for "two call sites
pass different counters"; the removal of the parameter is the fix and the compiler is the test.
PARSETEST prints the semantics line rather than a vacuous CHECK.

**Prediction.** `replays_this_game` is monotone non-decreasing within a game across BOTH `why`
values and its final value equals the game's `ask_replays_reserved`.

---

## Weakest evidence (declared)

1. **F6 and F7 have no failing test on base** — F6's is documentary (source read + the review's
   trigger) and F7's is structural. Everything else in this lane has either a red suite fixture
   or a red PARSETEST case on `1d8c44b47`.
2. **F1's policy threading is verified on ONE board per blind spot.** The fixtures prove the
   collapse predicate and the ability border on a Lotus Petal board and an upkeep-only
   activation; they do not prove that no OTHER caller of `castableForDisplay` /
   `hasInstantResponse` changed behaviour. The default argument keeps every human-display call
   on the old policy byte for byte, and the phase-restriction fix is the one change those
   callers DO see — it is a widening (an ability legal now becomes visible), which is the
   permissive direction, but it is a live change to the human's border that no live game has
   exercised in this lane.
3. **F3's unit test is a DENY list, not the brief's allow list.** The brief asked for "followed
   by nothing, or by the repeat row's noun"; the row's noun is not available at either store
   site, so what shipped is "followed by nothing, or by anything that is not one of the units a
   repeat row never counts". `stop at 12 artifacts` would still bind. The review's counterexample
   class (life/mana/cards/turn thresholds) is closed; a novel non-count unit is not.
4. **F2's arithmetic assumes two players and one turn number per turn.** `nowTurn >= heldTurn +
   (ownTurnAtHold ? 2 : 1)` is exact for this engine's two-seat duel; it would need the untap
   event alone (which is also wired) in any multiplayer variant.
5. **F4's per-row life test is a substring scan** (`life`, `drain`, `lifelink`, `damage`) over the
   rendered row. It is deliberately generous — a row that might feed the chain must never be
   called unrelated — so it will also print the life sentence over rows that only DEAL damage to
   a creature. That is the safe error; the precise version would read the ability's own script.
6. **No corpus and no live game was run.** Every render change here is verified by PARSETEST
   literals, two suite fixtures and the suite; none of it has been read by a model.
