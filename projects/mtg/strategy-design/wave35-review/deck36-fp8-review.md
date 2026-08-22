# deck36 (Master of Ether — UB Artifact Aggro) — FP8 Quant A/B Review

Seat: deck36 · Guide: `Res/ai/baka/deck36_strategy.txt`
Arm: qwen36-35b-a3b, **official FP8 quant** (matchups-20260822-111102, 6 games, 174 decisions)
Baselines: 4.75-bit PrismaQuant arm (8 comprehension failures, ≈86% adherent) and 122B arm
(0 failures, ≈95%) from `deck36-tier-review.md`.
Rubric: owner's — win rate irrelevant; guide-adherence per decision, and where the model
diverges, was diverging correct at that moment.

Question: do the 4.75-bit arm's five failure classes recur under FP8, or were they quant damage?

---

## Per-game walk (FP8 arm)

### F1 vs deck158 (WIN T9, 19 decisions)
Textbook. Kept 3-land hand by engine count (s1); artifact land T1 (Vault, s2); {0} Moxes
first, then Glaze Fiend, then Steel Wall — cheapest-first exactly (s3–s6); Master cast the
first turn listed (s12); full attacks every combat (s13/s17/s19).
- s10 T4 (adherent no-block): menu printed "your blocker dies, attacker lives" for BOTH
  Steel Wall and Cathodion vs the 4/4 Orc army; header said NOT lethal. Trace read the
  annotations correctly and held. This is annotation TRUST, the opposite of the 4.75-bit
  arm's signature misreads.

### F2 vs deck116 Emrakul/annihilator (loss T15, 41 decisions)
The stress game — same matchup that produced 3 of the 4.75-bit arm's worst failures.
- Sacrifice gauntlets T13 (annihilator 4) and T15: **keep-list HELD**. No creature, no
  Academy Ruins, no Nuisance Engine was ever picked; every intended pick was Pest-first
  (ladder rule 1). Prior class 3 (Academy Ruins sacrificed over Great Furnaces) did NOT recur.
- **s29 T13 (prompt-caused overrun, material)**: reply emitted `CHOICE: Pest #1, Pest #2,
  Pest #3, Pest #4` — four names, no number, one line. Engine fell through to **Vault of
  Whispers**, an off-ladder pick the model never chose. The intent was ladder-correct; the
  one-line-per-ask contract broke it. Docket #1 phrasing ("Choose card 3 of exactly 6")
  again.
- **s39 T15 (overrun again)**: three CHOICE lines in one reply; engine took the third (Seat
  of the Synod, tapped — legal ladder rung 3, but the model's first pick was Pest #3).
  s40 overran once more (comma list) but the engine parsed the intended Pest. 3 overrun
  events total; the 122B produced zero in the prior wave.
- **s28 T12 (FAILURE, minor — class 5 RECURS)**: Academy Ruins recursion targeting
  **Cathodion**. Guide: "use it only on Master of Etherium or Akroma's Memorial." The trace
  plans lethal math around the topdecked Cathodion but never engages the restriction.
  Same defect, same card-class (utility body), as the 4.75-bit arm's Scarecrone recursion.
- s23 T11 (defensible no-block): let Jin-Gitaxias (5/4) through at 20 life. Guide letter
  says front Steel Wall every turn, but its "they survive it" premise is false vs 5 power;
  trace argued blocker-dies-for-no-gain from the printed outcome. Divergent-neutral.
- s33 T13 (adherent): Steel Wall chumped Kozilek 12/12 (the biggest attacker), math in
  trace correct, outcome lines believed. **No invented blocking restrictions** — the exact
  spot where the 4.75-bit arm hallucinated backwards flying rules vs Emrakul. Class 1's
  worst form did not recur.
- s36 T14 (adherent): both `[deals 0]` Pests left home, Cathodion deliberately held as a
  blocker at 10 life (≤12 rule). The 4.75-bit arm attacked with a deals-0 Pest in its rerun.

### F3 vs deck105 infect (loss by poison T14 at 20 life, 26 decisions)
Infect discipline clean within its means: s9/s13/s21/s26 blocked an infect attacker in
every blockers window with the only available body; s13 header/annotations read correctly.
Memorial cast first-listed T9 ahead of everything (s19). Loss = the infect clock vs a
mono-Slasher board, same as both prior arms' losses here. No Academy misuse possible
(never drawn). No failures found.

### F4 vs deck152 (loss T16, 32 decisions)
- **s19 T8 (FAILURE, rules/annotation misread — class-1 family)**: menu printed
  `B1. Yotian Soldier — may block A1 (you kill it, your blocker lives)` (Luminarch
  Aspirant). Trace: "If I block, I take 4 damage and lose 2 creatures. Blocking gains
  nothing." Both halves false against the printed line — a free kill of the Aspirant was
  declined and no creature would have died. The unkilled Aspirant kept feeding the Wolf,
  which grew to 10/10 and took the game. This is the FP8 arm's one outright misread OF a
  correct annotation.
- s29 T13 (adherent): attacked 3 at 2 life — ≤12 rule computed correctly (only untapped
  opp creature was a 1/1; 1 < 2 ⇒ send everything).
- s30 T14 (adherent, good math): lone untapped Myr chumped the 10/10 (not the 1/1),
  surviving at 1 — the only line that survives.
- Cheapest-first, artifact-lands-over-basics (s2/s6 picked option 2 over a basic at
  option 1), Scarecrone draw-sac taken with a castable-empty hand — consistent throughout.

### F5 vs deck146 (WIN T17, 32 decisions)
- s9 T3 (adherent): `ATTACK: none` — the only offered attacker was the `[deals 0]`
  Ornithopter; correctly left home in ALL nine attack asks this game.
- s31 T17 (JUSTIFIED AUDIBLE): held the land drop with lethal on board (opp at 3, exactly
  3 on the table) — argued from assured lethal; the swing won that turn. Contrast the
  4.75-bit arm's "don't need mana" land decline, which had no lethal argument.
- Ornithopter chumps at s27/s30 (divergent-neutral, same as prior wave's #11).

### F6 vs deck139 (WIN T11, 24 decisions)
Clean: Master cast first-listed at printed (9/9) (s15); Memorial first-listed T9 (s18);
s13 correctly PASSED the Scarecrone draw-sac while ahead 20/17 (guide: only when behind) —
a discipline point the 4.75-bit arm failed in its A3 game. Full attacks, Yotian deployed
and sent. No divergences.

---

## The five prior classes under FP8

| # | 4.75-bit failure class | FP8 outcome |
|---|---|---|
| 1 | Backwards flying-rules invention; blocks declared against explicit "(your blocker dies)" lines | **Mostly gone.** No invented rules in the same Emrakul-deck combats (F2 s33/s41 read outcomes correctly). One residual outcome-line misread (F4 s19, ignored a "you kill it, blocker lives" free kill). 3 rules-misreads → 1. |
| 2 | Tolarian Academy tapped on opponent's turn | **NOT TESTABLE** — Academy never reached play in any FP8 game (0 prompt mentions). No verdict either way. |
| 3 | Sacrifice keep-list break (Academy Ruins over Great Furnace) | **Did NOT recur.** Two full gauntlets, keep-list intact, every intended pick ladder-rule-1. The docket-#1 one-line overrun DID recur (3 events, one causing an engine-side off-ladder pick the model never chose). |
| 4 | Reasoning-to-answer flip (concluded B1:A2, emitted B1:A1) | **Did NOT recur** — programmatic trace-vs-reply comparison across all 174 decisions found zero real flips (all regex hits were template echoes). |
| 5 | Academy Ruins recursion on the wrong card | **RECURS** — F2 s28, targeted Cathodion; trace never engages the Master/Memorial restriction. |

Also not recurring: the deals-0 attack (menu hallucination) and the no-lethal-argument
land decline.

## Score and A/B answer

**FP8 arm**: 174 decisions, **2 comprehension failures** (F4 s19 annotation misread,
F2 s28 recursion target) + 3 prompt-assisted overrun events (docket #1) + ~4
divergent-neutrals + 2 justified audibles ⇒ **≈97% guide-adherent** (≈98.8% counting
only model-caused failures).

| Arm | Comprehension failures | Adherence |
|---|---|---|
| 35B 4.75-bit | 8 | ≈86% |
| **35B FP8** | **2** | **≈97%** |
| 122B | 0 | ≈95% |

**Verdict: predominantly QUANT DAMAGE.** The aggressive 4.75-bit quant caused the bulk of
the seat's comprehension failures: the invented combat rules, the answer flips, the
keep-list break, and the deals-0/menu hallucinations all vanish under the official FP8
quant on the same tier — including in the identical Emrakul-gauntlet matchup that produced
them. A thin tier-intrinsic remainder separates FP8 from the 122B: Academy Ruins recursion
discipline (recurred exactly), one outcome-line neglect, and susceptibility to the
docket-#1 multi-answer phrasing (122B: zero overruns). Divergence quality also improved to
122B-like: both FP8 audibles (lethal-assured land hold; annotation-argued no-block) are
argued from true board facts.

Caveats: 6 games is low power for rate claims on rare events — "class extinct" reads are
observations at n=174 decisions, not proofs; and class 2 (Academy turn discipline) is
untested this run because Academy never hit play. A wave-36 prompt pass fixing docket #1
("sacrifice #3 — of 6 total, asked one at a time") plus a guide line pinning Academy Ruins
recursion targets would likely close most of the remaining gap to the 122B.
