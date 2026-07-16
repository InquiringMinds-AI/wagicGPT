# General suggestions (cross-seat / synthesis) — deck135 wave-16

Routed UP to synthesis; not deck135-guide changes. Ordered by value.

## 1. HEADLINE (engine) — Force offer-legality has a SECOND payability hole: the HARDCAST mode is offered while unpayable, then silently fizzles. Distinct from wave-15's #1b free-alt-cast fizzle; same root cure.

deck110 s8/s16/s25: `Cast Force of Negation {1}{u}{u}` was OFFERED three times when deck135 held only
`{g}` (or `{g}{u}{c}` — one U short) AND had no blue card in hand to pitch. The model selected it each
time; a `deferred_to_heuristic` step could not complete the unpayable cast; **Force never reached the
stack** (confirmed by zero `hand -> stack` in both seat logs). Wave-15's #1b was the *free alt-cast*
fizzling when tapped out; this is the *hardcast* offered without a mana check. **Both are the same
underlying gap: the offer-legality gate does not verify the caster can actually PAY the listed cast
(mana for hardcast, or a legal blue card for the exile alt-cost).** Cross-seat cure: before listing any
Force cast option, verify payability of THAT mode; if neither mode is payable, do not offer Force.
Every Force-carrying seat that gets offered Force while short its mana/pitch will hit this. See
deck135 notes.md ENGINE #1c (+ #1b carried).

## 2. Force reflexive-cast drift is WORSENING model-wide — now firing on FACE BURN, the guide's hardest "never" line.

Wave-15 flagged the swing from disciplined-decline to reflexive-cast (over-Forcing Cranial Plating).
This corpus it got worse: deck135's 4 Force casts were ALL bad targets — deck21 s19 Forced Volcanic
Hammer aimed at its own FACE (exiling Ice-Fang Coatl, a creature it needed, then dying the same turn
anyway), and deck110 x3 Forced Galvanic Blast (face burn) / Cranial Plating (equipment). The guide
states verbatim "NEVER Force a burn spell pointed at your own face." The model violated it while
CITING the free-alt-cast mechanic it half-understood. The guides already scope Force correctly, so this
is model over-application, not under-teaching — but if other Force-carrying seats show the same drift
onto face burn / marginal noncreature spells, it is a model-behavior regression worth a synthesis note
and a candidate for the model-experiments bucket.

## 3. Card-color / fetch-color hallucination cluster — the model misreads static card facts, and it now touches Force legality.

Two clean repros this seat: (a) deck110 s8/s16/s25 — the model calls **Into the North (a green {1}{g}
sorcery) a "blue card"** it can exile to the Force alt-cost (there is none in hand). (b) deck44 s18 —
the model believes **Flooded Strand can fetch a Snow-Covered Forest** (it fetches only Plains/Island).
These are the same family as the wave-15 damage-vs-toughness gap: the model invents/mis-recalls a fixed
card property to justify a play. Here it specifically corrupts Force decisions (imagining a legal exile
target) and fetch decisions. Cross-seat model-experiments item; not guide-fixable per deck (the facts
are in the option text the model overrode). Track alongside the damage-vs-toughness gap.

## 4. Latency-adjudication was NOT the story this corpus (deck135 seat) — the seat died for real.

Unlike wave-15 (2 adjudicated games, one a favorable coin-flip), all 5 deck135 losses this corpus were
REAL deaths (-3/-4/-4/-7/-9), three by turn 8/9/11. The brief's corpus-wide "only 4 life-adjudicated,
down from 9-13; games completing" is visible at this seat as: the deck is simply being beaten on the
board, not robbed by the tiebreak. Corpus-wide item (brief owns it), but the seat-level datum is that
adjudication noise is receding as games complete — read the 1/6 as construction, not tiebreak luck.

## 5. Construction ceiling restated (user owns decklist) — vs44 evasive-6/6 loss is now 3 corpora running.

Not new, but it is the dominant driver of the 1/6 and worth keeping visible to the decklist owner: the
2-Island / 2-Plains + fetch splash color-screws the deck's own {g}-requiring gold creatures (vs21: 0
creatures cast, green-screwed, dead T9), and nothing in the 75 answers an evasive snowballing flyer
(vs44 dead at -4 with SIX creatures deployed — a near-identical repeat of wave-13 and wave-15's vs44,
now three corpora). Direction (user's call, restate not relitigate): raise effective double-pip green
sources, and/or add an evasion/flyer answer.
