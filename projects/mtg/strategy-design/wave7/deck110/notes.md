# Deck-110 wave-7 — development notes (engine / harness / model / deck-construction)

## E1 — RESOLVED & VALIDATED: the re-equip "(ALREADY attached... change NOTHING)" marker.
Wave-6 filed this as SHIPPED-BUT-INERT (gated on `moved->auraParent`, which equipment never sets).
The fix LANDED in `src/AIPlayerGPT.cpp:1163-1173`: equipment is excluded from `auraParent`, so the
host is read as `host = moved->target` (the code carries the TYPE_EQUIPMENT carve-out comment).
Wave-7 validation from deck110's corpus: marker string now renders (4/6 games; 0 in wave-6),
29 marked-offer decisions, 28 obeyed, no-op re-equip rate 1/16 = 6% (was 9/18 = 50%). **CLOSE this
item — fixed and confirmed fired-and-obeyed end-to-end.** The guide's Rule #1 stopgap has been
compressed accordingly (strategy.txt); no further guide/engine work.

## M1 — MODEL/REPLY-PROTOCOL: plan/choice intent-collapse at a represented option.
`1784006250` (vs131) s33 T9. Reply = `3` then a PLAN that correctly reads the marker on option 3
("wasted action that does nothing") and states it wants to attach its SECOND Cranial Plating to
Master of Etherium = option 2 — then emits `3`. head-int == choice, both options legal, the planned
one IS listed. This is the PLAN/CHOICE MISMATCH class (intent collapse), NOT a marker failure and
NOT a desync (the head int matches the choice). Repro record above. Route to the model-experiments /
reply-protocol residuals track as a litmus case: the model names option A in prose and types option
B's index. Not guide-fixable. n=1 this seat.

## E2 — ENGINE/REPRESENTATION (watch): per-instance marker blind to multi-copy churn.
The marker fires only on "already attached to THIS target." With two Cranial Platings out, the pilot
shuffled them between Master of Etherium and Vault Skirge across 6 equip decisions in one turn
(`1784006250` s31-s36); only s33 was marked, because moving Plating-A off a carrier holding
Plating-B is a genuine state-change. Total team power was unchanged, so this is wasted priority
windows, not lost power — and it happened in a blowout win (opp -4 T9). Candidate engine fix:
suppress / demote the equip re-offer once EVERY copy of the equipment is attached to SOME carrier
(not just the specific target). Low priority; a watch item until a second multiples-of-an-equipment
deck reproduces a costly instance.

## DC1 — DECK-CONSTRUCTION FLAG (standing, reaffirmed — owns the loss column).
deck110 is structurally exposed on three axes the guide cannot fix; both wave-7 losses are on these:
1. **Flood / dead-draw.** ~8 of 25 "creatures" are ~0-power (4x Signal Pest 0/1, Ornithopter 0/2)
   atop 20 lands + 3 Mox Opal + 4 Cranial Plating. vs44: Ornithopter + Signal Pest were the only
   bodies for 13 turns; opp never dropped below 20.
2. **Counter-vulnerability (meta shift).** Wins route through a few discrete high-value threats
   (Master of Etherium, Etched Champion, Cranial Plating) = ideal 1-for-1 counter targets now that
   the stack is surfaced and counter options are labeled. deck44 countered Master TWICE this wave.
3. **Only 4 Galvanic Blast** = sole reach through Lightmine / stall / counter walls (vs140 couldn't
   close from opp-3 for want of a drawn Blast).
Not a guide task. If the deck list is ever tuned: trim ~0-power filler and/or the Mox/land count
toward more mid bodies, and add reach redundancy, to reduce flood + single-threat-counter exposure.

## Positive validations (no action, recorded for continuity)
- Harness CLEAN: 0 fallbacks / desyncs / no-ops across 188 decisions.
- Wave-6 RACE `<=8` attack-seam survival clause: block windows now arrive (6 corpus vs 0 in wave-6);
  vs133 blocked at my3/my2 correctly; vs109 (wave-6 loss to this exact leak) flipped to a WIN.
- Core `hand -> stack` cast-happened anchor: 0 no-ops for this seat — supports the cross-deck
  removal A/B on the wave-6 shortlist (evidence only; not a unilateral cut).
