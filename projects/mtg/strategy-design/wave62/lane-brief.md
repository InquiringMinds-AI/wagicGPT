# Wave-62 step-1 lane brief — KNOWN BUGS ONLY

Base: master 0d9b2d52e (wave-61 close). Baselines on 8097c12cc, hermetic `make -B`: PARSETEST 3915/0, suite 1249
with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at
WAGIC_TESTSUITE_THREADS=1), 65 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-61 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave62/known-bugs.md` (item numbers below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-62
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
- Gates run DETACHED: `systemd-run --user --unit=w62-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w62-<lane>-*.log 2>&1; touch ~/.gatelogs/W62_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W62_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave62/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W62-<lane> (Dn)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave62/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-092408/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W62-<lane> (Dn)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave60/codex-review.md and wave61/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave61/lane-R.md, lane-T.md, lane-U.md, lane-V.md for the emitters you will touch, and wave61/corpus-livelock.md:
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

## Lane W — D1, D15, D17 (the land-drop and hand rows: enters-tapped, MDFC faces, wording)
D1 is the wave's biggest false claim: 131 of 172 `Play <land>` rows print an unconditional `enters TAPPED - it makes no mana
this turn` while quoting the card's own "unless you control..." sentence. Read wave61/engine-seat.md HIGH-1, deck125/review.md
HIGH-1, deck152/review.md HIGH-1 (the two missed idioms with mtg.txt line cites), deck123/review.md MED-1 (the condition is
often DECIDABLE from the printed battlefield: land subtypes). Fix `landEntersTappedTag` (lane V finding 3 read only the first
idiom) in the general form: classify all three idioms; when the condition is decidable from the pilot's own battlefield,
RESOLVE it and print the truth ("enters UNTAPPED: you control a Plains"); otherwise print the hedged form quoting the
condition. PARSETEST: one case per idiom (Glacial Fortress / Drowned Catacomb shape; Deserted Beach `if ... then` shape;
Isolated Chapel one-line shape; Bant Panorama must-NOT-match), plus a resolved-true and resolved-false case. D15: MDFC land
faces (deck146 HIGH-3) — the header says lands are never listed in a casting menu; either the header stops lying or the face
carries a land marker in-game as it does pre-game. D17: the one-land land-drop ask wording (`Hold X - do not play it now`)
vs the many-land one (deck123 MED-2) — one wording, PARSETEST both shapes; and the `plan_missing` inconsistency (MED-3).

## Lane X — D2, D6, D8, D19 (combat aggregates under loops and forecasts)
D2 (deck123 HIGH-1, C1 repro): `ATTACK TOTAL ... KILLS them whatever they block, gain included` printed beside a loop
banner saying any gain is fatal; a BLOCKING lifegain trigger resolves in declare-blockers and the opposing Sanguine Bond +
Exquisite Blood loop runs the attacker to 0 before damage (seq 52 -> 73 -> gameend). Fail closed as C1 does for unpriced
punishers: no kill claim when a life loop is on their side and blockGain > 0 — and SAY why. D8 (deck126 engine HIGH): DRAW
FORECAST says "your next draw step" while the triggers are already on the stack, and a forecast exceeding printed life carries
no fatal verdict; and no row is tagged as winning while the loop banner prints — add the verdict, fix the tense, tag the
winning row when the engine can prove it. D19 (deck130 HIGH-2): target rows print `DIES` but never price it against the same
screen's CRACK-BACK total (152 s18: killing the 2/2 survives at 1, killing the 1/1 dies at 0). D6 (engine HIGH-2 + R6 FAIL):
CRACK-BACK names Lolth's -8 emblem "8 per activation" at 1 loyalty — blocking stops it — and the R6 gate prices a next-turn
forecast against THIS turn's open mana, dropping 53 true clauses. Split the gate: next-turn mana for next-turn claims.
Read wave61/lane-R.md and lane-V.md (findings 6, 8) first.

## Lane Y — D3, D4, D5, D7 (engine-side: a trigger that does not fire, X rungs, X=0, a modal cost)
D3 (deck162 HIGH-1; closes the lifelostfoeof question open since wave 60): `@lifelostfoeof(player)` (Exquisite Blood,
mtg.txt:37966) does NOT fire on damage-based life loss, so the LOOP CAUTION narration is false and the pilot's own punishers
do not feed the opponent's loop. Oracle ("Whenever an opponent loses life") counts damage as life loss. Verify Oracle
(Scryfall) AND the engine binding in source, then fix the ENGINE trigger in the general form (every `lifelost*` trigger that
should see damage-based loss) with a suite fixture RED on base; update the caution text only if the fix changes what is true.
State clearly what other cards the binding change touches (grep the primitives for the trigger). D4 (deck152 HIGH-2): Lair of
the Hydra's `{X}{G}: X/X` animate is rendered as `becomes a 1/1 hydra {1}{g}` in all 387 rows — the primitive defines 20 rungs
(borderline.txt:64293-64312); render the affordable rungs, or the best affordable, never rung 1 alone. D5 (deck130 HIGH-1):
an X spell whose max affordable X is 0 is offered, then the X menu is one row with no pass — card burned; suppress the cast
row at max X = 0 or give the X menu a Decline row (window-preserving: the latter). D7 (deck152 HIGH-3): the Intrepid
Adversary "add N counters" modal ask prints no mana and no `{paying this taps: ...}` clause; the engine paid 3 counters with
Katilda + Elite Spellbinder and the Attackers step vanished (G2 seq 23/24) — print the cost per rung; and look at the
intermittent `intrepid_adversary_repeated_payment.txt` (lane T) while you are there.

## Lane Z — D9, D10, D11, D12, D13, D14, D16, D18 (reply protocol, parser, hold, tails, transport)
D9 (deck146 HIGH-1): the answer line is committed before the model reasons; 146v152 seq 54 wrote `BLOCKS: B2:A2, B1:none`
then derived `B2:A1` in its PLAN and died. deck126 reports the engine now latches the LAST coded CHOICE (`answer_replaced`,
125 seq 64/79/87) — make BLOCKS/ATTACKS consistent with CHOICE (last coded line wins, PARSETEST), and state the rule in the
reply protocol. D10 (deck162 HIGH-2): `latched_row_mismatch` discarded `CHOICE: 4 (Cast Teferi's Puzzle Box)` where index and
name both matched row 4 — parser bug, PARSETEST with the exact record text. D11 (deck126): out-of-range `CHOICE: 5/6` naming a
card not on the battlefield lost three windows — the re-ask must quote the offending name. D12 (deck146 HIGH-2): the
survivable block matching is computed (`AIPlayerGPT.cpp:17351`) and only the number printed — print the assignment. D13
(deck125; C14b SPLIT): the hold row's "this turn or later" promise is seam-scoped (9 leaks) and `[you declined this exact list
N times]` contradicts `[hold check: 1 row above is new]` in one prompt — make the two notes agree or drop the false one. D14
(deck125): `Cast Sphinx's Revelation` hides its tap-out tail while carrying the best-X badge. D16 (deck152): a ~3 KB PLAN is
truncated then quoted back for six windows — bound it once; and "Night" (a day/night marker, not a permanent) is offered in
Emrakul's forced-sacrifice list. D18 (deck130): a 900 s `curl=28, connect_ms=20000` is classed `wall_miss_unrecorded` though
the transport phase says wall. Read wave61/lane-U.md, lane-V.md (findings 2, 4) and wave61/corpus-livelock.md first.

## Lane AA — the eight findings of wave62/codex-review.md (fix or refute, each with evidence)
Worktree: worktrees/lanes/w62-AA (base = master after the W-Z merge + the review commit). Read
wave62/codex-review.md in full, then the lane report each finding touches (lane-Y.md for #1/#4/#6,
lane-Z.md for #2/#8, lane-X.md for #3/#5/#6, lane-Y.md + lane-X.md for #3 — the D3/D8 cross-lane
double emit; lane-U.md (wave 61) + lane-Z.md for #7); wave61/lane-V.md shows the expected shape of a
CONFIRMED/REFUTED table. For EACH finding: VERIFY against the code first (REFUTED with the line that
proves it, or CONFIRMED with a reproduction — a PARSETEST case or fixture failing on the current
tree); fix every CONFIRMED one in the general form. #1 is HIGH and blocks the ship: the ordinary
cast-route ANNOUNCE_X branch in DecisionContract.cpp must set canDecline like the announcing branch
does, with a fixture on the cast route (Starstorm at max X = 0 keeps the card). #2 is HIGH: the
combat-restatement scanner must not promote a candidate whose preceding prose negates or rejects it
("should not", "not", "don't", "rather than", "instead of" ...) — the safe rule is: a restatement is
taken only when its own sentence is affirmative and no negation token precedes the directive on that
line; PARSETEST the negated shape as must-NOT-match. #3: one forecast line per draw step — the
merge left the wave-61 direction-agnostic LOOP CAUTION emitter alive beside lane Y's per-line
binding; remove the stale emit, PARSETEST that a theirs-forecast carries exactly one loop clause.
#4: pay-repeat affordability must fit colours (use the mana engine's payability for N copies of the
cost, not source count / CMC), else say "up to N if colours fit". #5: the next-turn manland gate must
exclude the manland's own mana from paying its activation and check colour fit. #6: THIS WINS THE
GAME off a loop only when the opponent's life can change (no cantchangelife / prevention on their
side readable); otherwise fail closed as the C1 rule does. #7: GptConfig.cpp must keep the received
HTTP status when curl later fails, so the status-first classifier sees it. #8: a completed HTTP 200
with an empty body is `empty_reply` regardless of elapsed fraction; fix the PARSETEST case that pins
the wrong order. Gate as usual (suite THREADS=1 0 failed, AI count, PARSETEST 0 failed, RED evidence
per confirmed finding). Tag `#W62-AA (Rn)`. Write wave62/lane-AA.md with the CONFIRMED/REFUTED table.
