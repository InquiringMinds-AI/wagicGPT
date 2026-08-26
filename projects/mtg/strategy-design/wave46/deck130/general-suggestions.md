# deck130 - core-prompt / render proposals, wave 46
Engine BUGS and render items with repros are in `wave46/seats/seat-123-130.md` (H1, H2, M1, M3,
L1-L4). This file holds the core-prompt / shared-frame proposals only.

## G46-5 (from ENGINE H1) - the board frame names one hidden cost class and should name the other.
CURRENT SITUATION already carries `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine
Bond. While it is in play, life ITS CONTROLLER gains also makes the other player lose that much
life...` - a summary of a mechanism whose individual facts are scattered across battlefield
entries and whose COST lands on an action taken somewhere else entirely. The draw-punish class is
the same shape: Underworld Dreams, Fate Unraveler and Ob Nixilis each print their own
`{effect: "Whenever an opponent draws a card, ... deals 1 damage ..."}`, and the cost lands on a
`cycling` row that says nothing about drawing. deck130 read all three, said so in its plan, and
cycled itself from 3 to 0.
PROPOSAL: a `DRAW PUNISHERS on the battlefield:` line built by the same scan-and-name pattern,
with the summed per-draw cost, in CURRENT SITUATION so it reaches every decision kind - including
the priority windows where the cycling rows actually appear. `@drawfoeof(player):damage:N` is
scriptable and summable, so the number is claimable; where a punisher's amount is not exactly
computable, name the permanent without the number rather than guessing one.
SECOND, CHEAPER HALF: append the cost to the row that causes it, the way the mana line appends
the pain-land caution - `cycling with Starstorm [cost: {3}, Cycle - drawing costs you 3 life
right now]`.

## G46-6 (from ENGINE H2) - every menu in the loop is annotated except the modal-choice menu.
`Choose an option for Starstorm:` renders `1. Cast Card Normally` / `2. cycling` / `3. Decline -
do nothing`. The cast row one screen before it carries `{X pricing: ...}` and `{card text: ...}`;
the announce menu one screen after it carries a pricing annotation per row. This menu carries
nothing - not the alternative cost, not the effect, not the draw.
PROPOSAL: annotate both rows from what the engine already parsed - the alternative cost from
`autohand=__CYCLING__({3})` and the effect from the card's own `auto=` line:
`1. Cast Card Normally {2}{r}{r}{x} {casting it: Starstorm deals X damage to each creature; draws
nothing}` / `2. cycling {3} {discard Starstorm and draw 1 card}`. Restriction-first is not the
register here for the same reason the GANG BLOCK clause is not: this is a cost the pilot is about
to pay, so lead with the outcome.
This is the single highest-value prompt change I can name from this seat: it is the only menu
where the pilot has repeatedly chosen the row it did not want, across two corpora, while quoting
the rule that says which row it wanted.

## G46-7 (from ENGINE M1) - an annotation that states only the MINIMUM reads as advice to
## underspend.
`{X pricing: max affordable X=3 (5 mana total); smallest X that kills an OPPONENT creature: X=1
(3 mana total) - kills THEIRS: Human; YOURS: none. At X=3 - kills THEIRS: Thraben Doomsayer,
Human; YOURS: none}`. Both halves are true. But the SMALLEST is a named, labelled quantity with a
clause of its own, and the largest is an unlabelled trailing sentence; and on the announce menu
the collapse wording (`same kills as X=N, for K less mana`) frames mana saved as the only axis
between rows, with nothing at all marking the row that kills the most.
PROPOSAL: give the maximum a label in the same shape as the minimum - `best affordable X: X=3 -
kills THEIRS: ...; YOURS: none` - and mark the largest-kill row once on the announce menu
(`<- most kills at any affordable X`). Neither adds a claim; both promote an existing fact to the
same salience as its opposite. The wave-45 lane-C work established that these facts are exactly
computable for a damage clause, so no new arithmetic is needed.

## G46-8 (from ENGINE M3, and offered as a general rule) - never put a non-colour token inside a
## list the prompt calls "colours".
`colours you can make: {1}{g}{r}` and `Talisman of Impulse {x} or {r} or {g}`. `{1}` is a generic
AMOUNT; `{x}` is a variable marker. Neither is a colour, and both appear inside a clause that
says the word "colours". The same card renders `{c}` in the same game a few windows later.
PROPOSAL, beyond the specific fix: the mana frame should have ONE symbol per concept, and the
colourless mana a source produces should be `{c}` everywhere it appears. A prompt whose own
vocabulary drifts between three spellings for one thing costs more than the character it saves,
and the mana line is the surface with the longest history of misread arithmetic in this project.
