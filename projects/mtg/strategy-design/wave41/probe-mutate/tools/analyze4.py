"""W41-4 probe metric: over every option/target line that names a card which the
same prompt's battlefield render marks as a MUTATE PILE, does the quoted text
name/concatenate every card in that pile (post-fix shape) or quote one member's
text (the W41-4 defect shape)?  Plus a per-card attribution check against the
engine's own primitives."""
import json, glob, re, collections

PRIM = {}
for path in ('/home/magi/Projects/wagicGPT/.claude/worktrees/agent-a7a8a007a847a0d98/projects/mtg/bin/Res/sets/primitives/mtg.txt',
             '/home/magi/Projects/wagicGPT/.claude/worktrees/agent-a7a8a007a847a0d98/projects/mtg/bin/Res/sets/primitives/borderline.txt'):
    name = None
    for ln in open(path, encoding='latin-1'):
        ln = ln.rstrip('\n')
        if ln.startswith('name='):
            name = ln[5:].strip()
        elif ln.startswith('text=') and name and name not in PRIM:
            PRIM[name] = ln[5:].strip()

MEMBER = re.compile(r'\|\| ([^:|]+): (.*?)(?= \|\| |$)')
UNDER = re.compile(r'\d+ cards? underneath: ([^\]]+)\]')


def parse_piles(prompt):
    out = []
    for ln in prompt.splitlines():
        if 'battlefield' not in ln or '[mutated pile' not in ln:
            continue
        idx = 0
        while True:
            i = ln.find('[mutated pile', idx)
            if i < 0:
                break
            j = ln.find(']', i)
            seg = ln[:i].split('; ')[-1]
            name = re.split(r'\s*\{|\s*\(|\s*\[', seg.strip())[0].strip()
            name = name.split(': ')[-1].strip().split(' #')[0]
            m = UNDER.search(ln[i:j + 1])
            if m and name:
                out.append((name, [u.strip() for u in m.group(1).split(',')]))
            idx = j + 1
    return out


tot = covered = 0
misses, attr_bad = [], []
attr_ok = 0
games = set()
depth = collections.Counter()

for p in sorted(glob.glob('keep/g*/*.jsonl')):
    g = p.split('/')[-2]
    for l in open(p):
        r = json.loads(l)
        piles = parse_piles(r.get('prompt', ''))
        if not piles:
            continue
        for o in (r.get('options_text') or []):
            o = str(o)
            if ' - "' not in o:
                continue
            for top, under in piles:
                if not o.startswith(top):
                    continue
                tot += 1
                games.add(g)
                if 'mutate pile - combined abilities of' in o:
                    covered += 1
                    depth[len(under) + 1] += 1
                    for nm, tx in MEMBER.findall(o):
                        nm = nm.strip()
                        head = tx.strip().rstrip('"').split('...')[0].strip()
                        real = PRIM.get(nm, '')
                        if real and head and real.startswith(head[:40]):
                            attr_ok += 1
                        else:
                            attr_bad.append((g, r.get('seq'), nm, head[:60], real[:60]))
                else:
                    misses.append((g, r.get('seq'), o[:170]))
                break

print('option/target lines naming a card the board render marks as a pile: %d' % tot)
print('  post-fix shape (per-card attributed concatenation): %d' % covered)
print('  W41-4 defect shape (one member text, possibly under another name): %d' % len(misses))
print('  COVERAGE FRACTION: %d/%d' % (covered, tot))
print('  pile depths: %s   games contributing: %d' % (dict(depth), len(games)))
print('per-card attributions verified against primitives: ok=%d bad=%d' % (attr_ok, len(attr_bad)))
for b in attr_bad[:10]:
    print('  BAD', b)
for m in misses[:10]:
    print('  MISS', m)
