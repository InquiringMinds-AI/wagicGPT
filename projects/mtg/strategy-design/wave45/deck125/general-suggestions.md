# Core-prompt / render proposals — deck125 seat (wave 45)

Layer routing: engine DEFECTS are in the seat file's engine list, not here. These are core-prompt
and render-composition proposals.

## G-A (core prompt, MED) — the plan-mismatch note fires on 58% of casting windows because these guides mandate a doctrinal PLAN line

Measured on this seat: the note fired 124/337 records (36.8%), and **0/152 land-drop windows**
(wave 44: 137/140 — the #W45-4 land-drop half is discharged on this seat). Precision is clean:
across all 164 fires on both my seats, **0** had a card named in the pilot's own PLAN text present
in that menu. So the predicate is TRUE every time it fires.

The residual is a composition effect, not a bug. deck125's guide orders the pilot to write the plan
"Answer their threats, resolve Staff of Nin, ping their face every turn" *word for word*. That
sentence names one card. Every menu that does not offer Staff of Nin therefore trips the note —
118/156 casting windows — while the pilot is in fact executing the plan (a Path to Exile IS
"answering their threats").

Proposal: the note is doing no harm and is honest, so do not gate it harder on a heuristic. Either
(a) measure the rate per SEAT against whether that seat's guide mandates a fixed plan string, and
report the two populations separately, or (b) soften the note's second sentence when the plan
contains no card name at all — "your plan names no specific card, so this note is only telling you
this menu is not the Staff/Revelation window" — so a true-but-uninformative fire reads as
uninformative. Not urgent; flagged so a future wave does not read 31% as a regression.

## G-B (render, MED) — `Mana available:` prints a colour SET but never a per-colour COUNT, while the opening-hand render already prints one

`manaAvailableLine()` (AIPlayerGPT.cpp ~2415) emits
`N total (... colours you can make: {g}{u}{r}{b}{w}; mana of ANY colour pays a generic cost like
{2})`, plus a per-source sub-list. `pregameHandHeaderText()` (~1326) already emits
`Mana sources among those lands, counted by the engine: {W} 2, {U} 2` — the exact count shape,
in the pre-game render only.

A pilot deciding whether it can cast a {3}{B}{B} spell has to derive the black COUNT by parsing the
per-source sub-list. It got that wrong in a game-deciding way on the deck126 seat (see the seat
file's HIGH item). Proposal: carry the pre-game shape onto the in-game line —
`colours you can make: {g}{b}{w} (sources that can make each: {G} 3, {B} 1, {W} 2)`. The data is
already in hand at that call site (the sub-list is built from it), it is count-first in the style
`manaAvailableLine`'s own comment (N-158g) argues for, and it removes an inference the render is
currently asking every pilot to perform.

## G-C (render, LOW) — the ANNOUNCE_X menu still shows no `ON THE STACK` section and no `Your hand:` delta

Confirmed again, 12/12 windows: the X-announce prompt carries the full CURRENT SITUATION block —
including `Your hand:` with the Sphinx's Revelation STILL LISTED — but no `ON THE STACK` line, so
nothing on the page says the spell has left the hand. The guide works around it by folding the
correction into the constant (X = 8 minus the hand line rather than 7 minus hand-minus-one).
Repro: `1787768354-ai_baka_deck125-0x562f799014b0-vs-ai_baka_deck123.jsonl` seq 65 (casting, hand 8
with two Revelations listed) vs seq 66 (X menu, same 8-card hand line, no stack section).
Cheapest fix if it is ever taken: mark the announcing spell on the hand line
(`Sphinx's Revelation ... [you are casting this now]`), which is a one-token edit and makes the
off-by-one visible instead of taught.
