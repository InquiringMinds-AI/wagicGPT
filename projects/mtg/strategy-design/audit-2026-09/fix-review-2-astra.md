# Verdict: REWORK — not ready for a human-facing Vita release

The rework removes the targeting-protection mutation, removes draw-triggered reveal cancellation, and repairs the multiple-choice lookup contract. However, **the new garbage-sweep shortcut disables deregistration precisely during recursive destruction**, **the edict still derives the sacrificing seat from the chosen creature**, and **the pass fingerprint still cannot establish that a repeat was unproductive**. The new sacrifice-failure branch also leaks its allocated lifegain ability.

I reviewed `5e618f379..306949a39`, the supplied diff, brief, both review/report documents, origin documents, and relevant callers. The reviewed engine implementations match the current checkout. **No builds, game/test execution, edits, or commits.** Rules quotations below come from the supplied `cr.txt`; card facts come from primitive `text=`/`auto=`.

**KEEP means no defect identified in the scoped change, not runtime verification.** Related declaration and implementation hunks are grouped explicitly. Findings distinguish new regressions from defects the rework leaves unresolved.

## Per-hunk findings

### Engine hunks

| Hunk / location | Verdict | One-line reason |
|---|---|---|
| Edict chooser declaration, constructor, zone predicate, eligibility and clone — [TargetChooser.h:348](/home/magi/Projects/wagicGPT/projects/mtg/include/TargetChooser.h:348), [TargetChooser.cpp:2679](/home/magi/Projects/wagicGPT/projects/mtg/src/TargetChooser.cpp:2679) | **KEEP** | Removes the card-wide protection bypass and consistently filters candidates for the supplied sacrificing seat; choosing the correct seat remains the caller’s responsibility. |
| Shared edict-shape predicate/declaration — [AllAbilities.h:5626](/home/magi/Projects/wagicGPT/projects/mtg/include/AllAbilities.h:5626), [AllAbilities.cpp:7266](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:7266) | **KEEP** | Keeps the sacrifice-result rail scoped away from Proper Burial-style lifegain without a stored sacrifice. |
| Edict eligibility, candidate scan and re-choice — [AllAbilities.cpp:6920](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6920) | **REWORK** | `owner = victim->controller()` makes the control test circular and can sacrifice—or reopen choices for—the wrong player after a control change. |
| Edict `canBeInterrupted = false` assignments, within that hunk — [AllAbilities.cpp:6991](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6991) | **KEEP** | The newly selected answer now resolves directly through `fireAbility`, rather than adding another response window. |
| Edict witness global and post-registration stamp — [AllAbilities.cpp:6865](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6865), [AllAbilities.cpp:7010](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:7010) | **KEEP** | Better registration evidence, but it checks the wrapper’s registration and template chooser’s owner—not an armed, human-operated chooser. |
| Sacrifice-result lifegain rail — [AllAbilities.cpp:7274](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:7274) | **REWORK** | Correctly suppresses life on a reported failure, but returns without deleting the already allocated `toActivate`. |
| `activateStored()` return propagation and pointer clearing — [AllAbilities.cpp:7306](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:7306) | **KEEP** | Propagates the one-shot result and removes its dangling member pointer; no additional option suppression identified here. |
| Fail-to-find helper/rationale — [AllAbilities.cpp:759](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:759) | **REWORK** | “Starts with `type:`” neither means “whole-zone look” nor establishes a search; the explanatory equivalence is false. |
| `searchMayFailToFind()` discriminator — [AllAbilities.cpp:807](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:807) | **REWORK** | Fixes the literal `reveal:4` Mulch counterexample, but retains script-shape inference instead of identifying actual search semantics. |
| Deletion of `closeParkedRevealsInto` — [AllAbilities.cpp:949](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:949) | **KEEP** | Removes the unjustified “a draw proves a stalled reveal” cancellation; its replacement comment overstates what the remaining stall guard proves. |
| Draw-time “logical top” replacement — [MTGGameZones.cpp:387](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGGameZones.cpp:387) | **REWORK** | Correct for an unchanged contiguous top-card park, but library origin alone does not establish top position or active reveal ownership. |
| Per-card purge retry loop — [ActionLayer.cpp:179](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:179) | **KEEP** | Fixes the demonstrated unconditional retry on a refused eviction; it does not establish that every cascade is bounded. |
| `forgetElement()` sweep bypass — [ActionLayer.cpp:235](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:235) | **REVERT** | Suppresses cleanup of live registrations and later garbage aliases when recursively owned children are destroyed during the sweep. |
| `cleanGarbage()` flag, declaration and initialization — [ActionLayer.cpp:282](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:282), [ActionLayer.h:155](/home/magi/Projects/wagicGPT/projects/mtg/include/ActionLayer.h:155), [ActionLayer.cpp:1001](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:1001) | **REWORK** | Initialization is correct, but the flag’s blanket semantic assumption is not; optimize cleanup without disabling required deregistration. |
| Mode sentinel, lookup dispatch and contract comments — [ActionLayer.h:112](/home/magi/Projects/wagicGPT/projects/mtg/include/ActionLayer.h:112), [ActionLayer.cpp:732](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:732) | **KEEP** | A mode row no longer masquerades as a layer index or gets rejected solely because that old index expired. |
| Expired-menu close declaration, Update call and implementation — [ActionLayer.h:105](/home/magi/Projects/wagicGPT/projects/mtg/include/ActionLayer.h:105), [ActionLayer.cpp:356](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:356), [ActionLayer.cpp:843](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:843) | **KEEP** | Stops rebuilding an expired mandatory decision into unrelated voluntary activations; retained registration is still not proof of current answerability. |
| Pass-probe rationale and counter/instance serialization — [AIPlayerBaka.cpp:3159](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:3159), [AIPlayerBaka.cpp:3224](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:3224) | **REWORK** | The added fields are useful, but “proven identical state” remains false and productive state changes remain invisible. |
| Pass-probe cast identities and activated-ability scan — [AIPlayerBaka.cpp:3258](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:3258) | **REWORK** | Records usable ability identities, not their targets or effect state, and adds mutating/allocating eligibility calls on floor ticks. |

**REVERT applies to the blanket `forgetElement` bypass—not to deregistration itself or the entire commit.**

### Fixture and test-command hunks

| Hunk / location | Verdict | One-line reason |
|---|---|---|
| Registry additions — [_tests.txt:1425](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/_tests.txt:1425) | **KEEP** | Registers the witnesses without removing existing tests. |
| Three test primitives — [test_primitives.txt:6451](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/lexicon/test_primitives.txt:6451) | **KEEP** | Test-only witnesses; the shroud witness’s claimed stable heuristic preference is contradicted by the lane’s own report. |
| All parked-library fixture hunks — [w69bg_reveal_parked_library_no_deckout.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w69bg_reveal_parked_library_no_deckout.txt:1) | **KEEP** | Correctly stops requiring a draw to void the other parked cards; covers only the contiguous-top case. |
| Pass-probe fixture changes — [w82ed_pass_probe_sees_mana.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w82ed_pass_probe_sees_mana.txt:1) | **KEEP** | Valid narrow mana/counter-quantity witness, not a test of productive repeats through `Act()` and its pass floor. |
| Multiple-choice fixture commentary — [w82ef_multichoice_stale_row_id.txt:14](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w82ef_multichoice_stale_row_id.txt:14) | **REWORK** | Claims a surviving live mode, while its command creates a synthetic menu without establishing a live `MenuAbility` dispatcher. |
| Shroud/bypass edict fixture — [w83fa_edict_no_protection_bypass.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w83fa_edict_no_protection_bypass.txt:1) | **KEEP** | Useful invariant pin; not evidence that a human saw or answered the replacement choice. |
| Unsacrificeable edict fixture — [w83fa_edict_unsacrificeable_never_pays.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w83fa_edict_unsacrificeable_never_pays.txt:1) | **REWORK** | Its claimed RED directly contradicts the report, which says base passes because the heuristic never activates the problematic path. |
| Mulch negative witness — [w83fb_mulch_is_not_a_search.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w83fb_mulch_is_not_a_search.txt:1) | **KEEP** | Correct expected outcome for its one-land fixture; does not validate the general search discriminator. |
| Partial-library draw witness — [w83fc_partial_library_logical_top.txt:1](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/test/w83fc_partial_library_logical_top.txt:1) | **KEEP** | Correct expected draw for a single parked top card; does not cover selective parks or intervening library operations. |
| Bypass assertion and edict assertion extensions — [TestSuiteAI.cpp:1505](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:1505), [TestSuiteAI.cpp:1557](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:1557) | **KEEP** | Check useful invariants without affecting production decisions, with the witness limitations above. |
| Multiple-choice assertion extensions — [TestSuiteAI.cpp:1859](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:1859) | **REWORK** | Proves “accepted non-index,” then manually clears the menu; does not prove mode dispatch or completion. |
| `addcounter` / `shrinkactionlayer` additions — [TestSuiteAI.cpp:1896](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:1896) | **KEEP** | Test-only operations; I found no registered fixture using `shrinkactionlayer`, contrary to the report’s usage claim. |
| Both expired-menu assertion extensions — [TestSuiteAI.cpp:2019](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:2019), [TestSuiteAI.cpp:2061](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:2061) | **KEEP** | Specifically checks that an unrelated voluntary activation does not become a mandatory replacement. |
| Both witness-reset hunks — [TestSuiteAI.cpp:2774](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:2774), [TestSuiteAI.cpp:3743](/home/magi/Projects/wagicGPT/projects/mtg/src/TestSuiteAI.cpp:3743) | **KEEP** | Appropriate serial-fixture resets. |

## Ranked fixes required before shipment

### 1. Preserve deregistration during recursive garbage destruction

**New conditional crash mechanism.**

[GameObserver.cpp:275](/home/magi/Projects/wagicGPT/projects/mtg/src/GameObserver.cpp:275) calls `cleanGarbage()` during ordinary gameplay, while live abilities still exist. The new flag makes **every** `forgetElement()` call return immediately—not merely calls concerning the garbage entry currently being deleted.

For a garbage parent owning a child:

- If the child remains in `mObjects`, deleting the parent can leave that child’s freed address in the live layer.
- If the child also occupies a later garbage slot, deleting the parent leaves that slot non-null, allowing another deletion later in the sweep.
- Menu, mana and waiting-action references are likewise no longer cleared.

These are the ownership/registration combinations the original deregistration facility expressly exists to support. Parent destructors do recursively delete abilities—for example [MayAbility::~MayAbility](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:9147) and [GenericTargetAbility::~GenericTargetAbility](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:9823). Emptying `garbage` afterward does not repair a stale live registration or prevent an earlier duplicate deletion.

**I did not reproduce a concrete card sequence or prove a new deterministic double-free. This is a conditional lifetime failure in the new contract, not a claimed observed crash.**

Also, the “unregistered temporary” optimization is only an **all-containers-empty** test. It does not recognize an unregistered temporary during normal play, so those destructors still scan the live layer and garbage.

### 2. Bind the edict to the affected player, not the victim’s current controller

**Unresolved rules defect in the rewritten gate.**

The new gate obtains `owner` from `victim->controller()` and then verifies that the victim is controlled by that same `owner`. That cannot detect “the targeted player no longer controls this creature.”

This is not merely a hypothetical pointer model:

- [MTGGameZones.cpp:658](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGGameZones.cpp:658) explicitly preserves the same object for battlefield-to-battlefield control changes.
- [MTGGameZones.cpp:1459](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGGameZones.cpp:1459) updates `lastController`.
- [MTGCardInstance::controller()](/home/magi/Projects/wagicGPT/projects/mtg/src/MTGCardInstance.cpp:1031) returns that field.

If the preselected creature changes control during the existing granted-ability response window, the gate can accept it and sacrifice it from the wrong battlefield. If it is then unusable, the replacement candidate scan instead belongs to its **new controller**. Tribute’s lifegain recipient is also derived from the victim controller at [AllAbilities.cpp:7031](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:7031).

[Tribute’s primitive](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/sets/primitives/mtg.txt:124073) instructs the **target opponent** to sacrifice.

> **701.21a** To sacrifice a permanent, its controller moves it from the battlefield directly to its owner’s graveyard. A player can’t sacrifice something that isn’t a permanent, or something that’s a permanent they don’t control. Sacrificing a permanent doesn’t destroy it, so regeneration or other effects that replace destruction can’t affect this action.  
> — [CR 701.21a](/home/magi/.gatelogs/engine-audit/cr.txt:6909)

**Human-choice answer:** for the ordinary dead-victim case, there is a credible human-capable path: mandatory `MayAbility` menu → cloned target ability → waiting chooser → `Owner` routing in [GameObserver.cpp:1214](/home/magi/Projects/wagicGPT/projects/mtg/src/GameObserver.cpp:1214). It is not another lowest-toughness autopick. But the global stamp is written **before that interaction occurs**, and the wrong-seat case remains.

The new null `targetter` correctly avoids the previous persistent bypass:

> **702.18a** Shroud is a static ability. “Shroud” means “This permanent or player can’t be the target of spells or abilities.”  
> — [CR 702.18a](/home/magi/.gatelogs/engine-audit/cr.txt:8177)

### 3. Stop treating fingerprint equality as proof that productive repetition ended

**Unresolved option-removal mechanism.**

The new counter quantities and instance pointers fix real omissions. They do not satisfy the claimed invariant.

Still absent are, among other things:

- Player energy, experience and poison counters.
- Prevention amounts and other effect state.
- Library order.
- Available target sets and mode parameters for an otherwise unchanged ability.

For a concrete omitted state writer, [AAAlterEnergy::resolve](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:2761) increments `Player::energyCount`; the probe never reads it. Another energy gain can leave the same abilities usable and therefore leave the fingerprint unchanged. The same issue applies to [experience counters](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:2806).

At [AIPlayerBaka.cpp:5944](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:5944), repeated equality still overrides `hasAnyLegalAction`. Nothing records that the answered menus were **declines rather than productive accepts**. Thus the predicate cannot prove the property required by the brief. I did not establish a natural 200-consecutive-menu execution reaching this failure.

> **117.3d** If a player has priority and chooses not to take any actions, that player passes. If any mana is in that player’s mana pool, they announce what mana is there. Then the next player in turn order receives priority.  
> — [CR 117.3d](/home/magi/.gatelogs/engine-audit/cr.txt:1911)

The added scan also calls eligibility methods that are not pure readers: [GenericTargetAbility::isReactingToClick](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:9775) can allocate and replace a chooser. This is floor-tick work, not every ordinary tick, but it needs to be counted as shared-path work rather than described as a cheap identity scan.

### 4. Finish the logical-library repair without restoring draw-triggered cancellation

**Incomplete repair, with a demonstrably false ordering assumption.**

The deletion of the draw-close helper is correct:

> **117.2e** Resolving spells and abilities may instruct players to make choices or take actions, or may allow players to activate mana abilities. Even if a player is doing so, no player has priority while a spell or ability is resolving. See rule 608, “Resolving Spells and Abilities.”  
> — [CR 117.2e](/home/magi/.gatelogs/engine-audit/cr.txt:1891)

[Sensation Gorger](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/sets/primitives/mtg.txt:102263) indeed contains the instructed draw inside its reveal processing.

But **“first parked out of this library” is not universally “logical top.”** The reveal implementation has a selective `revealCertainTypes` branch at [AllAbilities.cpp:290](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:290), and [Dwarven Recruiter](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/sets/primitives/mtg.txt:33375) uses it to park only Dwarves. A non-Dwarf can remain above the first parked Dwarf. The new draw code cannot distinguish that from its top-card fixtures.

> **121.1.** A player draws a card by putting the top card of their library into their hand. This is done as a turn-based action during each player’s draw step. It may also be done as part of a cost or effect of a spell or ability.  
> — [CR 121.1](/home/magi/.gatelogs/engine-audit/cr.txt:2283)

> **701.20b** Revealing a card doesn’t cause it to leave the zone it’s in.  
> — [CR 701.20b](/home/magi/.gatelogs/engine-audit/cr.txt:6887)

This establishes the false general assumption; **I did not establish a normal legal in-effect draw during Dwarven Recruiter’s particular reveal**.

The repair also still changes only drawing. Other library readers continue to see the physically depleted library, and there is no transaction ownership/order information in `previousZone`.

**Can the remaining close shut a reveal under a human?** The removed draw helper cannot anymore. An ordinary human-controlled reveal exits the interactive driver with `mAIDriveDone = true` at [AllAbilities.cpp:1155](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:1155), so its stall guard does not time out that ordinary human choice. Conversely, that means the report’s claim that this guard completes any genuinely stalled reveal is too broad. For interactive-AI reveals, timeout plus a partial driver signature still is not proof of impossibility.

### 5. Delete the unused lifegain ability on sacrifice failure

**New definite leak on the new failure branch.**

[AADynamic::resolve](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:7210) allocates `mainAbility = NEW AALifer(...)`, then passes it to `activateMainAbility`. The new `!sacrificed && isEdictShape()` branch returns before the normal `SAFE_DELETE(toActivate)`.

[AADynamic::~AADynamic](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:7403) deletes only `storedAbility`. The abandoned lifegain allocation is neither registered nor subsequently freed. This is an avoidable leak on the exact failure path the commit introduces.

## Search classification: corrected example, still false rationale

The added predicate does **not** mean what its comments and report say. [MTGRevealingCards::Update](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:267) evaluates `number` with `WParsedInt`. A `type:` expression can simply compute **how many top cards** to inspect.

For example, `Nessian Game Warden` uses `reveal:type:forest:mybattlefield`: it looks at the top X library cards, not the whole library. Its creature chooser satisfies this discriminator. Its text independently permits declining, so **this example proves misclassification, not a new illegal decline**.

The actual rule is:

> **701.23b** If a player is searching a hidden zone for cards with a stated quality, such as a card with a certain card type or color, that player isn’t required to find some or all of those cards even if they’re present in that zone.  
> — [CR 701.23b](/home/magi/.gatelogs/engine-audit/cr.txt:6941)

And the quantity-only boundary is:

> **701.23d** If a player is searching a hidden zone simply for a quantity of cards, such as “a card” or “three cards,” that player must find that many cards (or as many as possible, if the zone doesn’t contain enough cards).  
> — [CR 701.23d](/home/magi/.gatelogs/engine-audit/cr.txt:6953)

I found the intended Idyllic Tutor permission preserved and the Mulch example narrowed correctly. I did **not** establish another shipped mandatory non-search card that this revised predicate now permits declining illegally. Do not describe this as validated general search recognition.

## `getMenuControlId`: caller-by-caller result

| Caller | Result |
|---|---|
| Baka `selectMenuOption` | Normal mode menus use their own mode-ranking branch; ordinary readers skip nonpositive slots and bounds-check positive ones. |
| Decision-contract menu construction | Normal mode menus use `MenuAbility::abilities`; ordinary readers skip the sentinel and bounds-check. |
| `DecisionManager::inspectMayBatch` | Rejects nonpositive slots and checks bounds before indexing. |
| `ActionLayer::doReactTo` | Uses lookup success as a gate, then dispatches a mode by `menuIndex`; the sentinel fixes the erroneous early rejection. |
| `getLiveMenuSlot` / human `ButtonPressed` | Human mode dispatch occurs before ordinary slot resolution; ordinary slot zero remains indexable, unlike the AI readers’ convention. |
| `closeUnanswerableMandatoryMenu` | Excludes mode menus and requires a positive ordinary slot; no newly identified slot-zero card-option loss—slot zero normally holds the phase handler. |
| Suite callers | Safe as index consumers, but the synthetic mode witness never proves that a real dispatcher consumes the answer. |

One **retained** caller hazard remains: [ButtonPressedOnMultipleChoice](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionLayer.cpp:975) saves a layer index, calls `reactToChoiceClick`, then indexes `mObjects[currentMenuObject]` again without revalidation. The callback can execute effects. The sentinel repair neither causes nor resolves that mutation hazard.

## What I could not determine

- Actual Vita focus, button handling and human completion of the edict re-choice; registration stamps are not GUI observations.
- A concrete runtime sequence exhibiting the new sweep-bypass UAF/double-free, or proof that existing ownership conventions exclude it.
- Whether destructor-driven mutation can do more than the report’s admitted skipped Update entry; `Update` also retains a raw `currentAction` across its callback.
- An end-to-end surviving mode selection after compaction. The lane explicitly reports that its attempted fixture did not discriminate.
- A natural productive 200-menu sequence reaching the pass floor.
- Full logical-library correctness during selective reveals, reordering, shuffling and draw replacements.
- Runtime correctness of the reported Sensation Gorger scenario, which the lane did not pin.
- Vita performance or the reported suite/A/B results. I read those claims; I did not reproduce them.

**The clean parts should be retained. The lifetime shortcut, wrong-seat edict case, insufficient pass invariant, incomplete library model and new failure-path leak prevent a clean shipment verdict.**