# deck22 (Giants!) wave-28 notes

## R-ANNOTATION-ECHO-PARSE: CLOSED (e5 verified live)
The wave-27 ledger item (in-band render annotation echoed into the structured answer trips
c4 name-match -> stale_echo fallback) is DISCHARGED by the e5 annotation-echo tolerance.
This corpus: 5 CHOICE-line annotation echoes (4x `[changeling: counts as Giant]` at vs134/18/
93/136 s3, + 1 NEW form `[land] [your hand]` at vs136 s11) - ALL parsed clean, choice matched
the echoed number, ZERO fallbacks at the seat. The model echoes annotations MORE than in
wave-27 (5 vs 1), so the tolerance is genuinely exercised, and it generalizes past the single
`[changeling...]` shape that motivated the fix. No further parser work at deck22. Keep the
representation-lever <-> parser coupling documented as a SOLVED authoring lesson (skill.md).

## Steady-state validations (no bug)
- Automaton `[changeling: counts as Giant]` render present + consumed on every hand/attacker
  surface; d1 duplicate-name `#N` handles exercised again (vs18 s11 Automaton #1/#2; vs93
  s13/s16/s19 five-way #N attacker list) - collision-only, stable, model uses `#N` in
  reasoning. No binding error.
- c1 Inferno Titan face-split enumerated + all-to-face picked (vs93 s26-28 -> opp 12->9;
  vs146 s21-23 -> opp 8->5). No truncation.
- Land identity tags (e4) render clean; benign in replies; one `[land] [your hand]` echo
  parsed (vs136 s11).
- No timeout, no draw, no phantom-chooser, no "Flip Side" pseudo-action, no engine-illegal
  ask at this seat.

## Optional (NON-BINDING, NOT applied - freeze preserved through rotation)
Mulligan over-inclusion: the guide's MULLIGAN line lists Stinkdrinker Daredevil as a keepable
"early play," but a hand of ONLY Stinkdrinkers with no Giant/Automaton is a functional mull
(cost-reducers with nothing to reduce). vs146 s1: the model correctly MULLIGANED
3-Mountain + Mogg + 3x-Stinkdrinker, kept a 6, and WON. As with the wave-27 Mogg
over-strictness, the model already deviates correctly WITHOUT a carve-out, so the rigidity is
not binding. A carve-out ("all-Stinkdrinker, no-Giant hands are a mull") would reset the
freeze clock for zero observed benefit. Archived, not edited.

## ROTATION VERDICT - ROTATE OUT (applied with finality)
Wave-27 projected the wave-28 exit; the conditions it named are all met:
- Guide FROZEN two consecutive corpora (wave-27 + wave-28), cmp-verified byte-identical to
  deployed AND to wave-27. No guide modification was needed or made either wave.
- All three teaches (Automaton identity, attack floor, Mogg role) VALIDATED AGAIN this corpus
  (2nd validation) - zero regression on the frozen guide.
- The only engine-layer new-work signal (R-ANNOTATION-ECHO-PARSE) is now CLOSED by e5.
- The 5/6 -> 3/6 record drop is FULLY explained by pool-strengthening (deck93 +2/6; deck137
  convoke-enabled by e1) + deck22 draw variance. Both losses were correct-play losses. The
  record drop alone does NOT hold the seat (per doctrine); there is NO guide-addressable
  decision class among the losses.
- The two model deviations this corpus (vs93 s22 Mogg attack; vs146 s1 mulligan) are correct,
  value-positive, and already handled WITHOUT guide change - non-binding optional refinements,
  not new guide-layer work.
Termination = new-work exhaustion, and it is exhausted here. deck22 ROTATES OUT. Return the
seat to the guideless roster (~140). The frozen guide stays deployed as-is.
