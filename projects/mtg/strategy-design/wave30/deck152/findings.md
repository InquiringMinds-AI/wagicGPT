# deck152 (Bant Midrange) -- Step-0 GUIDELESS DEBUT -- wave-30 findings

Corpus: matchups-20260726-181642, binary 6e168aa05, -T 3000, REPPENALTY=1.05. Record 2/6.

Record read (results.tsv, deck152 perspective):
- WIN vs139 (winner=1, 152 at 29, 21 turns) -- clean beatdown win.
- WIN vs146 (adj1, 20 vs 15, 14 turns) -- adjudication win, ahead on board/life.
- LOSS vs122 (winner=0, 122 at 31 / 152 -5, 12 turns) -- clean loss.
- LOSS vs136 (adj0, 136 28 / 152 5, 15 turns) -- ground out; board stripped by BW removal.
- LOSS vs137 (adj0, 137 9 / 152 4, 16 turns) -- close adjudication loss.
- LOSS vs148 (winner=0, 148 4 / 152 -11, 16 turns) -- lost a race.
2/6 ~ pool median for a Step-0 seat here. Both wins came from the core plan (develop Humans +
escalate attacks); losses split between raw grind (136) and close races (137/148). Reads as a
functional-but-unpiloted deck, not a construction failure.

## Verify-oracle card table (read off Res/sets/primitives, NOT memory)

Engine matches Scryfall for these cards; no divergences to route. Numbers as printed.
- Teferi, Who Slows the Sunset -- {2}{W}{U} Legendary PW, loyalty 4. +1 untap yours/tap theirs +
  gain 2 life; -2 look top 3 take one; -7 emblem. THE ONLY BLUE CARD.
- Luminarch Aspirant -- {1}{W} 1/1 Human Cleric. Begin combat your turn: +1/+1 counter on a
  creature you control.
- Intrepid Adversary -- {1}{W} 3/1 Human Scout, lifelink. ETB pay {1}{W} any # times -> valor
  counters; your creatures +1/+1 per valor counter.
- Katilda, Dawnhart Prime -- {G}{W} 1/1 Legendary Human Warlock. Pro-Werewolves. Your Humans have
  "{T}: add one mana of any of this creature's colors". {4}{G}{W},{T}: +1/+1 on each creature you
  control.
- Elite Spellbinder -- {2}{W} 3/1 flying Human Cleric. ETB look opp hand, exile a nonland (+2 cost).
- Augur of Autumn -- {1}{G}{G} 2/3 Human Druid. See top of library; play lands from top; Coven ->
  cast creatures from top.
- Briarbridge Tracker -- {2}{G} 2/3 vigilance Human Scout. ETB investigate (Clue). While you
  control a token, +2/+0 (renders 4/3 with a Clue out).
- Brutal Cathar -- {2}{W} 2/2 Human Soldier Werewolf, daybound. ETB/transform-in: exile a target
  opponent creature until Cathar leaves. Back = Moonrage Brute (night) 3/3 first strike, ward-pay 3.
- Sigarda, Champion of Light -- {1}{G}{W}{W} 4/4 Angel, flying+trample. Humans you control +1/+1.
  Coven attack trigger: look top 5, may take a Human.
- Tovolar's Huntmaster -- {4}{G}{G} 6/6 Human Werewolf, daybound. ETB two 2/2 Wolves. Back =
  Tovolar's Packleader (night) 7/7, ETB/attack two 2/2 Wolves, {2}{G}{G}: fight.
- Fateful Absence -- {1}{W} Instant. Destroy target creature or planeswalker; controller gets a Clue.
- Ranger Class -- {1}{G} Enchantment - Class. L1 ETB 2/2 Wolf. L2 {1}{G} (sorcery): on attack
  +1/+1 counter on an attacker. L3 {3}{G} (sorcery): cast creatures off top of library.
- Lands: Forest {G}, Plains {W}; Branchloft {G}//Boulderloft {W}; Hengegate {W}//Mistgate {U};
  Barkchannel {G}//Tidechannel {U}; Deserted Beach {W}/{U} (tapped unless 2+ others); Overgrown
  Farmland {G}/{W} (tapped unless 2+ others); Lair of the Hydra {G} manland.
- daybound = becomes NIGHT if a whole turn passes with no spell cast; nightbound = becomes DAY if
  a player casts 2+ spells in their turn.

## SURFACE 1 -- WEREWOLF DAY/NIGHT TRANSFORM (first live outing since flip-thrash fix)

VERDICT: day-side steady state CLEAN; the LIVE day->night FLIP was NOT EXERCISED this corpus.
- NO werewolf transformed to its night side in ANY of the 6 games. Grep of the events field for
  "It becomes night/day", "-> Moonrage Brute", "-> Tovolar's Packleader" across all deck152 files
  is EMPTY. daybound needs a whole turn with zero spells cast, which never happened in these
  spell-dense games. The two "Moonrage Brute" hits (vs146) are the MODEL naming it in PLAN prose.
  So there is NO flip-thrash / isFlipped-oscillation evidence either way -- the fix's steady state
  under live flipping remains UNVALIDATED at this seat.
- Day-side render COHERENT (vs137 seq17): "Brutal Cathar {2}{w} (2/2) [daybound] [summoning sick
  - cannot attack this turn]; Day". Correct front-face P/T + mana + [daybound] tag; the day/night
  STATE renders as a "Day" designation pseudo-permanent on the battlefield line, and the creature
  count ("creatures: 2") correctly EXCLUDES it. No face-desync on the day side.
- Brutal Cathar's exile ETB fired and REVERSED end-to-end (vs137): seq17 Cathar enters ->
  "Edgewall Innkeeper: battlefield -> exile"; seq27 Cathar dies in combat -> "Edgewall Innkeeper:
  exile -> battlefield". The "until this creature leaves" clause works. DFC cast routes through a
  "Choose an option -> Cast Card Normally" menu (vs137 seq16), fine.
- Tovolar's Huntmaster ETB made its two 2/2 Wolves normally (vs122, vs139).

## SURFACE 2 -- CLASS LEVEL-UP (Ranger Class)

VERDICT: render CLEAN, decisions SANE.
- Level-up option renders "level Counter with Ranger Class [cost: {1}{g}]" (L2) / "[cost: {3}{g}]"
  (L3) -- costs match the engine (vs136 seq45/52/58, vs137 seq25+, vs146 seq34/43). Verify-oracle
  passes.
- Current level = counter count on battlefield: "Ranger Class {1}{g} [enchantment] [counters: 1x
  level]" -> "[counters: 2x level]" (vs137). Not counted as a creature (correct).
- Model levels at sane times (reached L2 in multiple games; cast Ranger Class early for the Wolf).
  One benign model musing at vs139: called Ranger Class "effectively a 2/2 ... though the prompt
  lists it as a creature" -- render is correct (enchantment, excluded from creature count); a
  minor model misread, not an engine issue.
- Intrepid Adversary "pay {1}{W} any number" ETB renders as a choice menu; handled correctly
  (vs139 seq43 chose "don't add any counter" with only {G} available). Clue renders as an
  activatable "Draw 1 with Clue [cost: {2}, Sacrifice]".

## SURFACE 3 -- GWU 3-COLOR MANA (dual-land oracle fix under 3-color stress)

VERDICT: the "Mana available" line is ACCURATE; the one fallback was the MODEL overriding it, not
an engine mislead. NO castable-but-not-offered (N-146a) offer-gap found.
- Line renders color SET + source count: "{g}{w} from 5 untapped sources", "{g}{u}{w} from 4
  untapped sources". Duals surfaced correctly in hand ("Overgrown Farmland (land: taps for
  {W}{G})", "Deserted Beach (land: taps for {W}{U})").
- Castable-but-not-offered probe (hands-vs-asks in losses): Fateful Absence IS offered whenever
  payable+targetable (vs136 seq19/25/26/28; vs137 x4; vs146 x3). The suspicious window (vs136
  seq27) is NOT an offer-gap -- see fallback below.
- Blue is the weak point: {U} only from Deserted Beach + the BACK faces of Pathways. Teferi
  ({2}{W}{U}) resolved 0/6 -- see N-152a (play-LAND menu never surfaces a Pathway's blue back
  face; blue near-inaccessible). Representation gap -> notes.

## FALLBACK DECOMPOSITION (1 priority unparsed -- vs136 seq27)

- Record: kind=priority, Main phase 1, YOUR turn, my_life 17, options=1, choice=-1,
  fallback="unparsed_reply". The ONE option: "1. Flip Side with Barkchannel Pathway -> DISPLAY
  TOGGLE only ... It does NOT cast anything ...".
- Mana line (correct): "Mana available: {w} from 2 untapped sources". Untapped = Plains + Hengegate
  (both {W}); Forest + Overgrown Farmland TAPPED (spent on Ranger Class that turn); Barkchannel was
  in HAND. So NO green.
- Reply: "CHOICE: 2 (Cast Briarbridge Tracker)". No option 2 exists. The model fabricated mana
  ("Untapped: Plains, Hengegate, Forest, Barkchannel ... tap Forest for {G}") -- inventing an
  untapped Forest and treating a HAND card as a source -- decided Briarbridge ({2}{G}) was castable,
  answered a nonexistent number -> unparsed -> correct parser rejection -> heuristic.
- Root cause: PERCEPTION leak -- model re-derived payability and OVERRODE the accurate "Mana
  available: {w}" line -- triggered by the bare-priority Flip-Side-only window (only a useless
  toggle offered, so it reached for a phantom play). Briarbridge was CORRECTLY not offered (no
  green). Exactly the TRUST-THE-RENDERED-NUMBER class; guide Rule #1 targets it. Seam -> N-152b.

## OTHER READS

- REASONING TAX: Coven drove heavy deliberation (15-24 "coven" mentions/game in vs122/146/137/148);
  latency spiked 202s (vs137), 186s (vs136), 181s (vs146); reply max 12-13k (vs136/137), near the
  ~14k truncation cliff. Guide pre-answers Coven to cut this tax + adjudication-costing latency.
- PASSIVITY vs board-stripping: vs136 had ZERO attacker-phase asks in a 15-turn loss -- but the
  board never survived BW removal (Legion's End exiled Luminarch turn 7); the Attackers phase
  produced no ask because there were no eligible attackers. Do NOT over-attribute to passivity;
  partly matchup/grind. Attack floor (Rule #2) still earns its place -- wins were escalating swings
  (vs139 seq46 Wolf -> 54 +Intrepid Adversary -> 60 +Elite Spellbinder -> 69 full team incl Katilda).
- g-batch: no g1-g5 surfaces are deck152's; no loot/discard asks at this seat. Tapped-tag reword
  "[tapped - cannot attack or block this turn]" renders and reads cleanly (vs136 seq27 opp lines).
  No parser contradictions observed.
