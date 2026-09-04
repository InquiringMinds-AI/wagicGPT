# Wave-59 step-1 lane brief — KNOWN BUGS ONLY

Base: master 35c9b85bf (wave-58 close). Baselines on a3f0700d3, hermetic `make -B`: PARSETEST 3455/0,
suite 1245 with 2 known concurrency-only failures (lifeline + merrow_reejerey; 0 at
WAGIC_TESTSUITE_THREADS=1), 61 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-58 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave59/known-bugs.md` (item numbers below). The
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
- Gates run DETACHED: `systemd-run --user --unit=w59-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w59-<lane>-*.log 2>&1; touch ~/.gatelogs/W59_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W59_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave59/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W59-<lane> (Kn)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave59/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

## Lane H — K1 (F3: the 2.5 s connect timeout that files transport failures as `empty_reply`)
Worktree: worktrees/lanes/w59-H. Read known-bugs.md #1 and wave58/engine-findings.md §F3 (site
src/GptConfig.cpp:822; sibling :926). Ship: (a) connect timeout raised to a generous value (the
request deadline `timeoutMs`, or a named constant of tens of seconds — owner doctrine: generous
defaults, tighten only on evidence; never a small arbitrary number); (b) every non-200 / empty
outcome records the curl result code and HTTP status in the translog record (a field, e.g.
`transport`), and the stderr line under the dev-log gate; (c) a transport failure (connect/timeout
curl code, HTTP 5xx, empty body with no 200) inside the decision deadline is RETRIED with the
remaining budget instead of handed to the heuristic on the first miss — bounded by the existing
deadline, never a loop; (d) PARSETEST cases for the classification (`noAnswerClassFor`) and the
new field's shape. Prediction: `empty_reply` records at 2.4-2.7 s = 0 of N; heuristic handoffs
fall toward the parse-failure floor.

## Lane I — K4, K5, K7 (three render verdicts that lie)
Worktree: worktrees/lanes/w59-I. K4 (wave58/engine-seat.md HIGH-1): the cleanup-discard rebadge
site still applies the whole-string predicate; route it through `verdictReadsZero` per scope so a
verdict naming a kill in any scope is never badged dead (Devour Flesh, Tragic Slip repros cited
there). K5 (wave58/deck162/review.md HIGH-1/2): lane B's D7 floor form prints a damage floor as a
survivable life floor and the B-row cites an A-line number that is not printed — the floor form
must claim death when `myLife - bestCase <= 0` and the cross-reference must point at where the
number actually is (AIPlayerGPT.cpp ~15122). K7 (wave58/deck146/review.md HIGH-2): the MDFC land
menu annotates the back face and renders the front face as bare `Play Land` — the front face row
gets the same name + arrival annotation (32 renders, 13 blind takes). PARSETEST for each string;
the existing #W58-B and #W58-C cases are updated, never deleted.

## Lane J — K8, K9, K10 (own-turn crack-back, PUT: parser, identical asks)
Worktree: worktrees/lanes/w59-J. K8 (wave58/deck123/review.md HIGH; general R319): on the pilot's
OWN turn no crack-back number renders at any life total; render the incoming-next-turn total from
opponent creatures that WILL untap (tapped ones count), same trample/menace rules as D9, at the
attackers seam and Main 1/2 casting windows. K9 (wave58/deck125/review.md HIGH-2): the cleanup
`PUT:` parser refuses `PUT: 9, 1 (Supreme Verdict, Fall of the Gavel)` — accept the parenthetical
gloss the CHOICE: protocol requires (positive/negative PARSETEST). K10 (wave58/deck126/review.md
HIGH-2): 32 byte-identical asks in one turn while a decided drain loop resolves and
`identical_option_asks_resolved` stays 0 — find why the existing identical-ask resolution does not
fire on this shape and make it fire (no blind cache: the answer is re-served only while the
rendered rows are byte-identical AND the plan is unchanged). Predictions per item.

## Lane K — K6 (Intrepid Adversary's repeated `{1}{W}` payment stops early)
Worktree: worktrees/lanes/w59-K. wave58/deck152/review.md HIGH-1: with 12 sources (7 W-capable = 6
payments) the engine made 4, then the mana line read 0; a second instance 7 sources -> 2 counters.
Verify the card's Oracle text (Scryfall) and its primitive (`/usr/bin/grep -n "name=Intrepid
Adversary" -A8 projects/mtg/bin/Res/sets/primitives/*.txt`) FIRST; then find whether the repeated
optional cost is the card script (fix in data) or the engine's repeated-payment loop / auto-tap
(fix in engine, general case). Fixture RED on base, GREEN after. Also make the mana line after a
partial repeated payment tell the truth.
