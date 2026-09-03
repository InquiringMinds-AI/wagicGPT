# Wave-54 lane G — audit lane A: ability object model (ownership, leaks, latent clones)

Branch `w54-lane-G`, worktree `worktrees/lanes/w54-G`, seeded from master dab9f72f4 (engine
= ef133ea2a). Items: A8, A9 (incl. the `TokenAndAbility` ruling), A10, A12 (`ExtraCost.cpp`
part), A14, A28, A29, A30, A31, A52, A33's two probe sites, L13, L14. Every code comment for
the lane is tagged `#W54-G (item)`.

## Gate numbers

| gate | binary | result |
|---|---|---|
| baseline, this worktree, `make` from the seeded objs | ef133ea2a engine | suite 1219 / 0 failed / 0 timed out, 49 AI / 0; PARSETEST 2258 / 0 |
| baseline ASAN+LSan full suite (single-threaded, MemoryMax=8G, timeout 900) | ef133ea2a engine, `Makefile.sdl-asan` full build | suite 1219 / 0, 49 AI / 0, **0 `ERROR: AddressSanitizer`**, LSan **83,720 B / 5,434 allocations** |
| after, normal build (`-j2`) | lane G commit | suite 1219 / 0 failed / 0 timed out, 49 AI / 0; PARSETEST 2258 / 0 (run twice: once before the disable flag was added, once on the final tree — identical) |
| after ASAN+LSan full suite, same recipe | lane G commit | suite 1219 / 0, 49 AI / 0, 0 timeouts, **0 `ERROR: AddressSanitizer`**, LSan 1,007,991 B / 4,893 allocations (29 direct records / 364 indirect — an UNCAPPED report; see the cap finding below) |

Baseline LSan by frame (a leak block is counted once per named frame it contains, so the
rows overlap; the `CardDescriptor::CardDescriptor` row is the chooser inside the
`GenericAddToGame` towers, not a separate root):

| frame | before | after |
|---|---|---|
| `GenericAddToGame::resolve` | 78,704 B / 4,919 | ABSENT |
| `ATokenCreator::resolve` | 3,956 B / 269 | 497,846 B / 1,442 — ALL at `AllAbilities.h:4314` = `Token::clone()` (the token INSTANCE, indirect, pre-existing — see below); 0 at the `TokenAndAbility`/`andAbility` lines |
| `AACloner::resolve` | 144 B / 10 | 8,300 B / 22 — `MTGCardInstance::clone` (the tokCard instance, indirect, pre-existing); 0 at the `TokenAndAbility` line |
| `SnowCost::isPaymentSet` | 176 B / 11 | ABSENT |
| `SnowCost::doPay` | 16 B / 1 | ABSENT |
| `CardDescriptor::CardDescriptor` | 78,832 B / 4,927 | 704 B / 44 (choosers under the leaked parse roots below, not towers) |
| `GenericPaidAbility::resolve` | 4 B / 1 | ABSENT |
| `IfThenAbility` (any frame) | 4 B / 1 | 19,618 B / 57 — all `Token::clone <- ATokenCreator::resolve <- IfThenAbility::resolve` (token instances, indirect); 0 in `IfThenAbility::clone`/dtor |
| `parseMagicLine` (any frame) | 24 B / 3 | 40,387 B / 113 (direct roots: `createTargetChooser` under `GenericRevealAbility::resolve`/`MTGAttackRule`, Momir parses, `ManaCost::Diff`/`parseManaCost` under `LegalActionsOracle` = lane C's A12 `ManaCost.cpp:1110` site) |

Success criterion from the plan: total < 4 KB and the six named frames absent, 0 ASAN
errors, suite green. Verdict: the six named frames are absent or reduced to pre-existing token-instance
indirects (see the cap finding); 0 ASAN errors; suite green. The "< 4 KB total" half of the
criterion was calibrated against a number that turns out to be an artifact and is NOT met —
nor can it be by any lane, because the true residual is dominated by process-lifetime globals.

### The LSan record cap — why every audit total read ~82 KB

LeakSanitizer stops collecting after 5,000 leak records (`kMaxLeaksConsidered`, hard-coded;
`max_leaks` does not lift it). The baseline run on this worktree reports **0 direct / 5,000
indirect** records and 83,720 B; a 319-fixture SUBSET on the pre-fix binary reports 0 direct /
5,000 indirect and 83,524 B / 5,420 — the same number for a quarter of the suite. The
`GenericAddToGame` towers (each `MayAbility`+`AManaProducer`+eight empty `ManaCost`s, a
distinct stack per fixture) filled the record list and crowded out every direct root, so the
audit's 82 KB figures measured the cap, not the heap. With A8 shipped the after-run is the
first UNCAPPED full-suite LSan report of this engine (393 records): 1,007,991 B / 4,893
allocations, of which the direct roots are (a) `Rules::loadAllRules` and its player/zone
templates (~300 KB, process-lifetime, never freed at exit — lane C/E files), (b)
`AutoLineMacro::AddMacro` (~36 KB one-time), (c) `LegalActionsOracle` → `ManaCost::canAfford`
`Diff`/`parseManaCost` (lane C's A12 site), (d) small parse-time choosers under
`GenericRevealAbility::resolve` / `MTGAttackRule::receiveEvent` / Momir, (e)
`AAProliferate::resolve`; and the large INDIRECT mass is token `MTGCardInstance`s
(`Token::clone` under `ATokenCreator::resolve`, ~2.6 KB each) hanging off those roots.

To show the token/IfThen indirects are not this lane's: an uncapped before/after on a 40-fixture
token-heavy subset (pre-fix master `bin-asan` of Sep 1 vs this tree, same registry) gives
**611,974 B / 3,631 vs 614,278 B / 3,631 — identical record set**, every `Token::clone`,
`MTGCardInstance::clone`, `IfThenAbility::resolve` and Rules row equal in object count (the +2
KB is a 32-byte-per-instance size delta between the two builds' `MTGCardInstance`). Logs:
`~/.gatelogs/w54G/cmp2{A,B}.*`, `asan{0,1,2}.*`.

Recommendation for the ledger: retire the 82 KB totals; the by-frame measure stands, and the
per-game leak rate question is now answerable (a per-game LSan is no longer blinded by towers).

## What shipped (by item)

**A8 — `GenericAddToGame::resolve` / `GenericAbilityMod::resolve` orphaned their clone; `AProduceMana::produce` never freed its five wrappers.** The fix is an ownership rule, not a
blanket delete, because the two kinds of clone that reach the click path end up owned by
different things: a `MayAbility` wrapper moves its payload into the game as `mClone`
(`MayAbility::reactToTargetClick`) and is finished either way — it is deleted; any OTHER
chooser-bearing ability that ACCEPTED the click has become the action layer's current
waiting action (`TargetAbility::reactToClick` → `setCurrentWaitingAction(this)`) and later a
`StackAbility` entry — deleting it would be a use-after-free, so it is `addToGame()`'d (the
idiom the rejected branch of `GenericAddToGame::resolve` already used); a rejected non-May
clone is referenced by nobody and is deleted. The legend-rule and planeswalker-rule menus
(`MTGRules.cpp:4102/4219`), `activate`/`emblem` keywords, draw replacements, totems and
pairing are the non-May callers. `selectmana`'s five `ga*` wrappers are deleted after their
`resolve()` (the `Counters.cpp` suspend-tower idiom). `ReplacementEffects.cpp` got the same
family of fixes: the one-shot clone is deleted after `resolve()`, an unconsumed `selection` is
freed, and — a latent double-free found on the way — a draw of N>1 cards handed the SAME
`selection` pointers to N `MenuAbility`s, each of which deletes its abilities; menus after the
first now get their own clones.

**A9 — `andAbility` never freed / aliased by five classes; `TokenAndAbility` ownership ruling.**
`ATokenCreator`, `AManaProducer`, `AManifest`, `AProvoke`, `AExert`: `SAFE_DELETE(andAbility)`
in the destructor and a deep copy in `clone()` (the AADrawer idiom the other 28 carriers use).
*Ruling:* **an `MTGCardInstance` OWNS its `TokenAndAbility`.** Every assignment site deep-copies
— `MTGCardInstance::copy()` (the copier path, which also frees any copy it already held),
`AACloner::resolve` (was a pointer alias), `ATokenCreator::resolve` (already cloned) — and
`~MTGCardInstance` frees it. Why this and not "drop the field and re-clone from the creator":
`AACopier::resolve` reads `_target->TokenAndAbility` off a token on the battlefield, which has
no link back to the creator ability; the field is the only carrier of the follow-up for a
copied token. Why sole ownership is safe: zone moves rebuild instances from the model
(`MTGCardInstance(MTGCard*, ...)` → `initMTGCI` → NULL), so the pointer never crosses the
`previous` chain; `clone()` builds from the model too; the implicit copy constructor is not
used on live instances (the `createSnapShot` comment documents why it was retired), and
`CardDescriptor` (which IS an `MTGCardInstance`, copied by value in
`DescriptorTargetChooser::clone`) is default-constructed with the field NULL. Cost: one extra
clone per token copy/populate of an `and!(...)!` token — bounded and rare.

**A10 — `IfThenAbility::clone()` shared `delayedElseAbility`; the `deletedpointers` guard.** The
clone deep-copies both branches; the destructor is two plain `SAFE_DELETE`s; the
thread_local address list (header, the two static definitions, the `GameStateDuel::End`
clear) is gone. No `deletedpointers` user remains (grep).

**A12 (`ExtraCost.cpp`) — `SnowCost` / `Offering` parse-and-drop.** A file-static
`snowPoolCanAfford(source, "{g}")` parses, asks the pool, deletes — the 14 inline
`canAfford(parseManaCost(...))` sites route through it. `Offering::canPay` reuses its `diff`
instead of allocating two more. (The lane C sites — `ManaCost.cpp`, `ManaEngine.cpp`,
`MTGRules.cpp` — are untouched.)

**A14 — cloning a triggered ability crashed.** `GenericTriggeredAbility::clone` no longer calls
through a NULL `destroyCondition`; `TrCardAddedToZone` (4 choosers), `TrCombatTrigger`,
`TrDamaged`, `TrVampired`, `TrTargeted`, `TrLifeGained` (+`gainException`), `TrCounter` /
`TrTotalCounter` (`counter` may legitimately be NULL; +`counterException`) and
`TrplayerProliferated` (+`proliferateException`) have real deep-copying `clone()`s. The base
`Trigger::tc` was already deep-copied/freed by `ActionElement`'s copy ctor/dtor, so it needed
nothing.

**A28 — parse-failure NULLs; a keyword that mutated state during parse.** NULL guards (loud
reject, never a crash) at the `target(...)` chooser (`MTGAbility.cpp`), `maxcast/maxplay`
scope, `grant` with an empty stash, `AModularAbility`, the two `aicode` parses in
`GenericRevealAbility`/`GenericScryAbility`, the three `delayedA` sites in `MTGScryCards`, the
two unguarded `abilityAltered` sites (`AASetTypeChosen`/`AASetNameChosen`), the three
`baseAbility` parses in `GenericPaidAbility::resolve`, and `APhaseAction::Update`.
`altermutationcounter:` / `mutationover:` / `mutationunder:` are now an ability
(`AAAlterMutationCounter`, resolve-time side effect; the last two also indexed a different,
empty vector) — no shipped primitive uses any of the three (grep), so no card's behaviour
changes.

**A29 — unbounded fixed arrays.** `TargetChooserFactory::createTargetChooser`: the local is
`zones[15]` (the member's size) with a bounded push (`TC_PUSH_ZONE`) at all 93 sites;
`TypeTargetChooser::addType` refuses an 11th type with a DebugTrace; `opponents[20]` in
`AKjeldoranFrostbeast` / `AFlankerAbility` is a vector.

**A30 — `APowerToughnessModifier` (nonstatic).** The four copies of the re-parse block are one
`rebuildPT()`; `triggers` is formatted with `snprintf` into 16 bytes. The re-parse itself is
kept on purpose: `PT` is rewritten in place the first time `numofactivation` is substituted, so
a "keep the parsed WParsedPT" rewrite would change verdicts — that is lane B's perf territory
and needs its own fixture.

**A31 — small per-parse leaks.** `kicked!:` / `costx!:` use a stack `WParsedInt`; the token
parser frees `multiplier` on its two "ID NOT FOUND" returns; the four Tr* exception choosers
are freed (with A14); `AEquip::unequip` deletes an `AACopier`-core ability that `equip()` marks
`forceDestroy` and never adds; `GenericPaidAbility::resolve` frees `nomenuAbility` and an
unconsumed `selection`; the dead `erase` delete branch in `MTGCardInstance::remove{Protection,
CantBeTarget,CantBeBlockedBy}` is removed (the choosers are owned by `AProtectionFrom` & co —
the "never freed" reading in the ledger was wrong on that point; the branch was the hazard).

**A52 — latent double-owner / OOB.** `MenuAbility::clone` only copies `optionalCosts[i]` that
exist (size and alignment preserved); `ExtraManaCost::clone` deep-copies `costToPay`.

**A33 (my two sites).** The `WAGIC_MAYPROBE` probes in `parseMagicLine` and `MayAbility::Update`
and the `WAGIC_REVEAL_DEBUG` trace + `REVEAL_DBG` macro are under
`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)` (release: compiled out; the macro is an empty
statement).

**L13 / L14.** Copy ctor copies the whole `menuText[256]`; `ABushidoAbility::destroy` subtracts
from `bushidoPoints` (was `attackCost`); `GrantedAndAbility` is no longer parked on the card by
`AACopier::resolve` (it dangled once the one-shot died — the follow-up is read directly);
`APhaseAction`'s constructor no longer parses with `card = NULL` (always NULL) and no longer
leaves the member dangling; `MenuAbility::~MenuAbility` lost a delete-of-NULL; four
deref-before-NULL-check sites reordered; `CardDisplay` bases are built from literals instead of
their own uninitialised members. NOT taken from L13: `AAlterCost::addToGame`'s "no-op" blocks
(they copy an EMPTY cost into the increased/reduced cost when its converted cost is 0 — that
clears X/extra content, not provably a no-op; left alone) and the two lane-B regions
(`MTGAbility.cpp:1030` can-play clone, `AAsLongAs::findMatchingAmount`).

## Disable flag

`WAGIC_W54G_LEGACY=1` (read once) restores the two output-affecting behaviours: the pre-A8
click-path ownership (the accepted clone is orphaned again) and `ABushidoAbility::destroy`'s
`attackCost` subtraction. Everything else is ownership/leak repair with no output surface.

## Suite pins

None could be expressed for the leak fixes (LSan is their instrument). For A14 a probe was
built and RETIRED: test primitive `{T}:draw:1 limit^@each my upkeep:draw:1^1` with a one-click
fixture — on the ef133ea2a archive binary it did not crash, it simply did not draw (the parser
splits `@...:` before the `{cost}:` branch that extracts `limit^`, so the line became a bare
trigger and no clone path was reached). That confirms the audit's "0 hits, dormant" reading:
today's parser has no route to `GenericTriggeredAbility::clone`, so no fixture can turn the fix
red-then-green. The probe files were removed.

## Predictions (falsifiable)

1. ASAN full suite after: `GenericAddToGame::resolve`, `ATokenCreator::resolve`,
   `AACloner::resolve`, `SnowCost::*`, `GenericPaidAbility::resolve` frames absent.
2. No `ERROR: AddressSanitizer` — in particular no use-after-free from the A8 non-May
   `addToGame` path (legend rule / planeswalker rule fixtures exercise it).
3. Suite verdicts identical to baseline (1219/0, 49 AI/0, PARSETEST 2258/0).

## What was NOT verified

- The A8 non-May accepted path (`activate`/`emblem` keywords, draw-replacement menus,
  `MTGCardInstance` totems, pairing) is verified only as far as the suite drives it; a live
  selfplay corpus under ASAN was not run.
- `AAAlterMutationCounter` has no user in the primitives; it compiles and parses but no fixture
  exercises it.
- A14's deep clones are latent by construction (see Suite pins) — they are verified by reading
  and by the ASAN suite not regressing, not by a clone actually happening.
- Per-game leak RATE on the Vita (the audit's open question) — desktop LSan only.
- The `w54gLegacyBehavior()` flag was not exercised by a gate run (default-off path is the
  gated one).
- The threaded suite (`THREADS=16`) was not run; single-threaded is the ground truth per the
  skill.
