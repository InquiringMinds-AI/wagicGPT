#!/bin/bash
# baka-ab.sh - heuristic-only (Baka vs Baka, no LLM) round-robin over the deck pool.
# The engine A/B gate: two binaries, same pool, same reps -> compare with tools/baka-ab-compare.py.
# Usage: tools/baka-ab.sh <bin-dir> <outdir> [reps=10] [jobs=4] [pool="146 152 125 130 123 126 162"]
# Every game is one headless process (WAGIC_AI=baka, fair hands, fastclock); results.tsv rows:
#   d0 d1 rep exit secs result   where exit != 0 (124 timeout, 134 SIGABRT, 137 killed, 139 SIGSEGV)
# or result == NONE is a game that did not finish - a HARD gate failure.
# Origin: 2026-09-13 engine audit - a 10% heuristic-game hang (d7526c7bf) shipped through 14 commits
# and a corpus binary because no gate ever played a game without the LLM seat.
BINDIR=$1; OUT=$2; REPS=${3:-10}; JOBS=${4:-4}; POOL=${5:-"146 152 125 130 123 126 162"}
[ -x "$BINDIR/wagic" ] || { echo "no wagic binary in $BINDIR" >&2; exit 2; }
mkdir -p "$OUT/stderr"; : > "$OUT/games.txt"
for r in $(seq 1 "$REPS"); do for a in $POOL; do for b in $POOL; do
  [ "$a" -lt "$b" ] && echo "$a $b $r" >> "$OUT/games.txt"; done; done; done
export BINDIR OUT
run_one() {
  set -- $1; a=$1; b=$2; r=$3; e="$OUT/stderr/game-${a}v${b}-r${r}.stderr"; t0=$(date +%s)
  ( cd "$BINDIR" && unset WAYLAND_DISPLAY DISPLAY && timeout -k 10 300s env WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
      WAGIC_SELFPLAY_FAIRHAND=1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 WAGIC_SELFPLAY_DECK0=$a WAGIC_SELFPLAY_DECK1=$b \
      WAGIC_AI=baka ./wagic > "$e" 2>&1 ); rc=$?; t1=$(date +%s)
  res=$(/usr/bin/grep -o 'WAGIC_SELFPLAY_RESULT.*' "$e" | tail -1)
  printf "%s\t%s\t%s\t%s\t%s\t%s\n" "$a" "$b" "$r" "$rc" "$((t1-t0))" "${res:-NONE}" >> "$OUT/results.tsv"
}
export -f run_one
printf "d0\td1\trep\texit\tsecs\tresult\n" > "$OUT/results.tsv"
xargs -P "$JOBS" -I{} bash -c 'run_one "{}"' < "$OUT/games.txt"
echo "$BINDIR" > "$OUT/bindir.txt"; touch "$OUT/DONE"
