# Wave-58 step-1 lane brief — KNOWN BUGS ONLY

Base: master 6976b839a (wave-57 close). Baselines on it, hermetic `make -B`: PARSETEST 3350/0,
suite 1245 with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at
WAGIC_TESTSUITE_THREADS=1), 61 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-57 findings; read each in FULL in
`projects/mtg/strategy-design/wave57/engine-ledger.md` (the `## Dn` sections named below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-58
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
- Gates run DETACHED: `systemd-run --user --unit=w58-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w58-<lane>-*.log 2>&1; touch ~/.gatelogs/W58_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W58_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave58/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W58-<lane> (Dn)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave58/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

## Lane A — D3 (rowSaysNoOp verb whitelist; `exiles 0` badges live)
Worktree: worktrees/lanes/w58-A. Site: `AIPlayerGPT::rowSaysNoOp` (AIPlayerGPT.cpp ~19862) and its
callers (~11561, ~18320 header gate, ~31261 dead-badge). Minimum: the badge/header decision must
recognise every computed-zero `{right now: <verb> 0 ...}` form the renderer can emit (exiles,
and any other verb the sweeper/edict/damage emitters produce — enumerate them from the emitters,
e.g. `sweeperClause`, not from memory); prefer a match on the computed magnitude over a string
whitelist where the call site has the number. The discard-menu half of D3 (6 of 455 discard rows
missing the cast evaluator's verdict) is IN SCOPE if the route is a contained change; otherwise
record it as not done. PARSETEST: one positive per verb form, one negative per verb with a real
magnitude (`exiles 2`), and the existing D14 cases must still pass.

## Lane B — D1, D5, D7 (verdicts and numbers that are wrong in the lethal direction)
Worktree: worktrees/lanes/w58-B. D1: the pay-3-life MDFC arrival row (`... enters UNTAPPED` at a
life cost) carries no lethality verdict; a seat at life 1 paid and died. Add the same
survival/lethality verdict the cast rows carry (`gets you to N` / would kill you) on every
life-payment menu row, and make sure a row that kills the pilot is badged as such — never
removed (legal choice stands). D5: `[<- best trade: the most of THEIRS at the smallest cost to
YOURS]` (~9374) marked a 1-for-5; the marker must not endorse a trade where the cost to YOURS
exceeds what it takes of THEIRS unless it is the only kill — fix the selection rule the marker
prints from, and its sibling `[<- best X for this cast: ...]` (~9103) inherits the fix. D7:
`assignableRemainderDamage` (~15018, called ~15624) drops trample from its own baseline and
printed `one legal assignment gets you to 1` on a lethal board; the bound must be a true floor
(pessimistic in the lethal direction). PARSETEST cases for each rendered string; a fixture for
D1 if the menu row is reachable in a suite game (say so if it is not).

## Lane C — D2, D4, D9 (false header, missing drift receipt, INCOMING gated on phase)
Worktree: worktrees/lanes/w58-C. D2: `NO LIVE CAST ROW ON THIS MENU: all N cast rows ...` (~18337)
is false on 8 of 497 renders — a live row (Devour Flesh) sat under it; find why the gate at
~18320 disagrees with the rows it summarises (the D3 whitelist is one cause; lane A owns that —
your job is the header's own consistency: it must be computed from the SAME per-row verdict the
rows print, so it can never contradict a row). D4: `asyncSlotDriftKind` (~10305, used ~10547)
renders 0 times in 21 stderr logs though 108 drops occurred — make every stale-drop line carry
the drift kind and an outcome token (compile-time-gated diagnostics are fine for the stderr
line; the translog stamp is not diagnostics and ships unconditionally). D9: `INCOMING THIS
COMBAT` is gated on combat phases so a seat at 3 life facing 7 power at Main 1 sees no number —
render the incoming-power fact whenever the opponent has untapped potential attackers and it is
the pilot's own turn-adjacent decision (read D9 for the exact gating and keep the trample
under-claim rule from D6/wave 56). PARSETEST for each string.

## Lane D — D45 (lowercased script tokens in narration, 698 occurrences / 422 prompts)
Worktree: worktrees/lanes/w58-D. Read D45 AND D28's narration half in the ledger: the MDFC/back-face
token reaches the narration lowercased at the write seam (the row head was fixed in wave 57;
`chosen_text` is clean; narration is not). Fix at the write seam so the narrated name is the
card's display name; cover every narration emitter that writes a script token (ARRIVAL,
`resolved and entered`, `created ->`, cast lines). PARSETEST: the echo shape with a mixed-case
MDFC name, and a negative that a genuinely lowercase card word is untouched. If D45 lists other
LOW items in its cluster, take only the narration-case one.
