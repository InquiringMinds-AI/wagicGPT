# Strategy-writing skill proposals from the deck126 seat (wave 47)

## S5 - AN ORDERED FIRST-MATCH LIST SILENTLY OVERRIDES EVERY CONDITION STATED OUTSIDE IT

deck126 carries an ordered cast list (RULE #7, entries 1-9) and, separately, RULE #4, which lists
four conditions under which Tribute to Hunger should be cast. One of those conditions - "the
opponent controls exactly ONE creature" - has no entry in the ordered list; Tribute's only entries
were 7b (heavily gated) and 8 (below every wall). Result: **0 of 5** windows where that condition
held. The condition was written, was correct, was legible, and could not fire, because the pilot
executes the numbered list and the list is total (the wave-46 boundary pass made it explicitly
total, with a roll-call naming every castable card).
This is a cost of amendment 62's own recommendation. Making a list total is right - the same edit
took unpaid `Cast nothing` answers from 24/24 to 4/47 on this seat - but a total list becomes the
guide's only executable ordering, and any conditional stated elsewhere is now dead text unless it
has a rank.
**Proposed amendment #92.** When a guide contains an ordered first-match list, every conditional
"cast X when Y" stated anywhere else in the guide must be expressible as a rank in that list. Audit
by walking each named card's conditions and asking which entry number they land at; a condition with
no landing place is a defect, not a redundancy. Adding a total-list roll-call (#76) should be paired
with this audit in the same wave.

## S6 - "TWO OR MORE OF YOUR CREATURES" IS A SHAPE NO POOL GUIDE HAS A RULE FOR

Both wave-47 five-attacker windows on this seat were answered by putting every blocker on one 3/2
while two menace attackers went unblocked. The render is complete and explicit on both the A-line
and the B-line ("only a block by TWO OR MORE of your creatures counts; one creature alone does not
block it at all"), and the guide's blocking heuristic - "give each attacker one blocker first" - is
actively wrong there: one blocker on a menace attacker is a wasted body AND stops nothing. The
pool's red-pin item (Baka menace gang-block probe) is on the engine side; this is the guide side of
the same mechanic and no guide in the pool addresses it.
**Proposed amendment #93.** Where the render marks an attacker as requiring N blockers, a blocking
guide must state the all-or-nothing rule explicitly, and any "one blocker per attacker first"
heuristic must be scoped to exclude those attackers. General form: a distribution heuristic written
for the default case must name the marked cases it does not cover, or it will be applied to them.

## S7 - RENDERING A FACT DOES NOT STOP THE MODEL RESTATING IT

Lane M shipped own-side `{effect:}` text so the pilot could read its own combo off the battlefield
line instead of re-deriving it in PLAN. The render works (0 unexplained bare own permanents in 612
decisions across both my seats). The behaviour did not move: replies restating the Bond/Blood loop
mechanics went **31/211 (15%) in wave 46 to 34/199 (17%) in wave 47**, and p90 reply length was flat
(602 -> 615 chars). Mean reply length fell (506 -> 405), which is the only movement, and one
decision on this seat still burned **800.8 s**.
**Proposed amendment #94.** A render change justified by "the pilot will stop re-deriving X" should
carry a measured re-derivation rate as its falsifier, and should not be counted as delivered on the
strength of the string appearing. Restatement in a plan is not the same behaviour as reading, and
the corpus can separate them cheaply (a regex over replies). Where the goal is actually to shorten
the reply, the lever is likely in the ASK's instructions, not in the state block.
