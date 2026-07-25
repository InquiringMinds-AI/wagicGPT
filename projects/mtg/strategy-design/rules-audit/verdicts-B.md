# Rules-variance verdicts — SET B (5xx turn structure)

Runtime probes for the five worker-B findings. Each fixture **asserts CR-correct
behavior** and is run on the current `bin/wagic` (built 2026-07-24 12:35). A FAIL =
**PROVEN** variance (the engine did the CR-wrong thing); a PASS = **REFUTED** (or a
control). Every FAIL was log-verified to fail for the *claimed* reason (fixture-bug
vs engine-bug discrimination). Synthetic cards live in
`bin/Res/test/rules-variance/_var_B_primitives.txt`; the registry is
`bin/Res/test/_var_B.txt`.

**Run command** (from `projects/mtg/bin/`):
```
env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 \
  WAGIC_TESTSUITE_THREADS=1 WAGIC_TESTSUITE_FILE=test/_var_B.txt \
  WAGIC_TEST_PRIMITIVES_FILE=test/rules-variance/_var_B_primitives.txt ./wagic
```
`WAGIC_TEST_PRIMITIVES_FILE` is **required** — every fixture uses synthetic cards.
Batch result: **6 tests, 3 failed, 0 timeouts** (the 3 fails are the 3 PROVEN ids).

## Verdict table

| # | Finding (CR ids) | Verdict | Fixture(s) | Observed vs CR |
|---|---|---|---|---|
| 1 | 500.1 / 501.1 / 504.1 — turn 1 deletes starting player's whole beginning phase | **UNPROBEABLE** | `var_501.1_turn1_upkeep_trigger.txt` (machinery control, PASS) | Deletion is the *real game's* `initState.phase=FIRSTMAIN`; the suite substitutes the fixture's own `[INIT]` phase, so no fixture can present a natural turn 1. Control proves the upkeep-trigger machinery itself fires (life 20→19) when the harness supplies an untap→upkeep transition. |
| 2 | 500.3 / 502.4 / 514.3 — untap & cleanup run as priority-bearing phases | **PROVEN** | `var_500.3_untap_priority_instant.txt` (FAIL), `var_514.3_cleanup_priority_instant.txt` (FAIL) | Active player cast an instant that **resolved during UNTAP** (p2 20→18) and again **during CLEANUP** (p2 20→18); CR grants no priority in either step. |
| 3 | 500.7 — extra turns as scalar counters (no APNAP / most-recent-first) | **UNPROBEABLE** | — | The single-seat script can't make the non-active player also stack an extra-turn effect, and the assert language (zones/life/phase only) can't express turn-*owner ordering*. Both required to observe the defect. |
| 4 | 511.3 — combat roles not cleared at end of combat | **PROVEN** | `var_511.3_attacking_persists_main2.txt` (FAIL); `var_511.3_never_attacked_control.txt` (negative control, PASS) | An attacker (dealt its 1 combat dmg, p2→19) was still destroyable by "destroy target **attacking** creature" **in postcombat main 2** — so the `attacking` flag persisted. Control proves the `[attacking]` target restriction is genuinely enforced (a never-attacked creature is un-targetable). |
| 5 | 514.1 / 514.2 — cleanup mandatory-action timing (damage removal / until-EOT expiry) | **REFUTED** (observable) | `var_514.2_cleanup_damage_ueot.txt` (PASS) | A 1/1 pumped to 1/3 (+0/+2 ueot) under 2 marked damage **survives** cleanup into the next turn. The engine removes damage (cleanup, phase 12) *before* ueot expiry (after-eot, phase 13), so no lethal 1/1-with-2-damage SBA window arises. The mechanism defect (514.2) is real in code, but does not surface as a divergent single-creature observable. 514.1 (hand-size discard) is `[ledger]`, not probed. |

## Evidence detail

### Finding 1 — UNPROBEABLE (500.1 / 501.1 / 504.1)
The audit locates the deletion at `Rules.cpp:822-843`: `Rules::initGame` reads
`initState.phase` (= `FIRSTMAIN` for the classic rules config) and jumps there, so a
**real** turn 1 opens at first main with untap/upkeep/draw already gone. The test
harness never runs that path — `TestSuiteGame::initGame` (`TestSuiteAI.cpp:1557-1558`)
calls `goToPhase(initState.phase, players[0], /*sendEvents=*/false)` with `initState.phase`
taken from each fixture's own `[INIT]` line. Consequently the fixture author *chooses*
the starting phase; the suite cannot present a "natural turn 1" whose beginning phase
the engine would have deleted. Choosing `[INIT] firstmain` reproduces the skip only
tautologically, and choosing `[INIT] untap`/`upkeep` *manufactures* the very phase the
real game removes.

The control fixture nails down that this is purely the phase-config gap, not a broken
trigger: `Var Bleed Sigil` (`auto=@each my upkeep:life:-1 controller`) seeded for
player1, `[INIT] untap`, one `next` (a real untap→upkeep transition, which *does* send
events) → **life 20→19, PASS**. So an upkeep trigger fires whenever an upkeep step
actually exists; the only reason turn-1 upkeep is missing in real play is the config
the suite bypasses.

### Finding 2 — PROVEN (500.3 / 502.4 / 514.3)
`var_500.3`: `[INIT] untap`, player1 holds `Var Spark Jolt` ({0} instant, `damage:2`),
`[DO] Var Spark Jolt / p2`. Trace: `Action added to stack: Var Spark Jolt` →
`Resolving Action on stack: Var Spark Jolt` → `6Damage`, all **in untap** (no phase
problem in the assert; final phase = untap). p2 20→18, spell to graveyard.
`var_514.3`: identical but `[INIT] cleanup` — same cast+resolve **in cleanup**, p2→18.
CR 500.3/514.3 grant no priority in untap or (default) cleanup, so no spell may be
cast/resolved there. 502.4's "no spells/abilities resolve during untap" is violated by
the same untap resolution.

*Discrimination (fixture-bug ruled out):* the harness does **not** blind-force casts —
it respects the engine's castability gate. Proven by Finding 4's negative control, where
clicking a spell with no legal target left it in hand. So the untap/cleanup casts
succeeding means the engine's own gate treated those steps as valid priority windows.

### Finding 4 — PROVEN (511.3)
`var_511.3_attacking_persists_main2`: `Var Brute Runner` (INIT 1/1) attacks unblocked
(p2 20→**19**, log-confirmed via matched life assert), combat completes, and in
**SECONDMAIN** player1 casts `Var Slay Charge` (`target=creature[attacking]`,
`auto=bury`) at it → trace shows `Resolving Action on stack: Var Slay Charge` +
`graveyard.wav`; both cards end in graveyard, battlefield empty. The creature was still
flagged `attacking` in postcombat main, exactly the 511.3 defect.
Negative control `var_511.3_never_attacked`: same spell, same creature, cast at
FIRSTMAIN before any combat → **UNCASTABLE, PASS** (creature in play, spell in hand),
proving `[attacking]` is a real restriction and the main-2 hit isn't an ignored filter.

### Finding 5 — REFUTED at the observable level (514.2)
`var_514.2`: `Var Brute Runner` (1/1) + `Var Bark Ward` (`auto=0/2 ueot`, +0/+2) →
1/3, then `Var Spark Jolt` for 2 marked damage → alive at 1/3. `eot` (auto-re-issues to
cleanup) + `next` crosses into the next turn; the creature is **in play** and the two
instants in graveyard → **PASS**. CR 514.2 wants damage-removal and ueot-expiry as one
simultaneous turn-based action; the engine splits them (damage: `@each cleanup`, phase
12; ueot: after-eot, phase 13) but in the *safe* order (damage first), so this
particular observable comes out CR-correct. A reversed order would have killed it; it
did not. The mechanism is still non-CR (a real 514.2 gap), just not observable as a
creature life/death divergence with this construction.

## Fixture/card inventory (all additive; no existing file modified)
- Registry: `bin/Res/test/_var_B.txt`
- Primitives: `bin/Res/test/rules-variance/_var_B_primitives.txt`
  (ids 2100000700/702/703/704/705: Var Bleed Sigil, Var Brute Runner, Var Slay Charge,
  Var Spark Jolt, Var Bark Ward — every ability form copied from a real card:
  Dreadhorde Invasion/Dawn of the Dead upkeep-loss, Dark Banishing `bury`+`target=`,
  Shock `target=anytarget`/`damage:2`, Giant/Mammoth Growth `X/Y ueot`).
- Fixtures under `bin/Res/test/rules-variance/`:
  `var_501.1_turn1_upkeep_trigger.txt`, `var_500.3_untap_priority_instant.txt`,
  `var_514.3_cleanup_priority_instant.txt`, `var_511.3_never_attacked_control.txt`,
  `var_511.3_attacking_persists_main2.txt`, `var_514.2_cleanup_damage_ueot.txt`.

Left uncommitted per the campaign contract.
