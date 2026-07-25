## 500s

| rule | verdict | evidence / note |
|---|---|---|
| 500.1 | INCORRECT | The normal ring has the ordered steps/phases (`src/PhaseRing.cpp:48-83`), but a new game is initialized at first main (`src/Rules.cpp:114-118`, `src/Rules.cpp:822-843`), so turn 1 omits the starting player's entire beginning phase rather than only its draw. `src/PreGamePhase.cpp:303-311` confirms this is deliberate. |
| 500.2 | IMPLEMENTED | Phase advance is blocked while the stack/choice/cost machinery is active (`src/GameObserver.cpp:360-414`); `NextGamePhase` then runs successive priority decisions before advancing (`src/ActionStack.cpp:1053-1075`, `src/ActionStack.cpp:1141-1205`). |
| 500.3 | INCORRECT | Untap and cleanup are ordinary ring phases (`src/PhaseRing.cpp:48-83`), and generic phase advancement may create a priority object in either (`src/GameObserver.cpp:360-414`). CR permits no priority in untap and only conditional priority in cleanup. |
| 500.4 | UNIMPLEMENTED | Good-faith searches for step/phase duration expiry (`until.*phase`, `until.*step`, phase-duration classes) found no general duration machinery; the parser only has the special `ueot` wrapper (`src/MTGAbility.cpp:2892-2901`). |
| 500.5 | PARTIAL | Every phase has a scripted `removeMana(*)` trigger (`bin/Res/rules/mtg.txt:56-68`) and `AARemoveMana` empties pools (`src/AllAbilities.cpp:7248-7341`), but it runs through phase-trigger ability machinery rather than as a non-stack end-of-step action, and general end-of-step durations are absent. |
| 500.5a | UNIMPLEMENTED | A named search for `until end of combat`, `ueoc`, and combat-duration machinery found only “remove from combat”; there is no duration that expires at the combat-phase boundary. |
| 500.5b | PARTIAL | `ueot` effects are parsed (`src/MTGAbility.cpp:2892-2901`) and expire around `MTG_PHASE_AFTER_EOT` (`src/AllAbilities.cpp:8470-8485`), but cleanup is not implemented as CR 514.2's simultaneous turn-based action. |
| 500.6 | IMPLEMENTED | `TriggerAtPhase::triggerOnEvent` recognizes phase transitions and creates ordinary triggered abilities for the next priority point (`src/MTGAbility.cpp:8439-8518`). |
| 500.7 | INCORRECT | Extra turns are scalar counters on each player and the current player simply repeats while their counter is positive (`src/GameObserver.cpp:145-178`; `src/AllAbilities.cpp:2456-2477`). This cannot preserve per-effect APNAP insertion and most-recent-first ordering when both players receive extra turns. |
| 500.8 | IMPLEMENTED | Extra combat/main phases are inserted directly after the anchor, and repeated insertions at the same anchor are consequently LIFO (`src/PhaseRing.cpp:229-310`). |
| 500.9 | PARTIAL | `PhaseRing::addPhaseAfter` inserts a cloned step immediately after its anchor with later same-anchor insertions first (`src/PhaseRing.cpp:327-353`), but there is no corresponding insert-before operation for effects that add a step directly before a specified step. |
| 500.10 | UNIMPLEMENTED | `APhaseAlter` either adds a whole combat or clones the named phase/step (`src/AllAbilities.h:6882-7014`); no search result implements construction of the containing phase with all other steps skipped. |
| 500.10a | UNIMPLEMENTED | `APhaseAlter::addTheEffect` has no check that a “you get” addition occurs only in its controller's turn (`src/AllAbilities.h:6970-7001`). |
| 500.11 | PARTIAL | Named phases can be removed and turns can be skipped (`src/PhaseRing.cpp:355-374`; `src/GameObserver.cpp:145-178`), but skip-turn is a per-player scalar and the flat phase representation does not implement all step/containing-phase semantics. |
| 500.12 | IMPLEMENTED | Phase alteration is dispatched as `WEventPhasePreChange` while still in the old phase, then the ring advances and emits `WEventPhaseChange` (`src/PhaseRing.cpp:162-203`); no intervening gameplay state exists. |

## 501s

| rule | verdict | evidence / note |
|---|---|---|
| 501.1 | INCORRECT | The ring orders untap, upkeep, draw (`src/PhaseRing.cpp:48-83`), but initial state jumps to first main (`src/Rules.cpp:822-843`), omitting all three on turn 1. |

## 502s

| rule | verdict | evidence / note |
|---|---|---|
| 502.1 | IMPLEMENTED | Entering untap calls `phasingPhase`, which processes all active-player permanents before untapping (`src/GameObserver.cpp:204-220`, `src/GameObserver.cpp:1913-1950`); attachment phasing is synchronized (`src/GameObserver.cpp:1002-1010`). |
| 502.2 | PARTIAL | Day/night markers record spell counts and transition during untap, with focused fixtures (`bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt`, `bin/Res/test/lexicon/basic_nightbound_flips_after_two_spells.txt`), but this is a scripted phase-trigger ability, not a non-stack turn-based action. |
| 502.2a | N/A | Shared-team-turn multiplayer rule; multiplayer is excluded by the audit frame. |
| 502.3 | PARTIAL | `MTGInPlay::untapAll` applies does-not-untap, exert, and freeze constraints before `attemptUntap` (`src/MTGGameZones.cpp:1396-1424`), but the engine performs sequential calls and has no general active-player choice/combined determination for replacement-style untap constraints. |
| 502.4 | INCORRECT | Untap remains a normal phase eligible for `NextGamePhase` priority handling (`src/GameObserver.cpp:360-414`), and `TriggerAtPhase` uses ordinary triggered-ability resolution (`src/MTGAbility.cpp:8439-8518`), rather than holding all untap triggers until upkeep with no spells/abilities resolving. |

## 503s

| rule | verdict | evidence / note |
|---|---|---|
| 503.1 | IMPLEMENTED | Upkeep has no bespoke turn-based action and uses the normal active-player priority route (`src/GameObserver.cpp:360-414`). |
| 503.1a | PARTIAL | Beginning-upkeep triggers are stacked before the normal priority decision (`src/MTGAbility.cpp:8439-8518`, `src/ActionStack.cpp:1141-1205`), but untap triggers can wrongly be offered/resolved during untap because 502.4 is not enforced. |
| 503.2 | UNIMPLEMENTED | A named search for `after upkeep`, upkeep-completion counters, and multiple-upkeep timing found no restriction state distinguishing “after the first upkeep” from phase IDs. |

## 504s

| rule | verdict | evidence / note |
|---|---|---|
| 504.1 | INCORRECT | The normal draw is scripted as `auto=@each my draw:draw:1` (`bin/Res/rules/mtg.txt:45-46`); `AADrawer` creates/resolves a draw action through ability machinery (`src/AllAbilities.cpp:4945-4981`) instead of a non-stack turn-based action. Turn 1 also skips the whole beginning phase (`src/PreGamePhase.cpp:303-311`). |
| 504.2 | IMPLEMENTED | After the phase-start draw trigger resolves, the generic phase machinery gives the active player the first priority decision (`src/ActionStack.cpp:1053-1075`, `src/ActionStack.cpp:1141-1205`). |

## 505s

| rule | verdict | evidence / note |
|---|---|---|
| 505.1 | IMPLEMENTED | The default ring places first main before combat and second main after combat (`src/PhaseRing.cpp:48-83`; phase IDs in `include/MTGDefinitions.h:43-60`). |
| 505.1a | IMPLEMENTED | `addCombatAfter` adds the extra combat followed, when requested, by a clone of `MTG_PHASE_SECONDMAIN`, so additional mains are postcombat (`src/PhaseRing.cpp:229-310`). |
| 505.1b | UNIMPLEMENTED | Main phases are represented only as FIRSTMAIN or SECONDMAIN IDs (`include/MTGDefinitions.h:43-60`); a named search found no per-turn ordinal counter for “first/second/... main phase.” |
| 505.2 | IMPLEMENTED | Main phases are single ring nodes and finish through the stack-empty successive-priority process (`src/PhaseRing.cpp:48-83`, `src/ActionStack.cpp:1141-1205`). |
| 505.3 | N/A | Archenemy-only turn-based action; that casual multiplayer variant is excluded by the audit frame. |
| 505.4 | UNIMPLEMENTED | Named searches for Saga, lore-counter, and chapter turn-based machinery found no precombat-main Saga action in `src/` or `include/`; incidental subtype/text matches do not implement it. |
| 505.5 | N/A | Attractions are a casual-variant mechanic excluded by the audit frame. |
| 505.6 | IMPLEMENTED | The main phase uses the generic active-player priority sequence (`src/ActionStack.cpp:1053-1075`, `src/ActionStack.cpp:1141-1205`). |
| 505.6a | IMPLEMENTED | `StackIsEmptyandSorcerySpeed` requires an empty stack, the controller's first/second main, and the controller's turn (`src/MTGCardInstance.cpp:1109-1122`). |
| 505.6b | IMPLEMENTED | Land play is main/sorcery-speed gated (`src/MTGCardInstance.cpp:1109-1122`) and the rules layer installs `maxPlay(land)1` (`bin/Res/rules/mtg.txt:47`); the action moves the land directly rather than putting a spell on the stack (`src/MTGRules.cpp:337-349`). |

## 506s

| rule | verdict | evidence / note |
|---|---|---|
| 506.1 | IMPLEMENTED | The ring contains all five ordered combat steps and skips blockers/damage after no attack (`src/PhaseRing.cpp:48-83`, `src/GameObserver.cpp:180-212`); internal combat states provide first/double-strike damage processing (`src/GameObserver.cpp:295-321`). |
| 506.2 | IMPLEMENTED | Combat selection is limited to the active player's creatures and supports the opponent, opposing planeswalkers, and opponent-protected battles as attack destinations (`src/MTGRules.cpp:2215-2338`). |
| 506.2a | N/A | Multiplayer defending-player selection is excluded by the audit frame. |
| 506.2b | N/A | Shared-team-turn multiplayer rule; excluded by the audit frame. |
| 506.3 | INCORRECT | `canAttack`/`canBlock` require a creature but do not reject a permanent that is also a battle (`src/MTGCardInstance.cpp:841-865`, `src/MTGCardInstance.cpp:1124-1134`), contrary to the battle exception. |
| 506.3a | INCORRECT | Put-attacking helpers directly clear summoning sickness and call `setAttacker` without a noncreature guard (`include/AllAbilities.h:2310-2341`, `src/AllAbilities.cpp:6601-6606`). |
| 506.3b | UNIMPLEMENTED | A named search of the put-attacking helpers found no validation that the entering creature is controlled by the attacking player (`include/AllAbilities.h:2310-2341`). |
| 506.3c | INCORRECT | `setAttacker(1)` defaults the combat destination to the opponent and the put-attacking helpers do not validate a specified player/permanent at resolution (`include/AllAbilities.h:2310-2341`; `src/GuiCombat.cpp:535-536`). |
| 506.3d | PARTIAL | Direct `setAttacker` begins with no blocker and therefore behaves unblocked (`src/MTGCardInstance.cpp:1653-1682`), but no timing-specific entry rule or persistence guard exists. |
| 506.3e | PARTIAL | Effect-based blocking uses `canBlock` and an attacking target (`include/AllAbilities.h:2389-2422`), which suffices for the two-player relation, but has no explicit defending-player/protector validation. |
| 506.3f | INCORRECT | Both declaration predicates omit the required “is not also a battle” check (`src/MTGCardInstance.cpp:841-865`, `src/MTGCardInstance.cpp:1124-1134`). |
| 506.3g | UNIMPLEMENTED | A named search found no guard preventing a battle from becoming an attacking/blocking creature through resolving ability code. |
| 506.4 | PARTIAL | Regeneration, phasing, and explicit removal clear combat state (`src/MTGCardInstance.cpp:735-775`; `include/AllAbilities.h:2359-2385`), but searches found no general type-change/controller-change handling for every listed object and attacked permanent. |
| 506.4a | IMPLEMENTED | Attack/block legality is tested during declaration transitions (`src/MTGRules.cpp:2124-2175`, `src/MTGRules.cpp:2498-2619`); later CANTATTACK/CANTBLOCK changes do not themselves call combat removal. |
| 506.4b | IMPLEMENTED | Damage assignment tests attacker/blocker state, not tapped state (`src/GuiCombat.cpp:521-549`), and tap/untap methods do not clear combat state. |
| 506.4c | INCORRECT | Damage falls back to the opponent when `isAttacking` is null (`src/GuiCombat.cpp:535-536`), so an unblocked attacker whose attacked permanent vanished can deal player damage instead of assigning none. |
| 506.4d | UNIMPLEMENTED | Named searches found no split combat-role retention logic for a permanent that is simultaneously blocking and an attacked planeswalker. |
| 506.4e | UNIMPLEMENTED | Named searches found no planeswalker-plus-battle attacked-object type-transition matrix or protector/control exception. |
| 506.5 | PARTIAL | `MTGCombatTriggersRule` implements “attacks alone” by counting attackers (`src/MTGRules.cpp:2397-2418`), but a named search found no corresponding declared/blocking-alone event distinction. |
| 506.6 | UNIMPLEMENTED | MUSTATTACK is checked live during declaration (`src/MTGRules.cpp:2124-2148`); no “had to attack” snapshot/query survives for later abilities. |
| 506.7 | PARTIAL | Combat timing restrictions are parsed as coarse comparisons of the current phase ID (`src/MTGAbility.cpp:877-900`), not all of the rule's declaration points and skipped-step cases. |
| 506.7a | PARTIAL | “Before attackers” maps to phase IDs through beginning of combat (`src/MTGAbility.cpp:877-900`), but the parser has no exact declaration-action boundary within a step. |
| 506.7b | PARTIAL | “Before battle damage” and coarse combat ranges exist (`src/MTGAbility.cpp:877-900`), but no exact before/after-blockers-declared boundary is represented. |
| 506.7c | IMPLEMENTED | “During battle” accepts all combat phase IDs (`src/MTGAbility.cpp:893-899`), including cloned extra combats because those use the same IDs (`src/PhaseRing.cpp:229-310`). |
| 506.7d | UNIMPLEMENTED | A named search found no combat-ordinal tracking with which to restrict an unqualified timing phrase to the first combat. |
| 506.7e | UNIMPLEMENTED | The phase-ID restriction parser has no fallback from a nonexistent blockers/damage point to the end of attackers or from skipped combat to precombat main (`src/MTGAbility.cpp:877-900`). |
| 506.7f | UNIMPLEMENTED | The parser does not record whether blockers were actually declared; it cannot prohibit “after blockers” when that step was skipped (`src/MTGAbility.cpp:877-900`). |
| 506.7g | PARTIAL | Activated abilities share the cast-restriction strings and coarse phase checks (`src/MTGAbility.cpp:877-900`), inheriting all 506.7a-f gaps. |

## 507s

| rule | verdict | evidence / note |
|---|---|---|
| 507.1 | N/A | Multiplayer-only defending-player selection; excluded by the audit frame. |
| 507.2 | IMPLEMENTED | Beginning of combat is a normal priority-bearing ring step (`src/PhaseRing.cpp:48-83`, `src/GameObserver.cpp:360-414`). |

## 508s

| rule | verdict | evidence / note |
|---|---|---|
| 508.1 | PARTIAL | The engine collects attackers and validates them when leaving the step (`src/MTGRules.cpp:2098-2196`), but it mutates/payments per creature and lacks the CR's atomic illegal-declaration rollback. |
| 508.1a | PARTIAL | `canAttack` enforces creature, battlefield, untapped, haste/summoning sickness, and CANTATTACK (`src/MTGCardInstance.cpp:841-865`) but fails to exclude creatures that are also battles. |
| 508.1b | IMPLEMENTED | Each attacker can select the opponent, an opposing planeswalker, or an opponent-protected battle (`src/MTGRules.cpp:2215-2338`). |
| 508.1c | PARTIAL | CANTATTACK and script restrictions are checked (`src/MTGCardInstance.cpp:841-865`, `src/MTGRules.cpp:2124-2175`), but illegal combinations are cleared/forced rather than rolling the whole declaration back. |
| 508.1d | INCORRECT | MUSTATTACK is greedily forced rather than maximizing jointly, and is expressly skipped for extra combats (`src/MTGRules.cpp:2139-2148`), although the rule applies in every declare-attackers step. |
| 508.1e | UNIMPLEMENTED | Banding state is not implemented as a declaration choice; known-defect catalog entry “Banding” agrees, and named searches found no band construction in `src/MTGRules.cpp`. |
| 508.1f | IMPLEMENTED | On successful declaration, every non-vigilant attacker is tapped (`src/MTGRules.cpp:2149-2152`). |
| 508.1g | UNIMPLEMENTED | A named search found no optional “as this attacks” cost-choice stage; only mandatory per-card attack-cost handling exists. |
| 508.1h | INCORRECT | Costs are requested and paid per selected creature (`src/MTGRules.cpp:1916-1976`), not totaled and locked before payment. The known defect catalog also records an unpayable alternate attack-cost wedge. |
| 508.1i | PARTIAL | Mana can be produced through the ordinary cost UI (`src/MTGRules.cpp:1916-1976`), but there is no distinct mana-ability window after an atomic total is locked. |
| 508.1j | PARTIAL | Individual mana costs require complete payment (`src/MTGRules.cpp:1916-1976`), but all attack costs are not paid as one total in arbitrary order. |
| 508.1k | PARTIAL | Selected cards become attackers, but `initAttackersDefensers` is deferred until later cleanup (`src/MTGCardInstance.cpp:760-775`, `src/Player.cpp:382-387`), so attacker status can outlive the combat phase. |
| 508.1m | IMPLEMENTED | Leaving attackers emits declared-attack events before priority in blockers (`src/MTGRules.cpp:2362-2379`). |
| 508.2 | IMPLEMENTED | The attacker step's transition enters the normal active-player priority system (`src/GameObserver.cpp:360-414`). |
| 508.2a | PARTIAL | Attack triggers inspect characteristics when the leave-attackers event is processed (`src/MTGRules.cpp:2362-2379`), slightly after interactive selection rather than snapshotting each object at the declaration event. |
| 508.2b | IMPLEMENTED | Attack events are emitted on the transition and their triggered abilities enter the action stack before the next priority decision (`src/MTGRules.cpp:2362-2379`, `src/ActionStack.cpp:1141-1205`). |
| 508.3 | PARTIAL | Several attack trigger events exist (`src/MTGRules.cpp:2362-2452`), but the following subrules expose missing distinctions. |
| 508.3a | INCORRECT | The transition loops over every card currently marked attacker and calls `eventattacked` (`src/MTGRules.cpp:2362-2379`), so a creature put onto the battlefield attacking before that transition can incorrectly trigger “attacks.” |
| 508.3b | UNIMPLEMENTED | A named search found no once-per-attacked-player/planeswalker/battle declaration event; existing events are per creature. |
| 508.3c | PARTIAL | Per-creature attack events can drive controller-based filters (`src/MTGRules.cpp:2362-2379`), but there is no first-class “[player] attacks with” event distinct from creature attack. |
| 508.3d | PARTIAL | `raidcount` and per-attacker events expose that the active player attacked (`src/MTGRules.cpp:2362-2379`), but there is no single once-per-declaration player-attacks event. |
| 508.3e | INCORRECT | The known-defect catalog's “attacked-entity primitive missing” applies: triggers cannot reliably distinguish attacking a player from attacking their planeswalker/battle; attack target is stored per creature but no matching event is emitted (`src/MTGRules.cpp:2362-2379`). |
| 508.3f | IMPLEMENTED | “Attacks and isn't blocked” events are emitted only when entering/processing the blockers result (`src/MTGRules.cpp:2420-2452`). |
| 508.4 | INCORRECT | Put-attacking helpers directly call `setAttacker` and default to the opponent (`include/AllAbilities.h:2310-2341`, `src/GuiCombat.cpp:535-536`), provide no destination choice, and can be counted as having attacked by the transition loop. |
| 508.4a | UNIMPLEMENTED | A named search of put-attacking resolution found no revalidation of a specified player, planeswalker, battle, controller, or protector (`include/AllAbilities.h:2310-2341`). |
| 508.4b | UNIMPLEMENTED | Effect-based “is attacking” has no specified-destination validity check before `setAttacker` (`src/AllAbilities.cpp:6601-6606`). |
| 508.4c | IMPLEMENTED | Direct `setAttacker` bypasses declaration-only `canAttack` and MUSTATTACK checks (`include/AllAbilities.h:2310-2341`). |
| 508.4d | PARTIAL | A newly marked attacker has no blocker and is therefore unblocked (`src/MTGCardInstance.cpp:1653-1682`), but no timing-specific persistence rule protects that designation. |
| 508.5 | INCORRECT | Combat damage can consult current `isAttacking`, but removal clears it and retains no former destination (`src/MTGCardInstance.cpp:760-775`); the required last defending player cannot then be recovered. |
| 508.5a | N/A | Multiplayer-specific defending-player disambiguation; excluded by the audit frame. |
| 508.6 | PARTIAL | Current attack and `didattacked` state exist (`src/MTGCardInstance.cpp:760-775`, `src/MTGRules.cpp:2362-2379`), but attacked-player identity and declared-versus-put-attacking semantics are not preserved reliably. |
| 508.7 | UNIMPLEMENTED | A named search for `reselect` and attack-destination reassignment found only card/rules text, no engine action. |
| 508.7a | UNIMPLEMENTED | No reselect action exists, so its no-removal/no-second-attack/history semantics are absent. |
| 508.7b | UNIMPLEMENTED | No reselect action exists to bypass declaration restrictions. |
| 508.7c | UNIMPLEMENTED | No reselect action exists to validate the new opponent/planeswalker/battle. |
| 508.7d | N/A | Multiplayer option restriction; excluded by the audit frame. |
| 508.7e | N/A | Multiplayer limited-range restriction; excluded by the audit frame. |
| 508.8 | IMPLEMENTED | If no attacker remains, `nextGamePhase` advances twice more, skipping blockers and damage (`src/GameObserver.cpp:199-212`). |

## 509s

| rule | verdict | evidence / note |
|---|---|---|
| 509.1 | PARTIAL | The defending player assigns blockers and the engine checks restrictions/requirements (`src/MTGRules.cpp:2498-2711`), but declarations are mutated rather than atomically validated and rolled back. |
| 509.1a | PARTIAL | `canBlock` requires an untapped battlefield creature unless CANBLOCKTAPPED applies (`src/MTGCardInstance.cpp:1124-1134`), but does not reject a creature that is also a battle. |
| 509.1b | PARTIAL | CANTBLOCK, lure, menace, and similar restrictions are enforced (`src/MTGRules.cpp:2498-2619`), but invalid assignments are cleared rather than rolling back an entire declaration. |
| 509.1c | INCORRECT | MUSTBLOCK greedily chooses the first legal attack target and the later checks clear assignments (`src/MTGRules.cpp:2498-2619`); there is no maximum-satisfiable-requirements solver, including per-extra-combat evaluation. |
| 509.1d | INCORRECT | Blocking costs are requested per card (`src/MTGRules.cpp:1996-2055`), not totaled and locked before any payment. |
| 509.1e | PARTIAL | The per-card cost UI permits mana production (`src/MTGRules.cpp:1996-2055`), but there is no post-lock aggregate mana-ability window. |
| 509.1f | PARTIAL | Each individual block cost must be paid (`src/MTGRules.cpp:1996-2055`), but costs are not one aggregate payable in arbitrary order. |
| 509.1g | PARTIAL | Chosen creatures receive blocking/defenser state, but combat state is not cleared at combat end (`src/MTGCardInstance.cpp:1690-1713`, `src/Player.cpp:382-387`). |
| 509.1h | INCORRECT | `removeBlocker` resets the attacker to unblocked whenever its last blocker leaves while still in the blockers phase (`src/MTGCardInstance.cpp:1703-1713`). CR requires the blocked status to remain unchanged through combat. |
| 509.1i | IMPLEMENTED | Completion emits `WEventBlockersChosen` and combat-trigger rules process block events (`src/GameObserver.cpp:224-226`, `src/MTGRules.cpp:2420-2452`). |
| 509.2 | IMPLEMENTED | The blockers step uses the normal active-player priority route after declaration (`src/GameObserver.cpp:360-414`). |
| 509.2a | IMPLEMENTED | `WEventBlockersChosen` is emitted before the priority round and its triggered abilities enter the action stack (`src/GameObserver.cpp:224-226`, `src/ActionStack.cpp:1141-1205`). |
| 509.3 | PARTIAL | Block/becomes-blocked event machinery exists (`src/MTGRules.cpp:2420-2452`), but it cannot express all trigger distinctions below. |
| 509.3a | PARTIAL | Blocking emits a blocker event (`src/MTGRules.cpp:2420-2452`), but effect-caused and put-onto-battlefield blocking are not represented distinctly enough to guarantee exactly the listed cases. |
| 509.3b | UNIMPLEMENTED | A blocker stores only one `defenser` pointer (`src/MTGCardInstance.cpp:1690-1713`); it cannot block multiple attackers or trigger once per attacked creature. |
| 509.3c | PARTIAL | The attacker-blocked event is emitted at declared blockers (`src/MTGRules.cpp:2420-2452`), but later effect-based changes do not robustly track the transition from unblocked exactly once. |
| 509.3d | IMPLEMENTED | The blockers-chosen loop emits an attacked-blocked event for each blocker assigned to the attacker (`src/MTGRules.cpp:2420-2452`). |
| 509.3e | UNIMPLEMENTED | A named search found no transition watcher for exact/at-least blocker counts when effects add or remove blockers. |
| 509.3f | IMPLEMENTED | Characteristics are evaluated when `WEventBlockersChosen` is processed and the relevant per-card event is emitted (`src/MTGRules.cpp:2420-2452`), not on later unrelated changes. |
| 509.3g | IMPLEMENTED | The blockers result emits “attacked not blocked” once for attackers with no declared blockers (`src/MTGRules.cpp:2420-2452`); later blocker removal does not rerun that declaration event. |
| 509.4 | UNIMPLEMENTED | A named search found no effect that puts a new permanent onto the battlefield blocking with an as-it-enters attacker choice; `AABlock` only changes an existing card's state (`include/AllAbilities.h:2389-2422`). |
| 509.4a | UNIMPLEMENTED | No put-onto-battlefield-blocking action exists to perform the specified attacker/defending-player validity checks. |
| 509.4b | UNIMPLEMENTED | No put-onto-battlefield-blocking action exists to bypass declaration-only requirements/restrictions while retaining blocking state. |

## 510s

| rule | verdict | evidence / note |
|---|---|---|
| 510.1 | PARTIAL | `GuiCombat` builds assignments for attackers then blockers (`src/GuiCombat.cpp:126-169`, `src/GuiCombat.cpp:521-549`), but legacy ordering/lethal constraints and single-defenser representation prevent all legal current-CR assignments. |
| 510.1a | IMPLEMENTED | `stepPower` returns combat power (or combat toughness where appropriate) floored at zero and filters first/double-strike steps (`src/MTGCardInstance.cpp:2075-2094`). |
| 510.1b | INCORRECT | When an unblocked attacker's destination pointer is absent, damage is assigned to the opponent as a fallback (`src/GuiCombat.cpp:535-536`); the rule requires no assignment. |
| 510.1c | INCORRECT | Assignment code enforces legacy sequential lethal allocation/order (`src/GuiCombat.cpp:126-169`) rather than allowing the current rule's arbitrary division among multiple blockers; last-blocker removal can also wrongly make the attacker hit the opponent (`src/MTGCardInstance.cpp:1703-1713`). |
| 510.1d | UNIMPLEMENTED | A blocker has only one `defenser` pointer (`src/MTGCardInstance.cpp:1690-1713`), so it cannot divide damage among two or more attackers. |
| 510.1e | PARTIAL | The GUI validates/adjusts assignments per creature (`src/GuiCombat.cpp:126-169`), but has no total-assignment rollback and itself excludes legal divisions under 510.1c. |
| 510.2 | IMPLEMENTED | All entries are accumulated into `DamageStack` before `resolve()` is called, so combat assignments resolve as a batch with no intervening priority (`src/GuiCombat.cpp:521-549`). |
| 510.3 | IMPLEMENTED | After the damage internal state completes, the combat-damage step uses the generic active-player priority machinery (`src/GameObserver.cpp:295-321`, `src/GameObserver.cpp:360-414`). |
| 510.3a | IMPLEMENTED | Damage resolves through `DamageStack`, generates events/triggers, and the action stack handles them before the next priority round (`src/GuiCombat.cpp:521-549`, `src/ActionStack.cpp:1141-1205`). |
| 510.4 | INCORRECT | Internal states create two passes, but `stepPower` tests first/double strike live in each pass (`src/GameObserver.cpp:295-321`, `src/MTGCardInstance.cpp:2075-2094`) rather than remembering which creatures had neither ability as the first step began. Losing first strike can wrongly grant second-step damage, and gaining it can wrongly suppress that damage. |

## 511s

| rule | verdict | evidence / note |
|---|---|---|
| 511.1 | IMPLEMENTED | End of combat is a normal ring step with no bespoke turn-based action and normal priority handling (`src/PhaseRing.cpp:48-83`, `src/GameObserver.cpp:360-414`). |
| 511.2 | PARTIAL | Phase-trigger machinery can fire beginning-of-combat-end abilities (`src/MTGAbility.cpp:8439-8518`), but no “until end of combat” duration expires at the later combat-phase boundary. |
| 511.3 | INCORRECT | Leaving combat end does not call `initAttackersDefensers`; combat flags are reset through later card/player cleanup (`src/MTGRules.cpp:2380-2394`, `src/MTGCardInstance.cpp:760-775`, `src/Player.cpp:382-387`), allowing attacking/blocking status to persist into postcombat main. |

## 512s

| rule | verdict | evidence / note |
|---|---|---|
| 512.1 | IMPLEMENTED | The phase ring places end step followed by cleanup (`src/PhaseRing.cpp:48-83`; `include/MTGDefinitions.h:57-59`). |

## 513s

| rule | verdict | evidence / note |
|---|---|---|
| 513.1 | IMPLEMENTED | End step has no special turn-based action and enters normal priority handling (`src/PhaseRing.cpp:48-83`, `src/GameObserver.cpp:360-414`). |
| 513.1a | N/A | This is an Oracle errata/history convention, not an engine turn action; Oracle-data correctness is handled by card-data audits rather than the 5xx turn engine. |
| 513.2 | IMPLEMENTED | Phase triggers require a transition (`newPhase != currentPhase`); an ability created while already in the end step records that current phase and cannot retroactively fire until a later end-step transition (`src/MTGAbility.cpp:8439-8518`, `src/AllAbilities.cpp:9703-9791`). |

## 514s

| rule | verdict | evidence / note |
|---|---|---|
| 514.1 | INCORRECT | Over-limit discard is allowed manually during cleanup (`src/GameObserver.cpp:1776-1786`) but forced only after advancing to `MTG_PHASE_AFTER_EOT` (`src/GameObserver.cpp:247-270`), not first in cleanup as a non-stack turn-based action. This matches known-defect catalog entry “sethand / cleanup discard not enforced.” |
| 514.2 | INCORRECT | Damage removal is a scripted `@each cleanup ... resetDamage` trigger (`bin/Res/rules/mtg.txt:87-88`; `src/AllAbilities.cpp:4559-4579`) and `ueot` expiry is separate phase-update machinery (`src/MTGAbility.cpp:2892-2901`), rather than one simultaneous non-stack action including phased-out permanents. |
| 514.3 | INCORRECT | Cleanup is an ordinary priority-bearing ring phase (`src/PhaseRing.cpp:48-83`, `src/GameObserver.cpp:360-414`), so priority is not limited to the CR exception. |
| 514.3a | UNIMPLEMENTED | A named search found no cleanup SBA/trigger exception followed by creation of another cleanup step; the ring has exactly one cleanup node per turn (`src/PhaseRing.cpp:48-83`). |

## Summary

| verdict | count |
|---|---:|
| IMPLEMENTED | 41 |
| PARTIAL | 39 |
| INCORRECT | 27 |
| UNIMPLEMENTED | 31 |
| N/A | 10 |
| **Total** | **148** |

## HIGHLIGHTS

1. **Turn 1 starts at first main, deleting untap and upkeep as well as the starting player's draw (500.1, 501.1, 504.1).** The new pregame path explicitly preserves this old shortcut (`src/PreGamePhase.cpp:303-311`). Beginning-phase triggers and first-upkeep behavior therefore cannot occur on the starting player's first turn.
2. **Untap and cleanup incorrectly operate as ordinary priority-bearing phases (500.3, 502.4, 514.3).** Untap triggers can be acted on too early, and cleanup lacks the CR's no-priority default plus repeat-cleanup exception.
3. **Cleanup's mandatory actions occur at the wrong times and through the wrong mechanism (514.1-514.3a).** Hand-size discard is delayed until after cleanup, damage removal is a scripted trigger, end-of-turn expiry is separate, and a trigger/SBA cleanup never creates another cleanup. The discard failure matches the known defect catalog.
4. **Blocked status is lost when the last blocker leaves during the blockers phase (509.1h).** `removeBlocker` flips the attacker back to unblocked (`src/MTGCardInstance.cpp:1703-1713`), enabling player/planeswalker/battle damage that CR expressly forbids.
5. **Combat roles persist past the combat phase (508.1k, 509.1g, 511.3).** The engine does not remove all attackers, blockers, planeswalkers, and battles from combat at the end-of-combat boundary, so second-main queries can observe stale combat state.
6. **Combat damage assignment follows obsolete ordering/lethal constraints, cannot represent a blocker damaging multiple attackers, and does not snapshot first-strike eligibility (510.1c-510.1e, 510.4).** Current 510.1c permits arbitrary division among blockers; the GUI still constrains assignment sequentially. Live keyword checks also mishandle gaining or losing first strike between damage steps.
7. **An attacker whose planeswalker/battle destination disappears can damage the opponent (506.4c, 510.1b).** `GuiCombat` substitutes the opponent for a null destination instead of assigning no combat damage.
8. **Attack/block declarations are not atomic CR procedures (508.1, 509.1).** Costs are paid per creature rather than totaled and locked; MUSTATTACK is not maximized and is skipped in extra combats; MUSTBLOCK is greedily assigned. The alternate attack-cost wedge is already in the known defect catalog.
9. **Put-attacking/put-blocking and attacked-entity semantics are substantially absent (506.3a-g, 508.3a-e, 508.4, 509.4).** Put-attacking defaults to the opponent, lacks type/destination validation, and can wrongly fire “attacks”; no true put-onto-battlefield-blocking action exists.
10. **Creatures that are also battles are allowed to attack and block (506.3, 506.3f, 508.1a, 509.1a).** Both declaration predicates check “creature” but omit the explicit battle prohibition.
11. **Extra-turn sequencing is not CR-correct (500.7).** Per-player scalar counters cannot model APNAP insertion and most-recent-created-first ordering across effects for both players.
12. **General phase/step duration and advanced insertion/timing semantics are missing (500.4, 500.5a, 500.10, 503.2, 506.7d-f).** The flat ring handles common extra phases well but cannot synthesize a containing phase for an added step, enforce controller-turn-only additions, or distinguish first-combat/skipped-step timing.
13. **Saga precombat lore-counter placement is absent (505.4).** No engine-side precombat turn-based action was found.
