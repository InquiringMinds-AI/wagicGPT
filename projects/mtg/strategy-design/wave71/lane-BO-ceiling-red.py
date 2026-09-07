#!/usr/bin/env python3
"""#W71-BO RED-on-base witness for wave-70 R3/LOW-3: the per-seam ANSWER ceilings
the base binary (77d82bbe6) ships are ~2.6x-3.3x the size the corpus they produced
says they need.  Measured on
  ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849
by re-parsing every reply into its PLAN line + its action line - which is 94.6% of
replies in full - and taking p99.9 per seam.

The fit rule: ceiling = ceil(p99.9 x 2 / 3.15 B-per-token) rounded up to 32,
floored at 256 tokens.  3.15 B/token is #W69-BF K2's own measured worst case; the
floor is justified by the corpus's largest single reply at ANY seam (457 B), which
256 tokens covers 1.76x over.
Exit 1 when a shipped cap is more than 1.5x its fitted value (RED), 0 otherwise.
"""
import json, glob, math, os, re, sys, collections

RUN = os.path.expanduser("~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849")
BASE = {"ask": 768, "priority": 768, "attackers": 768, "blockers": 896,
        "discard": 512, "reveal": 512, "bottom": 512}
FIT = {"ask": 288, "priority": 288, "attackers": 256, "blockers": 256,
       "discard": 256, "reveal": 256}          # bottom: 0 windows -> UNTESTED
BPT = 3.15
LAB = re.compile(r'^\s*(CHOICE|ATTACK|ATTACKERS|BLOCKS|PUT|BOTTOM|DISCARD|HOLD PRIORITY)\s*:', re.I)
PLAN = re.compile(r'^\s*(YOUR\s+)?PLAN\s*:', re.I)

per = collections.defaultdict(list)
for f in glob.glob(os.path.join(RUN, "*.jsonl")):
    for line in open(f, errors="replace"):
        try:
            r = json.loads(line)
        except Exception:
            continue
        k, rep = r.get("kind"), r.get("reply")
        if not rep or k not in BASE:
            continue
        t = rep.split("</think>")[-1]
        plan = act = None
        for ln in t.split("\n"):
            if plan is None and PLAN.match(ln):
                plan = ln.strip()
            elif act is None and LAB.match(ln):
                act = ln.strip()
        two = (len(plan.encode()) if plan else 0) + (len(act.encode()) if act else 0) + 1
        per[k].append(max(two, len(t.strip().encode())))   # the whole reply, never less

def q(v, p):
    v = sorted(v)
    return v[max(0, min(len(v) - 1, math.ceil(p * len(v)) - 1))] if v else 0

def fit(b):
    return max(256, ((math.ceil(b * 2 / BPT) + 31) // 32) * 32)

print("%-10s %5s %6s %6s %8s %8s %6s %6s" % ("seam", "n", "p99", "p99.9", "max", "fitted", "base", "ratio"))
red = []
for s in sorted(BASE):
    v = per.get(s, [])
    if not v:
        print("%-10s %5d %6s %6s %8s %8s %6d   UNTESTED" % (s, 0, "-", "-", "-", "-", BASE[s]))
        continue
    p999 = q(v, 0.999)
    f = fit(p999)
    print("%-10s %5d %6d %6d %8d %8d %6d  %5.2fx"
          % (s, len(v), q(v, 0.99), p999, max(v), f, BASE[s], BASE[s] / float(f)))
    if BASE[s] > 1.5 * f:
        red.append("%s: shipped %d tokens (%d B at %.2f B/tok) against a p99.9 need of %d B - %.1fx"
                   % (s, BASE[s], int(BASE[s] * BPT), BPT, p999, BASE[s] / float(f)))
    if s in FIT and FIT[s] != f:
        red.append("%s: the lane's table says %d, the arithmetic says %d" % (s, FIT[s], f))

if red:
    print("\nRED on base:")
    for r in red:
        print("  - " + r)
    sys.exit(1)
print("\nno base defect detected")
