# Wave-66 step-1 lane brief — KNOWN BUGS ONLY

Base: master 0ea7b971c (wave-65 close). Baselines on 0f5e0c7be, hermetic `make -B`: PARSETEST 4698/0, suite 1271
with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at WAGIC_TESTSUITE_THREADS=1; plus
intrepid_adversary_repeated_payment.txt, a genuine intermittent), 67 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-65 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave66/known-bugs.md` (item numbers below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-66
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
- Gates run DETACHED: `systemd-run --user --unit=w66-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w66-<lane>-*.log 2>&1; touch ~/.gatelogs/W66_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W66_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave66/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W66-<lane> (En)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave66/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W66-<lane> (En)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave64/codex-review.md and wave65/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave65/lane-AL.md .. lane-AP.md for the emitters you will touch, wave65/engine-seat.md for the
adjudication (wave 64 shipped three fixes that NEVER REACHED THE LIVE SEAM; wave 65 verified at the seam and 15 of 22 predictions PASSED: verify your fix at the seam the corpus
exercises, by a corpus-line PARSETEST case or an end-to-end fixture, never only at the helper), and wave61/corpus-livelock.md + wave62/corpus-hang.md + wave63/corpus-hang.md (three corpus-killing defects in three waves; waves 64 and 65 ran clean; a lane that
touches a shared choke point - a chooser, a key, a marker - must ask what ELSE reads it):
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

## Lane AQ — H1, H4, H9, H10 (forecasts and brackets that lie about the number)
H1 (engine HIGH-1; deck152 HIGH-2): `drawStepForecastText` (~`AIPlayerGPT.cpp:10931-10967`) charges the WHOLE draw step on
every window inside it — 123v162 seq 138-153 print the identical `resolving NOW ... = 6 life LOST BY YOU` for 16 windows
while life falls 7->3, and s152 (life 4, one point still owed) says `you would be at -2; that KILLS you` (seat lived at 3
and won); 152 s41-49 `draws 4 cards = 4 x 4 = 16` beside `ON THE STACK: 7 damage` (23 claimed, ~15 true; s42 cost 190 s
re-deriving it). Charge only the draws still ahead: count the draw events already narrated in the current step (the
narration has them) and subtract; the KILLS verdict rests on the remainder. PARSETEST both corpus shapes. H4 (deck125
HIGH-1): 125v162 s36 prints `ON THE STACK: 13 damage to you - you would be at -8; that would KILL you` AND `X=4 is the
largest listed X whose NET (-4) leaves you alive, at 1` — the X bracket starts from current life, not the post-stack life
the same prompt computed (5 of 8 stack-damage windows). One life-after-stack term feeds both. H9 (deck146 HIGH-1/2): the
mode ask (AL's modeRowAnnotations on the CHOOSE_MENU seam) prices half-dead PAIRS as whole modes and gives dead rows 1/4
no tag, contradicting the cast row's own live/dead census in the same game (146v162 s23->24, 146v126 s61->62; both took a
DEAD pair, s24 lost the game); its life forecast says "at 17" where Underworld Dreams printed in the same prompt makes it
16 — the cast row's DRAW GRANT bracket already folds punishers. One census function, both seams; fold the punisher term.
H10 (deck130 HIGH-3; deck162 HIGH-2): 130v123 s44 `board sweep: THEIRS 1 / YOURS 2 ... (it takes more of YOURS than of
THEIRS)` where THEIRS-1 was Bloodline Keeper under Intruder Alarm (cycled Starstorm, died at -78 to 40+ vampires);
162v126 s15 forced-sacrifice header "pays the least" pointed at Fate Unraveler while the same prompt listed it under
`DRAW PUNISHERS on the battlefield: yours`. Both brackets count bodies; they must name engines (token makers, draw
punishers, loop halves — detectors the prompt already runs) as not-a-body: `THEIRS 1 (Bloodline Keeper - a TOKEN ENGINE)`
and the sacrifice tie-break must exclude a named punisher/engine or say it is sacrificing one. Read wave65/lane-AL.md
(G1 seam, G4/G5), lane-AN.md (G6 totals), wave65/engine-seat.md §3 first.

## Lane AR — H2, H8 + MED (the reply rule's second half, and the re-ask that answers itself)
H2 (engine HIGH-2; deck130 HIGH-1; deck123 HIGH-1; deck126 HIGH-2): first-wins is KEPT (it removed 3 silent flips + 1 plan
mismatch at a price of 1 clearly-worse answer in 2209) but two halves are unfinished. (a) 72% of replies
(`post_answer_overrun` 1598/2209) put the coded line BEFORE the reasoning, so the first coded line is the un-reasoned
one: 130v162 s61 `CHOICE: 3 (Cast nothing)` then "Re-evaluating:" then `CHOICE: 1 (Cast Spark Spray)` (Ob Nixilis at 1
loyalty; ignored; lost 0-to-6). The protocol (`kReplyProtocol`) must demand the reasoning FIRST and the coded answer LAST
(one short reasoning block, then the coded line, then PLAN) — and the parser must tolerate both orders. (b) the
retraction-marker set accepts a marker only on the coded line or the line directly before it; 123v126 s36 wrote
"Correction: ... M (41) is already above stop (33)" two lines above `CHOICE: 0 (pass)` and 30 activations ran. Accept a
marker HEADER ("Correction:", "Re-evaluating:", "Actually,", "On reflection") within the 3 non-blank lines before the
later coded line when no other coded line intervenes; keep deliberation-only words ("wait", "hmm") excluded. (c) the
`later_answer_ignored` record trims everything past the PLAN (`[+2069 bytes ... trimmed]`, 130 s21), deleting the very
line the stamp names — record the ignored coded line verbatim in the record. H8 (deck126 HIGH-1): all 3 `named_row_reask`
in the corpus recovered to the exemplified row 1 verbatim; twice that row was `Tribute to Hunger {right now: they
control 0 creatures - at 0 this does nothing}` and s34 stamps `plan_contradicts_noop_row` then EXECUTES. `askExemplar`
(~`:15019`) must never exemplify a row `rowSaysNoOp` (~`:30714`) flags, and `plan_contradicts_noop_row` must re-ask once,
not execute. MED (engine MED-1): `name_over_index` still resolves a number/name disagreement silently outside the
reserved-row classes (130v125 s97/98) — every conflict routes through `index_name_conflict`. MED (deck126): the plan
carry truncates the correction and keeps the false premise (130 s21->22, 29/183 windows), and the staleness annotation
flags only names that left the MENU — a 15-turn "cast Idyllic Tutor" plan naming a card in NO zone went unflagged (125
s45->57); flag a carried plan naming a card in no visible zone. Read wave65/lane-AO.md, lane-AP.md (R2/R3), engine-seat.md
§3 HIGH-2 and §4 first; all three seams (CHOICE/ATTACK/BLOCKS) share `gptSelectAnswerIndex` — change it once.

## Lane AS — H3, H7 + MED (the loop that bills a round trip per point, and the hold that cannot hold)
H3 (deck123 HIGH-1/2; deck130 HIGH-2; deck162 HIGH-1; engine LOW-1): the `repeated N times, then stop` row is the only row
family printing no `{right now:}` verdict — 162 s73 prints M but not L, C, or usefulness; the seat wrote stop=20 in its
PLAN and took x33/x10/x21/x20/x47/x30/x200 in 10 of 13 windows (266 creatures vs an opponent on 15; 92% of that game's
decisions; 162v123 ended -1089). The row already DEMANDS a PLAN with stop/current/this-window and refuses without one:
read those numbers back and refuse the row (one re-ask, recorded `repeat_past_stop`) when stop - current <= 0, and print
`{right now: M=.., your stop=.., this window adds ..; past your stop = a wasted window}`. Doctrine: this is a re-ask on
the model's own stated stop, not a cap. Separately 130v126 s37-55: Sanguine Bond + Exquisite Blood billed 19 windows
(life 19->1) all answered "Cast nothing" — when `lifeLoopProvenWin` holds and the responder holds NO instant-speed row
(the oracle knows), resolve the chain without asking (auto-pass under the existing hold machinery, stamped
`loop_autopass`, narrated once as `the loop ran N times`); when it holds an instant, ask ONCE with the whole chain priced.
H7 (deck152 HIGH-3; deck123 MED; deck125 MED): the hold key carries volatile annotation numbers (`you would be at 4` ->
`at 3`) so a hold breaks every window — 152's loss game T15 ran 9 no-op windows / 334 s; deck-152 16 windows / ~630 s.
Strip numeric payload from the hold key (wave 63 normalised phase, not numbers), verify at the corpus lines. And the plain
decline row wins over hold 182:4 (123) and 213:29 (125): when the seat holds an instant-speed row, the hold row is the
default decline (listed first, `Cast nothing` second) — no row removed. MED (deck123): `index_name_conflict` re-asks on
UNIQUE names (162 s29/68/111, all 3 corpus-wide) — a unique name IS the answer; `no_pass_reask` because casting menus have
no row 0 while priority menus do (s45) — say so in the ask. Read wave65/lane-AM.md (G7 re-opener, mPassDeclineBoard),
lane-AL.md (G9 pass pricing), engine-seat.md §4 (G7 churn numbers) first; NOTHING volatile enters mPromptTail or the ask key.

## Lane AT — H5, H6 + MED (rows that omit a fact the engine already has)
H5 (deck125 HIGH-2): the cleanup-discard menu prints `{spare:}` on lands and then ORDERS the sweeper first — 125v126 s55
row 1 is the sweeper that destroys 7, the three `{spare:}` lands sit at rows 5/6/8, model took row 1; of 31 discard asks 12
offered a spare row and only 6 discarded spares only. The discard ask must lead with "discard the spare land" as its
default sentence and mark every non-spare row `{this is a real card: ..}` with the cast-row value it carries — order the
rows by disposability (spare lands first, then the cast-row ranking inverted); the engine's row indices stay stable
(map, don't renumber, or renumber consistently through the answer parser — verify with PARSETEST on the corpus ask).
H6 (deck152 HIGH-1; deck152 MED-1): `summoningSickTag()` (`:3553`) has one caller (`:7641`, the battlefield serializer);
target-choice rows carry `[doesn't untap ...]` but never `[summoning sick - cannot attack this turn]`: 152v162 s35-37 put
all three Luminarch counters on a Tracker cast that turn with the plan "attack with ... Tracker" (lost the only 152
game). Add the tag to target rows and to the attackers-window candidate list; the Aspirant self-target (123 s29) rides
the same row. MED (deck130): the converter clause gives no per-row auto-tap verdict though the row knows its tap set
(130v126 s32 "No safe play exists" with 6 castable cards) — print the tap plan the engine would use (`ManaEngine`
planPayment) on the row when it differs from the naive one; the mode menu drops the cast row's `{kills 0 of the 1
CREATURE target}` arithmetic (130v152 s77->78) — carry it. MED (deck146): land-drop rows price mana only — 146v125 s84/85
Hive declined twice at 2 life; a land with an activated creature/threat ability gets `{also a THREAT: ..}` from the
existing manland detector. MED (deck162): a flash punisher offered in the seat's own draw step shows no reservation
against the sorcery-speed punisher in hand (162v123 s11/14) — the `{reserve:}` clause exists for instants; extend to the
own-draw-step window. MED (engine MED-3): reveal windows stall the engine 835 s total (max 139 s) — measure where the time
goes (`reveal_wait_driver_*` records) and report; fix only if the cause is a wait the driver can skip. Read
wave65/lane-AL.md, wave64/lane-AJ.md (transient-clause hold), wave65/engine-seat.md §3 MED-3 first.
