# Lane N (audit lane H) — JGE loaders + Vita mains (wave 54)

Worktree `worktrees/lanes/w54-N`, branch `w54-lane-N`, seeded from master dab9f72f4.
Files touched: `JGE/src/pc/JGfx.cpp`, `JGE/src/JFileSystem.cpp`, `JGE/src/JLBFont.cpp`,
`JGE/src/JGui.cpp`, `JGE/src/zipFS/zfsystem.{h,cpp}`, `JGE/src/JAnimator.cpp`,
`JGE/src/JResourceManager.cpp`. `Vitamain.cpp` is UNTOUCHED (A47 is gated on telemetry, O1 is
the owner's), so the Vita-main stub compile did not apply; what stood in for it is in §3.
Every code comment for this lane is tagged `#W54-N (item)`.

## 1. What shipped, per item

**A2 (a) — pow2 padding off on the Vita, behind one flag.** `JGE_TEX_NPOT` is defined for
`VITA` unless `-DWAGIC_TEX_POW2_PAD` (the device A/B is that one CMake define; desktop keeps
padding, unchanged). `textureDims()` returns the real size; NPOT uploads use `GL_CLAMP_TO_EDGE`
instead of `GL_REPEAT` (repeat is only well-defined on POT sizes under SGX/ES2 rules and JGE never
samples outside a quad). Checked against vitaGL: `gpu_alloc_texture` (utils/gpu_utils.c) allocates
`VGL_ALIGN(w, 8) * h * bpp` and row-copies, so any width uploads; POT is forced only on the DXT
path (textures.c:877/1600). 360x514 card: 2,097,152 -> 740,160 bytes decoded, uploaded and
cache-accounted (`WCachedResource::size()` reads `mTexWidth*mTexHeight*pixel_size`, so it becomes
right with NO change to `WCachedResource.cpp:161` — lane D need not touch it). 48 MB
HUGE_CACHE_LIMIT: 24 -> ~64 full cards.
Prediction (device): `Destroying WCachedResource` per deck-editor page ~3x fewer; `vram_free`
sits higher for longer.

**A2 (b) — decode scratch, VITA only.** `JGE_DECODE_SCRATCH` (VITA unless
`-DWAGIC_NO_DECODE_SCRATCH`): the raw-file buffer and the decoded-pixel buffer are grow-only
statics reused by every LoadJPG/LoadPNG. The pixel scratch is single-slot with a busy flag: a
second texture decoded before the first is uploaded gets a heap block (no aliasing possible), and
`LoadTexture` uploads IMMEDIATELY under the flag (single-threaded platform, GL context on the
thread) so the scratch is free for the next load — the JQuad-time transfer becomes a no-op. One
release path (`releaseDecodedPixels`) serves upload, headless, `~JTexture` and every loader error
exit; nothing in `projects/mtg` reads `mBuffer` (grep: 0). Desktop (threaded loads) keeps the
per-load heap allocation.
Prediction (device): `heap_arena` flat across a 20-game session (was +1.4 MB/game);
`heap_used` +~2.2 MB one-off (the scratch pinned at the largest texture ever decoded — the
960x544 backdrop is 2.09 MB NPOT; it was a 4 MB padded transient before).

**A2 (c) — loader error handling.** LoadJPG: a `jpeg_error_mgr` whose `error_exit` longjmps
(the stock one called `exit()` — on the Vita that was "vanish to LiveArea"); an in-memory source
whose `fill_input_buffer` feeds a synthetic EOI AND flags truncation, so a short file finishes
cleanly and is then REJECTED (NULL texture, never a half-grey card cached as good); bounded
`skip_input_data`; `out_color_space = JCS_RGB` so the scanline buffer (sized for 3 components)
can no longer be overrun by a 4-component file; every exit frees `rawdata`/scanline/pixels. LoadPNG:
`line` and the pixel buffer are reachable from the setjmp handler (they leaked on any libpng error
after allocation); zero-size and short-read guards. `JFileSystem::ReadFile` now returns the bytes
actually read (`gcount()`) on both the zip-member and the stream path (it returned `size` on a
short zip read and 0 on ANY eof); both loaders reject `got != rawsize`.

**A2 (d) — pad fringe.** `fillTexturePadding()` replicates the last real column/row into the
first pad column/row (what CLAMP_TO_EDGE would sample) and zeroes the rest; no-op without padding
(i.e. on the Vita default). Candidate for the 2026-08-09 "battlefield 1 px line" on desktop.

**A46 — `JLBFont::GetStringWidth`.** `if (ch < 0 || ch > 127) continue;` mirroring DrawString;
the `mTexture` NULL check moved above its first dereference.

**A48 — `JGuiController::RemoveAt`.** Takes the pointer BEFORE `erase()` (it deleted the
successor and left it in the vector), bounds-checks `i`, `mCurr >= mCount` reset; the
click-to-select loop (`:176`) skips NULL rows like Update/Render already do; `mCursor` initialised.
Still no caller — the trap is disarmed, not moved.

**L28 — zipFS.** `virtual ~izfstream() { close(); }`, both ctors initialise
`m_Zipped/m_Used/m_UncompSize/m_Offset/m_CompSize`. Because `JFileSystem::mFile` is a member
destroyed after `~JFileSystem`'s body deletes the filesystems, the body now closes + detaches it
first (`mFile.close(); mFile.setFS(NULL);`) so the pooled (PSP) close cannot reach a freed FS.

**A42, JGE half.** (1) `filesystem::FileNotZipped` probes with one `stat()` + `S_ISREG` instead of
constructing an `ifstream` (open+close+streambuf per probe, twice per miss per FS); a directory
(the blank-name `sound/sfx/` sample lookups) no longer counts as a loose file. (2)
`JFileSystem::openForWrite` creates the parent directories of a user-FS write on every platform
(was ANDROID-only): `GetResourceFile` extracts `sound/sfx/*.wav` into the user FS before SDL_mixer
opens them by path, and on the Vita those parents do not exist (EnsureUserDirs makes `player/` and
`settings/` only) — if the Vita is silent, this is the fix. The old backward walk never terminated
for a file in the user root; the new one walks the RELATIVE path forward. Disable flag:
`-DWAGIC_NO_USERDIR_MKDIR`. NOT done: the `Mix_LoadWAV_RW` from-memory load that would drop the
extraction copy entirely (JSfx.cpp untouched); it is the better end state and a one-lane follow-up.

**A45, JGE twins.** `JAnimator.cpp:50` / `JResourceManager.cpp:101` allocate `size+1` and
NUL-terminate at the bytes actually read. Harmless whichever way O3 goes.

**A47 — analysis only (GATED, no pacing change).** `MainLoop` takes `frameStart` at the loop top,
runs Update/Render, `vglSwapBuffers` blocks to the next vblank (vsync on), then sleeps to
`TARGET_FRAME_MS 33`. Regimes, work = upd+rnd:
- work < 16.7 ms: swap returns at 16.7, sleep ~16 -> presented every 33 ms, 30 fps. This is the
  only regime where the sleep does anything, and it is the intended cap.
- 16.7 < work <= 33.3: swap returns at 33.3, `frameTime >= 33`, no sleep -> 30 fps.
- 33.3 < work < 50: swap at 50 -> 20 fps; the sleep never fires; vsync is the cap.
So the sleep does NOT turn a 25 ms frame into 50 ms. What it does cost: after a sleep the next
frame's work starts at ~33 and must finish by 50 to make the next vblank — a 17 ms budget instead
of the 33 ms a swap-interval-2 cap would give. Work oscillating around 17 ms therefore alternates
33/50 ms periods (~25 fps, visibly uneven); work above ~17 ms is self-correcting (the sleep stops
firing, 33.3 ms periods, 33 ms budgets) at the price of one 50 ms transition frame. Lane W's
desktop profile puts the reported wide boards at 3-10 ms desktop -O2 (Update dominated by
`AI Act` on the AI's turn) — a Vita factor of ~5-10x puts them BEYOND the sleep regime entirely,
where only the work matters. What `avg_swp` will say in vpk13's memlog: `avg_swp` near 16 with
`avg_upd+avg_rnd < 17` = sleep regime (harmless, O11 is a jitter fix); `avg_swp` ~= 33 - work with
large `avg_upd`/`avg_rnd` = work-bound (O11 changes nothing; the fix is the work). O11's code
would be: keep vsync, drop the sleep, express the 30 fps as a vblank count if vitaGL exposes one
(check vitaGL.h for a swap-interval API before assuming) — ~6 lines in `MainLoop` plus the stub
compile. Not landed; no reading exists.

## 2. Gate

Baseline re-confirmed on this worktree's seeded binary first: suite 1219 with exactly 2 failed
(lifeline.txt + merrow_reejerey.txt), 49 AI / 0, 0 timeouts; PARSETEST 2258 / 0.
After the lane (`make -f Makefile.sdl -j2`, clean link): suite **1219 tests, 2 failed
(lifeline.txt + merrow_reejerey.txt — the known concurrency pair), 49 AI tests, 0 failed, 0
timeouts**; PARSETEST **2258 passed, 0 failed**. Unchanged, as the brief requires (no parse
string, no engine rule touched). Logs: `~/.gatelogs/w54N/gate-{base,post1}-*.log`.

## 3. ASAN loader test (`~/.gatelogs/w54N/loadertest.cpp`, objects in `~/.gatelogs/w54N/asan/`)

A standalone main linked against ASAN builds of `pc/JGfx.cpp`, `JFileSystem.cpp`, zipFS,
`JLBFont.cpp` (compiled with **`-funsigned-char`** to make the desktop see ARM's unsigned `char`),
`JLogger.cpp`, `JGE.cpp` + the game's other objects (SDLmain excluded; its three exports stubbed).
Runs headless from `projects/mtg/bin`, `ASAN_OPTIONS=detect_leaks=1`, under the memory cap.
Fixtures (in `bin/User/w54n/`, deleted after): `ok.jpg` (Res avatar 35x50), `ok.png` (Ok.png
56x45), `trunc.jpg` (40 % of ok.jpg), `hdronly.jpg` (first 200 bytes), `garbage.jpg` (5000
random bytes), `soi_garbage.jpg` (FFD8FFE0 + random), `empty.jpg` (0 bytes), `trunc.png` (50 %),
`ihdr.png` (40 bytes), `garbage.png`, then ok.jpg/ok.png again, then `font:graphics/f3` measuring
`"caf\xE9 \xC9\xE8\xFC!"` vs `"caf !"`.

| binary | result |
|---|---|
| **pre-fix** loaders (HEAD `pc/JGfx.cpp` + HEAD `JLBFont.cpp`), one file per run | `trunc.jpg`, `hdronly.jpg`: **ASAN SEGV inside libjpeg** (the OOB read past `rawdata`). `garbage.jpg`, `soi_garbage.jpg`, `empty.jpg`: process **exit(1)** from libjpeg's default `error_exit` ("Not a JPEG file"). `trunc.png`: **LeakSanitizer 16,608 B in 2 allocations** at `LoadPNG:2342` (the `line`/`buffer` leak). Font: width(accented) **47.25** vs stripped 26.75 — garbage read past `mCharWidth` (intra-object, so no ASAN report; the wrong number IS the bug). |
| **post-fix** (`lt_new`, desktop defaults: padding on, no scratch) | all 10 bad files -> **NULL texture**, process continues, **0 ASAN errors, 0 leaks**; ok.jpg 35x50 tex 64x64, ok.png 56x45 tex 64x64; font accented == stripped (26.75, accents skipped exactly as DrawString skips drawing them). |
| **post-fix with the Vita defaults forced** (`lt_flags`: `-DJGE_TEX_NPOT -DJGE_DECODE_SCRATCH`) | same NULLs, **0 ASAN errors, 0 leaks**; ok.jpg tex **35x50**, ok.png tex **56x45** (NPOT), `buffer=released` after load (immediate upload + scratch hand-back verified), repeated loads reuse the scratch. |

This flag-forced run is what stood in for the Vita stub compile: the two VITA-default code paths
were compiled and executed on the desktop; only the `GU_PSM_5551` 16-bit branch (preserved
verbatim from the old loader) and vitaGL itself were not exercised here.

## 4. Predicted memlog deltas for vpk13 (device)

- `heap_arena`: **flat** after game 1 (was +1.4 MB/game, 48.7 -> 82.9 MB over 24 games).
- `heap_used`: +~2.2 MB one-off vs vpk11 at the same point (scratch pinned at the largest texture,
  the NPOT backdrop), then flat per game.
- `heap_free`: no longer draining; the bad_alloc horizon disappears for the decode path.
- `ram_free`: **unchanged (157758608)** — O1 (`_newlib_heap_size_user`) was NOT applied; it drops
  ~96 MB at boot only once the owner takes O1 (one file-scope line in Vitamain.cpp, VITA-guarded,
  plus the stub compile).
- `vram_free`: higher for longer (cards 2.8x smaller); `Destroying WCachedResource` per deck-editor
  page ~3x fewer; first-time art decode ~2.8x fewer bytes written.
- A47: `avg_swp`/`avg_upd`/`avg_rnd` are the reading; no prediction, see §1.

## 5. Owner decisions — what each would take (NOT done)

- **O1** heap constant: `extern "C" unsigned int _newlib_heap_size_user = 224 * 1024 * 1024;` at
  file scope in Vitamain.cpp under `#ifdef VITA` + stub compile; verify `ram_free` -96 MB at boot.
- **O3** dead modules: remove from the Vita CMake `JGE_SOURCES`, `wagic.pri`, `JGE/Makefile`
  (Xmain.o); the two A45 twins are safe either way now.
- **O6** `WAGIC_FIZZLELOG`: drop the define at CMakeLists.txt:294.
- **O7** `-std=c++11` -> `-std=gnu++14` at CMakeLists.txt:75 (vitasdk GCC supports it).
- **O10** `NOSTARSDYNAMIC` as a Vita CMake define.
- **O11** see A47 above; needs the vpk13 `avg_swp` reading first.

## 6. What I did NOT verify

- **Nothing ran on the console.** NPOT + CLAMP_TO_EDGE rendering (eyeball ONE card, then a menu
  icon of odd width such as 56x45 — vitaGL 8-aligns the stride by row copy, read in source, not
  seen), the `GU_PSM_5551` NPOT path, the immediate upload at load time, and the sound-directory
  fix (does the Vita now have sound?) are all vpk13 questions. The A/B for padding is
  `-DWAGIC_TEX_POW2_PAD`; for the scratch `-DWAGIC_NO_DECODE_SCRATCH`.
- **No Vita compiler saw this code.** `JGfx.cpp` compiled clean on the desktop with and without
  the two Vita-default macros forced; `Vitamain.cpp` is untouched, so the w53W stub method had
  nothing to check.
- **No suite pin** — the loaders, fonts and zipFS have no headless suite surface (the suite never
  loads art); the ASAN test above is the pin, and it lives in `~/.gatelogs`, not the repo.
- **Desktop art rendering was not eyeballed** (`bin/User/sets` is `sets.hidden`; the owner was on
  the TV). The padded desktop path only changed in what the pad contains (A2d) and in error
  handling.
- **The `ReadFile` return-value change** was exercised only by the loaders; other callers
  (JAudio, JLBFont `.dat`, JCooleyesMP3) ignore or `!=`-compare the value, read as compatible, not
  run.
- **L28 on the PSP pool build** (the only build with `USE_ZBUFFER_POOL`): reasoned, not built.
- A42's memoisation of misses at the game layer (`getSample`, tombstones) is lane D/G's half, not
  here; the ziplog miss-line count is therefore NOT predicted to drop from this lane alone — only
  each miss got cheaper (1 stat vs 2 stream opens per FS).
