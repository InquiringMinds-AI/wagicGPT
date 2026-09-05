# Wave-62 lane AA — the eight findings of `wave62/codex-review.md`

Branch `w62-lane-AA` off master `e6067a67e` (the W-Z merge + the review commit). Build from
`projects/mtg`: `rm -f bin/wagic && make -f Makefile.sdl -j4` (qmake untouched, no new sources).
Every wagic run under `systemd-run --user --scope -p MemoryMax=4G -p MemorySwapMax=0`; the gate ran
detached as `w62-AA-gate` / `w62-AA-gate2` with a foreground `until [ -f ~/.gatelogs/W62_AA_DONE ]`
wait. No kill under the cap on any run. All five sources byte-spliced with python `rb`/`wb`;
`git diff | /usr/bin/grep -c $'\357\277\275'` = 0 after every edit.

## The table

Every finding was checked against the merged source BEFORE anything was edited.

| # | sev | verdict | the evidence |
|---|---|---|---|
| 1 | HIGH | **CONFIRMED** | `DecisionContract.cpp` had `req.canDecline = false` at the top of `buildMenuChoice` and assigned it in the `menu->announcing && menu->announceCost` branch ONLY; the ordinary cast-route `ANNOUNCE_X` branch set `req.kind` and returned. Reproduced: fixture `w62aa_cast_route_x_decline.txt` + new suite command `assertxdecline`, RED on the current tree — `expected an ANNOUNCE_X request with canDecline=1, got a request kind=4 canDecline=0 rows=4`. |
| 2 | HIGH | **CONFIRMED** | `restatedCombatDirective` guards a QUOTED token, a worked-example echo and the PLAN block, and nothing else; the prose before the label on the line is never read. Reproduced: 4 PARSETEST must-NOT-match cases RED on the current tree (`I should not use BLOCKS: B2:A2.`, `Do not write ATTACK: ...`, `don't`/`Never`, `Rather than`/`Instead of`). |
| 3 | MED | **CONFIRMED** | Two `out <<` statements per side in `serializeGameStateImpl` — lane X's `drawStepForecastText(..., loopCaution, myDrawNow, life)` immediately followed by lane Y's `drawStepForecastText(..., loopCautionForLine(...))`, and the same shape for the `theirs` half. A merge artifact: neither lane wrote two lines. Reproduced through the new single emitter (see "RED evidence" below). |
| 4 | MED | **CONFIRMED** | `avail = pool CMC + potentialColorReach(...)` (a SOURCE COUNT), then `paid = avail / perCmc` and `payRepeatRowCostTag` re-derived the same scalar division. No colour ever entered the claim. |
| 5 | MED | **CONFIRMED** | `oppNextTurnManaReach(opp)` counted every untapped-next-turn producer with no exclusion, and `crackBackCostAffordable` ended in `mana <= oppReach` — a scalar. The manland is counted among the sources that pay for its own activation, and `{3}{B}` passes against five Plains. |
| 6 | MED | **CONFIRMED** | `lifeLoopWinTail` is gated by `playerHasLifeLoop(p)` alone (`AIPlayerGPT.cpp:2125`, `:32069`), which is `converter && mirror` on one battlefield. No reader of `CANTCHANGELIFE` / `CANTLIFELOSE` / `CANTLOSE` / `NOLIFEGAIN*` sat anywhere on that path. |
| 7 | MED | **CONFIRMED** | `GptConfig.cpp`: `*codeOut = (res == CURLE_OK) ? httpCode : 0;` — the proving line. `httpCode` is `CURLINFO_RESPONSE_CODE`, already read, already 503, and thrown away. |
| 8 | MED | **CONFIRMED** | `gptDeadlineMissed` excused only `httpStatus != 0 && httpStatus != 200`, so a completed 200 fell through to `elapsedMs * 100 >= timeoutMs * 95` and returned true. The PARSETEST case at the old `#W61-V R4` block pinned exactly that (`a 200 with an empty body at the wall is still the model's own deadline miss`). |

**Nothing was refuted.** All eight reproduce.

## What shipped

Files: `src/DecisionContract.cpp`, `src/AIPlayerGPT.cpp`, `src/GptConfig.cpp`, `src/TestSuiteAI.cpp`,
`bin/Res/test/w62aa_cast_route_x_decline.txt` (new), `bin/Res/test/_tests.txt`.
All code comments tagged `#W62-AA (Rn)`.

### R1 — the cast route carries the Cancel row too
`DecisionContract.cpp`: `req.canDecline = !object->checkCantCancel();` added to the ordinary
cast-route `ANNOUNCE_X` branch — the same call the announcing branch already made. Nothing else
changed: `applyMenuChoice`'s decline click, the GPT ask's Decline row and the format line are lane
Y's and were already correct for whichever branch set the flag.

New suite command `assertxdecline <0|1>` (`TestSuiteAI.cpp`): with an X-announcement menu armed, it
builds the DecisionRequest for the live menu and asserts kind == `ANNOUNCE_X` and `canDecline`. It is
added to the driver's two "cannot answer / do not pre-answer this" lists exactly as `assertmanasources`
is, or the suite's menu default would answer the menu before the probe could read it.

### R2 — a restatement its own line negates is not an answer
`AIPlayerGPT.cpp`: new pure `combatDirectiveNegatedOnLine()` (word-boundary scan for `not`, `never`,
`avoid`, `reject`, `instead`, `rather`, `without`, `cannot`, and the apostrophe-folded contractions
`dont`/`doesnt`/`wont`/`shouldnt`/`wouldnt`/`mustnt`/`isnt`/`arent`/`wasnt`, plus `cant`, `refuse`,
`skip`), consulted in `restatedCombatDirective`'s scan over the text between the line start and the
label. Line-scoped on purpose: a rejection two lines up must not disarm a later affirmative
re-answer, which is the whole D9 recovery. The payload is not scanned — the roster grammar already
refuses prose there.

### R3 — one forecast line per draw step
`AIPlayerGPT.cpp`: new `drawForecastBlock()` — the single emitter both call sites now go through,
carrying lane X's tense + life verdict AND lane Y's direction-correct loop clause on ONE line. The
stale direction-agnostic emit at each site is deleted. `loopCautionForBoards`' result still feeds
`drawPunisherSituationLine` (the board paragraph), untouched.

### R4 — the pay-repeat bill prices colours
`AIPlayerGPT.cpp`: the CHOOSE_MENU pay-repeat block now asks the MANA ENGINE for the largest K whose
K copies of the per-counter cost it can pay — `getManaPool()->canAfford(bill, anyMana)` first, then
`ManaEngine::planPayment(...)` — and passes K to `payRepeatRowCostTag` as a new `paidCopies`
argument. The magnitude (`avail`) is still printed as the magnitude; it no longer decides how many
payments it covers. Where the cost text will not parse, the row prints the arithmetic bound as a
BOUND: `which pays for up to N of them if your colours fit`. Lane Y's two D7 cases were updated to
pass the count explicitly; their WORDING is unchanged.

### R5 — the next-turn manland gate
`AIPlayerGPT.cpp`: `oppNextTurnManaReach(opp, exclude, outColorMask)` — the permanent whose own
activation is being priced is dropped from the count, and the walk returns the producible colour set
as a bit mask. `crackBackFloorSources` computes that reach PER CARD (the fallback to this turn's open
mana, and its colours, is unchanged). New pure `crackBackColorsFit()` — every purely-mana pip in the
cost head must have a source of an acceptable colour; sacrifice/exile/loyalty heads and hybrids with
a generic half carry no requirement; a mask of -1 (unread) claims nothing.

### R6 — a life loop wins only where their life can move
`AIPlayerGPT.cpp`: new pure `lifeLoopWinnable(loopClosed, theirLifeCanChange, iCanGainLife)` and the
board reader `lifeLoopProvenWin(me)` over `CANTCHANGELIFE` / `CANTLIFELOSE` / `CANTLOSE` on their
battlefield and `CANTCHANGELIFE` / `NOLIFEGAIN` here, `NOLIFEGAINOPPONENT` there. The two win-claim
call sites (the edict clause's `myLoopClosed`, and `damagePlayerVerdict`'s `myLifeLoop`) now read it.
`lifeLoopWinTail` itself is byte-identical — it is the GATE that moved. The OPPONENT-loop tags
(`playerHasLifeLoop(opponent())`, D2's blocking-gain rule) are untouched: they make no win claim.

### R7 — the transport keeps the status it received
`GptConfig.cpp`: `*codeOut = httpCode;`. `CURLINFO_RESPONSE_CODE` is itself 0 when no status line
came back, so it is already the "nothing arrived" sentinel and the zeroing was pure loss. A 503 that
then stalls until curl 28 now reaches the classifier as `(503, 28)` — `gptDeadlineMissed` refuses it
(a status arrived), and it classes `transport_error` with the BOUNDED transport retry instead of
`timeout` with a fresh full deadline. The in-game connection probe also names the real status now.

### R8 — a completed 200 is `empty_reply`
`AIPlayerGPT.cpp`: `gptDeadlineMissed` now returns false for ANY non-zero `httpStatus`. A round trip
that came back with a status COMPLETED; the elapsed fraction cannot change what arrived. The wrong
pin was replaced (not deleted) with the right one plus its consequences: the class is `empty_reply`
and `retryableTransportFailure(0, 200, true)` is false, so an answered empty 200 buys no retry at
all. The real wall miss — no status, or curl's own clock — is pinned unchanged as MUST-NOT-MATCH.

## Gate

| run | result |
|---|---|
| PARSETEST (`w62-AA-parsetest.log`) | `=== self-test: 4111 passed, 0 failed ===` (base `e6067a67e`: 4072 — **+39** cases) |
| suite, `WAGIC_TESTSUITE_THREADS=1` (`w62-AA-suite-t1.log`) | `1253 tests (0 failed), 66 AI tests (0 failed)`, `==Test timed out` count **0** |
| suite, default threads (`w62-AA-suite-mt.log`) | `1253 tests (2 failed), 66 AI tests (0 failed)`, 0 timeouts — `lifeline.txt` + `mephidross_vampire.txt`, the known concurrency-only pair (both 0 at THREADS=1) |

Test count 1252 -> 1253: the one new fixture (not an AI test; the AI count was already 66 on base —
lane Z's `w62Z_designation_marker_not_a_permanent.txt` carries the `ai` command).

### RED evidence, per confirmed finding

- **R1** — RED on the current tree directly: with the one-line `DecisionContract.cpp` change removed
  and the binary rebuilt, `w62aa_cast_route_x_decline.txt` fails with
  `TESTSUITE assertxdecline: expected an ANNOUNCE_X request with canDecline=1, got a request kind=4
  canDecline=0 rows=4`, `1 tests (1 failed)`. GREEN after.
- **R2** — RED on the current tree directly (the function's signature did not change): the four
  must-NOT-match cases failed on the unfixed binary, the three POSITIVEs passed. `4078 passed,
  6 failed` on that build.
- **R8** — RED on the current tree directly: `#W62-AA R8 a COMPLETED 200 with an empty body is not a
  wall miss, however late` and its class case both failed on the unfixed binary.
- **R3 / R4 / R5 / R6** — these four change the SHAPE of the surface (a new emitter, a new argument,
  a new argument, a new predicate), so a case written against the fixed shape cannot compile on the
  base tree. Evidence is the counterfactual the wave-62 lane Y used for D4: each fix was reverted in
  place to the merged tree's exact behaviour (`drawForecastBlock` emitting the line twice;
  `payRepeatRowCostTag` re-deriving `paidCopies = available / perCmc`; `crackBackCostAffordable`
  returning before the colour test; `lifeLoopWinnable` returning `loopClosed` alone), the binary
  rebuilt, and PARSETEST read `4099 passed, 12 failed` — 3 R3 cases, 4 R4, 1 R5, 4 R6. Restored and
  rebuilt it reads `4111 passed, 0 failed`.
- **R7** — no RED. See "What I did NOT verify".

## Predictions (falsifiable, for the wave-62 corpus)

1. **R1** — no ANNOUNCE_X ask record will show a one-row X menu, on either route. Every one will carry
   a final `Decline - do not cast this after all` row and the `(the LAST row of this menu declines...)`
   format line. Where `X pricing: your mana affords only X=0` appears on a cast row that is then
   taken, the X menu that follows offers 2 rows.
2. **R2** — no combat record will execute an assignment whose own reply line contains a negation
   token before the directive. Concretely: for every `blockers`/`attackers` record, the executed
   payload is absent from any line matching `(not|never|instead|rather|don't) .*(BLOCKS:|ATTACK:)`.
3. **R3** — no prompt will contain two `DRAW FORECAST` lines of the same side, and no prompt will
   contain both `LOOP CAUTION` and `LOOP SCOPE` on the same side's forecast. Corpus-wide the count of
   `DRAW FORECAST (theirs):` occurrences per prompt is exactly 0 or 1.
4. **R4** — no `{repeat cost: ...}` clause will claim `pays for N of them` for a per-counter cost
   whose coloured pips the same prompt's `Mana available:` line cannot produce. Where the engine could
   not decide, the clause reads `up to N of them if your colours fit`.
5. **R5** — no `noncreature permanents of theirs that can animate and attack` clause will name a
   manland whose activation needs a colour the opponent's board cannot make, and none will name one
   whose activation cost equals the source count only because the manland itself was counted.
6. **R6** — no `THIS WINS THE GAME` will print on a screen where the opponent's battlefield carries
   `cantchangelife`, `cantlifelose` or `cantlose`, or where this seat's side blocks life gain.
7. **R7/R8** — no decision record will carry `fallback: timeout` together with a transport stamp
   naming a non-zero HTTP status, and no `empty_reply` will be re-filed as `timeout` by its elapsed
   fraction. An answered empty 200 will show no retry.

## What I did NOT verify

- **No live model probe was run, and no corpus game.** Every claim above rests on PARSETEST, the
  suite, and source reading. None of the seven predictions has been observed on a real prompt.
- **R7 has no test.** The change is inside the curl call path and this harness has no server to drive
  it; the finding is CONFIRMED by the proving line and the fix is that line's inverse. I did NOT
  observe a 503-then-curl-28 round trip end to end, and I did NOT change the classifier's ORDER — with
  the status preserved, a `(503, 28)` outcome now classes `transport_error` (curl outranks HTTP in
  `noAnswerClassFor`), not `http_error`. That is a bounded retry rather than a fresh full deadline,
  which is what the finding asked for, but if the intended class was `http_error` the ordering change
  is a separate item and this lane did not make it. I also left `errBodyOut` gated on `res == CURLE_OK`
  (a body truncated by a transport failure is not trustworthy text).
- **R5's exclusion half is source-verified, not test-verified.** The PARSETEST cases pin the pure
  affordability rule (a reach of 3 fails a 4-mana head; a white-only mask fails a black pip). The
  board walk that DROPS the source from the count and builds the colour mask has no fixture — the
  suite cannot render a GPT option list. I also did not check whether excluding the source is ever
  over-strict for a mana-producing permanent whose priced ability neither taps it nor needs it to
  attack; I chose the conservative side deliberately.
- **R6**: I read `CANTCHANGELIFE`, `CANTLIFELOSE`, `CANTLOSE`, `NOLIFEGAIN` and `NOLIFEGAINOPPONENT`
  off `MTGDefinitions.h` and the zone-level `hasAbility` idiom in `Player::gainLife` /
  `Player::doDamageTest`. I did NOT build a board carrying any of them and watch the claim disappear,
  and I did NOT model damage-prevention effects that are not zone-level abilities (Teferi's Protection
  in this engine may express itself some other way) — a prevention shape outside those five constants
  is still not read, and the claim would still print.
- **R4**: `planPayment` is used as the payability oracle and I did not measure it against a board with
  floating mana plus untapped producers; I read that it seeds `result` from the pool. The per-copy
  loop is O(n) `planPayment` calls per row (n <= the row's counter count, 20 for the Intrepid
  Adversary class) — I did not profile it, and it runs once per menu render, not per tick.
- **R3**: I verified the two call sites in `serializeGameStateImpl`. I did NOT audit whether any other
  emitter in the file prints a second loop clause for the same side (`converterSummaryText`,
  `pendingLoopWarningText`, `drawPunisherSummaryText` were left alone, as lane Y left them).
- **R2**: the negation list is a closed word list. A rejection phrased outside it ("I reject the idea
  of BLOCKS: ...", "BLOCKS: B2:A2 would be a mistake" — the rejection AFTER the directive) still
  passes the guard. The review's stated rule is prefix-scoped and that is what shipped.
- **R1**: the fixture pins Ivy Elemental's cast route only. I did not exercise a card whose X
  announcement arms with extra costs already committed, and the `assertxdecline` probe reads the
  request without consuming it — I did not test the probe against a menu that is NOT an X menu
  beyond its own failure path.
- **The base PARSETEST figure (4072) is DERIVED, not measured on a base binary.** The intermediate
  build (all new cases present, the R1/R2/R8 fixes absent) read `4078 passed, 6 failed` = 4084 total;
  minus the 12 cases added at that point = 4072, and 4072 + 39 = 4111 checks out against the
  per-block counts (R2 10, R8 +3, R3 5, R4 7, R5 8, R6 6). I did not build `e6067a67e` clean to read
  the number directly.
- The 2 default-thread suite failures are asserted to be concurrency-only on the strength of the
  single-threaded run reading 0 failed and both names appearing in the wave's known pair / the
  documented per-run churn. I did not run either solo.
