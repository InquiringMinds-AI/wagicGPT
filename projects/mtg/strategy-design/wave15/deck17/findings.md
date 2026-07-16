# deck17 (Faeries) — GUIDELESS FIRST-PASS findings (wave-15)

Corpus: `matchups-20260716-040138`, binary `/tmp/wagic-aad32bc7f`. deck17 played all 6
games with NO guide (core prompt only). Record 3/6.

Seat files (all `ai_baka_deck17-*` acting seat):
- vs21 (Goblin Gang aggro): **WON**, opp -9 (t22). Flyers + Control Magic (stole Boggart Ram-Gang) + Plumeveil walls.
- vs44 (UB Faerie tempo, mirror-ish): **WON**, opp -4 (t21). Flyers under both anthem lords + Control Magic (stole Faerie Miscreant).
- vs133 (mono-B recursion): **WON** (adjudicated 20/7). Briarberry Cohort beatdown, Plumeveil wall, life never dropped.
- vs135 (snow midrange): **LOST** (adjudicated 8/13). Land-screwed + huge reasoning tax; discarded Persuasion.
- vs140 (Rakdos lifegain / Blightsteel Colossus): **LOST** (adjudicated 17/64). Went PASSIVE - stopped attacking ~18 turns while opp gained to 64; held Persuasion until t33.
- vs110 (affinity): **LOST** (adjudicated 14/21). Control Magic stole Vault Skirge; over-blocked a 0/2 Ornithopter; could not race.

Adjudications are the latency artifact (Spark @0.75, med 11.4s / p90 93s) - evaluate the POSITIONS.

## The deck (rules text derived from translog options / battlefield lines)

UB (splash W via hybrid) FLYERS tempo/control. Kill by flying over; medium clock (t8-14).
- **Islands x24** - the only mana; every colored pip in the deck is hybrid or mono-U.
- **Oona's Gatewarden {u/b} 2/1** - flying, **DEFENDER**, wither. A WALL. Never attacks.
- **Cloud Sprite {u} 1/1** - flying; **"can block only creatures with flying."** A flyer that can ONLY block other flyers (real restriction, confirmed by engine + model reasoning).
- **Briarberry Cohort {1}{u} 1/1** - flying (cheap evasive body).
- **Scion of Oona {2}{u} 1/1** - flash, flying, "Other Faeries get +1/+1; other Faeries have shroud." Anthem #1 (Faeries only).
- **Wasp Lancer {u/b}{u/b}{u/b} 3/2** - flying. Best 3-mana body; all three pips are hybrid -> castable off 3 Islands.
- **Glen Elendra Liege {1}{u/b}{u/b}{u/b} 2/3** - flying, anthem: other blue AND other black creatures get +1/+1 (confirmed: Sentinels went 4/5 under Liege+Scion). Anthem #2 (broad).
- **Sentinels of Glen Elendra {3}{u} 2/3** - flash, flying.
- **Faerie Swarm {3}{u}** - flying; P/T = number of blue permanents you control (0/0 -> 4/4 with 4 blue permanents). Grows late.
- **Plumeveil {w/u}{w/u}{w/u} 4/4** - flash, flying, **DEFENDER**. A WALL. Never attacks. Castable off 3 Islands (hybrid W/U).
- **Mahamoti Djinn {4}{u}{u} 5/6** - flying finisher.
- **Control Magic {2}{u}{u} x4** + **Persuasion {3}{u}{u} x2** - SIX steal auras: gain control of target creature.

Win path: walls (Gatewarden/Plumeveil, 8 total) survive early; flyers + two anthem lords + Mahamoti +
STOLEN creatures fly over. The six steal effects are the deck's signature.

## What the CORE PROMPT already got RIGHT (do NOT re-teach)

- **Land every turn**: 100%. Never declined a land drop.
- **Cast a creature on curve**: the "every listed option is legal AND payable" rule rescued nearly every
  cast - despite the model's hybrid panic (below), it almost always ENDED UP casting a body.
- **Steal-into-no-target discipline**: vs44 s16 Control Magic showed "the only legal targets are YOUR OWN
  right now" -> model correctly declined. Core annotation working - don't touch.
- **Block only favorable trades / take damage while ahead**: obeyed well vs133 (walls), vs21. Core carries it.
- **Won all three games where it kept flying**: vs21/vs44/vs133 are the deck working as intended.

## Where DECK-SPECIFIC knowledge was LOAD-BEARING (teach exactly this)

### 1. HYBRID MANA - the #1 leak (tax + real misplays). Every game.
The model repeatedly believes hybrid pips need the off-color it lacks: `{u/b}` "needs a Swamp",
`{w/u}` "needs White", and even MISCOUNTS `{u/b}{u/b}{u/b}` as **6 mana** (vs135 s11: "Wasp Lancer costs
6 mana, I only have 3"). Consequences:
- **Real under-deployment (x2):** vs44 s7 and vs135 s11 the model cast **Scion of Oona (1/1)** because it
  thought **Wasp Lancer (3/2)** was uncastable - deployed the smaller body off the same mana. Decision-level
  misplay, not just tax.
- **Enormous reasoning tax:** vs44 s13/s18/s23/s26 are thousand-token oscillations re-deriving "can I pay
  {u/b} with an Island?" - driving the p90 latency that costs adjudications (this deck lost 3 games to
  adjudication, twice while even/ahead on tempo but grinding slowly).
- The core "it would not be listed if unpayable" rule DOES rescue the final choice, but only after the tax.
  The right fix is a flat deck-fact: **all your spells cast off Islands alone.**

### 2. TWO cards are DEFENDERS - never attack with them. (vs21, vs44)
Oona's Gatewarden and Plumeveil have Defender and CANNOT attack. The model calls Gatewarden "a future
threat" (vs21 s3) and tries to attack with Plumeveil (vs21 s22 `ATTACK: A1, Plumeveil (first), Plumeveil
(second)` - engine correctly dropped the two defenders, recording only Scion). It doesn't KNOW they can't
attack, wasting reasoning and mis-planning lethal. They are WALLS. Teach it once, flatly.

### 3. The ATTACK FLOOR is not reliably obeyed. (vs140, the clearest loss)
vs140 the model cast bodies but STOPPED ATTACKING for ~18 turns (only 3 attacker windows in 33 turns)
while the opponent's lifegain climbed 17->64. Its flyers were unblockable (opp had no flyers/reach) yet
sat home. Against a deck it cannot out-grind, not swinging the air every turn is the loss. The cast floor
transfers, the attack floor does NOT - needs its own explicit per-turn rule keyed on "they have no flyer".

### 4. The SIX steal auras are under-prioritized / held too long. (vs140, vs135)
Control Magic was used well when forced (vs21 stole Boggart Ram-Gang; vs44 stole a blocker; vs110 stole
Vault Skirge). But Persuasion was **held until t33** vs140 (opp already at 64) and **discarded unused**
vs135. Six steal effects is the deck's best interaction and answer to a bomb (Blightsteel, Cranial-Plated
attacker) - it should fire early on the opponent's best creature, not be hoarded.

### 5. Cloud Sprite's block restriction cascades into rules confusion. (vs135)
Cloud Sprite genuinely "can block only creatures with flying." The model noted this correctly, then spent
a giant reply (vs135 s12) agonizing whether **Scion of Oona** (a normal flyer) shared the restriction - it
doesn't. State both facts so the model stops re-deriving: Cloud Sprite blocks only flyers; every OTHER
flyer blocks anything.

## Non-guide observations (-> notes.md)
- Degenerate repetition blowups (vs135 s6: "+1/+1 from Glen Elendra Liege (if I had one out)" x100 then
  "good blocker" x-infinity -> choice -1 heuristic). Model-incapacity/latency, not guide.
- Stale board-annotation contradiction `[tapped - untaps and can attack next turn] [attacking]` (vs21 s23)
  and a re-cast Gatewarden the log said "died" (vs135) - reasoning-tax multipliers, representation layer.
- choice=-1 long-ramble fallbacks (vs110 s15; vs44 s26 truncation) - the unparsed guard working; harmless.
- The mixed-form `ATTACK: A1, <name>, <name>` appeared (vs21 s22) but the names were DEFENDERS (ineligible),
  so the "drop" was CORRECT here - not a clean mixed-form-bug witness at this seat.
