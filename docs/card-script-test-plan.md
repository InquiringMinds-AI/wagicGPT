# Card-Script Conformance Test Plan

## Purpose

This campaign verifies that every construct documented in `card-script-lexicon.md` is accepted by the parser and behaves according to its documented rules-engine semantics. A parser hit alone is not semantic coverage: tests must observe game state that would differ if shape, scope, timing, quantity, or ownership were wrong.

Lexicon tests live in `projects/mtg/bin/Res/test/lexicon/`. Run them without modifying the shared registry:

```bash
cd projects/mtg/bin
WAYLAND_DISPLAY=wayland-1 SDL_VIDEODRIVER=wayland \
SDL_AUDIODRIVER=dummy WAGIC_TESTSUITE=1 \
WAGIC_TESTSUITE_FILE=test/lexicon/_tests.txt ./wagic
```

Parser constructs with no production card may use test-only definitions loaded after the
normal card database and before the TestSuite name cache is built:

```bash
WAGIC_TESTSUITE=1 \
WAGIC_TESTSUITE_FILE=test/lexicon/_tests.txt \
WAGIC_TEST_PRIMITIVES_FILE=test/lexicon/test_primitives.txt ./wagic
```

The injection file uses ordinary `[card]` blocks, but every block must provide a unique
positive `id=` in `2100000000-2100000999` and a name that does not collide (case
insensitively) with production or synthetic cards. Missing fields, malformed blocks,
duplicate IDs or names, and production collisions fail startup. The variable has no effect
unless `WAGIC_TESTSUITE` is also set, so these cards cannot enter ordinary gameplay.

## Required Evidence per Construct

Each construct advances through four independently recorded levels:

1. **Parse:** canonical syntax produces the intended ability/cost/chooser.
2. **Semantic:** an end-to-end fixture proves the documented effect.
3. **Boundaries:** applicable zero, one, many, minimum, maximum, repeated-event, controller/opponent, and cleanup cases are covered.
4. **Interaction:** pairwise tests cover the rules systems the construct crosses, such as targeting protection, replacement/prevention, zones, the stack, alternate costs, and continuous-effect teardown.

A test must include a discriminating assertion or counterfactual. Merely casting a card is not coverage.

## Campaign Batches

| Batch | Surface | Required dimensions |
|---|---|---|
| P | Prevention and replacement | pooled vs continuous; combat/noncombat/all; `to`/`from`; expiry; multiple sources/events |
| T | Targets and zones | default/both-side scope; every composite zone; `other`; `^`; exact/up-to/any/X quantities; AND/OR/negated/dynamic restrictions |
| C | Mana and extra costs | colored/generic/colorless/hybrid/Phyrexian/snow/X; zero/partial/full payment; cancel; repeated and multi-object costs |
| R | Triggers | every event family; controller/opponent/static binding; `once`; `turnlimited`; repeated events; restrictions; source tapped/removed |
| D | Duration and wrappers | permanent, one-shot, UEOT, `uynt`, `uent`; `may`/`choice`; `if`/`ifnot`; `&&`; nested `ability$!` and `and!` |
| E | Leaf effects | each effect's state transition, target/player ownership, multiplicity, and `andAbility` continuation |
| B | 240 basic abilities | printed vs granted vs removed; combat, casting, zone, player-state, and death-rule families |
| A | Alternative-cost fields | normal vs alternative, paid/not-paid predicates, zone legality, resulting destination, and combined extra costs |
| X | Cross-feature matrix | representative pairwise combinations selected from the boundaries above; full-suite leak detection |

## Current Coverage

| Construct | Parse | Semantic | Boundaries | Interaction | Fixtures |
|---|---:|---:|---:|---:|---|
| `prevent:N` | existing | yes | pooled and cleanup | sequential damage | `prevention_pooled_shield`, `prevention_shield_expires` |
| `preventallcombatdamage` | existing | yes | combat vs noncombat | multiple attackers | `prevent_combat_only` |
| `preventallnoncombatdamage` | existing | yes | controller vs opponent | mass damage | `prevent_noncombat_only` |
| `@tappedformana` | existing | yes | controller/opponent, repeated | mana production | `trigger_tapped_for_mana_scopes` |
| `@tokencreated` | existing | yes | multiple tokens/event | token macro | `trigger_tokencreated_per_token` |
| `@counteradded` | existing | yes | once/event | evolve + draw | `trigger_counteradded_once_per_event` |
| exact `<N>` targets | existing | yes | enough vs insufficient | cast legality | `Rack_and_Ruin`, `Rack_and_Ruin2` |
| `<upto:N>` targets | existing | yes | one vs maximum | library search/cast | `Armillary_Sphere`, `Armillary_Sphere2`, `collected_conjuring_i1085` |
| `<anyamount>` targets | existing | yes | multiple selections | return/tap costs | `Charge_Across_the_Araba`, `jaddi_lifestrider_i1085` |
| X-sized `<prexx>` targets | existing | yes | X=0 vs X=1 | graveyard exile/copy | `hour_of_eternity_i1085`, `hour_of_eternity_x1` |
| `kicker=` / multikicker | existing | yes | unpaid, paid, repeated | conditional effect/ETB counters | `kicker`, `kicker2`, `Apex_Hawks_MULTIKICKER` |
| `flashback=` | existing | yes | graveyard cast | post-resolution exile | `dematerialize_flashback` |
| `buyback=` | existing | yes | unpaid vs paid | graveyard vs hand destination | `buyback`, `whispers_of_the_muse` |
| `{cycle}` / `@cycled` | existing | yes | base and triggered cycling | discard, draw, target effect | `cycling`, `cycling2` |
| `{s(target)}` additional cost | existing | yes | land and typed creature | payment before spell effect | `shard_volley`, `natural_order` |
| undying | existing | yes | no +1/+1 counter at first death | Butcher Ghoul returns, attacks next turn as a 2/2, proving the +1/+1 counter return state | `basic_undying_returns_with_counter` |
| hybrid mana | existing | yes | colored and generic branches | permanent and token spell payment | `hybrid_mana`, `hybrid_mana_2`, `hybrid_mana_3` |
| `{l:N}` / X life payment | existing | yes | affordable and repeated-payment refusal | draw tied to life paid | `necrologia_i1085`, `griselbrand` |
| X-sized `E(<prex>...)` cost | existing | yes | X=1 and X=2 | multi-card exile, draw, and life loss | `skeletal_scrying_i1085`, `skeletal_scrying_x2_i1085` |
| affinity artifacts and land types | existing | yes | reduced generic cost reaches exact remaining mana | artifact, Swamp, Plains, Forest, Mountain, and Island affinities allow otherwise underpaid Golem/Frogmite casts and consume the remaining mana | `Frogmite`, `dross_golem`, `razor_golem`, `tangle_golem`, `affinity_mountains_oxidda_golem`, `affinity_islands_spire_golem` |
| `@movedto` | existing | yes | enter vs control change | self exclusion and zone movement | `triggers_at_movedto_i179` |
| `@untapped` | existing | yes | multiple permanents/event sequence | temporary power modification | `trigger_untapped` |
| `@drawn` | existing | yes | opponent scope | draw followed by damage | `trigger_drawn` |
| `@combatdamaged` | existing | yes | combat vs noncombat | player damage and optional draw | `trigger_combatdamaged_1`, `trigger_combatdamaged_2` |
| `aslongas(TC)` | existing | yes | threshold retained vs lost | continuous P/T teardown | `Auriok_Sunchaser_ASLONGAS_1`, `Auriok_Sunchaser_ASLONGAS_2` |
| `EFFECT_A && EFFECT_B` | existing | yes | either player's target | move plus target-controller draw | `Call_to_Heel_1`, `Call_to_Heel_2` |
| nested target / `choice` wrappers | existing | yes | distinct target and selected mode | two-target spell and modal destroy | `agony_warp_i1085`, `heartless_act_choice` |
| `(blink)` | existing | yes | leaves and returns | ETB retrigger | `blink_retriggers_etb` |
| `fizzleto(ZONE)` | existing | yes | exile, hand, library top | spell countering and destination | `fizzleto_exile`, `fizzleto_hand`, `fizzleto_librarytop` |
| continuous control | existing | yes | controlled creature removed/dies | Aura teardown and ownership | `control_magic`, `control_magic2` |
| `copy` | existing | yes | own vs opponent creature | characteristics and combat | `clone`, `clone2` |
| P/T switch and UEOT layering | existing | yes | multiple ordered modifiers | combat then cleanup | `PTswitch` |
| `token(...)` | existing | yes | multiplicity two | spell resolution and token identity | `dragon_fodder` |
| combat basic abilities | existing | yes | attacker/blocker and solo/multiple roles | flanking, bushido, exalted | `flanking`, `bushido_1`, `bushido_2`, `exalted_i143` |
| deathtouch | existing | yes | noncombat damage | granted damage ability and source identity | `deathtouch_taught_i1142` |
| indestructible | existing | yes | lethal toughness reduction | state-based death vs destruction immunity | `indestructible_2_i570` |
| legendary | existing | yes | same controller vs opposing controllers | legend choice and graveyard movement | `legendary` |
| double strike / regeneration | existing | yes | first-strike damage and regenerated blocker | blocked status in second damage step | `double_strike_vs_regen_i563` |
| shroud / equipment | existing | yes | attach before and after shroud | targeting legality and combat | `equip_shroud2` |
| countershroud | existing | yes | positive vs prohibited counter family | mass counter placement | `countershroud` |
| forced actions | existing | yes | attack and mandatory target | phase action and ETB choice | `attacks_each_turn`, `must1` |
| living weapon | existing | yes | token creation and attachment | equipment-derived combat stats | `livingweapon` |
| fading | existing | yes | final counter removal | upkeep sacrifice | `fading` |
| modular | existing | yes | source death and counter transfer | artifact target and combat stats | `arcbound` |
| sunburst | existing | yes | five colors spent | Arcbound Wanderer enters with five +1/+1 counters and deals 5 combat damage with haste | `sunburst` |
| proliferate | existing | yes | multiple selected permanents | counters, death, and combat stats | `Contagion_Clasp` |
| morph / face-up | existing | yes | face-down stats, turn-up survival, retained modifiers | casting, static pump, combat | `Morph#1`, `Morph#2`, `Morph#3` |
| `becomes(...)` | existing | yes | color and creature-form changes | target legality, flying, Moat | `becomes_transform_i559`, `becomes_transform_i559_2` |
| first strike | existing | yes | lethal first strike vs surviving blocker | two combat-damage steps | `first_strike`, `first_strike2` |
| lifelink | existing | yes | unblocked combat damage | simultaneous life gain | `lifelink` |
| regeneration | existing | yes | ordinary lethal vs wither counters | combat destruction and state-based death | `regenerate`, `regenerate_wither_i146` |
| unearth | existing | yes | haste, timing restriction, cleanup | graveyard return and exile replacement | `dregscape_zombie_unearth`, `unearth2`, `unearth3` |
| `exiledeath` | existing | yes | delayed death replacement after temporary battlefield return | unearthed Dregscape Zombie attacks, survives to cleanup, then is exiled instead of returning to graveyard | `unearth3` |
| `librarydeath` | existing | yes | replacement occurs before next draw | Gravebane Zombie dies to targeted removal, moves to library top, and Archivist draws it instead of the pre-existing library cards | `Gravebane_Zombie` |
| `bottomlibrarydeath` | synthetic | yes | pre-existing top card remains above dying creature | test-only creature dies, Archivist draws the existing Forest, and the creature is left as the only library card | `basic_bottomlibrarydeath_draws_existing_top` |
| `shufflelibrarydeath` | existing | yes | competing exile replacement vs self-shuffle replacement | Darksteel Colossus is destroyed under Leyline of the Void and appears in its owner's shuffled library instead of graveyard or exile | `leyline_of_the_void` |
| `handdeath` | synthetic | yes | destroyed creature avoids graveyard | test-only creature is destroyed by Wrecking Ball and moves to owner hand while the spell goes to graveyard | `basic_handdeath_returns_to_hand` |
| `inplaydeath` | synthetic | yes | battlefield return is untapped | test-only creature is destroyed, briefly changes zones, and returns to battlefield with zero tapped permanents | `basic_inplaydeath_returns_untapped` |
| `inplaytapdeath` | synthetic | yes | battlefield return is tapped | test-only creature is destroyed, returns to battlefield, and is the only tapped permanent | `basic_inplaytapdeath_returns_tapped` |
| `gainedhanddeath` | synthetic | yes | granted ability before lethal damage | test-only instant grants `gainedhanddeath`, deals lethal damage, and the damaged Bears move to owner hand while the instant goes to graveyard | `basic_gainedhanddeath_granted_damage` |
| `gainedexiledeath` | synthetic | yes | granted ability before lethal damage | test-only instant grants `gainedexiledeath`, deals lethal damage, and the damaged Bears move to exile while the instant goes to graveyard | `basic_gainedexiledeath_granted_damage` |
| graveyard-exile replacement | existing | yes | self-removal and competing replacement | destroy, shuffle replacement, destination | `leyline_of_the_void`, `restinpeace` |
| graveyard-to-battlefield movement | existing | yes | selected card | spell destination | `resurrection` |
| play from graveyard | existing | yes | one land per turn | hand vs graveyard legality | `crucible_of_worlds` |
| `fizzle` | existing | yes | single and multiple stack interactions | countered spell destination | `counterspell`, `counterspell2` |
| control-sensitive opponent | existing | yes | source changes controller | combat trigger and discard | `ExchangeController` |
| equipment | existing | yes | attach and derived stats | blocking and trample damage | `equipment_i252` |
| `@noncombatdamaged` | existing | yes | spell damage then combat | trigger pump distinction | `trigger_noncombatdamaged` |
| fear | existing | yes | invalid blocker | evasion and combat damage | `fear` |
| persist | existing | yes | first/second death and static pump | zone return and -1/-1 counter | `persist`, `persist2`, `persist3` |
| wither | existing | yes | damage counters survive cleanup | later combat stats | `wither` |
| flying/reach restrictions | existing | yes | flying removed while attacking | ordinary vs flying-only blocker | `gravity_well`, `gravity_well2` |
| protection | existing | yes | Aura and Equipment attachment | state-based detach/graveyard actions | `pledge_protection` |
| storm | existing | yes | two prior spells | copy multiplicity and ETB life gain | `hunting_pack` |
| devour | existing | yes | multiple sacrificed creatures | counters and combat power | `thunder-thrash_elder` |
| evoke | existing | yes | alternate cast and sacrifice | ETB target effect and combat | `briarhorn` |
| temporary granted fear | existing | yes | post-cleanup removal | next-turn blocking legality | `duskwalker` |
| source-specific prevention | existing | yes | large and first-strike sources | combat prevention | `circle_of_protection`, `circle_of_protection_i211` |
| conditional hexproof | existing | yes | controller vs opponent turn | target rejection and fallback target | `Oak_Street_Innkeeper` |
| draw then library placement | existing | yes | three drawn, two returned | ordered multi-step resolution | `brainstorm` |
| search then random discard | existing | yes | searched card is only discard candidate | sequencing across hand/library/graveyard | `gamble` |
| mill | existing | yes | multiple top cards | opponent library-to-graveyard movement | `millstone` |
| sacrifice ignores shroud | existing | yes | non-targeting cost | sacrifice plus UEOT pump | `sacrifice` |
| `{t(target)}` cost | existing | yes | another permanent pays | draw and later attack restriction | `tap_other_card_as_cost` |
| `{l2e}` cost | existing | yes | exactly ten repeated payments | library exile before damage | `cost_exile_library_arc_slogger` |
| `{p(color)}` cost | existing | yes | life-payment branch | life loss, source tap, target tap | `cost_phyrexian_life_branch` |
| `{e:N}` cost | existing | yes | exact available payment | attack trigger and token creation | `cost_energy_payment` |
| `{saclands}` cost | existing | yes | source plus multiple other lands | all sacrificed before token creation | `cost_sacrifice_all_lands` |
| `{m}` cost + source return | existing | **fails** | four payments succeed | subsequent `all(this) moveto(...)` fails | `cost_mill_rot_farm_skeleton` in `_known_failures.txt` |
| `showfromtoplibrary` + top-library play permissions | mixed | yes | land, creature, any, instant/sorcery, artifact, Aura/Equipment, and enchantment type gates | the single top library card can be selected and leaves the library only under the matching permission; Courser also proves landfall from a top-library land | `top_library_land_courser`, `top_library_creature_garruks_horde`, `top_library_any_synthetic`, `top_library_instant_sorcery_synthetic`, `top_library_artifact_synthetic`, `top_library_auraequip_synthetic`, `top_library_enchantment_synthetic` |
| multi-zone mass movement | existing | yes | both players | exile, sacrifice, and battlefield return | `living_death_i1085` |
| mass Aura return | existing | yes | invalid then valid attachment | graveyard selection, tokens, attachment | `Replenish` |
| put permanent from hand | existing | yes | invalid Aura choice | spell resolves without illegal movement | `Show_and_Tell` |
| `moveto(hand)` | existing | yes | typed graveyard target | sorcery identity after movement | `deja_vu` |
| copy edge cases | existing | yes | shroud and layered lord stats | non-targeting choice and P/T layers | `clone3`, `clone4` |
| continuous-control recalculation | existing | yes | dynamic stats and destroyed stolen token | lord state and ownership teardown | `control_magic3`, `control_magic4` |
| `@lifed` | existing | yes | amount-sensitive life gain | counters and combat power | `ageless_entity` |
| dynamic ETB value | existing | yes | static toughness modifier included | `@movedto` plus dynamic life gain | `angelic_chorus_TOUGHNESSLIFEGAIN` |
| `@damaged` | existing | yes | surviving and dying damaged creature | life gain, destruction, source survival | `dromad_purebred`, `hot_soup_i1085`, `mirri_the_cursed2_i284` |
| stack-entry trigger | existing | yes | exactly once per opponent cast | token creation and controller scope | `dragonlair_opponent_cast` |
| `@sacrificed` | existing | yes | scripted and extra-cost sacrifice | counter listener and tagged exile/return | `kathari_bomber_sacrifice_i1146`, `prowling_geistcatcher_i1085` |
| taught-trigger `this` scope | existing | yes | two matching permanents | only damaged instance retriggers | `spiteful_sliver_scope_i1085` |
| `@tapped` | existing | yes | mana ability also taps | opponent-controlled listener | `lifetap` |
| `@movedto ... from(...)` | existing | yes | battlefield-to-graveyard | opponent-controlled source and return to hand | `rancor_i749` |
| `[fresh]` history restriction | existing | yes | this-turn deaths vs older graveyard cards | token multiplicity | `fresh` |
| zone/type restriction | existing | yes | nonland in hand vs land | reveal and exile destination | `memory_leak_i1085` |
| dynamic minimum restriction | existing | yes | least toughness vs larger candidate | upkeep target selection | `purging_scythe_i1085` |
| modal target restriction | existing | yes | one chosen mode | nonland permanent and owner destination | `run_ashore_i1085` |
| numeric toughness restriction | existing | yes | toughness at threshold | conditional choice and destroy | `valorous_stance_family` |
| source-zone movement restriction | existing | yes | graveyard-to-battlefield prohibited | replacement back to graveyard | `weathered_runestone_i1085` |
| attacking-player/creature restriction | existing | yes | active opponent and selected attacker | sacrifice and toughness-valued tokens | `entrapment_maneuver_i1085` |
| characteristic-defining P/T | existing | yes | layer override and damage after set P/T | CDA, set values, and UEOT pump | `CDA#1`, `CDA#2` |
| hellbent / toughness gates | existing | yes | hand nonempty vs empty; low vs high toughness | exact two-target damage | `twinstrike`, `twinstrike2`, `twinstrike3` |
| `{t}` source cost | existing | yes | source starts untapped | source tap before targeted damage | `cost_tap_source` |
| `{s}` source cost | existing | yes | source sacrifices itself | graveyard movement before targeted damage | `cost_sacrifice_source` |
| `{unattach}` cost | existing | yes | attached Equipment | detach before combat prevention | `cost_unattach_equipment` |
| `{crew(target)}` cost | existing | yes | eligible untapped creature | payer tapped and Vehicle becomes targetable creature | `cost_crew_target` |
| `{delve}` cost | existing | yes | five repeated graveyard payments | exile, generic reduction, colored remainder | `cost_delve_multi_card` |
| `{improvise}` cost | existing | yes | seven distinct artifacts | all tapped to pay generic cost without mana | `cost_improvise_multi_artifact` |
| `{convoke}` cost | existing | yes | ten distinct creatures | generic reduction plus colored remainder | `cost_convoke_multi_creature` |
| true-colorless `{c}` | existing | yes | true-colorless success vs colored rejection | spell legality and P/T effect | `mana_true_colorless_success`, `mana_true_colorless_rejects_colored` |
| snow `{i}` | existing | yes | snow-source payment vs ordinary mana rejection | activated-ability legality | `mana_snow_source_payment`, `mana_snow_rejects_nonsnow` |
| `{c(counterspec...)}` cost | existing | yes | add and remove directions | untap/death threshold and damage activation | `cost_add_counter`, `cost_remove_counter` |
| color-restricted `{x:color}` | existing | yes | white X=3 vs wrong-color X=0 | payment retention, tap, and life amount | `mana_color_restricted_x_success`, `mana_color_restricted_x_rejects_wrong_color` |
| `{d}` random-discard cost | existing | yes | one-card deterministic hand | discard before untap at legal upkeep | `cost_random_discard` |
| `{d(target)}` chosen-discard cost | existing | yes | selected card | Aura-taught tap cost and replacement draw | `cost_discard_chosen_card` |
| `{s2l(target)}` cost | existing | yes | card selected from hand | library placement before source return | `cost_to_library_from_hand` |
| `{s2g(target)}` cost | existing | yes | opponent-owned exiled card | graveyard movement before lethal spell effect | `cost_to_grave_from_opponent_exile` |
| `{h(target)}` cost | existing | yes | controlled land | return to hand and granted-trample combat overflow | `cost_bounce_land_grants_trample` |
| `other=` alternative cost | existing | yes | restriction met vs unmet | hand exile and zero-mana cast legality | `vine_dryad`, `Lethargy_Trap`, `Lethargy_Trap2` |
| `retrace=` | existing | yes | graveyard cast | land discard, mana payment, token creation, return to graveyard | `cost_retrace_land_discard` |
| `bestow=` | existing | yes | alternate cast mode | attachment and granted +1/+1 combat damage | `cost_bestow_attaches` |
| `suspend(N)=` | existing | **potential failure** | cost and exile succeed | final counter is not removed at controller upkeep | `cost_suspend_one_casts` in `_potential_tests.txt` |
| `{chosencolor}` mana | existing | **potential / harness gap** | choice resolves | generated tap ability is not committed by TestSuite | `mana_chosen_color_persists` in `_potential_tests.txt` |
| `@next upkeep` | existing | yes | permanent and spell-created delayed triggers | draw multiplicity and controller binding | `pyknite_i426`, `arcane_denial_i1126` |
| `@next end` | existing | yes | original vs copied delayed rider | destruction, exile, and token-copy interaction | `puffer_extract_i1085`, `populate_zektar_i1145` |
| `@rebounded` | existing | yes | hand cast vs rebound cast | initial exile, next-controller upkeep, optional free recast, final graveyard | `trigger_rebounded_recasts` |
| `@lifelostfoeof` | existing | yes | one opponent life-loss event | `thatmuch` amount binding without a reciprocal loop | `trigger_lifelost_amount` |
| `@targeted ... from(...)` | existing | yes | equipped creature targeted by spell | taught trigger, source restriction, damage before creature death | `livewire_lash_i1085` |
| `@vampired ... from(...)` | existing | yes | source-damaged creature later dies | activated damage, death attribution, counter/combat consequence | `[at]Vampired#1` |
| `@discarded` | existing | yes | chosen discard from activated loot | replacement exile and subsequent draw | `bag_of_holding_i1085` |
| `@phasedin(this)` | existing | **potential failure** | phase-out occurs | phase-in event does not fire the source's draw trigger | `trigger_phasedin_draws` in `_potential_tests.txt` |
| `@facedup` | existing | yes | manifested permanent turned face up | trigger amount counts creatures after reveal | `trigger_facedup_life` |
| `@transformed` | existing | yes | equipped Werewolf transforms | listener Equipment flips and changes combat damage from 3 to 5 | `trigger_transformed_equipment` |
| `@coinflipped` | existing | **potential failure** | spell resolves | no coin-flip or result branch is created | `trigger_coinflipped_branch` in `_potential_tests.txt` |
| `@counterremoved` | existing | yes | two counters removed in one damage event | two delayed triggers add four counters and change later combat damage | `trigger_counterremoved_delayed` |
| `@energizedof` | existing | yes | controller gains two energy | non-targeted listener changes Gorger combat damage from 2 to 4 | `trigger_energized_controller` |
| rampage | existing | yes | one, two, and three blockers; first strike | per-extra-blocker scaling, survival, cleanup, and life gain from final power | `rampage`, `rampage2`, `rampage3`, `rampage_vs_first_strike` |
| vigilance | existing | yes | animated land attacks | attacker remains untapped while four mana creatures stay tapped | `mobilized_district_i1085` |
| haste | existing | yes | creature cast this turn | immediate attack and end-step sacrifice | `spark_elemental` |
| unblockable | existing | yes | opposing blocker available | combat connection and granted draw rider | `open_into_wonder_i1085` |
| shadow / reachshadow | existing | yes | ordinary vs shadow-capable blocker | ordinary blocker rejected; Wall of Diffusion blocks shadow | `jodahs_avenger`, `wall_of_diffusion2` |
| flash / `asflash` | existing | yes | granted flash before and after source removal | Quick Sliver permits a Sliver cast during untap, then teardown prevents a second Sliver after Quick Sliver dies | `quick_sliver_FORCEDALIVE` |
| menace | existing | yes | one vs two blockers | single blocker rejected; two distinct blockers accepted and combat resolves | `basic_menace_one_blocker`, `basic_menace_two_blockers` |
| intimidate | existing | yes | off-color creature vs artifact creature | colored blocker rejected; artifact blocker accepted | `basic_intimidate_colored_blocker`, `basic_intimidate_artifact_blocker` |
| horsemanship | existing | **potential failure** | ordinary blocker rejected | matching horsemanship blocker is also rejected | `basic_horsemanship_ordinary_blocker`; `basic_horsemanship_blocking` in `_potential_tests.txt` |
| cloud | existing | yes | ground attacker vs flying attacker | Cloud Sprite is rejected as a ground blocker but trades with a flying attacker | `basic_cloud_rejects_ground_block`, `basic_cloud_accepts_flying_block` |
| `cantblock` | existing | yes | blocker declaration attempted | attacker connects while prohibited blocker remains in play | `basic_cantblock_rejects_block` |
| one-blocker restriction | existing | yes | two blockers attempted | only one blocks and dies; second remains | `oneblocker` |
| strong | existing | yes | lower-power vs higher-power blocker | lower rejected; higher accepted and kills attacker | `basic_strong_rejects_lower_power`, `basic_strong_accepts_higher_power` |
| weak | existing | yes | higher-power vs equal-power attacker | higher rejected; equal accepted and trades | `basic_weak_rejects_higher_power`, `basic_weak_accepts_equal_power` |
| skulk | existing | **potential boundary failure** | greater-power blocker rejected | equal-power blocker is also rejected | `basic_skulk_rejects_higher_power`; `basic_skulk_accepts_equal_power` in `_potential_tests.txt` |
| `nosolo` | existing | yes | alone vs accompanied | solo attack rejected; paired attack deals combined damage | `basic_nosolo_rejects_alone`, `basic_nosolo_accepts_companion` |
| `mustblock` | existing | yes | no blocker selected manually | mandatory blocker is assigned and both creatures die | `basic_mustblock_automatic` |
| basic landwalks | existing | yes | matching land plus available blocker | Forest, Island, Mountain, Swamp, and Plains walkers connect | `landwalk`, `basic_islandwalk`, `basic_mountainwalk`, `basic_swampwalk`, `basic_plainswalk` |
| nonbasic / legendary landwalk | existing | yes | matching land plus available blocker | nonbasic and legendary land predicates bypass blocker | `basic_nonbasiclandwalk`, `basic_legendarylandwalk` |
| snow/desert landwalk | synthetic | yes | matching Snow or Desert land plus available blocker | canonical parser-table abilities bypass blocker; Desert Nomads uses noncanonical `desertwalk`, so canonical `desertlandwalk` is covered by test-only primitive | `basic_snowlandwalk_synthetic`, `basic_desertlandwalk_synthetic` |
| snow subtype landwalk | existing | yes | snow Forest, Swamp, Island, Plains, and Mountain | subtype-specific snow landwalk bypasses blocker; Zombie Musher proves the remaining production multi-ability carrier | `basic_snowforestlandwalk`, `basic_snowswamplandwalk`, `basic_snowislandlandwalk_zombie_musher`, `basic_snowplainslandwalk_zombie_musher`, `basic_snowmountainlandwalk_zombie_musher` |
| islandhome | existing | partial | defending player lacks Island | Dandan is kept alive by controller Island and rejected when the defending player has no Island; the positive defending-Island boundary crashes during fixture setup and is isolated in `_potential_tests.txt` | `basic_islandhome_rejects_no_defending_island`; `basic_islandhome_accepts_defending_island` in `_potential_tests.txt` |
| `uynt` duration | existing | yes | current controller turn, opponent turn, next controller turn | power modifier and two attacks | `duration_until_your_next_turn` |
| `uent` duration | existing | yes | current turn through controller's next turn | exile permission beyond UEOT | `duration_until_next_end_turn_persists` |
| `mycastingzone` | existing | yes | effect active then source removed | white spell cost increase and teardown | `alabaster_leech`, `alabaster_leech2` |
| `nonbattlezone` | existing | yes | sorcery in hand | cost reduction and draw resolution | `zone_nonbattlezone_cost_reduction` |
| `myzones` | existing | yes | resolving controller-owned instant | damage-source trigger and token creation | `zone_myzones_damage_source` |
| `opponentzones` | existing | yes | opposing permanent ability | target trigger occurs before lethal damage | `zone_opponentzones_target_source` |
| landwalk | existing | yes | defending player controls matching land | blocker rejection and combat damage | `landwalk` |
| trample | existing | yes | ordinary, indestructible, and multiple blockers | lethal assignment and excess player damage | `trample`, `trample_vs_indestructible`, `trample_vs_multiblock` |
| reach | existing | yes | Equipment-granted reach | flying blocker legality and combat death | `equip_reach` |
| first strike with multiple attackers | existing | yes | first-striker plus ordinary attacker | blocker death and unblocked damage | `first_strike3` |
| spiritlink vs lifelink | existing | yes | own/opponent creature and duplicate grants | controller binding and stacking rules | `SPIRITLINK_keyword#1`, `SPIRITLINK_keyword#2`, `SPIRITLINK_keyword#3` |
| conditional blocking restriction | existing | yes | high-power attacker with shroud | non-targeting blocker prohibition | `ironclaw_orcs` |
| infect vs countershroud | existing | yes | wither-style combat counters prohibited | state-based survival | `meliras_keepers` |
| color protection in combat | existing | yes | protected blocker vs black attacker | damage prevention and first strike | `white_knight1` |
| chosen-player protection | existing | yes | protected vs unprotected selection | identical opposing spell counterfactual | `tnn_blocks_chosen`, `tnn_protection_direction` |
| `@combat(blocked/blocking)` | existing | yes | source attacks vs blocks | distinct event direction and UEOT stats | `trigger_blocked`, `trigger_blocking` |

“Existing” parse evidence means production primitives already load; dedicated parser-contract assertions remain a later batch. As of 2026-07-08, the conformance registry passes 250 unique tests with zero failures. One confirmed failure is isolated in `_known_failures.txt`, and twelve additional unconfirmed/harness-blocked reproducers are listed in `_potential_tests.txt`. The complete suite previously passed 950 rules tests and 4 AI tests with zero failures before this continuation batch was added.

This milestone is broad but not complete lexicon certification. The largest open surfaces are unrepresented extra-cost tokens, composite zones and restriction predicates, uncommon event triggers, duration forms `uynt`/`uent`, the remaining leaf effects, and most of the 240-entry basic-ability table. A row is added above only when its fixture contains a state assertion that distinguishes the construct's documented behavior.

### Explicit Extra-Cost Backlog

Still requiring dedicated semantic witnesses: `{q}`, `{q(target)}`, `{n}`, `{emerge}`, subtype offerings, and dynamic `{value:...}` / `{eval(...)}` mana. Targeted discard/exile variants need zero/partial/full/cancel and repeated-object boundaries. The `{m}` payment itself is observed, but its only selected witness currently fails in the following payload and remains isolated below.

Known-failure witnesses live in `test/lexicon/_known_failures.txt`. Rot Farm Skeleton currently mills four cards correctly, proving repeated `{m}` payment, but remains in the graveyard because its `all(this) moveTo(myBattlefield)` payload does not move the resolving graveyard source. Comparable working graveyard abilities use direct `moveto(mybattlefield)`.

Confirmed and unconfirmed observations are tracked separately in `docs/card-script-potential-issues.md`; potential entries require investigation but do not make the green conformance registry fail.

### Explicit Composite-Zone Backlog

The parser recognizes several composite zones with no production-script occurrence, so current cards cannot exercise them directly: `mybattlefieldhand`, `myrestrictedcastingzone`, `mycommandplay`, `myhandlibrary`, `mygravelibrary`, `opponentgravelibrary`, `opponentgraveexile`, `opponentrestrictedcastingzone`, `opponentcommandplay`, `opponenthandlibrary`, `opponentnonplaynonexile`, `myhandexilegrave`, and `opponenthandexilegrave`. `mygraveexile` appears only as a basic-ability name, not a target-zone expression. Completing these rows requires test-only primitives or parser-level target-chooser tests. `mynonplaynonexile` still has a production Mishra path awaiting a deterministic fixture.

## Failure Classification

For every failure, identify one of: fixture/driving error, incorrect primitive, lexicon error, parser defect, or rules-engine defect. Verify real Oracle text before changing a production primitive. Prefer a faithful card-script correction; change shared engine behavior only when no documented construct can express the rule correctly. Preserve failing witnesses until the mechanism is understood.
