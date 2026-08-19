#!/usr/bin/env bash
# Android card-art pack.
# Tablet screens out-resolve the Vita and storage is roomy, so full art keeps
# Scryfall's "normal" pixel budget (488x680 — about 2x the Vita pack's area);
# thumbs use the desktop pack's 114x166 fixed-aspect convention. No pow2
# constraint on this GLES path. Tune with FULL_GEOM/FULL_Q if pack size on
# the device becomes a concern.
PLATFORM=android
FULL_GEOM="${FULL_GEOM:-488x680}"    FULL_Q="${FULL_Q:-88}"
THUMB_GEOM="${THUMB_GEOM:-114x166}"  THUMB_Q="${THUMB_Q:-85}"
source "$(dirname "$0")/pack-common.sh"
