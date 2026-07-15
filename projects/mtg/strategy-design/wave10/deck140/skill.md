# deck140 wave-10 — strategy-writing-skill proposals

Guide disposition this wave: **FROZEN, byte-identical.** No decision-level failure CLASS is
model-fixable by wording (Finding 1 validates the trigger a 3rd corpus; every residual routes to the
reply-protocol layer, the tooling ledger, confirmed-infra, or deck construction). So there is no guide
method to extract from a diff. The two proposals below are METHOD refinements grounded in this wave's
findings; both are reply-protocol/tooling-layer, single-seat-CONFIRMING (not core-cutting), for the
synthesis agent to weigh cross-seat.

## Proposal 1 [METHOD — grounded in Finding 4] When an engine change alters the reply PROTOCOL, any pre-existing diagnostic METRIC must be re-derived for the new protocol BEFORE its flag-count is compared across arms
The wave method already treats a shipped fix as a hypothesis until you see it FIRE, and already splits
"score the reasoning" from "score the outcome." Add the metric-side corollary: **a diagnostic built to
parse the OLD reply shape silently mis-scores the NEW one, in BOTH directions.**
`tools/intent-collapse-metric.py` was written for the head-first protocol; under answer-after-plan it
reports `non-numeric heads: 215/215` (its head parser is now vacuous) and its `reversal + concluded !=
choice` heuristic produced 21 deck140 flags that hand-audit shows are ~all FALSE POSITIVES (it matches
the ubiquitous "hold Rakdos" plan-tail mention and mis-indexes multi-option lists) while MISSING all 4
genuine mismatches. The naive cross-arm read (135 corpus vs 120 control) would falsely register
intent-collapse as WORSENING. Authoring move: before citing a metric's count across a protocol boundary,
confirm the metric's PARSE still matches the reply shape; if not, hand-audit is the ground truth and the
metric is rebuilt (parse the trailing `CHOICE:N` as the answer; compare against the plan's NAMED
conclusion with stronger option-text disambiguation) before any number is trusted. This is the
verification-metric-SPLITS rung extended to a protocol CHANGE, not just a fix.

## Proposal 2 [METHOD — grounded in Finding 3] The answer-after-plan protocol's residual is a distinct class — trailing PLAN/CHOICE mismatch that systematically casts a SPELL over a HOLD — and its durable fix exploits that the plan NAMES its choice in prose
The skill currently names two reply-protocol sub-classes: head-first INTENT-COLLAPSE (answer emitted
before reasoning, head never revised) and PLAN/CHOICE MISMATCH (head int matches a passive option, the
reasoning is for a different action). This corpus adds a THIRD shape specific to answer-LAST: the
reasoning PRECEDES the answer and reaches the CORRECT decision, yet the trailing `CHOICE:N` contradicts
the plan's stated conclusion — and it fails ASYMMETRICALLY, selecting the offered SPELL when the plan
wanted to HOLD/cast-nothing (4/4 deck140 windows: 2 wasted Rakdos kill shots, 1 Pyroclasm-into-C0,
1 Cruel-Edict-into-C0). Harness indexing is verified consistent 1-based, so this is a MODEL
number-emission defect, not an off-by-one. The authoring point for the reply-protocol A/B: **the model
reliably NAMES its choice in prose** (`"I will hold Rakdos"`, `"I will cast Staff of Nin"`), so the most
promising A/B arm reconciles the trailing index against the plan's NAMED action (or requires echoing the
option TEXT, not a bare integer) — this dominates "accept the last bare number," which would have taken
the wrong number in all 4 cases. Route to the code-appended reply-protocol layer; never re-word the
guide over it. deck140 is a 2nd witnessing seat (after deck135) that the mismatch class SURVIVES the
protocol shift — evidence the A/B is still DUE, with this sharper target.

## Nothing else
No demotion proposals: the pause-aware demotion terminus requires OBSERVED OBEYED, and while the shroud
line reached that bar this wave (Finding 2), the resume/pause context + thin sample (1 seat / 2 windows)
+ its status as a compact rules-fact with no replacing representation all argue KEEP at full strength.
The tapped/artifact belief-kills and the count-anchored trigger were obeyed but are core mechanics, not
demotable guardrails. No single-seat evidence here cuts any method or core line.
