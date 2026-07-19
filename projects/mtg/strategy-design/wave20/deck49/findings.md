# deck49 (Dragons) — wave-20 findings (REVISED-GUIDE, ROTATION-HOLD, streak broke)

Mono-red Dragons aggro, third guided corpus. 6 games, **4/6** (W vs deck102, deck62, deck35,
deck135; L vs deck27, deck14). Binary 95e89c204 (wave-20 engine batch: reveal-partition fix,
attacker-sparing payment, perception bundle incl. salvage extension to BLOCKS/ATTACK/PUT + `#N`
ordinals + combat options_text). 122 decisions this seat (ask 92, attackers 18, blockers 5,
gamestart 6, gameend 5 — vs135 logged no gameend).

Step-0-ter posture: judge by taught-class survival + tax, not the record (rule 1). The record fell
5/6->4/6 but that is POOL (deck27 and deck14 flipped from W last wave to L; both are construction/
matchup losses, below). The real story is the two-wave ZERO-FALLBACK STREAK BREAKING: 3 unparsed
replies, ALL at ask seq 8, vs three different opponents.

## HEADLINE — the ask-seq-8 triple: ONE develop-seam trigger, THREE surface mechanisms

All three fallbacks are `kind==ask`, **seq 8**, Main phase 1, YOUR turn, turn 4-5, **at 20/17 life**,
lone-1/1 board, burn (and/or a cheap creature) in hand. Same seam. None was loop-salvaged
(chosen_text=None all three; the baka heuristic took the turn).

### vs deck27 (seq8, t5, the LOSS's game) — DECODE-REPETITION LOOP
- Menu: `1. Cast Lightning Bolt {r} - legal targets: Bloodmark Mentor, you, Zombie Outlander, the opponent  2. Cast nothing`
- Board: lone Bloodmark Mentor (1/1) vs a Zombie Outlander (2/2). Opp 17.
- Reply ~300 lines. The model oscillates between the guide's **"DEFAULT: aim ... at THE OPPONENT"**
  and **"Only aim burn at a CREATURE when ... a key blocker walling your whole board"**, deciding a
  2/2 that can block its lone 1/1 IS such a key blocker. It repeats the SAME paragraph ~10x verbatim
  ("Wait, the guide says DEFAULT ... So I should kill it ... I will cast Lightning Bolt at the Zombie
  Outlander") and truncates mid-word: `I will cast Lightning`. Decode-repetition loop (tail REPEATS).
- **The model cites BOTH guide lines and loops between them.** Step-0 echo test: the guide's own
  competing wording caused the loop = a guide bug, not a pure model bug.

### vs deck62 (seq8, t5, a WIN) — PROGRESSING REASONING-SPIRAL
- Menu: `1. Cast Lava Spike (face/you)  2. Cast Dragonmaster Outcast (1/1)  3. Cast nothing`
- Reply ~260 lines. Not verbatim repetition — it ADVANCES, re-deriving the mana count 4x ("I can cast
  Thunderbreak" -> "I CANNOT cast Thunderbreak", settling on 3 mana) and re-litigating **DEPLOY a
  creature (Dragonmaster) vs BURN-to-face (Lava Spike)** — "deploy and attack every turn / cast the
  best creature you can afford" vs "burn is your clock, send it face." Truncates at `Wait, I need to
  check the PLAN`. Progressing spiral (a guide tiebreak CAN reach this; a repetition penalty cannot).

### vs deck35 (seq8, t4, a WIN) — PLAN/CHOICE MISMATCH (wrong menu, out-of-range index)
- Menu was a **LAND-DROP**: `1. Play Mountain  2. Hold Mountain`. (The prompt's `YOUR PLAN` field was
  contaminated with the model's prior casting-menu deliberation echoed back into it.)
- Reply is SHORT (not truncated): PLAN "Play the Mountain ... THEN cast Dragonmaster Outcast," then
  `CHOICE: 5 (Cast Dragonmaster Outcast)`. The model looked PAST the land-drop it was asked and
  answered the FOLLOW-ON cast — index 5 is out of range (menu has 2) and the name matches no option,
  so nothing to salvage -> unparse. Same deploy-vs-develop tension, collapsed into the wrong decision.

### The unifying diagnosis
ONE trigger: the **early-development Main-1 seam** (turn 4-5, ~20/17, lone-1/1 board) where the guide
presents co-firing imperatives with **no tiebreak** — (deploy the creature) vs (send burn to the
face) — plus a burn EXCEPTION ("kill a key blocker walling your whole board") whose wording lets a
small chump qualify when your board is a lone attacker. A weak executor with no priority order
deliberates; the deliberation LOOPS (deck27), SPIRALS (deck62), or COLLAPSES into the wrong menu
(deck35). Same seam, same missing-tiebreak root, three surfaces.

### Cost was ~0 games — the multi-decision turn structure absorbed it
In ALL THREE the engine issued FOLLOW-ON cast decisions the SAME turn and the model recovered:
- deck27: seq9-11 threw BOTH Lightning Bolts at the FACE (opp 17->14->11).
- deck62: seq9-12 cast TWO Lava Spikes at the FACE (17->14->11); won.
- deck35: seq9 cast Dragonmaster Outcast, seq10 War Paint on own creature, attacked; won.
The seq-8 fallback ate at most ONE cast/land decision and decided NO game (deck27 was lost much later
to running out of gas). The harm is the TRUNCATION (a reasoning-tax regression), not a misplay.

### Loop-salvage did NOT fire — and for deck27 that was arguably CORRECT
deck27's looped intent was "bolt the Zombie Outlander" (a creature) — the WRONG resolution; the
fallback+follow-on sent the burn to the FACE (the RIGHT one). Salvaging that loop would have LOCKED IN
the play the model was talking itself out of. Argues AGAINST extending loop-salvage to
genuinely-unresolved ask/target deliberations (see notes).

## FLOOD / NO-CURVE OFF-CASE — the HOLD's EXIT QUESTION: **EXERCISED this corpus**

Wave-19 left the off-case ("WHEN YOUR CURVE FAILS YOU, THE BURN IS YOUR DECK") UNEXERCISED, the sole
reason for the one-cycle hold. **This corpus exercised it.**

- **deck27 (the LOSS) is the pure off-case game.** Opening hand: 3 Mountain + Bloodmark Mentor (1/1) +
  Lava Spike + 2 Lightning Bolt — lone-1/1-plus-burn, NO dragon. The condition held ALL game (never
  drew a dragon; rode a lone Bloodmark Mentor 1/1).
- **The off-case prescription was FOLLOWED.** Every burn went at the FACE, none held: Lava Spike t1
  (20->17), both Lightning Bolts t5 (17->11). By turn 6 opp chipped to ~10 with burn + the 1/1,
  exactly as the off-case directs. It did NOT sit on burn.
- **The loss is CONSTRUCTION, not the off-case.** Spent all burn by t5, stalled the clock at opp
  10-11, ground 20->-17 over t6-11 with an empty hand ("Cast nothing" x4, seq16/17/19/20) — the
  top-heavy-curve/no-reach shape (G-49d).

Verdict: **off-case VALIDATED (pass on behavior).** The one wart: at deck27 seq8 the model did not
CLEANLY obey "throw burn at the face" — it agonized face-vs-blocker (the loop) before the burn reached
the face. That is the burn-EXCEPTION over-breadth leak (above), not the off-case failing. (The
condition also appeared at OPEN in deck35 but that game CURVED OUT — Dragonmaster t5 + Thunderbreak
t6 — so not a sustained-flood test.)

## WHICH GUIDE RULES FIRED (the audit) — 4/6 context vs last wave's 5/6

- **BURN-GOES-FACE: ~16/16 face, 0 creature-targeted (cleaner than wave-19's 15+1).** deck102 (Lava
  seq6 + Bolts seq8/12/14), deck27 (Lava seq3 + Bolts t5), deck14 (Bolts seq13/17), deck135 (Lava
  seq4/11 + Bolt seq13 — Icehide Golem was an available target, chose face), deck62 (Lava seq10/12),
  deck35 (Lava seq5 + Bolt seq20). ZERO creature-targeted burn. The one temptation (deck27 seq8) was a
  TRUNCATION, never an executed misaim — face-rate stays perfect.
- **never-aura-opponent / War-Paint-own: 3/3 own, plus the OWN-ONLY annotation surface.** deck14
  seq8/9 (mixed menu -> own Bloodmark), deck35 seq10/11 (-> own Dragonmaster), deck62 seq5 cast under
  the NEW annotation "the only legal targets are YOUR OWN right now" (mirror of the wave-19 enemy-only
  warning). All correct.
- **Crucible-needs-dragon: FIRED as a correct decline in a desperate spot.** deck14 seq25/26 (t12), 6
  untapped lands, Crucible in hand, 0 creatures — cast NOTHING rather than drop Crucible into an empty
  board. Held even with nothing else to do.
- **deploy-and-attack: obeyed everywhere eligible.** Multi-attacker declares in deck102/135/35/62; no
  0-attacker durdle. The deck14 t8-12 "gap" is NOT a durdle and NOT engine non-issuance — the opponent
  STOLE Thunderbreak Regent with Persuasion (control aura; final board shows it on the OPPONENT's side
  "attached: Persuasion"). deck49 was creatureless, correctly nothing to attack with. (Wave-19
  instrument headline-2: eligible untapped attacker? NO — under opponent control. Not a durdle.)
- **Rule #1 mana-trivial: held, with a contained resurgence inside the spiral.** 0 play-changing
  fabricated locks. But deck62 seq8 re-derived the mana count 4x before settling correctly on 3. The
  fork spiral RE-OPENS mana re-derivation; self-corrected, but a symptom of the same paralysis.

## The two losses — both construction/matchup, neither a decision defect
- **deck27 (L, -17):** off-case game, ran out of gas / no reach (G-49d). Above.
- **deck14 (L, -1/8):** CLOSE. Deep Blue control STOLE Thunderbreak via Persuasion and ground deck49
  out; ended with 6 Mountains, 0 creatures, a dead Crucible. Burn all face, War Paint correct, blocks
  reasonable. Matchup (threat theft) + the same no-reach tail. Routes to construction (G-49d family).

## Residual reasoning-tax — RELOCATED across the token cap (the regression)
Wave-19: 13/142 replies >=600 words, ALL parsed (0 fallbacks). Wave-20: the SAME tax class (combat/
card-value + stale-plan spirals, G-49a) at the develop-fork seam crossed the token cap and produced 3
TRUNCATED unparses. Not a new behavior — the same G-49a spiral, now with a worse consequence because
it hits an unadjudicated fork. That is why the streak broke. The guide can lower the TRIGGER frequency
(dissolve the fork); the loop/spiral tendency itself is CORE/decode (general-suggestions + notes).
