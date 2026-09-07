#!/usr/bin/env python3
"""#W70-BL (E6): the reply protocol says the reply is TWO labelled lines - a PLAN:
line of intended actions, then the action line for this window - and nothing else.
The owner's rule behind it: the model cannot be told to write nonsense or it will
write nonsense. So no string this seat SHOWS the model may license prose in the
reply, ask for the answer before the plan, or make the plan conditional.

Every wave from 66 to 69 re-litigated that in text (a three-sentence budget for
"your working", a correction grammar, an answer-first re-ask) because the corpora
ran with thinking OFF and the reply was the only place to think. This guard makes
the regression loud at link time instead of one corpus later.

WHAT IT SCANS: production statements in src/AIPlayerGPT.cpp (everything BEFORE the
`#if defined(WAGIC_GPT_PARSETEST_BUILD)` corpus, which legitimately quotes the
deleted wordings as MUST-NOT-MATCH cases) whose concatenated string literals talk
about the reply form - i.e. carry a reply LABEL (CHOICE:/ATTACK:/BLOCKS:/PUT:/
PLAN:). Comments are stripped first, so a comment may still name what was deleted.

Usage: check-reply-instructions.py [src-dir-or-file ...]   (default: src/)
Exit 1 on any hit, naming the line and the phrase.
"""
import os
import re
import sys

FORBIDDEN = ["working", "reasoning", "first line", "correction", "announce",
             "then a plan", "only if"]
LABELS = ("CHOICE:", "ATTACK:", "BLOCKS:", "PUT:", "PLAN:")
# Identifier-shaped tokens (translog classes, parse notes, json keys) are DATA the
# corpus reader consumes, never text the model is shown. They are excused by shape,
# not by name: a stamp has no spaces.
IDENT = re.compile(r"^[A-Za-z0-9_.]+$")


def strip_comments(src):
    out, i, n = [], 0, len(src)
    while i < n:
        c = src[i]
        if c == '"':
            out.append(c)
            i += 1
            while i < n:
                out.append(src[i])
                if src[i] == '\\':
                    if i + 1 < n:
                        out.append(src[i + 1])
                        i += 2
                        continue
                elif src[i] == '"':
                    i += 1
                    break
                i += 1
            continue
        if c == "'" and i + 2 < n:
            j = i + 1
            out.append(c)
            while j < n and src[j] != "'":
                if src[j] == '\\':
                    j += 1
                j += 1
            out.append(src[i + 1:j + 1])
            i = j + 1
            continue
        if src.startswith("//", i):
            j = src.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
            continue
        if src.startswith("/*", i):
            j = src.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append("".join(ch if ch == "\n" else " " for ch in src[i:j]))
            i = j
            continue
        out.append(c)
        i += 1
    return "".join(out)


LIT = re.compile(r'"((?:[^"\\]|\\.)*)"')


def check(path):
    raw = open(path, "r", encoding="utf-8", errors="replace").read()
    cut = raw.find("#if defined(WAGIC_GPT_PARSETEST_BUILD)")
    if cut >= 0:
        raw = raw[:cut]
    src = strip_comments(raw)
    bad = []
    start = 0
    # Statement boundaries are semicolons OUTSIDE string literals - the tails
    # themselves contain semicolons, and splitting inside one hid four of the
    # seven seams from an earlier draft of this guard.
    masked = LIT.sub(lambda m: '"' + ("." * len(m.group(1))) + '"', src)
    for stmt_end in [m.end() for m in re.finditer(r";", masked)]:
        stmt = src[start:stmt_end]
        start = stmt_end
        lits = [m.group(1) for m in LIT.finditer(stmt)]
        lits = [l for l in lits if not IDENT.match(l)]
        if not lits:
            continue
        joined = " ".join(lits)
        if not any(lab in joined for lab in LABELS):
            continue
        low = joined.lower()
        for phrase in FORBIDDEN:
            if phrase in low:
                line = raw.count("\n", 0, stmt_end) + 1
                bad.append((path, line, phrase, joined[:120]))
    return bad


def main(argv):
    targets = argv[1:] or ["src"]
    files = []
    for t in targets:
        if os.path.isdir(t):
            for root, _, names in os.walk(t):
                files += [os.path.join(root, f) for f in names
                          if f == "AIPlayerGPT.cpp"]
        else:
            files.append(t)
    bad = []
    for f in files:
        bad += check(f)
    for path, line, phrase, snip in bad:
        print("check-reply-instructions: %s:%d licenses %r in an instruction string: %s"
              % (path, line, phrase, snip))
    if bad:
        print("check-reply-instructions: FAILED (%d instruction string(s))" % len(bad))
        return 1
    print("check-reply-instructions: OK (%d file(s))" % len(files))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
