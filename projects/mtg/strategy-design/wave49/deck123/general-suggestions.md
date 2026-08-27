# deck123 - core-prompt / render proposals (wave 49), layer-routed

Engine BUGS are in `wave49/seats/seat-123-130.md` (H1-H3, M1-M4, L1-L3), not here.

**G1 (render, MED) - the repeat row's example number is the number the pilot writes.** The row's
example reads `e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x50)"`; of 11 takes at this
seat, one was x50 and one x100/x120 in the same turn. The example is the only number on the row.
Proposal: print the example with a placeholder (`x<N>`) and, since the engine knows the pilot's
creature count, print the count beside the row (`you control M creatures` is already in the
`[repeat:]` tag on the single row - repeat it on the N row).

**G2 (render, LOW) - the `[repeat:]` tag's two counters read as one number.** `activated this turn
19 times already; you control 20 creatures ... you have taken it 22 times in a row` - three
integers in one bracket; the guide teaches which is M. Proposal: lead with the battlefield count
in the same words the battlefield line uses (`of which 20 are creatures`), so the pilot's key is
one string.

**G3 (core prompt, MED) - the carried plan expired late.** Lane U's D7 said the carried plan is
dropped after 5 identical echoes or when it names no action; at `vs126` seq 2-9 the identical
opening plan rode 8 consecutive windows, and "The game is lost ..." rode `vs125` seq 53-56 (4
windows, a plan naming no action). Both under the predicted ceiling's spirit and over its letter;
the seat file carries the citations for the engine seat. Nothing prompt-side to add.

**G4 (core prompt, LOW) - a mandatory-choice ask should say it has no pass.** `vs130` seq 31 (the
damage-assignment-order ask) was answered `CHOICE: 0 (Pass)` after a valid first line; the reply
protocol's "0 = pass priority" habit carried into an ask that has no row 0. The ask's own text
already omits a 0 row; a one-line "this ask has no pass - pick a row" would remove the habit.
