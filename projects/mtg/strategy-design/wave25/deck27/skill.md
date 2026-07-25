# skill.md — deck27 reviewer, wave 25 (proposals for the synthesis agent)

Three proposals, all single-seat but two are LIVE-ACCEPTANCE witnesses that let the synthesis agent
retire/close standing watch items rather than open new rungs.

## #1 (CLOSE/PROMOTE) — b2 NATURAL-STOP precedence: FIRST live-positive, at the BLOCKERS seam. The
wave-24 headline-1 parser candidate ("prefer the model's FINAL coded line on a naturally-terminated
reply") is now OBSERVED WORKING and game-saving.

deck27 vs109 seq11 is the acceptance test the wave-24 synthesis called for. A natural-stop blockers
reply led with `BLOCKS: none` (lethal at 6 life vs 6 FS+trample), reasoned to completion, REVERSED to
`BLOCKS: B1:A2` (survival), and **the engine applied the FINAL line (choice=1); deck27 survived.** This
is the exact first-line-vs-conclusion shape that in wave-24 (deck131 s30 ATTACK; deck27 vs137 s5 bottom)
FROZE the wrong first token and lost material. The fix now catches it.

Reviewer-method update for the synthesis agent:
- Mark the wave-24 headline-1 parser candidate and headline-3 "diff 3 (leading line == concluding
  recommendation)" as **LIVE-VALIDATED at the blockers seam** — natural-stop last-line-wins is
  confirmed correct-and-beneficial. b2's scope note (ATTACK stays answer-first; natural-stop replies
  take the final coded line / final bottom-list) is holding.
- Add ONE decision-scoring check the corpus mandates: when a natural-stop reply's FIRST coded line
  differs from its CONCLUDING coded line, read the `choice` field to confirm WHICH the engine applied.
  This catches the fix working (this wave) AND would catch a regression where answer-first re-freezes.
- CAVEAT to record honestly: at the blockers seam the parser's own "`none` is not an assignment / last
  real BLOCKS line wins" behavior can co-produce the correct outcome, so this seat cannot fully isolate
  b2 from the pre-existing blocker parser. It is still a POSITIVE (correct rescue, the behavior b2
  mandates); a cleaner isolation would be a natural-stop reversal at the ASK/CHOICE seam.

## #2 (CLOSE-BY-FACT) — b5 wither-vs-first-strike annotation is CORRECT IN BOTH DIRECTIONS; close the
wave-24 wither watch as a representation fix, not a model belief.

The wave-24 reopening (false "(wither shrinks it to 0/0)" when a first striker kills the withering wall)
is FIXED and validated at both poles this corpus: first-strike case omits the shrink clause (vs109
seq9/seq11), non-first-strike case includes it correctly (vs137 seq12, Oona 2/1 shrinks a 5/5 to 3/3).
This is the crutch-lifecycle CLOSED-BY-FACT reference pattern again (annotate/ship-the-FACT/close the
watch) — a single-seat REPRESENTATION misrender the option-annotation fix eliminated, NOT a durable
model belief. Synthesis: mark the wither-under-first-strike clause CLOSED-BY-FACT; no rung.

## #3 (SINGLE-SEAT RUNG, converges with wave-24 headline-4) — a MANA-SIMPLIFICATION FACT must be
carried into EVERY cost-reasoning phase, not just the CAST seam; it does NOT transfer on its own to the
new PREGAME/BOTTOM phase.

deck27 vs102 seq3: the model bottomed its flexible triple-{U/B} hybrid (the deck's flagship evasion)
calling it "difficult to cast without significant mana fixing" — rule #1 exactly inverted, in the BOTTOM
phase. Rule #1 is framed around cast OPTIONS ("if a Cast option is listed..."), and that framing does
not reach the bottom phase (raw costs, no cast list). Guide-lane fix applied (bottom-phase restatement:
hybrids are easiest; ship a double-pip card before a flexible hybrid).

This CONVERGES with wave-24 headline-4 (deck59's green-veto belief cost MULLIGAN quality, fixed by the
same mana fact reaching the mulligan phase). Two seats now: a hybrid/flexible-mana deck's simplification
fact is a CROSS-PHASE fact — any guide that states "your mana is flexible / trust the number" for a
cast decision must ALSO state it for the mulligan and bottom decisions, or the weak executor
pattern-matches "many colored pips = hard to cast" in the phase the cast-framed rule doesn't cover.
Promotion note for the synthesis agent: if a THIRD flexible-mana deck shows the fact failing to reach a
non-cast phase, promote "carry the mana-simplification fact into every cost-reasoning phase (mulligan,
bottom, cast, X-announce)" to a standing skill rung for hybrid/devotion/flexible-mana guides.

## No core-prompt change from this seat (see general-suggestions.md — 20th consecutive PASS).
