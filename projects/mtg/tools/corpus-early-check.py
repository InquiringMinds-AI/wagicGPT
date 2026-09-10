#!/usr/bin/env python3
"""Early-check DIAGNOSIS for a running corpus (#W75-CI, P24).

Usage:  corpus-early-check.py <run outdir>            (preferred)
        corpus-early-check.py <launch script path>    (legacy, mtime-keyed)

WHY IT IS KEYED ON THE RUN, NOT ON mtime
----------------------------------------
The wave-74 version selected "this run's seats" as `mtime(seat log) >= mtime(launch
script)`.  Two ways that lies:

  * the translog directory ~/.Wagic/ai/gpt/logs is SHARED by every wagic process on
    the box - a probe, a fixture, a GUI game or a second harness writes into it and
    those logs are newer than the launch script, so they count as this run's seats
    (wave-74 CG: a probe's logs showed the corpus as "72 seats");
  * mtime is rewritten by the harness's end-of-run harvest, so the same predicate
    read against a finished run is meaningless.

A seat log's FILENAME carries the epoch its game started (`<epoch>-ai_baka_deckN-...`),
written once and never rewritten.  The harness creates `game-<d0>v<d1>-<gstart>.stderr`
in the OUTDIR at the moment it starts each game, live - so the OUTDIR itself is a
manifest of exactly which games this run started, at which second, with which decks.
A seat log belongs to this run when its filename epoch is within a few seconds of one
of those `gstart`s AND its deck is one of that game's two decks.  A launch-epoch window
alone is NOT enough: running this against the wave-74 corpus dir picks up 80 "seats",
because lane CH's deck198/deck199 probe games ran inside the same window.  The manifest
rule reads the same directory as 42.

Fallback: before the first game file exists (or for a legacy launch-script argument)
the tool degrades to `filename epoch >= start - 5`, the same rule the harness's own
tripwires use, and says so on its first line.

WHAT IT IS FOR (skill, "the EARLY corpus check is a DIAGNOSIS, not a liveness glance")
More than ~10 asks by one seat in one (turn, phase), or a `declined this exact list N
times` clause with N > 10, is a WINDOW LOOP: stop the corpus and look.  Fresh mtimes,
rising ask counts and zero fallbacks all read HEALTHY while a window loop burns hours.
"""
import sys, os, glob, json, re, collections, datetime

def run_start_epoch(arg):
    """(epoch, how) for a run outdir, or for a legacy launch-script path."""
    base = os.path.basename(os.path.normpath(arg))
    m = re.match(r'^\w+-(\d{8})-(\d{6})', base)
    if os.path.isdir(arg) and m:
        t = datetime.datetime.strptime(m.group(1) + m.group(2), "%Y%m%d%H%M%S")
        return int(t.timestamp()), "outdir name %s" % base
    if os.path.isdir(arg):
        return int(os.path.getmtime(arg)), "outdir mtime (name carries no timestamp)"
    return int(os.path.getmtime(arg)), "LEGACY: mtime of %s" % base

GAME_RE = re.compile(r'^game-(\w+?)v(\w+?)-(\d+)\.stderr$')

def run_manifest(arg):
    """[(gstart, {deck0, deck1})] for every game this run has STARTED, read off the
    outdir's own live game-*.stderr files.  Empty when there is no manifest yet."""
    if not os.path.isdir(arg):
        return []
    out = []
    for g in glob.glob(os.path.join(arg, 'game-*.stderr')):
        m = GAME_RE.match(os.path.basename(g))
        if m:
            out.append((int(m.group(3)), {m.group(1), m.group(2)}))
    return out

SEAT_RE = re.compile(r'^(\d+)-ai_baka_(\w+?)-')

def belongs(ep, base, start, manifest, slack=15):
    """`slack` covers the gap between the harness stamping `gstart` and the engine
    creating the seat log (measured at 2-5 s on the wave-74 corpus)."""
    if not manifest:
        return ep >= start - slack
    m = SEAT_RE.match(base)
    #the seat log spells the deck `deck125`, the game file spells it `125`
    deck = m.group(2)[4:] if (m and m.group(2).startswith("deck")) else (m.group(2) if m else None)
    for gstart, decks in manifest:
        if abs(ep - gstart) <= slack and (deck is None or deck in decks):
            return True
    return False

def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    start, how = run_start_epoch(sys.argv[1])
    manifest = run_manifest(sys.argv[1])
    if manifest:
        how += "; manifest of %d game(s) from the outdir" % len(manifest)
    else:
        how += "; NO game manifest yet - falling back to the launch-epoch window"
    logdir = os.path.expanduser('~/.Wagic/ai/gpt/logs')
    worst, tot, seats, skipped = [], 0, 0, 0
    fb = collections.Counter()
    for f in sorted(glob.glob(logdir + '/*.jsonl')):
        base = os.path.basename(f)
        try:
            ep = int(base.split('-')[0])
        except ValueError:
            skipped += 1
            continue
        if not belongs(ep, base, start, manifest):
            skipped += 1
            continue
        seats += 1
        c = collections.Counter()
        mx = 0
        for l in open(f, errors='replace'):
            try:
                r = json.loads(l)
            except Exception:
                continue
            if r.get('kind') != 'ask':
                continue
            tot += 1
            c[(r.get('turn'), r.get('phase'))] += 1
            if r.get('fallback'):
                fb[r['fallback']] += 1
            m = re.search(r'declined this exact list (\d+) time', r.get('prompt') or '')
            if m:
                mx = max(mx, int(m.group(1)))
        if c:
            (t, p), n = c.most_common(1)[0]
            worst.append((n, mx, os.path.basename(f)[11:32], t, p))
    worst.sort(reverse=True)
    print("run start %d (%s)" % (start, how))
    print("seats %d asks %d fallbacks %s  (%d log(s) in the shared dir are NOT this run)"
          % (seats, tot, dict(fb), skipped))
    for n, mx, name, t, p in worst[:4]:
        print("  seat %s: max asks in one phase %d (turn %s %s), max declined-count %d"
              % (name, n, t, p, mx))
    flag = [w for w in worst if w[0] > 10 or w[1] > 10]
    if flag:
        print("WINDOW-LOOP SUSPECT: %d seat(s) - stop the corpus and read the named seat's"
              " newest records before waiting for the late check" % len(flag))
    else:
        print("no window-loop signature")

if __name__ == '__main__':
    main()
