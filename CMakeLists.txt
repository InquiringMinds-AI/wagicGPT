# CMake 4.x removed compatibility with minimums below 3.5. The range form keeps
# policy behaviour pinned to what this file was written against while still
# configuring under a modern CMake.
cmake_minimum_required(VERSION 3.10...3.28)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(wagic C CXX)
include("${VITASDK}/share/vita.cmake" REQUIRED)

# Project-local vitaGL, built from CURRENT upstream.
#
# The inherited port pinned this to d4a8f9d (Jan 10 2026) because a newer
# vitaGL "silently failed to boot", citing a rename cascade
# (`frag_buf`->`vgl_def_frag_buf`, `mvp_matrix`->`vgl_mvp_matrix`,
# `glVertexAttribDivisor`). Owner ruling 2026-08-01: track current instead and
# fix what actually breaks - newer vitaGL carries real improvements, the target
# is a single fixed hardware profile, and the inherited port is prior art, not
# a truth document.
#
# That pin is also weakly motivated on inspection: wagic calls only
# vglSetParamBufferSize, vglUseTripleBuffering, vglUseCachedMem,
# vglInitExtended, vglUseVram, vglWaitVblankStart and vglSwapBuffers. The named
# renames are vitaGL-internal; `u_mvp_matrix` is wagic's own shader uniform.
#
# Built locally (vendor/, gitignored) rather than using the vdpm-installed
# system copy so the version is ours to control and rebuild:
#   cd vendor/vitaGL && make NO_DEBUG=1 -j8
# If a boot failure ever IS traced to a vitaGL bump, bisect vendor/vitaGL and
# record the offending commit here - do not silently re-pin to 2026.
set(WAGIC_PINNED_VITAGL "${CMAKE_SOURCE_DIR}/vendor/vitaGL")
if(EXISTS "${WAGIC_PINNED_VITAGL}/libvitaGL.a")
    message(STATUS "Using project-local pinned vitaGL at ${WAGIC_PINNED_VITAGL}")
    include_directories(BEFORE "${WAGIC_PINNED_VITAGL}/source")
    link_directories(BEFORE "${WAGIC_PINNED_VITAGL}")
else()
    message(WARNING "vendor/vitaGL/libvitaGL.a not found, falling back to system vitaGL "
                    "(may produce a binary that crashes at boot). "
                    "Run vendor/vitaGL setup per BUILD_VITA.md.")
endif()

set(VITA_APP_NAME "wagicGPT")
# Own title id, not the inherited port's WAGC00001. The title id owns the
# ux0:app/<id> directory and the LiveArea slot, so sharing one would install
# this fork OVER a user's existing Wagic instead of beside it. Save data is
# unaffected either way - it lives at ux0:data/Wagic, which both builds share.
#
# 42: the answer from a thinking machine, which is what this fork runs on.
set(VITA_TITLEID  "WAGC00042")
set(VITA_VERSION  "01.00")

# -g and a kept frame pointer are for CRASH FORENSICS, not for debugging builds.
# The Vita writes a psp2core dump on every crash, but a dump is only worth having
# if it can be read: without -g, addr2line falls back to nearest-symbol across a
# 48k-symbol static binary and confidently reports the wrong function (a real
# 2026-08-02 dump "resolved" its stack to curl, SDL and GLSL frames). Without a
# frame pointer the stack walk is guesswork on top of that.
#
# Neither flag changes codegen at -O2, but -g is NOT free on this platform: the
# debug sections DO travel into the self unless it is stripped, and measured here
# that took the self from 34 MB to 141 MB and the VPK from 40 MB to 76 MB. The
# Vita loads the executable into memory, so shipping that would trade a
# diagnosable crash for a memory problem. vita_create_self(... STRIPPED) below
# passes -s to vita-elf-create, so the ELF in build_vita keeps its debug info for
# addr2line while the console gets a stripped binary. Stripping does not move
# addresses, so a dump from the shipped build symbolizes correctly against the
# unstripped ELF sitting next to it.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fno-omit-frame-pointer -fexceptions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -fno-omit-frame-pointer -std=c++14 -fexceptions")

add_definitions(
  -DVITA
  -DLINUX
  -DSDL_CONFIG
  -DTIXML_USE_STL
  -DFORCE_GLES
  -DGL_OES_VERSION_1_1
  # The LLM opponent. Without this the whole GPT layer preprocesses away and
  # the build is Baka-only, which is not a shippable configuration for this
  # project. The transport is libcurl, which VitaSDK already ships built
  # against OpenSSL - so TLS is done in userspace and does not depend on the
  # console's own SSL module.
  -DWITH_GPT_AI
)

include_directories(
  ${CMAKE_SOURCE_DIR}/JGE/include
  ${CMAKE_SOURCE_DIR}/JGE/src/zipFS
  ${CMAKE_SOURCE_DIR}/projects/mtg/include
  ${CMAKE_SOURCE_DIR}/Boost
  $ENV{VITASDK}/arm-vita-eabi/include/SDL2
)

# JGE engine sources
set(JGE_SOURCES
  JGE/src/JApp.cpp
  JGE/src/JGE.cpp
  JGE/src/JGui.cpp
  JGE/src/JLBFont.cpp
  JGE/src/JLogger.cpp
  JGE/src/JGameObject.cpp
  JGE/src/JFileSystem.cpp
  JGE/src/JSprite.cpp
  JGE/src/Vector2D.cpp
  JGE/src/Encoding.cpp
  JGE/src/tinyxml/tinystr.cpp
  JGE/src/tinyxml/tinyxml.cpp
  JGE/src/tinyxml/tinyxmlerror.cpp
  JGE/src/tinyxml/tinyxmlparser.cpp
  JGE/src/hge/hgecolor.cpp
  JGE/src/hge/hgedistort.cpp
  JGE/src/hge/hgefont.cpp
  JGE/src/hge/hgeparticle.cpp
  JGE/src/hge/hgerect.cpp
  JGE/src/hge/hgevector.cpp
  JGE/src/zipFS/zfsystem.cpp
  JGE/src/zipFS/ziphdr.cpp
  JGE/src/zipFS/zstream.cpp
  # PC/OpenGL renderer + sound (shared with Linux/Win/Mac)
  JGE/src/pc/JGfx.cpp
  JGE/src/pc/JSfx.cpp
  # Vita entry point
  JGE/src/Vitamain.cpp
)

# MTG game sources
set(MTG_SOURCES
  projects/mtg/src/AbilityParser.cpp
  projects/mtg/src/ActionElement.cpp
  projects/mtg/src/ActionLayer.cpp
  projects/mtg/src/ActionStack.cpp
  projects/mtg/src/AIHints.cpp
  projects/mtg/src/AIMomirPlayer.cpp
  projects/mtg/src/AIPlayer.cpp
  projects/mtg/src/AIPlayerBaka.cpp
  projects/mtg/src/AIStats.cpp
  projects/mtg/src/AllAbilities.cpp
  projects/mtg/src/CardDescriptor.cpp
  projects/mtg/src/CardDisplay.cpp
  projects/mtg/src/CardGui.cpp
  projects/mtg/src/CardPrimitive.cpp
  projects/mtg/src/CardSelector.cpp
  projects/mtg/src/CarouselDeckView.cpp
  projects/mtg/src/Closest.cpp
  projects/mtg/src/Counters.cpp
  projects/mtg/src/Credits.cpp
  projects/mtg/src/Damage.cpp
  projects/mtg/src/DamagerDamaged.cpp
  projects/mtg/src/DeckDataWrapper.cpp
  projects/mtg/src/DeckEditorMenu.cpp
  projects/mtg/src/DeckManager.cpp
  projects/mtg/src/DeckMenu.cpp
  projects/mtg/src/DeckMenuItem.cpp
  projects/mtg/src/DeckMetaData.cpp
  projects/mtg/src/DeckStats.cpp
  projects/mtg/src/DeckView.cpp
  projects/mtg/src/DuelLayers.cpp
  projects/mtg/src/ExtraCost.cpp
  projects/mtg/src/GameApp.cpp
  projects/mtg/src/GameLauncher.cpp
  projects/mtg/src/GameObserver.cpp
  projects/mtg/src/GameOptions.cpp
  projects/mtg/src/GameStateAwards.cpp
  projects/mtg/src/GameState.cpp
  projects/mtg/src/GameStateDeckViewer.cpp
  projects/mtg/src/GameStateDuel.cpp
  projects/mtg/src/GameStateMenu.cpp
  projects/mtg/src/GameStateOptions.cpp
  projects/mtg/src/GameStateShop.cpp
  projects/mtg/src/GameStateStory.cpp
  projects/mtg/src/GameStateTransitions.cpp
  projects/mtg/src/GridDeckView.cpp
  projects/mtg/src/GuiAvatars.cpp
  projects/mtg/src/GuiBackground.cpp
  projects/mtg/src/GuiCombat.cpp
  projects/mtg/src/GuiHand.cpp
  projects/mtg/src/GuiLayers.cpp
  projects/mtg/src/GuiMana.cpp
  projects/mtg/src/GuiPhaseBar.cpp
  projects/mtg/src/GuiPlay.cpp
  projects/mtg/src/GuiStatic.cpp
  projects/mtg/src/IconButton.cpp
  projects/mtg/src/InteractiveButton.cpp
  projects/mtg/src/ManaCost.cpp
  projects/mtg/src/ManaCostHybrid.cpp
  projects/mtg/src/MenuItem.cpp
  projects/mtg/src/ModRules.cpp
  projects/mtg/src/MTGAbility.cpp
  projects/mtg/src/MTGCard.cpp
  projects/mtg/src/MTGCardInstance.cpp
  projects/mtg/src/MTGDeck.cpp
  projects/mtg/src/MTGDefinitions.cpp
  projects/mtg/src/MTGGamePhase.cpp
  projects/mtg/src/MTGGameZones.cpp
  projects/mtg/src/MTGPack.cpp
  projects/mtg/src/MTGRules.cpp
  projects/mtg/src/ObjectAnalytics.cpp
  projects/mtg/src/OptionItem.cpp
  projects/mtg/src/PhaseRing.cpp
  projects/mtg/src/Player.cpp
  projects/mtg/src/PlayerData.cpp
  projects/mtg/src/PrecompiledHeader.cpp
  projects/mtg/src/PlayGuiObject.cpp
  projects/mtg/src/PlayGuiObjectController.cpp
  projects/mtg/src/PlayRestrictions.cpp
  projects/mtg/src/Pos.cpp
  projects/mtg/src/PriceList.cpp
  projects/mtg/src/ReplacementEffects.cpp
  projects/mtg/src/Rules.cpp
  projects/mtg/src/SimpleButton.cpp
  projects/mtg/src/SimpleMenu.cpp
  projects/mtg/src/SimpleMenuItem.cpp
  projects/mtg/src/SimplePad.cpp
  projects/mtg/src/SimplePopup.cpp
  projects/mtg/src/StoryFlow.cpp
  projects/mtg/src/Subtypes.cpp
  projects/mtg/src/StyleManager.cpp
  projects/mtg/src/TargetChooser.cpp
  projects/mtg/src/TargetsList.cpp
  projects/mtg/src/Tasks.cpp
  projects/mtg/src/TestSuiteAI.cpp
  projects/mtg/src/TextScroller.cpp
  projects/mtg/src/ThisDescriptor.cpp
  projects/mtg/src/Token.cpp
  projects/mtg/src/Translate.cpp
  projects/mtg/src/TranslateKeys.cpp
  projects/mtg/src/Trash.cpp
  projects/mtg/src/utils.cpp
  projects/mtg/src/WCachedResource.cpp
  projects/mtg/src/WDataSrc.cpp
  projects/mtg/src/WEvent.cpp
  projects/mtg/src/WFilter.cpp
  projects/mtg/src/WFont.cpp
  projects/mtg/src/WGui.cpp
  projects/mtg/src/WParsedInt.cpp
  projects/mtg/src/WResourceManager.cpp

  # This fork's engine layer, absent from the original Vita port because that
  # port predates it: engine-owned priority and legality (LegalActions),
  # player-agnostic mana (ManaEngine), the frontend-agnostic
  # DecisionRequest->Action contract (DecisionContract) that the PC UI, the PSP
  # UI and the AI policies all consume as peers, and the pregame/mulligan state.
  projects/mtg/src/LegalActions.cpp
  projects/mtg/src/ManaEngine.cpp
  projects/mtg/src/DecisionContract.cpp
  projects/mtg/src/PreGamePhase.cpp

  # The LLM opponent. Compiled here for the same reason the PSP Makefile
  # compiles it: AIPlayerGPT is wired into the player/observer types, and the
  # heuristic AI answers every decision it declines, so a build with no endpoint
  # configured plays as pure Baka. The transport (JNetwork/JSocket) is
  # deliberately NOT built - same open question as the PSP network leg.
  projects/mtg/src/AIPlayerGPT.cpp
  projects/mtg/src/GptConfig.cpp
  projects/mtg/src/qrcodegen.cpp
  projects/mtg/src/OptionGpt.cpp
)

# ALPHA ONLY: compile the WAGIC_FIZZLELOG spell-resolution diagnostic into the
# Vita build so an invisible fizzle self-diagnoses on the owner's console -
# lines land in ux0:data/Wagic/fizzlelog.txt, readable over FTP after the
# session (Vitamain.cpp already creates ux0:data/Wagic at startup). Diagnostics
# do not belong in release builds - REMOVE this block for any release VPK.
#
# Scoped to THIS ONE SOURCE on purpose. WAGIC_DEVLOGS is a wide gate: defining
# it for the whole target would also switch on JGfx's thinlog, which adds a
# per-draw-call function call plus a getenv() to every quad, rect and line the
# console paints. The Android alpha can afford that; a 480x272 handheld chasing
# 60fps should not pay it for a diagnostic that lives in one function.
# ALPHA-ONLY - REMOVE FOR RELEASE. Dedicated define (like WAGIC_VITA_FBDUMP): the
# per-file WAGIC_DEVLOGS property produced no fizzlelog.txt on device (suspected
# inert for this generator), and a GLOBAL WAGIC_DEVLOGS would switch on JGfx's
# per-quad thinlog. WAGIC_FIZZLELOG is referenced only by MTGAbility.cpp's gates.
add_definitions(-DWAGIC_FIZZLELOG)
# Per-turn heap + vitaGL pool probe -> ux0:data/Wagic/memlog.txt (Vitamain.cpp
# vitaMemProbe; hooks in GameObserver/GameStateDuel). Chasing the 2026-08-30..09-01
# bad_alloc-in-LoadPNG crash cluster. ALPHA-ONLY - REMOVE FOR RELEASE.
add_definitions(-DWAGIC_VITAMEMLOG)
# Match transcripts -> ux0:data/Wagic/transcripts/<time>-<deck>-vs-<deck>.txt
# (replayable on desktop with WAGIC_REPLAY=<file>) + the post-match
# classification menu. ALPHA-ONLY - REMOVE FOR RELEASE.
add_definitions(-DWAGIC_TRANSCRIPT)

add_executable(${PROJECT_NAME}
  ${JGE_SOURCES}
  ${MTG_SOURCES}
)

# NOTE ON std::thread: it does NOT work in this build, and the fix is not a
# link flag. Forcing the pthread proxy symbols in with -Wl,-u DID flip
# libstdc++'s __gthread_active_p() to true - and that CRASHED THE GAME ON
# LAUNCH, in __cxa_guard_acquire (the guard around the first function-local
# static), locking a mutex that was never constructed. -u pulls in only the
# named archive members, so the member holding pthread-embedded's global state
# never came along: a half-linked pthread runtime, which is strictly worse than
# none. Hardware evidence: psp2core 1785640291, data abort in
# pthread_mutex_lock via __cxa_guard_acquire, debug.txt stopping right after
# "[3e] network up".
# Whole-archiving libpthread might resolve it, but the robust answer for the
# async model-call worker is a native sceKernelCreateThread implementation that
# does not depend on libstdc++'s threading model at all. Until then std::thread
# construction throws, AIPlayerGPT catches it, and the heuristic AI answers.
target_link_libraries(${PROJECT_NAME}
  SDL2_mixer
  SDL2
  vitaGL
  vitashark
  SceShaccCgExt
  SceShaccCg_stub
  taihen_stub
  mathneon
  modplug
  opusfile
  opus
  xmp-lite
  vorbisfile
  vorbis
  ogg
  mikmod
  FLAC
  mpg123
  freetype
  png
  jpeg
  # HTTP transport for the LLM opponent. Link order matters for static libs:
  # curl before its own dependencies. Taken from the SDK's libcurl.pc.
  curl
  ssl
  crypto
  zstd
  z
  m
  pthread
  stdc++
  SceDisplay_stub
  SceCtrl_stub
  SceAudio_stub
  SceAudioIn_stub
  SceSysmodule_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceAppMgr_stub
  SceTouch_stub
  SceHid_stub
  SceMotion_stub
  ScePower_stub
  SceAppUtil_stub
  SceKernelDmacMgr_stub
  SceKernelThreadMgr_stub
  SceIofilemgr_stub
  # Network stubs required by libcurl (SDK libcurl.pc) and by the sceNet /
  # sceNetCtl bring-up in Vitamain.cpp.
  SceNet_stub
  SceNetCtl_stub
  SceRtc_stub
  SceSysmem_stub
  SceKernelModulemgr_stub
  SceProcessmgr_stub
  SceLibKernel_stub
)

# UNSAFE: skips the -s flag passed to vita-make-fself. Without UNSAFE, the
# macro generates a "safe" eboot.bin with AUTHID 0x2F00000000000002, which
# this Vita's CFW silently rejects at boot. UNSAFE produces AUTHID
# 0x2F00000000000001 (the format Feb 28's working VPK used).
#
# UNCOMPRESSED also matches the Feb 28 working format (self ~ velf size).
# Belt-and-suspenders alongside the project-pinned vitaGL above.
vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME} UNCOMPRESSED UNSAFE STRIPPED)

# Card images and VPK packaging.
#
# All game resources go into a single Res/core.zip archive inside the VPK.
# The filesystem constructor (zfsystem.cpp) auto-discovers *.zip files in the
# Res/ root and indexes their contents, so openForRead(), FileExists() and
# scanfolder() all find files from core.zip transparently. No runtime code
# changes are needed.
#
# core.zip uses STORE mode (-0) for direct seek+read. This replaces ~1,400
# loose files plus 370 directories with one file and makes VPK install nearly
# instant on Vita.
#
# Card art (when provided) is embedded as per-set zip archives inside core.zip,
# e.g. sets/10E/10E.zip. Per-set zips also use STORE mode. The zip-in-zip
# reader silently skips compressed entries (zfsystem.cpp PreloadZip), so STORE
# is required. cardFile() already checks per-set zips before loose files.
#
# Configure with:
#   cmake ... -DCARD_IMAGES_DIR=/path/to/card_images
#
# Build targets:
#   make vpk            lightweight VPK, no card art
#   make vpk-full       full VPK, card art included, incremental
#   make card-images    build the card image archive only, reused next time
#
# Incremental: card images are archived once into wagic-card-images.zip and
# reused. Only eboot.bin is repacked on later "make vpk-full" runs. To force
# a rebuild: rm wagic-card-images.zip && make vpk-full
set(CARD_IMAGES_DIR "" CACHE PATH
  "Directory containing card art. Expected layout: <dir>/sets/<SETCODE>/<MTGID>.jpg")

# Helper: shared VPK base (eboot + sce_sys + Res/core.zip).
set(VPK_STAGE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/stage_vpk.sh")
file(WRITE ${VPK_STAGE_SCRIPT}
"#!/bin/bash
set -e
export PATH=\"/usr/local/vitasdk/bin:$PATH\"
BUILDDIR=\"$1\"
SRCDIR=\"$2\"
OUTNAME=\"$3\"

cd \"$BUILDDIR\"

# Generate param.sfo
vita-mksfoex -s TITLE_ID=${VITA_TITLEID} -s APP_VER=01.00 -s VERSION=01.00 -d ATTRIBUTE2=12 \"${VITA_APP_NAME}\" param.sfo

# Stage Res/core.zip: bundle ALL game resources into a single STORED archive.
# The filesystem constructor (zfsystem.cpp) auto-discovers *.zip files in the
# Res/ directory and indexes their contents into its virtual file map.
# All file operations (openForRead, FileExists, scanfolder) then find files
# transparently from the zip. No runtime code changes needed.
# STORE mode (-0) guarantees compatibility (direct seek+read, no decompression).
# This replaces ~1,400 loose files + 370 directories with a SINGLE file.
#
# Exclude directories not needed at runtime:
#   test/                  - compile-time test suite only (#ifdef TESTSUITE)
#   missing_cards_by_sets/ - dev metadata, not referenced in game code
#   oldthemes/             - deprecated, not referenced in game code
rm -rf vpk_staging
mkdir -p vpk_staging/Res
\"$SRCDIR/projects/mtg/tools/make-respack.py\" --compress stored --no-sha \\
  -o \"$BUILDDIR/vpk_staging/Res/core.zip\"

CORESIZE=\$(du -h \"$BUILDDIR/vpk_staging/Res/core.zip\" | cut -f1)
echo \"Res/core.zip created: \$CORESIZE\"

# Create VPK using vita-pack-vpk (proper VPK format, not raw zip)
vita-pack-vpk -s param.sfo -b ${PROJECT_NAME}.self \\
  -a \"$SRCDIR/vita/sce_sys/icon0.png\"=sce_sys/icon0.png \\
  -a \"$SRCDIR/vita/sce_sys/livearea/contents/bg0.png\"=sce_sys/livearea/contents/bg0.png \\
  -a \"$SRCDIR/vita/sce_sys/livearea/contents/startup.png\"=sce_sys/livearea/contents/startup.png \\
  -a \"$SRCDIR/vita/sce_sys/livearea/contents/template.xml\"=sce_sys/livearea/contents/template.xml \\
  -a \"$BUILDDIR/vpk_staging/Res/core.zip\"=Res/core.zip \\
  \"$BUILDDIR/$OUTNAME\"
")

# Target: vpk (lightweight, no card art).
add_custom_command(
  OUTPUT ${PROJECT_NAME}.vpk
  DEPENDS ${PROJECT_NAME}.self
  COMMAND bash "${VPK_STAGE_SCRIPT}"
    "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_SOURCE_DIR}" "${PROJECT_NAME}.vpk"
  COMMENT "Packaging ${PROJECT_NAME}.vpk (no card art)..."
)
add_custom_target(vpk ALL DEPENDS ${PROJECT_NAME}.vpk)

# Helper: build the card image archive (built once, reused across rebuilds).
# Creates per-set zip archives (e.g. Res/sets/10E/10E.zip) using STORE mode.
# STORE (-0) is required because zfsystem.cpp silently skips compressed
# entries. This collapses ~75,000 loose files into ~336 zips, which cuts
# VPK install time on Vita dramatically.
set(VPK_BUILD_IMAGES_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/build_card_images.sh")
file(WRITE ${VPK_BUILD_IMAGES_SCRIPT}
"#!/bin/bash
set -e
OUTZIP=\"$1\"
IMGDIR=\"$2\"

if [ -z \"$IMGDIR\" ]; then
  echo 'ERROR: CARD_IMAGES_DIR is not set. Re-run cmake with -DCARD_IMAGES_DIR=/path/to/images'
  exit 1
fi
if [ ! -d \"$IMGDIR/sets\" ]; then
  echo \"ERROR: $IMGDIR/sets/ does not exist. Expected layout: <dir>/sets/<SETCODE>/<MTGID>.jpg\"
  exit 1
fi

# Skip if archive already exists (incremental build)
if [ -f \"$OUTZIP\" ]; then
  SIZE=$(du -h \"$OUTZIP\" | cut -f1)
  echo \"Card images archive exists: $OUTZIP ($SIZE), skipping rebuild.\"
  echo \"  To force rebuild: rm $OUTZIP && make vpk-full\"
  exit 0
fi

echo \"Building card images archive (per-set zips): $OUTZIP\"
echo \"  Source: $IMGDIR\"

TMPSTAGE=$(mktemp -d)
trap 'rm -rf \"$TMPSTAGE\"' EXIT

SETCOUNT=0
TOTALFILES=0

for SETDIR in \"$IMGDIR/sets\"/*/; do
  [ -d \"$SETDIR\" ] || continue
  SETCODE=$(basename \"$SETDIR\")

  # Count image files
  NJPG=$(find \"$SETDIR\" -maxdepth 1 \\( -name '*.jpg' -o -name '*.png' \\) | wc -l)
  [ \"$NJPG\" -eq 0 ] && continue

  # Per-set zip in STORE mode (required by JFileSystem/zfsystem).
  # Bare filenames (no subdir prefix) so cardFile() can find them.
  mkdir -p \"$TMPSTAGE/Res/sets/$SETCODE\"
  (cd \"$SETDIR\" && zip -0 -q \"$TMPSTAGE/Res/sets/$SETCODE/$SETCODE.zip\" *.jpg *.png 2>/dev/null || true)

  SETCOUNT=$((SETCOUNT + 1))
  TOTALFILES=$((TOTALFILES + NJPG))

  if [ $((SETCOUNT % 50)) -eq 0 ]; then
    echo \"  Processed $SETCOUNT sets ($TOTALFILES files)...\"
  fi
done

echo \"  Created $SETCOUNT per-set zips ($TOTALFILES total card images)\"

# Bundle all per-set zips into a single archive for the build system
cd \"$TMPSTAGE\"
zip -r -0 -q \"$OUTZIP\" Res/sets/ -i '*.zip'

SIZE=$(du -h \"$OUTZIP\" | cut -f1)
echo \"Card images archive complete: $OUTZIP ($SIZE)\"
echo \"  ($SETCOUNT sets, $TOTALFILES card images as per-set stored zips)\"
")

# Helper: merge base VPK with card image archive.
set(VPK_MERGE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/merge_vpk_full.sh")
file(WRITE ${VPK_MERGE_SCRIPT}
"#!/bin/bash
set -e
export PATH=\"/usr/local/vitasdk/bin:$PATH\"
BUILDDIR=\"$1\"
SRCDIR=\"$2\"
OUTNAME=\"$3\"
IMGZIP=\"$4\"

# Step 1: Build base VPK (eboot + sce_sys + Res/core.zip)
bash \"$BUILDDIR/stage_vpk.sh\" \"$BUILDDIR\" \"$SRCDIR\" \"$OUTNAME\"

# Step 2: Inject per-set card zips into core.zip
if [ ! -f \"$IMGZIP\" ]; then
  echo \"ERROR: Card images archive not found: $IMGZIP\"
  echo \"  Run: make card-images\"
  exit 1
fi

echo \"Adding card images to core.zip...\"
TMPCARD=\$(mktemp -d)
trap 'rm -rf \"\$TMPCARD\"' EXIT

# Extract per-set zips from card images archive
# Archive contains: Res/sets/10E/10E.zip, Res/sets/5DN/5DN.zip, etc.
unzip -q \"$IMGZIP\" -d \"\$TMPCARD\"

# Add per-set zips into core.zip in STORE mode (required for zip-in-zip reading).
# Entries must not have a Res/ prefix because the filesystem root is Res/.
(cd \"\$TMPCARD/Res\" && \\
 zip -0 -q \"$BUILDDIR/vpk_staging/Res/core.zip\" sets/*/*.zip)

CORESIZE=\$(du -h \"$BUILDDIR/vpk_staging/Res/core.zip\" | cut -f1)
echo \"Res/core.zip with card images: \$CORESIZE\"

# Step 3: Rebuild VPK with enlarged core.zip using vita-pack-vpk
cd \"$BUILDDIR\"
vita-pack-vpk -s param.sfo -b ${PROJECT_NAME}.self \\
  -a \"$SRCDIR/vita/sce_sys/icon0.png\"=sce_sys/icon0.png \\
  -a \"$SRCDIR/vita/sce_sys/livearea/contents/bg0.png\"=sce_sys/livearea/contents/bg0.png \\
  -a \"$SRCDIR/vita/sce_sys/livearea/contents/startup.png\"=sce_sys/livearea/contents/startup.png \\
  -a \"$SRCDIR/vita/sce_sys/livearea/contents/template.xml\"=sce_sys/livearea/contents/template.xml \\
  -a \"$BUILDDIR/vpk_staging/Res/core.zip\"=Res/core.zip \\
  \"$BUILDDIR/$OUTNAME\"

FULLSIZE=\$(du -h \"$BUILDDIR/$OUTNAME\" | cut -f1)
echo \"vpk-full complete: $BUILDDIR/$OUTNAME (\$FULLSIZE)\"
")

# Target: card-images (build the archive once).
set(CARD_IMAGES_ARCHIVE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-card-images.zip")
add_custom_command(
  OUTPUT ${PROJECT_NAME}-card-images.zip
  COMMAND bash "${VPK_BUILD_IMAGES_SCRIPT}"
    "${CARD_IMAGES_ARCHIVE}" "${CARD_IMAGES_DIR}"
  COMMENT "Building card images archive (one-time)..."
)
add_custom_target(card-images DEPENDS ${PROJECT_NAME}-card-images.zip)

# Target: vpk-full (with card art, incremental).
add_custom_command(
  OUTPUT ${PROJECT_NAME}-full.vpk
  DEPENDS ${PROJECT_NAME}.self ${PROJECT_NAME}-card-images.zip
  COMMAND bash "${VPK_MERGE_SCRIPT}"
    "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_SOURCE_DIR}"
    "${PROJECT_NAME}-full.vpk" "${CARD_IMAGES_ARCHIVE}"
  COMMENT "Packaging ${PROJECT_NAME}-full.vpk (incremental)..."
)
add_custom_target(vpk-full DEPENDS ${PROJECT_NAME}-full.vpk)
