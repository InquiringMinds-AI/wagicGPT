#!/usr/bin/env python3
"""After merging lane branches: every scalar initialised in a LANE's ctor list must be in HEAD's.
Usage: check-merge-inits.py <lane-branch> [<lane-branch>...]  (run from the repo root)"""
import re,subprocess,sys
def inits(rev):
    # 2026-09-17: the GPT layer is split across AIPlayerGPT*.cpp; the ctor lives in whichever file defines it
    s=''
    for f in ['AIPlayerGPT.cpp','AIPlayerGPTTransport.cpp','AIPlayerGPTSeams.cpp','AIPlayerGPTParse.cpp','AIPlayerGPTTranslog.cpp']:
        t=subprocess.run(['git','show',f'{rev}:projects/mtg/src/{f}'],capture_output=True,text=True).stdout
        if 'AIPlayerGPT::AIPlayerGPT(' in t: s=t; break
    i=s.index('AIPlayerGPT::AIPlayerGPT('); j=s.index('\n{\n',i)
    return set(re.findall(r'\b(m[A-Z]\w*)\s*\(',re.sub(r'//[^\n]*','',s[i:j])))
head=inits('HEAD'); bad=0
for b in sys.argv[1:]:
    miss=sorted(inits(b)-head)
    print(b,'MISSING in HEAD:' if miss else 'ok',miss if miss else '')
    bad+=len(miss)
sys.exit(1 if bad else 0)
