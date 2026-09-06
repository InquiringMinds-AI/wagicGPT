# Wave-67 lane AX — I5, I7 + the transport MED and the `async_drops` census

Base: master `ff06cc432`. Branch `w67-lane-AX`, worktree `worktrees/lanes/w67-AX`.
Files changed: `src/AIPlayerGPT.cpp`, `src/AllAbilities.cpp`, `include/AIPlayerGPT.h`,
`bin/Res/test/_tests.txt`, new fixture `bin/Res/test/w67AX_reveal_refusal_takes_legal_card.txt`.
Every edit byte-spliced with python `rb`/`wb`; `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**
after every edit. Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); **nothing was
killed under the cap**. Every comment tagged `#W67-AX (<item>)`. No live model probe, no corpus run.

---

## I5 — a refused reveal VOIDS the spell, and the 600 s stall is the SAME event

### The root cause of the 600 s wait (this is the item's deliverable)

**What the driver was waiting on: a key it had already pressed and that the engine ignores.**

`MTGRevealingCards::CheckUserInput` (AllAbilities.cpp) ends a reveal's option-one chooser on
`JGE_BTN_NEXT` only inside

```
if (tc && (tc->targetMin == false || tc->maxtargets == TargetChooser::UNLITMITED_TARGETS))
```

Idyllic Tutor's option one is `optionone name(choose card) target(<1>enchantment|reveal)`
(`mtg.txt:57031`): `<1>` sets `targetMin = true` and `maxtargets = 1`, so **both** disjuncts are
false. A zero-target `BTN_NEXT` on that chooser matches no branch, presses nothing and returns
`false`.

The sequence, in the driver (`driveInteractiveRevealStep`):

1. phase 0 asks the model; the reply is refused, `decideReveal` returns -1, `mAIGraveSel` is empty,
   `clicked == 0`, the chooser is still armed → the `else` branch issues the finalize
   `CheckUserInput(JGE_BTN_NEXT)` and sets `mAIPhase = 3`. **The finalize is inert.**
2. phase 3 finds `abilitySecond == NULL` and re-issues the same inert `BTN_NEXT` ("building option
   two") — every tick, for ever.
3. Nothing in `revealProgressSignature(true)` moves: `mAIPhase` 3, `mAIClickIdx` 0, `mAIGraveSel`
   empty, `zone->nb_cards` constant, `ownChooser()` non-NULL, and `mAIPollTicks` frozen because no
   further model call is made. So the **full**-signature budget (`kRevealStallTicks` 20000 ticks AND
   `kRevealStallSecs` 600 s) runs out and `forceCloseStalledReveal("driver")` returns every revealed
   card to the library.

That is why both corpus stalls read `reveal_wait_driver_secs: 600` with `latency_ms: -1`: **no model
call was in flight, because the driver had stopped making them.** The 600 s and the void are one
event, not two, and the class is 2 of 2 — every refused reveal in the corpus (`126v162` s10 → s12,
`126v123` s51 → s53) ended this way. Reproduced in the fixture below; the base-binary trace is
literally `phase3: building option two (zone=5)` ten times, then the force-close.

### What shipped

* **The engine floor** (`AllAbilities.cpp`, phase 0). New pure `revealChooserCanDecline(targetMin,
  maxtargets)` — `CheckUserInput`'s own gate, lifted. When nothing was clicked **and** the chooser
  cannot be declined, the driver takes the first card `tc->canTarget` accepts instead of pressing
  the inert key. `canTarget` is exactly what the click enforces, so the pick is legal by
  construction and deterministic. This covers **every** path into a zero-target mandatory chooser,
  including the suite's async stub.
* **The seat's own degrade** (`AIPlayerGPT::decideReveal`). New pure
  `revealRefusalMustPick(pickExactlyOne, singlePickDeclineLegal)` and
  `firstEligibleRevealIndex(eligible, n)`. On an unusable reply where a decline is **not** a legal
  answer, the seat commits the first eligible row, records the class **`reveal_fallback_pick`** with
  the card it took (so `choice` is a real row and `chosen_text` a real name), narrates it, and
  returns 1. Where a decline **is** legal (`<upto:N>`, a bare optional target) nothing is forced and
  the old safe default stands byte-for-byte.
  *Not* Baka's `chooseCard`: it walks hand/library/battlefield/graveyard/stack/exile/command and
  **never the reveal zone**, so it cannot answer this seam at all. The first eligible row is the
  deck126 review's own recommendation and the only choice provable without a model.

### `reveal_wait_unexplained_secs` — read-after-consume, and a second instrument fault

`writeTransLog` stamps `latency_ms` from `mLastLatencyMs` and then consumes it (`mLastLatencyMs =
-1`); the reveal residual was read **after** that consume, so it always saw -1 and reported the whole
wait. Fixed by snapshotting `recordLatencyMs` beside the `latency_ms` field itself, so no future
reader can be re-ordered into the same fault.

Second fault found while pinning it: the wait is whole seconds off `time(NULL)` and the trip is
milliseconds, so a **floored** trip left up to 999 ms of itself in the residual — which is why lane
AT's own cases had to read `<= 1`. `revealWaitUnexplainedSecs` now rounds the trip UP to the wait's
granularity. Measured over the corpus's 16 reveal records:

| | reported (wave 66) | with the snapshot only | with the snapshot + granularity |
|---|---|---|---|
| sum of `reveal_wait_unexplained_secs` | **1622 s** | 1208 s | **1200 s** |
| records reading nonzero | 16 | 10 | **2** (the two force-closes) |

Two lane-AT cases were **AMENDED, not deleted** (`<= 1` → `== 0`, with the reason in place).

---

## I7 — the fallback record, and the reservation decline

### I7a — every fallback says what the heuristic executed

The heuristic's answer is known only **after** the seam returns, which is after the fallback record
is written — so it is stamped onto the `recovery` record that already trails it. New
`noteHeuristicExecuted(seam, row, text)` + three fields on the recovery record:
`executed_seam`, `executed_choice` (1-based printed row; **0 = a pass / no cast**, which is an
answer and is stated as one), `executed_text`. Captured with the same flush idiom as the rest of the
recovery latch, so a stamp cannot leak onto a later record; a seam that does not stamp stays silent
rather than claiming an unknown row. Report-only — no key, tail or behaviour moves.

Wired at the seams that produce the corpus's fallbacks (measured, 113 records):

| seam | records | covered |
|---|---|---|
| cast (`kind: ask` — all 46 `unparsed_reply` + 5 `named_row_reask` are the casting decision) | 51 | **yes** |
| priority (28 `unparsed_reply` + 20 `repeat_past_stop` + 1 `named_row_reask`) | 49 | **yes** (the 20 `repeat_past_stop` carry `choice >= 0` and latch no recovery — they are not fallbacks in the record's sense) |
| discard | 7 | already recorded its pick (`chosenText`) before this wave |
| reveal | 4 | 2 now answer with `reveal_fallback_pick`; 2 were the force-closes I5 removes |
| land drop | 0 this corpus | yes (same stamp) |
| wall_miss | 2 | no — no seam ran (see the transport MED) |

Also measured while scoping it: **75 of the 93** wave-66 recovery records already carried
`recovered_by` (the narration delta). The gap is real but narrower than "the executed action is not
in the record at all": the 18 silent ones are exactly the answers the log does not voice — a pass, a
decline — which is what `executed_choice: 0` now states outright.

### I7b — a reservation decline is honoured for the rest of the step

**The review's stated cause is wrong, and shipping its fix would have fired on nothing.** The
deck162 review reads `162v130` s16 → s17 as "one count inside a `{feeds:}` clause had moved".
Measured on the corpus's own `options_text`:

* s16 → s17: the row gains `in your hand: 1 - Liliana's Caress` where s16 read `in your hand: 0` —
  a card NAME, i.e. a **word**.
* s18 → s19: the row gains an entire `{spends 3 of your 4 untapped mana sources this turn; ...}`
  clause.

`holdStillStands` re-opens on both, correctly. A hold-shaped latch (`holdKeyRow`, which forgives
digits inside braces) would have held **neither pair**. Pinned as a PARSETEST case on the two
verbatim rows.

What did *not* move across all four windows is what the reserve arithmetic is made of:

* the **castable set** — one spell, Dictate of Kruphix, at every window;
* the **untapped-source count** — `{leaves 1 of your 4 untapped mana sources untapped}` on all four.

So the latch is `(turn, phase, castSetKey, untappedSources)`, taken only on a plain decline of a menu
that carries a `{reserve:` row, honoured only at the casting seam, and re-opened unconditionally by
the next step. `castSetKey` is built from the engine's own **candidate names** (`castSetKeyOf`), not
from the rendered rows: `optionSetKeyOf`'s stripper takes the first `}` after a `{`, and the reserve
clause NESTS mana symbols (`... Underworld Dreams {b}{b}{b} in your hand needs 3 ...`), so its
residue moves with the clause — pinned as a MUST-NOT-MATCH so nobody rebuilds the latch on it.
`reserveDeclineStillStands` is pure and both of its terms are pinned. A new gameend field
`reserve_decline_windows_skipped` counts what it holds.

**Slot-key discipline (wave61/corpus-livelock.md).** Nothing this lane adds reaches `mPromptTail`,
the ask key, the async slot key or the option-set key. `castSetKey` is an engine-side string built
from card names and never rendered; `mReserveDeclineSources` is an engine number; the three recovery
fields are written to the translog only. Shared choke points checked: `holdKeyRow` (untouched),
`optionSetKeyOf` (untouched — the latch deliberately does not use it), `revealWaitUnexplainedSecs`
(one reader, the reveal record).

---

## Transport MED — MEASURED; no engine change is warranted, and here is why

The two records are `130v146` s11 (`latency_ms` 900026) and `162v123` s9 (900022), both
`transport: curl=28,http=0,empty=1,connect_ms=20000,phase=wall`, `deadline_pct` 100.0.

* **Is the connect phase re-entered inside the deadline?** No. `GptConfig.cpp:871/995` set BOTH
  `CURLOPT_CONNECTTIMEOUT_MS` (`gptConnectTimeoutMs(timeout)` = 20 s) and `CURLOPT_TIMEOUT_MS` (the
  deadline) on every handle, so a connect that never completes dies at 20 s, not 900 s.
* **Is the band test on the wrong clock?** No. `transportPhaseFor` compares `latencyMs` against
  `deadlineMs`; `latencyMs` is the same elapsed figure `gptDeadlineMissed` and `deadline_pct` use.
  At 900026 of 900000 ms the verdict `wall` is correct, and `connect_ms=20000` on the stamp is the
  BUDGET that was in force, not evidence a connect failed (lane AT's own note says so).
* **Why no retry.** AP-R6's budget is `remainingTransportRetryMs(deadline, firstLatency)`, which is
  **0** after a wall miss by arithmetic. There is no missing ladder; the deadline was spent.

So both records are correctly classed and there is nothing here a parser change can recover. What
remains — 1800 s of a 127-minute run spent on two decisions that produced no bytes — is a
`WAGIC_GPT_TIMEOUT` question, and that dial is the owner's. The one lever that could abort earlier is
curl's low-speed abort, and it must NOT be taken: with `stream:false` the server legitimately sends
nothing until the whole completion is done, so a low-speed abort would kill exactly the slow-but-real
decisions the corpus's p99 is made of. **Not fixed, deliberately.** All five numbers are pinned in
PARSETEST against the corpus record.

## `async_drops` census (report only)

59 drops over 2098 round trips = **2.8%**, spread across the corpus; **0** `stale_livelock`, so this
is the self-healing re-ask class, not the wave-61 shape (which was 2644 livelocks over 3586
decisions).

| dimension | census |
|---|---|
| by drop ARM (the stamp's own first field) | casting **59** — every one; no combat, target or menu arm dropped anything |
| by the KEY HALF that moved | `question (or turn/phase)` 57 · `question and board` 1 · `board` 1 |
| by the record that stamped them | priority 45 · ask 14 |
| outcome | `re-asked` 59 of 59 — no drop reached the livelock breaker |

Nothing to fix: the drops are the async layer doing what it is for. The one thing worth carrying
forward is that all 59 are on the CASTING arm, so if a future wave wants the rate lower, that arm's
prompt-rebuild cadence is where to look.

---

## Gate

Detached unit `w67-AX-gate`, `MemoryMax=4G MemorySwapMax=0`, foreground until-loop wait on
`~/.gatelogs/W67_AX_DONE` (removed before launch). Binary rebuilt from clean
(`rm -f bin/wagic && make -f Makefile.sdl -j4`), freshness-checked by mtime after every make; qmake
untouched, no new sources.

| leg | this tree | base |
|---|---|---|
| build | clean link, 0 errors | — |
| PARSETEST | **4932 passed, 0 failed** | 4906 (**+26**) |
| suite (`WAGIC_TESTSUITE_THREADS=1`) | **1274 tests, 0 failed** | 1273 (**+1**, the new fixture) |
| AI tests | **67, 0 failed** | 67 / 0 |
| `==Test Failed !==` | 0 | 0 |
| `==Test timed out` | 0 | 0 |

Log: `~/.gatelogs/w67-AX-gate.log`.

## RED-on-base evidence

**One suite fixture, RED on base, GREEN after** —
`bin/Res/test/w67AX_reveal_refusal_takes_legal_card.txt`. It casts Idyllic Tutor, opts the scripted
seat into the REAL interactive-AI reveal driver (`revealasync` naming a card that is not in the
library, so the stub selects nothing — the same state a refused model reply leaves) and shrinks only
the no-progress TICK budget (`revealstallbudget 10`; the stall itself is entirely real).

* **Base** (this tree with only the AllAbilities.cpp driver hunk reverted, rebuilt, freshness-checked):
  ```
  [REVEAL] phase0 after clicks: clicked=0 tcAfter=0x... nbTargets=0 zone=5
  [REVEAL] phase0 finalize (same tick): tc=0x... nbTargets=0 zone=5
  [REVEAL] phase3: building option two (zone=5)     x10, nothing moving
  MTGRevealingCards: reveal from Idyllic Tutor made no progress (driver) ... FORCE-CLOSING it
  ==Card number not the same in player 0's library==, expected 4, got 5
  ==Card number not the same in player 0's hand==, expected 1, got 0
  ==Test Failed !==
  ```
  i.e. the corpus's exact defect: the tutor resolves, nothing is taken, the library is put back.
* **After**: `MTGRevealingCards: no card was selected for a reveal whose chooser cannot be declined -
  taking the first legal card (Wild Growth)` → `==Test Successful !==`.

**PARSETEST**: +26 cases. The four new pure predicates (`revealRefusalMustPick`,
`firstEligibleRevealIndex`, `revealChooserCanDecline`, `menuHasReserveRow`, `castSetKeyOf`,
`reserveDeclineStillStands`) did not exist on base, so "RED under reversion" is meaningless for them;
their falsifiable content is carried by MUST-NOT-MATCH and MUST-RE-OPEN cases and by the corpus-
verbatim repro rows. The two AMENDED lane-AT cases DO go RED under a reversion of the granularity
change (`revealWaitUnexplainedSecs(107, 106146)` reads 1, not 0).

## Predictions (falsifiable, for the wave-67 corpus)

1. **I5 (void).** **0** `reveal_stall_forced` records, and **0** `logEngineResolution` lines reading
   "the reveal could not be completed". Every `reveal` record carries `choice >= 1` and a real card
   in `chosen_text`, or the class `reveal_fallback_pick` with a real card. **Falsifier**: one
   force-close, or one reveal record with `choice: -1` on a window whose ask offered no `none`.
2. **I5 (stall).** No record carries `reveal_wait_driver_secs >= 600`. Weaker: total
   `reveal_wait_unexplained_secs` over the corpus is **0**. **Falsifier**: either.
3. **I5 (residual).** On every `reveal` record with `latency_ms > 0`,
   `reveal_wait_unexplained_secs == 0`. **Falsifier**: a nonzero residual on a record whose own
   round trip covers its wait.
4. **I7a.** Every `recovery` record whose `recovers_kind` is `ask` or `priority` carries
   `executed_seam`, `executed_choice` and `executed_text`; **0** of them are absent. The
   unadjudicable-fallback count (a fallback whose executed action cannot be read off the log) falls
   from 51 to **0** at those two seams. **Falsifier**: one recovery record at either seam with no
   `executed_seam`.
5. **I7b.** **0** pairs of consecutive `ask` records in one turn+phase where the earlier answered the
   plain decline on a menu carrying a `{reserve:` row, the later offers the same castable set at the
   same untapped-source count, and the later CASTS a reserve row. `reserve_decline_windows_skipped`
   is > 0 on at least one deck-162 seat. **Falsifier**: the `162v130` s16→s17 shape recurring, i.e.
   a flip inside one step with the source count unchanged.
6. **Transport.** Unchanged: `wall_miss_unrecorded_wall` still appears iff a decision spends the full
   deadline, and `transport_error` still appears only below the band. **Falsifier**: a
   `transport_error` record at `deadline_pct` 100, or a `wall_miss` under 95%.

## What I did NOT verify

* **No live model probe and no corpus run.** Every seat-side change is verified as a pure function
  over recorded bytes, as a suite fixture, or as a string — never as a decision a model made
  differently.
* **The seat-side reveal degrade (`decideReveal`) has NO suite coverage.** There is no endpoint under
  the suite, so the fixture exercises the DRIVER floor (which is what removes the void and the stall
  for every path); the seat's own `reveal_fallback_pick` branch is covered only by PARSETEST on its
  two pure predicates. If the two paths ever disagree, the driver's is the one that runs.
* **`firstEligibleRevealIndex` is deterministic, not good.** It takes the first row the chooser
  accepts, which on a tutor is the first eligible card in library order — a legal answer, not a
  chosen one. It exists so a refusal costs a window, not a spell; the model's own pick is still the
  only good answer.
* **The reserve latch is unmeasured against a corpus.** Its two terms are pinned as pure functions on
  the corpus's own rows, but whether it FIRES — and how often — is prediction 5's job. I did not
  measure how many casting windows in one step share a castable set and a source count, so the
  window count it will hold is unknown; if it turns out to hold many windows, the scope (one step) is
  the dial, not the doctrine.
* **I7a does not cover the target, menu, X-announce, blockers, attackers, bottom or mulligan seams.**
  38 sites call back into `AIPlayerBaka`; I wired the three that produce every fallback in this
  corpus (cast, land, priority) plus reveal's own record, and left the rest silent rather than
  claiming an unknown row. A wave whose corpus fails at another seam will need that seam wired.
* **The residual granularity change assumes `reveal_wait_secs` is second-granular.** It is
  (`time(NULL)` deltas); if that clock is ever made finer the rounding-up becomes wrong by up to a
  second in the other direction.
* **The transport verdict rests on reading the curl setup, not on standing up a hung endpoint.** I
  did not reproduce a 900 s wall against a real server, and I did not measure whether the two misses
  were the same endpoint or the same moment.
* **Nothing was checked against the two known concurrency-only failures** (`lifeline`,
  `merrow_reejerey`) or the intermittent `intrepid_adversary_repeated_payment`: the gate ran
  single-threaded and read 0 failures.
