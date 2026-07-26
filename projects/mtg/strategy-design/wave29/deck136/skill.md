# deck136 -> strategy-writing-skill proposals (wave-29)
# Layer: skill.md = proposals to the guide-WRITING skill. Tag single-seat + threshold.

## PROPOSAL 1 (CLOSES the verify-oracle incident arc) -- A FABRICATED-FACT FIX IS
## VALIDATED BY THE DECISION, NOT THE RECORD; and the validation can SUCCEED at the
## decision layer while the matchup it targets STILL LOSES to a different layer.
The wave-28 fix corrected a fabricated mana value (Relentless Rats MV4 -> MV3) that had
inverted both removal facts and made the pilot chase an impossible "Legion's End the Rats"
plan for five turns. Wave-29 is the validation corpus. Result, decomposed at the decision
layer (the ONLY honest place to read a card-fact fix):
  - The impossible-plan class is GONE: the pilot rejects Legion's End vs MV3 Rats at every
    decision, verbatim, and never relapses (findings Job 1a).
  - Ritual is now valued as THE answer, verbatim (Job 1b).
  - AND YET the targeted matchup (vs93) was LOST for the third straight wave - because a
    SEPARATE engine bug (a reveal seam offering illegal tutor options) silently ate the
    Ritual the pilot correctly went to fetch (Job 1c, N-136a).
The lesson, reinforcing the wave-28 P1/P2 rungs: a card-fact fix's success is read off the
PLAN prose and per-decision valuation, and it can be FULLY validated there while the W-L is
unmoved or still a loss. Do not read a fabricated-fact fix as "failed" because the record
didn't flip, and do not read it as "worked" because the record improved (deck136's record
went 2->5 this wave for reasons entirely unrelated to the fix - Job 2). The record and the
fix are on different layers. This CLOSES the verify-oracle-text fabrication incident that
opened wave-27 (fabricated MV) -> wave-28 (fix shipped) -> wave-29 (validated at decision
layer): the incident arc is complete; retire it from the active skill watch and keep it as
the canonical worked example for "how a fabricated card FACT is caught and validated."
Promotion: this is the SECOND wave running that the decision-audit-over-record method
proved load-bearing for this exact defect - promote the "decision-layer validation, not
record validation" clause to the named rung established in wave-28 P1.

## PROPOSAL 2 (NOVEL, threshold) -- A GUIDE CANNOT FIX AN ENGINE SEAM THAT OFFERS ILLEGAL
## OPTIONS; but it CAN steer the pilot away from the option class that trips it.
N-136a (reveal seam offers the whole library as tutor PUT options, ignoring the card's
constraint) is an engine defect, not a guide defect - the guide layer cannot make the
engine stop offering an illegal choice. But the guide CAN reduce the blast radius by
teaching the pilot the card's real constraint so it does not WANT the illegal option:
deck136's guide now states Mausoleum Secrets' hard dead-creature gate concretely and names
the bombs it cannot reach early, and tells the Rats matchup not to over-tutor for Ritual.
That does not fix the engine, but it means the pilot reaches for a LEGAL find (or declines)
instead of a phantom bomb. Method rule for the skill: when a reviewer routes an engine
illegal-option bug to notes.md, ALSO check whether the guide can lower the pilot's DEMAND
for the illegal option (state the constraint precisely, name what the effect cannot reach).
This is a defense-in-depth pattern - the engine fix removes the defect, the guide clause
removes the pilot's appetite for it - and the two are complementary, not redundant. Tag
single-seat for now; promote if a second deck has a constrained tutor/search whose guide
overstates its reach.

## PROPOSAL 3 (reaffirm, single-seat) -- STATE A CONSTRAINED-TUTOR'S GATE IN THE UNITS THE
## PILOT CAN COUNT AT THE TABLE, not as a time axis.
The wave-28 guide's Mausoleum Secrets line said "Early it finds cheap cards; late it finds
your bombs" - a TIME axis. The engine gate is a COUNT axis (creature cards in graveyard).
The pilot read "late = bombs" and cast Secrets on turn 5 expecting Ritual, with an empty
graveyard, finding nothing (findings Job 1c). Refinement: when a card's ability is gated on
a countable board quantity (cards in a zone, creatures dead, lands controlled), the guide
must state the gate in THAT quantity ("needs 4 of your creatures dead") and give the pilot
the arithmetic for its key targets ("MV4 Ritual needs 4"), never a vague temporal proxy
("late"). "Late" is not a legality the pilot can verify against the board; "4 creature
cards in your graveyard" is. Tag single-seat; congruent with the deciding-fact-rides-the-
option doctrine (put the checkable magnitude in front of the pilot).

## Boundary re-checks (nothing to merge into core from this seat)
- No core-prompt defect. The 2 fallbacks are a benign stale-drop and a modal->priority
  context bleed (routed to notes.md, not a prompt-architecture change). PASS #24.
- N-136a and the modal-bleed watch are ENGINE/harness items - recorded in notes.md, not
  method changes.
