# deck105 (Infected Wounds — BG Infect) — FP8 Quant A/B Review

**Question**: The wave-35 4.75-bit arm (qwen36-35b-a3b, PrismaQuant 4.75bit) showed 2 comprehension failures. Same model tier, official FP8 quant, same seat: do those failures recur?

- **FP8 arm**: `matchups-20260822-111102/*ai_baka_deck105*` — 6 games, **134 scoreable decisions**, 3W/3L (W vs 116 t12, 36 t14, 139 t13; L vs 146, 158, 152). Model string `qwen36-35b-a3b`.
- **Rubric**: owner's verbatim ruling — win rate irrelevant; score guide adherence, and on divergence whether departing was correct at that moment.
- Prior-arm numbers (deck105-tier-review.md): 4.75-bit 128/132 (~97%), 2 comprehension failures + 2 justified audibles; 122B 118/118, zero divergences.

## Hunt result — the two prior failure classes

**Class 1: printed-outcome-line trust at blocks (Ichorclaw "pumps while blocking"). RECURRED — at the belief level, decision uncorrupted.**
vs deck152 **seq26 t14** (LETHAL, life 2 vs 6 incoming): model assigned `B1(Ichorclaw):A1(Wolf 3/3), B2(Mamba):A2`. Trace: *"Block A1 with B1 -> B1 becomes 3/3, kills A1, dies to counter … 'becomes blocked' triggers in the declare blockers step"* — while the printed option line said verbatim **"may block A1 (your blocker dies, attacker lives (infect shrinks it to 2/2))"**. Identical misread to the 4.75-bit's A-152 seq9: Ichorclaw's trigger claimed for the blocking role, printed outcome line overridden. Difference this time: the block itself was **mandated** (LETHAL → block enough to survive), and every legal assignment yields the same damage to face (unblocked A3 deals 1; survived at 1), so the wrong belief flipped nothing. Verdict: **the rules-prior is still present under FP8; this run it had no decision surface to corrupt.**

**Class 2: generic-mana payability re-derivation ("{G}{B} can't pay {2}"). NOT RECURRED — 0/134.**
No decision anywhere re-derives payability against the engine's verified cast list. All 8 "Cast nothing" declines had explicit non-mana reasons:
- vs 152 seq23/25/29/31 — Relic Putrescence with the "only legal targets are YOUR OWN" annotation: guide says decline; **4/4 correct** (positive control holds, as it did 5/5 in the prior wave).
- vs 116 seq25 t12 — declined Blight Mamba/Corpse Cur with lethal alpha on board: *"Poison 6/10. Need 4. Attackers 4+1+1=6. Wins."* Won that turn. **Justified.**
- vs 139 seq17 t11 — declined pre-combat at opponent 9 poison ("I win now"), then still cast Tel-Jilad Fallen post-combat the same turn (seq19), wasting nothing. **Justified** (ordering-only deviation from step 2/3).

## Full-walk findings (all 6 games)

**Block rule — 15/15 exact.** Every LIFE-AFTER ≥14 answered "none" (116 seq16@17, seq21@14; 146 seq5@17, seq8@14 — single static 3/2, growth check moot; 36 seq5@19, seq10@17, seq15@14; 139 seq13@16, seq20@4-with-race-won; 152 seq5@17). Every LETHAL/≤13 ask blocked: 146 seq13 (LETHAL@0 → Hand blocks, survived at 3); 152 seq19 (LETHAL → Cystbearer blocks Brutal Cathar), seq26, seq32 (LETHAL@-14, blocked the 7/7 with Cystbearer; survival arithmetically impossible with 2 bodies vs 15 — no failure); and the standout **158 seq11 t8**: LIFE AFTER 1 vs the counters-grown 8/8 Orc army — trace: *"'NOT lethal…taking damage while ahead' — This is just the system prompt's assessment. I will follow the strategy guide's explicit rule: 'YOUR LIFE AFTER is 1 -> BLOCK'."* The generic-hint prompt seam that licensed the 4.75-bit's overrides (wave-35 docket item 1) was **explicitly recognized and refused** here. Skithiryx/Putrefax never named as blockers.

**Standing loop.** Land every turn offered; biggest-cast taken (incl. Putrefax cast+attack same turn, 139 seq15–16 t9, per guide); attacks full-list 18/19 — the one partial is vs 152 **seq30 t15**: attacked Plague Stinger only, held Cystbearer at 1 life vs a wolf board (*"Cystbearer attacking would kill me"*). Against "attack with every creature" but survival-correct on the trace's math: **JUSTIFIED AUDIBLE**. (It survived that turn; died t16 to a 15-power alpha no play prevents.)

**Card rules.** Necropede death-counter at opponent's every time (139 seq8–9 Dryad; 36 seq25–26 Salvage Slasher; 152 seq9–10 Wolf). Corpse Cur return taken (158 seq9–10, biggest offered). Ichor Rats cast despite self-poison. Blight Mamba regeneration never mis-timed (no combat needed it).

**Poison lethality.** Read off the printed line every time it mattered: 116 seq25 "6/10, need 4, attackers 6 — wins" (won t12 with opponent at **28 life** — kept attacking through 8 points of lifegain, never reconciled poison vs life); 139 seq17 "opponent has 9 poison"; 36 alpha t14. No hand-tallying observed.

**Mulligans 6/6 guide-legal** — every keep had 2–4 lands (engine-counted sources) + creatures + a "would cover the cost of" line naming spells.

**Harness health**: no `answer_replaced`/`commit_retracted`/`dropped_assignments`; `reasoning_degenerate` scores all ≤0.011. Stylistic note: FP8 traces show heavy end-of-trace rumination loops ("Done. Proceeds. Output matches." repeated 5–10×) — cost noise, zero decision impact.

## Divergence table

| Game | seq/turn | Guide line | Choice | Verdict |
|---|---|---|---|---|
| vs152 | seq26 t14 | "Ichorclaw gets nothing for blocking"; printed "your blocker dies, attacker lives" | LETHAL block taken (correct), but trace claims blocking Ichorclaw becomes 3/3 and kills the Wolf | **COMPREHENSION MISREAD, decision-neutral** — Class-1 recurrence |
| vs152 | seq30 t15 | "Attack with every creature listed" | Held Cystbearer home at 1 life | **JUSTIFIED AUDIBLE** |
| vs116 | seq25 t12 | Step 2 "cast the biggest…" | Cast nothing, lethal alpha on board | **JUSTIFIED** (won that turn) |
| vs139 | seq17 t11 | Step 2 ordering (cast before combat) | Declined pre-combat, cast same creature post-combat same turn | **JUSTIFIED** (nothing lost) |

## Scores and verdict

**FP8 adherence: 133/134 letter-adherent (~99%), 0 decision-corrupting failures, 3 justified audibles, 1 decision-neutral comprehension misread.** vs 4.75-bit 128/132 (~97%, 2 decision-corrupting failures) and 122B 118/118 (100%, zero misreads even at belief level).

**Quant vs tier:**
- **Class 2 (mana recount): consistent with quant damage.** 0/134 under FP8 vs 1 occurrence in the 4.75-bit arm. Power caveat: a single-occurrence class over ~130 decisions per arm cannot statistically separate "quant-caused" from variance — but categorically, FP8 produced zero and the 122B produced zero, so the 4.75-bit arm is the odd one out.
- **Class 1 (Ichorclaw blocking-pump): tier-intrinsic.** The identical wrong belief, overriding identical printed text, appeared in both 35B quants and never in the 122B. FP8 did not remove it; board context merely denied it a decision to corrupt. Expect it to resurface whenever a non-mandatory block features Ichorclaw.

**Seat verdict**: FP8 closes most of the gap to the 122B — decision quality this wave is outcome-indistinguishable (0 harmful divergences vs 0), and it beat the 4.75-bit arm on the exact prompt-seam (generic blockers hint) flagged in the wave-35 docket. The residual risk is the tier's Ichorclaw rules-prior, which is prompt-resistant at the guide level but was already contradicted by the printed option line — the wave-36 docket's annotate-at-the-option hardening ("as a BLOCKER it stays 1/1" on Ichorclaw's B-line) is the targeted fix. On this evidence: prefer FP8 over 4.75-bit for this seat unconditionally; FP8 vs 122B is acceptable for this seat pending that one hardening.

**New defects**: none decision-level. Note the rumination-loop trace style (cost, not correctness) and that vs deck146 the seat lost 7 life across t7–t9 with no blockers ask offered (bodies tapped from attacking — the guide's own all-in risk profile, same as the 122B's loss to 146 last wave).
