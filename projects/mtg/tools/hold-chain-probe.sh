#!/usr/bin/env bash
# Wave-74 lane CH -- the HOLD-UNDER-A-TRIGGER-CHAIN fixture.
#
# THE DEFECT IT PINS: the hold latch keyed on the RENDERED row, so every
# annotation that prices the board against a life total - the O9 own-clock tag,
# the O11 declined count, the O8 X-pricing bracket - re-opened the hold at every
# link of a trigger chain. Corpus 20260909-104713, game `125v126`: 41 casting
# windows in ONE upkeep, each a full model call, while that seat's own Sanguine
# Bond + Exquisite Blood chain drained the opponent 21 -> 1.
#
# WHY IT IS NOT A SUITE FIXTURE: the shape needs a seat that ASKS A MODEL and
# then TAKES THE HOLD ROW. The test suite has no endpoint, so its GPT seats fall
# back to the heuristic, `takeHold` is never called and the latch is never
# exercised (the same reason lane CG's window-loop fixture is a probe).
# tools/gpt-stub-server.py is the model: it casts the chain when it can and
# holds priority otherwise, which is exactly what the corpus seat did.
#
# READ THE VERDICT AS: whether the hold the model TOOK ever closes a window.
# A binary carrying the defect takes the hold, has it re-opened by the next
# link's annotations, and holds NOTHING; a fixed binary holds the whole chain.
#
# The probe decks live in tools/fixtures/w74-CH/ and are copied into
# Res/ai/baka only for the run, then deleted - the deck pool the corpus draws
# from is never left with a probe deck in it.
#
# Usage (from projects/mtg/):  bash tools/hold-chain-probe.sh <binary> [seconds]
set -u

HERE="$(cd "$(dirname "$0")/.." && pwd)"
BIN="${1:?usage: hold-chain-probe.sh <binary> [seconds]}"
SECS="${2:-300}"
PORT="${WAGIC_STUB_PORT:-8302}"
OUT="$(mktemp -d "${WAGIC_PROBE_TMP:-$HOME/.gatelogs}/hold-chain-probe.XXXXXX")"

python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" \
    --answer "PLAN: build the drain engine, then hold.
CHOICE: 1" \
    --prefer "Cast Sanguine Bond" --prefer "Cast Exquisite Blood" \
    --prefer "Cast Nyx-Fleece Ram" --prefer "Play " \
    --prefer "Hold priority" --log "$OUT/asks.jsonl" &
STUB=$!
DECKS="$HERE/bin/Res/ai/baka"
cp "$HERE/tools/fixtures/w74-CH/deck198.txt" "$HERE/tools/fixtures/w74-CH/deck199.txt" "$DECKS/"
trap 'kill "$STUB" 2>/dev/null; rm -f "$DECKS/deck198.txt" "$DECKS/deck199.txt"' EXIT
for _ in $(seq 1 40); do
    curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break
    sleep 0.25
done

cp "$BIN" "$HERE/bin/wagic-holdchain"
( cd "$HERE/bin" && timeout -k 10 "${SECS}s" \
    systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
    env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
        WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
        WAGIC_SELFPLAY_DECK0="${WAGIC_PROBE_DECK0:-198}" \
        WAGIC_SELFPLAY_DECK1="${WAGIC_PROBE_DECK1:-199}" \
        WAGIC_AI=gpt WAGIC_GPT_URL="http://127.0.0.1:$PORT" WAGIC_GPT_MODEL=stub-model \
        WAGIC_GPT_KEY= WAGIC_GPT_THINKING=0 WAGIC_GPT_TIMEOUT=30 \
        ./wagic-holdchain ) > "$OUT/game.stderr" 2>&1
rm -f "$HERE/bin/wagic-holdchain"

python3 - "$OUT" <<'PY'
import collections, json, os, re, sys
out = sys.argv[1]
HDR = re.compile(r"Casting decision \(([^,]+), ([^)]*)\)")
TURN = re.compile(r"=== Turn (\d+)")
per, calls = collections.Counter(), 0
with open(os.path.join(out, "asks.jsonl"), errors="replace") as fh:
    for line in fh:
        try: r = json.loads(line)
        except Exception: continue
        calls += 1
        msgs = (r.get("body") or {}).get("messages") or []
        text = msgs[-1].get("content", "") if msgs else ""
        m = HDR.search(text)
        if not m:
            continue
        turns = TURN.findall(text)
        per[(turns[-1] if turns else "?", m.group(1))] += 1
worst, where = (max(((v, k) for k, v in per.items())) if per else (0, None))
taken = reopened = held = 0
result = ""
HELD = re.compile(r"windows held by the model's own hold row: (\d+)")
with open(os.path.join(out, "game.stderr"), errors="replace") as fh:
    for line in fh:
        if "took the hold row at the cast seam" in line:
            taken += 1
        elif "hold re-opened at the cast seam" in line:
            reopened += 1
        elif line.startswith("WAGIC_SELFPLAY_RESULT"):
            result = line.strip()
        m = HELD.search(line)
        if m:
            held = max(held, int(m.group(1)))
print("model calls                      : %d" % calls)
print("cast-seam holds TAKEN            : %d" % taken)
print("cast-seam holds RE-OPENED        : %d" % reopened)
print("windows the hold actually HELD   : %d" % held)
print("casting decisions in one (turn, phase), worst: %d  %s" % (worst, where))
print("game finished                    : %s" % (result or "NO - the game never ended"))
# A hold the model TOOK that closes no window at all is the defect: every link
# of the chain re-opened it and the seat paid a model call per link.
print("VERDICT                          : %s"
      % ("RED - a hold was taken and held nothing" if (taken > 0 and held == 0)
         else ("GREEN" if taken > 0 else "INCONCLUSIVE - no hold was ever taken")))
print("evidence: " + out)
PY
