# Wave 57, lane V — HEURISTIC AI blocking weak points (owner's Vita play)

## The report

Owner, vpk15/16 session, 2026-09-03, four transcripts tagged `bad blocking`:

> "on the vita, ive been playing only against the heuristic ai. Im looking for gui
> bugs, and cardbugs, and heuristic ai weakpoints."
> "i think it was mostly good, aside from some bad blocks from the ai, legal, but
> deterministically bad."

"Deterministically bad" turned out to be exact: two rules inside
`AIPlayerBaka::chooseBlockers` produce the SAME wrong declaration every time the
same board shape comes up, and both are shipped fixed here.

## How the transcripts were read

The four dumps do not replay (a human-seat transcript diverges early — see
"Not verified"), so the combats were reconstructed from the click record.
`GameObserver::userRequestNextGamePhase` logs `pN.next <interrupt> <phase>` with
the phase the seat is LEAVING and the prefix of the TURN's player, so a combat is
the window between `next … 5` (attackers declared, `MTG_PHASE_COMBATATTACKERS`)
and `next … 6` (blockers declared) / `next … 7`. Card clicks log with the CARD's
controller, so the AI seat's `p2.battlefield[i]` lines inside the blockers window
are its declaration.

A block click is not a choice: `MTGBlockRule::reactToClick` CYCLES the blocker one
step through the legal attackers and then off again, so N clicks on a body land it
on attacker `N mod (attackers+1)`. Attacker order is the attacking player's
battlefield index order. The resolver is
`strategy-design/wave57/…` (script kept under `~/.gatelogs/w57V/blocks.py`, not
shipped); P/T and keywords come from `Res/sets/primitives`.

## The four reconstructions

**deck122 — `transcript-1788468516`, player deck7 "Relentless Growth" vs ai deck122
(W/B lifegain).** T5 Swarm Shambler + Experiment One attack; the seat's lone 1/1
Suture Priest blocks Experiment One (1/1) — an even trade that spends a value
engine. T11/T15 a 2/3 flying deathtouch lifelink Vampire Nighthawk blocks
Experiment One, then attacker **#1** (Swarm Shambler) out of three while the
freshly-pumped Rubblebelt Raiders walks in — the aim is index order, not threat.
*Judgment:* the Nighthawk blocks are not losses, but the target choice is
positional, and T13 it blocks nothing at all with the same board.

**deck84 — `transcript-1788468785`, player deck5 vs ai deck84 (Allies).** The
headline pile. T13: Ironroot Warlord (`power=*` = the attacker's creature count,
toughness 5), Squirrel Sovereign 2/2 and two 2/2 Squirrels attack. The seat has
FIVE untapped bodies and puts **all five on the Warlord** — one click each, which
is the pass-1 signature — so all three Squirrels connect. Two blockers die to the
Warlord's damage and the seat gains one dead Warlord.
*Judgment:* indefensible. Seascape Aerialist (2/3) kills a Squirrel and lives;
Joraga Bard (1/4) and Halimar Excavator (1/3) each absorb a Squirrel for free. The
right line blocks three attackers and takes the Warlord; the seat did the reverse.

**deck58 — `transcript-1788469781`, player deck2 "Skills Paid the Bills" (Boros
first strike/auras) vs ai deck58 (Gruul).** T5: a lone Youthful Knight
(**2/1 first strike**) attacks and Kird Ape blocks it. T11: Youthful Knight, Court
Street Denizen and Fencing Ace attack; Tattermunge Duo AND Noxious Hatchling both
land on the Knight (attacker #1) and the other two attackers are unblocked, with a
Kird Ape standing home.
*Judgment:* double-blocking a 2/1 first striker is a wasted body — the 2/3
Tattermunge Duo kills it alone and survives — and the free body belonged on Court
Street Denizen.

**deck19 — `transcript-1788471208`, player deck2 again vs ai deck19 (Elves).**
Fencing Ace (**1/1 double strike**) attacks ALONE on T11, T15, T17, T19 and the
seat answers with TWO blockers on T11/T15/T17 (one on T19). Against a double
striker the smaller of the two dies in the first-strike step having dealt nothing.
T21: Youthful Knight + Danitha Capashen (2/2 first strike lifelink) attack into a
double-Wilt-Leaf-Liege board and each takes one Elf Warrior.
*Judgment:* the repeated double block on a lone 1/1 is the "deterministic" one —
the same board, the same wrong answer, four turns running.

## The rules changed (`src/AIPlayerBaka.cpp`, tagged `#W57-V`)

### 1. The trade arithmetic — `evaluateDeclaration()`

Every kill/survive test in the file compared power to toughness RAW:
`canKill = (card->power >= attacker->toughness)`, `survives = (card->toughness >
attacker->power)`, and pass 1's running "tracked toughness" subtraction. That is
false whenever either side strikes first, and it is false in both directions.

`evaluateDeclaration(attacker, candidate, …)` resolves a WHOLE declaration over
both damage steps and reports (a) does the attacker die, (b) does the candidate
die, (c) how much creature VALUE the declaration spends. It carries:

* **first strike / double strike** — a blocker destroyed in the first-strike step
  deals nothing (CR 510.4), and a double striker deals again in the regular step;
* **damage division** (CR 510.1c) — the attacker's power is allocated greedily,
  cheapest lethal first, i.e. the DEFENDER'S WORST CASE. Counting every gang
  member as dead independently both over-prices sound gangs and under-prices bad
  ones, so the set had to be modelled, not the pair;
* **deathtouch / indestructible / protection** via `lethalNeededFrom()` — this
  absorbs wave 53's `blockerDiesToAttackerRegardless`, which is deleted.

Why it generalises: it replaces a proxy (power vs toughness) with the actual rule
the engine will apply, so every consumer of it gets first strike, deathtouch and
gang math for free — including cases nobody in these transcripts hit.

### 2. Pass 1 — aim the declaration and price the trade

The old pass 1 committed with ONE BLIND click and then accepted whatever attacker
#1 turned out to be, gated only on "is this a top-3 damage source
(`AIStats::isInTop`) and is its tracked toughness still positive". No trade test,
no per-attacker cap, no aim — so bodies funnelled onto the first attacker in
battlefield order until raw power covered the toughness. That is the deck84 pile
and the deck58 double-block, both exactly.

The replacement scores every attacker the body may LEGALLY block and aims with
`aimBlockerAt` (wave 53's aiming helper, previously only used by pass 3):

* pass 1 takes only blocks that **kill** — absorbing and chumping stay pass 3's and
  the survival sweep's jobs, and both still run untouched;
* the kill test is `evaluateDeclaration`, so a blocker that dies before it swings
  never counts as a killer again (deck19, deck58 T5);
* a declaration may not **spend more creature value than it kills**
  (`creatureValue` = the unbucketed raw score `DangerRanking()` buckets). This is
  what caps the gang: the second body has to earn its place and the fifth never
  can (deck84);
* an attacker already lethally blocked is skipped, so no body is added to a corpse;
* scoring prefers, in order: a block the body LIVES through, an attacker nobody is
  on yet (spreading beats piling), then the biggest threat. `isInTop` survives as a
  +100 tie-break rather than a gate.
* a pass-1 kill records `opponentsToughness[attacker] = 0` so pass 2 — which only
  counts raw power — cannot rip up a deathtouch or first-strike kill it can't see.

### 3. Pass 3 — same arithmetic, same price

`canKill`/`survives` now come from `evaluateDeclaration`; "already covered" is
`attackerDiesToCurrentBlock()` instead of a power sum; the menace/three-blocker cap
is `blockerCapOf()`; and the "always block if can kill" branch gained the same
value guard plus a +400 preference for a kill the blocker survives.

### 4. Diagnostic (development builds only)

`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)` and off unless `WAGIC_BLOCKLOG` is
set: one `W57V_BLOCK atk=… blk=… gang=N atkdies=… blkdies=… spent=… worth=… life=…
incoming=…` line per declared block, so a corpus can be COUNTED for these shapes
instead of eyeballed. Nothing is emitted in the suite or in ordinary play.

## Fixtures

Registered in `Res/test/_tests.txt`; the AI-test count goes 57 → 61.

| fixture | base 753c8d488 | after |
|---|---|---|
| `w57V_block_first_strike_trade.txt` — Youthful Knight 2/1 first strike vs one Grizzly Bears 2/2 at 20 | **RED**: Bears block, die to first strike, Knight lives, defender 20 with an empty board | **GREEN**: no block, defender 18 with its Bears |
| `w57V_block_double_strike_trade.txt` — Fencing Ace 1/1 double strike vs Mons's Goblin Raiders 1/1 at 20 | **RED**: `1 >= 1` blocks, Raiders die in the first-strike step, Ace lives, defender 20 | **GREEN**: no block, defender 18 (double strike hits an unblocked player twice) |
| `w57V_block_deathtouch_blocker_kills.txt` — Craw Wurm 6/4 vs Pitiless Gorgon 2/2 deathtouch at 20 | **RED**: no block at all (2 < 4), defender 14, both creatures alive | **GREEN**: Gorgon blocks, both die, defender 20 |
| `w57V_block_profitable_trade_control.txt` — Grizzly Bears 2/2 attacks a Hill Giant 3/3 (NEGATIVE CONTROL) | GREEN | GREEN — the plainest good block is still taken |

The three RED verdicts were taken on the archived base binary
(`~/.gatelogs/w57V-base-wagic`, built `make -B` from 753c8d488), not inferred.
The "lethal is still blocked" controls are the wave-53 fixtures
`ai_block_lethal_set_wave53.txt` and `ai_block_trample_topup_w53z.txt`, both still
green, as are `ai_block_aim_scored_attacker_w53z.txt`,
`ai_block_deathtouch_gang_w53u.txt` and `menace_defender_ai_control_vanilla.txt`.
**No existing AI test encoded the old behaviour; none was re-baselined.**

## Gate

`make -B` from 753c8d488 (base, re-confirmed) and incremental after the change,
both under the 4G memory cap, single-threaded suite:

| | base 753c8d488 | lane V |
|---|---|---|
| suite (THREADS=1) | 1245 tests / **0 failed** | 1245 tests / **0 failed** |
| AI tests | 57 / 0 | **61 / 0** (+4, all lane V) |
| `==Test timed out` | 0 | **0** |
| PARSETEST | 3350 / 0 | **3350 / 0** (no parse/render surface touched) |

Logs: `~/.gatelogs/w57-V-base-{make,suite,parsetest}.log`,
`~/.gatelogs/w57-V-gate-{suite,parsetest}.log`, final re-gate after the
diagnostic's `stage=` field landed: `~/.gatelogs/w57-V-gate2-{suite,parsetest}.log`,
`~/.gatelogs/w57-V-probe-{RED,NEW}.log`.

## Live A/B on an AI-vs-AI corpus

Ten fixed Baka-vs-Baka matchups (`WAGIC_SELFPLAY_ONESHOT`, `WAGIC_AI=baka`,
uncapped except a 900 s process backstop), run on a control binary built from
753c8d488 **with the diagnostic and nothing else** and on the lane binary, counting
every declared block:

| per DECLARED block, 10 games each | base + diagnostic only | lane V |
|---|---|---|
| blocks declared | 83 | 82 |
| …by the value passes (1-3) | 80 | 57 |
| …by the survival sweep (lethal / trample top-up) | 3 | 25 |
| **value-pass blocks that lose a creature with NO attacker death, not facing lethal** | **26 of 80 (32.5%)** | **0 of 57** |
| **value-pass declarations spending more creature value than the attacker is worth** | **20 of 80 (25%)** | **0 of 57** |
| value-pass gangs (2 bodies / 3 bodies on one attacker) | 15 rows / 6 rows | **0 / 0** |
| multi-body declarations remaining | — | survival-sweep top-ups only (16 rows at gang 2) |

The instrument is the `#W57-V` diagnostic, built into BOTH binaries and into
nothing else: the control is 753c8d488's `chooseBlockers` with the diagnostic
spliced in and no policy change (`~/.gatelogs/w57V-base-instr-wagic`; the splice is
instrument-only and is NOT in the commit). `stage=value|survival` is stamped by
snapshotting the declaration after pass 3 and before the survival sweep, because a
chump that dies without killing is CORRECT when the swing is lethal and only the
split makes the wrong ones countable — an earlier version of this table without the
split read backwards (see fleet memory `silent-instrument-failures`).

The survival sweep firing 3 -> 25 times is expected and not a regression: the value
passes no longer spend bodies on bad blocks, so more attackers reach the sweep,
which blocks them only when the incoming total is lethal.


Read the GAME-level effect as a direction, not a p-value: ten games per arm is a
small sample and the matchups are stochastic (fleet memory
`feedback-rare-event-ab-power`). The per-block rows are a different kind of claim -
they are censuses over 80 and 57 declarations, and the two headline rows go to
categorical zero, which is a shape claim, not a rate estimate.

## Falsifiable prediction for the wave-58 corpus

On the wave-58 AI-vs-AI corpus, counting AI-seat block declarations with
`WAGIC_BLOCKLOG=1`:

1. **`stage=value` blocks where the blocker dies, the attacker does not, and the
   seat is not facing lethal (`blkdies=1 atkdies=0 incoming < life`): 0 of N.**
   Pass 1 refuses them, pass 3's `canKill` no longer invents them and its
   `survives` no longer lies; only the survival sweep may spend a body for
   nothing, and only when `incoming >= life`. (Measured 0 of 57 above; the
   prediction is that it holds on a full corpus.)
2. **`stage=value` declarations whose `spent > worth`: 0 of N** (a sweep chump at
   lethal legitimately overspends — that is the game).
3. **No `stage=value` declaration of 2+ blockers on one attacker** except a
   menace / "three or more" requirement; every remaining multi-body declaration
   is a survival-sweep top-up.
4. Against a first-striking attacker the seat blocks only with a body that
   survives the first-strike step or that kills in it.

Any of these being nonzero for a non-sweep declaration falsifies the lane.

## What I did NOT verify

* **No Vita, no console build, no human play.** The change is desktop-suite and
  desktop-corpus verified only; the owner's seat is the real test.
* **The four transcripts were reconstructed, not replayed.**
  `WAGIC_REPLAY` on `transcript-1788468516` diverges at action 61 (turn 4, phase 4:
  expected `p2.no 1`, got `p1.choice 0`) — human-seat transcripts were already
  flagged unverified when record/replay shipped (2026-09-01). Separately and worth
  a wave-58 docket item: **blocks committed through `MTGCardInstance::toggleDefenser`
  are not click-logged at all**, so the wave-53 survival sweep and the CR 509.1c
  repair sweep are INVISIBLE in a transcript and cannot replay. Every block quoted
  above is a click-path block; a sweep block in those same combats would not appear.
* **The deck84 five-on-one pile has no fixture.** Pass 1's old gate required
  `AIStats::isInTop`, and `AIStats::stats` is built from observed DAMAGE EVENTS —
  it is empty until three distinct sources have dealt damage, which no single-combat
  `[AI]` fixture reaches. So the pile shape is RED-on-base only on a stats-primed
  board (a multi-turn fixture I did not build). It is covered instead by the live
  A/B above and by prediction 3. The rule that caused it is fixed and pinned
  indirectly by the deck58/deck19 fixtures, which exercise the same pass-1 path.
* **Creature "value" is a heuristic**, not a Magic-strategy truth: power + toughness
  + converted cost + one point per basic ability. It prices a Craw Wurm above a
  Grizzly Bears, which is all the gang cap needs; it does not know about card
  advantage engines, auras attached, or what is in a hand.
* **`AIPlayerGPT`'s bundled-blockers path is untouched** — it overrides
  `chooseBlockers`, and the model-driven policy is unchanged. The GPT seat falls
  back to this chooser only on failure, and inherits the improvement there.
* No change to attack declaration, damage ordering (`orderBlockers`), or any
  render/parse surface.
