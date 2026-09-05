# Wave-62 lane X — D2, D6, D8, D19 (combat aggregates under loops and forecasts)

Base: master `0d9b2d52e`. Branch `w62-lane-X`. Worktree
`worktrees/lanes/w62-X`. One file changed: `projects/mtg/src/AIPlayerGPT.cpp`.
Every wagic run memory-capped at 4 G with swap off; the gate ran detached under
`w62-X-gate`.

---

## What shipped

### D2 — the `ATTACK TOTAL` kill claim dies under an opposing life LOOP
`123v126` seq 52 printed *"that KILLS them whatever they block, gain included"*
(98 power, 95 guaranteed) beside the screen's own banner saying any life they
gain is fatal to the seat. Lane R's `blockGain` is a BOUNDED ceiling; with
Sanguine Bond + Exquisite Blood on their side it is not a ceiling at all — a
blocking lifegain trigger resolves in declare-blockers, before any of the damage
the figure counts, and the chain is unbounded. The corpus proves it in that game:
seq 73 → gameend, one point of gain took the seat 19 → 0 and them 20 → 39.

`attackTotalLine` takes `oppLifeLoop` (from `playerHasLifeLoop(oppL)`, the same
scan the banner is built from, so the line and the banner cannot disagree). With
`oppLifeLoop && blockGain > 0` the line keeps the damage floor and the life that
damage ALONE leaves, then says *"and that is NOT a ceiling: both halves of their
life LOOP are in play, so any life a block gains them chains without limit and
resolves before this damage. No kill claim is made from these figures"* — the
bounded "as high as N" figure goes with the claim it fed, because it was the
false half. Fail-closed in exactly C1b's shape: named reason, no verdict. With
`blockGain == 0` nothing changes (a loop a block cannot start is not this line's
business), and the defaulted call is byte-identical to wave 61.

### D6 — the crack-back ability gate, in both directions
*(a) Lolth, Spider Queen's -8 emblem, 17 renders.* Three independent errors, all
three fixed and each with its own case:

1. `activationManaCost` priced `{C(0/0,-8,Loyalty)}` as **one generic mana** via
   its `numeric ? digits : 1` fallback, so an ability needing 8 loyalty on a
   permanent standing at 1 read as free. A counter cost is not a mana cost:
   the head now returns -1 (unpriceable) and the caller claims nothing.
2. The amount `damage:8minusoplifelostminusend` is an EXPRESSION; the old test
   was `line[dp+7] >= '1'`, which passes on the leading `8`, so the render
   printed a flat 8. `crackBackFlatDamageAmount` requires digits followed by a
   delimiter, else 0.
3. The damage sits inside `emblem transforms((,newability[@combatdamagefoeof
   (player) …]))` — an emblem's TRIGGERED rider on their creatures' combat
   damage, so the clause's promise *"that no block stops"* is backwards.
   `crackBackDamageIsDirect` refuses any line with an `@` hook between the cost
   head and the damage.

*(b) the same gate dropping 53 TRUE clauses.* R6 priced a NEXT-TURN forecast
against THIS turn's open mana (Hive of the Eye Tyrant needs `{3}{B}` and the
opponent was never above 3 untapped sources; Siege-Gang `{1}{R}` against 0–1).
The gate is split: `oppNextTurnManaReach(opp)` walks the action layer's mana
objects with the same `FreeProducerPolicy` and the same one-per-source unit
`potentialColorReach` returns, replacing the tapped gate with the untap step's
own rule (a tapped source untaps unless it does not untap or is frozen), and
`crackBackFloorSources` prices against that. It falls back to the open-mana
figure when the layer cannot be read, so no clause appears that wave 61 would
not have printed from an unreadable board.

### D8 — the DRAW FORECAST's tense, its missing verdict, and the missing win tag
*(a) tense + verdict.* `126v162` seq 37 rendered *"your NEXT draw step draws 13
cards … = 26 life"* in phase Draw with all four draw triggers already on the
stack, two lines under `Your life: 20`, and carried no verdict while every other
lethal figure on that screen carried one. Both forecast emitters (the seat's and
the punisher-seat mirror, moved together so they cannot drift) take `stepIsNow`
and `holderLife`: the tense becomes *"your draw step, resolving NOW,"* when the
holder is standing in their own draw step, and the price gains *"- if it resolves
as forecast you would be at N"* plus `; that KILLS you` at or past 0. The verdict
is deliberately CONDITIONAL on the forecast resolving as printed: the step's own
card may already be drawn, and the count is not re-derived here.

*(b) the win tag.* No row in the corpus was ever tagged as winning while the loop
banner printed. `lifeLoopWinTail(myLoopClosed, amountKnownPositive)` states the
banner's own rule as a row verdict — *"and both halves of YOUR life LOOP are in
play, so that chains without limit until they are at 0: THIS WINS THE GAME"* —
and is wired at the two places the engine can prove it:

* `damagePlayerVerdict`, on a hit that leaves the opponent standing. Guarded by
  the same `winFoldBlockedTail` test the direct claim uses: a win the seat does
  not live to collect is not claimed, and the pilot's own row is never a win row.
* `edictClause`. At N=1 the gain is determined and carries the tail. At N>1 the
  victim is theirs to pick, so the row now states the FLOOR — *"you gain at least
  M (their smallest toughness)"* — which is what makes the verdict provable; that
  is the seq-37 repro exactly (Shield Sphere 0/6 and Master of the Feast 5/5,
  floor 5, opponent on 4, row 1 bare). Nothing prints when the gain goes to THEM,
  when the edict grants no life, or when the floor is not a real number.

### D19 — a removal row priced against the crack-back total above it
`130v152` s18: `CRACK-BACK … up to 10 … you would be at -1 or lower; that would
KILL you` over a target list with three `DIES` rows at power 1, 1 and 2. Killing
the 2/2 survives at 1; killing a 1/1 dies at 0. The seat had to subtract, and did
not.

The per-body half of the crack-back walk is extracted as
`crackBackBodyContribution` and now feeds BOTH the printed total and the row
subtraction, so the two are literally the same arithmetic.
`crackBackReliefClause` prints `{removes N from the CRACK-BACK total above:
T -> T-N - you would be at L}`, adding `; that still KILLS you` at or past 0 and,
on a total that rides the FLOOR wording, *", but that total is a FLOOR, not a
ceiling - surviving it is not guaranteed"* rather than a false all-clear. It
rides the damage-verdict and shrink-verdict rows only when the verdict is DIES,
only for a body the OPPONENT controls, and only when
`crackBackNextTurnDue(seat, phase, attackers, total)` says the line is actually
on the screen — the aggregate folds every trigger that gates the line it points
at.

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), every run under
`MemoryMax=4G` / `MemorySwapMax=0`; no cap kill.

- Suite (default threads): **1249 tests, 2 failed**, `==Test Failed !==` **2**,
  `==Test timed out` **0**, **65 AI tests, 0 failed**. The two are the brief's
  known concurrency-only failures, named from the log: `lifeline.txt` and
  `merrow_reejerey.txt`.
- Suite, `WAGIC_TESTSUITE_THREADS=1`: **1249 tests, 0 failed**, `==Test Failed
  !==` **0**, `==Test timed out` **0**, **65 AI, 0 failed** — 0 new failures.
- PARSETEST: **3958 passed, 0 failed** (baseline 3915/0; +43 cases).
- `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

### RED-on-base evidence
No item here changes what the ENGINE does with a card, an option or an answer —
all four are render/classification changes — so no suite fixture can be RED, and
none is claimed. RED was produced instead by reverting every mechanism to its
wave-61 behaviour with the new cases in place and rebuilding: `oppLifeLoop`
forced false, the loyalty guard disabled, `crackBackFlatDamageAmount` restored to
the leading-digit read, `crackBackDamageIsDirect` forced true,
`oppNextTurnManaReach` forced to -1 (the open-mana fallback = wave 61),
`lifeLoopWinTail` and `crackBackReliefClause` emptied, the edict floor zeroed,
and both forecasts forced to `stepIsNow=false, holderLife=-1`. Result:
**3936 passed, 22 failed** — 3 D2, 6 D6, 9 D8, 4 D19. The other 21 new cases are
NEGATIVE/defaulted-call pins that pass on base too, which is what they are for.

---

## Predictions (falsifiable, for the wave-62 corpus)

1. **D2** — 0 windows contain both `KILLS them` and `Both halves of their life
   LOOP are in play` in the same prompt. Every `ATTACK TOTAL` line carrying
   `life back across their` on a board with an opposing loop reads `NOT a
   ceiling: both halves of their life LOOP are in play`, and none of them reads
   `so blocking can leave them as high as`. Falsifier: one co-occurrence.
2. **D6a** — 0 `CRACK-BACK` renders name Lolth, Spider Queen (or any
   planeswalker) in the `aim ability damage at you that no block stops` clause;
   0 such clauses print a per-activation figure that is not a plain number in the
   source line. Falsifier: one planeswalker name in that clause.
3. **D6b** — on boards carrying Hive of the Eye Tyrant or Siege-Gang Commander,
   the share of `CRACK-BACK` lines carrying the `FLOOR, not a ceiling` clause
   rises against wave 61's 0 of 53. Falsifier: still 0 such clauses where the
   opponent's untapped-plus-untapping source count covers the animation or the
   activation cost.
4. **D8a** — 0 `DRAW FORECAST` lines read `your next draw step` in a record whose
   phase is `Draw` on the seat's own turn; every forecast whose priced life cost
   is at or above the holder's printed life carries `that KILLS`. Falsifier: one
   phase-Draw record with `next`, or one over-life forecast with no verdict.
5. **D8b** — at least one row per corpus carrying `THIS WINS THE GAME` off the
   loop tail, and 0 of them in a record whose seat is at or below the life loss
   printed on the same row (the survival guard). Falsifier: a win tail on a row
   the seat cannot survive to collect.
6. **D19** — every target row reading `DIES` for an opponent-controlled creature,
   in a record that also renders a `CRACK-BACK NEXT TURN` line, carries a
   `removes N from the CRACK-BACK total above` clause; 0 such clauses appear on a
   record with no crack-back line, and 0 on a target the seat controls.
   Falsifier: one of either.

---

## What I did NOT verify

- **No live model probe was run.** Fixtures and PARSETEST are the whole gate;
  nothing here has been seen by a model, so the wording's effect on behaviour is
  a prediction, not a measurement.
- **D6b is unmeasured against a real board.** `oppNextTurnManaReach` was proven
  only by inspection and by the fallback's byte-identity; no fixture exercises a
  board where the untap step changes the verdict, because the walk needs a live
  action layer. The direction of the change (more true clauses) is argued from
  the wave-61 census, not observed.
- **The 53-clause recovery number is wave 61's count, not a measurement of this
  build.** How many of the 53 the split gate actually recovers depends on the
  opponent's total source count, which the corpus did not record.
- **`damageTargetVerdict`'s LOG-narration call site (`AIPlayerGPT.cpp` ~22860,
  the "targeting …" line) was deliberately left alone** — it narrates a
  resolved action, not a decision row, so a crack-back relief clause there would
  price a choice already made.
- **D8's forecast COUNT is unchanged.** When the seat is already standing in its
  draw step the step's own card may already be drawn, so the printed `k` can be
  one card high; the verdict is worded conditionally for that reason, and the
  count was not re-derived (that is not this item).
- **D19 rides the damage and shrink verdict rows only.** Destroy-effect target
  rows, sacrifice rows and Siege-Gang's own sacrifice — which the deck130 seat
  names as the same shape — carry no verdict emitter to hang the clause on and
  were not given one.
- **No cross-lane interaction was tested.** Lanes W, Y and Z touch adjacent
  emitters; the combined diff has not been built here.
