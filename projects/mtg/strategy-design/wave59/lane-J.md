# Wave-59 lane J — K8, K9, K10

Base: master `35c9b85bf`. Branch `w59-lane-J`. Files touched:
`projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h`. No Res asset, no card
script, no engine rule: all three items are render / parse / ask-flow surfaces, so PARSETEST is
the whole gate by the brief's own split (the same split lane C took for D2/D4/D9).

## K8 — the crack-back number on the seat's OWN turn

**What it was.** D9 (`wave58/lane-C.md`) gated `INCOMING THIS COMBAT` on the OPPONENT's turn, and
said so: "as does the seat's own turn". So on the pilot's own turn no incoming number rendered at
any life total. `123v162` s23: 3 life, four opposing bodies, every per-attacker tag reading
`[held back, it CANNOT block: ...]` and every relevant body `[tapped - cannot attack or block this
turn]`; the seat declared 13 non-lethal power and died to 8 the next combat. `123v152` s12 is the
Main-1 form: 5 life, 17 power of theirs untapping, no line. The reason a "can attack right now"
count cannot fix this is arithmetical: on the seat's own turn NOTHING of theirs can attack, so
that count is 0 on exactly the board that kills the pilot.

**What shipped.** Three pure functions beside D9's forms, and one render line inside the same
opponent-board walk that already computes D9's counts:

* `crackBackBodyUntaps(tapped, doesNotUntap, shackler, frozen)` — the untap step's OWN rule
  (`MTGInPlay::untapAll`): an untapped body always counts, a tapped one counts unless it does not
  untap. Summoning sickness is deliberately not a filter — a creature that arrived this turn
  attacks on their next one.
* `crackBackNextTurnLine(ableAttackers, maxDamage, myLife)` —
  `CRACK-BACK NEXT TURN: N of their creatures will be able to attack (tapped ones untap first),
  for up to M - you would be at K` + `; that would KILL you` when `K <= 0`. Same claim shape and
  the same under-claim rule as D9's forecast: an upper bound over able bodies, no trample
  carry-over claim, no blocker assigned (on this turn the seat has not yet decided which of its
  own bodies will still be untapped).
* `crackBackNextTurnDue(selfActive, phase, ...)` — the seat's own **attackers seam and both main
  phases** only; its untap, upkeep, draw, the rest of its combat and its end steps stay silent.

The bodies are counted in the loop that already walks `opp->game->inPlay`, with
`boardCreatureCanAttackNow(ac, false)` — the restriction-free predicate the board header already
uses off the active seat's turn — plus the untap rule. No new evaluation and no new pass. The
line can never share a window with D9's four forms (those require the opponent to be the active
seat), so it adds a line and replaces none. The literal is deliberately NOT `INCOMING THIS COMBAT`:
a corpus counting D9's line never counts this one.

## K9 — the `PUT:` line and the name gloss the protocol requires

**What it was.** `125v130` s83 answered `PUT: 9, 1 (Supreme Verdict, Fall of the Gavel)` — the
exact form the prompt asks for plus the name gloss every `CHOICE:` line in the same protocol
*requires* — and it scored `unparsed_reply`; the heuristic picked the discards. The mechanism is
`#W52-G`'s echo reconciler: a parenthetical is read as the echo of the ONE index in front of it,
so a two-name gloss matched no single row and the index it followed was dropped as a stale echo.

**What shipped.** `stripTrailingListGloss(line, namesOut)` (and its per-line wrapper): a trailing
parenthetical whose comma-separated parts number EXACTLY the bare indices on the line is the
list's gloss — it leaves the line the numbers are read from and is handed back as names. It is
applied in `parseAttackerSet` only under `echoBinds` (the PUT: grammars; the ATTACK grammar's
parentheticals are free-form prose and are untouched). The names are then reconciled as a SET,
not by position: every part must uniquely name one row, and only then may the gloss speak —
agreeing with the indices it changes nothing, naming a different set it WINS (the CHOICE
grammar's own echo rule), naming anything the rows do not hold it is dropped and the indices
stand. A gloss that does not pair 1:1 — a different count, a nested parenthetical, a prose tail, a
numeric part — is left exactly where the model wrote it and the wave-51 rules judge it as before.

**RED on base, measured.** The four K9 CHECKs were built and run with the one-line wiring
disabled (`stripAnnotationBrackets(content)`, the wave-58 expression) and the rest of the lane in
place: `3486 passed, 4 failed`, the failures being exactly s83's reply and its three siblings.
With the wiring restored: `3490 passed, 0 failed`. The two `#W52-G` regressions (`PUT: 9, 1` and
`PUT: 3 (Sphinx's Revelation)`) and `PUT: none` pass in BOTH builds — the shipped grammar is
untouched.

## K10 — 32 byte-identical asks in one turn

**Why the existing resolution does not fire.** `identical_option_asks_resolved` counts
`identicalInterchangeableRows` (#W54-D D8b), which is about rows identical to EACH OTHER inside
ONE ask — a different mechanism from a repeated ask, so it reads 0 on this shape and always
would. The two mechanisms that could have absorbed `126v130` seq 56-87 both miss for the same
reason: the state-plus-question cache keys on the rendered BOARD (`serializeGameState()`), and a
resolving Sanguine Bond / Exquisite Blood drain moves the life totals every iteration, so the key
is fresh 32 times; and the model-owned HOLD (#W53-N D2, row-only since #W56-A D1) only replays a
hold the model itself took, and this seat kept taking the DECLINE row.

**What shipped.** A latch on the seat's own last answer, beside the cache and read one line after
it: `repeatAskKey(turn, phase, decision, rows)` and the pure predicate
`repeatAskAnswerStands(...)`. The answer is re-served only while every clause holds, and every
clause is a fact about what the MODEL was shown, never about the board the engine holds: the same
turn, the same phase, the same decision line, the same printed rows byte for byte, the same plan
(`mCurrentPlan`) it wrote that answer under, and a VALID choice in range (a fallback is never
re-served). Anything else clears the latch and the window is asked. A row whose printed price
moved, a newly offered row, a row that disappeared, a rewritten plan, a new phase or turn all
re-open it — the #W56-A D1 rule, applied to the ask key. Counted onto the gameend record as
`identical_ask_answers_reserved` (a NEW field; `identical_option_asks_resolved` keeps its wave-54
meaning and is not mixed) and onto the per-seat closing stderr line, plus a `DebugTrace` per
re-serve. Nothing is deleted, no cap is added: the pilot is asked the moment a row or the plan
moves.

## Gate

Hermetic build (`rm -f bin/wagic && make -f Makefile.sdl -j4`), then a detached unit under
`MemoryMax=4G` / `MemorySwapMax=0`, `WAGIC_TESTSUITE_THREADS=1` (ground truth):

- suite **1245 tests, 0 failed**, **0 timed out**; **61 AI tests, 0 failed** (base: 1245 with 2
  concurrency-only failures at the default thread count, 0 at THREADS=1; 61 AI/0)
- PARSETEST **3490 passed, 0 failed** (base 3455; **+35** = K8 10 + K9 15 + K10 10 — the sum is
  exact, nothing superseded, nothing deleted)
- `git diff | /usr/bin/grep -c $'\357\277\275'` = 0

## Predictions for the wave-59 corpus (falsifiable, one per item)

- **K8** — the seat's own attackers-seam and Main 1/2 windows with ≥ 1 opposing creature that
  will untap and ≥ 1 power, carrying no `CRACK-BACK NEXT TURN` line: **0 of N** (wave 58: every
  one of them, at every life total). And the negative: the literal appears on **0** windows of the
  opponent's turn, and on 0 of the seat's own untap/upkeep/draw/blockers/end/cleanup windows.
- **K9** — `PUT:` replies that carry a trailing name gloss with one name per index and are scored
  `unparsed_reply`: **0 of N** (wave 58: 1 of 71 `PUT:` asks, `125v130` s83; the other unparsed
  `PUT:` reply, `125v152` s119, is degenerate output and is predicted to STAY unparsed).
- **K10** — in any turn where a decided loop hands the seat priority repeatedly on a byte-identical
  menu, `identical_ask_answers_reserved` on that seat's `gameend` is ≥ (run length − 1) and the
  maximum `[you declined this exact list N times already this turn]` in that game falls below 5
  (wave 58: 32 and 20, with the counter at 0). The corpus-wide `ask` count for a seat that wins
  through such a loop falls by the loop's iterations and by nothing else — a drop in windows where
  the rows DID move is a lane break.

## What I did NOT verify

- **No live run, no corpus, no model call.** Everything here is proven as pure functions plus a
  read of the call sites; K8's line has never been seen in an actual rendered prompt, and its
  token cost on a wide board is unmeasured (three windows a turn on any board with a live
  opposing body).
- **K10 is not exercised by the test suite at all**: `askModel` returns at `mEndpoint.empty()`
  before the latch, so the suite's 1245 tests prove only that nothing else regressed. The latch's
  behaviour on a real seat — in particular whether `mCurrentPlan` is stable across the windows of
  one drain loop, which is the clause the fix leans on — is first observed in the wave-59 corpus.
- **K10's interaction with the async/pending path is untested live.** A re-served answer writes no
  translog record (it follows the state+question cache's own convention), so a corpus reading only
  `ask` records sees the window as absent; the count is on the `gameend` record and the stderr
  line, nowhere else.
- **K8 counts a body that will untap, not one that will be ABLE to attack next turn.** An effect
  that will remove or tap it before their combat, a "can't attack" that is applied on their turn
  only, or a creature that will be sacrificed to its own upkeep cost are all still in the number:
  it is an upper bound, deliberately, and is worded as one.
- **K9's set reconciliation trusts `uniqueNameMatch`**, the same matcher the CHOICE echo rule
  uses; a gloss whose names are ambiguous across two rows is dropped rather than guessed, and I
  did not measure how often that happens on real reveal lists (this lane saw the shape once).
- I did not read the deck123/125/126 **guides**, did not touch any other lane's item, and did not
  re-derive the wave-58 census numbers quoted above — they are the reviews' own.
