# Wave-57 step-1 lane brief (from wave56/engine-ledger.md — THE DOCKET; read your items there IN FULL)

Base: master at your worktree creation (>= 7ee3a51e8 = wave-56 close: lanes A-E + hotfix lanes Y/Z/X/W + wave-56 guides + synthesis). Baselines on 4366e8eec, hermetic `make -B` (~/.gatelogs/w56-vpk16b-gate-*.log): PARSETEST 2736/0; suite 1241 with exactly 2 failed (lifeline.txt + merrow_reejerey.txt = the known concurrency-only pair, solo-green) + 55 AI/0, 0 timeouts (re-confirm on your worktree's first gate before you start; a THIRD failure or any timeout is yours). Model for any live probe: qwen36-35b-a3b at http://100.116.136.74:8084
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
`until [ -f ~/.gatelogs/W57_<lane>_DONE ]; do sleep 10; done` loop (timeout 1200 s) with the run
launched as `systemd-run --user --unit=w57-<lane>-gate -p MemoryMax=4G -p MemorySwapMax=0
--working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY DISPLAY; ...; touch
~/.gatelogs/W57_<lane>_DONE'` (a backgrounded subshell dies with the Bash call); never a Monitor,
never a background task; never end your turn with the commit pending; COMMIT on your lane branch
in your worktree (no AI attribution of any kind in the message — no Co-Authored-By, no
Generated-with, no session link; `git add -f` for anything under bin/Res; never CLAUDE.md);
`cd` into your worktree inside every Bash call; ALL scratch/logs/done-files under ~/.gatelogs/
or <wt>/projects/mtg/strategy-design/wave57/ — NEVER in ~ or ~/Projects; build with
`make -f Makefile.sdl -j4` from <wt>/projects/mtg (qmake only with `-o Makefile.sdl` if you add
sources); suite from bin: `env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1
./wagic` (count BOTH `==Test Failed !==` and `==Test timed out`); PARSETEST:
`WAGIC_GPT_PARSETEST=1 ./wagic`; scoped runs via WAGIC_TESTSUITE_FILE=test/_probe.txt
(Res-relative). Tag every code comment for your change `#W57-<lane> (Dn)`. State a falsifiable
prediction per item (the docket has one); write `wave57/lane-<lane>.md` (what shipped, gate
numbers, predictions, what you did NOT verify). Never write mechanisms as the owner's positions.
Diagnostics you add are compile-time gated (`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`).

Corpus-side facts every lane must hold: the wave-56 corpus is the FIRST with 0 SEGV (lane C's
D17 UAF fix); D2's constant-166 rows are gone (145/145 correct, 4 lethal rows decided 3 games);
the D18 slot rescue fires (8x) but 65 drops stand; `chosen_text` is tail-less on 112/320 HOLD
takes (count by PREFIX); D8 back-face rows 65 / 6 takes / 6 arrivals but 44 offered where illegal
— FIXED by lane W (4366e8eec: `LegalActionsOracle::canPlayLandNow`, actor = controller) — so the
docket's D1 and D3 are ABSORBED (verify, do not redo) and D2's guide half shipped (146-A).
Vita: vpk16 = 4366e8eec is live; the owner's reports (proliferate Y, cancel-in-interrupt Z,
Light-Paws X) are fixed and on the console — new reports arrive as `bug`/`bad targeting` tags.

Lane A — async slot + record fidelity (worktrees/lanes/w57-A): D5 a SECOND async slot so the
  land-drop arm and the casting arm never displace each other (44 of 65 drops were a `Land
  drop:` ask displacing a cast ask; the Baka fetch-land resolution class from seat 123-130 E1
  — exclude an opponent's non-combat land arrival from the slot key's board half, or key on it
  separately), keeping D18's rescue; D4 `chosen_text` = the rendered row text byte-for-byte
  (the option-list entry, tail included) at every seam that writes it — the harvest's
  exact-match censuses undercount by 35% and a wrong docket number came from it twice; D16
  restore the broad `decision_reversed_in_prose` census so 0 conflicts != 0 counted; D31 a
  stderr companion for `hold_windows_skipped`. Predictions per the docket.
Lane B — combat surfaces (worktrees/lanes/w57-B): D6 `INCOMING THIS COMBAT` on EVERY window
  during the opponent's combat (25 of 260 today; creature-less seats never see it), computed
  once per combat and re-rendered; D24 print the ASSIGNABLE REMAINDER, not only the unblockable
  half; D22 lift the blocking-trigger TOTAL onto the BLOCKS header the way INCOMING is lifted;
  D10 de-nest the 10/36 residual B-lines. PARSETEST cases for every changed literal.
Lane C — cast-row and discard prices (worktrees/lanes/w57-C): D7 a creature cast row rendered
  while THEIR converter (Sanguine Bond class) is on the battlefield carries its price (the
  Emrakul-at-37-life game); D8 discard rows get a verdict on the 357/426 bare (the asymmetry
  is the item: which rows and why); D11 a `{right now:}` verdict counts the seat's OWN answer
  already on the stack (3 wasted Tributes); D30 two different cards with byte-identical
  verdicts at two prices get the cheaper marked (lane B's D15 shape); D21 a cycling row prices
  the cast it replaces and vice versa; D12 a menu-level `no live cast row` verdict when every
  cast row is dead (85 of deck123's 197 menus) — a header token, not a removed window.
Lane D — prompt economy (worktrees/lanes/w57-D): D14 the `Flip Side` display toggle must not
  reach the AI seat's option list (109 rows / 0 takes / ~72 KB; 760 option sets built and
  discarded — the `ishuman` lens inverted; the human keeps it); D29 collapse the 1,767 adjacent
  duplicate event-log lines (`repeated N times`, one line); D26 the back-face land row says the
  land will ask for 3 life; D27 `[NAMED BY THEIR <src>:` says per-cast vs one-off; D28 the row
  leads with the printed name, token second. D13 (narration budget by ask kind) is a PROPOSAL
  awaiting the owner (D43) — do NOT implement; measure and report the per-kind narration share.
Lane E — pregame, X, latches (worktrees/lanes/w57-E): D2 the pregame hand header counts MDFC
  back-face lands (lane-W.md has the plumbing + the tally-semantics question — decide it: count
  as a land AND keep the spell in "cheapest spell" with a one-line note carrying the
  arithmetic; sources[] gains the back face's colours; PARSETEST cases); D9 the mana-fit clause
  at ANNOUNCE_X (0/40 rows; the path is dead there); D15 the board frame carries the opponent's
  LIFE TREND (last 3 turns) so trend-keyed latches can fire; D19 the fetch row's colour
  parenthesis gets a positive form; D20 promote the `[<- ...]` marker off X menus to the other
  ranked-row surfaces the docket names; D23 `{feeds:}` counts converters in HAND too.
Lane F — engine hardening, no render change (worktrees/lanes/w57-F): D25 `MTGAbility::target`
  OWNERSHIP — clear the back-pointers when the garbage zone is deleted / evict dead abilities
  from `ActionLayer::mObjects`, and guard `a->source` in the same loop (lane C fixed the deref
  only); D34 a per-seat target-chooser owner (`observer->targetChooser` is global — lane Z's
  residual: the stall-watchdog release path can orphan a silent seat's chooser; lane Y's
  generalisation); D38 the phase-7 LIVELOCK (lane C's repro: the D17 stub command without a
  timeout — both seats `only display-toggle ... auto-passing` forever) — root-cause and fix,
  fixture-pinned; D36 the THIRD replay defect (a recorded mana click inside an interrupt window
  comes back `0<name>` refused) — replay indices before/after on lane E's four dumps.
Carried, not laned this wave: D17/D18 (need windows), D33, D35, D37, D39, D40, D41, D42/D43
(owner), D44 (Vita: vpk16 live).
Merge order preference: F, A, E, C, B, D (out of order fine). Done-files `W57_<lane>_DONE`.
