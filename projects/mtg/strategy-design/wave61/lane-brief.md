# Wave-61 step-1 lane brief — KNOWN BUGS ONLY

Base: master 718e99d2a (wave-60 close). Baselines on 614f8452e, hermetic `make -B`: PARSETEST 3699/0, suite 1248, 65 AI,
suite 1245 with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at
WAGIC_TESTSUITE_THREADS=1), 61 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-60 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave61/known-bugs.md` (item numbers below). The
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
- Gates run DETACHED: `systemd-run --user --unit=w61-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w61-<lane>-*.log 2>&1; touch ~/.gatelogs/W61_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W61_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave61/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W61-<lane> (Cn)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave61/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-171140/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W61-<lane> (Cn)`.

Standing rule from wave 60: after your lane merges, a Codex adversarial review reads the combined diff
(wave60/codex-review.md shows what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave60/lane-L.md, lane-N.md, lane-Q.md for the emitters you will touch.

## Lane R — C1, C2, C3, C4 (combat and stack aggregates that lie)
Worktree: worktrees/lanes/w61-R. C1: `ATTACK TOTAL` folds blocking-trigger lifegain (Perimeter Captain
per blocking defender) and attack-triggered punishers (Lightmine Field) that the per-row tags already
price, or states `before triggers: ...` explicitly — never a bare resulting life. C2: the blockers
ask's `INCOMING THIS COMBAT` gets B11's blocker-count-respecting floor; "may legally block every
attacker" prints only when blocker count >= attacker count (or per-attacker legality holds for a full
assignment); the `best case with every blocker assigned` clause never drops when it is computable.
C3: `CRACK-BACK NEXT TURN` counts creature-lands that can animate, activated direct damage from
untapped sources (sac outlets, pingers) as a separate `+ up to N from abilities` clause, and pump
triggers visible on the board; and nets evasion (fliers vs no flying/reach blockers) as a stated
sub-total. C4: `ON THE STACK` prices `target(anytarget)` activated abilities aimed at the pilot.
PARSETEST for each; a fixture where reachable.

## Lane S — C5, C6, C12, and the best-X NET half of C10 (price folds)
Worktree: worktrees/lanes/w61-S. C5: when both loop halves are on the seat's OWN battlefield the render
names the closed loop (LOOP CLOSED: ... every life gain drains, every drain gains) instead of only the
converter; LOOP HALF PENDING states when the other half is unaffordable now. C6: two `DRAW PRICE`
clauses on one row fold SEQUENTIALLY from the same base (one resulting life, the sum), never two
contradictory "you would be at" figures. C12: Peer into the Abyss (and any draw-N-for-target row)
carries the converter count and punisher price every other draw row prints. C10-NET: the
`[<- largest affordable X ...]` / best-X marker never endorses an X whose own row's NET life is
negative at the pilot's life without saying so (`... but NET -2 puts you at 0`). PARSETEST for each;
update, never delete, the wave-60 cases.

## Lane T — C7, C8, C9, C11 (facts the hand/land/menu rows withhold)
Worktree: worktrees/lanes/w61-T. C7: the hand listing marks each card `[castable now]` /
`[cannot pay now: needs X]` from the same castability the cast rows use (fourth-wave item; the reason
must be printed, not implied), and the land-drop row says `enters TAPPED` / `enters tapped unless ...`
from the primitive's auto=tap script. C8: a choose-ONE reveal (Pelakka Predation) renders a single-pick
protocol, and the narration reports what the engine actually took (never a two-card take on a one-card
choice) — find why the multi-select PUT shape was emitted. C9: dungeon-selection room counts come from
the engine's explore ladder (4/7/4), not a printed number; and the Acererak source clause (B12) is
verified to render on the row the model reads — it picked Lost Mine 14/14 after B12; find out why
(clause absent? wording? row order?). C11: `{spare: you control N lands already}` is a VERDICT — print
it only when N exceeds the highest cost in hand + 1 (or the deck's curve top); otherwise print the count
as a fact with no "spare". PARSETEST for each.

## Lane U — C10 sweeper marker + damage-wipe survivors, C13 transport, C14 economy
Worktree: worktrees/lanes/w61-U. C10: the sweeper CAST row gets the same `[<- ...]` ranking marker the X
menu's rung carries (the model obeys markers 6/6 and prose 0/51), and damage wipes (`deals N to each`)
list survivors by toughness as B7's roster does for destroy. C13: a curl 28 at the OVERALL deadline is a
`timeout`/`wall_miss` correctly — but connect-phase failures must class `transport_error` and retry;
prove which the two wave-60 records were (the log now has `transport`); restore `deadline_pct` (the
instrument B4's prediction reads) on every record. C14: a HOLD taken on an all-dead menu survives past
the turn boundary while the rendered rows stay byte-identical (no blind cache: any row change re-asks);
HOLD in a life-drain loop where "any change re-opens" is a real cost — state the re-open rule truthfully
per menu; and a reply that overruns past PLAN is trimmed at the record so the translog stops carrying
46 KB of overrun (parser behaviour unchanged). PARSETEST for each.

## Lane V — the eight findings of wave61/codex-review.md (fix or refute, each with evidence)
Worktree: worktrees/lanes/w61-V (base = master after the R-U merge + the review commit). Read
wave61/codex-review.md in full, then the lane report each finding touches (lane-T.md for #1/#3,
lane-U.md for #2/#4/#5, lane-R.md for #6/#8, lane-S.md for #7); wave60/lane-Q.md shows the expected
shape of a CONFIRMED/REFUTED table. For EACH finding: VERIFY against the code first (REFUTED with the
line that proves it, or CONFIRMED with a reproduction — a PARSETEST case or fixture failing on the
current tree); fix every CONFIRMED one in the general form. #1 is HIGH and blocks the ship: a
mandatory choose-one (Oracle "You choose") must not offer `PUT: none`; read the chooser's mandatory
flag from the script (`optional`/`may` vs bare), keep `none` only where the engine itself accepts a
decline, and fix the PARSETEST case that pins the decline as legal — this is legality enforcement,
not a removed window. #2: HOLD must retire when ANY held row disappears, not only when a new row
appears (superset, not subset, test — or exact set equality). #3: `tap(noevent)` counts as
enters-tapped only when it is on the card's own ETB line (`auto=tap(noevent)` at top level), not
inside an activated ability or a choice branch; shockland/choice shapes say `unless ...`. #4:
classification order — an HTTP status or curl result that arrived decides before elapsed time; a
late 503 is `http_error`, never `timeout`. #5: damage-wipe DIES/survivor claims exclude protection
from the damage's colour/source and regeneration/prevention where readable, else label the roster
`before prevention`. #6: crack-back ability sources count only untapped (or will-untap) sources whose
activation cost is payable from their open mana. #7: remove the four-branch cap in
`oneShotDrawGrantScan` (no hard caps). #8: blocking-lifegain ceiling counts only bodies that can block
at least one offered attacker (use the pairwise map). Gate as usual (suite THREADS=1 0 failed, AI
count, PARSETEST 0 failed, RED evidence per confirmed finding). Tag `#W61-V (Rn)`. Write
wave61/lane-V.md with the CONFIRMED/REFUTED table.
