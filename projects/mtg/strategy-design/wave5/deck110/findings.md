# Deck-110 (Etched Affinity) — Wave-5 game review

**Evidence base.** 6 seat translogs from `matchups-20260713-145947/` (one run).
281 logged decisions for this seat. Citations are `<epoch prefix>` + seq (+ turn).
Card facts verified against `bin/Res/sets/primitives/mtg.txt` `auto=`/`text=`.

**Record: 5 wins / 1 timeout-loss — again a top performer (5/6 last wave too).**

| file (epoch) | matchup | result | end turn | end life | notes |
|---|---|---|---|---|---|
| 1783974685 | 131v110 | **WIN** | 7 | my18 / op-1 | clean, fast; 1 equip |
| 1783975169 | 44v110  | **WIN** | 15 | my21 / op-7 | won; some same-turn re-equips |
| 1783975194 | 135v110 | **WIN** | 15 | my18 / op-5 | clean; 2 equips both legit |
| 1783977326 | 110v109 | **WIN** | 8 | my6 / op0  | close aggro-mirror race, WON |
| 1783980403 | 110v133 | **WIN** | 12 | my18 / op-1 | won with 0 equips (went wide) |
| 1783980343 | 140v110 | **TIMEOUT/LOSS** | 33 (cap) | my7 / op15 | double Lightmine Field; behind at cap |

**Harness sweep (required, clean):** desync 0/247 parseable ask+priority decisions
(head-first parser solid — confirms engine context); `fallback` absent in all 281
records. Every misplay below is a GENUINE decision, not a parser/no-op artifact.

---

## HEADLINE: the re-equip count (watch item 16 -> 35) — ~43% WASTED, and concentrated

Classified all 35 equip/attach selections by comparing the target creature's power in
the equip decision's prompt vs its power in the next decision's prompt (power rose = a
real attach landed; power unchanged = plating already on it, a no-op waste):

- **18 LEGIT** (power rose) — including legitimate SECOND-plating attaches (see below).
- **15 WASTE** (power unchanged — re-equipping an already-attached plating).
- **2 unknown** (creature left play before the next read).

So ~43% of the 35 are wasted re-equips. **The 16->35 growth is a corpus artifact, not a
discipline regression.** The waste is dominated by ONE game — the 140v110 timeout ran 115
decisions / 68 priority windows (double Lightmine Field, an unwinnable-by-combat grind) and
holds 21 of the 35 equips (~11 of the 15 wastes). Excluding that game, the 5 short won games
took 14 equips with only ~4 wastes across 5 games — Rule #1 is largely HOLDING in normal
games. Wave-4 baseline was 47 equips / ~27 waste over 12 games (~57% waste); this wave ~43%.
Per-game the count looks up only because one game was enormous.

### The decisive fact for engine task #26 (attachment-state invisibility)

At `1783980343` **seq84** (T21, priority) the model's reply reads verbatim:
> "The Cranial Plating is already attached to Steel Overseer (indicated by the '14/2
> (printed 1/1)' stats)... Equipping it again is a waste of mana. I will attack immediately."

…and then it chose option 2 = **Equip anyway**. The two-power-number DONE-SIGNAL was VISIBLE,
was READ, and was verbalized CORRECTLY — and the model still took the re-offered equip. One
decision earlier (seq83) it did the same no-op silently. **The perception-cue approach has hit
its ceiling: reading the cue does not stop the pick when the equip option is re-offered every
priority window with no attachment marker on the OPTION LINE.** Fresh, strong confirmation
that #26 (attachment markers on the option/board line, or suppression of the already-attached
equip line) is the real fix — instruction text has now failed at full strength across two
waves. Repro: `1783980343` seq83, seq84 (board `Steel Overseer (14/2) (printed 1/1)`
throughout; power unchanged 14->14 both times).

### New nuance: "at most ONE Equip per turn" is now WRONG when 2 Platings are out

Same game, same turn: seq81 equips Steel Overseer 2->8 (attach plating #1, LEGIT), seq82
equips it 8->14 (attach plating #2, LEGIT — the deck cast a second Cranial Plating at seq80).
Two equips in one turn were CORRECT here. The wave-4 brake "Take at most ONE Equip in a whole
turn" would forbid the legitimate second attach. 44v110 shows the same shape (seq42 Etched
Champion 8->14, legit second attach). The brake must key on unattached platings, not a flat
per-turn count.

---

## THE TIMEOUT (140v110) — a hard matchup, not a piloting collapse

deck140 has **two Lightmine Field** in play. Verified: *"Whenever one or more creatures
attack, Lightmine Field deals damage to each of those creatures equal to the number of
attacking creatures"* (`auto=@each blockers:foreach(creature[attacking]) damage:1
all(creature[attacking])`). ONE attacker -> each Lightmine deals 1 -> two fields deal 2 to it.
**Cranial Plating adds +power/+0 only — no toughness** (`auto={1}:equip`). Every creature in
this deck is toughness 1-2 (Memnite 1/1, Ornithopter 0/2, Signal Pest 0/1, Vault Skirge 1/1,
Etched Champion 2/2, Steel Overseer 1/1). **So any attacker into double Lightmine takes 2 and
dies dealing zero, however large its power.**

Literal at T21: seq86 declares the 14/2 Steel Overseer as a lone attacker (exactly what the
wave-4 Lightmine exception orders). Log:
> Damage: 1 dealt by Lightmine Field to Steel Overseer
> Damage: 1 dealt by Lightmine Field to Steel Overseer
> Your Steel Overseer: battlefield -> graveyard

Opponent stayed at 13. A 14-power swing dealt **zero** — the creature died to Lightmine first.
Life curve: deck got opp to 8 by T13, could not finish, deck140 lifegain pulled back to 13-15,
and from T19 the deck LOST the race (my 17->7 while opp 13->15). Behind at the cap (my7/op15) =
loss, not winning-but-slow.

**Attribution:** mostly DECK CONSTRUCTION / hard matchup + variance, with a guide overshoot
on top:
- The opp-at-8 close window (T13-T17, seq51-59) had **no Galvanic Blast drawn** — Blast never
  offered there. Blast is the deck's ONLY route through Lightmine (4 face damage w/ metalcraft)
  and the deck runs just 4. Not drawing it is variance, not a piloting leak — not scored against
  the pilot.
- The re-equip loop did not lose this game but inflated the decision count (115) and thus
  wall-clock, worsening the timeout.

---

## OVERSHOOT AUDIT (mandatory)

**FINDING — the guide's Lightmine exception is a locally-obedient, globally-bad rule.**
Wave-4 guide, lines 53-56:
> "if the opponent controls Lightmine Field (it damages each attacker by the number of
> attackers), attack with ONLY Etched Champion or your single equipped creature."

This ignores TOUGHNESS. The model obeyed it precisely at 140v110 T21 (sent the single equipped
creature, the 14/2 Steel Overseer) and it died to 2 Lightmine damage for zero (toughness 2 <=
2). Because Cranial Plating grants no toughness and the deck tops out at toughness 2, "attack
with your single equipped creature" IS a suicide attack against Lightmine Field — the rule
sends exactly the creature the pilot spent the turn building into a free death. Textbook
overshoot: the rule reads as permission to attack solo, the pilot obeys, the attack does
nothing. Correct play vs Lightmine Field: do NOT attack (unless a creature's toughness exceeds
the Lightmine damage); win via Galvanic Blast to the face. Fixed in the revised guide.

**No other overshoot found.** The wave-3 paralysis ("don't tap out" -> 0/6 timeouts) is absent:
cast-nothing ran ~17/73 casting decisions (~23%), mostly legit (no Blast target, or correctly
declining to attack into Lightmine). "Attack every turn / go wide" did NOT produce suicide
attacks outside the Lightmine case — the model correctly declared "no attackers" at 140v110
T5/T7 when a wide swing would have been Lightmine suicide (seq13, seq30). Go-wide RACE is not
over-running.

---

## WHAT WORKED (freeze — do not reword)

1. **The aggro mirror turned around.** Wave 4 lost BOTH games to deck109 Hellrider; this wave
   110v109 was a **WIN** (my6/op0, T8) — a real race, won at 6 life. n=1, so encouraging not
   proven, but THE RACE section (go wider not taller; chump to stay alive) is doing its job.
2. **Galvanic Blast to face + lethal check** remain reliable; Blasts go to the opponent's face
   and close games (opp finished negative in 4 of 5 wins).
3. **Mana falsity-fact + representation** hold — the "I have no mana" delusion did not recur.
4. **Go-wide without over-equipping** — 110v133 won at T12 with **zero** equips (drew no
   Plating; deployed bodies and raced). The deck does not depend on the equip engine to win.
5. **Fast clean kills** — 131v110 T7, tidy wins vs 135 and 44. Carrier/deploy order sound when
   a threat is live.

---

## Handoff for the guide revision (turnaround-preservation / constrained diff)

5/6 two waves running -> revise as a frozen-line diff, not a rewrite. New text only at the two
new leaks:
1. **Re-equip brake keyed to unattached platings, not a flat per-turn cap** (headline). Keep
   the two-power-number falsity-fact and the positive "mana -> creatures and Blasts" redirect
   verbatim; relax the count to "one per Cranial Plating on your battlefield (usually 1, at
   most 2)."
2. **Rewrite the Lightmine exception to respect toughness and route to Galvanic Blast**
   (overshoot). Net-zero length (replaces the wrong exception).
3. **Do NOT weaken** the mana rule, THE RACE, ATTACK EVERY TURN, the LETHAL CHECK,
   Blast-to-face, carrier order, or the mulligan — all demonstrably working.

The re-equip WASTE residual that survives the guide change routes to engine task #26 (notes.md)
— instruction text has failed it at full strength for two waves.

---

## FREEZE-CHECK (turnaround-preservation mode)

Frozen wave-4 lines and where each survived in the wave-5 strategy.txt:
- Identity block (kill by 5-6 / cast-nothing = lost turn / beatdown by default) — VERBATIM
  (only "equip Cranial Plating ONCE" -> "equip Cranial Plating", to allow 2 platings).
- Mana falsity-fact ("I have no mana for this" is always false when the cast is listed) — VERBATIM.
- Memnite/Ornithopter {0} "cast the moment you see them" — VERBATIM.
- THE ENGINE cast-Plating-turn-1 / equip-with-first-spare-{1} / carrier order — VERBATIM
  (added one clause: "adds POWER only, never toughness" — supports the Lightmine fix).
- THE RACE (go wider not taller; chump at 8) — VERBATIM (obeyed; turned 109 loss into a win).
- ATTACK EVERY TURN / Signal Pest at 0 power / Etched Champion walks past colored blockers — VERBATIM.
- LETHAL CHECK arithmetic + Blast-to-face-when-<=8 — VERBATIM.
- Steel Overseer tap / Master / Thoughtcast / Mox Opal defusal / MULLIGAN — VERBATIM
  (Steel Overseer gains a half-clause on toughness-vs-pingers; Mox Opal gains the Glimmervoid
  not-an-artifact note from wave-4 notes.md deck-builder item).
- Two-power-number DONE-SIGNAL falsity-fact — MOVED into rule #1 item 1, wording preserved.

Changed (aimed at new leaks only):
- Rule #1 count brake: "at most ONE Equip in a whole turn" -> "once per Cranial Plating you
  control (usually 1, at most 2)". Cause: legit 2nd-plating attaches (seq81/82, seq42).
- Lightmine exception: "attack with ONLY your single equipped creature" -> "do NOT attack; win
  by Galvanic Blast to the face; attack only with toughness > Lightmine damage". Cause: the
  T21 suicide (overshoot audit).
- Terminal "DO NOT" list -> positive "DECIDING SITUATIONS (recognize -> play)" recap, per the
  negative-framing ruling (last position, no positive neighbor to suppress).
