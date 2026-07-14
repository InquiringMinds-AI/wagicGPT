# Deck-110 wave-8 — core-prompt (general-strategy) proposals

## Verdict: NO core-prompt edits from this seat.

Layer-routing: every item this wave routes to ENGINE/REPRESENTATION, PER-DECK, or
MODEL-EXPERIMENTS — nothing is deck-agnostic-and-losing at the core layer. The burden of proof is on
core; this seat does not meet it.

### Why nothing goes to core

- **The wave's lever is a REPRESENTATION READ, handled PER-DECK.** The pilot hand-counts artifacts and
  over-counts while `Artifacts in play: you N` sits accurately (0/180 offset) on the board. The fix is
  a named-card, deck-specific read instruction (Cranial Plating / metalcraft / Etched Champion
  protection) — it is not deck-agnostic (only artifact decks have an "artifacts you control" gate).
  The GENERIC principle it instantiates ("prefer a surfaced number over a hand-count") is ALREADY in
  the skill/guide-authoring method. No core sentence. If a SECOND artifact/devotion/affinity deck
  reproduces the same hand-count-over-a-surfaced-line pattern, the candidate promotion is an
  ENGINE/REPRESENTATION nudge (bold/label the count), never a core prose line.

- **The marker is fully obeyed (16/0) — no core gap.** The representation fix
  (`AIPlayerGPT.cpp:1163-1173`) continues to carry the load; core did nothing and needed to do nothing.

- **The vs44 loss is META/DECK-CONSTRUCTION (counter + evasion + reach starvation)**, not a prompt
  defect. The stack-surfacing + labeled-counter representation is working as intended (it revived the
  opponent's counters against affinity's discrete threats); the evasive Archmage flier is a
  matchup/decklist fact. Core is correct; the deck list is the owner. No core change.

- **The vs131 degenerate reply (`Answer:\n###...`) and the vs140 defer** are a model-capability
  failure and a harness validation-drop respectively — MODEL-EXPERIMENTS / ENGINE, not core prose.

- **cast-nothing 17%** is concentrated in the double-Lightmine WIN (vs140, where the guide ORDER is
  "cast nothing / don't attack") and the vs131 blowout. Not a deck-agnostic passivity signal; the core
  act-floor is fine. No change.

### Standing shortlist item (not this seat's call, flagged for the synthesis agent)
The `hand -> stack` cast-happened anchor A/B was ADOPTED by the synthesis (secondary arm removed the
anchor sentence; cast-nothing 41.5% vs 41.9%, no regression) — the removal is now in the live prompt.
deck110's PRIMARY arm corroborates the no-regression: 0 engine no-ops across 206 decisions, the
anchor's target bug did not manifest for this seat. Recorded for the cross-deck tally; no seat action.
