# deck152 - wave-49 core-prompt / render proposals (layer-routed; engine bugs -> seat file engine list)

Lane T/U render items my deck152 seats confirmed WORKING (keep):
- `{paying this taps: Luminarch Aspirant, Elite Spellbinder, Brutal Cathar - they cannot attack
  this turn}` and `{tapping Katilda, Dawnhart Prime now: it cannot attack this turn}` on the
  Katilda counter-ability row and the creature-tapping cast rows (`152 vs162` s26, `152 vs146`
  s11/s16): rendered and OBEYED (0 lethal-window creature-taps). Retires wave-48 E-3.
- lane U `Both halves of a life LOOP are on THEIR battlefield` + per-A-row loop fold
  (`152 vs126` s31, s40): rendered and OBEYED (ATTACK: none). Retires wave-48 E-5.

New (routed to engine list):
- **R39 / D18 (carried, now a candidate) - counter-spread row annotation.** deck152's
  counter-spreading rule is at its fourth corpus of 2/13 breaks (counters onto an already-6/6+
  body, `vs126` s24/s30). The guide lane is inert at 2/N; the remedy is a ROW ANNOTATION on the
  Luminarch Aspirant / Katilda counter-target rows: `{already 6/6 - a counter here connects only if
  this body does}` or simply echo the target's current size prominence. Same emitter family as the
  `{right now:}` previews. MED (a measured guide-inert lane moving to render).
