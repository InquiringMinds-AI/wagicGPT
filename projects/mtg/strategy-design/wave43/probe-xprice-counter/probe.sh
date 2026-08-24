#!/usr/bin/env bash
# Wave-43 #W43-7 / #W43-8 -- the render-coverage instrument.
#
# Both features are pure RENDER, so the question a probe can answer is not "did
# the model play better" but "does the annotation actually reach the surface it
# was written for, in a real assembled prompt". One self-play game against the
# local stub endpoint (tools/gpt-stub-server.py) gives real prompt assembly,
# real HTTP, real parsing, real translog - deterministic answer, no inference.
#
# Recorded result (2026-08-24, 6 games, 400 decision records):
#   {X} cast lines priced 1.00 (18/18) | counter stack hits texted 1.00 (11/11)
#   | narration lines leaking an annotation 0.00 (0/738).
#
# Coverage fractions asserted on the resulting translogs:
#   * {X} cast option lines carrying "{X pricing:"        (target 1.00)
#   * counter option lines with a stack hit carrying
#     "{target text:"                                     (target 1.00)
#   * consumed-decision NARRATION lines carrying either
#     annotation                                          (target 0.00)
#
# Usage (from projects/mtg/): bash strategy-design/wave43/probe-xprice-counter/probe.sh
# The probe decks are NOT shipped: copy deck198.txt/deck199.txt into
# bin/Res/ai/baka/ first and DELETE them after.
set -u

HERE="$(cd "$(dirname "$0")/../../.." && pwd)"     # projects/mtg
DECK0="${1:-198}"
DECK1="${2:-199}"
PORT="${WAGIC_STUB_PORT:-8317}"
RUNHOME="$(mktemp -d /home/magi/w43-xprice-probe.XXXXXX)"

python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" &
STUB=$!
trap 'kill "$STUB" 2>/dev/null' EXIT
for _ in $(seq 1 40); do
    curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break
    sleep 0.25
done

cd "$HERE/bin" || exit 1
# A single game yields only a handful of {X} windows (2-4), which is too small a
# sample to distinguish full coverage from luck. Games are cheap here (no
# inference), so run several SEQUENTIALLY and aggregate every translog.
GAMES="${WAGIC_PROBE_GAMES:-6}"
for g in $(seq 1 "$GAMES"); do
    timeout 1800 systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
        env -u WAYLAND_DISPLAY -u DISPLAY \
        HOME="$RUNHOME" \
        WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
        WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
        WAGIC_SELFPLAY_DECK0="$DECK0" WAGIC_SELFPLAY_DECK1="$DECK1" \
        WAGIC_AI=gpt WAGIC_GPT_URL="http://127.0.0.1:$PORT" \
        WAGIC_GPT_MODEL=stub-model WAGIC_GPT_TRANSLOG=1 \
        ./wagic > "$RUNHOME/probe-$g.stdout" 2> "$RUNHOME/probe-$g.stderr"
    echo "game $g: $(tail -1 "$RUNHOME/probe-$g.stderr")"
done

echo "probe home: $RUNHOME"
python3 - "$RUNHOME" <<'PY'
import glob, json, os, re, sys
home = sys.argv[1]
logs = glob.glob(os.path.join(home, ".Wagic/ai/gpt/logs/*.jsonl"))
if not logs:
    print("FAIL: no translog written"); sys.exit(1)
recs = []
for p in logs:
    for line in open(p):
        line = line.strip()
        if line:
            recs.append(json.loads(line))

opt = re.compile(r"^\s*\d+\.\s")
x_total = x_priced = 0
c_total = c_texted = 0
narr_leak = narr_total = 0
sample_x = sample_c = None
for r in recs:
    prompt = r.get("prompt", "") or ""
    for line in prompt.split("\n"):
        if opt.match(line):
            low = line.lower()
            if re.match(r"^\s*\d+\.\s*Cast ", line) and "{x}" in low:
                x_total += 1
                if "{X pricing:" in line:
                    x_priced += 1
                elif sample_x is None:
                    sample_x = line
            if "can target on the stack:" in line:
                c_total += 1
                if "{target text:" in line:
                    c_texted += 1
                elif sample_c is None:
                    sample_c = line
        # Consumed decisions are narrated as plain history lines; neither
        # decision-time annotation may appear there.
        if line.startswith("You ") or line.startswith("Opponent "):
            narr_total += 1
            if "{X pricing:" in line or "{target text:" in line:
                narr_leak += 1

def frac(a, b):
    return "n/a" if not b else "%.2f (%d/%d)" % (a / b, a, b)

print("records                       : %d" % len(recs))
print("{X} cast lines priced         : %s" % frac(x_priced, x_total))
print("counter stack hits with text  : %s" % frac(c_texted, c_total))
print("narration lines leaking       : %s" % frac(narr_leak, narr_total))
if sample_x:
    print("UNPRICED SAMPLE: %s" % sample_x[:300])
if sample_c:
    print("UNTEXTED SAMPLE: %s" % sample_c[:300])
ok = x_total > 0 and x_priced == x_total and c_total > 0 and c_texted == c_total and narr_leak == 0
print("VERDICT: %s" % ("PASS" if ok else "FAIL"))
sys.exit(0 if ok else 1)
PY
