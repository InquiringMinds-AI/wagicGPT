# Wave-55 step-1 lane brief (from wave54/engine-ledger.md — THE DOCKET; read your items there IN FULL)

Base: master 8a1768d1c (= wave-54 step-1 + lanes P/Q + lane R stall-floor hotfix). Baselines on 8a1768d1c, hermetic `make -B` (~/.gatelogs/w55-base-gate-*.log): PARSETEST 2489/0; suite 1228 with exactly 2 failed (lifeline.txt + merrow_reejerey.txt = the known concurrency-only pair, solo-green) + 51 AI/0, 0 timeouts (re-confirm on your worktree's first gate before you start; a THIRD failure or any timeout is yours). Model for any live probe: qwen36-35b-a3b at http://100.116.136.74:8084
(auth: Authorization: Bearer $(ssh spark 'cat ~/.config/vllm/qwen35-api-key'); never print it).
Doctrine: enforce legality without constraining choice — NO hard caps on legal choices, NO
window removed, NO blind caches (D2's rejected-remedies paragraph is binding; D8(b) only AFTER
D8(a) shows the choice vacuous); every item adds a true token, a true row, a receipt, a stamp
or one re-ask, and deletes nothing; every parse/render change ships PARSETEST cases (positive,
must-NOT-match negative, echo shape of any new bracketed/braced annotation); every engine
change ships a suite pin (fixture RED on base, GREEN after); byte-splice (python rb/wb)
AIPlayerGPT.cpp, MTGRules.cpp, GameObserver.cpp (CP1252+CRLF), MTGAbility.cpp, AllAbilities.cpp,
primitives/*.txt, _cards.dat — they hold non-UTF-8 bytes; verify
`git diff | /usr/bin/grep -c $'\357\277\275'` = 0; use /usr/bin/grep; MEMORY CAP on every wagic
run (`systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 --` prefix; detached
units get `-p MemoryMax=4G -p MemorySwapMax=0`) — a kill under the cap is a RED test pointing
at an allocation loop in your change; WAIT FOR GATES WITH A FOREGROUND
`until [ -f ~/.gatelogs/W55_<lane>_DONE ]; do sleep 10; done` loop (timeout 1200 s) with the run
launched as `systemd-run --user --unit=w55-<lane>-gate -p MemoryMax=4G -p MemorySwapMax=0
--working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY DISPLAY; ...; touch
~/.gatelogs/W55_<lane>_DONE'` (a backgrounded subshell dies with the Bash call); never a Monitor,
never a background task; never end your turn with the commit pending; COMMIT on your lane branch
in your worktree (no AI attribution of any kind in the message — no Co-Authored-By, no
Generated-with, no session link; `git add -f` for anything under bin/Res; never CLAUDE.md);
`cd` into your worktree inside every Bash call; ALL scratch/logs/done-files under ~/.gatelogs/
or <wt>/projects/mtg/strategy-design/wave55/ — NEVER in ~ or ~/Projects; build with
`make -f Makefile.sdl -j4` from <wt>/projects/mtg (qmake only with `-o Makefile.sdl` if you add
sources); suite from bin: `env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1
./wagic` (count BOTH `==Test Failed !==` and `==Test timed out`); PARSETEST:
`WAGIC_GPT_PARSETEST=1 ./wagic`; scoped runs via WAGIC_TESTSUITE_FILE=test/_probe.txt
(Res-relative). Tag every code comment for your change `#W55-<lane> (Dn)`. State a falsifiable
prediction per item (the docket has one); write `wave55/lane-<lane>.md` (what shipped, gate
numbers, predictions, what you did NOT verify). Never write mechanisms as the owner's positions.
Diagnostics you add are compile-time gated (`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`).

Lane A — priority seam + HOLD row (worktrees/lanes/w55-A): D2 (a) the repeat/de-dup key becomes
  the option SET (row names, costs, ordinals), never the rendered string, so a life-tick inside
  a mandatory loop cannot defeat it; (b) the HOLD row's own key gets the same treatment (a life
  total moving inside a mandatory loop is not the board change the pilot meant) — the
  annotation itself is CORRECT and must not be trimmed; D4 offer the HOLD row on the seat's OWN
  turn once past Main 1 with the land drop spent (or always) — `holdBoardKeyOf` already retires
  it on any real change; D11 (a) exact row-label match beats prefix match (kills 13/16 false
  `pass_hold_ambiguous`), (b) make the two rows' leading words differ; D19 the declined-count
  annotation reaches the own-turn Main 1 / Main 2 pair (ANNOTATION only, no suppression); D21
  the HOLD row states its BENEFIT (`you have declined this same list N times this turn; taking
  this row skips the rest of their turn's identical windows`). Predictions per item in the docket.
Lane B — combat seams (worktrees/lanes/w55-B): D3 `all_assignments_illegal` gets ONE re-ask
  carrying the pruned pairs as text; a second all-illegal reply may declare no blocks, stamped
  `all_assignments_illegal_exhausted`; D13 a blockers header on D17-wave-54's pattern (`2 of the
  4 attackers need TWO or more blockers each; you have 2 blockers.`); D14 the fallback record
  names the pairs + reason with the class.
Lane C — cast-row / X / target render (worktrees/lanes/w55-C): D6 (a) the X menu always marks
  exactly ONE row and says what the mark means (`[<- best trade: ...]` when no X costs nothing),
  (b) the mark goes on the BOTTOM row of a collapsed `same kills as X=N` run or the kill list is
  restated per row; D7 (a) per-player `{right now:}` verdict when a `target=player` row
  enumerates both players (Devour Flesh 178/180), (b) state whether the row COMMITS a target or
  opens a follow-up ask; D10 `[NAMED BY THEIR <source>: casting this costs you 3 life and draws
  them a card - you would be at K]` on the cast row (the `[named:` TAG itself must not change);
  D15 ownership mark on single-target `{kills:}` lists; D16 ability rows pointing damage at a
  player print the life total (third wave carried); D17 fetch rows still colourless at deck123
  (21 renders) although lane E shipped `fetchLandColorsClause` — find why it did not fire.
Lane D — menus + collapse + discard (worktrees/lanes/w55-D): D8 the two-dimensional
  (source x target) collapse for ability menus (`Equip with Lightning Greaves targeting Human
  #1-#58 x58, all identical`) — a COLLAPSE, never a cap, index-safe map-back as lane P/D did;
  D18 library-reveal menus collapse duplicates the same way; D22 the `chooseaname` menu: correct
  header (a card-NAME menu, not modes) + per-row annotation of what naming it does, and keep the
  observer's `[named: <card>]` surface consistent; D9 the `discard` ask rows carry the three
  clauses the emitter already computes (`{spare: you control N lands}`, `{dead right now: they
  control 0 creatures}`, `{you already control one}`).
Lane E — reveal + observability + mulligan (worktrees/lanes/w55-E): D5 (a) extend lane F's
  force-close to ANY interactive reveal whose driver makes no progress, writing a `reveal_stall`
  field; (b) localise the per-tick prompt instability in the reveal menu build for
  `revealzone(opponenthand)` (A/B order: `WAGIC_GPT_AUDIT_M_OFF=1`, `WAGIC_GPT_BOARDINDEX=0`,
  `WAGIC_GPT_SCRIPTMEMO=0`) — fixture on Pelakka Predation's shape; D23 a 900 s wall miss
  always leaves a translog record; D12 the mulligan coverage clause's colour family says `on
  turn one` (or split the two families) — the count family unchanged.
Done elsewhere: D1 = lane R (stall floor, wall-clock + never-while-pending + WAGIC_STALL_FLOOR=0),
merged in the base; D30 = lane Q (merged: cap 32, work budget, WAGIC_OPT_SOURCES_CAP=14).
Carried: D20, D24-D27 LOW; D28 owner item; D29 replay (O8 RNG split = its own lane after this
wave's corpus); D31-D35. Merge order after gates: E, D, C, B, A.
