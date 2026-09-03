# Audit slice 8 — JGE engine layer + Vita port (2026-09-02, master 859186536)

Scope read in full: JGE/src/{JFileSystem,JGE,JGui,JApp,JGameObject,JAnimator,JSprite,JLBFont,
JGBKFont,JTTFont,JAudio,JResourceManager,Downloader,JNetwork,JLogger,OutputCapturer,Vitamain,
SDLmain,main}.cpp, JGE/src/pc/{JGfx (init/scene/bind/loaders/texture upload — draw primitives
only mapped),JSfx,JSocket}.cpp, JGE/src/zipFS/{zfsystem,zstream}.cpp+.h, JGE/src/JGfx.cpp (PSP:
texture pool, deferred frees, JPG/PNG loaders), JGE/include/{JFileSystem,JGui,JLBFont,JTypes}.h,
CMakeLists.txt (Vita), psp-work/NOTES.md memory/crash sections, psp-work/logs/20260902*/{memlog,
wagic-ziplog,debug}.txt, worktrees/psp (read-only; same commit as master), vendor/vitaGL
vgl.c + mem_utils.c + textures.c (for pool/NPOT semantics), WCachedResource.cpp size()/Attempt()
and WResourceManager.cpp cache paths (the texture seam only).
NOT read: Encoding.cpp GBK tables, JMP3/JCooleyesMP3 (PSP audio decode), hge/*, tinyxml, unzip/
(minizip — not linked anywhere), iOS/android/qt dirs, winmain/Xmain/glutmain/Qtmain/Qtconsole/
JGfx-fake (dead mains), JParticle*/JMD2Model/JOBJModel/JDistortionMesh/JSpline/Vector2D (0 users
in projects/mtg), pc/JGfx.cpp draw-primitive bodies (lines 1121-1960, 2890-4443).

Measurement done (desktop, master binary, scratch dir, headless suite under MemoryMax=4G):
open fds sampled every 2 s for 600 s = flat at 12 the whole run; VmRSS 93-102 MB flat. So the
JFileSystem/zipFS loose-file path leaks neither fds nor memory across ~1200 suite games
(art is not loaded headless, so the zip/art path is NOT covered by this run).

Ranking: HIGH 2, MED 4, LOW 6. Categories: MEM-EFF 3, MEM-SAFE 5, LEAK 2, CRUD 2 (+notes).

---

## HIGH

### H1. Vita: the 128 MB newlib heap is the crash ceiling while ~158 MB of vitaGL RAM pool sits idle
MEM-EFF / MEM-SAFE — VERIFIED (memlog + vitaGL source)
- Where: JGE/src/Vitamain.cpp:373 `vglInitExtended(0x40000, 960, 544, 24*1024*1024, ...)`;
  no `_newlib_heap_size_user` anywhere in the tree (grep: only the comment at Vitamain.cpp:57-63).
- Mechanism: vitasdk newlib carves its heap at process start as ONE memblock of
  `_newlib_heap_size_user` (weak, default 128 MB). vitaGL then takes everything still free
  minus `ram_threshold` for its own RAM pool — vendor/vitaGL/source/vgl.c:563-570
  (`size_user > ram_threshold ? size_user - ram_threshold : 0`). With threshold=24 MB that pool
  is ~158 MB, and it is never used: every memlog line in three sessions shows
  `ram_free=157758608` constant while `vram_free` does all the moving (61→33 MB). The five
  2026-08-30..09-01 dumps were `std::bad_alloc` in LoadPNG — heap exhaustion of the 128 MB
  block, not GPU memory. The Vitamain.cpp:57-63 comment claiming "vitaGL maps that SAME heap as
  VGL_MEM_EXTERNAL - textures spill into it" is not what happens: `vgl_mem_get_free_space`
  returns 0 for EXTERNAL by definition (mem_utils.c:600), textures live in VRAM then the RAM
  pool; the heap is consumed by the game's own allocations plus the decode churn in H2.
- Evidence (psp-work/logs/20260902-vpk11/memlog.txt, 24 matches):
  `heap_arena` 48.7 MB (game 1) → 82.9 MB (game 24), `heap_used` 39→44 MB, `heap_free` 10→38 MB.
  The arena ratchets ~1.4 MB/game toward the 128 MB wall while live data grows only ~0.2 MB/game
  (fragmentation, same shape as the 2026-08-04 PSP finding). At that rate the wall is ~30 games
  away, matching "~10 games per crash" before the nothrow guard.
- Fix: in Vitamain.cpp add `extern "C" unsigned int _newlib_heap_size_user = 224 * 1024 * 1024;`
  (file scope, VITA-guarded). The RAM pool shrinks by the same delta (~62 MB left) — still
  larger than the 48 MB HUGE_CACHE_LIMIT on top of 61 MB free VRAM, so textures lose nothing.
  Keep `ram_threshold` at 24 MB. Fold the H2 decode-scratch in so the arena stops ratcheting at all.
- Blast radius: Vita only. Verify: memlog `ram_free` drops by ~96 MB at boot; `heap_arena` after a
  20-game session stays far below the new size; no bad_alloc in a 50-game session.

### H2. Vita: card art is decoded into a pow2-padded RGBA8888 buffer — 2 MB per card, 2.8x what the
notes budget — and that buffer is a transient heap allocation on every load
MEM-EFF / PERF — VERIFIED arithmetic, SUSPECTED impact (no device run)
- Where: JGE/src/pc/JGfx.cpp:1967 `getNextPower2`, :2071-2080 (LoadJPG `tw*th*4`), :2342
  (LoadPNG), :2824 TransferTextureToGLContext frees it after `glTexImage2D`;
  projects/mtg/src/WCachedResource.cpp:161 `size() = mTexHeight * mTexWidth * 4`;
  projects/mtg/include/WResourceManagerImpl.h:16 `HUGE_CACHE_LIMIT 48000000` (Vita).
- Mechanism: the shipped Vita pack is 360x514 full art / 128x186 thumbs (NOTES "MEASURED CHAIN").
  getNextPower2 pads 360x514 → 512x1024 → 2,097,152 bytes decoded and uploaded; the cache
  accounts the padded size, so the 48 MB cache holds 24 full cards, not the ~65 the notes
  assume (740 KB). VRAM free (33-61 MB) holds ~14 before spilling to the RAM pool. The deck
  carousel/deck editor (largest AND most numerous cards) therefore evict constantly
  ("Destroying WCachedResource" churn), and every reload does `new BYTE[rawsize]` (~70 KB) +
  `new BYTE[2 MB]` + `delete[]` both — on the 128 MB newlib heap, interleaved with the small
  long-lived allocations of a running game. That is exactly the allocation shape the PSP
  campaign proved fragments the arena (its answer was the texture slab pool in JGE/src/JGfx.cpp:
  103-200); the Vita has no equivalent, and its arena ratchets (H1 numbers).
- Fix, two independent halves:
  (a) Drop pow2 padding on Vita: `#if defined(VITA) tw = width; th = height;` in LoadJPG/LoadPNG.
      JQuad UV math divides by mTexWidth/mTexHeight so nothing else changes. vitaGL uploads
      uncompressed textures at their real size (textures.c only forces POT for the DXT
      compress path, :877/:1600). 740 KB/card → 65 cards in 48 MB, ~3x fewer evictions, 65% less
      upload bandwidth. Verify on device with one card first (GL_REPEAT wrap on an NPOT texture
      is the one thing to eyeball).
  (b) Persistent decode scratch: two grow-only static buffers (raw file, decoded pixels) reused
      by LoadJPG/LoadPNG, handed to glTexImage2D and NOT freed. Vita is single-threaded on this
      path (WResourceManager.cpp:231 `UnthreadedCardRetriever`), desktop is threaded — so gate
      the scratch to `#if defined(VITA)` or guard with the existing sLoadFunctionMutex (JGE's
      LoadTexture is already called under it, WResourceManager.cpp:1384). Zero decode churn on
      the general heap; the arena stops ratcheting.
- Blast radius: every texture load on Vita (cards, backdrops, fonts). Verify: memlog
  `heap_arena` flat across games; count "Destroying WCachedResource" per deck-editor page.

---

## MED

### M1. pc/JGfx.cpp LoadJPG: no libjpeg error handler (process `exit()`), and a truncated file reads
past the end of `rawdata`
MEM-SAFE (robustness, own asset packs) — VERIFIED by reading
- Where: JGE/src/pc/JGfx.cpp:2051 `cinfo.err = jpeg_std_error(&jerr)` with no `error_exit`
  override → any decode error calls libjpeg's default `error_exit` = `exit()`; :1984
  `jpg_fill_input_buffer` returns TRUE without supplying bytes (the standard idiom inserts a fake
  EOI); :1990 `jpg_skip_input_data` subtracts unchecked. JGE/src/JFileSystem.cpp:631-632
  ReadFile returns `size` even on a short zip read ("TODO what if can't read").
- Mechanism: a truncated/corrupt JPEG (the pack builder already excluded one corrupt VOW pair;
  a short read on a flaky memory card produces the same bytes) → libjpeg runs out of input,
  fill_input_buffer says "more data" with `bytes_in_buffer==0` → the input macro decrements it
  to SIZE_MAX and walks past the buffer (OOB read → garbage tile or data abort), or hits a
  header error → `exit()` → on the Vita the game vanishes to LiveArea with no dump.
  Also :2063 the `output_components` early return leaks `rawdata`.
- Fix: same shape as LoadPNG (:2306): a `jpeg_error_mgr` with `error_exit = longjmp`, a
  fill_input_buffer that feeds a 2-byte EOI marker, and `return` after freeing rawdata; make
  ReadFile return the bytes actually read (`gcount()`) so callers can reject short files.
- Verify: feed a truncated JPEG through LoadTexture on desktop (ASAN build) — currently an
  ASAN read overflow / exit; after the fix a NULL texture and the no-art frame.

### M2. JLBFont::GetStringWidth reads past `mCharWidth[256]` on ARM for any byte >= 0x80
MEM-SAFE — VERIFIED by reading (ARM `char` is unsigned; desktop is signed, so the suite cannot see it)
- Where: JGE/src/JLBFont.cpp:243-259: `ch = *p - 32; if (ch < 0) continue;
  len += mCharWidth[ch+mBase]` — no upper bound; JGE/include/JLBFont.h:16 `MAX_CHAR 256`.
  DrawString (:157) has the `> 127` guard; the width path does not.
- Mechanism: the card corpus carries CP1252 bytes (brief: MTGRules/AllAbilities/card text are
  non-UTF-8). On Vita/Android/PSP `*p` is unsigned, so 0xE9 ('é') gives ch=201, +mBase(128)
  → index 329 → float read 292 bytes past the array (adjacent JLBFont members / heap). Read-only,
  so wrong widths (mis-centred text), not corruption — but it is a real OOB the desktop never
  exercises. Sibling: JLBFont.cpp:52 dereferences `mTexture` one line before the NULL check.
- Fix: `if (ch < 0 || ch > 127) continue;` mirroring DrawString; swap :52/:54.
- Verify: an ARM build with -fsanitize=address is not available; on desktop compile JLBFont.cpp
  with `-funsigned-char` under ASAN and render a card with 'é' — reproduces.

### M3. Vita main loop pacing sleeps after a vsync-blocked swap (30 fps cap is fine; the sleep is
what turns a 25 ms frame into 50 ms)
PERF — SUSPECTED (reasoned from Vitamain.cpp:600-640, not measured)
- Where: JGE/src/Vitamain.cpp:373 `vglWaitVblankStart(GL_TRUE)` (vsync on) + :620
  `vglSwapBuffers` + :632-636 sleep to `TARGET_FRAME_MS 33`.
- Mechanism: the frame timer includes the swap, and the swap already blocks to the next
  16.7 ms vblank. Work ≤ 16 ms → swap returns at 16.7 → sleep 16.3 → 30 fps, fine. Work in
  (16.7, 33] → swap returns at 33.3 → no sleep → 30 fps, fine. Work in (33.3, 50) → swap at 50,
  no sleep → 20 fps; the manual sleep never helps and the vsync already caps the rate, so the
  sleep is dead weight and the cap should be expressed as a swap interval instead. This is the
  regime the owner's "severe performance issues" game (autotap preview, now memoised) lived in;
  with the CPU fix landed the residual pacing is worth one measurement.
- Fix: drop the sleep when vsync is on (vsync IS the cap); or `vglWaitVblankStart(GL_FALSE)`
  + keep the sleep for a non-tearing 30 fps via vitaGL's swap-interval control. Measure with the
  existing memlog/heartbeat: add frame dt to the WAGIC_VITAMEMLOG line at untap.

### M4. JGuiController::RemoveAt deletes the wrong object and leaves a dangling pointer
MEM-SAFE / LEAK — VERIFIED by reading; currently DEAD (no callers in projects/mtg or JGE)
- Where: JGE/src/JGui.cpp:243-261: `mButtons.erase(begin()+i); delete mButtons[i];` (same for
  mObjects). erase shifts, so the delete hits the element that moved INTO slot i (the successor),
  the intended object leaks, and the successor stays in the vector deleted → Update/Render
  UAF, double delete in ~JGuiController → heap corruption on the next `free`. It is public and
  virtual on the base of SimpleMenu/DeckMenu/IconButtonsController/SimplePopup/StoryDialog —
  the same class whose `Add(0, "...")` overload trap produced this week's vpk9 NULL button.
  Nothing calls it today (grep: only JGui.cpp's own Remove(int)/Remove(JGuiObject*), which
  have no callers either).
- Fix: `JGuiObject * o = mButtons[i]; mButtons.erase(...); delete o;` — or delete the three
  functions (CRUD) so the trap cannot be armed.
- Also JGui.cpp:176 — the click-to-select loop dereferences `mObjects[i]->getTopLeft` with no
  NULL guard while the Update/Render loops around it got guards this week; `mCursor` is never
  initialised in the ctor (never read either — CRUD).

---

## LOW

### L1. zipFS: destroying an izfstream without close() leaks its buffer — and a pooled zip buffer
stays "used" forever (4 of those and every core.zip read fails)
LEAK (latent) — VERIFIED by reading; all 5 game call sites close today (checked GameStateMenu
372/425, MTGDeck 460/536→621, OptionItem 329→355)
- Where: JGE/src/zipFS/zstream.h:160 `~izstream() { rdbuf(NULL); }` under USE_ZBUFFER_POOL
  (zstream.h:55) — no unuse, no delete; zfsystem.cpp:114-130 `getValidBuffer`: >3 pool entries
  all in use → returns NULL → `File.setstate(badbit)` → the open fails. Real-file opens install
  a `new filebuf` (zfsystem.cpp:198) that the destructor also drops with its fd open.
  izfstream's ctor (zfsystem.h:243) leaves `m_Zipped`/`m_Used` uninitialised.
- Fix: `~izfstream() { close(); }` and init the two flags. Removes a footgun on the Vita, where
  core.zip is the only zip and 4 unclosed opens would blank every resource for the session.

### L2. pc/JGfx decode buffers: pad rows/columns are never zeroed
PERF/visual — SUSPECTED
- Where: JGE/src/pc/JGfx.cpp:2080 (JPG), :2342 (PNG) allocate `tw*th*4` and write only w*h;
  upload uses GL_REPEAT (:2795/:2812) + GL_LINEAR. Sampling the last real texel blends with the
  garbage pad column/row → a 1-px fringe on the right/bottom edge of every card/backdrop. The
  PSP path zeroes in TexAlloc for exactly this reason (JGfx.cpp:426-432 "the roaming green
  block"); desktop/Vita never did. Candidate for the 2026-08-09 "battlefield 1px line".
- Fix: memset just the pad column and the pad rows (cheap), or the H2(a) no-padding path
  removes it on Vita outright.

### L3. LoadPNG leaks on the longjmp path
LEAK — VERIFIED by reading
- JGE/src/pc/JGfx.cpp:2306 setjmp handler frees `rawdata` only; a libpng error after :2328
  leaks `line` (malloc) and after :2342 `buffer` (up to 2 MB). Corrupt PNGs only (own assets).
  Fix: hoist `line`/`buffer` above setjmp, free both in the handler.

### L4. Dead engine modules compiled into every target, two carrying latent memory bugs
CRUD — VERIFIED (0 users in projects/mtg for each)
- JAnimator.cpp:50 and JResourceManager.cpp:101 parse XML from `new char[size]` with no NUL
  terminator (tinyxml reads until NUL → OOB read); JTTFont.cpp:315 writes glyph rows into
  `texBuffer[mMaxCharWidth*mMaxCharHeight]` with `j` unbounded by the row width (wide glyphs
  overflow the heap) — none is reachable, all are linked (CMakeLists.txt JGE_SOURCES, wagic.pri).
  Also unused: JParticle*/JDistortionMesh/JOBJModel/JSpline/JGBKFont/Downloader/JNetwork/
  JSocket (JNetwork has 7 mentions but the transport is a relic), glutmain/Xmain/winmain/
  Qtmain/JGfx-fake (JGE/Makefile:25 still names Xmain.o). Removing them from the Vita/desktop
  source lists shrinks the 33 MB self and the attack surface; zero behaviour change.

### L5. Vita: GetResourceFile extracts sounds from core.zip into ux0:data/Wagic/<path> but
openForWrite creates parent dirs only on Android
LEAK-ish / functional — SUSPECTED (no device evidence either way; DebugTrace is invisible there)
- Where: JGE/src/pc/JSfx.cpp LoadMusic/LoadSample (VITA branch) → JFileSystem.cpp:799
  GetResourceFile → :508 openForWrite (mkdir chain is `#if defined(ANDROID)` only).
  `ux0:data/Wagic/sound/sfx/` does not exist (EnsureUserDirs makes player/ and settings/), so
  the copy fails and every sample/music load returns silently NULL unless VitaShell users
  created the dirs. If the Vita has sound today this is moot; if it is silent, this is why.
- Fix: make the mkdir chain unconditional (MAKEDIR is already platform-mapped), or better,
  let SDL_mixer read from memory (`Mix_LoadWAV_RW` on the bytes ReadFile already produces) and
  drop the extraction copy entirely — it is the "AVOID Using This function!!!" path.

### L6. Per-miss cost of a missing resource on the Vita
PERF — VERIFIED by reading (ziplog: 171 `zip-entry-miss` lines, e.g. `10E thumbnails/0.jpg` for
card id 0 requested repeatedly)
- Each miss: cardFile → AttachZipFile (2 `FileNotZipped` ifstream opens: ux0 then app0,
  zfsystem.cpp:391) → dir miss → `openForRead(mFile, "thumbnails/0.jpg")` (2 more opens) →
  Attempt's `fileExists` (2 stats) = 6 filesystem calls on the memory card per miss, retried
  up to kMissTombstoneAt times spaced by the miss ledger. Cheap on PPSSPP (notes), unmeasured
  on the Vita's sceIo. Fix: tombstone id 0 / token ids at the game layer (they never have art),
  and let the ledger tombstone after the FIRST miss when the set zip itself parsed fine
  (`dir>0` in the breadcrumb already distinguishes the two cases).

---

## The Aug-28 `_malloc_r` heap-corruption dump — what this slice can and cannot say
No candidate in the JGE layer WRITES out of bounds on the Vita build: M4 is dead code, M2 and
M1 are reads, L4's writers are not linked into any call path, the zip pool's UAF (L1) needs an
unclosed stream that no caller leaves. The Vita-specific amplifier is real though: H1/H2 mean
the newlib heap is continuously handing 2 MB decode blocks back and forth next to the game's
small objects, so a stale pointer from the game layer (the class the PSP combat/cursor UAFs
belonged to) lands in recycled memory within frames rather than sitting harmlessly as it does
on a desktop heap. Recommendation, not a diagnosis: land H2(b) (decode scratch) first — it
removes the churn that makes the corruption land quickly, and any surviving corruption then
points squarely at the game layer, where desktop ASAN self-play can hunt it. The owner's memlog
(mallinfo at untap) is still the right instrument; adding `heap_free` deltas per LoadTexture
under WAGIC_VITAMEMLOG would show whether decode blocks are what the corrupter overwrites.

## Notes outside my slice, surfaced while reading (for the owning slices)
- CMakeLists.txt:75 compiles the Vita with `-std=c++11` against the gnu++14 pin — builds today,
  but any gnu++14 construct in a later lane breaks only the Vita build 20 minutes into
  AIPlayerGPT.cpp.
- WAGIC_FIZZLELOG is on for every Vita alpha build (CMakeLists.txt:294): 2,146 lines/session of
  fopen/append/fclose to the memory card from the resolve path — the owner's diagnostics-out-
  of-release rule applies once the fizzle hunt closes.
- SDLmain.cpp:847-861 raises RLIMIT_NOFILE to the hard limit because "the art burst exhausts
  1024 fds". With fds flat at 12 across the headless suite (measured above) the consumer must
  be in the threaded art path (ThreadedCardRetriever + per-open filebufs), which this run did not
  exercise; a windowed game with art and an fd sampler would name it. The raise hides it.
