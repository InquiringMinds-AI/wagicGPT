# Wave-65 step-1 lane brief — KNOWN BUGS ONLY

Base: master 537070ac2 (wave-64 close). Baselines on dab6a6cba, hermetic `make -B`: PARSETEST 4536/0, suite 1269
with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at WAGIC_TESTSUITE_THREADS=1; plus
intrepid_adversary_repeated_payment.txt, a genuine intermittent), 67 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-64 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave65/known-bugs.md` (item numbers below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-65
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
- Gates run DETACHED: `systemd-run --user --unit=w65-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w65-<lane>-*.log 2>&1; touch ~/.gatelogs/W65_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W65_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave65/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W65-<lane> (En)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave65/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-001533/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W65-<lane> (En)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave63/codex-review.md and wave64/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave64/lane-AG.md .. lane-AK.md for the emitters you will touch, wave64/engine-seat.md for the
adjudication (three wave-64 fixes NEVER REACHED THE LIVE SEAM - F6, F12, F4-damage: verify your fix at the seam the corpus
exercises, by a corpus-line PARSETEST case or an end-to-end fixture, never only at the helper), and wave61/corpus-livelock.md + wave62/corpus-hang.md + wave63/corpus-hang.md (three corpus-killing defects in three waves; wave 64 ran clean; a lane that
touches a shared choke point - a chooser, a key, a marker - must ask what ELSE reads it):
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

## Lane AL — G1, G4, G5, G9 (prices that never reached the seam, or lied about the floor)
G1 (engine HIGH-1; deck146 HIGH-3): F6 NEVER FIRED — 3 CHOOSE_MODE windows (Silverquill Command), 23 rows, 0 `{this mode
right now:` and 0 wave-60 liveness/tap tags: the `ctx`-gated family at `AIPlayerGPT.cpp:33348-33394` is silent while the
header names the card from `req.contextName`. 146v130 seq 57/74: `you draw and sacrifice creature` (`life:-1 controller`,
borderline.txt:102747) taken at 5 and at 3 life, lost at 0. Find why `ctx` is empty on the `Choose an option for <card>`
ask (the request builder? the seam that calls the renderer?), price every mode row there, PARSETEST the composed ask with
the corpus rows verbatim. G4 (engine HIGH-4; deck123 HIGH-3; deck162 HIGH-2): 123v152 seq 9 `{crack-back cover:}` said "at
2, which you SURVIVE ... nothing uncounted here overturns it" off a number the same prompt called a FLOOR with a named
growth trigger (seq 10: 17 damage, best block 0, died at -8); 162 vs146 seq 12: "up to 7 - you would be at 2 or lower"
beside two named 3-power Hives (13 vs 9), and the cover gate keyed on that floor's lethality, so two castable 5/5 bodies
got no cover. No SURVIVE verdict on a floor; the cover clause gates on the ceiling and folds named manlands/growth
triggers or says it cannot. G5 (engine HIGH-5): 32 of 191 windows tell the seat its OWN draw step costs it life under
`DRAW PUNISHERS: yours` (162v125 seq 14) — wrong chair on the direction split; 159 correct. G9 (deck125 HIGH-1): the
pass row is the only unpriced row under a punisher — 162 seq 72/73 (T17, 12 life, hand 10, 7 open mana, Final
Judgment + Path castable) `Cast nothing` twice, cleanup billed 6 of 12 life, dead at seq 97; 18/18 punisher windows
unpriced, 18/18 passed. Price the pass row with the cleanup discard/loss the engine already computes on the X row and at
the cleanup ask. Read wave64/lane-AG.md (F6), lane-AH.md (F10, F11), lane-AK.md (R4/R7), wave64/engine-seat.md first.

## Lane AM — G2, G3, G7 (engine: an attack that deals nothing, an actor still inverted, a hard cap)
G2 (engine HIGH-2): `A#>W#` deals no damage — 123v126 seq 25: an unblockable 5/5 declared at Sorin (4 loyalty); seq
26->27 Attackers -> Main 2 with no combat-damage event; Sorin still 4 loyalty at seq 32, opponent life unchanged; 2 of 2
walker attacks delivered 0. Lane AI routed the apply through `MTGPlaneswalkerAttackRule` + its menu; the binding is not
taking. Fixture the engine path END TO END (declare A1>W1 -> combat damage -> loyalty counters removed), RED on base, and
make the W-row section say a planeswalker cannot block (deck152 LOW). G3 (engine HIGH-3): F12 UNCHANGED — 126v130 seq 16
prints lane AI's own falsifier verbatim (`You cast Tribute to Hunger` ... `Opponent used:` ... `You gained 1 life`); 10
lines / 5 events / 3 games. `e->source->controller()` on a `targetedplayer` dummy is the grantee; lane AI fixed a
different seam. Find the seam that emits `used:` for granted abilities (the narration writer that handles
WEventCardActivated/ability resolve for `targetedplayer` payloads), credit the card's controller, PARSETEST the corpus
line verbatim. G7 (deck123 HIGH-2; DOCTRINE): `AIPlayerGPT.cpp` ~28900 `declineCap=2`, no re-open — 162 seq 66/69 -> 73
retired the free `{T}` token-maker rows for the turn; when Intruder Alarm resolved in main 1 the menu held only three
equips and the combo could not fire on the turn it assembled. A hard cap on legal choices is forbidden: re-open on any
board change (the board key already exists), or drop the cap and rely on the declined-this-list tag. Read
wave64/lane-AI.md (F4, F12), lane-AK.md (R1/R2), wave62/lane-Z.md (the declined-list tag) first.

## Lane AN — G6, G10 + the forced-sacrifice tail (verdicts that contradict the same screen)
G6 (deck123 HIGH-1; deck146 HIGH-1; deck162 HIGH-1): 123v126 seq 48 — with 51 creatures the screen printed `ATTACK
TOTAL ... KILLS them whatever they block` AND stamped every unblockable row with the Sanguine Bond/Exquisite Blood "fatal
to you" clause; `ATTACK: none`, lost 43-0. 146 vs126 seq 29: the clause on all 3 attacker rows against a CREATURELESS
opponent where the same prompt says every attacker is unblockable (neither loop branch reachable: mtg.txt:37967/:99004).
162 vs126 seq 13: `{their converter:}` row priced a body `life 21 -> 17` six lines under the LOOP SCOPE paragraph "ANY
nonzero payment on a tag above is fatal" — the seat cast it and died from 21. Rule: the loop clause yields to a proven
lethal verdict on the same screen (say "lethal FIRST - the loop cannot fire after they are dead" or withdraw the clause);
it prints only where a loop branch is reachable (a creature that can be dealt with / a payment that can happen); the
per-row converter arithmetic consults the loop detector and prints the loop verdict, not a life figure. G10 (deck152
HIGH-1; deck126 HIGH-1; deck146 MED): 152v146 seq 13/27 "best case ... no better material reaches that life figure"
while the next line's `GANG BLOCK:` reaches the same life with one blocker and kills the attacker (0 of 10 suggestions
ever name two blockers on one attacker) — the suggestion search must include gang assignments or the claim must say
"among lone blocks"; 126v152 seq 9 (after blockers declared): the committed Perimeter Captain counted as still free,
damage subtracted twice, named assignment unreachable (`AIPlayerGPT.cpp:21480` uses `canBlock()`, no already-assigned
test) — exclude assigned blockers and fold their committed block; 152 s12: best-case life ignores the same block's
lifelink gift — fold it. MED: `buildForcedSacrificeAsk` (`AIPlayerGPT.cpp:25227`) appends "the one that pays the least"
to the gain==2 branch where the payment is life YOU gain (126 vs123 seq 10) — inverted; say "gains you the most" there.
Read wave64/lane-AG.md (F8/F9), wave63/lane-AB.md (E1/E3/E4), wave62/lane-X.md (D2/D8) first.

## Lane AO — G8 + the MED list (the reply rule, the plan carry, the small rows)
G8 (deck146 HIGH-2; deck130 HIGH-1/2; deck123 MED): 146 vs130 s93/s101, vs162 s39 — model wrote `CHOICE: 2 (Cast
nothing)`, engine executed row 1 (`answer_replaced:true`, `latched_line_offset:6544` vs `plan_block_end:735`,
`latched_line_in_plan:false`), 3 of 3 replacements flipped the row, two on a final turn; 130 vs146 seq 47 — a two-block
reply takes its answer from block 2 but its PLAN from block 1, so discarded reasoning becomes YOUR PLAN for the next
windows (11/424); 130 vs162 seq 57 — `CHOICE: 0` on both lines, record `choice: 4`: number and name disagree and the name
wins silently. Write the rule ONCE and make every seam obey it: the first coded line is the answer unless a LATER
column-0 coded line explicitly corrects it (the wave-64 R5 shape); a number/name conflict on the taken line is a
RE-ASK, never a silent pick; the PLAN is harvested from the SAME block the answer came from; every override writes
`plan_answer_line_ignored` or `answer_replaced` with both offsets. PARSETEST all three corpus replies verbatim. MED (each
with the seat's seq): the PLAN carry cap is 400 chars (`kPlanCarryMaxChars`, AIPlayerGPT.cpp:395) and never stated —
state it in the appended reply protocol (AIPlayerGPT.cpp:233) and cut at a sentence boundary (126 seq 41 dropped 5,522 B
containing "I win"; 146 seq 24 truncation inverted a self-correcting sentence; wave64/synthesis-notes.md routes this
here); the valor-counter ask prints 20 `add N counters` rows the prompt calls no-ops (152v146 s7/s12, 152v162 s7/s11 —
clamp to affordable or skip the ask); Elite Spellbinder's target window is bare — no tax rider, no decline though the
Oracle says "may" (152v123 s11); Kaya `-3` still offers Kaya as her own target (deck146); `Choose a card NAME` ranks the
token name `Goblin` first by visibility (130 s27); `{text:}` dropped from some reveal rows (152 s22); `{feeds:}` never
says a symmetric engine also draws YOU (41/41) and `draw converters in your hand: N` counts uncastable cards (8/8) (deck162);
the plain `Cast nothing` row should withdraw once the declined-this-list tag fires (deck125, 58% no-ops). Read
wave64/lane-AJ.md (F13), lane-AK.md (R5), wave63/lane-AD.md (E6) first. Anything you print that varies per rebuild stays
out of both keys (wave61/corpus-livelock.md).
