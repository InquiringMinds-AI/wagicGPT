#!/usr/bin/env bash
# PSP card-art pack.
#
# The PSP allocates textures at POWER-OF-TWO dimensions on BOTH axes
# (JGE/src/JGfx.cpp: getNextPower2 on each axis; size = w*h*4 bytes), so cost
# is decided by which pow2 bucket each axis lands in, not by pixel count —
# one pixel over a boundary doubles it. 180x250 keeps every source aspect
# ratio inside 256x256 = 256 KB; exact 64x93 thumbs land in 64x128 = 32 KB.
# Full derivation in psp-work/make-psp-cards.sh (the script this supersedes).
PLATFORM=psp
FULL_GEOM="${FULL_GEOM:-180x250}"   FULL_Q="${FULL_Q:-85}"
THUMB_GEOM="${THUMB_GEOM:-64x93}"   THUMB_Q="${THUMB_Q:-85}"
JOBS="${JOBS:-8}"
source "$(dirname "$0")/pack-common.sh"
