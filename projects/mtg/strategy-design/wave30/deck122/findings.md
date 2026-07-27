# deck122 (Eternal Life -- WB lifegain / Felidar alt-win / Sorin) -- wave 30 findings

FIRST-GUIDED corpus. Record **6/6 PERFECTA** (up from Step-0 2/6). Binary 6e168aa05,
corpus matchups-20260726-181642, -T 3000, REPPENALTY=1.05. 6 games, **ZERO fallbacks**
across all deck122 seats (0 of the campaign-record 4/1,474). This is the 4th Step-0 ->
first-guided validation arc (after the deck59/18 shape).

## Record decomposition (6/6)

From results.tsv + per-game gameend records:
- 139 v 122 -> **122 WIN (real, won=true)**, 122 at 40, opp -4, turn 13  -- LIFELINK BEATDOWN
- 122 v 152 -> **122 WIN (real, won=true)**, 122 at 31, opp -5, turn 12  -- CLEAN KILL
- 137 v 122 -> 122 WIN (adj1), 122 at 29 vs 13, turn 10  -- ADJ-AHEAD (aggro held off)
- 148 v 122 -> 122 WIN (adj1), 122 at 20 vs 16, turn 8   -- ADJ-AHEAD
- 146 v 122 -> 122 WIN (adj1), 122 at 19 vs 15, turn 19  -- ADJ-AHEAD (long grind)
- 136 v 122 -> 122 WIN (adj1), 122 at 39 vs 14, turn 26  -- ADJ-AHEAD (climbed to 51!)

**2 clean/real closes (opp to negative), 4 adjudication-ahead grinds.** See the taxes audit.

## PERFECTA VALIDATION -- every wave-29 guide teach checked live

### Teach 1: THRESHOLD DISCIPLINE (guide #1) -- VALIDATED. The wave-29 loss class is DEAD.
The Step-0 signature loss (vs93: raced own life 31 -> 25 -> 13, turned 6/6 Serras back into
1/1s, got alpha-struck) does NOT recur. Life trajectories CLIMB or hold in all 6 games; no
self-inflicted drop below a threshold:
- vs137 (GW aggro, the pressure test): t5 @23 life declared **"no attackers"** (correct
  control hold); t7 attacked only Kitchen Finks; life climbed 23 -> 25 -> 29 via lifelink.
  Divinity of Pride rendered `(8/8) (printed 4/4)` at 25+ life -- the model SAW the effective
  size (perception surface confirmed working again).
- vs148 (Kor aggro): dropped 21 -> 15 at t5, but that is OPPONENT damage, not a self-race;
  recovered to 19 by adjudication.
- No game shows the burn-own-life-for-tempo pattern. The reframe held under aggro.
- Minor note (not a leak): vs137 t11 attacked with Serra Ascendant at 29 life (1 short of
  her 30 line, so swinging a 1/1) during a lethal-adjacent alpha. Marginal in an adjudicated
  win; not worth a guide edit.

### Teach 2 + resource-reframe: LIFE-AS-ENGINE / LIFE-AS-WIN-AXIS (guide #1/#2) -- VALIDATED HARD.
Headline result. In Step-0, no PLAN across 6 games mentioned reaching 40 or winning with
Felidar. In wave 30 the Felidar / 40-life line is verbalized in the model's own PLAN in
**every single game** -- felidar-in-reply mention counts: vs139:13, vs152:13, vs137:6,
vs148:11, vs146:18, vs136:21. Representative (vs139 seq17 PLAN @34 life): "continue gaining
life to reach 40 for Felidar Sovereign or drain them out with Blood Artist." The resource
reframe landed at the reasoning level -- the model now treats climbing life as progress
toward winning, exactly the thesis the guide leads with.

### Teach 3: LIFELINK CLOCK (guide #4) -- VALIDATED, and it converged with the life axis.
vs139 (the cleanest win) was won by a 5-creature lifelink beatdown that SIMULTANEOUSLY
climbed the model's life 36 -> 40 (seq18 -> seq20). The guide's claim -- "both plans want the
same thing: gain life and keep it" -- played out literally: the swing that killed the
opponent (opp 4 -> -4) also carried the model to exactly 40. Both win axes advanced at once.

### Teach 4: SORIN CHAINS (guide #6) -- VALIDATED (matches wave-29).
vs146: Sorin cast t15, then +1 (make a 1/1 lifelink Vampire) activated t15/t17/t19 -- the
default every-turn +1 chain the guide codifies. vs148: Sorin in a t0 FindCardToPlay cast
plan. No -2/-6 misfire. Build-loyalty value-walker teach holds.

### Teach 5: BLOOD ARTIST DISCIPLINE (guide #5) -- VALIDATED.
Zero attacks declared with Blood Artist across all 6 games (0/1 kept back as a drain engine
wherever it appeared). The "never attack with it" teach held.

## FELIDAR / 40-LIFE LINE -- pursued, REACHED, but NOT won-by; N-122a STANDS

- **Pursued**: YES, actively, in every game (see Teach 2 mention counts).
- **Reached (40+ life)**: YES. vs139 hit exactly 40; vs136 climbed to **51 life** (t20),
  well past the 40 threshold, and hovered 39-40 at the turn-26 adjudication.
- **Won by it**: NO. Felidar Sovereign never resolved to the battlefield in ANY of the 6
  games. In vs136 (the 51-life game) Felidar was **never drawn** -- confirmed not in hand and
  not on battlefield at any point (checked all 42 records); pure draw variance, NOT a decision
  failure. The high-life state and the Felidar card were never co-present.
- **Conclusion**: the alt-win CLASS remains LIVE-UNVALIDATED. The random pool cannot be
  relied on to co-present Felidar + 40 life -- that is exactly why **N-122a (probe-deck live
  confirmation) STANDS** as the between-wave engine task. The guide teach is working (model
  wants the line); the engine mechanism (AAWinGame at upkeep 40+) still needs its one live
  firing on a stacked deck199 probe. Engine/probe-lane, not a seat hold.

## 6/6-HIDES-TAXES AUDIT

The perfecta is REAL but partly structural. Of 6 wins:
- **2 clean/real closes**: vs139 (lifelink lethal, opp -4) and vs152 (opp -5).
- **4 adjudication-ahead grinds**: vs137 (29v13), vs148 (20v16), vs146 (19v15), vs136 (39v14).

**Structural tax**: a lifegain deck adjudicated on LIFE TOTALS is favored at the cap almost
by construction -- it is the deck whose whole plan is being ahead on life, and life is the
adjudication tiebreaker. The 6/6 therefore over-states dominance: it is 2 kills + 4 "ahead on
my own win-metric when the clock ran out." This matches the brief's pool-wide flag (12 adj at
cap, grindiest pool yet) and should be read as a POOL-COMPOSITION property, not 6 commanding
closes. deck122 is uniquely exposed to this read because its win-metric IS the tiebreaker.

**Reply-length / latency profile** (steady, one outlier):
- vs139/152/148/146/136: median reply ~1095-1330 chars, median latency ~17-21s. Healthy.
- **vs137 (aggro race) is the outlier**: median reply **3850 chars**, median latency **63s**,
  **max 215s**. Heavy deliberation under aggro pressure (long threshold/blocking reasoning).
  Within -T 3000, no timeout, but it is the corpus's latency/verbosity tail at this seat.

## g-batch / verb-labeled loots / parser (null at this seat)

deck122 is WB lifegain with NO convoke, tutor, mutate, loot/discard, dungeon, or DFC cards,
so the g1-g5 step-1 batch does not fire here -- reported as a null observation, not a
validation. Zero parser contradictions, zero fallbacks, no unparsed/deferred/retracted
records across all 6 seats. Blocker/attacker bundles parsed clean (vs139 seq19: 5-attacker
bundle accepted). The core prompt handled a lifegain engine + planeswalker + may-triggers +
conditional-P/T renders without a single seam failure.

## Rotation posture (detail in notes.md)

All five wave-29 guide teaches validated live; guide FROZEN (byte-copied, cmp-verified). The
ONLY unexercised surface is the Felidar alt-win firing, a probe-deck job (N-122a), not a
pool-seat need. deck122 is NEAR-TERMINAL -> **wave-31 exit projected** per the pipeline
pattern (deck59/18 shape: teaches validated + frozen -> rotate; residual is engine-lane).
