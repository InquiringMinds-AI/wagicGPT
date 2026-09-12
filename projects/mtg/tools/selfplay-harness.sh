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
#W71-BP (L2, engine-seat HIGH-0/HIGH-1): the FAST arm of the same watchdog. The
#3,600 s clock above is sized for a slow decision and is right for one; the
#wave-70 hang was not slow, it was SPINNING - byte-identical engine lines at
#~340 KB/s for 3,641 s, 1.23 GB of stderr, while the seat log did not move at
#all. The old comment's warning still stands (stderr alone is not progress: the
#13-hour 404 park printed the whole time), so this arm requires BOTH - the
#translog silent for FAST_HANG_QUIET_S and the stderr grown by FAST_HANG_MB
#within that same silence. A slow-writing park cannot reach the byte threshold;
#a spin reaches it in minutes. That bounds the evidence file too: a wave-70-shaped
#hang is now stopped around 100 MB instead of 1.2 GB.
FAST_HANG_QUIET_S="${WAGIC_FAST_HANG_QUIET_S:-300}"
FAST_HANG_MB="${WAGIC_FAST_HANG_MB:-64}"
OUTDIR=""
URL="http://100.116.136.74:8081"   # Spark production port (8011 = serve.sh dev default)
MODEL="qwen35"
KEY=""
THINKING=""      #W70-BK (C1): no default. on|off, stated at launch.
FASTCLOCK=0.1   # game-seconds per engine tick; 0 = real-time pacing
FAIRHAND=1      # engine default; 0 = --riggedhand legacy-forensics escape

#W77-CR (R11 b, wave-76 engine-seat LOW-3): THE HARVEST BELONGS TO THIS RUN'S
# GAMES, NOT TO EVERYTHING NEWER THAN ITS START. `comm -13 $BEFORE_LIST` means
# "any seat log that did not exist when I started" - and every pool shares one
# $LOGDIR, so the wave-76 reruns (19 one-game pools, launched concurrently)
# harvested 38, 37, 36 ... 17 logs, descending with completion order: each pool
# swept up the other pools' IN-FLIGHT, gameend-less logs and the `-final` corpus
# had to be rebuilt by hand. Nothing in that corpus was tainted, but an
# aggregation that trusted a harvest count would have been. The outdir already
# IS a manifest - the harness creates `game-<d0>v<d1>-<gstart>.stderr` live, as
# each game starts - so the predicate `tools/corpus-early-check.py` uses (#W75-CI
# P24) is used here too: a log belongs when its FILENAME epoch (written once,
# never rewritten) is within 15 s of one of this outdir's game starts AND its
# deck is one of that game's two decks. With no manifest at all (a probe outdir,
# an old layout) it degrades to the set difference, which is the old behaviour.
#W77-CU (F10, Astra MED 10): DECK PAIR + A TOLERANCE IS NOT GAME IDENTITY.
# Astra executed the wave-77 predicate: with the manifest `125v123` at epoch
# 1000 it ACCEPTED foreign logs at 1005 for both `125v123` and `125v162`, and
# REJECTED the legitimate seat initialised at 1016 - and two concurrent games of
# the SAME pair were indistinguishable at any tolerance, which is the very shape
# the wave-76 reruns hit. The game itself is the only thing that knows which two
# files are its own (the name carries the seat's pointer), so each game now
# ANNOUNCES its two seat-log basenames on its own stderr
# (`WAGIC_GPT_TRANSLOG_FILE <base>`, printed by AIPlayerGPT when the translog is
# on) and the harness records them, per game, in `$OUTDIR/.seatlogs` as it
# reaps. Membership is then exact-name. The deck+tolerance predicate remains
# ONLY as the fallback for an outdir that has no `.seatlogs` at all (an old
# corpus, a probe outdir, a build whose engine does not announce), and it says so
# on stderr the first time it is used, so a silent degrade cannot be mistaken for
# identity.
harvest_belongs() {
    local outdir="$1" base="$2"
    python3 - "$outdir" "$base" <<'PYHARVEST'
import glob, os, re, sys
outdir, base = sys.argv[1], sys.argv[2]
named = os.path.join(outdir, '.seatlogs')
if os.path.exists(named) and os.path.getsize(named) > 0:
    with open(named) as fh:
        own = set(l.strip() for l in fh if l.strip())
    sys.exit(0 if base in own else 1)   # EXACT identity: the game named its own
GAME_RE = re.compile(r'^game-(\w+?)v(\w+?)-(\d+)\.stderr$')
SEAT_RE = re.compile(r'^(\d+)-ai_baka_(\w+?)-')
man = []
for g in glob.glob(os.path.join(outdir, 'game-*.stderr')):
    m = GAME_RE.match(os.path.basename(g))
    if m:
        man.append((int(m.group(3)), {m.group(1), m.group(2)}))
if not man:
    sys.exit(0)          # no manifest: the caller keeps the set difference
sys.stderr.write("!! harvest: no .seatlogs manifest in %s - falling back to deck+time"
                 " membership, which CANNOT separate concurrent pools of the same"
                 " deck pair (#W77-CU F10)\n" % outdir)
m = SEAT_RE.match(base)
if not m:
    sys.exit(0)          # not a seat log we can read: do not drop it
ep = int(m.group(1))
deck = m.group(2)[4:] if m.group(2).startswith('deck') else m.group(2)
for gstart, decks in man:
    if abs(ep - gstart) <= 15 and deck in decks:
        sys.exit(0)
sys.exit(1)              # a log of somebody else's game
PYHARVEST
}

harvest_translogs() {
    local logdir="$1" outdir="$2" before="$3" f base
    comm -13 "$before" <(ls "$logdir"/*.jsonl 2>/dev/null | sort) | while read -r f; do
        base="$(basename "$f")"
        harvest_belongs "$outdir" "$base" || continue
        cp "$f" "$outdir/"
        if [ -f "$logdir/askreplay/$base" ]; then
            mkdir -p "$outdir/askreplay"
            cp "$logdir/askreplay/$base" "$outdir/askreplay/"
        fi
    done
}

#W72-BT (M1): the harvest, self-tested. HIGH-1 was one missing line in a block
# no test ever ran; the sidecar is invisible to the corpus if it regresses, and
# invisible evidence is how the wave-71 corpus concluded "0 re-serves". This
# builds a throwaway LOGDIR with one pre-existing log (must NOT be harvested),
# one new log and its sidecar (both MUST be), and a sidecar with no matching new
# log (must NOT be), then asserts on the harvested tree. `--selftest` runs it
# before the regime-gate selftest and fails the whole flag if it fails.
harvest_selftest() {
    local tmp fails=0
    tmp="$(mktemp -d)"
    mkdir -p "$tmp/log/askreplay" "$tmp/out"
    : > "$tmp/log/old-seat.jsonl"
    ls "$tmp/log"/*.jsonl 2>/dev/null | sort > "$tmp/before"
    : > "$tmp/log/new-seat.jsonl"
    echo '{"kind":"ask_replay"}' > "$tmp/log/askreplay/new-seat.jsonl"
    echo '{"kind":"ask_replay"}' > "$tmp/log/askreplay/stale-seat.jsonl"
    harvest_translogs "$tmp/log" "$tmp/out" "$tmp/before"
    [ -f "$tmp/out/new-seat.jsonl" ] || { echo "harvest-selftest FAIL: this run's translog was not harvested" >&2; fails=1; }
    [ -f "$tmp/out/askreplay/new-seat.jsonl" ] || { echo "harvest-selftest FAIL: the ask_replay sidecar was not harvested (wave-71 HIGH-1)" >&2; fails=1; }
    [ -f "$tmp/out/old-seat.jsonl" ] && { echo "harvest-selftest FAIL: a pre-existing translog was harvested" >&2; fails=1; }
    [ -f "$tmp/out/askreplay/stale-seat.jsonl" ] && { echo "harvest-selftest FAIL: a sidecar with no translog of this run was harvested" >&2; fails=1; }
    rm -rf "$tmp"
    #W77-CR (R11 b): the MANIFEST case - the shape that cost the wave-76 reruns.
    # One outdir whose manifest holds a single game (125 vs 123), one new seat log
    # belonging to it, and one new seat log from a CONCURRENT pool's game (deck162,
    # a deck this outdir never started). Both are newer than $BEFORE_LIST, so the
    # set difference alone harvests both; the manifest must keep the second out.
    # The last two checks pin the FALLBACK: with the manifest removed, the old
    # behaviour returns and both are harvested (a probe outdir must not go empty).
    tmp="$(mktemp -d)"
    mkdir -p "$tmp/log" "$tmp/out"
    : > "$tmp/log/old-seat.jsonl"
    ls "$tmp/log"/*.jsonl 2>/dev/null | sort > "$tmp/before"
    : > "$tmp/out/game-125v123-1789074080.stderr"
    : > "$tmp/log/1789074083-ai_baka_deck125-0xaa-vs-ai_baka_deck123.jsonl"
    : > "$tmp/log/1789074098-ai_baka_deck162-0xbb-vs-ai_baka_deck125.jsonl"
    harvest_translogs "$tmp/log" "$tmp/out" "$tmp/before"
    [ -f "$tmp/out/1789074083-ai_baka_deck125-0xaa-vs-ai_baka_deck123.jsonl" ] \
        || { echo "harvest-selftest FAIL: this run's own game log was not harvested under the manifest" >&2; fails=1; }
    [ -f "$tmp/out/1789074098-ai_baka_deck162-0xbb-vs-ai_baka_deck125.jsonl" ] \
        && { echo "harvest-selftest FAIL: a concurrent pool's in-flight log was harvested (wave-76 LOW-3)" >&2; fails=1; }
    rm -f "$tmp/out"/game-*.stderr
    rm -f "$tmp/out"/*.jsonl
    harvest_translogs "$tmp/log" "$tmp/out" "$tmp/before"
    [ -f "$tmp/out/1789074083-ai_baka_deck125-0xaa-vs-ai_baka_deck123.jsonl" ] \
        && [ -f "$tmp/out/1789074098-ai_baka_deck162-0xbb-vs-ai_baka_deck125.jsonl" ] \
        || { echo "harvest-selftest FAIL: with no manifest the harvest must fall back to the set difference" >&2; fails=1; }
    rm -rf "$tmp"
    #W77-CU (F10): the THREE SHAPES Astra executed against the deck+tolerance
    # predicate, now against exact-name membership. Manifest game 125v123 at
    # epoch 1000; its two seat logs are named by the game itself.
    tmp="$(mktemp -d)"
    mkdir -p "$tmp/log" "$tmp/out"
    ls "$tmp/log"/*.jsonl 2>/dev/null | sort > "$tmp/before"
    : > "$tmp/out/game-125v123-1000.stderr"
    #  (1) a foreign log of the SAME pair, five seconds away - accepted before.
    : > "$tmp/log/1005-ai_baka_deck125-0xff-vs-ai_baka_deck123.jsonl"
    #  (2) a foreign log of a DIFFERENT pair sharing one deck - accepted before.
    : > "$tmp/log/1005-ai_baka_deck125-0xee-vs-ai_baka_deck162.jsonl"
    #  (3) THIS game's own seat, initialised sixteen seconds in - rejected before.
    : > "$tmp/log/1016-ai_baka_deck123-0xaa-vs-ai_baka_deck125.jsonl"
    printf '%s\n' '1016-ai_baka_deck123-0xaa-vs-ai_baka_deck125.jsonl' > "$tmp/out/.seatlogs"
    harvest_translogs "$tmp/log" "$tmp/out" "$tmp/before"
    [ -f "$tmp/out/1016-ai_baka_deck123-0xaa-vs-ai_baka_deck125.jsonl" ] \
        || { echo "harvest-selftest FAIL: a NAMED seat log 16 s from the game start was dropped (#W77-CU F10 shape 3)" >&2; fails=1; }
    [ -f "$tmp/out/1005-ai_baka_deck125-0xff-vs-ai_baka_deck123.jsonl" ] \
        && { echo "harvest-selftest FAIL: a foreign log of the SAME deck pair was harvested (#W77-CU F10 shape 1)" >&2; fails=1; }
    [ -f "$tmp/out/1005-ai_baka_deck125-0xee-vs-ai_baka_deck162.jsonl" ] \
        && { echo "harvest-selftest FAIL: a foreign log sharing one deck was harvested (#W77-CU F10 shape 2)" >&2; fails=1; }
    #  ...and two CONCURRENT games of the SAME pair are separable, which no
    #  tolerance could do: only the one this outdir named is taken.
    rm -f "$tmp/out"/*.jsonl
    : > "$tmp/log/1007-ai_baka_deck125-0x11-vs-ai_baka_deck123.jsonl"
    : > "$tmp/log/1007-ai_baka_deck125-0x22-vs-ai_baka_deck123.jsonl"
    printf '%s\n' '1007-ai_baka_deck125-0x11-vs-ai_baka_deck123.jsonl' >> "$tmp/out/.seatlogs"
    harvest_translogs "$tmp/log" "$tmp/out" "$tmp/before"
    [ -f "$tmp/out/1007-ai_baka_deck125-0x11-vs-ai_baka_deck123.jsonl" ] \
        && [ ! -f "$tmp/out/1007-ai_baka_deck125-0x22-vs-ai_baka_deck123.jsonl" ] \
        || { echo "harvest-selftest FAIL: two concurrent games of the same pair were not separated (#W77-CU F10)" >&2; fails=1; }
    #  FALLBACK: with no .seatlogs the old deck+time predicate returns (warned).
    rm -f "$tmp/out/.seatlogs" "$tmp/out"/*.jsonl
    harvest_translogs "$tmp/log" "$tmp/out" "$tmp/before" 2>/dev/null
    [ -f "$tmp/out/1005-ai_baka_deck125-0xff-vs-ai_baka_deck123.jsonl" ] \
        || { echo "harvest-selftest FAIL: with no .seatlogs the fallback predicate must still harvest (#W77-CU F10)" >&2; fails=1; }
    rm -rf "$tmp"
    [ "$fails" = 0 ] && echo "harvest-selftest: 14 checks, 0 failed"
    return "$fails"
}

#O26 (wave-73 psp-work NOTES 2026-09-08, the pilot-wedge incident). A WEDGED
# PILOT IS INVISIBLE TO EVERY WATCHDOG ABOVE. When the inference server stops
# generating but its API front end keeps answering, each seat waits out its full
# deadline, writes a `timeout` fallback record and plays on with the heuristic:
# translog mtimes advance (so no_progress_sweep sees progress), games do not hang
# (so the hang guard is silent), and latency is not measurable from records that
# never came back (so the feasibility half stands down). The 4-h check saw "22
# active logs" and called it normal while six games ran ~8 h on heuristics.
# A game answered by heuristics is not a corpus game (invariant 00), so this is a
# STOP condition, not a warning. Predicate: pool the last 3 records of every seat
# log of THIS run; if at least K of them are pooled and EVERY one is a `timeout`
# fallback, the pilot is not answering anybody. Pure - the same script backs the
# --selftest.
#O26/#W74-CF (F3, Astra review finding 3). "The last three records of every
# unfinished log" is neither the newest K decisions nor evidence that every active
# seat has stopped being answered, and it fails in BOTH directions:
#  - it KILLS a slow healthy run: 21 live logs, six carrying one initial timeout
#    each and fifteen still on `gamestart` with their first request pending, reads
#    STALL 6. Nothing established that the fifteen had failed at all.
#  - it MISSES a stalled tail: one unfinished seat with twenty consecutive
#    timeouts, everything else finished, reads OK 3 - that seat can never reach K
#    on a three-record-per-log pool.
#  - and one ARBITRARILY OLD record vetoes the newest failure streak: an
#    unfinished log holding a single stale success plus six newer failures
#    elsewhere reads OK 6.
# The predicate is now: the newest K decision records ACROSS seats (logs ordered by
# mtime, newest first - during a live sweep a log's mtime IS the time of its last
# record; the harvest that rewrites mtimes happens after the run and this predicate
# never reads a harvested directory), STALL only when all K are silent-fallback AND
# every active seat has produced at least one decision record. That last clause is
# what keeps a young run alive: a seat still waiting on its first answer is not
# evidence of a wedge, it is a seat with no evidence either way.
PILOT_STALL_K="${WAGIC_PILOT_STALL_K:-6}"
#Every class that means THE ENDPOINT DID NOT ANSWER. A wedged pilot writes
#`timeout` and, once the deadline can no longer fit a retry, `wall_miss_no_retry`
#- the wave-73 tails alternate between exactly those two, so keying on the single
#word "timeout" reads the run as healthy. `unparsed_reply` is deliberately NOT
#here: that is the model answering badly, which is a play problem, not a wedge.
#ONE list, exported, so the tripwire and the results banner cannot disagree about
#what "the model never answered this" means (#W74-CF F4).
PILOT_SILENT_CLASSES="timeout wall_miss_no_retry wall_miss_unrecorded empty_reply http_error"
export PILOT_SILENT_CLASSES
pilot_stall_verdict() {
    #$1 = logdir, $2 = run start epoch, $3 = K -> "STALL <n>" | "OK <n>"
    python3 - "$1" "$2" "$3" <<'PSY'
import glob, json, os, sys
logdir, start, k = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
SILENT = tuple(os.environ.get("PILOT_SILENT_CLASSES", "timeout").split())

def silent(r):
    return str(r.get("fallback", "")).startswith(SILENT)

live = []   # (mtime, [decision records, newest first]) for every ACTIVE seat
for f in sorted(glob.glob(os.path.join(logdir, "*.jsonl"))):
    try:
        ep = int(os.path.basename(f).split("-")[0])
    except ValueError:
        continue
    if ep < start - 5:
        continue
    recs = []
    ended = False
    try:
        for line in open(f, errors="replace"):
            try: r = json.loads(line)
            except Exception: continue
            if r.get("kind") == "gameend":
                ended = True
                continue
            #`recovery` is bookkeeping written BESIDE the failed decision (which
            #arm answered it), not a decision of its own - counting it as an
            #answer hides the very tail this predicate exists to read.
            if r.get("kind") in ("gamestart", "system", "ask_replay", "recovery"):
                continue
            recs.append(r)
    except OSError:
        continue
    #Only a game STILL IN FLIGHT can be wedged. A finished seat log's tail is
    #whatever it ended on, and pooling those dilutes the live seats out of the
    #verdict.
    if ended:
        continue
    try:
        mt = os.path.getmtime(f)
    except OSError:
        mt = 0
    live.append((mt, list(reversed(recs))))

#EVERY ACTIVE SEAT REPRESENTED: a seat that has not answered (or failed) a single
#decision yet carries no evidence, and a run where any seat is in that state is a
#run we cannot call wedged. This is the whole difference between Astra's case (a),
#a young run that must live, and case (c), an old record that must not veto.
allHaveEvidence = bool(live) and all(recs for _, recs in live)

##W75-CM (F9, Astra wave-75 review finding 9). ...AND EVERY LIVE SEAT MUST BE
#FAILING. Bounding each seat's CONTRIBUTION (below) stops one wedged seat from
#supplying all K, but it does not make the pool cover every seat: with 4 live
#seats and K=6 the cap is 2, so three timing-out seats fill the pool while the
#fourth is being answered normally, and the banner - which says "the last records
#of every live seat are timeout fallbacks" - was false. Executed by Astra at K=6:
#4 live seats (3 x two timeouts + 1 succeeding) -> STALL 6, and 21 live seats
#(6 failing, 15 succeeding) -> STALL 6. A seat whose NEWEST record is a decision
#the model answered is evidence AGAINST an endpoint stall, and one such seat is
#enough: the endpoint is generating. So the verdict now requires it of every live
#seat, which is exactly what the banner claims. The single-live-seat case (one
#unfinished seat with twenty consecutive timeouts, every other game finished) is
#unchanged - its newest record is a failure.
#
#RECENCY, which is what reconciles this with #W74-CF's own case (j): an
#ARBITRARILY OLD success must not veto a live failure streak. A seat's newest
#record is counter-evidence only while that seat is still WRITING - a log whose
#mtime trails the newest live log by more than the grace window is a seat nothing
#has happened on for longer than any decision takes, and its last answer says
#nothing about whether the endpoint is generating NOW. Grace is deliberately
#generous (10 min against a 120 s HTTP deadline): the cost of being wrong here is
#a corpus that keeps running, which the game watchdogs still bound.
STALL_EVIDENCE_GRACE = 600
newestLive = max([mt for mt, _ in live]) if live else 0
allNewestSilent = bool(live) and all(
    recs and (silent(recs[0]) or mt < newestLive - STALL_EVIDENCE_GRACE)
    for mt, recs in live)

#the newest K decisions ACROSS seats: newest log first, its newest record first.
##W75-CI (P24, wave-74 known-bugs): BOUNDED PER SEAT. The pool was filled seat by
#seat, so ONE seat's silent tail could supply all K while twenty other seats were
#answering normally - the tripwire would then stop a healthy corpus on the evidence
#of a single wedged game. ACROSS SEATS is the predicate's own word: no seat may
#contribute more than ceil(K / live seats). The cap is strict, so a lone silent seat
#leaves the pool short of K and the verdict is OK (that seat is the game watchdog's
#business, not the endpoint tripwire's). With one live seat the cap is K and the
#behaviour is exactly as before.
per = -(-k // max(1, len(live)))
pool = []
for _, recs in sorted(live, key=lambda lr: -lr[0]):
    taken = 0
    for r in recs:
        if taken >= per or len(pool) >= k:
            break
        pool.append(r)
        taken += 1
    if len(pool) >= k:
        break

timeouts = [r for r in pool if silent(r)]
if allHaveEvidence and allNewestSilent and len(pool) >= k and len(timeouts) == len(pool):
    print("STALL %d" % len(timeouts))
else:
    print("OK %d" % len(timeouts))
PSY
}

pilot_stall_sweep() {
    local verdict
    verdict=$(pilot_stall_verdict "$LOGDIR" "$START" "$PILOT_STALL_K") || return 0
    case "$verdict" in
        STALL*)
            set -- $verdict
            echo ""
            echo "== PILOT STALL: the last records of every live seat are timeout fallbacks ($2 of $2)."
            echo "== The endpoint's API may still answer /v1/models while its engine has stopped"
            echo "== generating (wave-73: frozen at ~17h, GPU pinned, zero throughput). Every seat"
            echo "== is now playing on the heuristic, and a game answered by heuristics is not a"
            echo "== corpus game (invariant 00). Stopping the run."
            echo "== Restart the pilot, then RERUN the affected matchups."
            touch "$OUTDIR/PILOT-STALL"
            kill -TERM "$HARNESS_PID" 2>/dev/null
            return 1;;
    esac
    return 0
}

#W74-CG: THE WINDOW LOOP. The wave-74 corpus (run 20260909-015553) burned 8 h
# with 11 of 21 games frozen at turns 3-15 and NOTHING in the supervisor saw it:
# every decision was answered by the model, on time, with reasoning - so the
# pilot-stall predicate above (which reads endpoint SILENCE) was correctly OK the
# whole way. What was actually happening is the opposite failure: one seat asked
# the SAME turn-4 Main-1 casting window 384 times, each a full ~60-100 s model
# call, because the decline row's re-ask clause carried a count that rose with
# every answer and rode into the ask key, so the identical-window cache could
# never hit again. The observable is in the prompt the seat is being served: a
# `declined this exact list N times` that keeps climbing inside ONE turn. Healthy
# waves sit at N <= 3 (wave 73's whole corpus); N >= 40 in one turn is not a
# pilot deliberating, it is a window that cannot close. A game that cannot leave
# a phase is not going to finish, and invariant 00 makes that a STOP, not a
# warning. Read off the NEWEST record of each live seat only - an old high count
# from a turn the seat has since left is history, not a wedge.
#
#W74-CH: AND THE BOARD MUST NOT HAVE MOVED. The count alone is a FALSE POSITIVE
# on a finite TRIGGER CHAIN, and it fired on one inside a day: run
# 20260909-104713 was stopped at 18/21 games because game `125v126`'s deck126
# seat was served 41 casting windows in one upkeep while its OWN Sanguine Bond +
# Exquisite Blood chain drained the opponent 21 -> 1 - one link per window, the
# chain one link from winning the game. That is progress, not a wedge: each
# window's board differs from the last. The dead loop of 20260909-015553 is the
# opposite - 384 windows at turn 4 Main 1 with `my_life` 20, `opp_life` 20 and an
# empty stack on every single record. So the predicate is now BOTH terms: the
# count at or above N, AND an UNCHANGED BOARD across the newest K decisions of
# that seat (identical my_life, identical opp_life, identical stack-top line -
# or no `events` on any of them). Either term alone is wrong: the count alone
# kills healthy chains, the board alone would stop any seat deliberating twice.
WINDOW_LOOP_N="${WAGIC_WINDOW_LOOP_N:-40}"
WINDOW_LOOP_K="${WAGIC_WINDOW_LOOP_K:-8}"
window_loop_verdict() {
    #$1 = logdir, $2 = run start epoch, $3 = N, $4 = K (default 8)
    #   -> "LOOP <n> <seat>" | "OK <n>"
    python3 - "$1" "$2" "$3" "${4:-8}" <<'WLY'
import glob, json, os, re, sys
logdir, start, n = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
k = int(sys.argv[4]) if len(sys.argv) > 4 else 8
DECL = re.compile(r"declined this exact list (\d+) times? this turn")
STACK_HEAD = "ON THE STACK"

def stack_top(prompt):
    #The board frame's stack header, then the first non-empty line under it. No
    #header at all is an EMPTY stack, which is its own (stable) value.
    lines = str(prompt).split("\n")
    for i, ln in enumerate(lines):
        if STACK_HEAD in ln:
            for nxt in lines[i + 1:]:
                if nxt.strip():
                    return nxt.strip()
            return "<empty>"
    return "<no stack>"

def board_of(r):
    return (r.get("my_life"), r.get("opp_life"), stack_top(r.get("prompt", "")))

worst, worstSeat = 0, ""
for f in sorted(glob.glob(os.path.join(logdir, "*.jsonl"))):
    try:
        ep = int(os.path.basename(f).split("-")[0])
    except ValueError:
        continue
    if ep < start - 5:
        continue
    recs, ended = [], False
    try:
        for line in open(f, errors="replace"):
            try: r = json.loads(line)
            except Exception: continue
            if r.get("kind") == "gameend":
                ended = True
                continue
            if r.get("kind") in ("gamestart", "system", "ask_replay", "recovery"):
                continue
            recs.append(r)
    except OSError:
        continue
    #Only a game STILL IN FLIGHT can be looping; a finished game's counts are
    #whatever it ended on.
    if ended or not recs:
        continue
    counts = [int(x) for x in DECL.findall(str(recs[-1].get("prompt", "")))]
    if not counts or max(counts) <= worst:
        continue
    #W74-CH: the SECOND term. The newest K decisions of this seat must sit on
    #ONE board - a trigger chain advances the board at every link, and the 41
    #windows it costs are 41 links of progress, not a window that cannot close.
    window = recs[-k:]
    boards = set(board_of(r) for r in window)
    events = [str(r.get("events") or "").strip() for r in window]
    if len(boards) == 1 or not any(events):
        worst, worstSeat = max(counts), os.path.basename(f)

if worst >= n:
    print("LOOP %d %s" % (worst, worstSeat))
else:
    print("OK %d" % worst)
WLY
}

window_loop_sweep() {
    local verdict
    verdict=$(window_loop_verdict "$LOGDIR" "$START" "$WINDOW_LOOP_N" "$WINDOW_LOOP_K") || return 0
    case "$verdict" in
        LOOP*)
            set -- $verdict
            echo ""
            echo "== WINDOW LOOP: a live seat is being served the same window over and over."
            echo "== $3 has just been told it declined this exact list $2 times THIS TURN"
            echo "== (healthy corpora sit at 3 or fewer). The model IS answering - this is not"
            echo "== a pilot stall - but the window never closes, so the phase never advances"
            echo "== and every repeat is another full model call. The game cannot finish, and a"
            echo "== corpus that does not complete games has failed (invariant 00). Stopping."
            echo "== Its newest $WINDOW_LOOP_K decisions also sit on ONE board (same life totals,"
            echo "== same stack top), so this is not a trigger chain making progress."
            echo "== Read that seat's newest two prompts: if they differ only in this count,"
            echo "== the ask key is unstable again (wave-74 lane CG); if they differ only in a"
            echo "== {...} or [...] annotation, the hold key is reading the render (lane CH)."
            touch "$OUTDIR/WINDOW-LOOP"
            kill -TERM "$HARNESS_PID" 2>/dev/null
            return 1;;
    esac
    return 0
}

window_loop_selftest() {
    local tmp fails=0 v
    tmp="$(mktemp -d)"
    mk() { #$1 = file, then the record lines
        local f="$tmp/$1"; shift; printf '%s\n' "$@" > "$f"
    }
    #a: the wave-74 shape - a live seat whose NEWEST prompt carries a high count
    mk 9999999999-ai_baka_deck123-a.jsonl \
      '{"kind":"ask","seq":299,"prompt":"3. Cast nothing {this same question will be asked again this turn: taking this row closes this window only, and you have already declined this exact list 294 times this turn}"}' \
      '{"kind":"ask","seq":300,"prompt":"3. Cast nothing {... you have already declined this exact list 295 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 1 40)
    case "$v" in "LOOP 295 9999999999-ai_baka_deck123-a.jsonl") ;;
      *) echo "window-loop-selftest FAIL: the wave-74 shape gave '$v', want 'LOOP 295 ...'" >&2; fails=1;; esac
    #b: a healthy corpus - wave-73 counts never left single digits
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck125-b.jsonl \
      '{"kind":"ask","seq":9,"prompt":"3. Cast nothing {... you have already declined this exact list 3 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 1 40)
    case "$v" in "OK 3") ;; *) echo "window-loop-selftest FAIL: a wave-73 count gave '$v', want 'OK 3'" >&2; fails=1;; esac
    #c: the count is only evidence while the game is LIVE - a finished game's
    #tail is history and must not stop the next run's sweep.
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck123-c.jsonl \
      '{"kind":"ask","seq":300,"prompt":"{... declined this exact list 295 times this turn}"}' \
      '{"kind":"gameend","seq":301,"won":1}'
    v=$(window_loop_verdict "$tmp" 1 40)
    case "$v" in "OK 0") ;; *) echo "window-loop-selftest FAIL: a finished game gave '$v', want 'OK 0'" >&2; fails=1;; esac
    #d: a high count the seat has ALREADY left behind is history, not a wedge -
    #only the newest record of a live seat counts.
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck123-d.jsonl \
      '{"kind":"ask","seq":300,"prompt":"{... declined this exact list 295 times this turn}"}' \
      '{"kind":"ask","seq":301,"prompt":"1. Attack with Bear"}'
    v=$(window_loop_verdict "$tmp" 1 40)
    case "$v" in "OK 0") ;; *) echo "window-loop-selftest FAIL: a count the seat moved on from gave '$v', want 'OK 0'" >&2; fails=1;; esac
    #e: an older run's logs in the same directory are not this run's evidence.
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck123-e.jsonl \
      '{"kind":"ask","seq":300,"prompt":"{... declined this exact list 295 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 99999999999 40)
    case "$v" in "OK 0") ;; *) echo "window-loop-selftest FAIL: an older-run log gave '$v', want 'OK 0'" >&2; fails=1;; esac
    #f: the threshold is the threshold - 40 fires, 39 does not.
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck123-f.jsonl \
      '{"kind":"ask","seq":40,"prompt":"{... declined this exact list 40 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 1 40)
    case "$v" in LOOP\ 40*) ;; *) echo "window-loop-selftest FAIL: N=40 gave '$v', want LOOP" >&2; fails=1;; esac
    v=$(window_loop_verdict "$tmp" 1 41)
    case "$v" in "OK 40") ;; *) echo "window-loop-selftest FAIL: N=40 under a 41 threshold gave '$v', want 'OK 40'" >&2; fails=1;; esac
    #W74-CH g: A TRIGGER CHAIN IS NOT A LOOP. The real shape that tripped the
    #wave-74 relaunch: game 125v126's deck126 seat, turn 32 Upkeep, its own
    #Sanguine Bond + Exquisite Blood chain draining the opponent one life per
    #window. The count walks past 40 - and the BOARD moves at every link, so the
    #run must NOT be stopped (it was one link from winning).
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck126-g.jsonl \
      '{"kind":"ask","seq":306,"turn":32,"my_life":37,"opp_life":2,"events":"- Opponent lost 1 life (now 2)","prompt":"ON THE STACK, waiting to resolve:\n- Sanguine Bond trigger\n3. Cast nothing {... reaches 0 in 2 more turns ...} {... you have already declined this exact list 38 times this turn}"}' \
      '{"kind":"ask","seq":307,"turn":32,"my_life":38,"opp_life":2,"events":"- You gained 1 life (now 38)","prompt":"ON THE STACK, waiting to resolve:\n- Exquisite Blood trigger\n3. Cast nothing {... reaches 0 in 2 more turns ...} {... you have already declined this exact list 39 times this turn}"}' \
      '{"kind":"ask","seq":308,"turn":32,"my_life":38,"opp_life":1,"events":"- Opponent lost 1 life (now 1)","prompt":"ON THE STACK, waiting to resolve:\n- Sanguine Bond trigger\n3. Cast nothing {... reaches 0 in 1 more turn ...} {... you have already declined this exact list 40 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 1 40 8)
    case "$v" in "OK 0") ;;
      *) echo "window-loop-selftest FAIL: a moving-life chain gave '$v', want 'OK 0'" >&2; fails=1;; esac
    #W74-CH h: the REAL dead loop still reads LOOP. Run 20260909-015553, deck123
    #seat: 384 windows at turn 4 Main 1, my_life 20, opp_life 20, empty stack and
    #no events on every record.
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck123-h.jsonl \
      '{"kind":"ask","seq":384,"turn":4,"my_life":20,"opp_life":20,"prompt":"1. Play Swamp\n3. Cast nothing {... you have already declined this exact list 293 times this turn}"}' \
      '{"kind":"ask","seq":385,"turn":4,"my_life":20,"opp_life":20,"prompt":"1. Play Swamp\n3. Cast nothing {... you have already declined this exact list 294 times this turn}"}' \
      '{"kind":"ask","seq":386,"turn":4,"my_life":20,"opp_life":20,"prompt":"1. Play Swamp\n3. Cast nothing {... you have already declined this exact list 295 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 1 40 8)
    case "$v" in "LOOP 295 9999999999-ai_baka_deck123-h.jsonl") ;;
      *) echo "window-loop-selftest FAIL: the real dead loop gave '$v', want 'LOOP 295 ...'" >&2; fails=1;; esac
    #W74-CH i: the second instance - the life that moves is the seat's OWN, under
    #the OPPONENT's punisher chain (125v162, turn 27 Draw). Also not a loop.
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck125-i.jsonl \
      '{"kind":"ask","seq":138,"turn":27,"my_life":26,"opp_life":18,"events":"- You lost 2 life","prompt":"3. Cast nothing {... declined this exact list 41 times this turn}"}' \
      '{"kind":"ask","seq":139,"turn":27,"my_life":25,"opp_life":18,"events":"- You lost 1 life","prompt":"3. Cast nothing {... declined this exact list 42 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 1 40 8)
    case "$v" in "OK 0") ;;
      *) echo "window-loop-selftest FAIL: a chain moving the seat's own life gave '$v', want 'OK 0'" >&2; fails=1;; esac
    #W74-CH j: LIFE alone is not the board - a chain that moves only the STACK is
    #still progress.
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck130-j.jsonl \
      '{"kind":"ask","seq":10,"turn":9,"my_life":20,"opp_life":20,"events":"- trigger","prompt":"ON THE STACK, waiting to resolve:\n- Howling Mine trigger\n3. Cast nothing {... declined this exact list 55 times this turn}"}' \
      '{"kind":"ask","seq":11,"turn":9,"my_life":20,"opp_life":20,"events":"- trigger","prompt":"ON THE STACK, waiting to resolve:\n- Underworld Dreams trigger\n3. Cast nothing {... declined this exact list 56 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 1 40 8)
    case "$v" in "OK 0") ;;
      *) echo "window-loop-selftest FAIL: a moving stack top gave '$v', want 'OK 0'" >&2; fails=1;; esac
    #W74-CH k: a seat whose board is frozen but which HAS events is still a loop
    #when the life totals and stack top never move - the two terms are OR'd on
    #the "unmoved" side deliberately, so a noisy narration cannot hide a wedge.
    rm -f "$tmp"/*.jsonl
    mk 9999999999-ai_baka_deck123-k.jsonl \
      '{"kind":"ask","seq":1,"turn":4,"my_life":20,"opp_life":20,"events":"- Phase: Main phase 1","prompt":"3. Cast nothing {... declined this exact list 99 times this turn}"}' \
      '{"kind":"ask","seq":2,"turn":4,"my_life":20,"opp_life":20,"events":"- Phase: Main phase 1","prompt":"3. Cast nothing {... declined this exact list 100 times this turn}"}'
    v=$(window_loop_verdict "$tmp" 1 40 8)
    case "$v" in LOOP\ 100*) ;;
      *) echo "window-loop-selftest FAIL: a frozen board with narration gave '$v', want LOOP" >&2; fails=1;; esac
    rm -rf "$tmp"
    [ "$fails" = 0 ] && echo "window-loop-selftest: 12 checks, 0 failed"
    return "$fails"
}

#O26: both halves of the tripwire, self-tested on crafted logs - the wedge is
# rare and expensive, so the predicate cannot wait for the next one to be checked.
pilot_stall_selftest() {
    local tmp fails=0 v
    tmp="$(mktemp -d)"
    #a: every seat's tail is a timeout fallback -> STALL
    printf '%s\n' \
      '{"kind":"ask","seq":1,"fallback":"timeout"}' \
      '{"kind":"ask","seq":2,"fallback":"timeout"}' \
      '{"kind":"ask","seq":3,"fallback":"timeout"}' > "$tmp/9999999999-ai_baka_deck1-a.jsonl"
    cp "$tmp/9999999999-ai_baka_deck1-a.jsonl" "$tmp/9999999999-ai_baka_deck2-b.jsonl"
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in STALL\ 6) ;; *) echo "pilot-stall-selftest FAIL: all-timeout tails gave '$v', want 'STALL 6'" >&2; fails=1;; esac
    #b: one live answer in the pool -> OK (the pilot is answering somebody)
    printf '%s\n' \
      '{"kind":"ask","seq":1,"fallback":"timeout"}' \
      '{"kind":"ask","seq":2,"fallback":"timeout"}' \
      '{"kind":"ask","seq":3}' > "$tmp/9999999999-ai_baka_deck2-b.jsonl"
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in OK\ 5) ;; *) echo "pilot-stall-selftest FAIL: a live answer gave '$v', want 'OK 5'" >&2; fails=1;; esac
    #c: too few records to judge -> OK (never stop a run on one seat's first ask)
    rm -f "$tmp/9999999999-ai_baka_deck2-b.jsonl"
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in OK\ 3) ;; *) echo "pilot-stall-selftest FAIL: 3 records gave '$v', want 'OK 3'" >&2; fails=1;; esac
    #f: the REAL wave-73 tail shape - wall_miss_no_retry, a `recovery` bookkeeping
    #record between them, then timeout. Keying on the word "timeout" alone, or
    #counting the recovery record as an answer, reads this as healthy.
    printf '%s\n' \
      '{"kind":"wall_miss","seq":1,"fallback":"wall_miss_no_retry"}' \
      '{"kind":"recovery","seq":2,"recovers_fallback":"wall_miss_no_retry"}' \
      '{"kind":"priority","seq":3,"fallback":"timeout"}' > "$tmp/9999999999-ai_baka_deck3-c.jsonl"
    cp "$tmp/9999999999-ai_baka_deck3-c.jsonl" "$tmp/9999999999-ai_baka_deck4-d.jsonl"
    rm -f "$tmp/9999999999-ai_baka_deck1-a.jsonl" "$tmp/9999999999-ai_baka_deck2-b.jsonl"
    v=$(pilot_stall_verdict "$tmp" 1 4)
    case "$v" in STALL\ 4) ;; *) echo "pilot-stall-selftest FAIL: the wave-73 tail shape gave '$v', want 'STALL 4'" >&2; fails=1;; esac
    #g: a FINISHED game's tail is not evidence of a wedge - only a live game can be
    #wedged, and pooling finished seats dilutes the live ones out of the verdict.
    printf '%s\n' \
      '{"kind":"priority","seq":1,"fallback":"timeout"}' \
      '{"kind":"gameend","seq":2,"won":true}' > "$tmp/9999999999-ai_baka_deck5-e.jsonl"
    v=$(pilot_stall_verdict "$tmp" 1 8)
    case "$v" in OK\ 4) ;; *) echo "pilot-stall-selftest FAIL: a finished seat log gave '$v', want 'OK 4'" >&2; fails=1;; esac
    rm -f "$tmp/9999999999-ai_baka_deck3-c.jsonl" "$tmp/9999999999-ai_baka_deck4-d.jsonl" "$tmp/9999999999-ai_baka_deck5-e.jsonl"
    printf '%s\n' \
      '{"kind":"ask","seq":1,"fallback":"timeout"}' \
      '{"kind":"ask","seq":2,"fallback":"timeout"}' \
      '{"kind":"ask","seq":3,"fallback":"timeout"}' > "$tmp/9999999999-ai_baka_deck1-a.jsonl"
    #d: logs from a PREVIOUS run are not this run's evidence
    mv "$tmp/9999999999-ai_baka_deck1-a.jsonl" "$tmp/1000000000-ai_baka_deck1-a.jsonl"
    v=$(pilot_stall_verdict "$tmp" 9999999999 6)
    case "$v" in OK\ 0) ;; *) echo "pilot-stall-selftest FAIL: older-run filter gave '$v', want 'OK 0'" >&2; fails=1;; esac
    #e: the per-game banner count reads the same field
    v=$(silent_fallbacks_in "$tmp/1000000000-ai_baka_deck1-a.jsonl")
    case "$v" in 3) ;; *) echo "pilot-stall-selftest FAIL: per-game count gave '$v', want '3'" >&2; fails=1;; esac
    rm -f "$tmp"/*.jsonl

    ##W74-CF (F3): Astra's three lifecycle/cardinality counterexamples. Each one
    # is a verdict the "last three per unfinished log" predicate got wrong, in a
    # direction that either kills viable work or certifies a wedged run.
    #h: A SLOW HEALTHY RUN MUST LIVE. 21 live logs: six carry one initial timeout,
    #   fifteen are still on `gamestart` with their first request pending. The old
    #   predicate pooled only the six and read STALL 6 - terminating a corpus
    #   without establishing that fifteen seats had failed at all.
    for i in 1 2 3 4 5 6; do
        printf '%s\n' '{"kind":"ask","seq":1,"fallback":"timeout"}' \
            > "$tmp/9999999999-ai_baka_deck$i-h$i.jsonl"
    done
    for i in 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21; do
        printf '%s\n' '{"kind":"gamestart","seq":0}' \
            > "$tmp/9999999999-ai_baka_deck$i-h$i.jsonl"
    done
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in OK*) ;; *) echo "pilot-stall-selftest FAIL: 15 seats still on their first ask gave '$v', want OK (a seat with no decision record is no evidence of a wedge)" >&2; fails=1;; esac
    rm -f "$tmp"/*.jsonl

    ##W75-CM (F9): Astra's two HEALTHY shapes. In both the pool reaches K from a
    # SUBSET of the live seats while another seat is being answered normally, so
    # the old verdict contradicted its own banner.
    #j: four live seats - three carry two timeouts each, the fourth's newest
    #   record is a decision the model answered. K=6, per-seat cap 2 -> the three
    #   failing seats fill the pool exactly.
    for i in 1 2 3; do
        printf '%s\n' '{"kind":"ask","seq":1,"fallback":"timeout"}' \
            '{"kind":"ask","seq":2,"fallback":"timeout"}' \
            > "$tmp/9999999999-ai_baka_deck$i-j$i.jsonl"
    done
    printf '%s\n' '{"kind":"ask","seq":1,"fallback":"timeout"}' \
        '{"kind":"ask","seq":2,"choice":1}' > "$tmp/9999999999-ai_baka_deck4-j4.jsonl"
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in OK*) ;; *) echo "pilot-stall-selftest FAIL: 4 live seats with one being answered gave '$v', want OK (a seat whose NEWEST record is a model answer is evidence against a stall)" >&2; fails=1;; esac
    rm -f "$tmp"/*.jsonl

    #k: twenty-one live seats - six failing, fifteen answering.
    for i in 1 2 3 4 5 6; do
        printf '%s\n' '{"kind":"ask","seq":1,"fallback":"timeout"}' \
            '{"kind":"ask","seq":2,"fallback":"timeout"}' \
            > "$tmp/9999999999-ai_baka_deck$i-k$i.jsonl"
    done
    for i in 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21; do
        printf '%s\n' '{"kind":"ask","seq":1,"choice":1}' \
            '{"kind":"ask","seq":2,"choice":2}' \
            > "$tmp/9999999999-ai_baka_deck$i-k$i.jsonl"
    done
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in OK*) ;; *) echo "pilot-stall-selftest FAIL: 21 live seats with 15 answering gave '$v', want OK" >&2; fails=1;; esac
    rm -f "$tmp"/*.jsonl

    #i: A STALLED TAIL MUST BE CAUGHT. One unfinished seat with twenty consecutive
    #   timeouts; every other seat finished. The old three-per-log pool could never
    #   reach K from one log, so it read OK 3 for ever.
    : > "$tmp/9999999999-ai_baka_deck1-i.jsonl"
    for i in $(seq 1 20); do
        printf '%s\n' "{\"kind\":\"ask\",\"seq\":$i,\"fallback\":\"timeout\"}" \
            >> "$tmp/9999999999-ai_baka_deck1-i.jsonl"
    done
    printf '%s\n' '{"kind":"ask","seq":1}' '{"kind":"gameend","seq":2,"won":true}' \
        > "$tmp/9999999999-ai_baka_deck2-i.jsonl"
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in STALL\ 6) ;; *) echo "pilot-stall-selftest FAIL: a 20-timeout live tail gave '$v', want 'STALL 6'" >&2; fails=1;; esac
    rm -f "$tmp"/*.jsonl

    ##W75-CI (P24): THE K-POOL MAY NOT FILL FROM ONE SEAT.
    #l: one wedged seat (its log the NEWEST, twenty consecutive timeouts) beside
    #   five seats the pilot is answering normally. The pool used to be filled
    #   seat by seat from the newest log, so that one seat supplied all six
    #   records and the tripwire stopped a corpus whose other twenty seats were
    #   fine. RED on base: this shape reads STALL 6 there.
    : > "$tmp/9999999999-ai_baka_deck1-l.jsonl"
    for i in $(seq 1 20); do
        printf '%s\n' "{\"kind\":\"ask\",\"seq\":$i,\"fallback\":\"timeout\"}" \
            >> "$tmp/9999999999-ai_baka_deck1-l.jsonl"
    done
    touch -d "@2000000000" "$tmp/9999999999-ai_baka_deck1-l.jsonl"
    for i in 2 3 4 5 6; do
        printf '%s\n' '{"kind":"ask","seq":1}' '{"kind":"ask","seq":2}' '{"kind":"ask","seq":3}' \
            > "$tmp/9999999999-ai_baka_deck$i-l.jsonl"
        touch -d "@1900000000" "$tmp/9999999999-ai_baka_deck$i-l.jsonl"
    done
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in OK\ 1) ;; *) echo "pilot-stall-selftest FAIL: one wedged seat among five answering seats gave '$v', want 'OK 1' - no seat may contribute more than ceil(K/live) to the pool" >&2; fails=1;; esac
    #m: and with ONE live seat the cap IS K, so case (i)'s stalled tail is
    #   unchanged - the bound adds breadth, it does not weaken the tripwire.
    rm -f "$tmp"/9999999999-ai_baka_deck[23456]-l.jsonl
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in STALL\ 6) ;; *) echo "pilot-stall-selftest FAIL: a lone live seat with a 20-timeout tail gave '$v', want 'STALL 6' (with one live seat the per-seat cap is K)" >&2; fails=1;; esac
    rm -f "$tmp"/*.jsonl

    #j: AN ARBITRARILY OLD RECORD MUST NOT VETO THE NEWEST STREAK. One unfinished
    #   log holds a single stale success; six newer failures sit in other live
    #   logs. The old predicate pooled the stale record beside them and read OK 6.
    #   The pool is now the newest K by log mtime, so the stale success is not in
    #   it - and the seat still counts as having evidence, so the run is judged.
    printf '%s\n' '{"kind":"ask","seq":1}' > "$tmp/9999999999-ai_baka_deck9-j.jsonl"
    touch -d "@1000000000" "$tmp/9999999999-ai_baka_deck9-j.jsonl"
    for i in 1 2 3 4 5 6; do
        printf '%s\n' "{\"kind\":\"ask\",\"seq\":$i,\"fallback\":\"timeout\"}" \
            > "$tmp/9999999999-ai_baka_deck$i-j.jsonl"
        touch -d "@2000000000" "$tmp/9999999999-ai_baka_deck$i-j.jsonl"
    done
    v=$(pilot_stall_verdict "$tmp" 1 6)
    case "$v" in STALL\ 6) ;; *) echo "pilot-stall-selftest FAIL: one stale success vetoed six newer failures, giving '$v', want 'STALL 6'" >&2; fails=1;; esac
    rm -f "$tmp"/*.jsonl

    ##W74-CF (F4): the tripwire and the published banner must agree about what
    # "the model never answered this" means. A log of wall_miss_no_retry is
    # endpoint silence by the tripwire's own class list; the banner counted 0 for
    # it and printed "every decision was answered by the model".
    printf '%s\n' \
      '{"kind":"wall_miss","seq":1,"fallback":"wall_miss_no_retry"}' \
      '{"kind":"priority","seq":2,"fallback":"empty_reply"}' \
      '{"kind":"priority","seq":3,"fallback":"http_error"}' \
      > "$tmp/9999999999-ai_baka_deck1-k.jsonl"
    v=$(silent_fallbacks_in "$tmp/9999999999-ai_baka_deck1-k.jsonl")
    case "$v" in 3) ;; *) echo "pilot-stall-selftest FAIL: the banner counted '$v' of 3 non-timeout silences, want '3' (it used to publish 'every decision answered by the model')" >&2; fails=1;; esac
    #and the two surfaces read the SAME list
    v=$(pilot_stall_verdict "$tmp" 1 3)
    case "$v" in STALL\ 3) ;; *) echo "pilot-stall-selftest FAIL: the tripwire read '$v' on the log the banner counts 3 silences in - the two surfaces disagree" >&2; fails=1;; esac
    #a decision the model DID answer badly is neither surface's business
    printf '%s\n' \
      '{"kind":"priority","seq":1,"fallback":"unparsed_reply"}' \
      > "$tmp/9999999999-ai_baka_deck2-k.jsonl"
    v=$(silent_fallbacks_in "$tmp/9999999999-ai_baka_deck2-k.jsonl")
    case "$v" in 0) ;; *) echo "pilot-stall-selftest FAIL: unparsed_reply counted as silence ('$v'), want '0'" >&2; fails=1;; esac

    ##W79-DB (T17, wave-78 deck152 MED-3 / known-bugs T17): A ZERO-ELIGIBLE
    # ENGINE-ANSWERED WINDOW IS NOT A FALLBACK. `152v123` seq 22 is a Sigarda
    # Coven reveal whose eligibility filter selected zero rows - no Human was
    # revealed, so `get a human` had no legal target and there was nothing to
    # ask. The engine answered correctly and the corpus census counted it as the
    # run's eighth fallback. Neither harness surface may count it: it is not
    # endpoint silence, and no decision was lost.
    printf '%s\n' \
      '{"kind":"reveal","seq":22,"fallback":"engine_answered","choice":-1,"latency_ms":-1}' \
      '{"kind":"recovery","seq":23,"recovers_fallback":"engine_answered"}' \
      > "$tmp/9999999999-ai_baka_deck152-w79.jsonl"
    v=$(silent_fallbacks_in "$tmp/9999999999-ai_baka_deck152-w79.jsonl")
    case "$v" in 0) ;; *) echo "pilot-stall-selftest FAIL: engine_answered counted as a silent fallback ('$v'), want '0'" >&2; fails=1;; esac
    v=$(pilot_stall_verdict "$tmp" 1 1)
    case "$v" in OK*) ;; *) echo "pilot-stall-selftest FAIL: an engine-answered window tripped the stall wire ('$v'), want OK" >&2; fails=1;; esac
    rm -f "$tmp"/*.jsonl

    rm -rf "$tmp"
    [ "$fails" = 0 ] && echo "pilot-stall-selftest: 19 checks, 0 failed"
    return "$fails"
}

#O26 (b): the per-game half of the same fact, for the results banner. A game that
# finished but answered N decisions on silent fallbacks is not a clean corpus game
# and the banner has to say so per game, not corpus-wide.
##W74-CF (F4, Astra review finding 4): it counted ONLY the exact string `timeout`
# and the banner then published "none - every decision was answered by the model".
# A log full of `wall_miss_no_retry` - a class the tripwire itself calls endpoint
# silence - counted 0 and was certified clean; `empty_reply` and `http_error` the
# same. One list ($PILOT_SILENT_CLASSES) now backs the tripwire, this helper and
# the banner, and the banner names the classes it counted.
silent_fallbacks_in() {
    python3 - "$1" <<'TFY'
import json, os, sys
SILENT = tuple(os.environ.get("PILOT_SILENT_CLASSES", "timeout").split())
n = 0
try:
    for line in open(sys.argv[1], errors="replace"):
        try: r = json.loads(line)
        except Exception: continue
        if str(r.get("fallback", "")).startswith(SILENT):
            n += 1
except OSError:
    pass
print(n)
TFY
}

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
        --selftest) harvest_selftest || exit 1; pilot_stall_selftest || exit 1; window_loop_selftest || exit 1; exec python3 "$(dirname "$0")/regime-gate.py" --selftest;;
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
    rm -f "$marker" "$marker.hung" "$marker.sz"
    #W77-CU (F10): this game's OWN seat-log basenames, straight off its own
    # stderr, appended to the outdir's exact-identity manifest.
    grep -oE 'WAGIC_GPT_TRANSLOG_FILE [^ ]+$' "$elog" 2>/dev/null \
        | awk '{print $2}' >> "$OUTDIR/.seatlogs"
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
        #W71-BP: the sidecars this loop writes match its own glob - skip them, or
        #a sweep reads a byte-count file as a marker (the .hung guard below only
        #covered the case by accident, and .sz would not have been covered at all).
        case "$m" in *.hung|*.sz) continue;; esac
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
        #W71-BP (L2): the stderr-growth arm. Anchor the byte count to the moment
        #the translog last moved, so growth is measured across the SILENCE and a
        #game that is deciding normally keeps re-anchoring and can never trip it.
        local elog2="$OUTDIR/game-${gd0}v${gd1}-${gstart}.stderr"
        local cur=0
        [ -e "$elog2" ] && cur=$(stat -c%s "$elog2" 2>/dev/null || echo 0)
        local pmark=0 psize=0 spin=0
        if [ -e "$m.sz" ]; then read -r pmark psize < "$m.sz"; fi
        if [ "${pmark:-0}" != "$quiet" ]; then
            echo "$quiet $cur" > "$m.sz"
        else
            local grew=$(( (cur - ${psize:-0}) / 1048576 ))
            if [ "$since" -ge "$FAST_HANG_QUIET_S" ] && [ "$grew" -ge "$FAST_HANG_MB" ]; then
                spin=1
                echo ""
                echo "!! GAME SPINNING: deck${gd0} vs deck${gd1} (started ${gstart}) wrote ${grew} MB"
                echo "!! of stderr while its seat logs stayed silent for ${since}s. A seat writing"
                echo "!! megabytes of the same line is a HANG, not thinking (wave-70 152v126:"
                echo "!! 2,584,190 identical iterations, 1.23 GB, one consumed decision)."
            fi
        fi
        if [ "$spin" -ne 1 ]; then
            [ "$since" -lt "$NO_PROGRESS_S" ] && continue
            echo ""
            echo "!! GAME HUNG: deck${gd0} vs deck${gd1} (started ${gstart}) has written NO seat"
            echo "!! translog record for ${since}s (limit ${NO_PROGRESS_S}s). The engine is not"
            echo "!! ticking - this is a dead loop, not a slow decision."
        fi
        echo "!! Stopping THAT GAME only."
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
        pilot_stall_sweep || return 1
        window_loop_sweep || return 1
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
if [ -f "$OUTDIR/PILOT-STALL" ]; then
    echo "== CORPUS FAILED: the pilot stalled (see above). The games still running when it"
    echo "== stalled were answered by the heuristic and are NOT corpus games; rerun them. =="
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
#W72-BT (M1, wave-71 engine-seat HIGH-1): the SIDECAR too. The engine writes one
# `ask_replay` record per silently re-served answer into `$LOGDIR/askreplay/<same
# basename>` - deliberately a SUBDIRECTORY, because both harness silence arms take
# max(mtime) over `$LOGDIR/*.jsonl` and a replay is NOT progress (W71-BS F4). The
# harvest used the same non-recursive glob, so 725 replay records - the corpus's
# only evidence of a re-serve loop - stayed outside the corpus and a reviewer
# reading only the corpus concluded "0 re-serves". Copy this run's basenames only:
# the sidecars are not snapshotted by $BEFORE_LIST and accumulate across runs.
harvest_translogs "$LOGDIR" "$OUTDIR" "$BEFORE_LIST"
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
#O26 (b): per-game timeout-fallback counts. The wave-73 corpus lost six games to
# a wedged pilot and the banner said nothing: the games finished, so nothing in the
# summary distinguished a full GPT game from one the heuristic played out. Any
# non-zero here means that seat spent decisions at the wall - read them before
# treating the game as evidence, and rerun the matchup if the count is material.
#W74-CF (F4): the SAME class list the stall tripwire uses. Counting only the exact
# string `timeout` let a log full of `wall_miss_no_retry` publish "every decision
# was answered by the model" - a certification the tripwire's own definition
# contradicts. The banner now says which classes it counted, so the two surfaces
# can be checked against each other by reading them.
SILENT = tuple(os.environ.get("PILOT_SILENT_CLASSES", "timeout").split())
to_fb = {}
by_class = Counter()
#W79-DB (T17): `engine_answered` is counted SEPARATELY and named as not-a-fallback.
# `152v123` seq 22 (wave 78) is a reveal whose eligibility filter selected zero
# rows: there was nothing to ask, the engine answered, no decision was lost. It is
# not endpoint silence and it is not a play problem, and reading it in the fallback
# tally cost the wave-78 engine seat a paragraph explaining that it was neither.
engine_answered = 0
for f in files:
    n = 0
    for line in open(f, errors="replace"):
        try: r = json.loads(line)
        except: continue
        fb = str(r.get("fallback", ""))
        if fb == "engine_answered":
            engine_answered += 1
            continue
        if fb.startswith(SILENT):
            n += 1
            by_class[fb] += 1
    if n: to_fb[os.path.basename(f)] = n
print(f"\n== silent fallbacks (decisions the model never answered; the heuristic played them) ==")
print(f"   counted classes: {', '.join(SILENT)}")
if not to_fb:
    print("  none of those classes appears in any seat log - every decision was answered by the model")
else:
    print(f"  {sum(to_fb.values())} across {len(to_fb)} of {len(files)} seat logs - these games are NOT clean corpus games")
    for c, n in by_class.most_common():
        print(f"  {n:5d}  class {c}")
    for b, n in sorted(to_fb.items(), key=lambda kv: -kv[1]):
        print(f"  {n:5d}  {b}")
if engine_answered:
    print(f"\n== engine-answered windows: {engine_answered} ==")
    print("   zero-eligible decisions (no legal option to offer) - the engine answered them;")
    print("   NOT a fallback, no round trip was owed and no decision was lost")
print(f"\nlogs + results.tsv in: {out}")
PY

#W54-F (D7c): a run that lost a game to a hang did not produce a complete corpus.
[ -f "$OUTDIR/HUNG" ] && exit 1
exit 0
