#!/usr/bin/env bash
# Wave-74 lane CG -- the WINDOW-LOOP fixture.
#
# THE DEFECT IT PINS: the decline row's O11 re-ask clause carries a count that
# rises with every answer. Both ask seams build their cache key from the rendered
# option list, so that count minted a FRESH question every window: the
# identical-window cache could never hit, the phase never advanced, and one
# turn-4 Main-1 casting window cost 384 full model calls in run 20260909-015553
# (11 of 21 games frozen at turns 3-15 after 8 h; the corpus was killed under
# invariant 00).
#
# WHY IT IS NOT A SUITE FIXTURE: the shape needs a seat that ASKS A MODEL. The
# test suite has no endpoint, so its GPT seats fall back to the heuristic and the
# ask cache is never exercised. tools/gpt-stub-server.py is the model: it always
# takes the plain decline row ("Cast nothing right now"), which is exactly what
# the corpus seat answered 384 times, so the loop is deterministic and free.
#
# READ THE VERDICT AS: a binary carrying the defect never finishes the game and
# walks the declined count into the thousands; a fixed binary finishes it and the
# count stays in single digits.
#
# Usage (from projects/mtg/):  bash tools/window-loop-probe.sh <binary> [seconds]
set -u

HERE="$(cd "$(dirname "$0")/.." && pwd)"
BIN="${1:?usage: window-loop-probe.sh <binary> [seconds]}"
SECS="${2:-240}"
PORT="${WAGIC_STUB_PORT:-8299}"
OUT="$(mktemp -d "${WAGIC_PROBE_TMP:-$HOME/.gatelogs}/window-loop-probe.XXXXXX")"

python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" \
    --answer "PLAN: hold and reassess.
CHOICE: 1" --prefer "Cast nothing right now" --log "$OUT/asks.jsonl" &
STUB=$!
trap 'kill "$STUB" 2>/dev/null' EXIT
for _ in $(seq 1 40); do
    curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break
    sleep 0.25
done

cp "$BIN" "$HERE/bin/wagic-windowloop"
( cd "$HERE/bin" && timeout -k 10 "${SECS}s" \
    systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
    env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
        WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
        WAGIC_SELFPLAY_DECK0="${WAGIC_PROBE_DECK0:-123}" \
        WAGIC_SELFPLAY_DECK1="${WAGIC_PROBE_DECK1:-126}" \
        WAGIC_AI=gpt WAGIC_GPT_URL="http://127.0.0.1:$PORT" WAGIC_GPT_MODEL=stub-model \
        WAGIC_GPT_KEY= WAGIC_GPT_THINKING=0 WAGIC_GPT_TIMEOUT=30 \
        ./wagic-windowloop ) > "$OUT/game.stderr" 2>&1
rm -f "$HERE/bin/wagic-windowloop"

python3 - "$OUT" <<'PY'
import json, os, re, sys
out = sys.argv[1]
decl = re.compile(r"declined this exact list (\d+) times? this turn")
worst = calls = 0
with open(os.path.join(out, "asks.jsonl"), errors="replace") as fh:
    for line in fh:
        try: r = json.loads(line)
        except Exception: continue
        calls += 1
        msgs = (r.get("body") or {}).get("messages") or []
        text = msgs[-1].get("content", "") if msgs else ""
        for m in decl.findall(text):
            worst = max(worst, int(m))
result = ""
with open(os.path.join(out, "game.stderr"), errors="replace") as fh:
    for line in fh:
        if line.startswith("WAGIC_SELFPLAY_RESULT"):
            result = line.strip()
print("model calls              : %d" % calls)
print("worst declined-list count: %d" % worst)
print("game finished            : %s" % (result or "NO - the game never ended"))
print("VERDICT                  : %s"
      % ("RED - the window loops" if (worst >= 40 or not result) else "GREEN"))
print("evidence: " + out)
PY
