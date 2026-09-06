## Verdict: not safe to install

The distillation preserves much of the contract, but loses operative rules and leaves contradictory instructions that can change what seats write.

I used Python to extract full amendment bodies from the original, including the dropped amendments listed in [revision notes, lines 19–21](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/skill-revision-notes.md:19). The sampled drops are not uniformly non-rules: A84 governs threshold replacement; A259 adds a substantive verification requirement. Nothing was written.

### 1. HIGH — The literal audit orders deletion of untested rules

**V2:** [lines 504–505](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:504) require “zero-hit literals repaired or deleted.”

**Original:** [A200, lines 11988–11997](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11988) expressly says an unexercised but reachable rule **stays**, while its anecdote can be compressed; [A283, lines 13242–13244](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13242) distinguishes a window never arising from a string failing to print.

V2 preserves the UNTESTED distinction at lines 114–115 but does not apply it to this mandatory deletion gate.

**Concrete failure:** A seat deletes a valid rare lethal-response branch because its literal had zero occurrences in this corpus.

### 2. MED — Dropped A84 is a durable threshold-revision rule

**V2:** [lines 188–190](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:188) retain only the instruction to audit a fully obeyed constant; lines 118–119’s two-sided test concerns a reporting artefact, not threshold behaviour.

**Original:** [A84, lines 10175–10192](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:10175) requires counting both crossings **above** a threshold and compliance failures **below** it, replacing an inert threshold with an evidenced condition, and remeasuring its rationale even when confirmed.

Classifying this entire amendment as a non-rule drops its operative test.

**Concrete failure:** A seat retains an ineffective headcount cap because its literal sunset was unmet, without checking that the pilot ignores it in both directions.

### 3. HIGH — Presence verification replaces truth verification

**V2:** [lines 129–136](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:129) audit occurrences, old strings and takes, but never require checking computed values against game-state fields.

**Original:** Dropped [A259, lines 12836–12853](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12836) requires precisely that fourth check, over the full population where fields exist or a stated sample otherwise. Its motivating failure was a consistently printed life projection calculated from a constant, incorrect starting life.

Primitive verification cannot validate a runtime calculation.

**Concrete failure:** A seat endorses a guide threshold keyed to a demonstrably false projected-life number because the literal appeared correctly.

### 4. MED — An invented blanket ban replaces A238’s qualified permission

**V2:** [lines 302–304](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:302) say never state what the render does not print.

**Original:** [A238, lines 12617–12625](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12617) explicitly permits an absence claim backed by a re-verification grep, an explanatory primitive and a retirement prediction. A332 removes historical evidence from the guide; it does not ban this supported decision-time fact.

V2 also contradicts itself immediately: [lines 300–301](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:300) prescribe “these rows print no result.”

**Concrete failure:** One seat deletes a supported missing-verdict warning while another writes that exact warning under the neighbouring instruction.

### 5. MED — The template-only restriction is extended to all worked situations

**V2:** [lines 153–156](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:153) require a worked situation to be “a shape with blanks,” never a finished sentence.

**Original:** [A105, lines 10523–10531](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:10523) restricts sentences the guide **asks the pilot to emit**, not explanatory examples. [A332, lines 14130–14132](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:14130) permits concrete situations, and the [rewrite brief, line 27](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/guide-rewrite-brief.md:27) requires situations with right answers.

**Concrete failure:** A seat replaces useful concrete board-and-answer examples with unfinished response templates.

### 6. HIGH — PLAN ordering remains contradictory, and A330’s collision remedy is weakened

**V2:** [lines 265–272](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:265) place a gate inside the dictated PLAN sentence and require blanks filled before CHOICE, then say PLAN is written after CHOICE.

**Original:** [A282, lines 13219–13232](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13219) explicitly requires choice first. [A330, lines 14066–14070](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:14066) prefers **one ritual per guide**, requiring unavoidable additional rituals to name their replacement on other windows and explicitly prioritise the rarer, higher-stakes ritual. V2 substitutes one ritual per window class and omits those displacement instructions.

**Concrete failure:** A seat writes competing pre-choice and post-choice PLAN requirements without telling the pilot which ritual takes precedence.

### 7. MED — Deduplication contradicts required cross-phase teaching

**V2:** [lines 512–514](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:512) prohibit any rule appearing twice except as a pointer, while [lines 337–340](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:337) require capability and attack/mulligan restatements.

**Original:** [wave-26 headline 1, line 2711](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:2711) explicitly requires restating the deciding fact at the phase where it matters—not merely linking elsewhere.

**Concrete failure:** A seat satisfying the final checklist replaces the combat-specific evasion instruction with a pointer, undoing the prescribed cross-phase teaching.

## Other requested checks

- **History leakage:** V2 does carry historical narration, particularly [lines 474–481](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:474). That is not itself an A332 violation: [the original ruling](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:14128) restricts the **guide**, not the authoring skill. I would not inflate lineage tags or the verbatim ruling into defects.
- **Method already present:** Oracle retrieval, the new size ceiling, KEEP-is-success and evidence placement are explicit. The missing verification method is A259, not those basics.

**Installation should wait:** resolve the contradictory execution instructions, restore the lost verification/revision tests, and distinguish reply templates from explanatory situations.