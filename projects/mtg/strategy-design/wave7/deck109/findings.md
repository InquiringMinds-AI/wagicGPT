# Wave-7 review — deck109 (Hellrider, mono-red aggro / RDW)

**Corpus:** `matchups-20260713-222831`, one round-robin run, 6 games (deck109 seat).
**Record: 4/6** — beat 140, 135, 133, 131; **lost to 44 and 110.** First slip from the 5/6 plateau.

| seat file (epoch) | result | opp | end (my / opp life, turn) |
|---|---|---|---|
| 1783999716 | **LOSS** | 44  | -9 / 10, T12 |
| 1784001159 | WIN     | 140 | 15 / 0, T15 |
| 1784003680 | WIN     | 135 | 12 / -1, T9 |
| 1784004399 | WIN     | 133 | 14 / -1, T10 |
| 1784004822 | WIN     | 131 | 20 / 0, T11 |
| 1784009179 | **LOSS** | 110 | -5 / 5, T8 |

**Headline.** The 5/6->4/6 slip is MATCHUP + VARIANCE, not a guide regression. Both new losses are decks
whose clock structurally beats mono-red GROUND aggro, and the deck44 loss rode a mana-light draw on top.
The frozen spine stayed obeyed (25/25 full attacker commitment, all burn to face, 0 hoarding). The
wave-6 fallback-rule DELETION caused ZERO regression -- no re-pick loops, no phantom "missed cast"
reasoning, no cast no-ops (audit below). **Obeyed-but-losing mode: freeze the spine, do not rewrite.**

The task's premise that deck44's *counter seam* ate our threats does NOT hold for this game: **no counter
fired against us** -- every deck109 cast resolved (`hand -> stack` confirmed in the log). deck44 beat us
in the AIR, not on the stack (details below).

---

## Harness-defect sweep (run first, per skill Step 2)

- **DESYNC (parser mis-grab): 0** across all 6 games (ask/priority head-int == recorded `choice`).
- **ENGINE CAST NO-OP: 0.** Same-pick / mana-unchanged sweep found 0 cast no-ops; Hellrider resolved on
  first pick where cast (110 seq11, T7, mana spent). The wave-5 no-op signature did NOT recur.
- **CAST-REPEAT LOOPS: 0** (no identical Cast pick repeated with mana unchanged).
- **NUMERIC-OFFSET (X menus): N/A** -- deck runs no X-spells.
- **Fallback / empty-reply / timeout: 0** in any deck109 game.

## MANDATORY AUDIT 1 -- wave-6 fallback DELETION: regression check (the task's #1 ask)

The wave-6 stopgap ("IF A CAST DID NOT HAPPEN..." fallback + Hellrider parenthetical) was deleted after
353f1a150 fixed the combo-hold-hint veto. **No regression from the deletion:**
- **Phantom "missed cast" reasoning: 0** across all 6 games (regex over every reply for
  missed/didn't/never-cast framing). The wave-6 misfire ("I must have missed casting Hellrider" used to
  justify a pass) did NOT return.
- **Re-pick / cast loops: 0.**
- **Cast-nothing while a body was listed: 1** (deck44 seq19, below) -- a phantom-*affordability*
  fixation, NOT the deleted phantom-*cast-happened* framing. Distinct leak, single instance, marginal.

Deletion verdict: **clean. Keep it deleted.**

## MANDATORY AUDIT 2 -- the two losses (obeyed-but-losing check before any rewrite)

### LOSS vs deck44 -- "Faerie Archmage" (UB faeries flyers) + mana-light draw
deck44 is a UB faeries tempo deck (Faerie Miscreant, Scion of Oona, Obyra, Archmage of Echoes + Arcane
Denial/Counterspell/Go for the Throat). In THIS game its counters never mattered -- it beat us with an
**all-flying, shroud-protected clock we cannot touch:**
- Board at death: Faerie Miscreant (2/2 fly) x2, Scion of Oona (fly), Obyra (fly), Archmage of Echoes
  (5/5 fly) -- **every threat FLIES** (mono-red ground aggro cannot block them) and **Scion of Oona
  grants the other faeries SHROUD** (verified `auto=lord(other faerie) shroud`) -- **our burn cannot
  target them.** Both of our answers (block, burn) are structurally dead vs this board.
- We were **mana-light**: stuck on **3 Mountains from T7 to T11** (missed land drops T8+T10), so BOTH
  Hellriders and Boros Reckoner sat stranded and our clock was Legion Loyalist (1/1) chipping 1/turn.
  Opp only reached 10 because our ground clock was that slow; their air clock killed us (my 13->4->-9).
- **Marginal misplay (seq19, T11, my4/opp12):** with {R}{R}{R} open and **Boros Reckoner (3/3) AND Ash
  Zealot (2/2 haste) both listed as castable**, the pilot picked **"Cast nothing"**, reasoning it
  "cannot cast Hellrider or Boros Reckoner this turn" (FALSE -- Boros Reckoner was in the list) because
  it fixated on an unassemblable Hellrider+Reckoner "15 lethal." This is the *decline-a-listed-body /
  phantom-affordability* leak the core already forbids. NOT outcome-changing (dying to unblockable
  flyers regardless), and a SINGLE instance corpus-wide.
- **Attribution: MATCHUP (evasive+shroud clock) + VARIANCE (3-land screw).** Not spine-fixable.

### LOSS vs deck110 -- affinity (faster clock + Etched Champion pro-red)
- We fell behind early and lost from behind: my 20->17->12->7->dead (T2-T8); opp 20->...->5.
- At **T7 we were at 7 life vs opp 17** -- losing the race decisively. **Etched Champion has protection
  from red** (metalcraft; verified `auto=aslongas(artifact) protection from red`), blanking BOTH our red
  attackers AND our burn. We cast Hellrider and alpha-struck all four (opp 17->5), tapping out; the
  affinity board swung back for 12 (my 7->-5) T8.
- The all-out attack at 7 vs 17 was the only shot (cannot race from 7-vs-17 by holding back; keeping 1-2
  blockers still dies to the swing-back -- the math doesn't close). Defensible "play as though they lack
  it" line; it lost.
- **Attribution: MATCHUP (affinity out-clocks ground aggro; Etched Champion neutralizes attacks+burn) +
  VARIANCE.** Not spine-fixable.

## What the pilot did WELL (freeze these lines)

- **Aggression total:** 25 attacker decisions, **0 "no attackers."** Full `A#.` commitment every game.
- **Burn is a clock:** all burn targets -> "The opponent (player, life N)"; 0 at own permanents; 0 hoard.
- **Development every turn**, MP2 checklist firing, Unleash counter always taken.
- **Blocker floor held:** only block windows were survival-range and correct; 0 reflexive high-life
  blocks (wave-5 fix still holding).

## Decision: freeze the spine; one surgical sharpen only

In obeyed-but-losing/turnaround mode the skill forbids the reflexive rewrite a bad short record tempts.
The one model-fixable lever (seq19) has a CEILING near zero games -- even a perfect fix does not win the
deck44 game (unblockable flyers). Per the skill's "size the residual lever's ceiling" rule, it earns a
tiny reinforcement folded into an existing obeyed line, **not the #1 slot**:
1. **Sharpen the existing "Cast nothing is for the rare turn" clause** so a phantom-lethal / phantom-bomb
   fixation can never justify declining a LISTED body. (Constrained diff; the rule already exists.)
2. **Add a compact EVASIVE/UNTOUCHABLE-BOARD matchup note** (parallel to the existing Phyrexian
   Obliterator block): vs faerie flyers (fly + shroud) and pro-red artifact creatures, block and burn are
   both dead -- your ONLY route is to race their FACE; deploy a body and send all burn face every turn.
Everything else is FROZEN VERBATIM. Freeze-check in notes.md.

## Deck-construction / meta flags (upstream, unchanged -- NOT guide-fixable)

- **No answer to evasion.** Mono-red ground aggro has zero interaction with flyers and zero targets on a
  shroud/pro-red board. deck44 (faeries) and deck110 (affinity+Etched Champion) are the pool's two decks
  that exploit this. The slip is exactly these two matchups landing their good draw.
- **No card advantage / mana sink** (standing since wave-5): the deck44 loss compounded a 3-land screw
  with no way to convert flood into pressure. Inherent to the archetype. Not recommended to change at 4/6.
