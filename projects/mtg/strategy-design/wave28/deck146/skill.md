# deck146 skill proposals (wave 28) -- campaign-first surfaces: venture/dungeon + loyalty

STEP-0 GUIDELESS DEBUT. First seat in the campaign to exercise VENTURE/DUNGEON and the second
to exercise planeswalker LOYALTY (Lolth; Kaya unexercised). These are new option-render faces.
All proposals tagged SINGLE-SEAT (first venture deck, first WB-planeswalker deck) unless noted.

## Proposal 1 -- NEW option-render FACE: the DUNGEON-SELECTION vs ROOM-BRANCH split (venture)
Venture surfaces THREE distinct render faces, and they are inconsistent in a way that matters:
  (A) Dungeon SELECTION (first venture) renders as a chooseTarget ("TARGET CHOICE for <source>
      ... pick the ONE target") listing the three dungeons WITH truncated room text.
  (B) Room BRANCH advancement renders as a bare "choose an option" menu with ROOM NAMES ONLY
      and NO effect text (deck134 seq36: "1. veils of fear / 2. oubliette").
  (C) Room EFFECT modal renders the effect AS the option text (deck134 seq38: "1. lose 2 life
      / 2. discard a card") -- clean.
The teaching point for the WRITING skill: **face (B) is the actual strategic venture decision
(which path through the dungeon) and it ships with zero deciding-fact on the option.** This is a
DECIDING-FACT-RIDES-THE-OPTION gap (the standing P1/P4 principle) at a brand-new seam. The fix is
REPRESENTATION-LANE (put each room's effect text on the branch option, same as (C) already does
for the effect modal) -> routed to notes.md as an engine/render item, NOT guide prose. The guide
can only give a generic branch heuristic ("prefer a room that makes a body/damages them over one
that only scries") because it cannot know which branch names will appear. AUTHORING RULE for any
future dungeon/menu-tree deck: if the branch face lacks effect text, the guide gives a generic
value-order heuristic and the real fix is a render annotation, never per-room guide prose (there
are too many rooms across three dungeons to enumerate).

## Proposal 2 -- NEW authoring face: DUNGEON CHOICE is an OPTION-1-BIAS trap keyed to a payoff
The model defaulted to the first-listed dungeon (Tomb of Annihilation) in BOTH games it chose
(deck134 seq25, deck22 seq11). Two forces stack: the standing "model favors option 1" and the
deck-file dungeon order (Tomb first). But the CORRECT default for a completion-payoff deck is the
SHORTEST dungeon (Lost Mine of Phandelver, option 2), because completion is what turns on the
deck's anthem/reanimation. **Authoring rule: when a deck has a menu whose best pick is NOT
option 1 AND the pick keys a downstream payoff (here: dungeon completion -> Nadaar anthem +
Barrowin reanimate), the guide must NAME the correct option explicitly ("pick Lost Mine, usually
option 2, NOT option 1") and state the payoff that makes it correct.** This is the same shape as
the deck134 Expedition-Map "take option X, here is why" rule and the deck137 "prefer the reliable
variant" companion pattern -- a guide selecting among offered legal options against the option-1
prior. Single-seat; promote to a general "named-option-against-the-option-1-prior" face if a 2nd
menu-tree deck reproduces the default-to-option-1 miss.

## Proposal 3 -- LOYALTY menu renders WELL; the teach is "activate every turn," not render
Lolth's loyalty menu (deck134 seq21) renders cleanly: "+0: draw card and lose life with Lolth
[cost: Counters] {card text ...}" / "-3: create spiders with Lolth ...". The +/- prefix, effect
gloss, source name, cost tag, and full card text all present -- no render gap. The observed leak
is DECISION (passed free Lolth +0 on 2 whole turns: seq26/28/30, seq48), not representation. So
the LOYALTY authoring lesson is a GUIDE line ("a planeswalker that does nothing on your turn is
wasted -- activate an ability every turn it is out; -X impact abilities first, then the +N/0
value ability -- do not pass with an idle walker"), NOT a render change. This mirrors deck134's
"do SOMETHING every turn" and the standing "state the loop as a per-turn sequence" rung
(method-headline-8a): a static "you have a planeswalker" line does not produce activations; a
per-turn activation ORDER does. Single-seat (first loyalty-active deck); the loyalty +/- discipline
line is a promotion candidate on a 2nd planeswalker deck.

## Proposal 4 -- Kaya UNEXERCISED: the "carry unexercised, do not certify clean" discipline holds
Kaya the Inexorable was drawn in 3 games and CAST in zero -- her loyalty menu never fired. Per
the standing surface-unexercised-carry discipline (wave-25 headline), do NOT read her render or
decision quality as validated. Lolth confirms the menu SHAPE, so the guide can teach Kaya's
abilities from Oracle (her -3 exile-any-nonland is the key removal line), but her specific
plus/minus/ultimate DISCIPLINE is a first-exercise item for a future corpus. This is a scope-
honesty note for the synthesis agent: the loyalty face is HALF-validated (Lolth yes, Kaya no).

## Negative confirmation (scope honesty)
The #1 STEP-0 leak (model cast nothing in the two blowout losses because it received ONLY
land-drop asks, never a cast/priority window -- deck18/deck93) is an ENGINE item, not a guide or
skill teach. Per method-headline-3 (classify tax as engine-caused before it ranks as a teaching
point) and the standing engine-vs-fact filter: a model that is never ASKED to cast cannot be
taught to cast. It routes wholly to notes.md; the guide's removal/deploy prose only converts the
windows the model actually receives. Do NOT let this loss column drive guide inflation.
