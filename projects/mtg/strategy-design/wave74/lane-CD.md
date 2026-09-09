# Wave-74 lane CD — the window model (O2, O4, O6, O10, O13, O23, O26)

Base: master `c0551632e`. Worktree `worktrees/lanes/w74-CD`, branch `w74-lane-CD`.
Gates on the finished tree: **suite THREADS=1 — 1283 tests 0 failed, 76 AI tests 0 failed;
both failure strings (`==Test Failed !==`, `==Test timed out`) count 0** (the three known
flakes all passed on this run). **PARSETEST 5615 passed, 0 failed** (base 5593; +22 in seven
blocks: O13 7, O10 4, O6 3, O23 3, O4 5). `git diff | grep -c $'\357\277\275'` = 0.

Everything below was run under `systemd-run --user --scope -q -p MemoryMax=4G -p
MemorySwapMax=0 --`; no run was killed under the cap.

---

## O2 HIGH — the declare-attackers window that is silently never asked

### Repro
`152v162` seq 9 (T5) and `152v123` seqs at T15/T17, the three the deck152 seat named. Two
facts from the corpus resolve it, and both contradict the review's own hypothesis:

1. **The suppression is NOT upstream of `pendingCombatDecision`.** The gate *passed* - it
   only traces on suppression, so a window lost after the gate leaves no line at all. That
   is why "no `[combattrace]` of any reason" looked like an upstream skip.
2. **All three turns end on the same stderr line**: `AIPlayerGPT: unclosed <think>
   (budget/truncation); forcing the answer`, and the next line is the next `Untap Phase`.
   `game-152v162-1788839803.stderr:558` (its ONLY force close in the game, at the T5 window),
   `game-152v123-1788839827.stderr:1199` (last line of the turn = rec T15) and `:1703`
   (rec T17). The one force close in that file that did NOT lose the attack (`:1736`) was
   followed by `declared attack (25 of 28)`.

### Mechanism (traced in code, then measured)
`pollCompletionRetry` has three arms that schedule a SECOND LEG - the two-phase forced close
(AIPlayerGPT.cpp), the transport/deadline retry, and the answer-ceiling re-ask. All three
**consume the first reply, arm `mRetryActivePrompt`, and return `kChoicePending`**. The async
slot is empty at that moment, so `decisionPending(0)` is FALSE, and
`AIPlayerBaka::Act`'s empty-clickstream branch commits `userRequestNextGamePhase()` on that
same tick - closing the step the pending decision belonged to.

That is only half. The retry state (`mRetryActivePrompt`/`mRetryBase`) is ONE per-seat slot,
while the async slots are split per arm (`#W57-A` D5: land-drop vs casting). The seam that
runs on the next tick - whichever arm - compares `userMsg == mRetryBase`, fails, and takes the
"decision changed under a pending retry: drop it" branch on the OTHER arm's leg. A temporary
probe printed the two prompts at the drop: they alternate 3244 B / 4645 B with a first
difference at byte 2183, and the two bodies are the `Land drop:` question and the
`Your legal actions (Main phase 1, YOUR turn)` question. Two arms alternating tick by tick
destroy each other's armed leg for ever.

### RED on base
Corpus-wide, on the untouched wave-73 logs: **33 forced closes issued (stderr), 2 phase-2
answers ever consumed** (`phase2_answer_recovered` 2 + `phase2_answer_missing` 0 summed over
all 42 gameends). 31 armed legs vanished.

Reproduced deterministically off the corpus with a stub endpoint
(`~/.gatelogs/w74cd/stub.py`: `/v1/models` answers, and every `/v1/chat/completions` whose
last message is a `user` message returns an UNCLOSED `<think>` with `finish_reason=length`,
so every decision must take the forced close; the phase-2 leg - the assistant-prefill request -
is answered). One pinned selfplay one-shot, deck152 vs deck162, `WAGIC_AI=gpt`:

| binary | force closes armed | phase-2 requests the stub received | ratio |
|---|---|---|---|
| base `c0551632e` | 409 | 38 | **9.3%** |
| fixed (this branch) | 7276 | 7092 | **97.5%** |

The base run also declared **0** attacks in the whole game.

### Fix
1. `AIPlayerBaka::decisionArmed()` (virtual, Baka false; `AIPlayerGPT` = a non-empty
   `mRetryActivePrompt`), consulted at the **post-computeActions pass gate only** - never at
   Act entry, or the armed leg would never be polled and the seat would deadlock. Bounded by
   `kRetryArmedHoldMs` = 2000 ms (time, not ticks, so the gate and the trace can both ask in
   one tick without the reads disagreeing); when the bound fires the pass goes through exactly
   as before, so nothing is removed, only deferred.
2. `mRetryArmLand` records WHICH arm armed the leg; `pollCompletionRetry` only consumes or
   abandons a leg whose arm matches the current `asyncLandArm(mPromptTail)`. The other arm
   polls its own slot and leaves it alone.
3. **New diagnostic** `[combatentry] t<N> seat=<p> PASSING declare-attackers with a legal
   attacker on the board (pending=%d armed=%d)`, dev-build only
   (`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`), once per (turn, seat), fired where the
   window actually dies. Guarded by a new `attackDeclarationAnswered()` hook
   (`mAttacksDoneTurn == observer->turn` on the GPT seat) so a deliberate no-attack turn is
   not reported as a lost window. This is the entry-side counterpart the brief asked for: the
   [combattrace] tracer cannot see a window lost with the gate OPEN.

### GREEN
Stub A/B above (9.3% -> 97.5%); `[combatentry]` fired 0 times in the fixed run and 4 times in
the intermediate build that had only fix (1) - which is how fix (2) was found. Suite 0 failed,
PARSETEST 0 failed. A control run of both binaries against a stub that answers directly (no
forced close) finishes the game normally on both (56 and 24 decisions), so the ordinary path
is unchanged.

### Prediction (falsifiable, next corpus)
`phase2_answer_recovered + phase2_answer_missing` summed over all gameends will be **>= 80% of
the count of `unclosed <think>` lines** in the run's stderr (wave 73: 2 of 33 = 6%), and no
seat log will show a turn on which the seat held a legal attacker, made no declaration, and
left a `[combatentry] ... PASSING declare-attackers` line naming `armed=1`.

---

## O4 HIGH — the land-drop menu hides an MDFC pathway's second face

### Repro
`146v162` seq 3: `1. Play Brightclimb Pathway / 2. Play Plains / 3. Play no land right now` -
no {B} face. Seq 4 then puts the full two-face menu, and the seq-3 answer anchors it. Same
double-ask at deck152 17->18, deck126 23->24 and 31->32, deck130 29->30.

`legalLandPlays` sees the card once, under its front-face name; the back face reaches the
battlefield through the MDFC flip ability, not through a land play, so it cannot appear on
that menu as its own row.

### Fix (renders one more true fact; removes nothing)
`w74MdfcLandBackFace()` reads the card's own MDFC land-play ability off the action layer (the
same `asMdfcLandPlay` the casting menu annotates) and resolves the name/tap-mana through the
collection; `w74MdfcLandDropFaceTag()` appends to the land-drop row:
`{TWO FACES, ONE CARD: this card's other face is "Grimclimb Pathway" (taps for {B}), also a
land. Taking this row does NOT pick a face - the next window asks which face enters the
battlefield, and either face spends this same land drop. Choose the face there, not here.}`

### GREEN
Live-traced, not just compiled: one pinned deck146-vs-deck162 selfplay game against the direct
stub, translog on - seq 30 renders
`Play Brightclimb Pathway {TWO FACES, ONE CARD: this card's other face is "Grimclimb Pathway"
(taps for {B}), also a land. ...}` on the exact card of the repro. PARSETEST +5.

### Prediction
On the next corpus every land-drop menu whose row is followed by a face menu in the next
record will carry the `TWO FACES, ONE CARD` tag, and the reasoning at that second window will
stop treating the first answer as a face commitment ("I'll stick with 1" / "Actually, I'll go
with Brightclimb" is the wave-73 shape). Deck-face double-asks stay - only the anchoring goes.

---

## O6 HIGH — the hold-check bracket ignores the carve-out its own contract prints

### Repro
`130v162` seqs 36->37, 39->40, 40->41, 41->42. Bracket says `4 rows above are new`; the
rendered menus differ in one row's projected life total.

### Mechanism
`holdStillStands` (the LATCH) routes both sides through `holdKeyRow`, which already normalises
`you would be at N`. `holdReopenNote` (the BRACKET) compared **raw row text** on both the
membership test and the memory it stores. Two delta classes on those windows, diffed byte for
byte off the corpus:
- `you would be at 12` -> `11` (rows 8, 9, 11)
- ` {if you pass here, this option is not offered again until the board changes}` present at
  seq 36 and absent at seq 37 (rows 1 and 11) - a statement about the ASK'S bookkeeping, not
  about what the row offers.

### RED on base
Replaying the base key over the four real window pairs: **11 rows "new" on every pair**. With
the fix's key: **0 on every pair**.

| pair | base key | fixed key |
|---|---|---|
| 36->37 | 11 new | 0 new |
| 39->40 | 11 new | 0 new |
| 40->41 | 11 new | 0 new |
| 41->42 | 11 new | 0 new |

### Fix
`w74HoldUnseenRows()` (pure) counts rows absent from the held set **through `holdKeyRow`**, and
`holdReopenNote` stores `holdKeyRow(row)` in its per-seam memory - one key on both sides, which
is also the "count only rows genuinely absent" half. `holdKeyRow` gains the last-offer clause
as its THIRD named erased clause (beside the combat-next and stack-target clauses), so the
latch and the bracket forgive the same thing. Every price, kill count, damage figure, survivor
count and verdict word still compares byte for byte.

### GREEN
PARSETEST +3, pinned on the seq-36/37 rows verbatim, including a MUST-NOT-MATCH that a verdict
which really moved (`SURVIVES` -> `DIES`) still counts as new, and counts once.

### Prediction
Next corpus: on any run of consecutive priority windows at one seam whose rendered rows differ
only by projected life totals, `[hold check: ...]` prints `every row above was also on the
menu ... (N windows in a row now)` with N advancing, instead of `k rows above are new`; the
deck130 chain's 11 windows across two turns collapse to at most the 2 windows where a verdict
actually moved.

---

## O10 MED — `main_phase_windows_skipped` counts phases that were not skipped

### Repro
Engine seat HIGH-2: 233 of 375 counted phases DID put a Casting decision to the model (honest
figure 142). deck126 MED-3: 58 skips alongside 640 holds in one game, 0 where holds were 0.

### Fix
1. `noteMainPhaseCastingOffered()` is now called from the **instant-speed response arm** as
   well, immediately before its `FindCardToPlay(icurrentMana, "")` - which IS the cast ask for
   the GPT seat (c5c). Same phase scope (`ownMainPhaseNow`) as the standard branch's call.
2. `noteMainPhaseHoldSuppressed()` records the (turn, phase) whose window this seat's own hold
   latch closed (both `holdHonoured` suppression sites), and `flushMainPhaseSkip` exempts it -
   a held phase is not a swallowed phase, and the two meters were double-counting one event.
3. The verdict is now the pure `w74MainPhaseSkipCounts(castingWasOffered, heldByOwnLatch)`.

### RED on base
The base counter's own corpus reading is the red: 375 counted, 233 of them demonstrably
offering a casting decision, 58-with-640-holds in one game. There is no fixture for this - it
is a meter, and the meter's disagreement with the translog IS the failing measurement.

### GREEN
PARSETEST +4 pinning both exemptions and their MUST-NOT-MATCH forms. Suite 0 failed.

### Prediction
Next corpus: cross-referencing every counted `(turn, phase)` against `Casting decision (Main
phase N, YOUR turn)` headers in the same seat's translog yields **0** counted phases that did
offer one (wave 73: 233 of 375), and a seat with high `hold_windows_skipped` no longer carries
a proportional `main_phase_windows_skipped` (deck126's 58-alongside-640 goes to ~0).

---

## O13 MED — MEASURE ONLY: the decision-empty chain window

No collapse, no behaviour change: the legal-option boundary forbids removing any of these rows
and this lane does not.

New gameend fields, counted only while a chain is resolving (the population
`chain_windows_collapsed` reports 0 for):
- `chain_windows_only_selfharm` - windows where EVERY acting row is decision-empty
- `chain_selfharm_rows` / `chain_acting_rows` - the row totals behind that verdict

`w74RowIsDecisionEmpty()` reads the row's OWN rendered verdict, never a re-derivation:
`{this hits YOUR permanent}`, `targeting you `, `SURVIVES`, `[DRAW PRICE`.

**Deliberately NOT covered, and this is why the all-rows counter is a LOWER BOUND**: row 7 of
`130v162` seq 37 - `targeting the opponent {right now: takes 2 damage - they would be at 15}` -
is a real chip at a face 17 away. Calling it empty needs a threshold nobody has agreed, so it
is not called empty, which means the strict counter reads 0 on the very windows that motivated
it and the two ROW counters are what carry the signal there (10 of 11 acting rows empty).

PARSETEST +7, all seven rows of that menu verbatim, including two MUST-NOT-MATCH rows.

### Prediction
Next corpus: `chain_acting_rows` is non-zero wherever `chain_windows_collapsed` is 0 on a
punisher board, and `chain_selfharm_rows / chain_acting_rows` on the deck130-style windows is
>= 0.8 while `chain_windows_only_selfharm` stays low - which is the measurement that says
whether a future collapse predicate should be row-fraction based rather than all-or-nothing.

---

## O23 LOW — the announced-X abandonment the instrument cannot see

### Repro
`130v162` seq 52 -> 53. 9 mana floated, the ANNOUNCE_X menu was put to the model, and the
model took that menu's own `Decline - do not cast this after all` row (`chosen_text` on seq 53,
verbatim). `grep -c 'dropped after its X was announced'` over every stderr in the corpus = 0.

### The path
`MTGRules.cpp:475` fires only inside `MTGPutInPlayRule::reactToClick`, i.e. only when a CLICK
arrives on a card whose `setX > -1` and which is no longer castable. The abandonment here is a
MENU ANSWER, in `AIPlayerGPT`'s ANNOUNCE_X branch (`if (req.canDecline && pick ==
(int) xRowCount)`), which never touches that rule. That is why the instrument reads 0 while an
abandonment demonstrably happened.

### Fix
`w74XAbandonTraceLine()` (pure) emitted at that branch, dev-build only. The **search string is
byte-identical** to the rules file's (`dropped after its X was announced`) so every existing
census keys on both paths; the line names the card and the floating mana the decline strands -
the payment-loss figure the carried docket wants. PARSETEST +3.

### Prediction
Next corpus: `grep -c 'dropped after its X was announced'` is non-zero and equals the number of
translog records whose `chosen_text` starts `Decline - do not cast this after all`, plus any
click-path drops. If it is STILL 0 while such a record exists, the branch is not the path and
the docket moves to `applyMenuChoice`'s Cancel click.

---

## O26 MED (harness) — the pilot-stall tripwire

`projects/mtg/tools/selfplay-harness.sh`. `bash -n` clean.

(a) `pilot_stall_verdict()` + `pilot_stall_sweep()`, run from the supervisor's 45 s loop beside
`no_progress_sweep` and `regime_gate_sweep`. Predicate: pool the last 3 decision records of
every seat log of THIS run **that has no `gameend`** (only a live game can be wedged, and
pooling finished seats dilutes the live ones out of the verdict); if at least K (default 6,
`WAGIC_PILOT_STALL_K`) records are pooled and every one carries an endpoint-silence fallback,
print `== PILOT STALL` with the count, `touch $OUTDIR/PILOT-STALL` and `kill -TERM` the
harness. A terminal banner beside the regime-gate and infeasible ones exits 1.

The silence classes are `timeout`, `wall_miss_no_retry`, `wall_miss_unrecorded`, `empty_reply`,
`http_error` - **not** `unparsed_reply`, which is the model answering badly, not a wedge.
`kind: recovery` records are skipped: they are bookkeeping written BESIDE the failed decision.

(b) The results banner now prints a per-seat-log `timeout fallbacks` table
("these games are NOT clean corpus games"), or "none" - the wave-73 banner said nothing while
six games had been played out by the heuristic.

### RED on base / GREEN
Run against the two REAL corpus directories:

| directory | verdict |
|---|---|
| `matchups-20260907-225637` (the wedged run, as harvested) | **`STALL 36`** |
| `matchups-20260907-225637-final` (the clean 21-game corpus) | `OK 0` |

The tripwire would have fired on the incident. Two intermediate predicates were rejected by
this same check and are worth recording: keying on the word `timeout` alone reads the wedged
run as healthy (its tails alternate `wall_miss_no_retry` / `recovery` / `timeout`), and not
excluding finished seat logs also reads it as healthy (`OK 12`).

`--selftest`: `pilot-stall-selftest: 7 checks, 0 failed` (all-timeout tails, a live answer in
the pool, too few records to judge, the previous-run filter, the per-game count, the real
wave-73 tail shape with its `recovery` record, and a finished seat log). Full flag:
`harvest-selftest 4/0`, `pilot-stall-selftest 7/0`, `regime-gate selftest 30/0`.

---

## Weakest evidence — what I did NOT test

- **O2 has no suite fixture.** The shape needs a live endpoint returning a truncated
  reasoning-only reply; the `[AI]` harness cannot drive `AIPlayerGPT` at all. The evidence is
  the corpus ratio (33/2) plus the stub A/B (409/38 -> 7276/7092). The stub is my own
  instrument, so it can be wrong in ways the real pilot is not; the honest claim is that the
  phase-2 DELIVERY RATE moved, not that three specific attack windows are now asked.
- **O2's second half is not proven to be the whole cause.** Fix (1) alone left `[combatentry]`
  firing 4 times; fix (2) took it to 0 in one game. One game is one sample. If the next corpus
  still shows an attackers window lost with `armed=0` on the trace, there is a third path.
- **The `kRetryArmedHoldMs` = 2000 ms floor never fired in any run I made.** Its behaviour under
  a leg whose seam is genuinely never re-entered is reasoned, not observed.
- **O4's premise that the face menu ALWAYS follows the land-drop row** is read off five corpus
  records, not off the engine. If a two-faced land can ever be committed straight from that
  row, the new sentence is false for that case - it is the one clause in this diff that could
  become a rendered non-fact.
- **O4 was verified on ONE card** (Brightclimb / Grimclimb Pathway). Emeria / Agadeem /
  Sea Gate style MDFCs, whose back face carries a pay-3-life arrival menu, were not seen live -
  their tag will render, but I have not read one.
- **O6's fix is proven on the four `130v162` pairs only.** I did not sweep every hold-bracket
  render in the corpus for other drift classes; a third clause of the same kind would still
  re-open holds.
- **O10 has no live re-measurement.** Both halves are pure-function pins plus a call-site move;
  the numbers that matter (233 -> 0, deck126's 58 -> ~0) are predictions, not results.
- **O13 is a lower bound by construction** (see above), and no game in any run I made produced
  a chain window, so the three new fields have never been observed non-zero. They compile and
  their classifier is pinned; the counter itself is untested end to end.
- **O23's new line has never fired.** No run of mine reached an ANNOUNCE_X decline; only the
  pure text is pinned. Whether `req.canDecline && pick == xRowCount` is the path the wave-73
  abandonment took is read off the seq-53 `chosen_text` matching that branch's row text
  verbatim - strong, but not executed.
- **O26's sweep was validated on harvested directories, never in a live supervisor loop.** The
  45 s cadence, the `kill -TERM` reaching the harness, and the `PILOT-STALL` banner path are
  untested against a running corpus - and no corpus was run this lane.
- **No corpus was run** (per the brief), so nothing here is validated against real pilot play.
- The three known suite flakes (lifeline, merrow_reejerey, intrepid_adversary_repeated_payment)
  happened to pass on this run, so this gate did not exercise them.
