# deck140 wave-11 — general (cross-deck / synthesis) suggestions

## 1 [REPLY-PROTOCOL A/B — sharpen ledger #3's reconcile-named-action arm] Handle "the plan's named action is not in the options" by mapping to the best AVAILABLE option, never an arbitrary cast
The trailing PLAN/CHOICE mismatch (ledger #3) now has enough cross-seat evidence to specify the A/B arm
precisely. deck140 wave-11 shows the residual is BIDIRECTIONAL and shares ONE root: the plan NAMES an
action for a card that is not in the offered options (verified unaffordable against the prompt's
`Mana available` line — Staff {6} on 5 mana; Pyroclasm with no red), and the trailing index mis-maps that
intent — over-casting a listed sweeper over Cast-nothing (vs135 s10, vs131 s27) OR under-casting to
Cast-nothing over an offered payable removal (vs109 s9). deck135 wave-11 Q5 independently witnessed the
same class (deck44 s34 Azcanta), so it is 2-seat this wave + the wave-10 deck110-P3 / deck140 lineage.

**Proposed reconcile-named-action arm (for the reply-protocol A/B):**
1. Parse the plan's NAMED action ("I will cast/hold/deploy X") from the prose.
2. If X maps to a listed option, and the trailing `CHOICE:N` disagrees, prefer the NAMED action's option
   (the wave-10 target).
3. **NEW — the promotable part:** if X is NOT among the listed options (the model wanted an
   unaffordable/uncastable card), do NOT accept the trailing bare integer and do NOT map to an arbitrary
   listed cast. Route to the best AVAILABLE option consistent with the plan's INTENT CLASS:
   - plan wanted to remove/sweep a threat -> the cheapest listed payable removal (e.g. the offered Cruel
     Edict), else Cast-nothing;
   - plan wanted to hold / deploy an unaffordable finisher -> Cast-nothing / pass.
   Equivalently: fall through to the engine HEURISTIC when the named action is unavailable, which the
   existing out-of-range guard already does correctly (vs131 s22 `CHOICE:5` -> `unparsed_reply` ->
   heuristic). The defect is only that an IN-RANGE-but-wrong index bypasses that fall-through.
This dominates both "accept the last bare number" (wrong in every deck140 case) and a named-action match
that only handles present options (which would still mis-handle the absent-card majority this wave). It is
NOT a core prompt-body edit and NOT a guide edit — the guide is read correctly throughout.

## 2 [HARNESS-SWEEP TAXONOMY] Separate the trailing-index MISMATCH from the PHANTOM-BOARD misread before routing a "sweep into C=0"
Two defects produce the same symptom (a sweeper cast into `(creatures: 0)`): the reply-protocol trailing
mismatch (plan reads C=0, concludes HOLD, only the index casts) vs the representation/harness phantom-board
misread (the plan itself believes a creature present and the index agrees — the wave-6 hallucinated-board /
stale `YOUR PLAN (as you last stated it)` echo, here from affinity's within-turn create-then-die churn:
deck140 vs110 s68/s82). Route by whether the PLAN's board-read matches the option-list's current
`(creatures: N)`: PLAN-vs-CHOICE mismatch = reply-protocol; PLAN-vs-BOARD mismatch = representation. A
symptom grep alone misroutes the phantom case to the A/B, which cannot fix a board misread. Candidate
representation micro-lever (WATCH, single seat): the `YOUR PLAN (as you last stated it)` echo can carry a
stale intent that references a since-dead creature; consider suppressing/annotating it when the referenced
permanent is no longer on the current board. Do NOT act on one seat — flag for a 2nd witness.

## 3 [carried, unchanged] intent-collapse-metric.py stays uncalibrated for answer-last
Confirmed again at the deck140 seat: `non-numeric heads` is vacuous by design and the reversal heuristic
throws false positives while missing the genuine mismatches. Do not compare its flag counts across the
protocol boundary; hand-audit is ground truth until it is rebuilt to parse the trailing `CHOICE:N` and
reconcile against the plan's named action. (deck140 wave-10 HARNESS #1; carried.)
