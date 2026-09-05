# Wave-61 lane U — C10 (sweeper marker + damage-wipe survivors), C13 (transport), C14 (economy)

Base: master `718e99d2a` (wave-60 close). Branch `w61-lane-U`.
Files: `src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`. No Res asset, no card script, no engine
rule outside the GPT seat.

Every item here lives inside `AIPlayerGPT` (render, latch, translog record). The suite has no
network seat — `askModel` returns at `mEndpoint.empty()` before any of this code, and no fixture
can make a curl failure or a model reply happen — so PARSETEST is the gate, the same split lanes
H (wave 59) and M/Q (wave 60) took for the same reason. What that costs is stated under
"RED evidence" and "What I did NOT verify" rather than papered over.

---

## C10a — the sweeper CAST row is ranked, in the channel the pilot demonstrably reads

**The finding.** deck130 was offered `Cast Starstorm` in **51 windows and cast it 0 times**,
including three whose own `{X pricing:}` named two or more of THEIRS — two of them the deciding
decision of a loss — while the `[<- ...]` marker on the ANNOUNCE_X menu one screen later was
obeyed **6 of 6** and the guide prose written at that seam fired **0 of 51**
(`wave60/deck130/review.md` HIGH-2). The cast row already carried `[<- best X for this cast: ...]`
(#W57-E D20) — but that marker answers **which X**, never **whether**, and rows 1..N of a cast
menu are in engine-internal order with no ranking of any kind between them.

**Mechanism.** The cast-row loop now carries two more index-parallel vectors beside
`rowUses`/`rowNames`/`rowCosts` — `rowSweepTheirs` / `rowSweepMine` — filled by the **same walk
that rendered the row's own clause**, so the marker and the clause underneath it cannot disagree:

* a `destroy`/`bury`/`moveto(exile) all(creature)` sweeper fills them from `boardCreatureCounts`
  inside `boardTurnOnClause` (indestructible creatures are already out of that count, #W60-Q R6);
* an `{X}` damage sweeper fills them from `xTradeCountsAt` at the X **its own marker names**
  (`xMenuMarkX`), so the two brackets on that row can never name different X values;
* every other row leaves `theirs = -1` and takes no part in the ranking.

`applyBoardSweepMark` then runs on the menu copy, beside `applyMenuFitTags` and
`applyDuplicateEffectTags` and for the same reason (a row cannot know its own number until the
suppression filter and any re-ask removal have settled). Exactly ONE row is marked: largest
THEIRS, ties to the smaller YOURS, then to the earlier row.

The marker is a FACT about the menu with its **scope stated**, never an instruction:

```
 [<- board sweep: THEIRS 4 / YOURS 1 - the only row on this menu that prices a board sweep]
 [<- board sweep: THEIRS 5 / YOURS 2 - no other row on this menu prices a bigger sweep of THEIRS]
```

Two truthfulness rails: the comparison names the measured population ("prices a board sweep")
because only board-sweep rows are measured — a row that removes creatures without pricing one is
never implicitly claimed to remove fewer; and the wording is tie-safe (`no other row ... prices a
bigger sweep`, the `xMonotoneMarker` idiom) so it stays true when two rows match. A sweep whose
YOURS exceeds its THEIRS is still the biggest sweep on the menu, so it is still marked — and
**priced in the same bracket** (`(it takes more of YOURS than of THEIRS)`) rather than endorsed
bare, which is the wave-60 lesson from the best-X badge that recommended a lethal NET. A sweep
that removes 0 of THEIRS is marked on no row (`{dead right now: destroys 0 ...}` is not dressed
as an answer).

**Deliberately out of scope:** the attack-punisher class (Lightmine Field) takes NO part in the
ranking. What it removes is conditional on the opponent attacking with everything, and ranking
that hypothetical beside a sweeper's present-tense removal would be a true statement in the wrong
scope. Its own question is answered by C10b instead.

## C10b — a damage wipe names its survivors, and what still gets through

**The finding.** `125v146` s48, at **3 life**, the last real decision of the game:
`Cast Lightmine Field {2}{w}{w} {right now: they control 3 creatures able to attack - deals 3 to
each if all 3 attack}` over a board of two 2/2s and a 4/4. The only question at 3 life is whether
the row stops lethal; it does not (Nadaar survives and connects for 4), and the row read as a
three-for-one (`wave60/deck125/review.md` HIGH-3).

**Mechanism.** `boardCreatureCounts` optionally fills a `WipeVictim` roster for the creatures it
already counts as able to attack — same walk, same `boardCreatureCanAttackNow` predicate, so the
roster cannot disagree with the count printed beside it. `wipeSurvivorTail` splits it on the one
fact that decides the row (toughness against the damage) and totals the side that survives:

```
they control 3 creatures able to attack - deals 3 to each if all 3 attack.
DIES at 3: Goblin (2/2); Triumphant Adventurer (2/2).
SURVIVES: Nadaar, Selfless Paladin (4/4) - 4 power of theirs survives it
```

Entries are separated by `; ` — a card name can contain a comma, and a separator the payload can
contain is not a separator (wave-60 engine-seat MED-3). An INDESTRUCTIBLE creature is not
destroyed by damage at any amount, so it is listed as a survivor with its keyword whatever its
toughness (the rail #W60-Q R6 put on the destroy roster). With no roster to print, the clause is
the wave-60 line byte for byte.

## C13 — which phase a transport failure died in, and the deadline every record spent

**The adjudication the ledger asked for.** Both wave-60 transport records
(`...deck130` s27, `...deck162` s16) read `curl=28,http=0,empty=1` at `latency_ms` 900,024 /
900,027 against the 900,000 ms deadline. CURLE_OPERATION_TIMEDOUT (28) is returned for BOTH the
connect timeout and the whole-request timeout, and **only the elapsed time tells them apart**: at
100% of the deadline these are WALL misses — correctly classed `timeout` / `wall_miss`, not
`transport_error`. The connect-phase arm is not broken; it did not fire because neither record
was a connect failure. Wave-60 lane M's B4 bound (connect <= 20 s and <= half the deadline) is
what makes a connect death land far short of the wall, and the classification chain behind it is
intact: `noAnswerClassFor(..., timedOut=false, curl=28)` → `transport_error`,
`retryableTransportFailure(28,0,true)` → true, `remainingTransportRetryMs(900000, 20013)` →
879,987 ms of budget for the one retry. All four are pinned as PARSETEST cases with those exact
numbers.

**What shipped, so the next corpus does not need that adjudication by hand:**

1. `transportPhaseFor(curl, latency, connectBudget, deadline)` — a pure verdict:
   `wall` (>= 95% of the deadline), `connect` (at or just past the connect bound), `midflight`,
   and `""` whenever the facts do not settle it. A phase is never guessed.
2. The stamp carries the two facts that make it provable:
   `curl=28,http=0,empty=1,connect_ms=20000,phase=wall`. Appended ONLY on a curl failure, so
   every other stamp — and every stamp written before this wave — is byte-identical.
3. **`deadline_pct` is restored on every record that spent a round trip.** It was written only
   inside the `long_reply` branch (answered replies past 95%), and the wave-60 corpus contained
   no such reply, so the field was absent from the entire corpus and B4's prediction was
   unmeasurable. `long_reply` keeps its own stamp; the fraction is now independent of it.
   `-1` (cache/reuse, or no deadline) still writes nothing.
4. The abandoned-wall-miss record carries the latency that missed the wall (`mWallMissLatencyMs`,
   stashed when the account is armed, consumed with the record). It previously read
   `latency_ms: -1` — the value that means "cache hit" — for a decision that had just spent the
   whole deadline, and carried no `deadline_pct` at all.

## C14a — the HOLD survives the turn boundary while the rows are byte-identical

**The finding.** `146v125` seqs 92→101: ten consecutive asks, 20.4-21.6 KB each (~210 KB of
inference), across Blockers → Main 2 → Upkeep → Draw → Main 1 → Main 1, on a menu whose only two
cast rows were annotated `at 0 this does nothing` and `the only legal targets are YOUR OWN right
now`. 22.7% of deck146's decisions were pure declines. Nothing about that menu changed; the TURN
did, and the turn boundary retired a hold the model had already taken over the identical rows
(`wave60/deck146/review.md` HIGH).

**Mechanism.** `holdHonoured`'s turn gate is gone. The latch is now retired by the only thing that
ever made it unsafe — a row the model has not seen — which `holdStillStands` already re-checks
byte for byte at every window, so a single moved price re-opens the question. This is not a blind
cache: nothing is remembered except the rendered rows, and nothing is skipped that differs from
them by one byte. `takeHold` now **replaces** the seam's row set instead of unioning into it:
with the turn no longer clearing it, a union would let a later hold widen the set the first was
taken over, which is exactly the blind cache this latch must not become.

Because the engine's rule changed, the ROW's promise changed with it (the surface owes the model
truth):

```
Hold priority: pass now, and do not ask me again - this turn or later - until one of the rows
above changes (any change re-opens this window; you give up no cast)
 {taking this row skips every later window whose rows are identical to these}
```

The retired wave-55..60 spelling is kept in `isReservedHoldEcho`, so a model that echoes the text
it learned still binds to this row rather than to row 0.

## C14b — the re-open rule, stated truthfully PER MENU

**The finding.** `152v162` seqs 32-42: eleven consecutive priority windows inside one Draw phase,
**163.3 s of inference for zero board effect** while the seat's life ticked 15 → 8 and every row's
price ticked with it. HOLD was offered every time and never taken — because on that menu "any
change re-opens this window" is not a guarantee, it is a COST: the hold is retired by the next
window and buys nothing (`wave60/deck152/review.md` I2).

**Mechanism.** `holdReopenNote(seam, rows)` remembers the previous window's rendered rows per seam
and measures the next window with **the latch's own predicate** (every row printed now was printed
then), then states which of the two regimes this menu is in:

```
[hold check: every row above was also on the menu at the last window at this seam (3 windows in a
row now) - a hold taken here holds until one of them changes]
[hold check: 1 row above is new since the last window at this seam - a row moving is what re-opens
a hold, so a hold taken here lasts only until one moves again]
```

It is **prompt-only**, in the declined count's own channel and for the declined count's own reason:
a number that moves with every answer must never enter the ask key or it mints a fresh question
every window and turns the cache into a call per tick. It is measured at both seams over the rows
the latch reads, and BEFORE the latch consumes the window, so a held window still updates the
memory. The first window at a seam measures nothing and claims nothing.

## C14c — the record stops carrying the overrun

**The finding.** 81 of 119 replies at deck162's seat overran the protocol; **46,052 characters** of
un-committed self-argument across six games, the largest single tail 11,302 bytes
(`wave60/deck162/review.md` MED, `deck130` LOW).

**Mechanism.** `recordReplyTrimmed` keeps the reply in full through the end of its PLAN line —
everything the protocol asked for and everything a seat review reads — plus a bounded 400-byte
head of the overrun so the SHAPE of the spiral is still auditable, then states the cut:
`[+N bytes written past the PLAN line trimmed from this record]`. Below a 1,200-byte overrun
nothing is touched, so a compliant reply's record is byte-identical. **The parser is unchanged**
(it has always stopped at the answer and the plan), and `post_plan_overrun` /
`post_answer_overrun` are still measured on the FULL reply before the trim runs — the corpus can
still say exactly how big the overrun was, and `reply_trimmed_bytes` says how much of it the
record dropped.

---

## Gate

Detached unit `w61-U-gate`, 6G cap (build) / 4G (runs), `rm -f bin/wagic` full relink.

| | base (`718e99d2a`, brief) | this lane |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1248 tests, 0 failed, 65 AI / 0 failed | **1248 tests, 0 failed, 65 AI / 0 failed** |
| `==Test Failed !==` / `==Test timed out` | 0 / 0 | **0 / 0** |
| PARSETEST | 3699 / 0 failed | **3747 / 0 failed** (+48) |

`git diff | /usr/bin/grep -c $'\357\277\275'` = 0.

## RED evidence per item

* **C14a** is the only item with a base test that CONTRADICTS it, and it produced the red result
  directly: five wave-55/57 PARSETEST cases pinning the turn-scoped hold row and its benefit
  clause went **FAIL on the first build of this lane** (`#W55-A D11b the hold row renders the
  reworded literal`, `#W53-N D2 the row states its own re-opener`, `#W55-A D11b the reworded row's
  disambiguating prefix`, `#W55-A D21 the benefit clause's literal`, `#W57-A D4 the rendered HOLD
  row carries its benefit tail`) — measured, 3694 passed / 5 failed, before the expectations were
  rewritten to the strings the engine now keeps. That is a base expectation protecting a retired
  promise, made concrete.
* **C10a, C10b, C13** add new functions and a new overload; their cases cannot compile against
  base at all (`boardSweepMarker`, `applyBoardSweepMark`, `wipeSurvivorTail`, the two-argument
  `attackPunisherClause`, `transportPhaseFor`, the six-argument `transportOutcomeStamp`). Their
  old behaviour was a silent omission (a count with no roster, a menu with no ranking, a stamp
  with no phase), so there is no base string to contradict; the new cases pin the corrected
  shape, and each ships an explicit NEGATIVE that the pre-change rendering is byte-identical
  wherever the new fact is absent.
* **C14b, C14c** are likewise additive functions with no base counterpart.
* **No suite fixture is possible for any of it.** The suite has no network seat and no model
  reply; `askModel` returns at `mEndpoint.empty()` before every code path this lane touches.
  This is the same split lane H took for K1 and lanes M/Q took for B4/R9, and it is stated here
  rather than claimed around.

## Predictions (falsifiable, next corpus)

1. **C10a**: every cast menu that offers a sweeper removing >= 1 of THEIRS carries exactly one
   `[<- board sweep: THEIRS n / YOURS m ...]` marker, on the row with the largest THEIRS.
   Falsifier: a window with two such markers, a marker on a row whose priced sweep is smaller
   than another marked row's, or a `{right now: destroys N of their creatures}` row with N >= 1
   and no marker anywhere on the menu. Outcome prediction, weaker and worth measuring: deck130's
   Starstorm cast rate rises above 0/51 in windows whose marker names >= 2 of THEIRS.
2. **C10b**: zero `deals N to each` clauses print a bare count — every one with a non-empty
   attacker roster carries `DIES at N:` and `SURVIVES:`, and no survivor's toughness is <= N
   unless it is marked `[indestructible]`.
3. **C13**: every record carrying a `transport` stamp with `curl=` > 0 also carries
   `connect_ms=` and `phase=`; `deadline_pct` appears on every record whose `latency_ms` >= 0;
   and no record classed `transport_error` carries `phase=wall`. Falsifier for the adjudication
   itself: a `phase=connect` record classed `wall_miss_unrecorded`, or a `phase=wall` record
   classed `transport_error`.
4. **C14a**: `hold_windows_skipped` rises, and at least one hold spans a turn boundary — i.e. a
   run of held windows whose stderr `holding priority at the ... seam` lines cross a turn header
   with no intervening `hold re-opened`. Falsifier: a held window whose rendered rows differ in
   any byte from the rows the hold was taken over (the latch would be a blind cache).
5. **C14b**: every ask after the first at a seam carries exactly one `[hold check: ...]` line,
   and the `every row above was also on the menu` form appears on precisely the windows whose
   rows are unchanged. Falsifier: a `hold check` line claiming an unchanged menu on a window
   whose own `[DRAW PRICE ...]` life figure differs from the previous window's.
6. **C14c**: no translog `reply` exceeds its committed head by more than ~480 bytes without a
   `reply_trimmed_bytes` field beside it, and the summed `post_plan_overrun` is unchanged in
   magnitude from wave 60 (the measurement is on the full reply; only the stored copy shrinks).

## What I did NOT verify

* **No live model probe and no corpus run.** Every render change, the latch and the record change
  are PARSETEST-verified only. The `transport` phase stamp has never been written by a real curl
  failure, and `deadline_pct`'s new emission sites have never been seen in a real translog.
* **No suite fixture exists for any item** (reason above). The suite result below is a
  no-regression check, not evidence any of this works.
* **C10a's `mine` for an X sweeper** is the count at the marked X only. A pilot who then announces
  a DIFFERENT X gets a marker that priced the X the row named — which is what the row already
  said, but it is not a claim about every X on the ladder.
* **C10b models damage as toughness vs amount.** Damage already marked on a creature this turn,
  damage prevention/protection shields, and toughness-changing effects that resolve between the
  render and the wipe are not modelled — the same limitation the destroy roster carries. A
  creature already damaged dies to LESS than the printed amount and is currently listed as a
  survivor.
* **The `4 power of theirs survives it` total** is scoped to the clause's own "if all N attack"
  hypothetical and does not net blockers, evasion or the crack-back's own arithmetic (C3 is
  lane R's).
* **C14a's unbounded hold is unbounded by design and unobserved in a real game.** I did not
  construct a game in which a hold spans several turns, and I did not measure how often a real
  menu is byte-identical across a turn boundary — the deck146 run is the evidence that it
  happens, not a measurement of how often.
* **The concurrency of this gate.** Two sibling lanes ran their own suites against the same
  `~/.Wagic` while this one ran (the wave-60 harness item about concurrent suite runs). The
  single-threaded result matched base exactly, so nothing was masked here that I can see, but I
  did not re-run in isolation to prove it.
* **I did not read the deck146/deck152/deck162 seat translogs myself** beyond the quoted seqs in
  the wave-60 reviews; every corpus figure above is quoted from those reviews and from
  `wave60/engine-seat.md`, not re-counted.
