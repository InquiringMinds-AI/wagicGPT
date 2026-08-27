# deck123 - proposals for the strategy-writing skill (wave 49)

Each proposal names its evidence; synthesis owns adoption.

**S1 - A dictated number-carrying PLAN shape for any repeated-action rule.** deck123's loop stop
(M reaches L + C + 3) held for exactly the windows in which the pilot's PLAN still contained the
number: at `vs125` seq 12 the plan read "Create 20 more Vampires to reach 23 total" and N was
right; from seq 16 the plan read "Attack with all creatures to win the game" and the next five takes
named 2, 25, 50, 120 and 100 with M past the stop. #115 says the governing number goes in the plan
sentence; this is the repeat-row instance, and the guide now dictates the shape `PLAN: stop at M =
<L + C + 3>; M is <M> now; this window: <x<N> / pass>` (#105, blanks the screen fills). Proposed
rule: wherever a render offers a one-decision repeated action, the guide's plan shape carries the
stop, the current count and the next window's answer - three blanks - and the review metric is
"replies on repeat-row windows whose PLAN line carries all three numbers".

**S2 - When a lane ships a count on the row, re-key the rule to the row's string in the same wave,
and retire the battlefield-line derivation.** Lane T's D10 put `{right now: they control N
creatures - ...}` on every edict row and `{right now: destroys N ... (K able to attack), M of
yours}` on every sweeper row. RULE 5 still told the pilot to find N on the opponent's battlefield
line; RULE 3 compared two "of which" counts and had no notion of K. Both rules are re-keyed (A49-4,
A49-5) and the K count produced a rule the old key could not express (a sweeper on two walls is
kept). This is #49/#110 applied at the moment a lane lands: the review carry lists the new strings;
the reviewer's checklist item is "which guide rules derive a quantity the carry's new string now
prints - re-key them".

**S3 - A rule that quotes a render string is checked against the emitter every wave (#103), and
the cheap check is a grep of the guide's quoted literals against the corpus prompts.** RULE 2 quoted
"Put in Play with Marsh Flats targeting"; the emitter prints "Put a card onto the battlefield with
Marsh Flats targeting" (every fetch window this corpus). The rule still worked because the pilot
matched on "Marsh Flats", but a literal that no prompt contains is a false sentence waiting to
matter. Proposed mechanical step for the reviewer: extract every quoted string of 4+ words from the
guide and grep the seat's prompts; report the misses.

**S4 - The engine's automatic choices at a seat are a review surface.** `vs152` seq 4 -> 5: the
cleanup-step discard was made by the engine (`You discarded Intruder Alarm`), not by the pilot, and
it discarded the deck's engine piece. No guide rule can fix a decision the pilot is not asked; the
guide can only avoid creating the decision (A49-8 teaches a hand-size check). Proposed: the seat
file records every narration line of the form `You <verb>ed <card>` that has no matching decision
record in the JSONL, as a list for the engine seat - the same shape as #117's offer-by-phase count,
for actions taken without an offer.

**S5 - "Violated, better" at a game-deciding absolute is evidence to soften with the render's
count, not to add an exception clause (#109).** Entry 0 said Damnation at 0-vs-2+ is above
everything; at `vs126` seq 11 (0 vs a Wall of Omens + a 1/1) the pilot took Slip + Tribute instead
and was right. The softening used the row's own `(K able to attack)` rather than a prose exception
("unless they are walls"), because the row prints the quantity the exception is about.
