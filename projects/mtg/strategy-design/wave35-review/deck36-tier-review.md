# deck36 (Master of Ether — UB Artifact Aggro) — Wave-35 A/B Strategy Review

Seat: deck36 · Guide: `Res/ai/baka/deck36_strategy.txt`
Arm A = qwen36-35b-a3b (matchups-20260820-192210, 6 games + 1 rerun in matchups-20260821-022400)
Arm B = qwen35 122B-A10B (matchups-20260821-033000, 6 games)
Rubric: owner's — win rate irrelevant; score guide-adherence per decision, and where the model diverges, was diverging correct AT THAT MOMENT.

Note: A's 192210 game vs deck146 has no `gameend` record (run truncated — this is the game
that got the 022400 rerun). Its decisions are still scored; the rerun is scored as its own game.

---

## Guide core lines (extraction)

1. Land every turn, artifact land over basic; artifacts cheapest-first ({0} Moxes/Ornithopter always); deploy in MP1, THEN attack.
2. Master of Etherium / Akroma's Memorial: cast the first turn listed.
3. Sacrifice ladder: Pest → tapped basic → other tapped land → Mox → untapped basic → Nuisance/Steel Wall; KEEP creatures, Master, Tolarian Academy, Academy Ruins, Memorial "to the very last". One card per ask ("the 6 is how many sacrifices... you will be asked again for each one").
4. Trust printed N ("Artifacts in play"); never recount.
5. Tolarian Academy: pass at upkeep and on opponent's turn; own MP1 only when casting something.
6. Attack everything except lines tagged `[deals 0 ...]`; tapped/sick defenders hold nothing home; at ≤12 life, hold blockers vs lethal, swing the rest.
7. Block EVERY infect attacker at any life, with the biggest body.
8. Academy Ruins recursion only on Master/Memorial. Scarecrone draw-sac only when behind. Nuisance Engine Pest in YOUR MP1.
9. Mulligan: keep any 7 with 2+ lands (read the engine's count).

---

## Arm A — 35B-A3B, per-game findings

### A1 vs deck158 (192210, loss T10)
Near-perfect execution: kept 3-land hand; Seat of the Synod over basics (seq2); Ornithopter/Moxes first (seq3-5); artifact lands every drop (seq7/11/15/17); `[deals 0]` Ornithopter left home in all 4 attack asks (seq9/13/16/18); Yotian fronted the biggest attacker (seq10).
- seq18 T9, attack at 2 life: adherent — trace correctly applies the guide's tapped-defenders rule ("Both are tapped, meaning they can't attack this turn"). The guide's own rule is this-turn-scoped; no model fault.

### A2 vs deck146 (192210, truncated — no gameend; rerun below)
Mostly adherent (artifact lands seq2/6, cheapest-first seq5/7, deals-0 exclusion seq15/19/23, Pest sac-ladder n/a). Two real divergences:
- **seq11 T5 (FAILURE, minor)**: declined the land drop — "I have 4 untapped lands/artifacts. I don't need another land" — misses that a land is a free +1 count (and Tree of Tales was offered). Recovered the drop later the same turn.
- **seq13 T5 (FAILURE)**: with the drop re-offered, chose **Swamp over Tree of Tales** — "Swamp is basic. Either works." Direct violation of the artifact-land rule; cost 1 artifact count. No board argument offered.
- seq17/21: Pest at upkeep instead of MP1 (minor, see pattern note below).

### A3 vs deck116 (192210, loss T13 to Emrakul)
Strong macro play: Academy Ruins land drop (seq10), Memorial cast first turn listed (seq19), Academy Ruins activation correctly passed at upkeep twice (seq17/21). Sacrifice gauntlet T11 (annihilator ×6): first five picks ladder-clean (tapped Islands, both Moxes, Seat of the Synod).
- **seq30 T11 (FAILURE)**: 6th sacrifice — chose **Academy Ruins over two untapped Great Furnaces**. Keep-list names Academy Ruins "to the very last"; Great Furnace is unprotected. Trace also fought the "Choose card 6 of exactly 6" phrasing and tried to emit SIX choice lines in one reply (post-answer overrun) despite the guide's explicit "you will be asked again for each one" (prompt-caused component, see docket).
- **seq31 T11 (FAILURE, rules misread)**: blockers vs Thragtusk 5/3 + Emrakul: trace asserts "Thragtusk... doesn't have flying, so only non-flying creatures can block it" (backwards — and the menu line said outright `B2. Cathodion — may block A1 (you kill it, your blocker lives)`). Quad-chumped Emrakul instead, gave up a free Thragtusk kill.
- seq23 T10 (FAILURE, minor): Academy Ruins recursion on **Scarecrone** — guide restricts it to Master/Memorial; spent the draw on the wrong card.
- seq15 T6 (divergent, defensible): Scarecrone draw-sac while not behind (20/14) — justified only by an empty castable hand; weak audible, tolerated.
- T13 gauntlet seq35-38: ladder-clean (tapped Furnaces/Vault, then Scarecrone before Memorial — consistent with keep-order).

### A4 vs deck105 infect (192210, loss by poison T12 at 20 life)
Infect discipline mostly excellent: seq12 Yotian blocks Ichorclaw ✓; seq19 both infect attackers blocked (Ornithopter absorb + Cathodion kills Cystbearer) ✓; seq25 lone blocker put on the 2-poison attacker ✓; Academy passed at upkeep/opp-turn in 9 of 10 windows; Master cast first-listed (seq34); Tolarian Academy land drop T3 ✓.
- **seq20 T6 (FAILURE, rules misread)**: tapped Tolarian Academy on the OPPONENT'S blockers phase — "free mana to use later this turn or next turn". Violates the guide's opponent's-turn rule AND the mana-emptying fact the guide states.
- **seq37 T12 (FAILURE, answer-flip)**: trace computes the right block ("blocking A2 (Ichor Rats) is good because it deals 2 poison... BLOCKS: B1:A2"), then the final-check re-emission silently flips to **B1:A1** (1 poison blocked instead of 2). Outcome-irrelevant (any single block still loses) but a clean specimen of the 35B's reasoning-to-answer divergence.

### A5 vs deck139 (192210, WIN T9)
Textbook: Mox before Glaze (seq4), Master cast T1-listed at (3/3) (seq5), cheapest-first (seq7/10), deals-0 exclusion (seq19).
- seq14/18 (JUSTIFIED AUDIBLES): "Cast nothing" with Nuisance/Myr payable while opponent at 1 life — lethal assured; correct at that moment.

### A6 vs deck152 (192210, loss T18, opp at 2)
High adherence across 34 decisions: artifact lands, Master first-listed (seq5), Yotian repeatedly fronted on the biggest attacker (seq14/19/24/27), deals-0 exclusion throughout, ≤12-life hold applied at seq33 (triple block, only trample through). No comprehension failures found.

### A7 vs deck146 rerun (022400, loss T16)
Sound macro (Ancient Den/Vault drops, cheapest-first, Pest sac pick seq14 = ladder rule 1, all-deals-0 attack lists correctly held home at seq13/17/22/31/36).
- **seq27 T11 (FAILURE)**: attacked with a `[deals 0]` Pest — trace planned "Attack with Cathodion and Pest", noticed Cathodion wasn't in the menu (summoning-sick), and shrugged "I'll just name it. It's fine." Engine took the Pest alone. Menu-contract misread.
- **seq28 T12 (FAILURE, rules misread)**: "Cathodion (3/3) can block A1 [1/1 first-strike deathtouch] safely. Cathodion MUST block A1" — against the explicit menu annotation "(your blocker dies, attacker lives)", while the same menu offered Cathodion kills-and-lives blocks on three other attackers. Lost the 3/3 to a 1/1.
- seq18 (minor): Ornithopter chump into deathtouch at 20 life — small value bleed, defensible.
- seq15/19/24/29/33: upkeep Pests (pattern note below).

## Arm B — 122B-A10B, per-game findings

### B1 vs deck158 (WIN T11)
Clean: 0-drops first, Silver Myr over Cathodion (cheapest-first, seq6), Memorial first-listed T9 (seq22), deals-0 exclusion in every attack ask (seq9/12/18/23/28).
- seq26/27 (JUSTIFIED AUDIBLES): declined Coiled Tinviper and Scarecrone reanimation with lethal assured on board (opp at 2) — correct at that moment.

### B2 vs deck146 (WIN T9)
Near-perfect 15-decision game: Seat over Academy Ruins T1, **Master of Etherium T1 off double Mox** (seq5), Academy Ruins as the T3 drop, no-block of Silver Myr into deathtouch (correct — blocker dies regardless), full attacks. Zero divergences.

### B3 vs deck152 (loss T10)
Adherent throughout: cheapest-first (seq5/7/8), Master first-listed (seq12), Pest in MP1 (seq13), deals-0-only attack lists correctly held home (seq19/25 — the seq25 hold also satisfies the ≤12 rule: 11 incoming vs 9 life). Loss was board-state (Master answered T6), not guide failure. Minor: seq16 upkeep Pest.

### B4 vs deck116 (WIN T10, 22→-24)
The reference game for Academy play: tapped Academy in own MP1 exactly when casting (seq8), **tapped Academy in MP2 to power Memorial the turn it was listed** (seq28→29), passed it at opp upkeep (seq16) and combat windows.
- seq17 (JUSTIFIED AUDIBLE): created a Pest on the OPPONENT'S MP1 — argued from engine-untap economics and explicitly rejected the Academy tap on the opponent's turn ("tapping Academy now is waste. Creating Pest is value"). Guide says MP1-of-your-turn; this instant-speed use adds a pre-combat blocker and count. Correct at that moment.
- seq25: attacked with 4 Master-pumped 1/2 Pests (no longer deals-0) — exactly the growing-bodies thesis.

### B5 vs deck105 infect (loss by poison T10 at 20 life, opp at 2)
Infect discipline exemplary: seq9 Cathodion (biggest body) blocks Blight Mamba, kills it, survives ✓; seq18 both infect attackers blocked with the two useful chumps ✓. Cheapest-first, artifact lands, deals-0 exclusion all clean. Loss = infect deck's clock, not decisions. (Engine labeled T1 cast asks "Main phase 2" — see docket.)

### B6 vs deck139 (WIN T17)
- **seq22/23 T13 (JUSTIFIED AUDIBLE, the arm's signature)**: declined to cast a second Akroma's Memorial with one already on the battlefield — "Casting Akroma's Memorial is redundant (one already on board)... lethal is assured." The guide says cast-first-turn-listed; the audible is argued from a real board fact (redundancy under the legend rule) and is correct. (It cast the copy at T15 when mana was idle; legend rule binned the old one — harmless, +1 graveyard artifact.)
- Memorial #1 cast first-listed T9 (seq16); Master first-listed (seq27); deals-0 Pests held home pre-Master, sent post-Master pump (seq18 vs seq29). Minor: upkeep Pest seq17.

---

## Divergence table

| # | Arm | Game/seq | Decision | Verdict | Why |
|---|-----|----------|----------|---------|-----|
| 1 | A | A2 seq11 | Declined MP1 land drop | FAILURE (minor) | "Don't need mana" — misses land=+1 count; recovered same turn |
| 2 | A | A2 seq13 | Swamp over Tree of Tales | FAILURE | Guide-explicit artifact-land rule; "Either works" |
| 3 | A | A3 seq15 | Scarecrone insta-draw-sac, not behind | Divergent-neutral | Weak audible: hand empty of castables |
| 4 | A | A3 seq23 | Academy Ruins recursion on Scarecrone | FAILURE (minor) | Guide: Master/Memorial only |
| 5 | A | A3 seq30 | Sac'd Academy Ruins over Great Furnace | FAILURE | Keep-list explicit; also multi-answer overrun (prompt-assisted) |
| 6 | A | A3 seq31 | Quad-chump Emrakul, Thragtusk unblocked | FAILURE (rules) | Invented blocking restriction; ignored "(you kill it, blocker lives)" |
| 7 | A | A4 seq20 | Tapped Academy on opp's blockers phase | FAILURE (rules) | "Mana for later/next turn" — mana empties; opp-turn rule |
| 8 | A | A4 seq37 | Blocked 1-poison over 2-poison attacker | FAILURE (flip) | Trace concluded B1:A2, emitted B1:A1 |
| 9 | A | A7 seq27 | Attacked with [deals 0] Pest | FAILURE | Hallucinated Cathodion into menu, "I'll just name it" |
| 10 | A | A7 seq28 | Cathodion blocks deathtouch 1/1 | FAILURE (rules) | "Can block safely" vs explicit "your blocker dies" annotation |
| 11 | A | A7 seq18 | Ornithopter chump into deathtouch @20 | Divergent-neutral | Small value bleed |
| 12 | A | A5 seq14/18 | Held casts with lethal near | AUDIBLE ✓ | Opp at 1 life |
| 13 | A+B | many | Nuisance Pest at upkeep, not MP1 | Divergent-neutral | Menu offers it there; net effect usually identical (docket #2) |
| 14 | B | B1 seq26/27 | Declined Tinviper + Scarecrone rez | AUDIBLE ✓ | Lethal assured, opp at 2 |
| 15 | B | B4 seq17 | Pest created on opp's MP1 | AUDIBLE ✓ | Engine-untap economics; refused Academy on opp turn |
| 16 | B | B6 seq22 | Declined 2nd Memorial (first-listed rule) | AUDIBLE ✓ | Legend-rule redundancy, lethal math stated |
| 17 | B | B6 seq24 | Cast the 2nd Memorial later anyway | Divergent-neutral | Harmless; mild inconsistency with its own T13 logic |

Arm A: 8 comprehension failures, 3 neutral, 2 audibles (~95 scored decisions; ≈86% guide-adherent).
Arm B: 0 comprehension failures, ~4 neutral, 5+ audibles (~100 scored decisions; ≈95% guide-adherent, and every hard divergence argued from true board facts).

---

## Prompt-caused defects (wave-36 docket; NOT model failures)

1. **"Choose card 3 of exactly 6" phrasing** (sacrifice gauntlets, A3 seq30): still parses as "pick 6 now" — even with the guide's dedicated paragraph, the 35B emitted six CHOICE lines in one reply (post_answer_overrun 174+). Reword to "sacrifice #3 (of 6 total, asked one at a time)".
2. **Nuisance Engine offered at Upkeep windows**: the menu invites the guide-divergent upkeep activation; both arms take it there routinely. Either suppress the upkeep offer for sorcery-ish engine activations or accept and amend the guide.
3. **T1 cast asks labeled "Main phase 2"** (B5 seq4/5): phase label contradicts the guide's "deploy in MP1" instruction on the turn it matters most for Glaze Fiend timing.
4. **Clue token card text renders as `"() source: Master of Etherium"`** (A6 seq15, B3 seq16/21): empty-parens text, no rules text for what a Clue does.
5. (Observation, not defect) The `[deals 0 ...]`, "(you kill it, your blocker lives)", tapped/sick tags, and the engine land/artifact counts were reliable in every decision reviewed — Arm A's combat failures were misreads OF correct annotations, never wrong annotations.

---

## Per-arm verdicts and the A/B answer

**Arm A (35B-A3B)**: Executes the deck's scripted loop with high fidelity — mulligans 7/7 by the engine count, 0-drops always taken, cheapest-first sequencing, artifact-land preference (1 lapse), Master/Memorial always cast first-listed, deals-0 exclusion near-perfect, infect blocking policy followed. Its failures concentrate in exactly the places the guide can't fully script: combat evaluation and multi-step sacrifice pressure. Three of its eight failures are rules-misreads *against explicit menu annotations* (Thragtusk block, deathtouch block, Academy mana persistence), and one is a trace-vs-answer flip. Divergence quality: poor — when it leaves the guide it is usually wrong.

**Arm B (122B-A10B)**: Follows the guide at least as tightly, and its divergences are the rubric's ideal: legitimate audibles argued from real, verifiable board facts (redundant legendary, lethal-assured holds, instant-speed token economics), several of which are *better* than the guide's letter. Zero comprehension failures across six games.

**A/B answer for seat deck36**: **Keep the 122B. The 35B is not yet sufficient to replace it on this seat.** The 35B's guide-following is genuinely close (≈86% vs ≈95%), but the rubric's second axis — divergence quality — separates them sharply: 8 comprehension-failure divergences vs 0. The 35B's specific weakness (ignoring correct combat annotations it was explicitly given, and finalization flips) costs real material in the decisive combats this aggro deck lives on. If a wave-36 prompt pass fixes docket #1/#2 and combat-annotation salience is reinforced (e.g., a guide line: "the parenthetical after 'may block' is the outcome — believe it"), the 35B is worth re-running on this seat.
