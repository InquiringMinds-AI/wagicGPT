# deck131 wave-6 -> proposed changes to the strategy-writing skill

Mode this wave: OBEYED-BUT-LOSING (record 1/6, obedience high, losses deck-bound). deck131
contributed the OBEYED-BUT-LOSING Step-0 mode in wave 5; this wave surfaces two REFINEMENTS to it.
Both are small; neither cuts or rewrites method. No new top-level method line.

## PROPOSAL 1 (refine OBEYED-BUT-LOSING / Step 0): a "resolved" prior-wave leak needs a PRECONDITION-
## COUNT before it is called fixed - a zero-count can be confounded by the precondition not occurring.

When last wave's #1 lever shows ZERO instances of its target misplay in the new corpus, do NOT record
it as "fix validated" on the zero alone. First count how many windows the fixed behavior's PRECONDITION
even existed - if the precondition rarely occurred, a zero-count is CONSISTENT with the fix working but
is under-powered evidence, not proof (the rare-event-A/B-power trap applied to leak resolution).

deck131 wave 6: wave-5's lever was "cast-nothing while Guttersnipe on your battlefield" (>=3 instances
wave 5). Wave 6 logged ZERO - but Guttersnipe reached the battlefield with a castable instant/sorcery
window in only 3 windows across all 6 games, ALL inside the one win (it was never drawn, or removed on
sight, in the other five). So "0 declines" tested the override in n=3 windows in one game. Report it as
"obeyed where the precondition existed (2 fired / 3 windows / 0 declined), resolution under-powered -
keep the recheck metric live," NOT "resolved." Concretely: add to the Step-0 demotion procedure's
"quantify obedience" step a sub-check - "for a rule whose target misplay VANISHED, also count the
windows where the rule could have fired; a vanished misplay off <5 precondition windows stays on the
watch list at full prominence, it is not demoted." This prevents demoting a load-bearing rule to death
on a lucky-variance zero.

## PROPOSAL 2 (refine OBEYED-BUT-LOSING / Step 3 allocation): SIZE the residual model-fixable lever's
## CEILING before giving it prominence - a lever dominated by the deck-construction residual gets a
## labeled seam block, not the loud #1 slot.

OBEYED-BUT-LOSING mode says "spend the wave on the one residual model-fixable lever." Add: before
allocating that lever the guide's loudest position, estimate its CEILING against the deck-construction
residual - if the construction cause dominates the outcome EVEN AFTER a perfect fix, the lever earns a
sharpened labeled seam block, not the #1 top slot (which would churn a frozen, obeyed rule for little
expected gain).

deck131 wave 6: the residual model-fixable lever was mulligan discipline (2 of 3 no-Mountain openers
KEPT against the guide's own rule, the general keep-default winning over guide precedence). Real and
worth sharpening - but a no-red 7 mulliganed to a 6 off a 14-Island/8-Mountain base STILL frequently
misses red (the one hand that DID mulligan correctly, vs109, still lost red-screwed by turn 9). So the
manabase is the dominating cause and stays the #1 upstream flag; the mulligan sharpen goes in a
promoted MULLIGAN seam block with a by-name override of the general default, NOT the top slot. The
frozen Guttersnipe #1 rule keeps its position.

## Non-proposals (existing method CONFIRMED this wave)
- Threat-before-value core completion: held (every Prism/Elixir cast was a no-threat-available window;
  the pilot never chose value over a castable threat). No change.
- Attacker-denominator = engine `A#.` list (deck131 wave-5 contribution): reconfirmed - 100%
  participation, no phantom subset-attack read. No change.
- Timeout triage (winning-but-slow / genuine-stall / latency-starved): applied cleanly to the vs133
  dead-even grind stall (neither ahead, reached turn 31, 54 decisions at ~11s median). No change.
- Reflexive-high-life-blocking convergence watch: CLEAN this seat (no bad blocks); stays a watch item,
  not promoted. No change.
- Wins-as-mirrors / transcribe-the-corpus-win: applied (the vs135 line is now the guide's "PROVEN WIN
  LINE"). No change.
