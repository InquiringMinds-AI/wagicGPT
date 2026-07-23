# Deck-135 wave-21 — general suggestions (cross-deck / model / corpus; NOT guide text)

Routed OFF the guide per layer-routing doctrine. deck135-observed but general in nature. Win column is
context-only; the core prompt is symmetric.

## #1 (ENGINE, DOMINANT — the corpus's top new finding) — FETCHLAND ACTIVATIONS INTERMITTENTLY FIZZLE.

The single highest-leverage item this corpus. A fetchland `Put in Play` / `search basic land` activated
ability (cost Tap+Life+Sacrifice) intermittently pays Tap+Life then ABORTS before the Sacrifice+search:
no land enters, the fetchland stays, the option re-arms, and the model (unable to perceive the silent
failure) believes it succeeded and stops re-cracking. This is the item-8(c) class the wave21
item8-diagnosis left OPEN, now caught cleanly on fetchlands (vs35 s6 verbatim repro in notes ENGINE-F1).
It is the primary driver of deck135's 4/6 -> 1/6 regression because deck135 is the pool's most
fetch-dependent deck. Two coupled fixes (both engine, notes ENGINE-F1/F1b):
  (a) verify the Sacrifice-cost sub-click lands; on refusal, roll back the already-paid Tap+Life (never
      leave a partial cost paid), or complete the sacrifice / re-resolve the ability by source card.
  (b) emit an engine event on a fizzled/aborted activation ("activation cancelled — no land fetched") so
      the model can perceive the failure and re-crack (the guide already tells it to). Without (b) the
      model narrates success in its plan and passes forever.
This affects EVERY fetch-based deck, not just deck135 — deck135 is just where it manifests hardest and
is measurable.

## #2 (ENGINE representation, NEW) — annotate CONDITIONAL TRANSFORM options with their eligibility, like reveals.

The Azcanta `Transform:...` option is offered only when Search for Azcanta's 7-cards-in-graveyard
threshold is met, but the option carries NO annotation saying so. The model fills the vacuum with an
error-prone manual graveyard recount, gets it wrong, and DECLINES a beneficial available transform
(vs14 seq 29 + 40, notes ENGINE-R6) — a real misplay in a grind it lost. Extend the reveal
eligibility-surfacing pattern to conditional-transform / threshold-gated options:
`Transform:Azcanta [available now — 7+ cards in graveyard]`. General fix for any threshold-gated ability
menu (delirium, threshold, metalcraft, ferocious, etc.). Same family as the shipped reveal marks.

## #3 (HARNESS parser, NEW) — the retracted_choice route over-fires on downstream reconsideration tokens.

The wave-21 retracted_choice fallback discards a single un-superseded, VALID CHOICE line when a
retraction-shaped token (`Correction:`, `Wait,...`) appears LATER in the reply about an unrelated point
(combat math, card mechanics) — vs27 s26 threw away a Rule#1-correct `Cast Ice-Fang Coatl` over a
`Correction:` about Islandwalk (notes HARNESS-1). Only route to heuristic when the retraction targets the
CHOICE/decision itself, or when a later CHOICE line supersedes; otherwise HONOR the single un-superseded
CHOICE. This is the inverse over-swing of the wave-20 HARNESS-1 lock-in; the two must be balanced by
"retraction must reference the choice."

## #4 (MODEL / DECODE, carried) — decode repeat-loop spirals persist; answer-first fixed PARSING, not the loop.

Answer-first (item b) is a real win at the parse layer: 0 unparsed_reply this seat (7 in wave-20), coded
line first in 100% of p95+ replies, and "last-CHOICE-wins" preserves reconsideration (vs49 s26, vs62 s9).
BUT the underlying >12k-char verbatim-repetition spiral is unchanged and still degrades decisions INSIDE
a clean parse — the Azcanta miscount happened in a 15k-char spiral (vs14 s29), and the HARNESS-1
false-positive rode a 13k spiral. The real fix is a decode-time repetition-penalty / max-token guard
(repetition_penalty was OFF this corpus per brief item j — turning it ON is the direct test). Model/decode,
NOT guide-fixable.

## #5 (METHOD, cross-deck — reinforces wave-20 #4) — a win-column swing on a FROZEN guide + UNCHANGED slate can be a pure ENGINE-BUG-x-VARIANCE footprint. Decompose before attributing.

deck135 fell 4/6 -> 1/6 with a byte-identical guide against an identical opponent pool. The naive read is
"the deck/guide/model got worse." The evidence read: an intermittent engine bug (fetchland fizzle) that
existed in BOTH waves happened to strike more of deck135's games this corpus; the only win is the game
where fetches resolved. The repeated-opponent control makes this decomposition airtight — same as the
wave-20 caution that a validated engine fix contributed ~0 to the win column. Cross-deck rule: when a
frozen-guide seat's record swings, first check whether an open ENGINE bug's incidence changed across the
games, before crediting/blaming guide, model, or draw.
