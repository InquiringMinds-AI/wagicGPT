#!/bin/bash
# baka-ab.sh - heuristic-only (Baka vs Baka, no LLM) round-robin over the deck pool.
# The engine A/B gate: two binaries, same pool, same reps -> compare with tools/baka-ab-compare.py.
# Usage: tools/baka-ab.sh [--paired[=<accepted-outdir>]] <bin-dir> <outdir> [reps=10] [jobs=4] [pool="146 152 125 130 123 126 162"]
# Every game is one headless process (WAGIC_AI=baka, fair hands, fastclock); results.tsv rows:
#   d0 d1 rep exit secs result seed   where exit != 0 (124 timeout, 134 SIGABRT, 137 killed, 139 SIGSEGV)
# or result == NONE is a game that did not finish - a HARD gate failure.
# Origin: 2026-09-13 engine audit - a 10% heuristic-game hang (d7526c7bf) shipped through 14 commits
# and a corpus binary because no gate ever played a game without the LLM seat.
#
# bug-list #13 (wave 82, lane EA): PAIRED games. Unpaired, every game is seeded by its launch
# second, so the accepted and candidate runs never play the same games and a |z| >= 2 flag needs
# 800+ games to resolve (step1-lane.md: deck152 flagged -2.25 then -0.87; 18 SEEDED games settled
# it in minutes). `--paired` draws one seed per game, passes it as WAGIC_SELFPLAY_SEED (the engine
# pins rand() and the observer's generators before any draw - GameStateDuel.cpp) and records it in
# the `seed` column; `--paired=<accepted-outdir>` REPLAYS the seeds of that run's results.tsv, game
# for game (same d0 d1 rep -> same seed), so `baka-ab-compare.py --paired` can judge each game as
# "same seed -> same or different outcome" instead of pooling per-deck rates. Unpaired runs write
# `-` in the seed column (clock-seeded, as before).
# A seeded game also runs with WAGIC_STALL_FLOOR=0: the held-window watchdog in ActionStack.cpp
# reads the WALL clock (steady_clock, #W54-R) every tick, so under load the number of ticks a
# window stays open - and with it the number of Baka's per-tick chance rolls - varied between two
# replays of one seed (proven 2026-09-18: 123v125 seed 401854594 replayed 5x concurrently gave
# turn 51 once and turn 52 four times; with the floor off, 3/3 byte-identical traces). The floor
# exists for interactive/LLM seats holding a window across a model round trip; a heuristic-only
# game answers every window in a tick, so nothing is lost by turning it off here.
PAIRED=0; SEEDSRC=""
case "${1:-}" in
  --paired) PAIRED=1; shift;;
  --paired=*) PAIRED=1; SEEDSRC="${1#--paired=}"; shift;;
esac
BINDIR=$1; OUT=$2; REPS=${3:-10}; JOBS=${4:-4}; POOL=${5:-"146 152 125 130 123 126 162"}
[ -x "$BINDIR/wagic" ] || { echo "no wagic binary in $BINDIR" >&2; exit 2; }
if [ -n "$SEEDSRC" ]; then
  [ -f "$SEEDSRC/results.tsv" ] || { echo "--paired=$SEEDSRC: no results.tsv to replay seeds from" >&2; exit 2; }
fi
mkdir -p "$OUT/stderr"; : > "$OUT/games.txt"
# games.txt: d0 d1 rep seed  (seed empty = clock)
for r in $(seq 1 "$REPS"); do for a in $POOL; do for b in $POOL; do
  [ "$a" -lt "$b" ] || continue
  seed=""
  if [ -n "$SEEDSRC" ]; then
    seed=$(awk -F'\t' -v a="$a" -v b="$b" -v r="$r" '$1==a && $2==b && $3==r {print $7; exit}' "$SEEDSRC/results.tsv")
    case "$seed" in ""|"-") echo "--paired=$SEEDSRC: no recorded seed for game $a v $b rep $r (was the accepted run --paired?)" >&2; exit 2;; esac
  elif [ "$PAIRED" = 1 ]; then
    seed=$(( (RANDOM * 32768 + RANDOM) % 1000000000 + 1 ))   # 1..1e9, never 0 (0 = clock)
  fi
  echo "$a $b $r $seed" >> "$OUT/games.txt"; done; done; done
export BINDIR OUT
run_one() {
  set -- $1; a=$1; b=$2; r=$3; seed=${4:-}; e="$OUT/stderr/game-${a}v${b}-r${r}.stderr"; t0=$(date +%s)
  ( cd "$BINDIR" && unset WAYLAND_DISPLAY DISPLAY && timeout -k 10 300s env WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
      WAGIC_SELFPLAY_FAIRHAND=1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 WAGIC_SELFPLAY_DECK0=$a WAGIC_SELFPLAY_DECK1=$b \
      ${seed:+WAGIC_SELFPLAY_SEED=$seed WAGIC_STALL_FLOOR=0} WAGIC_AI=baka ./wagic > "$e" 2>&1 ); rc=$?; t1=$(date +%s)
  res=$(/usr/bin/grep -o 'WAGIC_SELFPLAY_RESULT.*' "$e" | tail -1)
  printf "%s\t%s\t%s\t%s\t%s\t%s\t%s\n" "$a" "$b" "$r" "$rc" "$((t1-t0))" "${res:-NONE}" "${seed:--}" >> "$OUT/results.tsv"
}
export -f run_one
printf "d0\td1\trep\texit\tsecs\tresult\tseed\n" > "$OUT/results.tsv"
xargs -P "$JOBS" -I{} bash -c 'run_one "{}"' < "$OUT/games.txt"
echo "$BINDIR" > "$OUT/bindir.txt"; [ "$PAIRED" = 1 ] && echo "${SEEDSRC:-fresh}" > "$OUT/paired.txt"; touch "$OUT/DONE"
