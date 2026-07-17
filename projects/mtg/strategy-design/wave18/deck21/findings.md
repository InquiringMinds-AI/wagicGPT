# deck21 (Goblin Gang swarm aggro) — REVISED-GUIDE ROUND 2 VALIDATION findings (wave-18)

Corpus: `matchups-20260716-185930`, binary `/tmp/wagic-f664539a3`. deck21 plays its SECOND
corpus under the wave-16 race-math exception, now carrying the WAVE-17 REVISION (the POWER FLOOR
on the race-math flyer clause + the kill-on-sight-ENGINE sharpening). This round VALIDATES both
of those additions. Card rules text below is derived from translog option/board strings (engine
reality — see notes.md), not real-MTG memory.

## Record: 3/6 (up from 2/6). WON vs 62, 110, 135 · LOST vs 35, 49 · TIMEOUT/DRAW vs 14.
## Verdict: BOTH wave-17 revisions VALIDATED on the exact matchups that motivated them. STRUCTURAL floor holds; CLEAN core execution; NO taught class regressed. Guide is CONVERGED → freeze-and-rotate.

| opp | result | how it ended | class |
|-----|--------|--------------|-------|
| 62 (Enchantress/BoP) | **WON** my11 op-1 T10 | raced op 20→-1, Birds of Paradise present as a burn target 4x, ALL sent face | **POWER-FLOOR VALIDATED (flip: w17 LOST −1)** |
| 110 (Affinity)       | **WON** my16 op-2 T15 | killed Signal Pest (battle-cry) + Vault Skirge (lifelink flyer), then raced | **kill-on-sight VALIDATED (flip: w17 LOST T6)** |
| 135 (snow ramp)      | **WON** my20 op-3 T9  | 20–0 blowout, never took damage; 2 benign unparsed fallbacks | clean win |
| 35 (Slivers)         | **LOST** my-15 op9 T11 | 0 creatures until T8; Slivers went wide, first-strike trample alpha for 31 | STRUCTURAL flood + sliver-scaling (same as w17) |
| 49 (Dragons/RG)      | **LOST** my0 op11 T11 | **killed Dragonmaster Outcast on sight T2**; then gas-out (0 creatures T8+) | **kill-on-sight FIRED**; loss now pure flood-variance |
| 14 (Deep Blue)       | **TIMEOUT** my5 op5 T16 | face-burn all game, clever wither blocks; couldn't close tempo-control | STRUCTURAL stalemate (long game = non-win) |

**Headline:** the two open revisions from wave-17 BOTH validated on the SAME matchups that
produced their motivating misfires. vs62 (the BoP-misfire loss) FLIPPED to a 12-life win with the
model quoting the power floor verbatim and sending every burn face past a legal Birds-of-Paradise
target. vs49 (the Outcast kill-priority miss) saw the model KILL the Dragonmaster Outcast on sight
at T2 — the exact card it failed to kill last wave — and vs110 (the Affinity structural wall)
FLIPPED to a win on two correct engine/lifelink kills. Both losses this corpus are flood-variance
or sliver-scaling (structural), and the vs14 draw is the beatdown's structural inability to close a
tempo-control deck. No decision-driven loss remains. This is a CONVERGED guide.

## STEP 0-ter: DIED / PERSISTED / MUTATED (per taught class, seq repros)

### DIED — the two open failure classes closed by the wave-17 revision

- **0-POWER-FLYER MISFIRE (w17 E1'): DIED. Power floor VALIDATED on the exact card + matchup.**
  vs62 is the same opponent whose Birds of Paradise (0/1 flyer) drew a wasted Volcanic Hammer last
  wave (a game lost by 1). This corpus, BoP was a **legal burn target on four separate casts**
  (s6, s10, s14, s17 all list "Birds of Paradise" in the target set) and the model sent **every
  burn to the FACE**, each time explicitly citing the power floor:
    - s6: *"Do not target the Birds of Paradise (0 power) with burn as it deals no damage; focus on the face clock."*
    - s7: *"Following the strategy guide, Birds of Paradise is a 0-power flyer that deals no combat damage, so it is not a valid target for race-math removal."*
    - s10: *"I will ignore the Birds of Paradise (0 power, no race threat)…"*
    - s17: *"I must not waste the burn on the 0-power Birds of Paradise or the 0/1 Argothian Enchantress, as they deal no combat damage."*
  Result: op 20→-1, deck21 WON at 11 life — a 12-life swing from last wave's −1 loss. The power
  floor also **generalized** unprompted to a second 0-power body (Argothian Enchantress 0/1 shroud,
  s10/s17), which the model declined to burn on the same power=0 reasoning. This is a textbook
  first-revision-tightening validation: same matchup, same card, opposite (correct) decision,
  opposite (winning) result.

- **KILL-ON-SIGHT ENGINE UNDER-VALUATION (w17 secondary signal, vs49 Outcast): DIED.**
  The sharpened engine clause ("kill it EARLY, even a bare 1/1, BEFORE it comes online… 'No
  immediate advantage' is a trap") fired on the exact card it missed last wave:
    - **vs49 s6→s7 (T2, my20 op19):** the model Lightning-Bolted the **Dragonmaster Outcast (1/1)**
      on sight, quoting the guide: *"My strategy guide explicitly states to burn engines worth more
      dead than 3 face damage, especially token makers… I cannot afford to let this engine
      resolve."* It killed it at **two** lands — long before the 6-land dragon condition — exactly
      the "don't wait for it to do something first" behavior the wave-17 sharpening added. Last
      wave this same Outcast lived, took War Paint → 3/3, and anchored the lethal clock that lost
      the game by 3. Closed.
  The clause also fired correctly TWICE more vs110:
    - **s3→s4:** bolted **Signal Pest (0/1, battle cry)** as an attack-anthem engine / Cranial-Plating
      enabler in a deck it cannot block (a defensible engine-kill — see the one reasoning nit below).
    - **s12→s13:** bolted **Vault Skirge (1/1 flying, lifelink)** — *"remove the lifelink engine
      immediately so my damage isn't negated"* — a clean lifelink+flyer kill-on-sight.
  These two kills removed Affinity's snowball pieces and FLIPPED the w17 structural loss to a win.

### PERSISTED (working as taught — no regression, a full wave later)
- **burn-as-clock / face-by-default (w15 F1a): STILL DEAD.** Every burn fired face on deck21's own
  turn across all six games (vs14 s6/s10/s12, vs35 s10/s12/s19, vs110 s15/s23, vs135 s8/s13/s15,
  vs62 s7/s11/s15/s18/s20, vs49 s9/s13). Zero holds "for interaction."
- **burn-at-toughness>3 (w15 F1b): STILL DEAD.** No burn thrown at an uncastable-toughness body.
  Walls correctly ignored (vs135 s16/s18 the model reasoned "can't kill the 6/6 / 8/8 Treefolk,
  focus the race"; vs35 declined to Hammer the 2/2 blocker for tempo).
- **Spark Elemental MP1 timing (w15 F2): STILL DEAD.** Every Spark cast in Main phase 1 (vs135 s3,
  vs35 s6, vs49 s18) — no MP2 or held-and-wasted Sparks.
- **haste roster + summoning-sick tokens (w15 F3): STILL DEAD.** Raging Goblin / Boggart Ram-Gang
  cast-and-attacked; Gang Leader not miscounted.
- **anthem / first-strike math (w15 F4): PERSISTED.** Bloodmark Mentor and Boartusk Liege deployed
  as bodies; wither-block tech vs14 (Scuzzback shrinking the Merfolk Lords via -1/-1) shows the
  model tracks keyword interactions.
- **hybrid {R/G} affordability (w15 F5): STILL DEAD.** ~12 hybrid-annotated prompts, 0 declines.
- **don't durdle (w15 F7): PERSISTED.** Every "Cast nothing" was legitimate — no castable creature
  (vs110 s7 hand = burn+enchantment only), an instant held on the opponent's turn (vs110 s10), or
  a mana-constrained turn (vs35 s7). No empty-board durdles while a creature was castable.
- **race-math FAST-RACE guard (w16): STILL HELD.** No over-application; in winnable races the model
  went face (vs135, vs62) rather than chipping flyers.
- **blocking = default-none (guide BLOCKING section): VALIDATED.** vs14 escalated 4 blocker asks:
  2 correct "no blockers" (s16/s24) and 2 deliberate wither-blocks (s28/s32) that permanently
  shrank the opposing Merfolk Lords — a sound tempo use, not a chump-panic.

### MUTATED → nothing. No taught class mutated into a new failure shape this corpus.

## STRUCTURAL vs DECISIONS: the 3/6 is 3 clean wins + 2 structural losses + 1 structural draw.
- **vs35 (LOST −15): STRUCTURAL flood + sliver-scaling.** deck21 had **0 creatures until T8**
  (creature-light draw), burned face correctly to op12, then the Slivers went wide (6 bodies,
  Horned Sliver pumped 2/2→5/3 first-strike trample) and alpha'd for 31. Same class as w17 vs35 —
  a construction/variance loss, not a decision error.
- **vs49 (LOST 0): kill-on-sight FIRED CORRECTLY, then flood-variance.** The Outcast kill (the w17
  decision miss) is fixed; deck21 got op to 11 by T4 then had **0 creatures from T8 on** (5
  Mountains, no gas) and was ground out. The loss is now purely a gas-out draw, NOT a kill-priority
  error. This is the clearest proof the sharpening worked: the decision that lost the game last
  wave was made correctly this wave; the seat still lost for an unrelated structural reason.
- **vs14 (TIMEOUT 5–5): STRUCTURAL stalemate.** Aggro cannot close a mono-blue tempo/control deck
  that trades and bounces; a 16-turn draw is a non-win by the guide's own "long game = lost game."
  Execution was clean (face-burn, wither blocks). Not a decision failure.
- **vs62, vs110, vs135 (WON): clean, two of them validated flips.**

No decision-driven loss remains. The record floor is entirely structural (Slivers-scaling, flood
variance, control-stalemate) with clean, now-fully-validated core execution.

## Reasoning-tax / fallbacks
4 fallbacks / 119 asks (3.4% — matches corpus best-tier), ALL benign and ALL in ALREADY-WON games:
vs110 s30 (unparsed_reply) + s31 (stale_echo) at T15 with opp already at 2 (deck21 winning that
turn); vs135 s16 + s18 (unparsed_reply) at T7/T9 with opp at 7→4 (won 20–0). All four are the same
**giant-reply pocket** — the model over-analyzed an opposing high-toughness body (the 6/6→8/8
Treefolk, its own mana/hand state) and blew past the CHOICE line. The reasoning in each was
CORRECT ("can't kill the wall, race the face"); only the emission failed, and the heuristic
covered it in games already won. No stale-plan spirals, no scavenged casts, no unpayable-hardcast
menus. See notes.md (engine item) and general-suggestions (this is the same computed-P/T-distrust
pocket the deck62 seat carries).
