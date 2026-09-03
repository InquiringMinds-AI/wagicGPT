# Wave-54 step-1 lane brief (from wave53/engine-ledger.md — THE DOCKET; read your items there IN FULL)

Base: master 42b3bf5da (last engine commit ef133ea2a = lane Z merge; wave-53 corpus binary was
c628dd531, lanes U/V/W/X/Y/Z landed since). Baselines on ef133ea2a, hermetic `make -B`
(2026-09-02 21:16, ~/.gatelogs/w54-base-gate-*.log): PARSETEST 2258/0; suite 1219 with exactly 2
failed (lifeline.txt + merrow_reejerey.txt = the known concurrency-only pair, solo-green) + 49
AI/0, 0 timeouts (re-confirm on your worktree's first gate before you start; a THIRD failure or
any timeout is yours). Model for any live probe: qwen36-35b-a3b at http://100.116.136.74:8084
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
`until [ -f ~/.gatelogs/W54_<lane>_DONE ]; do sleep 10; done` loop (timeout 1200 s) with the run
launched as `systemd-run --user --unit=w54-<lane>-gate -p MemoryMax=4G -p MemorySwapMax=0
--working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY DISPLAY; ...; touch
~/.gatelogs/W54_<lane>_DONE'` (a backgrounded subshell dies with the Bash call); never a Monitor,
never a background task; never end your turn with the commit pending; COMMIT on your lane branch
in your worktree (no AI attribution of any kind in the message — no Co-Authored-By, no
Generated-with, no session link; `git add -f` for anything under bin/Res; never CLAUDE.md);
`cd` into your worktree inside every Bash call; ALL scratch/logs/done-files under ~/.gatelogs/
or <wt>/projects/mtg/strategy-design/wave54/ — NEVER in ~ or ~/Projects; build with
`make -f Makefile.sdl -j4` from <wt>/projects/mtg (qmake only with `-o Makefile.sdl` if you add
sources); suite from bin: `env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1
./wagic` (count BOTH `==Test Failed !==` and `==Test timed out`); PARSETEST:
`WAGIC_GPT_PARSETEST=1 ./wagic`; scoped runs via WAGIC_TESTSUITE_FILE=test/_probe.txt
(Res-relative). Tag every code comment for your change `#W54-<lane> (Dn)`. State a falsifiable
prediction per item (the docket has one); write `wave54/lane-<lane>.md` (what shipped, gate
numbers, predictions, what you did NOT verify). Never write mechanisms as the owner's positions.
Diagnostics you add are compile-time gated (`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`).

Lane A — priority seam + plan (worktrees/lanes/w54-A): D2 all four parts — (a) reserve the HOLD
  row's short name like `pass`; a reserved-0 vs HOLD-name conflict binds the NAMED row or
  re-asks, never a silent third thing; (b) exempt the HOLD row from the pass-verdict
  plan_choice_conflict test (a pass verdict CONFIRMS a hold; no other narrowing); (c) drop the
  hidden-zone counters (opponent hand size, both library counts) from `holdBoardKeyOf`
  (:12904) so the opponent's draw step no longer retires the hold — every other re-opener
  stays; (d) reword the row to the verb the model uses ("Pass priority, and do not ask me
  again this turn unless the board changes (any change re-opens this window; you give up no
  cast)"). D12: bound the served PLAN by SHAPE (first blank line / paragraph), not 1,600
  bytes, and add the one-clause diff against the CURRENT option list ("Master of the Feast" is
  no longer on your menu). PARSETEST for each shape, incl. the negatives the docket names.
Lane B — reply parser stamps (worktrees/lanes/w54-B): D13 note (better: re-ask) when the
  latched coded line's index AND parenthetical both differ from the executed row; D14 the
  ADDITIVE stamp when the chosen row's own annotation says no-op AND the PLAN argues against
  it (stamp only, no re-ask, no suppression); D15 echo matching against annotation-stripped
  row cores on EVERY tier (`stripRenderAnnotationsLc` :14669) so a name only inside `[finds
  only …]`/`{kills: …}`/`{right now: …}`/`[second copy: …]` can never bind; D9 `long_reply`
  stamp + elapsed fraction on any ANSWERED reply at >= 95% of WAGIC_GPT_TIMEOUT
  (`noAnswerClassFor` :8503 is the branch it misses; the deadline itself is the owner's dial).
Lane C — cast-row pricing (worktrees/lanes/w54-C): D4 three additive parts (player targets
  enumerated on `anytarget` cast rows; counted separately with the lethal tail `- and 3 to
  the opponent at life 1 WINS THE GAME`, else an honest `of the 2 CREATURE targets`; the
  player row in the target ask gets its own `{right now: …}` verdict) — `castKillSummaryTag`
  :13585; D10 the cost-form `[DRAW PRICE:` tail gets `- you would be at K` / `; this KILLS
  you` (the Forced Fruition form must stay 16/16); D11 `{kills: <name>}` / `{removes:
  <name>}` for unconditional removal from the row's own target list; D18 point the `{spends K
  of your M …}` computation at the MENU (`{taps you out - rows 3 and 4 on this menu become
  unaffordable}` or the positive form); D5 per-mode live/dead clause on modal cast rows AND
  the mode sub-menu (`{modes live right now: …; Dead: …}`), from what the engine already
  computes to build that sub-menu.
Lane D — chooser asks + collapse (worktrees/lanes/w54-D): D3 gate `buildHandRemovalAsk`
  framing on the ability's DESTINATION ZONE (`moveTo(mybattlefield)`/hand = GAIN ask, "pick
  the land you most need"), not on `handRemovalVerb`'s source-name match (:20084) — Path to
  Exile is the pin; D6 resolve `selfInflicted` (:20470) through `MTGCardInstance::
  storedSourceCard` when the chooser's source is the nameless granted dummy (the opponent's
  annihilator must print the `byOpponent` FORCED SACRIFICE branch); D8(a) extend
  `groupNumberedRows` (:4607) to ability, sacrifice and library-reveal menus (parser accepts
  any handle in the collapsed range); D8(b) ONLY after (a): a de-duplicated list of length 1
  whose members the handle map says are interchangeable resolves internally, logged like
  `mana_only_windows_skipped`; D25 LOW forced-sacrifice list sorted cheapest FIRST.
Lane E — combat/mulligan/land render (worktrees/lanes/w54-E): D21 gate `taking damage while
  ahead on LIFE` on `my_life > opp_life` (the corpus's only render FALSEHOOD — print nothing
  otherwise); D17 attackers header `They have N untapped creatures able to block; declaring
  more than N attackers leaves at least (your attackers - N) unblocked`; D16 mulligan coverage
  line appends its CAUSE (colour vs count); D20 fetch rows continue with the colours the
  target land makes (`and it adds {W} or {U} (you cannot make {U} right now)`); D19 stack
  effect label falling back to the source name prints the ability's menu text or `triggered
  ability`, never the name twice.
Lane F — observability + hang guard (worktrees/lanes/w54-F): D7 (a) a tick budget per reveal
  phase that force-closes the display and writes a record on expiry (lane X's latched
  phase-3 rebuild in `MTGRevealingCards`/`decideReveal` is the first half); (b) a fallback
  class on ANY engine-answered `-1` so it earns a `recovery` record (the `152v125` seq 28
  shape: `choice: -1`, no `fallback`, `latency_ms < 0`); (c) a harness-side NO-PROGRESS
  watchdog in the selfplay harness keyed on wall-clock since the last translog write per
  seat (report, never cap the game — invariant 00: stop dead loops and SAY so); D38 an
  `assertmtgid` suite command + lift the `playMode != MODE_TEST_SUITE` skip on flip so lane
  V's assembled `getOtherFaceCard` gets a pin.
Carried (not this wave unless the audit lanes take them): D22-D24, D26-D31 LOW; D32/D33 owner
items; D34 replay (RNG per AI seat), D35 kMaxOptSources, D36 `&&`-wrapped producers, D37
Dominating Vampire. D1 is discharged only by a 21/21 wave-54 corpus. Merge order after gates:
F, E, D, C, B, A (A touches the priority seam most broadly and merges last).
