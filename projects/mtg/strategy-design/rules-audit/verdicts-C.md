# Runtime-variance verdicts — set C (5xx combat findings)

Probe author C of the rules-audit variance campaign. Per finding, a fixture that
asserts **CR-correct** behavior on the current `bin/wagic`. **FAIL = PROVEN**
(the engine contradicts CR), **PASS = REFUTED** (the flagged code path does not
misbehave observably), **UNPROBEABLE** = no drivable/scriptable observable exists
on this harness, with the reason stated.

Fixtures live in `bin/Res/test/rules-variance/` (`var_*.txt`); the registry is
`bin/Res/test/rules-variance/_var_C.txt`; the one synthetic card is
`bin/Res/test/rules-variance/_var_C_primitives.txt`.

Run command (from `projects/mtg/bin/`):

```
env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 \
  WAGIC_TESTSUITE_THREADS=1 WAGIC_TESTSUITE_FILE=test/rules-variance/_var_C.txt \
  WAGIC_TEST_PRIMITIVES_FILE=test/rules-variance/_var_C_primitives.txt ./wagic
```

Last full run: **8 tests, 2 failed** — the two finding-1 fixtures fail (PROVEN),
the six finding-2/finding-5 fixtures pass (REFUTED + controls).

## Verdict table

| # | CR ids | finding (audit §12) | verdict | fixtures |
|---|---|---|---|---|
| 1 | 506.3 / 508.1a / 509.1a | creature-Battles can attack and block | **PROVEN** | `var_506_3_creature_battle_attacks.txt` (FAIL), `var_509_1a_creature_battle_blocks.txt` (FAIL) |
| 2 | 506.4c / 510.1b | unblocked attacker whose planeswalker destination vanished deals damage to the opponent as fallback | **REFUTED** | `var_506_4c_walker_vanish_bounce.txt` (PASS), `var_506_4c_walker_vanish_destroy.txt` (PASS), `var_506_4c_walker_attack_control.txt` (PASS, control) |
| 3 | 508.1d | MUSTATTACK greedily forced, not jointly maximized | **UNPROBEABLE** | — |
| 4 | 509.1c | MUSTBLOCK greedy first-legal, no maximum-requirements solve | **UNPROBEABLE** | — |
| 5 | 509.1h | blocked attacker whose only blocker leaves during the blockers phase reverts to unblocked | **REFUTED** | `var_509_1h_blocker_leaves_bounce.txt` (PASS), `var_509_1h_blocker_leaves_destroy.txt` (PASS), `var_509_1h_unblocked_control.txt` (PASS, control) |
| 6 | 510.1c / 510.1e | damage division among multiple blockers is legacy sequential-lethal | **UNPROBEABLE** | — |
| 7 | 510.4 | first-strike eligibility tested live per pass, not snapshotted | **UNPROBEABLE** | — |
| 8 | 508.5 | combat-damage removal clears isAttacking and loses destination | **UNPROBEABLE** | — |

## Evidence

### Finding 1 — PROVEN — creature-Battles attack and block (506.3 / 508.1a / 509.1a)

`MTGCardInstance::canAttack` (`src/MTGCardInstance.cpp:840-865`) and the block
predicate gate on `isCreature()` and omit CR's "...that isn't also a battle"
exception. A permanent that is both a Creature and a Battle therefore passes.

Carrier: synthetic `Var Bastion Beast`, `type=Creature Battle`, 2/2. The type line
registers BOTH types — `MTGDeck.cpp:401-405` splits the `type=` line and
`setType()`s each word, so `hasType("battle")` is true (verified against the
parser; "battle" is a first-class type used throughout the engine, e.g.
`MTGRules.cpp:2226`).

- **Attack side** (`var_506_3_creature_battle_attacks.txt`): the creature-Battle is
  declared attacking the open opponent and deals 2. Assert (CR-correct) is
  `P2 life 20`; engine result `18` → **FAIL → PROVEN**.
- **Block side** (`var_509_1a_creature_battle_blocks.txt`): Craw Wurm (6/4) attacks
  the player (`choice 0`, needed because P2's Battle also arms an attack-target
  menu); the creature-Battle blocks it. Trace shows `Defenser Toggle: Var Bastion
  Beast` (block accepted); Craw Wurm dealt 0 to the player and the beast died to
  the 6 combat damage. Assert (CR-correct, beast can't block) is `P2 life 14` with
  the beast alive; engine result `life 20` + beast in graveyard → **FAIL → PROVEN**.

### Finding 2 — REFUTED — planeswalker destination vanishes (506.4c / 510.1b)

Audit flagged `GuiCombat::resolve` (`src/GuiCombat.cpp`) falling back to
`observer->opponent()` when `attacker->isAttacking` is null. Probed by attacking a
planeswalker and removing it before combat damage (both removal casts happen in the
**attackers step**, where instant casting is drivable):

- Walker attack is declared with `Craw Wurm / choice 1 (Attack a Planeswalker) /
  choice 0 (Jace Beleren)`. The **control** fixture proves this is a real attack:
  with no removal, Craw Wurm's 6 combat damage kills 3-loyalty Jace (control PASSES).
- **Bounce** (`Boomerang`) and **destroy** (`Lightning Bolt`, 3 dmg) variants both
  remove Jace before damage. In both, `P2 stays at 20` → **PASS → REFUTED**: the
  `?opponent()` fallback does not fire. On the current binary the attacker whose
  planeswalker leaves is dropped from damage assignment and deals nothing to the
  player — the CR-correct outcome. (The dead fallback code still exists; it is
  masked by the prior removal path, consistent with the audit's own caveat that an
  INCORRECT code path may be masked by another path.)

### Finding 5 — REFUTED — blocker leaves during blockers phase (509.1h)

Audit flagged `removeBlocker` (`src/MTGCardInstance.cpp:1703-1713`) setting
`blocked=false` when the last blocker leaves during `MTG_PHASE_COMBATBLOCKERS`,
which would revert the attacker to unblocked and let it hit the player.

Casting an instant during the blockers step is **not drivable** (clicks route to
combat declaration; land taps and spell casts do not fire — see harness note
below), so removal is done with an **on-block trigger** that fires during the
Blockers phase, no cast needed:

- **Bounce** (`var_509_1h_blocker_leaves_bounce.txt`): Elven Warhounds (2/2) is
  blocked by Centaur Courser (3/3); Warhounds' own on-block trigger puts the
  blocker on top of its library. Trace confirms the `StackAbility` resolves while
  `Current phase is: Blockers`.
- **Destroy** (`var_509_1h_blocker_leaves_destroy.txt`): Acolyte of the Inferno
  (3/1) is blocked by Grizzly Bears (2/2); Acolyte's on-block trigger deals 2,
  killing the blocker during the Blockers phase (trace: graveyard move under
  `Current phase is: Blockers`).

Both assert `P2 life 20` (CR-correct: attacker stays blocked, no player damage) and
**PASS → REFUTED**. The **control** (`var_509_1h_unblocked_control.txt`) proves the
discriminator: an unblocked Warhounds deals 2 (`life 18`), so "stays at 20" really
means "no forbidden player damage," not a silent attack failure. The audit's
`blocked=false` code exists but does not produce the predicted player damage.

### Findings 3, 4, 6, 7, 8 — UNPROBEABLE

- **3 (508.1d MUSTATTACK) / 4 (509.1c MUSTBLOCK)** — proving "greedy, not jointly
  maximized" requires a constructed *two-requirement conflict* where a first-fit
  assignment obeys fewer requirements than the maximum obtainable without breaking
  a restriction. That needs multiple cards with interacting attack/block
  requirements plus a mutual-exclusion restriction, none of which is cleanly
  available in the pool or expressible as a verified synthetic; the "skipped for
  extra combats" clause additionally needs extra-combat generation. No reliable
  scenario is constructible within the harness.
- **6 (510.1c/510.1e damage division)** — distinguishing "arbitrary division" from
  "sequential-lethal" requires driving the multi-blocker damage-division menu. The
  harness exposes no command for choosing an arbitrary split; the engine forces the
  sequential-lethal order and there is no scripted observable that separates the two.
- **7 (510.4 first-strike live per pass)** — requires changing first-strike between
  the first-strike damage step and the regular damage step. That needs an
  instant/trigger fired precisely between the two combat-damage steps; instant
  casting is not drivable inside the combat-damage window (even less so than the
  blockers step), and no trigger fires cleanly at that boundary.
- **8 (508.5 combat-damage removal loses destination)** — the observable would need
  a card/effect that references an attacker's "defending player" *after* the
  attacker is removed from combat, to detect the lost former-destination. No clean
  card-scripted observable for that reference exists, and the removal+reference
  sequence is not scriptable.

## Harness note (reusable)

Discovered while authoring set C, worth recording for future combat probes:

- **Registry paths are `test/`-relative WITHOUT the `test/` prefix.** The loader
  does `readIntoString("test/"+filename)` (`TestSuiteAI.cpp:1375`); a registry line
  `test/rules-variance/x.txt` resolves to `test/test/...` → `==Could not load test
  file==` → an infinite hang on the end screen, not a clean failure. Use
  `rules-variance/x.txt`.
- **Instant casting is drivable in main phases and the ATTACKERS step, but NOT in
  the BLOCKERS or combat-damage steps.** During those steps card clicks route to
  combat declaration (`Defenser Toggle`), and land-tap / hand-card clicks do not
  cast. Mana-source clicks float mana in main/attackers steps only. Floating
  `manapool:` set at INIT empties on the next step boundary.
- **Attacking a planeswalker** = click the attacker, then `choice 1` (Attack a
  Planeswalker/Battle), then `choice 0` (the walker). Both the attack-player rule
  and the planeswalker-attack rule react to the click, producing a menu
  (`0=Attack Player`, `1=Attack a Planeswalker`); a *name* click there yields
  "menu default: cancel" and silently cancels the attack.
- **A Battle on the opponent's side arms the same attack-target menu**, so any
  attacker declaration against a player must answer `choice 0` first.
- To remove a creature *during* the blockers step without a cast, use a real
  card's **on-block trigger** (Elven Warhounds bounce, Acolyte of the Inferno
  damage) — it resolves inside the Blockers phase.
