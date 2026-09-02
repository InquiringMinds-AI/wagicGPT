# Core-prompt / render proposals from the deck162 seat (wave 53)

## R-8 (HIGH, render). A CAST MENU PRICES EACH ROW ALONE; NOTHING ON THE SCREEN SAYS WHICH ROWS
## FIT TOGETHER IN THIS WINDOW.

`162v152` s11 (turn 10, 4 life, five untapped sources) offered, on one menu:
```
2. Cast Ob Nixilis, the Hate-Twisted {3}{b}{b} {leaves 0 of your 5 ... - casting this taps you out}
3. Cast Master of the Feast {1}{b}{b} (5/5) {leaves 2 of your 5 untapped mana sources untapped}
4. Cast Shield Sphere {0} (0/6) {leaves 5 of your 5 untapped mana sources untapped}
```
Rows 3 and 4 are affordable **together** - a 5/5 flier and a 0/6, two blockers against the four
attackers their header had just printed. Every row states only what IT leaves. The seat took row
2, tapped out, and the next menu had two rows on it; three attackers for 7 killed it at 0 the
following turn, and its own PLAN on the losing reply said *"Cast Master of the Feast"* - a plan
the row it took had already made impossible.

Lane O/E's `{spends K of your M untapped mana sources this turn; <card> in your hand needs K}`
clause (666 renders corpus-wide, 112 at my seats) is exactly the right idea pointed at the HAND.
**Ask:** point the same computation at the MENU - on a row that does not tap you out, name what
else on this menu still fits after it, e.g. `{after this, rows 3 and 4 are still affordable}` or,
cheaper and sufficient, `{taps you out - rows 3 and 4 on this menu become unaffordable}` on the
rows that do. The engine recomputes affordability for the next ask anyway; printing it one ask
earlier is what turns a sequence of single-row prices into a window budget.

## R-9 (MED, prompt). THE PLAN LINE IS RE-SERVED VERBATIM AFTER THE BOARD IT DESCRIBES HAS BEEN
## SPENT, AND IT IS AN INPUT TO THE NEXT ANSWER.

`162v152` s11 wrote `PLAN: ... Cast Master of the Feast. Attack with Master of the Feast.` while
choosing the row that made Master unaffordable, and s12 was served that same plan as
`YOUR PLAN (as you last stated it, 1 window ago on turn 10)`. Same shape at `146v125` s528 (plan
naming a Kaya not in hand, carried across three windows) and at `152v123` s16 (a 630-second land
decision whose plan reasons about a Lord of Lineage that was killed the same turn). The age stamp
lane N shipped is working - **3,329 renders, 0 PLAN blocks without one** - so the staleness is
now visible as a NUMBER but not as a FACT: a plan two windows old that names a card no longer
castable reads exactly like a fresh one.

**Ask (routed to the core prompt, complementing general-strategy R146):** where the served plan
names a card, mark the ones the current screen contradicts - `YOUR PLAN (as you last stated it,
1 window ago on turn 10; "Master of the Feast" is no longer on your menu)`. The stamp already
proves the engine tracks the plan's provenance; a one-clause diff against the current option list
is the smallest thing that makes a stale plan look stale.

## R-10 (LOW, measurement). REPORT `hold_windows_skipped` AND `mana_only_windows_skipped` PER SEAT
## IN THE HARVEST SUMMARY.

Both are new on the `gameend` record and both are load-bearing for lane N's verdict:
`mana_only_windows_skipped` totals **406** across 40 gameends and is the suppression that is
working silently, while `hold_windows_skipped` totals **102** and is 0 on 38 of 40 seats. Without
them the HOLD row's 1,173-render / 29-take rate looks like a wording problem alone; with them it
is measurable that the mechanism saves ~3.5 windows per take when it is taken at all. Printing
both in the harness summary beside the fallback table would have made lane N's own falsifier
readable without a per-seat script.
