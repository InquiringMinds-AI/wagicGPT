# deck140 wave-15 — strategy-writing-skill proposals

Guide disposition: **FROZEN, byte-identical** (`cmp` clean vs live, 7364 bytes; and identical to the
wave-13 copy). This is the 6th corpus in which the count-anchored SWEEP TRIGGER is read correctly at
every window and the #1 Rakdos-hold / WHICH-SWEEPER / X-menu / Staff-face-ping / Lightmine / Venser's
/ Mulligan / Lands lines are obeyed wherever the MODEL controlled the outcome. Every residual routes
BELOW the guide (engine representation + auto-tap, and deck construction). No decision-level failure
CLASS is model-fixable by wording. **No guide delta.**

Two reviewer-METHOD proposals, both grounded in this wave; neither touches guide prose.

## Proposal 1 [REVIEWER-METHOD — grounded in Owned Item 2] Measure the dual-hiding defect by the OFFERED-vs-DISPLAY color diff, and separate the count from the impact
Wave-13 recorded the "Mana available hides dual second colors" defect from a single hand-found witness
(vs109 s9). Wave-15 shows the defect is far more pervasive (48 instances / 5 of 6 games) and that a
cheap, deterministic detector finds them all: for each prompt, diff the colored pips in the display
line against the colored pips required by each OFFERED "Cast … {cost}" — the offered list is
authoritative, so any offered cast needing a color absent from the display line IS a dual-hiding
instance. The authoring point for the skill's harness-sweep taxonomy: **frequency-count and
game-impact are DIFFERENT measures and must be reported separately.** This corpus had 48 display
contradictions and ZERO misplays, because the model treated the offered-options list as authoritative
(it even cast an "unaffordable-looking" red spell at vs135 s20). A reviewer who reports only the raw
count over-states harm; one who reports only "no misplay" under-states a real, pervasive engine defect
that WILL bite the day the hidden color is the only path to a needed cast (the wave-13 vs109 shape).
Report both: N contradictions AND K that changed a decision.

## Proposal 2 [REVIEWER-METHOD — grounded in Owned Item 1] For an X-payment / auto-tap engine item, "no recurrence" must distinguish NOT-REPRODUCED from NOT-EXERCISED
The wave-13 Rakdos X-underpay lived at the auto-tap/X-payment layer. This corpus produced ZERO Rakdos
casts — the card was held every game (correctly), so the defect got NO test. That is NOT the same as
"the fix held": a symptom-absence grep would falsely retire the item. The tell the skill should
codify: for an engine item that only manifests during a specific action (an X-spell cast, a forced
sacrifice, a block), first confirm the ACTION OCCURRED in the corpus before scoring recurrence. Where
the adjacent action WAS exercised (3 Black Sun's Zenith X-menus here — each surfaced as its own `ask`,
paid option-1 = biggest affordable, including a correct deliberate X=1), record that as partial,
mechanism-adjacent evidence, but keep the specific untested item OPEN. "No witness" != "resolved."

## Nothing else — no demotions
The pause-aware demotion terminus requires OBSERVED-OBEYED with a replacing surface. Nothing qualifies:
the SWEEP TRIGGER, tapped/artifact/shroud belief-kills, and the #1 Rakdos-hold are core mechanics read
correctly (not demotable guardrails); BSZ-X, Staff face-ping, Lightmine, Venser's, Mulligan, Lands all
obeyed where exercised. No single-seat evidence cuts any method or core line. Core stays PASS.
