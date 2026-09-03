# Core-prompt / render proposals from the deck130 seat, wave 56
(Engine bugs are in `wave56/seats/seat-123-130.md`'s ENGINE list, not here.)

## G6. The board frame should carry the opponent's LIFE TREND, the way it now carries their mana.
Lane B's D10 shipped `Their untapped sources: N` on 753 of 753 prompts and it is the model for
this: a fact the pilot would otherwise have to reconstruct across windows, printed once. deck130's
entire face-damage decision, and the CROSS-CHECK latch built on it, turn on **whether the
opponent's life is rising and by how much per turn** - and the prompt gives only a single scalar
`Opponent life: N`. One line - `Opponent life trend: +5 over their last turn (Elixir of
Immortality)` or `unchanged for 6 turns` - converts a rule the pilot must remember across 60 turns
into a rule it can read. **Layer: render.** This is the highest-value addition I can name for this
deck: `130v125` is 137 records long and the latch's whole input is invisible on 136 of them.

## G7. A hold-vs-spend rule needs the board's REPLACEMENT capacity, and the render half-prints it.
The opponent battlefield line already gives `of which N are creatures` and `M are lands`. What the
"hold the removal for their next creature" rule actually needs is whether a next creature is
possible: at 0 creatures and 0 lands (`130v123` s76) the answer is no, and the burn is the clock.
`Opponent hand size` is printed; their land count is printed; a one-clause summary -
`they have 0 creatures and 0 untapped mana - no threat can arrive before your next turn` - would
close the gap without the pilot re-deriving it. **Layer: render.**

## G8. The X menu's marker has earned its promotion: mark ONE row on every priced menu, not only X.
6 of 6 marked X rows taken this corpus, 2 of 3 last, 0 of 2 the corpus before the marker existed.
The `[<- ...]` idiom is the most reliably followed annotation either of my seats produces. The two
places it would pay next are (a) the Starstorm CAST row one screen earlier, where WHETHER is
decided and the pilot still reads two lists by hand, and (b) any menu where two rows produce the
same `{right now: ...}` verdict at different prices - `123v125` s45 listed a Devour Flesh and a
Tribute to Hunger with byte-identical dead verdicts. **Layer: render**, and it is the cheapest
decision-quality win in the ledger.

## G9. Cycling rows should print what the cast alternative is worth right now.
`cycling with Lay Waste [cost: {2}, Cycle]` says nothing about the land kill it is spending, and
the cast row on the same card says nothing about the draw it is spending. At `Opponent life: 2`
with 14 untapped sources against four of their lands (`130v125` s52-s73), the cast is worth nothing
and the draw is the only line - and neither row says so. A single clause on the cycle row -
`{the cast this replaces: destroys 1 of their 4 lands}` - would let the guide's gate be read off
the window instead of carried as a threshold. **Layer: render.** 137 windows offered a cycle at
this seat and 16 were taken.

## G10 (LOW). `only display-toggle (Flip Side) options; auto-passing without a model call` is
inference-free churn worth removing even though it is not the livelock.
158 occurrences in one of my games (`game-146v130`), max consecutive run 4; 20, 16 and 13 in three
others. Each is a phase re-entry that produces no decision. Not a correctness item and not lane C's
base defect - filed here so the count exists for whoever costs the seam. **Layer: core loop.**
