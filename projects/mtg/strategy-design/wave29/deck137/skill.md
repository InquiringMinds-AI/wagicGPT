# deck137 -> strategy-writing-skill proposals -- wave 29

## HEADLINE: the trace-ALL-variants refinement (merged wave-28) EARNED ITS KEEP on a post-fix
## binary, and it surfaced a REUSABLE resolution-predictor: the option-line enrichment annotation.

Context: this corpus is the f1 (March X-convoke) exit-gate validation. The fix PARTIALLY landed
(first-ever March token, but 8 residual defers). Reaching that verdict required exactly the rung
+ refinement the skill already carries. Confirmations and one new refinement:

1. **CONFIRM the resolution-verification rung AND the trace-ALL-variants refinement.** A naive
   read of this corpus would call f1 fixed ("March made a token, wins held at 3/6"). Tracing
   every March episode -- BOTH the convoke variant AND the plain X-cast variant -- to token
   creation showed 1/7 resolved and 8 defers still convoke-class. The plain X-cast (vs93 seq48,
   7 untapped sources) looked like the "working" route but cascaded into the same broken convoke
   sub-path and made 0 tokens. Had I traced only the direct-convoke picks I would have mis-drawn
   the boundary. Both rungs keep their standing place.

2. **NEW REFINEMENT (propose): when an engine fix ships a PERCEPTION artifact (an option-line
   annotation, a type tag, a magnitude readout), check whether that artifact's PRESENCE predicts
   resolution -- it is a free, high-signal diagnostic axis.** deck137's f1 shipped a
   `You can afford X up to N` annotation on the convoke-X option line. It appeared in EXACTLY the
   one game March resolved (vs139, 10x) and was ABSENT in every defer game -- a perfect
   correlation that pinned the residual to the can't-afford-X>=1 case in a single grep, WITHOUT
   tracing every payment sub-step. The general rule: an engine fix that renders a new
   payability/affordability signal gives the reviewer a cheap oracle -- cross-tab
   "signal present?" against "resolved?" before hand-tracing. If they diverge, the signal is
   lying; if they align, the signal partitions the residual for you.

3. **CONFIRM the "relax directives that guarded a now-fixed defect" corollary (wave-28) -- and
   ADD its complement: relax ONLY the part the evidence retired; keep guarding the part still
   broken.** deck137's Loxodon convoke steering was correctly softened wave-28 (both routes
   proven this wave: 2 arrivals, 0 defers -- the softening is validated, no misleading residual).
   But March is only PARTIALLY fixed, so the guide change this wave is NOT "stop worrying about
   March" -- it is a STRATEGIC affordability threshold (X=0 March makes 0 Soldiers per its own
   `auto=this(X=0) donothing`; only cast at X>=2-3). That clause is variant-agnostic and true
   regardless of the engine bug, AND it steers the model off the exact unaffordable picks that
   trigger the residual defer. So: after a PARTIAL fix, relax the guard on the fixed sub-case and
   REPLACE the guard on the still-broken sub-case with a strategically-sound (not bug-naming)
   preventive -- do not remove all steering just because "the fix shipped."

4. **CONFIRM the layer-routing boundary held under a strong temptation.** The tempting move was a
   guide line like "March defers when you can't afford X, so don't pick it then" -- but that
   encodes the engine bug in guide prose (forbidden). The legitimate line ("X=0 March = 0
   Soldiers = wasted, need X>=2-3") is grounded in the card's own oracle/`auto=` and reads as
   pure strategy; it happens to also dodge the defect. Verify-oracle HARD RAIL applied: checked
   the primitive (`borderline.txt`: `auto=this(X=0) donothing`, `text= ... Create X 1/1 white
   Soldier ... lifelink`) before asserting the X=0 fact. Boundary correctly placed; no new
   boundary change needed.

## Convergence signal for synthesis
The ONE refinement worth merging is (2) -- treat a fix's newly-rendered affordability/payability
annotation as a resolution-predictor and cross-tab it before hand-tracing. (3)'s complement
(partial-fix guide maintenance: relax the fixed sub-case, re-guard the broken sub-case with sound
strategy) may be independently hit by any reviewer validating a PARTIALLY-landed f/e-batch fix
this wave.
