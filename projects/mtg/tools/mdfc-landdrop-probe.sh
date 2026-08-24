#!/usr/bin/env bash
# Wave-43 #W43-12 -- the MDFC land-drop re-entry instrument.
#
# ARRIVAL TRACE: after a modal-DFC land is flipped and its back face chosen for
# the land drop, the land-drop decision must be COMMITTED once. Re-issuing it
# after the drop is spent hands the seat a stale cached answer that can no
# longer validate, and every such re-issue burns a deferred_to_heuristic record.
#
# Runs one self-play game against tools/gpt-stub-server.py, forcing the flip
# and then the back-face land play, and counts the deferred_to_heuristic
# records the flip produced.
#
# Usage (from projects/mtg/): bash tools/mdfc-landdrop-probe.sh [deckN] [oppN]
# Probe decks 198/199 are NOT shipped; write them into bin/Res/ai/baka/ first
# and delete them after.
set -u

HERE="$(cd "$(dirname "$0")/.." && pwd)"
DECK0="${1:-198}"
DECK1="${2:-199}"
PORT="${WAGIC_STUB_PORT:-8298}"
RUNHOME="$(mktemp -d "${WAGIC_PROBE_TMP:-$HOME}/mdfc-landdrop-probe.XXXXXX")"

python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" \
    --prefer "Play Mistgate Pathway" --prefer "Flip Side" --prefer "Play " &
STUB=$!
trap 'kill "$STUB" 2>/dev/null' EXIT
for _ in $(seq 1 40); do
    curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break
    sleep 0.25
done

cd "$HERE/bin" || exit 1
timeout "${WAGIC_PROBE_TIMEOUT:-1800}" systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
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
recs.sort(key=lambda r: r.get("seq", 0))
flips = [r for r in recs if str(r.get("chosen_text", "")).startswith("Flip Side")]
defers = [r for r in recs if r.get("fallback") == "deferred_to_heuristic"]
#The back face of a modal-DFC land can NEVER be played as a land drop in this
#engine (the front's restriction=compare(isflipped)~equalto~0 forbids it; the
#back reaches the battlefield through the autohand flip ability instead). So a
#land-drop question that lists it is offering the pilot an option the commit
#path will refuse - the defect, whether or not the pilot happens to pick it.
bad = [r for r in recs
       if "Land drop:" in str(r.get("prompt", ""))
       and "Play Mistgate Pathway" in str(r.get("prompt", ""))]
print("Flip Side commitments             : %d" % len(flips))
print("land asks offering the BACK face  : %d  seqs=%s" % (len(bad), [r.get("seq") for r in bad][:20]))
print("deferred_to_heuristic             : %d  seqs=%s" % (len(defers), [r.get("seq") for r in defers][:20]))
ok = flips and not bad and not defers
print("VERDICT: %s" % ("PASS" if ok else "FAIL"))
sys.exit(0 if ok else 1)
PY
