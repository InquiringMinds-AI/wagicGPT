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
import glob, os, re

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


def own_logs(logdir, own):
    """The full paths in `logdir` that belong to this run, sorted.

    `own` is an `own_seatlogs` result: None means no manifest and every *.jsonl
    in the directory is returned (the documented degrade); a set - including the
    EMPTY set, which is "nothing announced yet" - selects by exact basename."""
    files = sorted(glob.glob(os.path.join(logdir, '*.jsonl')))
    if own is None:
        return files
    return [f for f in files if os.path.basename(f) in own]
