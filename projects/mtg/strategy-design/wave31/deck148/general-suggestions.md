# deck148 (wave 31) - core / general-prompt suggestions: **PASS**

**PASS - the 26th consecutive PASS from this seat's lane.** Nothing observed at deck148 this corpus
warrants a change to the shared system prompt or the general strategy text.

Evidence for the PASS, not merely the absence of complaints:
- **0 fallbacks across 187 decisions** (91 ask / 34 attackers / 37 priority / 11 blockers / 2
  bottom). No deferred, unparsed, retracted, truncated or out-of-range reply anywhere in the six
  seat logs. Second consecutive fully clean corpus at this seat.
- **0 parser contradictions.** Every `choice` matched the reply's stated CHOICE line; no
  first-vs-last divergence, no prose-reversal, no stale-echo, no numeric-offset.
- **The core's combat framing did its job.** The blockers header's "NOT lethal: block only where the
  trade favors you; taking damage while ahead is often correct" was present and correctly phrased at
  vs152 seq10 - the model simply overrode it with a deck-level value judgment the *guide* had not
  supplied. That is the guide/skill lane's problem, not the core's; the core said the right generic
  thing.
- **Reply protocol held under long deliberation.** Six replies exceeded 10k characters (max 14,120,
  vs139 seq21) and every one still emitted a well-formed CHOICE and PLAN line. The protocol's
  code-appended tail is robust at the length where it matters.

Two items were considered for core routing and deliberately rejected:

1. **Add card-value weighting to the blockers `(both die)` annotation.** Rejected. The annotation is
   a mechanical combat-math statement and is CORRECT; making it opine on strategic card value would
   require the render to encode deck-plan knowledge it cannot have, and would apply a deck-specific
   judgment to every seat. The right home for "this particular body is worth more than the trade
   says" is the per-deck guide, authored by the skill (see skill.md proposal 1). Routing it to the
   core would violate the layer boundary in the direction the pipeline has consistently refused.
2. **A general "do not trade your engine" directive in the core prompt.** Rejected for the same
   reason plus the wave-25 headline-5(c) rule: a directive that fires at every seat and every seam
   to address a decision only some decks face is a core touch that buys noise. The deck that has an
   engine knows it from its guide.

Four representation/engine items DID come out of this seat (N-148a..d) - all are routed to notes.md
as engine-lane ledger items with seq repros, none of them touch the general prompt text.
