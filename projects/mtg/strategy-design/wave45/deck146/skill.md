# deck146 — proposals for the strategy-writing skill (wave-45 evidence only)

## S-A. A DECK NEEDS ONE ORDERED CAST LIST, AND SCATTERED PRIORITY IS A DEFECT EVEN WHEN EVERY RULE IS RIGHT
deck146's casting advice is correct in five separate places (#1 RULE for removal, DEPLOY for cheap
bodies, PLANESWALKERS for Lolth/Kaya, VENTURE for Acererak, CLOSE THE GAME for the Hive) and there
is no order between them. At vs162 seq 22 the model had 5 mana, Lolth and Kaya both on the menu and
a Fate Unraveler on their board, and cast a Barrowin whose own option row said it returned nothing —
then never had 5 mana again. deck162, whose guide carries a single numbered cast order, runs 17/20
on its own priority rule across the same corpus; deck146 has no comparable metric because it has no
comparable rule.
PROPOSAL: extend amendment #27 from "three or more branches" to a structural requirement — **every
guide carries exactly one ordered cast list covering its main-phase cast asks, and every card-level
section is written as detail hanging off a numbered entry in it, never as an independent
imperative.** The list is the artefact reviewers grade; a card rule that does not appear in it is
either unreachable or a competing imperative. This also gives a countable per-deck compliance
metric (first-matching-line obedience) that scattered advice cannot produce.

## S-B. WHEN A SHIPPED EDIT MOVES A COUNT BY 15%, THE FIX IS AT THE OTHER LAYER
Wave 44's Hive edit (146-C) said, in the guide, keyed to a rendered field, in two places including
the quick-reference list: find the Hive on your battlefield line, if it reads (3/3) pass; never
animate in upkeep. Result: redundant animations 12 -> 10, upkeep animations 19 -> 18, out of 27 taken
from 78 offers. The teach is not wrong and it is not unread — it is competing with an option row
that reappears every priority pass and says nothing about state.
PROPOSAL: add a review rung. **When a per-decision teach is re-measured after a full wave and moves
less than ~25%, stop re-wording it and route the finding to the representation layer** — annotate
the option row, not the guide. The precedent already exists in this engine's own conventions
(`{target text:}` on removal rows, `{right now: returns NOTHING}` on Barrowin, `{X pricing:}` on X
spells): state that changes the value of an option belongs ON that option. This is amendment #26's
"a floor that must beat a numbered list has to be numbered into it", one layer down.

## S-C. AN EXCEPTION AND A STOP THAT CAN LAND ON THE SAME RENDERED LINE MUST STATE WHICH WINS
deck146 carries a stop ("a tag containing 'your attacker dies, their blocker lives' means that
creature stays home") and an exception ("send into a Shield Sphere whether or not you outnumber
them"). At vs162 seq 20 and 27 the SAME A-row carried two Shield Spheres and a Fate Unraveler, both
rules applied, nothing said which won, and the Silencer died. Amendment #6 already requires
precedence on a card-note that can collide with a procedure; this is the tighter case — two rules in
the SAME section colliding on one rendered field.
PROPOSAL: when writing an exception to a stop, state its scope in terms of the rendered string, not
of the card ("the exception applies only when EVERY name on the tag is a Shield Sphere"), and add a
Definition-of-Done check: for each exception, construct the tag where both fire and confirm the
guide answers it.

## S-D. COUNT THE OFFERS A CARD DECLINES, NOT ONLY THE RULES IT OBEYS
`Cast Silverquill Command` appeared 13 times and was never taken. Nothing in the guide is violated
by that: all three of its rules are restrictions. A restriction-only card rule is invisible to
obedience metrics — it scores 13/13 while the card never gets cast.
PROPOSAL: add to the Definition of Done a **per-card offer/take count** for every card the guide
gives a rule to. A card with rules and a 0% take rate is amendment #36's prohibition-only defect at
card scale, and it is cheap to detect from the corpus.
