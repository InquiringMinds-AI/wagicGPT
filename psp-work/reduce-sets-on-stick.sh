#!/bin/bash
# Set-volume experiment (2026-08-01): prune stick WAGIC to 39 sets + baka decks 1-20.
# Sideline by RENAME on the stick (instant, reversible). Reverse with: restore-sets-on-stick.sh
set -euo pipefail

KEEP="10E 4ED 5DN 6ED 7ED 8ED 9ED A25 AKH ALA ARB AVR CFX CHK CSP DDF DKA DPA EVE LCI LRW M10 M11 M12 M13 M20 ME4 MPS MRD ONS OTJ RAV RV SHM TSB V10 VMA WOE ZEN"

WAGIC=$(find /mnt/pspstick /run/media/magi -maxdepth 4 -type d -ipath "*/PSP/GAME/WAGIC" 2>/dev/null | head -1 || true)
[ -z "$WAGIC" ] && { echo "WAGIC dir not found — is the stick mounted?"; exit 1; }
FILTERED=~/Projects/wagicGPT/psp-work/WTH-39/Res/sets/primitives
echo "stick WAGIC: $WAGIC"

# sanity: clean EBOOT still deployed
md5=$(md5sum "$WAGIC/EBOOT.PBP" | cut -d' ' -f1)
[ "$md5" = "1f6b3c7687c6220ec67fd71b46956bc0" ] || { echo "EBOOT md5 MISMATCH: $md5 — stop and investigate"; exit 1; }

SIDE="$WAGIC/SIDELINE"
mkdir -p "$SIDE/sets" "$SIDE/baka"

moved_sets=0
for d in "$WAGIC/Res/sets"/*/; do
  name=$(basename "$d")
  up=$(echo "$name" | tr a-z A-Z)
  [ "$up" = "PRIMITIVES" ] && continue
  keep=no
  for k in $KEEP; do [ "$up" = "$k" ] && keep=yes && break; done
  if [ "$keep" = no ]; then mv "$d" "$SIDE/sets/$name"; moved_sets=$((moved_sets+1)); fi
done

moved_decks=0
for f in "$WAGIC/Res/ai/baka"/*; do
  base=$(basename "$f")
  low=$(echo "$base" | tr A-Z a-z)
  n=$(echo "$low" | sed -n 's/^deck\([0-9]\+\)\.txt$/\1/p')
  [ -z "$n" ] && continue
  if [ "$n" -gt 20 ] || [ "$n" -eq 16 ]; then mv "$f" "$SIDE/baka/$base"; moved_decks=$((moved_decks+1)); fi
done

# primitives: sideline originals, install filtered versions (all but _macros.txt)
mkdir -p "$SIDE/primitives"
for f in mtg.txt borderline.txt planeswalkers.txt unsupported.txt; do
  src=$(find "$WAGIC/Res/sets" -maxdepth 2 -ipath "*/primitives/$f" | head -1)
  [ -z "$src" ] && { echo "primitives/$f not found on stick"; exit 1; }
  mv "$src" "$SIDE/primitives/$f"
  cp "$FILTERED/$f" "$(dirname "$src")/$f"
done

echo "sidelined: $moved_sets set dirs, $moved_decks deck files, 4 primitives files swapped"
echo "remaining sets: $(ls -d "$WAGIC/Res/sets"/*/ | wc -l)  remaining decks: $(ls "$WAGIC/Res/ai/baka" | grep -ci '^deck[0-9]*\.txt')"
sync
echo "sync done — safe to unplug after umount"
