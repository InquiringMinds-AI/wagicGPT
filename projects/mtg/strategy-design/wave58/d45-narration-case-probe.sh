#!/usr/bin/env bash
# #W58-D (D45) -- the narration casing instrument.
# One stub-server self-play game on a Pathway deck; counts every
# "- You/Opponent used: <label> with <Card>" line in the GPT seat's translog
# whose label is a LOWER-CASED card name (the D45 defect) versus the printed
# casing (the fix). No model, no inference call.
# Usage (from projects/mtg): bash strategy-design/wave58/d45-narration-case-probe.sh [deck0] [deck1]
set -u
HERE="$(cd "$(dirname "$0")/../.." && pwd)"
DECK0="${1:-152}"; DECK1="${2:-162}"
PORT="${WAGIC_STUB_PORT:-8297}"
RUNHOME="$(mktemp -d "${WAGIC_PROBE_TMP:-$HOME/.gatelogs}/d45-case.XXXXXX")"
python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" \
    --prefer "Pathway" --prefer "Play " &
STUB=$!
trap 'kill "$STUB" 2>/dev/null' EXIT
for _ in $(seq 1 40); do curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break; sleep 0.25; done
cd "$HERE/bin" || exit 1
timeout "${WAGIC_PROBE_TIMEOUT:-900}" systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
    env -u WAYLAND_DISPLAY -u DISPLAY HOME="$RUNHOME" \
    WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
    WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
    WAGIC_SELFPLAY_DECK0="$DECK0" WAGIC_SELFPLAY_DECK1="$DECK1" \
    WAGIC_AI=gpt WAGIC_GPT_URL="http://127.0.0.1:$PORT" \
    WAGIC_GPT_MODEL=stub-model WAGIC_GPT_TRANSLOG=1 \
    ./wagic > "$RUNHOME/probe.stdout" 2> "$RUNHOME/probe.stderr"
echo "probe home: $RUNHOME"
python3 - "$RUNHOME" "$HERE" <<'PY'
import glob, json, os, re, sys, collections
home, mtg = sys.argv[1], sys.argv[2]
names = set()
for f in glob.glob(os.path.join(mtg, "bin/Res/sets/primitives/*.txt")):
    for line in open(f, errors="replace", encoding="latin-1"):
        if line.startswith("name="):
            names.add(line[5:].strip().lower())
pat = re.compile(r"- (?:You|Opponent) used: (.*?)(?: with |$)")
low = collections.Counter(); up = collections.Counter()
logs = glob.glob(os.path.join(home, ".Wagic/ai/gpt/logs/*.jsonl"))
if not logs:
    print("FAIL: no translog"); sys.exit(1)
for lg in logs:
    for line in open(lg, errors="replace"):
        try: r = json.loads(line)
        except Exception: continue
        for fld in ("prompt", "events"):
            for m in pat.finditer(str(r.get(fld, ""))):
                lbl = m.group(1)
                if not lbl or lbl.lower() not in names: continue
                (low if lbl[0].islower() else up)[lbl] += 1
print("used-labels that ARE card names, LOWER-CASED (the defect):", sum(low.values()), dict(low))
print("used-labels that ARE card names, printed casing (the fix):", sum(up.values()), dict(up))
print("VERDICT:", "PASS" if not low else "FAIL")
sys.exit(0 if not low else 1)
PY
