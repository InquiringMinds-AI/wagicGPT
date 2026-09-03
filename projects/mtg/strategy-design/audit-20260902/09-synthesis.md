# Audit 2026-09-02 — synthesis (slices 01-08 deduplicated, ranked, laned)

Master 859186536. Inputs: 00-brief.md, 01-gpt-a, 02-gpt-b, 03-gpt-c, 04-abilities, 05-engine-core,
06-ai-baka-suite, 07-ui-resources, 08-jge-vita; plus the already-shipped wave-53 lanes S (hand-refresh
dedup), W (frame telemetry + GuiAvatars zone-widget skip), V (DFC art) and wave-52 D28 (RenderSpell /
Arena grouping — OWNER ITEM). Nothing here was re-measured; every number is a slice's or a lane's.
No code edited, no git, no wagic run.

## 0. Reading rules for the numbers

**Vita scaling.** The slices disagree on the desktop→Vita factor (brief/slice 7: 10-20x; lane S: 20x;
slice 6: 30-50x; lane W: refuses to quote one) and on the build they measured (the tracked desktop
makefile is **-O0**; the Vita is **-O2**; lane W rebuilt at -O2 and found string-heavy engine code
2-3x cheaper there, render code ~1x because GL-driver time does not optimise). Rule used below:
- from a **-O2** desktop number: Vita = x10-20;
- from a **-O0** desktop number on CPU-bound engine/string code: Vita = x5-10 (the -O0 penalty
  cancels part of the CPU gap);
- render numbers: **no factor** — vitaGL/GXM has no desktop analogue; lane W's `avg_rnd`/`avg_swp`
  telemetry on the next VPK is the only instrument. Estimates are tagged EST; the two device-measured
  items (A1, A2's arena ratchet) are tagged DEVICE.

**Vita-impact column.** FRAME = changes the console's frame time; HEAP = changes resident/peak heap
on the 128 MB newlib block (~40 MB free in-game); CRASH = changes crash rate; NONE = desktop/suite
only. A device budget worth keeping in mind (slice 8, memlog): live data grows only ~0.2 MB/game
while the arena ratchets ~1.4 MB/game — on the Vita, FRAGMENTATION (A2) outweighs the sum of every
per-game leak below by ~7x.

**Already shipped this week (not re-filed).** Lane S batched `usableAbilityCards` + the non-mana-tap
index (this IS slice 5 H5's "cheaper second step" and discharges slice 7 F6's blocker part: lane S
timed `canDeclareAttacker+canDeclareBlocker x19` at 0.002 ms). Lane W skips the ten invisible zone
widgets and fetches one icon (discharges the zone-icon 60 lookups/frame of slice 7 F2 and ~94% of
F7's 20 battlefield scans). Lane V removed `CardGui::Render`'s name-only `getCardByName` re-lookup
for face-swapped cards (part of slice 7 F4). D28 (RenderSpell O(n^2), Arena grouping) stays with the
owner; measured facts about it are in §4, not in a lane.

---

## 1. Ledger — HIGH

### A1 — HIGH · MEM-EFF/MEM-SAFE · VERIFIED (DEVICE) · **OWNER DECISION** — Vita crashes at the 128 MB newlib heap while ~158 MB of vitaGL RAM pool sits idle
- Where: `JGE/src/Vitamain.cpp:373` (`vglInitExtended(..., 24*1024*1024, ...)`); no
  `_newlib_heap_size_user` anywhere (only the comment at `Vitamain.cpp:57-63`, which is wrong —
  vitaGL's EXTERNAL pool is never the newlib heap, `vendor/vitaGL/source/mem_utils.c:600`).
- Slices: 8-H1.
- Mechanism: newlib carves ONE 128 MB heap at start; vitaGL takes the rest minus `ram_threshold`
  as its own pool (`vgl.c:563-570`), ~158 MB that every memlog line shows constant
  (`ram_free=157758608`). The five 2026-08-30..09-01 `std::bad_alloc` dumps in LoadPNG are heap
  exhaustion of the 128 MB block, not VRAM.
- Magnitude (DEVICE, `psp-work/logs/20260902-vpk11/memlog.txt`): `heap_arena` 48.7 MB (game 1) ->
  82.9 MB (game 24); live 39 -> 44 MB; arena ratchets ~1.4 MB/game -> wall ~30 games out,
  matching "~10 games per crash" pre-nothrow.
- Vita impact: **CRASH** (the crash ceiling itself).
- Fix: `extern "C" unsigned int _newlib_heap_size_user = 224 * 1024 * 1024;` at file scope in
  Vitamain.cpp (VITA-guarded). RAM pool shrinks to ~62 MB — still above the 48 MB HUGE_CACHE_LIMIT
  plus 61 MB free VRAM. Keep `ram_threshold` 24 MB. Land A2(b) with it so the arena stops ratcheting.
- Blast radius: Vita only. Verify: memlog `ram_free` drops ~96 MB at boot; `heap_arena` after 20
  games far below the new size; 50-game session with no bad_alloc.
- Owner because: a heap-size constant that re-partitions the console's memory (trade-off in §3).

### A2 — HIGH · MEM-EFF/PERF (+MEM-SAFE sub-items) · VERIFIED arithmetic, SUSPECTED impact — Vita card art decoded into a pow2-padded 2 MB RGBA buffer per load, transient on the general heap; the loaders also lack error handling
- Where: `JGE/src/pc/JGfx.cpp:1967` (`getNextPower2`), `:2071-2080` (LoadJPG `tw*th*4`), `:2342`
  (LoadPNG), `:2824` (freed after `glTexImage2D`); `projects/mtg/src/WCachedResource.cpp:161`
  (`size()` accounts the padded size); `include/WResourceManagerImpl.h:16` (`HUGE_CACHE_LIMIT
  48000000`). Sub-items folded in: `JGfx.cpp:2051` (LoadJPG: no `error_exit` override -> libjpeg
  calls `exit()`), `:1984` (`fill_input_buffer` returns TRUE with no bytes -> OOB read on a
  truncated file), `:1990`, `:2063` (`output_components` early return leaks `rawdata`);
  `JFileSystem.cpp:631-632` (ReadFile returns `size` on a short read); `:2306` (LoadPNG longjmp
  handler frees only `rawdata` — `line` and the 2 MB `buffer` leak); `:2080/:2342` (pad rows never
  zeroed -> 1-px fringe under GL_REPEAT+GL_LINEAR, `:2795/:2812`).
- Slices: 8-H2, 8-M1, 8-L2, 8-L3 (all one file); lane S's "what I did not verify" names this
  path as the unmeasured half of the hand-selection stall.
- Mechanism: the shipped Vita pack is 360x514 / 128x186; padding to 512x1024 = 2,097,152 bytes
  decoded AND uploaded AND accounted, so the 48 MB cache holds 24 full cards, not the ~65 the
  notes assume; VRAM (33-61 MB free) holds ~14 before spilling. Deck carousel / editor evict
  constantly; every reload = `new BYTE[~70 KB]` + `new BYTE[2 MB]` + both deleted, interleaved
  with the game's small long-lived objects — the exact shape the PSP campaign proved fragments
  the arena (PSP answered with a slab pool, `JGfx.cpp:103-200`; the Vita has none).
- Magnitude: 2.8x the budgeted per-card size; ~3x the eviction rate; DEVICE: the arena ratchet
  of A1 is the observable. Vita EST: each first-time art = a 2 MB decode + upload on the 444 MHz
  core on the render thread (tens of ms — unmeasured).
- Vita impact: **HEAP + CRASH** (fragmentation feeder; bad_alloc in LoadPNG) + **FRAME** (decode
  stalls on art bursts).
- Fix: (a) `#if defined(VITA) tw = width; th = height;` in LoadJPG/LoadPNG — vitaGL only forces
  POT for the DXT path (`textures.c:877/:1600`); JQuad UVs divide by mTexWidth/Height so nothing
  else changes; verify one card on device first (GL_REPEAT on NPOT is the thing to eyeball).
  (b) two grow-only static decode buffers (raw file, pixels) reused by both loaders and NOT freed —
  Vita is single-threaded on this path (`WResourceManager.cpp:231 UnthreadedCardRetriever`);
  gate to VITA or guard with the existing `sLoadFunctionMutex` (`WResourceManager.cpp:1384`).
  (c) libjpeg `error_exit = longjmp` + a fill_input_buffer that feeds an EOI, free rawdata on every
  return, ReadFile returns `gcount()`; hoist `line`/`buffer` above the PNG setjmp and free both.
  (d) memset the pad column/row (moot on Vita after (a)).
- Blast radius: every texture load on Vita (cards, backdrops, fonts); desktop shares the loaders
  but its `bin/User/sets` is renamed `sets.hidden` (lane V) so desktop cannot SEE art — a loader
  change needs a worktree with sets restored, or an ASAN loader test over sample JPG/PNG/truncated
  files. Verify: memlog `heap_arena` flat across games; "Destroying WCachedResource" count per
  deck-editor page ~3x lower; a truncated JPEG yields a NULL texture, not `exit()`/ASAN overflow.

### A3 — HIGH · PERF · VERIFIED — `AIPlayerBaka::selectAbility` scans abilities x cards, probes each pair twice, and runs the mana planner per pair
- Where: `src/AIPlayerBaka.cpp:2339-2541` (five identical zone rungs), near-copy `doAbility`
  `:2624-2688`; `:2355/:2359` (double `isReactingToClick`), `:2366` (ManaCost built when
  `abilityPayment` is stale — never cleared between pairs, `:2548` comment), `:2379`
  (`getPotentialMana(card)` inside the pair loop).
- Slices: 6-H1 (counters); lane W §2 (measured `AI Act` at -O2 — "the largest single number in
  the profile", not touched there).
- Mechanism: per tick, every non-mana ability in the action layer (avg 123) x every card in
  inPlay+command+hand+graveyard+exile (avg 11.5) is probed, though only the ability's own
  `source` (plus the `isReactingToTargetClick` multi-source cases) can ever react — ~99.9% of
  pairs are dead by construction; failing pairs with a cost then run `canPayMana` = full
  `ManaEngine::planPayment`.
- Magnitude: 737 calls -> 1,047,118 pairs, 2,094,236 probes, 8,307 planner runs, 314 ranked
  actions (0.43/call); **4.3 ms/call at -O0**; lane W at -O2: `Act` 1.68 ms avg / **10 ms per
  decision tick at 24-27 permanents**, 58.5 ms max. Throttled to one call per 0.1 s -> every 3rd
  frame at 30 fps, on the AI's turn only. Vita EST: 100-200 ms per decision tick on a wide board
  = the opponent's turn runs at 5-10 fps (slice 6's own "~150-200 ms" estimate agrees).
- Vita impact: **FRAME** (periodic spike, opponent's turn; grows super-linearly with board width —
  lane W's Update curve 3.4 -> 10 ms from 16 to 27 permanents is almost entirely this).
- Fix: iterate abilities once, take `card = a->source` (collect the multi-source cases by a cheap
  zone-membership test instead of five zone walks); probe once; reset `abilityPayment` per pair;
  lift `getPotentialMana(card)` out of the pair loop (one ManaCost per distinct card); collapse the
  five rungs into a `MTGGameZone*[5]` loop (as c5c did for `FindCardToPlay`); same for `doAbility`.
- Blast radius: every Baka activation; AIPlayerGPT's fallback path; the 46 [AI] fixtures.
- Verify: suite 1215/2 + 46 AI/0; PARSETEST; re-run the pair counter — pairs drop to ~abilities,
  **`ranked` set identical for the same seed** (`WAGIC_SELFPLAY_DECK0=5 DECK1=12`); lane W's -O2
  profile: `Act` at 24-27 permanents 10 ms -> <=2 ms.

### A4 — HIGH · PERF · VERIFIED — the human hand/battlefield availability display recomputes the full oracle on a 0.25 s TIMER, not on change
- Where: `src/GuiHand.cpp:236-275` (`GuiHandSelf::Update`), `src/LegalActions.cpp:623-673`
  (`castableForDisplay`), `:869-885` (`usableAbilityCards`, now batched by lane S), `:477-601`
  (`hasUsableActivatedAbility`), `GuiHand.cpp:263-270` (`ostringstream` signature per tick).
- Slices: 5-H5 (perf 15.1% of suite CPU — suite `dt`=1.0 fires the throttle every tick, so that
  share is the un-throttled cost, not the play cadence), 7-F6 (adds the signature + blocker walk),
  lane S (measured before/after; residual numbers below).
- Mechanism: every 0.25 s while a card is focused, `castableForDisplay` + the battlefield oracles
  + `selectAutoTapProducers` re-run whether or not anything changed; also for the AI seat's
  `GuiHandSelf` object (DuelLayers owns one for the rendered player even when that player is AI).
  If a frame ever exceeds 0.25 s, `dt` makes the refresh fire every frame — a positive feedback
  loop (lane S). `mPreviewSig` guards only `selectAutoTapProducers`.
- Magnitude (lane S, desktop -O0, 19 permanents / 9-card hand): per-tick refresh 1.64 -> **0.44 ms**
  after lane S; per focus change 2.84 -> **1.29 ms**. Vita EST: ~4-9 ms every 250 ms with a card
  focused, ~13-26 ms per hand move — a visible hitch, no longer a freeze. Lane W at -O2:
  `GuiHandSelf` 0.03 ms/frame averaged.
- Vita impact: **FRAME** (periodic 4 Hz spike on a static board; 0 after the fix).
- Fix: change-driven recompute. Extend the existing signature (pool string + hand pointers +
  battlefield pointer/tap state) with phase, stack `count(NOT_RESOLVED)`, `currentPlayer`,
  `mLayers->actionLayer()->mObjects.size()`, and set a dirty flag from `receiveEventPlus` (zone
  change, tap, phase, mana pool, damage) — skip both oracle calls when unchanged; skip entirely
  when `hand->owner->isAI()` and no human is looking; replace the `ostringstream` signature with
  a hash or the dirty flag. Compute the attacker list once per refresh for `canDeclareBlocker`
  (quadratic only inside the declare-blockers step).
- Blast radius: display flags only (`castableNow`, `hasUsableAbilityNow`, `canAttackNow`,
  `canBlockNow`, tap preview). Suite `w53_usable_ability_batch_board.txt` + the eight
  `assertusable` fixtures pin semantics.
- Verify: suite unchanged; perf share of `GuiHandSelf::Update` in the headless suite 15.1% -> the
  event rate (<2%); hand glow / tap preview still update on the same events (manual, desktop —
  no headless surface); the vpk11 "almost frozen" report is the console regression test.
- Note the contradiction resolved in §4.1: lane S's `kMaxOptSources` finding is NOT a cost item
  (0.08 ms) — it goes to the owner as a spec-applicability question.

### A5 — HIGH · PERF · VERIFIED — `CardGui::Render` resolves its texture and HUD quads by string every card, every frame, and does an O(battlefield) scan per card
- Where: `CardGui.cpp:162` -> `WResourceManager.cpp:265-273` (`RetrieveCard`: `filename.reserve
  (4096)` heap alloc per call) -> `MTGCard.cpp:73-85` (`getImageName`: `std::stringstream`) ->
  `WResourceManager.cpp:409` (`RetrieveQuad` takes `resname` by value) -> `WCachedResource.cpp:107`
  (`string resource(resname)` + `map<string,JQuadPtr>` find); same `reserve(4096)` +
  `ostringstream` in `RetrieveCardToken` (`WResourceManager.cpp:328`). `CardGui.cpp:241,269,291,
  292,501,555,565` (`GetQuad("white")` up to 7x/card), `:186,708,720` (`"shadow"`), `:472`
  (`RetrieveTempQuad("morph.jpg")`), `GuiHand.cpp:76`, `GuiPhaseBar.cpp` (`"phasebar"` +
  `"fakebar.png"`), `GuiBackground.cpp:28-41`, `GuiStatic.cpp:214`. `CardGui.cpp:504`
  (`game->currentPlayer->hasPossibleAttackers()` INSIDE the per-card render -> `Player.cpp:316-327`
  battlefield scan, `canAttack` x2 per permanent), `:590` (`hasType("legendary") &&
  hasType("eldrazi")` string lookups per creature per frame), `:404-412` (5 `hasSubtype` string
  lookups per art-less land per frame), `:611-619` (3 string ops for an almost-always-empty
  `starMark`), `:175` (`getCardByName` per frame — now only for copy/clone effects after lane V).
- Slices: 7-F1, 7-F2 (card part; the zone-icon part shipped in lane W), 7-F4; lane W §2
  (the -O2 split); lane V (removed the face-swap re-lookup).
- Mechanism: none of these handles change between frames for a card that is not transforming;
  every one is re-derived through a string temporary + a string-keyed map per card per frame.
- Magnitude (7 COUNT-1, -O0 windowed selfplay): exactly 1 `RetrieveCard`, 4.6 `GetQuad(string)`,
  4.4 `Subtypes::find`, 1 `hasPossibleAttackers` scan per `CardGui::Render`; 48-50 us per card
  render including GL driver time; MB-1: name build 680 ns vs 222 ns with snprintf, managed-quad
  find 109 ns. **Lane W at -O2 bounds the gain: 0.061 ms per card, of which only ~12% is quad
  resolution; the F2/F4 lookups sit inside the other 88% together with the actual draw calls,
  unsplit.** 25-permanent board: ~90 string finds + 625 `canAttack` calls per frame from this path.
- Vita impact: **FRAME**, linear in board width. EST (from -O2 numbers x10-20): the quad-resolve
  part ~0.07-0.15 ms/card -> 2-4 ms/frame at 25 permanents; the F2/F4 lookups + scan another
  ~0.3-1.2 ms/frame; the remaining draw-call share is vitaGL territory (unmeasured, D28/owner).
  Slice 7's "12-25 ms of a 16.7 ms budget in CardGui::Render alone" applies its 10-20x to a
  GL-driver-bound desktop number and is an upper bound, not an estimate.
- Fix: cache the resolved `JQuadPtr` + `alternate` flag on the `CardView`, keyed on
  `(getMTGId(), name != model name, copiedID)` and invalidated by the `WEventCardTransforms /
  CopiedACard / FaceUp` events GuiPlay already receives (re-request only while the pointer is
  NULL); `getImageName` via `snprintf` into a 32-byte stack buffer, drop `reserve(4096)` (both
  sites); resolve "white"/"shadow"/"extracostshadow"/"phasebar"/`kGenericCardThumbnailID` once
  into statics (managed quads are deadbolted and `WCachedTexture::Refresh` repoints them in
  place, exactly like `sAlternateThumbs` at `CardGui.cpp:776-790`) using the existing
  `GetQuad(int)` (`WResourceManager.cpp:378`); hoist `hasPossibleAttackers()` to once per frame
  in `GuiPlay::Render` (or cache on the observer per tap/untap/zone/phase event); `hasType(int)`
  with `Subtypes::TYPE_LEGENDARY` + a static resolved `eldrazi` id.
- Blast radius: render-only (CardGui, GuiStatic library-top, DeckView, WDataSrc, GuiCombat trample
  avatar). Verify: suite unchanged; `WAGIC_AUTODEMO` `wagicArtCalls` (`WResourceManager.cpp:248`)
  ~cards x frames -> ~cards; lane W's -O2 offscreen profile: `CardGui::Render` ms/card 0.061 ->
  target <=0.045, `std::_Rb_tree<string,...>::find` under `DuelLayers::Render` collapses; on the
  console, `avg_rnd` in the memlog frame line at p1perm+p2perm >= 20.

### A6 — HIGH · PERF · VERIFIED (counts, perf shares) — per-tick re-evaluation of every `this(...)` / `while(...)` / `aslongas(...)` / lord condition, plus the per-ability `testDestroy` stack scan
- Where: `include/AllAbilities.h:4800-4829` (`AThis::Update` -> `resolve` unconditionally),
  `:4896-4921` (AThisForEach), `:3643-3705` (`AAsLongAs::Update` calls `updateTargets` TWICE then
  `cards.clear()` so next tick re-adds everything), `src/MTGAbility.cpp:8644-8714`
  (`ListMaintainerAbility::updateTargets`: two `map<MTGCardInstance*,bool>` rebuilt, 9 zones x 2
  players, `canTarget` -> `intToZone` per zone, `TargetChooser.cpp:2058`), `src/ThisDescriptor.cpp:
  758-765` (`ThisVariable::match` -> `NEW WParsedInt` per tick), `:394-399`, `src/MTGAbility.cpp:
  106-1067` (`parseCastRestrictions`: `split`, dozens of `find`s, `createTargetChooser`+delete,
  `card->clone()` for `can play` at `:1030-1041`), `:8012-8032` (`ActivatedAbility::
  isReactingToClick` re-parses `limit` (`NEW WParsedInt`) and `restriction{}` per poll),
  `src/AllAbilities.cpp:8861-8880` (GenericTargetAbility rebuilds its TargetChooser per poll),
  `MTGAbility.cpp:9111-9116`, `:8563-8572` (`Trigger::triggerOnEvent` castRestriction per event),
  `:8044-8053` (planeswalker `isReactingToClick` walks all mObjects with a `dynamic_cast`),
  `include/AllAbilities.h:4695-4717` (`AForeach::foreachMatchCount` full walk per menu text);
  `src/ActionLayer.cpp:149-165` (destroy sweep) + `MTGAbility::testDestroy` (`stackLayer()->has
  (this)` linear over `mObjects`, which keeps RESOLVED entries until the next BEFORE_BEGIN).
- Slices: 4-H3, 4-M11, 4-M2, 5-M6, 5 cross-slice note (`updateTargets` 15.6% + `GenericTriggered
  Ability::Update` 6.5% + `testDestroy` 7.75% + `__dynamic_cast` 12.1% of suite CPU); lane W
  (`ActionLayer::Update` 0.26 ms/frame at -O2 — "walks every live ability object twice").
- Mechanism: the condition machinery is polled, not event-driven; several forms re-parse their
  own script text and allocate parser objects on every tick.
- Magnitude (4-M, one suite run): `updateTargets` **1,191,957** calls, `parseCastRestrictions`
  41,708, vs 13,362 card parses and 4,477 click predicates — ~90x the parser, ~270x the click
  work. Corpus exposure: `this(` 2782 lines, `thisforeach(` 1030, `aslongas(` 1456,
  `this(variable{` 1055, `while(restriction` 124, `lord(` 2301, `foreach(` 1629, `limit:` 284,
  `restriction{` 401. Vita EST (from lane W's 0.26 ms -O2 at 12-22 permanents): **2.6-5 ms per
  frame, constant, every frame**, rising with lords/tokens on the board.
- Vita impact: **FRAME** (the steady per-frame tax, the second largest CPU item after A3 on the
  AI's turn and the largest on the human's).
- Fix: (a) parse once — `AThis` with `restrictionCheck` compiles the restriction to a small
  enum+operand struct at construction; `ThisVariable` keeps one `WParsedInt` and re-evaluates;
  `limit` becomes a member parsed at construction; (b) gate re-evaluation on
  `stuffHappened` / phase change / a zone-change or stack event seen since the last tick (the
  engine already has `mPropertiesChangedSinceLastUpdate`); (c) `AAsLongAs::Update`: drop the
  duplicate `ListMaintainerAbility::Update(dt)` (SorterFunction already calls `updateTargets`);
  keep `cards` as a sorted vector/`unordered_set`, skip zones the chooser cannot target before
  iterating; (d) `ActionStack::has` over a `std::set<MTGAbility*>` of NOT_RESOLVED entries
  maintained at the 4 mutation sites, short-circuit `testDestroy` on `source->currentZone`, run the
  sweep only when a zone-change/stack event occurred; (e) 4-M4's cheap halves — hoist the
  `WParsedPT` at `MTGAbility.cpp:5891` behind an `isdigit/+/-` prefix test; cache the AI
  dry-mode `abilityEfficiency` verdict per `(card model id, mode)` on `MTGCard`.
- Blast radius: every lord/this/foreach/aslongas card and every activated ability with a limit or
  restriction — the suite (1215) is the behavioural guard; note 4-L1's `AAsLongAs::
  findMatchingAmount` only counts the LAST zone (a real correctness bug) — fix it separately so a
  verdict change is attributable.
- Verify: suite 1215/2 + PARSETEST; headless-suite perf: `updateTargets` 15.6% -> <3%,
  `testDestroy` 7.75% -> <1%, `parseCastRestrictions` count 41,708 -> ~parses; lane W -O2:
  `ActionLayer::Update` 0.26 -> <=0.1 ms/frame; console `abil`+`avg_upd` on the frame line.

### A7 — HIGH · MEM-EFF/PERF · VERIFIED — `ManaCost` pointer/copy constructors allocate eight empty sub-costs per copy; through `CardDescriptor`-is-an-`MTGCardInstance` a lord clone becomes a 330 MB transient
- Where: `src/ManaCost.cpp:623-652` (`ManaCost(ManaCost*)`), `:656-685` (`ManaCost(const
  ManaCost&)`), `:770-784` (`hasAnotherCost` true on any copy); `DescriptorTargetChooser::clone`
  -> `MTGCardInstance` copy-ctor (three by-value `ManaCost`s); `ALord::clone ->
  ListMaintainerAbility -> MTGAbility ctor -> CardDescriptor/MTGCardInstance/ManaCost` (slice 6's
  page-fault chain).
- Slices: 5-H4 (mechanism + perf), 6-H3 (the fixture: `generic/gift_of_the_gargantuan_i1085.txt`,
  RSS 129 -> **483 MB**, 85% of page faults under `ALord::clone`, LSan clean at exit = churn not
  leak), 5 cross-slice (`ManaCost(const&)` 3.2% entirely via `DescriptorTargetChooser::clone`),
  6-H2 (the 2,640-byte instance, see A15).
- Mechanism: `ManaCost(ManaCost* NULL)` returns a non-NULL empty object, so `kicker = NEW
  ManaCost(manaCost->kicker)` etc. allocate 9 objects + 9 vectors (~1.6 KB) per copy where
  `copy()` (`:834`) allocates only present sub-costs. Hot users: `refineForOptions`/`payableFrom`
  (<=64 x <=512 combos per human auto-tap plan), `selectAutoTapProducers`, `MTGPutInPlayRule::
  reactToClick` (4 copies), `Spell::resolve` (kept alive on the card), every `MTGCardInstance`
  copy (3 costs) — and every `CardDescriptor`, which derives from `MTGCardInstance`.
  Correctness half: `getAlternative()/getMorph()/...` non-NULL and `hasAnotherCost()==1` after a
  pointer copy — `LegalActionsOracle::payable`'s `cost->getMorph() && canAfford(getMorph())` is
  TRUE for an empty morph (latent: readers get the card's own `copy()`-built cost today).
- Magnitude: `ManaCost::init` 5.0% + ptr ctor 3.8% + copy ctor 3.2% + dtor 3.1% = **~15% of suite
  CPU** is ManaCost churn; 330 MB transient on one fixture.
- Vita impact: **HEAP (peak: a comparable lord/reveal interaction in play = certain OOM on a
  40 MB free heap) + FRAME** (allocation churn on every auto-tap plan and cast).
- Fix: `kicker = manaCost->kicker ? NEW ManaCost(manaCost->kicker) : NULL;` for all eight in both
  ctors (every reader NULL-checks; `copy()` already yields NULLs). Then re-measure the fixture:
  whatever remains is the `MTGCardInstance` copy inside `CardDescriptor` (A15).
- Blast radius: everything that copies a ManaCost; grep `hasAnotherCost` readers (AIPlayerBaka).
- Verify: suite + PARSETEST unchanged; perf `ManaCost::init` share ~halves; the single-fixture
  RSS run (slice 6's method) 483 MB -> target <250 MB; `TRACK_OBJECT_USAGE` counts.

### A8 — HIGH · LEAK · VERIFIED (LSan: 78.9 KB of 82.1 KB, 4,934 of 5,319 leaked allocations) — `GenericAddToGame::resolve` / `GenericAbilityMod::resolve` drop their clone when the click path succeeds; `AProduceMana::produce` ("selectmana") never frees its five wrappers
- Where: `include/AllAbilities.h:3445-3463`, `:3398-3410`; `src/AllAbilities.cpp:9974-10006`
  (`AProduceMana::produce`), `:10001-10005`; `src/ReplacementEffects.cpp:162,174-178` (same
  shape); `src/Counters.cpp:224-230` (the CORRECT idiom, deletes).
- Slices: 4-H0.
- Mechanism: `toAdd = ability->clone(); if (toAdd->reactToTargetClick(source)) return 1;` — the
  clone is neither added nor deleted; a `MayAbility` clone re-clones its nested ability into
  `mClone` (`AllAbilities.cpp:8221-8227`), so a whole `MayAbility`+`AManaProducer` tower is
  orphaned each time. `selectmana` builds five towers per tap and deletes none of the `ga*`.
- Magnitude: the largest leak root in the tree; on the Vita ~10 objects per tap of any
  "add one mana of any colour" producer, for the rest of the match.
- Vita impact: **HEAP** (per-tap leak; part of the 0.2 MB/game live growth).
- Fix: `if (toAdd->reactToTargetClick(source)) { SAFE_DELETE(toAdd); return 1; }` in both
  `resolve()`s (the accepted MayAbility already moved its payload into the game);
  `SAFE_DELETE(ga0..ga4)` after the five `resolve()` calls, exactly as `Counters.cpp:227-230`.
- Blast radius: every `may`/selectmana path. Verify: ASAN+LSan suite — the
  `GenericAddToGame::resolve AllAbilities.h:3447` frame disappears; `Res/test/` selectmana
  fixtures unchanged.

### A9 — HIGH · LEAK · VERIFIED (reading + LSan 142 allocs under `ATokenCreator::resolve`) — `andAbility` never freed by five classes, aliased by their `clone()`s; `TokenAndAbility` cloned per token and never deleted by the card
- Where: `include/AllAbilities.h:4135,4439-4442,4432-4437` (ATokenCreator), `include/MTGAbility.h:
  404` + `src/MTGAbility.cpp:9268-9279` (AManaProducer), `AllAbilities.h:7916,8014-8017`
  (AManifest), `:8024,8071-8074` (AProvoke), `:8080,8122-8125` (AExert); parser fill sites
  `MTGAbility.cpp:3891,3919,3968,4008,4029,4078,5784,4325,4341,4355,4370`; `AllAbilities.h:4330`
  (`spell->source->TokenAndAbility = andAbility->clone()` PER TOKEN), `src/MTGCardInstance.cpp:
  217` (pointer copy), `:473` (`= NULL`), `:324` (dtor does not touch it), `AllAbilities.cpp:6844`
  (`AACloner` aliases it).
- Slices: 4-H2; slice 5 read MTGCardInstance.cpp but did not rule on `TokenAndAbility` (gap, §4).
- Mechanism: 28 of 33 `andAbility` carriers delete + deep-clone; these five do neither, and every
  parse (per cast, per zone change via `MTGRules.cpp:2772`, per AI `effectBadOrGood` dry parse
  `AIPlayerBaka.cpp:2773`, equip/copy/flip/morph) drops the follow-up subtree. Corpus: 586
  `token(...and!(` lines, 26 `add{...} and!(`.
- Vita impact: **HEAP** (per token minted by an `and!` creator, per parse of such a card).
- Fix: `SAFE_DELETE(andAbility)` in the five destructors + `if(andAbility) a->andAbility =
  andAbility->clone();` in the five `clone()`s (the AADrawer idiom, `AllAbilities.cpp:5169-5180`).
  For `TokenAndAbility`: NOT a naive delete in `~MTGCardInstance` (`AACloner` and the instance
  copy alias it) — give the card sole ownership with a deep copy at `:217`/`:6844`, or drop the
  field and re-clone from the creator's `andAbility`.
- Verify: ASAN suite, `ATokenCreator::resolve` and `AACloner::resolve` leak frames gone.

### A10 — HIGH · MEM-SAFE/LEAK · VERIFIED (493 pushes, 2 address hits, list size 203) — `IfThenAbility::clone()` shares `delayedElseAbility`; the address-keyed `deletedpointers` guard that hides the double-delete leaks and grows for the session
- Where: `src/AllAbilities.cpp:8053-8070` (clone), `include/MTGAbility.h:245-253`,
  `src/MTGAbility.cpp:7725-7729` (`std::find` over `deletedpointers` before each delete),
  `src/GameStateDuel.cpp:576` (the only `clear()`).
- Slices: 4-H1.
- Mechanism: clone deep-copies `delayedAbility` but pointer-copies `delayedElseAbility`; the
  destructor's thread_local address list (a) skips any later delete whose address was seen before
  (malloc reuse -> silent leak of whole subtrees), (b) grows for the whole `GameStateDuel` — the
  Vita session — with an O(n) scan per `~IfThenAbility` (6,111 `if ... then` lines; every
  trigger clone churns one).
- Vita impact: **HEAP** (unbounded list + address-collision leaks) + **CRASH class** (a masked
  double-owner; any change to the else-branch lifetime turns it live).
- Fix: `a->delayedElseAbility = delayedElseAbility ? delayedElseAbility->clone() : NULL;` in
  `clone()`; plain `SAFE_DELETE` of both in the dtor; delete `deletedpointers` (header, two static
  definitions, the GameStateDuel clear).
- Verify: suite + PARSETEST, then ASAN suite (a double-free is reported at once); grep no
  remaining `deletedpointers` user.

### A11 — HIGH · MEM-SAFE · VERIFIED (reading; one-line invariant) — `garbageLastTurn` aliases `garbage`: dead tokens are freed ONE turn after they die, the root of the dead-token dangle class
- Where: `src/MTGGameZones.cpp:189-193` (`SAFE_DELETE(garbageLastTurn); garbageLastTurn = garbage
  = NEW MTGGameZone();`), `:151-172` (dtor consistently does not delete `garbageLastTurn`),
  `:394` (init).
- Slices: 5-H1.
- Mechanism: both pointers always name the same zone, so the `SAFE_DELETE` frees the tokens that
  died in the turn just ended (with their `previous` chains). Every raw pointer that outlives a
  token across one turn boundary (aura `target`, `imprintedCards`, AI-remembered pointers,
  `storedCard`, `myPair`, LKI reads) dangles at the next BEFORE_BEGIN. The engine's own comments
  name the class (`validateCardPointer`: cores 3266478, 3151670) and `validateCardPointer` scans
  the two zones as if distinct.
- Vita impact: **CRASH** (the token UAF family; slice 8's Aug-28 `_malloc_r` corruption analysis
  says a stale game-layer pointer lands in recycled memory within frames on the Vita heap).
- Fix: `SAFE_DELETE(garbageLastTurn); garbageLastTurn = garbage; garbage = NEW MTGGameZone();
  garbage->setOwner(owner);` and `if (garbageLastTurn != garbage) SAFE_DELETE(garbageLastTurn);`
  in the dtor. Memory cost: one extra turn of dead tokens.
- Blast radius: MTGTokensCleanup, `validateCardPointer`, `AIPlayerGPT.cpp:11261` (reads
  `garbageLastTurn`). Verify: suite; a fixture that kills a token, advances two turns, reads a
  stored pointer; ASAN selfplay corpus — dead-token UAF signatures disappear.

### A12 — HIGH · LEAK · VERIFIED (LSan on SnowCost; the other 13 sites by reading) — `NEW ManaCost(ManaCost::parseManaCost("{0}", ...))` leaks the parsed temporary at every call; `SnowCost`/`Offering` parse-and-drop on every payment probe
- Where: `src/ManaCost.cpp:1110` (`canAfford` anytypeofmana — also leaks the first `diff`),
  `src/ManaEngine.cpp:679` (`planPayment`), `src/MTGRules.cpp:367` (sunburst, leaks TWO —
  `changeCostTo` copies), `:1988,:2010` (PayZero is/reactToClick), `:2185,:2207` (attack cost),
  `:2265,:2287` (block cost), `src/AIPlayerBaka.cpp:2671,2678,2721,2729` (lane E region);
  `src/ExtraCost.cpp:160-166` (7x `parseManaCost("{g}"...)` per `SnowCost::isPaymentSet`),
  `:194-227` (`doPay`), `:1570-1578` (`Offering::canPay`: 3 leaks per call).
- Slices: 5-H3, 4-M1, LSan roots in 4-M, 5-M2, 6 (top root in all three runs: `SnowCost::
  isPaymentSet ExtraCost.cpp:162`, `doPay :204`).
- Mechanism: `parseManaCost(s, NULL, c)` returns a `NEW ManaCost` (`ManaCost.cpp:185-196`); the
  outer copy never frees it. Each leak is ~1.6 KB via A7's eight empties. Attack/block-cost and
  PayZero rules run `isReactingToClick` on every click walk during combat; `planPayment`
  anytypeofmana runs per castability probe (A4's cadence).
- Vita impact: **HEAP** (per click / per probe with such cards on the board).
- Fix: `NEW ManaCost()` at every `"{0}"` site (identical semantics: `{0}` adds nothing, `xColor`
  -1 either way); at `MTGRules.cpp:367` a stack `ManaCost empty; ...->changeCostTo(&empty)`; free
  the first `diff` in `canAfford`; hoist the seven snow costs into `static const` members;
  reuse+delete `diff` in `Offering::canPay`.
- Verify: LSan on an attack-cost + PayZero fixture and the snow fixtures — the `parseManaCost`
  frames vanish; suite/PARSETEST unchanged.

### A13 — HIGH · LEAK · VERIFIED (reading; suite never loads a commander deck) — commander deck loading leaks one `MTGCardInstance` per deck card per commander
- Where: `src/MTGGameZones.cpp:84-107` (`MTGPlayerCards::initDeck`, the `CommandZone.size()`
  branch: `NEW MTGCardInstance(cmdcard, this)` per non-basic card per commander, only to call
  `hasColor()` / `magicText.find()`, never deleted).
- Slices: 5-H2.
- Magnitude: 99 x ~2.8 KB = ~280 KB per player per game; **~560 KB per commander game** on the
  Vita heap; 1,200 commander games in one process ~670 MB.
- Vita impact: **HEAP** (commander/random-commander modes only).
- Fix: read colour identity off `cmdcard->data` (`CardPrimitive::hasColor`, `magicText`) — no
  instance needed; or one instance per CommandZone entry hoisted out of the loop and deleted.
- Verify: ASAN on a commander game (`WAGIC_REPLAY` of a commander transcript or a commander
  selfplay pairing) — the `initDeck` leak stack gone.

### A14 — HIGH (latent) · MEM-SAFE · VERIFIED by reading, 0 hits in the suite — cloning a triggered ability crashes: `GenericTriggeredAbility::clone()` calls `destroyCondition->clone()` on NULL, and every `Tr*::clone()` shallow-copies owned TargetChoosers/Counters
- Where: `src/MTGAbility.cpp:9074-9081` (`dc = NULL` always, `:2655`); `include/AllAbilities.h:
  255-266` (TrCardAddedToZone: 4 owned TCs), `:581-589` (TrCombatTrigger), `:1510-1518`
  (TrDamaged), `:1563-1571` (TrLifeGained), `:1605-1613` (TrVampired), `:1642-1650` (TrTargeted),
  `:1697-1702` (TrCounter: `NEW Counter(*counter)` with `counter` legitimately NULL for
  `counteradded((any))`), `:1801-1806` (TrTotalCounter).
- Slices: 4-H4.
- Mechanism: dormant only because the parser splits `@...:` before `lord(`/`all(`/`&&` and
  `transforms(newability[@...])` re-parses text per target; the clone paths that DO exist
  (`ALord/ATeach/AForeach::_added`, `MayAbility`, `MultiAbility`, `activateSideEffect` `limit^...^`,
  `AAsLongAs`, `APaired`, `AGrant`) crash the moment a trigger reaches them (e.g.
  `{T}:draw:1 limit^@each my upkeep:...^3`, or any parser reordering).
- Vita impact: **CRASH class**, latent; a trivial fix that every new clone path would otherwise arm.
- Fix: `a->destroyCondition = destroyCondition ? destroyCondition->clone() : NULL;`; real
  deep-copying `clone()`s for the Tr* with owned pointers (`toTcZone/toTcCard/fromTc*`,
  `gainException`, `counterException`, `proliferateException`, `counter ? NEW Counter(*counter)
  : NULL`).
- Verify: a synthetic fixture whose `limit^...^` side effect is a trigger, under ASAN.

### A15 — HIGH (project) · MEM-EFF · VERIFIED sizes, SUSPECTED Vita totals — every zone move allocates a 2,640-byte `MTGCardInstance` carrying a private copy of its primitive, and hand/library/exile cards keep their whole `previous` chain until game end
- Where: `src/CardPrimitive.cpp:90-140` (copy ctor deep-copies `text`, `formattedText`,
  `name`+`lcname` (same bytes twice), `magicText`, the `magicTexts` map (looked up twice per entry,
  `:136-137`), `types`, clones `restrictions`), `include/CardPrimitive.h:53-76`;
  `src/MTGGameZones.cpp:960-1025` (`removeCard` clones on every move), `src/MTGCardInstance.cpp:
  850-863` (`cleanup()` frees `previous` when not in use), `src/Player.cpp:432-436`
  (`cleanupPhase` -> inPlay + graveyard ONLY).
- Slices: 6-H2 (sizes: `MTGCardInstance` 2640, `CardPrimitive` 640, `ManaCost` 176, `string` 32 —
  24 on 32-bit), 5-M1 (chains), 5 measurements item 3 (gdb sizes agree), TestSuiteAI.cpp:959's
  own note ("every zone move makes a new object").
- Mechanism: (a) a bounce / Brainstorm / shuffle / exile chain keeps every prior instance alive
  until `~MTGGameZone` (peak heap, not a leak — LSan agrees); (b) each instance duplicates
  ~97-200 B magicText + ~156 B text + names + map nodes on top of 2,640 B, none of which changes
  for an ordinary instance; on the Vita `materializeMagicText` (`:505-542`) makes `magicText`
  resident and the copy ctor duplicates it into every instance anyway.
- Magnitude: a few hundred instances per game x ~3 KB = ~1 MB peak per game from (b); (a) adds
  the un-reaped chains on library/hand-cycling decks. EST, not measured on device.
- Vita impact: **HEAP** (peak, and the small-object churn that fragments the arena next to A2's
  2 MB blocks).
- Fix: (a) cheap — call `cleanupPhase()` on hand, library, exile, commandzone, reveal in
  `Player::cleanupPhase` (`stillInUse()` already guards action-layer references; readers of
  `->previous->previous` for non-battlefield cards are `WParsedInt "sunburst"`, `MTGPersistRule`/
  `MTGUnearthRule` inside the zone-change event — before any cleanup). (b) project — `const
  CardPrimitive * base` on the instance, immutable fields read through accessors
  (`getName/getLCName/getFormattedText/magicText`), copy-on-write for `setName` (flip),
  `addMagicText` (grants), `removeType`; at minimum stop copying `text/formattedText/nameOrig`
  and share `magicTexts` by pointer. Do it behind accessors first — blast radius is every direct
  `card->name`/`magicText` reader.
- Verify: (a) `TRACK_OBJECT_USAGE` `MTGCardInstance` count at end of each turn over a 30-turn
  selfplay, before/after; (b) suite + PARSETEST, then the Vita heap gauge (`WAGIC_VITAMEMLOG`
  `heap_used` at untap).

---

## 1b. Ledger — MED

### A16 — MED · CRUD/MEM-EFF · VERIFIED (nm/size + compile timing) · **OWNER DECISION** — `runParseSelfTest` (12.2k lines, ~1.06 MB text + 339 KB literals) compiles into every target including the three that can never run it
- Where: `src/AIPlayerGPT.cpp:24917-37134`, entry `:37138`; sole caller `JGE/src/SDLmain.cpp:1027`
  (env `WAGIC_GPT_PARSETEST`); Vitamain/PSP/Android never call it; the Vita CMake compiles the
  TU ungated. Slices: 3-2.
- Magnitude: `AIPlayerGPT.o` .text 2,840,904 -> 1,392,364 bytes stubbed (-51% with rodata); TU
  compile 152 s -> 59 s wall. ~1.4 MB (x86 numbers; ARM Thumb denser, literals identical) of
  dead code+rodata resident on the handhelds.
- Vita impact: **HEAP**-adjacent (resident code pages in the 33 MB self; not on the newlib heap).
- Fix: wrap `:24905-37134` in `#if defined(WAGIC_GPT_PARSETEST_BUILD)` with an `#else` stub, add
  the define next to `WITH_GPT_AI` in `wagic-SDL.pro:20`; or `#if !defined(VITA) && !defined(PSP)
  && !defined(ANDROID)`. Longer-term: own TU `AIPlayerGPTSelfTest.cpp` (needs a friend/header seam
  to the static parsers). Verify: desktop PARSETEST still 2249/0; `nm -S` on the Vita ELF shows
  the symbol gone. Owner because: a build-flag change to the release handheld builds.

### A17 — MED · MEM-EFF · VERIFIED — `mAskCache` is never cleared inside a game; keys are whole situation+option blocks
- Where: `include/AIPlayerGPT.h:330`; writes `src/AIPlayerGPT.cpp:17578`, reads `:17399`,
  key built `:17392-17401`; no `clear()`/`erase()` anywhere. Slices: 1-F4, 2-F1.
- Mechanism: each answered ask stores a 1.4-7.8 KB key (10-17 KB prompts; one 116 KB priority
  window cited at `:16483`) for the life of the seat; the key embeds phase+board so it can never
  hit once the board moves. Purpose (re-polls of the same state, earlier picks of a multi-target
  selection) is within one turn.
- Magnitude (1's corpus, 40 recent games): median 99 KB, max 1.59 MB (85-turn game) per seat-game.
- Vita impact: **HEAP** when the GPT seat is used on the console (4% of free heap worst case).
- Fix: `mAskCache.clear()` at the turn boundary next to `mPassDeclineCount` (`:16375` /
  `:17903`); optionally key by `std::hash<string>` (collision = replaying an already-parsed index
  on a byte-identical question). Verify: PARSETEST 2249/0; a live translog still shows
  `latency_ms -1` replay records on a same-turn re-poll; corpus replay identical choices.

### A18 — MED · LEAK-per-game/MEM-EFF · VERIFIED (trace + corpus) — `mNarrationPending` grows for the whole game whenever the translog is off (the shipped default = the Vita)
- Where: `src/AIPlayerGPT.cpp:9483-9487` (`writeTransLog` early return on `mTransLogPath.empty()`),
  feeders `:10246` (`writeNarration` -> `narrationAppend(..., &mNarrationPending)`), `:9882`,
  `:10845`; consumers `:9579-9583`, `:9463` both path-gated; the 24 KB trim at `:10103`
  deliberately skips it (W42-D8); member `include/AIPlayerGPT.h:665`; `GptConfig.cpp:37`
  (`translog=0` default). Slices: 1-F5 (LOW, 8 KB median / 41 KB max on the 40 most recent games),
  3-1 (MED, 29 KB median / **192 KB max** on the 40 largest seat logs). Ranked MED on 3's numbers
  and the one-line fix.
- Vita impact: **HEAP** (GPT seat on the console; +transient 2x on `+=` reallocation).
- Fix: `mNarrationPending.clear();` in the early return (mirrors `mLastParseNote.clear()`), and
  pass `NULL` as the delta pointer from `writeNarration` when the log is off so the copy is never
  made. Verify: suite + PARSETEST; a headless `WAGIC_GPT_TRANSLOG=0` game with a temporary size
  probe (removed before commit) shows the member flat at 0; translog-on records byte-identical.

### A19 — MED · PERF · VERIFIED counts — `serializeGameState` (the heavy situation render) runs 2-4 times per decision
- Where: cast: `:17912` (`boardNow`) + `askModel :17392` + `assemblePrompt :9931` -> 3, and
  `:17398` re-renders a 4th time when `reasked`; priority: `:16766` (boardKey) + `:9931` -> 2
  (+`:16643` per repeat-plan tick); target/menu/cost asks 2 per round. Its tail (`:12011-12316`)
  runs `potentialColorReach`, `selfDamageManaSources`, `legalLandPlays`, two `describeZoneCards`,
  `converterSituationLine`, `drawPunisherSituationLine`, two `drawStepExtrasScan`s each time.
  Slices: 1-F3 (perf: 9.8% inclusive of a fake-endpoint GPT selfplay — the largest single
  GPT-seat cost), 2-F4. Both correct the header comment (`AIPlayerGPT.h:258-286`): seams are NOT
  re-entered per tick — `AIPlayerBaka::Act` returns at `decisionPending` (`AIPlayerBaka.cpp:5034`).
- Vita impact: **FRAME** on the GPT seat's decision frames (per decision, not per tick).
- Fix: trivial half — `:17398` becomes `askKey = reasked ? askKey0 + "\n" + mAskReaskLine :
  askKey0`. Main — memoise the base render per engine tick (`mBoardRenderCache` keyed on a
  monotonic counter) and have `assemblePrompt` take the already-built situation string
  (overload `assemblePrompt(tail, const string* situation)`); the `&tail` variant differs only by
  the own-battlefield `effectSkip` line (`:12219-12229`) — render that line separately and splice.
- Blast radius: every seam; the ask key must keep exact bytes. Verify: `WAGIC_REPLAY` of a corpus
  record — prompts byte-identical; translog `latency_ms -1` behaviour unchanged; PARSETEST.

### A20 — MED · PERF · VERIFIED (4.6% perf) / SUSPECTED at scale — the battlefield line is O(N^2): `describeAttachments` with a string-keyed `findType` per pair, `instanceHandle` rescanning the battlefield per entry
- Where: `src/AIPlayerGPT.cpp:2078-2105` (`describeAttachments`: per host, both battlefields,
  `att->hasType(TYPE_EQUIPMENT) || att->hasType("fortification")` -> `MTGAllCards::findType
  (string)` per pair), called from `describeZoneCards :4882` per entry; `:2123-2148`
  (`instanceHandle`, 42 call sites; per entry `:4754`, per attachment `:2099`, per host `:4902`,
  per blocked attacker `:4910`). Slices: 1-F1, 1-F2; engine `findType` 23.6% inclusive game-wide
  in that profile (slice 5 sees the same string-keyed type map as the top self-time symbols).
- Magnitude: 4.57% inclusive on a 17-turn small-board game; the corpus boards run to 322 and
  1,539 permanents (header `:552-555`, `:4226-4231`) — 10^5-10^6 string compares per render.
- Vita impact: **FRAME** on GPT decisions with wide boards.
- Fix: one pass per `describeZoneCards`: `map<MTGCardInstance*, vector<MTGCardInstance*>>`
  host->attachments (aura `auraParent`; equipment/fortification `target`), fortification id
  resolved once (`static int kFort = findType("fortification")`, then `hasType(int)`); a per-render
  `HandleIndex { map<string,int> total; map<MTGCardInstance*,int> rank; }` for handles. Same
  strings, battlefield order preserved.
- Verify: PARSETEST (`{attached:` and `#N` pins, `joinZoneEntries` x-range cases); byte-identical
  prompts on a corpus replay; perf share of `describeAttachments` <0.5%.

### A21 — MED · PERF · VERIFIED duplicate calls, SUSPECTED cost — `describeAction` runs the mana planner and the colour-reach scan twice per option row; board-wide scans sit inside per-row loops
- Where: `src/AIPlayerGPT.cpp:14316-14320` and `:14350-14358` (`selectAutoTapProducers` twice,
  identical args), `:14354` + `:14415` (`potentialColorReach` board-wide per row; `FindCardToPlay`
  already hoists it at `:17911`); per-row board scans `drawPunisherScan` `:14469,18464,18481,
  19786`, `castTriggerDrawScan`/`converterScan` `:18476,18499`, `exileCastNote` walk `:13891-13907`;
  duplicate auto-tap scans `:14319-14338`/`:14357-14372` vs `:18056-18081`. Slices: 2-F2, 2-F9b
  clusters 7-8. Related memo: A23.
- Magnitude: `chooseOrderedAction` describes EVERY candidate before de-dup (`:16397`); the 432-row
  Staff-of-Nin window (`:16483`) = ~860 planner runs + ~430 reach scans per render. `planPayment`
  was 54% of a display refresh on the vpk11 board (`ManaEngine.cpp:386-392` note).
- Vita impact: **FRAME** on GPT priority decisions with wide option lists.
- Fix: compute `picks` once per row and reuse for both clauses; pass `untappedSources` (or a
  per-window member) so `potentialColorReach` runs once per render; hoist the three scans into a
  small per-window struct computed once per `chooseOrderedAction`/`FindCardToPlay`.
  Output must stay byte-identical — the render is the ask key.
- Verify: translog `options_text` byte-diff on a fixed-seed replay; PARSETEST.

### A22 — MED · PERF · VERIFIED structure, SUSPECTED magnitude — the combat prompt is O(attackers x blockers x permanents) with per-pair battlefield walks and script re-parses
- Where: `combatStatOf :21419-21441` -> `blockTriggeredLifeFor :20900-20936` (battlefield walk +
  per-card `magicText` line parse + a factory `TargetChooser` per matching line, computed for the
  ATTACKER side where its result is never read, `:21436-21439`); `combatBlockOutcome :21497-21505`
  / `combatAttackOutcome :21511-21519` (each 2x `combatStatOf`, 2x `combatPreventionKind`,
  `playerHasLifeToDamageConverter` battlefield walk); `chooseAttackers :22686-22733` (two extra
  prevention probes `:22707/:22726`, a third `combatStatOf :22723`); `chooseBlockers :23646-23701`
  (`combatStatOf(blockers[i])` per attacker k); `playerHasLifeLoop` per row `:22800,23551,22898`;
  `converterSituationLine :22890`. Slices: 2-F3.
- Magnitude: A=20, B=20, P=40 -> ~1,200 outcome calls x (2 walks + 2 parses + probes); the corpus
  had 22-row blocker menus and 1,539-creature boards.
- Vita impact: **FRAME** on GPT combat decisions with token boards.
- Fix: per window, `map<MTGCardInstance*, CombatTradeStat>` for every attacker and blocker
  (`blockTriggeredLifeFor` for the blocker side only), `bool oppConverter/oppLoop` once; a
  `combatBlockOutcome/AttackOutcome` variant taking precomputed stats
  (`combatTradePreviewStats` already takes the two structs); reuse the outcome's
  `combatPreventionKind` in `chooseAttackers`.
- Verify: translog `options_text` byte-diff on a fixed-seed replay; PARSETEST 2249/0 (the
  struct-only `combatTradePreviewStats` cases are untouched).

### A23 — MED · PERF · SUSPECTED — the "pure over magicText" script scanners lowercase and line-split the script on every call, per row, per render
- Where: `drawPunisherClause :6938`, `discardPunisherClause :7257`, `castTriggerDrawCount :7085`,
  `opponentExtraDrawPerTurn :7203` (`AutoLineMacro::Process` per `_` line), `scriptAbilityDrawCount
  :7568`, `scriptSelfDrawCount :7621`, `amassCountersFromScript :569`, `annihilatorTag :2221`,
  `counterAddPlusFromScript :692`, `drawStepExtrasScan :7461`, `lifeToDamageConverterScript`/
  `lifeLossMirrorScript :11878-11880`; `dynamicMagnitudes :1474` builds an `AbilityFactory` +
  `TargetChooserFactory` chooser per row (`riderHasLegalTarget :771`); `drawPunisherScan` has 8
  call sites. Slices: 1-F6 (0.7% on a matchup with no punishers; decks 125/130/162 hit every path
  per row per render).
- Vita impact: **FRAME** on GPT renders with the punisher/converter decks.
- Fix: one `ScriptFacts` struct per distinct script memoised in a file-local `map<string,
  ScriptFacts>` (or computed once on `CardPrimitive` at load): drawPunisher {per, conditional},
  discardPunisher, castTriggerDraw, extraDrawPerTurn, selfDraw, amassN, annihilatorN,
  counterAddPlus, converter/mirror flags, lowercased text. <100 structs per game on the Vita.
- Verify: every scanner already has PARSETEST pins — route them through the memoised front door.

### A24 — MED · "does it do the job" · VERIFIED by reading — the libcurl HTTP path swallows the status code: 401/404/413/429/5xx are logged nowhere and classified as `empty_reply`
- Where: `src/GptConfig.cpp:644-654` (`httpRequestImpl`: `if (res != CURLE_OK || httpCode != 200)
  return "";`), consumed `src/AIPlayerGPT.cpp:8245` -> `:8286` (`timedOut`) -> `:8510`
  (`empty_reply`); the Codex branch logs `HTTP <code>` once per distinct cause (`:8232-8240`), the
  JNI path logs `gptLastError`; libcurl has no log line at all. Also `GptConfig.cpp:625-642,
  701-716`: `CURLOPT_FOLLOWLOCATION` on a POST without `POSTREDIR` (a 30x becomes a bodiless GET =
  the same empty class), no `CURLOPT_MAXREDIRS`. Slices: 3-3, 3-9.
- Mechanism: a wrong key, a rejected model id, a prompt over the server's context, a rate limit
  — all indistinguishable from an unreachable server; a whole match plays on the heuristic with
  `fallback: empty_reply` on every record. #W53-Q (D10) just spent a wave separating "timeout"
  from "empty_reply" one layer up.
- Vita impact: NONE for frame/heap; the GPT seat's diagnosability everywhere including the console.
- Fix: return the code out of `httpRequestImpl` (optional `long* codeOut`, as `httpRequestFull`
  does); in `WorkerMain` log once per distinct `(url, code, first 160 body bytes)` under the
  existing `logMtx` pattern; stamp the code on `AsyncState` so the record carries `http_status`
  and `noAnswerClassFor` names `http_error`; `CURLOPT_MAXREDIRS 3` + `CURLOPT_POSTREDIR
  CURL_REDIR_POST_ALL` (or no FOLLOWLOCATION on POST); prefer `CURLOPT_XOAUTH2_BEARER` so libcurl
  scopes the key to the origin by construction.
- Verify: point `WAGIC_GPT_URL` at a bad-key server -> gpt-log names the 401, the record says
  `http_error`; PARSETEST `noAnswerClassFor` table rows added; a local 302 stub reaches the model.

### A25 — MED · PERF · VERIFIED (1.1 `Replace()` per event; 5.1 us/call -O0) — `GuiPlay::receiveEventPlus` re-lays out the whole battlefield for EVERY game event, often twice
- Where: `GuiPlay.cpp:456` (unconditional trailing `Replace()`), `:390` (zone-change path already
  calls it), attackers/blockers/type/unattach/equip/controller branches call it AND fall through;
  `Replace()` `:200-297` = `stable_partition` (temp alloc) + four full passes with
  `hasSubtype/hasType/isCreature/isAttacker` per card. Slices: 7-F3 parts (1)(2); part (3)
  (`RenderSpell`) is D28 — owner (§3).
- Magnitude: 4.2/frame in fast-clock selfplay, 21-36 us/frame at -O0; matters in event storms
  (untap of a big board, multi-symbol mana payments). Vita EST 0.05-0.1 ms per call.
- Vita impact: **FRAME** (event-storm frames).
- Fix: delete the trailing `Replace()`; set `mLayoutDirty` from the layout-changing handlers and
  relayout at most once at the top of `GuiPlay::Update` (events are queued in
  `GameObserver::receiveEvent`); keep the synchronous call before `CardSelector::Add`
  (`:386-389`).
- Verify: suite (layout is display-only); offscreen selfplay perf — `GuiPlay::Replace` and
  `std::stable_partition` leave the profile; human Vita play on a 20+ board.

### A26 — MED · PERF · VERIFIED share (2.2%) — `WParsedInt` re-parses its expression on every evaluation, builds a `TargetChooser` per `type:` evaluation, and its keyword-erase calls are wrong
- Where: `src/WParsedInt.cpp:48-386` (`init`: ~15 `find`s + a ~120-way `else if` chain),
  `:298-362` (`type:` -> factory chooser, destroyed each time), `:390-473`, `:825-836`;
  `:105-176` (`s.erase(pos, pos + len)` — count should be `len`; harmless only because the
  keywords are trailing). Slices: 5-M3; feeds A6 (`variable{}` per tick).
- Vita impact: **FRAME** (part of the per-tick tax).
- Fix: `s.erase(pos, len)` at the 12 sites; a compiled `WParsedIntExpr {kind, tc, constant}` with
  `eval(card, spell)` cached per ability instance — convert the per-tick creators first.
- Verify: PARSETEST + suite; perf share of `WParsedInt::init`.

### A27 — MED · MEM-SAFE (save/undo/replay correctness) · VERIFIED reading — `MTGPlayerCards::operator<<` serialises the HAND under `exile=`
- Where: `src/MTGGameZones.cpp:1917-1921` (`if(z.removedFromGame->cards.size()) { out << "exile=";
  out << *(z.hand) ... }`). Slices: 5-M2.
- Mechanism: undo, the post-pre-game transcript baseline, network `synchronize`, `dumpAssert` all
  go through this; `load()` rebuilds exile from it — hand duplicated into exile, real exiles lost.
  A Serum Powder redraw is exactly the moment the baseline is taken. Possible mechanism behind
  lane W's two vpk12 transcripts refusing to replay (one aborts in `dumpAssert` at load) — unproven.
- Vita impact: NONE directly; replay/transcript fidelity for every device report.
- Fix: `out << *(z.removedFromGame)`. Verify: an undo fixture with `exile=` non-empty in [INIT];
  `WAGIC_REPLAY` of a Serum Powder game; retry the two vpk12 transcripts.

### A28 — MED · MEM-SAFE · VERIFIED reading — parse-failure NULLs dereferenced, and a keyword that mutates game state during PARSE
- Where: `src/MTGAbility.cpp:2734-2744` (`tc->targetter` unchecked; guarded twin at `:5808`),
  `:5147`, `:3741-3742` (`grant` with empty stash -> `AGrantWrapper(NULL)`), `src/AllAbilities.cpp:
  1453`, `:4089,4143`, `:1307,1346,1376`, `:4532,4550,4562`, `:10211`, `include/AllAbilities.h:8261`;
  `src/MTGAbility.cpp:4783-4808` (`altermutationcounter:` mutates the card and fires an event
  during parse — an AI dry-run parse changes game state; `mutationover/under:` read
  `splitMutated[1]` of a different, empty vector — UB the day a card uses them). Slices: 4-M10.
- Vita impact: **CRASH** on a malformed or new-keyword script instead of the loud reject.
- Fix: `if (!x) return NULL;` at each site (the validator already routes NULLs); move the
  mutation side effect into an ability's `resolve()`; fix the two vector names.
- Verify: PARSETEST + `WAGIC_VALIDATE` pass.

### A29 — MED · MEM-SAFE · VERIFIED reading — fixed-size arrays without bounds checks in the target-chooser parser and two abilities
- Where: `src/TargetChooser.cpp:16` (`int zones[10]` local; `nonbattlezone` alone pushes 10, the
  member is `zones[15]` at `include/TargetChooser.h:112` — `|nonbattlezone,mystack` overflows the
  local), `:1864-1868` (`types[nbtypes++]` into `types[10]`, an 11-name `target(...)` overflows),
  `include/AllAbilities.h:6593,7547` (`opponents[20]` unbounded from `getNextOpponent`), `:5665`
  (`char land[20]`, safe today). Slices: 4-M5.
- Vita impact: **CRASH** (stack/heap smash on a card script that exceeds the literal).
- Fix: `int zones[15]` + guards; `if (nbtypes < 10)` with a DebugTrace; `opponents` -> vector.
  Verify: PARSETEST + suite (no behaviour change).

### A30 — MED · MEM-SAFE/PERF · VERIFIED reading — `APowerToughnessModifier` (nonstatic) rebuilds its `WParsedPT` every tick and formats `triggers` into `char buffer[4]`
- Where: `include/AllAbilities.h:3086-3107` (Update: delete + `NEW WParsedPT` per tick), `:3117-3138`
  (second copy same tick for cda), `:3146-3167`, `:3211-3232` (re-parses on every menu-text
  request); `:3089/3120/3149/3214` `sprintf(buffer, "%i", triggers)` — 5 bytes at 1000
  activations. 50 `nonstatic` lines. Slices: 4-M3.
- Vita impact: **FRAME** (small) + **CRASH** (one keystroke from a stack smash).
- Fix: keep the parsed `WParsedPT`, re-evaluate `getValue()`; one helper for the four blocks;
  `snprintf` into 16 bytes. Verify: `Res/test/` nonstatic fixtures + suite.

### A31 — MED · LEAK · VERIFIED reading (LSan: `CardDescriptor` 4 objs, `GenericPaidAbility::resolve` 4 objs) — small per-parse leaks in the trigger/castcard parsers, `AEquip::unequip`, and `MTGCardInstance`'s protection choosers
- Where: `include/AllAbilities.h:1529/1563-1566` (`TrLifeGained::gainException`), `:1662/1692-1695`
  (`TrCounter::counterException`), `:1717/1796-1799` (TrTotalCounter), `:789`
  (`TrplayerProliferated::proliferateException`, no dtor), `src/MTGAbility.cpp:4492,4502`
  (`NEW WParsedInt` for `kicked!:`/`costx!:`, 151 lines in borderline.txt), `:3882/3999`
  (`multiplier` on the error return); `src/AllAbilities.cpp:11245-11249` + `:11180-11191`
  (`AACopier`-core ability marked `forceDestroy` but never added, then `removeObserver` no-op —
  leaks per attach cycle); `AllAbilities.cpp:4567` (`nomenuAbility` on the menu path);
  `src/MTGCardInstance.cpp:378-388` (protections/canttarget/cantBeBlocked choosers never freed;
  `:2181-2193` `erase=1` branch would delete an unowned instance — no caller). Slices: 4-M6,
  4-L3, 5-L4, 4-M LSan table.
- Vita impact: **HEAP** (small, per parse/attach — parses are per cast/zone-change/AI evaluation).
- Fix: the four `SAFE_DELETE`s + deep copies in `clone()` (with A14); stack `WParsedInt`; add
  `AACopier` to the unequip delete list mirroring `AANewTarget`; remove the dead `erase` branch.
  Verify: LSan run.

### A32 — MED (project) · PERF · VERIFIED counts — card text is re-parsed from scratch at every zone change and every AI evaluation; nothing is cached per card model
- Where: `src/MTGAbility.cpp:6686-6858` (`getAbilities`: copy `magicText`, `AutoLineMacro::
  Process` whole text, `parseMagicLine` per line — hundreds of `find`s + a `WParsedPT` at `:5891`
  even for non-P/T lines); callers `src/MTGRules.cpp:2772` (every zone change),
  `src/AIPlayerBaka.cpp:2773` (`effectBadOrGood` dry parse per card per decision),
  `AllAbilities.cpp:2879,5299,5526,5948,11226,11267,9223,10209` (+ `APhaseAction`'s ctor parse
  with `card=NULL`, rejected at `:2108` = dead work). Slices: 4-M4; 6 (`getCardByName`, A36, is the
  load-time cousin).
- Magnitude: 13,362 `getAbilities` / 252,162 `parseMagicLine` per suite run (~19 line parses per
  card parse, ~11 card parses per test).
- Vita impact: **FRAME** (zone-change storms; AI evaluation ticks).
- Fix: the two cheap halves are in A6(e); the larger per-model parsed-line cache keyed
  `(magicText line, zone)` interacts with the stateful `stored*` stash strings in
  `AbilityFactory` — a project, not a patch. Carry as a next-wave lane.

### A33 — MED · CRUD · VERIFIED (grep) — diagnostics shipping in release builds (owner rule: compile out; gate `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`)
- Where: `src/MTGAbility.cpp:2449-2453` (`getenv("WAGIC_MAYPROBE")` -> stderr on every reveal-line
  parse, gated only `#ifndef PSP`), `src/AllAbilities.cpp:8096-8105` (same in `MayAbility::
  Update`), `:568-573` + `REVEAL_DBG` `:644` (`getenv` per reveal tick); `src/AIPlayerBaka.cpp:
  4254` (unconditional `printf("Choose attackers ...")` every AI combat, every platform),
  `:3629-3640` (the same `DebugTrace` twelve times); `src/AIPlayerGPT.cpp:18773-18778`
  (unconditional `fprintf(stderr, "menu ... could not be put to the model")` — the #W41-1
  "never silent" ruling; release channel = the translog); `src/TestSuiteAI.cpp:1931` (suite only,
  fine). `WAGIC_FIZZLELOG` on every Vita alpha build (`CMakeLists.txt:294`, 2,146 lines/session of
  fopen/append/fclose from the resolve path) -> OWNER (§3). Slices: 4-M9, 6-M1, 2-F9a, 8 note.
- Vita impact: **FRAME** (a `getenv` + `printf` per AI combat; the FIZZLELOG memory-card writes).
- Fix: wrap the three probe sites in the gate (FIZZLELOG at `:7004` shows the convention); delete
  the printf and eleven of twelve DebugTraces; replace the GPT stderr line with
  `writeTransLog("defer", ..., "menu_not_askable")` + the same text under the gate; `snprintf` at
  `:18887/:18898`. Verify: PARSETEST + suite; grep the gate.

### A34 — MED · LEAK · SUSPECTED — `MTGNewLegend::MoveLegend` / `MTGNewPlaneswalker::MovePW` clone a full `MTGCardInstance` as the menu's source and never free it
- Where: `src/MTGRules.cpp:4090`, `:4207` (`myClone` handed to `AAMover`, `GenericTargetAbility`,
  `GenericAddToGame`, `MenuAbility` as `source`; none owns it). Slices: 5-M5 (suite legend fixtures
  did not surface it — either a path not read frees it or the fixtures don't reach `MoveLegend`).
- Vita impact: **HEAP** (~3 KB per legend-rule event).
- Fix: keep the clones on the rule and delete in its dtor, or pass `card` itself (the clone exists
  only so `share!name!` has a stable name). Verify: LSan on `legend_rule*.txt` twice.

### A35 — MED · MEM-SAFE (determinism) · VERIFIED grep — one RNG stream feeds shuffles, dice AND the heuristic AI; four engine sites still use libc `rand()`, one reseeds it
- Where: `src/Rules.cpp:719-720` (`std::srand(time(NULL))` then `std::rand()`), `src/WParsedInt.cpp:
  1144`, `src/TargetChooser.cpp:1249`, `src/GameStateDuel.cpp:3440`, `src/utils.cpp:92` (`WRand`,
  no engine caller); `utils.h` states the two-stream intent that was never built. Slices: 5-M4.
- Mechanism: replay consumes `rvalues:` in order only if every consumer runs identically — a seat
  swap (Baka draws, GPT does not), an AI change, or any `std::rand()` site (unrecordable, shared
  across the threaded suite) shifts every later shuffle. The "replay diverges at action N"
  mechanism (lane W: `...deck164` diverges at action 94, turn 5).
- Vita impact: NONE; every device transcript's replayability.
- Fix: `aiRandomGenerator` on `GameObserver` seeded from the game seed for AIPlayer*/AIHints;
  route the four `std::rand()` sites through the game generator; delete `WRand`. **Owner note
  (§3):** pre-change transcripts stop replaying (draw sequence changes once); seeded fixtures that
  depend on AI draws need re-baselining.
- Verify: replay a Baka-vs-Baka transcript with one seat swapped to GPT — shuffles identical.

### A36 — MED · PERF (load time) · VERIFIED reading — `MTGAllCards::getCardByName` lower-cases all 74,377 printings per uncached lookup
- Where: `src/MTGDeck.cpp:1138-1201` (`transform(tolower)` on a fresh `string` per printing per
  miss, though `CardPrimitive::lcname` already holds it, `CardPrimitive.cpp:660-665`); the suite
  hides it with `prefetchCardNameCache`, which real play never calls. Slices: 6-M4; lane V added
  a cached `getOtherFaceCard` on top of the same scan.
- Magnitude: a 20-name deck ~1.5 M string constructions at the Vita's first load.
- Vita impact: **FRAME** at deck load / match start (a stall, not steady-state).
- Fix: compare `getLCName()`; better, build the name->printing index once (the prefetch function
  exists, token-vs-real rule included). Verify: time `MTGDeck` construction of `ai/baka/deck1.txt`
  headless before/after; `cards` map identical.

### A37 — MED · CRUD/MEM-SAFE · VERIFIED reading — `OrderedAIAction::getEfficiency` and `getRevealedEfficiency` are a 620-line copy that has drifted; the copy dereferences a NULL the original guards; both leak on the castcard path
- Where: `src/AIPlayerBaka.cpp:208-922` vs `:924-1548`; five drifts (STANDARD_BECOMES, PUMP 20 vs
  50, LIFER 80 vs 100, the sacrifice `/10`, the planeswalker/land tail); `:1226-1227`
  (`GAA->ability` without the NULL check at `:527`); `:256, :970` (`return 99` for `AACastCard`
  before `SAFE_DELETE(transAbility)`). Slices: 6-M3.
- Vita impact: **CRASH** (SUSPECTED reachable via an `aicode=` reveal/scry nested mana ability).
- Fix: one function with the two knobs as parameters, NULL guard kept, delete before return.
  Verify: `ai/*.txt` fixtures (46 AI/0) + PARSETEST.

### A38 — MED · PERF · SUSPECTED — Baka `chooseTarget` re-parses the ability text per candidate (x up to 4 multiplier repetitions); hint TargetChoosers built per creature per pass
- Where: `src/AIPlayerBaka.cpp:2896-2909` (`af.parseMagicLine(tc->belongsToAbility)` + heap
  `OrderedAIAction` inside the per-card loop), `:2884-2894` (`isInTop` x3 per card, each a linear
  list walk), `src/AIHints.cpp:152-170` (`HintSaysDontBlock` builds a `TargetChooser` per hint per
  creature; `chooseBlockers` calls it in five passes `:4449,4503,4519,4633,4718,4768`). Slices: 6-M5
  (selfplay decks carried no hints; counters showed 0 re-parses there).
- Vita impact: **FRAME** on the AI's targeting/blocking ticks with hinted decks (adds to A3).
- Fix: parse `belongsToAbility` once before the loop; `isInTop` once per card; cache hint
  TargetChoosers on `AIHint`. Verify: 46 AI/0; counter probe on a hinted deck.

### A39 — MED · PERF/IO · VERIFIED (stats file rewritten during the suite, halving each save) — AIStats reads a file at the first event and writes one at every game end; every damage event walks the action layer with a `dynamic_cast<ALord*>` per ability and sorts the table
- Where: `src/AIPlayerBaka.cpp:4902-4911, 5190-5197`, `src/AIStats.cpp:85-95` (lord scan), `:104`
  (`list::sort`), `:147-193` (`save()` stores `value/2` — the table decays toward 0, so `isInTop`
  mostly answers on `tooSmallCountsForTrue`; nearly inert, still paying the I/O). Slices: 6-M6.
- Vita impact: **FRAME** (per-damage-event scan) + a flash write per match.
- Fix: skip load/save when `observer->mSuiteGame`/headless; dirty-flag the save; use the `ALord`
  list the action layer already maintains. Whether to keep the feature at all -> owner (§3).

### A40 — MED · CRUD/PERF · VERIFIED reading — `WEventEngageMana` / `WEventEngageManaExtra` constructors do 12 string-keyed `hasType("snow")` lookups per mana produced and mutate player state
- Where: `src/WEvent.cpp:205-262` (two identical 60-line blocks). Slices: 7-F11.
- Magnitude: a 6-land tap = 72 `map<string>` lookups inside event ctors; any `WEventEngageMana`
  built for another reason double-counts snow mana.
- Vita impact: **FRAME** (every mana payment).
- Fix: `const bool snow = card->hasType(Subtypes::TYPE_SNOW)` once, pick the pool owner once,
  index `snowMana{...}` by colour, one shared helper — or move snow accounting to the ManaPool
  `add` site. Verify: suite (grep `snow` in `bin/Res/test/` first), PARSETEST.

### A41 — MED · PERF · VERIFIED reading — the cache-miss path builds an `ostringstream` for a `LOG` that is a no-op, once per miss and per eviction; `RemoveOldest` is O(n) per eviction
- Where: `include/WResourceManagerImpl.h:88-105` (`RequiresOldItemCleanup`), `JLogger.h:9-12`
  (`LOG` = `{}` on non-PSP), `WResourceManager.cpp:1483` (`Cleanup()` calls it per miss + per
  eviction), `:1100` (`RemoveOldest` linear over the cache map). Slices: 7-F12. With A2's 24-card
  effective cache the Vita sits in the over-limit state permanently, so this fires on every new art.
- Vita impact: **FRAME** on art loads (main thread, before the load even starts).
- Fix: `#if defined(DOLOG) || defined(PSP)` around the stream; optionally an intrusive LRU list.
  Verify: perf under a fresh-deck selfplay — `RequiresOldItemCleanup` vanishes.

### A42 — MED (Vita) · PERF/CRUD · VERIFIED (dev log + ziplog) — resource MISSES are never memoised and each costs several synchronous filesystem calls on the render thread
- Where: `src/MTGCardInstance.cpp:2263-2316` (`getSample`: `<subtype>.wav`, `<ability>.wav`,
  `<type0>.wav` probes per cast, a miss re-probed on the next cast of the same card),
  `include/WResourceManager.h:76-80` (`PlaySample("")` -> `RetrieveSample("")` -> `AttemptNew("")`
  opens the sfx DIRECTORY, fails, deletes — the blank-name "Destroying WCachedResource" lines),
  `WResourceManager.cpp:1321` (`Cleanup()` before every load attempt), `:1380-1420` (5-strike
  tombstone; retry spacing PSP/VITA-only; `ClearUnlocked()` at every menu return erases the
  tombstones while `sMissCounts` keeps counting), `:50-55` (`sMissCounts`/`sMissTime` 2 map
  entries per failing id, process lifetime); `JGE/src/zipFS/zfsystem.cpp:391` + `JFileSystem`
  (`zip-entry-miss`: 2 `FileNotZipped` opens + 2 `openForRead` + 2 stats = 6 FS calls per miss,
  171 miss lines in the vpk ziplog e.g. `thumbnails/0.jpg` for card id 0 repeatedly);
  `JGE/src/pc/JSfx.cpp` -> `JFileSystem.cpp:799/:508` (sound extraction into `ux0:data/Wagic/
  sound/sfx/` whose parent dirs are created only `#if defined(ANDROID)` — if the Vita is silent,
  this is why). Slices: 7-F18, 7-F16 (sMissCounts), 8-L6, 8-L5.
- Magnitude: 345 `Destroying WCachedResource` lines / 15 games on desktop (143 blank-name);
  ~23 failed probes per game; on the Vita each is a `sceIo` open on the render thread.
- Vita impact: **FRAME** (sync FS on the render thread) + a possible silent-sound functional bug.
- Fix: memoise the miss (`sampleResolved` on the instance or a sentinel); early-return `PlaySample`
  on ""; tombstone id 0 / token ids at the game layer; let the ledger tombstone after the FIRST
  miss when the set zip parsed fine (`dir>0` breadcrumb); move `Cleanup()` below the
  `RETRIEVE_EXISTING` check; bound `sMissCounts` (clear on `ClearUnlocked` or one strike byte);
  make the mkdir chain unconditional or load samples via `Mix_LoadWAV_RW` from the bytes ReadFile
  already produced (drops the "AVOID Using This function!!!" copy). Verify: dev log shows one probe
  per missing name per process; ziplog miss lines drop to ~distinct ids.

### A43 — MED (desktop text mode) / LOW (Vita) · PERF/MEM-SAFE · SUSPECTED — the focused big card is re-parsed and its vectors copied every frame in `AlternateRender`; `FilterCard` type-puns a `CardPrimitive*` as `MTGCardInstance*`
- Where: `CardSelector.cpp:393-402` (active card rendered twice per frame; `:377-378` timer per
  frame not dt), `CardGui.cpp:801-1100` (`AlternateRender`: vector copies at `:820,834,882`,
  `FilterCard` `:863` -> `:1562-2118` with a `CardDescriptor` + ~40 `find/substr` per render item
  per frame, `stringstream`s `:955-975`), `:2142`, `:1104`; `CardGui.cpp:1566` (`(MTGCardInstance*)
  _card->data` — `data` is a `CardPrimitive*`; `cd.match` reads `foretellTurn/kicked/counters/
  getPower()` past the object for any filter beyond `types`/`colors`). Slices: 7-F5, 7-F13.
  **Downgraded for the Vita:** slice 7's premise "the packless-install default on the Vita" is
  contradicted by lane V (art packs ARE deployed on the console, `ux0:/data/Wagic/sets/...`); on
  the Vita this path runs only for art-less cards (the 103 missing back faces, tokens, id 0) and
  in text mode.
- Fix: `const&` for the four vector copies; parse each render item's `mFilter` once into a
  `CardDescriptor` at modrules load (`ModRules.cpp:302`) and match against `MTGCard*`-safe fields
  (or pass the real instance `AlternateRender` already `dynamic_cast`s for `zpos`); drop the
  duplicate `active->Render()`. Verify: visual parity in text mode; ASAN with a `[power>=2]`
  modrules filter.

### A44 — MED · LEAK · VERIFIED reading (LSan-visible) — `WGuiKeyBinder::populateKeyBindingList` leaks every `OptionKey` row on "Load Defaults..."
- Where: `WGui.cpp:2536-2549` (`items.clear()` then re-`Add`; `~WGuiMenu` `:838-842` owns `items`;
  `currentItem` not reset). Slices: 7-F8. Vita impact: **HEAP** (15-25 objects per press, options
  screen only). Fix: a `clearItems()` that deletes, `currentItem = -1`. Verify: press twice under
  the ASAN build.

### A45 — MED · MEM-SAFE · VERIFIED reading — `StoryFlow::parse` hands TinyXML an un-terminated buffer (heap over-read); two dead JGE modules do the same
- Where: `StoryFlow.cpp:610-614` (`NEW char[size]; ReadFile; doc.Parse(xmlBuffer)`; StyleManager
  `:928-932` does it right via `readIntoString`); `JGE/src/JAnimator.cpp:50`, `JGE/src/
  JResourceManager.cpp:101` (same, unreachable but linked); `JGE/src/JTTFont.cpp:315` (glyph rows
  overflow `texBuffer`, unreachable). Also `StoryFlow.cpp:123,157` (`replace(pos, pos+6, ...)`
  passes an end as a count — wrong text replaced unless at offset 0). Slices: 7-F9, 8-L4, 7-F17.
- Vita impact: **CRASH** (story mode on a small heap parses garbage or faults).
- Fix: `NEW char[size+1]; xmlBuffer[size] = 0;` or `readIntoString`; the dead twins go with A50.
  Verify: ASAN, enter Story mode — reports heap-buffer-overflow read today.

### A46 — MED · MEM-SAFE · VERIFIED reading (ARM `char` is unsigned; desktop cannot see it) — `JLBFont::GetStringWidth` reads past `mCharWidth[256]` for any byte >= 0x80
- Where: `JGE/src/JLBFont.cpp:243-259` (`ch = *p - 32; if (ch < 0) continue; len +=
  mCharWidth[ch+mBase]`, no upper bound; `DrawString :157` has the `> 127` guard), `JLBFont.h:16`
  (`MAX_CHAR 256`); `:52` dereferences `mTexture` one line before its NULL check. Slices: 8-M2.
- Mechanism: the card corpus carries CP1252 bytes; 0xE9 -> ch 201 + mBase 128 -> index 329, a
  float read 292 bytes past the array. Read-only: mis-centred text on Vita/Android/PSP.
- Vita impact: OOB read (wrong widths), not corruption.
- Fix: `if (ch < 0 || ch > 127) continue;`; swap `:52/:54`. Verify: desktop `-funsigned-char`
  under ASAN, render a card with 'é'.

### A47 — MED · PERF · SUSPECTED (reasoned, not measured) — the Vita main loop sleeps AFTER a vsync-blocked swap, turning a 25 ms frame into 50 ms
- Where: `JGE/src/Vitamain.cpp:373` (`vglWaitVblankStart(GL_TRUE)`), `:620` (`vglSwapBuffers`),
  `:632-636` (sleep to `TARGET_FRAME_MS 33`). Slices: 8-M3; lane W's new `avg_swp`/`max_swp`
  telemetry is the instrument.
- Mechanism: work in (33.3, 50) ms -> swap returns at 50, no sleep -> 20 fps; vsync already caps
  the rate, the manual sleep only ever adds. This is the regime the "severe performance issues"
  game lived in.
- Vita impact: **FRAME** (pacing regime; one measurement decides it).
- Fix: drop the sleep when vsync is on (vsync IS the cap), or `vglWaitVblankStart(GL_FALSE)` +
  keep the sleep via vitaGL's swap-interval control. Gate on the next memlog: if `avg_swp` shows
  frames parked at ~16.7 ms multiples plus a sleep, land it; the 30-fps-cap POLICY is the owner's
  (§3).

### A48 — MED (dead today) · MEM-SAFE/LEAK · VERIFIED reading — `JGuiController::RemoveAt` deletes the wrong object and leaves a dangling pointer
- Where: `JGE/src/JGui.cpp:243-261` (`erase(begin()+i); delete mButtons[i];` — deletes the
  successor, leaks the intended object, keeps the freed successor in the vector -> UAF +
  double-delete in `~JGuiController`); `:176` (click-to-select loop derefs `mObjects[i]` with no
  NULL guard while the Update/Render loops got guards this week); `mCursor` never initialised
  (never read). Public virtual on the base of SimpleMenu/DeckMenu/IconButtonsController/
  SimplePopup/StoryDialog — the class whose `Add(0, "...")` trap produced vpk9's NULL button. No
  callers today (only `Remove(int)/Remove(JGuiObject*)`, themselves uncalled). Slices: 8-M4.
- Fix: `JGuiObject* o = mButtons[i]; erase; delete o;` — or delete the three functions so the
  trap cannot be armed; guard `:176`.

### A49 — MED (rare) · MEM-SAFE (threads) · SUSPECTED — detached model workers vs process teardown; `curl_global_init` per player with no cleanup and no init at all on the probe threads; a function-local static first-touched on a worker thread where the Vita's `__cxa_guard` has no lock
- Where: `src/GptConfig.cpp:1383-1398` (`std::thread(fn, ctx).detach()`), `src/AIPlayerGPT.cpp:
  9161-9163` (`curl_global_init` in every ctor; grep `curl_global_cleanup` = 0), `:8209-8261`
  (`WorkerMain`), `:8232-8233` (`static GptMutex logMtx; static string lastErr;` inside the Codex
  failure branch — first touch on the worker; `worktrees/psp/CMakeLists.txt:308-322` records
  gthreads inactive and a live `__cxa_guard_acquire` crash on hardware), function-local statics
  `GptConfig.cpp:899, :1078`; probes in `OptionGpt.cpp`/`GameStateOptions.cpp` call
  `curl_easy_init` on worker threads with no global init if no AIPlayerGPT was ever built.
  Slices: 1-F11, 1-F9 (curl), 3-4, 3-5. Both slices find the AsyncState/WorkerCtx ownership
  itself SOUND (mutex-guarded, shared_ptr, `delete ctx` on the worker only, refused-thread path
  resets synchronously).
- Mechanism: a process that exits with the losing seat's ask in flight (blockers on the lethal
  swing) runs the worker against OpenSSL's atexit and this TU's static dtors -> intermittent
  SIGSEGV after `main` returns, which a harness keyed on exit status records as a crash;
  `curl_global_init` is documented not thread-safe and can run while the previous duel's worker
  is still inside libcurl; two concurrent probes -> concurrent implicit global init; AI-vs-AI on
  the subscription preset both failing at once can double-construct the mutex on the Vita.
- Vita impact: **CRASH** (rare; at exit / at double failure).
- Fix: one `gptCurlInit()` with a function-local `static bool once` on the game thread (ctor +
  both probe sites); an atomic in-flight counter around `WorkerMain`; a bounded wait (<= 2 s, 50 ms
  poll) at the game-exit path before `main` returns, then `curl_global_cleanup`; hoist the two
  statics at `:8232-8233` to namespace scope (constant-initialised at load).
- Verify: PARSETEST/suite unchanged; a headless game with `WAGIC_GPT_TIMEOUT=600` against a slow
  endpoint, killed at a decision, exit status 0 — **repeat >= 30 times** (rare-event A/B power).

### A50 — MED · CRUD · VERIFIED (0 users each) · **OWNER DECISION** — dead modules compiled into every target, two carrying latent memory bugs
- Where: `src/AIPlayerBakaB.cpp` (186 lines, only under `AI_CHANGE_TESTING` = `_DEBUG && WIN32`,
  `include/config.h:34-36`; the `#ifndef AI_CHANGE_TESTING` guards in `computeActions` `:3727,
  3770,3941,3948` are permanent); `Navigator.cpp` (782 lines, referenced only by
  `CardSelectorSingleton.cpp`, whose `Create/Instance/Terminate` have zero callers —
  `DuelLayers.cpp:83` `NEW CardSelector` directly); `GuiFrame.cpp:15-73` (class body commented
  out, yet `DuelLayers.cpp:112` allocates one per duel and it takes a slot in the per-frame layer
  loop); `GuiCardsController.cpp` (empty TU); `PlayGuiObjectController.cpp:97-165` (60-line
  commented body); JGE: `JAnimator`, `JResourceManager`, `JTTFont` (latent OOB writers/readers,
  A45), `JParticle*`, `JDistortionMesh`, `JOBJModel`, `JSpline`, `JGBKFont`, `Downloader`,
  `JNetwork`/`JSocket` (7 mentions, relic transport), `glutmain/Xmain/winmain/Qtmain/JGfx-fake`
  (`JGE/Makefile:25` still names `Xmain.o`); `NETWORK_SUPPORT` defined in `Makefile.sdl:17` so
  `NetworkPlayer.cpp`, the `MENU_STATE_NETWORK_*` branches and `RemotePlayer` compile in
  although `RegisterNetworkPlayers` is empty; `GameOptions.cpp:293-305` (`menuStr` unused
  buffer), `DeckStats.cpp:339-360` (loop that only checks `deck1`), `MTGPack.cpp:226`
  (`getCardByName("card")` — a `<card>` pack entry can never resolve; dormant). Slices: 6-M7,
  7-F10, 7-F17, 8-L4.
- Vita impact: resident code in the 33 MB self; one empty layer per frame; attack surface.
- Fix: delete the files / branches, update `wagic-SDL.pro`, the Vita CMake source lists,
  `JGE/Makefile`; drop `NETWORK_SUPPORT` from the SDL build. Verify: all targets build; suite;
  PARSETEST. Owner because: dropping modules (and the network feature flag) is his call.

### A51 — MED (latent, suite-only) · MEM-SAFE · VERIFIED latent — the test harness writes fixtures into fixed arrays with no bound
- Where: `src/TestSuiteAI.cpp:1214-1218` (`actions[nbitems++]` into `string actions[100]`,
  `MAX_TESTSUITE_ACTIONS` `TestSuiteAI.h:6`), `:2352-2374` (`int list[100]` from every [INIT]
  zone). Largest fixture today: 45 [DO] lines, 33 cards/player. Slices: 6-M2.
- Vita impact: NONE. Fix: vectors (`MTGPlayerCards` already takes `(int*, int)` — pass `data()`).
  Verify: a scratch fixture with 120 [DO] lines green under ASAN.

### A52 — MED (latent) · MEM-SAFE · SUSPECTED (0 hits) — `MenuAbility::clone()` indexes `optionalCosts[i]` for every `abilities[i]`; `ExtraManaCost::clone()` aliases `costToPay` that `~ExtraCost` deletes
- Where: `src/AllAbilities.cpp:8632-8640` (only `GenericPaidAbility :4600` fills one
  `optionalCosts` entry while `abilities` can hold 2+ `:4544-4553`; reached via `MultiAbility::
  resolve :8721` cloning a `MayAbility` leg); `src/ExtraCost.cpp:92-96` vs `:36-40` (`ExtraCosts::
  clone :1879-1888`, `ManaCost.cpp:478,510,684,757` copy cost lists wholesale; `MenuAbility` builds
  `toPay` lists with `ExtraManaCost` at `:8544,8585`). Slices: 4-M7, 4-M8.
- Fix: `if (i < (int)optionalCosts.size() && optionalCosts[i])`; `ec->costToPay = costToPay ? NEW
  ManaCost(costToPay) : NULL;`. Verify: ASAN suite.

---

## 1c. LOW — carried (not laned this wave unless a lane owns the file and it is a one-liner)

| id | cat | where | what | slices |
|---|---|---|---|---|
| L1 | PERF | `AIPlayerGPT.cpp:2280-2325` (call sites 4829, 9790, 24614) | `collectTribalTypes` rebuilds the creature-type vocabulary + lowercases 4 zones per changeling per render; a deck-level fact — compute once per player in `buildSystemPrompt` | 1-F7 |
| L2 | PERF | `AIPlayerGPT.cpp:10988-11024` | `noteDesignationChange` + `noteChosenNames` scan both battlefields on EVERY event (0.11% small boards; 2xN per counter event on loop boards) — gate to the event types that can change them | 1-F8 |
| L3 | CRUD | `AIPlayerGPT.h:258-286`, `.cpp:1486-1489`, `:8169-8197`, `:3902-3912` | stale per-tick comment (it steered two audits wrong), dead null-guard in `dynamicMagnitudes`, split WorkerCtx doc comment, `collectMutatePile` run twice | 1-F9 |
| L4 | PERF/IO | `AIPlayerGPT.cpp:9314,9465,9658,9710`, `GptConfig.cpp:256` | translog + gpt-log open/close per record (the code's own `:8788-8792` names it a Vita lag source) — one lazily-opened `ofstream mTransLog`, flush per record. Vita impact NONE while `translog=0` is the console default | 1-F10, 3-6 |
| L5 | PERF | `AIPlayerGPT.cpp:16760,16589,16800,16804,17372,17384` | `tail.str()` copied up to 5x per render (116 KB window) — one `const string tailStr` | 2-F5 |
| L6 | MEM-EFF | `:16781-16785, 18572-18576, 17918, 18645, 13032-13034` | per-turn maps keyed by whole menu text (3 copies of a 116 KB list per turn) — hash keys | 2-F6 |
| L7 | MEM-SAFE | `parseBlockAssignments :23113-23144`, `parseAttackerSet :22295-22431`, `nameOrdinal :20719`, `proseAttackerOrdinal :21685`, `salvageProse* :21772, 22100`, `becomesBlockedSelfPump :21555,21560` | unbounded digit-run `int` accumulators: `"B4294967297:A1"` wraps to `B1:A1` and declares a blocker the model never named — one `readDigits(cap=100000)`; +3 PARSETEST cases | 2-F7 |
| L8 | MEM-SAFE | `noteLoopTake :16256-16266`, `:12847, 17148, 16614, 17157, 16432, 17143` | raw `MTGAbility*`/`MTGCardInstance*` identities compared across turns (no deref): address reuse can dispatch a queued repeat-N plan onto a foreign row if BOTH pointers collide — clear on the source leaving play | 2-F8 |
| L9 | CRUD | 8 clusters: echo word split (`:15153-15173` byte-identical to `:14787-14805` + 4 drifted copies), `CHOICE:` walker x5, `</think>` strip x12, 51 inline tolower loops (5xN copies per parse in `parseChoice`), gang-block price x2, `groupCombatCandidates` permutation x3, auto-tap scan x2, `drawPunisherScan`+join x4 | legibility for agents — each copy is where a fix lands on one seam and misses its twin (N-152e, W37 history); the tolower cluster is also a real per-parse cost | 2-F9b |
| L10 | PERF | `AIPlayerGPT.cpp:10103-10108, 10241-10245, 5730-5747, 10044-10070` | narration cap: once the zone digest passes ~3.9 KB every line re-trims (4 zone walks + 24 KB rebuild per event); an append that crosses the cap only because of the pending-phase line trims with an EMPTY marker | 3-7 |
| L11 | security | `GptConfig.cpp:163-176, 185-190, 1019-1024, 1653-1661` | keysalt = `rand()` where no `/dev/urandom` (Vita, Windows) seeded by `srand(time(0))`; three secret files created 0644 then chmod — `sceKernelGetRandomNumber` / `rand_s`; `open(..., 0600)` | 3-8 |
| L12 | MEM-SAFE | `OptionGpt.cpp:49, :102` | row binds `&cfg.urls[0]`; `cfg = fresh` on Reload can reallocate -> dangling `mBind` read every frame — bind `(GptSettings*, index)` | 3-10 |
| L13 | CRUD/bug | `AllAbilities.cpp:8676-8680, 10144-10150, 8932-8948, 1516, 5530/5537, 11039, 2865`; `MTGAbility.cpp:1030-1041, 7735-7738`; `AllAbilities.h:3651-3665, 7630-7635` | dead deletes/parses/no-ops, deref-before-check x3, `can play` clones a whole instance per evaluation, copy ctor copies 50 of `menuText[256]`; **two correctness bugs hiding as dead work:** `AAsLongAs::findMatchingAmount` only counts the LAST zone; `ABushidoAbility::destroy` subtracts from `attackCost` not `bushidoPoints`; `GrantedAndAbility` dangling after the copier dies | 4-L1 |
| L14 | MEM-SAFE | `AllAbilities.cpp:163, 1079` | `CardDisplay(...)` passes its own base members as ctor args (uninitialised reads, overwritten by `init()`) — pass literals | 4-L2 |
| L15 | PERF | `GameObserver.cpp:2611-2612, 2660-2663, 1741-1749` | `AffinityNeedsUpdate` re-armed by `WEventGameStateBasedChecked` every tick (0.64%) — set only from cost-relevant events; test the keyword bits before the two `getConvertedCost()` walks | 5-L1 |
| L16 | CRUD | `ActionStack.cpp:1181-1193` | dead `dynamic_cast<AManaProducer*>` on an `Interruptible*`; if it ever fired, `manaObjects.erase` with a foreign iterator — delete 4 lines | 5-L2 |
| L17 | MEM-SAFE | `ManaCost.cpp:688-709` | `operator=` shallow-copies nine owned pointers, skips X — implement over `copy()` or `= delete` | 5-L3 |
| L18 | misc | `GameObserver.h:281` (`if(this)`), `Player.cpp:14-70` (`deckId` uninitialised), `DecisionContract.cpp:345` (size_t underflow), `GameObserver.cpp:1268-1280` (erase inside forward loop skips), `DamagerDamaged.cpp:81` (`char buf[6]` sprintf) | one-liners | 5-L5 |
| L19 | MEM-SAFE | `utils.cpp:356-374` (`wordWrap` walks past index 0), `PriceList.cpp:37-41` (`char writer[20]`), `MTGDeck.cpp:1248` (substr throw), `:1331-1333` (NULL `newcard`), `AIHints.cpp:323,336` (deref before NULL test), `GameOptions.cpp:231-268` (uninitialised `color[]`), `MTGDeck.cpp:1817` (`setInfo` NULL) | small buffer/NULL hazards | 6-L1 |
| L20 | PERF | `DeckStats.cpp:51,79,91,107,187` | whole `map` copied by value per query; substring match resolves `deck2` to `deck20` | 6-L2 |
| L21 | MEM-EFF | `CardPrimitive.h:53-58, 73-76` | ~5 mostly-empty per-primitive members (`formattedText`, `nameOrig` empty on all 27,788, `backSide` 458, `partner` 37, `spellTargetType` 4,035, `magicTexts` header) — ~3 MB on 32-bit across the DB; side-table like `RareStrings` | 6-L3 |
| L22 | PERF/CRUD | `AIPlayerBaka.cpp:1567-1584` (7x2 zone walk per potential-mana enumeration), `AIPlayer.cpp:218-244` (`char deckFile[512]` sprintf of a profile path), `MTGDeck.cpp:2053` (`char[4096]` + a `MTGPack` file open per set x336 at load), `DeckManager.cpp:108-120` (a stats file read per opponent deck) | trailing items | 6-L4 |
| L23 | PERF | `GuiStatic.cpp:224-225` | residual after lane W: the visible library icons still compute both `hasAbility(SHOWFROMTOPLIBRARY/SHOWOPPONENTTOPLIBRARY)` for every zone type — move inside `type == GUI_LIBRARY` | 7-F7 |
| L24 | LEAK | `GameStateDeckViewer.cpp:67-98` (`menuButton` never deleted; `myCollection->parent` freed while `playerdata` only freed in `End()` -> double-free if the app exits from the editor), `GameStateMenu.cpp:164, 362` (`Release(bgTexture)` without nulling) | teardown gaps | 7-F14 |
| L25 | MEM-SAFE | `GameStateDuel.cpp:3180, 3206-3252` (`char buffer[256]` + `sprintf` with a user deck name), `WFilter.cpp:579-623` (five `char buf[4068]` recursive `"%s&%s"`), `GameStateShop.cpp:924-936` (uninitialised `char buffer[512]` returned), `GameStateAwards.cpp:344` (`counts[setId]` with `setId == -1` — same class as the W53 `WDataSrc.cpp:361` fix), `:394` (`strong->` for `tough->`), `WResourceManager.cpp:671-703` (`char buf[512]` themes path) | `snprintf`/`std::string`, clamp `setId` | 7-F15 |
| L26 | MEM-EFF | `DuelLayers.cpp:185-198` (`NEW Pos` per unclaimed zone change, freed only in the dtor — free at `mTrash->cleanup()` `:207`), `CardDisplay.cpp:84` (whole-zone vector copy per frame while a graveyard/library display is open), `GuiMana.cpp:115,293` (a `hgeParticleSystem` per engaged mana; `Update` steps every system — `NOSTARSDYNAMIC` as the Vita default -> owner) | per-duel / per-frame churn | 7-F16 |
| L27 | CRUD | `GameStateDuel.cpp:1386-1435` (`MENUITEM_TASKBOARD` appended to the EXISTING menu on every `JGE_BTN_MENU` press while the LLM patience prompt is open — duplicate rows), `GameStateMenu.cpp:651,773` (`gModRules.menu.other` copied per Update AND per Render) | leftovers | 7-F17 |
| L28 | LEAK | `JGE/src/zipFS/zstream.h:160` (`~izstream` drops the buffer without unuse/close; a pooled buffer stays "used" forever — 4 of those and every core.zip read fails), `zfsystem.h:243` (`m_Zipped/m_Used` uninitialised); all 5 game call sites close today | `~izfstream() { close(); }` + init the flags | 8-L1 |
| L29 | build | `worktrees/psp/CMakeLists.txt:75` compiles the Vita with `-std=c++11` against the gnu++14 pin — any gnu++14 construct in a later lane breaks only the Vita build 20 min into AIPlayerGPT.cpp | -> owner (§3) | 8 note |

---

## 2. Vita-impact summary (the console is the point)

| impact | items | what the owner would feel |
|---|---|---|
| CRASH | A1, A2, A11, A10, A14, A28, A29, A30, A37, A45, A49 | A1+A2 are the measured `bad_alloc`/arena-ratchet path (~30 games to the wall); A11 is the dead-token UAF class the core dumps already name; the rest are latent-but-armed |
| FRAME — steady, every frame | A6 (+A26), A5, A40, A25 | EST 2.6-5 ms/frame of ability polling at 12-22 permanents + 2-5 ms/frame of per-card string resolution at 25 permanents, out of a 33 ms budget |
| FRAME — periodic spikes | A3 (opponent's turn, every 0.1 s, EST 100-200 ms on wide boards — the single largest number), A4 (4 Hz while a card is focused, EST 4-9 ms; 13-26 ms per hand move), A39 (per damage event), A33 (per AI combat) | A3 is the "really bad frame rate on wide boards" candidate lane W measured but did not touch |
| FRAME — load/burst | A2 (2 MB decode per new art), A41, A42 (sync FS per miss), A36 (deck load), A21/A22/A19/A20/A23 (GPT decisions only) | stalls, not steady-state |
| HEAP — per game / per match | A8, A9, A12, A13 (commander), A15, A17/A18 (GPT seat), A31, A34, A44 | device budget: live data +0.2 MB/game total; the arena ratchet (A2) is 7x that |
| NONE (desktop/suite/replay) | A24, A27, A35, A51, A16 (resident code, not heap) | correctness/diagnosability |

Every FRAME estimate above is desktop-derived; lane W's `frames`/`frame` memlog lines
(`avg_upd/avg_rnd/avg_swp` per turn + the >100 ms single-frame line with `p1perm/p2perm/hand/abil`)
on the next VPK are what settle the ranking between A3, A5, A6 and the vitaGL draw share.

---

## 3. Lane plan (HIGH + MED), owner decisions, carried

Ownership is by FILE except the three files too large to split by anything else, which are split
by LINE REGION exactly as listed (`AIPlayerGPT.cpp`, `AllAbilities.h/.cpp`, `MTGAbility.cpp`);
a lane that touches a header adds members in a lane-named block so hunks stay disjoint. Every
lane: own worktree off master, gate = single-threaded suite (1215 / 2 known: lifeline +
merrow_reejerey / 46 AI / 0 timeouts) + PARSETEST 2249/0, under `systemd-run ... MemoryMax=4G`
+ `timeout 600`; ASAN/perf where named. Each lane ships a DISABLE FLAG for any output-affecting
optimisation (silent-instrument rule) and removes every probe before commit (no diagnostics in
release).

**Lane A — ability object model: ownership, leaks, latent clones**
Items: A8, A9 (incl. the `TokenAndAbility` ownership ruling), A10, A14, A12's `ExtraCost.cpp`
part, A28, A29, A30, A31, A52, A33's two probe sites, L13, L14. Files: `AllAbilities.h` regions
<3500 / 4135-4442 / 6593 / 7547 / 7916-8125 / 8261 + the Tr* block 255-1806; `AllAbilities.cpp`
regions 163-1516 / 4089-4567 / 8053-8070 / 8632-8680 / 9974-10006 / 11039-11249
(8861-8880 is lane B's); `MTGAbility.cpp` regions 2449-2453 / 2734-2744 / 3741-4502 / 5147 / 7725-7738 /
9074-9081 / 9268-9279; `MTGAbility.h`; `ExtraCost.cpp/.h`; `TargetChooser.cpp:16,1864-1868` +
`.h`; `ReplacementEffects.cpp`; `CardDescriptor.cpp`; `GameStateDuel.cpp:576`; `MTGCardInstance.cpp:
217,324,378-388,473,2181-2193` (the `TokenAndAbility` + chooser ownership sites).
Gate: ASAN+LSan full suite (`Makefile.sdl-asan`; runs >600 s under contention — MemoryMax=8G,
timeout 900 as slice 4 did). Risk: `deletedpointers` removal exposes the double-owner the hack
masked — ASAN reports it on the spot, which is the point; `TokenAndAbility` deep-copy touches the
instance copy path. Success (falsifiable): LSan total 82,108 B / 5,319 allocs -> **< 4 KB and the
frames `GenericAddToGame::resolve`, `ATokenCreator::resolve`, `AACloner::resolve`, `SnowCost::
isPaymentSet/doPay`, `CardDescriptor::CardDescriptor`, `GenericPaidAbility::resolve` all absent**;
0 `ERROR: AddressSanitizer`; suite 1215/2.

**Lane B — per-tick ability machinery (perf)**
Items: A6 (all five parts), A26, L15, L16. Files: `AllAbilities.h` regions 3643-3705 / 4695-4717 /
4800-4921; `AllAbilities.cpp:8861-8880`; `MTGAbility.cpp` regions 106-1067 / 5891 / 6355-6490 /
8012-8053 / 8563-8572 / 8644-8714 / 9111-9116 + `testDestroy`; `ThisDescriptor.cpp`;
`ActionLayer.cpp:149-165` + `.h`; `ActionStack.cpp/.h`; `WParsedInt.cpp/.h`; `GameObserver.cpp:
1741-1749, 2611-2612, 2660-2663`; `MTGCard.h` (efficiency cache). Rebases on lane A if any
`MTGAbility.cpp` hunk overlaps.
Gate: suite + PARSETEST; `perf record -F 997 -g` of the headless suite before/after (slice 5's
rig); lane W's -O2 offscreen frame profile (`Makefile.sdlO2` + `objs-o2`, `WAGIC_FASTCLOCK=0.0333`,
SDL offscreen driver — scripts in `~/.gatelogs/w53W/`). Risk: event-gating a condition that a
card changes without an event (suite catches a verdict drift; fix L13's `findMatchingAmount`
FIRST in its own commit so any verdict change is attributable). Success: `updateTargets` calls
1,191,957 -> < 100,000 on the same suite; `updateTargets` 15.6% -> < 3%, `testDestroy` 7.75% ->
< 1% of suite CPU; -O2 `ActionLayer::Update` 0.26 -> <= 0.10 ms/frame at 12-22 permanents;
suite verdicts identical.

**Lane C — engine-core memory (ManaCost, zones, instances)**
Items: A7 (ctor fix + the fixture re-measure), A11, A12's engine sites (`ManaCost.cpp:1110`,
`ManaEngine.cpp:679`, `MTGRules.cpp:367,1988-2287`), A13, A15(a), A27, A34, L17, L18. Files:
`ManaCost.cpp/.h`, `ManaEngine.cpp:679` (lane S is merged), `MTGGameZones.cpp/.h`, `MTGRules.cpp`,
`MTGCardInstance.cpp:850-863` (region; lane A owns the other regions), `Player.cpp`,
`GameObserver.cpp:1268-1280` + `.h:281`, `DecisionContract.cpp:345`, `DamagerDamaged.cpp:81`.
A15(b) (the shared-primitive refactor) is a NEXT-WAVE lane of its own — blast radius is every
`card->name`/`magicText` reader; do it behind accessors after (a) has been measured.
Gate: suite + PARSETEST + ASAN suite; LSan on a commander game (`WAGIC_REPLAY` of a commander
transcript or a commander selfplay pairing) and an attack-cost + PayZero + snow fixture; the
single-fixture RSS run (`gift_of_the_gargantuan_i1085.txt`, slice 6's sampler); a token-death
two-turn fixture for A11. Risk: A11 holds dead tokens one turn longer (anything that scans the
garbage zones); A7's correctness half (`hasAnotherCost` readers — grep AIPlayerBaka). Success:
fixture RSS peak 483 MB -> < 250 MB; `ManaCost::init` share 5.0% -> ~2.5%; `initDeck` leak stack
gone; dead-token UAF signatures gone from an ASAN selfplay corpus; A27's Serum Powder undo
fixture green; retry lane W's two vpk12 transcripts (a replay that now loads is a bonus, not
the gate).

**Lane D — render path (CardGui / GuiPlay / resource cache / events)**
Items: A5, A25 (parts 1-2 only — RenderSpell is D28), A40, A41, A42 (game-layer half: `getSample`,
`PlaySample`, tombstones, `sMissCounts`, `Cleanup()` placement), A43, A44, A45 (StoryFlow only),
L23, L24, L25, L26 (Pos, CardDisplay), L27. Files: `CardGui.cpp/.h`, `CardSelector.cpp`,
`GuiPlay.cpp`, `GuiPhaseBar.cpp` (the `GuiHand.cpp:76` literal belongs to lane E, which owns
`GuiHand.cpp` for A4), `GuiBackground.cpp`, `GuiStatic.cpp` (post lane W),
`GuiMana.cpp`, `WResourceManager.cpp/.h`, `WResourceManagerImpl.h`, `WCachedResource.cpp`,
`MTGCard.cpp:73-85`, `MTGCardInstance.cpp:2263-2316` (region), `WEvent.cpp`, `WGui.cpp`,
`StoryFlow.cpp`, `ModRules.cpp:302`, `DuelLayers.cpp:185-207`, `CardDisplay.cpp`,
`GameStateDuel.cpp:1386-1435, 3180-3252`, `GameStateMenu.cpp`, `GameStateDeckViewer.cpp`,
`GameStateShop.cpp`, `GameStateAwards.cpp`, `WFilter.cpp`.
Gate: suite (display-only) + PARSETEST; lane W's -O2 offscreen profile + slice 7's counter set
(`RetrieveCard`, `GetQuad(string)`, `RetrieveQuad`, `Subtypes::find` inside `CardGui::Render`,
`hasPossibleAttackers`, `GuiPlay::Replace`) before/after; ASAN for A44/A45; a windowed run with
`sets` restored in the worktree for visual parity (the desktop tree has `sets.hidden`). Risk:
a stale cached `JQuadPtr` after a transform/copy (the invalidation events are the contract —
lane V's eight transform fixtures + the DFC prediction are the check); `Replace()` laziness vs
`CardSelector::Add`'s positioning dependency. Success: `GetQuad(string)` inside `CardGui::Render`
58.8/frame -> 0; `RetrieveCard` 12.9/frame -> ~0 steady-state; `hasPossibleAttackers` 12.9/frame
-> 1; `Replace()` 1.1/event -> <= 1 per layout-changing event; -O2 `CardGui::Render` 0.061 ->
<= 0.045 ms/card; whole-frame Render at 20-23 permanents 1.28 -> <= 1.0 ms; console: `avg_rnd`
on the next VPK at p1perm+p2perm >= 20.

**Lane E — heuristic AI, deck DB, harness**
Items: A3, A4 (GuiHand.cpp + LegalActions.cpp are quiet after lane S — give them to this lane
since the dirty flag is fed from the same event plumbing the AI seat's `GuiHandSelf` shares;
alternatively lane D — pick ONE, this plan says E), A12's Baka sites (`AIPlayerBaka.cpp:2671-2729`),
A33's printf/DebugTrace, A36, A37, A38, A39, A51, L19 (`AIHints.cpp`, `MTGDeck.cpp`, `utils.cpp`,
`PriceList.cpp`, `GameOptions.cpp`), L20, L22. Files: `AIPlayerBaka.cpp/.h`, `AIHints.cpp/.h`,
`AIStats.cpp/.h`, `AIPlayer.cpp`, `MTGDeck.cpp`, `DeckStats.cpp`, `DeckManager.cpp`,
`TestSuiteAI.cpp/.h` (harness only — the suite runner stays green), `GuiHand.cpp`,
`LegalActions.cpp/.h`, `utils.cpp`, `PriceList.cpp`, `GameOptions.cpp`, `MTGPack.cpp`.
Gate: suite 1215/2 + **46 AI / 0** + PARSETEST; the pair-counter probe re-run (worktree-only
counters, removed before commit) on `WAGIC_SELFPLAY_DECK0=5 DECK1=12`; lane W's -O2 profile for
`AI Act`; `MTGDeck` load timing headless; the hand-glow / tap-preview manual check on desktop.
Risk: A3 changes WHICH abilities Baka considers if the multi-source cases are mishandled — the
falsifiable measure is the ranked set. Success: pairs 1,047,118 -> ~91,000 (= abilities scanned)
with **`ranked` identical (314) for the same seed**; `isReactingToClick` probes 2,094,236 ->
~91,000; planner calls 8,307 -> < 1,000; -O2 `Act` at 24-27 permanents 10 ms -> <= 2 ms; A4:
`GuiHandSelf::Update` share of the headless suite 15.1% -> < 2%; deck1 load time before/after.

**Lane F — GPT seat, region 1 + transport (AIPlayerGPT.cpp lines 1-12000, GptConfig.cpp, OptionGpt.cpp, SDLmain exit path)**
Items: A18, A19's `assemblePrompt` overload (declare the overload here; lane G calls it — or
lane G lands first and F rebases: pick G-first), A20, A23, A24, A49, L1, L2, L3, L4, L10, L11,
L12. Header: `AIPlayerGPT.h` members in an `// audit-F` block.
Gate: PARSETEST 2249/0 (+ `noAnswerClassFor` rows) + suite; **byte-identical prompts on a
`WAGIC_REPLAY` corpus record** (the render is the ask key — this is the falsifiable measure);
the bad-key endpoint test (gpt-log names the 401; record says `http_error`); the
`WAGIC_GPT_TRANSLOG=0` size probe for A18 (probe removed before commit); the kill-mid-decision
exit-status test x30 for A49. Risk: any byte drift in the board line invalidates cached answers
and the PARSETEST pins — the replay diff catches it. Success: replay prompt diff empty;
`describeAttachments` perf share 4.6% -> < 0.5% on the fake-endpoint selfplay; `mNarrationPending`
flat at 0 with the log off; 401 visible in gpt-log.

**Lane G — GPT seat, region 2 (AIPlayerGPT.cpp lines 12000-24050)**
Items: A17, A19 (the `:17398` one-liner + the memoised key/prompt sharing), A21, A22, A33's
`:18773` stderr line, L5, L6, L7 (+3 PARSETEST cases), L8, L9 (as far as PARSETEST pins allow —
legibility, lowest priority). Header block `// audit-G`.
Gate: PARSETEST (2249 + 3) + suite; translog `options_text` byte-diff on a fixed-seed replay; a
live translog showing `latency_ms -1` replay records still present for a same-turn re-poll
(the cache must still work within a turn); a sacrifice-the-token-maker-mid-plan live check for
L8. Risk: same as F (byte drift); `mAskCache.clear()` placed on the wrong boundary breaks
multi-target replays — the `latency_ms -1` check is the tripwire. Success: replay `options_text`
diff empty; planner runs per priority render on the 432-row window ~860 -> ~432 (counter probe,
removed before commit); `serializeGameState` calls per cast decision 3-4 -> 1-2.

**Lane H — JGE loaders + Vita mains**
Items: A2 (a)(b)(c)(d), A46, A47 (gated on the telemetry reading), A48, L28, A42's JGE half
(`zfsystem.cpp` miss path, `JSfx.cpp`/`JFileSystem.cpp:508,799` sound dirs), A45's two JGE twins
(`JAnimator.cpp:50`, `JResourceManager.cpp:101` — NUL-terminate them only if O3 keeps the files);
A1's one line once the owner rules. Files: `JGE/src/pc/JGfx.cpp`,
`JGE/src/JLBFont.cpp`, `JGE/src/JGui.cpp`, `JGE/src/Vitamain.cpp`, `JGE/src/JFileSystem.cpp`,
`JGE/src/pc/JSfx.cpp`, `JGE/src/zipFS/*`, `WCachedResource.cpp:161` (size accounting — coordinate
with lane D, one line).
Gate: desktop build + suite unaffected; an ASAN loader test over sample JPG/PNG + a truncated
JPEG (NULL texture, no exit, no overflow); `-funsigned-char` ASAN render of a card with 'é'
for A46; the standalone stub compile of the Vita main (`~/.gatelogs/w53W/vitastub.cpp` method)
for anything touching `Vitamain.cpp`; then a VPK (owner-initiated) and the memlog. Risk: NPOT
textures on vitaGL (eyeball one card first; keep the padding path behind a define so the A/B is
one flag); the decode-scratch is VITA-only (desktop is threaded). Success (device, memlog):
`heap_arena` flat across a 20-game session (was +1.4 MB/game); `Destroying WCachedResource` per
deck-editor page ~3x fewer; no `bad_alloc` in 50 games; and, with A1: `ram_free` drops ~96 MB at
boot. A47: `avg_swp` explains the pacing before any change lands.

**OWNER-DECISION list** (trade-off in two sentences each)
- **O1 (A1) `_newlib_heap_size_user = 224 MB` in Vitamain.cpp.** It moves ~96 MB from vitaGL's
  never-used RAM pool to the heap that actually runs out, ending the ~30-game crash horizon.
  The pool left (~62 MB) still exceeds the 48 MB cache cap plus VRAM, but it is a memory
  re-partition of the console and nobody has run it on hardware.
- **O2 (A16) gate `runParseSelfTest` out of the Vita/PSP/Android builds** (`WAGIC_GPT_PARSETEST_
  BUILD` or `!VITA && !PSP && !ANDROID`). ~1.4 MB of dead code+literals leaves the handheld
  binaries and the desktop TU compiles in 59 s instead of 152 s. The desktop gate must keep
  reporting 2249/0 and the Vita build gains a define — a release-build-flag change, his rule.
- **O3 (A50) delete the dead modules** (AIPlayerBakaB, Navigator, CardSelectorSingleton,
  GuiCardsController, GuiFrame layer, JGE JAnimator/JResourceManager/JTTFont/JParticle/
  Downloader/JNetwork/JSocket/dead mains) and drop `NETWORK_SUPPORT` from the SDL build. Smaller
  self, one fewer per-frame layer, two latent OOB writers gone, zero behaviour change. It closes
  the door on the network feature as it stands (its registrar is empty) — his call whether that
  door stays.
- **O4 (lane S) `kMaxOptSources` = 14.** His option-preserving autotap NEVER engaged in the
  vpk11 game (16 candidate producers) — raising the cap makes his spec apply on late boards.
  Lane S measured `refineForOptions` at 0.08 ms with the cap; the cost above the cap is unmeasured
  (<=64 x <=512 combos per A7's loop bounds) and should be measured before choosing a number.
- **O5 (D28) RenderSpell O(n^2) + Arena-style grouping.** Measured this week: `RenderSpell`
  0.13 ms/frame at -O2 (18-44 inner iterations/frame; grows with attached spells, not creatures —
  a Nazgul swarm does not feed it; EST 1.3-2.6 ms on the Vita), and `CardGui::Render` 0.061
  ms/card of which ~88% is draw calls that only drawing FEWER cards removes. Lane D takes the
  per-card resolution share; the draw-call share and the pile problem (34-Vampire boards, 40-row
  menus) remain the UI redesign he reserved.
- **O6 `WAGIC_FIZZLELOG` in every Vita alpha build** (`CMakeLists.txt:294`): 2,146 memory-card
  writes per session from the resolve path. Keeping it keeps the fizzle hunt's evidence; dropping
  it applies his diagnostics-out-of-release rule and removes a measurable per-resolve stall.
- **O7 Vita `-std=c++11` vs the gnu++14 pin** (`CMakeLists.txt:75`). Aligning the Vita to
  gnu++14 removes a build-only-breaks-on-the-console trap 20 minutes into `AIPlayerGPT.cpp`. It
  is a toolchain flag on the release build, so it is his; vitasdk's GCC supports it.
- **O8 (A35) the second RNG stream for the heuristic AI.** It makes shuffles seat-independent
  (a Baka-vs-GPT replay of a Baka-vs-Baka transcript keeps its draws) and removes the four
  unrecordable `std::rand()` sites. Every pre-change transcript stops replaying and seeded
  fixtures that depend on AI draws need re-baselining — a corpus-compatibility break.
- **O9 (A39) AIStats.** Its table halves on every save and mostly answers `tooSmallCountsForTrue`,
  so the feature is nearly inert while paying a per-damage-event scan and a flash write per match.
  Lane E fixes the I/O either way; whether the feature stays or goes is a product call.
- **O10 (L26) `NOSTARSDYNAMIC` as the Vita default** (`GuiMana` particle systems per engaged mana).
  Fewer particle systems stepped per frame on the console. It changes what the mana display looks
  like — UI.
- **O11 (A47) how the 30 fps cap is expressed** (sleep vs swap interval) once `avg_swp` says
  what the swap costs. If vsync is already the cap, the sleep only ever adds a frame in the
  33-50 ms regime; the change is a policy on the console's frame pacing.

**Carried (LOW, not laned):** L1-L29 as tabled; a lane that already owns the file may take a
one-liner (L15/L16/L17/L18 -> B/C; L23/L24/L25/L26/L27 -> D; L19/L20/L22 -> E; L1-L12 -> F/G;
L28 -> H). Also carried as next-wave lanes rather than LOW: **A15(b)** (shared primitive) and
**A32** (per-model parsed-line cache) — both projects with the largest blast radii in the audit
and the largest steady-state Vita heap/CPU upside after this wave's fixes are measured.

---

## 4. Contradictions and gaps

### 4.1 Where slices disagree — and which evidence wins
1. **Lane S's `kMaxOptSources` no-op vs slice 5's oracle-on-a-timer (A4).** Both are right about
   different things. Lane S's own leg table puts `refineForOptions` at 0.08 ms (3%) — the cap is
   NOT a cost item, it is a spec-applicability item (his autotap spec never engaged on his real
   board) -> O4. Slice 5's timer finding is the remaining cost: lane S's post-fix residual is
   0.44 ms/tick (-O0) fired 4x/s on a static board, and slice 5's 15.1% suite share is the
   un-throttled version of the same work (suite `dt`=1.0). Winner on cost: slice 5's mechanism
   with lane S's numbers; the event-gate makes the residual ~0 on a static board.
2. **Slice 7's `CardGui::Render` framing vs lane W's -O2 split (A5).** Slice 7 applied 10-20x to a
   GL-driver-bound -O0 number and concluded 12-25 ms of a 16.7 ms budget; lane W at -O2 measured
   0.061 ms/card with only ~12% in quad resolution. Lane W wins on the CEILING of what lane D can
   recover (the F2/F4 lookups are inside the unsplit 88%, share unknown); slice 7 wins on the
   MECHANISM list (its counters are exact). The draw-call remainder is D28/owner and vitaGL, which
   nobody measured — `avg_rnd` decides.
3. **Slice 7's "packless-install default on the Vita" (A43) vs lane V.** Lane V confirmed the
   deployed packs byte-for-byte (`MOM.zip 35656978` over FTP). Lane V wins; `AlternateRender`
   runs on the console only for art-less cards. A43 downgraded to LOW for the Vita.
4. **LSan totals.** 82,108 B / 5,319 allocs (slice 4, full suite), 82,092 B / 5,322 (slice 6, 296
   fixtures), 147,664 B / 9,442 (an earlier full run), 2,192,129 B / 49,373 (slice 5, a binary a
   few commits behind, ~1.8 KB/game). The two ~82 KB figures are suspiciously identical for
   different fixture counts and slice 4 notes LSan capped its record list at 5,000 — treat the
   per-game leak RATE as unresolved (280 B - 1.8 KB/game). The SITES agree across all four runs
   (SnowCost, GenericAddToGame, ATokenCreator, AACastCard::resolveSpell, parseMagicLine:2762,
   AutoLineMacro one-time), and slice 6's RSS curve (flat 161 MB for 147 tests, one 330 MB step,
   flat after) shows no per-game slope on desktop. Lane A's success measure is by-frame absence,
   not by total.
5. **Vita scaling factor** — 10-20x (brief, 7), 20x (S), 30-50x (6), none (W), and -O0 vs -O2
   (W's correction). Rule in §0; every FRAME number is EST until the memlog says otherwise.
6. **`mNarrationPending` rank** — slice 1 LOW (8/41 KB, 40 most recent games), slice 3 MED
   (29/192 KB, 40 largest seat logs). Different corpus selections, same mechanism; MED on the
   larger tail and the one-line fix.
7. **The brief's `AIPlayerGPT.cpp` line map was wrong** (slice 3): the async worker / transport /
   translog / narration machinery lives at 8146-10247, not 24000+; 24917-37134 is the PARSETEST.
   Slices 1 and 3 both audited that region; the lane split above follows the real layout
   (F = 1-12000 + GptConfig/OptionGpt; G = 12000-24050).
8. **The in-tree `Vitamain.cpp:57-63` comment vs slice 8** ("vitaGL maps that SAME heap as
   VGL_MEM_EXTERNAL — textures spill into it"). Slice 8 wins with vitaGL source
   (`mem_utils.c:600` returns 0 free for EXTERNAL) and the constant `ram_free` across three
   memlog sessions. The comment should be corrected with O1.
9. **Header comment `AIPlayerGPT.h:258-286` ("re-entered every tick while a call is in flight")
   vs slices 1 and 2**: both traced `AIPlayerBaka::Act` returning at `decisionPending`
   (`AIPlayerBaka.cpp:5034`) — per decision, not per tick. Slices win; the comment steered two
   audits toward a wrong cost model (L3).
10. **Slice 4 H2 vs slice 5 on `TokenAndAbility`**: slice 4 says `~MTGCardInstance` never deletes
    it and defers the ownership ruling to "the MTGCardInstance slice"; slice 5 read
    `MTGCardInstance.cpp` in full and did not mention the field. Nobody ruled — lane A owns the
    ruling (A9).

### 4.2 What NO slice read (the audit's blind spots)
- **The render path below `RenderQuad`**: `JGE/src/pc/JGfx.cpp` draw-primitive bodies (1121-1960,
  2890-4443), `JRenderer`, `CacheEngine.cpp`'s threaded retriever, `JLBFont` draw internals — i.e.
  the ~88% of `CardGui::Render` lane W could not split, and everything vitaGL-specific.
- **Nothing on the console.** No slice or lane measured on the Vita; lane W's telemetry ships in
  the next VPK (owner-initiated build) and is the only instrument for §2.
- **`runParseSelfTest` body** (25120-37040) — grep-scanned only (no heap, no globals, no files).
- **`GameStateOptions.cpp`'s model-fetch worker thread** (slice 7 read the file for UI; slice 3
  did not read it for thread safety); the Android Java side of the JNI transport.
- **Commander mode, human-seat play, hinted decks, multi-target-heavy decks** — never measured;
  A13, A38 and A7's `refineForOptions` bounds are arithmetic.
- **The replay/transcript loader** (`WAGIC_REPLAY`, `dumpAssert`) — lane W's two vpk12
  transcripts fail to replay (one diverges at action 94, one aborts at load) and no slice
  audited that path; A27 and A35 are candidate mechanisms, unproven.
- **The threaded suite's known concurrency pair** (lifeline / merrow_reejerey) — not audited.
- **The fd consumer behind `SDLmain.cpp:847-861`'s `RLIMIT_NOFILE` raise** ("the art burst
  exhausts 1024 fds") — fds were flat at 12 across the headless suite (art not loaded), so the
  consumer is in the threaded art path nobody exercised with a sampler; the raise hides it.
- **hge/*, tinyxml, Encoding.cpp GBK tables, JMP3/JCooleyesMP3, minizip (unlinked), the
  iOS/android/qt dirs**, `qrcodegen.cpp` (grepped only).
- **32-bit sizes** (`string` = 24 B on the Vita) are inferred from file comments, not measured;
  `sizeof(MTGCardInstance)` on the Vita is unknown.
- **`WAGIC_VALIDATE`'s loud-rejection path** — assumed by A28's fix, not read.
- **Per-`LoadTexture` heap deltas on device** — slice 8's suggested memlog addition (would say
  whether the Aug-28 `_malloc_r` corruption overwrites decode blocks) does not exist yet.
