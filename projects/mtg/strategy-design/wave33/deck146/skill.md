# deck146 -> proposals for the strategy-writing skill (wave 33)

Four proposals, all earned by this corpus. P1 and P2 are the load-bearing ones; both concern the
HL1 audit's SCOPE, which failed at this seat in a new way.

---

## P1 (RUNG, HL1 amendment) -- the audit's third source: the audit must check the guide against the CURRENT ENGINE, not only against Oracle and the primitive

**Current rung.** HL1 requires every card fact in the guide to be verified against real Oracle text
AND the primitive's `mana=`/`text=`/`auto=` before a seat may exit.

**What that missed here.** Both false facts this wave passed an Oracle-and-primitive check *at the
time they were written* and became false afterwards, for two different reasons:

1. **A crutch outlived its defect.** The guide's "Vanishing Verse CANNOT target an ARTIFACT or a
   LAND" was TRUE of the wave-32 binary and was authored deliberately to route around N-146j. The
   wave-33 step-1 batch fixed N-146j. Nobody re-read the guide against the fix, so the deployed
   guide shipped a false absolute into the corpus that was validating the fix, and the pilot recited
   it (vs36 s15) and once inverted it (vs116 s37: *"Vanishing Verse is useful for hitting lands"*).
2. **A script transcription was off by one.** "Hive enters tapped if you already control MORE THAN
   TWO other lands" was transcribed from `compare(type:land:myBattlefield)~morethan~2` -- but the
   entering land counts itself, so the engine taps at TWO others, exactly as Oracle says. Reading
   the script literally and reading Oracle would BOTH have caught this if either had been checked
   against a live render; neither was.

**Proposed rung.** HL1 gains two mandatory steps, both cheap and both mechanical:

- **(a) CRUTCH RE-READ.** Before the audit, list every guide line that exists to route around a
  known engine defect (they are identifiable: each one names an in-game restriction the card's
  Oracle text does not have, and each should already be in the seat's notes.md as "demote when
  fixed"). For each, check the CURRENT binary for the fix -- the ledger entry, the diff, or the
  corpus. **A fixed defect makes its crutch a false teach on the spot, and false teaches are the
  audit's whole subject.** The demotion is not optional and does not wait for a witness: the
  wave-33 corpus never showed this seat a monocolored artifact, and the teach was false anyway.
- **(b) RENDER CROSS-CHECK for any quantity.** Any number in the guide that the corpus can witness
  (a tapped-land threshold, a cost, a P/T, a room count) is confirmed against a LIVE RENDER from
  the corpus, not only against the script. One line of evidence per number: "vs139 seq6: board =
  Plains, Swamp, Hive [tapped] -> two other lands, tapped." Script-only transcription has now
  produced a wrong number twice (this seat, wave-32 and wave-33).

**Why this is a rung and not a note.** The campaign's audit has been finding *authoring* errors and
is now clean of them at this seat -- all three of this wave's failures are *drift* errors, which
the current rung structurally cannot catch, because it validates the guide against the world and
never against the delta.

## P2 (RUNG) -- a guide teach that references a RENDER STRING must be re-checked when the render changes

The guide's mulligan section spent four lines teaching the pilot how to read
`Mana available: 0 total (no untapped sources)`. i9 deleted that line from every pregame prompt.
The teach is now dead weight in the guide's highest-stakes paragraph, and worse, it tells the
pilot to look for a line the engine will never show it -- an invitation to reconcile the guide with
an absent surface, which is exactly the deliberation shape that costs 200 seconds elsewhere in this
corpus.

**Rung:** when a guide quotes a render string verbatim (and Step 5 encourages that, correctly),
the reviewer records it as a *dependency*, and the next wave's audit greps the corpus for the
string. **Zero occurrences = the teach is retired, not reworded.** Concretely: three greps at
audit time (`Mana available:`, `{card text:`, `{room effect}` at this seat), each answered with a
fraction, not a sample.

## P3 (ADAPT, Step 5 wording) -- route AROUND an underspecified surface with a rule that does not need the missing value

The trust doctrine already forbids teaching doubt about a render ("the number may be wrong") and
requires routing around a false surface with a decision rule that does not depend on the broken
value. This corpus produced the neighbouring case: a surface that is not FALSE but INCOMPLETE (the
dungeon room menu names two rooms and never says which room you are on), and the pilot burned
214s and 213s trying to complete it from narration.

The same route-around applies verbatim and is worth stating in the skill as its own case, because
"incomplete" does not trip the existing wording: give the pilot a rule that is decidable from what
IS on the option line, plus an explicit instruction that the missing fact is not needed -- e.g.
*"decide on the {room effect} text alone; you do not need to know which room number you are on, and
do not try to work it out."* That last clause is the load-bearing half: without it the model treats
the gap as a puzzle it is obliged to solve. It is NOT a hedge against the render, it is a
declaration that the render is sufficient for the decision -- which is true.

## P4 (NOTE, HL10 instruments) -- an index-only commit is invisible to every existing instrument

`fallback` is absent, `commit_retracted` is false, `post_plan_overrun` is 0, and the record looks
like a clean decision -- but the model answered a different question and the parser committed an
option it never named (vs116 s34, notes.md N-146m). The seat found it only by cross-checking the
parenthetical NAME in every `CHOICE: N (name)` reply against `options_text`.

Proposal: add that cross-check to the seat-review instrument list as a standard counted field --
**replies whose named choice anchors to no option, excluding option-less priority asks** -- and
report it as a fraction (this seat: 1/189). It is cheap, it is a one-line regex, and it caught the
single most consequential decision at this seat.
