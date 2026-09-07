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
#                             [-u URL] [-m MODEL] [-k KEY] --thinking on|off
#                             [--riggedhand] | --selftest
#
#W70-BK (C1, skill invariant 000(f)): --thinking on|off is REQUIRED and has NO
# default. Every corpus from wave 44 to wave 69 ran with reasoning OFF because
# this script's default was 0 and no launch recipe overrode it - the regime was
# never a decision anybody made, and nothing in the harvested data said which
# regime produced it. It is a stated argument now: a launch without it prints
# the ruling and exits 2, the regime is stamped on every translog record, and a
# per-regime GATE (tools/regime-gate.py) reads the first records of the first
# games and KILLS the corpus if the regime it was asked for is not the regime
# it is getting. The wave corpora run --thinking on ("fuckin obviously we need
# reasoning"); --thinking off is the PRODUCT regime and is validated when the
# owner asks for it.
#
# Hands are FAIR by default (owner ruling 2026-08-24, wave-44 lane A: "I want
# legitimate hands. legit mulligans. none of this fixing hands in advance."):
# the AI seats get a REAL random opening hand and run WITHOUT the
# agressivity+=100 / forceBestAbilityUse pins, so mulligan seams are live.
# --riggedhand exports WAGIC_SELFPLAY_FAIRHAND=0, the legacy-forensics escape
# back to the engine's old stacked AI-vs-AI hand (OptimizedHand 3 lands/1
# creature/3 spells) - NEVER for a corpus; waves <=43 were rigged and are not
# hand-comparable with wave 44+.
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
# #W54-F (D7c): NO-PROGRESS watchdog. The feasibility watchdog below measures
# LATENCY, and a HUNG engine produces none - wave-53's game 152v125 sat for ~13
# hours writing nothing at all and nothing fired, so the corpus finished 20/21
# and the ledger had to reconstruct the hang by hand. This is the complement:
# wall clock since either seat of a RUNNING game last wrote a translog record.
# It is a DEAD-LOOP detector, not a cap (invariant 00) - it never touches a game
# that is still deciding, however slowly, and the default clears the worst
# legitimate silence by a wide margin (a decision may cost the full
# WAGIC_GPT_TIMEOUT twice over, deadline + lane Q's one retry, and 420s is the
# largest deadline the harness ever sets). A game it stops is REPORTED and
# credited to NOBODY - like a crash, it owes a rerun.
NO_PROGRESS_S="${WAGIC_NO_PROGRESS_S:-3600}"
OUTDIR=""
URL="http://100.116.136.74:8081"   # Spark production port (8011 = serve.sh dev default)
MODEL="qwen35"
KEY=""
THINKING=""      #W70-BK (C1): no default. on|off, stated at launch.
FASTCLOCK=0.1   # game-seconds per engine tick; 0 = real-time pacing
FAIRHAND=1      # engine default; 0 = --riggedhand legacy-forensics escape

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
        --thinking) THINKING="${2:-}"; shift 2;;
        --selftest) exec python3 "$(dirname "$0")/regime-gate.py" --selftest;;
        --realtime) FASTCLOCK=0; shift;;
        --fairhand) FAIRHAND=1; shift;;
        --riggedhand) FAIRHAND=0; shift;;
        *) echo "unknown arg: $1" >&2; exit 2;;
    esac
done

#W70-BK (C1): THE REGIME IS A STATED THING. Refuse loudly rather than assume.
case "$THINKING" in
    on|off) : ;;
    *)
        cat >&2 <<'RULING'
FATAL: --thinking on|off is REQUIRED and has no default.

  Owner ruling (skill invariant 000): reasoning happens ONLY in the model's
  native reasoning channel, a plan precedes the action, and the reply is the
  PLAN line then the action line and nothing else. The wave corpora run with
  reasoning ON; thinking OFF is the product regime and is validated only when
  the owner asks for it. Either way the regime is a decision somebody MAKES.

  Every corpus from wave 44 to wave 69 ran with reasoning off because this
  harness defaulted to it and no launch recipe said otherwise. That default is
  gone. State the regime:

    tools/selfplay-harness.sh --thinking on  ...   (a wave corpus)
    tools/selfplay-harness.sh --thinking off ...   (the product regime)
RULING
        exit 2;;
esac
if [ "$THINKING" = "on" ]; then THINKING_ENV=1; else THINKING_ENV=0; fi

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
BIN="${WAGIC_BIN:-$HERE/bin/wagic}"   # W53: WAGIC_BIN overrides (ASAN corpus: bin-asan/wagic)
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
if [ "${WAGIC_AI:-}" = "baka" ]; then
  echo "WAGIC_AI=baka: heuristic-vs-heuristic corpus, skipping the endpoint probe" >&2
  PROBE_OUT="$MODEL"$'\n'"0.0"
else
PROBE_OUT="$(curl -s -m 20 -w '\n%{time_total}' "$URL/v1/models")"
fi
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
if [ "$THINKING" = "on" ]; then DEFAULT_GPT_TIMEOUT=420; else DEFAULT_GPT_TIMEOUT=240; fi
#W70-BK (C1): the regime in CAPITALS in the header - it is the first thing a
#reader of a corpus log has to be able to answer about it.
THINKING_UP=$(echo "$THINKING" | tr '[:lower:]' '[:upper:]')
echo "  model  : $MODEL @ $URL (thinking=$THINKING_UP, gpt timeout=${WAGIC_GPT_TIMEOUT:-$DEFAULT_GPT_TIMEOUT}s)"
echo "  regime : thinking=$THINKING_UP - $([ "$THINKING" = "on" ] && echo 'reasoning in the reasoning channel; every record must carry it' || echo 'PRODUCT regime; every reply must be exactly PLAN + action')"
echo "  caps   : ${TOTAL_CAP_S}s total, $([ "$GAME_TIMEOUT_S" = "0" ] && echo 'NO per-game cap' || echo "${GAME_TIMEOUT_S}s/game") (fastclock=$FASTCLOCK)"
echo "  hands  : $([ "$FAIRHAND" = "1" ] && echo 'FAIR (real deal, seats unpinned; wave 44+ baseline)' || echo 'RIGGED legacy (OptimizedHand 3/1/3 + agressivity pin) - forensics only, NOT a corpus')"
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
    fairhand_env=(WAGIC_SELFPLAY_FAIRHAND=$FAIRHAND)
    # #W54-F (D7c): run the game as a tracked child so the no-progress watchdog
    # can stop THIS game (and only this one) when it provably cannot finish. The
    # in-flight marker carries the pid and the decks/start the watchdog needs to
    # find this game's two seat translogs.
    local marker="$OUTDIR/.inflight-${d0}v${d2}-${gstart}"
    timeout -k 10 "${GAME_TIMEOUT_S}s" env \
        WAGIC_HEADLESS=1 "${fastclock_env[@]}" "${fairhand_env[@]}" \
        WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
        WAGIC_SELFPLAY_DECK0="$d0" WAGIC_SELFPLAY_DECK1="$d2" \
        WAGIC_AI=gpt WAGIC_GPT_URL="$URL" WAGIC_GPT_MODEL="$MODEL" WAGIC_GPT_KEY="$KEY" \
        WAGIC_GPT_THINKING="$THINKING_ENV" WAGIC_GPT_TRANSLOG=1 \
        WAGIC_GPT_TIMEOUT="${WAGIC_GPT_TIMEOUT:-$DEFAULT_GPT_TIMEOUT}" \
        ./wagic > "$elog" 2>&1 &
    local gpid=$!
    printf "%s %s %s %s\n" "$gpid" "$gstart" "$d0" "$d2" > "$marker"
    wait "$gpid"
    # W40 #16: the game's exit status is EVIDENCE and used to be thrown away.
    local rc=$?
    local hung=0
    [ -f "$marker.hung" ] && hung=1
    rm -f "$marker" "$marker.hung"
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
    # #W54-F (D7c): a game the no-progress watchdog stopped is a DEAD game, not
    # a close one. Life-adjudicating it would credit a seat for a position the
    # engine never played out - the same mistake the wave-39 crash rows made -
    # so it takes its own verdict and owes a rerun.
    [ "$hung" = "1" ] && winner="hang"
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
#W54-F (D7c): the NO-PROGRESS half. For every game still in flight, how long has
#it been since EITHER of its seats last wrote a translog record? A game deciding
#slowly writes; a game hung writes nothing at all. stderr is NOT usable as the
#progress signal - the 13-hour park kept printing "AttemptNew failed to load
#(404)" the whole time, so its stderr grew while the engine did not tick. Games
#with no translog yet (still loading the card DB) and Baka-only runs (which write
#none, ever) are exempt by construction: no record, no clock.
no_progress_sweep() {
    local now; now=$(date +%s)
    local m
    for m in "$OUTDIR"/.inflight-*; do
        [ -e "$m" ] || continue
        [ -e "$m.hung" ] && continue
        local gpid gstart gd0 gd1
        read -r gpid gstart gd0 gd1 < "$m" || continue
        [ -n "${gpid:-}" ] || continue
        local quiet
        quiet=$(python3 - "$LOGDIR" "$gstart" "$gd0" "$gd1" <<'NPY'
import glob, os, sys
logdir, gstart, d0, d1 = sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4]
newest = 0
for d in (d0, d1):
    for f in glob.glob(os.path.join(logdir, "*-ai_baka_deck%s-*.jsonl" % d)):
        try:
            ep = int(os.path.basename(f).split("-")[0])
        except ValueError:
            continue
        if not (gstart - 2 <= ep <= gstart + 300):
            continue
        try:
            newest = max(newest, int(os.path.getmtime(f)))
        except OSError:
            pass
print(-1 if not newest else newest)
NPY
)
        [ "${quiet:--1}" = "-1" ] && continue     # no seat log yet: nothing to time
        local since=$(( now - quiet ))
        [ "$since" -lt "$NO_PROGRESS_S" ] && continue
        echo ""
        echo "!! GAME HUNG: deck${gd0} vs deck${gd1} (started ${gstart}) has written NO seat"
        echo "!! translog record for ${since}s (limit ${NO_PROGRESS_S}s). The engine is not"
        echo "!! ticking - this is a dead loop, not a slow decision. Stopping THAT GAME only."
        echo "!! It is credited to no seat and owes a rerun; the rest of the corpus continues."
        echo "!! Evidence: $OUTDIR/game-${gd0}v${gd1}-${gstart}.stderr and the two seat logs."
        touch "$m.hung" "$OUTDIR/HUNG"
        kill $(pgrep -P "$gpid" 2>/dev/null) "$gpid" 2>/dev/null
    done
}

#W70-BK (C2): THE PER-REGIME GATE. Invariant 000(a): a corpus whose first
#records do not carry the regime's own evidence is INVALID - "stop it". This is
#that stop, automated, so it costs minutes instead of the eleven hours a wave-61
#corpus cost. It reads the FIRST records of the first seat logs this run wrote
#(tools/regime-gate.py, self-tested with --selftest) and asks the one question
#the regime turns on:
#   thinking on  -> every checked record carries reasoning (reasoning_chars > 0,
#                   no withheld trace, and a non-zero reasoning budget in the
#                   recorded max_tokens split - ruling 000(d));
#   thinking off -> no record carries reasoning text AND the replies are exactly
#                   a PLAN line plus an action line, with prose counted and
#                   aborted past WAGIC_CORPUS_PROSE_ABORT percent (default 5).
#Both arms also verify the regime STAMP on the record equals the regime asked
#for, which is the check that would have caught wave 44 the day it happened.
#W70-BN (F6/F7/F8/F9): a FAIL is terminal and never re-runs. A PASS is not -
#it is a statement about the seat logs seen SO FAR, remembered in
#$OUTDIR/regime-gate-state.txt, and every later sweep gates only the seat logs
#that have appeared since (their first five records). It is still not a monitor
#(fleet rule feedback-single-wake-batching): nothing wakes an agent, the work is
#five records per new file, and it stops as soon as the corpus does.
PROSE_ABORT="${WAGIC_CORPUS_PROSE_ABORT:-5}"
GATE_UNIT="${WAGIC_GATE_UNIT:-selfplay-harness}"
regime_gate_sweep() {
    [ -f "$OUTDIR/REGIME-GATE-DONE" ] && return 1
    #W70-BN (F8): a PASS is NOT a permanent amnesty. It names the seat logs it
    #checked, they are remembered in the state file, and every sweep keeps
    #gating the seat logs that have appeared since - a seat that starts late and
    #returns no reasoning still kills the corpus. The check is cheap: the first
    #five records of each not-yet-checked seat log.
    local verdict
    verdict=$(python3 "$HERE/tools/regime-gate.py" --logdir "$LOGDIR" --start "$START" \
                      --regime "$THINKING" --prose-abort "$PROSE_ABORT" \
                      --state "$OUTDIR/regime-gate-state.txt" 2>&1)
    case "$verdict" in
        PASS*)
            if [ ! -f "$OUTDIR/REGIME-GATE-PASSED" ]; then
                touch "$OUTDIR/REGIME-GATE-PASSED"
                echo ""
                echo "== REGIME GATE PASSED: ${verdict#PASS }"
            fi
            return 0;;
        WAIT*)
            #No verdict yet. If the corpus has been running long enough that
            #there SHOULD be records and there are none, that is its own answer.
            #W70-BN (F7/F8): only the NO-RECORDS wait is a deadline. A seat log
            #that is merely short of the minimum is still producing evidence, and
            #any bad record in it FAILs immediately on its own.
            case "$verdict" in
                *"no gateable records yet"*) ;;
                *) return 0;;
            esac
            #A PASS has been recorded: later sweeps gate only seat logs that
            #appeared since, and "no gateable records yet" then means "no NEW
            #seat log", not "the seats are not reaching the model". The
            #deadline below is for a corpus that has NEVER produced a record
            #(2026-09-06: the first thinking-on corpus PASSED at 210 records
            #and was killed by this deadline on the next sweep).
            [ -f "$OUTDIR/REGIME-GATE-PASSED" ] && return 0
            if [ $(( $(date +%s) - START )) -ge 1800 ]; then
                verdict="FAIL 30 minutes in and there are still no gateable decision records (${verdict#WAIT }) - the seats are not reaching the model."
            else
                return 0
            fi;;
    esac
    #FAIL. The corpus is invalid by the ruling; nothing downstream may read it
    #as evidence, so say so in every place a reader looks.
    touch "$OUTDIR/REGIME-GATE-DONE"   #a FAIL is terminal: no further sweeps
    local reason="${verdict#FAIL }"
    printf '%s\n' "$reason" > "$OUTDIR/REGIME-FAIL"
    mkdir -p "$HOME/.gatelogs"
    printf 'REGIME FAIL (thinking=%s): %s\n' "$THINKING" "$reason" \
        > "$HOME/.gatelogs/${GATE_UNIT}-REGIME-FAIL"
    [ -n "${WAGIC_DONE_FILE:-}" ] && printf 'FAILED regime gate (thinking=%s): %s\n' \
        "$THINKING" "$reason" >> "$WAGIC_DONE_FILE"
    echo ""
    echo "!! ================== REGIME GATE FAILED - CORPUS INVALID =================="
    echo "!! launched with --thinking $THINKING"
    echo "!! $reason"
    echo "!! Owner ruling (invariant 000): reasoning happens ONLY in the reasoning"
    echo "!! channel and the reply is the PLAN line then the action line. A corpus"
    echo "!! that is not in the regime it claims is not evidence of anything."
    echo "!! Killing all games now. Evidence: $OUTDIR and \$HOME/.gatelogs/${GATE_UNIT}-REGIME-FAIL"
    echo "!! ========================================================================"
    kill -TERM "$HARNESS_PID" 2>/dev/null
    return 1
}

supervisor() {
    while sleep 45; do
        no_progress_sweep
        regime_gate_sweep || return 1
        #Uncapped run: a full game can always fit, the latency projection has
        #nothing to violate - that half stands down (the sweep above does not).
        [ "$GAME_TIMEOUT_S" = "0" ] && continue
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
supervisor &
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
rm -f "$OUTDIR"/.inflight-*
#W70-BK (C2): the regime gate's verdict outlives the supervisor subshell.
if [ -f "$OUTDIR/REGIME-FAIL" ]; then
    echo "== CORPUS FAILED: regime gate (thinking=$THINKING). $(cat "$OUTDIR/REGIME-FAIL") =="
    echo "== The logs in $OUTDIR are NOT a corpus and must not be reviewed as one. =="
    exit 1
fi
if [ -f "$OUTDIR/INFEASIBLE" ]; then
    echo "== CORPUS FAILED: infeasible (see above). Partial logs in $OUTDIR are NOT a corpus. =="
    exit 1
fi

# #W54-F (D7c): a corpus that lost a game to a HANG is not a 21/21 corpus. Say so
# here, by matchup, so it cannot be read past - invariant 00 (a corpus completes
# full games or the test has failed). The good games are still harvested and are
# still evidence; what is NOT true is that the run is complete.
HUNG_ROWS=""
[ -f "$RESULTS" ] && HUNG_ROWS=$(/usr/bin/grep -P '\thang\t' "$RESULTS" 2>/dev/null || true)
if [ -n "$HUNG_ROWS" ]; then
    echo ""
    echo "!! ================= CORPUS INCOMPLETE: $(echo "$HUNG_ROWS" | wc -l) GAME(S) HUNG ================="
    echo "$HUNG_ROWS" | while read -r hd0 hd1 rest; do
        echo "!!   deck$hd0 vs deck$hd1 - engine stopped ticking, no seat credited, owes a rerun"
    done
    echo "!! A hang is an ENGINE defect, not a slow run. Read the game's .stderr and both"
    echo "!! seat translogs (the last record before the silence is the parked decision)."
    echo "!! =============================================================================="
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
wins = Counter(); games = Counter(); to = 0; adj = 0; crash = 0; hung = 0
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
    #W54-F (D7c): a hang credits nobody either, and unlike a timeout it is not a
    #play result at all - keep it out of the timeout bucket so the tally is true.
    elif w == "hang": hung += 1
    else: to += 1
print(f"\n== results ({sum(games.values())//2} games, {to} timeouts/draws, {adj} life-adjudicated at cap, {crash} CRASHED, {hung} HUNG - no winner, rerun owed) ==")
for d in sorted(games, key=lambda x:-(wins[x]/games[x] if games[x] else 0)):
    print(f"  deck{d:<4s} {wins[d]}/{games[d]} wins  ({100*wins[d]/games[d]:.0f}%)")
print(f"\nlogs + results.tsv in: {out}")
PY

#W54-F (D7c): a run that lost a game to a hang did not produce a complete corpus.
[ -f "$OUTDIR/HUNG" ] && exit 1
exit 0
