# deck62 notes — engine/harness items + rotation verdict (wave 17)

Seat: deck62 (Enchantresses), FIRST GUIDED corpus. 143 decisions across 6 games. Fallbacks
**3 (all unparsed): 2 vs135, 1 vs110** (~2.1%) — down from wave-16's 17 (~11.6%). Giant replies
(>8k) 14 -> 1. The guide's primary validation signal (tax collapse) is unambiguous.

## Engine / harness / card-script items (NOT guide prose)

- **N1 (representation, CROSS-SEAT — sub-menu misread).** vs135 s18: prompt "Choose the target for
  Web" listing 3 own creatures; the model ignored it, wrote an attack PLAN, emitted "CHOICE: 0
  (Attack ...)" -> out of range -> unparsed. Same SHAPE as deck135's multi-target sub-menu cluster
  (its charge). A single-target aura sub-menu is being read as a phase decision. Candidate fix:
  make target sub-menus visually distinct from phase menus (e.g. header "TARGET CHOICE (pick one
  creature below)" and reiterate it is not an attack/cast step). Cross-seat -> representation layer.

- **N2 (harness/parser — no-block token gap).** vs110 s5: correct no-block decision, but the model
  wrote "BLOCKS: (no assignments)" and the parser rejected it -> unparsed -> heuristic. The blockers
  prompt only demonstrates a per-blocker "B2:none" and has NO all-none example, whereas the
  attackers prompt has an explicit "ATTACK: none". Fix: add an explicit no-block token+example to
  the blockers prompt (e.g. `... or "BLOCKS: none" to block with nobody`) AND accept the common
  paraphrases. Cheap, removes a whole unparsed class. Convergent-value with any seat that declines
  blocks.

- **N3 (representation, tax driver — computed P/T distrust; carried from wave-16 N2).** The model
  re-derives Blanchwood Armor's "+1/+1 per Forest" from the land count at the CAST step even though
  the battlefield line shows "Saproling (6/6) (printed 1/1)" (vs49 s8 10528, s16 9962, s17 12710 —
  the corpus's single largest reply). The guide now names this explicitly (trust-line tightened);
  if the tax survives, the low-priority representation nudge is to annotate computed P/T as final,
  e.g. "(6/6 — Forests/auras already counted)". Held pending the wave-18 re-measure. No code change
  proposed yet.

- **N4 (positive — verify only).** The "only legal targets are YOUR OWN" / "legal targets right
  now: <names>" annotations are behaving correctly and are load-bearing for this seat (see
  general-suggestions #1). No change; logged so it is not accidentally removed.

## Rotation verdict
**NOT a rotation candidate — GUIDE REVISED (surgical) + NEW-WORK-SIGNAL present.** The wave-17
guide is a first-corpus revision per Step 0-ter(3): two evidence-driven TIGHTENINGS (Blanchwood
trust-line naming the printed "(printed 1/1)" annotation; attack-list rule "never name a creature
absent from the A-list" — directly targets vs49 s17 tax and vs135 s19 unparse) plus a light trim of
the now-validated-DEAD aura-targeting section (the engine annotation carries it). New-work-signal is
also present: the MUTATED eligibility class and two engine items (N1 sub-menu, N2 no-block token).
deck62 fails BOTH rotation gates (guide-mod = yes, new-work = yes) and STAYS in the pool. Wave-18
must validate that the Blanchwood tax and the summoning-sick over-attack drop.
