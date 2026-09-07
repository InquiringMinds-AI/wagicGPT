#!/usr/bin/env python3
"""#W70-BN (F6-F9): the four Astra findings against a regime-gate.py, either edition.

Run it against the FIXED gate (default) or against the base one to see the
verdicts the review reproduced:

    python3 lane-BN-regime-gate-red.py                 # tools/regime-gate.py
    python3 lane-BN-regime-gate-red.py /path/to/base/regime-gate.py

Base verdicts (reproduced 2026-09-06, merge 71b1e22be):
  F6 a successful forced-close recovery ................ FAIL (must PASS)
  F7 engine-answered records only ...................... FAIL "rebuild" (must WAIT)
  F8 five records from ONE seat, another seat silent ... PASS (must WAIT)
  F9 thinking off + reasoning_hidden / reasoning_tokens  PASS (must FAIL)
"""
import importlib.util, json, os, sys, tempfile

path = sys.argv[1] if len(sys.argv) > 1 else \
    os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../tools/regime-gate.py')
spec = importlib.util.spec_from_file_location('rg', path)
m = importlib.util.module_from_spec(spec)
spec.loader.exec_module(m)


def verdict(*a, **kw):
    r = m.gate(*a, **kw)
    return r[0], r[1]


def rec(f='f.jsonl', **kw):
    r = {'kind': 'ask', 'reasoning_chars': 0, 'thinking': 'on', 'prompt': 'p',
         'reply': 'PLAN: a\nCHOICE: 2 (Cast Bear)'}
    r.update(kw)
    return (f, r)


fails = []


def want(label, got, expect):
    ok = got == expect
    print('  %s %-72s got %s, want %s' % ('ok  ' if ok else 'FAIL', label, got, expect))
    if not ok:
        fails.append(label)


ok4 = [rec(reasoning_chars=900, max_tokens_reasoning=6000) for _ in range(4)]
want('F6 forced-close recovery WITH native reasoning',
     verdict(ok4 + [rec(reasoning_chars=900, max_tokens_reasoning=0,
                        reasoning_forced_close=True)], 0, 'on', 5.0, 5)[0], 'PASS')
want('F6 the stamp is not a blanket excuse (no reasoning)',
     verdict(ok4 + [rec(reasoning_chars=0, max_tokens_reasoning=0,
                        reasoning_forced_close=True)], 0, 'on', 5.0, 5)[0], 'FAIL')

with tempfile.TemporaryDirectory() as td:
    p = os.path.join(td, '2000000000-ai_baka_deck44-0xa-vs-ai_baka_deck9.jsonl')
    with open(p, 'w') as fh:
        for i in range(6):
            fh.write(json.dumps({'kind': 'ask', 'seq': i, 'prompt': '', 'reply': '',
                                 'choice': -1, 'fallback': 'engine_answered'}) + '\n')
    recs, unst, _ = m.run_records(td, 2000000000, 5)
    want('F7 engine-answered records only', verdict(recs, unst, 'on', 5.0, 5)[0], 'WAIT')

want('F8 five records from ONE seat while another seat is silent',
     verdict([rec('a.jsonl', reasoning_chars=900) for _ in range(5)]
             + [rec('b.jsonl', reasoning_chars=900) for _ in range(2)], 0, 'on', 5.0, 5)[0],
     'WAIT')
want('F9 thinking off + a WITHHELD trace',
     verdict([rec(thinking='off') for _ in range(4)]
             + [rec(thinking='off', reasoning_hidden=True)], 0, 'off', 5.0, 5)[0], 'FAIL')
want('F9 thinking off + BILLED reasoning tokens',
     verdict([rec(thinking='off') for _ in range(4)]
             + [rec(thinking='off', reasoning_tokens=1200)], 0, 'off', 5.0, 5)[0], 'FAIL')

print('\n%d of 6 checks failed' % len(fails))
sys.exit(1 if fails else 0)
