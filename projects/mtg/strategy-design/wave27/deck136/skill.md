# deck136 -> strategy-writing-skill proposals (wave-27, Step-0 debut)
# Layer: skill.md = proposals to the guide-WRITING skill (Step 0 / Step 0-bis method).
# Tag every item single-seat with a promotion threshold, per skill doctrine.

## PROPOSAL 1 (Step 0-bis rule 2 refinement, tagged single-seat) -- CLASSIFY the top
## reasoning-tax spike as ENGINE-caused vs DECK-FACT before it ranks your teaching points.
Rule 2 says "let the reasoning-tax spikes rank your teaching points -- they are the
saliency signal," and rank by (a) wrong/near-wrong action AND (b) tax magnitude. deck136
adds a NECESSARY pre-filter: a spike can be the LARGEST in the corpus and still be
un-teachable because it is an ENGINE bug the model is correctly refusing, not a deck fact
the model is re-deriving. Witness: the three largest replies in the whole deck136 corpus
(15.9k / 15.5k / 15.4k) are ALL the same Legion's End phantom target-chooser (an engine
card-script defect surfacing an illegal ask AFTER the spell resolved; R-RESOLVED-SPELL-
PHANTOM-CHOOSER). The refusal is CORRECT reasoning. Adding a guide line to "pre-answer"
it would (a) not fix the engine, and (b) actively endanger the WORKING legitimate seam
(the Legion's End cast-target choice, which the model nails every time). Proposed
one-line addition to rule 2: "Before a tax spike ranks as a teaching point, CLASSIFY it:
if the tax is the model correctly refusing/agonizing over an ILLEGAL or CONTRADICTORY
engine ask (an ask whose options cannot satisfy its own header, or an ask for an already-
resolved spell), it routes to notes.md as an engine item and the guide teaches NOTHING --
a crutch there fixes no engine bug and risks poisoning the card's LEGITIMATE seam. Only
tax where the model is re-deriving a knowable DECK FACT earns guide prose." Promotion:
already congruent with the standing "engine/card-script items go to notes.md, never guide
prose" doctrine and the crutch-lifecycle; this sharpens rule 2's ranking step with the
engine-vs-fact test. Promote to a rule-2 sub-step if a second Step-0 seat's top tax spike
is likewise an engine phantom-ask.

## PROPOSAL 2 (Step 0-bis rule 5, midrange nuance -- ADOPT-as-refinement) -- the attack
## floor for a MIDRANGE/attrition deck must be BOARD-STATE-KEYED, not "attack every turn."
Rule 5 mandates writing the attack floor on the first pass for "any evasion or aggro
deck." deck136 is a MIDRANGE deck that is the beatdown vs slow decks and the control vs
aggro -- a blanket "attack every turn" floor would be actively wrong (it must hold
blockers vs Kithkin/Rats/Giants/burn). Yet passivity IS present (vs22 "no attackers"
10.8k tax; vs93 racing with one creature). The refinement: for a midrange/dual-role deck,
the first-pass floor is still a per-item test (rule 5) but the GO condition is profit
("unblocked/flies, OR trades up/kills a happy target") and the closed exception is a
role-switch keyed to the opponent archetype ("hold enough blockers that their swing-back
is not lethal vs an aggro deck"), NOT the empty "only when every attacker dies for zero
gain." Name the deck's best attackers (evasion/mana-sink) as the lead. Tag single-seat
(first midrange dual-role Step-0 seat); promote to a named rule-5 face if a second
midrange deck reproduces the passivity-without-a-board-state-keyed-floor.

## PROPOSAL 3 (reaffirm, no new rung) -- MONO-COLOR + generic-cost decks are the CONTROL
## case for the mana-simplification rung. deck136 (mono-black, Ugin generic) showed ZERO
color-anxiety / mana-veto tax -- the wave-25/26 cross-phase mana-fact failure class needs
a color/hybrid confound to appear. The guide's trust-the-numbers line is cheap insurance,
not load-bearing here. This is a negative confirmation, not a change: keep the rung scoped
to DERIVED-mana decks (hybrid/devotion/multi-cast) and do not inflate a mono-color guide
with mana prose it does not need. (No skill edit; recorded to keep the rung's scope honest.)

## Boundary re-checks (nothing to merge into core from this seat)
- No core-prompt defect surfaced. Parser held (the 4 unparsed are the engine phantom-ask;
  parser correctly refused an unparseable refusal). PASS.
- The unexercised Rankle may-ask / Liliana loyalty / Ugin classes are carry-forward, not
  method changes.
