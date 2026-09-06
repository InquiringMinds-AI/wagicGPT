# Lane AK — the nine findings of wave64/codex-review.md

Base `52fcb617d` (master after the AG-AJ merge, `95b0aa152`, plus the review commit). Branch
`w64-lane-AK`, worktree `worktrees/lanes/w64-AK`.

Every finding was checked against the code BEFORE anything was changed. **Eight are CONFIRMED and
fixed in the general form; one (#3) is REFUTED as out of scope** — the wave-64 diff does not touch
the transport classifier, which the brief made the test for that finding.

## CONFIRMED / REFUTED

| # | Sev | Verdict | Evidence (checked first) | What shipped |
|---|-----|---------|--------------------------|--------------|
| 1 | HIGH | **CONFIRMED** | `DecisionContract.cpp:22` built the candidate set as `if (!card->isAttacker() && card->canAttack())` — the PLAYER predicate. `MTGCardInstance::canAttack(bool pwcheck)` (`MTGCardInstance.cpp:1030-1041`) checks `CANTATTACK` + `FLYERSONLY` when `pwcheck` is false and `CANTPWATTACK` when it is true, and `MTGPlaneswalkerAttackRule::isReactingToClick` (`MTGRules.cpp:2484`) gates on `canAttack(true)`. So a creature under Form of the Dragon's `flyersonly` (`mtg.txt:42987`), or any `CANTATTACK` without `CANTPWATTACK`, is a LEGAL planeswalker attacker the engine offers a human and the contract deleted before any row was built. Both later guards (`AIPlayerGPT.cpp:38426`, `DecisionContract.cpp:104`) dropped it again. | The candidate walk runs AFTER the target walk and admits a creature that may attack EITHER destination; `DecisionRequest::candidateMayAttackPlayer` / `candidateMayAttackTarget` carry which, parallel to `candidates` (permuted with them in `chooseAttackers`). `attackerDestinationTag` puts the fact on the row (`[may NOT attack them: ... only when you write it as A#>W#]`); the declaration guard asks the DESTINATION's predicate; `applyDeclareAttackers` asks `canAttack()` only on the route that sends a creature at the player. The damage-to-their-life aggregate excludes such rows and `walkerOnlyExclusionLine` says so. |
| 2 | HIGH | **CONFIRMED** | `AIPlayerBaka.cpp:5550` — after `kMenuPassHoldMax` (24) consecutive menu-answering ticks the deferral fell straight through to `userRequestNextGamePhase()`. Nothing between the counter and the pass asked whether the seat still had a legal action, so a chained mandatory-choice sequence that resolved 25 menus under one priority lost whatever cast or activation stood after them. | The floor is gated on `LegalActionsOracle::hasAnyLegalAction(this)` — the engine's own phase-aware predicate, deliberately permissive — so the pass is forced only where it removes nothing. While a legal action remains the hold continues. Every firing is RECORDED: `mMenuPassForced` counts it and `writeTransLog` writes `menu_pass_forced` on the next record. |
| 3 | HIGH | **REFUTED as out of scope** | The brief's test: did the wave-64 diff move it? `git log -L 871,871:src/GptConfig.cpp` → last touched by `98ed9baef` (**wave 59**, lane H/K1); `git log -L '/^bool gptDeadlineMissed/,/^}/:src/AIPlayerGPT.cpp'` → last touched by `0f1213f37` (**wave 63**, lane AF/R9). Neither line is in `be2e88282..52fcb617d`. No wave-64 lane touched the transport classifier. | Nothing. The substantive point is noted below as a carried observation for a transport lane — curl does return `CURLE_OPERATION_TIMEDOUT` for `CURLOPT_CONNECTTIMEOUT_MS` as well as `CURLOPT_TIMEOUT_MS`, so wave 63's "code 28 IS our deadline expiring" is true of the request timeout and not proven of the connect timeout. Fixing it is a change to wave-63 code with a wave-63 rationale, which this lane was not given. |
| 4 | HIGH | **CONFIRMED** | `AIPlayerGPT.cpp:20093` (`crackBackBlockerRowTag`): cover was `for (i < powers.size() && i < bodies) covered += powers[i]`, then `if (myLife - left > 0) o << ", which you survive"`, then a disclaimer naming evasion as excluded. Run the shipped formula on the review's board (3 life, one 5/5 flyer, a ground cast row): `bodies=1`, `powers=[5]` → `covered=5`, `left=0`, `3-0 > 0` → **"which you survive"** for a combat the body may not legally enter. A footnote after a verdict cannot repair the verdict. | The tag takes per-attacker `CrackBackAttackerFact` records (power, `blockersNeeded` from `minBlockersRequired()`, and `coverable` from the ENGINE's `canBlockPairwise`) and a CHECKED/UNCHECKED body split. The FLOOR (checked bodies only) is what a survival verdict may rest on — more blockers can only lower the damage, so it cannot be overturned. When the floor does not survive and unchecked bodies exist, the optimistic CEILING is still printed and the row says **"THIS IS NOT A SURVIVAL VERDICT"**. `crackBackAttackerPowers` (the power-only walk) is retired with its single caller. |
| 5 | HIGH | **CONFIRMED** | `lineIsCodedAnswerLine` (`:16914`) skipped only `*` and `#`; `findAnswerLabelLine` (`:16726`) skips `' '`, `'\t'`, `'*'`, `'#'` and `codedAnswerLineAt` (`:14437`) skips those plus `- > \``. So `CHOICE: 1 / PLAN: reconsidering / ␣␣CHOICE: 2` was an ANSWER to every scanner and PLAN PROSE to the terminator alone, and the correction lost to the line it corrected. The wave-64 PARSETEST case pinned that as a MUST-NOT-MATCH. | The skip set is now the answer seam's own set (`' '`, `'\t'`, `'*'`, `'#'`), restoring the predicate's stated invariant. The wave-64 case is inverted to a POSITIVE, and the E6b block is rewritten to assert what is now true: the bounded and unbounded walks AGREE on this reply, the correction stands, and the carried plan still stops at the answer line. |
| 6 | MED | **CONFIRMED** | `modeEffectPriceTag` split the segment on `&&` and did `if (clause.find("ability$!") != string::npos) continue;` — one CLAUSE, not the block. `choice name(Gift) ability$!name(x) draw:1 controller && life:-3 controller!$ opponent` skipped the draw and priced the drain on this seat: at 3 life the row read "you LOSE 3 … THIS KILLS YOU" for an effect aimed at the opponent — the deck126 actor inversion on the surface F6 built to stop it. | The whole `ability$! … !$` span (every occurrence) is excised from the segment before any clause is read; an unpaired `ability$!` truncates the segment. A clause OUTSIDE the block is still the caster's and is still priced. The per-clause skip stays as a backstop. |
| 7 | MED | **CONFIRMED** | `castBodiesAdded` (`:20031`) counts `card->isCreature() ? 1 : 0` plus every `create(...)` creature token, and `crackBackBlockerRowTag` (`:20095`) turned that raw count straight into "This adds N blockers". Nothing asked `CANTBLOCK` or whether the body enters tapped, so a tapland-style creature (`tap(noevent)`, the same script token the fetch-row clause already reads) was counted as a blocker for a combat it is not present in. | `crackBackCoverFacts` admits the cast creature as a CHECKED body only when `isCreature() && !basicAbilities[CANTBLOCK] && !castBodyEntersTapped(card)`. A body that cannot block is neither checked nor unchecked — it is known not to be cover, and the row's cover figure is zero from it. The row prints the checked/unchecked split explicitly. |
| 8 | MED | **CONFIRMED** | `gangKillsAttacker` (`:4941`): `if (budget > 64) budget = 64; //bounded: the DP below is O(items x budget)`. Measured on the review's board (a 100/25 first-striker vs four 20/30s) the clamped run returns **kills=true, damage=40**; the true first-strike budget removes three blockers (75 of 100) and leaves 20, which its 25 toughness survives. A silent clamp that makes the attacker weaker makes the gang look lethal — the exact failure mode F7 exists to prevent. | The clamp is gone. The budget is bounded by `mandatory + the cost of every removable blocker` — a board fact, not a constant: no first-strike damage past that can buy anything, because there is nothing left to spend it on. The DP is the size of the board and the verdict is exact at every board size. |
| 9 | MED | **CONFIRMED** | `xLibraryReserve` (`:11255`) added `+n` for every `@each my upkeep:…draw:N` permanent unconditionally. Turn order is untap → upkeep → draw, so in the seat's OWN upkeep the trigger has already gone and the next such draw is next turn's — after the draw step the reserve is reserving against. With 12 cards and Staff of Nin it reserved 2 and called X=10 best when X=11 is safe: a legal option removed, the direction F1 exists to prevent. | New pure `xReserveUpkeepAhead(myTurn, phase)`: the upkeep draw is owed only when the seat's next upkeep is still ahead of its next draw step, which is false exactly in `myTurn && phase == MTG_PHASE_UPKEEP`. An unknown observer answers "ahead", keeping the reserve generous. |

## Files

- `projects/mtg/include/DecisionContract.h`, `projects/mtg/src/DecisionContract.cpp` — R1 (candidate
  admission, the two destination vectors, the apply-path guard split).
- `projects/mtg/src/AIPlayerGPT.cpp` — R1 (`attackerDestinationTag`, `walkerOnlyExclusionLine`, the
  permutation of the destination vectors, the row, the aggregate exclusion, the declaration guard),
  R2 (`menu_pass_forced` on the record), R4/R7 (`CrackBackAttackerFact`, `crackBackCoverFacts`,
  `castBodyEntersTapped`, the rewritten tag), R5 (`lineIsCodedAnswerLine`), R6 (block excision),
  R8 (the board-sized budget), R9 (`xReserveUpkeepAhead`), and the PARSETEST cases below.
- `projects/mtg/src/AIPlayerBaka.cpp`, `projects/mtg/include/AIPlayerBaka.h` — R2 (the floor's
  legality gate and its counter).

All engine comments tagged `#W64-AK (Rn)`. `git diff | /usr/bin/grep -c U+FFFD` = **0**.

## RED evidence

The four findings whose function signatures are unchanged were measured, not argued. The SAME test
binary was rebuilt with the four implementations reverted to their shipped semantics (the R5 skip
set, the R6 per-clause skip, the R8 `budget > 64` clamp, R9's unconditional reserve) and the new
cases run against it:

- **base semantics: `4520 passed, 16 failed`** — 10 R5 cases, 3 R6 cases, 2 R8 cases, 1 R9 case.
- **this tree: `4536 passed, 0 failed`.**

R4/R7's reproduction is arithmetic on the shipped function rather than a run, because the tag's
signature changed: the shipped `crackBackBlockerRowTag(5, 3, 1, {5})` computes `covered=5`,
`left=0`, `3-0 > 0` and emits ", which you survive" — the review's board, from the shipped source.
R1's is the shipped line `if (!card->isAttacker() && card->canAttack())` against
`MTGPlaneswalkerAttackRule::isReactingToClick`'s `canAttack(true)`; **no suite fixture can reach
it**, because `DecisionManager::buildDeclareAttackers` has exactly one caller
(`AIPlayerGPT::chooseAttackers`) and the suite has no endpoint. R2's is the shipped fall-through at
`AIPlayerBaka.cpp:5550`.

## Gate

Detached unit `w64-AK-gate`, `MemoryMax=4G MemorySwapMax=0`, binary rebuilt from clean
(`rm -f bin/wagic && make -f Makefile.sdl -j4`), suite at `WAGIC_TESTSUITE_THREADS=1`.

| leg | result |
|---|---|
| build | 0 errors |
| PARSETEST | **4536 passed, 0 failed** |
| suite | **1269 tests (0 failed), 67 AI tests (0 failed)** |
| `==Test Failed !==` | 0 |
| `==Test timed out` | 0 |

No run was killed under the memory cap. Base counts are lane AI's 1269 / 67 - this lane adds no
fixture (nothing it changed is reachable from one) and changes no fixture's outcome.

## Predictions for the wave-64 corpus

1. **R1** — no attackers window will omit a creature the engine would let a human send at a
   planeswalker; every row carrying `[may NOT attack them: …]` will also appear excluded from the
   ATTACK TOTAL line's damage figure, and the two counts will agree. Falsifier: an attackers window
   on a board with an opposing planeswalker whose row count is smaller than the number of the
   seat's untapped, non-summoning-sick creatures.
2. **R2** — `menu_pass_forced` appears on 0 records; if it appears, the record before it shows a
   window where `hasAnyLegalAction` was false. Falsifier: `menu_pass_forced` on a record whose
   window offered a castable row.
3. **R4/R7** — no `{crack-back cover:` clause says "which you SURVIVE" on a screen whose per-attacker
   tags say the added body cannot block the attacker it is credited against; every clause with a
   token body carries either an earned SURVIVE or "THIS IS NOT A SURVIVAL VERDICT".
4. **R5** — `plan_answer_line_ignored` is written 0 times for a line-leading coded answer line at any
   indentation, and no `answer_replaced` record latches a line the reply wrote AFTER the one taken.
5. **R6** — no `{this mode right now:` tag prices a magnitude that sits between `ability$!` and `!$`
   in the mode's own script line.
6. **R8** — no `GANG BLOCK:` verdict claims a kill on an attacker whose first-strike power exceeds
   the total toughness of the blockers it would have to remove.
7. **R9** — no `[<- best X for this cast:` badge rendered during the seat's own upkeep names a
   ceiling one lower than `library - 1` when the seat's only upkeep-draw permanent has already
   triggered that turn.

## What I did NOT verify

- **No live model probe and no corpus run.** Every render change here (R1's row and exclusion
  sentence, R4/R7's rewritten cover clause, R6's silence) is verified as a STRING, not as a decision
  a model made differently.
- **R1 is unfixturable in the suite** and is therefore proven by code reading plus pure PARSETEST
  cases only. The apply path's walker route was written by lane AI and is pinned by its ordering
  fixture; what I changed is which creatures reach it, and that half has no runnable test.
- **R1 rests on the ENGINE's legality model, not on the CR.** Wagic's `flyersonly` blocks
  `canAttack(false)` only, so the engine lets such a creature attack a planeswalker. Modern MTG
  rulings treat "creatures can't attack you" as covering planeswalkers that player controls, so
  Wagic may be wrong here — but that is a card/engine rules bug, not a decision-surface bug, and
  fixing it in this lane would have deleted the option twice over. **Flagged, not fixed.**
- **R2's residual hold.** While `hasAnyLegalAction` stays true and a menu is answered on every tick,
  the seat now holds indefinitely rather than passing. I did not construct such a board; I assert
  from the code that it requires a menu that re-arms and is answered every tick forever, which is a
  defect in the menu machinery this floor was never the right place to break. `mMenuPassForced`
  makes the OTHER branch visible; a permanent hold would show as a stalled game, not a record.
- **R2's oracle is permissive by design.** `hasAnyLegalAction` answers true when in doubt, so the
  floor now fires strictly less often than it did. I did not measure how much less.
- **R4/R7: tokens are counted as UNCHECKED, not as blockers.** A token's block legality is knowable
  in principle from its `create(...)` spec, but not through `canBlockPairwise` without an instance,
  and re-deriving evasion from a keyword list here is the drift the engine's own extracted predicate
  exists to prevent. The consequence is that the Siege-Gang shape F11 was written for now prints a
  floor, a ceiling and NO verdict where it used to print a survival claim. That is the brief's other
  allowed answer, and it is a real loss of information where the claim happened to be true.
- **R7's enters-tapped test is the `tap(noevent)` token, unqualified.** A card whose text carries it
  for another reason is treated as arriving tapped, which under-counts cover. I did not enumerate
  the false positives.
- **R5 retires E6b in practice.** With the terminator and the answer scanners on the same skip set,
  no line-leading coded answer line can sit inside the plan block at all, so `#W63-AD`'s in-plan
  exclusion now fires only on lines the answer seam would refuse anyway (placeholders, prompt
  echoes). The plan CARRY still stops at the answer line, which is what keeps it out of later
  windows. I did not re-litigate whether E6b's original deck146 defect can return in some other
  shape.
- **R8's bound is exact, its cost is not measured.** `mandatory + sum(costs)` is a board quantity,
  but I did not measure the DP's runtime on a pathological board (very many very tough blockers
  against a very large first-striker).
- **#3 is refuted on SCOPE, not on merit.** I did not test the connect-timeout classification, and
  the observation above is from curl's documented behaviour, not from a run.
- **Nothing was checked against the two known concurrency-only failures.** The gate ran
  single-threaded, which is ground truth.
