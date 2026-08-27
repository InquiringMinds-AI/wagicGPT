# deck162 - wave-49 core-prompt / render proposals (layer-routed; engine bugs -> seat file engine list)

New / promoted (routed to engine list):
- **R39 (PROMOTED from carried) - the draw-engine cast row carries the punisher count.** deck162's
  Rule 2 (#102/#118) has now had its plan-text remedy FAIL: `vs125` s8 casts Master of the Feast
  with 0 punishers on its battlefield line while the PLAN still says "stack ... once a punisher is
  on the battlefield". #102's exhaustion clause routes this to render. Ask: on `Cast Master of the
  Feast` / `Cast Howling Mine` / `Cast Dictate of Kruphix` / `Cast Teferi's Puzzle Box` /
  `Cast Forced Fruition` rows, append `{feeds: the opponent draws N extra card(s) per turn;
  punishers on your battlefield: K}` - a count the engine already computes for the opposing
  `DRAW PUNISHERS` line. The 0-punisher case (`punishers on your battlefield: 0`) is the brake the
  guide keeps restating. MED. Prediction: engine casts at `punishers: 0` and opp `0 creatures`:
  **0** once the tag ships (was 3).

Lane items my deck162 seats confirmed WORKING (keep):
- lane N `[repeat: N times]` stops the SELF-loop but NOT the compound Intruder-Alarm + Doomsayer
  loop (the excluded deck123-vs-deck162 game, no gameend). Compound-loop stop is HIGH for the
  engine seat (see seat file E-1).
- lane U `DRAW FORECAST` / `DRAW PUNISHERS: yours` rendered on the offensive side wherever deck162
  had a punisher out; 0 render falsehoods.
