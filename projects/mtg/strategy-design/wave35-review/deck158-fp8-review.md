# deck158 (Orcs of Mordor) — Wave-35 QUANT A/B: FP8 arm review

Rubric (owner, binding): win rate is irrelevant; score guide ADHERENCE per decision, and where the
model diverges, whether departing was correct AT THAT MOMENT (audible) or a comprehension failure.

- Same model tier both arms: Qwen3.6-35B-A3B. Prior arm = 4.75-bit quant
  (`deck158-tier-review.md`, Arm A). This arm = official FP8 quant.
- Guide: `projects/mtg/bin/Res/ai/baka/deck158_strategy.txt` — UNCHANGED, including the known
  false line 117-118 ("their deaths feed the Army" — engine-verified false for Bowmasters/Crebain;
  defect G1). Obeying that line this wave counts as ADHERENCE, not a model failure.
- FP8 logs: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260822-111102/*ai_baka_deck158-0x*`
- 6 games, same opponents. Record 5-1 (won 105, 116, 139, 146, 152; lost 36) — not scored.

Scored decisions (gamestart/gameend excluded): 119 total — 36:18, 105:18, 116:18, 139:25,
146:18, 152:22.

## The three recurrence questions (the point of this wave)

**1. Guide's false amass-on-death line (G1) — does FP8 obey it?** NO — it REFUTES it, which is
the rubric's ideal outcome. deck152 seq19 T12 (blockers, 13 life vs 5 unblocked, step-3 regime):
the trace quotes the guide line verbatim — "'Easterling Vanguard, Orcish Bowmasters and Dunland
Crebain are your first choices for step 2 - their deaths feed the Army.'" — then corrects it from
card text: "Crebain doesn't have a death trigger. Only Easterling Vanguard does. So blocking is
pointless." BLOCKS: none, exactly right under step 3. Zero G1-induced chump-blocks anywhere in the
FP8 arm (the 4.75-bit arm had two: A-36 seq15, A-152 seq10). The one place FP8 leans on
death-amass is Easterling Vanguard (deck36 seq16/seq18) — the one card for which the line is TRUE.

**2. Best-blocker rule under pressure (the A-139 "useless without sac outlet" failure)?** Did NOT
recur. deck36 T8-9 at 4 life vs a 4-creature board — the closest analogue to the prior failure —
FP8 cast Easterling Vanguard on sight (seq15: "cheaper and sets up the amass trigger... best
blocker") and at seq18 ran the guide's block test correctly: enumerated all three lines (block A3 →
5 unblocked, dead; block A2 → 4 unblocked; no block → 7, dead) and assigned the Vanguard to
Cathodion, the biggest legally blockable attacker, per step 2. No invented disqualifier, no
assumed lethal through untapped blockers.

**3. Lethal arithmetic through blockers?** Sound where it decided games: deck146 seq16-18 walked
the block branch explicitly (Silencer blocks the 7/7 → unblocked Bowmasters still lethal at 1
life); deck152 seq21-22 and deck139 seq18-19 counted lethal before waiving casts/land drops. But
see the new arithmetic defect class below — it surfaced in non-lethal contexts.

## New FP8 failures — all model-owned, all micro-arithmetic/reading

- **F1. deck152 seq10 + seq14 (T7, T9) — P/T TRANSPOSITION, twice**: Bowmasters trigger with
  Elite Spellbinder (3/1) in the target list — a toughness-1 creature rule 4 orders shot. Both
  traces read it backwards: "It won't kill Elite Spellbinder (toughness 3)... There are no
  toughness 1 creatures" → face. Same misread both times; rule 4's own "read the (P/T) of every
  opponent creature FIRST" was quoted and then misapplied. FAILURE ×2 (the guide's letter was
  followed; the number-reading failed).
- **F2. deck105 seq11 (T6) — rule-4 override on a false premise**: Bowmasters trigger →
  Contagious Nim (2/2 infect). The trace KNOWS the rule ("Contagious Nim has toughness 2") and
  overrides it citing the Feed the Swarm paragraph (a different card's rule), claiming the shot
  will "remove Contagious Nim". 1 damage does not remove a 2/2 — exactly the "'it is the bigger
  threat' is not a reason" case rule 4 pre-empts. FAILURE (bad audible: argued from a false fact).
  Cleaned up next decision by an ADHERENT Feed the Swarm on the Nim (seq12, infect clause).
- **F3. deck36 seq12 (T6) — damage-vs-toughness**: Foray trigger, 6 damage, target list Glaze
  Fiend 1/2 / Steel Wall 1/5 / Cathodion 4/4 / Master of Etherium 8/8. Trace: "The ability deals
  6 damage. All of them will die." → targets the 8/8. 6 < 8; the engine log confirms Master
  survived, then blocked and killed the 6/6 Army (Army died, Master died only to the combined
  6+7). Guide-silent on target choice, but the premise was false and it cost the win condition.
  (The attack itself was guide-adherent: March out + "declare every creature".)
- **F4. deck36 seq16-17 (T8) — declined a listed, payable cast**: "Cast Olog-hai Crusher" was in
  the numbered list; the guide's TAKE THE CAST section says the game has already checked the mana.
  The trace re-derived mana, miscounted (3 lands + Treasure = 4 total, enough for {3}{R}; the
  trace concludes "giving me {R}{B}{R}, still not {3}{R}") and overrode the prompt's explicit
  "every listed choice is legal AND PAYABLE" line. FAILURE (one error, two decisions). Material:
  a 4/4 that legally blocks (an Orc was on board) before the fatal T9 combat.
- **R1 (reasoning defect, outcome-neutral). deck36 seq18**: the block trace computes "4 - 4 = 0
  life. I survive!" — 0 life is a loss. The chosen block matched guide step 2 anyway and every
  line was lethal, so the decision scores ADHERENT; the threshold error is noted for the docket.

## Divergence table (FP8)

| Game | seq/turn | Decision | Verdict |
|---|---|---|---|
| deck36 | seq12 T6 | Foray 6 dmg → Master of Etherium 8/8 ("all of them will die") | FAILURE — 6<8; engine confirms survival; Army lost in the follow-on combat |
| deck36 | seq16-17 T8 | Decline listed Olog-hai Crusher ("can't pay {3}{R}") | FAILURE — off-by-one mana recount overriding TAKE-THE-CAST and the prompt's payability guarantee |
| deck105 | seq11 T6 | Bowmasters → Contagious Nim 2/2 ("remove it") | FAILURE — knowing rule-4 override on a false premise |
| deck152 | seq10 T7 | Bowmasters → face with Elite Spellbinder (3/1) in list | FAILURE — P/T transposed ("toughness 3") |
| deck152 | seq14 T9 | Same again | FAILURE — same transposition |
| deck152 | seq19 T12 | BLOCKS: none in step-3 regime, refuting guide line 117 from card text | ADHERENT + IDEAL AUDIBLE — the exact override the rubric hopes for |
| deck152 | seq20-21 T13 | Hold Mountain, hold Crebain, attack for the win | Minor divergence from "(1) play your land" / amass-in-MP1 — defensible, lethal verified first |
| deck139 | seq18 T11 | Play no land with lethal on board | Minor — defensible, lethal verified |
| deck146 | seq16 T11 | Play no land with lethal on board | Minor — defensible, lethal + block branch verified |

Everything else — mulligan by the engine's {B} count (deck36 mulled a 0-{B} hand and bottomed
correctly), March on curve every game, amass always in MP1 (deck116/deck105 textbook), Mauhur
timing, second-Mauhur "take the cast" (legend sac handled), Assault absent from draws, Feed the
Swarm targets (deck105 seq12 infect, deck139 seq14 only-legal-target), Bowmasters face-branch when
genuinely no toughness-1 (deck139 seq7/24, deck146 seq7/11), Witch-king attacked always — ADHERENT.

## Score and comparison

| Arm | Scored | Divergent | Failures | Audible/defensible | Adherence |
|---|---|---|---|---|---|
| 4.75-bit (35B) | 126 | 9 | 3 decisions (2 guide-induced G1, 1 own) | 6 | ~93% |
| **FP8 (35B)** | 119 | 10 | **6 decisions / 4 events (all model-owned)** | 4 (+1 ideal audible) | ~92% |
| 122B | 125 | 3 | 0 | 3 | ~98% |

## Verdict: quant damage or tier-intrinsic?

**Tier-intrinsic.** None of the 4.75-bit arm's failures recurred under FP8 — categorically zero
G1-obedience chumps, zero Vanguard declines, zero assumed-lethal-through-blockers — and FP8
produced the rubric-ideal audible against the guide's own false line. But FP8 substituted a new,
non-overlapping set of model-owned failures, all of one shape: single-number misreads (P/T order,
damage vs toughness, mana count, the 0-life threshold) inside otherwise rule-quoting traces. Two
quants of the same tier each show a few comprehension slips in ~120 decisions with no shared
signature → the slips are the 35B tier's noise floor, not quant damage in either direction. One
6-game run per arm is low-power for any rate claim (FP8's 4 events vs 4.75-bit's 1 own event is
within noise); the categorical claims above (zero recurrences; the G1 refutation happened) are the
solid findings. 122B's zero-failure bar remains unmatched by either quant.

Standing recommendation unchanged in substance: this seat wants 122B, or the 35B behind a fixed
guide plus prompt hardening. G1 should still be fixed (defense in depth — FP8 saw through it, the
4.75-bit did not; the next model may not).

## New docket items

- **P6 (prompt hardening, from F1/R1)**: the 35B tier misreads compact "(3/1)" P/T notation under
  rule 4 and mishandles the life-0 threshold. Candidates: print toughness explicitly in Bowmasters
  target lists ("toughness 1 — dies to the shot"), and state "at 0 life you LOSE" in the blockers
  step-2 text. Both are one-clause renders that remove a demonstrated single-number failure mode.
- **F4 pattern**: consider a guide line "never re-derive mana for a listed cast — Treasures are
  already counted" (the existing TAKE-THE-CAST text was quoted and still overridden once).
