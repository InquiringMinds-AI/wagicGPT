# Rules audit: casting, abilities, and resolution (CR 601–608)

Scope and verdicts follow `00-frame.md`. The rules text audited was the local
`data/sec-601-608-casting.txt` slice. Searches covered `MTGRules`,
`ActionStack`, `MTGAbility`, `ManaCost`, `TargetChooser`,
`PlayRestrictions`, `LegalActions`, zone-change events, card-instance linked
state, and the relevant regression-defect catalogue.

The central limitation is that Wagic does not implement casting/activation as
an atomic CR 601 transaction. The ordinary cast path chooses targets and pays
costs while the card is still in its old zone, then moves it to the stack and
creates the `Spell`. Trigger handling is likewise split between real stack
objects and event-specific immediate resolution. Resolution has no common
target-legality or last-known-information pass; individual ability classes
operate on retained pointers and bespoke state.

## 601

| rule | verdict | evidence / note |
|---|---|---|
| 601.1 | IMPLEMENTED | The play rule distinguishes land play from nonland casting and labels the latter “Cast Card Normally” (`src/MTGRules.cpp:303-345`). |
| 601.1a | IMPLEMENTED | The same entry point routes a land directly to land play and a nonland through the spell stack (`src/MTGRules.cpp:303-345`, `605-641`). |
| 601.2 | PARTIAL | Wagic chooses X/targets and pays costs, then creates a `Spell`, but it does not perform one reversible ordered transaction (`src/MTGRules.cpp:427-560`, `601-641`). |
| 601.2a | INCORRECT | The card is moved to the stack only after mana and extra costs have been paid (`src/MTGRules.cpp:559-633`), whereas 601.2a requires moving it first. |
| 601.2b | PARTIAL | X menus, kicker/bestow, and named alternative-cost paths exist (`src/MTGRules.cpp:427-484`, `561-599`, `900-1004`); there is no general splice or per-symbol hybrid/Phyrexian announcement record. |
| 601.2c | PARTIAL | `TargetChooser` collects targets before payment and preserves them on `Spell` (`src/MTGRules.cpp:545-550`; `src/ActionStack.cpp:377-385`). It has no general solver for must-target effects or separate instances of the word “target.” |
| 601.2d | UNIMPLEMENTED | Searches for cast-time `divide`/`distribute` allocation found no spell object field or target-assignment model recording a positive share per target. |
| 601.2e | PARTIAL | Timing, restrictions, affordability, extra-cost targets, and target readiness are checked (`src/MTGRules.cpp:332-405`, `545-556`), but there is no proposal snapshot/rollback after those asynchronous steps. |
| 601.2f | INCORRECT | The normal path pays the printed mutable cost first, then optionally pays kicker/bestow separately (`src/MTGRules.cpp:559-601`); it does not build and lock one total cost with ordered increases, reductions, additions, and final modifiers. |
| 601.2g | PARTIAL | Human casting can auto-activate untapped producers before payment (`src/MTGRules.cpp:389-423`), but there is no explicit mana-ability window after a locked total cost and before all costs. |
| 601.2h | PARTIAL | Affordability and `canPayExtra()` gates prevent many partial payments (`src/ManaCost.cpp:1012-1037`; `src/MTGRules.cpp:545-601`), but separate asynchronous payments have no general rollback or random/library-public-zone ordering. |
| 601.2i | INCORRECT | The stack zone-change event fires inside `putInZone` before `Spell` construction; the source comments explicitly compensate for cast metadata being absent at that time (`src/MTGRules.cpp:620-641`). Cast/put-on-stack triggers therefore occur before casting is actually completed. |
| 601.3 | PARTIAL | `allowedToCast`, timing flags, `PlayRestrictions`, and zone permission flags gate ordinary casts (`src/MTGRules.cpp:307-350`), but legality is not evaluated through all proposal-dependent characteristics. |
| 601.3a | UNIMPLEMENTED | Searches of `allowedToCast`, `PlayRestrictions`, and cast rules found no prospective evaluation of proposal choices that can remove a prohibition. |
| 601.3b | UNIMPLEMENTED | Flash permission is checked as preexisting `FLASH`/`ASFLASH` flags (`src/MTGRules.cpp:344`), not by prospectively considering proposal choices. |
| 601.3c | PARTIAL | Alternative-cost rules can be offered on otherwise permitted instant-speed paths (`src/MTGRules.cpp:900-1004`), but there is no general “flash only if this additional/alternative cost is paid” permission dependency. |
| 601.3d | PARTIAL | Conditional scripts can grant/remove `ASFLASH`, and the cast gate reads the current flag (`src/MTGRules.cpp:344`); this is update-driven rather than a prospective condition check. |
| 601.3e | UNIMPLEMENTED | Searches for alternative-characteristic legality evaluation found only mechanic-specific mutations (bestow changes types after the spell is added at `src/MTGRules.cpp:1175-1182`). |
| 601.3f | UNIMPLEMENTED | Exile casting is a card permission flag (`src/MTGRules.cpp:307-310`); no face-down-exile visibility/“can look” legality check was found. |
| 601.4 | UNIMPLEMENTED | There is no joint constraint solver among modes, alternative/additional costs, targets, and later choices; the UI presents separate sequential menus. |
| 601.5 | INCORRECT | The engine has no proposal snapshot and no post-proposal permission check. Costs are committed before stack creation (`src/MTGRules.cpp:559-633`) and cannot be rewound if legality changes. |
| 601.5a | UNIMPLEMENTED | No cast transaction records that conditional flash permission existed when casting began. |
| 601.6 | UNIMPLEMENTED | Searches for an opponent-as-chooser field on `Spell`, cast menus, and `TargetChooser` found no way for the opponent to make the caster’s mode/target choices. |
| 601.6a | N/A | Multiple-opponent chooser selection is outside the two-player game types in the audit frame. |
| 601.6b | UNIMPLEMENTED | No casting-choice APNAP/“controller first” sequence exists; casting menus are owned by the acting player. |
| 601.7 | IMPLEMENTED | Each `Spell` owns a copied paid `cost`, and cost-changing abilities do not recalculate already-created stack objects (`src/ActionStack.cpp:364-400`, `456-460`). |

## 602

| rule | verdict | evidence / note |
|---|---|---|
| 602.1 | IMPLEMENTED | Activated effects are represented by `ActivatedAbility`, with a `ManaCost`/extra-cost object and a resolving effect (`include/MTGAbility.h:313-347`). |
| 602.1a | IMPLEMENTED | Click activation requires the source controller to be the acting player and pays from that acting player’s pool (`src/MTGAbility.cpp:7650-7655`, `7719-7724`, `7796-7832`). |
| 602.1b | PARTIAL | Timing enum restrictions, per-turn limits, and parsed cast restrictions are continuously checked (`src/MTGAbility.cpp:7655-7718`), but arbitrary Oracle activation instructions are not a first-class text component. |
| 602.1c | IMPLEMENTED | Only `ActivatedAbility` exposes the activation click/payment path; trigger and static observers use separate classes (`include/MTGAbility.h:267-347`, `383-430`). |
| 602.1d | IMPLEMENTED | Activated abilities use the activation path and are not treated as spells (`src/MTGAbility.cpp:7876-7927`; `src/ActionStack.cpp:325-329`). |
| 602.1e | PARTIAL | Ability costs are mutable `ManaCost` objects and affordability sees their extra costs (`src/MTGAbility.cpp:7758-7791`), but there is no single locked total-cost object or complete modifier ordering. |
| 602.2 | PARTIAL | Targets and costs are selected/paid and nonmana abilities enter `ActionStack` (`src/MTGAbility.cpp:7796-7927`), but the ability is not put on the stack first and illegal activation rollback is absent. |
| 602.2a | INCORRECT | `fireAbility()` creates the stack action only after targets and costs have been committed (`src/MTGAbility.cpp:7556-7562`, `7796-7927`); hidden-zone reveal and initial top-of-stack creation are not performed first. |
| 602.2b | PARTIAL | Activation reuses the same target, extra-cost, mana-payment, and stack classes as casting (`src/MTGAbility.cpp:7758-7927`), but inherits the non-transactional ordering defects identified for 601.2. |
| 602.3 | UNIMPLEMENTED | No activated-ability field or chooser route delegates the activator’s choices to an opponent. |
| 602.3a | N/A | Selection among multiple opponents is outside supported two-player game types. |
| 602.3b | UNIMPLEMENTED | No activation-time simultaneous-instruction ordering mechanism gives the controller’s choice/action first. |
| 602.4 | IMPLEMENTED | Stack abilities retain their own ability/cost state once added; later cost mutations do not recalculate them (`src/ActionStack.cpp:325-329`; `src/MTGAbility.cpp:7556-7562`). |
| 602.5 | PARTIAL | `NOACTIVATED`, `NOMANA`, `ONLYMANA`, timing restrictions, and parsed conditions prohibit supported activation classes (`src/MTGAbility.cpp:7712-7757`), but prohibition coverage is script/flag-specific. |
| 602.5a | IMPLEMENTED | Tap-cost activation requires an untapped source without summoning sickness, whose haste handling feeds `hasSummoningSickness()` (`src/MTGAbility.cpp:7719-7720`, `8765-8769`). |
| 602.5b | IMPLEMENTED | Use count is stored on the live ability object, incremented on activation rather than resolution, and reset only at turn cleanup (`include/MTGAbility.h:331-337`; `src/MTGAbility.cpp:7698-7710`, `7921-7927`). |
| 602.5c | PARTIAL | Granted abilities are separate live ability instances with separate counters, but the engine has no explicit acquired-from identity for enforcing all restriction-linkage cases. |
| 602.5d | IMPLEMENTED | `AS_SORCERY` checks active turn, main phase, and empty stack without requiring a sorcery card (`src/MTGAbility.cpp:7666-7675`). |
| 602.5e | PARTIAL | Unrestricted activated abilities use instant timing/priority in ordinary play, but there is no distinct “activate only as an instant” semantic restriction to preserve under rule-changing effects. |

## 603

| rule | verdict | evidence / note |
|---|---|---|
| 603.1 | PARTIAL | Trigger and phase-trigger classes recognize event/phase conditions and wrap payloads (`include/MTGAbility.h:267-311`, `433-495`), but the parser is a finite card-script lexicon rather than general trigger syntax. |
| 603.1a | UNIMPLEMENTED | Searches for post-effect triggered-ability targeting instructions and “can’t be countered” stack text found no separate instruction field on `TriggeredAbility`. |
| 603.1b | UNIMPLEMENTED | No trigger object records a set of several conditions and whether all occurred during a stated period. |
| 603.2 | PARTIAL | Events call `triggerOnEvent`, while phase triggers compare phase transitions (`src/MTGAbility.cpp:8132-8197`, `8439-8517`). Some event classes incorrectly resolve immediately. |
| 603.2a | PARTIAL | Trigger observers fire without using casting/activation legality (`src/MTGAbility.cpp:8132-8197`), but source-zone flags and bespoke immediate paths make this incomplete. |
| 603.2b | IMPLEMENTED | `TriggerAtPhase` fires on transition into the configured phase (`src/MTGAbility.cpp:8439-8517`). |
| 603.2c | PARTIAL | Each received event invokes a trigger once and `once` can disable it (`src/MTGAbility.cpp:8212-8235`), but compound-event occurrence counting is not generalized. |
| 603.2d | UNIMPLEMENTED | Searches for triggered-ability doubling/additional-trigger counts found no event-to-trigger multiplicity transformation. |
| 603.2e | PARTIAL | Dedicated events model many “becomes” transitions, and triggers react to those events rather than persistent state (`src/MTGAbility.cpp:8132-8190`); event coverage is incomplete. |
| 603.2f | UNIMPLEMENTED | No all-players-visibility test occurs in `Trigger::triggerOnEvent`; it checks active/phased/restriction state only (`src/MTGAbility.cpp:8212-8235`). |
| 603.2g | PARTIAL | Triggering follows emitted engine events, so an event suppressed by supported replacement machinery is absent, but the replacement/event model is incomplete and many direct mutations bypass it. |
| 603.2h | UNIMPLEMENTED | No per-source record of whether a trigger’s indicated action was performed for “Do this only once each turn” was found. |
| 603.3 | INCORRECT | Target, sacrifice, exploit, discard, cycling, and stack-zone events call `resolve()` immediately (`src/MTGAbility.cpp:8132-8187`) instead of waiting for priority and entering the stack. |
| 603.3a | PARTIAL | A trigger retains its source and normally uses that source’s controller, but no controller snapshot is stored at trigger time (`src/MTGAbility.cpp:8122-8129`, `8574-8591`). |
| 603.3b | INCORRECT | Triggers fire in observer/event iteration order, with several resolving immediately; there is no APNAP collection and controller-chosen ordering (`src/MTGAbility.cpp:8132-8197`). |
| 603.3c | PARTIAL | Trigger payloads can use `MenuAbility`/modal scripts, but `GenericTriggeredAbility` has no general legal-mode filtering/removal step (`src/MTGAbility.cpp:8662-8694`). |
| 603.3d | PARTIAL | Targeted trigger payloads now open mandatory chooser machinery and do nothing if no legal target (`src/MTGAbility.cpp:8670-8688`). Multiple simultaneous interactive payloads remain catalogued as the dual-interactive-ETB defect (`strategy-design/lexicon-triage/engine-bugs.md:25-37`). |
| 603.4 | PARTIAL | Parsed `castRestriction` conditions are checked when the trigger event/phase occurs (`src/MTGAbility.cpp:8225-8234`, `8503-8511`), but are not checked again at `GenericTriggeredAbility::resolve()` (`8662-8694`). |
| 603.5 | IMPLEMENTED | Optional payloads are represented by a `MayAbility` that is offered when the triggered payload resolves; mandatory targeted triggers use the same chooser with `must=true` (`src/MTGAbility.cpp:8683-8687`). |
| 603.6 | PARTIAL | `WEventZoneChange` carries zone movement and triggers can retain an event target (`src/MTGAbility.cpp:8179-8187`, `8599-8617`), but new-object identity and hidden-zone lookup are not generalized. |
| 603.6a | PARTIAL | Battlefield zone changes emit events seen by installed trigger observers, supporting common ETB scripts (`src/MTGAbility.cpp:8132-8190`); event ordering/parallel ETB handling is not complete. |
| 603.6b | INCORRECT | Static/lord effects discover and mutate entrants only during later `ListMaintainerAbility::Update()` scans (`src/MTGAbility.cpp:8360-8415`), so an entrant can be observed by ETB triggers before its continuously modified characteristics exist. |
| 603.6c | PARTIAL | Leaves-zone events expose the moved object/from-zone to triggers, but the special LKI trigger set is not centrally determined (`src/MTGAbility.cpp:8179-8187`, `8599-8617`). |
| 603.6d | PARTIAL | “Enters with/as/tapped” effects have bespoke card-script and zone-entry paths, but there is no unified static replacement pass; live abilities otherwise update after entry (`src/MTGAbility.cpp:8360-8415`). |
| 603.6e | UNIMPLEMENTED | Searches for Aura-specific paired LTB tracking found no mechanism allowing both the enchanted card’s new object and the Aura’s post-SBA graveyard object to be found. |
| 603.7 | PARTIAL | `TriggerNextPhase`, `TriggerRebound`, and persistent generic trigger wrappers implement common delayed triggers (`src/MTGAbility.cpp:8525-8571`). Arbitrary delayed-event semantics are script-specific. |
| 603.7a | PARTIAL | Resolving abilities can install trigger observers for later phases/events, but creation by replacement/static permissions and universal “not before creation” semantics are absent. |
| 603.7b | PARTIAL | Delayed phase triggers have `once`/destruction state (`src/MTGAbility.cpp:8525-8566`), but simultaneous multiple-event choice and all stated durations are not represented. |
| 603.7c | INCORRECT | Delayed wrappers retain raw card/target pointers and use current zone checks (`src/MTGAbility.cpp:8574-8591`, `8662-8694`), without CR object identity capable of distinguishing leave-and-return in every path. |
| 603.7d | PARTIAL | A spell-created wrapper retains the spell card as `source`, but delayed controller is not separately snapshotted at spell resolution (`src/MTGAbility.cpp:8574-8591`). |
| 603.7e | PARTIAL | Ability-created wrappers propagate source/target pointers (`src/MTGAbility.cpp:7603-7637`, `8574-8591`), but not an explicit resolving ability controller snapshot. |
| 603.7f | UNIMPLEMENTED | No replacement-effect object creates delayed triggers while recording the static source/controller at application time. |
| 603.7g | UNIMPLEMENTED | No static-permission action object was found that creates and controller-stamps a delayed trigger when the permitted action is taken. |
| 603.7h | UNIMPLEMENTED | No delayed trigger records how many times its creating ability has resolved during a turn. |
| 603.8 | PARTIAL | A live phase/state trigger is suppressed while its same ability object is already on the stack by `MTGAbility::testDestroy()`/stack membership (`src/MTGAbility.cpp:7547-7553`), but there is no general state-trigger class or exact rearm event. |
| 603.9 | N/A | In supported two-player games, a player losing ends the game immediately; multiplayer leave-game trigger handling is outside the audit frame. |
| 603.10 | PARTIAL | Event objects preserve some pre-change facts and are delivered during moves, supporting common LTB cases, but there is no enumerated look-back trigger pass (`src/MTGAbility.cpp:8132-8190`). |
| 603.10a | PARTIAL | Zone-change triggers receive movement events, but graveyard-leave and public-to-hand/library look-back behavior is parser/event dependent rather than a common rule. |
| 603.10b | UNIMPLEMENTED | Searches for phase-out triggered events/look-back handling found only the `isPhased` state, not a trigger event pipeline. |
| 603.10c | UNIMPLEMENTED | No becomes-unattached look-back trigger classifier was found. |
| 603.10d | UNIMPLEMENTED | Control changes are direct mutations; no look-back trigger event for loses/gains control was found. |
| 603.10e | PARTIAL | Countering uses `ActionStack::Fizzle` and can emit zone-change facts, but there is no general pre-counter spell snapshot specifically used for countered triggers (`src/ActionStack.cpp:1421-1459`). |
| 603.10f | N/A | Player-leaves/loss trigger ordering is relevant to multiplayer continuation, excluded by the audit frame. |
| 603.10g | N/A | Plane/planeswalk-away triggers belong to excluded Planechase. |
| 603.11 | PARTIAL | A static ability and triggers can coexist in one parsed card script and share source state, but the engine has no explicit linked-pair relation (`include/MTGCardInstance.h:121-123`, `207-210`). |
| 603.12 | UNIMPLEMENTED | Searches for reflexive-trigger objects or “this way” event capture during a still-resolving ability found no implementation. |
| 603.12a | UNIMPLEMENTED | With no reflexive-trigger model, neither per-occurrence triggering nor the “paid one or more times” exception can be represented. |

## 604

| rule | verdict | evidence / note |
|---|---|---|
| 604.1 | IMPLEMENTED | Static-style abilities are installed as live `MTGAbility`/`ListMaintainerAbility` observers rather than activated or triggered stack actions (`include/MTGAbility.h:383-430`). |
| 604.2 | PARTIAL | Live observers create ongoing characteristic/rule effects and some registered prevention/replacement effects, but coverage is class-specific and update-driven (`src/MTGAbility.cpp:8360-8415`). |
| 604.3 | PARTIAL | The engine has CDA helpers for color/type/P/T and can read them in several zones, but does not uniformly evaluate CDAs in all zones/outside the game (see `src/MTGCardInstance.cpp:970-1000`). |
| 604.3a | PARTIAL | `cdaactive`/CDA fields cover common P/T definitions, but the five defining criteria are not validated as a general static-ability category (`include/AllAbilities.h:3029-3041`). |
| 604.4 | IMPLEMENTED | Attachment modifiers are live target-bound abilities; moving/removing the attachment causes the old child to be removed and a new eligible target to be scanned (`src/MTGAbility.cpp:8288-8356`). |
| 604.5 | PARTIAL | Alternative/additional-cost and cast-permission fields function while a card is cast (`src/MTGRules.cpp:900-1004`, `1111-1145`), but arbitrary spell-stack static abilities are not a general category. |
| 604.6 | PARTIAL | Hand/library/graveyard/exile cast permissions and prohibitions are consulted before casting (`src/MTGRules.cpp:307-350`), with supported zones/phrases represented by flags rather than all such static text. |
| 604.7 | IMPLEMENTED | Static observers apply from their currently live source/target and are destroyed when their required source zone is lost; they do not request an LKI snapshot (`src/MTGAbility.cpp:7528-7553`). |

## 605

| rule | verdict | evidence / note |
|---|---|---|
| 605.1 | PARTIAL | Wagic has an `AManaProducer` class and immediate path (`src/MTGAbility.cpp:8733-8823`), but classification is parser/class based rather than derived from all CR criteria. |
| 605.1a | PARTIAL | Ordinary nonloyalty, untargeted mana producers map to `AManaProducer`, but the class itself does not reject targets or loyalty status (`src/MTGAbility.cpp:8733-8745`). |
| 605.1b | UNIMPLEMENTED | Searches for a triggered-mana-ability subclass/criteria found none; mana-production events are ordinary trigger events (`src/MTGAbility.cpp:8796-8805`). |
| 605.2 | PARTIAL | An `AManaProducer` remains on the immediate mana path regardless of current output viability, but this is class identity rather than rules-derived mana-ability status (`src/MTGAbility.cpp:8733-8783`). |
| 605.3 | PARTIAL | Mana producers reuse activation legality and cost payment, then special-case immediate resolution (`src/MTGAbility.cpp:8825-8864`); they inherit incomplete transaction/rollback semantics. |
| 605.3a | PARTIAL | Producers can be used with priority and human auto-tapping supplies mana during casting (`src/MTGRules.cpp:389-423`; `src/MTGAbility.cpp:8748-8783`), but arbitrary mid-resolution mana-payment windows are not centralized. |
| 605.3b | IMPLEMENTED | `ActivatedAbility::activateAbility()` calls a mana producer’s `resolve()` directly and returns without `fireAbility()`/stack insertion (`src/MTGAbility.cpp:7876-7919`). |
| 605.3c | IMPLEMENTED | Immediate `resolve()` completes synchronously before control returns to the activation path, so that same activation cannot be reentered before completion (`src/MTGAbility.cpp:7911-7919`). |
| 605.4 | UNIMPLEMENTED | There is no triggered-mana-ability classification to which the normal triggered rules plus an exception can apply. |
| 605.4a | UNIMPLEMENTED | Mana-production triggers use ordinary `TriggeredAbility` handling; no triggered mana payload is guaranteed to resolve immediately without the stack. |
| 605.5 | PARTIAL | Spells remain `Spell` objects and most nonproducer abilities use the stack, but mana-ability identity is determined by construction as `AManaProducer`, not by the complete criteria (`src/ActionStack.cpp:350-400`; `src/MTGAbility.cpp:8733-8745`). |
| 605.5a | INCORRECT | `AManaProducer` is an `ActivatedAbilityTP` with a target and immediately adds mana to that target’s player (`src/MTGAbility.cpp:8733-8739`, `8785-8793`); the class does not demote targeted producers to ordinary stack abilities. |
| 605.5b | IMPLEMENTED | Mana-producing cards cast as spells are ordinary `Spell` stack objects and resolve through card abilities (`src/ActionStack.cpp:350-400`, `462-498`). |

## 606

| rule | verdict | evidence / note |
|---|---|---|
| 606.1 | PARTIAL | Planeswalker-source activations receive special timing/use/counter checks (`src/MTGAbility.cpp:7725-7746`), but those checks are not attached specifically to loyalty abilities. |
| 606.2 | INCORRECT | The special handling is keyed on the source card having type Planeswalker (`src/MTGAbility.cpp:7725`), so every activated ability of a planeswalker is treated as loyalty-related rather than only abilities with loyalty symbols. |
| 606.3 | PARTIAL | The engine enforces controller turn, main phase, and prior-use checks (`src/MTGAbility.cpp:7725-7746`). Empty-stack timing depends on the separately parsed restriction rather than the loyalty check itself. |
| 606.4 | PARTIAL | Loyalty-counter changes can be activation extra costs and are paid before the ability stacks (`src/MTGAbility.cpp:7758-7791`, `7796-7832`), but cost modification/loyalty-symbol identity is not generalized. |
| 606.5 | UNIMPLEMENTED | Searches for combining multiple loyalty-counter costs into one net add/remove cost found no aggregation step. |
| 606.6 | PARTIAL | Extra-cost `canPay()`/`canPayExtra()` blocks supported negative counter costs when counters are insufficient (`src/MTGAbility.cpp:7770-7791`), but the check is not a complete combined-loyalty-cost calculation. |

## 607

| rule | verdict | evidence / note |
|---|---|---|
| 607.1 | PARTIAL | Card instances carry source-local imprinted cards, chosen values, and stored-source pointers (`include/MTGCardInstance.h:121-123`, `207-210`, `316-334`), supporting common links without a general linked-ability relation. |
| 607.1a | UNIMPLEMENTED | No metadata marks an ability printed inside a granted ability as printed on the recipient for linkage. |
| 607.1b | PARTIAL | Face changes copy/preserve some card-instance state, so source-local choices can survive faces, but there is no face-independent printed-ability linkage map. |
| 607.1c | UNIMPLEMENTED | No ability can be explicitly linked to itself; linkage is implicit in bespoke fields and scripts. |
| 607.1d | UNIMPLEMENTED | `storedSourceCard` supports some granted/token payload source recovery, but no two-object linked-ability relationship is created (`src/MTGAbility.cpp:3683-3684`, `3795-3796`). |
| 607.2 | PARTIAL | Several listed patterns are approximated by imprint/chosen/paid fields, but there is no type system enumerating linked-ability kinds. |
| 607.2a | PARTIAL | Imprint pushes exiled objects onto the source’s `imprintedCards` and records names (`src/AllAbilities.cpp:2848-2865`), but linkage to one particular printed exiling ability is implicit. |
| 607.2b | UNIMPLEMENTED | Replacement effects do not record which exiles were their direct results for a linked second ability. |
| 607.2c | UNIMPLEMENTED | Token/put-onto-battlefield operations may retain a creator source, but no collection records precisely the objects created/put by one printed ability. |
| 607.2d | PARTIAL | Chosen color/type/name values are stored on the card and later effects read them (`src/AllAbilities.cpp:3871`, `3930`, `3987`; `src/AllAbilities.cpp:8627-8656`). Multiple independently linked choices are not represented. |
| 607.2e | UNIMPLEMENTED | No general per-ability “noted information” store/link was found beyond fixed chosen/imprint fields. |
| 607.2f | UNIMPLEMENTED | Searches for arbitrary paired word-choice/word-reference linkage found no general mechanism. |
| 607.2g | UNIMPLEMENTED | No record associates a particular as-enters cost payment with a second printed ability. |
| 607.2h | PARTIAL | Same-source static and trigger scripts can share card-instance state, but there is no paragraph/link identity restricting the trigger to actions of that static ability. |
| 607.2i | PARTIAL | Kicker/alternative payment intent is stored on the card and queried by `Spell::FullfilledAlternateCost` (`src/ActionStack.cpp:411-449`), covering common paid/additional-cost links. |
| 607.2j | PARTIAL | Announced X and paid-cost method are copied onto the spell/permanent (`src/MTGRules.cpp:654-659`; `src/ActionStack.cpp:469-480`), but arbitrary variable additional costs do not get independent linked records. |
| 607.2k | PARTIAL | Champion-style scripts can use the source’s imprint collection, but that is a card-script convention rather than an explicit keyword-linked pair (`src/AllAbilities.cpp:2848-2865`). |
| 607.2m | UNIMPLEMENTED | Searches for anchor-word storage/linkage found no engine representation. |
| 607.2n | UNIMPLEMENTED | No pregame named-card exile collection is shared across every object with the specified name. |
| 607.2p | UNIMPLEMENTED | No pregame CDA-choice store persists as a linked value through every zone change. |
| 607.2q | UNIMPLEMENTED | Extra-cost exiles are not collected into a spell-to-resulting-permanent linked set; `alternateCostPaid` records only a cost method (`src/ActionStack.cpp:411-449`). |
| 607.3 | INCORRECT | Imprint can hold multiple cards, but consumers commonly use only `imprintedCards.back()`/`currentimprintName` (`src/AllAbilities.cpp:2848-2865`, `8743-8745`), not every exiled card or the required sum/choice semantics. |
| 607.4 | UNIMPLEMENTED | There is no link object, so one ability cannot be represented as a member of multiple explicit linked pairs. |
| 607.5 | UNIMPLEMENTED | Gained abilities are cloned/live abilities with no same-effect linked-pair identity preventing links to other abilities. |
| 607.5a | PARTIAL | Unset fixed choice fields default to sentinel/empty values (`src/MTGCardInstance.cpp:288-290`), making many references do nothing, but undefined choice is not tracked per gained linked pair. |

## 608

| rule | verdict | evidence / note |
|---|---|---|
| 608.1 | IMPLEMENTED | `ActionStack` identifies the latest unresolved object and resolves it after both players pass (`src/ActionStack.cpp:902-918`, `1181-1215`). |
| 608.2 | PARTIAL | A stack object calls its class-specific `resolve()` synchronously (`src/ActionStack.cpp:902-918`); there is no common ordered 608.2 precheck/finalization pipeline. |
| 608.2a | UNIMPLEMENTED | Trigger restrictions are checked when triggering, but `GenericTriggeredAbility::resolve()` does not recheck an intervening-if condition (`src/MTGAbility.cpp:8225-8234`, `8662-8694`). |
| 608.2b | INCORRECT | `TargetAbility::resolve()` iterates retained targets and only rejects phased permanents (`src/MTGAbility.cpp:8052-8098`). It neither revalidates chooser legality nor counters an all-illegal-target spell/ability; illegal remaining targets can be affected. |
| 608.2c | PARTIAL | Parsed/multi abilities generally invoke child payloads in stored order, and individual effects can use replacements, but English cross-clause semantics and replacement coverage are bespoke rather than general. |
| 608.2d | PARTIAL | Resolution-time `MenuAbility` choices exist and pending menus hold stack progress (`src/ActionStack.cpp:1091-1097`), but legal/impossible-option and untargeted distribution rules are not centralized. |
| 608.2e | INCORRECT | Multi-player/object payloads resolve through sequential child/target loops (`src/MTGAbility.cpp:8074-8095`), with no APNAP choice collection followed by simultaneous action. |
| 608.2f | INCORRECT | `TargetAbility` resolves the effect once per target in sequence (`src/MTGAbility.cpp:8074-8095`), so actions that should be simultaneous are observably ordered. |
| 608.2g | PARTIAL | Cast-during-resolution effects can put a card on top of `ActionStack`, and the current C++ resolver continues synchronously, approximating the nested sequence; there is no universal prohibition/window guard for every other activation during a pending resolution. |
| 608.2h | PARTIAL | Parsed values are commonly evaluated when an effect’s `resolve()` runs, but there is no general LKI snapshot when the referenced object left its expected public zone. |
| 608.2i | UNIMPLEMENTED | Searches for prior-game-state snapshots/history queries usable by arbitrary effects found only narrow turn counters/zone histories, not the rule’s general look-back information model. |
| 608.2j | PARTIAL | Descriptor/parsed-value effects can query a named characteristic independently, but individual card implementations may conflate related stored fields; no common resolving-effect rule enforces this. |
| 608.2k | PARTIAL | Costs and triggers can retain a target/source pointer for later effect use (`src/MTGAbility.cpp:7603-7637`), but zone moves recreate instances and there is no general “specific untargeted object” identity rule. |
| 608.2m | IMPLEMENTED | Resolution is a synchronous C++ call; moving the source during that call does not itself stop the remaining method/child calls (`src/ActionStack.cpp:902-918`). |
| 608.2n | IMPLEMENTED | After instant/sorcery abilities are installed/resolved, the spell is moved from stack to its owner’s graveyard (or the applicable replacement destination) (`src/MTGAbility.cpp:7353-7384`); resolved stack abilities are garbage-collected (`src/ActionStack.cpp:1397-1417`). |
| 608.2p | UNIMPLEMENTED | Searches of `WEvent` and `ActionStack::resolve` found no general spell-or-ability-resolved event; only damage-stack resolution has a dedicated event. |
| 608.3 | PARTIAL | `Spell::resolve()` distinguishes instant/sorcery from other spells and moves a permanent spell to the battlefield (`src/ActionStack.cpp:462-498`), but lacks the required target/type-specific prechecks. |
| 608.3a | IMPLEMENTED | A non-instant/non-sorcery spell is moved from its stack zone to its controller’s battlefield (`src/ActionStack.cpp:466-481`). |
| 608.3b | INCORRECT | `Spell::resolve()` moves every permanent spell to the battlefield without first validating its target or applying bestowed-Aura/mutate fallback (`src/ActionStack.cpp:462-481`). Bestow’s type is merely changed after stack insertion (`src/MTGRules.cpp:1175-1182`). |
| 608.3c | PARTIAL | Aura targeting/attachment is expressible by target-bound card abilities, but the common permanent-spell resolver enters the permanent before any Aura-specific attach step (`src/ActionStack.cpp:462-498`). |
| 608.3d | PARTIAL | Mutate has mechanic-specific merge handling, but it is not a branch in `Spell::resolve`; the known catalogue confirms merge can work while mutate-trigger parsing fails (`strategy-design/lexicon-triage/engine-bugs.md:281-284`). |
| 608.3e | UNIMPLEMENTED | No “cannot enter the battlefield” check/alternate graveyard step appears in the permanent-spell resolver (`src/ActionStack.cpp:462-481`). |
| 608.3f | PARTIAL | Spell copies and token/permanent copies exist, but the resolver has no explicit copied-permanent-spell-to-token step or “not created” event distinction (`src/ActionStack.cpp:462-498`). |
| 608.3g | PARTIAL | Dash/blitz-like delayed wrappers can be installed from card abilities as the permanent enters, but no general stack-static delayed-trigger creation step exists, and warp-specific semantics were not found. |

## Verdict counts

| verdict | count |
|---|---:|
| IMPLEMENTED | 23 |
| PARTIAL | 77 |
| INCORRECT | 16 |
| UNIMPLEMENTED | 47 |
| N/A | 5 |
| **TOTAL** | **168** |

## HIGHLIGHTS

1. **Casting and activation are not CR transactions (601.2, 602.2).** Costs are
   paid before the spell/ability object is put on the stack, total costs are
   not locked as one ordered value, and there is no snapshot/rollback. This is
   the root issue behind many proposal-dependent legality and cost defects.
2. **Target legality is not rechecked at resolution (608.2b, 608.3b).**
   `TargetAbility::resolve()` reuses retained targets and rejects only phasing;
   permanent spells enter before any target check. All-target-illegal fizzle,
   partial legal-target resolution, Aura failure, bestow fallback, and mutate
   fallback are therefore not generally rules-correct.
3. **Several triggered events resolve immediately (603.3, 603.3b).** Target,
   sacrifice, exploit, discard, cycling, and stack-zone events bypass the
   stack, eliminating responses and APNAP/controller ordering.
4. **Simultaneous resolution is sequential (608.2e–f).** Multi-target effects
   loop target-by-target, permitting intermediate triggers/state changes that
   the CR requires to happen only after a simultaneous action.
5. **Triggered mana abilities are absent and targeted mana producers are
   misclassified (605.1b, 605.4a, 605.5a).** Mana-ability status is a parser
   class, not a result of the CR criteria.
6. **Loyalty handling is keyed to the source being a planeswalker (606.2).**
   This applies loyalty timing/use restrictions to every activated ability on
   a planeswalker, while combined loyalty-counter costs are not modeled.
7. **Linked abilities are fixed card-instance fields, not links (607).**
   Imprint/chosen-value/paid-cost idioms cover common cards, but multiple links,
   gained linked pairs, replacement-result links, and multi-card “the exiled
   card” semantics are missing or incorrect.
8. **No post-resolution trigger event exists (608.2p), and intervening-if
   conditions are not rechecked at resolution (608.2a/603.4).**
