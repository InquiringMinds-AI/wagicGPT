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


#W80-DG (U16, wave-79 known-bugs MED / deck146 MED-3): THE HOLD APPARATUS,
#MEASURED. Three surfaces state the hold contract on one prompt - the hold ROW,
#the `[hold check:` bracket and the `[HOW A HOLD ENDS:` paragraph - and the
#deck146 seat put the three together at ~1.4 KB of every prompt. The wave-80
#fold takes the saving at the BRACKET only: its "what counts as the same row"
#rule is stated in full by the paragraph one block below, on exactly the windows
#that carry a hold row, and unlike the ROW's own re-opener clause (which #W61-U
#C14 put there from live evidence and three pins assert) nothing claims the
#bracket must say it. This measures both the before and the after.
HOLD_ROW_HEAD = "Hold priority - pass now, and do not ask me again"
HOLD_CHECK_HEAD = "[hold check: "
HOLD_CONTRACT_HEAD = "[HOW A HOLD ENDS:"
#the three rule tails the wave-80 fold replaces, and what it replaces them with
W80_FOLDED_TAILS = (
    " - a row that changes only in its annotations (a forecast, a clock, a count,"
    " a life total) is the SAME row and does not re-open a hold; the same action at"
    " a CHANGED COST is a different row, as is a row appearing, disappearing, or"
    " naming a different card or target]",
    " - a row disappearing re-opens a hold exactly as a row appearing does, and so"
    " does the same action at a CHANGED COST; a row that changes only in its"
    " annotations (a forecast, a clock, a count, a life total) is the SAME row]",
    " - a hold taken here holds until one of them appears, disappears, changes its"
    " COST, or names a different card or target; a row that changes only in its"
    " annotations is the same row]",
)
W80_FOLD_POINTER = " - HOW A HOLD ENDS, below this list, says what counts as the same row]"


def hold_bytes(prompt):
    """(row bytes, hold-check bytes, contract bytes, bytes the w80 bracket fold
    removes). The fold only applies where the contract paragraph is on the same
    prompt - that is the condition the engine itself gates on."""
    row = check = contract = 0
    check_lines = []
    for ln in prompt.split("\n"):
        stripped = ln.lstrip("0123456789. ")
        if stripped.startswith(HOLD_ROW_HEAD):
            row += len(ln) + 1
        elif ln.startswith(HOLD_CHECK_HEAD):
            check += len(ln) + 1
            check_lines.append(ln)
        elif ln.startswith(HOLD_CONTRACT_HEAD):
            contract += len(ln) + 1
    saved = 0
    if contract:
        for ln in check_lines:
            for tail in W80_FOLDED_TAILS:
                if ln.endswith(tail):
                    saved += len(tail) - len(W80_FOLD_POINTER)
                    break
    return row, check, contract, saved


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
    #W80-DG (U16): the hold apparatus, on a mocked prompt carrying all three
    #surfaces plus a bracket whose rule tail the wave-80 fold replaces.
    rowline = "3. " + HOLD_ROW_HEAD + " - and so on"
    checkline = HOLD_CHECK_HEAD + "1 row above is new" + W80_FOLDED_TAILS[0]
    contractline = HOLD_CONTRACT_HEAD + " taking the hold row ...]"
    mock = "\n".join(["Your life: 20", rowline, checkline, contractline, ""])
    r, c, ct, sv = hold_bytes(mock)
    if r != len(rowline) + 1 or c != len(checkline) + 1 or ct != len(contractline) + 1:
        print("SELFTEST FAIL: hold bytes %d %d %d" % (r, c, ct))
        ok = False
    if sv != len(W80_FOLDED_TAILS[0]) - len(W80_FOLD_POINTER) or sv <= 0:
        print("SELFTEST FAIL: hold fold saving %d" % sv)
        ok = False
    #...and with NO contract paragraph on the prompt the fold claims nothing
    if hold_bytes("\n".join([checkline, ""]))[3] != 0:
        print("SELFTEST FAIL: fold claimed a saving with no contract paragraph")
        ok = False
    if hold_bytes("Your life: 20") != (0, 0, 0, 0):
        print("SELFTEST FAIL: a prompt with no hold surfaces reports none")
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
                                           "logtrim": 0,
                                           #W80-DG (U16)
                                           "hrow": 0, "hcheck": 0, "hcontract": 0,
                                           "hsaved": 0})
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
                hr, hc, hct, hsv = hold_bytes(p)  #W80-DG (U16)
                s["hrow"] += hr
                s["hcheck"] += hc
                s["hcontract"] += hct
                s["hsaved"] += hsv
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
    #W80-DG (U16): the hold apparatus per deck, and what the wave-80 bracket fold
    #takes off it. Means are per PROMPT, over every prompt of that deck.
    print("HOLD APPARATUS (U16 - mean bytes per prompt; fold = the wave-80"
          " hold-check bracket fold, applied only where HOW A HOLD ENDS is on the"
          " same prompt)")
    for deck in sorted(per):
        s = per[deck]
        n = max(1, s["n"])
        tot = s["hrow"] + s["hcheck"] + s["hcontract"]
        print("  deck%-4s row %5.0f  check %5.0f  contract %5.0f  TOTAL %6.0f"
              "  -> after fold %6.0f  (fold saves %5.0f B/prompt, %.2f%% of the"
              " mean prompt)"
              % (deck, s["hrow"] / n, s["hcheck"] / n, s["hcontract"] / n, tot / n,
                 (tot - s["hsaved"]) / n, s["hsaved"] / n,
                 100.0 * s["hsaved"] / max(1, s["now"])))
    print("  TOTAL   n=%-5d mean %6d -> %6d   >20KB %4d -> %4d   hoist %7d B"
          " (%d prompts)   log trim %8d B   all prompts %d -> %d B (-%.1f%%)"
          % (tot["n"], tot["now"] // max(1, tot["n"]), tot["then"] // max(1, tot["n"]),
             tot["over20_now"], tot["over20_then"], tot["hoist"], tot["dup"],
             tot["logtrim"], tot["now"], tot["then"],
             100.0 * (tot["now"] - tot["then"]) / max(1, tot["now"])))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
