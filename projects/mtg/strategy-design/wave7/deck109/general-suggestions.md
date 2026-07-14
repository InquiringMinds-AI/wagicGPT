# General / core-prompt suggestions (from deck109 wave-7)

Layer-routed per the skill. Single-seat evidence cannot cut or add a core line -- these are OBSERVATIONS
for the synthesis agent to weigh against cross-deck convergence.

## 1. [CORE STOPGAP -- removal condition stays MET from this seat] `hand -> stack` cast-happened anchor.
Same status as wave-6: deck109 is the canonical repro for the "engine no-op" class, the defect is fixed
at source (353f1a150), and this corpus shows **0 cast no-ops and 0 cast-repeat loops** across 6 games,
even after the deck's own fallback rule was DELETED. The anchor's removal condition remains met from this
seat. Still route to the CROSS-DECK sweep before retiring it from core; do not cut on this seat alone.
The deletion of the deck-side fallback caused no regression, which strengthens the case that the core
anchor is also now inert for this class -- add to the removal shortlist for a deliberate A/B.

## 2. [PRESENT-AND-LOSING once -- no core action, watch only] Decline-a-listed-body / phantom affordability.
deck44 seq19: at 4 life with Boros Reckoner (3/3) and Ash Zealot (2/2 haste) BOTH listed, the pilot chose
"Cast nothing," reasoning it "cannot cast ... Boros Reckoner this turn" -- FALSE, it was in the list. The
core ALREADY forbids this (the "never decline a listed action because you believe you cannot pay" clause
and the deploy-every-turn / plan-from-list rules). It fired ONCE, was not outcome-changing, and the
trigger was a phantom-LETHAL fixation (fixated on an unassemblable Hellrider+Reckoner combo, so cast
nothing). Handled locally by sharpening the deck's own "Cast nothing" clause. NOT enough for a core
change from one seat -- flag for the synthesis agent to check whether other seats show the same
"fixated on an unaffordable bomb -> decline a listed body" pattern; if 2+ seats converge, the core's
phantom-lethal / plan-from-list lines may want a tightening ("a lethal or bomb you cannot pay for this
turn is not a reason to cast nothing -- deploy the best body you CAN pay for").

## 3. [FIXED -- no core action] Reply-parser desync, blocker floor.
0 desyncs across 6 games. 0 reflexive high-life blocks (wave-5 blocker floor still holding). No change.

No new core doctrine proposed from this seat.
