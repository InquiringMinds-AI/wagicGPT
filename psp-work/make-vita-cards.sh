#!/usr/bin/env bash
# Build a PS Vita card-image pack from the full-resolution desktop set.
#
# Source : projects/mtg/bin/User/sets/<SET>/<id>.jpg  (+ thumbnails/<id>.jpg)
# Output : <OUT>/sets/<SET>/<SET>.zip, STORED (already-compressed JPEG), with
#          in-zip keys "<id>.jpg" and "thumbnails/<id>.jpg" -- exactly what
#          WResourceManager::cardFile looks for once AttachZipFile picks up
#          sets/<SET>/<SET>.zip.  The user path is searched before the system
#          path, so this drops into ux0:data/Wagic/ without an engine change.
#
# Thumbnails are ALWAYS derived from the full-size art, never rescaled from the
# shipped 114x166 desktop thumbnail -- downscaling once from 618x882 beats
# resampling an already-lossy small JPEG, and it means every card gets a thumb
# regardless of whether the source pack shipped one.
#
# WOTC-derived scans: local + his devices only.  Never committed, never in a
# release artifact.
set -uo pipefail

SRC="${SRC:-/home/magi/Projects/wagicGPT/projects/mtg/bin/User/sets}"
OUT="${OUT:-/home/magi/Projects/wagicGPT/psp-work/card-packs/vita}"
WORK="${WORK:-/home/magi/Projects/wagicGPT/psp-work/card-packs/.work}"
JOBS="${JOBS:-6}"

# Pilot-measured geometry (set 10E: 61 MB source -> 32 MB packed).
FULL_GEOM="${FULL_GEOM:-360x514}"
FULL_Q="${FULL_Q:-88}"
THUMB_GEOM="${THUMB_GEOM:-128x186}"
THUMB_Q="${THUMB_Q:-85}"

mkdir -p "$OUT/sets" "$WORK"
LOG="$OUT/build.log"

convert_set() {
    local set_dir="$1" set_name
    set_name="$(basename "$set_dir")"
    local zip="$OUT/sets/$set_name/$set_name.zip"
    [ -f "$zip" ] && { echo "SKIP $set_name (already built)"; return 0; }

    local w="$WORK/$set_name"
    rm -rf "$w"; mkdir -p "$w/thumbnails"

    local n=0
    shopt -s nullglob
    for f in "$set_dir"/*.jpg; do
        local id; id="$(basename "$f")"
        magick "$f" -strip -resize "$FULL_GEOM" -quality "$FULL_Q" \
               "$w/$id" 2>/dev/null || { echo "  BAD  $set_name/$id"; continue; }
        # Always from the full-size art.  Exact geometry (trailing '!'), matching
        # the desktop pack's fixed 114x166 convention.
        magick "$f" -strip -resize "${THUMB_GEOM}!" -quality "$THUMB_Q" \
               "$w/thumbnails/$id" 2>/dev/null || true
        n=$((n+1))
    done
    shopt -u nullglob

    if [ "$n" -eq 0 ]; then rm -rf "$w"; echo "EMPTY $set_name"; return 0; fi

    mkdir -p "$OUT/sets/$set_name"
    ( cd "$w" && zip -q -0 -r -X "$zip" . ) || { echo "ZIPFAIL $set_name"; rm -rf "$w"; return 1; }
    local sz; sz="$(du -m "$zip" | cut -f1)"
    rm -rf "$w"
    echo "OK   $set_name  ${n} cards  ${sz} MB"
}
export -f convert_set
export SRC OUT WORK FULL_GEOM FULL_Q THUMB_GEOM THUMB_Q

echo "=== vita card pack: $(date -Is) ===" | tee -a "$LOG"
find "$SRC" -mindepth 1 -maxdepth 1 -type d -print0 \
  | xargs -0 -P "$JOBS" -I{} bash -c 'convert_set "$@"' _ {} 2>&1 | tee -a "$LOG"

echo "=== done: $(date -Is) ===" | tee -a "$LOG"
du -sh "$OUT/sets" | tee -a "$LOG"
find "$OUT/sets" -name '*.zip' | wc -l | xargs echo "zips:" | tee -a "$LOG"
rmdir "$WORK" 2>/dev/null
touch "$OUT/.build-done"
