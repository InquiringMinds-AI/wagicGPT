# Wave-69 step-1 lane brief — KNOWN BUGS ONLY

Base: master d8772a91c (wave-68 close). Baselines on 9be85773b, hermetic `make -B`: PARSETEST 5272/0, suite 1277
with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at WAGIC_TESTSUITE_THREADS=1; plus
intrepid_adversary_repeated_payment.txt, a genuine intermittent), 67 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-68 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave69/known-bugs.md` (item numbers below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-69
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
- Gates run DETACHED: `systemd-run --user --unit=w69-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w69-<lane>-*.log 2>&1; touch ~/.gatelogs/W69_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W69_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave69/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W69-<lane> (En)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave69/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W69-<lane> (En)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave67/codex-review.md and wave68/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave68/lane-BA.md .. lane-BE.md for the emitters you will touch, wave68/engine-seat.md for the
adjudication (wave 68: a merge comment at column 379 swallowed 30 ctor initializers and compiled clean — the guard `tools/check-ctor-init.py` now runs at link; lane BA sized caps on the previous corpus and the guide rewrite moved the distribution the same wave — size from THIS corpus and say what would move it; three wave-67 lane claims were FALSIFIED by the corpus — AW "row and header cannot differ", AX "zero retry budget", AY/AZ own-stop reachable — each was argued from code reading where a corpus count was available: COUNT before you claim; wave 66 shipped a PROTOCOL change whose composition cost — the model transcribing section names as labels — no PARSETEST case could see: a change to what the MODEL is told must be measured against the corpus replies verbatim, both the intended shape and the shapes the wording invites; verify your fix at the seam the corpus
exercises, by a corpus-line PARSETEST case or an end-to-end fixture, never only at the helper), and wave61/corpus-livelock.md + wave62/corpus-hang.md + wave63/corpus-hang.md (three corpus-killing defects in three waves; waves 64-68 ran clean; a lane that
touches a shared choke point - a chooser, a key, a marker - must ask what ELSE reads it):
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

Also binding this wave: NEVER chain a build behind a counting grep (`grep -c` printing 0 exits 1 and skips an `&&`-chained
make — lane AT gated a RED binary that way); freshness-gate the binary (`ls -la bin/wagic` after every make).

The guides are now ~20 KB decision-time-only documents (owner ruling 2026-09-06, Amendment 332): a render change that a
guide keys on must keep its printed string stable or say so in the lane report (the wave-68 seats found the losses were
rules keyed to literals that had stopped printing).

## Lane BF — K2, K3 (the cap that ate the answer, the reversal nobody sees)
K2 (engine HIGH-1/2/3; deck123 HIGH-2; deck152 MED; BE-R3 FAIL): MEASURE FIRST on this corpus — per seam, needed bytes
(first byte -> end of PLAN), where each cap bit, and what was cut (a correction? the PLAN? the label?); then: the
truncation re-ask at EVERY seam (discard/reveal/bottom/attackers/blockers — 125v146 s133 discard cap 384 -> heuristic
discarded Emrakul; 123v162 s19 combat truncated with no stamp); a recovery whose executed answer differs from the
truncated reply's coded line is stamped `reply_truncated_answer_changed`, never "recovered" (123v125 s23-25); every cap
re-sized from THIS corpus's distribution with headroom for one correction line; and 123v162 s34 — `CHOICE: 5 (Cast
Damnation)` then "You should NOT cast Damnation. You should attack." CUT by the cap (`post_answer_overrun 2253`), the
deciding decision of the corpus — the protocol says a correction goes BEFORE the PLAN line, and the parser scans the
truncated text for a reversal before executing. Do not credit the caps with the -46% (engine seat: ~10%); keep them as
the variance guarantee they are. K3 (deck123 HIGH-2; deck162 MED-3; deck152 LOW; BA-8 FAIL): `decision_reversed_in_prose`
fired 44 times (6.3x wave 67, 25 `plan_choice_conflict`) mostly on deliberation — 125 s19 a 2,276-char PLAN ending "I will
pass" after a correct CHOICE; 125 s47/48 and 130's 5/5 the re-ask returned the identical choice — while missing s34.
The predicate reads only the sentences between the coded line and `PLAN:` (the correction zone), never the PLAN body; a
reversal NAMES the alternative (a row, pass, "do nothing"); a re-ask that returns the same answer is recorded
(`reversal_reask_same_answer`) and not re-asked again in that window. PARSETEST every corpus reply named here verbatim.
Read wave68/lane-BA.md, lane-BE.md (R3/R4), wave68/engine-seat.md §3 HIGH-1..3 + §4 (9) first.

## Lane BG — K1, K5, K8 (engine: a game that ended for no reason, rows that do nothing, a tag that lies)
K1 (deck123 HIGH-1) FIRST: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/game-146v123-1788720096.stderr:1952`
`WAGIC_SELFPLAY_RESULT winner=0 life0=19 life1=11 turn=17`, libraries 42/43 — no loss condition. Sequence (stderr
1925-1952): deck123's Idyllic Tutor `reveal put 1 of 42`, `index_name_conflict` re-ask, a drifted in-flight answer
consumed, `StackAbility (Source: Idyllic Tutor)` left on the stack; deck146 casts Vision Skeins, two `10DrawAction`s
resolve, then both seats log game end. Root-cause by REPLAY (the seat translogs + stderr; `WAGIC_REPLAY_NOCONSUME=1` for
synthetic lines; gdb SYMBOL breakpoints on the loss setters — `Player::loseLife`/`setLost`/`drawFromLibrary`'s deck-out
branch, `GameObserver::gameOver` — line breakpoints do not bind in this codebase): which loss check fired and why (the
reveal's returned-cards path? the Tutor's search leaving the library in a state `drawFromLibrary` reads as empty? BE
R7's CANT* fold inverted?). Fixture it RED on base. Related: Idyllic Tutor never decrements the printed library after
the reveal (deck123 MED; 125: 126 s111-114). K5 (deck146 HIGH-1/MED; deck125 MED): `becomes beholder` rows tagged
`[this land is TAPPED: animated, it still cannot attack this turn]` taken 2/2 (146: 162 s22 at 1 life; 125 s243) — a
row yielding a body that can neither attack nor block this turn is priced `{right now: does nothing this turn}` and
joins the cast row's dead census; cleanup rows' `{you already control one: X}` / `{dead right now: ...}` ignored 4/4 while
the cast menu's `[legendary: you already control` mark is obeyed — ONE wording at both seams (the obeyed one); an
activation row offered at `Mana available: 0 total` (125: 126 s111/113) — say it cannot be paid or do not offer. K8
(deck126 MED): `[second copy: adds nothing new]` is FALSE for stacking triggers — a second Sanguine Bond doubles the
drain (mtg.txt:99004), a second Staff of Nin pings again (mtg.txt:112425); the tag reads the SCRIPT: a legendary or a
one-instance static earns the tag, a triggered/static effect that stacks reads `[second copy: one more of the same
effect]`. Read wave68/deck123/review.md HIGH-1, wave68/lane-BE.md (R7), wave65/lane-AM.md (engine fixtures) first.

## Lane BH — K4, K6 (numbers still wrong on the two surfaces the seats trust)
K4 (deck125 HIGH; BB claimed this closed): 126 s110 — Staff of Nin draws owed, the X ceiling read one too high, deck-out
at 65 life. COUNT on the corpus every X window with an `@each my upkeep ... draw` permanent on the seat's battlefield and
compare the printed ceiling to the true remainder; find which reserve term BB's fold still misses (draw-step extras?
`@each` permanents? both Staffs counted once?) and PARSETEST the s110 numbers. `Life with Elixir` prints no `{right now:}`
verdict (412 renders; 126 s60/s111) — every activation row gets the verdict its neighbours get. `plan_contradicts_noop_
row_reask` fires only off PLAN text — 14 K=0 sweeper takes (`destroys 0`) it could catch off the ROW's own zero (130 s20
shows the shape working): a take of a row whose own clause says zero earns the one re-ask regardless of the PLAN. K6
(deck162 MED-1/2; engine MED; deck152 MED): `{crack-back cover:}` projects a life total counting only the row's OWN new
body — 162v152 s15 "you would be at 4" with two untapped blockers already in play, s19 "at 0": fold the seat's existing
untapped blockers into the cover (the attackers/blockers screens already know them) — the only survival arithmetic on
screen must not read "this cast does not save you" when it does; `{reserve:}` ends in two operands ("leaves 3 sources,
and ... needs 5") — end in the verdict word its siblings use; `{answers the stack: NO}` has no positive twin (125v162
s68/69, 66 s burned) — when a row DOES answer the stack (a counter, a lifegain >= the damage, a removal of the source)
say `{answers the stack: YES - ...}`; the blockers header names a life-only assignment that spends material and the pilot
copied it 2/2 (152: 146 s15/s19) — name the best-material assignment beside the life-only one. Read wave68/lane-BB.md,
lane-BD.md (J8c), wave67/lane-AW.md (I4), wave68/engine-seat.md §3 first.

## Lane BI — K7 (the record that says nothing, the surfaces nobody sees)
`transport` / `attempt_ms` fields are ABSENT from every record in the corpus (engine MED) — lane BC shipped them in
wave 68: find whether the wiring never fired (no wall miss this corpus is expected for attempt_ms — but `transport` should
appear on every round trip) or never landed; fix and PARSETEST the record shape. `async_drops` 2.15% (up from 0.95%) and
absent from gameend — add to the gameend census. 2,727 suppressed hold windows leave no record — one gameend counter
per suppression class. The repeat row's short name contains the literal `N` -> 2 `repeat_count_reask` (123: 162 s23/s25)
— render the number. `[repeat: activated this turn N times]` taken past 4 with no `repeat_past_stop` (130) and BA-7 FAIL
(0 clamps corpus-wide): find why the guard did not see the tagged row. `- GANG BLOCK:` row tags and the collapsed
multi-blocker tag render 0 times in 42 files (146) — a dead surface: delete the emitter or wire it (say which and why).
`(neither dies)` gains a price clause so the guides' literal never matches — keep the bare literal as the row's head
and append the price after it. Band rows still 1,425 B (BD-6 FAIL) — shorten as BD intended. 435 past-turn `- Paid ... with`
lines survive the fold (BD-9 FAIL) — find the branch the fold misses. Two literals for one tap fact (`{tapping X now:}`
vs `{paying this taps:}`) — one literal. 18 unusable counter rows on the Adversary menu (152 LOW) — collapse per BD's band
rule. Read wave68/lane-BC.md (record fields), lane-BD.md (band rows, Paid fold), lane-BA.md (J1), wave68/engine-seat.md
§1 census first.
