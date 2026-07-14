# deck135 wave-6 — proposals for the strategy-writing skill / synthesis agent

Context: deck135 is the ORIGIN deck of the wave-5 P1 "displacement vs overshoot" doctrine and
of Rule #1 "deploy a creature before value." This wave tests that doctrine on its own deck.
Record improved 1/6 → 2W-2L-1draw-1incomplete. Most of what follows CONFIRMS existing method;
two items are genuinely additive.

## P1 (CONFIRMATION, no edit) — the displacement fix landed on its origin deck without inverting.
Rule #1 was obeyed 14/16 creature offers, 8/10 direct creature-vs-value conflicts; Icehide Golem
8/8; no creature crammed ahead of a better value play. The wave-4→5 prediction chain
(blue-screw → attack-seam passivity → durdle-on-value) resolved: fixing the durdle pushed 3 of 6
opponents below the wave-5 18–23 opp-life floor. **This validates the Step-0 DISPLACEMENT bullet
and the "keep what worked verbatim" rule — no method change requested.** The core+guide
double-teaching of threat-before-value did NOT overshoot; the audit question ("did double-teaching
cram creatures out ahead of genuinely-better value") answers NO. Worth recording in the changelog
as the doctrine's origin-deck confirmation.

## P2 (ADDITIVE — new diagnostic pattern) — HALLUCINATED-BOARD PHANTOM LETHAL.
A distinct leak class the current Step-2 taxonomy does not name. The pilot's PLAN confidently
asserts a PERMANENT is on its own battlefield and computes a lethal from it, then declines
development ("Cast nothing" over a real creature) because it believes it has already won — while
the visible `Your battlefield:` line shows the permanent is NOT in play and it was never cast.
Evidence: vs131 s52–s60, PLAN "lethal board state (Abominable Treefolk 5/5 and Icehide Golem 2/2
for 7)... win immediately next turn" carried across 9 consecutive decisions; Treefolk was offered
0 / cast 0 all game; pilot declined Ice-Fang Coatl twice and lost at T11.
- DISTINGUISH from the three existing plan failures: it is NOT plan/choice-mismatch (the plan does
  not name a listed action it then betrays — it names a non-existent board), NOT stale-plan
  deferral (not a re-affirmed future ACTION — a false CURRENT state), NOT plan-diverges-from-
  legality (nothing unaffordable — it just isn't real).
- The deciding fact is FULLY surfaced (battlefield line) and the general prompt already says
  "count this turn's damage only from creatures actually in the list" and "trust the board over
  the plan." Both are PRESENT and LOSING. Per the layer-routing discipline this is therefore
  ENGINE/REPRESENTATION or MODEL, not a louder core/guide line.
- ROUTE: model-experiments residuals as a litmus (own-board hallucination / phantom lethal). A
  guide can add at most a cheap board-read reinforcement ("if the creature isn't printed on your
  battlefield line it is NOT in play") — deck135's wave-6 guide does — but this is a stopgap; the
  durable fix is representation/model. SINGLE SEAT this wave → propose as a Step-2 diagnostic
  bullet on the convergence watch, do not cut/add core.

## P3 (CONVERGENCE INPUT — argues AGAINST a pending promotion) — reflexive high-life blocking.
The wave-5 deck109 proposal to sharpen reflexive-high-life-blocking into a standalone core sentence
is on the wave-6 convergence watch. **deck135 gives it NO support:** only 2 bundled blocker
decisions all corpus — one a correct low-life (6) survival block, one a high-life (20) FAVORABLE
trade-up (1/1 snow-deathtoucher kills a 4/4). Neither is a wincon chumped at high life. Synthesis
agent: absent a second seat reproducing the deck109 pattern, keep it per-deck; do not promote.

## P4 (ADDITIVE, minor — reaction-scoping) — a FREE reaction still costs a CARD; scope by card cost.
Force of Negation is free-on-opponent's-turn (exile a blue card), and the executor reads "free" as
"always fire" — cast 5/5 this corpus, none at a board wipe, twice exiling blue cards it needed to
2-for-1 itself against 1-mana spells while behind (vs109 s31/s32/s59). Method note for the
reaction/hold guidance: when a reaction's MANA is free but it still spends a CARD (alt-cost exile,
pitch, additional-cost sacrifice), the scoping rule must name the CARD cost, not just the target —
"fire only at a spell worth two of your cards." SINGLE SEAT → per-deck (deck135's guide now scopes
Force to board-wipes/only-threat). Flag for the skill only if a second free-alt-cost reaction deck
reproduces it.

## Non-proposals (existing method validated, no edit): harness-defect sweep (0 desyncs, 0 offset
traps), offered-vs-taken option-line tally (gave the Diamond-Faerie 0/0 headline), timeout triage
(cleanly separated winning-but-slow / latency-starved / grind), turnaround-preservation mode
(guide revised as a frozen-line diff), never-drawn-vs-in-hand-uncastable (Diamond Faerie is the
canonical in-hand-uncastable case, 2nd wave running).
