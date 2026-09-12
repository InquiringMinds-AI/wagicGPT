#!/usr/bin/env python3
"""#W79-DB (wave-79 lane DB, T10): the PER-DECK PROMPT-BYTE CENSUS, as a tool.

Wave-78 item T10 has two halves and each needs its own number, measured on the
prompts the model actually saw (each seat-log record's `prompt` field is the exact
bytes of that window):

  * the S13 HOIST residual - the same `{card text: "..."}` body repeated on two or
    more rows of one menu. Reported as bytes and as the count of prompts carrying
    one, with the split by whether the body is under the wave-78 100-byte floor
    (which is what the wave-79 savings gate replaces).
  * the NARRATION TRIM - the GAME LOG block at the head of every prompt. The
    wave-78 trim fired at 24,000 B and kept the newest 20,000; wave 79 fires at
    16,000 and keeps 12,000, so a log in the old steady band [20,000..24,000] lands
    in the new band [12,000..16,000] at the same point in its own cycle. The model
    printed below is exactly that: a log at or under the new TRIGGER is untouched,
    and one above it loses the 8,000-byte difference between the two bands.

The run directory is a required ARGUMENT and is echoed on the first output line
(LESSON of wave 75). Nothing here is a prediction about the pilot: it is arithmetic
over bytes already on disk.

  usage:  python3 tools/prompt-byte-census.py <run-dir> [<run-dir> ...]
          python3 tools/prompt-byte-census.py --selftest
"""
import collections
import glob
import json
import os
import re
import sys

CARD_TEXT = re.compile(r' \{card text: "(.*?)"\}', re.S)
CARD_TEXT_WRAP = len(' {card text: ""}')
HOIST_HEAD_FIXED = len(
    "Card text shared by options "
    " (they all act with the one card this text belongs to, so it is printed"
    " once here instead of on each of them): \"\"\n")
W78_BODY_FLOOR = 100      # the wave-78 hoist gate
W78_TRIM_TRIGGER = 24000  # the wave-78 narration trim
W79_TRIM_TRIGGER = 16000  # ...and wave 79's
W79_TRIM_KEEP = 12000
LOG_HEAD = "GAME LOG (everything that has happened so far):"


def log_block_bytes(prompt):
    """The GAME LOG block: the head line, then every log line up to the first line
    that is neither an event bullet nor a turn header nor the trim marker. Returns
    (bytes, trimmed?) - 0 when this prompt carries no log at all."""
    if not prompt.startswith(LOG_HEAD):
        return 0, False
    lines = prompt.split("\n")
    n = 0
    trimmed = False
    for ln in lines[1:]:
        if ln.startswith("(...earlier events trimmed"):
            trimmed = True
        elif not (ln.startswith("- ") or ln.startswith("=== Turn")):
            break
        n += len(ln) + 1
    return n, trimmed


def w79_log_bytes(now):
    """The same log under the wave-79 trim. A log at or under the new trigger is
    untouched; above it, the whole steady band shifts down by the difference
    between the two keep sizes, floored at the new keep."""
    if now <= W79_TRIM_TRIGGER:
        return now
    return max(W79_TRIM_KEEP, now - (W78_TRIM_TRIGGER - W79_TRIM_TRIGGER))


def hoist_saving(prompt):
    """Bytes the wave-79 hoist removes from this prompt, and the bytes the wave-78
    gate would have removed. Both count only groups on 2+ rows."""
    c = collections.Counter(CARD_TEXT.findall(prompt))
    w78 = w79 = 0
    for body, rows in c.items():
        if rows < 2:
            continue
        removed = rows * (len(body) + CARD_TEXT_WRAP)
        added = HOIST_HEAD_FIXED + len(body) + 8  # + the row-number list
        pays = removed > added and len(body) >= 8
        if len(body) >= W78_BODY_FLOOR:
            w78 += removed - added
            w79 += removed - added
        elif pays:
            w79 += removed - added
    return w79, w78


def deck_of(basename):
    m = re.search(r"ai_baka_deck(\d+)", basename)
    return m.group(1) if m else "?"


def selftest():
    ok = True
    # the log block: header + three bullets + a turn header, then the blank line
    p = (LOG_HEAD + "\n- a\n=== Turn 2 - YOUR turn ===\n- b\n\nYour life: 20\n")
    n, tr = log_block_bytes(p)
    if n != len("- a\n") + len("=== Turn 2 - YOUR turn ===\n") + len("- b\n") or tr:
        print("SELFTEST FAIL: log block %d trimmed=%s" % (n, tr))
        ok = False
    n2, tr2 = log_block_bytes(LOG_HEAD + "\n(...earlier events trimmed - x)\n- a\n\nx")
    if not tr2 or n2 != len("(...earlier events trimmed - x)\n") + len("- a\n"):
        print("SELFTEST FAIL: trimmed log block %d %s" % (n2, tr2))
        ok = False
    if log_block_bytes("Your life: 20") != (0, False):
        print("SELFTEST FAIL: a prompt with no log reports none")
        ok = False
    # the trim model
    if w79_log_bytes(9000) != 9000 or w79_log_bytes(16000) != 16000 \
            or w79_log_bytes(20647) != 12647 or w79_log_bytes(23156) != 15156:
        print("SELFTEST FAIL: trim model")
        ok = False
    # the hoist: the corpus's own Staff of Nin body, 96 B, on eleven rows
    body = ("At the beginning of your upkeep, draw a card. -- {T}: Staff of Nin deals"
            " 1 damage to any target.")
    if len(body) != 96:
        print("SELFTEST FAIL: fixture body is %d B, not 96" % len(body))
        ok = False
    fake = "".join(' {card text: "%s"}' % body for _ in range(11))
    w79, w78 = hoist_saving(fake)
    if w78 != 0 or w79 < 900:
        print("SELFTEST FAIL: hoist %d (w78 %d)" % (w79, w78))
        ok = False
    # ...and a long body on two rows is hoisted under BOTH gates
    long_body = "z" * 400
    w79b, w78b = hoist_saving(' {card text: "%s"} {card text: "%s"}'
                              % (long_body, long_body))
    if w78b != w79b or w78b <= 0:
        print("SELFTEST FAIL: long body %d %d" % (w79b, w78b))
        ok = False
    print("prompt-byte-census selftest: %s" % ("OK" if ok else "FAILED"))
    return 0 if ok else 1


def main(argv):
    if len(argv) == 2 and argv[1] == "--selftest":
        return selftest()
    if len(argv) < 2:
        print(__doc__)
        return 2
    dirs = [os.path.abspath(os.path.expanduser(a)) for a in argv[1:]]
    print("CORPUS: " + " ".join(dirs))
    per = collections.defaultdict(lambda: {"n": 0, "now": 0, "then": 0, "max_now": 0,
                                           "max_then": 0, "over20_now": 0,
                                           "over20_then": 0, "hoist": 0, "dup": 0,
                                           "logtrim": 0})
    seen = set()
    for d in dirs:
        for f in sorted(glob.glob(os.path.join(d, "*.jsonl"))):
            b = os.path.basename(f)
            if b in seen:
                continue
            seen.add(b)
            deck = deck_of(b)
            s = per[deck]
            for line in open(f, errors="replace"):
                line = line.strip()
                if not line:
                    continue
                try:
                    r = json.loads(line)
                except ValueError:
                    continue
                p = r.get("prompt")
                if not p:
                    continue
                now = len(p)
                logn, _tr = log_block_bytes(p)
                w79, _w78 = hoist_saving(p)
                trimmed = logn - w79_log_bytes(logn)
                then = now - w79 - trimmed
                s["n"] += 1
                s["now"] += now
                s["then"] += then
                s["max_now"] = max(s["max_now"], now)
                s["max_then"] = max(s["max_then"], then)
                s["over20_now"] += 1 if now > 20000 else 0
                s["over20_then"] += 1 if then > 20000 else 0
                s["hoist"] += w79
                s["dup"] += 1 if w79 else 0
                s["logtrim"] += trimmed
    print("PER DECK (now = the wave-78 prompts as measured; after = the same prompts"
          " under the wave-79 hoist gate and 16 KB/12 KB narration trim)")
    tot = {"n": 0, "now": 0, "then": 0, "hoist": 0, "logtrim": 0, "dup": 0,
           "over20_now": 0, "over20_then": 0}
    for deck in sorted(per):
        s = per[deck]
        print("  deck%-4s n=%-5d mean %6d -> %6d   max %6d -> %6d   >20KB %4d -> %4d"
              "   hoist %7d B (%d prompts)   log trim %8d B"
              % (deck, s["n"], s["now"] // max(1, s["n"]), s["then"] // max(1, s["n"]),
                 s["max_now"], s["max_then"], s["over20_now"], s["over20_then"],
                 s["hoist"], s["dup"], s["logtrim"]))
        for k in tot:
            tot[k] += s[k]
    print("  TOTAL   n=%-5d mean %6d -> %6d   >20KB %4d -> %4d   hoist %7d B"
          " (%d prompts)   log trim %8d B   all prompts %d -> %d B (-%.1f%%)"
          % (tot["n"], tot["now"] // max(1, tot["n"]), tot["then"] // max(1, tot["n"]),
             tot["over20_now"], tot["over20_then"], tot["hoist"], tot["dup"],
             tot["logtrim"], tot["now"], tot["then"],
             100.0 * (tot["now"] - tot["then"]) / max(1, tot["now"])))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
