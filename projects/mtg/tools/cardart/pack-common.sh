#!/usr/bin/env bash
# Shared pack engine: master pool -> per-set art zips for one platform.
# Sourced by the pack-<platform>.sh wrappers; not run directly.
#
# Input  : $SRC/<SET>/<id>.jpg          (full-size pool; fetch-art.py fills it)
# Output : $OUT/sets/<SET>/<SET>.zip    STORED (JPEG is already compressed), with
#          in-zip keys "<id>.jpg" and "thumbnails/<id>.jpg" -- exactly what
#          WResourceManager::RetrieveCard looks for once AttachZipFile picks up
#          sets/<SET>/<SET>.zip. The user path is searched before the system
#          path on every platform, so packs drop into the device's User dir
#          without an engine change.
#
# Contract the wrappers must set before sourcing:
#   PLATFORM    label for logs and the default OUT dir
#   FULL_GEOM   WxH box for full art (aspect-preserving fit), or "copy" to
#               pass the pool image through untouched (desktop)
#   FULL_Q      jpeg quality for resized full art
#   THUMB_GEOM  WxH, applied EXACTLY (forced, trailing '!') so the platform's
#               thumbnail bucket is provably uniform
#   THUMB_Q     jpeg quality for thumbs
# Optional overrides: SRC OUT WORK JOBS SETS (space-separated whitelist)
#
# Thumbnails are ALWAYS derived from the full-size pool art, never rescaled
# from another platform's thumb: downscaling once from ~672x936 beats
# resampling an already-lossy small JPEG, and every card gets a thumb whether
# or not any other pack shipped one.
#
# WOTC-derived scans: local + his devices only. Never committed, never in a
# release artifact.
set -uo pipefail

: "${PLATFORM:?wrapper must set PLATFORM}" "${FULL_GEOM:?}" "${FULL_Q:?}" \
  "${THUMB_GEOM:?}" "${THUMB_Q:?}"

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO="$HERE"; for _ in 1 2 3 4 5 6; do [ -d "$REPO/projects/mtg" ] && break; REPO="$(dirname "$REPO")"; done
POOL_BASE="${POOL_BASE:-$REPO/projects/mtg/bin/User/sets}"
# The dev pool hides itself as sets.hidden so the dev build boots artless.
if [ -z "${SRC:-}" ]; then
    if [ -d "$POOL_BASE/sets.hidden" ]; then SRC="$POOL_BASE/sets.hidden"; else SRC="$POOL_BASE"; fi
fi
OUT="${OUT:-$PWD/wagic-card-packs/$PLATFORM}"
WORK="${WORK:-$PWD/wagic-card-packs/.work-$PLATFORM}"
JOBS="${JOBS:-6}"
SETS="${SETS:-}"

# ImageMagick is multithreaded by default and will oversubscribe against -P.
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
    # *.jpg does not recurse, so a source thumbnails/ dir is excluded.
    for f in "$set_dir"/*.jpg; do
        local id; id="$(basename "$f")"
        if [ "$FULL_GEOM" = "copy" ]; then
            cp "$f" "$w/$id" || { echo "  BAD  $set_name/$id"; continue; }
        else
            magick "$f" -strip -resize "$FULL_GEOM" -quality "$FULL_Q" \
                   "$w/$id" 2>/dev/null || { echo "  BAD  $set_name/$id"; continue; }
        fi
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

echo "=== $PLATFORM card pack: $(date -Is) ===" | tee -a "$LOG"
if [ -n "$SETS" ]; then
    for s in $SETS; do printf '%s/%s\0' "$SRC" "$s"; done \
      | xargs -0 -P "$JOBS" -I{} bash -c 'convert_set "$@"' _ {} 2>&1 | tee -a "$LOG"
else
    find "$SRC" -mindepth 1 -maxdepth 1 -type d -print0 \
      | xargs -0 -P "$JOBS" -I{} bash -c 'convert_set "$@"' _ {} 2>&1 | tee -a "$LOG"
fi

echo "=== done: $(date -Is) ===" | tee -a "$LOG"
du -sh "$OUT/sets" | tee -a "$LOG"
find "$OUT/sets" -name '*.zip' | wc -l | xargs echo "zips:" | tee -a "$LOG"
rmdir "$WORK" 2>/dev/null
touch "$OUT/.build-done"
