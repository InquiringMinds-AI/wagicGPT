# Codebase audit 2026-09-02 — shared brief for the eight audit agents

Owner request (verbatim): "fan some fable powered agents to go over the code base looking for
improvements or cleanup that can be done - for example crud that can be cleaned up, performance
improvements that can be made, ways to use memory more efficiently, places that need to be made
memory safe, places the heap isnt cleaning itself up right, etc."

Repo: /home/magi/Projects/wagicGPT (master 859186536). Game code: projects/mtg/{src,include};
engine layer: JGE/; Vita port: worktrees/psp (CMakeLists.txt, JGE Vita main). READ-ONLY on the
master tree — a dev loop is running there (8 agents editing worktrees, a review in flight).

## What the code is
Wagic: 15-year-old C++ MTG engine, PSP origin (`gnu++14` pinned, GCC 15, `-Werror` OFF, clangd is
wrong — g++ is truth). Targets: desktop SDL (this machine), PS Vita (444 MHz ARM, ~40 MB heap
free in-game, the OWNER'S MAIN PLAY DEVICE — perf and memory there are the point), PSP, Android.
`AIPlayerGPT.cpp` (37k lines) is the LLM-opponent seat: per-decision prompt assembly, reply
parsing, async model worker, a 2,249-case self-test (`runParseSelfTest`, the PARSETEST).
Encoding landmines: MTGRules.cpp, GameObserver.cpp (CP1252 + CRLF), AIPlayerGPT.cpp,
MTGAbility.cpp, AllAbilities.cpp hold non-UTF-8 bytes — read with python rb or `iconv -f cp1252`
if a tool chokes; NEVER re-encode. Use /usr/bin/grep (bare grep is a wrapper that drops matches).

## Owner's code-quality criteria (his words — the ONLY axes that matter)
does it do the job; is it efficient; is it bug free; is it as secure as is reasonable for its use
case; is it legible for LLM CODING AGENTS; is the design forward-thinking about planned features.
He "increasingly cares little about what code looks like" — NO style/formatting/naming nitpicks,
no "modernize to C++17" (pinned gnu++14), no rewrites-for-taste. Concrete, evidenced, bounded.

## Categories to hunt (each finding tagged with one)
CRUD — dead code, unreachable branches, duplicated logic, stale #ifdefs, leftover diagnostics
  that ship in release builds (owner rule: diagnostics compile out of release; gate is
  `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)` or `WAGIC_VITAMEMLOG`), abandoned features.
PERF — per-tick/per-frame work that could be memoised or moved to an event; O(n^2) scans over
  battlefield/abilities/zones; string building in hot paths (ostringstream per tick, repeated
  `magicText` parses, `find()` chains); redundant re-parses of card scripts; allocation churn.
MEM-EFF — containers that over-reserve, copies where a reference would do, per-card duplicated
  data that could be shared (Vita heap is the constraint), caches without bounds.
MEM-SAFE — raw pointer lifetimes (ability GC, `clone()`/`copy()` without matching delete,
  dangling after zone moves / destroy), buffer sizes (fixed char arrays, `files[1024]`-style
  overflows — one was real), unchecked NULL from lookups, uninitialised members, integer
  narrowing on ids/counters, use-after-free windows around `MTGCardInstance` destruction,
  thread safety of the async worker (AsyncState shared_ptr + detached thread).
LEAK — `new` without owner, containers of raw pointers never cleared, resources (textures,
  files, curl handles) not released, `WCachedResource` lifecycle, per-game state that survives
  into the next game (the suite runs 1,200 games in one process — leaks accumulate there and
  on the Vita across matches).

## Method (binding)
1. Read your slice IN FULL (large files: in chunks). Trace, don't skim.
2. Every finding needs: `file:line` (master), category, the mechanism in plain words, EVIDENCE
   (a code excerpt, a grep count, or a measurement), a concrete fix sketch (what changes, where),
   the blast radius (which callers/tests), and how to verify (which suite test / PARSETEST case /
   measurement). Mark each VERIFIED (you demonstrated it) or SUSPECTED (reading only).
3. Measurements are welcome and preferred for PERF/LEAK claims. You may build in your OWN
   worktree: `cd /home/magi/Projects/wagicGPT && git worktree add -b audit-<slice>
   worktrees/lanes/audit-<slice> master`, then copy `projects/mtg/{objs-sdl,Makefile.sdl,
   PrecompiledHeader.h.gch}` and `projects/mtg/bin/User` + `projects/mtg/bin/wagic` from the
   master tree into it, build with `make -f Makefile.sdl -j4` from <wt>/projects/mtg, run from
   <wt>/projects/mtg/bin. An ASAN makefile exists (untracked) at
   projects/mtg/Makefile.sdl-asan (copy it; dirs objs-asan/ bin-asan/ must exist; `make -f
   Makefile.sdl-asan -j4`). Suite: `env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1
   WAGIC_TESTSUITE=1 ./wagic` (~30 s; baseline 1215 tests with exactly 2 known failures
   lifeline.txt + merrow_reejerey.txt, 46 AI/0); PARSETEST: `WAGIC_GPT_PARSETEST=1 ./wagic`
   (2249/0). EVERY wagic run is prefixed `systemd-run --user --scope -q -p MemoryMax=4G -p
   MemorySwapMax=0 --` and wrapped in `timeout 600`. Remove your worktree when done
   (`git worktree remove --force` + `git branch -D`). Scratch only under ~/.gatelogs/ or
   your worktree. NEVER edit the master tree, NEVER commit anywhere, NEVER run git on master
   beyond worktree add/remove, NEVER touch worktrees/psp or worktrees/lanes/w53-*.
4. Rank: HIGH (crash/UAF/leak-per-game/perf on the Vita hot path), MED, LOW. Cap the report at
   what is real — 8 good findings beat 40 thin ones. Say what you did NOT read.
5. Output: /home/magi/Projects/wagicGPT/projects/mtg/strategy-design/audit-20260902/<slice>.md
   (write it directly on the master tree — this directory is the ONE place you may write).
   Final message: a 10-line summary (counts by category/rank, top 3 findings with file:line).
