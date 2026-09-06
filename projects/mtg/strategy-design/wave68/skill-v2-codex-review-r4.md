# Round 4 adversarial review

## Round-3 findings: verification first

1. **#12 — RESOLVED (formerly HIGH): fixed-look mulligan policy.**  
   [V2:384–399](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:384) now explicitly rejects fixed floors and stop-looks, prices each look, requires land/colour-source counts, and specifies a bottoming order satisfiable at every N. This restores [original A119:10755–10761](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:10755) and [A153:11316–11318](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:11316). The unconditional chain-stop failure identified in round 3 is removed.

2. **#13 — RESOLVED (formerly MED): premature derivation retirement.**  
   [V2:180–187](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:180) and [278–280](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:278) require two corpora of N/N marked-row following and the restoration prediction before retiring the derivation, matching [original A284:13246–13254](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13246). Shipping a render alone no longer licenses deletion.

3. **#14 — RESOLVED (formerly MED): bottoming-order validation.**  
   [V2:396–399](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:396) restores offers/ships/bottoms, reply-implied order, and withholding judgment on two or three samples, preserving [original A222:12347–12355](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12347). Two correct bottoms no longer validate the entire ranking.

4. **#15 — RESOLVED (formerly MED): incompatible opening orders.**  
   [V2:49–58](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:49), line 384 and lines 499–500 consistently place mulligan first **within the rules section**. This explicitly reconciles [original A69:9941–9943](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:9941) with the later [brief:21–27](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/guide-rewrite-brief.md:21), rather than claiming both placements simultaneously.

## New findings only

### 16. MED — A row-literal audit restriction becomes an invented ban on other evidence fields

**V2:** [116–120](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:116): “Every N of M names its FIELD (**rendered `prompt`, never `options_text`**).”

**Original:** [A208:12111–12115](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12111) explicitly distinguishes valid measurements of `options_text`, `prompt` and `chosen_text`. [A240:12649–12652](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12649) requires rendered-prompt parsing specifically for **literals inside option rows**, not every count. Later [A327:13985–13989](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13985) still gives a byte-identical `options_text` repeat predicate as a valid prediction.

The merger changes “choose and disclose the field appropriate to the claim” into “always use one field,” contradicting the restored prediction-predicate discipline.

**Concrete failure:** A seat replaces an existing `options_text`-identity denominator with rendered-prompt identity, then writes a HOLD-rule revision from a changed measurement rather than changed behavior.

### 17. MED — Restored A217 loses the normalization that detects changed opportunities

**V2:** [120–123](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:120) requires reporting a large change in window counts, counts by kind, and offers by phase, but never requires comparing **windows per seat-turn**.

**Original:** [A217:12276–12287](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:12276) distinguishes lost opportunities from shorter games and explicitly commands comparing this corpus’s windows **PER SEAT-TURN** against the previous corpus.

The distinct `(game, turn)` count at v2:105 measures clustering of observations; it does not supply total seat-turn exposure, including turns with no opportunity.

**Concrete failure:** With unchanged window totals but twice as many seat-turns, a seat misses the halving of opportunities and writes stronger draw-go instructions instead of identifying the missing-window defect.

### 18. MED — Trade-marker arithmetic lacks its source’s explicit authority to reject the mark

**V2:** [278–280](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:278) mandates opening with “answer the mark,” then says only “where a marker prices a TRADE, supply the arithmetic.” Meanwhile [195–197](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill-v2.md:195) forbids derived counts and instructions to count harder.

**Original:** [A319:13842–13849](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:13842) makes obedience conditional on the marker’s definition matching the deck’s goal. It explicitly requires counting `THEIRS:` and `YOURS:` and rejecting the marker when YOURS is longer, pending the render fix.

“Supply the arithmetic” does not preserve either the rejection criterion or its precedence over “answer the mark.” One careful seat can supply explanatory arithmetic after mandatory obedience; another can treat that arithmetic as a veto, notwithstanding the derived-count prohibition.

**Concrete failure:** A seat writes “answer the marked row” followed by a trade calculation without a rejecting branch, allowing the pilot to sacrifice five friendly bodies to kill one opposing creature—the source amendment’s exact failure.

## Sampling and remaining checks

Read-only review; nothing written. I used recall-working-memory for instruction-design context and Python to extract full original amendment bodies, sampling the notes’ historical dropped set, including A117, A132, A167, A208, A217, A222, A223, A230–231, A240, A260, A262–263, A276, A295–300, A312, A327 and A329. Revision 4 correctly recognizes that these were not uniformly non-rules; findings 16–17 concern inaccurate or incomplete restoration. A296’s gate-removal testing requirement is genuinely engine-lane work.

Oracle retrieval, the replacement size ceiling, KEEP-is-success and evidence placement remain explicit at v2:344–348, 60–62, 98–101 and 160–165. I found no new direct conflict with A332 or the rewrite brief. Historical ruling text and lineage tags in the **skill** are not prohibited **guide** leakage under [A332:14128–14146](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave68/strategy-writing-skill.md:14128).

## Verdict

**Not safe to install as the current skill edition yet.** All four round-3 repairs hold, but the revised text still changes evidence semantics, drops exposure normalization, and leaves a consequential marker-following exception ambiguous. These are bounded corrections, not grounds for another wholesale rewrite.