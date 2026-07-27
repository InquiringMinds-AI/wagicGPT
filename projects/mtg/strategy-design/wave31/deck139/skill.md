# deck139 -- wave-31 proposals for the strategy-writing skill

Two proposals, both single-seat this wave and tagged as such.

---

## P1 (PROMOTION-GRADE candidate) -- Trace a fix to its TRIGGER CONDITION, not just to a clean
## corpus: "no crash" is the absence of a symptom, not the presence of a validation

Wave-30 promoted arrival-tracing over counting the fallback field (method headline 1) and separately
established UNEXERCISED as a first-class verdict (headline 4). deck139 wave-31 is the case where the
two RULES MEET, and the meeting point has a sharper rule in it than either parent.

The wave-31 brief said: *"the corpus itself is the validation -- any crash/abort at your seat is a
regression finding of the first order ... deck139: Pollywog-mutate casts should now RESOLVE."* Both
halves came back clean: zero crash signatures, and 4/4 offered-and-chosen mutates arrival-traced to
the merged-pile render. Arrival-tracing alone -- the wave-30 instrument -- would have closed the
item. It would have been wrong. **Not one of the four traced mutates was a Pollywog-discounted
cast.** Pollywog reached the battlefield in 1 of 6 games, its one discounted offer was declined, and
it died before the mutate that was taken. The corpus proved the mutate cast path healthy and said
nothing whatsoever about the code that crashed.

The rung: **before you accept a crash/regression fix as revalidated, write down the fix's TRIGGER
CONDITION as a board state, then search the corpus for that state -- not for the symptom and not for
the surrounding feature.** A crash fix's trigger is a conjunction (here: mutate spell + Pollywog on
the battlefield + payment actually made); the feature around it (mutate casting) fires far more
often than the conjunction and will look like coverage. Concretely:
- Write the trigger as a grep-able predicate over the translog before you start reading
  (`'Pollywog' in the "Your battlefield" line` AND a chosen option containing `mutate cost`).
- If the count is 0, the verdict is **UNEXERCISED**, and the deliverable is a PROBE SPEC, not a
  rotation recommendation.
- A crash fix has an asymmetric payoff that makes this mandatory rather than nice: the symptom
  (abort) exists only in a debug build. In release the same underflow computes a wrong cost
  silently, so "the corpus did not crash" is compatible with the bug being fully live.

Why this is not a restatement of headline 4: headline 4 covers a seat rotated in to exercise a
mechanic that never fired, where the null is obvious because the mechanic is the seat's whole
identity. Here the mechanic (mutate) fired FOUR TIMES and validated cleanly -- the null is hidden
INSIDE a successful validation, one conjunct deep. That is the failure mode worth a named rung.

Suggested placement: as a third face at the arrival-tracing rung (Step 2 REALIZED-MAGNITUDE column),
plus one line in the DoD -- *"for any fix whose symptom is a crash, an abort, or a silent no-op:
state the trigger condition and report the corpus count of that state; 0 = UNEXERCISED."*

---

## P2 (single-seat, tagged) -- A misread the guide already has a clause for is a WRONG-EXEMPLAR
## defect, not a compliance failure; re-aim the clause at the cards that actually failed

deck139's wave-30 guide carried `A Forest is a land - never call a hand with lands "no lands."` The
wave-31 corpus then shipped three guide-satisfying opening hands, and the pilot's stated reasons
were *"zero lands ... despite having three dual lands"*, *"zero lands and five high-cost cards (...
three dual lands ...)"*, and -- with a literal Forest in hand -- *"no Forests"*. The clause was
present, on-topic, and did nothing.

The reflex read is "the executor ignored the guide." The evidence says something more useful: the
clause names the ONE card in the failure class that the executor gets right almost every time, and
names none of the five it gets wrong. Its exemplar is doing no work, so the sentence never matches
the moment it was written for. The rung:

**When a documented teach fails, diff the CARDS/STRINGS in the teach against the CARDS/STRINGS in
the observed failures before rewriting anything.** If they do not overlap, the fix is to re-aim the
exemplar (enumerate the actual offenders by printed name, and quote the render substring the pilot
must read -- here `(land: taps for {R}{G})`), not to make the sentence louder. A generic teach
carrying a non-representative example is a LATENT misteach in exactly the wave-30
stopgap-retirement sense: it reads correct and it is inert.

Corollary observed here and worth one line wherever mulligans are discussed: **a mulligan teach with
no numeric floor is an unterminated pursuit** in the Step-5 sense -- the pilot's own PLAN lines
pre-authorised the spiral (*"If the 5-card hand is still unplayable, mulligan again to 4"*) and one
game reached seven mulligans and a 3-card keep. The keep criterion is a pursuit; it needs a
countable terminator ("at most two mulligans, then keep") the same way a brake needs a release.
Suggested placement: the pursuit/release rule in Step 5, as a named mulligan instance.
