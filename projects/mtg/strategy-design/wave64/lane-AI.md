# Wave-64 lane AI — F3, F4, F12, F14

Base `be2e88282`. Everything below is code-reading plus fixture/PARSETEST evidence and one
read of the wave-63 corpus records; **no live model probe was made**.

---

## F3 — a pathway's FRONT face consumed the rest of main phase 1 (deck152 HIGH-1, engine)

**Mechanism (found, not proposed).** A two-faced land in hand has TWO reacting rules
(`MTGPutInPlayRule`'s "Play Land" and the primitive's `autohand={0} ... flip(...)` back-face
entry), so the AI's click arms a MENU. `AIPlayerBaka::computeActions` answers an armed menu
**in place** — `doReactTo` on the heuristic path, `DecisionManager::applyMenuChoice` on the GPT
path (`AIPlayerGPT::computeActions`) — and queues nothing on `clickstream`. `AIPlayerBaka::Act`
then reads that empty clickstream as "nothing left to do this phase" and COMMITS a pass
(`userRequestNextGamePhase`) in the **same tick** the menu resolved.

Whether the pass took effect is decided by what the answer put on the stack, and that is the
whole front/back asymmetry the seat measured:

* FRONT face — `MTGPutInPlayRule::reactToClick`'s land branch does `putInZone` + `Spell::resolve()`
  synchronously. Nothing is on the stack, so `userRequestNextGamePhase`'s
  `stackLayer()->getNext(NULL, 0, NOT_RESOLVED)` gate does not refuse, and main 1 ends on the
  land drop. (corpus: 0 of 15 same-turn main-1 windows)
* BACK face — the `{0}` row is an activated ability, whose stack object trips exactly that gate,
  so the pass is refused and the window survives. (corpus: 5 of 6)

**Fix** (`src/AIPlayerBaka.cpp`, `include/AIPlayerBaka.h`): `Act` records whether an armed menu
was standing before `computeActions` and, when that menu is gone afterwards, DEFERS its pass by
one tick. Nothing is removed and no window is closed — the pass happens on the next tick if the
seat really has nothing left, over the board the menu produced. Floor: `kMenuPassHoldMax = 24`
consecutive deferrals, then the pass goes through regardless (the wave-57 D38 idiom), so a menu
that re-arms and is answered every tick can never hold a phase for ever.

**Shared choke point — what else reads it.** `ActionLayer::menuObject` is READ here and never
written; no seam that owns it (ActionLayer, `applyMenuChoice`, the GPT menu seam,
`selectMenuOption`) changes behaviour. `mMenuPassHold` is a per-seat member consulted only by
`Act`. Nothing added here enters `mPromptTail`, an ask key, a hold key or an option-set key —
the wave-61 livelock class is untouched.

**Blast radius:** this is `AIPlayerBaka::Act`, so it applies to every AI seat, suite games
included. That is why the gate is the evidence: 1269/0 and 67/0 with the tick cadence changed.

**Fixture** `bin/Res/test/ai_mdfc_front_face_keeps_main1_w64ai.txt` (AI). P1 plays Branchloft
Pathway through the two-row menu with a Mountain already untapped, then — only if main 1
survives — casts a HASTE Raging Goblin and attacks (P2 20 → 19).
**RED on base, verified:** built `be2e88282` clean in this worktree and ran the fixture alone
via `WAGIC_TESTSUITE_FILE`; it failed with `==life problem for player 1. Expected 19, got 20==`
(the Goblin was cast in main 2 and never attacked). GREEN after the fix.
**Control, also measured:** the identical fixture with a plain `Forest` in place of the pathway
(no menu, so no defect) passes on BOTH binaries — which is what makes the MENU the
discriminator rather than the AI's attack policy or the harness cadence.

**Prediction (falsifiable, wave-64 corpus):** taking a `(menu text: Play Land)` front-face row
is followed by a same-turn Main-phase-1 window at a rate indistinguishable from the back-face
row's (wave 63: 0/15 vs 5/6). Falsifier: any front-face take whose next window is Attackers /
Combat begins / Main phase 2 while a castable spell was on that turn's cast menu.

---

## F4 — no prompt ever offered attacking a planeswalker (deck152 HIGH-2)

**Shipped.** The engine has supported it for years (`MTGPlaneswalkerAttackRule`); no decision
surface said so.

* `DecisionManager::buildDeclareAttackers` now also enumerates `req.attackTargets` — the
  opponent's planeswalkers and battles, walked in **battlefield order**, which is the order
  `MTGPlaneswalkerAttackRule::reactToClick` builds its own choice menu in. Index i here is
  therefore index i on that menu, and the apply path needs no name matching.
* `AIPlayerGPT::chooseAttackers` renders them as `W#` rows (name, handle, `[planeswalker]` /
  `[battle]`, loyalty left and what combat damage does to it) plus one scope sentence: damage
  sent at a walker does not reduce their life, the walker keeps activating, and blocking is
  unchanged. Rows only — nothing is suggested and nothing is removed.
* Grammar: `A#>W#` (also `A1 -> W2`, lowercase `w`). Stated in the reply-shape sentence ONLY
  when a `W` row exists, so a window with no walker on their board reads byte-for-byte as before.
* Parse: new pure `parseAttackerTargets`, a SEPARATE pass over the same line the declaration was
  read from (`takenText`). `parseAttackerSet`'s A-index grammar already skips a digit glued to a
  letter, so `A1>W1` still declares A1 even if the suffix is dropped — a malformed target can
  never cost the declaration. First-wins per attacker; an out-of-range `A#`/`W#` drops that pair
  only. A record that bound one gets the parse note `attack_walker_target`.
* Apply (`applyDeclareAttackers`): clicks the planeswalker rule and answers its multiple-choice
  menu with the W index **in the same tick** — the attacker loop clicks the next creature
  immediately and an armed menu would swallow that click. Anything the rule refuses (no walker,
  an unpaid `attackPlaneswalkerCost`, a phased body) falls through to the ordinary attack rule,
  so the declaration always stands even when the target does not.

**PARSETEST:** 8 cases — positives (`A1>W1`, spaced-arrow, lowercase), MUST-NOT-MATCH (a plain
declaration binds nothing; an out-of-range `W#` drops that pair only; a digit glued to a word is
not a row label), a no-walkers negative, first-wins, and the echo check that the A-set parse is
unchanged by the suffix.

**Fixture** `bin/Res/test/w64ai_attack_planeswalker.txt`: two walkers on P2's battlefield;
`choice 1` on the attack menu takes "Attack a Planeswalker", `choice 1` on the walker menu takes
the SECOND one; only Jace dies, Garruk keeps its loyalty, P2's life is untouched at 20.

**This fixture is a PIN, not a red-on-base repro, and I say so plainly.** It pins the ordering
assumption the apply path rests on (menu index k == the k-th planeswalker/battle on the
opponent's battlefield). The engine path itself predates wave 64, so it is green on both
binaries. What wave 64 added — the rows, the `A#>W#` grammar and the target routing — cannot be
driven by a suite fixture: the reply comes from a model and the gate has no endpoint. PARSETEST
is the instrument for that half.

**Prediction:** attackers prompts on a board with an opposing planeswalker carry `W` rows in
100% of windows, and at least one corpus attack is declared with a `>W#` suffix whose translog
`chosen_text` reads `<attacker> -> <walker>` and whose walker loses that attacker's power in
loyalty with the defender's life unchanged. Falsifier: a `>W#` reply whose defender's life
dropped by the attacker's power, or an attackers window with a walker on their board and no W row.

---

## F12 — a `targetedplayer`-granted ability inverted the ACTOR (deck126 HIGH)

`WEventAbilityActivated::controller` is `GameObserver::currentlyActing()` at activation time —
the player CLICKING. For an ability granted to another player by a `targetedplayer` construct
(Tribute to Hunger, `mtg.txt:124075`) that is the TARGETED player, while the card the ability
sits on is the caster's. Both chairs then read the actor backwards, 8 occurrences over 5 games,
with the life lines that follow correct.

**Fix** (`src/AIPlayerGPT.cpp`): new pure `activationActorIsMine(activatorIsMe,
cardControllerKnown, cardControllerIsMine)` — the card the ability is ON is the object doing the
thing, so its controller is the actor; an unknown card controller falls back to the event's own
value. Applied at the verb only. `mine` (the activator) still gates the consumed-decision de-dup
`consumeSelfActivationStamp`, which is keyed on the seat that ACTED and must not move — so the
de-dup behaves exactly as before this wave.

Sibling case this also corrects: one of this seat's own abilities firing while the OPPONENT
holds priority was already credited to the opponent on base.

**PARSETEST:** 7 cases — the repro in both directions, MUST-NOT-MATCH for an ordinary own-card
activation on either chair, the unknown-controller negative, and the two ECHO shapes rendering
the corpus lines verbatim (`You used: Gain life equal to its toughness with Tribute to Hunger
targeting Master of the Feast`).

**Not fixtured, and why:** `TestSuiteAI`'s narration register records only stack zone changes
and payment receipts; it has no `WEventAbilityActivated` hook, and adding one is harness
widening this lane was told not to do. This is a render change, which the brief routes to
PARSETEST.

**Prediction:** 0 corpus GAME LOG lines credit a Tribute-to-Hunger-shaped activation to the seat
that did not cast it; every `used:` line agrees with the `gained N life` line beneath it.
Falsifier: any `Opponent used: ... with <a card the seat cast this game>` followed by
`You gained`.

---

## F14 — the 561 s reveal wait was the MODEL, not a gate (engine HIGH-3)

**The premise does not survive the record.** `152v146@1788653548` seq 31 carries
`reveal_wait_ticks: 144213`, `reveal_wait_secs: 561` — and `latency_ms: 560586` on the SAME
record. The 561 s IS the model round trip. seq 40 on the same seat reads 22 s wait / 21510 ms
latency: the two track each other exactly. Nothing was held:

* the full guard (`kRevealStallTicks` 20000 AND `kRevealStallSecs` 600) was not crossed — 561 < 600;
* the structural guard needs `revealStallStructSecsFor(deadline)` ≥ 1800 s, deliberately sized
  above deadline + one retry so a slow decision is never force-closed;
* `reveal_stall` (the boolean, wave-56 D12) is correctly ABSENT, and so is `reveal_stall_phase`.

So the floor did not release because there was nothing to release: the driver was parked waiting
for this seat's own answer, which is what `revealProgressSignature(false)` is built to count as
no progress. There is no engine defect here to fix, and I did not invent one. There WAS a
reporting defect, and it is what made this read as a 62%-of-all-reveal-wait stall:
`reveal_wait_*` folds inference time into a figure named like an engine wait, and the structural
counter cannot say otherwise on its own.

**Shipped** (`AllAbilities.cpp`, `AIPlayerGPT.cpp/h`, `Player.h`): `noteRevealStall` now also
carries the DRIVER-ONLY wait — the full progress signature, in which a model poll IS progress —
and the record writes `reveal_wait_driver_ticks` / `reveal_wait_driver_secs` beside the
structural pair. A wait whose driver half is ~0 is inference time; only a growing driver half is
a stall. Report only: nothing in the engine reads these, and the force-close budgets are
untouched.

**Prediction:** in the wave-64 corpus every `reveal` record's `reveal_wait_secs` is within a
second or two of `latency_ms / 1000` while `reveal_wait_driver_secs` stays near 0, and any
record where the driver half is large is a genuine engine stall worth a lane. Falsifier: a
record with a large `reveal_wait_driver_secs` and no `reveal_stall`.

**Not fixtured.** The shape is a 9-minute inference round trip; it is not reproducible in the
suite (no endpoint), and there is no behaviour change to pin — the two new fields are record
content. `revealasyncticks` can force a stall, but it would exercise the force-close path this
lane did not touch.

---

## Gate

Detached unit `w64-AI-gate`, `MemoryMax=4G MemorySwapMax=0` (build at 6G), binary rebuilt from
clean (`rm -f bin/wagic && make -f Makefile.sdl -j4`), suite at `WAGIC_TESTSUITE_THREADS=1`.

| leg | result |
|---|---|
| build | 0 |
| PARSETEST | **4359 passed, 0 failed** (base 4344; +15 = 7 F12 + 8 F4) |
| suite | **1269 tests (0 failed), 67 AI tests (0 failed)**, 0 `==Test timed out` (base 1268 / 66) |

Both new fixtures are registered in `bin/Res/test/_tests.txt`; the scratch probe registries were
deleted.

---

## What I did NOT verify

* **No live model probe and no corpus run.** F4's whole model-facing half — that the pilot reads
  the `W` rows and writes `A#>W#` — is untested against a model. The grammar is pinned pure and
  the apply path is pinned by the ordering fixture; whether the model USES it is the corpus's
  question.
* **F4's apply choreography answers a menu synchronously.** I did not observe that in a live
  game. It follows the shape `applyDeclareBlockers` already uses (click, then drive the engine's
  own cycle within the same apply), and the rule refuses rather than crashes when it cannot
  react — but the ability-GC click-burst hazard the c5 design note warns about is a real class
  and this is one more synchronous click on the attackers path.
* **F4 battles are enumerated with planeswalkers** because the engine's own rule menu does. I
  did not construct a battle board; the row says `[battle]` and the loyalty clause is skipped
  when no loyalty counter exists, but no fixture covers a battle.
* **F3's floor value (24) is a judgement, not a measurement.** No corpus shape needed more than
  one deferral; I did not measure how many consecutive menu-answering ticks a real window
  produces, and the floor exists only so a pathological re-arm cannot hold a phase.
* **F3 changes `AIPlayerBaka::Act`, the shared seat loop.** The gate is 0-failed and the tick
  cadence of 1269 fixtures survived it, but I did not audit every fixture whose `AICALLS` budget
  now buys one fewer pass, and a fixture that was one tick from its budget could become
  timing-sensitive.
* **F12's rule is "credit the card's controller".** I checked the cases I could enumerate
  (stolen permanents, abilities granted onto an opponent's own creature, the `targetedplayer`
  grant) and found no case where the ACTIVATOR is the truthful actor while the card's controller
  is not. I did not sweep every `targetedplayer`/`AbilityTP` construct in the primitives.
* **F14: I read two reveal records, not all of them.** The `wait ≈ latency` identity is measured
  on seq 31 and seq 40 of one seat file; I did not tabulate every `reveal` record in the 21
  games, and the "62% of all reveal wait time" figure is the deck-seat reviewer's, not mine.
* **I did not measure base myself for the aggregate counts.** 4344 / 1268 / 66 are the brief's
  figures on `0026a9d87`; my base is `be2e88282`. The RED counterfactual for F3 IS measured on
  this tree, on a clean `be2e88282` build.
* **Nothing here was checked against the two known concurrency-only failures** (`lifeline`,
  `merrow_reejerey`) — the gate ran single-threaded, which is ground truth, and read 0.
