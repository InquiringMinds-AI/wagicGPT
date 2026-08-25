#!/usr/bin/env bash
# selfplay-harness.sh - run wagicGPT AI-vs-AI games headlessly, in bulk, over a
# POOL of decks as round-robin MATCHUPS (not mirrors), and harvest the
# per-decision translogs + per-game results for prompt/strategy testing.
#
# How it works:
#   - WAGIC_SELFPLAY=1 boots straight into an AI-vs-AI game (no menu nav).
#   - WAGIC_SELFPLAY_DECK0/1 pin the matchup (deck NUMBERS -> ai/baka/deckN.txt).
#   - WAGIC_SELFPLAY_ONESHOT=1 makes each process play EXACTLY ONE game, print
#     "WAGIC_SELFPLAY_RESULT winner=N" (0/1/-1), and exit cleanly. The translog
#     is durable per-decision, so nothing is lost.
#   - WAGIC_HEADLESS=1 runs each game with no window and no GL context at all:
#     no display, no GPU, no compositor involved. (This replaced the old
#     hidden-Hyprland-workspace trick.)
#   - WAGIC_FASTCLOCK strips the demo's real-time pacing (the AI otherwise
#     acts at most ~14 times/sec of wall time): each engine tick gets a fixed
#     game-time step, so games are bound by real work (inference) instead of
#     padding. On by default here; --realtime disables it.
# Because each game is its own short-lived process, the harness controls the
# matchup schedule directly: it builds the round-robin pairing list (each pair
# once per -r repetition), shuffles it, and runs JOBS games concurrently,
# recording winner + matchup for later win-rate analysis.
#
# WHY MATCHUPS, NOT MIRRORS: a deck-vs-itself corpus cannot separate universal
# strategy from deck-specific or opponent-overfit signal, and mirrors demand
# their own atypical strategy. Varied matchups over a curated hard-deck pool are
# the substrate the cold-check / guide work actually needs.
#
# Usage:
#   tools/selfplay-harness.sh [-p "44,135,140,..."] [-r REPS] [-j JOBS]
#                             [-t TOTAL_CAP_S] [-T GAME_TIMEOUT_S] [-o OUTDIR]
#                             [-u URL] [-m MODEL] [-k KEY] [--thinking]
#                             [--fairhand]
#
# --fairhand exports WAGIC_SELFPLAY_FAIRHAND=1 (docket #W44-1): the AI seats get
# a REAL random opening hand instead of the engine's stacked AI-vs-AI hand
# (Rules.cpp OptimizedHand 3 lands/1 creature/3 spells - 42/42 wave-43 corpus
# hands held exactly 3 lands and none was ever mulliganed), and the seats run
# WITHOUT the agressivity+=100 / forceBestAbilityUse pins. This is what makes
# mulligan teaches testable at all. It is OFF by default, and the default is an
# OWNER call that has not been made: a --fairhand corpus is NOT hand-comparable
# with the rigged-hand corpora waves 1-43 produced.
# Defaults: the locked 7-deck hard pool, 1 rep/pair, j=8, Spark qwen35 keyless.
#
# Run from projects/mtg (needs ./bin/wagic and ./bin/Res).
set -uo pipefail

# The locked guide-development pool (see project_wagicgpt memory): 5 high-ceiling
# decks the Baka AI fumbles (guide targets) + 2 Baka-friendly aggro (opponents).
#   44 Faerie Archmage, 135 Modern Snow, 140 Wipe Them Out, 131 Mind Control,
#   110 Etched Affinity  |  109 Hellrider, 133 Phyrexian Asphodel
POOL="44,135,140,131,110,109,133"
REPS=1
JOBS=8
TOTAL_CAP_S=86400        # 24h overall wall cap
# NO per-game cap by default (owner ruling 2026-08-21: "no cap" - a corpus game
# runs to its NATURAL end, full stop; a killed game is a failed test, and the
# 122B wave nearly lost games to a cap sized from an estimate). GNU timeout
# treats a 0 duration as disabled, so -T 0 = uncapped; pass -T SECONDS only for
# short probe runs where a runaway matters more than completeness.
GAME_TIMEOUT_S=0
OUTDIR=""
URL="http://100.116.136.74:8081"   # Spark production port (8011 = serve.sh dev default)
MODEL="qwen35"
KEY=""
THINKING=0
FASTCLOCK=0.1   # game-seconds per engine tick; 0 = real-time pacing
FAIRHAND=0      # #W44-1: 1 = un-rig the AI-vs-AI opening hand and the seat pins

while [ $# -gt 0 ]; do
    case "$1" in
        -p) POOL="$2"; shift 2;;
        -r) REPS="$2"; shift 2;;
        -j) JOBS="$2"; shift 2;;
        -t) TOTAL_CAP_S="$2"; shift 2;;
        -T) GAME_TIMEOUT_S="$2"; shift 2;;
        -o) OUTDIR="$2"; shift 2;;
        -u) URL="$2"; shift 2;;
        -m) MODEL="$2"; shift 2;;
        -k) KEY="$2"; shift 2;;
        --thinking) THINKING=1; shift;;
        --realtime) FASTCLOCK=0; shift;;
        --fairhand) FAIRHAND=1; shift;;
        *) echo "unknown arg: $1" >&2; exit 2;;
    esac
done
# Spark serves max-num-seqs 16 and its memory is flat under request load (KV is
# pre-allocated), and Magic is turn-based (~1 in-flight request per game), so 16
# concurrent games fill the batch. OVERSUBSCRIPTION (-j above 16) is allowed and
# deliberate: streams beyond 16 queue server-side, and the queue backfills batch
# slots the instant a game goes between-decisions - the server holds full batch
# with no bubbles. Aggregate tok/s is what wall clock divides by, so a saturated
# batch is the fastest corpus. Scale WAGIC_GPT_TIMEOUT with j (guard(j) =
# worst_case_tokens / p10_per_stream_tok_s(j) * 1.5) and set -T to corpus-length
# at high j: ALL games run at queue pace and finish together. Hard cap 64 is a
# runaway guard, not a tuning point.
[ "$JOBS" -gt 64 ] && { echo "capping -j to 64 (runaway guard)"; JOBS=64; }
[ "$JOBS" -lt 1 ] && JOBS=1

HERE="$(cd "$(dirname "$0")/.." && pwd)"   # projects/mtg
BIN="$HERE/bin/wagic"
LOGDIR="$HOME/.Wagic/ai/gpt/logs"
[ -x "$BIN" ] || { echo "no binary at $BIN (build first)" >&2; exit 1; }
mkdir -p "$LOGDIR"
[ -n "$OUTDIR" ] || OUTDIR="$HOME/.Wagic/ai/gpt/selfplay-runs/matchups-$(date +%Y%m%d-%H%M%S)"
mkdir -p "$OUTDIR"
RESULTS="$OUTDIR/results.tsv"
printf "deck0\tdeck1\twinner\tlife0\tlife1\tturn\tstart_epoch\n" > "$RESULTS"

# Endpoint reachability (fail HARD rather than burn a corpus of silent
# Baka-fallback games - that already happened once). Latency matters too: the
# in-game probe allows 20s, but a slow /v1/models means a struggling server.
PROBE_OUT="$(curl -s -m 20 -w '\n%{time_total}' "$URL/v1/models")"
PROBE_TIME="${PROBE_OUT##*$'\n'}"
if ! printf '%s' "$PROBE_OUT" | grep -q "$MODEL"; then
    echo "FATAL: $MODEL not reachable at $URL/v1/models - every game would fall back to the heuristic AI. Aborting." >&2
    exit 1
fi
case "$PROBE_TIME" in
    0.*|1.*) : ;;
    *) echo "WARN: $URL/v1/models answered in ${PROBE_TIME}s - the server is struggling; expect probe fallbacks under concurrency." >&2;;
esac
echo "  probe  : $MODEL ok at $URL (${PROBE_TIME}s)"

# Build the round-robin pairing schedule (each unordered pair once per rep),
# then shuffle so coverage is even if we hit the time cap early.
IFS=',' read -r -a DECKS <<< "$POOL"
JOBFILE="$(mktemp)"
for ((r=0; r<REPS; r++)); do
  for ((i=0; i<${#DECKS[@]}; i++)); do
    for ((j=i+1; j<${#DECKS[@]}; j++)); do
      printf "%s %s\n" "${DECKS[$i]}" "${DECKS[$j]}"
    done
  done
done | shuf > "$JOBFILE"
NGAMES=$(wc -l < "$JOBFILE")

# Snapshot existing translogs so we harvest only this run's.
BEFORE_LIST="$(mktemp)"
ls "$LOGDIR"/*.jsonl 2>/dev/null | sort > "$BEFORE_LIST"

echo "== selfplay harness (matchups) =="
echo "  pool   : $POOL"
echo "  games  : $NGAMES ($(( ${#DECKS[@]} * (${#DECKS[@]} - 1) / 2 )) pairings x $REPS reps), $JOBS concurrent"
# The HTTP timeout is the only watchdog that falls back to the heuristic, so it
# must clear the worst case a decision can take. With --thinking that is the
# two-phase budget path (6000-token thinking window since wave-35 + the
# forced-answer close, ~250-275s at ~30 tok/s); without it, the terse-reply
# default stands. 420s keeps margin over that rather than shaving it.
if [ "$THINKING" = "1" ]; then DEFAULT_GPT_TIMEOUT=420; else DEFAULT_GPT_TIMEOUT=240; fi
echo "  model  : $MODEL @ $URL (thinking=$THINKING, gpt timeout=${WAGIC_GPT_TIMEOUT:-$DEFAULT_GPT_TIMEOUT}s)"
echo "  caps   : ${TOTAL_CAP_S}s total, $([ "$GAME_TIMEOUT_S" = "0" ] && echo 'NO per-game cap' || echo "${GAME_TIMEOUT_S}s/game") (fastclock=$FASTCLOCK)"
echo "  hands  : $([ "$FAIRHAND" = "1" ] && echo 'FAIR (real deal, seats unpinned) - NOT comparable with rigged-hand corpora' || echo 'stock rigged AI-vs-AI (OptimizedHand 3/1/3 + agressivity pin)')"
echo "  outdir : $OUTDIR"

cd "$HERE/bin"
START=$(date +%s)
done_ct=0

run_one_game() {
    local d0="$1" d2="$2"
    local gstart; gstart=$(date +%s)
    local elog="$OUTDIR/game-${d0}v${d2}-${gstart}.stderr"
    local fastclock_env=()
    [ "$FASTCLOCK" != "0" ] && fastclock_env=(WAGIC_FASTCLOCK="$FASTCLOCK")
    local fairhand_env=()
    [ "$FAIRHAND" = "1" ] && fairhand_env=(WAGIC_SELFPLAY_FAIRHAND=1)
    timeout -k 10 "${GAME_TIMEOUT_S}s" env \
        WAGIC_HEADLESS=1 "${fastclock_env[@]}" "${fairhand_env[@]}" \
        WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
        WAGIC_SELFPLAY_DECK0="$d0" WAGIC_SELFPLAY_DECK1="$d2" \
        WAGIC_AI=gpt WAGIC_GPT_URL="$URL" WAGIC_GPT_MODEL="$MODEL" WAGIC_GPT_KEY="$KEY" \
        WAGIC_GPT_THINKING="$THINKING" WAGIC_GPT_TRANSLOG=1 \
        WAGIC_GPT_TIMEOUT="${WAGIC_GPT_TIMEOUT:-$DEFAULT_GPT_TIMEOUT}" \
        ./wagic > "$elog" 2>&1
    # W40 #16: the game's exit status is EVIDENCE and used to be thrown away.
    local rc=$?
    local resline; resline=$(grep -E 'WAGIC_SELFPLAY_RESULT winner=' "$elog" | tail -1)
    local winner life0 life1 turn
    winner=$(echo "$resline" | grep -oE 'winner=-?[0-9]+' | cut -d= -f2)
    life0=$(echo "$resline"  | grep -oE 'life0=-?[0-9]+'  | cut -d= -f2)
    life1=$(echo "$resline"  | grep -oE 'life1=-?[0-9]+'  | cut -d= -f2)
    turn=$(echo "$resline"   | grep -oE 'turn=-?[0-9]+'   | cut -d= -f2)
    [ -z "$winner" ] && winner="timeout"
    # W40 #16 (wave-39 ledger). A game that DIED - SIGSEGV/SIGABRT, core dumped -
    # emits no WAGIC_SELFPLAY_RESULT line either, so it used to fall straight
    # into the cap adjudicator below and be handed a "winner": the wave-39 row
    # `139 125 adj1 16 28 14` credited deck139 with a game deck125 was AHEAD in
    # 28-16 when the engine crashed. A crash is its own verdict. Life and turn
    # are still filled in (real evidence of where the game died) but NO seat may
    # be credited, and the matchup owes a rerun. GNU timeout reports 124 when
    # IT ended the game; anything else nonzero is the game dying on its own.
    local crashed=0
    if [ "$winner" = "timeout" ] && [ "$rc" -ne 0 ] && [ "$rc" -ne 124 ]; then crashed=1; fi
    if [ "$winner" = "timeout" ]; then
        # Adjudicate from the seat translogs' final records (wave-7 ledger 7a):
        # fill life/turn so control-mirror timeouts don't need manual
        # reconstruction. The gamestart header's opp_deck disambiguates
        # concurrent games that share a deck.
        local adj
        adj=$(python3 - "$LOGDIR" "$d0" "$d2" "$gstart" <<'PYEOF' 2>/dev/null
import json, glob, os, sys
logdir, d0, d2, gstart = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4])
def last_state(mine, other):
    cand = []
    for f in glob.glob(os.path.join(logdir, "*-ai_baka_deck%s-*.jsonl" % mine)):
        try:
            ep = int(os.path.basename(f).split('-')[0])
        except ValueError:
            continue
        if not (gstart - 2 <= ep <= gstart + 300):
            continue
        try:
            recs = [json.loads(l) for l in open(f) if l.strip()]
        except Exception:
            continue
        if not recs:
            continue
        gs = next((r for r in recs if r.get('kind') == 'gamestart'), None)
        if gs and ("deck%s" % other) not in (gs.get('opp_deck') or ''):
            continue
        cand.append(recs[-1])
    return max(cand, key=lambda r: r.get('seq', 0)) if cand else None
a = last_state(d0, d2)
if a:
    print(a.get('my_life', '-'), a.get('opp_life', '-'), a.get('turn', '-'))
else:
    b = last_state(d2, d0)
    if b:
        print(b.get('opp_life', '-'), b.get('my_life', '-'), b.get('turn', '-'))
    else:
        print('- - -')
PYEOF
)
        read -r life0 life1 turn <<< "$adj"
        # Adjudicate the cap by life (wave-9 ledger #4): every wave-9 timeout
        # was a latency-starved control mirror that was AHEAD or even at the
        # cap - "timeout" as an undifferentiated loss made the win table lie.
        # The ahead seat takes an adjudicated win; ties stay timeout/draw.
        if [ "$crashed" = "0" ] && [ -n "$life0" ] && [ -n "$life1" ] && [ "$life0" != "-" ] && [ "$life1" != "-" ]; then
            if [ "$life0" -gt "$life1" ] 2>/dev/null; then winner="adj0"
            elif [ "$life1" -gt "$life0" ] 2>/dev/null; then winner="adj1"
            fi
        fi
        [ "$crashed" = "1" ] && winner="crash(rc=$rc)"
    fi
    printf "%s\t%s\t%s\t%s\t%s\t%s\t%s\n" "$d0" "$d2" "$winner" "${life0:--}" "${life1:--}" "${turn:--}" "$gstart" >> "$RESULTS"
}

trap 'echo "stopping..."; for p in $(jobs -p); do kill $(pgrep -P "$p") "$p" 2>/dev/null; done' INT TERM

# FEASIBILITY WATCHDOG (owner ruling 2026-08-20): a corpus completes a FULL game for
# every matchup or the test has FAILED - truncated games are a wrong test, not a
# partial result. Once 15 real decisions exist in this run's translogs, project
# median-decision-latency x expected-decisions-per-game; if that exceeds -T, a full
# game cannot fit: kill the whole corpus NOW and exit loudly, instead of burning
# hours producing 21 cap-adjudications. Override the per-game decision estimate with
# WAGIC_CORPUS_DECISIONS (default 130, from smoke/corpus history).
EXPECTED_DECISIONS="${WAGIC_CORPUS_DECISIONS:-130}"
HARNESS_PID=$$
feasibility_watchdog() {
    #Uncapped run: a full game can always fit, the projection has nothing to
    #violate - the watchdog stands down.
    [ "$GAME_TIMEOUT_S" = "0" ] && return 0
    while sleep 45; do
        local verdict
        verdict=$(python3 - "$LOGDIR" "$START" "$EXPECTED_DECISIONS" "$GAME_TIMEOUT_S" <<'WPY'
import json, glob, os, sys
logdir, start, dec, cap = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])
lat = []
for f in glob.glob(os.path.join(logdir, "*.jsonl")):
    try:
        ep = int(os.path.basename(f).split("-")[0])
    except ValueError:
        continue
    if ep < start - 5:
        continue
    for line in open(f):
        try: r = json.loads(line)
        except Exception: continue
        if r.get("latency_ms"):
            lat.append(r["latency_ms"] / 1000.0)
if len(lat) < 15:
    print("WAIT"); sys.exit()
lat.sort()
med = lat[len(lat)//2]
proj = med * dec
print(f"INFEASIBLE {med:.0f} {proj:.0f}" if proj > cap else f"OK {med:.0f} {proj:.0f}")
WPY
)
        case "$verdict" in
            INFEASIBLE*)
                set -- $verdict
                echo ""
                echo "!! CORPUS INFEASIBLE: median decision latency ${2}s -> projected full game ~${3}s > -T ${GAME_TIMEOUT_S}s."
                echo "!! Games CANNOT complete. A corpus that cannot finish full games is a FAILED test (owner ruling 2026-08-20)."
                echo "!! Killing all games. Fix throughput (serve config / lower -j) or raise -T before relaunching."
                touch "$OUTDIR/INFEASIBLE"
                kill -TERM "$HARNESS_PID" 2>/dev/null
                return 1;;
            OK*) : ;;
        esac
    done
}
feasibility_watchdog &
WATCHDOG_PID=$!

while read -r d0 d1; do
    # Overall time cap.
    if [ $(( $(date +%s) - START )) -ge "$TOTAL_CAP_S" ]; then
        echo "hit total time cap with $done_ct/$NGAMES games done"; break
    fi
    # Throttle to JOBS concurrent.
    while [ "$(jobs -rp | /usr/bin/grep -cv "^$WATCHDOG_PID\$")" -ge "$JOBS" ]; do wait -n 2>/dev/null || sleep 2; done
    run_one_game "$d0" "$d1" &
    sleep 2   # stagger startup so the card-DB loads don't thundering-herd
    done_ct=$((done_ct+1))
    echo "  launched $done_ct/$NGAMES: deck$d0 vs deck$d1  ($(jobs -rp | /usr/bin/grep -cv "^$WATCHDOG_PID\$") running, $(( $(date +%s)-START ))s elapsed)"
done < "$JOBFILE"

echo "waiting for in-flight games to finish..."
wait $(jobs -p | /usr/bin/grep -v "^$WATCHDOG_PID$") 2>/dev/null
kill "$WATCHDOG_PID" 2>/dev/null
trap - INT TERM
if [ -f "$OUTDIR/INFEASIBLE" ]; then
    echo "== CORPUS FAILED: infeasible (see above). Partial logs in $OUTDIR are NOT a corpus. =="
    exit 1
fi

# Harvest this run's translogs.
comm -13 "$BEFORE_LIST" <(ls "$LOGDIR"/*.jsonl 2>/dev/null | sort) | while read -r f; do
    cp "$f" "$OUTDIR/"
done
rm -f "$BEFORE_LIST" "$JOBFILE"

# Summary: decision kinds + win tally.
python3 - "$OUTDIR" "$RESULTS" <<'PY'
import json, glob, os, sys
from collections import Counter
out, res = sys.argv[1], sys.argv[2]
files = glob.glob(os.path.join(out, "*.jsonl"))
kinds = Counter(); n = 0
for f in files:
    for line in open(f):
        try: r = json.loads(line)
        except: continue
        kinds[r.get("kind","?")] += 1; n += 1
print(f"\n== harvested {len(files)} player-game logs, {n} decisions ==")
for k, c in kinds.most_common(): print(f"  {k:10s} {c}")
# win tally per deck
wins = Counter(); games = Counter(); to = 0; adj = 0; crash = 0
for i, line in enumerate(open(res)):
    if i == 0: continue
    p = line.rstrip("\n").split("\t")
    if len(p) < 3: continue
    d0, d1, w = p[0], p[1], p[2]
    games[d0]+=1; games[d1]+=1
    if w == "0": wins[d0]+=1
    elif w == "1": wins[d1]+=1
    elif w == "adj0": wins[d0]+=1; adj += 1
    elif w == "adj1": wins[d1]+=1; adj += 1
    # W40 #16: a crashed game credits NOBODY and is not a timeout either - it is
    # a missing result the wave owes a rerun for (completeness invariant).
    elif w.startswith("crash"): crash += 1
    else: to += 1
print(f"\n== results ({sum(games.values())//2} games, {to} timeouts/draws, {adj} life-adjudicated at cap, {crash} CRASHED - no winner, rerun owed) ==")
for d in sorted(games, key=lambda x:-(wins[x]/games[x] if games[x] else 0)):
    print(f"  deck{d:<4s} {wins[d]}/{games[d]} wins  ({100*wins[d]/games[d]:.0f}%)")
print(f"\nlogs + results.tsv in: {out}")
PY
