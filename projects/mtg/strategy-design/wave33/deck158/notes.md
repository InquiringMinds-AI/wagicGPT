# deck158 — development notes (wave 33)

Corpus `matchups-20260819-070739`, binary `bdb30d8e7`, 6 games at this seat. Layer-routing:
everything here is ENGINE / RENDER / HARNESS. Nothing below belongs in guide prose.
Card facts verified against `bin/Res/sets/primitives/borderline.txt` + `_macros.txt` and,
where load-bearing, against Scryfall Oracle text.

---

## N-158r — THE PREGAME HAND-ONLY SERIALIZER LATCHES THROUGH THE ON-THE-PLAY PLAYER'S FIRST LAND DROP. **HIGH (cost). NEW.**

**Severity: HIGH by cost, not by correctness** (all three land-drop choices were right).
It produced the worst latency and the longest reply at this seat: **214.5 s / 14,570
chars on a three-option land drop.**
**Repro quality: PERFECT — deterministic, source-read, and the discriminator is exact.**

**Symptom.** In the three games where deck158 was on the play, the turn-1 land drop is
served the `--- YOUR OPENING HAND ---` pregame prompt: **no battlefield, no opponent
board, no `Mana available:` line**, and carrying the pregame satisfiability header. In
the three games where deck158 was on the draw, the same decision gets the normal board
prompt.

| game | deck158 seat | seq 2 prompt form | latency | reply chars |
|---|---|---|---|---|
| 158 v 36 | **on the play** (deck0) | pregame, turn 0, NO board | 12 s | 696 |
| 158 v 116 | **on the play** (deck0) | pregame, turn 0, NO board | **214.5 s** | **14,570** |
| 158 v 105 | **on the play** (deck0) | pregame, turn 0, NO board | 12 s | 804 |
| 139 v 158 | on the draw (deck1) | normal, turn 1, board present | 6 s | 588 |
| 152 v 158 | on the draw (deck1) | normal, turn 1, board present | 8 s | 731 |
| 146 v 158 | on the draw (deck1) | normal, turn 1, board present | 6 s | 604 |

**Mechanism (source-read, not hypothesis).** `projects/mtg/src/AIPlayerGPT.cpp:2544`:
```cpp
bool pregame = (observer && observer->turn == 0);
```
and at `:2556-2557`
```cpp
if (pregame)
    u << "--- YOUR OPENING HAND ---\n" << serializePregameState();
```
The gate is the **turn counter**, not the ask. The player on the play takes their first
main phase while `observer->turn` is still 0, so their first land drop — an ordinary
in-game decision — is served `serializePregameState()`. The player on the draw acts at
turn 1 and is unaffected. `planActionsStale` is suppressed on the same predicate
(`:2582`), so the on-the-play pilot also loses its stale-plan notice for that decision.

**What it costs.** The satisfiability header is a HAND-WIDE claim
(*"Playing every land in this hand would cover the cost of: Orcish Bowmasters {1}{b},
Feed the Swarm {1}{b}, March from the Black Gate {1}{b}, March from the Black Gate
{1}{b}"*) that is true across the whole game and false for this turn, when only one land
may be played. With no board and no `Mana available:` line to arbitrate, the model tried
to reconcile the two and could not. **116 seq 2, verbatim from the reply:**
```
If I play a Swamp, I have 1 mana. I can't cast Bowmasters ({1}{B}) either.
Wait, {1}{B} is 2 mana. Is it possible the prompt is flawed or I am misinterpreting
"Mana sources"? ... If I have 0 lands on the battlefield, I have 0 mana. If I play a
land, I have 1 mana. I cannot cast a 2-mana spell. This is a contradiction.
```
6,591 of those characters landed **after** its own PLAN line (`post_plan_overrun`), on a
choice between Swamp, Mountain and nothing.

**Fix direction.** Gate the pregame serializer on the ASK, not the clock — the
`pregameMulliganDecision` / `pregameChooseBottom` / `pregameLeyline` entry points
(`:8466`, `:8493`, `:8568`) already identify themselves; a flag set on entry and cleared
on exit is exact, where `turn == 0` is a proxy that is wrong for half the seats. Second
choice: `turn == 0 && the ask is one of the pregame kinds`.

**Validation metric next corpus:** zero `--- YOUR OPENING HAND ---` prompts on any ask
whose option list is a land drop or a cast, measured at BOTH seats of every game
(the on-the-play seat is the only one that exercises it — a single-seat check will
report a false PASS half the time).

---

## N-158s — FORAY OF ORCS RENDERS `damage N` WITH ZERO LEGAL TARGETS, AND THE PILOT READ IT AS FACE DAMAGE. **HIGH (correctness). NEW — a defect on i6's own new surface.**

**Severity: HIGH.** It plants a false term in a lethal computation, and the pilot cited
the annotation *by name* as its authority for the false belief.
**Repro quality: PERFECT** (1 instance, but static and deterministic — the magnitude is
computed without consulting the target set).

**Repro.** `1787141266-...-deck158-...-vs-deck116.jsonl`, **seq 12, turn 8**.
Board, verbatim: `Opponent battlefield (creatures: 0): Tropical Island [tapped]; Forest
#1 [tapped]; Forest #2 [tapped]; Forest #3 [tapped]; Quicksilver Amulet {4} [artifact]`.
Option line, verbatim:
```
2. Cast Foray of Orcs {3}{r} {right now: Army 6/6 -> 8/8, damage 8} {card text: "Amass
Orcs 2. When you do, Foray of Orcs deals X damage to target creature an opponent
controls, where X is the amassed Army's power. ..."}
```
Reply, verbatim:
```
Since there are no opponent creatures to target, the spell deals 8 damage directly to
the opponent (as per the "right now" text: Army 6/6 -> 8/8, damage 8). This reduces the
opponent's life to 1.
PLAN: Cast Foray of Orcs to grow the Army to 8/8 and deal 8 damage to the opponent
(leaving them at 1 life).
```
Ground truth (Scryfall Oracle, `Foray of Orcs`, `{3}{R}` Sorcery): *"Amass Orcs 2. When
you do, Foray of Orcs deals X damage to **target creature an opponent controls**, where
X is the amassed Army's power."* Narration at seq 13 confirms: two counters added
(`now 7/7`, `now 8/8`), **no damage event, opponent life unchanged at 9**.

The `{card text: ...}` on the SAME LINE says "target creature an opponent controls". The
model chose the magnitude tag over the card text — which is the correct instinct in
general (the tag is the dynamic, situation-aware number) and is exactly why the tag must
never assert an effect that cannot happen.

**Why it did not cost the game:** the pilot swung for 9 into an empty board and won at
turn 8 regardless. Had the board contained a chump blocker, "opponent is at 1" would
have been the premise for a skipped attack or a wrong removal target.

**Fix direction.** `dynamicMagnitudes` must evaluate the target set before emitting the
damage clause. Either suppress the clause (`{right now: Army 6/6 -> 8/8}`) or qualify it
(`{right now: Army 6/6 -> 8/8; damage 8 — but there is no legal target on the board, so
no damage will be dealt}`). **Generalises past Foray:** any magnitude attached to a
targeted rider must be gated on the rider having a legal target, or the annotation
becomes an assertion that the effect will occur.

**Validation metric:** zero `damage N` clauses emitted on an offer whose targeted rider
has an empty legal-target set.

---

## N-158t — THE BLOCKER FORECAST'S NAIVE 1-ON-1 TRADE IGNORES "WHENEVER THIS BECOMES BLOCKED, IT GETS +X/+X". **MEDIUM. NEW.**

**Severity: MEDIUM** (the forecast is the pilot's only combat-math instrument and it was
wrong in the direction that loses a blocker for nothing).
**Repro quality: PERFECT** (1 instance, static ability, resolution captured).

**Repro.** vs105, **seq 13, turn 7**, my life 18, poison 1 of 10. Rendered:
```
A1. Ichorclaw Myr (1/1) deals 1 [infect] ... {text: Infect -- Whenever Ichorclaw Myr
    becomes blocked, it gets +2/+2 until end of turn.}
B1. Orcish Bowmasters (1/1) [flash] - may block A1 (both die), A2 (your blocker dies,
    attacker lives)
```
`(both die)` is false. The Myr becomes 3/3 on being blocked; Bowmasters' 1 damage cannot
kill it. Resolution, from the seq 14 narration:
```
- You declared blockers: Orcish Bowmasters blocks Ichorclaw Myr
- Damage: 1 dealt by Orcish Bowmasters to Ichorclaw Myr
- Infect damage: 3 from Ichorclaw Myr to Orcish Bowmasters - dealt as 3 -1/-1 counters ...
- Your Orcish Bowmasters: battlefield -> graveyard
```
Truth was `your blocker dies, attacker lives`. Oracle confirmed (Ichorclaw Myr, `{2}`
1/1 artifact creature, Infect, *"Whenever Ichorclaw Myr becomes blocked, it gets +2/+2
until end of turn."*) and the rendered card text on the same screen already carries it.

**The model reproduced the engine's error independently** (*"If I block it with Orcish
Bowmasters, the Myr becomes a 3/3. Since Bowmasters is a 1/1, both creatures die in
combat"*), so it is not purely an echo — but the render agreed with it and removed any
chance of a correction. No harm: the block was correct on poison grounds either way
(1 poison prevented for a body that was going to die to Cystbearer anyway).

**Fix direction.** The forecast already simulates the pairing; apply becomes-blocked
static/triggered P/T modifiers on the attacker before computing the trade. The class is
small and enumerable (becomes-blocked pumps, blocked-by-one-creature bonuses) and the
render's honesty at this seam is what the whole guide-side blocker procedure is built
on. If the general case is expensive, a cheap floor: when the attacker's text contains
a becomes-blocked clause, emit `(trade unknown - it grows when blocked)` rather than a
confident wrong verdict.

---

## N-158u — BATCHED `-1/-1` COUNTER NARRATION PRINTS THE FINAL TOTAL ON EVERY LINE. **LOW, latent. NEW.**

**Severity: LOW** (no misplay traced, no verbalized confusion).
**Repro quality: PERFECT** (18 distinct prompts carrying the same three-line block).

vs105 seq 14 narration, verbatim:
```
- Counter added to Orcish Bowmasters: -1/-1 (now -2/-2) [from Ichorclaw Myr]
- Counter added to Orcish Bowmasters: -1/-1 (now -2/-2) [from Ichorclaw Myr]
- Counter added to Orcish Bowmasters: -1/-1 (now -2/-2) [from Ichorclaw Myr]
```
Three separate counter events, each reporting the POST-batch total. The correct running
sequence for a 1/1 taking three -1/-1 counters is `(now 0/0)`, `(now -1/-1)`,
`(now -2/-2)`. The final value is right; the intermediate ones are not.

**The `+1/+1` path is correct** and is the control: `Ugluk of the White Hand: +1/+1
(now 4/4)` then `(now 5/5)` from a printed 3/3; `Orc army` running totals are correct
across all 21 distinct counter lines and 94 Army renders. So the defect is specific to
the batched infect-damage `-1/-1` application, which narrates after the whole batch has
been applied rather than per event.

**Fix.** Emit the counter narration inside the per-counter loop (as the `+1/+1` path
does), or snapshot the P/T before the batch and increment. Related to `0280a08b4`, which
routed counter events through every engine path and closed N-158l (empty `[from ]` —
now **0 occurrences**, was 38 prompts).

---

## N-158v — `Day {0}` / `Night {0}` RENDER AS PERMANENTS ON MY BATTLEFIELD; A COSTLESS BACK FACE PRINTS `{0}`. **LOW, latent. NEW — i5 residual.**

**Severity: LOW** (no misplay traced). **Repro quality: PERFECT** (20 renders, vs152).

Two shapes, both in the vs152 game (deck152 runs daybound/nightbound werewolves):

1. **The day/night designation is rendered as a permanent, on the wrong side, with a
   confabulated cost.** vs152 seq 22, `Your battlefield` line, verbatim tail:
   `... Sauron, the Lidless Eye {3}{r}{b} (4/4); Night {0}; Snarling Warg {3}{b} (4/4)
   (printed 3/4) [menace] ...`
   Day/night is a game-wide designation (CR 730), not a permanent and not *mine*.
   **12 occurrences on `Your battlefield`, 4 on `Opponent battlefield`** (seq 13-28).
2. **`Moonrage Brute {0} (3/3) [first strike, nightbound]`** — 4 renders. Scryfall:
   `Brutal Cathar // Moonrage Brute`, layout `transform`; the back face's `mana_cost` is
   the **empty string**. A back face has no mana cost; `{0}` is a different claim and a
   false one.

Real `{0}` costs render correctly at the same seat (`Mox Jet {0}`, `Mox Sapphire {0}`,
Ornithopter), so i5's `manaCostToken()` routing holds for cards that HAVE a `{0}` cost.
The residual is the **absent-cost** case: an object with no mana cost at all should emit
nothing, not `{0}`. Same root for the designation lines, which have no cost by nature.

**Fix.** `manaCostToken()` should distinguish "cost is zero" from "object has no cost",
and the battlefield serializer should exclude non-permanent designations from both
battlefield lines (or render day/night once, as a game-state line, which is what it is).

---

## L-158-2 — CONSUMED-ASK INSTRUCTION BOILERPLATE IS 15-19% OF THE GAME LOG. **Owner-directive-3 evidence.**

The wave-33 docket estimates the consumed-decision decoration at "~320 chars in a 12k
prompt". At this seat the `{card text: ...}` / `{right now: ...}` / `[cost: ...]`
decoration inside the game log measures **0 chars** — already clean. What is NOT clean
is the **menu echo replaying a consumed ask's full instruction text**, in the imperative,
as history:

| game | game-log chars (last prompt) | consumed-ask echo lines | echo chars | share of log |
|---|---|---|---|---|
| vs36 | 6,149 | 3 | 950 | **15.4 %** |
| vs152 | 8,960 | 5 | 1,672 | **18.7 %** |
| vs146 | 10,942 | 5 | 1,980 | **18.1 %** |

Representative line, verbatim from the log:
```
- TARGET CHOICE for Orcish Bowmasters - its "damage any target" ability (this
spell/ability is already on the stack and needs a target - it is NOT a cast or phase
step). Pick the ONE target it will affect from the list below, and answer with the
chosen TARGET's name (not "Orcish Bowmasters") -> Salvage Slasher (1/1) [opponent's
battlefield] [tapped] - "Salvage Slasher gets +1/+0 for each artifact card in your
graveyard."
```
The mulligan echo is the same shape (*"Keep this hand, or mulligan (shuffle back and
draw 7 again, bottoming one more at the next keep)? -> Keep this hand"*).

The narrated form of a consumed decision needs only *what was chosen*
(`- You targeted Salvage Slasher with Orcish Bowmasters' damage ability`). Everything
before the `->` is live instruction addressed to a decision that is over. **Measured
here at 5-6× the docket's estimate**, and it grows with game length, which is where the
prompt is already largest.

---

## Steady state / carried, re-confirmed at this seat

- **N-158e (flash windows), KNOWN-OPEN, 3rd corpus, steady:** 2 `priority` decisions in
  6 games (vs152 seq 20 and seq 24, both **Upkeep**), each offering only
  `creaures gain 2/0 with Sauron, the Lidless Eye [cost: ...]`; both passed.
  **Orcish Bowmasters was never offered at instant speed in 6 games.** Not counted
  against the guide.
  *(Cosmetic, same line: the ability label reads **"creaures"** — a typo in the
  activated-ability option text.)*
- **N-158d (Foray's target menu names the Army): UNEXERCISED.** Foray was cast once and
  had no legal target, so no target menu was raised.
- **N-158f / N-158i (Assault on Osgiliath): UNEXERCISED for the THIRD consecutive
  corpus** — Assault has never been offered under binaries `7fabd9bd0`, the wave-31
  binary, or `bdb30d8e7`, despite 2 copies in the 60-card list. Corpus play will not
  close these; the probe deck specified in wave-32 notes (4× Assault, 4× March, 8
  Mountain / 8 Swamp, one pinned game) is now the only instrument.
- **N-158n (name-form BLOCKS parser): fix shipped, UNEXERCISED.** All 8 blocker replies
  used `Bn:An` labels. `all_assignments_illegal`: **0 occurrences.**
- **N-158j (poison unrendered): CLOSED** — 11/11 infect-exposed prompts carry the count,
  both the status line and the per-event narration.
- **N-158k (Feed the Swarm target-menu costs): CLOSED at `948578887`** — 3/3 menus,
  11/11 target lines, all numbers correct. **The wave-33 brief's KNOWN-OPEN list is
  stale on this item**; the fix and its PARSETEST (`[W33-N158k]`, `AIPlayerGPT.cpp:10825`)
  are in the step-1 batch it describes.
- **N-158l (empty `[from ]`): CLOSED** — 0 occurrences (was 38 prompts). Attribution:
  interim `0280a08b4`, not step-1.
- **N-158m (amass macro scanner): CLOSED** — 59/63 offers, 59/59 of non-trigger-gated.
- **L2 cross-zone dup-name, witnessed:** vs36 seq 12, the pilot wrote *"'Your
  battlefield ... Orcish Bowmasters'. But the 'Your hand' list ALSO lists 'Orcish
  Bowmasters'. This is a contradiction in the provided text."* Both statements were
  true (2× Bowmasters in the list). Cite, don't re-diagnose.

## Corpus hygiene

- **Crash gate: 0 signatures** across all 6 stderr files at this seat
  (`game-158v116-1787141263`, `game-158v36-1787141261`, `game-158v105-1787152099`,
  `game-139v158-1787141870`, `game-152v158-1787143904`, `game-146v158-1787150014`),
  grepping `FATAL|Segmentation|Assertion|terminate called|core dumped` with
  `/usr/bin/grep`.
- **Fallbacks: 0/138.** No `retracted_choice`, no `unparsed_reply`, no
  `all_assignments_illegal`, no defers, no truncations.
- **Both seats logged in all 6 games.** (The opponent-side logs of the vs139 and vs152
  games carry a timestamp one second earlier than mine — `1787141873` and `1787143907`
  against my `1787141874` and `1787143908`. Noted because a prefix-exact pairing script
  will silently report them missing.)
