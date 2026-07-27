# deck122 -> development notes (engine / harness / representation) -- wave 30

FIRST-GUIDED corpus 6e168aa05, matchups-20260726-181642. deck122 seats: ZERO fallbacks, no
parser contradictions, no crashes, no new decision-class defects. Lifegain engine, Sorin
loyalty, may-triggers, conditional-P/T renders all ran clean. No new engine/harness item.

## N-122a (CARRIED, STILL LIVE-UNVALIDATED) -- Felidar Sovereign 40-life alt-win.

STATUS UPGRADE from wave 29: the 40-life STATE is now reached live (vs136 climbed to 51 life,
t20; vs139 hit exactly 40), and the model actively PURSUES the win line (verbalized in all 6
games, 6-21 reply mentions/game). What did NOT happen: Felidar Sovereign resolving to the
battlefield during a 40+ upkeep. In vs136 (51 life) Felidar was never drawn (confirmed not in
hand, not on battlefield across all 42 records) -- pure draw variance, not a decision failure.
So the upkeep wingame trigger STILL has not fired live in the campaign.

The random pool cannot be relied on to co-present Felidar + 40 life (draw dependent), so the
probe-deck live validation remains the right and only reliable confirmation:
- Engine still code-confirmed: mtg.txt:39654 `auto=@each my upkeep:this(controllerlife > 39)
  wingame` (oracle-faithful); `wingame` -> AAWinGame (MTGAbility.cpp:4915); AAWinGame::resolve
  (AllAbilities.cpp:7590) sets the opponent as loser (honors CANTLOSE/CANTWIN); 14 primitive
  cards reuse the keyword; GameStateDuel.cpp:2227/2233 consume the win.
- Probe: temporary Res/ai/baka/deck199.txt stacking Felidar Sovereign + Soul Warden x4 +
  Lone Missionary + Kabira Crossroads x4 + Kitchen Finks + Serra Ascendant (fast redundant
  lifegain), pin one selfplay game (WAGIC_SELFPLAY_DECK0/1), confirm the upkeep trigger fires
  wingame at 40+ and the Felidar controller wins. Delete the probe after.
- This gives the alt-win-condition CLASS its first live confirmation. Between-wave engine
  task, NOT guide work and NOT a reason to hold deck122's pool seat.

## POSITIVE representation finding (RECORD, no work) -- conditional-static P/T renders live.
Reconfirmed at a new datapoint: Divinity of Pride rendered `(8/8) (printed 4/4)` at 25+ life
in vs137. Same annotation family as Serra `(6/6) (printed 1/1)` and Kitchen Finks
`(2/1) (printed 3/2)` under persist. Perception surface for the life-threshold-anthem class is
solid; threshold play is correctly a guide teach, not a representation request. Do not
re-open as a suspected perception leak.

## Latency tail (RECORD) -- vs137 aggro deliberation.
vs137 (the GW aggro pressure test) ran median reply 3850 chars / median latency 63s / max
215s -- ~3x the seat's other games. Under aggro the model spends heavily on threshold and
blocking reasoning. Within -T 3000 (no timeout), but it is this seat's latency/verbosity tail
and consistent with the pool's grindiness. Not a defect; recorded for the model/latency ledger.

## Fallback ownership (per brief) -- deck122 owns NONE.
All 6 deck122 seats had zero fallbacks. The 4/1,474 campaign-record fallbacks (2 defers deck137,
1 priority unparsed deck152, 1 priority unparsed deck146) belong to other seats.

## ROTATION PROJECTION -- deck122 NEAR-TERMINAL, wave-31 EXIT.

Per the pipeline pattern (deck59/18 shape) and the sharpened rotation gate:
- All 5 wave-29 guide teaches VALIDATED live (threshold discipline, life-as-engine/win-axis,
  lifelink clock, Blood Artist discipline, Sorin chains). Guide FROZEN this wave (strategy.txt
  byte-copied from the deployed guide, cmp-verified identical).
- Record moved 2/6 -> 6/6 (perfecta), fully attributable to the guide teaches landing.
- The ONE remaining unexercised surface (Felidar wingame firing) is a probe-deck engine task
  (N-122a), which the random pool structurally cannot exercise -- so it is NOT a seat hold.
- No new decision-class defect, no parser issue, no core-prompt route (25th PASS), zero
  fallbacks. Nothing at this seat needs another guided corpus to observe.
RECOMMENDATION: rotate deck122 OUT at wave 31 (unsentimental, per new-work-exhaustion gate).
Carry N-122a to the engine ledger for a between-wave probe run independent of the pool.
