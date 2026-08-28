#!/bin/bash
# Replace the 336 per-set card zips on the PSP with the CORRECT pack.
#
# WHY: the 2026-08-04 reinstall copied psp-work/image-zips, which is the OLD pack
# that make-psp-cards.sh was written to replace: 27,571 of its cards are 180x257,
# one pixel over the 256 boundary, so they allocate 256x512 = 512 KB instead of
# 256x256 = 256 KB. 35% of the pack at double price. The correct pack is
# psp-work/card-packs/psp, uniformly <=180x250 -> 256 KB.
#
# Script file, never inline `bash -c`: '%' in ${z%.zip} is a systemd specifier.
set -u

D=/run/media/magi/disk/PSP/GAME/WAGIC/Res/sets
SRC=/home/magi/Projects/wagicGPT/psp-work/card-packs/psp/sets
LOG=/tmp/psp-fix-zips.log

exec > >(tee -a "$LOG") 2>&1
echo "=== card-zip fix start $(date +%F_%T) ==="

[ -d "$D" ]   || { echo "ABORT: no $D (PSP mounted?)"; exit 1; }
[ -d "$SRC" ] || { echo "ABORT: no $SRC"; exit 1; }
n=$(find "$SRC" -name '*.zip' | wc -l)
[ "$n" -eq 336 ] || { echo "ABORT: expected 336 source zips, found $n"; exit 1; }
echo "source OK: $n zips"

copied=0; skipped=0
while IFS= read -r z; do
  s=$(basename "$z" .zip)
  if [ -d "$D/$s" ]; then
    cp "$z" "$D/$s/$s.zip" && copied=$((copied+1))
  else
    skipped=$((skipped+1)); echo "  no set dir on device for $s"
  fi
done < <(find "$SRC" -name '*.zip' | sort)
sync
echo "copied=$copied skipped=$skipped"

echo "--- byte-size verification ---"
bad=0
while IFS= read -r z; do
  s=$(basename "$z" .zip)
  [ -f "$D/$s/$s.zip" ] || continue
  x=$(stat -c%s "$z"); y=$(stat -c%s "$D/$s/$s.zip")
  [ "$x" = "$y" ] || { echo "MISMATCH $s src=$x dev=$y"; bad=$((bad+1)); }
done < <(find "$SRC" -name '*.zip' | sort)
echo "verification: $([ $bad -eq 0 ] && echo 'all match' || echo "$bad MISMATCHED")"

sync
echo "device sets total: $(du -sh /run/media/magi/disk/PSP/GAME/WAGIC/Res/sets | cut -f1)"
echo "=== card-zip fix done $(date +%F_%T) ==="
touch /tmp/PSP-ZIPFIX-DONE
