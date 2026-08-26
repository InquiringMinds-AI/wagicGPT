# Strategy-writing skill proposals from seat deck123 (wave 45)

Only what this seat's evidence supports. Each is offered as an amendment candidate; the merge
rule (convergence across two seats, or one decisive falsification) is the synthesis step's to
apply, and where my evidence is one seat I say so.

## S1 - A GUIDE MUST GOVERN THE FREE ACTION, NOT ONLY THE PRICED ONES.
Every rule in deck123's 278-line guide governs a decision that costs mana or a card. The one
decision that costs NOTHING - the land drop - had no rule, and the seat answered "Play no land
right now" thirteen consecutive times across seven of its own turns and lost 42-0 without ever
casting its three-mana engine. The failure is not that the rule was weak; there was no rule, and
nothing in the guide told the executor that {2}{u} is three mana rather than two.
PROPOSED AMENDMENT: before shipping a guide, enumerate the decision KINDS the harness can raise
for that deck (land drop, mulligan, bottoming, cast, target, mode, announce X, activation,
attackers, blockers, forced sacrifice, reveal) and require a named owner for each - a section, or
one line in DECIDING SITUATIONS. A kind with no owner is the one that produces a thirteen-window
run, because nothing in the document ever contradicts the plan the executor is repeating. This
extends #37 ("a multi-ask sequence is one governed window") from sequences to the whole ask
space. Cheap check: the corpus already labels every record with its `kind`.
DECISIVE FALSIFICATION: cited above (one seat, one game, one loss).

## S2 - STATE COSTS AS INTEGERS AGAINST THE PRINTED MANA LINE - #33 EXTENDS TO CARD COSTS.
Amendment #33 made this ruling for the mana a rule RESERVES ("leave {1}{U} untapped" -> a table
of thresholds against "Mana available: N total"). The same failure occurred one layer down, on
the cost of the card being cast: the executor read {2}{u} as 2 and planned around it for seven
turns while the render printed "Mana available: 2 total" every window. A guide that names a
spell by its symbol string is asking the executor to do the conversion.
PROPOSED AMENDMENT: wherever a guide names a card cost, print the integer beside the symbols -
`Intruder Alarm {2}{u} (three mana)`. Applies to cast lists, curve tables and card lines alike.

## S3 - A RULE'S RATIONALE MUST BE WRITTEN DOWN SOMEWHERE THE NEXT REVIEWER CAN FIND IT.
deck123's 12-creature token cap was violated ten times this corpus after holding 42/42 and then
64/41 in earlier waves, and every violation was the better play by odds. I could not adjudicate
whether to raise, lower or cut it, because no wave-40..44 artefact records what the 12 is FOR
(it is not prompt size - the largest prompt at this seat is 19,155 chars and the ranged collapse
keeps wide boards flat). A cap whose purpose is unrecorded cannot be moved by evidence, only
re-obeyed or routed to the owner - which is a standing tax on every later review.
PROPOSED AMENDMENT: every numeric threshold shipped in a guide carries a one-line WHY in that
wave's edit-texts.md, in the form "T = <number> because <observable that goes wrong above it>",
so a later wave can re-derive it (this is what #34 already requires for rate-crossing
thresholds - generalise it to all thresholds).

## S4 - A COMBAT SELECTION RULE NAMES THE BODY CLASS, NOT JUST THE PRIORITY.
#25 put a survival gate above the ladder and #39 split token from card in a STOP. This seat
produced the third face of the same thing in a SELECTION with no survival pressure at all: at 12
life against 3 damage, fourteen blockers offered, thirteen of them free tokens, it spent
Bloodline Keeper (with the Greaves attached) and killed its own engine. The guide's "block with
your TOKENS first" bullet was present and third in a five-bullet prose list.
PROPOSED AMENDMENT: any block-side section states, as an ordered numbered STEP before any trade
evaluation, WHICH BODY is spent - with the render-readable discriminator (a token row prints no
mana cost; a card row prints one). Priority statements buried mid-list lose to the trade
language around them.

## S5 - WHEN THE EXECUTOR ASSERTS A PARENTHESIS BACKWARDS, THE FIX IS AT THE ROW, NOT THE RULE.
#30's companion check says a stop's rationale must survive the keyword that removes it. The
shape here is one step blunter: fourteen identical rows read "(your blocker dies, attacker
lives)" and the reply said "block it with Bloodline Keeper to kill it". No rule was violated
because no rule mentioned that string. Amendment #32 already asks for a cross-guide index of
render-string-keyed rules; this seat adds "(your blocker dies, attacker lives)" to that index as
a string every guide with blockers needs a verdict for - it is the commonest block-side
parenthesis in the corpus and the only one whose plain meaning the pilot has now inverted.
