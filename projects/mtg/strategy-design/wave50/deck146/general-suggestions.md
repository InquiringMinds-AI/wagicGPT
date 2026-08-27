# deck146 - core-prompt / render proposals (wave 50), layer-routed

Engine BUGS go to `wave50/seats/seat-146-152-162.md` (E-1 Soul Shatter no-sacrifice, E-2 missing
`- Paid` on activations, E-3 dual-land generic payment). Below are RENDER and CORE-PROMPT items.

G-1 (render, MED). **Offer land animations in Main 1, not only in Upkeep.** On the six deck146 seats
`becomes beholder with Hive of the Eye Tyrant` was offered 11 times, all 11 in Upkeep, 0 in a main
phase; the two takes (`146 vs126` s31, s35) burned {3}{B} twice before the draw step and produced
no attack (s34, s39 `ATTACK: none`). The wave-48 guide rule ("Upkeep -> PASS") is doing the
engine's job; deck152's Lair shows the same skew (19 Upkeep / 13 Main-1 offers). Proposal: when a
seat holds priority in its own Upkeep and the only non-toggle rows are mana-cost activations of
its own permanents, defer the ask to Main 1 (the row is offered there again anyway) - or at
minimum annotate the Upkeep row `{before your draw step - the same row is offered in Main 1}`.
Falsifier: Hive/Lair Upkeep takes 0/N with the Main-1 offer count > 0.

G-2 (render, MED). **Print the edict outcome on the Soul Shatter row.** Lane X put `{right now:
they control N creatures - ...}` on edict rows; the Soul Shatter row printed only its card text
(`146 vs162` s25; `146 vs123` s20, cast into 0 creatures). A `{right now: they sacrifice <name>}`
/ `{right now: they control 0 creatures - this does nothing}` clause would have exposed both the
empty-board cast and the engine defect E-1 at the row. Falsifier: Soul Shatter casts at `0
creatures` 0/N; the annotation's named creature equals the narrated sacrifice N/N.

G-3 (core prompt, LOW-MED). **Count decisions reversed in prose after the coded line.** `146 vs123`
s20: line 1 `CHOICE: 3 (Cast Soul Shatter)`, then 2,700 chars ending "Soul Shatter is bad ... I
will cast Pelakka Predation" with no second coded line; the first line executed. `146 vs126` s18:
four coded ATTACK lines in one reply (407 s). The reply protocol asks for the answer FIRST, so the
pilot commits before it thinks; lane Y's last-line rule only helps when the revision is coded.
Proposal (metric first): `decision_reversed_in_prose` = the reply's last "I will cast/attack X"
sentence names a row other than the last coded line's. If it is non-trivial corpus-wide, the
core-prompt half is one sentence: "if you change your answer, write the new CHOICE line last".

G-4 (render, LOW). **Sequential sacrifice asks show a stale battlefield line.** `146 vs125`
s60-s65 (Emrakul's annihilator 6): six asks in a row; the option list shrank each time but `Your
battlefield (14 permanents ...)` still listed Spider, Goblin and Plains #4 after they were chosen.
Harmless here; a pilot counting blockers from the state block mid-sequence would count dead ones.

G-5 (render, LOW). **The `(text omitted)` marker is now on every back face on my seats** (0 bare
`// <name>"}` endings in 18 logs) - lane X prediction PASS; wave-49 E-4 closes.
