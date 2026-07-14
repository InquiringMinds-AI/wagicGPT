# Wave-6 review — deck109 (Hellrider, mono-red aggro / RDW)

**Corpus:** `matchups-20260713-190011`, one round-robin run, 6 games (deck109 seat).
**Record: 5/6** — beat 110, 140, 44, 131, 135; lost only to **133**. (Same 5/6 as waves 4 and 5;
stable top performer. The wave-5 loss was to 110 — this wave 109 BEAT 110 and the single loss moved
to 133, so the record is steady but the losing matchup rotated with variance.)

| seat file (epoch) | result | opp | end (my / opp life, turn) |
|---|---|---|---|
| 1783993782 | WIN | 110 | 5 / -6, T9 |
| 1783993523 | WIN | 140 | 12 / 0, T27 |
| 1783994416 | WIN | 44 | 15 / 0, T11 |
| 1783994677 | WIN | 135 | 20 / -11, T9 |
| 1783994820 | WIN | 131 | 20 / 0, T9 |
| 1783996044 | **LOSS** | 133 | -2 / 6, T15 |

**Headline.** Two things resolved this wave, both in the deck's favor:
1. **The Hellrider "no-op" is FIXED and its mechanism is now understood.** It was never a mysterious
   engine no-op — it was the deck's own **combo-hold `#HINT`** vetoing the model's Hellrider pick until
   `2+ creatures on battlefield` (deck109.txt `#HINT` line). Commit **353f1a150** ("Combo-hold hints no
   longer veto the GPT model's explicit pick") removed that veto. This corpus confirms it: **2/2
   Hellrider casts resolved on the first pick**, including one on a LIGHT board (the exact condition
   that used to no-op). The guide's Hellrider FALLBACK rule is a retired stopgap — and it was actively
   MISFIRING (below), so retiring it is cleanup AND a small fix.
2. **The wave-5 blocker floor WORKED — reflexive high-life blocking is GONE (convergence watch: FIXED).**
   Last wave: 4 blocks, ALL ≥18 life, ≥3 wasteful. This wave: **0 high-life blocks.** The only 2 block
   decisions all corpus were at 7 and 5 life (survival range) and both correct. No displacement cost.

The 133 loss is a genuine close race lost by ~one turn to Geralf's Messenger (undying drain), on a
draw with no reach past the 2 burns drawn — obeyed-but-losing / deck-construction + matchup, not
guide-fixable.

---

## Harness-defect sweep (run first, per skill Step 2)

- **DESYNC (parser mis-grab): 0** across all 6 games (ask/priority head-int == recorded `choice`). The
  head-first parser stays clean. (Two raw "head≠choice" hits were multi-creature ATTACKER declarations
  whose reply opens with reasoning — not single-number desyncs.)
- **ENGINE CAST NO-OP: 0.** Corpus-wide same-pick-mana-unchanged sweep found only benign "Cast nothing"
  repeats. **Hellrider:** 2 casts, both RESOLVED — 140 seq38 (T25) mana 8→4; 131 seq11 (T7) mana 4→gone
  on a modest board. The wave-5 no-op signature (chosen, hand+mana unchanged next record) did NOT recur.
- **NUMERIC-OFFSET (X menus): N/A** — deck runs no X-spells.
- **Fallback / empty-reply: 0**; no timeouts in any deck109 game.

---

## MANDATORY AUDIT 1 — blocker floor: displacement vs overshoot + convergence watch

**Convergence watch: FIXED.** The wave-5 positive blocker-seam floor eliminated the reflexive
high-life blocking that was wave-5's leak.

- Only **2 blocker decisions** all corpus (both in the 110 win); **0 at high life.**
  - 110 seq12 (T6, my7/opp17): "no blockers" vs Memnite (2/1)+Signal Pest (0/1) = 2 incoming. Taking 2
    → 5 is non-lethal; kept bodies to attack; **won the race.** Correct racing math (the ≤8 survival
    clause fires to CUT LETHAL, not to chump every 2 damage).
  - 110 seq17 (T8, my5/opp8): "Goblin blocks Memnite" at 5 life — blocked to survive. Correct; won at 5.
- **Displacement/overshoot cost: none.** No missed chump lost a game; the survival block was NOT
  over-suppressed (it blocked at 5). Freeze the floor verbatim.

## MANDATORY AUDIT 2 — the 133 loss (obeyed-but-losing check before any loud rewrite)

The pilot played the race correctly and lost to a bomb + a gas-out, not a misplay:
- **Aggression full:** 7 attacker decisions, 0 "no attackers." Every single-Goblin swing was FULL
  commitment — the `A#.` list offered exactly one legal attacker (rest summoning-sick/tapped), so NOT
  under-commitment (verified T8/T10/T14 all offered A1 only).
- **Burn as clock:** both burns drawn (Searing Spear T2, Pillar T4) fired FACE. 0 hoarding.
- **Deploy every turn:** cast-nothing = 0 this game.
- **What killed us:** Geralf's Messenger (undying, ETB "opponent loses 2 life"). Game log shows
  repeated "Your life -2" ETB drains as it dies and returns via undying, plus a 4-damage combat hit;
  my life fell 16→6→dead across T13-15 while opp barely moved (8→7). Blocking/trading INTO it is a TRAP
  (death re-triggers undying + drain); racing was correct, and the pilot was never offered a block
  against it (creatures tapped attacking).
- **Discriminator:** opp died-adjacent at **6-7 life**, not near 20 → the plan WAS running; a close race
  lost by ~1 turn, not a durdle. **The hand emptied** (T10 Cackler → T12 Mountain → T14 Gore-House →
  none) with no more burn drawn.
- **Attribution: DECK CONSTRUCTION + variance + matchup.** Not guide-fixable; the wave-5 construction
  note (only reach is 8 burn; no card advantage/mana sink) biting in the one game burn didn't come vs a
  recursive-drain bomb. Do NOT write a louder rule.

---

## What the pilot did WELL (freeze these lines)

- **Aggression total:** 26 attacker decisions, **0 "no attackers."** Full `A#.` commitment.
- **Burn is a clock:** **16/16 damage targets → "The opponent (player, life N)"**, 0 at own permanents.
- **No burn hoarding:** the opp≤8 sweep found ZERO windows where burn was offered and declined for a
  non-face pick.
- **Unleash counter always taken.**
- **Cast-nothing ≈ 4%** (5 of ~117 asks), all early low-mana turns.
- **Development every turn**, MP2 checklist firing.

## Minor watch-item (NOT a new rule — noted, not acted on)

The now-obsolete FALLBACK rule is MISFIRING. 131 seq6 (T5, 1 land, only Pillar of Flame castable): the
pilot chose "Cast nothing" reasoning *"...I must have missed casting Hellrider"* — it invoked the
fallback's "did the cast happen?" framing to justify passing, when the real reason Hellrider wasn't
happening is it costs {2}{R}{R} and it had 1 mana. Two more early passes (110 seq2/seq3) held a lone
Pillar to develop later — defensible. Net: a couple points of face damage deferred across 3 windows,
cost 0 games. The fix is DELETING the fallback rule, which removes the misfiring reasoning at the
source. No new rule needed — turnaround mode, freeze the spine.

## Wave-5 findings status

- **Hellrider no-op — FIXED** (mechanism = combo-hold hint veto; commit 353f1a150). Retire the guide
  fallback rule + its parenthetical in the Hellrider KEY CARD line.
- **Reflexive high-life blocking — FIXED** by the wave-5 blocker floor. Freeze.
- **Output-format stopgap — stayed deleted** (0 desyncs). No action.
- **Phyrexian Obliterator matchup — not exercised** (not drawn in the 133 game). Keep as a standing
  matchup note; still correct.
- **Deck-construction note (no reach/gas past 8 burn) — CONFIRMED biting** in the 133 loss. Standing
  upstream flag; not urgent at 5/6.
