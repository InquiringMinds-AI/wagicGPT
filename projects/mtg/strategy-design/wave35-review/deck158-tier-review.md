# deck158 (Orcs of Mordor — Rakdos amass) — Wave-35 A/B strategy review

Rubric (owner, binding): win rate is irrelevant; score guide ADHERENCE per decision, and where the
model diverges, whether departing was correct AT THAT MOMENT (audible) or a comprehension failure.

- Guide: `projects/mtg/bin/Res/ai/baka/deck158_strategy.txt`
- Arm A (35B-A3B): `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260820-192210/*ai_baka_deck158*`
- Arm B (122B-A10B): `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260821-033000/*ai_baka_deck158*`
- 6 games per arm, same opponents (deck36, deck105, deck116, deck139, deck146, deck152).

## Guide core lines (scoring基准)

1. Beatdown always; amass in MAIN PHASE 1 then attack (overrides general second-main advice).
2. Army is the win condition; trust the printed `{right now: Army X/X -> Y/Y}` numbers.
3. March: best T2 play, cast BOTH copies. Mauhur before amass when both fit. Muster on sight.
4. Bowmasters: main-phase cast; trigger shoots the FIRST toughness-1 creature, else face.
5. Foray: printed damage only ever hits a creature; never count it toward face lethal.
6. Assault: take the LARGEST X offered (only X ≥ 2).
7. Feed the Swarm: read the printed life price; spend on what is actually killing you; equal
   threats → cheaper one.
8. Blocking: answer from the B-lines only; if unblocked total ≥ life, chump until below life
   (Vanguard/Bowmasters/Crebain first); else block only "you kill it, your blocker lives"/"both die".
9. Mulligan: read the engine's {B} count; {B}=0 → mulligan (red-spell exception); max two.

---

## Arm A (35B-A3B) — per-game findings

### A vs deck36 (won) — 22 scored decisions
- seq4 T2 March on curve; seq7 T4 Crebain (biggest amass); seq11-12 T6 Muster+Bowmasters all in
  MP1 before attacking — ADHERENT.
- seq13 T6 Bowmasters trigger → Silver Myr, the only toughness-1 in the list — ADHERENT (rule 4).
- **seq15 T7 DIVERGENCE (failure, guide-induced)**: chump-blocked Cathodion (3 dmg, life 15 —
  step-3 regime says take it). Trace: "Bowmasters triggers amass 1 on death... blocking is
  strictly better". FALSE — Bowmasters amasses on ETB/opp-draw, not death. Engine confirms: Army
  7/7 before the block, still 7/7 at seq17. The false fact comes from the GUIDE itself (line
  117-118, see defect G1). Lost a Bowmasters (future opp-draw triggers) to save 3 of 15 life.
- seq18 T8 Foray damage → Ornithopter over Cathodion — DIVERGENCE (justified audible): trace
  correctly notes Cathodion's death gives the opponent {3}; guide is silent on Foray target choice.
- seq20-21 T10 no land / no cast with lethal 10/10 on board — harmless, opponent at 2.

### A vs deck116 (won) — 17 scored decisions
- seq6 T4 Mauhur first (rule 3), seq9 T6 Foray by printed number, seq15-16 T10 Assault X=3 max
  and lethal attack — ADHERENT throughout. Cleanest Arm-A game; no divergences.

### A vs deck105 (won, infect) — 28 scored decisions
- seq1 mulligan: quotes the guide's "{B} 0: MULLIGAN" against the engine count — ADHERENT,
  exemplary. seq3 bottoms a Mountain first — ADHERENT.
- **seq8 T5 DIVERGENCE (defensible audible / open annotation question)**: declined the Army block
  on Ichorclaw Myr despite B-line "you kill it, your blocker lives" (rule 8 says take it). Trace
  reasons from Oracle: Myr's becomes-blocked +2/+2 makes it a 3/3 dealing 3 poison. If the engine
  implements that trigger the annotation is wrong and the pass was right; see defect P3.
- seq12 T7 Warg blocks the bigger infect ("you kill it, lives") — ADHERENT. seq19 T9 Mauhur eats
  Ichorclaw — ADHERENT.
- seq14 T8 Mauhur over Assault (menu X would be 1; rule 6 forbids) — ADHERENT.
- seq24-27 T12 Bowmasters ×2: first shoots the toughness-1 Ichorclaw, second (no toughness-1 left)
  the face — ADHERENT (rule 4, both branches exercised correctly).

### A vs deck139 (lost) — 17 scored decisions
- seq4 T3 March on curve — ADHERENT. seq5 T4 correct no-block (5 < 20).
- seq6 T5 held Mountain ("unnecessary for this turn's spells") — minor divergence from "(1) play
  your land"; no cost (Swamp played after the Muster draw), but the reasoning treats the land drop
  as optional.
- seq7 T5 Muster over the second March — mild divergence from "cast BOTH copies" (March adds a
  counter on every attack); defensible for the card draw; March landed T7 anyway.
- **seq15+seq17 T9 DIVERGENCE (comprehension failure, game-losing context)**: at 4 life vs a
  4-creature board, declined Easterling Vanguard TWICE — trace: "can cast, but useless without a
  sac outlet". Directly contradicts the guide: Vanguard "is your best blocker and best trade...
  Attack and block with it freely" and "A card is in your cast list: cast it." Same turn the plan
  asserts "Attack with Orc army to win this turn" (8/8 vs 7 life) through four untapped potential
  blockers — lethal assumed, not checked. Died on the crack-back at -5.

### A vs deck146 (lost) — 17 scored decisions
- seq4/seq7 amass in MP1 — ADHERENT. seq12 T7 Bowmasters → face with no toughness-1 in the list
  (Silencer is 3/2) — ADHERENT. seq13 T8 Army blocks Silencer "both die", unblocked total below
  life — ADHERENT (rule 8 permits both-die).
- seq18 T11 Feed → Goblin (0 life) over Nadaar 3/3 (3 life) at 6 life — DIVERGENCE (defensible
  audible): guide leans "pay more for what is killing you", but at 6 life the free kill is a
  reasonable life-preservation call; trace wobbles (flips twice) but lands on the printed prices.

### A vs deck152 (lost, 3-life finish) — 25 scored decisions
- seq4 Muster; seq8 T5 Bowmasters trigger → Intrepid Adversary, a toughness-1 lifelinker —
  ADHERENT (rule 4). seq12 T7 Foray printed number; damage → Elite Spellbinder (kills the flier)
  — ADHERENT. seq19-20 T11 Assault X=3 max — ADHERENT. seq24 T13 rebuilds with Swarming after
  Army death — ADHERENT.
- **seq10 T6 DIVERGENCE (failure, guide-induced)**: chump-blocked the Wolf at 17 life (2 dmg,
  step-3 regime) with Bowmasters, explicitly quoting guide line 117: "their deaths feed the Army".
  Engine confirms no amass (Army 2/2 before and after). Same false-fact source as A-36 seq15.
- seq27 T13 Feed → Wolf (0 life) at 3 life — defensible: cannot afford any life-price target.

## Arm B (122B-A10B) — per-game findings

### B vs deck36 (lost) — 20 scored decisions
- seq4 Muster (no March drawn); seq9 T5 Mauhur blocks Silver Myr "you kill it, lives", Cathodion
  correctly unblocked (3 < 18) — ADHERENT.
- **seq7 T4 DIVERGENCE (justified audible)**: declined to attack with the 1/1 Army. Trace walks
  the whole board: untapped Cathodion 3/3 blocks and eats the win condition for zero damage, no
  March out (so no attack trigger to bank). The guide's own Army clause ("attack whenever its
  blocker line says you kill it, your blocker lives") supports holding; the blanket "declare every
  creature" line does not. Correct at that moment.
- seq12 T6 Feed → Ornithopter (0 life) over Cathodion (3 life) — mild divergence/defensible:
  guide's list includes "a flier"; free removal of an Army-blocker, and the second Feed took
  Cathodion #2 at seq16 T8 paying the 3 deliberately. Low cost either way.
- All subsequent attacks declared; lost to the artifact deck's race with only two amass draws all
  game (variance, not choices).

### B vs deck105 (won, infect) — 15 scored decisions
- seq4 T2 March; seq6 T4 Mauhur first (rule 3 — only one spell fits, doubling all future amass
  with March already out; trace walks the guide priorities explicitly); seq9-10 T6 Muster + second
  March both in MP1 (rule 3 "cast BOTH copies"); attacks every turn — fully ADHERENT, zero
  divergences. Model's own arithmetic tracked the doubled counters correctly.

### B vs deck152 (lost) — 16 scored decisions
- seq4 March on curve; seq10 T8 Bowmasters → face with no toughness-1 (Moonrage Brute 3/3) —
  ADHERENT. seq12-13 T9 Feed → Sigarda 4/4 flier paying 4 life — ADHERENT (rule 7: the flier/
  biggest threat, price paid deliberately). seq15 T10 correct no-chump at 11 life (3 < 11).
- No divergences; lost to a curve-out. Adherent play, bad race.

### B vs deck116 (lost) — 17 scored decisions
- seq8 T6 SECOND March over Bowmasters — ADHERENT (rule 3's "cast BOTH copies", the exact line
  Arm A soft-missed in A-139). seq10 T8 Swarming (bigger amass) — ADHERENT. Attacked every turn.
- seq12-17 T11 (guide-silent, high quality): opponent's Emrakul Annihilator-6 forced six
  sacrifices; model sacrificed Treasure, three lands and BOTH Marches while keeping the 10/10 Army
  — correctly ranked the win condition above the engine with the game ending. Note the prompt
  mislabeled the effect as "your own effect (Sacrifice)" (defect P2); the model saw through it.
- No divergences. Died to Emrakul; nothing the guide covers.

### B vs deck139 (won) — 27 scored decisions
- seq5/seq8 correct no-chump vs Gemrazer (4 < life, chumps die for nothing) — ADHERENT.
- seq7 T5 Mauhur first — ADHERENT. seq11-12 T9 Muster + Bowmasters in MP1 (doubled to 5/5);
  seq13 Bowmasters → face, no toughness-1 in list — ADHERENT.
- seq15-16 T11 Feed → Migratory Greathorn (3 life at 8 life) over the 1-life Grazer — ADHERENT
  (rule 7: pay more for the creature actually in your way).
- seq21-24 T13: Muster in MP1, "cast nothing" on Olog-hai pre-combat, attack, then Olog-hai in
  second main — sophisticated ADHERENCE: the MP1 override applies to amass cards only; non-amass
  creatures correctly deferred per the general rule. seq27 T15 lethal alpha — ADHERENT.

### B vs deck146 (won) — 30 scored decisions
- seq4 March; seq7 Muster; seq11 Bowmasters amass; seq12 Bowmasters → face (no toughness-1) —
  ADHERENT. seq13 T8 opponent edict-style choice: gave up Bowmasters, kept the Army — correct
  guide-silent judgment. seq15 T9 Foray with printed number — ADHERENT.
- seq20-21 T13 Assault X=4, the largest offered — ADHERENT. seq23 T15 Witch-king cast and
  attacked every turn after ("Attack with it always") — ADHERENT.
- seq26-27 T17 Feed → hasty double-strike Goblin (0 life) with a deathtouch Adventurer also on
  board — DIVERGENCE (justified audible): trace shows both are 1/1 blockers that eat the Army
  either way, model holds TWO Feeds and sequences cheaper-first, Adventurer next, then attacks for
  the win. That is rule 7's "equally in your way → cheaper one" applied correctly. Won on plan.

---

## Divergence table

| Arm | Game | seq/turn | Decision | Verdict |
|---|---|---|---|---|
| A | deck36 | seq15 T7 | Chump-block Cathodion w/ Bowmasters at 15 life | FAILURE — false "amass on death" premise (guide defect G1); engine shows no counter |
| A | deck36 | seq18 T8 | Foray damage → Ornithopter not Cathodion | AUDIBLE — avoids gifting {3} from Cathodion's death trigger |
| A | deck139 | seq6 T5 | Hold Mountain | Minor failure — treats guide's "(1) play your land" as optional; zero cost |
| A | deck139 | seq7 T5 | Muster over 2nd March | Defensible — guide leans March ("cast BOTH copies"); draw is real value; March landed T7 |
| A | deck139 | seq15/17 T9 | Decline Vanguard twice at 4 life ("useless without sac outlet") | FAILURE — contradicts guide's best-blocker rule + "cast it"; assumed lethal through 4 blockers; game-losing context |
| A | deck105 | seq8 T5 | Decline "you kill it, lives" block vs Ichorclaw | Defensible audible — Oracle becomes-blocked trigger vs annotation (defect P3, unresolved) |
| A | deck146 | seq18 T11 | Feed → Goblin (0) over Nadaar (3) at 6 life | Defensible audible — life preservation; reasoning wobbly |
| A | deck152 | seq10 T6 | Chump-block Wolf w/ Bowmasters at 17 life | FAILURE — quotes guide line 117 verbatim (defect G1); engine shows no counter |
| A | deck152 | seq27 T13 | Feed → Wolf (0 life) at 3 life | Defensible — cannot pay any life price at 3 |
| B | deck36 | seq7 T4 | No attack with 1/1 Army | JUSTIFIED AUDIBLE — full board walk; win-con vs untapped 3/3, no March out |
| B | deck36 | seq12 T6 | Feed → Ornithopter (0) over Cathodion (3) | Defensible — "a flier"/blocker clause, free; 2nd Feed later took Cathodion paying 3 |
| B | deck146 | seq27 T17 | Feed → dbl-strike Goblin over deathtouch Adventurer | JUSTIFIED AUDIBLE — two Feeds sequenced cheaper-first per rule 7; won as planned |

Arm A: 126 scored decisions, 9 divergences → 3 failures (2 guide-induced, 1 own), 6 audible/defensible.
Arm B: 125 scored decisions, 3 divergences → 0 failures, 3 audible/defensible.

## Prompt/guide defects for the wave-36 docket

- **G1 (GUIDE TEXT BUG, deck158_strategy.txt lines 117-118)**: "Easterling Vanguard, Orcish
  Bowmasters and Dunland Crebain are your first choices for step 2 - their deaths feed the Army."
  Only Vanguard amasses on death; Bowmasters (ETB + opp-draw trigger) and Crebain (ETB) do not.
  Engine-verified twice (A-36: Army 7/7 unchanged after Bowmasters died; A-152: 2/2 unchanged).
  Directly caused both Arm-A chump-block failures — the 35B quotes the line verbatim. Fix the
  sentence to name Vanguard alone.
- **P2 (render mislabel)**: B-116 seq12-17 — opponent's Emrakul Annihilator-6 sacrifice prompts are
  headed "SACRIFICE ONE OF YOUR OWN CARDS: your own effect (Sacrifice) makes you sacrifice...".
  The effect is the opponent's. 122B parsed through it; a weaker model may not.
- **P3 (annotation vs trigger, needs engine verification)**: A-105 seq8 — B-line for blocking
  Ichorclaw Myr reads "you kill it, your blocker lives (infect shrinks your blocker to 1/1)",
  apparently ignoring the Myr's becomes-blocked +2/+2 (which would make it 3/3, killing the 2/2
  Army with 3 poison-counter damage). Either the annotation mis-predicts or the card script lacks
  the trigger (/wagic-cardscript check). The guide's "answer from the B-lines and nothing else"
  hinges on those annotations being true.
- **P4 (hand-render staleness, minor)**: B-105 seq6 — the hand list still contained the Mountain
  played earlier the same turn; the model flagged "might be a display error" and recovered from the
  mana lines. Verify hand snapshot timing.
- **P5 (floating-mana note readability, minor)**: B-36 seq12 — "Already in pool: {r}{b}... SEPARATE
  from the 2 untapped sources" cost the model a long disambiguation detour mid-target-choice. Both
  models coped; wording could state what produced the floating mana.

## Per-arm verdicts and the A/B answer

**Arm A (35B-A3B)**: excellent on the guide's mechanical rails — mulligan by the engine count
(quoted verbatim), Bowmasters toughness-1 targeting perfect in every instance including the
no-toughness-1 face branch, Assault X always max, Foray/amass printed numbers trusted, amass
always main-phase-1. Its failures cluster where the guide is wrong or silent: it echoes the guide's
false Bowmasters-death fact uncritically (twice), and in the one real off-script moment (A-139 T9)
it invented a disqualifier for Vanguard ("useless without sac outlet") that the guide explicitly
contradicts, while assuming an uncontested lethal. 35B follows the letter; it does not audit it.

**Arm B (122B-A10B)**: same rail adherence (including the both-copies-of-March and Mauhur-first
lines Arm A soft-missed) plus genuinely correct audibles — every divergence was argued from true
board facts and each was right at that moment. Guide-silent decisions (Annihilator triage, edict
choice, MP1-amass/MP2-creature split) were uniformly high quality. Zero comprehension failures.

**A/B for this seat**: 122B is clearly better on divergence quality; adherence is near-parity.
Two of 35B's three failures are guide-induced and die with the G1 fix, leaving roughly one real
comprehension failure in six games — but that one (A-139) is the dangerous kind: a self-invented
rule overriding an explicit guide line at a decisive moment. Recommendation: NOT a safe swap yet
on this seat. Fix G1 (and check P3), re-run Arm A; if the chump-blocks disappear and no new
self-invented disqualifiers show up, 35B is acceptable here — its rail-following is already at
122B level, and this deck's guide is rail-heavy by design.
