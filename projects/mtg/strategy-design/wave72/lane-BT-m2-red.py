#!/usr/bin/env python3
"""#W72-BT M2 RED-on-base: the base predicate reads the two prospective-zero
PERMANENT rows as no-ops.

This is a faithful transcription of base 0e4e6b9b8's `rowSaysNoOp` magnitude
path (`rightNowComputedMagnitudesAreZero`, AIPlayerGPT.cpp:32011): the operative
core is only the text between "right now:" and the FIRST " - ", so what the
scanner actually reads on both rows below is the PRECONDITION, never an effect.
Run it: every row prints NO-OP, which is the refusal that ate 3 of the wave-71
corpus's 4 fallbacks.

The binary says the same thing without this script: base's own PARSETEST case
"#W58-A D3 POSITIVE every computed-zero verb emitted by the renderer is a no-op"
runs over an array that CONTAINS the Lightmine Field row and passes on
~/.gatelogs/w72-BT-base-wagic. Wave 72 moves that row out of the array and pins
it negative.
"""
import re

ROWS = {
 "125v152 seq 32 Lightmine Field (FALSE fire)":
   "Cast Lightmine Field {2}{w}{w} {right now: they control 0 creatures able to attack -"
   " deals 0 until they have an attacker - 1 noncreature permanent of theirs can animate"
   " into a creature and is not in that count}",
 "123v152 seq 10 Lightning Greaves (FALSE fire)":
   "Cast Lightning Greaves {2} {right now: you control 0 creatures - this equips nothing}",
 "125v152 seq 64 Supreme Verdict (TRUE fire, must stay a no-op)":
   "Cast Supreme Verdict {1}{u}{w}{w} {right now: destroys 0 of their creatures"
   " (0 without a restriction against attacking), 0 of yours - 1 noncreature permanent"
   " of theirs can animate into a creature and is not in that count}",
}

def verdict_clause(row):
    i = row.find("{right now:")
    if i < 0: return ""
    d = 0
    for j in range(i, len(row)):
        if row[j] == '{': d += 1
        elif row[j] == '}':
            d -= 1
            if d == 0: return row[i:j+1]
    return ""

def base_computed_zeros_are_zero(row):
    """base rightNowComputedMagnitudesAreZero, simplified to the shapes here."""
    clause = verdict_clause(row).lower()
    if not clause: return False
    colon = clause.find(':')
    end = clause.find(" - ", colon + 1)
    if end < 0: end = len(clause)
    core = clause[colon+1:end]
    core = re.sub(r'\([^)]*\)', '', core)          # parentheticals qualify, not state
    found = False
    for seg in core.split(','):
        seg = seg.strip()
        m = re.match(r'^(?:(?:they|you)\s+\w+\s+)?([+-]?\d+)', seg)
        if m:
            found = True
            if m.group(1).lstrip('+-') != '0':
                return False
    return found

if __name__ == "__main__":
    bad = 0
    for name, row in ROWS.items():
        z = base_computed_zeros_are_zero(row)
        print(("NO-OP  " if z else "LIVE   ") + name)
        if z and "FALSE fire" in name: bad += 1
    print()
    print("base refuses %d row(s) it should not (M2 RED)" % bad)
    raise SystemExit(1 if bad else 0)
