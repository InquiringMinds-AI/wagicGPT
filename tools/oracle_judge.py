#!/usr/bin/env python3
"""Judge DIFF-bucket cards: does the primitives text= describe the same FUNCTION
as current Oracle? Uses Spark vLLM (OpenAI-compatible, structured output).

Usage: oracle_judge.py [--limit N] [--offset N] [--out FILE]
Verdicts TSV: name <TAB> verdict <TAB> reason
  SAME      - wording/templating differs, function identical
  DIFFERENT - the card does something different now (errata changed function)
  UNSURE    - judge could not decide -> treat as DIFFERENT downstream
"""
import json, sys, os, argparse, urllib.request
from concurrent.futures import ThreadPoolExecutor, as_completed

BASE = 'http://100.116.136.74:8081/v1'
MODEL = 'qwen35'
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DIFF = os.path.join(ROOT,'projects/mtg/strategy-design/oracle-sweep/diff.tsv')

SCHEMA = {
  "type":"object",
  "properties":{
    "verdict":{"type":"string","enum":["SAME","DIFFERENT","UNSURE"]},
    "reason":{"type":"string","maxLength":200}
  },
  "required":["verdict","reason"],"additionalProperties":False
}

PROMPT = """You compare two rules texts of the Magic: The Gathering card "{name}".

TEXT A is from an old game database (may be paraphrased, abbreviated, or use pre-errata wording).
TEXT B is the card's current official Oracle text.

Ignore ALL of the following as irrelevant: wording style, templating era ("comes into play" vs "enters", "put a token onto the battlefield" vs "create", "his or her" vs "their", "converted mana cost" vs "mana value", "add to your mana pool" vs "add"), reminder text in parentheses, missing flavor, abbreviation, capitalization, punctuation, and line order.

Decide ONLY whether the card's FUNCTION changed: different numbers, costs, targets, zones, timing, conditions, affected players, added or removed abilities, or changed scope (e.g. "creatures" vs "creatures you control", "target player" vs "any target" where planeswalkers now can be hit).

If TEXT A is an obvious abbreviation of the same behavior, answer SAME.
If you cannot confidently rule out a functional difference, answer UNSURE.

TEXT A: {old}

TEXT B: {new}"""

def judge(name, old, new):
    body = {
      "model": MODEL,
      "messages":[{"role":"user","content": PROMPT.format(name=name, old=old, new=new)}],
      "max_tokens": 2048,
      "temperature": 0.0,
      "chat_template_kwargs": {"enable_thinking": False},
      "response_format":{"type":"json_schema","json_schema":{"name":"verdict","schema":SCHEMA}}
    }
    import time
    for attempt in range(3):
        try:
            req = urllib.request.Request(f'{BASE}/chat/completions', data=json.dumps(body).encode(), headers={'Content-Type':'application/json'})
            with urllib.request.urlopen(req, timeout=300) as r:
                d = json.load(r)
            v = json.loads(d['choices'][0]['message']['content'])
            return name, v['verdict'], v.get('reason','')
        except Exception as e:
            if attempt == 2: return name, 'ERROR', str(e)[:180]
            time.sleep(5)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--limit', type=int, default=0)
    ap.add_argument('--offset', type=int, default=0)
    ap.add_argument('--out', default=os.path.join(ROOT,'projects/mtg/strategy-design/oracle-sweep/verdicts.tsv'))
    a = ap.parse_args()
    rows = [l.rstrip('\n').split('\t') for l in open(DIFF)]
    rows = rows[a.offset : a.offset+a.limit if a.limit else None]
    done = set()
    if os.path.exists(a.out):
        done = {l.split('\t')[0] for l in open(a.out)}
    rows = [r for r in rows if r[0] not in done]
    out = open(a.out,'a')
    n = 0
    with ThreadPoolExecutor(max_workers=24) as ex:
        futs = [ex.submit(judge,*r) for r in rows]
        for fut in as_completed(futs):
            name,v,reason = fut.result()
            out.write(f'{name}\t{v}\t{reason.replace(chr(9)," ").replace(chr(10)," ")}\n'); out.flush()
            n += 1
            if n % 100 == 0: print(f'{n}/{len(rows)}', flush=True)
    print('done', n)

main()
