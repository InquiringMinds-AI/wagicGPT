# Runtime-variance verdicts — agent D (findings 1–11: CR 601–608 / 609–616)

**What this is.** Each audit finding assigned to agent D was turned into a fixture (or
UNPROBEABLE verdict) that asserts the **CR-correct** behavior on the current `bin/wagic`.
Per the campaign contract: a fixture **FAIL = the defect is PROVEN**; a **PASS = the finding
is REFUTED** at the observable level; **UNPROBEABLE** = the harness cannot construct a clean
observable, with the reason.

All probe fixtures live in `bin/Res/test/rules-variance/` and are registered in
`bin/Res/test/rules-variance/_var_D.txt`. Synthetic cards are in
`bin/Res/test/rules-variance/_var_D_primitives.txt` (ids 2100000700–2100000799).

Run:
```
cd projects/mtg/bin
env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 \
  WAGIC_TESTSUITE_THREADS=1 WAGIC_TESTSUITE_FILE=test/rules-variance/_var_D.txt \
  WAGIC_TEST_PRIMITIVES_FILE=test/rules-variance/_var_D_primitives.txt ./wagic
```
Result: **14 tests, 0 failed.** (The two `_var_D` primitive/registry files are the only
registry + primitives I added; the fixtures and this report are the deliverables.)

## Headline

**Zero of the cleanly-probeable findings reproduced as a wrong observable outcome — every
one REFUTED; the rest are UNPROBEABLE.** This is a real, control-validated result, and it
directly corroborates the audit's own epistemic caveat (99-summary §7): the INCORRECT
verdicts in §2 are **code-inspection leads**, and "a code path that looks wrong on
inspection may be masked or corrected by another path not in view." For every finding I
could drive, that masking path exists and produces the CR-correct end state. The defects are
real at the **mechanism** level (no CR-613 layer system, non-atomic casting, install-after
enters-with) but do not surface as wrong outcomes in the representative interactions the
card pool can express. Nothing here says the engine *implements* CR 613 / atomic casting —
it says the per-frame recompute + spell-resolution target check + `skipDamageTestOnce`
workaround **converge to the right answer** in these classic cases.

## Verdict table

| # | Finding | Verdict | Fixture(s) / evidence |
|---|---|---|---|
| 1 | 608.2b — no resolution-time target revalidation/fizzle (spell) | **REFUTED** | `var_608_2b_target_fizzle` (+`_instant_control`) |
| 2 | 608.2e/608.2f — multi-target simultaneity observable | **UNPROBEABLE** | see below |
| 3 | 608.3b — permanent (Aura) spell resolves to battlefield w/o target re-check | **REFUTED** | `var_608_3b_aura_fizzle` (+`_aura_control`) |
| 4 | 610.3a — blink exiles even when return already happened | **UNPROBEABLE** | see below |
| 5 | 610.4a — "phase out until EVENT" phases in at next untap | **UNPROBEABLE** | phasing keyword unimplemented [ledger] |
| 6 | 611.2e — enters-with characteristics installed after zone entry | **REFUTED** | `var_611_2e_enters_with_counters` |
| 7 | 613 — no layer system (Humility / lord / type-change / switch) | **REFUTED** (5 discriminators) | `var_613_*` (six fixtures) |
| 8 | 614.5 — replacement applies twice to one event | **UNPROBEABLE** | no replacement-based doubler in pool |
| 9 | 614.7a — zero-damage consumes a one-shot prevention shield | **REFUTED** | `var_614_7a_zero_damage_shield` (+2 ctls) |
| 10 | 615.7 — no allocation choice for limited shields | **UNPROBEABLE** | allocation menus not scriptable |
| 11 | 601/602/603.3 — casting/trigger atomicity | **UNPROBEABLE** | intra-cast ordering / stack windows unobservable |

---

## REFUTED findings — evidence and mechanism

### 1. 608.2b — single-target spell with an illegal target fizzles correctly
`var_608_2b_target_fizzle`: P1 casts **Var Surge Blessing** (`target=creature`, `+2/+2` and
a controller-side `life:5` rider) on its own Grizzly Bears; P2 Shocks the Bears dead in the
interrupt window. At resolution the only target is gone. **CR-correct outcome observed:** the
spell fizzles to the graveyard and the `life:5` rider does **not** fire (P1 stays at 20).

- **Observability engineering.** A bare pump on a now-dead creature is invisible (the buff on
  a graveyard card produces the same end state as a fizzle). The `life:5` rider makes "the
  ability resolved" visible on the *controller*, independent of the dead target.
- **Positive control** `var_608_2b_instant_control`: same spell on a *living* Bears → rider
  fires, P1 reaches 25. Proves the observable is live, so the fizzle case's "20" genuinely
  means the spell did not resolve.
- **Caveat (residual, not probed here):** the audit cites `TargetAbility::resolve`
  (`MTGAbility.cpp:8052-8098`), which is also the **activated-ability** resolution path. My
  probe exercised the **spell** path (`Spell::resolve`), which fizzles correctly. An
  activated targeted ability whose sole target dies mid-stack could still exhibit the raw
  `TargetAbility::resolve` reuse; that sub-case is left open.

### 3. 608.3b — Aura with a dead target fizzles to graveyard, does not enter play
`var_608_3b_aura_fizzle`: P1 casts **Var Phantom Grip** (Aura, `+2/+1`, with an ETB
`gain 5 life`) on its own Grizzly Bears; P2 Shocks the Bears dead in response. **CR-correct
outcome observed:** the Aura goes to P1's graveyard, its ETB does **not** fire (P1 at 20).

- **Observability engineering.** End-zone alone cannot separate "never entered" (CR) from
  "entered then died to the unattached-Aura SBA" (the shape the audit's `Spell::resolve`
  finding would predict) — both land the Aura in the graveyard. The ETB `life:5` makes
  "touched the battlefield" visible.
- **Positive control** `var_608_3b_aura_control`: same Aura on a *living* Bears → it enters,
  ETB fires (P1 = 25), Aura survives attached. Proves the observable is live.
- **Mechanism.** Despite `Spell::resolve` (`ActionStack.cpp:462-499`) unconditionally moving
  a permanent spell to the battlefield on inspection, the runtime Aura-attachment path
  fizzles a dead-target Aura *before* the ETB runs. The audit's code-inspection finding is
  masked by that path.

### 6. 611.2e — a 0/0 that enters with +1/+1 counters survives (no pre-counter SBA death)
`var_611_2e_enters_with_counters`: P1 casts **Var Hollow Spike** (base 0/0, `counter(1/1,2)`
= enters with two +1/+1 counters). **CR-correct outcome observed:** it enters as a 2/2 and
survives in play; it does **not** die to the zero-toughness state-based action.

- The audit itself names the `skipDamageTestOnce` workaround as the reason the install-after
  ordering does not kill the entrant. This probe confirms the workaround holds at the
  observable level: the SBA never sees the pre-counter 0/0. The defect is mechanism-only.

### 7. 613 — layer results are CR-correct across five classic discriminators
The engine has no formal layer system, but its per-frame recompute (each `Update` re-derives
characteristics from a clean base) converges to the CR-correct answer for every classic
interaction I could build. P/T is read via combat damage from **Var Measure Golem** (4/4,
`mustattack`). Diagnostics confirmed each ingredient in isolation (anthem → 5 dmg, base-set
→ 1 dmg, creature-lord → 5 dmg, one swap → 1 dmg).

| sub-behavior proved | fixtures | CR result | observed |
|---|---|---|---|
| **7b set + 7c pump, order-independent** | `var_613_layers_setfirst`, `..._pumpfirst` | golem 2/2 in both orders | 2/2 both (P2 = 18) |
| **Humility-class ability-loss shuts off a creature-based lord** | `var_613_humility_vs_lord` | golem 1/1 (lord's +1/+1 removed) | 1/1 (P2 = 19) |
| **layer-4 type-change feeding a type-conditional pump (dependency), both orders** | `var_613_dependency_graftfirst`, `..._musterfirst` | golem is a Zombie → +2/+2 = 6/6 both orders | 6/6 both (P2 = 14) |
| **two 7e P/T-switch effects cancel** | `var_613_double_switch` | 3/1 → 1/3 → 3/1 (deals 3) | 3/1 (P2 = 17) |

Even the two discriminators the audit calls out most sharply — the `ALoseAbilities` "hack"
(Humility vs a creature-lord) and "multiple switch effects collapse into one boolean toggle"
(double switch) — produce the **CR-correct** result at runtime. I could not construct a
divergent case with the available primitives. A genuine outcome divergence would likely need
a pathological timestamp-dependency or the judge-debated Opalescence+Humility 0/0 case, which
is not cleanly assertable via combat damage.

### 9. 614.7a — a zero-damage event does NOT consume a one-shot prevention shield
`var_614_7a_zero_damage_shield`: P1, in one seat, casts three instants that resolve in order
(the harness resolves each instant as it is cast): **Var Ward Charm** (`prevent:2` shield on
Grizzly Bears) → **Var Null Bolt** (`damage:0` to the Bears, a real 0-magnitude damage event)
→ **Shock** (2 damage). **CR-correct outcome observed:** the 0 does not spend the shield, so
Shock's 2 is prevented and the 2/2 Bears **survives**.

- **Two controls make it rigorous:** `var_614_7a_ctl_noshield` (no shield → Null Bolt's 0
  then Shock kills the Bears — confirms lethality and that Null Bolt itself does nothing) and
  `var_614_7a_ctl_oneshot` (shield + two Shocks → first prevented, second kills — confirms
  the shield is genuinely **one-shot**, so the probe's "survives" is not a persistent-shield
  artifact).
- **Mechanism.** `prevent:N` uses the numeric `target->preventable` path, which
  `Damage::resolve` checks *after* the `if(!damage) return 0` zero-check — so a 0-damage
  event returns before touching it. The finding's cited `-1 oneShot` replacement branch
  (`ReplacementEffects.cpp:24-29`) is not the path these cards take, and no card in the pool
  constructs an `-1, oneShot=true` shield.

---

## UNPROBEABLE findings — reasons

### 2. 608.2e/608.2f — multi-target simultaneity
Reproducing the sequential-vs-simultaneous divergence needs a two-target effect whose second
target's fate is changed by an intervening trigger/SBA fired between the two per-target
resolutions. That construction requires (a) driving a multi-target chooser menu from the
scripted seat, and (b) a death/leave trigger timed precisely between the two sub-resolutions.
The multi-target menu driving plus death-trigger timing cannot be made reliable in the
harness without manufacturing an artifact, so no clean observable exists. The single-target
atomicity that underlies this cluster is REFUTED (see 608.2b/608.3b).

### 4. 610.3a — blink whose return event already happened
The finding requires the blinked object's *return* to have already occurred (via another
effect) before `ABlink` exiles it — a race between two effects on one object. There is no
scriptable way to force "the return already happened" ahead of the exile from a single seat;
the construction is not reliably drivable.

### 5. 610.4a — "phase out until EVENT"
Phasing is largely unimplemented (99-summary Tier-1 UNIMPLEMENTED 702.26, [ledger]): the
keyword and one-shot token parse, but there is no untap-step phase-out/phase-in transition
and no until-event phaser to place in the pool. There is nothing drivable to distinguish
"phases in at next untap regardless" from correct until-event behavior. Fast verdict per the
brief.

### 8. 614.5 — replacement applies twice to one event
The bug is in `ReplacementEffects::replace`'s recursion (`if (newEvent != e) return
replace(newEvent)`) with no already-applied set. The card pool's damage *doublers* (Furnace
of Rath, Gratuitous Violence, Fiery Emancipation) are implemented as **triggered abilities**
(`@damaged(...):damage:thatmuch`), which bypass the replacement pipeline entirely and cannot
exercise the recursion. The only replacement effects the pool builds are damage *prevention*
shields, which self-exhaust (`damage=0` after firing, guarded by `if(!damage) return event`)
and therefore cannot re-apply. No card constructs a modify-and-return replacement that would
re-enter the modifier list, so the recursion defect has no drivable trigger.

### 10. 615.7 — allocation of a limited shield across simultaneous damage
The finding is precisely the *absence* of a controller choice: the engine subtracts a numeric
shield per source with no allocation menu. Probing "the choice is missing" would require the
allocation menu to exist to be driven; there is no menu to script, and there is no observable
that separates "no allocation offered" from a fixed subtraction order without a
player-decision surface. Not scriptable.

### 11. 601/602/603.3 — casting / trigger atomicity
The representative observables (a cast-trigger seeing the card still in hand mid-cast; a
sacrifice-cost trigger resolving without a stack response window) all depend on observing
**intra-cast ordering** or the **presence/absence of a stack response window**. The scripted
suite drives a single active seat with no genuine ad-hoc instant-speed response modeling, so
it cannot observe whether an event used the stack or resolved immediately, nor inspect a
card's zone at a mid-cast instant. The downstream *outcomes* of these atomicity properties
that ARE observable (single-target fizzle) are REFUTED above.

---

## Honesty notes

- **No FAILs to verify.** The contract asks to verify every FAIL is for the claimed reason;
  I produced none — every probe passed (REFUTED). Rigor came instead from **positive/negative
  controls** attached to each REFUTED finding (aura ETB control, instant rider control, five
  613 isolation diagnostics, and the two 614.7a controls), which rule out trivially-passing
  fixtures by proving each observable is live and each shield genuinely consumable.
- **REFUTED means "outcome CR-correct at the observable level," not "CR-implemented."** The
  audit's mechanism-level INCORRECT verdicts (no layer graph, non-atomic cast, install-after
  enters-with) stand as code facts; this probe set only shows they do not manifest as wrong
  outcomes in the representative cases the card pool can express.
- **Observability is honest about the atomicity class.** Several findings (608.2e/f, 603.3,
  614.5, 615.7) are ordering/atomicity properties that are only observable through
  trigger-ordering or response-window side effects the single-seat harness cannot surface;
  those are marked UNPROBEABLE rather than forced.
