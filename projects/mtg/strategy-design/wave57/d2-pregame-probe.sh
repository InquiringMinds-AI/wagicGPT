#!/usr/bin/env bash
# #W57-E (D2) -- the pregame MDFC-land header instrument.
# Runs N one-shot self-play games (deck146 holds 4 MDFCs) against the stub
# endpoint and reports, for every mulligan record, the header's land count and
# whether the dual-role note fired, beside the hand line the narration prints.
set -u
HERE="$(cd "$(dirname "$0")/../.." && pwd)"
DECK0="${1:-146}"; DECK1="${2:-125}"; REPS="${3:-6}"
PORT="${WAGIC_STUB_PORT:-8307}"
RUNHOME="$HOME/.gatelogs/w57E-d2probe"
rm -rf "$RUNHOME"; mkdir -p "$RUNHOME"
python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" --answer "CHOICE: 1" &
STUB=$!
trap 'kill "$STUB" 2>/dev/null' EXIT
for _ in $(seq 1 40); do curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break; sleep 0.25; done
cd "$HERE/bin" || exit 1
for i in $(seq 1 "$REPS"); do
  timeout 300 systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
    env -u WAYLAND_DISPLAY -u DISPLAY HOME="$RUNHOME" \
    WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
    WAGIC_SELFPLAY_DECK0="$DECK0" WAGIC_SELFPLAY_DECK1="$DECK1" \
    WAGIC_AI=gpt WAGIC_GPT_URL="http://127.0.0.1:$PORT" \
    WAGIC_GPT_MODEL=stub-model WAGIC_GPT_TRANSLOG=1 \
    ./wagic > "$RUNHOME/g$i.stdout" 2> "$RUNHOME/g$i.stderr"
done
python3 - "$RUNHOME" <<'PY'
import glob, json, os, sys, re
home = sys.argv[1]
logs = sorted(glob.glob(os.path.join(home, ".Wagic/ai/gpt/logs/*.jsonl")))
mdfc = ("agadeem", "emeria's call", "pelakka", "bala ged", "akoum", "kazuul", "silundi",
        "sea gate", "malakir", "turntimber", "beyeen", "song-mad", "glasspool", "shatterskull",
        "branchloft", "hengegate", "clearwater", "cragcrown", "barkchannel", "brightclimb",
        "needleverge", "riverglide", "blightstep", "darkbore")
tot = dual = 0
for f in logs:
    for line in open(f, errors="replace"):
        try: r = json.loads(line)
        except: continue
        p = r.get("prompt", "")
        m = re.search(r"Your hand \(\d+ cards\), counted by the engine: [^\n]*", p)
        if not m: continue
        hand = re.search(r"Your opening hand \([^)]*\): ([^\n]*)", p)
        hl = (hand.group(1) if hand else "").lower()
        has = any(k in hl for k in mdfc)
        if not has: continue
        tot += 1
        note = "counted TWICE above" in p
        dual += 1 if note else 0
        print("%-28s %s | note=%s" % (os.path.basename(f)[:28], m.group(0), note))
        print("     hand: %s" % (hand.group(1) if hand else "?"))
print("MDFC-holding pregame prompts: %d ; carrying the dual-role note: %d" % (tot, dual))
PY
