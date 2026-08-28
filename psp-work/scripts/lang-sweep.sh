#!/bin/bash
# Per-card language sweep of the whole art pool.
# OCRs every full-size card image (eng model) and flags images whose text
# carries strong non-English type-line markers. Latin-script languages are
# caught by marker words; CJK cards under an eng model produce mostly-empty/
# garbage output with no English type words, caught by the NOTYPE heuristic.
#
# The flags are a NET, not verdicts: NOTYPE false-positives on noisy old card
# frames and needs vision verification before anything is refetched.
#
# Resumable. The file list is fixed (sweep-filelist.txt, sorted) so an index
# into it is a stable resume point.
#
#   ./lang-sweep.sh [START_INDEX] [WORKERS]
#
# OMP_THREAD_LIMIT=1 is load-bearing: tesseract links OpenMP and otherwise
# takes ~2.2 cores per process, so -P N oversubscribes by >2x and thrashes.
# With the pin, 8 workers do the whole 73k pool in ~75 min instead of ~66 h.

set -u
WORK=/home/magi/Projects/wagicGPT/psp-work
POOL=/home/magi/Projects/wagicGPT/projects/mtg/bin/User/sets
LIST="$WORK/sweep-filelist.txt"
START="${1:-1}"
WORKERS="${2:-8}"
OUT="$WORK/lang-sweep-flags.part-${START}.txt"
PROG=/tmp/lang-sweep.progress

export OMP_THREAD_LIMIT=1

[ -f "$LIST" ] || { echo "missing $LIST" >&2; exit 1; }
: > "$OUT"

scan_one() {
    f="$1"
    rel="${f#"$POOL"/}"
    txt=$(tesseract "$f" - -l eng --psm 3 2>/dev/null)
    # strong single-word markers, unique to their language's type/rules line
    if echo "$txt" | /usr/bin/grep -qiE 'instant.neo|criatura|conjuro|hechicer|encantamiento|artefacto|tierra b|retrospectiva|instant.nea|feiti|terreno b|stregoneria|istantaneo|incantesimo|terra base|spontanzauber|kreatur|hexerei|verzauberung|standardland|eph.m.re|rituel|terrain de base|enchantement'; then
        echo "$rel LATIN-MARKER" >> "$OUT"
        return
    fi
    # CJK/garbage heuristic: no English type word anywhere AND very little text
    if ! echo "$txt" | /usr/bin/grep -qiE 'creature|instant|sorcery|enchant|artifact|land|planeswalker|battle|token|summon|interrupt|mana source'; then
        n=$(echo "$txt" | tr -cd '[:alnum:]' | wc -c)
        if [ "$n" -lt 40 ]; then
            echo "$rel NOTYPE-LOWTEXT" >> "$OUT"
        else
            echo "$rel NOTYPE" >> "$OUT"
        fi
    fi
}
export -f scan_one
export POOL OUT

TOTAL=$(wc -l < "$LIST")
echo "sweep: index $START..$TOTAL, $WORKERS workers, OMP_THREAD_LIMIT=1" > "$PROG"

tail -n "+$START" "$LIST" \
  | xargs -P "$WORKERS" -n 1 -I{} bash -c 'scan_one "$@"' _ {} &
XPID=$!
DONEBASE=$((START - 1))
while kill -0 $XPID 2>/dev/null; do
    # progress is inferred from mtime-ordered completion, so just report flags
    echo "$(wc -l < "$OUT") flags | started idx $START of $TOTAL | $(date +%H:%M:%S)" > "$PROG"
    sleep 30
done
wait
echo "DONE $(wc -l < "$OUT") flags" >> "$PROG"
