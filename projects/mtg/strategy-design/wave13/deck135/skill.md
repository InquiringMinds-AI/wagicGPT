# Deck-135 skill contribution (wave 13b) — one method refinement for synthesis

Wave-13b is the FIRST clean validation corpus after the wave-12/13a Spark-crash contamination. Per the
epistemics rule, deck135 offers NO new guide-authoring headline and NO core edit (guide FROZEN, core
PASS). It contributes ONE method refinement that COMPLETES the deck135-owned reasoning-tax / annotation-
contradiction rung (wave-8 tax-drop → wave-10 contradiction-multiplier → wave-11 tax-collapse signal),
extending it to a new LOCUS and a new, worse CONSEQUENCE. The current skill remains
`strategy-design/wave11/strategy-writing-skill.md`.

## Method refinement — the representation-CONTRADICTION-multiplies-reasoning class is NOT confined to OPTION annotations; it lives on the BOARD-STATE serialization too, and under a latency cap its worst outcome is no longer a tax but a DECISION TIMEOUT that hands the choice to the heuristic.

Wave-10 (deck135) diagnosed a mis-applied OPTION annotation (the permanent-target "NO legal target right
now" firing on the stack-targeting Force option) rendering a self-contradiction that drove ~470 words of
oscillation; the fix's success signal was the contradiction string → 0 plus a reasoning-tax COLLAPSE.
That rung has been framed around the OPTION surface (the annotated `Cast X - <annotation>` lines).

Wave-13b shows the SAME failure shape at a DIFFERENT locus and with a DIFFERENT terminal cost:

1. **New LOCUS — the battlefield/board-state line, not an option annotation.** deck109 s19: a creature
   FROZEN by Abominable Treefolk's ETB (Boros Reckoner) is serialized as
   `[tapped - untaps and can attack next turn] [attacking]` while it is NOT in the numbered Attackers
   list (A1-A3) and the harness's own `up to 8` damage line correctly excludes it. Four board defects
   collide into one self-contradiction: a stale `[attacking]` tag on a non-attacker, and a freeze-state
   that is not represented but actively INVERTED (`untaps next turn`). The model spent its ENTIRE reply
   re-deriving the damage math to reconcile "4 attacking-tagged creatures vs a 3-attacker list vs up-to-8"
   — the identical multiplier-on-derivation-length mechanism, now sourced from the BOARD line.

2. **New terminal CONSEQUENCE under the latency regime — a TIMEOUT, not merely tax.** In wave-10 the
   contradiction cost words (recoverable). In wave-13b, under the GPU_MEM_UTIL=0.75 latency cap, the
   inflated derivation hit the token/latency wall and produced `unparsed_reply` → heuristic fallback on a
   COMBAT decision. The representation bug thus CONVERTS into a harness-fallback: the latency cap is not
   just slow, it turns a board-surface contradiction into a decision surrendered to the heuristic.

**Generalized rule (completes the tax rung):** when auditing for the representation-contradiction class,
grep the BOARD-STATE serialization for self-contradictory tags (a creature both `[attacking]` and absent
from the declared-attacker list; a state annotation that contradicts a known effect, e.g. `untaps next
turn` on a frozen permanent), not only the OPTION annotations — and score its cost across BOTH surfaces:
(a) reasoning-tax at the affected decision AND (b) whether the inflated derivation TIMED OUT into a
heuristic fallback (grep `fallback=unparsed_reply` co-located with a long combat/priority reply). Under a
latency cap, a board contradiction is doubly expensive: bad reasoning AND a lost decision.

CONFIDENCE / GATE: single-seat, single-instance this corpus (outcome-neutral — the position was lethal
regardless). It COMPLETES the existing deck135-owned rung rather than opening a new one, so it is offered
as a method refinement, not a guide/core change. Synthesis should GATE it against other seats' latency-
timeout observations (a 2nd witnessing seat where a board/annotation contradiction co-locates with an
`unparsed_reply` on a NON-lost decision would promote it from refinement to a named engine-batch item).

## Held from wave-10/11 (reconfirmed at this seat, no change)

- **Score a value-floor / reactive rule (Force) by DECISION-CORRECTNESS, not usage-rate.** Force offered
  once as a cast (deck44 s22), declined CORRECTLY (opponent GFTT-ing its own creature = a strictly bad
  Force), 0 casts. The round-3 offer-legality fix VALIDATED: 0 dead-end "exile a blue card" actions, the
  one alt-cast offer correctly gated on a payable exile target. Falling take-rate is IMPROVED discipline.
- **A shipped annotation/parse fix is scored by the CLASS going to zero, not a raw count.** Round-3: 0
  dead-end picks corpus-wide at seat; name-echo 100% compliant; the one out-of-range CHOICE (deck44 s36,
  `CHOICE: 3` into a 2-option list naming an absent card) correctly REJECTED to heuristic, NOT mis-remapped.
- **The latency/timeout HARNESS class must not inflate a decision count.** 8 `unparsed_reply`, 0
  `empty_reply` (the Spark-death flood CLOSED); all self-healed, none changed a game. Score obedience
  against the REASONING; treat a fallback window as a harness item, never annotation-ignore.
- **colored-pays-generic is model-UNSTABLE, not un-taught.** Fired in first-draft at vs110 s5 and
  SELF-CORRECTED to the right cast — no wrong action. CORE stays PASS; a 3rd copy is the sanctioned-
  against escalation. Cross-seat grep for an ACTUAL offered-payable decline remains the promotion trigger.

## Convergence-watch (deck135 votes)

- **NO new guide line, NO core edit, NO demotion** (pause-aware: untested board-read/phantom-lethal
  anchors frozen at full prominence). Guide byte-identical.
