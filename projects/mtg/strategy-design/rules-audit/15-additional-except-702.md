# CR 7xx audit (excluding 702) — additional rules

Scope and verdicts follow [00-frame.md](00-frame.md). Rules were read from `data/sec-700s-except-702.txt`; engine evidence was inspected in `projects/mtg/src`, `projects/mtg/include`, the regression fixtures, and the known-defect catalogue. Searches described as having no result covered both `projects/mtg/src` and `projects/mtg/include`. The 704 rows deliberately distinguish the existence of a state-effects pass from CR-correct timing, simultaneity, fixed-point repetition, and last-known-information behavior.

## 700–799

| rule | verdict | evidence / note |
|---|---|---|
| 700.1 | PARTIAL | The event queue represents game happenings (`src/GameObserver.cpp:1962-1990`), but one happening is not exposed as a CR event aggregate for arbitrary trigger/replacement partitioning. |
| 700.2 | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2a | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2b | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2c | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2d | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2e | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2f | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2g | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2h | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.2i | PARTIAL | Modal spell/ability choices and target menus exist, but modes are not a first-class object carrying all CR choices/copy semantics (`src/MTGAbility.cpp:2041-2308`, `src/AllAbilities.cpp:4374-4450`). |
| 700.3 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'pile' projects/mtg/src projects/mtg/include`. No generic pile grouping object exists. |
| 700.3a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'pile' projects/mtg/src projects/mtg/include`. No generic pile grouping object exists. |
| 700.3b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'pile' projects/mtg/src projects/mtg/include`. No generic pile grouping object exists. |
| 700.3c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'pile' projects/mtg/src projects/mtg/include`. No generic pile grouping object exists. |
| 700.3d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'pile' projects/mtg/src projects/mtg/include`. No generic pile grouping object exists. |
| 700.4 | IMPLEMENTED | Dies is represented by battlefield-to-graveyard events (`include/AllAbilities.h:236-245`, `src/MTGRules.cpp:3499-3552`). |
| 700.5 | PARTIAL | Devotion-like symbol counts exist through parsed count expressions, but no CR 700.5a pre-layer devotion calculation was found (`src/WParsedInt.cpp:1670-1750`). |
| 700.5a | PARTIAL | Devotion-like symbol counts exist through parsed count expressions, but no CR 700.5a pre-layer devotion calculation was found (`src/WParsedInt.cpp:1670-1750`). |
| 700.6 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'historic' projects/mtg/src projects/mtg/include`. |
| 700.7 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'this object identity' projects/mtg/src projects/mtg/include`. |
| 700.8 | PARTIAL | Party is approximated by affinity/count expressions, not a maximum matching assignment engine (`include/MTGDefinitions.h:340`, `src/MTGAbility.cpp:660-705`). |
| 700.8a | PARTIAL | Party is approximated by affinity/count expressions, not a maximum matching assignment engine (`include/MTGDefinitions.h:340`, `src/MTGAbility.cpp:660-705`). |
| 700.8b | PARTIAL | Party is approximated by affinity/count expressions, not a maximum matching assignment engine (`include/MTGDefinitions.h:340`, `src/MTGAbility.cpp:660-705`). |
| 700.8c | PARTIAL | Party is approximated by affinity/count expressions, not a maximum matching assignment engine (`include/MTGDefinitions.h:340`, `src/MTGAbility.cpp:660-705`). |
| 700.8d | PARTIAL | Party is approximated by affinity/count expressions, not a maximum matching assignment engine (`include/MTGDefinitions.h:340`, `src/MTGAbility.cpp:660-705`). |
| 700.9 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'modified permanent' projects/mtg/src projects/mtg/include`. |
| 700.10 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'activated this turn' projects/mtg/src projects/mtg/include`. |
| 700.11 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'descended this turn' projects/mtg/src projects/mtg/include`. |
| 700.12 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'outlaw' projects/mtg/src projects/mtg/include`. |
| 700.12a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'outlaw' projects/mtg/src projects/mtg/include`. |
| 700.13 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'commit crime' projects/mtg/src projects/mtg/include`. |
| 700.14 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'expend' projects/mtg/src projects/mtg/include`. |
| 700.15 | IMPLEMENTED | The engine's battlefield zone is the enter destination and card scripts use moved-to events (`include/AllAbilities.h:221-245`). |
| 701.1 | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.2 | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.2a | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.3 | PARTIAL | Equipment/Aura attachment is tracked through `card->target` and `AEquip`, with legality cleanup in `src/GameObserver.cpp:902-999`; the representation does not cover every attachable object/player or timestamp rule. |
| 701.3a | PARTIAL | Equipment/Aura attachment is tracked through `card->target` and `AEquip`, with legality cleanup in `src/GameObserver.cpp:902-999`; the representation does not cover every attachable object/player or timestamp rule. |
| 701.3b | PARTIAL | Equipment/Aura attachment is tracked through `card->target` and `AEquip`, with legality cleanup in `src/GameObserver.cpp:902-999`; the representation does not cover every attachable object/player or timestamp rule. |
| 701.3c | PARTIAL | Equipment/Aura attachment is tracked through `card->target` and `AEquip`, with legality cleanup in `src/GameObserver.cpp:902-999`; the representation does not cover every attachable object/player or timestamp rule. |
| 701.3d | PARTIAL | Equipment/Aura attachment is tracked through `card->target` and `AEquip`, with legality cleanup in `src/GameObserver.cpp:902-999`; the representation does not cover every attachable object/player or timestamp rule. |
| 701.4 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'behold' projects/mtg/src projects/mtg/include`. |
| 701.4a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'behold' projects/mtg/src projects/mtg/include`. |
| 701.4b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'behold' projects/mtg/src projects/mtg/include`. |
| 701.5 | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.5a | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.5b | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.6 | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.6a | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.6b | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.7 | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.7a | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.7b | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.7c | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.8 | PARTIAL | Core destroy/regeneration paths exist (`src/MTGCardInstance.cpp:574-594`), but zero-toughness and deathtouch expose CR timing/replacement defects described under 704.5f/704.5h. |
| 701.8a | PARTIAL | Core destroy/regeneration paths exist (`src/MTGCardInstance.cpp:574-594`), but zero-toughness and deathtouch expose CR timing/replacement defects described under 704.5f/704.5h. |
| 701.8b | PARTIAL | Core destroy/regeneration paths exist (`src/MTGCardInstance.cpp:574-594`), but zero-toughness and deathtouch expose CR timing/replacement defects described under 704.5f/704.5h. |
| 701.8c | PARTIAL | Core destroy/regeneration paths exist (`src/MTGCardInstance.cpp:574-594`), but zero-toughness and deathtouch expose CR timing/replacement defects described under 704.5f/704.5h. |
| 701.9 | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.9a | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.9b | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.9c | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.10 | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.10a | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.10b | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.10c | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.10d | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.10e | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.10f | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.10g | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.11 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'triple' projects/mtg/src projects/mtg/include`. |
| 701.11a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'triple' projects/mtg/src projects/mtg/include`. |
| 701.11b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'triple' projects/mtg/src projects/mtg/include`. |
| 701.11c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'triple' projects/mtg/src projects/mtg/include`. |
| 701.12 | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.12a | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.12b | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.12c | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.12d | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.12e | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.12f | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.12g | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.12h | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.13 | IMPLEMENTED | The action is represented by the central zone mover (`src/MTGGameZones.cpp:407-440`, `src/MTGGameZones.cpp:454-700`). |
| 701.13a | IMPLEMENTED | The action is represented by the central zone mover (`src/MTGGameZones.cpp:407-440`, `src/MTGGameZones.cpp:454-700`). |
| 701.14 | PARTIAL | Fight has a dedicated damage ability (`src/AllAbilities.cpp:7160-7173`); it uses normal damage events, but unusual legality/self-fight cases remain script-dependent. |
| 701.14a | PARTIAL | Fight has a dedicated damage ability (`src/AllAbilities.cpp:7160-7173`); it uses normal damage events, but unusual legality/self-fight cases remain script-dependent. |
| 701.14b | PARTIAL | Fight has a dedicated damage ability (`src/AllAbilities.cpp:7160-7173`); it uses normal damage events, but unusual legality/self-fight cases remain script-dependent. |
| 701.14c | PARTIAL | Fight has a dedicated damage ability (`src/AllAbilities.cpp:7160-7173`); it uses normal damage events, but unusual legality/self-fight cases remain script-dependent. |
| 701.14d | PARTIAL | Fight has a dedicated damage ability (`src/AllAbilities.cpp:7160-7173`); it uses normal damage events, but unusual legality/self-fight cases remain script-dependent. |
| 701.15 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'goad' projects/mtg/src projects/mtg/include`. |
| 701.15a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'goad' projects/mtg/src projects/mtg/include`. |
| 701.15b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'goad' projects/mtg/src projects/mtg/include`. |
| 701.15c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'goad' projects/mtg/src projects/mtg/include`. |
| 701.15d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'goad' projects/mtg/src projects/mtg/include`. |
| 701.16 | IMPLEMENTED | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.16a | IMPLEMENTED | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.17 | PARTIAL | Library-to-graveyard movement exists, but draw/mill edge semantics are distributed across generic movers (`src/MTGGameZones.cpp:310-375`) rather than a complete keyword-action object. |
| 701.17a | PARTIAL | Library-to-graveyard movement exists, but draw/mill edge semantics are distributed across generic movers (`src/MTGGameZones.cpp:310-375`) rather than a complete keyword-action object. |
| 701.17b | PARTIAL | Library-to-graveyard movement exists, but draw/mill edge semantics are distributed across generic movers (`src/MTGGameZones.cpp:310-375`) rather than a complete keyword-action object. |
| 701.17c | PARTIAL | Library-to-graveyard movement exists, but draw/mill edge semantics are distributed across generic movers (`src/MTGGameZones.cpp:310-375`) rather than a complete keyword-action object. |
| 701.17d | PARTIAL | Library-to-graveyard movement exists, but draw/mill edge semantics are distributed across generic movers (`src/MTGGameZones.cpp:310-375`) rather than a complete keyword-action object. |
| 701.18 | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.18a | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.18b | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.18c | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.18d | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.18e | IMPLEMENTED | The casting/activation stack paths and cost checks implement the core action (`src/MTGRules.cpp:307`, `src/MTGAbility.cpp:7715`). |
| 701.19 | PARTIAL | Regeneration shields are represented by `triggerRegenerate()`/`destroy()` (`src/MTGCardInstance.cpp:503-594`), with known incorrect interaction at toughness 0 under 704.5f. |
| 701.19a | PARTIAL | Regeneration shields are represented by `triggerRegenerate()`/`destroy()` (`src/MTGCardInstance.cpp:503-594`), with known incorrect interaction at toughness 0 under 704.5f. |
| 701.19b | PARTIAL | Regeneration shields are represented by `triggerRegenerate()`/`destroy()` (`src/MTGCardInstance.cpp:503-594`), with known incorrect interaction at toughness 0 under 704.5f. |
| 701.19c | PARTIAL | Regeneration shields are represented by `triggerRegenerate()`/`destroy()` (`src/MTGCardInstance.cpp:503-594`), with known incorrect interaction at toughness 0 under 704.5f. |
| 701.20 | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.20a | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.20b | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.20c | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.20d | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.20e | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.21 | IMPLEMENTED | The action is represented by the central zone mover (`src/MTGGameZones.cpp:407-440`, `src/MTGGameZones.cpp:454-700`). |
| 701.21a | IMPLEMENTED | The action is represented by the central zone mover (`src/MTGGameZones.cpp:407-440`, `src/MTGGameZones.cpp:454-700`). |
| 701.22 | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.22a | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.22b | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.22c | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.22d | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23 | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23a | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23b | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23c | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23d | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23e | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23f | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23g | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23h | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23i | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.23j | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.24 | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.24a | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.24b | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.24c | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.24d | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.24e | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.24f | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.24g | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.25 | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.25a | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.25b | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.25c | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.25d | PARTIAL | Reveal/scry/surveil displays and zone handling exist (`src/MTGAbility.cpp:2412-2428`, `src/MTGAbility.cpp:4738-4784`, `docs/testsuite-fixture-authoring.md:129-166`), but duration, simultaneous choice, and all hidden-zone nuances are incomplete. |
| 701.26 | IMPLEMENTED | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.26a | IMPLEMENTED | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.26b | IMPLEMENTED | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.27 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.27a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.27b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.27c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.27d | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.27e | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.27f | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.27g | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.28 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.28a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.28b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.28c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.28d | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.28e | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.28f | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.29 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'fateseal' projects/mtg/src projects/mtg/include`. |
| 701.29a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'fateseal' projects/mtg/src projects/mtg/include`. |
| 701.30 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'clash' projects/mtg/src projects/mtg/include`. |
| 701.30a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'clash' projects/mtg/src projects/mtg/include`. |
| 701.30b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'clash' projects/mtg/src projects/mtg/include`. |
| 701.30c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'clash' projects/mtg/src projects/mtg/include`. |
| 701.30d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'clash' projects/mtg/src projects/mtg/include`. |
| 701.31 | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.31a | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.31b | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.31c | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.31d | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.32 | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.32a | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.32b | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.32c | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.33 | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.33a | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.33b | N/A | N/A: Planechase/Archenemy-only keyword action, excluded by the audit frame. |
| 701.34 | PARTIAL | A dedicated proliferate parser/action exists (`src/MTGAbility.cpp:5841-5866`), but the script chooser/event model does not establish every CR simultaneous-selection nuance. |
| 701.34a | PARTIAL | A dedicated proliferate parser/action exists (`src/MTGAbility.cpp:5841-5866`), but the script chooser/event model does not establish every CR simultaneous-selection nuance. |
| 701.34b | PARTIAL | A dedicated proliferate parser/action exists (`src/MTGAbility.cpp:5841-5866`), but the script chooser/event model does not establish every CR simultaneous-selection nuance. |
| 701.35 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'detain' projects/mtg/src projects/mtg/include`. |
| 701.35a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'detain' projects/mtg/src projects/mtg/include`. |
| 701.36 | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.36a | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.36b | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.37 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'monstrosity' projects/mtg/src projects/mtg/include`. |
| 701.37a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'monstrosity' projects/mtg/src projects/mtg/include`. |
| 701.37b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'monstrosity' projects/mtg/src projects/mtg/include`. |
| 701.37c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'monstrosity' projects/mtg/src projects/mtg/include`. |
| 701.38 | N/A | N/A: voting requires multiplayer, excluded by the audit frame. |
| 701.38a | N/A | N/A: voting requires multiplayer, excluded by the audit frame. |
| 701.38b | N/A | N/A: voting requires multiplayer, excluded by the audit frame. |
| 701.38c | N/A | N/A: voting requires multiplayer, excluded by the audit frame. |
| 701.38d | N/A | N/A: voting requires multiplayer, excluded by the audit frame. |
| 701.39 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'bolster' projects/mtg/src projects/mtg/include`. |
| 701.39a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'bolster' projects/mtg/src projects/mtg/include`. |
| 701.40 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.40a | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.40b | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.40c | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.40d | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.40e | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.40f | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.40g | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.40h | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.41 | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.41a | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.42 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.42a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.42b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.42c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 701.43 | PARTIAL | Exert status and restrictions exist (`src/MTGAbility.cpp:732-746`, `src/MTGAbility.cpp:4252-4260`), but all attack-declaration and untap semantics remain distributed. |
| 701.43a | PARTIAL | Exert status and restrictions exist (`src/MTGAbility.cpp:732-746`, `src/MTGAbility.cpp:4252-4260`), but all attack-declaration and untap semantics remain distributed. |
| 701.43b | PARTIAL | Exert status and restrictions exist (`src/MTGAbility.cpp:732-746`, `src/MTGAbility.cpp:4252-4260`), but all attack-declaration and untap semantics remain distributed. |
| 701.43c | PARTIAL | Exert status and restrictions exist (`src/MTGAbility.cpp:732-746`, `src/MTGAbility.cpp:4252-4260`), but all attack-declaration and untap semantics remain distributed. |
| 701.43d | PARTIAL | Exert status and restrictions exist (`src/MTGAbility.cpp:732-746`, `src/MTGAbility.cpp:4252-4260`), but all attack-declaration and untap semantics remain distributed. |
| 701.44 | PARTIAL | Explore emits a dedicated event (`src/AllAbilities.cpp:1532-1564`, `src/MTGAbility.cpp:4748-4755`), while reveal/choice/counter sequencing is generic and incomplete. |
| 701.44a | PARTIAL | Explore emits a dedicated event (`src/AllAbilities.cpp:1532-1564`, `src/MTGAbility.cpp:4748-4755`), while reveal/choice/counter sequencing is generic and incomplete. |
| 701.44b | PARTIAL | Explore emits a dedicated event (`src/AllAbilities.cpp:1532-1564`, `src/MTGAbility.cpp:4748-4755`), while reveal/choice/counter sequencing is generic and incomplete. |
| 701.44c | PARTIAL | Explore emits a dedicated event (`src/AllAbilities.cpp:1532-1564`, `src/MTGAbility.cpp:4748-4755`), while reveal/choice/counter sequencing is generic and incomplete. |
| 701.44d | PARTIAL | Explore emits a dedicated event (`src/AllAbilities.cpp:1532-1564`, `src/MTGAbility.cpp:4748-4755`), while reveal/choice/counter sequencing is generic and incomplete. |
| 701.45 | N/A | N/A: Contraption casual-variant machinery is outside supported classic duel play. |
| 701.45a | N/A | N/A: Contraption casual-variant machinery is outside supported classic duel play. |
| 701.46 | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.46a | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.47 | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.47a | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.47b | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.47c | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.47d | PARTIAL | Token creation and token-created events exist (`include/AllAbilities.h:1293-1318`, `src/MTGRules.cpp:3659-3683`), but replacement ordering and nonbattlefield token semantics are not fully modeled. |
| 701.48 | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.48a | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.49 | PARTIAL | Dungeon counters/events are represented (`include/MTGDefinitions.h:313-319`, `src/MTGAbility.cpp:2920-2940`), but a complete dungeon/room engine is absent. |
| 701.49a | PARTIAL | Dungeon counters/events are represented (`include/MTGDefinitions.h:313-319`, `src/MTGAbility.cpp:2920-2940`), but a complete dungeon/room engine is absent. |
| 701.49b | PARTIAL | Dungeon counters/events are represented (`include/MTGDefinitions.h:313-319`, `src/MTGAbility.cpp:2920-2940`), but a complete dungeon/room engine is absent. |
| 701.49c | PARTIAL | Dungeon counters/events are represented (`include/MTGDefinitions.h:313-319`, `src/MTGAbility.cpp:2920-2940`), but a complete dungeon/room engine is absent. |
| 701.49d | PARTIAL | Dungeon counters/events are represented (`include/MTGDefinitions.h:313-319`, `src/MTGAbility.cpp:2920-2940`), but a complete dungeon/room engine is absent. |
| 701.50 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'connive' projects/mtg/src projects/mtg/include`. |
| 701.50a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'connive' projects/mtg/src projects/mtg/include`. |
| 701.50b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'connive' projects/mtg/src projects/mtg/include`. |
| 701.50c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'connive' projects/mtg/src projects/mtg/include`. |
| 701.50d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'connive' projects/mtg/src projects/mtg/include`. |
| 701.50e | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'connive' projects/mtg/src projects/mtg/include`. |
| 701.51 | N/A | N/A: Attraction/Unfinity-only action, excluded by the audit frame. |
| 701.51a | N/A | N/A: Attraction/Unfinity-only action, excluded by the audit frame. |
| 701.51b | N/A | N/A: Attraction/Unfinity-only action, excluded by the audit frame. |
| 701.51c | N/A | N/A: Attraction/Unfinity-only action, excluded by the audit frame. |
| 701.52 | N/A | N/A: Attraction/Unfinity-only action, excluded by the audit frame. |
| 701.52a | N/A | N/A: Attraction/Unfinity-only action, excluded by the audit frame. |
| 701.53 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'incubate' projects/mtg/src projects/mtg/include`. |
| 701.53a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'incubate' projects/mtg/src projects/mtg/include`. |
| 701.53b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'incubate' projects/mtg/src projects/mtg/include`. |
| 701.54 | PARTIAL | Ring temptation counters/events exist (`src/MTGAbility.cpp:4653-4666`, `src/MTGAbility.cpp:1544-1550`), but emblem progression and Ring-bearer requirements are script-driven and incomplete. |
| 701.54a | PARTIAL | Ring temptation counters/events exist (`src/MTGAbility.cpp:4653-4666`, `src/MTGAbility.cpp:1544-1550`), but emblem progression and Ring-bearer requirements are script-driven and incomplete. |
| 701.54b | PARTIAL | Ring temptation counters/events exist (`src/MTGAbility.cpp:4653-4666`, `src/MTGAbility.cpp:1544-1550`), but emblem progression and Ring-bearer requirements are script-driven and incomplete. |
| 701.54c | PARTIAL | Ring temptation counters/events exist (`src/MTGAbility.cpp:4653-4666`, `src/MTGAbility.cpp:1544-1550`), but emblem progression and Ring-bearer requirements are script-driven and incomplete. |
| 701.54d | PARTIAL | Ring temptation counters/events exist (`src/MTGAbility.cpp:4653-4666`, `src/MTGAbility.cpp:1544-1550`), but emblem progression and Ring-bearer requirements are script-driven and incomplete. |
| 701.54e | PARTIAL | Ring temptation counters/events exist (`src/MTGAbility.cpp:4653-4666`, `src/MTGAbility.cpp:1544-1550`), but emblem progression and Ring-bearer requirements are script-driven and incomplete. |
| 701.55 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'villainous choice' projects/mtg/src projects/mtg/include`. |
| 701.55a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'villainous choice' projects/mtg/src projects/mtg/include`. |
| 701.55b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'villainous choice' projects/mtg/src projects/mtg/include`. |
| 701.55c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'villainous choice' projects/mtg/src projects/mtg/include`. |
| 701.55d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'villainous choice' projects/mtg/src projects/mtg/include`. |
| 701.56 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'time travel' projects/mtg/src projects/mtg/include`. |
| 701.56a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'time travel' projects/mtg/src projects/mtg/include`. |
| 701.57 | PARTIAL | Discover is present only through card-script/generic reveal-cast composition; named source search found no complete `discover` action class (`rg -i discover src include`). |
| 701.57a | PARTIAL | Discover is present only through card-script/generic reveal-cast composition; named source search found no complete `discover` action class (`rg -i discover src include`). |
| 701.57b | PARTIAL | Discover is present only through card-script/generic reveal-cast composition; named source search found no complete `discover` action class (`rg -i discover src include`). |
| 701.57c | PARTIAL | Discover is present only through card-script/generic reveal-cast composition; named source search found no complete `discover` action class (`rg -i discover src include`). |
| 701.58 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.58a | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.58b | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.58c | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.58d | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.58e | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.58f | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.58g | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.58h | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.59 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'collect evidence' projects/mtg/src projects/mtg/include`. |
| 701.59a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'collect evidence' projects/mtg/src projects/mtg/include`. |
| 701.59b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'collect evidence' projects/mtg/src projects/mtg/include`. |
| 701.59c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'collect evidence' projects/mtg/src projects/mtg/include`. |
| 701.60 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'suspect' projects/mtg/src projects/mtg/include`. |
| 701.60a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'suspect' projects/mtg/src projects/mtg/include`. |
| 701.60b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'suspect' projects/mtg/src projects/mtg/include`. |
| 701.60c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'suspect' projects/mtg/src projects/mtg/include`. |
| 701.60d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'suspect' projects/mtg/src projects/mtg/include`. |
| 701.61 | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.61a | PARTIAL | Core zone/action machinery exists, but the script parser models card-specific approximations rather than every CR atomicity, choice, replacement, and event nuance (`src/MTGAbility.cpp:2308`, `src/AllAbilities.cpp:3160`). |
| 701.62 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.62a | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.62b | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 701.63 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'endure' projects/mtg/src projects/mtg/include`. |
| 701.63a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'endure' projects/mtg/src projects/mtg/include`. |
| 701.63b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'endure' projects/mtg/src projects/mtg/include`. |
| 701.64 | PARTIAL | Harness-related energy primitives exist, but no complete keyword action class was found (`rg -i harness src include` returned only generic/card-state references). |
| 701.64a | PARTIAL | Harness-related energy primitives exist, but no complete keyword action class was found (`rg -i harness src include` returned only generic/card-state references). |
| 701.64b | PARTIAL | Harness-related energy primitives exist, but no complete keyword action class was found (`rg -i harness src include` returned only generic/card-state references). |
| 701.65 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'airbend' projects/mtg/src projects/mtg/include`. |
| 701.65a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'airbend' projects/mtg/src projects/mtg/include`. |
| 701.65b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'airbend' projects/mtg/src projects/mtg/include`. |
| 701.66 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'earthbend' projects/mtg/src projects/mtg/include`. |
| 701.66a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'earthbend' projects/mtg/src projects/mtg/include`. |
| 701.66b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'earthbend' projects/mtg/src projects/mtg/include`. |
| 701.67 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'waterbend' projects/mtg/src projects/mtg/include`. |
| 701.67a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'waterbend' projects/mtg/src projects/mtg/include`. |
| 701.67b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'waterbend' projects/mtg/src projects/mtg/include`. |
| 701.67c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'waterbend' projects/mtg/src projects/mtg/include`. |
| 701.68 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'blight' projects/mtg/src projects/mtg/include`. |
| 701.68a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'blight' projects/mtg/src projects/mtg/include`. |
| 701.68b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'blight' projects/mtg/src projects/mtg/include`. |
| 701.68c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'blight' projects/mtg/src projects/mtg/include`. |
| 701.68d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'blight' projects/mtg/src projects/mtg/include`. |
| 703.1 | PARTIAL | The phase engine performs common automatic actions before interaction, but lacks a unified ordered turn-action scheduler for every listed action (`src/GameObserver.cpp:200-290`). |
| 703.1a | PARTIAL | The phase engine performs common automatic actions before interaction, but lacks a unified ordered turn-action scheduler for every listed action (`src/GameObserver.cpp:200-290`). |
| 703.2 | PARTIAL | The phase engine performs common automatic actions before interaction, but lacks a unified ordered turn-action scheduler for every listed action (`src/GameObserver.cpp:200-290`). |
| 703.3 | PARTIAL | The phase engine performs common automatic actions before interaction, but lacks a unified ordered turn-action scheduler for every listed action (`src/GameObserver.cpp:200-290`). |
| 703.4 | PARTIAL | The phase engine performs common automatic actions before interaction, but lacks a unified ordered turn-action scheduler for every listed action (`src/GameObserver.cpp:200-290`). |
| 703.4a | IMPLEMENTED | Untap begins with synchronous phasing, then `untapAll()` (`src/GameObserver.cpp:268-282`, `src/GameObserver.cpp:1919-1960`). |
| 703.4b | PARTIAL | Day/night is approximated by real `Day`/`Night` battlefield marker cards; regression fixtures explicitly document this (`bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt:6-14`, `bin/Res/test/lexicon/basic_nightbound_flips_after_two_spells.txt:9-17`). |
| 703.4c | IMPLEMENTED | Untap begins with synchronous phasing, then `untapAll()` (`src/GameObserver.cpp:268-282`, `src/GameObserver.cpp:1919-1960`). |
| 703.4d | IMPLEMENTED | The phase engine performs this automatic action outside the spell stack (`src/GameObserver.cpp:200-290`, `src/GameObserver.cpp:1910-1960`; combat assignment/dealing is in `src/GuiCombat.cpp:90-390`). |
| 703.4e | N/A | N/A: Archenemy only. |
| 703.4f | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'lore counter\\|saga' projects/mtg/src projects/mtg/include`. No automatic per-Saga lore-counter turn action exists. |
| 703.4g | N/A | N/A: Attractions only. |
| 703.4h | N/A | N/A: multiplayer defending-player selection only. |
| 703.4i | IMPLEMENTED | The phase engine performs this automatic action outside the spell stack (`src/GameObserver.cpp:200-290`, `src/GameObserver.cpp:1910-1960`; combat assignment/dealing is in `src/GuiCombat.cpp:90-390`). |
| 703.4j | IMPLEMENTED | The phase engine performs this automatic action outside the spell stack (`src/GameObserver.cpp:200-290`, `src/GameObserver.cpp:1910-1960`; combat assignment/dealing is in `src/GuiCombat.cpp:90-390`). |
| 703.4k | IMPLEMENTED | The phase engine performs this automatic action outside the spell stack (`src/GameObserver.cpp:200-290`, `src/GameObserver.cpp:1910-1960`; combat assignment/dealing is in `src/GuiCombat.cpp:90-390`). |
| 703.4m | IMPLEMENTED | The phase engine performs this automatic action outside the spell stack (`src/GameObserver.cpp:200-290`, `src/GameObserver.cpp:1910-1960`; combat assignment/dealing is in `src/GuiCombat.cpp:90-390`). |
| 703.4n | IMPLEMENTED | The phase engine performs this automatic action outside the spell stack (`src/GameObserver.cpp:200-290`, `src/GameObserver.cpp:1910-1960`; combat assignment/dealing is in `src/GuiCombat.cpp:90-390`). |
| 703.4p | IMPLEMENTED | The phase engine performs this automatic action outside the spell stack (`src/GameObserver.cpp:200-290`, `src/GameObserver.cpp:1910-1960`; combat assignment/dealing is in `src/GuiCombat.cpp:90-390`). |
| 703.4q | IMPLEMENTED | The phase engine performs this automatic action outside the spell stack (`src/GameObserver.cpp:200-290`, `src/GameObserver.cpp:1910-1960`; combat assignment/dealing is in `src/GuiCombat.cpp:90-390`). |
| 704.1 | PARTIAL | A per-update `gameStateBasedEffects()` pass exists (`src/GameObserver.cpp:716-1070`), but several listed actions are event-driven/immediate or absent. |
| 704.1a | IMPLEMENTED | Triggered abilities are separate event observers/stack objects, not handled as SBAs (`include/AllAbilities.h:179-260`, `src/GameObserver.cpp:1962-1990`). |
| 704.2 | PARTIAL | The central pass is automatic, but the legend implementation opens an action-layer menu (`src/MTGRules.cpp:3776-3798`) and the full SBA set is incomplete. |
| 704.3 | INCORRECT | The pass mutates players/cards sequentially and returns early for displays, stack, menus, and choosers (`src/GameObserver.cpp:821-831`, `src/GameObserver.cpp:833-1060`); it has no simultaneous collect/apply/repeat-to-fixed-point algorithm. |
| 704.4 | INCORRECT | Deathtouch destroys its target synchronously from the damage event (`src/MTGRules.cpp:4188-4214`), so an SBA can effectively occur during resolution. |
| 704.5 | PARTIAL | Only a subset of the enumerated actions is implemented; see 704.5a-z findings below. |
| 704.5a | IMPLEMENTED | `DeadLifeState(true)` loses a player at life <= 0, respecting can't-lose/can't-win effects (`src/Player.cpp:334-379`, `src/GameObserver.cpp:810-819`). |
| 704.5b | INCORRECT | An empty-library draw calls `setLoser` immediately inside `drawFromLibrary()` rather than recording the attempt for the next SBA check (`src/MTGGameZones.cpp:322-336`). |
| 704.5c | IMPLEMENTED | `DeadLifeState` loses a player at ten poison counters, with can't-lose handling (`src/Player.cpp:334-379`). |
| 704.5d | INCORRECT | `MTGTokensCleanup` moves a token to garbage synchronously on the zone-change event, rather than at the next SBA check (`src/MTGRules.cpp:3659-3683`). |
| 704.5e | UNIMPLEMENTED | No general spell-copy/card-copy SBA or non-stack copy object lifecycle was found; token cleanup only handles `isToken` (`src/MTGRules.cpp:3659-3683`). |
| 704.5f | INCORRECT | For ordinary creatures with toughness <= 0, the pass calls `destroy()`, which can regenerate (`src/GameObserver.cpp:1020-1027`, `src/MTGCardInstance.cpp:585-594`); only indestructible creatures are force-moved. CR forbids regeneration. |
| 704.5g | IMPLEMENTED | Damage reduces creature life and `afterDamage()`/the state pass calls `destroy()`, permitting regeneration and respecting indestructible (`src/MTGCardInstance.cpp:480-494`, `src/GameObserver.cpp:1020-1027`). |
| 704.5h | INCORRECT | `MTGDeathtouchRule::receiveEvent` calls `destroy()` immediately for each positive damage event (`src/MTGRules.cpp:4188-4214`), rather than marking damage and destroying at the next simultaneous SBA check. |
| 704.5i | PARTIAL | Zero loyalty is detected and moved to graveyard (`src/GameObserver.cpp:843-855`, `src/MTGRules.cpp:4118-4128`), but counter removal can move it immediately, not via the fixed-point SBA pass. |
| 704.5j | INCORRECT | The legend event opens a chooser that moves only one matching permanent (`src/MTGRules.cpp:3761-3798`). With 3+ duplicates it does not atomically choose one to keep and move all others; it is event-driven, not a simultaneous SBA. |
| 704.5k | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'world rule\\|TYPE_WORLD' projects/mtg/src projects/mtg/include`. `TYPE_WORLD` is declared, but no world-rule resolver exists. |
| 704.5m | INCORRECT | Aura cleanup is guarded by `card->target`; an unattached Aura with no target bypasses the checks (`src/GameObserver.cpp:931-999`). Legality is also approximated by string type matching. |
| 704.5n | PARTIAL | `AEquip::unequip()` is invoked for several illegal Equipment/Fortification cases (`src/GameObserver.cpp:902-929`), but all illegal permanent/player attachments are not represented. |
| 704.5p | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'battle or creature is attached' projects/mtg/src projects/mtg/include`. The attachment model special-cases Aura/Equipment/Fortification. |
| 704.5q | PARTIAL | Opposing P/T counters cancel only when `AACounter` explicitly calls `cancelCounter()` (`src/AllAbilities.cpp:3248-3262`, `src/Counters.cpp:41-57`), not as a general simultaneous SBA over all counter placements. |
| 704.5r | INCORRECT | `AACounter` prevents additions above `maxNb` (`src/AllAbilities.cpp:3210-3227`) instead of allowing the event and removing excess counters as an SBA; no static 'can't have more than N' resolver exists. |
| 704.5s | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'lore\\|final chapter\\|saga' projects/mtg/src projects/mtg/include`. Saga behavior is card-scripted; there is no final-chapter stack-aware sacrifice SBA. |
| 704.5t | PARTIAL | Dungeon completion flags/counters exist (`include/MTGDefinitions.h:313-319`), but no general bottom-room, pending-trigger-aware dungeon removal SBA was found. |
| 704.5u | N/A | N/A: space sculptor/sector designations are Unfinity-only, excluded by the audit frame. |
| 704.5v | INCORRECT | At defense 0 the engine emits `WEventCardDefeated` and sets `isDefeated` but does not itself put the battle in its owner's graveyard (`src/GameObserver.cpp:846-852`, `src/MTGRules.cpp:4129-4139`). |
| 704.5w | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'protector' projects/mtg/src projects/mtg/include`. Battle protector selection is not modeled as an SBA. |
| 704.5x | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'protector' projects/mtg/src projects/mtg/include`. Siege controller/protector correction is not modeled. |
| 704.5y | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'Role' projects/mtg/src projects/mtg/include`. No timestamp-based Role attachment pruning exists. |
| 704.5z | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'start your engines\\|speed' projects/mtg/src projects/mtg/include`. No player speed designation/SBA exists. |
| 704.6 | N/A | N/A: umbrella for variant-only SBAs, excluded by the audit frame. |
| 704.6a | N/A | N/A: Two-Headed Giant only. |
| 704.6b | N/A | N/A: Two-Headed Giant only. |
| 704.6c | N/A | N/A: Commander only. |
| 704.6d | N/A | N/A: Commander only. |
| 704.6e | N/A | N/A: Archenemy only. |
| 704.6f | N/A | N/A: Planechase only. |
| 704.7 | INCORRECT | Because SBAs are applied sequentially/event-by-event rather than collected as one event (`src/GameObserver.cpp:833-1060`), one replacement effect is not guaranteed to replace all identical simultaneous results. |
| 704.8 | INCORRECT | The sequential mutation loop snapshots LKI per card immediately before processing it (`src/GameObserver.cpp:835-842`), not from one pre-SBA game state for all simultaneously leaving permanents. |
| 705.1 | PARTIAL | Coin-flip parsing, random result, and trigger events exist (`src/MTGAbility.cpp:1671-1690`, `src/MTGAbility.cpp:3472-3486`), but declared-result/replacement control and all win/lose semantics are not first-class. |
| 705.2 | PARTIAL | Coin-flip parsing, random result, and trigger events exist (`src/MTGAbility.cpp:1671-1690`, `src/MTGAbility.cpp:3472-3486`), but declared-result/replacement control and all win/lose semantics are not first-class. |
| 705.3 | PARTIAL | Coin-flip parsing, random result, and trigger events exist (`src/MTGAbility.cpp:1671-1690`, `src/MTGAbility.cpp:3472-3486`), but declared-result/replacement control and all win/lose semantics are not first-class. |
| 706.1 | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.1a | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.1b | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.2 | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.2a | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.2b | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.3 | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.3a | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.3b | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.3c | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.4 | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.5 | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.6 | PARTIAL | Digital d4/d6/d8/d10/d12/d20 rolls and roll triggers exist (`src/MTGAbility.cpp:1649-1668`, `src/MTGAbility.cpp:3487-3598`), but modifiers, reroll ordering, ignored rolls, and generalized result tables are incomplete. |
| 706.7 | N/A | N/A: Planechase-only planar die clause. |
| 706.8 | N/A | N/A: Centaur of Attention is an Unfinity/physical-play card outside the supported game types. |
| 706.8a | N/A | N/A: Centaur of Attention is an Unfinity/physical-play card outside the supported game types. |
| 706.8b | N/A | N/A: Centaur of Attention is an Unfinity/physical-play card outside the supported game types. |
| 706.8c | N/A | N/A: Centaur of Attention is an Unfinity/physical-play card outside the supported game types. |
| 707.1 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.2 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.2a | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.2b | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.2c | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.3 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.4 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.5 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.6 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.7 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.8 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.8a | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.9 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.9a | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.9b | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.9c | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.9d | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.9e | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.9f | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.9g | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.10 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.10a | UNIMPLEMENTED | No spell/card-copy SBA exists; `MTGTokensCleanup` only recognizes `isToken` (`src/MTGRules.cpp:3659-3683`). |
| 707.10b | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.10c | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.10d | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.10e | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.10f | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.10g | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.11 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.12 | PARTIAL | Permanent-copy/token machinery exists (`include/AllAbilities.h:2176-2200`, `src/MTGCardInstance.cpp:102-166`), but it does not implement the full copiable-value layers or general spell/ability copy semantics. |
| 707.13 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'Garth One-Eye' projects/mtg/src projects/mtg/include`. No Oracle-defined outside-game or noted-name copy constructor exists. |
| 707.14 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'Magar' projects/mtg/src projects/mtg/include`. No Oracle-defined outside-game or noted-name copy constructor exists. |
| 708.1 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.2 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.2a | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.2b | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.3 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.4 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.5 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.6 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.7 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.8 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.9 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.10 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.11 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 708.12 | PARTIAL | Morph/manifest face-down state exists (`include/MTGCardInstance.h:100-120`, `src/AllAbilities.cpp:5264-5356`), but it is not a general implementation of all face-down characteristics and reveal obligations. |
| 709.1 | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.2 | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.3 | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.3a | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.3b | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.3c | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.4 | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.4a | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.4b | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.4c | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.4d | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5 | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5a | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5b | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5c | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5d | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5e | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5f | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5g | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5h | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5i | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 709.5j | PARTIAL | Split-card face/name/cost support exists in deck/card parsing and casting, but every zone/stack characteristic and fused-copy rule is not centralized (`src/MTGDeck.cpp:170-215`, `src/MTGCardInstance.cpp:102-166`). |
| 710.1 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 710.1a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 710.1b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 710.1c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 710.2 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 710.3 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 710.4 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 710.5 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 711.1 | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 711.2 | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 711.2a | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 711.2b | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 711.3 | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 711.4 | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 711.5 | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 711.6 | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 711.7 | PARTIAL | Leveler state is tracked and abilities can set it (`include/MTGCardInstance.h:94`, `src/AllAbilities.cpp:7515-7532`), but level-symbol characteristics are script transformations rather than a complete rules subsystem. |
| 712.1 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.2 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.2a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.2b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.2c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.3 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.3a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.3b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.3c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.4 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.4a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.4b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.4c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.5 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.5a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.5b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.5c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.5d | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.5e | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.5f | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.5g | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.6 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.7 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.8 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.8a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.8b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.8c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.8d | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.8e | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.8f | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.8g | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.9 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.10 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.11 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.11a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.11b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.11c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.11d | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.12 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.13 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.13a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.14 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.14a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.14b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.14c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.15 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.15a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.16 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.17 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.18 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.19 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.20 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.21 | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.21a | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.21b | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.21c | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.21d | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 712.21e | PARTIAL | Transform/flip/meld machinery exists (`src/AllAbilities.cpp:5361-5429`, `src/AllAbilities.cpp:5556-5805`), but face-specific copiable values, zone identities, and all DFC restrictions are only partially represented. |
| 713.1 | N/A | N/A: substitute/checklist/helper card handling is a physical deck-presentation procedure; the digital engine stores actual card objects. |
| 713.2 | N/A | N/A: substitute/checklist/helper card handling is a physical deck-presentation procedure; the digital engine stores actual card objects. |
| 713.2a | N/A | N/A: substitute/checklist/helper card handling is a physical deck-presentation procedure; the digital engine stores actual card objects. |
| 713.2b | N/A | N/A: substitute/checklist/helper card handling is a physical deck-presentation procedure; the digital engine stores actual card objects. |
| 713.2c | N/A | N/A: substitute/checklist/helper card handling is a physical deck-presentation procedure; the digital engine stores actual card objects. |
| 713.3 | N/A | N/A: substitute/checklist/helper card handling is a physical deck-presentation procedure; the digital engine stores actual card objects. |
| 713.4 | N/A | N/A: substitute/checklist/helper card handling is a physical deck-presentation procedure; the digital engine stores actual card objects. |
| 713.5 | N/A | N/A: substitute/checklist/helper card handling is a physical deck-presentation procedure; the digital engine stores actual card objects. |
| 714.1 | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.1a | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.2 | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.2a | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.2b | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.2c | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.2d | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.2e | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.3 | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.3a | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.3b | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 714.4 | PARTIAL | Saga chapters and lore changes are encoded per card, but automatic lore turn action and final-chapter SBA are absent (named source search `rg -i 'lore\\|saga\\|chapter' src include`; see 703.4f and 704.5s). |
| 715.1 | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.2 | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.2a | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.2b | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.2c | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.3 | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.3a | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.3b | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.3c | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.3d | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.4 | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 715.5 | PARTIAL | Adventure alternate casting is recognized (`include/MTGDefinitions.h:288`, `src/AIPlayerGPT.cpp:2956-2993`), but the full two-part characteristic and exile/cast permissions remain card-data dependent. |
| 716.1 | PARTIAL | Class levels are encoded as activated/scripted transformations; no generic Class level subsystem was found by `rg -i 'class level' src include`. |
| 716.2 | PARTIAL | Class levels are encoded as activated/scripted transformations; no generic Class level subsystem was found by `rg -i 'class level' src include`. |
| 716.2a | PARTIAL | Class levels are encoded as activated/scripted transformations; no generic Class level subsystem was found by `rg -i 'class level' src include`. |
| 716.2b | PARTIAL | Class levels are encoded as activated/scripted transformations; no generic Class level subsystem was found by `rg -i 'class level' src include`. |
| 716.2c | PARTIAL | Class levels are encoded as activated/scripted transformations; no generic Class level subsystem was found by `rg -i 'class level' src include`. |
| 716.2d | PARTIAL | Class levels are encoded as activated/scripted transformations; no generic Class level subsystem was found by `rg -i 'class level' src include`. |
| 716.3 | PARTIAL | Class levels are encoded as activated/scripted transformations; no generic Class level subsystem was found by `rg -i 'class level' src include`. |
| 716.4 | PARTIAL | Class levels are encoded as activated/scripted transformations; no generic Class level subsystem was found by `rg -i 'class level' src include`. |
| 717.1 | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 717.2 | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 717.2a | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 717.2b | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 717.3 | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 717.4 | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 717.5 | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 717.6 | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 717.6a | N/A | N/A: Attraction/Unfinity rules are excluded by the audit frame. |
| 718.1 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.2 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.2a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.3 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.3a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.3b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.3c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.3d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.4 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 718.5 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'prototype' projects/mtg/src projects/mtg/include`. |
| 719.1 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'case solved\\|case card' projects/mtg/src projects/mtg/include`. |
| 719.2 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'case solved\\|case card' projects/mtg/src projects/mtg/include`. |
| 719.3 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'case solved\\|case card' projects/mtg/src projects/mtg/include`. |
| 719.3a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'case solved\\|case card' projects/mtg/src projects/mtg/include`. |
| 719.3b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'case solved\\|case card' projects/mtg/src projects/mtg/include`. |
| 719.3c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'case solved\\|case card' projects/mtg/src projects/mtg/include`. |
| 720.1 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.2 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.2a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.2b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.2c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.3 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.3a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.3b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.3c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.3d | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.4 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 720.5 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'omen card\\|omen spell' projects/mtg/src projects/mtg/include`. |
| 721.1 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'station card\\|charge station' projects/mtg/src projects/mtg/include`. |
| 721.2 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'station card\\|charge station' projects/mtg/src projects/mtg/include`. |
| 721.2a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'station card\\|charge station' projects/mtg/src projects/mtg/include`. |
| 721.2b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'station card\\|charge station' projects/mtg/src projects/mtg/include`. |
| 721.2c | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'station card\\|charge station' projects/mtg/src projects/mtg/include`. |
| 721.3 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'station card\\|charge station' projects/mtg/src projects/mtg/include`. |
| 721.4 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'station card\\|charge station' projects/mtg/src projects/mtg/include`. |
| 722.1 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.1a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.1b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.2 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.3 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.4 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.5 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.5a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.5b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.6 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.7 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.8 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 722.9 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'control another player' projects/mtg/src projects/mtg/include`. The engine has no decision redirection layer for another player's choices/private information. |
| 723.1 | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.1a | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.1b | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.1c | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.1d | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.1e | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.1f | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.2 | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.2a | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.2b | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.2c | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.2d | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.2e | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.2f | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 723.2g | UNIMPLEMENTED | Common phase progression exists (`src/GameObserver.cpp:1919-1960` and phase code), but no CR 723 stack-exiling/end-turn procedure was found; `APhaseAlter` only adds/skips phases (`src/MTGAbility.cpp:5203`). |
| 724.1 | PARTIAL | The engine stores a single two-player monarch flag and emits an event (`src/AllAbilities.cpp:1854-1887`); inherent draw/combat-damage rules are card-script/rule approximations rather than a complete designation subsystem. |
| 724.2 | PARTIAL | The engine stores a single two-player monarch flag and emits an event (`src/AllAbilities.cpp:1854-1887`); inherent draw/combat-damage rules are card-script/rule approximations rather than a complete designation subsystem. |
| 724.3 | PARTIAL | The engine stores a single two-player monarch flag and emits an event (`src/AllAbilities.cpp:1854-1887`); inherent draw/combat-damage rules are card-script/rule approximations rather than a complete designation subsystem. |
| 724.4 | PARTIAL | The engine stores a single two-player monarch flag and emits an event (`src/AllAbilities.cpp:1854-1887`); inherent draw/combat-damage rules are card-script/rule approximations rather than a complete designation subsystem. |
| 724.5 | PARTIAL | The engine stores a single two-player monarch flag and emits an event (`src/AllAbilities.cpp:1854-1887`); inherent draw/combat-damage rules are card-script/rule approximations rather than a complete designation subsystem. |
| 725.1 | PARTIAL | The engine stores a single two-player initiative flag and emits an event (`src/AllAbilities.cpp:1894-1925`), but Undercity and all inherent-trigger semantics are not fully centralized. |
| 725.2 | PARTIAL | The engine stores a single two-player initiative flag and emits an event (`src/AllAbilities.cpp:1894-1925`), but Undercity and all inherent-trigger semantics are not fully centralized. |
| 725.3 | PARTIAL | The engine stores a single two-player initiative flag and emits an event (`src/AllAbilities.cpp:1894-1925`), but Undercity and all inherent-trigger semantics are not fully centralized. |
| 725.4 | PARTIAL | The engine stores a single two-player initiative flag and emits an event (`src/AllAbilities.cpp:1894-1925`), but Undercity and all inherent-trigger semantics are not fully centralized. |
| 725.5 | PARTIAL | The engine stores a single two-player initiative flag and emits an event (`src/AllAbilities.cpp:1894-1925`), but Undercity and all inherent-trigger semantics are not fully centralized. |
| 726.1 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'restart the game\\|Karn Liberated' projects/mtg/src projects/mtg/include`. No game-state reconstruction preserving exempt objects exists. |
| 726.1a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'restart the game\\|Karn Liberated' projects/mtg/src projects/mtg/include`. No game-state reconstruction preserving exempt objects exists. |
| 726.2 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'restart the game\\|Karn Liberated' projects/mtg/src projects/mtg/include`. No game-state reconstruction preserving exempt objects exists. |
| 726.3 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'restart the game\\|Karn Liberated' projects/mtg/src projects/mtg/include`. No game-state reconstruction preserving exempt objects exists. |
| 726.4 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'restart the game\\|Karn Liberated' projects/mtg/src projects/mtg/include`. No game-state reconstruction preserving exempt objects exists. |
| 726.5 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'restart the game\\|Karn Liberated' projects/mtg/src projects/mtg/include`. No game-state reconstruction preserving exempt objects exists. |
| 726.5a | N/A | N/A: Commander-only restart exception. |
| 726.6 | N/A | N/A: subgame-only interaction; subgames are unsupported. |
| 726.7 | N/A | N/A: multiplayer limited-range interaction. |
| 727.1 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'rad counter\\|radiation' projects/mtg/src projects/mtg/include`. |
| 727.1a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'rad counter\\|radiation' projects/mtg/src projects/mtg/include`. |
| 728.1 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.1a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.1b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.2 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.2a | N/A | N/A: supplementary-deck/Vanguard/Commander subgame clause, excluded by the audit frame. |
| 728.2b | N/A | N/A: supplementary-deck/Vanguard/Commander subgame clause, excluded by the audit frame. |
| 728.2c | N/A | N/A: supplementary-deck/Vanguard/Commander subgame clause, excluded by the audit frame. |
| 728.3 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.4 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.4a | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.4b | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.5 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 728.5a | N/A | N/A: supplementary-deck/Vanguard/Commander subgame clause, excluded by the audit frame. |
| 728.5b | N/A | N/A: supplementary-deck/Vanguard/Commander subgame clause, excluded by the audit frame. |
| 728.5c | N/A | N/A: supplementary-deck/Vanguard/Commander subgame clause, excluded by the audit frame. |
| 728.6 | UNIMPLEMENTED | No engine implementation found by named search `rg -i 'subgame\\|Shahrazad' projects/mtg/src projects/mtg/include`. No nested game/zones lifecycle exists. |
| 729.1 | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2 | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2a | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2b | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2c | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2d | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2e | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2f | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2g | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2h | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2i | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.2j | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.3 | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.3a | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.3b | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.3c | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.3d | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 729.3e | PARTIAL | Mutation/merge uses parent/child component vectors and zone cleanup (`src/MTGRules.cpp:4223-4270`, `src/MTGGameZones.cpp:684-735`); known `@mutated` trigger failure is catalogued in `strategy-design/lexicon-triage/engine-bugs.md`. |
| 730.1 | INCORRECT | Day/night is approximated by real `Day`/`Night` battlefield marker cards; regression fixtures explicitly document this (`bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt:6-14`, `bin/Res/test/lexicon/basic_nightbound_flips_after_two_spells.txt:9-17`). A marker card is an interactable/countable permanent-like object, contradicting the CR game designation. |
| 730.1a | INCORRECT | Day/night is approximated by real `Day`/`Night` battlefield marker cards; regression fixtures explicitly document this (`bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt:6-14`, `bin/Res/test/lexicon/basic_nightbound_flips_after_two_spells.txt:9-17`). A marker card is an interactable/countable permanent-like object, contradicting the CR game designation. |
| 730.2 | INCORRECT | Day/night is approximated by real `Day`/`Night` battlefield marker cards; regression fixtures explicitly document this (`bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt:6-14`, `bin/Res/test/lexicon/basic_nightbound_flips_after_two_spells.txt:9-17`). A marker card is an interactable/countable permanent-like object, contradicting the CR game designation. |
| 730.2a | INCORRECT | Day/night is approximated by real `Day`/`Night` battlefield marker cards; regression fixtures explicitly document this (`bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt:6-14`, `bin/Res/test/lexicon/basic_nightbound_flips_after_two_spells.txt:9-17`). A marker card is an interactable/countable permanent-like object, contradicting the CR game designation. |
| 730.2b | INCORRECT | Day/night is approximated by real `Day`/`Night` battlefield marker cards; regression fixtures explicitly document this (`bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt:6-14`, `bin/Res/test/lexicon/basic_nightbound_flips_after_two_spells.txt:9-17`). A marker card is an interactable/countable permanent-like object, contradicting the CR game designation. |
| 730.2c | INCORRECT | Day/night is approximated by real `Day`/`Night` battlefield marker cards; regression fixtures explicitly document this (`bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt:6-14`, `bin/Res/test/lexicon/basic_nightbound_flips_after_two_spells.txt:9-17`). A marker card is an interactable/countable permanent-like object, contradicting the CR game designation. |
| 731.1 | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.1a | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.1b | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.1c | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.2 | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.2a | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.2b | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.2c | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.3 | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.4 | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.5 | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 731.6 | N/A | N/A: informal negotiated physical-play shortcuts/loop declarations are not an engine game type or card mechanic; the digital UI executes explicit actions. |
| 732.1 | PARTIAL | Legality gates reject many invalid casts/actions (`src/MTGRules.cpp:307-330`, `src/MTGAbility.cpp:7715-7735`), but there is no transaction log capable of reversing every payment, trigger, zone move, shuffle, or revealed-library exception. |
| 732.2 | PARTIAL | Legality gates reject many invalid casts/actions (`src/MTGRules.cpp:307-330`, `src/MTGAbility.cpp:7715-7735`), but there is no transaction log capable of reversing every payment, trigger, zone move, shuffle, or revealed-library exception. |

## Counts

| verdict | count |
|---|---:|
| IMPLEMENTED | 39 |
| PARTIAL | 420 |
| INCORRECT | 18 |
| UNIMPLEMENTED | 177 |
| N/A | 79 |
| **TOTAL** | **733** |

## HIGHLIGHTS

1. **P0 — 704.3/704.7/704.8: the state-based-action pass is not simultaneous or fixed-point.** `GameObserver::gameStateBasedEffects()` mutates each player/card in sequence, dispatching events as it goes, and snapshots LKI one card at a time. This can expose intermediate states, mishandle one replacement effect applying to several identical SBA results, and derive LKI from different states. The correct repair direction is a collect-all/apply-all/repeat transaction before pending triggers are stacked.
2. **P0 — 704.5f: a normal creature at toughness 0 or less can regenerate.** The generic pass calls `destroy()` for non-indestructible creatures; `destroy()` invokes regeneration. Only the indestructible branch force-moves the creature. CR 704.5f is not destruction and cannot be regenerated.
3. **P0 — 704.5h/704.4: deathtouch destroys synchronously during the damage event.** `MTGDeathtouchRule::receiveEvent` immediately calls `destroy()`, rather than marking that deathtouch damage occurred and waiting for the next simultaneous SBA check. This permits incorrect ordering during resolving spells/abilities.
4. **P0 — 704.5j: the legend rule is an event-driven one-card mover, not “choose one, put all others away” as one SBA.** The chooser targets at most one duplicate. Three or more same-name legendary permanents can therefore remain illegal, and the action is not atomic with other SBAs.
5. **P1 — 704.5b/704.5d: empty-library loss and token disappearance occur immediately in their initiating functions/events.** Both should wait for the next SBA check. Immediate loss or cleanup can change trigger/replacement ordering inside a resolving effect.
6. **P1 — 704.5m: unattached Auras can survive.** Aura cleanup is entered only when `card->target` is non-null; an Aura represented with no attachment bypasses the state check entirely. Its enchant legality is also reduced to coarse string/type tests.
7. **P1 — 704.5v: zero-defense battles are marked defeated but the SBA does not put them into the graveyard.** The engine emits a defeated event and relies on follow-on card behavior, contradicting the base battle SBA when no pending defeat trigger applies.
8. **P1 — 704.5r: maximum-counter rules prevent additions instead of removing excess counters as an SBA.** This changes whether counters were put on the permanent and therefore changes replacement/trigger observability.
9. **P1 — 730: day/night is represented by a real battlefield marker card.** Existing green fixtures explicitly count and move that marker. CR day/night is a game designation, so the marker contaminates battlefield counts and can be selected or affected as a card.
10. **P1 — 703.4f + 704.5s + 714: Sagas lack centralized turn-action and final-chapter SBA machinery.** Card scripts can approximate individual Sagas, but there is no automatic lore-counter action or stack-aware final-chapter sacrifice rule.
11. **P2 — 707: copying is materially incomplete.** Permanent/token copying exists, but full copiable-value layers, spell/ability decisions, DFC copies, cast copies, and the copy-object SBA are not general engine concepts.
12. **P2 — 708/709/712/729: special card representations are approximations.** Morph/manifest, split cards, DFCs, meld, and mutate have dedicated fields and scripted paths, but their zone characteristics, copiable values, component identities, and face restrictions are not uniformly CR-conformant. The known `@mutated` trigger parser failure remains catalogued.
13. **P2 — 722/723/726/728: major additional-rule subsystems are absent.** Controlling another player, ending a turn/phase, restarting the game, and subgames have no general machinery.
14. **P2 — 724/725: monarch and initiative are flags, not complete inherent-rule subsystems.** Single-holder transfer events exist, but the inherent draw/combat/venture triggers and edge behavior are distributed among scripts and are incomplete.
15. **P3 — the post-2024 keyword-action tail is mostly absent.** Behold, connive, incubate, time travel, collect evidence, suspect, endure, airbend, earthbend, waterbend, and blight have no named implementation; several older actions are only compositions of generic script primitives.

