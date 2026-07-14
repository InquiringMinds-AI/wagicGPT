# Deck-135 wave-8 — core-prompt / general-layer proposals (layer-routed)

Layer routing per the skill: engine items -> notes.md (never prompt text); model/reply-protocol
items -> here as OBSERVATIONS with the correct owning layer named; guide items -> strategy.txt.

## No core PROMPT-BODY edit proposed (PASS continues — 5th wave).

deck135 returns no core-prompt-body edit. Rule #1's else-branch fix (develop when no creature is
castable) is DECK-SPECIFIC guide text (it names this deck's fixing spells: Into the North ->
Island), not a general sentence — it lives in strategy.txt. If a second deck shows the same
"freeze when nothing to deploy / hold mana for an absent threat" durdle, that would be the
promotion trigger for a general "an empty board with mana up is never a pass — develop toward a
threat" line; one seat cannot cut core.

## Reply-protocol OBSERVATION (owning layer = the code-appended reply protocol, NOT the prompt body)

**Answer-before-reasoning can cause intent-collapse.** vs131 s48: reply head `4` (Cast nothing)
while the PLAN reasons fully to "Option 2, cast Icehide Golem" (self-corrected next phase). The
protocol "the number must be the FIRST character of your reply... then your PLAN" forces the
choice to commit before the model reasons in the PLAN, so a model that thinks in its PLAN and
changes its mind is already locked to a stale head token.

- This is NOT a prompt-body candidate and NOT a guide candidate — it is a property of the
  code-appended reply-protocol layer. Recorded here only so the synthesis agent routes it, and
  the between-wave "address development notes" step can consider a reply-protocol A/B.
- Candidate protocol experiments (for the notes ledger / a core A/B, NOT adopted here): (a) accept
  the LAST bare number in the reply rather than the first; (b) move the number to AFTER the PLAN
  ("PLAN: ... \n CHOICE: N"); (c) a lightweight reconcile — if the reply text ends by naming a
  specific option index different from the head, prefer the later one. Each is a symmetric-core
  A/B, so win-rate is blind to it; judge by decision-quality (does the recorded choice match the
  reply's own conclusion?). SINGLE instance this corpus — do NOT change the protocol on one datum;
  arm the metric and watch.

## A/B note (already folded by synthesis, no action)
The `hand -> stack` cast-happened anchor removal A/B is adopted per the brief; deck135 shows no
same-name double-cast or cast-nothing regression tied to it (Rule #1 obeyed, no phantom-board).
Nothing for deck135 to add.
