# Runtime-variance verdicts — set A

Runtime probes for audit findings 304.4/307.4/400.4a, 306.7, 310.8b/c, 400.7, 402.2, run
against the **current** `bin/wagic` (HEAD, built Jul 24). Each verdict fixture asserts the
**CR-correct** behavior: a FAIL = variance PROVEN in gameplay, a PASS = the audit's INCORRECT
claim REFUTED on the probed path. Fixtures live in `bin/Res/test/rules-variance/`; synthetic
carriers in `rules-variance/_var_primitives.txt`; registry `bin/Res/test/_var_A.txt`.

Run recipe (from `bin/`):
```
env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 \
  WAGIC_TESTSUITE_THREADS=1 WAGIC_TESTSUITE_FILE=test/_var_A.txt \
  WAGIC_TEST_PRIMITIVES_FILE=test/rules-variance/_var_primitives.txt ./wagic
```
Final batch: **6 tests, 1 failed** (only `var_306.7`), 0 timed out.

## Verdict table

| finding | CR rule | verdict | fixture(s) | evidence |
|---|---|---|---|---|
| 1 | 304.4 / 307.4 / 400.4a | **REFUTED** | var_304.4_sorcery_to_battlefield, var_304.4_instant_to_battlefield | both PASS — sorcery/instant stay in graveyard; engine guards the mover |
| 2 | 306.7 | **PROVEN** | var_306.7_pw_damage_redirect | FAIL — player-targeted burn redirected onto opponent's planeswalker |
| 3 | 310.8b / 310.8c | **UNPROBEABLE** | (battle probe, deleted) | no `protector` machinery in engine; harness cannot instantiate a Battle |
| 4 | 400.7 | **REFUTED** (probed channel) | var_400.7_kicker_memory_reanimate (+ 2 controls) | PASS — reanimated kicked creature re-enters as fresh un-kicked 1/2 |
| 5 | 402.2 | **SKIPPED** | — | [ledger] cleanup auto-discard; not a 5-min confirmation, per instructions |

---

## Finding 1 — 304.4 / 307.4 / 400.4a: instants/sorceries movable to battlefield — REFUTED

**Fixtures:** `var_304.4_sorcery_to_battlefield.txt`, `var_304.4_instant_to_battlefield.txt`
(both PASS). Synthetic movers `Var Tomb Lifter` (`{0}:all(sorcery|mygraveyard)
moveto(mybattlefield)`) and `Var Tomb Lifter Instant` (instant variant) — generic
`moveto(battlefield)` movers with **no destination/type guard in the script**. Each is
activated with a real instant/sorcery (Lava Spike / Shock) sitting in the graveyard.

**Observed = CR-correct:** the mover fires and resolves (log: `Action added to stack:
StackAbility (Source: Var Tomb Lifter)` → `Resolving Action on stack: StackAbility`), but the
sorcery/instant **stays in the graveyard** — it is NOT put onto the battlefield.

**Root cause the audit missed:** `AAMover::resolve` (the `moveto` ability) carries **two**
guards keyed on `isSorceryorInstant()`:
- `src/AllAbilities.cpp:6804` — the put-into-play-via-`Spell` path requires
  `!_target->isSorceryorInstant()`.
- `src/AllAbilities.cpp:6867` — the generic-`putInZone` branch: `if(_target->isSorceryorInstant()
  && destZone == inPlay) { ... return 0; }` (does nothing, unless an `AAFlip` andAbility).

The audit cited `src/AllAbilities.cpp:6925-6979`, which is the inert `getMenuText`/`getName`
helper, and concluded no guard exists. The operative guard is in `resolve()` at 6804/6867. The
low-level `putInZone` itself has no such guard, but the `moveto`/AAMover DSL — the only generic
mover reachable from card script — never routes an instant/sorcery to the battlefield.

**What the probe did NOT exercise:** a hypothetical *non-AAMover* caller of
`putInZone(card, from, battlefield)` for an instant/sorcery (the guard lives in AAMover, not
putInZone). No such scriptable/gameplay path was found; the standard reanimation/`moveto` path
is guarded and conformant.

## Finding 2 — 306.7: obsolete planeswalker damage-redirection still active — PROVEN

**Fixture:** `var_306.7_pw_damage_redirect.txt` (FAIL, as intended). P1 casts **Lava Spike**
(`target=player,planeswalker`, `damage:3`) at **P2 the player** (`p2` target token); P2
controls **Jace Beleren** (3 loyalty). The `choice 1` line answers the engine's obsolete
redirect menu (option 0 = player, option 1 = first walker) by picking the planeswalker.

**Observed = CR-violation.** The fixture asserts CR-correct (P2 takes 3 → life 17, Jace
untouched in play) and FAILS with:
```
==life problem for player 1. Expected 17, got 20==
==Card number not the same in player 1's graveyard==, expected 0, got 1
==Card number not the same in player 1's battlefield==, expected 1, got 0
==Card ID not the same. Didn't find 625255==   (Jace Beleren)
```
The log shows the redirect fired: `TESTSUITE choice !!!` → `Action added to stack: 6Damage`
→ resolved, Jace went to P2's graveyard, P2 stayed at 20. (Log "player 1" = fixture PLAYER2,
0-indexed.)

**Engine vs CR:** a spell that deals damage to *target player* had its damage **redirected
onto a planeswalker that player controls** — the pre-2018 planeswalker-redirection rule that
CR 306.7 explicitly removed. Root: `AADamager::resolve`, `src/AllAbilities.cpp:2070-2093`,
builds a `MenuAbility` of {player, each opposing planeswalker} whenever
`_target == game->opponent() && opponent has a planeswalker`.

## Finding 3 — 310.8b / 310.8c: Battle protector eligibility absent — UNPROBEABLE

Two independent, compounding reasons the fixture harness cannot exercise the protector
attack/block-eligibility rules:

1. **No `protector` concept exists in the engine.** `grep -rc protector src/ include/`
   returns zero hits across the whole tree. CR 310.8b/c govern which players may attack/block
   a Battle *relative to its protector* — a designation the engine cannot represent, so there
   is no eligibility logic to compare against CR. (Corroborated by the audit's 704.5w/x.)
2. **The harness cannot instantiate a functioning Battle.** Casting a real Siege (Invasion of
   Belenon, `type=Battle`, `auto=counter(0/0.3.Defense)`, `auto=moveto(opponentbattlefield)`)
   from a fixture is a **no-op** — the probe left the card in hand with mana unspent
   (`Card number not the same in player 0's hand, expected 0, got 1`; `Mana problem ... got 3`).
   Battle lifecycle is also independently broken (310.4b/704.5v, [ledger]).

With no protector to test and no path to place a stable Battle into combat, a CR-comparable
attack/block-eligibility scenario cannot be constructed here. The exploratory battle-probe
fixture was removed (it never entered a valid state). The audit's code-level finding (combat
falls back to ordinary controller/opponent logic) stands on inspection; it is not
runtime-refutable or -confirmable through this harness.

## Finding 4 — 400.7: zone changes preserve forbidden object memory — REFUTED (probed channel)

**Fixtures:** `var_400.7_kicker_memory_reanimate.txt` (PASS) + controls
`var_400.7_kick_confirm.txt`, `var_400.7_reanim_confirm.txt` (both PASS).

The code **does** copy forbidden memory across zone changes — `MTGGameZone::removeCard`'s clone
copies `X`, `castX`, `kicked`, `storedCard`, `storedSourceCard`, alternate-cost flags, commander
damage, etc. (`src/MTGGameZones.cpp:936-957`). So the finding is correct at the code level. The
task is whether that produces **observable gameplay variance**.

**Probed channel — kicker across a reanimation round-trip.** P1 casts **Ardent Soldier** with
kicker (`choice 1`; ETB `if paid(kicker) then counter(1/1)` → enters 2/3), destroys it
(→ graveyard), and reanimates it with a synthetic mover (ETB re-runs). CR 400.7: the reanimated
Soldier is a new, un-kicked object → 1/2. A finishing **Shock** (2 damage) then discriminates:
1/2 dies (CR-correct) vs 2/3 survives (memory leaked).

**Observed = CR-correct.** The reanimated Soldier **died to 2 damage** (fixture asserts it in
the graveyard — PASS), i.e. it re-entered as **1/2**: the copied `kicked` flag did **not**
re-grant the ETB counter. The two controls make this a real read, not a spurious one:
- `var_400.7_kick_confirm` — a kicked Soldier (2/3) **survives** the same Shock (PASS), so
  `choice 1` genuinely pays the kicker in this sequence.
- `var_400.7_reanim_confirm` — the same cast→destroy→reanimate chain, minus the Shock, leaves
  Ardent Soldier **in play** (PASS), so the reanimation genuinely fired.

Interpretation: the "enters with a counter if kicked" replacement applies only at cast
resolution, not on a `moveto`-reanimation, so the remembered `kicked` flag has no effect through
this channel.

**What the probe did NOT exercise:** the `castX`-driven-P/T channel does not exist to test — every
X-creature in the pool defines P/T via `counter(1/1,X)` (counters, correctly lost on a zone
change), none via a `castX`-read power/toughness. The `storedCard`/`storedSourceCard` link
channels and alternate-cost-flag channels were not probed (no clean single-card observable
found). The code-level memory copy is real but surfaced no CR-visible variance through the one
gameplay channel that could be constructed.

## Finding 5 — 402.2: cleanup auto-discard — SKIPPED

Marked [ledger] in the summary; the instructions gate a probe on it being a 5-minute
confirmation only. The cleanup discard-choice behavior requires driving a full turn to the
cleanup step with a manual over-hand-size discard choice, which is not a 5-minute confirmation,
so it was left to the existing ledger entry (paired with 514.1).

---

## Could-not-verify / caveats

- **306.7 (PROVEN)** rests on driving the redirect menu with `choice 1`. The menu-*default*
  (unscripted) picks option 0 = the player, so a fixture that never answers the menu would look
  CR-correct; the variance is that the redirect menu **exists and is drivable** onto a
  planeswalker from a player-targeted spell, which no modern-CR engine offers. Verified by the
  planeswalker dying and the player taking zero.
- **304.4/307.4 (REFUTED)** covers the AAMover/`moveto` path only — see the "did NOT exercise"
  note (non-AAMover direct `putInZone`-to-battlefield callers were not enumerated).
- **400.7 (REFUTED)** is channel-specific: the underlying memory-copy code is real; only the
  kicker→ETB→counter observable was shown CR-correct. Do not read this as "400.7 memory copy is
  harmless in general" — read it as "no gameplay variance was demonstrable through the
  constructible channels (kicker, and P/T, which has no castX carrier)."
- **310.8 (UNPROBEABLE)** — the no-op Battle cast could itself be a fixture-craft limitation
  rather than proof battles never cast; but combined with zero protector machinery, the
  protector eligibility rules remain untestable here regardless.
