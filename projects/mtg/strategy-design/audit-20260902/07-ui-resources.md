# Audit 2026-09-02 — slice 7: UI / render path / resource cache

Master commit 859186536. Read-only on master; measurements from worktree
`worktrees/lanes/audit-ui-resources` (branch `audit-ui-resources`, removed at the end).
All `file:line` refer to `projects/mtg/src/` (or `include/`) on master.

## Coverage

READ IN FULL (traced): CardGui.cpp, CardSelector.cpp, CardDisplay.cpp, GuiPlay.cpp, GuiHand.cpp,
GuiStatic.cpp, GuiCombat.cpp, GuiMana.cpp, GuiPhaseBar.cpp, GuiAvatars.cpp, GuiFrame.cpp,
GuiBackground.cpp, GuiLayers.cpp, DuelLayers.cpp, PlayGuiObject*.cpp, Pos.cpp, GuiCardsController.cpp,
CardSelectorSingleton.cpp, WResourceManager.cpp (+ WResourceManagerImpl.h), WCachedResource.cpp,
WFont.cpp, WEvent.cpp, WDataSrc.cpp, WFilter.cpp, WGui.cpp, GameStateDuel.cpp (incl. Tournament),
GameStateMenu.cpp, GameStateDeckViewer.cpp, GameStateShop.cpp, GameStateOptions.cpp,
GameStateAwards.cpp, GameStateStory.cpp, GameStateTransitions.cpp, GameState.cpp, GameApp.cpp,
GameLauncher.cpp, Navigator.cpp, Tasks.cpp, Credits.cpp, SimpleMenu.cpp, SimpleMenuItem.cpp,
SimplePad.cpp, SimplePopup.cpp, MenuItem.cpp, IconButton.cpp, InteractiveButton.cpp,
TextScroller.cpp, StyleManager.cpp, DeckEditorMenu.cpp, DeckMenu.cpp, DeckMenuItem.cpp, DeckView.cpp,
GridDeckView.cpp, CarouselDeckView.cpp, StoryFlow.cpp, ObjectAnalytics.cpp, NetworkPlayer.cpp.

NOT READ: qrcodegen.cpp (third-party, only grepped for allocation patterns — nothing found);
the JGE side of the render path (JRenderer/JGfx, CacheEngine.cpp's threaded retriever, JLBFont) — out of
slice, so draw-call cost below the `RenderQuad` boundary is not assessed; LegalActions.cpp bodies beyond
the two oracle predicates GuiHand calls.

Method notes: Vita builds `-O2 -g` without `_DEBUG`/`DOLOG` (worktrees/psp/CMakeLists.txt:74, read only),
so `DebugTrace`/`LOG` are no-ops there — none of the findings below depend on them. The desktop
dev binary is `-g` with NO `-O` flag (Makefile.sdl:19), so the perf profile below is directional
(unoptimised STL inflates the string/map share) — treat the ordering as real, the percentages as an
upper bound.

## Measurements

MB-1 (VERIFIED, micro-benchmark `scratchpad/bench.cpp`, g++ -O2, i7-7700HQ, 2M iterations):

| per-call cost | ns |
|---|---|
| `RetrieveCard`-style name build: `std::stringstream` in `MTGCard::getImageName()` + `filename.reserve(4096)` | 680 |
| same with `snprintf` into a 32-byte stack buffer, no reserve | 222 |
| `GetQuad("white")` = `std::map<string,...>::find` over the 42 managed quads | 109 |

The Vita runs at 444 MHz ARM with a much smaller cache; a 10-20x multiplier is the usual rule of
thumb, i.e. ~7-14 µs per battlefield card just to *name* the texture, before the cache lookup.

PERF-1 (VERIFIED, `perf record -F 999 -g`, 150 s windowed selfplay `WAGIC_SELFPLAY=1 WAGIC_AI=baka
WAGIC_FASTCLOCK=0.1`, desktop -O0 build; raw data `~/.gatelogs/audit-ui-perf2.data`).

Self-time top of the 51,419-sample profile (`perf report --no-children`, desktop -O0 build, so
libstdc++ accessors are NOT inlined and show up as separate symbols):

| self % | symbol |
|---|---|
| 7.71 | `std::string::size()` |
| 4.47 | `std::string::max_size()` |
| 2.33 | `std::min<unsigned long>` (string growth) |
| 2.30 | `__dynamic_cast` (+1.36 + 1.04 in the cxxabi dyncast helpers) |
| 2.27 | `std::string::_M_data()` |
| 2.05 | `std::string::compare` |
| 1.18 + 0.62 + 0.60 + 0.58 + 0.47 | `std::_Rb_tree<std::string, …>` find/less/compare (string-keyed maps) |
| 0.68 | `tolower` |
| 0.52 | `CardPrimitive::hasType(int)` |
| 0.51 | `JRenderer::RenderQuad` |

Callchain attribution (samples whose chain contains the symbol; the fp-based unwinder stops at the
first frame-pointer-less library frame, so top-level `GameApp::Render/Update` never appear and these
are LOWER bounds): `WLBFont/JLBFont::DrawString` 3.09 %, `RetrieveQuad/RetrieveTempQuad` 2.56 %,
`RetrieveCard` 1.46 %, `Player::hasPossibleAttackers` 0.33 %, `getImageName` 0.23 %.
Reading: ~20 % of ALL self time in this 150 s run (AI fast-clocked, so the engine is busy too) is
std::string / string-keyed-map machinery, and the only render-path entries visible are the resource
lookups and font rendering — consistent with F1/F2/F4 below. This is not a Vita measurement; it
ranks the mechanisms, it does not size them.

COUNT-1 (VERIFIED, instrumented worktree build — counters added to `RetrieveCard`,
`GetQuad(const string&)`, `RetrieveQuad`, `CardGui::Render`, `GuiPlay::Replace`, `RenderSpell`
inner loop, `Player::hasPossibleAttackers`, `Subtypes::find`, `DuelLayers::receiveEvent`; printed
from `DuelLayers::Render` every 300 frames; same windowed selfplay):

Run: 130 s, 22 windows x 300 frames = 6,600 frames (~51 fps), 15 self-play games, board at the
sample instants 7-25 permanents (fast-clock: ~30 frames per turn, so the per-frame averages below are
over a board that is usually smaller than the instantaneous count). `CardGui::Render` counts every
CardView drawn (battlefield + hand + stack). Raw log `~/.gatelogs/audit-ui-count3.stderr`.

| counter | per frame (mean) | peak window (25 permanents, turn 20) | note |
|---|---|---|---|
| `CardGui::Render` calls | 12.9 | 22.1 | wall time 646 us/frame mean, 1,068 us/frame peak = **48-50 us per card render** (-O0 desktop, includes the GL driver time of `RenderQuad`/`DrawString`) |
| `RetrieveCard` | 12.9 | 22.1 | exactly 1 per `CardGui::Render` (F1) |
| `GetQuad(const string&)` | 70.9 total / **58.8 inside CardGui::Render** | 110.9 / 98.8 | 4.6 string-keyed map finds per card render (F2) |
| `RetrieveQuad` | 77.0 total / 12.9 inside CardGui::Render | 86.1 / 22.1 | ~64/frame OUTSIDE card render = the zone-icon / background / phase-bar `RetrieveTempQuad` path (F2, F7) |
| `Subtypes::find` | 326 total (engine + UI) / **57.4 inside CardGui::Render** | 733 / 99.4 | 4.4 string->id map lookups per card render (F4) |
| `Player::hasPossibleAttackers` | 13.6 | 22.8 | 12.9 of them from `CardGui::Render` (F4) — each is a battlefield scan |
| `RenderSpell` inner iterations | 18.3 | 43.8 | lane W's 0.13 ms item; small |
| game events reaching `DuelLayers::receiveEvent` | 3.8 | 4.0 | fast-clock inflates this vs human play |
| `GuiPlay::Replace` calls | 4.2 | 4.6 | **1.1 per event** — confirms the unconditional call (F3) |
| `GuiPlay::Replace` wall time | 21.4 us/frame | 35.8 us/frame | **5.1 us per call** at -O0 desktop |

Reading: per-card render cost (~50 us desktop -O0) dominates everything else measured; at the Vita's
10-20x that is 0.5-1 ms per permanent, i.e. a 25-permanent board spends 12-25 ms of a 16.7 ms frame
budget in `CardGui::Render` alone — which is the "really bad frame rate at 20-30 permanents" report.
`Replace()` is comparatively cheap (F3 is re-ranked MED on this evidence).

---

## Findings (ranked)

### F1 — HIGH / PERF — `CardGui::Render` rebuilds the texture filename with a `stringstream` and a 4 KB `reserve` for every battlefield card, every frame
- `CardGui.cpp:162` → `WResourceManager.cpp:265-273` (`RetrieveCard`) → `MTGCard.cpp:73-85` (`getImageName`).
- Mechanism: every visible card calls `RetrieveCard(card, CACHE_THUMB)` once per frame. `RetrieveCard`
  does `string filename; filename.reserve(4096);` (a 4 KB heap allocation per call, freed at return) and
  `getImageName()` constructs a `std::stringstream` (locale init + heap buffer) to format an int.
  Then `RetrieveQuad` takes `string resname` BY VALUE (`WResourceManager.cpp:409`), `WCache::Get` does
  a `map<int>` find, and `WCachedTexture::GetQuad` copies the resname AGAIN (`WCachedResource.cpp:107`
  `string resource(resname)`) before a `map<string, JQuadPtr>` find. That is 3 heap strings + 1 stringstream
  + 1 map<string> lookup per card per frame — and the result never changes between frames for a
  card that is not transforming.
- Evidence: MB-1 above (680 ns vs 222 ns per call on desktop for the name build alone); the same
  `reserve(4096)` idiom is duplicated in `RetrieveCardToken` (`WResourceManager.cpp:328`), which also
  builds an `ostringstream`. On a 30-permanent board at 60 Hz this is 1,800 4 KB allocs + 1,800
  stringstreams per second on the Vita's single 444 MHz core, in the exact scenario of the two
  "really bad frame rate" reports (20-30 permanents).
- Fix sketch: (a) in `MTGCard::getImageName()` format with `snprintf` into a small stack buffer (the id
  is an int, the pattern is fixed); (b) drop `filename.reserve(4096)` (the longest set/idNNNN.jpg is
  < 32 chars); (c) cache the resolved `JQuadPtr` (and the `alternate` flag) on the `CardView`, keyed on
  `(card->getMTGId(), card->name != model name, hasCopiedToken/copiedID)` and invalidated by the
  existing `WEventCardTransforms`/`WEventCardCopiedACard`/`WEventCardFaceUp` events GuiPlay already
  receives — re-request only when the cached pointer is null (art not loaded yet) or the key changes.
  (c) alone removes the whole per-frame chain; (a)+(b) are the safe minimum if (c) is deferred.
- Blast radius: every caller of `RetrieveCard` (CardGui, GuiStatic library-top, DeckView, WDataSrc,
  GuiCombat trample avatar); no engine/rules effect. Verify: suite unchanged (art is display-only);
  `WAGIC_AUTODEMO` `wagicArtCalls` counter (`WResourceManager.cpp:248`) drops from ~cards×frames to
  ~cards; Vita frame time on a 25-permanent board.

### F2 — HIGH / PERF — Per-frame string-keyed resource lookups all over the duel HUD (~100+ `map<string>` finds per frame)
- `CardGui.cpp:241,269,291,292,501,555,565` (`GetQuad("white")` up to 7×/card),
  `CardGui.cpp:186,708,720` (`GetQuad("shadow")`), `CardGui.cpp:472` (`RetrieveTempQuad("morph.jpg")`),
  `GuiStatic.cpp:234-239` (SIX `RetrieveTempQuad("icon*.png")` per zone icon per frame, ×10 zone icons
  = 60 lookups/frame, each going through `makeID` → `ids[filename]` map<string> + cache map<int> + `mTrackedQuads`
  map<string> + a string copy), `GuiStatic.cpp:214` (`GetQuad(kGenericCardThumbnailID)`),
  `GuiHand.cpp:76`, `GuiPhaseBar.cpp` (`GetQuad("phasebar")` + `RetrieveTempQuad("fakebar.png")`),
  `GuiBackground.cpp:28-41` (2-3 `RetrieveTempQuad` per frame).
- Mechanism: `GetQuad(const string&)` is `mManagedQuads.find(quadName)` (`WResourceManager.cpp:370`);
  every call site passes a string LITERAL, so each call also constructs a temporary `std::string`.
  `RetrieveTempQuad` is worse: `RetrieveQuad(filename, ..., "temporary", RETRIEVE_NORMAL)` → `makeID`
  `ids[filename]` (map<string>) → `cache.find` → `GetQuad(offX,...,resname)` (`string resource(resname)`
  + map<string> find). None of these handles change during a duel.
- Evidence: VERIFIED (COUNT-1): 4.6 `GetQuad(string)` per card render (59-99/frame) plus ~64
  `RetrieveQuad`/frame from outside the card render (zone icons, background, phase bar) — 130-200
  string-keyed lookups per frame measured. MB-1 (109 ns per managed-quad find on desktop; the temp-quad
  path is 3 lookups + 2 string copies). Static count for a 25-card board: CardGui ≈ 25 × 3-4 = 90 finds, zone icons 60, misc ~8 →
  ~160 string-keyed finds/frame (~1-2 ms/frame on the Vita by the 10-20x rule) and ~200 short-lived
  heap strings/frame.
- Fix sketch: resolve once. `GetQuad(int id)` already exists (`WResourceManager.cpp:378`): have
  GameApp::Create record the ids of "white"/"shadow"/"extracostshadow"/"phasebar"/`kGenericCardThumbnailID`
  in a small struct (or `static JQuadPtr` members on CardGui/GuiGameZone fetched on first use — the
  managed quads are deadbolted and never evicted, and `WCachedTexture::Refresh` repoints them in place,
  exactly like `AlternateThumbQuad`'s `sAlternateThumbs` cache at `CardGui.cpp:776-790`). For
  `GuiGameZone::Render`, fetch the ONE icon the zone `type` needs, in the constructor.
- Blast radius: render-only. Verify: suite + a windowed selfplay; `perf` share of
  `std::_Rb_tree<std::string,...>::find` under `DuelLayers::Render` should collapse.

### F3 — MED / PERF — `GuiPlay::receiveEventPlus` calls `Replace()` for EVERY game event, and `Replace()` is a 4-pass re-layout with a `stable_partition` allocation
- `GuiPlay.cpp:456` (unconditional `Replace();` at the bottom of `receiveEventPlus`, reached for every
  event type that is not `WEventZoneChange`-to-play / `WEventCardTap` — those `return` early, but the
  tap path does its own work and the zone-change path already called `Replace()` at `GuiPlay.cpp:390`
  before returning; attackers/blockers/type-change/unattach/equip/controller-change each call
  `Replace()` explicitly AND fall through to the unconditional one → two full re-layouts per event).
  `Replace()` (`GuiPlay.cpp:200-297`): `stable_partition` (allocates a temp buffer), then four full
  passes over `cards`, each doing `hasSubtype/hasType/isCreature/isAttacker/...` per card.
- Mechanism: `GameObserver::receiveEvent` (`GameObserver.cpp:2386`) forwards every event to
  `DuelLayers::receiveEvent`, which fans out to all 13 layers. Damage events, life events, mana
  engage/consume events (one per mana symbol tapped), counters, draw, phase-change, GSBA checks...
  all land here. An untap step on a 25-permanent board emits ~25 `WEventCardTap` (own path) plus the
  mana engage/consume storm on every cast; each non-tap event triggers a 4-pass layout of the whole
  battlefield.
- Evidence: VERIFIED (COUNT-1): 1.1 `Replace()` per game event, 4.2 per frame in fast-clock selfplay,
  5.1 us per call at -O0 desktop (21 us/frame mean, 36 us/frame on a 25-permanent board). Small next to
  F1/F2/F4 — on the Vita ~0.05-0.1 ms per call, so it only matters in event storms (untap of a big board,
  a multi-symbol mana payment) — hence MED, not HIGH. This is the D28 "O(n²)" item's
  bigger sibling: `RenderSpell` (`GuiPlay.cpp:26-40`) is per FRAME n×s (n permanents × s attached
  spells, recursive with `begin+1`), which is cheap because s is small; `Replace()` is per EVENT and
  touches everything.
- Fix sketch: (1) delete the trailing unconditional `Replace()` at `GuiPlay.cpp:456` and let only the
  branches that change layout call it (zone-change-to-play, attacker, blocker, type/attach/controller
  change); (2) make `Replace()` lazy — set `mLayoutDirty = true` in the event handlers and run the
  relayout at most once, at the top of `GuiPlay::Update` (events are batched in
  `GameObserver::receiveEvent`'s queue, so a cast that fires 6 events becomes one relayout);
  (3) for D28's `RenderSpell`, keep a per-CardView vector of attached-spell CardViews rebuilt inside
  `Replace()` (it already partitions spells) so `Render` walks attachments directly instead of scanning
  `[begin,end_spells)` per permanent.
- Blast radius: GuiPlay only; `Replace()` also positions cards before `CardSelector::Add` (comment at
  `GuiPlay.cpp:386-389`) — keep that one synchronous. Verify: suite (layout is display-only); a
  windowed selfplay with `perf` — `GuiPlay::Replace` and `std::stable_partition` should disappear
  from the profile; human Vita play on a 20+ board.

### F4 — MED / PERF — `CardGui::Render` per-card work that is O(battlefield) or does string→id lookups every frame
- `CardGui.cpp:504`: `game->currentPlayer->hasPossibleAttackers()` is evaluated INSIDE the per-card
  render → `Player.cpp:316-327` scans the whole battlefield calling `canAttack(true)||canAttack()` per
  permanent → O(n²) `canAttack` calls per frame (25 cards → 625 `canAttack`/frame; `canAttack` is a
  dozen `has()` bit tests + `hasSummoningSickness` + `isInPlay`, `MTGCardInstance.cpp:990`). The value
  is the same for every card in the frame.
- `CardGui.cpp:590`: `card->hasType("legendary") && card->hasType("eldrazi")` in the P/T colour
  else-chain — `hasType(const string&)` (`CardPrimitive.cpp:696-700`) builds a `std::string` from the
  literal, `Subtypes::find` copies it AGAIN by value (`Subtypes.cpp:33`) and does a `map<string,int>`
  find; this runs for every undamaged, unbuffed creature every frame. Same pattern at
  `CardGui.cpp:404-412` (`hasSubtype("plains")`… for art-less cards) — 5 lookups per art-less land per
  frame, which is EVERY land in a packless install.
- `CardGui.cpp:175`: `MTGCollection()->getCardByName(card->name)` (a map<string> over the whole
  collection) per frame for any renamed card (copies, Clone effects).
- `CardGui.cpp:611-619`: `string buff`, `string starMark`, `buff = starMark + buff` per card per frame
  (SSO, no heap, but 3 string ops for a value that is almost always "").
- Evidence: VERIFIED (COUNT-1): 4.4 `Subtypes::find` (string->id map lookups) and exactly 1
  `hasPossibleAttackers` battlefield scan per `CardGui::Render`; 57-99 `Subtypes::find` per frame from
  the render path alone.
- Fix sketch: hoist `hasPossibleAttackers()` to once per frame (compute in `GuiPlay::Render` or cache
  on the observer per phase — it can only change on tap/untap/zone/phase events); replace the two
  string `hasType` calls with `hasType(Subtypes::TYPE_LEGENDARY)`/`TYPE_ELDRAZI` int ids (the
  `Subtypes` enum already has the fixed ones; `eldrazi` is a subtype — resolve its id once into a
  static); resolve `getCardByName` once per rename via the same CardView cache as F1.
- Blast radius: CardGui only. Verify: suite; perf share of `Subtypes::find` / `Player::hasPossibleAttackers`
  under `CardGui::Render`.

### F5 — MED / PERF — The focused big card (`CardSelector::Render`) is re-parsed every frame in text mode / for art-less cards
- `CardSelector.cpp:393-402`: the active card is rendered TWICE per frame (once by its layer, once here,
  `active->Render()`), then `DrawCard(npos, mDrawMode)` for up to 800 frames (`timer` is decremented
  per frame, not per dt, `CardSelector.cpp:377-378`).
- `CardGui::AlternateRender` (`CardGui.cpp:801-1100`, the path taken with DISABLECARDS or when art is
  missing — the packless-install default on the Vita) per frame: copies TWO `vector<...*>` of mod-rules
  items (`CardGui.cpp:820,834`), copies the whole formatted-text `vector<string>` (`CardGui.cpp:882`,
  `getFormattedText()` returns a `const&` that is cached — the copy is the caller's), runs `FilterCard`
  (`CardGui.cpp:863` → `1562-2118`) for every render item that has a filter — `modrules.xml` ships
  four (`creature`, `[black;green;blue]`), and `FilterCard` builds a `CardDescriptor` and does ~40
  `string::find`/`substr` per call — and builds `stringstream`s for title/id/power/toughness
  (`CardGui.cpp:955-975`). `RenderCountersBig` copies the text vector again (`CardGui.cpp:2142`).
  `TinyCropRender` (`CardGui.cpp:1104`) and `RenderBig` have the same copies.
- Evidence: reading (SUSPECTED). This is the "CardSelector focused-card re-render" the brief names.
- Fix sketch: `const vector<...>&` for the mod-rules and formatted-text vectors (4 sites); parse each
  render item's `mFilter` ONCE into a `CardDescriptor` stored on the `ModRulesRenderCardGuiItem` at
  modrules load (`ModRules.cpp:302`) and call `cd.match()` — this also removes the latent cast bug in
  F13; skip the duplicate `active->Render()` when the active card's own layer is visible (CardSelector
  sits above the play/hand layers precisely so the zoomed card draws on top — keep only that draw).
- Blast radius: CardGui text rendering, deck editor/shop big-card views (same functions). Verify:
  visual parity in text mode; suite unaffected.

### F6 — MED / PERF — (extends slice 5 H5) `GuiHandSelf::Update`: per-card `canDeclareBlocker` re-walks the attackers, plus an `ostringstream` signature per tick
- Slice 5 (05-engine-core.md H5) already measured the timer-driven `castableForDisplay`/`usableAbilityCards` refresh at 15.1 % of suite CPU and owns that fix; this entry adds only the two parts it did not cover.
- `GuiHand.cpp:229-289`: every 0.25 s: `castableForDisplay` (whole hand), `usableAbilityCards` (whole
  battlefield, already batched by W53-S), then PER battlefield card `canDeclareAttacker` (cheap) and
  `canDeclareBlocker` (`LegalActions.cpp:586-611`: walks `getNextAttacker` and calls `canBlock(attacker)`
  per attacker → O(defenders × attackers) per tick), plus an `ostringstream` signature over the hand and
  battlefield pointers (`GuiHand.cpp:263-270`) to dedupe the auto-tap plan.
- Mechanism: the refresh is time-driven, not event-driven, so a static 25-permanent board pays the
  full oracle four times a second whether or not anything changed; `canDeclareBlocker` early-outs on
  phase so its cost is confined to the declare-blockers step, but there it is quadratic.
- Fix sketch: gate the whole block on a dirty flag set from `receiveEventPlus` (zone change, tap,
  phase change, mana pool change, damage) plus a cheap "phase == combat blockers" tick; compute the
  attacker list once per refresh and pass it into `canDeclareBlocker` (or compute `canBlockNow` only
  for the defending player's creatures); replace the `ostringstream` signature with a hash of
  `(focused, pool string, hand nb_cards, tapped bitset)` or simply the dirty flag.
- Blast radius: display-only signals (`castableNow`, `canAttackNow`, ...). Verify: the hand glow and
  tap-preview still update on the same events (there is no suite coverage — manual on desktop).

### F7 — MED / PERF — `GuiGameZone::Render` scans the battlefield twice per zone icon per frame
- `GuiStatic.cpp:224-225`: `battlefield->hasAbility(SHOWFROMTOPLIBRARY)` and the opponent's
  `SHOWOPPONENTTOPLIBRARY` (`MTGGameZones.cpp:1246`, linear over all permanents) computed for ALL ten
  zone icons, but only used by the two library icons (`type == GUI_LIBRARY`, line 291). 20 battlefield
  scans per frame on top of F2's 60 string lookups.
- Fix sketch: move both `hasAbility` calls inside the `type == GUI_LIBRARY` branch (and ideally cache
  them per frame in GuiAvatars). Trivial, zero risk.
- Verify: library-top reveal still shows with a "play with the top card revealed" effect.

### F8 — MED / MEM-SAFE — `WGuiKeyBinder::populateKeyBindingList` leaks every `OptionKey` row on "Load Defaults..."
- `WGui.cpp:2536-2549`: `items.clear()` then re-`Add`s fresh `OptionKey` objects. `WGuiMenu` owns
  `items` (deleted in `~WGuiMenu`, `WGui.cpp:838-842`), so clearing without deleting leaks the old rows
  (one per key binding, ~15-25 objects) each time the button is pressed (`ButtonPressed(3)`,
  `WGui.cpp:2262-2269`). `currentItem` is also not reset, so it can point past the new list.
- Fix: delete `items` before clearing (a `clearItems()` helper on WGuiMenu), `currentItem = -1`.
- Verify: press "Load Defaults..." twice under the ASAN build (`Makefile.sdl-asan`) — LeakSanitizer
  reports the `OptionKey` allocations today.

### F9 — MED / MEM-SAFE — `StoryFlow::parse` hands TinyXML an un-terminated buffer
- `StoryFlow.cpp:610-614`: `char *xmlBuffer = NEW char[size]; fileSys->ReadFile(xmlBuffer, size);
  doc.Parse(xmlBuffer);` — `TiXmlDocument::Parse` takes a NUL-terminated C string; the buffer is exactly
  `size` bytes with no terminator, so the parser reads past the allocation until it finds a zero byte
  (heap over-read; on a small heap like the Vita's it may parse garbage or fault). `StyleManager.cpp:928-932`
  does the same job correctly via `readIntoString`.
- Fix: `NEW char[size + 1]; xmlBuffer[size] = 0;` or switch to `readIntoString` like StyleManager.
- Verify: ASAN build, enter Story mode with any campaign — reports heap-buffer-overflow read today.

### F10 — MED / CRUD — Dead navigation/selector scaffolding still compiled into every build
- `Navigator.cpp` (782 lines, `Navigator` class) is referenced ONLY by `CardSelectorSingleton.cpp`,
  and `CardSelectorSingleton::Create/Instance/Terminate` have zero callers (`DuelLayers.cpp:83` does
  `NEW CardSelector` directly). Both files + headers are dead: 817 lines and one static `CardZone`
  map per would-be instance.
- `GuiFrame.cpp`/`GuiFrame.h`: the whole class body is commented out (`GuiFrame.cpp:15-73`), yet
  `DuelLayers.cpp:112` allocates one per duel and it takes a slot in the per-frame layer loop
  (`DuelLayers::Render/Update` call empty virtuals).
- `GuiCardsController.cpp` is an empty translation unit for a header-only class with no users.
- `PlayGuiObjectController::CheckUserInput` (`PlayGuiObjectController.cpp:97-165`) is a 60-line
  commented-out body returning `false`.
- Fix: delete Navigator.{cpp,h}, CardSelectorSingleton.{cpp,h}, GuiCardsController.{cpp,h}; remove
  the GuiFrame layer from DuelLayers (or delete the class); drop the commented body. Update
  wagic-SDL.pro / Vita CMake source lists.
- Verify: build all targets; suite; PARSETEST.

### F11 — MED / CRUD+PERF — `WEventEngageMana` / `WEventEngageManaExtra` constructors do 12 string-keyed `hasType("snow")` lookups per mana produced
- `WEvent.cpp:205-262`: two identical 60-line blocks; each of the 12 `if` lines re-evaluates
  `card->hasType("snow")` (string → `Subtypes::find` map lookup, see F4) and
  `card->controller()->getManaPool() == destination`. Every mana symbol produced in the game constructs
  one of these events (`ManaCost`/`AManaProducer` path), so a 6-land tap = 72 map<string> lookups —
  inside an event CONSTRUCTOR that also mutates player state (side-effecting ctor: a `WEventEngageMana`
  built for any other reason would double-count snow mana).
- Fix sketch: `const bool snow = card->hasType(Subtypes::TYPE_SNOW)` (int id) once; pick the pool
  owner once (`Player * p = destination == controller pool ? controller : opponent`); index
  `snowMana{G,U,R,B,W,C}` by colour with a small switch; share one helper between the two ctors. Better:
  move the snow accounting to the ManaPool `add` site where it belongs.
- Blast radius: snow-mana cards (Rimewind etc.). Verify: suite (snow tests exist under
  `bin/Res/test/`? — grep `snow` in test/ before touching), PARSETEST unchanged.

### F12 — MED / PERF — Cache-miss path builds an `ostringstream` even though `LOG` is a no-op
- `include/WResourceManagerImpl.h:88-105` `RequiresOldItemCleanup()`: whenever the cache is over
  any limit it constructs a `std::ostringstream`, formats a message, and passes it to `LOG(...)`, which
  expands to `{}` on every non-PSP build (JLogger.h:9-12). `Cleanup()` (`WResourceManager.cpp:1483`)
  calls it once per miss plus once per eviction iteration, so on the Vita in a steady over-limit state
  (cache 48 MB / 800 items, cap reached mid-match) every new card art costs several stringstream
  constructions on the main thread before the load even starts.
- Also `RemoveOldest` (`WResourceManager.cpp:1100`) is a linear scan of the whole cache map per
  eviction (fine at 800; it is O(n) per miss, so a burst of 30 new arts is 30×800 iterations).
- Fix: wrap the stream in `#if defined(DOLOG) || defined(PSP)`; (optional) keep an intrusive
  LRU list so eviction is O(1).
- Verify: perf under a fresh-deck selfplay (many first-time arts) — `RequiresOldItemCleanup` vanishes.

### F13 — LOW / MEM-SAFE — `CardGui::FilterCard` type-puns a `CardPrimitive*` as `MTGCardInstance*`
- `CardGui.cpp:1566`: `MTGCardInstance * card = (MTGCardInstance*) _card->data;` — `data` is a
  `CardPrimitive*`; `MTGCardInstance` derives `CardPrimitive, MTGCard, Damageable`
  (`include/MTGCardInstance.h:28`). `cd.match(card)` (`CardDescriptor.cpp:230-410`) then reads
  `card->foretellTurn`, `card->kicked`, `card->counters`, `card->getPower()` etc. — fields that live
  PAST the end of the real `CardPrimitive` object — whenever a filter sets the matching comparison
  mode. The shipped `modrules.xml` filters (`creature`, `[black;green;blue]`) only touch `types`/`colors`
  (in the `CardPrimitive` sub-object, so it works by accident); any user modrules with `power=`,
  `foretold`, `kicked`, ... reads garbage.
- Fix: parse into a `CardDescriptor` and match against `MTGCard*`-safe fields only, or pass the real
  `MTGCardInstance*` when the caller has one (`AlternateRender` receives `MTGCard*` and does
  `dynamic_cast<MTGCardInstance*>` for `zpos` already — reuse it). Folds into F5.
- Verify: ASAN build with a test modrules `filter` of `[power>=2]` on the big-card render.

### F14 — LOW / LEAK — `GameStateDeckViewer` and `GameStateMenu` teardown gaps
- `GameStateDeckViewer.cpp:67-98`: the destructor deletes 10 of the 11 `InteractiveButton`s —
  `menuButton` (`:58`) is never deleted (one object per process; also the destructor path deletes
  `myCollection->parent` which is `playerdata->collection`, and `playerdata` itself is only freed in
  `End()` — if the app exits from inside the editor `playerdata` leaks and, worse, its collection was
  freed through `myCollection->parent`, so a later `PlayerData` destructor would double-free; today
  `GameApp::Destroy` does not call `End()` first).
- `GameStateMenu.cpp:164` (`Destroy`) and `:362` (`End`) both `Release(bgTexture)` without nulling it;
  `Release` (`WResourceManager.cpp:457-475`) finds the entry by raw pointer compare — after `End()`
  freed it, `Destroy()` releases a stale pointer, which is harmless unless the address was reused by a
  new texture (then an unrelated locked texture is unlocked/deleted). Set `bgTexture = NULL` after Release.
- Verify: ASAN/LSAN at exit from the editor and from the menu.

### F15 — LOW / MEM-SAFE — fixed `char` buffers fed with user/asset-controlled strings via `sprintf`
- `GameStateDuel.cpp:3180` `renderScoreTable`: `char buffer[256]` + `sprintf(buffer, _("Player: %s (%i/%i)")…, deckName)`
  (three `snprintf` sites next to it were already fixed; six `sprintf` sites at `:3206-3252` were not).
  Deck names come from user deck files; a >220-char `#NAME:` overflows.
- `WFilter.cpp:579-623`: five `char buf[4068]` `getCode()` sites compose recursively (`"%s&%s"`) — the
  deck-editor filter UI can stack ~10 clauses, each `type:%s;` with an arbitrary subtype string; no
  bound check.
- `GameStateShop.cpp:924-936` `ShopBooster::getName`: `char buffer[512]` is returned UNINITIALISED when
  `!mainSet && !pack` (empty setlist path) — garbage menu title.
- `GameStateAwards.cpp:344`: `counts[c->setId] += count` with `setId` possibly `-1`
  (same class as the W53-ASAN heap-overflow fixed in `WDataSrc.cpp:361`) — writes one int before a
  `calloc` block; `:394` prints `strong->` where `tough->` was meant (wrong card name, no crash).
- `WResourceManager.cpp:671-703` `graphicsFile`: `char buf[512]` with `themes/%s/%s` of theme name +
  filename (theme dir names are user-created).
- Fix: `snprintf` with `sizeof`, or `std::string` concatenation; clamp `setId`.
- Verify: ASAN + a 300-char deck name / a 12-clause filter.

### F16 — LOW / MEM-EFF — small per-duel / per-frame allocation churn
- `DuelLayers.cpp:185-198`: every zone change no layer claims (to graveyard/library/exile when no
  GuiGameZone takes it — e.g. tokens dying, library shuffles) pushes a `NEW Pos` onto `waiters`, freed
  only in `~DuelLayers`. ~48 B per event, grows for the whole duel (a 60-turn mill game = a few KB).
  Free them when `mTrash->cleanup()` runs at `MTG_PHASE_BEFORE_BEGIN` (`DuelLayers.cpp:207`) — the
  `card->view` pointers they replace are re-pointed by the next `CardView` ctor anyway.
- `CardDisplay.cpp:84`: `CardDisplay::Update` copies the whole zone (`vector<MTGCardInstance*> newCD`)
  every frame while a graveyard/library display is open (60-card library → 480 B alloc/frame). Compare
  in reverse without the copy.
- `WResourceManager.cpp:50-55` `sMissCounts`/`sMissTime`: two `map<int,…>` entries per failing card id
  (thumb + full = 2 ids), never cleared except on a later success; a packless install (every card
  404s) holds ~4 map nodes × ~48 B per distinct card seen, for the process lifetime. Bound it (clear
  on `ClearUnlocked`, or store a single `unsigned char` strike count in one map).
- `GuiMana.cpp:115,293`: each engaged mana allocates a `hgeParticleSystem` (60 emitters); `Render`
  caps at 20 icons but `Update` still steps every system. `NOSTARSDYNAMIC` already exists — consider
  it the Vita default in `options`.

### F17 — LOW / CRUD — leftovers
- `GameStateDuel.cpp:1386-1435`: when `JGE_BTN_MENU` is pressed while `menu` already exists (the LLM
  patience prompt), the `MENUITEM_TASKBOARD` row is appended to the EXISTING menu every press
  (`menu->Add` outside the `if (!menu)`), producing duplicate rows.
- `StoryFlow.cpp:123,157`: `text.replace(pos, pos + 6, …)` passes an END position as the COUNT —
  wrong text replaced whenever `${SET}`/`${CARD}` is not at offset 0.
- `GameStateMenu.cpp:651,773`: `vector<ModRulesOtherMenuItem*> items = gModRules.menu.other;` copied
  per Update AND per Render; `GameApp.cpp:179` same for `cardgui.background` (once, harmless).
- `NETWORK_SUPPORT` is defined in the SDL build (Makefile.sdl:17) so `NetworkPlayer.cpp`, the
  `MENU_STATE_NETWORK_*` branches and `RemotePlayer` compile in although the feature is non-functional
  (`RegisterNetworkPlayers` is empty) — dead weight in every state machine; out of my slice to rule on
  JNetwork, but the UI-side branches can go with it.

### F18 — LOW / PERF+CRUD — Sound-effect misses: `getSample()` never memoises a miss, `PlaySample("")` probes the sfx directory, and every miss walks the cache Cleanup path
- `MTGCardInstance.cpp:2263-2316` (`getSample`), `include/WResourceManager.h:76-80` (`PlaySample`),
  `ActionStack.cpp:596`, `WResourceManager.cpp:1321` (`Cleanup()` before every load attempt),
  `:1380-1420` (`LoadIntoCache` strike ledger, `kMissTombstoneAt = 5`, retry spacing PSP/VITA-only).
- Mechanism (this answers "what triggers the `Destroying WCachedResource` churn"): the dev log's
  blank-name `Destroying WCachedResource:` lines are SAMPLE misses, not textures. On every cast
  `getSample()` tries `<subtype>.wav` for each type, then `<basic ability>.wav` for each ability, then
  `<type0>.wav`; the shipped `sound/sfx/` has ~20 files, so most cards miss every probe, `sample` stays
  "", and the whole chain re-runs on the NEXT cast of the same card (a hit is memoised, a miss is not).
  `PlaySample(getSample())` then calls `RetrieveSample("")` → `AttemptNew("")` → `NEW WCachedSample`,
  open `sound/sfx/` (a directory), fail, delete — the blank-name destroy. Each miss also runs
  `WCache::Cleanup()` (`:1321`) before the load attempt. Strikes tombstone an id after 5 failures, but
  `ClearUnlocked()` (every return to the menu, `GameStateMenu.cpp:193`) erases the tombstones while
  `sMissCounts` keeps the strike count, so each new match re-probes each missing name once more.
- Evidence: VERIFIED from `~/.gatelogs/audit-ui-count3.stderr` (130 s, 15 games): 345 `Destroying
  WCachedResource` lines — 143 blank-name (the `""` sample), ~100 `swamp.wav`/`land.wav`/`basic.wav`/…,
  the rest card art (`RV/NNNN.jpg` ×5 each = the desktop 5-strike ledger with no retry spacing). ~23
  failed file probes per game; on the Vita each probe is a synchronous FS open on the render thread.
- Fix sketch: memoise the miss (`sampleResolved = true` on `MTGCardInstance`, or store a sentinel in
  `sample`); early-return in `PlaySample` on an empty name; on desktop give texture misses the same
  `kMissRetryMs` spacing as PSP/VITA (or drop the 5-strike rule to 1 for `ENOENT`, which is not
  transient). Also `Cleanup()` only needs to run when a load is actually about to happen — move it
  below the `RETRIEVE_EXISTING` check into `LoadIntoCache`.
- Blast radius: sfx playback only. Verify: dev log shows one probe per missing name per process;
  `AttemptNew failed to load (404):` with a blank name disappears.

## What I checked and found sound (no finding)
- The `Add(0, "literal")` null-button trap the Vita crashed on: the only other `Add(0, …)` is
  `GameStateStory.cpp:68`, already `Add(0, string(...))` → resolves to `SimpleMenu::Add(int, const string&)`;
  every other 2-arg `Add` uses an enum constant (not a null-pointer constant) — safe.
- `GuiCombat::resolve` frees the `DamageStack` on the empty path; `GuiCombat::reseatCursor` guards the
  NULL cursor (2026-08-31 fix present). `Credits` async flush + `ensureFlushed` is correctly bounded.
- `AlternateThumbQuad` cache is bounded (8 entries) and pinned correctly on non-PSP.
- `WCache::Retrieve/Get` locking is consistent; `RETRIEVE_MANAGE` entries are unlinked from the
  evictable cache before being deadbolted (no double-free between `cache` and `managed`).
- `GameStateMenu::Start` calls `ClearUnlocked()` on every return to the menu — so cross-match texture
  growth is bounded per menu visit; within a tournament (End();Start() without the menu) the cache
  grows to its 48 MB / 800-item Vita cap and then evicts — by design, not a leak.

## Suggested order of work
F1 + F2 + F4 (same file, same fix pattern: resolve-once handles on CardView / statics — the measured
~50 us per card render is where the Vita's frame budget goes) → F7 (one-line hoist) → F3 (lazy
`Replace`, D28's RenderSpell folds in) → F12 → F5/F13 (parse modrules filters once) → F6 (after
slice 5's H5 lands) → F8/F9 (ASAN-verifiable) → F10 (delete dead code) → F11 → F18 → F14-F17.

## Measurement artefacts
Instrumented worktree (removed): counters + `steady_clock` accumulators around `CardGui::Render`,
`GuiPlay::Replace`, `RenderSpell`, `RetrieveCard`, `GetQuad(string)`, `RetrieveQuad`, `Subtypes::find`,
`Player::hasPossibleAttackers`, `DuelLayers::receiveEvent`, printed every 300 frames from
`DuelLayers::Render`; logs `~/.gatelogs/audit-ui-build{,2,3}.log`, `audit-ui-count3.{stdout,stderr}`,
`audit-ui-perf2.{data,script,stdout,stderr}`. Suite/PARSETEST were NOT re-run on the instrumented
binary (no behaviour change; counters only).
