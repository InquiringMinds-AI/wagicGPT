# deck140 wave-11 — strategy-writing-skill proposals

Guide disposition this wave: **FROZEN, byte-identical** (`cmp` clean, 7364 bytes). No decision-level
failure CLASS is model-fixable by wording — the SWEEP TRIGGER validated a 4th corpus (READ correctly at
all 24 relevant windows; 23 clean fires), the ramble/truncation guard shipped and VALIDATED, and every
residual routes to the reply-protocol layer, representation/harness, or deck construction. The two
proposals below are METHOD refinements grounded in this wave's findings; both are reply-protocol/harness
layer, and Proposal 1 now CROSSES the single-seat bar (2 seats this wave + wave-10 lineage), so it is
offered to the synthesis agent as a promotable sharpening — not a core/guide cut.

## Proposal 1 [METHOD — grounded in Finding 3, PROMOTABLE] The answer-after-plan trailing-mismatch residual has a single ROOT — the UNLISTED/UNAFFORDABLE-CARD FIXATION — and it is BIDIRECTIONAL; the reconcile-named-action A/B arm must handle "named action absent from options"
Wave-10's skill named the trailing PLAN/CHOICE mismatch as an answer-LAST shape that fails
ASYMMETRICALLY (casts a SPELL over a HOLD), and flagged deck110-P3 as a single-seat sub-variant (the plan
fixates on an UNLISTED card and emits a poorly-mapped fallback label). **Wave-11 promotes that
sub-variant to the MAIN shape and shows it is the common root of BOTH failure directions.** At deck140
every genuine mismatch this corpus is: *the plan's NAMED action is a card that is NOT among the offered
options because it is unaffordable this turn, and the trailing index maps the intent onto a WRONG listed
option* — verified against the prompt's `Mana available` line in every case (Staff {6} on 5 mana; a
Pyroclasm the pilot has no red for). It fails in two directions from that one root:
- **OVER-cast** — wants an unlisted FINISHER on an empty board, maps to a listed SWEEPER over Cast-nothing
  (vs135 s10, vs131 s27: Pyroclasm into C=0).
- **UNDER-cast** — wants an unlisted SWEEPER at C>=1, maps to Cast-nothing over an OFFERED payable removal
  (vs109 s9: ignored the offered Cruel Edict at a 5-creature aggro board).
Cross-seat THIS wave: deck135 Q5 independently witnessed the under-develop direction (deck44 s34 —
"transform Azcanta" desire, unoffered, mapped to Cast nothing over an offered develop spell), citing the
same deck110-P3 lineage. So the class now has **2 witnessing seats in wave-11 alone + the wave-10
lineage**, and it CROSSES the single-seat bar. **Authoring point for the reply-protocol A/B (ledger #3
reconcile-named-action arm):** the reconciler must parse the plan's NAMED action, and **when that named
action is ABSENT from the options (the model wanted an unaffordable/uncastable card), route to the best
AVAILABLE listed option — a payable removal if the plan wanted removal, the pass/Cast-nothing if the plan
wanted to hold or deploy an unaffordable finisher — NEVER an arbitrary listed cast.** This dominates both
"accept the last bare number" AND a naive named-action match that only handles present options. Route to
the code-appended reply-protocol layer; never re-word the guide over it (the guide is read correctly
throughout).

## Proposal 2 [METHOD — grounded in Finding 4/6] Distinguish the trailing-index MISMATCH (plan disagrees with choice) from the PHANTOM-BOARD misread (plan AGREES with a wrong choice) before routing — they share a symptom (a wasted sweep into C=0) but different layers
A reviewer scoring "sweeper cast into `(creatures: 0)`" must READ the plan before routing, because two
distinct defects produce the identical symptom: (a) the **trailing-index mismatch** (Finding 3) — the
plan reads C=0 and concludes HOLD, only the emitted index casts (reply-protocol layer); vs (b) the
**phantom-board misread** — the plan itself believes a creature present and the index AGREES with that
wrong plan (representation/harness layer, the wave-6 hallucinated-board / stale `YOUR PLAN (as you last
stated it)` echo, here driven by affinity's within-turn create-then-die churn: vs110 s68/s82). Symptom
grep alone conflates them and would misroute (b) to the reply-protocol A/B, which cannot fix a board
MISREAD. The tell is whether the PLAN's stated board-read matches the option-list's current
`(creatures: N)`: mismatch of PLAN-vs-CHOICE = reply-protocol; mismatch of PLAN-vs-BOARD = representation.
This is the plan/choice-mismatch twin of the wave-6 hallucinated-board class, and it sharpens the
harness-sweep taxonomy.

## Nothing else — no demotions
The pause-aware demotion terminus requires OBSERVED OBEYED with a replacing surface. Nothing qualifies:
the SWEEP TRIGGER, tapped/artifact/shroud belief-kills, and the #1 Rakdos-hold are core mechanics read
correctly (not demotable guardrails); the BSZ partial-sweep guardrail exercised once and read correctly
(freeze); the Lightmine second-copy line's precondition did not recur. No single-seat evidence here cuts
any method or core line. Core stays PASS (the mana-misread at vs109 s9 is the colored-mana/offered=payable
family — already carried verbatim in the core body; representation route stands).
