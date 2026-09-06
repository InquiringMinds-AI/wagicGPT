# Round 2 adversarial review

**Not safe to install yet.** Five first-round findings are resolved; one fix introduces an overbroad retention rule, and the reply-order contradiction remains. Four new findings follow.

Read-only review: I used Python to extract amendment bodies and sampled the notes’ dropped set, including A117, A132, A198, A208, A223, A240, A297, A299, A312 and A329. These are not uniformly disposable narration: A198 specifies consequential review methodology. Nothing was written.

## First-round findings: verification

1. **REGRESSED — MED: zero-hit literals.** [V2:130–132](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:130) now correctly preserves unexercised rules, but also mandates keeping rules whose windows the deck **cannot produce**. [Original A200:11992–11996](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11992) preserves the rule **because its branch remains reachable**; A283 classifies structural impossibility without prescribing retention. V2 also bans text for inactive window classes at line 498.  
   **Failure:** A seat retains an impossible branch indefinitely as “UNTESTED,” spending the guide’s limited bytes on non-applicable instruction.

2. **RESOLVED — formerly MED: threshold revision.** [V2:189–195](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:189) restores both directions of the test, replacement of inert thresholds, and remeasurement of the rationale, matching [Original A84:10175–10192](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:10175).

3. **RESOLVED — formerly HIGH: truth verification.** [V2:122–128](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:122) requires checking computed values against game-state fields, with population/sample scope, matching [Original A259:12849–12853](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12849).

4. **RESOLVED — formerly MED: absence claims.** [V2:311–320](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:311) restores qualified permission and puts its three receipts in review.md, consistent with [Original A238:12617–12625](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12617).

5. **RESOLVED — formerly MED: examples versus templates.** [V2:152–158](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:152) distinguishes concrete explanatory situations from sentences the pilot must emit, matching [Original A105:10523–10531](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:10523) and A332.

6. **NOT RESOLVED — HIGH: reply ordering.** [V2:270–275](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:270) still instructs seats to write “the first line of your answer is \<number\>,” then mandates CHOICE first. Declaring “A282 governs” does not remove the conflicting text seats are expressly told to put into guides. [Original A79:10115–10118](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:10115) supplies the older instruction; [A282:13228–13232](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13228) requires choice first. A330’s ritual-displacement remedy **is** restored at v2:275–277.  
   **Failure:** A seat writes both a mandatory number-first reply and a mandatory CHOICE-first reply.

7. **RESOLVED — formerly MED: cross-phase repetition.** [V2:285–288 and 513–516](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:285) explicitly exempt deciding facts from deduplication, preserving [Original wave-26 headline 1:2711](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:2711).

## New findings

### 8. MED — Lost mulligan verification cases

**V2:** [380–389](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:380) preserves A131’s twelve-hand construction but omits A153’s added **two-land/no-coverage** cases and conditional carried-PLAN test.

**Original:** [A153:11309–11318](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11309) explicitly extends that test: exercise two lands without coverage at keeping seven and six; if a second shippable property survives, test the resulting PLAN against the next hand. Merely citing A153 does not preserve its method.

**Failure:** A seat validates all twelve listed hands while leaving a coverage clause that wrongly ships a two-land hand or propagates a rejected hand’s plan into the next keep decision.

### 9. MED — Dropped A198 loses the independent-failure check

**V2:** [102–113](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:102) requires window counts and raw/live/consequential splits, but no distinct-game/turn count.

**Original:** Dropped [A198:11967–11973](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11967) requires reporting distinct `(game, turn)` counts beside N/M because repeated identical windows can represent one failure, not independent trials. None of v2’s three splits captures that distinction.

**Failure:** A seat treats several same-turn repeats as independent evidence and writes a stronger guide intervention than the observed failure supports.

### 10. MED — Perception routing forbids the route-around later prescribed

**V2:** [75–79](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:75) routes unprinted facts to the ledger, “never the guide”; [305–306](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:305) nevertheless permits reconstructing a quantity from true printed surfaces.

**Original:** [A287:13289–13298](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13289) expressly authorizes that guide-side arithmetic workaround, subject to retirement. V2’s initial classifier needs this exception, not an unconditional rejection.

**Failure:** One careful seat removes the temporary MDFC land-count correction as a perception problem, while another writes it under the route-around instruction.

### 11. MED — “Whole contract” leaves rewrite output scope ambiguous

**V2:** [5–7](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:5) names the live `bin/Res` guide as the edit target and calls itself “the whole contract”; [61–63](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:61) places verification in review.md.

**Original/contract:** [A332:14134–14136](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:14134) separates guide from evidence but grants no live-install instruction. The [rewrite brief:3–5](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/guide-rewrite-brief.md:3) expressly forbids `bin/Res` edits; [38–42](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/guide-rewrite-brief.md:38) requires staged strategy.txt and changes.md. V2 never distinguishes ordinary review from this rewrite workflow.

**Failure:** A rewrite seat following v2 as its complete contract edits the live guide and omits the required staged changes.md.

## Remaining checks and verdict

Oracle retrieval, the replacement size ceiling, KEEP-is-success, and evidence placement are explicit. Historical material remains in v2’s ruling and lineage discussion, but that is **not a new leak defect**: A332 restricts guides, not authoring skills.

**Verdict: not safe to install.** Resolve the two outstanding first-round issues, restore the omitted verification methods, and explicitly scope the route-around and rewrite-workflow exceptions.