# deck162 -> core-prompt / render proposals (wave 56), layer-routed

## R-8 (MED, render) - `hold_windows_skipped` should have a companion on the PROMPT side
EVIDENCE. The declined-note literal `[you declined this exact list N times already this turn]` is
the whole reason deck162's window economy collapsed from 65 N>=2 windows to 0. It renders on 877
corpus prompts / 90 of mine. But a seat only learns its own N; the gameend field
`hold_windows_skipped` (9 on `162v125`, 0 elsewhere at my seats) is the only place the SAVING is
counted, and it is invisible to the pilot and to the guide author.
ASK: no change to the pilot's prompt. Emit the running per-game total to stderr the way the hold
line already does (`AIPlayerGPT: holding priority at the priority seam (... N windows held this
game)` fires 282 times in my games and is exactly the right shape) - the ask is only that the
SKIPPED count get the same treatment, so a reviewer can cross-tab holds against savings without
reading gameends.

## R-9 (LOW, render) - `reveal_stall` is gone and `reveal_wait_ticks/_secs` replaced it: confirmed
EVIDENCE. Wave-55 E-5 asked for this. This corpus has **0 `reveal_stall` fields** and
`reveal_wait_ticks` + `reveal_wait_secs` on **9 of 9** reveal records. Lane C's D12 prediction
PASSES. Recorded, no ask.

## R-10 (LOW, prompt) - the `{feeds:}` clause's `converters on your battlefield: N` is the single
best decision surface this deck has, and its counterpart is missing on the HAND line.
EVIDENCE. Every K-of-0 judgement this deck makes needs two numbers: converters on the battlefield
(printed) and converters IN HAND (not printed - the pilot must scan its own hand line for three
card names, and at `162v125` s15 it got that scan wrong by counting Forced Fruition).
ASK: extend the clause to `{feeds: ... converters on your battlefield: 0; in your hand: 1}` - the
same computation over the hand zone. It is the discriminator the guide's release condition is
written against, and it would let the guide drop its card-name list entirely.
