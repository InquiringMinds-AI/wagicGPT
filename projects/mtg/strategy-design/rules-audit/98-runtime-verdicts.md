# Runtime variance verdicts — gameplay-proof pass over the audit's INCORRECT list

Owner order (2026-07-24): "make tests to prove variance between rules behavior and gameplay
using the engine. if it's impossible to prove with gameplay, then it's probably not something
we should focus on now."

Method: for each audit INCORRECT finding (99-summary.md §2), a fixture asserting the
CR-CORRECT behavior was run on the current engine. **FAIL = variance PROVEN in gameplay.
PASS = claim REFUTED (engine conforms in the probeable case). UNPROBEABLE = the harness/card
pool cannot express an observable — deprioritized per the owner's ruling.** Every PROVEN
fixture was log-verified to fail for the claimed reason; REFUTED verdicts carry positive
controls proving the observable was live. Fixtures live in `bin/Res/test/rules-variance/`
with per-set registries `_var_A..E.txt` (run each with its own `_var_X_primitives.txt` via
WAGIC_TEST_PRIMITIVES_FILE — the synthetic id ranges overlap between sets, do not merge the
registries). Per-set details: verdicts-A.md … verdicts-E.md.

## PROVEN — variance demonstrated in gameplay (6 findings) — **ALL SIX FIXED (2026-07-25)**

Status update: every finding in the table below was FIXED in the follow-up engine batch; each
acceptance fixture flipped FAIL->PASS and the full variance corpus (all five sets) now runs
0 failed — the fixtures are permanent regression guards. Notable fix designs: combat-role
clearing defers while another declare-attackers phase is scheduled this turn (the engine's
extra-combat cards rely on persisted attacker flags); the day/night + city's-blessing fix
excludes all six designation markers (type=Emblem) from the suite's zone accounting while
leaving the game-side detection consumers untouched; turn 1 now begins at untap with a
draw-only skip (103.8a), so turn-1 upkeep triggers fire. Eight legacy fixtures that
themselves exploited the illegal untap/cleanup priority window were re-authored to legal
windows (4 found by the fix agent, 4 more by the integration gate — click-at-assert-time
signatures).

| CR | defect (engine behavior vs rule) | evidence |
|---|---|---|
| 306.7 | Player-aimed damage redirects to an opposing planeswalker via a live menu (obsolete pre-2018 rule still active) | var_306.7_pw_damage_redirect.txt FAILS: Lava Spike at player killed Jace, player took 0 |
| 500.3 / 502.4 / 514.3 | Untap and cleanup are priority-bearing: instants cast AND resolve during both | var_5* (set B) FAIL: bolt resolved during untap (20→18) and during cleanup |
| 511.3 | Combat roles never cleared at end of combat: "destroy target attacking creature" works in postcombat main 2 | set B fixture FAILS with negative control (never-attacked creature untargetable) |
| 506.3 / 508.1a / 509.1a | A creature-Battle can attack and block (missing "is not also a battle" exception) | var_506_3_* FAIL: synthetic 2/2 Creature Battle attacked for 2 and blocked a 6/4 |
| 730.x | Day/night is a physical battlefield marker (countable permanent; carries shroud so countable-not-targetable) — CR says designation, not object | set E fixture FAILS: Night emblem object present on battlefield |
| 702.131 | City's blessing is a battlefield token (counts as a permanent) — CR says immutable player designation | set E fixture FAILS: blessing = 11th permanent in count |

Plus the **pre-proven [ledger] items** (runtime witnesses already parked from the lexicon
campaign — no new probes needed): reduceto no-op (119.5), dungeon completion (309.5b), Battle
defense-counter parse NULL (310.4b), sideboard/commandzone target moves inert (400.11b),
cleanup auto-discard hand->cards[0] (402.2/514.1-discard), unpayable alternate attack-cost
wedge (508.1h), attacked-entity primitive missing (508.3a), Recover no-op (702.59), Plot
non-castable (702.170), Gift promised-branch unreachable (702.174).

Owner correction (2026-07-24): **sideboard/command-zone targeting (400.11b) is RECLASSIFIED
off the defect list** - there is no sideboard feature (no match structure), so inert moves to
a zone that doesn't exist are a FEATURE BOUNDARY, not a gameplay variance. Its only in-game
consumers are Wish-class cards, already in the owner's parked-unimplemented ruling. The
loud-rejection doctrine applies meanwhile: scripts referencing those zones should reject
loudly rather than sit silently inert.

## PROVEN-MECHANISM, narrow gameplay surface (1)

- **500.1/501.1/504.1 — turn 1 deletes the starting player's untap+upkeep+draw.** Mechanism
  verified twice in code (Rules.cpp initState.phase=FIRSTMAIN; independently by the
  pregame-phase build); every real game's turn 1 opens at Main 1. Suite-unprobeable (fixtures
  override the phase). The ONLY in-pool gameplay observable is Leyline Surge (the sole
  battlefield-capable-before-turn-1 card with an upkeep trigger) in the starting player's
  kept hand — a 1-in-27,777-cards edge. Classified real-but-narrow; ranks below the six
  broad PROVEN items.

## REFUTED — engine produces the CR-correct outcome in gameplay (14 findings closed)

- 304.4/307.4/400.4a instant/sorcery-to-battlefield (audit mis-cited an inert helper; real
  guards exist at AllAbilities.cpp:6804/6867) · 400.7 object-memory (code copies state but the
  kicker channel produced a correctly-fresh object) · 514.1/.2 cleanup-timing observable ·
  506.4c/510.1b vanished-walker fallback · 509.1h blocked-status revert · 608.2b spell fizzle
  (dead-target spell fizzled correctly) · 608.3b aura target re-check · 611.2e enters-with
  (skipDamageTestOnce works) · **613 layers — ALL FIVE discriminators the pool can express
  converge to CR-correct outcomes (incl. Humility-vs-lord and double-P/T-swap)** · 614.7a
  zero-damage shield consumption · **704.5f toughness≤0 regeneration — FALSE INCORRECT: the
  audit missed triggerRegenerate's own toughness re-check (MTGCardInstance.cpp:742-758); the
  shield banks but correctly cannot save the creature** · 704.4/702.2b deathtouch timing
  (outcomes match CR) · 704.5m unattached auras (correctly graveyarded).

The pattern: the audit's mechanism-level readings are largely accurate, but masking paths and
per-frame recomputation convergence deliver correct OUTCOMES for everything the current card
pool expresses. These matter again only if novel custom cards express what the pool cannot.

## UNPROBEABLE — cannot be demonstrated in gameplay today (~19; DEPRIORITIZED per owner ruling)

Set A: 310.8b/c Battle protector (no machinery, Battles not even castable by harness).
Set B: 500.7 extra-turn APNAP (can't script both-player extra turns).
Set C: 508.1d/509.1c MUSTATTACK/MUSTBLOCK maximization; 510.1c/e damage division (menus not
scriptable); 510.4 first-strike snapshot (can't toggle FS between damage steps); 508.5.
Set D: 608.2e/f multi-target simultaneity; 610.3a blink-return; 610.4a phase-until (phasing
unimplemented anyway); 614.5 double-replacement; 615.7 shield allocation; 601/602/603.3
casting atomicity (ordering only observable via response windows the harness can't drive).
Set E: 704.5b library-loss timing (protection already inlined → CR-correct outcomes);
704.5j legend 3+ (entry-driven; simultaneous entry not constructible — sequential is correct);
704.5r max-counters (no scripted card); 704.5v zero-defense Battle (rides the broken Battle
cluster); 702.61 split second (control showed the harness can't tap mid-window at all).

## Bottom line

Of the audit's 127 INCORRECT rule-ids (~55 distinct defects): **16-17 defects are
runtime-proven** (6 new + 10 pre-proven ledger items + 1 narrow), **14 are refuted as
gameplay-invisible or wrong**, **~19 cannot be expressed in gameplay today**, remainder
skipped as already-catalogued. The honest near-term engine shopping list is the PROVEN table
above — headlined by the planeswalker damage-redirect, untap/cleanup priority, sticky combat
roles, creature-Battles in combat, and the day/night + city's-blessing marker objects.
