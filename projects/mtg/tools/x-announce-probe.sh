#!/usr/bin/env bash
# Wave-41 #W41-1 / #W41-2 -- the X-announcement instrument.
#
# The test suite cannot host an AIPlayerGPT seat (TestSuiteAI derives from
# AIPlayerBaka; see ledger #W41-15), and the heuristic seat refuses a zero-slack
# {X} cast outright (its X-slack penalty drives shouldPlayPercentage negative).
# So the ONE decision this measures - the GPT seat announcing and committing
# X = 0 through a real model call - has to be measured in a real game.
#
# It runs one self-play game against a LOCAL STUB endpoint (tools/gpt-stub-
# server.py): real prompt assembly, real HTTP round trip, real reply parsing,
# real translog write, deterministic answer, no inference. Then it asserts the
# #W41-1 metric on the resulting translog:
#
#     ANNOUNCE_X records / {X} casts committed by the GPT seat  ==  1.00
#     every one of them a MODEL call (latency_ms >= 0, no fallback)
#
# Usage (from projects/mtg/):   bash tools/x-announce-probe.sh [deckN] [oppN]
# Defaults to the wave-41 probe decks 198 (zero-slack) vs 199 (clock). The probe
# decks are NOT shipped - write them into bin/Res/ai/baka/ first and delete them
# after (strategy-design/wave41/probe-xcommit/ holds the decklists).
set -u

HERE="$(cd "$(dirname "$0")/.." && pwd)"
DECK0="${1:-198}"
DECK1="${2:-199}"
PORT="${WAGIC_STUB_PORT:-8299}"
RUNHOME="$(mktemp -d /tmp/x-announce-probe.XXXXXX)"

python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" &
STUB=$!
trap 'kill "$STUB" 2>/dev/null' EXIT
for _ in $(seq 1 40); do
    curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break
    sleep 0.25
done

cd "$HERE/bin" || exit 1
timeout 1200 systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
    env -u WAYLAND_DISPLAY -u DISPLAY \
    HOME="$RUNHOME" \
    WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
    WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
    WAGIC_SELFPLAY_DECK0="$DECK0" WAGIC_SELFPLAY_DECK1="$DECK1" \
    WAGIC_AI=gpt WAGIC_GPT_URL="http://127.0.0.1:$PORT" \
    WAGIC_GPT_MODEL=stub-model WAGIC_GPT_TRANSLOG=1 \
    ./wagic > "$RUNHOME/probe.stdout" 2> "$RUNHOME/probe.stderr"

echo "probe home: $RUNHOME"
python3 - "$RUNHOME" "$DECK0" <<'PY'
import glob, json, os, sys
home, deck = sys.argv[1], sys.argv[2]
logs = glob.glob(os.path.join(home, ".Wagic/ai/gpt/logs/*deck%s-*.jsonl" % deck))
if not logs:
    print("FAIL: no translog for the GPT seat"); sys.exit(1)
recs = [json.loads(l) for l in open(logs[0])]
casts = [r for r in recs if r.get("kind") == "ask"
         and str(r.get("chosen_text", "")).startswith("Cast ")
         and "{x}" in str(r.get("chosen_text", "")).lower()]
xs = [r for r in recs if "Announce the value of X" in r.get("prompt", "")]
bad = [r for r in xs if r.get("fallback") or r.get("latency_ms", -1) < 0]
print("{X} casts committed : %d" % len(casts))
print("ANNOUNCE_X records  : %d" % len(xs))
print("ratio               : %s" % ("n/a" if not casts else "%.2f" % (len(xs) / len(casts))))
print("non-model answers   : %d" % len(bad))
ok = casts and len(xs) >= len(casts) and not bad
print("VERDICT: %s" % ("PASS" if ok else "FAIL"))
sys.exit(0 if ok else 1)
PY
