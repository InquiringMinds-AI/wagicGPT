# Wave-61 lane V — the eight findings of `wave61/codex-review.md`

Branch `w61-lane-V`, base `9f18ad794` (master after the R–U merge + the review commit).
Four files changed: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/src/AllAbilities.cpp`,
`projects/mtg/include/Player.h`, `projects/mtg/include/AIPlayerGPT.h`.

Every finding was verified against the code before anything was changed. Seven are CONFIRMED and
fixed in the general form; one is REFUTED in the half the review names and CONFIRMED in a second
half it also names, and that half is fixed. Nothing outside the eight findings was touched.

---

## CONFIRMED / REFUTED

| # | Verdict | Evidence |
|---|---------|----------|
| 1 (HIGH) | **CONFIRMED** | `AllAbilities.cpp:1182` set `singlePickOptional = true` whenever `oneTc->maxtargets == 1 && !oneTc->targetMin`. `TargetChooser.cpp:522` sets `targetMin` only for a `<N>` prefix carrying no `upto:`, so a BARE `target(*[manacost>=3]|reveal)` (Pelakka Predation, `borderline.txt:82651`) and an `<upto:1>` chooser are indistinguishable there — and the ask then printed `or exactly "PUT: none" to choose no card` over a choice whose current Oracle reads "You choose a card from it with mana value 3 or greater". PARSETEST at the old `AIPlayerGPT.cpp:54728` asserted that decline text as correct. |
| 2 | **CONFIRMED** | `holdStillStands` (`AIPlayerGPT.cpp:19812`) tested only `nowRows ⊆ heldRows`, so `{B}` was accepted as unchanged after `{A,B}` lost A. The wave-53 PARSETEST at `48334` pinned exactly that (`a row DISAPPEARING is not a re-opener`), while the #W61-U C14 row printed to the pilot promises "a hold taken here holds until one of them changes". |
| 3 | **CONFIRMED** | `landEntersTappedTag` stopped at the FIRST line containing `tap(noevent)` and called it conditional only on `aslongas`/`if(`/`restriction`. Bant Panorama (`mtg.txt:8934-8936`) carries `tap(noevent)` only inside `{1}{T}{S}:` fetch abilities, for the FETCHED basic — it rendered "enters TAPPED - it makes no mana this turn". Blood Crypt (`mtg.txt:12034`) carries it as one `choice` branch of the pay-2-life choice — it rendered as unconditional. |
| 4 | **REFUTED (class) / CONFIRMED (retry budget)** | The class table is already ordered correctly: `noAnswerClassFor(bool,bool,bool,long httpStatus)` returns `http_error` for any `httpStatus != 0 && != 200` BEFORE consulting `timedOut` (`AIPlayerGPT.cpp:11912-11917`), so a late 503 was never classed `timeout`. What IS wrong is what the review's second sentence names: the worker set `timedOut` from `body.empty() && elapsed >= 95%` alone, and `pollCompletionRetry`'s deadline arm is gated on `mLastTimeout`, not on the class — so the 503 opened a `wall_miss` account and was granted `mRetryBudgetMs = 0` (a FULL fresh deadline) instead of the bounded transport retry. Fixed there. |
| 5 | **CONFIRMED** | `wipeSurvivorTail` (`AIPlayerGPT.cpp:1719`) computed `killed = !indestructible && damage >= toughness`, and the collection site filled only `indestructible`. A creature with protection from the damage's source is dealt no damage at all, so `DIES` was false and its power was dropped from `survivingPower`. |
| 6 | **CONFIRMED** | `crackBackFloorSources` (`AIPlayerGPT.cpp:17574`) emitted "they can also aim ability damage at you that no block stops" for any non-triggered line matching `damage:N` + a player-ish target, with no test of tapped state, untappability or cost. |
| 7 | **CONFIRMED** | `oneShotDrawGrantScan` ended `if ((…) && out.size() < 4) out.push_back(b);` — a hard cap on legal modal branches. |
| 8 | **CONFIRMED** | The `blockGain` loop counted every creature for which parameterless `canBlock()` is true. That is the SOLO gate (untapped / no can't-block); the pairwise half the per-attacker rows use is `couldBlockIfItAttacked(attacker)`, and it was not asked. |

---

## What shipped

**R1 — a mandatory choose-one is never offered a decline.** `revealSinglePickDeclineLegal(spec)`
(pure, `AIPlayerGPT.cpp`) reads the engine's own arity grammar the way
`TargetChooser::createTargetChooser` reads it: a `<...>` prefix sets a MINIMUM equal to its amount
unless it carries `upto:` (or `anyamount`), and no `<...>` at all is a plain one-target chooser —
a minimum of one. `AllAbilities.cpp` now passes two independent flags: `singlePickBare` (the
one-card protocol, unchanged from #W61-T C8) and `singlePickDeclineLegal` (whether `PUT: none` is
a legal answer). Pelakka Predation therefore keeps its ONE-card protocol, loses both decline
clauses, and gains `This choice is NOT optional: while a card qualifies you must take one.`
An `<upto:1>` chooser keeps the decline wording verbatim; the fixed `<1>` Thoughtseize shape is
byte-identical to wave 20. This removes no window: the engine still accepts every card it accepted,
and only an answer the card forbids stops being advertised.

**R2 — the hold retires on ANY row change.** `holdStillStands` is now set EQUALITY over the rendered
rows, in both directions, with its own reason string (`a printed row it was held over is gone`).
This retires holds more often — one extra ask — and never fewer, so no window closes.

**R3 — the tapped-land classifier reads the card's OWN entry line.** `landEntersTappedLine` +
`scriptLineIsActivated` (both pure) scan EVERY line and reject three shapes before claiming
anything: a `@` trigger, an activated cost head (`{1}{T}{S}:` — a cost head is only `{...}` groups,
so `life:-2` is not one), and a tap that lands on a `target(`/`moveto(` card. A `choice` branch in
front of the tap makes the entry CONDITIONAL, which is the shockland shape. Bant Panorama now
annotates nothing; Blood Crypt says `enters tapped UNLESS its own condition holds`; Arcane Sanctum
and Isolated Chapel are byte-identical to wave 61 step one.

**R4 — a result that arrived decides before the clock.** `gptDeadlineMissed(emptyBody, elapsedMs,
timeoutMs, httpStatus, curlCode)` (pure) replaces the inline elapsed-time test in the worker: a
non-200 HTTP status, or a curl failure that is not `CURLE_OPERATION_TIMEDOUT` (28), means the round
trip FINISHED and is not a wall miss. A real wall miss (no status, or curl 28) is unchanged, so
#W53-Q's `timeout` class and #W55-E's wall-miss account keep exactly the population they had.

**R5 — the damage-wipe roster states its outs and its scope.** `WipeVictim` gains
`protectedFromSource` (from `MTGCardInstance::protectedAgainst(card)` — asked of the card that DEALS
the damage) and `canRegenerate` (from the same script read `#W60-Q` R6 uses). Protection means the
creature does not die at any amount and is listed under SURVIVES with its power still counted;
a regenerator stays in DIES, marked `(may survive: it can regenerate)`. What is NOT readable here
is the general prevention/replacement layer, so the header says so once:
`. DIES at N (before prevention): …`.

**R6 — crack-back ability sources must be usable.** `activationManaCost(costHead)` and
`crackBackCostAffordable(costHead, sourceStaysTapped, oppReach)` (both pure) gate BOTH activated
branches (the animator clause and the pinger clause). `sourceStaysTapped` is true only for a tapped
permanent that does not untap or is frozen — a tapped permanent otherwise untaps in their untap
step, which is the turn this line forecasts. `oppReach` is `ManaEngine::potentialColorReach` under
`FreeProducerPolicy`: the same integer the prompt's own "their open mana" line is built from.
Fails closed on an unpriceable cost head and on an unknown reach.

**R7 — the four-branch cap is gone.** `oneShotDrawGrantScan` stores every branch it can price.

**R8 — the blocking-lifegain ceiling counts only bodies that can block.** The per-blocker figures
and their pairwise legality are collected into two lists and summed by the pure
`blockingLifeCeiling`; a body that `couldBlockIfItAttacked` is false of for EVERY offered attacker
contributes nothing, so an all-shadow attack no longer "gets life back".

---

## Gate

Binary rebuilt from clean (`rm -f bin/wagic && make -f Makefile.sdl -j4`), every run memory-capped
at 4 G with swap off.

- Suite, `WAGIC_TESTSUITE_THREADS=1`: **1249 tests, 0 failed**, `==Test Failed !==` **0**,
  `==Test timed out` **0**, **65 AI tests, 0 failed**. (The brief's 1248 baseline predates the
  R–U merge, which added one fixture; 65 AI and 0 failures match it exactly.)
- PARSETEST: **3911 passed, 0 failed** (0 failed is the gate).
- `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

### RED-on-base evidence

No finding here changes what the ENGINE does with a card, an option or an answer — every fix is a
render, a classification or an ask-wording change, plus one predicate that decides whether an ask is
issued — so no suite fixture can be RED. RED was produced instead by reverting each mechanism to its
wave-61-step-one behaviour, with the new PARSETEST cases in place, and rebuilding. Two runs:

- Reverting R3, R4, R5 and R8: **14 failed** — the three R3 cases (Bant Panorama, Blood Crypt
  conditional, Blood Crypt not-flat), the two R4 cases (the late 503, the late connect failure), the
  four R5 cases plus the three updated #W61-U C10 pins, and the two R8 cases.
- Reverting R1, R2 and R6: **5 failed** — the R1 no-decline negative, the R2 disappearing-row case,
  and the three R6 cases.

R7 has NO RED case, and that is stated rather than smoothed over: the cap lived in
`oneShotDrawGrantScan`, which takes an `MTGCardInstance *` and has no pure seam, while the tag
renderer it feeds never had a cap. The shipped case pins the uncapped invariant on the render
(six branches render as six) and passes on base too; the cap's removal is verified by inspection.

### Cases updated, none deleted

- The wave-61 C8 case that asserted `you can never take two, and "PUT: none" declines` for a bare
  chooser is UPDATED to the mandatory shape, and the declining shape keeps its own case driven by
  the script arity.
- The wave-53 D2 case pinning a disappearing row as no re-opener is UPDATED to the set-equality
  expectation, with the positive (identical menu holds) and the added-row negative kept beside it.
- The three #W61-U C10 wipe pins are UPDATED for the `(before prevention)` scope.
- The wave-20/N-166p reveal calls gained the new flag with no text change.

---

## Predictions (falsifiable, for the next corpus)

1. **R1** — 0 reveal asks whose option one is a bare single-target chooser contain the string
   `PUT: none`; every Pelakka-class reveal narration reports exactly one discarded card, and the
   `146v125` s18 shape (a two-card take narrated on a one-card choice, then a decline) does not
   recur.
2. **R2** — 0 windows are auto-declined by a hold whose rendered row set differs from the held set
   in either direction; `hold_windows_skipped` falls (more asks), and no `stale_echo` cluster
   appears whose menu is a strict subset of the held menu.
3. **R3** — 0 `Play <land>` rows carry an `enters TAPPED` bracket for a land whose only
   `tap(noevent)` sits behind a cost head or on a fetched card; every shockland row reads
   `enters tapped UNLESS`.
4. **R4** — 0 records carry `wall_miss` together with an `http_status` that is neither 0 nor 200,
   and every such record's class is `http_error` with a bounded (not full-deadline) retry.
5. **R5** — 0 `DIES at N` clauses name a creature with protection from the printing source's colour;
   every damage roster carries `(before prevention)` exactly once.
6. **R6** — 0 `they can also aim ability damage at you` clauses name a source whose activation cost
   exceeds the opponent's open-mana count printed in the same prompt.
7. **R7** — a modal spell with five or more priced draw branches renders all of them; grep the
   corpus for `DRAW GRANT` rows and check no row's branch count is exactly 4 while its card's script
   holds more.
8. **R8** — 0 `ATTACK TOTAL` lines claim blocking lifegain on a board where no offered attacker can
   legally be blocked.

## What I did NOT verify

- **No live model probe.** Every claim here is fixtures, PARSETEST and code reading; no prompt was
  put to Spark, so the pilot-behaviour halves of the predictions are unmeasured.
- **R8 and the R2 live path have no board-level test.** The pure seams are pinned
  (`blockingLifeCeiling`, `holdStillStands`); that the collection sites feed them the right facts is
  verified by reading, not by a running game.
- **R4's retry budget is not exercised end to end.** `gptDeadlineMissed` is pinned and
  `pollCompletionRetry`'s arm reads `mLastTimeout` — that the bounded transport retry then fires for
  a late 503 follows from `retryableTransportFailure(0, 503, true)` being true, which I read but did
  not run against a live endpoint.
- **R6's reach is the opponent's OPEN mana**, per the lane brief's wording. A source they could
  afford only after untapping more lands next turn is therefore dropped from the clause — the
  conservative direction, but it is a scope choice, not a proof.
- **R5 does not price the general prevention layer** (a Fog, a Circle of Protection, a shield
  counter). The roster says so; it does not detect them.
- **R3's activated-line test is a cost-head grammar**, not a parse of the engine's own ability
  builder. A cost shape outside `{...}` groups would be read as "not activated"; none is known in
  the primitives, but I did not sweep the whole set.
- The `intrepid_adversary_repeated_payment.txt` order-sensitive flake lane T recorded was not
  re-investigated.
