#!/bin/bash
# Clean reinstall of Wagic on the PSP Memory Stick, PRESERVING User/.
# Written as a script file, never inline `systemd-run bash -c`: '%' in ${z%.zip}
# is a systemd specifier and gets mangled (cost 336 copies to one file 2026-08-01).
set -u

D=/run/media/magi/disk/PSP/GAME/WAGIC
SRC_RES=/home/magi/Projects/wagicGPT-psp/projects/mtg/bin/Res
ZIPS=/home/magi/Projects/wagicGPT/psp-work/card-packs/psp/sets
SIDE=/home/magi/Projects/wagicGPT/psp-work/sidecar
EBOOT=${EBOOT:-/tmp/EBOOT-bisect-10-671b6c412.PBP}
LOG=/tmp/psp-reinstall.log

exec > >(tee -a "$LOG") 2>&1
echo "=== reinstall start $(date +%F_%T) ==="

# Hard preconditions - refuse to delete anything unless every source is present.
for p in "$D" "$SRC_RES" "$ZIPS" "$SIDE" "$EBOOT"; do
  [ -e "$p" ] || { echo "ABORT: missing $p"; exit 1; }
done
[ -d "$D/User" ] || { echo "ABORT: no User/ on device"; exit 1; }
n=$(find "$D/User" -type f | wc -l)
[ "$n" -ge 10 ] || { echo "ABORT: User/ has only $n files, expected >=10"; exit 1; }
echo "preconditions OK (User/ has $n files, preserved untouched)"

echo "--- removing EBOOT.PBP and Res/ (User/ untouched) ---"
rm -f "$D/EBOOT.PBP"
rm -rf "$D/Res"
sync
echo "removed. remaining top level:"; ls -1 "$D"

echo "--- copying base Res ($(du -sh "$SRC_RES" | cut -f1)) ---"
cp -a "$SRC_RES" "$D/Res"
sync
echo "base Res copied: $(find "$D/Res" -type f | wc -l) files"

echo "--- copying $(ls "$ZIPS"/*.zip | wc -l) per-set image zips ---"
cd "$ZIPS" || exit 1
n=0; skip=0
for z in *.zip; do
  s=${z%.zip}
  if [ -d "$D/Res/sets/$s" ]; then
    cp "$z" "$D/Res/sets/$s/$s.zip" && n=$((n+1))
  else
    skip=$((skip+1)); echo "  no set dir for $s"
  fi
done
sync
echo "image zips: $n copied, $skip skipped"

echo "--- copying sidecars ---"
cp "$SIDE"/cardtext.dat "$SIDE"/cardtext.idx "$SIDE"/cardauto.dat "$SIDE"/cardauto.idx "$D/Res/"
sync
ls -l "$D/Res"/card*.dat "$D/Res"/card*.idx

echo "--- copying EBOOT ($EBOOT) ---"
cp "$EBOOT" "$D/EBOOT.PBP"
sync

echo "--- verification ---"
a=$(stat -c%s "$EBOOT"); b=$(stat -c%s "$D/EBOOT.PBP")
echo "EBOOT src=$a dev=$b $([ "$a" = "$b" ] && echo OK || echo MISMATCH)"
bad=0
for f in cardtext.dat cardtext.idx cardauto.dat cardauto.idx; do
  x=$(stat -c%s "$SIDE/$f"); y=$(stat -c%s "$D/Res/$f")
  [ "$x" = "$y" ] || { echo "MISMATCH $f src=$x dev=$y"; bad=1; }
done
echo "sidecars: $([ $bad -eq 0 ] && echo 'all match' || echo 'MISMATCH')"
zbad=0
cd "$ZIPS" || exit 1
for z in *.zip; do
  s=${z%.zip}
  [ -f "$D/Res/sets/$s/$s.zip" ] || continue
  x=$(stat -c%s "$z"); y=$(stat -c%s "$D/Res/sets/$s/$s.zip")
  [ "$x" = "$y" ] || { echo "MISMATCH zip $s src=$x dev=$y"; zbad=$((zbad+1)); }
done
echo "image zips: $([ $zbad -eq 0 ] && echo 'all match' || echo "$zbad MISMATCHED")"
echo "User/ preserved: $(find "$D/User" -type f | wc -l) files"
echo "total on device: $(du -sh "$D" | cut -f1), $(find "$D" -type f | wc -l) files"

sync
echo "=== reinstall done $(date +%F_%T) ==="
touch /tmp/PSP-REINSTALL-DONE
