#!/usr/bin/env bash
# #W81-DI: the cached-replay bound's LIVE pin.
#
# The wave-80 corpus's HUNG game (`162v123`, matchups-20260912-074153) spent
# 3,641 s in an engine dead loop: Thraben Doomsayer taps for a Human token,
# Intruder Alarm untaps it, the priority menu, the target identities and the
# untapped-source count are all byte-identical, so the ask key is unchanged and
# `AIPlayerGPT::chooseOrderedAction` replays `mLastChoice` with no round trip -
# 3,659 times, with the model asked exactly zero times after the one window that
# answered it.
#
# WHY THIS IS A SCRIPT AND NOT A SUITE FIXTURE: the test harness's AI seat is
# `TestSuiteAI : AIPlayerBaka` (include/TestSuiteAI.h) and `[AI]` only flips
# `playMode` on those objects, so NO suite fixture can reach AIPlayerGPT's seams
# at all. A fixture on this board would exercise the heuristic - the component
# NEXT TO the defect - and would be green before and after the fix. This probe
# drives the real seam instead, deterministically and offline, through the
# development-build reply stub (`WAGIC_GPT_STUB`, compiled out of release).
#
# The stub answers `CHOICE: 1` for its first 120 calls and passes for ever after,
# so the seat activates a BOUNDED number of times of its own accord. The two legs
# then separate cleanly:
#   BASE  - the cache replays the activation with no further model call: measured
#           14 stub calls, 916 activations, 914 `menu is unchanged after action
#           N but the board moved` and exit 124 - THE GAME NEVER ENDS.
#   FIXED - the token the row itself made moves the legal-continuation digest, so
#           the key is not unchanged and the window is ASKED; anything that
#           re-keys equal is replayed once and then re-asked. `menu is unchanged
#           after action` collapses to ~0, the stub's list runs out, the seat
#           passes, and the game ENDS with a winner (exit 0).
# The board is shuffled, so the loop does not form in every game - a BASE leg that
# exits 0 with `menu is unchanged after action` at 0 simply never built the combo;
# re-run it. A FIXED leg must never exit 124.
#
# Usage: tools/w81-di-alarm-loop-probe.sh [seconds]   (run from projects/mtg)
set -u
LIMIT="${1:-300}"
MTG="$(cd "$(dirname "$0")/.." && pwd)"
RES="$MTG/bin/Res/ai/baka"
OUT="${TMPDIR:-/tmp}/w81-di-probe.log"

cleanup() { rm -f "$RES/deck198.txt" "$RES/deck199.txt" \
                  "$RES/deck198_strategy.txt" "$RES/deck199_strategy.txt"; }
trap cleanup EXIT

cat > "$RES/deck198.txt" <<'EOD'
#NAME:W81DI Alarm Loop
#DESC:probe: Thraben Doomsayer + Intruder Alarm
Thraben Doomsayer (C20) *10
Intruder Alarm (WOT) *10
Plains *20
Island *20
EOD
sed 's/W81DI Alarm Loop/W81DI Alarm Loop B/' "$RES/deck198.txt" > "$RES/deck199.txt"
cp "$RES/deck123_strategy.txt" "$RES/deck198_strategy.txt" 2>/dev/null
cp "$RES/deck123_strategy.txt" "$RES/deck199_strategy.txt" 2>/dev/null

STUB="$(python3 -c "print('|'.join(['PLAN: build the board.\\\\nCHOICE: 1 (x)']*120
                                   +['PLAN: stop here.\\\\nCHOICE: 0 (pass)']),end='')")"

cd "$MTG/bin" || exit 1
env -u WAYLAND_DISPLAY -u DISPLAY \
    WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
    WAGIC_SELFPLAY_DECK0=198 WAGIC_SELFPLAY_DECK1=199 WAGIC_AI=gpt \
    WAGIC_GPT_URL=stub://offline WAGIC_GPT_MODEL=stub WAGIC_GPT_STUB="$STUB" \
    timeout -k 10 "$LIMIT" \
    systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
    ./wagic > "$OUT" 2>&1
rc=$?

echo "exit=$rc (124 = the game never ended: RED)"
for s in "STUB reply" "Using Activated ability" "menu is unchanged after action" \
         "cached answer" "WAGIC_SELFPLAY_RESULT"; do
    printf '%-34s %s\n' "$s" "$(grep -c "$s" "$OUT")"
done
grep "WAGIC_SELFPLAY_RESULT" "$OUT"
echo "log: $OUT"
[ "$rc" -eq 0 ] || exit 1
