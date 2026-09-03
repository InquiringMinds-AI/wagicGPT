# Core-prompt / render proposals from the deck152 seat (wave 54)

## R-5 (MED) - the blockers ask needs a menace COUNT header, in D17's style
Lane E gave the attackers prompt `They have N untapped creatures able to block ...` (71 of 71 of
my attack windows carry it). The blockers prompt has no equivalent for the constraint that
actually voided an answer here. At `152v146` s58 the menace rule is stated THREE times per
blocker - on each A-row and inside each B-row's per-attacker parenthesis - and the seat still
spread one blocker across two menace attackers. The information was complete; what was missing
was the arithmetic in one line.
Ask: one header line above the B-rows, on the same pattern as D17 -
`2 of the 4 attackers need TWO or more blockers each; you have 2 blockers.`
It is a count claim over rows the engine already computes, it fits the seat's own reading order
(header before rows), and it makes the impossible assignment visible before the answer is written
rather than after it is thrown away.

## R-6 (LOW) - `all_assignments_illegal` should say which assignment was illegal and why
The record carries the fallback name and nothing about the cause; the reply is recoverable only
by re-deriving the menace rule from the prompt. Ask: add the offending pairs and the reason to
the record (`all_assignments_illegal: B1:A3 (menace, 1 blocker), B2:A4 (menace, 1 blocker)`), the
way `plan_choice_conflict` was made readable in wave 53. One field, and the next reviewer does
not have to reconstruct it.

## R-7 (LOW, positive) - the life-loop clause is doing real work
`(you kill it, your attacker lives (blocking trigger: they gain 3, and their converter takes 3
off you))` and `(their life LOOP is in play: any life they gain or you lose in a loop ...)` are
what let this seat race a Sanguine Bond board with two fliers and win 20/-10 without ever
dropping a life point. Recorded so no lane trims the second half of the parenthesis: the
"and their converter takes N off you" clause is the half the decision turns on.
