# Wave-68 lane BE — the eight findings of `wave68/codex-review.md`

Base: `master 1a79e4b78` (wave-68 BA-BD merge + the review commit). Branch `w68-lane-BE`.
Baselines on this tree: PARSETEST **5250 / 0 failed**; suite `WAGIC_TESTSUITE_THREADS=1`
**1275 / 0 failed**, **67 AI / 0**, **0 timeouts**.

Gate after the lane (fresh `bin/wagic`, link at 13:30, both legs in one detached unit
`w68-BE-gate2` under `MemoryMax=4G MemorySwapMax=0`):

| gate | base | after |
|---|---|---|
| PARSETEST (`WAGIC_GPT_PARSETEST=1`) | 5250 passed / 0 failed | **5272 passed / 0 failed** (+22) |
| suite THREADS=1, `==Test Failed !==` | 0 | **0** |
| suite THREADS=1, `==Test timed out` | 0 | **0** |
| suite test count | 1275 | **1277** (+2, the two legend fixtures) |
| AI tests | 67 / 0 | **67 / 0** |

## Verdicts

| # | sev | finding | verdict | what shipped |
|---|---|---|---|---|
| 1 | HIGH | `//#W68-BA (J3/J6)` at col 379 of the ctor initializer list comments out 29 initializers | **CONFIRMED** | comment moved to the line's trailing tag; full member/initializer diff run; a build-time guard added (R1) |
| 2 | HIGH | the legend-rule clause reverses the engine (`AAMover` moves the SELECTED target) | **CONFIRMED — by fixture, both directions** | clause rewritten to say the pick DIES; two suite fixtures pin the engine semantics (R2) |
| 3 | HIGH | the stop clamp turns a re-affirmed single activation into a pass | **CONFIRMED** | `repeatStopClampApplies` — only COUNTED rows clamp; the re-ask still fires for a base row; the pin that ratified the suppression is replaced (R3) |
| 4 | MED | the combat seams delegate to Baka on a cap-truncated reply, with no `reply_truncated_reask` | **CONFIRMED** | one truncation re-ask per turn at attackers and at blockers, on its own latch, quoting the cap (R4) |
| 5 | MED | the edict crack-back prints a worst-case bound as a death verdict | **CONFIRMED** | the bound is printed as a bound; "that is the one they will give up" removed (R5) |
| 6 | MED | the announcement-decline clause claims the floating pool is FORFEITED | **CONFIRMED** | the clause says what the pool actually does; `stripNarrationDecoration`'s prefix moved with it (R6) |
| 7 | MED | the deck-out countdown ignores `CANTLOSE` / `CANTMILLLOSE` / `CANTWIN` | **CONFIRMED** | the three flags the engine itself checks are folded into the helper; the count is still printed, the consequence changes (R7) |
| 8 | LOW | per-attempt `deadline_pct` divides the retry by the ORIGINAL deadline | **CONFIRMED** | each leg is measured against the budget it ran under (`remainingTransportRetryMs`) (R8) |

Eight findings, eight confirmed. Nothing was refuted.

## Mechanisms

### R1 — the eaten initializer list (`src/AIPlayerGPT.cpp`, `wagic-SDL.pro`, `tools/check-ctor-init.py`)

`AIPlayerGPT.cpp:15516` is 1071 bytes wide and carried `//#W68-BA (J3/J6)` at column 379.
Everything after it on that physical line was a comment: **29 initializers**, verbatim from the
guard's own output —

```
mAnswerReplacedFalse, mAttacksDoneTurn, mCastAskPhase, mCastAskTurn, mEngineRevealFloorPicks,
mHoldTurn, mHoldWindowsSkipped, mIdenticalOptionAsksResolved, mLoopAbility, mLoopClick,
mLoopCount, mManaOnlyWindowsSkipped, mPassDeclineTurn, mRecoveryExecRow, mRepeatAbility,
mRepeatAbsent, mRepeatAskAnswersReserved, mRepeatAskChoice, mRepeatAskTurn, mRepeatClick,
mRepeatDone, mRepeatNoProgress, mRepeatRemaining, mRepeatTotal, mReserveDeclinePhase,
mReserveDeclineSources, mReserveDeclineTurn, mReserveDeclineWindows, mStuckCastTurn
```

(The review said 30/29; the exact count is 29 initializers plus the line's own trailing tag
comment.) The list still compiled because the last surviving initializer already ended in a
comma and line 15517 continued the list. Fix: the tag moved to the end-of-line comment the
line already had.

**Full audit, not just the eaten line.** Every `m…` member declared in `include/AIPlayerGPT.h`
(232 declarations) was diffed against the ctor's initializer list (101 initializers) and against
the ctor BODY. Every POD/pointer member absent from the initializer list is assigned in the ctor
body (`mStatedStop`, `mLastPoison[]`, `mOppLifeByTurn[]`, `mBulkMove*`, `mSearch*`,
`mDamageLife*`, `mPregameBottom*`, `mLastHttpStatus`, `mPatienceLimit`, `mThinking`, …); every
other absentee is a `string`/`vector`/`map`/`json` that self-initializes. **Nothing else is
uninitialized.**

**The guard chosen: `projects/mtg/tools/check-ctor-init.py`, wired as `QMAKE_PRE_LINK` in
`wagic-SDL.pro` — it runs before every link of the dev build and fails the build.**
A blanket "no `//` between the ctor's `:` and its `{`" test would be pure noise here: trailing
tag comments on init-list lines are the codebase's own idiom (this very line ends with one).
The test used instead is exact: strip `//` tails from the initializer list, and compare the set
of `name(` initializers **before and after** stripping. Any name that disappears was eaten by a
comment. Only members (`m<Upper>…`) and the base-class initializer count, so a comment tail that
merely mentions `#W67-AX (I7)` is not a hit. **Zero false positives across all 118 `src/*.cpp`.**

RED evidence: the pre-fix line, restored into a scratch copy, makes the guard exit 1 and name
all 29 eaten initializers (quoted above). On the fixed tree it exits 0 (`check-ctor-init: OK
(118 file(s))`), and that line is now printed by every `make -f Makefile.sdl` before the link.

### R2 — the legend rule says what the engine does

`MTGNewLegend::MoveLegend` (`MTGRules.cpp:4093-4105`) builds a `GenericTargetAbility` over
`AAMover(..., "ownergraveyard", "Put in Graveyard")`; `AAMover::resolve()` moves the **selected
target**. So the copy the pilot picks is the copy that dies, and the wave-68 clause
("you KEEP the one you pick here") pointed the model at exactly the wrong card.

Verified **by fixture, in both directions**, using two printings of the same legendary
planeswalker so the ASSERT can name *which* copy survived — Ajani Goldmane `140233` (LRW) and
`191239` (M10):

- `bin/Res/test/legend_rule_pick_dies_w68be.txt` — click `191239` → `191239` in graveyard, `140233` in play.
- `bin/Res/test/legend_rule_pick_dies2_w68be.txt` — click `140233` → `140233` in graveyard, `191239` in play.

Both pass; together they prove the survivor is decided by the pick and not by list order.
Both registered in `Res/test/_tests.txt` (suite 1275 → 1277).

The clause now reads: *"the one you pick on this list is the one that GOES TO ITS OWNER'S
GRAVEYARD now; the other stays on the battlefield. Pick the copy you want to LOSE, not the copy
you want to keep."* (`s go` / `stays` agree in number at 3+ copies.) The no-merge sentence is
unchanged apart from "the one you keep" → "the one that stays".

### R3 — a second explicit answer for ONE activation executes one activation

J1's re-ask for the plain activation row is **kept** — a stated stop does apply to any
activation of the same ability, and asking once more is legality-free. What is removed is the
second half: the wave-68 code ran the clamp on the re-affirmed answer too, and because
`repeatStopClampCount(1, stop, M)` is 0 whenever `stop - M <= 0`, the `else` arm set
`choice = 0` — an explicit `CHOICE: 1` became a pass the model never wrote. That is the engine
answering for the model, which the doctrine forbids.

New pure predicate `repeatStopClampApplies(repeatRowTaken, stopTestCount)`: the clamp is
consulted only for a COUNTED take. A base-row second answer executes as given and the record
carries `stop_conflict_single_activation_executed(stated_M=…,stated_stop=…,executed=1)` so the
class is countable in the next corpus. Counted rows clamp exactly as `#W66-AS (H3)` /
`#W67-AY (I6)` / `#W67-AZ (R2)` shipped them — `repeatStopClampCount` and
`repeatStopClampReceipt` are untouched.

RED: the base binary's own PARSETEST prints
`PASS  #W68-BA J1 POSITIVE the single activation at M=24 against stop=23 performs the model's
own zero` — the pin that ratified the suppression. It is replaced by three cases pinning the
doctrine (a re-affirmed single activation is never clamped; a counted take still is; a repeat
row with no count is not a counted take).

### R4 — the combat seams honour J3's promise

J3 gave `chooseAttackers`/`chooseBlockers` a `max_tokens` cap and no recovery: a reply that
reported `finish_reason:"length"` before writing its label reached
`AIPlayerBaka::chooseAttackers()` / `chooseBlockers()` directly. On the base source
`reply_truncated_reask` exists at exactly two seams (priority `:33827`, ask `:34878`) and at
neither combat fallback (`:44248`, `:45522`) — that is the RED, structural and countable.

Shipped: one truncation re-ask per turn at each combat seam, on its **own** latch
(`mAttackTruncReaskTurn` / `mBlockTruncReaskTurn`) so it cannot spend the prose-reversal arm's
single ask, sharing the seam's existing correction LINE (only one correction can be pending).
The correction text is the pure helper `combatTruncationReaskLine(attackersSeam, maxTokens)` so
PARSETEST pins the exact words; it quotes the cap that was used
(`mLastRequestMaxTokens`), states that nothing was declared, and asks for the label FIRST.
The heuristic answers only after the arm is spent; the record carries
`reply_truncated_reask` / `…_recovered` / `…_exhausted` / `…_unanswered`, matching the ask and
priority seams' vocabulary.

Explicit declines are untouched: `BLOCKS: none` is answered above this point, and
`ATTACK: none` parses to `result >= 0`.

### R5 — the crack-back bound is printed as a bound

`total - minAttackerPower` is the **most** that can be left, because THEY choose the sacrifice.
Base line, verbatim (10 life, four bodies in an 18 total, smallest 2):

> `… smallest of them contributes 2 - that is the one they will give up - so this takes AT LEAST 2 off it: 18 - 2 = 16 -> you would be at -6; that STILL KILLS you`

Handing over a bigger body leaves less than 16 and the seat lives. The bound holds in one
direction only: it can never overturn survival, and it can never prove death. Now:

> `… smallest of them contributes 2, so whichever one THEY hand over this takes AT LEAST 2 off it: 18 - 2 = 16 is the MOST that can be left -> you could still be as low as -6 - a KILL IS POSSIBLE, not certain: THEY pick which creature to give up, and giving up a bigger one leaves less than 16. This row does not prove you die and it does not prove you live`

The SURVIVE branch and the FLOOR branch are byte-identical to wave 68 apart from the shared
lead-in; the header comment's "because that is the one they will give up" is gone too.

### R6 — the announcement decline says what the pool does

The clause said the pool is FORFEITED while `castAbandonedNarration`, written on the same
decline, said the mana is "still floating, unspent". The pool survives the decline: the sources
stay tapped, the mana stays in the pool, and it is spendable until end of step. What *is* true
of this screen is that no row on THIS list can use it — an X-announcement menu only sets X for
the cast being cancelled — so the clause names that instead of inventing a forfeiture.

`stripNarrationDecoration`'s prefix moved with the wording (`{declining now FORFEITS ` →
`{declining strands `, length 19). **That coupling is the trap this class of edit sets** — the
prefix is what keeps the clause out of history, and the ECHO case
`#W68-BB J5 ECHO the forfeiture clause never enters history` caught it going red mid-lane.

### R7 — the deck-out countdown folds the engine's own exceptions

`MTGPlayerCards::drawFromLibrary` (`MTGGameZones.cpp:387-397`) returns **without setting a
loser** when the drawing player's battlefield has `CANTLOSE` or `CANTMILLLOSE`, or the other
side's has `CANTWIN`. `opponentZoneCountsLine` took zone counts only and promised a loss the
engine will not deliver. The three flags are now read at the call site from the same zones the
engine reads (opponent's `inPlay` for CANTLOSE/CANTMILLLOSE, the seat's own for CANTWIN) and
passed as one bool.

Per the trust doctrine the count is never hidden — only the consequence sentence changes:
*"they have N cards left, but DECKING THEM OUT DOES NOT WIN: a permanent in play stops the
empty-library loss …, so their draw from an empty library ends the game for nobody."*
Outside three draws the flag changes nothing.

### R8 — each attempt against its own budget

The retry never receives the original deadline: the transport arm sets
`mRetryBudgetMs = remainingTransportRetryMs(mTimeoutMs, firstMs)` and `buildRequestBody` uses
it as that call's timeout. `deadlineTenthsPctOfAttempts` divided both legs by `mTimeoutMs`, so a
120 s decision that failed transport at 20 s and then spent its whole 100 s remainder published
**83.3 %** for a leg that consumed **100 %** of its budget. Each leg is now measured against
what it was given. BC's three shipped cases are unchanged by construction (`162v126 s13` is
still 100.0 %, a single-attempt record is still the shipped figure, no leg is still `-1`), and
that invariance is now itself a PARSETEST case.

## RED evidence

Two RED instruments, both reproducible:

**(a) the guard, against the pre-fix source** — `tools/check-ctor-init.py` on a scratch copy of
`AIPlayerGPT.cpp` with the comment restored to column 379 exits 1 and names all 29 eaten
initializers; on the shipped tree it exits 0 over 118 files.

**(b) the base helpers, against lane BE's new expectations** — the six pure helpers were lifted
verbatim out of `git show HEAD:projects/mtg/src/AIPlayerGPT.cpp`, compiled standalone, and run
against the new cases (`~/.gatelogs/w68be/red-base.cpp`, log `red-base.log`). Output:

```
RED-CONFIRMED  R2 base clause does NOT say the pick dies -> new POSITIVE case is RED on base
RED-CONFIRMED  R2 base clause says the OPPOSITE of the engine -> new MUST-NOT-MATCH is RED on base
RED-CONFIRMED  R5 base prints no bound -> new POSITIVE case is RED on base
RED-CONFIRMED  R5 base prints the categorical kill AND the unchosen-decision claim -> new MUST-NOT-MATCH is RED on base
RED-CONFIRMED  R6 base never says the pool survives -> new POSITIVE case is RED on base
RED-CONFIRMED  R6 base claims forfeiture -> new MUST-NOT-MATCH is RED on base
RED-CONFIRMED  R7 base countdown is unconditional (the helper takes no flag) -> new blocked-case is RED on base
RED-CONFIRMED  R8 base reports 83.3% for a retry that spent 100% of its own budget -> new POSITIVE (==1000) is RED on base
=== 8 RED expectations checked, 0 did NOT reproduce ===
```

**R3** RED is the base binary's own PARSETEST line (`PASS #W68-BA J1 POSITIVE the single
activation … performs the model's own zero`) — the base affirmatively pins the suppression this
lane removes. **R4** RED is structural and counted on the base source: `reply_truncated_reask`
at 2 seams, 0 of them combat, while both combat fallbacks sit at `:44248` / `:45522`.

Base-binary PARSETEST for the counterfactual was captured whole at
`~/.gatelogs/w68be/parsetest-base.log` (`5250 passed, 0 failed`), together with the base binary
itself at `~/.gatelogs/w68be/wagic-base-renders`.

**RED-on-base count: 10** (8 helper expectations + the R3 pin + the R4 seam census), plus the
ctor guard.

## Predictions for the wave-68 corpus (falsifiable)

1. **R1** — no GPT seat constructs with a non-`-1` `mAttacksDoneTurn` or a positive
   `mRepeatRemaining` before any repeat is requested; `FindCardToPlay` returning NULL with no
   repeat in flight occurs 0 times.
2. **R3** — `stop_conflict_single_activation_executed` appears on records where wave 68 would
   have written `repeat_clamped_to_own_stop(named=1,…,executed=0)`; the count of the latter with
   `named=1` is **0**.
3. **R4** — `reply_truncated_reask` appears on `attackers` / `blockers` records; every one of
   them resolves to `_recovered`, `_exhausted` or `_unanswered`, and no combat record carries a
   `finish_reason:length` fallback to the heuristic without one of those notes first.
4. **R8** — no record publishes `deadline_pct` above 100.0 unless one attempt genuinely
   exceeded its own budget; `attempt_ms` and `deadline_pct` agree leg-for-leg.
5. **R2/R5/R6/R7** — the new wordings appear verbatim in the prompts; no reply argues from
   "I keep the one I pick", from a certain edict kill, from a forfeited pool, or from a
   deck-out that the board blocks.

## What I did NOT verify

- **No live model call was made.** Every finding was verified by fixture, by pure-helper RED,
  or by source/call-path reading. R4's re-ask has **not** been observed firing against a real
  truncated reply — the wording and the latch discipline are pinned by PARSETEST, the wiring is
  not exercised end-to-end. Its firing rate is a wave-68 measurement, not a claim.
- **R7's blocked branch has no fixture.** No `CANTLOSE` / `CANTMILLLOSE` / `CANTWIN` permanent
  was put on a board and rendered; the flag is read from the same two zones
  `drawFromLibrary` reads, and the rendered branch is pinned in PARSETEST, but the *call site's*
  boolean is verified by reading only.
- **R3's runtime path** is pinned at the predicate, not at a live re-ask: no fixture drives a
  priority window through a stop conflict and a second explicit answer. The clamp arithmetic and
  the applicability predicate are pure and pinned; which branch a real window takes is not.
- **R5's `totalIsFloor` branch** was left untouched and re-read, not re-derived.
- The legend fixtures use two *printings* of one legendary planeswalker rather than two copies
  at different loyalties. That exercises the same `MTGNewLegend` path and is the only way the
  ASSERT can name which copy survived, but it does **not** prove anything about loyalty
  counters surviving the move.
- The `QMAKE_PRE_LINK` guard only binds after `qmake6 … -o Makefile.sdl` is re-run. It is live
  in this worktree (the link line prints `check-ctor-init: OK (118 file(s))`); a tree carrying
  an older generated `Makefile.sdl` keeps the guard as a manual invocation until it regenerates.
- No VPK/EBOOT/APK was built.
