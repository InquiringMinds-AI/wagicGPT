# deck137 -> strategy-writing-skill proposals -- wave 28

## HEADLINE: FIRST LIVE APPLICATION of the RESOLUTION-VERIFICATION rung (promoted wave-27).
## It WORKED, and it CAUGHT a real distinction the win-rate alone would have hidden.

The wave-27 rung says: never certify a defer class from the decisions AROUND it; trace the
DEFERRED ACTION to its downstream game-state effect. This corpus is its first application to a
POST-FIX binary (ad8930961 shipped the convoke fix). What the rung caught/confirmed:

1. **The rung is what distinguished CLOSED from PARTIALLY-CLOSED.** Win-rate rose 2/6 -> 3/6 and
   the fix agent's own live check showed "0 defers." A naive read ("defers dropped, wins up ->
   fixed") would have declared e1 closed. Tracing every defer to its card + tracing every
   Loxodon/March pick to a battlefield-arrival / token-creation showed: Loxodon convoke RESOLVES
   (0 defers, arrives, blocks -- vs93 end-to-end), but March is STILL 0/17 tokens with all 16
   remaining defers on it. The verdict is PARTIALLY closed, and only the resolution trace yields
   it. **CONFIRM: the rung earns its standing place -- keep it.**

2. **REFINEMENT (propose): the rung must ALSO trace the NON-deferred variant of the same play,
   not just the deferred one.** deck137's March is offered as BOTH a plain X-cast and a convoke
   cast. The plain X-cast produces NO defer record -- but it ALSO produces 0 tokens (it cascades
   through a `Cast Card Normally` menu back into the convoke defer). Had I traced only the
   deferred (convoke) picks I would have concluded "convoke is the broken path, plain works" and
   the guide's prefer-plain steering would look like a fix. Tracing the plain picks too showed
   plain March ALSO fails -- the defect is the X-announcement seam, not the convoke choice. So:
   when a play has multiple offered variants and one defers, trace ALL variants to outcome before
   attributing the failure to the deferred one. A defer is a SYMPTOM location, not necessarily the
   defect location.

3. **CONFIRM the wave-27 corollary "a verification-suppressing guide directive inherits the
   reliability of the thing it trusts," and ADD its inverse:** when the engine defect the
   directive was guarding against gets FIXED, the directive becomes obsolete and should be
   RELAXED on evidence. deck137's guide steered AWAY from convoke because convoke was broken; now
   that Loxodon convoke resolves (proven live), the blanket "prefer plain" is over-constraining --
   this wave's strategy.txt softens it to "either cast works reliably; prefer plain only to keep
   creatures untapped." Propose adding to the rung: **after an engine fix ships, re-audit the
   guide directives that were written to avoid the old defect and relax the ones the fix retired
   -- a guide that keeps steering around a fixed bug leaves value on the table AND masks whether
   the fix works** (the softening also keeps deck137 exercising the convoke path so future
   regressions surface here, the sole convoke seat).

4. **CONFIRM the layer-routing discipline held under temptation.** March is fully engine-bound
   (both variants fail). The tempting guide move is "tell the model March doesn't resolve, stop
   casting it" -- but that describes an engine bug in guide prose (forbidden). The legitimate
   guide action is the variant-agnostic anti-re-pick behavior clause ("one attempt per card per
   turn, then act"), which routes to a working alternative without naming the bug. Kept there.
   No new skill change needed; this is a confirmation that the existing boundary is correctly
   placed.

## Convergence signal for synthesis
The resolution-verification rung is now battle-tested on a post-fix binary and the ONE refinement
worth merging is (2) -- trace ALL offered variants of a play, not only the deferred one. (3) is a
guide-maintenance corollary (relax directives that guarded now-fixed defects) that other reviewers
touching fixed e-batch items this wave may independently hit.
