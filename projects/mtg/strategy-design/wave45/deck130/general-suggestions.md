# deck130 - core-prompt / render proposals (wave 45)

Layer-routed; engine items are in the seat file (M1-M3, L4-L5).

## G4 (render, and the one I would ship first) - price the DEGENERATE X row like any other.
The wave-45 X-pricing work is a clean pass at this seat: both of its ANNOUNCE_X windows carry
`{X pricing: ...}` on every row including the collapse form `{X pricing: same as X=2}`, and
plan-vs-chosen divergence fell to 1 of 14 corpus-wide with the one divergence costing a single
mana. What the annotation still does not do is make a NULL cast look null. At
`...deck130-...-vs-ai_baka_deck123.jsonl` seq 21 the three rows read
`X = 2 {kills THEIRS: none; YOURS: none}` / `X = 1 {same as X=2}` / `X = 0 {same as X=2}` -
truthful, and read by the pilot as "X=0 is the cheap way to spend the card". A row that kills
nothing on either side is not a choice between numbers, it is a spell that should not be cast.
Proposal: when NO listed X kills anything on either battlefield, say so once at the top of the
menu in the same voice the cast row already uses - `no value of X on this list kills anything;
casting this spell has no effect` - and, separately, on the CAST row for a 0-effect X spell keep
the existing `there is no creature on the battlefield for it to damage` clause (it was present
and correct here). Both halves cost one line and neither removes the legal choice.

## G5 (render) - the modal menu could say which row disposes of the card and which uses it.
"Choose an option for Starstorm:" lists `1. Cast Card Normally` / `2. cycling ... [cost: {3},
Cycle]` / `3. Decline - do nothing`. The cycling row already carries its cost and its card text
("{3}, Discard this card: Draw a card."); "Cast Card Normally" carries neither an outcome nor a
pointer to the X menu that follows it. One trailing clause would remove the confusion the seat
actually had - `1. Cast Card Normally [you will be asked for X next]`. This is the same class as
the ANNOUNCE_X preview: a menu step whose consequence is fully known and not shown.
Repro: file above, seq 20.

## G6 (render, LOW) - the "held back" tag is one-sided.
`[held back, it CANNOT block ANY of their 1 creatures: Master of the Feast (flying)]` on
deck130's Blastminer row tells the seat what keeping a body home does NOT buy. There is no
converse clause telling it what keeping a body home DOES buy on a board where the answer is the
whole game: at `...vs-ai_baka_deck146.jsonl` seq 21/25 the attack list was Rorix alone, the
opponent's only creature was a 4/4 with vigilance, and nothing on the attackers screen mentioned
that Rorix is also the only blocker or that the blockers screen's trade for that pair is "you
kill it, your blocker lives". Proposal, symmetric with the existing tag and computable from the
same data: on an attack row for the seat's ONLY untapped creature, append
`[if you keep it home it can block: Nadaar (4/4) (you kill it, your blocker lives)]`.
I have written the arithmetic into the guide (EDIT 45-2) because a guide can compute this from
what is already printed; the tag would make it cheaper and pool-wide, which is why it is here
and not only there.
