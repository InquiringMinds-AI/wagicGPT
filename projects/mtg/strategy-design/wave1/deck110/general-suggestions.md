# Suggestions for the GENERAL strategy guide (system_prompt.txt)

Based on piloting deck110 and studying its self-play logs. The failures below
were not deck110-specific — they are ways the weak qwen executor systematically
underplays, so they belong in the shared prior. The guide must stay short, so
each addition is justified and paired with a cut or a reword, not just appended.

## The systemic failures observed (generalized)

1. **Deployed an engine piece, never activated it.** qwen cast Cranial Plating
   (equipment) and never equipped it — the card sat in play doing nothing.
   Generalizes to every permanent whose value is an ACTIVATED ability:
   equipment (equip), tap-abilities (e.g. a creature that taps to buff), mana
   rocks, sacrifice outlets, planeswalkers. qwen treats "cast it" as finishing
   the play. Nothing in the guide tells it that a permanent can require an
   action AFTER it resolves.
2. **Tapped out, so it couldn't do the key thing.** Because it emptied its mana
   each turn, it had no mana left to equip / activate. The guide covers holding
   mana for INTERACTION but never for your OWN key ability.
3. **Durdled on redundant support instead of advancing the plan.** It cast a
   2nd and 3rd copy of the engine card instead of a threat, and read "use your
   mana efficiently each turn" as "spend all your mana / cast something." This
   is the same misread the deck8 guide has to fight ("don't dump your hand").

## Suggestion 1 (most important) — add an "activate your permanents / keep mana for it" principle

qwen's single biggest cross-deck leak is deploying synergy and never completing
it. Add ONE bullet to the STRATEGY list (line 15-23). Place it right after the
cards-and-mana bullet (currently line 18), so it sits high:

    - Casting a permanent is not the end of the play. Many permanents do
      nothing until you ACTIVATE their ability - equipment must be equipped,
      tap-to-buff and mana abilities must be used, sacrifice/pump outlets must
      be fired. Each turn, look at your own permanents and take the activated
      abilities that advance your board, and keep enough mana untapped to do
      so. A synergy piece you played but never activated is a wasted card. When
      the same ability is offered again with nothing to gain, pass (0).

Justification: directly fixes failures 1 and 2, and generalizes to any deck
with activated abilities (most decks). It is executable — it tells qwen to look
at its permanents and act, and to hold mana for it. Cost: ~5 lines; pay for it
with Suggestion 4.

## Suggestion 2 — reword the "use your mana efficiently" line so it doesn't mean "dump your hand"

Line 5 currently reads:

    ...develop your mana, use your mana efficiently each turn, trade resources
    favorably, hold instant-speed interaction for the opponent's threats...

"Use your mana efficiently each turn" is read by qwen as "always spend all your
mana," which drives redundant casts and tapping out. Replace that clause with:

    ...develop your mana, spend mana on the plays that advance your plan (don't
    waste it, but don't dump your hand or cast redundant copies just to empty
    your mana), trade resources favorably, hold instant-speed interaction...

Justification: fixes failure 3 at the source and stops undercutting Suggestion
1. Net length change: ~zero (a reworded clause, not a new line).

## Suggestion 3 — append two items to the "common errors" list

Line 23's error list is the cheapest place to reinforce. Append:

    ...; deploying a permanent but never activating the ability that makes it
    good; tapping out so you cannot use your own key activated ability.

Justification: near-free (adds to an existing sentence), and the error list is
where qwen scans for "don't do this." Reinforces Suggestion 1 at low cost.

## Suggestion 4 — cut to make room: trim the information/bluffing bullet

Line 20 (the whole "Control the opponent through the information you show them
... back the representation with the real card ... so the bluff stays credible"
bullet) is the guide's most abstract, least executable content. Credible
bluffing — representing an instant you don't hold, leaving mana open for its
psychological effect, backing it later — is a subtle human skill a weak
one-decision-at-a-time model cannot reliably run, and against the symmetric
self-play opponent it earns little. Cut it down to a single retained sentence:

    - Leaving mana untapped represents an instant and can deter the opponent's
      attack; back it with a real card when you have one.

Justification: recovers ~3 lines to fund Suggestion 1, and removes guidance
qwen is unlikely to execute well anyway. The one kept sentence preserves the
concrete, actionable core (open mana deters attacks).

## Net effect on length

Adds ~5 lines (Suggestion 1), ~0 (2), ~1 (3); removes ~3 (4). Roughly
length-neutral, and it replaces the guide's least-executable content with its
most-needed missing rule. No other additions — attention is the budget; keep
the rest of the guide as-is.

## Considered and deliberately NOT suggested

- A "never target yourself with your own damage/removal" line: in the logs qwen
  correctly avoided self-targets even when offered, so this is not an observed
  failure and would spend length on a non-problem.
- Rewriting the strategy section to be more concrete/imperative overall: higher
  risk, out of scope, and better handled per-deck in {STRATEGY_GUIDE}, which is
  where deck-specific reflexes belong.
