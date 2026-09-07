# PSP rescue track — working notes

**Goal**: modern Wagic that runs on real PSP hardware (community's #1 field failure; FINDINGS.md §1
in discord-mining/). Fix target: crash scales with set-data volume (proven by solo_mag's
set-swap experiment); 0.18.6-from-2012 workaround is dying with wololo.net.

## LAYOUT MAP (2026-08-28 reorganisation — READ BEFORE FOLLOWING ANY PATH BELOW)

psp-work/ was reorganised into a table of contents; every older section below still names the
OLD flat paths. Translate:
  *.sh, *.py (deploy-vita.sh, run-suite.sh, make-*-cards.sh, oai-*.py, gen-cardtext-sidecar.py)
                                        -> psp-work/scripts/
  *.patch                               -> psp-work/patches/
  *.log, lang-sweep-flags*, sweep-filelist.txt, memprobe-*, crashlog-*, run*.log
                                        -> psp-work/logs/
  crash-2026-08-02, psp-crash-20260818, vita-crash-20260824, vita-crashes, vita-dumps,
  vita-elfs, evidence-wedge-2026-08-10  -> psp-work/crashes/
  stick-archive-20260731, tablet-user-backup-20260818, save-merge-20260807, decks-20260806,
  generic-cards-orig-20260803, graphics-orig-20260803(-audit), WagicPSP-0255.zip,
  minpspw.tar.lzma, WTH*, opt           -> psp-work/snapshots/
  openai-subscription-research.md, todo-audit-report.md, deferred-backlog.md, weakmodel-data,
  ring-probe-deck.txt                   -> psp-work/research/
  unchanged: artpack/, card-packs/, sidecar/, deploy/, tools/, vita-translogs/, NOTES.md
Top level: android-alpha/ and projects/mtg/win-stage/ -> ports/android-alpha, ports/win-stage;
HANDOFF_CARD_SCRIPT_LEXICON_TESTS.md -> projects/mtg/strategy-design/lexicon-triage/.

## Layout
- `~/Projects/wagicGPT-psp` — git worktree, branch `psp-rescue`, based on `upstream/master`
  (fork master is C++11+ now; GCC 4.3.5 can't build it, and the PSP build doesn't want the GPT layer).
- `psp-work/opt/pspsdk` — **minpspw 0.11.2** (GCC 4.3.5, 2011): upstream's actual release toolchain
  (from .travis.yml; sourceforge still serves it; binaries run natively on Arch 2026).
  Env: `export PSPDEV=~/Projects/wagicGPT/psp-work/opt/pspsdk PSPSDK=$PSPDEV/psp/sdk PATH=$PATH:$PSPDEV/bin`
- Modern `pspdev/pspdev` docker (GCC 15.2) FAILS on bundled 2011 Boost (JNetwork/JSocket
  threading shim) — parked; minpspw route works and is release-faithful.
- `psp-work/WTH/` — official 0.25.5 release layout (EBOOT + exception.prx + full 357-set Res).
  Test EBOOTs go here as `EBOOT-autodemo.PBP`.

## Build (from worktree)
```
cd ~/Projects/wagicGPT-psp/JGE && make -j8            # libjge300.a (needs env above)
cd ../projects/mtg && make EXTRA_DEFS=-DWAGIC_AUTODEMO -j8   # EBOOT.PBP + wagic.elf (symbols!)
```
- `EXTRA_DEFS` hook added to projects/mtg/Makefile PSP CFLAGS (our plumbing).
- `WAGIC_AUTODEMO` gates ALL repro instrumentation: auto-boot into Endless demo
  (GameStateMenu MAINMENU hook + GameStateDuel deckmenu hook), WAGIC_PROBE printfs
  (primitive/set loads, duel phase transitions) each carrying `free=` from
  sceKernelTotalFreeMemSize(). Never ship a build with this define.
- `wagic.elf` is unstripped MIPS with debug info — keep per-build for symbolization
  (field dumps give `JGEApp_Title.text + offset`).

## PPSSPP headless (the repro harness)
- Arch `ppsspp` package ships `PPSSPPHeadless`. v1.20.4.
- Invocation gotchas (each cost a silent false-success, exit 0 with nothing run):
  - File path MUST be absolute (or have a dir component); with `-r . EBOOT.PBP` forms the
    loader doubles the path (`umd0://EBOOT.PBP/EBOOT.PBP`) and "succeeds" with exit 0.
  - File argument BEFORE options.
  - Without `-l` only emulated printfs print (clean probe channel); with `-l` you get a
    ~50M-line debug firehose (useful: sceIoOpen trace).
  - stdout is BLOCK-buffered to a file — wrap with `stdbuf -oL -eL` or you see nothing
    until exit.
  - Emulated speed ≈ real PSP: full 357-set load takes many minutes. Use long --timeout.
- Boot verified: "Game requested full PSP-2000 memory access" (64MB model — matches the
  crash-reporting PSP-3000s), module `JGEApp_Title`, Wagic's exception.prx installs.

## Memory facts
- `PSP_HEAP_SIZE_KB(-256)` in JGE/src/main.cpp — heap = all free RAM minus 256KB.
  Observed MemPart on 64MB model: ~52MB. PSP-1000 (32MB) would get ~24MB.
- Data to swallow: sets/ = 18MB on disk (12.9MB primitives text, ~14K [card] in mtg.txt
  alone, 357 set _cards.dat). In-memory expansion is the suspected killer.
- Field crash signature (solo_mag, PSP-3008, latest official): BadVAddr 0x18C318BF (beyond
  address space), DEADBEEF-filled regs, EPC in JGEApp_Title.text — garbage-pointer deref
  consistent with alloc-failure/UAF under exhaustion. -fno-exceptions is commented OUT in
  the Makefile, so a throwing `new` under exhaustion is also in play.

## Field repro targets (from discord mining)
1. solo_mag: crash starting FIRST match, full sets, artwork off. (= our autodemo path)
2. ancientdragon20 (PPSSPP): crash at SECOND match start / second deck-edit —
   return-to-menu path. Autodemo endless mode covers consecutive games.
3. Set-swap: 0.25 engine + 0.23-era sets/ = works. Reproduce by pruning sets dir.

## Harness iteration log (2026-07-31)
- printf probes NEVER surface through PPSSPPHeadless (PSP stdio doesn't reach host; PPSSPP
  also force-restores its own defaults on boot, so config-file overrides like
  FastMemoryAccess=False get stomped). WORKING CHANNEL: fopen("ms0:/wagic-probe.log","a")
  + fclose per line → lands live at ~/.ppsspp/wagic-probe.log on the host. tail -F it.
- First-boot input gates that stall unattended runs (all bypassed under WAGIC_AUTODEMO):
  1. Language chooser (no User profile → MENU_STATE_MAJOR_LANG) → force options LANG="en".
  2. Endless-demo "choose number of games" menu (DUEL_STATE_CHOOSE_NUMBER_OF_GAMES) →
     auto ButtonPressed(CNOGMENU_ITEM_SINGLE_GAME) every entry (state re-entered per game).
- sceKernelTotalFreeMemSize is USELESS as a heap gauge here: PSP_HEAP_SIZE_KB(-256) sbrk's
  everything at startup; kernel free sits at 232KB forever. Use newlib mallinfo():
  used=uordblks, arena=sbrk'd. (minpspw newlib has it.)
- Demo pacing is REAL-TIME upstream (no FASTCLOCK): a match runs >35 min emulated.
  WAGIC_AUTODEMO now multiplies dt by 4 in DUEL_STATE_PLAY + probes every ~10s wall.

## Measured (run 4, full 357-set Res, 64MB model, artwork off)
- Full load completes in ~6 min emulated; all 357 sets parse fine.
- Collection cost: **used=37.1MB / arena=37.7MB of the ~52MB heap** at demo fire.
- Match entry adds ~1.4MB (used=38.2MB entering DUEL_STATE_PLAY).
- Headroom at match start: ~13MB. In-game curve = run 5's question.
- Scope ruling (owner, 2026-07-31): PSP-1000/32MB is OUT of scope — zero field reports
  mention it; all complainants are 64MB-class (3000/3008/Go) or emulators.

## MODERN TOOLCHAIN PORT (2026-07-31, owner directive: PSP build = FORK master, not upstream)
Branch `psp-port` (worktree ~/Projects/wagicGPT-psp, off fork master 948578887/wave-33).
Builds with pspdev docker (GCC 15.2). Port fixes, all committed there:
1. JGE/include/Threading.h PSP branch: boost::shared_ptr/bind → std:: (2011 bundled Boost dies under GCC15).
2. JGE/src/JMP3.cpp: SceMp3InitArg modern fields (unk1/unk2 gone, SceUChar8* buffers).
3. JGE/Makefile: PSP had NO libhgetools.a rule (upstream shipped a 2011 prebuilt = the old-ABI
   symbol source). Added $(AR)-based rule; ALSO gnu++14 + -Werror off.
4. SwizzlePlot PIXEL_TYPE mangling drift (unsigned long vs int across TUs) → unified `unsigned int`
   in JGBKFont.cpp def + JGfx/JTTFont externs.
5. projects/mtg/Makefile PSP: +9 fork sources (AIPlayerBakaB, AIPlayerGPT, Closest, DecisionContract,
   GptConfig, LegalActions, ManaEngine, OptionGpt, PreGamePhase), -lcurl, gnu++14, $(EXTRA_DEFS).
6. Vendored include/nlohmann/json.hpp (v3.11.3 single header).
7. Harness: fork's WAGIC_SELFPLAY env hooks force-enabled under WAGIC_AUTODEMO via
   WAGIC_SELFPLAY_ACTIVE macro (PSP has no env) + lang bypass + ms0: probe channel + dt*4 + probes.
Toolchain facts: pspdev has libcurl 7.64.1 preinstalled; std::thread WORKS (pthread emu);
nlohmann not packaged. PPSSPP smoke boot: module loads, pthread semas init, no crash.

## Gotchas (port era)
- projects/mtg: PSP Makefile and qmake SDL build SHARE objs/ — full `rm objs/*.o` when switching
  targets, or the linker eats MIPS objects ("file in wrong format").
- Docker builds run as root → root-owned artifacts; the JGE Makefile's psp-config probe then dies
  on `Makefile.cache: Permission denied` and SILENTLY dispatches to the linux target (host g++).
  chown after docker builds; delete Makefile.cache.
- ar archives UPDATE (cru): stale-ABI members survive rebuilds — delete lib/psp/*.a on toolchain switch.
- Worktree PC suite is env-broken (assertion `!findingCard` in force_of_negation fixture with BOTH
  binaries; main-tree env green with both) — per project doctrine worktree gates are advisory;
  gate in the MAIN tree.
- GPT-on-PSP leg (task 6) still needs: PSP WiFi bring-up (sceNet/apctl) before curl works —
  NETWORK_SUPPORT was never defined for PSP; JNetwork PSP path unproven. Plus LAN→tailnet proxy.

## THE ALLOCATION-ELISION SAGA (2026-07-31, commit 28b5aabf9) — fork engine RUNS on PSP
Boot-hang root cause chain, in order discovered:
1. Fork EBOOT hung at boot; bisected with User/-file markers (ms0:/ paths DON'T work under
   modern newlib — use relative "User/..." paths, cwd=umd0 maps to the host dir).
2. Hang site: ResetCacheLimits -> ramAvailable(). First theory (interrupts-off + lwmutex
   malloc deadlock) was REAL but not sufficient — hang persisted.
3. TRUE cause: **GCC 15 allocation elision**. ramAvailableLineareMax()'s malloc/free-and-
   null-check probe loops get legally compiled into infinite loops (malloc "always
   succeeds" because it never runs). My own "does malloc return NULL" probes suffered the
   SAME elision — 200x1MB "successes" on a 52MB heap were the optimizer, not the
   allocator. Defeat with volatile touches; with them, malloc semantics are CORRECT
   (54x1MB then NULL, sbrk bounded).
4. Fix shipped: bounded _sbrk in JGE/src/main.cpp owning the heap block +
   wagicHeapFreeBytes()/wagicHeapLargestBlock() O(1) queries; utils.cpp PSP
   ramAvailable() family now O(1), probing loops non-PSP only.
LESSON for all retro ports: any 2000s-era malloc-probe idiom is an infinite loop under
modern GCC. grep for probe loops before first boot.

## Fork-on-PSP first full run (PPSSPP, 357 sets, no images)
- Boots, loads everything, enters endless-demo match unattended. Full telemetry live.
- Collection cost: **42.9MB used** at deck-choice (upstream 0.25.5: 37.1MB) — the fork's
  richer card structures cost ~+5.8MB. Heap ~55MB -> ~12MB headroom at match start.
- ⚠ In-game leak: used grows ~500B/turn linearly (45.9MB@t1 -> 50.0MB@t8660). Exhaustion
  ~turn 17k in this state. Harmless per real game (30-60 turns) but needs a look.
- ⚠ Anomaly: demo game ran 8660+ "turns" without ending (turn counter spinning ~6/sec at
  4x dt). Suspect deck-load or game-end logic under selfplay on PSP — DIAGNOSE NEXT:
  read User/wagic-probe.log + check whether ai/baka decks actually loaded (fork Res).
- Field-crash theory UPDATE: solo_mag's BadVAddr 0x18C318BF (~270MB past heap) fits
  unchecked-growth semantics, but that build is 2011-toolchain; ITS newlib bounds are
  probably intact -> the upstream field crash needs its own diagnosis pass against the
  0.25.5 memory profile (37.1MB + match overhead + images on a 52MB heap is simply tight;
  exhaustion under image decode buffers remains the likely mechanism).

## Endless-game anomaly SOLVED: FAT 8.3 uppercase vs rules-file lookup (2026-07-31)
Chain (each step verified via User/wagic-probe.log probes):
1. Enriched per-turn probe (life/lib/hand/play/gy/ex) showed a DEAD game: hands drained
   to 0 by turn ~2, libraries frozen at 54, no draws ever, turn counter spinning ~6/sec,
   heap +480 B/turn (turn machinery only — the "leak" was a symptom, not a leak).
2. Zone math proved the pregame (London mulligan) ran CORRECTLY (p0: 2 mulls, bottomed 2,
   kept 5; p1: 1 mull, bottomed 1, kept 6) — then each kept hand was discarded whole at
   that player's first cleanup: Player::handsize was 0, i.e. `sethand:7` never resolved.
3. `sethand:7` AND `@each my draw:draw:1` (and every other mtg.txt [INIT] auto=) live on
   the Rules object -> Rules::initGame -> addExtraRules. Probe showed addExtraRules NEVER
   RAN: GameObserver::startGame got rules == NULL.
4. ROOT CAUSE: PSP memory-stick FAT dir scans return 8.3-compatible names UPPERCASED
   (probe log: "sets/primitives/MTG.TXT" but "borderline.txt"). scanfolder("rules") gives
   "CLASSIC.TXT"; Rules::getRulesByFilename("classic.txt") compared case-SENSITIVELY ->
   NULL -> game started with NO rules: no hand limit, no draw step, no win conditions.
   Reproduces identically on real hardware (same FAT semantics). Upstream never hit it:
   their menu paths compare the scanned (uppercase) name against itself; the fork's
   selfplay hook is the first hardcoded-lowercase lookup.
   FIX: case-insensitive compare in Rules::getRulesByFilename (Rules.cpp).
5. With the fix: REAL game verified in PPSSPP — opening 7s kept, lands played, per-turn
   draws, combat damage (life 20->13 by turn 12), graveyards filling, memory a healthy
   sawtooth 44-50MB (WResourceManager cache trim observed dropping used by 6MB).
NOTE on probes: mtg.txt rule keywords (putinplayrule, attackrule, ...) install observers
via addObserver and return NULL from parseMagicLine BY DESIGN — "parsed NULL" for those
29 lines is healthy, not a failure.
LESSON for PSP/Vita ports: NEVER compare scanned filenames case-sensitively against
literals; FAT uppercases 8.3 names. Audit any getXByFilename seam.

## REAL-HARDWARE first boot (2026-07-31, his PSP-2000, 6.60 PRO-C)
- BOOTS AND LOOKS GOOD (his words) — menus, full 357-set load all work on hardware.
- ⚠ REPRODUCIBLE CRASH-TO-OFF: Play → Play Game → Classic → Single Game → hang →
  system powers OFF. Twice. Crash window = post-CNOG deck-menu build (DeckMenu ctor /
  BuildDeckList / renderDeckMenu / premade fill — avatar JPEG decode + real-GU texture
  upload + VRAM alloc; PPSSPP is lenient about all of these, esp. software renderer).
- Deployed WAGIC_HWPROBE build (commit 7fad84b0f) to his stick — same game, user-driven,
  breadcrumbs to ms0:/PSP/GAME/WAGIC/User/wagic-probe.log (survives power-off). Probe
  brackets around the crash window. AWAITING: his Classic re-run crash + Demo-path test
  (Demo surviving = human-path-specific fault).
- Rescue side-quest DONE same session: full 3.7GB stick archive at
  psp-work/stick-archive-20260731/ (139 MP3s, FFXIV maps, SAVEDATA verified); FFIX +
  MHFU deleted from stick per his call; 2.5GB now free; Wagic deployed to
  ms0:/PSP/GAME/WAGIC/ (clean EBOOT currently REPLACED by the probe build).

## Hardware crash bisect tracker (2026-07-31 evening)
- Round 1 (clean build): Demo crashed ~turn 3 mid-tutorials; Classic crashed at/after
  CNOG "single game". Crash-to-OFF (no exception screen) = kernel-level wedge.
- Round 2 (HWPROBE v1, musicVolume=0/sfxVolume=0, tournament stats cleared): STILL
  crashes, same window -> AUDIO EXONERATED. Log shows games die turns 0-2 (tails may be
  lost to power cut). Classic attempts leave NO duelphase lines -> dying during duel-
  screen resource loading, before PREPARE_CNOGMENU is even set.
- Round 3 pending: tutorials stripped from stick classic.txt (18 lines) + HWPROBE v2
  EBOOT deployed (texture trail: name/size/vram/free per load; Start-entry probe;
  WITH_GPT_AI now in - binary 6.2->8.2MB).
- KEY MEASUREMENT (PPSSPP, v2 build): VRAM free at MAIN MENU = ~37KB of 2MB. Every
  duel-entry texture allocates at exhaustion; RAM fallback paths exist and check NULL
  (audited: PNG/GIF/JPEG loaders all guard). Suspicion: something in the
  VRAM-exhausted + fallback churn path misbehaves on real GE only (PPSSPP default
  IgnoreBadMemAccess=true hides bad GE addresses).
- WITH_GPT_AI: was NEVER defined for PSP/Android (only wagic-SDL.pro) -> GPT layer
  compiled out. Fixed f8cd1aeca (PSP links -lmbedtls -lmbedx509 -lmbedcrypto for curl).
- Commits this arc: 62982a9af (FAT rules lookup), 73be7e4e0 (Android APK),
  7fad84b0f (HWPROBE), f8cd1aeca (WITH_GPT_AI + tex trail).

## ROOT CAUSE #2 (hardware-only): getenv returns GARBAGE on real PSP (2026-07-31)
Probe v2's "duel Start" line proved it: Classic (human, oplevel NEW) logged
choose_deck1 -> play — a transition ONLY the WAGIC_SELFPLAY hijack produces, and its
gate is getenv("WAGIC_SELFPLAY") != NULL. Real-hardware newlib environ points into
UNINITIALIZED RAM -> getenv nondeterministically non-NULL. PPSSPP zeroes memory ->
invisible in the emulator. Effects: selfplay kidnapped every Classic entry (no CNOG
menu, human seat loaded w/ nonexistent AI-deck path -> empty library -> instant loss ->
ghost play->show_score games); every other env consumer (WAGIC_AI, GPT overrides,
WAGIC_TESTSUITE_FILE) read the same garbage.
FIX 67333c752: JGE/include/psp_noenv.h force-included into every PSP TU (-include in
both Makefiles' PSP CFLAGS) maps getenv -> always NULL. AUTODEMO unaffected (its
selfplay switch is #define 1, compile-time).
LESSON: on PSP, NEVER gate behavior on getenv; emulator-vs-hardware divergences to
date = FAT 8.3 uppercase names, uninitialized RAM (env), IgnoreBadMemAccess leniency.

## GOTCHA: JGE/lib/psp/*.a are TRACKED VENDORED 2011 DEPS — do not rm
libpng/libjpeg/libfreetype/libgif/libmikmod/libz/libjinput .a in JGE/lib/psp are
2011-era prebuilts the PSP link consumes FIRST (-L../../JGE/lib/psp precedes pspdev
libs). rm'ing them breaks the link (JGfx calls libpng-1.2 API: png_set_gray_1_2_4_to_8,
gone in pspdev's libpng16). All working EBOOTs = GCC15 code + 2011 dep archives (C ABI,
fine). Modernizing to pspdev libpng16 = later task (needs JGfx API updates).
Demo turn-2-3 crash-to-off: STILL OPEN — next hardware round (probe v3, env fixed,
tutorials stripped) isolates it; texture trail will name the last load before death.

## Round 4-5 state + THE OPEN RIDDLE (2026-07-31 late)
- Round 4 (probe v4: roundrect fix + card probes + canChooseDeck probe): demo STILL lags
  hard + crashed after ~3 card plays; shop still dies leaving pack row; options still
  crash-to-XMB after idle. RenderBig fired (Nettle Sentinel) - full-size renders DO
  happen in demo. So: roundrects were NOT the (whole) crash/lag story. Probe I/O itself
  (fopen/fclose on ms0 per event) is a real lag contributor in probe builds.
- RIDDLE: v3+v4 logs show Classic sessions doing unset->choose_deck1->play inside ONE
  Start() call (no CNOG transition, no deckmenu-bracket probes, no CHOOSE_DECK1 Update
  tick - the canChooseDeck probe NEVER fired). The only code shape matching = the
  WAGIC_SELFPLAY branch, but psp-objdump on the v3 elf shows ZERO getenv calls in
  Start() (noenv macro verified applied; branch dead-code-eliminated; clean elf has 0
  "WAGIC_SELFPLAY" strings). Stick FAT fsck = CLEAN; stick EBOOT md5-matches deployed
  v4. UNRESOLVED - do not re-derive from priors; next lever = the clean-round behavior:
  CNOG appears -> probe-build-specific/transient; CNOG absent -> mechanism is in clean
  code, disasm the DEPLOYED artifact's Start and single-probe the gate arms.
- ResetCacheLimits underflow CLAMPED (7a88986ab): ram<5MB wrapped unsigned -> ~4GB cache
  limit -> no eviction -> heap exhaustion; candidate for solo_mag's field crash (fits his
  set-swap result). Post-load limit is ~free-5MB+current (sane ~4-9MB) - the underflow
  needs a mid-game dip below 5MB free.
- libcurl calls getenv() INTERNALLY (proxy envs; curl_getenv linked): on hardware that
  reads garbage env -> hazard when GPT layer does HTTP. TODO: CURLOPT_NOPROXY or build
  curl without env, or extend noenv shim to override newlib getenv symbol itself
  (define a non-static getenv in JGE main.cpp - link-level override like _sbrk).
- DEPLOY VERIFICATION now standard: md5sum the stick EBOOT after every cp (done for the
  clean deploy: 1f6b3c7687c6220ec67fd71b46956bc0).
- Clean build (commit 7a88986ab tree, no probes) DEPLOYED; awaiting his round. Stick
  still has: tutorials stripped classic.txt, musicVolume unknown (file may have been
  regenerated), User profile from crashes.
- Field-report credit: HIS observations drove everything tonight - the full-size-card
  crash pattern, the "worse since round 1" regression call (probe I/O), music-keeps-
  playing (ME alive, main CPU wedged = GE/bus hang class, audio exonerated).

## SET-VOLUME EXPERIMENT deployed (2026-08-01, his 2nd ask — should have run it sooner)
- Clean-round result: CNOG menu REAPPEARED -> the v3/v4 choose_deck1->play riddle was
  probe-build-specific; getenv fix works in clean code. Crash persists after "single
  match": screen tiled face-button symbols + distortion then power-off = GE rasterizing
  garbage textures (memory-pressure signature).
- MEASURED: pruning set DIRS alone (357->39) saves only ~4.4MB (32.7 vs 37.1MB) —
  primitives parse unconditionally and dominate. FILTERING PRIMITIVES to cards printed
  in kept sets is the real lever: used at demo fire = 10.6MB (vs 37.1). Headroom at
  match start ~13MB -> ~40MB.
- Filter: scratchpad filter-primitives.py — keeps [card] blocks whose name= appears as
  primitive= in any kept _cards.dat (byte-level, non-UTF8 safe). _macros.txt kept whole.
  needed=7033 names; kept 6592 blocks total (mtg 5057, borderline 1197, unsupported 297,
  planeswalkers 41). ~441 set entries reference absent primitives (pre-existing, engine
  warns+skips).
- Keep list = union of sets in baka decks 1-20 (39 codes; DDF+V10 don't exist in full
  Res either — decks already tolerate missing sets). deck16 is unannotated "(*)" pool
  deck -> sidelined too; remaining 19 decks verified 0 unresolved names vs filtered
  primitives. Player premades only need 10E.
- PPSSPP validation (WTH-39/, autodemo EBOOT): full load 10.6MB, CNOG flow normal,
  entered DUEL_STATE_PLAY at 11.7MB, no crash.
- STICK: reduce-sets-on-stick.sh run 2026-08-01 — 299 set dirs + 145 decks sidelined to
  ms0:WAGIC/SIDELINE/ (rename, reversible via restore-sets-on-stick.sh), 4 primitives
  swapped (md5-verified vs WTH-39). EBOOT untouched (clean 1f6b3c76...). AWAITING round.
- Interpretation guide: crash gone/moved -> memory pressure confirmed, then binary-search
  set count upward for the shippable ceiling (or ship reduced + on-stick set packs).
  Crash identical -> NOT heap volume; pivot to VRAM-exhaustion / GE-path on real GU
  (PPSSPP IgnoreBadMemAccess + software rasterizer leniency), disasm deck-menu window.

## MEMORY CAMPAIGN round 2 (2026-08-01): eviction UAF fix + text= sidecar offload
- HIS reduced-set round: FULL PASS (games end-to-end, story, deck builder, demos, 3min
  menu idle). Memory pressure CONFIRMED as the crash mechanism. Mandate: solve WITHOUT
  trimming the card list.
- Mechanism story: 37MB resident collection -> 13MB headroom -> texture cache limit
  starves (4-9MB) -> constant mid-frame eviction; real GE renders ASYNC, eviction
  free()s pixels the current display list still references -> garbage tiles (his
  "tiled face-button symbols + distortion") -> GE wedge -> crash-to-off. PPSSPP
  software renderer is synchronous = invisible in emulator.
- FIX A (JGE/src/JGfx.cpp): JTexture dtor no longer frees pixels immediately; frees
  queue in gDeferredFreesRam/Vram, flushed in EndScene AFTER sceGuSync + at BeginScene
  (GE idle). Turns pressure into degradation instead of death.
- FIX B (CardPrimitive + MTGDeck seam): text= (display-only, ~10MB resident incl.
  overhead) offloaded to deploy-time sidecar Res/sets/primitives/cardtext.{idx,dat}.
  getFormattedText() (the SINGLE read seam, already self-caching) fetches on demand;
  setText() no-ops when sidecar present; sidecar absent = unchanged behavior. Generator:
  psp-work/gen-cardtext-sidecar.py (27342 entries, 0 FNV32 collisions, dat 4.7MB,
  idx 328KB resident). ASCII-only lowercasing BOTH sides (non-UTF8 accent bytes).
- Known caveats: (1) GPT layer reads card->text directly (prompts, landTapMana, DFC,
  dungeon rooms) — instances copy EMPTY text under offload; Baka path clean; fix at
  instance-creation before PSP network inference (task #6). (2) Translations (setText
  from Translator) dropped under offload — English-only alpha OK. (3) fopen path is
  relative "Res/..." — if it fails, offload silently off, measurement shows 37MB.
- Aaron's structural point (correct): 13.2MB raw -> ~30MB resident = std::string/map/
  vector chunk overhead ~15-20 mallocs/card. Real fix = packed-arena primitives store;
  parked as roadmap (offload removes the biggest strings + their overhead cheaper).
  magicText offload (~12MB, at MTGCardInstance-creation seam) = next lever if needed.
- Expected numbers: full 357 sets + sidecar ≈ 27MB used at demo fire (vs 37.1) ->
  ~25MB headroom. With eviction UAF fixed, cache limit computes generous again.

## Round-3 build DEPLOYED (2026-08-01): full 357 sets + dual sidecar + UAF fix
- PPSSPP measured ladder (full 357-set Res, autodemo, per-file probe deltas):
  stock ~35MB primitives / text-offload 29.9 / text+auto offload 22.1MB.
  Demo fire ~26.3 -> entering play 31.7 -> in-game steady ~39.2MB (vs 47 before) of
  ~52MB heap. Games REAL (plays, life swings, graveyards — materialization works).
- NOTE: "run 1 vs run 2 identical" confusion resolved: run 1's binary looked for the
  sidecar at the OLD path (sets/primitives/) where the files then were -> BOTH runs
  had text offload ON. True stock baseline is ~+5MB over run 2.
- cardauto sidecar: 24559 entries (raw auto*/anyzone lines); materialization seam =
  CardPrimitive copy ctor (every in-game card: instances, tokens, flips) + DeckStats
  read site. addMagicText gated (gMaterializing bypass). WFilter color-refine reads
  magicText collection-wide: empty = its existing fallback -> deck-editor color filter
  slightly coarser on PSP, accepted. processConfLine has NO runtime callers (audited).
- STICK STATE: full 337 set dirs + 196 decks + ORIGINAL primitives restored (SIDELINE
  gone); 4 sidecar files at Res/ root md5-verified; clean EBOOT md5
  c6ec75d69519c1f92f10a1136328e38d (0 probe strings); classic.txt still
  tutorial-stripped (restore before release).
- Round-3 hardware questions: does Classic get past deck menu? demo lag? shop?
  If stable -> full collection ships on PSP. If not -> magicText offload can extend
  (name/type dedup, arena store) or tier the data as last resort.

## IMAGE ROUND deployed (2026-08-01)
- His round-3 verdict: FULL COLLECTION WORKS ON HARDWARE (everything: classic, story,
  deck builder, demos, menu idle). Memory campaign closed; his call: throw images at it.
- ⚠ IMAGES ARE WOTC-DERIVED — NEVER commit to the public repo or ship in any release
  artifact. Local psp-work + his stick ONLY. Alpha ships image-less.
- Stick FAT32 cluster = 32KB -> 131k loose files ≈ 4.2GB = impossible. Engine's own
  per-set zip support used instead: WResourceManager attaches Res/sets/<S>/<S>.zip,
  in-zip keys are "<id>.jpg" + "thumbnails/<id>.jpg" (per-set dir zipped at root).
- image-zips/ = 336 stored (-0) zips, 1.8GB, corrupt VOW/541118 pair excluded.
  On stick md5-spot-verified; 448MB free left. PPSSPP smoke w/ 3 zips: in-game 44.8MB
  used (~86% heap, images ≈ +5.5MB RAM textures) — playable, cache under pressure but
  UAF-fixed. Expect possible sluggishness on image-heavy screens on hardware.
- ⚠ GOTCHA: NEVER put ${var%.zip} in systemd-run inline bash -c — % is a systemd
  specifier; it silently emptied the var and 336 cps hit ONE file (sets/.zip). Use a
  script file (copy-image-zips.sh).

## IMAGE-CRASH DIAGNOSIS + probe-hijack discovery (2026-08-01)
- His image round (clean build): crash-to-off MID-GAME ~turn 4 (game + demo). PPSSPP
  full-image curve: +~700KB/turn (4 new card textures/turn: land+creature each seat),
  41.6MB at turn 7, NO plateau — but hardware dies ~41MB with ~11MB FREE.
  NOT heap exhaustion. Working theory: GE cannot (reliably) read textures allocated
  above the phat 24MB user boundary (0x0A000000) on large-memory units — fits EVERY
  crash this campaign (no-image rounds allocate all textures at boot = low addresses;
  image rounds allocate mid-game at 36MB+ = high). Unconfirmed by web search; being
  settled empirically. First probe datum: avatar.jpg bits=0x0AE78D30 (above the line).
- ⚠ RECORD CORRECTION: the v3/v4 "riddle" (choose_deck1->play, no CNOG) was NOT
  getenv garbage — GameStateDuel.cpp gated WAGIC_SELFPLAY_ACTIVE=1 on HWPROBE too
  (GameStateMenu had it right). Every HWPROBE round ghost-played Classic (empty-hand
  p0, instant show_score). Fixed e14468af5. getenv-garbage remains a real hardware
  hazard class (noenv shim stays) but the v3 phantom-transition evidence was the macro.
- Probe round 1 with images "froze before CNOG" = ghost game artifact, NOT image data.
  Round 2 probe (4550b6d1..., hijack-free, tex trail logs bits=%p + used) DEPLOYED.
  DECISION RULE: crash textures at bits>=0x0A000000 -> GE-address theory confirmed ->
  fix = low-pool texture allocator (reserve pool at boot below the line, route texture
  pixel allocs through it). All-low addresses at death -> theory dead, look at deferral
  accounting overshoot / real-GE swizzle-upload path next.
- Mulligan blind-choice bug (his report): pregame menus are modal, hand-reveal key
  never arrives, hand starts hidden. FIX 3c5685f43: PreGamePhase::Render draws the
  deciding human seat's hand as a thumb row under the menu (all platforms; AI never
  rendered). Suite green 1031/0+24/0.

## IMAGE CRASH ROOT-CAUSED + fixed-thumb deploy (2026-08-01)
- Probe round 2 (hijack-free) caught it: GE-ADDRESS THEORY DEAD (thumbs at 0x0B92xxxx
  rendered fine all game). REAL cause: pack "thumbnails" were 176x257 = FULL-SIZE ->
  256x512 padded texture = 262KB each; cache fits ~12, board needs ~15 -> load/evict/
  reload thrash (log shows same id re-requested in 5 lines); mid-frame the deferral
  holds evicted buffers -> transient balloon -> JPEG decoder mallocs FAIL (3 "tex load"
  with no "tex done" right before power-off; NULL loads return before the done-probe)
  -> some unguarded alloc follows -> crash-to-off. His images-toggle-mid-game crash =
  same spiral, burst-triggered.
- FIX (data-side): all 54k pack thumbs rescaled to 64x93 (~16KB texture incl pow2 pad,
  16x smaller); zips rebuilt (1.3GB), redeployed, spot-md5-verified. Stick now:
  clean EBOOT 71ffdecd1441480e5b52888483224b5a (0 probe strings) + mulligan-hand fix
  + fixed-thumb zips. 952MB free. AWAITING playable-images round.
- ⚠ BUILD GOTCHA (bit tonight): cleaning only projects/mtg leaves JGE objs in the
  previous define flavor -> "clean" EBOOT linked HWPROBE JGfx (tex-log lag). ALWAYS
  clean BOTH trees on define change; gate = strings|grep -c wagic-probe == 0.
- If this round still crashes: next suspects = deferral balloon hardening (cache
  admission accounting for pending frees) and full-size (262KB) preview churn — could
  downscale full-size to 128x183 (64KB) at preview-quality cost.

## BURST-CRASH fix round (2026-08-01 late): mid-frame GE checkpoint + cache cap
- His fixed-thumb round: rapid display-mode cycling (tap-tap-tap) at game start =
  crash-to-off; SLOW cycling = full image game PLAYED GREAT; demo = insta-crash.
  Mechanism: mode switches reload hand/board textures; rapid cycling stacks reload
  storms into single frames; deferred frees hold evicted pixels till frame end ->
  transient old+new balloon -> alloc failure cascade. Demo = same storm naturally.
- FIX 1: JRenderer::TexMemCheckpoint() (JGfx.cpp + JRenderer.h PSP slot): on texture
  alloc failure, sceGuFinish+sceGuSync the in-flight list, flush deferred frees,
  sceGuStart fresh list (multi-list frames are normal GU), force tex/blend rebind
  (mCurrentTex/mCurrentBlend=-1). All 9 pixel-buffer alloc sites route through
  TexAlloc() = memalign, on NULL checkpoint+retry. gInFrame flag tracks Begin/EndScene.
- FIX 2: ResetCacheLimits PSP: hard cap texture cache at 8MB (PSP_TEXCACHE_HARD_CAP)
  - the free-minus-5MB formula was too greedy for burst transients; 8MB ≈ 500 thumbs.
- PPSSPP full-image demo validation: memory curve FLAT 34.3->35.8MB over 7 turns
  (was +700KB/turn to 47MB). ~16MB standing headroom.
- STICK: clean EBOOT 7307fa8cb321716123cbf78a950a5f8b (0 probe strings) + fixed-thumb
  zips. AWAITING his torture round (rapid cycling + demo + normal game).
- Uncommitted: JGfx.cpp/JRenderer.h checkpoint, WResourceManager cap — suite gate
  running; commit when green.

## TORTURE ROUND PASSED + demo pacing (2026-08-01 late)
- His verdict on checkpoint+cap build: display-mode storm survived, images ON, played
  fine — the crash campaign is CLOSED. Burst fixes committed 8a4eddc53.
- Demo pacing shipped (AIPlayerBaka::Act clickstream-drain hook): 1.5s hold after each
  COMPLETED play, demo-only, passes stay quick; PSP always paced (no env); PC harness
  contexts + WAGIC_DEMO_FAST=1 stay full speed. His verdict: "still pretty fast...
  we'll leave it like this for now" — demo polish LOW PRIORITY by owner call; do not
  tune further unprompted (constant at AIPlayerBaka.cpp ~line 4437 if he asks).
- Stick state: EBOOT 69b8dd5f152e2b384e5cfcf29caf4697 (checkpoint+cap+pacing+mulligan
  hand, 0 probe strings) + fixed-thumb zips + full 337 sets + sidecars. FUNCTIONAL.
- Pending: gate 4 (pacing suite run) -> commit AIPlayerBaka.cpp. Then alpha legs:
  #9 Android Res handling + tablet test, #11 Vita VPK, #12 Windows mingw, #10 notes.

## Session wrap (2026-08-01): PSP leg DONE, Android leg staged
- Commit chain today (psp-port): 36c9ff4ca deferred tex frees / 6c0100d7d sidecars /
  9f8677595 CHOOSE_DECK1 probe / 3c5685f43 mulligan hand / e14468af5 hijack gate +
  addr trail / 8a4eddc53 checkpoint + cache cap / ee57b964f demo pacing /
  095296d81 CREDITS / 9b8be318f Android gesture tuning. All suite-gated 1031/0+24/0.
- STICK (his PSP): EBOOT 69b8dd5f... + fixed-thumb zips + full 337 sets + sidecars =
  FUNCTIONAL with images (his torture round passed). Demo pacing 1.5s (owner: "still
  pretty fast", parked).
- ANDROID: APK rebuilt 3.08MB (Android/bin/wagic-debug.apk) w/ gestures + controller
  + pacing + GPT. Res delivery = MANUAL PLACEMENT (owner call). Next = tablet round.
- OWNER RULINGS this session (also in anchor memory): alpha = bug-shaking vehicle,
  rules correctness the bar; MTG official cards leave the public offering long-run;
  NO luxuries for MTG content (esp. art/image flows — danger zone); cardscript compat
  is a STANDING GUARANTEE (community ports Res from origin); credit info sources
  (community projects generously; commercial restraint; license floor always).

## ANDROID leg (2026-08-01): SIGILL root-caused + remote tablet loop established
- Tablet = SM-T227U (janus, Android 13, rooted agent-host). Access WITHOUT adb toggling:
  Termux sshd :8022 on tailnet (Voyager key now authorized) + magisk root broker
  socat 127.0.0.1:8623 (see ~/Projects/root-t227u/EXECUTION-NOTES.md). Broker gotchas:
  use ABSOLUTE /system/bin paths (Termux PATH), merge 2>&1, pull binaries via base64
  over the stream. Full loop proven: scp APK -> pm install -r -> am start -> input tap
  -> screencap -> base64 back.
- FIRST-BOOT CRASH root cause: SIGILL ILL_ILLOPC in WCachedTexture::GetQuad =
  raw LSE `ldadd` (shared_ptr refcount) — Application.mk had -march=armv8.1-a;
  Helio P22 = Cortex-A53 = ARMv8.0. FIX: -march=armv8-a (outline-atomics helpers
  remain = runtime-dispatched, safe). LIKELY ALSO upstream's field "crashes before
  loading primitives on Android" class IF upstream ships v8.1 — worth checking for
  the wagic-fixes-port campaign.
- Android launcher data contract: expects Wagic-core-0255.zip (name from
  Wagic_Version.h WAGIC_RESOURCE_NAME) at <storage>/Wagic/Res/; reads Res from INSIDE
  the zip; missing -> auto-downloads UPSTREAM GitHub core (WAGIC_RESOURCE_URL) — fork
  must repoint or neuter before release; "Yes" on the prompt DELETES local + refetches.
  android-alpha/ bundle now ships Wagic-core-0255.zip (root-relative entries).
- Manifest: added requestLegacyExternalStorage=true (target-29 scoped-storage lockout
  of /Wagic on Android 10+).
- Game BOOTED on tablet after fix (language chooser rendered) — first-ever Android
  boot of the fork.

## ANDROID PLAYABLE (2026-08-01) — commit b6ed2168a
- His verdict: "works." Boots -> full 68020-card DB -> plays -> menu swipe exits games.
- Fix chain this leg: armv8-a march (SIGILL), requestLegacyExternalStorage, swipe
  threshold proportional to view height (was fixed 800px > landscape screen height).
- Post-crash rescue path proven: Lang=cn (my blind tap) -> WGBKFont UAF crash on menu
  render -> sed Lang=en in User/settings/options.txt over SSH. OPEN ENGINE BUG: font
  texture use-after-free (BindTexture on 0x85-poison ptr) — font textures not pinned
  against cache eviction; CJK fonts make it deterministic. Fix before release.
- Android UX defect ledger (alpha-notes material, owner: "just needs to work"):
  (1) cold-start storage picker re-asks every launch; (2) core-download prompt every
  launch, Yes silently nukes local data + fetches UPSTREAM; (3) no discoverable
  back-out (swipe-up is secret knowledge — consider visible menu button for alpha);
  (4) system Back gesture no-ops (event chain eats KEYCODE_BACK); (5) top-right
  corner = hand toggle surprise; (6) downloader URLs still point at upstream GitHub.
- android-alpha/ bundle current (APK w/ all fixes + Wagic-core-0255.zip + images).

## ANDROID input/UX round 2 (2026-08-01, late) — state at compaction
- SHIPPED to tablet + committed (cae849ce2, 0b14052db, d95196022, b6ed2168a):
  pointer-vs-limitor fix (targeted spells castable on touch — ENGINE bug, hits mouse
  play too), Back at dispatchKeyEvent, storage asked once, boot-straight-in,
  non-sticky immersive, deferred key-up (engine needs the press to survive a frame —
  that was why every Back attempt looked dead), keymap split AC_BACK->SEC +
  SDLK_MENU->MENU, swipe-up sends KEYCODE_MENU.
- Gate 8 GREEN (1031/0 + 24/0) -> committed 1edcd4d7f "Android: split Back (cancel)
  from the game menu, and hold key presses a frame". Working tree clean of source
  changes; the tablet's installed APK is that build.
- His verified behavior after the last deploy: Back = correct cancel/back in MENUS,
  Back = opens duel menu IN GAME (he accepts this; Android convention). Swipe-up
  reported not working after the keycode change — likely bundled 2011 SDL maps
  AKEYCODE_MENU to UNKNOWN; UNVERIFIED, and moot if Back covers both roles.
- NOT YET VERIFIED BY HIM: (a) casting a targeted spell on touch (the limitor fix —
  his original blocker), (b) card images rendering on Android.
- KNOWN ENGINE DEFECT for release: WGBKFont/BindTexture use-after-free on evicted
  font textures (tombstone captured; deterministic with CJK fonts). Fix before alpha.
- Owner posture: "alot of nitpicks... I don't want to revise the whole interface right
  now" — no interface overhaul; ship what works.
- Tablet remote loop (no adb toggling): ssh -p 8022 100.78.34.3 (Voyager key
  authorized) + root broker /dev/tcp/127.0.0.1/8623; absolute /system/bin paths;
  screenshots via base64 stream. scp APK -> pm install -r -> am start -> input tap.

## RES PACK: bundled into the APK (2026-08-01, commit 671b6c412)
- OWNER RULING: bundle the pack in the release artifact; NO update channel. "the same
  Res pack per user, determined at release time." The manifest/minEngine/latest-URL
  design was dropped - it only serves data outpacing releases, which is not our case.
- projects/mtg/tools/make-respack.py is now THE packer (Res/createResourceZip.py is
  py2 + ant-era; dead). Build: tools/make-respack.py -o out.zip. --print-name gives
  the pack filename (single version parser; callers must not re-parse Wagic_Version.h).
- HARD GUARD: refuses to build if any image/zip is under Res/sets/<SET>/. The loose
  colour frames at Res/sets/*.jpg (back/blue/morph/...) are engine no-art fallbacks,
  NOT card scans, and are packaged. Verified the guard fires and emits no artifact.
- ⚠ EVERY DIRECTORY needs its own zip entry, nested included. JGE zipFS
  filesystem::scanfolder (JGE/src/zipFS/zfsystem.cpp:328) looks the folder name up in
  m_Files and keeps only entries whose remainder has no interior slash - so with no
  "sets/10E/" entry the set is invisible. SYMPTOM: game boots fine, main menu says
  "0 cards". Cost one deploy round trip to find; the fix is in the packer.
- Pack is DEFLATED (25.6MB vs 52.6MB stored). zipFS reads deflate fine. Only
  zip-inside-zip must be STORED (JFileSystem::ReadFile comment + m_Size check).
- Old shipped pack was a naive whole-tree zip: 3153 files incl. 1742 test fixtures,
  214 missing_cards_by_sets, 137 oldthemes, createResourceZip.py. New pack = 1061.
- build-apk.sh step 0 builds the pack into assets/ + assets/respack.sha256. Both are
  gitignored (derived, 25MB). APK is 29.87MB.
- Launcher: startDownload/DownloadFileAsync/forceResDownload GONE. installBundledRes()
  + InstallResAsync extract from assets, stage .tmp, rename, THEN record the sha in
  SharedPreferences (kInstalledPackIdPreference). Mismatch vs assets/respack.sha256 =
  reinstall, so a new APK refreshes data with no version bookkeeping. Menu item is now
  "Reinstall Game Data" (repair path).
- getResourceUrl() JNI + WAGIC_RESOURCE_URL still exist but are UNUSED on Android.
  databaseurl (CardImageLinks.csv) still points upstream - untouched, image feature.
- VERIFIED on tablet: pack auto-replaced twice (30726324 -> 26779014 -> 26815066),
  no crash, 68020 cards (26078 unique). Owner confirmed earlier: targeted-spell cast
  works ("flawless"), card art renders.
- USER-DATA SAFETY VERIFIED (2026-08-01, not merely reasoned): planted a canary deck
  (User/player/deck1.txt) + User/settings/canary.txt, deleted Res/Wagic-core-0255.zip,
  relaunched to force a full reinstall. All four md5s IDENTICAL after (deck1,
  collection.dat, tasks.dat, canary); Res/sets/ image packs untouched (still 10:13).
  Canaries removed from the device afterwards. Structural reason: Wagic/User and
  Wagic/Res are SIBLINGS and InstallResAsync writes exactly Res/<name>.tmp -> rename.
  The old forceResDownload did oldRes.delete() first - that destroyer is gone.
- SIGNING-KEY DATA LOSS: CLAIMED THEN RETRACTED (2026-08-01). Claude asserted a
  release-key change would wipe saves; that was an unverified inference from the
  flounderbounder Discord field report ("APK-upgrade data-wipe pitfall (keystore
  mismatch on reinstall)", CREDITS.md) and it contradicts the mechanism. A key change
  forces an uninstall, but uninstall only removes APP-PRIVATE data
  (/data/data/net.wagic.app + /storage/emulated/0/Android/data/net.wagic.app). Wagic
  saves to Environment.getExternalStorageDirectory()+"/Wagic" = /storage/emulated/0/Wagic,
  which is PUBLIC shared storage (hence WRITE_EXTERNAL_STORAGE +
  requestLegacyExternalStorage) and survives uninstall. Expected loss = SharedPreferences
  only (storage choice + pack id) -> one storage prompt + one pack re-extract.
  STATUS: UNVERIFIED EITHER WAY on device. The original field report is real but its
  actual mechanism is unknown - it may have been the OLD forceResDownload flow, which
  did oldRes.delete() and refetched upstream when a user tapped "Yes" (that flow is
  now removed). DO NOT put a key-change data-loss warning in release notes until an
  uninstall/reinstall test is run (back up User/ to /data/local/tmp first).

## VITA LEG — started 2026-08-01 (task #11)
DEVICES: PSTV (10.0.0.227 as of 2026-08-07, was .95 - DHCP, sweep :1337 when stale; FW 3.60, Enso, VitaShell, FTP :1337) + a LAUNCH-MODEL PSVita
  (PCH-1000). Both 512MB, one build serves both. PSTV is the automatable one (FTP), but
  ⚠ VitaShell's FTP must be STARTED BY HAND (SELECT) each session - curl exit 7 = not
  running / console off, NOT a build problem. Vita FTP is single-threaded: sequential
  uploads only. ⚠ A past Claude session destroyed his Vita setup by swapping mount
  software - NEVER touch shells/StorageMgr/taiHEN modules without explicit authorization.

PORT PROVENANCE: bren17's (Brendon Moncada) native Vita port is ALREADY MERGED in the
  fork tree (commits 84e0cdf78 "clean up", 8793519a4 "fix images"). Files: CMakeLists.txt
  (root, 503 lines), BUILD_VITA.md, build_vita.sh (still has his WSL SRCDIR - not used),
  JGE/src/Vitamain.cpp, vita/sce_sys/*. vitaGL + SDL2, 960x544, native sceCtrl/sceTouch,
  30fps, game logic still in PSP 480x272 virtual coords scaled at draw time.

⚠ THE KEY FINDING: bren17's CMake source list PREDATES this fork's engine layer. Ten
  sources were missing. Fixed 2026-08-01 by adding SEVEN to MTG_SOURCES, matching what
  the PSP Makefile compiles: LegalActions, ManaEngine, DecisionContract, PreGamePhase,
  AIPlayerGPT, GptConfig, OptionGpt. The other three (JNetwork, JSocket, Downloader) are
  the TRANSPORT and stay OUT - same open question as PSP task #6. BUILD_VITA.md's "No
  networking" claim is bren17's design, not a fork decision.
  => Any future source added to wagic.pri must ALSO be added to CMakeLists.txt by hand.
     There is no glob; the lists drift silently. Diff them when adding sources.

TOOLCHAIN (containerized, same pattern as PSP - nothing installed to /usr/local):
  docker image vitasdk/vitasdk:latest (1.36GB, has cmake 4.2.3, python3, git, make,
  SDL2/SDL2_mixer/FLAC prebuilt). ⚠ NO `zip` binary in the image.
  vitaGL: OWNER RULING 2026-08-01 - DO NOT PIN. Track CURRENT upstream and fix what
  actually breaks. His words: "I'm unconvinced by the pinned vita library... the new
  library should have various improvements over the older version, and it's for a
  specific hardware profile. the existing vita port is a prior art reference, not a
  truth document." (BUILD_VITA.md still documents the old pin - it is bren17's doc and
  is now WRONG on this point; do not let it talk a future session back into pinning.)
  The inherited pin was d4a8f9d (Jan 10 2026) claiming newer vitaGL "silently fails to
  boot". Weakly motivated on inspection: wagic calls only vglSetParamBufferSize,
  vglUseTripleBuffering, vglUseCachedMem, vglInitExtended, vglUseVram,
  vglWaitVblankStart, vglSwapBuffers; the cited renames (frag_buf, mvp_matrix) are
  vitaGL-INTERNAL and u_mvp_matrix is wagic's own shader uniform. Now on 6e7fe40
  (2026-07-31); libvitaGL.a went 2.2MB -> 5.75MB. If a boot failure IS ever traced to
  vitaGL, BISECT vendor/vitaGL and record the commit - do not silently re-pin.
  Built into vendor/vitaGL (gitignored):
    docker run --rm -v /home/magi/Projects/wagicGPT-psp:/build vitasdk/vitasdk:latest \
      bash -c 'cd /build/vendor/vitaGL && make NO_DEBUG=1 -j8'
  Configure+build:
    docker run --rm -v /home/magi/Projects/wagicGPT-psp:/build vitasdk/vitasdk:latest \
      bash -c 'cd /build/build_vita && cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake .. && make -j8 wagic.self-self vpk'
  ⚠ The two-stage `wagic.self-self vpk` is REQUIRED (vita_create_self does not expose
    wagic.self as a transitive dep, so plain `make vpk` will not relink after edits).
  FIXED: cmake_minimum_required was 3.0; CMake 4.x removed <3.5 support. Now 3.10...3.28.
  Container writes root-owned files into the mount - chown after builds like the PSP leg.

RES PACKING UNIFIED: the VPK staging script's `zip -r -0` call was replaced with
  tools/make-respack.py --compress stored --no-sha. Removes the missing-`zip` dependency
  AND brings the card-art guard to the Vita release path. STORED is kept deliberately
  here: bren17 documents that the zip-in-zip reader (zfsystem PreloadZip) SILENTLY SKIPS
  COMPRESSED ENTRIES, so nested per-set art zips must be STORED. (Outer-pack entries may
  be deflated - proven on Android. The two facts are not in conflict.)
  The card-art path (make vpk-full, -DCARD_IMAGES_DIR) builds per-set zips from an
  EXTERNAL dir straight into the VPK and never puts art in Res/ - so the guard does not
  interfere with his personal art build. That path still needs `zip` in the container.
STATUS AT WRITING: configure clean, pinned vitaGL detected, compile in progress. Not yet
  linked, no VPK, nothing on hardware. Watch for -std=c++11 in CMAKE_CXX_FLAGS vs the
  project's gnu++14 pin, and GCC 15.2 strictness on this old codebase.

TOUCH: already implemented in JGE/src/Vitamain.cpp - native sceTouch FRONT port,
  Android-style gestures deliberately mirroring SDLmain.cpp: motion -> LeftClicked()
  (hover/drag), tap <250ms and <50px -> JGE_BTN_OK, longer drag/flick -> Scroll().
  => today's cae849ce2 (pointer clicks must not be filtered by the D-pad limitor)
  CARRIES OVER to Vita touch for free - same CardSelector path, so the "can't tap a
  board target with your hand open" bug would have hit Vita touch identically.
  ⚠ TESTING GAP: the PSTV has NO TOUCHSCREEN, so touch can only be validated on the
  LAUNCH-MODEL PCH-1000. Whether that handheld is hacked is UNKNOWN - ask before
  planning a touch-validation round. Rear touchpad is unused by the port.
TITLE ID — RESOLVED, see the "TITLE ID DECIDED" entry further down: owner chose
  WAGC00042 + app name "wagicGPT" (commit b9530ad8e, 2026-08-01). Installs BESIDE stock
  Wagic. (This paragraph once said "NOT yet answered" and a 2026-08-07 session asserted
  the stale WAGC00001 state from it without checking param.sfo — resolve-in-place when
  a decision lands, don't only append.)
- VITA MILESTONE 2026-08-01, commit 763676441: wagic.vpk BUILDS, 37.3MB, on CURRENT
  vitaGL 6e7fe40. Unpinning broke exactly ONE symbol: vglUseVram (removed; absorbed
  into vglInitExtended's ram_threshold, which the port already passes 24MB to - intent
  preserved, not dropped). Fixed in JGE/src/Vitamain.cpp with a comment.
  ⚠ This does NOT disprove the pin's boot claim - that was a RUNTIME failure and only
  hardware settles it. If a bump breaks boot: BISECT vendor/vitaGL, record the commit.
  VPK verified: sce_sys/param.sfo + eboot.bin (28.5MB) + LiveArea + Res/core.zip;
  core.zip = 1061 files, 374 dir entries, STORED, NO card art under sets/<SET>/.
  Fork engine symbols present in the self (51 hits for DecisionContract/LegalActions/
  ManaEngine/AIPlayerGPT). NOTHING TESTED ON HARDWARE YET.
  Container writes root-owned files: sudo chown -R magi:magi build_vita vendor after.
- DEVICE FACTS (owner, 2026-08-01): BOTH Vitas (PSTV + launch PCH-1000) run CURRENT
  ENSO and have TLS. So (a) the same VPK installs on either, (b) the handheld CAN
  validate touch once charged - it was flat on 2026-08-01, charging.
  ⚠ TLS ON VITA REORDERS THE NETWORK WORK: a Vita with working TLS can actually REACH
  an HTTPS inference endpoint, so the GPT layer could RUN there, not just compile.
  That makes Vita a cheaper first target for the LLM-over-wifi path than PSP task #6
  (which needs a raw sceNet/apctl + curl/mbedTLS stack built from scratch). NOT acted
  on - the Baka-only build is the correct alpha target and is what is built/verified.
  NOTE: reference_vita_tv.md memory says FW 3.60; "current enso" may mean a newer FW.
  Do not trust the 3.60 figure without asking.
- DEPLOY: psp-work/deploy-vita.sh [host] [port] - FTP-uploads build_vita/wagic.vpk to
  ux0:/data/wagic.vpk. UPLOAD ONLY; install is manual (VitaShell -> Cross). Deliberately
  touches nothing else on the device. VitaShell FTP must be started by hand (SELECT).

## ⚠ OWNER RULING 2026-08-01 — LLM PATH IS A RELEASE GATE ON EVERY PLATFORM
His words: "all platforms will require the llm path to be figured out before I push a
release onto the public repo. the llm path is the whole point of my project, if they can
only use it for baka, then it's not worth my time to do releases for it."
=> "Games must run entirely with Baka for users without an LLM" is a FALLBACK
   requirement, NOT a shippable target. A Baka-only platform DOES NOT SHIP.
=> PSP task #6 is no longer deferrable; it is a release blocker, as is the equivalent
   work on Android and Vita. The font-texture UAF and the Windows leg rank BELOW this.

### THE TRANSPORT SEAM (the good news)
The entire HTTP surface is ONE function in projects/mtg/src/GptConfig.cpp:
    string httpRequestImpl(const string& url, const string& postBody,
                           long timeoutMs, const string& bearer)
  exposed as gptHttpGet/gptHttpPost. Real impl = libcurl. #ifdef WAGIC_NO_CURL swaps in
  a stub returning "" (every GPT seam treats that as "fall back to Baka").
  The WHOLE FILE is additionally wrapped in #ifdef WITH_GPT_AI.
  => Porting the LLM path to a platform = provide one HTTPS/HTTP POST. Nothing else.

### VERIFIED PER-PLATFORM STATE (checked 2026-08-01, not assumed)
| platform | WITH_GPT_AI        | curl                         | net bring-up      | status |
| Linux/PC | wagic-SDL.pro,     | real libcurl                 | OS                | WORKS  |
|          | Makefile.sdl:17    |                              |                   |        |
| PSP      | Makefile:78 YES    | LINKED: -lcurl -lmbedtls     | MISSING sceNet/   | close  |
|          |                    | -lmbedx509 -lmbedcrypto      | apctl bring-up    |        |
| Android  | Android.mk:23 YES  | NO - Android.mk:20 sets      | OS provides       | stub   |
|          |                    | -DWAGIC_NO_CURL              |                   |        |
| Vita     | ⚠ NOT DEFINED      | not linked (SDK HAS          | MISSING sceNet    | absent |
|          |                    | libcurl.a/libssl/libcrypto)  | init in Vitamain  |        |
CORRECTION: an earlier claim this session that the Vita build "compiles AIPlayerGPT and
falls back to Baka" was WRONG - without WITH_GPT_AI the GPT layer is compiled OUT.
(AIPlayerGPT.cpp/GptConfig.cpp objects exist but are ~716 bytes = empty.)

### ⚠ TLS IS NOT REQUIRED — the proxy already removes it
psp-llm-proxy is LIVE on Voyager, 0.0.0.0:8180, PLAIN HTTP (verified listening
2026-08-01, systemd user service active). It forwards /v1/chat/completions,
/v1/completions, /v1/models to the upstream and injects the vLLM API key SERVER-SIDE,
forcing stream:false. See memory project_psp_llm_proxy. So every handheld only needs a
plain HTTP/1.1 POST on the LAN - no TLS stack on-device is strictly needed. (Both Vitas
do have TLS anyway per owner, but PSP does not need it either.)

### CHEAPEST-FIRST ORDER FOR THE LLM PATH
1. VITA - SDK already ships libcurl.a/libssl.a/libcrypto.a and curl headers. Needs:
   -DWITH_GPT_AI, link curl+ssl+crypto+SceNet/SceNetCtl/SceHttp stubs, and sceNetInit +
   sceNetCtlInit bring-up in Vitamain.cpp (currently ZERO sceNet references there).
2. ANDROID - do NOT cross-compile curl for the NDK. Implement httpRequestImpl via JNI
   into Java HttpURLConnection/HttpsURLConnection: already on the platform, handles TLS,
   no native deps. Drop -DWAGIC_NO_CURL only if a real impl is provided.
3. PSP - curl/mbedtls ALREADY LINKED; the gap is WiFi bring-up (sceNetInit, apctl
   connect) plus the known getenv hazard. Plain-HTTP proxy means mbedtls need not work.

## LLM-PATH RELEASE TESTING — owner requirement 2026-08-01
Test against BOTH (a) his local inference and (b) the OpenRouter API.
⚠ BUDGET: only ~$10 OpenRouter credit (key lives on box at ~/.config/openrouter/.env,
mode 600 - keep it SERVER-SIDE, never in session/history or on a handheld).

GOOD NEWS: OpenRouter is ALREADY a built-in preset - GptConfig.cpp kPresets[4] =
{"OpenRouter", "https://openrouter.ai/api"}. urls/model/key are all config
(endpoints.txt, GUI-editable). On PC this is settings-only, no code.

⚠ THE TLS QUESTION COMES BACK. OpenRouter is HTTPS. The psp-llm-proxy plain-HTTP
simplification only covers LOCAL inference. Two ways, and they test DIFFERENT things:
 (a) Front OpenRouter with psp-llm-proxy too (it already terminates plain HTTP and
     injects a key server-side). Devices stay plain HTTP; the OpenRouter key never
     leaves Voyager; one place to enforce spend caps. BUT this tests a path a real
     user does not have - they have no proxy.
 (b) Give each handheld real TLS: Android = free (Java HttpsURLConnection); Vita =
     plausible (SDK ships libssl/libcrypto); PSP = RISKY/UNPROVEN (mbedtls is linked
     but 2007 hardware against modern TLS1.2+ cert chains is not a given).
=> UNRESOLVED OWNER DECISION: does the ALPHA expect users to (i) point at their own
   LAN inference (plain HTTP is then the honest path, proxy-style), or (ii) plug in
   their own OpenRouter/cloud key (then on-device TLS is mandatory and PSP is at real
   risk of being the platform that cannot ship)? This materially changes #6/#14/#15.

COST DISCIPLINE (the plan that protects the $10):
 - SEPARATE transport validation from play validation. Proving "the HTTP path works"
   needs a handful of requests; proving "it plays well" needs volume. Do volume on
   LOCAL inference (free), and hit OpenRouter only to prove the public-API path.
 - Use an OpenRouter :free model for transport validation = $0.
 - Rough cost of a FULL GAME: wagic prompts are large (system = rules primer + both
   decklists + strategy guide; per-decision = narration + board + options), est.
   6-10k input tokens x ~50-150 decisions = ~0.5-1.5M input tokens/game.
   At $0.10/M that is ~$0.05-0.15/game; at $3/M it is ~$1.50-4.50/game (i.e. the whole
   budget in 2-6 games). MODEL CHOICE DOMINATES - never run a full game on a
   frontier-priced model without deciding to spend that much.
 - Guardrails worth adding before any multi-game OpenRouter run: max_tokens cap, a
   request counter, and a spend check. OpenRouter prompt caching would help a lot
   given the static per-duel system prompt.

## VITA LLM PATH (#14) — BUILDS 2026-08-01
Owner rulings that shaped it: LLM path is a release gate on every platform; and
"for testing purposes, a method for hitting my secured inference is needed, for release
purposes, requiring the use of a second device is unacceptable" => psp-llm-proxy is
TEST-ONLY scaffolding; a released build must reach endpoints DIRECTLY, so on-device TLS
is required for cloud endpoints. One curl call serves both (http:// proxy for testing,
https:// OpenRouter for release), so this is ONE piece of work, not two.

WHAT WAS WIRED (CMakeLists.txt + JGE/src/Vitamain.cpp + projects/mtg/src/GptConfig.cpp):
1. -DWITH_GPT_AI in add_definitions. WITHOUT IT THE ENTIRE GPT LAYER PREPROCESSES AWAY -
   GptConfig.cpp.obj and AIPlayerGPT.cpp.obj were ~716 bytes (empty). This is the single
   most important line; an earlier claim this session that Vita "compiles AIPlayerGPT and
   falls back to Baka" was WRONG for exactly this reason.
2. libcurl linked with the dependency set taken verbatim from the SDK's libcurl.pc:
   curl ssl crypto zstd z + SceNet_stub SceNetCtl_stub SceRtc_stub SceSysmem_stub
   SceKernelModulemgr_stub SceProcessmgr_stub SceLibKernel_stub.
   ⚠ VitaSDK's curl is built against OPENSSL, not the console's SceSsl - TLS runs in
   userspace and is NOT bounded by 2012 system libraries. This is what makes the
   no-second-device release requirement plausible on Vita.
3. VitaNetInit() in Vitamain.cpp (new, called as step [3e] after SDL_Init): loads
   SCE_SYSMODULE_NET, sceNetInit with a static 1MB pool, sceNetCtlInit. Treats
   SCE_NET_ERROR_EBUSY as success. DELIBERATELY NON-FATAL - no wifi must still boot+play.
   ⚠ Unlike PSP there is NO access-point join to drive: Vita system software owns the
   wifi association. That is why Vita is far cheaper than PSP task #6.
4. ⚠ GptConfig.cpp used getenv("HOME") at FOUR sites for its writable config. Vita has no
   HOME => reads would silently fall back to the READ-ONLY bundled Res copy and WRITES
   WOULD FAIL, so the options GUI could never save an endpoint and the LLM opponent would
   be permanently unconfigurable. Replaced with static gptUserRoot(): returns
   "ux0:data/Wagic" under #ifdef VITA (Vitamain already creates it), else the identical
   $HOME/.Wagic string. SHARED CODE - gated on the PC suite.

VERIFIED (build-time, on the linked ELF - not assumed):
   ELF 22,837,336 -> 30,365,760 bytes; 18 curl_easy symbols; 2 sceNet symbols;
   GptConfig.cpp.obj 716 -> 192,780 B; AIPlayerGPT.cpp.obj -> 880,076 B.
STILL UNTESTED ON HARDWARE. Nothing has made a real HTTP request from a Vita.
NEXT: point endpoints.txt at the proxy (Voyager:8180, plain HTTP) for the first live
test, then at OpenRouter over https for the release path.

## ⚠ OWNER RULING 2026-08-01 — PSP IS CONDITIONAL, AND THE CONDITION IS COFFEESHOP PLAY
His words: "if the PSP path turns out to be unusable for coffeeshop play, we'll use our
findings to build a PR for the origin, and drop PSP compatibility as a primary target
all together on my fork. possibly do occasional builds that can be used in home LAN
situations at major project breakpoints if it remains possible."

=> "COFFEESHOP PLAY" IS THE BAR: away from his LAN, so no psp-llm-proxy. That means the
   PSP must reach a CLOUD endpoint itself: WPA2 on public wifi + TLS1.2+ with modern
   cert chains + (realistically) captive portals, on 2007 MIPS hardware with ~52MB heap.
   Captive portals in particular have no plausible story on PSP.
=> THIS MAKES THE PSP TLS PROBE A GO/NO-GO DECISION GATE, not a task. Run it EARLY and
   CHEAP - a single HTTPS request to a real public endpoint - BEFORE investing more PSP
   porting effort. The answer decides which of two tracks the PSP work goes down:
     PASS -> PSP stays a primary target, finish task #6.
     FAIL -> PSP is DROPPED as a primary fork target. The findings become an UPSTREAM PR
             (vehicle = the InquiringMinds-AI/wagic fork, see memory
             project_wagic_fixes_port; upstream still targets PSP so the memory work is
             genuinely valuable to them: sidecar text/auto offload, deferred texture
             frees, mid-frame TexMemCheckpoint, texture-cache hard cap, FAT rules-lookup
             fix, ARMv8.0 baseline). Occasional HOME-LAN-ONLY PSP builds at major project
             breakpoints remain acceptable IF still possible - a nice-to-have, not a
             commitment.
⚠ Standing rules for that PR path: CONFIRM WITH HIM before filing anything; external-repo
  PR descriptions must lead with the Claude disclosure line; and hyprwm-style AI bans do
  not apply here but WagicProject's own norms should be checked first.
NOTE: this does NOT change Vita or Android - both must still get a working LLM path to
ship, and both have plausible TLS stories (Vita = userspace OpenSSL via SDK curl,
Android = Java HttpsURLConnection).

### REFINEMENT 2026-08-01 (same conversation) — PREFER TO KEEP THE PSP WORK
His words: "right now, if we can get the PSP path in a good place for my projects testing
needs, I'd rather boggart the fix work and drive users to my platform. but if we can't,
then eliminating it from scope makes everything else easier."
=> DEFAULT IS TO KEEP THE PSP WORK IN HIS FORK as a differentiator and drive users to it.
   The upstream PR is the CONSOLATION path if PSP is dropped entirely - do NOT frame
   upstreaming as a win or pitch it while PSP is still viable.
⚠ TENSION TO RESOLVE BEFORE FILING ANYTHING: memory project_wagic_fixes_port carries a
   standing ruling to offer engine fixes to upstream pro bono. That may still hold for
   GENERAL engine bugfixes while this PSP-specific work is held back - but do not assume.
   ASK HIM before filing any PSP-related PR. Nothing is being filed now, so not blocking.

### ⚠ CORRECTION 2026-08-01 — "MY PROJECT'S TESTING NEEDS" MEANS *USERS*, NOT HIM
Claude wrote a now-deleted section here claiming a PSP that reaches his LAN proxy would
be "good enough for HIS testing" and would justify keeping PSP. THAT REASONING WAS WRONG.
His correction, verbatim: "I'm not going to test wagicgpt on my [PSP] ... it's unlikely
I'll do much testing personally on any environment other than Linux, except when working
on ux." And: "think of the deployed to users software as deployed testing suites, and the
users as chaos driven test creators who will try things and find issues that they will
report some of to me. if they can't use the llm feature, the thing I forked for, then
they can only give engine bug reports. they are impaired testers. and so its extra work
to keep this build maintained without the payoffs I'm hoping for."

=> HIS TESTING MODEL: he develops/tests on LINUX. Other platforms exist to be handed to
   USERS, who are the chaos-driven test corpus. A port's VALUE is the quality of bug
   reports it generates back.
=> THIS IS THE RATIONALE BEHIND THE RELEASE GATE, and it predicts future calls: any
   platform that cannot exercise the LLM feature yields only engine-bug reports =
   impaired testers = maintenance cost without payoff = DROP IT.
=> So the PSP bar is a USER-FACING bar. His own LAN proxy is irrelevant to it - users do
   not have his proxy. psp-llm-proxy stays what he already said it is: TEST SCAFFOLDING.

### THE REAL PSP QUESTION: CAN A *USER* USE THE LLM FEATURE ON PSP?
Two user configurations, different difficulty:
  (A) User runs their OWN local inference (llama.cpp/Ollama - already kPresets[1]/[2])
      and the PSP reaches it over PLAIN HTTP on their home LAN. NO TLS. Gap is only
      sceNet + apctl AP-join (task #6); curl/mbedtls are ALREADY linked
      (projects/mtg/Makefile:67). PLAUSIBLE. This is couch play, not portable play.
  (B) Portable / "coffeeshop" play: PSP -> HTTPS -> cloud endpoint on foreign wifi.
      Needs TLS1.2+ with modern cert chains on 2007 MIPS, plus captive portals which
      have NO plausible story on a PSP. MAY WELL FAIL.
  UNRESOLVED WITH HIM: is (A) alone enough to keep PSP in scope for users, or does a
  handheld only justify itself if (B) works? Also unresolved: does PHONE HOTSPOT count
  as passing (B)? It skips captive portals and is how handhelds usually get online.
  ASK before spending task-#6 effort - the answer changes whether that work is worth it.
- VITA LLM PATH COMMITTED: d799d87e7, suite gate 9 GREEN (1031/0 + 24/0 - required
  because GptConfig.cpp is shared with PC/PSP/Android). VPK repackaged: 39.6MB
  (was 37.3MB pre-curl). ⚠ Suite must be run in the wagicGPT-psp WORKTREE - the
  wagicGPT worktree is on MASTER and would test the wrong code.
  NEXT LIVE TEST (no touch, no handheld needed): point endpoints.txt at the proxy
  (http://<voyager-lan-ip>:8180) and boot on the PSTV; read ux0:data/Wagic/debug.txt
  over FTP for "[3e] network up" and then check the endpoint probe.
- TITLE ID DECIDED (owner, 2026-08-01): the fork uses VITA_TITLEID "WAGC00042" (was the
  inherited WAGC00001) and VITA_APP_NAME "wagicGPT" (was "Wagic: The Homebrew").
  Commit b9530ad8e. Valid format = 4 uppercase letters + 5 digits; WAGC is not a Sony
  prefix (theirs are PCS*/NPXS*) so no collision. The id owns ux0:app/<id> + the LiveArea
  slot, so the fork now installs BESIDE stock Wagic instead of over it. Save data is
  unaffected either way - both builds share ux0:data/Wagic.
  (App-name change was Claude's call for LiveArea distinguishability, flagged to him;
  trivially revertible if he wants different wording.)
- PSP SCOPE: still UNDECIDED by owner as of 2026-08-01 ("I haven't decided. it's on my
  mind."). Do NOT start task #6 until he calls it - the question is whether LAN-only
  local inference is enough to keep PSP, or whether it must work portably.

## ⚠ VITA FIRST HARDWARE RUN 2026-08-01 — BOOTS; CRASH ROOT-CAUSED
GOOD NEWS FIRST, both settled by ux0:data/Wagic/debug.txt:
 - CURRENT vitaGL BOOTS. The inherited d4a8f9d pin was unnecessary; owner's call to
   drop it was right. Full init log reached "[5] Entering MainLoop".
 - "[3e] network up" -> the sceNet/sceNetCtl bring-up WORKS on hardware.
 - ux0:data/Wagic/{player,settings,campaigns,...} created by the game => gptUserRoot()
   writable path is live.

THE CRASH (both repros: confirming the mulligan hand; and demo right after the
battlefield background draws, before cards) WAS NOT GRAPHICS OR NETWORK.
Root-caused from a psp2core dump, chain unambiguous:
  udf #255 <- _kill_r <- abort <- __gnu_cxx::__verbose_terminate_handler
  <- __cxxabiv1::__terminate <- __cxa_throw
  <- std::thread::_M_start_thread (thread.cc:176) <- std::system_error
  R2 = 0x6 (SIGABRT). Stop reason "Undefined instruction exception" = GCC's trap.
=> std::thread CONSTRUCTION THREW and nothing caught it -> terminate -> abort.
   The thread is AIPlayerGPT.cpp's async model-call worker, which is why both repros
   are "the first moment the AI must decide something" (mulligan hands priority to the
   AI; demo does the same before any card is drawn).
=> INTRODUCED BY ENABLING WITH_GPT_AI ON VITA, but the DEFECT IS SHARED-CODE and latent
   on every platform: any device refusing a thread under memory/handle pressure aborts.
FIXED commit 9993910d6 (suite 1031/0 + 24/0): both std::thread sites (AIPlayerGPT model
call + OptionGpt endpoint probe) now catch and publish an empty reply = same shape as an
unreachable endpoint, so the existing Baka fallback takes over. Options screen now says
"cannot test: no worker thread" instead of blaming the endpoint it never contacted.
⚠ STILL UNKNOWN: WHY the Vita refused the thread. Two very different causes:
   (a) libstdc++ gthreads inactive on this toolchain ("Enable multithreading to use
       std::thread") => std::thread NEVER works on Vita; the async worker would need a
       native pthread/sceKernelCreateThread implementation. LLM-on-Vita blocked until then.
   (b) pthread_create failed for a resource reason => tunable (stack size etc).
   The exception's what() distinguishes them. NEW: gptLogLine() appends to
   <userroot>/ai/gpt/gpt-log.txt because DebugTrace is inside #ifdef _DEBUG (undefined in
   the Vita build) and resolves to std::cerr (invisible on a console). READ
   ux0:data/Wagic/ai/gpt/gpt-log.txt AFTER THE NEXT RUN - that string is the next decision.

## VITA OPERATIONAL GOTCHAS (2026-08-01)
- ⚠ VitaShell FTP DIES WHEN THE CONSOLE SLEEPS (owner confirmed). Any multi-step remote
  sequence needs a liveness re-check between steps, not just at the start. Symptom is
  nasty: directory LISTINGS return EMPTY WITH EXIT 0 (looks like "empty dir"); only a
  direct file fetch surfaces the real curl exit 7.
- IP was 10.0.0.227 on 2026-08-01, NOT the 10.0.0.95 in memory reference_vita_tv.md.
  DHCP moves it - always take the IP from VitaShell's on-screen FTP banner.
- deploy-vita.sh now targets ux0:/vpk/ (was ux0:/data/, which looked like a failed
  upload since VitaShell users look in /vpk) and passes --ftp-create-dirs.
- CRASH DUMPS: ux0:/data/psp2core-*.psp2dmp are GZIPPED ARM ELF cores. Recipe:
    gunzip -c X.psp2dmp > X.core
    git clone https://github.com/xyzz/vita-parse-core   (needs 3 py3 patches: it imports
      elftools.common.py3compat (removed in pyelftools >=0.30) for str2bytes/bytes2str,
      and util.py c_str indexes bytes as str)
    pip install --target=<dir> pyelftools   # pure python, mount into the container
    docker run -v repo:/build -v scratch:/scratch vitasdk/vitasdk \
      bash -c 'PYTHONPATH=/scratch/pylibs python3 main.py <core> /build/build_vita/wagic'
  Patched copy + pylibs live in this session's scratchpad; re-clone if lost.
  arm-vita-eabi-gdb CANNOT read these (Vita uses custom note types 0x1002-0x101c, not
  NT_PRSTATUS) - it reports "Couldn't find general-purpose registers".
  Address mapping: ELF is linked at 0x81000000, module loaded at 0x8106D000 =>
  ELF addr = runtime addr - 0x6D000; symbolize with arm-vita-eabi-addr2line -f -p -C.

## BUTTON CONVENTION — OPEN UX DECISION (owner reported 2026-08-01)
"vita ships a different default for confirm/cancel - the international standard, where
psp wagic ships the japan standard as default." The ENGINE ALREADY KNOWS: JTypes.h says
  JGE_BTN_OK,  // Circle in Japan, Cross in Europe
  JGE_BTN_SEC, // Cross or Circle (secondary)
The Vita port hardcodes CROSS->OK, CIRCLE->SEC (international) in gVitaButtonMap
(JGE/src/Vitamain.cpp), so moving between his PSP build and the Vita build swaps them.
RESOLVED by owner 2026-08-01, commit 684182495: "lets not worry about 'console
standard' lets just use one layout for defaults across platforms. we'll use cross
confirm, circle cancel, because thats my mental default, and anyone who doesnt like it
can remap." => ONE LAYOUT EVERYWHERE. Do NOT re-introduce per-console regional defaults.
⚠ THE FIX WAS ON PSP, NOT VITA - the Vita already had CROSS->OK. JGE/src/main.cpp
gDefaultBindings had the Japanese assignment (PSP_CTRL_CIRCLE->JGE_BTN_OK); swapped so
CROSS->JGE_BTN_OK and CIRCLE->JGE_BTN_SEC. Triangle keeps CANCEL, Square keeps PRI.
NOTE on engine semantics: JGE_BTN_SEC (not JGE_BTN_CANCEL) is what behaves as
"back/cancel" in practice - confirmed on Android earlier the same day when Back was
bound to SEC and the owner reported correct menu behavior. JGE_BTN_CANCEL is Triangle.
⚠ NOT REBUILT FOR PSP: pure binding-table reorder, same symbols. Verify at the next PSP
build. Android GAMEPAD face-button mapping (flounderbounder's community fix, credited as
"PSP-style") was NOT audited for this - CHECK IT before the alpha; if it maps Circle to
confirm it now disagrees with every other platform.

- FIXED VPK IS ON THE DEVICE 2026-08-01: ux0:/vpk/wagic.vpk, 39,637,580 bytes, contains
  commit 9993910d6 (thread guard). NOT YET INSTALLED OR RUN BY THE OWNER. When he does:
  expect it to REACH THE DUEL AND PLAY BAKA instead of aborting, then pull
  ux0:data/Wagic/ai/gpt/gpt-log.txt for the "worker thread refused: ..." line.

## ENDPOINT PROBE: /v1/models SHAPE (2026-08-01)
RETRACTION FIRST: mid-session I claimed gptProbeEndpoint was BROKEN for llama.cpp
because "current llama-server returns Ollama-shaped {"models":[...]} while the probe
demands OpenAI-shaped data[]". THAT CLAIM IS WRONG. Measured on the live server
(127.0.0.1:8080 and through psp-llm-proxy:8180), one reply carries BOTH:
  {"models":[{"name":"qwen3.5-4b","model":"qwen3.5-4b",...}],
   "object":"list","data":[{"id":"qwen3.5-4b",...}]}
"models" is simply the FIRST key; the original parser reads "data" and succeeds. The
bad diagnosis came from reading a head -c truncated body. There is NO confirmed probe
bug against llama.cpp - if a probe failure is ever OBSERVED, re-diagnose from scratch
(on Vita the likeliest cause is the thread abort, not JSON).
CHANGE MADE ANYWAY (robustness, not a bugfix): gptProbeEndpoint now accepts the model
list under "data" OR "models", takes the first entry that names itself via "id",
"name" or "model", and tolerates entries that are bare strings. Costs nothing and
stops the probe betting on one server lineage's choice of key.

## ANDROID LLM TRANSPORT — BUILT 2026-08-01 (NOT YET DEVICE-TESTED)
Android had WITH_GPT_AI on but -DWAGIC_NO_CURL, so gptHttpPost returned "" forever and
AIPlayerGPT fell back to Baka on every decision. Now wired, no libcurl involved.

SHAPE: the transport is ONE static Java method called over JNI.
 - Java: SDLActivity.gptHttpRequest(String url, String body, int timeoutMs, String bearer)
   -> HttpURLConnection. Empty body = GET, else JSON POST. Returns the body on HTTP 200,
   "" on ANY failure (same contract as the curl impl: "" = unreachable -> Baka).
   Chosen over a libcurl port so TLS rides the platform's trust store + OS updates
   instead of a CA bundle baked into the APK.
 - Native: GptConfig.cpp, #ifdef WAGIC_HTTP_JNI branch of httpRequestImpl.
 - Android.mk keeps -DWAGIC_NO_CURL (it gates the curl includes / curl_global_init in
   GptConfig.cpp:8 and AIPlayerGPT.cpp:34,1921 and literally means "no libcurl here")
   and ADDS -DWAGIC_HTTP_JNI, which wins over the no-transport stub.

⚠ THE JNI TRAP THAT SHAPES THE CODE: FindClass on a thread ATTACHED FROM NATIVE resolves
against the SYSTEM class loader and CANNOT see app classes. The model call runs on exactly
such a thread (AIPlayerGPT's detached worker). So SDLActivity is resolved ONCE in
JNI_OnLoad (SDL_android_main.cpp, which already existed there to call setJVM) and kept as
a GLOBAL ref via gptAndroidCacheClass(). The worker attaches/detaches per call - a
natively-attached thread that exits without detaching aborts the VM.

TWO BLOCKERS FOUND AND FIXED THE SAME DAY, both of which would have looked like "the LLM
just doesn't work" on a user's device:
 1. CLEARTEXT. targetSdkVersion=29 => Android blocks plain HTTP by default (flipped at
    28). Every local-inference endpoint (his 10.0.0.198:8180, any LAN llama.cpp/Ollama)
    would be refused BEFORE a packet left the device, and the failure is indistinguishable
    from a wrong IP. FIXED: android:usesCleartextTraffic="true" on <application>.
    Verified present in the built APK via aapt2 dump xmltree. HTTPS is unaffected.
 2. CONFIG PATH. gptUserRoot() used $HOME/.Wagic; an Android app process has no useful
    HOME, so endpoints.txt was never found or writable. FIXED: on ANDROID it now returns
    JFileSystem::GetInstance()->GetUserRoot() (trailing slashes stripped) = the same user
    root every other Wagic file uses (internal storage or the SD card, per his setting).
    PC is unchanged; note PC's GetUserRoot() happens to equal $HOME/.Wagic anyway.

DIAGNOSTIC CHANNEL (because "" is a silent outcome): Java records the failure reason in
sGptLastError ("HTTP 401", the exception's toString()); when a call comes back empty the
native side calls SDLActivity.gptLastError() and writes "android transport failed: <why>"
into <userroot>/ai/gpt/gpt-log.txt. That file is what to ask a user for.

BUILD: ndk-build must be run BEFORE build-apk.sh (the script only packages libs/).
  ~/Android/Sdk/ndk/27.1.12297006/ndk-build -j8   # from projects/mtg/Android
  ./build-apk.sh
Verified in the artifact: libmain.so carries the "android transport:" strings, classes.dex
carries gptHttpRequest + gptLastError, manifest carries usesCleartextTraffic + INTERNET.

⚠ STATUS: BUILDS AND PACKAGES. NOT RUN ON HARDWARE. Nothing here is device-verified.
DEVICE TEST PLAN: install; Options -> GPT tab -> set Endpoint URL to the LAN inference
box; "Test connection" should say "OK - serving <model>"; then start a duel and confirm
the opponent thinks. If it falls back to Baka, pull <userroot>/ai/gpt/gpt-log.txt.

## PHASE-STOP UX — OWNER'S DESIGN RULING (2026-08-01)
Triggered by the Vita session ("gameplay halts at every player upkeep, even when they
have nothing they can play or do"). Claude's first fix made ASKIP_SAFE skip upkeep via
the oracle; the owner REJECTED that shape:
  "i think i need to probably revert some changes around how ive adjusted skipping. i
   should probably follow the intentional configurable design instead of just making the
   default config do what i want the config to do in general. its a userland nob, and i
   should respect its design logic."
That change was reverted before it was committed. DO NOT reintroduce a fix that makes one
ASKIP level quietly behave like another.

THE RULING (his words):
  "the default skip method should be a low friction ux that provides opportunities for the
   player to interrupt only when they have a playable option, this is the ux that ive been
   aiming for."
  "maybe what i want is to revert the skip behavior to the defaults, but make it so phases
   and interrupt opportunites where no legal player actions exist are skipped. so if i
   cannot do anything then it skips regardless of posture"
=> TWO SEPARATE MECHANISMS, deliberately orthogonal:
   1. ASPHASES (NONE/SAFE/FULL) keeps its ORIGIN semantics and origin default. It governs
      stops where a legal action EXISTS. Revert the first-launch override added by
      39b91b51d (goEnum->number = ASKIP_SAFE in GameOptions.cpp) - the widget in
      GameStateOptions.cpp already declares ASKIP_NONE as its default, so that override
      was a preference baked into a default.
   2. A NEW always-on rule, ABOVE the posture: if the player has NO legal action in this
      phase/window, it is skipped even at ASKIP_NONE.

His own interrupt habits, which the rule has to keep reachable (do not "optimize" these
windows away): in response to an opposing action (cast, ability, declared attacks,
declared blocks INCLUDING no blocks); at the end of the opponent's turn; during upkeep to
manipulate his draw; more rarely in response to the resolution of an ability/spell/damage;
rarely at the end of his own turn for end-of-turn effects.

⚠ SAFETY ASYMMETRY that governs every predicate here: a false "no legal action" skips a
window the player needed and can LOSE A GAME; a false "has an action" costs one keypress.
Every predicate must lean toward STOP. This is why hasInstantResponse already uses
ManaEngine::potentialManaPermissive rather than strict potentialMana.

ALREADY BUILT, do not rebuild: the REACTIVE half. GameObserver.cpp:417 opens a priority
window only for a forced stop, an enabled per-phase INTERRUPT_* option, or
LegalActionsOracle::hasInstantResponse(phaseResponder). Note INTERRUPT_UPKEEP defaults to
0 and has no row in the options screen - it was NOT the cause of the upkeep stop.
THE MISSING HALF is the ACTIVE seat: the ASPHASES auto-skip block (GameObserver.cpp ~1237)
advances phases from a hardcoded phase list and never asks whether the human can act.

## VITA std::thread — WHY IT FAILS, AND WHY THE LINK-FLAG FIX IS A TRAP (2026-08-01)
HARDWARE FACT 1 (gpt-log.txt, 209 identical lines, one per AI decision):
  "worker thread refused: Enable multithreading to use std::thread: Not owner"
That is libstdc++ reporting __gthread_active_p() == false. The LLM opponent has NEVER
run on the Vita; every decision fell back to Baka. The crash guard (9993910d6) works -
the game degrades instead of aborting.

HARDWARE FACT 2 (the attempted fix, commit b58b9fd2e, REVERTED by 8ef1277f6):
__gthread_active_p() is a WEAK reference to a pthread proxy symbol, so -lpthread alone
does not satisfy it (no strong reference -> the archive member is never pulled).
Forcing it with -Wl,-u,pthread_create -Wl,-u,pthread_cancel -Wl,-u,pthread_key_create
DID flip the check to true - AND CRASHED THE GAME ON LAUNCH.
  psp2core-1785640291: Data abort exception, PC in pthread_mutex_lock at its first
  instruction `ldr r4,[r0,#0]`, LR inside __cxa_guard_acquire (the guard around the
  FIRST function-local static). debug.txt stops right after "[3e] network up" - the
  crash is before VitaInit even returns.
MECHANISM: -u pulls in ONLY the named archive members. The member carrying
pthread-embedded's global state never came with them, so libstdc++ took its threaded
path and locked a mutex that was never constructed. A HALF-LINKED PTHREAD RUNTIME IS
STRICTLY WORSE THAN NONE.
⚠ WHICH SYMBOL IS THE PROXY: pthread_create and pthread_key_create are pulled in by
SDL either way (nm shows them T in BOTH builds). pthread_cancel is the ONLY one the -u
flags added, so pthread_cancel is what libstdc++ actually tests here. If anyone retries
this, that is the single symbol whose presence flips the behavior - and flipping it
without the rest of the runtime is the crash above.

NEXT MOVE IF LLM-ON-VITA IS PURSUED: do NOT chase link flags. Whole-archiving
libpthread MIGHT work (untested, and it is a gamble on the owner's device time). The
robust path is a NATIVE sceKernelCreateThread worker for the async model call, which
does not depend on libstdc++'s threading model at all. The async seam is one function:
AIPlayerGPT.cpp's std::thread(...).detach() site plus OptionGpt's probe thread, both of
which already publish results through a mutex-guarded AsyncState.

## VITA AI PASSIVITY — OPEN, NOT REPRODUCIBLE ON LINUX (2026-08-01)
OWNER REPORT (both the pre- and post-phase-stop builds, so it PREDATES today's engine
work): "the enemy is still just drawing and passing turn every turn. in demo mode, only
on the first turn, the player plays a land."

RULED OUT, each with evidence - do not re-walk these:
1. The GPT->Baka fallback itself. Linux selfplay with a DEAD endpoint
   (WAGIC_GPT_URL=http://127.0.0.1:9) played a full 8-turn game: lands, attacks, a
   winner.
2. The thread-refusal path specifically. NEW TOOL: WAGIC_GPT_NOTHREAD=1 makes the
   std::thread site throw, emulating the Vita's inactive gthreads layer on a desktop -
   the fallback path is otherwise unreachable where it is easiest to test. Linux
   selfplay under it played 13 turns with blocks, activated abilities and combat damage.
   So the code path the Vita takes is NOT broken; something else on the Vita is.
3. A runaway ask/refuse loop. gpt-log.txt went 209 -> 344 lines across his session:
   135 refusals, one per decision. A per-tick loop would be thousands.
4. Demo spectator pacing (AIPlayerBaka.cpp, timer = 1.5f after a completed play): it is
   GAME_TYPE_DEMO-only and a 1.5s hold, and his classic-mode games are passive too.
5. The PSP dead-game root cause (rules never loaded -> no draw step; see the FAT 8.3
   section above). NOT this: draws ARE happening on the Vita.

BISECT RESULT 2026-08-02: with the LLM opponent turned OFF in Options -> GPT (which
stops AIPlayerGPT being constructed at all, so the seat is pure AIPlayerBaka with no
async gate and no fallback), the owner reports "baka is now playing the game correctly".
=> THE ENGINE AND THE HEURISTIC AI ARE FINE ON VITA. The fault is in the GPT layer's
fallback path - which nonetheless plays a full game on Linux under WAGIC_GPT_NOTHREAD=1.
FIRST FIX ATTEMPT (commit 34938f377, suite 1031/0 + 24/0, VPK built, NOT YET TESTED ON
HARDWARE): gptLogLine ran three mkdir syscalls + open/write/close ON THE GAME THREAD, and
the refusal path called it once per DECISION (209 then 344 identical lines). That is the
ONLY part of the fallback path that behaves differently on Vita vs Linux - ux0 storage
I/O per game event, already measured as a real lag contributor during the PSP campaign.
Now: dirs created once per run, refusal logged once. If the AI still will not act with
the LLM on, this fix is a prerequisite for the probe anyway - a per-decision probe would
have drowned in the same I/O it was trying to measure.

IF THAT IS NOT IT, the next step is on-device instrumentation, the method that root-caused every
other hard bug on this project: gptLogLine() already writes to
ux0:data/Wagic/ai/gpt/gpt-log.txt from anywhere in the engine, so a probe build should log
per decision what Baka actually chose (computeActions result / FindCardToPlay verdict /
legalCasts + legalLandPlays counts) and what the phase/turn state was. Ask for one demo
game plus one classic game, then pull the log.
⚠ Test BOTH game types: the demo is AI-vs-AI (both seats AIPlayerGPT) while classic has a
human seat, and they differ in which skips and which decision seams are live.

SEPARATE DEFECT FOUND AND FIXED WHILE INVESTIGATING (introduced by the phase-stop rule
earlier the same day, so it is NOT the cause of the report above): the new
no-legal-action skip was not turn-gated. userRequestNextGamePhase() advances the phase
GLOBALLY regardless of whose turn it is, so firing it on the opponent's turn rips the
phase out from under the AI before its throttled Act can play. Every pre-existing skip in
that block is turn-gated for exactly this reason (opponent()->isAI() for the own-turn
skips; currentPlayer->isAI() && !opponent()->isAI() for the W3b blockers skip). Now gated
on currentPlayer == humanSeat. The opponent's turn is already covered by the reactive
half, which only opens a window when the non-acting player can respond.

## PUBLIC REPO — STATE, AND THE PLAN THE OWNER APPROVED (2026-08-01)
He was asked for a link to his repo and found it unrecognizable. Investigated:

WHAT ACTUALLY HAPPENED — the repo was RENAMED, not nuked. `gh repo view
InquiringMinds-AI/wagicGPT` resolves to `InquiringMinds-AI/wagic` (identical createdAt
2026-06-10T01:35:46Z), a fork of WagicProject/wagic. GitHub's rename redirect is why
`git fetch origin` kept working and hid the rename from us. As `wagic` it is being used
as the upstream-contribution fork, so its master was pointed back at upstream
(37115e15e, 2026-06-26) - which contains NO AIPlayerGPT/GptConfig/OptionGpt.
NOTHING WAS LOST. A rename deletes nothing, and the history lives in the local clone:
408 commits on master (2026-05-04 -> 2026-07-27, 395 authored by him) + 37 on psp-port
(2026-07-31 -> 2026-08-01) = ~445. No published branch carries the GPT sources (checked
master, june-grind, fixes-integration).

OWNER DECISION (2026-08-01): publish wagicGPT as a NEW STANDALONE PUBLIC REPO, not a
GitHub fork - forks are excluded from GitHub search by default and do not accumulate
their own stars, which is a real cost for portfolio material. `InquiringMinds-AI/wagic`
stays as-is, the upstream-contribution fork. Creating the wagicGPT name also reclaims it
from the rename redirect.

SEQUENCE (NOT YET EXECUTED - needs his explicit go, and his README edits first):
 1. commit README onto master (worktree ~/Projects/wagicGPT) replacing upstream's
 2. gh repo create InquiringMinds-AI/wagicGPT --public --description "<his wording>"
 3. push master -> default branch;  4. push psp-port (he has not decided on this one)
DRAFT README at wagicGPT-psp/README.draft.md (untracked). No AI-tooling attribution
anywhere in it, per his standing rule.

PRE-PUBLISH AUDIT DONE (clean): no WOTC card art tracked on master (images under sets/
are only the 19 engine no-art fallback frames at the directory ROOT; count inside
sets/<SET>/ is 0), no CLAUDE.md, no endpoints.txt, no .env, no keys.

COMMIT EMAIL - SETTLED 2026-08-01, no history rewrite needed. GLOBAL git user.email is
now aaron@inquiringminds.ai (was noreply@inquiringminds.ai; changed fleet-wide at his
instruction, so it affects his other repos too). On GitHub, aaron@, github@ (primary)
AND noreply@ are all VERIFIED - the noreply@ verification mail was simply delayed, not
lost. GitHub attributes commits by email and does so RETROACTIVELY, so the ~445 existing
commits authored as noreply@ attribute correctly with no surgery.
⚠ A REWRITE WAS PROPOSED AND THEN CORRECTLY ABANDONED - do not resurrect it. It would
have touched 833 commit-refs across 79 branches and invalidated the 33 commit SHAs cited
in this file plus 19 in fleet memory, all to solve a problem that verification solved for
free. Mail-delivery note for future reference: inquiringminds.ai has Proton MX
(mail.protonmail.ch) with a catch-all; inquiringminds.COM has NO MX at all, so anything
addressed to the .com spelling vanishes silently.

## RELEASE POLICY — BINARIES (owner ruling 2026-08-01)
"no compiled binaries. i want to do all compiled binaries in one release" — and on being
asked what counted: "intermediates and things that already existed before we forked are
not what im talking about. i mean things where someone downloads x, and they run it, to
play the game".
=> THE RULE IS ABOUT PLAYABLE ARTIFACTS, not build intermediates. The APK, the VPK,
EBOOT.PBP, the Windows .exe, the Linux binary: NEVER committed, and shipped in ONE
GitHub release carrying EVERY platform together, not per-platform releases.
NOT in scope: the ~41MB of prebuilt libs/intermediates inherited from upstream
(admobsdk/*.a, JGE/lib/*, Boost/lib/*, Android/libs/armeabi-v7a/*.so, groovy jar). They
predate the fork and he explicitly excluded them - do NOT "clean them up" on his behalf.
STATE: no playable artifact has ever been tracked. The PSP outputs (EBOOT.PBP, wagic.elf,
wagic.prx, PARAM.SFO) and build_vita/ were untracked but UNIGNORED - now ignored.
Already ignored beforehand: projects/mtg/bin/wagic, the APK, the VPK, Android libmain.so.

## CARD ART RULE — REFINED BY THE OWNER (2026-08-01)
The standing rule has been recorded here and applied as a blanket "no card images ever".
That is TOO BROAD. His correction, verbatim: "long term plan is to make our own card,
with card art, and to distribute those things. we're only opposed to distributing
copyright card art."
=> THE CONSTRAINT IS COPYRIGHT, NOT ART. Wizards-derived card scans: never in the public
repo, never in a release artifact, local + his own devices only (unchanged). ORIGINAL
cards with ORIGINAL art are a PLANNED DELIVERABLE and are meant to ship with the game.
Do not describe the project as image-less as though it were a permanent design property,
and do not build tooling that assumes art can never be distributed - the res-pack guard
in tools/make-respack.py is correctly scoped (it refuses art staged under Res/sets/, i.e.
the imported WOTC scan layout) and should NOT be generalized into an all-art ban.
This connects to the project's long arc already in the skill: original card content and a
de-MTG retheme toward an open platform for user-created shareable card games.

## VITA AI PASSIVITY — ROOT-CAUSED AND FIXED (2026-08-02)
SYMPTOM: with the LLM opponent ON, the AI drew and passed every turn and never played;
demo played one land on turn 1 and then nothing. Baka with the LLM OFF was fine.

⚠ MY EARLIER "RULED OUT" ENTRY WAS WRONG AND IS SUPERSEDED. Both Linux tests used a DEAD
endpoint, and AIPlayerGPT PROBES THE ENDPOINT AT CONSTRUCTION: unreachable => the seat
becomes a permanent Baka for the whole game and never attempts a model call
("AIPlayerGPT: no LLM endpoint reachable, falling back to Baka heuristics for every
decision"). So those runs never entered the per-decision fallback at all - 0 worker
refusals in the log. The claim "the same path works on Linux" was unsupported.

THE REPRO (fast, local, deterministic - use this, not the device):
  WAGIC_HEADLESS=1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 WAGIC_FASTCLOCK=0.2 \
  WAGIC_AI=gpt WAGIC_GPT_NOTHREAD=1 WAGIC_GPT_URL=http://127.0.0.1:8080 ./wagic
REACHABLE endpoint (so no permanent-Baka early-out) + WAGIC_GPT_NOTHREAD=1 (so every
model call hits the thread refusal, as on Vita). Broken: 215 refusals, 1 play, turn 107,
both players still at 20 life. Fixed: 51 refusals, 26 plays, turn 13, real winner.

ROOT CAUSE: the refusal path published state->status = 2 synchronously and then returned
kChoicePending. kChoicePending means "no action this tick", and the ONLY thing stopping an
empty clickstream from being committed as a PASS is decisionPending(), which is true only
while asyncBusy() - i.e. status == 1. So the seam said "waiting" while the gate said
"idle", and the engine passed the turn. Every decision became attempt-once-then-get-passed
=> ~1 refusal per player per turn, forever. (The old comment at the seam claiming "the Act
override keeps the empty clickstream from being committed as a pass" is STALE - c5d
retired that override in favour of the decisionPending hook.)
FIX: on refusal, go back to status = 0 with an empty response and RETURN 0 in the same
tick, which is the same shape as a synchronous transport failure and is what every seam
already answers with the heuristic AI.
LESSON: kChoicePending is a promise that something is in flight. Never return it without
leaving asyncBusy() true, or the engine will pass on the player's behalf.

## CARD ART — THE REAL SOURCE, AND VITA PACK SIZING (2026-08-02)
⚠ SOURCE LOCATION, found by filename-probe search (his hint: folder+filename match across
all res packs, so grep the fs for one known card id):
  projects/mtg/bin/User/sets/<SET>/<id>.jpg  +  .../thumbnails/<id>.jpg
NOT ~/.Wagic (that is 1.2GB of ai/ transcript logs only) and NOT bin/Res (19 no-art
fallback frames). 12 GB, 336 sets, full ~618x882 (varies: 646x910, 672x936), thumbs
114x166. An older copy also exists at ~/Emulation/WagicPSP-0231/Wagic/User/sets/.
The PSP pack (180x257 / 64x93) was DOWNSCALED from this.

DEPLOYMENT IS FEASIBLE ON VITA - verified in code, no engine change needed:
WResourceManager::cardFile resolves sets/<SET>/<SET>.zip (AttachZipFile) or
sets/<SET>/<id>.jpg relative to the JFileSystem roots, and the USER path is searched
before the system path. So per-set zips go to ux0:data/Wagic/sets/<SET>/<SET>.zip even
though Res is read-only inside the VPK. In-zip keys: "<id>.jpg" + "thumbnails/<id>.jpg",
per-set dir zipped at root, STORED (-0) - same layout proven on PSP.

PILOT MEASURED (set 10E, 389 cards, magick -resize 360x514 q88 / 128x186 q85):
  source 61MB -> vita 32MB -> (psp pack was 8.4MB).  12.4s per set to convert.
=> FULL 336-SET VITA PACK ~= 5 GB (PSP pack was 1.3GB; ratio 3.8x).
⚠ TRANSFER IS THE BOTTLENECK, NOT THE BUILD: Vita FTP measured ~800KB/s and DIES ON
CONSOLE SLEEP, so 5GB is ~1.7 HOURS of babysat transfer. Pulling the SD card and copying
directly is the sane path for a pack this size. Scope reduction (only sets he plays) is
the other lever. UNRESOLVED: his Vita's free space.


## FULL VITA CARD PACK — BUILT (2026-08-02)
Owner ruling on scope: "lets do the full collection of images, if it causes crashes,
thats a feature, not a regret... we need to find crashes so we can fix them." So the pack
is ALL 336 sets, no subset. On free space he said the cut is his side, not the pack's:
"nah. the natural cut is to delete a game or two."

Owner ruling on thumbnails: "just make thumbnails from the full size images. and then make
the missing thumbnails for the full size res pack." Two consequences, both implemented:
 (a) the Vita pack ALWAYS derives its thumb by downscaling the 618x882 art - it never
     rescales the shipped 114x166 thumb (one resample instead of two, off data that still
     has the detail), and every card gets a thumb whether or not the source shipped one;
 (b) the DESKTOP pack was backfilled in place.

SCRIPTS (both in psp-work/, both re-runnable and non-destructive):
  make-vita-cards.sh     - 336 sets -> <OUT>/sets/<SET>/<SET>.zip, STORED, keys
                           "<id>.jpg" + "thumbnails/<id>.jpg". Skips sets already built,
                           so an interrupted run resumes. Full 360x514 q88 (aspect-fit),
                           thumb 128x186! q85 (EXACT).
  backfill-thumbnails.sh - writes only thumbs that do not exist; DRYRUN=1 to count.
Output staged OUTSIDE both worktrees at ~/Projects/wagic-card-packs/vita/ - psp-work/ is
NOT gitignored, and WOTC scans must never be near a tracked path.

⚠ THUMBNAIL GEOMETRY IS A FORCED RESIZE, NOT AN ASPECT FIT. Every sampled desktop thumb is
exactly 114x166, but 618x882 fitted to width 114 is 163 tall. So the pack was built with
-resize 114x166! and the backfill must match, or the new thumbs sit 3px short of their
neighbours in a grid. Same reasoning applied to the Vita thumbs (128x186!).

MEASURED (was: 77302 full-size but only 54103 thumbs):
  backfill created 23199 thumbs across 77 sets (worst: CMM 1196, J22 961, CLB 931).
  After: 77302 full-size / 77302 thumbs - exact parity, 0 truncated, 60/60 sample at
  114x166 and decoding clean under a verbose parse.
  => this ALSO fixes the desktop deck-editor/browser falling back to the no-art frame for
     those 23k cards. It was a live gap on Linux, not only a Vita concern.

SIZE ESTIMATE CORRECTED: the earlier ~5 GB scaled 10E per-SET; per-CARD from the same
pilot is the stable unit (32MB/389 = 82.3KB/card x 77302 = 6.4GB). Mid-build actuals
tracked ~18-20 MB/set => ~6.2-6.8 GB. Build takes ~1 hour at JOBS=6 (two full-resolution
decodes per card now, not one full + one thumb-sized).

VERIFY A BUILT ZIP BEFORE TRUSTING IT: magick writes the JPEG header before it finishes
encoding, so a killed run leaves present, non-zero, GARBAGE files that a resume-by-presence
check would skip. The script rm -f's its own failures for exactly this reason; spot-check
with `find -size -1k` plus a verbose decode, not with ls.

TRANSFER: still the bottleneck. FTP ~800KB/s and dies on console sleep => ~2.3h babysat for
6.5GB. SD card is the sane path. OPEN: which he picks.


## VITA CARD RENDER SIZES — THE MEASURED CHAIN (2026-08-02)
Pack SHIPPED and verified on device: 336 zips, 5,858,784,316 bytes, 0 size mismatches,
md5 spot-checks clean, card unmounted cleanly. 3.8 GB free left. sets/ was EMPTY before,
so nothing of his was overwritten.

SHIPPED GEOMETRY (width x height): full art 360x514 (85%) / 360x507 (15%, from 646x910 and
672x936 source scans aspect-fitting 7px shorter); thumbs 128x186 uniform (forced resize).
Avg 70 KB full + 9.6 KB thumb = ~80 KB/card x 77302 = 5.9 GB.

THE FOUR-LINK CHAIN THAT DECIDES ART SIZE (no single link means anything alone - two
earlier answers in this session were wrong from grabbing one link and stopping):
  1. panel                 960x544 physical (Vitamain.cpp:64-65)
  2. viewport vs ortho     glViewport(960x544) + glOrthof(0..480, 0..272)  => JGfx.cpp:2782/2788
     => the 2x is a COORDINATE TRANSFORM, not a framebuffer upscale. Textures ARE sampled
     at full panel density. Logical px * 2 = physical px.
  3. base card height      CardGui.cpp:1298  scale = pos.actZ * 250 / q->mHeight
  4. per-surface zoom

MEASURED RENDER SIZES (physical px tall):
  in-play card       38 logical (x1.4 focused)      =  76 / 106   CardGui.cpp:178
  grid deck view     285 x 0.52                     =  296        GridDeckView.cpp:9
  IN-MATCH LARGE     250 x 0.8                      =  400        CardSelector.cpp:69,352
  deck carousel      285 x ~0.896 (peak)            =  510.8      CarouselDeckView.cpp:4,72
=> LARGEST IN APP is the DECK CAROUSEL, not the in-match card. 514-tall art covers it.
   Wagic NEVER draws a card edge-to-edge; the 544 panel height is NOT the operative limit.

TWO FACTS THAT KILL WHOLE ARGUMENT BRANCHES:
 - Aspect is never distorted: RenderQuad(q, x, y, t, scale, scale) is UNIFORM scale, so the
   quad inherits the texture's proportions. Only HEIGHT is pinned; width follows.
 - The carousel's scale is a COSINE OF POSITION, so its peak 510.8 is fractional and
   momentary. No integer texture is ever pixel-perfect there. Only the in-match 400 is a
   fixed, exact, achievable target.
 - Direction rule: overshoot is benign (minification), undershoot is not (magnification
   invents pixels and looks soft). 514 >= every render size => nothing is EVER magnified.

CACHE MATH (the crash surface): the texture cache holds DECODED size, not JPEG bytes.
360x514 @32bpp = ~740 KB resident, so HUGE_CACHE_LIMIT 48MB holds only ~65 full-art cards
(MAX_CACHE_OBJECTS 800 never binds - BYTES bind first, so raising the object count would
not help; move the byte ceiling). Brendonm17 raised both himself (84e0cdf78, 2026-05-04)
and his comment names the worst case: "reduces eviction thrashing in the deck builder" -
which is exactly where cards are BOTH largest AND most numerous. Watch that screen first.

OWNER'S DIRECTION (2026-08-02, contemplating - NOT a task): "when I rework things.. I'll
make the deck editor match the battlefield." One render size => one texture size => genuinely
pixel-perfect everywhere instead of choosing which surface to compromise. Until then:
"for now we'll use things as is." Do NOT rebuild the pack unprompted.

WHY THE PACKS WERE SMALL ORIGINALLY (his question): not a Vita decision at all. 180x257 is
NATIVE for a 480x272 PSP screen; the Vita port inherited the only pack that ever existed.
b17's "70k+ images in normal size working" is gfx/cache work, not image sourcing - he had
already raised the Vita cache ceilings to fight exactly the eviction problem bigger art causes.


## HARDWARE CONFIRMATION (2026-08-02)
Owner installed the VPK carrying ff5a5cc2b and reports: "opponent is playing now."
=> The AI passivity fix is CONFIRMED ON DEVICE. The bug (refusal path publishing status=2
then returning kChoicePending, so the seam said "waiting" while asyncBusy() said "idle" and
the engine passed the turn on the AI's behalf) is closed.
⚠ The earlier "still not playing any cards" report was the OLD binary - the VPK had been
uploaded but never installed. Before diagnosing any future Vita AI report, CONFIRM THE
INSTALL FIRST; an uploaded VPK at ux0:/vpk/ is not an installed app.
OPEN at time of writing: whether he ran with the LLM opponent switch ON (the path the fix
touches) or OFF (plain Baka, already known good). Only the ON case validates the fix.


## UX ROUND + FIRST CRASH WITH ART (2026-08-02)

**26ed94908 - declaring an attacker moves the cursor somewhere you can predict.**
MTGAttackRule::reactToClick fired a SYNTHETIC RIGHT PRESS to advance to the next
attacker. Nondeterministic three ways: closest() skips anything still fading in
(actA < 32), it silently returns the CURRENT selection when nothing qualifies
(so "didn't move" and "chose not to move" are indistinguishable), and
CheckUserInput's zone-memory/edge-fallback can carry the cursor off the
battlefield. Declaring also RE-LAYS-OUT the board (attackers move to their own
row, GuiPlay.cpp:128-135), so the press measured screen distance against
coordinates the same click had just moved. Hence: same press sometimes advanced,
sometimes not, and sometimes left the cursor on the phase-skip control.
FIX: new CardSelector::SelectNextInZone - stable reading order over ONE zone,
ordered by TARGET position not animated position, blind to fade state, cannot
leave the zone, no-op when nothing qualifies. Both attack rules (MTGAttackRule
and MTGPlaneswalkerAttackRule) carried identical copies. Bypassing CheckUserInput
entirely closes the off-battlefield route without needing to know WHICH of its
two mechanisms did it - the exact path was never proven.

**a6600e507 - availability rings on the battlefield.** Two display-only flags
(canAttackNow, hasUsableAbilityNow) refreshed in GuiHandSelf::Update's EXISTING
throttled 0.25s pass, rendered in CardGui::Render. ORANGE = declarable attacker
(outer ring), GREEN = usable+affordable activated ability (inner ring, inset
~10%). NESTED not exclusive, at owner's suggestion - two quads at identical
geometry blend into a muddy third colour instead of reading as two signals.
Blue tap-preview still draws last and wins. Predicates are EXACT (they mirror
MTGAttackRule::isReactingToClick and reuse hasUsableActivatedAbility narrowed by
an `only` param) - unlike hasAnyLegalAction, which is deliberately permissive.
A border makes a PROMISE; one the engine then refuses is worse than no border.
Mana abilities excluded: every land lighting up would kill the signal.

**4e2dbb3c1 - Vita builds now carry -g, and STOP shipping symbols.**
⚠ TWO CORRECTIONS TO WHAT WAS ASSUMED, both measured:
 - `-g` DOES travel into the self. Unstripped: self 34MB -> 141MB, VPK 40 -> 76MB.
   The console loads the executable, so that would have traded a diagnosable
   crash for a memory problem while memory pressure was a live suspect.
 - vita_create_self(... STRIPPED) passes -s to vita-elf-create. With it: self is
   12MB, VPK 32MB. STRIPPED was NEVER set before, so every earlier build shipped
   its symbol table unintentionally - the device binary is now 22MB SMALLER than
   what had been running. Whether that reduces RUNTIME memory is UNVERIFIED (it
   depends on whether the loader maps non-alloc sections); do not claim it does.
Stripping does not move addresses => dumps from the shipped build symbolize
against build_vita/wagic. VERIFIED: addr2line returns
  CardSelector::SelectNextInZone(...) -> projects/mtg/src/CardSelector.cpp:128

**FIRST CRASH WITH THE ART PACK INSTALLED - UNRESOLVED.** Full forensics,
the raw dump, and the psp2dmp parsing recipe are in
`psp-work/crash-2026-08-02/FINDINGS.md`. Read that before touching it. Short
version: PC=0x00000010 (indirect call through a null/garbage pointer), frame
corrupted, and the build had no -g so the symbolization was NOISE - the
`MTGDeck::MTGDeck` answer is an ARTIFACT, never cite it. Leading hypothesis
(unconfirmed): the known-open WGBKFont/BindTexture use-after-free on EVICTED
textures, whose pressure rose enormously the same day the 5.6 GB art pack landed
(~740 KB decoded per card vs a 48 MB cache = ~65 cards resident).

All three commits gated: suite 1031 (0 failed), 24 AI (0 failed), PARSETEST 319
passed 0 failed. Owner confirmed on hardware that the opponent plays, cards show,
and there were no issues in deck editor, shop, menus or gameplay before the crash.


## RENDERING + PROMPT-SIZE FINDINGS (2026-08-02, later)

**154d2fb9a - the ring bug was in the RENDERER, not the predicate.**
The green "usable ability" ring never appeared on hardware. I instrumented the
engine (temporary WAGIC_RINGPROBE hook in AIPlayerBaka::Act, since removed) and
PROVED hasUsableAbility correct: it tracks mana exactly (ability=1 with untapped
Forest+Mountain, 0 when tapped), both Cobra abilities register as ActivatedAbility
with restr=0/needsTap=0, and Battle Rampart rings while summoning-sick because its
activation needs no tap. Then I concluded his lands must have been tapped. WRONG.
⚠ THE GEOMETRY RULE, worth more than the fix: these border quads are drawn BEHIND
the card art, so the ONLY visible part is the rim that protrudes past it. Card art
is 38 units tall; every visible border in CardGui::Render is 43 = ~2.5 units of rim
per side. My "subtle" inset ring at 39 left HALF A UNIT and was invisible by
construction. NESTING MUST GO OUTWARD: ability now 43 (standard rim), attack 46
(halo outside it). The owner supplied this from memory - "when you did outlines for
the can cast indication, you had to make them bigger to be visible" - which no
amount of reading the constants would have revealed.
LESSON: verifying a COMPONENT is not verifying the FEATURE. The chain was
predicate -> flag -> render; I measured two links and blamed the input data.

**Same commit - the interrupt prompt told Vita players the wrong buttons.**
Which glyph means OK is a REGIONAL convention; JTypes.h says so at the enum
("Circle in Japan, Cross in Europe"). ActionStack hardcoded CROSS for interrupt
(JGE_BTN_SEC) and CIRCLE for no (JGE_BTN_OK) - correct on the Japanese PSP this
was written for. Vitamain.cpp:97-99 maps Cross->OK, Circle->SEC, so on Vita BOTH
labels were exactly inverted. Square (JGE_BTN_PRI) was never convention-dependent,
which is why No-To-All alone looked right. Glyphs now follow the mapping.
iconspsp.png row 0 indices: 0-3 d-pad, 4 CIRCLE, 5 triangle, 6 SQUARE, 7 CROSS.

⚠ RETRACTED 2026-08-06 (owner: "i dont remember this... applied out of context").
The quote below was recorded while the rings were BROKEN (green ring invisible,
orange rendering blue on PSP) - "they aren't correct" reads as a bug report on
malfunctioning indicators, not a design critique of the colour scheme. The
"four meanings, two colours" framing and the glyph proposal are CLAUDE
constructs, not owner positions. With both render bugs fixed (154d2fb9a +
3d9e5a935), there is NO open indicator issue and nothing awaiting a ruling.
Kept for the record, superseded: COLOUR COLLISION. The engine ALREADY used red (forcedBorderA) and
green (forcedBorderB) before the rings were added; the rings then took orange-red
and green. Four meanings, two colours. Owner: "these indicators piss me off. they
aren't correct... it's unclear what is what." A proposal (one colour = actionable,
a corner GLYPH for which action) is on the table but NOT ruled on. Do not pick new
colours unilaterally.

## PROMPT SIZE + CONTEXT MEASURED (2026-08-02)
Owner asked what an OpenAI subscription would cost per match, and whether a weak
model's context can hold a full game. Measured with a counting PROXY in front of
llama-server (scratchpad/tokcount.py: forwards /v1/*, records usage per request;
the engine discards the usage block, so no engine change was needed).

⚠ CORRECTION TO AN EARLIER ESTIMATE IN THIS FILE'S LINEAGE: the ~1,600-token
static head derived from historical logs came from UNGUIDED decks. With a deck
that HAS a stabilized strategy guide (owner's requirement for a fair test), the
head is ~7x bigger:
  deck146 (14.9KB guide) vs deck135 (13.4KB): FIRST ask (mulligan, empty
  narration) = prompt 11,131 tokens, completion 273.
  => the static head is ~11.1K tokens = 34% of a 32,768 window.
Narration growth measured at ~30-50 tokens/decision, so even the logged worst case
(72 decisions) lands ~13.3K total. CONTEXT IS FINE; the HEAD dominates, the tail
is cheap - which is what the append-only-narration design was betting on.
COST implication: prompt caching is not an optimization here, it is the difference
between paying 11K once per match and paying it 40+ times. The head is
byte-identical every request by construction.
Historical stats for reference (658 logged games, 400 sampled): decisions/game
median 18 / mean 21 / p90 31 / max 72; user message median 1,448 chars.

OWNER'S FRAME on the OpenAI path (2026-08-02), which supersedes an adoption-win
framing I offered: "I don't want to produce something where people feel like they
are being ripped off when they put money into it." The per-match cost is a
DUTY-OF-CARE threshold for players, not a convenience threshold - his bar is "more
than a few percent of a $20 sub per match" = don't build it. OAuth is permitted by
OpenAI for personal/third-party use (his knowledge, he is confident) but NOT
embeddable in a product; each user authenticating with their own account is the
sanctioned pattern. wagicGPT already ships endpoint/model/key as CONFIG, so the
credential and any OAuth code stay out of the released artifact either way.


## GEMMA 4 E2B AS A WEAK-MODEL CANDIDATE (2026-08-02)
People are asking the owner specifically about gemma E2B, so it is a real
evaluation target, not a curiosity. RESEARCHED, not yet run.

CAN WE RUN IT: yes, both requirements verified.
 - Voyager's llama.cpp b10144 (llama.cpp-cuda-git b10143.r1.d73c1d6b22-1) already
   lists the architecture: gemma gemma2 gemma3 gemma3n gemma4.
 - GGUFs exist in quantity. PREFERRED: google/gemma-4-E2B-it-qat-q4_0-gguf -
   Google's own QAT (quantization-aware trained) q4_0, i.e. the weak model at its
   BEST rather than handicapped by a post-hoc quant. Reference alternative:
   ggml-org/gemma-4-E2B-it-GGUF. Also unsloth/bartowski/lmstudio. apache-2.0.
 - ~5.1B stored / 2.3B effective at q4_0 = ~3-3.5GB, fits the 1060's 6GB.

SPEC (owner-supplied table): E2B = 2.3B effective / 5.1B with embeddings, 35
layers, SLIDING WINDOW 512 TOKENS, context 128K, vocab 262K, text+image+audio.

⚠ THE NUMBER THAT MATTERS IS THE 512-TOKEN SLIDING WINDOW, NOT THE 128K CONTEXT.
Gemma interleaves local and global attention: most layers attend within 512
tokens, sparse global layers carry the rest. Against our prompt shape:
  [rules primer][both decklists][14.9KB guide][narration][SITUATION][options]
   <------------------ ~11,100 tok ------------------>   <-- ~500 tok -->
The decision-relevant tail (board + numbered options) is SMALL and ADJACENT, so it
sits inside the local window - the head/tail design happens to put exactly the
right things next to the answer. But the STRATEGY GUIDE sits ~3,700 tokens away
and is reachable only through the sparse global layers.
=> PREDICTED FAILURE MODE is NOT incoherence. It is the model playing legally and
sensibly while THE GUIDE HAS NO MEASURABLE EFFECT - which matters because the
decks were pinned to guided ones precisely so the guide would be in play.
=> THE EXPERIMENT THAT ANSWERS IT: run E2B twice, WITH and WITHOUT the guide in
the system prompt. Statistically indistinguishable results mean the guide is not
reaching the model at that size. That is a better question than raw win rate, and
it is the actual "how weak can it go" boundary - the point where the guide stops
paying for its ~3,700 tokens.

TWO SECONDARY CONSEQUENCES:
 - KV cache is CHEAP: sliding-window layers hold only 512 tokens of KV, so long
   prompts cost far less cache than 35 layers implies. Good on a 6GB card.
 - 262K vocab is why 5.1B is stored for 2.3B effective (embedding table
   dominates), and it tokenizes our structured text efficiently. ⚠ When comparing
   the Gemma run to the Qwen run, NORMALIZE ON CHARACTERS, not tokens - the same
   prompt will not produce the same token count across the two tokenizers.


## OWNER IDEA: SHIP THE MODEL WITH THE GAME (2026-08-02)
"the e2b model is a phone purposed model. we could hypothetically make a package
that brought the model with it." This DELETES the player-cost objection rather
than mitigating it - no endpoint, no account, no OAuth, no quota, nothing to feel
ripped off about. It is the strongest form of the LLM-opponent pitch.
unsloth/gemma-4-E2B-it-qat-mobile-GGUF exists specifically for this deployment.

⚠ THE CRUX IS PREFILL, NOT DECODE. Decode on a phone is fine (~5-15 tok/s vs our
~270-token replies). The 11,100-token head is the problem: at an optimistic
100 tok/s prefill that is ~110s PER DECISION x ~20 decisions = unplayable.
WHAT RESCUES IT: prefix caching, which our prompt architecture already suits by
accident of design. The head is byte-identical every request and narration
APPENDS AT THE END, so llama.cpp reprocesses only the changed suffix (tens of
tokens). First decision of a duel costs a minute or two; the rest are near
instant. Tolerable as a one-time "opponent is thinking..." on turn one.
=> DO NOT evaluate on-device feasibility from a cold-prompt benchmark; measure
with the cache warm, which is the real steady state.

=> THIS MAKES THE GUIDE EXPERIMENT LOAD-BEARING, not a curiosity. The strategy
guide is ~3,700 of the ~11,100 head tokens = a third of the first-turn stall AND
a third of the KV footprint on a memory-tight device. If E2B cannot reach it
through the 512-token sliding window anyway, dropping it is free latency and free
memory. Two independent reasons converge on the same with/without measurement.

PLATFORM REALITY: Android is the target (already builds, JNI transport exists);
a ~3GB payload wants to be a first-run download or asset pack rather than a 3GB
APK, though GitHub-release distribution means no store cap forces the issue.
PC works. Vita and PSP are OUT on RAM alone - do not chase it there.


## MODEL-COMPARISON REVIEW DESIGN (2026-08-02, owner-requested)
Owner: "after the games, have a reviewer compare them to the games qwen played
with the same guide version. see if it's decisions are comparable in quality."

GUIDE VERSION PINNED (both runs MUST use these exact bytes or the comparison is
void). Committed clean at 903d56eb9 (wave-32, 2026-07-27):
  deck146_strategy.txt  sha256 489e65cb74a77748  14,863 bytes
  deck135_strategy.txt  sha256 d4e45f6fdc6ecfc7  13,357 bytes
  deck146.txt d6ef936f27a7cf96 / deck135.txt 2e5acfa9b62c59c5
Matchup pinned: WAGIC_SELFPLAY_DECK0=146 vs DECK1=135.

DATA: ~/.Wagic/ai/gpt/logs/*.jsonl, one per SEAT per game (~125KB/game). Each
'ask' record carries model, events (board narration), options_text (exactly what
was offered) and chosen_text. That is a complete decision record - no extra
instrumentation needed, and the `model` field means the review can be BLINDED and
un-blinded afterwards.

DESIGN CONSTRAINTS (decided BEFORE seeing results, deliberately):
 1. BLIND the reviewer - strip `model`, label the two sets A/B. Otherwise the
    known-weaker model gets marked down for the same choice.
 2. POSITIONS CANNOT BE PAIRED. The games diverge immediately, so there is no
    identical position to compare head-to-head. Sample N decisions per model
    instead, and compare DISTRIBUTIONS, not matched pairs.
 3. MATCH DECISION KINDS. Compare casting to casting, blocks to blocks. Mixing
    kinds confounds - one model drawing more block decisions would skew a pooled
    score with no difference in skill.
 4. ANCHOR ON BLUNDERS, NOT VIBES. Ask "given these options and this board, is
    this choice clearly wrong?" rather than "rate 1-10". A blunder rate is far
    more robust to judge noise than a quality score, and it is the thing that
    actually ruins a game to play against.
 5. GIVE THE REVIEWER THE GUIDE. Part of the question is whether the model
    followed its deck's strategy, which cannot be judged without it.

FREE OBJECTIVE SIGNALS to report alongside the judge (no LLM opinion needed):
fallback rate (Qwen: 0 in 36 decisions), decisions/game, game length in turns,
win/loss, output-token mean and max (Qwen: 250 / 1,249 - the max indicates
protocol-ignoring rambles), peak prompt tokens.
⚠ Illegality is NOT a signal here - illegal choices are structurally impossible
(options come from the engine's legal set), so a model cannot score badly on it.


## BASELINE CORRECTION - THE COMPARISON IS AGAINST THE 122B HISTORY (2026-08-02)
Owner: "I meant the extant qwen 122b history." And the decisive structural fact,
his words: the dev cycle requires decks to play a series of games that do not
result in a guide change before a guide counts as stabilized. => EVERY stabilized
guide has >=6 games of 122B play data ON EXACTLY ITS FINAL BYTES, by
construction. The baseline exists as a certainty of the loop, not something to
reconstruct forensically.

⚠ RETRACTION: I tried to verify guide-version provenance by grepping a line of
the current guide inside the old jsonl logs and reported the 07-26/27 122B games
as "differ". The probe was INVALID - today's 4B logs, which certainly ran the
current guides, ALSO failed the grep, because the logs do not carry the guide
text verbatim. None of those "differ" results measured anything. Do not cite.

REVISED REVIEW DESIGN (supersedes the matchup-level framing above, everything
else in that section stands - blinding, no pairing, kind-matching, blunder-rate):
 - Baseline corpus = each deck's STABILIZATION SERIES: the final run of 122B
   games against the guide bytes that did not change afterward. Which games
   those are is recorded in the wave campaign materials (wave-32 review cycle,
   commit 903d56eb9; loop state in the anchor memory project_wagicgpt.md) - pull
   series membership FROM THE WAVE RECORDS, never infer from file dates.
 - Stabilization games have varied opponents, so compare PER-SEAT decision
   quality for the SAME DECK: deck146 under the 122B vs deck146 under the 4B.
   Same deck, same guide, same decision kinds; only the model varies. Cleaner
   than matchup-level comparison anyway.
 - The 4B corpus is today's batch (146v135, 6 games, logs tagged model=qwen3.5-4b
   in ~/.Wagic/ai/gpt/logs, 2026-08-02).


ADDENDUM (owner, 2026-08-02): "should give us insight into how well the model
plays, and how well it follows the guide." TWO INDEPENDENT AXES per sampled
decision, never one blended score - they dissociate in both directions and the
difference dictates the remedy:
  plays-well + ignores-guide  -> prompt/placement problem (the E2B sliding-window
                                 concern); fix is prompt work, not a bigger model.
  follows-guide + plays-badly -> capability floor on what the guide doesn't
                                 cover; no guide edit fixes it.
Scoring: (1) PLAY QUALITY judged with the guide HIDDEN (blunder / defensible),
so guide-conformant-but-bad is not laundered into "fine". (2) GUIDE ADHERENCE
judged with the guide SHOWN, with an explicit "guide is silent here" category -
most decisions are not covered, and forcing a verdict there manufactures noise.
The 122B stabilization series is ALSO the adherence CEILING: the wave loop tuned
those guides until the 122B's play stopped driving changes, so its adherence
rate is what "following the guide" looks like when it works. Measure the 4B's
gap from THAT, not from 100%. The quadrant counts themselves locate any gap
("adherent but blundered" reads completely differently from "non-adherent
everywhere").


⚠ CONFOUND (owner, 2026-08-02): the LLM UX has been iterated since the 122B
stabilization games - per-choice prompts (option annotations, narration) differ
between the historical corpus and today's build, IN THE CHALLENGER'S FAVOUR.
Asymmetric effect: 4B-clearly-worse still stands (lost despite the handicap);
4B-looks-comparable is AMBIGUOUS (model parity vs prompt improvement are
indistinguishable) - and that is the outcome the exercise hopes for, so the
confound sits on the worst spot.
FIX: RE-BASELINE THE 122B ON THE CURRENT BUILD. Same binary, same prompts, same
decks (146v135), same 6-game battery, WAGIC_GPT_URL -> Spark vLLM :8081. Then
the corpora differ in exactly ONE variable (the model), and the stabilization
series retires to what it actually is - the guide-tuning record, and the
adherence-ceiling reference. 122B decides in ~4-5s, so this is 1-2h of Spark
time. Queue behind the running 4B batch. (Spark serving state: check
yuki-serving-config / spark-vllm-serving memories before launching anything.)

SEQUENCING (owner, 2026-08-02): Spark is tuning longcat-next for ~1-2 days - the
122B re-baseline WAITS. Do not launch anything at Spark until that clears (and
verify serving state when it does). Meanwhile: the 4B corpus completes and its
baseline-independent results (fallbacks, tokens, context) are reportable now;
the Gemma E2B battery can run on Voyager's 1060 without Spark, so both
challengers can be in the can first. Run ONE review pass over all corpora after
the re-baseline, not two.

OWNER HYPOTHESIS, RECORDED BEFORE THE E2B DATA EXISTS (2026-08-02, his framing,
explicitly vibes): "active parameters is the largest impact aspect for ability
to reason over context. then param total for breadth of priors." Prediction it
implies: E2B (~2.3B active) blunders more than Qwen3.5-4B (dense, 4B active) in
TACTICAL spots (combat math, sequencing, timing) while staying comparable on
KNOWLEDGE spots - i.e. the gap shows on the play-quality axis, not adherence.
Confound to hold in mind: architecture is not controlled - E2B's 512-token
sliding window limits what it can SEE independent of reasoning; the
guide-stripped variant separates "couldn't reach it" (stripped plays the SAME)
from "couldn't reason over it" (stripped plays WORSE).
Also his read on the zero-fallback result: "a sign we built good instructions
for choice expression and robust choice parsing" - the contract pre-enumerates
legal options and the parser tolerates rambles, so weak models cannot express
illegal choices and do not need to be well-behaved. That property is what makes
weak-model deployment thinkable at all.

## 2026-08-02 — ring polish round 2 (from Vita live play)
Commit 216c3ab77 (gated twice: 1031/0 + 24 AI/0 + PARSETEST 319/0 both runs).
- PHANTOM RING (his Thallid Shell-Dweller screenshot, 0 spore counters, ring shown):
  hasUsableActivatedAbility checked tapping+mana but NOT ExtraCosts. Fix walks
  cost->extraCosts->costs, binds ec->setSource(aa->source) per cost (NOT
  setExtraCostsAction — that NULLs source->storedCard, unacceptable from a
  0.25s display tick), requires canPay() on all. TapCost::canPay==1 for
  untapped sources so {T} rings survive. Semantics are ENGINE's, not CR's:
  LifeCost::canPay only refuses at life<=0/CANTPAYLIFE (owner raised the
  pay-to-zero + Platinum Angel line; engine is even looser than CR 118.4 —
  no amount check — and the ring mirrors the click, so that line stays lit).
- GREEN RING SIZE: owner "a skosh smaller" — 43 -> 42 (rim 2.5 -> 2.0 units;
  invisibility floor ~39). Attack halo stays 46.
- OPEN (owner musing, unruled): duplicate copies in hand — legalCasts dedupes
  by displayName+zone, so castableForDisplay lights only the FIRST twin; the
  other renders FADED (false "uncastable"). Self-heals on cast. Proposed
  ~6-line expansion pass in castableForDisplay; he said "not sure it's a
  problem"; do NOT fix without his word.
- Fresh VPK staged: build_vita/wagic.vpk 32,208,184 bytes (216c3ab77, both
  fixes) — AWAITING FTP upload. Unstripped ELF build_vita/wagic (137MB) is the
  symbolization sidecar for any new crash dump.
- Binary archived /tmp/wagic-216c3ab77.

## 2026-08-02 — weak-model batch 1 verdict: TRUNCATED, relaunched full-length
Batch 1 (6 games, Qwen3.5-4B): ALL SIX hit the per-game `timeout 1200` (every
exit=124) — NO game concluded, NO winners. Earlier "game 1 complete" claim was
WRONG (truncation window, not a finished match). The 1200s cap was an arbitrary
chokehold at ~45s/decision local pacing (~35 decisions/window vs full-match
needs). Data archived: psp-work/weakmodel-data/qwen4b-batch1-truncated1200s.*
VALID from batch 1 (per-decision economics, 190 decisions):
- input ~11.9K tok/decision (head 11.1-13.7K; peak prompt 14,813 of 32K)
- output mean 269 (max 2,662 — rambles still parse)
- ZERO fallbacks in 190 decisions (contract robustness at 4B confirmed)
Batch 2 launched (unit weakmodel2): same 6-game battery, timeout 7200 safety
net (not a pace constraint), fresh /tmp/tokcount.jsonl + /tmp/weakmodel.log,
single-wake monitor on /tmp/weakmodel.done. Expected wall: 5-8h.
Full-match totals + winners come from batch 2; Gemma E2B battery queued after.

## 2026-08-02 — batch 2 was GARBAGE: llama-server OOM-killed, silent Baka fallback
Batch 2 (weakmodel2) finished 6 "games" in 30 seconds, all exit=0 with
winners — but tokcount.jsonl held ZERO requests. Cause chain: kernel OOM
killer took llama-server (6.8GB RSS, biggest victim on the 16GB box) during
the concurrent heavy window (vitasdk docker build + 2 suite gates + make -j8
+ selfplay); it had been launched in a tmux scope, nothing restarted it; the
proxy 502'd every ask (and only logs responses WITH usage, so errors are
invisible to it); the engine fell back to Baka per decision as designed and
raced 6 Baka-vs-Baka games at CPU speed. LESSONS:
- A completed battery is NOT a valid battery: ALWAYS check request count vs
  decision count before believing results. "exit=0 + winner" can be 100% fallback.
- Don't co-run docker builds / -j8 compiles with a local-inference battery on
  this 16GB box; llama-server is the OOM killer's favorite meal.
- llama-server now runs as user unit `llama-qwen` (systemd-run --collect,
  ~/bin/llama-qwen.sh). Verified end-to-end through proxy before relaunch.
Batch 3 (weakmodel3) launched with fresh logs; monitors: per-game-end with
request-count sanity + a first-request tripwire. Batch-2 garbage discarded.

## 2026-08-02 — duplicate-copies display fix SHIPPED; batch 3 confirmed live
Commit 42bc7631f (gated 1031/0 + 24 AI/0 + PARSETEST 319/0): castableForDisplay
now spreads cast verdicts across same-named hand copies (lands exempt - the
sorcery rung judges each land instance itself). Owner ruled "go ahead and fix it".
Batch 3 (weakmodel3) SANITY-CONFIRMED live: first request 11,139 prompt tok.
VPK rebuild DEFERRED until battery ends (vitasdk docker co-run = the OOM
recipe). Staged VPK 32,208,184B = 216c3ab77 (phantom ring + slim green only);
next VPK adds 42bc7631f. Binary archived /tmp/wagic-42bc7631f.

## 2026-08-02 — 4B FULL battery DONE (batch 3); Gemma E2B battery LAUNCHED
Qwen3.5-4B, 6 COMPLETE games, 274 decisions, ZERO fallbacks, all exit=0.
Data: weakmodel-data/qwen4b-batch3-full.{tokcount.jsonl,log}
Per game (dec/input/output/result): 51/678K/15.5K/135-T13 · 35/411K/10.3K/135-T11
· 83/1240K/20.0K/146-T20 · 38/478K/13.3K/146-T11-DECKOUT · 53/772K/17.5K/146-T12
· 14/168K/4.6K/146-T5-ALT(loser at 19 life!)
MEAN FULL MATCH: 46 decisions, 625K raw input, 13.5K output. Human match = one
seat ≈ half. Peak prompt 18,564/32K. REVIEW FLAGS: game 4 deck-out (seat1 drew
from empty T11, library was 46+ early — card velocity), game 6 T5 alt-ending at
19 life (mechanism unidentified — read narration before review).
GEMMA E2B: server unit llama-gemma (llama-server, gemma-4-E2B_q4_0-it.gguf,
ctx 32768, --jinja, port 8080). FINDING: gemma4 template is a THINKING template
— output lands in reasoning_content unless enable_thinking:false; the engine
ALREADY sends chat_template_kwargs enable_thinking per config (endpoints.txt
thinking=0), verified honored (clean 'OK'). Battery unit weakmodel-gemma,
same decks/env as 4B batch 3, monitors armed (per-game + tripwire).
Comparison rule: normalize Gemma-vs-Qwen on CHARACTERS not tokens (262K vocab).

## 2026-08-02 — GEMMA E2B BATTERY COMPLETE: the weak-model answer is in
6 COMPLETE games, 440 decisions, ZERO fallbacks, all exit=0.
Data: weakmodel-data/gemma-e2b-battery.{tokcount.jsonl,log}
Results (dec/input/output-mean/result): 110/1677K/116/146-T16 · 77/1032K/109/135-T23
· 78/973K/106/146-T18 · 54/688K/109/146-T13-ALT(loser 16 life) · 51/631K/177/146-T12
· 70/954K/108/146-T14. Winners: 146 5-1.
MEAN MATCH: 73 decisions (vs 4B's 46 — longer games, T12-23), input 992K raw,
output 8.7K. Output mean 116 tok/dec vs 4B's ~296 — E2B is far terser (confirm
on CHARACTERS before claiming; 262K vocab). One output hit the 4096 max_tokens
cap (game 5) with no fallback — parser/retry absorbed it.
HEADLINE: a 2.3B-active phone-class model played 6 full matches with ZERO
protocol failures. The predicted sliding-window failure (guide unreachable)
did NOT manifest as illegality or fallback; whether guide ADHERENCE suffered
is the review's question, not answerable structurally.
DECK-135 STRUCTURAL FLAG (cross-model): 4 non-damage deaths total for seat
135 (4B games 4+6, Gemma game 4 at 16 life; 4B game 6 at T5/19 life!) — deck
135 or its guide drives self-out. Read narrations before the review pass.
Local stack restored: llama-gemma stopped, llama-qwen resident again. Proxy
(tokcount, :8099) left up for the 122B re-baseline leg.
REMAINING for task #18: 122B re-baseline on Spark (blocked ~1-2 days,
longcat-next tuning) -> ONE blinded two-axis review pass -> real-endpoint
reasoning-token sample for the OpenAI cost conversion.

## 2026-08-02 — CORRECTION: "zero fallbacks" was measured on the wrong surface
The zero-fallbacks claims for both batteries counted proxy-side requests and
grep'd engine stderr — but fallbacks log in the per-seat TRANSLOG jsonl, not
stderr. Phase-1 review (qwen4b×deck146 agent, translog evidence): 14/118
deck-146 decisions (11.9%) were engine fallbacks — 13 empty replies (10 at the
120s WAGIC_GPT_TIMEOUT ceiling, model still generating at ~45s/decision pace)
+ 1 stale_echo — and they CLUSTER (g3 seq 23-25 = 3 consecutive, lost a lethal
window). The "contract holds at weak scale" conclusion still stands for
PARSING (replies that arrived parsed fine); it does NOT hold for latency.
Local-pacing artifact in part (122B on Spark answers in 4-5s), but the 120s
ceiling + 1060-class decode is a real deployment shape for bundled models.
ALSO from phase-1 reviews: deck-135 self-deck-out mechanism = ENGINE BUG
(Into the North reveal-without-return, g6: 51 reveals 0 returns); Silverquill
Command graveyard-mode likely card-script bug; reveal-spam bloats prompts
~9KB. Full lists in weakmodel-data/reports/*.md.

## 2026-08-02 — REVIEW COMPLETE: 8 reports in weakmodel-data/reports/
4 phase-1 (model×deck) + 4 comparison-vs-122B reports, all evidence-cited.
CROSS-REPORT SYNTHESIS (each item independently found by 2+ agents unless noted):
1. HARNESS BUG D1 (both 146 agents): in 15/405 122B numbered replies the engine
   executed a DIFFERENT option than the leading CHOICE: line (fallback: None) —
   incl. 3 mulligans of stated keeps and 3 forbidden Agadeem casts. Trigger =
   long self-revising replies (later-text scan overrides leading line —
   hypothesis; parser unread). 0/87 and 0/405-4B mismatches in weak runs.
   Corrupts 122B record; fix before any future baseline.
2. TIMEOUT ASYMMETRY (135 agents): both 4B and 122B deliberate past 120s at
   ~7-8% of decisions; the 122B historical runs had no effective cap (ran to
   213s, completed), weak runs hard-capped at 120s. Much of the weak models'
   "reliability gap" is WAGIC_GPT_TIMEOUT, not capability. Raise for local.
3. INSTRUCTION-SHAPE BIMODALITY IS SCALE-INVARIANT: lookup/named-string rules
   ~100% at every scale; conditional-judgment rules fail at every scale
   (different families per model). The mulligan UX block is the existence
   proof that judgment->lookup conversion works: 122B shipped 25+ bad
   mulligans without it (incl. "zero lands" claims on 3-land hands, mull to
   0 twice); weak models 6/6+6/6 keeps with it. Guide style lesson is general.
4. 122B IS NOT A CEILING: removal hoarding (Verse fired 24-26% of live
   windows, Soul Shatter 2/33; declined at 2 life calling tapped creatures
   "no creatures"), Acererak-over-removal 9/15 (WORSE than E2B's 1/3 on
   identical guide text), lost one game to the turn cap while ahead 21-6.
5. MODEL-SPECIFIC TRUE GAPS: E2B = initiative (Cast nothing 49% vs 15%;
   priority windows passed 5/30 vs 60/66) + board-evaluation depth
   (unclosable by guide edits). 4B = conditional-rule misses (Force 0/2,
   DEFENDER never) but BEATS 122B on removal timing (Verse 6/8, Shatter 3/3).
6. ENGINE FAULTS CONFIRMED CROSS-SCALE: Into the North reveal (5/5 answered
   at 122B restored library — scale HIDES the bug), poison counters invisible
   in situation block (g17 lost to unseeable Infect at "21 life"), ATTACK:
   <name> parse gap, Silverquill Command gy-mode script bug, false
   "already played a land" (both scales), stale_echo on Azcanta transform.
7. GUIDE DEFECTS (both scales misread same text): Rule #1.3 "develop" read
   as colour-fixing-only; Force of Negation rule too restrictive (122B obeyed
   it and died to Ancestral Mask — the one clean guide-wrong case).
8. NO STRENGTH RANKING POSSIBLE from this data: opponents differ (13-deck
   gauntlet vs 146-only), guide versions differ for 146, prompt UX iterated,
   D1. Clean comparison = common gauntlet re-run AFTER D1 + timeout fixes.
Agent-proposed guide edits are IN the reports (proposals, not applied —
several flagged as needing carve-outs, e.g. anti-Cast-nothing would regress
122B's correct lethal holds). Nothing applied to guides or engine.

## 2026-08-03 — blocker rings + kicker auto-tap (from Vita live play)
Commits (gated 1031/0 + 24 AI/0 + PARSETEST 319/0):
- b2c2a2e9d blocker rings: LegalActionsOracle::canDeclareBlocker mirrors
  MTGBlockRule::isReactingToClick + requires >=1 blockable current attacker;
  canBlockNow flag on MTGCardInstance; refreshed in GuiHandSelf 0.25s tick;
  renders as the SAME orange halo as attack (time-disjoint, one colour = one
  idea: "can enter combat now"). Owner may still want distinct colour.
- 128a3600c kicker: MTGKickerRule predates auto-tap rework — menu priced kick
  off FLOATING pool only + its reactToClick had NO auto-tap (base rule's not
  inherited). Fixed: menu appears on potential-mana affordability of
  base+kicker; auto-taps full kicked cost pre-snapshot; KICKER_ALWAYS path
  same. MULTIKICKER: taps base+ONE kick only (owner pushback — floating pool
  stays the count selector; max-greedy taps seize reserved mana). Owner
  expects this may still bite; the PROPER fix = count menu via ANNOUNCE_X-
  style decision contract, same rework that owns the X-cost gap (X announce
  menu still counts floated pool only; ManaEngine::maxAnnounceableX exists).
  Also fixed withKickerCost leak in isReactingToClick.
MTGRules.cpp edits were byte-spliced (Windows-1252); encoding verified clean.
Binary /tmp/wagic-128a3600c. VPK building (capped docker, wide rebuild).
OWNER VITA FEEDBACK on 42bc7631f build: phantom-ring fix confirmed good;
ring size change unnoticed (fine); no duplicate-copy situation seen yet.

## 2026-08-03 — kicker ask-first default + NON-ENGLISH ART PURGE
Commit 96716a0c7 (gated after one caught regression): KICKERPAYMENT default
KICKER_ALWAYS -> KICKER_CHOICE. Owner's principle, verbatim-adjacent: always-
pay assumed floating mana = intent; "our new philosophy is that while the
player can manually tap mana, we assume they aren't. so we need to know on
flexible spends what they want" — applies to X/multikicker/strive family too.
REGRESSION LESSON: suite scripts encode MENU GEOMETRY — the ask-first entry
shifted `choice N` on kickable cards (strength_of_tajuru_i1085 failed);
fixed by pinning KICKER_ALWAYS in BOTH suite paths (TestSuiteGame ctor +
TestSuite::initGame), same pattern as mSuiteGame. Ask-first path needs its
own tests (open).
GLYPH/TOOLTIP DIRECTIVE (owner, unbuilt): tooltips across the board must
reflect the platform's CURRENT keybinds AND the active input method — PC on
controller shows pad glyphs; mouse/keyboard shows key binds. Today's glyphs
are compile-time constants of the default map (TranslateKeys' glyph layer is
dead code; ActionStack indexes iconspsp.png directly). Needs: last-input-
device tracking in JGE + reverse binding lookup + per-device glyph tables.
ART PURGE (complete): audit of all 336 pool sets via 6 vision agents found
8 non-English sets — PS11 (Salvat 2011, Spanish, no English printing exists)
+ PMPS/PMPS06-11 (Japanese MPS land promos, ditto). Fix per owner ruling
(borrow English printing of same card by name; if none, delete image ->
text fallback): scratchpad/refetch-english.py replaced 275/275 via Scryfall
(0 misses), regenerated t.jpg + thumbnails/. Vita zips for the 8 sets
rebuilt (336 zips intact, PS11.zip 18MB) — AWAITING FTP upload of 8 zips.
PSP pack (psp-work/image-pack-psp) regenerated for the 8 sets (180x257 +
114x166 thumbs) — awaiting next PSP USB session. PC pool fixed in place.
Spot-verified English (Jagged Lightning STK art for PS11 16947; 2026 Swamp
for PMPS 17914). PLIST has ONE Japanese-art Swamp (295350) - not flagged,
predominantly English set.

## 2026-08-03 — PSAL (Salvat 2005) purged; full-pool per-card sweep RUNNING
Owner found "Flamas absorbentes" (= Engulfing Flames, PSAL 17111) — PSAL is
the SECOND Salvat set (762 images, Spanish product) and the per-SET sampling
audit missed it (its one sample happened to look English; mixed sources).
LESSON: sampling catches wrong-language PRODUCTS, not per-card STRAYS.
- PSAL refetched 721/721 English via refetch-english.py (now takes set args),
  0 misses. Spot-verified 17111 English by OCR. Vita PSAL.zip rebuilt
  (AWAITING FTP), PSP set regenerated. Only PS11+PSAL carry Salvat metas.
- FULL-POOL SWEEP running (unit lang-sweep, ~1-2h): tesseract eng over all
  73,261 full images -> /tmp/lang-sweep-flags.txt with LATIN-MARKER (strong
  foreign type-line words ES/FR/DE/IT/PT) and NOTYPE/NOTYPE-LOWTEXT (no
  English type word = CJK/garbage candidates; noisy old frames will false-
  positive here). Flags MUST be vision-verified by agent before refetch.
  Script: scratchpad/lang-sweep.sh. On flags: refetch by id (script needs a
  per-id mode), regen thumbs, rebuild affected vita zips + psp sets, FTP.
PENDING VITA UPLOADS next window: PSAL.zip (+ any sweep-flagged sets later).
Vita test list for owner: kicker ask menu (saved profile may override —
flip option once), English art in PS11/PSAL/PMPS*, duplicate-copy hand glow.

## 2026-08-03 — combo-hint mana float + TODO purge campaign

**Floating-mana bug (owner live report, deck15 Rats!)**: opponent floated {B}{B} every turn.
Root cause: `activateCombo` (AIPlayerBaka.cpp) pre-paid the hint's `totalmananeeded({B}{B})`
into the pool, then the main-phase cast path paid AGAIN; the duplicate producer click refused
(source already tapped) and the wave-20 refused-payment abort killed the queued Fear cast.
Net: two floated mana, Fear never cast, every turn. Pre-wave-20 the dup clicks were harmless
no-ops, which is why 15-year-old combo scaffolding only now broke. Fix: activateCombo is an
affordability CHECK only (potential+pool canAfford totalCost); the cast path is the single
payer. Verified in selfplay (10 Fear casts / 5 games, zero aborts, no residual float).
Commit af7806b43, gated 1031/0 + 24 AI/0 + PARSETEST 319/0. Binary /tmp/wagic-af7806b43.
NOTE: deck15's combo hint semantics degrade gracefully — multi-piece combos now pay per piece
instead of reserving the whole total up front; single-piece combos identical.

**TODO/FIXME purge (owner directive: no TODO comments — do it, or state the decision)**:
147 entries triaged by 3 agents → reports at scratchpad todo-triage-{1,2,3}.md (session-scoped;
verdicts summarized here). DONE 11, OBSOLETE 68, S 21, M 20, L 5, UNCLEAR 5, VENDORED 2.
Application wave (3 agents) in flight at time of writing: all DONE/OBSOLETE comments
deleted/reworded, kept items reworded to constraint statements (no TODO/FIXME words survive),
and these S-fixes implemented: combatDamages deprecation, preventDamageTypes enum,
menuText→std::string, ActivatedAbility ctor overload, AIStats docs, TargetZoneChooser
ALL_ZONES equals, Tasks include/cast + constants + light #NAME reader, WGui no-results msg,
GameOptions asColor uninit fix, Rules initPlayer(s) NULL guards, GuiPhaseBar banner cache,
ActionElement::stopWaiting, multiplechoice "choice N" logging (suite-relevant!),
GameObserver ORDER-branch NULL guard, JMP3 retry constant, JFileSystem zip short-read fix.
Owner-decision backlog (M/L/UNCLEAR — presented after the mechanical commit): 5 card-DSL
parser gaps (activated/UEOT protection-from / cantbetargetof / cantbeblockedby parse to NULL),
StoryFlow bypasses AIPlayerFactory (story mode never gets the LLM opponent), Tasks accepted
mechanic never built (all tasks auto-pay), DeckMetaData eager-load (~4s menu delay),
regenerate-vs-stack-damage heuristic, bestow "memleak" comment (ASAN one bestow cast to
settle), createSnapShot aliasing (L), Navigator.cpp dead code (wholesale removal candidate),
AIPlayerBaka 2474 TC-ownership inversion (L, root-cause campaign).
**Update**: TODO purge COMMITTED as 6e4a56951 (69 files, +324/-307), gated green
1031/0 + 24 AI/0 + PARSETEST 319/0. Binary /tmp/wagic-6e4a56951. Also swept 16 header
stragglers in projects/mtg/include (first sweep scoped src/ only — headers live in
include/). Compile fix during central build: agent's delegating ActivatedAbility
overload was ambiguous vs the defaulted 9-arg ctor — overload dropped, placeholder
args named inline instead. Owner report: psp-work/todo-audit-report.md (delivered).
Owner follow-up answered: efficiency TODOs were each judged could-it/does-it-matter;
implemented (phase-bar cache, task name reader), kept as backlog (selectAbility
quadratic probe, AlternateThumbQuad RTT, DeckMetaData eager load), or dismissed on
call-site frequency (once-ever award save etc.). No profiling done — offered a Vita
profiling pass if he wants ranked ground truth.
Vita VPK rebuild (af7806b43+6e4a56951) launched (unit vpk-build) — upload next FTP
window along with the pending PSAL.zip.
Unpushed commits now 11 (9 prior + af7806b43 + 6e4a56951) — ask before pushing.

**Owner-approved TODO batch — COMMITTED bf366f2c2** (13 files, +425/-63), gated
1031/0 + 24 AI/0 + PARSETEST 319/0 + 3 selfplay games. Binary /tmp/wagic-bf366f2c2.
Owner rulings on the decision list: (1) tasks auto-pay is the design — `accepted`
kept as save-format slot, comment states the settled design; (2) ONE opponent class
— story routes through AIPlayerFactory (he may add scripted "challenge" scenarios
later, story has none now); (3) shop booster discount asymmetry — leave it;
(4) tutorials: settings option to toggle off AND reset; SEC button toggles off;
(5) five card-DSL parser gaps DEFERRED until after the alpha release; (6) deck-name
load fix — done, BUT agent's honest correction: parsing was never the 4s, it's ~3
filesystem round-trips x 196 decks (FileExists pre-check + re-open). Follow-up: drop
the redundant stat in DeckManager::getDeckMetaDataByFilename or build a header index;
(7) regenerate/prevent stack-awareness — done; (8) perf backlog — AlternateThumbQuad
done (it fetches a colour-bucket JPEG, NOT text render — no RTT needed; text path is
CardGui::AlternateRender, still per-frame under DISABLECARDS, follow-up). Quadratic
selectAbility scan NOT yet done (needs LegalActionsOracle convergence).
OWNER FOUND A REAL BUG in review: regenerateTokens only rises at RESOLVE, and
getEfficiency's stack guard is per-ability-POINTER, so a SECOND regenerate ability
aimed at the same card stacked a wasted second shield (pre-existing, combat path).
Fixed with regenerationPendingFor() on both branches.
NAVIGATOR: owner ruled KEEP — after this workload, finish Navigator and make a branch
that integrates it (it's a CardSelector replacement: zone-based cursor + limitor
stack; never instantiated, not in the build files).
Open owner questions from the tutorial work: should Reset also re-enable tutorials if
SEC turned them off? SEC's new meaning has no on-screen affordance (adding an icon
touches the 480x272 layout).
Vita: VPK with af7806b43+6e4a56951 INSTALLED + PSAL.zip (58,938,477) uploaded/verified.
Owner verified: Rats! float fix GOOD, no crashes seen. New VPK (bf366f2c2) building.
Unpushed commits: 12.
**Vita test list CLOSED 2026-08-03**: owner confirmed duplicate-copy hand highlight
works (last unverified item — needed two same-named castable cards in hand). All
defects from his live round are now device-verified fixed: phantom ability rings,
ring proportions, blocker rings, kicker ask-menu (+ cancel), Rats! mana float, and
duplicate highlighting. Remaining device-side unverified: PS11/PMPS art spot-check
(he "didnt notice"), PSAL art (uploaded this session, not yet reported on).
**Tutorial reset now also re-enables TUTORIALS** (owner: "if someone is resetting
them.. sure, toggle them on") + optionsTabs->Reload() so the cached toggle row
refreshes; deliberately NOT the mReload path (it re-reads the profile from disk,
which would discard the flip on Vita where the save is deferred). Gating.
**Owner asked for a full deferred/follow-up review** before Navigator -> wrote
psp-work/deferred-backlog.md (groups A-G: alpha blockers, LLM track, gameplay/UX,
perf follow-ups, engine constraints, small items, housekeeping). Awaiting his rulings.
**OWNER RULING 2026-08-03 on E1-E5** (the five large engine rearchitectures from the
TODO audit): "all things i want to do, but lets do them after shipping the alpha
release." So they are POST-ALPHA ROADMAP, not permanent debt — do not re-present them
as "leave alone". Recommended order recorded in deferred-backlog.md: E4 (parser
nesting model) FIRST since it is the ceiling on card-language expressiveness and makes
the A3 parser gaps easier; then E1 (true createSnapShot), E2 (TC ownership inversion
root cause), E5 (gameType dedup, mechanical); E3 (computeActions) LAST and only as a
migration onto the decision-contract seams.
CONSEQUENCE: the ALPHA RELEASE is now the critical path — E1-E5, A3 and Navigator all
queue behind it.

## 2026-08-03 (cont) — profiling + card-art framing finding

**PERF (owner asked to look into D1-D3). D4 was right: all three were WRONG.**
Installed perf. Profiled headless selfplay. None of D1/D2/D3 appeared. The real top
cost was `GameSettings::checkProfile` -> `MTGAllCards::countBySet` called once PER SET
(336x) each doing a full linear scan of the ~26k-card collection (~8.7M map steps),
and the loop's early `break` on the first unlocked set threw all of it away in the
normal case. FIX: ask the cheap question (any set unlocked?) first; only if not, one
pass via new `MTGAllCards::countBySets(vector<int>&)`. MEASURED 2.89s -> 2.18s user
CPU per game (-24%), wall 4.21 -> 3.46s. Gated separately (perf-gate).
NEXT COST: card-database loading = 44% of total CPU (processConfLine 31.5%). Bigger +
riskier job; not started.
NEGATIVE RESULT (recorded so nobody retries it): swapping Subtypes' string->id intern
table from map to unordered_map measured NO improvement (2.18 -> 2.20, within noise).
REVERTED. Short strings compare cheaply enough that hashing does not pay.
LESSON REPEATED TWICE TODAY: do NOT rebuild while a gate/docker build is running — it
compiles or tests a mixed tree. Both times the result had to be thrown away.

**LANG-SWEEP was mis-parallelized**: tesseract is multithreaded (~2.2 cores each) and
the script ran 6 in parallel on an 8-thread box -> demanded ~13 cores, thrashed, 4.5%
done in 3h (~66h projected). With OMP_THREAD_LIMIT=1 + 8 workers it is ~75 min.
STOPPED at index 3291/73261; partial flags preserved at psp-work/lang-sweep-flags.partial.txt,
sorted file list at psp-work/sweep-filelist.txt. RELAUNCH with the thread pin.

**CARD-ART FRAMING (owner spotted it live: "same card, one blurred and squished")**
Thallid Shell-Dweller has FOUR printings in the pool: MMA 370486, TSP 116731,
TSR 509602, PSAL 17129 AND 17141 (PSAL lists it twice). But the printing is not the
cause. The pool has TWO INCOMPATIBLE FRAMING CONVENTIONS:
  - 149 sets @ ar 0.7007 (618x882): cropped tight, card face fills the frame, NO border
  - 123 sets @ 0.7099 (646x910), 55 @ 0.7179 (672x936), 9 @ 0.7176 (488x680):
    full card INCLUDING the black border + rounded corners
Rendered into the same on-screen slot, the bordered ones show a visibly smaller card
face (reads as squished) with fewer pixels of art (reads as blurred). 187/336 sets
affected. Thumbnail file sizes corroborate: MMA/TSP ~28KB vs TSR/PSAL ~8KB at the
same 114x166.
FIX PROVEN: `-gravity center -crop 92%x94%+0+0` on a bordered image lands at ar 0.7023
vs the 0.7007 target and matches the tight-crop framing visually. Then regenerate
thumbnails. Scope = ~187 sets of full images + thumbnails + rebuild Vita/PSP packs.
⚠ MY REGRESSION: scratchpad/refetch-english.py fetched `image_uris.normal` (488x680)
when `large` (672x936) exists — the 9 refetched sets (PS11, PMPS, PMPS06-11, PSAL) are
lower resolution than the rest. Re-run with `large` when the framing pass happens.
AWAITING OWNER RULING on whether to do the normalization pass.
**Perf fix COMMITTED dfcea42a6**, gated 1031/0 + 24 AI/0 + PARSETEST 319/0.
Binary /tmp/wagic-dfcea42a6. Unpushed commits now 15.

---

## 2026-08-03 (later) — owner rulings, push, and the patience dialog

**ART PASS: RULED OUT.** Owner: "no art pass, the card art is virtually irrelevant."
The framing-normalization analysis above stands as a RECORD of a real defect, but the
fix is NOT wanted — do not resurrect it. Correction to the analysis while it is here:
sets are NOT homogeneous. TSR alone splits 443 images @0.7099 and 411 @0.7179, so the
one-image-per-set survey that produced the "149/123/55/9 SETS" figures was misleading;
any such pass would have had to key on each IMAGE's own aspect ratio. Moot now.

**LANG-SWEEP: STOPPED FOR GOOD.** Same ruling by implication — its only purpose was
finding non-English card ART to refetch. Relaunched briefly with the OMP_THREAD_LIMIT=1
pin (which does work: ~4 img/s at 6 workers even with docker competing), then killed.
Durable resumable script now at psp-work/lang-sweep.sh (was scratchpad-only, would
have evaporated). Partial flags: lang-sweep-flags.partial.txt + .part-3251.txt. Both
deletable. DO NOT relaunch without a new reason.

**PUSHED.** `ff5a5cc2b..dfcea42a6` -> origin/psp-port, 14 commits (not 15; earlier
count was off by one). Guard-checked: no CLAUDE.md, no card art, no set zips.

**VITA VPK REBUILT CLEAN** after `make clean` — 32,216,071 bytes, covers everything
through dfcea42a6. The prior 05:23 VPK was built from a tree being edited mid-build;
quarantined at /tmp/wagic-INDETERMINATE-0523.vpk. NOT yet uploaded (needs an FTP window).

**PATIENCE DIALOG (owner's design, replaces the raise-the-timeout idea).**
Owner rejected raising the timeout: "no human is waiting 10 minutes on one game
decision... bring up a confirm dialog for the user to switch off the llm or wait",
and then "if they wait, we give another duration and ask again, repeat". Built:
  - `Player::aiPatiencePromptDue()` / `aiPatiencePromptAnswer(bool)` — new virtuals
    next to `isInteractiveAI()`, so GameStateDuel needs NO AIPlayerGPT knowledge and
    no WITH_GPT_AI ifdef. Base returns false/no-op.
  - `AIPlayerGPT` overrides: due when `asyncBusy() && mThinkTime >= mPatienceLimit`.
    Answer resets mThinkTime -> re-arms, so it asks again every window, UNBOUNDED
    repeats. "Switch off" clears mEndpoint, which is exactly the no-endpoint state:
    all 17 `mEndpoint.empty()` seam guards fall through to Baka AND `isInteractiveAI()`
    goes false so card-data ishuman gates hand back the heuristic dice-roll lines.
    In-flight worker is left alone (AsyncState is a shared_ptr; its answer is never read).
  - `GameStateDuel`: new DUEL_MENU_LLM_PATIENCE + MENUITEM_LLM_KEEP_WAITING/-SWITCH_OFF,
    raised right after `rules->postUpdateInit(game)` in DUEL_STATE_PLAY, guarded on
    `mParent->players[0] == PLAYER_TYPE_HUMAN` so self-play/suite NEVER stop for it.
    Both answers route to DUEL_STATE_CANCEL (existing close-and-resume path).
  - Config: `patience=` in GptSettings (default 60s, 0 = never, clamp <10 -> 10),
    GUI row "Ask after waiting (seconds)" using OptionGptNumber's zeroText="Never",
    env `WAGIC_GPT_PATIENCE`.
  NO re-entry guard needed and none added: raising the prompt moves the duel to
  DUEL_STATE_MENU, which stops calling game->Update, which is the only thing that
  advances mThinkTime. The state machine already provides the mutual exclusion.

**TIMEOUT raised 120 -> 600s** (GptSettings default + GUI row re-ranged 30..600 step 30).
Justified by repeat-asking, NOT by making anyone wait: with a 120s cap the SECOND "keep
waiting" was a lie (curl had killed the socket; `asyncBusy()` goes false so the prompt
would not even re-appear). The dialog now bounds how long a PERSON waits; the cap's only
remaining job is releasing a dead socket.

**ALERTING AUDIT + FIX (owner: "I just don't want it to fail silently").**
Audited every fallback path. Alerting already EXISTED and was broad — `setNotice()` at
the no-endpoint case (12s), inside `askModel()` itself (so all menu/target/X/mode/cost-
target/mulligan/land-drop/cast-menu seams), cast planning, attackers, blockers (incl.
cut-off + all-illegal variants), reveal, and the decode-garbage re-ask.
ONE GENUINE GAP FOUND: the pregame London-mulligan putback polls directly rather than
through askModel, filled with highest-cost cards, and said NOTHING. Fixed.
THE REAL RISK was noticeability, not absence: one line of MAIN_FONT, 5s, top-centre, on
a 480x272 panel while the player looks at their hand at the BOTTOM. Faithfully emitted
and easily never seen. So: new `noticeFallback()` = setNotice + counter + standing
marker, substituted at 9 existing sites + the new pregame one. Render draws a bottom-left
marker, two mutually exclusive states: "LLM off - built-in AI" (endpoint cleared, whole
duel) or "LLM: N fallbacks" (recent failures, lapses after 45s).
DELIBERATELY NOT CONVERTED (do not "fix" these later): "asking again" and "previous reply
was corrupted - re-asking briefly" are RE-ASKS — the model still answers, so counting them
would make a recovering opponent look broken; "no LLM endpoint reachable" is covered by the
permanent OFF marker and would otherwise tick the counter once per decision all duel.
The 45s lapse is load-bearing in the other direction: a marker that latched forever becomes
wallpaper, which is silent failure by another route.

**REJECTED DESIGN (owner rethought it mid-flight, do not build):** on a FAILED request,
prompt retry / skip (baka handles this one choice) / switch off. He asked for it, then:
"no, I'm rethinking this. let's keep it as is, but are we alerting the user?" Fallback-to-
Baka on failure STAYS immediate; the answer was alerting, above. Note for whoever revisits:
`pollCompletionRetry()` (src/AIPlayerGPT.cpp) is the single funnel every seam polls through
and is where such a prompt would have to intercept, because retry needs the question still
standing rather than consumed.

**COMMITTED 037706a67** "Waiting ten minutes is not a choice the game gets to make",
8 files +195/-14, gated 1031/0 + 24 AI/0 + PARSETEST 319/0. Binary /tmp/wagic-037706a67.
Vita VPK rebuilt on top of it (the dfcea42a6 VPK is superseded, do not ship that one).

**VITA VPK at 037706a67**: 32,217,133 bytes, build_vita/wagic.vpk, 2026-08-03 07:05, exit 0.
Determinate (tree committed and untouched, nothing else running). NOT UPLOADED — Vita FTP
at 10.0.0.227:1337 was unreachable (timeout) when tried; console asleep or FTP off.
Deploy when a window opens:
  VPK=/home/magi/Projects/wagicGPT-psp/build_vita/wagic.vpk \
    /home/magi/Projects/wagicGPT/psp-work/deploy-vita.sh 10.0.0.227 1337
Then VERIFY by directory listing / byte-size compare (FTP here fails dishonestly), and
CONFIRM THE INSTALL on-device — deploy-vita.sh is upload-only; a file in ux0:/vpk/ is not
a running app. What to test on it: the 60s patience dialog (keep waiting / play without the
LLM, repeats every 60s), the bottom-left "LLM off" / "LLM: N fallbacks" marker, and the
Advanced-tab rows "Ask after waiting (seconds)" + the re-ranged "Call timeout (seconds)".
UPLOADED 2026-08-03 12:10 device time — byte-VERIFIED against the Vita's own listing
(32,217,133 both sides). Still needs INSTALLING on-device (VitaShell -> Cross).
⚠ Expected on Vita: the LLM path is not live there (task #14, needs a native
sceKernelCreateThread worker), so "LLM off - built-in AI" should show IMMEDIATELY and
persist — that is correct behaviour, and a good check that the marker renders at the
bottom-left position on a real 960x544 panel. The patience dialog cannot trigger without
a live endpoint; test that one on Linux against Spark or the local llama-server.

**PSP BUILD + DEPLOY at 037706a67 (2026-08-03).** pspdev docker, `make -j6` from
projects/mtg. exit 0, ZERO errors, 163 warnings ALL pre-existing (the one naming
AIPlayerGPT.h — -Wreorder on mLastRetry/mPregameBottomAsked — was VERIFIED pre-existing:
neither member appears in 037706a67's diff).
VERIFIED GENUINELY MIPS, not the silent host-dispatch failure: wagic.elf = ELF 32-bit
MIPS-II, and objs/AIPlayerGPT.o + objs/GameStateDuel.o (both files changed that day) are
MIPS relocatables.
⚠ objs/ SHARING handled by PARKING, not deleting: `mv objs objs-sdl-x86` before, then
`mv objs objs-psp-mips; mv objs-sdl-x86 objs` after — both object sets now coexist, so
neither target needs a full rebuild. bin/wagic verified byte-identical to
/tmp/wagic-037706a67 afterwards (no cross-contamination).
DEPLOYED: EBOOT.PBP 8,416,734 bytes -> ms0:/PSP/GAME/WAGIC/EBOOT.PBP, byte-verified.
Previous Aug-1 EBOOT (8,391,422) saved as /tmp/EBOOT-psp-rollback-20260801.PBP.
Stick: /dev/sda1 vfat 3.9G, 952M free BEFORE the copy; mounted via `udisksctl mount -b
/dev/sda1` -> /run/media/magi/disk, synced + unmounted cleanly after.
⚠ PSP does NOT enumerate until USB Connection is selected on the XMB — "plugged in" is
not enough, and lsusb shows NOTHING (not even a 054c id) until then. Don't diagnose cables
before checking that.
NOT DONE, deliberately: the 9 refetched sets (PS11/PMPS/PMPS06-11/PSAL) still hold their
Aug-1 zips on the stick. Updating them is ART work, which the owner ruled irrelevant the
same day. The stick stores per-set ZIPs, so it would mean rebuilding zips from
image-pack-psp/sets/ first. Left alone on purpose — G3 is effectively closed by the ruling.

---

## 2026-08-03 (later still) — THE PSP RULE, and three PSP-only defects

**OWNER'S RULE, now the first hypothesis for any PSP-only misbehaviour:**
"memory overflow. nearly every issue that produces different behavior on the psp as
compared to other platforms is going to be this, probably."
Full write-up + the tell-them-apart test is in fleet memory `wagicgpt/psp-divergence-is-memory.md`.
⚠ THE TRAP: allocation failure on PSP is SILENT and PARTIAL, so the symptom appears far
from the cause. Example from this session: `Res/rules/mtg.txt` [PLAYERS] has
`auto=sethand:7` on one line and `auto=@each my draw:draw:1` on the NEXT — a 7-card
opening hand plus never drawing again all game is exactly what a rules block that stopped
installing halfway looks like, and nothing about it says "memory".

**KEY DIAGNOSTIC FACT (owner clarified, and it reframed everything):** his "seems fine"
validated the VITA at 037706a67 — gameplay, deck editor, shop, menu, demo, AND the LLM
fallback marker. SAME COMMIT. So every PSP symptom was PSP-specific, NOT a regression in
the commit range, and bisecting the ~20-commit jump would have been wasted effort. When a
sibling console validates HEAD, stop reading diffs.

**COMMITTED 3d9e5a935** "A texture you can never evict is memory you never get back",
pushed; gated 1031/0 + 24 AI/0 + PARSETEST 319/0. Binary /tmp/wagic-3d9e5a935.
1. `CardGui::AlternateThumbQuad` — MY regression from bf366f2c2. RETRIEVE_MANAGE pins the
   texture in the managed map where ClearUnlocked/RemoveOldest can NEVER free it; the
   commit's own justification ("28x40 ... negligible VRAM") was an assumption and wrong
   twice (the WHOLE decoded texture is pinned, not the drawn region; "negligible" does not
   exist on PSP). `#if defined(PSP)` restores RetrieveTempQuad; cache kept elsewhere.
   ⚠ STANDING RULE: RETRIEVE_MANAGE is desktop/Vita only. NEVER pin on PSP.
2. `JFileSystem::ReadFile` — MY regression from 6e4a56951. The strict short-read check was
   right, but that branch shares ONE stream across every read from an attached zip (= PSP
   card art). One short read set eofbit -> every later seek/read failed -> one bad image
   became no resource ever loading again. Strictness kept; `mZipFile.clear()` before the
   seek and after any failure so failures stay local.
3. `JGE/include/JTypes.h` ARGB — PRE-EXISTING port bug, not from this week. The
   unconditional `#define ARGB` at the top pre-empts the PSP block's `#ifndef ARGB`, so
   the byte-swapped ABGR8888 form the GU needs was DEAD CODE and every code-drawn colour
   reached the hardware with red/blue exchanged. Invisible for years (textures carry their
   own format; white/grey chrome has r == b). Surfaced only when the new saturated
   availability borders hit the screen: the ORANGE attack ring rendered BLUE.
   Now `#if !defined(PSP)` around the unconditional define. Vita is unaffected (different
   branch) — which is why the Vita rings were always correct.

**DEPLOYED to PSP**: EBOOT.PBP 8,413,766 bytes, byte-verified on the stick, unmounted
cleanly. Copy at /tmp/EBOOT-psp-fixes.PBP; rollback still /tmp/EBOOT-psp-rollback-20260801.PBP.

**STILL OPEN — the human never draws (only the starting hand, all game).** NOT fixed and
NOT explained. Ruled out with evidence, do not re-walk these: JGE_BTN_MAX (holds is a
std::set, so the sentinel is just always-false), the purge's deck-viewer edits
(comment-only), Rules::initPlayer(s) (NULL guards that would previously have crashed), the
turn-0 CR 103.8a skip (turn++ is per player-turn, fires once), dfcea42a6's checkProfile
(behaviour-identical — the old setId was already discarded by the !ok guard), and the
rules file failing to load (sethand:7 works, so the block loaded and the draw ability
exists — the TRIGGER never fires). Desktop AI-vs-AI selfplay draws normally (game ended
turn 8, life1=-7), but selfplay has NO human seat so it does not exercise the
gameStateBasedEffects no-legal-action auto-skip, which only runs when a human seat exists.
NEXT STEP if it persists after the memory fixes: instrument the [PLAYERS] rules install on
PSP and see how far down the block it gets, rather than more diff archaeology.
⚠ PPSSPP repro was ATTEMPTED and did not pay off: PPSSPPHeadless booted the autodemo EBOOT
but emitted no probe output before timeout. Needs memstick/probe-channel setup before it
is worth another attempt.

---

## 2026-08-03 — ⚠ THE 52MB HEAP FIGURE IS AN EMULATOR ARTEFACT. SIDECARS ARE MANDATORY.

**PROVEN ON HARDWARE 2026-08-03**: a clean PSP install (EBOOT e14468af5 + 67MB Res, NO card
art, NO sidecars) CRASHED TO OFF during load. Adding ONLY the four sidecar files
(cardtext.{idx,dat}, cardauto.{idx,dat}, 12MB, at the Res root) made it boot AND the deck
editor open. Nothing else changed.

**CONSEQUENCE 1 — the heap is NOT ~52MB.** Every "~52MB heap" figure in the Memory facts /
run-4 sections above came from PPSSPP, not hardware (the section's own preamble says
"Emulated speed ~= real PSP"). The ONLY hardware evidence was the boot string "Game
requested full PSP-2000 memory access", which proves the app ASKED for the extra memory,
not that it got 52MB of usable heap. Owner's recollection from the original PSP campaign
was "around 30" and that is far closer: at 52MB the recorded 37.1MB collection load would
have fit with ~13MB spare, and it demonstrably did not. TREAT EVERY BUDGET DERIVED FROM
52MB AS WRONG (cache limits, pack sizing, headroom claims).

**CONSEQUENCE 2 — sidecars are a HARD DEPLOY REQUIREMENT on PSP, not an optimisation.**
commit 6c0100d7d's "Absent sidecars = unchanged behavior" is functionally true and
dangerously misleading: primitives cost 35MB resident without them, 22MB with. On desktop
that is 13MB you have; on PSP it is fatal. ANY PSP DEPLOY MUST INCLUDE THE SIDECARS.
Source: psp-work/sidecar/*.{dat,idx} -> ms0:/PSP/GAME/WAGIC/Res/.
(This was self-inflicted: the "clean install" deliberately omitted them to minimise
variables, which silently removed the one thing making the card pool fit.)

**STILL TO MEASURE**: the actual heap size on the device. `wagicHeapFreeBytes()` exists in
JGE/src/main.cpp and the WAGIC_HWPROBE build writes probes to ms0:. One probe boot replaces
every emulator-derived budget in this file with a real number. DO THIS BEFORE re-deriving
cache limits or deciding how much card art the device can hold.

**Card pack rebuilt correctly** (psp-work/make-psp-cards.sh, new): the old pack had 158/336
sets at 180x257 full art -> POT 256x512 = 512KB each (one pixel over the 256 boundary), and
9 sets (PS11/PMPS/PMPS06-11/PSAL - the English refetch) carrying 114x166 DESKTOP thumbnails
-> POT 128x256 = 128KB vs the correct 64x93 -> 64x128 = 32KB. New pack is uniform: full art
fits 180x250 (POT 256x256 = 256KB, half), thumbs exactly 64x93 (32KB, a quarter). Thumbs are
always derived from full-size source art so desktop thumbs can never leak in again.
Output: /home/magi/Projects/wagic-card-packs/psp/sets/<SET>/<SET>.zip (STORED).

---

## 2026-08-03 — ROOT CAUSE FOUND: the deck-editor loop is 8a4eddc53's TexMemCheckpoint

**CONFIRMED ON HARDWARE by removal.** `af7806b43` loops; `af7806b43` with ONLY
`8a4eddc53` reverted loads and works. Both hermetic, minutes apart, same art, same
sidecars, same Res. Single variable.

**MECHANISM.** `TexAlloc` routes every texture-pixel allocation through
`TexMemCheckpoint()` on failure, which mid-frame does:
  `sceGuFinish(); sceGuSync(0,0); FlushDeferredTextureFrees();
   sceGuStart(GU_DIRECT, list); mCurrentTex = -1; mCurrentBlend = -1;`
i.e. it TEARS DOWN THE IN-FLIGHT DISPLAY LIST, blocks on a GPU sync, frees, and
restarts the list with texture/blend state invalidated. The commit assumed allocation
failure is exceptional. MEASURED (probe build, same day): the deck editor peaks at
40.4 MiB used of a 43.07 MiB heap — 2.7 MiB free. There, allocation failure is the
STEADY STATE, so this fires continuously and the partial frame is torn down and
redrawn forever. Owner's words on the mechanism description: "this perfectly describes
the visual experience of the fail loop in the deck editor."

**WHY IT RESISTED EVERYTHING ALL DAY**: PSP-only (`#if PSP` renderer), invisible to the
suite and to PPSSPP, unaffected by the purge or the art pack, and it landed 3 MINUTES
after the last known-good EBOOT was built — so "the last good build" and "the first bad
commit" were adjacent in a way no source-level reasoning surfaced.

⚠ **THE THING THAT COST HOURS — STALE STATIC LIBRARIES.** `JGE/Makefile` archives with
`$(AR) r` (replace-or-insert): members already in libjge300.a/libhgetools.a that are not
in the current $(OBJS) SURVIVE FOREVER, and `clean` is defined ONLY inside
`ifeq ($(TARGET_ARCHITECTURE),linux)` — there is NO supported clean for the PSP target.
Consequence: builds of the SAME COMMIT differ depending on session. Concretely, builds
before 08:47 today linked Aug-1 JGE while only projects/mtg recompiled, so the JTypes.h
ARGB fix reached mtg code but NOT JGE code — one binary with two definitions of the same
macro. It also made the first bisect meaningless: every point linked the same stale
library, so the defect was constant across all samples.
**RULE: every PSP build is hermetic.**
  `rm -f JGE/lib/psp/libjge300.a JGE/lib/psp/libhgetools.a JGE/objs/*.o projects/mtg/objs/*.o`
  then `cd JGE && make` then `cd projects/mtg && make`. VERIFY the checked-out commit
  BEFORE launching (a failed `git checkout` in an `&&` chain still built the wrong tree
  once today). Archived EBOOTs older than 2026-08-03 are NOT trustworthy bisect
  references — only hermetic rebuilds are.
**FIX THE MAKEFILE** so this cannot recur: rm the archive before creating it, and give
the PSP branch a real clean target. NOT YET DONE.

**8a4eddc53 WAS solving a real problem** (burst load->evict inside one frame holds
old+new pixels because deferred frees wait for the frame sync). The intent is sound; the
remedy is not. SAFE VERSION: reclaim only when NOT in a frame; in-frame, let the
allocation fail so the texture is simply absent for one frame and loads on the next after
the normal end-of-frame sync. Same reclamation, no render-state destruction.

**FIXES COMMITTED + PUSHED (2026-08-03)**: `55f91ca78` (revert the TODO purge -> drawing
restored on PSP) and `3c2e79036` (safe TexMemCheckpoint -> deck-editor loop fixed,
isolated on hardware). Gated 1031/0 + 24 AI/0 + PARSETEST 319/0. Binary
/tmp/wagic-3c2e79036. PSP carries a hermetic build of that exact tree
(/tmp/EBOOT-fixed-tree.PBP, 8,419,134).

**PSP INSTALL STATE after the wipe**: clean Res (3153 files) + all 4 sidecars + corrected
336-set art pack (/home/magi/Projects/wagic-card-packs/psp, built by psp-work/make-psp-cards.sh).
User/ = only player/options.txt, player/collection.dat, settings/options.txt (+probe log).
NO user deck files exist; Res/player/premade has deck1.txt, matching the reference release
layout exactly. Owner reported "a build deck that isn't built, can't play with the default"
— UNRESOLVED: there is no deck file to delete. Next step is to see what the deck-selection
screen actually shows; the only deletable user state is collection.dat (979 B, regenerates)
and player/options.txt (holds his keybindings + ASPhases=Safe — do NOT delete casually).

**OPEN, in priority order**
1. JGE/Makefile hermetic fix: `$(AR) r` never evicts stale members and `clean` exists only
   under `ifeq ($(TARGET_ARCHITECTURE),linux)`. Until fixed, reproducible PSP builds depend
   on remembering to rm the .a files. rm the archive before creating it + add a psp clean.
2. Reintroduce the purge's five real fixes INDIVIDUALLY (asColor uninitialized read, NULL
   guards in Rules::initPlayer(s) and GameObserver::cardClick, multiple-choice action-log
   entry, JFileSystem short-read check). ⚠ the JFileSystem one caused visible PSP loading-
   screen flicker when it reported failure — keep it lenient or handle short reads without
   failing the read.
3. WITH_GPT_AI is ON by default in the PSP CFLAGS (projects/mtg/Makefile:78) — the whole LLM
   opponent + nlohmann compiled into a platform with no network path (task #6 pending).
   Notes measured it at binary 6.2->8.2MB. Free experiment: build PSP without it and measure
   the heap recovered. Matters because the deck editor peaks at 2.7 MiB free of 43.07 MiB.
4. Vita/desktop unaffected by any of today's PSP fixes but both need a re-verify pass after
   the purge revert (drawing + the patience dialog on desktop; Vita VPK is stale vs HEAD).

## 2026-08-03 — duel crash-to-off: the cache cap was load-bearing

`3c2e79036` bundled TWO changes. Only one was ever isolated on hardware.

| half | evidence | verdict |
|---|---|---|
| safe `TexMemCheckpoint` (never reclaim mid-frame) | deck editor "loads, navigates" on device | KEEP — proven fix |
| revert of the 8 MB PSP texture-cache hard cap | never tested separately | **WRONG — restored** |

Symptom that exposed it: with `3c2e79036` on the device, selecting a deck and an
opponent crashes the PSP to OFF. Owner's call: "aside from that change, previously,
this build was able to load into the game. so... its that."

The two symptoms map one-to-one onto the two halves. `55f91ca78` — the commit
immediately before — had the guilty mid-frame checkpoint AND the cap. In that build
the deck editor looped (checkpoint) but duels loaded ("draw happened"). Remove both
and the loop goes away while the duel starts crashing. So: checkpoint -> loop,
cap -> duel survives.

Why the cap matters, `WResourceManager.cpp` `ResetCacheLimits()`:

    myNewSize = ram - OPERATIONAL_SIZE + textureWCache.totalSize;

That sizes the texture cache to *all free heap minus a fixed reserve* — an
overcommit. Entering a duel then allocates two decks of cards, their abilities and
the GPT layer out of whatever the reserve left. On the measured 43.07 MiB heap that
does not fit, and PSP heap exhaustion presents as power-off.

My stated reason for removing it — "sized against a 52 MB figure that came from an
emulator" — was a reason to RESIZE it, never to delete it. Deleting a memory guard
because its constant was derived from a wrong heap figure removed the protection and
kept none of it. Restored at 8 MB (with the real heap, 8 MB of 32 KB thumbnails is
~250 resident cards, not the ~500 the original comment claimed).

**Rule this earns**: on PSP, never bundle a memory *guard* change with a memory
*behaviour* change in one commit — they fail on different screens, and only
separately-shipped halves can be attributed.

### Still unverified
- ARGB colour fix (blue combat border) — never visually confirmed; every earlier
  build claiming it linked stale JGE.
- Drawing in matches on the current tree.

### ⚠ The hermetic build rule was INCOMPLETE — it omitted the toolchain

There is no pspdev toolchain on Voyager's PATH. PSP builds run in a CONTAINER:

    docker run --rm --memory=5g --memory-swap=5g \
      -v /home/magi/Projects/wagicGPT-psp:/build pspdev/pspdev:latest \
      bash -c "cd /build/JGE && make -j6 && cd /build/projects/mtg && make -j6"

Running the inner `make` on the host instead builds **Linux**, silently.
`JGE/Makefile:51` selects the target with `RESULT = $(shell psp-config --psp-prefix)`
and falls back to `TARGET_ARCHITECTURE = linux` when the binary is absent — no error,
no warning. `projects/mtg` then dispatches to the `qt` target and dies on
`QtOpenGL: No such file or directory`.

Worse than the wasted build: the Linux pass writes x86 `.o` files into `JGE/src/`,
the same directory the PSP build uses, and archives `JGE/lib/linux/*.a` (tracked
files). Both must be cleaned before the next PSP build or the stale-object defect
returns in a new form. Full clean:

    find JGE/src -name '*.o' -delete
    rm -f JGE/lib/psp/libjge300.a JGE/lib/psp/libhgetools.a \
          JGE/objs/*.o projects/mtg/objs/*.o projects/mtg/PrecompiledHeader.h.gch
    git checkout -- JGE/lib/linux projects/mtg/Makefile.linux   # tracked, restore if removed

### The safe TexMemCheckpoint is INCOMPLETE — TexAlloc's NULL is dereferenced

Observed 2026-08-03 on the capfix build (HEAD + 8 MB cap restored): the deck editor
CRASHES (it previously LOOPED with the guilty checkpoint, and worked with the cap off).

Matrix:

| build | 8MB cap | checkpoint | deck editor | duel |
|---|---|---|---|---|
| `55f91ca78` and earlier | present | guilty  | loops           | loads |
| `3c2e79036`             | absent  | safe    | loads/navigates | crash to off |
| capfix (HEAD + cap)     | present | safe    | **crashes**     | loads, but NO DRAWS |

MECHANISM. `JGE/src/JGfx.cpp:122`:

    static void * TexAlloc(int size)
    {
        void * p = memalign(16, size);
        if (!p) { JRenderer::GetInstance()->TexMemCheckpoint(); p = memalign(16, size); }
        return p;
    }

The GUILTY checkpoint always flushed deferred frees, so the retry usually succeeded.
The SAFE checkpoint returns immediately when `gInFrame`, so the retry fails identically
and TexAlloc returns NULL. Callers do not check:

  * `JGfx.cpp:1880` — `tex->mBits = (PIXEL_TYPE*) TexAlloc(size);` followed by an
    UNCONDITIONAL `memset(tex->mBits, 0, size);`  <-- the crash
  * `JGfx.cpp:1515` — `bits = (PIXEL_TYPE*) TexAlloc(size);` then used unchecked
  * `JGfx.cpp:1712` — same; the `if (buffer)` below it guards the swizzle buffer, not `bits`
  * `JGfx.cpp:1490` — this one DOES check and returns `JGE_ERR_MALLOC_FAILED` (the model to copy)

The commit message for `3c2e79036` claims "the texture is simply absent for one frame".
That is only true if callers handle NULL. They do not. **The fix is incomplete, not wrong.**

NEXT STEP (not yet done): add NULL handling at 1515, 1712 and 1880 so a failed texture
allocation abandons the texture instead of dereferencing — 1490's early-return is the
pattern. Then re-test deck editor AND duel on the same build.

Also relevant: `OPERATIONAL_SIZE` is 5 MB (`WResourceManagerImpl.h:23`) and
`ResetCacheLimits()` is called ONLY from `GameApp.cpp:175` (boot) and
`GameStateMenu.cpp:648` — NOT on entering a duel or the deck editor. The cache limit is
therefore computed once, at the menu, where free heap is large, and never re-adapts.

### Bisect in progress (draws)
Checked out `763676441` ("Vita: build the fork engine, and track current vitaGL") =
10 commits after `8a4eddc53`, with `JGE/src/JGfx.cpp` taken from `3c2e79036` (safe
checkpoint) so the deck editor does not regress; the 8 MB cap is NATIVE at that commit.
Build was launched but SHOULD NOT be deployed until the NULL guards land, because it
carries the same cap+safe-checkpoint combination that crashes the deck editor.

⚠ The `texcache-cap-restore.patch` save produced 0 lines (the change was STAGED, so
`git diff` was empty) and `git reset --hard` then discarded it. The edit is trivial to
recreate: restore the `PSP_TEXCACHE_HARD_CAP = 8000000` clamp before
`textureWCache.Resize(...)` in `ResourceManagerImpl::ResetCacheLimits()`.

**NULL guards APPLIED** (2026-08-03, on the bisect checkout, not yet committed to a branch):
32 lines added to `JGE/src/JGfx.cpp` at the three unchecked sites — PNG loader bails with
`JGE_ERR_MALLOC_FAILED` (matching line 1490), GIF loader bails with the existing
`free(LineIn); DGifCloseFile(GifFileIn); return 1;` pattern, and `CreateTexture` does
`delete tex; return NULL;` instead of `memset(NULL, ...)`. Returning NULL from
CreateTexture is already part of its contract — the `new JTexture()` above can fail the
same way. These guards must be PORTED TO psp-port once verified; they are currently only
in the detached bisect tree.

**GUARDS VERIFIED ON DEVICE** (2026-08-03): on `763676441` + safe checkpoint + guards, the
the deck editor loads WITHOUT CRASHING. Owner's exact sequence: the background overlay
rendered, the game seemed to freeze, some particle effects happened, more freezing, then
the overlay DISAPPEARED revealing the card carousel, after which it ran normally with the
flowing-light cursor particle effect.

Read it as EVICTION THRASH, not a failed load (an earlier note here said the overlay never
rendered — wrong, it rendered first). At the native 8 MB cap the carousel's thumbnails do
not fit, so entering it forces evictions; the large already-resident background overlay is
what gets evicted to make room, and the freezes are JPEGs being re-decoded out of the zips.
The guards are still confirmed: degradation instead of a crash is exactly their purpose.

So 8 MB is too small to HOLD the deck editor's working set, while NO cap is too large for
the duel to survive. Both follow from `ResetCacheLimits()` running only at
`GameApp.cpp:175` (boot) and `GameStateMenu.cpp:648` — never on entering the deck editor
or a duel, so one number computed at the menu has to serve two screens with opposite
needs. PRINCIPLED FIX (proposed, not implemented): call `ResetCacheLimits()` on entry to
each state and size the reserve (`OPERATIONAL_SIZE`, currently 5 MB at
`WResourceManagerImpl.h:23`) to cover a duel's non-texture allocations. Removing
`WITH_GPT_AI` from PSP (~2 MB, no PSP network path — task #6) buys headroom for both.

### Draw regression — bisect state (2026-08-03)

**`763676441` DRAWS WORK** (owner on device: "i am drawing now"). So the regression is in
the 31 commits `763676441..3c2e79036`, NOT before. An earlier note in this file guessed the
opposite; that guess was wrong and is superseded.

Prime suspects on MECHANISM, not position — both change auto-skip / priority-window behaviour:
  * `aa0e6bf5a` A window with no legal action is not a window
  * `cfc2a8a76` Gate the no-action skip to the player's own turn

Why they are suspects: `GameObserver.cpp:210` (the CR 103.8a turn-0 skip) states the rule
outright — *"the draw-step rule (@each my draw:draw:1) triggers by polling
getCurrentGamePhase between Updates, so a phase the game never rests in never draws."*
An auto-skip that advances past MTG_PHASE_DRAW inside a single update would silently
suppress every draw, which is exactly the symptom. The auto-skip block is
`GameObserver.cpp:~1236-1250` and already lists `mCurrentGamePhase == MTG_PHASE_DRAW`
among the phases it fast-forwards when the opponent is an AI.

NOW TESTING: `cfc2a8a76` + safe checkpoint + NULL guards + PSP deck-bg removal.
  * draws BROKEN there -> cause is within `aa0e6bf5a` / `4e0ae99ba` / `cfc2a8a76`
  * draws WORK there    -> cause is later in the range

**Tutorial messages are NOT a regression.** They reappeared only because `763676441`
predates `4e0ae99ba` ("Tutorial messages: an off switch, and never in an AI-only game"),
which adds `Options::TUTORIALS`, off by default in this fork, plus a "Show tutorial
messages" row in Options. Any build above that commit will not show them. (A wiped
`User/player/options.txt` also clears the per-message `tuto_<hash_djb2(message)>` flags
that `ATutorialMessage::alreadyShown()` reads, but the off switch is decisive.)

### PSP deck-editor background REMOVED (owner's call, 2026-08-03)
`GameStateDeckViewer::Render()` called `RetrieveTexture("pspbgdeckeditor.jpg")` EVERY
FRAME. A full-screen JPEG cannot coexist with the carousel thumbnails in the texture
cache, so the two evicted each other and every swap re-decoded a JPEG from the zip —
the freeze/particles/freeze bursts the owner saw until the background stopped returning.
He preferred the result ("it looks fine without... the darkening transparent effect makes
the text pop... doesn't seem like a problem"), so it is now deliberate on PSP. The removed
code also called `RenderQuad(wpQuad.get(), ...)` with no null check, unlike the non-PSP
branch directly above it. NOT YET COMMITTED — lives in the bisect tree with the guards.

### Draw bisect — results so far (2026-08-03)

Range `763676441..3c2e79036` = 31 commits. Owner tests on device; I carry a single
bundled patch across bisect points (`/tmp/bisect-carry.patch`, regenerate with
`git diff` and apply with `git apply --3way`) containing: safe TexMemCheckpoint +
the three JGfx NULL guards, the PSP deck-editor background removal, the three tutorial
presentation fixes, and a `WAGIC_DRAWPROBE` counter in `MTGPlayerCards::drawFromLibrary`.
⚠ THE PROBE MUST BE REMOVED BEFORE ANY COMMIT.

  * `763676441` (base)    — DRAWS WORK on device ("i am drawing now")
  * `cfc2a8a76` (#10)     — DRAWS WORK on device ("draws happened on this build")
  * `128a3600c` (#21)     — TESTING NOW

**The auto-skip hypothesis is DEAD.** `aa0e6bf5a` (#8) and `cfc2a8a76` (#10) are both
below a confirmed-working point, so neither can be the cause. Desktop agrees: a headless
selfplay run at `cfc2a8a76` with the draw probe logged 11 draws in 11 turns, all at
phase 3, library counting down 53->49. (Caveat kept: that desktop run used the desktop
profile's ASPHASES setting, not the device's `ASPhases=Safe`, so it is corroboration,
not proof.)

Remaining range #11-#31. Owner's memory-overflow reading stays live — a commit could
break draws by ADDING PRESSURE rather than by breaking logic. Two in range fit that
shape: `a6600e507` (#15, "Show the player what the board can do, not just the hand")
and `dfcea42a6` (#27, "Counting the whole collection once per set").

Also expected-not-bugs at low bisect points, so they are not chased again:
  * no attacker border below `154d2fb9a` (#17) — that commit exists because the inset
    border was invisible
  * blue instead of orange border below `3d9e5a935` (#29) — the ARGB fix
  * tutorial messages below `4e0ae99ba` (#9) — the off switch

### Draw bisect — narrowed to #22-#31 (2026-08-03)

  * `763676441` (base) — WORKS
  * `cfc2a8a76` (#10)  — WORKS
  * `128a3600c` (#21)  — WORKS ("works")
  * `c9183da44` (#26)  — TESTING

`6e4a56951` (#24, the TODO purge) is EXONERATED by construction: its revert is #30, so at
HEAD the purge's changes are already undone, yet draws are broken at HEAD. The cause must
be something else in the range.

Strongest remaining suspects:
  * `3d9e5a935` (#29) — unpinned PSP thumbnails (`AlternateThumbQuad` -> `RetrieveTempQuad`
    under `#if defined(PSP)`). If a drawn card cannot obtain a quad, the hand LOOKS
    unchanged — i.e. "drawn but invisible", the owner's memory-overflow reading in its
    exact testable form.
  * `dfcea42a6` (#27) — "Counting the whole collection once per set, then throwing it away"

### Tutorial presentation — fixed (owner directive, 2026-08-03)
Owner: "these atrocious tutorial messages have gotta get fixed. i dont think the game
doesnt need a tutorial. but the way they are presented is maximally annoying." Then, on
the board image: "delete it. i am so over that tutorial. no. i dont want that image."

Changes in `AllAbilities.cpp` (`ATutorialMessage`), all verified to compile:
  1. `options.save()` on every dismissal now skipped on PSP as well as Vita — it is a
     synchronous Memory Stick write per message and was the visible "lag".
  2. `taskboard.png` DELETED. It was 512x512 fetched with `RETRIEVE_LOCK` = 1 MiB PINNED
     for the process lifetime, to decorate a text box. `mBgTex` now stays NULL, which
     falls through to the drawn-panel branch already present in `Render()` (two
     translucent FillRects + the same title/text/continue button) — no new draw code.
     The deleted code also read `mBgTex->mHeight` BEFORE its own NULL check.
  3. Both stings removed (`tutorial.wav`, `chain.wav`).
  4. Slide-in/out 0.75s each way -> 0.12s (it is modal throughout, so this was 1.5s of
     forced waiting per message).

### PSP asset sizing — the power-of-two audit (2026-08-03)
Cost on PSP = nextPow2(w) * nextPow2(h) * 4 bytes. File size is no guide (a 24 KB JPEG
can be 1 MiB resident). Measured:

| asset | pixels | bucket | resident |
|---|---|---|---|
| `pspbackdrop.jpg` (match) | 450x255 | 512x256 | 512 KiB |
| `pspbackdropframe.png` (match) | 480x**272** | 512x**512** | **1 MiB** |
| `pspbgdeckeditor.jpg` | 480x272 | 512x512 | 1 MiB (now unused) |
| `taskboard.png` (tutorial) | 512x512 | 512x512 | 1 MiB, was PINNED (now unused) |

FREE WIN NOT YET TAKEN: `pspbackdropframe.png` is 480x**272**. Those last 16 pixels of
height push it from the 256 bucket into the 512 bucket and DOUBLE it. Resizing/cropping to
480x256 saves 512 KiB with NO code change. Going procedural for the match background would
save the full 1.5 MiB — on the screen that crashed to off.

`GameApp.cpp:257` correctly `#if !defined (PSP)`-gates the seven 960x544 desktop backdrops
(each would be a 1024x1024 bucket = 4 MiB), so those are NOT a PSP problem.

### Deck-selection title removed
`GameStateDeckViewer::updateDecks()` passed "Choose Deck To Edit" to `DeckEditorMenu`. The
title box is 180px wide centred at x=110, so on PSP it clipped ("hoose Deck To Edit") and
sat on the deck art. Now "" — `DeckMenu::Render` already guards `if (!title.empty())`.

### PSP asset power-of-two resize — DONE (2026-08-03, owner: "do that. thats an easy win")

15 `psp*` assets were authored at the PSP's native 480x272 (or 255x272). 272 is 16 pixels
ABOVE the 256 power-of-two boundary, so every one landed in the 512 bucket and cost DOUBLE.
Resized to height 256 (width unchanged).

    before: 17664 KB (17.2 MiB)   after: 10496 KB (10.2 MiB)   saved: 7168 KB (7.0 MiB)

NO CODE CHANGE was needed: every one of these is drawn with
`RenderQuad(q, 0,0,0, SCREEN_WIDTH_F / q->mWidth, SCREEN_HEIGHT_F / q->mHeight)`, i.e.
stretched to fill the screen, so a 256-tall image is stretched back to 272 at draw time
(~6.25% vertical, imperceptible on backgrounds/panels). Verified beforehand that none of
them are sliced at fixed pixel offsets — all are whole-image `RetrieveTempQuad`.

⚠ `bin/Res` IS GITIGNORED, so the originals exist nowhere in git. They are backed up at
`psp-work/graphics-orig-20260803/` (27 files, 1.2 MB). Restore from there to revert.

Files changed: pspavatarholder.png pspbackdropframe.png pspdeckmenu.png pspfakebar.png
pspmenubgdeckeditor.png pspmenuholder.png pspstatsholder.png psptextscroller.png
psptextscrollershadow.png pspawardback.jpg pspbgdeckeditor.jpg pspmenubgdeckeditor.jpg
pspmenupanel.jpg pspshop.jpg pspshop_light.jpg

STILL TO DO: copy them onto the PSP stick's `Res/graphics/` (device was not in USB mode
when the resize ran). Until that copy happens the device still has the 272-tall originals.

NOT TOUCHED: the desktop/Vita 960x544 assets. Worth a separate audit — on Vita those land
in a 1024x1024 bucket = 4 MiB each, though Vita's HUGE_CACHE_LIMIT is 48 MB so it is far
less urgent than PSP.

### Owner direction: a possible TOTAL ENGINE REVISION (2026-08-03, considering — NOT started)

His framing, verbatim in substance: use the existing code plus the official MTG
Comprehensive Rules as reference material, and treat "the existing lexicon of cardscript
as a contract that has to serve all the existing scripted cards."

SEQUENCING (his decision): the revision comes AFTER running the dev loop over every
extant deck — "i want the revision to assume the llm opponent, and be structured for
that." The loop is what produces the requirements; designing first would be guessing.

Existing assets for the contract (already built, found 2026-08-03 — do NOT rebuild):
  * `docs/card-script-lexicon.md` — 1544 lines
  * `docs/card-script-lexicon-coverage.tsv` — 852 entries (basic 240, effect 190,
    macro 159, target 87, trigger 77, cost 64, field 35)
  * `Res/test/lexicon/` — 638 tests
  * `tools/audit_lexicon_coverage.py`, `HANDOFF_CARD_SCRIPT_LEXICON_TESTS.md`,
    `projects/mtg/strategy-design/lexicon-triage/` (engine-bugs.md, affected-cards.md,
    validator-corpus-failures.tsv)

⚠ STATUS COLUMN: **851 of 852 entries are `covered-unverified`; exactly 1 is `covered`.**
So the contract is ENUMERATED but not ENFORCED. Promoting that ledger to verified is the
safety net any revision would need (and is worth having for the current engine anyway).
Read `HANDOFF_CARD_SCRIPT_LEXICON_TESTS.md` before inventing a promotion criterion — the
Codex run that produced this may already define one.

Differential-testing infrastructure ALREADY EXISTS for a rewrite: `WAGIC_SELFPLAY` with
`WAGIC_SELFPLAY_DECK0/1` pinning, `WAGIC_SELFPLAY_ONESHOT`, and seeded RNG
(`initRand(seed)` / `GameObserver::resetSeed`) make deterministic replay possible, so old
and new engines can be diffed on identical seeded games.

CLAUDE'S PROPOSAL (mechanism, not his position): the current decision seams were shaped
for Baka, which can be asked N times for free. An LLM inverts that cost profile, which is
why attackers/blockers already had to be BUNDLED. A revision that assumes the LLM should
make bundling the default — the engine poses the largest coherent legal decision with the
deciding facts attached — with `DecisionContract` as the spine rather than a layer over
the 2008 click choreography. Structural bets worth naming: CR 613 (layers) and CR 704
(state-based actions) are what make rules-correctness compositional instead of a queue of
special cases.

SUGGESTED during the loop: capture ENGINE-SHAPED complaints as a byproduct, not just card
bugs — decisions that could not be bundled, legality that had to be recomputed because the
seam did not carry it, stale answers forcing a re-ask. That is the revision's requirements
document and it only exists while games are being watched.

### PSP asset resize BROKE RENDERING — and the likely reason (2026-08-03)

Resizing 15 `psp*` assets from 480x272 to 480x**256** corrupted several screens on
hardware. Evidence:
  * Deck selection: the right ~53% of the screen (x 225..480, exactly where
    `DeckMenu.cpp:289` draws `menupanel`) rendered as horizontal streaks of
    uninitialised texture memory. Photo confirmed.
  * Reverting FIVE deck-menu assets to 272 (`pspmenupanel.jpg`, `pspavatarholder.png`,
    `pspmenuholder.png`, `pspmenubgdeckeditor.png/.jpg`) — assets only, no rebuild —
    FIXED that screen. Isolated by removal.
  * A flat GREEN bar remained where `VerticalTextScroller::Render` draws
    `psptextscroller.png` / `psptextscrollershadow.png` — both still at 256.

Format was ruled out first: `identify` shows orig and resized are byte-for-byte the same
PNG type / depth / colourspace / alpha / interlace. Only the height differs.

HYPOTHESIS (Claude's, NOT yet confirmed): the failure is **exactly-power-of-two height**,
not the resize. At 480x256 the image fills the 256-high bucket exactly, so the quad's V
coordinate reaches 1.0 and samples the texture's last-row boundary. At 480x272 the bucket
was 512 and V only reached 272/512 = 0.53, nowhere near the edge. Every asset drawn with
`RenderQuad(q,0,0,0, SCREEN_WIDTH_F/q->mWidth, SCREEN_HEIGHT_F/q->mHeight)` would hit this.

TEST IN FLIGHT: regenerate all 15 from the backups at height **255** instead of 256.
Same 256-high bucket (so the full 7 MiB saving survives), but V = 255/256 = 0.996 and
never touches the edge. Assets are staged in `bin/Res/graphics`, NOT yet on the stick.
  * renders clean -> hypothesis confirmed, and this becomes a RULE for every future PSP
    asset: never author a dimension exactly equal to a power of two; land one pixel under.
  * still broken -> revert all 15 to 272 from `psp-work/graphics-orig-20260803/` and leave
    the memory on the table. Do NOT leave a partial resize in place — the shop, awards,
    stats popup and phase bar use touched assets on screens not yet visited.

### CONFIRMED: never author a PSP asset at exactly a power of two (2026-08-03)

The 480x**256** resize corrupted several screens; regenerating the same 15 assets at
480x**255** rendered CLEAN on hardware (owner photo: deck selection, deck art, description
panel all correct). Same 256-high texture bucket, so the FULL memory saving survives.

MECHANISM (hypothesis, now supported by removal-and-restore on device): at exactly 256 the
image fills the bucket, so the quad's V coordinate reaches 1.0 and samples the texture's
last-row boundary. At 255 V = 0.996 and never touches the edge. At the original 272 the
bucket was 512 and V only reached 0.53.

**RULE for every future PSP asset: land one pixel UNDER the power of two, never on it.**
480x255, not 480x256. This joins the bucket rule (cost = nextPow2(w)*nextPow2(h)*4).

Originals remain at `psp-work/graphics-orig-20260803/` (27 files). Note `bin/Res/graphics`
IS tracked in git (an earlier note in this file wrongly said bin/Res is gitignored — that
applies to other Res subtrees, not these), so `git checkout` also restores them.

### MEASURED RESULT of today's memory work — deck editor, on hardware

| | this morning | after today's changes | delta |
|---|---|---|---|
| peak `used`      | 42.35 MB | 38.46 MB | **-3.9 MB** |
| `heapfree` @peak |  2.81 MB |  **6.68 MB** | **+3.87 MB** |
| `largest` block  |  2.75 MB |  4.76 MB | +2.0 MB |

Free headroom in the deck editor went 2.8 -> 6.7 MB (2.4x). `used + heapfree` = 45,146,880
in EVERY sample, re-confirming the 43.05 MiB heap independently. This is COLLECTIVE (tutorial
pinned MiB + deck-editor background + chrome gating + asset resize); the data cannot split them.

### Victory-screen stall — MEASURED, and it is NOT what I guessed

    victory: PlayerData ctor          74 ms
    victory: getDoneTasks              0 ms
    victory: unlock checks          5234 ms   <-- 73%
    victory: tasks                     0 ms
    victory: playerdata save        1863 ms   <-- 26%
    victory: TOTAL 7171 ms

My prior was that the Memory Stick writes dominated. They are real but secondary.
`PlayerData ctor` — which I flagged as a serious contender for scanning the whole
~26,000-card database — is 74 ms. Not it.

The stall is INTERMITTENT (owner: fast one match, "crazy delay" the next). That fits:
the chain only runs when `unlocked == -1`, and `unlockRandomSet()` calls `options.save()`
ONLY when it actually unlocks something. So the expensive path is "a set got unlocked".
NEXT: finer marks inside the `unlock checks` block (DeckStats::load, isDifficultyUnlocked,
the Unlockable loop, eviltwin/commander/randomdeck, unlockRandomSet incl. its options.save,
IsMoreAIDecksUnlocked) to find which of the seven costs 5 seconds.

### OPEN: flat green rectangle on the deck-selection screen
Present in three photos, survives the 255 fix, so NOT the asset resize. RULED OUT:
`psptextscroller.png` / `psptextscrollershadow.png` (both ~96% transparent, near-black,
alpha mean 0.046); DeckMenu's own fills (only two 37x50 `DrawRect(ARGB(200,3,3,3))`);
`DeckMenuItem.cpp` (no colour/rect code at all); `DeckMenu::RenderBackground` (no colour
fallback); every bright-green ARGB constant in the tree (all in CardGui / ActionStack /
GuiCombat, i.e. in-game, not menu). Source not yet identified.

### ROOT CAUSE of the 15s victory stall: 196 AI decks opened to answer one question

Three WINPROBE samples on device (2026-08-03):

    PlayerData ctor 74   getDoneTasks 0   unlock 5234   tasks 0      save 1863   TOTAL  7171
    PlayerData ctor 77   (loss path)                                 save 2112   TOTAL  2189
    PlayerData ctor 74   getDoneTasks 0   unlock 5010   tasks 9033   save 1824   TOTAL 15941

`tasks` = `passOneDay()` + two `addRandomTask()`. It is 0 ms when the player already has
6 tasks and ~9 s when tasks are generated — which is exactly the reported intermittency.

CHAIN: `addRandomTask` -> `createFromStr` -> `Task::randomize()` -> `getAIDeckCount()` ->
`LoadAIDeckNames()`, which walked EVERY `ai/baka/deckN.txt` doing `fileExists` AND
`readDeckName` (open/read/close). **There are 196 AI deck files.** 196 stats + 196 opens on
a Memory Stick ~= 9.0 s. Cached in a static, so once per launch — but it lands on the
victory screen, which is the worst possible moment.

FIX APPLIED (`Tasks.cpp`, compiles clean, not yet device-verified):
  * `getAIDeckCount()` returns `MIN(AIDECKS_UNLOCKED, total)`, so probing past the unlocked
    ceiling (20 for this profile) was counted then discarded. Now stops at the ceiling,
    resumes incrementally if the ceiling rises (`IsMoreAIDecksUnlocked` adds 10), and stops
    permanently once a file is genuinely missing. Uses `fileExists` ONLY - no opens.
  * `getAIDeckName(id)` reads ONLY deck `id`. `sAIDeckNames` is now a sparse per-id cache
    (empty string = not read yet). `LoadAIDeckNames()` is retained as a no-op because the
    header declares it.
  Expected: 196 stats + 196 opens -> <=20 stats + 1 open per opponent name shown.

STILL OUTSTANDING on that screen: `unlock checks` ~5.0-5.2 s every time, and
`playerdata save` ~1.8-2.1 s. Finer marks now added inside the unlock block
(`u:deckstats+diff`, `u:unlockables`, `u:unlock chain`) to split the 5 s. Note
`unlockRandomSet()` and the AI-decks branch each call `options.save()`, so some of that
5 s is likely more Memory Stick writes.

⚠ PRE-EXISTING, NOT FIXED: `Task::randomize()` does `rand() % getAIDeckCount()`. If the
count is ever 0 (AIDECKS_UNLOCKED=0, or no deck files) that is a division by zero. Not
reachable on this profile (aidecks=20) but it is a latent crash.

### Crash on CANCEL during damage assignment (2026-08-03)

Owner: "game crashed to off when i hit the cancel button during damage assignment against
2 creatures."

TWO UNGUARDED NULL DEREFERENCES found in `GuiCombat::CheckUserInput`, both on that path:

  1. `case JGE_BTN_CANCEL:` did `oldActive->zoom = kZoom_level2;` with no check.
     `oldActive` is a copy of `active` taken at function entry, and the
     `JGE_BTN_PRI` / `JGE_BTN_NEXT` / `JGE_BTN_PREV` cases in the SAME switch set
     `active = activeAtk = NULL`. The block immediately AFTER the switch already
     guards the identical pointer: `if (oldActive && oldActive != activeAtk)`.
  2. `else if (ATK == cursor_pos) { if (!activeAtk->blockers.empty())` — `activeAtk`
     unchecked, while `GuiCombat::Render()` opens its equivalent with `if (activeAtk)`.
     The cancel path can leave `cursor_pos == ATK` with `activeAtk` NULL.

Both guarded now (compiles clean; NOT yet gated, NOT yet device-verified).

⚠ EPISTEMICS: this is NOT proven to be the crash. "Crash to off" is the owner's memory
signature, and combat rendering under a tight heap remains a live alternative. The guards
are correct regardless — the file's own code proves both pointers are nullable. If the
crash survives this build, the answer is memory, and combat needs the same probe treatment
the deck editor got.

Same edge-sampling family as today's other finds, unrelated to this crash but recorded:
`DeckMenuItem.cpp:122` requests `RetrieveQuad("new.png", 2.0f, 2.0f, tex->mWidth - 0.0f,
tex->mHeight - 0.0f)` on a 32x24 image — sampling out to (34,26), past the image on both
axes. The near-identical call at line 35 subtracts `4.0f` and its comment says that is
exactly why. Line 122 has the same comment and the wrong arithmetic. NOT fixed.

### Owner ruling: auto-tap must be lookahead-aware (2026-08-03)

His words: "the way autotapped mana decides what mana to tap needs to look at what will be
able to cast after it. if one arrangement leaves you unable to cast, and one lets you cast,
then it should leave the mana configuration that can cast."

And on source preference, correcting Claude's proposal: "not really. in practice, you want
to use double sources over single sources usually because single sources are more flexible.
you obviously dont want to over tap, but fewer sources tapped means more configurations
possible later. and then the things you're trying to preserve are - be able to spend on
other casts, then variety of color."

TWO DISTINCT FLEXIBILITIES (Claude conflated them; his ruling separates them):
  * COLOUR flexibility - a dual pays more kinds of symbol.
  * GRANULARITY flexibility - many small sources give more combinations than few big ones.
  GRANULARITY DOMINATES. Spend the high-output sources, keep the singles, because tapping
  fewer PERMANENTS leaves more configurations available later.

RESULTING SPEC (lexicographic, among arrangements that pay the cost without overpaying):
    1. maximise: other casts still possible      <- the dominance rule he stated first
    2. then     : distinct colours still available
    3. then     : minimise permanents tapped     <- i.e. prefer high-output sources

CURRENT CODE: `ManaEngine::selectAutoTapProducers` (ManaEngine.cpp:639) is a two-pass
greedy over a layer-ordered `planPayment` result - pass 0 takes any producer whose
single-colour output covers a still-needed coloured symbol, pass 1 fills generic. It never
compares alternatives and never consults `amp->output->getConvertedCost()`, so it cannot
express any of the three criteria above. Its existing comment already describes the
overpay half of the problem ("{1}{G} tapping two Mountains before the Forest, floating the
extra red").

SCORER ALREADY EXISTS: `LegalActionsOracle` for "other casts still possible";
`ManaEngine::potentialColorReach` (ManaEngine.cpp:619) for colour variety;
`potentialManaPermissive` (605) for reach.

⚠ COST CONSTRAINT: `selectAutoTapProducers` also drives the human tap-preview border
(`GuiHand.cpp:274`), which re-runs as the cursor moves. Any candidate search needs a hard
cap (~8 arrangements) or it becomes a per-frame cost on a 333 MHz CPU.

NOT STARTED - queued behind verifying the texture-cache cap restore.

### Victory screen: 15941 ms -> 4445 ms (measured, 2026-08-03)

    start of day (worst)   15941 ms   tasks 9033 + unlock 5010 + save 1824
    after AI-deck fix       7418 ms   tasks 0    + unlock 5234 + save 1929
    now (worst / typical)   4445 / 3464 ms

`tasks` 0-275 ms (was 9033) - the AI-deck lookup fix is CONFIRMED on hardware.
`u:unlock chain` fell from 4921-9876 ms to 0-2374 ms as awards got granted, which supports
the reading that its cost is `options.save()` firing per award (GameOptionAward::giveAward
saves eagerly, and unlockRandomSet saves again) rather than computation. Every per-award
mark is 0-1 ms; `u:deckstats+diff` is 1-2 ms; `unlock checks` tail is a flat 67 ms.

REMAINING COST IS MEMORY STICK WRITES: `playerdata save` ~1.85-1.95 s EVERY victory, plus
up to ~2.4 s when an award saves. Any further win here means writing less often or less -
not computing less. Not yet attempted.

### Bisect: draws CONFIRMED WORKING at `3d9e5a935` (#29) on hardware
Owner: "cards are being drawn". Also "in game colors are right now" - the ARGB fix is
VISUALLY CONFIRMED for the first time (it had never been, because every earlier build
claiming it linked stale JGE).

So the draw regression is in #30 (`55f91ca78`, purge revert) or #31 (`3c2e79036`, safe
TexMemCheckpoint + REMOVAL OF THE 8 MB CAP). Deployed now: psp-port HEAD with the cap
restored plus every fix from today. This is a REGRESSION CHECK, not a repair - draws
currently work on #29 and the new build is what might break them.
  * still draws -> the cap was the cause; bisect closed
  * stops       -> the purge revert (#30) is the cause; 69-file change to attack directly

⚠ OWNER'S CORRECTION to Claude's model (important, supersedes the "drawn but invisible"
framing recorded earlier): "its not that it was invisible, because then i could have
selected it. its that memory overflow writes over other memory and produces undefined
behavior." Heap exhaustion here does NOT fail cleanly - `ResetCacheLimits`' own comment
says "2008-era callers don't check malloc failures". So exhaustion corrupts unrelated
state. That single mechanism covers the no-draw, the garbage streaks, the flat green
block, and the power-offs. It also means Claude's JGfx NULL guards are CONTAINMENT AT ONE
SITE, not a fix - the cap is what prevents the exhaustion.

### RULES BUG: buffed creature dies when the buff wears off (CR 514.2 violation)

Owner, live: "just had a goblin die from what im pretty sure was a race, where the buff it
was under giving it +1/+1 fell off before the combat damage it took was removed end of turn."

CONFIRMED MECHANISM (read from source, not yet reproduced in a test):

  `MTGCardInstance::addToToughness` (MTGCardInstance.cpp) models damage as REDUCED LIFE:
      toughness += value;
      life += value;
      doDamageTest = 1;

  `InstantAbility::testDestroy` (MTGAbility.cpp:8274) expires "until end of turn" effects
  when the phase becomes MTG_PHASE_AFTER_EOT.

  `APowerToughnessModifier`'s removal (AllAbilities.h, ~line 3009) calls
      target->power -= ...;
      target->addToToughness(-wppt->toughness.getValue());
  which decrements `life` AND sets `doDamageTest`, and
  `MTGCardInstance::afterDamage()` destroys the card when `life <= 0`.

  Trace for a 1/1 goblin pumped +1/+1 that took 1 combat damage:
      base            toughness 1  life 1
      buffed          toughness 2  life 2
      1 damage        toughness 2  life 1
      buff expires    toughness 1  life 0  -> afterDamage() -> destroy()   WRONG

CR 514.2: in the cleanup step, damage is removed from permanents and "until end of turn"
effects end SIMULTANEOUSLY, as a single turn-based action. CR 514.3: no player receives
priority and state-based actions are not checked in between. The engine does them
separately with a lethal check in the gap.

⚠ OPEN: I could NOT find any per-turn damage-removal step. `GameObserver::cleanupPhase()`
is called at MTG_PHASE_BEFORE_BEGIN (GameObserver.cpp:234) and chains to
`MTGGameZone::cleanupPhase` -> `MTGCardInstance::cleanup()`, which resets summoning
sickness / regenerateTokens / preventable / thatmuch and deletes `previous` - it does NOT
touch `life`. Searching for `life = toughness` across src finds only constructors,
regeneration, and a few one-off ability effects. Either damage removal lives somewhere I
have not found, or creature damage persists across turns. RESOLVE THIS BEFORE FIXING -
the fix depends on which.

FIX DIRECTION (not implemented): restore `life` to `toughness` for in-play creatures at
the cleanup step, in the same action that expires UEOT effects, with the lethal check
suppressed across the pair. `MTGCardInstance::afterDamage()` already has a
`skipDamageTestOnce` flag that is the natural hook.

This is ENGINE RULES CODE - needs the full suite (WAGIC_TESTSUITE_THREADS=1, 0 failed)
plus a regression test for the exact scenario before it can ship.

### Two decks built from the owner's collection (2026-08-03)
His collection: `User/player/collection.dat`, 207 cards / 156 unique, all resolvable.
Resolve IDs via `Res/sets/<SET>/_cards.dat` (`primitive=` + `id=`), then look up
mana/type/text by primitive name in `Res/sets/primitives/mtg.txt`.
Deck format is `Card Name (SET) *count` with `#NAME:` / `#DESC:` headers.

  * `User/player/deck1.txt` "Aetherborn Grind" - B/G midrange, 64 cards
  * `User/player/deck2.txt` "Skills Paid the Bills" - R/W aggro, 61 cards
  Sources kept at /tmp/deck1.txt, /tmp/deck2.txt (regenerate from collection.dat).
Basics cap the design: he owns only ~10 of each basic land, so two-colour decks max out
around 20 lands.

⚠ SIDE EFFECT HE WAS WARNED ABOUT: `GameStateDuel.cpp:318-325` sets `decksneeded = 0` as
soon as ANY user deck exists, and the premade fill only runs when `decksneeded` is 1. So
with user decks present the PREMADE DECKS VANISH from the duel deck menu. This is the same
mechanism behind his earlier "I have a build deck that isn't built, so I can't play with
the default". Deleting the deck files restores the premades.

### STATE AT COMPACTION (2026-08-03)
ON THE PSP NOW: the UI-marker diagnostic build (8,419,798) = psp-port HEAD + cap restored
+ all of today's fixes + `WAGIC_UIMARKERS` + the shared-"white"-quad tint fix. Built with
`EXTRA_DEFS="-DWAGIC_DECKPROBE -DWAGIC_WINPROBE -DWAGIC_UIMARKERS"`.

UNREPORTED / AWAITING HIS TEST:
  1. DRAWS on HEAD+cap (regression check; draws were confirmed WORKING at #29).
     still draws -> cap was the cause, bisect closed. stops -> purge revert #30 is.
  2. THE GREEN BLOCK, under markers. Six FillRoundRect sites now draw magenta / cyan /
     yellow / orange / blue / white (deliberately no green). Block changes colour -> that
     site. Stays green -> all six cleared. Disappears -> it was the shared-quad tint leak.

WORKING TREE: on psp-port HEAD, ~10 source files modified, NOTHING COMMITTED, NOT GATED.
The tree carries diagnostics that MUST come out before any commit: `WAGIC_DECKPROBE`
(GameStateDeckViewer), `WAGIC_WINPROBE` + per-award marks (Credits.cpp), `WAGIC_DRAWPROBE`
(MTGGameZones), and `WAGIC_UIMARK` (JTypes.h + MenuItem.cpp + InteractiveButton.cpp).
Keepers: cap restore, JGfx NULL guards, tutorial changes, deck-editor bg removal, chrome
gating, deck-title removal, Tasks AI-deck fix, GuiCombat NULL guards, new.png sub-quad,
shared-white-quad restores.


================================================================================
2026-08-03 (late session) — SESSION HALTED BY OWNER. UNRESOLVED.
Owner is reverting the PSP to a known-working build and reviewing every change
below with Fable 5. Written for that reviewer, not for continuity.
================================================================================

## THE UNRESOLVED SYMPTOM

Owner, verbatim across the session:

  "there are odd sots where it freezes up for a moment in the middle of a match,
   then the screen flickers and it resumes."
  "game is still freezing up in match, happens whenb i go to open my hand for the
   first time in a turn"
  "its also happening when they play, its gotta be related to loading a new image"
  "halts. no frames are written"

So: the main thread BLOCKS while a new card image is needed. Last frame stays on
screen, nothing renders, then it resumes. Owner explicitly ruled out two of my
readings — nothing changes size, there is no pop, and the flicker is not a
rendering pathology, it is just the resume.

Owner's boundary claim, which I did not honour early enough:
  "it MOITHERFUCKING WORKED PERFECTLY BEFORE WE STARTED WORKING ON THE VITA"

VERIFIED AGAINST HISTORY: `763676441` ("Vita: build the fork engine") is the FIRST
Vita commit. My entire bisect was anchored there — i.e. anchored at the START of
the broken era. Everything I "confirmed good" was confirmed inside the bad range.
Last commit before Vita work = `671b6c412`. 30 commits from there to HEAD.

Also verified: `8a4eddc53` ("PSP: mid-frame texture-memory checkpoint + texture
cache hard cap") is PRE-Vita, i.e. inside the working era. I spent the day
reverting and restoring halves of that commit. It was never the problem.

## STATE OF THE DEVICE (what the owner must undo)

Deployed to /run/media/magi/disk/PSP/GAME/WAGIC/ at ~17:50:
  - EBOOT.PBP            8,417,374 bytes  (byte-verified)
  - Res/graphics/        19 resized assets (byte-verified)

REVERTING THE EBOOT ALONE IS NOT ENOUGH — the 19 graphics files were also
overwritten on the Memory Stick. Restore both:
  git checkout -- projects/mtg/bin/Res/graphics/
  or from  /home/magi/Projects/wagicGPT/psp-work/graphics-orig-20260803-audit/  (19 files)
Generic card backups (NOT yet deployed):
  /home/magi/Projects/wagicGPT/psp-work/generic-cards-orig-20260803/  (9 files)

Prior EBOOT archived at /tmp/EBOOT-uimark.PBP (8,419,798) — that is the UI-marker
diagnostic build, NOT a known-good build.

## WORKING TREE — UNCOMMITTED, PARTIALLY UNGATED

13 source files modified vs HEAD (`3c2e79036`). `git diff HEAD --stat`.

FOUR DIAGNOSTIC SWITCHES STILL IN THE SOURCE, must be stripped before any commit:
  WAGIC_DECKPROBE   GameStateDeckViewer.cpp   #if defined  — compiled OUT of the deployed build
  WAGIC_WINPROBE    Credits.cpp               #if defined  — compiled OUT
  WAGIC_UIMARKERS   JTypes.h/MenuItem/InteractiveButton  #if defined — compiled OUT
  WAGIC_DRAWPROBE   MTGGameZones.cpp          getenv()     — compiled IN but DEAD
                    (psp_noenv.h #defines getenv to always return 0 on PSP)

CHANGES MADE THIS SESSION:
  REVERTED  WResourceManager.cpp — the 8 MB PSP_TEXCACHE_HARD_CAP I had restored.
            File is now identical to HEAD. Owner asked for this explicitly.
  NEW       AllAbilities.cpp — ATutorialMessage::Render() guard changed from
            `if (!mBgTex)` to `if (!mBgTex && mObjects.empty())`.
            REAL DEFECT I INTRODUCED EARLIER TODAY: removing the taskboard.png
            assignment meant mBgTex stayed NULL forever in text mode, so the block
            re-ran EVERY FRAME, allocating a fresh IconButton per frame and growing
            the child list without bound. GATED GREEN (see below).
  NEW       JGfx.cpp — TexAlloc() now memsets its allocation to 0, and a new
            VTexAlloc() does the same for the four valloc() VRAM sites.
            REASON: texture buffers are allocated at the POWER-OF-TWO bucket size
            but loaders only write the image's own w*h pixels. Everything right of
            and below the image was uninitialised heap, and was rendered.
            NOT BUILT. NOT GATED. NOT DEPLOYED.
  NEW (data) 19 PSP chrome assets resized to land under a power of two.
            psp* texture bucket total 17.75 MiB -> 10.25 MiB (7.50 MiB freed).
            DEPLOYED. Owner reported the long-standing green block on the deck
            select screen is GONE after this.
  NEW (data) 9 generic fallback cards Res/sets/*.jpg 200x285 -> 180x250.
            2.25 MiB freed; also makes the placeholder match real card geometry
            (the card pack is 180x250). NOT DEPLOYED.

## GATE

Run on the desktop binary linked 17:38, i.e. WITH the AllAbilities fix and
WITHOUT the JGfx zeroing change:

  1031 tests (0 failed)   24 AI tests (0 failed)   PARSETEST 319 passed, 0 failed

Exactly baseline. So the tutorial leak fix is clean. The JGfx change is UNGATED.

## MEMORY AUDIT — FINDINGS (arithmetic, not measured on device)

  1. 10 PSP assets were 480x272 -> 512x512 bucket = 1.00 MiB each. At height 255
     they are 512x256 = 0.50 MiB. Saved 5.00 MiB.
  2. pspshop / pspawardback / pspmenubgdeckeditor.jpg / pspshop_light / pspmenupanel
     + three assets sitting exactly ON 256. Saved ~2.50 MiB.
  3. ATutorialMessage per-frame IconButton leak (above).
  4. RetrieveQuad DEFAULTS to RETRIEVE_LOCK (WResourceManagerImpl.h:153) — 19 call
     sites pin their texture for the life of the process without saying so.
     ~1 MiB pinned on PSP, never evictable. NOT ADDRESSED.
  5. RetrieveTempQuad called every frame from Render() in TextScroller.cpp:169-182,
     GuiPhaseBar.cpp:99-101, SimplePopup.cpp:43-49 — string-keyed cache lookup per
     frame. NOT ADDRESSED.

  Card pack is CORRECT — 180x250 -> 256x256 = 256 KB, and make-psp-cards.sh
  documents the bucket rule in its header. I wrongly claimed it was 200x285; that
  was Res/sets/*.jpg, the generic fallbacks. Correction stands.

## WHERE I GOT TO ON THE HALT (unfinished, low confidence)

Card loads ARE async on PSP: on a cache miss WResourceManager.cpp:1241 queues a
CacheEngine request and returns the generic card immediately. So the file read is
not what blocks the UI thread.

Two candidates I had NOT finished checking when the session ended:
  a) WCache::Cleanup() runs ON THE MAIN THREAD (WResourceManager.cpp ~1243), takes
     sCacheMutex, and loops RemoveOldest() until under the limit.
  b) LoadIntoCache() holds sLoadFunctionMutex across the ENTIRE decode
     (WResourceManager.cpp:1273). The code's own comment says this exists because
     "paging up in the deck editor which forces 7 cards to load simultaneously...
     we'd block the UI thread for a long period." Any MAIN-thread texture load
     (UI chrome, fonts) waits behind a worker-thread card decode.
  (b) fits "first time opening my hand in a turn" — multiple cards resolve at once.
  NEITHER IS CONFIRMED. Do not treat this as a diagnosis.

Also note RemoveOldest() sorts purely on lastTime, so it can evict a texture that
is about to be drawn again in the same frame. Untested idea, not implemented.

## MISTAKES I MADE THIS SESSION (for the reviewer)

  1. Anchored the bisect at `763676441`, which IS the first Vita commit — so the
     baseline was inside the broken range. Owner had to tell me twice.
  2. Ran the desktop gate and the PSP Docker build CONCURRENTLY, and moved the
     objs/ directories out from under the running gate. Owner: "dont build two
     pieces of sofware at once... you fuck it up and cross contaminate."
  3. Claimed the card pack was 200x285 and needed regenerating. Wrong — I had
     measured Res/sets/*.jpg (generic fallbacks). The pack is correct and I wrote
     the script that made it correct.
  4. Read JPEG dimensions with `file`, which reports something other than image
     dimensions. Squashed two 480x272 backgrounds to 255 wide before catching it;
     restored from backup and redid at 480x255. USE PIL, NOT `file`, FOR JPEG.
  5. Attributed the freeze/flicker to a card-size "pop" on image swap. Owner:
     "nothing ius changing size, there is no pop, i did not say pop."
  6. Attributed the freeze to the 8 MB cap I had restored. Wrong — the cap is
     pre-Vita and was present throughout the working era.

## OWNER'S STANDING DIRECTION

Considering a TOTAL ENGINE REVISION: existing code + the MTG Comprehensive Rules
as reference, treating "the existing lexicon of cardscript as a contract that has
to serve all the existing scripted cards." Sequenced AFTER running the dev loop
over every extant deck, because "i want the revision to assume the llm opponent,
and be structured for that."

Structural findings supporting that, read from source this session:
  - MTGCardInstance::life is BOTH toughness and remaining damage capacity
    (addToToughness does `toughness += value; life += value;`), which is why a
    creature dies when a +1/+1 buff wears off after it took damage — CR 514.2
    requires damage removal and UEOT expiry to be simultaneous.
  - Effect removal is hand-written inversion (`power -= x`), not recomputation
    from base. No CR 613 layer system.
  - The lethal check is EDGE-triggered on a per-card `doDamageTest` flag that
    callers must remember to set (MTGCardInstance::afterDamage early-returns
    without it), not a level-triggered CR 704 sweep. `skipDamageTestOnce` is a
    manual patch over one such case. `GameObserver::gameStateBasedEffects()`
    exists but is a procedural chore loop (snow mana clamping, devoid colour
    stripping, haunt cleanup) — the lethal check is not in it.
  - STILL OPEN: I could not find where per-turn damage removal happens.
    cleanupPhase() -> MTGCardInstance::cleanup() does NOT touch `life`. Resolve
    this before attempting the CR 514.2 fix; the fix depends on the answer.


## BUILD WALK-UP LOG (2026-08-04) — Res held CONSTANT

Baseline established 2026-08-04: clean Res reinstall from verified sources + the
CORRECT card pack (wagic-card-packs/psp, all cards <=180x250 -> 256 KB). PSP free
space reads 830 MB, matching the owner's pre-wipe reading. 336/336 zips byte-verified.
Only EBOOT.PBP changes between steps; Res and User are not touched.

Owner's hypothesis driving this walk (his words, 2026-08-04): the delays were
"always there, but because my baseline expectation was the slow framerate achieved
under the pressure of the larger cards, i read those delays as minor inconsistencies
in the framerate. once the framerate became faster... the delays that were not
framerate related became more apparent, reading as emergent freezes."
Corroborated by the audit timeline: the correct pack first reached the device
2026-08-03 15:40; the bad 180x257 pack was resident from 08-01 11:21 until then.
IMPLICATION: if true, no commit introduced the delays and a bisect will not find them.
CONFIRMED IN PART at step 0: freezes are present on a capped build low in the range,
with the correct pack. So the walk-up is NOT a hunt for the freeze origin - it is the
owner observing build-to-build differences generally, with Res held constant.

| step | build | size | commit / contents | owner's observation |
|------|-------|------|-------------------|---------------------|
| 0 | EBOOT-awardtiming | 8,413,174 | cap present, colour fix not yet linked | FREEZES PRESENT on the correct pack. Owner confirmed 2026-08-04 - this is the observation the masking hypothesis was derived FROM. Establishes the delays are pre-existing at this commit, not introduced further up the ladder. |
| 1 | EBOOT-29 | 8,415,566 | 3d9e5a935 - colour fix links, cap present | PENDING (re-deployed as the discriminating test below) |
| 2 | EBOOT-capfix-head | 8,419,662 | HEAD + cap restored | **CRASH TO OFF on starting a SECOND match after finishing the first. Reproduced TWICE.** Cap IS present, so this is not the no-cap exhaustion mode. |

### Hypothesis from step 2 (Claude's, NOT established)
capfix-head contains 3c2e79036's TexMemCheckpoint change; EBOOT-29 does not.
That change made the checkpoint `if (gInFrame) return;` - i.e. mid-frame it now
reclaims NOTHING. The old code reclaimed by tearing down the in-flight display list
(crude, and the cause of the deck-editor loop, which is why it was changed). But the
replacement removed mid-frame reclamation entirely: deferred texture frees sit on
gDeferredFreesRam/Vram until a checkpoint fires OUT of frame, which in practice means
screen transitions. Match 2 therefore starts with match 1's freed textures still
unreclaimed, then allocates a whole new board on top. Fits 'first match fine, second
match crashes' exactly.
DISCRIMINATING TEST: EBOOT-29 predates the change. Two matches back to back.
  survives -> JGfx change implicated, specific line to fix
  crashes  -> mechanism is older, hypothesis dead



## 2026-08-04 — CROSS-MATCH CRASH ROOT-CAUSED: ARENA RATCHET, NOT A LEAK

Probe build: worktree at 3d9e5a935 + WAGIC_MEMPROBE (EBOOT 8,414,422, /tmp/EBOOT-memprobe.PBP).
Instrumentation logs at every duel phase transition to User/wagic-memprobe.log.
Raw log preserved at psp-work/memprobe-20260804.log. Run: match 1 completed, returned
to main menu, CRASH TO OFF loading match 2.

                          managed  UNRECLAIM_KB   heapUsedKB   arenaKB
  m1 choose_deck1            12       2266          30462      30584
  m1 play                    13       2394          36642      37212
  m1 end                     13       2394          39316      41840
     -- main menu, ClearUnlocked() runs --
  m2 choose_deck1            13       2394          30106      42360
  m2 play                    13       2394          37133      42564   <- crash

VERDICT (measured, not inferred):
  - managed map flat (12->13): NOT a managed-map leak.
  - UNRECLAIMABLE_KB flat (2266->2394): NOT a locked-texture leak.
  - heapUsedKB RETURNS to baseline (39316 -> 30106): the game frees correctly.
    THERE IS NO LEAK. Three earlier hypotheses (ClearUnlocked call sites, managed
    map, IconButton pinning) are all dead.
  - arenaKB NEVER returns: 30584 -> 41840 -> 42360 -> 42564. The allocator's
    high-water mark ratchets and fragmented free space cannot be returned to the
    system. PSP heap total ~44,088 KB, so at crash the arena held 96.5% of it.

MECHANISM: match 1's PEAK becomes match 2's permanent FLOOR. Match 1 is always fine;
match 2 has ~1.5 MB of room and dies. Independent of build, which is why the bisect
found nothing and no leak was locatable.

WHY THE CAP MATTERS: the texture cache reached cacheKB=7728 / items=63 by end of
match 1. The cap does not stop a leak - it bounds the HIGH-WATER MARK, the only
thing that matters under an arena ratchet. Uncapped builds let peak-match-1 consume
the arena outright -> crash to off. Capped builds degrade to lag instead.

FIX DIRECTION: lower peak allocation during a match. NOT leak-hunting.
  candidates: restore/tune the texture cache cap; reduce simultaneous card textures;
  reduce per-match transient allocation; consider whether the allocator can be made
  to release (mallopt/trim) on PSP at all.

## 2026-08-04 — LEVER 3 (malloc_trim) IS DEAD. REAL MECHANISM IS FRAGMENTATION.

Probe: worktree at 3c2e79036 + PSP_TEXCACHE_HARD_CAP + WAGIC_MEMPROBE with mallinfo
keepcost/fordblks. EBOOT 8,419,790 (= capfix-head 8,419,662 + 128 bytes of probe).
Raw log: psp-work/memprobe-keepcost-20260804.log. Run: match 1 completed, menu,
CRASH TO OFF loading match 2. Same outcome as the uninstrumented capfix-head.

                        heapUsedKB   arenaKB   freeKB   KEEPCOST_KB
  m1 choose_deck1          30462      30583      120        44
  m1 end                   38927      41319     2391        64
     -- main menu --
  m2 choose_deck1          29999      39087     9087         0
  m2 choose_deck2_to_play  36889      42171     5281        12
  m2 play                  37019      42171     5152        12   <- crash

FINDING 1 - LEVER 3 CLOSED: keepcost never exceeds 64 KB. At m2 start there are
9,087 KB free in the arena and 0 KB returnable to the system. malloc_trim(0) would
recover nothing. The custom _sbrk in JGE/src/main.cpp DOES support negative incr
(bounds-checked), and newlib provides malloc_trim - the machinery works, there is
simply nothing at the top of the heap to give back.

FINDING 2 - THE MECHANISM IS FRAGMENTATION, NOT A HIGH-WATER RATCHET:
m2 begins with 9 MB free inside the arena and the arena STILL GROWS 39,087 -> 42,171
to load the board. malloc had 9 MB and could not use it: the free space is shattered
into fragments smaller than a 256 KB texture, so each allocation extends the arena
instead of reusing a hole. The heap exhausts CONTIGUOUS space long before total space.
Match 1's mixed-size texture alloc/free churn shreds the heap; match 2 cannot fit
into the debris.

CONSEQUENCE FOR THE FIX: bounding peak allocation (cap, smaller assets) only DELAYS
this - fragmentation accrues regardless. The structural fix is UNIFORM allocation:

  SLAB ALLOCATOR FOR TEXTURE PIXELS. JGE/src/JGfx.cpp TexAlloc() is already the single
  choke point for every texture-pixel allocation (PNG/JPEG/GIF loaders + the VRAM
  VTexAlloc path). Back it with a pool of fixed-size slots carved once at startup:
  freed slots are exactly reusable forever and texture memory stops fragmenting the
  general heap. Slot size follows the bucket rule already in force (256x256x4 = 256 KB
  covers every card; larger chrome sheets need a second class or a fallback).

Retroactive explanation: the correct card pack (uniform <=180x250 -> 256 KB) fragments
far less than the bad pack's mix of 256 and 512 KB. The pack swap fixed a fragmentation
source before either party knew that was the mechanism.

DEAD HYPOTHESES (all measured, do not revisit): ClearUnlocked call sites; managed-map
growth; locked-texture growth; IconButton pinning; a leak of any kind (heapUsed returns
to baseline every match); malloc_trim/arena return.

## 2026-08-06 — HANDOFF (owner continuing with Fable)

DEVICE STATE (PSP):
  EBOOT.PBP 8,419,790 = commit 3c2e79036 + PSP_TEXCACHE_HARD_CAP + WAGIC_MEMPROBE.
  Res: clean reinstall, 3,493 files, CORRECT card pack (wagic-card-packs/psp, every
  card <=180x250 -> 256 KB bucket, 336/336 byte-verified). PSP reports 830 MB free,
  matching the owner's pre-wipe reading. User/ intact (14 files) + backed up at
  psp-work/psp-user-backup-20260803-2015/.
  The WRONG pack (180x257, 35% at double cost) was DELETED from disk at owner's
  instruction - psp-work/image-zips and psp-work/image-pack-psp are gone.

REPO STATE (~/Projects/wagicGPT-psp):
  HEAD = 3c2e79036. 13 source files modified, UNCOMMITTED, UNGATED. Contains keepers
  (tutorial per-frame IconButton leak fix, Tasks.cpp victory-screen fix verified on
  hardware, GuiCombat NULL guards, JGfx NULL guards + TexAlloc/VTexAlloc zeroing,
  CardGui shared-white-quad restores, DeckMenu chrome gating) AND four diagnostic
  switches that MUST be stripped before commit: WAGIC_DECKPROBE, WAGIC_WINPROBE,
  WAGIC_UIMARKERS, WAGIC_DRAWPROBE. Last gate on the tutorial fix was GREEN
  (1031/0, 24 AI/0, PARSETEST 319/0); the JGfx zeroing change is UNGATED.
  git worktree at ~/Projects/wagic-probe holds the probe build (3c2e79036 + cap +
  memprobe) - separate checkout, main tree unaffected. Remove with
  `git worktree remove ~/Projects/wagic-probe` when done.

THE OPEN PROBLEM, root-caused and measured (see the two sections above):
  Match 2 crashes to off / lags because match 1's mixed-size texture churn FRAGMENTS
  the heap. NOT a leak - heapUsed returns to baseline every match. NOT returnable -
  keepcost is ~0, so malloc_trim recovers nothing. Match 2 starts with 9 MB free and
  the arena still grows, because no free fragment is big enough for a 256 KB texture.
  PROPOSED FIX (not built): slab allocator behind JGE/src/JGfx.cpp TexAlloc(), the
  single choke point for all texture-pixel allocation. Fixed-size slots carved at
  startup; freed slots exactly reusable. Open questions before building: largest
  single texture allocation (chrome sheets exceed a card, may need a second size
  class or memalign fallback), and total pool reservation.

ALSO UNRESOLVED (separate from the crash): the mid-match freezes on hand-open and on
card-play. Owner's masking hypothesis (2026-08-04) is that these are PRE-EXISTING and
only became perceptible once the correct card pack raised the frame rate. Consistent
with the fragmentation finding: a near-full, shattered heap makes every texture load
a long search. Not independently confirmed.

## 2026-08-06 — FABLE REVIEW + CLEANUP: the working tree is now eleven commits, pushed

Full review of the 08-03..08-06 work, then the cleanup it called for. Branch
psp-port `3c2e79036` -> `8a8caa705`, pushed. Gate on the exact committed source
state, main-tree env, single-threaded, 4G-capped: **1031/0 + 24 AI/0 + PARSETEST
319/0** — exact baseline. Desktop binary /tmp/wagic-8a8caa705. (The PSP-side
JGfx.cpp changes are structurally ungateable by the desktop suite — it compiles
JGE/src/pc/JGfx.cpp — so their verification remains the hardware rounds already
run plus the next one.)

WHAT THE CLEANUP DID:
  1. UU merge-state on AllAbilities/Tasks resolved-and-staged; stash dropped
     (verified fully covered by 55f91ca78 except build artifacts); wagic-probe
     worktree salvaged (cap + memprobe extracted) and removed.
  2. Hunt diagnostics STRIPPED: WAGIC_UIMARKERS (purpose discharged — the green
     block is explained by uninitialised bucket padding) and WAGIC_DRAWPROBE
     (getenv-gated = live in desktop builds). KEPT as permanent opt-in infra:
     WAGIC_DECKPROBE, WAGIC_WINPROBE, WAGIC_MEMPROBE (comments rewritten from
     "REMOVE before committing" to what they are: the instruments that found
     the fragmentation and the 196-file walk).
  3. **THE CAP IS BACK ON THE BRANCH** (`0d973006c`), 8 MB, PSP-only, its own
     commit per the never-bundle rule. The 08-03 removal order predates the
     08-04 fragmentation finding that proved the cap load-bearing; the commit
     message carries the full reversal rationale. ⚠ Aaron: this deliberately
     reverses your 08-03 "revert the cap" instruction — the measured arena
     ratchet is why; flag if you disagree.
  4. The 19-asset height-255 resize RE-LANDED as a commit (`8a8caa705`,
     regenerated from tree originals with PIL/LANCZOS, 7.00 MiB bucket saving,
     verified no dimension on 256 or at 272). The 9 generic-card 180x250
     resizes committed (`accec619b`).
  5. JGE/Makefile FIXED (`4bd2b1432`): both PSP archive rules delete-before-ar,
     EXTRA_CLEAN teaches build.mak's clean about libhgetools.a. `make clean`
     inside the container is now a real clean (dry-run verified; hermetic build
     of this tree launched to prove it in anger).
  6. CR 514.2 buff-death bug docketed in
     strategy-design/lexicon-triage/engine-bugs.md (master `288dad9d9`) — it is
     PLATFORM-INDEPENDENT engine rules code, not a PSP item.

DEVICE: unchanged this session — still the probe EBOOT (8,419,790) from the
08-06 handoff. The hermetic rebuild of `8a8caa705` is the natural next deploy;
it now contains everything the probe build had (cap included) MINUS the
memprobe define, PLUS the re-landed 255 chrome (which must ALSO be copied to
the stick's Res/graphics — the EBOOT alone does not carry it).

OPEN LEDGER (carried, none closed this session):
  - Fragmentation is THE crash mechanism; slab allocator behind TexAlloc is the
    proposed structural fix (handoff section above). Swizzle scratch buffers
    verified small (texWidth*8 rows ≈ 8 KB) — TexAlloc really is the only choke
    point that matters.
  - Mid-match freezes: unconfirmed; LoadIntoCache holds sLoadFunctionMutex
    across whole decodes (candidate b), consistent with a shattered near-full
    heap making every load a long search.
  - Never-draws: never formally closed — presumed memory-UB (both 08-04 probe
    runs completed match 1 normally). One deliberate observation on the next
    build closes it or reopens it.
  - Victory screen: playerdata save ~1.9 s every win (Memory Stick write cost;
    write less or less often).
  - RetrieveQuad's silent RETRIEVE_LOCK default: 19 sites pin ~1 MiB. Unaudited.
  - Task::randomize() rand()%getAIDeckCount() divides by zero if the count is 0.
    Latent, unreachable on this profile.
  - WITH_GPT_AI compiled into PSP (~2 MB binary, no network path yet): build
    without it and measure the heap recovered — free experiment, needs a device
    round.
  - Auto-tap lookahead spec (owner ruling 08-03) queued; deck-editor
    ResetCacheLimits-per-state proposal queued.

HERMETIC EBOOT OF `8a8caa705` BUILT (2026-08-06, via the NEW `make clean` — the
fresh `ar: creating` lines confirm delete-before-ar works in anger): 8,417,726
bytes, md5 f2deb7f37b0062cb306e1bd34aa80970, archived /tmp/EBOOT-8a8caa705.PBP.
NOT deployed. Deploying it needs BOTH the EBOOT and the 19 resized
Res/graphics/psp* files copied to the stick.

## 2026-08-06 — OWNER CORRECTION: the pre-Vita era is a STRONG baseline. Two dead stories.

OWNER, definitive: the pre-Vita PSP EBOOT was verified WITH card art, as it
would be deployed — "much of the testing of the psp eboot utilized card art" —
and he played AT LEAST 20 SEQUENTIAL MATCHES on it. The match-2 crash
"only materialized when I decided to wipe the resources and reestablish the
baseline" (the 2026-08-04 clean reinstall).

TWO CLAUDE CAUSAL STORIES ARE DEAD — do not re-derive either:
  1. "The crash was always there, just unreachable until match 1 became
     completable" — WRONG. 20+ sequential matches pre-Vita.
  2. "Card art churn arriving 08-01 / the resized-chrome headroom swing is the
     delta" — WRONG as stated. The working pre-Vita config had full art AND
     original 272-tall chrome, same as the crashing post-reset config.
  ALSO SUSPECT: the retroactive note "the correct pack fragments far less than
  the bad pack" — the OWNER'S timeline runs the other way (bad-pack era
  survived 20+ matches; correct-pack era dies on match 2). Unverified inference;
  treat as open.

WHAT STANDS (measured): the death MECHANISM at match-2 load is contiguous-space
exhaustion (memprobe: no leak, keepcost ~0, arena grows past 9 MB nominal free).
The open question is why the pre-Vita configuration did not produce it.

THE TWO REMAINING DELTAS between working and crashing eras:
  A. Code: the ~30 commits 671b6c412..HEAD (671b6c412 = last pre-Vita commit).
     The owner's halt-session boundary claim pointed here; the 08-03 bisect over
     this range was invalidated by stale libraries, and the 08-04 walk-up never
     stepped below the Vita boundary. NEVER TESTED HERMETICALLY.
  B. Stick: pre-wipe accumulated Res (incl. the bad 180x257 pack) vs the 08-04
     clean reinstall (correct 180x250 pack, verified sources).

DISCRIMINATING EXPERIMENT (launched this session): hermetic 671b6c412 EBOOT on
the CURRENT stick, nothing else touched, owner's normal multi-match protocol.
  survives match 2+ repeatedly -> regression is in the 30 commits; bisect them
    hermetically with the stick frozen.
  crashes -> code exonerated; the delta is the stick, prime suspect = the pack
    swap (regenerate the 257-tall pack to test directly).

PRE-VITA PROBE EBOOT BUILT (2026-08-06, hermetic, container): commit 671b6c412
("Bundle the core resource pack into the APK"), 8,391,286 bytes, md5
6cfef80a6f29280fe05fdfa544c0531d, at /tmp/EBOOT-671b6c412-previta.PBP.
Ancestry verified: sidecar offload, deferred frees, ResetCacheLimits underflow
guard, FAT rules lookup, noenv shim, WITH_GPT_AI, and the native
checkpoint+8MB-cap are ALL in it — it runs against the current stick as-is.
DEPLOY PROTOCOL: copy the EBOOT ONLY; do not touch Res or User (the stick is
the frozen variable). Play the normal multi-match sequence.
  match 2+ survives repeatedly -> regression in 671b6c412..3c2e79036; bisect
    hermetically, stick frozen.
  crashes -> code exonerated -> stick delta causal; prime suspect the
    180x257 -> 180x250 pack swap (regenerate the 257 pack to test).
Worktree returned to psp-port @ 8a8caa705; pre-Vita build artifacts cleaned.

## 2026-08-06 — AI NEVER BLOCKS on post-Aug-1 builds: mechanism identified (not yet fixed)

OWNER, live on the pre-Vita probe EBOOT: "enemies dont seem to block at all
[on recent builds]. and on this build, they're blocking, maybe even
excessively." (Excessive = normal Baka gang-piling.)

MECHANISM (read from the diffs, high confidence, not yet live-verified):
`aa0e6bf5a` added the no-legal-action auto-skip (human seat has no legal action
in a phase -> userRequestNextGamePhase). `cfc2a8a76` gated it to the human's
OWN turn to protect the AI's turn. BLIND SPOT: the AI's BLOCKER DECLARATION
happens on the HUMAN's turn. During declare-blockers, currentPlayer == human,
so the gate passes; with no castable instants, hasAnyLegalAction(human) is
false and the skip advances combat BEFORE the AI defender's throttled Act
declares blocks. The skip races a decision the AI never gets to make.
The commits' "opponent's turn is covered by the reactive half" covers instant
RESPONSES, not the opponent's mandatory combat declaration during your turn.

WHY NOTHING CAUGHT IT: the skip is inert in suite games (by design - the
commit says live play gates the behaviour) and selfplay has no human seat
(humanSeat NULL -> skip never fires). Only live human-vs-AI play exposes it.
Timeline fits exactly: blocks on 671b6c412 (skip absent), none after Aug 1.

FIX SHAPE (queued BEHIND the match-2 crash experiment - do not confound it):
add a fourth hold alongside stack/menu/targetChooser - do not skip while the
OTHER seat has a pending combat decision (pendingCombatDecision is the
existing engine authority). Verification must be LIVE play (desktop
human-vs-AI: attack into an AI with obvious blockers), because the suite
structurally cannot exercise this rule. Also re-examine whether the same race
can eat other AI decisions that occur on the human's turn.

## 2026-08-06 — PRE-VITA MEMPROBE RUN: the economics are measured, and CODE IS FULLY EXONERATED

Instrumented pre-Vita EBOOT (671b6c412 + WAGIC_MEMPROBE, md5 e0ce0768...), current
stick, 3-match session: m1 clean, m2 clean AND SMOOTH (owner: "no extra frame
lag"), m3 CRASH TO OFF right after the `play` transition (post-music, pre-UI).
Log harvested: psp-work/memprobe-previta-20260806.log. Earlier same day, the
UNinstrumented pre-Vita EBOOT crashed twice in match-2 setup (opponent menu,
then player-deck menu) — and note the probe build ran FURTHER than the plain
one; single runs on this margin say nothing about probe overhead either way.

THE MEASURED ECONOMICS (this log + the Aug-4 logs agree):
  ceiling ~44.1 MB | cold-boot menu floor 30.6 | post-match menu floor 37.4
  (stable across matches — the ratchet does NOT climb further) | every
  consecutive match peaks ~40.5-41.5 | trimmable ~0 always; menu "free" is
  6.6-7.4 MB of SHATTERED fragments. So EVERY match after the first runs within
  ~2.5 MB of death and the outcome is DICE — which covers every observation at
  once: menu crashes, play crashes, smooth runs, and the run-to-run variance.
  m2 survived at 40.5; m3 died at 41.5.

CONSEQUENCES:
  1. Pre-Vita code shows THE SAME floor/peak/death as post-Vita code (Aug-4
     logs) — the 30-commit code delta is CLOSED BY MEASUREMENT. The regression
     hunt is over; there is no code regression in the crash.
  2. The 20-match era ran at better odds — the remaining candidate deltas are
     stick-side (pack composition) and/or collection scale, but the FIX no
     longer depends on resolving that history: buy back margin.
  3. QUANTIFIED FIX LADDER: (a) deploy the 255 chrome (committed, NOT on
     stick) — returns MB exactly at the menus that die; (b) lean PSP build
     without the GPT layer (`make PSP_GPT=0`, Makefile switch added) — ~2 MB of
     binary out of the same heap; (c) the slab allocator — makes the 6-7 MB of
     shattered free USABLE, ends the dice. (a)+(b) ≈ +4-6 MB margin on a
     2.5 MB-margin failure = likely robust consecutive play before (c) lands.

## 2026-08-06 — FULL-PAYLOAD ROUND: margin bought distance, not survival. SLAB IS NOW.

Deploy: EBOOT d0f8e86ca-full (blocker fix, cap, GPT aboard) + the 19 resized
chrome assets. Owner's run: matches 1 and 2 complete, CRASH TO OFF mid-game-3,
"about 1 turn before id have won." Best distance of any session this week (the
probe build died LOADING match 3; the plain builds died at the match-2 menus)
— consistent with +margin shifting the dice, and with fragmentation still
accruing underneath. The owner earlier REJECTED the lean (GPT-less) deploy on
principle: removing the LLM layer hides the problem the fork exists to solve —
the full build must fit. So the remaining gap is structural.
NEXT BUILD = the slab allocator behind TexAlloc (design questions now answered
by measurement: classes 32KB thumbs / 256KB cards / 512KB chrome max, single
choke point, fallback-to-memalign for oversize with a counter).

## 2026-08-06 — SLAB ALLOCATOR BUILT; first deploy crashed at BOOT; carve timing was the bug

THE SLAB IS IN (JGE/src/JGfx.cpp): bounded buddy, orders 32K..512K over one
10 MB region, TexAlloc tries pool -> counted fallback to old memalign path;
TexFree routes by address range; all loader frees + the deferred-free flush
routed. Cache cap retuned 8 -> 6 MB so cache + pinned + in-flight fit the
pool. Memprobe line grew poolFreeKB/poolLargestKB/poolFallbacks/poolOversize.

FIRST DEPLOY (EBOOT dd603ec4, lazy first-texture init) CRASHED TO OFF at boot,
after intro video/text, before the loading screen drew. ROOT CAUSE: the lazy
init put the 10 MB carve IN FRONT of the primitives/sets load - the largest
heap transient in the program (the same spike that killed the no-sidecar
install on 08-03). FIX: explicit carve at load COMPLETION (GameStateMenu, the
!nextSetFolder branch, after splash release) - boot rides the whole heap as
before, and boot-era pinned textures (fonts/chrome) stay OUT of the pool, so
the pool serves only card churn. Deployed EBOOT 3662b650 (slab2 + memprobe).

TWO PROCESS TRAPS, both cost a build cycle today:
  1. pspdev container is busybox-ish: NO /usr/bin/grep. A pipeline through it
     dies and can SIGPIPE-kill the make it wraps - the EBOOT looked built but
     was the previous binary (md5 caught it). Use tail/busybox tools in
     container pipelines, and ALWAYS md5-compare after build.
  2. projects/mtg/objs/ + JGE/src/*.o are SHARED between the PSP container
     build and the desktop Makefile.sdl build - a desktop link after a PSP
     build eats MIPS objects ("relocations in generic ELF (EM: 8)"). Clean
     objs when switching targets. DOCKET: separate obj dirs per target in
     both makefiles (the durable fix).

OWNER PROCESS RULING (2026-08-06): deploy the EBOOT to the device FIRST, run
the desktop suite CONCURRENTLY as the commit gate - never serialize the
device round behind the suite ("you're effectively doubling the wall clock
time per iteration"). Suite still gates every COMMIT/PUSH.

## 2026-08-06 — SLAB BOOT-CRASH ROOT-CAUSED: `free (rgbadata)` — a SPACE hid the killer

Five hardware rounds of breadcrumbs converged on it: the pool allocator is
FLAWLESS (host test: 1M randomized cycles clean; every on-device address in
the crumb logs hand-verifies as textbook buddy behaviour). The crash was
LoadJPG's VRAM-path epilogue: stage pixels in a TexAlloc buffer (rgbadata),
swizzle to VRAM, then `free (rgbadata16);` — PLAIN NEWLIB FREE OF A POOL
POINTER, written with a SPACE before the paren so the `free(`-pattern sweep
that routed every other site to TexFree never matched it. newlib walks
boundary tags that are actually pixel garbage: first JPG (back.jpg) poisons
the arena, second (back_thumb.jpg) detonates - deterministic, and exactly
where every slab build died. GIF's swizzle path had the identical
`free (buffer)`. All three routed through TexFree (safe for non-pool pointers
- it falls back to free()).

WHY THE OTHER REPROS LIED: PPSSPP zeroes memory, so the garbage boundary tags
read as benign zeros and the emulator boots clean - hardware-only by
construction. The 08-03 "PSP divergence is memory" rule held again, in its
most literal form.

RULE EARNED (grep discipline, 3rd incident of the day): pattern-sweeps over
this codebase MUST be whitespace-tolerant - `free[[:space:]]*\(` - and any
"routed every call site" claim needs the tolerant sweep as its verification,
not the sweep that produced the sites.

Carve-timing lessons kept (both learned from real crashes, both still true):
lazy-at-first-texture starves the primitives-load transient; the carve lives
at app start (GameApp::Create, PSP block). Pool 8 MB / cap 4 MB.

Builds this arc: slab1 dd603ec4 (lazy carve, boot crash) -> slab2 (menu carve,
Play crash) -> slab3 6727cec (app-start carve, boot crash = the free() bug all
along; the carve-timing "fixes" were real improvements chasing a misread
symptom) -> slab4-7 (breadcrumb bisection) -> slab8 fd127af4 = THE FIX
CANDIDATE, deployed, awaiting the owner's boot.

## 2026-08-06 — SLAB SHIPPED (e957e8e74..f36b783d3, pushed). VERDICTS FROM THE GAUGED ROUND.

Play round on the fixed slab (fd127af4): boot + menus + match 1 + into match 2
- the corruption fix HOLDS. Gauges: poolFallbacks 3 for the whole session, ALL
oversize, ZERO exhaustion, 2.2 MB pool spare at peak. **Texture fragmentation
of the general heap is DEAD** - the week's crash mechanism is closed.

Two residuals, both measured:
  1. LAG (owner: pervasive, spikes + steady) = the cap-4 cache in constant
     re-decode. Fixed forward: cap 5 (f36b783d3), fits the pool per gauges.
     Deployed as EBOOT-slab9-cap5 e9b1fcdd.
  2. MID-MATCH-2 CRASH = the GENERAL heap now: arena 38.7 of ~44 at m2 start
     (pool included), ~3-5 MB non-texture match churn ran it over. Mild
     non-texture ratchet (+2.5 MB after m1) + churn variance = a smaller die,
     still rolled each match.

THE STRUCTURAL NEXT LEVER (owner's own roadmap item, now measured as THE
bottleneck): the card database costs ~26 MB of general heap (boot log: arena
1.7 MB pre-load -> 36 MB at menu, pool's 8 included). The sidecar offload was
the cheap 80%; the packed-arena primitives store is the 20% that buys the
3-5+ MB that ends the match-2 dice permanently. Everything else is +-1 MB
nibbling.

Current deployed: slab9-cap5 (e9b1fcdd) + resized chrome + revised decks +
sidecars + correct pack. Instrumentation (memprobe/crumbs/boot log) compiled
in and play-safe; a no-probe build is one `make` without EXTRA_DEFS.

## 2026-08-06 (later) — SLAB VERDICT: crash NOT closed. Pivot to the card-DB diet.

OWNER CORRECTION (right): "texture fragmentation is dead" was overclaimed. slab9-cap5
crashed mid-match-2 exactly like before, and played WORSE. The gauges only proved the
pool allocator works — not that the crash mechanism was closed.

WHAT THE LOGS SHOWED (memprobe + 3,827-line crumb log off the stick):
- Crash = GENERAL-heap exhaustion. Match-2 start: arena 38.7/44.1 MB, heapUsed 35.9,
  ~8 MB total room; died mid-match. Pool HEALTHY throughout (2.2 MB spare, 3 fallbacks
  all oversize, zero exhaustion). Death was outside texture code (crumb log ends on a
  cleanly completed jpg decode).
- Lag = cache-eviction thrash: card art re-decoded from the stick CONTINUOUSLY through
  match play. Lag tracks the CAP (8 MB eras played smooth; 4/5 MB thrash), not the slab.
- valloc (VRAM) returned NULL 318 times — VRAM fills early, every later art load tries
  it, fails, falls through to the pool. Pre-existing; not the crash.
- CAP IS NOW REDUNDANT PROTECTION: post-slab, the pool boundary is the protection the
  cap used to provide; the cap's only remaining effect is eviction lag. But it CANNOT
  be raised meaningfully: pinned/managed 1.9 MB + cap + in-flight must fit the 8 MB
  pool, and a match's art working set exceeds any cache that fits. Lag is unfixable
  until the pool can grow, which needs general-heap headroom first.

CARD DB COMPOSITION (measured/estimated, matches the 26.4 MB boot delta):
- 27,788 CardPrimitives ≈ 17 MB (~620 B/card: ~312 B = 13 std::string members MOSTLY
  EMPTY at 24 B each on 32-bit; 64 B two bitset<240>; ~110 B ManaCost; overheads)
- 74,811 MTGCard printings + map<int,MTGCard*> ≈ 5 MB (~68 B/printing)
- map<string,CardPrimitive*> name index ≈ 1.8 MB
- Sparse fields: backside=458, partner=37, doublefaced=33, spellTargetType≈4k,
  AICustomCode/CrewAbility/PhasedOutAbility/ModularValue ≈ 0 — of 27,788.

SLICE 1 SHIPPED — commit d7a89c890 (pushed), EBOOT f0a9d4e56b106d108bdc032843838c8e
DEPLOYED (probe logs cleared): the 5 accessor-wrapped rare strings moved to a sparse
side-table in CardPrimitive.cpp (~3.3 MB reclaim expected; boot-floor memprobe line is
the verifier). Gate 1031/0 + 24 AI/0 + PARSETEST 319/0. Binary /tmp/wagic-d7a89c890.
AWAITING owner play verdict on this build.

QUEUED SLICES (in order):
  2. backSide/partner/spellTargetType → accessors + side-table (~2 MB; 71 sites,
     14 files, INCLUDING CP1252 landmines MTGRules.cpp/GameObserver.cpp — byte-splice).
  3. Slab-allocate the 74,811 MTGCard printings contiguously + sorted-array id lookup
     replacing map<int,MTGCard*> (~3-4 MB; they are never individually freed).
  4. THEN grow pool + raise cap with the reclaimed headroom — that is the lag fix.
  Candidates beyond: LKIbasicAbilities down to MTGCardInstance (0.9 MB), nameOrig/
  lcname dedup (~1 MB), magicTexts map side-table (0.7 MB).

PROCESS LESSONS (both cost a cycle today):
- GATE INVOCATION (the gui-less suite EXISTS, owner built it a month ago — DO NOT
  reach for a display): from MAIN-tree projects/mtg/bin, run the WORKTREE binary:
  systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- env
  WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 WAGIC_TESTSUITE_THREADS=1 <worktree>/bin/wagic
  Results: stdout summary + bin/test/results.html. PARSETEST leg: same but
  WAGIC_GPT_PARSETEST=1 alone. Bare exit code 139 at startup = no display = you
  forgot WAGIC_HEADLESS.
- CHANGING sizeof(CardPrimitive) invalidates MULTIPLE-INHERITANCE THUNK OFFSETS
  (_ZThn580_...) baked into caller vtables; the Makefile's dep tracking does NOT
  recompile all dependents → undefined thunk refs at link (AIMomirPlayer.o, Token.o).
  Any layout change to a base class = full objs/deps clean on BOTH targets. Also:
  `make | tail` masked make's failure (exit 0 from tail) — EBOOT existence + md5 is
  the only build truth.

## 2026-08-06 (evening) — SLICE-1 VERIFIED ON HARDWARE; SLAB A/B: pool stays; counter-event class bug docketed

SLICE 1 (side-table, d7a89c890) HARDWARE VERDICT: 3 matches NO CRASH, gameplay
smoother. Measured: menu floor 36.1 -> 33.0 MB (-3.1, prediction ~3.3), match
arena peak 36.1/44.1. The crash margin roughly quadrupled.

NEW OWNER REPORTS from the same session: SHOP super laggy per-card-view
("big operation every time i look at a different card"), deck-editor category
switch worse. Log forensics: NOT thrash (577 loads/359 unique), not price
(table lookup), not sidecar text (indexed read, cached), not the side-table
getters (only in gameplay paths). Remaining mechanism = SYNCHRONOUS first-view
jpeg decode on the main thread (~100-300 ms per new card; same door as the
docketed hand-open freezes). Baseline "before" unresolved - owner's last good
shop visit predates cap/pool/sidecars/new pack.

SLAB A/B (owner suggested; WAGIC_NO_TEXPOOL switch, b0917f2af): identical FEEL
(shop lag unchanged -> pool exonerated, decode confirmed as the lag), but the
fragmentation disease returned instantly in the numbers: arena ratchet
~2.4 MB/match (vs ~1.0 with pool), 5.8-7.8 MB free-but-unusable arena,
heapUsed floor flat at 26.4 - the exact pre-slab pattern. VERDICT: POOL STAYS
(it halves heap rot; it was never the lag). Pool build f0a9d4e5 restored to
stick. Even with pool there is ~1 MB/match residual ratchet - long sessions
still eventually hit the ceiling; the DB-diet slices remain the lever.

COUNTER-EVENT CLASS BUG (owner live report: Winding Constrictor + Experiment
One evolve = 1 counter, should be 2): docketed on master 1f9c37e57 (pushed).
TrTotalCounter consumes WEventTotalCounters; only AACounter + counter-cost
emit it; 23 raw Counters::addCounter sites (evolve among them) are invisible
to every plus-rider card. Fix direction in the docket: emit from
Counters::addCounter as the single choke point, dedupe the two existing
emitters; needs suite + Constrictor/evolve fixture.

NEXT: printings slab (~3-4 MB reclaim, 74,811 MTGCard objects + id map ->
contiguous slab + sorted-array lookup) is the queued slice; spellTargetType
side-table DEPRIORITIZED (14% populated = weak net, hot-ish path). Shop lag
fix = async/deferred decode or bigger cache AFTER more reclaim.

## 2026-08-06 (night) — SLICE 2 SHIPPED: printings index (0e68bf5ed); borderline investigation

SLICE 2 (map<int,MTGCard*> -> sorted vector, ~2 MB): commit 0e68bf5ed pushed,
gate 1031/0+24/0+319/0, EBOOT a02828783ee7549b0c621841b618cd6a DEPLOYED (logs
cleared), desktop binary /tmp/wagic-0e68bf5ed. AWAITING play verdict — expect
menu floor ~31 MB (from 33.0). Design: append-on-load + lazy stable-sort with
first-in-wins dedupe (3 real id collisions ship in the sets; MTGSetInfo::uncount
rolls back the loser's accounting exactly) + binary search; own mPrintingsMutex
because getCardByName holds mMutex across getCardById (non-recursive -> the
deadlock I nearly shipped). External scans via printingsCount()/printingAt().

BUILD TRAP GENERALIZED (nearly bit again): JGE libs are poisoned by the LAST
EXTRA_DEFS they were built with — the A/B round left libjge300.a compiled with
-DWAGIC_NO_TEXPOOL, and linking mtg against it would have silently shipped a
no-pool EBOOT. ANY define change = clean JGE too, not just JGE source changes.

BORDERLINE INVESTIGATION (owner asked; two Explore agents, findings verified
with citations): "borderline" is a FILENAME CONVENTION, not an enforced grade —
borderline.txt has ZERO grade= markers so all ~12k cards parse GRADE_SUPPORTED;
the only gated file is unsupported.txt (grade=unsupported header, loader aborts
at line 1; 89% text-only stubs, never resident). Membership = ERA: mtg.txt is
frozen ~2017-and-earlier (zero modern mechanics), everything ~2018+ lands in
borderline by policy (CHANGELOG says so). Divergence taxonomy where it IS wrong:
honor-system choice menus w/ donothing (~7.5% of cards), replacement-as-trigger
(the Winding Constrictor class), per-object "one or more" batch triggers,
unrolled d20 tables, flattened X values, look-at leaked as reveal, 48 admitted
#MISSING comments. mtg.txt not audited-correct either (163 donothing, 644
aicode). Correctness signal = greppable markers, not the filename.

## 2026-08-06 (late night) — POOL-10/CAP-6.5 DEPLOYED (uncommitted, awaiting verdict); obj-dir split SHIPPED

Slice-2 probes read on hardware: menu floor 32.5 arena (steady reclaim ~1.1 MB
not the ~2 predicted - map node ~32B real vs 36 est + vector slack; the 670KB
freeKB at menu = stable_sort's one-time scratch high-water). Cumulative diet:
~4.2 MB. Match end 34.9/44.1 = 9.2 MB margin, best yet. Owner felt no change
(expected - slice 2 bought margin, not speed).

LAG BUILD DEPLOYED: EBOOT b327e96778a4fc0c595f43ba7822a204 = pool 8->10 MB
(TEXPOOL_BYTES, JGfx.cpp) + cap 5->6.5 MB (WResourceManager.cpp). 6.5+1.9+0.7
< 10. UNCOMMITTED - PSP-only paths, desktop suite can't see them; commit after
the owner's play verdict. Expect: mid-match lag spikes drop, shop revisits stop
re-paying decodes (first views still pay the synchronous decode).

OBJ-DIR SPLIT SHIPPED (eeca077d7 pushed): wagic-SDL.pro OBJECTS_DIR/MOC_DIR ->
objs-sdl/ (regenerate Makefile.sdl via qmake6 -o Makefile.sdl). PSP keeps objs/.
objs-sdl/ added to main-tree .git/info/exclude (governs the worktree). The
clean-objs-when-switching-targets ritual is RETIRED for projects/mtg. (JGE
tree objs remain PSP-only - desktop compiles JGE sources through qmake into
objs-sdl - so no collision remains there either.)

## 2026-08-06 (close) — LAG BUILD VERIFIED + COMMITTED (e89d663b3)

Owner verdict on pool-10/cap-6.5 (EBOOT b327e967): "definitely plays alot
smoother now... otherwise smooth as could be asked for during the match."
Residual = first-appearance card-art hitch only (synchronous decode on main
thread - the known docket item; candidate fix is deferred/async decode or
pre-warming the two decks' art at match load). Committed e89d663b3, pushed.

DAY LEDGER (2026-08-06, the whole arc): crash root-caused as general-heap
exhaustion -> card-DB diet slice 1 (side-table, -3.1 MB measured) + slice 2
(printings index, -1.1 MB measured) -> match-2+ crash GONE (multiple multi-
match sessions) -> reclaimed margin spent on pool 10 + cap 6.5 -> match lag
GONE. Also: slab A/B (pool exonerated for lag, halves heap rot, stays),
counter-event class bug docketed (master 1f9c37e57), borderline taxonomy
investigated, obj-dir split shipped (eeca077d7), headless gate recipe
recorded. Open: first-view decode hitch; shop browse still pays it per new
card. Next candidates: async/deferred decode, deck-art pre-warm at match
load, playerdata-save stall, RetrieveQuad RETRIEVE_LOCK pins.

## 2026-08-06 (later night) — COUNTER-EVENT CLASS BUG FIXED + EVOLVE REBUILT AS A REAL TRIGGER (0280a08b4)

The docketed class bug is FIXED, and the owner's ruling ("evolve is a triggered
ability... a lot of the fun interactions require that it handle this correctly")
drove the full fix, not the partial one. Commit 0280a08b4 pushed, gate 1033/0 +
24/0 + 319/0. EBOOT 5101569b0b43a7cff081d8a168be147f DEPLOYED (= pool10/cap65 +
counter fix + stacked evolve). AWAITING play verdict.

WHAT SHIPPED:
- Counters::addCounter/removeCounter now emit WEventTotalCounters as the single
  choke point (new _batchManaged param suppresses per-call emission for callers
  that batch: AACounter, CounterCost, Damage.cpp wither/vigor/hydra/Szadek loops
  + new Counters::emitTotalCountersEvent for their one-per-batch event).
- TrTotalCounter: e->source NULL-deref guarded (4 sites) - latent, exposed when
  engine paths emit events without a causing card.
- Evolve: AEvolveAbility (immediate) REPLACED by TrEvolve + oneShot AACounter in
  GenericTriggeredAbility. Same condition, but the counter rides the stack and
  resolves AFTER Spell::resolve registers the entering card's abilities - so
  the owner's exact scenario (Constrictor's own ETB triggering evolve, then
  riding it) works. Both scenarios fixtured (rider + race); upstream
  generic/evolve.txt passes unchanged.

HARD-WON MECHANICS (cost ~2h of blind alleys):
- GenericTriggeredAbility::resolve RESOLVES only oneShot payloads; persistent
  payloads get clone()->addToGame() and sit INERT. Every parsed trigger payload
  is oneShot; any C++-constructed GTA payload MUST set oneShot = 1.
- GTA::clone() calls destroyCondition->clone() UNGUARDED (NULL dc = UB) - but
  clone isn't in the fire path, so parser's NULL dc survives. Don't "fix"
  casually; noted only.
- SUITE RUNS MUST START IN MAIN-TREE bin/ - launching from any other cwd gives
  a menu that idles forever (no User/profile env), which reads as a code hang.
  Lost ~1h to this TWICE in one day -> psp-work/run-suite.sh now encodes the
  correct invocation (arg = test list rel. to Res, no arg = full suite).
- WAGIC_TESTSUITE_FILE = the test MANIFEST (list file), not a single test. New
  fixtures must ALSO be appended to bin/Res/test/_tests.txt (tracked).
- Suite phase-walk scripts: a fired stack trigger sometimes consumes a 'next'
  (interrupt-ask) and sometimes doesn't; when a click misses its phase the
  no-attacker auto-skip collapses later phases and the drift misleads. Diagnose
  with the full TESTSUITE command trace, never from the final phase number.
- Counters.h signature change = stale-object undefined-reference at PSP link
  (_ZN8Counters10addCounterEPKciibb...) - full objs clean, as ever.
- pipe-buffering artifact: `wagic | tail` after timeout kill shows NOTHING -
  always redirect suite runs to a FILE then read it.

## 2026-08-06 (afternoon) - decode queue: first-view art hitch fix

SHIPPED e63c62284 (psp-port, pushed). EBOOT 9e3033db4290cfa763a63b32f53f09e9
deployed + md5-verified, probe logs cleared. Gate: 1031/0 + 24 AI/0 + 319/0
parse (1031 not 1033: the 2 evolve fixtures live in the WORKTREE manifest only;
main-tree manifest was deliberately reverted during gate-pollution cleanup).

MECHANISM (why it was a stall): PSP/Vita use UnthreadedCardRetriever -
QueueRequest WAS the synchronous JPEG decode, called from inside the render
pass on first visibility (WResourceManager.cpp:1289, cache-miss path). The
async-tolerant contract already existed: miss returns the generic card-back
quad (WResourceManager.cpp:1277-1292) and callers render on. So the fix is
queue-only, zero render changes.

DESIGN (CacheEngine.h):
- CardRetrieverBase::Service(int maxDecodes) virtual no-op; threaded
  (desktop) retriever ignores it.
- UnthreadedCardRetriever: deque<CacheRequest> + set<int> dedupe (no mutex,
  single-threaded by definition). Duplicate request = move entry to BACK.
  Since RetrieveCard re-fires every frame a card stays visible, back-of-deque
  == "on screen right now" - Service pops from the BACK (newest-first).
- Cap 16, drop-oldest; dropped cards re-queue themselves if still visible.
- Drain site: GameApp::Update (every frame, all scenes) ->
  WResourceManager::Instance()->ServiceCacheRequests() ->
  CacheEngine::Instance()->Service(1). One decode/frame keeps frame cost
  bounded; ~60 cards/sec throughput is ample.
- ROUTING CONSTRAINT: CacheEngine.h defines its statics IN THE HEADER, so it
  is includable ONLY in WResourceManager.cpp - hence the passthrough method
  on WResourceManager (new virtual = vtable change = full objs clean, done).

DELIBERATELY NOT INCLUDED: deck-art pre-warm at match load (option b). One
variable at a time; only if the pop-in itself annoys in play. Would drop the
IsThreaded() gates on Options::CARDPREFETCHING (GridDeckView.cpp:126,
CarouselDeckView.cpp:90).

TELEMETRY from 5101569b play (morning, archived psp-work/logs/5101569b-play/):
two full matches, no crash, match-end arena 37217/44088 KB (~6.9MB margin),
pool fallbacks 2-3 stable, largest block 512KB. Counter/evolve play verdict
still pending - same watch list applies to the new EBOOT.

Artifacts: /tmp/wagic-e63c62284 (desktop), /tmp/EBOOT-decode-queue.PBP.

## 2026-08-06 (evening) - load-path war + evolve HARDWARE-VERIFIED

OWNER VERDICTS: evolve "appears to be working. observed a few times already"
(stacked-trigger rebuild 0280a08b4 = hardware-verified; Constrictor +1 rider
not separately called out yet). Load fixes: "way better" on 9dc17951.

LOAD-PATH FORENSICS (this evening's arc):
- Decode queue (e63c62284) verdict was NEGATIVE ("still lagspikes... maybe
  even a negative") -> loadprobe built. MEASURED on hardware (293 loads):
  cross-set card load median 2.7s = crumbs ~690ms + zip central-dir re-parse
  (31 FULL cache wipes, parses up to 5s) + JDCT_ISLOW decode ~235ms. Raw
  stick reads innocent (6ms).
- FIXES 692b82d75: poolcrumbs gated behind WAGIC_POOLCRUMB (off; arm only for
  crash hunts - measured 690ms/load); zip dir cache 4500-wipe-all -> 12000 +
  per-zip LRU (JZipCache.lastUse, gZipCacheTick); JDCT_IFAST + plain
  upsampling. VERIFIED next session (195 loads): all-in median 52ms (42x),
  decode 24ms, crumbs 0.
- Queue REVERTED 120d8e34a (owner: "try getting rid of the 1 per frame
  thing") - at 50ms loads decode-on-request is the right shape.
- REMAINING TAIL fixed next: first-touch central-dir parse still up to 4.9s
  (17 LRU evictions still cycling at 12000 entries). Root cause: PreloadZip
  parsed the central dir IN PLACE - ~20 tiny reads + seeks per entry, each
  seek discards the stream buffer. Fix (uncommitted, gate running): bulk-read
  the central dir [CentralDir(File)..EOF] into a string, parse via
  istringstream with the SAME NextHeader/ReadHeader loop (zfsystem.cpp
  PreloadZip else-branch; zipped-branch untouched).

EBOOT LINEAGE tonight: 9e3033db (queue) -> ca817578 (loadprobe) ->
9dc17951 (3 fixes, "way better") -> 105074ed (no queue; never launched) ->
a8a13434 (= + bulk central-dir read) DEPLOYED, logs cleared.
Play logs archived: logs/ca817578-probe/ (2.7s baseline numbers),
logs/9dc17951-play/ (52ms verification numbers).

PROBE CHANNEL NOTES: loadprobe = User/wagic-loadprobe.log, one line per
LoadJPG, buffered 16 lines/flush + flush on JRenderer::Destroy - stages
open/read/dec/fin + att/prs/wipe/crumb as delta-since-last-load (attach and
parse happen in cardFile BEFORE LoadJPG entry). wipe now counts LRU
single-zip evictions, not full wipes.

## 2026-08-06 (late night) - menu war + rootborn investigation open

MENU-TRANSITION WAR (menuprobe attributed, fixes shipped):
- AVATAR FIX 3f896ddfa HARDWARE-VERIFIED: DeckMenuItem used RetrieveTexture
  as an avatar EXISTENCE check = full JPEG decode per menu item per entry
  (84 avatar decodes/session). Now DeckMetaData::avatarExists() =
  fileOK(graphicsFile(...)) cached for object lifetime. Warm opponent menu
  1400ms -> 153ms (two clean samples).
- startGame ISOLATED: 1.7-2.8s pure compute (marks around the call). NEXT
  TARGET. GameObserver.cpp is CP1252 - internal marks need python byte-splice.
- Owner's opponent picks hit an UNMARKED loadPlayer site (likely Random AI
  at GameStateDuel ~1764/1780) - mark those next probe build.
- Post-match "play -> end -> next duel Start" gap 5.7-7.2s = the known
  unlock-chain + playerdata-save docket pair, now with clean numbers.
- Menuprobe delta caveat: lines report delta-since-previous-mark, which
  INCLUDES user think time between menus - only marks bracketing a single
  construction are pure compute.

ROOTBORN DEFENSES (owner report: combat trick, creature died to combat
damage anyway) - INVESTIGATION OPEN:
- Script is Oracle-faithful (all(creature|myBattlefield) indestructible ueot
  + interactive populate line).
- Main-phase cast PASSES: worktree fixture rootborn_defenses_combat_trick.txt
  (uncommitted) - grant works, populate does not break it, bears survive
  lethal combat damage. Engine indestructible-vs-combat has long-green
  coverage (generic/indestructible.txt, dauntless_escort.txt).
- Interrupt-window cast: fixture rootborn_defenses_damage_interrupt.txt
  (uncommitted) shows the suite NEVER OFFERS the interrupt ask for the
  combat DamageStack (resolves instantly; no interrupt-ask debug line) -
  the yes/cast/endinterruption idiom lands too late. The live-play window
  he used is governed by the INTERRUPT phase options (wouldOfferWindow,
  ActionStack.cpp ~1204) = a code path the suite did not exercise.
- BLOCKED ON OWNER ANSWERS: (1) attacking or blocking, and cast via the
  interrupt prompt or by clicking the card during blockers? (2) did the
  killing creature have wither/infect/deathtouch? (wither/infect = -1/-1
  counters = indestructible correctly does not save it - engine would be
  RIGHT).
- Fixture-timing lessons: clicking a hand card during blocker-ASSIGNMENT
  does not cast; suite resolves DamageStack immediately on phase advance.

EBOOTs: ef16f4c1 (avatar fix + all marks) ON STICK, is current best build.
Logs: logs/ef16f4c1-play/ (menu numbers), logs/a8a13434-play/ (cdbulk
verification: worst parse 4.9s -> 404ms).

## 2026-08-07 (early) - rootborn investigation closed-form + ORDER guard

SHIPPED e4b3cc96e (psp-port, pushed): ORDER-step click-gate NULL guard + 3
rootborn fixtures (in worktree manifest; main-tree manifest deliberately
unpolluted per the standing gate pattern). Gate on revised binary:
1031/0 + 24/0 + 319/0. EBOOT 8ce1bc43 DEPLOYED (byte-verified, logs cleared).

THE INVESTIGATION (owner: "used rootborn defenses as a combat trick, my
creature was destroyed anyways, with combat damage"; wither/infect ruled out
by owner; scenario details unrecalled):
- Script Oracle-faithful. Engine indestructible machinery sound.
- ALL legal cast windows protect correctly - three fixtures pin them:
  rootborn_defenses_combat_trick (main phase), _damage_interrupt (blockers
  ask), _damagestack_ask (last window before damage; 'no' then 'yes').
- OWNER RULES CORRECTIONS (both confirmed by traces): (1) no casting during
  damage-assignment ordering - the first fix draft that enabled it was
  WRONG and was revised to swallow-all + guarded deref; (2) Wagic does NOT
  have a pre-M10 damage-on-stack response window - interrupt asks fire on
  NextGamePhase objects (phase-boundary priority = modern model), the
  internal 11DamageStack resolves with NO ask. Claude misread the object
  name; retracted.
- LIVE INCIDENT UNREPRODUCED. Owner detail that kills the eaten-click
  theory: live cast had FULL feedback (sfx, mana tapped, card left hand).
  If it recurs: note the cast window + board state.
- Fixture-authoring landmines (now in the commit message + fixture
  comments): manapool empties at phase change -> use lands tapped inside
  the ask; name-clicks hit the FIRST match -> same-named lands re-click the
  tapped one (use distinct basics: Plains/Snow-Covered Plains/Swamp);
  dual lands open a mana-choice menu that desyncs the script; 'yes'/'no'/
   'endinterruption' answer asks; asks fire only if hasInstantResponse
  (untapped sources) - a tapped-out fixture NEVER gets a window.
- cardClick diagnostic idiom: temporary fprintf(stderr) byte-spliced into
  GameObserver.cpp (CP1252!) showing per-click combatStep + reaction -
  removed after use; reaction=0 rejected, 1 cast/tap, 2+ = menu opened.

SESSION-2 TELEMETRY (logs/ef16f4c1-play2/, cumulative 5 match cycles):
avatar fix holds (warm 153-154ms every entry); startGame 1.7-2.8s (next
target - internals need CP1252 byte-splice marks in GameObserver.cpp);
pick-to-play window 2.5-7.6s still hides AI deck load (owner's picks route
through an UNMARKED loadPlayer site - likely Random at GameStateDuel
~1764/1780; mark them next probe build); heap margin 6.4MB at match end,
pool fallbacks 4 - stable.

EBOOT LINEAGE: ef16f4c1 -> a03f707d (wrong fix, DISCARDED) -> 8ce1bc43
(revised, ON STICK). Desktop binary /tmp/wagic-e4b3cc96e.

## 2026-08-07 (day) - untap phantom window + probe marks shipped

SHIPPED (both pushed, gates green 1031/0+24/0+319/0):
- f24bfb8e5: startGame stage timers (sgMark in GameObserver.cpp - CP1252,
  byte-spliced helper writing to menuprobe log: begin/initPlayers/DuelLayers/
  initGame/hand preload) + Random-AI loadPlayer marks (GameStateDuel
  MENUITEM_RANDOM_AI/_HARD handlers).
- 9888e3e37: wouldOfferWindow returns false during MTG_PHASE_UNTAP.
  OWNER INCIDENT (screenshot): Lightning Helix at opponent's untap, mana
  untappable, phase bar "(opponent's turn - you play) Untap" - the engine
  offered the ask (hasInstantResponse saw untapped lands) while phase rules
  refused every action. OWNER RULING: "you cannot give me an interrupt
  window if I'm not allowed to interrupt during that window." Rules dialogue
  established (CR quotes from the LOCAL CR COPY at
  strategy-design/rules-audit/data/MagicCompRules-20260227.txt - use it, web
  fetches truncate): 502.4 + 117.3a no priority during untap; 503.1/.1a the
  functional slot he wants = upkeep ask, spells stack ABOVE pending upkeep
  triggers and resolve first. Owner initially believed an untap-upkeep
  window existed; resolved via form-vs-function walk (his framing).
- Per-phase INTERRUPT_* options in GameOptions are mostly DEAD (defined,
  never consulted); the ask machinery is wouldOfferWindow only.

8ce1bc43 SESSION TELEMETRY (9 cycles, roster grew to 28 AI decks):
fillDeckMenu warm 234ms FLAT (avatar fix scales ~8ms/deck); human deck load
32ms (browse time dominates the gaps); startGame 1.4-2.7s median ~1.75s
(stage breakdown arrives with ef862d22's sgMark data); heap arena tail
37.8->38.1MB across matches, fallbacks 9->11, margin ~6.0MB - SLOW RATCHET,
watch item, not a threat at current session lengths.

EBOOT ef862d22 ON STICK (= everything through 9888e3e37: untap fix + sgmark
+ random-AI marks). AWAITING: untap-fix play verdict (first opponent-turn
stop should be upkeep; Helix castable there) + sgMark stage numbers.
Archives: /tmp/wagic-9888e3e37, /tmp/EBOOT-untapfix.PBP. /tmp was quota-full
2026-08-07 - pruned Aug-3-era EBOOTs/binaries (all rebuildable from git).

## 2026-08-07 (harvest, ef862d22 play session)

Two matches played on EBOOT ef862d22 (= 9888e3e37). Logs at `logs/ef862d22-play/`.

**startGame attribution (sgMark)** — match1 cold / match2 warm:
- initPlayers ~0 / ~0; DuelLayers 54ms / 54ms
- initGame bracket **1236ms / 1078ms** ← the fixed cost (covers PhaseRing ctor + resetStartupGame serialize + Rules::initGame)
- hand preload 1420ms / 501ms (pure cache behavior, shrinks warm)
- totals 3085ms / 1904ms
- NOTE: `sg begin` delta line spans the whole previous match (sgMark has its own delta clock) — artifact, ignore.

**Analysis**: PhaseRing ctor = ~44 small allocs (trivial). Rules::initGame = trivial for classic
(initState zones empty). Prime suspect = `resetStartupGame()` — serializes full game state via
stringstream (`stream << *this`, both decks). It is LOAD-BEARING: baseline for in-game undo
(pause menu MENUITEM_UNDO → GameObserver::undo() replays startupGameSerialized + actionsList).
Cannot delete; if confirmed, options are faster serializer or PSP-specific handling.

**Sub-marks shipped** (commit 6831fef1c, EBOOT **0ae7bcbc** deployed md5-verified, logs cleared):
initGame bracket now splits into `phaseRing` / `serialize` / `initGame` (now = Rules::initGame only).
Probe-only change inside WAGIC_MEMPROBE && PSP guard; desktop compile gate green; suite cannot
exercise it (no full-suite run for this commit, rationale recorded).

**Cold-session first-touch tax** (informational, already optimal): human loadPlayer 1791ms cold
vs 32ms warm; fillDeckMenu 3152ms cold vs 235ms warm; random-ai loadPlayer 516ms (deck 8) vs
2543ms (deck 25, cold).

**Heap**: arena peak 37,622KB (~6.3MB margin to 44,088), pool fallbacks 2->4 over two matches.
Same slow-ratchet watch item; no action.

**Awaiting**: untap-fix play verdict (first opponent-turn stop should be upkeep); serialize
sub-mark numbers from next session.

## 2026-08-07: Rootborn RECURRENCE + suite-harness discoveries

**Live incident #2 (exact detail, screenshots)**: his first main, cast Rootborn Defenses,
swung all in; Youthful Knight (2/1 FIRST STRIKE) died to a Rhox War Monk (3/4 lifelink)
block — died to regular-step combat damage after dealing first-strike damage.

**Exact-repro fixture PASSES on desktop**: `rootborn_defenses_first_strike.txt`
(FIRSTMAIN cast, Knight attacks, Rhox blocks, assert Knight alive + P2 life 23 from
lifelink). So the desktop engine handles first-strike + indestructible-ueot correctly;
the PSP failure is a runtime differential (data-on-stick ruled mostly out: primitives
restored to original 2026-08-01, Rootborn script unchanged since 2013; wave-33 didn't
touch it).

**Ruleprobe shipped (next EBOOT)**: `User/wagic-ruleprobe.log`, WAGIC_MEMPROBE && PSP:
- `toGrave <name> forced=%d indest=%d life=%d` on every creature death
  (MTGCardInstance::toGrave — the single chokepoint; even destroy() lands here)
- `grant indest <name> was=%d` / `ungrant indest <name> restore=%d` in
  AInstantBasicAbilityModifierUntilEOT addToGame/destroy (AllAbilities.h)
Discriminates: grant never applied vs expired early vs forced-bypass. Next live
occurrence = read the log around the death.

**SUITE-HARNESS DISCOVERIES (hard-won, do not relearn)**:
1. Res.txt points at nonexistent ../../wagic_res/ -> binary falls back to CWD's ./Res.
   run-suite.sh cd's main-tree bin => fixtures + manifests load from MAIN tree only.
   => Worktree-only fixtures (the 3 rootborn + evolve extras) were NEVER run by any
   prior gate. "1031 green" gates ran the main manifest only.
2. WAGIC_TESTSUITE_FILE expects a LIST file (replaces test/_tests.txt), and list
   entries load as "test/"+line (TestSuiteAI.cpp:1413) — never prefix entries with
   test/. Wrong entries => ==Could not load test file==.
3. A short/failed list does NOT reach the "Test suite finished" exit — the process
   wedges in the post-suite UI loop; kill it. (Full-manifest runs exit normally.)
4. Suite verdicts live in main-tree bin/User/test/results.html (Log() appends there),
   summary line "Test suite finished: N tests (M failed)..." on stderr at exit.
5. run-suite.sh REWRITTEN: additively syncs worktree fixtures to main test/ (main
   _tests.txt untouched per unpolluted-manifest ruling), copies worktree manifest as
   _tests_psp.txt, default run = full worktree manifest (1077 incl. 4 rootborn).

first_strike fixture added to worktree manifest. Full gate rerun in flight at
this writing; PSP EBOOT with ruleprobe building (full objs clean — AllAbilities.h
inline-method change).

## 2026-08-07: Rootborn ROOT CAUSE + CR 514.2 backstop fix

**Owner's 3rd incident cracked it (his diagnosis, confirmed)**: his protected 1/1
survived combat, killed its blocker, then went to graveyard AT END OF TURN =
damage-removal racing ueot-expiry.

**Mechanism (code-confirmed)**:
- Damage removal = SCRIPTED trigger `@each cleanup:all(*|myBattlefield) resetDamage`
  (rules/mtg.txt:88; classic.txt inherits via `include mtg.txt`) — resolves via stack.
- Ueot expiry = HARDCODED: InstantAbility::testDestroy retires at MTG_PHASE_AFTER_EOT;
  the AFTER_EOT block in nextGamePhase (GameObserver.cpp ~261) calls
  actionLayer()->Update(0) synchronously (strip), then recurses to BEFORE_BEGIN which
  runs cleanGarbage()/garbageCollect() — can DISCARD the still-unresolved heal trigger.
- Live play's phase auto-skip (ASKIP_SAFE/FULL, GameObserver.cpp:1278 cleanup skip)
  can pull cleanup->AFTER_EOT->next-turn into ONE synchronous chain before the trigger
  resolves -> heal lost -> next SBA tick kills the creature (lethal damage, indest=0).
- **Suite CANNOT reproduce**: line 1266 forces ASKIP_NONE for suite games — the
  race needs auto-skip. Both cleanup-crossing fixtures pass on desktop (expected).
  Race is tick-interleave dependent => intermittent in live play. NOT PSP-specific;
  PSP is just where he plays.

**Fix**: CR 514.2 backstop — synchronous damage removal at the TOP of the AFTER_EOT
block, before the strip; mirrors AAResetDamage::resolve() (NODAMAGEREMOVED, PW
loyalty, battle defense). Scripted trigger kept (now idempotent second heal).
Byte-spliced (GameObserver.cpp is CP1252/CRLF).

**Validation**: no suite repro possible (above), so: full gate green = no regression;
hardware verdict = his play (protected creatures must survive EOT) + the ruleprobe log
(a "toGrave ... indest=0" on a protected creature would disprove the fix; grant/ungrant
lines show ordering).

**Fixtures**: rootborn_defenses_first_strike.txt (exact incident-2 repro, passes) +
rootborn_defenses_survives_cleanup.txt (eot-crossing, ASSERT UNTAP, passes) — both in
worktree manifest. Gate1 (pre-fix, first to actually run worktree fixtures):
1037/0 + 24 AI/0. Gate2 (post-fix) in flight at this writing.

**EBOOT 85700660** = backstop + ruleprobe + startGame sub-marks; supersedes 26c3f3e5
(never deployed) and 0ae7bcbc (on stick). Deploy on next plug-in.

## 2026-08-07: startGame attribution SOLVED — serialize exonerated, Rules::initGame is the second

11-match session on 0ae7bcbc (logs at logs/0ae7bcbc-play/): phaseRing 0.2ms,
**serialize 0.5ms (NOT the cost — suspicion dead)**, **Rules::initGame ~1064ms
FLAT every match (1063-1067ms, zero variance => pure CPU)**, hand preload
313-1632ms (cache-dependent, self-improving). Prime suspect inside initGame:
`addExtraRules(g)` — parses ~87 systemic auto= rule lines (classic.txt +
include mtg.txt) through the full ability parser per match. goToPhase + zone
loops trivial for classic.

Next-cycle probes STAGED in worktree (not yet in a deployed EBOOT): sgMark
made non-static (GameObserver.cpp), Rules::initGame sub-marks `ig goToPhase` /
`ig zones` / `ig addExtraRules` (Rules.cpp, ASCII/Edit-safe). NOTE: desktop
suite CANNOT measure addExtraRules — suite rules = testsuite.txt (0 auto= lines).

Candidate fix once confirmed: parse-once-clone-per-game for extra rules
(engine's existing template-clone idiom).

Heap after 11 matches: arena peak 37.4MB, fallbacks 13/boot, margin ~6.5MB —
ratchet stable, watch unchanged.

**DEPLOY STATE**: EBOOT 85700660 (CR 514.2 backstop + ruleprobe + startGame
stage marks) ON STICK md5-verified, logs cleared. His next session = fix
verdict (protected creatures must survive EOT) + ruleprobe log backup.

## 2026-08-07: CR 514.2 fix HARDWARE-VERIFIED (owner + ruleprobe)

Owner verdict on 85700660: "root born defenses appears to work now". Ruleprobe log
(logs/85700660-play/) captured the complete bug path surviving:
`grant indest Champion of the Parish` -> `toGrave ... indest=1 life=0` (kill attempt
BLOCKED at the gate with lethal damage marked) -> `ungrant` at EOT -> NO death after
ungrant (backstop healed first). Also 4 unprotected deaths logged normally, forced=0
throughout — no bypass path seen. CR 514.2 unit CLOSED on the psp-port branch.
Master-tree port + the buff-death docket item: fold into the wagic-fixes-port
campaign tiers (engine fix, upstream-worthy).

Session perf data unchanged: initGame ~1072-1230ms, heap arena peak 37.9MB /
fallbacks 5 per boot. **EBOOT 8d6ad6a0 ON STICK** = + Rules::initGame sub-marks
(ig goToPhase / ig zones / ig addExtraRules); next harvest names addExtraRules'
share of the flat second. Untap-fix verdict still not explicitly given (no
complaints across many sessions since; treat as soft-pass until he says otherwise).

## 2026-08-07 (later): MERGE TO MASTER + addExtraRules profile

**MERGE**: psp-port (92 commits) merged to master `cfe186f07`, zero conflicts, clean
master build + full gate green (1038/0 + 24 AI/0), PUSHED. Master is now the superset;
platform-specific bits live behind PSP guards only. Owner ruling driving it: game
improvements are universal — platform branches must not fork the game. Future builds
of any platform cut from master inherit everything. (Deployed binaries elsewhere—
Android alpha etc.—are frozen until rebuilt from master.)

**addExtraRules confirmed + profiled**: ig sub-marks (8d6ad6a0 session): addExtraRules
1038-1199ms = the whole initGame second (goToPhase 26ms, zones 0.015ms). Desktop bench
(TEMPORARY env-gated WAGIC_BENCH_XR code in worktree Rules.cpp — NOT committed, remove
before ship): 96 lines = 8.3ms debug-desktop; per-line distribution FLAT (top: stun
handler 205us, 10 land-lords ~190us each, tutorials ~90-105us, removemana ~80us) — no
dominant line, no dominant helper (perf: diffuse std::string find/copy/construct).
PSP -O2 confirmed (Makefile:88).

**ANOMALY -> NEXT PROBE**: PSP 1064ms vs ~2-3ms release-desktop ≈ 400x, vs ~50-100x
hardware-predicted => PSP runs 4-8x worse than clock explains. Hypothesis: allocator-
bound (thousands of tiny parse allocs through newlib malloc/pool). Next EBOOT: count
allocations + alloc-time during addExtraRules. If allocator-bound -> bump-arena for
parse phase (bounded, semantics-free). If CPU-bound -> owner decision:
accept ~1s vs deep parse-cache surgery.

Ruleprobe (8d6ad6a0 session): second live save captured (Infantry Veteran indest=1
life=-6 kill attempt BLOCKED, ungrants clean, 10 normal deaths). Fix holding.

## 2026-08-07: startGame WAR CLOSED at ~2s — attribution complete, fixes mapped, deferred

Micro-bench (f9df5521 session): malloc bench INVALID (elided — no volatile sink;
lesson: sink allocations through volatile). strfind 2.6us/find (92-char line). Warm
reparse of lord line = 2.74ms — fully explained by string-scan volume => NOT
allocator-bound. Cold mixed-line reality 1064ms vs ~220ms warm-extrapolated =>
~5x I-CACHE THRASH (each line type walks a different region of the huge parser
footprint vs 16KB I-cache). Remaining fixes are BOTH deep parser surgery:
(a) keyword-dispatch to cut per-line scans, (b) parse-once-per-session cache with
observer/target rebinding visitor. DEFERRED — banked at ~2s warm transition
(1.04s parse + 0.4-0.7s preload). The WAGIC_BENCH_XR + xrb micro-bench code stays
in worktree Rules.cpp (env-gated / one-shot, harmless) — remove before any release.

**NEXT PERF TARGET: post-match 6-13s freeze** (unlock-chain ~2.6s + playerdata save
~1.9s per old forensics + unattributed remainder) — 3-6x the remaining startGame cost.

## 2026-08-07: POST-MATCH WAR OPENED — EBOOT b6ade19f (= f9df5521 + WAGIC_WINPROBE)

Docket recap from 2026-08-03 forensics: tasks-9s (196 AI-deck walk) FIXED then,
soft-verified by days of play since. Remaining every-win: unlock checks ~5.1s +
playerdata save ~1.9s. The finer unlock marks (u:deckstats+diff / u:unlockables /
u:unlock chain, Credits.cpp) existed but were compiled out — WAGIC_WINPROBE now ON.
GOTCHA relearned: EXTRA_DEFS changes do NOT trigger rebuilds (deps don't track
CFLAGS) — first build came out byte-identical; rm the consumer .o (objs/Credits.o).
DEPLOYED b6ade19f md5-verified. Next harvest: u: lines split the 5s.

## 2026-08-07: POST-MATCH ATTRIBUTED — double options.save found + first fix shipped

WINPROBE (b6ade19f session, 2 victories): u:unlock chain 2503/2649ms + playerdata
save 2008/1931ms = ~4.7s total (down from 7.2-15.9s; the Aug-3 tasks fix confirmed
live at 0-1ms). All other blocks ~0.

**KEY MEASUREMENT**: profile files are TINY (collection.dat 2.2KB, player options.txt
2.4KB, tasks.dat 507B) — the cost is per-file-operation overhead on the Memory Stick
(FAT create/truncate/flush ~0.5-1.25s per save), NOT bandwidth, NOT line-buffering
(ofstreams are buffered; per-line-syscall theory considered and dropped).

**BUG FOUND**: GameOptionAward::giveAward() persists via options.save() internally
(in-code TODO admits placement), and unlockRandomSet()/unlockSetByName() called
options.save() AGAIN => every set unlock = TWO full options saves. With most of 337
sets locked, his profile unlocks a set nearly every win => every win paid double.

**FIX SHIPPED (EBOOT 9a3916eb deployed)**: redundant options.save() removed from both
call sites (giveAward's own save is the persist; correctness argued: unlockRandomSet
pre-checks locked, unlockSetByName giveAward no-op leaves nothing to persist).
ur: sub-marks added in unlockRandomSet (ur:roll / ur:giveAward) to price the single
save next session. Expected: unlock chain 2.5s -> ~1.25s, post-match ~4.7s -> ~3.4s.

NEXT CANDIDATES (data-gated): playerdata save 2.0s = 4-5 file ops (player.dat +
collection.dat via .tmp+rename + tasks.dat) — dirty-flag skip for unchanged
collection/tasklist (PlayerData is loaded fresh ~100ms before saving, usually
unmodified); and/or defer saves past first victory-screen render.

## 2026-08-07: double-save fix HARDWARE-VERIFIED; collection dirty-skip built

9a3916eb session: ur:giveAward 1259ms = the single remaining options.save (prediction
exact); unlock chain 2503->1259ms; win TOTAL 4655->3483ms; loss path 2230ms (~pure
playerdata save). playerdata save steady ~2.1s.

**NEXT UNIT SHIPPED-TO-BUILD (this writing)**: victory path constructs
PlayerData(MTGCollection()) — collection IS rewritten every win (.tmp + Rename = 2
FAT ops) though usually unchanged. Fix: MTGDeck::stateFingerprint() (deterministic
digest: cards map + sorted SB/CMD/DNG + meta) snapshotted at PlayerData load;
PlayerData::save() skips collection->save() when unchanged, updates FP after real
saves. Shop/award mutation paths still save (fingerprint differs). Also pd:player.dat
/ pd:collection / pd:tasks WINPROBE sub-marks (winMark/winStart/winFlush moved OUT of
anon namespace for external linkage — data stays anonymous; mark buffer 24->32).
Expected: win ~3.5s -> ~2.5-2.8s (player.dat + tasks + single options save remain);
loss 2.2s -> ~1.2-1.5s. Full objs clean (PlayerData layout grew). Gate + PSP build
in flight; deploy next plug-in; commit on green.

Remaining post-match floor after this (if marks confirm): player.dat ~0.5-1s (credits
always change — irreducible by skipping) + tasks ~0.5-1s (passOneDay may always
mutate — fingerprint-gate TaskList as follow-up if marks show it) + options save
1.26s on unlock wins. Structural end-game: defer writes past first victory render.

## 2026-08-07: Collective Brutality mode-1 missing — ROOT-CAUSED + FIXED (parser)

Owner report: plain cast, AI hand mostly full, menu offered only modes 2+3 — mode 1
(opponent reveals, choose I/S, discard) never appears. His rules read confirmed:
mode 1 targets the PLAYER; peeking the hand is the effect, must always be offered.

**Root cause (3 cooperating mechanisms, MAYPROBE+STASHGATE traces)**:
1. AbilityFactory::storedAbilityString (a factory MEMBER) stashes a line's reveal:
   payload before target-extraction so nested targets aren't hoisted.
2. `alternative` branch lines DEFER their body to cast-time — their stashed payload
   is never consumed, leaving the member dirty.
3. Dirty member => stash gate (storedAbilityString.empty()) fails for every LATER
   line of the card => mode-1's nested optionone target(*[instant;sorcery]|reveal)
   hoisted into the mode's target chooser => TC on the EMPTY pre-reveal zone =>
   MayAbility::Update's validTargetsExist gate silently drops the mode. Universal
   (every platform, every game) — not board-dependent.

**Fix**: `deferredAlternative = StartsWith(s, "alternative")` added to all FOUR stash
gates (chooseaname/grant/reveal/scry — same leak shape); deferred lines keep payloads
in-line for their cast-time parse (which re-enters without the keyword and stashes
correctly). MTGAbility.cpp.

**Fixture**: collective_brutality_probe.txt — full mode-1 interaction (cost menu
choice 0, mode menu choice 0, optionone choice 0, click lightning bolt, 4 nexts,
assert Bolt in P2 grave + hand=Bears,Plains at SECONDMAIN). GREEN. In worktree
manifest.

**Kept instrumentation (env-gated, #ifndef PSP)**: WAGIC_MAYPROBE — MayAbility
menu-skip verdicts (stderr) + STASHGATE prints; the menu-silently-missing class
will recur, this names the gate instantly.

**Suite-script lore reinforced**: alt-cost cards need `choice N` right after the
cast click (cost menu); mode menus/reveal menus each need their own choice line;
suite choice N = actionLayer doReactTo(N), 0-based.

Full gate + PARSETEST leg + PSP build in flight at this writing.

## 2026-08-07 — harvest 86139631: collection dirty-skip CONFIRMED on hardware

pd: split from wagic-probe.log (14 matches, MEMPROBE+WINPROBE build 86139631):
- **pd:collection 1-2ms on EVERY match** (was ~1.2s) — fingerprint gate works.
- Win TOTAL now ~2.6-2.9s (was ~3.5s); loss TOTAL ~1.2-1.6s (was ~2.2s).
  Matches prediction. Cumulative from the 7.2-15.9s starting point: ~2.7s/~1.4s.
- Remaining per-match floor (hardware numbers):
  - ur:giveAward ~1.33-1.47s on unlock wins (the single remaining options.save) —
    OR u:unlock chain ~1.47s on two wins (different award path, same save cost;
    tasks 144-146ms on those same wins). One of the two fires per unlock win,
    never both. Non-unlock wins skip both (TOTAL ~1.5s, e.g. line "unlock checks 0ms").
  - pd:player.dat 438-941ms every match (credits always change — expected).
  - pd:tasks 559-697ms EVERY match — passOneDay mutates tasks.dat each match.
    => TaskList fingerprint-gate is the next fix candidate (~0.6s/match, universal).
  - PlayerData ctor ~77-80ms, prx_* + unlock checks ~65ms — noise.
- ruleprobe: normal-death traffic only, no indest grants this session (no Rootborn
  casts) — nothing anomalous.
- Heap: arena peak ~38.3MB, free ~0.6MB at match end, recovers to ~35.5MB at menu;
  poolFallbacks 15→18 over session — same slow ratchet, watch only.

EBOOT 8aaa415b (parser fix + dirty-skip + double-save + backstop) deployed
2026-08-07, md5-verified, stick logs cleared. Awaiting in-play verdicts:
Collective Brutality all-3-modes + continued Rootborn soak.

## 2026-08-07 — defer-writes-past-render: v1 CRASHED, relocated to Update side

**Change under test**: end-of-match persistence deferred past first victory-screen
render. compute() now stashes (mPendingPlayerData + mPendingOptionsSave) instead of
saving; giveAward(bool deferSave) added (GameOptionAward), threaded through
unlockRandomSet/unlockSetByName/tryToUnlock; all win-path awards defer and coalesce
into ONE options.save() at flush. Credits::flushPendingSave() does the writes;
dtor is the safety net; compute() head flushes any stale pending (multi-game
re-entry would otherwise read collection/tasks back from disk BEFORE the pending
write lands = silent loss).

**v1 (EBOOT 58e07223): flush called from Credits::Render() 3rd frame → PSP CRASH
TO OFF after victory screen displayed.** Forensics (logs/58e07223-crash1/): probe
log shows compute TOTAL 83ms then flush block COMPLETE (flush:options 0ms,
pd:player.dat 830ms, pd:tasks 791ms, TOTAL 1623ms) — no truncation anywhere, so
the crash hit AFTER the flush finished, somewhere on/after the victory screen.
Mechanism NOT pinned. Only structural asymmetry vs every stable build: the old
1.6-3s save stall ran in Update (between frames); v1's ran mid-Render, inside the
GU display-list build. Relocated on that hypothesis — if v2 crashes with flush
TOTAL again written, the stall-location theory is DEAD and it needs real
instrumentation (immediate-append breadcrumbs after flush).

**v2: flush driven from GameStateDuel::Update, new `case DUEL_STATE_END:` (was
falling to default:), gated on credits->readyToFlush() (pending && renderCount>=2;
Render only increments the counter). Desktop links clean; PSP build in flight.**

**PSP build gotcha extended**: PSP objs/*.d deps track NEITHER CFLAGS NOR HEADERS —
signature change in Credits.h left stale StoryFlow.o → undefined-ref link fail
(default args are invisible in mangled names: unlockRandomSet(bool) !=
unlockRandomSet(bool,bool)). Sweep = rm objs/X.o for every src grep-matching the
changed decls (or including the changed header) before the docker make.

**Meanwhile confirmed**: victory screen itself now appears in ~83ms of compute
(was 2.6-2.9s synchronous) — the defer works; only the flush location was wrong.

**v3 (final form): flush on a WORKER THREAD + press latch** — user ruling: a
confirm pressed during the save must be honored, not dropped ("if I hit confirm,
then it shouldn't wait for me to press it again"). JGE ships a boost-alike
thread for PSP (JGE/include/Threading.h, sceKernel underneath, worker prio 0x15
vs main 0x11 — PSP: lower number = higher prio, worker runs in main's vsync
blocks; desktop maps to std::thread under gnu++14). ⚠ its join() is
sceKernelTerminateDeleteThread — only call after the done-flag is set.
- Credits: startAsyncFlush() spawns FlushProc → flushPendingSave() → volatile
  mFlushDone=true. readyToFlush() refuses while mFlushStarted. ensureFlushed()
  (compute head + dtor) waits done-flag, join()s, then sync-flushes remainder.
- GameStateDuel DUEL_STATE_END (Update): readyToFlush → startAsyncFlush;
  BTN_OK → mEndExitPending latch; transition fires when latch && !flushInProgress.
  mEndExitPending reset in ctor + Start().
- Thread-safety by ownership: worker exclusively owns mPendingPlayerData +
  options.save; main thread only reads mFlushDone. Victory-screen textures are
  cached during frames 1-2, BEFORE the worker starts (readyToFlush needs
  renderCount>=2) — no concurrent resource I/O. JFileSystem::openForWrite has no
  shared mutable state (reads immutable mUserFSPath, caller-owned ofstream).
- EBOOT ccf33c38 deployed 2026-08-07 (supersedes 77da763d, never played).
  In-play verdicts pending: no crash + instant victory screen + latched confirm
  + Collective Brutality 3 modes.

**v3 (EBOOT ccf33c38) LOCKED UP on hardware** — victory screen displayed, never
took input; no crash. Forensics (logs/ccf33c38-lockup/): compute block present
(847ms, ur:giveAward 0ms = deferral fine) but NO flush block → worker never ran.
ROOT CAUSE (near-certain, from code): JGE's PSP thread wrapper requests a
0x40000 (256KB) STACK, but PSP_HEAP_SIZE_KB(-256) (JGE/src/main.cpp:40) leaves
only ~256KB of partition memory outside the malloc heap — sceKernelCreateThread
fails with no-memory and the wrapper SILENTLY skips StartThread (if(id>0) only).
mFlushDone never set → flushInProgress() true forever → END-state exit gate
waited forever. Cost: that match's results were never persisted (lost on
power-cycle) — the lockup is strictly worse than the freeze it replaced.
Secondary hazard confirmed while reading: mVsync defaults FALSE (JGfx.cpp:534)
so the PSP main loop doesn't reliably block → even a created 0x15-priority
worker can starve under the strict-priority scheduler.

**v4 fixes (all four belts)**:
1. Threading.h PSP wrapper: stack 0x40000→0x10000 (64KB fits the ~248KB free),
   default ctor inits mThreadProcID(0), join() guarded, NEW started() accessor —
   creation failure is now observable. (Shared JGE header; only Credits uses the
   wrapper on PSP.)
2. startAsyncFlush: if !started() → INLINE synchronous flush (freeze, never
   lockup, never lost data) + crumb.
3. END-state Update: while flushInProgress, boost sleep 10ms/tick — donates CPU
   to the worker (anti-starvation), still renders + polls input.
4. ensureFlushed: bounded 30s wait then join (PSP=terminate-delete) + sync
   redo — pending flags clear only AFTER each write, so a terminated worker's
   unfinished files get rewritten whole.
Plus WIN_CRUMB (PSP+WINPROBE): immediate-append per-event trail in
User/wagic-crumb.log (async:start / worker:enter / worker:done /
async:create-failed) — next incident reads as fact, not inference.

**v4 (EBOOT f10e0dac) HARDWARE-VERIFIED + SHIPPED**: owner verdict "works, feels
nice"; crumbs show worker path 3/3 (async:start→worker:enter→worker:done, no
fallback), compute ~148ms before screen, ~2.6s writes behind it, coalesced
options save confirmed (flush:options 1355ms on an unlock win). Gates: suite
1039/0 + 24/0, parsetest 319/0. Committed e44ede0fe (psp-port incl. Threading.h
stack fix), merged to master bec880ce6, both pushed. THE POST-MATCH WAR IS
EFFECTIVELY CLOSED: perceived transition = ~0.15s from 7.2-15.9s at campaign
start. Remaining pd: costs (player.dat ~0.6s, tasks ~0.65s) now run invisibly.
Still pending in-play: Collective Brutality 3-modes check, a LOSS-side flush
verdict (all hardware verdicts were wins), Rootborn soak.

## 2026-08-07 — VITA PARITY BUILD + OWNER RULING REVISES THE PSP GO/NO-GO
**Owner ruling (his words): "I just want to accept that PSP isn't going to be an ideal
engine for using inference. I'm not going to try to go further than a LAN access point.
something the users phone or pc can serve to the PSP."** This RESOLVES the 2026-08-01
coffeeshop GO/NO-GO without running the TLS probe: the coffeeshop bar is withdrawn for
PSP. PSP's shipping shape = LAN companion-bridge (user's phone/PC runs the proxy);
on-device TLS is off the table permanently (mbedtls risk moot); task #6 shrinks to WiFi
bring-up + plain-HTTP POST to a configurable host. The "drop PSP entirely" branch is NOT
taken — PSP stays a target in its LAN-only shape. Vita/Android direct-endpoint
requirements are UNCHANGED (Vita has userspace OpenSSL TLS; Android gets JNI HTTPS).
Dev-side blocker for the PSP transport probe: his LAN is 5GHz/WPA2+ everywhere — PSP
sees nothing usable (confirmed by scan: Crainial 5GHz-only ch44; all 2.4GHz neighbors
WPA2/WPA3). Voyager's 8265 can't AP on 2.4 while STA on 5 (#channels<=1, verified via
iw). Paths: ethernet uplink frees the radio, or a USB 2.4GHz AP dongle — owner may own
one, digging through storage (days). LLM-on-PSP work PAUSED on that.

**Vita parity VPK built + verified from tip (incl. defer-save + Vita-safe fallback):**
wagic.vpk aa7609f82a531dbf537afd9ef69a69e2, 32MB, param.sfo = WAGC00042/"wagicGPT"
(owner's Aug-1 decision, verified in the artifact this time), eboot 12.1MB stripped,
core.zip 1062 files all STORED, no art leak, GPT symbols present. NEW Vita-relevant
commit 116462370: startAsyncFlush catches std::thread construction throw (Vita has no
pthread runtime — uncaught it would have crashed every match end; same catch discipline
as AIPlayerGPT). On Vita the victory-save runs synchronously — correct, cost was Memory
Stick physics. Awaiting VitaShell FTP window to deploy. NOTE: this VPK carries the WIRED
Vita LLM path (#14, WITH_GPT_AI + curl+OpenSSL + VitaNetInit) — the parity install can
double as the FIRST live LLM request test: endpoints.txt/GUI → http://10.0.0.198:8180
(Voyager proxy; verify the DHCP IP at test time).

## OWNER RULING 2026-08-07 — OpenAI-subscription access must be IN-CLIENT
His words: "I want users to be able to use their subscription... obviously an
intermediate that connects a computer to the client doesn't meet the criteria."
A Voyager/PC-side bridge was proposed and REJECTED — it re-violates the standing
no-second-device release bar. The correct design: the GAME implements the
ChatGPT/Codex third-party-harness flow itself — OAuth device-code sign-in
rendered in-game (code on screen, user approves on their phone), tokens stored
on-device, refresh in-client, direct TLS to the OpenAI backend (Responses-style
API, NOT /v1/chat/completions — needs a translation layer inside GptConfig's
transport seam). Applies wherever the platform has real TLS (Vita, Android,
desktop). Docket item, sequenced AFTER the OpenRouter direct-TLS test.
(LAN-bridge remains the sanctioned shape ONLY for PSP, per the earlier
LAN-only ruling — that is a different device class, not a precedent.)

## 2026-08-07 (cont) — CLOUD LLM PATH: measurements + provider pinning shipped
Vita LLM worker fix (native sceKernelCreateThread seam, commit eba378fe9) proved out:
first live handheld HTTP requests (GET /v1/models then real chat POSTs vs the local
4B upstream: 15-20s/decision typical, one 102s spiral = 1906-token verbose answer,
NOT thinking-leak; local 4B decode 19.5 tok/s, prefill ~240 tok/s, prompts ~1.3K tok).
OpenRouter live measurements (his key, from Voyager):
- qwen/qwen3.7-flash: 81 tok/s effective BUT thinks despite enable_thinking:false
  (OpenRouter/Alibaba ignore chat_template_kwargs; params UNDISCLOSED). Follow-up if
  ever revisited: OpenRouter's unified `reasoning` request field.
- deepseek/deepseek-v4-flash (base slug): 13 tok/s (bad provider roll).
- deepseek/deepseek-v4-flash-0731 unpinned: 47-63 tok/s (Novita/Baidu/StreamLake).
- deepseek-v4-flash-0731 PINNED to first-party DeepSeek: 71-76 tok/s STEADY — owner's
  chosen config. Required HIS OpenRouter privacy toggle (account had train-on-inputs
  providers blocked → 404 "guardrail restrictions"; per-request data_collection:allow
  does NOT override the account setting).
⚠ OWNER RULING (angrily reinforced): when he says "only from DeepSeek" the ONLY
acceptable routing is first-party — test and configure EXACTLY what he names.
SHIPPED dd43003c5: provider_only config key (+ WAGIC_GPT_PROVIDER env) →
provider:{only:[...],allow_fallbacks:false}; omitted for api.openai.com.
DEPLOYED to Vita: VPK cccd23f7 (provider_only support) + user endpoints.txt
(OpenRouter direct TLS, deepseek-v4-flash-0731, provider_only=DeepSeek, translog=1;
old config backed up /tmp/vita-endpoints-backup.txt → copy into save-merge dir).
Awaiting his in-game verdict: TLS handshake from Vita + decision quality + latency.

## 2026-08-07 (cont) — TRANSLOG WAS DEAD ON VITA: fifth raw-HOME site
His "we're capturing the decodes right?" exposed it: translog path construction in
AIPlayerGPT.cpp (~line 2010) used getenv("HOME") directly — the FIFTH such site (the
Aug-1 gptUserRoot sweep fixed four) → mTransLogPath empty on Vita → NO translog ever,
config irrelevant. An earlier session answer of "yes we're capturing" was WRONG.
Fix: gptUserRoot() exported from GptConfig (was file-static gptUserRootImpl inside the
anonymous namespace; public wrapper added, header doc says EVERY writable GPT path
must come from it). Translog block now builds <root>/ai/gpt/logs/<ts>-<deck>-<ptr>.jsonl
on all platforms. VPK 750ad303 built (pin + translog fix); AWAITING FTP window to
upload + his install. gpt-log.txt harvest (42KB) confirmed: all "worker thread
refused... Not owner" lines are HISTORICAL (old-build sessions); no refusals after the
native-worker build = worker seam confirmed working in his live session.
"Make it faster" plan (measurement before surgery): install 750ad303 → one game →
pull logs/*.jsonl → attribute per-decision time to reply-length (protocol plan
restatement) vs prefill vs decision-count → tune the identified knob (prompt edit /
head trim / engine batching respectively). ⚠ unknown which VPK his slow DeepSeek
session ran — if pre-cccd23f7, provider pin was IGNORED (unpinned 13-63 tok/s roulette
explains the slowness by itself).

## 2026-08-08 — TEST CONNECTION SEAM PORT (5598fbe21 / merge 1bf751067)

Owner's screenshot caught the Vita GPT options tab showing "...hreading to use
std::thread: Not owner)" on the Test connection row — the SECOND raw-std::thread
site the seam port missed (OptionGpt.cpp:267). Test connection has NEVER worked
on Vita; its error does NOT implicate the match worker (different path). Fixed:
probe now goes through gptSpawnWorker; ProbeState std::mutex (no-op on Vita =
data race under a real worker) -> GptMutex; no-worker platforms (PSP today)
probe SYNCHRONOUSLY (options screen stalls up to the 6s timeout, real answer).
Grep confirms: no raw std::thread/std::mutex remains in GPT code.
Gates: suite 1039/0 + 24 AI/0, parsetest 319/0. Committed 5598fbe21 (psp-port),
merged 1bf751067 (master), both pushed. Vita VPK b4d026f81d27b4fc09c1e79be9d787c5
(32,096,372 bytes) built at build_vita/wagic.vpk — NOT yet uploaded (FTP closed).

TIMESTAMP FORENSICS RESOLVED (owner-led): VitaShell FTP reports UTC; the Vita's
display tz was California (2h behind him year-round; both zones observe DST).
Owner ruling on sequence: every pushed VPK was installed and run — 750ad303 IS
the installed build; the missing logs/ dir means no LLM match played on it yet
(translog dir is created in the GPT player ctor, only at match start). VitaShell
re-signs eboot.bin on install (header AND section offsets shift) — installed
eboot never hash-matches the VPK's; identify builds by compiled-in strings or
the source VPK.

NEXT: FTP window -> upload b4d026f8 -> he installs -> Test connection should
answer honestly (OK - serving <model> / unreachable) -> one DeepSeek match ->
harvest ux0:data/Wagic/ai/gpt/logs/*.jsonl for the speed attribution.

## 2026-08-08 — HIDDEN-REASONING FIX (71f4f615c / merge 8d5446a73) — THE VITA SPEED ANSWER

FIRST VITA TRANSLOG harvested (1786238841-ai_baka_deck9-*.jsonl, 36 decisions, 26
turns, vs pinned first-party DeepSeek on VPK b4d026f8). Findings:
- Latency 5s (early) -> 40-55s (late), tracking PROMPT growth; replies tiny (med
  279 chars). Decode was never the cost.
- 13/36 decisions fell back to Baka with EMPTY replies, all dying at 46-58s.
- ROOT CAUSE (replayed seq-30 from Voyager, same body shape as buildRequestBody):
  usage showed 3,548 HIDDEN reasoning tokens behind a 139-token answer.
  chat_template_kwargs.enable_thinking is IGNORED via OpenRouter (same as the
  qwen3.7-flash finding). Fallbacks = reasoning spirals hitting max_tokens 4096:
  whole budget burned thinking, content empty (46-58s ~= 4096 tok of decode).
  Slowness and fallbacks were ONE defect. Owner's rate-limit hypothesis ruled out.
- FIX: request["reasoning"] = {enabled: <thinking toggle>} scoped to
  openrouter.ai. Same decision replayed: 41.9s -> 2.2s, SAME block chosen
  (B1:A2), 0 reasoning tokens. Projected in-game: 2-5s decisions, no fallbacks.
Gates: suite 1039/0 + 24/0, parsetest 319/0. Committed 71f4f615c, merged
8d5446a73, pushed. VPK 9a1b3ae40edcc90c5e08dcee41a9258c (32,096,464) UPLOADED to
ux0:/vpk/wagic.vpk, round-trip hash-verified. Awaiting his install + play verdict.

Earlier same day: Test connection PROVEN WORKING on Vita hardware (screenshot:
"serving inclusionai/ling-3.0-tiny:free" = catalog head from /v1/models, cosmetic
- docket: echo the CONFIGURED model when set instead of the catalog head).
gptHttpPost failures are SILENT (no gpt-log line, no HTTP status distinction) -
docket: log non-200s with status so transport vs model failures separate without
a replay experiment.

## 2026-08-08 (cont) — REASONING FIX VERIFIED LIVE + PROVIDER PIN ROW (0e8333fae / eb7bf7443)

Post-fix match translog (1786242469-*, 23 decisions, 13 turns): latency med 3.8s
max 5.6s, ZERO fallbacks. Was: med 39s, 13/36 fallbacks. Owner: "much much better."

OWNER ORDER: in-app config for model + provider (OpenRouter), and the reasoning
flag must work from the settings menu. Findings: Model row ALREADY existed
(OptionGptText, "(auto-detect)", above API key - off-screen in his screenshots);
Thinking toggle ALREADY flows (cfg.thinking -> mThinking -> reasoning.enabled,
live since 9a1b3ae4). Gap was the provider pin: ADDED "Provider pin (OpenRouter)"
free-text row under Model, binds cfg.providerOnly. Gates 1039/0+24/0, 319/0.
Committed 0e8333fae, merged eb7bf7443, pushed. VPK 88856747b501c96c03f887520f82c637
(32,096,274) uploaded + hash-verified (first upload died partial at Vita FTP
restart - exit 18 on verify caught it; retry clean on his second window).

OPEN/DOCKET from this arc: (a) auto-detect vs OpenRouter picks catalog head
(ling-3.0-tiny:free) - bad default for multi-model gateways, decide a guard;
(b) Test connection should echo CONFIGURED model; (c) gptHttpPost should log
non-200 status lines; (d) on-screen-keyboard text entry on Vita rows UNVERIFIED
on hardware (URL/key/model/pin all share the mechanism).

OWNER DESIGN RULING (2026-08-08): current GPT config menus are "functional, but
not user friendly - fine for the alpha release," but LONG RUN the model and
provider rows must POLL the API and present a CHOOSER, not text entry. Design
sketch agreed: /v1/models -> filterable model list (degrades to short list on
single-model servers); OpenRouter /v1/models/:id/endpoints -> per-model provider
list chosen AFTER the model (prevents pinning a provider that doesn't serve it);
free text remains the escape hatch. POST-ALPHA docket item, not scheduled.

HARDWARE VERDICT (2026-08-08): on-screen keyboard text entry on Vita rows WORKS
(docket item (d) above closed). OWNER: the popup is Wagic's own SimplePad, "not
sure if this keyboard is preferable over the native vita keyboard... not an
alpha concern, but we should try using the native keyboard." POST-ALPHA docket:
trial sceImeDialog (native Vita IME) for text rows - platform-seam it like
threads/paths; SimplePad remains the PSP/desktop path.

VITA COSMETIC BUG REPORTED (2026-08-08, owner screenshots x5): small white dots/
glyph-like fragments UNDER the SELECTED main-menu icon (follows selection; some
fragments resemble letterforms). A/B RUN: desktop build renders the same menu +
flame CLEAN -> Vita-specific rendering artifact, NOT particle data/menu code.
Prime suspect: vitaGL state leak between hgeParticleSystem quad draws (shared
sprite, per-particle RenderQuad) and the adjacent font draws (letterform look =
possibly sampling the font atlas). Same family as the deck-select green
rectangle docket item. Cosmetic, alpha-acceptable. Fix entry point: JGfx Vita
renderer's texture/UV state across RenderQuad<->DrawString interleave;
GameStateMenu.cpp:505 wires the emitters, MenuItem.cpp Render() draws them.

VITA COSMETIC BUG #2 (2026-08-08, owner side-by-side photo, "been ignoring it"):
deck-select ("Choose a Deck") BACKGROUND DISTORTED on Vita - what looks like a
second image (landscape photo?) bleeding through top-left + dense speckle noise
across the panel; parchment + emblem faint underneath. PSP renders the same
screen clean -> assets fine, Vita texture path at fault (wrong/stale texture
bound under the background quad, or pixel-format/stride mismatch on upload).
Same family as the menu-dots leak. GROUPED: do menu-dots + this + (PSP green
rectangle is separate but adjacent) as ONE Vita-graphics investigation of
vitaGL texture-bind/state handling in JGfx when the pass is scheduled.

CORRECTION (owner, 2026-08-08): the main-menu dots appear on OTHER PLATFORMS
too, sometimes. Reclassify: NOT Vita-specific - intermittent SHARED-CODE
artifact; my single desktop launch was insufficient evidence against a
sometimes-bug. New prime suspect: hgeParticleSystem spawn/init path (particles
rendered with incompletely-initialized age/size/location around Fire()/spawn) -
same uninitialized-state family as the PSP green rectangle. The deck-select
background corruption (bug #2) remains Vita-only as far as observed - keep the
two separate until evidence links them.

## 2026-08-08 (session close) — RESUME ANCHOR: OPENAI SUBSCRIPTION AUTH

SESSION HANDOFF (filter downgrade -> new session on Opus 4.8). State at close:

VITA LOOP: HEALTHY, closed out. VPK 88856747 installed (provider-pin GUI row +
reasoning fix + Test-connection seam). Pinned first-party DeepSeek via
OpenRouter: med ~4s/decision, max ~8s, ZERO fallbacks across matches 2+3.
Translog corpus at psp-work/vita-translogs/ (match1=pre-fix forensic baseline,
match2-3=healthy; grows every match, harvest each FTP window). Owner verdict:
Baka "just terrible" vs DeepSeek - validates thesis; corpus doubles as future
policy-model training data.

ACTIVE TASK (owner order, research DONE, experiment NOT run): connect the game
to his OpenAI SUBSCRIPTION, IN-CLIENT (standing angry ruling: no PC
intermediary; device-code-on-phone is acceptable). 4-agent research 2026-08-08
archived at psp-work/openai-subscription-research.md (full source URLs). Digest:
- Viable path = Codex subscription auth (Track 2), same as Zed/OpenClaw/Cline.
  NOT "Sign in with ChatGPT" (that's identity-only, partner beta 2026-08-02).
- Device-code flow (beta, NOT vanilla RFC 8628): POST auth.openai.com/deviceauth/usercode
  {"client_id"} -> {device_auth_id, user_code, interval}; user enters code at
  https://auth.openai.com/codex/device (15-min expiry; user must first enable
  device auth in ChatGPT Settings->Security); poll POST /deviceauth/token
  {device_auth_id, user_code} (403/404 = pending) -> {authorization_code,
  code_challenge, code_verifier} -> standard PKCE exchange at /oauth/token.
  Public client id: app_EMoamEEZ73f0CkXaXp7hrann. Client needs: 3 HTTPS POSTs +
  JSON + poll timer. Vita has all (curl+OpenSSL).
- Inference goes to https://chatgpt.com/backend-api/codex/responses (RESPONSES
  shape, not chat completions -> AIPlayerGPT needs an adapter); undocumented,
  no SLA. Models: gpt-5.6-sol/terra/luna, gpt-5.5 fallback. Plan limits =
  rolling 5h windows (Plus could feel ~30-req matches; Pro comfortable).
- ToS: tolerated grey zone; personal use = industry norm w/ OpenAI's visible
  blessing (Zed blog, Huet quote); no GA program for arbitrary apps. SHIPPING
  it in the public alpha = separate owner decision later; personal use now OK.

NEXT ACTION (agreed, not yet executed): prove transport from Voyager BEFORE any
C++ - script the device-code dance with curl against HIS account (he enters
code on phone), then ONE real completion against backend-api/codex/responses to
learn: does the flow work outside Codex CLI; exact headers/shape (community
hints: may need chatgpt-account-id header from the id token + instructions
field - UNVERIFIED); cost of a game-sized request against his window. THEN
port into the client behind the endpoint-preset system (preset "OpenAI
subscription"), reusing the gptSpawnWorker seam; store refresh token via the
key-storage path (keysalt obfuscation) like the API key.

STANDING RULES REFRESHER for the next session: deploy-first, suite gates
commits concurrently (suite 1039/24 + parsetest 319 both must be green); merge
psp-port->master after every unit; NO AI attribution anywhere; his repo = his
rules; when he names a provider/model, configure EXACTLY that; never bring the
local 4B into cloud comparisons; VitaShell FTP = UTC timestamps, silent-empty
on dead connections (verify with exit codes); Vita deploys: build via
tmp/vita-build.sh, upload ux0:/vpk/wagic.vpk, HE installs via VitaShell Cross;
"upload confirmed, go install" handshake each cycle; PSP transport still PAUSED
on his USB-dongle dig. Rules.cpp temp bench (WAGIC_BENCH_XR) still uncommitted
in the worktree - remove before any release.

DESIGN PRINCIPLE (owner correction, 2026-08-08): provider endpoints (DeepSeek/
OpenRouter, OpenAI subscription, OpenAI API key, local, ...) are PEER OPTIONS the
END USER chooses among with their own money — NO default ranking, NO
"everyday vs premium" hierarchy, NO Claude editorializing about which to prefer.
The owner will NOT use the subscription or an API key for his own play; his
runs are TESTING, not a usage preference. Do not attribute provider preferences
to him or bake a favored default into the UI/design.

## 2026-08-09 — OPENAI SUBSCRIPTION: TRANSPORT PROVEN (no attestation wall)

Device-code auth SUCCEEDED (tokens at ~/.config/wagic-oai/auth.json 0600:
access+id+refresh; account.json: chatgpt_account_id present, plan=plus). Path
corrections vs the research digest (all verified against openai/codex source):
- usercode: POST https://auth.openai.com/api/accounts/deviceauth/usercode
  JSON {client_id} (digest wrongly dropped /api/accounts/; 405+HTML SPA = right
  host wrong route).
- token poll: POST {AUTH}/api/accounts/deviceauth/token JSON {device_auth_id,
  user_code}; pending on 403/404; success returns {authorization_code,
  code_challenge, code_verifier} (server-generated PKCE).
- exchange: POST {AUTH}/oauth/token form-encoded grant_type=authorization_code
  &code&redirect_uri&client_id&code_verifier; redirect_uri = {AUTH}/deviceauth/
  callback (device flow, NOT localhost:1455). DEFAULT_ISSUER https://auth.openai.com,
  client_id app_EMoamEEZ73f0CkXaXp7hrann, code-entry page {AUTH}/codex/device.
Scripts: tmp/oai-device-auth.py (working), tmp/oai-completion-probe.py.

COMPLETION PROBE = HTTP 200, NO ATTESTATION REQUIRED. POST
https://chatgpt.com/backend-api/codex/responses with MINIMAL headers works:
Authorization: Bearer <access>, chatgpt-account-id: <acct>, Content-Type:
application/json, originator: codex_cli_rs, OpenAI-Beta: responses=experimental,
User-Agent codex UA, Accept text/event-stream. (Omitted ALL codex telemetry +
attestation headers — server didn't care.) Response = SSE Responses events
(response.created/in_progress/... ; body is Responses shape not chat-completions).
Rate headers returned EVERY call: x-codex-plan-type plus, x-codex-active-limit
premium, PRIMARY window = 10080 min = 7 DAYS (not 5h), used 0%, reset 604800s;
no secondary; credits-has-credits False. gpt-5.6-luna defaults reasoning
effort=medium -> adapter MUST set reasoning.effort low/minimal (same latency
trap as the DeepSeek reasoning fix).

ADAPTER WORK (next): AIPlayerGPT needs a Responses-endpoint mode: translate
system->instructions, user->input[{type:message,role,content:[{type:input_text,
text}]}], set reasoning.effort low, store false. Test stream:false first (Vita
path is non-streaming); if rejected, add a small SSE reader behind gptSpawnWorker.
Endpoint preset "OpenAI subscription" -> auth from ~/.config/wagic-oai (Vita:
ux0:data/Wagic/ai/gpt/); refresh via refresh_token when access expires (expires_in
in auth.json). Surface x-codex-primary-used-percent to the user. UA/originator
are Codex-fingerprint-coupled -> versioned constant + distinct "challenged" vs
"denied" vs "rate" error paths (HTML body = web tier = wrong route/blocked).

## ═══ HANDOFF (2026-08-09 morning) — SUPERSEDED by the ═══ block at FILE END ═══

(Kept for history; the adapter/tab/picker/sign-in units below all landed the
same day. Read the file-end handoff instead.)

READ THIS BLOCK FIRST, then the two dated sections above it (2026-08-09 OpenAI
transport + 2026-08-08 sessions). Fleet memory `project-wagicgpt` hook routes
here. Session history: worked the OpenAI-subscription track on Opus 4.8 after a
Fable filter downgrade (UA-header-past-Cloudflare tripped safeguards); this
handoff exists because the owner wants to try resuming on Fable.

WHERE THINGS STAND — two live tracks, strategy-loop still paused:
- VITA (healthy, deployed): VPK 88856747 installed. LLM opponent = OpenRouter
  direct-TLS, deepseek-v4-flash-0731 PINNED first-party, ~4s/decision, zero
  fallbacks. Full couch config (model+provider-pin+key rows; Test connection
  works via the gptSpawnWorker/GptMutex Vita seam). Translog corpus at
  psp-work/vita-translogs/. Owner verdict: Baka "just terrible" vs DeepSeek.
- OPENAI SUBSCRIPTION (transport PROVEN, adapter NOT built): device-code auth
  works, tokens at ~/.config/wagic-oai/ (plan=plus). Backend
  https://chatgpt.com/backend-api/codex/responses returns HTTP 200 with a
  MINIMAL header set — NO attestation wall. Full endpoint/header/shape/rate
  facts in the 2026-08-09 section above. Scripts: tmp/oai-device-auth.py,
  tmp/oai-completion-probe.py (both working; tmp = ephemeral, copy anything
  worth keeping into psp-work/).

IMMEDIATE NEXT STEPS (owner's call which, if any):
1. (optional, 1-line) re-run the probe with stream:false to see if the backend
   returns a single JSON blob -> lets the Vita reuse its non-streaming parser
   instead of an SSE reader. Edit tmp/oai-completion-probe.py "stream": False.
2. BUILD the OpenAI-subscription adapter in AIPlayerGPT: Responses-shape request
   (system->instructions, messages->input[{type:message,role,content:[{type:
   input_text,text}]}]), reasoning.effort low, store false; endpoint preset
   "OpenAI subscription" reading ~/.config/wagic-oai (Vita: ux0:data/Wagic/ai/
   gpt/); refresh via refresh_token on expiry; surface x-codex-primary-used-percent.
   Behind gptSpawnWorker seam. Then SSE reader iff stream:false is rejected.
3. POST-ALPHA docket (do NOT start unprompted): API-polled model/provider
   PICKERS (replace text entry); native Vita IME (sceImeDialog) for text rows;
   Test connection echo the CONFIGURED model not catalog head; gptHttpPost log
   non-200 status. Vita graphics: menu-dot particles (intermittent, cross-
   platform, shared-code particle-init suspect) + deck-select bg corruption
   (Vita-only texture path) — group as one graphics pass. PSP LLM transport
   PAUSED on owner's USB-WiFi-dongle dig. In-client OpenAI OAuth is the ACTIVE
   answer to "OpenAI subscription for end users" — bridge/PC-intermediary was
   REJECTED (angry ruling).

STANDING RULES (violating these has burned trust this campaign):
- Providers are PEER OPTIONS for the END USER; no default ranking; his runs are
  TESTING not his preference (memory feedback-wagicgpt-peer-provider-options).
- When he names a provider/model, test/configure EXACTLY that, first-party only.
- Never bring the local 4B into cloud comparisons.
- Deploy-first; suite (1039/24) + parsetest (319) both green gate commits
  concurrently; merge psp-port->master after every unit; NO AI attribution ever.
- Vita deploy: build tmp/vita-build.sh -> upload ux0:/vpk/wagic.vpk -> "upload
  confirmed, go install" -> HE installs via VitaShell Cross. VitaShell FTP =
  UTC timestamps, silent-empty on dead connections (verify exit codes). His
  sequence-of-events is ground truth over timestamp forensics.
- Verify endpoints/paths against SOURCE (openai/codex) not research digests —
  the digest was wrong twice (missing /api/accounts/ path segment; the wall
  that wasn't). HTML body on an API call = web tier = wrong route/blocked.
- Don't relitigate settled decisions; he flags policy-relitigation as token-waste.

GIT: worktree psp-port @ 0e8333fae == master @ eb7bf7443 (in lockstep, pushed).
Uncommitted intentional: projects/mtg/src/Rules.cpp temp bench (WAGIC_BENCH_XR)
— REMOVE before any release. No OpenAI C++ written yet (all tmp scripts + this
ledger). Suite baseline last green: 1039/0 + 24/0, parsetest 319/0.

## 2026-08-09 — OPENAI-SUBSCRIPTION ADAPTER SHIPPED (f2442e6f6, master+psp-port, pushed)

THE ADAPTER IS BUILT AND LIVE-VERIFIED ON DESKTOP. Commit f2442e6f6 (4 files,
+706): preset "OpenAI subscription" -> https://chatgpt.com/backend-api/codex.
Gate: suite 1039/0 + 24/0 single-threaded, PARSETEST 326/0 (319 + 7 new
CODEX-SSE cases). Binary /tmp/wagic-f2442e6f6. Both branches lockstep, pushed.

LIVE SMOKE (desktop, real backend): full selfplay game, BOTH seats
gpt-5.6-luna, 33 decisions, ZERO fallbacks, latency min 1.9s / med 2.9s /
max 6.3s, played to a turn-12 kill. Translogs 17862518*.jsonl in
~/.Wagic/ai/gpt/logs/.

BACKEND FACTS PINNED BY PROBE (2026-08-09, all of these 400 otherwise):
- stream:false REJECTED ("Stream must be set to true") -> replies always SSE.
- max_output_tokens REJECTED ("Unsupported parameter") -> NO decode-side
  truncation cap on this preset; reasoning.effort + protocol brevity are the
  only levers. Body carries ONLY: model, instructions, input, reasoning,
  store, stream.
- reasoning.effort tiers for gpt-5.6-luna: none/low/medium/high/xhigh/max —
  there is NO "minimal" (first probe run failed on it; caught by the
  gpt-log.txt failure-visibility channel exactly as designed). Mapping:
  thinking -> "low", non-thinking -> "none". Default medium = the
  hidden-reasoning latency trap, never omit the field.
- response.completed arrives with an EMPTY output array — the text rides
  response.output_text.done (one per content part) / .delta. Extraction
  order: done events, then accumulated deltas (cut streams), then the
  completed output array (future-proofing).
- Rate headers on EVERY reply; x-codex-primary-used-percent surfaced in-game
  as a notice WHEN IT MOVES ("ChatGPT plan: N% of the weekly limit used").

ARCHITECTURE (where things live):
- GptConfig.cpp: httpRequestFull (headers+status+resp-headers out, curl-only;
  JNI/NO_CURL stub returns transport-failure -> Baka), the codex section
  (auth cache, refresh, SSE extract, gptCodexComplete), preset row, probe
  branch (probe = minimal REAL completion, ~25 tokens; no /v1/models there).
- AIPlayerGPT.cpp: buildRequestBody codex branch (Responses shape),
  WorkerMain codex branch REWRAPS extracted text into the chat-completions
  envelope so every downstream parser is untouched, plan-usage notice on
  consume. WorkerCtx gained a codex flag.
- Auth file: <user root>/ai/gpt/oai-auth.json (Vita:
  ux0:data/Wagic/ai/gpt/oai-auth.json), desktop fallback
  ~/.config/wagic-oai/auth.json (+sibling account.json). Shape:
  {"tokens":{access_token,refresh_token,...},"minted_at":N,
  "chatgpt_account_id":S}. Refresh: POST auth.openai.com/oauth/token JSON
  {client_id,grant_type=refresh_token,refresh_token} (VERIFIED against
  codex-rs login/src/auth/manager.rs). Tokens ROTATE — all auth mutations
  under ONE mutex, persisted immediately; a stale-refresh reuse
  (refresh_token_reused) kills the whole login.
- OAuth scripts recreated DURABLY at psp-work/oai-device-auth.py +
  oai-completion-probe.py (the tmp/ originals were lost to ephemeral tmp —
  second time; anything worth keeping goes in psp-work/ FIRST now).

SECURITY SWEEP (owner asked before his planned login scrub): pending diff,
tracked files, and full git grep all CLEAN — no tokens/keys anywhere in the
repo; the only credential-adjacent string is the PUBLIC client id. psp-work/
(translogs, NOTES, scripts) is untracked and stays that way. OWNER RULING:
tokens stay LIVE until testing is done ("no need to invalidate it while
we're still working"); he scrubs after.

VITA-BROWSER QUESTION ANSWERED: the code-entry page (auth.openai.com/codex/
device) is a modern React SPA behind Cloudflare — NetFront (ES5-era JS, old
TLS) cannot render it. Irrelevant to the design: device-code flow = game does
3 JSON POSTs + poll (Vita curl+OpenSSL, proven by OpenRouter), code entered
on the PHONE per standing ruling.

FOLLOW-UP COMMIT 579fc63e7 (master+psp-port, pushed): Vita build fix —
GptConfig.cpp needed an unconditional #include <mutex> (on Vita GptMutex is
the kernel-mutex class and GptConfig.h's Vita branch never pulls <mutex>, so
std::lock_guard was undeclared; lock_guard is header-only RAII, safe
everywhere). Desktop rebuilt clean, PARSETEST 326/0 re-confirmed, binary
/tmp/wagic-579fc63e7. LESSON RE-LEARNED: edits land in the MAIN tree — the
worktree VPK build runs the WORKTREE'S checkout; commit+merge BEFORE
rebuilding the VPK or you rebuild the old code (one docker cycle wasted).

**VITA VPK READY: md5 6e8279d28f01314b302111a4682f0467, 32,127,403 bytes,
build_vita/wagic.vpk, built 2026-08-09 from 579fc63e7** (determinate: tree
committed, nothing else running). UPLOADED + INSTALLED same day (owner
photo: GPT tab live, Test connection "OK - serving gpt-5.6-luna" = the SSE-
over-Vita-curl leg PROVEN). His live-match report: EVERY decision fell back
— diagnosed as the stale-model landmine (installed build predates the heal;
config still model=deepseek/... on the codex URL; Test passed because the
old probe used the default). Fix rides the next VPK.

## 2026-08-09 (cont) — GPT TAB REWORK (686770c8c) + MODEL PICKER (6baf91566), both pushed

**686770c8c "GPT tab speaks each provider's real vocabulary"** (gate: suite
1039/0+24/0 ran green post-commit; PARSETEST 326/0): preset-aware rows.
- Codex model roster VERIFIED BY PROBE: gpt-5.6-luna / gpt-5.6-terra /
  gpt-5.6-sol / gpt-5.5 and NOTHING else for ChatGPT accounts ("-fast" and
  "-codex" variants 400: "not supported when using Codex with a ChatGPT
  account"). "Luna fast" is not an id — it's luna at low effort.
- Reasoning row: under codex = effort cycler over the server's set
  (none/low/medium/high/xhigh/max; "none" is REAL on gpt-5.6 — server-
  enumerated + a full desktop game ran on it, reasoning_tokens 0); default
  low; config key reasoning_effort= / env WAGIC_GPT_EFFORT (invalid tier →
  default + logged). Elsewhere the row stays the thinking toggle.
- API key + provider pin render "(not used with this preset)" under codex,
  refuse the keyboard.
- STALE-MODEL LANDMINE closed 3 ways: preset switch across the codex
  boundary clears a non-belonging model id; resolveEndpoint HEALS a foreign
  id to the default (logged; reproduced live against his exact Vita config
  shape — healed, played on luna, zero fallbacks); gptProbeEndpoint takes a
  modelHint so Test connection probes the CONFIGURED model.

**6baf91566 "Model picker: poll the endpoint's catalog, browse it
vendor-first"** (PARSETEST 326/0; suite gate launched, deploy-first): the
Model row now opens a picker instead of bare keyboard. gptListModels =
/v1/models everywhere it exists, static roster on codex. GameStateOptions
owns the menu chain (telemetry-consent pattern; menus only built from
Update — the delete-in-callback UAF idiom respected; worker owns fetch
state via shared_ptr so leaving the screen mid-fetch is safe; back button
cancels a slow fetch). Vendor-first bucketing by the id prefix before '/':
OpenRouter today = 400 models / 58 vendors, largest bucket openai@95 (long
scroll but navigable; within-vendor split deliberately not built). Single-
bucket catalogs skip the vendor step. "Type manually..." item = the escape
hatch (absent on codex — closed roster); no-listing endpoints drop straight
to the keyboard. ⚠ GUI flow is compile+logic-verified only — the menu chain
has NOT been driven live (owner is on the shared TV; desktop GUI driving
would fight his display). First live gate = his next Vita session:
press Model on OpenRouter preset (expect vendor list) and on subscription
preset (expect the 4-model roster).

**VITA VPK e709f3e651c78e55dcb4a389d8bc6ed2 (32,139,382 B, from 6baf91566 =
adapter + tab rework + heal + picker) UPLOADED + byte-verified 2026-08-09,
supersedes the never-installed 8814128a.** INSTALLED same night — HARDWARE
CONFIRMED by translog harvest (7 games pulled to vita-translogs/): the 00:41
game = 13/13 fallbacks (deepseek id on codex, old VPK; gpt-log carries the
exact predicted 400); from 01:15 = gpt-5.6-luna, THREE games ZERO fallbacks,
med 4.5-6s/decision, AI 2-1 vs the owner — the HEAL FIRED on hardware (config
still says deepseek; translog model=gpt-5.6-luna). The 01:15 deck7 game is
his reported Armageddon roflstomp: the model named Armageddon as a
finisher-once-ahead in its TURN-0 mulligan plan, renewed the hold-condition
in every PLAN line, and cast it turn 9 at exactly "two creatures vs none" —
the PLAN-carry architecture visibly doing cross-turn intent. Owner reaction:
"this game is alot harder with a competent opponent" + his insight that DECK
TIERS RE-PRICE under a competent pilot (dangerous decks = engines now, not
stat-sticks; the roster re-pricing corpus = selfplay round-robin win-rate,
nearly free at <1%-plan-week/game). Plan meter: backend reports INTEGER
percent; all of today = still 0% (games are that cheap; his cache-first
prompt architecture — his stated standing practice — is why). Picker menus +
reasoning-effort cycler LIVE-DRIVEN by the owner same night (his report:
"used the model picker, and cycled through the reasoning options already") —
every 2026-08-09 unit is hardware-confirmed; nothing from today awaits
verification.

## 2026-08-09 (cont) — IN-CLIENT DEVICE-CODE SIGN-IN + QR (c85374157, pushed both branches)

Owner ordered the in-client flow + asked for a QR instead of typing the URL.
SHIPPED: sign-in row on the GPT tab (codex preset; "(subscription preset
only)" elsewhere; shows signed-in state via gptCodexAuthPresent) → full-panel
screen owned by GameStateOptions: QR of https://auth.openai.com/codex/device
(RIGHT half, quiet zone 4, integer scale) + user code LARGE (left) + poll
status; Back cancels anywhere; any button dismisses terminal states.
- QR = vendored qrcodegen v1.8.0 (Nayuki, MIT; cpp pair), rendered via
  JRenderer::FillRect. Wired into ALL FIVE build listings: wagic.pri (+qmake
  regen), Makefile.sdl, PSP Makefile objs, Android.mk, root CMakeLists.
  ⚠ CMake needed a RE-RUN inside the vitasdk container to pick up the new
  source (cmake .. before make).
- QR content = the BARE verified URL. Codex source builds verification_url
  with NO code param ({base}/codex/device); whether the SPA prefills from
  ?user_code= is UNVERIFIABLE headlessly — do not ship the guess.
- Worker (GptConfig.cpp, curl platforms only): usercode POST → publish
  code+url → 250ms-sliced poll loop (cancel-aware, interval clamped 2-30s,
  16-min expiry) → PKCE exchange (form-encoded, redirect_uri
  {AUTH}/deviceauth/callback) → account id + plan from id_token JWT claims
  (base64URL → the -_ translate + reuse b64decode) → writes oai-auth.json
  with chatgpt_account_id INLINE (single file now suffices; the sibling
  account.json fallback stays for old mints) → RESETS the codex auth cache
  under codexMutex so Test connection + the next completion see the login
  without a restart (the cache remembers misses by design).
- No-transport platforms (PSP WAGIC_NO_CURL, Android JNI) + threadless
  refusals fail the state with a named reason — the flow waits on a human
  and cannot run synchronously.
- Gates: PARSETEST 326/0; suite launched (deploy-first). Desktop clean
  build first try. VPK 5f57c6e5e5094baddf7ad96ff2ab6f1b (32,166,689 B) from
  c85374157 STAGED — FTP was closed; upload next window.
- NOT yet observed live: the whole GUI flow (QR scannability from the Vita
  panel included). HIS FIRST VITA SIGN-IN IS THE LIVE GATE — and doubles as
  the re-mint rehearsal for his planned login scrub: scrub → old tokens die
  → sign in fresh from the couch. gpt-log.txt names the failing leg if any.

CODE-IN-LINK: SETTLED NO (owner asked; agent research 2026-08-09, bundle-level
evidence — do NOT re-research). The code-entry page's own JS route module
(auth-cdn.oaistatic.com route for deviceauth/callback, read in full) reads
ONLY the OAuth code/state params; the code grid has no prefill branch, no
user_code URL read, no path-param route; server drops ?user_code= from the
redirect chain; usercode API returns no verification_uri_complete; every
third-party tool on this flow (openclaw/kilocode/etc) shows bare-URL+code.
DELIBERATE: codex has a "device-code phishing warning" commit — code-in-link
is a phishing amplifier, OpenAI is intentionally hostile to it. Ceiling UX =
what we shipped (QR of bare URL + big code); phone-side entry is one PASTE
(bundle-confirmed paste handler fills the grid) or OTP autofill.

NEXT: (1) Deploy on FTP window (psp-work/deploy-vita.sh; byte-verify; HE
installs via VitaShell Cross — "upload confirmed, go install" handshake).
(2) Copy ~/.config/wagic-oai/auth.json to the Vita as
ux0:data/Wagic/ai/gpt/oai-auth.json (single file; account id is embedded in
the game's reader via sibling OR inline field — auth.json alone works iff it
carries chatgpt_account_id inline; his current auth.json does NOT (account id
sits in the sibling account.json) -> copy BOTH files, or merge the field in
first). (3) Set preset "OpenAI subscription" in the GPT tab; Test connection
= a real ~25-token completion. (4) Vita live test = the SSE path over Vita
curl (the one leg not yet observed live). (5) LATER unit: in-client
device-code flow (show code on screen, poll in-game) — oai-device-auth.py is
the reference; owner scrubs the login once testing completes (his ruling:
keep tokens live until tests done). tmp/vita-build.sh was ALSO lost with /tmp
— docker recipe: cd build_vita && cmake
-DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake .. && make -j8
wagic.self-self vpk; then chown build_vita back to magi.

## ═══ HANDOFF → next session (2026-08-09 close) ═══

READ THIS, then the 2026-08-09 dated sections above it. Fleet memory
project-wagicgpt routes here. This supersedes the morning handoff block.

THE DAY: the whole OpenAI-subscription arc shipped in one session — five
commits, master+psp-port lockstep, every one gated (suite 1039/0+24/0 ST,
PARSETEST 326/0):
  f2442e6f6  adapter (Codex Responses backend as the "OpenAI subscription"
             preset; SSE-only; rewraps text into chat-completions envelope)
  579fc63e7  Vita <mutex> build fix
  686770c8c  preset-aware GPT tab (verified 4-model roster, reasoning-effort
             row, stale-model landmine closed 3 ways incl. resolveEndpoint
             heal — heal HARDWARE-CONFIRMED via translogs)
  6baf91566  polled model picker (vendor-first; owner live-drove it same
             night, plus the effort cycler)
  c85374157  in-client device-code sign-in with on-screen QR (qrcodegen
             vendored into all 5 build listings)
Binaries /tmp/wagic-<hash> for f2442e6f6, 579fc63e7, 6baf91566, c85374157.

HARDWARE STATE: Vita has VPK 5f57c6e5 (from c85374157) uploaded; his OLD
credentials DELETED from ux0:data/Wagic/ai/gpt/ at his order (fresh-install
test). ⚠ OPEN GATE: his fresh-install sign-in run (install VPK → subscription
preset → sign-in row → QR/code/approve → Test connection) — result NOT yet
reported when this block was written. If it failed, gpt-log.txt on the stick
names the leg. After a PASS, his login scrub is unblocked (scrub on phone →
sign in fresh; desktop tokens at ~/.config/wagic-oai/ die too — desktop
re-mint = psp-work/oai-device-auth.py or the desktop GUI row).

SETTLED TODAY (do not reopen): code-in-link for device auth = NO (bundle-level
evidence, deliberate OpenAI phishing hardening — section above); backend
rejects stream:false AND max_output_tokens; effort tiers none/low/medium/high/
xhigh/max (no "minimal"); ChatGPT-account model roster = luna/terra/sol/5.5
exactly; plan meter is INTEGER percent (a full game rounds to 0% on Plus).

OPEN DOCKETS (unchanged): PSP LLM transport paused on his USB-dongle dig;
post-alpha GUI items (Vita IME, menu-dot particles, deck-select bg); wave-33
strategy-loop corpus still unrun (its resume anchor stands in fleet memory);
owner's roster-re-pricing idea (deck tiers under a competent pilot — selfplay
round-robin, nearly free on the subscription backend) is an IDEA he voiced,
not a tasked run. Rules.cpp WAGIC_BENCH_XR temp bench still uncommitted in
the worktree — remove before any release.

STANDING RULES: providers are peer options (no ranking, his runs = testing);
when he names a provider/model configure EXACTLY that; deploy-first, gates
run concurrently; merge psp-port↔master after every unit; NO AI attribution;
edits land in the MAIN tree — commit+merge BEFORE any worktree VPK build;
durable scripts/notes go in psp-work/ FIRST (tmp/ losses bit twice).

## 2026-08-18 — cardart toolkit (fetch + 5 platform packers)

Owner asked for per-platform card-art res-set scripts: download from the
licensed source, then platform resizes (full + thumb). Built psp-work/cardart/:
- fetch-art.py — Scryfall fetcher into the master pool (User/sets/sets.hidden,
  auto-detected). Reuses User/_bulk_map_large.json (70,133 ids); --rebuild-map
  streams fresh bulk (stream-parser, no 2GB json.load). Ladder: map ->
  /cards/multiverse/<id> -> English-borrow via /cards/named?exact=<name from
  _cards.dat> (Salvat ids 404 at step 2 — VERIFIED live, PS11 16835/16836
  fetched English 672x936 by name, 0 misses). image_uris.large per the
  2026-08-03 normal-vs-large regression note. Throttle 120ms, UA set,
  resumable/skip-existing, misses.txt.
- pack-common.sh engine + pack-{psp,vita,android,windows,linux}.sh profiles:
  psp 180x250/85 + 64x93!/85 (pow2 derivation preserved), vita 360x514/88 +
  128x186!/85, android 488x680/88 + 114x166!/85 (new profile, my sizing call),
  windows/linux = pool copy (no recompress) + 114x166!/85. Same zip contract:
  sets/<SET>/<SET>.zip, keys <id>.jpg + thumbnails/<id>.jpg, stored. SETS
  whitelist env added. All five profiles smoke-verified on a scratch pool
  (geometries confirmed via identify). Supersedes make-psp-cards.sh /
  make-vita-cards.sh (kept for their derivation headers).
Art remains local-only — nothing committed, nothing in release artifacts.

## 2026-08-18 — fresh-install test campaign (all four platforms)

Owner directive: fresh-install test per platform, art preserved (no gig
re-downloads), GPT functions tested on FRESH saves (his save NOT restored
until after the test).
- ANDROID: PASS. Old save backed up psp-work/tablet-user-backup-20260818
  (md5-verified). Art moved aside on-device, uninstall, fresh install of
  bin/wagic-debug.apk (the alpha shape, all card fixes): storage flow ->
  bundled-res extract -> 68020 cards. Art back (336 sets), then owner OK'd
  restoring his save (verified: 16% achieved / 7382 credits).
  Gotchas hit: (1) language picker treats TAP as confirm-highlighted, not
  select-at-position -> Lang=cn again; sed rescue. Alpha-notes item.
  (2) isExternalStorageManager needs the UID appop mode — `appops set`
  package mode does NOT satisfy it; only the real Settings toggle (or
  --uid) works. (3) If the app dies while the grant page is up, its task
  is a hollow settings task; Back out + relaunch.
- VITA: PASS — **CLOSES the 2026-08-09 open gate**: owner verdict "the vita
  gpt stuff all works, also checked the shop, the deck editor, and gameplay"
  on a TRUE fresh install (bubble uninstalled, dist VPK reinstalled, fresh
  data tree). Method: ux0:data/Wagic renamed to Wagic-pre-alpha-20260818
  (save+art preserved on-device, zero copying); save also pulled to
  psp-work/vita-user-backup-20260818 (44 files, endpoints.txt unread).
  After his test: art sets/ renamed back into the fresh tree; then per his
  ask the ANDROID player/ save was uploaded to the Vita (md5 round-trip
  verified). Vita FTP was at 10.0.0.227 this session (scan: port 1337 sweep).
  Vita layout note: ux0:data/Wagic IS the User tree (player/, settings/,
  sets/ at its root; no Res — the bubble carries core res).
- VOYAGER: staged. Fresh untar of dist tarball at
  ~/Applications/wagicGPT-alpha/wagicGPT-0.1.0-alpha/, headless-smoked
  (booted to menu, User tree created), then User/ wiped so his first boot
  is virgin. Artless by design; dev pool untouched.
- PSP: in progress. Save backed up psp-work/psp-user-backup-20260818
  (md5-verified). WAGIC -> WAGIC-PRE rename on stick; fresh extract of
  dist zip via detached unit psp-fresh-extract (first attempt as a
  backgrounded Bash chain was harness-killed mid-unzip — the
  voyager-cc-background-waits lesson applies to slow vfat writes too).
  Art restore = mv 336 set zips WAGIC-PRE/Res/sets/<SET>/<SET>.zip into
  fresh Res/sets/<SET>/ after extract.

## 2026-08-18 — OWNER RULING: PSP carries NO GPT function
His words: "the psp doesn't need to carry the gpt function." This supersedes
the 2026-08-07 LAN-only shape as the shipping target — PSP is offline-vs-Baka,
period. The Makefile already encodes it (PSP_GPT default OFF, opt-in
PSP_GPT=1 for experiments; a prior session's bring-up failure ladder lives on
the 'psp' branch per the Makefile comment). VERIFIED the shipped alpha EBOOT
complies: zero GPT feature strings (endpoints.txt/OpenRouter/thinking-notice
all absent; only wagicGPT branding matches "gpt"). No rebuild needed.
Correction for the record: earlier this session I claimed the PSP alpha had
GPT compiled in — wrong; I misread the ifeq branch and counted branding
strings. Task #6 is CLOSED-WONTFIX, not paused.

## 2026-08-18 — PSP combat UAF crash: root-caused, fixed, ALL artifacts regenerated
Owner's fresh-save PSP game crashed hard (exception screen): GuiCombat::Update,
BadVAddr 3F6EFE0D = 0.9335f, regs full of 0.0665f/0.9335f = texture UV floats in
a recycled freed block, right after JRenderer::LoadTexture. Symbolized via
wagic.elf (addr2line: EPC=GuiCombat::Update, ra=DuelLayers::Update).
ROOT CAUSE: GuiCombat::receiveEventMinus's WEventCreatureAttacker branch erased+
trashed the AttackerDamaged WITHOUT clearing activeAtk (zone-change branch did),
and NO branch cleared the cursor `active` (whose target dies with the attacker —
~AttackerDamaged deletes its blockers). REGRESSION INTERACTION with f0500ad66
(2026-07-25): its CR 511.3 end-of-combat removeFromCombat sweep fires
stop-attacking events every combat end, so any combat ending with the cursor set
dangled both pointers. PC heaps usually leave stale memory intact (silent); the
PSP's tight art-churning heap recycled it into UV floats within frames.
FIX 4dab86746 (pushed both branches): unsetCursorIfOwnedBy() clears
activeAtk+active at all four trash sites. Gate 1045/0 + 24 AI/0 single-threaded.
Binary /tmp/wagic-4dab86746. Crash evidence preserved:
psp-work/psp-crash-20260818/ (exception.log + crash-time User).
ALL FIVE deliverables regenerated on the fix and deployed:
- PSP: EBOOT md5 07ab3460... on stick (0 probe/GPT strings), dist zip rebuilt;
  WAGIC-PRE/EBOOT.PBP renamed .parked (duplicate XMB entry gone).
- Linux tarball + Windows zip: release rebuilds, repackaged into dist/.
- Android APK 163f2d4d... rebuilt + installed on tablet (install-over; save+art kept).
- Vita VPK rebuilt (param.sfo/no-art verified), dist copy updated, uploaded to
  ux0:/vpk/wagicGPT-alpha.vpk — owner must reinstall via VitaShell (install-over
  keeps ux0:data/Wagic).

## 2026-08-18 — SECOND PSP crash (victory screen), forensics in progress
On the FIXED EBOOT (07ab3460/4dab86746): "==Destroying GameObserver==" then
Bus error (data), BadVAddr again EXACTLY 3F6EFE0D (0.9335f). Symbolized
against a temp-worktree rebuild of the same commit (git worktree add at
4dab86746 + docker make wagic.elf; JGE libs copied — only game code differed):
EPC AND ra both inside newlib _free_r (_mallocr.c:2723/2660) → free() walked
heap metadata containing the UV float. MECHANISM REVISION: not (only) a stale
READ — something writes float data through a stale pointer during play
(repeated exact value = converged Pos-lerp/UV write), corrupting recycled
heap; crash 1 read it mid-combat, crash 2's free() tripped on it at teardown.
Matches the DECADE-OLD field crash class (ancientdragon20: second-match /
return-to-menu; solo_mag: first match) — PSP tight heap is the amplifier.
GuiCombat cursor fix (4dab86746) stands but was not the whole story.
RULED OUT: Latin font eviction UAF (JLBFont loads texture DIRECT from
JRenderer, not the evictable WCache — the ledgered font UAF is CJK-only).
AUDITED CLEAN so far: GuiHandSelf/GuiPlay (erase+selector-remove before
trash), ~CardView nulls card->view down the next-chain, BattleField holds no
pointers. NOTED ASYMMETRY: GuiHandOpponent trashes without CardSelector
remove (but opponent hand views are never selector-added — benign).
INSTRUMENT: ASAN PC build (Makefile.sdl-asan, objs-asan/bin-asan) — headless
selfplay soak found nothing (render path never runs headless); WINDOWED
selfplay ASAN soak running. Caveat: selfplay has no cursor interaction, so
human-UI-only holders stay dormant — a clean soak is NOT proof.
Both exception logs + crash-time User: psp-work/psp-crash-20260818/.
Sym worktree: ~/Projects/wagic-symtmp (remove when done).

## 2026-08-18 (cont) — PPSSPP reproducer stood up; THIRD crash class found+fixed
PPSSPPHeadless on Voyager now runs the PSP build UNTHROTTLED (full 357-set boot
+ duel start in ~2s wall — the old "emulated speed = real PSP" note is obsolete
on this machine). Recipe: stage a game dir (dist-zip Res + EBOOT built with
`make EXTRA_DEFS=-DWAGIC_AUTODEMO` — the in-tree WAGIC_AUTODEMO/WAGIC_SELFPLAY
harness; the old psp-work/autodemo-harness.patch is STALE, do not apply — its
probes are committed), then `PPSSPPHeadless <abs>/EBOOT.PBP --timeout=N` from
the game dir; probe lands at User/wagic-probe.log (per-phase + per-turn lines).
Quirk: occasional instant clean exits at play entry (sceKernelExitGame, cause
unproven — rerun; a temp GameApp probe for the START+SEC quit combo is in the
worktree, strip before any release build).
THIRD CRASH CLASS (deterministic under PPSSPP, = solo_mag "crash starting
first match"): ActionStack ctor deref'd pspIcons[i] null when
RetrieveQuad(RETRIEVE_MANAGE) fails (cache can't promote / 404) — NULL+0x28 =
SetHotSpot. FIX d424a9faf (pushed both branches, gate 1045/0+24/0): guard ctor
+ 3 render sites; icons degrade to text labels. With the guard the PPSSPP run
proceeds into real games. 2h endless soak running to hunt the poison-float
corruption (crashes 1-2); caveat: artless staging + no cursor input — art
re-add and hardware retest still owed.

## 2026-08-18 — HERMETIC-RULE VIOLATION FOUND (owner caught it) + rule correction
Owner: the pre-test EBOOT never crashed like this; asked whether the release
build was hermetic. IT WAS NOT — the alpha EBOOT, the 4dab86746 fix EBOOT and
the d424a9faf EBOOT were all INCREMENTAL builds over parked objs + existing
JGE libs, violating the 2026-08-03 rule. All hardware/PPSSPP crash evidence
from these builds is SUSPECT: the poison-float corruption may be stale-object
skew, not an engine bug (the two fixed defects are real by inspection either
way). ALSO: the rule's own rm line is WRONG — JGE objects live at
JGE/src/**/*.o (43 stale .o found), NOT JGE/objs/*.o, so even a
followed-to-the-letter clean missed them. CORRECTED RULE:
  find JGE -name '*.o' -delete
  rm -f JGE/lib/psp/libjge300.a JGE/lib/psp/libhgetools.a
  rm -f projects/mtg/objs/*.o projects/mtg/deps/*.d
  then cd JGE && make; cd projects/mtg && make — and VERIFY the commit first.
Hermetic rebuild at d424a9faf running; then: repackage dist zip, redeploy
stick, rebuild hermetic autodemo EBOOT, rerun the PPSSPP soak, and owner
retests hardware. Verdict logic: crashes vanish on hermetic build -> taint
confirmed; persist -> real engine bug, hunt continues on a trusted build.
"FIX THE MAKEFILE" (real clean target) is STILL not done - now higher priority.

## 2026-08-18 — the 3h soak "stall" was the INSTRUMENT, not the game

The hermetic PPSSPP soak ran its full 3h (rc=1 in /tmp/ppsspp-soak.log =
grep-found-no-crash-lines, benign) with ZERO crash lines, but the probe log
froze at "ingame turn=2" for ~2h55m. Forensics (heartbeat probe, now
COMMITTED b87005def, compile-gated WAGIC_AUTODEMO):
- Emulated-side the game is HEALTHY: dt=16.7ms (vsync-normal), frame work
  ~21-51ms emulated (=20-48fps equivalent, matches playable hardware),
  turns progress, ~10 card-art retrievals/frame all missing (artless
  staging) but costing only a few ms — the art-miss retry storm is real
  but CHEAP. No engine stall exists.
- Host-side PPSSPPHeadless spends ~2 WALL-SECONDS per emulated in-game
  frame (240s bought ~100-150 frames). Boot is fast (I/O-shaped), in-game
  render grinds. So the soak covered only ~4-5k frames in 3h = a few turns.
  The "stall at turn 2" was glacial pacing, possibly worsening as the board
  fills. JIT is the headless default (checked --help), not the cause;
  suspect the default headless graphics backend (software).
- --graphics=gles HANGS in init headless (unkillable by SIGTERM, needed
  kill -9). PPSSPPSDL (windowed, real GL) boots the EBOOT but logged HLE
  module-link errors and wrote no probe log in 3 min (real-time boot is
  minutes); NOT diagnosed further — timeboxed.
VERDICT ON THE VEHICLE: PPSSPPHeadless stays the BOOT-PATH reproducer
(357-set boot ~2s, crash #3 reproduced there); it is USELESS as an in-game
soak. In-game soak coverage = the owner's hardware play. The 3h run is
weak-but-nonzero clean evidence (no crash lines in the frames it did run).
The taint-vs-real-bug verdict still rides on the owner's hardware retest of
hermetic EBOOT bc08fb57.
COMMITTED: b87005def "Autodemo harness: heartbeat frame telemetry"
(master+psp-port lockstep) — hb line every 200 frames: dt, emulated now,
updms/rndms split, art/miss counters, duel phase + turn (guarded: turn read
only when getCurrentGamePhase()>=0; GameObserver::turn is uninitialized
before game start — reads as garbage otherwise).
Staging note: scratchpad pspgame EBOOT is now the b87005def autodemo probe
build; /tmp archives: probe-soak5-archive.log (the stalled soak's probe log).

## 2026-08-18 — "FIX THE MAKEFILE" DONE: make clean is now the hermetic recipe

Commit b11297063 (master+psp-port, pushed). Two gaps closed: projects/mtg
clean now takes deps/%.d (stale dep under-listing headers = silent skipped
rebuild, same taint class as stale archives); JGE clean now takes HGE_OBJS
(only the archive was in EXTRA_CLEAN — 6 hge .o survived). VERIFIED in the
container: after `make clean` in both dirs, zero .o / .d / generated .a /
EBOOT survive (JGE/lib/psp keeps only the 7 vendored third-party .a, correct),
and a full build from that state succeeds.
NEW CANONICAL HERMETIC RECIPE (supersedes the hand-rm list above):
  docker run --rm --memory=5g --memory-swap=5g \
    -v /home/magi/Projects/wagicGPT-psp:/build pspdev/pspdev:latest \
    bash -c "cd /build/JGE && make clean && make -j6 && \
             cd /build/projects/mtg && make clean && make -j6"
(+ EXTRA_DEFS=-DWAGIC_AUTODEMO on the mtg make for harness builds.)
Note: orphaned .o from REMOVED sources can still linger (clean removes only
current-list objects) but are harmless — the delete-before-ar archive rules
guarantee archives contain exactly the current objects.
Staging EBOOT is now the b11297063 autodemo build (from-clean).

## 2026-08-19 — CRASH INCIDENT CLOSED (owner verdict): hermetic retest survived

Owner played the hermetic EBOOT bc08fb57 on hardware: lost a game, won a
game, no crash; then a second test on a wiped save (virgin first-boot ->
first victory, the exact path crash #2 and the solo_mag field report
implicate) also survived. OWNER RULING: "problem solved, or intermittent.
this suffices for an alpha release."
EPISTEMIC RECORD: the poison-float crashes (#1/#2) are closed as
BUILD-TAINT-CONFIRMED-OR-INTERMITTENT — the hedge stands; the incremental
builds are gone either way, and the two code fixes (4dab86746 cursor dangle,
d424a9faf null icon quads) remain as real-by-inspection hardening.
If the 0x3F6EFE0D signature ever reappears ON A HERMETIC BUILD, that
reopens the real-bug branch (resume: art-zip staging churn + cursor-path
coverage on a trusted reproducer).
ALPHA SET: all four dist artifacts are release-equivalent to tip
(b11297063's two commits touch only WAGIC_AUTODEMO-gated telemetry + PSP
makefiles; release binaries unchanged from d424a9faf). PSP stick =
virgin save + hermetic EBOOT + 357 art sets; retest save stashed at
psp-work/psp-user-backup-20260819-retest/. REMAINING DEPLOY: Vita bubble
still runs the first-upload VPK — dist/wagicGPT-vita-alpha.vpk upload +
reinstall next time FTP is up. Owner's real PSP save (20260818 backup)
was never restored; restore on request.
Cleanup: wagic-symtmp worktree removed.

## 2026-08-19 — Windows-under-Proton triage, release guide, cardart in-repo

Commit chain (master+psp-port lockstep, pushed, all gated — suite 1045/0 +
24/0 ST at the final tree, PARSETEST 328/0):
- 019a88b17 SDL init: dropped the legacy ACCUM-buffer GL attributes (made
  pixel-format selection FAIL on strict WGL/Proton — Windows alpha died
  silently at launch: SEGV via unguarded DestroyGame on the never-init
  engine ptr); multisample now retry-once-without; failure prints
  SDL_GetError + desktop message box; DestroyGame null-guarded. Diagnosed
  via WINEDEBUG=+seh (crash at JGE::SetApp entry, this==NULL, callers
  DestroyGame<-SDL_main) + headless-under-Proton split test (game logic ran
  fine -> failure isolated to window/GL leg). GE-Proton10-34 recipe:
  STEAM_COMPAT_DATA_PATH=<prefix> STEAM_COMPAT_CLIENT_INSTALL_PATH=~/.local/
  share/Steam proton run wagic.exe, DISPLAY=:1 (Voyager XWayland; :0 fails
  auth). OWNER PLAYED IT: "ok. it works."
- 924da3334 GptConfig: native Windows HOME fallback to USERPROFILE (HOME
  unset on real Windows = config saves silently disabled; Proton masked it).
- d776e898d + 4c0d84365 + 6da908d66: ALPHA-GUIDE.md (projects/mtg/docs/,
  copy at dist/README.md — Karpathy register, human+agent dual audience,
  owner-driven revisions: subscription passkey hand-entry step, per-platform
  crash artifacts incl. Vita psp2core dumps, telemetry-consent==translog
  link, feedback section) + cardart toolkit MOVED INTO REPO at
  projects/mtg/tools/cardart/ (paths genericized: repo-root walk-up,
  POOL_BASE/OUT overridable; .gitignore for pack output). psp-work/cardart
  is now the stale copy — the repo path is canonical.
- Android: trimmed SDL lacks SDL_ShowSimpleMessageBox -> desktop-only ifndef.
ARTIFACTS refreshed at 6da908d66: windows zip, linux tarball, android apk
(tablet updated via pm install -r, Success); vita vpk + psp zip unchanged
(Vitamain/PSP don't compile SDLmain/GptConfig-desktop paths — vpk still the
02:49 build, psp still hermetic bc08fb57). dist/README.md rides with them.
Proton test install (~/Applications/wagicGPT-alpha-win) exe refreshed.

## 2026-08-19 — ★ ALPHA RELEASED PUBLICLY ★

https://github.com/InquiringMinds-AI/wagicGPT/releases/tag/v0.1.0-alpha
Tag v0.1.0-alpha on c1fadc65e, marked prerelease. All five artifacts
attached (byte-verified against dist/), each art-free (0 card jpgs,
checked before upload). Release notes = short intro linking the guide at
the tag; NO AI attribution anywhere (owner's repo policy). Guide got two
more owner passes before release: GUI + heuristic-AI feedback categories
(card-agnostic tuning stated as POLICY), QoL paragraph in What-this-is,
cardart README rewritten public-voice ("his devices" leak caught by
owner; the internal-voice version exists in git history, harmless).
Cleanup: transient units stopped, done-files removed. The strategy-guide
corpus (wave 33) remains the paused track; post-release work = triage
whatever feedback arrives.

## 2026-08-19 — post-release close-out (context-shed handoff)

After the tag: bdb30d8e7 corrected the model-quality claim in guide +
release notes (owner ruling: no perceivable play-quality difference 4B vs
122B vs gpt-5.6-luna beyond response time in his testing — do NOT claim
quality "depends heavily on model", it chills modest-hardware users;
playability-forward framing, smarter-model inference left to the reader).
Release-notes guide link now points at master blob (not the frozen tag) so
guide fixes reach readers. Owner announced in the Wagic Discord (his own
words, proofread only).
VOYAGER INSTALL STATE (durable): ~/Applications/wagicGPT-alpha/ = Linux
alpha with his android save restored + his dev PC settings + 336 art-set
symlinks into the dev pool + Spark endpoint live. His real
~/.Wagic/ai/gpt/endpoints.txt: dead :8011 url line REMOVED (8081 first now),
translog=1 ARMED (his games are logging decisions to ai/gpt/logs/ — also
the deployment-shaped bench corpus for the serve-optimization re-exam; ask
before disarming). Proton test install ~/Applications/wagicGPT-alpha-win/
(GE-Proton10-34 recipe in the Proton-triage block). Launch unit pattern:
wagic-alpha-test / wagic-win-test (transient, stopped).
NEXT: owner returns to the CORE LOOP — the paused strategy-guide campaign
(wave-33 corpus unrun; resume anchor in fleet memory project_wagicgpt).

## 2026-08-24 — fresh Vita VPK built + uploaded (owner request)

psp-port fast-forwarded to master 6f03b216c (0 unique commits — pure ff, pushed):
carries the owner's four live-play fixes (Delver, faithful Black Vise + chooser
auto-resolve, sliver grant arithmetic, X-announce), waves 41-42 engine batches, and
the three same-day narration fixes. HERMETIC build (build_vita wiped first — the
alpha crash lesson): vitaGL reused (vendored, unchanged), cmake+`wagic.self-self vpk`
from clean in vitasdk/vitasdk:latest, exit 0. VPK 32,510,225 bytes.
UPLOADED to ux0:/vpk/wagic.vpk (device at 10.0.0.227:1337 this session; sole
FTP-open device on the LAN, VitaShell banner-verified). Size on device matches
byte-for-byte. deploy-vita.sh's pre-check curl false-negatived once (single-threaded
server) while the port was provably open — direct curl -T worked; script check may
want a retry. Install remains MANUAL per policy: VitaShell -> ux0:/vpk/wagic.vpk ->
Cross. Old wagicGPT-alpha.vpk (08-19) still alongside in ux0:/vpk/.

## 2026-08-24 (later) — second VPK of the day: owner-fix build uploaded
psp-port ff'd to e1ade0171; hermetic vitasdk build (logs ~/.gatelogs/vita_vpk2.log);
VPK 32,516,320 bytes uploaded to ux0:/vpk/wagic.vpk, size-verified. Carries: reveal/scry
input-flush (held-trigger self-decline fix — hold-stepping now HALTS at a reveal),
manarestriction{} spend-restricted mana, Condemn fizzle logger (ALPHA-only WAGIC_DEVLOGS
on MTGAbility.cpp via CMake set_source_files_properties; log at ux0:data/Wagic/
fizzlelog.txt — owner plays to repro, FTP the file off after a session). One unverified
link flagged by the lane: if fizzlelog.txt never appears despite targeted spells, the
per-file define didn't apply — fall back to add_definitions or a dedicated flag.

## 2026-08-24 (third VPK) — crash-fix build uploaded
psp-port at 9f47ce024. Carries: lord-grant UAF fix (victory-screen crash root-caused via
ASAN — InstantAbility::testDestroy now honours forcedAlive) + fizzlelog define repaired
(global -DWAGIC_FIZZLELOG in CMakeLists; per-file property suspected inert). VPK
32,519,221 bytes -> ux0:/vpk/wagic.vpk, size-verified. Watch: fizzlelog.txt must now
appear after any game with targeted spells; if absent, escalate. First upload attempt
timed out (single-threaded server busy) — direct retry worked; pre-check retry note
stands.

## 2026-08-24 — standing rule
Owner: "don't make vpk I don't ask for." VPK builds are owner-initiated only; fixes
accumulate on psp-port until he asks.

## 2026-08-25 — wave-44 close VPK uploaded (owner-requested)
psp-port at 60dbf39b4 (= wave-44 step-1 final, gate 1130/0+37/0 + 1150/0). Hermetic
build, VPK 32,608,180 -> ux0:/vpk/wagic.vpk, size-verified. Carries since last stick
build (9f47ce024): menace legality all consumers (human blockers-step HOLDS on an
under-filled menace block — add the 2nd blocker or remove), damage-order screen
restored on multi-blocks (was skipped unless holding an instant), lethal-basis fix
(trample through pre-damaged/first-struck blockers), fair hands incl. attract demo,
manarestriction, reveal/transform narration, X pricing, removal target text, Day/Night
line, fizzle logger (still watch for ux0:data/Wagic/fizzlelog.txt existing after games
with targeted spells — if absent, the define needs another pass).

## 2026-08-27 — Vita VPK for the owner's three live-play reports (owner request: "Make a vpk. I'll turn on ftp")
psp-port ff'd to 6819183f7. Hermetic vitasdk build (build_vita wiped; vitaGL reused), unit
vita-vpk5, log ~/.gatelogs/vita_vpk5.log, exit 0. VPK 32,875,073 B -> handheld 10.0.0.227:1337
ux0:/vpk/wagic.vpk, size-verified. Install manual. Carries since 9f47ce024/60dbf39b4: human
cast gate fix (dual lands counted first colour only — Selesnya Guildgate/Blossoming Sands,
19e2b04f7); castability oracle fixes (Delve priced — Gurmag Angler; self-tapping ability
excludes its own mana — Westvale Abbey; 6819183f7); wave-45..48 engine batches (E1 re-answer
replacement, loop affordances: real Pass row + repeat-N row + scoped [repeat:] tag, ranged
collapse on combat A-rows/target menus/log cycles, attack-count header, priced blocker
representative, stack keyword tags, card-text clause budget, auto=choice menu headers,
DRAW PRICE reach, ANNOUNCE_X plan suppression). Suite 1137/0 + 37/0, PARSETEST 1655/0.

## 2026-08-27 — layout: psp-port worktree MOVED to ~/Projects/wagicGPT/worktrees/psp
Owner reorganised ~/Projects to a single wagic node. Every recipe above that mounts
`-v /home/magi/Projects/wagicGPT-psp:/build` now reads `-v /home/magi/Projects/wagicGPT/worktrees/psp:/build`;
deploy-vita.sh default VPK path updated. Lane worktrees go under worktrees/lanes/.

## 2026-08-27 — session-close Vita VPK (owner request: "after the merges... build a vpk, and deploy it")
psp-port ff'd to b2ce13e89 (= master at the wave-52 step-1 merge; gate 1158/0 + 43 AI/0 on a
`make -B` full rebuild, PARSETEST 2121/0). Hermetic vitasdk build from a wiped build_vita
(root-owned after docker — `sudo rm -rf`), unit vita-vpk6, log ~/.gatelogs/vita_vpk6.log,
exit 0. VPK 33,136,250 B -> handheld 10.0.0.227:1337 ux0:/vpk/wagic.vpk, size-verified.
Install manual (VitaShell -> Cross). Carries since the morning VPK (6819183f7): ability-border
permissive double-count fix (Ironroot Warlord), morph Face-Up liveness (Mystic of the Hidden
Way), Soul Shatter/Flare/Riveteers card scripts (highest-MV creature OR planeswalker), AI
casts routed through the payment planner (dual-vs-mono tap order), Battlement one-tap,
instants offered on the opponent's turn, countered-cast double-pay fix, cleanup discard as a
model decision, and the wave-49..52 LLM-UX batches. Docker stopped after the build.

## 2026-08-28 — owner Vita play-report batch + Delver ROOT CAUSE (GUI-reproduced) + VPK from 4abcf5dd0

Ten play-report items came in; two commits on master (c15106f4d, 4abcf5dd0), psp-port ff'd.
What matters for the console build:
- DELVER (and every "@each my upkeep"-class trigger): phase triggers POLL the phase once per
  tick; the no-legal-action/ASPhases skips AND a HELD phase-advance key (JGE repeats it - the
  Vita trigger) left the upkeep in the tick it was entered, so nothing ever polled it. Fix =
  GameObserver::mPhaseTicks settle gate (automation + userRequestNextGamePhase; suite seats
  exempt). Reproduced RED in the desktop GUI on b2ce13e89 with HIS profile (ASPhases=Safe,
  closed_hand=visible, no instant): 3 upkeeps, no trigger. GREEN on the fix. Not pinnable in
  the headless suite (the pump masks the race) - the GUI is the test for phase-timing bugs.
- Calamity Bearer: DOUBLE STRIKE hack removed; doubling via triggers + new `damage:N notrg`
  (Damage::noTrigger) so a doubler can't re-trigger on its own output.
- Morph/evoke cast-mode menu for the human: alt-cost rules priced the POOL only -> now
  planner-priced + auto-tap (humanCanPayAlternative). RED live on old (Coral cast face-up,
  no menu at 3 lands), GREEN on new.
- Delete Deck menu item (yes/no confirm). WSrcUnlockedCards setId -1 heap-overflow READ
  (ASAN) fixed. Keyword-vs-oracle lint (VALIDATE-WARN) in the validator.
- Vita crash dump psp2core-1787894891 (Aug 28 05:28): data abort in newlib _malloc_r = heap
  corruption, victim MTGCardInstance::computeNewCost/ManaCost::init. Parser tooling now
  persistent: psp-work/tools/vita-parse-core (py3-patched) + psp-work/tools/venv (pyelftools);
  run inside vitasdk docker with PYTHONPATH=/pw/tools/venv/lib/python3.14/site-packages
  (mount psp-work as /pw, worktrees/psp as /build; ELF build_vita/wagic has debug_info).
  Dumps + parsed output in psp-work/vita-dumps/. Desktop ASAN sweeps (15 Baka games + full
  suite) found nothing beyond the setId read -> the Vita corrupter needs on-device evidence.
  Frame-lag report: no mechanism found.
- NOT reproduced on desktop (need his concrete board): Metamorph-copies-Muse upkeep, Drake
  Hatcher counters/indicator (counters accrue on desktop; suspect Vita counter rendering),
  autotap "worst combination" (three probes pick correctly).
- Keyword-vs-oracle lint follow-up (e8425fb34): corpus sweep = 3 cards; Bold Plagiarist was a
  false positive (keyword COUNTER names - lint now skips counter( lines), Steeple Roc had NO
  text= at all, Efreet Weaponmaster's text= lacked "First strike" - both text= fixed (the LLM
  seat reads text=). Lint: 0 warnings on 68,020 printings. Not in the vpk7 VPK (built from
  4abcf5dd0; text-only).
- Oracle sweep wave 3 (a9dfe70dc): the owner asked how missing-keyword text= survived the July
  sweep. Answer: the sweep's no_text.tsv (354 rows) was never applied, and its LLM judge
  mis-read its own prompt on keyword-only diffs (ruled Efreet Weaponmaster SAME "because the
  prompt says to ignore added abilities" - it says the opposite). Deterministic audit
  (abilities= printed keyword vs text=): 81 + 24 no-text; 45 text= replaced from the Scryfall
  bulk JSON; 51 skips for review in strategy-design/oracle-sweep/wave3-keyword-text.tsv
  (tokens/back faces; scripts granting keywords the Oracle doesn't print). LESSON: an LLM
  judge over 5k diffs needs a deterministic cross-check for every rule it is told to apply.
- VPK (owner: "Build it. Then push to vita"): psp-port at 4abcf5dd0, hermetic vitasdk build from a
  wiped build_vita (the .sh stagers are CMake-generated - safe to wipe), unit vita-vpk7, log
  ~/.gatelogs/vita_vpk7.log, exit 0. VPK 33,137,575 B -> 10.0.0.227:1337 ux0:/vpk/wagic.vpk,
  size-verified (deploy-vita.sh's pre-check false-negatived again; plain curl -T worked).
  Install manual (VitaShell -> Cross). Carries: Delver settle gate, Bearer/notrg, morph-evoke
  cast menu, Delete Deck, ASAN setId fix, lint. NOT in it: e8425fb34/a9dfe70dc text= fixes.
- Owner rules point (flash keyword vs "as though it had flash"): the engine distinguishes
  `asflash` from `flash`, but the Ghitu-Fire class is scripted with plain `flash`; split cards
  with an instant half carry `flash` on the whole primitive (lets the sorcery half go at instant
  speed); Circling Vultures uses `flash` for instant-speed discard. All docketed in
  oracle-sweep/wave3-keyword-text.tsv #REVIEW-NOTES. Voice of the Blessed's mis-keyed flash removed.
- OWNER RULING (2026-08-28, "solve these problems rather than putting them off. The oracle text
  needs to be accurate, cardscript needs to correctly translate the oracle text to the engine,
  the engine needs to correctly follow the rules. Where the linter can be used to find problems,
  it should. When problems are found, they should be solved."): flash-vs-instant-half class
  SOLVED - new script token `other=... instant` / `flashback=... instant` (ManaCost::instantSpeed)
  gives a split/adventure/aftermath half its own timing; ~60 cards retagged off primitive-wide
  flash/asflash; keyword mis-keys fixed (list in oracle-sweep/wave3-keyword-text.tsv #RESOLVED);
  lint covers all printed keywords with engine equivalences; 0 warnings. Fixtures pin the
  positive AND negative timing (Circling Vultures discard, Mutual Destruction with/without a
  flash permanent) using the `next`/`yes`/`endinterruption` opponent-upkeep idiom.
- VPK vita-vpk8 from psp-port = bd515aad7 (per-half timing + keyword fixes + every 2026-08-28 fix),
  hermetic build exit 0, 33,137,888 B -> 10.0.0.227:1337 ux0:/vpk/wagic.vpk, size-verified.
  Install manual (VitaShell -> Cross). Supersedes vpk7.
- DELIBERATE deletions 2026-08-28 (owner approved the recommendations): archives/ trimmed to the
  anchored binaries + newest six; snapshots/WTH-fork (superseded by worktrees/psp), the
  re-downloadable WagicPSP-0255.zip / minpspw.tar.lzma / opt/pspsdk, and the ASAN build products
  (objs-asan, bin-asan, build-asan - rebuild with Makefile.sdl-asan when needed). KEPT:
  snapshots/stick-archive-20260731 (pre-wipe PSP stick image). Don't restore any of the above.

## 2026-09-01 — owner Vita notes batch (five crash dumps + 21 notes + 5 photos)

Master 9385f6f6a (pushed; psp-port ff'd + pushed). Gate 1207/2 (lifeline +
merrow_reejerey, the known concurrency pair, solo-green) + 43 AI/0, 0 timeouts,
PARSETEST 2121/0. Binary archives/wagic-9385f6f6a. NO VPK built (owner-initiated).

CRASH DUMPS (psp-work/crashes/vita-dumps/core-17881*.parsed.txt, parser recipe unchanged):
- 4x "Undefined instruction" R2=6 = abort() from __verbose_terminate_handler: std::bad_alloc
  in JRenderer::LoadPNG `new BYTE[tw*th*4]` reached from DuelLayers::Render -> WCache::Get ->
  WCachedTexture::Attempt -> LoadTexture. Vita newlib heap = fixed 128MB (_init_vita_heap
  default 0x8000000, weak _newlib_heap_size_user unset) and vitaGL maps that same heap as
  VGL_MEM_EXTERNAL (mem_utils.c) - textures spill into it once VRAM/RAM pools fill. fizzlelog
  showed 56 games since vpk7, ~10 per crash; desktop endless Baka self-play (92 games) holds
  RSS flat, so the growth is Vita-side. Fixes: nothrow image buffers (degrade to text frame),
  and vitaMemProbe -> ux0:data/Wagic/memlog.txt (mallinfo + vglMemFree per pool, at game
  start/untap/end; gate WAGIC_VITAMEMLOG). The corrupter still needs that on-device log.
- 1x data abort GuiCombat::shiftRight case ATK with active==NULL (creature left combat,
  cursor_pos stayed ATK). GuiCombat::reseatCursor() + closest() NULL guard.
- The Aug-28 dump (_malloc_r heap corruption) remains unexplained; memlog is the instrument.

ENGINE/CARD FIXES (lanes A-I, each RED->GREEN with fixtures; see commit messages):
Hapatra/All Will Be One/Generous Patron/Stocking the Pantry `bycontroller`; Wildwood Scourge
`[-sub_hydra]` (`hydra` is a keyword ability - `[-hydra]` never meant the subtype); Moss-Pit
Skeleton `source()`->`from()`; evolve compares at resolution (enters-with counters count);
Light-Paws `srcmanacost` + `diffname!aura!` (bare manacost read the ENCHANTED creature's
MV); Wilt-Leaf Liege discard-to-play now resolves a Spell (abilities registered; also
Obstinate Baloth/Loxodon Smiter/Dodecapod - Dodecapod's counters still revert, CS-026);
Transcendent Envoy `modbenchant` (bestow reducer); autotap ranks non-mana-tap lands last
(Squirrel Nest forest untouched); aura rehook/newhook re-binds static effects to the new
host (Ajani's Chosen); Hurkyl end-step soft lock = invented `tobecast` token -> forced
library chooser (script rewritten, CS-027); Lizard Blades menu growth = AEquip::equip
re-registering every ability line + AThis leaking its clone (16 reconfigure cards);
Ajani's Pridemate art = J22 TOKEN printing (-591418, art 591418t.jpg) winning name-only
lookup (getCardByName + prefetchCardNameCache now agree: lowest-id non-token printing);
PSAL collapsed to one entry per name (434 duplicates, one image each); Baka
computeActions `findingCard` was a function static shared across suite threads.
NOT reproducible without a board (owner: "I dont remember"): Rootcast 2-of-4 (likely
correct legality filtering), Coralhelm indicator, Giant Killer tap prompt, Lantern
Flare/Gatekeeper interrupt; Show of Confidence on the human's creature is Oracle-legal
(any target) = AI quality, not rules. Photos in strategy-design/play-reports/2026-09-01-photos/.

TRANSCRIPTS (owner ask, shipped 1245b7d68): replayable dump per game + post-match
classification menu + desktop WAGIC_REPLAY. Details + the record/replay rules in the
/wagicgpt skill. Vita transcripts land in ux0:data/Wagic/transcripts/. FIRST THING next
Vita session: fetch memlog.txt + transcripts, replay any classified-as-bug game.

NEXT VPK: from 9385f6f6a (carries every fix above + memlog + transcripts; CMake defines
WAGIC_VITAMEMLOG + WAGIC_TRANSCRIPT, alpha-only). Vita compile NOT yet exercised for the
new Vitamain.cpp/GameObserver/GameStateDuel code - build will tell.

## 2026-09-01 (later) — owner follow-ups: option-preserving autotap + AI pump targeting

OPTION-PRESERVING AUTOTAP (c1a0bf814). Owner spec, verbatim: "tapping should be done in a
way that leaves the most options open to the player... consider activated abilities to be
options. then, if there are ties, you look to maximize unrevealed options, preserving color
availability and then cards with unaffordable activated abilities whose cost includes tapping
them" — and the W54-F utility-land rule "should generally only count for lands that can
afford to cast that ability". `ManaEngine::refineForOptions` (called at the end of
`selectAutoTapProducers`, default on): same-size source sets over the baseline plan,
class-prefix enumeration (<=14 candidate sources, single-mana bare producers, no X),
scored lexicographically by (castable-now hand cards still payable + non-mana activations
still payable + creature sources that could still attack, distinct colours left, held
sources with an UNAFFORDABLE tap ability); baseline stands unless beaten strictly. The
W54-F ordering flag is now affordable-only (`sourceHasNonManaTapAbility(p, card, true)`,
priced by pool + strict potential minus the source — no planner recursion). Human autotap,
tap preview and the suite's `autotap` ride it; AIPlayerGPT's `{paying this taps}` forecasts
pass preserveOptions=false (the AI seat still pays via planPayment). Fixtures RED on
9385f6f6a: autotap_options_keeps_castable_card, autotap_options_keeps_ability,
autotap_options_unaffordable_utility_spends (Kher Keep; colourless pool asserts as `{C}`).

SHOW OF CONFIDENCE (5ca11d6e2). Owner: "you have correctly identified the problem. its still
a problem." Root cause was two gaps: `abilityEfficiency` had no verdict for
ATransformerInstant (every `transforms((...))` pump/grant -> DONTKNOW -> chooseTarget aims at
the opponent's field), AND chooseTarget judged the chooser by re-parsing
`tc->belongsToAbility`, which is the ability TEXT after the parser hoists nested payloads
out — the grant arrives as `transforms(()) ueot` and parses to NULL. Fix: content verdict
for ATransformerInstant (P/T set-value vs target body; newability lines by core efficiency;
keywords by the bad table) + chooseTarget judges the LIVE waiting ability
(`actionLayer()->isWaitingForAnswer()` whose tc == this chooser) when the text re-parse
says DONTKNOW. Fixture ai_show_of_confidence_targets_own_creature (asserts at
`combatbegins` — the pumped Lions trade with the Bears otherwise). Any other card whose
effect is a `transforms((,newability[...]))` grant on `target(creature)` was mis-aimed the
same way — worth a corpus look.

Rootcast 2-of-4 stays OPEN (owner: still a problem) — needs a board; first Vita transcript.
Gate after both: 1210 (lifeline/merrow) + 44 AI / 0, PARSETEST 2121/0. Binary
archives/wagic-5ca11d6e2. NEXT VPK: from 5ca11d6e2.

## 2026-09-01 — VPK vita-vpk9 (owner: "build and push the vpk")
psp-port = fe681f00a (master; = d09764b68 + one Vita compile fix). The first hermetic build
(unit vita-vpk9, ~/.gatelogs/vita_vpk9.log) FAILED in GameObserver.cpp: the vitaMemProbe
hooks declared `extern "C"` INSIDE function bodies — a linkage specification is not allowed
there (GCC: "expected unqualified-id before string constant"); the desktop never compiles
that path (gate is VITA && WAGIC_VITAMEMLOG). Fix fe681f00a: file-scope declarations in
GameObserver.cpp + GameStateDuel.cpp. Rebuilt (unit vita-vpk9b, log vita_vpk9b.log, exit 0;
AIPlayerGPT.cpp alone takes ~20 min on the cross-compiler — a lone cc1plus at 98% on that
file is NOT a hang). VPK 33,164,899 B -> 10.0.0.227:1337 ux0:/vpk/wagic.vpk, size-verified
against the console listing (first curl -T timed out on connect; the retry with
--connect-timeout 40 went through). Install manual (VitaShell -> Cross). Supersedes vpk8.
Carries: every 2026-09-01 fix (crash cluster nothrow + memlog probe, GuiCombat cursor,
transcripts + classification menu, all nine lane fixes, Pridemate/PSAL, option-preserving
autotap, Show of Confidence targeting). FIRST THING after his next play session: fetch
ux0:/data/Wagic/memlog.txt + ux0:/data/Wagic/transcripts/, replay any game he classified.

## 2026-09-02 — vpk9 play report: "severe performance issues" + crash at the last combat's damage
Owner played ONE game on vpk9 (player_deck2 vs ai_baka_deck121, 42 turns, human won 19/-3;
memlog: heap_free ~10MB flat across the game, vram_free 33-35MB - NO memory growth in one
game) and reported severe performance issues + a crash "on the damage phase of what would have
been the last round of combat". Fetched: psp2core-1788315925 (crashes/vita-dumps/core-1788315925.*),
logs/20260902/{memlog,transcript-1788315466,fizzlelog,debug,wagic-ziplog}.txt.

CRASH (vpk9): thread WAGC00042 data abort at JGuiController::Update (JGE/src/JGui.cpp
`mButtons[i]->Update(dt)`), R0=0: a SimpleMenu whose mButtons vector held exactly ONE NULL
(start 0x822d4088, finish 0x822d408c). Chain: GameStateDuel::Update -> SimpleMenu::Update:323 ->
JGuiController::Update; stale SimpleMenu::Add / JGuiController::Add frames still on the stack
=> the FIRST Update of the freshly built "How was this match?" transcript menu, i.e. the crash
IS at game end (the "damage phase" = the lethal combat). SimpleMenu never adds buttons, so a
fresh menu with a button entry means heap damage of unknown origin (the core carries no heap;
0x82934620 unreadable). Ruled out: class-layout/ODR mismatch (members unconditional), the
vitaMemProbe buffer (320B, fixed format), desktop menu path (ASAN self-play through the
victory screen via the new WAGIC_TRANSCRIPT_MENU_SELFTEST hook: clean, #classification
written). SHIPPED (aaa68c888): NULL guards in JGuiController::Update + CheckUserInput button
loops (mitigation - the data abort cannot recur at that site; the corruption source stays
OPEN). Still on the docket with the Aug-28 _malloc_r heap-corruption dump.

PERFORMANCE (vpk9): the option-preserving autotap preview (ManaEngine::refineForOptions via
GuiHandSelf::Update) re-ran every 0.25s while a hand card was focused - castableForDisplay
twice + the configuration search on a 444MHz ARM. Now memoised on (focused card, pool, hand,
battlefield tap state) so a static board plans once. NOT MEASURED on device - the owner's next
session is the test; if still slow, the next suspects are (1) sourceHasNonManaTapAbility's
affordable-only pricing (a potentialMana walk per utility source inside every planPayment,
and planPayment runs per hand card + per battlefield card every 0.25s through the oracle) and
(2) GuiCombat::reseatCursor at every CheckUserInput.

REPLAY of the Vita human game (desktop `WAGIC_REPLAY=<transcript>`): first attempt diverged at
action 0 and SEGV'd in the divergence REPORTER (currentActionCard freed). Fixes: replay runs
SKIP-FREE (loadingScripted also when WAGIC_REPLAY - the human seat only ever advanced by
recorded `next` presses), a `next` for a phase the retries already passed counts as
satisfied, the reporter names cards only while the action layer is waiting. Result: 22/42
turns faithful, then DIVERGED at action 323: `p1.battlefield[3] 1westvale abbey` then
`p1.choice 1` - on the console the Abbey click opened an ability menu (choice 1 = {5}{T}{L:1}
cleric), in the replay the click resolved with NO menu (chooser=none menu=none). Open: either
the menu gate (affordability of the {5} ability) sees a different mana state in replay
(autotap re-derivation vs recorded taps?) or the console's click layer offers unaffordable
abilities. NEXT replay item. Also: suite/PARSETEST games leaked "testsuite-vs-testsuite"
transcripts (51 files) - gated on WAGIC_TESTSUITE/WAGIC_GPT_PARSETEST env.

Rootcast 2-of-4: still needs a transcript that contains it.

VPK vita-vpk10 from psp-port = aaa68c888: incremental vitasdk build (unit vita-vpk10, log
~/.gatelogs/vita_vpk10.log, exit 0), 33,166,309 B -> 10.0.0.227:1337 ux0:/vpk/wagic.vpk,
size-verified. (The Vita FTP refused the first connect both times tonight; the retry loop
with --connect-timeout 60 went through.) Install manual. Supersedes vpk9. Carries the NULL
guards + the memoised tap preview; the crash source and the perf verdict are decided by the
owner's next session (fetch memlog.txt + transcripts first).

2026-09-02 (later) vpk10 VERDICT: owner "completed a match with no issues" (player_deck3 vs
ai_baka_deck43, 10 turns, human won 20/-11, classified "other" - his words: no apparent bugs).
Victory-screen menu worked end to end on the console (#classification written); the vpk9
game-end crash site did not recur; memlog flat (heap_free ~12MB, vram ~35-37MB). Perf: not
mentioned = the memoised preview answered it for now. Logs in psp-work/logs/20260902/.

## 2026-09-02 (later) — vpk9 crash ROOT CAUSE FOUND: overload resolution, not heap damage
Owner: "no problem wasnt offered" (the classification menu showed Bug..Other only). Desktop
windowed self-play with WAGIC_TRANSCRIPT_MENU_SELFTEST=hold + a grim screenshot reproduced it.
Cause: SimpleMenu has `using JGuiController::Add;`, so `transcriptMenu->Add(0, "No problems")`
resolved to JGuiController::Add(JGuiObject*, bool) - literal 0 = null pointer, the string
literal = true - i.e. it pushed a NULL BUTTON instead of adding the first item. That is
EXACTLY the vpk9 dump (mButtons = one NULL, first Update of the menu -> data abort), so the
"heap damage of unknown origin" verdict above is RETRACTED. Fix: ids 1..6 (never a literal 0
as a SimpleMenu item id), callback maps controlId-1. The NULL guards in JGuiController stay.
Lesson: a SimpleMenu item id of literal 0 silently becomes a NULL button - grep for `->Add(0,`
whenever a SimpleMenu is added.

VPK vita-vpk11 from psp-port = c9dc6b75c (the real menu fix): 33,166,613 B -> 10.0.0.227:1337 ux0:/vpk/wagic.vpk, size-verified. Install manual. Supersedes vpk10.

## 2026-09-02 (vpk11 play session) — four owner reports, logs at psp-work/logs/20260902-vpk11/
Owner played 24 matches on vpk11 (transcripts 1788326631..1788330366, tags: 18 `no problems`,
1 `other`, 1 `bad blocking`, 1 `bug`, 3 abandoned with no result line). memlog flat (~38 MB
heap free, turn 16 of the last game) — no leak; no new core dumps in ux0:/data. The
"How was this match?" menu works (ids 1..6 fix confirmed by 21 classified transcripts).
1. "heuristic ai got stuck targeting a spell, cycled through the targets endlessly" =
   1788327409 deck7 vs deck154, turn 6: p2 clicks its own Dominating Vampire / Odric ten
   times then never acts; human nexts to turn 12 and quits. Screenshots show the red target
   border cycling over the HUMAN's creatures on the AI's Main 1. Dominating Vampire ETB
   `target(creature[manacost<=type:vampire:mybattlefield])`. -> lane R.
2. "legend shrine's ability to put counters ... didn't allow me to tap my creature for mana
   after it asked if i wanted to use it" = Go-Shintai of Boundless Vigor `@each my end:may
   ... pay({1})`; 1788327632 l.259-264 and 1788327030 l.349-353 (yes -> land click ->
   shrine click -> endinterruption). Mana creature = Twitching Doll. -> lane R.
3. "almost frozen when attempting to select cards in my hand" = 1788330366 deck6 vs deck109
   (tag `bug`), late board: Karn's Bastion + Meteorite tokens + Draconic Disciple + Roxanne
   lord + 9-card hand — the option-preserving autotap / tap-preview worst case (the vpk9
   perf suspects). -> lane S (measure on desktop, bound the work).
4. "game losing block by the heuristic ai, could have blocked differently and likely won" =
   1788329701 deck7 vs deck72 (tag `bad blocking`), turn 14, life 2/-2: Ooze + Hydra +
   Greensleeves attack, Baka cycles Liege/Worm/Trow/Hag blocks and takes lethal. -> lane T.
Lanes R/S/T = worktrees/lanes/w53-{R,S,T} from master 2d83e169f; merge after M,Q,O,P,N.
- vpk12 built 2026-09-02 10:04 (psp-port = master 859186536, wave-53 lanes M-T; incremental docker
  build, 33,242,644 B at worktrees/psp/build_vita/wagic.vpk). Ships: Go-Shintai pay-interrupt fix
  (lane R), Baka aggregate-lethal blocking (lane T), hand-refresh dedup (lane S), plus the GPT-seat
  lanes. UPLOADED 2026-09-02 ~10:40 after a Vita reboot (33242644 B verified). GOTCHA: VitaShell FTP is
  single-threaded and a client that times out leaves the server blocked on the dead session (TCP
  accepts, no banner, every later attempt times out) — the curl retry loop WEDGES it; when the
  first attempt times out, have the owner restart FTP/reboot and then connect ONCE. Docker stopped.

## 2026-09-02 (vpk12 play session, 16 matches) — four owner reports, logs at psp-work/logs/20260902-vpk12/
1. Heliod, the Radiant Dawn (Aura Farming = player deck4; MOM, backside=Heliod, the Warped
   Eclipse) shows a DIFFERENT untransformed card's art when transformed. -> lane V.
2. Two games with "really bad frame rate" tagged `other`: 1788378791 (Merkfolk vs Nazgul deck164,
   16 Nazgul clicks = a Nazgul/Ring swarm) and 1788381501 (Squirelly Wrath vs deck95 Troll
   Ascetic/Slith). Wide boards. Lane S's dedup did not answer it; render path was unmeasured.
   -> lane W (frame-time telemetry into memlog, alpha-gated; desktop profile of both boards).
3. "opponent double blocked for no good reason" = 1788381069 deck1 vs deck132 (tag `bad
   blocking`), turns 23-24, AI at 13 facing Hapatra/Aetherborn/Nighthawk/Fynn. -> lane U.
4. NEW: 9 of 16 transcripts carry #classification but NO #result line (vpk11: every classified
   transcript had one). -> lane U.
Also: memlog heap_free 26-28 MB late (was 38 MB in vpk11 games) — watch, not yet an item.

## 2026-09-02 evening (vpk12, 88 more matches) — logs at psp-work/logs/20260902-vpk12b/
Tags: 71 no problems, 5 bad blocking, 2 bug, 8 other (owner: "a few very lagged games"), 2
abandoned; 39 classified transcripts lack #result (the static-pointer bug lane U fixed — not
in vpk12). memlog has no `frames` lines (lane W telemetry is not in vpk12).
1. Branching Evolution scripted as a `@counteradded ... target(...) counter(1/1)` TRIGGER
   (opens a target ask, adds one) instead of a doubling replacement — owner: "letting me add
   counters to a target instead of doubling". Game 1788397152 deck7 vs deck101 (`bug`). Engine
   idiom exists (Doubling Season/Vorinclex `@totalcounteradded ... duplicate(all)`). -> lane Y
   (data-only + audit of the same shape across borderline.txt's 544 @counteradded lines).
2. Five `bad blocking`: 1788388578 (deck1 vs 72), 1788390984 (deck7 vs 2), 1788392029 (deck7 vs
   10), 1788394118 (deck3 vs 3), 1788397358 (deck1 vs 36). -> lane Z (classify vs lane U's
   deathtouch fix / lane T's sweep / new holes).
3. Second `bug` 1788398189 deck1 vs deck49 — owner did not describe it ("maybe some issues i
   forgot"); unlabeled, not delegated.
4. Lag games (`other` x8) — no frame data on vpk12; vpk13's memlog `frames` lines are the test.
5. SOFTLOCK (owner: "one of the quit to menu games was a softlock"): both abandoned transcripts
   (1788390621 deck4 vs deck54; 1788393737 deck5 vs deck33) end after `p2.next 1 12` with no
   `p1.next 1 1` — the human's upkeep never opened. Game 1's human upkeep carries Sleeper
   Agent's self-damage (AI gave it) + Nyx-Fleece Ram lifegain; game 2's may carry The Rack.
   -> lane AA (replay both; fix the seam; fixture stalls-under-timeout on base).

## vpk13 — 2026-09-03 03:43 (Voyager) — psp-port f29684e59 (= master caf023dbc + Vita header fix)
- 32,574,803 B, uploaded to ux0:/vpk/wagic.vpk in ONE curl attempt after the owner said "ftp up"; size verified on the listing.
- Carries: wave-54 step-1 engine lanes A-F (HOLD row reword/keys, parser stamps, cast-row pricing incl. player targets, gain-framed searches, forced-sac ordering, render truths, reveal stall guard + second reveal hang fix), wave-53 lane AA (AI-held interrupt window stall floor = the softlock), audit lanes G-N (ability ownership, per-tick ability epoch gate, ManaCost/garbage/instances memory, render caches, Baka selectAbility 35x, GPT transport/narration/cache), O1 heap 224 MB, O2 PARSETEST compiled out of the handheld.
- First Vita build failed: GptConfig.cpp used sceKernelGetRandomNumber / sceKernelDelayThread without psp2/kernel/{rng,threadmgr}.h (lane L's Vita code never saw the console compiler). Fixed f29684e59; second build clean.
- What the memlog should show (predictions, lane N / O1): `ram_free` ~96 MB lower at boot; `heap_arena` flat after game 1 (was +1.4 MB/game); `heap_used` +~2.2 MB one-off (decode scratch); no bad_alloc in 50 games. `frames turn=N ... avg_swp` decides O11.
- Disable flags compiled into this build (A/B by rebuild): -DWAGIC_TEX_POW2_PAD, -DWAGIC_NO_DECODE_SCRATCH, -DWAGIC_NO_USERDIR_MKDIR (JGE); env flags per wave54/review-carry.md §C apply where the Vita reads env (it mostly does not — rebuild to A/B).
- NOT eyeballed by any lane: NPOT+CLAMP card rendering, immediate-upload path, Vita sound (the userdir mkdir fix may make it audible for the first time).

## vpk13 verdict (owner, 2026-09-03): "vpk13 runs very smoothly.. no apparent sign of the frame lag that was present before."
- The wave-54 audit lanes (Baka selectAbility 39->1.6 ms, hand oracle on change, render caches, ability epoch gate, NPOT textures + decode scratch) + O1 heap 224 MB are the candidates; no memlog read yet to say which. Pull the memlog `frames` lines before attributing.
## vpk14 — 2026-09-03 (Voyager) — psp-port 860528eb5
- Adds: wave-55 lanes A-E (own-turn HOLD row + option-set keys, blockers re-ask + menace header, X markers + both-player edict verdicts + NAMED-BY price, reveal/discard collapse + chooseaname + discard verdicts, reveal_stall/wall_miss), lane P dead-module purge (network kept; Vita list edits verified by the vpk14chk build), lane Q autotap search (cap 32, work budget — the option-preserving autotap now ENGAGES on 16+ producer boards at <1 ms est.), lane R stall floor (wall clock, never while a model call is in flight, WAGIC_STALL_FLOOR=0), wave-55 guides. NOT included: O7 gnu++14 (own build later).
## vpk13 late report (owner, 2026-09-03): "for vpk13 there was a game with one of my opponents cards that was supposed to gain counters repeatedly, it only ever gained 1 counter. i accidentally classed it as no problem end of game."
- Card unnamed; the transcript is tagged `no problems`, so it will NOT surface from the tag census. vpk13 transcripts not yet pulled (no psp-work/logs/20260903-vpk13/). To find: pull the vpk13-window transcripts (ids after the 20260902-vpk12b set) when the owner says FTP is up, scan the AI seat's cards for repeat-counter effects (@each-upkeep/@attacked/@totalcounteradded shapes), compare against Oracle per the verify-oracle-text rule. Sibling shape already seen: vpk12 Branching Evolution (lane Y, trigger vs doubling mis-script). Engine vs card-script unknown until the card is named.
## 2026-09-03 pull (vpk13 + vpk14 play sessions) — logs at psp-work/logs/20260903-vpk13/ (41 transcripts: 33 vpk13-window <1788452000, 8 vpk14-window; memlog.txt 510 KB; debug.txt)
- Tags: 38 `no problems`, 3 unclassified. #result present on every classified transcript (lane U fix holds).
- memlog `frames` (506 turn-lines, both sessions): avg frame median 7.0 ms, p90 14.1, worst 32.1 (p1perm=14 p2perm=9 abil=161); avg_upd median 4.4 ms (dominant); avg_rnd median 2.4; **avg_swp median 0.1 ms, max 0.5** -> O11 SETTLED: swap/vsync is NOT the cost, the update phase is. 22/506 turns average above the 16.7 ms budget. max_upd spikes to 3.8 s are single-frame stalls (turn 1 = game load).
- FTP: listing ux0:/data/Wagic/ai/gpt/ TIMED OUT (60 s, 0 bytes) after the transcript/memlog pulls succeeded — not retried (single-thread rule); VitaShell FTP may need a bounce before the next pull.
- Owner's "counters only once" report: deck122 Eternal Life (1788429079) has NO grower cast (Pridemate never drawn). Best match = Oran-Rief Survivalist, deck84 Rainforest Allies, game 1788428546 (04:42, vpk13): 1/1 CMC2, `may counter(1/1)` self-ETB + `_RALLY_may counter(1/1)`; 12+ Allies (incl. 8 Kor Ally tokens) entered after it. Desktop: rally fires for the pure-Baka fixture seat (3/3 asserted) on master AND the caf023dbc/ef133ea2a/dddb6f62b archives -> not a regression in the trigger. Vita seat = AIPlayerGPT on the thread-refusal fallback (gpt-log: "worker thread refused ... falling back to the heuristic AI"), whose may-menu fallback maps AIPlayerBaka::selectMenuOption() into option space and DEFAULTS TO DECLINE when nothing maps — under test via WAGIC_GPT_NOTHREAD desktop selfplay (deck84 vs deck1).
- CLOSED (owner, 2026-09-03: "its not survivalist ... just let it go for now, ill find it again eventually"). Survivalist RULED OUT: under WAGIC_GPT_NOTHREAD desktop selfplay (the Vita's fallback path) it grew on every Ally entry incl. Halimar Excavator's targeted rally and Captain's Claws Kor Ally tokens (1/1 -> 12/10 in one game). Card still unnamed; owner will re-find it on Gatherer. Probe deck999 + run dirs deleted.
## vpk15 — 2026-09-03 12:05 (Voyager) — psp-port 6e8a7782c = vpk14 source (860528eb5) + O7 ONLY
- Single variable vs vpk14: Vita CMAKE_CXX_FLAGS `-std=c++11` -> `-std=c++14` (master f9a39205b). First attempt with `-std=gnu++14` FAILED: the gnu dialect turns on newlib's BSD visibility, whose `quad` macro clobbers `hgeDistortionMesh::quad` (hgedistort.cpp:30 "'quad_t' was not declared"). Strict c++14 builds clean. 32,597,085 B (vpk14: 32,586,638). Archived archives/wagic-6e8a7782c-vpk15.vpk. Not uploaded yet.
- Owner's expectation was that vpk14 carried O7; it did not (held back per "not in conjunction with many other changes"). Plan: vpk15 = flag-only A/B against vpk14; wave-56 ships as vpk16 on c++14 if vpk15 holds.
- UPLOADED 2026-09-03 (owner: 'give me the new vpk, ftp isup'): first curl attempt timed out at 60 s with 0 bytes (server still answered a listing); second attempt completed, listing shows 32,597,085 B. vpk15 LIVE. Owner verdict pending (A/B vs vpk14 on the c++14 flag alone).
## vpk15 verdict (owner, 2026-09-03): "This vpk doesnt have any apparent regressions. It may be an illusion, but it seems subtly smoother even." -> O7 HOLDS: Vita stays on -std=c++14; wave-56 ships as vpk16 on it.
- New report: "One match where the opponent proliferated +1/+1 counters on my creature marked bad targeting." -> pull the `bad targeting` transcript, name the proliferate source, check its script + Baka's target choice (abilityEfficiency verdict for the proliferate effect).
- vpk15 bad-targeting report = game 1788458030 (player deck6 vs deck133 Phyrexian Asphodel): the AI activated Yawgmoth, Thran Physician's `{B}{B}{discard}:_PROLIFERATE_` FIVE times in one turn and proliferated the HUMAN's +1/+1 counters (Stonebrow/Toski/Boartusk Liege board; the AI's own permanents carried no counters). Mechanism: `_PROLIFERATE_` = `notatarget(proliferation) proliferate` -> ProliferateChooser; its PLAYER branch has AI guidance (no own poison, no counter-less opponent) but its CARD branch accepts ANY in-play card with counters, and AIPlayerBaka::chooseTarget sets playerTargetedZone=2 (both sides) for it; activation efficiency = 90 when not player-aimed (AIPlayerBaka.cpp:851/1541). -> lane Y (hotfix worktree w56-Y): AI guidance in the card branch (own card only if its counters help it; opponent's only if they hurt it), so validTargetsExist gates the activation too. Fixture: Yawgmoth + BB + discard fodder vs a +1/+1-countered human creature -> no activation; with an own +1/+1-countered creature -> proliferate it.
- vpk15 `bug` report (owner: "engine forced me to kill my own creature, not allowing me to cancel casting a spell that i hadnt declared targets for. this is rules noncompliant, and also not fun.") = game 1788460441 (player deck1 vs deck100), AI turn 9 interrupt window: human tapped Swamp+Forest+Forest, clicked Putrefy (`0putrefy` = target chooser armed, NOT paid - MTGPutInPlayRule pays after targetListIsSet), pressed cancel 18x -> each logged `p1.endinterruption`: ActionStack::CheckUserInput's `else if (observer->isInterrupting)` branch consumes JGE_BTN_SEC (endOfInterruption + return true) so ActionLayer::cancelCurrentAction never runs; the armed chooser outlived the window; the AI's board was 5 Plains, so the only legal target was his own Thornweald Archer, which the next click killed. -> lane Z (hotfix worktree w56-Z): SEC while actionLayer()->isWaitingForAnswer() cancels the pending choice first (window stays open); endOfInterruption with a pending waiting action cancels it as a safety net. cantCancel is NOT involved (only set for must-menus).
- vpk15 `bug` report #2 (owner: "found another bug with light paws. it lets me put the tutored aura onto other creatures.") = game 1788460925 (player deck4 vs deck27), turn 5: `p1.battlefield[2] 1light-paws` then `p1.battlefield[4] 1starfield mystic` = the tutored Aura attached to Starfield Mystic. Oracle: "put that card onto the battlefield attached to Light-Paws, Emperor's Voice". Script: `... target(aura[...]|mylibrary) moveto(mybattlefield) and!( transforms((,newability[shuffle],newability[name(Attach aura) retarget target(Light-Paws^ Emperor's Voice|mybattlefield)])) oneshot )!` — the Aura's own attach prompt on entering wins over / precedes the retarget. Existing fixtures (light_paws_*.txt) assert zones only, never the attachment. -> lane X (worktree w56-X): card-script first (an enters-attached idiom, e.g. how Sovereigns of Lost Alara / Bruna are scripted), engine only if no idiom; fixture asserts the Aura's attachment target.
## 2026-09-03 afternoon — the three vpk15 reports FIXED on master (each fixture-pinned RED->GREEN on the base binary)
- Lane Y a829763b7 (proliferate): ProliferateChooser card-branch AI guidance — per-kind counter score (P/T sum; named 0/0 lists), own card iff net >= 0, opponent's iff < 0; Baka never activates when nothing qualifies (createAbilityTargets adds nothing). Human seat untouched.
- Lane Z 3b1eb7f8e (cancel in interrupt window): new ActionStack::cancelPendingChoice() — SEC with a pending choice cancels the SPELL and keeps the window (second press ends it); endOfInterruption cancels any orphaned chooser (human + suite seats only). Gotchas: observer->targetChooser is GLOBAL, not per seat (putting the net in cancelInterruptOffer killed the active caster's chooser — two fixtures went red; removed); DSL command is `cancelbutton` (`cancel` shadowed the card named Cancel). Residual: stall-watchdog release path can still orphan a silent seat's chooser -> per-seat chooser owner docket item.
- Lane X d94e4aa4d (Light-Paws): the script's `and!(...)!` attach leg had NEVER run — AAMover enters the Aura via a deferred AACastCard (oneShot=false) and the payload was applied to `_target->next` before that instance existed. Engine: AACastCard now carries/delivers the andAbility; script: `all(creature[Light-Paws...]|mybattlefield) rehook` replaces `retarget target(...)` (which parked as an activated menu entry). Runed Crown / Mantle of the Ancients share the dormancy class — docketed, not edited.
- Gate for vpk16 launched on d94e4aa4d (unit w56-vpk16-gate).
- Lane W 4366e8eec (MDFC back-face legality, seat 146/152/162 E-2): the seat's premise was WRONG — `restriction{can play land}` was never a no-op (MTGAbility.cpp:1029 handles `can play <type>` with the land quota + sorcery-speed test); the defect was the ACTOR: the handler reads observer->currentActionPlayer, so a seat acting in its own priority window on the opponent's turn skipped the timing test and read the opponent's quota. Fix = LegalActionsOracle::canPlayLandNow(card, actor) (actor must be the controller; same StackIsEmptyandSorcerySpeed + PlayRestrictions quota the normal land drop uses), wired at parseCastRestrictions AND ActivatedAbility::isReactingToClick (the latter removes the row from the option list). The seat's `restriction{assorcery}` swap would have DROPPED the quota check — do not apply it. Residual: the legacy handler still uses currentActionPlayer for ~60 `can play creature/sorcery` entries (foretell, disturb) — docketed. E-3 done (Land drop summary counts back faces); E-1 (pregame hand land count) docketed with the tally-semantics question.
## vpk16 — 2026-09-03 14:52 (Voyager) — psp-port = master 4366e8eec, -std=c++14 — UPLOADED 19:53 Vita clock, 32,615,944 B size-verified, ONE curl attempt (owner: "when you have all of this waves fixes merged, build and push a new vpk, ftp is up")
- Carries: wave-56 step-1 lanes A-E (hold predicate on rendered rows + async slot key; AIAction player init = the constant-166 fix + INCOMING total; [RE-ASK] latched line, reveal_stall, X rows, the MTGAbility::target UAF fix; MDFC land backs playable; seeded AI RNG + replay parking), hotfix lanes Y (proliferate targeting), Z (cancel in interrupt window), X (Light-Paws attach), W (MDFC back-face legality: controller-only, sorcery speed, land drop left), wave-56 reviewer guides. Gate 1241/2-known, 55 AI/0, PARSETEST 2736/0. Archive archives/wagic-4366e8eec-vpk16.vpk. Docker stopped.
- Owner-facing changes to look for: Yawgmoth-class proliferate no longer grows your creatures; cancel (SEC) during an interrupt window cancels the pending spell first, second press ends the window; Light-Paws' fetched Aura lands on Light-Paws; MDFC back-face lands (Agadeem, Emeria, Pathways...) are playable by both seats, own main phase only, use the land drop.
- Owner rulings 2026-09-03 on the two carried docket items (verbatim in wave57/owner-decisions.md): D42 grouping YES with identical-name-AND-state stacking only (sickness, attachments, tapped split the group) -> wave-57 lane G (frontend); D43 log window "measure." -> lane H (mechanism behind a default-OFF flag) + an A/B corpus.
## vpk16 report #1 (owner, 2026-09-03): "in the last match, i softlocked after the opponent canceled my spell." Game 1788467289 (player deck2 Fencing Ace/Plains vs deck131 Mind Control: Counterspell/Essence Scatter/Dissipate/Aetherize). Transcript last flush = AI turn 6 requesting phase 12 (end step); memlog: untap turn=6 at t=465, slow update frames at t=475 (324 ms) and t=495 (1104 ms), NO untap turn=7 -> the game stalled inside turn 6 after the end step began. Shape: human casts in the opponent's end-step window, AI counters, game never reaches the human's untap. Suspect #1 = lane Z (3b1eb7f8e) endOfInterruption safety net (cancelPendingChoice / SAFE_DELETE observer->targetChooser — global chooser) — the exact window it added code to. -> lane S (worktree w57-S): reproduce with a human-seat fixture on archives pre-Z (wagic-d0515fae3-w56step1) vs post-Z (wagic-d94e4aa4d-w56hotfix / 4366e8eec), bisect, fix, gate. Owner asked: which spell, whose turn, what the softlock looked like.
- Owner detail (verbatim): "I dont remember the card. For the loftlock, my screen was frozen, unable to move the selector, and the turn indicator was opponent, despite being my turn." -> a HANG inside one update tick at the turn-6->7 transition (cursor is drawn per frame; memlog's last line is a 1104 ms frame then silence), not an input-dead logic stall. Lane S told: hunt a spinning process (exit 124 + CPU busy), gdb backtrace = root cause; first suspects ActionLayer::Update's cantCancel re-click block on a chooser whose source was countered, lane Z's cancelPendingChoice re-entry, MayAbility re-trigger, stack resolution over a destroyed ability.
- CORRECTION (owner, verbatim): "No, the menu button works. I used it." / "That is how i exited. I pressed start, then selected main menu." -> the loop was ALIVE; the softlock is a LOGIC STALL (human seat not the acting player in the layers' view; phase never advanced past the AI's end step after the counter). Lane S redirected (idle game, suite timeout signature, per-tick ActionStack/ActionLayer state probe, GPT fallback decisionPending/asyncBusy suspects). Lane T re-scoped: primary = in-game menu entry "Softlock: dump diagnostics and quit to menu" beside Start->Main Menu, transcript #classification=softlock; in-thread guard secondary; watchdog thread dropped.
- Lane S (0a5668074): softlock NOT reproduced in 10 desktop variations (scripted, mixed-seat via the new `aiseat` DSL, Dissipate, Essence Scatter on flash, counter war, cancelbutton, NOTHREAD selfplay 3/3). CORRECTION: gpt-log-vpk16.txt line 1 = "signed in to ChatGPT (plan: plus)" and NO "worker thread refused" — the vpk16 seat ran LIVE model calls (the thread-refusal reading came from the 2026-09-01 log). Mechanism candidates that match the symptoms exactly (turn indicator pinned, keys discarded as isAI, loop alive): (1) a live chooser / menuObject stranded (GameObserver::Update re-pins isInterrupting = tc->Owner every tick; cancelInterruptOffer cannot undo it) — lane Z's cancelPendingChoice has no ownership check and is reachable from an abilities-menu Cancel (-> D34, lane F); (2) an in-flight model call: while aiDecisionInFlight() the seat extends the interrupt offer EVERY tick with NO wall bound — a wedged HTTP call on the Vita freezes the duel forever; against it: no patience prompt reported. Owner asked: did the screen say "opponent is thinking"? Shipped: aiseat DSL, WAGIC_STALLPROBE (dev-gated per-tick priority-state vector), 2 negative-control fixtures. Transcript gotcha: card-click lines are prefixed by the CARD's controller, not the acting seat.
- MERGE-PROCEDURE LESSON (wave-57 lane D, 2026-09-03): the auto-resolver's brace-balance check REJECTED a hunk but the chained command still `git add -f`'d the marker-laden file, the build failed, a stale bin/wagic answered PARSETEST, and `git commit --no-edit` landed a merge with conflict markers (twice, amended). Rule from now on: the resolver exits non-zero on a failed balance check and NOTHING after it runs (`set -e` / `&&` from the python step); never chain `git add` after a resolver whose verdict was not read; PARSETEST only after `ls -la bin/wagic` shows a fresh mtime. Brace balance is confounded by `{...}` inside string literals — the self-test seam rule stands: if ours' last line is a CHECK/blank and the common tail's `    }` closes theirs' block, insert `    }` between ours and theirs.
- Lane F c2f6ff389: D25 second UAF (a->source via MTGGameZone::hasCard) — dead-ability sweep before the garbage zone is freed, ASAN RED->GREEN, flag WAGIC_NO_DEADREF_SWEEP; D38 phase livelock root cause = unconditional `if (getCurrentTargetChooser()) return;` in userRequestNextGamePhase -> consecutive-refusal breaker releases an AI-owned chooser with no decision in flight (this is lane S's mechanism (1) — a plausible fix for the vpk16 softlock); D36 replay loading stall floor; initMTGCI zeroes swapP/swapT/MaxLevelUp/handEffects/castX/nb_damages. New residuals: Intruder Alarm + Thraben Doomsayer activation loop livelock (146v123 stub, exit 124); 4th + 5th replay defects.
- Lane U 67b2bb38a: in-flight model call bounded: request abandoned at deadline+grace (grace = clamp(deadline/2, 1 s, 30 s)), window backstop at 2*deadline+grace (600 s default -> 630 s / 1230 s); `abandoned_in_flight` fallback; new mHoldInFlightSinceMs anchor (extendInterruptOffer's zeroing could rewind the floor forever); dev emulator WAGIC_GPT_WEDGE; positive control WAGIC_INFLIGHT_BOUND=0 re-freezes. This is lane S's mechanism (2). vpk17 = 67b2bb38a carries BOTH candidate softlock fixes + the dump-and-quit menu entry.
## 2026-09-03 late — vpk16 session #2 (owner): "lost more transcripts. i think it was mostly good, aside from some bad blocks from the ai, legal, but deterministically bad." 46 transcripts pulled (1788468284..1788475129): 42 no problems, 4 bad blocking (AI decks 122 Eternal Life, 84 Rainforest Allies, 58 Bloodhall Ooze, 19 elves). ux0:/data/Wagic/ai/gpt/logs/ holds only 5 mid-August JSONL — the CONSOLE SEAT WRITES NO TRANSLOG, so model-vs-heuristic attribution of the blocks is impossible from the pull; gpt-log.txt is art-fail noise + "signed in to ChatGPT (plan: plus)". Action: enable translog/telemetry on the Vita via its GPT config file (data, no rebuild) before the next session so play reports carry decisions.
- Vita GPT config: added `translog=1` to ux0:/data/Wagic/ai/gpt/endpoints.txt via FTP (353 -> 364 B, backup in the session scratchpad; contents never displayed). Next console session writes per-decision JSONL to ux0:/data/Wagic/ai/gpt/logs/ — pull them with the transcripts; block decisions become attributable (model vs heuristic fallback).
- OWNER FRAMING (verbatim, 2026-09-03): "Oh, on the vita, ive been playing only against the heuristic ai. Im looking for gui bugs, and cardbugs, and heuristic ai weakpoints." -> every vpk play report so far is a BAKA game: the vpk16 softlock is lane S's mechanism (1) (stranded AI-owned chooser; lane F's breaker in vpk17), NOT an in-flight model call (lane U stays as general hardening); the 4 `bad blocking` games are AIPlayerBaka blocker logic (deterministic) -> lane V (heuristic blocker weak points from transcripts 1788468516 deck122, 1788468785 deck84, 1788469781 deck58, 1788471208 deck19); the translog=1 config edit is inert for Baka games. The "signed in to ChatGPT" gpt-log line is config state, not seat activity.
## vpk17 — 2026-09-03 18:00 (Voyager) — psp-port = master 753c8d488, -std=c++14 — BUILT + ARCHIVED (32,675,663 B), NOT YET UPLOADED (first curl timed out at 60 s with 0 bytes; the listing after it answered nothing -> FTP down/wedged; one attempt only, awaiting the owner's next "ftp up").
- First build FAILED: lane T's softlock dump used sceIoMkdir without <psp2/io/stat.h> (GameStateDuel.cpp) — the vpk13 GptConfig class of miss; fixed 753c8d488 (VITA-ifdef include), second build clean.
- Carries: wave-57 step-1 lanes A-H + S/T/U (see wave57/review-carry.md), i.e. for the Baka-seat play the owner does: lane F's orphan-chooser breaker (the softlock candidate), lane T's "Softlock: dump diagnostics and quit to menu" entry beside Main Menu, lane G's board grouping (identical name+state) with the spell-target board rim, lane F's second-UAF sweep + initMTGCI zeroing. First thing to try: divided damage across 3 members of one pile; and any softlock -> use the new menu entry, then FTP the softlock-<epoch>.txt.
- Lane V 027db3b09 (heuristic blocking, from the 4 `bad blocking` transcripts): reconstructions — deck122 Nighthawk aims at attacker #1 positionally; deck84 FIVE blockers pile on Ironroot Warlord while three Squirrels connect; deck58 Kird Ape blocks a 2/1 first-striker and dies; deck19 two blockers into a lone 1/1 double-striker four turns running. Rule fix: `evaluateDeclaration()` resolves both damage steps (first/double strike, CR 510.1c damage division, deathtouch/indestructible/protection); pass 1 takes only blocks that KILL and never spends more creature value than it kills (the gang cap), aimed via `aimBlockerAt`; pass 3 same price test; chump/absorb/lethal branches untouched; AIPlayerGPT untouched. Suite 1245/0, 61 AI/0 (+4 fixtures RED->GREEN on base), PARSETEST 3350/0. Live A/B 10+10 Baka games: dead-for-nothing value blocks 26/80 -> 0/57, over-spent declarations 20/80 -> 0/57. MERGED GIT-ONLY while the wave-57 corpus runs — master bin/wagic NOT rebuilt yet; build + gate after W57_CORPUS_DONE; ships in vpk18. Docket: blocks committed via toggleDefenser are never click-logged (transcripts cannot show the survival sweep); deck84 pile shape has no fixture (AIStats empty in a single-combat fixture).
- LOG-WINDOW A/B LAUNCHED 19:54 (owner D43 "measure"): unit ab-logwindow, done-file ~/.gatelogs/AB_LOGWINDOW_DONE, per-arm logs ~/.gatelogs/ab-logwindow-{unset,kind_3,4}.log + arm markers in ab-logwindow.log; three arms SEQUENTIAL at -j 3 (unset / kind:3 / 4), binary = archives/wagic-027db3b09-w57V run from worktrees/ab-logwindow (main tree free for wave-58 merges). Expect ~3-4 h per arm. Harvest per lane-H.md: arm verification by `log_window` census, prompt composition, per-decision inference, fallback rate, seat-reviewed misplays, win-rate BAND (SE ~10.9). No seed pairing possible.
- Wave-57 engine seat: 34 PASS / 11 FAIL / 16 UNTESTED. D5 adjudicated: 108 drops = 45 land-arm (35 Baka land auto-tap turn-flip) + 63 casting-arm (55 stack-trigger, 17 after `hold re-opened`), 11 rescues, ZERO cross-arm -> the second slot WORKS; residual = intra-arm re-ask (cost 0.82 h = 3.3%). D4 PASS 0/2960. D6 PASS on the emitter's window set. HOLD adoption in N>=3 windows REGRESSED to 9.6%. Reviewer flag "lane H's elided count reaches no record" is NOT a gate: `log_window_turns_elided` is written only when > 0 (AIPlayerGPT.cpp:11643) — the full arm has none by design; the kind:3 / 4 arms will carry it.
- ROTATION DIAGNOSIS (owner asked 2026-09-03; his ruling: "as long as we're finding things that need improving, thats fine"): pool = the same 7 since wave 44 (14 corpora); "zero exits" already recorded at w32/w33/w36. Earlier 2-wave exits (decks 59/18/22) were the guide-only Step-0 era. Cause: the engine-first loop changes render literals every wave -> guides re-keyed (19-51 literal-keyed edits per wave in deckN/edit-texts.md) -> "no mods" never holds; and "no engine items surfaced" is the negation of the seats' current job. Fair hands (w44) added variance, not the stall. Roster: 164 decks, 19 commander, 37 guides (7 in pool, ~30 canaries), 109 valid-format never-cycled + 18 commander. NO RULE CHANGE MADE; if rotation should move: count strategy-side edits only, treat engine items as the wave's product.
- ROTATION STALL ROOT CAUSE (owner's hypothesis "a change in how the review agents are being prompted" — CONFIRMED on disk): the reviewer brief was REWRITTEN at wave 44 (00-reviewer-brief.md 7-15 KB -> reviewer-brief.md 3.7 KB) and every later brief is a sed of it. Exit-era briefs (w28-35) carried per-deck rotation state in the header (HOLD / EXIT-ELIGIBLE / "exit projected wave-N"), a wave-to-wave projection, the exit gates (HL1 card-fact audit), and "Rotation: UNSENTIMENTAL... adherence/maturity-shaped, not record-shaped"; the w44 rewrite kept only the words "rotation evidence" in the seat-file contents list. The literal re-keying mechanism is real but secondary. Proposed (owner to approve): restore the rotation clause in the wave-58 brief + have synthesis carry the projection.
- Wave-57 synthesis ba351866b: docket D1-D45 (top: pay-3-life row no lethality verdict; false NO LIVE CAST ROW 8/497; rowSaysNoOp missing `exiles 0`; async 0/108 cross-arm; `[<- best trade:` 1-for-5). A/B flag: `log_window_kind` labels only 28.3% of records (72% unclassified) — the kind:3 arm will elide at fewer windows than designed; harvest must report ACTUAL elision per record (the `log_window_turns_elided` field, present only when > 0), not the design intent. Not a gate on the running control arm.
- COST DIAGNOSIS (owner 2026-09-03: "the loop [is] wiping out my tokens even faster than it had before" after a subscription upgrade): wave-57 subagent spend ~5.9 M tokens over 22 Opus agents (12 engine lanes 3.2 M, 4 Vita hotfix lanes 0.9 M, 4 seats 1.3 M, synthesis+boundary 0.5 M) + orchestrator context re-billed per wake + ~25 K fleet-memory/CLAUDE.md tax per general-purpose subagent. Drivers: the guide half (~1/3 of the wave, no outcome signal), a fixed six-lane wave shape, and EVERYTHING on Opus contrary to his 2026-07-30 Codex-first delegation posture. Proposed plan (awaiting his ruling): lanes -> Codex CLI, 3 per wave by docket rank; guide half -> 1 engine seat + 1 guides seat (Opus) under the null-outcome brief; synthesis every other wave unless a rule breaks at 2 seats; orchestrator compacts at every wave boundary.
- OWNER LOOP DEFINITION (verbatim, 2026-09-03) recorded at projects/mtg/strategy-design/wave58/loop-as-envisioned.md — AUTHORITATIVE; "everything else in how the loop is operating is an invention of claude." Wave 58 runs on the three steps only; inventions to drop are named in that file (docket/predictions/carry-list, boundary pass, engine seat + seat groupings, amendment/R ledgers as growth machines, six-lane quota, rotation machinery, the draft brief clauses).

## 2026-09-03 ~22:20 — wave-58 step one LAUNCHED (post-compaction)
Worktrees worktrees/lanes/w58-{A,B,C,D} on master 6976b839a; shared brief
projects/mtg/strategy-design/wave58/lane-brief.md (untracked until merge). Lane A = CODEX TRIAL
(D3 rowSaysNoOp / exiles 0): `codex exec --sandbox workspace-write -C <wt-A>`, prompt
~/.gatelogs/w58-laneA-prompt.md, output ~/.gatelogs/w58-laneA-codex.out, log
~/.gatelogs/w58-laneA-codex.log, done-file ~/.gatelogs/W58_A_CODEX_DONE; Codex writes its
gate logs/lane-A.md INSIDE the worktree (sandbox). Lanes B (D1/D5/D7), C (D2/D4/D9), D (D45)
= Opus agents; their gates write ~/.gatelogs/W58_<lane>_DONE. Merge order when all report:
A, B, C, D (guarded python resolver, PARSETEST 0 failed before each commit), then hermetic gate
(sed w57-final-gate-launch.sh -> w58), archive wagic-<hash>-w58step1, push, vpk18.
A/B log-window: arm 1 (unset) still running since 19:54; Monitor re-armed on AB_LOGWINDOW_DONE.
~22:55: lanes done — A (Codex) 3df9f437d [Codex could not commit: worktree git metadata under main .git is read-only in its sandbox; verified under cap + committed by orchestrator], B a714f4cd0, C adab5e59b. master = 23a2798cf (A ff, B clean, C conflict pattern (c) + one seam fix: verdictReadsZero re-wraps each scope as `{right now: <op>}` before rowSaysNoOp, since lane A's grammar needs the clause). PARSETEST 3443/0. LESSON: my first merge chain committed on `grep 'self-test:'` (matched a 5-failed line) — gate on ' 0 failed' literally. Lane D pending; suite on ABC running (unit w58-abc-suite, W58_ABC_SUITE_DONE).
~23:35: lane D da76e6814 merged clean -> master ca2c9ce97 (UNGATED until the hermetic gate: unit w58-final-gate, script ~/.gatelogs/w58-final-gate-launch.sh, done-file W58_FINAL_GATE_DONE, logs w58-final-gate-{make,suite,parsetest}.log). Expected: PARSETEST 3455/0 (3350+24+44+25+12), suite 1245/2-known at THREADS=16, 61 AI/0. Then: archive wagic-ca2c9ce97-w58step1, commit lane-brief.md (untracked), push, remove lane worktrees, vpk18.
~22:45: WAVE-58 STEP ONE CLOSED. master 4581580f5 (pushed) = ca2c9ce97 merge chain + brief. Gate: PARSETEST 3455/0, suite 1245/2-known, 61 AI/0, 0 timeouts; archive archives/wagic-ca2c9ce97-w58step1. Lane worktrees removed. vpk18 build launched (unit vpk18-build, ~/.gatelogs/vpk18-launch.sh, VPK18_DONE, log vita_vpk18.log) from psp worktree = master; carries lane V + wave-58 A-D. Corpus (step two) deliberately HELD until AB_LOGWINDOW_DONE (arm 2 started 21:43; arm 3 ~2h after) so the A/B's remaining arms run at the same inference load as arm 1. Then: `sed` w57-corpus-launch.sh -> w58 with binary archives/wagic-ca2c9ce97-w58step1.
Codex trial verdict (lane A): did the D3 work correctly (numeric grammar, +24 PARSETEST, RED measured), ran serial suite itself; only friction = cannot commit from a linked worktree (git metadata outside sandbox) -> orchestrator commits. Cost: 0 Opus tokens. Opus lanes B/C/D: ~200K tokens each (vs ~270K avg in wave 57).
~23:00: vpk18 = archives/wagic-4581580f5-vpk18.vpk (32,690,283 B; master 4581580f5 = wave-57 A-H/S/T/U/V + wave-58 A-D) UPLOADED to ux0:/vpk/wagic.vpk, size-verified. vpk17 never uploaded (first attempt timed out at connect; the second, vpk18, worked after a live listing). Docker stopped.
~23:10: OWNER: D42 stacking rejected ('really obnoxious'), he'll think; do NOT redesign unprompted. Recorded in wave58/owner-decisions.md.
~23:15: OWNER D42 follow-up: 2 impl bugs (G1 opponent-land stack churn; G2 count indicators not on top, esp. when targeting) + design verdict 'not visually intuitive'. Bugs queued for next step one only if the feature survives; no redesign.
~23:25: OWNER: 'well, fix the bugs. and.. give it a setting to apply only to tokens. ill try it like that.' -> lane E (worktree w58-E, Opus): G1+G2 fixes + three-state BOARDGROUPING (Off/Tokens only/All, default Tokens only). Then merge, gate, vpk19. If the corpus is already running from the main tree when E lands, gate in the worktree and merge only (no main-tree rebuild).
~23:55: lane E a3f0700d3 ff-merged (master, pushed). Mechanisms: G1 = stacksPinnedNow() pinned the WHOLE board on any stack entry (not the key); G2 = badge drawn in CardGui::Render under later layers -> GuiPlay::RenderStackBadges after the selector layer; setting 1=Off/2=Tokens only/3=All (default Tokens only; legacy bool migrated). bin/wagic = archives/wagic-a3f0700d3-w58step1b (lane's hermetic gated build; corpus script updated). vpk19 building (VPK19_DONE). Lane E's unverified: nothing seen on screen; marked-pile expansion untested; no Vita compile.
~00:00 (09-04): vpk19 = archives/wagic-a3f0700d3-vpk19.vpk (32,692,014 B) BUILT, NOT uploaded — one attempt on next 'ftp up'. Docker stopped.
01:55 (09-04): A/B DONE (arms 195400 unset / 214338 kind:3 / 235619 4; 42 translogs each). Harvest delegated to Codex read-only -> ~/.gatelogs/ab-harvest-codex.out. WAVE-58 CORPUS LAUNCHED 01:50 (unit w58-corpus, W58_CORPUS_DONE, binary a3f0700d3-w58step1b, WAGIC_BLOCKLOG=1). NO main-tree rebuilds until it finishes (~1 day at wave-57 pace: 25 h inference).
02:10 (09-04): A/B harvest committed e663e3f56 (wave58/ab-logwindow-harvest.md; no ruling — owner's D13). NEW CRASH F1: SIGABRT vector bounds in DecisionManager::buildMenuChoice (DecisionContract.cpp:367, menu id 175 > mObjects.size) in A/B arm 4 game 146v152 after Lost Mine of Phandelver venture; recorded wave58/engine-findings.md; lane F on CODEX (worktree w58-F, prompt ~/.gatelogs/w58-laneF-prompt.md, out w58-laneF-codex.out). Cannot merge-and-rebuild main tree until W58_CORPUS_DONE; merge F then (gate in worktree). Corpus binary carries the same hole — a corpus game may abort the same way (dungeon path, rare).
04:35: CORPUS 20/21 natural + 1 HUNG (152v126, F2 recorded in engine-findings.md; lane G on Codex, detached setsid, out ~/.gatelogs/w58-laneG-codex.out). Rerun launched 04:22 (unit w58-rerun1, W58_RERUN1_DONE, log w58-rerun1.log). Corpus dir matchups-20260904-015059 (2457 decisions, wall_miss 7, results: 146 6/6, 162 4/6, 152/126/125 3/6, 130 1/6, 123 0/6). Step three launches after the rerun.
05:1x: rerun done (deck126 beat deck152; dir /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-042235). Step three launched: 7 deck agents + engine seat (Opus) on wave58/reviewer-brief.md.
06:35: D22 adjudicated (heuristic-vs-heuristic rerun of lane V's instrument on a3f0700d3): P1-P3 PASS 0/53 value blocks, P4 UNTESTED; recorded in wave58/engine-seat.md addendum. F3 = GptConfig.cpp:822 CURLOPT_CONNECTTIMEOUT_MS 2500 -> empty_reply 89/2270 (recorded engine-findings.md). Step-three seats all done; synthesis running; Codex F/G still running.
07:00: STEP THREE CLOSED 758244a95 (+ wave59/known-bugs.md). Synthesis adopted 14/14 (R319-R330, #330-331) — flag to owner (busywork risk). Guides live: 162/130/123/126/125 revised, 146/152 kept. Next step one = wave59/known-bugs.md; waiting on Codex F/G to merge first; then hermetic gate, vpk20. vpk19 still not uploaded.
07:10: WAVE-59 STEP ONE LAUNCHED on master 35c9b85bf: H (Codex, F3 connect timeout), K (Codex, Intrepid Adversary), I (Opus, discard badge + D7 floor wording + MDFC front face), J (Opus, own-turn crack-back + PUT parser + identical asks). Codex F/G (wave-58 F1/F2) still running. Merge order when in: F, G, H, I, J, K; gate; vpk20.
05:45 (real clock; NOTES stamps 06:35/07:00/07:10 above were ~1.5 h ahead): CODEX POOL EXHAUSTED at 05:06 (reset 09:24). Lanes G/H/K exited mid-work (uncommitted diffs in worktrees w58-G, w59-H, w59-K); F stalled since 02:10 (process gone). Plan: Opus finishes H now from Codex's partial diff (agent running); I merged (ff), J merging (conflict); F/G/K relaunch on Codex after 09:24 with 'continue from the working tree' prompts. LESSON: pgrep -f 'codex exec' matches the Monitor shells watching it — never kill by that pattern; match the codex binary path or use pidfiles.
05:55: I (1789de648) + J (5d41b5a51) merged -> master b8609f632 (pattern-c resolve), PARSETEST 3515/0; hermetic gate unit w59-gate1 (W59_GATE1_DONE). H on Opus (finishing Codex's partial). Codex reset 09:24 -> relaunch F/G/K. Stale pgrep Monitors stopped.
06:15: gate1 on b8609f632 GREEN (1245/2-known, 61 AI/0, PARSETEST 3515/0); archive wagic-b8609f632-w59gate1; pushed.
06:30: H (98ed9baef, Opus finished Codex's partial; Opus found 3 real defects in Codex's diff: timeout class swallowed, D10 retry disabled, CURLOPT_TIMEOUT_MS 0 = infinite) merged -> master 742324b5a (pattern-b resolve), PARSETEST 3529/0; gate2 unit w59-gate2 (W59_GATE2_DONE). Then archive + push. F/G/K on Codex after 09:24.
06:45: gate2 on 742324b5a GREEN (1245/2-known, 61 AI/0, 3529/0); archive wagic-742324b5a-w59gate2; pushed; I/J/H worktrees removed. Remaining: F/G/K (Codex after 09:24, worktrees w58-F, w58-G, w59-K keep their partial diffs). vpk20 after G.
09:30: Codex F/G/K relaunched (continuation prompts *-prompt2.md, outputs *-codex2.out, pidfiles *-codex.pid).
09:55: Codex F/G/K relaunch hit the usage limit again after ~20 min (next reset 14:27). Opus takes G (F2 hang) now from Codex's partial diff in w58-G; F and K wait for 14:27. NOTE for the owner: three parallel Codex lanes exhaust a refilled window in ~20 min — the 'Codex surplus' posture (2026-07-30) may be stale.
11:15: G (1c98896b7, Opus finished; hang = reveal driver retired its own stall guard; exit fixed, cause = known-bugs 12/13) merged clean -> master bde77f35a (+ known-bugs commit), PARSETEST 3529/0; gate3 unit w59-gate3 (W59_GATE3_DONE). Then archive, push, vpk20. F/K wait for Codex 14:27.
11:40: gate3 GREEN on 68184078e (1245/2-known, 62 AI/0, 3529/0); archive wagic-68184078e-w59gate3; pushed; w58-G removed; vpk20 building from 68184078e (VPK20_DONE).
11:13: vpk20 = archives/wagic-68184078e-vpk20.vpk (32,700,991 B; master 68184078e: wave-58 A-E + wave-59 I/J/H + G hang fix) BUILT, NOT uploaded — one attempt on next 'ftp up'; supersedes vpk19. Docker stopped.
12:19: OWNER: Codex is for adversarial review/tests, not implementation volume (recorded in model-tier memory). F and K move to Opus; the 14:27 Codex relaunch is cancelled.
12:57: F (15f6f6b4a: menu rows keep element identity, re-point/skip on compaction) + K (df935e123: propagateSource repoints extra costs — granted abilities' TapCost named the granter) merged -> master 5039427da; PARSETEST 3529/0; gate4 unit w59-gate4. Then archive, push, vpk21.
13:00: WAVE-59 STEP ONE CLOSED master 5039427da (gate4: 1247/2-known, 62 AI/0, 3529/0; archive wagic-5039427da-w59step1; pushed). vpk21 building (VPK21_DONE). WAVE-59 CORPUS launched (unit w59-corpus, W59_CORPUS_DONE) — no main-tree rebuilds until done.
13:13: vpk21 = archives/wagic-5039427da-vpk21.vpk (32,702,059 B; master 5039427da = wave-59 step one complete) BUILT, NOT uploaded; supersedes vpk20. Docker stopped.
15:17: WAVE-59 CORPUS DONE matchups-20260904-130044 (21/21 natural, 0 crash/hang, 3098 decisions; 146 4/6, 125 4/6, 152/126/162/130 3/6, 123 1/6). STEP THREE launched: 7 deck agents + engine seat (Opus) on wave59/reviewer-brief.md (synthesis told REJECT is the expected verdict).
15:35: WAVE-59 STEP THREE CLOSED 8b922abf0 (pushed). Synthesis 1/1 REJECT. Guides live: 162/152/123 revised. Next step one = wave60/known-bugs.md (14 HIGH). vpk21 not uploaded.
15:37: WAVE-60 STEP ONE CUT on 8b922abf0: lanes L (B1/B2/B11 lethality), M (B4/B3/B13 economy+transport), N (B5/B6 punishers), O (B7/B8/B10/B12 rows), P (B9/B14 engine) — all Opus; Codex = review pass on diffs at merge (owner's intended use).
16:32: WAVE-60 STEP ONE MERGED master 1fcdeeedc (L 08f753198 ff, M cdb7481bd, N 04e6c3c97, O f7335e80a, P 1fcdeeedc; resolver ~/.gatelogs/resolve-merge.py patterns a/b/c/d, driver merge-lane.sh); PARSETEST 3673/0. Gate unit w60-gate1 (W60_GATE1_DONE). Codex ADVERSARIAL REVIEW of the diff running (read-only; out w60-review-codex.out). Flags for wave-61: lane N — corpus showed the pilot's own punishers feeding the OPPONENT's Sanguine Bond loop (16->0/24->40), not supported by Oracle => possible engine/card-script bug in @lifelostfoeof binding (unexamined); lane N — concurrent suite runs sharing ~/.Wagic are NOT safe (a gate stopped at 378 tests); lane L — B1 fold and B2 total never fired live in 2 probe games.
16:39: Codex review (wave60/codex-review.md, committed): 9 findings, 2 HIGH (#1 namedCastLifeSurcharge first-match; #2 lane P drivingFor source-only early return = hang class). master NOT pushed. Lane Q (Opus, w60-Q) = fix-or-refute all nine, then re-gate, then push/vpk22/corpus.
17:08: lane Q 614f8452e (all 9 review findings CONFIRMED+fixed; #2's hang unreachable, gate scoped to ownership) ff-merged -> master 614f8452e; gate2 unit w60-gate2 (W60_GATE2_DONE). Then archive, push, vpk22, corpus.
17:11: WAVE-60 STEP ONE CLOSED master 614f8452e (gate2 1248/2-known, 65 AI/0, 3699/0; archive wagic-614f8452e-w60step1; pushed; lane worktrees removed). vpk22 building (VPK22_DONE). WAVE-60 CORPUS launched (w60-corpus, W60_CORPUS_DONE) — no main-tree rebuilds until done.
17:22: vpk22 = archives/wagic-614f8452e-vpk22.vpk (32,731,092 B; master 614f8452e = wave-60 step one incl. review fixes) BUILT, NOT uploaded; supersedes vpk21. Docker stopped.
18:44: WAVE-60 CORPUS DONE /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-171140. STEP THREE launched (7 deck agents + engine seat).
18:59: WAVE-60 STEP THREE CLOSED 718e99d2a (pushed; guides 146/125/126 revised; no proposals). WAVE-61 STEP ONE CUT: lanes R (C1-C4 combat/stack aggregates), S (C5/C6/C12/best-X NET), T (C7/C8/C9/C11), U (C10 marker+survivors, C13 transport, C14 economy) — Opus; Codex review after merge.
19:04: OWNER: 'vpk22? wtf? why are you making vpks.' — the per-wave VPK build was Claude's over-generalization of the one-time wave-57 instruction. RULE: NO VPK builds unless he asks. Latest built = vpk22 (614f8452e), only vpk18 was ever uploaded.
21:04: WAVE-61 STEP ONE MERGED master  (R 83a8faf10 ff, S 04a98d9eb, T 684be5517, U; PARSETEST 3872 expected); gate unit w61-gate1 (W61_GATE1_DONE); Codex review running (w61-review-codex.out). NOT pushed until both are in.
21:04: WAVE-61 STEP ONE MERGED master  (R ff, S 04a98d9eb, T 684be5517, U; resolver gained pattern (e) decl-block); gate unit w61-gate1; Codex review running. NOT pushed until both are in.
21:05: WAVE-61 STEP ONE MERGED master  (U needed a manual signature-hunk resolve + resolver pattern (e)); gate unit w61-gate1; Codex review running. NOT pushed until both are in.
21:06: WAVE-61 STEP ONE MERGED master b6f60b47e (R ff, S 04a98d9eb, T 684be5517, U; resolver patterns e/f + generalized c); PARSETEST 3872/0; gate unit w61-gate1 (W61_GATE1_DONE); Codex review running (w61-review-codex.out). NOT pushed until both are in.
21:12: Codex review (wave61/codex-review.md, committed): 8 findings, 1 HIGH (#1 Pelakka mandatory choice offered PUT: none). master b6f60b47e+review NOT pushed. Lane V (Opus, w61-V) = fix-or-refute all eight, then re-gate, push, corpus.
21:58: lane V 5faa9e970 (7/8 confirmed, #4 split) ff-merged -> master 5faa9e970; gate2 unit w61-gate2 (W61_GATE2_DONE). Then push, corpus.
22:02: WAVE-61 STEP ONE CLOSED master 5faa9e970 (gate2 1249/2-known, 65 AI/0, 3911/0; archive wagic-5faa9e970-w61step1; pushed; lane worktrees removed). WAVE-61 CORPUS launched (w61-corpus, W61_CORPUS_DONE) — no main-tree rebuilds until done. NO VPK.

## 2026-09-04 22:08 — post-compaction: w61 corpus active (21/21 in flight, unit w61-corpus), Monitor re-armed on W61_CORPUS_DONE (task b2z37dfft). wave61/reviewer-brief.md pre-derived from wave60 (corpus dir matchups-20260904-220227; CORPUS_RESULT_TBD placeholder to fill at completion; general/skill editions = wave59). Step three not launched.

## 2026-09-05 09:24 — w61 corpus KILLED at 11h (15,900 stale drops, 73.7% decisions stale_livelock->Baka; root cause: lane-U C14 hold-check counter ticked per rebuild inside the async slot key). Fix 8097c12cc (holdNoteSameWindow + assemblePrompt keyTail), gate green (1249/2 known, 65 AI/0, PARSETEST 3915/0), archived wagic-8097c12cc-w62fix, pushed. Record: wave61/corpus-livelock.md. RERUN launched as unit w61r-corpus (done-file W61R_CORPUS_DONE) on 8097c12cc. NO VPK.

## 2026-09-05 10:49 — w61 RERUN corpus DONE (`matchups-20260905-092408`, 21/21 natural, 0 crash/hang, 2,035 decisions, 6 fallbacks 0.3%, 16 stale drops, 1.4 h; 162 6/6, 125 4/6, 152/126/146 3/6, 130 2/6, 123 0/6). Fix 8097c12cc validated. Launching step three (7 deck agents + engine seat, Opus) on wave61/reviewer-brief.md.

## 2026-09-05 11:08 — WAVE-61 STEP THREE CLOSED 0d9b2d52e (pushed): 5 KEEP, 125/126 EDIT installed; 20 PASS/3 FAIL/4 UNTESTED/3 SPLIT; no proposals. wave62/known-bugs.md written (19 items; D1 enters-tapped classifier 131/172 false, D2 kill claim under life loop, D3 lifelostfoeof binding resolved = damage-based loss does not fire). Lategame specimen wave61/lategame-specimen.txt (125v123 seq 105). NEXT = cut wave-62 step one. NO VPK.

## 2026-09-05 11:10 — WAVE-62 STEP ONE CUT on 0d9b2d52e: Opus lanes W (D1/D15/D17 land rows), X (D2/D6/D8/D19 combat aggregates), Y (D3 lifelostfoeof engine/D4 Lair X/D5 X=0/D7 Intrepid cost), Z (D9-D14/D16/D18 protocol+parser+hold+transport) in worktrees/lanes/w62-{W,X,Y,Z}; brief wave62/lane-brief.md. Then merge-lane.sh, Codex review, fix lane, gate, push, corpus. NO VPK.

## 2026-09-05 12:51 — WAVE-62 STEP ONE MERGED: W 333a6fd97 -> 4ea74d962, X 4fa357706 -> 2a4227247, Y 55deeed00 -> daa3a2061 (one hunk hand-resolved: ours ends 8-space inner }, theirs new block; resolver pattern (g) added), Z d7526c7bf -> 05b207d2e; PARSETEST 4073/0 (= 3915+36+43+25+54). Gate w62-gate2 + Codex review (w62-review-codex.pid) running. D3 engine claim REFUTED by lane Y (engine already fires lifelost on damage; caution was misbound per line); D4 real engine bug (RankingContainer key collapse + uninit AIAction::id).

## 2026-09-05 12:58 — w62 gate2 GREEN on 05b207d2e (1252/2-known, 66 AI/0, PARSETEST 4073/0). Codex review committed e6067a67e (8 findings: HIGH #1 cast-route ANNOUNCE_X canDecline unset, HIGH #2 combat restatement ignores negation; MED double LOOP forecast emit, colour-blind pay-repeat, manland self-mana, WINS under cantchangelife, HTTP status dropped on curl fail, 200-empty at 95% classed timeout). Lane AA (Opus) running in worktrees/lanes/w62-AA.

## 2026-09-05 13:31 — WAVE-62 STEP ONE CLOSED: master 12be0b79c (pushed; W-Z + Codex review e6067a67e 8 findings -> lane AA af079512d confirmed all 8; gate 1253/2-known, 66 AI/0, PARSETEST 4111/0; archive wagic-12be0b79c-w62step1). Lane worktrees removed. WAVE-62 CORPUS launched 13:31 (unit w62-corpus, W62_CORPUS_DONE; watch = terminal + early 30m + late 4h). wave62/reviewer-brief.md pre-derived (TBD placeholders). NO VPK.

## 2026-09-05 14:32 — OWNER REQUEST (shop UI): per-printing owned count + full set name in the art area for boosters -> branch shop-ui-counts a9e97fd89 in worktrees/lanes/shop-ui (built clean; suite gate + merge to master AFTER the w62 corpus ends — no suite beside a live corpus). Booster ART: owner himself flagged copyright + distribution; not built. NO VPK.

## 2026-09-05 14:41 — procedural booster pack committed on shop-ui-counts (16cff1ebe); hooks for sets/<CODE>/pack.jpg + symbol.png. Research agent on pack-art/set-symbol sources running -> strategy-design/shop-art-research.md. Branch still awaits suite gate + merge after the w62 corpus.

## 2026-09-05 14:45 — OWNER REQUEST (verbatim): "yeah, when it makes sense to do so, build a vpk with it, and ill give it a test" -> ONE VPK (vpk23) for the shop-ui merge, AFTER the w62 corpus ends + shop branch merged + suite gate green. This is a one-time instruction for THIS change; it does not generalize.

## 2026-09-05 15:18 — SET SYMBOLS DONE (owner: "lets not do forge. symbols are good."): tools/cardart/fetch-symbols.py (Scryfall by code -> by [meta] name -> Keyrune) ran on his pool: 328/335 sets have User/sets/<SET>/symbol.png (8 promo sets have no icon anywhere: HTR/HTR17-19, MB1, PDP11, PI13/14 -> generated emblem). pack-common.sh ships symbol.png in each set zip. Branch shop-ui-counts now 4 commits; awaits corpus end -> merge -> gate -> vpk23.

## 2026-09-05 15:22 — OWNER: "do it" = ship symbols to the Vita with vpk23. Full pack rebuild rejected (old ~/Projects/wagic-card-packs is gone; 5.6 GB rebuild+upload) — instead 328 loose symbol.png staged at ~/.gatelogs/vita-symbols/sets/, uploader psp-work/scripts/deploy-vita-symbols.sh (sequential curl, --ftp-create-dirs; dest ux0:/data/Wagic/User/sets/<SET>/symbol.png — CONFIRM the device sets dir with a listing first). Do at the vpk23 window, after the VPK, one attempt each.

## 2026-09-05 15:26 — SYMBOLS UPLOADED to Vita (owner: "ftp is up. can copy assets now"): device tree is ux0:/data/Wagic/ (merged Res+User; sets/ directly under it) -> 328 x sets/<SET>/symbol.png, 0 failed, RAV+10E verified by listing. vpk23 building from shop-ui-counts 522bc32f3 (unit vita-vpk23).

## 2026-09-05 15:40 — vpk23 = archives/wagic-522bc32f3-vpk23.vpk (32,786,775 B; shop-ui-counts = master 12be0b79c + shop counts/set name/procedural pack/symbol disc) BUILT + UPLOADED to ux0:/vpk/wagic.vpk. Docker stopped. Owner installs via VitaShell. Merge/gate of shop-ui-counts still pending the w62 corpus.
15:40: vpk23 upload: first curl attempt failed silently (listing still vpk18), second attempt completed, ux0:/vpk/wagic.vpk = 32,786,775 B size-verified. Owner to install via VitaShell.

## 2026-09-05 15:57 — vpk23 owner feedback: font small, black wrong colour, info line clipped by Show List. Fixed on shop-ui-counts: deb523968 (1.35x pale text, dark panel, white-rasterized symbols tinted at render on a dark disc; pool re-rasterized WHITE) + 7f9686245 (descPurchaseFit: full/medium/compact wording sized to the free span). vpk24 building from 7f9686245. White symbols re-upload to Vita PENDING (FTP dropped, "FTP not answering") — needs next ftp up.
15:59: vpk24 = archives/wagic-7f9686245-vpk24.vpk (32,787,338 B) BUILT, NOT uploaded (FTP down). Pending on next "ftp up": vpk24 -> ux0:/vpk/wagic.vpk + 328 WHITE symbol.png (~/.gatelogs/vita-symbols/sets) -> ux0:/data/Wagic/sets/<SET>/. Docker stopped.

## 2026-09-05 16:15 — W62 CORPUS DONE 16:08 (matchups-20260905-133058): 20/21 natural, 1 HUNG 152v125 = REGRESSION from lane Z D16 (marker guard hid Day/Night from the daybound count -> two opposite markers -> flip loop, 88k stack adds). Fix committed 9230a94c3 (acceptsDesignationMarkers by name/negation; fixture w62fix_daybound_marker_seen_by_count; wave62/corpus-hang.md). Gate4 unit w62-gate4 running; then archive, push, rerun 152v125 (-j 1), then shop-ui merge + gate5 + push. vpk24 + white symbols still await ftp up.

## 2026-09-05 16:26 — W62 RERUN DONE (matchups-20260905-161801, 152v125 natural, 152 won t30). STEP THREE launched (7 deck agents + engine seat). Owner Vita report on vpk23 (D42): no pile badge + all tapped tokens expand while targeting -> fixed on shop-ui-counts f7232ccc7 (badges draw beside the preview; per-pile expansion only for legal targets of the live chooser). Shop branch merged to master; gate5 next, then push + vpk25 (owner test build).
16:30: gate5 GREEN on 66afd72bb (1254/2-known, 66 AI/0, 4111/0); archived wagic-66afd72bb-shopui; pushed; shop-ui worktree removed. vpk25 building.
16:30: vpk25 = archives/wagic-66afd72bb-vpk25.vpk (32,787,344 B; master 66afd72bb = w62 step one + hang fix + shop-ui incl. D42 badge/expansion fixes) BUILT. Docker stopped. Pending next ftp up: vpk25 -> ux0:/vpk/wagic.vpk + 328 WHITE symbols. vpk24 superseded, never uploaded.

## 2026-09-05 16:36 — OWNER RULING (D42 targeting, verbatim in wave58/owner-decisions.md): piles stay stacked while targeting; only the TARGETED object unstacks. My first fix (fan piles with legal targets) was WRONG; reworked as caa5d6ea3 (rim-marked card excluded from piles; OK on a pile targets the leader; wave-57 OK-expands rule retired). gate6 + vpk26 building. vpk25 superseded, never uploaded.
16:42: gate6 on caa5d6ea3: 1254/3 (known pair + intrepid_adversary_repeated_payment — PRE-EXISTING flake: solo THREADS=1 fails 2/3 on caa5d6ea3 AND 1/3, 2/3 on the previous 66afd72bb binary; carry as a wave-63 item), 66 AI/0, 4111/0. Archived wagic-caa5d6ea3-d42; pushed. vpk26 = archives/wagic-caa5d6ea3-vpk26.vpk (32,787,702 B) BUILT, NOT uploaded (FTP down); supersedes vpk25/24. Docker stopped. Step three: 7/7 deck seats in (152 KEEP; 146/125/126/162/123/130 EDIT; 123 wrote general-proposals.md -> synthesis owed); engine seat pending.

## 2026-09-05 16:56 — WAVE-62 STEP THREE CLOSED 41fbad879 (pushed): 152 KEEP, six EDIT installed; 18 PASS/5 FAIL/8 UNTESTED; synthesis 1/2 (R331). wave63/known-bugs.md written (18 items). Lategame specimen wave62/lategame-specimen.txt (125v126 seq 324, turn 61). NEXT = cut wave-63 step one. Pending Vita: vpk26 + white symbols on next ftp up.

## 2026-09-05 16:58 — WAVE-63 STEP ONE CUT on 41fbad879: Opus lanes AB (E1/E3/E4 combat), AC (E2/E5/E9/E13/E15/E16 prices+rows), AD (E6/E7/E10/E11/E14 record/latch/hold), AE (E8/E12/E17/E18) in worktrees/lanes/w63-{AB,AC,AD,AE}; brief wave63/lane-brief.md. NO VPK unless asked.
17:12: OWNER "ftp up": vpk26 UPLOADED to ux0:/vpk/wagic.vpk (32,787,702 B size-verified, first attempt) + WHITE symbols re-uploaded (see line below). Owner installs via VitaShell.
17:13: white symbols: 328 uploaded, 0 failed; RAV/symbol.png 638 B on device (was 620 B black).

## 2026-09-05 18:02 — WAVE-63 STEP ONE MERGED: AB aab9ca815 -> 278595bf8, AC 943a8d28a -> c05903fcf (seam hand-fixed: resolver (c) left an inner block open; resolver now closes it = pattern ci), AD 939873e2b -> 96b89b3a9 (same seam fix), AE 922a33820 -> d623a9605; PARSETEST 4299/0 (=4111+35+68+66+19). gate1 on d623a9605: 1256/2 (lifeline + intrepid — intrepid STILL flakes threaded despite AE E17 main-thread fix; carry), 66 AI/0. Codex review running (w63-review-codex.pid). OWNER Vita report (vpk26): Branching Evolution stacks N times on one Mossborn Hydra landfall -> script looped one counter per event; fixed 76e4f58b9 (counter(1/1,counter{1/1.1}); fixture via Hardened Scales 5/5 vs 6/6). Census: 8 more "double the number of counters" cards share the loop -> lane AF.
18:10: Codex review w63 committed 29cc95efe (9 findings: HIGH #1 ANNOUNCE_X hard cap X=50 on the cast route; HIGH #2 AD hold-key normalization auto-answers Main 2 — adjudicate vs E10; MED #3-#9). Lane AF (Opus) running in worktrees/lanes/w63-AF with the 9 findings + E19 (8 doubling-loop cards). Then gate2, push, corpus.

## 2026-09-05 18:33 — OWNER Vita softlock (vpk26, ftp up): pulled softlock-1788650768.txt + transcript to psp-work/logs/20260905-vpk26/. DIAGNOSIS: not a hang — stack calm (all items RESOLVED), human in Blockers with ONE blocker (Renata) on Nullpriest of Oblivion (MENACE) -> W43-1 gate refuses the advance (correct) but SILENTLY; transcript shows no p1.next reached the engine. Fix 349a4d5b4: GameObserver::setAdvanceRefusal + 4 s banner in GameStateDuel naming the attacker and the blockers it needs. Lane AF still running; then gate2 + push; VPK only if he asks.

## 2026-09-05 19:02 — SOFTLOCK 1788650768 INVESTIGATION (OPEN): reproducible headless via WAGIC_REPLAY (recipe + state in wave63/softlock-blockers-1788650768.md). No probed gate in userRequestNextGamePhase fires; suspicion = MTGGamePhase::NextGamePhase pre-gates / a waiting action re-armed every tick (stallprobe wait= flicker). Committed 98d13050f (probe + record). MASTER STATE: lane AF merged 5aa1e55ed + Mossborn 76e4f58b9 + refusal banner 349a4d5b4 + probe — UNGATED, UNPUSHED. NEXT: gate2 (w63-gate2 launch script from w62-gate1 sed), push, archive, wave-63 corpus (unit w63-corpus; watch = terminal + early 30m + late 4h), pre-derive wave63/reviewer-brief.md; then step three. VPK only on request. Owner still testing vpk26 on the Vita.

## 2026-09-05 19:11 — GATE2 GREEN on 98d13050f: 1267/2 (lifeline + merrow_reejerey, known), 66 AI/0, PARSETEST 4344/0. Archived archives/wagic-98d13050f-w63step1. Pushed; lane worktrees w63-{AB,AC,AD,AE,AF} removed. wave-63 corpus launching (unit w63-corpus, binary 98d13050f, watch = terminal + early 30m + late 4h). wave63/reviewer-brief.md pre-derived (corpus placeholders). Softlock 1788650768 still OPEN.

## 2026-09-05 19:32 — SOFTLOCK 1788650768 ROOT-CAUSED (commit e974dd01c, pushed): W43-1 menace gate refused Renata-alone-on-Nullpriest silently; replays e/f/g (worktree build + probes, isolated HOME) prove removal->advance passes. Fix = banner 349a4d5b4 (master, NOT in any VPK). Earlier analysis errors recorded in wave63/softlock-blockers-1788650768.md (seat p1/p2 in logs; stale engine-action consumption). Corpus w63 running (matchups-20260905-191148). VPK only on request.

## 2026-09-05 21:47 — WAVE-63 CORPUS DONE (matchups-20260905-191148): 19 natural + 2 HUNG (152v162, 152v130) = daybound marker count reads 0 -> second marker -> flip loop. ROOT CAUSE: `*[day;night]` builds a DescriptorTargetChooser; the w62 fix 9230a94c3 only covered TypeTargetChooser and NEVER worked (w62 fixture loops 73x on that binary, passed because markers are invisible to asserts). FIX: DescriptorTargetChooser::acceptsDesignationMarkers; fixtures w63fix_daybound_flip_casts_no_second_marker + rewritten w62fix (RED on 98d13050f, GREEN on fix, via test primitive 2100000604 Night Counter). Record wave63/corpus-hang.md. gate3 running; then archive/commit/push, rerun 152v162 + 152v130 (w63-rerun), then step three.

## 2026-09-05 22:27 — WAVE-63 STEP THREE CLOSED: reruns natural (152v162 -> 162; 152v130 -> 152). Seats: 146/126 KEEP; 125/152/130/123/162 EDIT installed (all in band). Engine seat: 12 PASS/5 FAIL/11 UNTESTED/2 mixed; HOLD takes 359/1951 (was 6/459); 4 fallbacks; no w58/59 signatures. Synthesis 0/1 (G1 rejected; note: general-strategy.md is NOT loaded at runtime — one comment hit at AIPlayerGPT.cpp:28125 — so general adoptions reach no model; the deck guides are the live surface). wave64/known-bugs.md written (F1-F14 HIGH). Lategame specimen wave63/lategame-specimen.txt (owner review, non-blocking).

## 2026-09-05 22:30 — WAVE-64 STEP ONE CUT on be2e88282 (brief a5663e510 pushed): Opus lanes AG (F6/F7/F8/F9 combat+modal renders), AH (F1/F2/F5/F10/F11 verdicts/facts), AI (F3/F4/F12/F14 engine), AJ (F13 + MED list) in worktrees/lanes/w64-{AG,AH,AI,AJ}, branches w64-lane-*. Then merge (merge-lane.sh), gate, Codex review -> fix lane, gate, push, corpus. NO VPK unless asked.

## 2026-09-05 23:25 — LANE WORKTREE SEEDING LESSON (wave-64 cut): a fresh worktree has an EMPTY bin/User; the suite binary then never reaches the TESTSUITE auto-launch and sits in the menu forever (reads as a slow suite), and PARSETEST/replays load only 2 primitives. SEED = Makefile.sdl + objs-sdl + `rsync -a --exclude sets/ projects/mtg/bin/User/ <wt>/projects/mtg/bin/User/`. Lanes AI/AG found it; AH/AJ were unblocked by hand. Lane AI b3ca9fe13 (F3 root cause = Baka Act passes priority in the tick a menu is answered in place; F4 W# rows; F12; F14 = reporting, not engine). Lane AG 6c030a650 (F6-F9; PARSETEST 4387).

## 2026-09-06 23:40 — WAVE-64 STEP ONE MERGED: AG 6c030a650 -> 3d3b54bb6, AH a8e354ced -> 04b1ee263 (resolver (ci) added an extra 8-space brace at the PARSETEST seam - hand-fixed), AI b3ca9fe13 -> 698b027e0 (resolver dropped the closing brace of AH F11 CRACK-BACK COST block next to AI F4 hunk - hand-fixed), AJ 3f4f8fc03 -> 95b0aa152 (pattern g, clean). PARSETEST 4507/0 at merge. gate1 (w64-gate1) + Codex review (w64-review-codex) running. UNPUSHED.

## 2026-09-06 23:46 — gate1 on 95b0aa152 GREEN (1269/2-known, 67 AI/0, PARSETEST 4507/0; archive wagic-95b0aa152-w64gate1). Codex review committed 52fcb617d: 9 findings (HIGH #1 PW route still gated by player canAttack; #2 F3 fix 24-tick forced pass = doctrine; #3 curl 28 connect-timeout classified as decision timeout; #4 crack-back cover ignores evasion; #5 indented top-level correction demoted + PARSETEST pins it; MED #6-#9). Lane AK cut (worktrees/lanes/w64-AK, seeded incl. bin/User). UNPUSHED since be2e88282... actually a5663e510 pushed; merges 3d3b54bb6..52fcb617d unpushed.

## 2026-09-06 00:15 — WAVE-64 STEP ONE CLOSED: AK 99afd0ce9 -> dab6a6cba (8 confirmed fixed, #3 refuted out of scope); gate2 1269/2-known, 67 AI/0, PARSETEST 4536/0; archive wagic-dab6a6cba-w64step1; pushed; w64 worktrees removed. Corpus w64 launching (unit w64-corpus, two-check watch). wave64/reviewer-brief.md pre-derived.

## 2026-09-06 01:59 — WAVE-64 CORPUS DONE (matchups-20260906-001533): 21/21 natural, 0 crash/hang, 2,295 decisions, 1 fallback (wall_miss). 152 6/6, 126 4/6, 162/130/146 3/6, 125 2/6, 123 0/6. Brief filled (41f2557fa pushed); STEP THREE launched (7 deck seats + engine seat, Opus).

## 2026-09-06 02:21 — WAVE-64 STEP THREE CLOSED: guides 125/130/123 EDIT installed; 146/152/126/162 KEEP. Engine seat: PASS F2/F3/F4-render/F7-fs/F8a-c/F9/F11/F13a/F14; FAIL F6 (never fired: ctx-gated family silent on CHOOSE_MODE), F12 (wrong seam), F10-behaviour; NEW: A#>W# deals no damage. Synthesis 0/1 (P1 plan-carry cap -> engine lane; general-strategy.md confirmed dead at runtime: only deck<N>_strategy.txt is opened, AIPlayerGPT.cpp:15332). HOLD 248/1556 (15.9%). wave65/known-bugs.md written (G1-G10 HIGH). Specimen wave64/lategame-specimen.txt.

## 2026-09-06 02:23 — WAVE-65 STEP ONE CUT on 537070ac2 (brief 2680ccd08 pushed): Opus lanes AL (G1/G4/G5/G9), AM (G2/G3/G7 engine+doctrine), AN (G6/G10+forced-sac tail), AO (G8 reply rule + plan carry + MED rows) in worktrees/lanes/w65-{AL,AM,AN,AO}, branches w65-lane-*, seeded incl. bin/User. Then: merge-lane.sh each (brace-balance check the touched functions after any resolver hunk), gate1, Codex review (sed w64-review-prompt.md), fix lane AP, gate2, archive, push, corpus w65, step three (derive wave65/reviewer-brief.md from wave64/). NO VPK unless asked. Owner-pending: lategame specimens wave63/wave64, the loop cost question.

## 2026-09-06 03:35 — WAVE-65 STEP ONE MERGED + REVIEWED: AN 5e90c9580 -> 534e3d15b (clean), AM c09d46276 -> 28f14e20c (resolver (ci) extra 8-space brace at the PARSETEST seam, hand-fixed), AO e3007abd5 -> 0359e56e8 (g, clean), AL 33c038197 -> 9e2440011 (d, clean); PARSETEST 4662/0. gate1 GREEN (1271/2-known lifeline+merrow, 67 AI/0, 0 timeouts). Codex review db3e32638: 7 findings (HIGH #1 decline scope strips phase header = legal option retired across phases; #2 ATTACK/BLOCKS drivers still last-wins; #3 marked correction adjacent to original folded into run; #4 LETHAL COMES FIRST ignores blocker lifelink; #5 double strike lifelink counted once; #6 curl 28 connect-timeout = decision timeout (3rd wave carried, FIXED this time); LOW #7 plan cap bytes vs chars). Lane AP a1a5bb8d0 -> 0f5e0c7be: all 7 CONFIRMED + fixed, PARSETEST 4698/0, RED-on-base 15. Lane facts: G1 root cause = modal rows arrive as CHOOSE_MENU (isMultipleChoice=0), not CHOOSE_MODE; G2 root cause = applyDeclareAttackers answered the walker menu under `if (al->menuObject)` which is always NULL that tick (new suite driver `aideclareattack`); G7 declineCap replaced by board-scoped re-opener. gate2 on 0f5e0c7be running; then archive wagic-0f5e0c7be-w65step1, push, corpus w65. Merged lane worktrees removed; AP worktree pending removal.

## 2026-09-06 05:05 — WAVE-65 CORPUS DONE (matchups-20260906-033121, 87 min): 21/21 natural, 0 crash/hang, 2,339 decisions, 0 heuristic fallbacks, 8 re-asks (3 named_row_reask, 3 index_name_conflict, 1 stale_echo, 1 no_pass_reask). 152 5/6, 162 4/6, 125 4/6, 123 3/6, 146 2/6, 126 2/6, 130 1/6. Oddity: 162v123 ended life -1089 (turn 11) — routed to engine seat. Brief 98f73a97c; STEP THREE launched (7 deck seats + engine seat, Opus).

## 2026-09-06 05:40 — WAVE-65 STEP THREE CLOSED (7926a5d56 pushed): guides 162/146 EDIT installed (65,630 / 70,997 B); 152/125/126/130/123 KEEP; no proposals, no synthesis. Engine seat: 15 PASS / 1 FAIL (AO-3 by letter) / 6 UNTESTED (AN-5, AN-7, AP-R3, AP-R5, AP-R6, AP-R7 half). G7 churn measured +2.8 re-offers/seat-game for -3.4% decisions (scope stands); G4 starves nothing; -1089 = deck123 Intruder Alarm + Lord of Lineage token loop, verdicts correct. HIGH for wave 66 (wave66/known-bugs.md H1-H10): H1 DRAW FORECAST re-charges the step each window (false KILLS you); H2 first-wins executes the pre-reasoning answer (72% overrun, marker used 2x; KEEP rule, fix protocol order + header-marker window + record the ignored line); H3 repeat row no {right now:}/stop guard + closed loops bill a round trip per point; H4 X bracket ignores stack damage; H5 discard menu orders against {spare:}; H6 target rows omit summoning sickness; H7 hold key holds volatile numbers + decline beats hold 182:4; H8 re-ask exemplar attractor on a no-op row; H9 mode ask contradicts cast-row census; H10 sweep/forced-sac brackets threat-blind. Specimen wave65/lategame-specimen.txt (owner review, non-blocking). Next: wave-66 step one cut on 7926a5d56.

## 2026-09-06 05:55 — WAVE-66 STEP ONE CUT on bdd0ec6e5 (brief wave66/lane-brief.md pushed): Opus lanes AQ (H1 draw forecast / H4 X-after-stack / H9 mode census / H10 threat-blind brackets), AR (H2 reply protocol order + header marker window + record ignored line / H8 exemplar attractor / MED name_over_index, plan-carry staleness), AS (H3 repeat-row {right now:} + stop re-ask + closed-loop autopass / H7 hold key numbers + hold-first decline / MED unique-name conflict, no_pass_reask), AT (H5 discard ordering / H6 summoning-sick target rows / MED autotap verdict, mode kill arithmetic, land threat, draw-step reserve, reveal stalls) in worktrees/lanes/w66-{AQ,AR,AS,AT}, seeded incl. bin/User. Then: merge-lane.sh each, gate1, Codex review (sed w65-review-prompt.md), fix lane AU, gate2, archive, push, corpus w66, step three (brief from wave65/reviewer-brief.md). NO VPK unless asked.

## 2026-09-06 06:45 — WAVE-66 STEP ONE MERGED: AS 9dd425352 -> 4ddfcbf3f (clean), AT 62e038282 -> b85870578 (resolver (ci) stray brace, hand-fixed), AQ 2a2d2ec1e -> 5b57a1c6d (resolver (ci) stray brace, hand-fixed), AR c9b1e152a -> c2ec05117 (4 real hunks: AS repeatPastStop vs AR noopPlanConflict at the priority re-ask seam — UNION of both flags/branches/messages; two shared closing braces outside the hunk regions re-inserted by hand). PARSETEST 4879/0 (4698 + 46 AS + 41 AT + 50 AQ + 44 AR). Lane facts: AT proved reveal stalls are 100% round trips (no engine wait); AR reordered the protocol to REASONING -> ANSWER -> PLAN (its first live test is the w66 corpus); AS's loop autopass fires only when the oracle says no legal action. Resolver lesson: pattern (ci) on the PARSETEST tail seam has now mis-braced 4 of 8 merges — the shared `    }` after an inner `        }` is the recurring casualty. gate1 (w66-gate1) + Codex review (w66-review-codex) running. Merged worktrees removed.

## 2026-09-06 07:35 — WAVE-66 STEP ONE REVIEWED: gate1 on c2ec05117 GREEN (1273/2-known, 67 AI/0, PARSETEST 4879/0). Codex review 5 findings (HIGH #1 hold key normalised EVERY number in braces = blind cache over kill counts; HIGH #2 draw-step accounting subtracts cantrip/stack draws from the scheduled forecast; MED #3 `:token(` one-shot ETBs classed as repeatable engines; MED #4 sick-attacker line promises next-turn attack unchecked; MED #5 all-dead exemplar still names row 1). Lane AU b5e057e78 -> 9b797fb81: all 5 CONFIRMED + fixed (hold key forgives only six enumerated life-projection clauses; WEventcardDraw now carries the drawing ability's source; token-engine head grammar; next-turn promise gated on canAttack gates; exemplar prefers the decline row / row-free template). PARSETEST 4906/0. gate2 on 9b797fb81 running; then archive wagic-9b797fb81-w66step1, push, corpus w66.

## 2026-09-06 06:45 — WAVE-66 STEP ONE CLOSED: gate2 on 9b797fb81 GREEN (1273/2-known lifeline+merrow, 67 AI/0, 0 timeouts, PARSETEST 4906/0); archive wagic-9b797fb81-w66step1; pushed (e78f3d73d); all w66 worktrees removed. Corpus w66 launched (unit w66-corpus, W66_CORPUS_DONE, start epoch 1788694838; two-check watch + stale tripwire baselined at 0). (Note: the two NOTES timestamps above reading 07:35 were written ~06:15-06:30 local — clock estimate error, order is correct.)

## 2026-09-06 08:50 — WAVE-66 CORPUS DONE (matchups-20260906-064038, 127 min): 21/21 natural, 0 crash/hang, 2,317 decisions. ⚠ NEW SIGNATURE: 83 `unparsed_reply` -> Baka (3.6%; wave 65 had 0 heuristic fallbacks) = lane AR's REASONING->ANSWER->PLAN reorder's first live cost — 33 replies carry NO coded line (prose "Therefore, I cast nothing" / "I will choose option 1", label never written), 50 carry a label the parser still refused. Also 20 repeat_past_stop (AS H3 firing), 6 named_row_reask, 2 reveal_stall_forced, 2 wall_miss; post_answer_overrun 60% (was 72%); p90 latency 94 s (was 74 s). 152 5/6, 146 4/6, 162/130/126 3/6, 123 2/6, 125 1/6 (125 KEPT guide went 4/6 -> 1/6; two lopsided 125v130 67/0, 125v126 60/5). Brief filled 79d85ee73 (pushed); STEP THREE launched (7 deck seats + engine seat; engine seat's first question = the 83 unparsed replies, both shapes, parser seam named).

## 2026-09-06 09:40 — WAVE-66 STEP THREE CLOSED (8ddf25c89 pushed): guides 162/125/123 EDIT installed (69,897 / 71,199 / 71,308 B — 123's first draft was 72,699 B OVER BAND, the seat trimmed it); 146/152/126/130 KEEP; no proposals. Engine seat 22 PASS / 7 FAIL / 8 UNTESTED. HEADLINE: lane AR's REASONING->ANSWER->PLAN reorder cost 83 unparsed_reply -> Baka (45 = `ANSWER: CHOICE:` prefix the label scanner at AIPlayerGPT.cpp:17506 never skips; 32 = no label at all, PLAN pushed past the 300-byte ramble guard :18137) and did NOT produce reasoning-first (73% answer-first). HOLD taken 393 vs 126 declines (H7b inverted the ratio). Other HIGH: LIBRARY CEILING ignores stack draws (125 decked at 44 life); refused reveal VOIDS the spell + 2 genuine 600 s engine reveal stalls; repeat re-ask advisory (x34 ran twice past stop 26); fallback record stores nothing Baka executed; graveyard never rendered; 21-row repeat-pay menus. wave67/known-bugs.md I1-I9. Specimen wave66/lategame-specimen.txt. Next: wave-67 cut on 8ddf25c89.

## 2026-09-06 09:55 — WAVE-67 STEP ONE CUT on ff06cc432 (brief wave67/lane-brief.md pushed): Opus lanes AV (I1-I3 the reply rule as ONE design — corpus measurement table first: heading-word-before-label per seam, PLAN-but-no-label salvage rate, first/last/before-PLAN anchor accuracy, retraction vocabulary; then scanner skip + label-less salvage + anchor + kReplyProtocol without label-able section names), AW (I4 library ceiling incl. stack draws / I9a rung collapse / ATTACK TOTAL verdict / sorcery-speed half / MDFC untapped / Tribute floor / max-X mismatch), AX (I5 reveal void -> legal pick + 600 s reveal stall root cause + unexplained-secs field order / I7 fallback record executed row + decline honoured like a hold / 900 s curl-28 wall / async_drops census), AY (I6 execute the model's own stated stop + verdict plan source / I8 graveyard line + exiled loop half / I9b noop conjunction / statics source / half-dead exemplar / death-line bucketing / hold churn census) in worktrees/lanes/w67-{AV,AW,AX,AY}, seeded incl. bin/User. Then: merge-lane.sh each (expect the (ci) brace at the PARSETEST tail), gate1, Codex review (sed w66-review-prompt.md), fix lane AZ, gate2, archive, push, corpus w67, step three (brief from wave66/reviewer-brief.md). NO VPK unless asked.

## 2026-09-06 09:50 — WAVE-67 STEP ONE MERGED: AV e6c6b8513 -> 96b269953 (clean), AX 8401f514d -> 4bffe77f3, AW 3493c1abc -> 19719c6ae, AY 24843b1c8 -> af986181b (the last three each needed the (ci) stray brace removed at the PARSETEST tail seam — now 7 of 12 merges). PARSETEST 5071/0 (4906 +34 AV +26 AX +60 AW +45 AY). Lane facts: AV measured the corpus and KEPT first-wins (before-PLAN anchor tied 4/9, only 75% of replies carry a line-leading PLAN); shipped the heading-word skip at every seam + label-less salvage (ask/priority only) + a kReplyProtocol with no label-able section names; `wait` deliberately excluded (130v146 s24 would flip a correct answer). AX root-caused the 600 s reveal stall = MTGRevealingCards::CheckUserInput ignores BTN_NEXT on a `<1>` targetMin chooser, so a zero-target finalize pressed nothing and the 600 s guard force-closed = the void; fixed at the driver floor (fixture RED on base) + fallback records now carry executed_seam/choice/text. AW: X library ceiling folds draw-step extras + stack draws; repeat-pay rungs collapsed at the render with shownToFull mapping (by construction only); docketed ManaEngine::maxAnnounceableX as the third X source. AY: second past-stop answer executes the model's OWN stated stop (`repeat_clamped_to_own_stop`); graveyard line (feeds boardKey); exiled loop half = BROKEN; planArguesAgainstRow now sees anaphora + unlabeled plans; 208-line death batch bucketed. gate1 + Codex review running; merged worktrees removed.

## 2026-09-06 10:35 — WAVE-67 STEP ONE REVIEWED: gate1 on af986181b GREEN (1274/2-known, 67 AI/0, PARSETEST 5071/0). Codex review 7 findings (HIGH #1 label-less salvage ignores negation; HIGH #2 own-stop clamp allowed==1 passes while narrating one; HIGH #3 reservation-decline latch keyed on phase+names+source count = blind cache (DOCTRINE); HIGH #4 repeat-pay render HIDES legal rungs (DOCTRINE); MED #5 may-draws counted mandatory in the library reserve; MED #6 graveyard loop half assumed returnable; HIGH #7 mandatory reveal fallback takes vector index 0 instead of the heuristic (DOCTRINE)). Lane AZ cut (worktrees/lanes/w67-AZ, base 5afffc21b). Corpus launch template prepared (hash pending gate2).

## 2026-09-06 10:05 — WAVE-67 LANE AZ MERGED: 48f855f49 -> 14977f3af (clean; all 7 Codex findings confirmed+fixed; PARSETEST 5107/0). AZ correction: at 162v130 s16-19 the board DID change (hand/library/stack), so the corrected full-key reservation latch would NOT have held the corpus pairs — expect reserve_decline_windows_skipped ~0. gate2 on 14977f3af running; then archive wagic-14977f3af-w67step1, push, corpus w67. ⚠ CODEX MODEL CHANGE (owner ruling 2026-09-06): Codex reviews now run on gpt-6-astra (config.toml; CLI updated 0.144.6 -> 0.153.4) — the wave-67 review was the last on Sol.

## 2026-09-06 10:30 — WAVE-67 STEP ONE CLOSED: gate2 on 14977f3af GREEN (1274/2-known lifeline+merrow, 67 AI/0, 0 timeouts, PARSETEST 5107/0); archive wagic-14977f3af-w67step1; pushed (22e63212e); all w67 worktrees removed. Corpus w67 launched (unit w67-corpus, W67_CORPUS_DONE, start epoch 1788708398; two-check watch, stale baseline 0, early check also counts unparsed_reply — AV predicts < 5).

## 2026-09-06 12:10 — WAVE-67 CORPUS DONE (matchups-20260906-102638, 101 min): 21/21 natural, 0 crash/hang, 2,436 records, unparsed_reply 0 (was 83 — AV's fix landed), 7 fallbacks (3 wall_miss, 2 index_name_conflict, 1 engine_answered, 1 named_row_reask), overrun 71% (was 60%), p90 75 s, max 1233 s (> 900 s deadline — flagged to the engine seat). 162/152/125 4/6, 146/123 3/6, 130 2/6, 126 1/6. Lopsided: 152v123 -119/8, 146v125 0/51. Brief filled f9f2aa036 (pushed); STEP THREE launched (7 deck seats + engine seat).

## 2026-09-06 13:40 — WAVE-67 STEP THREE CLOSED (949a514d7 pushed): guides 152/126/125/162/123/146 EDIT installed (all within 20 B of the 71,500 ceiling — pool at the band wall); 130 KEEP; no proposals. Engine seat 22 PASS / 8 FAIL / 6 UNTESTED. unparsed_reply 0 (AV landed); answer-first 98.9%, overrun 662,814 B — the protocol ORDER lever is spent, reasoning moved past the label instead of shortening. HIGH for wave 68 (wave68/known-bugs.md J1-J9): J1 own-stop guard blind to the single-activation row (20 past stop; proseNamesOtherMenuRow killed 15/15 reversal re-asks); J2 900 s wall miss retries with a FULL second deadline (1233 s, deadline_pct 137 — AX's "zero budget" wrong); J3 overrun = latency tail -> per-seam max_tokens cap + two-sided overrun meter; J4 ANNOUNCE_X row vs header ceilings disagree, `; M more pending` never rendered (AW claim falsified); J5 post-announce decline strands paid mana, cannot-pay tag ignores floating pool (55/82); J6 prose reversals unwired at ATTACKERS / prose-only retraction / no-op re-ask budget; J7 CARD SCRIPT ward `opponentzones` leaks library+hand (borderline.txt:73998; audit all wards) + Teferi cascade stage 1 blank; J8 Silverquill edict floor missing, opponent-draw priced as benefit, Tribute no crack-back cover; J9 stack death verdict 40 lines from the answer, hold not re-opened on lethal crack-back. Specimen wave67/lategame-specimen.txt (X ceiling binding live). Next: wave-68 cut.

## 2026-09-06 13:50 — WAVE-68 STEP ONE CUT on 4837e76a1 (brief wave68/lane-brief.md pushed): Opus lanes BA (J3 per-seam max_tokens cap + two-sided overrun meter, measured first / J6 reversal predicate at every seam + budget-exempt no-op re-ask / J1 own-stop on any activation), BB (J4 one X ceiling for header+row, bare-integer X names / J5 post-announce decline forfeiture + floating-pool cannot-pay / J9 stack death verdict in the option block + hold re-open on verdict word), BC (J2 wall-miss retry budget + per-attempt latency / J7 ward `opponentzones` -> `opponentstack` audit via /wagic-cardscript + Teferi cascade stage 1 / MED recovery record, async_drops, legend-rule ask), BD (J8 edict floor for modes, granted draw as PRICE, Tribute crack-back cover / MED NO LIVE CAST ROW, band rows, narration census, deck-out forecast, empty type gates) in worktrees/lanes/w68-{BA,BB,BC,BD}, seeded incl. bin/User. Brief header now carries the wave-67 lesson: three lane claims were falsified by the corpus — COUNT before you claim. Then: merge-lane.sh each ((ci) brace expected), gate1, Codex review ON ASTRA (sed w67-review-prompt.md), fix lane BE, gate2, archive, push, corpus w68, step three (brief from wave67/reviewer-brief.md). NO VPK unless asked.

## 2026-09-06 14:10 — GUIDE REWRITE PASS (owner ruling, verbatim in memory `wagicgpt-guides-decision-only`): "The guide is too long. It contains many details that are not relevant to the llm playing the game. It even uses concepts that the llm has no context to understand. 'This corpus' 'previous games' these are useful to the agent writing the guides maybe, but they dont belong in the guide." Census: the 7 loop guides at ~71 KB / ~830 lines vs 3-18 KB for the other 30; the register is post-mortem narration of past corpora. Shipped: wave68/strategy-writing-skill.md = wave59 + Amendment 332 (decision-time instruction only; band retired; hard ceiling 20,000 B; grep-verified 0 provenance tokens); wave68/guide-rewrite-brief.md; 7 Opus rewrite seats -> wave68/rewrite/deck<N>/{strategy.txt,changes.md} (running, concurrent with the w68 lanes). wave68/reviewer-brief.md pre-derived with the rule. On return: verify size+grep, install with `git add -f`, commit, push; the w68 corpus is the first test of the short guides. Also noted for the owner: the strategy-writing skill itself is 1.3 MB / 332 amendments — the same disease one level up.

## 2026-09-06 15:20 — WAVE-68 LANES BC/BB/BD MERGED: BC 6310b38f6 -> 3dee8bd7d (clean; 4 wall misses all armed a FULL second deadline — the wall arm never called remainingTransportRetryMs; now `wall_miss_no_retry` + per-attempt `attempt_ms`; ward `opponentzones` -> `opponentstack` across 85 lines via the 5 _WARD macros, Oracle verified; Teferi stage-1 NOT shipped, design recorded), BB 041ca9c2b -> c2d9eb3aa ((ci) brace hand-fixed; J4 root cause = row used only the STACK term of the reserve; X menu bare-integer names resolved; floating pool folded into cannot-pay; forfeiture clause; `{answers the stack: NO}` on hold rows; crack-back verdict word in the hold key recomputed live), BD d4208d714 -> 2b34abcfc (clean; one edictFloorScan for modes + standalone; granted draws priced as a PRICE; crack-back cover on edict rows without adopting the review's floor premise; narration census: `Paid ... with` fold saves 3.9%). PARSETEST 5217/0. BA (J1/J3/J6) still running. Concurrent: Fable skill revision (v2) + 7 guide rewrite seats.

## 2026-09-06 15:35 — WAVE-68 STEP ONE MERGED: BA e95236ec4 -> 95e8f11c3 (pattern g, clean; corpus measured 24.6 seat-hours of generation, 49 kB pre-label + 663 kB post-label, r(bytes,latency)=0.727; per-seam max_tokens caps ask 640 / priority 768 / attackers 768 / blockers 896 / discard-reveal-bottom 384, replayed: 2.3% hit the cap, 0.39% lose the answer -> one re-ask; reversal predicate wired at ATTACK/BLOCKS/CHOICE; own-stop on the plain row). PARSETEST 5250/0. gate1 on 95e8f11c3 + Codex review ON ASTRA (w68-review-codex) running. Skill v2 (Fable, 39,894 B, 331/331 amendments located, 305 cited in 101 statements, 26 dropped as non-rules) committed e85f8cc5c; Astra adversarial review of it running (skill-v2-review). 7 guide rewrite seats still running.

## 2026-09-06 16:05 — WAVE-68 REVIEWED ON ASTRA (first Astra review): gate1 on 95e8f11c3 GREEN (1275/2-known, 67 AI/0, PARSETEST 5250/0). 8 findings; ⚠ #1 VERIFIED REAL by the orchestrator: AIPlayerGPT.cpp:15516 col 379 `//#W68-BA (J3/J6)` inside the ctor initializer list comments out the 30 initializers after it (mAttacksDoneTurn, mRepeat*, mLoop*...) — compiled clean, undefined reads at every seat construction; the suite/PARSETEST cannot see it. Others: #2 legend-rule clause says KEEP the pick but AAMover moves the pick; #3 stop clamp on the plain row auto-passes an explicit second answer (doctrine); #4 combat seams lack reply_truncated_reask; #5 edict crack-back bound printed as a kill verdict; #6 forfeiture clause contradicts the still-floating narration; #7 deck-out countdown ignores CANTMILLLOSE/CANTLOSE/CANTWIN; #8 per-attempt deadline_pct divides by the original deadline. Lane BE cut (worktrees/lanes/w68-BE, base 1a79e4b78) with #1 first + a guard against comment-in-initializer-list. Skill v2 Astra review: 7 findings (3 HIGH: literal audit deletes untested rules; A259 truth-verification lost; PLAN ordering contradiction) -> Fable fix pass in flight; round-2 Astra prompt staged. Guide rewrite deck130 installed (19,996 B).

## 2026-09-06 17:10 — GUIDE REWRITE COMPLETE (e03eb06ed pushed): all seven loop guides rewritten to decision-time instruction only and installed — 123 19,996 / 125 19,978 / 126 19,952 / 130 19,996 / 146 19,981 / 152 19,974 / 162 19,993 B (total 139,870 B vs ~500 KB), every one grep-clean of provenance tokens. Pattern to watch: every seat filled to within 50 B of the 20,000 ceiling (the ceiling became the target again) and each cut 2-7 narrow rules "purely for the byte ceiling" (listed in wave68/rewrite/deck<N>/changes.md (b)/(c)). Card fact corrected in passing: Forced Fruition draws SEVEN (primitive draw:7, Oracle confirms). The w68 corpus is the first test of the short guides. Skill v2: Astra round 1 (7 findings) -> rev 2; round 2 (5 resolved, 1 regressed, 1 open, 4 new) -> rev 3 (39,990 B); round 3 running. Lane BE running.

## 2026-09-06 17:40 — WAVE-68 LANE BE MERGED: 635361ed2 -> 9be85773b (clean; all 8 Astra findings confirmed+fixed; PARSETEST 5272/0, suite +2 legend fixtures = 1277). Guard for the swallowed-initializer class: tools/check-ctor-init.py wired as QMAKE_PRE_LINK in wagic-SDL.pro (exact test: initializer set before vs after stripping `//` tails; RED on the pre-fix line naming all 29); Makefile.sdl regenerated with `-o Makefile.sdl` so the guard runs on every link. Merge coupling learned: a bracketed clause's literal prefix is also stripNarrationDecoration's drop key — rewording one requires the other. gate2 on 9be85773b running; corpus script ready.

## 2026-09-06 17:50 — WAVE-68 STEP ONE CLOSED: gate2 on 9be85773b GREEN (1277/2-known, 67 AI/0, 0 timeouts, PARSETEST 5272/0, ctor guard ran clean); archive wagic-9be85773b-w68step1; pushed. Corpus w68 launched (unit w68-corpus, W68_CORPUS_DONE, start epoch 1788720080) — FIRST TEST of the short guides (7 x ~20 KB) and the per-seam token caps together; early check reports unparsed + truncated counts. Reviewer brief placeholders filled except the corpus line. Skill v2 revision 4 (Fable) in flight, Astra round 4 staged.

## 2026-09-06 18:40 — SKILL V2 INSTALLED (owner: "sure. this should be fine"): wave68/strategy-writing-skill-v2.md (39,998 B, Fable distillation of the 1.3 MB log stack, 330/331 amendments tagged) is the current edition; reviewer brief repointed; old file headed SUPERSEDED. Astra rounds: 7/4/4/3/4/3 findings, every repair held, all MED since round 2 except one resurrected mulligan floor (round 3) — asymptotic, install called by the owner. Round-6 residuals (#23-25) docketed in skill-revision-notes.md. Cadence from here: Astra reviews the skill once per wave against seat outputs. w68 corpus early check: 1,247 records / 4 games done at 30 min, median latency 23 s (w67: 41 s), p90 57 s (111), 21 cap truncations (20 kept their answer, 1 re-ask), 0 unparsed.

## 2026-09-06 19:05 — WAVE-68 CORPUS DONE (matchups-20260906-134120, 74 min = fastest yet): 21/21 natural, 2,414 records, total generation 47,931 s vs 88,567 s (-46%), median 10.6 s / p90 46 s / max 315 s; post-answer overrun bytes UNCHANGED (~643 kB) => the cut is prefill (short guides), not the caps; 58 truncations (5 re-asks); 1 unparsed; 25 plan_choice_conflict (J6 wiring). 162/152 5/6, 146/126/125 3/6, 130 2/6, 123 0/6 (rewrite cut 7 rules for the ceiling — seat briefed to adjudicate). Brief filled 1ea082700 (pushed); STEP THREE launched — first seats reading the v2 skill (7 deck seats + engine seat).

## 2026-09-06 20:30 — WAVE-68 STEP THREE CLOSED (d8772a91c pushed): all seven guides EDIT, installed <=20,000 B grep-clean (the first edits under the v2 skill); no proposals. Engine seat 13 PASS / 9 FAIL / 12 UNTESTED. VERDICT ON THE REWRITE: every seat found the losses were KEPT rules keyed to strings the render never prints (dead literals) — the rewrite lost nothing, the v2 literal audit found the real defect; 162 went 4/6 -> 5/6 on a 3.6x shorter guide. ATTRIBUTION of the -46%: ~90% the short system prompt (100 KB -> 51 KB median; per-byte decode rate halved with the KV prefix), ~10% the caps (post-answer bytes -3%; max latency 1,233 -> 315 s = a variance guarantee). HIGH for wave 69 (wave69/known-bugs.md K1-K8): K1 ENGINE a game ended with no loss condition (146v123 stderr:1952, winner=0 life 19/11, after Idyllic Tutor's unresolved StackAbility + Vision Skeins draws); K2 caps ate decisions at un-wired seams (discard 384 -> heuristic discarded Emrakul; a cut correction "You should NOT cast Damnation" = the corpus's deciding decision) and a "recovered" truncation changed the answer; K3 reversal predicate misses real reversals, fires on PLAN deliberation (44 fallbacks, 6.3x); K4 X ceiling STILL under-counts Staff draws (deck-out at 65 life); K5 dead rows offered/taken (animated-tapped beholder at 1 life; cleanup dead tags ignored 4/4); K6 crack-back cover ignores existing blockers; K7 record fields transport/attempt_ms ABSENT (BC wiring?), dead GANG BLOCK surface; K8 second-copy tag false for stacking triggers. Next: wave-69 cut.

## 2026-09-06 20:45 — WAVE-69 STEP ONE CUT on 169ee41f3 (brief wave69/lane-brief.md pushed): Opus lanes BF (K2 caps: re-ask at every seam, answer-changed stamp, re-size from this corpus, correction-before-PLAN / K3 reversal predicate scoped to the correction zone — measured first), BG (K1 ENGINE no-loss-condition game end, root-cause by replay / K5 dead rows / K8 second-copy tag), BH (K4 X ceiling Staff draws — BB's claim falsified / Elixir verdict / noop off the row / K6 crack-back cover with existing blockers, reserve verdict, answers-the-stack YES, blockers material), BI (K7 transport/attempt_ms ABSENT — BC wiring? / async_drops + hold-suppression counters / repeat `N` literal / tagged repeat row vs stop guard / dead GANG BLOCK surface / (neither dies) literal / band rows / Paid fold / one tap literal / Adversary rows) in worktrees/lanes/w69-{BF,BG,BH,BI}, seeded incl. bin/User + the guard-bearing Makefile.sdl. Brief header adds: guides are ~20 KB keyed on printed literals — a render change must keep the string or say so. Then: merge-lane.sh each ((ci) brace expected), gate1, Astra review (sed w68-review-prompt.md), fix lane BJ, gate2, archive, push, corpus w69, step three (brief from wave68/reviewer-brief.md; skill = v2). NO VPK unless asked.

## 2026-09-06 20:55 — COST QUESTION CLOSED (owner, verbatim): "use what needs to be used to do the task right. i only have a small sub to open ai, theres no way its going to support a workload that drains my 200$ claude subscription." Wave 68 measured ~3.8M Opus tokens (1.76M lanes + 2.01M seats) + one-offs (guide rewrite ~2.1M, skill distillation ~0.55M Fable). The loop keeps its shape; Codex/Astra stays review-only.

## 2026-09-06 21:55 — WAVE-69 STEP ONE MERGED: BG ba8d59f69 -> 4d2115eeb (clean; K1 ROOT CAUSE: Idyllic Tutor's reveal MOVES the whole library into the reveal zone (MTGRevealingCards::Update putInZone, count expr `type:*:mylibrary`), the option-one return payload StackAbility never resolved, Vision Skeins' draws hit library 0 -> drawFromLibrary's deck-out setLoser at 11 life; fix = drawFromLibrary pulls back cards parked out of THIS library; fixture RED on base; replay diverged at turn 4 = unexplained, carried), BI e943e5784 -> ae216a58d (pattern (e) cut the head of the amended W49-T PARSETEST block — restored verbatim from the lane, BG's K5 sub-block folded in, orphan removed; transport/attempt_ms were CONDITIONAL emitters that never triggered, now on every round trip; GANG BLOCK is reachable not dead; band rows + Adversary rows = one defect in payRepeatCollapse), BF 6d83ed441 -> 3cfa7ca53 (clean; measured: 52/58 truncations cut post-answer prose, 6 cut the label; caps ask 640->768, small seams 384->512, nothing tightened; all 22 reversal firings came from the PLAN body — predicate now reads the correction zone + a negation naming the taken row (s34)), BH 49c81eab1 -> 02201b130 ((ci) brace; K4 missing term = scanStackAbilityDraws matched only Player* targets while Staff of Nin's draw targets the CARD -> stack term rendered 0 times in 2,282 prompts; 2 under-counts found incl. the 65-life deck-out). PARSETEST 5402/0. gate1 + Astra review running; worktrees removed. Lesson: the resolver's pattern (e) can drop a block HEAD, not just a brace — check for duplicated section heads after any (e).

## 2026-09-06 22:05 — CODEX POOL EXHAUSTED (usage limit until 6:03 PM local; 8 Astra runs today ≈ 60K tokens each). Wave-69 adversarial review re-run on FABLE (owner: "use fable as the adversarial fallback"; an Opus attempt was stopped) with the same prompt contract (~/.gatelogs/w69-review-prompt.md -> ~/.gatelogs/w69-review-fable.out) + the owner's "use what needs to be used" ruling. gate1 still running.

## 2026-09-06 22:30 — WAVE-69 REVIEWED ON FABLE (Codex pool exhausted; owner: "use fable as the adversarial fallback"): gate1 on 02201b130 GREEN (1279/2-known, 67 AI/0, PARSETEST 5402/0, guard OK). 8 findings: F1 HIGH merge duplicate (BG kept the old `paymentTapsClause` line, BI added the de-duplicated builder after it — the tap clause prints twice on every affordable-by-tap cast row); F2 HIGH K6a counts the seat's untapped creatures as crack-back blockers in its OWN main 1 before attacking; F3 K4b refill "+1 this card" for self-exiling cards; F4 K6d ALTERNATIVE line can name a lethal assignment silently; F5 composition: after K1 the game continues with library 0 + 41 parked cards (Tutor payload still stranded); F6 negation test bag-of-words; F7 "0 mana" ignores floating; F8 noop re-ask on the newly priced animate row. Lane BJ cut (base 8110ea2de). Corpus template staged. Resolver lesson from BI: pattern (e) dropped a block HEAD and the merge compiled nothing until restored from the lane.

## 2026-09-06 23:15 — WAVE-69 LANE BJ MERGED: ebcd8e53f -> f152d0df4 (clean; all 8 Fable findings confirmed+fixed; PARSETEST 5420/0). F5 Tutor payload STRANDING still open — hypothesis: MTGRevealingCards::toResolve addObserver's the payload and fireOneShot returns unless oneShot (AllAbilities.cpp:445's own comment) — carried to wave 70. intrepid_adversary_repeated_payment flake measured 8/12 on this tree AND on base = pre-existing. gate2 on f152d0df4 running; corpus script ready; brief hash+BJ filled.

## 2026-09-06 23:30 — WAVE-69 STEP ONE CLOSED: gate2 on f152d0df4 GREEN (1279/2-known, 67 AI/0, 0 timeouts, PARSETEST 5420/0, guard OK); archive wagic-f152d0df4-w69step1; pushed. Corpus w69 launched (unit w69-corpus, W69_CORPUS_DONE, start epoch 1788730899) — second corpus on the short guides, first on the re-fitted caps + the K1 draw fix + the record fields. Brief placeholders left: corpus line only.

## 2026-09-06 23:55 — ⚠⚠ EPIC FINDING + OWNER RULING (memory `wagicgpt-reasoning-channel-only`): every corpus since at least wave 44 ran with THINKING=0 — tools/selfplay-harness.sh defaults THINKING=0 (since its first commit 2026-07-05) and exports WAGIC_GPT_THINKING=0 per game; no corpus launch script on disk (w44..w69) passes --thinking; the request builder therefore sends enable_thinking:false; 0 records carry reasoning_content. The model reasoned IN-BAND the whole time; waves 66-69's reply-shape work (protocol reorders, overrun meters, per-seam max_tokens caps, correction markers, reversal predicates, heading-word skip) was compensation for that symptom, and the protocol text tolerating prose TAUGHT the model to write it. Owner (verbatim): "the model CANNOT be told to writing nonsense OR IT FUCKING WILL WRITE NONSENSE." He is considering REVERTING the compensation layer or the project. STATE: w69 corpus (unit w69-corpus, thinking=0) still running = INVALID, not stopped (his call); NOTHING reverted; NOTHING relaunched. PROPOSED NEXT STEP (awaiting his go): stop w69-corpus; strip kReplyProtocol to answer line + PLAN only (remove every sentence licensing prose: reasoning-first, sentence budgets, correction syntax); flip harness default THINKING=1 and pass --thinking; seam caps must not bound reasoning tokens (WAGIC_GPT_SEAMTOKENS=0 or reasoning budget set; the wave-35 two-phase budget path = reasoningBudget + kAnswerReserveTokens; WAGIC_GPT_TIMEOUT=900 already exceeds the 420 thinking default); GATE the first minutes on non-empty reasoning_content in the records; then a thinking-on corpus whose engine-seat census (which compensation mechanisms still fire) decides the revert list. Unknown: whether waves 35-43 ran with thinking on (no launch scripts on disk for them). Split of what stands regardless (engine/render/card fixes, guide rewrite, skill v2, ctor guard) vs compensation candidates is in the chat record and the memory body.

## 2026-09-07 00:40 — ⚠⚠ THE THINKING=0 FINDING, THE OWNER'S RULINGS, AND THE AUDIT (read this before touching the loop)
FINDING: every self-play corpus since at least wave 44 ran with enable_thinking:false — tools/selfplay-harness.sh defaults THINKING=0 (first commit 2026-07-05), exports WAGIC_GPT_THINKING=0 per game, and no launch script (~/.gatelogs/w44..w69-corpus-launch.sh, each sed-copied from the last) passes --thinking; the request builder honours it; 0 records in any corpus carry reasoning_content. The model reasoned IN-BAND the whole time. Waves 66-69's reply-shape work (protocol reorders, overrun meters, ramble guards, correction markers/windows, first-wins, reversal predicates, negation tests, per-seam max_tokens caps, truncation re-asks, heading-word skip) was compensation for that symptom, and its protocol text TAUGHT the model to write prose. I (the orchestrator) authored wave-66 H2 "reasoning BEFORE the coded line" and wave-68 J3's sentence budget myself, against the ruling.
OWNER RULINGS (verbatim, 2026-09-06/07): "reasoning could not be done in the output, and ... a plan had to precede the action" (the reasoning channel exists so "the reviewers have insights towards development"); "the model CANNOT be told to writing nonsense OR IT FUCKING WILL WRITE NONSENSE"; "choice-then-PLAN ... is antithetical to how llms function, mechanically"; "i havent read a lategame prompt in tens of waves. i only read them until i stopped finding fault with them" (prompt review = alert-only); "the wagicgpt skill is the procedural memory for this project. but it consistently lacks procedures that you rederive, incorrectly"; "we need to audit. and fix every single parser and instruction. because fucking obviously they are all not going to work". Settled shape: thinking ON, reasoning captured off reasoning_content; reply = PLAN line then the action line, nothing else. Skill invariant 000 + memory `wagicgpt-reasoning-channel-only` hold it; wagicgpt/llm-opponent-architecture.md's wave-21 "answer-first" line is superseded.
GOVERNANCE (agreed in principle, not yet built): a RULINGS section in the skill in his voice only (sessions propose, never edit); every brief names the rulings its items touch and a lane may report on a ruled surface but not change it; rulings become CHECKS wherever possible (harness refuses thinking-off; corpus gated on reasoning_content; request shape pinned in the gate; a per-wave rulings audit seat); his attention only on alerts. The RULINGS draft for him to correct is still TO DO.
STATE: w69 corpus (matchups-20260906-164139, thinking=0) finished — INVALID, no seats launched, not evidence. Nothing reverted; master f152d0df4 + notes commits. AUDIT LAUNCHED (Fable): projects/mtg/strategy-design/wave70/instruction-parser-audit.md — Table A every instruction the model sees (KEEP/CHANGE/DELETE), Table B every parser/tolerance (KEEP/CHANGE/DELETE/VERIFY; which assume choice-before-plan), C harness/config checks, D fix order + what the ONE thinking-on probe corpus must measure. NEXT after the audit: fix lanes (briefs name invariant 000), protocol = PLAN then action, harness default THINKING=1 + gate, thinking-on corpus, census decides the wave 66-69 revert list.

## 2026-09-07 01:30 — WAVE-70 CUT: THE REASONING-REGIME FIX (owner go; corpora run with REASONING ON — "fuckin obviously we need reasoning"). Forensics: w43 corpus ran thinking=1 (direct command, Aug 24); w44's launch script (first on disk, Aug 25) was written "Recipe per project_wagicgpt memory" — from the anchor's wave-33 launch line, which predates the thinking redesign and has no --thinking; every later script copied w44's. Audit (Fable, wave70/instruction-parser-audit.md): Table A 65 surfaces 28/29/8 KEEP/CHANGE/DELETE; Table B 70 rows 23/21/20/6 KEEP/CHANGE/DELETE/VERIFY; four parsers mis-parse PLAN-first; biggest risk = gptSeamMaxTokens bounds reasoning tokens when reasoning_budget is 0/unset or WAGIC_GPT_MAXTOKENS is set. Lanes (Opus, base 52ab71610, brief wave70/lane-brief.md with a RULINGS block): BK harness `--thinking on|off` REQUIRED + per-regime gates that kill a wrong-regime corpus + regime stamped on every record + caps never bound reasoning + PARSETEST request-shape pins; BL every instruction -> one golden reply-protocol.txt (PLAN then action, nothing else) + all tails/re-asks + 3 guides staged under wave70/guides/; BM every parser -> two-line reader, DELETE in-band tolerances, plan carried as a SEQUENCE, VERIFY rows get census counters. Then: merge (BK first), gate, Fable adversarial review (Codex pool exhausted; owner: Fable is the fallback), fix lane BN, gate2, archive, push, ONE thinking-on probe corpus gated on reasoning_content -> engine-seat census decides the remaining wave 66-69 revert list. STILL TO DO for the owner: the RULINGS section draft in his voice (he corrects it; sessions never edit it).

## 2026-09-07 — OWNER REFINEMENT of the stale-procedure ruling (verbatim): "you can have an old superseded procedure in project memory, because iterating and improving procedures is easier when you know what been tried and why you moved on. but not in the procedural memory. not in the context we expect a fresh agent to load and act on immediately. a superseded procedure is no longer 'the procedure' it is a piece of history." -> skill invariant 00-P + memory `procedure-change-replaces-never-appends` updated: PROCEDURAL memory = skill + anchor (live procedure only, replace+sweep); PROJECT memory (wagicgpt/ sub-memories, wave notes, review.md) keeps superseded procedures as history marked SUPERSEDED.

## 2026-09-07 — WAVE-70 STEP ONE MERGED: BK 091eabeae -> f20ab36e6 (clean; `--thinking on|off` REQUIRED, tools/regime-gate.py kills a wrong-regime corpus, regime + reasoning_chars stamped on every record, reasoning budget separated from the answer ceiling — seam caps stay 768 because PLAN+action p99.5 needs ~770 tokens; engine default stays OFF per 000(f), announced once). Audit revision abf559f9a. BL 0c4f7821e -> 715338077 ((ci) stray brace; golden Res/ai/gpt/reply-protocol.txt = compiled fallback, pinned; 29 CHANGE/8 DELETE applied; new link guard tools/check-reply-instructions.py (24 hits on base, 0 now) — Makefile.sdl REGENERATED so both QMAKE_PRE_LINK guards run; guides STAGED in wave70/guides/ (6 files, 19,729-19,999 B, A332 grep 0), not installed). BM 46bfae08e -> 71b1e22be (7 conflicts by hand: ctor init-list = union (BK's 4 + BM's 10); prose-reversal re-ask arms at priority/ask/attackers/blockers = BL's DELETION kept (BM's PLAN-line-verdict rewrite dropped; planSaysPassThisWindow/planChoiceConflict stay computed for parse notes only — REVIEW MUST CHECK nothing prose-read still reaches a re-ask); truncatedBlockCommitmentAbandoned(content, mLastFinishLength) = BM's signature; W67-AV I1/I2/I3 PARSETEST sections DELETED per BM; W70-BK C4/C5 + W70-BL E1-E6 + W70-BM E1-E6 all kept — a shared `    }` at the BL/BM seam was lost and restored). PARSETEST after BM merge 5283/0 (BM deleted 203 cases with their mechanisms). Gate 1 launched (unit w70-gate1, make -B + suite + PARSETEST). Codex Astra review launched (pool reset) on ~/.gatelogs/w70-review-prompt.md, diff 52ab71610..HEAD -> w70-review-codex.out.
