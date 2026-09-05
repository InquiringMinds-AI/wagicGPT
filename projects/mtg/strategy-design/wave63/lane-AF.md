# Lane AF — the nine codex-review findings, verified first, plus E19

Base: `29cc95efe` (master after the AB-AE merge + the review commit + the Mossborn fix
`76e4f58b9`). Branch `w63-lane-AF`, worktree `worktrees/lanes/w63-AF`.

Every finding was checked against the code BEFORE anything was changed. Eight are CONFIRMED and
fixed in the general form; one (#2) is REFUTED as a defect against E10's owner-facing evidence and
answered with the disclosure the brief made a condition of keeping the normalisation.

## CONFIRMED / REFUTED

| # | Sev | Verdict | Evidence (checked first) | What shipped |
|---|-----|---------|--------------------------|--------------|
| 1 | HIGH | **CONFIRMED** | `DecisionContract.cpp` built the cast route's X menu as `int shown = maxX > 50 ? 50 : maxX;` while `announceXHeader` prints "higher values are NOT offered (they are unaffordable)". The ENGINE's own menu has a row per affordable X (`MTGRules.cpp:533` `options = playerMana - cost + 1`). Fixture `w63af_announce_x_no_hard_cap.txt` (61 mana floated, Ivy Elemental `{X}{G}`): **RED on base — contract carries 51 rows, engine has 61**. | The cap is gone. `shown = maxX`, clamped only by the engine's actual row count (`menu->abilities.size() - 1`), which is the one bound that is a fact rather than a constant. New suite command `assertxrows <N>`. |
| 2 | HIGH | **REFUTED as a defect; the disclosure gap is CONFIRMED and fixed** | Adjudicated against E10 (wave62/engine-seat.md HIGH-2) as the brief directs. The review's own trigger is "pass through an **empty combat without changing rows**" — the nothing-moved case, which the owner-facing evidence names as the promise KEPT, not the defect. `holdStillStands` is SET EQUALITY over every rendered row (`#W61-V R2`), and `holdKeyRow` erases exactly one substring, `" (combat comes next this turn)"` — every price, annotation and row name is still compared byte for byte, so any board change that moves a row still re-opens the window. The second half of the brief's test ("the model could not have known Main 2 differs") also fails: the hold row already said "A pass row that differs only by naming which step comes next is the same row". | The row now says it in PHASE words, which is what the brief required for keeping the normalisation: *"…so a hold taken in your first main phase also covers your second main phase while these rows do not change"*. PARSETEST pins the literal at both places that hold it. |
| 3 | MED | **CONFIRMED** | `AIPlayerGPT.cpp` `const int reachCap = myLands + 2;` — a constant with no board fact behind it, feeding a clause that already claims to be *"what N lands plus the drops this hand can still make would pay for"*. At the review's board (11 lands, a 6-drop and a legal 14-drop) the 14-drop is excluded at cap 13 and every land is called spare. The wave-63 E16 PARSETEST case pinned that 13 as a constant. | `spareReachCap(myLands, handLandCards)` — the lands down plus the LAND CARDS still in the hand this ask is about, which is exactly the quantity the sentence names. The E16 case now derives its 13 from `spareReachCap(11, 2)` instead of pinning a heuristic. |
| 4 | MED | **CONFIRMED** | `GptPlanCaveat.h` `kMarkers[]` held bare `"wait"`, `"hold on"`, `"hang on"`, `"let me"`, `"let's"`. `planScratchpadCut("Hold removal. Wait until their end step, then cast it.")` returned `"Hold removal."` — **RED verified by rebuild**: three new REPRO/POSITIVE cases fail on the old list. | Markers narrowed to correction SHAPES only: `wait,` / `wait -` / `wait no` / `but wait` / `hold on,` / `hang on,` / `hmm` / `let me re` / `let's re` / `lets re` / `actually` / `on second thought` / `scratch that` / `correction` / `re-read` / `reread` / `i need to re` / `no,`. Both original repros ("Wait, I have Sorin", "…but wait-Sanguine Bond was exiled") still cut. |
| 5 | MED | **CONFIRMED** | `inPlayLc` was built from BOTH battlefields and the guard's first act was `if (live) continue;` — so "I control Sanguine Bond" was discharged by the OPPONENT's copy. | Every presence phrase now carries the board it is a claim about (`kBackScope` / `kFwdScope`: 0 = either, 1 = mine, 2 = theirs), and only that board is checked. Theirs-scoped phrases added ("they control", "the opponent controls", "on their battlefield"…). An empty board is a fact, not a missing argument: the caller passes `ownershipScoped=true`; an unscoped caller gets the wave-62 verdict byte for byte. |
| 6 | MED | **CONFIRMED** | `if (rank[i1][j2] + rank[i2][j1] > rank[i1][j1] + rank[i2][j2])` — a SUM over ordinal ranks. `[4,2]` (one blocker lost) and `[3,3]` (none lost) both sum to 6, so the strict `>` keeps the losing pair while the header says the line was chosen for the blockers' material. | `betterBlockerPair()` — lexicographic on the pair sorted WORST FIRST (raise the worst outcome first), sum kept only as the last tie-break so no previously-improving swap is lost. On the 4/3/2/1 scale that is exactly "fewest blockers lost first". Pure; the whole table is in PARSETEST. |
| 7 | MED | **CONFIRMED** | `crackBackReliefClause(cbTotal, crackBackBodyContribution(dtc), …)` subtracts only the killed body's own current power, and the clause presents `total -> after` as the post-removal total. A lord's contribution to its neighbours is not recomputed (the review's Goblin King case). | The engine already knows: an `ALord` keeps a map of the abilities it created, keyed by the creature each is attached to. `crackBackBodyHoldsUpOthers()` fires only when one of those creatures is ANOTHER body in the same crack-back total; the clause then reads `T -> A BEFORE static effects - <name> is also changing the power of their other attackers, and that goes with it, so the real total after it dies is not exactly A`, and a lethal remainder says "at this figure". An ordinary body is byte-identical to wave 62. |
| 8 | MED | **CONFIRMED** | `stackPendingDrawsFor` does `dynamic_cast<StackAbility *>(it)` and `continue`s on failure. `DrawAction` is its own `Interruptible` subclass (`ActionStack.h:160`) carrying `player` and `nbcards`, pushed by `ActionStack::addDraw`. Fixture `w63af_pending_draws_see_drawaction.txt`: **RED on base — 3 staged cards, the scan sees 0**. | A `DrawAction` branch that credits `da->nbcards` when `da->player == seat`, honouring the same `exclude`. New suite command `assertpendingdraws <expected> <staged>`; `gptStackPendingDrawsFor` exported so the fixture pins the SAME function the rows are priced off. |
| 9 | MED | **CONFIRMED** | `gptDeadlineMissed` asked `if (httpStatus != 0) return false;` BEFORE the curl code. curl publishes the status the moment response HEADERS land, so 200-headers + no body + curl 28 at our own deadline returned false and `noAnswerClassFor` then called a real wall miss `transport_error`. **RED verified by rebuild**: three new REPRO cases fail on the old order. | `curlCode == kCurlOperationTimedOut` is asked FIRST and returns true — we set that timeout, so code 28 IS the deadline expiring, whatever arrived before it. Wave-62 R8 is untouched: an empty-bodied 200 that COMPLETED carries curlCode 0 and is still `empty_reply`. |

## E19 — "double the number of +1/+1 counters" is ONE counter event

Every Oracle text read off Scryfall on 2026-09-05 (live API), not from memory. All eight say
"double the number of +1/+1 counters" — one event of N, not N events of one. The old script
`thisforeach(counter{1/1.1}) counter(1/1)` put N separate one-counter events on the stack, so every
counter-added replacement (Branching Evolution, Hardened Scales) fired N times — the owner's Vita
vpk26 report.

**Two rewrite forms, and the reason there are two.** Outside a `newability[...]` payload the comma
form `counter(1/1,counter{1/1.1})` is used, exactly as the Mossborn fix. INSIDE a `newability[...]`
payload it CANNOT be: `AbilityFactory` splits the transforms payload on `,` with a naive
`split(sabilities, ',')`, so a comma inside the brackets truncates the grant. `parseCounter` splits
on `,` OR on `.`, taking whichever yields more parts, so `counter(1/1.counter{1/1})` is the same
ability with no comma — and `counter{1/1}` is byte-equivalent to `counter{1/1.1}` through
`parseCounter` (both resolve to power 1 / toughness 1 / unnamed). Verified by the fixtures, not by
reading.

Every fixture is a Hardened Scales board ("that many plus one"), which is the only discriminator
that separates one event of N (+1 once) from N events of one (+1 each). Every one was measured RED
on the old script before the new number was pinned.

| Card | File | Oracle (Scryfall 2026-09-05) | Script before | Script after | Fixture | RED on base | GREEN after |
|---|---|---|---|---|---|---|---|
| Kalonian Hydra | mtg.txt | "Whenever this creature attacks, double the number of +1/+1 counters on each creature you control." | `_ATTACKING_all(creature\|mybattlefield) transforms((,newability[thisforeach(counter{1/1.1}) counter(1/1)]))` | `…newability[counter(1/1.counter{1/1})]…` | `w63af_kalonian_hydra_one_event.txt` | 15/15 | **11/11** |
| Primordial Hydra | mtg.txt | "At the beginning of your upkeep, double the number of +1/+1 counters on this creature." | `@each my upkeep:thisforeach(counter{1/1.1}) counter(1/1)` | `@each my upkeep:counter(1/1,counter{1/1.1})` | `w63af_primordial_hydra_one_event.txt` | 9/9 | **7/7** |
| Hydra's Growth | borderline.txt | "At the beginning of your upkeep, double the number of +1/+1 counters on enchanted creature." | `teach(creature) transforms((,newability[@each my upkeep:thisforeach(counter{1/1.1}) counter(1/1)]))` | `…newability[@each my upkeep:counter(1/1.counter{1/1})]…` | `w63af_hydras_growth_one_event.txt` | 8/8 | **7/7** |
| Solarion | mtg.txt | "{T}: Double the number of +1/+1 counters on this creature." | `{T}:thisforeach(counter{1/1.1}) counter(1/1)` | `{T}:counter(1/1,counter{1/1.1})` | `w63af_solarion_one_event.txt` | 18/18 | **13/13** |
| Tanazir Quandrix | borderline.txt | "When Tanazir Quandrix enters, double the number of +1/+1 counters on target creature you control." | `target(creature[counter{1/1}]\|mybattlefield) transforms((,newability[thisforeach(counter{1/1.1}) counter(1/1) all(this)])) oneshot` | `…newability[counter(1/1.counter{1/1}) all(this)]…` | `w63af_tanazir_quandrix_one_event.txt` | 6/6 | **5/5** |
| Biogenic Upgrade | borderline.txt | "Distribute three +1/+1 counters among one, two, or three target creatures, then double the number of +1/+1 counters on each of those creatures." | three lines, each `…newability[thisforeach(counter{1/1.1}) counter(1/1) all(this)]…` | all three `…newability[counter(1/1.counter{1/1}) all(this)]…` | `w63af_biogenic_upgrade_one_event.txt` | 13/13 | **12/12** |
| Invigorating Surge | borderline.txt | "Put a +1/+1 counter on target creature you control, then double the number of +1/+1 counters on that creature." | `transforms((,newability[thisforeach(counter{1/1.1}) counter(1/1) all(this)])) oneshot` | `…newability[counter(1/1.counter{1/1}) all(this)]…` | `w63af_invigorating_surge_one_event.txt` | 8/8 | **7/7** |
| Visions of Dominance | borderline.txt | "Put a +1/+1 counter on target creature, then double the number of +1/+1 counters on it." | `transforms((,newability[thisforeach(counter{1/1.1}) counter(1/1) all(this)])) oneshot` | `…newability[counter(1/1.counter{1/1}) all(this)]…` | `w63af_visions_of_dominance_one_event.txt` | 6/6 | **5/5** |

The Tanazir fixture also pins the brief's target question: the amount expression is evaluated by
`WParsedInt(nbstr, spell, target)`, and the fixture's Tanazir carries NO counters at all while the
targeted Mossborn Hydra carries two — a doubling read off the source instead of the target would
add nothing, so 5/5 is only reachable if the count came from the TARGET.

No `thisforeach` user outside the owner's eight was touched. **One sibling was observed and left
alone**: `Ascendant Acolyte` (borderline.txt:5624) carries the identical loop for the identical
Oracle sentence. It is not on the owner's list, so it is reported here rather than changed.

## Files

- `projects/mtg/src/DecisionContract.cpp` — R1 (the cap).
- `projects/mtg/src/AIPlayerGPT.cpp` — R2 (hold row text), R3 (`spareReachCap`), R6
  (`betterBlockerPair`), R7 (`crackBackBodyHoldsUpOthers` + the clause), R8 (`DrawAction` branch,
  `gptStackPendingDrawsFor`), R9 (`gptDeadlineMissed` order), R5 (call site passes both boards),
  and 45 new PARSETEST cases.
- `projects/mtg/include/GptPlanCaveat.h` — R4 (markers), R5 (ownership scoping).
- `projects/mtg/include/AIPlayerGPT.h` — R8 (the exported scan).
- `projects/mtg/src/TestSuiteAI.cpp` — `assertxrows`, `assertpendingdraws`.
- `projects/mtg/bin/Res/sets/primitives/mtg.txt`, `borderline.txt` — E19, eight cards (ten lines).
- `projects/mtg/bin/Res/test/` — ten new fixtures + `_tests.txt`.

All engine comments tagged `#W63-AF (Rn)`; the card work carries its reasoning in the fixture
headers. `git diff | grep -c U+FFFD` = 0.

## Gate

Hermetic `make -B -f Makefile.sdl -j8`; detached unit `w63-AF-gate`, MemoryMax=4G,
MemorySwapMax=0.

- **PARSETEST**: `4344 passed, 0 failed` (base 4299; +45, the exact count of CHECKs this lane adds).
- **Suite, `WAGIC_TESTSUITE_THREADS=1`**: `1267 tests (0 failed), 66 AI tests (0 failed)`.
  `==Test Failed !==` count 0, `==Test timed out` count 0. Base was 1257 tests; +10 fixtures.
- No run was killed under the memory cap.

## Predictions for the wave-63 corpus

1. **R1** — no ANNOUNCE_X ask in the corpus will offer exactly 51 rows while its own header claims
   the values above are unaffordable. Any menu whose row count is 51 will have `maxX == 50`.
2. **R3** — `{spare:` will appear on land rows of a deck125-style hand holding an uncastable
   fifteen-drop, and the "is NOT counted here" clause will name a cap equal to (lands down + land
   cards in hand), never (lands down + 2).
3. **R4** — no carried plan will be truncated at a sentence beginning "Wait until", "Let's <verb>"
   or "Hold on to"; the `[...the rest of your plan was not carried…]` marker will appear only after
   an explicit correction shape.
4. **R5** — no `plan_asserted_absent` withdrawal will fire for a card the seat itself controls, and
   at least one will fire for an "I control X" claim whose only live copy is the opponent's.
5. **R6** — no blockers header will say "chosen for your blockers' material" over an assignment
   that loses a blocker when a legal reshuffle of the SAME matched attackers loses none.
6. **R7** — every `removes N from the CRACK-BACK total above` on a lord (a body with a registered
   `ALord` over another counted attacker) will carry "BEFORE static effects"; no plain body will.
7. **R9** — `transport_error` will not co-occur with an HTTP status of 200 in any translog record;
   that combination becomes `timeout`.
8. **E19** — no game log will show a counter-doubling trigger putting more than ONE counter event
   on the stack; with a doubler in play, a doubled hydra's counter total will be `2N` (or `2N+1`
   under Hardened Scales), never `4N`.

## What I did NOT verify

- **No live model probe.** Nothing in this lane was run against Spark; fixtures and PARSETEST are
  the whole gate, so every RENDER change (R2's row wording, R3's clause, R7's caveat) is verified as
  a string and not as a decision the model made differently.
- **R2's residual hole.** I refuted the finding as stated, but I did NOT prove the stronger claim
  that no board change can leave every cast row byte-identical. `holdStillStands` compares only the
  RENDERED rows; if a combat that changed life totals leaves every row of a cast menu byte-identical
  (no row of that menu prices life), the latch would honour a Main-1 hold in Main 2 across a real
  board change. There is no board key inside the hold predicate, and `serializeGameState()` cannot
  be used as one without re-breaking E10 (it opens with `Phase: <name>`). Left as a scoped question
  for the corpus, not patched blind.
- **R7 does not recompute the total.** It NAMES the omission; it does not produce the true
  post-removal number. The Goblin King case still prints `6 -> 4`, now labelled as pre-static.
  Detecting the lord is exact (`ALord::abilities`); the corrected arithmetic is not attempted.
- **R8's live path.** The fix is verified by a fixture that STAGES a `DrawAction`. I could not build
  one that reaches it through real play: `AADrawer::resolve` resolves its own `DrawAction` inside
  the same call, and `AJandorsRing` leaves one unresolved only inside a priority window the scripted
  pump cannot reliably land a command in. So the code path is proven; its reachability in a real
  game is asserted from the two call sites, not observed.
- **R1's `hasSpecificX` route.** The clamp to the engine's row count is the right bound there too,
  but the fixture exercises the ordinary `hasX` route only (`options == maxX + 1`). I did not
  construct a `hasSpecificX` board.
- **E19 combat/attack interactions beyond the fixtures.** Kalonian Hydra's trigger is verified on a
  single attacking hydra; the "each creature you control" fan-out over several creatures at once is
  not pinned by a fixture.
- **Ascendant Acolyte** (same defect, same Oracle sentence) is unfixed by deliberate scope.
