# Wave-70 step-1 lane brief — THE REASONING-REGIME FIX (owner-ordered audit + fix of every parser and instruction)

Base: master b4b2f426c (wave-69 step one + the audit). Baselines on f152d0df4, hermetic `make -B`: PARSETEST 5420/0, suite 1279
with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at WAGIC_TESTSUITE_THREADS=1; plus
intrepid_adversary_repeated_payment.txt, a genuine intermittent), 67 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items come from `projects/mtg/strategy-design/wave70/instruction-parser-audit.md` (Table A instructions, Table B parsers, sections C/D) — read it in FULL first, then skill invariant 000 in /home/magi/.claude/skills/wagicgpt/SKILL.md (the owner ruling this whole wave serves) and memory `wagicgpt-reasoning-channel-only`. The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-69
corpus. Do not widen: nothing beyond your items, no refactors, no style edits.

Rules (all binding):
- Doctrine: enforce legality without constraining choice — NO hard caps on legal choices, NO
  window removed, NO blind caches; every change adds a true token / true row / stamp or one
  re-ask and deletes nothing the model relies on.
- Every parse/render change ships PARSETEST cases (a positive, a must-NOT-match negative, and the
  echo shape of any new bracketed/braced annotation). Every engine-behaviour change ships a suite
  fixture RED on base, GREEN after (state how you verified RED).
- Byte-splice (python rb/wb) AIPlayerGPT.cpp, MTGRules.cpp, GameObserver.cpp (CP1252+CRLF),
  MTGAbility.cpp, AllAbilities.cpp, primitives/*.txt, _cards.dat — non-UTF-8 bytes; after any
  edit verify `git diff | /usr/bin/grep -c $'\357\277\275'` prints 0. Use /usr/bin/grep for
  searches (bare grep is a wrapper that drops matches).
- MEMORY CAP on every wagic run: prefix `systemd-run --user --scope -q -p MemoryMax=4G
  -p MemorySwapMax=0 --`; detached units get `-p MemoryMax=4G -p MemorySwapMax=0`. A kill under
  the cap is a RED test pointing at an allocation loop in your change.
- Build from <wt>/projects/mtg: `rm -f bin/wagic && make -f Makefile.sdl -j4` (qmake only with
  `-o Makefile.sdl`, only if you add sources). Suite from bin: `env -u WAYLAND_DISPLAY -u DISPLAY
  WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 ./wagic` — count BOTH `==Test Failed !==` and `==Test timed
  out`. PARSETEST: `WAGIC_GPT_PARSETEST=1 ./wagic` (must read ` 0 failed`). Scoped runs:
  `WAGIC_TESTSUITE_FILE=test/_probe.txt` (Res-relative; delete the probe file after).
- Gates run DETACHED: `systemd-run --user --unit=w70-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w70-<lane>-*.log 2>&1; touch ~/.gatelogs/W70_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W70_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave70/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W70-<lane> (En)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave70/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W70-<lane> (En)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave67/codex-review.md and wave68/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave68/lane-BA.md .. lane-BE.md for the emitters you will touch, wave68/engine-seat.md for the
adjudication (wave 68: a merge comment at column 379 swallowed 30 ctor initializers and compiled clean — the guard `tools/check-ctor-init.py` now runs at link; lane BA sized caps on the previous corpus and the guide rewrite moved the distribution the same wave — size from THIS corpus and say what would move it; three wave-67 lane claims were FALSIFIED by the corpus — AW "row and header cannot differ", AX "zero retry budget", AY/AZ own-stop reachable — each was argued from code reading where a corpus count was available: COUNT before you claim; wave 66 shipped a PROTOCOL change whose composition cost — the model transcribing section names as labels — no PARSETEST case could see: a change to what the MODEL is told must be measured against the corpus replies verbatim, both the intended shape and the shapes the wording invites; verify your fix at the seam the corpus
exercises, by a corpus-line PARSETEST case or an end-to-end fixture, never only at the helper), and wave61/corpus-livelock.md + wave62/corpus-hang.md + wave63/corpus-hang.md (three corpus-killing defects in three waves; waves 64-68 ran clean; a lane that
touches a shared choke point - a chooser, a key, a marker - must ask what ELSE reads it):
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

Also binding this wave: NEVER chain a build behind a counting grep (`grep -c` printing 0 exits 1 and skips an `&&`-chained
make — lane AT gated a RED binary that way); freshness-gate the binary (`ls -la bin/wagic` after every make).

The guides are now ~20 KB decision-time-only documents (owner ruling 2026-09-06, Amendment 332): a render change that a
guide keys on must keep its printed string stable or say so in the lane report (the wave-68 seats found the losses were
rules keyed to literals that had stopped printing).

RULINGS THIS WAVE TOUCHES (owner, verbatim — every lane obeys, none re-derives): reasoning happens ONLY in the native
reasoning channel; "a plan had to precede the action"; the reply is the PLAN line then the action line and NOTHING else;
"the model CANNOT be told to writing nonsense OR IT FUCKING WILL WRITE NONSENSE" (no instruction may license prose, no
parser tolerance may teach it); the PLAN is "a scratchpad so it can execute sequenced decisions" — a sequence of intended
actions carried across windows, never reasoning; the wave corpora run with REASONING ON; the product regime is thinking
OFF and the reply shape is identical in both. WHY the wave exists: every corpus since wave 44 ran with thinking OFF (a
stale launch recipe in memory) and waves 66-69 built tolerance for in-band reasoning that taught the model to write it.

## Lane BK — harness, config, caps, gates, record stamps (audit section C) — FIRST, the others build on it
(1) `tools/selfplay-harness.sh`: the regime is a REQUIRED argument `--thinking on|off` (no default; a launch without it
prints the ruling and exits 2); the header prints `thinking=on|off` in capitals; `WAGIC_GPT_THINKING` exported
accordingly. (2) Per-regime GATE in the harness: after the first N decisions of the first games (N=5 per game or 3 minutes),
read the seat translogs — thinking on requires non-empty reasoning on every record (the record must carry
`reasoning_chars` — add it; verify `mLastReasoning` is captured off `reasoning_content` and the inline `<think>` path per
the dual-path capture); thinking off requires `reasoning_chars==0` AND reply = exactly a PLAN line + an action line, prose
counted, aborting past a threshold env (`WAGIC_CORPUS_PROSE_ABORT`, default 5%). A failed gate KILLS the corpus, writes
`~/.gatelogs/<unit>-REGIME-FAIL` with the reason, and the done-file says FAILED. (3) Stamp the regime (`thinking`) on every
record and on the gameend record (verified absent today: gameend 17137-17165 carries none; `mThinking` is never written).
(4) CAPS (the audit's biggest risk): `gptSeamMaxTokens` must NEVER bound reasoning tokens — under thinking on,
max_tokens = reasoning budget + an ANSWER CEILING (PLAN + action only), with `reasoning_budget=0`/unset meaning the
configured ceiling minus the answer ceiling, never the seam cap; under thinking off the seam caps are re-fitted to
PLAN+action bytes (a two-line reply — measure the audit's numbers, expect < 200 tokens) — and `WAGIC_GPT_MAXTOKENS` /
`max_reply_tokens` must not silently replace the budget sum (31586-31597). (5) PARSETEST pins: the corpus-mode request
JSON asserts `enable_thinking` follows the regime and `max_tokens` = budget + answer ceiling under on; a case that builds
the request for both regimes. (6) `GptConfig` `thinking(-1)` default: unset means the harness/env decides; nothing
defaults to off silently — an unset regime at request time is a loud DebugTrace + the ruling text. Fixture the gate with
a scripted seat where possible; else a harness self-test (`tools/selfplay-harness.sh --selftest`) that feeds synthetic
records to the gate function. Write wave70/lane-BK.md.

## Lane BL — every INSTRUCTION the model sees (audit Table A, all 29 CHANGE + 8 DELETE)
The reply protocol becomes ONE GOLDEN FILE: `projects/mtg/bin/Res/ai/gpt/reply-protocol.txt` (shipped, `git add -f`),
loaded at startup and code-appended exactly as `kReplyProtocol` is today (the same after-the-template position);
`kReplyProtocol` becomes a fallback identical to the file; PARSETEST asserts the compiled fallback == the file byte for
byte. Its content, in this order and nothing more: the reply is TWO labelled lines — `PLAN:` first (a short sequence of
the actions you intend across the coming windows, e.g. "swing all, Giant Growth after blockers, Rhino in main 2"; restate
it only when it changes; carry it otherwise), then the action line for THIS window (`CHOICE: N (row text)` / `ATTACK:` /
`BLOCKS:` / `PUT:` / X etc. per seam); no other text — no working, no explanation, no correction syntax, no announcement
headers, no "and why". Then every per-seam tail, re-ask text, `[RE-ASK]` wording, truncation re-ask, `DEFAULT ANSWER:`,
exemplar and hold/decline row wording listed in Table A gets its CHANGE/DELETE applied so that NOTHING says "answer
first", "then a PLAN line only if", "one short sentence of working", or licenses prose; re-asks quote the two-line shape.
The three guides Table A flags (deck123 ANSWER FORMAT correction rule; deck126 "engine runs your LAST one"; deck146
"write that sum into your reply") are edited to the ruling — staged in wave70/guides/deck<N>_strategy.txt (<= 20,000 B,
Amendment-332 grep 0) for the orchestrator to install; also the PLAN templates in deck123/125/130 that are number
ledgers rather than action sequences (Table A under (e)) are rewritten as sequences. PARSETEST: the golden text, every
rewritten tail (positive + MUST-NOT-MATCH on the deleted phrases), a grep-style guard that the whole set of instruction
strings contains none of: "working", "reasoning", "first line", "correction", "announce", "then a PLAN", "only if".
Write wave70/lane-BL.md.

## Lane BM — every PARSER and tolerance (audit Table B, all 21 CHANGE + 20 DELETE; VERIFY items get a census stamp)
Rewrite the reply reader around the two-line shape: locate `PLAN:` and the action label as LABELLED LINES; PLAN precedes
the action; the action line is step one of the plan. Fix the four that MIS-PARSE PLAN-first (`replyTerminatedNaturally` /
`truncatedBlockCommitmentAbandoned` 43548/43689 — PLAN present must not mean the answer was not cut; `postPlanOverrun` /
`recordReplyTrimmed` 16070/16111 and `preAnswerOverrun` 16161 — the meters count bytes OUTSIDE the two lines, both sides;
`planSaysPassThisWindow` 27540 — scan the PLAN line, not from CHOICE forward; the answer-first re-ask texts are BL's).
DELETE the pure in-band-reasoning tolerance rows the audit marks DELETE (correction markers/windows, restatement and
negation predicates, `proseReversesInCorrectionZone`, `planArguesAgainstRow`'s whole-reply fallback, the 300-byte ramble
guard's prose branch, `label_missing_*` salvage, heading-word skip, `later_answer_ignored` machinery, `answer_replaced`) —
delete, do not disable; their PARSETEST cases go with them (RED-on-base is the deletion compiling and the two-line cases
passing). Plan handling per (e): the split + verbatim carry stay; `planParagraphBound` / the 400-char cut must never drop
LATER STEPS of a sequence (bound by steps, not bytes, or raise to a sequence-sized limit); changed-only carry needs a
step-done marker (the action executed = step one consumed; carry the remainder); the stop ledger and truncation note stop
treating PLAN as prose. VERIFY rows (6): leave the mechanism, add a gameend census counter so the probe corpus decides.
Every corpus reply shape in PARSETEST becomes the two-line shape; add the literal `PLAN: ...\nCHOICE: N` pins the audit
says are missing, per seam, plus MUST-NOT-MATCH for prose-before-PLAN and action-before-PLAN (the latter is parsed —
the model may still emit it — but stamped `action_before_plan` for the census, never silently accepted as the intended
shape). Coordinate with BL only through the golden file's text (read it from BL's worktree if needed; do not edit it).
Write wave70/lane-BM.md.
