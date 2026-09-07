# Wave 70 — lane BK: the regime is a stated thing (harness, config, caps, gates, record stamps)

Audit section C. Base master 52ab71610, branch `w70-lane-BK`.

**The ruling this serves** (skill invariant 000, owner 2026-09-06/07): reasoning happens ONLY in
the native reasoning channel; a plan precedes the action; the reply is the PLAN line then the
action line and nothing else; per-seam `max_tokens` caps never bound reasoning tokens; the harness
requires the regime explicitly, gates each regime on its own evidence, and stamps it on every
record. The wave corpora run reasoning ON; thinking OFF is the product regime.

**The failure it exists to make impossible:** every corpus from wave 44 to wave 69 ran with
reasoning OFF, because `tools/selfplay-harness.sh:78` said `THINKING=0`, no launch recipe passed
`--thinking`, and nothing in the harvested data said which regime produced it. Three separate
surfaces had to change before that can be true again: the launch (a default), the record (no
stamp), and the caps (they bounded the thinking window silently).

---

## 1. `tools/selfplay-harness.sh` — the regime is a REQUIRED argument

- `THINKING=""`; `--thinking on|off` takes a VALUE (`--thinking) THINKING="${2:-}"; shift 2`).
- A launch with no regime, or any value that is not `on`/`off`, prints the ruling and **exits 2**
  before the endpoint probe, before the schedule, before a single game. Verified:
  `./tools/selfplay-harness.sh` → rc 2; `--thinking maybe` → rc 2; `--thinking on -u <dead>` gets
  past the regime block and fails on the endpoint probe instead (so the refusal is the regime's,
  not a parse accident).
- Header prints `thinking=ON|OFF` in capitals plus a `regime :` line naming what that regime's
  gate will require. `WAGIC_GPT_THINKING` is exported as `1`/`0` from `THINKING_ENV`.
- `DEFAULT_GPT_TIMEOUT` is 420 s under `on`, 240 s under `off` (unchanged arithmetic, new
  spelling of the condition).
- `--selftest` execs `tools/regime-gate.py --selftest` (21 checks, no server, no game, no corpus).

**RED on base:** `git show master:projects/mtg/tools/selfplay-harness.sh` still reads
`THINKING=0` (78), `--thinking) THINKING=1; shift` (93), `thinking=$THINKING` in the header (169),
`WAGIC_GPT_THINKING="$THINKING"` (196) — a launch without the flag runs a thinking-off corpus and
prints `thinking=0` in a line nobody reads. The lane harness refuses the same launch, rc 2.

## 2. The per-regime GATE — `tools/regime-gate.py`, wired into the supervisor

A separate, self-testable script rather than an inline heredoc, so its verdicts are provable
without a server. It reads the FIRST 5 gateable records of each seat log **this run** wrote
(`--per-file 5`, `--min-records 5`) and returns exactly one of `PASS` / `WAIT` / `FAIL <reason>`.

A *gateable* record is one that carried a real round trip. `reasoning_chars` is written on exactly
those (§4 below), so its **absence is itself a verdict**: a binary that predates this wave FAILs
the gate with "the regime cannot be verified — rebuild", instead of passing blind.

Both regimes first check the regime **stamp** on the record equals the regime the launch asked for
— that single check is what would have caught wave 44 on the day it happened.

| regime | requires | FAILs on |
|---|---|---|
| `on` | every checked record carries reasoning | `reasoning_chars == 0` on any record; `reasoning_hidden` on any record (provider withheld the trace ⇒ the reviewers are blind ⇒ invalid); `max_tokens_reasoning <= 0` on any record (a cap is bounding the window — ruling (d)) |
| `off` | no reasoning text, and the reply is exactly a PLAN line + an action line | any record carrying reasoning text (the server ignored `enable_thinking:false`); off-protocol replies above `WAGIC_CORPUS_PROSE_ABORT` percent (default 5) |

Shape is classified as `two_line` / `action_first` / `plan_only` / `action_only` / `off_protocol`,
and the counts ride the PASS line so a passing corpus still says what the model wrote. Under
`on` the shape count is **reported, not fatal** (the brief scopes the prose abort to `off`).

**Behaviour when it fires:** it is a ONE-SHOT decision (fleet rule `feedback-single-wake-batching`)
— the supervisor calls it every 45 s only until it says PASS or FAIL, then never again; a `WAIT`
that is still `WAIT` 30 minutes in becomes a FAIL ("the seats are not reaching the model"). On
FAIL it writes `$OUTDIR/REGIME-FAIL` with the reason, writes
`~/.gatelogs/${WAGIC_GATE_UNIT:-selfplay-harness}-REGIME-FAIL`, appends
`FAILED regime gate (thinking=…): <reason>` to `$WAGIC_DONE_FILE` when the launcher exports one,
prints a banner, and `kill -TERM`s the harness. The main flow then exits 1 with
"The logs in $OUTDIR are NOT a corpus and must not be reviewed as one."

**Verification:** `tools/selfplay-harness.sh --selftest` → 21 checks, 0 failed. It covers every
FAIL arm above, the WAIT arms, the stale-binary arm, the shape classifier, and an end-to-end pass
over real `.jsonl` files on disk including one written *before* the run (correctly not gated).
It has NOT been run against a live corpus (see §6).

## 3. Record stamps

- `thinking: "on"|"off"` on **every** decision record and on the **gameend** record. Verified
  absent before this wave: gameend carried no regime field and `mThinking` was never written
  anywhere in the log.
- `reasoning_chars` is now written on every record that carried a round trip, **present or zero**
  (it used to appear only when reasoning was non-empty, so "no reasoning" and "field not
  implemented" were indistinguishable — exactly the ambiguity the gate must resolve). The
  `reasoning` text itself stays present-only-when-true.
- `max_tokens_answer` / `max_tokens_reasoning` alongside `max_tokens`, so a corpus **proves**
  ruling (d) held instead of a code reading asserting it.

⚠ Consumers that counted `reasoning_chars`-present as "had reasoning" must now compare `> 0`.
I know of no such consumer in-tree; I did not audit the wave-review scripts under
`strategy-design/wave*/`.

## 4. Caps: `gptResolveMaxTokens`, and why the seam table did NOT move

The whole `max_tokens` arithmetic is now one pure function (`GptTokenPlan {answer, reasoning,
total}`) that PARSETEST can prove without a game or a server. The shape it enforces: a completion
under thinking carries two things and only one of them is anybody's to cap. The **answer ceiling**
(PLAN + action) is what a seam cap, a configured `max_reply_tokens`/`WAGIC_GPT_MAXTOKENS` and the
answer-locked retry size. The **reasoning budget** is its own summand, ADDED last — from the
configured budget, or, when the budget is 0/unset ("unbounded"), from the RAW ceiling minus the
answer ceiling, **never from the seam cap**.

Three shipped paths broke that silently, all three now impossible by construction:

| audit | shipped behaviour | now |
|---|---|---|
| B7.6 | `if (mMaxTokens > 0) maxTokens = mMaxTokens;` REPLACED the budget sum → an operator ceiling bounded the thinking window | the ceiling is the ANSWER ceiling and is added to the budget |
| B7.7 | thinking on + `reasoning_budget=0` → the seam table capped the WHOLE completion (768 tokens for ask, reasoning included) | seam cap sizes the answer; reasoning = raw ceiling − answer |
| B1.6/B7.10 | the answer-locked retry's flat 512 capped a completion that still had to think | 512 is the answer ceiling; the budget rides on top |

**RED on base, counted:** `strategy-design/wave70/lane-BK-cap-arithmetic-red.py` replicates the
master arithmetic and the lane arithmetic line for line and runs both over the exact assertions
the new PARSETEST block makes. **Base fails 6 of 10; the lane passes 10 of 10** — and the four
thinking-OFF cases are byte-identical, so the product regime's caps are unchanged.

```
thinking on, budget 6000, ask seam         6400 -> 6768   RED on base
thinking on, budget 6000, blockers         6400 -> 6896   RED on base
thinking on, budget 0 (unbounded), ask      768 -> 4096   RED on base   <- (d) violation
thinking on, ceiling 900, budget 6000       900 -> 6768   RED on base   <- (d) violation
thinking on, ceiling 300, budget 6000       300 -> 6300   RED on base   <- (d) violation
thinking on, answer-locked retry            512 -> 6512   RED on base   <- (d) violation
thinking off (4 cases)                      unchanged
```

**The seam table itself did not move, and the brief's expectation for it is what was wrong.** The
brief predicted a two-line reply would re-fit these caps under 200 tokens. Measured over
`matchups-20260906-134120` (2,282 replies), the bytes of the PLAN line **plus** the action line
alone — everything else discarded — are p99.5 ask 2,286 B, priority 1,943 B, attackers 2,416 B,
blockers 579 B, discard 492 B, reveal 384 B, bottom 623 B. At the worst-case 3.15 B/token that is
726 / 617 / 767 / 184 / 156 / 122 / 198 tokens: the three big seams need essentially the whole 768
they already carry, because today's PLAN line is itself a 2 KB number ledger. Lane BL rewrites the
PLAN into a short action sequence, which should move this distribution hard — but **that corpus
does not exist yet**, and sizing a cap on a rewrite's predicted effect is precisely the wave-68
error (lane BA sized on the previous corpus while the same wave moved the distribution). So:
nothing tightened, the measurement is in the code at `gptSeamMaxTokens`, and the re-fit is owed to
the FIRST two-line corpus.

## 5. `GptConfig thinking(-1)` — unset is loud, never silent

`thinking=-1` (nothing in `endpoints.txt`) still resolves to **OFF** — that is the product regime
the owner named, and an end user who never opened the GPT tab must not suddenly pay for reasoning.
What changed is that the client now remembers whether anyone *stated* it
(`mThinkingRegimeExplicit`): when nobody did, the first request logs the ruling to the GPT log AND
`DebugTrace`s it, once per seat. A corpus can never reach that branch — the harness refuses to
launch without the regime. A separate one-shot line fires when thinking is ON and the resolved
reasoning budget is 0 (an operator's own explicit tiny ceiling — honoured, and said out loud).

I did **not** flip the engine default to ON as audit C.2 proposes: invariant 000(c) makes the
regime an owner-ruled surface and 000(f) names thinking-off as the product regime, so the default
stays where the owner put it and the enforcement lives in the harness and the announcement.

## 6. Gate numbers (this worktree, hermetic `rm -f bin/wagic && make -f Makefile.sdl -j4`)

- binary freshness: `bin/wagic` rebuilt at 20:55, gate legs ran after it, under
  `MemoryMax=4G MemorySwapMax=0` in unit `w70-BK-gate`.
- **PARSETEST: 5437 passed, 0 failed** (baseline 5420 + **17** cases: 12 in the C4 block, 5 in the
  C5 block). Nothing was retired.
- **Suite, `WAGIC_TESTSUITE_THREADS=1`: 1279 tests (0 failed), 67 AI tests (0 failed)**;
  `==Test Failed !==` 0, `==Test timed out` 0. Matches the baseline exactly.
- `tools/regime-gate.py --selftest`: 21 checks, 0 failed.
- `git diff | grep -c $'\357\277\275'` → 0.
- `check-ctor-init: OK (118 files)` — the four new members are in the ctor init list.

## 7. Predictions (falsifiable, for the wave-70 corpus)

1. Launched `--thinking on`, every decision record carries `thinking: "on"`, `reasoning_chars > 0`
   and `max_tokens_reasoning == 6000`; `max_tokens == 6000 + the seam's answer ceiling`
   (6768 at ask/priority/attackers, 6896 at blockers, 6512 at the small seams). No record under
   thinking has `max_tokens <= 896`.
2. No corpus can be launched without a stated regime: the wave's launch line contains
   `--thinking on` or it produced no games at all.
3. If the model returns no reasoning (server default drift, a provider withholding traces, a bad
   `enable_thinking`), the corpus dies inside the first ~5 decisions per seat with
   `~/.gatelogs/<unit>-REGIME-FAIL` on disk — minutes, not hours.
4. Median decision latency under `--thinking on` rises well above the wave-69 thinking-off corpus
   (a 6000-token window is now actually funded); the 420 s timeout floor holds, i.e.
   `fallback: empty_reply` from the wall stays near its wave-69 rate.
5. A `--thinking off` probe run against the wave-69 protocol text FAILs the gate on shape (today's
   protocol is answer-then-PLAN). That is correct and expected until lane BL's golden file lands;
   it is not a gate bug.

## 8. What I did NOT verify

- **No live run.** The gate has never seen a real server: every verdict is proven against synthetic
  records and files on disk. The first live corpus is its real test, and a false FAIL there would
  kill a corpus — so read `~/.gatelogs/<unit>-REGIME-FAIL` before re-launching, not after.
- **No live request was inspected.** `enable_thinking` and the `max_tokens` sum are pinned by
  PARSETEST against a request built the way `buildRequestBody` builds it, not against bytes a
  server received. A regression that changes what `buildRequestBody` sends *around* those two
  fields would not be caught here.
- **The 6000 budget and the 400 answer reserve are not re-sized.** Both were measured on a
  different regime (audit B7.4/B7.5 mark them VERIFY); the corpus re-sizes them.
- **The seam table is not re-fitted for the two-line reply** — see §4. It is measured from the
  wave-69 corpus and owed a re-fit from the first two-line one.
- **`reasoning_chars`-present-vs-zero consumers.** I changed the field's presence semantics and
  did not audit the per-wave review scripts that may read it.
- **Interaction with lanes BL/BM is untested.** I touched no protocol text and no parser; the
  gate's `off`-regime shape check is written against the two-line shape BL is shipping, so the two
  must merge together for a thinking-off run to pass.
- **Nothing about play quality.** This lane changes what is measured and what is refused, not how
  the AI plays.
