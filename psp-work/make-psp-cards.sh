#!/usr/bin/env bash
# Build a PSP card-image pack from the full-resolution desktop set.
#
# Source : projects/mtg/bin/User/sets/<SET>/<id>.jpg   (English pool; the nine
#          refetched sets - PS11, PMPS, PMPS06-11, PSAL - are English there)
# Output : <OUT>/sets/<SET>/<SET>.zip, STORED (JPEG is already compressed), with
#          in-zip keys "<id>.jpg" and "thumbnails/<id>.jpg".
#
# ============================ WHY THESE GEOMETRIES ===========================
# The PSP allocates textures at POWER-OF-TWO dimensions on BOTH axes
# (JGE/src/JGfx.cpp: `int texWidth = getNextPower2(width)`, and the same for
# height; size = texWidth * texHeight * 4 bytes). So the memory a card costs is
# decided by which power-of-two bucket each axis lands in, NOT by its pixel
# count. One pixel over a boundary doubles the cost.
#
# The pack this replaces was built by fitting to a box, which let the source
# aspect ratios decide the final height - and produced three groups:
#
#     180x257  (158 sets)  -> 256 x 512 = 512 KB   <-- one pixel over 256
#     180x254  (123 sets)  -> 256 x 256 = 256 KB
#     180x251  ( 55 sets)  -> 256 x 256 = 256 KB
#
# and thumbnails in two groups, the second being desktop-sized thumbs that leaked
# into the PSP pack for the nine refetched sets:
#
#      64x93   (273 sets)  ->  64 x 128 =  32 KB
#     114x166  (  9 sets)  -> 128 x 256 = 128 KB   <-- 4x, incl. PSAL's 761 cards
#
# Against an 8 MB texture cache that is the difference between ~256 resident
# thumbnails and ~64. The deck editor carousel is the densest consumer of them.
#
# So: cap the LONG axis under 256 for full art and under 128 for thumbs, with
# margin, and never let a source aspect ratio push a dimension over a boundary.
# 180x250 fits every source ratio in the pool (0.7007 / 0.7099 / 0.7179 w/h):
# height binds at 250, width lands 175-180, both inside 256.
#
#     full   -> <=180 x <=250  -> 256 x 256 = 256 KB   (uniform, no outliers)
#     thumb  -> exactly 64x93  -> 64 x 128  =  32 KB   (uniform, no outliers)
#
# Thumbnails are ALWAYS derived from the full-size source art, never rescaled
# from the shipped 114x166 desktop thumb: downscaling once from 618x882 beats
# resampling an already-lossy small JPEG, and every card gets a thumb whether or
# not the desktop pack shipped one. It is also what keeps the nine refetched
# sets from re-importing their desktop-sized thumbs.
#
# WOTC-derived scans: local + his devices only. Never committed, never in a
# release artifact.
set -uo pipefail

SRC="${SRC:-/home/magi/Projects/wagicGPT/projects/mtg/bin/User/sets}"
OUT="${OUT:-/home/magi/Projects/wagicGPT/psp-work/card-packs/psp}"
WORK="${WORK:-/home/magi/Projects/wagicGPT/psp-work/card-packs/.work-psp}"
JOBS="${JOBS:-8}"

# Fit inside the box (no '!'): aspect preserved, height binds at 250.
FULL_GEOM="${FULL_GEOM:-180x250}"
FULL_Q="${FULL_Q:-85}"
# Exact (trailing '!'): every thumb identical, so the bucket is provably uniform.
THUMB_GEOM="${THUMB_GEOM:-64x93}"
THUMB_Q="${THUMB_Q:-85}"

# ImageMagick is multithreaded by default and will oversubscribe against -P.
# (Same trap the tesseract sweep hit: 6 workers demanded ~13 cores and thrashed.)
export MAGICK_THREAD_LIMIT=1

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
    # "$set_dir"/*.jpg does not recurse, so the source thumbnails/ dir is excluded.
    for f in "$set_dir"/*.jpg; do
        local id; id="$(basename "$f")"
        magick "$f" -strip -resize "$FULL_GEOM" -quality "$FULL_Q" \
               "$w/$id" 2>/dev/null || { echo "  BAD  $set_name/$id"; continue; }
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

echo "=== psp card pack: $(date -Is) ===" | tee -a "$LOG"
find "$SRC" -mindepth 1 -maxdepth 1 -type d -print0 \
  | xargs -0 -P "$JOBS" -I{} bash -c 'convert_set "$@"' _ {} 2>&1 | tee -a "$LOG"

echo "=== done: $(date -Is) ===" | tee -a "$LOG"
du -sh "$OUT/sets" | tee -a "$LOG"
find "$OUT/sets" -name '*.zip' | wc -l | xargs echo "zips:" | tee -a "$LOG"
rmdir "$WORK" 2>/dev/null
touch "$OUT/.build-done"
