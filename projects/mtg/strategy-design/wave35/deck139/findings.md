# deck139 "Mutants" (Ikoria mutate) — wave-35 GUIDE-REVIEW findings (all three arms, weighted to arm C)

Rubric (owner ruling #1): win rate irrelevant; score adherence, and where the pilot diverged,
whether departing was correct at that moment. Arm weighting per the wave-35 reviewer brief:
**arm C (Qwen3.6-35B-A3B-FP8, `matchups-20260822-111102`) is the standing pilot the revised
guide must teach**; arm B (122B) marks under-specification via justified audibles; arm A
(retired quant) is background only. Arms A/B evidence is cited from my wave-35 tier review
(`wave35-review/deck139-tier-review.md`); arm C mined fresh here (no fp8-review existed for
this seat).

## Arm C headline numbers (6 games as deck139, 1 win)

| Metric | value |
|---|---|
| Games (wins) | 6 (1 — vs116, won t12 at 28 life) |
| Decisions (non-gameend/gamestart) | ~195 |
| Fallbacks | 3 (1.5%): 1 unparsed, 2 stale_echo, **0 empty/budget** |
| Mutate-cost cast lines taken when offered | **9/10** (the skip: vs146 seq24, see D3) |
| Merge step ("mutate" vs Cast Normally) | 3/3 mutate |
| "mutate over" answers | 7/7 over — 6 correct, **1 forced misplay by the guide (G1, vs105 seq38)** |
| Host choices | 4/5 good (vs105 seq39 compounded G1) |
| Mulligans | 6/6 rule-correct, incl. one correct mull the guide's own gate text does not license (G3) |
| Shockland pay/tap answered | 12 tap / 3 pay (2 pays justified; 1 leak = 2 life) — self-inflicted life **6 over 6 games** (arm A wave-33 measured era: 40) |
| Discard asks | lands pitched when a land was listed 6/6; landless lists handled sensibly 4/4 (guide has no rule for them — D3) |

All three arm-C fallbacks landed on the **shockland pay/tap ETB ask** (vs158 seq9 unparsed;
vs152 seq8 + seq27 stale_echo) — routed to notes.md N2, not the guide.

## The two mandated guide bugs, resolved with cross-arm evidence

### G1 — "ALWAYS mutate over" is wrong when the cast card is smaller than the host. CONFIRMED AT BOTH ARMS, opposite outcomes.

- **Arm C obeyed it into a material misplay** (139v105 seq38–39, t12, opp at 7, own poison 9):
  Migratory Greathorn (3/4) mutating; reasoning quotes the guide verbatim — *"According to the
  strategy guide: 'Always answer mutate over.'"* — answers "mutate over" (seq38), then hosts the
  **6/6 Brokkos pile** (seq39). The pile's P/T is the TOP card's: the 6/6 trampler became a 3/4
  (battlefield render at seq42 confirms `Migratory Greathorn ... (3/4) [mutated pile...]`). The
  race at opp-7 was blunted; the game was lost at 10 poison next combat. This is the Step-0
  echo test firing: a rule the executor cites while misplaying is a guide bug.
- **Arm B audibled correctly against the same rule** (139v146 seq32, t19): casting Greathorn
  (3/4) onto Brokkos (6/6), the 122B walked the guide's contradiction and answered "under",
  keeping the 6/6 on top with the fetch trigger still firing — objectively correct.
- **Mechanism verified in the primitives** (`Res/sets/primitives/`): every mutate card carries
  BOTH `@mutated(this)` and `@mutated(mytgt)` trigger lines — the mutate trigger fires whether
  the card ends up over or under; only the top card's printed P/T changes. "Under" costs
  nothing. The old rule's rationale ("putting the Apex on top is how you get its trigger") is
  half false: the trigger is unconditional; only P/T/evasion ride the top card.
- **Ask-order constraint for the rewrite**: the engine asks over/under BEFORE the host target
  (seq38 -> seq39), so the rule must instruct the pilot to decide its intended host first.
- **Fix shipped** (revised guide): "the biggest printed P/T goes on top" — over when the cast
  card is bigger, under when the host is; host-selection rule added (cheap mutates host a small
  engine creature, never the Apex pile except with "under"); worked example = the exact
  observed Greathorn-onto-Brokkos board; falsity fact intercepts "'over' is always right".

### G2 — Dryad-on-curve vs cheap-mutate priority conflict. RESOLVED: Dryad wins.

- Arm B hit the exact collision twice and audibled to Dryad both times, quoting both of the
  guide's "every time" imperatives (139v158 seq7 t2; 139v116 seq11 t5) — justified: Dryad is
  the fixing that turns the {W}{W}/{U}{U} Apex mutates on, and the skipped mutate keeps.
- Arm C never faced the literal Dryad-vs-mutate list, but showed the same under-specified
  priority seam in another guise (139v146 seq24 t13, at 8 life: skipped the offered Greathorn
  mutate line for Nissa — the lone mutate-line skip of the arm; letter-divergent, thin).
- **Fix shipped**: the Gemrazer/Greathorn "every time it is offered" imperative now carries the
  explicit exception in the same sentence ("when Cast Dryad is also listed and you cannot pay
  for both this turn, cast Dryad FIRST — the mutate keeps; take it next turn"), plus a
  DECIDING SITUATIONS line. A light Nissa clause scopes her below bodies at <=10 life under
  attack (the seq24 shape).

## Other arm-C guide-attributable findings

- **D1 (game-losing, guide-fixable) — poison full-coverage failure.** 139v105 seq42 (t13, 9/10
  poison, three infect attackers, THREE legal blockers listed): the pilot assigned only two
  blocks — *"I have only two functional blockers"* — writing off the B-list's Arboreal Grazer
  (-2/1) as *"dead, cannot block effectively"*, left the 2/2 Contagious Nim unblocked, took 2
  poison, lost at 20 life. Its own PLAN priced the loss correctly ("9 + 2 = 11. Loss
  confirmed") and its stated rule was the guide's own wording: *"Blocking the two largest
  threats is optimal"* = "block the biggest poison numbers you can" read as triage when full
  coverage existed. Fix: poison block is now a coverage PROCEDURE (one blocker on EVERY
  attacker until blockers run out) + falsity fact that a 0-power/negative-power blocker still
  stops that attacker's entire poison. (Contrast: seq30 t11, two attackers/two blockers, the
  pilot covered both correctly — the defect only bites when the weak-blocker excuse is
  available.)
- **D2 (minor leak, 2 life)** — 139v116 seq8 t2: paid 2 on Steam Vents with no same-turn use
  (narration confirms nothing cast before t5). Same class as arm A's two leaks; rule text
  unchanged (12 tap / 2 justified pays elsewhere show it landing — vs152 seq10 paid and cast
  Fertile Footsteps the same turn, textbook). Note: this land entered via Grazer's put — engine
  rules question routed to notes.md N1.
- **D3 (adherent, worth naming)** — the discard seam: with a land listed, 6/6 lands pitched
  (vs146 seq32/33 Temple Garden x2, seq41/42 Plains x2; vs105 seq24/25 Temple Garden x2). With
  NO land listed the guide is silent and the pilot improvised well — vs105 seq19 pitched the
  duplicate Brokkos (which mutates back out of the graveyard; the guide's own Brokkos line
  makes this the best discard), vs116 seq18 pitched the just-drawn Beanstalk. Fix: the discard
  rule now carries the landless else-branch as an ordered list (spare land -> Brokkos ->
  duplicate -> most expensive uncastable).
- **G3 (gate gap)** — the mulligan section's two sides are not complementary: keep requires
  {G}>=1, but the "Mulligan only when..." list omits {G}=0. 139v158 seq1 hit the gap (3 lands,
  {U}{B}{R}, {G} 0) and the pilot mulled correctly anyway. Fix: "{G} number is 0" added to the
  mulligan side.
- **Adherent highlights (freeze evidence)**: the vs116 win is a textbook guide game (Gemrazer
  mutate t4 -> attack every turn -> Nethroi body when {W}{W} short, called as a hardcast ->
  Everquill onto Snapdax t8 -> won t12). Attack floor obeyed incl. the [deals 0] tag (vs158
  seq16 at 3 life). Early no-blocks vs146 (seq7/10/14) are rule-correct declines (0/3 walls
  die to 3/2 for zero). Pollywog-first above 10 life, merge step, mulligan counts, DECIDE FAST
  — no budget overruns at FP8 (arm B's P2 class: 6 empty; arm C: 0).

## Card-fact audit (HL1 hard gate)

Every mana cost, mutate cost, P/T and zone claim in the deployed guide re-checked against
`Res/sets/primitives/*.txt` this wave: all TRUE (Snapdax {2}{BR}{W}{W} 3/5 double strike —
mutate trigger 4 dmg + 4 life; Illuna {3}{RG}{U}{U} 6/6; Nethroi {4}{GW}{B}{B} 5/5 — engine
target list is `power<=10` anyamount, guide's "take everything it lets you take" is
engine-faithful; Brokkos {2}{UB}{G}{G} 6/6 + `autograveyard` mutate confirmed; Vadrok
{1}{WU}{R}{R} 3/3; Everquill {3}{R} 4/4 token(Feather Eve) = artifact, not creature; Gemrazer
{1}{G}{G} 4/4; Greathorn {2}{G} 3/4; Pollywog {1}{U} 1/3 lord altercost -1; Grazer {G} 0/3;
Dryad {2}{G} 2/4). The one guide sentence FALSIFIED by the primitives is G1's rationale (the
over-only trigger claim), rewritten above.

## Revision mode

Constrained diff (turnaround-preservation): arm A ~97% adherent, arm C 9/10 mutate take-rate,
shockland/mulligan/attack-floor/blocker sections all observed obeyed — those lines are FROZEN
verbatim (moved only where an edit lands inside their block). New text aims at G1, G2, D1, G3,
D3-discard only. Freeze-check: every frozen line survives verbatim in the revised guide except
the #1-rule "ALWAYS over" bullet (replaced per G1 — its misplay is this wave's witness) and the
poison "biggest numbers you can" clause (replaced per D1 — quoted while misplaying).
