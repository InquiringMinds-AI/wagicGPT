# Strategy-writing skill proposals from the deck125 seat (wave 53)

Only what this seat's evidence supports. Each proposal names the corpus fact that produced it.

## S1 (new amendment) - A THRESHOLD THE RENDER DOES NOT PRINT IS A GUIDE BUG, NOT A PILOT BUG

deck125's loss is a rule that was *followed* and was *wrong*. The guide said: under Forced Fruition,
"cast a counter ONLY when that price is below your life minus 1". At 24 life the price was 14, so
the rule permitted it; the seat paid 58% of its life to deny 1 damage per future card and died three
turns later without casting again (`vs162` seq 78).

The defect is a family, not an instance: **a threshold expressed as an arithmetic relation between
two moving numbers (`price < life - 1`) is satisfied over a huge, mostly-wrong region.** The pilot
cannot see the region; it can only evaluate the relation at one point. A threshold expressed as a
CONSTANT read against a number the row already prints (`- you would be at K`; take it only when
K >= 15) is checkable, is falsifiable in the next corpus, and cannot silently widen.

Proposed rule for the skill: **when a render prints a finished number, the guide's threshold must be
a constant compared against that number, never a relation between two totals the pilot has to
combine.** Corollary for reviewers: when a rule is followed and the game is still lost, suspect the
threshold's SHAPE before suspecting the pilot's reading.

## S2 (extends #168) - RE-KEY TO THE MOST DEDICATED LITERAL THE PROMPT OFFERS, NOT THE MOST DETAILED

#168 tells us to re-key a guide sentence when its literal changes. This corpus adds a selection
rule. Three of this seat's rules pointed at *substrings inside a composite line* - "read the OPPONENT
battlefield line for an [enchantment] whose {effect:} reads 'Whenever an opponent loses life...'" -
while the prompt separately printed a **dedicated single-purpose line** carrying the same fact:
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` (353 renders in the game where
the rule broke twice, `vs126` seq 294/332).

The composite-line key failed after four correct applications on the identical board. The dedicated
line is one string, appears at a fixed place, and cannot be crowded out by a 25-permanent battlefield
listing. Proposed rule: **when the prompt carries both a composite surface and a dedicated one for
the same fact, key the guide to the dedicated one and say so explicitly ("you do not read the
battlefield for it").**

## S3 (extends #143 / #177) - AN EXCEPTION ADDED IN ONE SECTION MUST BE WALKED INTO EVERY BULLET THAT
RESTATES THE RULE, IN THE SAME EDIT

The Path carve-out (W79a) contradicted two DECIDING-SITUATIONS bullets the moment it was written;
the Fruition threshold (W78a) contradicted a bullet three sections earlier and a summary sentence in
its own section. Wave 52's #177 asks for a prose-vs-bullet diff **before scoring**. This corpus
suggests moving it earlier: run the diff **as part of each edit**, not as a pass at the end, because
the reviewer who writes the exception is the only person who knows every place it lands. Four of my
27 deck125 edits (W86c-W86f) exist only because I did the diff late.

## S4 - "NOTHING ELSE ON THE MENU WAS CASTABLE" IS A RECURRING PILOT ARGUMENT AND GUIDES SHOULD REFUSE
IT BY NAME

Both of this seat's low-X Revelation casts (`vs126` seq 59, `vs123` seq 17) and both of deck126's
zero-Tribute casts (`vs125` seq 73/74) happened on menus where **every other cast row was one the
guide forbids**. The pilot appears to treat a menu with one legal-looking row as an instruction to
take it. Guides for this tier should carry, once, an explicit sentence that a menu on which nothing
is worth casting is answered with the decline row, and cite the seat's own biggest decline ceiling
as proof it is a normal answer (deck125: 155 sweeper windows at 0). I wrote this into W81.

## S5 - EVIDENCE COUNTS IN A GUIDE ARE A MAINTENANCE COST; PREFER ONE CURRENT NUMBER TO THREE HISTORIC
ONES

deck125's guide carried "sixteen windows one corpus... two casts the corpus after that... this corpus
eighteen such rows" for a single rule. Three of my six trims (W85a, W85d, W85e) collapsed such chains
to one current figure with no loss of force, and recovered ~1.1 KB. Proposed rule: **a rule carries at
most one corpus's counts plus, where it earns its place, one named catastrophe.** The catastrophe is
what changes behaviour; the count history is bookkeeping for the reviewer and belongs in the seat file.
