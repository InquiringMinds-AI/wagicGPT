#!/usr/bin/env bash
# Wave-75 lane CI -- the MDFC LAND-DROP FACE fixture (P18).
#
# THE DEFECT IT PINS: the land drop was decided at TWO windows. The `Land drop:`
# menu named the CARD (with #W74-CD O4's "the next window asks which face" note),
# and one record later a second menu asked which face - `146v125` seq 3 -> 4,
# deck152 17 -> 18, deck126 23 -> 24 and 31 -> 32, deck130 29 -> 30, deck162
# 4 -> 5, deck123 4 -> 5 / 7 -> 8 / 32 -> 33 (9 repro pairs, 18 of one seat's 322
# decisions). Two model calls for one drop, and the second menu's own
# `Decline - do nothing` row could un-make a drop the first answer committed.
#
# WHY IT IS NOT A SUITE FIXTURE: the shape needs a seat that ASKS A MODEL - the
# suite has no endpoint, so its GPT seats fall back to the heuristic and neither
# window is ever put. tools/gpt-stub-server.py is the model (the same instrument
# lane CG and lane CH used), and it always takes row 1.
#
# READ THE VERDICT AS: on a binary carrying the defect the land-drop menu offers
# ONE row per pathway and a SECOND ask follows it naming the two faces; on a
# fixed binary the land-drop menu carries a FRONT row and a BACK row and no face
# menu is asked at all.
#
# Usage (from projects/mtg/):  bash tools/mdfc-face-probe.sh <binary> [seconds]
set -u

HERE="$(cd "$(dirname "$0")/.." && pwd)"
BIN="${1:?usage: mdfc-face-probe.sh <binary> [seconds]}"
SECS="${2:-180}"
PORT="${WAGIC_STUB_PORT:-8298}"
OUT="$(mktemp -d "${WAGIC_PROBE_TMP:-$HOME/.gatelogs}/mdfc-face-probe.XXXXXX")"

python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" \
    --answer "PLAN: play a land.
CHOICE: 1" --log "$OUT/asks.jsonl" &
STUB=$!
trap 'kill "$STUB" 2>/dev/null' EXIT
for _ in $(seq 1 40); do
    curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break
    sleep 0.25
done

cp "$BIN" "$HERE/bin/wagic-mdfcface"
( cd "$HERE/bin" && timeout -k 10 "${SECS}s" \
    systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
    env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
        WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
        WAGIC_SELFPLAY_DECK0="${WAGIC_PROBE_DECK0:-146}" \
        WAGIC_SELFPLAY_DECK1="${WAGIC_PROBE_DECK1:-125}" \
        WAGIC_AI=gpt WAGIC_GPT_URL="http://127.0.0.1:$PORT" WAGIC_GPT_MODEL=stub-model \
        WAGIC_GPT_KEY= WAGIC_GPT_THINKING=0 WAGIC_GPT_TIMEOUT=30 \
        ./wagic-mdfcface ) > "$OUT/game.stderr" 2>&1
rm -f "$HERE/bin/wagic-mdfcface"

python3 - "$OUT" <<'PY'
import json, os, sys
out = sys.argv[1]
asks = []
with open(os.path.join(out, "asks.jsonl"), errors="replace") as fh:
    for line in fh:
        try: r = json.loads(line)
        except Exception: continue
        msgs = (r.get("body") or {}).get("messages") or []
        asks.append(msgs[-1].get("content", "") if msgs else "")

# the FIRST window is the land-drop QUESTION, not the board line that also says
# "Land drop:" (the situation block prints "Land drop: ALREADY USED this turn").
landdrops = [a for a in asks if "Land drop: which land do you play now" in a
             or "Land drop: play " in a]
twoface   = [a for a in landdrops if "TWO FACES, ONE CARD" in a]
# the SECOND window: a menu whose rows are the two faces, offered as a card menu
# the SECOND window as the engine actually renders it: a CHOOSE_MENU headed
# "Choose an option for <card>:" whose rows name the card's other land face.
facemenus = [a for a in asks
             if "Choose an option for" in a and "OTHER FACE of" in a]
frontback = [a for a in twoface
             if "plays the FRONT face" in a and "plays the BACK face" in a]
print("model calls                 : %d" % len(asks))
print("land-drop windows           : %d" % len(landdrops))
print("...of them two-faced        : %d" % len(twoface))
print("...offering BOTH faces as rows: %d" % len(frontback))
print("SECOND (face) windows asked : %d" % len(facemenus))
ok = twoface and len(frontback) == len(twoface) and not facemenus
print("VERDICT                     : %s"
      % ("GREEN - one window, both faces" if ok else
         "RED - the face is asked at a second window" if facemenus else
         "INCONCLUSIVE - no two-faced land was drawn in this game"))
print("evidence: " + out)
PY
