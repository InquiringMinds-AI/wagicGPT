#!/usr/bin/env python3
#W70-BK (C4): RED-ON-BASE evidence for the max_tokens arithmetic.
#
# The shipped (master b4b2f426c / 52ab71610) arithmetic and the lane's
# gptResolveMaxTokens, replicated line for line from buildRequestBody, run over
# the exact assertions the new PARSETEST block makes. The base arithmetic fails
# 6 of 10 - and every one of the six is a completion carrying REASONING that a
# cap bounded (invariant 000(d)). The four thinking-OFF cases are unchanged, so
# the product regime's caps are byte-identical to what shipped.
SEAM = {'ask': 768, 'priority': 768, 'attackers': 768, 'blockers': 896,
        'discard': 512, 'reveal': 512, 'bottom': 512}
RESERVE, DEF, LOCK = 400, 4096, 512


def seam(s, c):
    if not s:
        return c
    t = SEAM.get(s)
    if t is None:
        return c
    return c if (c > 0 and c < t) else t


def base(thinking, forceClose, budget, ceiling, s, disabled, lockedRetry):
    m = DEF
    if thinking and budget > 0:
        m = budget + RESERVE
    if ceiling > 0:
        m = ceiling                       # audit B7.6: REPLACES the sum
    if not (thinking and budget > 0) and not disabled:
        m = seam(s, m)                    # audit B7.7: the whole completion
    if lockedRetry:
        m = RESERVE if forceClose else 512   # audit B7.10
    return m


def lane(thinking, forceClose, budget, ceiling, s, disabled, lockedRetry):
    raw = ceiling if ceiling > 0 else DEF
    a = raw if disabled else seam(s, raw)
    if lockedRetry:
        a = RESERVE if forceClose else LOCK
    r = 0
    if thinking and not forceClose:
        r = budget if budget > 0 else (raw - a if raw > a else 0)
    return a + r


CASES = [
    ("thinking on, budget 6000, ask seam",     (True, False, 6000, -1, 'ask', False, False), 6768),
    ("thinking on, budget 6000, blockers",     (True, False, 6000, -1, 'blockers', False, False), 6896),
    ("thinking on, budget 0 (unbounded), ask", (True, False, 0, -1, 'ask', False, False), 4096),
    ("thinking on, ceiling 900, budget 6000",  (True, False, 6000, 900, 'ask', False, False), 6768),
    ("thinking on, ceiling 300, budget 6000",  (True, False, 6000, 300, 'ask', False, False), 6300),
    ("thinking on, answer-locked retry",       (True, False, 6000, -1, 'ask', False, True), 6512),
    ("thinking off, ask seam",                 (False, False, 0, -1, 'ask', False, False), 768),
    ("thinking off, unnamed seam",             (False, False, 0, -1, 'mulligan', False, False), 4096),
    ("thinking off, SEAMTOKENS=0",             (False, False, 0, -1, 'ask', True, False), 4096),
    ("forced close (phase 2)",                 (True, True, 6000, -1, 'ask', False, True), 400),
]

if __name__ == '__main__':
    bad = 0
    print("%-38s %8s %8s %8s" % ("case", "base", "lane", "expect"))
    for name, args, exp in CASES:
        o, n = base(*args), lane(*args)
        assert n == exp, (name, n, exp)
        bad += (o != exp)
        print("%-38s %8d %8d %8d%s" % (name, o, n, exp, '   <== RED on base' if o != exp else ''))
    print("\nbase fails %d of %d; lane passes %d of %d."
          % (bad, len(CASES), len(CASES), len(CASES)))
