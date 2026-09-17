#!/usr/bin/env python3
"""enum-addtogame.py <projects/mtg root> [--all] - enumerate every addToGame() call site under src/ and
include/ and classify its RECEIVER (#W87-JB, audit-2026-09 bug list item 12). Classes: FRESH-* (the receiver
was allocated in the same function: NEW / clone() / factory parse - by construction never in the garbage),
BASE-CHAIN (a qualified X::addToGame() inside an override - reduces to the override's caller), OTHER (an
existing pointer or `this` - READ each one). Declarations, definitions and comments are excluded."""
import re, sys, os, glob, collections
root = sys.argv[1]
files = sorted(glob.glob(os.path.join(root, 'src', '*.cpp')) + glob.glob(os.path.join(root, 'include', '*.h')))
pat = re.compile(r'addToGame\s*\(\s*\)')
classes = collections.OrderedDict()
def put(cls, f, ln, text):
    classes.setdefault(cls, []).append((os.path.relpath(f, root), ln, text.strip()))
for f in files:
    try:
        lines = open(f, 'rb').read().decode('cp1252', errors='replace').split('\n')
    except Exception as e:
        print('skip', f, e); continue
    for i, line in enumerate(lines):
        if 'addToGame' not in line: continue
        # skip declarations / definitions / comments
        s = line.strip()
        if re.match(r'^\s*(virtual\s+)?int\s+[\w:]*addToGame\s*\(', line): continue
        if s.startswith('//') or s.startswith('*') or s.startswith('/*'): continue
        for m in pat.finditer(line):
            pre = line[:m.start()]
            # receiver expression: text before 'addToGame' up to the operator
            r = re.search(r'([\w\)\]]+(?:\s*(?:->|\.|::)\s*[\w\(\)]+)*)\s*(->|\.|::)\s*$', pre)
            recv = r.group(1) if r else ''
            op = r.group(2) if r else ''
            cls = None
            if op == '::':
                cls = 'BASE-CHAIN (qualified call inside an override)'
            elif not r or recv == '' :
                cls = 'THIS (unqualified addToGame() inside a member)'
            elif recv == 'this':
                cls = 'THIS (this->addToGame())'
            elif 'NEW ' in pre and re.search(r'NEW\s+\w+\s*\([^;]*\)\s*\)?\s*->\s*$', pre):
                cls = 'FRESH-INLINE (NEW X(...)->addToGame())'
            elif re.search(r'clone\s*\(\s*\)\s*->\s*$', pre):
                cls = 'FRESH-CLONE (...->clone()->addToGame())'
            else:
                var = re.match(r'[\w\]\)]+', recv).group(0) if re.match(r'[\w\]\)]+', recv) else recv
                var = re.sub(r'\W.*$', '', recv)
                # look back for the variable's origin within 25 lines
                origin = None
                for j in range(i, max(-1, i-25), -1):
                    L = lines[j]
                    if re.search(r'\b' + re.escape(var) + r'\s*=\s*NEW\b', L) or re.search(r'\*\s*' + re.escape(var) + r'\s*=\s*NEW\b', L):
                        origin = 'NEW'; break
                    if re.search(r'\b' + re.escape(var) + r'\s*=\s*[^;]*clone\s*\(', L):
                        origin = 'clone'; break
                    if re.search(r'\b' + re.escape(var) + r'\s*=\s*[^;]*parseMagicLine\s*\(', L) or re.search(r'\b' + re.escape(var) + r'\s*=\s*af\.\w+\(', L):
                        origin = 'factory'; break
                    if re.search(r'\b' + re.escape(var) + r'\s*=\s*[^;]*getAbility\w*\(', L):
                        origin = 'factory'; break
                if origin == 'NEW': cls = 'FRESH-VAR (var = NEW ... ; var->addToGame())'
                elif origin == 'clone': cls = 'FRESH-VAR (var = ...clone(); var->addToGame())'
                elif origin == 'factory': cls = 'FRESH-VAR (var = factory-built; var->addToGame())'
                else: cls = 'OTHER (receiver not shown fresh within 25 lines: READ)'
            put(cls, f, i+1, line)
total = sum(len(v) for v in classes.values())
print('TOTAL call sites:', total)
for cls, sites in classes.items():
    print(f'\n== {cls}: {len(sites)}')
    if cls.startswith('OTHER') or cls.startswith('THIS') or '--all' in sys.argv:
        for f, ln, t in sites:
            print(f'   {f}:{ln}: {t[:110]}')
