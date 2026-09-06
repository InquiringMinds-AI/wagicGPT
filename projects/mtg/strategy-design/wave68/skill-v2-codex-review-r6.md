# Round 6 adversarial review

## Round-5 findings: verification first

1. **#19 — RESOLVED (formerly MED).** [V2:198–202](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:198) now names both arithmetic exceptions; [280–285](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:280) no longer calls trade arithmetic the sole exception. This preserves the authorized two-true-surfaces workaround in [original A287:13289–13298](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13289).

2. **#20 — RESOLVED (formerly MED).** [V2:437–440](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:437) explicitly supplies the no-saving-assignment branch and requires ordering rules to state it. This restores [original A271:13049–13059](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13049), rather than merely requiring repeated arithmetic.

3. **#21 — RESOLVED (formerly MED).** [V2:149–155](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:149) expressly normalizes prompts **and the whole guide file**; [335–336](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:335) repeats the two-sided requirement. This matches [original A294:13388–13403](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13388); wrapped obsolete quotations no longer evade the prescribed audit.

4. **#22 — RESOLVED (formerly MED).** [V2:163–168](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:163) requires **both** the corrective render and zero breaks across two corpora containing the triggering shape. This restores [original:7339–7343](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:7339).

## New findings only

### 23. MED — Belief diagnosis is ambiguously narrowed to arithmetic

**V2:** [79–81](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:79) says to check for “a wrong BELIEF (**the same wrong arithmetic every time**).” The parenthesis reads as a definition, not one example of the class.

**Original:** [A218:12290–12300](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12290) concerns a non-arithmetic theory: “tapping out is dangerous.” Its remedy is to explain the deck-specific facts that invalidate that theory—almost no instants, no counterspell, defenders still block—**before** repeating the instruction. Even [A106:10537–10551](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:10537) includes vocabulary and card-category beliefs beyond arithmetic.

Two careful seats can therefore treat identical arithmetic as either an illustrative symptom or a required diagnostic condition. The latter interpretation loses A218 despite its citation.

**Concrete failure:** A seat encountering repeated tap-out refusals writes another imperative or escalates to render work instead of adding the deck-specific explanation that corrects the pilot’s mistaken theory.

### 24. MED — A155’s hidden-payment teaching rule is not retained

**V2:** [198–202](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:198) cites A155 under the generic rendered-input rule. Its closest payment instruction, [419–425](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:419), requires a payment-source checklist for **repeatable activations**, not all actions with hidden life costs.

**Original:** [A155:11339–11354](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11339) expressly requires hidden costs—pain lands, Talismans, phyrexian mana—to be taught at the string identifying **which source pays**, rather than keyed merely to a life threshold, with an explicit life-transition statement. That operational rule does not appear elsewhere in v2.

This is not preserved merely by requiring card-fact verification or numbers printed beside mana symbols: neither makes a cast procedure inspect its payment source.

**Concrete failure:** A seat writes a low-life cast restriction without identifying the life-paying source or showing the resulting life change, reproducing the Talisman gate that A155 says failed.

### 25. MED — “Not refuted” replaces A324’s positive evidence requirement for mechanisms

**V2:** [490–492](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:490) prohibits “a mechanism the render refutes,” citing A324. It does not require a proposed behavioral explanation to have been measured in the current corpus.

**Original:** [A324:13925–13939](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13925) distinguishes a correct instruction from its unsupported explanation and expressly says: state the reason **only if measured this wave; otherwise state the conclusion alone**.

V2 retains the negative check against contradictory evidence but drops the positive evidence gate. Its general per-edit counts do not necessarily measure the mechanism: a HOLD take count, for example, does not establish whether unchanged windows reopen.

**Concrete failure:** A seat keeps an unmeasured “HOLD prevents this identical window reopening” explanation because no inspected render expressly contradicts it, teaching an unsupported premise for future decisions.

## Sampling and remaining checks

Read-only; nothing written. I used the recall-working-memory skill for instruction-design context, Python to extract original amendment bodies, and `rg` to check corresponding v2 language.

Samples from the notes’ [historical dropped set:19–21](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/skill-revision-notes.md:19) included A117, A132, A167, A223, A230–231, A260, A262–263, A276, A295–300, A312 and A329. These are **not uniformly non-rules**: A223 directly requires a guide scope sentence after correct play from a new render; A117 and A132 prescribe review method. Revision 4 acknowledges their restoration. A296’s gate-removal legality testing is genuinely engine-lane work, not a missing guide-writing rule.

Oracle retrieval, the replacement size band/ceiling, KEEP-is-success and evidence placement remain explicit at v2:350–354, 59–61, 96–99 and 163–168. I found no new direct contradiction with Amendment 332 or the rewrite brief. V2 carries historical calibration and lineage narration, but [A332:14128–14146](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:14128) restricts **guides**, not authoring skills; that is not a defensible new leak violation.

## Verdict

**Not safe to install as the current skill edition yet.** All four round-5 repairs hold, but v2 still narrows a diagnostic, omits a specific payment-source teaching obligation, and weakens the evidence gate for explanatory mechanisms. These need bounded corrections, not another wholesale rewrite.