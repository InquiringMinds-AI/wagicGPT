# Wave-79 lane CZ — the window/hold family (T2, T3, T4, T13, T12, T14)

Base `06fad1449` (= master). Worktree `worktrees/lanes/w79-CZ`, branch `w79-lane-CZ`.
Every fixture below is the CORPUS's own bytes — `options_text` lifted verbatim from
`matchups-20260911-125420-final` — driven through the LIVE builders
(`joinNumberedRows` → `w77KeyTailOf` → `asyncSlotKeyOf`; `holdActionKeyRow`;
`w78HoldLatchKeys`/`w78HoldStillStands`; `w78HoldRowDelta`; the collapse identity and
its predicate), not through a re-implementation. No corpus was run and no request was
sent to the pilot.

PARSETEST **6688 passed / 0 failed** (base 6642 / 0; +46 net, of which 8 are pins whose
wave-77/78 contract this lane SUPERSEDES and rewrote in place — listed per item).

---

## T2 — the S3 collapse cannot fire on a live board

**RED (measured on the corpus, then pinned).** `stop_reached_reputs_collapsed` = **0** over
**44** asked stop-reached windows in four deck123 chains (5,448 s = 90.8 min of decode for
**3** non-pass answers). Fixture = `123v152` seqs **16 → 17** (priority, turn 9,
Upkeep → Draw), the exact prompts: one acting row
`Create human with Thraben Doomsayer [cost: Tap] … {right now: M=26, your stated stop=26,
so this window would add to a count ALREADY AT OR PAST your own stop}` plus the hold row.
The two puts' **action-key sets are identical** (`holdActionKeyRow(a[i]) == holdActionKeyRow(b[i])`
on both rows — the only delta is the `{if you pass here, …}` group). Three failing pins, all
red on base:

* `w78RePutCollapseIdentity(rows16, declineBoardScope(<upkeep board, 20 life>), 26, 9, 26, "plan")
  != w78RePutCollapseIdentity(rows17, declineBoardScope(<draw board, 19 life>), …)` — the board
  leg. (`123v162` seqs 36-41 realised it live: 20 → 19 → 18 → 17 across identical-row re-puts.)
* the same identity with `stopTurn/stopCountAtStatement` moved `9,26 → 10,27` differs — the model
  re-states its PLAN at nearly every window, re-dating its own stop.
* `!w78StopReachedRePutCollapses(true, true, 26, /*planTurn*/9, /*nowTurn*/10, 1, true)` — the
  wave-72 `planTurn == nowTurn` clause underneath refuses every window of a chain that crosses
  the turn the stop was stated on, and all four chains do.

**Fix.** The collapse identity is now `w79RePutCollapseIdentity(actingRows, carriedStop)` — the
ACTION-KEY SET (`holdActionKeyRow`: every annotation out, `[cost: …]` kept) plus the stated stop
NUMBER, and nothing else; the board and the stop's date and source are out, because a life total
that creates no row is not the question and the seat's own PLAN carries the stop across the turn
boundary. `w79StopReachedRePutCollapses` drops the same-turn clause entirely — `declinedN >= 1`
is already this-turn scoped (`mListDeclineCount` is cleared at every turn change), so the FIRST
put of a list on any turn is still asked and only its re-puts collapse.

**GREEN.** `w79RePutCollapseIdentity(rows16,26) == w79RePutCollapseIdentity(rows17,26)` and
`w79StopReachedRePutCollapses(true,true,26,1,true)` is true. Five MUST-NOT-MATCH pins hold the
"no legal option lost" line: `declinedN == 0` (first put) is asked; a moved identity is asked;
one live row of another kind / no stop-reached row / no stated stop is asked; a **changed
`[cost: …]`** on the acting row is a different identity; an **added row** is a different identity;
a different stated stop is a different question.

**Prediction (wave-79 census).** `stop_reached_reputs_collapsed` **> 0 on deck123's seats**
(expected band 20-35 against the wave-78 population of 44 asked stop-reached windows, since the
first put of the list on each turn is still asked), and no (turn, phase, seat) cell shows more
than two asks over a stop-reached menu. Negative clause: if `stop_reached_reputs_collapsed`
reads 0 again the fix did not reach the live caller — that is a FAIL, not "untested".

---

## T3 — the CG shape, fourth wave (CY F8 FAIL), and the hold latch under a moving board

**RED (measured).** Recounted independently over the 42 seat logs: **229 byte-identical** +
**186 strip-identical** consecutive same-seam same-turn re-asks (the seat's 233/186, within my
seam proxy). Split by PHASE — which matters, because the engine's own decline scope deliberately
grants one fresh offer per phase:

| | same turn, SAME phase | same turn, different phase |
|---|---|---|
| byte-identical | **135** (9,911 s of decode) | 94 |
| identical after `{…}`/`[…]` strip | **98** (8,828 s) | 88 |

The 233 same-phase pairs — **5.2 hours of decode** — are questions that had not moved at all.
Three named bytes, each a corpus pair, each driven through the live key builders:

* **(a) a `[...]` render note is inside the key.** `123v125` seqs **59 → 62** (cast, T12, Main 1):
  identical but for `[this cannot target the spell on the stack - battlefield permanents only] `
  on row 1. RED: the wave-78 key tail (`w76StripBalancedAnnotationGroups ∘ stripDeclineReaskTags ∘
  w78StripSharedCardTextHeader`) takes `{…}` groups only, so the two tails differ and both the ask
  key and the async slot key moved.
* **(b) the board half.** `123v126` seqs **93 → 96** (priority, T13, Main 1): rows BYTE-IDENTICAL,
  asked twice. RED: `serializeGameState() + tail` differs on one life tick (13 → 14).
* **(c) the hold latch.** The same pair: seq 93 took the hold, seq 96 was asked again. stderr
  across the corpus: **194** `hold re-opened … a printed row changed or is newly available`
  + 24 `a printed row it was held over is gone`, over 87 holds taken at deck123 (56 followed at
  once by a byte-identical menu). The mover is not a row — it is one of the three synthetic
  verdict markers the latch compares beside the rows, and `w77OwnLoopVerdictKey` carried the
  NAMES of whatever sat on the opponent's stack.

**Fix (three parts, one rule — a window's key is its ACTION-KEY SET at its seam).**
1. `w77KeyTailOf` now reduces every numbered row of the tail through `holdActionKeyRow`
   (`w79ActionNormalisedKeyTail`): `[…]` and `{…}` out, target-preview numbers out, life
   projections normalised, `[cost: …]` KEPT. Non-row lines are byte-untouched, so the question
   head (`Land drop: `, which `asyncLandArm` reads) and the protocol's closing sentence are
   unchanged.
2. The ask key's and the async slot key's board half is `w79AskScopeKey(turn, phase)` — the seam,
   not the serialised board. `boardKey`/`boardStateKey` are still computed and still carry every
   other consumer (the decline scope, the sibling rule, the cross-phase entry).
3. The own-loop marker is a FACE with no names, and the latch compares each verdict marker
   against the face the hold was TAKEN over through `w79HoldVerdictForCompare`: a move DOWN the
   family's danger ladder (or a lateral move) is not a re-opener, a move UP it still is —
   #W68-BB (J9)'s obligation is one-way and stays one-way.

**GREEN.** On (a): `w77KeyTailOf(tA) == w77KeyTailOf(tB)` and the two `asyncSlotKeyOf` values are
equal. On (b): `w79AskScopeKey(13,3) + keyTail` is one key for both. On (c): the LIVE
`w78HoldStillStands(w78HoldLatchKeys(rows93 + heldMarker), rows96 + clampedMarker)` holds.
MUST-NOT-MATCH: a changed `[cost: …]` is a different key; a menu that LOST a row is a different
key; `w79AskScopeKey(13,3) != w79AskScopeKey(13,4)` and `!= w79AskScopeKey(14,3)`; an ordinary
row is never clamped; the escalation direction still re-opens (held `you survive` → live `LETHAL`
fails `w78HoldStillStands` on the same two menus).

**Superseded pins rewritten in place:** `#W77-CU F1 KEY` and `#W78-CY F8` (the key tail now
lower-cases through the action key, so the same assertion reads
`cast hammer of bogardan {2}{r}{r}` / `equip with bonesplitter`); `#W77-CU F4 GREEN the threatened
marker names the spell and the piece` → the marker is the face, the RENDERED line keeps both
names; `#W78-CY F4`'s last clause (`a different spell above the trigger is a different key`) →
the same face with a different spell is now the SAME key.

**Prediction (wave-79 census).** Same-turn **same-phase** byte-identical re-asks → **0** (was 135)
and strip-identical same-phase re-asks → **0** (was 98); `ask_replays_reserved` up by **≥ 135**
over the wave-78 figure of 1,344; `hold_verdict_safer_ignored` **> 0** (expected ≥ 20 against the
194 re-opens). Cross-phase re-asks are NOT predicted to fall — a phase change is a different seam
by the engine's own decline-scope rule, and 182 of the 419 are that. Negative clause: a
same-phase byte-identical re-ask that still appears means the key did not reach the live caller.

---

## T4 — the own-loop verdict that never rendered

**RED.** `[own loop verdict:` occurs **0 times in all 42 seat logs** while
`own_loop_windows_asked` read 2, and CV's wave-78 pin asserted rendered bytes and passed. Two
causes, both pinned:
* `w77CountOwnLoopWindow` is called at the ROW-BUILD site, ahead of every collapse/hold gate, and
  `mWindowSeq` only advances when a record is WRITTEN — so a counted window that is then skipped
  writes no record and no prompt, and its count lands on whatever window is written next. The
  population and the render were never the same fact.
* `w78OwnLoopVerdictLine` returns the empty string on three live shapes: a THREATENED or UNPROVEN
  verdict whose card names the walk could not fill, and a proven loop with nothing of it on the
  stack. Pinned RED: all three are `.empty()` on base.

**Fix.** `w79OwnLoopVerdictLineFor(state, loopProven, spell, piece)` renders a true face for every
shape of a PROVEN loop, name or no name (a silent omission is worse than wrong text — #W62-AC);
the counter `own_loop_verdict_lines_rendered` is incremented AT THE SPLICE on both seams
(priority and cast), so the figure is by construction the number of prompts the line is in; and
the face is stamped onto the record as **`own_loop_verdict`**, so the windows wave 78 could not
locate are a joinable field.

**GREEN.** All four faces render non-empty when the loop is proven; the idle face is pinned
verbatim; a named threat still renders its names. MUST-NOT-MATCH: a seat with **no** proven loop
renders nothing on any face.

**Prediction.** `own_loop_verdict_lines_rendered` **≥ 2** and equal to the number of prompts
containing `[own loop verdict:`; **≥ 1 record carries `own_loop_verdict`** on deck126's seats.
Negative clause: `own_loop_verdict_lines_rendered > 0` with zero prompts carrying the literal
means the splice is being built and dropped, which is CV's untested hypothesis, finally testable.

---

## T13 — the drain clause's `HOLD covers every link` promise

**RED.** `123v125` seqs **379 → 382** and **395 → 398**: byte-identical four-row menus one TURN
apart (T51 → T52 and T53 → T54, both Upkeep), the hold between them retired by the seat's own
untap release (`releaseHoldIfUntapPassed`, #W73-BY N2a). The clause promised coverage with no end
and the corpus falsified it across the release.

**Fix.** T3's latch fix makes the hold survive the drain WITHIN a stack, so the promise is kept
rather than dropped — and it is SCOPED to what is true: `covers every link OF THIS STACK, and is
released at your next untap - a new stack on a later turn asks you again`.

**GREEN.** `#W79-CZ T13 GREEN the drain clause, verbatim` (the wave-78 verbatim pin rewritten to
the new bytes; its MUST-NOT-MATCH clauses — one link is not a drain, a moved menu is not a re-put,
no hold row means no clause — are untouched and still pass).

**Prediction.** Every rendered drain clause carries `OF THIS STACK` and `released at your next
untap`; `stack_drain_windows_asked` > 0 with ≥ 1 rendered clause; **0** drain-hold pairs whose
re-ask is inside the SAME turn (the wave-78 pairs were both across a turn boundary).

---

## T12 — the occurrence index over-merges, and the cost-change sentence

**RED.** `125v123` seq **263** (referent 260): the two acting rows went from
`deal 1 damage with Staff of Nin targeting …` to `… Staff of Nin #2 targeting …` — a second Staff
had entered and the FIRST one's rows were no longer offered — and the bracket printed
`every row above was also on the menu … and no row that was on it is gone`.
`w78StripHandleOrdinal` dropped every ordinal, so the rows merged. Separately, `152v125` seqs
65 → 66: the same sentence calls a cost change both `the SAME row` (`a price` was in its
same-row list) and `a different cost` (in its re-opener list).

**Fix.** Only ` #1` is a renumber artefact — Wagic prints no ordinal on a lone copy and appends
`#1` the moment `#2` arrives — so ` #1` and no-ordinal normalise together and **every higher
ordinal is a different instance**. And the bracket's sentence names the cost ONCE, as a
re-opener: `a price` is out of the same-row list, `the same action at a CHANGED COST is a
different row` is in the re-opener list, in all three of its branches.

**GREEN.** `w78StripHandleOrdinal("… staff of nin #2 …")` is unchanged; `"… #1 …"` still strips;
`w78HoldRowDelta(last, now)` on the corpus rows reads **2 new / 2 gone**. MUST-NOT-MATCH: the
renumber-in-place case (one row → `#1` + `#2`) still reads exactly **1 new / 0 gone**, so wave
78's fix is kept; the sentence contains no `a price` and does contain
`CHANGED COST is a different row`.
**Superseded pins rewritten in place:** `#W78-CY F5`'s two `Goblin #1 → Goblin #2` pins, which
asserted that pair is the SAME row — that reading is exactly MED-1. A new pin covers the
renumber case they were reaching for (no-ordinal → `#1` still stands, bracket and latch agree).

**Prediction.** ≥ 1 hold-check bracket prints both `is new` and `is gone` on a window where a
second instance arrived; **0** brackets print `no row that was on it is gone` over a menu whose
rows gained a `#2`; **0** prompts contain the string `(a price,`.

---

## T14 — `0 unrecorded`, `declined_face_latches`, and why phase 2 read 0

**RED.** The game-end stderr line printed `deadline misses: N (0 unrecorded, M no-retry)` — that
`unrecorded` is `mWallMissUnrecorded`, the DEADLINE-MISS population — in a wave whose
`forced_close_unrecorded` was **16**. Two different "unrecorded"s on one surface; the wave-78 seat
had to open both the records and the stderr to tell them apart.

**Fix.** The line names each by its own mechanism and prints the force-close identity beside it:
`… (N wall-miss unrecorded, M no-retry); forced closes: E events, U superseded
(dropped_decision_moved); phase-2 answer recovery: R recovered, X missing`. The
`declined_face_latches` gameend field is **RETIRED** (0 for six waves — wave-78 T18); the
AIPlayerBaka-side latch and its own DebugTrace are untouched.

**Why `phase2_answer_recovered` read 0 / `_missing` 0** (wave 77: 15 / 1) — named, as T14 asks,
and this lane's reading, not an owner position: the phase-2 second call is entered only when a
reply ARRIVES WITHOUT AN ANSWER SEGMENT. Wave 78 had `reply_truncated` **0**, `transport`
`http=200, empty=0` on all 2,629 calls and `deadline_pct` max 43.7 — no reply arrived truncated.
The 16 budget truncations that did occur were all taken by the FORCE-CLOSE arm (16 `armed` ==
16 stderr `unclosed <think>` lines), and every one of those 16 then read `dropped_decision_moved`
— the decision moved out from under the pending retry before the close could be consumed. So the
path was never entered because its precondition never occurred, not because it broke. It stays
**UNTESTED**, and the stderr line above now carries the two figures so the next seat can say so
without a record join.

**Prediction.** Every game-end stderr line carries `wall-miss unrecorded` and a
`forced closes: … superseded (dropped_decision_moved)` clause; **0** gameend records carry
`declined_face_latches`; the force-close identity still closes to the byte
(`armed == stderr "unclosed <think>" lines`, `armed + superseded == forced_close_events`).

---

## Weakest evidence (in order)

1. **T3's ask-key change is the largest blast radius in this lane and is pinned only on key
   EQUALITY, never on live play.** Dropping the serialised board means that within one turn and
   phase, a cached answer is replayed over any board whose action rows match. The bound on that is
   `askReplayRefuseScoped(…, kAskReplayRefuseMax)`, which this lane did not re-measure. A corpus is
   the only instrument that can say whether a replayed answer was ever wrong; `ask_replays_reserved`
   rising is NOT by itself evidence the replays were right. Watch `identical_option_asks_resolved`
   and any rise in `menu_pass_no_progress` / deadlock-breaker traces.
2. **T2's collapse now suppresses windows ACROSS PHASES within one turn** (the 16 → 17 fixture is
   Upkeep → Draw). The corpus says those windows were answered `pass` 41 times in 44, but "the
   model would have passed" is inference from the wave-78 population, not proof about wave 79.
   The falsifier is a deck123 game that loses a token-maker activation it wanted after the first
   put of a turn.
3. **T4's prediction depends on `lifeLoopProvenWin(this)` being true at an ASKED window.** The idle
   face now makes that sufficient, but if deck126 draws no loop this corpus the counter reads 0 for
   want of a board, not for want of a fix — the distinguishing evidence is
   `own_loop_windows_asked` also reading 0.
4. **The verdict-rank clamp is pinned on the three known families and their known face strings.**
   A future face worded differently falls to `rank == -1` (never clamped, always a re-opener) —
   the safe direction, but it means a reworded verdict silently re-opens holds again.
5. **T13 is a wording change adjudicated against two corpus pairs on one seat** (`123v125`). The
   claim "the hold now survives a drain within one stack" rides on T3's latch clamp and has no
   in-stack corpus pair of its own — the wave-78 pairs are both across the untap release.
6. **T12's `#1` rule is derived from the corpus's rendered bytes, not from the engine's naming
   code.** If Wagic ever prints `#1` for a lone copy (rather than only after `#2` arrives), the
   strip would merge a lone `#1` with a later `#1`-of-two. No corpus row contradicts it, and I did
   not read the disambiguation site.
7. **No live path of any item was exercised.** Every pin is a pure-function or builder-level
   assertion over corpus bytes; the LIVE callers were read, not run. Wave 78's own lesson is that
   this is exactly how S3 and S6 shipped green and did nothing — the wave-79 census is the gate.
