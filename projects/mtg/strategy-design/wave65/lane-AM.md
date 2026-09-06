# Wave-65 lane AM — G2, G3, G7

Base: master `537070ac2` (worktree `w65-lane-AM`). Everything below is code reading plus
fixture / PARSETEST evidence and a read of the wave-64 corpus records cited in
`known-bugs.md` / `engine-seat.md`. **No live model probe and no corpus run.**

---

## G2 — `A#>W#` was declared, echoed, and dealt nothing (engine HIGH-2)

**Mechanism (found, and measured, not proposed).** `MTGPlaneswalkerAttackRule::reactToClick`
builds a `MenuAbility` over the opposing planeswalkers/battles and calls `resolve()`, which only
`addToGame()`s it. The menu is ARMED by `MenuAbility::Update` -> `setCustomMenuObject`, on the
FOLLOWING tick. Wave 64's apply path answered it under `if (al && al->menuObject)` — and that
pointer is **always NULL at that point**, so the answer was never given. Execution then fell
through to the ordinary attack rule and declared the creature at the PLAYER; when the walker
menu finally armed, its answer ran `AAPlaneswalkerAttacked::resolve` -> `toggleAttacker(true)`
on an **already-declared** attacker, and `toggleAttacker`'s else-branch REMOVES a creature from
combat. That is the corpus shape: 123v126 seq 25 declared an unblockable 5/5 at a 4-loyalty
Sorin, seq 26->27 ran Attackers -> Main 2 with no combat-damage event, Sorin read 4 loyalty at
seq 32 and their life was 23 before and after. 2 of 2 walker attacks delivered 0.

**Fix** (`src/DecisionContract.cpp`, `applyDeclareAttackers`): answer the menu the rule just
created **by identity** — the newest `MenuAbility` on the action layer whose `source` is this
card, not "whatever menu is armed" (another rule's or another seat's menu must never be
consumed by an attack declaration). `MenuAbility::resolve` has already set `triggered`, so
`reactToChoiceClick` is answerable now; it is the same entry `ButtonPressedOnMultipleChoice`
would reach a tick later. Nothing else changed: a reply that names no target still attacks the
player, and a target the rule refuses still leaves the declaration standing.

**Instrument (new).** No scripted fixture could reach that seam: a scripted click rides the
menu on a LATER tick, which is exactly the path that WORKS — which is why
`w64ai_attack_planeswalker` was green on a broken binary. New driver command
`aideclareattack <attacker>[ > <target>]` (`src/TestSuiteAI.cpp`) calls
`DecisionManager::buildDeclareAttackers` + `applyDeclareAttackers` with the same arguments
`AIPlayerGPT::chooseAttackers` passes, so a fixture pins the declaration END TO END.

**Fixtures** (both registered in `_tests.txt`):
* `bin/Res/test/w65am_gpt_attack_planeswalker_damage.txt` — the corpus shape. Hill Giant (3/3)
  at Jace Beleren (3 loyalty): Jace dies, defender's life untouched at 20.
* `bin/Res/test/w65am_gpt_attack_second_planeswalker.txt` — two walkers, the SECOND taken, so
  the index is discriminating. Garruk keeps 4 loyalty, Jace dies, life 20.

**RED on base, measured on this tree.** With the driver command present and
`src/DecisionContract.cpp` reverted to `537070ac2`, a clean rebuild ran both fixtures alone via
`WAGIC_TESTSUITE_FILE`: **2 of 2 failed**, both with
`==life problem for player 1. Expected 20, got 17==` plus `Jace Beleren` still on the
battlefield — i.e. the fall-through attacked the PLAYER for 3. GREEN after the fix.

**Measured and therefore NOT claimed:** I initially also "fixed" `MenuAbility::processAbility`
re-reading `abilities[0]` for the resolving clone's target. Restoring only
`DecisionContract.cpp` and leaving `AllAbilities.cpp` at base turned BOTH fixtures green, so
that second change was unnecessary and was reverted: `reactToChoiceClick` SAFE_DELETEs the
unchosen options, `SAFE_DELETE` NULLs the slot, and `if(head)` then skips the re-derivation, so
the chosen target stands. No `AllAbilities.cpp` change ships.

**Second half (deck152 LOW).** The W-row scope paragraph named the destination and said nothing
about what a planeswalker does in COMBAT. It is now the named constant
`kAttackTargetScopeFacts` and states, restriction-first, that a planeswalker is not a creature
and can never block. Additive: the two wave-64 facts are byte-identical (PARSETEST pins that).

**Prediction (falsifiable, wave-65 corpus):** every `A#>W#` reply whose `chosen_text` reads
`<attacker> -> <walker>` is followed by that walker's board line losing the attacker's power in
loyalty counters (or the walker leaving the battlefield), with the defender's life unchanged.
Falsifier: any `>W#` take after which BOTH the walker's loyalty and the defender's life are
unchanged across the combat — the wave-64 shape, which the lane's old falsifier ("their life
dropped") could not catch.

---

## G3 — the granted-ability actor, at the seam the corpus prints (engine HIGH-3)

**Why wave 64 could not move it.** The fix asked `e->source->controller()`. On a
`targetedplayer` grant `e->source` IS the nameless dummy `ATargetedAbilityCreator::resolve`
built, and the dummy's controller is the GRANTEE — the same player
`WEventAbilityActivated::controller` already named. The two facts were identical, so the line
never changed: 126v130 seq 16 still prints the lane's own falsifier verbatim (`You cast Tribute
to Hunger` / `Your Tribute to Hunger resolved` / `Opponent used: Gain life equal to its
toughness with Tribute to Hunger targeting Goblin #1` / `You gained 1 life (now 21)`), 10 lines
/ 5 events / 3 games.

**Fix** (`src/AIPlayerGPT.cpp`): the NAME half was already right because `resolveOwningCardName`
walks `storedSourceCard` back to the granting card. The CHAIR now walks the SAME chain. New
pure `activationSourceIsGrantedDummy(displayName, rawName)` (a dummy is a card with no name of
any kind — precisely the rung the name ladder skips) and pure
`activationChairOverLadder(activatorIsMe, rungs)`, which takes the first NAMED rung's controller
and defers to the existing `activationActorIsMine` for the unknown-controller fallback, so
wave 64's rule still governs where it was right. The call site builds the rungs from
`e->source` outward through `storedSourceCard` (same depth limit as the name ladder), so the
rendered name and the rendered chair can never name two different cards.

**PARSETEST (verified at the corpus line, not at the helper):** 12 cases — the dummy predicate
positive and three MUST-NOT-MATCH forms; the REPRO ladder in BOTH chairs; a MUST-NOT-MATCH that
reproduces exactly what wave 64 shipped (asking rung 0 alone yields the grantee — the reason the
fix was invisible); MUST-NOT-MATCH for ordinary own/their activations in both chairs; the
unknown-controller and empty-ladder negatives; and two ECHO cases rendering the corpus line
verbatim through `abilityActivationNarration` from each seat.

**Not fixtured, and why:** `TestSuiteAI`'s narration register records stack zone changes and
payment receipts only; it has no `WEventAbilityActivated` hook, and adding one is harness
widening this lane was told not to do. This is a render change, which the brief routes to
PARSETEST.

**Prediction:** 0 corpus GAME LOG lines credit a `targetedplayer`-granted activation to the seat
that did not cast the granting card; every `used:` line agrees with the life line beneath it.
Falsifier: any `Opponent used: ... with <a card this seat cast this game>` followed by
`You gained`.

---

## G7 — the two-decline cap, re-opened (deck123 HIGH-2; DOCTRINE)

**The breach.** `declineCap = 2` with **no re-opener at all** retired legal rows for the rest of
the turn. 162 seq 66/69 retired the free `{T}` token-maker rows; Intruder Alarm then RESOLVED in
main 1 (seq 73) and the menu held only three equips — the combo could not fire on the turn it
assembled. "Enforce legality without constraining choice" forbids that.

**Fix** (`src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`): a decline is an answer about the BOARD
it was given on. New `mPassDeclineBoard` records that board per decline key; when the board has
moved, the count is erased and the allowance starts again. Nothing is cached blind and nothing
is removed — an UNCHANGED board still honours the two declines, which is the churn the cap was
built for (a held fetch-crack re-asked at 44-97 windows a game).

* The re-opener is scoped by pure `declineBoardScope(boardKey)` = the board key **without its
  first line**. `serializeGameState`'s first line is the phase/turn header, and a phase advance
  is NOT a board change; keying on the raw key would restart the allowance ~10 times a turn and
  turn a churn control into churn. Everything after that line is game state.
* `boardKey` is now computed before the option loop instead of after it. `serializeGameState()`
  only reads the game and nothing between the two sites mutates it, so the value is
  byte-identical to the one built at the old site (which still consumes the same variable).
* A TAKEN fetch crack is CONSUMED, not declined: it stamps an empty board, and the re-opener
  skips empty stamps, so a spent crack never comes back.
* The row's clause said "not offered again **this turn**" and that is no longer true, so it now
  reads "not offered again **until the board changes**". `lastOfferClause` rides the RENDERED row
  only; the decline key, the ask key and the translog keep the pure line (wave-47 R4's rule,
  unchanged). Nothing added by this lane enters `mPromptTail`, the ask key, the hold key or the
  option-set key — the wave-61 livelock class is untouched.

**PARSETEST:** 9 cases — POSITIVE (a permanent arriving re-opens), MUST-NOT-MATCH (a phase
advance over an unchanged board does not, plus a case proving the raw keys DO differ there so
the distinction is real), NEGATIVE (a key with no header line scopes to nothing and cannot
compare equal to a real board), the new clause text, MUST-NOT-MATCH on the retired "this turn"
wording, the no-allowance-spent and no-affirmative-converse negatives, and two ECHO cases (a
reply copying the reworded clause still binds; the clause leaves no residue in the record). The
wave-47 R4 block's expectation was updated to the new text in place.

**Not fixtured, and why:** the suite has no endpoint, so no fixture can put a priority menu to a
model and count what it is re-offered. This is a render/offer-policy change on the GPT priority
seam; PARSETEST is the instrument the brief routes it to. Its real gate is the wave-65 corpus.

**Prediction:** 0 windows in the wave-65 corpus retire a row across a board change — i.e. for
every priority window, any option line present in an earlier window of the same turn and absent
here is absent over a board (`declineBoardScope`) identical to the one it was declined on.
Falsifier: a window whose board differs from the previous window's and whose option list is a
strict subset of it with no cast, sacrifice or zone change explaining the missing row. Second,
narrower falsifier keyed on the repro: a turn in which a combo piece resolves and the next
priority window's menu still omits a row declined earlier that turn.

---

## Gate

Detached unit `w65-AM-gate`, `MemoryMax=4G MemorySwapMax=0`, binary rebuilt in this worktree,
suite single-threaded (ground truth).

| leg | result | base (brief) |
|---|---|---|
| build | clean link | — |
| PARSETEST | **4561 passed, 0 failed** | 4536 (+25: 12 G3, 4 G2, 9 G7) |
| suite | **1271 tests (0 failed)**, **67 AI tests (0 failed)**, 0 `==Test timed out` | 1269 / 67 (+2 fixtures) |

Both new fixtures are registered in `bin/Res/test/_tests.txt`; the scratch probe registry
`test/_probe_am.txt` was deleted. `git diff | grep -c $'\357\277\275'` = 0.

---

## What I did NOT verify

* **No live model probe and no corpus run.** G2's model-facing half (that the pilot writes
  `A#>W#` at all) and G3's and G7's whole surfaces are unmeasured against a model this wave.
* **G2's fix is verified at the seam the GPT reply rides, through a driver command I added.**
  It is the same entry point `AIPlayerGPT::chooseAttackers` uses, with the same arguments — but
  it is not literally a model reply, and the harness's tick cadence around it is the suite's,
  not a live game's.
* **G2 battles are still untested.** `req.attackTargets` enumerates battles alongside
  planeswalkers because the engine's own menu does; I built no battle board, and the corpus has
  0 `[battle]` rows.
* **G2's synchronous menu answer is one more same-tick click on the attackers path** — the
  ability-GC click-burst hazard the c5 design note warns about is a real class. It is now ONE
  `reactToChoiceClick` per walker declaration where wave 64 intended one
  `ButtonPressedOnMultipleChoice`; I did not stress it with many walker-targeted attackers in
  one declaration.
* **G3's rule is "the first NAMED rung on the storedSourceCard chain".** I checked the cases I
  could enumerate (the `targetedplayer` grant, a stolen permanent, an own ability firing under
  the opponent's priority) and found none where the dummy's own controller is the truthful
  actor. I did not sweep every `targetedplayer`/`AbilityTP` construct in the primitives, and I
  did not observe the corrected line in a live game.
* **G3 is not fixtured** (no `WEventAbilityActivated` hook in the suite register) — the ECHO
  cases render the corpus line, but nothing pins that the engine hands this seam a dummy with a
  populated `storedSourceCard` on every granted construct. `resolveOwningCardName` producing
  "Tribute to Hunger" in the corpus is the evidence that it does for this one.
* **G7's re-opener granularity is a judgement.** "The board moved" = the board key minus its
  first line, so a floating mana change or a life change re-opens an allowance. I did not
  measure the resulting ask volume; the cap's original purpose (a fetch-crack re-asked at 44-97
  windows a game) is preserved only for windows whose board is genuinely unchanged, and the
  wave-65 corpus is where the token cost of this shows up. If it rises sharply, the scope — not
  the doctrine — is the dial.
* **G7 moved `boardKey`'s computation earlier.** I argued (not measured) that nothing between
  the two sites mutates game state; the gate's 1271/0 is the only evidence that the ask cache
  and the re-ask memory still key on the same value.
* **The two known concurrency-only failures** (`lifeline`, `merrow_reejerey`) were not
  exercised: the gate ran single-threaded and read 0.
* **Base counts 4536 / 1269 / 67 are the brief's**, on `dab6a6cba`; my base is `537070ac2`. The
  RED counterfactual for G2 IS measured on this tree.
