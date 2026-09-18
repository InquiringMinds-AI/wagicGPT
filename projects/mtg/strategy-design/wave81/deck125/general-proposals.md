# deck125 seat — general-guide proposal (wave 81)

One proposal. Evidence is in `wave81/deck125/review.md` §1 (deck50 game) and items H1/H2.

## P-G1: your own library is a resource that can lose you the game, and nothing on the screen says so

**What the games show.** deck125 lost the deck50 (Erosion) game at turn 42 holding 59 life against an
opponent on 7 life with one card left in their library — it drew from an empty library. Every step of that
was taken on a true screen: the opponent's line printed
`Opponent library: 1 cards - DECK-OUT IS IN RANGE: a player who must draw from an empty library LOSES …`,
while the seat's own line printed `Your library: 11 cards` and then `Your library: 2 cards` with no
annotation at all (corpus-wide: `DECK-OUT IS IN RANGE` renders on the opponent's line only, 0 of 2,122
windows on the seat's own). The pilot read the opponent's deck-out as a win condition and never read its
own as a loss condition, because only one of the two was written down.

**Proposed general-guide text** (one paragraph, to sit with the other loss-condition material):

> Your library is a life total you cannot see fall. A player who must draw from an empty library loses, and
> the draws that empty it are ones no row declines: your draw step, and every extra draw a permanent of
> yours or theirs gives you. Read `Your library: N cards` at every window where you are choosing to draw or
> to cast, count the draws already owed, and treat a row that draws more cards than the game has turns left
> as a cost, not a gain. A card of theirs that says an opponent who casts a spell mills that player prices
> EVERY cast of yours, and no row's own count includes it.

**Why the general guide and not only deck125's.** Any deck can be milled and any deck with a draw engine
can deck itself; deck50 (Erosion) has just entered the pool as a dedicated mill deck, so this shape will
now be produced against every seat, and the six games here show a pilot that reads the opponent's
deck-out line correctly and has no sentence at all for its own.

**Ranking note for synthesis.** If item H1 is fixed — the `DECK-OUT IS IN RANGE` clause rendered on the
seat's own library line as well as the opponent's — the first two sentences of this paragraph become a
derivation the render performs, and the paragraph should shrink to the mill-on-cast sentence, which item
H2 would then also retire. Adopting the render fix is worth more than adopting this text; adopt both and
retire the text when the render lands.
