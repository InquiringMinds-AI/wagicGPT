#!/usr/bin/env python3
"""Sweep primitives card text= against current Scryfall Oracle text.

Buckets:
  MATCH      - normalized-identical (reword/whitespace only) -> no action
  DIFF       - normalized texts differ -> needs functional-vs-reword judgment
  NO_TEXT    - primitives entry has no text= line
  NOT_FOUND  - card name not on Scryfall (custom/token/misnamed)
Outputs TSVs into projects/mtg/strategy-design/oracle-sweep/.
"""
import json, re, sys, unicodedata, os
from collections import OrderedDict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PRIM = os.path.join(ROOT, 'projects/mtg/bin/Res/sets/primitives')
OUT  = os.path.join(ROOT, 'projects/mtg/strategy-design/oracle-sweep')
BULK = None
for f in os.listdir(os.path.join(OUT,'data')):
    if f.startswith('oracle-cards-') and f.endswith('.json'):
        BULK = os.path.join(OUT,'data',f)
assert BULK, 'no bulk file'

FILES = ['mtg.txt','borderline.txt','planeswalkers.txt','unsupported.txt']

def parse_primitives():
    cards = OrderedDict()  # name -> dict(text, entries=[(file,card_start_off)])
    for fn in FILES:
        data = open(os.path.join(PRIM,fn),'rb').read()
        pos = 0
        while True:
            s = data.find(b'[card]', pos)
            if s < 0: break
            e = data.find(b'[/card]', s)
            if e < 0: break
            blk = data[s:e].decode('latin-1')
            pos = e + 7
            m = re.search(r'^name=(.*)$', blk, re.M)
            if not m: continue
            name = m.group(1).strip()
            tm = re.findall(r'^text=(.*)$', blk, re.M)
            text = ' -- '.join(t.strip() for t in tm) if tm else ''
            key = name.lower()
            if key in cards:
                cards[key]['entries'].append((fn, s))
                if text and not cards[key]['text']:
                    cards[key]['text'] = text
            else:
                cards[key] = {'name': name, 'text': text, 'entries': [(fn,s)]}
    return cards

def load_oracle():
    d = json.load(open(BULK))
    ix = {}
    for c in d:
        if c.get('layout') in ('token','double_faced_token','emblem','art_series'): continue
        if c.get('oracle_text') is not None:
            texts = {c['name']: c['oracle_text']}
        else:
            texts = {}
            faces = c.get('card_faces',[])
            full = ' -- '.join(f.get('oracle_text','') for f in faces)
            texts[c['name']] = full
            for f in faces:
                texts.setdefault(f['name'], f.get('oracle_text',''))
        for n,t in texts.items():
            ix.setdefault(n.lower(), (n,t))
    return ix

RENAMES = [
    (r'converted mana cost', 'mana value'),
    (r'enters the battlefield', 'enters'),
    (r'is put onto the battlefield', 'enters'),
    (r'put (.*?) onto the battlefield', r'\1 enters'),  # weak, rarely matches sanely; harmless on normalized compare
]

def norm_name(s):
    s = unicodedata.normalize('NFKD', s)
    s = s.replace('’',"'").replace('Æ','ae').replace('æ','ae')
    return re.sub(r'\s+',' ', s).strip().lower()

def norm_text(text, cardname):
    t = text
    t = unicodedata.normalize('NFKD', t)
    t = t.replace('’',"'").replace('‘',"'").replace('“','"').replace('”','"')
    t = t.replace('−','-').replace('—','-').replace('•','')
    t = re.sub(r'\([^)]*\)', '', t)              # reminder text
    t = t.replace(' -- ', '\n')
    t = t.lower()
    n = cardname.lower()
    t = t.replace(n, '~')
    # face names of split cards etc. left as-is
    t = t.replace('this spell','~').replace('this creature','~').replace('this card','~')
    t = t.replace('this permanent','~').replace('this artifact','~').replace('this enchantment','~').replace('this land','~').replace('this token','~')
    t = t.replace('this aura','~').replace('this equipment','~').replace('this vehicle','~').replace('this planeswalker','~')
    t = t.replace('this contraption','~').replace('this siege','~').replace('this scheme','~')
    first = n.split(',')[0].strip()
    if ',' in cardname and len(first) > 3: t = t.replace(first,'~')
    t = t.replace(' - ',' ')
    # older templating renames (pure rewords)
    t = t.replace('comes into play','enters').replace('come into play','enter')
    t = t.replace('is unblockable',"can't be blocked").replace('are unblockable',"can't be blocked")
    t = re.sub(r' to (your|their) mana pool','',t)
    t = t.replace('chaos ensues','you roll {k}')
    # singular-they errata
    t = t.replace('his or her','their').replace('him or her','them').replace('he or she','they')
    # dies templating
    t = re.sub(r'is put into (a|their|your) graveyard from the battlefield','dies',t)
    t = re.sub(r'are put into (a|their|your) graveyards? from the battlefield','die',t)
    # mill templating -> canonical 'mill N'
    t = re.sub(r'puts? the top (\w+) cards? of (your|their) library into (your|their) graveyard', r'mill \1', t)
    t = re.sub(r'mills? (\w+) cards?', r'mill \1', t)
    # token creation templating -> canonical 'create'
    t = re.sub(r'\bputs?\b([^.]*token[^.]*?) onto the battlefield( under (your|their) control)?', r'create\1', t)
    t = re.sub(r'\bcreates\b','create',t)
    # mana production: add {N} == add N x {C}
    def addfix(m):
        n = int(m.group(1)); return 'add ' + '{c}'*n
    t = re.sub(r'add \{(\d)\}', addfix, t)
    t = re.sub(r'adds \{(\d)\}', addfix, t)
    # activation templating
    t = t.replace('play this ability only','activate only').replace('activate this ability only','activate only')
    for a,b in RENAMES:
        t = re.sub(a,b,t)
    t = re.sub(r'[^a-z0-9~+/{}-]+',' ', t)
    t = ' '.join(w for w in t.split() if w not in ('-','//'))
    t = re.sub(r'\s+',' ', t).strip()
    return t

def main():
    prim = parse_primitives()
    orc  = load_oracle()
    buckets = {'MATCH':[], 'DIFF':[], 'NO_TEXT':[], 'NOT_FOUND':[]}
    for key,c in prim.items():
        nk = norm_name(c['name'])
        hit = orc.get(nk) or orc.get(key)
        if not hit:
            buckets['NOT_FOUND'].append((c['name'],'',''))
            continue
        oname, otext = hit
        if not c['text']:
            buckets['NO_TEXT'].append((c['name'],'',otext))
            continue
        a = norm_text(c['text'], c['name'])
        b = norm_text(otext, oname)
        if a == b:
            buckets['MATCH'].append((c['name'],c['text'],otext))
        else:
            buckets['DIFF'].append((c['name'],c['text'],otext))
    os.makedirs(OUT, exist_ok=True)
    for b,rows in buckets.items():
        with open(os.path.join(OUT, f'{b.lower()}.tsv'),'w') as f:
            for name,old,new in rows:
                f.write('\t'.join([name, old.replace('\t',' '), new.replace('\t',' ').replace('\n',' -- ')])+'\n')
        print(b, len(rows))
    print('total unique names:', len(prim))

if __name__ == '__main__':
    main()
