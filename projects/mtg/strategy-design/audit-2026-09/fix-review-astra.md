# Verdict: REWORK — not ready for a human-facing Vita release

The commit fixes some real defects, but introduces **a persistent targeting-protection bypass**, makes **non-search reveals incorrectly optional**, and can **reject every legal answer to a live AI menu**. The reveal-close replacement also still intervenes at draw time, without establishing that the reveal is stalled.

I reviewed `faf744acc..35aa9e606`, the supplied brief/report/origin documents, callers, local CR text, and primitive `text=`/`auto=` data. **No builds, game/test execution, edits, or commits.** The relevant engine implementations match the reviewed commit in the current checkout; later GPT changes were not treated as part of this fix.

## Per-hunk findings

Related declarations and their implementation hunks are grouped explicitly. **KEEP means no defect identified in that hunk, not runtime verification.**

### Engine hunks

| Hunk / location | Verdict | One-line reason |
|---|---|---|
| Edict witness globals — [AllAbilities.cpp:6858](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6858) | **KEEP** | No gameplay choice changed by these counters, but recording an intended owner does not prove a human was presented with the chooser. |
| Edict stale-victim gate — [AllAbilities.cpp:6946](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6946) | **REWORK** | Phasing and sacrifice-prohibition checks are useful, but the predicate is not the same as the candidate filter or actual sacrifice eligibility. |
| Edict replacement/chooser — [AllAbilities.cpp:6960](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6960) | **REWORK** | Mutates a surviving creature’s `bypassTC`, reconstructs an overbroad chooser, and puts the resolution-time choice through a new interruptible ability. |
| Fail-to-find predicate/declaration — [AllAbilities.cpp:787](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:787), [AllAbilities.h:166](/home/magi/Projects/wagicGPT/projects/mtg/include/AllAbilities.h:166) | **REWORK** | “Library origin + qualified chooser” does not establish a **search**; Mulch is a concrete counterexample. |
| Human reveal finalization — [AllAbilities.cpp:609](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:609) | **REWORK** | Correctly enables Idyllic Tutor’s empty answer, but the overbroad predicate also permits skipping mandatory non-search instructions. |
| Interactive-AI reveal fallback — [AllAbilities.cpp:1436](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:1436) | **REWORK** | Same misclassification lets a refused AI answer bypass mandatory reveal processing outside the named tutor defect. |
| New reveal-close helper — [AllAbilities.cpp:937](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:937) | **REVERT** | Closes every matching live reveal without checking stalledness, pending choices, or which transaction owns the parked library cards. |
| Draw-time rescue replacement — [MTGGameZones.cpp:407](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGGameZones.cpp:407) | **REVERT** | Infers “cannot complete” merely from a draw encountering parked cards; legal draws during resolution contradict that inference. |
| Mandatory-menu rebuild declaration, implementation and Update call — [ActionLayer.h:105](/home/magi/Projects/wagicGPT/projects/mtg/include/ActionLayer.h:105), [ActionLayer.cpp:790](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:790), [ActionLayer.cpp:328](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:328) | **REWORK** | Rebuilds a generic same-card menu, not necessarily the expired decision; unrelated voluntary activations can become mandatory. |
| Multiple-choice slot check — [ActionLayer.cpp:719](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:719) | **REWORK** | Bounds-checking a non-slot does not establish identity, and rejecting it prevents `doReactTo` from reaching the valid mode-choice handler. |
| Baka indexing guard — [AIPlayerBaka.cpp:3090](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:3090) | **KEEP** | Prevents the reported out-of-bounds indexing; does not itself fix multiple-choice identity or completion. |
| Decision-contract indexing guard — [DecisionContract.cpp:532](/home/magi/Projects/wagicGPT/projects/mtg/src/DecisionContract.cpp:532) | **KEEP** | Appropriate defensive bound at the reader; does not remove a valid ordinary-menu slot. |
| Pass fingerprint — [AIPlayerBaka.cpp:3159](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:3159) | **REWORK** | Counter quantities, instance identity and activated-action availability remain invisible; equality still does not prove no progress. |
| Declined-face hook and overrides — [AIPlayerBaka.h:307](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerBaka.h:307), [AIPlayerGPT.h:461](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerGPT.h:461), [TestSuiteAI.h:237](/home/magi/Projects/wagicGPT/projects/mtg/include/TestSuiteAI.h:237) | **KEEP** | Explicitly limits this mechanism to its arming seat classes without changing ordinary Baka’s available actions. |
| Declined-face early exits — [AIPlayerBaka.cpp:3271](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:3271), [AIPlayerBaka.cpp:3296](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:3296) | **KEEP** | Removes the fingerprint work from ordinary Baka and from empty latch sets; no new option suppression identified. |
| Card-destruction sweep and declarations — [MTGCardInstance.cpp:332](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGCardInstance.cpp:332), [GameObserver.cpp:556](/home/magi/Projects/wagicGPT/projects/mtg/src/GameObserver.cpp:556), [ActionLayer.cpp:162](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:162), [GameObserver.h:332](/home/magi/Projects/wagicGPT/projects/mtg/include/GameObserver.h:332), [ActionLayer.h:141](/home/magi/Projects/wagicGPT/projects/mtg/include/ActionLayer.h:141) | **REWORK** | Broadens destructive sweeps to every card destruction, with an unconditional retry on failed eviction and no corresponding ownership proof. |
| Ability-destructor deregistration and declaration — [MTGAbility.cpp:7882](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGAbility.cpp:7882), [ActionLayer.cpp:221](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:221), [ActionLayer.h:145](/home/magi/Projects/wagicGPT/projects/mtg/include/ActionLayer.h:145) | **REWORK** | Fixes parent-first dangling registration, but adds full-container scans to every ability destruction and changes mutation behavior during layer traversal. |
| Detached-card land guards — [LegalActions.cpp:251](/home/magi/Projects/wagicGPT/projects/mtg/src/LegalActions.cpp:251), [MTGCardInstance.cpp:1309](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGCardInstance.cpp:1309) | **KEEP** | A live card lacking a game cannot pass these game-dependent gates; checking its observer is not protection against an already-freed card. |

**REVERT above means reject those replacement hunks as the solution—not endorse restoring the old one-card rescue for shipment.**

### Fixture and test-command hunks

| Hunk / location | Verdict | One-line reason |
|---|---|---|
| Both registry hunks — [_tests.txt:1389](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/_tests.txt:1389), [_tests.txt:1420](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/_tests.txt:1420) | **KEEP** | Registers the new witnesses and removes the rules-false Idyllic Tutor expectation. |
| Deleted `w67AX_reveal_refusal_takes_legal_card.txt`; replacement [w82ec_search_may_fail_to_find.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w82ec_search_may_fail_to_find.txt:1) | **KEEP** | The expectation change is justified by CR 701.23b; a non-search negative witness is still missing. |
| Both parked-reveal fixture hunks — [w69bg_reveal_parked_library_no_deckout.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w69bg_reveal_parked_library_no_deckout.txt:1) | **REWORK** | Asserts card return, not that the owner’s live chooser/payload actually closed; direct `drawcard` does not discriminate legitimate reveals from stalls. |
| [w82ea_edict_rechoice_is_the_players.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w82ea_edict_rechoice_is_the_players.txt:1) | **REWORK** | Useful AI outcome witness, but cannot substantiate the claimed human interaction, eligibility filtering or absence of an added response window. |
| [w82ed_pass_probe_sees_mana.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w82ed_pass_probe_sees_mana.txt:1) | **KEEP** | Valid narrow mana-digest witness; it does not exercise productive repeated menu answers or forced passing. |
| [w82ef_multichoice_stale_row_id.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w82ef_multichoice_stale_row_id.txt:1) | **REWORK** | Accepts rejection as success without proving the surviving legal mode can still be selected. |
| [w82eg_expired_mandatory_menu.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w82eg_expired_mandatory_menu.txt:1) | **REWORK** | Covers only “all reacting abilities removed”; misses rebuilding into unrelated voluntary abilities. |
| [w82eh_detached_card_land_gate.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w82eh_detached_card_land_gate.txt:1) | **KEEP** | Valid null-observer guard witness, not a dangling-ability or destructor-ownership regression. |
| Probe/edict assertion command hunk — [TestSuiteAI.cpp:1475](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:1475) | **REWORK** | The edict assertion reads globals assigned before `addToGame`, rather than observing an armed chooser and its actual answering seat. |
| Detached-card/stale-menu/expired-menu command hunk — [TestSuiteAI.cpp:1740](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:1740) | **REWORK** | Stale-menu command manually clears the menu after checking rejection, hiding exactly the completion failure introduced by the fix. |
| Both edict-witness reset hunks — [TestSuiteAI.cpp:2613](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:2613), [TestSuiteAI.cpp:3580](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:3580) | **KEEP** | Appropriate resets for the serial witnesses; not evidence of human-seat behavior. |

## Ranked fixes required before shipping to a human on Vita

### 1. Remove the edict’s persistent protection bypass

At [AllAbilities.cpp:6984](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6984), the chooser is constructed with **`cands[0]` as its source**. [TargetChooser.cpp:1598](/home/magi/Projects/wagicGPT/projects/mtg/src/TargetChooser.cpp:1598) initializes `targetter = card`. Consequently:

```cpp
tc->targetter->bypassTC = true;
```

changes the **actual surviving creature**, not an isolated chooser property. Clearing `tc->targetter` immediately afterward does not undo that mutation.

[TargetChooser.cpp:1690](/home/magi/Projects/wagicGPT/projects/mtg/src/TargetChooser.cpp:1690) subsequently returns `targetter->bypassTC` for shroud, protection and opposing hexproof. If the human sacrifices a different candidate, the first creature keeps the bypass. An existing targeted ability sourced by that creature can then pass protections it must respect.

From the supplied [CR text](/home/magi/.gatelogs/engine-audit/cr.txt:8177):

> **702.18a** Shroud is a static ability. “Shroud” means “This permanent or player can’t be the target of spells or abilities.”

And:

> **702.11b** “Hexproof” on a permanent means “This permanent can’t be the target of spells or abilities your opponents control.”

This is a **new, concrete rules regression outside the sacrifice effect**. Setting this chooser’s `targetter` to null already expresses the untargeted choice; the card-wide mutation must not survive.

### 2. Stop identifying every qualified library reveal as a search

[Mulch’s primitive](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/sets/primitives/mtg.txt:77095) says:

> Reveal the top four cards of your library. Put all land cards revealed this way into your hand and the rest into your graveyard.

Its script uses `reveal:4`, `target(<4>land|reveal)`, and an option-two effect moving the remaining cards to the graveyard. It satisfies the new predicate: library origin and a type-qualified chooser.

A human can now finalize with zero selected lands and send those lands to the graveyard instead of the hand. The same predicate changes the interactive-AI fallback.

The actual permission is narrower:

> **701.23b** If a player is **searching a hidden zone** for cards with a stated quality, such as a card with a certain card type or color, that player isn’t required to find some or all of those cards even if they’re present in that zone.

[CR 701.23b](/home/magi/.gatelogs/engine-audit/cr.txt:6941) does not convert “reveal the top four” into a search. Also, [CR 608.2c](/home/magi/.gatelogs/engine-audit/cr.txt:5593), excerpt:

> **608.2c** The controller of the spell or ability follows its instructions in the order written.

**Keep Idyllic Tutor’s fail-to-find option, but identify actual search semantics.** The unqualified-search restriction is explicitly supplied by [CR 701.23d](/home/magi/.gatelogs/engine-audit/cr.txt:6953):

> **701.23d** If a player is searching a hidden zone simply for a quantity of cards, such as “a card” or “three cards,” that player must find that many cards (or as many as possible, if the zone doesn’t contain enough cards).

### 3. Do not close a legitimate reveal merely because a draw occurs

The new helper tests only matching `zone` and `!mAIForceClosed`. It does **not** test any stall budget, failed completion condition, outstanding decision, or reveal ownership beyond the shared zone pointer.

The premise in the draw hunk—no priority during resolution therefore nothing should draw mid-reveal—is rules-false:

> **117.2e** Resolving spells and abilities may instruct players to make choices or take actions, or may allow players to activate mana abilities. Even if a player is doing so, no player has priority while a spell or ability is resolving. See rule 608, “Resolving Spells and Abilities.”

[CR 117.2e](/home/magi/.gatelogs/engine-audit/cr.txt:1891) forbids priority, not instructed draws.

There is a shipped script exercising that distinction: [Sensation Gorger](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/sets/primitives/mtg.txt:102263) performs `discard:20 && draw:4` in reveal option one, before option two returns the revealed top card. With that card exhausting the physical library, a legitimate in-effect draw satisfies the new force-close trigger.

The helper then invokes [forceCloseStalledReveal](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:871), which removes option abilities and returns **every card in the shared reveal zone**. That is not proof that the pending effect completed correctly.

> **701.20b** Revealing a card doesn’t cause it to leave the zone it’s in.

[CR 701.20b](/home/magi/.gatelogs/engine-audit/cr.txt:6887) supports maintaining the correct logical library, not cancelling its active effect.

The partial-library case also remains unfixed: while the physical library is nonempty, the draw still skips over its logically top, parked cards. **Fix transaction/driver completion or logical-zone semantics; do not infer a stall from a draw.**

### 4. Preserve live multiple-choice answers after compaction

The new bounds check runs before [doReactTo’s multiple-choice dispatch](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:855). After the layer shrinks below the captured tail ID:

1. `getMenuControlId` returns false.
2. `doReactTo` returns without invoking `ButtonPressedOnMultipleChoice`.
3. The live `MenuAbility` may still have all its legal modes.
4. `rebuildExpiredMandatoryMenu` explicitly excludes multiple-choice menus.

Thus **an AI’s legal answer becomes unreachable**, potentially indefinitely. The human route differs: `ButtonPressed` dispatches multiple choice **before** slot resolution.

The converse is also unresolved: if the captured ID remains in range but now names another element, it is accepted. A range check is not identity validation.

**Caller audit:**

- Baka ranking and regular contract construction now have safe bounds, but cannot interpret a custom row’s synthetic ID as its option ability.
- `DecisionManager::inspectMayBatch` already checks bounds; no new indexing defect found there.
- `doReactTo` uses lookup as an eligibility gate even when no slot is needed.
- Human `ButtonPressed` bypasses that gate for multiple choice.
- Ordinary `ButtonPressed` accepts `slot >= 0`, contradicting the new assertion that every caller forbids indexing slot zero.
- Rebuild treats any successful lookup—including zero—as an answer, while Baka/contract readers skip zero.

Separate **row identity, mode index, layer slot and sentinel** semantics. The fixture must select a surviving real mode after compaction, not manually clear the unanswered menu.

### 5. Make the edict a valid resolution-time choice, not just an owner stamp

There is positive evidence for the intended seat routing:

- [ATargetedAbilityCreator](/home/magi/Projects/wagicGPT/projects/mtg/include/AllAbilities.h:4560) constructs the granted effect’s dummy source for the targeted player.
- The re-choice sets `tc->Owner`.
- The chooser is cloned by the existing ability-copy machinery.
- [GameObserver.cpp:1214](/home/magi/Projects/wagicGPT/projects/mtg/src/GameObserver.cpp:1214) routes an armed target chooser to its owner.

**For the ordinary stale-dead-victim case, this is a real UI-capable chooser, not another lowest-toughness autopick.** But the implementation still has these defects:

- The filtered `cands` vector is discarded when constructing `creature|mybattlefield`. That chooser does not exclude `CANTBESACRIFIED` or mutated-down components as the vector does.
- The single-victim predicate omits the mutation exclusion.
- `activateStored()` ignores the sacrifice result before lifegain proceeds; that remains an incomplete part of the named repair.
- The fallback at line 7016 explicitly auto-selects `cands[0]` if chooser construction fails.
- `GenericTargetAbility` retains default interruptibility. Its answer follows `TargetAbility::reactToClick → ActivatedAbility::activateAbility → fireAbility`, which adds another stack ability. The purported resolution-time re-choice therefore introduces another response opportunity.

The relevant [CR 608.2d](/home/magi/.gatelogs/engine-audit/cr.txt:5597) text begins:

> **608.2d** If an effect of a spell or ability offers any choices other than choices already made as part of casting the spell, activating the ability, or otherwise putting the spell or ability on the stack, the player announces these while applying the effect.

It also says:

> The player can’t choose an option that’s illegal or impossible …

And [CR 701.21a](/home/magi/.gatelogs/engine-audit/cr.txt:6909), excerpt:

> **701.21a** To sacrifice a permanent, its controller moves it from the battlefield directly to its owner’s graveyard. A player can’t sacrifice something that isn’t a permanent, or something that’s a permanent they don’t control.

The fixture’s global owner assignment, occurring **before** `armed->addToGame()`, proves none of these completion properties.

### 6. Rebuild the expired decision, not a mandatory menu of unrelated actions

[setMenuObject](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:607) gathers all abilities reacting to the subject. It prefers triggered/free rows when present; otherwise it retains ordinary cost-bearing/permanent rows.

If an expired mandatory trigger was on a permanent that still has a voluntary activated ability, rebuilding with `setMenuObject(subject, true)` can replace the trigger’s question with **“activate this other ability” without Cancel**.

This removes the human’s legal option not to activate it. “Same card” does not mean “same decision.”

[CR 117.3d](/home/magi/.gatelogs/engine-audit/cr.txt:1911), excerpt:

> **117.3d** If a player has priority and chooses not to take any actions, that player passes.

The cited rule does not justify turning a voluntary activation into mandatory completion. Nor does identity-only `anyLive` establish that a registered row still reacts legally.

### 7. Bound and validate destructor-driven deregistration

Two concrete structural problems remain:

- **Quadratic cleanup:** `cleanGarbage()` deletes each of `G` entries, and every `~MTGAbility` now scans the full `garbage` vector again. That adds Θ(`G²`) garbage scanning, plus live-layer/menu/mana scans and scans for unregistered temporary abilities. This is a new shared-path cost on Vita, not the narrowly gated work removed from Baka.
- **Retry without progress:** [ActionLayer.cpp:188](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:188) sets `again = true` even when `moveToGarbage(a)` fails. `removeFromGame` explicitly returns false for an element already in `mDestroying`. A reentrant sweep encountering that element retries forever, preventing the outer removal from completing.

The second is a **conditional hang mechanism demonstrated by the control flow**, not a reproduced card sequence.

Nulling garbage slots is appropriate for the existing indexed cleanup and avoids shifting that vector. But erasing `mObjects` inside a destructor changes behavior of active indexed traversals; forward Update/event loops can skip shifted entries, and Update retains `currentAction` after invoking its callback. The report does not establish that the newly supported “deleted while registered” paths are safe under those traversals.

I did **not** establish a new deterministic double-free. I also cannot accept “bookkeeping only” as a lifetime proof.

### 8. Stop treating the expanded fingerprint as proof of identical state

The new fingerprint still misreads several engine fields:

- `Counters::mCount` counts **counter records/types**, not counter quantity. [Counters.cpp:115](/home/magi/Projects/wagicGPT/projects/mtg/src/Counters.cpp:115) increments `Counter::nb` for another counter of an existing type without changing `mCount`.
- [MTGCard::getId()](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGCard.cpp:59) returns the printing’s `mtgid`, not instance identity.
- The “legal-action set” includes casts and land plays, **not activated abilities**.
- Granted abilities, counter names/quantities, damage and other state can change without changing the digest.

A second charge counter on an existing counter record can therefore be invisible if the other serialized fields remain equal. At the floor, [Act](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:5884) still lets repeated equality override `hasAnyLegalAction`.

This does not earn the claim “productive repeat cannot be force-passed.” The mana fixture verifies one previously missing input, not that property. The rule boundary remains CR 117.3d’s **player choosing not to act**, quoted above.

## What I could not determine

- **Actual Vita interaction:** I traced the human routing but did not observe the sacrificing player’s menu, focus, controls, or interruption behavior.
- **Destructor crash elimination:** No execution, sanitizers, or core replay. The lane itself says deregistration alone did not eliminate both crash shapes.
- **A concrete naturally occurring reentrant destructor-hang sequence:** The unconditional-retry failure is clear; its card-level trigger remains unproven.
- **A full productive-menu sequence reaching the pass threshold:** The digest collisions are established statically; actual forced passing on such a sequence was not exercised.
- **Controlled Vita performance:** Quadratic scanning is visible in code; its frame-time impact was not measured. The lane’s uncontrolled wall-time comparison cannot isolate it.
- **All control-change/LKI edict cases:** The re-choice derives its owner from the stored victim’s controller rather than an explicit sacrificing-player context; I did not establish correctness across every relevant zone/control transition.
- **Independent gate results:** The reported suite, parser and 840 post-fix heuristic games are the lane’s evidence, not checks I reran. Their coverage does not discriminate the human-facing counterexamples above.