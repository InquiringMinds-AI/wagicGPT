# General suggestions (wave 11) — deck110 seat

No new CORE prompt-body candidate from the deck110 seat (core PASS, 8th consecutive wave from this
seat's view). All wave-11 deck110 items route to the ENGINE/REPLY-PROTOCOL layers, not the shared core
prompt. Listed here for the synthesis ledger; each has its full repro in `notes.md`.

## Engine ledger items (cross-deck relevance)

1. **Equip re-offer no-op/low-value marker should be computed from the OUTCOME DELTA, not target
   identity (E2, unfixed, ledger #4).** Two churn shapes evade the identity-keyed `(ALREADY attached...)`
   marker — equivalent carriers (wave-10 F1) and a second Plating consolidated onto an already-equipped
   creature (wave-11 deck109 T8, costly in a loss). One predicate fixes both: mark/suppress
   "Equip ... targeting X" whenever the move would NOT raise X's resulting power above its current
   carrier's. Also relevant to any deck that runs multiple equipments or multiple copies of one
   equipment. Owner: `AIPlayerGPT.cpp` equip-option serialization. Priority MEDIUM.

2. **Tag artifact CREATURES in the HAND serialization (E3 sub-gap, low priority).** Hand cards now tag
   noncreature artifacts (`Cranial Plating [artifact]`, `Galvanic Blast [instant]`) but artifact
   creatures show only P/T, so the pilot guesses their type in forward projections. Add
   `[artifact creature]` (or `[artifact]`) to hand artifact-creatures. Non-outcome-affecting for deck110
   this wave (no recurrence of the wave-10 mis-typing); a cheap correctness micro-lever for any affinity/
   artifact deck. Owner: `AIPlayerGPT.cpp` hand serialization.

## Reply-protocol layer (the A/B, not a prompt-body edit)

3. **Answer-last trailing PLAN/CHOICE mismatch — reconcile the trailing index against the plan's NAMED
   action, or require echoing the option TEXT (M1a).** Now witnessed at a 2nd seat (deck140 s6 at the
   deck110 seat: plan says HOLD the Blast, emits Cast Blast — asymmetric toward casting the offered
   spell). "Accept the last bare number" is dominated; the fix belongs to the code-appended reply-protocol
   layer. Cross-deck (deck140's own seat is the primary witness). Not guide-fixable, never a core edit.

## Harness

4. **Empty_reply fallbacks are the 120s-HTTP-timeout class, not the token cap.** 7 this seat, all choice
   -1 / reply_len 0, all in decided/won positions (none flipped a game). WAGIC_GPT_TIMEOUT 240s already
   shipped for the next corpus. No action.

## Deck construction (not a guide task)

5. **deck110's loss column is owned by draw variance (few creatures into a flood deck), not obedience.**
   If ever tuned: trim ~0-power filler and Mox/land count toward more mid bodies + reach/evasion
   redundancy to cut flood + single-threat + grind exposure at once. Standing DC1; NOT a wave task.
