# deck131 wave-6 -> core-prompt / general-strategy suggestions

Baseline: the wave-5 general prompt composed into `bin/Res/ai/gpt/system_prompt.txt` (ON THE STACK,
threat-before-value/highest-impact act-floor, mana-discipline, runnable combat block).

## Verdict: PASS. No core-prompt change proposed from this deck's corpus.

Every general-layer feature deck131 exercised performed correctly this wave:
- **Threat-before-value / highest-impact act-floor (wave-5 core edit, partly aimed at THIS deck's Prism
  Ring durdle): CONFIRMED WORKING.** Audited every Prism Ring / Elixir cast (8 total) - in each, no
  threat (Young Pyromancer / Guttersnipe) was in the offered options that window, so the pilot never
  chose a lifegain value card OVER a castable threat. The line either fired correctly or had nothing to
  move. The durdle it targeted did not recur as a piloting error.
- **The COMBAT block** drove 100% attacker participation (vs135) and correct "no blockers" at 18/16/14
  life (survival-first-at-8 respected).
- **ON THE STACK / reaction discipline:** in the vs133 stall, the pilot correctly held Downsize /
  Aetherize / Cyclonic Rift as "Cast nothing" when they had no target and Guttersnipe was not on board
  - correct core behavior, not passivity.
- **Interface contract / transport:** 0 desyncs, no fallbacks, no engine no-ops across 6 games.

Per the layer-routing epistemics (single-seat evidence cannot cut OR add a core line; burden of proof
is on core), nothing clears the bar for a core edit.

## Convergence-WATCH candidate (routed PER-DECK this wave, flagged for cross-deck check): guide
## precedence at the MULLIGAN seam.

The general prompt's mulligan default ("keep any 7 with 2-5 lands and a castable spell") won over the
deck guide's red-source override 2 of 3 times (vs110, vs133 kept no-Mountain hands the guide says to
ship; vs109 correctly mulliganed the same shape). The general line does say "Your strategy guide may
override this," so this is the pilot under-applying guide precedence AT THE MULLIGAN SEAM, not a wrong
core rule. Fixed PER-DECK this wave (promoted + by-name-override mulligan block in strategy.txt). It is
NOT a core change from one seat - but flag for wave-7 synthesis: **if other decks with a
guide-specialized mulligan (e.g. a "ship any hand without a red source" specialization) also show the
general keep-default winning, the mulligan seam may be one where guide precedence needs a core-level
reinforcement** ("at the mulligan decision, a strategy-guide keep/ship rule OVERRIDES the land-count
default"). Until cross-deck agreement exists, per-deck only.

## Routed to INFRA / DECK (see notes.md), not core
- The vs133 timeout is a dead-even (17-17) decision-dense grind stall, not a passivity failure - the
  core correctly did not drive it. No anti-passivity change warranted (would regress a non-problem).
- The record is deck-construction bound (manabase + threat density); routed to notes.md as a decklist
  flag, not a core or guide fix.
