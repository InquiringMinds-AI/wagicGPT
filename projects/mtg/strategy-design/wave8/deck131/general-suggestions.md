# deck131 wave-8 - suggestions for other owners (core prompt / harness / deck builder)

## For the HARNESS / representation owner
- **RESOLVED - the wave-7 top ask (land-drop enumeration) is CONFIRMED FIRED for deck131.** 12
  decisions across 6 games now co-offer "Play Mountain" and "Play Island" in one option list (0 last
  wave), and the pilot chose Mountain 12/12 (by name, not position). The three-wave "red screw"
  narrative is largely dissolved for the color that was in hand. No further ask here - thank you.
- **Timeout games (vs140) still write no per-seat `gameend` record in the jsonl** - results.tsv now
  fills life0/life1/turn (adjudicated), which covers the reviewer need, but the seat file's final
  state still has to be read off the last decision. Low priority now that results.tsv is adjudicated.
  Open since wave 4.
- **The `gamestart` header record removed ~7x epoch-arithmetic toil per wave** (game_id + opp_deck in
  the seat file). Confirmed working; keep it.

## For the core-prompt owner
- **NO core edit proposed from this seat.** One candidate rules-fact surfaced but is SINGLE-SEAT:
  `damage >= toughness = dead`. vs140 seq51 the pilot declined an available counter for a Pyroclasm on
  the stack because it believed its 2/2 Guttersnipe would survive 2 damage ("Guttersnipe has 2
  toughness"). This is a general false-world-model belief in the class the core handles (X-spells read
  as 0, "attacking taps my lands", etc.), and if a SECOND seat reproduces a "my N-toughness creature
  survives N damage" misplay it is a clean core rules-fact to add. Carrying as a WATCH only; handled
  per-deck this wave via a falsity-fact tied to Guttersnipe.
- The reflexive-block convergence watch: deck131 contributes NO new instance this wave (the wave-7
  Guttersnipe-block did not recur; blocks were sound chumps). The per-deck NEVER-block-Guttersnipe
  floor stays interim.

## For the deck BUILDER (deck131 rebuild levers - RE-WEIGHTED now the interface is fixed)
- **[DOWNGRADED from #1] Manabase 14 Island / 8 Mountain.** With the land-menu fix, red reached the
  board on curve in 5/6 games; the one late Mountain (vs44 T7) was a MULLIGAN error (a 0-red no-draw
  hand kept), not the manabase. The 14I/8M split still leans slightly wrong for all-red payoffs, so a
  cheap 11M/11I shift is harmless, but this is no longer the dominant loss cause - the interface fix
  recovered the color games it was blamed for three waves running.
- **[NOW #1] Threat density 6/60 (3 Guttersnipe + 3 Young Pyromancer) and NO reach once they are
  gone.** Every loss this wave was deck131 having no second act after its first threat was answered or
  outpaced (opp at 20/18/16/20 at death). Perfect piloting durdles when neither payoff is drawn.
  **Cut the 4 Prism Ring** (pure lifegain filler - cast only as "nothing better" mana sinks, never
  advanced the clock across 4 waves) for reach or more threats. Spellheart Chimera (already #SB,
  scales with instants/sorceries in yard) is on-theme reach worth maindecking.
- **Against lifegain + a sweeper (deck140: gains life AND repeatedly Pyroclasms the 2/2 Guttersnipe)
  the deck cannot win at any pilot quality** - a 2-damage clock cannot out-race a gaining opponent,
  and the only reach dies to a 2-damage board wipe every time it lands. Times out every game (~40+
  turns of wall clock). Fine if deck tiers are intended; otherwise the deck needs a non-incremental
  finisher and/or a resilient (>2 toughness) payoff.
