# Core-prompt / render proposals from the deck123 seat, wave 56
(Engine bugs are in `wave56/seats/seat-123-130.md`'s ENGINE list, not here.)

## G1. The Cleanup-discard menu should run the cast rows' dead-verdict family. (Re-filed from
wave-55 E4; untouched by the wave-56 lanes.)
`123v130` s22: on one 14-row discard menu with the opponent controlling no creature, the
`Tragic Slip` rows carry `{dead right now: 0 legal targets on the board for it}` while
`Tribute to Hunger`, `Devour Flesh` and `Damnation` carry nothing - and an edict at N=0 and a
Damnation at `destroys 0` are exactly as dead as the Slip. Until the predicate is the same family,
no guide can teach "the unannotated row is the keep", and every deck's discard rung stays at
"scan for `copy 2 of 2`". **Layer: render.** 14 discard asks at this seat, 1 annotated row family.

## G2. A menu-level verdict line. 85 of this seat's 197 casting menus had NO live cast row.
The pilot has to read six rows and their `{right now: ...}` clauses to discover that the menu is
empty; it did that correctly 84 times and failed once. One line under the question -
`every cast row on this menu is priced dead right now` - would make the whole class a one-token
read, and it is computed from clauses the emitter already produces. **Layer: render.** This is
the single highest-frequency inference the prompt currently asks the model to perform at this seat.

## G3. The fetch row's colour parenthesis should have a positive form, not only a negative one.
`... and it adds {W} or {B} (you cannot make {W} right now)` prints only when a colour is missing,
so on a window where both colours are available every row is untagged and a rule keyed to the tag
is silent. `123v126` s6 answered `adds {B}` over `adds {W} or {B}` there. A symmetric tag -
`(this is your only source of {U})` or simply marking the row that adds the most colours you are
short of, in the X-marker's `[<- ...]` idiom - would carry the same decision on the window where
the negative tag cannot appear. **Layer: render.** 5 fetch windows, 4 carrying the negative tag.

## G4. `INCOMING THIS COMBAT` should print the assignable remainder, not only the unblockable half.
The line already splits the total: `16 unblocked damage - you would be at -8; this KILLS you (of
that, 10 from 2 attackers none of your creatures can block)`. The decision it feeds is "can any
assignment get me above 0", and the seat has to do the subtraction (`123v152` s23: 16 - the largest
blockable 3 = 13 > 8, so decline). Printing `best case with every blocker assigned: you would be at
-5` would answer the question the header raises. **Layer: render**, from numbers already computed
for the B-lines' trade previews.

## G5 (LOW). `[NAMED BY THEIR <src>: ...]` should say whether the price is per-cast or one-off.
`123v146` s7: `[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life and draws them a
card]`. The seat cannot tell from the row whether declining this window makes the surcharge go
away, or whether the card is taxed for the rest of the game - which is the difference between
"pay it now" and "pay it when you must". 4 renders, 1 take. **Layer: render.**
