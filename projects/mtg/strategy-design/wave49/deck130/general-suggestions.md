# deck130 - core-prompt / render proposals (wave 49), layer-routed

Engine BUGS (the vanishing rows at `vs146` seq 14 -> 16; self-only target rows) are seat items H3/M3
in `wave49/seats/seat-123-130.md`.

**G1 (render, MED) - tag self-targets on the row, in the same words the guide uses.** Damage and
destroy rows print `targeting <name> [your battlefield]`; nothing else marks that the row hurts its
controller. The two self-hits this corpus were on windows where the self-target row was the ONLY
action row. Proposal (layer: render, not the menu): append `{this hits YOUR permanent}` to any row
whose target's controller is the acting player, and on a window where every action row carries it,
print a line above the rows: `Every action here targets your own permanent; 0 (pass) is the
usual answer.` No option removed.

**G2 (render, LOW) - the target ask's `{right now: takes 3 damage - SURVIVES (toughness 4)}` is
excellent and was ignored once (`vs146` seq 25).** Proposal: when every creature target on the ask
reads SURVIVES and the spell's damage cannot kill anything, say so once at the top (`no creature on
this list dies to 3 damage`), so the read is one line instead of N.

**G3 (render, LOW) - the Starstorm cast row's `smallest X that kills an OPPONENT creature` clause
still leads.** At `vs123` seq 22 the row read `smallest X ...: X=1 ... kills THEIRS: Human x2;
YOURS: Dwarven Blastminer. At X=4 - kills THEIRS: Thraben Doomsayer, Human x2 ...` and the pilot
took X=1 after spending mana elsewhere. The guide teaches "read to the end"; the render could lead
with the largest affordable X's kill list and put the cheapest-kill clause second (owner ruling:
largest-first on the X menu - the cast row's annotation could follow the same order).

**G4 (core prompt, LOW) - carried plan naming an action the window does not offer.** Both self-hits
carried a plan naming a target not on the window ("destroy Brightclimb Pathway", "destroy
Forest"). D7 retires a plan that names NO action; a plan that names an action the current window
cannot perform is the neighbouring case. Proposal: when no row on the window contains the plan's
named target, print `(your plan's target is not on this window)` under the carried plan.
