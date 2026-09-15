# GPT-layer cleanup lane — sections A, B, C

Branch `gpt-cleanup`, base `45be854f6`. Worktree
`/home/magi/Projects/wagicGPT/worktrees/lanes/gpt-cleanup`. Ledger:
`LEDGER.md` beside this file (marked PROVISIONAL — Astra's cross-review of its
own findings runs at 19:00 CDT; §"If Astra refutes" below names exactly which
items would have to be revisited).

Commits, one per group so a revert is possible per group:

| commit | group |
|---|---|
| `f1a722daf` | A — the eleven confirmed flaws (L1–L11) |
| `1ef2c17da` | B — the confirmed crud |
| `2b2dd7716` | B tail — two dead members the sweep left as declarations |
| `c6001b432` | C — the complexity items the flaw fixes reach |

Rules claims below were grepped from `~/.gatelogs/engine-audit/cr.txt`, never
recalled; every citation in the code carries the rule number and its own text.

---

## A. The eleven flaws

Evidence convention. A PARSETEST pin is the instrument wherever the defect has a
PURE surface (a renderer, a key builder, a parser predicate). Where the defect
lives in a LIVE window — a `continue` inside a seam, a gate on a member function —
there is no pure surface to pin, and the instrument is the two-game LLM probe at
the end of this report plus, for the doctrine items, the deletion of the code
that could produce the behaviour at all. Where a pin was INVERTED (the old pin
asserted the defective behaviour and now asserts the fixed one) that is stated:
an inverted pin is a RED-on-base by construction — it failed on the base binary,
which is how it was found.

### L1 — legal options withheld / auto-answered

RED. Ten sites, one family. Pins that FAILED on base and pass now:

* `[#W47-R4]` → `[#W82-A L1]` (`AIPlayerGPTSelfTest.cpp`): the last-offer bracket
  `{if you pass here, this option is not offered again until the board changes}`
  is not producible. Base rendered it on 47 corpus prompts.
* `[#W65-AM G7]` → `[#W82-A L1]`: `declineBoardScope`, the board-keyed re-opener
  built around the cap, has nothing to re-open.
* `[#W51-E D7]`: the Upkeep animation clause names no decline count and no LAST
  offer (base: both).
* `[W54-D D8b]`, `[#W80-DF U13]`, `[#W69-BH K4c]`, `[#W66-AS H3b]`: the pins of
  the three auto-answers and the cast auto-pass — deleted with the code.

FIX.

| shape | file:line (now) |
|---|---|
| modal-DFC flip cap, animated-land repeat withhold, pass-decline cap | `src/AIPlayerGPT.cpp:46216-46232` |
| the `{last offer}` bracket, `declineBoardScope`, `fetchLineKey` | deleted, `src/AIPlayerGPT.cpp:45905-45911` |
| all-declined auto-pass | `src/AIPlayerGPT.cpp:46600-46604` |
| fetch consume-on-choose stamp + flip count | `src/AIPlayerGPT.cpp:47714-47717` |
| byte-identical-row auto-answer | `src/AIPlayerGPT.cpp:48151-48157` |
| ETB pay-or-tap auto-click (CR 119.4) | `src/AIPlayerGPT.cpp:54160-54167` |
| single-outcome repeat-pay auto-answer | `src/AIPlayerGPT.cpp:54607-54613` |
| `<upto:N>` zero-target answer (CR 115.6) | `src/AIPlayerGPT.cpp:55021-55028` |
| 40-candidate target cap | `src/DecisionContract.cpp:783-790` |
| land-face decline latch | `src/AIPlayerGPT.cpp:49699-49708` |
| `mStuckCastLines` | `src/AIPlayerGPT.cpp:49911-49920`, `include/AIPlayerGPT.h:1331-1343` |

KEPT, the one exception the ledger names: the generic one-legal-row ask
(`optionsIn.size() == 1 && !askEvenIfSingle → return 0`). A menu with exactly one
legal row is a mechanism, not a choice, and `declineRowOffered` means a decline
row is appended INTO the vector by the caller, so a one-row vector has no hidden
decline.

Two things the fix had to build rather than only delete:

1. **The cast-seam livelock.** `mStuckCastLines` was the guard against the
   documented 135v133 wedge (turn 2, 2400 s, 903k re-picks) and against the
   re-poll churn of a conceded window (corpus 20260823 deck125 vs139 t5 seq11-15:
   five exhaustion records over one board, zero model calls). Deleting it without
   a replacement re-arms both. It is replaced by a WINDOW-level marker
   (`mCastNoProgressBoard`): the one window following a no-progress pick over a
   byte-identical board defers to the heuristic once, and the marker is spent by
   that deferral. No row leaves any menu — the model is shown every row the
   engine offers, and what ends is the window, which is the same exit the
   validation re-ask budget already takes.
2. **The heuristic's own copy of the land-face latch.** `AIPlayerBaka.cpp:3534`
   skips a face-declined land in the land-drop proposal. With the GPT-side filter
   gone the pilot can now NAME such a land, and that skip would have silently
   discarded the answer. It is guarded with `!aiForcedCandidate` — the one member
   only the GPT seams set — so the heuristic's own proposal is byte-identical and
   only a forced (model-chosen) candidate is exempt. This is the lane's ONLY
   change outside the GPT layer and `DecisionContract.cpp`; it is narrower than
   the ledger's item and exists to stop the fix introducing an answer discard.

GREEN. PARSETEST 0 failed; probe below.

### L2 — the ask / priority / async keys are BOARD STATE + QUESTION

RED (inverted pins — each asserted the boardless key on base):

* `#W79-CZ T3 (b)`: base pinned that a life tick 13 → 14 under a byte-identical
  menu must key EQUAL. It now pins the opposite, and both halves:
  `w82WindowKey(board13, tail) != w82WindowKey(board14, tail)` and
  `w82WindowKey(board13, tailA) == w82WindowKey(board13, tailB)`.
* `#W79-DC F1` (the whole lane-DC section): base pinned the legal-continuation
  digest. RED-ON-BASE is stated in the new section: with the digest gone the
  shipped key degenerates to `turn+phase+tail`, byte-identical across a second
  legal target, a grown X ceiling and a moved life total.
* `#W80-DE U8`: base pinned that a declined list REPLAYS across a phase. It now
  pins that End → Cleanup is a different key and is asked.

FIX. `w82WindowKey(boardKey, question)` at `src/AIPlayerGPT.cpp:36575-36594`;
priority seam `:46996`; `askModel` `:48344`; `asyncSlotKey` `:19292-19299`. The
board serialisation's own first line is the `Phase: … | It is …` header, so turn
and phase are inside the board half.

Compensations deleted with it: the continuation digests
(`w79ContinuationDigestOf/Priority/Cast`), the `mAskScopeDigest` census (also
L10's unretired map), the #W81-DI cached-replay bound, the cross-phase replay
branch (the prompt-only NOTE stays — it is a fact and costs no window).

KEPT deliberately: the priority-seam deadlock breaker. The ledger lists it as a
compensation, and its board read WAS one — but its function is the livelock
guard, and with the board now inside the key an unchanged board replays a cached
answer with no round trip, which is exactly the loop the breaker exists to break.
Removing it would have been a regression, not a simplification.

Honest cost, stated by the ledger and unchanged: the 233 same-phase re-asks
#W79-CZ (T3) measured come back. That is the trade the ruling already chose.

### L5 — the crack-back verdict family

RED:

* `#W81-DL V5` → `[#W82-A L5]`, rebuilt from the corpus records. RED-ON-BASE
  retained verbatim: `w80CrackBackVerdictLine(crackBackVerdictKey(5,15,20),-1,20)`
  reads `you survive` on the very screen whose header reads `that would KILL you`
  (`152v130` seq 17, engine-seat HIGH-3, 11 windows).
* NEW RED-ON-BASE for the blockable/unblockable split: priced as UNBLOCKABLE the
  same four points read `LETHAL EVEN THROUGH YOUR BEST BLOCK` — which is what the
  wave-81 line did to every animator.
* NEW: the marker. `crackBackVerdictKey(5,14,16)` is `survive` while
  `crackBackVerdictKey(5,17,16)` is LETHAL — the marker used the first and the
  line the second, so a hold taken at `survive` was never re-opened.
* NEW: the `162v146` seq-30 board (turn 16, Main 1, life 7). Header publishes 17;
  the pin asserts the verdict uses 17 and that the wave-80 third figure
  (`leaving you at -3`, 14 minus Fog Bank's 4) is not producible.

FIX. `struct CrackBackFacts` + `crackBackFactsNow()` — computed once, memoised on
the window seq: `include/AIPlayerGPT.h:2265-2300`, `src/AIPlayerGPT.cpp:37353`.
Sub-defects:

* (a) GATE. `f.due = crackBackNextTurnDue(selfActive, …)` — the header's own gate,
  now on the verdict too. Base printed it on 43–47 opponent-turn windows (11–13
  LETHAL) over a walk that counts their tapped and summoning-sick bodies as
  bodies that "untap first" — CR 502.3 (the ACTIVE player untaps), CR 302.6.
* (b) MARKER. `crackBackVerdictNow()` takes the published total.
* (c) `floorUnsized` is a parameter and hedges the line.
* (d) the best-block DP takes the same body rule the row cover uses
  (`attacksSettled`), so verdict and cover cannot name different blockers.
* (e) the addenda are SPLIT. An animator's combat damage is dealt by a creature in
  combat and is assigned to its blockers — CR 510.1c — so it raises the published
  total and NOT the block floor; only the pinger and compulsory-draw shares are
  described as "damage no block removes".
* (f) joint affordability: the animator rungs are admitted under ONE next-turn
  mana budget, biggest body first (`crackBackRungConvertedCost`).
* (g) the `none` short-circuit no longer swallows a compulsory-draw-only lethal.

The six own-loop verdict emitters: NOT folded, and this is a deliberate
disposition rather than an omission. The render path is ALREADY one entry point
(`w79OwnLoopVerdictLineFor`) over one fact; the other three serve different
consumers (the latch key, the rank comparison, the record face) and Astra's
cross-review defended them explicitly as "not six interchangeable emitters". The
crack-back half of X6 — the lineage that really did re-walk the board four times
per window — is folded, in section C.

### L3 — the in-flight watchdog

RED. Static, and the instrument is the call graph: `reapWedgedRequests` had
exactly one caller, `pollCompletion`, reached only through the decision seams;
`AIPlayerBaka::Act` returns at its first statement (`AIPlayerBaka.cpp:5878`)
whenever `decisionPending(dt)` is true, which `AIPlayerGPT::decisionPending`
returns for as long as `asyncBusy()`. The bound was born unreachable (entry gate
`b192afd1c` 2026-07-12; bound `b9feba4e0` 2026-09-03).

FIX. `src/AIPlayerGPT.cpp:52218-52232` — the reap runs from `decisionPending`, the
gate that IS reached every tick. The reap itself is unchanged and bumps the
generation rather than freeing anything.

### L4 — a discarded worker could destroy a locked mutex

RED. Static: `delete ctx` ran inside the scope of
`lock_guard<GptMutex> g(ctx->state->mtx)`; if that context held the last
`shared_ptr` the guard then unlocked freed storage. UB for `std::mutex`, worse
for the Vita kernel mutex.

FIX. `src/AIPlayerGPT.cpp:18262-18310` — a local `shared_ptr` keeps the state
alive across the guard's destruction; the discard sets a flag and returns after
the guard is gone.

### L6 — row mana vs header mana

RED. The comment at the cast seam claimed the two "can never disagree"; the
corpus has four screens where they do (`146v152` seqs 28 / 32 / 37 / 38 — header
`2 total` over rows reading `of your 3`, board holding a Treasure).

FIX. One policy. Header `src/AIPlayerGPT.cpp:31300`, `windowReach()` `:42897`,
cast rows already `GptManaPolicy` — the policy the payment planner actually
spends with, which is what every play on the screen is paid out of. The false
comment is rewritten to the invariant that now holds.

### L8 — the parser

RED (inverted): `#W79-DD` case (3) pinned that "an action line with prose after it
is refused — the reader takes the reply's LAST word or nothing". It now pins the
ruling: the answer is read, and genuine ambiguity (a second answer-shaped line)
is still refused.

FIX.

* label-less answer: the "must be the reply's LAST non-blank line" clause is
  deleted, `src/AIPlayerGPT.cpp:9107-9119`. Prose is still MEASURED as
  off-protocol — measured, not obeyed.
* blocker ranges: `B4 - B9` (spaced), `B4-9` (bare high index) and `B1 blocks A1`
  are read, `src/AIPlayerGPT.cpp:59404-59460`. The attackers grammar already
  accepted `A4-9` / `A4 - A9`, and `kBlockerRangeNote` teaches the shape on the
  very prompt this parses.
* stale echo: an IN-RANGE coded answer whose name disagrees now takes the
  `named_row` RE-ASK lane at both seams instead of falling through to the
  heuristic (`:47385`, `:48756`). The parser still refuses to execute the
  untrusted number; the window is asked again.
* the no-op re-ask is deleted, both lanes, both seams.

### L9 — wrong-scope and false renders

Twelve renders. Every one had a pin asserting the old wording; all were inverted,
which is 24 RED pins on the base binary.

| render | file:line | rule |
|---|---|---|
| `[tapped - cannot attack or block this turn]` scoped to the ACTIVE player's board (310 of 440 corpus renders were on the opponent's board on our turn) | `:5569-5589`, caller `:12508` | CR 508.1a, 509.1a |
| vigilance stated on the attackers facts and in `system_prompt.txt` | `:302-313`, `bin/Res/ai/gpt/system_prompt.txt:3` | CR 702.20b |
| double strike doubled on the BLOCKERS seam (the attackers seam already did) | `:60012-60022`, `:60130-60141` | CR 702.4b |
| enters-tapped is a state, not a turn-long prohibition | `:49780-49789` | CR 701.26a |
| the land-drop line no longer infers history from legality | `:9694-9724`, `:31367-31383` | CR 305.2 |
| DRAW PUNISHERS no longer claims a drain ("to yours") | `:14440-14452` | `planeswalkers.txt:2686` = damage |
| counterspell target scope names the stack | `:2427-2436` | — |
| held-back universal scoped to legality in this combat | `:7047-7057` | CR 702.3b |
| life-trend samples say they are first-window figures | `:31025-31031` | — |
| the vacuous blocker-count clause prints only when reachable | `:7268-7284` | — |
| "before you act again" → "before your next draw step" | `:13965` | — |
| the pregame bottom ask no longer promises an order the engine does not honour | `:61630-61645` | CR 103.5 |

The bottom-order item is the one L9 fix that makes the SURFACE truthful rather
than making the ENGINE honour the choice: the reply is parsed into a membership
mask, so the naming order is not carried, and an over-pick is trimmed to the
lowest hand positions. Honouring CR 103.5's "in any order" needs an ordered
parse; that is a real improvement and is named in "Left undone" below.

### L7 — the forced-close rescue

RED. Corpus: 13/13 armed closes dropped on the very next record as
`dropped_decision_moved`, each followed by a fresh full-price phase-1 ask on the
same window (35–186 s each, 23.4 min), with the wasted decode's latency zeroed so
the corpus could not price it.

FIX. The leg is SLOT-KEYED, not byte-equal to the whole prompt:
`src/AIPlayerGPT.cpp:20453` (`asyncSlotKey(userMsg) == mRetryBase`), with every
arming site storing `asyncSlotKey(userMsg)`; the wall-miss leg has the identical
gate and gets the identical fix. The abandoned phase-1 latency is now carried onto
the record instead of being zeroed (`:20553-20566`). The ledger offered "slot-key
the legs OR delete the rescue machine — the lane picks the smaller"; slot-keying
is the smaller and it is the one that keeps the answers.

### L10 — telemetry on the wrong record / unbounded

FIX. `mAsyncDropStamps` and `mAbandonedInFlightSecs` carry the window seq they
were stamped on and are emitted only on that window's record, else dropped
(`include/AIPlayerGPT.h:2608-2618`, `src/AIPlayerGPT.cpp:22546`, `:22831`). Base
landed a `casting/…` drop on a `blockers` record. `mAskScopeDigest` — inserted,
compared, never erased, growing for the length of the game — is gone with L2.

NOT done: the `deferSeat` cross-seat record write (`computeActions`), which can
consume the OPPONENT seat's pending members onto a record about this seat's menu.
It is a real defect (Fable F11) but its fix is the per-window record shape Fable
proposes as I8, which the ledger puts in §D. Named in "Left undone".

### L11 — the cast-seam loop/chain auto-pass

FIX. Deleted, `src/AIPlayerGPT.cpp:51430-51440`. It is reached only with a
NON-EMPTY cast menu, and `loopAutoPassFor` passes only when
`hasAnyLegalAction(p, GptManaPolicy)` is false — so it is dead or it auto-passed a
rendered cast row. `LegalActions.cpp` prices with its own mana computation, so
agreement is not by construction. The priority-seam call is the legitimate one and
stays.

---

## B + C. Deletion list

`src/AIPlayerGPT.cpp` **63,671 → 62,979** (−692).
`include/AIPlayerGPT.h` **2,813 → 2,818** (+5: the members and maps deleted are
outweighed by `CrackBackFacts` and the invariants written where false comments
stood — the header got smaller in state and larger in stated contract).
`src/AIPlayerGPTSelfTest.cpp` 43,140 → 42,804 (−336).
`include/AIPlayerGPTSelfTestAccess.h` 1,147 → 1,138 (−9).
Whole diff vs base: **1,430 insertions, 2,449 deletions** across 7 files.
Comment lines in `AIPlayerGPT.cpp` 22,186 → 21,951.

Functions deleted: `loopCautionForBoards`, `combatDirectiveNegatedOnLine`,
`combatDirectiveExampleEcho` (+ their explanatory block), `askReplayRefuse`,
`loopAutoPassApplies`, `w80CrackBackVerdictLineNow`,
`w80ScriptLineHasActivationCost` (an EXACT duplicate of `lineIsActivatedCost` —
same automaton, only the flag name and comments differed), `lastOfferClause`,
`declineBoardScope`, `fetchLineKey`, `identicalInterchangeableRows`,
`w80SingleOutcomeRepeatMenu`, `writeSingleOutcomeMenuRecord`,
`w79ContinuationDigestOf/Priority/Cast`, `w81CachedReplayMustReask`,
`w80CrossPhaseReplayable`, `w81CastDigestStackTerm`, `noopRowEarnsReask`,
`noopReaskLine`, `w79AskScopeKey`.

Folds: `noAnswerClassFor`'s four overloads → ONE ordered table (the order WAS
their content; the `audit-L A24` precedence — a status at the wall names the
status — is preserved and pinned); `transportOutcomeStamp`'s 3-arg forwarder →
default arguments; `gptRetrySelectArmClose` → its one caller.

Members deleted: `retracted` + the unwritable `retracted_choice` class,
`mAskReplayKey`/`mAskReplayRun`, `mCeilingReasks`, `mStatedStopCount`,
`mPlanEchoCount` (+ `plan_echo_count`), `mIdenticalOptionAsksResolved`,
`mContinuationDigest`/`mNextContinuationDigest`, `mAskScopeDigest`,
`mAskKeyContinuationDiffers`, `mCachedReplayKey/Runs/Reasked`,
`mAskNoopReaskKey`, `mPriorityNoopReaskBoard`, `mCrossPhaseReplayed`,
`mSingleOutcomeMenusAnswered/RowsSpared`, `mFlipDoneCount`,
`mPassDeclineCount/Board`, `mStuckCastLines/Turn`, `mHoldEvents`,
`mActionBeforePlanReplies`, `mActionBeforePlanRejects`,
`mHoldReopenedNewThreat/NewLethal`, `mWallMissEvents/Unrecorded/NoRetry`.
Dead assignments: the duplicate `mPlanSetTurn`/`mCurrentPlan` pair; `consumePlan`'s
discarded `planStart`/`planEnd`; the duplicate `force_close_arms_deferred` gameend
row (it serialised `force_close_arms_refused`).

Zero counters: deleted ONLY under the ledger's rule — a per-record field already
carries the fact. `wall_miss_events`/`_unrecorded`/`_no_retry` (the `wall_miss`
record carries the class per event), `action_before_plan_replies`/`_rejected_replies`
(the per-record fields), `hold_reopened_new_threat`/`_new_lethal`/`hold_events`
(the `hold_event` records). **The force-close counters are KEPT** even though they
are zero: the gameend accounting identity is pinned on them, and the rule is a
per-record join, not a zero.

False comments rewritten to the invariant: `mRetryPark` "never read anywhere
else" (it is read at six sites), `retryArmMatches` "kept as an assertion rather
than the gate" (it is still `&&`-ed into the gate), `reasoning_tail_answers`
DELETED (the counter is; the branch is not), `multi_answer_first_taken` DELETED
(re-added in #W81-DM), the "until c5 moves payment+clickstream" promise
(`planCastSpell` exists and this seam still does not use it), `consumePlan`'s
"find the LAST label" (the selector is first-clean), the rows-and-header mana
"can never disagree", the priority staleness key "is the full prompt".

Epitaph comments: **73 → 48**. 42 comment blocks (201 lines) whose ENTIRE content
was an epitaph for code that no longer exists are deleted. The 48 that remain are
blocks that also carry a live WHY, a rules citation or a doctrine rail; deleting
those would take the invariant with the provenance, and the invariant is what
stops the next wave re-adding what this one removed. This is a deliberate
narrowing of the ledger's item, stated here so the owner can overrule it.

Section C is absorbed: X2 → L2, X6 → L5 (finished in `c6001b432`: the eight
`crackBackScreenTotal(this, …)` sites, each re-walking the whole opponent board,
now read the memoised struct through `crackBackScreenTotalNow`), X7 → L6, X3 →
L7, X4 → L8, the duplicate scanners and forwarders → B. X1, X5, X8, X9, X10 and
Astra's typed-verdict shape are §D by the ledger's own disposition.

REFUTED / KEEP-DORMANT items were not touched: `declineRowAtFact`,
`shortDecodeGarbage`, `w72HeldMenuShowedEveryRow`, `w71EveryBranchIsANoOp`,
`wagicGptParseSelfTest`, `revealSinglePickDeclineLegal`, `gptNoteEngineRevealFloor`,
`noteRevealStall`, `asyncDropTraceLine`, and the whole KEEP-DORMANT card-class
list.

---

## Gates

### (a) Test suite — PASS

`systemd-run --user --unit=gpt-cleanup-suite … WAGIC_TESTSUITE_THREADS=1`, waited
with a foreground until-loop (no Monitor), run on the `make -B` binary.

```
Test suite finished: 1312 tests (0 failed), 81 AI tests (0 failed)
```

Baseline 1312/0 + 81 AI/0 — matched exactly. Both failure strings zero:
`grep -c 'Test Failed'` = 0, `grep -c 'FAILED'` = 0. None of the three known
flakes (`lifeline`, `merrow_reejerey`, `intrepid_adversary_repeated_payment`)
fired, so no solo rerun was needed.

### (b) PARSETEST — 0 failed

**7,275 → 7,210 passed, 0 failed.** The net −65 is two movements, as the brief
anticipated:

FELL — pins of code this lane deleted, removed with it: the `[#W47-R4]`
last-offer block, the `[#W65-AM G7]` decline-scope block, `[W54-D D8b]`
(`identicalInterchangeableRows`), `[#W80-DF U13]`
(`w80SingleOutcomeRepeatMenu`), `[#W69-BH K4c]` (`noopRowEarnsReask`),
`[#W66-AS H3b]` (`loopAutoPassApplies`), the `[#W81-DI]` replay-bound and
object-count-digest sections, `[#W81-DK V4]` (the cast digest's stack term), the
`#W71-BP L1c` / `#W72-BT M22` unscoped-`askReplayRefuse` cases, the `#W72-BT M2`
and `#W73-BZ N15` `noopReaskLine` cases, and the `#W80-DE U8` cross-phase
replay-rule cases.

ROSE — new `#W82-A` pins: the L1 doctrine block (no row advertises a
retirement; the HOLD row is the only stop), the L2 board-half block (two windows
differing only in a board fact key UNEQUAL; two identical windows key EQUAL; both
halves load-bearing; the turn/phase live inside the board half), the lane-DC
replacement block (a second legal target, a grown X ceiling and a changed cost
all key unequal with no digest to notice them), the cross-phase key pair, the L5
`ONE TOTAL` block (blockable vs unblockable addenda, the unsized hedge, the
compulsory-draw-only lethal, the marker moving with the published total, and the
`162v146` seq-30 board), the L8 label-less-with-prose cases, and the L9
inversions (twelve renders).

### (c) Static gates — PASS

* `check-ctor-init: OK (119 file(s))` — run by the Makefile on every link; every
  new header scalar (`mPriorityTurnSeen`, `mCrackBackFactsSeq`,
  `mAsyncDropStampsSeq`, `mAbandonedInFlightSeq`) is in the ctor list.
* `check-reply-instructions: OK (1 source file(s), 37 guide(s))` — the reply
  protocol text and the 37 deck guides are untouched by this lane.
* `git diff | grep -c $'\357\277\275'` = **0** — no U+FFFD anywhere in the diff.
* The protocol text (`bin/Res/ai/gpt/reply-protocol.txt` / `kReplyProtocol`) and
  the thinking regime are byte-identical. The one `bin/Res` edit is
  `system_prompt.txt` line 3 — the PRIMER, not the golden protocol — and it adds
  the vigilance exception the CR requires (L9).
* Harness `--selftest` / `corpus-stats --selftest` not run: this lane touches no
  tool under `projects/mtg/tools/`.

### (d) Heuristic A/B — PASS

`tools/baka-ab.sh <worktree>/projects/mtg/bin ~/.gatelogs/gpt-cleanup-baka 10 4`,
compared against `archives/baka-ab/62a5733ec`. Two independent samples were taken
because the first flagged one deck; the reason to take a second is below the
table.

| deck | accepted | sample 1 | z | sample 2 | z |
|---|---|---|---|---|---|
| 123 | 77/180 | 33/60 | +1.65 | 33/60 | +1.65 |
| 125 | 66/180 | 21/60 | −0.23 | 18/60 | −0.94 |
| 126 | 105/180 | 38/60 | +0.68 | 39/60 | +0.91 |
| 130 | 48/180 | 13/60 | −0.77 | 14/60 | −0.51 |
| 146 | 91/180 | 21/60 | **−2.09 FLAG** | 25/60 | −1.19 |
| 152 | 122/180 | 40/60 | −0.16 | 37/60 | −0.87 |
| 162 | 121/180 | 44/60 | +0.88 | 44/60 | +0.88 |
| | | 210 games, **0 unfinished** | PASS (hard) | 210 games, **0 unfinished** | PASS |

The hard gate — **0 unfinished, both samples, 420 games** — passes. The deck-146
flag in sample 1 is sampling noise, and the argument is not "z is only 2.09": it
is that the heuristic path is provably byte-identical. This lane's only change
outside the GPT layer and `DecisionContract.cpp` is one token at
`AIPlayerBaka.cpp:3534` — `!aiForcedCandidate` added to the land-face skip — and
`aiForcedCandidate` is written in exactly three places: `AIPlayerBaka.cpp:5783`
(`= NULL`) and the two GPT seams (`AIPlayerGPT.cpp:49687`, `:51459`). In a
Baka-vs-Baka game it is NULL at every evaluation, so the guard is always true and
the branch is the one that shipped. Corroboration from the runs themselves: three
samples of that provably-identical path put deck 146 at z = −0.75, −2.09 and
−1.19, and sample 1's other movement (deck 152 at −1.79) did not reproduce
either — the flag moves decks between runs, which is what noise does and what a
behaviour shift does not.

### (e) LLM-seat live probe — PASS (the gate that matters for this layer)

Two full games on the pilot, `qwen36-35b-a3b` at `http://100.116.136.74:8084`,
`--thinking on -r 1 -T 0 -j 1 -p 152,125`, each a detached
`systemd-run … -p MemoryMax=12G -p MemorySwapMax=0` unit waited on with a
foreground until-loop polling every 60 s. The regime gate passed on both
(`thinking=on: 10 records, all carry reasoning. shapes: two_line=10`).

| | game 1 | game 2 |
|---|---|---|
| result | deck152 beats deck125 | deck152 beats deck125 |
| life at end | 36 vs **−5** | 23 vs **−2** |
| turn | 30 | 14 |
| natural end? | yes — opponent dead | yes — opponent dead |

Neither game was answered by heuristics: `fallbacks ALL kinds {}` and
`engine_answered 0` across both.

Census over `~/.gatelogs/gpt-cleanup-probe*`:

```
dirs 4 clean seat logs 4 = games 2
ALL KINDS {'gamestart': 4, 'system': 4, 'ask': 121, 'priority': 34,
           'forced_close': 1, 'gameend': 4, 'bottom': 2, 'attackers': 15, 'reveal': 3}
RECORD KINDS SUM (all kinds except gamestart/system/gameend) 176
  ...of which forced_close  1
  ...of which engine_answered 0
fallbacks ALL kinds {}
fallbacks (ask seam) {} total 0
ask records 121 model decisions 121
with reasoning 121 thinking values {'on'}
CORPUS-WIDE (all kinds): model decisions 175 off_protocol_bytes>0 3
                         plan_line_missing 3 latency p50 31.9 s
ASK-SEAM ONLY: reply_truncated 0 off_protocol_bytes>0 3
               plan_line_missing 3 action_before_plan 0
latency p50 31.3 s p95 55.9 s n=121
reasoning_chars median 5564 max 16551
protocol_deviation classes {'compliant': 172, 'unlabelled_plan': 3}
decisions/turn 1.38
CENSUS CHECK: gameend protocol_replies 175 vs record-kind sum 176
              (difference 1 = forced_close 1)
CENSUS CHECK RECONCILES.
WINS per deck: deck152 2/2 deck125 0/2
```

Against the gate's four conditions:

* **0 ask-seam fallbacks** — `fallbacks (ask seam) {} total 0`, and zero
  fallbacks of any kind corpus-wide.
* **`reasoning_content` on every record** — `with reasoning 121` of
  `ask records 121`, `thinking values {'on'}`.
* **`plan_line_missing` in the wave-80 band (≤ ~5 %)** — 3 of 175 = **1.7 %**
  (wave-80 corpus: 62 of 2,135 = 2.9 %).
* **0 records classed unparsed where the reply carries an unambiguous answer** —
  zero `unparsed_reply` fallbacks; the only deviation class is `unlabelled_plan`
  ×3, which is a PLAN-line shape, not an unread answer, and all three were
  answered.

Three gameend counters are worth naming because they are live evidence for the
fixes rather than an absence of regressions:

* **`phase2_answer_recovered: 1`, `phase2_answer_missing: 0`,
  `forced_close_unrecorded: 0`** — one forced close armed, one answer RECOVERED.
  In the wave-80 corpus this pair was 0/0 "by construction while flaw F2 stands"
  (13 armed, 13 dropped, 0 consumed). This is **L7 working on a live board**: the
  leg survived the re-render because it is slot-keyed, and the phase-1 decode the
  model had already been paid for was spent instead of thrown away.
* **`crossphase_identical_reputs: 0`, `crossphase_board_unchanged: 0`** — the
  cross-phase replay branch is gone (L2), and nothing tried to reach it.
* **`menu_pass_no_progress: 0`, `menu_pass_no_progress_suppressed: 0`,
  `async_drops: 0`** — no livelock and no stale-answer drop over two games, which
  is the direct question hanging over L1's removal of `mStuckCastLines` and L2's
  board-in-key (both of which make an unchanged board replay rather than spin).
  `ask_replays_cache: 12` / `identical_ask_answers_reserved: 2` show the cache is
  still doing its job over genuinely unchanged windows — the board-in-key
  tightened reuse, it did not abolish it.

---

## Weakest evidence

The single weakest claim in this lane is that **L1's withholds are safe to remove
in the long run, on boards these two games did not reach.** Two games is a small
sample for a property whose failure mode is a LIVELOCK, and a livelock is exactly
the class that a short run cannot distinguish from its absence. The wave-65
comment I deleted records that the pass-decline cap was built because a held
fetch-crack was re-asked at 44–97 windows a game, and the header comment I
deleted records the 135v133 wedge (turn 2, 2400 s, 903k re-picks). My argument
that those cannot return is a CODE argument, not a measurement: the board is
inside the ask key now, so an unchanged board replays a cached answer with no
round trip; the priority-seam deadlock breaker (kept, deliberately, against the
ledger's listing) still forces a pass on an unchanged key over an unmoved board;
and the cast seam's new window-level `mCastNoProgressBoard` defers one window
after a no-progress pick. Two clean games at 1.38 decisions/turn with
`menu_pass_no_progress: 0` are consistent with that argument and do not prove it.
The thing that would prove it is a full corpus. **If any later run shows a window
count per turn climbing or a game failing to end, `mCastNoProgressBoard` and the
deadlock breaker are the two places to look first, and the honest fallback is a
window-level bound, never a return to withholding rows.**

Four more, in descending order of how much they should worry a reader:

1. **The L1 doctrine items have no PARSETEST pin, by construction.** Nine of the
   ten shapes were a `continue` or an early `return` inside a seam — there is no
   pure surface to pin, and after the fix there is no code to pin either. Their
   evidence is (i) the deletion itself, (ii) the pins of the *clauses* that
   advertised them, which now assert those clauses are not producible, and (iii)
   the probe. A reviewer who wants a stronger instrument would have to drive each
   shape through a `WAGIC_GPT_STUB` board that forces it — an ETB pay-or-tap on an
   already-tapped land, an `<upto:N>` chooser, a 40+-candidate library search —
   and I did not build those boards.
2. **L5's joint-affordability budget is a new heuristic, not a measurement.**
   `crackBackRungConvertedCost` parses a rendered cost string, and the budget is
   the largest per-card next-turn reach seen in the walk, which slightly
   OVER-estimates (it does not exclude each admitted card's own mana from the
   remaining budget). It replaces an unbounded sum with a bounded one, which is
   strictly closer to true, and it can still over-claim. It fired on no board in
   these two games (`crackback_verdict_folded_total: 0`), so it is untested live.
3. **L9's double-strike fix is untested on a real board.** No double striker
   attacked a GPT seat in the wave-80 corpus and none appeared in these two games.
   The change is a `*= 2` mirroring what the attackers seam has done since #W64,
   with CR 702.4b cited, and it is pinned nowhere because the blockers seam's
   arithmetic is not a pure function.
4. **The epitaph sweep is a judgment call I made narrower than the ledger.** The
   ledger says delete 73; I deleted 42 blocks (201 lines) and kept 48 epitaph
   lines whose blocks also carry a rules citation, an owner ruling or a doctrine
   rail. If the owner wants the literal 73, the remaining 48 are all findable with
   `grep -nE '^\s*//.*\bDELETED\b'` and each would have to be read for what it
   takes with it.

## If Astra refutes (the ledger is PROVISIONAL)

Astra's cross-review of its own findings runs after this lane. Items that would
have to be revisited, and what it would cost:

* **L2 (the key).** The highest-stakes item and the one with a stated price — the
  233 same-phase re-asks #W79-CZ measured come back. If Astra argues #W79-CZ's
  departure was right, the revert is ONE function (`w82WindowKey`) and its three
  call sites; the deletions that rode with it (digests, census, replay bound,
  cross-phase replay) would each need re-deriving from `f1a722daf`, so revert the
  whole of A rather than unpick it.
* **L5 (e) and (f)** — the blockable/unblockable split and the mana budget are the
  two places I went beyond "make the verdict use the header's total" into
  re-modelling what the number MEANS. Both are CR-anchored (510.1c) but both
  change printed numbers on live boards. Contained in `w81CrackBackVerdictLine`
  and `crackBackFloorSources`.
* **L8's stale-echo re-route.** I widened `stale_echo_in_range` to ALWAYS take the
  `named_row` re-ask lane, including when the name echoes a prior window's row.
  If that population is genuinely a stale echo rather than a disagreement, this
  buys a round trip where the heuristic used to answer. Two lines, both seams.
* **L1's `mStuckCastLines` replacement.** If Astra refutes the livelock reading,
  `mCastNoProgressBoard` is unnecessary and can go; if Astra says the withhold was
  load-bearing, the answer is still not to restore a row withhold.
* **B's zero-counter deletions** rest on the ledger's join rule; if a join is
  refuted, the counter comes back as a one-line gameend row.
* Nothing in A depends on a finding Astra alone made: every A item carries two
  reviewers' verdicts, and the ones Astra could soften (its own F6/F7/F8/F9/F10)
  are all L8/L9 truth fixes that remove no option.

## Left undone, and why

* **`deferSeat` cross-seat telemetry** (`computeActions`): a `defer` record can
  consume the OPPONENT seat's pending members. Real, confirmed, and its fix is
  the per-window record shape the ledger files as §D proposal I8.
* **The pregame bottom ORDER** (CR 103.5): the surface is now truthful, but the
  engine still bottoms in hand order. Honouring the model's order needs the reply
  parsed as an ORDERED list rather than a membership mask.
* **The 12-pick `<upto>` sanity bound** (`picks.size() >= 12`): the ledger's L1
  anchors do not list it, and unlike the 40-candidate cap it is a loop bound whose
  removal risks a hang. Flagged, not touched.
* **X1, X5, X8, X9, X10** and the typed-verdict / render-at-the-boundary shape:
  §D by the ledger's disposition.
