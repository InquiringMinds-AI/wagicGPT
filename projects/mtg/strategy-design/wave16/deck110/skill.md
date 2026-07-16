# Skill delta (wave 16) — deck110 seat contribution

CURRENT skill = `strategy-design/wave15/strategy-writing-skill.md`. Wave 16 at the deck110 seat is a
VALIDATION + FREEZE wave (9th consecutive guide freeze). The seat proposes **NO new guide-authoring
method headline** and **NO cut/edit to any existing method rung**. Every item confirms an existing
rung; one offers a cross-seat SYNTHESIS candidate at the HARNESS layer (not a guide-authoring rung).

## Confirmations of existing method (all PASS again)

- **Method headline 3 (verify RESOLVED effects against the game-log events, never the model's asserted
  magnitude) — the Galvanic rung is re-validated at full corpus scale.** 11 Galvanic Blast casts, both
  metalcraft arms correct (2 at N<3, 4 at N>=3), 0 errors — read by COUNTING `Damage:` instances, not
  the model's prose. The wave-15 verification CAVEAT held literally: my first (string-dedup) pass
  collapsed the two identical `Damage: 2` lines and mis-reported metalcraft Blasts as "2"; the
  instance-count pass showed `2,2`. Re-derive the success signature from the FIXED engine's own output
  (two-instance shape) before counting — the pre-fix grep silently mis-scores a working fix.

- **Method headline 2 (score attacker declarations by parsed intended-vs-recorded, durable fix =
  name->index reconcile respecting eligibility) — re-validated.** 17 attacker windows; every eligible
  named creature unioned in (MIXED `A1, <name>` and pure-index forms both), every summoning-sick name
  excluded (deck135 s6 `A1, Steel Overseer` -> only Signal Pest kept). No eligible attacker dropped.
  Reconstruct against the eligible roster + declared-attacker log, never the fallback flag.

- **Method headline 5 / wave-10 (a no-op/low-value marker on a REPEATABLE move/attach must be computed
  from OUTCOME DELTA, not target identity) — VALIDATED BY ABSENCE.** No A->B->A churn recurred; the one
  MOVE (deck135 s15, Signal Pest->Steel Overseer) was a genuine UPGRADE (7->9 power, tougher body) that
  the outcome-delta lever would correctly ALLOW, and every `(ALREADY)` re-offer was passed. Classify
  equip decisions by outcome-delta, never raw same-name count — the rule cleanly allows upgrades and
  rebuilds while it would still suppress the wave-15 downgrade-bounce. Engine ledger, never a guide line.

- **The "run the representation rung on the MENU, not just the board" sharpening (wave-7 headline 3)
  held.** E6 (Mox Opal cast-with mislabel) simply did not appear on any menu this wave — the seat can
  only report non-exercise, not confirm/deny. Not-exercised != not-fixed; the menu is where you look.

## Cross-seat SYNTHESIS candidate (single-seat, HARNESS layer — offered, not a guide-authoring headline)

- **The `YOUR PLAN (as you last stated it)` carry is a likely ROOT of the PLAN/CHOICE-mismatch family
  AND the reasoning-tax loops (folds under wave-11 headline 1's "single root," at the harness layer).**
  Wave-11 headline 1 established that the answer-after-plan trailing PLAN/CHOICE mismatch has a single
  root. This seat exposes a NEW contributor to that root: the prompt re-injects the model's PREVIOUS
  full PLAN prose, and when the game state has advanced past it (card already cast, phase/menu changed,
  or the prior plan ended mid-"Wait..."), the stale carry (a) drives the CHOICE to contradict the prose,
  (b) seeds ~14k-char unparsed spirals when it contradicts the current prompt (deck21 s13, deck133 s9),
  and (c) in the worst case makes the model pick the option whose NAME matches the stale plan — deck21
  s7 targeting its OWN Steel Overseer with Galvanic Blast because the carried plan said "cast Steel
  Overseer" (a real, game-costing self-blast). Method note for reviewers: when auditing a PLAN/CHOICE
  mismatch or an unparsed loop, CHECK THE CARRIED PLAN against the current menu — a plan that names an
  action absent from the menu is the tell. This is a HARNESS/prompt-assembly item (owner
  `AIPlayerGPT.cpp`), NOT a guide-authoring rung and NOT guide-fixable; carrying only the last resolved
  `CHOICE:` line (not the rambly PLAN tail) is the candidate fix. Cross-seat question for synthesis:
  do other seats show PLAN/CHOICE mismatches or unparsed loops traceable to a stale carried plan?

## No change to core
`general-strategy.txt` core prompt untouched from this seat (12th consecutive PASS). No 2+-seat core
candidate surfaced. The false-belief family (colored-pays-generic) produced zero offered-payable
declines at this seat again. The wave-15 engine batch (free-cast-at-zero-mana, stale_echo subset
matcher, hybrid-pip annotation) had no material exercise at this mono-{cost} artifact seat.
