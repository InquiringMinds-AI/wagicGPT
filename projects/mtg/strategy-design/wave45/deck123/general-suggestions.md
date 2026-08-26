# deck123 - core-prompt / render proposals (wave 45)

Layer-routed. Engine BUGS are in the seat file's ENGINE list (M1-M3, L4-L5); nothing here asks
for a code fix disguised as prompt text, and nothing here asks the prompt to carry a fact the
guide should carry.

## G1 (prompt) - the land-drop question could name the cost of skipping it, once.
The "Land drop:" ask is already the best-explained question in the prompt ("The land drop is its
OWN decision ... its absence from the choices below does not mean it is gone"). What it never
says is what "Play no land right now" costs, and it is the only option in the game that is
strictly dominated whenever another option exists. One clause on the option row itself would
have ended a thirteen-window run:
  `4. Play no land right now  [you have 3 lands in hand and 2 on the battlefield; skipping this
     leaves your mana total unchanged until your next turn]`
Both numbers are already computed elsewhere in the same prompt. This is a nudge, not a rule -
declining is legal and occasionally right (a Forgotten Cave held for a cycling turn, deck130) -
so the clause must state the cost, never forbid the choice.
Repro of the run: `1787768332-ai_baka_deck123-0x564b59dd4a40-vs-ai_baka_deck126.jsonl`
seq 12-24.

## G2 (prompt) - the plan carried into a window is stale far more often than the plan-note fires.
The plan-note gate is now clean and cheap (48 fires in 575 decisions at these seats, 0 false by
the card-name test, 0 on land drops - #W45-4 PASS). But the failure it does NOT catch is a plan
that names an action the menu DOES contain and the seat cannot afford: "Cast Intruder Alarm
{2}{u} using Tundra and Underground Sea" survived thirteen windows at "Mana available: 2 total",
and deck130 carried "next turn, untap to 8 mana" for four turns on four lands. Both are
arithmetic contradictions between the plan text and a number printed two lines above it.
Proposal, same channel and same conservatism as the existing note:
  `(note: your plan names Intruder Alarm {2}{u}, which costs 3 mana; this window shows 2.)`
Fire only on an exact card-name match against the plan text with a computed cost strictly above
the printed total - i.e. a strictly-decidable arithmetic mismatch, never on a judgment.
Repro: file above, seq 13; deck130 vs123 seq 27 ("untap to 8 mana", four Mountains).

## G3 (prompt, LOW) - the bottoming ask could print what the keep is for.
The London bottoming ask is correct and complete (CR 103.5 cited, the whole hand listed with
card text, "keep your best 4 and bottom your worst 3"). What it does not carry is the one number
the decision turns on: how many mana the kept cards will have available and what they cost. The
opening-hand ask three screens earlier DOES carry it - "Playing every land in this hand would
cover the cost of: ...". Re-emitting that line on the bottoming ask, recomputed for the cards
NOT yet bottomed, would price the choice in the vocabulary the seat has already read once.
Repro: `...vs-ai_baka_deck126.jsonl` seq 5 (bottomed Idyllic Tutor, Vision Skeins and Damnation
from a four-card keep, retaining an Intruder Alarm that does nothing without a creature).
