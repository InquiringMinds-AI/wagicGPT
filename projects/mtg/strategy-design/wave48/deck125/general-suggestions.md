# deck125 - core-prompt / render proposals (wave 48), layer-routed

Engine BUGS with repro are in `wave48/seats/seat-125-126.md` (engine list). These are render /
core-prompt affordances.

G1 (render, MED) - **Cast rows under an opposing "whenever an opponent casts a spell, that player
draws N" permanent need the composed `[DRAW PRICE:]`.** Lane Q's tag reaches rows whose CARD draws
(Dream Fracture, Wall of Omens - both observed), but the seven-card draw Forced Fruition forces
on EVERY cast is invisible at the row, and with Underworld Dreams beside it the row's true price
was 7 (then 14) life. `vs162` seq 44 (Elixir cast at 15 life -> 13) and seq 51 (Cancel at 11 life
-> dead) both chose from rows that read only `{leaves N ...}`. Proposal: when an opposing
permanent's `auto=` matches `@movedTo(...|opponentstack):draw:N`, every `Cast ...` row gets
`[DRAW PRICE: casting this draws you N (their Forced Fruition), and their Underworld Dreams deals
you N]` - the same emitter, one more trigger source. Guide W8 is the workaround until it lands.

G2 (render, MED) - **Sweeper rows carry no "what it would hit" preview, and the guide's rule fails
4/8 casts five waves running.** The header's count is on a different line from the row; `{right
now: ...}` per-branch previews (R25) exist for other families. Proposal: `Cast Supreme Verdict ...
{right now: destroys 0 of their creatures (0 able to attack), 0 of yours}`. The row that says
"destroys 0" is the affordance the guide has been trying to write in prose (`vs130` seq 45/72,
`vs126` seq 46, `vs162` seq 21; Lightmine Field `vs123` seq 40: `{right now: 0 creatures able to
attack - deals 0}`).

G3 (render, LOW) - **`{leaves N ...}` on activated-ability rows** (carried, wave-47 LOW #5 / R15):
`Life with Elixir of Immortality [cost: {2}, Tap]` still prints no `leaves`; 4 Elixir activations
this corpus took the seat to 0-2 open (`vs162` seq 29, `vs152` seq 26, `vs126` seq 13/22).

G4 (core prompt, LOW) - **Print the CURRENT keep size on the Keep row.** The mulligan row reads
"Mulligan (a keep after this one would keep 5 cards)" and the Keep row reads only "Keep this hand";
at `vs126` seq 2-3 the model shipped two keepable hands down to a 4-card keep. Guide W13 now
reasons from the mulligan row's number, but "Keep this hand (keeping 6 cards)" would put the
number the decision is about on the row being chosen.
