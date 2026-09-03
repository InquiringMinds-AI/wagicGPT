# Audit 2026-09-02 — slice 4: the ability object model

Scope (master 859186536, read IN FULL): `src/AllAbilities.cpp` (12190), `include/AllAbilities.h`
(8289), `src/MTGAbility.cpp` (9360), `include/MTGAbility.h`, `src/AbilityParser.cpp`,
`src/ExtraCost.cpp` + `.h`, `src/ThisDescriptor.cpp`, `src/CardDescriptor.cpp`,
`src/TargetChooser.cpp` + `.h`, `src/TargetsList.cpp`, `src/Counters.cpp`,
`src/ReplacementEffects.cpp`, `src/PlayRestrictions.cpp`; plus `src/ActionLayer.cpp`,
`src/ActionElement.cpp` and `include/ActionLayer.h` (the GC the abilities live in) for context.
NOT read: `ManaCost.cpp`, `MTGCardInstance.cpp`, `WParsedInt.cpp`, `GameObserver.cpp` (other
slices) — where a finding depends on them it is marked SUSPECTED.

Measurement rig: worktree `audit-abilities` (removed at the end), ASAN+LSan build
(`Makefile.sdl-asan`) with counters compiled into `AbilityFactory::parseMagicLine`,
`parseCastRestrictions`, `getAbilities`, `ListMaintainerAbility::updateTargets`,
`ActivatedAbility::isReactingToClick`, `IfThenAbility::~IfThenAbility`,
`GenericTriggeredAbility::clone`, `MenuAbility::clone`; the full suite
(`WAGIC_TESTSUITE=1`, 1215 tests, one process) run once under
`ASAN_OPTIONS=detect_leaks=1`. Numbers are in section M at the bottom; each finding cites them.

Line numbers are master-tree lines. All file paths are under `projects/mtg/`.

---

## Ranked findings

### H0 — LEAK (VERIFIED, the largest leak in the slice) — `GenericAddToGame::resolve` / `GenericAbilityMod::resolve` drop their clone when the click path succeeds; `AProduceMana::produce` ("selectmana") never frees its five wrappers
`include/AllAbilities.h:3445-3463` (GenericAddToGame), `3398-3410` (GenericAbilityMod),
`src/AllAbilities.cpp:9974-10006` (AProduceMana::produce), other callers `10001-10005`,
`src/ReplacementEffects.cpp:162,174-178`, `src/Counters.cpp:224-230` (this one deletes correctly).

Mechanism. `resolve()` does `MTGAbility * toAdd = ability->clone();` then, if the clone has a
chooser, `if (toAdd->reactToTargetClick(source)) return 1;` — the clone is neither added to the
game nor deleted. For a `MayAbility` clone, `reactToTargetClick` (AllAbilities.cpp:8221-8227)
clones ITS nested ability into `mClone` and adds THAT to the game, so `toAdd` (a MayAbility
owning a full AManaProducer) is orphaned every time. `AProduceMana::produce` for `selectmana`
builds five `AManaProducer→MayAbility→GenericAddToGame` towers per mana tap, calls
`resolve()` on each and deletes none of the five `ga*` wrappers either — two leaks per colour
per tap.

Evidence (VERIFIED, section M): LSan attributes 4934 of the 5319 leaked allocations (78.9 KB
of 82.1 KB) in one suite run to `GenericAddToGame::resolve() AllAbilities.h:3447`; nothing
else in the process comes close. On the Vita every tap of a "any colour" producer
(`produceextra:selectmana`) leaks ~10 objects for the rest of the match.
Fix. In both `resolve()`s: `if (toAdd->reactToTargetClick(source)) { SAFE_DELETE(toAdd);
return 1; }` — the accepted MayAbility already moved its payload into the game; and in
`AProduceMana::produce` `SAFE_DELETE(ga0..ga4)` after the five `resolve()` calls (exactly what
`Counters.cpp:227-230` does for the same tower). Verify: rerun the ASAN suite; the
`GenericAddToGame::resolve` frame must disappear from the LSan report; `Res/test/` selectmana
fixtures unchanged.

### H1 — MEM-SAFE/LEAK — `IfThenAbility::clone()` shares `delayedElseAbility`; the `deletedpointers` guard that hides it is unsound and unbounded
`src/AllAbilities.cpp:8053-8070`, `include/MTGAbility.h:245-253`, `src/MTGAbility.cpp:7725-7729`,
`src/GameStateDuel.cpp:576`.

Mechanism. `IfThenAbility::clone()` deep-copies `delayedAbility` but copies the
`delayedElseAbility` POINTER (`NEW IfThenAbility(*this)` then only `a->delayedAbility =
delayedAbility->clone()`). Two IfThen objects (template + every clone a lord/foreach/may makes)
therefore own the same else-subtree, and `~IfThenAbility` would double-delete it. The code
"fixes" that with `MTGAbility::deletedpointers`, a thread_local `vector<void*>` of every
delayed-ability address ever deleted, consulted with `std::find` before each delete:

```cpp
if(delayedAbility && (std::find(deletedpointers.begin(), deletedpointers.end(), delayedAbility) == deletedpointers.end())) {
    deletedpointers.push_back(delayedAbility); // Fix to avoid crash on May abilities nested in IfThenElse Abilities.
    SAFE_DELETE(delayedAbility);
}
```
Three consequences:
1. The guard keys on ADDRESS. malloc reuses addresses constantly, so any later
   `delayedAbility`/`delayedElseAbility` that happens to land on an address already in the list
   is silently never deleted → a leak of that whole ability subtree, undetectable by reading.
2. The list only ever grows; it is cleared in `GameStateDuel::End()` only (line 576). The suite
   runs 1215 tests inside ONE `GameStateDuel`, and the Vita plays a whole session in one, so the
   vector grows for the life of the session and every `~IfThenAbility` is an O(n) scan over it
   (6111 `if ... then` lines in the primitives; every trigger clone/resolve churns one).
3. It is thread_local only to stop the threaded suite corrupting it — the comment in
   MTGAbility.h admits the hack's own hazard.

Evidence (section M, one suite run): 493 pushes into `deletedpointers`, 2 address HITS (deletes
skipped by the guard — i.e. 2 subtrees either double-owned or wrongly retained), list size
reached 203 on one worker thread and never shrank. VERIFIED.
Fix. In `IfThenAbility::clone()` add `a->delayedElseAbility = delayedElseAbility ?
delayedElseAbility->clone() : NULL;`, make the destructor a plain
`SAFE_DELETE(delayedAbility); SAFE_DELETE(delayedElseAbility);`, and delete `deletedpointers`
(header, the two static definitions, the `clear()` in GameStateDuel). Blast radius: every
`if/ifnot ... then ... else` card (the "May abilities nested in IfThenElse" case the hack was
added for is exactly the double-owner case the clone fix removes). Verify: full suite +
PARSETEST, then the ASAN suite (double-free would be reported at once); grep that no other
`deletedpointers` user remains.

### H2 — LEAK — `andAbility` never freed by `ATokenCreator`, `AManaProducer`, `AManifest`, `AProvoke`, `AExert`; and their `clone()` aliases it
`include/AllAbilities.h:4135,4439-4442,4432-4437` (ATokenCreator), `include/MTGAbility.h:404`
+ `src/MTGAbility.cpp:9268-9279` (AManaProducer), `include/AllAbilities.h:7916,8014-8017`
(AManifest), `8024,8071-8074` (AProvoke), `8080,8122-8125` (AExert). Parser sites that fill
them: `src/MTGAbility.cpp:3891,3919,3968,4008,4029,4078` (token), `5784` (add mana), `4325`,
`4341`, `4355`, `4370`.

Mechanism. 33 classes carry `MTGAbility * andAbility` (the `and!(...)!` follow-up). 28 of
them `SAFE_DELETE(andAbility)` in the destructor and deep-clone it in `clone()`; these five do
neither. Every parse of such a line allocates the follow-up subtree and drops it. Parses are not
one-per-card: `AbilityFactory::getAbilities` runs on every cast, on every zone change
(`MTGRules.cpp:2772` `OtherAbilitiesEventReceiver`), on every AI "is this card good"
evaluation (`AIPlayerBaka.cpp:2773` `effectBadOrGood` → `magicText` dry mode, which parses
and deletes), on equip/unequip (`AEquip::equip`), copy, flip, morph, aura rebind. Corpus
exposure: `grep -c 'token(.*and!(\|create(.*and!('` = 586 primitive lines; `:add{...} and!(`
= 26; manifest/exert/provoke = 4.

Evidence. VERIFIED by reading (destructor bodies) and by LSan: 142 leaked allocations rooted
in `ATokenCreator::resolve() AllAbilities.h:4316` in one suite run (the per-token
`TokenAndAbility` clone described below); the parse-time `andAbility` copies themselves are
freed only because the suite deletes the parent through `delete a` paths LSan cannot see as
leaks until process end — section M.
Fix. Add `SAFE_DELETE(andAbility)` to the five destructors and `if(andAbility)
a->andAbility = andAbility->clone();` to the five `clone()`s (the exact idiom AADrawer uses,
`src/AllAbilities.cpp:5169-5180`). Note ATokenCreator ALSO hands `andAbility->clone()` to
`spell->source->TokenAndAbility` (AllAbilities.h:4330) PER TOKEN CREATED — and
`MTGCardInstance` never deletes that field (`src/MTGCardInstance.cpp`: only `= NULL` at 473 and
a pointer copy at 217; `~MTGCardInstance` at 324 does not touch it), so every token minted by
an `and!(...)!` creator leaks its own clone of the follow-up as well (VERIFIED by grep; the
`MTGCardInstance` side belongs to another slice). `AACloner` (6844) and the instance copy
(217) alias the pointer between token instances, so the right fix is NOT a naive delete in
`~MTGCardInstance` — either give the card sole ownership with a deep copy at 217/6844, or
drop `TokenAndAbility` and have copiers re-clone from the creator's `andAbility`.
Verify: suite + `WAGIC_TESTSUITE` ASAN run, leak count for `ATokenCreator` drops to 0.

### H3 — PERF (Vita hot path) — `this(...)`/`while(...)`/`aslongas(...)` re-evaluate their condition EVERY TICK, some by re-parsing strings and allocating TargetChoosers
`include/AllAbilities.h:4800-4829` (AThis::Update→resolve), `4896-4921` (AThisForEach),
`3643-3705` (AAsLongAs::Update → ListMaintainerAbility::Update AND SorterFunction →
updateTargets twice), `src/MTGAbility.cpp:8644-8714` (updateTargets walks 9 zones × 2
players), `src/ThisDescriptor.cpp:758-765` (ThisVariable::match → `NEW WParsedInt` per tick),
`394-399` (ThisTargetCompare), `src/MTGAbility.cpp:106-1067` (parseCastRestrictions: `split`,
string finds, `createTargetChooser`+delete, even `card->clone()` for `can play`).

Mechanism. `AThis::Update` calls `resolve()` unconditionally each tick; `resolve()` runs
`td->match(source)` or, for the `while(restriction{...})` form, a full
`AbilityFactory::parseCastRestrictions` (allocates a `vector<string>` split, does dozens of
`find`s, and for `type(...)`/`lastturn(...)` conditions builds and deletes a TargetChooser).
`ThisVariable::match` builds a `WParsedInt` (its own mini-parser) every tick. `AAsLongAs`
calls `updateTargets()` twice per tick, then `cards.clear()`, so next tick re-adds everything
(`added()` per card). Corpus exposure: `this(` 2782 lines, `thisforeach(` 1030, `aslongas(`
1456, `this(variable{` 1055, `while(restriction` 124, `cantargetcard(` 769.

Evidence (section M, one suite run of 1215 short tests): `ListMaintainerAbility::updateTargets`
ran 1,191,957 times and `parseCastRestrictions` 41,708 times — versus 13,362 card parses and
4,477 click predicates; i.e. the per-tick condition machinery is ~90x the parser and ~270x the
click work. VERIFIED (counts).
Fix (bounded). (a) Cache the parsed condition: `AThis` with `restrictionCheck` should parse
the restriction string once (a small struct of enum+operands) and evaluate it per tick without
`split`/`find`; likewise `ThisVariable` should keep one `WParsedInt` and call a re-evaluate.
(b) Gate re-evaluation on change: recompute only when `game->stuffHappened`/phase change/a
zone-change event fired since the last tick (the engine already has
`mPropertiesChangedSinceLastUpdate` and `stuffHappened`). (c) `AAsLongAs::Update`: drop the
`ListMaintainerAbility::Update(dt)` call (SorterFunction already calls `updateTargets`).
Blast radius: every lord/this card; verify with the suite (behaviour must be identical) and a
Vita frame-time sample on a board with several lords.

### H4 — MEM-SAFE (latent, double-destroy family) — cloning a triggered ability is a crash: `GenericTriggeredAbility::clone()` calls `destroyCondition->clone()` on NULL, and every `Tr*::clone()` shallow-copies owned TargetChoosers/Counters
`src/MTGAbility.cpp:9074-9081`; `include/AllAbilities.h:255-266` (TrCardAddedToZone: 4 owned
TCs, dtor deletes, clone copies), `581-589` (TrCombatTrigger fromTc), `1510-1518` (TrDamaged),
`1563-1571` (TrLifeGained), `1605-1613` (TrVampired), `1642-1650` (TrTargeted), `1697-1702`
(TrCounter: `NEW Counter(*this->counter)` with `counter` legitimately NULL for
`counteradded((any))`), `1801-1806` (TrTotalCounter, same).

Mechanism. `parseMagicLine` always builds GTAs with `dc = NULL` (line 2655), so
`a->destroyCondition = destroyCondition->clone();` is a virtual call through NULL whenever a GTA
is cloned; if it survived that, the cloned trigger would share `toTcZone/toTcCard/fromTc*`
with the original and both destructors would free them (the "double destroy" shape the brief
describes). Today the parser keeps this dormant: `@...:` is split BEFORE `lord(`/`all(`/`&&`,
and `transforms(newability[@...])` re-parses text per target rather than cloning. The paths that
DO clone arbitrary nested abilities are `ALord/ATeach/AForeach::_added`, `MayAbility`,
`MultiAbility::clone/addToGame`, `ActivatedAbility::activateSideEffect` (`limit^...^` side
effects), `AAsLongAs`, `APaired`, `AGrant` — a trigger reaching any of them (e.g.
`{T}:draw:1 limit^@each my upkeep:...^3`, or a future parser reordering) crashes.

Evidence. Instrumented `GenericTriggeredAbility::clone` was reached 0 times in the full suite
(section M), so the crash is LATENT today; the NULL call and the shallow copies are VERIFIED by
reading. Rank stays HIGH because the fix is trivial and every new clone path added to the
parser (lords/mays over triggers) turns it live.
Fix. `GenericTriggeredAbility::clone`: `a->destroyCondition = destroyCondition ?
destroyCondition->clone() : NULL;`. Give each Tr* with owned pointers a real `clone()` that
deep-copies (`toTcZone`, `toTcCard`, `fromTcZone`, `fromTcCard`, `fromTc`, `gainException`,
`counterException`, `proliferateException`; `counter ? NEW Counter(*counter) : NULL`).
Verify: a synthetic fixture whose `limit^...^` side effect is a trigger, run under ASAN.

### M1 — LEAK — `SnowCost` and `Offering` leak `ManaCost` objects on every payment probe
`src/ExtraCost.cpp:160-166` (7× `ManaCost::parseManaCost("{g}"...)` per `isPaymentSet`, never
deleted), `194-227` (same in `doPay`), `1570-1578` (`Offering::canPay`: `diff` allocated, then
`Diff()` called twice more inline and never freed — 3 leaks per call).

Mechanism. `isPaymentSet`/`canPay` are polled: `ExtraCosts::isPaymentSet` runs on every
`reactToClick`, and AI oracles (`LegalActionsOracle`, `ExtraCosts::canPay`) call `canPay` for
each ability on each decision. Each poll leaks. Bounded by snow/offering cards in the deck, but
per-poll, so it grows through a match.
Evidence. LSan: 23+1 leaked allocations rooted at `SnowCost::isPaymentSet() ExtraCost.cpp:162`
and `SnowCost::doPay() ExtraCost.cpp:204` in the suite (VERIFIED); `Offering::canPay` VERIFIED
by reading only (no offering fixture exercised it).
Fix. Hoist the seven `parseManaCost` results into a `static const` table (or `ManaCost` members
built once in the ctor) and compare against those; in `Offering::canPay` reuse `diff` and
`SAFE_DELETE` it on every path (as `isPaymentSet` right below already does). Verify: ASAN suite
leak report loses the `SnowCost::isPaymentSet` / `Offering::canPay` frames; `Res/test/` snow
fixtures still pass.

### M2 — PERF — `ActivatedAbility::isReactingToClick` re-parses `limit` and `restriction{}` on every poll
`src/MTGAbility.cpp:8012-8032`, `src/AllAbilities.cpp:8861-8880` (GenericTargetAbility:
also rebuilds its whole TargetChooser via `createTargetChooser` when `tcString` is set and no
targets are chosen), `src/MTGAbility.cpp:9111-9116` (AManaProducer), `8563-8572`
(Trigger::triggerOnEvent castRestriction per event).

Mechanism. `isReactingToClick` is the click predicate: `ActionLayer::isReactingToClick`
walks ALL action-layer objects for a clicked card, `ActionLayer::setMenuObject` again, and the
AI's legal-move scans call it per ability per candidate. Each call with a `limit` does `NEW
WParsedInt(limit)` + delete, and each with a `castRestriction` does the full
`parseCastRestrictions` string machine (see H3). 284 primitives lines carry `limit:`, 401
activated lines carry `restriction{`.
Evidence. 4,477 `ActivatedAbility::isReactingToClick` calls in the suite (section M); the
re-parse per call is VERIFIED by reading, the cost share is SUSPECTED (dominated by H3 in the
suite; real play with a human clicking through menus is where this one is felt).
Fix. Parse `limit` once into an int/`WParsedInt` member at construction (it is a constant per
ability); memoise `parseCastRestrictions` per (ability, turn, phase, stuffHappened) or parse
the restriction once into a compiled form (H3 fix serves both).

### M3 — MEM-SAFE — `APowerToughnessModifier` (nonstatic) rebuilds its `WParsedPT` every tick and formats `triggers` into `char buffer[4]`
`include/AllAbilities.h:3086-3107` (Update: `SAFE_DELETE(wppt); ... wppt = NEW WParsedPT(...)`
per tick), `3117-3138` (second copy in the same tick for cda), `3146-3167` (addToGame),
`3211-3232` (getMenuText — re-parses on EVERY menu text request, and PTInstant reads the
menu text through it), `3089/3120/3149/3214`: `char buffer[4]; sprintf(buffer, "%i",
triggers);` — `triggers` is an unbounded activation count; at 1000 it writes 5 bytes.
Corpus: 50 `nonstatic` lines; `numofactivation` cards can reach three digits in a long game
only in theory, but the array is one keystroke from a stack smash and the four copies of the
block are pure duplication.
Fix. Keep the parsed `WParsedPT` and only re-evaluate its `getValue()` (WParsedInt already
re-evaluates dynamic expressions); make the four blocks one helper; size the buffer 16 or use
`snprintf`. Verify: `Res/test/` nonstatic fixtures + suite.

### M4 — PERF — card text is re-parsed from scratch at every zone change and every AI evaluation; nothing is cached per card model
`src/MTGAbility.cpp:6686-6858` (getAbilities: copies `magicText`, runs
`AutoLineMacro::Process` over the whole text, `parseMagicLine` per line; each line's parse is
hundreds of `find()`s and allocates a `WParsedPT` (5891) even for lines that are not P/T),
callers `src/MTGRules.cpp:2772` (every zone change of every card → `magicText(..., dest)`),
`src/AIPlayerBaka.cpp:2773` (`effectBadOrGood` → dry-mode parse+delete per card per
decision), `src/AllAbilities.cpp:2879,5299,5526,5948,11226,11267` (copy/aura
rebind/morph/flip/equip/mutate), `src/AllAbilities.cpp:9223` (ATransformer parses each
`newability[...]` string per granted target), `10209` (APhaseAction parses its line each fire —
and its constructor parses it once more with `card = NULL`, which `parseMagicLine` rejects at
line 2108, so that parse is dead work), `include/AllAbilities.h:7948,7964,8260`.

Evidence (section M): 13,362 `getAbilities` and 252,162 `parseMagicLine` calls (recursive
count) in one suite run — ~19 line-parses per card parse, ~11 card parses per test. VERIFIED
(counts); the caching design is a proposal.
Fix (design, forward-looking). Two cheap wins first: (a) `AbilityFactory::magicText` dry mode
(AI) parses only to ask `abilityEfficiency` — cache the verdict per `(card model id, mode)` in
`MTGCard` (it is a property of the printed text, not the instance); (b) hoist the `WParsedPT`
construction at 5891 behind a cheap `isdigit/+/-` prefix test. The larger win is a per-model
parsed-line cache keyed by `(magicText line, zone)` that stores the leaf constructor
arguments, but that interacts with the stateful `stored*` stash strings in `AbilityFactory`
and is a project, not a patch.

### M5 — MEM-SAFE — fixed-size arrays without bounds checks in the target-chooser parser
`src/TargetChooser.cpp:16` (`int zones[10]` local — `nonbattlezone` alone pushes 10; the
member it feeds is `zones[15]` at `include/TargetChooser.h:112`, so `|nonbattlezone,mystack`
overflows the LOCAL), `src/TargetChooser.cpp:1864-1868` (`TypeTargetChooser::addType` writes
`types[nbtypes++]` into `types[10]` with no check — a `target(a,b,c,...)` list of 11 names
overflows), `include/AllAbilities.h:6593,7547` (`MTGCardInstance * opponents[20]` in
AKjeldoranFrostbeast/AFlankerAbility filled from `getNextOpponent` with no bound; a creature
blocked by >20 creatures is impossible in practice but the array is unchecked),
`include/AllAbilities.h:5665` (`char land[20]` — inputs are the fixed literals in
`MTGAbility.cpp:7643`, safe today).
Fix. `int zones[15]` + `if (nbzones < 15)` guard on every push; `if (nbtypes < 10)` in
`addType` with a DebugTrace; `opponents` → `vector<MTGCardInstance*>`. Verify: PARSETEST +
suite (no behavioural change).

### M6 — LEAK — parse-time leaks in the trigger and castcard parsers
`include/AllAbilities.h:1529/1563-1566` (`TrLifeGained::gainException` never deleted),
`1662/1692-1695` (`TrCounter::counterException`), `1717/1796-1799`
(`TrTotalCounter::counterException`), `789` (`TrplayerProliferated::proliferateException`, no
dtor at all), `src/MTGAbility.cpp:4492,4502` (`WParsedInt * val = NEW WParsedInt(...)` for
`kicked!:`/`costx!:` never deleted; 151 lines in borderline.txt use them), `3882/3999`
(`multiplier` leaked on the "ID NOT FOUND" error return).
Each is one small object per PARSE, but parses are per cast/zone-change/AI-evaluation (M4), so
they accumulate across a match and across the 1,200-game suite process.
Fix. Add the four `SAFE_DELETE`s to the destructors (and deep-copy in `clone()` per H4); make
`val` a stack `WParsedInt`. Verify: LSan run.

### M7 — MEM-SAFE (SUSPECTED) — `MenuAbility::clone()` indexes `optionalCosts[i]` for every `abilities[i]`
`src/AllAbilities.cpp:8632-8640`. `optionalCosts` is only filled by `GenericPaidAbility`
(one entry, `4600`) while `abilities` can hold 2+ (`4544-4553`); every other builder
(`GenericChooseTypeColorName`, `AARemoveSingleCounter`, `AAProliferate`, `GenericFlipACoin`,
`REDrawReplacement`) leaves `optionalCosts` empty. `NEW ManaCost(optionalCosts[i])` with
`i >= size()` is an out-of-bounds vector read (UB; ASAN reports it as heap-buffer-overflow when
it lands past the allocation). Reached only if a live `MenuAbility` is cloned
(`MultiAbility::resolve` 8721 clones any `MayAbility` leg — `MenuAbility` IS a `MayAbility`).
Evidence. Instrumented: 0 clones with `optionalCosts.size() < abilities.size()` in the suite
(section M) — SUSPECTED/latent. Fix: `if (i <
(int)optionalCosts.size() && optionalCosts[i]) a->optionalCosts.push_back(NEW
ManaCost(optionalCosts[i]));`.

### M8 — MEM-SAFE — `ExtraManaCost::clone()` aliases `costToPay`; `~ExtraCost` deletes it
`src/ExtraCost.cpp:92-96` vs `36-40`. Every other `ExtraCost` subclass re-clones `tc`; none
re-clones `costToPay`, and `ExtraManaCost` is the only one that has it. `ExtraCosts::clone()`
(1879-1888) and `ManaCost`'s copy paths (`ManaCost.cpp:478,510,684,757`) clone cost lists
wholesale, and `MenuAbility` builds `toPay` lists containing `ExtraManaCost` (`8544,8585`) —
if any of those lists is copied, the two `ExtraManaCost`s double-free `costToPay`.
SUSPECTED (no crash observed; `toPay` is built after the copy paths run today).
Fix: `ec->costToPay = costToPay ? NEW ManaCost(costToPay) : NULL;`.

### M9 — CRUD (owner rule: diagnostics compile out of release) — env-gated probes shipping in release builds
`src/MTGAbility.cpp:2449-2453` (`getenv("WAGIC_MAYPROBE")` → `fprintf(stderr, "STASHGATE…")`
on every reveal-line parse, gated only by `#ifndef PSP`), `src/AllAbilities.cpp:8096-8105`
(`WAGIC_MAYPROBE` in `MayAbility::Update`), `568-573` and the `REVEAL_DBG` macro at `644`
(`getenv("WAGIC_REVEAL_DEBUG")` evaluated per reveal tick). The FIZZLELOG block right next to
them (7004) shows the convention the owner wants: `#if defined(_DEBUG) ||
defined(WAGIC_DEVLOGS)`. Fix: wrap the three sites in that gate. Zero behavioural change.

### M10 — MEM-SAFE — parse-failure NULLs dereferenced (script error becomes a crash instead of a loud reject)
`src/MTGAbility.cpp:2734-2744` (`tc = createTargetChooser(...)` then `tc->targetter` with no
NULL check — a malformed `target(...)` crashes; contrast the guarded copy at 5808),
`5147` (`castTargets` unchecked), `3741-3742` (`grant` with empty stash → `AGrantWrapper(NULL)`
→ `AGrant::getMenuText` derefs), `src/AllAbilities.cpp:1453` (`GenericScryAbility` aicode
parse `a3->oneShot`; the reveal twin at 77 checks), `4089,4143` (AASetTypeChosen/AASetNameChosen
`abilityAltered->oneShot`; the colour twin at 4034 checks), `1307,1346,1376` (MTGScryCards
`delayedA->oneShot`), `4532,4550,4562` (GenericPaidAbility `baseAbility->target`), `10211`
(APhaseAction `ability->clone()`), `include/AllAbilities.h:8261` (AModularAbility
`modCounter->oneShot`). Also `src/MTGAbility.cpp:4783-4808`: `altermutationcounter:` MUTATES
the card and fires an event during PARSE (so an AI dry-run parse of the card changes game
state), and `mutationover:`/`mutationunder:` read `splitMutated[1]` — a DIFFERENT, empty
vector — undefined behaviour the moment a card uses those keywords (none does today).
Fix: `if (!x) return NULL;` at each site (the loud-rejection validator already routes NULLs);
move the mutation side effect into an ability's `resolve()`; fix the two copy-paste vector
names. Verify: PARSETEST + WAGIC_VALIDATE pass.

### M11 — PERF/CRUD — `ListMaintainerAbility::updateTargets` and `ActionLayer` scans are O(zones×cards) per lord per tick
`src/MTGAbility.cpp:8644-8714`: two `map<MTGCardInstance*,bool>` rebuilt every tick (allocating
nodes), 9 zones × both players walked with `canTarget` per card, each `canTarget` calling
`MTGGameZone::intToZone` per configured zone (`TargetChooser.cpp:2058`). 2301 `lord(` + 1629
`foreach(` + 1456 `aslongas(` + `teach(` lines. `AForeach::foreachMatchCount`
(`AllAbilities.h:4695-4717`) repeats the full walk for every menu-text request.
`ActionLayer::isReactingToClick/setMenuObject/getMenuIdFromCardAbility` each walk all
`mObjects` (linear per click; fine), but `ActivatedAbility::isReactingToClick` for a
planeswalker (`MTGAbility.cpp:8044-8053`) walks all mObjects with a `dynamic_cast` per
object per click check → O(abilities²) per menu build with several planeswalkers.
Fix: see H3(b) (event-gate the recompute); for `updateTargets` keep `cards` as a sorted
`vector` or `unordered_set` and skip zones the chooser cannot target before iterating (it does
`canTarget(zone)` already — the map churn is the cost).

### L1 — CRUD — dead and duplicated code in the ability layer
- `src/AllAbilities.cpp:8676-8680` `MenuAbility::~MenuAbility`: `mClone = NULL; delete
  mClone;` — deletes NULL (dead), the comment says why the real delete was removed.
- `src/AllAbilities.cpp:10144-10150` `APhaseAction` ctor parses `sAbility` with `card = NULL`,
  which `parseMagicLine` rejects at `MTGAbility.cpp:2108` — the result is always NULL, so
  `psMenuText` is always `sAbility`; delete the parse.
- `src/AllAbilities.cpp:8932-8937, 8943-8948` `AAlterCost::addToGame`: `NEW ManaCost();
  copy(empty); delete` — a no-op.
- `src/AllAbilities.cpp:1516-1517` (`ability->target = target;` before `if (ability)`),
  `5530/5537` (`a->source =` before `if (a)`), `11039` (`_target->mutation` before `if
  (_target)`): deref-before-check; either drop the check or move it up.
- `src/MTGAbility.cpp:1030-1041` `can play` restriction clones a whole `MTGCardInstance`
  per evaluation just to change its types.
- `src/MTGAbility.cpp:7735-7738` copy ctor copies 50 of `menuText[256]`.
- `include/AllAbilities.h:3651-3665` `AAsLongAs::findMatchingAmount` overwrites `Value` per
  zone, so `compare` only ever counts the LAST zone (sideboard) — a correctness bug hiding as
  dead work. `7630-7635` `ABushidoAbility::destroy` subtracts from `attackCost`, not
  `bushidoPoints`.
- `src/AllAbilities.cpp:2865` `source->GrantedAndAbility = andAbility` leaves a dangling
  pointer on the card after the one-shot copier is deleted (only read on the next line; make it
  a local).

### L2 — MEM-SAFE — uninitialised-member reads in constructors
`src/AllAbilities.cpp:163` and `1079`: `CardDisplay(_id, game, x, y, listener, NULL,
nb_displayed_items)` passes `x`, `y`, `listener`, `nb_displayed_items` — members of the
`CardDisplay` base being constructed — as its own constructor arguments (the same class of bug
the ASeizeWrapper/AVanishing comments at 10499 and 9897 describe as already fixed elsewhere).
Harmless today because `init()` overwrites them, but ASAN/MSAN-visible and one refactor from
real. Fix: pass literals (0, 0, NULL, 7).

### L3 — LEAK — `AEquip::unequip` cannot free abilities `equip` never added
`src/AllAbilities.cpp:11245-11249` marks an `AACopier`-core ability `forceDestroy=1` and
`continue`s without `addToGame`; `11180-11191` then `game->removeObserver(a)` it — a no-op for
an unregistered object — so it leaks per attach cycle. Fix: add `dynamic_cast<AACopier*>` to
the delete list at 11183, mirroring the `AANewTarget` case.

---

## What the action-layer GC looks like from the ability side (context for slices 3/5)
Ownership is by convention, not by type: a `MTGAbility` is owned by (a) the ActionLayer once
`addToGame()` ran (deleted via `garbage`/`cleanGarbage`), or (b) its wrapper/parent (every
`NestedAbility` deletes `ability`; `ALord/ATeach/AForeach` keep `map<Damageable*,
MTGAbility*>` of clones they ALSO added to the game and rely on `forcedAlive=1` to keep the
layer from freeing them), or (c) nobody (one-shots resolved and deleted inline). The three
invariants the current code depends on and that H1/H4 violate: `clone()` must deep-copy every
pointer the destructor deletes; an object must be in exactly one of (a)/(b); `forcedAlive`
must be cleared by the owner before it asks the layer to remove the object (ALord::removed does;
`InstantAbility::testDestroy` now honours it — the `lifeline.txt` UAF in the header comment).

## Measurements (section M)
Rig: worktree `audit-abilities` off master 859186536, `Makefile.sdl-asan` build with the
counters listed at the top compiled into `MTGAbility.cpp`/`AllAbilities.cpp` (worktree only,
discarded), suite run `WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 ASAN_OPTIONS=detect_leaks=1` under
`systemd-run --scope -p MemoryMax=8G`, `timeout 900`. Logs: `~/.gatelogs/audit-abilities/`.

Counters at process exit (whole suite, 1215 tests, threaded workers, one process):
```
parseMagicLine=252162  parseCastRestrictions=41708  getAbilities=13362
updateTargets=1191957  activated_isReacting=4477
ifthen_push=493  ifthen_hit=2  ifthen_maxsize=203   (H1)
gta_clone=0  menu_clone_oob=0                        (H4, M7: latent)
```
ASAN: 0 `ERROR: AddressSanitizer` (no UAF/overflow reached in the suite).
LSan: `82108 byte(s) leaked in 5319 allocation(s)`; attribution by deepest slice frame
(LSan capped the record list at 5000, all reported records are Indirect; roots as listed):
```
78944B 4934obj  GenericAddToGame::resolve()        AllAbilities.h:3447   (H0)
 2096B  142obj  ATokenCreator::resolve()           AllAbilities.h:4316   (H2)
  368B   23obj  SnowCost::isPaymentSet()           ExtraCost.cpp:162     (M1)
   88B    6obj  AACloner::resolve()                AllAbilities.cpp:6845 (H2, TokenAndAbility alias)
   64B    4obj  CardDescriptor::CardDescriptor()   CardDescriptor.cpp:10 (TargetChooser cd leak, M6 family)
   64B    4obj  ActionLayer::receiveEventPlus()    ActionLayer.cpp:287   (slice 3)
   52B    4obj  GenericPaidAbility::resolve()      AllAbilities.cpp:4567 (nomenuAbility never freed on the menu path)
   16B    1obj  SnowCost::doPay()                  ExtraCost.cpp:204     (M1)
```
A second, earlier ASAN suite run by another slice today (`~/.gatelogs/asan/suite.stderr`)
reported `147664 byte(s) leaked in 9442 allocation(s)` on the unmodified tree with the same
`ATokenCreator::resolve` root visible — the numbers move with test order/threading, the sites
do not.

Suite result under the instrumented build: same 2 known failures (lifeline.txt,
merrow_reejerey.txt); the instrumentation changes no behaviour.

## Not measured / not read
Vita frame timing (no device in this rig); `ManaCost`, `MTGCardInstance`, `WParsedInt`,
`GameObserver`, `ActionStack` internals (other slices) — findings that lean on them (H2's
`TokenAndAbility` ownership, M8) are marked SUSPECTED where the cited code is outside the slice.
