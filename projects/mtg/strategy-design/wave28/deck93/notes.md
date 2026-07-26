# deck93 dev notes (wave 28) - engine/harness/representation ledger + rotation verdict

## N-93a (ENGINE / PROMPT-REPRESENTATION, MED severity) - board render lacks a
## summoning-sickness tag; the perception gap drives the persistent L2 phantom-attacker leak

**Symptom.** Across all 6 deck93 games, in 11 of 15 attack declarations the model pads the
ATTACK line with attacker slots the engine did not offer, because it counts creatures from the
CURRENT-SITUATION board render rather than from the A-lines. The engine drops the phantom slots
safely (0 fallbacks), but the model's stated damage math and PLAN are built on non-existent
attackers, and in the vs18 loss (seq18, turn 11, 3 life) it tapped its one legal Rat while
believing it swung "12 total power to lethal," then died with no blockers.

**Root cause (verbatim evidence).** The own-battlefield render tags `[tapped]` state but has NO
summoning-sickness marker. Summoning-sick Rats render identically to attack-ready ones:

  vs18 seq18 "Your battlefield" line (verbatim):
    ... Relentless Rats #1 {1}{b}{b} (4/4) (printed 2/2); ... Relentless Rats #2 {1}{b}{b}
    (4/4) (printed 2/2); Relentless Rats #3 {1}{b}{b} (4/4) (printed 2/2)
  A-lines for the same decision (verbatim):
    A1. Relentless Rats #1 (4/4)          <- ONLY #1 is legal; #2 and #3 were cast this turn
  Reply (verbatim): "ATTACK: A1, A2, A3 ... a wide board of three 4/4s (12 total power)"

The board says three identical 4/4s; the A-lines say one legal attacker; the model trusts the
board. Guide prose ("count from the A-lines, not the board") is present in
deck93_strategy.txt and is ignored - a perception surface that contradicts the instruction
wins over the instruction.

**Proposed fix (mirrors the e4 land-tag that killed land-blindness).** Add a per-creature
state tag to the board render for creatures that cannot attack this turn due to summoning
sickness, e.g. `Relentless Rats #2 {1}{b}{b} (4/4) [summoning sick - cannot attack this
turn]`. The tag infrastructure already exists (the `[tapped - untaps and can attack next
turn]` tag is emitted the same way). This is the annotation analog of the land tag: annotate
the restriction INLINE on the surface the model reads, rather than relying on it to
cross-reference the A-lines. Expected effect by analogy to land-blindness: the phantom-attacker
class collapses.

**Repro.** Binary ad8930961, corpus matchups-20260726-050449, file
`1785069527-ai_baka_deck93-*vs-ai_baka_deck18.jsonl`, seq18 (and vs134 seq8, vs22 seq24 for
more instances). Grep the deck93 seat logs: ATTACK lines listing more A-slots than the prompt's
`A\d+\.` lines. Programmatic scan found 11 attacker over-lists + 1 blocker over-list (vs18
seq19 "B3" when only B1/B2 legal), all safely bound.

**Layer note.** This is the ONLY open work-signal at the deck93 seat. It is engine/
representation, not guide - the guide teach for this leak already exists and failed, which is
itself the evidence that the fix belongs at the annotation layer (see skill.md S-93a, the
perception-vs-strategy discriminator).

## N-93b (COSMETIC, LOW) - mulligan prompt double-renders the opening hand
The pre-game mulligan prompt lists the opening hand twice: once plain ("- Your opening hand
(6 cards): Swamp; Swamp; Swamp; Relentless Rats; ...") and once tagged ("Your hand: Swamp
(land: taps for {B}); ..."). Harmless (kept 6/6, no confusion), but the plain line is redundant
now that the tagged line carries strictly more information. Candidate for de-duplication.

## Steady-state confirmations (no action)
- Instance handles / R-DUPLICATE-NAME-INSTANCE: CLEAN at scale (0 fallbacks, 97 decisions,
  6 concurrent Rats peak). Re-confirms wave-27 audit.
- e4 land tags: consumed cleanly, no prompt-noise, no latency effect.
- e5 annotation-echo: N/A at this seat (0 fallbacks; the echo shape was deck22's).
- Both corpus retracted_choice fallbacks are at OTHER seats (deck134 seq25, deck146 seq40);
  all 16 defers are at deck137. deck93 seat = 0 fallbacks.

## ROTATION VERDICT: NOT YET - near-terminal on the guide axis
- Guide is VALIDATED and FROZEN (byte-identical redeploy this wave, cmp-verified 5124 bytes).
  All three guide-layer teaches resolved: mulligan land-blindness FIXED (6/6 keep, guide cited
  verbatim), L3 over-blocking FIXED (lord-shrink cited, no gang-block-while-ahead), L4
  critical-mass sound (losses are structural matchup speed, not a guide defect).
- BUT deck93 produced a NEW engine/representation work-signal this wave (N-93a summoning-sick
  tag), so it FAILS the rotation test (rotation = no-guide-mod AND no-new-work-signal). The
  no-guide-mod half is satisfied; the no-new-work half is not.
- Projection: this is the LAST open leak at the seat. Once the summoning-sick annotation lands
  and is validated (expected to kill L2 by the land-tag analogy), deck93 is a clean rotation
  candidate - guide frozen, handles proven at scale, no residual work. One more validation
  cycle to terminal.
