# Wave-79 lane DC — the Astra review fix lane (F1..F11)

Base `244d8b5f8` (= master; gate 1: 1293/0, 76 AI/0, PARSETEST 6810/0). Worktree
`worktrees/lanes/w79-DC`, branch `w79-lane-DC`. Findings verbatim in
`wave79/codex-review.md`; the lanes repaired are CZ (F1, F2, F9, F10), DA (F3, F4,
F5, F8) and DB (F6, F7, F11).

**ALL ELEVEN CONFIRMED.** Nothing was refuted. Three of the eleven (F1c, F3, F9)
were confirmed by an instrument stronger than Astra's source trace: F3's two boards
are now **suite fixtures driven through the engine's own damage code**, and they say
the engine disagrees with the shipped arithmetic by 6 and by 4 points respectively.

PARSETEST **6883 passed / 0 failed** (base 6810 / 0; +73, of which 4 are wave-79
pins this lane SUPERSEDES and rewrote in place — listed per finding). Suite
THREADS=1 **1295 tests (0 failed), 76 AI tests (0 failed)** (base 1293/0; +2 = the
two new F3 fixtures; `==Test Failed !==` 0, `Test timed out` 0, no flake hit).
`selfplay-harness.sh --selftest` 30 checks / 0 failed; `bash -n` OK;
`corpus-stats.py --selftest` OK; `check-reply-instructions` OK;
`check-ctor-init` OK (118 files); `git diff | grep -c U+FFFD` = **0**.

No corpus was run and no request was sent to the pilot. T1 is untouched: the parser
is byte-identical and no tolerance for an unlabelled answer was added.

---

## F1 — HIGH, T3: the key caches row labels, not the legal decision — **CONFIRM**

**RED.** `#W79-CZ` (T3) replaced the ask key's and the async slot key's board half
with `turn + phase`. Two windows at one seam whose printed rows are byte-identical
then share a key however the board moved, so the cached answer is replayed. Pinned
on Astra's own shape, through the LIVE collector (`joinNumberedRows` →
`w77KeyTailOf` → `w79AskScopeKey`): two separately-rendered menus for a `Cast Doom
Blade` window, one with a single legal creature target and one after a second
creature entered in the same phase, produce **byte-identical keys**
(`#W79-DC F1 RED-ON-BASE`). The third part of the finding is at the priority seam:
`unchanged = (askKey == mLastAskKey)` with `mLastChoice > 0` force-passes priority,
and under a scope-plus-rows key a SUCCESSFUL reusable activation (one that creates a
token, taps a permanent, moves a card) leaves every normalised row standing — so the
breaker fires after a play that worked.

**Fix (two sentences).** The key's board half is now the seam scope PLUS a
legal-continuation digest: per offered cast/activation row, the identities the row's
own live `TargetChooser` accepts (the engine's `canTarget` verdict over the zones
`countValidTargets` walks), and once per window the seat's untapped-source count,
which is the currency every X ceiling and every `{leaves N of your M}` figure on the
menu is priced in — no life total, no log line, no annotation text, and a stack
object only where a row can legally target it. The deadlock breaker's "no progress"
premise reads the BOARD (`w79ForcePassNoProgress(keyUnchanged, lastChoice,
boardMoved)`, off the `boardKey` this seam already renders), so a successful
activation is never read as a loop, while the ask CACHE is deliberately left
board-blind — re-asking on any board move would put all 233 of CZ's same-phase
re-asks straight back.

**GREEN.** The corpus's own re-asked pairs still key EQUAL through the live
builders: `123v126` seqs 93→96 (rows byte-identical; between them life ticked
13→14, a log line arrived and the opponent's stack top changed from Exquisite
Blood's Life to Sanguine Bond's Life Loss — and the one acting row, `Create human
with Thraben Doomsayer #2`, targets nothing) and `123v125` seqs 59→62 (one `[...]`
render note; Tragic Slip's single legal creature target is the same instance at
both windows, off one untapped source). Astra's shapes key UNEQUAL: a second legal
Doom Blade target, and an X ceiling grown from 3 sources to 4. The breaker holds on
an unchanged board and does not fire on a moved one; a moved key and a pass/hold as
the last choice were never its population and still are not.

**Prediction (wave-79 census).** `ask_key_continuation_differs` **> 0** (expected
band 30-200 against the wave-78 population of 419 same-seam same-turn re-asks — it
counts exactly the windows whose seam and normalised rows recur with a moved
continuation) and `menu_pass_no_progress_suppressed` **> 0** on any seat that takes
a repeatable activation twice. Negative clause: `ask_key_continuation_differs` = 0
with `ask_replays_reserved` still rising means the digest is empty at the live seam
— i.e. `mContinuationDigest` never reached the key builder — which is a FAIL, not
"no such window occurred".

---

## F2 — HIGH, T3: a new threat after the answered one is suppressed — **CONFIRM**

**RED.** `#W79-CZ` made the own-loop marker a bare FACE, so the latch's rank clamp
(`w79HoldVerdictForCompare`) sees threat A and threat B as the same string and
returns the held face for both: `n <= h` → held. Pinned through the LIVE latch
(`w78HoldLatchKeys` + `w78HoldStillStands` with `holdActionKeyRow`): with a face-only
marker the hold **still stands** over a new threat on a different component
(`#W79-DC F2 RED-ON-BASE`), so the one window that matters is never asked.

**Fix.** The THREATENED face — and only it — carries the identity of the threat
(`[own loop verdict: THREATENED - <spell> vs <component>]`); the clamp yields to the
live face whenever two own-loop faces at equal rank differ. The three safer faces
stay bare, which is where all 194 of wave 79's corpus re-opens lived, and an unnamed
THREATENED (the record stamp's own call) is byte-identical to wave 79.

**GREEN.** Same threat → held; a NEW threat at equal rank → the live latch re-opens;
RESOLVING after THREATENED → still held (CZ's clamp untouched); an equal-rank
crack-back face that merely reprices on a life tick → still clamped. Superseded in
place: `#W79-CZ T3 GREEN the threatened MARKER is the face alone`.

**Prediction.** `hold_reopened_new_threat` **> 0** on any seat that takes a hold
over a THREATENED loop and is then threatened again (deck123/deck126 carried 87
holds and 194 re-opens in wave 78), and `hold_verdict_safer_ignored` stays **> 0** —
both non-zero, because the fix narrows the clamp rather than removing it. Negative
clause: `hold_reopened_new_threat` = 0 while `hold_verdict_safer_ignored` is large
means the THREATENED names are not reaching the marker at the live seam.

---

## F3 — HIGH, T5: an incomplete combat model promoted to an exact verdict — **CONFIRM**

**RED, in the engine's own damage code.** Two new suite fixtures:

* `w79dc_f3_doublestrike_trample_two_steps.txt` — Dragon Tyrant (6/6 flying,
  trample, double strike) into Wall of Air (1/5 defender, flying). The engine ends
  that combat at **13 life from 20 — seven damage**. The wave-79 one-step DP prices
  the same board at `6 − 5 = 1` (life 19) and the header promised survival there.
* `w79dc_f3_firststrike_blocker_kills_trampler.txt` — Rootbreaker Wurm (6/6
  trample) into Minotaur Aggressor (6/2 first strike). The engine leaves the seat at
  **20 and kills the attacker**; the DP calls `6 − 2 = 4` a proven maximum ("no
  assignment of your blockers does better"). A legal block BEATS the printed
  ceiling, so DA's claim that the fidelity gaps are exclusively pessimistic is false
  in this direction.

Both figures are pinned in PARSETEST against the base model's own inputs
(`#W79-DC F3 RED-ON-BASE`, 1 and 4).

**Fix.** `w79ResidualFor` models both damage steps (CR 510.4): a first- or
double-striker assigns its lethal cuts and tramples in step 1, blockers that took
lethal there are gone, a double striker deals again in step 2 with the spill
recomputed against what is left, a first-striker without double strike does not,
and an attacker killed in step 1 by a first-striking blocker deals nothing at all;
`W79AtkFact` gained `toughness`/`firstStrike`/`doubleStrike` and blockers gained
power and their own strike abilities (`W79BlkFact`). Where any declared attacker or
candidate blocker carries an ability outside the model
(`w79CombatAbilityOutsideModel`: protection, indestructible, persist/undying,
wither/infect) the search does not run, so `exactAssignment` stays false and the
wording drops both the optimality claim AND the death badge — both live inside the
`bestCaseOptimal` branch alone.

**GREEN.** The blocked assignment on Astra's first board prices at **7**, exactly
the engine's figure, and the DP's best case becomes 6 (decline the block) rather
than the 1 that promised survival — at Astra's 4 life every line is lethal and the
header no longer says otherwise. The second board prices at **0**. The two wave-79
trample fixtures are byte-identical (`146v152` seq 21 still reads 5): a plain
toughness list is a blocker list with no power and no strike abilities, so no
ordinary board moves. The unpromoted line carries neither claim; the proven branch
still prints the badge.

**Prediction.** `blocker_forecast_rows` stays **> 0** and at least one wave-79
prompt renders the honest unsearched wording over a board carrying protection or
indestructible — i.e. `blocker_forecast_collapsed` **> 0** with `no block saves you`
absent from those windows. Negative clause: if every blockers header still carries
"no assignment of your blockers does better" on a board with a protection creature
in it, the out-of-model guard is not reaching the caller.

---

## F4 — HIGH, T9: cover added back that was never subtracted — **CONFIRM**

**RED.** `crackBackReliefClause(7, 2, 6, false, "", 2, 3)` printed
`6 − (7 − 2) − 2 = −1 … that still KILLS you`. `total` is the opponent's attacking
POWER and `myLife - after` is the projection for DECLARING NO BLOCKS — no blocker's
cover is in it — so subtracting the lost cover double-prices the body and invents a
death: on Astra's board (6 life, an untapped Mogg Fanatic and a 0/6 against a 5/5
and a 2/1, sacrifice the Fanatic to kill the 2/1) only five opposing power remains
and declining every block leaves ONE.

**Fix.** The spent body is charged against the base it actually costs — the BLOCKED
best case — and the figure above is named for what it is ("the figure just above
already assumes you declare NO blocks, so it does not move — what this costs you is
G off your best case WITH blockers"); the death verdict reads off the no-block base
alone, which is the only number this clause proves.

**GREEN.** Astra's board reads `you would be at 1` and claims no death; the spent
body is still priced. `130v146` seq 53 at 1 life is lethal on the no-block
projection itself (1 − 5 = −4) and the verdict stands; a cost that cannot take a
blocker renders the wave-78 bytes. And the double subtraction can no longer occur
for ANY give-back: a sweep over give-backs 0..9 produces no death claim on a
survivable no-block projection. Superseded in place: two `#W79-DA T9` pins.

**Prediction.** ≥ 1 rendered relief clause carries `off your best case WITH
blockers` (`stack_drain_windows_asked`-style population: the clause rendered 0 times
corpus-wide before wave 79, and DA's own prediction for it stands), and **0**
prompts contain `count it back and you would be at`. Negative clause: if the new
string appears 0 times while the old DIES verdict rows are present, the clause is
again not reaching its row builder — the #W63-AC E9 failure, twice.

---

## F5 — HIGH, T9: the folded attack sentence bypasses their blockers — **CONFIRM**

**RED.** The fold read `N reaches them and they would be at oppLife − N - not
oppLife − totalPower`. Every figure above it is careful to say which side of
blocking it is on ("At least N lands whatever they BLOCK", "declaring all of them
with NONE blocked"); the fold prices ONE declaration against the declaration
trigger and nothing else, so its residual is damage BEFORE blocking. Astra's board —
a 1/1 and an ordinary 5/5 declared into Lightmine Field with an untapped opposing
5/6 — gets a sentence promising five player damage where the 5/6 blocks the
survivor for zero.

**Fix.** The fold states the power that SURVIVES the punisher, says explicitly that
this is power before blocking, and projects no life at all — it routes the reader to
the blocker-aware figures above, which already price their untapped blockers.

**GREEN.** `reaches them and they would be at` is gone from the emitter; Astra's
board renders `5 power survives the field and goes to the declaration. That is the
power BEFORE blocking … what reaches THEM is whatever their blockers do not stop`.
The fold itself is kept — DA's whole finding was the silence and nothing goes back
to silent — and a punisher that kills nothing still says so. Superseded in place:
one `#W79-DA T9` pin.

**Prediction.** ≥ 1 prompt carries `power survives the field` and **0** prompts
contain `reaches them and they would be at`. Negative clause: a prompt carrying the
old string means a second, unfixed copy of the fold exists.

---

## F6 — MED, T8: a creature's casting cost reserved as a counterspell — **CONFIRM**

**RED.** `w79CounterspellScript` accepted `fizzle` anywhere in any script. Ertai,
Wizard Adept's repository script is `{2}{U}{U}{T}:fizzle target(*|stack)` — an
ACTIVATED ability on a creature — so his `{2}{U}` CASTING cost was reserved as "a
counter held up", displacing a real counter. The reserve leaves no usable answer:
Ertai must enter, outlive summoning sickness and then pay a separate activation
cost.

**Fix.** A counterspell candidate is a SPELL whose own cast effect counters: the
`fizzle` must sit in a line with no colon ahead of it (a colon means the effect
belongs to a trigger or an activation, not to the spell), and the card must be
castable in the window the gate is about (an instant, or flash).

**GREEN.** Ertai is rejected twice over (the colon rule, and the type gate);
Counterspell (`fizzle`), Mana Leak
(`transforms((,newability[pay[[{3}]] name(pay 3 mana) donothing?fizzle])) forever`)
and Dissipate (`fizzleto(exile)`) are still candidates; Path to Exile and an empty
script are not; a sorcery-speed card whose own effect counters cannot be held up and
is not one; a TRIGGERED fizzle is not one.

**Prediction.** ≥ 1 rendered keep-X clause names a counter (`holding up a counter`
population, DB's own T8 prediction), and **0** of them name a card whose `fizzle`
is behind a colon. Negative clause: if the counter arm renders 0 times the candidate
rule is now too tight and the MED fix has cost DB's feature — watch for the clause
naming a cheaper non-counter instant again.

---

## F7 — MED, T11: a kept clause loses the clause it depends on — **CONFIRM**

**RED.** Chrome Mox's repository text is
`Imprint - When Chrome Mox enters, you may exile a nonartifact, nonland card from
your hand. -- {T}: Add one mana of any of the exiled card's colors.` — a 91-byte
imprint clause and a 53-byte mana clause, 148 with the separator. The mana clause is
decision-bearing (`{T}:`) and the imprint clause is not, so the selector keeps the
mana clause first and cannot then fit its own antecedent at either live budget (120
or 140): it printed "of any of the exiled card's colors" with nothing on screen
saying which card is exiled or that the exile is a nonartifact, nonland card from
hand. Pinned: the dependency direction is derived from
`w79ClauseNeedsAntecedent`/`w79DecisionBearingClause`, not asserted.

**Fix.** A clause that refers back ("the exiled card", "that card/creature", a bare
`it`, "if you do", "this way", …) is only SELECTED where its antecedent fits beside
it; a dependent clause whose antecedent will not fit is not selected at all, and the
budget then goes to the antecedent.

**GREEN.** At 120 and at 140 the snippet is exactly
`Imprint - When Chrome Mox enters, you may exile a nonartifact, nonland card from
your hand. (...more)` — the antecedent, with the omission marked. An independent
decision-bearing clause is still hoisted over a long keyword clause and still marks
its omission, and where everything fits the emitter still says nothing.

**Prediction.** **0** prompts contain `of any of the exiled card's colors` without
`exile a nonartifact`, and ≥ 1 prompt renders the Chrome Mox snippet with
`(...more)`. Negative clause: if hoisted snippets stop appearing at all
(`put_gloss_stripped` collapsing, no `(...more)` in any prompt) the dependency pass
is firing far wider than its vocabulary should allow.

---

## F8 — MED, T7: a spent, opponent-only ETB named as your untap engine — **CONFIRM**

**RED.** `w79UntapEngineSources` matched `untap` + `all(`/`foreach(` + `creature` in
any line of any permanent's script on either battlefield. Village Bell-Ringer's
whole script is the bare resolution effect `untap all(creature|mybattlefield)` —
already spent when it entered, and aimed at ITS OWN controller's creatures — and it
satisfies every one of those three tests, so the tap bill named THEIR Bell-Ringer as
a source that can untap the seat's creatures before declaration.

**Fix.** Two gates, both read off the matched line: LIVENESS (the effect must sit
after a colon — a trigger `@...:` or an activation `<cost>:`; a bare line is a
resolution that has already happened for a permanent in play) and SCOPE (read from
the EFFECT alone, so Intruder Alarm's `mybattlefield` inside its TRIGGER, which
describes what FIRES it, is not mistaken for what it untaps; `mybattlefield` in the
effect reaches the seat only when the seat controls the card, `opponentbattlefield`
is the mirror, and an unqualified `all(creature)` counts for both).

**GREEN.** Their Village Bell-Ringer is not a source; neither is the seat's own one
already in play (the liveness gate, the honest direction). Intruder Alarm
(`@movedTo(creature|myBattlefield):untap all(creature)`) counts on BOTH sides. The
standing `lord(creature) doesnotuntap` is not an engine; a live trigger scoped to
`mybattlefield` counts for its controller only, `opponentbattlefield` mirrors it, and
a single-target untap is not an untap-ALL engine.

**Prediction.** ≥ 1 rendered tap-bill clause names `Intruder Alarm`, and **0**
rendered clauses name a permanent whose only untap line is a bare resolution effect.
Negative clause: if the untap clause renders 0 times in the wave-79 corpus, this fix
has closed DA's T7 feature rather than scoping it — compare against the wave-78
deck152 HIGH-1 board, which is an Intruder Alarm board and must still fire.

---

## F9 — MED, T12: positional ordinals mistaken for instance identities — **CONFIRM**

**RED.** `instanceHandle` recomputes ` #N` from the CURRENT battlefield order of
same-named permanents, so removing an earlier copy renumbers every later one.
`#W79-CZ` (T12) kept every ordinal above one as identity, so the held row
`deal 1 damage with staff of nin #3 targeting goblin` and the same permanent's row
after an earlier tapped Staff is removed
(`… staff of nin #2 …`) are two different occurrence keys (`#W79-DC F9
RED-ON-BASE`) — a hold re-opened with no new legal action.

**Fix.** `w79RealignRankShifts` groups rows by SHAPE (the key with every ordinal
blanked) and, for a shape carrying the same number of rows in both windows whose
ordinals are a pure DECREASE, realigns the now-rows onto the held ones before the
comparison. That is the engine's own renumbering semantics: a rank falls only when
an earlier copy leaves, and rises only when one arrives. Applied in both the bracket
(`w78HoldRowDelta`) and the live latch (`w78HoldStillStands`).

**GREEN.** Astra's rank shift reads **0 new / 0 gone** and the live latch holds.
MUST-NOT-MATCH, four ways: `125v123` seq 263 (no ordinal → ` #2`, a different SHAPE
and an ordinal going UP) still reads 2 new / 2 gone, so CZ's T12 is untouched; the
renumber-in-place case (one row → `#1` + `#2`) still reads exactly 1 new / 0 gone,
so wave 78's fix is kept; a real second copy beside the held one is 1 new; a rank
going UP with the count unchanged is 1 new / 1 gone; and a different TARGET is a
different row whatever the ordinals do.

**Prediction.** `hold_windows_skipped` **> 0** and the stderr count of
`hold re-opened … a printed row changed or is newly available` FALLS below wave 78's
194 — and **0** hold-check brackets print `is new` over a window whose row set
differs only in a descending ordinal. Negative clause: an unchanged 194 means the
realignment is not reaching the latch at the live seam.

---

## F10 — MED, T4: verdicts counted for prompts that never existed — **CONFIRM**

**RED.** `#W79-CZ` (T4) moved the own-loop count and the record face from the
row-build site to the splice into the prompt tail. The splice is still upstream of
every gate that discards the window: at the casting seam the honoured hold, the
reservation decline and the loop auto-pass all `return NULL` after it, and at the
priority seam the deadlock-breaker pass and the ask-cache replay (which answers from
`mLastChoice` with no round trip at all) do the same. Pinned as the wave-79 ORDER:
stamping at the splice and then suppressing leaves a counted line and a live face
for a prompt that was never sent (`#W79-DC F10 RED-ON-BASE`) — the exact
helper-versus-caller shape the wave-78 lesson forbids.

**Fix.** The face is HELD at the splice and applied by `w79ApplyLoopFaceAtSend` at
the point where the prompt is handed to `pollCompletionRetry` / `askModel`; every
suppressing path calls the same predicate with `sent == false`, which clears the
face so no later record can consume it.

**GREEN.** A suppressed window counts nothing, stamps nothing and CLEARS the face;
an asked window stamps the face its prompt carried and counts exactly one line; a
window with no verdict line stamps nothing even when it IS sent.

**Prediction.** `own_loop_verdict_lines_rendered` **> 0** and EQUAL to the number of
prompts containing `[own loop verdict:`, and the number of records carrying
`own_loop_verdict` equals that same figure. Negative clause: a count above the
literal's prompt count means a suppressing path still stamps — which is now one
predicate to inspect rather than five call sites.

---

## F11 — LOW, T17: a seat-specific meaning invented for record turns — **CONFIRM**

**RED.** The tool called the record `turn` "this SEAT's turn count". There is no
per-seat turn count in this engine: `AIPlayerGPT::translogTurn` is
`observer->turn + 1`, the GAME's turn as the narration prints it, identical for both
seats — which is the whole point of `#W51-D` (D18). results.tsv's `turn` is the raw
`game->turn` that `GameStateDuel.cpp:1722` prints on the `WAGIC_SELFPLAY_RESULT`
line at game end.

**Fix.** The explanation names both sources by their code site and the gap by its
mechanism: the only two contributors are the +1 narration offset (worth one per seat
log) and a seat whose LAST RECORD predates the final turn, which makes the record
column LOWER. `turn_gap_note` states which of the two the measured gap is.

**GREEN, on the corpus itself.** 1,016 − 974 = **42 = exactly one per seat log**, so
the gap is the offset alone and every seat's last record landed on the game's final
turn. The wave-79 run prints
`gap 42 = 42 seat logs x the +1 narration offset, exactly`. A gap BELOW the offset
names how many turns of records are missing; a gap ABOVE it is flagged as an
impossibility rather than explained away; with no results.tsv nothing is claimed.
Selftest OK (4 new checks). The reported late-check discrepancy is therefore **not**
a turn-numbering artefact and DB's diagnosis was looking in the wrong place.

**Prediction.** The wave-79 census prints a `gap N = N seat logs x the +1 narration
offset` line whose N equals the seat-log count, i.e. **> 0** and equal to the number
of clean seat logs. Negative clause: a `BELOW`/`EXCEEDS` branch firing means the
wave-79 corpus genuinely differs from wave 78 here, and the late-check population
must be re-derived rather than re-explained.

---

## Weakest evidence (read before trusting the above)

1. **F1's live collectors are never executed by the gate.** `w79ContinuationDigestPriority`
   and `w79ContinuationDigestCast` walk a live `TargetChooser` and
   `ManaEngine::potentialColorReach`; the engine test suite drives `AIPlayerBaka`,
   and a lane may not run a corpus or call the pilot. They compile and link, their
   pure formatter and the whole key are pinned on corpus bytes, and
   `ask_key_continuation_differs` exists so a 0 is a FAIL — but nothing here proves
   a non-empty digest ever reached the key at the live seam. This is the S6/S3
   shape; the wave-79 census is the only instrument that closes it.
2. **F1's digest may be too coarse or too fine and no pin can tell which.** It is
   coarse where a row's chooser reaches a big zone (`library`, `stack`): a card
   drawn into hand can move a digest that no legal option depended on, which costs
   asks. It is FINE where a legal continuation lives somewhere the chooser does not
   look — an alternative cost becoming payable, a play restriction lifting, a
   replacement effect entering — and those still alias. Watch
   `identical_option_asks_resolved` and `menu_pass_no_progress` together.
3. **F1's target walk is per-row and per-tick.** Seven zones x two players x
   `canTarget` per offered row, rebuilt on every polling tick. On a wide board with
   a large library that is tens of thousands of `canTarget` calls per window. It is
   nothing beside the model call it precedes, but it was not measured, and a
   `systemd-run` kill under the 4G cap on a wide-board corpus game would point here
   first.
4. **F3's `toughness == 0` convention is load-bearing and silent.** It means "not
   modelled, the attacker cannot die", and every caller that does not fill it gets
   exactly the wave-79 behaviour. Only ONE caller fills it. If a future caller
   forgets, the DP silently returns to a one-step model with no pin failing.
5. **F3's out-of-model list is explicit and therefore incomplete.** Damage
   PREVENTION (Fog Bank's `preventAllCombatDamage`), redirection, becomes-blocked
   pumps and granted abilities are not in `w79CombatAbilityOutsideModel`, so a board
   carrying one of those still gets a figure promoted to EXACT. The list fails
   closed, which is the wrong direction for this particular claim, and widening it
   needs a reading of the ability graph this lane did not do.
6. **F3's death badge was traced to ONE site.** `no block saves you` lives inside
   `incomingCombatLine`'s `bestCaseOptimal` branch and `exactAssignment == false`
   removes it. The brief also asks for the hold latch's death-verdict key: that key
   is `crackBackVerdictKey(attackers, total, life)`, which is computed from
   crack-back totals and never reads this DP, so no change was made there. If some
   other surface reads `bestCase` as proven, it was not found.
7. **F4's give-back figure itself is unaudited.** `w79SacrificeBlockerGiveBack`
   prices cover over the attacker list INCLUDING the body this row is about to kill.
   The fix removes the false subtraction and the false death; it does not make the
   remaining `G` figure exact, and the new sentence asserts `G off your best case
   WITH blockers` without computing that best case.
8. **F7's back-reference vocabulary is hand-written.** Thirteen phrases plus a bare
   `it`. A card whose dependency is phrased some other way still prints alone, and a
   card whose `it` is not a back-reference now drags its predecessor in (or loses
   itself). The blast radius over the whole primitives pool was not measured — only
   Chrome Mox at both budgets and one independent control pair are pinned.
9. **F9's realignment is a pure string rule standing in for an instance id.** The
   brief asked for the permanent's INSTANCE id; there is no per-instance identifier
   on `MTGCardInstance` and threading one through `mHoldRows` / `mHoldLatchRows` /
   `W76HoldMemory` (all `std::set<string>`) is a refactor this lane did not attempt.
   The descending-ordinal rule is derived from the engine's renumbering semantics and
   is exact for the two cases Astra and CZ name, but it is a proxy: two genuinely
   different instances whose ordinals happen to descend with an unchanged row count
   would be merged, and that is the permissive direction.
10. **F6 and F8 both narrow a feature DB and DA shipped one wave ago, and neither
    has a corpus render to compare against.** If either clause now renders zero
    times the MED fix has cost the HIGH feature; each prediction carries that
    negative clause, but only a corpus can adjudicate it.
11. **F10's placement is the fix and the placement is not pinned.** The predicate is
    pinned on all four of its inputs; that the predicate is CALLED at the send and at
    every suppressing path is a reading of five call sites, not a test. The
    census equality (`own_loop_verdict_lines_rendered` == prompts containing the
    literal) is the only instrument that checks it.
12. **F2's pin uses synthetic threat names.** The corpus has no THREATENED →
    THREATENED pair with differing components in it (CZ's 194 re-opens were measured
    as a total, not split by face), so the shape Astra constructed is reproduced from
    the engine's own verdict builder but not from a recorded board.
