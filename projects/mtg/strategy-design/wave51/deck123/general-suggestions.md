# deck123 - core-prompt / render proposals (wave 51), layer-routed

R1 (render, -N/-N spell target rows). Tragic Slip's target rows print `(P/T)` but no `{right now:
-1/-1 - SURVIVES (toughness 2)}`; damage rows do. 123v152 seq 11 ("it is 3/1") and 123v130 seq 21
misread toughness. Ask: the same DIES/SURVIVES clause on -N/-N target rows, computed from the
printed toughness. (Seat item M2.)

R2 (render, ability-target menus). `Equip with Lightning Greaves targeting Vampire #1..#25` printed
one row per identical token (123v126 seq 42: 36 rows, prompt 29,469 chars - the corpus max at this
pair; seq 44 35 rows; seq 51 34 rows). The cast-row collapse (`Vampire #1-#20 x20`) does not apply
to ability rows. Ask: collapse identical-token target rows on ability menus to one row + `xN`, as
the spell target list already does. (Seat item L1.)

R3 (render, fetch rows). On `Put a card onto the battlefield with Marsh Flats targeting ...` rows,
print `{this land makes no mana until sacrificed; taking this raises your untapped sources from N
to N+1}` - the pilot counted the Flats as a land it had (123v130 seq 6-13). Guide half A51-1.

R4 (core prompt). The prompt line "You will have priority again later this turn, so instants and
activated abilities you hold stay castable this turn" is printed on opponent-turn empty-stack
priority windows (123v130 seq 8) where no Cast row is ever offered (seat item H1: 0 of 51 such
windows corpus-wide carried a Cast row). Until H1 ships, the sentence promises a window that does
not come; drop it or restrict it to the pilot's own turn.
