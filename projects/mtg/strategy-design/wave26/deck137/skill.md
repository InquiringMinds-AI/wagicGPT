# deck137 skill proposal (wave 26)

## PROPOSAL 1 -- AMEND the wave-25 ceiling clause: the residual generic-seam spiral is latency-only EXCEPT where it rides a representation ambiguity, which reconverts it to a correctness risk.

Wave-25 PROMOTED the ceiling clause: "floor the high-stakes seams (attackers, blockers), then
stop -- the residual generic-seam over-deliberation is a MODEL property, harmless to correctness
(choices land right), and belongs to latency/harness management, not the guide." Wave-26 data
CONFIRMS the ceiling (reply profile p50=1107 / p95=9919 / max=14603 vs wave-25 max 14,987 --
stable; nine >10k replies all on ubiquitous unfloorable seams: land drops, convoke casts,
priority passes, library reveals, shockland pay-2) -- BUT it also supplies ONE counterexample to
the "harmless" half:
- vs18 s36 (blockers, 10,817 ch): the spiral did NOT land right. It opened illegal
  (`B1:A1, B1:A2`), self-corrected, and CONVERGED on a WRONG answer (`BLOCKS: none`, declining a
  free engine-offered kill-and-survive block). The correctness failure was not caused by the
  spiral length per se -- it was caused by a REPRESENTATION ambiguity underneath the seam
  (two same-named Lovestruck Beasts, one tapped, one untapped, no instance handle; the model
  could not bind the offered B-line to the untapped copy).

**Amendment to record as a refinement (not a reversal) of the ceiling clause:**
The doctrine "floor high-stakes seams, the rest is latency" holds, WITH one caveat: a residual
spiral is latency-only when the underlying FACTS are unambiguous. When a generic seam sits on top
of a representation ambiguity (name collision, missing instance handle, contradictory-looking
render), the same over-deliberation can talk the model OUT of a correct engine-offered play --
the failure mode is not "spiraled too long" but "distrusted an authoritative offer while
spiraling." The fix for THAT is NOT another per-seat guide floor (which cannot reach a generic
seam) and NOT sampler tuning (repetition_penalty=1.05 unchanged for a 4th corpus) -- it is
REMOVING the ambiguity at the representation layer (disambiguate same-named instances). So:
(a-d from wave-25 stand); ADD (e): when a residual spiral produces a WRONG commit (not just
latency), look for a representation ambiguity beneath the seam before treating it as a guide or
sampler problem -- correctness regressions under the ceiling are almost always a FACT-surface
gap, not a missing floor.

## PROPOSAL 2 -- the "trust the engine-offered legal set BOTH directions" doctrine now has a SECOND-SEAM witness (attackers -> blockers). Still single-DECK; WATCH, do not core-promote yet.

Wave-25 PROPOSAL 2 established: a guide telling the model to trust an engine-computed legal set
must state BOTH directions ("if X is absent the condition failed" AND "if X is present the
condition is already satisfied -- act, do not re-derive"), because the one-directional form lets
the model re-litigate the present case (deck137 s21 attack-list: overrode a PRESENT Lovestruck).
Wave-26 supplies the SAME failure at a DIFFERENT seam: vs18 s36, the model overrode a PRESENT
B-list offer (Lovestruck offered as a legal untapped blocker) by re-deriving its tapped state
from the game log. Two takeaways:
- **The "trust the offered set both ways" doctrine generalizes across decision seams**, not just
  attackers -- any engine-offered legal set (A-list, B-list, target list, activation list) can be
  overridden by a model that re-derives the underlying condition. The blockers-side inverse was
  shipped to the deck guide this wave ("the offered B-list is authoritative -- a B-line creature
  is untapped and can block this turn; do not re-derive its tapped state from the log").
- **This is now witnessed at TWO seams but still ONE deck (deck137).** Per the wave-25 ceiling
  doctrine (c) -- a general "trust the offered set, don't re-derive" is a CORE/representation
  touch, never multiplied per-deck -- the promotion bar is a SECOND DECK, not a second seam.
  Keep as a per-deck clause + a WATCH toward a single core directive; do NOT promote to
  general-suggestions on one deck's two seams. Recorded in general-suggestions.md as below-bar
  accumulating evidence, not a proposal.

## Reconfirmed doctrine (no new rung) -- b2 last-answer-wins remains a spiral SAFETY NET, with one honest limit.
Eleven first!=last reversals this corpus, all parsed to the model's final coded line (c4 gate
PASS). b2 continues to protect spiral CORRECTNESS by honoring the considered final answer. The
honest limit surfaced this wave: b2 protects the answer the model CONVERGES on, so when the model
converges on a WRONG answer (s36: illegal -> none, never reaching the correct B1:A1), b2 faithfully
ships the wrong-but-legal final line. b2 is a net for snap-first-line errors that the model itself
corrects; it is NOT a net for a model that reasons its way to a wrong conclusion. That residual
(a considered-wrong final answer under a representation ambiguity) is exactly what PROPOSAL 1(e)
routes to the representation layer, not to b2 or the guide.
