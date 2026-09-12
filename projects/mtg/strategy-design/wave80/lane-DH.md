# Wave 80 — Lane DH: the Astra review, adjudicated

Base: `288556656` (master after the wave-80 merge repair). Every finding below is
CONFIRM or REFUTE; F1 is the GUARD, the nine dropped initializers having been repaired
by the orchestrator before this lane launched.

**Twelve findings, twelve CONFIRMs, zero REFUTEs.** Astra's review was source-traced and
every trigger it named is reachable on the shipped code. What is qualified rather than
refuted is the *consequence* of two of them (F2's population is empty in three waves of
corpora; F12's divergence is latent until the two ordinals actually part), and both are
said so below rather than dressed as wins.

Gate: PARSETEST **7086 passed / 0 failed** (baseline 7035 — +51). Suite THREADS=1
**1296 tests (0 failed), 76 AI tests (0 failed)**; `==Test Failed !==` 0 and
`Test timed out (game never ended)` 0. `bash -n selfplay-harness.sh` OK,
`selfplay-harness.sh --selftest` 30 checks / 0 failed, `corpus-stats.py --selftest` OK,
`check-reply-instructions.py` OK, `check-ctor-init.py --selftest` OK,
`check-ctor-init.py src` OK. `git diff | grep -c U+FFFD` = 0.

---

## F2 — HIGH — a labelled answer PRECEDING the plan was accepted — **CONFIRM**

**RED.** `gptAnswerSegment` (the selector `consumePlan` calls) finds the labelled answer
in `CHOICE: 2 (Cast Wall of Omens)\nPLAN: block and draw.`, and `findPlanMarker` returns
the marker *after* it — so `consumePlan` returned the choice and the ordering was
recorded as telemetry (`action_before_plan`) and nothing else. The owner's rule is "any
plan, must precede action"; the label-less reader has enforced it since #W79-DD, the
labelled walk never did.

**FIX.** `w80AnswerPrecedesPlan(labelLineStart, planMarkerPos)` is one predicate over the
two offsets the live scanners produce, and `consumePlan` calls it: an answer line above
the plan marker drops the answer and the window takes the path a missing answer takes.
The PLAN is still folded — a plan the model wrote is a plan — and the parser's *tolerance*
is untouched: the scanner still finds the line in both orders, so "make the parser robust"
and "plan before action" are two separate rules and stay two separate rules.

**GREEN.** 5 pins: the flipped reply is an order violation, the ordered one is not, lane
DD's label-less-answer-after-the-plan case stays read and unviolating, and a reply missing
either part is not an ordering violation.

**PREDICTION.** `action_before_plan_rejected_replies` (gameend) == the number of records
carrying `action_before_plan_rejected: true`. **It will read 0** unless the model produces
the shape — `action_before_plan_replies` is 0 across all 42 wave-79 seat logs and has been
for three waves. This is stated as the brief requires: a non-zero here would be a *new*
model behaviour, not evidence the fix works. What *is* falsifiable now: if the counter is
ever non-zero, every increment has a record naming the window.

## F3 — HIGH — a hold survives a SECOND lethal stack — **CONFIRM**

**RED.** Pinned on the shipped key builders: `stackDeathVerdictKey(3,3)` for the first Bolt
and for the second are byte-identical and both rank 2, and `w79HoldVerdictForCompare`
returns the held face. Rows unchanged ⇒ latch stands ⇒ the window is never put. Rendering
the lethal paragraph (DE's U2) does not help: the suppressed window carries no prompt.

**FIX.** A lethal face is an EVENT, not a rank. `takeHold` records the **object identity**
behind each of the two lethal families (`w80CrackBackThreatIdentityNow`,
`w80StackThreatIdentityNow` — the attacking bodies and the stack entries that make the face
lethal, by object, sorted); the hold check re-opens whenever the live face is top-rank and
that set differs. The identities are compared internally only — they enter no key, no row,
no prompt and no record, so the wave-74 rule is untouched and no held row set moves.

**GREEN.** 6 pins on `w80NewLethalThreat`: different objects re-open; the same objects do
not (T3's saving stands); a non-lethal face never re-opens here; an empty live identity
claims nothing; both lethal families are covered and the own-loop ladder is not.

**PREDICTION.** `hold_reopened_new_lethal` > 0, with one `kind: hold_event` record per
increment carrying `what: reopen_new_lethal`, `face`, `seam` and `window_seq`. Wave 79
clamped 59 times with no per-record trace at all, which is why the 0 on
`hold_reopened_new_threat` had to be called UNTESTED.

## F4 — HIGH — the rendered crack-back verdict converts unblocked power into death — **CONFIRM**

**RED.** Astra's board driven through the engine's own DP: 4 life, their ordinary 5/5,
your untapped 0/6. `crackBackVerdictKey(1,5,4)` ranks 2 (LETHAL) while
`w79BestBlockDamage` — the blockers header's own search — returns **0**. The wave-80 line
said "a hold here is taken over a board that kills you next turn".

**FIX.** The MARKER stays a threshold (the latch is untouched); the LINE is blocker-aware.
`w80CrackBackBestBlockFloorNow()` runs the same DP over the crack-back's own attacker walk
and the seat's bodies that can still block on their turn, and the line names the floor
("with your best block: N"). `-1` (no blockers, an ability outside the DP's model, a combat
too wide) leaves the wave-80 wording, so the change can only withdraw a false death claim,
never manufacture a false survival one.

**GREEN.** 7 pins: Astra's board is not lethal on the line and the line names its floor; a
board the best block does NOT save still says so; the no-floor branch says it subtracts no
blockers; the latch's marker is unchanged; all four line faces are distinguishable on the
record.

**PREDICTION.** `crackback_lethal_blocked_away` > 0, incremented **at the send** inside
`w80ApplyVerdictFacesAtSend`, with per-record trace `crackback_verdict` =
`[crack-back verdict: LETHAL unblocked, blocked away]`. It cannot exceed
`crackback_verdict_lines_rendered`.

## F5 — HIGH — U1 retires the cast latch at the FIRST completing menu — **CONFIRM**

**RED.** Pinned by walking the state machine the two live sites run, in their order: with
a Starstorm cast committed, the cast-mode menu sets latch 2 and its record cleared it to
0, so the X announcement one window later read `castPendingCompletion == false` and
`w80CarriedPlanSteps` returned `then attack with Rorix in second main phase.` — the cast
clause deleted at the very window being asked to set its X. That is the defect U1 exists
to fix, reappearing at the second menu.

**FIX.** The latch retires on the **cast event**, not on a menu answer: `receiveEvent`
closes it when the committed spell reaches or leaves the stack (the leave half is the
countered/fizzled case), with a turn-boundary belt for a cast the engine cancelled. A
completing menu now drops the latch back to 1 instead of to 0. Matched on the committed
card's display name because this engine clones a card instance on every zone move.

**GREEN.** 4 pins: both completing menus of one cast keep the clause; neither consumes a
second plan step; an ordinary window between them echoes exactly what wave 79 echoed and
does not retire the cast.

**PREDICTION.** `plan_cast_steps_closed` > 0 and equal to the number of committed casts;
per-event trace is the stderr line `the open cast plan step closed - <the spell reached the
stack | the spell left the stack | turn changed with the cast never completed>`. Lane DG's
own prediction (`dropped after its X was announced` = 0) must now hold at ANY completing
menu, not only the first.

## F6 — MED — "live starter" meant script substring — **CONFIRM**

**RED.** `w80LiveLoopStarterName` returned the first battlefield card whose script
mentions `life:`/`damage:`, with no legality, timing or event test.

**FIX.** The line's SHAPE decides which question is asked: an ACTIVATED line is gated on
`LegalActionsOracle::hasUsableAbility` (the engine's own "could legally use AND afford
right now", the same gate the human's ability border is drawn from); an ENTERS trigger is
gated on the seat actually being able to put a creature onto the battlefield (the same
payment test the hand branch uses); any other trigger belongs to the turn cycle and stays
live. With no live starter the line prints the GAP — DE's U10 wording, unchanged.

**GREEN.** 7 pins: a tapped/unaffordable Prodigal Sorcerer is not a starter and an untapped
affordable one is; Soul Warden counts only while a creature can enter; a cycle trigger stays
live; a line that moves no life is no starter.

**PREDICTION.** `own_loop_verdict_lines_rendered` unchanged in count, but the rendered
population shifts: `[own loop verdict: proven but no live starter]` faces appear where
wave 80 would have named a tapped permanent. Per-record trace: `own_loop_verdict` (DE's
U10 line-derived face).

## F7 — HIGH — the ETB detector mistook activated self-bounce for leaving on entry — **CONFIRM**

**RED.** Fleeting Image's whole repository script is `{1}{U}:moveto(hand)`. The wave-80
`w80EtbSelfLeavesLine` found no `@`, no `target(`, a `moveto(` to a non-battlefield zone —
and returned true, subtracting a flier that can block and printing that its
enters-the-battlefield text removes it.

**FIX.** An untriggered line that carries an ACTIVATION COST is an activation. The cost is
the top-level `{...}` group in front of the first top-level `:` (the same split #W71-BR
L16 documents), depth-aware so `this(variable{type:*:myhand}=0)` is a spec and `draw:1` /
`life:-4` are effects, not costs.

**GREEN.** 5 pins: Fleeting Image and two other cost-prefixed self-leaves are refused;
Acererak's own untriggered ETB line (the #W80-DF U3 case) still counts; an explicit
`@movedto(this|battlefield):` trigger still counts; the depth-aware split is pinned on the
spec case and on an effect colon.

**PREDICTION.** `crack-back cover: NONE from this body` clauses fall to the cards that
really do leave on entry; the false mechanism sentence disappears from Fleeting Image rows.
Falsifier: any wave-80 prompt carrying that clause over a card whose leave line has a cost
prefix.

## F8 — HIGH — the blocking lesson is false for trample — **CONFIRM**

**RED.** `w80CoverMechanismSentence()` was unconditional and says "a 1/1 in front of a 5/5
stops the whole 5". CR 702.19b: a blocked trampler assigns only lethal damage to its
blockers and the rest reaches the player.

**FIX.** `CrackBackAttackerFact` carries `trample`, filled at all three live build sites.
With a trampler in the combat the sentence states the SPILL and says the figures are an
upper bound; the SURVIVAL verdict is withdrawn and said to be withdrawn (the #W65-AL G4
discipline for a FLOOR total, applied to the same hole in the other dimension). The
stay-home cover clause gets the same treatment.

**GREEN.** 4 pins including the wave-76 lesson's requirement: the POSITIVE clause still
prints on the shape it was built for — an ordinary 5/5 is fully absorbed and the row still
says "which you SURVIVE".

**PREDICTION.** Non-zero occurrences of `TRAMPLE, so blocking it does not remove its whole
power` in the wave-80 prompts, on rows whose crack-back includes a trampler; zero
occurrences of `stops the whole 5` on those same prompts. Per-prompt trace: the cover
clause itself.

## F9 — MED — U14 charged optional draws as compulsory lethal damage — **CONFIRM**

**RED.** Sylvan Library's line is `@each my draw:may name(Draw 2 cards) draw:2 && ...`.
`drawStepExtrasScan` reported `("Sylvan Library", 2)` with no optionality, and U14's
blockers verdict summed it into a compulsory lethal charge — declaring loss at −1 over a
draw the seat can decline and live at 1.

**FIX.** The scan hands back a parallel optionality vector (`w80DrawExtraIsOptional`: a
top-level `may` token before the payload's `draw:`), U14 charges only the forced draws, and
the optional ones print as "plus up to N more if you CHOOSE ... NOT counted in the figure
above".

**GREEN.** 4 pins: Sylvan Library optional; Howling Mine compulsory; `may` inside a word is
not the token; a `may` nested in a `newability[...]` belongs to that sub-ability.

**PREDICTION.** DRAW-FORECAST-fed lethal verdicts on boards holding a `may`-draw permanent
drop by that permanent's amount × the punisher's per-draw figure; the "if you CHOOSE" clause
appears in the combat forecast. Falsifier: any wave-80 blockers prompt whose lethal figure
still includes a `may` draw.

## F10 — MED — the "send-time" counters ran before cache suppression — **CONFIRM**

**RED.** The casting caller applied the faces with a hardcoded `sent=true` immediately
before `askModel`, and `askModel` has three branches that answer with no round trip (ask
cache, repeat latch, cross-phase replay). Pinned on the predicate: `w80CountRenderedAtSend`
with `sent=false` counts nothing, so the caller's `true` was an assertion, not evidence.
(The priority seam already did this correctly; the cast seam did not.)

**FIX.** The caller STAGES its faces; `askModel` owns the boundary. A scoped guard applies
them exactly once — `true` on the line that hands the prompt over, `false` on every other
exit — as a destructor rather than a call at each `return`, because there are a dozen exits
and the next one added would otherwise be silently uncounted. The staging is swapped out on
entry so it can never key a later ask.

**GREEN.** 4 pins: an unsent window counts nothing; a real send counts once; a polling-tick
rebuild cannot count twice; an unsent window CLEARS the face so no later record consumes it.

**PREDICTION.** `crackback_verdict_lines_rendered`, `stack_death_verdict_lines_rendered`,
`own_loop_verdict_lines_rendered` and `stack_drain_windows_asked` each equal the number of
prompts in the corpus containing their line, exactly — wave 79's gaps were 63 vs 56, 172 vs
170 and 30 vs 18. Per-record trace: the corresponding `*_verdict` record field, present on
exactly the records whose prompt carried the line.

## F11 — MED — hold-event fields overwrote events and migrated — **CONFIRM**

**RED.** `mHoldSaferIgnoredFace` and `mHoldReopenReason` were one shared string each,
overwritten by every clamp in a window and consumed by the next `writeTransLog` of **any**
kind — so a window that clamped both faces kept at most one, and the next unrelated target
or discard record inherited the survivor. Wave 79 counted 59 clamps and could adjudicate
none of them.

**FIX.** Both record fields are DELETED. Every clamp and every re-open writes its own
`kind: hold_event` record at the event (`what`, `seam`, `face`, `reason`, `window_seq`,
turn, phase), on the `forced_close`/`menu_single_outcome` pattern — telemetry about a window
that may never be asked, taking no ask seq.

**GREEN.** Pinned on the census side: `corpus-stats.py --selftest` now asserts that
`hold_event` is summed and explained by the derived census with no edit to the tool (the
wave-77 lesson as a test rather than a hope).

**PREDICTION.** `hold_events` == the number of `kind: hold_event` records in the seat log,
and `hold_verdict_safer_ignored + hold_reopened_new_threat + hold_reopened_new_lethal +
(rows-moved re-opens)` == `hold_events`. Wave 79's 59 clamps give a floor: **`hold_events`
≥ 59** across the wave-80 corpus, every one of them nameable.

## F12 — MED — cross-phase replay used the wrong source-sequence namespace — **CONFIRM**

**RED.** `logAskReplay`'s `replayed_from` is a RECORD seq on every other path
(`mAskCacheSeq`, `mRepeatAskSeq`, both snapshots of `mTransSeq`); the cross-phase path
passed `windowSeq`, the window counter. The entry stored no record seq at all, so the join
could not even be repaired after the fact.

**FIX.** `W76CrossPhaseAsk::recordSeq`, snapshotted from `mTransSeq` at cache time beside
the two existing snapshots, and passed to `logAskReplay`.

**GREEN.** 2 pins: an entry that has stored no ask records no source record; the two
ordinals are distinct fields and diverge.

**PREDICTION.** Every `why: crossphase_replay from X to Y` sidecar record's `replayed_from`
resolves to an `ask` record with that `seq` in the same seat log. Wave 79's did not.
Falsifier: any `crossphase_replay` whose `replayed_from` has no matching record seq.

## F1 — HIGH — the ctor guard passed while nine initializers were missing — **CONFIRM (guard)**

The nine dropped initializers were repaired before this lane; DH's work is the guard.

**RED.** `check-ctor-init.py` detects an initializer EATEN BY A COMMENT and nothing else,
so a member that was never written leaves no trace on the line for it to find.

**FIX.** A second test asks the header: every SCALAR member a class declares must be in the
ctor initializer list **or** assigned in the ctor BODY. The body half is derived from the
source, not kept as a hand-maintained whitelist — a whitelist is a file someone has to
remember to update and is wrong the first time it is not. Scalars only; pointers are
deliberately out (they are set from ctor arguments across several branches and would drown
the signal), which is fine because the defect class is the integer counter.

**Pre-existing uninitialised scalars enumerated:** AIPlayerGPT declares 205 scalars; 26 are
absent from the initializer list and **all 26 are assigned in the ctor body**
(`mLastHttpStatus`, `mLastCurlResult`, `mTimeoutMs`, `mPatienceLimit`, `mThinking`,
`mMaxTokens`, `mRepetitionPenalty`, the five `mOppLife*`/`mDamageLife*`, `mBulkMove*`,
`mSearch*`, `mPregame*`, the four `mStatedStop*`, `mCastDecisionOpen`) — so none needed
initialising and none needed whitelisting; the derived rule covers them. Across the rest of
the tree the check finds **55** genuinely uninitialised scalars (SimpleButton, SimplePad,
SimplePopup, TestSuiteAI, TextScroller and others). Those are real debt and not this
guard's to fix in the wave that adds it, so the MISSING check is a BUILD GATE for
`ENFORCED_CLASSES = ('AIPlayerGPT',)` and prints the residual count on every build
(`--notes` lists them), which makes the debt visible and lets a class be adopted the day it
is cleaned.

**GREEN.** `--selftest` builds a fixture header/source pair in a temp dir and asserts: the
header scan sees the three scalars and neither the string, the pointer, the array nor the
static; the DROPPED member is the one and only finding; a listed member and a
ctor-body-assigned member are both clean; the repaired fixture reports nothing. Verified
against the real defect shape by removing `mHoldEvents(0)` from the live initializer list —
`check-ctor-init: FAILED (0 eaten initializer line(s), 1 uninitialised scalar member(s))`,
naming the member — and restoring it.

**PREDICTION.** Not a corpus counter: the guard's falsifiable claim is that re-applying the
wave-80 merge damage (dropping any of DE's seven or DF's two counters) fails the build
before the link. Demonstrated on one; the mechanism is per-member.

---

## Weakest evidence

1. **No fixture runs a live GAME.** `runParseSelfTest` is static and PARSETEST constructs
   no `AIPlayerGPT`, so F2, F3, F5 and F10 are pinned by driving the live *scanners,
   builders and predicates* over the real bytes/boards, in the order the live callers call
   them — not by a seat answering a window. F3's `takeHold`→`holdHonoured` round trip and
   F5's `receiveEvent` close are therefore **unexercised by any test**; they are argued from
   the code, and the wave-80 corpus is what will actually decide them. This is the wave-78
   lesson's own hazard and I could not discharge it in this lane.
2. **F5's state-machine pin is a replay of the live sequence, not the live sequence.** It
   runs the same two helpers in the same order with the same transitions, but the prompt
   builder and `writeTransLog` are not in the loop. A future edit that changes only the
   *call sites* would leave the pin green.
3. **F2's prediction is structurally 0.** The shape has not occurred in three waves, so the
   corpus cannot confirm the fix fires — only that it never fires wrongly (lane DD's pins,
   which stay green). If a wave-80 seat ever writes the flipped shape, the fix costs that
   window to the heuristic; that is the owner's ruling applied, but it is a real cost and
   should be watched.
4. **F4's `-1` branch is a silent carry-forward.** Any board with protection, indestructible,
   wither/infect or more than 12 bodies a side keeps the wave-80 "LETHAL if it is UNBLOCKED"
   wording. That is honest but it means the false-death class is only *narrowed*, and the
   census cannot distinguish "no floor existed" from "the DP declined".
5. **F6's enters-trigger gate is a proxy.** "A creature can enter" is "a creature in hand
   whose cost the mana engine can plan" — it misses token-makers, reanimation and flicker,
   so a real starter can be under-reported (printing the GAP where a win is proven). The
   error direction is the safe one, but it is an error.
6. **F8 fixes the SENTENCE and withdraws the verdict; it does not fix the ARITHMETIC.** The
   cover model carries no blocker toughness, so a trample board still prints a cover figure
   that is an upper bound. It now says so in the clause, but a model that reads only the
   number is still reading a number that is too generous.
7. **F11 has no engine-side pin.** The reconciliation is pinned in `corpus-stats.py`; that
   `writeHoldEventRecord` is actually reached once per clamp is argued from the call sites
   (three of them), not tested.
8. **The 55 residual uninitialised scalars are un-gated.** They are counted and listed on
   demand, not fixed and not enforced. Anything reading one of them before writing it is a
   live bug this lane found and did not repair.
