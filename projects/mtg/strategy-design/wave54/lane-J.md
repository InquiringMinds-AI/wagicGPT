# Wave-54 lane J — audit lane D: render path (CardGui / GuiPlay / resource cache / events)

Branch `w54-lane-J`, worktree `worktrees/lanes/w54-J`, seeded from master dab9f72f4.
Commit: **64ec42193** (single commit; 23 files). No `bin/Res` changes, no new sources
(no qmake).

## What shipped (ledger items, all tagged `#W54-J (An)` in code)

- **A5** `CardGui::Render` per-card per-frame resolution:
  - the managed HUD quads (`"white"` x up to 7, `"shadow"` x up to 3, `"extracostshadow"`,
    the five `c_*` land icons) are resolved once per resource manager through `hudQuad()`
    (`CardGui.cpp`, audit-J block). **Invalidation contract**: only MANAGED quads are held —
    they are deadbolted (never evicted) and `WCachedTexture::Refresh` repoints their texture in
    place, so the pointer is valid for the manager's life; the cache is keyed on the manager
    pointer (the singleton is torn down only at `GameApp` exit). Evictable/temporary quads and
    **card art are NEVER cached** — `WCache::isLocked()` ignores tracked quads, so a held
    `JQuadPtr` would outlive its texture after `RemoveOldest`/`ClearUnlocked`; and on PSP/Vita
    a miss returns the generic back, not NULL, so the ledger's "re-request while NULL" rule
    would freeze a late-loading card on its back face. `RetrieveCard` therefore still runs
    once per card per frame (it also keeps the on-screen textures' LRU stamp fresh) and was
    made cheap instead: no 4 KB `reserve`, `getImageName` via `snprintf` (both card and token
    paths), `RetrieveQuad` takes `resname` by const reference, `WCachedTexture::GetQuad` keys
    by pointer instead of copying.
  - `Player::hasPossibleAttackers()` (a battlefield scan, `canAttack` x2 per permanent) ran
    for EVERY card EVERY frame; it now sits behind the cheap per-card predicates and runs only
    for a creature of the acting HUMAN inside its combat window. Pure reorder of pure reads —
    the mask drawn is identical.
  - `hasType("legendary")&&hasType("eldrazi")` and the five `hasSubtype("plains"...)` string
    lookups (each took the subtype-list mutex in `MTGAllCards::findType`) are resolved ids.
  - the token/copy/exerted tag is built without three `std::string` temporaries per card.
  - **Disable switch** (one switch for every cache this lane added, including A25):
    `-DWAGIC_RENDER_NOCACHE`, or `WAGIC_RENDER_NOCACHE=1` in a development build
    (`_DEBUG`/`WAGIC_DEVLOGS`) — `wagicRenderCacheOff()` in `CardGui.h`.
- **A25** `GuiPlay`: every event still marks the layout dirty (the old unconditional trailing
  `Replace()` had exactly this coverage — nothing narrowed), relayout happens at most once at
  the top of `Update` and once at the top of `Render`; the synchronous `Replace()` before
  `CardSelector::Add` is kept (position is the selector's zone cue). NOCACHE restores the eager
  call.
- **A40** `WEventEngageMana`/`Extra`: one `hasType(Subtypes::TYPE_SNOW)` + one owner pick in a
  shared helper (was 12 string-keyed `hasType("snow")` per mana produced, two identical
  blocks). Same pool chosen as before (controller's pool, else opponent's, else none).
- **A41** `RequiresOldItemCleanup` builds its `LOG` stream only under `DOLOG || PSP`.
  (`RemoveOldest` O(n) left as is — bounded by `MAX_CACHE_OBJECTS`=300.)
- **A42 game-layer half**: `PlaySample("")` returns early (the blank-name "Destroying
  WCachedResource" lines); `getSample()` memoises its miss (`sampleResolved`, initialised in
  `initMTGCI`); the `sMissCounts`/`sMissTime` strike ledger resets with the tombstones in
  `ClearUnlocked`. `Cleanup()` was already below the `RETRIEVE_EXISTING` return — no change.
  NOT done here: id-0/token tombstoning at the game layer, the zip miss path and the sound
  mkdir chain (JGE half, lane H).
- **A43** `AlternateRender`/`TinyCropRender` read the mod-rules vectors by `const&` (three
  copies per art-less card per frame); `FilterCard` matches the REAL `MTGCardInstance` when
  handed one and, on a bare `MTGCard` (deck viewer/shop), refuses the instance-only comparison
  modes instead of reading past a `CardPrimitive` punned as an instance. The "duplicate
  `active->Render()`" in `CardSelector::Render` was NOT removed: it is the focused card
  re-rendered on top of its layer (z-order), and the second draw is the big side panel — two
  different renders. Parse-once of the filters at modrules load not done (the shipped filters
  are `creature` / `[black;green;blue]`).
- **A44** `WGuiKeyBinder::populateKeyBindingList` deletes the rows it drops, resets
  `currentItem`. Safe re-entrancy: the only re-entrant caller is `confirmMenu`'s listener,
  which runs after this list's own item loop; `JGuiController::CheckUserInput` returns
  straight after the listener call.
- **A45** `StoryFlow::parse` NUL-terminates the TinyXML buffer; the two `replace(pos, pos+N)`
  end-as-count bugs fixed. JGE twins -> lane H / O3.
- **L23** `GuiGameZone::Render`: the SHOWFROMTOPLIBRARY/SHOWOPPONENTTOPLIBRARY scans run only
  for `GUI_LIBRARY`.
- **L24** deck-viewer teardown: `~GameStateDeckViewer` frees the wrapper then `playerdata`
  (which owns the collection — the old dtor freed `myCollection->parent`, i.e. the same
  object PlayerData's dtor frees), `menuButton` freed, and `playerdata` is now initialised in
  the ctor (it was never NULLed; the first `-O2` A/B run crashed in `~PlayerData` at
  `GameApp::Destroy` — caught, fixed, re-run clean). `GameStateMenu` nulls `bgTexture` after
  `Release` in both `End()` and the dtor.
- **L25** `snprintf`/`std::string` at the listed sites (`GameStateDuel` 9 sites, `WFilter`
  5 returns, `ShopBooster::getName` initialised + bounded, `graphicsFile` left: 512 with a
  theme name is fine but not touched — `WResourceManager.cpp:671-703` NOT converted); awards
  `counts[setId]` clamped for `setId == -1`; "Toughest" names `tough`, not `strong`.
- **L26** `CardDisplay::Update` indexes the zone from the back instead of copying it per frame.
  `DuelLayers` `waiters` Pos objects NOT freed at `mTrash->cleanup()`: the Pos is what
  `card->view` points at for every card that left the GUI zones, and `GuiPlay::
  receiveEventPlus` copies `*(event->card->view)` when the card returns — freeing them
  per-turn is a use-after-free, not a leak fix (it is bounded per duel, freed in the dtor).
  `GuiMana` particle systems -> owner item O10.
- **L27** the task-board row is added only to a menu that this MENU press built (`builtMenu`
  local) — no more duplicate rows while another menu (the patience prompt) is open;
  `gModRules.menu.other` read by reference in `GameStateMenu` Update/Render/Start.

## Gate (worktree, -O0, `make -f Makefile.sdl`, committed tree)

- suite: **1219 tests / 2 failed** (`lifeline.txt`, `merrow_reejerey.txt` = the known
  concurrency-only pair), **49 AI / 0**, 0 timed out (`~/.gatelogs/w54J/gate-suite.log`).
- PARSETEST: **2258 / 0** (no parse/render-annotation strings touched; no new cases).
- ASAN (`Makefile.sdl-asan`, `bin-asan/wagic`): 150 s offscreen self-play (deck 164 mirror,
  Baka, 183 turns) + SIGINT teardown through `GameApp::Destroy` — **0 AddressSanitizer
  errors**; LeakSanitizer: 5005 indirect allocations / 80 KB at exit, **0 direct leaks**, none
  in the touched files (`~/.gatelogs/w54J/asan.log.*`).

## Counters (slice 7's set) and the -O2 offscreen profile

Rig: lane W's `apply_prof.py` + `~/.gatelogs/w54J/apply_counters.py`, applied on top of the
commit for measurement only and reverted (`git checkout`) before the final gate — nothing of
it is in the tree. Build: `Makefile.sdlO2` (= Makefile.sdl + `-O2`, `objs-o2`, `bin/wagic-o2`,
deleted afterwards). Run: `SDL_VIDEODRIVER=offscreen`, `WAGIC_SELFPLAY_DECK0=164 DECK1=164`,
`WAGIC_AI=baka`, `WAGIC_FASTCLOCK=0.0333`, 80 s legs **interleaved** after/before/after/before
then after+NOCACHE (`~/.gatelogs/w54J/run-*.log`, `prof-*.txt`). Board: avg 11-13 permanents,
max 28-31. Per-frame means over the whole leg (two legs each, `before` / `after`):

| counter (per frame) | before | after | after + `WAGIC_RENDER_NOCACHE=1` |
|---|---|---|---|
| `CardGui::Render` calls (cards drawn) | 13.0 / 12.6 | 12.9 / 12.1 | 13.1 |
| `GetQuad(string)` INSIDE `CardGui::Render` | **59.6 / 58.1** | **0.0 / 0.0** | 34.2 |
| `GetQuad(string)` total (all layers) | 71.7 / 70.1 | 12.1 / 12.0 | 46.2 |
| `RetrieveCard` | 13.0 / 12.6 | 12.9 / 12.1 (= cards drawn, BY DESIGN — see contract) | 13.1 |
| `RetrieveQuad` total | 17.6 / 17.2 | 17.5 / 16.7 | 17.7 |
| `Player::hasPossibleAttackers` total | **13.8 / 13.4** | **0.8 / 0.9** (inside Render: 13.0 -> 0.0; residual is engine-side) | 0.8 |
| `MTGAllCards::findType(string)` inside Render | **56.8 / 55.7** | **0.1 / 0.1** | 0.1 |
| `findType(string)` total (engine + UI) | 325 / 312 | 221 / 223 | 224 |
| events reaching `DuelLayers` | 2.49 / 2.45 | 2.44 / 2.56 | 2.47 |
| `GuiPlay::Replace` per EVENT | **1.07 / 1.07** | **0.43 / 0.43** | 0.96 |
| `GuiPlay::Replace` per frame | 2.66 / 2.61 | 1.04 / 1.10 | 2.38 |
| `Replace` wall us/frame | 4.0 / 3.6 | 3.0 / 2.4 | 3.3 |

Against the plan's success line: `GetQuad(string)` in Render 58.8 -> **0** (met);
`hasPossibleAttackers` 12.9 -> **0 from the render path** (plan said 1; in AI-vs-AI it is 0 —
a human seat in its combat window pays it once per own creature that has already passed the
cheap tests); `Replace()` 1.1/event -> **0.43/event, ~1 per frame** (met); `RetrieveCard`
12.9 -> **unchanged** — deliberately NOT met, see the contract above (a per-view `JQuadPtr`
cache is the one change here I judged unsafe on the console).

**Wall time — not claimed.** `CardGui::Render` us/card: before 31.4 / 31.7, after 37.1 / 28.0,
NOCACHE 25.7. Whole-frame Render at 20-23 permanents: before 0.83 / 0.81 ms, after 1.00 / 0.73,
NOCACHE 0.68; at 24-27: before 0.93 / 0.91, after 1.06 / 0.79. The spread between IDENTICAL
binaries on consecutive legs (+-25%, this laptop is shared with the other lanes and the render
leg is GL-driver bound offscreen) exceeds the ~12% quad-resolution share lane W measured, so
the desktop timing cannot resolve this change either way; the counters are the verified
result, the console `avg_rnd` line on the next VPK is the timing gate. `RetrieveCard`'s own
cost per call did drop (no 4 KB heap alloc, no stringstream, no string copies) but was not
timed in isolation.

## Predictions (falsifiable)

1. A development build with `WAGIC_RENDER_NOCACHE=1` shows baseline-shaped counters
   (`GetQuad(string)` back inside Render, `Replace` ~1/event) — **verified above** (34.2, 0.96).
2. Next VPK memlog: `avg_rnd` at p1perm+p2perm >= 20 falls by the quad-resolve + scan share
   (order 10-15%), NOT by half — the draw calls are untouched (D28/owner).
3. On the console a card whose art loads late still flips from the generic back to its art
   (RetrieveCard is still per frame).
4. A human seat in its combat window still sees the grey mask on its summoning-sick /
   cannot-attack creatures, and only then (the reorder is pure).
5. Lane V's eight transform fixtures + the DFC prediction stay green (no card-art pointer is
   cached across frames) — the suite that carries them is green here.

## NOT verified

- **No windowed run — visual parity NOT eyeballed.** Every measurement was offscreen
  (`SDL_VIDEODRIVER=offscreen`). The HUD quads are the same managed objects the string path
  returns, so the pixels should be identical; nobody looked.
- A44 (options -> key bindings -> "Load Defaults..." twice) and A45 (Story mode) were NOT
  driven under ASAN — GUI-only paths; the ASAN evidence covers the duel render path and the
  exit teardown (which is what exercised L24's dtor).
- L27's duplicate-row repro (MENU press while the patience prompt is open) not driven.
- PSP/Vita/Android compile of the touched files not attempted (nothing platform-specific was
  added; `snprintf`, `const&`, ids).
- Console `avg_rnd` — needs an owner-initiated VPK.
