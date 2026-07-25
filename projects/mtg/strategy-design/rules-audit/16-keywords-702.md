# Comprehensive Rules 702 keyword-ability audit

| rule | keyword | verdict | evidence / note |
|---|---|---|---|
| 702.1 | Keyword abilities (general) | PARTIAL | `src/MTGDefinitions.cpp:33-273` has a fixed keyword catalogue; `src/MTGAbility.cpp:6002-6026` grants catalogue entries, but variants/variables are not generically modeled. |
| 702.1a | Keyword abilities (general) | PARTIAL | `src/MTGDefinitions.cpp:33-273` has a fixed keyword catalogue; `src/MTGAbility.cpp:6002-6026` grants catalogue entries, but variants/variables are not generically modeled. |
| 702.1b | Keyword abilities (general) | PARTIAL | `src/MTGDefinitions.cpp:33-273` has a fixed keyword catalogue; `src/MTGAbility.cpp:6002-6026` grants catalogue entries, but variants/variables are not generically modeled. |
| 702.1c | Keyword abilities (general) | PARTIAL | `src/MTGDefinitions.cpp:33-273` has a fixed keyword catalogue; `src/MTGAbility.cpp:6002-6026` grants catalogue entries, but variants/variables are not generically modeled. |
| 702.1d | Keyword abilities (general) | PARTIAL | `src/MTGDefinitions.cpp:33-273` has a fixed keyword catalogue; `src/MTGAbility.cpp:6002-6026` grants catalogue entries, but variants/variables are not generically modeled. |
| 702.2 | Deathtouch | INCORRECT | `src/MTGRules.cpp:4188-4213` uses current/LKI deathtouch, but destroys during the damage event rather than the toughness>0 state-based action required by 702.2b; `src/GuiCombat.cpp:134-162` does use one damage as lethal for assignment. |
| 702.2a | Deathtouch | IMPLEMENTED | `src/MTGRules.cpp:4206` checks current/LKI deathtouch; `src/GuiCombat.cpp:134-162` treats one combat damage as lethal and boolean storage makes instances redundant. |
| 702.2b | Deathtouch | INCORRECT | `src/MTGRules.cpp:4188-4213` uses current/LKI deathtouch, but destroys during the damage event rather than the toughness>0 state-based action required by 702.2b; `src/GuiCombat.cpp:134-162` does use one damage as lethal for assignment. |
| 702.2c | Deathtouch | IMPLEMENTED | `src/MTGRules.cpp:4206` checks current/LKI deathtouch; `src/GuiCombat.cpp:134-162` treats one combat damage as lethal and boolean storage makes instances redundant. |
| 702.2d | Deathtouch | IMPLEMENTED | `src/MTGRules.cpp:4206` checks current/LKI deathtouch; `src/GuiCombat.cpp:134-162` treats one combat damage as lethal and boolean storage makes instances redundant. |
| 702.2e | Deathtouch | IMPLEMENTED | `src/MTGRules.cpp:4206` checks current/LKI deathtouch; `src/GuiCombat.cpp:134-162` treats one combat damage as lethal and boolean storage makes instances redundant. |
| 702.2f | Deathtouch | IMPLEMENTED | `src/MTGRules.cpp:4206` checks current/LKI deathtouch; `src/GuiCombat.cpp:134-162` treats one combat damage as lethal and boolean storage makes instances redundant. |
| 702.3 | Defender | IMPLEMENTED | `src/MTGAbility.cpp:824-829` rejects attacks by defender; the boolean catalogue at `src/MTGDefinitions.cpp:50` makes duplicates redundant. |
| 702.3a | Defender | IMPLEMENTED | `src/MTGAbility.cpp:824-829` rejects attacks by defender; the boolean catalogue at `src/MTGDefinitions.cpp:50` makes duplicates redundant. |
| 702.3b | Defender | IMPLEMENTED | `src/MTGAbility.cpp:824-829` rejects attacks by defender; the boolean catalogue at `src/MTGDefinitions.cpp:50` makes duplicates redundant. |
| 702.3c | Defender | IMPLEMENTED | `src/MTGAbility.cpp:824-829` rejects attacks by defender; the boolean catalogue at `src/MTGDefinitions.cpp:50` makes duplicates redundant. |
| 702.4 | Double Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` assigns first/double-strike-step damage and reflects mid-combat ability changes. |
| 702.4a | Double Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` assigns first/double-strike-step damage and reflects mid-combat ability changes. |
| 702.4b | Double Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` assigns first/double-strike-step damage and reflects mid-combat ability changes. |
| 702.4c | Double Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` assigns first/double-strike-step damage and reflects mid-combat ability changes. |
| 702.4d | Double Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` assigns first/double-strike-step damage and reflects mid-combat ability changes. |
| 702.4e | Double Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` assigns first/double-strike-step damage and reflects mid-combat ability changes. |
| 702.5 | Enchant | PARTIAL | Aura targeting/attachment is implemented in `src/MTGAbility.cpp:609-807`, but the model does not evidence intersection of multiple enchant restrictions. |
| 702.5a | Enchant | PARTIAL | Aura targeting/attachment is implemented in `src/MTGAbility.cpp:609-807`, but the model does not evidence intersection of multiple enchant restrictions. |
| 702.5b | Enchant | PARTIAL | Aura targeting/attachment is implemented in `src/MTGAbility.cpp:609-807`, but the model does not evidence intersection of multiple enchant restrictions. |
| 702.5c | Enchant | PARTIAL | Aura targeting/attachment is implemented in `src/MTGAbility.cpp:609-807`, but the model does not evidence intersection of multiple enchant restrictions. |
| 702.5d | Enchant | PARTIAL | Aura targeting/attachment is implemented in `src/MTGAbility.cpp:609-807`, but the model does not evidence intersection of multiple enchant restrictions. |
| 702.6 | Equip | PARTIAL | `src/AllAbilities.cpp:1479-1537` implements attachment and sorcery-speed equip scripts; no equip-planeswalker parser/behavior was found in the named keyword surfaces. |
| 702.6a | Equip | PARTIAL | `src/AllAbilities.cpp:1479-1537` implements attachment and sorcery-speed equip scripts; no equip-planeswalker parser/behavior was found in the named keyword surfaces. |
| 702.6b | Equip | PARTIAL | `src/AllAbilities.cpp:1479-1537` implements attachment and sorcery-speed equip scripts; no equip-planeswalker parser/behavior was found in the named keyword surfaces. |
| 702.6c | Equip | PARTIAL | `src/AllAbilities.cpp:1479-1537` implements attachment and sorcery-speed equip scripts; no equip-planeswalker parser/behavior was found in the named keyword surfaces. |
| 702.6d | Equip | PARTIAL | `src/AllAbilities.cpp:1479-1537` implements attachment and sorcery-speed equip scripts; no equip-planeswalker parser/behavior was found in the named keyword surfaces. |
| 702.6e | Equip | UNIMPLEMENTED | Good-faith named search found no equip-planeswalker target/as-creature behavior in the requested keyword surfaces. |
| 702.7 | First Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` implements the two combat-damage-step rules and boolean redundancy. |
| 702.7a | First Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` implements the two combat-damage-step rules and boolean redundancy. |
| 702.7b | First Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` implements the two combat-damage-step rules and boolean redundancy. |
| 702.7c | First Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` implements the two combat-damage-step rules and boolean redundancy. |
| 702.7d | First Strike | IMPLEMENTED | `src/MTGCardInstance.cpp:2075-2093` implements the two combat-damage-step rules and boolean redundancy. |
| 702.8 | Flash | IMPLEMENTED | `src/MTGRules.cpp:344-346` permits flash timing from playable zones. |
| 702.8a | Flash | IMPLEMENTED | `src/MTGRules.cpp:344-346` permits flash timing from playable zones. |
| 702.8b | Flash | IMPLEMENTED | `src/MTGRules.cpp:344-346` permits flash timing from playable zones. |
| 702.9 | Flying | IMPLEMENTED | `src/MTGCardInstance.cpp:1184-1185` enforces flying/reach blocking. |
| 702.9a | Flying | IMPLEMENTED | `src/MTGCardInstance.cpp:1184-1185` enforces flying/reach blocking. |
| 702.9b | Flying | IMPLEMENTED | `src/MTGCardInstance.cpp:1184-1185` enforces flying/reach blocking. |
| 702.9c | Flying | IMPLEMENTED | `src/MTGCardInstance.cpp:1184-1185` enforces flying/reach blocking. |
| 702.10 | Haste | IMPLEMENTED | `src/MTGAbility.cpp:814-842` applies summoning-sickness checks and the haste exception to attacks and tap abilities. |
| 702.10a | Haste | IMPLEMENTED | `src/MTGAbility.cpp:814-842` applies summoning-sickness checks and the haste exception to attacks and tap abilities. |
| 702.10b | Haste | IMPLEMENTED | `src/MTGAbility.cpp:814-842` applies summoning-sickness checks and the haste exception to attacks and tap abilities. |
| 702.10c | Haste | IMPLEMENTED | `src/MTGAbility.cpp:814-842` applies summoning-sickness checks and the haste exception to attacks and tap abilities. |
| 702.10d | Haste | IMPLEMENTED | `src/MTGAbility.cpp:814-842` applies summoning-sickness checks and the haste exception to attacks and tap abilities. |
| 702.11 | Hexproof | PARTIAL | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.11a | Hexproof | IMPLEMENTED | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.11b | Hexproof | IMPLEMENTED | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.11c | Hexproof | IMPLEMENTED | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.11d | Hexproof | PARTIAL | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.11e | Hexproof | PARTIAL | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.11f | Hexproof | PARTIAL | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.11g | Hexproof | PARTIAL | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.11h | Hexproof | IMPLEMENTED | `src/TargetChooser.cpp:1556-1560,2082-2139` enforces ordinary permanent/player hexproof; the fixed `src/MTGDefinitions.cpp:75,113-114` catalogue has no quality-variant model. |
| 702.12 | Indestructible | IMPLEMENTED | Destruction checks consume `Constants::INDESTRUCTIBLE` (`src/AllAbilities.cpp:3264-3277`); the flag is boolean/redundant. |
| 702.12a | Indestructible | IMPLEMENTED | Destruction checks consume `Constants::INDESTRUCTIBLE` (`src/AllAbilities.cpp:3264-3277`); the flag is boolean/redundant. |
| 702.12b | Indestructible | IMPLEMENTED | Destruction checks consume `Constants::INDESTRUCTIBLE` (`src/AllAbilities.cpp:3264-3277`); the flag is boolean/redundant. |
| 702.12c | Indestructible | IMPLEMENTED | Destruction checks consume `Constants::INDESTRUCTIBLE` (`src/AllAbilities.cpp:3264-3277`); the flag is boolean/redundant. |
| 702.13 | Intimidate | IMPLEMENTED | `src/MTGCardInstance.cpp:1169-1182` enforces artifact-or-shared-color blocking. |
| 702.13a | Intimidate | IMPLEMENTED | `src/MTGCardInstance.cpp:1169-1182` enforces artifact-or-shared-color blocking. |
| 702.13b | Intimidate | IMPLEMENTED | `src/MTGCardInstance.cpp:1169-1182` enforces artifact-or-shared-color blocking. |
| 702.13c | Intimidate | IMPLEMENTED | `src/MTGCardInstance.cpp:1169-1182` enforces artifact-or-shared-color blocking. |
| 702.14 | Landwalk | PARTIAL | `src/MTGCardInstance.cpp:1202-1228` implements only enumerated basic, snow, nonbasic, legendary, and desert landwalk forms, not arbitrary 702.14 qualities. |
| 702.14a | Landwalk | PARTIAL | `src/MTGCardInstance.cpp:1202-1228` implements only enumerated basic, snow, nonbasic, legendary, and desert landwalk forms, not arbitrary 702.14 qualities. |
| 702.14b | Landwalk | IMPLEMENTED | `src/MTGCardInstance.cpp:1202-1228` implements only enumerated basic, snow, nonbasic, legendary, and desert landwalk forms, not arbitrary 702.14 qualities. |
| 702.14c | Landwalk | PARTIAL | `src/MTGCardInstance.cpp:1202-1228` implements only enumerated basic, snow, nonbasic, legendary, and desert landwalk forms, not arbitrary 702.14 qualities. |
| 702.14d | Landwalk | IMPLEMENTED | `src/MTGCardInstance.cpp:1202-1228` implements only enumerated basic, snow, nonbasic, legendary, and desert landwalk forms, not arbitrary 702.14 qualities. |
| 702.14e | Landwalk | IMPLEMENTED | `src/MTGCardInstance.cpp:1202-1228` implements only enumerated basic, snow, nonbasic, legendary, and desert landwalk forms, not arbitrary 702.14 qualities. |
| 702.15 | Lifelink | IMPLEMENTED | `src/MTGRules.cpp:4155-4165` gains life from each damage event and checks LKI ability flags. |
| 702.15a | Lifelink | IMPLEMENTED | `src/MTGRules.cpp:4155-4165` gains life from each damage event and checks LKI ability flags. |
| 702.15b | Lifelink | IMPLEMENTED | `src/MTGRules.cpp:4155-4165` gains life from each damage event and checks LKI ability flags. |
| 702.15c | Lifelink | IMPLEMENTED | `src/MTGRules.cpp:4155-4165` gains life from each damage event and checks LKI ability flags. |
| 702.15d | Lifelink | IMPLEMENTED | `src/MTGRules.cpp:4155-4165` gains life from each damage event and checks LKI ability flags. |
| 702.15e | Lifelink | IMPLEMENTED | `src/MTGRules.cpp:4155-4165` gains life from each damage event and checks LKI ability flags. |
| 702.15f | Lifelink | IMPLEMENTED | `src/MTGRules.cpp:4155-4165` gains life from each damage event and checks LKI ability flags. |
| 702.16 | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16a | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16b | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16c | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16d | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16e | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16f | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16g | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16h | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16i | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16j | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16k | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16m | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16n | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.16p | Protection | PARTIAL | `src/MTGCardInstance.cpp:1877-1881`, `src/TargetChooser.cpp:1556-1567`, and `src/Damage.cpp:72-108` implement DEBT only for enumerated colors/colored spells, not arbitrary qualities/everything. |
| 702.17 | Reach | IMPLEMENTED | `src/MTGCardInstance.cpp:1184-1185` allows reach to block flying without restricting what it can block. |
| 702.17a | Reach | IMPLEMENTED | `src/MTGCardInstance.cpp:1184-1185` allows reach to block flying without restricting what it can block. |
| 702.17b | Reach | IMPLEMENTED | `src/MTGCardInstance.cpp:1184-1185` allows reach to block flying without restricting what it can block. |
| 702.17c | Reach | IMPLEMENTED | `src/MTGCardInstance.cpp:1184-1185` allows reach to block flying without restricting what it can block. |
| 702.18 | Shroud | IMPLEMENTED | `src/TargetChooser.cpp:1556,2135` rejects targets for shroud regardless of controller. |
| 702.18a | Shroud | IMPLEMENTED | `src/TargetChooser.cpp:1556,2135` rejects targets for shroud regardless of controller. |
| 702.18b | Shroud | IMPLEMENTED | `src/TargetChooser.cpp:1556,2135` rejects targets for shroud regardless of controller. |
| 702.19 | Trample | PARTIAL | Combat assignment/trample routing exists in `src/GuiCombat.cpp:104-167,473-535`, but static evidence does not establish all planeswalker/excess-damage and deathtouch ordering cases in 702.19. |
| 702.19a | Trample | PARTIAL | Combat assignment/trample routing exists in `src/GuiCombat.cpp:104-167,473-535`, but static evidence does not establish all planeswalker/excess-damage and deathtouch ordering cases in 702.19. |
| 702.19b | Trample | PARTIAL | Combat assignment/trample routing exists in `src/GuiCombat.cpp:104-167,473-535`, but static evidence does not establish all planeswalker/excess-damage and deathtouch ordering cases in 702.19. |
| 702.19c | Trample | PARTIAL | Combat assignment/trample routing exists in `src/GuiCombat.cpp:104-167,473-535`, but static evidence does not establish all planeswalker/excess-damage and deathtouch ordering cases in 702.19. |
| 702.19d | Trample | PARTIAL | Combat assignment/trample routing exists in `src/GuiCombat.cpp:104-167,473-535`, but static evidence does not establish all planeswalker/excess-damage and deathtouch ordering cases in 702.19. |
| 702.19e | Trample | PARTIAL | Combat assignment/trample routing exists in `src/GuiCombat.cpp:104-167,473-535`, but static evidence does not establish all planeswalker/excess-damage and deathtouch ordering cases in 702.19. |
| 702.19f | Trample | PARTIAL | Combat assignment/trample routing exists in `src/GuiCombat.cpp:104-167,473-535`, but static evidence does not establish all planeswalker/excess-damage and deathtouch ordering cases in 702.19. |
| 702.19g | Trample | IMPLEMENTED | Combat assignment/trample routing exists in `src/GuiCombat.cpp:104-167,473-535`, but static evidence does not establish all planeswalker/excess-damage and deathtouch ordering cases in 702.19. |
| 702.20 | Vigilance | IMPLEMENTED | `src/MTGRules.cpp:2150-2151` omits the attack tap for vigilance. |
| 702.20a | Vigilance | IMPLEMENTED | `src/MTGRules.cpp:2150-2151` omits the attack tap for vigilance. |
| 702.20b | Vigilance | IMPLEMENTED | `src/MTGRules.cpp:2150-2151` omits the attack tap for vigilance. |
| 702.20c | Vigilance | IMPLEMENTED | `src/MTGRules.cpp:2150-2151` omits the attack tap for vigilance. |
| 702.21 | Ward | PARTIAL | Ward pay/decline behavior is covered by `bin/Res/test/lexicon/macro_ward_pay.txt` and `macro_ward_decline.txt`; `_macros.txt:196-201` only provides fixed numeric mana wards. |
| 702.21a | Ward | PARTIAL | Ward pay/decline behavior is covered by `bin/Res/test/lexicon/macro_ward_pay.txt` and `macro_ward_decline.txt`; `_macros.txt:196-201` only provides fixed numeric mana wards. |
| 702.21b | Ward | PARTIAL | Ward pay/decline behavior is covered by `bin/Res/test/lexicon/macro_ward_pay.txt` and `macro_ward_decline.txt`; `_macros.txt:196-201` only provides fixed numeric mana wards. |
| 702.22 | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22a | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22b | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22c | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22d | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22e | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22f | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22g | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22h | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22i | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22j | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22k | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.22m | Banding | UNIMPLEMENTED | Known catalogue defect: `MTGCardInstance::banding` is never made operational; failing witness `bin/Res/test/lexicon/basic_banding_attacks_as_group.txt` is registered in `_known_failures.txt:22`. |
| 702.23 | Rampage | IMPLEMENTED | `src/MTGAbility.cpp:5097-5120` creates the once-per-blocked-event rampage bonus; `bin/Res/test/rampage.txt` exercises it. |
| 702.23a | Rampage | IMPLEMENTED | `src/MTGAbility.cpp:5097-5120` creates the once-per-blocked-event rampage bonus; `bin/Res/test/rampage.txt` exercises it. |
| 702.23b | Rampage | IMPLEMENTED | `src/MTGAbility.cpp:5097-5120` creates the once-per-blocked-event rampage bonus; `bin/Res/test/rampage.txt` exercises it. |
| 702.23c | Rampage | IMPLEMENTED | `src/MTGAbility.cpp:5097-5120` creates the once-per-blocked-event rampage bonus; `bin/Res/test/rampage.txt` exercises it. |
| 702.24 | Cumulative Upkeep | PARTIAL | `src/MTGAbility.cpp:2953-2991` and `src/AllAbilities.cpp:8811-8898` provide age-counter/upkeep-cost machinery, but static reading does not establish per-counter independent choices or multiple-instance accounting. |
| 702.24a | Cumulative Upkeep | PARTIAL | `src/MTGAbility.cpp:2953-2991` and `src/AllAbilities.cpp:8811-8898` provide age-counter/upkeep-cost machinery, but static reading does not establish per-counter independent choices or multiple-instance accounting. |
| 702.24b | Cumulative Upkeep | PARTIAL | `src/MTGAbility.cpp:2953-2991` and `src/AllAbilities.cpp:8811-8898` provide age-counter/upkeep-cost machinery, but static reading does not establish per-counter independent choices or multiple-instance accounting. |
| 702.25 | Flanking | IMPLEMENTED | `src/MTGAbility.cpp:5160-5176` creates a separate flanking trigger per instance; `bin/Res/test/stacking_flanking.txt` covers stacking. |
| 702.25a | Flanking | IMPLEMENTED | `src/MTGAbility.cpp:5160-5176` creates a separate flanking trigger per instance; `bin/Res/test/stacking_flanking.txt` covers stacking. |
| 702.25b | Flanking | IMPLEMENTED | `src/MTGAbility.cpp:5160-5176` creates a separate flanking trigger per instance; `bin/Res/test/stacking_flanking.txt` covers stacking. |
| 702.26 | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26a | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26b | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26c | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26d | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26e | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26f | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26g | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26h | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26i | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26j | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26k | Phasing | N/A | N/A: player-leaves-game/multiplayer phasing clause; multiplayer is excluded. |
| 702.26m | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.26n | Phasing | N/A | N/A: player-leaves-game/multiplayer phasing clause; multiplayer is excluded. |
| 702.26p | Phasing | UNIMPLEMENTED | Known catalogue defect: phasing has a name/effect parser (`src/MTGDefinitions.cpp:100`; `src/MTGAbility.cpp:4226`) but no untap-step toggle; failing witness `bin/Res/test/lexicon/basic_phasing_leaves_and_returns.txt`. |
| 702.27 | Buyback | IMPLEMENTED | Buyback cast/payment and destination replacement are implemented at `src/MTGRules.cpp:1220-1342` and `src/MTGAbility.cpp:7372-7376`. |
| 702.27a | Buyback | IMPLEMENTED | Buyback cast/payment and destination replacement are implemented at `src/MTGRules.cpp:1220-1342` and `src/MTGAbility.cpp:7372-7376`. |
| 702.28 | Shadow | IMPLEMENTED | `src/MTGCardInstance.cpp:1190-1194` enforces the symmetric shadow restriction. |
| 702.28a | Shadow | IMPLEMENTED | `src/MTGCardInstance.cpp:1190-1194` enforces the symmetric shadow restriction. |
| 702.28b | Shadow | IMPLEMENTED | `src/MTGCardInstance.cpp:1190-1194` enforces the symmetric shadow restriction. |
| 702.28c | Shadow | IMPLEMENTED | `src/MTGCardInstance.cpp:1190-1194` enforces the symmetric shadow restriction. |
| 702.29 | Cycling | PARTIAL | Cycling activation/event ordering is implemented in `src/AllAbilities.cpp:1784-1847` and `src/MTGAbility.cpp:8171-8177`; only basic-landcycling has a reusable grammar macro (`_macros.txt:12-13`). |
| 702.29a | Cycling | IMPLEMENTED | Cycling activation/event ordering is implemented in `src/AllAbilities.cpp:1784-1847` and `src/MTGAbility.cpp:8171-8177`; only basic-landcycling has a reusable grammar macro (`_macros.txt:12-13`). |
| 702.29b | Cycling | IMPLEMENTED | Cycling activation/event ordering is implemented in `src/AllAbilities.cpp:1784-1847` and `src/MTGAbility.cpp:8171-8177`; only basic-landcycling has a reusable grammar macro (`_macros.txt:12-13`). |
| 702.29c | Cycling | IMPLEMENTED | Cycling activation/event ordering is implemented in `src/AllAbilities.cpp:1784-1847` and `src/MTGAbility.cpp:8171-8177`; only basic-landcycling has a reusable grammar macro (`_macros.txt:12-13`). |
| 702.29d | Cycling | IMPLEMENTED | Cycling activation/event ordering is implemented in `src/AllAbilities.cpp:1784-1847` and `src/MTGAbility.cpp:8171-8177`; only basic-landcycling has a reusable grammar macro (`_macros.txt:12-13`). |
| 702.29e | Cycling | PARTIAL | Cycling activation/event ordering is implemented in `src/AllAbilities.cpp:1784-1847` and `src/MTGAbility.cpp:8171-8177`; only basic-landcycling has a reusable grammar macro (`_macros.txt:12-13`). |
| 702.29f | Cycling | PARTIAL | Cycling activation/event ordering is implemented in `src/AllAbilities.cpp:1784-1847` and `src/MTGAbility.cpp:8171-8177`; only basic-landcycling has a reusable grammar macro (`_macros.txt:12-13`). |
| 702.30 | Echo | IMPLEMENTED | `_macros.txt:112-113` uses the next-upkeep cost primitive; active regression `bin/Res/test/lexicon/macro_echo.txt` covers pay-to-keep. |
| 702.30a | Echo | IMPLEMENTED | `_macros.txt:112-113` uses the next-upkeep cost primitive; active regression `bin/Res/test/lexicon/macro_echo.txt` covers pay-to-keep. |
| 702.30b | Echo | IMPLEMENTED | `_macros.txt:112-113` uses the next-upkeep cost primitive; active regression `bin/Res/test/lexicon/macro_echo.txt` covers pay-to-keep. |
| 702.31 | Horsemanship | IMPLEMENTED | `src/MTGCardInstance.cpp:1196-1197` implements the one-way horsemanship block restriction. |
| 702.31a | Horsemanship | IMPLEMENTED | `src/MTGCardInstance.cpp:1196-1197` implements the one-way horsemanship block restriction. |
| 702.31b | Horsemanship | IMPLEMENTED | `src/MTGCardInstance.cpp:1196-1197` implements the one-way horsemanship block restriction. |
| 702.31c | Horsemanship | IMPLEMENTED | `src/MTGCardInstance.cpp:1196-1197` implements the one-way horsemanship block restriction. |
| 702.32 | Fading | IMPLEMENTED | `src/MTGAbility.cpp:5444-5454` and `src/AllAbilities.cpp:9449-9517` implement fade counters and upkeep sacrifice. |
| 702.32a | Fading | IMPLEMENTED | `src/MTGAbility.cpp:5444-5454` and `src/AllAbilities.cpp:9449-9517` implement fade counters and upkeep sacrifice. |
| 702.33 | Kicker | PARTIAL | Kicker/multikicker payments and counts are implemented in `src/MTGRules.cpp:561-583,678-823`; no sticker-kicker/sticker machinery is in scope. |
| 702.33a | Kicker | PARTIAL | Kicker/multikicker payments and counts are implemented in `src/MTGRules.cpp:561-583,678-823`; no sticker-kicker/sticker machinery is in scope. |
| 702.33b | Kicker | PARTIAL | Kicker/multikicker payments and counts are implemented in `src/MTGRules.cpp:561-583,678-823`; no sticker-kicker/sticker machinery is in scope. |
| 702.33c | Kicker | PARTIAL | Kicker/multikicker payments and counts are implemented in `src/MTGRules.cpp:561-583,678-823`; no sticker-kicker/sticker machinery is in scope. |
| 702.33d | Kicker | PARTIAL | Kicker/multikicker payments and counts are implemented in `src/MTGRules.cpp:561-583,678-823`; no sticker-kicker/sticker machinery is in scope. |
| 702.33e | Kicker | PARTIAL | Kicker/multikicker payments and counts are implemented in `src/MTGRules.cpp:561-583,678-823`; no sticker-kicker/sticker machinery is in scope. |
| 702.33f | Kicker | PARTIAL | Kicker/multikicker payments and counts are implemented in `src/MTGRules.cpp:561-583,678-823`; no sticker-kicker/sticker machinery is in scope. |
| 702.33g | Kicker | PARTIAL | Kicker/multikicker payments and counts are implemented in `src/MTGRules.cpp:561-583,678-823`; no sticker-kicker/sticker machinery is in scope. |
| 702.33h | Kicker | N/A | N/A: sticker kicker depends on physical sticker/ticket procedures outside supported classic play. |
| 702.34 | Flashback | IMPLEMENTED | Flashback casting and exile replacement are implemented in `src/MTGRules.cpp:1280-1450` and `src/MTGAbility.cpp:7377-7387`. |
| 702.34a | Flashback | IMPLEMENTED | Flashback casting and exile replacement are implemented in `src/MTGRules.cpp:1280-1450` and `src/MTGAbility.cpp:7377-7387`. |
| 702.35 | Madness | PARTIAL | Madness replacement/cast paths exist at `src/MTGAbility.cpp:650-677,4392-4427`; static evidence is incomplete for every public-zone post-resolution reference case. |
| 702.35a | Madness | PARTIAL | Madness replacement/cast paths exist at `src/MTGAbility.cpp:650-677,4392-4427`; static evidence is incomplete for every public-zone post-resolution reference case. |
| 702.35b | Madness | PARTIAL | Madness replacement/cast paths exist at `src/MTGAbility.cpp:650-677,4392-4427`; static evidence is incomplete for every public-zone post-resolution reference case. |
| 702.35c | Madness | PARTIAL | Madness replacement/cast paths exist at `src/MTGAbility.cpp:650-677,4392-4427`; static evidence is incomplete for every public-zone post-resolution reference case. |
| 702.36 | Fear | IMPLEMENTED | `src/MTGCardInstance.cpp:1164-1167` enforces artifact-or-black blocking. |
| 702.36a | Fear | IMPLEMENTED | `src/MTGCardInstance.cpp:1164-1167` enforces artifact-or-black blocking. |
| 702.36b | Fear | IMPLEMENTED | `src/MTGCardInstance.cpp:1164-1167` enforces artifact-or-black blocking. |
| 702.36c | Fear | IMPLEMENTED | `src/MTGCardInstance.cpp:1164-1167` enforces artifact-or-black blocking. |
| 702.37 | Morph | PARTIAL | Face-down cast/turn-up machinery exists at `src/MTGRules.cpp:1575-1677` and `src/MTGAbility.cpp:5925-5939,6589-6610`; megamorph/X/copiable-value edge cases are not fully modeled. |
| 702.37a | Morph | PARTIAL | Face-down cast/turn-up machinery exists at `src/MTGRules.cpp:1575-1677` and `src/MTGAbility.cpp:5925-5939,6589-6610`; megamorph/X/copiable-value edge cases are not fully modeled. |
| 702.37b | Morph | PARTIAL | Face-down cast/turn-up machinery exists at `src/MTGRules.cpp:1575-1677` and `src/MTGAbility.cpp:5925-5939,6589-6610`; megamorph/X/copiable-value edge cases are not fully modeled. |
| 702.37c | Morph | PARTIAL | Face-down cast/turn-up machinery exists at `src/MTGRules.cpp:1575-1677` and `src/MTGAbility.cpp:5925-5939,6589-6610`; megamorph/X/copiable-value edge cases are not fully modeled. |
| 702.37d | Morph | PARTIAL | Face-down cast/turn-up machinery exists at `src/MTGRules.cpp:1575-1677` and `src/MTGAbility.cpp:5925-5939,6589-6610`; megamorph/X/copiable-value edge cases are not fully modeled. |
| 702.37e | Morph | PARTIAL | Face-down cast/turn-up machinery exists at `src/MTGRules.cpp:1575-1677` and `src/MTGAbility.cpp:5925-5939,6589-6610`; megamorph/X/copiable-value edge cases are not fully modeled. |
| 702.37f | Morph | PARTIAL | Face-down cast/turn-up machinery exists at `src/MTGRules.cpp:1575-1677` and `src/MTGAbility.cpp:5925-5939,6589-6610`; megamorph/X/copiable-value edge cases are not fully modeled. |
| 702.37g | Morph | PARTIAL | Face-down cast/turn-up machinery exists at `src/MTGRules.cpp:1575-1677` and `src/MTGAbility.cpp:5925-5939,6589-6610`; megamorph/X/copiable-value edge cases are not fully modeled. |
| 702.38 | Amplify | UNIMPLEMENTED | Good-faith search found only card text/hand scripts for `amplify`; no amplify parser or behavior in `MTGDefinitions.cpp`, `MTGAbility.cpp`, `AllAbilities.cpp`, `MTGRules.cpp`, or `CardPrimitive.cpp`. |
| 702.38a | Amplify | UNIMPLEMENTED | Good-faith search found only card text/hand scripts for `amplify`; no amplify parser or behavior in `MTGDefinitions.cpp`, `MTGAbility.cpp`, `AllAbilities.cpp`, `MTGRules.cpp`, or `CardPrimitive.cpp`. |
| 702.38b | Amplify | UNIMPLEMENTED | Good-faith search found only card text/hand scripts for `amplify`; no amplify parser or behavior in `MTGDefinitions.cpp`, `MTGAbility.cpp`, `AllAbilities.cpp`, `MTGRules.cpp`, or `CardPrimitive.cpp`. |
| 702.39 | Provoke | IMPLEMENTED | `src/MTGAbility.cpp:4266-4291` implements provoke/untap and `src/MTGRules.cpp:2532` enforces the block. |
| 702.39a | Provoke | IMPLEMENTED | `src/MTGAbility.cpp:4266-4291` implements provoke/untap and `src/MTGRules.cpp:2532` enforces the block. |
| 702.39b | Provoke | IMPLEMENTED | `src/MTGAbility.cpp:4266-4291` implements provoke/untap and `src/MTGRules.cpp:2532` enforces the block. |
| 702.40 | Storm | IMPLEMENTED | `src/MTGRules.cpp:644-654` creates copies from prior spells; `bin/Res/test/lexicon/basic_storm_copies_prior_spell.txt` covers behavior. |
| 702.40a | Storm | IMPLEMENTED | `src/MTGRules.cpp:644-654` creates copies from prior spells; `bin/Res/test/lexicon/basic_storm_copies_prior_spell.txt` covers behavior. |
| 702.40b | Storm | IMPLEMENTED | `src/MTGRules.cpp:644-654` creates copies from prior spells; `bin/Res/test/lexicon/basic_storm_copies_prior_spell.txt` covers behavior. |
| 702.41 | Affinity | IMPLEMENTED | Enumerated and selector-based affinity reductions are implemented at `src/MTGAbility.cpp:5827-5840` and `src/AllAbilities.cpp:7666-7686`. |
| 702.41a | Affinity | IMPLEMENTED | Enumerated and selector-based affinity reductions are implemented at `src/MTGAbility.cpp:5827-5840` and `src/AllAbilities.cpp:7666-7686`. |
| 702.41b | Affinity | IMPLEMENTED | Enumerated and selector-based affinity reductions are implemented at `src/MTGAbility.cpp:5827-5840` and `src/AllAbilities.cpp:7666-7686`. |
| 702.42 | Entwine | PARTIAL | Cards encode entwine through alternate-cost/mode scripts (for example `primitives/mtg.txt` `name(Entwine)` lines); no native additional-cost/mode-link keyword behavior was found. |
| 702.42a | Entwine | PARTIAL | Cards encode entwine through alternate-cost/mode scripts (for example `primitives/mtg.txt` `name(Entwine)` lines); no native additional-cost/mode-link keyword behavior was found. |
| 702.42b | Entwine | PARTIAL | Cards encode entwine through alternate-cost/mode scripts (for example `primitives/mtg.txt` `name(Entwine)` lines); no native additional-cost/mode-link keyword behavior was found. |
| 702.43 | Modular | IMPLEMENTED | `src/MTGAbility.cpp:7335-7343` installs modular and `bin/Res/test/lexicon/basic_modular_transfers_counter.txt` covers transfer. |
| 702.43a | Modular | IMPLEMENTED | `src/MTGAbility.cpp:7335-7343` installs modular and `bin/Res/test/lexicon/basic_modular_transfers_counter.txt` covers transfer. |
| 702.43b | Modular | IMPLEMENTED | `src/MTGAbility.cpp:7335-7343` installs modular and `bin/Res/test/lexicon/basic_modular_transfers_counter.txt` covers transfer. |
| 702.44 | Sunburst | PARTIAL | `src/MTGRules.cpp:360-387` counts mana colors for sunburst, but the payment-color bookkeeping and both counter-kind branches are not comprehensively evidenced. |
| 702.44a | Sunburst | PARTIAL | `src/MTGRules.cpp:360-387` counts mana colors for sunburst, but the payment-color bookkeeping and both counter-kind branches are not comprehensively evidenced. |
| 702.44b | Sunburst | PARTIAL | `src/MTGRules.cpp:360-387` counts mana colors for sunburst, but the payment-color bookkeeping and both counter-kind branches are not comprehensively evidenced. |
| 702.44c | Sunburst | PARTIAL | `src/MTGRules.cpp:360-387` counts mana colors for sunburst, but the payment-color bookkeeping and both counter-kind branches are not comprehensively evidenced. |
| 702.44d | Sunburst | PARTIAL | `src/MTGRules.cpp:360-387` counts mana colors for sunburst, but the payment-color bookkeeping and both counter-kind branches are not comprehensively evidenced. |
| 702.45 | Bushido | IMPLEMENTED | `src/MTGAbility.cpp:5178-5186` creates separate bushido bonuses. |
| 702.45a | Bushido | IMPLEMENTED | `src/MTGAbility.cpp:5178-5186` creates separate bushido bonuses. |
| 702.45b | Bushido | IMPLEMENTED | `src/MTGAbility.cpp:5178-5186` creates separate bushido bonuses. |
| 702.46 | Soulshift | PARTIAL | Soulshift is reproduced by per-card death/target scripts in primitives; no native linked keyword consumer exists in the named C++ surfaces. |
| 702.46a | Soulshift | PARTIAL | Soulshift is reproduced by per-card death/target scripts in primitives; no native linked keyword consumer exists in the named C++ surfaces. |
| 702.46b | Soulshift | PARTIAL | Soulshift is reproduced by per-card death/target scripts in primitives; no native linked keyword consumer exists in the named C++ surfaces. |
| 702.47 | Splice | UNIMPLEMENTED | `_macros.txt:118-119` explicitly labels Splice onto Arcane “not implemented”; no native splice consumer was found. |
| 702.47a | Splice | UNIMPLEMENTED | `_macros.txt:118-119` explicitly labels Splice onto Arcane “not implemented”; no native splice consumer was found. |
| 702.47b | Splice | UNIMPLEMENTED | `_macros.txt:118-119` explicitly labels Splice onto Arcane “not implemented”; no native splice consumer was found. |
| 702.47c | Splice | UNIMPLEMENTED | `_macros.txt:118-119` explicitly labels Splice onto Arcane “not implemented”; no native splice consumer was found. |
| 702.47d | Splice | UNIMPLEMENTED | `_macros.txt:118-119` explicitly labels Splice onto Arcane “not implemented”; no native splice consumer was found. |
| 702.47e | Splice | UNIMPLEMENTED | `_macros.txt:118-119` explicitly labels Splice onto Arcane “not implemented”; no native splice consumer was found. |
| 702.48 | Offering | PARTIAL | Offering cost reduction/sacrifice has code at `src/MTGRules.cpp:934-999,1132`; chooser-based casts remain a documented suite/interaction ambiguity. |
| 702.48a | Offering | PARTIAL | Offering cost reduction/sacrifice has code at `src/MTGRules.cpp:934-999,1132`; chooser-based casts remain a documented suite/interaction ambiguity. |
| 702.48b | Offering | PARTIAL | Offering cost reduction/sacrifice has code at `src/MTGRules.cpp:934-999,1132`; chooser-based casts remain a documented suite/interaction ambiguity. |
| 702.48c | Offering | PARTIAL | Offering cost reduction/sacrifice has code at `src/MTGRules.cpp:934-999,1132`; chooser-based casts remain a documented suite/interaction ambiguity. |
| 702.49 | Ninjutsu | IMPLEMENTED | Ninjutsu activation/events are implemented at `src/MTGAbility.cpp:1616-1620,3622-3654`; regression `cost_ninjutsu_return_attacker.txt` covers the return cost. |
| 702.49a | Ninjutsu | IMPLEMENTED | Ninjutsu activation/events are implemented at `src/MTGAbility.cpp:1616-1620,3622-3654`; regression `cost_ninjutsu_return_attacker.txt` covers the return cost. |
| 702.49b | Ninjutsu | IMPLEMENTED | Ninjutsu activation/events are implemented at `src/MTGAbility.cpp:1616-1620,3622-3654`; regression `cost_ninjutsu_return_attacker.txt` covers the return cost. |
| 702.49c | Ninjutsu | IMPLEMENTED | Ninjutsu activation/events are implemented at `src/MTGAbility.cpp:1616-1620,3622-3654`; regression `cost_ninjutsu_return_attacker.txt` covers the return cost. |
| 702.49d | Ninjutsu | IMPLEMENTED | Ninjutsu activation/events are implemented at `src/MTGAbility.cpp:1616-1620,3622-3654`; regression `cost_ninjutsu_return_attacker.txt` covers the return cost. |
| 702.50 | Epic | IMPLEMENTED | `src/MTGAbility.cpp:4537-4558`, `src/AllAbilities.cpp:4615-4657`, and `src/MTGRules.cpp:346` implement copy-on-upkeep and the cast lock. |
| 702.50a | Epic | IMPLEMENTED | `src/MTGAbility.cpp:4537-4558`, `src/AllAbilities.cpp:4615-4657`, and `src/MTGRules.cpp:346` implement copy-on-upkeep and the cast lock. |
| 702.50b | Epic | IMPLEMENTED | `src/MTGAbility.cpp:4537-4558`, `src/AllAbilities.cpp:4615-4657`, and `src/MTGRules.cpp:346` implement copy-on-upkeep and the cast lock. |
| 702.51 | Convoke | IMPLEMENTED | Convoke extra-cost/reduction behavior is selected at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_convoke_multi_creature.txt` covers multiple creatures. |
| 702.51a | Convoke | IMPLEMENTED | Convoke extra-cost/reduction behavior is selected at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_convoke_multi_creature.txt` covers multiple creatures. |
| 702.51b | Convoke | IMPLEMENTED | Convoke extra-cost/reduction behavior is selected at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_convoke_multi_creature.txt` covers multiple creatures. |
| 702.51c | Convoke | IMPLEMENTED | Convoke extra-cost/reduction behavior is selected at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_convoke_multi_creature.txt` covers multiple creatures. |
| 702.51d | Convoke | IMPLEMENTED | Convoke extra-cost/reduction behavior is selected at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_convoke_multi_creature.txt` covers multiple creatures. |
| 702.52 | Dredge | IMPLEMENTED | `src/MTGAbility.cpp:2190-2194` installs the replacement and `src/AllAbilities.cpp:10644-10757` implements dredge. |
| 702.52a | Dredge | IMPLEMENTED | `src/MTGAbility.cpp:2190-2194` installs the replacement and `src/AllAbilities.cpp:10644-10757` implements dredge. |
| 702.52b | Dredge | IMPLEMENTED | `src/MTGAbility.cpp:2190-2194` installs the replacement and `src/AllAbilities.cpp:10644-10757` implements dredge. |
| 702.53 | Transmute | PARTIAL | Transmute is expressed card-by-card with hand discard/tutor scripts; no native keyword/linkage consumer exists, so grants and generic interactions are unsupported. |
| 702.53a | Transmute | PARTIAL | Transmute is expressed card-by-card with hand discard/tutor scripts; no native keyword/linkage consumer exists, so grants and generic interactions are unsupported. |
| 702.53b | Transmute | PARTIAL | Transmute is expressed card-by-card with hand discard/tutor scripts; no native keyword/linkage consumer exists, so grants and generic interactions are unsupported. |
| 702.54 | Bloodthirst | IMPLEMENTED | `src/MTGAbility.cpp:5418-5428` implements intervening damage condition and counters. |
| 702.54a | Bloodthirst | IMPLEMENTED | `src/MTGAbility.cpp:5418-5428` implements intervening damage condition and counters. |
| 702.54b | Bloodthirst | IMPLEMENTED | `src/MTGAbility.cpp:5418-5428` implements intervening damage condition and counters. |
| 702.54c | Bloodthirst | IMPLEMENTED | `src/MTGAbility.cpp:5418-5428` implements intervening damage condition and counters. |
| 702.55 | Haunt | PARTIAL | Haunt/haunted-event machinery exists at `src/MTGAbility.cpp:4150-4164` and `src/AllAbilities.cpp:2905-2944`, but full exile linkage/change-zone behavior is not established. |
| 702.55a | Haunt | PARTIAL | Haunt/haunted-event machinery exists at `src/MTGAbility.cpp:4150-4164` and `src/AllAbilities.cpp:2905-2944`, but full exile linkage/change-zone behavior is not established. |
| 702.55b | Haunt | PARTIAL | Haunt/haunted-event machinery exists at `src/MTGAbility.cpp:4150-4164` and `src/AllAbilities.cpp:2905-2944`, but full exile linkage/change-zone behavior is not established. |
| 702.55c | Haunt | PARTIAL | Haunt/haunted-event machinery exists at `src/MTGAbility.cpp:4150-4164` and `src/AllAbilities.cpp:2905-2944`, but full exile linkage/change-zone behavior is not established. |
| 702.56 | Replicate | IMPLEMENTED | Replicate uses repeatable kicker payment/counting in `src/MTGRules.cpp:568-583` and a replicate flag at `src/MTGDefinitions.cpp:231`; active tests cover copies. |
| 702.56a | Replicate | IMPLEMENTED | Replicate uses repeatable kicker payment/counting in `src/MTGRules.cpp:568-583` and a replicate flag at `src/MTGDefinitions.cpp:231`; active tests cover copies. |
| 702.56b | Replicate | IMPLEMENTED | Replicate uses repeatable kicker payment/counting in `src/MTGRules.cpp:568-583` and a replicate flag at `src/MTGDefinitions.cpp:231`; active tests cover copies. |
| 702.57 | Forecast | PARTIAL | Forecast is card-scripted with hand/upkeep/once restrictions; no native keyword identity or generic reveal-cost linkage was found. |
| 702.57a | Forecast | PARTIAL | Forecast is card-scripted with hand/upkeep/once restrictions; no native keyword identity or generic reveal-cost linkage was found. |
| 702.57b | Forecast | PARTIAL | Forecast is card-scripted with hand/upkeep/once restrictions; no native keyword identity or generic reveal-cost linkage was found. |
| 702.58 | Graft | PARTIAL | Graft is card-scripted with counters and move-counter triggers; no native multi-instance keyword consumer was found. |
| 702.58a | Graft | PARTIAL | Graft is card-scripted with counters and move-counter triggers; no native multi-instance keyword consumer was found. |
| 702.58b | Graft | PARTIAL | Graft is card-scripted with counters and move-counter triggers; no native multi-instance keyword consumer was found. |
| 702.59 | Recover | INCORRECT | Known catalogue defect: `_RECOVER_` parses (`_macros.txt:124-125`) but fails to return or exile the card; witness `bin/Res/test/lexicon/macro_recover.txt` is in `_known_failures.txt:31`. |
| 702.59a | Recover | INCORRECT | Known catalogue defect: `_RECOVER_` parses (`_macros.txt:124-125`) but fails to return or exile the card; witness `bin/Res/test/lexicon/macro_recover.txt` is in `_known_failures.txt:31`. |
| 702.60 | Ripple | PARTIAL | Ripple is a reveal/options macro at `_macros.txt:121-122`; generic reveal behavior parses, but no focused behavioral regression establishes every reveal/cast/bottom step. |
| 702.60a | Ripple | PARTIAL | Ripple is a reveal/options macro at `_macros.txt:121-122`; generic reveal behavior parses, but no focused behavioral regression establishes every reveal/cast/bottom step. |
| 702.60b | Ripple | PARTIAL | Ripple is a reveal/options macro at `_macros.txt:121-122`; generic reveal behavior parses, but no focused behavioral regression establishes every reveal/cast/bottom step. |
| 702.61 | Split Second | INCORRECT | `src/ActionStack.cpp:1144-1151` immediately resolves a split-second stack object, wrongly suppressing allowed mana abilities/special actions and bypassing normal priority rather than selectively forbidding casts/nonmana activations. |
| 702.61a | Split Second | INCORRECT | `src/ActionStack.cpp:1144-1151` immediately resolves a split-second stack object, wrongly suppressing allowed mana abilities/special actions and bypassing normal priority rather than selectively forbidding casts/nonmana activations. |
| 702.61b | Split Second | PARTIAL | `src/ActionStack.cpp:1144-1151` immediately resolves a split-second stack object, wrongly suppressing allowed mana abilities/special actions and bypassing normal priority rather than selectively forbidding casts/nonmana activations. |
| 702.61c | Split Second | INCORRECT | `src/ActionStack.cpp:1144-1151` immediately resolves a split-second stack object, wrongly suppressing allowed mana abilities/special actions and bypassing normal priority rather than selectively forbidding casts/nonmana activations. |
| 702.62 | Suspend | PARTIAL | Suspend/time-counter/cast machinery exists at `src/MTGRules.cpp:1451-1573`; chooser-based activation remains documented as undrivable by the scripted suite. |
| 702.62a | Suspend | PARTIAL | Suspend/time-counter/cast machinery exists at `src/MTGRules.cpp:1451-1573`; chooser-based activation remains documented as undrivable by the scripted suite. |
| 702.62b | Suspend | PARTIAL | Suspend/time-counter/cast machinery exists at `src/MTGRules.cpp:1451-1573`; chooser-based activation remains documented as undrivable by the scripted suite. |
| 702.62c | Suspend | PARTIAL | Suspend/time-counter/cast machinery exists at `src/MTGRules.cpp:1451-1573`; chooser-based activation remains documented as undrivable by the scripted suite. |
| 702.62d | Suspend | PARTIAL | Suspend/time-counter/cast machinery exists at `src/MTGRules.cpp:1451-1573`; chooser-based activation remains documented as undrivable by the scripted suite. |
| 702.63 | Vanishing | IMPLEMENTED | Vanishing/time counters are implemented by `src/MTGAbility.cpp:5431-5441` and `src/AllAbilities.cpp:9449-9517`. |
| 702.63a | Vanishing | IMPLEMENTED | Vanishing/time counters are implemented by `src/MTGAbility.cpp:5431-5441` and `src/AllAbilities.cpp:9449-9517`. |
| 702.63b | Vanishing | IMPLEMENTED | Vanishing/time counters are implemented by `src/MTGAbility.cpp:5431-5441` and `src/AllAbilities.cpp:9449-9517`. |
| 702.63c | Vanishing | IMPLEMENTED | Vanishing/time counters are implemented by `src/MTGAbility.cpp:5431-5441` and `src/AllAbilities.cpp:9449-9517`. |
| 702.64 | Absorb | PARTIAL | Damage subtracts stored absorb instances at `src/Damage.cpp:101-104`; parser `src/MTGAbility.cpp:6002-6016` only evidences unit increments, not a general printed Absorb N. |
| 702.64a | Absorb | PARTIAL | Damage subtracts stored absorb instances at `src/Damage.cpp:101-104`; parser `src/MTGAbility.cpp:6002-6016` only evidences unit increments, not a general printed Absorb N. |
| 702.64b | Absorb | PARTIAL | Damage subtracts stored absorb instances at `src/Damage.cpp:101-104`; parser `src/MTGAbility.cpp:6002-6016` only evidences unit increments, not a general printed Absorb N. |
| 702.64c | Absorb | PARTIAL | Damage subtracts stored absorb instances at `src/Damage.cpp:101-104`; parser `src/MTGAbility.cpp:6002-6016` only evidences unit increments, not a general printed Absorb N. |
| 702.65 | Aura Swap | UNIMPLEMENTED | Named search found no aura-swap parser/behavior in the five requested keyword surfaces and no primitive grammar use. |
| 702.65a | Aura Swap | UNIMPLEMENTED | Named search found no aura-swap parser/behavior in the five requested keyword surfaces and no primitive grammar use. |
| 702.65b | Aura Swap | UNIMPLEMENTED | Named search found no aura-swap parser/behavior in the five requested keyword surfaces and no primitive grammar use. |
| 702.66 | Delve | IMPLEMENTED | Delve optional reduction is wired at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_delve_multi_card.txt` covers multiple exiles. |
| 702.66a | Delve | IMPLEMENTED | Delve optional reduction is wired at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_delve_multi_card.txt` covers multiple exiles. |
| 702.66b | Delve | IMPLEMENTED | Delve optional reduction is wired at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_delve_multi_card.txt` covers multiple exiles. |
| 702.66c | Delve | IMPLEMENTED | Delve optional reduction is wired at `src/MTGRules.cpp:978-992`; `bin/Res/test/lexicon/cost_delve_multi_card.txt` covers multiple exiles. |
| 702.67 | Fortify | PARTIAL | Fortification attachment shares equipment machinery (`src/AllAbilities.cpp:6171-6210`), but no complete fortify timing/target regression was found. |
| 702.67a | Fortify | PARTIAL | Fortification attachment shares equipment machinery (`src/AllAbilities.cpp:6171-6210`), but no complete fortify timing/target regression was found. |
| 702.67b | Fortify | PARTIAL | Fortification attachment shares equipment machinery (`src/AllAbilities.cpp:6171-6210`), but no complete fortify timing/target regression was found. |
| 702.67c | Fortify | PARTIAL | Fortification attachment shares equipment machinery (`src/AllAbilities.cpp:6171-6210`), but no complete fortify timing/target regression was found. |
| 702.68 | Frenzy | PARTIAL | Frenzy is expanded into attack/unblocked pump scripts on cards; no native keyword identity/multiple-instance consumer was found. |
| 702.68a | Frenzy | PARTIAL | Frenzy is expanded into attack/unblocked pump scripts on cards; no native keyword identity/multiple-instance consumer was found. |
| 702.68b | Frenzy | PARTIAL | Frenzy is expanded into attack/unblocked pump scripts on cards; no native keyword identity/multiple-instance consumer was found. |
| 702.69 | Gravestorm | UNIMPLEMENTED | Named search found no gravestorm parser/behavior in the requested C++ surfaces; primitive occurrences are text only. |
| 702.69a | Gravestorm | UNIMPLEMENTED | Named search found no gravestorm parser/behavior in the requested C++ surfaces; primitive occurrences are text only. |
| 702.69b | Gravestorm | UNIMPLEMENTED | Named search found no gravestorm parser/behavior in the requested C++ surfaces; primitive occurrences are text only. |
| 702.70 | Poisonous | UNIMPLEMENTED | Named search found no Poisonous trigger. `src/Damage.cpp:192`'s `POISONDAMAGER` replaces normal damage and is not 702.70 behavior. |
| 702.70a | Poisonous | UNIMPLEMENTED | Named search found no Poisonous trigger. `src/Damage.cpp:192`'s `POISONDAMAGER` replaces normal damage and is not 702.70 behavior. |
| 702.70b | Poisonous | UNIMPLEMENTED | Named search found no Poisonous trigger. `src/Damage.cpp:192`'s `POISONDAMAGER` replaces normal damage and is not 702.70 behavior. |
| 702.71 | Transfigure | PARTIAL | Transfigure is hand-scripted on individual cards; there is no native sacrifice/tutor/mana-value keyword consumer. |
| 702.71a | Transfigure | PARTIAL | Transfigure is hand-scripted on individual cards; there is no native sacrifice/tutor/mana-value keyword consumer. |
| 702.72 | Champion | UNIMPLEMENTED | `_macros.txt:106-107` explicitly labels Champion “not implemented” and does not implement the leave-return linked object. |
| 702.72a | Champion | UNIMPLEMENTED | `_macros.txt:106-107` explicitly labels Champion “not implemented” and does not implement the leave-return linked object. |
| 702.72b | Champion | UNIMPLEMENTED | `_macros.txt:106-107` explicitly labels Champion “not implemented” and does not implement the leave-return linked object. |
| 702.72c | Champion | UNIMPLEMENTED | `_macros.txt:106-107` explicitly labels Champion “not implemented” and does not implement the leave-return linked object. |
| 702.73 | Changeling | IMPLEMENTED | `src/MTGDefinitions.cpp:89` stores changeling continuously; `bin/Res/test/lexicon/basic_changeling_is_goblin_for_lord.txt` covers all-type matching. |
| 702.73a | Changeling | IMPLEMENTED | `src/MTGDefinitions.cpp:89` stores changeling continuously; `bin/Res/test/lexicon/basic_changeling_is_goblin_for_lord.txt` covers all-type matching. |
| 702.74 | Evoke | IMPLEMENTED | Evoke sacrifice timing is installed at `src/MTGAbility.cpp:6779-6797`; `bin/Res/test/evoke_flicker_i1129.txt` covers intervening-zone behavior. |
| 702.74a | Evoke | IMPLEMENTED | Evoke sacrifice timing is installed at `src/MTGAbility.cpp:6779-6797`; `bin/Res/test/evoke_flicker_i1129.txt` covers intervening-zone behavior. |
| 702.75 | Hideaway | PARTIAL | Hideaway is reproduced with reveal/exile/cast scripts, but linked face-down-card and Hideaway-N behavior is not native. |
| 702.75a | Hideaway | PARTIAL | Hideaway is reproduced with reveal/exile/cast scripts, but linked face-down-card and Hideaway-N behavior is not native. |
| 702.75b | Hideaway | PARTIAL | Hideaway is reproduced with reveal/exile/cast scripts, but linked face-down-card and Hideaway-N behavior is not native. |
| 702.76 | Prowl | PARTIAL | Prowl uses alternate-cost/combat-damage conditions (`src/MTGAbility.cpp:657-676`); generic quality linkage is incomplete. |
| 702.76a | Prowl | PARTIAL | Prowl uses alternate-cost/combat-damage conditions (`src/MTGAbility.cpp:657-676`); generic quality linkage is incomplete. |
| 702.77 | Reinforce | PARTIAL | Reinforce is hand-scripted as hand discard/counters; no native keyword identity exists. |
| 702.77a | Reinforce | PARTIAL | Reinforce is hand-scripted as hand discard/counters; no native keyword identity exists. |
| 702.77b | Reinforce | PARTIAL | Reinforce is hand-scripted as hand discard/counters; no native keyword identity exists. |
| 702.78 | Conspire | PARTIAL | Conspire is tap/copy scripting; no native per-instance optional-cost/trigger linkage was found. |
| 702.78a | Conspire | PARTIAL | Conspire is tap/copy scripting; no native per-instance optional-cost/trigger linkage was found. |
| 702.78b | Conspire | PARTIAL | Conspire is tap/copy scripting; no native per-instance optional-cost/trigger linkage was found. |
| 702.79 | Persist | IMPLEMENTED | Persist condition/return/counter behavior is implemented at `src/MTGRules.cpp:3380-3432`. |
| 702.79a | Persist | IMPLEMENTED | Persist condition/return/counter behavior is implemented at `src/MTGRules.cpp:3380-3432`. |
| 702.80 | Wither | IMPLEMENTED | Wither converts damage to -1/-1 counters at `src/Damage.cpp:139-190`; boolean storage makes instances redundant. |
| 702.80a | Wither | IMPLEMENTED | Wither converts damage to -1/-1 counters at `src/Damage.cpp:139-190`; boolean storage makes instances redundant. |
| 702.80b | Wither | IMPLEMENTED | Wither converts damage to -1/-1 counters at `src/Damage.cpp:139-190`; boolean storage makes instances redundant. |
| 702.80c | Wither | IMPLEMENTED | Wither converts damage to -1/-1 counters at `src/Damage.cpp:139-190`; boolean storage makes instances redundant. |
| 702.80d | Wither | IMPLEMENTED | Wither converts damage to -1/-1 counters at `src/Damage.cpp:139-190`; boolean storage makes instances redundant. |
| 702.81 | Retrace | IMPLEMENTED | Retrace casting and land discard are implemented at `src/MTGRules.cpp:1385-1450`. |
| 702.81a | Retrace | IMPLEMENTED | Retrace casting and land discard are implemented at `src/MTGRules.cpp:1385-1450`. |
| 702.82 | Devour | PARTIAL | Devour is per-card sacrifice/counter scripting; generic devoured-object linkage is absent. |
| 702.82a | Devour | PARTIAL | Devour is per-card sacrifice/counter scripting; generic devoured-object linkage is absent. |
| 702.82b | Devour | PARTIAL | Devour is per-card sacrifice/counter scripting; generic devoured-object linkage is absent. |
| 702.82c | Devour | PARTIAL | Devour is per-card sacrifice/counter scripting; generic devoured-object linkage is absent. |
| 702.83 | Exalted | IMPLEMENTED | `src/MTGAbility.cpp:7321-7333` installs exalted and supports separate instances. |
| 702.83a | Exalted | IMPLEMENTED | `src/MTGAbility.cpp:7321-7333` installs exalted and supports separate instances. |
| 702.83b | Exalted | IMPLEMENTED | `src/MTGAbility.cpp:7321-7333` installs exalted and supports separate instances. |
| 702.84 | Unearth | IMPLEMENTED | Unearth move/haste/exile cleanup uses `_macros.txt:187-188` and `src/MTGRules.cpp:3531-3629`. |
| 702.84a | Unearth | IMPLEMENTED | Unearth move/haste/exile cleanup uses `_macros.txt:187-188` and `src/MTGRules.cpp:3531-3629`. |
| 702.85 | Cascade | PARTIAL | Cascade machinery is at `src/MTGAbility.cpp:4984-4992` and `src/AllAbilities.cpp:2345-2467`; DFC/mana-value edges remain ambiguous. |
| 702.85a | Cascade | PARTIAL | Cascade machinery is at `src/MTGAbility.cpp:4984-4992` and `src/AllAbilities.cpp:2345-2467`; DFC/mana-value edges remain ambiguous. |
| 702.85b | Cascade | PARTIAL | Cascade machinery is at `src/MTGAbility.cpp:4984-4992` and `src/AllAbilities.cpp:2345-2467`; DFC/mana-value edges remain ambiguous. |
| 702.85c | Cascade | PARTIAL | Cascade machinery is at `src/MTGAbility.cpp:4984-4992` and `src/AllAbilities.cpp:2345-2467`; DFC/mana-value edges remain ambiguous. |
| 702.86 | Annihilator | PARTIAL | Annihilator is per-card attack/sacrifice scripting; no native defending-entity generalization was found. |
| 702.86a | Annihilator | PARTIAL | Annihilator is per-card attack/sacrifice scripting; no native defending-entity generalization was found. |
| 702.86b | Annihilator | PARTIAL | Annihilator is per-card attack/sacrifice scripting; no native defending-entity generalization was found. |
| 702.87 | Level Up | PARTIAL | Level counters/range lords exist (`src/AllAbilities.cpp:3165`), but level-up is card-scripted rather than generic. |
| 702.87a | Level Up | PARTIAL | Level counters/range lords exist (`src/AllAbilities.cpp:3165`), but level-up is card-scripted rather than generic. |
| 702.87b | Level Up | PARTIAL | Level counters/range lords exist (`src/AllAbilities.cpp:3165`), but level-up is card-scripted rather than generic. |
| 702.87c | Level Up | PARTIAL | Level counters/range lords exist (`src/AllAbilities.cpp:3165`), but level-up is card-scripted rather than generic. |
| 702.88 | Rebound | IMPLEMENTED | Rebound is defined at `_macros.txt:27-28` and triggered by `src/MTGAbility.cpp:2010-2015`; active regressions cover recast. |
| 702.88a | Rebound | IMPLEMENTED | Rebound is defined at `_macros.txt:27-28` and triggered by `src/MTGAbility.cpp:2010-2015`; active regressions cover recast. |
| 702.88b | Rebound | IMPLEMENTED | Rebound is defined at `_macros.txt:27-28` and triggered by `src/MTGAbility.cpp:2010-2015`; active regressions cover recast. |
| 702.88c | Rebound | IMPLEMENTED | Rebound is defined at `_macros.txt:27-28` and triggered by `src/MTGAbility.cpp:2010-2015`; active regressions cover recast. |
| 702.89 | Umbra Armor | IMPLEMENTED | Active `basic_totemarmor_saves_creature_and_destroys_aura.txt` verifies saving and consuming the Aura. |
| 702.89a | Umbra Armor | IMPLEMENTED | Active `basic_totemarmor_saves_creature_and_destroys_aura.txt` verifies saving and consuming the Aura. |
| 702.89b | Umbra Armor | IMPLEMENTED | Active `basic_totemarmor_saves_creature_and_destroys_aura.txt` verifies saving and consuming the Aura. |
| 702.90 | Infect | IMPLEMENTED | Infect player/creature conversion is implemented in `src/Damage.cpp:139-200` with LKI flags. |
| 702.90a | Infect | IMPLEMENTED | Infect player/creature conversion is implemented in `src/Damage.cpp:139-200` with LKI flags. |
| 702.90b | Infect | IMPLEMENTED | Infect player/creature conversion is implemented in `src/Damage.cpp:139-200` with LKI flags. |
| 702.90c | Infect | IMPLEMENTED | Infect player/creature conversion is implemented in `src/Damage.cpp:139-200` with LKI flags. |
| 702.90d | Infect | IMPLEMENTED | Infect player/creature conversion is implemented in `src/Damage.cpp:139-200` with LKI flags. |
| 702.90e | Infect | IMPLEMENTED | Infect player/creature conversion is implemented in `src/Damage.cpp:139-200` with LKI flags. |
| 702.90f | Infect | IMPLEMENTED | Infect player/creature conversion is implemented in `src/Damage.cpp:139-200` with LKI flags. |
| 702.91 | Battle Cry | PARTIAL | Battle cry is per-card attack-trigger scripting; keyword grants/multiple instances lack a native consumer. |
| 702.91a | Battle Cry | PARTIAL | Battle cry is per-card attack-trigger scripting; keyword grants/multiple instances lack a native consumer. |
| 702.91b | Battle Cry | PARTIAL | Battle cry is per-card attack-trigger scripting; keyword grants/multiple instances lack a native consumer. |
| 702.92 | Living Weapon | IMPLEMENTED | `src/MTGAbility.cpp:3775-3818` implements token creation/attachment; `bin/Res/test/livingweapon.txt` covers it. |
| 702.92a | Living Weapon | IMPLEMENTED | `src/MTGAbility.cpp:3775-3818` implements token creation/attachment; `bin/Res/test/livingweapon.txt` covers it. |
| 702.93 | Undying | IMPLEMENTED | Undying condition/return/counter behavior is at `src/MTGRules.cpp:3395-3432`. |
| 702.93a | Undying | IMPLEMENTED | Undying condition/return/counter behavior is at `src/MTGRules.cpp:3395-3432`. |
| 702.94 | Miracle | PARTIAL | Miracle casting is parsed at `src/MTGAbility.cpp:641-676`, but reveal-duration/linkage is not fully evidenced. |
| 702.94a | Miracle | PARTIAL | Miracle casting is parsed at `src/MTGAbility.cpp:641-676`, but reveal-duration/linkage is not fully evidenced. |
| 702.94b | Miracle | PARTIAL | Miracle casting is parsed at `src/MTGAbility.cpp:641-676`, but reveal-duration/linkage is not fully evidenced. |
| 702.95 | Soulbond | IMPLEMENTED | Soulbond pairing/teardown are at `src/MTGRules.cpp:3164-3247`; active pairing regression passes. |
| 702.95a | Soulbond | IMPLEMENTED | Soulbond pairing/teardown are at `src/MTGRules.cpp:3164-3247`; active pairing regression passes. |
| 702.95b | Soulbond | IMPLEMENTED | Soulbond pairing/teardown are at `src/MTGRules.cpp:3164-3247`; active pairing regression passes. |
| 702.95c | Soulbond | IMPLEMENTED | Soulbond pairing/teardown are at `src/MTGRules.cpp:3164-3247`; active pairing regression passes. |
| 702.95d | Soulbond | IMPLEMENTED | Soulbond pairing/teardown are at `src/MTGRules.cpp:3164-3247`; active pairing regression passes. |
| 702.95e | Soulbond | IMPLEMENTED | Soulbond pairing/teardown are at `src/MTGRules.cpp:3164-3247`; active pairing regression passes. |
| 702.96 | Overload | PARTIAL | Overload paths exist at `src/MTGRules.cpp:883-934,1799`; scripted targeting substitutes for a general text-change layer. |
| 702.96a | Overload | PARTIAL | Overload paths exist at `src/MTGRules.cpp:883-934,1799`; scripted targeting substitutes for a general text-change layer. |
| 702.96b | Overload | PARTIAL | Overload paths exist at `src/MTGRules.cpp:883-934,1799`; scripted targeting substitutes for a general text-change layer. |
| 702.96c | Overload | PARTIAL | Overload paths exist at `src/MTGRules.cpp:883-934,1799`; scripted targeting substitutes for a general text-change layer. |
| 702.97 | Scavenge | PARTIAL | Scavenge uses `_macros.txt:130-131` plus card `{E}` costs; generic power/LKI edges remain script-dependent. |
| 702.97a | Scavenge | PARTIAL | Scavenge uses `_macros.txt:130-131` plus card `{E}` costs; generic power/LKI edges remain script-dependent. |
| 702.98 | Unleash | PARTIAL | Unleash is per-card ETB choice/counter/block scripting; no native keyword consumer exists. |
| 702.98a | Unleash | PARTIAL | Unleash is per-card ETB choice/counter/block scripting; no native keyword consumer exists. |
| 702.99 | Cipher | PARTIAL | Cipher is per-card encoded scripting; generic linkage through control/type changes is not modeled. |
| 702.99a | Cipher | PARTIAL | Cipher is per-card encoded scripting; generic linkage through control/type changes is not modeled. |
| 702.99b | Cipher | PARTIAL | Cipher is per-card encoded scripting; generic linkage through control/type changes is not modeled. |
| 702.99c | Cipher | PARTIAL | Cipher is per-card encoded scripting; generic linkage through control/type changes is not modeled. |
| 702.100 | Evolve | IMPLEMENTED | `src/MTGAbility.cpp:5119-5122` installs evolve; `bin/Res/test/evolve.txt` covers it. |
| 702.100a | Evolve | IMPLEMENTED | `src/MTGAbility.cpp:5119-5122` installs evolve; `bin/Res/test/evolve.txt` covers it. |
| 702.100b | Evolve | IMPLEMENTED | `src/MTGAbility.cpp:5119-5122` installs evolve; `bin/Res/test/evolve.txt` covers it. |
| 702.100c | Evolve | IMPLEMENTED | `src/MTGAbility.cpp:5119-5122` installs evolve; `bin/Res/test/evolve.txt` covers it. |
| 702.100d | Evolve | IMPLEMENTED | `src/MTGAbility.cpp:5119-5122` installs evolve; `bin/Res/test/evolve.txt` covers it. |
| 702.101 | Extort | IMPLEMENTED | `_macros.txt:172-173` implements extort payment/loss/gain; active regression is green. |
| 702.101a | Extort | IMPLEMENTED | `_macros.txt:172-173` implements extort payment/loss/gain; active regression is green. |
| 702.101b | Extort | IMPLEMENTED | `_macros.txt:172-173` implements extort payment/loss/gain; active regression is green. |
| 702.102 | Fuse | PARTIAL | Fuse uses `HASNOKICKER`/combined scripts (`src/MTGRules.cpp:571-582`); combined characteristics/order are not native. |
| 702.102a | Fuse | PARTIAL | Fuse uses `HASNOKICKER`/combined scripts (`src/MTGRules.cpp:571-582`); combined characteristics/order are not native. |
| 702.102b | Fuse | PARTIAL | Fuse uses `HASNOKICKER`/combined scripts (`src/MTGRules.cpp:571-582`); combined characteristics/order are not native. |
| 702.102c | Fuse | PARTIAL | Fuse uses `HASNOKICKER`/combined scripts (`src/MTGRules.cpp:571-582`); combined characteristics/order are not native. |
| 702.102d | Fuse | PARTIAL | Fuse uses `HASNOKICKER`/combined scripts (`src/MTGRules.cpp:571-582`); combined characteristics/order are not native. |
| 702.103 | Bestow | PARTIAL | Bestow paths exist at `src/MTGRules.cpp:587-598` and `src/AllAbilities.cpp:6272-6328`; copy/phasing clauses are incomplete. |
| 702.103a | Bestow | PARTIAL | Bestow paths exist at `src/MTGRules.cpp:587-598` and `src/AllAbilities.cpp:6272-6328`; copy/phasing clauses are incomplete. |
| 702.103b | Bestow | PARTIAL | Bestow paths exist at `src/MTGRules.cpp:587-598` and `src/AllAbilities.cpp:6272-6328`; copy/phasing clauses are incomplete. |
| 702.103c | Bestow | PARTIAL | Bestow paths exist at `src/MTGRules.cpp:587-598` and `src/AllAbilities.cpp:6272-6328`; copy/phasing clauses are incomplete. |
| 702.103d | Bestow | PARTIAL | Bestow paths exist at `src/MTGRules.cpp:587-598` and `src/AllAbilities.cpp:6272-6328`; copy/phasing clauses are incomplete. |
| 702.103e | Bestow | PARTIAL | Bestow paths exist at `src/MTGRules.cpp:587-598` and `src/AllAbilities.cpp:6272-6328`; copy/phasing clauses are incomplete. |
| 702.103f | Bestow | PARTIAL | Bestow paths exist at `src/MTGRules.cpp:587-598` and `src/AllAbilities.cpp:6272-6328`; copy/phasing clauses are incomplete. |
| 702.103g | Bestow | PARTIAL | Bestow paths exist at `src/MTGRules.cpp:587-598` and `src/AllAbilities.cpp:6272-6328`; copy/phasing clauses are incomplete. |
| 702.104 | Tribute | PARTIAL | Tribute choice/counters exist at `src/AllAbilities.cpp:6275-6328`; linked unpaid coverage is incomplete. |
| 702.104a | Tribute | PARTIAL | Tribute choice/counters exist at `src/AllAbilities.cpp:6275-6328`; linked unpaid coverage is incomplete. |
| 702.104b | Tribute | PARTIAL | Tribute choice/counters exist at `src/AllAbilities.cpp:6275-6328`; linked unpaid coverage is incomplete. |
| 702.105 | Dethrone | IMPLEMENTED | `src/MTGRules.cpp:2139-2143` checks greatest life and adds the counter; active regression covers ties. |
| 702.105a | Dethrone | IMPLEMENTED | `src/MTGRules.cpp:2139-2143` checks greatest life and adds the counter; active regression covers ties. |
| 702.105b | Dethrone | IMPLEMENTED | `src/MTGRules.cpp:2139-2143` checks greatest life and adds the counter; active regression covers ties. |
| 702.106 | Hidden Agenda | N/A | N/A: hidden agenda/double agenda are Conspiracy command-zone/secret physical procedures, excluded by the audit's supported-game scope. |
| 702.106a | Hidden Agenda | N/A | N/A: hidden agenda/double agenda are Conspiracy command-zone/secret physical procedures, excluded by the audit's supported-game scope. |
| 702.106b | Hidden Agenda | N/A | N/A: hidden agenda/double agenda are Conspiracy command-zone/secret physical procedures, excluded by the audit's supported-game scope. |
| 702.106c | Hidden Agenda | N/A | N/A: hidden agenda/double agenda are Conspiracy command-zone/secret physical procedures, excluded by the audit's supported-game scope. |
| 702.106d | Hidden Agenda | N/A | N/A: hidden agenda/double agenda are Conspiracy command-zone/secret physical procedures, excluded by the audit's supported-game scope. |
| 702.106e | Hidden Agenda | N/A | N/A: hidden agenda/double agenda are Conspiracy command-zone/secret physical procedures, excluded by the audit's supported-game scope. |
| 702.106f | Hidden Agenda | N/A | N/A: hidden agenda/double agenda are Conspiracy command-zone/secret physical procedures, excluded by the audit's supported-game scope. |
| 702.107 | Outlast | PARTIAL | Outlast is represented by sorcery-speed tap/counter activated scripts; no native keyword identity, so generic grants/interactions are partial. |
| 702.107a | Outlast | PARTIAL | Outlast is represented by sorcery-speed tap/counter activated scripts; no native keyword identity, so generic grants/interactions are partial. |
| 702.108 | Prowess | PARTIAL | Cards pair `abilities=prowess` with explicit noncreature-cast pump scripts (for example `primitives/mtg.txt:70-71`); the parsed keyword name alone has no behavior consumer. |
| 702.108a | Prowess | PARTIAL | Cards pair `abilities=prowess` with explicit noncreature-cast pump scripts (for example `primitives/mtg.txt:70-71`); the parsed keyword name alone has no behavior consumer. |
| 702.108b | Prowess | PARTIAL | Cards pair `abilities=prowess` with explicit noncreature-cast pump scripts (for example `primitives/mtg.txt:70-71`); the parsed keyword name alone has no behavior consumer. |
| 702.109 | Dash | PARTIAL | Dash is card-scripted using alternate cost, haste, and end-step return; generic paid-method linkage and multiple delayed effects are incomplete. |
| 702.109a | Dash | PARTIAL | Dash is card-scripted using alternate cost, haste, and end-step return; generic paid-method linkage and multiple delayed effects are incomplete. |
| 702.110 | Exploit | IMPLEMENTED | Exploit sacrifice/event machinery is at `src/MTGAbility.cpp:1701-1704,4357-4363`; active `effect_exploits_fires_exploited_trigger.txt` verifies the corrected behavior. |
| 702.110a | Exploit | IMPLEMENTED | Exploit sacrifice/event machinery is at `src/MTGAbility.cpp:1701-1704,4357-4363`; active `effect_exploits_fires_exploited_trigger.txt` verifies the corrected behavior. |
| 702.110b | Exploit | IMPLEMENTED | Exploit sacrifice/event machinery is at `src/MTGAbility.cpp:1701-1704,4357-4363`; active `effect_exploits_fires_exploited_trigger.txt` verifies the corrected behavior. |
| 702.111 | Menace | IMPLEMENTED | `src/MTGRules.cpp:2582-2591` requires at least two blockers; boolean storage makes instances redundant. |
| 702.111a | Menace | IMPLEMENTED | `src/MTGRules.cpp:2582-2591` requires at least two blockers; boolean storage makes instances redundant. |
| 702.111b | Menace | IMPLEMENTED | `src/MTGRules.cpp:2582-2591` requires at least two blockers; boolean storage makes instances redundant. |
| 702.111c | Menace | IMPLEMENTED | `src/MTGRules.cpp:2582-2591` requires at least two blockers; boolean storage makes instances redundant. |
| 702.112 | Renown | IMPLEMENTED | `_macros.txt:203-204` implements combat-damage counters and a persistent renowned designation; active renown regression covers it. |
| 702.112a | Renown | IMPLEMENTED | `_macros.txt:203-204` implements combat-damage counters and a persistent renowned designation; active renown regression covers it. |
| 702.112b | Renown | IMPLEMENTED | `_macros.txt:203-204` implements combat-damage counters and a persistent renowned designation; active renown regression covers it. |
| 702.112c | Renown | IMPLEMENTED | `_macros.txt:203-204` implements combat-damage counters and a persistent renowned designation; active renown regression covers it. |
| 702.113 | Awaken | PARTIAL | Awaken is encoded through alternate-cost and land-transform scripts; conditional targeting is card-specific rather than native linked keyword behavior. |
| 702.113a | Awaken | PARTIAL | Awaken is encoded through alternate-cost and land-transform scripts; conditional targeting is card-specific rather than native linked keyword behavior. |
| 702.113b | Awaken | PARTIAL | Awaken is encoded through alternate-cost and land-transform scripts; conditional targeting is card-specific rather than native linked keyword behavior. |
| 702.114 | Devoid | IMPLEMENTED | `src/MTGDefinitions.cpp:157` stores devoid in all zones; `basic_devoid_is_colorless_for_protection.txt` verifies colorlessness. |
| 702.114a | Devoid | IMPLEMENTED | `src/MTGDefinitions.cpp:157` stores devoid in all zones; `basic_devoid_is_colorless_for_protection.txt` verifies colorlessness. |
| 702.115 | Ingest | IMPLEMENTED | `src/MTGAbility.cpp:4962-4977` implements combat-damage library exile; active ingest regression covers the move. |
| 702.115a | Ingest | IMPLEMENTED | `src/MTGAbility.cpp:4962-4977` implements combat-damage library exile; active ingest regression covers the move. |
| 702.115b | Ingest | IMPLEMENTED | `src/MTGAbility.cpp:4962-4977` implements combat-damage library exile; active ingest regression covers the move. |
| 702.116 | Myriad | N/A | N/A: myriad's additional-opponent token creation has no operative opponent beyond the defender in supported two-player games. |
| 702.116a | Myriad | N/A | N/A: myriad's additional-opponent token creation has no operative opponent beyond the defender in supported two-player games. |
| 702.116b | Myriad | N/A | N/A: myriad's additional-opponent token creation has no operative opponent beyond the defender in supported two-player games. |
| 702.117 | Surge | PARTIAL | Surge is card-scripted using an alternate cost and prior-spell restriction; teammate language is excluded and generic alternative-cost linkage remains partial. |
| 702.117a | Surge | PARTIAL | Surge is card-scripted using an alternate cost and prior-spell restriction; teammate language is excluded and generic alternative-cost linkage remains partial. |
| 702.118 | Skulk | IMPLEMENTED | `src/MTGDefinitions.cpp:162` plus blocker legality implements skulk; active greater/equal-power regressions cover the comparison. |
| 702.118a | Skulk | IMPLEMENTED | `src/MTGDefinitions.cpp:162` plus blocker legality implements skulk; active greater/equal-power regressions cover the comparison. |
| 702.118b | Skulk | IMPLEMENTED | `src/MTGDefinitions.cpp:162` plus blocker legality implements skulk; active greater/equal-power regressions cover the comparison. |
| 702.118c | Skulk | IMPLEMENTED | `src/MTGDefinitions.cpp:162` plus blocker legality implements skulk; active greater/equal-power regressions cover the comparison. |
| 702.119 | Emerge | PARTIAL | Emerge sacrifice/reduction is implemented at `src/MTGRules.cpp:934-999,1132`; 'emerge from quality' and all cost-choice edges are not comprehensively evidenced. |
| 702.119a | Emerge | PARTIAL | Emerge sacrifice/reduction is implemented at `src/MTGRules.cpp:934-999,1132`; 'emerge from quality' and all cost-choice edges are not comprehensively evidenced. |
| 702.119b | Emerge | PARTIAL | Emerge sacrifice/reduction is implemented at `src/MTGRules.cpp:934-999,1132`; 'emerge from quality' and all cost-choice edges are not comprehensively evidenced. |
| 702.119c | Emerge | PARTIAL | Emerge sacrifice/reduction is implemented at `src/MTGRules.cpp:934-999,1132`; 'emerge from quality' and all cost-choice edges are not comprehensively evidenced. |
| 702.120 | Escalate | PARTIAL | Escalate is encoded as modal/card-specific additional costs; no generic per-extra-mode keyword consumer was found. |
| 702.120a | Escalate | PARTIAL | Escalate is encoded as modal/card-specific additional costs; no generic per-extra-mode keyword consumer was found. |
| 702.121 | Melee | N/A | N/A: melee's varying count is a multiplayer mechanic; supported two-player games cannot attack more than one opponent. |
| 702.121a | Melee | N/A | N/A: melee's varying count is a multiplayer mechanic; supported two-player games cannot attack more than one opponent. |
| 702.121b | Melee | N/A | N/A: melee's varying count is a multiplayer mechanic; supported two-player games cannot attack more than one opponent. |
| 702.122 | Crew | PARTIAL | Crew costs/vehicle conversion exist (`src/CardPrimitive.cpp:368-376`; `_macros.txt:250-254`), but reusable macros enumerate narrow one/two-creature patterns rather than arbitrary total-power selections. |
| 702.122a | Crew | PARTIAL | Crew costs/vehicle conversion exist (`src/CardPrimitive.cpp:368-376`; `_macros.txt:250-254`), but reusable macros enumerate narrow one/two-creature patterns rather than arbitrary total-power selections. |
| 702.122b | Crew | PARTIAL | Crew costs/vehicle conversion exist (`src/CardPrimitive.cpp:368-376`; `_macros.txt:250-254`), but reusable macros enumerate narrow one/two-creature patterns rather than arbitrary total-power selections. |
| 702.122c | Crew | PARTIAL | Crew costs/vehicle conversion exist (`src/CardPrimitive.cpp:368-376`; `_macros.txt:250-254`), but reusable macros enumerate narrow one/two-creature patterns rather than arbitrary total-power selections. |
| 702.122d | Crew | PARTIAL | Crew costs/vehicle conversion exist (`src/CardPrimitive.cpp:368-376`; `_macros.txt:250-254`), but reusable macros enumerate narrow one/two-creature patterns rather than arbitrary total-power selections. |
| 702.123 | Fabricate | IMPLEMENTED | `_macros.txt:88-89` implements the ETB choice; active fabricate regression covers counters versus Servo tokens. |
| 702.123a | Fabricate | IMPLEMENTED | `_macros.txt:88-89` implements the ETB choice; active fabricate regression covers counters versus Servo tokens. |
| 702.123b | Fabricate | IMPLEMENTED | `_macros.txt:88-89` implements the ETB choice; active fabricate regression covers counters versus Servo tokens. |
| 702.124 | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124a | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124b | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124c | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124d | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124e | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124f | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124g | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124h | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124i | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124j | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124k | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124m | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.124n | Partner | N/A | N/A: partner variants here govern Commander deck construction/command-zone procedure, excluded game types. |
| 702.125 | Undaunted | N/A | N/A: undaunted varies only with number of opponents; multiplayer is excluded and its two-player reduction is always zero. |
| 702.125a | Undaunted | N/A | N/A: undaunted varies only with number of opponents; multiplayer is excluded and its two-player reduction is always zero. |
| 702.125b | Undaunted | N/A | N/A: undaunted varies only with number of opponents; multiplayer is excluded and its two-player reduction is always zero. |
| 702.125c | Undaunted | N/A | N/A: undaunted varies only with number of opponents; multiplayer is excluded and its two-player reduction is always zero. |
| 702.126 | Improvise | IMPLEMENTED | Improvise optional artifact tapping/reduction is handled through extra-cost selection at `src/MTGRules.cpp:978-992`; multi-artifact regression covers it. |
| 702.126a | Improvise | IMPLEMENTED | Improvise optional artifact tapping/reduction is handled through extra-cost selection at `src/MTGRules.cpp:978-992`; multi-artifact regression covers it. |
| 702.126b | Improvise | IMPLEMENTED | Improvise optional artifact tapping/reduction is handled through extra-cost selection at `src/MTGRules.cpp:978-992`; multi-artifact regression covers it. |
| 702.126c | Improvise | IMPLEMENTED | Improvise optional artifact tapping/reduction is handled through extra-cost selection at `src/MTGRules.cpp:978-992`; multi-artifact regression covers it. |
| 702.127 | Aftermath | PARTIAL | Aftermath reuses flashback with `HASAFTERMATH` (`src/MTGDefinitions.cpp:239`; `src/AllAbilities.cpp:5471-5475`), but split-card orientation/characteristics are not fully modeled. |
| 702.127a | Aftermath | PARTIAL | Aftermath reuses flashback with `HASAFTERMATH` (`src/MTGDefinitions.cpp:239`; `src/AllAbilities.cpp:5471-5475`), but split-card orientation/characteristics are not fully modeled. |
| 702.128 | Embalm | PARTIAL | Embalm is card-scripted as exile/clone/token modification; copy-without-cost/text and sorcery timing are not native generic behavior. |
| 702.128a | Embalm | PARTIAL | Embalm is card-scripted as exile/clone/token modification; copy-without-cost/text and sorcery timing are not native generic behavior. |
| 702.128b | Embalm | PARTIAL | Embalm is card-scripted as exile/clone/token modification; copy-without-cost/text and sorcery timing are not native generic behavior. |
| 702.129 | Eternalize | IMPLEMENTED | `_macros.txt:213-214` implements the graveyard exile/4/4 Zombie copy; active eternalize regression covers the token. |
| 702.129a | Eternalize | IMPLEMENTED | `_macros.txt:213-214` implements the graveyard exile/4/4 Zombie copy; active eternalize regression covers the token. |
| 702.130 | Afflict | IMPLEMENTED | Afflict is represented by blocked-trigger life-loss scripts; behavior exists for cards but no native keyword/multiple-instance identity exists. |
| 702.130a | Afflict | IMPLEMENTED | Afflict is represented by blocked-trigger life-loss scripts; behavior exists for cards but no native keyword/multiple-instance identity exists. |
| 702.130b | Afflict | IMPLEMENTED | Afflict is represented by blocked-trigger life-loss scripts; behavior exists for cards but no native keyword/multiple-instance identity exists. |
| 702.131 | Ascend | INCORRECT | `_ASCEND_` (`_macros.txt:133-137`) represents the immutable player designation as a battlefield token, so it can be counted/affected as a permanent contrary to 702.131 and is not a true once-acquired player designation. |
| 702.131a | Ascend | INCORRECT | `_ASCEND_` (`_macros.txt:133-137`) represents the immutable player designation as a battlefield token, so it can be counted/affected as a permanent contrary to 702.131 and is not a true once-acquired player designation. |
| 702.131b | Ascend | INCORRECT | `_ASCEND_` (`_macros.txt:133-137`) represents the immutable player designation as a battlefield token, so it can be counted/affected as a permanent contrary to 702.131 and is not a true once-acquired player designation. |
| 702.131c | Ascend | INCORRECT | `_ASCEND_` (`_macros.txt:133-137`) represents the immutable player designation as a battlefield token, so it can be counted/affected as a permanent contrary to 702.131 and is not a true once-acquired player designation. |
| 702.131d | Ascend | INCORRECT | `_ASCEND_` (`_macros.txt:133-137`) represents the immutable player designation as a battlefield token, so it can be counted/affected as a permanent contrary to 702.131 and is not a true once-acquired player designation. |
| 702.132 | Assist | N/A | N/A: assist is a teammate-payment multiplayer rule; team games are excluded. |
| 702.132a | Assist | N/A | N/A: assist is a teammate-payment multiplayer rule; team games are excluded. |
| 702.133 | Jump-Start | PARTIAL | Jump-start is card-scripted graveyard casting plus discard/exile; no native keyword identity or all alternative-cost interactions were found. |
| 702.133a | Jump-Start | PARTIAL | Jump-start is card-scripted graveyard casting plus discard/exile; no native keyword identity or all alternative-cost interactions were found. |
| 702.134 | Mentor | IMPLEMENTED | `_macros.txt:153-154` implements lesser-power attacking target and counter; active mentor regression covers it. |
| 702.134a | Mentor | IMPLEMENTED | `_macros.txt:153-154` implements lesser-power attacking target and counter; active mentor regression covers it. |
| 702.134b | Mentor | IMPLEMENTED | `_macros.txt:153-154` implements lesser-power attacking target and counter; active mentor regression covers it. |
| 702.134c | Mentor | IMPLEMENTED | `_macros.txt:153-154` implements lesser-power attacking target and counter; active mentor regression covers it. |
| 702.135 | Afterlife | IMPLEMENTED | `_macros.txt:163-164` supplies the dies token and active afterlife regression covers multiplicity. |
| 702.135a | Afterlife | IMPLEMENTED | `_macros.txt:163-164` supplies the dies token and active afterlife regression covers multiplicity. |
| 702.135b | Afterlife | IMPLEMENTED | `_macros.txt:163-164` supplies the dies token and active afterlife regression covers multiplicity. |
| 702.136 | Riot | PARTIAL | Riot is per-card ETB choice scripting; simultaneous replacement-choice and haste/counter linkage have no native keyword layer. |
| 702.136a | Riot | PARTIAL | Riot is per-card ETB choice scripting; simultaneous replacement-choice and haste/counter linkage have no native keyword layer. |
| 702.136b | Riot | PARTIAL | Riot is per-card ETB choice scripting; simultaneous replacement-choice and haste/counter linkage have no native keyword layer. |
| 702.137 | Spectacle | PARTIAL | Spectacle is card-scripted as an alternate cost gated by opponent life loss; generic paid-method linkage is incomplete. |
| 702.137a | Spectacle | PARTIAL | Spectacle is card-scripted as an alternate cost gated by opponent life loss; generic paid-method linkage is incomplete. |
| 702.138 | Escape | PARTIAL | Escape is card-scripted graveyard alternative casting/exile costs (`src/AllAbilities.cpp:769-801` parses restrictions), but persistent escaped identity and all additional-cost combinations are not generic. |
| 702.138a | Escape | PARTIAL | Escape is card-scripted graveyard alternative casting/exile costs (`src/AllAbilities.cpp:769-801` parses restrictions), but persistent escaped identity and all additional-cost combinations are not generic. |
| 702.138b | Escape | PARTIAL | Escape is card-scripted graveyard alternative casting/exile costs (`src/AllAbilities.cpp:769-801` parses restrictions), but persistent escaped identity and all additional-cost combinations are not generic. |
| 702.138c | Escape | PARTIAL | Escape is card-scripted graveyard alternative casting/exile costs (`src/AllAbilities.cpp:769-801` parses restrictions), but persistent escaped identity and all additional-cost combinations are not generic. |
| 702.138d | Escape | PARTIAL | Escape is card-scripted graveyard alternative casting/exile costs (`src/AllAbilities.cpp:769-801` parses restrictions), but persistent escaped identity and all additional-cost combinations are not generic. |
| 702.139 | Companion | UNIMPLEMENTED | Named search found no companion pregame reveal, deck-restriction validation, or outside-game hand placement in the requested engine surfaces. |
| 702.139a | Companion | UNIMPLEMENTED | Named search found no companion pregame reveal, deck-restriction validation, or outside-game hand placement in the requested engine surfaces. |
| 702.139b | Companion | UNIMPLEMENTED | Named search found no companion pregame reveal, deck-restriction validation, or outside-game hand placement in the requested engine surfaces. |
| 702.139c | Companion | UNIMPLEMENTED | Named search found no companion pregame reveal, deck-restriction validation, or outside-game hand placement in the requested engine surfaces. |
| 702.139d | Companion | UNIMPLEMENTED | Named search found no companion pregame reveal, deck-restriction validation, or outside-game hand placement in the requested engine surfaces. |
| 702.140 | Mutate | PARTIAL | Mutate cast/merge machinery exists at `src/MTGRules.cpp:3707-3755` and active mutate regressions pass; copy/layering, ownership, and all merged-component edge cases remain incomplete. |
| 702.140a | Mutate | PARTIAL | Mutate cast/merge machinery exists at `src/MTGRules.cpp:3707-3755` and active mutate regressions pass; copy/layering, ownership, and all merged-component edge cases remain incomplete. |
| 702.140b | Mutate | PARTIAL | Mutate cast/merge machinery exists at `src/MTGRules.cpp:3707-3755` and active mutate regressions pass; copy/layering, ownership, and all merged-component edge cases remain incomplete. |
| 702.140c | Mutate | PARTIAL | Mutate cast/merge machinery exists at `src/MTGRules.cpp:3707-3755` and active mutate regressions pass; copy/layering, ownership, and all merged-component edge cases remain incomplete. |
| 702.140d | Mutate | PARTIAL | Mutate cast/merge machinery exists at `src/MTGRules.cpp:3707-3755` and active mutate regressions pass; copy/layering, ownership, and all merged-component edge cases remain incomplete. |
| 702.140e | Mutate | PARTIAL | Mutate cast/merge machinery exists at `src/MTGRules.cpp:3707-3755` and active mutate regressions pass; copy/layering, ownership, and all merged-component edge cases remain incomplete. |
| 702.140f | Mutate | PARTIAL | Mutate cast/merge machinery exists at `src/MTGRules.cpp:3707-3755` and active mutate regressions pass; copy/layering, ownership, and all merged-component edge cases remain incomplete. |
| 702.141 | Encore | PARTIAL | Encore is per-card graveyard/token scripting; its opponent-by-opponent attack destinations and delayed exile are not generically modeled. |
| 702.141a | Encore | PARTIAL | Encore is per-card graveyard/token scripting; its opponent-by-opponent attack destinations and delayed exile are not generically modeled. |
| 702.142 | Boast | IMPLEMENTED | Boast event/once-per-turn machinery exists at `src/MTGAbility.cpp:1592-1595,4728-4737`; active boast regressions cover the event. |
| 702.142a | Boast | IMPLEMENTED | Boast event/once-per-turn machinery exists at `src/MTGAbility.cpp:1592-1595,4728-4737`; active boast regressions cover the event. |
| 702.142b | Boast | IMPLEMENTED | Boast event/once-per-turn machinery exists at `src/MTGAbility.cpp:1592-1595,4728-4737`; active boast regressions cover the event. |
| 702.143 | Foretell | PARTIAL | Foretell exile/turn stamp/event are at `src/AllAbilities.cpp:3117-3145`, but face-down information and the generic grant/cast relationship are incomplete; one event regression remains catalogued as failing. |
| 702.143a | Foretell | PARTIAL | Foretell exile/turn stamp/event are at `src/AllAbilities.cpp:3117-3145`, but face-down information and the generic grant/cast relationship are incomplete; one event regression remains catalogued as failing. |
| 702.143b | Foretell | PARTIAL | Foretell exile/turn stamp/event are at `src/AllAbilities.cpp:3117-3145`, but face-down information and the generic grant/cast relationship are incomplete; one event regression remains catalogued as failing. |
| 702.143c | Foretell | PARTIAL | Foretell exile/turn stamp/event are at `src/AllAbilities.cpp:3117-3145`, but face-down information and the generic grant/cast relationship are incomplete; one event regression remains catalogued as failing. |
| 702.143d | Foretell | PARTIAL | Foretell exile/turn stamp/event are at `src/AllAbilities.cpp:3117-3145`, but face-down information and the generic grant/cast relationship are incomplete; one event regression remains catalogued as failing. |
| 702.143e | Foretell | PARTIAL | Foretell exile/turn stamp/event are at `src/AllAbilities.cpp:3117-3145`, but face-down information and the generic grant/cast relationship are incomplete; one event regression remains catalogued as failing. |
| 702.143f | Foretell | PARTIAL | Foretell exile/turn stamp/event are at `src/AllAbilities.cpp:3117-3145`, but face-down information and the generic grant/cast relationship are incomplete; one event regression remains catalogued as failing. |
| 702.144 | Demonstrate | N/A | N/A: demonstrate necessarily gives a copy to an opponent and is classified as multiplayer-only under this audit scope. |
| 702.144a | Demonstrate | N/A | N/A: demonstrate necessarily gives a copy to an opponent and is classified as multiplayer-only under this audit scope. |
| 702.145 | Daybound and Nightbound | IMPLEMENTED | Day/night establishment and transitions are implemented and covered by active `basic_daybound_flips_after_empty_turn.txt` and `basic_nightbound_flips_after_two_spells.txt`; catalogue history reclassified the engine as working. |
| 702.145a | Daybound and Nightbound | IMPLEMENTED | Day/night establishment and transitions are implemented and covered by active `basic_daybound_flips_after_empty_turn.txt` and `basic_nightbound_flips_after_two_spells.txt`; catalogue history reclassified the engine as working. |
| 702.145b | Daybound and Nightbound | IMPLEMENTED | Day/night establishment and transitions are implemented and covered by active `basic_daybound_flips_after_empty_turn.txt` and `basic_nightbound_flips_after_two_spells.txt`; catalogue history reclassified the engine as working. |
| 702.145c | Daybound and Nightbound | IMPLEMENTED | Day/night establishment and transitions are implemented and covered by active `basic_daybound_flips_after_empty_turn.txt` and `basic_nightbound_flips_after_two_spells.txt`; catalogue history reclassified the engine as working. |
| 702.145d | Daybound and Nightbound | IMPLEMENTED | Day/night establishment and transitions are implemented and covered by active `basic_daybound_flips_after_empty_turn.txt` and `basic_nightbound_flips_after_two_spells.txt`; catalogue history reclassified the engine as working. |
| 702.145e | Daybound and Nightbound | IMPLEMENTED | Day/night establishment and transitions are implemented and covered by active `basic_daybound_flips_after_empty_turn.txt` and `basic_nightbound_flips_after_two_spells.txt`; catalogue history reclassified the engine as working. |
| 702.145f | Daybound and Nightbound | IMPLEMENTED | Day/night establishment and transitions are implemented and covered by active `basic_daybound_flips_after_empty_turn.txt` and `basic_nightbound_flips_after_two_spells.txt`; catalogue history reclassified the engine as working. |
| 702.145g | Daybound and Nightbound | IMPLEMENTED | Day/night establishment and transitions are implemented and covered by active `basic_daybound_flips_after_empty_turn.txt` and `basic_nightbound_flips_after_two_spells.txt`; catalogue history reclassified the engine as working. |
| 702.146 | Disturb | PARTIAL | Disturb reuses retrace/transform/exile flags (`src/MTGDefinitions.cpp:233`; `src/MTGAbility.cpp:7357-7360`); back-face characteristic/timing edges remain card-scripted. |
| 702.146a | Disturb | PARTIAL | Disturb reuses retrace/transform/exile flags (`src/MTGDefinitions.cpp:233`; `src/MTGAbility.cpp:7357-7360`); back-face characteristic/timing edges remain card-scripted. |
| 702.146b | Disturb | PARTIAL | Disturb reuses retrace/transform/exile flags (`src/MTGDefinitions.cpp:233`; `src/MTGAbility.cpp:7357-7360`); back-face characteristic/timing edges remain card-scripted. |
| 702.147 | Decayed | IMPLEMENTED | `src/MTGDefinitions.cpp:236` stores decayed and card scripts enforce no-block/sacrifice; active decayed regression covers attack sacrifice. |
| 702.147a | Decayed | IMPLEMENTED | `src/MTGDefinitions.cpp:236` stores decayed and card scripts enforce no-block/sacrifice; active decayed regression covers attack sacrifice. |
| 702.148 | Cleave | PARTIAL | Cleave is encoded as an alternate cost with separate card script text, not a generic bracketed-text removal effect. |
| 702.148a | Cleave | PARTIAL | Cleave is encoded as an alternate cost with separate card script text, not a generic bracketed-text removal effect. |
| 702.148b | Cleave | PARTIAL | Cleave is encoded as an alternate cost with separate card script text, not a generic bracketed-text removal effect. |
| 702.149 | Training | IMPLEMENTED | `_macros.txt:18-19` and `src/AllAbilities.cpp:3008-3045` implement the greater-power attack trigger; active training regression covers it. |
| 702.149a | Training | IMPLEMENTED | `_macros.txt:18-19` and `src/AllAbilities.cpp:3008-3045` implement the greater-power attack trigger; active training regression covers it. |
| 702.149b | Training | IMPLEMENTED | `_macros.txt:18-19` and `src/AllAbilities.cpp:3008-3045` implement the greater-power attack trigger; active training regression covers it. |
| 702.149c | Training | IMPLEMENTED | `_macros.txt:18-19` and `src/AllAbilities.cpp:3008-3045` implement the greater-power attack trigger; active training regression covers it. |
| 702.150 | Compleated | PARTIAL | Compleated is modeled by card mana/loyalty scripts, but no native per-Phyrexian-symbol loyalty replacement keyword layer exists. |
| 702.150a | Compleated | PARTIAL | Compleated is modeled by card mana/loyalty scripts, but no native per-Phyrexian-symbol loyalty replacement keyword layer exists. |
| 702.151 | Reconfigure | IMPLEMENTED | `src/MTGAbility.cpp:4013-4047` and `src/AllAbilities.cpp:10852-10923` implement attach/unattach and creature status; active reconfigure regression covers attachment. |
| 702.151a | Reconfigure | IMPLEMENTED | `src/MTGAbility.cpp:4013-4047` and `src/AllAbilities.cpp:10852-10923` implement attach/unattach and creature status; active reconfigure regression covers attachment. |
| 702.151b | Reconfigure | IMPLEMENTED | `src/MTGAbility.cpp:4013-4047` and `src/AllAbilities.cpp:10852-10923` implement attach/unattach and creature status; active reconfigure regression covers attachment. |
| 702.152 | Blitz | PARTIAL | Blitz is card-scripted alternate cost, haste, end-step sacrifice, and death draw; generic paid-status linkage is incomplete. |
| 702.152a | Blitz | PARTIAL | Blitz is card-scripted alternate cost, haste, end-step sacrifice, and death draw; generic paid-status linkage is incomplete. |
| 702.152b | Blitz | PARTIAL | Blitz is card-scripted alternate cost, haste, end-step sacrifice, and death draw; generic paid-status linkage is incomplete. |
| 702.153 | Casualty | PARTIAL | Casualty is card-scripted sacrifice/copy behavior; no native additional-cost/copy trigger or multi-instance identity was found. |
| 702.153a | Casualty | PARTIAL | Casualty is card-scripted sacrifice/copy behavior; no native additional-cost/copy trigger or multi-instance identity was found. |
| 702.153b | Casualty | PARTIAL | Casualty is card-scripted sacrifice/copy behavior; no native additional-cost/copy trigger or multi-instance identity was found. |
| 702.154 | Enlist | PARTIAL | `_ENLIST_` (`_macros.txt:239-240`) approximates enlist by granting an activated tap/pump after attack declaration; failing witness `_known_failures.txt:35` shows the granted action is undrivable in the suite, and CR attack-cost timing is not modeled. |
| 702.154a | Enlist | PARTIAL | `_ENLIST_` (`_macros.txt:239-240`) approximates enlist by granting an activated tap/pump after attack declaration; failing witness `_known_failures.txt:35` shows the granted action is undrivable in the suite, and CR attack-cost timing is not modeled. |
| 702.154b | Enlist | PARTIAL | `_ENLIST_` (`_macros.txt:239-240`) approximates enlist by granting an activated tap/pump after attack declaration; failing witness `_known_failures.txt:35` shows the granted action is undrivable in the suite, and CR attack-cost timing is not modeled. |
| 702.154c | Enlist | PARTIAL | `_ENLIST_` (`_macros.txt:239-240`) approximates enlist by granting an activated tap/pump after attack declaration; failing witness `_known_failures.txt:35` shows the granted action is undrivable in the suite, and CR attack-cost timing is not modeled. |
| 702.154d | Enlist | PARTIAL | `_ENLIST_` (`_macros.txt:239-240`) approximates enlist by granting an activated tap/pump after attack declaration; failing witness `_known_failures.txt:35` shows the granted action is undrivable in the suite, and CR attack-cost timing is not modeled. |
| 702.155 | Read Ahead | UNIMPLEMENTED | Named search found no Read Ahead parser/behavior or primitive grammar usage in the requested surfaces. |
| 702.155a | Read Ahead | UNIMPLEMENTED | Named search found no Read Ahead parser/behavior or primitive grammar usage in the requested surfaces. |
| 702.155b | Read Ahead | UNIMPLEMENTED | Named search found no Read Ahead parser/behavior or primitive grammar usage in the requested surfaces. |
| 702.155c | Read Ahead | UNIMPLEMENTED | Named search found no Read Ahead parser/behavior or primitive grammar usage in the requested surfaces. |
| 702.156 | Ravenous | PARTIAL | Ravenous is expanded per card into ETB counters/draw checks; no native X/payment-linked keyword behavior exists. |
| 702.156a | Ravenous | PARTIAL | Ravenous is expanded per card into ETB counters/draw checks; no native X/payment-linked keyword behavior exists. |
| 702.157 | Squad | PARTIAL | Squad is represented by repeatable additional-cost/token-copy scripts; generic each-payment triggers and copied characteristics are incomplete. |
| 702.157a | Squad | PARTIAL | Squad is represented by repeatable additional-cost/token-copy scripts; generic each-payment triggers and copied characteristics are incomplete. |
| 702.157b | Squad | PARTIAL | Squad is represented by repeatable additional-cost/token-copy scripts; generic each-payment triggers and copied characteristics are incomplete. |
| 702.158 | Space Sculptor | N/A | N/A: space sculptor applies to Galaxy sector cards from an excluded casual variant and has no engine surface. |
| 702.158a | Space Sculptor | N/A | N/A: space sculptor applies to Galaxy sector cards from an excluded casual variant and has no engine surface. |
| 702.158b | Space Sculptor | N/A | N/A: space sculptor applies to Galaxy sector cards from an excluded casual variant and has no engine surface. |
| 702.158c | Space Sculptor | N/A | N/A: space sculptor applies to Galaxy sector cards from an excluded casual variant and has no engine surface. |
| 702.158d | Space Sculptor | N/A | N/A: space sculptor applies to Galaxy sector cards from an excluded casual variant and has no engine surface. |
| 702.158e | Space Sculptor | N/A | N/A: space sculptor applies to Galaxy sector cards from an excluded casual variant and has no engine surface. |
| 702.159 | Visit | N/A | N/A: visit is an Attraction/casual-variant keyword; Attractions are outside supported game types. |
| 702.159a | Visit | N/A | N/A: visit is an Attraction/casual-variant keyword; Attractions are outside supported game types. |
| 702.159b | Visit | N/A | N/A: visit is an Attraction/casual-variant keyword; Attractions are outside supported game types. |
| 702.160 | Prototype | PARTIAL | Prototype is card-scripted alternate cost plus characteristic transformation; stack/permanent copiable characteristics are not natively modeled. |
| 702.160a | Prototype | PARTIAL | Prototype is card-scripted alternate cost plus characteristic transformation; stack/permanent copiable characteristics are not natively modeled. |
| 702.161 | Living Metal | UNIMPLEMENTED | Named search found no living-metal turn animation behavior in the requested engine surfaces. |
| 702.161a | Living Metal | UNIMPLEMENTED | Named search found no living-metal turn animation behavior in the requested engine surfaces. |
| 702.162 | More Than Meets the Eye | UNIMPLEMENTED | Named search found no More Than Meets the Eye converted-cost/cast-transformed behavior in the requested engine surfaces. |
| 702.162a | More Than Meets the Eye | UNIMPLEMENTED | Named search found no More Than Meets the Eye converted-cost/cast-transformed behavior in the requested engine surfaces. |
| 702.163 | For Mirrodin! | PARTIAL | For Mirrodin! is approximated by Living Weapon-style Rebel token/attachment scripts; color/type and trigger identity are card-specific. |
| 702.163a | For Mirrodin! | PARTIAL | For Mirrodin! is approximated by Living Weapon-style Rebel token/attachment scripts; color/type and trigger identity are card-specific. |
| 702.164 | Toxic | IMPLEMENTED | Toxic values 1-10 are stored at `src/MTGDefinitions.cpp:83-85,200,263-268` and read by `src/MTGCardInstance.cpp:604-628`; active exact-poison regressions cover values. |
| 702.164a | Toxic | IMPLEMENTED | Toxic values 1-10 are stored at `src/MTGDefinitions.cpp:83-85,200,263-268` and read by `src/MTGCardInstance.cpp:604-628`; active exact-poison regressions cover values. |
| 702.164b | Toxic | IMPLEMENTED | Toxic values 1-10 are stored at `src/MTGDefinitions.cpp:83-85,200,263-268` and read by `src/MTGCardInstance.cpp:604-628`; active exact-poison regressions cover values. |
| 702.164c | Toxic | IMPLEMENTED | Toxic values 1-10 are stored at `src/MTGDefinitions.cpp:83-85,200,263-268` and read by `src/MTGCardInstance.cpp:604-628`; active exact-poison regressions cover values. |
| 702.165 | Backup | UNIMPLEMENTED | Good-faith search found Backup only in card text; no counter-and-ability-copy keyword behavior in the five requested C++ surfaces. |
| 702.165a | Backup | UNIMPLEMENTED | Good-faith search found Backup only in card text; no counter-and-ability-copy keyword behavior in the five requested C++ surfaces. |
| 702.165b | Backup | UNIMPLEMENTED | Good-faith search found Backup only in card text; no counter-and-ability-copy keyword behavior in the five requested C++ surfaces. |
| 702.165c | Backup | UNIMPLEMENTED | Good-faith search found Backup only in card text; no counter-and-ability-copy keyword behavior in the five requested C++ surfaces. |
| 702.165d | Backup | UNIMPLEMENTED | Good-faith search found Backup only in card text; no counter-and-ability-copy keyword behavior in the five requested C++ surfaces. |
| 702.166 | Bargain | PARTIAL | Bargain is card-scripted through optional sacrifice and paid-condition branches; no native bargained designation/linked-target behavior exists. |
| 702.166a | Bargain | PARTIAL | Bargain is card-scripted through optional sacrifice and paid-condition branches; no native bargained designation/linked-target behavior exists. |
| 702.166b | Bargain | PARTIAL | Bargain is card-scripted through optional sacrifice and paid-condition branches; no native bargained designation/linked-target behavior exists. |
| 702.166c | Bargain | PARTIAL | Bargain is card-scripted through optional sacrifice and paid-condition branches; no native bargained designation/linked-target behavior exists. |
| 702.166d | Bargain | PARTIAL | Bargain is card-scripted through optional sacrifice and paid-condition branches; no native bargained designation/linked-target behavior exists. |
| 702.167 | Craft | PARTIAL | Craft is card-scripted with exile materials and transform; no generic linked record of the exact materials exists. |
| 702.167a | Craft | PARTIAL | Craft is card-scripted with exile materials and transform; no generic linked record of the exact materials exists. |
| 702.167b | Craft | PARTIAL | Craft is card-scripted with exile materials and transform; no generic linked record of the exact materials exists. |
| 702.167c | Craft | PARTIAL | Craft is card-scripted with exile materials and transform; no generic linked record of the exact materials exists. |
| 702.168 | Disguise | PARTIAL | Disguise is approximated through morph/face-down scripts, but the face-down ward {2}, copiable characteristics, and disguise-specific X/action semantics are not a native keyword. |
| 702.168a | Disguise | PARTIAL | Disguise is approximated through morph/face-down scripts, but the face-down ward {2}, copiable characteristics, and disguise-specific X/action semantics are not a native keyword. |
| 702.168b | Disguise | PARTIAL | Disguise is approximated through morph/face-down scripts, but the face-down ward {2}, copiable characteristics, and disguise-specific X/action semantics are not a native keyword. |
| 702.168c | Disguise | PARTIAL | Disguise is approximated through morph/face-down scripts, but the face-down ward {2}, copiable characteristics, and disguise-specific X/action semantics are not a native keyword. |
| 702.168d | Disguise | PARTIAL | Disguise is approximated through morph/face-down scripts, but the face-down ward {2}, copiable characteristics, and disguise-specific X/action semantics are not a native keyword. |
| 702.168e | Disguise | PARTIAL | Disguise is approximated through morph/face-down scripts, but the face-down ward {2}, copiable characteristics, and disguise-specific X/action semantics are not a native keyword. |
| 702.168f | Disguise | PARTIAL | Disguise is approximated through morph/face-down scripts, but the face-down ward {2}, copiable characteristics, and disguise-specific X/action semantics are not a native keyword. |
| 702.169 | Solved | PARTIAL | Solved conditions are card-scripted; there is no generic Case solved state/trigger/activation keyword layer in the requested surfaces. |
| 702.169a | Solved | PARTIAL | Solved conditions are card-scripted; there is no generic Case solved state/trigger/activation keyword layer in the requested surfaces. |
| 702.169b | Solved | PARTIAL | Solved conditions are card-scripted; there is no generic Case solved state/trigger/activation keyword layer in the requested surfaces. |
| 702.169c | Solved | PARTIAL | Solved conditions are card-scripted; there is no generic Case solved state/trigger/activation keyword layer in the requested surfaces. |
| 702.169d | Solved | PARTIAL | Solved conditions are card-scripted; there is no generic Case solved state/trigger/activation keyword layer in the requested surfaces. |
| 702.170 | Plot | INCORRECT | `_PLOT_`/`_PLOTCAST_` (`_macros.txt:178-182`) reuse foretell without establishing a working plotted-cast permission; `macro_plotcast.txt` is a known failure, so plotted cards cannot reliably be cast on a later turn. |
| 702.170a | Plot | INCORRECT | `_PLOT_`/`_PLOTCAST_` (`_macros.txt:178-182`) reuse foretell without establishing a working plotted-cast permission; `macro_plotcast.txt` is a known failure, so plotted cards cannot reliably be cast on a later turn. |
| 702.170b | Plot | INCORRECT | `_PLOT_`/`_PLOTCAST_` (`_macros.txt:178-182`) reuse foretell without establishing a working plotted-cast permission; `macro_plotcast.txt` is a known failure, so plotted cards cannot reliably be cast on a later turn. |
| 702.170c | Plot | INCORRECT | `_PLOT_`/`_PLOTCAST_` (`_macros.txt:178-182`) reuse foretell without establishing a working plotted-cast permission; `macro_plotcast.txt` is a known failure, so plotted cards cannot reliably be cast on a later turn. |
| 702.170d | Plot | INCORRECT | `_PLOT_`/`_PLOTCAST_` (`_macros.txt:178-182`) reuse foretell without establishing a working plotted-cast permission; `macro_plotcast.txt` is a known failure, so plotted cards cannot reliably be cast on a later turn. |
| 702.170e | Plot | INCORRECT | `_PLOT_`/`_PLOTCAST_` (`_macros.txt:178-182`) reuse foretell without establishing a working plotted-cast permission; `macro_plotcast.txt` is a known failure, so plotted cards cannot reliably be cast on a later turn. |
| 702.170f | Plot | INCORRECT | `_PLOT_`/`_PLOTCAST_` (`_macros.txt:178-182`) reuse foretell without establishing a working plotted-cast permission; `macro_plotcast.txt` is a known failure, so plotted cards cannot reliably be cast on a later turn. |
| 702.171 | Saddle | PARTIAL | Saddle is approximated with tap-total-power scripts and a temporary marker; no generic saddled designation/creature attribution exists. |
| 702.171a | Saddle | PARTIAL | Saddle is approximated with tap-total-power scripts and a temporary marker; no generic saddled designation/creature attribution exists. |
| 702.171b | Saddle | PARTIAL | Saddle is approximated with tap-total-power scripts and a temporary marker; no generic saddled designation/creature attribution exists. |
| 702.171c | Saddle | PARTIAL | Saddle is approximated with tap-total-power scripts and a temporary marker; no generic saddled designation/creature attribution exists. |
| 702.172 | Spree | PARTIAL | Spree is card-scripted with modal additional costs; there is no generic choose-one-or-more/cost-per-mode keyword parser. |
| 702.172a | Spree | PARTIAL | Spree is card-scripted with modal additional costs; there is no generic choose-one-or-more/cost-per-mode keyword parser. |
| 702.172b | Spree | PARTIAL | Spree is card-scripted with modal additional costs; there is no generic choose-one-or-more/cost-per-mode keyword parser. |
| 702.173 | Freerunning | UNIMPLEMENTED | Named search found no freerunning parser/behavior or primitive grammar use in the requested surfaces. |
| 702.173a | Freerunning | UNIMPLEMENTED | Named search found no freerunning parser/behavior or primitive grammar use in the requested surfaces. |
| 702.174 | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174a | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174b | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174c | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174d | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174e | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174f | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174g | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174h | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174i | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174j | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174k | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.174m | Gift | INCORRECT | Known catalogue defect: Gift cards use equal `mana=`/`other=` costs, so alternative-payment state is never set and the promised-gift branch is unreachable; all gift effects in 702.174 are therefore behaviorally wrong when promised. |
| 702.175 | Offspring | PARTIAL | Offspring is card-scripted with kicker-like payment and a 1/1 clone; multiple-instance payment linkage and full copy exceptions are not generic. |
| 702.175a | Offspring | PARTIAL | Offspring is card-scripted with kicker-like payment and a 1/1 clone; multiple-instance payment linkage and full copy exceptions are not generic. |
| 702.175b | Offspring | PARTIAL | Offspring is card-scripted with kicker-like payment and a 1/1 clone; multiple-instance payment linkage and full copy exceptions are not generic. |
| 702.176 | Impending | PARTIAL | Impending is card-scripted alternate cost/time counters/noncreature state; all four linked abilities and paid-history persistence are not generically modeled. |
| 702.176a | Impending | PARTIAL | Impending is card-scripted alternate cost/time counters/noncreature state; all four linked abilities and paid-history persistence are not generically modeled. |
| 702.177 | Exhaust | PARTIAL | Exhaust is approximated with per-ability limits, but no object-independent 'activate only once' history or cross-effect check required by 702.177b exists. |
| 702.177a | Exhaust | PARTIAL | Exhaust is approximated with per-ability limits, but no object-independent 'activate only once' history or cross-effect check required by 702.177b exists. |
| 702.177b | Exhaust | PARTIAL | Exhaust is approximated with per-ability limits, but no object-independent 'activate only once' history or cross-effect check required by 702.177b exists. |
| 702.178 | Max Speed | UNIMPLEMENTED | Named search found no speed or max-speed state/ability behavior in the requested engine surfaces. |
| 702.178a | Max Speed | UNIMPLEMENTED | Named search found no speed or max-speed state/ability behavior in the requested engine surfaces. |
| 702.178b | Max Speed | UNIMPLEMENTED | Named search found no speed or max-speed state/ability behavior in the requested engine surfaces. |
| 702.179 | Start Your Engines! | UNIMPLEMENTED | Named search found no player speed, start-your-engines state-based action, or inherent speed trigger in the requested surfaces. |
| 702.179a | Start Your Engines! | UNIMPLEMENTED | Named search found no player speed, start-your-engines state-based action, or inherent speed trigger in the requested surfaces. |
| 702.179b | Start Your Engines! | UNIMPLEMENTED | Named search found no player speed, start-your-engines state-based action, or inherent speed trigger in the requested surfaces. |
| 702.179c | Start Your Engines! | UNIMPLEMENTED | Named search found no player speed, start-your-engines state-based action, or inherent speed trigger in the requested surfaces. |
| 702.179d | Start Your Engines! | UNIMPLEMENTED | Named search found no player speed, start-your-engines state-based action, or inherent speed trigger in the requested surfaces. |
| 702.179e | Start Your Engines! | UNIMPLEMENTED | Named search found no player speed, start-your-engines state-based action, or inherent speed trigger in the requested surfaces. |
| 702.179f | Start Your Engines! | UNIMPLEMENTED | Named search found no player speed, start-your-engines state-based action, or inherent speed trigger in the requested surfaces. |
| 702.180 | Harmonize | UNIMPLEMENTED | Named search found no Harmonize graveyard cast/tap-for-reduction/exile behavior in the requested surfaces. |
| 702.180a | Harmonize | UNIMPLEMENTED | Named search found no Harmonize graveyard cast/tap-for-reduction/exile behavior in the requested surfaces. |
| 702.180b | Harmonize | UNIMPLEMENTED | Named search found no Harmonize graveyard cast/tap-for-reduction/exile behavior in the requested surfaces. |
| 702.181 | Mobilize | PARTIAL | `_MOBILIZE_` (`_macros.txt:281-282`) creates attacking Warriors and an end-step sacrifice marker, but no focused regression establishes tapped/attacking destination and exact delayed sacrifice. |
| 702.181a | Mobilize | PARTIAL | `_MOBILIZE_` (`_macros.txt:281-282`) creates attacking Warriors and an end-step sacrifice marker, but no focused regression establishes tapped/attacking destination and exact delayed sacrifice. |
| 702.182 | Job Select | UNIMPLEMENTED | Named search found no Job Select token-and-attach behavior in the requested surfaces. |
| 702.182a | Job Select | UNIMPLEMENTED | Named search found no Job Select token-and-attach behavior in the requested surfaces. |
| 702.183 | Tiered | UNIMPLEMENTED | Named search found no Tiered modal additional-cost behavior in the requested surfaces. |
| 702.183a | Tiered | UNIMPLEMENTED | Named search found no Tiered modal additional-cost behavior in the requested surfaces. |
| 702.184 | Station | PARTIAL | Station is approximated by per-card tap/counter scripts; no native station symbols or alternative-characteristic modifier model exists. |
| 702.184a | Station | PARTIAL | Station is approximated by per-card tap/counter scripts; no native station symbols or alternative-characteristic modifier model exists. |
| 702.184b | Station | PARTIAL | Station is approximated by per-card tap/counter scripts; no native station symbols or alternative-characteristic modifier model exists. |
| 702.184c | Station | PARTIAL | Station is approximated by per-card tap/counter scripts; no native station symbols or alternative-characteristic modifier model exists. |
| 702.185 | Warp | PARTIAL | Warp cards use alternate-cost/exile/cast-permission scripts (parser composition noted at `src/MTGAbility.cpp:2845-2851`), but warped identity and post-turn permission are not generic. |
| 702.185a | Warp | PARTIAL | Warp cards use alternate-cost/exile/cast-permission scripts (parser composition noted at `src/MTGAbility.cpp:2845-2851`), but warped identity and post-turn permission are not generic. |
| 702.185b | Warp | PARTIAL | Warp cards use alternate-cost/exile/cast-permission scripts (parser composition noted at `src/MTGAbility.cpp:2845-2851`), but warped identity and post-turn permission are not generic. |
| 702.185c | Warp | PARTIAL | Warp cards use alternate-cost/exile/cast-permission scripts (parser composition noted at `src/MTGAbility.cpp:2845-2851`), but warped identity and post-turn permission are not generic. |
| 702.186 | ∞ (Infinity) | UNIMPLEMENTED | Named search found no harnessed state or Infinity keyword behavior in the requested surfaces. |
| 702.186a | ∞ (Infinity) | UNIMPLEMENTED | Named search found no harnessed state or Infinity keyword behavior in the requested surfaces. |
| 702.186b | ∞ (Infinity) | UNIMPLEMENTED | Named search found no harnessed state or Infinity keyword behavior in the requested surfaces. |
| 702.187 | Mayhem | PARTIAL | Mayhem is approximated by graveyard alternate-cast scripts; discarded-this-turn identity and the costless play form are not generically modeled. |
| 702.187a | Mayhem | PARTIAL | Mayhem is approximated by graveyard alternate-cast scripts; discarded-this-turn identity and the costless play form are not generically modeled. |
| 702.187b | Mayhem | PARTIAL | Mayhem is approximated by graveyard alternate-cast scripts; discarded-this-turn identity and the costless play form are not generically modeled. |
| 702.187c | Mayhem | PARTIAL | Mayhem is approximated by graveyard alternate-cast scripts; discarded-this-turn identity and the costless play form are not generically modeled. |
| 702.188 | Web-slinging | UNIMPLEMENTED | Named search found no web-slinging alternate cost or tapped-creature return behavior in the requested surfaces. |
| 702.188a | Web-slinging | UNIMPLEMENTED | Named search found no web-slinging alternate cost or tapped-creature return behavior in the requested surfaces. |
| 702.189 | Firebending | UNIMPLEMENTED | Named search found no firebending attack trigger or end-of-combat retained-mana behavior in the requested surfaces. |
| 702.189a | Firebending | UNIMPLEMENTED | Named search found no firebending attack trigger or end-of-combat retained-mana behavior in the requested surfaces. |
| 702.189b | Firebending | UNIMPLEMENTED | Named search found no firebending attack trigger or end-of-combat retained-mana behavior in the requested surfaces. |
| 702.190 | Sneak | UNIMPLEMENTED | Named search found no Sneak declare-blockers alternate-cost/attack-destination behavior in the requested surfaces. |
| 702.190a | Sneak | UNIMPLEMENTED | Named search found no Sneak declare-blockers alternate-cost/attack-destination behavior in the requested surfaces. |
| 702.190b | Sneak | UNIMPLEMENTED | Named search found no Sneak declare-blockers alternate-cost/attack-destination behavior in the requested surfaces. |

## Per-verdict counts

- **IMPLEMENTED:** 260
- **PARTIAL:** 318
- **INCORRECT:** 32
- **UNIMPLEMENTED:** 97
- **N/A:** 47

## HIGHLIGHTS

1. **INCORRECT — Gift (702.174):** promised gifts are unreachable because equal normal/alternative costs never set the paid-alternative state; this breaks the gift effect and every conditional promised-gift branch.
2. **INCORRECT — Split second (702.61):** the stack auto-resolves the spell, suppressing mana abilities and special actions that the CR expressly permits and bypassing selective priority restrictions.
3. **INCORRECT — Deathtouch (702.2b):** lethal assignment and LKI exist, but the engine destroys during damage-event delivery instead of the required state-based action, creating ordering/regeneration interaction risk.
4. **INCORRECT — Plot (702.170):** plot exile is present, but known-failing cast permission leaves plotted cards unable to reliably be cast on a later turn.
5. **INCORRECT — Recover (702.59):** the macro parses but its known-failure witness returns neither the card nor the required exile result.
6. **INCORRECT — Ascend (702.131):** the city's blessing is represented as a battlefield token rather than an immutable player designation, making it interact with permanent counts/effects incorrectly.
7. **UNIMPLEMENTED — Companion (702.139):** no pregame reveal, deck-restriction validation, or outside-game-to-hand procedure; high impact for deck legality and opening state.
8. **UNIMPLEMENTED — Banding (702.22):** the name parses but attack grouping and damage-assignment behavior are unreachable; this is a catalogue-confirmed defect.
9. **UNIMPLEMENTED — Phasing (702.26):** the name and one-shot token parse, but no untap-step phase-out/phase-in transition exists; this is catalogue-confirmed.
10. **UNIMPLEMENTED — Poisonous (702.70):** no combat-damage poison trigger exists; the similarly named damage-replacement flag is not Poisonous.
11. **UNIMPLEMENTED — Champion (702.72):** the macro is explicitly marked not implemented and lacks the linked leave-return object behavior.
12. **UNIMPLEMENTED — Backup (702.165):** current card entries expose reminder text but no counter-plus-ability-copy behavior.
13. **UNIMPLEMENTED — Current speed package (702.178-179):** Max Speed and Start Your Engines! have no player-speed state, SBA, or inherent trigger.
14. **UNIMPLEMENTED — 2025-26 keyword wave:** Harmonize, Job Select, Tiered, Infinity, Web-slinging, Firebending, and Sneak have no engine behavior; Mobilize/Station/Warp/Mayhem are only partial card-script approximations.
