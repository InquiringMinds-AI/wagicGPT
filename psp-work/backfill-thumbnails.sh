#!/usr/bin/env bash
# Backfill missing thumbnails in the full-size desktop card pack.
#
# The pack ships 77302 full-size cards but only 54103 thumbnails, so ~23k cards
# fall back to the no-art frame anywhere the engine renders small (deck editor,
# card browser).  Generate the absent ones from the full-size art.
#
# Geometry matches what the pack already uses: an EXACT 114x166 -- sampled
# thumbnails are uniformly 114x166 even though 618x882 scaled to width 114 is
# 163 tall, i.e. the pack uses a forced resize, not an aspect-preserving fit.
#
# Non-destructive: only writes files that do not exist.  Pass DRYRUN=1 to count.
set -uo pipefail

SRC="${SRC:-/home/magi/Projects/wagicGPT/projects/mtg/bin/User/sets}"
GEOM="${GEOM:-114x166}"
Q="${Q:-85}"
JOBS="${JOBS:-6}"
DRYRUN="${DRYRUN:-0}"
LOG="${LOG:-/home/magi/Projects/wagicGPT/psp-work/card-packs/thumb-backfill.log}"
mkdir -p "$(dirname "$LOG")"

do_set() {
    local set_dir="$1" set_name made=0 failed=0
    set_name="$(basename "$set_dir")"
    shopt -s nullglob
    for f in "$set_dir"/*.jpg; do
        local id; id="$(basename "$f")"
        local t="$set_dir/thumbnails/$id"
        [ -f "$t" ] && continue
        if [ "$DRYRUN" = "1" ]; then made=$((made+1)); continue; fi
        mkdir -p "$set_dir/thumbnails"
        if magick "$f" -strip -resize "${GEOM}!" -quality "$Q" "$t" 2>/dev/null; then
            made=$((made+1))
        else
            rm -f "$t"; failed=$((failed+1))
        fi
    done
    shopt -u nullglob
    [ "$made" -gt 0 ] || [ "$failed" -gt 0 ] && \
        echo "$set_name  made=$made failed=$failed"
    return 0
}
export -f do_set
export SRC GEOM Q DRYRUN

echo "=== thumbnail backfill (DRYRUN=$DRYRUN): $(date -Is) ===" | tee -a "$LOG"
find "$SRC" -mindepth 1 -maxdepth 1 -type d -print0 \
  | xargs -0 -P "$JOBS" -I{} bash -c 'do_set "$@"' _ {} 2>&1 | tee -a "$LOG"
echo "=== done: $(date -Is) ===" | tee -a "$LOG"
echo "full-size: $(find "$SRC" -mindepth 2 -maxdepth 2 -name '*.jpg' | wc -l)  thumbs: $(find "$SRC" -path '*/thumbnails/*.jpg' | wc -l)" | tee -a "$LOG"
touch /home/magi/Projects/wagicGPT/psp-work/card-packs/.thumbs-done
