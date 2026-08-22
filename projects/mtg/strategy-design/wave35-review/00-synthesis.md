# Wave-35 tier A/B synthesis — 35B-A3B vs 122B-A10B (2026-08-22)

Rubric (owner, verbatim): "win rate is irrelevant. the question is, how well do the models
follow the provided strategies, and if they diverge from the provided strategies, is it a
good choice to do so at that time." Same seven seats, same guides, both arms; 21/21 full
natural games per arm.

## Per-seat verdicts

| seat | adherence A(35B) / B(122B) | failures A / B | verdict |
|---|---|---|---|
| deck152 | 99% / 98% | 0 / 2 | **35B sufficient** |
| deck146 | 99% / 97-98% | 0 / 2 (one game-losing) | **35B sufficient** |
| deck139 | 97% / 95% | 4 (2 prompt/proto) / 3 + 6 budget-defaults | **35B sufficient** |
| deck105 | 97% / 100% | 2 / 0 | 122B holds (close; re-test) |
| deck158 | 93% / 98% | 3 (2 = guide bug) / 0 | 122B holds (re-test after G-fix) |
| deck116 | 96% / 99% | 2 (one game-losing) / 0 | 122B holds |
| deck36 | 86% / 95% | 8 / 0 | 122B holds (decisive) |

Overall: **not a clean swap today — 4 of 7 seats keep the 122B** — but the margin is
narrow and concentrated at fixable surfaces (see below); 105 and 158 are explicitly
re-testable after the docket fixes.

## Failure-mode fingerprints (the actual tier difference)

- **35B: trusts its priors over printed text at legislated seams.** Every severe 35B
  failure is a misread of an explicit, correct annotation — "(your blocker dies)" ignored
  three seats running (105, 116, 36), generic-mana payability re-derived against the
  engine's own verified cast list (105), Academy tapped on the opponent's turn (36). It
  FOLLOWS guides excellently (adherence parity on 5/7 seats); it argues with the board
  surface. One new defect class: the reasoning-to-answer FLIP (36 A4 seq37: trace decided
  B1:A2, reply emitted B1:A1) — instrumentation candidate.
- **122B: plan-inertia and named-do-not violations, plus budget pressure.** Its failures
  are stale-PLAN-as-reason (152), and the wave's single worst play: Agadeem's Awakening
  X=5 "to return Kaya" (a planeswalker; creature-only spell; no-op; lost a won game) —
  exactly what the guide's named do-not exists to prevent (146 B3). On deck139 it also
  DEFAULTED 6 decisions (3.2%): 22-24k reasoning chars, budget exhausted, empty reply,
  engine default fired — the two-phase rescue does not always recover an answer at this
  depth. Its divergence JUDGMENT, when it diverges, is the best thing on either arm
  (legend-rule redundancy audible, Annihilator sacrifice through a mislabeled prompt,
  mutate-under exposing a guide defect).
- Both arms: mulligans flawless (37/37 engine-count keeps), named-card rules executed,
  poison lethality read correctly.

## Throughput (context)

35B: wave in ~2.5h, decision p50 90-97s, budget hits ~0.2%. 122B: wave in ~29h, p50
885s, budget hits 20%, 0.7% fallbacks. The 35B buys ~10x iteration speed.

## Recommendation

1. Ship the wave-36 fix docket below (prompt contract hardening at the combat seam —
   "the parenthetical outcome line is engine-computed truth", the becomes-blocked trade
   math, the guide bugs), then RE-TEST the 35B on 105/158/116/36. Its failure mass sits
   almost entirely on those surfaces.
2. Until then, a split loop is coherent: 35B for guide-development iteration (fast, and
   sufficient on the seats where guides are the work), 122B for validation waves.
3. The 122B keeps a real edge the fixes cannot transfer: divergence judgment. If the
   owner weighs audible quality above iteration speed, the 122B stays the dev loop's
   validator regardless.

## Consolidated wave-36 docket (cross-seat)

ENGINE: Spellbinder exile-target menu offered ONLY lands vs Oracle "nonland card" (152);
NO attacker asks for 9 turns with untapped creatures (146 B3 — also the grinder's real
cause); Agadeem's offered as a no-op cast with no legal returns (146); Kitchen Finks
hybrid {g/w} castability absent from cast list (116, low-confidence); Ichorclaw
becomes-blocked +2/+2 missing from B-line annotation AND naive-trade line (158 P3 + 139
P1, confirmed game-affecting); 105 poison-final-attack missing blockers ask (possible
translog gap).
PROMPT/RENDER: fused-mode target ask lacks mode attribution (146, unanswerable);
damage-order asks render bogus keyword lists + no reply-format contract (116); "choose
card 6 of exactly 6" sacrifice phrasing induces overrun (36); Nuisance-Engine-at-upkeep
invites guide-divergent activations (36); T1 cast ask labeled "Main phase 2" (36); Clue
renders empty card text (36); mana line invites the generic-cost misread — state "any
colour pays generic" (105); stale hand render after land play (158); combat-seam trust
contract (the 35B's #1 failure surface).
PROTOCOL/INSTRUMENTATION: budget-overrun salvage when phase-2 also comes back empty
(139 B, 6 defaults); reasoning-to-answer flip detector (36); parser number-wins leniency
(139).
GUIDES: deck158 "deaths feed the Army" is FALSE (engine-verified; caused two misplays);
deck139 "ALWAYS mutate over" wrong when cast card smaller than host + Dryad-vs-mutate
curve conflict; deck152 generic blockers hint contradicts deck105's numeric block rule.
