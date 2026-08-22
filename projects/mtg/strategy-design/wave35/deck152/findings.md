# deck152 (Bant Human Midrange) — Wave-35 guide review (arms A+B+C, arm-C-weighted)

**Seat**: deck152 | **Deployed guide**: `bin/Res/ai/baka/deck152_strategy.txt`
**Arm A/B findings**: `wave35-review/deck152-tier-review.md` (cited, not re-derived — this seat had
no fp8-review file, so the arm-C read below is new work from the translogs).
**Arm C (Qwen3.6-35B-A3B-FP8, THE STANDING OPPONENT)**: `matchups-20260822-111102`, 6 games, 5-1.
Rubric: owner ruling #1 — adherence + divergence quality; win table is context only.

## Arm C per-game findings (new)

### C vs116 — won t9 (20/3)
Textbook: Ranger t2 (s6), Sigarda on curve t6 (s12), Spellbinder exile pick took **Emrakul from
their hand** (s14-15, t8), escalating attacks s10/s13/s16. Plan used as a memo and verified
against the board every ask (s2/s6/s8/s11). 0 divergences.

### C vs105 (infect) — won t16 at 20 life
The infect posture executed end-to-end: Aspirant blocks Necropede s9 (t5); **FA #1 on Contagious
Nim s13-14 (t6)** and **FA #2 on Hand of the Praetors — the infect lord — s24-25 (t10)**: "kill
[infect] on sight" verbatim. Deploy floor every turn; Katilda s28, Huntmaster s39. One harness
fallback: s32 (t15 upkeep priority) — the window offered only 2 Flip-Side toggles + 2
activations; the model answered `CHOICE: Cast Tovolar's Huntmaster` (no number, option absent)
→ `unparsed_reply`, heuristic passed, Huntmaster cast normally at s39. Harness-lane (notes.md
§3), not chargeable. One trivial divergence: s12 (t6) spent a decision on Flip Side (the
never-flip line broken once, zero cost).

### C vs146 — won t15 (17/-4), but the arm's defining failure class fired twice
- s10 (t6): BLOCKS: none vs Triumphant Adventurer, trace cites "deathtouch + first strike means
  the 2/2 dies and the 1/1 lives" — **adherent, and proof the pilot can know the mechanic.**
- **s14 (t8): Sigarda blocks Triumphant Adventurer — FAILURE.** The render printed
  `may block A1 (your blocker dies, attacker lives)` and `[first strike, deathtouch]` on the
  A-line. The trace READ it and overrode it: *"The parenthetical is likely a template error.
  I'll stick with blocking."* Sigarda — the guide's named best card — died to a 1/1 for zero
  (first strike kills her before she deals damage). A trust-doctrine violation the guide never
  anticipates: rule 4 makes the "may block" LIST authoritative but says nothing about the
  printed TRADE OUTCOME.
- **s21 (t10): Intrepid Adversary blocks the same Adventurer — FAILURE, same class.** Trace:
  *"Intrepid hits TA for 3. TA dies. Great trade"* — false (first strike + 1 toughness: Intrepid
  dies, TA lives, exactly as the parenthetical said). Lost the lifelink lord to prevent 1 damage.
- s43 (t15) Spellbinder exile took Kaya the Inexorable — good. s38 `stale_echo` on the
  Cast-Card-Normally sub-menu (echo named Elite Spellbinder mid-Cathar-cast); safe-direction
  fallback completed the cast correctly (Moonrage Brute attacked s46). Notes §6.

### C vs139 (ramp) — won t13 (22/0)
- s13-14 (t7): FA on Arboreal Grazer (0/3 reach) — **justified**: their ONLY creature, opponent
  hand 0, sole wall in front of Katilda/Aspirant (arm B's identical audible was graded justified;
  here it is even closer to rule 5's letter since Grazer WAS their best creature).
- s19 (t9): reply read `CHOICE: 11 (add 5 counters)` — **echo/index mismatch**: option 11 is
  "add 10 counters", "add 5" is option 6. Index won; engine PARTIAL-PAID 2 valor counters
  (events s20). Outcome coincidentally = the guide's "pay as many as you can afford". Harness
  datum (notes §1-2), not a guide item.
- Teferi CAST at t11 (s25) — the blue splash line works when the mana is real. s37 "Cast
  nothing" at t13 with opp at 1 and lethal on board = exception (b), adherent.

### C vs36 (artifacts) — won t16 (13/-13)
- Deploy/attack floors clean; FA both copies cleared the Yotian Soldier wall (s24, s33) which
  the escalating Wolf attacks then rode to the win — defensible "best available target" picks
  (their board was Myrs + Soldier + Tinviper).
- **s12 (t5): Intrepid Adversary blocks Yotian Soldier (1/4) — FAILURE, third instance of the
  class.** Render: `(your blocker dies, attacker lives)`. Blocked anyway to stop 1 damage;
  lost the 3/1 lord, attacker survived. No first strike involved — plain toughness math this
  time, same override of the printed outcome.
- s41 (t13): Katilda trades with a Silver Myr (`both die`) at 19 life vs 4 total damage —
  minor divergence (the guide's own mana engine swapped for a mana dork with no life pressure);
  low impact, noted for the Katilda entry.

### C vs158 (amass) — **LOST t13 (-8/8), the arm's one loss**
- s1 keep: floor line named Fateful Absence → KEEP, adherent (mono-white 3-lander; the loss's
  root is that the deck then drew ZERO green sources all game — hand at s19 held Katilda,
  2×Ranger Class, Tracker, all green, forever dead; lands drawn were Plains/Hengegate(W/U) only.
  Variance, not chargeable; the mulligan rule was right to keep).
- s6 (t4): FA on the Orc army — their best creature, adherent; deck158 simply amassed a new
  9/9 by t10.
- s19 (t11): BLOCKS: none, Cathar vs the 9/9 at 12 life → 3, NOT lethal — **adherent-leaning
  and well-argued** (chump loses Cathar AND returns its exiled Orcish Bowmasters; trace works
  exactly that). Note the trace explicitly REJECTED plan-preservation logic and argued from the
  board — the arm-B plan-inertia motif did not appear.
- **s22 (t12): ATTACK: Elite Spellbinder + Brutal Cathar at 3 life — FAILURE (poor audible),
  the game-loser.** Both creatures attacked; the untapping 9/9 (+ Crebain) swung back into an
  empty board; dead at t13 (-8). The trace checked only "can they block my attack" ("Orc army
  tapped, neither can block") and never ran the return swing. Correct line: Spellbinder alone
  (unblockable, opp 13→10), Cathar stays home to chump. Rule 2's floor ("attack with everything
  that profits... keep swinging") endorses the misplay by omission — there is no low-life
  return-swing check anywhere in the guide.

**Arm C totals**: ~220 scored decisions. Divergences: **6** — 1 justified (FA-on-Grazer),
2 trivial (one Flip-Side decision, Katilda-for-Myr), **3 failures in one class** (blocks into
the printed `your blocker dies, attacker lives` outcome: vs146 s14, s21; vs36 s12) **plus the
return-swing attack failure** (vs158 s22). Comprehension: zero mana/mulligan/protocol failures;
plan usage healthy (memo-and-verify at every ask; no stale-plan-as-reason instance found in a
corpus-wide grep of all six seat files).

## The arm-C failure fingerprint (what the revision targets)

1. **Printed-trade override (3×, 2 games, one kill of the deck's best card).** The blocker
   render already computes the outcome (first strike, deathtouch, toughness included); the FP8
   pilot reads it and stochastically overrides it with its own combat math — once literally
   calling the render "a template error". s10-vs-s14 in the SAME game proves this is a
   belief-vs-surface coin flip, not missing knowledge → belief-correcting guide clause + the
   named Triumphant Adventurer teach (engine-verified: `abilities=deathtouch`, first strike on
   its turn, per `borderline.txt`).
2. **No return-swing arithmetic at low life (1×, game-losing).** Countable brake added inside
   rule 2 with scope (life ≤ their biggest creature's power) and releases (lethal-now, or life
   above the number).

## Seat-focus items from the brief

- **Plan-inertia ("the plan is a note, not a rule")**: arm B cited stale PLAN text as
  load-bearing twice, both in losses (tier review: B-vs105 s21 "must NOT block to keep the plan
  valid"; B-vs139 s22). Arm C uses the plan constantly and PRODUCTIVELY (verify-then-act), so a
  suppressive rule would churn working behavior. Adopted the brief's line in its narrow form: a
  two-sentence paragraph — plan is a memo, re-derive from CURRENT SITUATION, plus one
  quote-and-forbid falsity fact ("the thought 'I must not do X because it would break my plan'
  is always false"). Positive rule first, interception second, per the negative-framing ruling.
- **FA-sequencing text validated at arm A** (A-vs105 s12-13 infect-on-sight, A-vs36 s15-16
  best-creature, A-vs139 justified audible; C repeats all three shapes) — **kept verbatim.**
- **Generic-blockers-hint contradiction (docket)**: for deck152 the generic hint ("block only
  where the trade favors you; taking damage while ahead is often correct") AGREES with rule 4 —
  no contradiction at the normal seams. It contradicts only the INFECT posture (arm B's vs105
  s21 no-block at 2-of-10 poison leaned on exactly the race framing). Resolved guide-side: one
  scoping sentence inside the infect block stating the hint is about LIFE and never applies to
  [infect] attackers.
- **False teach found and fixed (HL3.3 class)**: the Flip-Side section claims flipping "changes
  nothing about what you can play". For the deck's TWELVE Pathway lands this is false — the
  render itself (vs158 s21 option 2) now explains: "only the currently-shown face can actually
  be played as a land", i.e. Flip Side then Play Land plays the OTHER face
  (primitive: `anyzone={0}:doubleside(...)` + flipped-gated `autohand`). The deck is a
  three-colour splash deck whose colour screw is its main loss mode (the vs158 loss); the
  rewrite keeps NEVER-flip for the creature DFCs (true — transform-only) and teaches the
  narrow land recipe with faces named (Branchloft {G}/Boulderloft {W}, Barkchannel
  {G}/Tidechannel {U}, Hengegate {W}/Mistgate {U} — verified in `borderline.txt`).

## HL1 hard gate — retroactive card-fact audit (entire deployed guide)

Checked every cost/P/T/ability claim against `Res/sets/primitives/borderline.txt` + arm-C
renders: Intrepid {1}{W} 3/1 lifelink ETB-pay ✓; Sigarda {1}{G}{W}{W} 4/4 flying trample,
Humans +1/+1, coven attack-dig ✓; Katilda {G}{W}, prot-Werewolves, Humans-tap, {4}{G}{W},{T}
mass counter ✓; Luminarch {1}{W} ✓; Ranger Class {1}{G} / L2 {1}{G} / L3 {3}{G} ✓; FA {1}{W}
instant, creature-or-planeswalker, Clue ✓; Brutal Cathar {2}{W} 2/2 exile-until-leaves ✓;
Moonrage Brute 3/3 first strike ward-3-life ✓; Huntmaster {4}{G}{G} 6/6 two Wolves /
Packleader 7/7 wolves on enter-or-attack + {2}{G}{G} fight ✓; Teferi {2}{W}{U} ✓; Deserted
Beach {W}/{U} ✓; land split 25 = 14 G-front / 15 W-front, 4 Farmlands both ✓ (recounted from
deck152.txt). **One FALSE teach found**: the Flip-Side "changes nothing about what you can
play" claim as applied to Pathways (fixed above). New named teach verified: Triumphant
Adventurer deathtouch + first-strike-on-its-turn 1/1 ✓. The teaches FIRED correctly elsewhere;
after this revision the teaches are TRUE.

## Instruments / fallbacks (arm C, this seat)

2 recorded fallbacks / ~220 decisions: s32-vs105 `unparsed_reply` (numberless hallucinated-cast
answer at a noise-only upkeep window), s38-vs146 `stale_echo` (correct safe-direction catch).
Plus one INVISIBLE-to-fallback event: the s19-vs139 echo/index conflict (Mechanism A — echo
named an offered option, index executed a different one; HL11.4's named-choice cross-check
catches it). All three in notes.md with repros.

## Verdict & rotation observation

Guide layer: the deployed guide's named rules were executed at very high fidelity by the FP8
pilot (mana trust, deploy floor, FA sequencing, infect posture, day/night, mulligan floor all
clean); its failures cluster in TWO unwritten spots (trade-parenthetical trust, return-swing
check) plus one false teach (Pathway flips) — all addressed in this revision as narrow adds to
otherwise frozen text (turnaround-preservation mode; obeyed lines untouched).

Rotation: NOT ready to exit. Adherence is mature (A 99%, C ~97% with a single dominant failure
class), but this wave AUTHORS two new load-bearing teaches (trade-trust, return-swing) and one
falsity-fix (Pathway flip) — each needs its first at-scale validation, and the trade-trust
clause is a belief-correcting crutch that carries a 2-corpora bar by the wave-23 rule. Project
exit after one clean validation corpus if the block-into-printed-death class reads 0 and the
flip teach produces no flip-loops.
