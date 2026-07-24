# Lexicon test campaign — engine bug catalogue (post fix-pass, 2026-07-18)

## ORACLE DRIFT SWEEP — WAVE 1 (planeswalker-damage errata, 2026-07-19)

Sweep tooling: `tools/oracle_sweep.py` + `tools/oracle_judge.py`; buckets in
`strategy-design/oracle-sweep/`. 244 confirmed functional-drift cards; wave 1 =
the 118-card planeswalker-damage class ("target player" → "any target" /
"player or planeswalker" 2017 errata).

**SHIPPED (probe-verified):** 85 mechanical widenings of EXISTING choosers
(`target=player`→`player,planeswalker`, `target(opponent)`→
`target(opponent,planeswalker)`, 2× `anytarget`) + Kiss of Death / Searing
Flesh / Stolen Grain / Vampiric Touch converted to spell-level
`target=opponent,planeswalker` + Keldon Marauders' choice-ladder replaced with
`damage:1 target(player,planeswalker)` + Relic Bind choice widened. Probes
registered in main `_tests.txt`: blightning_pw, searing_flesh_pw,
keldon_champion_pw, keldon_marauders_pw.

**ENGINE FIXED:** TargetChooserFactory had NO opponent+planeswalker form —
`target=opponent,planeswalker` SILENTLY parsed as opponent-only
(`s.find("opponent")==0` matched first; loud-rejection doctrine case). Added
`opponentOnly` flag to DamageableTargetChooser + factory branch for
`opponent,planeswalker` / `planeswalker,opponent` (TargetChooser.{h,cpp}).

**NEW ENGINE GAP — dual-interactive-ETB fizzle:** when a card has TWO ETB
auto lines that each need an interactive chooser, only ONE StackAbility ever
stacks — the other leg silently fizzles (probe-proven on Sparkcaster
bounce+damage and Manticore counter+damage, both orders). Consequence: cards
whose Oracle needs a second targeted leg CANNOT be upgraded from non-targeted
`damage:N opponent` without losing the other leg. REVERTED to pre-existing
1v1-approximate scripts: Sparkcaster, Manticore of the Gauntlet, Ravager of
the Fells (same shape via its may-damage second line).

**NEW ENGINE GAP — trigger-payload targeted conversion fizzles:** converting a
non-interactive trigger payload (`damage:N opponent`) to
`target(...)` makes the trigger fire with an unanswered chooser → effect
becomes a NO-OP (probe-proven on Scalding Tongs upkeep trigger; `p2` player
click in suite does not reach the chooser). REVERTED: Scalding Tongs,
Thumbscrews, Mogg Maniac, Wall of Souls. These need verified
trigger-target machinery (how does AIPlayerBaka answer mid-trigger choosers?)
before conversion.

**NEW ENGINE GAP — attacked-entity primitive missing:** Hellrider / Raid
Bombardment / Mage Slayer / Myr Battlesphere need "the player or planeswalker
it's attacking". Engine tracks it (`MTGCardInstance::isAttacking` is
`Targetable*`, planeswalker attacks exist via MTGPlaneswalkerAttackRule) but no
script token exposes it. Scripts stay `damage:N opponent` (correct vs players,
wrong when a planeswalker is attacked).

### WAVE 2 (remaining 126 DIFFERENT cards, 2026-07-19)

Four Opus proposal agents (per-class briefs in `oracle-sweep/wave2/`), edits
applied centrally (100 edit dicts, 0 apply failures). Verdict totals:
TEXT_ONLY 49 (faithful scripts, stale display text — the judge compares
text= lines), SCRIPT_FIX 34, OK_AS_IS 12 (incl. name-collision false flags:
Blood token vs Flesh//Blood, Fast//Furious joke printing, Day/Night =
engine-internal daybound emblems — future sweep exclusion rule), ENGINE_BLOCKED
6, UNSUPPORTED_STUB 25. Notable real fixes: Lanterns' Lift was missing its
entire +1/+1+flying teach (probe lanterns_lift.txt registered); Soulhunter
Rakshasa flat 5 → per-Swamp dynamic damage (probe soulhunter_swamps.txt);
Enatu Golem life 3→4; generic-vs-colorless family `add{1|2}` → `add{C}...`
(Contested Cliffs, Mana Seism, Bounty of the Luxa, Mana Screw, Weaver of
Currents, Soldevi Machinist); Fallen Ferromancer + Assemble from Parts cost
drift (web-verified); optional-trigger errata via `may` (Soul's Attendant,
Leyline of Vitality, Awakening Zone; Pirated Copy inverse). ENGINE_BLOCKED
ledger: Soul of Shandalar linked-controller chain, Nicol Bolas -9
targetedplayer→pw-controller, Grizzled Wolverine blocked-gated activation,
Arms Scavenger equip-cost reduction (#MISSING), Liliana of the Veil -6
two-pile, Flaccify half-mana cost. Residual watch: Old-Growth Troll nested
create-then-tap rider unprobed (failure mode = status quo untapped token);
Expedition Supplier self-ETB inclusion unprobed.

**RESIDUE (complex composites, unconverted, logged):** Heart of Bogardan,
Eternal Flame, Goblin Lyre, Landslide (second-target limit), Flames of the
Blood Hand, Chandra Pyromaster (+1 choice-ladder), Curse of the Pierced Heart.
OK-as-is: Captain's Maneuver, Pyromancy (already anytarget). N/A: 7
unsupported.txt stubs. Remaining sweep classes (scope 35, numbers 33, other
27, timing 21, ability 6, target/zone 4) NOT yet worked — see
oracle-sweep/verdicts.tsv.

## FIX WAVE RESOLUTION LEDGER (2026-07-19) — authoritative overlay on the items below

The implemented-but-wrong pile was worked by five worktree agents, integrated,
and verified (lexicon 805/0/0, main 1010+8/0). Dispositions:

**ENGINE FIXED (5):** #17 donothing now composes its `and!` rider (MultiAbility);
#19-partial: an EMPTY-primary `and!()` payload now parses as the effect
(Finality); #14 totemarmor now destroys the consumed Umbra (self-finding
ATriggerTotem, the un-drivable chooser removed); #34 oppnomaxhand no longer
cleared when the beneficiary's battlefield is empty but the opponent grants it;
#24 manifest dread's mill leg now fires (missing fireOneShot in
MTGRevealingCards::CheckUserInput). Harness bonus: zone lists accept `^` as an
escaped comma, so comma-named cards are placeable (`inplay:Anafenza^ the
Foremost`).

**NOT ENGINE BUGS — witness/data defects, fixed as such (8):** #2 @mutated (the
trigger machinery is CORRECT; the witnesses declined their own `may` prompt —
driving fixed); #13 combattoughness (comma-split "Doran, the Siege Tower" never
entered play; keyword verified with Assault Formation); #15 noloyaltydamage
(fixture setup artifact — Spark Rupture's ETB draw stole Jace from the library;
the NOLOYALTYDAMAGE keyword remains registered-but-unconsumed, moot because the
transform dominates); #16 alterdevoffset (symmetric all along — Altar of the
Pantheon's own tap-ability contaminated the observation); #18 count/countb (the
plumbing is correct; witnesses used the invented bare form — real form is
`count(type:creature:mybattlefield)`); #25 echo (malformed `_ECHO_` macro —
fixed to the real `upcost[...;next upkeep]` form; engine pay-path correct); #28
cantbetargetof (invented persistent-activated form; faithful idiom is
`transforms((,newability[cantbetargetof(*[color])])) ueot`); #35 Anafenza (the
comma trap again; regression fixture added on the `^` escape).

**ALREADY FIXED / STALE (1):** #20 exploits — passes at HEAD, no bystander
sacrifice, @exploited fires.

**REMAINING FROM #19 (witness rework queued, not engine):** the if/then/else
parser absorbs a trailing `&& rider` into its else-branch — this only affects
the synthetic observability riders (real amass/monarch cards don't use the
shape, verified); their witnesses should observe tokens/counters directly.
Devastating Summons' script is unfaithful to its Oracle (sacrifice is an
ADDITIONAL COST, not a resolution effect) — re-script it, which sidesteps the
synchronous-notatarget gap entirely.

**POST-INTEGRATION PROBE RESULTS (2026-07-19):** Rampaging Ferocidon PASS
(entering creature pings its controller; `other` self-exclusion works); Zabaz
PASS (activated destroy per Oracle). Risen Executioner: recast works and the
`then`-gated branch selection is correct, but the probe exposed BOTH a script
off-by-one (the graveyard count is SELF-INCLUSIVE — diagonal shifted down by
one, now per-line Oracle-correct) AND a narrow engine gap: the `Pay({N})`
surcharge leg inside an `autograveyard ... if ... then Pay({N}) ...` composite
never collects (doesn't arm an mExtraPayment either — `paycost` reports none
pending), so the card currently undercharges its surcharge (base {2}{B}{B}
always exact). Single-card costing nuance, logged for the engine queue.

**DESIGN NOTES (not bugs):** `grant ... grantend` is inherently WHILE-TAPPED —
a `{0}` non-tapping activation evaporates next Update; use transforms/ueot for
persistent zero-cost pumps. NEW LEAD (deferred, high blast radius): Altar of the
Pantheon's `this(variable{...}>0)` conditional life-gain fires when the variable
should be 0, and the mana engine taps it unnecessarily when the floating pool
already covers the cost.

Provenance: 832 lexicon fixtures swept per-process → 300 non-passes triaged
one-by-one (9 agents, probe-verified) → all fixture-side defects FIXED and
re-verified → what remains below is the residue that survived BOTH passes. Every
"CONFIRMED" entry has a clean fixture in `Res/test/lexicon/` that fails ONLY on the
defect (those fixtures are parked in `_known_failures.txt` as regression witnesses —
when a bug is fixed, its fixture returns to `_tests.txt`).

The fix pass FALSIFIED eleven of the original triage's engine-bug verdicts (epic,
scry-drain, the deplete/ingest repeat family, tap(noevent), flip, extort, dredge,
completedungeon, affinity-dead-undercount, nolifegainopponent, player-poison
proliferate) — those turned out to be fixture/synthetic-card defects and are now
green. Details of each falsification live in the fix reports; the authoring guide
(docs/testsuite-fixture-authoring.md) encodes the lessons.

## CARD-SCRIPT FIX WAVE — borderline.txt (agent 2, 2026-07-19)

Worked the validator-corpus failures whose offending line lives in
`Res/sets/primitives/borderline.txt`. My-file failure-row contribution: **119 → 32**
(global validator failures 150 → 49). Every fix Oracle-verified (Scryfall API) and
re-run through `WAGIC_VALIDATE=1` to confirm the line now parses and produces an
ability. 0 U+FFFD introduced (byte-splice edits on ASCII anchors).

**FIXED (27 cards, faithful rewrites):**
- Spurious payload-less `lord(...)` lines DELETED (Oracle has no anthem): Unstoppable
  Plan, Rapacious Guest, Arbor Adherent, Case of the Shattered Pact.
- Stub/duplicate lines DELETED (effect already scripted elsewhere): Dropkick Bomber (2
  empty `@combatdamaged(...):` lines — the sacrifice rides the `{R}` transforms grant),
  Urza's Workshop (`foreach(|myBattlefield)` stub), Galvanic Discharge (raw-prose line;
  `_HARNESSED_LIGHTNING_`/`alterenergy` lines carry it), Ugin's Binding (raw-prose line;
  `autograveyard` recast line carries it).
- Syntax corrections: `cantbeblockedby(power<=2)`→`cantbeblockedby(creature[power<=2])`
  AND further needed a `transforms((,newability[...])) ueot` wrapper for the ueot grant
  (Rhonas's Stalwart, Verdant Outrider); `life-2`→`life:-2` (Fell Specter); added `destroy`
  verb (Zabaz); `counter(-1/-1)` selector→`counter{-1/-1}` (Tenacious Hunter);
  `life:thisforeach(Gate)*2`→`foreach(Gate|mybattlefield) life:2` (Archway Angel);
  missing `:`/`then` in trigger conditionals — `@each my upkeep:if...` (Priest of the
  Wakening Sun), `@movedto(other creature|...):damage:1` (Rampaging Ferocidon, +`other`
  for "another creature"), nested-if `then` (Gideon's Defeat, checks opponentexile since
  the gideon is exiled), Risen Executioner (all 21 graveyard-recast lines: added `then`
  after `~equalto~N`); reordered `destroy all(enchantment)`→`all(enchantment|battlefield)
  destroy` (Cleansing Meditation, `<7` threshold); `@totalcounteradded`→`@counteradded`
  + `turnlimited` before the colon (Generous Pup); dropped duplicated `name()` token
  (Landroval); grant-keyword-ueot needs transforms wrapper — `lifelink`/`cantbetargetof`
  (Psemilla, Veil of Summer); static `cantbetargetof(<typelist>|zones)` → single-selector
  `cantbetargetof(*|opponentbattlefield,opponentgraveyard)` (Shanna); conditional static
  via aslongas absence-threshold `aslongas(creature|opponentBattlefield) indestructible <1`
  (Erebos's Titan); `this(cantargetcard(*[fresh])) hexproof`→`aslongas(this[fresh]|
  mybattlefield) hexproof` (Thrasta); reordered `all(this)` before verb in flip
  (Molten Birth) and in aslongas+transforms (Veiled Crocodile); `choice D(*|myhand)`→
  `choice reject notaTarget(*|myhand)` (Tragic Lesson); stray `auto=*|stack`→`target=*|stack`
  (Spell Syphon); `1/0 foreach(*|hand)`→`foreach(*|myhand) 1/0` (Ral's Staticaster);
  bare `target(opponent)`→`target(opponent) donothing` (Beckoning Will-o'-Wisp);
  `protection from artifact`→`protection from(artifact)` paren form (Rayami).
- **Named-token references fail for multi-word-type / legendary / spaced-name tokens**
  (`token(Smaug)`, `token(Ballistic Boulder)` fail; single-word-type `token(Gold)`,
  `token(Spawnwrithe)` work). Fix: INLINE token defs — Mordor Trebuchet
  `token(Ballistic Boulder,Artifact Creature Construct,2/1,flying)` (its sacrifice/attacking
  rides the existing `and!(transforms(...))`); There and Back Again
  `token(Smaug,Legendary Creature Dragon,6/6,red,flying,haste) and!( transforms((,newability[
  _DIES_name(Create treasures) _TREASURE_*14])) forever )!` (dies→14-Treasures preserved via
  the and!/transforms chain). This named-token limitation is itself a latent engine/validator
  gap worth a loud parse-time diagnostic.

**ENGINE-GAP (left as-is + logged; no faithful card-script form exists):**
- **`thisforeach(aura)` as a lord multiplier** — Heavenly Blademaster (+1/+1 per attached
  aura/equipment). `thisforeach(gear) lord(...)` PARSES (equipment-attach is tracked), but
  `thisforeach(aura) lord(...)` does not — no aura-attached-count keyword. The gear leg
  works; the aura leg is the residual. (2 lines × printings.)
- **`protection from colorless` grant** — Giver of Runes. Color legs (`protection from white`
  …) work; `colorless` isn't a color in that path and the `protection from(colorless)` paren
  form also fails to parse, despite the engine handling `"colorless"` in AAuraIncreaseReduce
  (MTGAbility.cpp:4755). Needs an engine grant path for colorless protection.
- **`myriad` + `iscommander`** — Ironwill Forger (Lieutenant grants myriad ueot). Both keywords
  have ZERO engine implementation (grep: 0 uses); payload also lacks a verb.
- **Discard-as-alternative-cost lord** — Dream Halls (`{D(*|myhand)}: castcard(restricted)
  lord(*[share!color!][-land]|myhand) ...`). `castcard(restricted)` is supported (Miracle) but
  the whole cost-substitution-for-all-color-sharing-spells construct does not parse. (2 lines
  × 2 zones.)

**OUT OF TERRITORY / SKIP (not a borderline.txt data-line fix):**
- `_HEROIC_` macro (Hero of the Games/Nyxborn/Pride/Winds — `@targeted...|mycastingzone`,
  parser-agent) and `_IMPULSE_DRAW_` macro (Meria) live in `_macros.txt`, not borderline.txt.
- Unimplemented keywords logged, not invented: `_IMPULSEDRAW_` (Diversion Specialist),
  `mobilize` (Infantry Shield), `pdrewcount` (Spinehorn Minotaur), `giftcard`/gift (Kitnap).
- `aslongas(type(*|zone)~cmp~N)` comparison form (River Serpent) — parser-agent grammar item.

**NEEDS USER MAGIC-JUDGMENT:**
- Gideon's Defeat: rewrote the "if it was a Gideon" check to `type(gideon|opponentexile)` since
  the target is exiled first — could false-positive if the opponent already had a Gideon in
  exile. Faithful-enough; flag if precise "the exiled card was a Gideon" is wanted.
- Rampaging Ferocidon: split into my/opponent-battlefield ETB lines dealing to `controller`/
  `opponent` respectively to model "that creature's controller"; added `other` to exclude self.

## The validator (shipped 2026-07-18)

`WAGIC_VALIDATE=1` (+ WAGIC_HEADLESS=1) lints the whole collection through the
REAL parse path (AbilityFactory::getAbilities with full game context, per-zone),
emitting `VALIDATE-FAIL` records (`WAGIC_VALIDATE_OUT` for a file) and counted
`VALIDATE-SKIP` categories; exit 1 on any failure. The by-design unpaid-alternative
trace no longer wears an ERROR costume ("INFO ABILITYFACTORY: alternative not
paid (by design)"). First corpus run: 222 raw failures — TRIAGED 2026-07-18 (runtime-confirmed +
isolation-tested per cluster): **71 of 222 are FALSE POSITIVES from a validator
harness bug** — the validate loop reuses ONE AbilityFactory across all cards and
its stored parse-state members (storedAbilityString/PayString/String/AndAbility)
leak between cards, corrupting later parses; production constructs a fresh
factory per card. THE SHOCKLANDS ARE FINE (Blood Crypt runtime-verified: pay-2 →
untapped, decline → tapped) and so is the alternative-transforms cluster. FIX
QUEUED: per-card factory/state reset in the validate loop (~4 lines,
TestSuiteAI.cpp). GENUINE residue: 87 distinct lines / 151 rows — 2 FIXED
(Will of the All-Hunter `__CYLCING__` typo; Turtleshell Changeling
`twist`→`swap`, analogue-proven), a junk-stub tail (Tamiyo Collector "sorry cant
code yet", two raw-prose lines — inert either way), ~50 fix-script candidates
deferred to a proper Oracle-verify pass, and the FIX-PARSER grammar list:
`if paid(kicker) then <global> all(...)` payloads, `@targeted ...
|mycastingzone` pump family (the 4 Hero-of cards), `aslongas(type(*|zone)~cmp~N)`
comparisons, cost-embedded `token(named)`/loyalty forms, and unimplemented
keywords worth adding or rejecting loudly: `mobilize`, `giftcard`,
`_impulsedraw_`, `pdrewcount`. Bring-up also found and fixed a REAL engine
crash: `trigger->castRestriction` written unconditionally when `parseTrigger`
returned NULL (`@each combat restriction{...}` lines) — now guarded.

CALIBRATION NUANCE (adjusts items 2 and 5 below): a cold-parse validator catches
lines that NEVER produce an ability (altermutationcounter ✓ caught). The
`@mutated <upto:mutations>` and real-Battle defense-counter NULLs are
RUNTIME-CONTEXT failures — they parse non-NULL cold and only fail when fired in a
live game — so they are behavior bugs outside the validator's reach, correctly
absent from its output.

## Crashes / parser failures (CONFIRMED)

1. **FIXED 2026-07-18 — `{chosencolor}` OOB crash** (was "cost-catalog
   vector<short> crash"). Root cause: `parseManaCost`'s `{chosencolor}` branch
   called `add(c->chooseacolor, 1)` unguarded, and `chooseacolor` is `-1` until
   an AASetColorChosen resolves → `cost[(size_t)-1]` OOB (assert-abort in debug;
   heap-corrupting 2-byte store in release — CONFIRMED as the multithreaded
   full-registry segfault, same defect). Every other consumer already guarded the
   sentinel; ManaCost.cpp:334 now does too. The original "accumulation across the
   full load" characterization was wrong — deterministic, single-token, fires at
   ability RESOLVE not primitive load. Real cards always wrap `{chosencolor}` in
   `chooseacolor ... chooseend`, so none crashed in play. Witness
   `cost_dynamic_tokens_parse` back in `_tests.txt`, green.
2. **`@mutated` trigger parser NULL** — Archipelagore's
   `@mutated(this):may ... target(<upto:mutations>creature|battlefield) freeze`
   → ABILITYFACTORY NULL; mutate merges fine, the trigger never registers.
   Witnesses: `basic_mutate_triggers_mutation_event`, `effect_mutateover_*`.
3. **Gift mechanic unreachable — RESCOPED to 23 cards, gift branch only
   (2026-07-18 probe).** NOT a parser bug: `alternative`/`ifnot paid(alternative)`
   parse and execute correctly (~220 Kicker/Cleave/Adventure/etc. cards verified
   working), and the "Parser returned NULL" trace is BY-DESIGN output on the
   unpaid branch of every alt-cost card (MTGAbility.cpp:2742-46) — misleading
   ERROR-shaped noise worth silencing/renaming as part of the loud-validation
   work. The real defect: all 23 Gift cards set `other=` equal to `mana=` (gifts
   cost nothing extra), and the engine only sets MANA_PAID_WITH_ALTERNATIVE for a
   DISTINCT payment — so `paid(alternative)` can never become true and every gift
   branch is dead ("gift declined" permanently; each card's default effect works).
   Fix seat: engine — a promise/yes-no additional-cost primitive that sets the
   flag without a mana difference. Secondary (recheck after): `tap(noevent)`
   inside the `and!(...)!` of `_FISHTOKEN_` appeared dropped in the trace.
   Witness: `macro_fish_into_the_flood_maw`.
4. **`altermutationcounter` unusable** — factory case does its side-effects but
   returns no ability ("falls through") → Parser NULL. Witness:
   `effect_altermutationcounter_emits_mutated_event`.
5. **Battle defense-counter line parses NULL on the real card** — Invasion of
   Zendikar's `counter(0/0.6.defense)` ETB → ABILITYFACTORY NULL (synthetic
   `counter(0/0.1.Defense)` parses fine), so the Battle sits at 0 defense and
   `nodefensedamage` has no baseline. Witness: `basic_nodefensedamage_*`.

## Unimplemented mechanics (source-confirmed)

6. **`noentertrg`** — the suppression gate in AllAbilities.h:201 is literally
   commented `//NOT WORKING`; Hushbringer does not stop ETB triggers. Witness:
   `basic_noentertrg_suppresses_etb`.
7. **BANDING** — `MTGCardInstance::banding` only ever assigned NULL;
   `getNextPartner()` never called; band-sharing branches unreachable. Witness:
   `basic_banding_attacks_as_group`.
8. **PHASING** — `GameObserver::phasingPhase()` declared, never defined or called;
   untap never toggles `isPhased`; the `phaseout` effect leaves the card in-zone
   (no observable). Witness: `basic_phasing_leaves_and_returns`.
9. **`leyline`** — Constants::LEYLINE parsed but zero game-logic references; the
   harness also has no opening-hand step. Witness: `basic_leyline_*`.
10. **Day/Night: RESCOPED — machinery WORKS, ~0 real-play impact (2026-07-18
    probe).** Trace-verified: markers establish from bound cards, empty-turn →
    Night and two-spells → Day transitions fire (Day→Night needs the
    establishment turn + one clean empty turn), nonight correctly freezes at
    day, bound permanents transform. The original claims re-adjudicated:
    "trigger never fires" = fixture under-pacing; "castcard(Day) parses NULL" =
    benign re-parse noise on the flipped face (marker already established) —
    silence as part of trace hygiene. The LOAD-BEARING item is a HARNESS GAP:
    zone asserts collapse a DFC's two faces (TestSuiteAI.cpp:680-732 matches by
    id-then-name and both faces resolve to the same card) — no fixture can
    observe a flip. FIX SEATS in order: (a) harness face-aware zone matching —
    unblocks testing for ALL DFC/werewolf/MDFC cards; (b) re-pace the three
    fixtures (they are fixture-bugs; stay parked until (a) lands); (c) low-prio
    engine: standalone `{0}:doubleside(backside)` activated form parses NULL
    (only parsed inside transforms(...) context, MTGAbility.cpp:5476) — the
    automatic cycle never uses it (fold with item 40).
    Witnesses: `basic_daybound_*`, `basic_nightbound_*`, `basic_nonight_*`.
11. **`primitive=` aliasing in a primitives file** — never copies the referenced
    card's data (even aliasing Grizzly Bears yields an empty card); the field is
    only functional for set `_cards.dat`. Witness: `card_field_primitive_alias_*`.

## Wrong results (CONFIRMED, clean witnesses)

12. **`reduceto` no-op** — all four forms (activated, static, controller, the
    real-card `transforms((,newability[reduceto:N]))` idiom) leave life unchanged.
    Witness: `effect_reduceto_sets_life_total`.
13. **`combattoughness` ignored for damage to players** — Doran-style: unblocked
    0/4 deals 0 (expected 4); has(COMBATTOUGHNESS) is set. Witness:
    `basic_combattoughness_assigns_toughness_damage`.
14. **`totemarmor` never consumes the aura** — creature saved, Umbra stays on the
    battlefield (permanent shield). Witness: `basic_totemarmor_*`.
15. **`noloyaltydamage` carrier blocks the cast entirely** — with Spark Rupture in
    play a Shock aimed at a planeswalker never reaches the stack (works without
    it). Witness: `basic_noloyaltydamage_preserves_planeswalker`.
16. **`alterdevoffset` asymmetric drain** — Gray Merchant + Altar: opponent loses
    devotion (correct) but controller gains devotion+1. Witness:
    `effect_alterdevoffset_changes_devotion`.
17. **`donothing` swallows its `and!` follow-up** — control shape with `life:1`
    gains; donothing's follow-up never fires (AAFakeAbility). Witness:
    `effect_donothing_preserves_followup`.
18. **`count`/`countb` values not passed to `draw:counted*amount`** — DrawAction
    resolves with amount 0 despite matches. Witnesses: `effect_count_*`,
    `effect_countb_*`.
19. **`&&`/`and!()` INTERACTIVE-PAYLOAD FIZZLE (family)** — a combined ability
    whose non-token leg is interactive/forced never executes that leg:
    Devastating Summons' forced land sacrifice, `_AMASS_` (whole line dies, no
    army, no rider), `_MONARCH_OPPONENT_`/`_INITIATIVE_OPPONENT_` riders,
    Finality's exiledeath payload. Non-interactive `transforms(...)` macros with
    the same `&& life:1` shape work. Witnesses: `macro_red_elemental_*`,
    `macro_amass_variants`, `macro_conditionals_and_markers`,
    `macro_suspect_and_finality`.
20. **`exploits` over-sacrifices and `@exploited` never fires** — sacrifices the
    target AND a bystander. Witness: `effect_exploits_fires_exploited_trigger`.
21. **`doforetell` doesn't emit `@foretold`** — exiles correctly, event missing;
    also grants no FORETELL keyword, so a plotted/foretold card can never be cast
    from exile (blocks `macro_plotcast` too). Witnesses: `effect_doforetell_*`,
    `macro_plotcast`.
22. **`reveal ... afterrevealed`: RESCOPED TO 0 REAL CARDS (2026-07-18 probe).**
    `afterrevealed` only fires once the reveal zone is EMPTIED, and only the
    `optionone`/`optiontwo` abilities empty it (AllAbilities.cpp
    MTGRevealingCards::Update). All 410 real usage lines carry an option block
    and all four real structural variants probe WORKS (whole-library tutor,
    fixed-N, revealuntil, reveal-by-type). The witness invented an option-less
    form no card uses → bad-synthetic. Residual engine action: reveal WITHOUT a
    drain block should be REJECTED AT PARSE TIME (silent card-stranding today) —
    a loud-validation item, not a behavior fix. Witness to be reauthored on the
    real form. Fixture: `effect_reveal_resolves_followup`.
23. **surveil: RESCOPED — engine correct, HARNESS limitation, ~0 real-play
    impact (2026-07-18 probe).** The reveal→select→moveto(ownergraveyard) chain
    is engine-correct end-to-end (activated-witness proof; AAMover
    OWNER_GRAVEYARD path verified, MTGGameZones.cpp:1587). The fixtures fail
    because TestSuiteGame::getCard searches library BEFORE the reveal zone, so
    for TRIGGERED reveals the selection click is consumed by the library copy
    and never re-queued (the pending-menu re-queue only covers menu-based
    reveals) — the unselected card then takes the put-back default, whose
    library staging via temp (flushed only by shuffleLibrary,
    MTGGameZones.cpp:673-678) produced the "stranded in temp" symptom. In real
    play the player/AI clicks the reveal display directly — all ~104 surveil
    cards function (keep-on-top default and @surveiled trigger verified green).
    RESOLVED 2026-07-18: getCard now searches the reveal zones FIRST while a
    reveal display is open, and — the deeper blocker — the aicode fork
    (GenericRevealAbility/scry substituting a headless heuristic whenever an
    AI controls the card, which the suite seat is) gained an opt-in
    `mForceInteractiveReveal` flag driven by a new `interactivereveal` [DO]
    directive. All three surveil fixtures re-authored and back in _tests.txt,
    green. Compat verified by counterfactual (a blanket carve-out broke two
    aicode-search fixtures; the per-fixture opt-in doesn't).
    ISHUMAN-LENS FOLLOW-UP (AI-track ledger, real-play behavior change,
    needs live validation): AIPlayerGPT also takes the aicode path for reveal
    cards (gate keys on isAI(), not isInteractiveAI()) — the GPT opponent
    could arguably drive the interactive reveal instead of the headless
    heuristic for surveil/scry-class decisions.
    Witnesses: `macro_surveil1/2/3_*` (now active conformance tests).
24. **`manifest dread` battlefield leg missing** — mills one, never puts the other
    face-down onto the battlefield. Witness: `macro_manifest_dread`.
25. **`echo` pay-to-keep fails** — paying the echo cost at the right upkeep still
    sacrifices. Witness: `macro_echo`.
26. **`_RECOVER_` no-op** — the dead creature is neither returned to hand nor
    exiled. Witness: `macro_recover`.
27. **`sethand` / cleanup discard not enforced** — no discard at cleanup over max
    hand size (may be a wider engine gap than sethand). Witness:
    `effect_sethand_reduces_cleanup_hand_limit`.
28. **`cantbetargetof` granted-to-another never applies** — the grant never
    reaches the stack. Related: **a `{0}`-cost targeted `grant` fails to bind its
    target** (the same grant with `{T}` works) — zero-cost targeted-grant
    activation looks broken. Witnesses: `effect_cantbetargetof_*` (+ fix-report
    evidence on the grant witness).
29. **`{myevictcost}` always charges {0}** — imprinted-card state not applied at
    cost-parse time. Witness: `cost_dynamic_eval_mycost_myevictcost`.
30. **`ward`: RESCOPED to ~0 real-play impact — suite-driver limitation
    (2026-07-18 probe).** All 114 real ward cards (count corrected from 87; all
    mana-tax `_WARDn_`, no life/discard forms exist) share one mechanism: a
    mandatory pay-menu arming `mExtraPayment`, which the scripted suite can
    neither complete nor cancel (needs JGE_BTN_SEC) — hence the wedge. Real play
    works: humans click/cancel, and AIPlayerBaka::selectAbility pays-or-fizzles
    (empirically no wedge under an [AI] drive). Actions: (a) harness — add a
    suite command that completes/cancels an mExtraPayment so ward becomes
    testable (same family as item 38); (b) payer-attribution smell — WITHDRAWN
    2026-07-18: the pay ability is re-parented onto the SPELL by transforms(...),
    so source->controller() is the attacker; payment verified charging the
    spell's controller, rules-correct. RESOLVED 2026-07-18: harness gained
    `paycost`/`cancelcost` [DO] commands (drive mExtraPayment via the real
    ManaEngine/decline paths; MenuAbility::declineExtraPayment extracted for
    reuse); ward now has real pay+decline conformance fixtures
    (`macro_ward_pay`, `macro_ward_decline`, both green in _tests.txt;
    `macro_ward_variants` retired). Note: the actual scripted-seat wedge was
    the ward's intermediate target(*|opponentzones) chooser (answered by
    re-clicking the spell), not the pay menu itself. The commands do NOT
    unblock item 38 (offering/suspend) — that blocker is upstream (the
    alt-cost menu never initiates from the heuristic seat).
31. **`crewbonus` not applied to the crewed vehicle** — Gearshift Ace's first
    strike doesn't reach the Copter. Witness: `field_crewbonus_gearshift_ace`.
32. **sideboard/command-zone targeting inert** — clicks register,
    `target(...|sideboard)`/`(...|commandzone)` moves never happen (all 6 pairs).
    Witness: `target_sideboard_commandzone_matrix`.
33. **CLOSED — NOT A BUG (2026-07-18 probe, could-not-reproduce).** Forced
    discard DOES fire `@discarded` on both engine paths (random/discardRandom
    and targeted reject/AADiscardCard), for controller-side AND opponent-side
    watchers, probe-verified on real cards (Megrim, Glint-Horn Buccaneer) —
    emit/listen wiring confirmed (WEventCardDiscard emitted with the card still
    in hand; TrCardDiscarded accepts any non-cycle discard). Madness rides a
    separate, working flag-based replacement path. The original diagnosis was
    likely poisoned by the repeated-same-zone-INIT-line OVERWRITE trap (the
    watcher silently never entered play) — now in the authoring doc. Follow-up:
    add a watcher-path fixture to lock coverage (current fixture only exercises
    the `this`-self cycling form).
34. **`oppnomaxhand` protection collapses when the beneficiary controls no
    permanents** — GameObserver.cpp:1151 clears `nomaxhandsize` by scanning the
    player's own battlefield, ignoring grants from the opponent's permanents.
    Worked around in the fixture (beneficiary given a land); latent gap.
35. **Anafenza, the Foremost: `oppgcreatureexiler` inert on the real card only** —
    synthetic carrier and a byte-identical clone both work; card-data/loading
    discrepancy. (Keyword fixture now uses the synthetic; Anafenza herself is the
    residual bug.)

## Card-DATA bugs (fix in tracked Res, no C++)

36. **RESOLVED AS NOT-BUGS (2026-07-18 step-3 pass).** `_SCAVENGE_`/`_ETERNALIZE_`
    do NOT omit the exile cost — every real card carries it as the `{E}` token in
    its own activation line (`autograveyard={4}{G}{E}:_SCAVENGE_(...)`); the test
    witnesses had omitted `{E}`. Witnesses fixed, both fixtures back in `_tests.txt`
    and green. `_METALCRAFT_`'s missing trailing space after `then` is real but has
    ZERO concatenated users; ruling: macros are invoked with a separating space
    (as all real invocations do) — encoded in the authoring doc rather than as a
    fragile trailing-whitespace edit that any editor would silently strip.

## Engine robustness / harness-boundary items

37. **Unpayable alternative-attack-cost prompt wedges the phase machine** —
    clicking Floodtide Serpent (alt cost `{H(enchantment)}`, nothing to return)
    during attackers leaves the phase stuck; the prompt isn't cancelable by
    default handling.
38. **Menu/chooser-based alternative-cost casts are undrivable by the scripted
    suite seat** (subtype offering, suspend) — the scripted seat is a heuristic AI
    (`ishuman`=0, and several such cards are gated `compare(ishuman)`); mana-cost
    alternatives (emerge) drive fine. Needs an interactive test seat or harness
    support; their fixtures sit in `_known_failures.txt`.
39. **`enlist`'s granted `{T(creature)}` ability is undrivable** — a second click
    on the attacker un-declares it; effectively untestable (and possibly
    unplayable) under the click model. Witness: `macro_enlist_argivian_cavalier`.
40. **`doubleside`** — battlefield-forbidden by design AND unreachable from hand
    via the harness; no test path exists.
41. **Watch item (unconfirmed)**: a `ueot`-wrapped TRIGGERED ability may not tear
    down at cleanup (fired on two consecutive end steps in a strengthened probe).

## Doc-note surprises (engine is "right", the name misleads)

- `explores` only emits the `@explored` event — it never reveals/moves cards.
- `flip(X/Y)` is not a P/T setter — `flip(<CardName>)` transforms into that card
  (`flip(4/4)` found a card literally named "Black Vise" via lookup fallback).
- deplete/ingest color/name "repeat" compares WITHIN a batch of N (N=1 can never
  repeat), and the ingest flags are separate words (`coloringest ingest:N`).
- `mygraveexiler` exiles ALL the controller's dying cards, not just creatures.
- lord-family keywords (`foreach`, `this()`, lords) must be CONTINUOUS — the
  activated `{0}:` form breaks attack declaration for the carrier.
- `completedungeon` sideboards the completing card and emits `@dungeoncompleted`
  with the card in the sideboard — triggers must scope `*|mysideboard`.

## Modal-DFC back-face cast resolves the FRONT face (wave-24, R-DFC-BACKFACE-RESOLVES-FRONT)

Repro (live): deck102 vs27 s34. Tergrid, God of Fright // Tergrid's Lantern is a
Kaldheim modal DFC modelled as the front creature carrying the back as an
alternative cost:
  other={3}{B} name(Tergrid's Lantern)
  autostack=if paid(alternative) then flip(Tergrid's Lantern) forcetype(Legendary Artifact)
With exactly {3}{B} on the pool (God front {3}{B}{B} unaffordable) the pilot
flipped the display to the Lantern side, cast, and the {3}{B} deployed the 4/5
God — a mechanical discount, beneficial that game but rules-wrong.

DIAGNOSIS (deterministic, this worktree):
- The INTENDED path WORKS. Flip the display (the anyzone {0} `doubleside`
  toggle), then cast via the ALTERNATIVE-COST menu option ("Tergrid's Lantern"):
  paid(alternative) is set (MTGRules.cpp ~L1144), the autostack AAFlip fires with
  forcetype='legendary artifact', and the Lantern resolves. PROVEN by the
  registered witness `probe/tergrid_lantern_altcost_wave24.txt` (green).
- The BUG is a DIFFERENT menu option. `AATurnSide::resolve` (AllAbilities.cpp
  ~L5500) updates the flipped card's MTGId to the back face ONLY when
  `playMode != MODE_TEST_SUITE`. So in a LIVE game the post-flip card IS the
  Lantern (artifact, {3}{B}, castable "normally"), and the click menu offers a
  plain **"Cast Card Normally"** (MTGPutInPlayRule) NEXT TO the correct
  alternative-cost option. "Cast Card Normally" pays {3}{B} but never sets
  paid(alternative), so the `autostack ... if paid(alternative)` flip is skipped
  and the base front (God) face resolves. The pilot picked that option.
- HARNESS LIMITATION: because the suite path keeps the God's MTGId (the
  MODE_TEST_SUITE branch), the "Cast Card Normally" option never appears in a
  scripted fixture and the bug itself cannot be witnessed here — only the
  correct path can.

FIX SEAT (PARKED — not shipped; bug is currently benefit-side and a wrong change
risks the just-achieved Tergrid cast+steal milestone): the "Cast Card Normally"
path for a flipped modal-DFC back face must either be suppressed (leaving only
the alternative-cost cast) or routed through the same forcetype flip so it can't
deploy the front face. Both live in engine flip/cast machinery gated by the
MODE_TEST_SUITE divergence above; they need live (non-suite) validation the
harness can't provide. Reassess when the modal-DFC surface is next touched.
