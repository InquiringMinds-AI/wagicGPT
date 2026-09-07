# Wave-70 lane BN — the Astra review, fixed

Branch `w70-lane-BN`, base `71b1e22be` (+ the review commit `4da25e707`). Twelve findings from
`wave70/codex-review.md` (Codex CLI 0.153.4, gpt-6-astra). Every one was REPRODUCED RED on the base
before it was fixed. Comment tag on every change: `#W70-BN (F<n>)`.

**The rule all twelve serve** (invariant 000, owner 2026-09-06): a reply is the PLAN line and then
the action line, and NOTHING else. Everything outside those two lines is `off_protocol_bytes` and
is never an input to a decision, a veto, a re-ask wording, a clamp or a fallback. Caps never bound
reasoning under thinking-on. A plan step is consumed by an ACTION, not by a parse.

No owner-ruled surface moved: the protocol text (`reply-protocol.txt` / `kReplyProtocol`) is
untouched, the thinking regime and the harness flags are untouched. Only code that failed to honour
them changed.

---

## Per-finding disposition

| # | Verdict | Reproduction (RED on base) | Fix |
|---|---|---|---|
| F1 | CONFIRMED | PARSETEST witness `!truncatedBlockCommitmentAbandoned("PLAN: block their attacker\nBLOCKS: B1:A1\nI should not block", true)` → **FAIL** on base (`~/.gatelogs/w70-BN-pt-red.log`) | `truncatedBlockCommitmentAbandoned` + `laterCodedBlockAssignment` + `declineSentenceIsLabelScoped` DELETED, with the blockers-seam arm that applied an empty declaration. A legal `BLOCKS:` assignment stands whatever the bytes after it say; a truncated reply with no usable declaration already has the seam's ONE truncation re-ask (#W68-BE R4) and then the heuristic. |
| F2 | CONFIRMED | Two PARSETEST witnesses (`salvageProseAttackers("I will attack with A1 and A3", 3, send) == 0`, `salvageProseBlocks("I will block A3", …) == 0`) → **FAIL** on base | `salvageProseAttackers`, `salvageProseBlocks`, `proseAttackerOrdinal` DELETED with both live call sites and the four PARSETEST cases that positively pinned prose salvage (47930/47958 and kin). |
| F3 | CONFIRMED | `!planArguesAgainstRow("PLAN: cast Tribute to Hunger\nCHOICE: 1 (Cast Tribute to Hunger)\nThis is a waste.", row)` → **FAIL** on base | the span is the PLAN **LINE**, not everything after the marker. The pronoun rule can no longer reach the action line or a trailing sentence, so no off-protocol byte selects a re-ask wording or a fallback class. |
| F4 | CONFIRMED | `!repeatPlanStopAndCurrent("PLAN: create three humans\nCHOICE: 2 (…)\nCorrection: stop 20; M 25")` → **FAIL** on base | new pure `protocolLinesOnly(reply)` returns the two permitted lines, in text order, byte for byte; both priority-seam stop scans read it. A stop stated in off-protocol bytes is not a stop, so it can neither supply the constraint nor drive the retry-exhaustion clamp — the counted action stands as written. A stop the PLAN line states is still the model's own stop and still executes. |
| F5 | CONFIRMED | `gptResolveMaxTokens(true,false,0,400,"ask",false,false).reasoning > 0` → **FAIL** on base | the unbounded-budget arm falls back to `kDefaultReasoningBudget` when the raw ceiling leaves the answer nothing, so reasoning is never 0 with `enable_thinking:true`. The PARSETEST case at the old ~75490 that ACCEPTED the starvation is flipped to MUST-NOT-MATCH; an explicitly configured small budget is still honoured verbatim. |
| F6 | CONFIRMED | `lane-BN-regime-gate-red.py` against the base gate → **FAIL** (want PASS) | the 0-token allocation check skips a record that carries `reasoning_forced_close` **and** native reasoning — phase two legitimately allocates 0. A forced-close record with NO reasoning still FAILs (pinned). |
| F7 | CONFIRMED | same script → **FAIL "rebuild"** (want WAIT) | `run_records` gates only records with a completed round trip (non-empty `prompt`); engine-answered records are skipped, so "no gateable records yet" is WAIT. A record that DID call the model and carries no `reasoning_chars` is still the stale-binary FAIL. |
| F8 | CONFIRMED | same script → **PASS** (want WAIT) | the minimum is PER SEAT LOG; a seat short of it holds the verdict at WAIT. PASS names the seat logs it checked, the harness remembers them in `$OUTDIR/regime-gate-state.txt`, and every later sweep gates the seat logs that appeared since. A FAIL is still terminal. |
| F9 | CONFIRMED | same script → **PASS** ×2 (want FAIL) | the off arm FAILs on any `reasoning_hidden` and on any `reasoning_tokens > 0`. An explicit `reasoning_tokens: 0` is not evidence of reasoning (pinned). |
| F10 | CONFIRMED | `lane-BN-plan-step-red.py` (the pointer lives on the seat object, so PARSETEST cannot drive it; the script replicates both rules over one window sequence) → base re-serves `cast Bear` after Bear executed, and skips `attack` after an empty reply and a re-ask | both advances DELETED from `consumePlan`; the pointer advances in `writeTransLog` on the record of an EXECUTED decision, via the new pure `AIPlayerGPT::planStepExecuted(hasPlan, modelCall, choice, fallback)` (PARSETEST-pinned). A new plan resets to 0 and its step one is consumed by that window's execution like any other. |
| F11 | CONFIRMED | `lane-BN-plan-split-red.py` over the 1,582 PLAN lines of `matchups-20260906-134120` against the engine's own 346 comma-bearing card names: base puts **13 boundaries inside a card name across 13 lines**, and averages **8.25 steps/line**; fixed puts **0** and averages **4.67** | a bare comma is no longer a boundary. Boundaries are `;` + whitespace, a newline, the word `then` (with or without a comma before it, cut BEFORE `then` so the remainder reads `then <action>`), and `.`/`!`/`?` followed by whitespace **and** an upper-case letter. `1,000` and `3.5` still never split. |
| F12 | CONFIRMED | `check-reply-instructions.py` on the base staged guides → **4 hits** (deck123:70 second template line, deck123:99 `theirs` ledger, deck130:113 both) | the three templates rewritten as one line of intended actions with the stop AS A STEP; the guard extended to scan `*_strategy.txt` for a ledger word after `PLAN:` and for a second, equally-or-more-indented template line. |

Deleted-symbol guard: the same script now fails the build if any of the six deleted prose readers is
defined or called again. **RED on the base source: 12 hits** (`git stash` + run, recorded below).

## What shipped

- `src/AIPlayerGPT.cpp` — F1/F2 deletions (3 functions + 3 helpers + 2 call sites + 4 PARSETEST
  pins), F3 span, F4 `protocolLinesOnly` + 2 call sites, F5 arithmetic + the flipped BK case,
  F10 `planStepExecuted` + the writeTransLog advance + the two removed parse-time advances,
  the `#W70-BN` PARSETEST block (21 cases), and three `#W70-BM E1` pins re-written to the new
  separator grammar.
- `include/AIPlayerGPT.h` — `planStepExecuted` declaration.
- `include/GptPlanCaveat.h` — F11 grammar (`planStepThenAt` + `planStepEnds`).
- `tools/regime-gate.py` — F6/F7/F8/F9 + `--state`; selftest 21 → **30 checks, 0 failed**.
- `tools/selfplay-harness.sh` — the sweep passes `--state`, a PASS no longer stops the sweeps
  (only a FAIL is terminal), and the 30-minute WAIT deadline fires only on "no gateable records".
- `tools/check-reply-instructions.py` — guide scan + deleted-reader guard.
- `strategy-design/wave70/guides/deck123,deck130` and the live `bin/Res/ai/baka/deck123,deck130`
  (see "one deliberate extension" below).
- `strategy-design/wave70/lane-BN-{plan-split,plan-step,regime-gate}-red.py` — the three
  reproductions that cannot live in PARSETEST.

## Gate numbers (this worktree, `rm -f bin/wagic && make -f Makefile.sdl -j8`, binary mtime checked before every run)

- **PARSETEST: 5302 passed, 0 failed** (base on this worktree 5283/0, `~/.gatelogs/w70-BN-pt-base.log`).
  Delta **+19**: 21 added in the `#W70-BN` block, 3 added in the BK C4 block (F5), and **5 removed** —
  the four prose-salvage pins (`C prose 'block A3' salvages…`, the ambiguity and negation pins, and
  `C prose 'attack with A1 and A3' salvages`) and the BK case that accepted the reasoning starvation.
  The brief expected a DROP; the removals are 5, and the replacements are more numerous.
- **Suite, `WAGIC_TESTSUITE_THREADS=1`: 1279 tests (0 failed), 67 AI tests (0 failed)**;
  `==Test Failed !==` 0, `==Test timed out` 0 (`~/.gatelogs/w70-BN-suite.log`, detached unit
  `w70-BN-suite`, `MemoryMax=4G MemorySwapMax=0`). lifeline and merrow_reejerey pass single-threaded,
  as the brief's baseline says they do.
- `tools/regime-gate.py --selftest`: **30 checks, 0 failed**.
- `lane-BN-regime-gate-red.py`: 0 of 6 failed against this tree; **5 of 6 failed** against
  `HEAD:tools/regime-gate.py`.
- `lane-BN-plan-step-red.py`: prints the base/fixed divergence, exit 0 ("RED on base").
- `check-reply-instructions.py src strategy-design/wave70/guides bin/Res/ai/baka`: **OK**
  (1 source file, 43 guides). `check-ctor-init: OK (118 files)`.
- `git diff | /usr/bin/grep -c $'\357\277\275'` → **0**.
- Guides: deck123 19,772 B / deck130 19,993 B staged; deck123 19,989 B / deck130 19,993 B live.

## One deliberate extension of the brief's path list

The brief lists `bin/Res` nowhere, but it orders the guard to scan `bin/Res/ai/baka/*_strategy.txt`.
The LIVE deck123 and deck130 carry the same ledger templates the staged ones did (deck123's is worse:
a two-line `PLAN: L <opponent life>, C <their creature count>, stop <L+C+3>; M is <M> now; / this
window: <x<N> or pass>`), so the guard cannot be both fatal and green without them. They are the
guides the model reads TODAY, so the same minimal template edit is applied there and committed with
`git add -f`. Nothing else in those two files was touched — this is not the staged-guide install,
which remains the orchestrator's step.

## Predictions for the wave-70 corpus (falsifiable)

1. No record carries `fallback: truncated_abandoned` or `truncated_abandoned_heuristic`, and no
   translog line says `prose-salvaged` — those classes can no longer be written.
2. `plan_contradicts_noop_row_reask` falls relative to `noop_row_zero_reask`: the plan half now
   requires the argument to be ON the PLAN line. If it does not fall at all, the corpus is telling
   us the model writes its argument in the plan, which is the shape the ruling wants.
3. Under `--thinking on`, every record has `max_tokens_reasoning > 0` except records stamped
   `reasoning_forced_close`, and the gate passes rather than killing the corpus at minute one.
4. `plan_paragraph_bound_cuts` and the step counts drop: PLAN lines average ~4.7 steps under the new
   grammar against ~8.25 under the old, so fewer plans reach `kPlanCarryMaxSteps`.
5. A seat that starts late and returns no reasoning kills the corpus within ~45 s of its fifth
   gateable record, not never.

## Weakest evidence / what I did NOT verify

- **The golden file's own PLAN exemplar uses BARE COMMAS** ("PLAN: swing all, Giant Growth after
  blockers are declared, Rhino in the second main") while the grammar now needs `;`, a newline or
  `then`. A comma-only plan is therefore ONE step: nothing is mis-split, nothing is lost, and the
  whole plan is carried every window (the pre-wave-70 behaviour) — but step consumption is a no-op
  for it. The protocol text is BL's and owner-ruled, so I did not touch it; aligning the exemplar
  (`swing all, then Giant Growth after blockers, then Rhino in the second main`) is the one edit
  that would make F10 and F11 pull together, and it needs the owner's go-ahead.
- **F10 has no RED PARSETEST run**, only the replication script and the pure predicate. The pointer
  is seat state; PARSETEST cannot construct a seat. The advance site (`writeTransLog`) is argued
  from the record contract, not observed live: if a seam ever writes a decision record with an empty
  `fallback` for something that did not actually execute, that record would consume a step.
- **The F11 corpus measurement is against wave-69 (thinking-OFF) PLAN lines**, which are number
  ledgers rather than action sequences. The 13-mis-split figure is a floor, not the rate a two-line
  corpus will show, and 346 comma-bearing card names is the primitives file's whole set, not the
  pool's.
- **Nothing here ran against a live model.** The regime gate's new arms are proven on synthetic
  records and files on disk only, exactly as lane BK's were.
- **`salvageProsePutList` survives** — a prose reader with NO live caller (lane BM deleted its gate),
  still defined and still pinned by the W25-2 PARSETEST block. It is not one of the twelve and
  deleting it means rewriting that block; it is the obvious next deletion.
- **`commitRetracted` still classifies `truncated_abandoned*`**, which can no longer be written. Dead
  branch, dead pins; harmless, listed here so the next reader does not read it as live behaviour.
- I did not re-fit the seam caps or the 6000-token budget (BK's open item), and I changed no
  rendered option row, tag or annotation, so no guide rule keyed on a printed literal moved.
