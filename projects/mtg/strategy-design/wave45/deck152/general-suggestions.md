# deck152 — core-prompt / render proposals (layer-routed)

Engine bugs and render gaps are in the seat file's ENGINE / RENDER ITEMS list; this file is only for
CORE PROMPT text and render conventions.

## G-5 (MED, render convention) — a benefit inside a fatal parenthesis needs its branch named
`(your attacker dies, their blocker lives (lifelink: you gain 3))` produced a reply that added the
3 life to the 3 face damage. Every other clause in that parenthesis describes the BLOCKED fight, so
the tail is internally consistent — but on a line whose head is fatal, a bare benefit reads as an
addition. Convention proposal: bind the number to its branch, e.g. `(your attacker dies, their
blocker lives (lifelink: you gain 3 from the block, and this attacker deals nothing to their life))`.
Three records, one dead lord. Docketed as seat-file E-1.

## G-6 (LOW, core prompt) — the side-chooser sub-ask still has no subject
All nine remaining `A choice is required - choose an option:` headers in the corpus are on this
deck, and all nine are Teferi's +1 side chooser (`choose your land / choose opponent land`,
`choose your creature / choose opponent creature`). The parent ask is correctly headed
`Choose an option for Teferi, Who Slows the Sunset:` — the E-3 fix reached the modal root and not
the TargetChooser sub-ask beneath it. **No fallback resulted this corpus** (0 on this seat, 2 in
the whole corpus, neither here), so this is legibility, not parse risk: the model cannot tell from
the sub-ask which spell it is resolving or what the choice does. Cheapest fix is to carry the
parent's source name into the sub-ask header. Docketed as seat-file E-5.

## G-7 (LOW, core prompt) — the converter line is good and could name the second half
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` rendered correctly and is a
real improvement. In the game where it fired, the opponent ALSO controlled Exquisite Blood, and the
pair is an unbounded loop that killed deck152 from 22 to 0 on one ping. The line describes each
converter singly. If a second converter of the reverse polarity (gain-on-their-loss) is on the same
battlefield, saying so would be worth a clause — but note this is a rules-content proposal, not a
bug: nothing rendered was false, and deck152 had no answer to the board in any case.

## G-8 (informational) — plan-note precision
Audited all 49 fires on my three seats against their option lists: 49/49 TRUE. Land-drop fires
0/238 corpus-wide (was 86/125). Nothing to propose; recording the measurement so the lane can be
closed on a real denominator rather than a reconstruction.
