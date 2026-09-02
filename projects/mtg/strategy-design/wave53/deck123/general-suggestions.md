# Core-prompt / render proposals from the deck123 seat (wave 53)

Layer-routed. Engine defects with repros are ranked in `wave53/seats/seat-123-130.md`; this file
carries the proposed TEXT and the prompt-layer asks only.

## G1 (core prompt + emitter) - bound the PLAN capture, and say whose text it is
**What happened:** 123v130 s57's reply put `PLAN:` in front of ~2,000 characters of live
deliberation ("Wait, I need to act NOW. I am in Main Phase 2 ... Vampire died. So Morbid applies
... Rorix ... dies"). Everything after `PLAN:` was captured, and the whole block was replayed as
`YOUR PLAN (as you last stated it, 1 window ago on turn 17)` at s63 and s64, where the phase
header said Upkeep and Draw and the row said Morbid does NOT apply. The pilot acted on the plan.
**Ask (emitter):** capture the PLAN at the first blank line or the first sentence that starts a
new line without a connective - one paragraph, not the tail of the reply. A reply's second
paragraph is thinking, not a plan.
**Ask (prompt text):** the header already stamps the age; add the ownership, e.g.
`YOUR PLAN (your own words, N windows ago on turn T - the board lines below are now)`.

## G2 (core prompt) - the coded line is demanded before the reasoning exists
**What happened, four times at this seat:** 123v125 s83/s116/s152 wrote `CHOICE: 1 (Cast
Damnation)` and then reasoned, inside the same reply, to the opposite conclusion
("Casting Damnation is a waste of mana. I should cast a creature"); a second coded line saved all
three (`answer_replaced`, correct 3/3). At 123v125 s84 the same reasoning did not produce a second
coded line and the dead Damnation resolved. 123v126 s22 is the mirror: a clean single coded line
with no reversal in it was flagged `decision_reversed_in_prose` and re-asked.
**Ask:** allow the reply to open with one short `THINK:` block ABOVE the `CHOICE:` line (bounded,
e.g. 600 characters, stripped before narration), or state explicitly that the LAST coded line
wins and the model may correct itself. Either removes the structural incentive to commit before
counting. Cheaper variant, no format change: keep the format and treat the last coded line as
binding (already true) but say so in the reply rules.

## G3 (render) - fetch rows name a land and never its colours
**What happened:** 123v130 s36. The Marsh Flats menu printed seven rows of the form
`Put a card onto the battlefield with Marsh Flats targeting Underground Sea [your library]
[cost: Tap, Life, Sacrifice] {this land makes no mana - crack it for a land: your untapped mana
sources go from 2 to 3}`. The clause counts SOURCES; nothing on any row says Tundra makes {W}{U}
and a Swamp makes {B}, while the hand held two {2}{u} spells and the mana line read `{b}{w}`.
**Ask:** continue the existing clause with the colours, using the words the mana line already
uses: `... sources go from 2 to 3, and it adds {W} or {U} (you cannot make {U} right now)`. The
land's colours are already known to the emitter that prints `(land: taps for {W}{U})` in the hand
line.

## G4 (render) - the non-lethal blockers hint asserts a condition it has not checked
**What happened:** 123v130 s55, at 7 life against 12, the blockers header printed
`... - NOT lethal: block only where the trade favors you; taking damage while ahead on LIFE is
often correct`. The pilot was BEHIND on life. `BLOCKS: none`, to 1, dead next turn.
**Ask:** gate the second clause on the two totals the emitter already holds - print it only when
`my_life > opp_life`, and otherwise print nothing (the guide owns the rest). A general hint that
states a false premise is worse than no hint.

## G5 (render, cheap) - the HOLD row does not say what it costs
249 HOLD rows offered at this seat, one taken. The row says what it does
(`do not ask me again this turn unless the board changes`) but not that it is free - that a board
change re-opens the window and no cast is given up. **Ask:** append the guarantee to the row:
`Hold priority - do not ask me again this turn unless the board changes (any change re-opens this
window; you give up no cast)`. One clause, on the row where the decision is made.
