# Wave-64 step-1 lane brief — KNOWN BUGS ONLY

Base: master be2e88282 (wave-63 close). Baselines on 0026a9d87, hermetic `make -B`: PARSETEST 4344/0, suite 1268
with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at WAGIC_TESTSUITE_THREADS=1; plus
intrepid_adversary_repeated_payment.txt, a genuine intermittent), 66 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-63 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave64/known-bugs.md` (item numbers below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-64
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
- Gates run DETACHED: `systemd-run --user --unit=w64-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w64-<lane>-*.log 2>&1; touch ~/.gatelogs/W64_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W64_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave64/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W64-<lane> (En)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave64/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-191148 (+ the reruns matchups-20260905-214946 = 152v162 and matchups-20260905-215345 = 152v130)/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W64-<lane> (En)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave62/codex-review.md and wave63/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave63/lane-AB.md .. lane-AF.md for the emitters you will touch, wave63/engine-seat.md for the
adjudication, and wave61/corpus-livelock.md + wave62/corpus-hang.md + wave63/corpus-hang.md (three corpus-killing defects in three waves; a lane that
touches a shared choke point - a chooser, a key, a marker - must ask what ELSE reads it):
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

## Lane AG — F6, F7, F8, F9 (the blockers and modal renders that decided games)
F6 (deck146 HIGH-1): modal `choice` rows carry only the bare `name(...)` label. 146v152 seq 48 at 1 life: "return
creature and you draw" is `draw:1 controller && life:-1 controller` (primitive and Scryfall agree); the seat died at 0
with lethal on board. Price every modal row like a cast row (life delta, draw count, lethality verdict) — the
`describeAction`/row-price path the cast rows already use; R293/D1's rule on a surface it never reached. F7 (deck146
HIGH-2): `AIPlayerGPT.cpp:36702-36721` computes `need` from raw power vs toughness, so a 3/3 vs a 1/1 deathtoucher
gives need=1 and `gangBlockPriceTag` returns "" at its `need<2` guard while the printed 1-on-1 results say the
attacker LIVES (146v152 seq 20, seq 26; 7 boilerplate `GANG BLOCK:` strings, 0 live verdicts). Fold deathtouch and
first strike into `need` (the same combat math the 1-on-1 results use), never a guard that hides the verdict. F8
(deck123 HIGH-1; deck162 HIGH-1; deck152 MED; deck126 MED): the blockers-header suggestion (`...for the life: X blocks
Y`) is a life-maximiser rendered on NOT-lethal headers and copied 3/3 (123 vs146 s11/s15: spent the only token-maker
under Intruder Alarm; s15 named `your blocker dies, attacker lives` over `you kill it, your blocker lives`); 162 rerun
seq 18: the named block kills a punisher and hands 5 lifelink life, contradicting the same prompt's `KILLS them` draw
forecast; 146v152 seq 18: a pure chump labelled "chosen for your blockers' material" (`AIPlayerGPT.cpp:19488`); 126
vs152 seq 13: the best-case projection excludes the block-trigger gain the next line advertises. Wave-63 E3 ranked
material > survival and still printed these: make the label TRUE (say "life only" when material is not preserved;
never claim material for a chump), fold block-trigger gain into the projection, and on a NOT-lethal header say the
suggestion is optional and what it costs (the punisher, the token-maker) — it may not read as an instruction. F9
(deck123 HIGH-2; synthesis debt): vs152 s31 `this KILLS you ... you would be at 1 AT BEST` — the two clauses contradict
left-to-right; with 26 blockers printed the seat answered `BLOCKS: none` and lost a won game. State survival
affirmatively first when AT-BEST > 0 ("you SURVIVE with N if you block: ..."), and reserve `this KILLS you` for the
case no assignment survives. Read wave63/lane-AB.md and wave63/engine-seat.md (E3/E4 adjudication) first.

## Lane AH — F1, F2, F5, F10, F11 (verdicts and facts that lie by construction)
F1 (deck125 HIGH-1): `[<- best X for this cast: X=20 ... no listed X does more]` on a 12-card library (125v126 seq
527/528); the seat drew 20 from 70 life, 47 ahead, and decked itself. The verdict is mana-only: for any X whose
effect draws, clamp "best" to what the library allows (minus one per untapped refill the seat controls), print the
ceiling on the row, and never call an X that decks you "best". F2 (deck125 HIGH-2/3): `kNoPassRowFact` ("this ask has
no pass row", `AIPlayerGPT.cpp:307`, selected at `:28679`) renders on 680 of 843 windows that DO offer `Cast
nothing`/`Hold priority` — wave-62's `declineRowOffered` is passed by the menu family only; the cast seam (`:30706`)
and the priority seam never set it. Fix the flag at every seam and PARSETEST the composed window (a fact that
contradicts a row in the same prompt is the class). F5 (deck152 HIGH-3): `AIPlayerGPT.cpp:33893-33898` says "pick a
permanent worth bouncing, or decline" unconditionally; 152v125 seq 45 had 2 rows, no decline, and the spell DESTROYS —
the seat killed its own Sigarda. Derive the verb from the ability (`destroy`/`exile`/`bounce`/`sacrifice`) and the
decline sentence from whether a decline row exists. F10 (deck130 HIGH-1): the DRAW PUNISHERS/FORECAST block says
"before choosing to draw" and never that the draw step is compulsory; 162 seq 35: the seat took Hold at 10 straight
windows "to avoid drawing", 16 -> 1 idle. Say the turn's draw is not a choice; price only the OPTIONAL draws as
choices. F11 (deck130 HIGH-2, MED-3): at 1 life under `would KILL you`, `Cast Siege-Gang Commander [castable now]`
was priced against mana only; nothing tied its four bodies to the crack-back line (162 seq 43 — the seat passed on a
confabulated punisher trigger; the primitive draws nothing). On a lethal crack-back, every castable body/blocker row
says what it does to that line ("adds N blockers: crack-back N -> M"); and the attackers window says each attacker is
one fewer blocker for the printed crack-back (rerun seq 45, died at -4). Read wave63/lane-AC.md, lane-AD.md (E7, E10)
and wave62/lane-Y.md (D5) first.

## Lane AI — F3, F4, F12, F14 (engine: a phase that vanishes, an attack never offered, an actor inverted, a wait that never releases)
F3 (deck152 HIGH-1, engine): playing a pathway/MDFC land's FRONT face consumes the rest of main phase 1 — corpus-wide
FRONT -> 0 of 15 same-turn main-1 windows, back face -> 5 of 6; A/B 130-rerun seq 6 vs 126 seq 6; cost 9 damage at
152v125 seq 74 (no attackers window that turn at all) and pushed Intrepid Adversary to main 2 at 152v162 seq 16.
Find the mechanism (the land-drop click path for a two-faced front vs back — a phase advance, a consumed priority
window, or the E7/W3 auto-advance reading "no legal action"?) with a suite fixture RED on base (front face played,
then a castable creature must still be castable in main 1), fix the CAUSE. F4 (deck152 HIGH-2): no attackers prompt
in 21 games offers attacking a planeswalker though the engine supports it (152v162 seq 17: Ob Nixilis at 3 loyalty,
opponent creatureless, three unblockable attackers; the walker dealt 6 of the damage that killed the seat). Offer the
planeswalker as an attack target in the attackers bundle (`A1>PW` or a per-attacker target row — pick the shape the
parser can keep first-wins/illegal-drop safe), PARSETEST the reply shapes, fixture the engine path (`chooseAttackers`
-> planeswalker damage). F12 (deck126 HIGH, narration): a `targetedplayer`-granted ability inverts the ACTOR — your
Tribute to Hunger reads `Opponent used: Gain life`, theirs `You used:`; 8 occurrences / 5 games; cause visible at
mtg.txt:124075 (the granted ability's controller vs the targeted player). Fix the actor at the narration seam for
granted abilities (the life lines that follow are already correct — use their subject). F14 (engine HIGH-3): reveal
wait 561 s / 144,213 ticks at 152v146@1788653548 seq31 with no `reveal_stall_forced` — 62% of all reveal wait time in
21 games; the 20 s floor did not release. Find which gate held it (the async decision in flight? the stall counter
reset by a re-arm?) and make the floor release or say why it held; fixture if reproducible. Read wave63/lane-AD.md
(E11), wave60/lane-*.md for the reveal stall floor, and GameObserver.cpp's auto-advance block (W3b / no-legal-action
rule) first. Anything you add to a shared choke point (a chooser, a key, a marker): ask what ELSE reads it.

## Lane AJ — F13 + the MED list (the record, the hold, and the small lies)
F13 (engine HIGH-1/2; E6b FAIL): 130v126@1788653538 seq29 `latched_line_in_plan:true` on a column-0 `ATTACK: A4`
after a blank line (a SECOND top-level answer, not plan text) with no `plan_answer_line_only`; `plan_answer_line_ignored`
was written 0 times; 11 of 14 `answer_replaced` records carry a `latched_line` absent from the trimmed reply
(130v123@1788653511 seq30, 5,456 B trimmed). Record the latched line's byte offset and the paragraph bound in the
record; narrow `planParagraphBound` so a blank-line-separated column-0 coded line is an answer, not plan; write
`plan_answer_line_ignored` whenever a demotion happens. MED (each with the seat's seq): E9's relief clause leaks
`{removes ` into 9 `chosen_text` values (engine) — the chosen-text strip must drop the clause; E14a `(Wait,` defeats
the plan-cut sentence-start guard (engine, 1/2,801); E13's `; converters` string 30x (lane AC's emitter) and the
`{feeds:}` row whose two clauses both end `in your hand: N` counting different cards (deck162, vs125 seq 9); hold
broken by a transient `[this cannot target the spell on the stack]` clause — rows return byte-identical and the
window re-asks with no re-arm (deck130 MED-4, 125 seqs 72-74: a clause that varies per rebuild stays out of the key —
wave61/corpus-livelock.md); "N untapped creatures able to block" unfiltered by evasion, contradicting the per-attacker
tags in the same prompt (deck162, vs123 seq 16); "choose a card NAME" offers token names and annotates by public-zone
visibility (deck146 MED-3, 146v130 seq 14); effect text truncated mid-clause on Teferi's Puzzle Box hiding the clause
the lethal forecast is built from (deck146 MED-4, 146v162 seq 33); 8/12 self-targeting Kaya `-3` rows uncollapsed
(deck146 MED-5); E4b by the letter — `(1 if all block)` summed price below the tag's largest single price 4 (engine);
44/44 declined land drops were `[enters TAPPED]` rows — the tag omits next-turn value (deck125 MED-1). Read
wave63/lane-AD.md (E6, E10, E14), lane-AC.md (E9, E13), lane-AB.md (E4) and wave63/engine-seat.md first.

## Lane AK — the nine findings of wave64/codex-review.md (fix or refute, each with evidence)
Worktree: worktrees/lanes/w64-AK (base = master after the AG-AJ merge = 95b0aa152 + the review commit). Read
wave64/codex-review.md in full, then the lane report each finding touches (lane-AI.md for #1/#2, lane-AH.md for
#3?/#4/#7/#9 — #3 names the transport classifier, which no wave-64 lane touched: verify whether the diff moved it at
all and REFUTE-as-out-of-scope with the line if not, else fix, lane-AG.md for #6/#8, lane-AJ.md for #5);
wave63/lane-AF.md shows the expected shape of a CONFIRMED/REFUTED table. For EACH finding: VERIFY against the code
first (REFUTED with the line that proves it, or CONFIRMED with a reproduction — a PARSETEST case or fixture failing on
the current tree); fix every CONFIRMED one in the general form. #1 is HIGH and a doctrine breach (a legal option
removed): the candidate set for the attackers bundle must include every creature that may attack ANY legal
destination (player OR planeswalker/battle), with each row saying which it may attack. #2 is HIGH: adjudicate the
F3 fix's 24-tick floor against the doctrine (no window removed) AND the hang class it exists for (wave-58: a driver
that never passes is a dead game) — the pass may only be forced when the seat has NO legal action left (ask
computeActions / the oracle), never on a bare count; if a floor stays as a livelock breaker it must be gated on
"nothing legal remains" and its firing must be recorded. #4/#7: the crack-back cover claim must fold evasion
(flying/menace/unblockable per attacker), CANTBLOCK and enters-tapped, or say it cannot and print no survival
verdict. #5: a top-level correction may be indented — align `lineIsCodedAnswerLine` with `codedAnswerLineAt` and fix
the PARSETEST case that pins the wrong expectation. #6: skip the whole `ability$!...!$` granted block, not one
clause. #8: no silent cap at 64 — size the budget from the board, or fall back to the uncapped path and say so. #9:
the reserve counts a next-upkeep draw only when that upkeep is still ahead of the draw in question. Gate as usual
(suite THREADS=1 0 failed, AI count, PARSETEST 0 failed, RED evidence per confirmed finding). Tag `#W64-AK (Rn)`.
Write wave64/lane-AK.md with the CONFIRMED/REFUTED table.
