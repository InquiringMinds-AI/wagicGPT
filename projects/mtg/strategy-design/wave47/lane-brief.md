# Wave-45 step-1 lane brief (common)

Repo: your lane WORKTREE path is given in your task (a checkout of /home/magi/Projects/wagicGPT on branch w47-lane-X). Work ONLY there; cd to it with an absolute path in every command (cwd persists across calls). bin/User and objs-sdl are pre-populated (incremental build). Game dir projects/mtg.
Your docket item is in projects/mtg/strategy-design/wave46/engine-ledger.md (HIGH + MED tables; the R-numbers map to wave46/general-strategy.md, which holds the evidence and the wave-46 prediction for each) — read
it in full, then the repro records in ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-155842/
(python, not eyes) BEFORE reading code. Verify any card fact against
projects/mtg/bin/Res/sets/primitives/*.txt (/usr/bin/grep; bare grep drops matches).

RULES (all standing, each cost real damage):
- Build: `cd <worktree>/projects/mtg && make -f Makefile.sdl -j8`; g++ is ground truth (ignore clangd). Add no sources.
- MEMORY CAP, every wagic invocation, no exceptions: prefix with
  `systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 --`. A kill under the cap is a
  RED test pointing at an allocation loop in YOUR diff — audit every loop you touched.
- Encoding: MTGRules.cpp / GameObserver.cpp are CP1252 (+CRLF); primitives hold non-UTF8 bytes.
  Edit those only by python rb/wb byte-splice. AIPlayerGPT.cpp / DecisionContract.cpp are plain.
  After any edit: `git diff | /usr/bin/grep -c $'\357\277\275'` must be 0.
- PARSETEST (`cd bin && env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_GPT_PARSETEST=1
  ./wagic`) must stay 0 failed and GROW: for every render string you add, ship a positive case, a
  negative that must NOT match, and the echo shape if the model could echo it. Append your CHECK
  block at the END of the corpus (merge order). Baseline 1337.
- Suite: `WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_TESTSUITE=1 WAGIC_TESTSUITE_THREADS=1 ./wagic`
  from bin (unset WAYLAND_DISPLAY DISPLAY). Count BOTH '==Test Failed' and 'Test timed out'.
  Baseline 1130/0 + 37 AI/0; exactly 1 'W43-1 ASSERT' line is EXPECTED (menace provocation).
  Worktree suites are advisory (slow); the merged gate on master is the one that counts.
- TRUST DOCTRINE: the surface owes the model truth. Render the true token, never delete; wording
  restriction-first; no affirmative substring a model can latch. Read the doctrine in
  wave46/strategy-writing-skill.md (search 'TRUST DOCTRINE' / 'perception') if unsure.
- Scope: touch ONLY the regions your item names; other lanes are editing other regions of
  AIPlayerGPT.cpp concurrently. No Res edits, no guide edits, no docket edits.
- COMMIT on your worktree branch when green (explicit files, NO AI attribution of any kind — no
  Co-Authored-By, no 'Generated with', no session links). Do not merge, do not push.
Final message (<=15 lines): files touched, mechanism, PARSETEST before->after count, suite
counts, the falsifiable prediction from the ledger restated, and what you did NOT verify.

WAVE-47 SPECIFICS: PARSETEST baseline 1337 (master 19e00cf66+). Lanes: J = R1 draw-punisher
summary line + per-draw cost on draw rows, R2 the bare cast/cycle/decline modal menu gets
cost + effect per row, R15 X=0 called out as a null cast on the announce row; K = R3 fold
the converter's doubling into the blocking-trigger tag + attackers footnote, R11 affirmative
`[no creature they control can block this attacker]`, R12 `THIS COMBAT` scope on the
zero-power STOPS clause; L = R4 manland/activation offer timing (surface at a main-phase
window or annotate the row as the turn's last offer), R9 price the Mulligan row + gate the
carried-PLAN block on pre-game asks, R13 land-drop asks: suppress the PLAN request + state
the consequence; M = R6 {effect:} per-clause budget keeping the last (ultimate), R7 own-side
{effect:} for permanents with no option row this window, R8 ranged collapse on the blockers
screen (mirror the battlefield line's), R10 `{c}` never a bare count in a colour set (find
which path flips x/c), R14 `(of which N are creatures, M able to attack)` + `{leaves N
untapped}` on cast rows. Stay inside your regions; the doctrine: engine correctness + LLM
UX (legality without constraint; information that maximizes good choices; efficient
inference). Ship the true token; restriction-first; never delete a surface.
