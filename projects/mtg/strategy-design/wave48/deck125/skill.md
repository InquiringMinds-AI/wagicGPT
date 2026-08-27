# deck125 - proposals for the strategy-writing skill (wave 48)

S1 - **Put the load-bearing number INTO the plan sentence the guide dictates.** On this seat 100% of
replies carry the guide's plan line verbatim ("Answer their threats, resolve Staff of Nin, ping
their face every turn"), and the one rule that has failed five waves running (the tap-out gate)
lives in a section the model demonstrably does not re-read at the cast ask. The plan line is the
only guide text guaranteed to be in the model's own output at every decision; a gate written into
it ("resolve Staff of Nin from a row reading leaves 5 or more") travels with the decision. Evidence:
`vs162` seq 27 (Staff at `leaves 2`, three counters in hand, Underworld Dreams resolved with no
window the next turn). Falsifier: Staff casts below `leaves 5` with a counter in hand, wave 49.

S2 - **A gate broken repeatedly at zero cost is a gate the executor learns to ignore - scope it to the
spends that matter.** Six of ten tap-out-gate violations this corpus were Elixir CASTS at {1}; none
cost anything. Every free violation is evidence, to the executor, that the rule is optional, and it
was then broken on the Staff cast that lost a game. When auditing a gate's fraction, split it by
spend size and exempt the spends whose violation has never cost anything (W10).

S3 - **A punisher that keys on "cast" needs its own rule; "optional draw" rules do not cover it.** The
guide's draw-is-damage stop held perfectly (0/9 Revelations under a punisher) while the seat
cast Elixir and Cancel under Forced Fruition + Underworld Dreams and took 7 and 14. The executor
classifies by the guide's vocabulary: "draw" rules fired on draws, and a spell that makes you draw
was not a "draw". When a pool card converts one action class into another (cast -> draw, draw ->
damage), the guide needs the composed price stated at the action the executor takes (W8).

S4 - **Adjudicate a 'hold' rule by what happened in the window AFTER the violation, not only by the
count.** W33's Staff sub-lane fell 6/7 -> 2/6 and would read as improving; one of the two carried
the game's deciding uncontested resolution the very next turn. Report the count AND the
consequence trace (the next opponent-turn record, or its absence) for any hold rule.
