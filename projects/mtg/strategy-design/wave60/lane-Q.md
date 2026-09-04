# Wave-60 lane Q — the nine findings of `wave60/codex-review.md`

Branch `w60-lane-Q`, base `c8c3d3477` (= `1fcdeeedc`, the L-P merge, + the review commit).
Base binary for every counterfactual below: `archives/wagic-1fcdeeedc-w60step1`.

## Verdict table

| # | Sev | Verdict | Evidence |
|---|---|---|---|
| 1 | HIGH | **CONFIRMED** | `namedCastLifeSurcharge` returned `nLife` on the FIRST matching naming permanent and the cast-row loop carried a `break` after one `namedCastPriceTag`. Two permanents naming one card charge two independent triggers, so both the target-ask surcharge and the cast row under-priced by every punisher after the first — and the B1 win fold subtracts exactly that number. Fixed: both scans SUM; the cast row emits one tag per naming permanent, each priced against the life left after the ones already counted. |
| 2 | HIGH | **CONFIRMED as an unproven gate; NOT REPRODUCIBLE as a live hang** | The gate really did prove only source identity: `drivingFor` (AllAbilities.cpp) returned true for any live async reveal whose `source == tc->source`, and `ownChooser()` tested `tc->source != source` and nothing else — neither proves the chooser belongs to the reveal DRIVER. Fixed with `MTGRevealingCards::ownsWaitingAction()` (the layer's waiting element must be the reveal itself, its two ability wrappers, or their `MayAbility::mClone`s), used by both. The hang the review predicts was NOT observed — see "The #2 fixture" below. |
| 3 | MED | **CONFIRMED** | `cleanupDiscardPriceClause` stated a hypothetical hand-size discard as a certainty ("forces 11 discards you cannot decline ... = 44 life - you would be at -31; this KILLS you"), and PARSETEST pinned that wording. It is a CEILING: every card spent before the cleanup step removes one discard. Reworded to the ceiling it is ("unless you spend cards before then ... up to 11 discards ... = up to 44 life - at worst you would be at -31; at worst this KILLS you") and the X row's NET reads `NET at worst -53` whenever a cleanup ceiling is folded into it. Three base PARSETEST expectations rewritten. |
| 4 | MED | **CONFIRMED** | `attackTotalLine` was fed raw `power` and printed it as life loss. Fixed at the source of the number, not the wording: an INFECT attacker is excluded from the total AND the floor and the exclusion is named (poison, not life); a DOUBLESTRIKE attacker contributes twice; and an opponent with `CANTLOSE`/`CANTLIFELOSE` on their battlefield gets NO resulting-life figure and no kill claim at all. The header now reads "N total combat damage to a player". |
| 5 | MED | **CONFIRMED** | `tapUntapBranchTag` asserted "a tapped creature CANNOT BLOCK" and "it UNTAPS in THEIR untap step" from the branch's card TYPE. `CANBLOCKTAPPED` and `DOESNOTUNTAP` falsify each. The target is not chosen at this menu, so the honest form is scoped, not per-target: the caller counts those two states over the opponent's battlefield and the tag says "normally X — EXCEPT that N of the creatures you could pick here …". Both counts 0 → byte-identical. |
| 6 | MED | **CONFIRMED** | `boardCreatureCounts` pushed every creature into `theirNames`/`myNames` for a `destroy all(creature)` sweeper. An indestructible creature now leaves the destroyed COUNT and the roster and is listed as ` - NOT DESTROYED (indestructible), stays on the battlefield: …`; a creature whose script can regenerate stays in the count and is marked `(may survive: it can regenerate)`, scoped to `destroy` (not `bury`, not `CANTREGEN`). Exile sweepers are untouched — indestructible does not stop exile. |
| 7 | MED | **CONFIRMED** | `ABlink::returnCardIntoPlay` runs `spell->source->counters->init()` (AllAbilities.cpp:11364) before the permanent reaches the battlefield, so every counter-derived point of P/T is gone. The descriptor and the crack-back clause both read the exile object's live `power`/`toughness`. Fixed: `exileHostagePower`/`exileHostageToughness` read the PRINTED values (`basepower`/`basetoughness`, `origpower`/`origtoughness` on a shown other face — the same definition `printedPTTag` uses), and the crack-back total uses the printed power. |
| 8 | LOW | **CONFIRMED** | `planCarryBound` appends a marker ending in `]`; the caller's next pass read that as an unfinished sentence and truncated back through the last `.` past the midpoint — deleting the marker on the common case. The two passes are now ONE function, `gptcaveat::planCarryCompose`, which skips the stump trim whenever the bound chose the cut. PARSETEST now checks the COMPOSED path, which is what the review said was missing. |
| 9 | LOW | **CONFIRMED** | `catch (json::exception&) { content.clear(); }` with curl 0 and HTTP 200 left every existing class false, so `noAnswerClassFor` reported `empty_reply`. New class `bad_reply`, set when a NONEMPTY body yields no content (parse throw, or a parse that carries neither content nor reasoning), cleared the moment any path recovers content. Ranked BELOW stale-livelock / timeout / transport / non-200 (each explains the missing body outright). The translog now writes `http_status` on a `bad_reply` record too — the 200-plus-no-answer pairing IS the evidence. |

Nine findings read, nine CONFIRMED at the code level, nine fixed. Only #2's predicted CONSEQUENCE
is refuted; its mechanism is not.

## The #2 fixture — what it pins, and what it does not

`bin/Res/test/w60Q_reveal_gate_foreign_chooser.txt` + test primitive `2100000900`
(`W60Q Twin Trigger Angel`, in `test/lexicon/test_primitives.txt`, the file the suite loads by
default): ONE card carrying TWO `@combat(attacking)` triggers — a targeted
`moveto(exile)` and the Sigarda-shaped interactive reveal — so both choosers carry
`tc->source ==` the Angel. That is exactly the review's trigger.

It does not go RED on base. Measured, with a temporary `W60Q_PROBE` fprintf on the gate
(removed before the gate run) and `WAGIC_REVEAL_DEBUG=1`:

* while the reveal display is open the **`MTGRevealingCards` element is itself the action
  layer's `currentWaitingAction`**, so `getCurrentTargetChooser()` returns the REVEAL's own
  chooser. `ActionLayer::setCurrentWaitingAction` allows one waiting action at a time
  (`assert(!ae || !currentWaitingAction)`), so the second ability's chooser was never observed
  as the CURRENT one while the reveal was live. Every `gate WITHHELD` line the probe printed was
  the reveal's own chooser — which is what the gate is for.
* the run that first looked like the review's hang (`orphaned target chooser … blocked 12 phase
  advances`) was an artefact of `revealasyncticks 40` — a 40-tick simulated in-flight window
  tripping GameObserver's own orphan-release valve. At `revealasyncticks 2` it does not appear,
  and the engine has that release valve regardless, so the review's "parked until the
  1,800-second structural force-close" is additionally guarded against.

So the fixture ships as a REGRESSION PIN on the gate's scope, not as a RED repro: it asserts
that a reveal plus a second targeted ability from one source both reach an outcome (Wall of Wood
in exile, library back to 7, turn ends). It is GREEN on `wagic-1fcdeeedc-w60step1` and GREEN
after — stated plainly rather than dressed up.

The fix ships anyway, and is strictly safer than the code it replaces: it can only ever make the
seat answer MORE choosers, never fewer, and it removes the review's stated hazard by
construction. `ownChooser()` gained the same predicate, so the driver can no longer read a
foreign same-source chooser as option one either.

## Files

* `src/AllAbilities.cpp`, `include/AllAbilities.h` — R2: `ownsWaitingAction()`; used by
  `drivingFor()` and `ownChooser()`.
* `src/AIPlayerGPT.cpp` — R1, R3, R4, R5, R6, R7, R9 + every new PARSETEST case.
* `include/AIPlayerGPT.h` — R9: `mLastBadReply`, the sixth `noAnswerClassFor` overload.
* `include/GptPlanCaveat.h` — R8: `planCarryCompose`.
* `bin/Res/test/lexicon/test_primitives.txt`, `bin/Res/test/w60Q_reveal_gate_foreign_chooser.txt`,
  `bin/Res/test/_tests.txt` — the R2 fixture.

`AIPlayerBaka.cpp` is UNCHANGED: the review cites line 3698, but the defect is in what
`drivingFor` proves, not in the branch that calls it.

## Gate

Detached unit `w60-Q-gate`, 6G cap (build) / 4G (runs), `rm -f bin/wagic` full rebuild first.

| | base (`wagic-1fcdeeedc-w60step1`) | this lane |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1247 tests, 0 failed, 64 AI / 0 failed | **1248 tests, 0 failed, 65 AI / 0 failed** |
| `==Test Failed !==` / `==Test timed out` | 0 / 0 | **0 / 0** |
| PARSETEST | 3673 / 0 failed | **3699 / 0 failed** (+26) |

`git diff | /usr/bin/grep -c $'\357\277\275'` = 0. Suite base figures are lane P's measured
numbers on this base; PARSETEST base was re-measured here on the archived base binary.

## RED evidence per confirmed finding

R3, R4 and R5 each required REWRITING a base PARSETEST expectation that pinned the false claim —
three cases that were green on base and are red against the fixed strings, visible in the diff
(`forces 11 discards you cannot decline` → `forces up to 11 …`; `20 total power` →
`20 total combat damage to a player`; `a tapped creature CANNOT BLOCK` → `normally CANNOT BLOCK`).
That is the review's own charge for #3 — "the green test protects a false expectation" — made
concrete. R8's new case is red on base by construction (base's composed path deletes the marker
the case requires); R9's `bad_reply` cases cannot even compile against base (the overload is
new). R1, R6 and R7 have no base case to contradict: their old behaviour was silent
under-pricing / over-naming, and the new cases pin the corrected shape.

## Predictions (falsifiable, next corpus)

1. **R1**: no target-ask or cast row whose board carries TWO naming permanents matching the cast
   name prices only one of them; falsifier is a `{named-cast price}` tag count lower than the
   count of matching `chooseaname` permanents on the opponent's battlefield in the same window.
2. **R3**: zero `CLEANUP PRICE` clauses stating a discard count or a life figure without `up to`
   / `at worst`; and zero X rows reading `NET -N` where a cleanup clause is in the same brace.
3. **R4**: zero `ATTACK TOTAL` lines that name an infect attacker's power in the total, and zero
   kill claims against an opponent controlling a `CANTLOSE`/`CANTLIFELOSE` permanent.
4. **R6**: zero sweeper rows naming an indestructible creature among the destroyed; the
   `NOT DESTROYED` tail appears on every board that has one.
5. **R8**: every carried plan longer than 400 characters ends in the truncation marker. Falsifier:
   a translog prompt whose `YOUR PLAN` echo is exactly 400ish characters and ends on a `.`.
6. **R9**: `empty_reply` no longer appears on any record carrying `http_status: 200`;
   `bad_reply` appears instead. If the next corpus has no schema fault at all the prediction is
   untested, not confirmed.

## What I did NOT verify

* **No live model probe and no corpus run.** Every render change is PARSETEST-verified only.
* **#2's live hazard is unproven in BOTH directions.** I could not construct a board on which a
  second same-source chooser becomes the layer's current one during a live reveal, so I cannot
  say the review's shape is impossible — only that it did not occur in the one construction the
  engine let me build. The fix is a narrowing of a withholding gate and is safe either way.
* **`ownChooser()`'s new predicate is exercised only by the existing reveal fixtures.** They are
  green (w58G, both w60P fixtures, w53 reveal, the vita hurkyl softlock), which is the evidence
  that the ownership set is COMPLETE for the reveal shapes the suite covers — not proof that it
  is complete for every reveal shape in the card pool. A reveal whose chooser is armed by an
  element outside {self, abilityFirst, abilitySecond, their `MayAbility` clones} would now have
  its chooser answered by the seat instead of its driver. No such shape is in the suite.
* **R4's prevention/"cannot deal combat damage" half is NOT covered.** The engine has no general
  predicate for a damage-prevention shield or a creature that deals no combat damage; I handled
  the three states it does expose (INFECT, DOUBLESTRIKE, CANTLOSE/CANTLIFELOSE) and left the
  rest. The ceiling stays conditional ("with none blocked"), the floor does not, and a
  prevention effect can still falsify the floor.
* **R6's regeneration detection is a script-text scan** (`magicText` contains "regenerate",
  `CANTREGEN` clear), not an ability-graph walk. It can miss a granted regeneration and can fire
  on a card whose text mentions regeneration without granting it to itself.
* **R7 ships no PARSETEST case of its own** — the change is inside two accessors over a live
  `MTGCardInstance`, and the descriptor's wording (already pinned by lane P) is unchanged. It is
  verified by reading `ABlink::returnCardIntoPlay` and by the callers, not by a test.
* **R9's `bad_reply` path was never exercised against a real endpoint.** The classification
  function is pinned; the integration path that sets `mLastBadReply` is not (the review says as
  much: "Scalar PARSETEST classification cases cannot exercise this integration path").
* **The build is incremental on the seeded `objs-sdl`**, though `rm -f bin/wagic` plus the two
  header edits (`AllAbilities.h`, `AIPlayerGPT.h`, `GptPlanCaveat.h`) forced a wide recompile.
  No `make -B`.
