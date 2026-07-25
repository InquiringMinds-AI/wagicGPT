# Rules-conformance audit — card types and zones (CR 3xx–4xx)

Scope and verdicts follow `00-frame.md`. The rules text audited was
`data/sec-3-4-types-zones.txt`; “search found no machinery” notes refer to
good-faith searches of `src/` and `include/`. Variant-only card types and ante
are N/A for the supported two-player classic game types.

## 3xx — Card Types

| rule | verdict | evidence / note |
|---|---|---|
| 300.1 | PARTIAL | The type registry covers the ordinary types, Kindred, Dungeon, and Battle, but has no Phenomenon type (`src/Subtypes.cpp:9-30`, `include/Subtypes.h:16-38`). Phenomenon itself is variant-only, but the general enumeration is incomplete. |
| 300.2 | IMPLEMENTED | Cards store a vector of types and all type queries independently match that vector (`src/MTGCardInstance.cpp:154`, `src/Subtypes.cpp:89-108`). |
| 300.2a | IMPLEMENTED | The normal-play rule branches on `isLand()` before the spell-casting branch, so a land with other types is played as a land (`src/MTGRules.cpp:303-344`). |
| 300.2b | IMPLEMENTED | Kindred is a registered card type without a distinct resolution path, so the accompanying type controls casting/resolution (`src/Subtypes.cpp:22`, `src/MTGRules.cpp:303-405`). |
| 301.1 | IMPLEMENTED | Noninstant artifact cards use the priority/main-phase/empty-stack gate and are moved to the stack (`src/MTGRules.cpp:303-405`, `src/MTGRules.cpp:620-650`). |
| 301.2 | IMPLEMENTED | Permanent-spell resolution puts non-instant/non-sorcery cards onto the battlefield (`src/ActionStack.cpp:350-375`, `src/ActionStack.cpp:470-512`). |
| 301.3 | IMPLEMENTED | Artifact and Equipment are registered independently and cards retain multiple type/subtype ids (`src/Subtypes.cpp:14-20`, `src/MTGCardInstance.cpp:402-436`). |
| 301.4 | IMPLEMENTED | Artifact status and color/mana cost are independent stored characteristics (`include/MTGCardInstance.h:47-55`, `src/MTGCardInstance.cpp:154-169`). |
| 301.5 | PARTIAL | Equipment attachment exists, but legality is a periodic, type-string-driven check rather than a general attachment legality model (`src/GameObserver.cpp:903-927`). |
| 301.5a | IMPLEMENTED | Equipment records its attached object in `target`, and attach/unattach update the equipped creature’s equipment count (`src/AllAbilities.cpp:10735-10770`). |
| 301.5b | PARTIAL | Equipment enters unattached and equip uses a target chooser, but the legality enforcement is the limited check in `GameObserver` (`src/MTGAbility.cpp:4004-4034`, `src/GameObserver.cpp:903-927`). |
| 301.5c | PARTIAL | The engine detaches creature Equipment and noncreature targets, but search for `reconfigure` and multi-attachment choice found no general implementation (`src/GameObserver.cpp:903-920`). |
| 301.5d | IMPLEMENTED | Attachment uses the Equipment object’s controller while the target remains independently controlled; equip action ownership is refreshed from the Equipment controller (`src/GameObserver.cpp:922-925`, `src/AllAbilities.cpp:10760-10770`). |
| 301.5e | PARTIAL | Put-on-battlefield attachment is scripted by individual mover/equip abilities; no general replacement handling for an illegal undefined attachment was found (searched `attach`, `equipment`, `putInZone`; relevant path `src/AllAbilities.cpp:6845-6979`). |
| 301.5f | IMPLEMENTED | Equipment effects consistently dereference the permanent’s current `target`, not a requirement that the source still have Equipment type (`src/AllAbilities.cpp:10735-10770`). |
| 301.6 | PARTIAL | Fortification is recognized by string and land legality is checked, but search for a full fortify/Fortification rules path found only the shared Equipment implementation (`src/GameObserver.cpp:903-920`). |
| 301.7 | PARTIAL | Crew costs and the Vehicle transformation idiom exist, but Vehicle has no dedicated type rules object (`src/ExtraCost.cpp:819-890`; search `Vehicle` in `src/` and `include/`). |
| 301.7a | PARTIAL | P/T is ignored by accessors when an object is not a creature, but the values remain on every card object (`src/MTGCardInstance.cpp:1005-1018`). |
| 301.7b | IMPLEMENTED | Becoming a creature exposes stored/base P/T and continuous modifiers through the normal P/T calculation (`src/MTGCardInstance.cpp:165-169`, `src/MTGCardInstance.cpp:938-967`). |
| 302.1 | IMPLEMENTED | Creature spells use the normal sorcery-speed cast gate and stack path (`src/MTGRules.cpp:303-405`, `src/MTGRules.cpp:620-650`). |
| 302.2 | IMPLEMENTED | Permanent-spell resolution puts creatures onto the battlefield (`src/ActionStack.cpp:470-512`). |
| 302.3 | IMPLEMENTED | Creature subtypes are dynamically registered and multiple type ids are retained (`src/Subtypes.cpp:48-67`, `src/MTGCardInstance.cpp:402-436`). |
| 302.4 | IMPLEMENTED | P/T accessors return meaningful current values only for creatures (`src/MTGCardInstance.cpp:1005-1018`). |
| 302.4a | IMPLEMENTED | Combat damage is derived from current power (subject to rules-changing abilities) (`src/MTGCardInstance.cpp:2075-2093`). |
| 302.4b | IMPLEMENTED | Creature state uses current toughness/life and the state pass destroys lethal creatures (`src/MTGCardInstance.cpp:165-169`, `src/GameObserver.cpp:825-844`). |
| 302.4c | PARTIAL | Printed/base P/T plus modifiers are represented, but this does not establish complete CR 613 layer ordering (`src/MTGCardInstance.cpp:165-169`, `src/MTGCardInstance.cpp:938-967`). |
| 302.5 | IMPLEMENTED | Creature attack/block state and blocker lists are first-class engine state (`src/MTGCardInstance.cpp:760-769`, `src/MTGRules.cpp:2160-2590`). |
| 302.6 | IMPLEMENTED | `summoningSickness` gates creatures, exempts haste/noncreatures, and clears for the controller at cleanup (`src/MTGCardInstance.cpp:772-820`). |
| 302.7 | PARTIAL | Lethal damage and cleanup resetting are implemented, but the engine models marked damage through mutable `life`, not a distinct damage-mark ledger (`src/MTGCardInstance.cpp:165-169`, `src/GameObserver.cpp:825-844`). |
| 303.1 | IMPLEMENTED | Enchantments use the normal sorcery-speed cast gate and stack path (`src/MTGRules.cpp:303-405`, `src/MTGRules.cpp:620-650`). |
| 303.2 | IMPLEMENTED | Resolving enchantment permanents enter the battlefield (`src/ActionStack.cpp:470-512`). |
| 303.3 | IMPLEMENTED | Enchantment/Aura are registered and cards retain multiple subtype ids (`src/Subtypes.cpp:10-20`, `src/MTGCardInstance.cpp:402-436`). |
| 303.4 | PARTIAL | Aura attachment and enchanted-object state exist, but legality is reduced to stored target strings and protections (`src/GameObserver.cpp:941-988`). |
| 303.4a | IMPLEMENTED | Aura spell parsing supplies a target chooser and the spell retains chosen targets (`src/MTGAbility.cpp:1100-1115`, `src/ActionStack.cpp:364-385`). |
| 303.4b | IMPLEMENTED | Aura `target`/`playerTarget`, `enchanted`, and `auraParent` model the enchanted object/player relationship (`include/MTGCardInstance.h:95-96`, `include/MTGCardInstance.h:152`). |
| 303.4c | PARTIAL | Missing/illegal permanent targets send Auras to graveyard, but player attachments and all continuous restrictions are not covered by the same check (`src/GameObserver.cpp:941-988`). |
| 303.4d | PARTIAL | The single `target` field prevents multiple attachments, but search found no explicit self-Aura or Aura-creature state-based rule (`src/GameObserver.cpp:941-988`; searched `Aura`, `self`, `isCreature`). |
| 303.4e | IMPLEMENTED | Aura and enchanted-object control are stored independently; effects follow the target object while the Aura retains its controller (`src/MTGCardInstance.cpp:823-833`, `src/GameObserver.cpp:941-950`). |
| 303.4f | PARTIAL | Individual put-into-play Aura code checks/chooses targets, but no general Aura replacement procedure was found (`src/AllAbilities.cpp:6837-6852`). |
| 303.4g | PARTIAL | A scripted Aura put checks for targets, but the engine has no general “remain in current zone / token not created” rule (`src/AllAbilities.cpp:6837-6852`; searched `Aura`, `legal`, `current zone`). |
| 303.4h | UNIMPLEMENTED | Search of `putInZone`, attachment, Aura, Equipment, and Fortification found no global rule that strips an attempted attachment from other permanent types. |
| 303.4i | PARTIAL | Some Aura mover paths decline the move without a valid target, but this is not enforced by the generic zone mover (`src/AllAbilities.cpp:6837-6852`, `src/MTGGameZones.cpp:467-831`). |
| 303.4j | PARTIAL | Rehook/equip code validates some targets, but no general attach-event replacement was found (`src/AllAbilities.cpp:5087-5145`). |
| 303.4k | UNIMPLEMENTED | Search for face-up Aura attachment legality (`turningOver`, `Aura`, `attach`) found morph handling but no rule 303.4k choice procedure. |
| 303.4m | IMPLEMENTED | Aura effects use the current attachment target even where the source’s current type is separately mutable (`src/GameObserver.cpp:941-988`, `src/AllAbilities.cpp:7388`). |
| 303.5 | PARTIAL | Saga cards can be represented/scripted as enchantments, but search found no general Saga rules object; behavior is card-script driven. |
| 303.6 | PARTIAL | Class is a dynamically representable subtype, but search found no general Class-level rules object; behavior is card-script driven (`src/Subtypes.cpp:33-45`). |
| 303.7 | PARTIAL | Role is dynamically representable as a subtype, but there is no dedicated Role attachment machinery (`src/Subtypes.cpp:33-45`). |
| 303.7a | UNIMPLEMENTED | Search for `Role`, timestamps, and same-controller Aura pruning found no Role state-based action. |
| 304.1 | IMPLEMENTED | Instants bypass the sorcery-speed check but still require the acting/priority cast path and use the stack (`src/MTGRules.cpp:303-405`, `src/MTGRules.cpp:620-650`). |
| 304.2 | IMPLEMENTED | Spell resolution executes its ability then sends instant/sorcery sources to the graveyard (`src/ActionStack.cpp:470-512`). |
| 304.3 | IMPLEMENTED | Instant subtype ids are dynamically registered and may be multiple (`src/Subtypes.cpp:33-67`, `src/MTGCardInstance.cpp:402-436`). |
| 304.4 | INCORRECT | Generic battlefield moves do not reject instant cards; `AAMover` sends any selected card to the battlefield and `putInZone` has no instant/sorcery destination guard (`src/AllAbilities.cpp:6925-6979`, `src/MTGGameZones.cpp:467-831`). |
| 304.5 | PARTIAL | Priority/sorcery-speed helper concepts exist, but “as an instant” is generally implemented by Flash/ASFLASH card flags rather than a rules-level timing query (`src/MTGRules.cpp:339-345`). |
| 305.1 | IMPLEMENTED | Lands require current-turn sorcery timing, move directly through a resolving special path, and do not become stack actions (`src/MTGRules.cpp:335-342`, `src/MTGRules.cpp:475-493`). |
| 305.2 | IMPLEMENTED | Battlefield land entries are limited by a max-per-turn play restriction, with effects able to change the maximum (`src/PlayRestrictions.cpp:20-38`, `src/MTGRules.cpp:335-338`). |
| 305.2a | IMPLEMENTED | The restriction compares the permitted maximum with matching land entries seen this turn (`src/PlayRestrictions.cpp:24-37`). |
| 305.2b | IMPLEMENTED | `canPutIntoZone` rejects a land once its maximum is reached, and normal play honors that rejection (`src/PlayRestrictions.cpp:24-37`, `src/MTGRules.cpp:335-338`). |
| 305.3 | IMPLEMENTED | `StackIsEmptyandSorcerySpeed` requires the card controller to be the current player (`src/MTGCardInstance.cpp:1109-1119`). |
| 305.4 | IMPLEMENTED | Effects can call `putInZone` directly; only the normal-play path consults/increments the land-play restriction (`src/AllAbilities.cpp:6925-6979`, `src/MTGRules.cpp:335-342`). |
| 305.5 | IMPLEMENTED | Land and its dynamically registered subtypes coexist in the type vector (`src/Subtypes.cpp:13`, `src/Subtypes.cpp:48-67`). |
| 305.6 | PARTIAL | Basic lands and mana producers are supported, but intrinsic mana is supplied through card primitives/abilities rather than generated solely from basic land type (`src/MTGCardInstance.cpp:1054-1073`; searched basic-land type assignment). |
| 305.7 | PARTIAL | Type-changing APIs can add/remove/reset types and abilities, but no general implementation of the exact “set basic land type” loss/gain recipe was found (`src/MTGCardInstance.cpp:402-436`, `src/MTGCardInstance.cpp:938-967`). |
| 305.8 | IMPLEMENTED | Basic is registered as a supertype independently from land subtype (`src/Subtypes.cpp:17`, `src/Subtypes.cpp:84-87`). |
| 305.9 | IMPLEMENTED | Same mechanism as 300.2a: any `isLand()` object takes the land-play branch, not the cast branch (`src/MTGRules.cpp:303-344`). |
| 306.1 | IMPLEMENTED | Planeswalker spells use the normal sorcery-speed stack path (`src/MTGRules.cpp:303-405`). |
| 306.2 | IMPLEMENTED | Resolving planeswalker permanents enter the battlefield (`src/ActionStack.cpp:470-512`). |
| 306.3 | IMPLEMENTED | Planeswalker is registered as a type and subtype names remain independent dynamic ids (`src/Subtypes.cpp:21`, `src/Subtypes.cpp:48-67`). |
| 306.4 | PARTIAL | Legendary-name state handling exists, but the AI still contains stale same-subtype planeswalker policy code (`src/MTGCardInstance.cpp:1021-1035`, `src/AIPlayerBaka.cpp:2788`). |
| 306.5 | IMPLEMENTED | Planeswalker loyalty is represented by named loyalty counters (`src/AllAbilities.cpp:4569-4571`). |
| 306.5a | PARTIAL | Printed loyalty is card data, but no separate off-battlefield loyalty accessor was found; the engine primarily reads counters/life on permanents. |
| 306.5b | PARTIAL | Card scripts add loyalty counters on entry; this is not a type-intrinsic replacement effect (searched `loyalty` counter creation; parser path `src/MTGAbility.cpp:7735-7744`). |
| 306.5c | IMPLEMENTED | Current planeswalker life/loyalty is synchronized from its loyalty counters (`src/AllAbilities.cpp:4569-4571`). |
| 306.5d | PARTIAL | Main-phase/current-player and once-per-turn loyalty checks exist, but exceptions are flag-driven and not a complete generic CR 606 model (`src/MTGAbility.cpp:7735-7744`). |
| 306.6 | IMPLEMENTED | Combat target selection explicitly permits attacking planeswalkers (`src/MTGRules.cpp:2169`, `src/MTGRules.cpp:2278`). |
| 306.7 | INCORRECT | The removed planeswalker-redirection rule is still implemented: player damage searches for an opposing planeswalker and redirects (`src/AllAbilities.cpp:2070-2090`). The rule requires modern Oracle damage to target planeswalkers directly. |
| 306.8 | IMPLEMENTED | Damage to a planeswalker removes loyalty counters (`src/MTGRules.cpp:4084-4107`). |
| 306.9 | IMPLEMENTED | The state pass moves a planeswalker without loyalty counters to its owner’s graveyard (`src/GameObserver.cpp:832-844`). |
| 307.1 | IMPLEMENTED | Sorcery cards use the current-turn main-phase/empty-stack cast gate and stack (`src/MTGRules.cpp:303-405`). |
| 307.2 | IMPLEMENTED | Resolution performs the spell action then moves sorcery/instant sources to graveyard (`src/ActionStack.cpp:470-512`). |
| 307.3 | IMPLEMENTED | Spell subtypes are dynamically registered and multiple ids are retained (`src/Subtypes.cpp:33-67`, `src/MTGCardInstance.cpp:402-436`). |
| 307.4 | INCORRECT | The same generic-zone-move defect as 304.4 permits sorcery cards to enter the battlefield (`src/AllAbilities.cpp:6925-6979`, `src/MTGGameZones.cpp:467-831`). |
| 307.5 | IMPLEMENTED | `StackIsEmptyandSorcerySpeed` checks empty stack, current player, and either main phase, without asking whether a sorcery card exists (`src/MTGCardInstance.cpp:1109-1119`). |
| 307.5a | PARTIAL | Cast timing is recorded, but search for an explicit “cast when sorcery could not” predicate found only phase/stack restriction strings, not the complete retrospective query. |
| 308.1 | IMPLEMENTED | Kindred has no separate cast/resolution branch; the accompanying card type determines the path (`src/Subtypes.cpp:22`, `src/MTGRules.cpp:303-405`). |
| 308.2 | IMPLEMENTED | Kindred and creature subtype ids coexist in the same dynamic type system (`src/Subtypes.cpp:22`, `src/Subtypes.cpp:48-67`). |
| 308.3 | PARTIAL | Kindred is registered, but search of card loading for a `Tribal` compatibility alias found no explicit alias/errata mapping. |
| 309.1 | IMPLEMENTED | Dungeon is a registered nonpermanent card type (`src/Subtypes.cpp:26`, `include/Subtypes.h:33`). |
| 309.2 | PARTIAL | Deck loading separates dungeon cards, but places them in the engine sideboard rather than maintaining the CR outside-game pool (`src/MTGGameZones.cpp:148-160`). |
| 309.2a | PARTIAL | Dungeon/command-zone machinery exists, but selection is card-script/deck driven rather than a generic venture choice procedure (`src/MTGGameZones.cpp:148-160`, `src/MTGAbility.cpp:4621-4632`). |
| 309.2b | PARTIAL | Command zone is represented, but dungeon completion is implemented by moving the dungeon to sideboard (known ledger note in `strategy-design/lexicon-triage/engine-bugs.md`; mechanism `src/AllAbilities.cpp:1717-1736`). |
| 309.2c | PARTIAL | Dungeon is a nonpermanent type and command-zone abilities are supported, but generic zone APIs do not prohibit a Dungeon leaving command (`src/Subtypes.cpp:89-108`, `src/MTGGameZones.cpp:467-831`). |
| 309.2d | UNIMPLEMENTED | Search for venture-only admission from outside the game found no generic guard; dungeon loading is unconditional deck initialization (`src/MTGGameZones.cpp:148-160`). |
| 309.3 | PARTIAL | Deck parsing limits dungeon selection, but the code supports a `DungeonZone` collection and does not establish the runtime one-owned-dungeon invariant (`src/MTGDeck.cpp:1214-1236`). |
| 309.4 | UNIMPLEMENTED | Search for venture markers, rooms, and arrows found no C++ room graph/marker model. |
| 309.4a | UNIMPLEMENTED | No venture-marker placement model was found (searched `venture`, `room`, `marker`, `dungeon`). |
| 309.4b | N/A | Room names being flavor text requires no engine behavior independent of the unimplemented room model. |
| 309.4c | PARTIAL | Dungeon cards can supply command-zone scripted abilities, but no intrinsic room trigger/ownership model exists (`src/MTGAbility.cpp:6573-6575`, `src/GameObserver.cpp:1296-1304`). |
| 309.5 | UNIMPLEMENTED | Search found completion counters/events but no `venture into the dungeon` marker-movement keyword action (`src/AllAbilities.cpp:1717-1736`). |
| 309.5a | UNIMPLEMENTED | No room graph, outgoing-arrow choice, or venture marker was found. |
| 309.5b | INCORRECT | The engine’s documented completion mechanism sideboards the completing card and increments a player counter rather than removing the completed dungeon, choosing a new dungeon, and entering its top room (`src/AllAbilities.cpp:1717-1736`; matching known ledger note). |
| 309.6 | UNIMPLEMENTED | No bottom-room/pending-room-trigger state-based action was found (searched `bottommost`, `room`, `venture`). |
| 309.7 | PARTIAL | Completion is represented by `dungeonCompleted` and emits an event, but it is not tied to the CR removal event (`src/AllAbilities.cpp:1732-1736`). |
| 310.1 | IMPLEMENTED | Battle spells use the normal sorcery-speed stack path (`src/MTGRules.cpp:303-405`). |
| 310.2 | IMPLEMENTED | Resolving Battle permanents enter the battlefield (`src/ActionStack.cpp:470-512`). |
| 310.3 | IMPLEMENTED | Battle is a registered type and subtype ids are dynamically supported (`src/Subtypes.cpp:30`, `src/Subtypes.cpp:48-67`). |
| 310.4 | PARTIAL | Defense is modeled by a named counter, but not as a distinct characteristic API (`src/AllAbilities.cpp:4572-4574`). |
| 310.4a | PARTIAL | Printed defense is card-script data; search found no off-battlefield defense accessor. |
| 310.4b | INCORRECT | Entry defense is card-scripted, not intrinsic, and the known ledger records real Battle `counter(0/0.6.defense)` lines parsing NULL, leaving the Battle at 0 defense (`strategy-design/lexicon-triage/engine-bugs.md`, item “Battle defense-counter line”; state consumer `src/GameObserver.cpp:846-856`). |
| 310.4c | IMPLEMENTED | A Battle’s current life/defense is synchronized from defense counters (`src/AllAbilities.cpp:4572-4574`, `src/GameObserver.cpp:855-856`). |
| 310.5 | IMPLEMENTED | Combat target selection explicitly permits attacking Battles (`src/MTGRules.cpp:2169`, `src/MTGRules.cpp:2278`). |
| 310.6 | IMPLEMENTED | Damage removes that many defense counters (`src/MTGRules.cpp:4108-4117`). |
| 310.7 | PARTIAL | Zero-defense cleanup exists, but it does not test the CR exception for a pending ability sourced by that Battle (`src/GameObserver.cpp:846-856`). |
| 310.8 | UNIMPLEMENTED | Search for `protector` in all C++ source/header files returned no field or rules machinery. |
| 310.8a | UNIMPLEMENTED | No Battle protector choice on entry exists (search `protector` returned no C++ matches). |
| 310.8b | INCORRECT | Attacking is keyed to the Battle’s controller/opponent battlefield, with no protector; this cannot enforce who may attack a Battle (`src/MTGRules.cpp:2160-2280`). |
| 310.8c | INCORRECT | Blocking is keyed to ordinary controller combat state and has no protector identity, so the protector-only blocking rule is not representable (`src/MTGRules.cpp:2470-2590`). |
| 310.8d | UNIMPLEMENTED | No protector means “defending player” cannot be rebound relative to a Battle (search `protector` returned no matches). |
| 310.8e | UNIMPLEMENTED | No representation of “player who protects a battle” exists. |
| 310.8f | UNIMPLEMENTED | No protector state exists to enforce uniqueness or replacement. |
| 310.8g | UNIMPLEMENTED | No protector state exists to persist through type/copy changes. |
| 310.9 | PARTIAL | Aura legality can recognize Battle targets, but there is no Battle-specific prohibition on all attachments (`src/GameObserver.cpp:952-978`). |
| 310.10 | UNIMPLEMENTED | No protector state or state-based protector repair/graveyard action exists. |
| 310.11 | PARTIAL | Siege can be represented as a dynamic subtype, but no dedicated Siege rules class exists (`src/Subtypes.cpp:33-67`). |
| 310.11a | UNIMPLEMENTED | No opponent protector choice exists (search `protector` returned no C++ matches). |
| 310.11b | PARTIAL | Individual Battle scripts can react to defense removal/transform, but the intrinsic Siege trigger is not type-generated; the defense parser defect also breaks real cards (`src/MTGRules.cpp:4129-4133`; known Battle ledger entry). |
| 311.1 | N/A | Plane cards are exclusive to the excluded Planechase variant. |
| 311.2 | N/A | Planechase command-zone behavior is outside supported game types. |
| 311.3 | N/A | Plane subtype syntax is exclusive to Planechase. |
| 311.4 | N/A | Plane command-zone abilities are exclusive to Planechase. |
| 311.5 | N/A | Planar controller is a Planechase concept. |
| 311.6 | N/A | Face-up plane handling is exclusive to Planechase. |
| 311.7 | N/A | Chaos abilities are exclusive to Planechase. |
| 312.1 | N/A | Phenomena are exclusive to the excluded Planechase variant. |
| 312.2 | N/A | Phenomenon command-zone behavior is Planechase-only. |
| 312.3 | N/A | Phenomenon subtype rules are Planechase-only. |
| 312.4 | N/A | Planar controller is Planechase-only. |
| 312.5 | N/A | Encountering phenomena is Planechase-only. |
| 312.6 | N/A | Face-up phenomenon handling is Planechase-only. |
| 312.7 | N/A | Phenomenon state-based planeswalking is Planechase-only. |
| 313.1 | N/A | Vanguard cards are exclusive to the excluded Vanguard variant. |
| 313.2 | N/A | Vanguard command-zone behavior is variant-only. |
| 313.3 | N/A | Vanguard subtype rules are variant-only. |
| 313.4 | N/A | Vanguard command-zone abilities are variant-only. |
| 313.5 | N/A | Vanguard ownership/control is variant-only. |
| 313.6 | N/A | Vanguard hand modifiers are variant-only. |
| 313.7 | N/A | Vanguard life modifiers are variant-only. |
| 314.1 | N/A | Schemes are exclusive to the excluded Archenemy variant. |
| 314.2 | N/A | Scheme command-zone behavior is Archenemy-only. |
| 314.3 | N/A | Scheme subtype rules are Archenemy-only. |
| 314.4 | N/A | Scheme ability behavior is Archenemy-only. |
| 314.5 | N/A | Scheme ownership/control is Archenemy-only. |
| 314.6 | N/A | “Set in motion” is Archenemy-only. |
| 314.7 | N/A | Ongoing schemes are Archenemy-only. |
| 315.1 | N/A | Conspiracies are exclusive to excluded Conspiracy Draft. |
| 315.2 | N/A | Conspiracy command-zone setup is variant-only. |
| 315.3 | N/A | Bringing conspiracies into the game is variant-only. |
| 315.4 | N/A | Conspiracy subtype rules are variant-only. |
| 315.5 | N/A | Conspiracy command-zone abilities are variant-only. |
| 315.5a | N/A | Conspiracy start-of-game effects are variant-only. |
| 315.5b | N/A | Face-down conspiracy characteristics are variant-only. |
| 315.6 | N/A | Conspiracy ownership/control is variant-only. |
| 315.7 | N/A | Looking at face-down conspiracies is variant-only. |

## 4xx — Zones

| rule | verdict | evidence / note |
|---|---|---|
| 400.1 | PARTIAL | Library, hand, battlefield, graveyard, stack, exile, command, sideboard, and internal zones are allocated per player; battlefield/stack/exile/command are nevertheless represented as per-player containers rather than truly shared zones (`src/MTGGameZones.cpp:380-400`). |
| 400.2 | PARTIAL | Library/hand GUIs hide opponent information and public zones are displayable, but face-down public-zone visibility is not generally modeled (zone initialization `src/MTGGameZones.cpp:380-400`; face-down search found only morph). |
| 400.3 | PARTIAL | Owner-zone helpers correctly send graveyard, library, and hand moves to `card->owner`, but generic destination selection can use controller-specific zones (`src/MTGGameZones.cpp:407-465`, `src/MTGGameZones.cpp:1640-1675`). |
| 400.4 | PARTIAL | Some card-type/zone constraints arise from cast and state code, but there is no central card-type destination validator (`src/MTGGameZones.cpp:467-831`). |
| 400.4a | INCORRECT | Instants and sorceries can be moved to the battlefield by generic `AAMover`; `putInZone` does not leave them in the prior zone (`src/AllAbilities.cpp:6925-6979`, `src/MTGGameZones.cpp:467-831`). |
| 400.4b | N/A | The listed command-zone-locked cards are exclusive to excluded casual variants. |
| 400.5 | PARTIAL | Ordered zones use vectors and stack/library operations preserve order, but graveyard/face-down-pile rearrangement permissions are not enforced as a rules concept (`include/MTGGameZones.h:109`, `src/MTGGameZones.cpp:918-975`). |
| 400.6 | PARTIAL | `putInZone` applies several hard-coded replacement cases before the move, but there is no general applicable-replacements discovery and affected-player choice for mutually exclusive effects (`src/MTGGameZones.cpp:467-555`). |
| 400.7 | INCORRECT | Zone moves clone a new object, but the engine copies substantial forbidden memory (X, kicker, stored cards, alternate-cost flags, commander damage, etc.) across ordinary moves (`src/MTGGameZones.cpp:918-969`). This exceeds the listed exceptions. |
| 400.7a | PARTIAL | The stack-to-battlefield copy chain preserves cast characteristics/types, but the generic copy also preserves unrelated state (`src/MTGGameZones.cpp:571-591`, `src/MTGGameZones.cpp:918-969`). |
| 400.7b | PARTIAL | Granted/static properties may be carried through cloned basic-ability state, but there is no explicit static-ability exception boundary (`src/MTGGameZones.cpp:938-960`). |
| 400.7c | UNIMPLEMENTED | Search for prevention effects surviving from a permanent spell to its resulting permanent found no explicit linkage. |
| 400.7d | PARTIAL | X, kicker, mana-used, and cast-method information is deliberately copied and available after resolution (`src/MTGGameZones.cpp:571-575`, `src/MTGGameZones.cpp:938-960`). |
| 400.7e | IMPLEMENTED | Zone-change events carry the new card object plus `from`/`to`, and the previous-object chain remains available to triggers (`src/MTGGameZones.cpp:795-798`, `src/MTGGameZones.cpp:918-969`). |
| 400.7f | PARTIAL | Aura cleanup can follow a vanished target and move the Aura, but simultaneous enchanted-permanent/Aura LKI is not modeled as this exact exception (`src/GameObserver.cpp:930-988`). |
| 400.7g | PARTIAL | Play-from-zone flags enable the move to stack and are copied, but the implementation is broad persistent basic-ability copying rather than the narrow granted-cast exception (`src/MTGRules.cpp:303-310`, `src/MTGGameZones.cpp:938-960`). |
| 400.7h | PARTIAL | Spell objects retain their source and cast metadata after a permitted cast, but no general “other parts of that effect find it” identity API was found (`src/ActionStack.cpp:350-390`). |
| 400.7i | PARTIAL | A land allowed from another zone is moved to battlefield and remains linked by `previous`/`next`, but no scoped effect-reference API was found (`src/MTGGameZones.cpp:918-969`). |
| 400.7j | PARTIAL | The `previous`/`next` chain lets resolving code find moved objects, but it is global and retains more memory than CR permits (`src/MTGGameZones.cpp:918-969`). |
| 400.7k | PARTIAL | Madness replacement to exile and discard state exist, but search found no explicit post-madness public-zone identity exception (`src/MTGGameZones.cpp:486-493`, `src/MTGGameZones.cpp:652-657`). |
| 400.7m | UNIMPLEMENTED | Search for `sticker` in `src/` and `include/` found no sticker state or public-zone retention machinery. |
| 400.8 | IMPLEMENTED | A same-zone exile move still removes/clones/adds the card, producing a new object (`src/MTGGameZones.cpp:467-555`, `src/MTGGameZones.cpp:918-969`). |
| 400.9 | N/A | Face-up command-zone cards turned face down belong to excluded variants. |
| 400.10 | IMPLEMENTED | Same-zone command moves take the generic clone/new-object path (`src/MTGGameZones.cpp:467-555`, `src/MTGGameZones.cpp:918-969`). |
| 400.11 | IMPLEMENTED | The engine keeps sideboard/outside cards outside all gameplay zones, with sideboard allocated separately (`src/MTGGameZones.cpp:133-160`, `src/MTGGameZones.cpp:380-400`). |
| 400.11a | N/A | Physical sideboard status is explicitly physical-play/format administration under the audit frame. |
| 400.11b | INCORRECT | Outside-game cards are represented in sideboard, but the known target matrix shows sideboard/command-zone target moves are inert (`strategy-design/lexicon-triage/engine-bugs.md`, “sideboard/command-zone targeting inert”; chooser zones `src/TargetChooser.cpp:1645-1679`). |
| 400.11c | PARTIAL | Ordinary gameplay scans omit sideboard, but effects explicitly parse sideboard targets; the inert target/move defect prevents reliable “bring into game” behavior (`src/MTGGameZones.cpp:395`, `src/TargetChooser.cpp:1645-1679`; matching ledger item). |
| 400.12 | PARTIAL | Zone-wide effects can enumerate card vectors, but no general operation treats an instruction to a zone as an atomic all-cards action (`include/MTGGameZones.h:109`, search `all cards`, `zone`). |
| 401.1 | IMPLEMENTED | Deck cards are loaded into `library`, and game initialization shuffles it (`src/MTGGameZones.cpp:50-132`, `src/MTGGameZones.cpp:224-233`). |
| 401.2 | N/A | Keeping a physical face-down pile and prohibiting physical inspection/reordering is physical-play etiquette; hidden library UI is implementation presentation. |
| 401.3 | IMPLEMENTED | Library exposes `nb_cards`, used throughout UI/rules code and therefore countable without revealing card faces (`include/MTGGameZones.h:100-110`). |
| 401.4 | UNIMPLEMENTED | Search for simultaneous multi-card library placement plus owner ordering found only deterministic vector/top/bottom operations, not an owner choice. |
| 401.5 | PARTIAL | Top-card permissions and reveal displays exist, but no cast/activation/special-action reveal freeze matching the rule was found (`src/MTGCardInstance.cpp:1076-1105`; searched top-library reveal and casting). |
| 401.6 | UNIMPLEMENTED | Search found no new-object transition when a continuously revealed top card stops and resumes being revealed. |
| 401.7 | PARTIAL | Position/top/bottom library abilities exist, but search found no explicit “Nth exceeds size => bottom” clamp in the placement class (`include/AllAbilities.h:2071-2084`). |
| 402.1 | IMPLEMENTED | Opening initialization draws seven by moving the library top into hand; other effects use the same zone mover (`src/MTGGameZones.cpp:224-233`, `src/MTGGameZones.cpp:322-371`). |
| 402.2 | INCORRECT | Cleanup enforces the numeric limit, but if the player has not discarded manually it automatically discards `hand->cards[0]` repeatedly rather than letting that player choose the excess cards (`src/GameObserver.cpp:247-259`). The related max-hand defect family is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 402.3 | PARTIAL | The opponent hand is hidden/displayed as a count and the owner hand is inspectable, but arranging a hand is UI behavior rather than an audited engine rule (`src/DuelLayers.cpp:103-109`). |
| 403.1 | IMPLEMENTED | The battlefield starts as an empty `MTGInPlay` and receives resolving permanents (`src/MTGGameZones.cpp:380-400`, `src/ActionStack.cpp:470-512`). |
| 403.2 | PARTIAL | Continuous and targeted abilities commonly default to battlefield zones, but the parser can scan many zones and correctness depends on each scripted target spec (`src/TargetChooser.cpp:1645-1679`). |
| 403.3 | IMPLEMENTED | `battlefield` aliases `inPlay`, the sole permanent container (`src/MTGGameZones.cpp:384-389`). |
| 403.4 | IMPLEMENTED | Entering uses the generic remove/clone/add new-object path and emits a zone-change event (`src/MTGGameZones.cpp:555-708`, `src/MTGGameZones.cpp:795-798`). |
| 403.5 | IMPLEMENTED | Engine parsers and zone mappings accept both `inplay` and `battlefield` (`src/MTGGameZones.cpp:1928-1930`). |
| 404.1 | IMPLEMENTED | Countered/destroyed/discarded/sacrificed/resolved-card paths converge on owner graveyard helpers (`src/MTGGameZones.cpp:407-440`, `src/ActionStack.cpp:727-731`). |
| 404.2 | PARTIAL | Graveyard is an ordered vector and publicly displayable, but the engine does not expose tournament-format graveyard-order policy as a rule (`include/MTGGameZones.h:109`, `src/MTGGameZones.cpp:918-975`). |
| 404.3 | UNIMPLEMENTED | Search for simultaneous same-graveyard insertion ordering found no owner-choice procedure; inserts follow engine event order. |
| 405.1 | IMPLEMENTED | Casts move their card to the player stack zone and create a `Spell`; abilities create cardless `StackAbility` actions (`src/MTGRules.cpp:620-650`, `src/ActionStack.cpp:325-375`, `src/ActionStack.cpp:734-742`). |
| 405.2 | IMPLEMENTED | `ActionStack::addAction` appends objects; resolution queries the last unresolved object (`src/ActionStack.cpp:816-843`, `src/ActionStack.cpp:846-852`). |
| 405.3 | UNIMPLEMENTED | Search for APNAP ordering of simultaneously triggered/stacked objects found no APNAP batch or per-controller relative-order choice. |
| 405.4 | PARTIAL | Spell objects retain source/card and caster metadata; stack abilities retain their originating ability, but delayed-trigger controller rules were not established here (`src/ActionStack.cpp:325-390`). |
| 405.5 | IMPLEMENTED | The priority state records both players’ pass/interrupt decisions and resolves the last unresolved action after passes (`src/ActionStack.cpp:790-843`, `src/ActionStack.cpp:930-1035`). |
| 405.6 | IMPLEMENTED | The engine distinguishes stack actions from immediate effects, mana actions, phase actions, and state processing (`src/ActionStack.cpp:725-843`, `src/GameObserver.cpp:802-1030`). |
| 405.6a | IMPLEMENTED | Effect objects execute as the result of resolving spell/ability actions rather than being independently cast (`src/ActionStack.cpp:325-390`, `src/ActionStack.cpp:470-512`). |
| 405.6b | IMPLEMENTED | Continuous/permanent abilities live on the action/observer layer, not as unresolved `ActionStack` objects (`src/DuelLayers.cpp:90-103`). |
| 405.6c | PARTIAL | Mana-producing abilities are resolved through direct mana ability paths, but full immediate compound-effect semantics were not independently established (mana parser/producer `src/MTGCardInstance.cpp:1054-1073`). |
| 405.6d | IMPLEMENTED | Land play resolves directly instead of calling `addSpell`, demonstrating special actions outside the stack (`src/MTGRules.cpp:475-493`). |
| 405.6e | IMPLEMENTED | `nextGamePhase` performs phase/turn actions before advancing and separately adds a phase request action (`src/GameObserver.cpp:220-270`, `src/ActionStack.cpp:767-787`). |
| 405.6f | IMPLEMENTED | State checks run in the observer update and directly move lethal/illegal permanents without adding stack objects (`src/GameObserver.cpp:802-1030`). |
| 405.6g | IMPLEMENTED | Concession sets the loser directly rather than creating a stack action (`src/GameObserver.cpp:1940-1955`). |
| 405.6h | N/A | Leaving a multiplayer game is outside the supported two-player classic scope. |
| 406.1 | IMPLEMENTED | Exile is a dedicated `MTGRemovedFromGame` zone and generic effects can move objects there (`src/MTGGameZones.cpp:389-397`, `src/MTGGameZones.cpp:449-453`). |
| 406.2 | IMPLEMENTED | `putInExile` moves an object from its current zone to its owner’s exile container (`src/MTGGameZones.cpp:449-453`). |
| 406.3 | UNIMPLEMENTED | Search for face-down exile visibility/piles found no exile face-down state; the only face-down machinery is morph/card-face rendering. |
| 406.3a | UNIMPLEMENTED | No characteristicless face-down-exile object or turn-face-up-before-announcement path was found (searched `face down`, `exile`, `isMorphed`). |
| 406.3b | UNIMPLEMENTED | No permission/look-state check for casting quality-constrained face-down exiled cards was found. |
| 406.4 | UNIMPLEMENTED | Exile is one vector per player; no separate face-down piles, look permissions, pile choice, or random-within-pile mechanism exists (`src/MTGGameZones.cpp:389-397`). |
| 406.5 | PARTIAL | Cards retain stored-card/source links that individual temporary-exile scripts can use, but exile has no engine-level separate-return-pile model (`src/MTGGameZones.cpp:938-960`). |
| 406.6 | PARTIAL | Individual abilities can retain `storedCard`/`storedSourceCard` across the move, approximating linked exile, but linkage is not enforced as a general pair of abilities (`src/MTGGameZones.cpp:943-946`). |
| 406.7 | IMPLEMENTED | Re-exiling an already exiled card takes the generic clone/new-object move path even when `from == to` (`src/MTGGameZones.cpp:467-555`, `src/MTGGameZones.cpp:918-969`). |
| 406.8 | IMPLEMENTED | The engine retains the legacy internal alias `removedFromGame` while exposing it as `exile`, so old/new terminology maps to the same zone (`src/MTGGameZones.cpp:389-392`). |
| 407.1 | N/A | Ante is an excluded optional variation. |
| 407.2 | N/A | Ante setup/ownership transfer is outside supported game types. |
| 407.3 | N/A | Ante-only deck legality is outside supported game types. |
| 407.4 | N/A | Ante moves are outside supported game types. |
| 408.1 | IMPLEMENTED | A dedicated command-zone container exists and command-zone card abilities are loaded separately from battlefield permanents (`src/MTGGameZones.cpp:395-397`, `src/MTGAbility.cpp:6573-6575`). |
| 408.2 | PARTIAL | The `emblem` parser creates a persistent ability in `ExtraRules`, but it does not create an emblem object in the command-zone container (`src/MTGAbility.cpp:2928-2935`). |
| 408.3 | N/A | Planechase, Vanguard, Commander, Archenemy, and Conspiracy Draft are all excluded game types. |

## Verdict counts

| verdict | count |
|---|---:|
| IMPLEMENTED | 86 |
| PARTIAL | 69 |
| INCORRECT | 11 |
| UNIMPLEMENTED | 27 |
| N/A | 48 |
| **TOTAL** | **241** |

## HIGHLIGHTS

1. **Battle protector rules are absent (310.8–310.11a).** There is no `protector`
   field or C++ match at all, so attack eligibility, defending player, blocking,
   protector repair, and Siege’s opponent choice cannot be represented. Battle
   combat currently falls back to controller/opponent combat logic.
2. **The engine still implements obsolete planeswalker damage redirection
   (306.7).** `src/AllAbilities.cpp:2070-2090` redirects damage aimed at a player
   to a planeswalker, contradicting the current rule and modern Oracle targeting.
3. **Generic zone moves permit instants and sorceries to enter the battlefield
   (304.4, 307.4, 400.4a).** `AAMover` and `putInZone` have no destination/type
   guard.
4. **Zone changes preserve forbidden object memory (400.7).** The new-object clone
   copies X, kicker, stored-card links, alternate-cost state, commander damage,
   and more across ordinary zone changes; this is materially broader than the
   CR’s enumerated exceptions.
5. **Battle entry/defense is script-dependent and a known real-card parser defect
   leaves Battles at zero defense (310.4b).** This matches the Battle defense
   entry in `strategy-design/lexicon-triage/engine-bugs.md`.
6. **Dungeon rules are largely a counter/script approximation, not the CR room
   model (309.2–309.7).** No venture marker, room graph, arrow choice, or
   bottom-room state action exists; completion sideboards the dungeon. This
   matches the dungeon note in the known engine ledger.
7. **Cleanup’s fallback discard chooses the wrong cards (402.2).** The engine
   automatically discards `hand->cards[0]` until the limit is met instead of
   requiring the player to choose the excess cards.
8. **Face-down exile is unimplemented (406.3–406.4).** Exile has no face-down
   visibility permissions, characteristics suppression, separated piles, or
   random pile selection.
9. **Sideboard/command-zone target moves are known inert (400.11b–c).** This
   cross-references the existing target-matrix defect in
   `strategy-design/lexicon-triage/engine-bugs.md`.
10. **Simultaneous object ordering gaps remain (401.4, 404.3, 405.3).** The engine
    has ordered vectors/stacks but no owner choice for simultaneous library or
    graveyard placement and no APNAP trigger-batch ordering.
