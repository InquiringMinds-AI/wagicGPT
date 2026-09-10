# Wave-76 Lane CQ — the fix lane for the Astra adversarial review

Worktree `worktrees/lanes/w76-CQ`, branch `w76-lane-CQ`, base = master `dc664a18d`
(step-one gate 1: suite 1289/0 + 76 AI/0, PARSETEST 6112/0 — re-measured on the
seeded tree before any edit: **PARSETEST 6112 passed, 0 failed**).

**Gates after the lane**: suite THREADS=1 **1291 tests (0 failed), 76 AI (0 failed)**
(1289 + the two new fixtures; `==Test Failed !==` 0, `==Test timed out` 0 — none of
the three known flakes fired), PARSETEST **6168 passed, 0 failed** (+56 pins),
ctor-init guard OK (118 files), reply-instruction guard OK (1 source, 37 guides),
`git diff | grep -c U+FFFD` = 0. `reply-protocol.txt` and `kReplyProtocol` untouched.

Order worked: F2, F1, F10, F5, F6, F7, F3, F4, F9, F8 (as briefed).

All ten findings **CONFIRM**. None was refuted; one (F6) turned out to be right about
the mechanism and wrong about the *exemplar* — see F6.

---

## F2 (HIGH, Q8) — a refused force-close arm executed the heuristic. CONFIRM

**Repro (source-traced, then executed as a state-machine replay).** `AIPlayerGPT.cpp`
`pollCompletionRetry`: the Q8 refusal branch incremented `mForceCloseArmsRefused`,
`DebugTrace`d, and fell through. A 200 carrying native reasoning and no answer is
not `mLastTimeout`, not `retryableTransportFailure`, not a declaration payload and
not decode garbage, so none of the three retry arms below it could fire; the
function returned 0 with empty `content` and the seam's heuristic answered. The
comment's claim ("the window unwinds and is re-asked") had no code behind it.

**RED evidence.** Executed in PARSETEST as the base reading of the same state:
`#W76-CQ F2 REPRO` asserts `!w76ForceCloseArmAllowed(park.forceCloseArmed)` and
records that the base code had exactly **two** outcomes for that bool — arm, or
fall through — with no third. The fall-through is the heuristic. (This finding's
RED is structural rather than a failing assertion: the fixed code introduces the
third outcome, so a pin over it cannot compile against the base tree. Named in
Weakest evidence.)

**Fix.** `w76ForceCloseDecision(earned, parkArmed, deferTicks)` is now the single
decision point, returning `kW76CloseFallThrough` / `kW76CloseArm` / `kW76CloseDefer`.
A refused arm takes `kW76CloseDefer`: it builds the retry slot exactly as the arming
branch does (base, prefill, phase-1 finish reason, force-close tag) and returns
`kChoicePending` **without buying a decode**. The deferral rides the per-arm slot
(`GptRetrySlot::forceCloseDeferred` / `forceCloseDeferTicks`, swapped by
`selectRetryArm` with everything else), so an arm can never wait on a deferral that
is not its own — and a deferred close is **not** counted as outstanding for the
other arm's refusal test, which is what stops two arms deferring on each other.
At the top of the retry-poll branch a deferred slot re-asks one question per tick
and returns `kChoicePending` again, buying nothing; the moment the park clears it
arms (`gptForceCloseArm`, with the `mForceCloseUnrecorded` accounting taken there
rather than at the refused window) and phase 2 is bought. `mForceCloseArmsRefused`
now counts once per deferral, not once per tick. A drifted decision drops the
deferral with the slot and counts nothing unrecorded (it never armed).

**Deadlock clause (both halves pinned).**
- the heuristic is never reached while the rescue is earned: the loop pin walks 64
  ticks with the park permanently busy and asserts `fell == 0`;
- the decision resolves: the same loop asserts `arms == 1` and
  `waits == kW76CloseDeferMaxTicks` (8), and a second pin asserts that a park that
  clears arms the close on that very tick. `force_close_defer_bound_hits` is a new
  gameend field, so a permanently parked other arm is visible in the corpus rather
  than silently paid for.

**GREEN**: PARSETEST 6168/0 (11 new F2 assertions).
**Prediction**: next corpus, `force_close_arms_deferred` > 0 with
`force_close_defer_bound_hits` ≈ 0, and **zero** decisions whose record carries a
fallback class on a reply that was `reasoning_only` with a non-empty trace on a
force-close-supporting endpoint. The negative clause has its own pin: if
`force_close_defer_bound_hits` is non-zero and roughly equal to
`force_close_arms_deferred`, the park is not clearing and the bound — not the
clear — is doing the work; that is a re-open of Q8, not a success.

---

## F1 (HIGH, Q1) — a genuinely asked window could commit nothing. CONFIRM

**Repro.** `w76HoldReopenNote`: `holdNoteSameWindow(first, unseen, measuredSeq, nowSeq)`
returned the cached note *before* staging. Astra's sequence, replayed exactly:
`{A,B,hold}` asked at seq 1 and committed; an identical window built at seq 2 and
then suppressed — `w76HoldWindowNotAsked` erases `pending` while `measuredSeq` stays
2; B leaves the board and `{A,hold}` is built at seq 2 — zero unseen rows, same seq,
so the early return fired and staged nothing; that window IS asked and earns a
record, and `w76HoldWindowAsked` found no pending entry. `last` still held `{A,B,hold}`,
so when B returned the bracket called it "also on the menu at the last window I asked
you at this seam" — a window that contained only A.

**RED evidence (executed).** `#W76-CQ F1 REPRO/GREEN` — on the base tree the
committed set contains `holdActionKeyRow(rowB)` and has size 3; the pin requires it
absent and size 2. Both assertions fail on `dc664a18d`.

**Fix.** The early return now re-stages: the rows being built *right now* become
`pending`, at this seq, under the run the reused note actually claims
(`W76HoldMemory::measuredRun`, a new map that outlives the staging entry). Commit
on record, not on note reuse. `commitAtBuild` still reproduces the wave-75 policy
side by side.

**Key stability**: the change touches no rendered byte; the note text is unchanged
and the bracket stays on the prompt-only channel (existing Q1 KEY PIN set still
green). MUST-NOT-MATCH pins: a rebuild of one window still reuses the note and does
not re-measure it (`1 window in a row`), the committed run equals the printed run,
and a suppressed window still commits nothing.

**GREEN**: PARSETEST 6168/0 (7 new F1 assertions).
**Prediction**: next corpus, zero `[hold check: every row above was also on the menu
at the last window I asked you at this seam]` brackets whose named referent window
did not contain every row then printed. Negative clause with its own pin: if a
*suppressed* window ever appears as the referent, the re-stage has leaked past
`w76HoldWindowNotAsked` — the third F1 pin is exactly that case.

---

## F10 (MED, Q1) — the new pin institutionalised the forbidden key drift. CONFIRM

**Repro.** `stripRenderAnnotationsLc` took the **first** `}` after a `{`, so
`{reserve: … Puzzle Box {4} in your hand needs 4 … 3 sources cannot pay 4 …}` was
stripped only as far as the inner `{4}` and the remainder — carrying the board's own
untapped-source count — stayed in every key that function guards (option-set, ask,
echo matcher). Wave 76 fixed this for the hold key alone
(`w76StripBalancedAnnotationGroups`) and then **pinned the residue in the option-set
key as correct** (`optionSetKeyOf(r16) != optionSetKeyOf(r24)`), against the
governing key-stability ruling.

**Fix.** The shared stripper walks the group to its own close (depth counting), with
the mana-symbol test applied to the balanced span so a top-level cost pip still
survives and a cost quoted *inside* an annotation goes with the annotation. An
unbalanced `{` is left where it is and scanning continues — deterministic, nothing
truncated.

**Pins re-derived from the rule, never adjusted to pass.** Three pins had assumed the
first-`}` behaviour; all three were re-derived, and two of them were **RED after the
fix on the first run** (that is the executed evidence that the stripper really moved):
1. `#W76-CN Q1 KEY PIN` (the option-set drift) → now
   `optionSetKeyOf(r16) == optionSetKeyOf(r24)`, with a MUST-NOT-MATCH set: a
   different card still keys apart, a different top-level mana cost still keys apart,
   the whole nested group goes contents and all, an unbalanced group keys
   deterministically.
2. `#W67-AX I7` asserted `optionSetKeyOf(menu18) != optionSetKeyOf(menu19)` — the
   pair's whole delta lives inside annotation groups, so the rule's answer is EQUAL.
   Re-derived to equality; the surrounding claim (the hold key re-opens the pair on
   its own terms; the latch is not built on the render key) is unchanged and green.
3. `#W69-BF K3` asserted `menuRowProseName(row5) != "damnation"` *because* the nested
   group left annotation glued to the name. Re-derived: the prose name of that row IS
   the card name, and the two name readers now agree on it.

**GREEN**: PARSETEST 6168/0; full corpus re-run, no other pin moved.
**Prediction**: next corpus, zero hold re-opens and zero option-set-key changes whose
only row delta is inside a `{…}` group that quotes a cost. Negative clause: if a
*cast row's own mana cost* stops distinguishing two rows, the balanced strip has eaten
a cost pip — the two MUST-NOT-MATCH cost pins are that alarm.

---

## F5 (HIGH, Q5) — the cover clause subtracted creatures that survive. CONFIRM

**Repro.** `boardCreatureCounts` (the sweeper walk) and `xSpellPricing` (the X-damage
walk) both summed `crackBackBodyContribution(c)` for every creature the row may
legally hit, with no survival test at all — while the roster built two lines above,
in the same walk, already carries `sweeperRegenerationTail` and the ghostform tail.
Astra's board: two life, no blockers, the opponent's only attacker a regenerating
body under Day of Judgment. `crackRemoved = 3`, the clause promised survival, the
engine consumes the shield, taps it, and it untaps and attacks for 3.

**Fix.** `w76CrackVictimTrulyRemoved(card, destroyKind)` — the same liveness the
`{removes:}` / O1 forecast reads: `w76BodyReturnsInsteadOfDying` (#W76-CP Q10
ghostform), `sweeperRegenerationTail` (#W60-Q R6, which already gates CANTREGEN and
destroyKind), indestructibility under a destroy or bury (an exile sweeper still takes
it), and persist/undying. Consulted at **both** accumulation sites; the X-damage walk
passes destroyKind 1, since damage destruction is a destroy. Nothing leaves the
roster or any count — only the survival promise stops being made.

**RED evidence (caller path, live board).** New suite fixture
`bin/Res/test/w76cq_crack_victim_survives.txt` + a new suite command
`assertcrackremoved <0|1> <destroyKind> <card>` calling the predicate on a real
battlefield (the `#W74-CC O1` / `assertghostformlive` instrument shape, because the
clause is a GPT render string no zone or P/T assertion can see). RED on the seeded
tree: the command does not exist there and the walk subtracted the regenerator whole.
Five asserts: Drudge Skeletons not removed by a destroy, removed by a bury; Darksteel
Myr not removed by a destroy, removed by an exile sweeper; Grizzly Bears removed.

**GREEN**: suite 1291/0.
**Prediction**: next corpus, no `{crack-back cover: …}` clause names a body that is on
the battlefield untapped on the following turn. Negative clause: if the clause
*disappears* from rows that previously carried it and the removed bodies had no
regeneration/indestructible/ghostform/persist marker, the predicate is over-refusing
— the Grizzly Bears assert is that alarm.

---

## F6 (HIGH, Q6) — the converter clause priced a prohibited gain. CONFIRM (mechanism), exemplar corrected

**Repro.** The forced-sacrifice row's converter clause ("they gain N life (its
toughness) — and with their Sanguine Bond that is N off YOUR life") is built from
`lifeToDamageConverterTake` over the opponent's battlefield, with no consultation of
any life-gain prohibition. `Player::gainOrLoseLife` (`Player.cpp:185`) returns before
generating a life event when the gainer's opponent's battlefield has
`NOLIFEGAINOPPONENT` or the gainer's own has `NOLIFEGAIN` — so no gain, no Bond
trigger, no damage, and the row invents the loss. The identical exposure exists on the
edict row's own converter clause (#W53-O D13), so both were fixed: fixing one alone
would have made the two surfaces disagree about one board.

**Fix.** `w76LifeGainReaches(cantChangeLife, noLifeGain, foeNoLifeGainOpponent)` — the
pure rule, the same three zone-level abilities `lifeLoopProvenWin` reads — plus
`w76PlayerCanGainLife(gainer)`, the board reader, which **fails closed**: a side this
seat cannot read makes no claim. Both converter scans are gated on it.

**RED evidence (caller path, live board).** New fixture
`bin/Res/test/w76cq_lifegain_prohibited.txt` + `assertcangainlife <0|1> <1|2>`.

**Exemplar corrected.** Astra names Erebos, God of the Dead. Erebos cannot be placed
by the fixture's `inplay:` list at all — the list is comma-separated and the card's
name contains a comma, so `Erebos, God of the Dead` loads as two non-existent cards
and the first draft of the fixture reported "player 2 can gain" for that reason, not
because the engine disagrees. The fixture uses **Grima Wormtongue**
(`borderline.txt:49087`, `abilities=nolifegainopponent`), the same prohibition without
the comma. The finding stands unchanged; only the card in the repro moved.

**GREEN**: suite 1291/0.
**Prediction**: next corpus, no forced-sacrifice or edict row carries a converter
clause in a game where either player's battlefield holds a life-gain prohibition
covering the gainer. Negative clause: the clause must still appear on boards with no
prohibition — the `assertcangainlife 1 1` half is that alarm (the prohibition is
one-directional and must not silence the seat's own gain).

---

## F7 (MED, Q2) — the roster is bounded in SHAPES, not in bodies. CONFIRM

**Repro (executed).** `joinVictimRoster`: `kVictimRosterGroupCap` caps how many
distinct shapes print; each group then emits every body it holds, and the run-collapse
only fires on a **consecutive rank run**. 100 same-named Vampires alternating two
rendered P/T shapes put every body at alternating ranks inside its shape group, no run
reaches the collapse floor, and all 100 print — two shapes defeating the bounded
roster without approaching thirteen.

**RED evidence.** `#W76-CQ F7 REPRO/GREEN` builds exactly that board the way the
roster's own entries are built and requires `roster.size() < 1024`. On base the string
is several thousand bytes and the assertion fails.

**Fix.** A group prints at most `kVictimRosterGroupItemCap` (3) entries — a collapsed
range counts as one — and then NAMES the rest: `+ N more bodies of this same shape`.
Applied only above the collapse floor, so below it every byte is the wave-60 wording
and no existing pin moves. Nothing is trimmed silently and no option is removed: this
is a summary of a `{right now: …}` clause, not a menu.

**Pins**: the population is still stated first (`100 bodies: `); the residue counts sum
to exactly 94 (100 = 2 shapes × 3 printed + 94 residue — the arithmetic closes);
MUST-NOT-MATCH — a two-entry roster is byte-identical, an empty roster is empty, and a
genuine 40-body consecutive run still folds to ONE range row with no residue at all.

**Key stability**: shipped as part of the consolidated CQ KEY PIN set below.
**GREEN**: PARSETEST 6168/0 (8 new F7 assertions).
**Prediction**: next corpus, no single option row exceeds ~2 KB from its victim roster,
and every roster whose printed entries are fewer than its stated body count carries a
residue count that closes the arithmetic.

---

## F3 (MED, Q13) — the "phase-free" key still carried the phase. CONFIRM

**Repro (executed).** `w76CrossPhaseListKey` dropped the numeric phase and kept the
`decision` STRING — and the live casting seam (`AIPlayerGPT.cpp`, the `Casting
decision (` builder) puts the phase inside that string:
`Casting decision (Upkeep, opponent's turn): which card do you cast now, if any?` vs
`(Draw, …)`. The `152v125` ten-window shape could therefore never match, and the
wave-76 pin fed the artificial string `"Casting decision"`, which no caller passes.

**RED evidence.** The Q13 pins are re-derived to build both decision strings **with the
live seam's own `ostringstream` expression**, character for character, and then require
`w76CrossPhaseListKey(12, dUpkeep, rows) == w76CrossPhaseListKey(12, dDraw, rows)`.
On base those keys differ and the assertion fails. An INSTRUMENT pin above it asserts
the two live strings really do differ, so the comparison is not vacuous.

**Fix.** `w76PhaseFreeDecision` removes the first parenthetical carrying the turn-side
clause (`, YOUR turn` / `, opponent's turn`), which is the shape every seam header
builds — whatever the phase name is. MUST-NOT-MATCH: a different SEAM
(`Blockers decision (Upkeep, opponent's turn): who blocks?`) is still a different list.

**GREEN**: PARSETEST 6168/0.
**Prediction**: next corpus, a byte-identical list re-put at two phases inside one turn
produces a non-zero `cross_phase_re_puts` and the bracket appears. Negative clause: if
`cross_phase_re_puts` is still 0 across a corpus that contains a same-turn multi-phase
re-put, the key is still carrying something phase-shaped.

---

## F4 (MED, Q13) — the board claim was never a comparison, and the counter ticked per poll. CONFIRM (both halves)

**Repro.** (a) `w76CrossPhaseRePutNote` printed "nothing on the board has changed" from
turn + question + rows alone; neither the saved entry nor the comparison held a board
snapshot, so an unrelated permanent entering between two asks left the sentence false.
(b) `mCrossPhaseRePuts++` sat **above** the `pollCompletionRetry(...) == kChoicePending`
return, and the prompt is rebuilt on every polling tick — so one qualifying
asynchronous ask was counted again on every tick.

**Fix.** (a) `W76CrossPhaseAsk` carries `boardKey`, the ask-cache's own board string
(`serializeGameState()` / the situation prefill, now named once as `boardStateKey` so
nothing is rendered twice) with only the serialiser's leading `Phase: … | It is …`
line removed by `w76PhaseFreeBoardKey` — keeping that line would make the comparison
false by construction at every phase, and a clause that can never print is not a check.
Equality is required before the board clause prints; otherwise the note states the
re-put and says the board HAS moved. (b) `W76CrossPhaseAsk::countedSeq` — the counter
increments only when the window's own seq has not been counted yet.

**RED evidence (executed).** `#W76-CQ F4 REPRO/GREEN` requires the moved-board wording,
which does not exist on base (the note took no such parameter and the sentence was
unconditional). Supporting pins: the phase line is out of the board key so the
comparison is possible at all across two phases; an unrelated permanent MOVES the board
key; a board with no phase line is compared whole.

**GREEN**: PARSETEST 6168/0.
**Prediction**: next corpus, `cross_phase_re_puts` ≤ the number of distinct asked
windows that qualified (never a multiple of the tick count), and every printed
"nothing on the board has changed" is over two asks whose serialised boards match
below the phase line. Negative clause with its own pin: if the board clause never
prints at all, the phase line is not the only phase-varying content in the serialiser
and the comparison is over-strict (fail-closed, but a dead clause).

---

## F9 (MED, Q15) — the echo class was the flag, not the bytes. CONFIRM

**Repro (executed).** `w75ProtocolDeviationClass`'s third argument was the forced-close
FLAG (`rec.count("reasoning_forced_close") > 0`), so **any** positive off-protocol byte
count on a rescued reply became `forced_close_prefill_echo` — including new commentary
written after a valid PLAN/action pair, which is exactly the `prose_outside_two_lines`
the class then concealed. The wave-76 pins supplied only the flag and a byte count, so
they could not tell the two cases apart; the base pin at
`#W76-CP Q15 REPRO/GREEN` asserting `w75ProtocolDeviationClass(false, 823, true) ==
"forced_close_prefill_echo"` is the base behaviour, executed.

**Fix.** `offProtocolBytes` optionally hands back the off-protocol LINES;
`w76OffProtocolIsPrefillEcho(lines, prefill)` requires **every** line to appear
verbatim in the prefill (one line of the model's own makes the whole reply prose — the
direction that cannot hide a deviation); with no prefill nothing is ever an echo. The
prefill survives the phase-2 consume in a new `mLastForceClosePrefill`, consumed by the
record that classifies the reply. The class's third argument is now that verdict.

**RED evidence.** `#W76-CQ F9 REPRO/GREEN`: new prose after a valid pair is NOT an
echo. On base that same reply classified as `forced_close_prefill_echo`.
MUST-NOT-MATCH: a reply that echoes AND adds is not an echo; with no prefill nothing is.

**GREEN**: PARSETEST 6168/0 (8 new F9 assertions).
**Prediction**: next corpus, `protocol_deviation: forced_close_prefill_echo` appears
only on records that also carry `reasoning_forced_close`, and sampling any three of
them shows every off-protocol line inside the record's own `reasoning` field.
Negative clause: if the class drops to 0 across a corpus that still has rescued replies
with off-protocol bytes, the prefill is not surviving to the record — `mLastForceClosePrefill`
is being cleared too early.

---

## F8 (MED, Q9, measure only) — an ordinary targeted bounce was exempted. CONFIRM

**Repro.** `w75PlanNamesUncastableZoneCard` ran `w76SelfRecursiveZoneScript` over the
card's ordinary `magicText` as well as over its zone script. Unsummon's real primitive
(`mtg.txt:126295`) is `target=creature` + `auto=moveTo(hand)` — it moves its TARGET —
so a plan naming Unsummon while it sits dead in the graveyard was exempted from the
measure it belongs in.

**Fix.** The moveTo family is read from the ZONE script only (`magicTexts["graveyard"]`
/ `["exile"]`), which is the only place a script can act on the card itself from a dead
zone — Hammer of Bogardan's real `autograveyard={2}{R}{R}{R}:moveto(hand) myUpkeepOnly`
(`mtg.txt:52046`) is unaffected. Ordinary text now buys only a cast-PERMISSION keyword
(`w76CastPermissionKeyword`: flashback / retrace), which moves nothing and keeps the
#W75-CL trio's own exemptions intact.

**RED evidence.** `#W76-CQ F8 REPRO/GREEN` over both cards' **real** primitive strings:
Unsummon's `moveTo(hand)` buys no exemption; Hammer's graveyard script still does. On
base the first assertion is false (the ordinary-text scan matched `moveto(hand)`).

**GREEN**: PARSETEST 6168/0. Measure only — nothing is refused, re-asked or rendered.
**Prediction**: next corpus, `plan_names_uncastable_zone_card` rises relative to wave 75
by roughly the number of targeted-bounce instants in the seat's graveyards, and no
sampled exemption is a card whose only `moveto` lives in ordinary text. Negative
clause: if the count rises by more than the graveyard's bounce population, the
flashback/retrace permission read has narrowed too far.

---

## Consolidated KEY-STABILITY pin set (F5, F6, F7 — every fix that touches a rendered row)

Built the way the LIVE seam builds it: the held set is `holdActionKeyRow` over the
menu's own rows (as `mLastMenuRows` supplies them), the ask tail is
`stripDeclineReaskTags(joinNumberedRows(rows, NULL))`, and the two rows are produced by
the production builders (`joinVictimRoster`, `crackBackKillRowTag`,
`forcedSacrificeRowTag`) — not by hand-written strings.

- INSTRUMENT: the two rows really do differ (a pin over identical bytes proves nothing).
- OPTION-SET key: identical across both clauses moving.
- HOLD LATCH: the held sets are identical, and `holdStillStands` holds.
- ASYNC SLOT key: a pure function of the ask tail; moves with the render, never with a
  key of its own.
- HOLD-CHECK bracket: two windows differing only in these clauses measure zero new rows.
- F6's converter clause appearing or disappearing moves NO key (it rides a `[…]`
  bracket): option-set and hold-action keys both equal.

---

## Weakest evidence

1. **F2's RED is structural, not a failing assertion.** The fix adds a third outcome to
   a decision that had two, so a pin over the fixed function cannot compile against
   `dc664a18d`. What IS executed is the base reading of the same state
   (`!w76ForceCloseArmAllowed(park.forceCloseArmed)`) plus the fact — traced through
   every branch below the refusal — that no other arm can fire on a 200 with a trace
   and no answer. A live endpoint that makes every reply reasoning-only would settle it
   outright; this lane sends no requests.
2. **F2 and F5/F6 are pinned at different altitudes.** F2's tick loop is replayed
   through the caller's own decision function and the production slot swap, but not
   through `pollCompletionRetry` itself, which needs a transport. F5/F6 go through a
   live board via new suite commands, but the commands ask the *predicate*, not the
   rendered clause — the clause is a GPT render string the suite cannot assert on. Both
   are the strongest instrument available in-tree; neither is the row itself.
3. **F5's regeneration test is "can regenerate", not "has a shield up right now."** It
   reuses `sweeperRegenerationTail`, whose own contract is "MAY survive". That is
   fail-closed for the survival promise (a creature that could pay is not subtracted),
   but it will under-count removal against a board that cannot actually pay the
   regeneration cost. Left as-is deliberately: the roster's tail says the same thing on
   the same row, and making the two disagree would be worse than being conservative.
4. **F6's exemplar could not be reproduced with Astra's card.** `Erebos, God of the
   Dead` cannot be placed in a fixture's comma-separated `inplay:` list. The mechanism
   is pinned with Grima Wormtongue; whether Erebos *specifically* registers
   `nolifegainopponent` on the battlefield zone in this engine (its devotion transform
   does a `removetypes`) is **not** established by this lane and is worth a fixture of
   its own — an `inplay:` list that can carry a comma'd name, or a cast-and-resolve
   fixture.
5. **F4's board comparison may be too strict to ever fire.** Only the serialiser's
   leading phase line is removed; anything else in the state render that varies by
   phase (a stack line, a "combat comes next" tag, a mana-pool line) will keep the
   boards unequal and the note will always take the "the board HAS moved" branch. That
   is the safe direction — no false claim — but it means the positive clause is
   unproven against real prompts. The prediction's negative clause names it.
6. **F7's cap of 3 entries per group is a judgement, not a measurement.** No corpus
   evidence says three handles is the right number for a decision; it was chosen to be
   small because a group is one shape. If a review finds the seat needs more named
   handles inside a shape, the constant is the only thing to move.
7. **F10 widened a stripper shared by four consumers** (option-set key, ask key, echo
   matcher, `menuRowProseName`). The full PARSETEST corpus and the whole suite were
   re-run and exactly two other pins moved, both re-derived from the rule. But the echo
   matcher's behaviour on nested annotations is only pinned indirectly (via
   `menuRowProseName`); a reply that echoes a nested-annotation row is not pinned.
