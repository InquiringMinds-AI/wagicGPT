# deck22 (Giants!, mono-R tribal ramp/midrange) — wave-26 STEP-0 findings

Corpus matchups-20260725-115216, binary 77e91ef4e. Record 2/6: adj-win vs102, win vs137;
losses vs133 (-2/4), vs131 (0/20), vs27 (-4/2), vs18 (-1/20 T8). ZERO fallbacks / 0 parser
misfires across 74 decisions — c4 hardening clean at this seat (spot-checked mulligan/
bottom/attackers replies: every `choice` matched the reply's final coded line; no
natural-stop reversal contradictions).

(Placed by the orchestrator from the reviewer's returned report — the seat agent's .md
writes were blocked by a harness path guard; content verbatim from its final report.)

## Top leaks the initial guide addresses

1. **Rule #1 — the model does not know Universal Automaton is a Giant.** Changeling text
   IS rendered on the option line (vs102 s14: `[changeling] ... "every creature type"`) so
   this is a BELIEF/inference gap, not a representation gap. It cost two games at
   mulligan/bottom: vs131 s1 mulliganed a textbook keep ("Universal Automaton is a 1/1
   with no immediate impact ... Blind-Spot Giant requires another Giant which we don't
   have"), then s4 bottomed Automaton+Blind-Spot — its own enablers — flooded, lost 0/20.
   vs18 s1 same misread, mulliganed the aggressive keep, run over -1/20 T8. vs137 s4
   bottomed two Automatons. Cross-phase fact: taught for mulligan+bottom, not just cast.
2. **Passivity / under-attack** (cost both close losses): vs133 the 4/3 Blind-Spot Giant
   (enabled from T5) didn't attack until T11 while Mogg Sentry (1/1) chipped
   s8/s10/s12/s16 — ~12 damage left home, lost by 2. vs27 Behemoth blocked (s21) in a race
   lost by 4. Contrast the vs137 WIN = the one full alpha strike (s19, Calamity-doubled).
   Guide ships a per-item attack floor over the Giant list.
3. **Mogg Sentry attacked as a 1/1** (vs133/vs27/vs102), violating its own
   `#HINT:dontattackwith` — it's a blocker/ambusher.

Verified via Scryfall (load-bearing): Universal Automaton {1} 1/1 Changeling; Calamity
Bearer {2}{R}{R} 3/4 Giant, doubles Giant-source damage; Sunrise Sovereign +2/+2 trample —
all match the engine primitives.

## c1-c5 observations

- c1 target enumeration EXERCISED clean (vs102 s18-21 Inferno Titan 3-way split, face
  offered+picked, no truncation).
- c3 PT-pump renders clean (vs102 s30/s31 `+1/+0 (6/6 -> 7/6)`, no stat-conflation).
- c2/c5 N/A at mono-R seat. c4 no misfire.

## What the core already does right (not re-taught)

Cost-reduction (Stinkdrinker discounts read+cast correctly), Inferno split targeting,
firebreathing, and mono-R mana (zero color-anxiety — no mana paragraph written).
