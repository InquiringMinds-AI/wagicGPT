# Strategy-writing skill proposals from the deck125 seat (wave 47)

Only what this seat's corpus supports. Each is stated as an amendment candidate with the seq that
pays for it. Numbering continues from amendment 87.

## S1 - A RULE THAT ASKS FOR A NUMBER DIES ON THE SURFACE, NOT ON THE ARITHMETIC

The deck125 tap-out gate has now failed four consecutive waves (6/11, 3-4/7, 4/7, 6/7) across three
different formulations: a four-way branch on which counter is held, then a single constant (9), then
the constant re-stated in DECIDING SITUATIONS. Every version asked the pilot to compare a number it
would have AFTER the cast against a line printed BEFORE it. Wave 46's collapse-to-a-constant was the
right *kind* of fix by amendment 62 and it still failed, which isolates the variable: the problem was
never the size of the computation, it was that the answer lived on a different surface from the
decision.
The same wave, three rules keyed to a field printed **on the option row itself** went to ceiling
without any guide change: X sizing (12/12 once `{X pricing:}` appeared on the row), the land drop
(93/93), the mana-artifact counter class (0/28).
**Proposed amendment #88.** When a rule needs a quantity, prefer, in this order: (a) a number
printed on the OPTION ROW the pilot is about to select; (b) a number printed on a header line in the
same prompt; (c) a constant; (d) an arithmetic instruction. Moving a failing rule from (c)/(d) to
(a) is a distinct and cheaper repair than restating or hoisting it, and it should be tried before
either. Corollary: when the engine ships a new per-row or per-header field, sweep every rule in the
guide that was deriving that quantity - the derivation is now a liability, not redundancy.

## S2 - A RULE STATED ONLY AS ITS EXCEPTION FIRES ONLY AS ITS EXCEPTION

Path to Exile: 1 of 13 windows at life <= 12 with a legal high-power target, twelve misses in one
lost game. The Path section states the power rule and then spends four times as many words on the
floor that suppresses it, and DECIDING SITUATIONS carried **only** the suppressing line ("the only
Path targets offered are defenders or 1-power creatures and your life is above 12: cast nothing").
Lightmine Field is the mirror image with the same outcome: DECIDING SITUATIONS carried only the
POSITIVE line and no stop, and it was cast 3/3 into boards it could not touch.
**Proposed amendment #89.** Any rule that appears in the nearest-the-decision section must appear
there with BOTH polarities - the condition under which you take the action and the condition under
which you do not. A single-polarity entry is not a compressed rule, it is a different rule. When
auditing, grep the deciding section for each named card and check that both branches are present.

## S3 - TWO ABSOLUTE RULES IN ONE SECTION NEED THEIR PRECEDENCE WRITTEN

Emrakul was hoisted into DECIDING SITUATIONS in wave 46 ("TAKE IT", stated absolutely) and declined
3 of 4 times in wave 47. Sixteen lines below it, in the same section, the tap-out gate says
absolutely not to cast anything leaving fewer than 5 sources - and a fifteen-mana spell can never
leave 5. The rules were not ambiguous; they were contradictory, and hoisting the loser again would
not have helped.
**Proposed amendment #90.** Before adding or hoisting an absolute rule ("always", "every time",
"whatever the numbers say"), scan the same guide for other absolutes whose conditions can co-occur
with it, and write the precedence at the point of collision. A hoist is the wrong repair for a rule
that is losing a conflict rather than being unread; the symptom that distinguishes them is a reply
that shows no reasoning at all, which is what all three Emrakul declines showed.

## S4 - WHEN THE ENGINE SHIPS A FIELD, RE-READ THE GUIDE FOR SENTENCES IT HAS FALSIFIED

The deck125 X section asserted in bold that the ask "does NOT print a count of your own hand". That
was true when written and is now false on 413 of 413 prompts. The trust doctrine forbids teaching
doubt of the render; a sentence asserting the render's *absence* becomes exactly that the moment the
field lands, and it is more corrosive than a merely stale rule because it licenses the pilot to
distrust what it can see.
**Proposed amendment #91.** Guides may state what the render DOES print. They may not state what it
does not print. Where an absence needs to be handled, phrase the rule around what IS printed
instead. Existing negative-existence claims are a standing audit item for every wave in which the
render changes.
