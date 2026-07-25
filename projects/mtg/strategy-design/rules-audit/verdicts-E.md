# Runtime-variance probe verdicts — batch E

Runtime probing of ten INCORRECT findings from `99-summary.md` §2 (the 7xx/702 SBA
cluster), against the current `bin/wagic` (built 2026-07-24 12:35). Each probe is a
fixture that asserts the **CR-correct** behavior, so:

- **FAIL = PROVEN** — the engine contradicts CR at runtime; the finding is confirmed observable.
- **PASS = REFUTED** — the engine produces the CR-correct result; the code-inspection finding does not manifest at runtime (or is masked by another path).
- **UNPROBEABLE** — no CR-correct-vs-buggy observable can be constructed/driven in the scripted test harness; reason given.

The three findings tagged `[ledger]` with existing witnesses (702.59 Recover, 702.170
Plot, 702.174 Gift) were skipped per the batch contract.

Registry: `bin/Res/test/rules-variance/_var_E.txt` (6 shipped fixtures).
Run: from `bin/`, `env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_AI=baka
WAGIC_TESTSUITE=1 WAGIC_TESTSUITE_THREADS=1 WAGIC_TESTSUITE_FILE=test/rules-variance/_var_E.txt ./wagic`.
Batch result: **6 tests, 2 failed** — the 2 failures are the two PROVEN findings (730, 702.131).

> Harness notes for re-runners: registry paths are relative to `Res/test/` (a `test/`
> prefix yields "Could not load test file" and then hangs). The opponent seat follows
> the user's `endpoints.txt` master switch, so pass `WAGIC_AI=baka` to keep any
> opponent decision off the LLM path.

## Verdict table

| # | finding | CR | verdict | fixture / evidence |
|---|---|---|---|---|
| 1 | 704.5f — normal creature at toughness≤0 can regenerate | 704.5f | **REFUTED** | `var_704_5f_regen_zero_toughness_dies` (PASS) + `var_704_5f_control_regen_saves_destroy` (PASS) |
| 2 | 704.4 / 702.2b — deathtouch destroys synchronously, not at SBA | 704.4/510.1c/702.2b | **REFUTED** (unobservable) | `var_704_4_deathtouch_synchronous_order` (PASS) |
| 3 | 704.5b — empty-library loss fires inside drawFromLibrary | 704.5b | **UNPROBEABLE** | code read — see below |
| 4 | 704.5j — legend rule: 3+ same-name legends can remain | 704.5j | **UNPROBEABLE** | 3 controls — see below |
| 5 | 704.5m — unattached Auras can survive | 704.5m | **REFUTED** | `var_704_5m_unattached_aura_dies` (PASS) |
| 6 | 704.5r — max-counter prevents additions vs SBA removal | 704.5r | **UNPROBEABLE** | see below |
| 7 | 704.5v — zero-defense Battle marked defeated, not graveyarded | 704.5v | **UNPROBEABLE** (Battle cluster) | see below |
| 8 | 730 — day/night is a physical marker permanent | 730.1/730.2 | **PROVEN** | `var_730_daynight_not_an_object` (FAIL) |
| 9 | 702.61 — split second suppresses legal mana abilities/priority | 702.61 | **UNPROBEABLE** | control — see below |
| 10 | 702.131 — Ascend / city's blessing is a battlefield token | 702.131 | **PROVEN** | `var_702_131_ascend_blessing_not_an_object` (FAIL) |

## Evidence

### 1. 704.5f — REFUTED (the audit missed a guard)
The audited SBA path is intact: `GameObserver.cpp:1021-1026` routes a non-indestructible
creature with `life < 1` through `card->destroy()`, which honors regeneration
(`MTGCardInstance::destroy()` → `if(!triggerRegenerate()) toGrave()`). But the audit
stopped there. `MTGCardInstance::triggerRegenerate()` (`src/MTGCardInstance.cpp:742-758`)
re-checks toughness before honoring the shield:
```
regenerateTokens--;
tap();
if(isCreature()){ life = toughness; initAttackersDefensers(); if (life < 1) return 0; } // "regeneration didn't work"
```
When toughness is still ≤0 (a lingering `-X/-X`), `life = toughness` stays ≤0, `triggerRegenerate`
returns 0, and `destroy()` falls through to `toGrave()`. **The creature dies — the CR-correct
outcome.**

`var_704_5f_regen_zero_toughness_dies`: Drudge Skeletons banks a `{B}:regenerate` shield
(trace: `StackAbility (Source: Drudge Skeletons)` resolves via `choice 0`), then Disfigure
`-2/-2` takes it to toughness −1. It goes to the graveyard → PASS. The companion control
`var_704_5f_control_regen_saves_destroy` uses the *same* shield against a real destroy
(Murder) and the creature **survives** (tapped) → PASS. The control proves the shield is
genuinely banked and functional, so the main fixture's pass is a real CR-correct result,
not a hollow pass from a never-banked shield. Finding REFUTED; the regen shield is consumed
but correctly cannot save a toughness≤0 creature.

### 2. 704.4 / 702.2b — REFUTED as unobservable at the combat-outcome level
`var_704_4_deathtouch_synchronous_order`: Typhoid Rats (1/1 deathtouch) attacks, Grizzly
Bears (2/2) blocks. CR (simultaneous damage) → both die; the deathtouch confirms it fires
(1 damage from a 1-power creature killing a 2/2). The fixture asserts both in the graveyard
and **PASSES** — the engine's outcome matches CR. Whatever ordering the engine uses
internally (`MTGRules.cpp:4188-4214`), it does not change the *both-die* combat outcome, so
the finding is not observable through a combat result. A deeper observable (e.g. a
"whenever a creature dies" trigger fired mid-damage-step vs at SBA) would need machinery
outside a clean combat-outcome probe; not pursued.

### 5. 704.5m — REFUTED
`var_704_5m_unattached_aura_dies`: Holy Strength enchants Grizzly Bears, then Murder
destroys the Bears. The aura is put into the graveyard along with the creature → PASS. The
engine does move an aura whose enchanted permanent left; the `card->target`-gated cleanup
(`GameObserver.cpp:931-999`) does not leave this aura stranded in play at runtime for the
creature-dies-to-destroy case.

### 8. 730 — PROVEN
`var_730_daynight_not_an_object`: establishes day and lets it become night via a daybound
Ballista Watcher (the timeline the lexicon `basic_daybound` fixture traces). CR: day/night
is a game *designation*, so no object exists to count. The engine creates a real `Day`/`Night`
marker **card** (`type=Emblem`, borderline.txt:25650/77105) that sits on a battlefield and
bounces to the opponent each end step. The fixture asserts the battlefield holds only the
transformed Ballista Wielder; it **FAILS** — `player 1's battlefield expected 0, got 1` — the
`Night` marker is a countable permanent parked on P2's battlefield. This is the count/existence
observable; the marker also carries `shroud`, so it is countable-but-not-targetable, which is
why the probe is a count assertion rather than a "destroy target permanent" one.

### 10. 702.131 — PROVEN
`var_702_131_ascend_blessing_not_an_object`: Dusk Charger (`_ASCEND_`) plus nine Darksteel
Citadel = ten permanents, so the controller gets the city's blessing. CR: the city's blessing
is a per-player *designation*, not an object. The engine's `_ASCEND_` macro creates a
`token(City's Blessing)` on the battlefield. The fixture asserts exactly the ten real
permanents; it **FAILS** — `player 0's battlefield expected 10, got 11` — the extra object is
the City's Blessing token, a countable permanent. A control (ten permanents with a vanilla
Grizzly Bears in place of the ascend card) asserts 10 and **PASSES**, cleanly attributing the
+1 to the ascend-created token.

## UNPROBEABLE findings — reasons

### 3. 704.5b — empty-library loss timing
`MTGPlayerCards::drawFromLibrary()` (`MTGGameZones.cpp:322-330`) already **inlines** the
protections before it calls `setLoser`: `if (inPlay->hasAbility(CANTLOSE) ||
hasAbility(CANTMILLLOSE) || opponent…hasAbility(CANTWIN)) return;`. So every common
"would-be-saved" case (Platinum-Angel-class cantlose, cantwin-lock, Laboratory Maniac —
which is scripted, but via a cantlose-family/win ability that the same inline check catches)
produces the CR-correct outcome. Distinguishing the *immediate-vs-next-SBA* timing needs an
intervening state change between the empty draw and the SBA that the immediate `setLoser`
would pre-empt — e.g. a simultaneous mutual deck-out that CR 104.4a makes a *draw* but the
sequential immediate-loss makes a one-sided win. That needs both libraries empty plus a
symmetric both-players-draw effect plus a draw-vs-loser outcome assertion, none of which is
cleanly constructible/drivable (the draw step is single-player; the turn-1 draw is skipped).
No clean observable.

### 4. 704.5j — legend rule, 3+ copies
The legend rule (`MTGNewLegend`) is **event-driven on entry** (`receiveEvent` →
`CheckLegend`, `MTGRules.cpp:3696-3760`), not a per-update SBA sweep, and `MoveLegend` moves
**one** duplicate per firing (`maxtargets=1`). Three controls bound the probe:
- INIT-placed duplicates never fire the rule: `inplay:X,X` leaves 2, `inplay:X,X,X` leaves 3
  (0 moved). So INIT cannot set up a "3 present when the SBA fires" state.
- A normal sequential cast is handled correctly: casting a 2nd copy (`cast / choice 0 /
  target`) reduces 2→1 (control PASSES). Sequential casting therefore never exhibits the 3+
  bug — each cast only ever creates a 2-present state.
- Driving a 3rd copy into a board already (illegally, via INIT) holding 2 requires clicking
  the shared card name; the harness `getCard` resolves same-name clicks to an **in-play**
  copy, so neither a hand-cast nor a graveyard reanimate (Zombify, target-restricted to
  `mygraveyard`) of the 3rd copy executes — the spell/target is left unresolved (controls
  leave the caster's card stuck in hand).
The specific *simultaneous three-present* entry the finding requires cannot be constructed
in the scripted harness. (Code inspection is consistent with the finding — a single-mover
would leave 2 — but it is not runtime-drivable here.)

### 6. 704.5r — maximum counters
A max-counter card is scripted (Rasputin Dreamweaver, `mtg.txt`, "can't have more than seven
dream counters", enters with 7). But the finding is a difference in *mechanism* —
prevent-the-addition vs remove-the-excess-as-an-SBA — that is only observable to a
replacement/trigger watching the counter-placement event (with "prevent" there is no event;
with "remove as SBA" the event fires then the excess is removed). Constructing that requires
a counter-added trigger plus a forced over-max addition on the same permanent, which no clean
drivable card interaction provides. No constructible observable seam.

### 7. 704.5v — zero-defense Battle
Folds into the known Battle cluster. Battle entry defense is `[ledger]`-broken (310.4b: real
`counter(0/0.6.defense)` lines parse NULL, leaving the Battle at 0 defense), and there is no
`protector` machinery, so a functioning Battle cannot be stood up from card script to reach a
clean "defeated-but-not-graveyarded" observable. UNPROBEABLE until the Battle lifecycle
(310.4b / 310.8) is fixed; the finding rides that cluster.

### 9. 702.61 — split second and mana abilities
The trace confirms the split-second object auto-resolves: after `Quagnoth` is cast the log
goes straight to `Resolving Action on stack: Quagnoth`, with the `yes` window not producing a
priority stop. But the distinguishing observable — a **legal mana ability firing under split
second** — cannot be driven: a control that casts a *normal* (non-split-second) spell and then
has the defender tap a single-ability mana source (Darksteel Citadel) in the response window
**also fails to tap** (`tapped battlefield … Expected 1, got 0`). Since the scripted seat
cannot drive a defender's mana-ability activation in *any* response window, the split-second
failure to tap cannot be attributed to engine suppression rather than a harness limitation.
UNPROBEABLE (response-window mana abilities are not scriptable here — the case the batch
contract anticipated). The existing `basic_split_second_cannot_be_countered` already covers
the correctly-suppressed non-mana-spell case.

## Shipped fixtures
- `var_704_5f_regen_zero_toughness_dies.txt` — REFUTED (PASS)
- `var_704_5f_control_regen_saves_destroy.txt` — REFUTED control (PASS)
- `var_704_4_deathtouch_synchronous_order.txt` — REFUTED/unobservable (PASS)
- `var_704_5m_unattached_aura_dies.txt` — REFUTED (PASS)
- `var_730_daynight_not_an_object.txt` — PROVEN (FAIL)
- `var_702_131_ascend_blessing_not_an_object.txt` — PROVEN (FAIL)

All uncommitted, under `bin/Res/test/rules-variance/`.
