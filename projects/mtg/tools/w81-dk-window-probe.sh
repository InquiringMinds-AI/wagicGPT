#!/usr/bin/env bash
# #W81-DK: the LIVE pin for V1 (a state-based-action window is never served from the
# ask cache), V3 (a hold re-open invalidates the retained answer for that window) and
# V4 (the casting decision is per (phase, STACK STATE)).
#
# WHY A SCRIPT AND NOT A SUITE FIXTURE: the test harness's AI seat is
# `TestSuiteAI : AIPlayerBaka` (include/TestSuiteAI.h) and the `[AI]` command only
# flips `playMode` on those objects - NO suite fixture can reach AIPlayerGPT's seams.
# A fixture on any of these boards exercises the HEURISTIC, the component next to the
# defect, and is green before and after the fix. This probe drives the real callers
# (`chooseOrderedAction`, `askModel`, `chooseTarget`'s legend-rule branch and the hold
# latch) offline and deterministically, through the development-build reply stub
# (`WAGIC_GPT_STUB`, compiled out of release).
#
# The deck stacks the three boards into one game:
#   * Nadaar, Selfless Paladin x12  - the LEGEND RULE (a state-based action) fires as
#     soon as a second copy lands, and again on every later copy: V1's window.
#   * Essence Scatter / Path to Exile x8 each - instants, so a casting decision
#     answered over an empty stack is followed by windows with an object ON the stack:
#     V4's window.
#   * The stub takes the HOLD row when one is offered, which arms the hold latch and
#     lets an incoming spell re-open it: V3's window.
#
# Read the counters off the gameend record of the harvested translog:
#   sba_windows_cache_bypassed          (V1) > 0  with cache_bypass_reason on records
#   hold_reopen_answer_invalidated      (V3) > 0  with answer_invalidated on hold_events
#   cast_decision_reopened_new_stack    (V4) > 0  with cast_reopen_reason on records
#   verdict_faces_dropped_unrecorded    (V9) the residue of the exact census
#
# Usage: tools/w81-dk-window-probe.sh [seconds] [binary]   (run from projects/mtg)
set -u
LIMIT="${1:-300}"
MTG="$(cd "$(dirname "$0")/.." && pwd)"
BIN="${2:-$MTG/bin/wagic}"
RES="$MTG/bin/Res/ai/baka"
OUT="${TMPDIR:-/tmp}/w81-dk-probe.log"
LOGDIR="$HOME/.Wagic/ai/gpt/logs"

cleanup() { rm -f "$RES/deck196.txt" "$RES/deck197.txt" \
                  "$RES/deck196_strategy.txt" "$RES/deck197_strategy.txt"; }
trap cleanup EXIT

cat > "$RES/deck196.txt" <<'EOD'
#NAME:W81DK Windows
#DESC:probe: legend rule (SBA) + instants on the stack + a hold row
Nadaar, Selfless Paladin (AFR) *12
Essence Scatter (*) *8
Path to Exile (*) *8
Plains *16
Island *16
EOD
sed 's/W81DK Windows/W81DK Windows B/' "$RES/deck196.txt" > "$RES/deck197.txt"
cp "$RES/deck123_strategy.txt" "$RES/deck196_strategy.txt" 2>/dev/null
cp "$RES/deck123_strategy.txt" "$RES/deck197_strategy.txt" 2>/dev/null

# row 1 for a while (cast / activate), then the LAST row (the hold / decline row is
# always last) so the hold latch arms, then pass for ever.
STUB="$(python3 - <<'PY'
rows = ['PLAN: develop the board.\\nCHOICE: 1 (x)'] * 40
rows += ['PLAN: hold here.\\nCHOICE: 2 (x)'] * 40
rows += ['PLAN: stop.\\nCHOICE: 0 (pass)']
print('|'.join(rows), end='')
PY
)"

BEFORE="$(ls -1 "$LOGDIR" 2>/dev/null | wc -l)"
cd "$MTG/bin" || exit 1
env -u WAYLAND_DISPLAY -u DISPLAY \
    WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
    WAGIC_SELFPLAY_DECK0=196 WAGIC_SELFPLAY_DECK1=197 WAGIC_AI=gpt \
    WAGIC_GPT_URL=stub://offline WAGIC_GPT_MODEL=stub WAGIC_GPT_STUB="$STUB" \
    WAGIC_GPT_TRANSLOG=1 \
    timeout -k 10 "$LIMIT" \
    systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
    "$BIN" > "$OUT" 2>&1
rc=$?

echo "binary=$BIN exit=$rc (124 = the game never ended)"
for s in "STUB reply" "state-based-action window" "the retained answer for that window is invalidated" \
         "took the hold row" "hold re-opened" "WAGIC_SELFPLAY_RESULT"; do
    printf '%-58s %s\n' "$s" "$(grep -c "$s" "$OUT")"
done
grep "WAGIC_SELFPLAY_RESULT" "$OUT"
echo "log: $OUT ; new translogs: $(( $(ls -1 "$LOGDIR" 2>/dev/null | wc -l) - BEFORE ))"
