#!/usr/bin/env python3
"""#W71-BQ (L3): rowSaysNoOp on the wave-70 corpus's real cast rows.

The base predicate (77d82bbe6, AIPlayerGPT.cpp `rowSaysNoOp`) is:

    lowercase the WHOLE row; true if it contains "does nothing" or
    "does not apply"; else the computed-magnitude grammar.

Two phrase cues, two emitter families. "does not apply" has exactly one emitter - `morbidMagnitudeClause`, INSIDE the
parenthetical qualifier of a real magnitude: "-1/-1 (no creature has died this
turn, so Morbid does NOT apply)". So every Tragic Slip row cast outside Morbid
matched, including rows whose own `{kills: ...}` clause named the bodies the
cast removes. This script replays both predicates over every cast row the
corpus rendered and reports each verdict change.

Usage: lane-BQ-noop-row-red.py [corpus-dir]
"""
import glob, json, os, re, sys

CORPUS = sys.argv[1] if len(sys.argv) > 1 else \
    os.path.expanduser('~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849')


def right_now_clause(row):
    s = row.find('{right now:')
    if s < 0:
        return ''
    depth = 0
    for i in range(s, len(row)):
        if row[i] == '{':
            depth += 1
        elif row[i] == '}':
            depth -= 1
            if depth == 0:
                return row[s:i + 1]
    return ''


def computed_zero(row):
    """The number grammar (#W58-A D3), close enough for this census: the core
    before ' - ', every magnitude in it zero, at least one magnitude present."""
    clause = right_now_clause(row).lower()
    if not clause:
        return False
    colon = clause.find(':')
    if colon < 0:
        return False
    end = clause.find(' - ', colon + 1)
    body = clause[colon + 1:end if end > 0 else len(clause)]
    nums = re.findall(r'[-+]?\d+', body)
    return bool(nums) and all(int(n) == 0 for n in nums)


def base_no_op(row):
    low = row.lower()
    return ('does nothing' in low) or ('does not apply' in low) or computed_zero(row)


def scope_is_conditional(low, hit):
    start = 0
    for i in range(hit, 0, -1):
        if low[i - 1] in ';{}':
            start = i
            break
    seg = low[start:hit]
    return bool(re.search(r'(^|[ (\-])(if|unless|would) ', seg))


def phrase_is_verdict(low, phrase):
    depth, quoted, i = 0, False, 0
    while i < len(low):
        c = low[i]
        if c == '"':
            quoted = not quoted
        elif quoted:
            pass
        elif c == '(':
            depth += 1
        elif c == ')':
            depth = max(0, depth - 1)
        elif depth == 0 and low.startswith(phrase, i) and not scope_is_conditional(low, i):
            return True
        i += 1
    return False


def names_a_live_kill(row):
    for m in re.finditer(r'\{(?:kills|removes)(?: whichever you target)?: ', row):
        close = row.find('}', m.end())
        if close < 0:
            continue
        if row[m.end():close].strip():
            return True
    return False


def fixed_no_op(row):
    if names_a_live_kill(row):
        return False
    low = row.lower()
    if phrase_is_verdict(low, 'does nothing') or phrase_is_verdict(low, 'does not apply'):
        return True
    return computed_zero(row)


rows, changed = 0, []
for path in sorted(glob.glob(os.path.join(CORPUS, '*.jsonl'))):
    for line in open(path, encoding='utf-8', errors='replace'):
        try:
            rec = json.loads(line)
        except ValueError:
            continue
        for row in (rec.get('options_text') or []):
            if not isinstance(row, str) or not row.lstrip().lower().startswith('cast '):
                continue
            rows += 1
            b, f = base_no_op(row), fixed_no_op(row)
            if b != f:
                changed.append((os.path.basename(path), rec.get('seq'), b, f, row))

print('cast rows scanned: %d' % rows)
print('verdict changes  : %d  (all base=TRUE -> fixed=FALSE: %s)'
      % (len(changed), all(b and not f for _, _, b, f, _ in changed)))
seen = set()
for name, seq, b, f, row in changed:
    key = row[:120]
    if key in seen:
        continue
    seen.add(key)
    print('\n  %s seq %s : base no-op=%s -> fixed no-op=%s\n    %s' % (name, seq, b, f, row[:300]))
