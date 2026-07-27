# deck137 -> strategy-writing-skill proposals -- wave 30
# THE CONVOKE ARC'S CLOSING CHAPTER (what six waves of one engine defect taught the reviewer)

## HEADLINE: this seat's whole value for the last six waves was as a SINGLE-DEFECT OBSERVATION
## POST, and the arc's durable lesson is about REVIEW METHOD, not about GW Adventures. One new
## refinement earns merging (arrival-tracing beats fallback-counting); the rest CONFIRM.

### THE ARC (harmless defers -> pinned root -> partial fix -> the metric trap)
deck137 has exercised convoke (Venerated Loxodon fixed-cost + March of the Multitudes X-cost)
since it entered the pool. The convoke defect surfaced, was mislabeled, got pinned, got partially
fixed, and this wave revealed that the "fixed" signal was partly an artifact of the METRIC. The
arc, compressed:
  - EARLY ("harmless defers"): March/Loxodon convoke picks fell back to the heuristic. Read as
    noise -- low count, deck still functioned, no obvious loss attributed. The reviewer's temptation
    was to write it off. LESSON: a recurring fallback at a seat that exercises a mechanic NO OTHER
    seat exercises is never noise -- it is the seat's entire reason to exist. Do not discount a
    low-count residual that is UNIQUE to one seat.
  - MID (0/17, 0/16 tokens): tracing revealed March NEVER made a token across two whole corpora.
    The "harmless" defers were a TOTAL payoff failure hiding behind a small number. LESSON: count
    the OUTCOME (tokens created), not the DECISION (picks made). A mechanic can be picked every
    game and deliver zero times.
  - TRACE-ALL-VARIANTS: the plain X-cast route LOOKED like it worked (7 untapped sources) but
    cascaded into the same broken convoke sub-path and made 0 tokens. LESSON: trace EVERY variant
    (convoke AND plain) of the same card to its game-state effect; the "working-looking" route can
    fail identically.
  - PARTIAL FIX (w29): {X} propagated onto the convoke shell + X-announce -> FIRST token ever
    (X=1), but 8 defers remained. The X-affordability annotation appeared and proved a PERFECT
    resolution predictor (present -> resolves; absent -> defers). LESSON: when an engine fix ships
    a PERCEPTION artifact, cross-tab its presence against resolution before hand-tracing -- a free
    high-signal oracle.
  - THIS WAVE (w30, payability-before-offer): defers 8 -> 2, first MULTI-token March (X=3), churn
    tamed. Looked like closure. But arrival-tracing found 3 UNLOGGED silent no-ops on offered
    convokes that the defer counter never saw. The gate was staged on "2 defers"; the truth was
    "2 defers + 3 silent no-ops, ~55% offered-convoke resolution." LESSON (the new one, below).

### THE ONE REFINEMENT TO MERGE
**A defer/fallback counter UNDERCOUNTS resolution failure whenever the same failure can also end
SILENTLY. Verify a "residual -> 0" exit gate by ARRIVAL-TRACING (did each offered choice produce
its game-state effect?), never by counting the fallback field alone.** deck137's convoke failure
has TWO exits: a logged `deferred_to_heuristic` and an UNLOGGED silent no-op (the engine consumes
the pick, taps mana, and abandons the cast with no record). The corpus reported 2 defers; the true
offered-but-unresolved count was 5. Had I trusted the fallback field, I would have declared the
gate met and rotated the seat on a false-clean signal. The general rule for ANY exit gate phrased
as "fallbacks/defers -> 0": restate it as "offered choices that produce no downstream effect -> 0"
and verify by tracing arrivals in the narration, because an engine can decline an offered choice
without emitting a fallback record. Fallback count is a LOWER BOUND on decision failure, not a
measure of it.

### CONFIRMATIONS (all held again this wave; not re-proposing, logging their standing)
1. The resolution-verification rung + trace-ALL-variants refinement: still load-bearing. A naive
   read would call w30 fixed ("March made 3 tokens, churn gone"). Tracing every episode showed
   1/5 resolved.
2. The perception-artifact-as-resolution-predictor refinement (merged from w29): CONFIRMED a third
   time. The `You can afford X up to N` annotation partitioned resolved-vs-failed perfectly again
   (present only in vs152 t12, the only clean resolution).
3. The partial-fix guide-maintenance corollary: CONFIRMED. After a partial fix, relax the guard on
   the fixed sub-case, keep a STRATEGICALLY-SOUND (not bug-naming) preventive on the broken one.
   deck137's guide needed NO change this wave -- the X>=2-3 threshold and one-attempt clause both
   worked; softening the Loxodon prose further would have encoded the bug or over-steered.
4. Layer-routing boundary: held. Every residual is engine-lane; general-suggestions is PASS #25.

### A NOTE ON ROTATION DISCIPLINE (for the synthesis agent / rotation policy)
This seat is the textbook case of a SINGLE-DEFECT OBSERVATION POST: comprehension long converged,
closing-speed resolves to construction + pool, and the ONLY thing it observes is one engine defect
seen nowhere else. The rotation temptation each wave is "the model side is clean, the residual is
engine-lane, so rotate." The discipline: an engine-lane residual that is observable at EXACTLY ONE
seat holds that seat until the residual CLOSES (arrival-traced), because rotating it out removes
the only instrument that can tell you whether the next fix worked. "Engine-lane, not a seat hold"
is true only when the mechanic is ALSO exercised elsewhere. Convoke is not.

## Convergence signal for synthesis
The refinement worth merging is arrival-tracing over fallback-counting for "residual -> 0" exit
gates -- it is general to any seat validating a fix whose failure mode can be silent (silent
declines, dropped optional triggers, auto-passed abilities). It extends the w29 resolution-
verification rung: the rung says "trace the outcome"; this wave says "you MUST trace the outcome
because the failure counter can read clean while the outcome is broken."
