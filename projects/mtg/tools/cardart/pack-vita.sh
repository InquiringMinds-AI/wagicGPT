#!/usr/bin/env bash
# PS Vita card-art pack.
# Pilot-measured geometry (set 10E: 61 MB source -> 32 MB packed); thumbs match
# the desktop pack's fixed-aspect convention scaled up for the Vita screen.
PLATFORM=vita
FULL_GEOM="${FULL_GEOM:-360x514}"    FULL_Q="${FULL_Q:-88}"
THUMB_GEOM="${THUMB_GEOM:-128x186}"  THUMB_Q="${THUMB_Q:-85}"
source "$(dirname "$0")/pack-common.sh"
