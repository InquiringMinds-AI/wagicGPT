#!/usr/bin/env python3
"""#W71-BO RED-on-base witness for wave-70 L9 and L10, measured on the corpus the
base binary (77d82bbe6, wave-70 build 14c82df37) produced:
  ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849

L9  the base stamps `reasoning_budget_hit` / `reasoning_forced_close` on records
    whose decision never had a phase-2 close.  A phase-2 close is IDENTIFIABLE in
    the data: it is the only request sent with max_tokens_reasoning == 0 (the
    answer-locked / forced-close leg).  If the markers were bound to the close,
    #stamped == #closes.  They are not.
L10 a MISSING `PLAN:` line is invisible: every reply with no PLAN line at all
    carries off_protocol_bytes == 0, i.e. the same value a perfect reply carries.
Exit 1 when the base defect is present (RED), 0 when it is not.
"""
import json, glob, os, re, sys

RUN = os.path.expanduser("~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849")
PLAN = re.compile(r'^[ \t]*(?:\*\*|[-*>]\s*)?PLAN[ \t]*:', re.I | re.M)

stamped_budget = stamped_close = closes = 0
plan_missing = plan_missing_off0 = 0
mislabelled_charged = 0
records = 0

for f in glob.glob(os.path.join(RUN, "*.jsonl")):
    for line in open(f, errors="replace"):
        try:
            r = json.loads(line)
        except Exception:
            continue
        if r.get("kind") in ("gameend", "gamestart", "system", "recovery"):
            continue
        records += 1
        if r.get("reasoning_budget_hit"):
            stamped_budget += 1
        if r.get("reasoning_forced_close"):
            stamped_close += 1
        if r.get("max_tokens_reasoning") == 0 and r.get("max_tokens_answer"):
            closes += 1
        rep = r.get("reply") or ""
        t = rep.split("</think>")[-1]
        if not PLAN.search(t):
            plan_missing += 1
            if r.get("off_protocol_bytes", 0) == 0:
                plan_missing_off0 += 1
        elif r.get("off_protocol_bytes", 0) > 0:
            mislabelled_charged += 1

print("records with a round trip .............. %d" % records)
print("L9  reasoning_forced_close stamped ..... %d" % stamped_close)
print("L9  reasoning_budget_hit   stamped ..... %d" % stamped_budget)
print("L9  phase-2 closes actually sent ....... %d  (max_tokens_reasoning == 0)" % closes)
print("L10 replies with NO PLAN: line ......... %d" % plan_missing)
print("L10 ...of those, off_protocol_bytes == 0 %d" % plan_missing_off0)
print("L10 mislabelled plans CHARGED bytes .... %d" % mislabelled_charged)

red = []
if stamped_close != closes:
    red.append("L9: %d forced-close stamps against %d actual phase-2 closes - the marker is "
               "not bound to the close it names" % (stamped_close, closes))
if stamped_budget != closes:
    red.append("L9: %d budget-hit stamps against %d actual phase-2 closes" % (stamped_budget, closes))
if plan_missing_off0:
    red.append("L10: %d of %d replies with no line-leading PLAN: label carry "
               "off_protocol_bytes == 0 - byte-identical to a perfect two-line reply. No field "
               "on the base record distinguishes them" % (plan_missing_off0, plan_missing))
if red:
    print("\nRED on base:")
    for r in red:
        print("  - " + r)
    sys.exit(1)
print("\nno base defect detected")
