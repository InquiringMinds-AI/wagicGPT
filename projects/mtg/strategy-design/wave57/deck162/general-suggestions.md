# deck162 -> core-prompt / render proposals (layer-routed)

Engine bugs are in the seat file (E-1..E-6). These are prompt/render asks.

## R-7 (MED) - the `{feeds:}` clause states a fact where a VERDICT is available, and the pool's other braces already give verdicts

`{feeds: the opponent draws 1 extra card per turn; converters on your battlefield: 0 (nothing of
yours punishes their draws or discards yet - the cards you hand them are free until a converter is
out); in your hand: 0}` is complete and true, and this seat read it correctly five times and cast
the row anyway (`162v125` seqs 40, 41, 57, 64, 65). Compare its neighbours, which do not stop at
the fact: `[legendary: you already control X - legend rule: casting this sends one copy to the
graveyard]`, `{right now: returns NOTHING - your graveyard has no creature cards}`,
`{dead right now: ...}`. Those three name the OUTCOME of taking the row.

Ask: give the zero-converter case the same shape - `{feeds: ... converters on your battlefield: 0}
{right now: this row gives the opponent N cards and converts none of them}`. It costs one clause,
it reuses the `{right now:` channel that already exists on this deck's own Agadeem row, and it
turns a fact the pilot narrates into an outcome it must answer. This is a render ask, not a rule:
I am not asking the engine to forbid the row.

## R-8 (LOW) - Dictate of Kruphix's symmetry is not on its row

`{feeds: the opponent draws 1 extra card per turn ...}` is true and half the card: Dictate of
Kruphix draws BOTH players an extra card each draw step, and unlike Master of the Feast it puts no
body on the board. The row prints the gift and, correctly, does not price the gain - but a pilot
weighing "engine now, converter later" has only the gift number in front of it. If `{feeds:}` ever
gains a second half, the honest form is symmetric: what it gives them and what it gives you, both
counted. Repro `162v125` seq 40 (turn 21, two Dictates cast at 0 converters in consecutive windows).

## R-9 (LOW, observation, no ask) - this deck's decisions are cheap and its games are long

180 decisions across 6 games, mean prompt 10,846 chars, GAME LOG 48.3% of it - the lowest log share
of my three decks - and yet its largest prompt (`162v125` seq 56, **30,052 chars**) is the largest
at any of my seats, because its own engine puts 23 permanents on its battlefield line. If the D13
narration budget is ever taken, this deck is the one where a per-ask-kind cut would collide with a
legitimately large BOARD frame rather than a large log. Worth a datapoint in that decision, not a
change here.
