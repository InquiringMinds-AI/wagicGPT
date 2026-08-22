# deck116 (TIME OF NEED — Simic cheat-into-play) — wave-35 A/B strategy review

Rubric (owner, binding): win rate is irrelevant; score guide ADHERENCE and, where the model
diverges, DIVERGENCE QUALITY (justified audible vs comprehension failure).

- Guide: `projects/mtg/bin/Res/ai/baka/deck116_strategy.txt`
- Arm A = qwen36-35b-a3b, run `matchups-20260820-192210` (6 games)
- Arm B = qwen35 (122B-A10B), run `matchups-20260821-033000` (6 games, same opponents)

Guide core lines used for scoring: R1 never cast the five legends; R2 enabler first (and no
Fabricate with an Amulet already in hand); R3 take Put-in-Play, target biggest; R4 Piper never
attacks/blocks; R5 always make a play, ranked creature/enabler > search (Fabricate-if-no-enabler >
Explore > Urban Evolution > Time of Need > Farseek) > nothing; R6 mulligan by engine counts;
R7 search picks (Emrakul > Kozilek > Ulamog > Sheoldred > Jin); R8 trust the printed board.

Scoreable decisions (excl. gamestart/gameend): Arm A 126, Arm B 116. All 12 opening hands were
engine-counted 3 lands with a {G} source → all 12 "Keep" answers adherent (R6).

---

## Arm A (35B) per-game findings

### A vs deck36 — WON t13. Near-model game with one minor R5 violation.
Amulet t5 (seq7, R2) → cheat Thragtusk t7 (seq8) → ToN→Emrakul t9 (seq11/12, R7 rank-1 pick)
→ cheat Emrakul same turn (seq14) → ToN→Ulamog t11 (seq23/24) → cheat Ulamog (seq26) → lethal
annihilator swings (seq19, 29). Textbook execution of the whole guide loop.
- seq13, seq25 "Cast nothing" while holding Farseek/ToN: JUSTIFIED — trace shows it was holding
  exactly {4} for the Amulet activation it then took the same turn (seq14/seq26). Good audible
  against R5's letter, correct against its spirit.
- seq28 t13 "Cast nothing" with 3 mana and Farseek castable: minor R5 step-2 violation. Harmless
  (lethal already on board), but it is the exact named failure mode.
- seq20–22: PROMPT DEFECT (see docket below) — model coped correctly.

### A vs deck158 — LOST t10 (aggro clock). Adherent play, two minor R5 violations at the end.
Finks t5 (seq7, R5 step-1), Fabricate t7 with NO enabler in hand (seq10 — verified hand line:
ToN/Farseek/Fabricate/Explore/Island — this is Fabricate's sanctioned case), ToN→Emrakul t7
(seq12/13), Amulet t9 (seq15). Died on opponent's t10 from 6 (20-damage swing) — no line wins.
- seq16, seq17 t9 "Cast nothing" with 2 mana, Explore castable: R5 step-2 violations (minor;
  trace fixates on "Activate Amulet next turn" — there was no next turn, though Explore would not
  have changed that).

### A vs deck105 (infect) — LOST t12 to poison. Fully adherent; zero divergences.
Finks t5, Piper t7 (seq9), cheat Thragtusk t9 (seq11), cheat Finks t11 (seq15), guide-conform
blocks (seq8, 14, 19), Farseek at seq17 was the only cast offered (see suspected defect #2).
Trace at seq17 shows correct poison awareness ("staying alive at 8 poison counters"). Loss is
the matchup, not the play.

### A vs deck152 — LOST t12 (drew all monsters, no enabler until too late). Adherent.
Every cast turn the option list held only ToN or nothing (seq11, 16 — hand was 3–4 uncastable
legends; verified Piper was in hand but unaffordable at 3 mana when the model briefly suspected
an options omission, seq16 trace). ToN picks obeyed R7 given holdings (Sheoldred at seq17 —
Emrakul/Kozilek/Ulamog already in hand, Sheoldred > Jin). Piper over Thragtusk at seq19 (R2).
Loss = draw order; the deck's known failure case, played by the book.

### A vs deck139 — WON t13, but contains Arm A's clearest RULE violation.
Piper t5 (seq7) → cheat Ulamog t7 (seq9) → second Piper t7 (seq11, defensible: no other creature
in hand, R5 step-1) → Amulet t9 (seq13, cites R2) → cheat Emrakul t11 (seq19) → cheat Pelakka
t13 (seq24) → win.
- **seq14 t9 attackers: "Elvish Piper, Ulamog, Elvish Piper" — R4 VIOLATION, comprehension
  failure.** R4: "never declare Elvish Piper as an attacker." Trace never engages the rule:
  "Checking if there's any reason to hold back Pipers? No. Chumping is fine, and they deal 1
  damage each." Opponent had two 0/3 reach Grazers and a 4/4 flyer able to eat a Piper for free.
  It got away with it (won anyway) but this is the guide's named engine-suicide play.

### A vs deck146 — LOST t12 to a game-losing block blunder.
Adherent through t11: ToN→Emrakul t5 (seq7/8), Fabricate t7 with no enabler in hand (seq11),
Amulet t9 (seq14), cheat Emrakul t11 (seq15).
- **seq16 t12 blockers: "Emrakul blocks Triumphant Adventurer" — COMPREHENSION FAILURE, fatal.**
  Board: life 10 vs 12 incoming; prompt said "LETHAL if it all connects: block enough to
  survive." A1 Adventurer (2 dmg, first strike + DEATHTOUCH, line printed "your blocker dies,
  attacker lives"); A2 Nadaar 4/4 (line printed "you kill it, your blocker lives"). The guide's
  own situation row — big blocker + "you kill it, your blocker lives" → block — pointed at A2,
  which leaves 2+2+4=8 unblocked → survive at 2 with Emrakul alive. The model blocked A1,
  reasoning "A1 deals 2. Emrakul has 15. Survives... Unblocked: 10. Life: 10 -> 0. Loss. So I
  lose anyway. Blocking A1 is fine." Two misreads in one: ignored the printed deathtouch outcome
  (Emrakul died), and "I lose anyway" is false arithmetic (B1:A2 survives). Emrakul died AND the
  game was lost on the spot.

## Arm B (122B) per-game findings

### B vs deck152 — WON t15. Fully adherent; zero divergences.
Finks t5/t7 (seq7/9), Fabricate t9 with no enabler in hand (seq15), Amulet t11 (seq18), held {4}
and cheated Sheoldred same turn (seq19/20 — Sheoldred was the only legend in hand, so "biggest in
hand" is satisfied), Thragtusk recast from exile t13 (seq23 — correctly tracked Elite Spellbinder's
exile from the log), lethal team swing t15. Clean tracking, clean sequencing.

### B vs deck36 — LOST t10 to a 46-damage combo swing. Fully adherent; zero divergences.
Finks t5, Thragtusk t7 (R5 step-1), ToN→Emrakul + Farseek both cast t9 (seq13/15 — Pelakka Wurm
7-mana unaffordable at 6; ToN then Farseek is the correct step-2 ranking of what remained; trace
explicitly re-planned when it verified Pelakka uncastable). Died from 22. No line offered escape.

### B vs deck105 (infect) — LOST t8 to poison. Adherent; near-zero decision surface.
Finks t5, block Blight Mamba (seq7), Explore+ToN→Emrakul t7 (seq9-11). Opponent's infect curve
ended it on t8. Nothing the guide asks for was missed.

### B vs deck146 — WON t15. Adherent with two GOOD audibles.
- seq8 t7: Farseek chosen over Explore/ToN (R5 step-2 ranking puts Explore first) — **JUSTIFIED
  AUDIBLE**: hand held Amulet + no land; Farseek guarantees the 4th land → guaranteed Amulet next
  turn, Explore only draws at random. Trace states exactly this plan; Amulet landed t9 (seq11)
  and the plan won the game. Divergence better than the guide's static ranking.
- Cheat Emrakul t11 (seq13), Kozilek t13 (seq15), guide-conform Kozilek block (seq19).
- seq20 t15 priority: declined "Put in Play targeting Elvish Piper" (only creature in hand) —
  justified trivial audible ("Piper doesn't attack. It just sits there"); took the lethal swing
  instead (seq22).

### B vs deck158 — WON t11 from 3 life. Adherent; one harmless R5 violation.
Amulet t5 (seq7 — earliest enabler of either arm's set), Finks + ToN→Emrakul t7 (seq9-11),
cheat Emrakul t9 at 3 life (seq13), ToN→Kozilek (seq15/16, R7: Emrakul already in hand).
- seq19 t11 "Cast nothing" with 7 mana and Farseek/ToN castable: minor R5 step-2 violation,
  harmless — Emrakul lethal was on board and taken (seq21).

### B vs deck139 — LOST t14 after opponent destroyed the Amulet. One JUSTIFIED audible.
Finks t5/t7, Amulet t9 (seq13, trace explicitly cites "Passing is bad (Rule #5)"). Opponent's
Gemrazer line destroyed the Amulet ("Your Quicksilver Amulet was put into your graveyard" — the
model caught this in the seq15 trace and re-planned, exemplary R8 board-trust). seq15 "no
blockers" at 18 vs 8 incoming with only a 2/1: correct per the prompt's own "NOT lethal" note.
- seq16 t13 at 1 life: cast Kitchen Finks over the second Amulet (guide table: enabler first) —
  **JUSTIFIED AUDIBLE**: an Amulet cannot activate until two turns out; a 3/2 body + 2 life ETB
  was the only survival-relevant play. Trace argues exactly that. Died anyway t14; the Amulet
  line dies identically.

---

## Divergence table

| Arm | Game | seq/turn | Decision | Guide line | Verdict |
|---|---|---|---|---|---|
| A | vs36 | 13/t9, 25/t11 | Cast nothing, holding {4} | R5 step 2 | JUSTIFIED — spent same turn on Amulet activation (seq14/26) |
| A | vs36 | 28/t13 | Cast nothing (Farseek up) | R5 step 2 | Violation, minor/harmless (lethal on board) |
| A | vs158 | 16,17/t9 | Cast nothing (Explore up) | R5 step 2 | Violation ×2, minor (no winning line existed) |
| A | vs139 | 14/t9 | Attacked with BOTH Elvish Pipers | R4 | **COMPREHENSION FAILURE** — rule never engaged: "Chumping is fine" |
| A | vs146 | 16/t12 | Emrakul blocks deathtouch A1, not A2 | R8 + block row | **COMPREHENSION FAILURE, game-losing** — ignored printed "your blocker dies", false "I lose anyway" math |
| B | vs146 | 8/t7 | Farseek over Explore | R5 step-2 rank | **JUSTIFIED AUDIBLE** — no land in hand; guarantees Amulet mana; won |
| B | vs146 | 20/t15 | Declined Put-in-Play (Piper only target) | R3 "take it" | Justified trivial audible — Piper adds nothing; lethal taken |
| B | vs139 | 16/t13 | Finks over 2nd Amulet at 1 life | Situation table | **JUSTIFIED AUDIBLE** — only survival-relevant play |
| B | vs158 | 19/t11 | Cast nothing (Farseek/ToN up) | R5 step 2 | Violation, minor/harmless (lethal on board) |

Adherence (scoreable decisions, adherent-or-justified): Arm A 121/126 ≈ 96% (2 comprehension
failures, 3 minor R5 violations). Arm B 115/116 ≈ 99% (0 comprehension failures, 1 minor R5
violation). Audibles: A 2 (both mana-holding, correct); B 3 (2 substantive, both correct and
game-relevant).

## Prompt-caused defects (wave-36 docket) — not model failures

1. **Damage-assignment-order render defect** (A vs36, seq20–22, t11): every blocker option line
   carries an identical, wrong ability annotation — "Ornithopter (0/2) [trample, flying, first
   strike, haste, vigilance, protection from red, protection from black]" (and the same list on
   Cathodion / Vedalken Archmage). Ornithopter's real keyword is flying only; the list looks like
   a foreign creature's (or concatenated) ability dump. Model chose a sane order anyway.
2. **Same prompts lack a reply-format contract**: the seq20 trace shows the model guessing
   between BLOCKS: and CHOICE: syntax for a "choose the blocker dealt damage in position 1" ask.
   It guessed right; a dedicated format line would remove the risk.
3. **Suspected castability omission** (A vs105, seq17, t11): "Mana available: 4" with hand
   Kitchen Finks {1}{g/w}{g/w}, yet the cast list offered only Farseek. If ≥2 G-capable sources
   were untapped, Finks should have been offered — needs engine-side verification (hybrid-mana
   castability with Glimmerpost/Tropical mix). Low confidence; may be correct if only one green
   source was untapped.
4. (Legibility, minor) B vs139: Amulet destruction surfaced only as the bare log line "Your
   Quicksilver Amulet was put into your graveyard" with no cause. The 122B coped (re-planned
   correctly); a weaker model may not.

## Verdicts

**Arm A (35B)**: Executes the deck's MACRO loop very well — enabler-first sequencing, correct
Fabricate gating (both casts verified no-enabler-in-hand), correct ToN picks, correct
Put-in-Play usage, correct mulligans. Its failures are concentrated at the COMBAT seam: one
direct Rule-4 violation (Pipers attacking, rule never surfaced in the trace) and one
game-losing block misread (deathtouch ignored + wrong survival arithmetic despite the prompt
printing both answers). It also shows a mild "cast nothing" residue (3 minor R5 violations).

**Arm B (122B)**: Followed the guide essentially perfectly across all six games, and — the
rubric's second axis — its divergences were the best decisions in the whole sample: the
Farseek-for-guaranteed-Amulet line and the Finks-at-1-life survival call are exactly what a
justified audible looks like, each argued from true board facts in the trace. It also uniquely
demonstrated R8 board-trust under adversity (tracking exile and Amulet destruction from logs).

**A/B answer for this seat**: NOT sufficient to swap. The 35B is a competent guide-follower and
would be acceptable on macro play alone, but both of its comprehension failures are in combat
decisions the guide explicitly legislates (R4; the block row), one of them game-losing, and its
traces show the rules simply not being consulted at those seams. The 122B's margin here is not
win rate — it is zero comprehension failures plus positive-value audibles. Keep the 122B on
deck116; if the 35B is retried after wave-36, gate it on the two combat-seam cases above.
