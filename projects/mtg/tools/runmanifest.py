#!/usr/bin/env python3
#W81-DJ: THE RUN'S OWN SEAT LOGS - one answer, shared by every harness check.
#
# $HOME/.Wagic/ai/gpt/logs is SHARED. Every pool, every probe, every suite
# fixture and every concurrently running lane writes its seat translogs into
# that one directory, and a filename's epoch says only WHEN a log was opened,
# never WHOSE it is. Wave 76 learned this for the HARVEST (19 concurrent
# one-game pools each harvested the others' in-flight logs) and wave 77 CU F10
# fixed it there by making each game ANNOUNCE its two seat-log basenames on its
# own stderr (`WAGIC_GPT_TRANSLOG_FILE <base>`, printed by AIPlayerGPT when the
# translog opens) and recording them in `$OUTDIR/.seatlogs` as the game is
# reaped.
#
# Every OTHER check kept scanning the shared directory by time, and on
# 2026-09-12 that killed a valid run: a single-game rerun's own two seats PASSED
# the regime gate ("10 records, all carry reasoning"), then the next sweep read
# 20 records of a DIFFERENT lane's suite-fixture game (deck198 vs deck199, GPT
# stub endpoint, thinking off) out of the shared directory and declared the
# corpus invalid. Nothing was wrong with the corpus.
#
# So identity is not recency, and this module is the only place that answers
# "is this seat log mine": the run's own announcements, nothing else.
#
# THE ONE DEGRADE, and it is deliberate: an outdir with no `game-*.stderr` and
# no `.seatlogs` has no announcement channel at all (a probe directory, a
# selftest fixture, an old corpus layout), and `own_seatlogs` returns None
# meaning "no manifest - do not filter". A LIVE run always has its per-game
# stderr file, created before the game starts, so a live sweep can never take
# that branch; what it CAN see is a manifest that is still EMPTY because no game
# has announced yet, and an empty set is the honest answer there - the check
# waits for the announcement instead of scanning the directory.
import glob, os, re, sys

#The announcement the engine prints once per seat when the translog opens.
ANNOUNCE_RE = re.compile(r'WAGIC_GPT_TRANSLOG_FILE\s+(\S+)\s*$')

#A game's stderr can reach gigabytes when the engine spins (wave-70: 1.23 GB of
#one repeated line), and these checks run every 45 s. The announcements are
#written when the first decision opens the translog - 5-7 KB into a normal
#game's stderr - so reading a generous head of the file is both cheap and far
#past where they can be. The cap is on BYTES READ, not on lines matched.
STDERR_SCAN_BYTES = 8 * 1024 * 1024


def _announced(path, out):
    try:
        with open(path, errors='replace') as fh:
            read = 0
            for line in fh:
                read += len(line)
                m = ANNOUNCE_RE.search(line)
                if m:
                    out.add(os.path.basename(m.group(1)))
                if read >= STDERR_SCAN_BYTES:
                    break
    except OSError:
        pass


def own_seatlogs(outdir, game=None):
    """The seat-log BASENAMES this run announced, or None if it cannot announce.

    `outdir` is the run's output directory. With `game` (a
    `game-<d0>v<d1>-<gstart>.stderr` basename) the answer is that ONE game's two
    seats, which is the identity the per-game watchdogs need; without it, every
    seat log the whole run has announced so far.

    Sources, both written by the run itself and neither able to name a foreign
    log: `$OUTDIR/.seatlogs` (appended as each game is reaped) and the live
    `WAGIC_GPT_TRANSLOG_FILE` line on each game's own stderr (present from the
    game's first decision, so an in-flight game is covered too).

    Returns None - "no manifest, do not filter" - only when the outdir has no
    announcement channel at all: see the module note."""
    if not outdir or not os.path.isdir(outdir):
        return None
    stderrs = ([os.path.join(outdir, game)] if game
               else sorted(glob.glob(os.path.join(outdir, 'game-*.stderr'))))
    named = os.path.join(outdir, '.seatlogs')
    have_named = os.path.exists(named)
    if not have_named and not any(os.path.exists(p) for p in stderrs):
        return None
    own = set()
    #`.seatlogs` is flat (the whole run), so it answers the run-wide question
    #only; a per-game question is answered by that game's own stderr alone.
    if game is None and have_named:
        try:
            with open(named, errors='replace') as fh:
                own.update(os.path.basename(l.strip()) for l in fh if l.strip())
        except OSError:
            pass
    for p in stderrs:
        _announced(p, own)
    return own


#W81-DM (V2 residual). The degrade above is deliberate, but wave 80 shipped it
#SILENT: a manifest-less outdir fell back to scanning the shared directory by
#time and every consumer's output looked identical to a manifest-keyed one. That
#is the exact shape the wave-80 false regime gate cost a rerun for, so the
#degraded path now SAYS SO, before it returns anything, on the first line of the
#degraded read. One announcement per process (the six harness sites each run in
#their own python), on stderr so no consumer's parsed stdout changes.
MANIFEST_ABSENT_LINE = 'MANIFEST ABSENT - scanning by time'
_announced_degrade = False


def announce_degrade(stream=None):
    """Print the degrade banner once per process. Returns True if it printed."""
    global _announced_degrade
    if _announced_degrade:
        return False
    _announced_degrade = True
    print(MANIFEST_ABSENT_LINE, file=stream if stream is not None else sys.stderr)
    return True


def own_logs(logdir, own):
    """The full paths in `logdir` that belong to this run, sorted.

    `own` is an `own_seatlogs` result: None means no manifest and every *.jsonl
    in the directory is returned (the documented degrade, which announces itself
    via `announce_degrade`); a set - including the EMPTY set, which is "nothing
    announced yet" - selects by exact basename."""
    files = sorted(glob.glob(os.path.join(logdir, '*.jsonl')))
    if own is None:
        announce_degrade()
        return files
    return [f for f in files if os.path.basename(f) in own]


def _selftest():
    import tempfile
    global _announced_degrade
    fails = []

    def check(cond, what):
        print('%s  %s' % ('ok  ' if cond else 'FAIL', what))
        if not cond:
            fails.append(what)

    root = tempfile.mkdtemp(prefix='runmanifest-selftest-')
    logdir = os.path.join(root, 'logs')
    os.makedirs(logdir)
    mine = os.path.join(logdir, '1789216933-ai_baka_deck130-0xAA-vs-ai_baka_deck126.jsonl')
    theirs = os.path.join(logdir, '1789239545-ai_baka_deck198-0xBB-vs-ai_baka_deck199.jsonl')
    for f in (mine, theirs):
        open(f, 'w').close()

    #(1) A manifest-less outdir returns None and its read announces the degrade
    #    on the FIRST line it emits, and returns every jsonl in the directory.
    bare = os.path.join(root, 'bare')
    os.makedirs(bare)
    own = own_seatlogs(bare)
    check(own is None, 'manifest-less outdir -> own_seatlogs None')
    import io
    _announced_degrade = False
    buf = io.StringIO()
    err = sys.stderr
    sys.stderr = buf
    try:
        got = own_logs(logdir, own)
    finally:
        sys.stderr = err
    first = buf.getvalue().splitlines()[:1]
    check(first == [MANIFEST_ABSENT_LINE],
          'degraded read prints %r on its first line (got %r)'
          % (MANIFEST_ABSENT_LINE, first))
    check(got == sorted([mine, theirs]), 'degraded read returns every jsonl')

    #(2) The banner is once per process, not once per call.
    buf2 = io.StringIO()
    sys.stderr = buf2
    try:
        own_logs(logdir, None)
    finally:
        sys.stderr = err
    check(buf2.getvalue() == '', 'degrade banner prints once per process')

    #(3) NEGATIVE: an announcing outdir keys by name, takes no degrade branch and
    #    prints NOTHING - the foreign log is not returned.
    live = os.path.join(root, 'live')
    os.makedirs(live)
    with open(os.path.join(live, 'game-130v126-1789216933.stderr'), 'w') as fh:
        fh.write('noise\nWAGIC_GPT_TRANSLOG_FILE %s\nmore noise\n' % mine)
    own2 = own_seatlogs(live)
    check(own2 == {os.path.basename(mine)}, 'announcing outdir -> the run\'s own basename')
    _announced_degrade = False
    buf3 = io.StringIO()
    sys.stderr = buf3
    try:
        got2 = own_logs(logdir, own2)
    finally:
        sys.stderr = err
    check(got2 == [mine], 'keyed read returns only the run\'s own log')
    check(buf3.getvalue() == '', 'keyed read prints no degrade banner')

    #(4) The EMPTY set is "nothing announced yet", NOT a degrade.
    _announced_degrade = False
    buf4 = io.StringIO()
    sys.stderr = buf4
    try:
        got3 = own_logs(logdir, set())
    finally:
        sys.stderr = err
    check(got3 == [] and buf4.getvalue() == '',
          'empty manifest selects nothing and does not announce a degrade')

    import shutil
    shutil.rmtree(root, ignore_errors=True)
    print('runmanifest selftest: %s' % ('OK' if not fails else '%d FAILED' % len(fails)))
    return 1 if fails else 0


if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == '--selftest':
        sys.exit(_selftest())
    print(__doc__ or 'runmanifest: a library; run with --selftest', file=sys.stderr)
    sys.exit(2)
