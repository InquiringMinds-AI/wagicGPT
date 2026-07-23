# Skill contribution — deck135 (Modern Snow) seat, wave 21

deck135 remains a FROZEN veteran (guide byte-identical to live, `cmp`-verified). No guide-CONTENT change.
All contributions are METHOD / instrument, valid as single-seat inputs (none adds/cuts a core line).

## PROMOTE (new instrument rung) — the ZONE-OUTCOME reveal-trace generalizes into a "SILENT-FAILURE ACTIVATION TRACE": score an activated ability by the STATE DELTA it produced, not by "the model chose it."

The wave-19/20 reveal-seam skill said: for a `kind:reveal`, score the ZONE OUTCOME in the next record's
events, never "did the model answer." This wave the same discipline caught a DIFFERENT seam: the
fetchland-fizzle bug (notes ENGINE-F1) is invisible to every model-facing signal — the model chose the
fetch, its plan narrates success, no fallback fires, the reply parses clean — and is ONLY visible in the
engine state delta: a `Put in Play with <fetchland>` CHOICE followed by `Your <fetchland> life -N` with
NO `<fetchland>: battlefield -> graveyard` and NO `Snow-Covered X: -> battlefield`. Portable rung: for ANY
chosen activated ability with a cost, score the seam by confirming (i) the FULL cost resolved (each cost
event present) AND (ii) the effect resolved (the intended state change appears) — a cost paid WITHOUT the
effect, or an effect WITHOUT the cost, is a fizzle the model cannot self-report. This is the reveal
zone-outcome instrument abstracted one level: reveals score to-hand/graveyard zone deltas; activations
score cost-paid-and-effect-landed deltas. The tell that promoted it: a whole game (vs35) lost with the
model believing its mana was fine, detectable only by diffing chosen-action against produced-events.

## PROMOTE (regression-attribution rung) — on a FROZEN-guide + UNCHANGED-slate seat, decompose a win-column swing by CHANGE-IN-ENGINE-BUG-INCIDENCE before crediting guide/model/draw.

Extends wave-20's "a validated engine fix can move the record by ~0." This wave's dual: an intermittent
engine bug present in BOTH waves moved the record by -3 purely by striking more games. The instrument: on
a repeated-opponent control, for each flipped game diff the ENGINE state-delta health (here: did fetches
resolve?) between the seat's win and loss games — the bug's incidence, not play quality, is the variable.
deck135's only win is the only game with resolving fetches; the three flips are the three worst
fetch-fizzle games. Rung: a frozen-guide record swing is an engine-bug-incidence hypothesis FIRST,
play-quality hypothesis LAST.

## CONFIRMS (carried, still valid)
- Reveal zone-outcome trace (wave-19/20): ENGINE-R1 confirmed n=2 this wave by the same zone-outcome read
  (vs14 s23: 6 picks -> 6 `goes to hand`, 0 graveyard). The instrument closed its loop cleanly.
- Answer-order leak class (wave-20 HARNESS-1): now has an INVERSE member — the parser DISCARDING a
  not-retracted choice (vs27 s26), not just LOCKING IN a retracted one. The scoring rung "grep clean
  parses for retraction + multiple/absent-final CHOICE" should now ALSO flag clean-parsed
  retracted_choice fallbacks where the retraction token does not reference the CHOICE line. Both
  directions are the same "leaked-at-number / mis-classified-at-number" family.
- Pause-aware WATCH discipline: Force of Negation fired once off-whitelist in a dead game (vs49 s28), no
  board-wipe window — insufficient to clear OR strongly re-trigger the reflexive-Force watch. Stays at
  WATCH (absence/degenerate-firing is not vindication).
