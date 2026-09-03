set -uo pipefail
H=/home/magi/Projects/wagicGPT/worktrees/lanes/w54-F/projects/mtg/tools/selfplay-harness.sh
W=/home/magi/.gatelogs/w54F-wd
rm -rf "$W"; mkdir -p "$W/out" "$W/logs"
OUTDIR="$W/out"; LOGDIR="$W/logs"; NO_PROGRESS_S=1800
# extract the function under test verbatim from the shipped harness
sed -n '/^no_progress_sweep() {/,/^}$/p' "$H" > "$W/fn.sh"
[ -s "$W/fn.sh" ] || { echo "EXTRACT FAILED"; exit 1; }
. "$W/fn.sh"

run_case() {  # $1=label $2=age_seconds $3=expect_kill(0/1)
  local label="$1" age="$2" expect="$3"
  rm -f "$OUTDIR"/.inflight-* "$OUTDIR/HUNG"
  local gs; gs=$(date +%s)
  : > "$LOGDIR/${gs}-ai_baka_deck44-0xaa.jsonl"
  : > "$LOGDIR/${gs}-ai_baka_deck135-0xbb.jsonl"
  touch -d "@$(( gs - age ))" "$LOGDIR/${gs}-ai_baka_deck44-0xaa.jsonl" "$LOGDIR/${gs}-ai_baka_deck135-0xbb.jsonl"
  sleep 300 & local p=$!
  printf "%s %s %s %s\n" "$p" "$gs" 44 135 > "$OUTDIR/.inflight-44v135-${gs}"
  no_progress_sweep > "$W/out.txt" 2>&1
  sleep 1
  local killed=0; kill -0 "$p" 2>/dev/null || killed=1
  kill "$p" 2>/dev/null
  local marked=0; [ -f "$OUTDIR/.inflight-44v135-${gs}.hung" ] && marked=1
  local flagged=0; [ -f "$OUTDIR/HUNG" ] && flagged=1
  if [ "$killed" = "$expect" ] && [ "$marked" = "$expect" ] && [ "$flagged" = "$expect" ]; then
    echo "PASS  $label (killed=$killed marked=$marked flagged=$flagged)"
  else
    echo "FAIL  $label (killed=$killed marked=$marked flagged=$flagged, expected $expect)"; cat "$W/out.txt"
  fi
  rm -f "$LOGDIR"/*.jsonl
}

run_case "quiet 2h with a limit of 1800s -> STOPPED and reported" 7200 1
run_case "quiet 900s (a deadline+retry could cost that) -> LEFT ALONE" 900 0
run_case "quiet 1799s, one second inside the limit -> LEFT ALONE" 1799 0
# no translog at all: still loading the card DB, or a Baka run that writes none
rm -f "$OUTDIR"/.inflight-* "$OUTDIR/HUNG"; gs=$(date +%s); sleep 300 & p=$!
printf "%s %s %s %s\n" "$p" "$gs" 44 135 > "$OUTDIR/.inflight-44v135-${gs}"
no_progress_sweep >/dev/null 2>&1; sleep 1
if kill -0 "$p" 2>/dev/null && [ ! -f "$OUTDIR/HUNG" ]; then echo "PASS  no seat log yet -> exempt, no clock to run"; else echo "FAIL  no-log exemption"; fi
kill "$p" 2>/dev/null
# the report names the matchup
rm -f "$OUTDIR"/.inflight-* "$OUTDIR/HUNG"; gs=$(date +%s)
: > "$LOGDIR/${gs}-ai_baka_deck44-0xaa.jsonl"; touch -d "@$(( gs - 7200 ))" "$LOGDIR/${gs}-ai_baka_deck44-0xaa.jsonl"
sleep 300 & p=$!; printf "%s %s %s %s\n" "$p" "$gs" 44 135 > "$OUTDIR/.inflight-44v135-${gs}"
no_progress_sweep > "$W/out.txt" 2>&1; kill "$p" 2>/dev/null
/usr/bin/grep -q "GAME HUNG: deck44 vs deck135" "$W/out.txt" && /usr/bin/grep -q "no seat credited\|owes a rerun" "$W/out.txt" \
  && echo "PASS  the report names the matchup and says it owes a rerun" || { echo "FAIL  report text"; cat "$W/out.txt"; }
