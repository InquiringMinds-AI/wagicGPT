#!/usr/bin/env bash
# Splice repaired card images into the already-built Vita/PSP card packs.
#
# make-vita-cards.sh SKIPS a set whose zip exists, so after fix-dfc-backface-art.py
# rewrites a handful of back-face JPEGs the built packs are stale.  Rebuilding
# 336 sets from 12 GB of source to change 207 files is absurd; the zips are
# STORED, so `zip -0 -X` replaces the individual entries in place and every
# other byte of the pack (and every other set) is untouched.
#
# Geometry MUST match make-vita-cards.sh exactly or the replaced cards render
# at a different size than their neighbours.
#
# Usage: patch-vita-cardpack.sh <file-with "SET ID" lines>
set -uo pipefail

SRC="${SRC:-/home/magi/Projects/wagicGPT/projects/mtg/bin/User/sets/sets.hidden}"
OUT="${OUT:-/home/magi/Projects/wagicGPT/psp-work/card-packs/vita}"
WORK="${WORK:-/home/magi/Projects/wagicGPT/psp-work/card-packs/.patchwork}"
FULL_GEOM="${FULL_GEOM:-360x514}"; FULL_Q="${FULL_Q:-88}"
THUMB_GEOM="${THUMB_GEOM:-128x186}"; THUMB_Q="${THUMB_Q:-85}"

LIST="${1:?usage: patch-vita-cardpack.sh <list>}"
rm -rf "$WORK"; mkdir -p "$WORK"
patched=0; skipped=0

while read -r set id; do
    [ -z "${set:-}" ] && continue
    zip="$OUT/sets/$set/$set.zip"
    src="$SRC/$set/$id.jpg"
    if [ ! -f "$zip" ] || [ ! -f "$src" ]; then
        echo "  SKIP $set/$id (no zip or no source)"; skipped=$((skipped+1)); continue
    fi
    w="$WORK/$set"; mkdir -p "$w/thumbnails"
    magick "$src" -strip -resize "$FULL_GEOM" -quality "$FULL_Q" "$w/$id.jpg" 2>/dev/null || \
        { echo "  BAD  $set/$id"; skipped=$((skipped+1)); continue; }
    magick "$src" -strip -resize "${THUMB_GEOM}!" -quality "$THUMB_Q" "$w/thumbnails/$id.jpg" 2>/dev/null || true
    ( cd "$w" && zip -q -0 -X "$zip" "$id.jpg" "thumbnails/$id.jpg" ) && patched=$((patched+1))
done < "$LIST"

echo "patched entries: $patched   skipped: $skipped"
rm -rf "$WORK"
