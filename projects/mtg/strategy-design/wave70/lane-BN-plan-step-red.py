#!/usr/bin/env python3
"""#W70-BN (F10): plan-step consumption, base vs fixed, over one window sequence.

Astra review finding 10: the pointer advanced inside `consumePlan` - at PARSE
time - so a NEW plan whose step one executed re-served that step, while a
re-ask, an invalid echo and an empty reply each walked the sequence forward
although nothing happened on the board. The fix advances only on the record of
an EXECUTED decision (a model round trip, choice >= 0, no fallback class).

`mPlanStepsDone` lives on the seat object, so PARSETEST cannot drive it; this
replicates both rules line for line and shows what each serves next window.
The pure predicate the engine now calls (`AIPlayerGPT::planStepExecuted`) IS
pinned by PARSETEST - this script is the sequence around it.
"""
import sys


def step_ends(plan):
    ends, st = [], next((k for k, c in enumerate(plan) if c not in ' \t\r\n;,.!?'), None)

    def then_at(i):
        if plan[i:i + 4].lower() != 'then':
            return False
        if i + 4 < len(plan) and (plan[i + 4].isalnum() or plan[i + 4] == '_'):
            return False
        return i > 0 and plan[i - 1].isspace()
    for i, c in enumerate(plan):
        end = None
        if c == '\n':
            end = i + 1
        elif c == ';' and (i + 1 >= len(plan) or plan[i + 1].isspace()):
            end = i + 1
        elif c in '.!?' and i + 2 < len(plan) and plan[i + 1].isspace() and plan[i + 2].isupper():
            end = i + 1
        elif then_at(i):
            back = i
            while back > 0 and plan[back - 1].isspace():
                back -= 1
            end = back
        if end is None or st is None or end <= st or (ends and ends[-1] >= end):
            continue
        ends.append(end)
    if not ends or ends[-1] < len(plan):
        last = len(plan.rstrip())
        if last and (not ends or ends[-1] <= last):
            ends.append(last)
    return ends


def steps_after(plan, done):
    if done == 0:
        return plan
    ends = step_ends(plan)
    if len(ends) <= 1:
        return plan
    done = min(done, len(ends) - 1)
    at = ends[done - 1]
    while at < len(plan) and (plan[at].isspace() or plan[at] in ';,'):
        at += 1
    return plan if at >= len(plan) else plan[at:]


# A window: (the plan the reply states or None to omit it, choice, fallback class)
WINDOWS = [
    ("cast Bear; then attack; then cast Rhino", 2, None),   # executed: Bear
    (None, -1, "empty_reply"),                              # nothing happened
    ("cast Bear; then attack; then cast Rhino", 3, "plan_choice_conflict_reask"),  # re-ask
    ("cast Bear; then attack; then cast Rhino", 3, None),   # executed: attack
]


def walk(parse_time):
    plan, done = "", 0
    out = []
    for stated, choice, fb in WINDOWS:
        out.append(steps_after(plan, done) if plan else "(no plan)")
        if stated is not None:
            if parse_time:
                if stated == plan:
                    if done + 1 < len(step_ends(stated)):
                        done += 1
                else:
                    done = 0
            else:
                if stated != plan:
                    done = 0
            plan = stated
        elif parse_time and plan and done + 1 < len(step_ends(plan)):
            done += 1
        if not parse_time and plan and choice >= 0 and not fb \
                and done + 1 < len(step_ends(plan)):
            done += 1
    out.append(steps_after(plan, done) if plan else "(no plan)")
    return out


def main():
    base, fixed = walk(True), walk(False)
    print('window | served (base = advance at PARSE) | served (fixed = advance at EXECUTION)')
    for i, (b, f) in enumerate(zip(base, fixed)):
        print('%6d | %-46s | %s' % (i, b[:46], f))
    bad = base[3] != fixed[3]
    print()
    print('base  serves window 3 (after Bear executed, an empty reply and a re-ask): %r' % base[3])
    print('fixed serves window 3: %r' % fixed[3])
    print('RED on base' if bad else 'no difference')
    return 0 if bad else 1


if __name__ == '__main__':
    sys.exit(main())
