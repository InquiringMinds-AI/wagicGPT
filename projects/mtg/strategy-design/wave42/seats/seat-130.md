# Wave-42 seat: deck130 "Budde's Ponza" (2/6, was 0/6) — first guided corpus

542 decisions (58% from one 75-turn deck125 marathon — rates given ex-125 where it
matters), 0 fallbacks, seating verified 6/6.

Predictions: 5 HELD / 5 FALSIFIED (3 narrowly) / 1 null. Highlights:
- P2 Lay Waste HELD (7 casts vs 0/8; LD taken 22/24 offered = biggest guide win).
- P4 face-burn HELD strongly (1/13, was 8/10); Pyrite draw-mode 6/6.
- P7 real attacks HELD (4/6 games; BOTH wins were the Rorix beatdown games).
- P8 goblin-sac discipline HELD (0 face sacs at >6 life, was 7).
- P1 Starstorm cycles FALSIFIED — but all 4 breaking cycles were at "of which 0 are
  creatures": the pilot found P10's predicted release condition empirically. 110/135
  Starstorm offers were at 0 creatures; 83/95 cast-nothings had it as sole option; the
  deck125 loss was DECKING with blanks clogging hand -> EDIT 1 creatureless release.
- P5 Blastminer FALSIFIED loudly 9/9-split: the sends are ALL sole-option windows — the
  "send EVERY except X" sentence points both ways when X is the only creature -> EDIT 3
  first-case restructure.
- P3 narrowly missed (3, all Forgotten Cave "CYCLE IT FREELY" colliding with the upkeep
  pass rule) -> EDIT 4.
- P6 timing missed by hairs (median t7 vs <=6; 10/21 at <=4 lands; ex-125 8/10).
- Costliest misplay class: Hammer cast twice into TWO Silverquill Silencers with
  Starstorm X=2 unpriced in hand (X-spells render NO magnitude annotation) -> EDIT 2 +
  N2 render fix.
- Self-inflicted Blastminer activation destroyed OWN Forgotten Cave — ability-target
  rows carry no owner tag (spell rows do) -> N3 render fix, EDIT 5 interim backstop.

## Edits 1-6 verbatim in task transcript; KEEP verdicts recorded (rule #2 ladder,
## Pyrite gate, Hammer brake 14+17->6+5, mulligans, upkeep-pass 66/91).

## Dev notes -> wave-43 docket
- N1 HIGH: SILENT CAST DROP — chose Cast Starstorm, confirmed Cast Card Normally, then
  NO announce/cast/resolve event, card back in hand 3 records later (deck125 seq293-298).
  Arrival-tracing defect: 1 of 3 Starstorm casts no-op'd silently.
- N2 HIGH: X-cost damage spells render unpriced (no {right now:} magnitude) — annotate
  smallest lethal X + kill list.
- N3 HIGH: ability-target option rows lack owner tags.
- N4 LOW: phantom Clue option sourced to Blastminer with empty card text.
- N5: economics — one seat-game = 314 decisions ~6h inference.
- N6: deck125 matchup ceiling (11 face reach vs lifegain) — construction/matchup item
  for the owner, not a guide defect.
