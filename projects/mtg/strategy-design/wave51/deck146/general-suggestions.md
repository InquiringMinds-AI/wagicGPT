# deck146 - core-prompt / render proposals (wave 51), layer-routed

Engine bugs are in `wave51/seats/seat-146-152-162.md` (E-1..E-7); nothing below is prompt text
for a bug.

G-1 (render, ability rows). **A self-life-loss ability row carries the post-action life.** Lolth's
`+0: draw card and lose life` row prints no number; the seat took it at 3 and at 2 life (`146
vs125` s269, s282). Lane E gave target rows `{right now: takes N damage - DIES/SURVIVES}`; the same
family for the CONTROLLER's life - `{right now: you go to N}` - is the observable the guide's floor
sentence keys on and the row does not print. LOW; one seat, two windows.

G-2 (render, planeswalker rows). **A `-3` row whose only legal targets are the pilot's own
permanents prints the self-hit clause, not just `[your battlefield]`.** `146 vs125` s284: `-3:
exile non-land permanent with Kaya the Inexorable targeting Kaya the Inexorable [your
battlefield]` was the only -3 row and it was taken at 1 life. Lane W's `{this hits YOUR permanent}`
tag did not render on this row family (the row printed a `{if yo...` clause I could not read to the
end at 400 chars - the seat file records the row verbatim). Route to lane W's self-target family.

G-3 (protocol, the reversed decision). `146 vs125` s282: PLAN "We must pass and hope to draw a
creature", CHOICE `1 (Lolth 0)`. Lane C's `plan_choice_conflict` keys on the literal `this window:
pass` / `stop reached`; the natural-language "we must pass" / "the answer is pass" shape is the
same conflict and is not caught. Proposal: extend the PLAN-side pattern to `\bmust pass\b|\bwe
pass\b|answer is pass` before the CHOICE index is executed. LOW-MED; D20's `decision_reversed_in_
prose` counter would have counted this one.

G-4 (efficiency). Prompt size crossed 30,000 chars three times at one seat (`146 vs125` s235
30,533; s237 30,898; s279 33,536 - turns 74-78 of an 80-turn game), all narration-dominated;
D30's log-share measurement stands, this is its first over-30K corpus at my seats. No change
proposed beyond the docketed decision.
