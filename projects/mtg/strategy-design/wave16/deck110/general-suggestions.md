# General suggestions (wave 16) — deck110 seat

No new CORE prompt-body candidate from the deck110 seat (core PASS, 12th consecutive wave). Guide
FROZEN (9th freeze). The wave-16 items route to HARNESS (new), ENGINE ledger (dormant), representation
(open, no exercise), and MODEL layers. Full repros in `notes.md`.

## New harness item (HIGH, cross-deck)

1. **[NEW / HIGH] The `YOUR PLAN (as you last stated it)` carry re-injects STALE/confused prior
   reasoning and caused a real, game-costing blunder.** deck21 s6->s8 (a LOSS): the carried plan said
   "cast Steel Overseer" after the Overseer was already cast; the model then (s6) emitted CHOICE:1 =
   Cast Galvanic Blast (the action its own prose rejected) and (s7) selected the option NAMED in the
   stale plan — its OWN Steel Overseer — so the metalcraft Blast (2+2=4) killed its own just-cast
   creature. Same stale-carry mechanism seeded the two ~14k-char `unparsed_reply` spirals (deck21 s13,
   deck133 s9, each where the carry contradicted the current menu/mana line) and the prose-vs-CHOICE
   divergences (deck135 s15). Owner: `AIPlayerGPT.cpp` prompt assembly. FIX: when the carried plan's
   named action is not on the current menu (state advanced), drop/truncate the carry — or carry only
   the last resolved `CHOICE:` line, not the full PLAN including its "Wait..." tail. NOT guide-fixable.
   Cross-deck (every seat carries a plan); deck110 gives the cleanest repro with an outcome cost.

## Shipped-fix validations (close/hold the ledger)

2. **[RESOLVED / standing] Galvanic Blast metalcraft correct on both arms — 11 casts, 0 errors.** 2 at
   N<3, 4 at N>=3 across the whole corpus (COUNT `Damage:` instances). Finisher CLOSED. Cross-deck to
   any other Galvanic/metalcraft-oneshot consumer, verified by the two-instance log shape. Not a guide edit.

3. **[RESOLVED / standing] Attackers name->index reconcile works.** 17 windows; no eligible attacker
   dropped; MIXED forms unioned, summoning-sick names excluded. Not a guide edit.

## Engine / representation ledger (cross-deck, still open — NO exercise this wave)

4. **[MEDIUM / dormant] Equip re-offer no-op marker should be OUTCOME-DELTA-computed, not
   target-identity (E2, unfixed).** Did NOT recur this wave (all equips were first-equip, an upgrade
   move, or a post-death rebuild; 0 `(ALREADY)` options taken). The lever is validated-by-absence — it
   would correctly ALLOW deck135 s15's upgrade and still suppress the wave-15 downgrade-bounce. Owner:
   `AIPlayerGPT.cpp` equip-option serialization. Hold.

5. **[LOW-MED / representation, open] "Cast Card Normally with Mox Opal" mislabel (E6).** Not exercised
   — no cast-with-mana-source option shape appeared this corpus (all Mox interactions were plain
   `Cast Mox Opal`, clean). No confirm/deny from this seat this cycle; ledger stays open. Owner:
   `AIPlayerGPT.cpp` cast-with-mana-source serialization (show the CARD, not the rock's own text).

## Model-layer items (not guide-fixable; cross-deck synthesis questions)

6. **[MODEL WATCH] Summoning-sickness still not tracked — the model names freshly-cast creatures as
   attackers.** deck135 s6 named `A1, Steel Overseer` (Overseer ineligible); the reconcile excluded it,
   harmless. Latent risk if it ever gates a lethal/hold-back. Cheap harness mitigation if it bites:
   board-line tag `[can't attack yet — summoning sick]`. Cross-seat synthesis question (aggro seats).

7. **[MODEL] Reasoning-tax / ramble rate elevated but non-biting (~5.6% fallbacks; deck17 hotspot at 3
   unparsed).** All heuristic-answered, none a lethal miss; the only spiral with a downstream cost was
   deck21's (item 1). The stale plan-carry (item 1) is one amplifier; the rest are metalcraft-recount
   (E3), mana-availability misreads, phase confusion, cast-vs-target. Fixing item 1 should cut the loop
   rate. Latency/timeout risk under Spark @0.75 (no timeout occurred this corpus).

## Deck construction (not a guide task)

8. **deck110's loss column is owned by draw variance / matchup, not obedience.** Both losses:
   deck133 = one threat removed (Inquisition/Fatal Push) + drew 4 lands in a row = flood vs attrition;
   deck21 = land-heavy draw vs fast red + the item-1 self-blast. Standing DC1. If ever tuned: trim
   ~0-power filler + Mox/land toward mid bodies + reach/evasion redundancy to cut flood + single-threat
   + wipe/race exposure at once. NOTE (Finding 7): do NOT tighten the mulligan — all 6 hands this wave
   were 1-creature keeps and the deck went 4/2, so shipping 1-creature hands would ship 4 winners.
