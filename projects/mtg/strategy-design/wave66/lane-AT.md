# Wave-66 lane AT — H5, H6 + the deck130 / deck146 / deck162 / engine-MED-3 items

Base: master `bdd0ec6e5` (wave-66 step-one brief). Branch `w66-lane-AT`, worktree
`worktrees/lanes/w66-AT`. **One file changed: `src/AIPlayerGPT.cpp`** (+675/-10). Every wagic run
memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); **no kill under the cap on any run**. The gate ran
detached as `w66-AT-gate2` with a foreground until-loop wait. `git diff | /usr/bin/grep -c
$'\357\277\275'` = **0** after every edit (all splices python `rb`/`wb`). Every comment tagged
`#W66-AT (<item>)`.

Every item is a RENDER / ORDER / RECORD change: none of them changes what the engine does with a
card, an option or an answer, so no suite fixture can be RED on base and none is claimed. RED was
produced the way lanes AG/AH/AK/AL produced it — each mechanism reverted to its wave-65 behaviour
with the new cases already in place, binary rebuilt. See **RED-on-base evidence**.

---

## What shipped

### H5 — the discard ask stops ordering against the prices it prints (deck125 HIGH-2)

`125v126` seq 55: row 1 was `Supreme Verdict {right now: destroys 7 of their creatures ...}`, rows
5/6/8 were lands carrying `{spare: you control 10 lands already ...}`, and the seat answered
`PUT: 1`. Corpus-wide: 31 discard asks, 12 offered at least one spare row, 6 discarded spares only.

Two causes, both in `buildCleanupDiscardAskText`, both fixed by ADDING:

* **Hand order was the row order.** `discardDisposabilityClass(isLand, row)` reads the row's OWN
  printed verdicts — spare land (0), a card the engine priced `{dead right now:` (1), a row it
  could not price (2), a card whose `{right now:` verdict says it still does something (3) — i.e.
  the cast-row value ranking INVERTED, which is what a discard wants. `discardDisposabilityOrder`
  is a stable bucket sort over that vector, so ties keep hand order.
* **The polarity was one-sided.** `{spare:}` marked the disposable rows and nothing marked the
  rest, so a bare row read as neutral. `discardNotSpareTag` marks every non-spare row
  `{NOT spare: a real card, not a surplus land}` (only when the list actually holds a spare row —
  a hand with nothing spare renders byte-identically to wave 65), and
  `discardSpareDefaultLine(spareRows, over)` leads the ask with
  `DEFAULT ANSWER: discard the spare land.` plus the count and the ordering rule. The header is now
  composed first and EMITTED after the row scan (nothing else writes to `tail` in between), because
  the sentence names a count the rows have to exist to produce.

**The indices stay consistent through the answer parser.** The emitter already carried a
permutation rail (`outOrder`, filled by the copy-collapse `groupNumberedRows`, un-permuted at
`cleanupDiscard` by `unpermuteSelection`). The disposability order is COMPOSED onto it —
`composeRowOrder(dispOrder, groupOrder, discardOrder)`, printed position k -> hand position
`dispOrder[groupOrder[k]]` — rather than renumbering anything by hand. Both halves are pure
functions so the whole pipeline is provable without a game, and the PARSETEST case runs the
**corpus ask** (all eight `125v126` s55 rows, verbatim verdict clauses): it asserts the classes, the
permutation is a permutation (every hand position exactly once), the composed order, and that
`PUT: 1` now lands on hand position 4 — Seachrome Coast, the first spare land — instead of the
sweeper.

`{NOT spare: ` joins `stripNarrationDecoration`'s drop list (it rides a rendered row the model may
echo). Every input is a board/hand fact of THIS window, so nothing that can differ between two
rebuilds of the same window enters the ask text, and the discard seam has no hold latch or slot key.

### H6 — the summoning-sick tag reaches the row that decides (deck152 HIGH-1 / MED-1)

`summoningSickTag()` had exactly one caller, the battlefield serializer. `152v162` seq 35-37 put all
three Luminarch Aspirant counters on a Briarbridge Tracker cast that turn — the board line said
`[summoning sick - cannot attack this turn, but CAN block]`, the TARGET ROW said `(4/3) [vigilance]
[your battlefield]`, and each reply's plan read "Attack with ... Briarbridge Tracker". Only deck152
game in the corpus, and it was lost. Two halves:

* **`describeTarget`** now emits the SAME tag on the SAME predicate, under the two scopes wave 34
  settled, lifted into a pure predicate `summoningSickRowApplies(sick, inPlay,
  controllerIsActivePlayer)` so the gate itself is pinned: the permanent must be in play (a hand or
  graveyard target has no attack this turn to restrict) and its controller must be the ACTIVE player
  (N-166f: on the defender's board the clause is trivially true and reads as a restriction on the
  coming combat that it is not).
* **The attackers window** names what its candidate walk left out. `buildDeclareAttackers` is
  `canAttack()`-gated, so a summoning-sick body is simply ABSENT — the silent-omission shape the
  trust doctrine's third rule is about, and exactly what the deck152 plan confabulated into.
  `attackersHeldSickLine` prints `NOT offered above and NOT able to attack this turn (summoning
  sick: entered this turn without haste) - <names>. They attack from your NEXT turn on; a plan that
  names one of them as an attacker THIS turn cannot be executed.` No row is created (they are still
  not choosable) and none is removed. Membership is tested against the PERMUTED candidate vector, so
  a creature that IS offered can never appear here. Restriction-first per N-139k: the only attack
  licence in the line is NEXT turn, pinned by a MUST-NOT-MATCH.

### deck130 MED (a) — the per-row auto-tap verdict, negative half

The board line says `CAUTION - some usable mana sources DAMAGE YOU when tapped for mana (auto-tap
when you cast may spend them)`; `paymentLifeCostClause` prints only when the plan DOES spend one, so
a row whose plan spends none said nothing, and "may" plus silence is not a verdict. `130v126` seq 32:
two Talisman of Impulse on a board carrying BOTH halves of the opponent's life LOOP (so any nonzero
payment was fatal), six castable cards, every row's `selectAutoTapProducers` plan taking Mountains
only — and the seat answered "Cast nothing right now", plan "no safe play exists", and held for the
rest of the game. `paymentNoLifeCostClause` states the plan's answer when the answer is "none of the
ones that hurt you", off the SAME picks the row already gathered. Gated three ways: only when the
board actually has a damaging source, only when this row's plan total is 0, only when the row taps
something.

### deck130 MED (b) — the cast-mode menu carries the cast row's kill arithmetic

`130v152` seq 77 offered `Cast Spark Spray {r} {kills 0 of the 1 CREATURE target at 1 damage - and 1
to the opponent at life 25 leaves them at 24}`; the seat still planned "Kill Brutal Cathar with Spark
Spray", and one window later the mode menu for the same card read `1. Cast Card Normally [cost: {r}]
{card text: ...}` with no verdict at all. `castKillVerdictNow(observer, this, ctx)` RECOMPUTES the
verdict from the board of THIS window — never carried across from the earlier render, which could be
stale — off the same four predicates (`spellSingleDamageAmount` / `spellPTDropAmount` /
`damageKillsTarget` / `ptDropKillsTarget`) and the same two emitters (`castKillSummaryTag`,
`castPlayerDamageTail`) the cast row uses, so the two seams can differ only where the board moved.
The enumeration is deliberately narrower than the cast row's five-zone walk: that walk is wide
because it also PRINTS the target list, and the kill test it feeds already discards everything that
is not a battlefield creature. `{kills ` / `{kills: ` are already on the narration drop list.

### deck146 MED — the land-drop row says the land is also a body

`146v125` seq 84/85 declined Hive of the Eye Tyrant twice in one turn, at 2 life, on a creatureless
board; both rows priced mana and entry-tapped-ness only. `landDropThreatTag` runs the detector the
crack-back screen already runs on the opponent's board (`crackBackBestAnimateRung`, #W63-AC E5) at
RISING reach and stops at the first rung that arms — the same affordability pricer, asked "from how
little" — and prints `{also a THREAT: this land ANIMATES - it has an activated ability that turns it
into a creature, cheapest rung {3}{b} for 3 power. Playing it deploys a body without spending a cast;
its own mana is not available to that cost (a land tapped for mana is tapped)}`. The script is
matched LOWERCASED, exactly as the crack-back walk does it — the primitives write
`becomes(Creature Beholder^3/3^black^menace)` with a capital C, and a case-sensitive find would have
answered "not a manland" for the very card this item is about (caught by the PARSETEST case, which
uses `borderline.txt`'s line verbatim). `{also a THREAT: ` joins the drop list.

### deck162 MED — the sorcery-speed reservation

`#W52-L`'s strand clause names what a cast strands at INSTANT speed. Its mirror was missing: an
instant-speed row offered in the seat's OWN upkeep or draw step can wait (its last window this turn
is the end of the opponent's turn), while a sorcery-speed card in hand has exactly one window left —
this turn's main phase — and the mana this row spends comes out of it first. `162v123` seqs 11/14
cast Dictate of Kruphix (Flash) in the seat's own draw step with Ob Nixilis, the Hate-Twisted
stranded in hand. `sorceryReserveClause` prints `{reserve: this row is INSTANT SPEED - it still has
a window at the end of THEIR turn. Taking it HERE, before your main phase, leaves N sources, and
<card> <cost> in your hand needs M - it is SORCERY SPEED, so your main phase this turn is its last
window}`, gated on `currentPlayer == this && phase < MTG_PHASE_FIRSTMAIN` and on the row's own card
being instant-speed. It names the most EXPENSIVE stranded sorcery-speed card (the one the remainder
is furthest from). `{reserve: ` joins the drop list.

### engine MED-3 — reveal stalls: MEASURED, and the instrument that mis-read them fixed

**The measurement (this is the item's deliverable; no engine change was warranted).** All 16 reveal
records in `matchups-20260906-033121` carry `reveal_wait_secs` equal to the SAME record's
`latency_ms / 1000`:

| game | seq | reveal_wait_secs | latency_ms | driver_secs |
|---|---|---|---|---|
| 123v146 | 14 | 139 | 139015 | 0 |
| 126v123 | 9 | 107 | 106146 | 0 |
| 152v162 | 23 | 68 | 68246 | 0 |
| 146v152 | 12 | 68 | 67848 | 0 |
| 152v125 | 43 | 54 | 54747 | 0 |
| … (11 more, same shape) | | | | |
| **total** | 16 | **835** | **~835 000** | **0** |

So the 835 s is the seat's own round trips, end to end, and there is **no engine wait to skip** —
the item's fix condition is not met. Cross-check: the reveal window's median latency (52.5 s) sits
inside the corpus's own per-kind spread (blockers 49.7 s, attackers 35.8 s, priority 21.6 s, ask
20.0 s), and 16 decisions is 0.7% of 2339, so reveal is not an anomalous kind — it is a small number
of ordinary round trips.

**Why the wave-65 seat read it as engine-side.** `reveal_wait_driver_secs` is 0 on all 16, and that
was read as "the wait is engine-side, not model-side". It cannot mean that, BY CONSTRUCTION: the
driver-half counter's progress signature treats a model poll as progress, so it resets on every tick
while a call is in flight and can never accumulate during a round trip. A missing metric and an
absent bug produced identical output. The record now carries the subtraction a reviewer otherwise
has to do by hand: `reveal_wait_unexplained_secs` = `reveal_wait_secs - latency_ms/1000`, floored at
0, with a cache hit (`latency_ms` -1) explaining nothing so the whole wait is the residual. It is
the only figure on the record a real stall would move, and **0 means there is nothing here to fix**.

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), qmake untouched, no new sources.
Detached unit `w66-AT-gate2`, `MemoryMax=4G` / `MemorySwapMax=0`, foreground until-loop wait.
Log `~/.gatelogs/w66-AT-gate2.log`.

| leg | result |
|---|---|
| build | 0 errors, clean link |
| PARSETEST | **4739 passed, 0 failed** (base 4698/0 — **+41 cases**) |
| suite, `WAGIC_TESTSUITE_THREADS=1` (ground truth) | **1271 tests (0 failed), 67 AI tests (0 failed)** |
| `==Test Failed !==` / `==Test timed out` over the T1 leg | **0** / **0** |
| suite, default threads | **1271 tests (2 failed), 67 AI tests (0 failed)**, 0 timeouts |
| the default-threads failures | `lifeline.txt`, `merrow_reejerey.txt` — the brief's known concurrency-only pair |

**0 new failures.** `intrepid_adversary_repeated_payment.txt` (the brief's genuine intermittent)
passed in both legs of gate2; its rate was not exercised here. (It DID fail once, in the discarded
gate1 run — see the note below — which is the brief's own description of it.)

⚠ Process note, recorded because it nearly produced a false green: gate1 ran the RED binary. The
restore-and-rebuild command was chained with `&&` through a `grep -c` that printed `0` and therefore
EXITED 1, so `rm -f bin/wagic && make` never ran and the gate exercised the previous build. It was
caught because the gate log's PARSETEST leg read `4723 passed, 16 failed` — the RED number — not
because anything about the suite legs looked wrong. Freshness-gate the binary; never `&&` a build
behind a counting grep.

### RED-on-base evidence — measured, not asserted

The nine mechanisms were reverted to their wave-65 behaviour with the new cases in place and the
binary rebuilt: `discardDisposabilityClass` returning 2 for every row (base = hand order),
`discardSpareDefaultLine` / `discardNotSpareTag` / `attackersHeldSickLine` /
`paymentNoLifeCostClause` / `sorceryReserveClause` / `landDropThreatTag` returning "",
`summoningSickRowApplies` returning false (base had no target-row tag),
`revealWaitUnexplainedSecs` returning 0 (base had no such field).

Result: **`4723 passed, 16 failed`** (log `~/.gatelogs/w66-AT-red.log`) — **H5 ×7, H6 ×3, deck130 ×1,
deck162 ×2, deck146 ×1, MED-3 ×2, and nothing else**: 4723 + 16 = 4739, so **no pre-existing case
goes RED under the reversion** and none was deleted or re-pinned. The other 25 new cases are
MUST-NOT-MATCH, NEGATIVE, CONTROL and echo-shape pins that pass on base too, which is what they are
for.

---

## Predictions (falsifiable, for the wave-66 corpus)

1. **H5 (order).** On every cleanup-discard ask whose list holds at least one `{spare:` row, row 1
   is a `{spare:` row and no `{right now:`-priced row precedes any `{spare:` row; every non-spare row
   carries `{NOT spare:` and no spare row does; the ask's first line after the CR 514.1 header is
   `DEFAULT ANSWER: discard the spare land.` and its count equals the number of `{spare:` rows below
   it. **Falsifier**: one of any. Outcome prediction, weaker: the spares-only discard rate over the
   12-of-31 window class rises above wave 65's 6 of 12.
2. **H5 (indices).** 0 discard records whose `chosen_text` names a card that is not at the hand
   position the answered number maps to — concretely, every `discard` record's `chosen_text` appears
   in the printed row it numbered. **Falsifier**: one mismatch (that is the renumbering bug this
   composition exists to prevent, and it would show as a discarded card the reply never named).
3. **H6 (target rows).** Every target-choice row naming a battlefield creature that is summoning
   sick AND controlled by the active player carries `[summoning sick - cannot attack this turn`;
   **0** such rows render bare (wave 65: all of them did). **0** such tags appear on a row whose
   target is in a hand, graveyard, library or exile, or on the non-active player's board.
   **Falsifier**: one of either. Outcome: the `152v162` s35-37 shape — three counters onto a
   summoning-sick body under a plan that names it as an attacker — does not recur.
4. **H6 (attackers window).** Every attackers prompt issued on a turn where the seat controls a
   summoning-sick creature carries `NOT offered above and NOT able to attack this turn`, naming
   exactly the creatures the A-rows do not; **0** names appear in both the A-rows and that line.
   **Falsifier**: either. Weaker outcome: the count of PLANs naming an attacker absent from the
   following attackers prompt falls.
5. **deck130 (a).** On every cast row rendered on a board whose situation block carries the
   `CAUTION - some usable mana sources DAMAGE YOU` line, exactly one of
   `{paying this costs you N life` or `{paying this costs you NO life` appears (never both, never
   neither) whenever the row taps at least one source. **Falsifier**: a bare priced row on such a
   board. Outcome: deck130's "no safe play exists" pass streak on a Talisman board does not recur
   with castable rows whose plans spend no Talisman.
6. **deck130 (b).** Every `Cast Card Normally` row on a cast-mode menu for a card the cast menu
   would have priced (a single-`damage:` spell or a flat P/T drop) carries a `{kills` or
   `{no creature target` clause; the figures match what the cast row for the same card printed in
   the same turn on an unchanged board. **Falsifier**: a bare `Cast Card Normally` row for such a
   card, or two seams disagreeing on a board that did not move between them.
7. **deck146.** Every `Play <land>` row for a land whose script holds an activated
   `becomes(creature ...)` rung carries `{also a THREAT:`; **0** rows for a land with no such rung
   carry it. On Hive of the Eye Tyrant specifically the tag reads `cheapest rung {3}{b} for 3 power`.
   **Falsifier**: one of either. Outcome: the 146 seat's manland land-drop decline rate falls below
   wave 65's 2 of 2.
8. **deck162.** `{reserve:` appears only on rows for instant-speed cards, only on the seat's own
   turn, and only at a phase before its first main; on every such row the named card is
   sorcery-speed and its `needs M` exceeds the row's own `leaves N`. **Falsifier**: one violation of
   any of the four.
9. **MED-3.** Every `reveal` record carrying `reveal_wait_ticks` also carries
   `reveal_wait_unexplained_secs`, and its value equals `max(0, reveal_wait_secs - latency_ms/1000)`
   on every record. The corpus-wide SUM of `reveal_wait_unexplained_secs` is **under 20 s** (wave 65:
   the equivalent residual was ~0 and the docketed figure was 835 s). **Falsifier**: a sum above
   that — which would be the first evidence of a real engine-side reveal wait and would re-open the
   item with a number attached.

---

## What I did NOT verify

- **No live model probe was run and no game was driven.** PARSETEST, the suite and source reading
  are the whole gate; nothing here has been seen by a model, so every behavioural prediction is a
  prediction, not a measurement.
- **H5's emitter is proved through its pure parts, not through `buildCleanupDiscardAskText`.** That
  function needs a live hand and an endpoint, so the corpus case runs
  `discardDisposabilityClass` -> `discardDisposabilityOrder` -> `groupNumberedRows` ->
  `composeRowOrder` -> `unpermuteSelection` — the exact call chain the emitter makes, on the exact
  rows the corpus printed — but the wiring itself (that the emitter passes those rows to those
  calls in that order) is argued from the single edit site. I did NOT observe a rendered discard ask.
- **H5's classification reads the row STRING, not the emitter's state.** `discardDisposabilityClass`
  keys on ` {spare: `, ` {dead right now: ` and ` {right now: `. If a future emitter changes any of
  those three literals the classifier silently falls back to class 2 (hand order) rather than
  mis-ordering — safe, but silent. There is no assertion tying the literals to their producers.
- **H5 does not price the non-land rows against each other beyond their verdicts.** "The cast-row
  ranking inverted" is implemented as dead-before-unpriced-before-live; it does not rank two live
  rows against each other (Supreme Verdict vs Final Judgment stay in hand order). Doing that would
  need a cross-row value model the discard seam does not have.
- **H6's target-row emission is source-verified at the call site.** The GATE is pinned as a pure
  predicate and the TAG is pinned byte for byte, but `describeTarget` takes a live `Targetable`, so
  no pure case proves the two are wired together; I read the single edit.
- **H6's attackers line is source-verified for its membership walk.** The line is pinned pure; that
  `game->inPlay` minus the permuted `attackers` vector is the right set is read, not measured. In
  particular I did not construct a board where a creature with haste-granted-then-removed status
  could appear in both.
- **deck130 (b) `castKillVerdictNow` has no pure case.** It builds a `TargetChooser` from a live
  observer. Its four arithmetic predicates and both emitters are already pinned by wave 53/54/55
  cases; the new walk over `inPlay` is read, not measured. I also did NOT check what it renders for
  a card whose target chooser is stack-scoped — `castKillSummaryTag` returns "" at 0 creature
  targets with no player tail, which is the intended silence, but no case exercises that path
  through this function.
- **deck130 (a) is the CAST row only.** The ability-row emitter (~`:26944`) has the identical
  silence and was left alone: the corpus evidence is a cast menu, and widening was out of scope.
- **`landDropThreatTag`'s reach loop tops out at 20.** A rung costing more than 20 generic arms
  nothing and the row stays silent. I did not enumerate the pool for such a card.
- **deck162's clause does not model the seat's own upcoming land drop.** `untappedSources` is this
  window's count; a land drop in the coming main phase could pay for the stranded card after all,
  and the clause does not say so. It is a statement about the mana this window has, which is what
  the `#W52-L` clause beside it is too.
- **MED-3's residual is derived from `mLastLatencyMs`, which is the LAST round trip on this seat,
  not provably the one this reveal waited on.** On every corpus record the two coincide (the reveal
  record is written at the reveal's own consume), but a reveal answered from cache after an
  unrelated call would attribute the wrong trip. The floor at 0 makes the failure direction
  conservative (it under-reports a stall, never invents one) — a residual that grows is still real.
- **No cross-lane interaction was tested.** Lane AQ owns `modeRowAnnotations` on the CHOOSE_MENU
  seam (H9) and lane AS owns the hold key and the `{reserve}`-adjacent decline rows (H7); the
  combined diff has not been built here. Three merge-time looks are worth it:
  `stripNarrationDecoration`'s drop list (three entries added at the same chain AL widened), the
  cast-row annotation block (`paymentNoLifeCostClause` and the sorcery reserve are appended inside
  the same `if (payCost ...)` region other lanes annotate), and the `Cast Card Normally` row, which
  AQ's mode work may also touch.
