# Strategy-writing-skill proposals from deck125 (wave 44)

Only proposals my own corpus evidence supports. Each names the instance that produced it.

## S1 — A capped rule must carry its cap inside the sentence with the action verb

The X section held two unconditional instructions about the same click, three lines apart:

> When the X menu appears, pick the line showing the LARGEST X. ... the line you pick must
> literally read "X = " followed by the biggest number in the menu.

and then

> Announce the largest X that keeps (cards in hand + X) at 7 or less.

Result over 14 X windows: **14/14 took the largest offered X**; the hand cap held in 5/14;
the library cap held in 13/14 and failed at the one window where it bound (library 13,
announced X = 12, 12 cards then discarded).

Proposal for the skill: a rule of the form "take the biggest / the first / all of them"
must not be followed by a separate sentence that narrows it. Either the narrowing lives in
the same sentence as the verb, or the menu-reading instruction is rewritten to point at a
number the model computed BEFORE looking at the menu ("pick the line that equals the number
you worked out"), never at a property of the menu ("the biggest number listed"). A menu
option's own headline ("option 1 is the LARGEST X") already pushes toward the extreme; a
guide that also says "pick the largest" is pushing the same way twice.

## S2 — Every hard prohibition in the body must appear in the quick-reference list

deck125's guide ends with a DECIDING SITUATIONS list. Its sweeper section had five bullets
keyed off one header number; four of them made the list, and the fifth — `0 creatures:
answer "Cast nothing right now"` — did not. That fifth is the one violated 3 times in 14
sweeper casts, and one of those three decided the seat's only loss.

Proposal: the skill should treat the closing situation list as a completeness contract, not
a summary. Any body rule phrased as a prohibition ("never", "cast nothing", "keep it") gets
a line in the list, and the list is checked against the body's prohibitions before the guide
ships.

## S3 — Threshold rules must be sized against the window SAMPLING RATE, not the danger point

The Elixir rule fired at `library <= 12`. The Elixir is only offered in priority windows,
which arrive about every other turn, and with two Staffs of Nin the library moves three
cards a turn. In one game the option appeared 17 times, the last offer at library 13, and
the rule never fired at all while the library bottomed at 12.

Proposal: for any "act when a rendered counter crosses T" teach, the writer must set
T >= danger_point + (counter's rate of change x the number of turns between windows that
offer the action). The guide should state the arithmetic, so a future reviewer can re-derive
T when the deck's draw rate changes rather than guessing.

## S4 — Prefer arithmetic on printed numbers over symbolic mana comparisons

`cast Staff of Nin only when it leaves {1}{U} untapped` was violated in 5 of 11 Staff casts.
The render prints `Mana available: N total`; it does not print "what remains after this
cast expressed in coloured pips". Replacing the symbol with a small table of concrete
thresholds ("8 or more with an Essence Scatter in hand, 9 with a Cancel or Dream Fracture,
11 with only a Fall of the Gavel, 6 if you hold no counter") turns a two-step symbolic
comparison into a one-step numeric one.

Proposal: the skill's "read it off the render" doctrine should extend explicitly to mana —
if a rule needs a mana comparison, express it as a single integer to compare against the
"Mana available:" line.

## S5 — A teach that fixes one surface must be checked against its twin surface

Wave 43's W6 taught: *a creature whose rules text does something every turn is worth the
removal however small its printed power*. It PASSED on the surface it was written for (the
Path section; 6/7 engine-shaped targets Pathed within two windows). The identical shape on
the stack, one window earlier, was still governed by `printed power 0 or 1: LET IT RESOLVE`,
and three spells (Dwarven Blastminer twice, Luminarch Aspirant once) resolved under it — each
then costing a Path, the card the same guide calls its only answer to a resolved creature.

Proposal: when the skill records a teach keyed to a card property (text, tag, P/T), it should
prompt the writer to enumerate every DECISION SURFACE where that property is rendered — stack
(counter), battlefield (removal), combat (block/attack), target menus — and either apply the
teach to all of them or state why not. The property was rendered on all three surfaces here;
only one used it.
