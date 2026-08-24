"""Probe-2 metric: deck152's two wave-40 guide edits, adjudicated against the
synthesis predictions (a)/(b)/(c)."""
import json, glob, re, sys, collections

PT = re.compile(r'\(([0-9]+)/([0-9]+)\)')
FORECAST = re.compile(r'you would be at (-?\d+)')
LETHAL = re.compile(r'\bIS lethal\b|- LETHAL', re.I)
BLINE = re.compile(r'^(B\d+)\.\s+(.*?)\s+-\s+may block (.*)$')
ALINE = re.compile(r'^(A\d+)\.\s+(.*)$')
ASSIGN = re.compile(r'(B\d+)\s*:\s*(A\d+|none)', re.I)
DRIFT = re.compile(r'but actually|still stops|either way|preserve .* for future|let the attackers through', re.I)

files = sorted(glob.glob(sys.argv[1] if len(sys.argv) > 1 else 'keep2/*/*deck152-*.jsonl'))

armed_sole = []
armed_multi = []
block_windows = []
free_damage = 0
drift_hits = []
tot = collections.Counter()

for p in files:
    g = p.split('/')[-2]
    for l in open(p):
        r = json.loads(l)
        k = r.get('kind')
        tot[k] += 1
        prompt = r.get('prompt', '')
        opts = [str(o) for o in (r.get('options_text') or [])]
        reply = str(r.get('reply') or '')
        if 'free damage' in reply.lower():
            free_damage += 1
        if k == 'attackers':
            # biggest opposing creature power off the Opponent battlefield line
            big = 0
            for ln in prompt.splitlines():
                if not ln.startswith('Opponent battlefield'):
                    continue
                for seg in ln.split(';'):
                    if 'creature' in seg.lower() or PT.search(seg):
                        m = PT.search(seg)
                        if m:
                            pw = int(m.group(1))
                            if '+1/+1' in seg:
                                pw += 1
                            big = max(big, pw)
            life = r.get('my_life')
            n_att = len([o for o in opts if ALINE.match(o)])
            armed = life is not None and big and life <= big
            rec = dict(game=g, seq=r.get('seq'), turn=r.get('turn'), life=life, big=big,
                       n=n_att, chosen=r.get('chosen_text'), reply=reply[:400])
            if armed and n_att == 1:
                armed_sole.append(rec)
            elif armed and n_att >= 2:
                armed_multi.append(rec)
        elif k == 'blockers':
            m = FORECAST.search(prompt)
            fc = int(m.group(1)) if m else None
            lethal = bool(re.search(r'\bIS lethal\b', prompt))
            rung3 = {}
            for o in opts:
                mm = BLINE.match(o)
                if mm:
                    for att, res in re.findall(r'(A\d+) \(([^)]*)\)', mm.group(3)):
                        rung3.setdefault(mm.group(1), {})[att] = res
            chosen = str(r.get('chosen_text') or '')
            assigns = ASSIGN.findall(chosen)
            if not assigns and ' blocks ' in chosen:
                # the consumed answer is rendered as prose: "<Blocker> blocks <Attacker>"
                bname, aname = {}, {}
                for o in opts:
                    mm = BLINE.match(o)
                    if mm:
                        bname[re.split(r'\s*\(', mm.group(2))[0].strip()] = mm.group(1)
                    ma = ALINE.match(o)
                    if ma:
                        aname[re.split(r'\s*\(', ma.group(2))[0].strip()] = ma.group(1)
                for b in bname:
                    for a in aname:
                        if ('%s blocks %s' % (b, a)) in chosen:
                            assigns.append((bname[b], aname[a]))
            taken3 = []
            taken_any = 0
            for b, a in assigns:
                if a.lower() == 'none':
                    continue
                taken_any += 1
                res = rung3.get(b, {}).get(a, '')
                if 'your blocker dies, attacker lives' in res:
                    taken3.append((b, a))
            offered3 = any('your blocker dies, attacker lives' in v
                           for d in rung3.values() for v in d.values())
            offered_any = bool(rung3)
            block_windows.append(dict(game=g, seq=r.get('seq'), turn=r.get('turn'),
                                      life=r.get('my_life'), forecast=fc, lethal=lethal,
                                      chosen=chosen, offered_any=offered_any,
                                      offered_rung3=offered3, taken_any=taken_any,
                                      taken_rung3=taken3, reply=reply[:400]))
            if DRIFT.search(reply):
                drift_hits.append((g, r.get('seq'), DRIFT.search(reply).group(0), reply[:200]))

print('records by kind:', dict(tot))
print()
print('=== (a) ARMED SOLE-ATTACKER windows:', len(armed_sole))
for w in armed_sole:
    ok = 'ATTACK: none' if w['chosen'] in ('no attackers', None) else w['chosen']
    print('  %s s%s t%s life=%s bigOppPower=%s -> %r' % (w['game'], w['seq'], w['turn'], w['life'], w['big'], w['chosen']))
print('  armed MULTI-attacker windows (context):', len(armed_multi))
for w in armed_multi:
    print('    %s s%s t%s life=%s big=%s n=%s -> %r' % (w['game'], w['seq'], w['turn'], w['life'], w['big'], w['n'], w['chosen']))
print('  replies containing "free damage" (any kind):', free_damage)
print()
print('=== (b) BLOCKER windows:', len(block_windows))
hi = [w for w in block_windows if w['forecast'] is not None and w['forecast'] >= 10]
lo = [w for w in block_windows if w['forecast'] is not None and w['forecast'] <= 9]
print('  forecast >= 10:', len(hi), ' of which rung-3 offered:', sum(1 for w in hi if w['offered_rung3']),
      ' rung-3 ASSIGNMENTS taken:', sum(len(w['taken_rung3']) for w in hi))
print('  forecast <= 9 :', len(lo), ' of which any block offered:', sum(1 for w in lo if w['offered_any']),
      ' windows with a block taken:', sum(1 for w in lo if w['taken_any']))
for w in block_windows:
    print('  %s s%s t%s life=%s N=%s lethal=%s offered3=%s taken=%r rung3taken=%s' %
          (w['game'], w['seq'], w['turn'], w['life'], w['forecast'], w['lethal'],
           w['offered_rung3'], w['chosen'], w['taken_rung3']))
print()
print('=== (c) trade-drift hits:', len(drift_hits))
for d in drift_hits:
    print('  ', d)
