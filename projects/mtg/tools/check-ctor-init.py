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


# ---------------------------------------------------------------------------
# #W80-DH (F1, Astra wave-80 review finding 1 - HIGH). THE GUARD PASSED WHILE
# NINE INITIALIZERS WERE MISSING.
#
# The wave-80 merge helper's auto-resolve took one side of the ctor-initializer
# hunk twice and DROPPED nine members from the list - lanes DE's seven counters
# and DF's two. Every GPT seat then constructed with indeterminate integers, and
# `check-ctor-init.py` reported OK, because the test above detects an initializer
# EATEN BY A COMMENT and nothing else: a member that was never written at all
# leaves no trace on the line for it to find.
#
# So the second test asks the header: every SCALAR member a class declares must
# be initialised. "Initialised" is either an entry in the ctor's initializer list
# or an assignment in the ctor BODY - the body is derived from the source rather
# than kept as a hand-maintained whitelist, because a whitelist is a file someone
# has to remember to update and is wrong the first time it is not. A member the
# ctor neither lists nor assigns is read before it is written or it is not read
# at all, and the compiler cannot tell you which.
#
# Scalars only. A std::string, vector, map or set default-constructs; a raw
# POINTER is deliberately NOT covered here (pointer members in this tree are set
# from ctor arguments and engine wiring in several branches, and the false
# positives would drown the signal) - the defect class this exists for is the
# integer/bool counter, which is exactly what the merge dropped.
SCALAR = (r'(?:bool|char|short|int|long|unsigned|signed|float|double|size_t|'
          r'time_t|int8_t|int16_t|int32_t|int64_t|uint8_t|uint16_t|uint32_t|uint64_t)')
MEMBER_DECL = re.compile(r'^\s*((?:mutable\s+)?(?:unsigned\s+|signed\s+|long\s+|short\s+)*'
                         + SCALAR + r')\s+([^;{}()=]+);')
MEMBER_NAME = re.compile(r'^m[A-Z]\w*$')


def header_scalar_members(header_path, class_name):
    """Scalar (non-static, non-array, non-pointer) members `class_name` declares."""
    try:
        raw = open(header_path, 'rb').read().decode('cp1252', 'replace')
    except Exception:
        return None
    at = raw.find('class ' + class_name)
    if at < 0:
        return None
    body = raw[at:]
    out = []
    for line in body.split('\n'):
        if 'static' in line or 'typedef' in line:
            continue
        m = MEMBER_DECL.match(line)
        if not m:
            continue
        decl = m.group(2)
        if '*' in decl or '[' in decl or '&' in decl:
            continue
        for part in decl.split(','):
            name = part.strip()
            if MEMBER_NAME.match(name) and name not in out:
                out.append(name)
    return out


def ctor_init_and_body(raw, class_name):
    """(names in the initializer list, ctor body text) for `class_name`'s own ctor."""
    sig = class_name + '::' + class_name
    at = raw.find(sig)
    while at >= 0:
        # the definition, not a declaration or a nested mention
        open_paren = raw.find('(', at)
        brace = raw.find('\n{', at)
        if brace < 0:
            return None, None
        head = raw[at:brace]
        if open_paren >= 0 and ';' not in head:
            names = set(re.findall(r'\b(m[A-Z]\w*)\s*\(', head))
            depth, end = 0, len(raw)
            for i in range(brace + 1, len(raw)):
                if raw[i] == '{':
                    depth += 1
                elif raw[i] == '}':
                    depth -= 1
                    if depth == 0:
                        end = i
                        break
            return names, raw[brace + 1:end]
        at = raw.find(sig, at + 1)
    return None, None


# Which classes the MISSING check is a BUILD GATE for. The check finds 55
# pre-existing uninitialised scalars across the engine's UI and harness classes
# (SimpleButton, SimplePad, TextScroller, TestSuiteAI, ...) - real debt, and not
# this guard's to fix in the wave that adds it. Their COUNT is printed on every
# build (and `--notes` lists them), so the debt is visible and a class can be
# adopted the day it is cleaned; an enforced class FAILS the build. AIPlayerGPT is enforced because it is the
# class the wave-80 merge damaged, the class every wave edits, and the class whose
# counters are read before they are written.
ENFORCED_CLASSES = ('AIPlayerGPT',)


def scan_missing(path, header_dir, enforced_only=True):
    try:
        raw = open(path, 'rb').read().decode('cp1252', 'replace')
    except Exception:
        return 0
    bad = 0
    for class_name in sorted(set(re.findall(r'^(\w+)::\1\s*\(', raw, re.M))):
        if enforced_only and class_name not in ENFORCED_CLASSES:
            continue
        header = os.path.join(header_dir, class_name + '.h')
        if not os.path.isfile(header):
            continue
        members = header_scalar_members(header, class_name)
        if not members:
            continue
        init, body = ctor_init_and_body(raw, class_name)
        if init is None:
            continue
        for name in members:
            if name in init:
                continue
            if re.search(r'\b' + name + r'\s*=[^=]', body):
                continue  # the ctor BODY sets it: initialised, just not in the list
            print("%s: %s::%s leaves scalar member `%s` UNINITIALISED - it is in "
                  "%s.h, absent from the initializer list, and never assigned in "
                  "the ctor body" % (path, class_name, class_name, name, class_name))
            bad += 1
    return bad


FIXTURE_H = """class CtorFixture
{
public:
    CtorFixture();
private:
    int mListed;
    int mBodySet;
    bool mDropped;
    std::string mNotScalar;
    int * mPointer;
    int mArray[4];
    static int mStatic;
};
"""

FIXTURE_CPP = """#include "CtorFixture.h"

CtorFixture::CtorFixture()
    : mListed(0)
{
    mBodySet = 7;
}
"""


def selftest():
    import tempfile, shutil
    d = tempfile.mkdtemp(prefix='ctor-init-selftest-')
    try:
        inc = os.path.join(d, 'include')
        src = os.path.join(d, 'src')
        os.makedirs(inc)
        os.makedirs(src)
        open(os.path.join(inc, 'CtorFixture.h'), 'w').write(FIXTURE_H)
        cpp = os.path.join(src, 'CtorFixture.cpp')
        open(cpp, 'w').write(FIXTURE_CPP)
        members = header_scalar_members(os.path.join(inc, 'CtorFixture.h'), 'CtorFixture')
        ok = True
        if members != ['mListed', 'mBodySet', 'mDropped']:
            print("check-ctor-init selftest: header scan should see the three SCALARS and "
                  "neither the string, the pointer, the array nor the static -> %r" % (members,))
            ok = False
        import io as _io, contextlib
        buf = _io.StringIO()
        with contextlib.redirect_stdout(buf):
            bad = scan_missing(cpp, inc, False)
        out = buf.getvalue()
        if bad != 1 or 'mDropped' not in out:
            print("check-ctor-init selftest: the DROPPED member must be the one and only "
                  "finding (bad=%d): %s" % (bad, out.strip()))
            ok = False
        if 'mListed' in out or 'mBodySet' in out:
            print("check-ctor-init selftest: a listed member and a ctor-body-assigned "
                  "member are both initialised and must NOT be reported: %s" % out.strip())
            ok = False
        # and the fixture with the member restored is clean
        open(os.path.join(inc, 'CtorFixture.h'), 'w').write(
            FIXTURE_H.replace('    bool mDropped;\n', ''))
        buf = _io.StringIO()
        with contextlib.redirect_stdout(buf):
            bad = scan_missing(cpp, inc, False)
        if bad != 0:
            print("check-ctor-init selftest: the repaired fixture must be clean: %s"
                  % buf.getvalue().strip())
            ok = False
        print("check-ctor-init selftest: %s" % ("OK" if ok else "FAILED"))
        return 0 if ok else 1
    finally:
        shutil.rmtree(d, ignore_errors=True)


def main():
    args = sys.argv[1:]
    if '--selftest' in args:
        return selftest()
    targets = [a for a in args if not a.startswith('--')] or ['src']
    files = []
    for t in targets:
        if os.path.isdir(t):
            for root, _, names in os.walk(t):
                files += [os.path.join(root, n) for n in names if n.endswith('.cpp')]
        else:
            files.append(t)
    bad = sum(scan(f) for f in sorted(files))
    #W80-DH (F1): ...and the MISSING-initializer half. The header for `X::X` is
    #looked for beside the source tree it was found in (src/ -> ../include/); a
    #class with no such header is skipped rather than guessed at.
    missing = 0
    unenforced = 0
    for f in sorted(files):
        header_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(f))),
                                  'include')
        if os.path.isdir(header_dir):
            missing += scan_missing(f, header_dir)
            if '--notes' in sys.argv:
                unenforced += scan_missing(f, header_dir, False)
            else:
                import io as _io, contextlib
                _b = _io.StringIO()
                with contextlib.redirect_stdout(_b):
                    unenforced += scan_missing(f, header_dir, False)
    if bad or missing:
        print("check-ctor-init: FAILED (%d eaten initializer line(s), %d uninitialised "
              "scalar member(s))" % (bad, missing))
        return 1
    note = ""
    if unenforced > missing:
        note = ("; %d uninitialised scalar(s) outside the enforced classes %s - "
                "pre-existing debt, run with --notes to list them"
                % (unenforced - missing, ", ".join(ENFORCED_CLASSES)))
    print("check-ctor-init: OK (%d file(s))%s" % (len(files), note))
    return 0

if __name__ == '__main__':
    sys.exit(main())
