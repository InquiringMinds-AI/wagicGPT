# Wave-41 seat report: deck130 "Budde's Ponza" Step-0 author (harvested 2026-08-24)

[Seat-returned report, orchestrator-filed. 0/6 guideless, 305 decisions, 0 fallbacks.
All card scripts verified against Scryfall Oracle — zero divergences found.]

## Engine
Mana denial -> creature control -> one real body. 13 LD effects + 11 removal/damage +
7 bodies; total face reach 11 (cannot race; burn is removal). Cycling is the flood valve
— and precisely where the pilot destroyed the deck.

## Preserve ledger (frozen — guide does not legislate)
0 fallbacks; 6/6 keeps; land drop always taken; 12/12 LD at opponent lands (10/12
nonbasic); 7/7 Blastminer activations at opponent nonbasics; 17/17 Hammer recursions;
the one Starstorm cast was a correct 4-for-2.

## Failure catalogue (absence table outranks single misplays)
9 attack declarations in 6 games (8 = lone 1/1 Blastminer); 0 blocks in 305 decisions;
Lay Waste cast 0/8 drawn; Starstorm cast 1/8; Rorix attacked 0; goblins attacked 0.
1. Cycling trap: 16/25 cycles discarded Lay Waste or Starstorm ("cycling is free card
   advantage" / "dig for spells" verbatim in replies).
2. Upkeep cycling steals turn mana: 14/25 at own upkeep, 12 with land drop unused.
3. Face-burn with creature/planeswalker on the menu: 8/10.
4. Whole board sacrificed for 6 face damage at 46-life creatureless opponent (twice).
5. Blastminer attacked into [defender] walls, tapping away land kills.
6. Hammer lock as plan substitute (14 casts + 17 recursions t31-66, 8 mana per 3 damage).
7. Cycled 4x into Liliana's Caress (8 of 20 life lost) with Stone Rain rotting in hand.

## Guide: DEPLOYED as deck130_strategy.txt (145 lines / 11.3KB)
Pre-ship greps: frequency-word audit zero hits; every quoted render string verified >=1
hit in corpus prompts (pilot anti-pattern phrases verified against reply/reasoning).

## Falsifiable predictions (next corpus)
1. Starstorm cycles -> 0; casts >=4 of drawn (was 7 cycled/1 cast).
2. Lay Waste casts >=3 (was 0/8); remaining cycles only at >=8 own lands.
3. Upkeep cycles with land drop unused: 12 -> <=2.
4. Face-burn with creature/PW on menu: 8/10 -> <=3/10 (per-card fractions separately).
5. Blastminer in 0 ATTACK lines (loudest exclusion — any appearance falsifies).
6. First LD cast median turn 9 -> <=6; >=half of LD casts at opponent <=4 lands.
7. >=2 games with a real attack line (Siege-Gang/Goblin/Rorix; was 1/9).
8. Goblin sacs at player >6 life: 7 -> <=1.
9. Blocking 0/305 — block ladder UNEXERCISED; next-corpus block seam is new evidence.
10. Counter-risk: NEVER-cycle-Starstorm + Hammer brake could produce passivity vs
    creatureless control (watch vs125 cast-nothing rate + cleanup discards; if up, the
    Starstorm rule needs a creatureless-opponent release).
