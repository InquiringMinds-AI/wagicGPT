# deck35 — core-prompt (general-strategy) proposals — layer-routed

Single-seat; for the synthesis agent to weigh against convergence. Both are general Magic-literacy /
reasoning-discipline items surfaced by deck35's block-seam blow-up but NOT sliver-specific.

## G1 [CORE-PROMPT candidate — an attacker's damage is its POWER, the first number]
In the 62 s16 unparsed spiral, the model repeatedly read a Saproling "(2/4)" as **dealing 4** —
mistaking toughness for power — producing arithmetic (10+4+2=16) that never reconciled with the
correct "deal up to 14" annotation (10+**2**+2), which convinced it the numbers were wrong and drove
the 11k-char loop. This is not a sliver fact; any deck can misread it. A single flat core line —
"a creature deals combat damage equal to its POWER, the first number in [X/Y]; toughness never deals
damage" — would remove the seed of this failure across all decks. Layer = core prompt (general
rules literacy). Note it also has a REPRESENTATION cure (label/emphasize power in the option line);
see notes.md — synthesis should pick the layer, not both.

## G2 [CORE-PROMPT candidate — when an outcome is FORCED, stop and answer]
The same reply shows a general reasoning-discipline gap: once the model had (correctly) established
"I am dead no matter which block I make," it kept re-deriving instead of emitting ANY legal answer,
until it timed out into an unparsed fallback. A brief core rung — "if you have determined the result
is the same across all your legal options (e.g. you take lethal no matter what you block), pick any
legal option and move on; do not keep re-checking" — is deck-agnostic and directly prevents the
self-poisoning loop. Flag for convergence: watch whether other seats show forced-outcome spirals at
any seam (blocks, chump lethal, must-target). If ≥2 seats show it, this is a strong core add.
