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


#W79-DB (T17, wave-78 engine-seat CY F12 FAIL): A ROUND-TRIP KIND THAT MADE NO
#ROUND TRIP. `152v123` seq 22 is `kind: reveal`, `fallback: engine_answered`,
#`latency_ms: -1`, no prompt and no reply - a reveal whose eligibility filter
#selected zero rows, so there was nothing to ask and nothing was asked. `reveal`
#is in ROUNDTRIP_KINDS, so the wave-78 census listed 40 and the difference was
#41 and the tool printed CENSUS CHECK MISMATCH on a corpus with no defect in it.
#The fix does NOT move the `reveal` kind (every other reveal DID make a round
#trip): it counts, per RECORD, the ones that provably did not.
ENGINE_ANSWERED_LABEL = "engine-answered (no round trip)"


def engine_answered_records(records):
    """Round-trip-kind records that made no round trip: the engine answered the
    window itself, or the record's own latency says no call was made."""
    n = 0
    for r in records:
        if r.get("kind") not in ROUNDTRIP_KINDS:
            continue
        if r.get("fallback") == "engine_answered" or r.get("latency_ms") == -1:
            n += 1
    return n


def no_roundtrip_kinds(kinds):
    """Every kind in the census that protocol_replies does not count."""
    return sorted(k for k in kinds
                  if k and k not in NON_DECISION_KINDS and k not in ROUNDTRIP_KINDS)


def census_kind_sum(kinds, engine_answered=0):
    """...and its total, so a mis-classified kind is LOUD, not silent."""
    return sum(kinds[k] for k in no_roundtrip_kinds(kinds)) + engine_answered


def census_kind_list(kinds, engine_answered=0):
    """The CENSUS CHECK's explanatory list, as printed."""
    parts = ["%s %d" % (k, kinds[k]) for k in no_roundtrip_kinds(kinds)
             if kinds.get(k)]
    if engine_answered:
        parts.append("%s %d" % (ENGINE_ANSWERED_LABEL, engine_answered))
    return ", ".join(parts)


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
    #W80-DH (F11): the wave-80 lane's new kind, `hold_event` - one record per hold
    # clamp / hold re-open - is a decision-adjacent record that makes NO round trip,
    # and the derived census explains it with no edit to this tool. Pinned here so
    # the wave-77 lesson ("the census must sum every record kind, including the new
    # ones a lane adds") is a test rather than a hope.
    k3 = _c.Counter({"ask": 4, "hold_event": 6, "menu_single_outcome": 1, "gameend": 1})
    if census_kind_list(k3) != "hold_event 6, menu_single_outcome 1" \
            or census_kind_sum(k3) != 7:
        print("SELFTEST FAIL: hold_event census %r / %d"
              % (census_kind_list(k3), census_kind_sum(k3)))
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
    #W79-DB (T17): the engine-answered reveal, which is what the wave-78 corpus
    #actually holds. One reveal that made no round trip, and the difference the
    #gameend counter leaves must include it.
    ea = [{"kind": "reveal", "fallback": "engine_answered", "latency_ms": -1},
          {"kind": "reveal", "fallback": None, "latency_ms": 41000},
          {"kind": "ask", "fallback": "unparsed_reply", "latency_ms": 90500},
          {"kind": "recovery", "fallback": None}]
    if engine_answered_records(ea) != 1:
        print("SELFTEST FAIL: engine_answered_records %d" % engine_answered_records(ea))
        ok = False
    #a latency of -1 alone is enough - a record that names no call made none
    if engine_answered_records([{"kind": "ask", "latency_ms": -1}]) != 1 \
            or engine_answered_records([{"kind": "forced_close", "latency_ms": -1}]) != 0:
        print("SELFTEST FAIL: latency -1 rule")
        ok = False
    k4 = _c.Counter({"gamestart": 42, "system": 42, "ask": 1906, "bottom": 7,
                     "priority": 532, "attackers": 93, "forced_close": 32,
                     "gameend": 42, "reveal": 18, "blockers": 22, "discard": 52,
                     "recovery": 8})
    d4 = sum(v for k, v in k4.items() if k not in NON_DECISION_KINDS)
    if d4 != 2670 or census_kind_sum(k4, 1) != d4 - 2629 \
            or census_kind_list(k4, 1) != ("forced_close 32, recovery 8, "
                                           + ENGINE_ANSWERED_LABEL + " 1"):
        print("SELFTEST FAIL: wave-78 corpus reconciliation %d %d %r"
              % (d4, census_kind_sum(k4, 1), census_kind_list(k4, 1)))
        ok = False
    #...and with no engine-answered record the listing is byte-identical to wave 78
    if census_kind_list(k4, 0) != "forced_close 32, recovery 8":
        print("SELFTEST FAIL: zero engine-answered changed the listing")
        ok = False
    #W79-DC (F11): the turn-gap explanation, on the wave-78 corpus's own figures.
    if turn_gap_note(1016, 974, 42) != (" | gap 42 = 42 seat logs x the +1 narration"
                                        " offset, exactly: every seat's last record is"
                                        " on the game's final turn"):
        print("SELFTEST FAIL: turn gap 1016/974/42 %r" % turn_gap_note(1016, 974, 42))
        ok = False
    #a seat that stopped writing records early reads LOWER, and the note says which way
    if "BELOW" not in turn_gap_note(1010, 974, 42) \
            or "6 turn(s) of records are missing" not in turn_gap_note(1010, 974, 42):
        print("SELFTEST FAIL: turn gap below %r" % turn_gap_note(1010, 974, 42))
        ok = False
    #...and a gap ABOVE the offset is flagged as an impossibility, never explained away
    if "EXCEEDS" not in turn_gap_note(1020, 974, 42):
        print("SELFTEST FAIL: turn gap above %r" % turn_gap_note(1020, 974, 42))
        ok = False
    #no results.tsv: no claim at all
    if turn_gap_note(1016, 0, 42) != "" or turn_gap_note(1016, 974, 0) != "":
        print("SELFTEST FAIL: turn gap with no results.tsv claimed something")
        ok = False
    #W80-DG (U17): the seam-cost table and the correlation, on mocked records.
    mock = [
        {"kind": "ask", "latency_ms": 100, "prompt": "x" * 10, "reasoning_chars": 10},
        {"kind": "ask", "latency_ms": 300, "prompt": "x" * 30, "reasoning_chars": 30},
        {"kind": "ask", "latency_ms": 200, "prompt": "x" * 20, "reasoning_chars": 20},
        {"kind": "priority", "latency_ms": 50, "prompt": "x" * 5, "reasoning_chars": 0},
        {"kind": "ask", "latency_ms": -1, "prompt": "x" * 999, "reasoning_chars": 999},
    ]
    rows = w80_seam_cost_rows(mock)
    if [r[0] for r in rows] != ["ask", "priority"] or rows[0][1] != 3 \
            or rows[0][2] != 200 or rows[0][3] != 20 or rows[0][4] != 20 \
            or abs(rows[0][5] - 10.0) > 1e-9:
        print("SELFTEST FAIL: seam cost rows %r" % (rows,))
        ok = False
    #a cache hit (latency -1) never enters the table - it made no round trip
    if rows[1][1] != 1 or rows[1][5] is not None:
        print("SELFTEST FAIL: no-reasoning seam %r" % (rows[1],))
        ok = False
    if abs(w80_pearson([1, 2, 3], [2, 4, 6]) - 1.0) > 1e-9 \
            or abs(w80_pearson([1, 2, 3], [6, 4, 2]) + 1.0) > 1e-9 \
            or w80_pearson([1, 1, 1], [1, 2, 3]) is not None \
            or w80_pearson([1], [1]) is not None:
        print("SELFTEST FAIL: pearson")
        ok = False
    print("corpus-stats selftest: %s" % ("OK" if ok else "FAILED"))
    return 0 if ok else 1


#W80-DG (U17, wave-79 known-bugs MED - MEASURE ONLY, no behaviour changes):
#latency p50 rose 78 -> 94 s on FEWER decisions (2,629 -> 2,203) with a flat
#reasoning median (5,835), and the wave-79 seat could not say why. The three
#quantities that could move it are the seam mix, the prompt the model reads and
#the reasoning it writes, so they are reported TOGETHER, per seam, plus the
#correlation of latency against each. Nothing here is a prediction: it is
#arithmetic over records already on disk.
def w80_pearson(xs, ys):
    """Pearson r over two equal-length numeric lists; None when undefined."""
    n = len(xs)
    if n < 2 or n != len(ys):
        return None
    mx = sum(xs) / float(n)
    my = sum(ys) / float(n)
    sxx = sum((x - mx) ** 2 for x in xs)
    syy = sum((y - my) ** 2 for y in ys)
    if sxx <= 0 or syy <= 0:
        return None
    sxy = sum((x - mx) * (y - my) for x, y in zip(xs, ys))
    return sxy / ((sxx ** 0.5) * (syy ** 0.5))


def w80_seam_cost_rows(records):
    """One row per record KIND that cost a round trip: (kind, n, latency ms p50,
    prompt bytes p50, reasoning_chars p50, ms per reasoning char at the medians).
    A record with no positive latency_ms made no round trip and is not counted."""
    by = collections.defaultdict(lambda: {"lat": [], "pb": [], "rc": []})
    for r in records:
        lat = r.get("latency_ms")
        if not isinstance(lat, (int, float)) or lat <= 0:
            continue
        s = by[r.get("kind") or "?"]
        s["lat"].append(float(lat))
        s["pb"].append(len(r.get("prompt") or ""))
        s["rc"].append(int(r.get("reasoning_chars") or 0))
    rows = []
    for kind in sorted(by):
        s = by[kind]
        lat = statistics.median(s["lat"])
        pb = statistics.median(s["pb"])
        rc = statistics.median(s["rc"])
        rows.append((kind, len(s["lat"]), lat, pb, rc, (lat / rc) if rc else None))
    return rows


def results_turn_sum(dirs):
    """#W79-DB (T17): the GAME's turn count, summed over both seats of every
    finished game, read from results.tsv. 0 when no results.tsv is present."""
    total = 0
    for d in dirs:
        p = os.path.join(d, "results.tsv")
        if not os.path.exists(p):
            continue
        for line in open(p):
            c = line.split("\t")
            if len(c) < 6 or not c[0].isdigit():
                continue
            try:
                total += 2 * int(c[5])
            except ValueError:
                pass
    return total


def turn_gap_note(record_turns, results_turns, seat_logs):
    """#W79-DC (F11): the gap between the two turn columns, explained by mechanism.

    The only two contributors are the +1 narration offset (one per seat log) and a
    seat whose last record predates the final turn (which makes the record column
    LOWER). A gap of exactly `seat_logs` is the offset alone."""
    if not results_turns or not seat_logs:
        return ""
    gap = record_turns - results_turns
    if gap == seat_logs:
        return (" | gap %d = %d seat logs x the +1 narration offset, exactly: every"
                " seat's last record is on the game's final turn"
                % (gap, seat_logs))
    if gap < seat_logs:
        return (" | gap %d is BELOW the %d expected from the +1 offset, so %d turn(s)"
                " of records are missing: that many seats stopped writing records"
                " before the game's final turn (held, collapsed or heuristic-answered"
                " to the end)" % (gap, seat_logs, seat_logs - gap))
    return (" | gap %d EXCEEDS the %d expected from the +1 offset by %d - a record"
            " `turn` past the game's own final turn, which should not happen; check"
            " for a seat log paired with the wrong results.tsv row"
            % (gap, seat_logs, gap - seat_logs))


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
    #W79-DB (T17): ...and the round-trip-kind records that made no round trip.
    engine_answered = engine_answered_records(allrecs)
    print("  ...of which %-13s %d (round-trip kinds the engine answered itself, or"
          " whose latency_ms is -1)" % ("engine_answered", engine_answered))

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

    #W80-DG (U17, MEASURE ONLY): per-seam latency against the two things that
    #could be paying for it - the bytes the model READ and the reasoning it WROTE.
    rows = w80_seam_cost_rows(allmodel)
    if rows:
        print("PER-SEAM COST (U17 - latency vs prompt bytes vs reasoning_chars, medians;"
              " records with no round trip excluded):")
        print("  %-14s %6s %10s %12s %12s %12s"
              % ("seam", "n", "lat p50 s", "prompt B p50", "reason p50", "ms/reas ch"))
        for (kind, n, lat, pb, rc, per) in rows:
            print("  %-14s %6d %10.1f %12d %12d %12s"
                  % (kind, n, lat / 1000.0, int(pb), int(rc),
                     ("%.3f" % per) if per is not None else "n/a"))
        pairs = [(r, len(r.get("prompt") or ""), int(r.get("reasoning_chars") or 0))
                 for r in allmodel
                 if isinstance(r.get("latency_ms"), (int, float)) and r["latency_ms"] > 0]
        lats = [float(r["latency_ms"]) for (r, _pb, _rc) in pairs]
        rpb = w80_pearson([pb for (_r, pb, _rc) in pairs], lats)
        rrc = w80_pearson([rc for (_r, _pb, rc) in pairs], lats)
        print("  CORRELATION with latency over %d round trips: prompt bytes r=%s,"
              " reasoning_chars r=%s (r is not a cause; a near-zero pair means"
              " neither quantity explains the wait and the answer is elsewhere -"
              " server queueing, seam mix, or output tokens)"
              % (len(pairs),
                 ("%.3f" % rpb) if rpb is not None else "n/a",
                 ("%.3f" % rrc) if rrc is not None else "n/a"))

    # ---- protocol deviation classes (the `answer_label_absent` population).
    dev = collections.Counter(r.get("protocol_deviation") for r in allmodel
                              if r.get("protocol_deviation"))
    print("protocol_deviation classes %s" % dict(dev))

    #W79-DB (T17, wave-78 known-bugs): WHICH TURN NUMBER. The wave-78 late-check
    #diagnosis read the seat log's per-record `turn` (62/81) while results.tsv
    #says 85/68 for the same two games. The tool names its source and prints BOTH.
    #W79-DC (F11, Astra wave-79 review finding 11): AND THE EXPLANATION WAS WRONG.
    #There is no per-seat turn count anywhere in this engine. The record `turn` is
    #`AIPlayerGPT::translogTurn`, which is `observer->turn + 1` - the GAME's turn as
    #the narration prints it ("=== Turn 10"), identical for both seats, which is the
    #whole point of #W51-D (D18). results.tsv's `turn` is the raw `game->turn` that
    #GameStateDuel.cpp prints on the WAGIC_SELFPLAY_RESULT line at game end. So the
    #two columns differ for exactly two reasons, neither of them a seat:
    #  (a) the +1 narration offset, worth ONE per seat log;
    #  (b) a seat whose LAST RECORD predates the final turn (it was held, collapsed or
    #      heuristic-answered through the end of the game) reads LOWER, not higher.
    #The wave-78 corpus is 1,016 vs 974 over 42 seat logs, and 1016 - 974 = 42 = one
    #per seat - i.e. (a) alone, with (b) contributing nothing: every seat's last record
    #landed on the game's final turn. The reported late-check discrepancy is therefore
    #NOT a turn-numbering artefact at all, and a diagnosis that blamed one was looking
    #in the wrong place.
    record_turns = 0
    for recs in seen.values():
        t = [r.get("turn") for r in recs if isinstance(r.get("turn"), int)]
        if t:
            record_turns += max(t)
    results_turns = results_turn_sum(dirs)
    seat_logs = len(seen)
    print("TURN SOURCES: sum of max record `turn` per seat %d (translogTurn ="
          " observer turn + 1, the GAME's turn as the narration prints it - NOT a"
          " per-seat count) | sum of results.tsv `turn` x2 seats %s (game->turn at"
          " game end, no +1)%s"
          % (record_turns, results_turns if results_turns else "n/a",
             turn_gap_note(record_turns, results_turns, seat_logs)))
    turns = results_turns or record_turns
    print("decisions/turn (model decisions / %s) %.2f"
          % ("results.tsv game turns x2 seats" if results_turns
             else "sum of max record turn per seat",
             len(model_asks) / turns if turns else 0))

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
              "(difference %d = the records that carry no round trip: %s)"
              % (pr, decision_records, decision_records - pr,
                 census_kind_list(kinds, engine_answered) or "(none)"))  #W79-DB (T17)
        if census_kind_sum(kinds, engine_answered) != decision_records - pr:
            print("CENSUS CHECK MISMATCH: the listed records sum to %d but the difference"
                  " is %d - a record kind is on the wrong side of ROUNDTRIP_KINDS"
                  % (census_kind_sum(kinds, engine_answered), decision_records - pr))
        else:
            print("CENSUS CHECK RECONCILES.")

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
