# deck131 wave-9 — cross-layer suggestions (routed OUT of the guide)

Because this is the FINAL wave before a project pause, each item below is self-contained (full repro
+ context) and stands without a wave-10 follow-up.

## [REPLY-PROTOCOL A/B — PROMOTE from single-seat watch] head-first intent-collapse, now 2 seats

**Class:** the reply protocol asks for a bare number FIRST, then a PLAN. The model emits the head
number, THEN reasons in the PLAN, and when the reasoning concludes a DIFFERENT option in the SAME
window, the choice stays locked to the stale head. This commits the answer before the model has
thought.

**Evidence (2 seats, 2 decks, 2 waves):**
- deck135 wave-8 s48: head `4` (=Cast nothing), PLAN concludes "Option 2, cast Icehide Golem"
  (self-corrected the following phase — cost limited).
- deck131 wave-9 vs44 seq1 (corpus `matchups-20260714-094942`, file
  `1784046898-ai_baka_deck131-0x559d9770f980.jsonl`): options `['Keep this hand','Mulligan']`, head
  `1` (=Keep), PLAN reasons correctly and concludes "I must Mulligan ... I will shuffle this hand
  back." `chosen_text`=Keep. NOT self-correctable — a mulligan is a one-shot window, so the stale head
  is FINAL and cost the game (kept a 0-red, no-draw hand → 0/18 loss vs faerie tempo).

**Why it matters now:** deck131's wave-8 GUIDE fix (strict mulligan gate) demonstrably corrected the
REASONING — the guide can do no more here. The residual bad outcome is 100% reply-protocol. Two seats
clears the single-seat bar; escalating deck-guide wording cannot touch it.

**Recommended A/B (harness, symmetric core — judged by decision-quality, not win-rate):**
- Arm A: accept the LAST bare integer emitted in the reply (post-reasoning), not the first.
- Arm B: move the CHOICE/number emission to AFTER the PLAN in the reply-format contract.
Either lets the reasoning bind the choice. Distinct from PLAN/CHOICE MISMATCH (head matches a
genuinely-different intended action) — here head and reasoning target the same window.

## [DECK CONSTRUCTION — dominant, unchanged, cheap] threat density + reach

deck131 has lost the win column for 5 straight waves while the guide is fully obeyed. The construction
residuals the guide cannot fix:
- **Threat density 6/60** (3 Guttersnipe + 3 Young Pyromancer). The deck bricks on threats: multiple
  losses are "drew no sticky payoff / first payoff answered, no second act." Two wave-9 losses were
  CLOSE (vs110 opp 9, vs133 opp 5) — a small density bump likely flips them. Suggest +2-3 threats
  (e.g. Spellheart Chimera from the #SB doubles as reach) at the cost of the softest lifegain.
- **No reach vs lifegain** (vs140 lifegain grind → timeout even while AHEAD 29/25; the 2-damage
  Guttersnipe clock can't close through Prism Ring/Elixir-style gain on the far side). Reach cards
  (Spellheart Chimera; more burn) address this AND the Guttersnipe-swept problem.
- **Manabase 14I/8M** leans slightly wrong for all-red payoffs (both win-cons red). A cheap 11M/11I
  shift is harmless now that the land-menu interface is fixed (color reaches the board on curve when
  drawn). Secondary to threat density.
- **Cut a Prism Ring for a threat/reach card** — Prism Ring is pure lifegain, does not advance the
  clock, and the guide already deprioritizes it to "nothing better to cast" windows.

## [CORE — WATCH only, do NOT promote] `damage >= toughness = dead` rules-fact

Single-seat still (wave-8 deck131 vs140 seq51: pilot asserted a 2/2 Guttersnipe survives 2 damage and
declined an available Dissipate on a Pyroclasm that then killed it). NO recurrence wave-9 (precondition
— sweeper-on-stack-with-counter over a live Guttersnipe — did not occur). Carry as a candidate core
rules-fact for the core owner; a single seat cannot cut core. If a second deck ever shows a model
asserting "N damage does not kill N toughness," it becomes a core rules-fact.

## Metrics to re-check IF the project resumes (stable signals; win-rate is noise at n=6)
- Mulligan BOTH directions, split reasoning-correct vs outcome-correct: no-red mulligans (reasoning
  1/1, outcome 0/1 this wave — the gap is the reply-protocol defect), keepable-kept (5/5).
- Multi-land co-offer count + Mountain-first rate (9/9 this wave; if the co-offer count drops to 0 the
  land enumeration regressed).
- Guttersnipe-on-battlefield decision count + fuel-declined leaks (0 leaks / 2 games this wave).
- Attack participation (100%), NEVER-block-Guttersnipe (held), reflexive high-life blocks (0).
