# deck126 - proposals for the strategy-writing skill (wave 48)

S1 - **A rendered count is not a rendered PREDICATE - name the predicate the count satisfies.** The
header said `of which 1 is a creature`; the rule said "when that reads exactly 1, cast Tribute";
twice the model wrote "if it becomes their only creature" and deferred. The executor matched the
number but did not resolve "1" into "only", i.e. into the condition the rule is really about. When
a rule keys on a rendered number, write the sentence the number PROVES ("1 means the printed
creature IS their only creature, now") next to the number, and name the tense ("now, in this
window, not if it becomes") - the executor's deferral is a tense error, not a reading error (F5).

S2 - **A hold rule that blocks the only productive row on the menu is wrong, whatever the count
says.** D33's "0 Lantern casts at {B} 2+" read as FAIL 3/3; every one of the three windows had no
higher-ranked castable row, and two of the games were won. Before scoring a hold rule's fraction,
read the menus of the violations: if the rule's alternative was "Cast nothing", the rule is the
defect, and the fraction is the executor being right (F6).

S3 - **Adjudicate E1-era replies by the LAST coded line, and read the reasoning between the first
and the last.** With the parser taking the final `CHOICE:` as a re-answer, a record's `choice` can
be a different option from the reply's first line, and the reasoning in between is now the
mechanism trace (9/174 replies on this seat carry two or more coded lines). Two of those changed
the play for the worse (`vs146` seq 43: ping -> pass; `vs130` seq 25: "cast nothing" -> Tribute at
0, via a hallucinated card name). Scoring by the first line would have missed both; scoring by
`choice` alone would have blamed the guide for the second.

S4 - **A rule's exception written as "when their side is empty" is a rule that never fires against a
real board.** The win-button attack rule needed "blocked or not" - lifelink pays on damage to a
blocker. Write the mechanism ("damage to a blocker is still damage") rather than the safe-looking
board state, or the executor keeps the win in hand (F7).
