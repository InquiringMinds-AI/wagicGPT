#!/usr/bin/env python3
"""#W70-BN (F11): the step-boundary grammar, measured against real PLAN lines.

Astra review finding 11: every punctuation boundary followed by whitespace was a
step boundary, so "cast Thalia, Guardian of Thraben; attack" became three steps
and the carried remainder named a card that does not exist. This replicates the
shipped (base) grammar and the fixed one, in Python, line for line, and counts -
over the PLAN lines of the wave-69 corpus, against the engine's own card names -
how many boundaries each grammar puts INSIDE a card name.

Usage: lane-BN-plan-split-red.py [corpus-dir] [primitives-file]
"""
import glob, json, os, re, sys

CORPUS = sys.argv[1] if len(sys.argv) > 1 else \
    os.path.expanduser('~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120')
PRIM = sys.argv[2] if len(sys.argv) > 2 else \
    os.path.join(os.path.dirname(__file__), '../../bin/Res/sets/primitives/mtg.txt')


def base_step_ends(plan):
    """The shipped grammar (GptPlanCaveat.h before this lane)."""
    ends = []
    for i, c in enumerate(plan):
        if c not in ';,.!?\n':
            continue
        if c != '\n' and i + 1 < len(plan) and not plan[i + 1].isspace():
            continue
        st = next((k for k, ch in enumerate(plan) if ch not in ' \t\r\n;,.!?'), None)
        if st is None or i < st:
            continue
        ends.append(i + 1)
    return ends


def then_at(plan, i):
    if not plan[i:i + 4].lower() == 'then':
        return False
    if i + 4 < len(plan) and (plan[i + 4].isalnum() or plan[i + 4] == '_'):
        return False
    return i > 0 and plan[i - 1].isspace()


def fixed_step_ends(plan):
    """The grammar this lane ships."""
    ends = []
    st = next((k for k, ch in enumerate(plan) if ch not in ' \t\r\n;,.!?'), None)
    for i, c in enumerate(plan):
        end = None
        if c == '\n':
            end = i + 1
        elif c == ';' and (i + 1 >= len(plan) or plan[i + 1].isspace()):
            end = i + 1
        elif c in '.!?' and i + 2 < len(plan) and plan[i + 1].isspace() and plan[i + 2].isupper():
            end = i + 1
        elif then_at(plan, i):
            back = i
            while back > 0 and plan[back - 1].isspace():
                back -= 1
            end = back
        if end is None or st is None or end <= st:
            continue
        if ends and ends[-1] >= end:
            continue
        ends.append(end)
    return ends


def load_names():
    names = set()
    try:
        for line in open(PRIM, encoding='latin-1'):
            if line.startswith('name='):
                n = line[5:].strip()
                if ',' in n and len(n) > 6:
                    names.add(n)
    except OSError:
        pass
    return names


def plan_lines():
    out = []
    for f in glob.glob(os.path.join(CORPUS, '*.jsonl')):
        for line in open(f, errors='replace'):
            try:
                r = json.loads(line)
            except Exception:
                continue
            for ln in (r.get('reply') or '').split('\n'):
                s = ln.strip()
                if s.upper().startswith('PLAN:'):
                    out.append(s[5:].strip())
                    break
    return out


def spans(plan, names):
    """[start, end) of every card name occurrence in the line."""
    out = []
    for n in names:
        at = 0
        while True:
            at = plan.find(n, at)
            if at < 0:
                break
            out.append((at, at + len(n)))
            at += 1
    return out


def main():
    names = load_names()
    lines = plan_lines()
    print('PLAN lines: %d   comma-bearing card names known: %d' % (len(lines), len(names)))
    for label, fn in (('base ', base_step_ends), ('fixed', fixed_step_ends)):
        cuts = inside = affected = 0
        steps = 0
        for l in lines:
            sp = spans(l, names)
            ends = fn(l)
            steps += max(1, len(ends))
            hit = False
            for e in ends:
                cuts += 1
                if any(a < e < b for a, b in sp):
                    inside += 1
                    hit = True
            if hit:
                affected += 1
        print('%s: %6d boundaries, %5.2f steps/line, %4d inside a card name, '
              '%3d lines mis-split (%.1f%%)'
              % (label, cuts, steps / max(1, len(lines)), inside, affected,
                 100.0 * affected / max(1, len(lines))))
    return 0


if __name__ == '__main__':
    sys.exit(main())
