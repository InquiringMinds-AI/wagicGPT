# general-suggestions.md (core-prompt proposals) -- deck102 wave 26

PASS -- no core-prompt change proposed this wave (21st consecutive PASS from this seat's routing view).

The one behavioral phenomenon worth a core-prompt sentence is the REVERSAL-SPIRAL (deck18 s5, deck27
s9, deck137 s8: the model emits an opening CHOICE line, then reconsiders and reverses it two-to-three
times before landing a final answer). It is TEMPTING to propose a core nudge to "commit to one answer
and stop reversing." **That temptation should be REFUSED, and this seat's evidence is why:** in all
three cases the reversal reached the STRICTLY BETTER answer -- Sol Ring -> Animate Dead (a guide-
correct threat-deploy), Witch's Cottage -> Barren Moor (correct tapped-land/Cycling reasoning), and a
genuine block re-evaluation ending correctly at BLOCKS: none. Suppressing the reconsideration would
suppress the improvement. The COST of the spiral (135-224s latency, long replies) is already fully
absorbed by the shipped c4 parser hardening (line-anchored last-coded-line selection), which resolved
all three to the model's true final answer with ZERO fallbacks and ZERO misfires. The harness handles
the spiral; the spiral produces better decisions; there is nothing for the core prompt to fix.

So the reversal-spiral routes to notes.md as a benign WATCH (does a future spiral ever reverse PAST
the correct answer, or into an out-of-range index?), NOT to core.

All other items this corpus route BELOW/BESIDE core:
- 0/6 collapse -> pool-rise + draw-variance, decisions sound -> findings.md; no routing.
- mulligan edit first corpus -> VALIDATED (6/6 correct keeps, over-mulligan class did not recur) -> notes.md.
- steal/Tergrid -> 0 live windows, discharged by probe -> notes.md.
- R-DFC-BACKFACE (un-re-exercised 4th wave) -> ENGINE, master ledger -> notes.md.
- c1 target-preview opponent-first, c4 parser hardening -> VALIDATED at this seat -> findings.md/notes.md.
