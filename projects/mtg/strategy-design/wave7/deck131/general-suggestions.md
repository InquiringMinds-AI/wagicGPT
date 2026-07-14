# deck131 wave-7 - suggestions for other owners (core prompt / harness / deck builder)

## For the HARNESS / representation owner (NEW - top priority this wave)
- **The land-drop menu offers lands ONE AT A TIME and never co-offers two lands.** Every land
  decision is "Play <land> / Hold <land>" for a single land the engine picks; there is no "Play
  Mountain vs Play Island" choice anywhere in 6 games (verified: 0 decisions offered both). When
  Islands are surfaced before the Mountain, the pilot literally cannot play the Mountain and the
  color it needs sits dead in hand for turns (vs44: Mountain in hand from T1, first offered T7).
  This makes "play your key color first" unexecutable for ANY color-hungry deck, not just deck131.
  **Ask: present all lands in hand as simultaneous Play options** (Play Mountain / Play Island /
  Hold), or at minimum annotate the current land offer with the other playable lands in hand so the
  pilot knows to hold. This is the single highest-leverage change for deck131 and likely helps every
  two-color deck in the pool. (Holding to advance is NOT a reliable workaround - vs44 held an Island
  in MP1 and was re-offered an Island in MP2, never the Mountain.)
- **Timeout games STILL write no `gameend` record** (vs140 timed out at turn 41+ with no closing
  line; final 11/28 had to be read off the last decision). A timeout-adjudication record (final
  life + turn + who was ahead) would let reviewers score timeouts without reconstruction. Open since
  wave 4.
- **Result pairing is still manual epoch arithmetic** (seat_epoch = results start_epoch + 2..3, then
  confirm by opponent card names). A game-id/opponent/result header in each seat jsonl would remove
  ~7x duplicated toil per wave.

## For the core-prompt owner
- **NO core edit proposed from this seat.** The reflexive-block instance (vs110, blocking with the
  kill-condition creature) is n=1 in an unwinnable matchup and stays a per-deck floor. The mulligan
  false-positive is a per-deck guide-phrasing issue. Do not promote either.
- Watch item to carry: the land-menu representation defect above, if a harness fix ships, will let
  authors DROP the interim "hold-to-reach-your-color" stopgap from every two-color guide at once -
  track it as a stopgap with that removal condition.

## For the deck BUILDER (deck131 rebuild levers - 4th wave confirmed, now with a caveat)
- **[#1 FLAG] Manabase 14 Island / 8 Mountain over all-red payoffs.** Still real: vs109 and vs133
  were genuinely low on red. Cheapest fix: move 3 Islands to Mountains (11M/11I). Blue is forgiving
  - only Counterspell {U}{U}, Dissipate {1}{U}{U}, Opportunity {4}{U}{U} are blue-hungry and all are
  support; the workhorses are single-{U}. **CAVEAT NEW THIS WAVE:** part of the "red screw" is an
  INTERFACE defect (see harness section) - the red source was in hand but unofferable in vs44/vs109.
  A harness fix will recover some of these games without touching the decklist, so weight the manabase
  change against the interface fix landing.
- **Threat density 6/60 (3 Guttersnipe + 3 Young Pyromancer) is the structural ceiling.** Perfect
  piloting durdles when neither is drawn, and a removal-heavy opponent answers them on sight (vs44,
  vs110). The deck has NO reach once its 6 threats are gone. **Cut the 4 Prism Ring** (pure lifegain
  filler, cast only as "nothing better" mana sinks, never advanced the clock across 3 waves) for red
  sources or reach. Spellheart Chimera (already #SB, scales with instants/sorceries in yard) is
  on-theme reach worth maindecking.
- **Against lifegain (deck140) the deck cannot win at any pilot quality** - a 2-damage clock vs a
  gaining opponent (climbed to 28 here, 143 last wave) times out every game and burns ~40 turns of
  wall clock. Fine if deck tiers are intended; otherwise the deck needs a non-incremental finisher.
