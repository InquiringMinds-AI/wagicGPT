# Deck-135 wave-7 — proposals for the strategy-writing skill

Mode this wave: OBEYED-BUT-LOSING (record 2W→1W, every obedience metric healthy,
losses lack a precondition the guide can create). So most of the value is
CONFIRMATIONS of existing method, plus two genuinely additive refinements. Nothing
here cuts a core line (single seat).

## CONFIRMATIONS (existing method validated on its origin deck)

- **Phantom-lethal board-read anchor (wave-6 135 P2 diagnostic) HELD — 0 recurrences.**
  The wave-6 skill routed hallucinated-on-board phantom lethal to model-experiments/
  representation and allowed "a cheap board-read reinforcement" as a guide stopgap. That
  reinforcement shipped in RULE #1 ("if a creature is not printed on your battlefield line
  it is NOT in play"). This wave, in the same vs131 matchup, the pilot tracked Treefolk as
  in-hand and CAST it (15/15) rather than hallucinate it onto the board. Confirms the
  routing AND that the cheap stopgap is compatible with the fix. Caveat per the
  rare-event rule: precondition count was modest (2 games carried Treefolk-centric plans),
  so this is a non-recurrence, not a proof of resolution — keep the anchor at full
  prominence and keep the class on the convergence watch.

- **Per-deck scoping of a "free" reaction by NAMED board-wipe cards WORKS — no core edit
  needed.** Wave-6 rejected promoting "a FREE reaction still costs a CARD" to core (single
  seat) and kept it per-deck (scope Force by card cost). This wave the shipped guide line
  ("Force ONLY on a board wipe / a spell that kills your only threat") was OBEYED: 9/10
  fires correct/defensible (6 literal sweepers in vs140, 1 removal-on-lone-threat in vs44).
  The per-deck fix resolved the wave-6 5/5 over-fire without touching core. Confirms the
  layer-routing call: keep it per-deck; still single seat, do NOT promote.

- **OBEYED-BUT-LOSING variance-entry test held.** A falling n=6 record against a harder
  slate, with healthy obedience and color/clock-shaped losses, is variance/construction —
  freeze the obeyed lines (Rule #1, Rule #2), route the record to a construction flag, do
  the guide as a frozen-line diff. This wave is a clean instance; the mode's discriminator
  ("opponent near 20 at death") fired on 2 of 4 losses.

## ADDITIVE REFINEMENTS (single seat — propose as watch / method sharpening)

- **Distinguish STALE-PLAN-AROUND-AN-UNCASTABLE-CARD from both phantom-board and
  single-window plan-diverges-from-legality.** The existing plan-diverges-from-legality
  bullet describes a ONE-window event: the pilot plans an unaffordable cast, then picks
  "Cast nothing" instead of reconciling. vs44 this wave shows a MULTI-TURN variant driven
  by a manabase strand: the pilot re-affirmed "cast Abominable Treefolk immediately" across
  s19/s30/s66 while Treefolk was never offered (0 casts all game, short blue). It is NOT
  phantom-board (the pilot correctly says "in hand," never "on battlefield") and NOT a
  single unaffordable-plan reconcile — it is a stale PLAN that keeps naming a cast the
  engine never surfaces because the mana never arrives. Two consequences for the reviewer:
  (1) do not mis-file it as the phantom-lethal class (the board-read anchor does not
  address it — the board read is CORRECT); (2) its true owner is a construction flag
  (manabase) plus stale-plan poisoning (model), and the cheap guide stopgap is a
  list-anchor reinforcement tied to the named card ("if Treefolk is NOT in your cast list
  you cannot pay for it yet; deploy what IS listed"). Proposed as a sub-note under the
  plan-diverges-from-legality diagnostic pattern.

- **When quantifying a RE-OFFER engine defect's cost, count OFFER windows, not just
  consecutive re-picks.** Wave-6 measured the fetch re-offer loop by consecutive identical
  re-picks (3-4-deep chains). This wave those chains shrank to ≤2 — which would read as
  "improved" if that were the only metric. But the dominant cost turned out to be the
  HELD-fetch re-offer TAX: 44-97 windows per game where the crack was re-offered and the
  pilot correctly declined, each still a model call (vs140's 97 such windows fed its
  144-decision timeout). A re-offered repeatable activation has TWO cost surfaces — the
  waste-loop (re-taking it) and the attention/latency tax (processing the re-offer even
  when declining) — and a digest that counts only the former under-reports the defect.
  Proposed as a measurement refinement to the waste-loop / re-offer diagnostic and to the
  "extract, don't eyeball" digest guidance.

## NO-CHANGE PROPOSALS (method used as written, no edit)
- Verify-a-shipped-fix-FIRED-before-crediting (Step-0 opener): applied — greased the Force
  targets and the phantom pattern out of the corpus before crediting either. Worked.
- Harness-defect sweep first: 0 desyncs, 0 numeric-offset traps, 1 opponent-seat
  validation-drop (correctly excluded). Worked.
- Timeout triage: vs140 read as a slow-clock control-mirror stalemate (Force vs sweepers +
  fetch tax), not passivity — checked who was ahead (behind 15v26) and latencies (med 7s)
  first. Worked.
