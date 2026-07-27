# General (core / general-prompt) suggestions -- deck139 seat, wave-30

## VERDICT: PASS (25th consecutive)

The core/general prompt held cleanly at the mutate seat across 151 decisions in 6 games:
- 0 fallbacks, 0 retries, 0 unparsed replies, 0 truncation-abandons.
- The CHOICE-number contract parsed the entire multi-ask mutate flow (cost-choice,
  normal/mutate, DISCARD-loot, over/under, mutate-target, mutate-trigger) with no drift or
  off-by-one, including the two-word pseudo-options ("mutate over"/"mutate under") and the new
  self-describing "mutate [cast for the MUTATE cost: ...]" option.
- The plan/PLAN carry and the "actions your plan names are no longer among the options" advisory
  fired correctly through the flow.
- The g3 render fixes are surface/representation changes; nothing in them touched the core
  contract. The core prompt is not implicated in any wave-30 finding.

## The latency/reply-length WATCH -- STRENGTHENED by evidence, still not a proposed edit

Wave-29 flagged (for the synthesis owner) a WATCH: the core prompt does not bound reply length,
and mutate/combat produced the campaign's steepest latency tail (six 195-199k ms decisions, a
14,181-char reply). Wave-30 gives that watch harder evidence:
- The tail did NOT shrink after the render fix; it MOVED onto combat math and big-board
  deliberation (max 209,562 ms; a 14,424-char blockers reply -- new campaign high). The render
  fix cured PERCEPTION latency; the residual is intrinsic STRATEGY latency.
- On the wall-clock-capped harness, deck139's 200s decisions are plausibly COSTING games via
  early adjudication (two losses are cap/crash artifacts; the pool had 12 adjudications and the
  slowest seat is disadvantaged by adjudication-behind).
This remains DECK-SPECIFIC stimulus, addressable at the GUIDE layer ("decide combat fast,
commit" -- strengthened in this wave's strategy.txt with the clock rationale) and the RENDER
layer, so it still does NOT argue for a core change NOW. BUT the watch is now witnessed at two
waves and by a second mechanism (adjudication, not just truncation). If a THIRD combat-heavy deck
reproduces the >195k tail, a core-level "state your CHOICE/ATTACK/BLOCK line FIRST, then keep
reasoning brief" nudge (or a soft reasoning-length ceiling) crosses from watch to proposal.
Flagging for the synthesis owner as an escalating WATCH, not an edit. Consistent with the
deck18/deck122 "win column hides the tax -- audit harder" finding: this seat is parse-perfect yet
carries a real, game-costing latency tax.
