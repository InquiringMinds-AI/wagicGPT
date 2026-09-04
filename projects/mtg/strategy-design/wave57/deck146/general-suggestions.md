# deck146 -> core-prompt / render proposals (layer-routed)

Engine BUGS go to the seat file's engine list (E-1..E-6), never here. These are prompt/render
asks - things the pilot is shown, or not shown.

## R-1 (HIGH) - price a life payment on its own row, the way every other menu is priced

The MDFC arrival menu is the only menu in the game that can end it, and it is the only menu that
prints no verdict. Every neighbour has one: ability rows carry `they would be at K`, blockers
windows carry `- best case with every blocker assigned: you would be at K`, cast rows carry
`{leaves N of your M untapped mana sources untapped}`. The pay row carries
`[usable (tap for mana / attack) this turn]` - the BENEFIT, unpriced.

Ask, in the existing braced channel so it stays out of history:
`1. pay 3 life - Agadeem, the Undercrypt enters UNTAPPED {this payment puts you at N}` and, when
N <= 0, `{this payment puts you at N: you LOSE the game}`.

Repro `146v130` seq 20 (`Your life: 1`, paid, gameend `my_life: -2` one record later). Generalise
it: **any row whose own text names a life cost should print the resulting life total**, computed
the same way `they would be at K` already is.

## R-2 (HIGH) - carry the MDFC land half onto every window that culls the hand

`1 card is counted TWICE above, on purpose:` renders on 6 prompts in this corpus and all 6 are
pregame. The cleanup discard, the forced discard and the reveal-to-bottom menus list the same card
as a plain sorcery. D2's detector reads the `autohand=` bucket and needs no board state, so it is
available at every one of those seams.

Ask: `{this card is also the land <back face> (taps for {X}) - discarding it discards that land}`
on the ROW, in the braced channel. Repro `146v130` seq 7 (row 8, one land on the battlefield).

## R-3 (MED) - the identical-declined note needs a second number

`[you declined this exact list N times already this turn]` fires on 801 corpus windows at N>=2
while only 163 windows in the whole corpus repeat their situation block. The note is true about the
menu and false about the game, and it is load-bearing (the trust doctrine makes it so): three decks
carry a guide rule keyed on it.

Ask: `[you declined this exact list N times already this turn; the board has not changed since M of
them]`, with M computed from the same block the prompt already assembles. When M is 0 the note is
telling the pilot to hold a window that will re-open anyway, which is the state 66 of deck152's 66
were in.

## R-4 (LOW) - the pregame mana-source line should count both faces of a land/land MDFC

`152v125` seq 1 prints `{W} 1, {G} 3` on a hand with three Pathways whose other faces make {W} and
{U}; deck152 played those other faces 7 times this corpus. Same shape as the D2 fix, one card class
over. Say them as alternatives, not additions: `{G} 3 ({G} 2 of them are Pathways that can instead
make {W} or {U} - one face each, not both)`.

## R-5 (LOW) - the take narration should use the head the row printed

`- You used: grimclimb pathway with Brightclimb Pathway` appears in 422 prompts / 684 occurrences.
`chosen_text` is already clean, so only the write seam is left. This is a prompt-quality ask, not a
bug: the log is 55-58% of the mean prompt at my seats and it is teaching the pilot a card name that
does not exist on any card.
