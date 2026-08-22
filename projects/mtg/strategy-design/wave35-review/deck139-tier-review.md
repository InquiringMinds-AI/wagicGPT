# deck139 "Mutants" (Ikoria mutate) — wave-35 A/B strategy review

Rubric (owner, binding): win rate is irrelevant; score (a) adherence to the guide's line for the
board state, (b) where the model diverges, whether departing was correct AT THAT MOMENT.

- Guide: `projects/mtg/bin/Res/ai/baka/deck139_strategy.txt`
- ARM A = qwen36-35b-a3b, run `matchups-20260820-192210` (6 games as deck139)
- ARM B = 122B-A10B, run `matchups-20260821-033000` (6 games as deck139)

Guide core lines used for scoring: #1 take every "with its mutate cost" line / always "mutate
over"; #2 under aggro a fresh body beats a mutate (say so); cast floor; shocklands default "tap"
(pay only for this-turn use, never ≤12 life except to deploy a blocker); mulligan two-counts rule;
Pollywog-first above 10 life / highest-toughness when ≤10 and behind; discard = spare land;
attack/block floors; poison priced against M, not life.

## Headline numbers

| Metric | ARM A (35B) | ARM B (122B) |
|---|---|---|
| Games (wins) | 6 (1) | 6 (2) |
| Scoreable decisions | 155 | 185 |
| Unanswered/defaulted decisions | 2 (1.3%) | 6 (3.2%) |
| Mutate-cost lines taken when offered | 6/7 (skip = guide's own Pollywog-first clause) | 11/16 (all 5 skips reasoned; see table) |
| "mutate over" step | 6/6 over | 9/11 (1 deliberate justified "under", 1 defaulted) |
| Merge step ("mutate" vs Cast Normally) | 1/1 | 3/3 |
| Mulligan asks correct | 6/6 | 8/8 |
| Shockland asks | 31 (28 tap, 2 bad pays, 1 defaulted) | 20 (14 tap, 5 pays — 4 justified, 1 bad — 1 defaulted) |
| Discard asks = land when land available | 2/3 | 2/2 |
| Median / max decision latency | 97 s / 264 s | 830 s / 1399 s |
| Divergence verdicts (audible / failure / prompt-caused) | 3 / 4 / 1 | 6 / 3 / 0 (+6 budget-defaults) |

Adherence of answered decisions: ARM A ≈ 97% guide-conformant, ARM B ≈ 95%. Statistically the
same; the texture differs — B's divergences are smarter, B's failure mode (budget overrun) is worse.

## Per-game findings

### ARM A (35B)

**vs deck105 "Infected Wounds" (loss, poison at 20 life).** Mulligan keep correct (seq1). Ramp
curve per guide (seq6 Grazer T2, seq8 Dryad, seq12/13 double Pollywog). Brokkos mutate line taken
with full flow (seq16-19: mutate cost -> discard Forest -> mutate over -> host Dryad) — textbook
#1-rule execution. Everquill mutate taken seq22-25. Combat is where the game was lost, and the
decisive misplay is **prompt-caused** (see defect P1): at seq15 (t7) the render promised Pollywog
"you kill it, your blocker lives" vs Ichorclaw Myr; the Myr's becomes-blocked +2/+2 (printed in
its own {text}) made that false — Pollywog died, and the guide's "read the parenthetical... and
believe it" commands trusting the wrong line. Earlier at seq10 (t5) the model *caught* the same
trap: it declined the render-endorsed Dryad block explicitly because "Blocking triggers the pump...
blocking is strictly worse" — a justified audible over a defective render. seq30 blocks
(BLOCKS: B1:A1, B2:A3, B3:A4) priced against the printed poison M per the guide and were
near-optimal chumps; the chosen_text "Arboreal Grazer... Arboreal Grazer..." is two distinct
copies, not an illegal double-assign. seq23 discard: pitched a spare Grazer over Stomping Ground
with a land available — letter-divergent, argued (kept fixing land + still held a second Grazer,
cast it that turn, seq28) — weak audible.

**vs deck158 (win).** seq6 (t1): paid 2 on Steam Vents with no same-turn use ("so I can use it for
Dryad and Pollywog" — future turns) — **comprehension failure** of the pay-only-for-this-turn rule,
2 life wasted. Rest clean: Nissa/Dryad value lines, Gemrazer merge flow correct.

**vs deck36 (loss).** Clean adherence while flooded/pressured; seq17 t7 at 1 life gang-chumped all
three attackers with three distinct Grazers (survived the turn) — block floor followed. Loss was
board-state, not guide breach.

**vs deck116 (win=False).** seq14 (t6): skipped Everquill's mutate line to cast Pollywog at 20
life, even board — this is the guide's own "Above 10 life... cast Pollywog first" clause →
adherent; it then took the (now discounted) Everquill mutate next turn (seq16-19). seq31 (t11,
Annihilator-4 sacrifice prompt): reply "CHOICE: 4 (Mountain)" against a 2-option list — the model
renumbered its own hallucinated list; parse failed, engine default fired. **Model reply-protocol
failure.**

**vs deck152 (loss).** seq5 (t2 pay/tap): reply "CHOICE: 2 (Sacred Foundry)" — number 2 (= tap,
guide-correct) but wrong short-name → parser rejected it, default fired (see defect P3). Otherwise
adherent; lost to a bigger board.

**vs deck146 (loss).** seq8 (t5, 17 life): paid 2 with no same-turn use, trace says "I will just
pick 1 to be safe" — **comprehension failure** (the guide's default is exactly the opposite).
Otherwise adherent under heavy pressure.

### ARM B (122B)

**vs deck105 (loss).** seq6 (t2): paid 2 on Overgrown Tomb claiming it could "tap it for {U}" for
Pollywog — Overgrown Tomb makes {B}/{G}; no cast followed. **Comprehension failure** (misread
mana). Rest of the short game adherent.

**vs deck146 (win, the marquee game).** seq27 (t15, 8 life, behind): hardcast Greathorn over its
own mutate line — guide #2 rule executed with the blocker rationale stated. seq30 (t19): hardcast
Gemrazer over two offered mutate lines, PLAN line: "Stabilize by hardcasting Gemrazer for an
additional blocker" — #2 rule again, said in so many words. seq32: **deliberate "mutate under"** —
casting Greathorn (3/4) onto Brokkos (6/6): the trace walks the guide's contradiction ("ALWAYS
answer mutate over" vs "Putting the Apex on top is how you get its P/T") and keeps the 6/6 on top
with the fetch trigger still firing — objectively correct MTG; **justified audible** exposing guide
defect G1. seq38 no-attack at 8 life into two deathtouchers + menace: no attack-floor condition
passes (nothing survives every block or trades up) — adherent, correctly argued from deathtouch.
seq53 (t31, 1 life): "Cast nothing" with castable Gemrazer, protecting the lethal Illuna+Snapdax
swing (opponent fully tapped; it computed 12 > 9) — letter breach of the cast floor, correct at
that moment; it won on the spot. Audible. Budget-default at seq10 (pay/tap, empty reply after 22.5k
chars of reasoning).

**vs deck36 (loss).** Adherent through a mana-starved game (Nethroi merge flow seq26-28 correct:
mutate -> over -> host Pollywog, graveyard takes taken). One budget-default (seq13 discard prompt,
no land in hand, harness default benign).

**vs deck158 (loss).** seq7 (t2): skipped Gemrazer's mutate line to cast Dryad on curve — trace
quotes both of the guide's "every time" imperatives and resolves to Dryad (ramp/fixing engine, no
Gemrazer target). **Justified audible**; also guide-conflict item G2. seq21 (t11): declined a free
"neither dies" Grazer block on Orcish Bowmasters — minor letter failure (1 damage, no downside
block refused). Rest adherent.

**vs deck152 (loss).** seq17 (t8): no-attack though Dryad kills Intrepid Adversary (3/1 lifelink)
and lives — attack floor says send; trace declines citing the 3 lifelink and its Nissa land-creature
plan. Divergent — thin audible (real facts, defensible, but the floor is against it). seq27 (t12,
9 life, Sigarda 5/5 flier untapping next turn): no-attack to keep both bodies home and set up the
reach blocker — justified audible (guide's own stop-dying posture). seq24 pay-2 at 11 life then
Beanstalk Giant deployed vs a live board — inside the guide's ≤12 exception, justified. Two
budget-defaults (seq3 pay/tap, seq33 Nethroi target prompt).

**vs deck116 (win).** seq11 (t5): Dryad on curve over the discounted Greathorn mutate — same
justified audible as B-deck158 seq7. seq17 (t7): declined the render-endorsed Greathorn block that
kills Kitchen Finks with blocker surviving, "to preserve the merged creature" — the render already
said it lives; minor overcaution **failure**. seq27 (t10): mutate over/under step lost to budget
overrun — the trace had *decided* "mutate over" (matching the guide: Gemrazer 4/4 onto the
Grazer/Dryad pile) but never emitted it; engine default fired. Two budget-defaults total.

## Divergence table

| # | Arm | Game/seq (turn) | Choice vs guide line | Verdict |
|---|-----|-----------------|----------------------|---------|
| 1 | A | deck105 seq10 (t5) | No block though render said "you kill it, your blocker lives" | AUDIBLE — saw Ichorclaw's +2/+2; render was wrong |
| 2 | A | deck105 seq15 (t7) | Pollywogs block (engine pieces; one died) | PROMPT-CAUSED — believed defective render line, as the guide instructs |
| 3 | A | deck105 seq23 (t10) | Discarded spare Grazer over a land | weak AUDIBLE (kept fixing; still held+cast the 2nd Grazer) |
| 4 | A | deck158 seq6 (t1) | Paid 2, no this-turn use | FAILURE (future-turns rationale) |
| 5 | A | deck146 seq8 (t5) | Paid 2 at 17, "pick 1 to be safe" | FAILURE |
| 6 | A | deck116 seq31 (t11) | CHOICE: 4 on a 2-option sacrifice list | FAILURE (reply protocol; defaulted) |
| 7 | A | deck152 seq5 (t2) | Right number, wrong short-name → rejected | FAILURE (reply protocol) + parser docket P3; intent was guide-correct "tap" |
| 8 | B | deck158 seq7 (t2) | Dryad on curve over Gemrazer mutate line | AUDIBLE (guide conflict G2) |
| 9 | B | deck116 seq11 (t5) | Dryad on curve over Greathorn mutate line | AUDIBLE (G2) |
| 10 | B | deck146 seq32 (t19) | "mutate under" with Brokkos host | AUDIBLE — keeps 6/6 top, trigger fires; guide defect G1 |
| 11 | B | deck146 seq53 (t31) | Cast nothing at lethal turn | AUDIBLE (protected the winning attack; won) |
| 12 | B | deck152 seq17 (t8) | No attack though Dryad kills the 3/1 lifelinker | thin AUDIBLE (floor says attack; reasoned from lifelink + plan) |
| 13 | B | deck152 seq27 (t12) | No attack at 9 life pre-Sigarda | AUDIBLE (defensive posture) |
| 14 | B | deck158 seq21 (t11) | Declined free "neither dies" block | FAILURE (minor, 1 dmg) |
| 15 | B | deck116 seq17 (t7) | Declined kill-block on Kitchen Finks | FAILURE (minor overcaution against the render) |
| 16 | B | deck105 seq6 (t2) | Paid 2 "for {U}" off Overgrown Tomb | FAILURE (misread mana; no use) |
| 17 | B | 6 decisions (146:10, 36:13, 152:3, 152:33, 116:22, 116:27) | Empty reply → engine default | BUDGET-DEFAULT (22-24k reasoning chars, 9-23 min; not strategy) |

Note: B-deck146 seq27/seq30 (hardcast over mutate at 8 life) score as ADHERENT — they are the
guide's #2 rule executed with the rationale stated, not divergences.

## Prompt/harness defects for the wave-36 docket

- **P1 (confirmed, game-affecting).** The blocker render's "naive 1-on-1 trade" line ignores
  becomes-blocked triggers: vs Ichorclaw Myr ("Whenever ... becomes blocked, it gets +2/+2") it
  printed "you kill it, your blocker lives (infect shrinks your blocker to 0/2)". False — the
  blocker dies (verified: A-deck105 seq15 block, Pollywog #1 dead in the seq16 log). The guide's
  "read the parenthetical ... and believe it" turns this render bug into forced misplays. Same
  wrong line appeared at A-deck105 seq10 and seq30. Fix: fold becomes-blocked triggers into the
  naive-trade math, or caveat the line when such a trigger is present.
- **P2 (B-arm reliability).** 6/185 B decisions produced an empty reply after 22-24k chars of
  reasoning (latency 559s-1399s) → choice=-1, engine default. In at least one (B-deck116 seq27)
  the trace had already decided the guide-correct answer ("mutate over") and lost it. A salvage
  pass (forced short answer on budget exhaustion) would recover most of these.
- **P3 (parser leniency).** A-deck152 seq5: "CHOICE: 2 (Sacred Foundry)" — correct option number,
  wrong short-name → rejected, default overrode a guide-correct "tap". Consider number-wins
  leniency (or explicit re-ask) when the number is unambiguous.
- **G1 (guide fix).** "#1 ... ALWAYS ANSWER 'mutate over' ... Never answer 'mutate under' with an
  Apex" is wrong when the CAST card is smaller than the host (Greathorn onto Brokkos): intent
  (Apex P/T on top) requires "under". Reword to "keep the biggest P/T on top: answer 'over' when
  the card you are casting is the bigger half, 'under' when your battlefield creature is."
- **G2 (guide conflict).** "Dryad ... cast her on curve every time" collides with "Gemrazer/
  Greathorn ... take their mutate line every time it is offered" when both are simultaneously
  legal (B-deck158 seq7, B-deck116 seq11). Both arms resolved to Dryad. State the priority
  explicitly (Dryad-on-curve outranks a cheap mutate before turn ~5).

## Verdicts

**ARM A (35B):** Follows the guide's letter tightly — mutate flow, mulligans, poison pricing,
Pollywog-first, #2-rule contexts all correct; ~97% of answered decisions conformant. Its 4
failures are small (two 2-life shockland leaks, two reply-protocol fumbles). Its one deep read
(seq10 catching the Ichorclaw trap over the defective render) shows real rules comprehension.
Divergence quality: adequate, occasionally shallow.

**ARM B (122B):** Same adherence rate on answered decisions (~95%) with visibly richer divergence
judgment — the mutate-under call, the articulated #2-rule hardcasts, the lethal-protection
cast-nothing. But it leaks decisions to reasoning-budget overruns (3.2% defaulted, one of them a
mutate placement), misread a land's colors once, and runs 8.5x slower per decision (median 830s vs
97s).

**A/B answer for this seat: the 35B is sufficient to replace the 122B on deck139.** Adherence is
statistically identical; the 35B's failures are no worse than the 122B's; the 122B's genuine edge
(divergence quality) mostly surfaced where the guide itself is defective (G1/G2 — fixable in text)
and is partly cancelled by its budget-overrun defaults, which the 35B never hits. Fix P1/G1/G2 and
the remaining gap on this seat is noise.
