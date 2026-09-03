# Wave-56 step-1 lane brief (from wave55/engine-ledger.md — THE DOCKET; read your items there IN FULL)

Base: master 104668cfa (src identical to b228b8648 = wave-55 step-1 lanes A-E; commits since are guides/docs). Baselines on b228b8648, hermetic `make -B` (~/.gatelogs/w55-final-gate-*.log): PARSETEST 2636/0; suite 1229 with exactly 2 failed (lifeline.txt + merrow_reejerey.txt = the known concurrency-only pair, solo-green) + 51 AI/0, 0 timeouts (re-confirm on your worktree's first gate before you start; a THIRD failure or any timeout is yours). ALWAYS `make -B` before your gate if you touch a widely-derived header (wave-55 lane E: stale copied objs + a new virtual in Player.h = phantom segfaults). Model for any live probe: qwen36-35b-a3b at http://100.116.136.74:8084
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
`until [ -f ~/.gatelogs/W56_<lane>_DONE ]; do sleep 10; done` loop (timeout 1200 s) with the run
launched as `systemd-run --user --unit=w56-<lane>-gate -p MemoryMax=4G -p MemorySwapMax=0
--working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY DISPLAY; ...; touch
~/.gatelogs/W56_<lane>_DONE'` (a backgrounded subshell dies with the Bash call); never a Monitor,
never a background task; never end your turn with the commit pending; COMMIT on your lane branch
in your worktree (no AI attribution of any kind in the message — no Co-Authored-By, no
Generated-with, no session link; `git add -f` for anything under bin/Res; never CLAUDE.md);
`cd` into your worktree inside every Bash call; ALL scratch/logs/done-files under ~/.gatelogs/
or <wt>/projects/mtg/strategy-design/wave56/ — NEVER in ~ or ~/Projects; build with
`make -f Makefile.sdl -j4` from <wt>/projects/mtg (qmake only with `-o Makefile.sdl` if you add
sources); suite from bin: `env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1
./wagic` (count BOTH `==Test Failed !==` and `==Test timed out`); PARSETEST:
`WAGIC_GPT_PARSETEST=1 ./wagic`; scoped runs via WAGIC_TESTSUITE_FILE=test/_probe.txt
(Res-relative). Tag every code comment for your change `#W56-<lane> (Dn)`. State a falsifiable
prediction per item (the docket has one); write `wave56/lane-<lane>.md` (what shipped, gate
numbers, predictions, what you did NOT verify). Never write mechanisms as the owner's positions.
Diagnostics you add are compile-time gated (`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`).

Lane A — HOLD latch + priority seam (worktrees/lanes/w56-A): D1 the hold's re-open predicate
  becomes the RENDERED OPTION ROWS (name, cost, ordinals AND `{right now:}` prices) — byte-identical
  rows = the hold stands; any printed-row change re-opens it at once; state the incoming-lethal
  risk in the lane note (no suppression, no cap); D18 key the async slot on (seam, turn, phase,
  boardKey) and discard-without-re-issue ONLY when the seam changed (the land-drop/casting
  alternation across the turn flip); D16 `plan_choice_conflict` fires only when the prose names a
  card/row on THIS menu that is not the row the CHOICE took; D11 (a) PLAN refresh at a hard age
  (~40 windows), (b) when the retraction note fires on a plan > ~4 windows old, serve the note
  alone; D4 `chosen_text` is never empty while `choice >= 0` (executed row text or an explicit
  `<refused: ...>` reason). Predictions per item in the docket.
Lane B — render prices (worktrees/lanes/w56-B): D2 the ability-row `they would be at K` reads the
  LIVE seat's life (describeAction `else if (action.playerAbilityTarget || action.player)` branch,
  `pt = action.player` -> resolve to `this`/`opponent()` before `damagePlayerVerdict`; 238/248 rows
  computed from a constant ~166) — pin K against the record's `opp_life` in PARSETEST AND add a
  suite/probe check that reads a real Player; D6 `INCOMING THIS COMBAT: N attackers, M unblocked
  damage - you would be at K` on every window during the opponent's combat (blockable/unblockable
  split when the seat has bodies); D14 equipment cast rows `{right now: you control 0 creatures -
  this equips nothing}` / `{right now: <name> already wears ...}`; D15 `{same effect as row N, for K
  more mana}` on the dearer of two rows naming one card with identical verdicts; D9 a COUNT on the
  edict tie list (`they sacrifice ONE of these 24, their choice`); D13 the blocking-trigger gain as
  three SEPARATE annotations (dies / this combat gain / after-combat blocker count); D10 `Their
  untapped sources: N (colours they could make: ...)` under the opponent battlefield line.
Lane C — parser, records, X rows, the broken lever (worktrees/lanes/w56-C): D3 the `[RE-ASK]`
  conflict notice quotes the LATCHED coded line and takes its prose evidence from the same region
  (2/8 notices quoted the discarded first line and one turned a Starstorm cycle into a pass);
  D12 `reveal_stall` becomes a guarded boolean + `reveal_wait_ticks/_secs` (or is emitted only
  above the structural threshold), `reveal_stall_phase` carries the real phase or goes; D7 X rows
  carry the `{leaves N sources}` fit clause, a fourth marker for MONOTONE menus (`[<- largest
  affordable X - ...; no listed X does more]`), and the identical middle of a monotone run collapses
  like `#N` runs; D17 FIX the `WAGIC_GPT_BOARDINDEX=0` SEGFAULT (lane L's legacy per-host scan
  path; take a backtrace under gdb on a stub game — exit 139 at turn ~10 on the base too) so lane
  L's lever works: gate = a full stub selfplay game with the flag completes exit 0.
Lane D — MDFC land backs (worktrees/lanes/w56-D): D8 the land back face of an MDFC in HAND is
  never offered as a land drop (`autohand={0}:restriction{can play land,compare(isflipped)~equalto~1}
  ... flip(<back>) forcetype(land)` in borderline.txt:33968 / :1177 — `isflipped` is never set for
  a card in hand). CARD-SCRIPT FIRST: if a script form exists that offers the land face from hand
  (an `anyzone`/hand entry) ship it as data across every MDFC land in the pool sets; if the engine
  must change, the smallest change that makes the hand entry legal + a suite pin (fixture RED on
  base: Emeria's Call in hand, land drop available, back face playable). Guide obligation on the
  fix: deck146's "NOT LANDS FOR YOU" block retires (tell the orchestrator; do not edit guides).
Lane E — replay determinism (worktrees/lanes/w56-E): O8 (OWNER YES 2026-09-03) = audit A35: a
  second RNG stream for the heuristic AI (`aiRandomGenerator` on GameObserver seeded from the game
  seed, used by AIPlayer*/AIHints), route the four `std::rand()` sites (Rules.cpp:719-720
  srand/rand, WParsedInt.cpp:1144, TargetChooser.cpp:1249, GameStateDuel.cpp:3440) through the game
  generator, delete `WRand`; PLUS the equip-autotap replay blocker (wave-53 lane AA: a human
  transcript with an equip diverges because the engine auto-taps then refuses the recorded target
  click 60x) — pin it with the vpk12 transcripts under psp-work/logs/ (lane W's two, lane AA's
  deck5-vs-deck33). ACCEPTED COSTS (owner-ruled): pre-change transcripts stop replaying; seeded
  fixtures depending on AI draws are re-baselined (list every re-baselined fixture in the lane
  note with its before/after). Verify: replay a Baka-vs-Baka transcript with one seat swapped to
  GPT — shuffles identical.
Done elsewhere: D29 = wave-54 lane Q (shipped; cap 32 + work budget); D19 ruling stands (no
per-decision inference target). Carried: D5 discard verdict family (next wave — after D2's price
evaluator is trusted), D20-D24 LOW, D25/D26/D27, D30-D35. Merge order after gates: E, D, C, B, A.
