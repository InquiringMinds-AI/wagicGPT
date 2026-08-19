#!/usr/bin/env bash
# Windows card-art pack.
# Desktop keeps the pool image untouched (no recompress generation loss);
# thumbs are the desktop convention's exact 114x166.
PLATFORM=windows
FULL_GEOM="${FULL_GEOM:-copy}"       FULL_Q="${FULL_Q:-0}"
THUMB_GEOM="${THUMB_GEOM:-114x166}"  THUMB_Q="${THUMB_Q:-85}"
source "$(dirname "$0")/pack-common.sh"
