# Wave-68 step-1 lane brief — KNOWN BUGS ONLY

Base: master 949a514d7 (wave-67 close). Baselines on 14977f3af, hermetic `make -B`: PARSETEST 5107/0, suite 1274
with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at WAGIC_TESTSUITE_THREADS=1; plus
intrepid_adversary_repeated_payment.txt, a genuine intermittent), 67 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-67 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave68/known-bugs.md` (item numbers below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-68
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
- Gates run DETACHED: `systemd-run --user --unit=w68-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w68-<lane>-*.log 2>&1; touch ~/.gatelogs/W68_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W68_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave68/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W68-<lane> (En)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave68/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W68-<lane> (En)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave66/codex-review.md and wave67/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave67/lane-AV.md .. lane-AZ.md for the emitters you will touch, wave67/engine-seat.md for the
adjudication (three wave-67 lane claims were FALSIFIED by the corpus — AW "row and header cannot differ", AX "zero retry budget", AY/AZ own-stop reachable — each was argued from code reading where a corpus count was available: COUNT before you claim; wave 66 shipped a PROTOCOL change whose composition cost — the model transcribing section names as labels — no PARSETEST case could see: a change to what the MODEL is told must be measured against the corpus replies verbatim, both the intended shape and the shapes the wording invites; verify your fix at the seam the corpus
exercises, by a corpus-line PARSETEST case or an end-to-end fixture, never only at the helper), and wave61/corpus-livelock.md + wave62/corpus-hang.md + wave63/corpus-hang.md (three corpus-killing defects in three waves; waves 64-67 ran clean; a lane that
touches a shared choke point - a chooser, a key, a marker - must ask what ELSE reads it):
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

Also binding this wave: NEVER chain a build behind a counting grep (`grep -c` printing 0 exits 1 and skips an `&&`-chained
make — lane AT gated a RED binary that way); freshness-gate the binary (`ls -la bin/wagic` after every make).

## Lane BA — J1, J3, J6 (the reply that runs long, the reversal nobody honours, the stop nobody reads)
J3 (engine FAILs AV4/AV5; deck146 HIGH-3; deck126 HIGH-1/2; deck152; deck130): answer-first is 98.9% and overrun 662,814 B —
reasoning MOVED past the label, it did not shorten; 146v125 s19 5,675 B / 866 s; 126's two answer-LAST blockers replies
(825 s, 462 s) score 0 overrun. FIRST measure on the corpus: per seam, the wall-time spent in reply generation (latency
by seam), bytes before and after the label, and what the 20 slowest replies contain (repetition? re-derivation of a
printed fact?). Then ship BOTH: (a) a per-seam `max_tokens` cap in the request (`WAGIC_GPT_MAXTOKENS` stays the global
ceiling; each seam gets a smaller default sized from the corpus — answer + PLAN + a bounded allowance), a reply cut
before its label earns ONE re-ask quoting the cap (`reply_truncated_reask`), never Baka; the protocol states the budget in
words ("at most N sentences before the PLAN line"); (b) the overrun meter counts bytes on both sides of the label. Do NOT
touch the answer order or first-wins. J6 (deck130 HIGH-3; deck123 HIGH-2; engine MED): `decision_reversed_in_prose` is not
wired at ATTACKERS (0/74; 130v126 s24 `ATTACK: A1-A4` executed under a PLAN saying "I must NOT attack ... pass combat",
lost 0/34); `answer_replaced` never fires on a prose-only retraction (123v130 s43 answered Devour Flesh then "at 0
creatures does nothing... I will hold priority", cast it); the no-op re-ask lost to the one-re-ask-per-board budget (dead
Devour Flesh cast at 7 life). One reversal predicate at every seam (CHOICE/ATTACK/BLOCKS/PUT), a prose reversal naming
PASS/hold/"do nothing" earns the one re-ask, and a `{does nothing}`/`destroys 0` row's re-ask is exempt from the
per-board budget. J1 (engine HIGH-1; deck123 MED-1): `repeatPastStop` (~`:32639`) is gated on `namedCount >= 1` so it sees
only the counted row (4 records) while 20 records ran a plain single-activation past the reply's own stated stop (123
s27-48, M 24->35 vs stop=23); `proseNamesOtherMenuRow` narrowed 15/15 reversal candidates away. The stated stop applies to
ANY activation of the same ability; a reversal that names pass is never narrowed by a row-name mention. Read
wave67/lane-AV.md, lane-AY.md (I6), lane-AZ.md (R2), wave67/engine-seat.md §3 HIGH-1 + §4 first.

## Lane BB — J4, J5, J9 (numbers the two surfaces disagree on)
J4 (deck125 HIGH-1/2; AW claim falsified): ANNOUNCE_X header and row clause state DIFFERENT ceilings — 125v130 s90 header
"3 draws are already owed ... X=1 is the largest value the library pays for", option 6 `{library: this draws 4 of your 4
library cards - 0 left}`; `; M more pending` rendered 0 times in the corpus. One ceiling function feeds both surfaces and
the pending draws fold into the ROW clause (PARSETEST the s90 shapes verbatim, both surfaces from one call). And the X
menu's parser treats a bare-integer row name as the X VALUE — `CHOICE: 2 (X = 2)` must never raise `index_name_conflict`
(125v162 s90). J5 (deck130 HIGH-1/2): a post-announcement decline forfeits the mana already paid and the row never says so
(130v126 s21-24, s27-31 with a false `Paid ... from mana already floating` line for a cast that vanished; 130v162 s70-73
lost the deck's lethal line) — the decline row on a post-announcement menu carries `{declining now FORFEITS the N mana
already paid}`, and the vanished-cast narration is fixed (say what happened); `[cannot pay now: needs N mana, you have 0
untapped sources]` ignores the floating pool — 55 of 82 floating-pool prompts, 6 contradicting an offered Cast row in the
same prompt; fold the pool (the row's own affordability test already knows it). J9 (deck123 HIGH-1; deck162 MED-3): the ON
THE STACK death verdict (`you would be at -1; that would KILL you`) sits ~40 lines above the options; 123v162 s32 held
priority with a self-targetable Devour Flesh on row 1 and died. Repeat the verdict INSIDE the option block on the row(s)
that answer it (the blockers seam already does this) — `{answers the stack: ...}`; and the hold key must re-open when the
CRACK-BACK verdict WORD changes (162v146 s19/20, `hold_windows_skipped 56` in the lost game) — extend AU R1's enumerated
clauses with the verdict-word test. Read wave67/lane-AW.md (I4), wave66/lane-AS.md (H7), lane-AU.md (R1), wave67/engine-
seat.md §3 first.

## Lane BC — J2, J7 + record MEDs (transport, card script, the record)
J2 (engine HIGH-2; deck162 HIGH-1): a 900 s wall miss takes the wall arm (~`:15196`, `mRetryBudgetMs = 0` meaning "the full
deadline again"), the retry ran 333 s, `:15090` sums both legs into one `latency_ms` and `:16047` divides by one deadline
(`deadline_pct 137`). Lane AX's "zero retry budget by arithmetic" was wrong — COUNT: 3 wall misses this corpus, each
retried? A wall miss gets NO second full deadline: no retry (Baka, `wall_miss_no_retry`) unless a stated remainder
exists; the record splits per-attempt latency (`attempt_ms`), and `deadline_pct` is per attempt. PARSETEST the s13 record's
numbers. J7 (deck152 HIGH-1/2) CARD SCRIPT, invoke the /wagic-cardscript discipline: Moonrage Brute `borderline.txt:73998`
`target(*[instant;sorcery]|opponentzones)` leaked 12 opponent LIBRARY cards + a HAND card as pickable rows (152v130 s33)
mislabelled as the ward controller's choice. Verify the Oracle (ward N — counter the spell unless its controller pays),
fix to `opponentstack` (198 uses in the same file), then AUDIT EVERY ward script in `primitives/*.txt` for the same idiom
and fix each (data, no recompile; fixture one). Teferi's +1 cascade: stage 1 renders `1. choose a creature / 2. Decline`
with no candidates and no pass row after it (152v123 s39-41, 427 s; 152v125 s40/57 committed onto an all-`[tapped]`
list) — the stage-1 ask lists the candidates stage 2 will offer and declares the pick order (land/creature/artifact).
MEDs: a `named_row_reask` recovery record carries neither `executed_seam` nor `recovered_by` (123 s42) — add the
`noteHeuristicExecuted` site; `async_drops` 22 — census by seam; the legend-rule ask prints neither the rule nor either
copy's loyalty (146 s51, model fabricated "4+4=8") — print both. Read wave67/lane-AX.md, wave65/lane-AP.md (R6),
wave67/engine-seat.md §3 HIGH-2, /home/magi/.claude/skills/wagic-cardscript/SKILL.md first.

## Lane BD — J8 + render MEDs (the wrong side of the ledger)
J8 (deck146 HIGH-1/2; deck126 MED-3): Silverquill Command's sacrifice mode prints no edict floor — 146v123 s31 `{this mode
has a legal object right now}` against 31 creatures (planned to edict Thraben Doomsayer, killed 2 of 31 tokens) while
standalone edicts print `they control N creatures - they choose which one`: ONE edict-floor function for mode rows and
standalone rows. "Opponent draws" modes are annotated as a benefit and the DRAW GRANT bracket prices their draw as `take 0
off them` — 146v125 s16/20 the model wrote "a 2-for-1 (I spend 1 card, they draw 1)": a granted draw is a PRICE
(`#W47-R3`'s framing for granted life), on the mode row and in the bracket. Tribute to Hunger's row never prints
`{crack-back cover:}` though creature rows do (126 g123 s12) — the cover clause on every row that removes an attacker
(edicts, removal), from the same crack-back screen. MEDs: `NO LIVE CAST ROW` never says the land drop / manland / attack
/ walker ability are still live (146 MED; 146v125 declined an untapped Hive 12 times against zero creatures) — the header
names what IS live; I9a's byte saving is ZERO after AZ R4 (7 band windows 2514-2737 B) — keep every rung, shorten each
band row to label + `(same effect right now)`; narration 484 lines / 30.5 kB post-bucketing — census what dominates
and bucket it; the opponent's library size as a deck-out forecast when it is within 3 draws (162 LOW); gates asked for
types the opponent has none of (152 s59) — say "none" instead of asking. Read wave67/lane-AW.md (I9a), wave66/lane-AQ.md
(H9 census), wave67/deck146/review.md first.

## Lane BE — the eight findings of wave68/codex-review.md (fix or refute, each with evidence)
Worktree: worktrees/lanes/w68-BE (base = master after the BA-BD merge = 95e8f11c3 + the review commit). Read
wave68/codex-review.md in full, then the lane report each finding touches (lane-BA.md for #1/#3/#4, lane-BC.md for #2/#8,
lane-BD.md for #5/#7, lane-BB.md for #6); wave67/lane-AZ.md shows the expected shape of a CONFIRMED/REFUTED table.
#1 FIRST, it is a real defect the orchestrator verified: `AIPlayerGPT.cpp:15516` col 379 carries `//#W68-BA (J3/J6)` INSIDE
the constructor initializer list and comments out the 30 initializers after it (mAttacksDoneTurn, mPassDeclineTurn,
mLoopAbility/Click/Count, mRepeatAbility/Click/Remaining/Total/Done/NoProgress/Absent, ...) — undefined reads at every
seat construction. Move the comment off the line, verify every member of that list is initialized (diff the list
against the header's members), and add a guard: a PARSETEST-time or build-time check that no `//` occurs inside the
ctor initializer list (python over the source in the gate script, or `-Wreorder`/`-Weffc++` evidence) so this class
cannot recur. #2 HIGH: the legend-rule clause says "you KEEP the one you pick" but `MTGRules.cpp:4102`'s AAMover moves
the SELECTED target to the graveyard — verify the engine's semantics by fixture (two same-named walkers at 1 and 8
loyalty, pick one, assert which survives) and make the clause say what the engine does; fix the pin at ~`:74058`. #3
HIGH doctrine: extending the stop clamp to the plain activation row converts an explicitly re-affirmed `CHOICE: 1` into a
pass (~`:33987`, pin at ~`:74705`) — after the stop-conflict re-ask, a SECOND explicit answer for ONE activation executes
ONE activation (the model's own stop applies to counted takes; a single explicit take is the model overriding its own
plan, which it may do); clamp only counted rows; fix the pin. #4 MED: the combat seams (attackers ~`:44244`, blockers)
delegate to Baka on a cap-truncated reply with no label — wire `reply_truncated_reask` there as J3 promised. #5 MED: the
edict crack-back "STILL KILLS you" is a worst-case bound, not a verdict — they choose the sacrifice; print the bound as a
bound ("can still reach N; a kill is POSSIBLE, not certain") and drop "that is the one they will give up"; fix ~`:74500`.
#6 MED: the announcement-decline clause claims the floating pool is FORFEITED while `castAbandonedNarration` says it is
still floating — say the truth: the mana stays floating until end of step, spendable on another row (name whether a row
can use it); fix ~`:74244`. #7 MED: the deck-out countdown must fold `CANTMILLLOSE`/`CANTLOSE`/`CANTWIN` (helper takes zone
counts only — pass the flags); fix ~`:74526`. #8 LOW: per-attempt `deadline_pct` divides by the original deadline —
divide each attempt by its own budget. Gate as usual (suite THREADS=1 0 failed, AI count, PARSETEST 0 failed, RED
evidence per confirmed finding). Tag `#W68-BE (Rn)`. Write wave68/lane-BE.md with the CONFIRMED/REFUTED table.
