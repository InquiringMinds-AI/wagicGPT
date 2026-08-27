#!/bin/bash
# Copy per-set image zips to the stick: Res/sets/<SET>/<SET>.zip
# Run via script file, NOT inline systemd-run bash -c: '%' in ${var%.zip} is a
# systemd specifier and gets mangled (learned 2026-08-01, 336 copies to one file).
set -u
W=/mnt/pspstick/PSP/GAME/WAGIC/Res/sets
cd ~/Projects/wagicGPT/psp-work/image-zips
n=0; skip=0
for z in *.zip; do
  s=${z%.zip}
  if [ -d "$W/$s" ]; then
    cp "$z" "$W/$s/$s.zip" && n=$((n+1))
  else
    skip=$((skip+1))
  fi
done
sync
echo "$n copied, $skip skipped" > /tmp/claude-1000/-home-magi/c5f2cc15-8c71-4d5e-893c-df195763a534/scratchpad/imgcopy.done
