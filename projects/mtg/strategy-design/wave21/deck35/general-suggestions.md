# deck35 -- general Magic-reasoning / cross-deck suggestions (single-seat, non-core)

Layer-routed: general reasoning/representation/decode observations, NOT deck35 guide lines and NOT
engine bugs (engine/harness -> notes.md). None can add/cut a skill core line.

## G1 [DECODE-TIME GUARD, cross-deck -- the reasoning tax is DEFUSED for correctness but persists as token/latency cost; the decode-time guard is now a COST fix, still worth shipping]
Answer-first removed the parse risk from the long-reply tax (deck35: 87/87 lead with the coded line;
the four longest replies 12.1-13.1k chars all parsed correct, incl. a 12.5k-char LAND DROP that
answered `Play Mountain` on line 1). But the spirals themselves persist -- 12-13k chars with verbatim
repetition tails ("I die." x4; "I have {g}{u}{r}{r}{r}." x4) -- on decisions the guide says to answer in
one line. `repetition_penalty` shipped as a config key but is OFF this corpus. Recommend enabling a
modest decode-time repetition penalty and/or a per-reply max-token guard corpus-wide: it no longer
buys correctness (answer-first did that) but it buys latency (p90 was 70s) and token cost at every seam
at once. A guide off-case cannot reach a decode loop (the model quotes correct guide lines mid-loop), so
this stays at the sampler layer.

## G2 [HARNESS, cross-deck -- extend PROSE-INTENT salvage from combat to the priority/casting menu for the commit-then-change-mind case]
The combat prose-salvage (batch item d) recovers a decision stated in reasoning when no coded line
parses. deck35's one fallback (vs62 seq18) is the priority/casting analogue of the SAME need: line-1
`CHOICE: 3 (Heart)` went stale when the model reasoned to Fury and stated "Cast Fury" in its PLAN; the
retraction net correctly dropped the stale line but then relied on the heuristic argmax to recover the
right card (it happened to). A prose-intent salvage that reads the PLAN line on a `retracted_choice`
priority/ask record would recover the model's actual final single-card intent deterministically. Guard
it the same way as combat salvage (unambiguous-only, negation-guarded, single clear intent). Single
instance this corpus, correct outcome, so this is a LOW-priority hardening, not a fix -- flagged so the
answer-first stale-line class is on the synthesis radar as answer-first generalizes.

## G3 [REPRESENTATION, positive -- the per-blocker trade tags continue to drive correct SELECTIVE blocking]
Reconfirmed 4th wave. deck35 declined every non-lethal non-profitable block ("no blockers" in healthy
spots: vs62 seq16, vs135) and took the profitable ones: vs27 seq10 a double-block (Bonesplitter blocks
Vodalian, Gemhide blocks Metathran) killing both attacking Zombies while both blockers survived (pumped
5/3 and 6/4 bodies), and vs62 seq20 Fury blocks Canopy Spider. The "(you kill it, your blocker lives)"
/ "deal up to N -- LETHAL/NOT" tags remain the durable cure for the block-math re-derivation tax and
generalize to every combat-heavy seat. Keep them.

## G4 [CONSTRUCTION-adjacent lens, cross-deck for aggro seats -- the haste-dodges-the-bug correlate is now RETIRED by the payment fix]
Wave-20's G4 noted a haste enabler did double duty: tempo AND sidestepping the attackers-non-issuance
bug. With the partial-sparing fix validated, the SECOND duty is gone -- no-haste boards now attack
reliably (vs14 attacked with the unsick subset every turn; vs62 every eligible turn). Update the
synthesis lens accordingly: "did this aggro seat have haste?" NO LONGER partially predicts
engine-caused low-attacker games, because the payment mechanism that made no-haste boards vulnerable is
fixed. A low-attacker aggro game post-fix should be diagnosed as draw-screw / all-sick-board / forced
mana-dork-tap, NOT the old engine bug.
