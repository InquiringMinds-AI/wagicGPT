# Wave-45 step-1 lane brief (common)

Repo: your lane WORKTREE path is given in your task (a checkout of /home/magi/Projects/wagicGPT on branch w45-lane-X). Work ONLY there; cd to it with an absolute path in every command (cwd persists across calls). bin/User and objs-sdl are pre-populated (incremental build). Game dir projects/mtg.
Your docket item is in projects/mtg/strategy-design/wave44/engine-ledger.md (HIGH section) — read
it in full, then the repro records in ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-111937/
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
  block at the END of the corpus (merge order). Baseline 1150.
- Suite: `WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_TESTSUITE=1 WAGIC_TESTSUITE_THREADS=1 ./wagic`
  from bin (unset WAYLAND_DISPLAY DISPLAY). Count BOTH '==Test Failed' and 'Test timed out'.
  Baseline 1130/0 + 37 AI/0; exactly 1 'W43-1 ASSERT' line is EXPECTED (menace provocation).
  Worktree suites are advisory (slow); the merged gate on master is the one that counts.
- TRUST DOCTRINE: the surface owes the model truth. Render the true token, never delete; wording
  restriction-first; no affirmative substring a model can latch. Read the doctrine in
  wave44/strategy-writing-skill.md (search 'TRUST DOCTRINE' / 'perception') if unsure.
- Scope: touch ONLY the regions your item names; other lanes are editing other regions of
  AIPlayerGPT.cpp concurrently. No Res edits, no guide edits, no docket edits.
- COMMIT on your worktree branch when green (explicit files, NO AI attribution of any kind — no
  Co-Authored-By, no 'Generated with', no session links). Do not merge, do not push.
Final message (<=15 lines): files touched, mechanism, PARSETEST before->after count, suite
counts, the falsifiable prediction from the ledger restated, and what you did NOT verify.
