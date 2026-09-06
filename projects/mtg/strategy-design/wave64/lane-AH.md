# Wave-64 lane AH — F1, F2, F5, F10, F11 (verdicts and facts that lie by construction)

Base: master `be2e88282` (wave-63 close). Branch `w64-lane-AH`, worktree
`worktrees/lanes/w64-AH`. **One file changed: `projects/mtg/src/AIPlayerGPT.cpp`.**
Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); no kill under the cap on any
run. The gate ran detached as `w64-AH-gate2` with a foreground until-loop wait.
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0** after every edit (all splices python rb/wb).

Environment note for the merge: this worktree's seeded `bin/User` was a partial copy, and the
suite binary then sits in the menu forever instead of auto-launching (it reads exactly like a
slow suite — one scoped test did not finish in 110 s, on the ARCHIVED BASE binary as well as on
this lane's). The orchestrator replaced `bin/User` from master; the gate below is the run after
that. Nothing in this lane's diff was implicated.

---

## What shipped

### F1 — the "best X" verdict now knows the library (deck125 HIGH-1)
`125v126` seq 527/528: `[<- best X for this cast: X=20 - largest affordable X - X=20 gains 20
life and draws 20 cards; no listed X does more]` on a 12-card library. The seat took X=20 from 70
life, 47 ahead, and decked itself. The badge is the most obeyed annotation this render produces
and it was computed from MANA alone, while `Your library: 12 cards` sat ~3 kB earlier in the same
prompt with nothing linking it to the option.

The library is a SECOND ceiling on the same ladder, and it is now computed on the same rails the
NET-life ceiling (#W61-S C10) already uses. `xLibraryReserve` reads the seat's own library count
and the draws standing between this cast and its next chance to act: ONE for the draw step —
which F10 below establishes on the same screen is not a choice — plus one per card of the seat's
OWN whose script carries `@each my upkeep: ... draw:N` (Staff of Nin, `mtg.txt:112426`, which is
the refill deck125's review names). `xLibraryCeilingX` folds that to the largest listed X the
library pays for; `xLibraryCeilingClause` states it with the arithmetic spelled out, in the two
distinguishable shapes — an X that draws PAST the library ("you would be asked to draw from an
EMPTY library and LOSE the game as that draw is attempted") and one that empties it and leaves
the compulsory draw step to finish the job. Both call sites of `xMonotoneMarker` carry it: the
CAST row (`xSpellPricing`) and the ANNOUNCE_X menu (`xAnnounceRowKills`) — seq 528, the screen
the pick was actually made on, is the menu.

Nothing is deleted: the affordable cap still prints, `no listed X does more` still prints (it is
true), and the clause corrects the verdict after them. What changes is the one word that reads
as an instruction — `xCastRowBestXMarker`'s `best X for this cast:` prefix now names the library
ceiling, so an X that decks the seat is never called best.

Deliberately NOT done: the MENU's marked row is still the cap row (`markX = capX`), because the
badge's own body describes capX and moving it would put a capX description on a different row.
An optional or conditional upkeep draw (`may`, `restriction{`) is not counted — an under-count
leaves the ceiling generous, which is the direction that cannot cost a legal line.

### F2 — the pass-row fact is read off the rows, not off a flag (deck125 HIGH-2)
`kNoPassRowFact` ("this ask has no pass row") rendered on **680 of deck125's 843 windows that DO
end in `Cast nothing right now` / `Hold priority`** — a sentence contradicting a row printed 12
lines above it, on 4 decisions in 5, on the surface the trust doctrine says the model must
believe. Wave-62's `declineRowOffered` is passed by the CHOOSE_MENU family alone (`:32875`); the
cast seam (`:30706`), the priority/land-drop seam (`:29737`) and the target seam never set it.

Passing the flag at each of those seams would fix the three windows we know about and leave the
next seam to forget it again. The claim is instead DERIVED from the rows the window renders:
`declineRowText` (prefix-matched at a word boundary, so `Cast nothing right now (combat comes
next this turn)` and the whole hold row both read as declines and `Passenger Pigeon` does not),
`declineRowIndexOf` (scanned from the bottom, because the house ordering rule puts declines
last), and `declineFactForMenu`, which returns one of THREE true sentences: the wave-62
`kDeclineRowFact` when the decline is last, a new `(row N of this menu declines: ...)` when the
grouping pass moved it, and `kNoPassRowFact` when there genuinely is none (damage order). The
caller's flag is KEPT as the fallback for a decline row worded in a way the scan does not know
(the Intrepid Adversary `Don't add any counter` shape), so nothing wave 62 got right is lost.

The fact line is inside the ask key. It is derived from the rendered rows only — no counter, no
clock, no running total — so it cannot differ between two rebuilds of the same window
(wave61/corpus-livelock.md).

### F5 — the stack-trap NOTE derives its verb and its decline sentence (deck152 HIGH-3)
`AIPlayerGPT.cpp` `chooseTarget` emitted, unconditionally, "pick a battlefield permanent that is
worth **bouncing**, or **decline**." `152v125` seq 45 was Fateful Absence — *Destroy target
creature or planeswalker* — with two rows, both the seat's own creatures, and NO decline row. The
model had just written "I must accept this loss", went looking for the decline the note
advertised, and destroyed its own 6/6 Sigarda. Two false claims in one sentence.

`stackTrapEffectGerund` maps the verb off the ability the engine is actually waiting on — the
same `getMenuText()` the forced-loss detection twenty lines below already trusts, and the same
zone-aware `getMenuText(tc)` for a move, so "Bounce" is claimed only when the destination is a
hand and the chooser reads the battlefield. An unrecognised label yields "" and the note then
makes NO verb claim ("worth targeting with this") — a wrong verb here is strictly worse than an
unnamed one. `stackTrapNoteText` takes the decline sentence from `mayStop`, the same flag that
decides whether `Done - no further targets` is pushed onto `opts` twelve lines earlier: where the
escape exists it is NAMED; where it does not, the note says so and states the choice that
actually remains ("pick the one you can most afford to lose"). Everything the wave-20 note was
built for — the on-stack spell is not a legal target — is byte-preserved.

### F10 — the draw step is not a choice (deck130 HIGH-1)
The punisher paragraph closed "Count that cost before choosing to draw." `162` seq 35 (turn 12,
10 life, three punishers) read that as an invitation to find the row that declines; the only row
that sounds like one is the hold, and the seat took it at TEN straight windows under the plan
"hold priority through main phase to avoid drawing", drew every turn regardless, and went 16 -> 1.

`drawPunisherSummaryText` now states the bound before it states the invitation: *"Your DRAW STEP
is COMPULSORY: no row on this or any other menu declines it, and holding priority does not
prevent it - the cost above is charged whether you act or not."* The "before choosing" framing
survives, SCOPED to the draws a row really does control (a cycling ability, a draw spell, any
extra draw). `drawStepForecastText` — the pilot's own forecast line, the one that carries the
number — carries the same fact on the same line, between the number and the loop clause. The
opponent's mirror (`theirDrawStepForecastText`) does NOT: the pilot declines nothing on their
draw step and a claim about their rows would be out of scope.

### F11 — castable bodies and declared attackers, against the crack-back line (deck130 HIGH-2, MED-3)
`162` seq 43, at 1 life: `CRACK-BACK NEXT TURN: 2 of their creatures ... for up to 6 - you would
be at -5; that would KILL you` sat five lines above `2. Cast Siege-Gang Commander {3}{r}{r} (2/2)
{leaves 0 of your 5 untapped mana sources untapped}` and the card text promising three Goblins.
Nothing on the row connected the four bodies to the line that kills the seat; the seat
confabulated a punisher trigger (the primitive draws nothing) and passed.

`crackBackAttackerPowers` is the per-attacker half of the SAME walk `crackBackTotalOver` sums, so
the total the line prints and the cover a row claims are literally the same bodies.
`castBodiesAdded` counts the blocking bodies the cast puts on the board: the creature itself, plus
the creature tokens its untriggered / `@movedTo(this|battlefield)` lines create — read through
`AutoLineMacro::Process`, because Siege-Gang's whole rider is the macro `_GOBLINTOKEN_*3`
(`mtg.txt:105072`) and a raw scan sees an opaque token (the N-158m lesson, same file). Conditional
and optional forms are skipped: an under-count keeps the claim conservative.
`crackBackBlockerRowTag` renders it on the cast row, gated through `crackBackScreenTotal` (so a
row can never point at a line that is not above it) and on the LETHAL case only — a clause on
every cast row is a clause nobody reads. It restates the line, names the bodies, states the rule
that lets them block (`summoning sickness stops attacking, not blocking`), does the arithmetic
(each blocker stops at most ONE attacker, so B bodies cover at most the B LARGEST), gives the
resulting life and the survive/kill verdict, and says what the ceiling EXCLUDES (evasion, and any
removal they draw) with a pointer to the per-attacker tags that price it.

MED-3, the same defect from the other side: the crack-back figure on the ATTACKERS window is
identical whether the seat attacks with 0, 1 or 2 bodies, so no number on the screen moved when
the choice that killed it was made (rerun seq 45, died at -4). The attackers tail now carries
`CRACK-BACK COST OF ATTACKING` — on a lethal crack-back only — saying that the figure above does
not move but what is left to BLOCK it with does, that declaring an attacker taps it and a tapped
creature cannot block, that each blocker kept back stops at most one attacker, and how many of
the offered attackers have VIGILANCE and stay untapped.

`{crack-back cover: ` joins `stripNarrationDecoration`'s drop list: it prices a board that is
true while the window is open and false the moment combat happens.

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), qmake untouched, no new sources.
Detached unit `w64-AH-gate2`, `MemoryMax=4G` / `MemorySwapMax=0`; no cap kill.

| run | result |
|---|---|
| PARSETEST | **`=== self-test: 4406 passed, 0 failed ===`** (baseline 4344/0 measured on this worktree's own base build; **+62 cases**) |
| suite, default threads | **1268 tests (2 failed), 66 AI tests (0 failed)**; `==Test Failed !==` **2**, `==Test timed out` **0** |
| the 2 failures | `lifeline.txt` and `merrow_reejerey.txt` — the brief's known concurrency-only pair, named from the log |
| suite, `WAGIC_TESTSUITE_THREADS=1` | **1268 tests (0 failed), 66 AI tests (0 failed)**, 0 timeouts — **0 new failures** |
| `intrepid_adversary_repeated_payment.txt` | passed in both runs of this gate (lane AE's item; its rate was not exercised here) |

Test count 1268 -> 1268: this lane ships **no suite fixture, and claims none**. All five items are
RENDER / classification changes — no item changes what the engine DOES with a card, an option or
an answer, so no fixture can be RED on base. (The one place a behaviour could have moved is the
ask KEY, which the fact line sits inside; it is derived from the rendered rows only, so the key is
as deterministic per window as it was before.)

### RED-on-base evidence — measured, not asserted
Produced the way lane AC produced it: every mechanism reverted to its wave-63 behaviour with the
new cases in place, and the binary rebuilt. Reverted: `xMonotoneMarker`'s library clause;
`xCastRowBestXMarker`'s ceiling-clamped `bestX`; `declineFactForMenu`'s row scan (flag-only);
`stackTrapNoteText`'s verb and decline sentence (restored to the wave-20 literal);
`drawPunisherSummaryText`'s closing sentence; `drawStepForecastText`'s compulsory clause;
`crackBackBlockerRowTag` emptied.

Result: **`4381 passed, 25 failed`** (log `~/.gatelogs/w64-AH-red.txt`). **19 of the 25 are this
lane's own new cases** — F1 x3, F2 x3, F5 x4, F10 x4, F11 x5 — and the other 6 are pre-existing
wave-49/50/60/62/63 cases that were UPDATED (never deleted) to the strings F10 changes, which is
why they go RED under the reversion too. The remaining 43 new cases are pure-unit,
MUST-NOT-MATCH, echo-shape and defaulted-call pins that pass on base as well, which is what they
are for.

### Cases updated, none deleted
`#W49-U D6` x2, `#W50-X D16`, `#W60-N B6`, `#W62-X D8 NEGATIVE`, `#W63-AC E13 MUST-NOT-MATCH` —
all six are F10's two strings, re-pinned to the new wording with the invariant each case exists
for intact (the E13 case still pins that ONE sentence closes the paragraph and governs no
number; the sentence is now the optional-draw scoping).

---

## Predictions (falsifiable, for the wave-64 corpus)

1. **F1** — every `[<- ... largest affordable X ...]` badge whose spell draws, on a window whose
   own board block prints a library smaller than `capX * drawPerX + 1`, carries `LIBRARY
   CEILING:`; and **0** cast rows corpus-wide read `best X for this cast: X=N` where `N *
   drawPerX` exceeds the library that window printed minus one. **Falsifier**: one such row, or
   one deck-out taken off a badge that carried no ceiling clause.
2. **F2** — `(this ask has no pass row)` occurrence count on windows whose option list contains a
   row beginning `Cast nothing right now`, `Play no land right now`, `Hold priority`, `Decline -`
   or `Done - no further targets`: **0** (from 680 of 843 in deck125's six games). The damage-order
   and multi-pick-minimum asks still carry it. **Falsifier**: one window carrying both a decline
   row and the no-pass-row sentence.
3. **F2, second order** — deck125's hold-take rate over `[you declined this exact list N times
   already this turn]` windows rises above wave-63's 4.6% (390 tagged / 18 taken). This is a
   PREDICTION about the model, not a claim about the mechanism; the mechanism's own falsifier is
   prediction 2. **Falsifier**: a rate at or below 4.6% over a comparable window count.
4. **F5** — every `NOTE: these targets are battlefield permanents only` names a verb matching the
   spell's own text on the same screen, and **0** notes say `bouncing` on a screen whose card text
   says Destroy/Exile/Sacrifice. Every such note on a list with no `Done - no further targets` row
   carries `There is NO decline row on this list`, and **0** notes advertise a decline on a list
   that has none. **Falsifier**: one of either.
5. **F10** — every `DRAW PUNISHERS on the battlefield` paragraph carries `Your DRAW STEP is
   COMPULSORY`; `Count that cost before choosing to draw.` (the unscoped sentence): **0**
   occurrences. Every `DRAW FORECAST: your ...` line carries `This draw step is COMPULSORY`, and
   **0** `DRAW FORECAST (theirs)` lines do. No reply in the corpus states a plan of holding
   priority in order to avoid a draw STEP. **Falsifier**: one of any.
6. **F11** — every cast row for a card that adds a body, on a window whose `CRACK-BACK NEXT TURN`
   line reads `would KILL you`, carries `{crack-back cover:`; **0** such tags appear on a window
   with no crack-back line or a survivable one; every tag's `N of M` figure is ≤ the crack-back
   total that same window printed. Every ATTACKERS window with a lethal crack-back carries
   `CRACK-BACK COST OF ATTACKING`. **Falsifier**: one tag on a survivable header, or one lethal
   crack-back attackers window without the clause.

---

## What I did NOT verify

- **No live model probe was run and no game was driven.** PARSETEST, the suite and source reading
  are the whole gate. Nothing here has been seen by a model; every prediction above is a
  prediction, not a measurement.
- **F1's reserve scan is not enumerated against the pool.** `@each my upkeep` + `draw:N` was read
  off Staff of Nin's primitive and the two shapes (`may`, an opponent-directed draw) are excluded
  by hand. I did NOT enumerate which other cards in the pool carry a compulsory own-upkeep draw,
  and a draw owed by some other route (a stack-borne draw, a replacement effect, another player's
  Howling Mine) is NOT in the reserve — the ceiling is then one card too generous, never too
  tight. The deck125 review's wording is "untapped Staff of Nin"; a triggered upkeep draw does not
  require the permanent to be untapped, so this scan does not filter on tapped state, which is the
  rules-correct reading and a deliberate divergence from the review's phrasing.
- **F1's menu badge still marks the CAP row.** `markX` is unchanged, so on the ANNOUNCE_X menu the
  clause rides row 1 (X=capX) and names the safe rung in prose rather than moving the marker onto
  it. Whether the model follows a named rung as reliably as a marked row is untested.
- **F1's `[<- best X ...]` prefix clamp is proven at the FUNCTION, not on a board.**
  `xCastRowBestXMarker` is pinned with a hand-built `XVictimSurvey`; I did not render it against a
  live 12-card library.
- **F2's word-boundary scan is a LITERAL list.** A future emitter that words its decline row
  differently falls back to the caller's flag, and where the caller passes false it will read
  `kNoPassRowFact` again. That is the wave-62 failure mode, narrowed but not closed by
  construction; closing it needs the row to be marked at the point it is pushed, which is a
  different change across five seams.
- **F2's composed-window case composes the fact and the rows in PARSETEST, not through
  `askModel`.** `askModel` takes a live board, so no pure case proves the call site; the wiring is
  argued from the single edit at the one place the fact is emitted, and prediction 2 settles it.
- **F5's verb table is not enumerated against every `getMenuText()` in `AllAbilities.cpp`.** I read
  `AADestroyCard`, `AASacrificeCard` and both `AAMover` overloads; other ability classes whose
  label is not in the table render the no-verb wording, which is the intended failure direction.
  A GRANTED ability riding a nameless dummy will report its own label, not the granter's — I did
  not check that case against a board.
- **F5's `mayStop` is the target seam's own escape flag.** It is true only for a multi-target
  chooser past its minimum; a single-target chooser is correctly told there is no decline. I did
  not check whether any other row on that list (a `Cancel` the engine appends elsewhere) can make
  the "NO decline row" sentence false.
- **F11's `castBodiesAdded` is proven by reading the macro and the two script shapes, not by
  rendering it against a board.** It is not a PARSETEST case (it needs an `MTGCardInstance`), so
  the body COUNT is the untested half of F11; `crackBackBlockerRowTag`'s arithmetic is pinned pure.
  A token created by a shape this scan does not read (a `token(` spelling, an `if paid(kicker)`
  rider, a granted ETB) is under-counted, and the row's claim is then conservative.
- **F11's cover figure ignores evasion.** It is a CEILING and the clause says so and points at the
  per-attacker tags; it does not compute `canBlockPairwise` for the new bodies, because they do
  not exist yet. Menace is likewise not folded (a menace attacker needs two of the new blockers).
- **F11's attackers clause is not a pure function** — it is composed inline in `chooseAttackers`,
  so it carries no PARSETEST case. Its inputs (`crackBackScreenTotal`, the vigilance count) are
  each pinned or trivial, but the composed sentence is verified by reading only.
- **No cross-lane interaction was tested.** Lanes AG, AI and AJ touch adjacent emitters; the
  combined diff has not been built here. Two merge-time looks are worth it: `stripNarrationDecoration`'s
  drop list (one entry added at the tail of the same chain lane AC widened) and
  `askModel`'s format line, where the decline fact now comes from a helper rather than a ternary.
- **Key safety.** Everything added here is derived from the board, the script or the rendered
  rows — never from a counter, a clock or a running total of asks — so nothing new can differ
  between two rebuilds of the same window. The only new string that enters an ask key is the
  decline fact, and it is a pure function of the rows that are already in that key.
