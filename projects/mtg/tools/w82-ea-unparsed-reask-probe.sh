#!/usr/bin/env bash
# #W82-EA (H7): the LIVE pin for the unreadable-reply RE-ASK lane at the ask and priority
# seams. `50v125` seq 41 (wave 81) fell to the heuristic on the FIRST unreadable reply with
# 891 s of deadline left; the lane now re-asks once and hands the window to the heuristic only
# when the re-ask is unreadable too (or the deadline is gone).
#
# Two stub games through the development-build reply stub (WAGIC_GPT_STUB, compiled out of
# release), both seats GPT, real decks 123 v 125:
#   recover: replies alternate unreadable / readable -> every ask/priority window re-asks once
#            and the re-ask is READ (fallback `unparsed_reask` on the first record, parse note
#            `unparsed_reask_recovered` on the next, no heuristic hand-off at those seams);
#   exhaust: every reply is unreadable -> the re-ask is unreadable too, the note says
#            `unparsed_reask_unanswered` and THEN the heuristic answers (one re-ask, never two).
# Usage: tools/w82-ea-unparsed-reask-probe.sh [seconds] [binary]   (run from projects/mtg)
set -u
LIMIT="${1:-300}"
MTG="$(cd "$(dirname "$0")/.." && pwd)"
BIN="${2:-$MTG/bin/wagic}"
LOGDIR="$HOME/.Wagic/ai/gpt/logs"
OUTDIR="${TMPDIR:-/tmp}/w82-ea-probe"; mkdir -p "$OUTDIR"
run_variant() {
    local name="$1" stub="$2"
    local out="$OUTDIR/$name.log"
    local before; before="$(ls -1 "$LOGDIR"/*.jsonl 2>/dev/null | wc -l)"
    ( cd "$MTG/bin" && env -u WAYLAND_DISPLAY -u DISPLAY \
        WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
        WAGIC_SELFPLAY_DECK0=123 WAGIC_SELFPLAY_DECK1=125 WAGIC_AI=gpt \
        WAGIC_GPT_URL=stub://offline WAGIC_GPT_MODEL=stub WAGIC_GPT_STUB="$stub" \
        WAGIC_GPT_TRANSLOG=1 \
        timeout -k 10 "$LIMIT" \
        systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
        "$BIN" > "$out" 2>&1 )
    echo "[$name] exit=$? $(grep -o 'WAGIC_SELFPLAY_RESULT.*' "$out")"
    local files; files="$(ls -1t "$LOGDIR"/*.jsonl | head -n $(( $(ls -1 "$LOGDIR"/*.jsonl | wc -l) - before )))"
    python3 - "$name" $files <<'PY'
import json, sys, collections
name, files = sys.argv[1], sys.argv[2:]
fb = collections.Counter(); notes = collections.Counter(); heur = collections.Counter(); seams = collections.Counter()
for f in files:
    for l in open(f, errors='replace'):
        try: r = json.loads(l)
        except Exception: continue
        if r.get('kind') == 'window' and r.get('seam') in ('ask', 'priority'):
            seams[r['seam']] += 1
            if r.get('fallback'): fb[(r['seam'], r['fallback'])] += 1
            n = r.get('parse_note') or ''
            for k in ('unparsed_reask_recovered', 'unparsed_reask_unanswered'):
                if k in n: notes[(r['seam'], k)] += 1
        if r.get('kind') == 'recovery' and r.get('recovers_kind') in ('ask', 'priority'):
            heur[(r.get('recovers_kind'), r.get('executed_by'))] += 1
print("[%s] seat logs %d; ask/priority windows %s" % (name, len(files), dict(seams)))
print("[%s] fallbacks %s" % (name, dict(fb)))
print("[%s] re-ask notes %s" % (name, dict(notes)))
print("[%s] recovery records (heuristic hand-offs) at ask/priority %s" % (name, dict(heur)))
PY
}
# The stub answers SYNCHRONOUSLY: the re-ask's launch call (pollCompletionRetry, which under
# the real transport only puts the corrected question in flight) already consumes a stub
# reply that the next tick's poll never sees, so a re-asked window costs THREE stub replies:
# the unreadable one, one burned by the launch, and the one the re-ask reads.
ALT="$(python3 -c "print('|'.join(['I will think about it.', 'burned by the launch', 'PLAN: develop.\\\\nCHOICE: 1 (x)'] * 200))")"
run_variant recover "$ALT"
run_variant exhaust "I will think about it."
echo "logs: $OUTDIR"
