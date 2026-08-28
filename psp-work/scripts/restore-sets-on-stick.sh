#!/bin/bash
# Reverse of reduce-sets-on-stick.sh: move everything in SIDELINE back into Res.
set -euo pipefail
WAGIC=$(find /mnt/pspstick /run/media/magi -maxdepth 4 -type d -ipath "*/PSP/GAME/WAGIC" 2>/dev/null | head -1 || true)
[ -z "$WAGIC" ] && { echo "WAGIC dir not found"; exit 1; }
SIDE="$WAGIC/SIDELINE"
[ -d "$SIDE" ] || { echo "no SIDELINE dir — nothing to restore"; exit 0; }
for d in "$SIDE/sets"/*/; do [ -d "$d" ] && mv "$d" "$WAGIC/Res/sets/$(basename "$d")"; done
for f in "$SIDE/baka"/*; do [ -f "$f" ] && mv "$f" "$WAGIC/Res/ai/baka/$(basename "$f")"; done
if [ -d "$SIDE/primitives" ]; then
  PRIM=$(find "$WAGIC/Res/sets" -maxdepth 1 -type d -iname primitives | head -1)
  for f in "$SIDE/primitives"/*; do [ -f "$f" ] && mv -f "$f" "$PRIM/$(basename "$f")"; done
fi
rmdir "$SIDE/sets" "$SIDE/baka" "$SIDE/primitives" "$SIDE" 2>/dev/null || true
echo "restored. sets now: $(ls -d "$WAGIC/Res/sets"/*/ | wc -l)"
sync
