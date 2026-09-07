#!/usr/bin/env python3
"""#W72-BU (M4) RED-on-base evidence.

Replays the wave-71 corpus and counts the DOUBLE ASK the window model removes:
a `Casting decision` ask and its sibling `priority` ask issued at the same
(seat, turn, phase) over a byte-identical board.  On the base engine both calls
are made; with the window-scoped hold (gptHoldCoversSiblingWindow) the pairs
whose casting ask was answered with the HOLD row make only one.

Only the `--- CURRENT SITUATION ---` block is compared, minus the two lines that
move BECAUSE the first ask was answered (the carried `YOUR PLAN` heading and its
`(note: ...)` continuation) - those are the answer being consumed, not the board
moving.  Everything else, including every price and count, is compared byte for
byte.

Usage: lane-BU-double-ask-red.py [corpus-dir]
       (default ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-085638)
"""
import collections
import glob
import json
import os
import re
import sys

CORPUS = sys.argv[1] if len(sys.argv) > 1 else os.path.expanduser(
    "~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-085638")


def situation(prompt):
    i = prompt.find("--- CURRENT SITUATION ---")
    if i < 0:
        return None
    ends = [x for x in (prompt.find("Your legal actions", i),
                        prompt.find("Casting decision", i)) if x > 0]
    body = prompt[i:min(ends) if ends else len(prompt)]
    return "\n".join(l for l in body.split("\n")
                     if not l.startswith("YOUR PLAN") and not l.startswith("(note:"))


pairs = 0
answers = collections.Counter()
seats = collections.Counter()
examples = []
windows = 0
for path in sorted(glob.glob(os.path.join(CORPUS, "*.jsonl"))):
    seat = re.search(r"-(ai_baka_deck\d+)-", os.path.basename(path))
    seat = seat.group(1) if seat else os.path.basename(path)
    recs = []
    for line in open(path, errors="replace"):
        try:
            r = json.loads(line)
        except ValueError:
            continue
        if r.get("kind") in ("ask", "priority"):
            recs.append(r)
    windows += len(recs)
    for i in range(len(recs) - 1):
        a, b = recs[i], recs[i + 1]
        if a["kind"] == b["kind"]:
            continue
        if a.get("turn") != b.get("turn") or a.get("phase") != b.get("phase"):
            continue
        cast = a if "Casting decision" in a.get("prompt", "") else (
            b if "Casting decision" in b.get("prompt", "") else None)
        if cast is None:
            continue
        sa, sb = situation(a.get("prompt", "")), situation(b.get("prompt", ""))
        if sa is None or sa != sb:
            continue
        pairs += 1
        seats[seat] += 1
        chosen = (cast.get("chosen_text") or "").lower()
        if chosen.startswith("hold priority"):
            answers["HOLD (collapses after the fix)"] += 1
            if len(examples) < 5:
                examples.append("%s seq %s->%s turn %s %s" %
                                (seat, a.get("seq"), b.get("seq"),
                                 a.get("turn"), a.get("phase")))
        elif "cast nothing" in chosen:
            answers["cast nothing (still two asks, by design)"] += 1
        else:
            answers["a cast was taken (still two asks)"] += 1

print("corpus:            %s" % CORPUS)
print("ask/priority windows: %d" % windows)
print("same-window cast/priority DOUBLE ASKS: %d (%.1f%% of windows)"
      % (pairs, 100.0 * pairs / windows if windows else 0))
for k, v in answers.most_common():
    print("   %4d  %s" % (v, k))
print("by seat: %s" % dict(seats))
print("examples of the collapsing class:")
for e in examples:
    print("   %s" % e)
