# Wave-57 lane B — combat surfaces (D6, D24, D22, D10)

Branch `w57-lane-B`, worktree `worktrees/lanes/w57-B`, base master 4d4ea0bdf (wave-57 lane brief;
src = wave-56 close 4366e8eec + the brief commit).

## Gate

| | baseline (brief, 4366e8eec) | after (hermetic `make -B`) |
|---|---|---|
| PARSETEST | 2736 passed / 0 failed | **2780 / 0** (+44, all `#W57-B`) |
| suite | 1241 tests, 2 failed (`lifeline.txt` + `merrow_reejerey.txt` — the known concurrency-only pair), 55 AI / 0, 0 timeouts | **1241 tests, 2 failed (the SAME two), 55 AI / 0, 0 timeouts** |

Logs `~/.gatelogs/w57-B-buildB.log` (rc 0), `w57-B-parsetest.log`, `w57-B-suite.log`.
`git diff | /usr/bin/grep -c $'\357\277\275'` = 0. No new suite fixture: all four items are prompt
BUILDERS, pinned in PARSETEST (the wave-56 lane-B precedent for the same seam).

## What the corpus actually says about D6 (the number the docket's headline rounds off)

Read over the whole wave-56 corpus (42 files, 2,127 `ask` records) with the seat's own prompt as
the source of truth:

* `INCOMING THIS COMBAT:` renders on **9** prompts corpus-wide.
* Windows during the OPPONENT's combat (phase ∈ {Combat begins, Attackers, Blockers, Combat
  damage, Combat ends} and `It is the opponent's turn.`): **218**.
* Cross-referencing each game's OTHER seat file for `- You declared attackers` on that turn, the
  emitter's post-declaration hit rate is **6 of 6** (Attackers 1/1, Blockers 2/2, Combat damage
  2/2, Combat ends 1/1). **The wave-56 emitter is not broken where it fires.**
* The gap is entirely PRE- and POST-declaration: **57 `Combat begins`** windows and **43
  `Attackers`** windows where the opponent had not (yet) declared, plus **40 `Combat ends`**
  windows. The cause is the gate: `MTGCardInstance::attacker` is false before the declaration and
  clears once combat is over, so the header token is absent from exactly the windows in which the
  seat is asked to spend mana about the attack.
* The docket's cited repro `125v146` **seq 65** is a **Main phase 1 on the seat's OWN turn**
  (`3 of them without a restriction against attacking` is the non-active-seat header form). It is
  a real "ten power and no total" window but it is not a window during their combat, so it is
  outside what this lane changed. Stated so the next wave does not read it as covered.

## D6 — the header token on EVERY window of their combat, computed once and re-rendered

Four forms now, selected by one pure function `incomingCombatForm(oppActive, phase, liveAttackers,
latchTurn, turn, ableAttackers)` (phase is the engine's own `GamePhase` ordinal):

1. **declared** — live attacker flags: the wave-56 line, recomputed (blocks already declared still
   remove their attacker from the total), and **latched** on `observer->turn`.
2. **settled** — the latch, re-rendered past tense once the flags clear:
   `INCOMING THIS COMBAT: 3 attackers, 11 unblocked damage - already dealt or removed this combat`.
   No `you would be at` — that subtraction has already happened to the life total the frame prints
   two lines above.
3. **forecast** — before the declaration:
   `INCOMING THIS COMBAT: not declared yet - 3 of their creatures can attack, for up to 10 - you
   would be at 3` (`; that would KILL you` on `K <= 0`), gated on the same
   `boardCreatureCanAttackNow` predicate the board header uses.
4. **closed** — past the declaration step with nothing declared:
   `INCOMING THIS COMBAT: they declared no attackers - no combat damage is coming at you this
   combat`. That is the fact a hold-up-mana decision at Blockers or Combat ends turns on, and it
   closes the forecast the seat has been reading.

Latch state is three ints on `AIPlayerGPT` (`mIncomingCombatTurn/Attackers/Damage`), read only for
the turn they were written on. The form selector is pure, so PARSETEST walks a whole combat
(Combat begins → Attackers pre/post declaration → Blockers → Damage → Combat ends) and requires
**no window to return form 0**, plus negatives for the seat's own combat, main phases and upkeep,
and for a latch left over from an earlier turn.

**Prediction.** Windows during the opponent's combat (all ask kinds) with no `INCOMING THIS COMBAT:`
line — **0 of N** (wave-56: 209 of 218). Of the four forms, the two new pre/post ones account for
~140 of those windows.

## D24 — the assignable remainder

`incomingCombatLine` gained `bestCaseDamage` / `bestCaseOptimal`. With bodies and a legal pairing:

`... (of that, 10 from 2 attackers none of your creatures can block) - best case with every blocker
assigned: you would be at -5` — `123v152` seq 23's exact number (16 − 3 = 13, at 8 life).

The remainder comes from `assignableRemainderDamage(damage, can)`: `can[i][j]` is the ENGINE's own
`MTGCardInstance::canBlock(attacker)` map, one row per creature of the seat's that `canBlock()`.
Each blocker blocks one attacker and a second blocker on an already-blocked attacker removes
nothing more, so maximum prevented damage is a maximum-weight bipartite matching whose weights
depend only on the ATTACKER — a transversal matroid, on which the greedy (heaviest attacker first,
Kuhn augmenting path) is provably optimal. PARSETEST pins the augmenting case that a naive greedy
would get wrong (the 7 and the 5 sharing the only blocker that can take the 7).

Two attacker classes are deliberately given a preventable value of **0**: a **trampler** (blocking
it does not remove all of its face damage) and a **menace/three-blockers** attacker (one body is
not a legal block of it). The assignment found is then still legal, so its number is ACHIEVABLE but
not proven maximal — and the wording drops accordingly to `- one legal assignment gets you to -5`.
Nothing is claimed with no bodies, with no legal pairing at all (the existing
`none of your creatures can block` parenthetical already carries that), or above 32 a side.

**Prediction.** Lethal-header blockers windows where the seat must do the subtraction itself to
reach the right answer — **0 of N**; and blockers windows where a printed `best case` clears 0
while the header says `this KILLS you` and the seat still answers `BLOCKS: none` — below
`123v152` seq 23's 1 of 1.

## D22 — the blocking-trigger TOTAL, on the BLOCKS header

New window line directly under `combatDamageForecast`, from the same `blockTriggeredLifeFor()` the
per-row `{blocking trigger, this combat:}` clause reads, so header and rows cannot disagree:

`BLOCKING THIS COMBAT: each of your 3 blockers that blocks gains you 2, and your converter takes
that much off them - up to 6 off their 10`

The distributive `each of your N` is used **only** when every triggered blocker carries the same
(certain, may) pair; otherwise the line falls back to
`your N blockers with a blocking trigger gain you up to K in total`. The script's own "may" half is
kept apart from the certain half (D13's rule), the whole thing is always voiced as `up to` (how
many actually block is the reader's choice), and with no converter of the seat's the drain half is
absent entirely (`- up to 6 life for you`). No lethal claim is made off `K >= their L` — a
converter board is exactly where a life-gain answer can move that number.

The per-row clause is untouched.

**Prediction.** Blockers windows where the seat controls a converter and declines all blocks under a
non-lethal header — falls below wave-56's 1 of 1.

## D10 — the last nested B-line gains

D13 un-nested the blocking trigger and left the LIFELINK gain — the same species — inside the
survival verdict. Two more out-params on `combatTradePreviewStats`
(`outBlockerLifelink`, `outAttackerLifelink`), same seam and same default-NULL discipline: with
them NULL every caller and every wave-56 PARSETEST case renders byte-identically (pinned).

* the OWN gain (`126v123` seq 37): `{lifelink, this block: you gain 1}`
* the ATTACKER's gain (the nine residuals, `126v152` s7/s11, `123v126` s57/s65/s92, `162v152` s8,
  `146v126` s20): `{their attacker's lifelink, this block: they gain 3 from this block only, and
  this attacker deals nothing to your life}`

A double striker keeps the wave-44 discipline — names the effect, withholds the number — in both
routed and nested forms. Both new braces are added to `stripNarrationDecoration`, so neither enters
history; PARSETEST pins the echo shape (`BLOCKS: B1:A1` off the annotated row still parses, the row
narrates clean).

**Prediction.** B-lines nesting any gain inside the survival verdict — **0 of N** (wave-56: 10 of 36).

## What I did NOT verify

* **No live game was run.** No corpus, no probe deck, no GUI, no model call. Every claim above
  rests on the wave-56 JSONL corpus (read-only, python), PARSETEST, the suite, and code reading.
* **No suite pin.** All four items are prompt builders wired at one call site each; the WIRING —
  which window each form actually reaches in a real prompt — is unverified until the wave-57
  corpus. This is the same exposure wave-56 lane B carried and the same one that let D6's
  "every window" claim ship at 9 of ~200.
* **D6 latch, extra combats.** The latch is keyed on `observer->turn`, so a turn with TWO combat
  phases would re-render the first combat's total as the second's settled line. No deck in the
  corpus grants an extra combat; not exercised, and not guarded.
* **D6 form 3 at the end of the attackers step.** If the opponent declares no attackers, the
  window is still inside `MTG_PHASE_COMBATATTACKERS` and reads `not declared yet` rather than
  form 4. Accurate as a statement about that instant, but it is not the form the reader would
  most want there; form 4 only takes over at Blockers.
* **D6 forecast magnitude.** `ableDamage` is the sum of printed power over creatures
  `boardCreatureCanAttackNow` accepts. It does NOT model attack COSTS, propaganda-style taxes,
  or the opponent's own choice to hold back — it is a ceiling, and it says `up to`.
* **D24 optimality is proven only for the model I chose.** The matching treats a blocker as
  removing an attacker's whole face damage. First strike, deathtouch, damage prevention, banding
  and multi-block orderings do not change the DAMAGE PREVENTED by a block, so they do not enter
  it; I did not audit for an effect that lets a blocked attacker still hit the face other than
  trample (which is excluded). If one exists, the number over-claims prevention for that attacker.
* **D24 cost.** The matching is O(A·B·A) on boards capped at 32 a side and runs once per rendered
  window during their combat (not memoised). I did not measure it on a wide board.
* **D22 uniformity.** `blockTriggeredLifeFor` is per BLOCKER, so two blockers covered by different
  trigger sources that happen to grant the same numbers read as "uniform". That is true of the
  numbers, which is what the line claims; it is not a claim about the sources.
* **D22 converter half.** `playerHasLifeToDamageConverter(this)` is the same predicate the per-row
  clause uses; I did not re-verify it against a gated or conditional converter.
* **D10 voicing.** The two out-params are only ever passed by the blockers-window B-line emitter
  (`attackerSeat == false`). Nothing stops the attackers seam from passing them later, and the
  routed strings would then be voiced from the wrong seat. Not guarded, only documented.
