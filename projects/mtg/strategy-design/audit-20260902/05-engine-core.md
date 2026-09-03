# Audit 2026-09-02 - slice 5: engine core

Master 859186536. Files read IN FULL: GameObserver.cpp/.h, MTGRules.cpp/.h, ActionStack.cpp/.h,
ActionLayer.cpp/.h, ActionElement.cpp/.h, MTGGameZones.cpp/.h, MTGCardInstance.cpp/.h, MTGCard.cpp/.h,
Player.cpp/.h, Damage.cpp/.h, DamagerDamaged.cpp/.h, ManaCost.cpp/.h, ManaCostHybrid.cpp/.h,
ManaEngine.cpp/.h, LegalActions.cpp/.h, DecisionContract.cpp/.h, Rules.cpp/.h, WParsedInt.cpp/.h,
MTGGamePhase.cpp/.h, PhaseRing.cpp/.h, Trash.cpp/.h, Token.cpp/.h, Subtypes.cpp/.h, Closest.cpp
(a header-only template despite the name), plus utils.h (RandomGenerator) and the two consumers
needed to size findings (GuiHand.cpp:225-300, MTGAbility::testDestroy). NOT read: CardPrimitive,
Counters, TargetChooser, MTGAbility/AllAbilities bodies, DuelLayers, GuiLayers - other slices.

## Measurements (worktree `audit-engine-core`, master sources rebuilt, then removed)

1. `perf record -F 997 -g` of the headless suite (`WAGIC_TESTSUITE=1`, 1215 tests / 2 known
   failures / 46 AI, identical to baseline; 72,152 samples, -O0 debug build). Inclusive shares of
   total suite CPU (`perf report --children --no-inline`):
   - `GameObserver::Update` 63.3% -> `DuelLayers::Update` 56.3% -> `ActionLayer::Update` 31.2%
   - `GuiHandSelf::Update` **15.1%** = `LegalActionsOracle::usableAbilityCards` 9.1% +
     `castableForDisplay` 5.7% (-> `legalCasts` 4.2%, `payable` 2.3%, `ManaEngine::planPayment` 7.4%
     across callers). This is the human castability/usable-ability DISPLAY refresh, running in a
     headless AI-vs-AI suite.
   - `ListMaintainerAbility::updateTargets` 15.6% (lords, other slice), `GenericTriggeredAbility::Update`
     6.5% (other slice), `MTGAbility::testDestroy` 7.75% (driven per ability per tick by
     `ActionLayer::Update`, of which `ActionStack::has` 1.4%).
   - `MTGAllCards::findType`/`Subtypes::find` 8.7% (string-keyed type lookups; the top three
     SELF symbols in the whole profile are `std::string::max_size/size/_M_data` at 4.2/3.7/2.5%,
     i.e. `map<string,int>::find` inside `Subtypes::find`).
   - `ManaCost::init` 5.0%, `ManaCost::ManaCost(ManaCost*)` 3.8%, `ManaCost(const ManaCost&)` 3.2%
     (the latter entirely via `DescriptorTargetChooser::clone` -> `MTGCardInstance` copy-ctor),
     `~ManaCost` 3.1% - ManaCost churn is ~10% of suite CPU.
   - `__dynamic_cast` 12.1% inclusive (the per-tick `receiveEvent`/`manaObjects` walks).
   - `GameObserver::gameStateBasedEffects` 6.2% inclusive (0.40% self); `GameObserver::Affinity`
     0.64%; `GameObserver::receiveEvent` 7.5%; `ActionStack::Update` 5.5%; `WParsedInt::init` 2.2%;
     `TargetChooserFactory::createTargetChooser` 5.1%.
2. ASAN + LeakSanitizer on the suite (master `bin-asan/wagic`, built 2026-09-01 21:48 = a few
   commits behind HEAD; every quoted line is re-checked against master source). Two full runs
   (stack depth 12 and 5): **zero AddressSanitizer errors** (no UAF/overflow in 1215 games) and
   `SUMMARY: 2192129 byte(s) leaked in 49373 allocation(s)` - ~1.8 KB per game. Direct-leak roots
   are all OUTSIDE this slice's owners (`AutoLineMacro::AddMacro` 14 KB one-time startup,
   `ManaCost::parseManaCost` via `SnowCost::isPaymentSet/doPay` (ExtraCost.cpp), `AACastCard::
   resolveSpell`, `AbilityFactory::parseMagicLine:2762`). The per-game teardown of this slice
   (`~GameObserver` -> players -> zones -> `previous` chains, `ActionLayer` garbage, `Trash`) is
   clean in the suite. The leak findings below are therefore MODE-specific (commander decks,
   attack/block-cost cards, PayZero, anytypeofmana, legend-rule menus) that the suite does not
   exercise, and are argued from source; the SnowCost leak in the LSan report is the same
   `parseManaCost`-returns-NEW idiom as H3.
3. `gdb print sizeof` on the master binary: `MTGCardInstance` 2640 B, `CardPrimitive` 640,
   `ManaCost` 176, `Counters` 40, `Player` 496, `MTGGameZone` 160, `MTGAbility` 432,
   `GameObserver` 640, `TargetChooser` 128, `WEvent` 16.

Counts: 16 findings - HIGH 5, MED 6, LOW 5. Category: LEAK 4, MEM-SAFE 5, PERF 4, MEM-EFF 2, CRUD 1.

---

## HIGH

### H1. `garbageLastTurn` aliases `garbage`: dead tokens are freed ONE turn after they die, not two - the root of the "dead token dangle" class
- **Where**: `src/MTGGameZones.cpp:189-193` (`MTGPlayerCards::beforeBeginPhase`), `:151-172` (destructor), `:394` (init)
- **Category**: MEM-SAFE. **VERIFIED** (reading; the invariant is one line).
- **Mechanism**: the member pair is named for a two-turn deferral (`garbage` = this turn's dead tokens, `garbageLastTurn` = last turn's, freed one turn later). The code does
  ```
  SAFE_DELETE(garbageLastTurn);
  garbageLastTurn = garbage = NEW MTGGameZone();
  ```
  so both pointers always name the SAME zone; the `SAFE_DELETE` frees the zone `garbage` pointed to a moment ago, i.e. every token that died during the turn that just ended, together with its `previous` chain (`~MTGGameZone` deletes `cards[i]->previous` and `cards[i]`). The destructor consistently does NOT delete `garbageLastTurn` (it would double-free) - the aliasing is an accident, not a design.
- **Why it matters**: the engine's own comments name this class repeatedly - `GameObserver::validateCardPointer` ("Really-deleted instances (dead tokens) come back NULL - the raw-pointer dangle class (currentActionCard core 3266478, ATransformer target core 3151670)"), DecisionContract's `validatedCardPointer` ("a token that dies while its menu is armed is deleted for real"). Every raw pointer that outlives a token across one turn boundary (`card->target` of an aura, `imprintedCards`, AI seats' remembered pointers, `storedCard`, `myPair`, LKI reads) dangles at the very next `BEFORE_BEGIN`. `validateCardPointer` scans `garbage` and `garbageLastTurn` as two zones but they are one.
- **Fix**: `SAFE_DELETE(garbageLastTurn); garbageLastTurn = garbage; garbage = NEW MTGGameZone(); garbage->setOwner(owner);` and in `~MTGPlayerCards` `if (garbageLastTurn != garbage) SAFE_DELETE(garbageLastTurn);` (init keeps them aliased until the first turn - the `!=` guard covers it). `setOwner` already handles both.
- **Blast radius**: MTGTokensCleanup (the only producer of garbage-zone cards), validateCardPointer, AIPlayerGPT.cpp:11261 (reads `garbageLastTurn`). Memory: one extra turn of dead tokens held (a few instances).
- **Verify**: suite (1215/2 baseline); a fixture that kills a token, advances two turns and has an ability read a stored pointer to it; ASAN on the selfplay corpus - the dead-token UAF signatures should disappear.

### H2. Commander deck loading leaks one `MTGCardInstance` per deck card per commander
- **Where**: `src/MTGGameZones.cpp:84-107` (`MTGPlayerCards::initDeck`, the `else` branch under `deck->CommandZone.size()`)
- **Category**: LEAK (per game, Vita heap). **VERIFIED** by reading; not reachable by the suite (4 of 845 fixtures mention the command zone, none load a commander deck) so LSan is silent on it.
- **Mechanism**: inside the per-card loop, for every non-basic-land card and every commander, `MTGCardInstance * commander = NEW MTGCardInstance(cmdcard, this);` is built solely to call `hasColor()`/`magicText.find()` on it and is never deleted. `sizeof(MTGCardInstance)` = 2640 B plus its `Counters`, three by-value `ManaCost`s' vectors and the `initMTGCI()` work. A 99-card single-commander deck leaks ~99 x ~2.8 KB = ~280 KB per player per game (~560 KB per commander game on a 40 MB Vita heap); 1,200 commander games in one process ~670 MB.
- **Fix**: read colour identity off `cmdcard->data` (`CardPrimitive::hasColor`, `magicText`) - no instance is needed at all; or hoist one instance per `CommandZone` entry out of the loop and delete it after.
- **Blast radius**: commander/random-commander modes only.
- **Verify**: ASAN binary on a commander game (`WAGIC_REPLAY` of a commander transcript or a commander selfplay pairing): the `initDeck` leak stack must be gone.

### H3. `NEW ManaCost(ManaCost::parseManaCost("{0}", ...))` leaks the parsed temporary at every call - 13 live sites, several on per-click and per-plan paths
- **Where** (master): `src/ManaCost.cpp:1110` (`canAfford`, anytypeofmana path - ALSO leaks the first `diff` it overwrites on the next line), `src/ManaEngine.cpp:679` (`planPayment`, anytypeofmana), `src/MTGRules.cpp:367` (sunburst, `isReactingToClick`), `:1988`, `:2010` (MTGPayZeroRule is/reactToClick), `:2185`, `:2207` (attack cost is/reactToClick), `:2265`, `:2287` (block cost is/reactToClick), `src/AIPlayerBaka.cpp:2671,2678,2721,2729` (other slice, same idiom). `/usr/bin/grep -rn 'NEW ManaCost(ManaCost::parseManaCost' src` = 14 hits (one in commented-out code, GameObserver.cpp:1729).
- **Category**: LEAK. **VERIFIED** by reading (`parseManaCost(s, NULL, c)` returns a `NEW ManaCost()` when `_manacost` is NULL - `ManaCost.cpp:185-196`; the outer `NEW ManaCost(ptr)` copies it and the inner pointer is never freed). The suite has no attack/block-cost, PayZero or anytypeofmana cards, so LSan does not reach these sites; it DOES report the same idiom's leak from `SnowCost` (ExtraCost.cpp:162/204, 24 objects) which corroborates the mechanism.
- **Mechanism**: the idiom is "build an empty cost" spelled as parse-then-copy. Each execution leaks a `ManaCost` (176 B + its 7-entry vector + the eight empty sub-costs the pointer copy-ctor allocates, see H4: ~1.6 KB). `MTGAttackCostRule::isReactingToClick` / `MTGBlockCostRule::isReactingToClick` run on every `isReactingToClick` walk during combat for cards with attack/block costs (i.e. every click while such a card is on the board); `PayZeroRule::isReactingToClick` runs for every `PAYZERO` card on every click; `planPayment` with anytypeofmana runs per castability probe (4 Hz on the Vita, H5).
- **Fix**: replace every `NEW ManaCost(ManaCost::parseManaCost("{0}", NULL, x))` with `NEW ManaCost()` (identical: `{0}` adds nothing, `xColor` is -1 either way). At `MTGRules.cpp:367` `changeCostTo(NEW ManaCost(...))` copies its argument and does not take ownership, so that site leaks TWO objects: use a stack `ManaCost empty; card->getManaCost()->changeCostTo(&empty);`. In `canAfford` free the first `diff` before reassigning.
- **Blast radius**: none behavioural. PARSETEST + suite unchanged.
- **Verify**: LSan on a fixture with an attack-cost creature (e.g. `attackcost` primitives) and a PayZero card: the `parseManaCost` frames vanish.

### H4. `ManaCost` pointer/copy constructors allocate eight empty sub-cost objects unconditionally, and turn `hasAnotherCost()`/`getAlternative()` etc. TRUE on every copy
- **Where**: `src/ManaCost.cpp:623-652` (`ManaCost::ManaCost(ManaCost*)`), `:656-685` (`ManaCost(const ManaCost&)`)
- **Category**: MEM-EFF/PERF + latent correctness. **VERIFIED** (reading; `ManaCost(ManaCost* NULL)` returns after `init()`, so `kicker = NEW ManaCost(manaCost->kicker)` yields a non-NULL empty object even when the source has no kicker). perf: `ManaCost::init` 5.0% + pointer ctor 3.8% + copy ctor 3.2% + dtors 3.1% of suite CPU; LSan's indirect-leak records for H3's cousins show the nested chain `ManaCost(ManaCost*)` -> `ManaCost(ManaCost*)` x8 -> `init` (2.0-3.0 KB per leaked cost).
- **Mechanism**: every pointer-copy of a cost performs 9 heap allocations (self + kicker, Retrace, BuyBack, alternative, FlashBack, morph, suspend, Bestow) plus 9 `cost` vectors, ~1.6 KB, where `copy()` (`:834`) allocates sub-costs only when present. Hot users: `ManaEngine::refineForOptions` - `payableFrom` builds `ManaCost total(leftover)` per assignment combination (<=64) per hand option per configuration (<=512) on every human auto-tap plan; `selectAutoTapProducers` (`NEW ManaCost(pool)`), `leftoverAfter`, `Eval::run`; `MTGPutInPlayRule::reactToClick` (`previousManaPool`, `costToTap`, `withKickerCost`, `withBestowCost`); `Spell::resolve` (`setManaUsedToCast(NEW ManaCost(this->cost))`, one per resolved spell, kept alive on the card); the `MTGCardInstance` copy-ctor (three by-value `ManaCost` members -> `DescriptorTargetChooser::clone`, 3.2% of the suite).
  Correctness half: after a pointer copy `getAlternative()/getKicker()/getMorph()/getFlashback()/getRetrace()/getBestow()/getSuspend()` all return non-NULL empties and `hasAnotherCost()` (`:770-784`) returns 1. A reader that branches on "has an alternative" against a pointer-copied cost is misled (`LegalActionsOracle::payable`'s `cost->getMorph() && pMana->canAfford(cost->getMorph(),0)` is TRUE for an empty morph, `{0}` is always affordable). Today those readers get the card's own cost (built by `copy()`), so this is latent.
- **Fix**: `kicker = manaCost->kicker ? NEW ManaCost(manaCost->kicker) : NULL;` (same for the other seven, both ctors). `copy()` already produces NULLs for absent sub-costs and every reader NULL-checks, so nothing depends on the empties.
- **Blast radius**: everything that copies a ManaCost. Grep `hasAnotherCost` (AIPlayerBaka) for readers of copied costs; PARSETEST + suite.
- **Verify**: suite/PARSETEST unchanged; `perf` share of `ManaCost::init` drops by roughly the sub-cost share (~half).

### H5. The human castability/usable-ability display recomputes the full oracle on a TIMER (4 Hz), not on change - 15% of suite CPU, a ~30 ms spike every 250 ms on the Vita
- **Where**: `src/GuiHand.cpp:236-275` (`GuiHandSelf::Update`, consumer), `src/LegalActions.cpp:623-673` (`castableForDisplay`), `:869-885` (`usableAbilityCards`), `:477-601` (`hasUsableActivatedAbility`, which runs `ManaEngine::potentialMana` + `planPayment` per ability with a mana cost)
- **Category**: PERF (Vita hot path, human seat). **VERIFIED**: perf `GuiHandSelf::Update` 15.09% inclusive of the headless suite (`usableAbilityCards` 9.13%, `castableForDisplay` 5.67%); the suite's fake `dt` of 1.0 makes the 0.25 s throttle fire every tick, so the suite number is the un-throttled per-tick cost. The W53-S comment in LegalActions measured the pre-batching refresh at 1.57 ms desktop = ~30 ms Vita; the batching cut the constant, not the cadence.
- **Mechanism**: every 0.25 s the refresh re-runs `castableForDisplay` (potentialMana + `legalCasts`: per hand card `payable` -> `canAfford` x5 + `planPayment`, a `TargetChooser` build + destroy, `parseCastRestrictions`) and `usableAbilityCards` (permissive potential + per activated ability: strict `potentialMana` + `canAfford` + `planPayment`), regardless of whether anything changed. On a static board (the common case while the player is reading the screen) the answers are identical every time. The refresh also runs for AI seats' `GuiHandSelf` objects (the suite has no human seat and still spends 15% here) - the DuelLayers own a `GuiHandSelf` for the rendered player even when that player is an AI.
- **Fix**: change-driven recompute. The signature trick already used a few lines down for `refineForOptions` (`mPreviewSig`: pool string + hand pointers + battlefield pointer/tap state) is exactly the right key; add phase, stack `count(NOT_RESOLVED)`, `currentPlayer`, and `mLayers->actionLayer()->mObjects.size()` to it and skip both oracle calls when it is unchanged. Also skip entirely when `hand->owner->isAI()` and the seat is not interactive (no human is looking). A cheaper second step: in `hasUsableActivatedAbility` compute the strict `potentialMana` once per (player, tapsSource) instead of per ability.
- **Blast radius**: display flags only (`castableNow`, `hasUsableAbilityNow`, `canAttackNow`, `canBlockNow`, tap preview). Suite `w53_usable_ability_batch_board.txt` pins the semantics.
- **Verify**: suite unchanged; `perf` share of `GuiHandSelf::Update` falls to the event rate; on the Vita, the "almost frozen when selecting cards in hand" report (vpk11) is the regression test.

## MED

### M1. Zone-move `previous` chains are only reaped for battlefield and graveyard cards; hand/library/exile cards keep every prior instance alive until game end
- **Where**: `src/MTGCardInstance.cpp:850-863` (`cleanup()` frees `previous` when not in use), `src/Player.cpp:432-436` (`Player::cleanupPhase` -> inPlay + graveyard only), `src/MTGGameZones.cpp:960-1025` (`removeCard` clones on every move)
- **Category**: MEM-EFF (Vita heap over a long game). **SUSPECTED** (reading; growth is proportional to zone moves per game).
- **Mechanism**: every zone change allocates a fresh `MTGCardInstance` (2640 B + Counters + 3 ManaCost vectors + strings, ~3 KB) and hangs the old one off `copy->previous`. `cleanup()` deletes the chain at end of turn, but only for cards in `inPlay` and `graveyard`. A card that goes hand -> stack -> battlefield -> hand (bounce), library -> hand -> library (Brainstorm, shuffle effects), or anything that ends in exile/hand/library keeps its whole chain until `~MTGGameZone` at game end (`~MTGCardInstance` recurses on `previous`, so the game-end free IS complete - this is peak heap, not a leak; LSan agrees).
- **Fix**: call `cleanupPhase()` on hand, library, exile, commandzone and reveal in `Player::cleanupPhase`; `cleanup()`'s `stillInUse()` guard already protects instances the action layer references.
- **Blast radius**: readers of `->previous->previous` for non-battlefield cards (`WParsedInt "sunburst"`; `MTGPersistRule`/`MTGUnearthRule` read `e->card->previous` inside the zone-change event, before any cleanup). End-of-turn reaping keeps those safe.
- **Verify**: `TRACK_OBJECT_USAGE` `MTGCardInstance` count at end of each turn over a 30-turn selfplay, before/after; suite unchanged.

### M2. `MTGPlayerCards::operator<<` serialises the HAND under `exile=`
- **Where**: `src/MTGGameZones.cpp:1917-1921`
- **Category**: correctness of the save/undo/replay path (MEM-SAFE bucket). **VERIFIED** (reading: `if(z.removedFromGame->cards.size()) { out << "exile="; out << *(z.hand) << endl; }`).
- **Mechanism**: the serialised game (`operator<<(GameObserver)` -> `resetStartupGame()`; used by undo, the post-pre-game transcript baseline, network `synchronize`, `dumpAssert`) writes the hand's ids as the exile zone whenever exile is non-empty; `load()` rebuilds exile from that line, so the loaded state has the hand duplicated into exile and the real exiled cards gone. The post-pre-game re-baseline is exactly the moment a Serum Powder redraw has just filled exile.
- **Fix**: `out << *(z.removedFromGame)`.
- **Verify**: an undo fixture with `exile=` non-empty in [INIT]; a WAGIC_REPLAY of a Serum Powder game.

### M3. `WParsedInt` re-parses its expression from scratch on every evaluation, with a `TargetChooser` build per `type:` evaluation; its keyword-erase calls are wrong
- **Where**: `src/WParsedInt.cpp:48-386` (`init`), `:298-362` (`type:` branch), `:390-473` (`convertedcost:/power:` branch), `:825-836` (`countDevotionTo`), `:105-176` (erase calls)
- **Category**: PERF. **VERIFIED** share: `WParsedInt::init` 2.2% of suite CPU (part of `createTargetChooser`'s 5.1%); the cost per evaluation is from reading.
- **Mechanism**: `WParsedInt` is constructed from the raw script substring at every evaluation (lords, `variable{}` restrictions, `computeNewCost`, `AALifer`, counters). `init()` runs ~15 `find()`s for rounding keywords, then a ~120-way `else if` chain of string compares; the `type:` family allocates a `TargetChooser` through the factory (its own parser) and destroys it each time. The erase calls are `s.erase(pos, pos + len)` - `erase(pos, count)` with count = pos+len - correct only for a keyword at index 0 and harmless only because the keywords are always the trailing token.
- **Fix**: (a) `s.erase(pos, len)` at all 12 sites; (b) cache the parse: the expression is immutable per ability instance, so the branch (an enum) and the `type:` chooser can be built once and re-evaluated - a small `WParsedIntExpr {kind, tc, constant}` with `eval(card, spell)`; convert the per-tick creators first (`variable{}` in lord/restriction conditions).
- **Verify**: PARSETEST 2249 + suite; `perf` share of `WParsedInt::init`.

### M4. Game RNG: one stream per game feeds shuffles, dice AND the heuristic AI's decision randomness; four engine sites still draw from libc `rand()` (one reseeds it)
- **Where**: `getRandomGenerator()->random()` consumers: AIPlayerBaka.cpp 19 sites, Rules.cpp 16, AllAbilities.cpp 6, MTGRules.cpp 4 (Momir/StoneHewer/Hermit), AIPlayer.cpp 3, MTGGameZones.cpp 2 (`shuffle`, `discardRandom`), WParsedInt.cpp 1 (`genrand`), AIMomirPlayer.cpp 1. libc `rand()` in engine paths: `src/WParsedInt.cpp:1144` (`cursedscrollresult`), `src/TargetChooser.cpp:1249`, `src/Rules.cpp:719-720` (`std::srand(time(NULL))` then `std::rand()` - RESEEDS the process-global stream), `src/GameStateDuel.cpp:3440` (coin flip), `src/utils.cpp:92` (`WRand()`).
- **Category**: MEM-SAFE (determinism/replay drift). **VERIFIED** by grep; the design intent is written in `utils.h` ("to be able to control the AI randoms independently of the other game randoms so that we can actually test AI") and is not implemented - there is exactly one `RandomGenerator` per `GameObserver`.
- **Mechanism**: a transcript records `rvalues:`; replay consumes the stream in the same order only if every consumer runs identically. A seat change (Baka draws for attack choice, ability use and target picks; GPT draws none of them), an AI-side change between builds, or any `std::rand()` site (unrecordable, and shared across the threaded suite's games) shifts every subsequent shuffle - the "replay diverges at action N" mechanism whenever the AI branch differs.
- **Fix**: a second `RandomGenerator` on `GameObserver` (`aiRandomGenerator`, seeded from the game seed) for AIPlayer*/AIHints draws; shuffles/dice/`genrand`/Momir stay on the game stream. Route the four engine `std::rand()` sites through the game generator (`Rules.cpp:719` must not `srand`). `WRand()` has no engine caller - delete.
- **Blast radius**: pre-change transcripts will not replay after it (draw sequence changes once); seeded fixtures that depend on AI draws need re-baselining.
- **Verify**: replay a Baka-vs-Baka transcript with one seat swapped to GPT - shuffles must stay identical; `rvalues` length becomes AI-independent.

### M5. `MTGNewLegend::MoveLegend` / `MTGNewPlaneswalker::MovePW` build a full `MTGCardInstance` clone as the menu's source and never free it
- **Where**: `src/MTGRules.cpp:4090` and `:4207`
- **Category**: LEAK (per legend-rule event). **SUSPECTED** (reading: `myClone` is handed to `AAMover`, `GenericTargetAbility`, `GenericAddToGame` and `MenuAbility` as `source`; none owns its source; nothing deletes it. The suite's legend fixtures did not surface it in the LSan run, so either a path I did not read frees it or the fixtures do not reach `MoveLegend`).
- **Mechanism**: each time a second same-name legend enters, ~3 KB is allocated and orphaned once the menu resolves - several times per game in a legend-heavy deck.
- **Fix**: keep the clones on the rule and delete them in its destructor, or pass `card` itself as source (the clone exists only so the `share!name!` chooser has a stable name to compare).
- **Verify**: LSan on a fixture that triggers the legend rule twice (`legend_rule*.txt`).

### M6. `ActionLayer::Update` calls `testDestroy()` on every ability every tick; each call scans the whole stack
- **Where**: `src/ActionLayer.cpp:149-165` (the destroy sweep), `src/MTGAbility.cpp` `MTGAbility::testDestroy` (`stackLayer()->has(this)` = linear `mObjects` scan; four `isIn*` zone tests)
- **Category**: PERF (per tick). **VERIFIED** share: `MTGAbility::testDestroy` 7.75% inclusive of the suite (`ActionStack::has` 1.44%). This is O(abilities x stack objects) per tick, and the stack keeps RESOLVED entries until `garbageCollect` at the next BEFORE_BEGIN, so the scanned list grows over the turn.
- **Fix**: `ActionStack::has(MTGAbility*)` over a `std::set<MTGAbility*>` of NOT_RESOLVED `StackAbility::ability` pointers maintained by `addAbility`/`resolve`/`Fizzle`/`garbageCollect` (4 sites); and short-circuit `testDestroy` on `source->currentZone` (`isInPlay` is already O(1) via `hasCard`, but four of them run before the stack scan). The sweep itself could run only when `stuffHappened` or a zone-change/stack event was seen since the last sweep.
- **Verify**: suite; `perf` share of `testDestroy`.

## LOW

### L1. `AffinityNeedsUpdate` memo is defeated: every tick ends with `WEventGameStateBasedChecked`, which re-arms it
- **Where**: `src/GameObserver.cpp:2611-2612` and `:2660-2663` (`receiveEvent` sets the flag for ANY event), `:1741-1749` (`Affinity` walks 5 zones x 2 players).
- **Category**: PERF. **VERIFIED** mechanism; measured cost only 0.64% of suite CPU, hence LOW. The flag can never be false at the top of a tick; each card pays 20 `has()` tests and two `getConvertedCost()` walks (with `dynamic_cast` over extra costs) per tick.
- **Fix**: set `AffinityNeedsUpdate` only from cost-relevant events (zone change, controller change, ability add/remove) - not from `WEventGameStateBasedChecked`/phase/combat-step events; and in `Affinity()` test the 20 keyword bits before the two `getConvertedCost()` calls.

### L2. `ActionStack::garbageCollect` erases from `manaObjects` with an iterator into `mObjects`
- **Where**: `src/ActionStack.cpp:1181-1193`
- **Category**: CRUD / latent MEM-SAFE. **VERIFIED** (reading). `dynamic_cast<AManaProducer*>(current)` on an `Interruptible*` can never succeed (no class derives from both), so the branch is dead; if it ever fired, `manaObjects.erase(iter)` with a foreign iterator is UB. Delete the four lines.

### L3. `ManaCost::operator=` is a SHALLOW copy of nine owned pointers (and skips the X slot)
- **Where**: `src/ManaCost.cpp:688-709`
- **Category**: MEM-SAFE (latent double-free). **VERIFIED** (reading); no by-value assignment found by grep in the files read. Implement over `copy()` or `= delete` it so a future assignment fails to compile instead of double-freeing.

### L4. `MTGCardInstance` never frees the `protections`/`canttarget`/`cantBeBlockedBys`/`cantBeBlockerOfs` choosers it holds; `removeCantBeBlockerOfCard(card, erase=1)` would `delete` an `MTGCardInstance` it does not own
- **Where**: `src/MTGCardInstance.cpp:378-388` (dtor), `:2181-2193`
- **Category**: LEAK (small) / CRUD. **SUSPECTED**: ownership belongs to the granting abilities (other slice) which call `removeProtection(tc, erase=1)`; a grant still live at abrupt teardown leaks its 128 B chooser. The `erase` branch of `removeCantBeBlockerOfCard` has no caller passing 1 - remove it.

### L5. One-line items
- `include/GameObserver.h:281` `getResourceManager() { if(this) ... }` - UB; GCC may fold `if(this)` to true. NULL-observer callers exist (the `Player(NULL,"","")` templates in Rules).
- `src/Player.cpp:14-70` `Player::deckId` is never initialised (only `AIPlayerFactory` sets it); `DeckManager::getDeckMetaDataById` reads it. Initialise to 0.
- `src/DecisionContract.cpp:345` `for (size_t m = size-1; m > 0; m--)` underflows on an empty `mObjects` (never empty in a live game).
- `src/GameObserver.cpp:1268-1280` `imprintedCards.erase(begin()+ic)` inside a forward loop over the same vector skips the element after each erase.
- `src/DamagerDamaged.cpp:81` `char buf[6]; sprintf(buf, "%i", sumDamages())` - use `snprintf`.
- `src/ManaCost.cpp:1105-1115` `canAfford(cost, anytypeofmana>0)` leaks the first `Diff` result (see H3).

---

## Cross-slice notes (measured here, owned elsewhere)
- `ListMaintainerAbility::updateTargets` 15.6% and `GenericTriggeredAbility::Update` 6.5% of suite CPU: lords and phase triggers re-evaluate every tick (MTGAbility slice).
- `MTGAllCards::findType`/`Subtypes::find` 8.7%: string-keyed type lookups from `CardDescriptor::match`/`hasType(string)`; the three top self-time symbols in the whole profile are std::string internals from that map. Interning type ids at parse time is the cross-cutting win.
- `DescriptorTargetChooser::clone` copies a whole `MTGCardInstance` by value (3.2%): CardDescriptor derives from MTGCardInstance, so every chooser clone deep-copies three ManaCosts (H4 mechanism).
- LSan direct leaks in the suite: `SnowCost::isPaymentSet/doPay` (ExtraCost.cpp:162/204) - H3's idiom; `AACastCard::resolveSpell` (AllAbilities.cpp:11667); `AbilityFactory::parseMagicLine:2762`.

## What I did not do
- No commander-mode or human-seat measurement (the suite is AI-vs-AI headless): H2's size is arithmetic, H4's `refineForOptions` cost is from the loop bounds.
- The ASAN binary predates HEAD by the day's commits (it reports 3/2 suite failures vs 2/0 - stale build, not new bugs); every leak site quoted was re-verified against master sources.
- Did not read TargetChooser / AbilityFactory / AllAbilities, so ownership questions in L4 and the per-evaluation cost of `createTargetChooser` in M3 are stated from this side of the interface.
- Perf is from a -O0 debug build; ratios, not absolute times.
