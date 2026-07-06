#!/usr/bin/env bash
# selfplay-harness.sh - run wagicGPT AI-vs-AI games headlessly, in bulk, for
# prompt/strategy testing, and harvest the per-decision translogs.
#
# The GUI is out of the way entirely: WAGIC_SELFPLAY=1 boots the game straight
# into an endless AI-vs-AI demo (no menu navigation), and the window is parked
# on a hidden Hyprland special workspace (real GPU, no compositor throttle) so
# nothing appears on the user's display. Each decision both players make is
# written to ~/.Wagic/ai/gpt/logs/*.jsonl (WAGIC_GPT_TRANSLOG). This script
# runs until a target number of games, harvests that run's logs into a labeled
# directory, and prints a decision-kind summary. Win-rate is not computed here
# (game outcomes are not yet logged - a planned follow-up); the primary output
# is the decision transcripts for cold-check / decision-quality analysis.
#
# Usage:
#   tools/selfplay-harness.sh [-n GAMES] [-t TIMEOUT_S] [-o OUTDIR] \
#                             [-u URL] [-m MODEL] [-k KEY] [--thinking]
# Defaults: 10 games, 3600s cap, Spark qwen35 (keyless), thinking off.
#
# Run from projects/mtg (needs ./bin/wagic and ./bin/Res).
set -uo pipefail

GAMES=10
TIMEOUT_S=3600
OUTDIR=""
URL="http://100.116.136.74:8011"
MODEL="qwen35"
KEY=""
THINKING=0
JOBS=1   # concurrent game instances (each is a full AI-vs-AI game)

while [ $# -gt 0 ]; do
    case "$1" in
        -n) GAMES="$2"; shift 2;;
        -t) TIMEOUT_S="$2"; shift 2;;
        -o) OUTDIR="$2"; shift 2;;
        -u) URL="$2"; shift 2;;
        -m) MODEL="$2"; shift 2;;
        -k) KEY="$2"; shift 2;;
        -j) JOBS="$2"; shift 2;;
        --thinking) THINKING=1; shift;;
        *) echo "unknown arg: $1" >&2; exit 2;;
    esac
done
# Games are inference-latency-bound (~1 game / 15-20 min each), so throughput
# comes from running games CONCURRENTLY. Spark serves max-num-seqs 16 requests
# and its memory is flat under load (KV is pre-allocated), so up to ~8 games
# (16 requests, 2 players each) saturate the batch without risk; beyond that,
# requests just queue. Cap accordingly.
[ "$JOBS" -gt 8 ] && { echo "capping -j to 8 (Spark max-num-seqs 16 = ~8 games)"; JOBS=8; }
[ "$JOBS" -lt 1 ] && JOBS=1

HERE="$(cd "$(dirname "$0")/.." && pwd)"   # projects/mtg
BIN="$HERE/bin/wagic"
LOGDIR="$HOME/.Wagic/ai/gpt/logs"
[ -x "$BIN" ] || { echo "no binary at $BIN (build first)" >&2; exit 1; }
mkdir -p "$LOGDIR"
# Run outputs live OUTSIDE the repo (never committed).
[ -n "$OUTDIR" ] || OUTDIR="$HOME/.Wagic/ai/gpt/selfplay-runs/run-$(date +%Y%m%d-%H%M%S)"
mkdir -p "$OUTDIR"

# Endpoint reachability check (fail fast rather than run Baka-fallback games).
if ! curl -s -m 6 "$URL/v1/models" | grep -q "$MODEL"; then
    echo "WARN: $MODEL not reachable at $URL/v1/models - games would fall back to the heuristic AI." >&2
    echo "      (proceeding anyway; ctrl-C to abort)" >&2
fi

# Baseline: only count translog files created after we launch.
BEFORE_LIST="$(mktemp)"
ls "$LOGDIR"/*.jsonl 2>/dev/null | sort > "$BEFORE_LIST"

# Open the game hidden on a special workspace (no flash on the visible display).
SIG="$(ls -t /run/user/1000/hypr/ 2>/dev/null | head -1)"
if [ -n "$SIG" ]; then
    HYPRLAND_INSTANCE_SIGNATURE="$SIG" hyprctl keyword windowrule \
        "workspace special:selfplay silent, class:^(wagic)$" >/dev/null 2>&1
fi

echo "== selfplay harness =="
echo "  binary : $BIN"
echo "  target : $GAMES games (cap ${TIMEOUT_S}s), $JOBS concurrent"
echo "  model  : $MODEL @ $URL (thinking=$THINKING)"
echo "  outdir : $OUTDIR"

cd "$HERE/bin"
PIDS=()
for j in $(seq 1 "$JOBS"); do
    WAYLAND_DISPLAY="${WAYLAND_DISPLAY:-wayland-1}" SDL_VIDEODRIVER=wayland SDL_AUDIODRIVER=dummy \
        WAGIC_SELFPLAY=1 WAGIC_AI=gpt \
        WAGIC_GPT_URL="$URL" WAGIC_GPT_MODEL="$MODEL" WAGIC_GPT_KEY="$KEY" \
        WAGIC_GPT_THINKING="$THINKING" WAGIC_GPT_TRANSLOG=1 \
        ./wagic > "$OUTDIR/wagic-stdout-$j.log" 2>&1 &
    PIDS+=($!)
    sleep 2   # stagger startup so instances don't race the GL/profiling init
done
kill_all() { for p in "${PIDS[@]}"; do kill "$p" 2>/dev/null; done; }
trap kill_all EXIT INT TERM

# Each game creates 2 new translog files (one per AIPlayerGPT). Stop at the
# target, the time cap, or if all instances die.
START=$(date +%s)
target_files=$(( GAMES * 2 ))
while :; do
    sleep 15
    ALIVE=0; for p in "${PIDS[@]}"; do kill -0 "$p" 2>/dev/null && ALIVE=$(( ALIVE + 1 )); done
    if [ "$ALIVE" -eq 0 ]; then echo "all game instances exited early"; break; fi
    NEW=$(comm -13 "$BEFORE_LIST" <(ls "$LOGDIR"/*.jsonl 2>/dev/null | sort) | wc -l)
    EL=$(( $(date +%s) - START ))
    echo "  ... ${EL}s: $NEW new translog files (~$(( NEW / 2 )) games), $ALIVE/$JOBS instances alive"
    [ "$NEW" -ge "$target_files" ] && { echo "reached target"; break; }
    [ "$EL" -ge "$TIMEOUT_S" ] && { echo "hit time cap"; break; }
done

kill_all; wait 2>/dev/null
trap - EXIT INT TERM

# Harvest this run's logs.
comm -13 "$BEFORE_LIST" <(ls "$LOGDIR"/*.jsonl 2>/dev/null | sort) | while read -r f; do
    cp "$f" "$OUTDIR/"
done
rm -f "$BEFORE_LIST"

# Decision-kind summary over the harvested run.
python3 - "$OUTDIR" <<'PY'
import json, glob, os, sys
from collections import Counter
out = sys.argv[1]
files = [f for f in glob.glob(os.path.join(out, "*.jsonl"))]
kinds = Counter(); n = 0
for f in files:
    for line in open(f):
        try: r = json.loads(line)
        except: continue
        kinds[r.get("kind","?")] += 1; n += 1
print(f"\n== harvested {len(files)} player-game logs, {n} decisions ==")
for k, c in kinds.most_common():
    print(f"  {k:10s} {c}")
print(f"\nlogs in: {out}")
PY
