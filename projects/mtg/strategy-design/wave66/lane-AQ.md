# Wave-66 lane AQ — H1, H4, H9, H10 (forecasts and brackets that lie about the number)

Base: master `bdd0ec6e5` (wave-66 step-one brief). Branch `w66-lane-AQ`, worktree
`worktrees/lanes/w66-AQ`. Files changed: `src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`.
Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); **no kill under the cap on
any run**. Gates ran detached (`w66-AQ-gate`, `w66-AQ-gate2`) with a foreground until-loop wait.
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0** after every edit (all splices python
`rb`/`wb`). Every comment tagged `#W66-AQ (Hn)`.

All four items are RENDER / classification changes: none of them changes what the engine does
with a card, an option or an answer, so no suite fixture can be RED on base and none is claimed.
RED was produced the way lanes AL/AG/AH produced it — the mechanisms reverted to their wave-65
behaviour with the new cases already in place, binary rebuilt. See **RED-on-base evidence**.

The one non-render addition is a COUNTER (`receiveEvent` tallies library->hand moves inside a
draw step). It is read by one emitter and by nothing else: no gate, no key, no cache. It cannot
enter `mPromptTail` or an ask key because it is not printed as a running total — H1's line
prints a REMAINDER of a fixed step size, and both terms are facts of the window that two
rebuilds of the same window agree on (wave61/corpus-livelock.md).

---

## What shipped

### H1 — the DRAW FORECAST charges the draws STILL AHEAD (engine HIGH-1, deck152 HIGH-2)

`123v162` seq **138-153**: sixteen consecutive windows inside ONE draw step render the identical
`your draw step, resolving NOW, draws 6 cards (...) = 6 x 1 = 6 life LOST BY YOU ... you would be
at 1` while the seat's life falls 7 -> 3; at seq 152 (life 4, ONE point still owed) the same line
reads `you would be at -2; that KILLS you`. The seat lived at 3 and won. `152` seqs 41-49 read
`draws 4 cards = 4 x 4 = 16` beside `ON THE STACK: 7 damage`.

The step's SIZE is still printed and nothing is deleted; the PRICE is charged on the remainder.

* `AIPlayerGPT::receiveEvent` counts library->hand moves for whichever seat's draw step is
  running (`observer->currentPlayer == the zone owner`, phase `MTG_PHASE_DRAW`, `mDealDone`),
  keyed on `observer->turn` — three new members, all NSDMI, all read-only outside that hook.
  A stale turn key reads 0, so a step that has not begun subtracts nothing.
* `drawsStillAhead(stepSize, resolvedInStep, stepIsNow)` — pure, clamped at both ends, and inert
  unless `stepIsNow`. A forecast of a step that has not started is byte-identical to wave 65.
* `drawsResolvedClause(stepSize, remaining)` — the sentence that states the subtraction, and
  points at the narration the same prompt carries (`they are in the log above`), so the model can
  check it: seq 152's narration lists the drawn cards by name.
* Both emitters (`drawStepForecastText`, `theirDrawStepForecastText`) charge `left x perDraw`,
  say `if the rest resolves as forecast` once anything has resolved, and — when the whole step
  has resolved — print `= 0 life LOST BY YOU ... from the rest of this step` and **no life total
  and no KILLS verdict at all**, because there is nothing left to forecast.

### H4 — the X ladder starts from the life the STACK leaves (deck125 HIGH-1)

`125v162` seq **36** printed, in one prompt, `ON THE STACK: 13 damage to you - you would be at
-8; that would KILL you` and `X=4 is the largest listed X whose NET (-4) leaves you alive, at 1`.
The badge's base was the printed life total; the post-stack one was three lines above it.

* `lifeAfterPendingStack(life, stackLossToMe)` is the ONE term. `pendingStackDamageLine` now
  subtracts through it (byte-identical output), and so does the X badge — the two surfaces
  cannot disagree by construction.
* `pendingStackLifeLossToSeat(observer, seat, exclude)` sums the pending loss from the SAME walk,
  the same per-object reader (`stackObjectLifeLossToSeat`) and the same filters (`NOT_RESOLVED`,
  spell-or-ability, `stackObjectIsRespondable`) the `ON THE STACK:` total is built from.
  `exclude` drops the announcing card's own objects, so a spell on the stack does not price
  itself at the ANNOUNCE_X seam.
* `xNetLadder` searches for `safeX` against the post-stack life; `xMonotoneMarker` states the
  verdict from it and NAMES the term (`counted from the -8 life the 13 damage ALREADY ON THE
  STACK leaves you on`). Where the stack alone is lethal it says so
  (`the stack alone puts you at -8, whatever you announce`) instead of naming a survivable X.
* Hardening the codex review would otherwise find: the badge now refuses to restate a `safeX`
  that does not itself survive the base it is printing (`xLifeBase + netAtSafeX > 0`), so a
  ladder computed against one life total can never have its survival claim reprinted against
  another.
* With `stackLossToMe == 0` every byte is wave 65's — pinned as a NEGATIVE case.

### H9 — one mode census on both seams, and the punisher folded into the mode price (deck146 HIGH-1/2)

`146v162` seq 23's CAST row named seven of Silverquill Command's nine pairs dead. Seq 24 — the
ask the seat answers — tagged **none** of them, priced `you draw and sacrifice creature` and
`opponent draws and sacrifice creature` (both HALF dead: the sacrifice half had no legal object)
as whole modes, and gave rows 1 and 4 no tag at all. The seat took row 6 and lost the game.
Same shape at `146v126` s61->62. And the mode price said `you would be at 17` where Underworld
Dreams — printed in the same prompt, and already folded by the cast row's DRAW GRANT bracket —
makes it 16.

* **Third bucket.** `modalChoiceModes` now records whether each half of a pair exists and whether
  the untargeted half carries payload (`outerPayload` / `subPresent`). `modalModeVerdict` is the
  pure rule: nothing dead = LIVE, something dead with something live = HALF DEAD, otherwise DEAD.
  `modalModesTag` prints the third bucket and OMITS it when empty, so a board with no half-dead
  pair renders exactly as wave 65 wrote it; `modeLivenessRowTag` is the row form, with wave-65's
  two strings byte-identical.
* **The seam the corpus exercises.** The mode ask arrives as CHOOSE_MENU with `req.contextCard`
  NULL (#W65-AL measured this), which is why the census never reached it. The subject is
  recovered by matching the arm-time SCRIPT against `magicText` across the seat's public zones
  (`modalSubjectFromScript`) — an identity test, not a name match. Not found = no census
  rendered, which is wave-65 behaviour, never a guess. Both menu seams tag rows from the same
  census function.
* **The punisher term.** `modeEffectPriceTag` takes the opponent's per-draw punisher rate
  (`drawPunisherScan`, the scan behind the DRAW PUNISHERS paragraph the same prompt prints) and
  folds it into the mode's own life figure and its lethality verdict:
  `you LOSE 1 life, and their draw punishers take 1 more for the 1 card it makes you draw - you
  would be at 16`. A mode that draws THEM a card is never billed to the seat (pinned as a
  MUST-NOT-MATCH); rate 0 is byte-identical to wave 65.
* Presentation only throughout: `req.optionTexts` (the staleness key) and the row ORDER are
  untouched, and `{HALF DEAD right now` joins `stripNarrationDecoration`'s drop list beside the
  two census tags already on it.

### H10 — the two body-count brackets name the ENGINES they are counting (deck130 HIGH-3, deck162 HIGH-2)

`130v123` seq 44: `board sweep: THEIRS 1 / YOURS 2 ... (it takes more of YOURS than of THEIRS)`
where THEIRS-1 was Bloodline Keeper under Intruder Alarm; the seat cycled Starstorm and died at
-78 to 40+ vampires. `162v126` seq 15: the forced-sacrifice header's `the one that pays the
least` pointed at Fate Unraveler while the same prompt listed Fate Unraveler under
`DRAW PUNISHERS on the battlefield: yours`.

* `engineKindForScript(magicText)` asks only detectors the prompt ALREADY runs: a repeatable
  token maker (a `token(` that hangs off a cost or a trigger — the `{T}:token(Vampire...)` on
  both faces of Bloodline Keeper; a one-shot ETB `token(...)` with no `:` is a body's arrival and
  is NOT an engine), `drawPunisherClause`, and the two life-loop halves
  (`lifeToDamageConverterScript` / `lifeLossMirrorScript`). Pure over the script text.
* **Sweep bracket.** `boardCreatureCounts` fills an engine roster from the same walk that builds
  the victim roster (so the survivors split is honoured), it rides `CastRowBoardAnswer` to
  `applyBoardSweepMark`, and the marker prints
  `board sweep: THEIRS 1 (including Bloodline Keeper - a TOKEN ENGINE ...) / YOURS 2 - ... (it
  takes more of YOURS than of THEIRS). THAT COUNT IS BODIES, NOT VALUE: ...`. The tally is NOT
  re-weighted and no row is re-ranked — inventing a value weight would be a claim the board does
  not support; what changes is that the comparison now says what it is comparing.
* **Forced sacrifice.** The rows that are engines carry `; THIS IS NOT JUST A BODY: <kind>`, and
  the header SCOPES its toughness tie-break off them (`NOT ALL OF THESE ARE BODIES: ... read that
  tie-break as applying to the rows that are NOT named here, and if you sacrifice one that is,
  say in your PLAN that you are giving up that engine on purpose`) — the brief's "exclude a named
  punisher/engine or say it is sacrificing one", answered with both halves. No row is removed and
  no row is re-ordered.
* With no engine on either list both brackets are byte-identical to wave 65, pinned as NEGATIVEs
  (the sacrifice ask additionally pins that the new sentence is only ever APPENDED to the
  wave-65 string).

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), qmake untouched, no new sources.
Detached units, memory-capped, foreground until-loop wait.

| leg | result |
|---|---|
| build | 0 errors |
| PARSETEST | **4748 passed, 0 failed** (base 4698/0 — **+50 cases**) |
| suite, `WAGIC_TESTSUITE_THREADS=1` (ground truth) | **1271 tests (0 failed), 67 AI tests (0 failed)** |
| `==Test Failed !==` / `==Test timed out` (T1) | **0** / **0** |
| suite, default threads (run 1) | **1271 tests (2 failed), 67 AI tests (0 failed)**, 0 timeouts — `lifeline.txt`, `merrow_reejerey.txt` |
| suite, default threads (run 2) | **1271 tests (3 failed), 67 AI tests (0 failed)**, 0 timeouts — the same pair plus `intrepid_adversary_repeated_payment.txt` |

**0 new failures.** Both default-thread runs hit only names the brief already lists: the
concurrency-only pair (`lifeline`, `merrow_reejerey`) in both, and the brief's declared genuine
intermittent (`intrepid_adversary_repeated_payment`) in the second. All three pass at
`WAGIC_TESTSUITE_THREADS=1`, which is ground truth, in both runs. The intermittent's RATE was not
measured here.

### RED-on-base evidence — measured, not asserted

Mechanisms reverted to wave-65 behaviour with the new cases in place, binary rebuilt:
`drawsStillAhead` returning the whole step and `drawsResolvedClause` returning ""; the X badge's
and `xNetLadder`'s stack term forced to 0 (the SHARED term left alone, so wave-65's
`pendingStackDamageLine` cases stay green — they are controls here, not reds);
`modalModeVerdict` collapsed to wave-65's two buckets, `modeLivenessRowTag(1)` and the
half-dead bucket silenced, `punishLoss` forced to 0; `engineKindForScript` returning NULL and
both H10 bracket clauses silenced.

Result: **`4727 passed, 21 failed`** (log `~/.gatelogs/w66-AQ-red.log`) — **H1 x7, H4 x3,
H9 x6, H10 x5, and nothing else**: 4727 + 21 = 4748, so **no pre-existing case goes RED under
the reversion** and none was deleted or re-pinned. The other 29 new cases are NEGATIVE,
MUST-NOT-MATCH, ECHO-shape and control pins that pass on base too, which is what they are for.

---

## Predictions (falsifiable, for the wave-66 corpus)

1. **H1** — **0** runs of two or more windows inside ONE draw step render the same
   `DRAW FORECAST: your draw step, resolving NOW` line with the SAME `= N x M =` product while
   the seat's rendered life differs between them (wave 65: 12 runs of >= 3 identical forecasts
   across 2 seats, longest 10). Every such window after the first draw of that step carries
   `have ALREADY been drawn and paid for in this step` (or `has`, at one), and **0** windows
   carry `that KILLS you` on a forecast whose remaining product is smaller than the seat's
   rendered life. **Falsifier**: one repeated identical product across two windows of one draw
   step at two different life totals, or one KILLS verdict a life-total read of the same window
   contradicts (the `123v162` seq 152 shape).
2. **H4** — **0** windows carry both an `ON THE STACK: N damage to you` line and a
   `leaves you alive, at K` clause whose K is not `life - N + NET` (wave 65: 5 of 8 stack-damage
   X windows). Every X badge on a window with pending stack damage to the seat carries
   `ALREADY ON THE STACK`. **Falsifier**: one survivable-X claim computed from the pre-stack
   life, or one `ALREADY ON THE STACK` clause on a window whose stack block is absent.
3. **H9** — every rendered row of a modal `auto=choice name(...)` ask (`Choose an option for
   <card>:` / `Choose one mode for <card>:`) whose label matches a `name(...)` in the arming
   script carries exactly one of the three census tags; **0 such rows render with no census tag**
   (wave 65: 6 of 6 rows on 3 windows). A pair the CAST row's census one window earlier put in
   its dead or half-dead bucket never carries `{this mode has a legal object right now}` at the
   ask. And **0** mode price tags on a window carrying `DRAW PUNISHERS on the battlefield:
   theirs` state a `you would be at` figure that omits the punisher rate. **Falsifier**: one bare
   modal row, one census disagreement between the two screens in one game, or one unfolded
   figure.
4. **H10** — **0** `[<- board sweep:` markers whose THEIRS side includes a permanent
   `engineKindForScript` matches render without `(including ` and `THAT COUNT IS BODIES, NOT
   VALUE`; **0** `FORCED SACRIFICE` asks listing such a permanent render `the one that pays the
   least` without `NOT ALL OF THESE ARE BODIES`, and every such row carries `THIS IS NOT JUST A
   BODY`. Both bracket families are byte-identical to wave 65 on boards with no engine.
   **Falsifier**: one of any. Secondary (behaviour, not a render fact): a sweep row whose THEIRS
   count is 1 and whose named body is a token engine is taken at least once, or the seat's
   forced-sacrifice pick avoids a named engine when a non-engine row exists.

---

## What I did NOT verify

- **No live model probe was run and no game was driven.** PARSETEST, the suite and source reading
  are the whole gate; nothing here has been seen by a model, so every behavioural prediction is a
  prediction, not a measurement.
- **H1's counter is not fixtured.** `receiveEvent` has no PARSETEST reach (it needs a live
  observer and a WEvent), so the SUBTRAHEND is verified by reading only: the emitters and the
  arithmetic are pinned, the tally that feeds them is not. Two specific unverified edges: (a) a
  draw that happens during a draw step from a source that is NOT the step (an instant cast in the
  seat's own draw step) is counted as a resolved step draw and would make the remainder too
  small — the clamp keeps it non-negative and the sentence points at the log, but I did not find
  or construct such a window; (b) a SECOND draw step inside one `observer->turn` (an extra-turn
  or extra-draw-step effect) would keep accumulating against the same key. Neither shape appears
  in the wave-65 pool; both would show up as an UNDER-charge, never as a phantom KILLS verdict.
- **H4's live sum is verified by construction, not by measurement.** `pendingStackLifeLossToSeat`
  re-walks the stack with the same filters the situation block uses; I read both walks and they
  agree line for line, but there is no fixture that runs them over one board and compares the two
  numbers, because the situation block's total is computed inside the render loop. If the two
  ever drift, the badge and the `ON THE STACK` line will disagree — that is the falsifier in
  prediction 2.
- **H9's subject recovery is the weakest evidence in this lane.** `modalSubjectFromScript` is
  argued from #W65-AL's measurement (the CHOOSE_MENU arrival with a NULL pointer and a populated
  `contextText`) plus the corpus narration showing Silverquill Command in the graveyard at the
  ask, and from reading `MTGGameZone::removeCard`'s clone path. It is NOT measured: there is no
  endpoint in the suite, so `chooseMenuAction` cannot be driven. If the clone does not carry
  `magicText`, or the subject is in a zone I do not search, the census renders NOTHING — wave-65
  behaviour, not a false surface — and prediction 3's first falsifier fires on the first modal
  window of the corpus.
- **The half-dead rule is proven on ONE real primitive** (Silverquill Command, the pool's only
  `auto=choice name(...)` list carrying both a targeted and a granted half) plus synthetic
  shapes. `modalChoiceModes` reads at most one outer and one sub spec per line, so a card writing
  three requirements on one `choice` line would be classified from two of them.
- **The punisher fold counts only the OPPONENT's rate on the seat's own draws.** A mode that
  makes the OPPONENT draw under the seat's OWN punishers is not folded (the figure it would move
  is theirs, and the tag already prints their life separately); I did not extend the symmetric
  half, and no corpus window needed it.
- **H10's token-engine test is a script-shape test, not a semantic one.** Any `token(` with a
  `:` earlier on its line counts, so a triggered token maker that can fire once per game reads as
  an engine. That is generous in the direction that adds a true fact to a row and never removes
  one, but it is not a claim about repeatability rate, and the tag's wording ("one per
  activation") is the token-engine branch's wording for all of them.
- **The sweep roster is the SWEPT set, not the whole board.** Engines that survive the sweeper
  (indestructible under a destroy) are excluded with the bodies, which is correct for the
  bracket; I did NOT add an engine census of what the sweep LEAVES behind.
- **No cross-lane interaction was tested.** Lanes AR, AS and AT touch adjacent surfaces (AS owns
  the hold key and the repeat row's `{right now:}`, AT owns target rows and the discard ask, AR
  owns the reply rule). The combined diff has not been built here. Three merge-time looks are
  worth it: `stripNarrationDecoration`'s drop list (one entry added), the `modeRowAnnotations`
  signature (AT's MED item also touches the mode menu), and the PARSETEST corpus tail, where
  every lane appends.
