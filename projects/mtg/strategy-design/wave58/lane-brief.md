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

## Lane E — D42 board grouping: two owner-reported implementation bugs + a tokens-only setting
Worktree: worktrees/lanes/w58-E (base master 4581580f5). Feature under repair = lane G's wave-57
board grouping (`#W57-G (D42)` tags in GuiPlay.cpp, CardGui.cpp, CardSelector.cpp, ActionStack.cpp,
GameOptions.cpp/.h, GameStateOptions.cpp; `Options::BOARDGROUPING`, menu label "Stack identical
permanents", default ON for the human seat; `WAGIC_BOARDGROUP_PROBE=1` before/after instrument).
Read `projects/mtg/strategy-design/wave57/lane-G.md` first, then
`wave58/owner-decisions.md` (the owner's verbatim reports; on the Vita, vpk18, human seat vs
the heuristic AI, token decks).
Owner's instruction, verbatim: "well, fix the bugs. and.. give it a setting to apply only to
tokens. ill try it like that."
1. G1 — "it's stacking and unstacking the opponents land when they arent manipulating those
   lands." Find the actual mechanism (do not guess from the suspects list in owner-decisions.md —
   they are the orchestrator's guesses): what changes in the grouping key or the regroup trigger
   for lands nobody is touching. A likely class is transient state (tap during the AI's mana
   payment, a `castableNow`/focus/hover flag, summoning-sickness expiry, or regrouping on every
   render). A group must change only when a member's GROUPING state actually changes; prove it
   with a probe (compile-time gated) that logs every regroup with its cause, then show the
   cause is gone.
2. G2 — "the indicators are not remaining on top, so sometimes not visible, especially when
   targeting the stack." The stack-count indicator must render above every card of its pile
   and above the focused/targeted card's re-render (CardSelector re-renders the focused card
   after the row; wave-56's castableNow flag learned the same lesson — draw as part of the
   card's own Render at the right z-order, or after the selector's re-render, not as a
   row-time overlay). Verify in a windowed run if you can (WAYLAND_DISPLAY=wayland-1
   SDL_VIDEODRIVER=wayland SDL_AUDIODRIVER=dummy), otherwise by the render-order proof.
3. Setting — replace the boolean with a three-state option: Off / Tokens only / All permanents,
   DEFAULT = Tokens only (his choice for the next build). "Tokens only" = only token permanents
   (MTGCardInstance::isToken or the engine's equivalent — verify) are ever stacked; non-token
   permanents render exactly as with grouping Off. Keep the existing unique-state split (tapped,
   summoning sick, attached equipment, counters, etc. never stack together) in both modes. Keep
   the option persisting through the existing GameOptions save path; migrate a saved boolean 1
   to "Tokens only" and 0 to Off (say what you did).
Constraints: Vita/PSP builds compile this code — any Vita-only include under `#ifdef VITA`, C++14
only (no C++17 constructs); no PARSETEST cases needed unless you touch AIPlayerGPT strings;
suite must stay 1245/0 at THREADS=1 (or 2-known at 16) — run it. Human-facing: this is a
GUI change judged by the owner's eyes; ship a short `wave58/lane-E.md` telling him exactly what
to look for on the Vita for each of the three changes. Tag comments `#W58-E (D42)`.

## Lane F — F1 (SIGABRT: stale menu id indexes past ActionLayer::mObjects)
Worktree: worktrees/lanes/w58-F (base master a3f0700d3 or later). Read
`projects/mtg/strategy-design/wave58/engine-findings.md` §F1 in full — it has the backtrace, the
site (src/DecisionContract.cpp:367) and the stderr context. The core file is at
<wt>/projects/mtg/strategy-design/wave58/ab4-core (binary that produced it =
/home/magi/Projects/wagicGPT/archives/wagic-027db3b09-w57V; `gdb -batch -ex 'bt' -ex 'frame 5'
-ex 'info locals' <binary> <core>` works; do NOT commit the core). Establish the mechanism (why
the id exceeds the vector: which abilities were removed, and who built the menu) before fixing.
Fix EVERY site that maps a menu id to `mObjects` (grep `abilitiesMenu->mObjects` and
`->GetId()]` across src/), so a stale menu yields "no decision this tick" and never an index;
ship a suite fixture RED on base (a synthetic test-only card via WAGIC_TEST_PRIMITIVES_FILE is
acceptable if Nadaar + Lost Mine cannot be driven in a fixture; say which) and GREEN after. Tag
`#W58-F (F1)`. Suite must read 0 failed at THREADS=1; PARSETEST unchanged (3455/0) unless you
touch AIPlayerGPT strings.

## Lane G — F2 (engine hang after an AI menu answer of -1 on Sigarda, Champion of Light)
Worktree: worktrees/lanes/w58-G. Read `wave58/engine-findings.md` §F2 in full, then the evidence
files it names (read-only: the corpus directory must not be modified), then wave57/lane-S.md,
lane-T.md and lane-F.md (the softlock tooling and the orphan-chooser breaker). Establish the
mechanism: reproduce with a fixture (lane S's `aiseat` DSL, `WAGIC_STALLPROBE=1`, a synthetic
test card if Sigarda's Coven trigger cannot be driven directly) that hangs on base — bound every
probe run with `timeout 120` and the memory cap — then fix the ENGINE so the game always advances
(a -1/cancel on a may-menu must resolve the ability as declined and release every chooser it
owns), never by capping or removing the menu. Also answer: why did this menu produce no translog
record, and should it (a decision with no receipt is invisible to the corpus)? Fixture RED (hang
= timeout) on base, GREEN after; suite 0 failed at THREADS=1; PARSETEST unchanged unless you touch
AIPlayerGPT strings. Tag `#W58-G (F2)`.
