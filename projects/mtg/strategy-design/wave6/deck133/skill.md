# Wave 6 — deck133 proposals to the strategy-writing skill

deck133 ran in OBEYED-BUT-LOSING / turnaround mode (3/6 rising; the wins are the wave-5
guide working). Revised as a constrained diff. Two small method refinements + confirmations.

## PROPOSE (adapt an existing method line)

### P1. A conditional payoff at the TOP of an impact list needs its ELSE stated, or the executor takes it unconditionally.
Extends the Step-5 lines "cast a payoff whose value scales when it does the most, not the first
turn you can afford it" and "convert a self-locking prohibition to a numeric GO threshold."
**New observation:** when a gated payoff is written as item #1 of a ranked act-floor ("Gray
Merchant WHEN it wins or saves you"), the weak executor reads the POSITION ("#1 = my best play,
take it") and drops the gating clause — deck133 vs131 seq69 cast Gray Merchant at 2 devotion
(drain 4, opp at 22, own life 18: neither trigger true) and got it countered. The fix that
worked in the diff: state the FALSE branch inside the entry ("if it does NEITHER, take a
creature from lower on the list to build devotion"). A top-of-list conditional entry must carry
its else-action explicitly; the ranked position otherwise overrides the condition. Suggest a
one-line addition to the ranked-list / low-branching guidance in Step 5.

### P2. When two near-identical cards differ in SELF-COST, split them in the guide — the brake keys on the one that costs.
Refines "Life-as-resource needs a BUDGET." deck133's guide lumped "Thoughtseize/Inquisition" as
one discard slot, but Thoughtseize costs the caster 2 life and Inquisition is free. The pilot
paid Thoughtseize's 2 life into a KNOWN-EMPTY hand (vs135 seq24) in a race it lost by 2 life.
The budget rule cannot bite while the free and the life-costing card share one line. **Method
add:** when a deck runs two cards of the same role that differ in self-inflicted cost (life,
sacrifice, discard), give them SEPARATE guide entries so the cost-gated caution attaches only
to the one that pays it — a single merged entry lets the executor apply the permissive card's
freedom to the costly one. (This is the same "measure redundancy before you prescribe" logic
applied to a card's cost, not its count.)

## CONFIRM (existing method held under test — no change)
- **"Prefer an engine-SURFACED number over hand-computed, and VERIFY its offset"** (deck133's
  own wave-5 contribution) HELD: the Gray Merchant `{right now: drains N}` display and its +2
  offset were re-confirmed on the one resolved cast (vs44: shown 10 → actual 12). The
  read-then-add-2 stopgap works; keep it, removal condition = the display fix ships (notes.md).
- **OBEYED-BUT-LOSING / turnaround-preservation mode** applied cleanly: freeze-check produced,
  new text aimed only at the two residual levers, no rewrite. Method needs no change.
- **Two-distinct-uses-two-keyed-triggers** (deck133 wave-5) still correct in principle; the
  stabilize trigger simply went unexercised (GM never offered at ≤10 life this corpus) — a
  reminder that a single-seat corpus can leave a rule UNTESTED, which is not a reason to cut it.
- **Timeout triage** held: print final life first; both capped games were infra + matchup, not
  durdle (vs140 81 priority re-asks; vs131 grind+latency). No anti-passivity pressure written.
- **The vs109 win was TRANSCRIBED** as the pilot's own proven burn-aggro line (findings b) —
  transcription over invention, per method.

## NOT proposed
- No new tell, no new stopgap class, no negative-framing change. The one new caution
  (Thoughtseize into an empty/irrelevant hand) is written POSITIVELY as situation→play and as a
  split impact-list entry, not a DON'T.
