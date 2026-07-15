#!/usr/bin/env python3
"""Offline intent-collapse study over selfplay translogs.

For each ask/priority record: parse the reply head (current parser semantics),
find any LAST bare integer (arm A), detect non-numeric heads (arm C), and
detect PLAN self-reversal language + a PLAN-concluded option that differs from
the recorded choice (via option-text mention in the plan's final sentences).
"""
import json, re, sys, glob, os

def strip_think(t):
    i = t.rfind('</think>')
    return t[i+8:] if i >= 0 else t

def head_int(text, nopts):
    m = re.match(r'\s*(\d+)', text)
    if m:
        n = int(m.group(1))
        if 0 <= n <= nopts:
            return n
    return None

def first_int(text, nopts):
    for m in re.finditer(r'\d+', text):
        n = int(m.group(0))
        if 0 <= n <= nopts:
            return n
    return None

def last_bare_int(text, nopts):
    # "bare" = an integer on its own line, or after CHOICE:, or the final token
    cands = []
    for line in text.splitlines():
        s = line.strip()
        m = re.fullmatch(r'(\d+)[.)]?', s)
        if m:
            cands.append(int(m.group(1)))
    if cands:
        for n in reversed(cands):
            if 0 <= n <= nopts:
                return n
    return None

REVERSAL = re.compile(r'\b(wait|actually|however|instead|no,|must mulligan|i must|correction|on second thought|but the guide|therefore,? i (should|must|will))\b', re.I)

def plan_concluded_option(plan, options):
    """Which option does the plan's final 2 sentences name? Crude: look for
    option text words (first 4 significant words of each option) in the tail."""
    tail = ' '.join(re.split(r'(?<=[.!?])\s+', plan.strip())[-2:]).lower()
    hits = []
    for i, opt in enumerate(options):
        # significant words of the option text
        words = [w for w in re.findall(r'[a-z]{4,}', opt.lower())
                 if w not in ('cast','with','play','targeting','your','this','that','option','nothing','none')]
        if not words:
            # decline-ish option: match 'nothing'/'decline'/'pass'/'mulligan'/'keep'
            base = opt.lower()
            for kw in ('nothing','mulligan','keep','pass','done','hold'):
                if kw in base and re.search(r'\b'+kw+r'\b', tail):
                    hits.append(i); break
            continue
        matched = sum(1 for w in words[:5] if w in tail)
        if matched >= min(2, len(words[:5])):
            hits.append(i)
    return hits

def main(dirs):
    total = ask = 0
    nonnum_head = 0
    lastbare_differs = 0
    reversal_and_mismatch = []
    nonnum_examples = []
    for d in dirs:
        for f in glob.glob(os.path.join(d, '*.jsonl')):
            for line in open(f, errors='replace'):
                try:
                    r = json.loads(line)
                except Exception:
                    continue
                if r.get('kind') not in ('ask', 'priority'):
                    continue
                total += 1
                reply = strip_think(r.get('reply') or '')
                opts = r.get('options_text') or r.get('options') or []
                if isinstance(opts, int):
                    opts = []
                nopts = len(opts) if opts else 30
                choice = r.get('choice')
                h = head_int(reply, nopts)
                if h is None:
                    nonnum_head += 1
                    if len(nonnum_examples) < 8:
                        nonnum_examples.append((os.path.basename(f), r.get('seq'), reply[:120]))
                # split plan
                pidx = reply.find('PLAN:')
                plan = reply[pidx+5:] if pidx >= 0 else ''
                lb = last_bare_int(reply, nopts)
                if h is not None and lb is not None and lb != h:
                    lastbare_differs += 1
                if plan and opts and REVERSAL.search(plan):
                    concl = plan_concluded_option(plan, opts)
                    if concl and choice is not None and choice not in concl:
                        reversal_and_mismatch.append((os.path.basename(f), r.get('seq'), choice, concl, plan[-160:].replace('\n',' ')))
    print(f'total ask/priority records: {total}')
    print(f'non-numeric heads: {nonnum_head}')
    print(f'last-bare-int differs from head: {lastbare_differs}')
    print(f'reversal-language + concluded-option != recorded choice: {len(reversal_and_mismatch)}')
    for x in reversal_and_mismatch[:20]:
        print('  MISMATCH', x[0], 'seq', x[1], 'choice', x[2], 'concluded', x[3])
        print('    ...', x[4])
    for x in nonnum_examples:
        print('  NONNUM', x[0], 'seq', x[1], repr(x[2]))

if __name__ == '__main__':
    main(sys.argv[1:])
