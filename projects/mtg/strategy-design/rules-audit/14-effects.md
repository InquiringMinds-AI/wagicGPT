# Rules audit: effects (CR 609–616)

Scope and verdicts follow `00-frame.md`. The rules text audited was the local
`data/sec-609-616-effects.txt` slice. Code searches covered `MTGAbility`,
`AllAbilities`, `ReplacementEffects`, `WEvent`, `ManaCost`, `Damage`,
`Counters`, `MTGCardInstance`, `ActionLayer`, `GameObserver`, and the card
scripts/tests for text-changing, prevention, replacement, phasing, and splice
terms.

The central architectural fact for this section is that Wagic does **not**
compute characteristics from a base object by applying an ordered collection
of continuous effects. Active abilities instead mutate shared
`MTGCardInstance` fields when added, updated, or destroyed. Lords rescan zones
on `ActionLayer` update ticks and add/remove child abilities. There is no
general layer number, effect timestamp, dependency graph, or affected-player
replacement-order object in this machinery. Some P/T helpers deliberately
approximate the four layer-7 sublayers, but that local arithmetic is not a CR
613 evaluator.

## 609

| rule | verdict | evidence / note |
|---|---|---|
| 609.1 | IMPLEMENTED | One-shot payloads call `resolve()`, while non-one-shot payloads call `addToGame()` and remain active (`src/MTGAbility.cpp:8662-8694`). Static-style abilities likewise live in the `ActionLayer`. |
| 609.2 | PARTIAL | Target choosers explicitly select zones and `ListMaintainerAbility` scans only zones a chooser reports (`src/MTGAbility.cpp:8275-8285`, `8310-8331`). This supports many stated nonbattlefield effects, but there is no general “permanents unless stated otherwise” semantic validation. |
| 609.3 | PARTIAL | Many individual effects guard missing/invalid targets (for example `AAPhaseOut`, `src/AllAbilities.cpp:2797-2815`), but partial performance is not centralized and varies by effect implementation. |
| 609.4 | PARTIAL | Individual “as though” cases are represented by flags and special paths (for example library-top permissions in `src/MTGCardInstance.cpp:1076-1105`), not a scoped condition system applicable only to the stated effect. |
| 609.4a | UNIMPLEMENTED | Search for `as though` found no engine effect-combination machinery; independent special-case flags cannot combine arbitrary differently worded “as though” effects. |
| 609.4b | PARTIAL | `ANYTYPEOFMANAABILITY` makes affordability treat the cost as generic (`src/ManaCost.cpp:1070-1087`) and payment redistributes pool colors without changing the cost object (`1040-1065`). The engine does not preserve a reliable typed record of which mana was actually spent through this conversion. |
| 609.5 | UNIMPLEMENTED | Search for effect-level tie handling found no default-or-instruction framework; any tie behavior must be coded in the individual ability/menu. |
| 609.6 | PARTIAL | `ReplacementEffects` exists, but only damage prevention, counter prevention, and draw replacement subclasses are registered (`include/ReplacementEffects.h:13-66`). Many other “instead,” prevention, and “can’t” effects bypass it or are absent. |
| 609.7 | PARTIAL | `REDamagePrevention` can filter a damage event by source and target choosers (`src/ReplacementEffects.cpp:14-44`), but this is not a complete source-selection/effect model. |
| 609.7a | PARTIAL | Circle of Protection can choose a permanent or spell and converts the choice to a card source chooser (`include/AllAbilities.h:3356-3369`). The enumerated legal choices (including arbitrary referenced objects and command-zone objects) are not generally represented. |
| 609.7b | PARTIAL | The source and target choosers are rechecked when damage is about to occur, and a mismatch leaves the shield unused (`src/ReplacementEffects.cpp:16-24`, `44`). Spell-to-resulting-permanent identity and every legal source category are not tracked. |
| 609.7c | PARTIAL | Static prevention installs a live chooser-based modifier (`src/AllAbilities.cpp:9358-9385`), so current properties are consulted at an event. Coverage is limited to Wagic’s chooser-visible damage sources and supported property predicates. |

## 610

| rule | verdict | evidence / note |
|---|---|---|
| 610.1 | IMPLEMENTED | Damage, movement, counters, tokens, and similar payloads are ordinary one-shot abilities; one-shot children are resolved and deleted rather than installed (`include/AllAbilities.h:3702-3711`). |
| 610.2 | IMPLEMENTED | Delayed triggered work is supported by persistent trigger/phase wrappers; `GenericTriggeredAbility` resolves its stored payload later (`src/MTGAbility.cpp:8574-8694`). |
| 610.3 | PARTIAL | `ABlink` can exile and return immediately, at end of turn, or when its source leaves (`src/AllAbilities.cpp:10347-10364`, `10418-10442`). It is not a general “until any specified event” second-one-shot model. |
| 610.3a | INCORRECT | If the source-leaves event already occurred, `ABlink` still exiles the target first (`src/AllAbilities.cpp:10418-10420`) and merely clears its return marker afterward (`10431-10438`). CR 610.3a says the object does not move. |
| 610.3b | INCORRECT | The same `ABlink` path has no record of whether an event occurred after a trigger fired but before its resolution; it moves first and tests only the source’s current battlefield state (`src/AllAbilities.cpp:10418-10438`). |
| 610.3c | IMPLEMENTED | A non-hand blink returns through the target owner’s battlefield (`src/AllAbilities.cpp:10454-10458`). |
| 610.3d | UNIMPLEMENTED | Return operations occur in per-ability `Update()` calls and zone moves, not as a simultaneous batch (`src/ActionLayer.cpp:172-181`; `src/AllAbilities.cpp:10347-10364`). |
| 610.4 | UNIMPLEMENTED | `AAPhaseOut` only sets `isPhased` and a turn marker (`src/AllAbilities.cpp:2797-2811`); there is no “phase out until specified event” paired-one-shot object. |
| 610.4a | INCORRECT | All phased-out permanents, including ones phased out by `AAPhaseOut`, phase in during their controller’s next untap because `phasingPhase()` tests the undifferentiated `isPhased` flag (`src/GameObserver.cpp:1921-1946`). An “until” phased-out permanent must not phase in from that turn-based action. |
| 610.4b | UNIMPLEMENTED | No phase-out effect records the activated ability’s stack time or whether its specified event has already occurred; `AAPhaseOut::resolve()` always sets the flag for a valid target (`src/AllAbilities.cpp:2797-2814`). |
| 610.4c | UNIMPLEMENTED | No phase-out effect records trigger time or performs the trigger-time intervening-event test (`src/AllAbilities.cpp:2797-2814`). |
| 610.4d | UNIMPLEMENTED | Phase-ins are emitted one card at a time in a loop (`src/GameObserver.cpp:1921-1946`), with no simultaneous second-one-shot batch. |
| 610.5 | UNIMPLEMENTED | Searches for static effects that grant an ability to a spell as it is put on the stack found no general next-spell/as-cast grant mechanism. Ordinary ability grants operate on card instances after an effect is installed. |

## 611

| rule | verdict | evidence / note |
|---|---|---|
| 611.1 | PARTIAL | Control (`include/AllAbilities.h:5514-5535`), characteristics (`src/AllAbilities.cpp:8665-8872`), and rule/player flags are supported piecemeal. Fixed and indefinite durations depend on wrapper subclasses rather than a general duration field. |
| 611.2 | IMPLEMENTED | Resolving payloads can create persistent child abilities: `GenericInstantAbility` installs its child and itself (`include/AllAbilities.h:3118-3126`). |
| 611.2a | PARTIAL | Until-end-of-turn children use `InstantAbility::testDestroy()` at `AFTER_EOT` (`src/MTGAbility.cpp:8259-8267`); indefinite children remain while source/target tests pass. Arbitrary stated durations are bespoke and not complete. |
| 611.2b | PARTIAL | `AAsLongAs` dynamically adds/removes a child according to its condition (`include/AllAbilities.h:3528-3609`), but it does not model the special “duration never starts / ended before first application” rule for resolving effects. |
| 611.2c | PARTIAL | A targeted resolving effect normally retains its target pointer (`include/AllAbilities.h:3112-3125`), approximating a locked affected set, while `ALord` rescans for static effects (`include/AllAbilities.h:3681-3743`). Multi-part effects have no independent set-determination model. |
| 611.2d | PARTIAL | Many resolving abilities evaluate `WParsedInt` once locally (for example turn modification at `src/AllAbilities.cpp:2462-2475`), but dynamic P/T observers can rebuild parsed expressions every update (`include/AllAbilities.h:2940-2971`), so the distinction is script/class dependent rather than guaranteed. |
| 611.2e | INCORRECT | Continuous changes are generally installed/mutated after zone entry. The code contains explicit timing workarounds such as `skipDamageTestOnce` because an ability must update before toughness is checked (`src/AllAbilities.cpp:3868-3900`), not a simultaneous enters-with characteristic pass. |
| 611.2f | UNIMPLEMENTED | No delayed continuous-effect object waits for the next appropriate spell to be put on the stack and then locks onto that spell; searches for next-spell characteristic modifiers found none. |
| 611.3 | IMPLEMENTED | Static-style abilities are installed as observers and can create persistent modifications/lord children (`include/AllAbilities.h:3650-3743`). |
| 611.3a | PARTIAL | Lords rescan all chooser-relevant zones and update membership (`src/MTGAbility.cpp:8288-8356`), so their affected set is dynamic. Recalculation occurs on update ticks rather than as an instantaneous continuous-effect evaluation. |
| 611.3b | PARTIAL | Base `MTGAbility::testDestroy()` keeps an ability while its source is in the appropriate flagged zone or battlefield and removes it otherwise (`src/MTGAbility.cpp:7528-7553`). Zone appropriateness is implemented through a few source flags, not arbitrary static-ability zones. |
| 611.3c | INCORRECT | `ALord` grants/removes effects only when its `Update()` rescans (`src/MTGAbility.cpp:8288-8340`, `8412-8415`). Therefore changes are not guaranteed simultaneous with entry or available before ETB-trigger determination. |
| 611.3d | PARTIAL | Permission flags such as play-from-library are consulted from cards and controller battlefields (`src/MTGCardInstance.cpp:1076-1105`), but the exception preserving battlefield-only abilities on the permitted spell/card for the stated duration has no general implementation. |

## 612

| rule | verdict | evidence / note |
|---|---|---|
| 612.1 | UNIMPLEMENTED | `CardPrimitive` stores parsed characteristics and magic text, but active effects do not rewrite an object’s rules text/type-line text as text. Missing-card lists contain the classic text-changing cards rather than implementations. |
| 612.2 | UNIMPLEMENTED | With no text-change parser, there is no role-sensitive replacement of color words, land types, or creature types. |
| 612.2a | UNIMPLEMENTED | Token creation parses fixed token characteristics; no text-changing effect can rewrite a creature-type word in a token-creating spell/ability. |
| 612.3 | UNIMPLEMENTED | Ability grants are child abilities/basic-ability bits (`include/AllAbilities.h:2473-2501`, `3650-3743`), but there is no separate text-change domain with which to enforce this interaction. |
| 612.4 | UNIMPLEMENTED | Tokens have mutable parsed characteristics, but no text-changing effect can change their defining subtype/rules text. |
| 612.5 | UNIMPLEMENTED | Search for `Exchange of Words`, text-box exchange, or equivalent machinery found none. |
| 612.6 | UNIMPLEMENTED | Search for Volrath’s Shapeshifter/full-text semantics found no implementation. Copy routines are card-copy operations, not the special full-text text change. |
| 612.7 | UNIMPLEMENTED | Spy Kit is listed under missing cards (`bin/Res/missing_cards_by_sets/CN2.txt:134-135`); an object cannot hold the Oracle-wide set of nonlegendary creature names. |
| 612.8 | PARTIAL | Objects can have their stored `name` changed (for example transformers and face changes), and `CardPrimitive::setName` exists (`src/CardPrimitive.cpp:401`). There is no general continuous “set name” effect with CR 613 timing/reversion. |
| 612.9 | N/A | Name stickers belong to excluded casual sticker play under the audit frame. No sticker system was considered required for classic two-player constructed/limited. |
| 612.10 | UNIMPLEMENTED | `_SPLICEARCANE_` appears only as lexicon/test data; searches of `src/` and `include/` found no splice engine that adds the spliced card’s rules text to the spell. |

## 613

| rule | verdict | evidence / note |
|---|---|---|
| 613.1 | INCORRECT | `ATransformer` directly saves and mutates colors/types/abilities/base P/T in one `addToGame()` call (`src/AllAbilities.cpp:8695-8872`) rather than applying each part in layers to printed/copiable starting values. |
| 613.1a | PARTIAL | Copy routines exist (`src/AllAbilities.cpp:2647-2681`), but copy effects are immediate object mutation and are not collected/applied as layer 1. |
| 613.1b | INCORRECT | Control changes occur immediately in constructors/resolution (`include/AllAbilities.h:5518-5534`, `7437-7461`), not in a continually recomputed layer 2 with timestamp/dependency ordering. |
| 613.1c | UNIMPLEMENTED | There is no text-changing effect machinery (see 612), hence no layer 3. |
| 613.1d | INCORRECT | Type changes directly call `addType`/`removeType` in effect execution order (`src/AllAbilities.cpp:8716-8771`), not layer 4 order. |
| 613.1e | INCORRECT | Color changes directly clear/remove/set colors in the same transformer execution (`src/AllAbilities.cpp:8695-8715`, `8773-8776`), not layer 5 order. |
| 613.1f | INCORRECT | Ability additions mutate `basicAbilities` bits (`src/AllAbilities.cpp:8778-8781`), while ability removal traverses and removes live `ActionLayer` objects (`9205-9274`). The source even labels the lord exception a hack and notes P/T layers are not handled (`9248-9256`). |
| 613.1g | PARTIAL | Wagic has special base, bonus/counter, CDA, and switch helpers (`src/MTGCardInstance.cpp:902-1000`) that approximate layer-7 categories, but there is no general layer-7 effect ordering. |
| 613.2 | UNIMPLEMENTED | No effect carries a layer-1 sublayer or timestamp, and no dependency ordering pass exists; repository searches for layer/timestamp/dependency in effects machinery found none. |
| 613.2a | PARTIAL | `AACopier` copies an object (`src/AllAbilities.cpp:2647-2681`), but merging and as-enters/face-up copiable-effect ordering are not represented in a layer-1a pass. |
| 613.2b | PARTIAL | Morph directly overwrites a copied card’s colors/types/abilities/name/P/T (`include/AllAbilities.h:7657-7669`), approximating face-down values but not applying them as layer 1b after copy effects. |
| 613.2c | UNIMPLEMENTED | The engine exposes no post-layer-1 copiable-values snapshot; copying reads mutable card/model fields through bespoke copy routines. |
| 613.3 | INCORRECT | Layers 2–6 do not distinguish CDA-first, timestamp, or dependency order. Effects execute as observers are inserted/updated; ability loss explicitly iterates `ActionLayer::mObjects` in reverse vector order (`src/AllAbilities.cpp:9223-9269`). |
| 613.4 | PARTIAL | The P/T representation separates CDA/base values, aggregate bonuses/counters, and switching (`src/MTGCardInstance.cpp:902-1000`), but does not order individual effects by timestamp/dependency within sublayers. |
| 613.4a | PARTIAL | `cdaactive` calls `cdaPT` and marks `isCDA` (`include/AllAbilities.h:3029-3041`; `src/MTGCardInstance.cpp:970-977`). Multiple CDAs and dependency/timestamp interactions are not evaluated. |
| 613.4b | PARTIAL | Set-base effects use `addbaseP/addbaseT`, which preserve the aggregate bonus (`src/MTGCardInstance.cpp:936-950`; `src/AllAbilities.cpp:8835-8863`). Multiple simultaneous setters are mutable state with ad hoc reversion, not timestamp-ordered layer 7b. |
| 613.4c | PARTIAL | Counters and ordinary modifiers share `pbonus/tbonus` arithmetic (`src/MTGCardInstance.cpp:902-934`; `src/Counters.cpp:56-84`), placing them after the stored base in common cases. Individual ordering/dependencies are absent. |
| 613.4d | PARTIAL | Counter/modifier/base changes explicitly undo and reapply `switchPT` (`src/Counters.cpp:56-84`; `include/AllAbilities.h:2942-2974`), approximating “switch last.” Multiple switch effects are a single boolean toggle (`include/AllAbilities.h:5334-5360`), not timestamped effects. |
| 613.5 | INCORRECT | Lord/static recalculation happens during discrete `ActionLayer::Update()` calls (`src/ActionLayer.cpp:172-181`; `src/MTGAbility.cpp:8412-8415`), so results are neither an automatic atomic layer pass nor necessarily instantaneous around events. |
| 613.6 | INCORRECT | A multi-part transformer applies types, colors, abilities, and P/T sequentially in one method (`src/AllAbilities.cpp:8695-8872`). If ability removal occurs during these mutations, there is no frozen cross-layer affected set ensuring later parts continue. |
| 613.7 | UNIMPLEMENTED | Active effects have action IDs/vector positions but no CR timestamp property or comparison. `ActionLayer` simply updates in vector order (`src/ActionLayer.cpp:172-181`). |
| 613.7a | UNIMPLEMENTED | No static-effect timestamp follows the source object or later ability-grant timestamp. Lord child abilities are created when a scan discovers a card (`include/AllAbilities.h:3702-3743`). |
| 613.7b | UNIMPLEMENTED | A resolving effect is appended as an observer (`src/MTGAbility.cpp:7516-7519`) but no semantic timestamp is recorded or used to order continuous effects. |
| 613.7c | UNIMPLEMENTED | `Counter` stores name, P/T, and count only (`src/Counters.cpp:17-24`); same-kind counters coalesce by incrementing `nb` (`101-128`) with no timestamps. |
| 613.7d | UNIMPLEMENTED | Zone moves create/copy card instances, but no zone-entry timestamp is stored or consulted by effects. |
| 613.7e | UNIMPLEMENTED | Attachment changes have no effect timestamp refresh used for continuous-effect ordering. |
| 613.7f | UNIMPLEMENTED | Face-up/down changes mutate a card; no timestamp refresh is stored or used. |
| 613.7g | UNIMPLEMENTED | Transform/convert paths mutate/copy faces; no timestamp refresh is stored or used for ordering. |
| 613.7h | N/A | Plane, phenomenon, and scheme cards are excluded game types under the audit frame. |
| 613.7i | N/A | Vanguard is excluded under the audit frame. |
| 613.7j | N/A | Conspiracy draft cards are excluded under the audit frame. |
| 613.7k | N/A | Stickers are excluded casual mechanics under the audit frame. |
| 613.7m | UNIMPLEMENTED | Simultaneous entrants/attachments receive no APNAP relative timestamp choices; zone and observer loops process objects sequentially. |
| 613.7n | UNIMPLEMENTED | With no timestamps, the special relative ordering between an entering object’s static effect and a simultaneous resolving effect cannot be represented. |
| 613.8 | UNIMPLEMENTED | Searches of the effect machinery found no dependency graph or dependency-order algorithm. |
| 613.8a | UNIMPLEMENTED | No code tests whether one same-layer effect changes another’s text, existence, affected set, or result. Lord target choosers merely rescan current mutated state (`src/MTGAbility.cpp:8288-8340`). |
| 613.8b | UNIMPLEMENTED | There is no wait-until-dependencies-applied step, dependent-effect timestamp order, or dependency-loop fallback. |
| 613.8c | UNIMPLEMENTED | Remaining effect order is never reevaluated after each applied effect; update loops advance through the existing action vector (`src/ActionLayer.cpp:172-181`). |
| 613.9 | PARTIAL | Direct mutation means a later effect can overwrite an earlier field, and lord eligibility can change after property mutation (`src/MTGAbility.cpp:8290-8307`). Correct override results are incidental without layers, timestamps, and dependency. |
| 613.10 | INCORRECT | Player effects are flags/live abilities applied when their class runs; they are not collected after object characteristics or ordered by timestamp/dependency. `ListMaintainerAbility` even processes player membership in the same scan (`src/MTGAbility.cpp:8343-8355`). |
| 613.11 | INCORRECT | Rule effects and cost changes are stored/mutated by separate bespoke classes (for example `AAlterCost`, `src/AllAbilities.cpp:8517-8573`) without a post-characteristics timestamp/dependency pass. Cost ordering cannot inherit a nonexistent CR 613 order. |

## 614

| rule | verdict | evidence / note |
|---|---|---|
| 614.1 | PARTIAL | `ReplacementEffects::replace` watches an event and lets registered modifiers return a changed event or `NULL` (`src/ReplacementEffects.cpp:164-175`). Its event/subclass coverage is narrow. |
| 614.1a | PARTIAL | A few scripted “instead” results map to bespoke replacement classes or keyword flags, but the parser does not recognize “instead” as a general replacement-effect construction. |
| 614.1b | PARTIAL | Turn skipping is represented by `Player::skippingTurn` and consumed by `nextPlayer()` (`src/GameObserver.cpp:161-174`), but skip steps/phases/events and replacement interaction are not generalized. |
| 614.1c | PARTIAL | Enters-with counters and copy/choice mechanics have card-specific setup paths, but they are not `ReplacementEffect` events and cannot participate in CR 616 ordering. |
| 614.1d | PARTIAL | Some global ETB modifications are expressible through lords/scripts, but lord children appear on scans after entry (`src/MTGAbility.cpp:8288-8340`), not as an enters replacement. |
| 614.1e | UNIMPLEMENTED | Search found no as-turned-face-up replacement-effect pipeline. Face changes are direct mutation. |
| 614.2 | PARTIAL | `REDamagePrevention` is an event replacement filtered by a source chooser (`src/ReplacementEffects.cpp:14-44`), but damage replacement/redirection generally is incomplete. |
| 614.3 | PARTIAL | Activated prevention can install a modifier, and persistent/static modifiers are removed on ability destruction (`include/AllAbilities.h:3356-3392`; `src/AllAbilities.cpp:9388-9392`). Duration/use-up behavior is class-specific. |
| 614.4 | PARTIAL | Damage calls the existing registry before dealing damage (`src/Damage.cpp:40-61`), so registered effects cannot retroactively affect a completed event. Other replacements bypass this event pipeline. |
| 614.5 | INCORRECT | When a modifier returns a new event, `ReplacementEffects::replace` recursively restarts at the first modifier with no “already applied to this event” set (`src/ReplacementEffects.cpp:164-174`). A replacement can therefore get another opportunity on the modified event, contrary to 614.5. |
| 614.6 | PARTIAL | A fully prevented/replaced event can be deleted and return `NULL` (`src/ReplacementEffects.cpp:24-42`), and draw replacement suppresses the original draw event (`89-154`). Modified-event triggering is not consistently modeled across direct special cases. |
| 614.7 | IMPLEMENTED | A nonmatching modifier returns the original event without consuming its state (`src/ReplacementEffects.cpp:16-23`, `44`; `89-95`). |
| 614.7a | INCORRECT | The registry is invoked before the zero-damage check (`src/Damage.cpp:45-58`). A one-shot `REDamagePrevention` with unlimited amount sets its `damage` to zero even when the incoming event already has 0 damage (`src/ReplacementEffects.cpp:24-29`), consuming a shield when CR says there is no event to replace. |
| 614.8 | PARTIAL | Regeneration shields increment `regenerateTokens`, then destruction consumes one, taps, restores damage/life, and removes the creature from combat (`src/MTGCardInstance.cpp:735-768`). It is outside the replacement registry and competing regeneration/totem effects use a bespoke menu (`485-535`), so general replacement ordering is absent. |
| 614.9 | UNIMPLEMENTED | No damage-redirection `ReplacementEffect` exists. `REDamagePrevention` only changes amount; old planeswalker targeting logic creates damage at a chosen target before the damage event rather than redirecting an event (`src/AllAbilities.cpp:2066-2100`). |
| 614.10 | PARTIAL | Whole-turn skip counters are created and consumed before switching players (`src/AllAbilities.cpp:2456-2475`; `src/GameObserver.cpp:161-174`). Started-step timing and arbitrary event/step/phase skips are absent. |
| 614.10a | PARTIAL | Multiple whole-turn skip effects accumulate numerically (`src/AllAbilities.cpp:2472-2475`), but scheduled “next” events and step/phase skips have no waiting model. |
| 614.10b | UNIMPLEMENTED | No skip-then-action object schedules the action as the first event of the next actually occurring step/phase/turn. |
| 614.11 | IMPLEMENTED | `AADrawer` creates and replaces `WEventDraw` before adding draws, regardless of whether a physical card is available at that point (`src/AllAbilities.cpp:4954-4967`). |
| 614.11a | PARTIAL | `REDrawReplacement` resolves a replacement once per requested card before suppressing the draw event (`src/ReplacementEffects.cpp:135-154`). Multiple-choice replacements create asynchronous menus inside that loop, so completion ordering is not robust. |
| 614.11b | UNIMPLEMENTED | Draw-plus-additional-action linkage is not represented in `WEventDraw`/`REDrawReplacement`; the replacer suppresses only the draw event, while chained payload behavior is script-specific. |
| 614.12 | INCORRECT | There is no centralized prospective-ETB replacement pass. Zone entry/copy, counters, lords, and transformers mutate in separate sequential paths, so applicable effects are not recalculated from the permanent as it would exist on the battlefield. |
| 614.12a | PARTIAL | Some clone/mode/target menus make choices before their direct zone-entry operation, but no rule ensures every ETB replacement choice precedes entry. |
| 614.12b | UNIMPLEMENTED | No simultaneous-permanents ETB replacement-choice/cost solver exists. |
| 614.12c | UNIMPLEMENTED | Search for anchor-word ETB ability choice/linkage found no general implementation. |
| 614.13 | UNIMPLEMENTED | ETB modifications are not modeled as replacement events capable of containing additional zone changes. |
| 614.13a | UNIMPLEMENTED | Consequently there is no candidate filtering that excludes the entering object(s) from auxiliary zone-change choices. |
| 614.13b | UNIMPLEMENTED | There is no per-ETB-replacement chosen-object set preventing the same object from changing zones twice. |
| 614.13c | UNIMPLEMENTED | There is no nested ETB/mill/top-exile replacement event model that can reserve cards simultaneously entering from the library. |
| 614.14 | PARTIAL | Imprint stores cards on the source and later scripts can reference its imprinted collection, but the link is not tied to cards exiled as the direct result of a registered replacement event; gained-pair linkage is not general. |
| 614.15 | UNIMPLEMENTED | `ReplacementEffects::replace` has no self-replacement type or priority category; every modifier is traversed in list insertion order (`src/ReplacementEffects.cpp:164-174`). |
| 614.16 | PARTIAL | Counter creation is routed through `WEventCounters` and the registry (`src/Counters.cpp:101-139`), allowing a supported counter prevention to see ordinary additions. Token creation has no analogous generic replacement event, and replacement-created counters/tokens are not comprehensively nested. |
| 614.17 | PARTIAL | Many “can’t” clauses are direct basic-ability flags checked by the relevant action (for example `CANTREGEN`, `src/MTGCardInstance.cpp:735-747`), not replacement effects. Coverage and precedence are bespoke. |
| 614.17a | PARTIAL | A live flag is consulted before the prohibited action in supported cases, but no common event gate guarantees this for every “can’t” effect. |
| 614.17b | PARTIAL | Some extra-cost `canPay()` paths test legality before payment, but there is no general connection from every “can’t happen” effect to cost-choice prohibition. |
| 614.17c | UNIMPLEMENTED | No self-replacement category or “event cannot happen” state exists in the registry, so the required exclusivity against other replacement/prevention effects cannot be enforced. |
| 614.17d | INCORRECT | With no prospective ETB/layer evaluation, “can’t enter” checks cannot inspect the fully modified would-be battlefield characteristics in the required iterative manner (see `ATransformer`’s post-entry mutations, `src/AllAbilities.cpp:8665-8872`). |

## 615

| rule | verdict | evidence / note |
|---|---|---|
| 615.1 | PARTIAL | Registered prevention watches damage before it is dealt, but other prevention is applied later inside `Damage::resolve`; there is no complete, uniform continuous prevention-effect model. |
| 615.1a | PARTIAL | Scripted prevent effects can become `AADamagePrevent` or `APreventDamageTypes`, but recognition is parser/class-specific rather than semantic handling of every “prevent” instruction. |
| 615.2 | PARTIAL | Source-filtered prevention uses a `TargetChooser` at event time (`src/ReplacementEffects.cpp:18-23`), subject to the source limitations noted for 609.7. |
| 615.3 | PARTIAL | Numeric shields persist until consumed/cleanup (`src/Damage.cpp:69-78`; `src/MTGCardInstance.cpp:772-787`), and registered shields have one-shot/static lifetime fields. Durations are not generalized. |
| 615.4 | PARTIAL | Registered and numeric shields must be present before `Damage::resolve()` reaches them (`src/Damage.cpp:45-73`), but the split pipelines prevent a single ordering guarantee. |
| 615.5 | UNIMPLEMENTED | No prevention-result event reports the amount prevented, and `REDamagePrevention` has no stored follow-up ability (`include/ReplacementEffects.h:24-38`). Additional “if damage was prevented” work cannot be generally sequenced immediately afterward. |
| 615.6 | PARTIAL | Fully prevented damage returns before damage triggers (`src/ReplacementEffects.cpp:24-42`; `src/Damage.cpp:139-175`). Bespoke conversions (Vigor, Phantom, Hydra, etc.) occur outside the replacement registry, so modified-event semantics are inconsistent. |
| 615.7 | INCORRECT | Numeric shields subtract the minimum and retain the remainder (`src/Damage.cpp:68-78`), but simultaneous damage is represented/resolved as separate stack actions and there is no controller choice allocating a limited shield among simultaneous sources. |
| 615.8 | PARTIAL | An unlimited, one-shot `REDamagePrevention` prevents one matching instance then sets its internal availability to zero (`src/ReplacementEffects.cpp:24-29`). It incorrectly consumes on a 0-damage event (614.7a) and lacks complete source identity. |
| 615.9 | PARTIAL | Chooser predicates are rechecked when damage occurs, and a mismatch neither prevents nor consumes (`src/ReplacementEffects.cpp:16-23`, `44`). The legal source/property space remains incomplete. |
| 615.10 | PARTIAL | A non-one-shot `REDamagePrevention` can reduce each matching event, and static `APreventDamageTypes` installs a persistent modifier (`src/AllAbilities.cpp:9358-9385`). The public static path generally prevents all damage, while fixed-per-event variants are not uniformly modeled. |
| 615.11 | PARTIAL | Lords can create a separate child/shield for each currently matching card (`include/AllAbilities.h:3681-3743`), approximating per-creature shields. Because membership is update-tick driven and shield construction is class-specific, resolution-time snapshot semantics are not assured. |
| 615.12 | UNIMPLEMENTED | Searches for unpreventable/can’t-be-prevented damage found no engine flag or damage-event property. Prevention therefore cannot be applied for additional effects while leaving shield capacity intact. |
| 615.12a | UNIMPLEMENTED | Without an unpreventable-damage state, the “apply each prevention once” rule cannot be represented. |
| 615.13 | UNIMPLEMENTED | There is no event for damage prevented or prevention effect applied; fully prevented `WEventDamage` is deleted before observer dispatch (`src/ReplacementEffects.cpp:24-42`). |

## 616

| rule | verdict | evidence / note |
|---|---|---|
| 616.1 | INCORRECT | `ReplacementEffects::replace` walks `modifiers` and applies the first matching transformations automatically (`src/ReplacementEffects.cpp:164-175`). Only draw replacement has a special menu, and it does not generalize to object/player events or APNAP. |
| 616.1a | UNIMPLEMENTED | There is no self-replacement class/category, so self-replacements cannot be forced first. |
| 616.1b | UNIMPLEMENTED | There are no registered ETB control-changing replacement effects or priority category for them. |
| 616.1c | UNIMPLEMENTED | There are no registered as-enters copy replacement effects or priority category for them. |
| 616.1d | UNIMPLEMENTED | There are no registered back-face-up ETB replacements or priority category for them. |
| 616.1e | INCORRECT | The affected player cannot choose among arbitrary applicable effects; registry insertion order decides. `REDrawReplacement`’s narrow “Choose Draw Replacement” menu (`src/ReplacementEffects.cpp:111-149`) is the only exception found. |
| 616.1f | PARTIAL | The registry continues through later modifiers on the same event and recursively restarts for a newly returned event (`src/ReplacementEffects.cpp:168-175`). It neither asks for each choice nor excludes already-applied effects, causing the 614.5 repeat defect. |
| 616.1g | UNIMPLEMENTED | Events do not expose a containment tree and the registry has no rule delaying replacement of a contained event until its containing replacement is chosen. |
| 616.2 | PARTIAL | Returning a different event recursively invokes the registry, which can make another modifier applicable (`src/ReplacementEffects.cpp:171-174`). Because the scan restarts without applied-effect tracking or player choice, this only approximates the rule and can repeat an effect illegally. |

## Verdict counts

| verdict | count |
|---|---:|
| IMPLEMENTED | 8 |
| PARTIAL | 60 |
| INCORRECT | 22 |
| UNIMPLEMENTED | 56 |
| N/A | 5 |
| **TOTAL** | **151** |

## HIGHLIGHTS

1. **CR 613 is not implemented as a layer system.** Characteristics are mutated
   in-place by observer callbacks. There are no layer tags, effect timestamps,
   dependency ordering, or an atomic recomputation pass. This makes the
   overarching 613/613.1 verdict **INCORRECT**, not merely incomplete
   (`src/ActionLayer.cpp:146-181`; `src/AllAbilities.cpp:8695-8872`).
2. **Layer-6/lord interactions are explicitly ad hoc.** `ALoseAbilities`
   traverses live actions in reverse vector order and exempts lord-granted
   abilities via a documented “hack”; the source itself notes that P/T layers
   are not handled (`src/AllAbilities.cpp:9223-9269`). Type-changing effects
   and lords therefore cannot obtain dependency-correct Humility/Opalescence-
   style results.
3. **Layer 7 is approximated, not absent—but still nonconforming.** Stored CDA
   values, set-base values, aggregate counters/bonuses, and a P/T-switch flag
   deliberately approximate 7a/7b/7c/7d (`src/MTGCardInstance.cpp:902-1000`).
   No individual timestamps/dependencies exist, multiple switch effects
   collapse into one boolean toggle, and reversion is mutation-based.
4. **Continuous static effects are frame-late.** Lords discover entrants and
   grant/remove children on `ActionLayer::Update`, not simultaneously with
   entry (`src/MTGAbility.cpp:8288-8340`, `8412-8415`). This can affect ETB
   trigger qualification and state checks (611.3c/613.5).
5. **Replacement/prevention ordering violates CR 616.** The general registry
   applies effects in insertion order with no affected-player choice, APNAP,
   or mandatory priority categories (`src/ReplacementEffects.cpp:164-180`).
   The draw-only menu does not repair the general case.
6. **A replacement can apply twice to one event.** A returned event recursively
   restarts the modifier list without tracking effects already used
   (`src/ReplacementEffects.cpp:171-174`), directly contradicting 614.5.
7. **Zero damage can consume a one-shot shield.** Replacement processing runs
   before the zero-damage test, and unlimited one-shot prevention marks itself
   spent even for a 0-damage event (`src/Damage.cpp:45-58`;
   `src/ReplacementEffects.cpp:24-29`), contradicting 614.7a.
8. **“Phase out until” is materially wrong.** There is no distinct until-event
   phase-out state; every `isPhased` permanent phases in at its controller’s
   next untap (`src/GameObserver.cpp:1921-1946`), contrary to 610.4a.
9. **Text-changing effects are essentially unsupported.** There is no rules-
   text transformation domain or layer 3. Classic color-word/land-word cards
   and Spy Kit appear in missing-card data, and splice has no engine
   implementation.
10. **Prevention has two incompatible pipelines.** Registered prevention runs
    before damage, while numeric shields and card-specific prevention/conversion
    run later inside `Damage::resolve` (`src/Damage.cpp:45-137`). There is no
    prevented-amount event, “damage can’t be prevented” support, or controller
    allocation among simultaneous sources.
