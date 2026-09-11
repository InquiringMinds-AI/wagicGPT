#!/usr/bin/env python3
"""#W78-CX (wave-78 lane CX): the self-play corpus census, as a TOOL.

Every wave until now re-derived this in a throwaway `~/.gatelogs/wNN-stats.py`,
and the wave-74 edition ignored its own argument and reported the WRONG corpus
with full confidence (wave-75 lane brief, LESSON 2). So: the run directory is a
required ARGUMENT and it is echoed on the first output line, and the kind census
sums EVERY record kind - including the ones later waves added (`recovery`,
`defer`, `wall_miss`) which the gameend counter `protocol_replies` does not see
(wave-77 engine-seat L4: 2,740 vs the true 2,746).

  usage:  python3 tools/corpus-stats.py <run-dir> [<run-dir> ...]
          python3 tools/corpus-stats.py --selftest

A seat log is EXCLUDED when it has no `gameend` record (the game did not finish)
or when any record carries `fallback: timeout` (a timeout-tainted seat); both
exclusions are named on stderr. Logs are de-duplicated by BASENAME, so passing a
run dir and its consolidated `-final` twin cannot double-count a seat.
"""
import collections
import glob
import json
import os
import statistics
import sys

NON_DECISION_KINDS = ("gamestart", "system", "gameend")
#W78-CY (F12): the kinds the gameend counter `protocol_replies` DOES see - the
#records that cost a round trip. Everything else that is a decision record is, by
#construction, a kind that carries no round trip, so the CENSUS CHECK's explanatory
#list is computed from the census itself and cannot go stale when a wave adds a kind
#(wave 78 added `forced_close`, and the hard-coded list printed a difference of one
#with an empty explanation).
#The decision seams that ASK the model: every one of these increments the seat's
#`mProtocolReplies` when its record is written. A kind that is NOT here carries no
#round trip, whether this tool has heard of it or not - which is the point: the
#wave-78 `forced_close` needed no edit to be explained, and neither will the next one.
ROUNDTRIP_KINDS = ("ask", "priority", "attackers", "blockers", "discard",
                   "reveal", "bottom")


def no_roundtrip_kinds(kinds):
    """Every kind in the census that protocol_replies does not count."""
    return sorted(k for k in kinds
                  if k and k not in NON_DECISION_KINDS and k not in ROUNDTRIP_KINDS)


def census_kind_sum(kinds):
    """...and its total, so a mis-classified kind is LOUD, not silent."""
    return sum(kinds[k] for k in no_roundtrip_kinds(kinds))


def census_kind_list(kinds):
    """The CENSUS CHECK's explanatory list, as printed."""
    return ", ".join("%s %d" % (k, kinds[k]) for k in no_roundtrip_kinds(kinds)
                     if kinds.get(k))


def selftest():
    """#W78-CY (F12): the reconciliation, on mocked input. One ask, one
    forced_close, gameend protocol_replies 1 -> difference 1, explained."""
    import collections as _c
    ok = True
    kinds = _c.Counter({"gamestart": 1, "ask": 1, "forced_close": 1, "gameend": 1})
    decision_records = sum(v for k, v in kinds.items() if k not in NON_DECISION_KINDS)
    listed = census_kind_list(kinds)
    if decision_records - 1 != 1 or listed != "forced_close 1":
        print("SELFTEST FAIL: difference %d list %r" % (decision_records - 1, listed))
        ok = False
    # every future kind is explained the same way, with no edit to this tool
    k2 = _c.Counter({"ask": 2, "recovery": 1, "defer": 1, "wall_miss": 1,
                     "forced_close": 1, "some_future_kind": 3, "gameend": 2})
    if census_kind_list(k2) != ("defer 1, forced_close 1, recovery 1, "
                                "some_future_kind 3, wall_miss 1"):
        print("SELFTEST FAIL: generic list %r" % census_kind_list(k2))
        ok = False
    # ...and a census with nothing but round-trip kinds explains nothing, because
    # there is nothing to explain.
    if census_kind_list(_c.Counter({"ask": 5, "priority": 2, "gameend": 1})) != "":
        print("SELFTEST FAIL: round-trip-only census listed something")
        ok = False
    # the wave-78 corpus's own reconciliation: 2,746 - 2,740 = recovery 5 + defer 1
    k3 = _c.Counter({"gamestart": 42, "system": 42, "ask": 1999, "reveal": 21,
                     "discard": 51, "gameend": 42, "attackers": 82, "priority": 567,
                     "recovery": 5, "blockers": 18, "bottom": 2, "defer": 1})
    d3 = sum(v for k, v in k3.items() if k not in NON_DECISION_KINDS)
    if d3 != 2746 or census_kind_list(k3) != "defer 1, recovery 5" \
            or census_kind_sum(k3) != d3 - 2740:
        print("SELFTEST FAIL: wave-78 corpus %d %r" % (d3, census_kind_list(k3)))
        ok = False
    print("corpus-stats selftest: %s" % ("OK" if ok else "FAILED"))
    return 0 if ok else 1


def load(dirs):
    """basename -> records, with the two exclusions applied."""
    seen = {}
    for d in dirs:
        for f in sorted(glob.glob(os.path.join(d, "*.jsonl"))):
            b = os.path.basename(f)
            if b in seen:
                continue
            recs = []
            for line in open(f):
                line = line.strip()
                if not line:
                    continue
                try:
                    recs.append(json.loads(line))
                except ValueError:
                    pass
            gameend = [r for r in recs if r.get("kind") == "gameend"]
            timeouts = sum(1 for r in recs if r.get("fallback") == "timeout")
            if not gameend or timeouts:
                print("EXCLUDED %s (%s, timeouts %d)"
                      % (b[:60], "gameend" if gameend else "no-gameend", timeouts),
                      file=sys.stderr)
                continue
            seen[b] = recs
    return seen


def pct(values, p):
    if not values:
        return 0.0
    s = sorted(values)
    i = min(len(s) - 1, int(len(s) * p))
    return s[i] / 1000.0


def main(argv):
    if len(argv) == 2 and argv[1] == "--selftest":
        return selftest()  #W78-CY (F12)
    if len(argv) < 2:
        print(__doc__)
        return 2
    dirs = [os.path.abspath(os.path.expanduser(a)) for a in argv[1:]]
    # LESSON of wave 75: the tool says WHICH corpus it read, first, always.
    print("CORPUS: " + " ".join(dirs))
    seen = load(dirs)
    print("dirs %d clean seat logs %d = games %d"
          % (len(dirs), len(seen), len(seen) // 2))
    allrecs = [r for recs in seen.values() for r in recs]

    # ---- the kind census: EVERY kind, and the sum that includes the new ones.
    kinds = collections.Counter(r.get("kind") for r in allrecs)
    decision_records = sum(v for k, v in kinds.items() if k not in NON_DECISION_KINDS)
    print("ALL KINDS %s" % dict(kinds))
    print("RECORD KINDS SUM (all kinds except %s) %d"
          % ("/".join(NON_DECISION_KINDS), decision_records))
    for k in no_roundtrip_kinds(kinds):  #W78-CY (F12): generic, never a fixed list
        print("  ...of which %-13s %d" % (k, kinds.get(k, 0)))

    # ---- fallbacks, by kind and flat.
    fbk = collections.Counter((r.get("kind"), r.get("fallback"))
                              for r in allrecs if r.get("fallback"))
    print("fallbacks ALL kinds %s" % dict(fbk))
    asks = [r for r in allrecs if r.get("kind") == "ask"]
    askfb = collections.Counter(r["fallback"] for r in asks if r.get("fallback"))
    print("fallbacks (ask seam) %s total %d" % (dict(askfb), sum(askfb.values())))

    # ---- the regime gate (invariant 000).
    model_asks = [r for r in asks if r.get("reasoning_chars") is not None]
    print("ask records %d model decisions %d" % (len(asks), len(model_asks)))
    print("with reasoning %d thinking values %s"
          % (sum(1 for r in model_asks if (r.get("reasoning_chars") or 0) > 0),
             set(str(r.get("thinking")) for r in model_asks)))

    # ---- CORPUS-WIDE (every seam that made a model call) vs ASK-SEAM ONLY.
    allmodel = [r for r in allrecs
                if r.get("kind") not in NON_DECISION_KINDS
                and r.get("reasoning_chars") is not None]
    lat_all = [r["latency_ms"] for r in allmodel
               if isinstance(r.get("latency_ms"), (int, float)) and r["latency_ms"] > 0]
    print("CORPUS-WIDE (all kinds): model decisions %d off_protocol_bytes>0 %d "
          "plan_line_missing %d latency p50 %.1f s"
          % (len(allmodel),
             sum(1 for r in allmodel if (r.get("off_protocol_bytes") or 0) > 0),
             sum(1 for r in allmodel if r.get("plan_line_missing")),
             pct(lat_all, 0.5)))
    print("ASK-SEAM ONLY: reply_truncated %d off_protocol_bytes>0 %d "
          "plan_line_missing %d action_before_plan %d"
          % (sum(1 for r in asks if r.get("reply_truncated")),
             sum(1 for r in model_asks if (r.get("off_protocol_bytes") or 0) > 0),
             sum(1 for r in model_asks if r.get("plan_line_missing")),
             sum(1 for r in model_asks if r.get("action_before_plan"))))
    lat = [r["latency_ms"] for r in model_asks
           if isinstance(r.get("latency_ms"), (int, float)) and r["latency_ms"] > 0]
    if lat:
        print("latency p50 %.1f s p95 %.1f s n=%d" % (pct(lat, 0.5), pct(lat, 0.95), len(lat)))
    rc = sorted(r["reasoning_chars"] for r in model_asks if r.get("reasoning_chars"))
    if rc:
        print("reasoning_chars median %d max %d" % (rc[len(rc) // 2], rc[-1]))

    # ---- protocol deviation classes (the `answer_label_absent` population).
    dev = collections.Counter(r.get("protocol_deviation") for r in allmodel
                              if r.get("protocol_deviation"))
    print("protocol_deviation classes %s" % dict(dev))

    turns = 0
    for recs in seen.values():
        t = [r.get("turn") for r in recs if isinstance(r.get("turn"), int)]
        if t:
            turns += max(t)
    print("decisions/turn (model decisions / sum of max turn per seat) %.2f"
          % (len(model_asks) / turns if turns else 0))

    # ---- gameend counter sums.
    sums = {}
    for r in allrecs:
        if r.get("kind") != "gameend":
            continue
        for k, v in r.items():
            if isinstance(v, int) and not isinstance(v, bool) \
                    and k not in ("seq", "turn", "my_life", "opp_life"):
                sums[k] = sums.get(k, 0) + v
    print("GAMEEND SUMS:")
    print(json.dumps(dict(sorted(sums.items())), indent=None))
    # the census rule the wave-77 seat asked for, stated rather than inferred.
    pr = sums.get("protocol_replies")
    if pr is not None:
        print("CENSUS CHECK: gameend protocol_replies %d vs record-kind sum %d "
              "(difference %d = the kinds that carry no round trip: %s)"
              % (pr, decision_records, decision_records - pr,
                 census_kind_list(kinds) or "(none)"))  #W78-CY (F12)
        if census_kind_sum(kinds) != decision_records - pr:
            print("CENSUS CHECK MISMATCH: the listed kinds sum to %d but the difference"
                  " is %d - a record kind is on the wrong side of ROUNDTRIP_KINDS"
                  % (census_kind_sum(kinds), decision_records - pr))

    print("askreplay files: %d"
          % sum(len(glob.glob(os.path.join(d, "askreplay", "*.jsonl"))) for d in dirs))

    # ---- wins.
    wins, games = {}, {}
    for d in dirs:
        p = os.path.join(d, "results.tsv")
        if not os.path.exists(p):
            continue
        for line in open(p):
            c = line.split("\t")
            if len(c) < 3 or not c[0].isdigit():
                continue
            a, b, w = c[0], c[1], c[2].strip()
            games[a] = games.get(a, 0) + 1
            games[b] = games.get(b, 0) + 1
            if w == "0":
                wins[a] = wins.get(a, 0) + 1
            elif w == "1":
                wins[b] = wins.get(b, 0) + 1
    if games:
        print("WINS per deck (from results.tsv):")
        print(" ".join("deck%s %d/%d" % (k, wins.get(k, 0), games[k])
                       for k in sorted(games, key=lambda k: -wins.get(k, 0) / games[k])))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
