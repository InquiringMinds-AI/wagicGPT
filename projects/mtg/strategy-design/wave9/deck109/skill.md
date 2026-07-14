# Skill contribution — deck109, wave 9

deck109 this wave is a **clean validation-and-freeze seat**: the wave-8 headline revision (re-anchored
chump rule) VALIDATED where a window existed, and there was NO new failure class. The method points below
are about how to READ a validating-but-thin corpus at a project pause — they refine existing lines, they
do not open new ones.

## 1. Validate a re-anchor by defect-class->0, but a THIN sample validates the class, not the general rule
The re-anchored chump rule ("block ONLY on surfaced LETHAL") shipped wave-8 and produced **3/3 correct
blocker decisions this corpus** — but the corpus offered only THREE blocker windows total (an aggro deck
racing hard rarely defends). Of the two wave-8 misplay classes, only ONE (high-life gang-block) actually
got a test window (deck110 seq4: 20/20, "NOT lethal" -> no blockers, DIED). Reporting "the re-anchor is
validated" is correct for the high-life class; it is NOT yet validated for the Obliterator-block class,
which had zero windows. **Method:** when a re-anchor validates on a thin sample, state validation
PER-CLASS against the windows that actually occurred, and keep any class with zero windows at WATCH — do
not fold "no recurrence off zero opportunities" into "the rule is proven." (Extends the wave-8 re-anchor
validation line + the rare-event-A/B-power trap.)

## 2. Do NOT demote an untested guardrail into a project pause
The NEVER-block-Phyrexian-Obliterator absolute (wave-8) got zero test windows this corpus (Obliterator
never entered combat — deck133 was raced out T10). The turnaround demotion terminus would normally invite
compressing a guardrail once the seat is winning. But this guardrail (a) was never exercised, and (b)
guards a KNOWN game-loser (wave-8: block-at-10 -> sacrifice board -> loss). With NO wave-10 to catch a
regression, **the pause discipline overrides the demotion terminus: keep an untested-this-wave guardrail
at full prominence.** Demotion requires the class to have been OBSERVED obeyed, not merely absent.
(Refines the demotion terminus with a pause-aware precondition: "observed obeyed," not "no counterexample."
Aligns with the wave-9 brief's standalone-guide mandate.)

## 3. A validated corrective's SOFT residual is not a re-open trigger
The Pillar `{R}` corrective killed the false `{1}{R}` cost belief (0 instances). A softer residual survived
(one "hold Pillar for combat tricks" plan) — but the pilot fired Pillar the same turn anyway, the behavior
is covered by an existing guide clause, and it changed no outcome. **Method:** distinguish the TARGETED
belief (dead -> corrective validated) from an adjacent SOFT instinct that is covered + benign; the latter
is not a wave edit. Editing over it would be gold-plating and risks displacement into the deploy rule.

## 4. REPLY-PROTOCOL INTENT-COLLAPSE now has a NON-NUMERIC-HEAD tell at the attacker seam
deck44 seq7: `kind=attackers`, head token was the literal WORD `"none"`, engine parsed it to option 0 =
`no attackers`, while the PLAN unambiguously wanted the attack. This is the wave-8 intent-collapse class
(deck135, where a stale NUMBER head lost to a differently-reasoned PLAN) with a new shape: a **non-numeric
head defaulting to the passive option**. Distinct value: the parser's fallback for a word head is
option-0, which at the attacker seam is the WORST option (no attackers, breaks the aggro spine). **Route to
the reply-protocol layer** (accept the LAST bare number / scan the PLAN when the head is non-numeric),
never the guide, never scored against a guide rule. The guide's #1 combat rule was obeyed in the reasoning.
Strengthens the case for the standing reply-protocol A/B; adds "reject/repair non-numeric heads" to its
scope.

## Freeze / displacement / layer-routing (per skill discipline)
- **Freeze-check:** strategy.txt is byte-identical to the live wave-8 guide (cmp verified). Every kept line
  was checked against this corpus; identity block, #1 deploy-and-attack rule, MP2 checklist, BURN
  face-default + Pillar `{R}` clause, BLOCKING re-anchor + exceptions, Obliterator note, EVASIVE/UNTOUCHABLE
  note, KEY CARDS, MULLIGAN, SITUATIONS — all validated or untested-but-load-bearing; none contradicted.
- **Displacement audit:** the frozen spine held (27/28 attacks, all burn-to-face, lethal taken); the only
  leak moved to the REPLY-PROTOCOL layer (head-parse), not to any adjacent guide seam. No new upstream
  durdle appeared.
- **Layer routing:** blocker lethal math = representation (present-and-winning, no guide change);
  Obliterator rider = engine ledger #3 (representation, owned by deck133 seat); non-numeric head =
  reply-protocol layer. Zero core-prompt candidates from this seat.
