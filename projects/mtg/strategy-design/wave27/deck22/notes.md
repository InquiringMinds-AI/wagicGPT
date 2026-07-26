# deck22 (Giants!) wave-27 notes

## d2 R-CHANGELING-TYPE-INFERENCE: FIX VALIDATED, item CLOSABLE at this seat
The wave-26 ledger item is DISCHARGED for deck22. Annotation renders verbatim on
hand/battlefield surfaces (`Universal Automaton #1 {1} (1/1) [changeling: counts as Giant]`),
the pilot consumes it (echoed it into a CHOICE line, vs131 s3), and the misread it targeted
died: 6/6 keeps, zero "no other Giant" mulligans/bottoms. Keep the class documented for any
FUTURE changeling/type-matters deck, but no further work needed at deck22.

## NEW LEDGER ITEM - R-ANNOTATION-ECHO-PARSE (parser, LOW, safe-direction, deck22 seat)
The d2 in-band annotation induces a c4 name-match fallback when the model echoes it into the
structured answer. Repro: vs131 s3 (turn 1, Main phase 1 cast) - reply first line
`CHOICE: 1 (Cast Universal Automaton {1} (1/1) [changeling: counts as Giant])`; canonical
option is `Cast Universal Automaton {1} (1/1)` (no bracket); the number+name reconciliation
failed on the extra `[...]` tail -> stale_echo fallback -> heuristic answered (Baka cast the
Automaton anyway; game WON). 1/146 decisions, benign.
- Candidate fix: in the answer-name reconciliation, match on choice NUMBER + card-name PREFIX,
  or strip bracketed `[...]` annotation tails from the echoed name before comparison. General:
  ANY in-band render annotation the model can see, it can echo - the answer parser must be
  annotation-tolerant. This is the first case of a representation lever inducing a parser
  fallback (cross-lane coupling; see skill.md).
- Priority LOW: safe-direction, single occurrence, no game impact. Fix opportunistically when
  next touching parseChoice.

## Steady-state validations (no bug)
- c1 target enumeration: Inferno Titan 3-way face split fully enumerated + picked, no
  truncation (vs136 s12-14, s20-22, s25-27, s39-41).
- c3 PT-pump delta+result: firebreathing renders `+1/+0 until EOT (6/6 -> 7/6)` and STACKS
  correctly across successive pumps up to `(8/6 -> 9/6)` (vs136 s28/29/35/36/37). No conflation.
- d1 duplicate-name instance handles ALSO exercised at this seat: two Universal Automatons
  render `#1`/`#2` on the battlefield line, collision-only, stable. No binding error.
- Borderland Behemoth base-P/T representation note (wave-26 carry): not exercised as a
  misvalue this corpus; still WATCH-only.
- No engine bug, no timeout, no draw at this seat.

## ROTATION verdict - PROJECTED wave-28 (near-terminal; freeze the guide)
- Guide FROZEN byte-for-byte this wave (cmp-verified identical to deployed
  bin/Res/ai/baka/deck22_strategy.txt). All three teaches validated at 5/6 in one guided
  corpus; no guide modification was needed.
- Remaining new-work signal is ENGINE-layer only (R-ANNOTATION-ECHO-PARSE) - routed to the
  ledger, not the guide. The Mogg over-strictness is a marginal OPTIONAL guide refinement
  (add the Stinkdrinker-style "unless lethal / nothing can block" carve-out to the Mogg rule);
  deferred because the model already deviates correctly WITHOUT it, so the rigidity is not
  binding. Changing it would reset the freeze clock for no observed benefit.
- Verdict: NOT rotated this wave (FIRST-GUIDED, only one validation corpus - matches the
  deck18/deck59 pattern where the rotation test applies FROM NEXT WAVE). deck22 is a
  wave-28 rotation candidate IF the guide stays frozen and no new guide-layer signal appears;
  the parser item does not block rotation (engine-layer, worked between waves). Apply
  unsentimentally next wave.
