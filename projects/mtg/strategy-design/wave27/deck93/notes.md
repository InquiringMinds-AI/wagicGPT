# deck93 notes (engine / harness / representation ledger) - Step-0 debut

Layer-routing: these are NOT prompt-text items. Engine/representation -> ledger.

## N-A (REPRESENTATION, MODERATE) - basic lands render with no land/mana tag; pilot
miscounts them at the mulligan.
Surface: hand + mulligan render. Basic lands render as bare "Swamp" (no type/mana
annotation), while spells carry "{1}{b}{b}". The pilot mulliganed the identical 3-Swamp
opening 5/6 games claiming "zero lands" while holding three Swamps (findings L1, verbatim
repros: vs22/vs134/vs137/vs131/vs18 seq1). It kept the same hand correctly once (vs136
seq1) - so it is a stochastic land-count misread, not a stable belief. vs18 seq1 proves
the model KNOWS "Swamps produce black mana" yet still wrote "zero lands" - so this is
partly reasoning/sampling, not pure representation.
OPTION (not a mandate): annotate basic lands with a land/mana tag at the hand + mulligan
surfaces, e.g. "Swamp (land: taps for {B})" or a trailing "[land]" marker, so the pilot
cannot miscount lands as spells at the mulligan. This is the highest-leverage surface (the
mulligan decides games) and connects to the standing ledger item "mulligan surface 4
corpora unexercised" and the cross-phase deciding-fact rung. Guide (strategy.txt Rule #1)
is the FIRST lever this wave; this annotation is the representation hedge if the guide
alone does not stabilize the mulligan. LOW-RISK: purely additive to the hand render.

## N-B (REPRESENTATION, LOW / below-bar) - no summoning-sick marker on the board render.
[tapped] instances show "[tapped - untaps and can attack next turn]", but a creature cast
THIS turn (summoning sick, cannot attack) has NO marker on the board line. The pilot
conflated board-count with attacker-count and declared phantom attackers (vs134 seq23: 4
Rats on board, replied A1..A4, only A1/A2 legal). The A-lines are authoritative and were
correct, and the engine dropped the illegal slots (no mis-bind), so this is below-bar. An
optional "[summoning sick - cannot attack this turn]" tag on newly-cast bodies would close
the model's math gap. Guide line (strategy.txt COMBAT) covers it for now. Do not raise
above LOW unless a second seat shows phantom-attacker math costing a game.

## N-C (POSITIVE / no action) - d1 R-DUPLICATE-NAME-INSTANCE validated at the stressor seat.
Recorded so the ledger can mark d1 discharged at deck93: 25/25 battlefield renders
contiguous #N, 26 A/B lines carry handles, model uses #N in reasoning, every binding hit
the intended-legal instance (incl. the seq20 vs136 tapped-exclusion + hallucinated-B2/B3-
drop stress case), no handle instability, negligible prompt-size impact (36 names in the
cached system prompt; per-decision prompts ~1KB; replies shorter than deck137). No engine
work needed for d1 from this seat.

## Deck construction (no action) - the deck (24 Swamp / 36 Relentless Rats) is sound: 40%
land supports the {1}{B}{B} double-black cost; the losses trace to pilot mulligan/combat
leaks (findings L1/L4), not construction. Do not alter the deck list.
