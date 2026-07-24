# deck131 wave-24 — skill.md proposal

One reviewer-method proposal for the strategy-writing skill. Everything else at this seat is covered by
existing doctrine (rotation new-leak hunt, answer-first scoring, the wave-23 claimed-vs-actual-tap diff,
crutch lifecycle). This wave adds a rung the wave-23 tap-diff did not cover.

## PROPOSAL — SEPARATE "WRONG BOARD READ" FROM "RIGHT READ, WRONG EXTRACTION": diff the first ACTION line against the reply's own CONCLUSION

Method-headline candidate (single-seat now; deck131 vs27 s30, GAME-RELEVANT): the wave-23 skill added a
claimed-vs-actual-tap diff to catch a "safe swing" rationale resting on a FALSE board read. This corpus
produced a DISTINCT failure the tap-diff cannot catch: a reply whose board read is CORRECT and whose
reasoning explicitly CONCLUDES the guide-correct action ("you must leave Guttersnipe home... attack with
none"), but whose FIRST line — the answer-first token the harness locks — names the opposite action
(`ATTACK: A1`), reached via a *"Correction: Wait, looking at the prompt again..."* self-correction spiral.
The harness executed the first line; Guttersnipe died. A claimed-vs-actual-tap diff PASSES this reply (the
tap read was right); only diffing the leading `ATTACK:`/`CHOICE:` line against the reply's own concluding
recommendation catches it.

So the attacker/decision-scoring procedure now needs THREE diffs, in order:
1. **execution fidelity** (wave-22): declared-set == recorded-set.
2. **decision-input fidelity** (wave-23): claimed-board-state == actual [tapped] tags, for any "this is
   safe because X" rationale.
3. **answer-vs-conclusion fidelity** (NEW): the leading action line == the reply's own concluding
   recommendation. When a coherent reply's body argues its way to a DIFFERENT action than its first line,
   the harness locked a wrong answer-first token — score the decision by what the model CONCLUDED, and
   flag the extraction failure to the harness lane (NOT the guide lane; no guide prose fixes a
   first-line-vs-body contradiction).

Why this is a distinct rung and why it matters for verdicts: it separates a MODEL-COMPREHENSION miss
(rung 2 — the model was wrong) from a HARNESS-EXTRACTION miss (rung 3 — the model was RIGHT but the
first token wasn't). Conflating them mis-routes the fix: rung-2 misses tempt a guide edit (usually the
sanctioned-against "louder"), while rung-3 misses are purely harness/answer-first and must never touch the
guide. deck131 this corpus had BOTH in the same matchup (vs59 s23 = rung 2; vs27 s30 = rung 3), which is
exactly why the diff pair must be run separately — the two look identical from the win column (a lost
Guttersnipe) but route to opposite lanes.

Corollary already applied at deck131 (guide lane, unrelated to the above): when a brand-new engine PHASE
appears (this wave: the London-mulligan BOTTOM step), audit the guide for ZERO-coverage of the new
surface before scoring any decision in it as a pilot error — an uncovered new surface warrants an
ADDITIVE guide line using the guide's existing valuations (deck131 got a BOTTOM-priority line reusing its
Prism Ring/Elixir-lowest ranking), which is NOT the sanctioned-against "louder at a covered line."

Promotion threshold: a 2nd seat showing a first-line-vs-conclusion contradiction promotes rung 3 to a
standing decision-scoring step. Single-seat now — proposing the reviewer diff (cheap, catches a class the
tap-diff and the win column both hide), not a cross-seat mandate.
