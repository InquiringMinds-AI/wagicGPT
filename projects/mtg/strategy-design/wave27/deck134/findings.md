# deck134 (Urza's Control, mono-U/G tron) - wave-27 findings (GUIDELESS Step-0 debut, 0/6)

Corpus: matchups-20260725-193608, binary ffec79fd8. Seat logs (deck134 side):
- vs18 (Kithkin)  1785026171 - LOSS, turn 10, my_life -4, opp 20
- vs93 (Rats)     1785027413 - LOSS, turn 15, my_life -16, opp 20
- vs137 (convoke) 1785029176 - LOSS, turn 12, my_life 0, opp 16
- vs136 (BW aggro)1785032474 - LOSS (adj), turn 17, my 9, opp 19
- vs131 (UR burn) 1785032563 - LOSS (adj), turn 18, my 9, opp 32
- vs22 (Giants)   1785035486 - LOSS, turn 10, my_life -2, opp 20

Fallbacks: 1 total (vs131 seq39 defer_to_heuristic on Forbidden Alchemy card-pick - harmless).
Parser/protocol clean. NOT a harness problem.

## The deck (verified against primitives/mtg.txt + borderline.txt)
Slow blue-green tron control. Mana engine = the three Urza lands (Mine/Power-Plant/Tower);
controlling all three makes ~7 colorless from 3 lands. Off-color pips (G fogs/Pulse, W
Stonehorn, B Dinrova) come off Unknown Shores (filter, any color) + Prophetic Prism (any
color). Green base is thin (4 Thornwood Falls + filters) but reachable.
- Expedition Map {1} art - {2},T,Sac: search library for a LAND to hand. (assembles tron)
- Stonehorn Dignitary {3}{W} 1/4 - ETB: `nextphasealter(remove,combat*,opponent)` = target
  opponent SKIPS their next combat phase. ENGINE CONFIRMED (5 nextphasealter lines).
- Ghostly Flicker {2}{U} inst - exile+return TWO of your artifacts/creatures/lands (re-fires
  their ETBs). `target=<2>*[artifact;creature;land]|mybattlefield`.
- Mulldrifter {4}{U} 2/2 flyer, ETB draw 2; Evoke {2}{U} (draw 2 then sacrifice - no body).
- Moment's Peace {1}{G} inst - prevent ALL combat damage this turn; Flashback (recastable from
  graveyard). Fog. (engine flashback cost = {2}{G}, real card is {5}{G} - divergence, see notes)
- Pulse of Murasa {2}{G} inst - return creature/land from A graveyard to hand + GAIN 6 LIFE.
- Mnemonic Wall {4}{U} 0/4 defender, ETB: may return an instant/sorcery from your graveyard.
- Compelling Argument {1}{U} sorc - target player mills 5; Cycling {U}.
- Counterspell {U}{U}; Condescend {X}{U} counter-unless-pay-X + scry 2.
- Dinrova Horror {4}{U}{B} 4/2 ETB bounce a permanent + owner discards (never cast in corpus).
- Mystical Teachings {3}{U} inst tutor for instant/flash (never cast in corpus).
- Impulse/Forbidden Alchemy/Prophetic Prism = card selection/draw.

## 0/6 decomposition - CONSTRUCTION vs DECISIONS (roughly half/half)

CONSTRUCTION ceiling (flag upward, not guide-fixable):
- This is a glacial durdle-control deck in an ALL-AGGRO pool (Kithkin, Rats, Giants, UR burn,
  BW aggro, GW convoke). Opponent final life across the 6 games: 20,20,16,19,32,20 - deck134
  dealt ~0-4 damage in FIVE of six games. It has essentially no clock: wincons are Mulldrifter
  2/2-flyer beats (glacial) and Compelling Argument mill (needs the Flicker+Wall loop to be
  fast enough - ~10+ turns raw). Under -T 3000 cap-adjudication it CANNOT win the life race, so
  every grind it doesn't outright close is a scored loss. n=1 per aggro opponent: a large part
  of 0/6 is matchup/variance, not pilotable.
- The 4 outright losses (vs18/93/137/22, dead by turn 10-15 at 0 to -16) were aggro blowouts:
  the deck got run over before its mana engine came online. Even good play likely loses some.

DECISION leaks (the guide's job - these cost the 2 winnable adjudicated games and worsened all):

1. **NEVER cracks Expedition Map / never assembles tron (the #1 leak).** The "search card with
   Expedition Map" activation was OFFERED at ~every priority window and the pilot chose "pass":
   - vs18: Map cast seq4 (T3); crack offered seq5,7,8,9,10,13,14,15,16,19 - PASSED every time,
     NEVER cracked it the whole game. Tron never assembled. Died T10 with 3 uncracked-Map windows.
   - vs93: cracked only at seq33 (T12) - ~6 turns after casting it; and the FIRST search picked
     Unknown Shores (seq34) not a missing Urza piece.
   - vs131: Map recast+cracked only at seq34/35 (T17). vs136 was the exception - cracked T2 (seq5),
     the one game it built mana early. The Map crack is the deck's tempo lifeline and the pilot
     treats a priority window whose only action is the crack as "pass by default."

2. **Missed the Stonehorn Dignitary + Ghostly Flicker soft-lock - the deck's actual anti-aggro
   engine.** vs136 seq25: cast Stonehorn (1/4), then seq28 ATTACKED with it for 1 damage and held
   Ghostly Flicker (seq27,29 offered, chose nothing). Blinking Stonehorn each turn = opponent
   skips combat forever = cannot lose to attacks. Assembled in ZERO games. This alone likely wins
   vs136 (adjudicated at 9 life, opp aggro throttled).

3. **Ghostly Flicker mis-targeted onto LANDS (dead blink).** vs93 seq46/47: flickered Urza's
   Tower + Unknown Shores (two lands - does nothing). vs136 seq16/17: flickered Mnemonic Wall
   (good - regrow a spell) + Urza's Tower (land, wasted). Got it right ONCE: vs131 seq32/33 blinked
   two Prophetic Prisms = draw 2. Flicker must hit ETB-value permanents (Stonehorn, Mulldrifter,
   Mnemonic Wall, Prophetic Prism), never a plain land.

4. **Evoke Mulldrifter under aggro (threw away a needed blocker).** vs137 seq15 at 7 life:
   offered hard-cast {4}{U} (2/2 flyer + draw 2) AND evoke {2}{U} (draw 2, no body) - chose evoke.
   At 7 life vs convoke aggro it needed the flying blocker. vs131 seq12/13 evoked again at 13 life.
   The 2/2 flyer is this deck's only real blocker/clock; hard-cast it when a body matters.

5. **Under-used survival tools; durdled to death / to the cap.** vs18: died at 1 life with
   Moment's Peace (fog) castable in hand (seq18 chose "Cast nothing", never fogged the lethal
   swing). Pulse of Murasa (+6 life) cast late/never at low life. The 2 adjudicated games (vs131,
   vs136) were durdles to the T17/18 cap while behind - no committed win plan and no lock.

6. **Reasoning-tax spikes (latency 90-207 s; medians ~18-24 s).** A control deck with many
   instants and a big stalled board generates giant deliberations (vs93 seq44 206 s, vs131 seq27
   202 s, vs137 seq19 188 s). This is what turns grinds into cap-adjudication losses. The guide
   reduces it by pre-answering the recurring derivations (Map=crack; Flicker targets; role=always
   defense) so the executor stops re-deriving them.

Counterspell/Condescend usage was actually SOUND (countered Relentless Rats, Calamity Bearer,
Ritual of Soot; scaled Condescend X to 7). Keep - the core handles counter timing well; the guide
touches it only lightly.

## What the CORE already handles (do NOT re-teach)
Act-floor / don't-durdle, role check, "listed = payable", counter timing + ON THE STACK, combat
math + lethal check, "activate your permanents' abilities", "sacrifice a fetchland promptly"
(the Map crack is the analog but needs a NAMED line because the pilot still passed it ~10x),
mulligan by land count, X-is-never-worth-0. The guide adds only deck-specific engine facts.
