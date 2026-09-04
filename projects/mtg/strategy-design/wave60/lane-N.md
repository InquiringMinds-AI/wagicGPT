# Wave-60 lane N — B5, B6 (punisher lenses)

Base: master `8b922abf0`. One file touched: `projects/mtg/src/AIPlayerGPT.cpp`.
All six changes are RENDER changes (prompt text the LLM opponent reads); no engine
behaviour, no rules resolution and no legality gate is touched, so the gate is
PARSETEST plus a no-regression suite run.

## What shipped

### B5 — a DISCARD-punisher lens beside the draw one

1. **The class split at the scan** (`converterClassOfScript`, `converterScanZone`,
   `converterScan`, `handConverterScan`). `converterScanZone` ran
   `drawPunisherClause` and `discardPunisherClause` into ONE `names` vector, so a
   `@discarded(...)` permanent was counted and named under a sentence that reads
   "the opponent draws N extra cards per turn". The scan now classifies each
   permanent (`kConverterClassDraw` / `kConverterClassDiscard`, both bits for a
   permanent that is both) and fills two lists. Nothing is dropped.
2. **`feedsRowTag`** takes the draw lists as before and two new discard lists. The
   count ahead of the names is now the DRAW punishers only; the discard punishers
   get their own clause that says what does and does not set them off (a draw is
   not a discard, though a larger hand can overflow a cleanup step). With no
   discard punisher anywhere the tag is byte-identical to the wave-57 shape.
   Repro fixed: `162v152` seq 13, `converters on your battlefield: 1 - Liliana's
   Caress` under a Howling Mine row (18 rows across that seat's six games).
3. **`discardPunisherScan` / `discardPunisherSummaryText` /
   `discardPunisherSituationLine`** — a `DISCARD PUNISHERS on the battlefield:`
   line in the CURRENT SITUATION block, on the same rails as the draw one (a
   conditional amount is NAMED, never numbered). It names the CLEANUP step
   explicitly as the discard that cannot be declined.
4. **`cleanupDiscardPriceClause`** — the arithmetic `125v162` seq 111 owed. Used on
   two surfaces: the X-menu row (`xLifeDrawRowCore` / `xLifeDrawRowAnnotations`,
   whose `NET` now subtracts the forced cleanup discard as well as the draw price)
   and the CAST row (`xSpellPricing`, `CLEANUP PRICE at X=N: ...` inside the
   existing `{X pricing: ...}` braces). `forcedCleanupInputs` reads the hand this
   cast leaves (the spell itself subtracted when it is still in the hand zone),
   the maximum hand size (`nomaxhandsize` suppresses the clause) and the
   opponent's per-discard price.
   The wave-59 row now reads `NET -53` instead of `NET 0` for X=9 at hand 9 /
   limit 7 / Caress x2.
5. **`cleanupDiscardHeaderText`** — the cleanup ask itself prices the discard it is
   about to force ("... cost you 44 life - you would be at -31; this KILLS you.
   You cannot decline the discard; you choose only WHICH cards go."). Defaults
   render the old bytes exactly.

### B6 — the loop the punisher numbers were counting past

6. **`loopCautionClause`** + wiring into `drawPunisherSummaryText`,
   `theirDrawStepForecastText` and `drawStepForecastText` (via
   `loopCautionForBoards`, which reuses `lifeToDamageConverterScript` /
   `lifeLossMirrorScript`, the same two predicates the LIFE-TO-DAMAGE CONVERTER
   paragraph uses). Every number stays; the sentence that follows it says what
   the completed pair does with it. Wording is taken from the two Oracle texts,
   both verified on disk and against Scryfall: Exquisite Blood `mtg.txt:37966-8`
   "Whenever an opponent loses life, you gain that much life"; Sanguine Bond
   `mtg.txt:99004-6` "Whenever you gain life, target opponent loses that much
   life". Repro: `162v126` seq 20, `DRAW FORECAST (theirs): ... = 5 x 1 = 5 life
   to you from your punishers above` with the pair already closed on their board.
7. **`pendingLoopWarningText`** + `loopPendingSituationLine` — the half-in-play /
   half-SEEN state. `converterSummaryText`'s own gate needs a CONVERTER in play,
   so `152v126` printed nothing for 25 consecutive decisions (seq 9-33) with
   Exquisite Blood on the opponent's battlefield and Sanguine Bond named in the
   seat's own frame. The new line fires from the moment both facts are known,
   reading the second half from the seat's remembered opponent hand
   (`mKnownOppHand`, the same list the frame prints) or from that player's
   graveyard / exile / own hand. It states explicitly that nothing has chained
   yet, so it can never be mistaken for the closed-loop block.

Every code comment is tagged `#W60-N (B5)` / `#W60-N (B6)`.

## Gate

Detached unit `w60-N-gate2`, `MemoryMax=4G`, `MemorySwapMax=0`, from
`projects/mtg/bin`, built with `rm -f bin/wagic && make -f Makefile.sdl -j4`.

- Suite, `WAGIC_TESTSUITE_THREADS=1`: **`Test suite finished: 1247 tests (0
  failed), 62 AI tests (0 failed)`**; `==Test Failed !==` **0**, `==Test timed
  out` **0**.
- PARSETEST: **`=== self-test: 3565 passed, 0 failed ===`** (base 3529 → +36
  cases, and one wave-59 case corrected rather than deleted).
- `git diff | grep -c $'\357\277\275'` → **0**.

The first gate attempt (`w60-N-gate2`'s predecessor) ended after 378 tests with no
summary line while lane M's suite and lane L's probe were running against the same
`~/.Wagic` user directory; re-run alone it completed. Nothing was changed between
the two runs.

### PARSETEST cases added
Two blocks, `[#W60-N B5]` and `[#W60-N B6]`, each carrying positives, MUST-NOT-MATCH
negatives and the echo shape:
- `converterClassOfScript` over the four real primitives + an empty script.
- the corrected repro row: `converters on your battlefield: 1 - Underworld Dreams #1`
  and MUST-NOT-MATCH `converters on your battlefield: 2`, with Caress still named.
- byte-identity of `feedsRowTag` and `cleanupDiscardHeaderText` with no discard
  punisher supplied, and of the wave-47 `{X pricing:}` row with no discard inputs.
- `cleanupDiscardPriceClause` exact string for the seq-111 board, plus its four
  empty branches and the singular/plural and life-not-supplied branches.
- `NET -53` on the X=9 row and MUST-NOT-MATCH `NET 0 life for this cast`.
- echo: the new clauses ride inside the existing `{X pricing: ...}` braces, the row
  still binds bare and fully-echoed, and `stripNarrationDecoration` leaves no residue.
- `loopCautionClause` / the three punisher emitters (with byte-identity when there
  is no loop) and `pendingLoopWarningText` incl. the MUST-NOT-MATCH that it never
  claims the loop is closed.

The wave-59 case at what was `AIPlayerGPT.cpp:43631` (`#W51-F D11`) baked the merge
in; it is FIXED, not deleted — `two` is now two draw punishers and the Caress
pairing is asserted in its split form in the new block.

No suite fixture ships: nothing here changes engine behaviour, so there is no
board-state assertion that could be RED on base. The RED-on-base evidence for these
items is the corpus itself (the four cited records) plus the PARSETEST cases, which
do not compile against base (`converterClassOfScript`, `cleanupDiscardPriceClause`,
`discardPunisherSummaryText`, `loopCautionClause`, `pendingLoopWarningText` and the
new `feedsRowTag` / `xLifeDrawRowAnnotations` / `cleanupDiscardHeaderText` arities
do not exist there).

## Predictions (wave-58 corpus, falsifiable)

- **B5-1.** Zero `{feeds: ...}` rows will name a `@discarded(...)` permanent inside
  the `converters on your battlefield: K` count. Wave 59: 18 such rows at deck162's
  seat alone. Any row whose K count includes a discard punisher falsifies it.
- **B5-2.** Every `{X pricing: X=n - ...}` row whose draws would put the caster's
  hand over its maximum hand size, with a discard punisher on the opponent's
  battlefield, will carry `CLEANUP PRICE` and a `NET` that includes it; no such row
  will print `NET 0 life for this cast`. Wave 59: 1 of 1 printed `NET 0`.
- **B5-3.** Every cleanup-discard ask taken with a discard punisher on the
  opponent's battlefield will state the life those discards cost. Wave 59: 0 of
  them did (52 cleanup discards at deck125's seat).
- **B6-1.** No `DRAW PUNISHERS` line and no `DRAW FORECAST` line will render
  without `LOOP CAUTION` while both halves of a life loop stand on the opponent's
  battlefield. Wave 59: 3 lines in one prompt (`162v126` seq 20) contradicted the
  converter block on the same screen.
- **B6-2.** The first decision at which one loop half is on a battlefield and the
  other is in a zone the seat has seen will carry `LOOP HALF PENDING`, not the Nth.
  Wave 59: 25 silent decisions before the warning (`152v126` seq 9-33).

## What I did NOT verify

- **No live probe.** No prompt was generated against a real board and no model was
  called; every claim above is PARSETEST-level plus the suite's no-regression run.
  In particular the board-facing wiring (`loopPendingSituationLine`,
  `forcedCleanupInputs`, `discardPunisherSituationLine`, `converterScan`'s new
  arity) is exercised only by compilation and by the suite's non-GPT paths — the
  PURE emitters are pinned, the scans that feed them are not.
- **`forcedCleanupInputs`'s hand count at ANNOUNCE_X.** I subtract the X spell from
  the hand zone if it is still there, which is correct under either engine
  ordering, but I did not observe which ordering the engine actually uses, and I
  did not check any replacement effect that changes maximum hand size mid-turn
  beyond `nomaxhandsize` / `handmodifier`.
- **The 126-game's actual chain direction.** Under the printed Oracle texts, a
  punisher of the pilot's own damaging the OPPONENT does not itself start their
  Sanguine Bond + Exquisite Blood loop (Exquisite Blood's "an opponent" is the
  pilot). The corpus nonetheless shows 16 → 0 / 24 → 40 in one draw step, which
  the wave-59 reviewer also could not fully attribute (`[-lifefaker]` unexamined).
  I therefore wrote the caution as what the PAIR guarantees from its own text and
  did NOT assert that the pilot's punishers feed the loop. If the engine's
  `@lifelostfoeof(player)` binding is in fact loose, that is a separate card-script
  / engine item and is untouched here.
- **`MTGCollection()->getCardByName(name, -1)`** for a remembered opponent-hand
  card resolves by name across sets; I did not check the behaviour for a name whose
  printings disagree in script.
- Whether any deck guide's prose depends on the merged K count that B5-1 splits
  (deck162's guide carries a hand-written Caress qualifier that the split now makes
  redundant rather than wrong; I did not edit any guide).
- Any other lane's items, and the wave-58 corpus itself (the predictions above are
  written for it, not measured against it).
