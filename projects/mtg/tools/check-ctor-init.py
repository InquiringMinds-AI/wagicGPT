#!/usr/bin/env python3
"""Guard: a `//` comment inside a constructor initializer list silently deletes
every initializer after it on the same physical line.

Wave 68 defect (#W68-BE R1): `//#W68-BA (J3/J6)` landed at column 379 of
AIPlayerGPT.cpp's ctor initializer list and commented out the 30 initializers
that followed it on that line (mAttacksDoneTurn .. mRecoveryExecRow) - every
GPT seat then constructed with indeterminate control-flow state and pointers.
A trailing `//` comment at the END of an init-list line is legal and common in
this codebase, so a blanket "no // in the init list" test would be all noise.

The exact test used here instead: strip `//` tails from the initializer list and
compare the set of `name(` initializers BEFORE and AFTER stripping. Any name
that disappears was eaten by a comment. Zero false positives by construction.

Exit 0 = clean, 1 = at least one eaten initializer (message names it).
Usage: check-ctor-init.py [file-or-dir ...]   (default: src/)
"""
import os, re, sys

INIT_RE = re.compile(r'\b([A-Za-z_]\w*)\s*\(')
CTOR_RE = re.compile(r'^\s*(\w+)::(~?\w+)\s*\(')

def strip_line_comment(line):
    # naive but sufficient for initializer lists: no // inside string literals
    # occurs in any ctor init list in this tree (checked), and a stray one would
    # only ever ADD a name to the stripped set, never remove one.
    i = line.find('//')
    return line if i < 0 else line[:i]

def scan(path):
    try:
        raw = open(path, 'rb').read().decode('cp1252', 'replace')
    except Exception as e:
        print("check-ctor-init: cannot read %s: %s" % (path, e))
        return 0
    lines = raw.split('\n')
    bad = 0
    for i, line in enumerate(lines):
        m = CTOR_RE.match(line)
        if not m or m.group(2).startswith('~'):
            continue
        # walk forward to the init-list ':' and then to the body '{'
        j = i
        found_colon = False
        chunk = []
        while j < len(lines) and j < i + 200:
            s = lines[j]
            if s.strip() == '{' or s.rstrip().endswith('{'):
                if found_colon:
                    chunk.append(s)
                break
            if found_colon:
                chunk.append(s)
            elif re.search(r'(^\s*:\s)|(\)\s*:\s)', s):
                found_colon = True
                chunk.append(s)
            j += 1
        if not found_colon or not chunk:
            continue
        for k, s in enumerate(chunk):
            raw_names = set(INIT_RE.findall(s))
            cut_names = set(INIT_RE.findall(strip_line_comment(s)))
            # A comment tail may legitimately mention a name with a paren
            # ("//#W67-AX (I7)"), so only member initializers count: this tree's
            # members are all `m<Upper>...`, and a base-class initializer is the
            # class's own base name. Nothing else can be an eaten initializer.
            eaten = sorted(n for n in (raw_names - cut_names)
                           if re.match(r'^m[A-Z]\w*$', n) or n.startswith('AIPlayer')
                           or n == m.group(1))
            if eaten:
                print("%s:%d: initializer(s) commented out inside the ctor "
                      "initializer list of %s::%s -> %s"
                      % (path, i + 1 + k, m.group(1), m.group(2), ", ".join(eaten)))
                bad += 1
    return bad

def main():
    targets = sys.argv[1:] or ['src']
    files = []
    for t in targets:
        if os.path.isdir(t):
            for root, _, names in os.walk(t):
                files += [os.path.join(root, n) for n in names if n.endswith('.cpp')]
        else:
            files.append(t)
    bad = sum(scan(f) for f in sorted(files))
    if bad:
        print("check-ctor-init: FAILED (%d eaten initializer line(s))" % bad)
        return 1
    print("check-ctor-init: OK (%d file(s))" % len(files))
    return 0

if __name__ == '__main__':
    sys.exit(main())
