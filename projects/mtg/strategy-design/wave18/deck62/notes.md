# deck62 notes — engine/harness items + rotation verdict (wave 18)

Seat: deck62 (Enchantresses), SECOND guided corpus (revised-guide round 2). 182 decisions across 6
games. Fallbacks 5 (all unparsed_reply): vs14 s21, vs21 s12, vs21 s14, vs21 s17, vs110 s21 (2.7%,
up from wave-17's 3/143 = 2.1%). Giant replies (>8k) 1 -> 8. The giant explosion is real but is
localized to the losing off-case, not a taught-class regression.

## Engine / harness / representation items (NOT guide prose)

- **N1 (representation, CROSS-SEAT) — CLOSED.** The wave-17 sub-menu-as-phase-menu misread (vs135
  s18) did NOT recur. The TARGET CHOICE engine framing works: all 20 target sub-menus parsed to a
  target name, none read as an attack/phase step. Verified dead; logged so the framing is not
  removed. Confirms the KNOWN-CORPUS-WIDE "TARGET CHOICE framing validated at scale."

- **N2 (harness/parser — attacker eligibility filter). PRIORITY, cross-seat.** vs14 s21 and vs21
  s12: the model's ATTACK line named creatures ABSENT from the eligible A-list (summoning-sick),
  and the parser rejected the WHOLE declaration -> heuristic. Correct answer both times: ATTACK:
  none. Fix: when an ATTACK line names creatures, drop the ineligible ones and attack with the
  eligible subset (empty subset => ATTACK: none) instead of unparsing. Converts a whole unparsed
  class into a correct auto-answer; convergent value for every attack-seat that over-names. (See
  general-suggestions #2.) This is the durable fix for the mutated eligibility class that the
  wave-17 guide tightening could not fully close.

- **N3 (representation, computed-P/T distrust) — DOWNGRADE / mostly CLOSED.** The wave-17 concern
  (recounting Forests on a creature ALREADY showing "(printed X/Y)" with Blanchwood) is GONE. The
  surviving Forest-counts (vs49 s26, vs14 s31/s42) are PROSPECTIVE cast-value math — Blanchwood not
  yet on the target, so the model legitimately computes what it will do. That is not the ban's
  target and is not bannable. Re-measure verdict: **the Blanchwood recount tax dropped as planned;
  N3 is retired as a distinct watch.** The residual verbosity now lives inside N4 (the spiral), a
  different mechanism.

- **N4 (representation/behavior — losing-position lethal-hunt spiral). NEW, dominant tax driver.**
  vs21 s14/s15/s17, vs110 s21, vs135 s28 (five >8k replies; three truncated -> unparsed). At 2 life
  behind a high-life opponent, the model re-derives its whole board repeatedly hunting for
  impossible lethal, blows the full token budget, truncates with no CHOICE line -> unparse ->
  heuristic. NOT token-budget-fixable (a bigger budget spirals longer). Primary lever shipped:
  guide off-case branch ("when behind, you cannot always win this turn - do not hunt impossible
  lethal; make the best play and pass"). Secondary representation lever if it survives wave-19: a
  "you cannot reach lethal this turn" annotation is out of scope for the engine (it can't know
  intent), so hold to the guide line + re-measure the giant count at losing seats next corpus.

- **N5 (representation, LOW — redundant global in hand).** Primal Rage in hand while already on the
  battlefield (deck21 runs 3x) amplified the N4 spiral ("why is it in my hand?"). Cheap nudge:
  annotate a hand card whose global effect is already active. LOW; guide carries a stopgap note.

## N3 re-measure verdict (the wave-17 explicit ask)
Wave-17 held N3 pending a wave-18 re-measure of Blanchwood-cast reply lengths. **Measured: the
recount-of-already-printed-P/T tax is gone; remaining long Blanchwood replies are legitimate
prospective cast math or the N4 spiral. The recount-ban WORKED. N3 retired.**

## Rotation verdict
**NOT a rotation candidate — GUIDE REVISED (surgical, round 2) + NEW-WORK-SIGNAL present.** Two
evidence-driven edits: (1) rule-6 off-case branch for the behind/can't-win-this-turn spiral (the
dominant new tax, 5 giants / 3 unparses); (2) A-list rule sharpened to bind eligibility to the list
under "must-attack" pressure (targets vs14 s21 + vs21 s12). Plus a card-note stopgap for the
redundant-global confusion. New-work-signal is strong and durable: the N2 attacker-eligibility
filter (cross-seat harness fix) and the N4 losing-spiral shape both need another corpus to confirm.
deck62 fails BOTH rotation gates (guide-mod = yes, new-work = yes) and STAYS in the pool. Wave-19
must validate that (a) the anti-spiral line collapses the giant count in losing games, and (b) the
sharpened A-list rule stops the must-attack override.
