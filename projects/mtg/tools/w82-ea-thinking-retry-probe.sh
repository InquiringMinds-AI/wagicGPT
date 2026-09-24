#!/usr/bin/env bash
# #W82-EA (H6): the LIVE pin, through the REAL transport (curl to a local stub), that the
# reasoning-budget overrun retry keeps thinking ON and raises the budget. The wave-81 corpus
# retried with `max_tokens_reasoning: 0` and `enable_thinking:false` (`125v152` seq 23,
# `125v162` seq 300) - the native reasoning channel switched off under a regime of on.
#
# tools/gpt-stub-server.py --reasoning-only-first answers the FIRST request of every distinct
# prompt as a phase-1 budget hit (empty content, a reasoning trace, finish_reason "length") and
# the retry normally. The stub's request log then shows what the retry SENT, and the seat's
# translog shows what the record SAYS. Two games: the default (thinking retry) and the legacy
# prefill close behind WAGIC_GPT_FORCECLOSE_PREFILL=1, as the contrast.
# Usage (from projects/mtg/): bash tools/w82-ea-thinking-retry-probe.sh [binary] [seconds]
set -u
HERE="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$(readlink -f "${1:-$HERE/bin/wagic}")"
SECS="${2:-240}"
PORT="${WAGIC_STUB_PORT:-8304}"
LOGDIR="$HOME/.Wagic/ai/gpt/logs"
OUT="$(mktemp -d "${WAGIC_PROBE_TMP:-$HOME/.gatelogs}/w82-ea-retry.XXXXXX")"
run_variant() {
    local name="$1"; shift
    python3 "$HERE/tools/gpt-stub-server.py" --port "$PORT" --reasoning-only-first \
        --answer "PLAN: develop.
CHOICE: 1" --log "$OUT/$name-asks.jsonl" &
    local stub=$!
    for _ in $(seq 1 40); do curl -sf "http://127.0.0.1:$PORT/v1/models" >/dev/null && break; sleep 0.25; done
    local before; before="$(ls -1 "$LOGDIR"/*.jsonl 2>/dev/null | wc -l)"
    ( cd "$HERE/bin" && timeout -k 10 "${SECS}s" \
        systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
        env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
            WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 WAGIC_SELFPLAY_DECK0=123 WAGIC_SELFPLAY_DECK1=125 \
            WAGIC_AI=gpt WAGIC_GPT_URL="http://127.0.0.1:$PORT" WAGIC_GPT_MODEL=stub-model \
            WAGIC_GPT_KEY= WAGIC_GPT_THINKING=1 WAGIC_GPT_REASONING_BUDGET=6000 WAGIC_GPT_TIMEOUT=60 \
            WAGIC_GPT_TRANSLOG=1 "$@" \
            "$BIN" ) > "$OUT/$name-game.stderr" 2>&1
    echo "[$name] exit=$? $(grep -o 'WAGIC_SELFPLAY_RESULT.*' "$OUT/$name-game.stderr")"
    kill "$stub" 2>/dev/null; wait "$stub" 2>/dev/null
    local files; files="$(ls -1t "$LOGDIR"/*.jsonl | head -n $(( $(ls -1 "$LOGDIR"/*.jsonl | wc -l) - before )))"
    python3 - "$name" "$OUT/$name-asks.jsonl" $files <<'PY'
import json, sys, collections
name, asks, files = sys.argv[1], sys.argv[2], sys.argv[3:]
# what the retry SENT: the second request of each distinct prompt
seen, retries = {}, collections.Counter()
for l in open(asks, errors='replace'):
    r = json.loads(l); b = r.get('body') or {}
    msgs = b.get('messages') or []
    key = json.dumps([(m.get('role'), m.get('content')) for m in msgs if m.get('role') != 'assistant'], sort_keys=True)
    if key in seen:
        retries[(('enable_thinking', (b.get('chat_template_kwargs') or {}).get('enable_thinking')),
                 ('prefill', any(m.get('role') == 'assistant' for m in msgs)),
                 ('continue_final_message', b.get('continue_final_message')),
                 ('max_tokens', b.get('max_tokens')))] += 1
    seen[key] = 1
print("[%s] retry requests SENT (shape -> count): %s" % (name, dict(retries) or 'none'))
# what the record SAYS
recs = collections.Counter(); n = 0
for f in files:
    for l in open(f, errors='replace'):
        try: r = json.loads(l)
        except Exception: continue
        if r.get('kind') == 'window' and r.get('retry'):
            n += 1
            recs[(('thinking', r.get('thinking')), ('retry_thinking', r.get('retry_thinking')),
                  ('max_tokens_reasoning', r.get('max_tokens_reasoning')),
                  ('reasoning_budget_hit', r.get('reasoning_budget_hit')),
                  ('reasoning_forced_close', r.get('reasoning_forced_close')),
                  ('phase1_reasoning_chars>0', (r.get('phase1_reasoning_chars') or 0) > 0),
                  ('reasoning_chars>0', (r.get('reasoning_chars') or 0) > 0),
                  ('choice>=1', (r.get('choice') or -1) >= 1))] += 1
print("[%s] retry RECORDS %d (shape -> count): %s" % (name, n, dict(recs) or 'none'))
PY
}
run_variant default
run_variant legacy WAGIC_GPT_FORCECLOSE_PREFILL=1
echo "out: $OUT"
