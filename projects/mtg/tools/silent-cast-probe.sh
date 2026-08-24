#!/usr/bin/env bash
# Wave-43 #W43-6 -- the silent-cast-drop instrument.
#
# ARRIVAL TRACE, not a counter: for every cast the GPT seat COMMITS through the
# alternative-cost menu ("Cast Card Normally" for an {X} spell), the seat must
# either reach the X announcement and put the spell on the stack, or leave a
# fallback record. A commitment that produces NEITHER is the defect.
#
# Runs one self-play game against tools/gpt-stub-server.py with --prefer, so
# the whole multi-step commitment (cast -> alternative menu -> X) is forced
# deterministically without an inference call.
#
# Usage (from projects/mtg/):  bash tools/silent-cast-probe.sh [deckN] [oppN]
# Probe decks 198/199 are NOT shipped; write them into bin/Res/ai/baka/ first
# and delete them after.
set -u

HERE="$(cd "$(dirname "$0")/.." && pwd)"
DECK0="${1:-198}"
DECK1="${2:-199}"
SPELL="${WAGIC_PROBE_SPELL:-Starstorm}"
PORT="${WAGIC_STUB_PORT:-8299}"
RUNHOME="$(mktemp -d "${WAGIC_PROBE_TMP:-$HOME}/silent-cast-probe.XXXXXX")"

python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" \
    --prefer "Cast $SPELL" --prefer "Cast Card Normally" --prefer "X = " &
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
python3 - "$RUNHOME" "$DECK0" "$SPELL" <<'PY'
import glob, json, os, sys
home, deck, spell = sys.argv[1], sys.argv[2], sys.argv[3]
logs = glob.glob(os.path.join(home, ".Wagic/ai/gpt/logs/*deck%s-*.jsonl" % deck))
if not logs:
    print("FAIL: no translog for the GPT seat"); sys.exit(1)
recs = [json.loads(l) for l in open(logs[0])]
recs.sort(key=lambda r: r.get("seq", 0))

# a COMMITMENT = the alternative-cost menu answered "Cast Card Normally"
commits = [r for r in recs if str(r.get("chosen_text", "")) == "Cast Card Normally"]
announces = [r for r in recs if "Announce the value of X" in str(r.get("prompt", ""))]
by_seq = {r.get("seq"): r for r in recs}

def cast_narrated_after(seq):
    """Did the NEXT record's narration report the spell going on the stack?"""
    later = [r for r in recs if r.get("seq", 0) > seq]
    if not later:
        return None  # no evidence either way (end of log)
    return ("You cast %s" % spell) in str(later[0].get("prompt", ""))

dropped = []
for c in commits:
    s = c.get("seq")
    nxt = [r for r in recs if r.get("seq", 0) > s]
    reached_x = bool(nxt) and "Announce the value of X" in str(nxt[0].get("prompt", ""))
    narrated = cast_narrated_after(s)
    if not reached_x and narrated is False:
        dropped.append(s)

print("cast commitments (Cast Card Normally) : %d" % len(commits))
print("ANNOUNCE_X windows                    : %d" % len(announces))
print("SILENT DROPS (no X window, no cast)   : %d  %s" % (len(dropped), dropped[:20]))
#every {X} commitment must produce its OWN announcement window - the ratio is
#the arrival trace, the drop list only catches the shape we already know.
ok = commits and not dropped and len(announces) >= len(commits)
print("VERDICT: %s" % ("PASS" if ok else "FAIL"))
sys.exit(0 if ok else 1)
PY
