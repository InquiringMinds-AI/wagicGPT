# Wave-64 lane AG — F6, F7, F8, F9 (the blockers and modal renders that decided games)

Base: master `be2e88282`. Branch `w64-lane-AG`. Worktree `worktrees/lanes/w64-AG`.
One file changed: `projects/mtg/src/AIPlayerGPT.cpp`. Every wagic run memory-capped at 4 G with
swap off; the gate ran detached as `w64-AG-gate`. No cap kill. All four items are
render/classification changes — nothing here changes what the engine does with a card, an option
or an answer — so RED was produced the way wave-63 lane AB produced it (mechanisms reverted to
their wave-63 behaviour with the new cases already in place); see **RED-on-base evidence**.

---

## What shipped

### F6 — modal `choice` rows carry a price and a lethality verdict (deck146 HIGH-1)

`146v152` seq 48, at 1 life, offered `5. return creature and you draw` and nothing else. The
engine's `name(...)` label is an OMISSION, not a summary: the same script line carries
`life:-1 controller` (`borderline.txt:102744`; Scryfall agrees). Under the trust doctrine the
model owes the surface belief, so it read "you draw" as free, wrote a lethal-attack plan and
died at 0 before combat. Every other menu in this engine already carries a live verdict — cast
rows `{right now: drains N}`, ability rows `they would be at K; THIS WINS THE GAME`, the stack
line `that would KILL you`. This is R293/D1's rule on the one surface it never reached.

`modeEffectPriceTag(script, label, myLife, oppLife)` (pure) reads the magnitudes off the SCRIPT
the menu was built from, by the same `name(<label>)` match `tapUntapBranchTag` uses — never from
the label's words, because the label is exactly what is untrustworthy here. It rides each row in
the `CHOOSE_MODE` render beside the liveness and tap/untap tags; `req.optionTexts` (the
staleness key) and the row ORDER are untouched, so `act.choice` still means what
`applyMenuChoice` thinks it means.

Only two payload shapes are read, both unambiguous and both stating their player explicitly:
`life:<signed N> controller|opponent` and `draw:<N> controller|opponent`. A non-numeric
magnitude, an implicit player, or a payload inside an `ability$!...!$` block (whose actor is
another player — deck126's HIGH class) is left alone, and the tag says what it counts
(`life and draws only: anything else this mode does is in the row label and the card text`) so a
partial price is never read as a total. The seq-48 row now renders:

```
 {this mode right now: you LOSE 1 life - you would be at 0; THIS KILLS YOU; you draw 1
  (life and draws only: ...)}
```

### F7 — the GANG BLOCK verdict is DERIVED, not guarded away (deck146 HIGH-2)

`146v152` seq 20: `A1. Triumphant Adventurer (1/1) [first strike, deathtouch]` against three
candidates, each printing `(you kill it, your attacker lives)` 1-on-1, and NO `GANG BLOCK:`
clause — while any two of them kill it. The reply is the trap verbatim. 7 `GANG BLOCK:` strings
in that seat's corpus, all boilerplate, 0 live verdicts.

Two suppressors, both removed. `need` was accumulated from RAW POWER against raw toughness
(3 power ≥ 1 toughness → `need = 1` → `gangBlockPriceTag`'s `need < 2` guard returns ""), and the
whole claim was abandoned at the caller whenever the ATTACKER had first strike or double strike
or any candidate had deathtouch/wither/infect. Two computations disagreed about the same board
and the one that went silent is the one that would have stopped the attack.

`gangKillsAttacker` / `gangKillNeed` (both pure) model the same combat the 1-on-1 results model:

* a blocker with deathtouch and nonzero power kills the attacker outright, so the attacker must
  remove EVERY one of them or it dies;
* first strike / double strike on the attacker lets it remove blockers before they deal damage —
  its power is a budget and removing a blocker costs that blocker's toughness (1 apiece if the
  attacker has deathtouch). A blocker with its OWN first strike cannot be removed before it hits;
* WHICH bodies it removes is the attacker's own choice, so the budget is spent **optimally for
  the attacker** — an exact knapsack over the removable bodies, maximising the power it keeps off
  itself. The verdict therefore claims a kill only where the attacker's best defence still loses,
  so it can only ever under-claim. The DP is bounded (budget clamped at 64, at most 64
  candidates);
* wither/infect deal their damage as −1/−1 counters, which kill on the same threshold, so for
  THIS question they are ordinary power.

`need` is the smallest group of the DANGEROUS-first ordering that kills it; `anyOfThem` re-runs
the same test on the WEAKEST group of that size, which is what makes "any N" a fact. The
declaration minimum is the search's floor, so a group below it is never priced (the wave-63 code
could price one when there were fewer candidates than the minimum — that case is RED on base).
What still abandons the claim at the caller is what changes the meaning of "kill":
indestructible, persist/undying, a becomes-blocked pump, and any candidate whose damage to this
attacker is prevented. Both windows (attackers `mine=false`, blockers `mine=true`) use the one
function, so they cannot drift.

`gangBlockPriceTag` gains two shapes for what the derived math can now reach: `afterFirstStrike`
(the damage figure is what reaches it "past everything its own first strike can kill") and
`byDeathtouch` (the group kills it because a deathtoucher survives to deal damage, so **no**
damage total is offered as the reason — stating one would be false). A plain board renders
byte-identically to wave 63.

### F8 — the block suggestion says only what it did (deck123 HIGH-1, deck162 HIGH-1, deck152 MED-1, deck126 MED)

Three falsehoods in one clause, all the same class: the label claiming more than the computation
performed.

**(a) the material claim was unconditional.** Material only breaks ties INSIDE the maximum-life
set, so `123v146` s15 named the pairing that loses the blocker for nothing (`your blocker dies,
attacker lives`) over the one that kills the Goblin and keeps it, and `152v146` seq 18 claimed a
ranking over a single pure chump where there was nothing to rank. `blockAssignmentClause` now
takes `matchRank` — `blockPairMaterialRank`'s 4/3/2/1 for exactly the pairings it is about to
NAME — and states one of three heads: material-as-well-as-life only where every named pairing
survives (rank ≥ 3), `chosen for the LIFE ONLY - it does not preserve your material and no better
material reaches that life figure` where it does not, and `chosen for the LIFE ONLY (no material
ranking was performed on it)` where the ranks are absent. Where bodies are spent it NAMES them
(`- taking it SPENDS Thraben Doomsayer`), which is the fact `123v146` s11/s15 needed: both spent
the deck's only token-maker under a live Intruder Alarm.

**(b) it was obeyed as an instruction on NOT-lethal headers** (3 renders, 3 copied verbatim), two
lines above the header's own "block only where the trade favors you". Where declining leaves the
seat alive the clause now says so with the number: `This assignment is an OPTION, not an
instruction: declining every block leaves you at N and costs you no permanent`. On a lethal
header it is silent (declining is not an option there), so the two authorities in the window
never disagree.

**(c) the projection excluded the block-trigger gain the next line advertises** (`126v152`
seq 13: "you would be at 7", the seat finished that combat at 10). Blocking triggers resolve
before combat damage, so their life is part of the figure. The caller sums
`blockTriggeredLifeFor`'s CERTAIN half over the blockers in THIS assignment (a "may" gain is not
a figure a projection can rely on) and `incomingCombatLine` states both halves rather than
silently folding — `you would be at 7 before your blocking triggers, 10 after the 3 they gain
you` — so the reader can check it against the `BLOCKING THIS COMBAT` line below. The same number
now feeds the `no block saves you` badge and the lethal-screen branch: a trigger gain that lifts
the best case above 0 withdraws a death badge that would otherwise be false.

F8's fourth cited shape — `162` rerun seq 18, where the suggested block kills a DRAW PUNISHER
and hands 5 lifelink life, contradicting the same prompt's `KILLS them` forecast — is **not**
fixed here; see *What I did NOT verify*.

### F9 — survival is stated first, and the death verdict is scoped (deck123 HIGH-2)

`123v152` s31 read `you would be at -3; this KILLS you ... you would be at 1 AT BEST` — two
clauses that contradict each other left to right, the death claim first, phrased as a ceiling
rather than as survival. With 26 legal blockers printed the seat read the headline, answered
`BLOCKS: none`, and lost a won game. `this KILLS you` is a claim about DECLINING, and where a
block is proven to survive it is simply false of that window.

`incomingCombatLine` now decides that verdict from the best case it already holds:

* **exact branch** (`bestCaseOptimal`, a proven maximum): `; you SURVIVE at N if you block - this
  KILLS you ONLY if you decline every block`;
* **floor branch** (trample/menace in the total, so the figure is a CEILING on life and no
  assignment is proven to reach it): survival is NOT asserted, but the death claim is still
  scoped — `no assignment is PROVEN to save you, and none is proven to lose either: the least
  damage any block can leave puts you at N ... this KILLS you ONLY if you decline every block`;
* **no assignment survives** (or no bodies, or the loop branch): the bare `this KILLS you`, as
  wave 63 printed it.

One ordering detail, deliberate: where the unblockable split `(of that, D from A attackers none
of your creatures can block)` is printed, its "of that" refers to the unblocked-damage TOTAL, and
putting a second damage figure between the two would break the reference. In that ordering the
early position takes the SHORT scoped form (`; this KILLS you ONLY if you decline every block`)
and the survival sentence follows the split. Both clauses are true from their first word, they
never contradict, and the survival figure still precedes every ceiling below it. Both orderings
are pinned.

### Re-pinned PARSETEST expectations (deliberate, not drift)

Two wave-62 `#W62-Z D12` head-string equalities pinned the UNRANKED call still making the
material claim — which is deck152 MED-1's defect exactly. Both were replaced in place with a
`#W64-AG` comment saying why; neither was deleted.

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), detached unit `w64-AG-gate`,
`MemoryMax=4G` / `MemorySwapMax=0` on the unit and on every interactive run. No cap kill.

| | base (brief, `be2e88282`) | this lane |
|---|---|---|
| PARSETEST | 4344 passed, 0 failed | **4387 passed, 0 failed** (+43) |
| suite, default threads | 1268, 2 known concurrency + 1 known intermittent | **1268 tests, 3 failed**, `==Test Failed !==` **3**, `==Test timed out` **0**, 66 AI / 0 failed |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1268, 0 failed | **1268 tests, 0 failed**, `==Test Failed !==` **0**, `==Test timed out` **0**, 66 AI / 0 failed |

The three default-threads failures are the brief's own known set, named from the log:
`lifeline.txt`, `merrow_reejerey.txt` (concurrency-only) and
`intrepid_adversary_repeated_payment.txt` (the genuine intermittent). All green at THREADS=1,
which is ground truth. **0 new failures.**

`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

### Worktree defect found and fixed (not a code change)

The seeded worktree's `projects/mtg/bin/User/` held ONE file. Without it the binary never
reaches the `#ifdef TESTSUITE` auto-launch in `GameStateMenu` — it sits in the menu forever, at
~4 % CPU, with a block-buffered log frozen at 1,832 bytes, which reads exactly like a slow suite.
Proven environmental, not mine: the archived `wagic-0026a9d87` (the wave-63 close) hangs the
same way in this worktree bin and runs the same scoped test in ~20 s from master's bin. Fixed by
copying master's `bin/User` subtrees (all but the 12 G `sets/` image cache) into the worktree.
`bin/User` is untracked, so nothing about this is in the commit. **Lanes AH and AJ were sitting
in the identical hang at the same moment** — worth telling them.

### RED-on-base evidence

Nothing here changes engine BEHAVIOUR — all four items are render/classification changes — so no
suite fixture can be RED and none is claimed (the standing of wave-63 lane AB and wave-62 lane
X). RED was produced instead by reverting every mechanism to its wave-63 behaviour with the new
cases already in place and rebuilding: `modeEffectPriceTag` returning "" at entry, `gangKillNeed`
replaced by the wave-63 raw-power accumulation plus its caller guard (first strike or any
deathtouch candidate → no verdict), `blockAssignmentClause` restored to its unconditional
material head with the SPENDS and OPTION clauses suppressed, `blockTriggerGain` forced to 0, and
the header's verdict restored to the bare `this KILLS you`.

Result: **4364 passed, 23 failed** — 5 F6, 6 F7, 5 F8, 5 F9, plus the 2 re-pinned `#W62-Z D12`
head cases. The other 22 new cases are NEGATIVE / MUST-NOT-MATCH / echo-shape pins that pass on
base too, which is what they are for. The tree was restored from a byte copy taken before the
revert and rebuilt clean before the gate.

---

## Predictions (falsifiable, for the wave-64 corpus)

1. **F6** — every `CHOOSE_MODE` prompt whose source script carries an explicit
   `life:<N> controller|opponent` or `draw:<N> controller|opponent` on the matched mode renders
   `{this mode right now:` on that row; 0 such rows render bare. Every one whose controller life
   delta reaches the reading seat's life carries `THIS KILLS YOU`, and **0** rows carry
   `THIS KILLS YOU` for a delta aimed at the OPPONENT. Falsifier: one bare priced mode, one
   opponent-facing cost carrying the seat-kill tag, or one seat-lethal mode without it.
2. **F7** — 0 attacker rows list two or more candidates that provably kill the attacker under
   the same combat math the 1-on-1 results use while carrying no `GANG BLOCK:` clause. Every
   `GANG BLOCK:` on an attacker with first strike or double strike reads `past everything its own
   first strike can kill`, and every one decided by deathtouch reads `together KILL this attacker`
   with no damage total. Falsifier: one first-strike/deathtouch attacker with ≥2 lethal-in-group
   candidates and no verdict (the `146v152` seq 20 shape), or one damage figure printed as the
   reason on a deathtouch-decided verdict.
3. **F8a** — every `chosen for your blockers' material as well as for the life` clause is on an
   assignment whose named pairings ALL read `(you kill it, your blocker lives)` or
   `(neither dies)` in the same window's B rows; 0 appear on an assignment containing a chump or
   a trade. Every clause naming a pairing that dies carries `chosen for the LIFE ONLY` and
   `- taking it SPENDS <name>`. Falsifier: one material claim over a named chump (`152v146`
   seq 18's exact shape).
4. **F8b** — every `INCOMING THIS COMBAT` best-case figure rendered in a window that also prints
   a `BLOCKING THIS COMBAT` line with a certain gain carries the `before your blocking triggers,
   N after` pair, and that N equals the header's figure plus the certain gain of the blockers the
   assignment names. 0 windows carry `no block saves you` where that after-figure is above 0.
   Falsifier: one of either.
5. **F8c** — every `one legal assignment that reaches it` clause on a header whose own life
   arithmetic is survivable without blocking carries `This assignment is an OPTION, not an
   instruction`; 0 lethal-header clauses carry it. Falsifier: one of either. Secondary (the
   behaviour this is about, not a render fact): the verbatim-copy rate of the named assignment on
   NOT-lethal headers falls below wave 63's 3 of 3.
6. **F9** — 0 `INCOMING THIS COMBAT` lines contain an unqualified `; this KILLS you` in the same
   render as a best-case or AT-BEST figure above 0. Every such line reads either
   `you SURVIVE at N if you block` or `no assignment is PROVEN to save you, and none is proven to
   lose either`, and every `this KILLS you` beside one of them is scoped
   `ONLY if you decline every block`. Falsifier: one co-occurrence of an unscoped death verdict
   with a positive survival figure (the `123v152` s31 shape).

---

## What I did NOT verify

- **No live model probe was run.** Fixtures and PARSETEST are the whole gate; nothing here has
  been seen by a model, so every effect on behaviour is a prediction, not a measurement.
- **No corpus render was captured.** All four emitters were exercised only through their pure
  builders and the real Silverquill Command primitive text; how many wave-64 windows each fires
  on is unmeasured.
- **F8's deck162 half is NOT fixed.** `162` rerun seq 18 — the suggested block kills a DRAW
  PUNISHER and hands 5 lifelink life, invalidating the same prompt's `that KILLS them` forecast —
  needs the suggestion to recompute the DRAW FORECAST for the line it names, which is a different
  emitter (lane AH owns the forecast block this wave) and a cross-emitter dependency I did not
  build. What shipped makes the clause's own claims true and marks it optional; it still has no
  term for what the block does to another paragraph's arithmetic. This is the one cited F8 shape
  left standing, and it should go back on the ledger.
- **F6 does not cover the Lolth `+0: draw card and lose life` loyalty row** named in the same
  finding (`146v152` seq 45). That row is a CHOOSE_MENU ability row, not a mode row, and its
  magnitude is not on a `choice name(...)` line; pricing it is a different seam.
- **F6 reads only `life:` and `draw:`.** Damage, sacrifice, discard and every targeted effect are
  unpriced by design (their magnitude needs a chosen target), and the tag says so. A mode whose
  real cost is damage-to-self will still render only its label.
- **`modeEffectPriceTag` was proven against ONE real primitive** (Silverquill Command, all nine
  lines) plus synthetic shapes. Other modal cards may write the same effects in forms the parse
  does not read; those render exactly as wave 63 did (silence), never wrongly.
- **The F7 knapsack is proven pure, not on a board.** `GangBlockerStat` is filled from
  `basicAbilities` and printed P/T at both call sites; if those disagree with what
  `combatTradePreviewStats` sees for some pairing (a becomes-blocked pump is excluded, a
  prevention shield abandons the claim, but e.g. protection subtleties are not re-derived), the
  verdict inherits that error rather than adding one.
- **F7 says nothing about damage ASSIGNMENT ORDER beyond the attacker's own optimum.** The model
  assumes the attacker assigns its first-strike damage to whichever bodies help it most; a
  defender who orders blockers differently is not modelled, and cannot make the verdict wrong in
  the claiming direction (the verdict is the attacker's best case).
- **F7's `anyOfThem` wording is over the LISTED candidates**, as in wave 63 — a body that could
  not legally block this attacker is out of the list and out of the claim.
- **The F8b trigger gain counts the CERTAIN half only**, over the blockers the assignment names.
  A "may" trigger the script might take is in neither the projection nor the badge; the header
  therefore under-states the reachable life on a may-only board, deliberately and in the
  fail-closed direction.
- **F9's floor branch asserts no survival.** Where trample or menace is in the total, a seat that
  could in fact survive is told only that nothing is proven either way. That is the honest read
  of a ceiling and it is stricter than the ledger's "state survival affirmatively first" — the
  affirmative form is reserved for the exact branch, where it is provable.
- **No cross-lane interaction was tested.** Lanes AH and AI touch adjacent emitters (AH owns the
  DRAW FORECAST and crack-back pricing this lane's F8 note points at); the combined diff has not
  been built here.
