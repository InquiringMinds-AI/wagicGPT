# Round 3 adversarial review

**Not safe to install yet.** All six requested round-2 findings are resolved, but the revised text still restores superseded mulligan policy and loses substantive verification constraints.

Read-only review; nothing written. I used the recall-working-memory skill for instruction-design context and Python to extract original amendments, including the notes’ dropped A117, A132, A167, A198, A208, A217, A222, A223, A230–231, A240, A260, A262–263, A276, A295–300, A312, A327 and A329. The dropped set is not uniformly non-operative: A222 contains a consequential guide-validation method.

## Round-2 findings: verification

1. **#1 — RESOLVED (formerly MED): unreachable-rule retention.**  
   [V2:136–140, 505–508](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:136) now distinguish reachable-but-unexercised branches, which stay, from structurally impossible windows, whose text is cut. This preserves [original A200:11992–11997](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11992) without prescribing indefinite retention of impossible branches.

2. **#6 — RESOLVED (formerly HIGH): reply ordering.**  
   [V2:276–290](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:276) explicitly retire number-first output and specify compute → CHOICE → PLAN. That matches [original A282:13228–13232](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13228); the one-ritual preference and displacement rule also preserve [A330:14066–14070](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:14066).

3. **#8 — RESOLVED (formerly MED): mulligan test cases.**  
   [V2:392–397](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:392) restore two-land/no-coverage cases at keeping seven and six, plus the conditional carried-PLAN test required by [original A153:11309–11318](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11309).

4. **#9 — RESOLVED (formerly MED): independent-failure counts.**  
   [V2:107–110](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:107) now require distinct `(game, turn)` counts beside N/M, preserving [original A198:11967–11973](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11967).

5. **#10 — RESOLVED (formerly MED): perception-classifier exception.**  
   [V2:77–84, 310–311](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:77) explicitly permit the temporary arithmetic route-around using true printed surfaces, with retirement machinery, as required by [original A287:13289–13298](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13289).

6. **#11 — RESOLVED (formerly MED): installation and rewrite scope.**  
   [V2:5–10](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:5) now prohibit live-guide edits, specify ordinary staging, and defer rewrite workflow to its brief. This respects [original A332:14134–14136](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:14134) and [brief:3–5, 38–42](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/guide-rewrite-brief.md:3).

## New findings only

### 12. HIGH — Superseded fixed-look mulligan policy is restored

**V2:** [385–397](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:385) prescribe a “hard floor,” keeping any land-bearing hand at keeping five or lower, and stopping the chain at the second look.

**Original:** [A119:10743–10761](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:10743) explicitly **supersedes A66/A67’s fixed stop look as a floor**. It requires pricing each look, stating deck land/colour-source counts, and constructing a bottoming preference order satisfiable at every N. [A153:11316–11318](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11316) reiterates “Odds, never a floor.”

V2 cites A119 but retains the superseded command and omits its explicit any-N satisfiability requirement; it also conflicts with its own odds-based doctrine at lines 68–70.

**Concrete failure:** A seat reinstalls an unconditional stop-look rule instead of writing the deck-specific odds comparison and a bottoming order that remains feasible at small keeps.

### 13. MED — Render-derived explanation can be retired before the required validation

**V2:** [168–173](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:168) list “derivations the render now performs” as material to cut, retaining one “answer the marked row” sentence. [268–270](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:268) similarly retain derivation only for the unmarked case.

**Original:** [A284:13246–13254](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13246) conditions this specific retirement on N/N following across two corpora and a prediction that a miss restores the derivation. V2 preserves generic trim predictions but drops the eligibility threshold.

**Concrete failure:** A seat deletes a marked-case derivation immediately after its render ships, before observing whether the pilot reliably follows the mark.

### 14. MED — Dropped A222 loses bottoming-order validation

**V2:** [107–119](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:107) reject one favourable sample and define UNTESTED through missing opportunities, but do not address sparse evidence for a preference order.

**Original:** Dropped [A222:12347–12355](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12347) requires offers/ships/bottoms **with the order implied by the reply**, withholding a verdict until the sample can discriminate that order; it expressly says two or three samples cannot.

**Concrete failure:** A seat treats two correct bottom choices as validation of its entire bottoming order and retains an unsupported ranking.

### 15. MED — Guide opening has two incompatible prescriptions

**V2:** [51–55](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:51) present deck thesis → engine order → decision-kind rules, while [385–387, 510–511](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:385) mandate mulligan at the top.

**Original/contract:** [A69:9932–9943](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:9932) explicitly places the first ask **before the deck thesis**; the [rewrite brief:21–27](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/guide-rewrite-brief.md:21) prescribes the opposite shape. V2 reproduces both without a content-order exception.

**Concrete failure:** One careful seat places mulligan first, while another follows the advertised guide shape and buries the pregame stop below the thesis and engine.

## Remaining checks and verdict

Oracle retrieval, the replacement size ceiling, KEEP-is-success and evidence placement are explicit. Historical narration remains in the ruling and lineage tags, but no **new** leak defect is established: A332 restricts guides, not the authoring skill. Finding 12 is a resurrected superseded rule, not an invention without any historical source.

**Verdict: not safe to install.** The round-2 repairs hold, but the remaining policy and validation losses can materially change what seats write; reconcile these before making v2 the current edition.