# wave-57 core-prompt / render proposals — deck125 seat

Layer routing: engine DEFECTS are in `wave57/seats/seat-125-126.md`'s engine list (HIGH #1, #2,
#3; MED #4–#7), never here. What follows is prompt COMPOSITION and decision-UX — things that are
right today and could present better, plus the narration-budget measurement this seat owns.

## #G1 — NARRATION BUDGET BY ASK KIND: the measurement, and the ranking lane H's §D needs

Measured over my 1,469 prompt-bearing decisions (`log_window` full on all of them, 0 elided):
**20.29 MB total, narration 67.6% of the mean prompt.**

| kind | n | mean chars | mean narration | narration % | total MB | forced? |
|---|---|---|---|---|---|---|
| cast / ask | 1,176 | 14,240 | 9,667 | 67.9% | **16.75** | no |
| priority | 80 | 15,703 | 10,970 | 69.9% | 1.26 | no |
| **land drop** | **132** | 9,402 | 6,194 | 65.9% | **1.24** | **132 of 132 played; 75 menus had one `Play` row** |
| discard | 48 | 16,301 | 10,992 | 67.4% | 0.78 | no |
| blockers | 9 | 12,032 | 6,991 | 58.1% | 0.11 | no |
| reveal | 5 | 13,718 | 4,630 | 33.8% | 0.07 | no |
| announce X | 7 | 9,412 | 5,938 | 63.1% | 0.07 | no |
| mulligan | 12 | 1,488 | **0** | 0% | 0.02 | no |

Two things fall out of this table and neither needs the A/B to be decided:
1. **The mulligan ask already ships with a zero-turn window and nobody has ever complained.** It is
   the existence proof that a kind-scoped window is implementable and safe; the question the A/B is
   for is only how far to take it on the others.
2. **The land-drop ask is the right first cut and it is not close.** It is the only kind whose
   answer is 132-for-132 forced, it is 9% of my decisions, it costs 1.24 MB and ~1 hour of
   inference at 27.5 s per decision, and its decision-relevant surface is the `Land drop:` line,
   the colour counts and the hand's land — roughly 400 characters. The specimen (`125v123` seq
   176) is a two-row land drop at turn 45 carrying 11,902 characters of 45-turn narration.
   Suggested arm for §D: `kind:2` on land-drop asks specifically, before any global `kind:K`.

I am NOT proposing a global narration cut. `cast/ask` is 83% of the bytes and it is the kind where
narration plausibly earns its place (the counterspell decisions in `125v162` turn on what has
already resolved). The measurable claim is about the FORCED kinds.

## #G2 — THE TRUST DOCTRINE HAS A PRECONDITION, AND THIS CORPUS BROKE IT ONCE

The skill's trust doctrine ("never teach doubt of the render") is a rule for GUIDES. It presumes
the render is trustworthy. `126v152` seq 14 is the first record I have seen of the engine breaking
that presumption in the pilot's face: a printed `one legal assignment gets you to 1` on a board
whose true floor was −5, and a reply that spent ~250 words re-deriving the truth and writing "the
prompt's hint ... seems contradictory to the math".

That is not a doctrine problem and I am not proposing a doctrine change. It is a statement about
what the core loop owes the doctrine: **every number the prompt prints is load-bearing precisely
because guides forbid the pilot from checking it.** Proposal for the core loop's own standard:
where a printed number is a BOUND rather than a fact (a best case, a forecast, a "would be at"), it
must be provably conservative in the direction that cannot kill the seat — a survival estimate may
be pessimistic and may not be optimistic. HIGH #2 is the defect; this is the invariant it violates.

## #G3 — TWO PLACES WHERE A FINISHED NUMBER STOPS SHORT OF THE FACT THE SAME PROMPT ALREADY KNOWS

Not new emitters — the same emitters, finishing their own arithmetic:

- The `{their converter:}` tag prices ONE iteration (`life 52 -> 37`) on a board where the prompt's
  own paragraph forty lines up has already detected the Sanguine Bond + Exquisite Blood LOOP and
  says "chains until YOU are at 0". Two numbers for one board, and the reassuring one is on the
  row. (Seat file HIGH #3.)
- `INCOMING THIS COMBAT` is gated on the combat phases, so the opponent's Upkeep, Draw and Main 1
  — the last windows where an instant of the seat's can change the outcome — carry no total.
  20 such windows at my seats, including `125v146` seq 23 at 3 life against 7 printed power.
  (Seat file MED #7.)

The pattern worth stating once for the core loop: **a price is finished when it accounts for
everything the SAME prompt already asserts.** Both of these are already computed elsewhere in the
same string; neither needs new game-state access.

## #G4 — THREE WORDINGS FOR ONE FACT (X pricing), and one of them is invisible to a strict grep

`{X pricing: max affordable X=N (M mana total); ...}` (401 rows) · `{X pricing: X=N - you gain N
life...}` plus a sibling `{leaves K of your M floating mana unspent}` (28 rows) · `{X pricing: your
mana affords only X=0 right now; ...}` (7 rows, no `leaves`/`spends` clause). All three are true
and the third is why a mechanical "does every X row carry a fit clause" predicate needs three
patterns. Suggestion, cosmetic and cheap: give the X=0 form the same `{leaves M of your M ...}`
sibling the others have, so the fit clause is one string across the family. This is the D9 family's
last inconsistency and it costs nothing to close.

## #G5 — THE FORCED-ASK CLASS IS BIGGER THAN THE LAND DROP, AND THE ENGINE CAN SEE IT

132 land drops, 0 declines, 4 corpora. 405 menus this corpus carrying `NO LIVE CAST ROW ON THIS
MENU`, 0 spends off any of them. 445 zero-Tribute rows at the sister seat, 0 takes. These are three
different shapes of the same thing: **a window whose answer the engine can already predict with
certainty from its own rendered verdicts.**

I am not asking for a cap on legal choices — the standing ruling rejects those on sight, and none
of this removes a row. The ask is narrower and is about WHEN to spend a round trip:
`mana_only_windows_skipped` already exists, deck126 used it 385 times this corpus with no ill
effect, and MED #4's opponent-turn phases plus the single-row land drop are the same argument with
counted denominators. If the answer is "the model must still be asked", then the land drop is the
first candidate for a kind-scoped narration window instead (#G1) — the two proposals are
alternatives, not a pair.
