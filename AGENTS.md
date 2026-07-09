# Repository Guidelines

## Project Structure & Module Organization

Wagic is a cross-platform C++ card-game engine. Game rules, AI, and native UI code live in `projects/mtg/src/`, with headers in `projects/mtg/include/`. Engine and platform backends are in `JGE/`. Assets, card primitives, AI decks, and regression scenarios live under `projects/mtg/bin/Res/`. Platform projects are under `projects/mtg/{Android,iOS,MacOS,Windows}/` and `vita/`. The dependency-free `web/` viewer renders the engine-neutral `BoardState` contract.

## Current Direction & Architecture

Prioritize non-LLM engine bugs before further LLM-opponent work. Before changing `AIPlayerGPT`, read `projects/mtg/strategy-design/REDESIGN-NOTES.md`; migrate toward a strangler-style `DecisionRequest -> Action` interface. Keep the `web/` renderer pure, with networking confined to the connection layer.

## Build, Test, and Development Commands

Use the SDL2 debug build for routine Linux work:

```bash
cd projects/mtg
qmake6 wagic-SDL.pro CONFIG+=debug -o Makefile.sdl
make -f Makefile.sdl -j$(nproc)
cd bin
SDL_VIDEODRIVER=wayland SDL_AUDIODRIVER=dummy WAGIC_TESTSUITE=1 ./wagic
```

Always pass `-o Makefile.sdl`; bare qmake overwrites the tracked PSP makefile. Results land in `projects/mtg/bin/User/test/results.html`. Follow `BUILD_VITA.md` and its pinned vitaGL revision for Vita builds. Open `web/index.html` directly.

## Card-Bug and Testing Workflow

For every card-behavior bug, retrieve current Oracle text from Scryfall or Gatherer before trusting community-authored primitives. Compare `text=`, `auto=`, mechanism, and target. First seek a faithful data-only card-script fix; change shared engine code only when the Oracle effect cannot be expressed by existing constructs. Use `docs/card-script-lexicon.md`, survey comparable cards, and test the general case—not only the reported fixture.

Add focused `.txt` regressions under `projects/mtg/bin/Res/test/` and register them in `_tests.txt`. Scoped runs may temporarily replace that registry; preserve existing edits and restore it exactly afterward. New files beneath ignored `bin/` require `git add -f`. Run the full suite for engine changes. GPT paths require separate GUI/translog validation.

## Coding Style & Safety

Match nearby legacy style: four-space C++ indentation, separate-line braces, `PascalCase` types/functions, and `mCamelCase` members. Stay compatible with GNU C++14. Avoid unrelated reformatting. `MTGRules.cpp` and `GameObserver.cpp` contain legacy encoding/line endings; edit byte-safely and verify no U+FFFD replacement characters appear. Inspect `git status` before editing and preserve unrelated worktree changes.

## Commits & Pull Requests

Use concise imperative subjects, optionally subsystem-prefixed, such as `Blocking: forbid assigning one blocker to two attackers`. Keep commits scoped and record verification performed. This fork is the product: do not open upstream Wagic PRs. Do not add AI-attribution trailers or generated-tool footers. Never commit binaries, downloaded card art, credentials, endpoint keys, or local build directories.
