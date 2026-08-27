# deck125 - proposals for the strategy-writing skill (wave 49)

S1 - THE PLAN SENTENCE'S NAMED ACTION IS WHAT THE EXECUTOR TAKES; A QUALIFIER AFTER THE ACTION IS
DROPPED. Extends #115. deck125's dictated plan line was "resolve Staff of Nin from a row reading
leaves 5 or more"; three Staff casts this corpus (`vs152` seq 23, `vs146` seq 30, `vs130` seq 54)
echoed that sentence VERBATIM in the same reply that cast off a row reading "taps you out" or
"leaves 1". Six corpora of this gate failing share the shape: the action ("resolve Staff") fires,
the condition trailing it does not. Proposed rule: when the plan sentence carries a gate, write
the gate as the sentence's first clause and the action with "only" ("Keep a counterspell's mana
open; Staff of Nin only from a row reading leaves 5 or more"), and make the DECIDING SITUATIONS
bullet for that action carry the gate inside it - a second bullet that says "take it" without the
gate (deck125 had one) is the contradiction the executor resolves in favour of acting. Measure by
the plan-text metric (#118): a reply whose plan line contains the gate while the cast breaks it.

S2 - AN ABSOLUTE "TAKE IT WHENEVER IT IS PRINTED" FOR A FINISHER NAMES THE ONE BOARD WHERE THE
FINISHER IS THE OPPONENT'S CARD. Extends the obedient-absolutism pitfall. deck125's Emrakul line
("it outranks the tap-out gate ... take it anyway") was obeyed into a loss at 48 life with the
win already on the board (`vs126` seq 117 -> the opponent's Tribute to Hunger, -12), and the
same edict shape (Devour Flesh) cost 15 life in a won game (`vs123` seq 55 -> opp 2 -> 17). The
render had already said it: the LOOP clause ("ANY nonzero payment ... is fatal") was on the
prompt, and the opponent's Tribute row printed "Emrakul ... is sacrificed, you gain 15" on the
other seat. Rule: every "always take X" for a game-ending card is written with its counter-shape
in the same paragraph, keyed to a surface the executor can see (the opponent battlefield line, the
game log's "Opponent cast <edict>"), and the exception outranks the absolute by position (it comes
after it and says so).

S3 - RE-KEY A MANA-THRESHOLD RULE TO THE ROW'S PRINTED SIZE WHEN THE ROW PRINTS ONE. deck125's
"cast Revelation at Mana available: SIX or more" was broken five times for X=1/X=2 casts while
the row printed `{X pricing: max affordable X=N}`. The number the rule wants IS on the row; a rule
that asks the executor to compare a total against a cost it has to remember is a rule it
approximates. Same family as #110 (answer the mark) - applies whenever lane Q/T annotations put
the decisive number on the row itself.
