# Wave-53 step-1 lane brief (from wave52/engine-ledger.md — THE DOCKET; read your items there IN FULL)

Base: master 3dfd77d65 (last engine commit c9dc6b75c; the wave-52 corpus binary was dddb6f62b,
no src change since). Baselines on that binary: PARSETEST 2121/0; suite 1210 with exactly 2
failed (lifeline.txt + merrow_reejerey.txt = the known concurrency-only pair, solo-green) + 44
AI/0, 0 timeouts (re-confirm on your worktree's first gate before you start; a THIRD failure or
any timeout is yours). Model for any live probe: qwen36-35b-a3b at http://100.116.136.74:8084
(auth: Authorization: Bearer $(ssh spark 'cat ~/.config/vllm/qwen35-api-key'); never print it).
Doctrine: enforce legality without constraining choice — NO hard caps on legal choices, NO
window removed, NO blind caches (D2's rejected-remedies paragraph is binding); every
parse/render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape of any
new bracketed/braced annotation); every engine change ships a suite pin (fixture RED on base,
GREEN after); byte-splice (python rb/wb) AIPlayerGPT.cpp, MTGRules.cpp, GameObserver.cpp
(CP1252+CRLF), primitives/*.txt — they hold non-UTF-8 bytes; verify
`git diff | /usr/bin/grep -c $'\357\277\275'` = 0; use /usr/bin/grep; MEMORY CAP on every wagic
run (`systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 --` prefix; detached
units get `-p MemoryMax=4G -p MemorySwapMax=0`) — a kill under the cap is a RED test pointing
at an allocation loop in your change; WAIT FOR GATES WITH A FOREGROUND
`until [ -f ~/.gatelogs/W53_<lane>_DONE ]; do sleep 10; done` loop (timeout 1200 s) with the run
launched as `systemd-run --user --unit=w53-<lane>-gate -p MemoryMax=4G -p MemorySwapMax=0
--working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY DISPLAY; ...; touch
~/.gatelogs/W53_<lane>_DONE'` (a backgrounded subshell dies with the Bash call); never a Monitor;
never end your turn with the commit pending; COMMIT on your lane branch in your worktree (no
AI attribution of any kind in the message; `git add -f` for anything under bin/Res; never
CLAUDE.md); `cd` into your worktree inside every Bash call; ALL scratch/logs/done-files under
~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave53/ — NEVER in ~ or ~/Projects; build with
`make -f Makefile.sdl -j6` from <wt>/projects/mtg (qmake only with `-o Makefile.sdl` if you add
sources); suite from bin: `env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1
./wagic` (count BOTH `==Test Failed !==` and `==Test timed out`); PARSETEST:
`WAGIC_GPT_PARSETEST=1 ./wagic`; scoped runs via WAGIC_TESTSUITE_FILE=test/_probe.txt
(Res-relative). State a falsifiable prediction per item (the docket has one); report what you
did NOT verify. Never write mechanisms as the owner's positions.

Lane M — blockers seam (worktrees/lanes/w53-M): D1 a `multiblock_reask` can end with ZERO
  blocks and no record — an illegal duplicate costs the DUPLICATE only (first legal pairing per
  blocker kept), the re-ask's record is always written (incl. empty result) and stderr prints
  `declared blocks from N assignment(s)` on every path. Also D23 LOW (BLOCKS format line carries
  the one-per-blocker constraint) and D19 LOW (blocker handle in the attacker slot: note it).
Lane N — priority-window economy + plan (worktrees/lanes/w53-N): D2 the model-takeable row
  `Hold priority - do not ask me again this turn unless the board changes` (engine honours it;
  re-opened on ANY board change, new stack object, or newly affordable row; the row is a
  CHOICE, no window removed) plus `[you declined this exact list N times already this turn]` on
  re-offered identical lists; D9 `CHOICE: n (Pass)` on a menu carrying `0. Pass priority`
  resolves to the pass row (short-label rule); D12 PLAN age stamp `(as you last stated it, N
  windows ago on turn T)`, the re-ask reply logged / latched line marked, core-prompt sentence
  that a board claim in PLAN is re-served verbatim.
Lane O — pricing annotations (worktrees/lanes/w53-O): D3 `[DRAW PRICE: ...]` gets lane K's
  `- you would be at K; this KILLS you` tail; D5 targeted-spell CAST rows carry the kill summary
  the target rows already compute (`{kills 0 of the 3 legal targets at -1/-1}` /
  `{kills: Name}`), damage and fight rows included; D6 `{paying this taps: <defender>}` says
  `- it cannot block on their turn` (per-source consequence when several sources differ);
  D13 fold the LIFE-TO-DAMAGE CONVERTER into `they gain N` rows (`- and their Sanguine Bond
  takes N off YOU: life 14 -> 10`) + the edict victim's `(P/T)`/`[defender]` at N=1.
Lane P — render/state (worktrees/lanes/w53-P): D4 stack lines name source + victim
  (`ability: Soul Shatter's sacrifice (aimed at Emrakul, the Aeons Torn) [from your Soul
  Shatter]`) and edict `{right now:}` clauses add `- a sacrifice is already on the stack aimed
  at this permanent; this one would find their next-highest`; D7 `#a-#b`/`xN` collapse on
  ABILITY target menus (parser accepts any handle); D8 stamp the `canplayfromexile` cause on
  the card so `{castable from exile}` keeps naming its source after the granter leaves; D11
  `[second copy:` appends the usefulness verdict when the script has no stacking term (test is
  the SCRIPT — Intruder Alarm dead, Talisman/Lantern not; partial-redundancy form for Lantern);
  D14 battlefield header counts lands too.
Lane Q — observability (worktrees/lanes/w53-Q): D10 a 900 s non-answer is stamped
  `fallback: timeout` (distinct from `empty_reply`), stderr prints `AIPlayerGPT: no reply after
  Ns - heuristic`, ONE retry before the heuristic handoff; D24 LOW `unparsed_reply` writes a
  recovery record; D20 LOW `(xN)` collapse reaches draws/loyalty/discards if cheap. The
  WAGIC_GPT_TIMEOUT number itself is the owner's — do not change defaults.
Not this wave: D15-D18, D21-D22, D25-D26 carried; D27 log-window measurement (owner decision);
D28 frontend (owner item). Merge order after gates: M, Q, O, P, N (N touches the priority seam
most broadly and merges last).
