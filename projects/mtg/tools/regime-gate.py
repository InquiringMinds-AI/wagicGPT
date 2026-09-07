#!/usr/bin/env python3
#W70-BK (C2): THE PER-REGIME CORPUS GATE.
#
# Skill invariant 000 (owner ruling 2026-09-06): reasoning happens ONLY in the
# native reasoning channel, the reply is a PLAN line then an action line and
# nothing else, and the harness must "gate each regime on its own evidence
# (on: reasoning_content present; off: reply = exactly PLAN + action)".
#
# WHY THIS EXISTS: every corpus from wave 44 to wave 69 ran with thinking OFF -
# the harness default was never overridden and NOTHING in the data said so. The
# regime is now a required launch argument; this is the check that the binary,
# the server and the model actually delivered the regime that was asked for,
# read off the first records of the first games. A failed gate kills the corpus
# in minutes instead of producing an invalid one in hours.
#
# It is a ONE-SHOT check, not a monitor (fleet rule feedback-single-wake-batching):
# the harness calls it until it returns PASS or FAIL, and then never again.
#
# Output is exactly one line on stdout: "PASS ...", "WAIT ...", or "FAIL ...".
# Exit status is 0 for PASS/WAIT, 1 for FAIL (and for a --selftest failure).
import argparse, glob, json, os, re, sys, tempfile

# The action labels the seams emit. The gate only asks "is this line an action
# line"; WHICH action it is belongs to the engine's parsers, not here.
ACTION_RE = re.compile(r'^(CHOICE|ATTACK|BLOCKS|PUT|ORDER|X|KEEP|MULLIGAN|DISCARD|REVEAL|BOTTOM)\s*:', re.I)
PLAN_RE = re.compile(r'^PLAN\s*:', re.I)


def reply_shape(reply):
    """'two_line' | 'action_first' | 'plan_only' | 'action_only' | 'off_protocol'.

    The permitted shape is EXACTLY two non-blank lines: PLAN first, the action
    line second. Anything else is a violation; the name says which one so a
    failed gate reports what the model actually wrote."""
    lines = [l.strip() for l in (reply or '').split('\n')]
    lines = [l for l in lines if l]
    plan = [i for i, l in enumerate(lines) if PLAN_RE.match(l)]
    act = [i for i, l in enumerate(lines) if ACTION_RE.match(l)]
    if len(lines) == 2 and plan == [0] and act == [1]:
        return 'two_line'
    if len(lines) == 2 and plan == [1] and act == [0]:
        return 'action_first'
    if plan and not act:
        return 'plan_only'
    if act and not plan:
        return 'action_only'
    return 'off_protocol'


def run_records(logdir, start, per_file):
    """The first `per_file` GATEABLE records of each seat log this run wrote.

    #W70-BN (F7, Astra review finding 7): a gateable record is one that carried a
    COMPLETED MODEL ROUND TRIP - a non-empty `prompt`. `writeTransLog` always
    writes `reply` and deliberately omits `reasoning_chars` when the prompt is
    empty, so the shipped reader counted every engine-answered record as missing
    instrumentation and the early sweep returned "FAIL - rebuild" on a current
    binary. Engine-answered records are SKIPPED; only a record that called the
    model and still carries no `reasoning_chars` is evidence of a stale binary.

    Returns (per-file dict of records, unstamped count, file count)."""
    out, unstamped, files = {}, 0, 0
    for f in sorted(glob.glob(os.path.join(logdir, '*.jsonl'))):
        try:
            ep = int(os.path.basename(f).split('-')[0])
        except ValueError:
            continue
        if ep < start - 2:
            continue
        files += 1
        name = os.path.basename(f)
        taken = 0
        try:
            fh = open(f)
        except OSError:
            continue
        with fh:
            for line in fh:
                if taken >= per_file:
                    break
                try:
                    r = json.loads(line)
                except Exception:
                    continue
                if r.get('kind') in ('gamestart', 'gameend'):
                    continue
                if not r.get('prompt'):
                    continue          # engine-answered, cache hit or reuse
                if 'reasoning_chars' not in r:
                    unstamped += 1
                    continue
                out.setdefault(name, []).append(r)
                taken += 1
    return out, unstamped, files


def gate(per_file, unstamped, regime, prose_abort_pct, min_records, checked=()):
    """One verdict over the seat logs this run wrote.

    `per_file` maps a seat-log name to its first N gateable records. `checked` is
    the set of seat logs a previous sweep already PASSED - #W70-BN (F8): PASS is
    not a permanent amnesty, it is a statement about the seats seen so far, and a
    seat log that appears later is gated on its own first records.

    Returns (verdict, why, files_now_checked)."""
    if isinstance(per_file, list):        # tolerate the flat form in tests
        d = {}
        for f, r in per_file:
            d.setdefault(f, []).append(r)
        per_file = d
    pending = sorted(f for f in per_file if f not in checked)
    records = [(f, r) for f in pending for r in per_file[f]]
    if unstamped and not records:
        return ('FAIL', 'the seat translogs carry no `reasoning_chars` stamp on %d records that '
                        'DID call the model: this binary predates #W70-BK and the regime cannot '
                        'be verified - rebuild.' % unstamped, [])
    if not records:
        return ('WAIT', 'no gateable records yet', [])
    # #W70-BN (F8): the minimum is PER SEAT LOG. Five records from one fast seat
    # said nothing about the seat that had not answered yet.
    short = [f for f in pending if len(per_file[f]) < min_records]
    if short:
        return ('WAIT', 'seat log %s has only %d of %d gateable records'
                        % (short[0], len(per_file[short[0]]), min_records), [])

    stamp_bad = [(f, r.get('thinking')) for f, r in records if r.get('thinking') != regime]
    if stamp_bad:
        return ('FAIL', 'the binary played %d of %d checked records with thinking=%s while the launch '
                        'asked for %s (first: %s) - the regime did not reach the engine.'
                        % (len(stamp_bad), len(records), stamp_bad[0][1], regime, stamp_bad[0][0]), [])

    shapes = {}
    for _, r in records:
        s_ = reply_shape(r.get('reply', ''))
        shapes[s_] = shapes.get(s_, 0) + 1
    off_shape = len(records) - shapes.get('two_line', 0)
    shape_note = ' shapes: ' + ', '.join('%s=%d' % kv for kv in sorted(shapes.items()))
    seat_note = ' seats: %d' % len(pending)

    if regime == 'on':
        hidden = [f for f, r in records if r.get('reasoning_hidden')]
        if hidden:
            return ('FAIL', 'reasoning was requested and the provider WITHHELD the trace on %d of %d '
                            'checked records (first: %s). The reviewers read the reasoning channel; a '
                            'corpus without it is invalid (invariant 000(a)).'
                            % (len(hidden), len(records), hidden[0]), [])
        empty = [f for f, r in records if not r.get('reasoning_chars')]
        if empty:
            return ('FAIL', 'thinking=on but %d of %d checked records carry reasoning_chars 0 '
                            '(first: %s). The model is not reasoning in the reasoning channel - '
                            'the corpus is INVALID (invariant 000(a)).'
                            % (len(empty), len(records), empty[0]), [])
        # #W70-BN (F6, Astra review finding 6): the ALLOCATION check is about the
        # request that funded the thinking window. The forced close is phase TWO -
        # it resumes an already-closed <think> block with enable_thinking false, so
        # it legitimately allocates 0 reasoning tokens and stamps
        # `reasoning_forced_close`. A record that carries native reasoning and that
        # stamp is a SUCCESSFUL recovery, not a capped window; failing it killed
        # corpora for working correctly.
        capped = [f for f, r in records
                  if 'max_tokens_reasoning' in r and r.get('max_tokens_reasoning', 0) <= 0
                  and not (r.get('reasoning_forced_close') and r.get('reasoning_chars'))]
        if capped:
            return ('FAIL', 'thinking=on but %d of %d checked records were decoded with a reasoning '
                            'budget of 0 tokens (first: %s) - a cap is bounding the thinking window '
                            '(invariant 000(d)).' % (len(capped), len(records), capped[0]), [])
        warn = ''
        if off_shape:
            warn = (' WARN %d of %d replies are not the two-line shape (reported, not fatal under '
                    'thinking on).' % (off_shape, len(records)))
        return ('PASS', 'thinking=on: %d records, all carry reasoning.%s%s%s'
                        % (len(records), warn, seat_note, shape_note), pending)

    # regime == 'off'
    reasoned = [f for f, r in records if r.get('reasoning_chars')]
    if reasoned:
        return ('FAIL', 'thinking=off but %d of %d checked records carry reasoning text '
                        '(first: %s) - the server ignored enable_thinking:false and the run is not '
                        'the product regime it claims to be.'
                        % (len(reasoned), len(records), reasoned[0]), [])
    # #W70-BN (F9, Astra review finding 9): reasoning that is WITHHELD is still
    # reasoning. `reasoning_chars` is 0 whenever the provider hides the trace, so
    # the off arm accepted a run that reasoned for 1,200 tokens per decision
    # behind `reasoning_hidden`. Either field is affirmative evidence.
    hidden = [f for f, r in records if r.get('reasoning_hidden')]
    if hidden:
        return ('FAIL', 'thinking=off but %d of %d checked records report reasoning_hidden (first: '
                        '%s) - the provider reasoned and withheld the trace, so this is not the '
                        'product regime.' % (len(hidden), len(records), hidden[0]), [])
    billed = [f for f, r in records if (r.get('reasoning_tokens') or 0) > 0]
    if billed:
        return ('FAIL', 'thinking=off but %d of %d checked records were billed reasoning tokens '
                        '(first: %s) - the model reasoned whatever the reply shows.'
                        % (len(billed), len(records), billed[0]), [])
    pct = 100.0 * off_shape / len(records)
    if pct > prose_abort_pct:
        return ('FAIL', 'thinking=off and %.1f%% of the first %d replies are not exactly a PLAN line '
                        'plus an action line (limit %.1f%%).%s The reply protocol is not being '
                        'followed - stop and fix the instructions, not the parsers.'
                        % (pct, len(records), prose_abort_pct, shape_note), [])
    return ('PASS', 'thinking=off: %d records, no reasoning text, %.1f%% off-protocol replies '
                    '(limit %.1f%%).%s%s' % (len(records), pct, prose_abort_pct, seat_note,
                                             shape_note), pending)


def selftest():
    """Feed the gate synthetic records. No server, no game, no corpus."""
    fails = []
    n = [0]

    def check(cond, label):
        n[0] += 1
        print(('  ok   ' if cond else '  FAIL ') + label)
        if not cond:
            fails.append(label)

    def rec(f='f.jsonl', **kw):
        r = {'kind': 'ask', 'reasoning_chars': 0, 'thinking': 'on', 'prompt': 'p',
             'reply': 'PLAN: a. b.\nCHOICE: 2 (Cast Bear)'}
        r.update(kw)
        return (f, r)

    on_ok = [rec(reasoning_chars=900) for _ in range(5)]
    check(gate(on_ok, 0, 'on', 5.0, 5)[0] == 'PASS', 'thinking on + reasoning on every record = PASS')
    on_bad = on_ok[:4] + [rec(reasoning_chars=0)]
    v, why, _ = gate(on_bad, 0, 'on', 5.0, 5)
    check(v == 'FAIL' and 'reasoning_chars 0' in why, 'thinking on + ONE record without reasoning = FAIL')
    hid = on_ok[:4] + [rec(reasoning_chars=0, reasoning_hidden=True)]
    check(gate(hid, 0, 'on', 5.0, 5)[0] == 'FAIL', 'thinking on + a withheld trace = FAIL')
    capd = [rec(reasoning_chars=900, max_tokens_reasoning=0) for _ in range(5)]
    v, why, _ = gate(capd, 0, 'on', 5.0, 5)
    check(v == 'FAIL' and '000(d)' in why, 'thinking on + a 0-token reasoning budget = FAIL (ruling (d))')
    off_ok = [rec(thinking='off') for _ in range(5)]
    check(gate(off_ok, 0, 'off', 5.0, 5)[0] == 'PASS', 'thinking off + two-line replies = PASS')
    off_reason = off_ok[:4] + [rec(thinking='off', reasoning_chars=120)]
    check(gate(off_reason, 0, 'off', 5.0, 5)[0] == 'FAIL', 'thinking off + reasoning text = FAIL')
    prose = [rec(thinking='off', reply='I should think about this.\nCHOICE: 2 (Cast Bear)\nPLAN: a.')
             for _ in range(5)]
    v, why, _ = gate(prose, 0, 'off', 5.0, 5)
    check(v == 'FAIL' and 'not exactly a PLAN line' in why, 'thinking off + prose replies past the threshold = FAIL')
    one_bad = [rec(thinking='off') for _ in range(19)] + [rec(thinking='off', reply='hmm')]
    check(gate(one_bad, 0, 'off', 10.0, 5)[0] == 'PASS', 'one off-protocol reply in 20 is under a 10% limit')
    check(gate(one_bad, 0, 'off', 1.0, 5)[0] == 'FAIL', 'the same 5% is over a 1% limit')
    mism = [rec(thinking='off') for _ in range(5)]
    v, why, _ = gate(mism, 0, 'on', 5.0, 5)
    check(v == 'FAIL' and 'did not reach the engine' in why, 'a record stamped off under --thinking on = FAIL')
    check(gate([], 0, 'on', 5.0, 5)[0] == 'WAIT', 'no records yet = WAIT, never a verdict')
    check(gate([], 7, 'on', 5.0, 5)[0] == 'FAIL', 'records with no regime stamp at all = FAIL (stale binary)')
    check(reply_shape('PLAN: a\nCHOICE: 1 (x)') == 'two_line', 'the permitted shape')
    check(reply_shape('CHOICE: 1 (x)\nPLAN: a') == 'action_first', 'action before plan is named, not accepted')
    check(reply_shape('CHOICE: 1 (x)') == 'action_only', 'a bare action line is named')
    check(reply_shape('PLAN: a\nBecause the bear is big\nCHOICE: 1 (x)') == 'off_protocol',
          'prose between the two lines is off-protocol')

    # ---- #W70-BN: the four Astra review findings against this gate ----------
    # F6: a SUCCESSFUL forced-close recovery. Phase one reasoned, phase two
    # allocated 0 reasoning tokens and returned the answer. Native reasoning is
    # on the record; the shipped gate FAILED it and killed the corpus.
    fc = ([rec(reasoning_chars=900, max_tokens_reasoning=6000) for _ in range(4)]
          + [rec(reasoning_chars=900, max_tokens_reasoning=0, reasoning_forced_close=True)])
    check(gate(fc, 0, 'on', 5.0, 5)[0] == 'PASS',
          '#W70-BN F6 a forced-close record that CARRIES reasoning PASSES')
    fc_bad = ([rec(reasoning_chars=900, max_tokens_reasoning=6000) for _ in range(4)]
              + [rec(reasoning_chars=0, max_tokens_reasoning=0, reasoning_forced_close=True)])
    check(gate(fc_bad, 0, 'on', 5.0, 5)[0] == 'FAIL',
          '#W70-BN F6 MUST-NOT-MATCH the stamp is not a blanket excuse - no reasoning still FAILs')
    # F9: withheld or billed reasoning under the OFF regime.
    off_hidden = ([rec(thinking='off') for _ in range(4)]
                  + [rec(thinking='off', reasoning_hidden=True)])
    v, why, _ = gate(off_hidden, 0, 'off', 5.0, 5)
    check(v == 'FAIL' and 'reasoning_hidden' in why,
          '#W70-BN F9 thinking off + a WITHHELD trace = FAIL')
    off_billed = ([rec(thinking='off') for _ in range(4)]
                  + [rec(thinking='off', reasoning_tokens=1200)])
    v, why, _ = gate(off_billed, 0, 'off', 5.0, 5)
    check(v == 'FAIL' and 'billed reasoning tokens' in why,
          '#W70-BN F9 thinking off + BILLED reasoning tokens = FAIL')
    check(gate([rec(thinking='off', reasoning_tokens=0) for _ in range(5)], 0, 'off', 5.0, 5)[0] == 'PASS',
          '#W70-BN F9 MUST-NOT-MATCH an explicit reasoning_tokens 0 is not evidence of reasoning')
    # F8: the minimum is PER SEAT LOG, and PASS is not a permanent amnesty.
    two_seats = ([rec('a.jsonl', reasoning_chars=900) for _ in range(5)]
                 + [rec('b.jsonl', reasoning_chars=900) for _ in range(2)])
    v, why, _ = gate(two_seats, 0, 'on', 5.0, 5)
    check(v == 'WAIT' and 'b.jsonl' in why,
          '#W70-BN F8 a seat log short of the minimum holds the verdict at WAIT')
    v, why, passed = gate([rec('a.jsonl', reasoning_chars=900) for _ in range(5)], 0, 'on', 5.0, 5)
    check(v == 'PASS' and passed == ['a.jsonl'], '#W70-BN F8 PASS names the seat logs it checked')
    later = ([rec('a.jsonl', reasoning_chars=900) for _ in range(5)]
             + [rec('b.jsonl', reasoning_chars=0) for _ in range(5)])
    check(gate(later, 0, 'on', 5.0, 5, checked=['a.jsonl'])[0] == 'FAIL',
          '#W70-BN F8 a LATER seat with no reasoning still kills the corpus after a PASS')
    check(gate(later[:5], 0, 'on', 5.0, 5, checked=['a.jsonl'])[0] == 'WAIT',
          '#W70-BN F8 ...and an already-checked seat alone is not re-judged')

    # And end to end over real files on disk, so the reader is exercised too.
    with tempfile.TemporaryDirectory() as td:
        path = os.path.join(td, '2000000000-ai_baka_deck44-0xa-vs-ai_baka_deck9.jsonl')
        with open(path, 'w') as fh:
            fh.write(json.dumps({'kind': 'gamestart'}) + '\n')
            for i in range(6):
                fh.write(json.dumps({'kind': 'ask', 'seq': i, 'thinking': 'on', 'prompt': 'p',
                                     'reasoning_chars': 500, 'max_tokens_reasoning': 6000,
                                     'reply': 'PLAN: a\nCHOICE: 1 (x)'}) + '\n')
        recs, unst, nf = run_records(td, 2000000000, 5)
        check(sum(len(v) for v in recs.values()) == 5 and unst == 0 and nf == 1,
              'the reader takes the first N records of each seat log')
        check(gate(recs, unst, 'on', 5.0, 5)[0] == 'PASS', 'end to end over files on disk = PASS')
        old = os.path.join(td, '1999999999-ai_baka_deck44-0xb-vs-ai_baka_deck9.jsonl')
        with open(old, 'w') as fh:
            fh.write(json.dumps({'kind': 'ask', 'prompt': 'p', 'reply': 'x'}) + '\n')
        recs2, unst2, _ = run_records(td, 2000000000, 5)
        check(sum(len(v) for v in recs2.values()) == 5, 'a log from BEFORE this run is not gated')

    # #W70-BN F7: engine-answered records are not evidence of a stale binary.
    with tempfile.TemporaryDirectory() as td:
        path = os.path.join(td, '2000000000-ai_baka_deck44-0xa-vs-ai_baka_deck9.jsonl')
        with open(path, 'w') as fh:
            for i in range(6):
                fh.write(json.dumps({'kind': 'ask', 'seq': i, 'prompt': '', 'reply': '',
                                     'choice': -1, 'fallback': 'engine_answered'}) + '\n')
        recs, unst, _ = run_records(td, 2000000000, 5)
        v, why, _ = gate(recs, unst, 'on', 5.0, 5)
        check(unst == 0 and v == 'WAIT',
              '#W70-BN F7 engine-answered records are SKIPPED, and no gateable record is WAIT')
        with open(os.path.join(td, '2000000001-ai_baka_deck9-0xb-vs-ai_baka_deck44.jsonl'), 'w') as fh:
            fh.write(json.dumps({'kind': 'ask', 'prompt': 'p', 'reply': 'PLAN: a\nCHOICE: 1 (x)'}) + '\n')
        recs, unst, _ = run_records(td, 2000000000, 5)
        v, why, _ = gate(recs, unst, 'on', 5.0, 5)
        check(unst == 1 and v == 'FAIL' and 'rebuild' in why,
              '#W70-BN F7 ...but a record that DID call the model and carries no stamp is a stale binary')

    print('regime-gate selftest: %d checks, %d failed' % (n[0], len(fails)))
    return 1 if fails else 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--logdir')
    ap.add_argument('--start', type=int)
    ap.add_argument('--regime', choices=['on', 'off'])
    ap.add_argument('--per-file', type=int, default=5)
    ap.add_argument('--min-records', type=int, default=5)
    ap.add_argument('--prose-abort', type=float, default=5.0)
    #W70-BN (F8): the seat logs a previous sweep already PASSED, one per line.
    #The file is rewritten on every PASS, so each new seat log is gated once.
    ap.add_argument('--state')
    ap.add_argument('--selftest', action='store_true')
    a = ap.parse_args()
    if a.selftest:
        return selftest()
    if not (a.logdir and a.start and a.regime):
        print('FAIL regime-gate called without --logdir/--start/--regime')
        return 1
    checked = []
    if a.state and os.path.exists(a.state):
        try:
            checked = [l.strip() for l in open(a.state) if l.strip()]
        except OSError:
            checked = []
    recs, unstamped, _ = run_records(a.logdir, a.start, a.per_file)
    verdict, why, now = gate(recs, unstamped, a.regime, a.prose_abort, a.min_records, checked)
    if verdict == 'PASS' and a.state:
        try:
            with open(a.state, 'w') as fh:
                fh.write('\n'.join(sorted(set(checked) | set(now))) + '\n')
        except OSError:
            pass
    print('%s %s' % (verdict, why))
    return 1 if verdict == 'FAIL' else 0


if __name__ == '__main__':
    sys.exit(main())
