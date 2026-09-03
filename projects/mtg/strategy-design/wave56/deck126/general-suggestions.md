# deck126 -> core-prompt / render proposals (wave 56)

Layer-routed. Engine BUGS are in `wave56/seats/seat-125-126.md`'s ranked list; these are
prompt-text and render-content proposals. Strings quoted from rendered `prompt`s in
`matchups-20260903-120952`.

## G6 (HIGH) - `INCOMING THIS COMBAT` should be gated on THEIR attackers, not on MY blockers

The line shipped and it is right: `126v146` seq 41 renders `INCOMING THIS COMBAT: 9 attackers, 26
unblocked damage - you would be at -18; this KILLS you (your creatures may legally block every
attacker in that total)`, on 9 of 9 Blockers-phase windows at this seat, with the sum matching the
A-lines every time.

But it is emitted from the `blockers` ask, and a seat with no creature never gets one. deck125 took
**68** opponent-combat windows (`Combat begins` 28, `Attackers` 20, `Combat ends` 20) across six
games and **none of them carries the line**, while its battlefield line prints every attacker's
(P/T) and `[attacking]` tag and no total. Ask: emit the same device on any `ask`/`priority` window
during the opponent's combat whenever the opponent's header shows attackers, whether or not the
seat has a blocker. A control deck's whole decision at that window is "does this combat kill me",
and it is the one number the prompt does not print for it.

## G7 (MED) - a `{right now:}` verdict should notice the seat's own answer already on the stack

`126v146` seq 19: the prompt's stack section reads `1 (top): ability: Tribute to Hunger's gain life
equal to its toughness (aimed at Silverquill Silencer) [from your Tribute to Hunger]`, and row 1
still reads `Cast Tribute to Hunger {2}{b} {right now: they control 1 creature - Silverquill
Silencer (3/2) is sacrificed, you gain 2}`. Both are true at that instant; together they mislead.
Same shape at `126v130` seq 29 and `126v125` seq 262.

Ask: append `- but an answer of yours on the stack is already removing it` when an object on the
stack targets or removes the victim the row names. The engine has both facts on screen already; the
row is where the decision is made. 3 of this seat's 10 Tribute casts went this way, and the deck
runs two copies.

## G8 (MED) - the empty-stack opponent-turn window is skippable, and deck126 already proves it

deck126 skipped **1,482** windows via `mana_only_windows_skipped` across six games (508/197/58/19/
104/596) with no cost this corpus can find, and still answered 230 opponent-turn windows of which
**226 produced no cast** - Upkeep 53, Draw 20, Combat begins 18, Attackers 18, Combat ends 17,
Main 2 17, End 17, Cleanup 16, Blockers 9, Untap 6, all zero. deck125's figure is 210 of 302 at
nine phases, fifth corpus running.

Ask (unchanged in substance from wave 53 MED #6, now with the mana-only skip as a working
precedent in the SAME binary): skip and count an opponent-turn window whose `ON THE STACK` section
is EMPTY at a phase where this seat has not cast in this game. At 28.6 s per decision the deck126
side alone is ~1.8 h of inference across six games for zero casts.

## G9 (MED) - say on the row when the whole menu is dead

deck126's failure mode this corpus was six spends on rows the engine itself had already marked
worthless: `{right now: they control 0 creatures - at 0 this does nothing}` (296 rows),
`[second copy: you already control <name>]` (27 rows), and
`[finds only an enchantment card - every enchantment left in your library is a copy of one you
already control or hold: ...]` (17 rows). Each row says it is dead; nothing says the MENU is.

Ask: when every `Cast` row on a menu carries one of the engine's own dead-row markers, add one
line above the decline row - `{every cast row on this menu is marked as changing nothing}`. It
adds no choice and removes none; it states a fact the render has already computed row by row. The
mirror of lane B/D's `{leaves N sources - no other row on this menu needs more than N}`, which
works well.

## G10 (LOW) - name the trigger, not just the parenthesis, in the B-line

`{blocking trigger, this combat: you may gain 2, and if you do your converter takes 2 off them}` is
a good clause and it is doing real work (45 rows). One improvement: it renders INSIDE / beside the
1-on-1 verdict `(your blocker dies, attacker lives)`, whose plain meaning is "don't". At
`126v146` seq 37 the seat read the death half, answered `no blockers`, and lost the game to an
opponent it could have drained. Ask: when the seat controls a converter, lift the drain onto the
BLOCKS header the way `INCOMING THIS COMBAT` is lifted - e.g.
`BLOCKING THIS COMBAT: each of your N blockers that blocks gains you 2 and takes 2 off them - up to
K off their K+M`. The per-row clause stays; the total is the number the decision turns on, and
totals are what this render does well everywhere else.
