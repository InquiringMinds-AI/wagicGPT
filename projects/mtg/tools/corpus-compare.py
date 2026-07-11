#!/usr/bin/env python3
"""Compare two selfplay-harness corpus runs on the W4 instruments.

Usage: corpus-compare.py BASELINE_DIR CANDIDATE_DIR

Reads every per-seat translog (*.jsonl) in each run directory and reports,
per run and as a delta:
  - games / seats / decisions
  - cast-nothing rate: "Cast nothing right now" picks / casting asks offered
  - offered-vs-taken: mean options per ask, share of asks answered with a
    real action (not decline/nothing)
  - window count: priority-kind decisions per seat (the stops engine's
    response windows actually taken to a decision)
  - fallback rate: decisions with a fallback reason (empty/unparsed reply)
  - game completion: seats with a gameend record / expected, timeouts
  - decision latency: median/p90 of latency_ms (excluding cache hits = -1)

The instruments are DECISION-QUALITY telemetry, not win-rate: both seats run
the same core prompt, so win-rate is blind to it (see the wagicgpt skill).
"""
import json
import os
import statistics
import sys


def load_run(d):
    seats = []
    for name in sorted(os.listdir(d)):
        if not name.endswith(".jsonl"):
            continue
        recs = []
        with open(os.path.join(d, name)) as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                try:
                    recs.append(json.loads(line))
                except ValueError:
                    pass
        if recs:
            seats.append((name, recs))
    return seats


def analyze(seats):
    m = {
        "seats": len(seats),
        "decisions": 0,
        "casting_asks": 0,
        "cast_nothing": 0,
        "real_action": 0,
        "declines": 0,
        "options_sum": 0,
        "options_n": 0,
        "windows": 0,
        "fallbacks": 0,
        "gameends": 0,
        "wins": 0,
        "latencies": [],
    }
    for name, recs in seats:
        for r in recs:
            kind = r.get("kind", "")
            if kind == "gameend":
                m["gameends"] += 1
                if r.get("won"):
                    m["wins"] += 1
                continue
            m["decisions"] += 1
            if r.get("fallback"):
                m["fallbacks"] += 1
            if kind == "priority":
                m["windows"] += 1
            lat = r.get("latency_ms")
            if isinstance(lat, (int, float)) and lat >= 0:
                m["latencies"].append(lat)
            opts = r.get("optionCount", r.get("options"))
            if isinstance(opts, int) and opts > 0:
                m["options_sum"] += opts
                m["options_n"] += 1
            chosen = str(r.get("chosen_text", ""))
            prompt_tail = r.get("prompt", "")[-400:]
            choice = r.get("choice")
            if "Casting decision" in prompt_tail or "Cast nothing right now" in prompt_tail:
                m["casting_asks"] += 1
                if chosen:
                    if "Cast nothing" in chosen:
                        m["cast_nothing"] += 1
                elif isinstance(choice, int) and isinstance(opts, int) and choice == opts:
                    # pre-c2 schema: no chosen_text; the decline is the LAST
                    # option of a casting ask and choice is 1-based
                    m["cast_nothing"] += 1
            if chosen:
                if chosen.startswith(("Cast ", "Play ")):
                    m["real_action"] += 1
                elif "Decline" in chosen or "Cast nothing" in chosen or "No -" in chosen:
                    m["declines"] += 1
    return m


def fmt(m):
    lat = m["latencies"]
    lines = []
    lines.append("  seats: %d  gameends: %d  decisions: %d" % (m["seats"], m["gameends"], m["decisions"]))
    if m["casting_asks"]:
        lines.append("  cast-nothing rate: %d/%d = %.1f%%" % (m["cast_nothing"], m["casting_asks"], 100.0 * m["cast_nothing"] / m["casting_asks"]))
    if m["options_n"]:
        lines.append("  mean options per ask: %.2f" % (m["options_sum"] / float(m["options_n"])))
    lines.append("  real-action picks: %d  declines/nothing: %d" % (m["real_action"], m["declines"]))
    lines.append("  priority windows taken: %d (%.2f per seat)" % (m["windows"], m["windows"] / float(m["seats"] or 1)))
    lines.append("  fallbacks: %d/%d = %.1f%%" % (m["fallbacks"], m["decisions"] or 1, 100.0 * m["fallbacks"] / (m["decisions"] or 1)))
    if lat:
        lines.append("  latency ms: median %d  p90 %d  (n=%d live calls)" % (statistics.median(lat), sorted(lat)[int(0.9 * len(lat)) - 1], len(lat)))
    return "\n".join(lines)


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    for label, d in (("BASELINE", sys.argv[1]), ("CANDIDATE", sys.argv[2])):
        seats = load_run(d)
        if not seats:
            sys.exit("no translogs found in %s" % d)
        print("%s  %s" % (label, d))
        print(fmt(analyze(seats)))
        print()


if __name__ == "__main__":
    main()
