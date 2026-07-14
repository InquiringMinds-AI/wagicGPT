# Wave-8 review — deck109 (Hellrider, mono-red aggro / RDW)

**Corpus:** PRIMARY control arm `matchups-20260714-044131`, one round-robin, 6 games (deck109 seat).
Binary carries the full wave-7 engine ledger (incl. #5 blocker-seam lethal arithmetic). Guide in play =
the wave-7 revision (byte-identical to the live `deck109_strategy.txt`).

**Record: 4/6** — beat 140, 44, 135, 131; **lost to 133 and 110.** Same 4/6 as wave-7 but a DIFFERENT
opponent split: this wave it **BEAT 44** (UB faeries — the evasion matchup that beat it last wave) and
**LOST to 133** (mono-black devotion: Obliterator + Gray Merchant — which it beat last wave). Per the
user doctrine the record is context only: 4/6 off a reshuffled opponent+draw slate is matchup/variance,
not a guide verdict.

| seat file (epoch) | result | opp | end (my / opp life, turn) |
|---|---|---|---|
| 1784022870 | WIN  | 140 | 6 / 0, T27 (grind, sweeper+lifegain) |
| 1784023977 | WIN  | 44  | 8 / 0, T9 |
| 1784024496 | WIN  | 135 | 20 / -3, T9 |
| 1784025145 | WIN  | 131 | 20 / 0, T9 |
| 1784027795 | **LOSS** | 133 | -4 / 3, T13 |
| 1784029007 | **LOSS** | 110 | -3 / 20, T8 |

---

## Harness-defect sweep (run first, per skill Step 2) — CLEAN

- **DESYNC: 0** across all 6 games (ask/priority head-int == recorded `choice`).
- **ENGINE CAST NO-OP: 0.** No same-pick/mana-unchanged loops. (The one flagged "repeat" — vs140 s29/s31
  both "Cast Legion Loyalist" — is **T17 then T19, two turns apart with attacks + a board-wipe between**;
  a legit recast of a 2nd copy after deck140 swept, mana spent each time. Not a no-op.)
- **Phantom "missed cast" / cast-didn't-happen reasoning: 0** (regex over every reply). The deleted
  Hellrider fallback shows **no regression** — wave-8 re-confirms wave-7.
- **NUMERIC-OFFSET: N/A** (deck runs no X-spells).
- **Attacker commitment: 65/65** offered-attacker slots declared (0 "no attackers" in 6 games). Frozen
  spine intact.

## Mandatory inheritance checks

- **Hellrider-fallback DELETION regression (wave-6/7 inheritance):** 0 no-ops, 0 loops, 0 phantom framing.
  Keep it deleted. Core `hand -> stack` anchor removal condition stays met from this seat (already ADOPTED
  in the wave-8 A/B per the brief — nothing to do).
- **Phantom-lethal fixation -> decline-a-listed-BODY (my 1-seat watch, promote at 2nd instance):** **NO 2nd
  instance in the creature-body shape.** Every "Cast nothing" this wave was either declining *Pillar*
  (a burn spell, not a body — see below) or a genuine out-of-mana window (vs131 s19: opp at 3, burn spent,
  then attacked for lethal). No turn declined a listed CREATURE while fixated on an unaffordable bomb.
  **Keep the watch at 1 seat; do NOT promote.**
- **Evasion-matchup class is a construction fact, not a play failure:** restated below, not relitigated.

---

## THE DECIDING LEVER — the shipped blocker-seam lethal math (#5) is PRESENT-AND-LOSING

Engine change #5 fired in **all 3** blocker prompts, printing the exact rider:
`Unblocked, these attackers deal up to N - you would be at M - NOT lethal: block only where the trade
favors you; taking damage while ahead is often correct.`
The pilot **read it and disobeyed it in BOTH loss games** — a representation-beats-instruction wall at
the blocker seam (the fact is fully surfaced and the pilot acts against it).

### (1) vs133 seq22 — blocked Phyrexian Obliterator into its sacrifice punisher (the key misplay)
- me **10** / opp 10. Surfaced: `deal up to 7 - you would be at 3 - NOT lethal`.
- Pilot chose **B1:A2, B2:A2** — both Goblins block Phyrexian Obliterator (5/5 trample). PLAN: "block ...
  to prevent it from dealing lethal damage to me via trample (5 damage would kill me at 10 life if
  unblocked)" — **false, and directly contradicted by the surfaced "at 3 - NOT lethal" line it was
  looking at.**
- Result (verified in the next records' events): each Goblin dealt 1 damage to Obliterator ->
  `auto=@damaged(this)... sacrifice` fired -> **I sacrificed 2 permanents (2 Mountains -> manaless)**, lost
  both Goblins, and STILL went to 5 (3 trample + 2 Bloodghast). Taking the hit unblocked (->3, survivable)
  was strictly better and keeps 2 Goblins + 2 lands.
- Engine text confirmed: `text=Trample -- Whenever a source deals damage to Phyrexian Obliterator, that
  source's controller sacrifices that many permanents.` Blocking it is a trap; the pilot did not know it
  (its plan called the sacrifice "avoiding the loss of permanents" — backwards).
- **This worsened the game.** Manaless at 5, it lost T13 at -4. Against a hard matchup (devotion + Gray
  Merchant lifegain drained opp 2->10 / me 18->10 the turn before), but the block was a genuine, avoidable
  decision error, not matchup.

### (2) vs110 seq13 — high-life reflexive block of a 6/6
- me **19** / opp 18. Surfaced: `deal up to 7 - you would be at 12 - NOT lethal`.
- Pilot gang-blocked **Master of Etherium (6/6)** with 2 Goblins. PLAN: "prevent the lethal 6 damage that
  would drop me to 13 life" — acknowledges 13, blocks anyway. The 6/6 survives, 2 Goblins die for nothing,
  2 attackers lost while racing. **Floor violation** (guide: "above 8 -> declare NO blockers"). Matchup was
  lost regardless (affinity alpha-struck for 21 next turn), so non-outcome-changing here — but it is the
  same disobey-the-surfaced-math class.

### Why this matters — intra-guide contradiction (wave-7 deck44 pattern, reproduced at deck109's seat)
The vs133 block is the 2nd seat of the wave-7 "block-a-damage-PUNISHER because a survival/chump rule
re-authorized it" guise. Two deck-guide defects let it through:
- **The BLOCKING rule (b)** — "chump when unblocked attackers would drop you to 8 or less" — is TOO EAGER:
  it fired at me10->**3** (which the surfaced math flagged **NOT lethal**) and re-authorized blocking,
  overriding the Phyrexian Obliterator note. The chump threshold (<=8 projected) FIGHTS the surfaced lethal
  line (<=0). **Answer to the wave-7 watch: the surfaced math does NOT improve the guide's blocking rule —
  it FIGHTS rule (b), because (b) chumps at <=8 while the math only flags actual lethal.**
- **The Phyrexian Obliterator note** says "attack only past it ... take the 5" but **never says "do not
  BLOCK it."** The concrete chump rule (which NAMES the block option) beat the buried prohibition.

**Fix = per-deck, layer-correct** (representation is already shipped and losing; do NOT escalate wording of
the surfaced line — that's the model-experiments frontier for synthesis). Re-anchor rule (b) to the
surfaced lethal line (threshold-reuse), and hoist a "NEVER block Phyrexian Obliterator" absolute as (b)'s
explicit exception. See strategy.txt diff + notes.md freeze-check.

---

## Minor leak — Pillar of Flame false-cost / burn-hoarding (vs140 x2, vs110 x6)

Across two games the pilot repeatedly declined its ONLY castable option, `Cast Pillar of Flame {r}`,
choosing "Cast nothing." Stated reasons: Pillar "requires {1}{R}" (**FALSE** — the engine option literally
reads `{r}` and the primitive is `mana={R}`), or "keep mana open for instant interaction" (it holds no
instants). Non-outcome-changing in both (vs140 ground it out and won; vs110 died to the alpha strike with
Pillar in hand), so NOT the #1 slot — but a recurring false-cost belief + hold-mana metastasis worth a
one-line corrective, tied to the named card, folded into an already-obeyed section.

## What the pilot did WELL — freeze verbatim
- 65/65 attacker commitment, 0 "no attackers"; Unleash counter always taken; MP2 development every turn.
- Burn to face by default; 0 burn at own permanents; correct exception fired (vs131 s7: gang-blocked
  Young Pyromancer with 2 Goblins to kill the token engine — exactly guide exception (a)).
- 0 desyncs / 0 no-ops / 0 phantom framing.

## Deck-construction / meta flags (upstream — restate, do NOT relitigate; NOT guide-fixable at 4/6)
- **Evasion / untouchable-board class stands.** vs44 (fly+shroud) it won this wave on a good draw; the
  structural weakness (no reach, nothing to block flyers, no targets on shroud/pro-red) is unchanged.
- **Devotion + Obliterator + Gray Merchant (deck133)** is a sibling hard matchup: cannot profitably attack
  into Obliterator, and Gray Merchant's drain undoes the race. Race-the-face is already the plan.
- **No card advantage / mana sink** (standing since wave-5): the vs110 loss was out-developed + alpha-struck.
  Inherent to the archetype. Not recommended to change at 4/6.

## vs140 win, T27 — winning-but-slow, NOT a durdle (no action)
deck140 ("Wipe Them Out!") swept the board repeatedly and gained life (opp 3->7 mid-game); the pilot
redeployed a body and attacked every turn and ground it out. Latency-normal, ahead throughout. Per timeout
triage this is matchup shape, not passivity — nothing to fix.
