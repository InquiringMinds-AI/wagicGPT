# Wave-60 lane M — B4, B3, B13 (protocol economy and transport budget)

Base: master `8b922abf0`. Branch `w60-lane-M`, commit **`cf6cf3bb1`**.
Files: `src/GptConfig.cpp`, `include/GptConfig.h`, `src/AIPlayerGPT.cpp`,
`include/AIPlayerGPT.h`, `include/GptPlanCaveat.h`. No Res asset, no card script, no engine
rule: every item is a transport-budget, parse or render surface, so PARSETEST is the gate by
the same split lane H took for K1 (the suite has no network seat and `askModel` returns at
`mEndpoint.empty()` before any of this code).

---

## B4 — the connect timeout is a bounded fraction of the decision deadline

**Mechanism (one cause, three symptoms).** Wave 59's K1 removed a hardcoded 2.5 s connect cap
by setting `gptConnectTimeoutMs(timeoutMs) = timeoutMs` — connection setup became as generous
as the whole request. The worker's deadline test is
`timedOut = body.empty() && elapsedMs * 100 >= timeoutMs * 95` (AIPlayerGPT.cpp, `WorkerMain`),
so a connect that never completes elapses ~100% of the deadline and is **classified as a wall
miss**. That single fact produced all three of the wave-59 findings:

1. `curl=28` burned the full 900 s (`130v162` s21, `latency_ms 900020`) to reach the same
   heuristic handoff the 2.5 s cap reached in 2.5 s;
2. `noAnswerClassFor`'s `transport_error` arm is gated `!timedOut && curlCode > 0`, so it fired
   **0 of 2** — the class lane H added was unreachable for the shape it was added for;
3. `pollCompletionRetry`'s transport arm (bounded by `remainingTransportRetryMs`) was never
   taken; the **wall-miss** arm was, and D10 gives that arm a fresh FULL deadline by design —
   which is why `123v146` s7 finished at `deadline_pct 108.6` (977,594 ms). Lane H's stated
   guarantee was not falsified by its own arithmetic; it was falsified by the classification.

**What shipped.** `gptConnectTimeoutMs` moves out of the curl-only anonymous namespace to file
scope, is declared in `GptConfig.h`, and is pure over three named constants:

```
share  = deadline / kGptConnectShareDiv (8)
bounds = [kGptConnectTimeoutMinMs (5 s), kGptConnectTimeoutCapMs (20 s)]
        and never more than HALF the deadline
no deadline given -> kGptConnectTimeoutCapMs   (stated, never libcurl's 0 = unbounded)
```

900 s deadline -> 20 s connect budget; 120 s (the config default) -> 15 s; 80 s -> 10 s; 24 s
-> the 5 s floor. The half-the-deadline clamp is the load-bearing one: it is what makes it
impossible for connection setup alone to reach the 95% mark, so a connect failure is now
always distinguishable from a wall miss. Both curl transports call it (the decision transport
and `httpRequestFull`). Nothing else changed: the classification table, the retry predicate and
`remainingTransportRetryMs` are untouched — the bound is what lets them run.

Generous by doctrine (tens of seconds, not 2.5 s), finite, and stated rather than inherited.

## B3 — a repeated cleanup-discard index is deduped and counted, never rejected whole

**Correction to the ledger's reading, from the code.** `parseAttackerSet` has always set
`out[v-1]` only when it is false, so a repeated index never selected a row twice and the
answer's distinct part was never discarded: `125v126` s246 (16 numbers, `19` twice) put **15**
of the model's own choices into `chosen` and the heuristic filled the 16th. The reply was
nonetheless logged `unparsed_reply` with `parse_note` null, and there is no recovery window on
this channel — the prompt says "this is the ONLY ask for them". So the fault is three narrower
things than "rejected whole", and each is fixed at its own site:

* **The ask never said the numbers must differ.** `buildCleanupDiscardAskText` now ends its
  PUT: sentence with `; the numbers must all be DIFFERENT (a number written twice counts once)`
  on every multi-card discard (a 1-card ask cannot have a duplicate and does not print it).
* **The caller could not tell a SHORT list from a REPEATED one.** `parseAttackerSet` gains an
  optional `int * repeatedOut`, incremented where a bare index names a row this reply already
  named. Range overlaps (`PUT: 1-4, 3-6`) are excluded — only `n == hi` counts — because a
  range is not a repeated index. The set semantics are byte-identical to before.
* **A repeat that left the list SHORT had no re-ask.** When `repeatedIdx > 0`, the distinct
  count is below `over`, and this hand has not already been re-asked, `cleanupDiscard` mints ONE
  corrected question (`mDiscardReaskKey` / `mDiscardReaskLine`, the `#W49-S` D8 idiom from the
  ask seam), logs the first attempt as `distinct_index_reask`, and returns in-flight. The latch
  is keyed on the ask text and cleared when the discard completes, so it is one re-ask per
  discard, never a loop. A repeat whose distinct set IS complete is accepted as it stands and
  records `duplicate_index_deduped`; a short one that survives the re-ask records
  `distinct_index_reask_exhausted`.

## B13a — the carried plan is bounded, and withdrawn when it denies the board

**(i) The bound.** The emit site already applied `planParagraphBound` (#W54-A) and then a
1,600-character ceiling. 1,600 characters is not a bound on a deliberation stream: one wave-59
seat carried 410 echoes, **148 over 400 characters**, longest 1,599, and the 1,236-character
one at `126v125` s48 produced `CHOICE: 4` on a three-row menu (a `named_row_reask`, a recovery
and a re-ask — 137.0 s, invisible to the fallback counter as one record).
`gptcaveat::planCarryBound(plan, kPlanCarryMaxChars = 400)` replaces the ceiling: cut at the
last sentence end at or below the bound (falling back to a word break, then to the bound), and
**append `planTruncationMarker()`** —
`" [...the rest of your plan was not carried - restate it if you still mean it]"`. Under the
trust doctrine a silent omission is worse than a stated one: the model is told its own words
were shortened and invited to restate, rather than served a stump that reads like a complete
plan.

**(ii) The contradiction.** s48's plan said `No Sanguine Bond. No Exquisite Blood.` four lines
below a battlefield line ending `...; Exquisite Blood {4}{b} [enchantment] ...`. That is not a
stale plan, it is a false surface competing with a true one.
`gptcaveat::planDeniesOwnPermanent(plan, ownInPlayNames, deniedOut)` detects it **narrowly**,
on the observed shape only: a bare negation immediately governing the name (`no X`,
`not have X`, `n't have X`, `lack X`, `without X`, `missing X`) or a copular denial after it
(`X is not on the battlefield` / `in play`). Two exclusions keep it off true sentences — a
qualifier between the negation and the name (`no SECOND Exquisite Blood`) does not match the
pattern at all, and a zone qualifier following the name (`no Exquisite Blood in hand`,
`in the graveyard`, `in my library`, `in exile`, ...) is a different, true claim and is
excluded explicitly. The vocabulary is the **pilot's own** `game->inPlay` only, so a denial
about the opponent's board is never contradicted by the pilot's. On a hit the echo is withdrawn
and `planContradictedBlock` prints what it wrote, what the board shows and what to do next —
the retracted plan is never printed under its own retraction.

## B13b — already shipped; pinned, not re-implemented

`#W57-D` (D29) collapses identical consecutive events at the narration WRITE seam
(`collapseAdjacentDuplicate` -> `collapsedRunNarration`), which is exactly the item's ask, and
the wave-59 corpus says it works: across all **42** seat files' final rendered logs there are
**0** consecutive duplicate event lines and **34** collapsed renders. The 50
`- Opponent drew a card` lines that deck126 HIGH-2 counts are one per turn, separated by a
`- Phase:` marker — a real chronological separation, and collapsing across it would falsify the
append-only log. Five PARSETEST cases pin the shapes the item names (`Opponent drew 5 cards`,
the generic `(x3)` form, the token plural, and the two negatives) so a later change to the
renderer cannot quietly remove them. **No behaviour change shipped for B13b.**

## B13c — the declined-list counter counts only asks the model saw

`[you declined this exact list N times already this turn]` is a sentence addressed to the
model, and after `#W59-J` (K10) the ask seam answers some windows without showing them: the
K10 latch re-serves the seat's own answer, and the state+question cache replays one. Both
return early from `askModel`, and the **cast seam** then incremented `mListDeclineCount`
anyway — 23 in a turn with 9 asks (`125v126` t31), 9 over 7 windows (`130v125` t67).
`askModel` now sets `mAskAnswerReserved` false at entry and true on exactly those two
answer-reuse returns; the two `FindCardToPlay` increments are guarded by it. The priority seam
needed no change — its increment already sits inside the model-call branch. Nothing is deleted
and no window is removed: the engine still re-puts the question and the model still sees every
window it is actually asked; only the number now describes what it says it describes.

---

## Gate

Hermetic build (`rm -f bin/wagic && make -f Makefile.sdl -j4`), then a detached unit
`w60-M-gate` under `MemoryMax=4G` / `MemorySwapMax=0`.

- suite, `WAGIC_TESTSUITE_THREADS=1` (ground truth): **1247 tests (0 failed)**,
  `==Test Failed !==` **0**, `==Test timed out` **0**, **62 AI tests (0 failed)**.
  The archived base binary `wagic-5039427da-w59step1` run in this same tree reads
  **1247 / 0, 62 AI / 0** — identical, so the count is base, not a drift.
- PARSETEST: **3571 passed, 0 failed** (base 3529; **+42** = B4 12, B3 10, B13a 15, B13b 5).
- `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

**RED on base, measured.** With the three wave-59 mechanisms restored in place
(`gptConnectTimeoutMs` = the deadline; `planCarryBound` disabled and the 1,600-char ceiling
back at the emit site; the `repeatedOut` increment disabled) and the rest of the lane present:
**3558 passed, 13 failed** — 7 B4 cases, 3 B3 cases, 3 B13a bound cases. Restored: 3571/0.
The B13b cases pass in BOTH builds, which is the evidence that item is already shipped.

## Predictions (falsifiable, for the wave-60 corpus)

- **B4** — no decision's `latency_ms` reaches the deadline with a `transport` stamp whose
  `curl` is nonzero and whose `http` is 0: a connect failure costs at most
  `gptConnectTimeoutMs(deadline)` (20 s at the corpus's 900 s wall), so every such record has
  `latency_ms` under ~2× that plus the retry, and **`deadline_pct` never exceeds 100 on a
  record carrying a transport stamp**. The positive arm: any decision whose transport stamp
  shows a connect failure and whose retry also failed is classed **`transport_error`**, not
  `wall_miss_unrecorded` and not `empty_reply` (wave 59: 0 of 2). If a `curl=28` record again
  reads `latency_ms` ≈ the full deadline, the bound is not the mechanism.
- **B3** — `PUT:` replies on the `discard` channel that contain a repeated index and are scored
  `unparsed_reply`: **0 of N** (wave 59: 2 of 66 discard windows, both permanent). Each such
  reply instead produces either a complete distinct set with `duplicate_index_deduped` in
  `parse_note`, or exactly one `distinct_index_reask` record followed by a second answer. And
  the negative: **no discard window produces two `distinct_index_reask` records** — the re-ask
  is one per discard.
- **B13a** — carried `YOUR PLAN (as you last stated it...)` blocks longer than
  `400 + len(marker)` characters: **0 of N** (wave 59: 148 of 410 over 400 in one seat, longest
  1,599). Every block that was cut ends with the truncation marker; a block without the marker
  is under the bound. And: **0** prompts render a `YOUR PLAN` block that denies, in the bare or
  copular form, a card the same prompt's own battlefield line prints for the pilot — the
  `YOUR PLAN was withdrawn:` line appears instead wherever that happens.
- **B13b** — unchanged from wave 59 and predicted to stay so: **0** consecutive duplicate
  event lines in any rendered GAME LOG.
- **B13c** — the maximum `[you declined this exact list N times already this turn]` in any game
  is **≤ the number of `ask`/`priority` records in that turn for that seat** (wave 59: 23
  against 9 asks, 18 against 7). A rendered N above that turn's record count is a lane break.

## What I did NOT verify

- **No live probe, no corpus run, no model call.** The connect bound has never been exercised
  against a real endpoint: that a bounded connect failure actually arrives with `timedOut`
  false, that `transport_error` then appears in a real translog, and that the transport retry
  recovers a decision are all reasoned from the code paths, not observed. The corpus is the
  gate.
- **No suite fixture is RED on base for any of the three items**, and none can be: the suite
  has no network seat, and `askModel` / `cleanupDiscard`'s model half return at
  `mEndpoint.empty()` before every line this lane touches. The RED evidence above is the
  PARSETEST measurement, which is where lane H put the same class of proof for K1.
- **The wall-miss retry arm is untouched.** A genuine wall miss (the model thought for the
  whole deadline) still gets a fresh full deadline, so a `deadline_pct` above 100 remains
  POSSIBLE on a `wall_miss` record. I did not change that: it is D10's deliberate allowance and
  removing it would delete the retry a real wall miss depends on. What this lane guarantees is
  that a CONNECT failure no longer reaches that arm.
- **B13a's contradiction detector is scoped to the observed shape.** It will miss a
  contradiction phrased any other way ("Sanguine Bond died", "I lost the Blood", an implicit
  denial), and I did not measure its false-negative rate on the corpus's 410 plan echoes — only
  that it fires on s48 and stays silent on the four true-denial forms in PARSETEST.
- **`kPlanCarryMaxChars = 400` is a judgement, not a measurement.** It comes from the review's
  own ">400 chars" census line; I did not test whether decision quality changes at 300 or 600,
  and a plan legitimately longer than 400 characters now arrives truncated with a marker.
- **The non-curl transports** (Android JNI, the PSP/no-transport stub) do not call
  `gptConnectTimeoutMs` at all and were not built or run here.
- **B13c's guard covers the cast seam only.** I did not audit every other consumer of
  `askModel` for a similar "what the model did" counter, and `mAskAnswerReserved` is currently
  read at exactly the two guarded sites.
- **I did not read any deck guide**, made no strategy judgement, and touched no other lane's
  item. Card facts: none were needed — no card script or Oracle text is involved in any of
  these three items.
