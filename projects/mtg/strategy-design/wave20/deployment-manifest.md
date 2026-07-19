# Wave-20 deployment manifest

Source of truth for each seat's revised guide: `strategy-design/wave20/deckN/strategy.txt`.
Live target: `projects/mtg/bin/Res/ai/baka/deckN_strategy.txt`.
Every revised file was `diff`'d against the deployed file; the actual diff matches the seat report exactly (verified,
not rubber-stamped). FROZEN files were `cmp`-verified byte-identical.

| deck | action | verification | detail |
|------|--------|--------------|--------|
| 135 | **FROZEN** — no copy | byte-identical to deployed (`cmp` clean) | frozen veteran, guide unchanged since wave-11 |
| 35 | **FROZEN** — no copy | byte-identical to deployed (`cmp` clean) | revision validated; losses decompose to engine + race, none guide-fixable |
| 62 | **DEPLOY** | diff = +5 lines after line 98 (edit D); md5 `e6de5a…` → `e82d4a…` | scope-sharpen edit B: a favorable REAL-creature block tagged "(you kill it, your blocker lives)"/"(both die)" is a trade, not the forbidden 0/1-engine chump — make it and move on. Purely additive (edit B's active tokens frozen). |
| 49 | **DEPLOY** | diff = exactly two hunks; md5 `a7d481…` → `479f90…` | (1) tightened the over-broad "key blocker walling your whole board" burn-exception (a small creature that merely TRADES is not a key blocker → aim at the face); (2) added the develop-seam "DEPLOY OR BURN — EITHER IS FINE, PICK ONE AND COMMIT" tiebreak. Every other line byte-frozen. |
| 14 | **DEPLOY** | diff = one bullet hoisted, +8 lines (lines 60-61 → 60-69); md5 `998e06…` → `fde49b…` | hoisted the buried passive "bounce cannot answer a spell on the stack" into a prominent named trap: "BOUNCE-ON-THE-STACK IS YOUR #1 MISFIRE" with the upstream (don't cast to answer a stack spell) + downstream (battlefield-only menu; never self-bounce) rules. |
| 27 | **DEPLOY** | diff = one insertion, +5-6 lines (after line 18); md5 `c2f0d6…` → `de216c…` | extended the anti-durdle rule to the LATE ground-stall face: when not being raced and life is stable, keep attacking every turn to force trades; a board that never swings loses the turn-clock adjudication. |
| 102 | **DEPLOY (NEW)** | no deployed file exists (guideless newcomer) | initial guide: capability table, Tergrid steal engine (prioritize+protect-when-drawn), the SYMMETRIC-TRAP fire-gate (Pox/Smallpox only when payoff out / ahead / empty; never while behind), named finishers + deploy-and-attack floor, the Thoughtseize pick-ONE surface, reanimation lines. |

## Copy commands (DEPLOY rows)

```
cd /home/magi/Projects/wagicGPT/projects/mtg
cp strategy-design/wave20/deck62/strategy.txt  bin/Res/ai/baka/deck62_strategy.txt
cp strategy-design/wave20/deck49/strategy.txt  bin/Res/ai/baka/deck49_strategy.txt
cp strategy-design/wave20/deck14/strategy.txt  bin/Res/ai/baka/deck14_strategy.txt
cp strategy-design/wave20/deck27/strategy.txt  bin/Res/ai/baka/deck27_strategy.txt
cp strategy-design/wave20/deck102/strategy.txt bin/Res/ai/baka/deck102_strategy.txt
```

deck135 and deck35 need no copy (already byte-identical to the deployed files).

## Discrepancy flags

None. Each revised file differs from its deployed counterpart ONLY as its seat report describes (135/35 identical;
62 +5; 49 two hunks; 14 +8; 27 +5-6; 102 new). No unexpected edits, no freeze-check regressions. deck102 is the only
NEW file (no prior deployment to diff).
