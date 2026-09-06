# Wave-67 step-1 lane brief — KNOWN BUGS ONLY

Base: master 8ddf25c89 (wave-66 close). Baselines on 9b797fb81, hermetic `make -B`: PARSETEST 4906/0, suite 1273
with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at WAGIC_TESTSUITE_THREADS=1; plus
intrepid_adversary_repeated_payment.txt, a genuine intermittent), 67 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-66 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave67/known-bugs.md` (item numbers below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-67
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
- Gates run DETACHED: `systemd-run --user --unit=w67-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w67-<lane>-*.log 2>&1; touch ~/.gatelogs/W67_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W67_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave67/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W67-<lane> (En)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave67/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W67-<lane> (En)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave65/codex-review.md and wave66/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave66/lane-AQ.md .. lane-AU.md for the emitters you will touch, wave66/engine-seat.md for the
adjudication (wave 66 shipped a PROTOCOL change whose composition cost — the model transcribing section names as labels — no PARSETEST case could see: a change to what the MODEL is told must be measured against the corpus replies verbatim, both the intended shape and the shapes the wording invites; verify your fix at the seam the corpus
exercises, by a corpus-line PARSETEST case or an end-to-end fixture, never only at the helper), and wave61/corpus-livelock.md + wave62/corpus-hang.md + wave63/corpus-hang.md (three corpus-killing defects in three waves; waves 64-66 ran clean; a lane that
touches a shared choke point - a chooser, a key, a marker - must ask what ELSE reads it):
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

Also binding this wave: NEVER chain a build behind a counting grep (`grep -c` printing 0 exits 1 and skips an `&&`-chained
make — lane AT gated a RED binary that way); freshness-gate the binary (`ls -la bin/wagic` after every make).

## Lane AV — I1, I2, I3 (the reply rule, designed once from the corpus and measured before it ships)
This is ONE defect in three parts; read wave66/engine-seat.md §3 HIGH-1/HIGH-2 and §4 (7), wave66/deck130/review.md HIGH-1..3,
deck152/review.md HIGH-2/3, deck123/review.md HIGH-2/3, deck125/review.md HIGH-1, deck126/review.md HIGH-1, wave66/lane-AR.md
and lane-AP.md (R2/R3) FIRST. Then, before writing any code, MEASURE on the corpus replies (all 2317 records, `reply` field):
(a) how many replies carry a heading word before a label (`ANSWER:`, `CORRECTION:`, `THE ANSWER LINE:`, `So `, `Final:`)
per seam; (b) how many carry a PLAN but no coded line, and for how many of those a "cast/choose/play/discard X" sentence
before PLAN name-matches exactly one row; (c) for every reply with >1 coded line: which of {first coded line, last coded
line, the coded line immediately before `PLAN:`} equals the model's actual conclusion (judge from the prose), and how
often the model's retraction was "Wait,", "Correct Plan:", "Actually", or an enumerated header. Write the table into
wave67/lane-AV.md, then design: I1 — the label scanner (`findAnswerLabelLine` ~`:17506`, and `codedAnswerLineAt`,
`lineIsCodedAnswerLine`, the combat drivers, `parseChoice`) skips a heading word + colon before a label at EVERY seam (one
predicate, PARSETEST every corpus shape verbatim incl. `ANSWER: CHOICE: 3 (Cast nothing right now)` 162v123 s13, `CORRECTION:
CHOICE: 3` 130 s52, `So PUT: 1, 8` 126 s258, and `ANSWER: PUT: 44` 126v162 s10 which VOIDED a Tutor); I2 — a complete reply with
a PLAN and zero coded lines is salvaged by an exact single-row name match on the last action sentence before PLAN, else ONE
re-ask quoting the protocol line, NEVER Baka (record `label_missing_salvaged` / `label_missing_reask`); I3 — adopt the answer
anchor your table supports (the candidate is "the coded line immediately before `PLAN:`" — the model writes PLAN in 98% of
replies; if the count beats first-wins, ship it with first-wins as the fallback when no PLAN exists, record every override
as `answer_anchor_override` with the ignored line verbatim), and rewrite `kReplyProtocol` so NO section is named with a word
the model would write as a label (no "ANSWER", no "REASONING" as headings — describe, don't label) and the shape is
reasoning -> ONE coded line -> PLAN -> stop; the 8 per-decision tails must agree with it. Doctrine: first coded line
executes ONLY if the anchor rule says so; no last-wins over a negated candidate (wave-62 R2 — keep `combatLineIsClean` and
the negation test). Predictions: unparsed_reply < 5 per corpus; answer-first share; overrun bytes.

## Lane AW — I4, I9a + the row MEDs (numbers a row must carry)
I4 (engine HIGH-3; deck125 HIGH-2): the LIBRARY CEILING on X-draw rows ignores draws already on the stack — 125v123 s130
(library 14, two Staff of Nin triggers on the stack, badge reserved 1, X=13, decked at 44 life vs 3 = the lategame specimen);
125v126 s273 (library 22, ceiling 19, X=21). Every drawing X row and the ANNOUNCE_X ask print `{draws N of your L library
cards - L-N left; M more draws pending on the stack / this turn}` built from `stackPendingDrawsFor` (wave-63 R8) + the
draw-step forecast (AQ H1); the ceiling folds both; the X menu's ask repeats the ceiling sentence (125 HIGH-3: the X menu got
3.3 s of thought — the irreversible number lives in the second window, so the second window carries the fact). I9a (deck152
HIGH-1): repeat-pay mode menus print every unreachable rung — 152v146 s11 21 rows (~2.7 KB) for Intrepid Adversary with 1
payment affordable, rows 3-21 identical in effect; collapse unreachable rungs to ONE line naming the reachable maximum (the
rows stay in the engine menu; the RENDER collapses; map the answer back — verify with PARSETEST on the corpus ask). MEDs:
ATTACK TOTAL never says lethal/not (146 MED; 130 s20 called "lethal" a line leaving them at 3) — print the verdict the
totals already compute; `[no cast row now: sorcery speed - only in your own main phase with an empty stack]` says WHICH half
failed (130 MED; 125 s52); MDFC land rows carry the `enters UNTAPPED` clause ordinary land rows do (152 MED-2; verify
Hengegate Pathway `borderline.txt:52388`); Tribute rows print the "you gain at least N" floor at 20+ creatures (126 MED);
the cast row's "max affordable X" vs board mana vs menu ceiling disagreement (125 MED, 1 of 17 — find the third source).
Read wave66/lane-AQ.md (H1/H4), wave65/lane-AL.md, wave66/engine-seat.md §3 HIGH-3 first.

## Lane AX — I5, I7 + transport (engine: a void, a stall, a record that says nothing)
I5 (deck126 HIGH-2; engine HIGH-4; AT MED-3 FAIL): a refused reveal reply VOIDS the spell — 126v162 s10 `ANSWER: PUT: 44`
refused -> s12 "the reveal could not be completed - the engine returned the 52 revealed cards to your library" (Idyllic
Tutor voided T5; game lost). A refused/unparsed reveal reply must degrade to a LEGAL pick (Baka's chooseCard on the revealed
set, recorded `reveal_fallback_pick`), never a void; fixture it (the suite has reveal drivers). Both `reveal_stall_forced`
records (driver_secs 600, latency -1) are GENUINE engine-side stalls — the wave-58/59 signature — root-cause the 600 s wait
(what is the driver waiting on when no model call is in flight?) and fix or say exactly why not; and `reveal_wait_unexplained_secs`
is read (`:15684`) after `mLastLatencyMs` is consumed (`:15348` stamp, `:15414` consume) — 14/16 reveals report the whole wait;
fix the order (PARSETEST the pure residual with the real sequence). I7 (engine MED-1, load-bearing; deck162 HIGH-2): the
record stores NOTHING about what Baka executed on a fallback (`choice:-1`, `chosen_text:<refused>` on 76/83) — every
fallback record carries the executed row index + text + the seam (one field pair; no key/tail change); and a DECLINE whose
board key is unchanged is honoured like a hold, not re-asked in the same step (162v130 s16->17, s18->19: reservation
declined, re-asked with nothing changed, flipped, lost at exactly-lethal) — extend the AS H7 hold machinery to the
reservation decline (`{reserve:}` rows) with the same key discipline (NOTHING volatile in mPromptTail). Transport MED: 2 wall
misses are 900 s curl-28 EMPTY replies with `connect_ms=20000` and no retry (146 s11) — AP-R6 says a connect timeout short of
the band is `transport_error` with the bounded remainder; find why a 900 s wall was reached (is the connect phase re-entered
inside the deadline? is the band test on the wrong clock?) and PARSETEST the corpus record's exact numbers. Also `async_drops`
59 (wave 65: ?) — census by seam, report. Read wave66/lane-AT.md (MED-3), wave65/lane-AP.md (R6), wave66/engine-seat.md §1/§3.

## Lane AY — I6, I8, I9b + render MEDs
I6 (deck123 HIGH-1/4): `repeat_past_stop` is advisory — 123v126 s84 "you at 66 with your stop at 26", s85 answered the
identical `x34` and it RAN (M 66->100); 3/20 executed past the stop. When the second answer repeats a counted take past the
stop the MODEL ITSELF stated, execute that stated stop (the model's own number — not a cap; record `repeat_clamped_to_own_stop`
with both numbers); and the `{right now: M=.., your stated stop=..}` clause appeared in 1/20 refusal windows because it reads
the CARRIED plan while the guard reads the reply's own PLAN — one source for both, and the clause must survive a refusal
(the carry drops after refusals 5/31 and after long pass runs). I8 (deck146 HIGH-1/2): the seat's GRAVEYARD is never
rendered (146v130 s29/31/33: correct live-mode census, no corroborating surface, 165 s re-derivation, "contradiction",
declined at 3 life, died) — add `Your graveyard (N): ...` beside `Your battlefield` (and `Their graveyard (N)` when
non-empty; bucket beyond 12 like the battlefield line; NOT in the key tail); and `LOOP HALF PENDING` calls an EXILED half
"one resolution from closing" (146v126 s21-33, 674 s) — gate on a returnable zone. I9b (deck126 HIGH-3): `plan_contradicts_
noop_row` did not fire on the textbook case 126v125 s83/84 (row `{they control 0 creatures - at 0 this does nothing}`, plan
"this does nothing. This is a waste of mana and cards") — find why the AR H8 conjunction missed (`planArguesAgainstRow`
wording? the seam?) and PARSETEST the corpus reply verbatim. MEDs: opposing statics tagged on a creature without naming their
SOURCE (152 MED-3) — name it; the exemplar names a HALF DEAD row (engine MED; 146 s22) — extend AU R5's rule to half-dead;
the turn-14 log prints ~200 per-token death lines while the board line is bucketed (123 MED; 126 s115) — bucket the
narration the same way; hold churn runs (130: 8 windows in one draw step at 3 life; 123: an 18-window drained turn) —
census the cause (which fact changed each window?) and fix only what the census proves. Read wave66/lane-AS.md (H3/H7),
lane-AR.md (H8), lane-AU.md (R5), wave66/engine-seat.md §4 first.

## Lane AZ — the seven findings of wave67/codex-review.md (fix or refute, each with evidence)
Worktree: worktrees/lanes/w67-AZ (base = master after the AV-AY merge = af986181b + the review commit). Read
wave67/codex-review.md in full, then the lane report each finding touches (lane-AV.md for #1, lane-AY.md for #2/#6,
lane-AX.md for #3/#7, lane-AW.md for #4/#5); wave66/lane-AU.md shows the expected shape of a CONFIRMED/REFUTED table. For
EACH finding: VERIFY against the code first (REFUTED with the line that proves it, or CONFIRMED with a reproduction — a
PARSETEST case or fixture failing on the current tree); fix every CONFIRMED one in the general form. Three are DOCTRINE
breaches and take priority: #3 (~`:35367`) the reservation-decline latch auto-passes later windows on phase + candidate
names + source count alone — a decline may be honoured ONLY while the FULL board key (the hold key, AS H7 + AU R1 discipline:
stack, life, permanents, hand count) is unchanged; anything less is a blind cache — key it on that, re-verify at 162v130
s16-19 that the corpus pair still latches (nothing changed there). #4 (~`:37781`) the repeat-pay render HIDES legal rungs —
the render must keep EVERY legal row reachable: collapse to `rows 3-21: Add 3 .. Add 20 (identical in effect right now —
answer any number in that band)` so a number in the band maps to its engine row (the shownToFull map must accept the band),
never a single surviving row; fix the pin at ~`:71452`. #7 (~`:44296`, `AllAbilities.cpp:1341`) an unusable mandatory reveal
reply is answered with the FIRST eligible card instead of the heuristic's pick — call Baka's `chooseCard`/target-choice
scorer on the eligible set (the "safe default" the brief asked for is the heuristic, not vector index 0), fix the pin at
~`:71126`; the driver floor in AllAbilities may stay as the last resort but must record that it fired. #1 (~`:18407`) the
label-less salvage matches `I will not cast Doom Blade` to the Doom Blade row — polarity: a sentence with a negation
(`not`, `never`, `don't`, `won't`, `rather than`, `instead of`) before the action verb is NOT a decision; when the only
match is negated, re-ask; PARSETEST the negated shapes. #2 (~`:32549`) the clamp's `allowed == 1` boundary converts one
legal repetition into a pass while narrating one — execute one (the row family supports x1) or narrate zero truthfully;
add the boundary case. #5 (~`:12263`, `scanStackAbilityDraws` ~`:4366`) an unresolved `MayAbility` draw is counted as
mandatory in the library reserve — count a may-draw as OPTIONAL (say "up to"), never as "cannot decline". #6 (~`:20505`)
a loop half in the graveyard is not returnable without a recursion effect — `halfCanReturn` requires a visible recursion
source (a known reanimation/return-to-hand card in hand/battlefield) or reads "in the graveyard; needs recursion to
return"; fix the pin at ~`:71763`. Gate as usual (suite THREADS=1 0 failed, AI count, PARSETEST 0 failed, RED evidence per
confirmed finding). Tag `#W67-AZ (Rn)`. Write wave67/lane-AZ.md with the CONFIRMED/REFUTED table.
