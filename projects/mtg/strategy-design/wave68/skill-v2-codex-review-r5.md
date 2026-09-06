# Round 5 adversarial review

## Round-4 findings: verification first

1. **#16 — RESOLVED (formerly MED).** [V2:115–119](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:115) now requires choosing and disclosing the field appropriate to the claim, preserving `options_text` identity predicates while requiring rendered-prompt parsing for row literals. This matches [original A208:12111–12115](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12111), A240 and A327. The compulsory substitution of measurement fields is removed.

2. **#17 — RESOLVED (formerly MED).** [V2:121–125](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:121) explicitly compares windows **per seat-turn**, distinguishing exposure from observation clustering. This restores [original A217:12283–12287](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12283); unchanged totals can no longer conceal reduced opportunity density under the prescribed method.

3. **#18 — RESOLVED (formerly MED), with a new collateral contradiction below.** [V2:279–285](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:279) makes marker obedience conditional and explicitly gives rejection precedence when YOURS is longer. That preserves [original A319:13842–13849](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13842). Supplying arithmetic without its rejecting branch is no longer sufficient.

## New findings only

### 19. MED — Revision 5 invents a *sole* arithmetic exception, contradicting the retained route-around

**V2:** [199–202](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:199) says “sole exception: the trade-marker count below”; [284–285](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:284) repeats “the one named exception.” But [75–78](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:75) expressly permits arithmetic route-arounds over two true printed surfaces.

**Original:** [A287:13289–13298](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13289) specifically permits counting named land-backed cards and adding them to the header number. A319 introduces trade arithmetic without declaring that it revokes A287.

This is a new exclusivity claim introduced by the latest repair, not the old perception-classifier omission.

**Concrete failure:** A seat deletes an authorized temporary land-count correction because it requires counting hand entries and is not trade-marker arithmetic.

### 20. MED — A271’s impossible-survival branch is lost

**V2:** [438–448](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:438) requires first assigning enough blockers to bring remaining damage below life, “only then” ranking trades; it never supplies the branch where no legal assignment can achieve that threshold. Its unspecified `BLOCKS: none` comparison does not define this case.

**Original:** [A271:13049–13059](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13049) uses exactly that impossibility to justify declining a block and explicitly requires every ordering rule to say what happens when its threshold cannot be reached.

“Re-checking after each assignment” preserves repeated arithmetic, not the missing terminal branch.

**Concrete failure:** A seat writes an unconditional lethal-header instruction to spend blockers even where every assignment remains lethal, instead of preserving the source’s explicit no-saving-assignment branch.

### 21. MED — Whitespace normalization is specified for prompts, not both sides of the literal audit

**V2:** [151–157](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:151) qualifies rendered `prompt` as “whitespace-normalised” but separately tells seats to grep the guide for old-string sentences; [334–335](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:334) again describes the verification as a verbatim grep.

**Original:** [A294:13388–13403](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13388) requires normalization on **BOTH sides**, specifically because a wrapped obsolete quotation survived three guide-side greps. Its concrete instrument is whole-file whitespace normalization before counting.

Two careful seats can implement v2 differently: normalize the entire comparison, or normalize prompts while running ordinary line-based guide greps.

**Concrete failure:** A seat reports the obsolete-literal sweep clean while retaining a stale guide instruction whose quotation crosses a newline.

### 22. MED — Belief-correction retirement loses the required render check

**V2:** [164–169](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:164) defines belief-correcting retirement as “zero breaks across two corpora with the triggering shape,” without requiring the corrective fact to be rendered.

**Original:** [7340–7343](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:7340) explicitly requires **BOTH** the render check and the two-corpus behavioral check. The fuller [wave-25 explanation:2783](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:2783) calls rendering necessary but insufficient.

This is distinct from the previously repaired A284 marked-row derivation threshold: it concerns the general belief-correcting retirement class.

**Concrete failure:** A seat removes a successful corrective fact after two clean corpora even though the guide remains the only place supplying that correction.

## Sampling and remaining checks

Read-only; nothing written. I used the recall-working-memory skill for instruction-design context and Python to extract original amendment bodies. Samples from the notes’ historical dropped set included A117, A132, A167, A223, A230, A260, A263, A276, A296–300, A312 and A329. These are not uniformly non-rules: most contain operative review method now restored; A296’s gate-removal legality tests genuinely belong to engine work.

Oracle retrieval, the replacement size ceiling, KEEP-is-success and evidence placement remain explicit. I found no new direct contradiction with Amendment 332 or the rewrite brief. V2’s historical quotation and lineage tags are present, but A332 prohibits such material in **guides**, not authoring skills; those are not defensible leak findings.

## Verdict

**Not safe to install as the current skill edition yet.** The three round-4 repairs land, but the new exclusivity claim conflicts with a retained exception, and the distillation still loses an actionable combat branch and two verification constraints. These warrant bounded corrections, not another wholesale rewrite.