# deck162 -> core-prompt / render proposals (wave 55)

## R-1 (HIGH, efficiency) - the identical-window re-ask is this pool's largest inference cost, and
the HOLD row alone is not closing it

`162v125` is 113 asks over 28 turns for a game the seat won 20/0, and the shape is one menu:
`1. Cast Dictate of Kruphix ... {feeds: ... converters on your battlefield: 0 ...}` /
`2. Cast nothing right now` / `3. Hold priority for the rest of this turn ...`, re-asked at Main 1,
Combat begins, Attackers, Combat ends, Main 2, End and Cleanup - eight to ten windows per turn, with
`[you declined this exact list N times already this turn]` counting to 10, for turns 11, 13, 15, 17,
19 and 21. Nothing on either board changed inside any of those turns.
The HOLD row is offered on every one of them and this seat took it **0 times** (deck162: 174
renders / 2 takes; at N>=2: 65 windows / 0 takes). The wave-55 guide edit teaches the row, which is
the guide side of the fix. The render side worth considering: when the declined-note N reaches a
threshold on a menu whose option-set key is unchanged, **stop asking for the rest of the turn and
record the auto-hold** rather than continuing to offer a row the pilot has declined nine times.
That is a policy change (it removes a legal choice from the pilot for the remainder of the turn),
so it is raised as a proposal and not as a fix - the owner's standing ruling rejects hard caps on
legal choices on sight, and this is adjacent to one. The non-adjacent half is free: hoist the HOLD
row to **row 1** on any window whose declined note reads N>=3, so the row the seat should take is
the row it anchors on.

## R-2 (MED, render) - the `{feeds:}` clause should name what would release the brake

The clause reads `converters on your battlefield: 0 (nothing of yours punishes their draws or
discards yet - the cards you hand them are free until a converter is out)`. It names the state and
not the exit. The engine knows the hand: appending
`- you hold Underworld Dreams` (or `- no converter in your hand either`) would put the wave-55
release condition (162-B) on the row instead of asking the pilot to cross-reference the hand line.
Repro: `162v123` s18 and `162v126` s23 (punisher in hand, take was right) versus `162v125` s98
(none in hand, take was the break) - three prompts whose `{feeds:}` clauses are byte-identical and
whose correct answers differ.

## R-3 (LOW, render) - the DRAW FORECAST line is the best clause in this deck's corpus; extend it
one step

`162v123` seq 30 declared `ATTACK: none` and won on the opponent's draw step, reasoning entirely
from two rendered lines: `DRAW PUNISHERS on the battlefield: yours - Underworld Dreams #1, Fate
Unraveler, Underworld Dreams #2. Every card the OPPONENT draws costs them 3 life` and
`DRAW FORECAST (theirs): their next draw step draws 5 cards (1 + Howling Mine #1 1 + Dictate of
Kruphix 1 + Howling Mine #2 1 + Howling Mine #3 1)`. The multiplication (5 x 3 = 15 against their
10) was done correctly by the model, which is the one arithmetic step it does not always do.
Suggested: print the product, `- that is 15 damage on their next draw step; they are at 10`. Low
priority precisely because it worked without it - recorded so the two lines are not simplified away.
