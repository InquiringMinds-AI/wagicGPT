# Skill proposals — deck109 wave-23

## Proposal 1 (RUNG SHARPEN — real, single-strong-witness): the crutch-lifecycle CLOSED-criteria are TOO WEAK for a COST/COMPREHENSION crutch — one clean corpus does not prove redundancy when the model can ignore a correctly-rendered fact

**Context.** Wave-22 (my own P2, ADOPTED as Method-headline-4's crutch-lifecycle maturation) retired
deck109's Pillar cost-correction ("{R}, not {1}{R}") under the CLOSED-criteria: (i) the failure went
to 0 in the corpus AND (ii) the durable annotation renders on the option line (`Cast Pillar of Flame
{r}`). Wave-23 (this corpus, the confirming round-2) **falsifies the retirement for this crutch
class**: vs137 s12, the model declined an affordable Pillar reasoning verbatim *"both Searing Spear
and Pillar of Flame require {1}{R} (two mana total)"* — it conflated Pillar's {R} with Spear's {1}{R}
and **ignored the correctly-rendered option-line cost from prior belief**. The same model costed
Pillar correctly two games later (vs131 s11/s13), so the miss is STOCHASTIC.

**The distinction the rung is missing.** The crutch-lifecycle worked example base (deck102 Thoughtseize
reveal, deck44 SITUATIONS-crutch, deck109 hybrid-mana) all retired crutches for a REPRESENTATION the
engine ADDS that the model had NO OTHER WAY to know (a hidden trade outcome, a hybrid pip's payability,
an absent signal). Those retire safely: once the engine renders the fact, the model has the input it
lacked. **A COST crutch is different in kind** — the model always "knew" a cost; the crutch corrects a
WRONG PRIOR BELIEF. Rendering the correct cost on the option line does NOT overwrite the prior; the
model can (and intermittently does) reason from memory and ignore the render. So the render is
necessary but NOT sufficient — the failure mode is "belief overrides display", which a display fix
cannot reach.

**Proposed rung refinement (add to the crutch-lifecycle CLOSED-criteria):**
> A crutch that supplies MISSING information (hidden outcome, hybrid payability, absent-signal note)
> retires under the two CLOSED-criteria (failure -> 0 AND durable annotation renders). A crutch that
> CORRECTS A WRONG PRIOR BELIEF the model holds independently of the display (a cost the model can
> mis-recall, a rules interaction it mis-applies) needs a STRONGER bar: the failure must stay 0
> across at least TWO corpora with the belief-triggering shape PRESENT, because the model can ignore
> a correctly-rendered fact from prior belief and the miss is stochastic. On a recurrence, RESTORE
> the crutch — but lean and sharpened to name the specific observed conflation (do not re-add the
> old verbose lecture).

Worked example: deck109 Pillar cost retirement regressed round-2 (vs137 s12), restored as a lean line
naming the Pillar-{R}-vs-Spear-{1}{R} conflation. This is a "belief-correcting" crutch; the hybrid-
mana crutch retired ALONGSIDE it (an "information-supplying" crutch) HELD cleanly — same corpus, same
seat, opposite outcomes, isolating the variable.

## Proposal 2 (CONFIRM the attacker-scoring rung generalizes to the FIX side — no new text)

Wave-22 my-P1 added the declared-vs-recorded-for-EVERY-window attacker-scoring instrument (ADOPTED).
This wave used it to VALIDATE the fix that closed the defect it exposed (Ledger A2). Clean confirmation
that the instrument works in both directions: it FOUND the wave-22 silent under-count (vs62 s21) and
CONFIRMED its absence this wave (26/26 windows, 0 eligible attackers dropped, no prose-salvage
override). No new skill text — logging that the wave-21/22 "validate a fix on the shape still being
produced" discipline (Method-headline-3) applied cleanly here to an ATTACKER-parse fix, not just an
FP-suppression fix. The witness held only until confirmed (this corpus), per that rung.

## Proposal 3 (PASS on new deck-guide doctrine and on core)

No new guide-AUTHORING doctrine surfaced. The Pillar-cost restoration is a lean re-add of a retired
line under the (refined) crutch-lifecycle rung, not new doctrine. No core-prompt line proposed
(general-suggestions = PASS). The repetition_penalty pilot verdict is a decode/sampling synthesis
input, not skill text.
