# Wave-60 step-1 lane brief — KNOWN BUGS ONLY

Base: master 8b922abf0 (wave-59 close). Baselines on 5039427da, hermetic `make -B`: PARSETEST 3529/0, suite 1247,
suite 1245 with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at
WAGIC_TESTSUITE_THREADS=1), 61 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-59 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave60/known-bugs.md` (item numbers below). The
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
- Gates run DETACHED: `systemd-run --user --unit=w60-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w60-<lane>-*.log 2>&1; touch ~/.gatelogs/W60_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W60_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave60/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W60-<lane> (Bn)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave60/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-130044/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W60-<lane> (Bn)`.

## Lane L — B1, B2, B11 (lethality the render withholds or lies about)
Worktree: worktrees/lanes/w60-L. B1: the `THIS WINS THE GAME` fold must subtract the cast's own life cost
and any damage/loss already pending on the stack before it claims a win — a row that kills the pilot
first says so and never says WIN (130v146 s43/44: `this KILLS you` and `WINS THE GAME` in one brace;
the target menu printed only the win). Locate the fold. B2: damage pending ON THE STACK against the
pilot is totalled and printed at every decision where it exists (`ON THE STACK: N damage to you - you
would be at K`), and TARGET CHOICE windows render the stack (now 11/89); a triggered ability row names
its effect, never bare "triggered ability" (146v162 s26). B11: the ATTACKERS ask gets the aggregate
line the blockers ask has (total power, damage if unblocked, their resulting life, and the crack-back
that already renders) — computed, not a new claim.

## Lane M — B4, B3, B13 (protocol economy and transport budget)
Worktree: worktrees/lanes/w60-M. B4: lane H's connect timeout = deadline means `curl=28` burns 900 s
and a retry finished at deadline_pct 108.6 — the connect timeout becomes a bounded fraction (a named
constant; tens of seconds), the retry budget is strictly the remaining deadline, and `transport_error`
must actually fire on a connect failure (wave 59: 0 of 2). B3: a cleanup `PUT:` list with a repeated
index is not rejected whole — dedupe and accept when the distinct set is complete, else ONE re-ask
whose notice says indices must be distinct; the ask text states it. B13: (a) the carried `YOUR PLAN`
echo is bounded to the plan's first paragraph / N chars with a stated truncation marker (148/410 >
400 chars; a 1,236-char stale stream produced an off-menu CHOICE) and is dropped when it names a
board fact the CURRENT SITUATION contradicts (say how you detect that, or scope to the bound only);
(b) identical consecutive narration events collapse to one line with a count (`Opponent drew a card
(x5)`), append-only semantics preserved; (c) the declined-list counter counts only asks the model
SAW, never K10 re-serves. PARSETEST for each.

## Lane N — B5, B6 (punisher lenses)
Worktree: worktrees/lanes/w60-N. B5: add a DISCARD-punisher lens beside the draw one — `converterScanZone`
(AIPlayerGPT.cpp ~8566) must not merge `@discarded` triggers into the draw-converter count (PARSETEST
43631 bakes the wrong pairing in: fix the case, do not delete it); the X-row / cast-row NET life
clause and the cleanup-discard ask price a forced discard under a discard punisher (125v162 s111: 44
life unpriced). B6: the draw-punisher forecast (`DRAW PUNISHERS` / `DRAW FORECAST`) accounts for the
OPPONENT's Sanguine Bond + Exquisite Blood loop turning the pilot's own punishers against it (126-game
s20), and the converter warning fires when one half is on the battlefield and the other is a SEEN card
(hand/graveyard/revealed), not one decision after the loop closes (152 s9-33).

## Lane O — B7, B8, B10, B12 (rows that hide what they do)
Worktree: worktrees/lanes/w60-O. B7: sweeper rows name the creatures they destroy, as Path rows do
(`{removes: A, B}`), within the existing count clause. B8: Teferi's +1 (and any ability whose text is
truncated before its verbs) renders its full operative sentence, and its tap/untap sub-menus say which
branch taps what and that untap-step timing applies. B10: a forced-sacrifice choice (Tribute to Hunger)
is headed as what it is and each row says `you sacrifice this; they gain N (its toughness)`. B12: the
venture ask names the venturing SOURCE and, when the source's own text conditions on a dungeon by name
(Acererak), says so (11/11 asks unnamed, two waves).

## Lane P — B9, B14 (engine state the render cannot see)
Worktree: worktrees/lanes/w60-P. B9: a creature exiled "until this leaves the battlefield" must be visible
at the blockers/attackers decision that would return it — render `[returns X (4/4) if this dies]` on
the exiler's row and count it in the crack-back. B14: wave-59 known-bugs 12/13 — after a DECLINE on an
interactive reveal's option one, option two never arms (nested target ability stays
currentWaitingAction; address reuse), and the double-consumer at revealasyncticks 2 takes "Get a
human" after a decline; read wave58/lane-G.md's four failed attempts first. Fixtures RED on base.

## Lane Q — the nine findings of wave60/codex-review.md (fix or refute, each with evidence)
Worktree: worktrees/lanes/w60-Q (base = master after the L-P merge, 1fcdeeedc + the review commit).
Read wave60/codex-review.md in full, then the lane report each finding touches (lane-L.md for #1/#4,
lane-P.md for #2/#7, lane-N.md for #3, lane-O.md for #5/#6, lane-M.md for #8/#9). For EACH finding:
first VERIFY it against the code (a reviewer can be wrong — say REFUTED with the line that proves it,
or CONFIRMED with a reproduction: a PARSETEST case or fixture that fails on the current tree); then
fix every CONFIRMED one in the general form. Priorities: #2 (a `drivingFor` source-only early return
that can withhold a legal target chooser for the stall wall — the wave-58 hang class; the gate must
prove the chooser BELONGS to the live reveal driver, or be removed; fixture: a second targeted
ability from the reveal's source while the reveal display is live) and #1 (first-match `break` in
`namedCastLifeSurcharge` and the cast-row accumulation — sum ALL naming punishers) are HIGH and block
the ship. #3: the X-row cleanup price is a CEILING that assumes no cards are cast before cleanup —
either word it as such (`up to`) and fix the PARSETEST expectation, or compute the floor; never a
guaranteed number. #4: ATTACK TOTAL must not state life for infect / double strike / prevention /
can't-deal-damage — exclude or label those attackers (D6's under-claim rule). #5: tap/untap tag
must read CANBLOCKTAPPED and does-not-untap state, not card type. #6: sweeper roster excludes
indestructible (and regenerate-able as "may survive"). #7: exile-hostage P/T from the returning
object's printed values, not the exile copy's counters. #8: `planCarryBound`'s marker must survive
the trim pass (compose the two; PARSETEST the composed path). #9: a 200 with an unparsable/empty-
schema body classes as `bad_reply` (or similar), never `empty_reply`; translog carries the HTTP
status. Gate as usual (suite THREADS=1 0 failed, AI count, PARSETEST 0 failed, RED evidence per
confirmed finding). Tag `#W60-Q (Rn)` with the review's finding number. Write wave60/lane-Q.md with a
CONFIRMED/REFUTED table.
