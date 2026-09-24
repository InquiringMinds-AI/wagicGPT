#!/usr/bin/env python3
"""baka-ab-compare.py [--paired] <accepted-dir> <candidate-dir> - the engine A/B gate verdict.
       baka-ab-compare.py --selftest
HARD FAIL: any game in the candidate with exit != 0 or no WAGIC_SELFPLAY_RESULT (hang/crash/timeout).
REPORT: per-deck wins with a two-proportion z vs the accepted run; |z| >= 2 is FLAGGED - name the
commits since the accepted binary that touch AIPlayerBaka/AllAbilities/TargetChooser/ActionLayer/
LegalActions/DecisionContract and either explain the shift as an intended heuristic-play change
(owner accepts) or treat it as a regression. Exit 1 on hard fail, 0 otherwise (flags are advisory).

--paired (bug-list #13, wave 82 lane EA): both runs were played by `baka-ab.sh --paired` and the
candidate replayed the accepted run's seeds (`--paired=<accepted-dir>`), so every (d0, d1, rep) is
the SAME game on both binaries. The verdict is then per GAME, not per deck rate: a game is
CONCORDANT when the same seed produced the same winner, DISCORDANT when it did not. Two binaries
that play identically are 0 discordant on every seed; a real play change shows up as named games
whose seed replays it (WAGIC_SELFPLAY_SEED=<seed> WAGIC_SELFPLAY_DECK0=<d0> WAGIC_SELFPLAY_DECK1=<d1>).
The per-deck z table is still printed - it is what an unpaired reader expects - but the paired
line is the gate's evidence. A seed mismatch on any joined game is an error: the runs are not a pair.
KNOWN RESIDUAL (measured 2026-09-18, one binary replaying its own seeds): ~1 game in 20 is NOT
trace-identical even on the same binary - the heuristic iterates pointer-keyed sets
(`std::set<MTGCardInstance*>` in AIPlayerBaka.cpp, address order), so a heap layout that differs
between two processes (thread timing; ASLR off does not remove it) can reorder which ability it
tries first, and the game forks from there (bimodal: 123v125 seed 64990388 gave turn 45 x6 and turn
38 x2 over 8 replays). So a DISCORDANT game is adjudicated by replaying its seed 3x on EACH binary:
a binary that is itself bimodal on that seed is the engine residual, not the change under test."""
import csv, re, sys, math, os, statistics as st, tempfile


def load(d):
    rows = list(csv.DictReader(open(f'{d}/results.tsv'), delimiter='\t'))
    out = []
    for r in rows:
        m = re.search(r'winner=(-?\d+) .*turn=(\d+)', r['result']) if r['result'] != 'NONE' else None
        out.append(dict(d0=r['d0'], d1=r['d1'], rep=r.get('rep', ''), exit=int(r['exit']),
                        win=int(m.group(1)) if m else None, turn=int(m.group(2)) if m else None,
                        seed=(r.get('seed') or '-').strip()))
    return out


def wins(rows):
    w = {}
    for r in rows:
        if r['win'] is None: continue
        for i, d in enumerate((r['d0'], r['d1'])):
            w.setdefault(d, [0, 0]); w[d][1] += 1
            if r['win'] == i: w[d][0] += 1
    return w


def paired_join(A, B):
    """[(a, b)] joined on (d0, d1, rep); raises ValueError on a seed mismatch or an
    unseeded game (the runs are not a pair)."""
    key = lambda r: (r['d0'], r['d1'], r['rep'])
    bi = {key(r): r for r in B}
    joined, bad = [], []
    for a in A:
        b = bi.get(key(a))
        if b is None:
            continue
        if a['seed'] in ('-', '') or b['seed'] in ('-', '') or a['seed'] != b['seed']:
            bad.append((key(a), a['seed'], b['seed']))
        joined.append((a, b))
    if bad:
        raise ValueError("seed mismatch on %d joined game(s) - not a paired run (first: %s accepted seed %s"
                         " vs candidate seed %s)" % (len(bad), " ".join(bad[0][0]), bad[0][1], bad[0][2]))
    return joined


def paired_report(joined):
    """(lines, discordant) for a joined list; games the candidate did not finish are
    listed, never counted as concordant."""
    same = diff = unfinished = sameturn = 0
    names, perdeck = [], {}
    for a, b in joined:
        if b['win'] is None or a['win'] is None:
            unfinished += 1
            continue
        if a['win'] == b['win']:
            same += 1
            if a['turn'] == b['turn']:
                sameturn += 1
        else:
            diff += 1
            names.append("  %sv%s rep %s seed %s: accepted winner=%d turn=%d -> candidate winner=%d turn=%d"
                         % (a['d0'], a['d1'], a['rep'], a['seed'], a['win'], a['turn'], b['win'], b['turn']))
            for i, d in enumerate((a['d0'], a['d1'])):
                perdeck.setdefault(d, [0, 0])
                if b['win'] == i and a['win'] != i: perdeck[d][0] += 1   # gained
                if a['win'] == i and b['win'] != i: perdeck[d][1] += 1   # lost
    lines = ["PAIRED: %d game(s) joined on (d0, d1, rep) with equal seeds; %d concordant (%d also on the same"
             " turn), %d DISCORDANT, %d unfinished" % (len(joined), same, sameturn, diff, unfinished)]
    if diff:
        lines.append("discordant games (same seed, different winner) - each seed replays its game:")
        lines += names
        lines.append("per-deck wins gained/lost on discordant games: "
                     + " ".join("deck%s +%d/-%d" % (d, g, l) for d, (g, l) in sorted(perdeck.items())))
    return lines, diff


def compare(accepted, candidate, paired):
    A, B = load(accepted), load(candidate)
    bad = [r for r in B if r['exit'] != 0 or r['win'] is None]
    turnsA = [r['turn'] for r in A if r['turn']]; turnsB = [r['turn'] for r in B if r['turn']]
    print(f"accepted: {len(A)} games, {sum(1 for r in A if r['exit']!=0 or r['win'] is None)} unfinished, turns med {st.median(turnsA) if turnsA else '-'} max {max(turnsA) if turnsA else '-'}")
    print(f"candidate: {len(B)} games, {len(bad)} unfinished, turns med {st.median(turnsB) if turnsB else '-'} max {max(turnsB) if turnsB else '-'}")
    wa, wb = wins(A), wins(B); flags = 0
    print("deck  acc  cand   z")
    for d in sorted(wa):
        if d not in wb: continue
        a, na = wa[d]; b, nb = wb[d]; p = (a + b) / (na + nb); se = math.sqrt(p * (1 - p) * (1 / na + 1 / nb)) or 1
        z = (b / nb - a / na) / se; flag = ' FLAG' if abs(z) >= 2 else ''
        if flag: flags += 1
        print(f"{d:>4} {a:>4}/{na:<4} {b:>4}/{nb:<4} {z:+.2f}{flag}")
    discordant = 0
    if paired:
        try:
            joined = paired_join(A, B)
        except ValueError as e:
            print("PAIRED ERROR: %s" % e); return 2
        lines, discordant = paired_report(joined)
        print("\n".join(lines))
    if bad:
        ex = {}
        for r in bad: ex[r['exit']] = ex.get(r['exit'], 0) + 1
        print(f"HARD FAIL: {len(bad)} candidate game(s) did not finish: exits {ex} - see the stderr files"); return 1
    if paired:
        print("PASS (hard); %d paired game(s) changed outcome - replay each seed 3x on BOTH binaries (a binary"
              " bimodal on its own seed is the engine's address-order residual, see --help), then explain the"
              " rest as an intended play change or treat as a regression"
              % discordant if discordant else "PASS; every paired game reached the same outcome on both binaries")
    else:
        print(f"PASS (hard); {flags} deck(s) flagged for explanation" if flags else "PASS")
    return 0


def _write(d, rows):
    os.makedirs(d, exist_ok=True)
    with open(os.path.join(d, 'results.tsv'), 'w') as f:
        f.write("d0\td1\trep\texit\tsecs\tresult\tseed\n")
        for r in rows:
            f.write("\t".join(str(x) for x in r) + "\n")


def selftest():
    ok = True
    def chk(c, what):
        nonlocal ok
        if not c:
            ok = False; print("SELFTEST FAIL: " + what)
    res = lambda w, t: "WAGIC_SELFPLAY_RESULT winner=%d life0=1 life1=1 turn=%d" % (w, t)
    tmp = tempfile.mkdtemp(prefix="baka-ab-selftest-")
    acc = [("152", "123", 1, 0, 9, res(0, 12), 11), ("152", "123", 2, 0, 9, res(1, 20), 22),
           ("125", "152", 1, 0, 9, res(1, 15), 33), ("125", "152", 2, 0, 9, res(0, 9), 44)]
    # candidate: same seeds; game 2 flips its winner, game 3 same winner on a later turn
    cand = [("152", "123", 1, 0, 9, res(0, 12), 11), ("152", "123", 2, 0, 9, res(0, 18), 22),
            ("125", "152", 1, 0, 9, res(1, 16), 33), ("125", "152", 2, 0, 9, res(0, 9), 44)]
    _write(tmp + "/a", acc); _write(tmp + "/b", cand)
    j = paired_join(load(tmp + "/a"), load(tmp + "/b"))
    lines, d = paired_report(j)
    chk(len(j) == 4 and d == 1, "4 joined, 1 discordant (got %d joined, %d discordant)" % (len(j), d))
    chk(lines[0].startswith("PAIRED: 4 game(s)") and "3 concordant (2 also on the same turn), 1 DISCORDANT" in lines[0],
        "paired summary line %r" % lines[0])
    chk(any("152v123 rep 2 seed 22: accepted winner=1 turn=20 -> candidate winner=0 turn=18" in l for l in lines),
        "the discordant game is named with its seed")
    chk(any("deck152 +1/-0" in l and "deck123 +0/-1" in l for l in lines), "per-deck gained/lost on the flip")
    # identical runs: 0 discordant, and the verdict says so
    _write(tmp + "/c", acc)
    lines2, d2 = paired_report(paired_join(load(tmp + "/a"), load(tmp + "/c")))
    chk(d2 == 0 and "4 concordant (4 also on the same turn), 0 DISCORDANT" in lines2[0], "identical runs are 0 discordant")
    # a seed mismatch is an error, not a comparison
    _write(tmp + "/d", [("152", "123", 1, 0, 9, res(0, 12), 99)] + acc[1:])
    try:
        paired_join(load(tmp + "/a"), load(tmp + "/d")); chk(False, "seed mismatch must raise")
    except ValueError as e:
        chk("seed mismatch on 1 joined game" in str(e), "seed mismatch message %r" % str(e))
    # an unseeded (clock) run cannot be paired
    _write(tmp + "/e", [(a, b, r, x, s, q, "-") for a, b, r, x, s, q, _ in acc])
    try:
        paired_join(load(tmp + "/a"), load(tmp + "/e")); chk(False, "unseeded run must raise")
    except ValueError:
        pass
    # an unfinished candidate game is listed as unfinished, never concordant
    _write(tmp + "/f", [("152", "123", 1, 124, 300, "NONE", 11)] + acc[1:])
    lines3, d3 = paired_report(paired_join(load(tmp + "/a"), load(tmp + "/f")))
    chk("3 concordant" in lines3[0] and "1 unfinished" in lines3[0] and d3 == 0, "unfinished is not concordant %r" % lines3[0])
    # the legacy 6-column results.tsv still loads for the unpaired path
    os.makedirs(tmp + "/g", exist_ok=True)
    with open(tmp + "/g/results.tsv", "w") as f:
        f.write("d0\td1\trep\texit\tsecs\tresult\n152\t123\t1\t0\t9\t%s\n" % res(0, 12))
    chk(load(tmp + "/g")[0]['seed'] == '-', "legacy rows read as unseeded")
    print("baka-ab-compare selftest: %s" % ("OK" if ok else "FAILED"))
    return 0 if ok else 1


if __name__ == "__main__":
    args = sys.argv[1:]
    if args == ["--selftest"]:
        sys.exit(selftest())
    paired = False
    if args and args[0] == "--paired":
        paired = True; args = args[1:]
    if len(args) != 2:
        print(__doc__); sys.exit(2)
    sys.exit(compare(args[0], args[1], paired))
