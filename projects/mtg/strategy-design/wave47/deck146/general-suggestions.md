# deck146 seat - core-prompt / render proposals (wave 47)

Layer routing: everything an ENGINE has to fix is in the seat file's engine list (E-1..E-6).
This file holds only proposals about the CORE PROMPT's own text and the shape of what it renders.

---

## G-1 (HIGH) - the collapsed blocker tag selects its representative by BODY SIZE; the decision it
## feeds is about PRICE.
Current string: `[their untapped blockers: they have 5 untapped creatures that could block this
one, biggest Vampire (1/1) (you kill it, your attacker lives (lifelink: they gain 1, and their
converter takes 1 off you))]`.
At `146 vs126` seq 50 the four blockers the collapse dropped were `Perimeter Captain (0/4)`
(gains 2 for EVERY defender they block with), `Pride Guardian (0/3)` (gains 3), and two
`Wall of Omens (0/4)`. The one it kept - the biggest body, a 1/1 - had the SMALLEST price on the
board. The model priced the swing at 1 and paid 11, and the game was lost from a winning position.
"Biggest" is the right selector for a fight-outcome question and the wrong one for a price
question, and lane K's converter fold has just made the price the load-bearing half of the tag.
**Proposal (restriction-first, no surface deleted):** keep the biggest-body representative and add
a second, price-selected one whenever the hidden set contains a priced blocker the printed one
does not represent - e.g. `... biggest Vampire (1/1) (...); most expensive to attack into,
Perimeter Captain (0/4) (neither dies (blocking trigger: they may gain 2, and if they do their
converter takes 2 off you))`. Or, cheaper: append the summed worst case, `[if they block with
every creature on that list, they gain up to N and your converter loss is N]`. Either is a true
token; today the tag's silence reads as "there is nothing else to pay".

## G-2 (MED) - the cast ask and the ability/priority ask are two decisions and the model answers
## them as one.
`146 vs125` seq **35** offered `1. Cast Soul Shatter / 2. Cast Silverquill Command / 3. Cast
Acererak the Archlich / 4. Cast Pelakka Predation / 5. Cast nothing right now` and the model chose
5. The very next record, seq **36**, same turn and same phase, is a `priority` ask whose only rows
are `1. becomes beholder ... / 2. Flip Side ...` - and the reply is **`CHOICE: 8 (Cast Acererak
the Archlich)`**, a number that is out of range on this menu and a card that is not on it. It was
the corpus's only `unparsed_reply` on my seats.
The model is treating the two consecutive menus as one continuous list and is answering the
earlier one late. Two candidate prompt-side fixes, both restriction-first: (a) when a `priority`
ask directly follows a cast ask in the same phase, say so on the ask - "you have already answered
this turn's casting question; these are ACTIVATED ABILITIES only, and no card in your hand appears
below"; (b) state the range explicitly - "answer with a number from 1 to 2, or 0".
Note this is a prompt-composition proposal, not a parser proposal: the parser was right to refuse.

## G-3 (MED) - `Cast nothing right now` is the most-taken option on this seat and the ask says
## nothing about what it costs.
20 of 71 cast menus. Every other option row on the screen carries a consequence annotation
(`{leaves 4 of your 7 untapped mana sources untapped}`, `{right now: returns NOTHING - your
graveyard has no creature cards}`), and the one row that means "spend nothing and change nothing"
is bare. **Proposal:** annotate it the way the land-drop ask was annotated by R13 - e.g.
`Cast nothing right now {you keep N untapped sources you cannot carry into your next turn}`, or
on a main-2 window `{this is your last casting window this turn}`. The true token exists (mana
empties at end of turn) and the deck's guide already argues it; the render currently makes the
free-looking answer look free.

## G-4 (LOW) - the mode-chooser header still names no card, and the emitter class is now known.
See seat-file E-2 for the engine item. The prompt-side observation: the exemplar change works
(all four `A choice is required` menus this corpus were answered in the option's own vocabulary,
`CHOICE: 2 (creature gains 3/3 and you draw)`, and produced **zero** fallbacks against 2 of 4 last
corpus), so the residual is purely the missing carrier name, not the answer format.
