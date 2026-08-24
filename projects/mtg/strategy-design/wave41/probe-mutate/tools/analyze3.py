import json, glob, re, collections

MEMBER = re.compile(r'\|\| ([^:|]+): ')
UNDER = re.compile(r'\d+ cards? underneath: ([^\]]+)\]')

agree = 0
disagree = []
tags = 0
tag_names = collections.Counter()


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
            tag = ln[i:j + 1]
            head = ln[:i]
            # the permanent's own segment starts after the last "; " before the tag
            seg = head.split('; ')[-1]
            name = re.split(r'\s*\{|\s*\(|\s*\[', seg.strip())[0].strip()
            name = name.split(': ')[-1].strip().split(' #')[0]
            m = UNDER.search(tag)
            if m and name:
                out.append((name, [u.strip() for u in m.group(1).split(',')]))
            idx = j + 1
    return out


for p in sorted(glob.glob('keep/g*/*.jsonl')):
    for l in open(p):
        r = json.loads(l)
        piles = parse_piles(r.get('prompt', ''))
        for top, under in piles:
            tags += 1
            tag_names[(top, tuple(under))] += 1
        for o in (r.get('options_text') or []):
            o = str(o)
            if 'mutate pile - combined abilities of' not in o:
                continue
            members = set(x.strip() for x in MEMBER.findall(o))
            for top, under in piles:
                if o.startswith(top):
                    expect = set([top]) | set(under)
                    if members == expect:
                        agree += 1
                    else:
                        disagree.append((r.get('seq'), sorted(members), sorted(expect)))
                    break

print('battlefield-summary pile tags seen (secondary evidence):', tags)
print('distinct (top, underneath) pile compositions:')
for k, v in tag_names.most_common():
    print('   ', k, 'x', v)
print('option-blob membership == battlefield-tag membership:', agree, 'disagreements:', len(disagree))
for d in disagree[:10]:
    print('   ', d)
