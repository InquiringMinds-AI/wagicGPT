# Lane P — A50 / owner decision O3: the dead-module purge

Branch `w54-lane-P`, seeded from master `3e53b8d4a`. One commit, zero behaviour change.

## Owner ruling applied

O3: delete the dead modules; **KEEP** `NETWORK_SUPPORT`, `NetworkPlayer.cpp`, `RemotePlayer`,
`JNetwork`/`JSocket` and the `MENU_STATE_NETWORK_*` branches ("networking is high on my desired
features list ... i dont anticipate working on it soon"). Verified untouched: `NETWORK_SUPPORT`
still defined in `wagic-SDL.pro`/`JGE/Makefile`, `JNetwork.o`/`JSocket.o` still in every source
list, `NetworkPlayer.cpp`, `RemotePlayer` and the menu branches unmodified.

## Deleted (39 files, 9,863 lines removed, 19 inserted)

projects/mtg (10 files): `AIPlayerBakaB.{cpp,h}`, `Navigator.{cpp,h}`,
`CardSelectorSingleton.{cpp,h}`, `GuiCardsController.{cpp,h}`, `GuiFrame.{cpp,h}`.

JGE (29 files): `JAnimator`, `JResourceManager`, `JTTFont`, `JParticle`, `JParticleEffect`,
`JParticleEmitter`, `JParticleSystem`, `JDistortionMesh`, `JOBJModel`, `JSpline`, `JGBKFont`,
`Downloader` (each `.cpp`+`.h`), plus the dead mains `glutmain.cpp`, `Xmain.cpp`, `winmain.cpp`,
`Qtmain.cpp`, `JGfx-fake.cpp`.

## Dead bodies / allocations removed in surviving files

- `DuelLayers.cpp`: `#include "GuiFrame.h"` and `Add(NEW GuiFrame(go));` — the per-duel
  allocation and its slot in the per-frame Update/Render/event loops. `GuiFrame` rendered and
  updated nothing (whole body commented out), so the layer count drops by one with no visual
  or input change (`Add` appends; nothing indexes `objects[]` positionally).
- `PlayGuiObjectController.cpp`: the 64-line commented `CheckUserInput` body (`return false;`
  kept).
- `GameOptions.cpp` `GameOption::menuStr`: the `char buf[12]` + `sprintf` whose result was
  never read.
- `AIPlayer.cpp` / `AIPlayer.h` / `GameObserver.cpp`: `AIPlayerBakaB`'s only construction site.
  `createAIPlayerTest` existed solely to build an `AIPlayerBakaB`; with the class gone it is
  byte-identical to `createAIPlayer`, so the `#ifdef AI_CHANGE_TESTING` dispatch in
  `GameObserver.cpp:3079` collapses to the plain `createAIPlayer` call it already fell through
  to. `GameObserver.cpp` was byte-spliced (CP1252 + CRLF); `git diff | grep -c $'\357\277\275'`
  = 0.
- `WResourceManager.h`: `#include <JResourceManager.h>` dropped. `INVALID_ID` / `ALREADY_EXISTS`
  lived in that JGE header and are used only by `WGui.h`, `WGui.cpp`, `GameOptions.cpp` and
  `WResourceManager.cpp`, all of which reach them through this header — so the two `#define`s
  move here verbatim (tagged `#W54-P (O3)`) rather than keep a dead TU alive for them. This was
  a real compile break caught by the gate, not a speculative edit.
- `JGE.cpp`, `pc/JGfx.cpp`: `JResourceManager.h` includes (no symbol use); `JGE.cpp`'s
  commented `JParticleSystem.h` include; `JGE.h`'s `class JResourceManager;` /
  `class JParticleSystem;` forward declarations.

## Build lists updated (so every target still builds)

| list | entries removed |
|---|---|
| `projects/mtg/wagic.pri` (SDL/desktop) | 26: `GuiCardsController`, `GuiFrame`, `AIPlayerBakaB` (src+hdr), `Downloader`, `JAnimator`, `JDistortionMesh`, `JOBJModel`, `JParticle`, `JParticleEffect`, `JParticleEmitter`, `JParticleSystem`, `JResourceManager`, `JSpline` (src+hdr) |
| **`CMakeLists.txt` (Vita)** | **14: `JGE/src/{JSpline,JAnimator,JResourceManager,JParticle,JParticleEmitter,JParticleEffect,JParticleSystem,JDistortionMesh,JOBJModel}.cpp` and `projects/mtg/src/{AIPlayerBakaB,CardSelectorSingleton,GuiCardsController,GuiFrame,Navigator}.cpp`** (plus one stale comment that named `Downloader` as part of the not-built transport) |
| `projects/mtg/Makefile` (PSP) | 5 objects: `CardSelectorSingleton.o`, `GuiCardsController.o`, `GuiFrame.o`, `Navigator.o`, `AIPlayerBakaB.o` |
| `projects/mtg/Android/jni/Android.mk` | 13: the four mtg sources above (no `Navigator`) + the nine JGE sources |
| `JGE/Makefile` | `GENERIC_OBJS`: `JGBKFont.o`, `JSpline.o`, `JAnimator.o`, `JResourceManager.o`, `JParticle.o`, `JParticleEmitter.o`, `JParticleEffect.o`, `JParticleSystem.o`, `JTTFont.o`, `JOBJModel.o`; `LINUX_OBJS`: `Xmain.o`. `JNetwork.o` KEPT. |
| `projects/mtg/iOS/Makefile` (unmaintained target) | 12 of the same sources |
| `projects/mtg/wagic-qt.pro` (unmaintained target) | `Qtmain.cpp`, `JGfx-fake.cpp` |

`wagic-SDL.pro` itself names no deleted source (it pulls the list from `wagic.pri`);
`Makefile.sdl` regenerated with `qmake6 wagic-SDL.pro CONFIG+=debug -o Makefile.sdl`.

## Gate

Baseline on the seeded worktree, before any edit
(`~/.gatelogs/w54-P-base.log`): suite **1226 tests, 2 failed** (`lifeline.txt` +
`merrow_reejerey.txt` — the known concurrency-only pair), **51 AI tests, 0 failed**,
0 timeouts; PARSETEST **2489 passed / 0 failed**.

After the purge (`~/.gatelogs/w54-P-post.log`): clean `make -f Makefile.sdl -j2` link; suite
**1226 tests, 2 failed** — the same two files by name — **51 AI / 0**, **0 timeouts**;
PARSETEST **2489 / 0**. `bin/wagic` 54,528,680 -> 53,061,424 bytes (**-1,467,256**, ~1.4 MB of
resident code out of every target).

## Deliberately NOT deleted, with reasons

- **`DeckStats.cpp:339-360`** (A50's "loop that only checks `deck1`"). It is a *degenerate*
  loop, not dead code: it pushes into `aiDeckNames`/`aiDeckStats`, which
  `GameStateDeckViewer.cpp:1630-1636` renders. Deleting it would remove a visible stats row —
  a behaviour change, which this lane's invariant forbids. It is a real bug (the loop never
  advances past deck1) and belongs in a lane that fixes it, not in the purge.
- **`MTGPack.cpp:226`** (`getCardByName("card")`). A50 itself calls it dormant; it is a
  wrong-argument bug in a live parser path, not a dead module. Same reasoning.
- **The rest of `AI_CHANGE_TESTING`** (`GameStateDuel` A/B thread pool + `handleResults`,
  `GameStateMenu`'s "AI A/B Testing" menu entry, `AIPlayerBaka`'s four `#ifndef` guards,
  `GameObserver.cpp:3088`'s `PLAYER_TYPE_CPU_TEST` fast-timer path). Only the *`AIPlayerBakaB`
  construction* is A50's dead module; the surrounding A/B-testing mode is a separate feature
  and removing it is a much wider diff. It can only be defined under `_DEBUG && WIN32`
  (`config.h:34-36`) and no target defines it.

## What I did NOT verify

- **The Vita, PSP, Android and iOS builds — not built.** No docker here. The 14 Vita
  `CMakeLists.txt` entries listed in bold above are the exact removals the next VPK is the
  check on; likewise the 5 PSP `Makefile` objects and the 13 Android.mk entries. My evidence
  that they are safe is source-level only: `/usr/bin/grep` over `projects/mtg` + `JGE` finds
  no remaining reference to any deleted symbol or header outside the deleted files themselves
  (sweep is clean, exit 1).
- **The `INVALID_ID`/`ALREADY_EXISTS` move is desktop-verified only.** If any handheld TU
  reached those macros through a path that did not go through `WResourceManager.h`, it would
  fail there and not here — I found no such TU, but I could not compile to prove it.
- **`wagic-qt.pro` and `projects/mtg/iOS/Makefile` are now source-consistent but still
  unbuildable targets** (the Qt one has no `main` left, by design — `Qtmain.cpp` was on A50's
  dead-mains list). Neither is in the owner's must-build set; I did not attempt either.
- No runtime/visual check of a real duel: the GuiFrame layer removal is argued from the
  commented-out body and `DuelLayers`' append-only `Add`, and covered by the suite, not by a
  human play session.
