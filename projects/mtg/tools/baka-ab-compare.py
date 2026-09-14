#!/usr/bin/env python3
"""baka-ab-compare.py <accepted-dir> <candidate-dir> - the engine A/B gate verdict.
HARD FAIL: any game in the candidate with exit != 0 or no WAGIC_SELFPLAY_RESULT (hang/crash/timeout).
REPORT: per-deck wins with a two-proportion z vs the accepted run; |z| >= 2 is FLAGGED - name the
commits since the accepted binary that touch AIPlayerBaka/AllAbilities/TargetChooser/ActionLayer/
LegalActions/DecisionContract and either explain the shift as an intended heuristic-play change
(owner accepts) or treat it as a regression. Exit 1 on hard fail, 0 otherwise (flags are advisory)."""
import csv, re, sys, math, statistics as st
def load(d):
    rows = list(csv.DictReader(open(f'{d}/results.tsv'), delimiter='\t'))
    out = []
    for r in rows:
        m = re.search(r'winner=(-?\d+) .*turn=(\d+)', r['result']) if r['result'] != 'NONE' else None
        out.append(dict(d0=r['d0'], d1=r['d1'], exit=int(r['exit']), win=int(m.group(1)) if m else None,
                        turn=int(m.group(2)) if m else None))
    return out
def wins(rows):
    w = {}
    for r in rows:
        if r['win'] is None: continue
        for i, d in enumerate((r['d0'], r['d1'])):
            w.setdefault(d, [0, 0]); w[d][1] += 1
            if r['win'] == i: w[d][0] += 1
    return w
if len(sys.argv) != 3: print(__doc__); sys.exit(2)
A, B = load(sys.argv[1]), load(sys.argv[2])
bad = [r for r in B if r['exit'] != 0 or r['win'] is None]
turnsA = [r['turn'] for r in A if r['turn']]; turnsB = [r['turn'] for r in B if r['turn']]
print(f"accepted: {len(A)} games, {sum(1 for r in A if r['exit']!=0 or r['win'] is None)} unfinished, turns med {st.median(turnsA)} max {max(turnsA)}")
print(f"candidate: {len(B)} games, {len(bad)} unfinished, turns med {st.median(turnsB) if turnsB else '-'} max {max(turnsB) if turnsB else '-'}")
wa, wb = wins(A), wins(B); flags = 0
print("deck  acc  cand   z")
for d in sorted(wa):
    if d not in wb: continue
    a, na = wa[d]; b, nb = wb[d]; p = (a + b) / (na + nb); se = math.sqrt(p * (1 - p) * (1 / na + 1 / nb)) or 1
    z = (b / nb - a / na) / se; flag = ' FLAG' if abs(z) >= 2 else ''
    if flag: flags += 1
    print(f"{d:>4} {a:>4}/{na:<4} {b:>4}/{nb:<4} {z:+.2f}{flag}")
if bad:
    ex = {}
    for r in bad: ex[r['exit']] = ex.get(r['exit'], 0) + 1
    print(f"HARD FAIL: {len(bad)} candidate game(s) did not finish: exits {ex} - see the stderr files"); sys.exit(1)
print(f"PASS (hard); {flags} deck(s) flagged for explanation" if flags else "PASS")
