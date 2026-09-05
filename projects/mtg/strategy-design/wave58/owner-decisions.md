# Wave-58 owner decisions

## 2026-09-03 — D42 board grouping (lane G, vpk17/18): REJECTED as shipped
Owner, verbatim, after playing token decks on vpk18: "nah, this stacking is not the answer. i need to
think about it, but its really obnoxious like this". Status: he is thinking; NO redesign is to be
started until he brings a direction. The feature stays behind its Options toggle (BOARDGROUPING,
default ON for the human seat); nothing else changes. D37 (console/touch/divided-damage verification
of the grouping) is MOOT until he rules.

Follow-up, owner verbatim: "first there are issues in implementation. it's stacking and unstacking
the opponents land when they arent manipulating those lands. the indicators are not remaining on
top, so sometimes not visible, especially when targeting the stack. then seperately from the
implementation, its not visually intuitive."
=> TWO IMPLEMENTATION BUGS (queued for the next step one IF the feature survives his think):
  G1. Opponent's lands stack/unstack (churn) while the opponent is not manipulating them —
      the grouping key or the regroup trigger is unstable across frames/state that should be
      identical (suspects: tap/untap during the AI's mana payment, the "unique state" split
      flapping on transient attributes, or regrouping on every render instead of on change).
  G2. The stack-count indicators do not stay on top of the pile — hidden by later-drawn cards,
      worst when the stack is the current target (CardSelector focused-card re-render draws
      over overlays; same class as the castableNow flag lesson: draw inside CardGui::Render, not
      as an overlay).
=> DESIGN VERDICT: "not visually intuitive" — separate from the bugs; he is thinking; no redesign.

## 2026-09-05 — D42 grouping while targeting (owner ruling, verbatim)

On the first fix ("only piles with a member the open chooser can legally target fan out"):
"wrong. it should remained stacked, unless one is targeted, in which case only the targeted
object becomes unstacked, since it is now uniquely targeted."

Applied: no expansion for legal targets; a pile is targeted as a pile (its leader). A card
wearing a target/source rim leaves its pile alone; the rest stay stacked. The OK-press-expands
rule (wave 57) is retired. Focus (cursor inside a pile) still fans it for navigation.
