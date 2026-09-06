# Lane AP — the seven findings of wave65/codex-review.md

Base `19d592aac` (master after the AL-AO merge, `9e2440011`, plus the review commit `db3e32638`
and the lane brief). Branch `w65-lane-AP`, worktree `worktrees/lanes/w65-AP`. Two files:
`projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/GptPlanCaveat.h`. Every code comment is
tagged `#W65-AP (Rn)`. `git diff | /usr/bin/grep -c U+FFFD` = **0**.

Every finding was checked against the code BEFORE anything was changed. **All seven are CONFIRMED**
and fixed in the general form; none is refuted.

## CONFIRMED / REFUTED

| # | Sev | Verdict | Evidence (checked first) | What shipped |
|---|-----|---------|--------------------------|--------------|
| 1 | HIGH | **CONFIRMED** | `declineBoardScope` (`:29213`) returned `boardKey.substr(nl + 1)` — the key MINUS its first line — and `serializeGameStateImpl`'s first line is `Phase: <name> \| It is your/the opponent's turn.` So the whole phase/turn axis was stripped from the scope the two-decline allowance re-opens on: two declines given in upkeep retired the row through first main over an unchanged battlefield, and the PARSETEST case at `:65352` pinned exactly that (`declineBoardScope(b1) == declineBoardScope(b2)` for a phase advance) as desirable. What is LEGAL changes with the phase — every sorcery-speed action becomes legal in a main phase — so a refusal in one phase is not an answer to the next phase's question: a legal option removed. | The scope is the WHOLE key, header included (the brief's first option). The churn control the cap exists for is untouched at the granularity that produced it: a phase holds MANY priority windows, and within one phase over an unchanged board the two declines still stand; what a phase change buys is one fresh offer of a row the engine calls legal. The inverted case is re-pinned as a POSITIVE with the reason, and the malformed-key NEGATIVE is rewritten (a headerless key now scopes to itself, not to the empty string every other malformed key would also equal). |
| 2 | HIGH | **CONFIRMED** | `chooseAttackers` (`:39354-39395`) and `chooseBlockers` (`:40361-40408`) each carried their own walk: take the first parsing `ATTACK:` line, then let the LAST CLEAN one replace it (`attack_last_line_taken` / `blocks_last_line_taken`). #W65-AO's first-wins rule went into `findAnswerLabelLine` only, and its report claims the rule is label-wide while `kReplyProtocol` tells the model so. So `ATTACK: A1 / … / ATTACK: none` executed `none`, and `BLOCKS: B1:A1 / … / BLOCKS: none` executed the later unmarked line the protocol calls thinking-out-loud — a false surface at two seams. | The selection is extracted to `gptSelectAnswerIndex` (first usable CLEAN candidate; a later one supersedes only on an explicit #W65-AO correction marker, on itself or on the nearest preceding non-blank line) and **all three seams call it**: `findAnswerLabelLine`'s head walk, plus two new pure driver functions `gptAttackLineFromReply` / `gptBlocksLineFromReply` that the live drivers now delegate to whole. `collectLabeledLines` gained an optional preceding-non-blank-line output so the combat seams read the same correction window CHOICE does. PARSETEST exercises the DRIVER functions with the review's replies, not the helpers beside them. |
| 3 | HIGH | **CONFIRMED** | `findAnswerLabelLine` (`:17027`) ran the adjacency test first: `if (adjacent && !heads.empty()) heads.back().runLen++;` — an adjacent line never became a head, so its `correction` flag (computed only in the `else`) was never even reached. `CHOICE: 1 (Cast X)\nCHOICE: 2 (Cast Y) - CORRECTION` extended the first head's run and the engine executed the RETRACTED choice — the exact syntax `kReplyProtocol` had just told the model to use. | Correction detection runs BEFORE the run fold: a line carrying the marker ON ITSELF is always a head. The preceding-line cue is deliberately NOT consulted for an adjacent line — there the preceding line IS the answer being corrected, and reading its tail would turn an ordinary ranked list into a correction (pinned as a NEGATIVE). The W36 adjacent-run head rule is otherwise unchanged (a 3-line ranked list still answers with its head, `runLen == 3`). |
| 4 | HIGH | **CONFIRMED** | `attackTotalLine`'s ceiling was `blockGain` alone, and `blockGain` is `blockingLifeCeiling` over `blockTriggeredLifeFor` — blocking TRIGGERS only (`:39095-39109`). Nothing counted a blocker's own LIFELINK. Run the shipped formula on the review's board (they are at 5 behind one 5/5 lifelink blocker, two blockable 5/5s offered): `guaranteed = 5`, `blockGain = 0` → `5 - 5 <= 0` → **"that KILLS them whatever they block"** and `*outKillClaim = true`, which also suppresses the #W65-AN G6 loop clause on every A-row. Blocking one attacker gains them 5, they live, and their loop kills the seat. | New `blockLifelink` parameter (defaulted, so every window without one renders byte for byte as it did — pinned). The call site sums, over the same `couldBlockIfItAttacked` set the count line and the trigger ceiling use, each lifelink blocker's power (×2 for double strike). The verdict, the "as high as" figure and the kill claim all read the SUM, and the sentence names each half it folded rather than leaving a gap. |
| 5 | HIGH | **CONFIRMED** | `blockerLifelinkGain` (`:36283`) took one `blkFirstStrike` flag and both callers (`:22033`, `:22056`) pass it as `FIRSTSTRIKE \|\| DOUBLESTRIKE`, then the function returns `blkPower` — one power-sized gain for a creature that deals damage in two steps. The review's board: at 3 life a 3/3 double-strike lifelink blocker credited 3 instead of 6, so `lethalScreen` (`:21980`) projects death where the seat survives at 3 — a "no block saves you" verdict from an under-count. | A `blkDoubleStrike` parameter (defaulted false, so the whole shipped table is byte-identical). A double striker gains twice, except where the attacker's own first strike killed it in the first step — still a floor, still fail-closed. Both callers pass the blocker's `DOUBLESTRIKE`. |
| 6 | HIGH | **CONFIRMED** (fixed this wave, per the brief; refuted-out-of-scope twice before) | `GptConfig.cpp:871` sets `CURLOPT_CONNECTTIMEOUT_MS` (#W60-M B4's bounded connect budget) beside `CURLOPT_TIMEOUT_MS`, and curl returns `CURLE_OPERATION_TIMEDOUT` (28) for BOTH. `gptDeadlineMissed` (`:12556`) answered `if (curlCode == 28) return true;` unconditionally, so a connect timeout at ~30 s of a 900 s deadline became `timeout`, opened a `wall_miss` account and took the FULL fresh deadline (`:14177`) instead of `transport_error` with `remainingTransportRetryMs`. Wave 63's rationale (`0f1213f37`, R9) is about a 200-headers-then-stall that runs to OUR deadline — true of the request timeout, never proven of the connect timeout, which is exactly what wave-64 AK #3 recorded as the carried observation. | Code 28 is a wall miss only inside the same `elapsed * 100 >= timeout * 95` band every other wall verdict uses — the band `transportPhaseFor` already uses to stamp `phase=wall` on the record, so the class and the record can no longer disagree. Wave 63's own case is re-pinned as a MUST-NOT-MATCH (900,024 ms of 900,000, http 200, curl 28 → still `timeout`). |
| 7 | LOW | **CONFIRMED** | `planCarryBound` (`GptPlanCaveat.h`) bounded `std::string::size()` — BYTES — while `kReplyProtocol` (`:281`) and #W65-AO's pin both claim "the FIRST 400 CHARACTERS", and `planTruncationNote` reports the drop in "characters" off `size()` too. A plan with accented card names or non-Latin text was cut early and the cut could land inside a multibyte sequence. | The bound is denominated in CODE POINTS (`utf8Length` / `utf8ByteLimit`), so the protocol's number is true and the cut is always on a character boundary; the truncation note counts the same unit. ASCII is one byte per code point, so every shipped plan and every shipped case is byte-identical. (The brief's other option — saying "bytes" — was rejected: the protocol's number is the thing the model writes to.) |

## Files

- `projects/mtg/src/AIPlayerGPT.cpp` — R1 (`declineBoardScope`), R2 (`gptSelectAnswerIndex`,
  `collectLabeledLines`'s preceding-line output, `gptAttackLineFromReply`,
  `gptBlocksLineFromReply`, both drivers), R3 (`findAnswerLabelLine`), R4 (`attackTotalLine`'s
  `blockLifelink` term and the call site's ceiling), R5 (`blockerLifelinkGain` + both callers),
  R6 (`gptDeadlineMissed`), and the PARSETEST cases.
- `projects/mtg/include/GptPlanCaveat.h` — R7 (`utf8Length`, `utf8ByteLimit`, `planCarryBound`,
  the two note denominators in `planCarryCompose`).

## RED evidence

The same tree was rebuilt with the seven mechanisms reverted to their shipped semantics — the
header-stripped scope, both drivers' last-CLEAN-wins walks, the adjacency-before-correction fold,
the trigger-only ceiling, the single lifelink step, the unconditional code-28 verdict, and the byte
bound — and the new cases run against it:

- **base semantics: `4683 passed, 15 failed`** — R1 ×2, R2 ×3, R3 ×1, R4 ×3, R5 ×2, R6 ×2, R7 ×2.
  Every finding is represented; no fix rests on argument alone.
- **this tree: `4698 passed, 0 failed`.**

No suite fixture ships: nothing here is an engine-behaviour change (the GPT seams have no endpoint
under the suite), so the instrument is PARSETEST, and the R2 cases deliberately call the functions
the DRIVERS execute rather than the helpers beside them — the review's specific complaint about
wave-64's green.

## Gate

Detached unit `w65-AP-gate`, `MemoryMax=4G MemorySwapMax=0`, binary rebuilt from clean
(`rm -f bin/wagic && make -f Makefile.sdl -j8`), suite at `WAGIC_TESTSUITE_THREADS=1` (ground truth).

| leg | result | base (this tree) |
|---|---|---|
| build | clean link, 0 errors | — |
| PARSETEST | **4698 passed, 0 failed** | 4662 (+36) |
| suite | **1271 tests (0 failed), 67 AI tests (0 failed)** | 1271 / 67 |
| `==Test Failed !==` | 0 | 0 |
| `==Test timed out` | 0 | 0 |

Nothing was killed under the memory cap.

## Predictions for the wave-65 corpus

1. **R1** — no priority window omits a row it offered in an EARLIER PHASE of the same turn while
   that row is still legal. Falsifier: two windows in one turn, different phases, the later one's
   option list a strict subset of the earlier's with no cast, sacrifice or zone change explaining
   the missing row. Counter-measurement (the cost this buys): the per-window ask volume and the
   fetch-crack re-offer rate; if re-offers rise past one per phase, the scope — not the doctrine —
   is the dial.
2. **R2** — no `attackers` or `blockers` record executes a declaration read from a later coded line
   while an earlier coded line of the same label parsed, unless a correction marker is present.
   Falsifier: a record whose `chosen_text` matches the reply's SECOND `ATTACK:`/`BLOCKS:` line with
   no marker on it or the line above; `later_answer_ignored` on the record is the positive signal.
3. **R3** — no record carries a `CHOICE` executed from a line the reply marked as corrected on the
   line immediately below it. Falsifier: a reply containing an adjacent marked correction whose
   `choice` is the first of the pair.
4. **R4** — no `ATTACK TOTAL` claims a kill on a board where the opponent controls a lifelink
   blocker that can block one of the offered attackers and whose power covers the shortfall.
   Falsifier: `KILLS them whatever they block` on a screen whose blocker rows include a LIFELINK
   body with power >= (guaranteed - oppLife).
5. **R5** — every incoming-combat projection over a double-strike lifelink blocker prices twice its
   power. Falsifier: a `GANG BLOCK:`/best-case line whose life figure moves by exactly one power on
   a declared double-strike lifelink blocker.
6. **R6** — no record classes `timeout` (or opens a `wall_miss`) at a latency far short of the
   deadline; a `curl=28` record below the 95% band reads `transport_error` and its retry budget is
   the remainder. Falsifier: a `wall_miss` record whose `latency_ms` is under 95% of
   `WAGIC_GPT_TIMEOUT`, or a record whose stamp says `phase=connect` while its class says `timeout`.
7. **R7** — no carried `YOUR PLAN` line ends in an invalid UTF-8 sequence, and a plan of accented
   text is carried to 400 characters rather than ~200. Falsifier: a served plan whose final bytes
   are an incomplete sequence.

## What I did NOT verify

- **No live model probe and no corpus run.** Every change here is verified as a STRING or as a pure
  function over recorded bytes, never as a decision a model made differently.
- **R1's cost is unmeasured.** The allowance now re-opens on every phase advance, so a row declined
  twice can be re-offered up to once per phase instead of once per turn. I asserted (did not
  measure) that the churn wave-9 priced was window-level, not phase-level; prediction 1's
  counter-measurement is where that shows up. The alternative the brief allowed — re-open on any
  change to the LEGAL SET — would be tighter and needs the pre-decline candidate list as a key; I
  did not build it, because a legal-set key is a new key over volatile rendered text and the
  wave-61 livelock class lives exactly there.
- **R2 changes what wins for BLOCKS/ATTACK, and #W62-Z (D9)'s prose restatement now loses to an
  earlier coded line.** Under last-clean-wins a restatement in prose could replace the coded line;
  under first-wins it is taken only when nothing earlier parsed, or when it carries a marker. That
  is the rule the brief specified applied consistently, and it is a real behaviour change I did not
  measure against the corpus's actual restatement rate. No shipped case pinned the losing side (the
  D9 cases test `restatedCombatDirective` itself, which is unchanged).
- **R2's driver functions are the drivers' line CHOICE, not the whole drivers.** The salvage paths
  below them (`salvageLoopedSubset`, `salvageProseAttackers`, the bare-answer fallback) are
  untouched and are still exercised only through the seam. A PARSETEST case cannot reach
  `chooseAttackers` itself — there is no endpoint and no game under the self-test.
- **R4's lifelink ceiling counts a blocker once, for one block.** A lifelink blocker that could
  block only one of the offered attackers is credited its full power, which is the CEILING the seat
  must survive (correct direction for withholding a kill claim), but it is not an assignment: I did
  not build the pairing, and the number can therefore be higher than any single legal block
  produces. The line says it is a ceiling; it does not say which block reaches it.
- **R4 does not fold their blockers' lifelink into the per-attacker crack-back rows**, only into the
  ATTACK TOTAL's ceiling and its kill claim. A row-level claim that rests on their life is a
  separate emitter (#W64-AK R4's cover clause) and was out of this finding's scope.
- **R5's second step is credited on the attacker's damage alone.** Prevention, a pump in response,
  or removal between steps can still take the second hit away; the term is a floor in the same
  fail-closed sense the shipped one was, and I did not enumerate those cases.
- **R6 is proven at the classifier, not against a real connect timeout.** I did not stand up an
  unreachable endpoint and watch the record; the shape (http=0, curl=28, elapsed far below the
  deadline) is from `GptConfig.cpp:871` plus curl's documented behaviour, as wave-64 AK #3 recorded
  it. Also unverified: whether the wave-60 records that motivated #W61-U C13 were connect-phase or
  wall-phase — both read 900,024/900,027 ms, i.e. wall, so this change does not reclassify them.
- **R7 rewrites the DENOMINATOR, not the cut policy.** The sentence-boundary preference and the
  halfway-mark rule are unchanged; I did not check whether a non-Latin plan has `.!?` sentence ends
  at all (it may cut at the bound, which is what the ASCII path already does when it must).
- **Nothing was checked against the two known concurrency-only failures** (`lifeline`,
  `merrow_reejerey`) or the intermittent `intrepid_adversary_repeated_payment`: the gate ran
  single-threaded and read 0 failures.
