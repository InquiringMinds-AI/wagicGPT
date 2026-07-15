# Deck-135 wave-10 — general (core-prompt) suggestions

Layer-routing enforced. Engine/harness items are NOT here (they are in notes.md). Convergence gate:
single-seat items cannot cut or add core lines on their own; they are PARKED with an explicit cross-seat
promotion trigger.

## OBSERVATION 1 (report PROMINENTLY) — the PARKED core candidate "colored mana can't pay generic / offered = payable" RECURRED at this seat. Still SINGLE-SEAT; cross-seat 2nd deck = promote.

Status carried from wave-9: parked pending a 2nd SEAT. Wave-10 gives a clean within-seat recurrence but
NOT the cross-seat trigger.

REPRO (deck110 s21, T6 MP2, life 19/19, creatures:1): mana `{g}{u}`; options include
`Cast Into the North {1}{g}` and `Cast Search for Azcanta {1}{u}` (both offered = payable). The pilot
reasons it correctly first ("{1} is generic. So I can cast Search for Azcanta"), then reverses into the
FALSE belief and declines both: "I do not have a generic mana ... I cannot pay {1}{u} for Search for
Azcanta because I lack the generic {1}. I cannot cast Into the North {1}{g} because I lack the generic
{1}. I cannot cast any creature because they all cost generic mana. Therefore, I must cast nothing." ->
Cast nothing. This is the identical belief to wave-9's vs140 s24 (declined an OFFERED Astrolabe believing
{G} cannot pay {1}).

Why it belongs in CORE, not a per-deck guide line:
- It is DECK-AGNOSTIC — it fires on any `{N}{color}` cost, on any deck, and it is the SAME FAMILY as the
  general prompt's existing correction ("Mana in your pool: (none)" / empty pool read as "I cannot cast").
  The general prompt already teaches offered = payable + empty-pool-is-normal; it does NOT explicitly
  teach that a COLORED mana pays a GENERIC pip. That is the missing universal fact.
- The wave-9 per-deck patch (deck135 Rule #1 point-3b, scoped to Astrolabe's {1}) is MOSTLY working — the
  pilot got the inference RIGHT 3x this same corpus (deck135 s25, deck133 s10, deck140 s12) — but it
  generalizes poorly (s21 was Search/Into the North, not Astrolabe) and the belief is model-unstable
  (oscillates within one derivation). A 4th per-deck restatement is the forbidden "3rd copy"; the durable
  layer is core.

PROPOSED core sentence (ADD to the existing interface-contract / mana facts, do NOT restate the empty-pool
line): a one-line universal — "Colored mana pays generic costs: any {G}, {U}, {W}, {B}, or {R} you have
each pays a {1}. A cost like {1}{U} or {2}{G} is payable whenever you have its colored pips PLUS that many
total untapped sources; `Mana available: {g}{u}` CAN pay {1}{u} and {1}{g}. If a cast is in your options
list it is payable — never decline it believing a colored mana 'is not generic'."

PROMOTION TRIGGER (for synthesis): grep every OTHER deck's seat replies this corpus for a decline of an
OFFERED cast citing "lack the generic", "no generic mana", "colored mana can't pay generic", or
"cannot pay {N}{...} because ... generic". ANY single instance at a DIFFERENT deck = the 2nd seat -> ship
the core sentence in the next core A/B. If none, keep PARKED at WATCH but note the within-seat recurrence
raises priority (this is now the parked candidate's 2nd within-deck instance across two waves).

## OBSERVATION 2 (NOT a core edit) — the reply-protocol answer-last change is validated; core prompt body unchanged.

The head-first intent-collapse class (a wave-8/9 single-seat/multi-seat harness item) is DEAD under
answer-after-plan (findings Q1). This confirms the reply-protocol A/B outcome; it is NOT a core prompt-body
edit — it belongs to the code-appended reply-protocol layer, which already shipped as this corpus's format.
No action on the core prompt body. Recorded here only to close the loop that the wave-9 "reply-protocol A/B
is DUE" item is now DISCHARGED as adopted-and-working.

## CORE STATUS (deck135 view): PASS — no core prompt-body edit from this seat.
The one core-flavored candidate (colored-pays-generic) is single-seat (same deck, 2 waves) and fails the
convergence gate until a 2nd deck reproduces it — PARKED with the grep trigger above. Everything else
routes to engine (Force, fetch residual), model (uncastable-plan hallucination), or construction (manabase).
