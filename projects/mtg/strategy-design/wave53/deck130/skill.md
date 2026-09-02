# Strategy-writing skill proposals from the deck130 seat (wave 53)

Only what this seat's twelve-game evidence supports; the deck123 file carries S1-S5, which this
seat's evidence also supports (S2 is grounded in 130v162 s62). Two more from here.

## S6 (new) - A GUIDE'S RULE ORDER IS ITS PRIORITY ORDER, AND THE WIN CONDITION GOES FIRST
deck130's #2 RULE ran "1. planeswalker -> 2. creature it kills -> 3. creature that is hurting you
-> 4. the face, only at 6 or less", under a heading reading "DAMAGE GOES AT THEIR CREATURES AND
PLANESWALKERS, NOT THE FACE". Every step was correct, the floor was correct, and the ordering
lost a won game: at `Opponent life: 1` the pilot walked the list from the top and put a 3-damage
Hammer into a 3-loyalty planeswalker (130v162 s62/s63), then declined a lethal Pyrite Spellbomb
(s65). **Proposed rule:** in any guide whose rule is a top-down procedure, the step that WINS THE
GAME is step 0, above the procedure, and stated as an override of the rest of the rule (including
of the rule's own latches and floors). A discipline rule that can outrank "you win now" is
mis-ordered, however well each step is written. Corollary for review: for every ordered procedure
in a guide, construct the state where the last step is lethal and check that the earlier steps do
not consume the resource.

## S7 (new) - #132 HAS A THIRD VERDICT, AND A GUIDE SECTION CAN EARN A COMPRESSION
The wave-52 boundary pass set an explicit condition: if a third corpus produced no 2+-land
`would not cover` look at deck130, the section is UNTESTABLE-AT-THIS-SEAT and the check moves off
the guide. It did (6 looks, 6 keeps, 0 ships, 0 bottom asks, 0 renders of the branch in three
corpora). **Proposed rule:** "the check moves off the guide" means (a) the section is no longer
scored per corpus, (b) the RULE stays because the branch is still reachable, and (c) its
supporting anecdote may be compressed - a story about a hand that never comes is the cheapest
text in the file to lose, and guides are 36-70 KB. Making (c) explicit stops each wave's reviewer
from either re-litigating the rule or growing it.
