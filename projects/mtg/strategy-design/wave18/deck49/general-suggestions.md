# general-suggestions.md — deck49 (Dragons) wave-18 (core / general-prompt items)

Single-seat items cannot add/cut core lines (doctrine). These are candidates for the synthesis
layer to weigh against other seats; none is a deck49 guide change.

## G-49a — residual reasoning-tax: combat-math + stale-plan spirals at hard decisions (CORE)
The fallback tax collapsed to zero, but 20/125 deck49 replies still ran ≥600 words (peak 2,800 at
vs21 s5; 2,523 at vs110 s11), all resolving to CORRECT, parsed choices. Two shapes, both core-level:
  (1) **combat/lethal-math loops under pressure** — vs110 s11 (at 2 life) produced a ~2,500-word
      block that re-derives the same attack five times before landing (correctly) on Lava-Spike-the-
      Signal-Pest. The math is right; the model just cannot stop re-checking it.
  (2) **stale-plan re-derivation** — vs21 s5 opens "My previous plan to 'play land then wait' was
      incorrect because…" and spirals 2,800 words re-planning the turn. This is the PLAN-caveat class
      firing WITHOUT going unparsed (good) but at a large token cost.
Both are general-prompt concerns (a "you have already done the math / committed the plan — state the
choice" brake), NOT deck-specific. Flagged for the synthesis; corroborates other seats' giant-reply
pockets. Note: this seat's spirals stayed parsed, so the PLAN-caveat class is NOT among the corpus's
unparsed at deck49 (0 unparsed here) — consistent with the brief's caveat-class expectation.

## G-49b — E-49c suppression: this corpus is evidence for the owner's annotation redesign (HIGH ledger)
The owner ruling replaces both effectBadOrGood suppressions with warning ANNOTATIONS. deck49 is a
clean data point on BOTH sides of that ruling:
  - The suppression WORKED as a loop-preventer: enemy-only Goblin War Paint was never offered, so the
    fatal wave-17 C4 loops (deck17 s18, deck135 s22) had zero recurrence.
  - BUT it also removed a legal play the pilot might rarely want (War Paint on an enemy creature is
    legal, if near-never correct) — the exact objection behind the ruling.
  - The engine ALREADY ships the annotation STYLE we want, on the own-target side: vs35 s9 offered
    "Cast Goblin War Paint {1}{r} - the only legal targets are YOUR OWN right now." The redesign
    should mirror that phrasing for the enemy-only case, e.g. "Cast Goblin War Paint {1}{r} - WARNING:
    the only legal targets are the OPPONENT'S creatures; buffing them helps THEM." That single line
    gives the model the representation to decline without a loop, and keeps the play legal.
This strengthens the annotation design; deck49's never-aura guide rule is retained to pair with it.

## G-49c — burn/player target-menu label (LOW ledger, VALIDATED-CLOSE)
The wave-17 "(player, life N)" suffix-strip (E-49a) is validated at this seat: 17/17 burn/player
target menus parsed correctly with zero wrongful downgrades (was the deck21 s4 wrongful downgrade
last wave). No further action; record E-49a as closed by corpus.

## G-49d — construction, not a prompt item (NOTE for deck-design, not core)
All four losses trace to deck49's top-heavy curve (early clock is a 1/1 that needs 6 lands; real
threats at 4-6 mana; only two {6} finishers, thin 3-cost body). This is a DECKLIST property, outside
the strategy-guide and core-prompt surfaces. Recorded so the synthesis does not misattribute deck49's
2/6 to a prompt regression. If deck rebalancing is ever in scope, a 3-drop flyer or a second cheap
evasive threat is the lever; not actionable from here.
