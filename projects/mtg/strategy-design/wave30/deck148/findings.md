# deck148 (Kor Army - equipment) findings - wave 30 FIRST-GUIDED corpus

Corpus: matchups-20260726-181642, binary 6e168aa05, -T 3000, REPPENALTY=1.05.
Record: **2/6 (up from 1/6)** - and both wins are now REAL (non-adj) vs wave-29's single
ADJUDICATED win. Wins: vs136 (t17, won at 5 life), vs152 (t16, won 4 to -11). Losses: vs146
(14 to -3, t10), vs122 (adj, 16-20, t8), vs137 (adj, behind 11-21, t12), vs139 (0 to 8, t14).
This is the FIRST guided corpus - the wave-29 guide (Step-0 authored) deployed here and byte-frozen.
All card facts verified against engine text=/auto= in wave-29 (verify-oracle rail); the guide is
byte-identical to the wave-29 authored file (cmp-confirmed).

## GUIDE VALIDATION - all four target teaches FIRED. Turn-11 passivity is DEAD.

Per-teach, cited per the wave-28 headline-4 method (validate each teach INDEPENDENTLY of the win column):

### Teach #1 - ATTACK FLOOR (the #1 wave-29 leak): VALIDATED / DEAD
Wave-29 profile was "first attack turn 11 with bodies down since turn 3" (vs136). This wave, the
floor fires the moment a body exists, every game:
- vs152 (WON): first attack **t4**, then t6/t8/t10/t12/t14/t16 - **swung EVERY one of its turns**.
  Textbook attack floor. -> the win.
- vs122 (adj loss, but close 16-20): first attack **t2**, then t4/t6 - every turn, maximally
  aggressive from turn 2.
- vs146 (loss): first attack **t5**, then t7/t9 - every turn.
- vs136 (WON): first attack **t9** (improved from wave-29's t11 in the same matchup, which was a
  12-6 LOSS - the matchup flipped loss->win with earlier aggression).
- vs137 (adj loss): first attack **t7** with its ONLY creature (Stone Haven Outfitter, geared 5/3),
  then t9. Creature-light early, NOT durdling - it swung the moment it had a body.
- vs139 (loss): first attack **t13** - CREATURE-STARVED (see below), the only late-first-attack in
  the corpus and it is a draw problem, not a passivity problem.
Verdict: the durdle-to-t11 leak is dead. The two late first-attacks trace to having no legal
attacker (creature-light draws), which is construction, not decision.

### Teach #2 - GEAR ARMAMENT MASTER FIRST: VALIDATED where exercised (1 clean exercise, perfect)
AM only reached deck148's battlefield-and-survived in ONE game (vs152). There the teach executed
textbook: vs152 seq7 t4 the FIRST equip of the game = `Equip with Captain's Claws targeting
Armament Master [cost {1}]` - geared the lord itself before any attacker - then a team of Kor Ally
tokens (pumped by AM) swung every turn to the win. This is the guide's #2 rule producing a win
end-to-end.
- AM was NOT on board in vs146/vs122/vs137/vs139 (never drawn/cast, or the deck flooded on
  equipment) - the teach simply had no opportunity, not a miss.
- vs136 is the one nuance: AM was CAST t3 but never re-appears on the own-battlefield render - it
  DIED as a bare 2/2 before it could be geared (AM is a 2/2 with no protection/haste, and the model
  tapped {W}{W} to cast it). Not a teach failure - the guide cannot keep a dead lord alive. Logged
  as a construction observation (AM survivability) in notes.md.

### Teach #3 - EQUIP KOR DUELIST FOR DOUBLE STRIKE: VALIDATED
The model gears Kor Duelist whenever present:
- vs137 seq25/28 t9: Bone Saw + Captain's Claws -> Kor Duelist = board render `Kor Duelist (3/1)
  [double strike]`, then attacked with it. It also re-loaded a fresh Duelist at t13.
- vs136 seq16 t9: Captain's Claws -> Kor Duelist.
- vs152 seq35/36 t14: Captain's Claws + Cranial Plating -> Kor Duelist.
The double-strike-priority teach fires; the model reads the `[double strike]` render on the geared
Duelist and swings it.

### Teach #4 - DEPLOY OVER EQUIP WHEN CREATURE-LIGHT: HELD
vs139 is the deploy-discipline test: at t13, life 4, the hand was FIVE equipment cards (Cranial,
2x Sword of Vengeance, 2x Bladed Pinions) with only 2 creatures on board (Outfitter +
summoning-sick Duelist). The model did NOT waste mana dumping equipment onto nothing - it held the
equipment and swung the bare Outfitter. The loss is the equipment-flood draw itself (construction),
not a decision leak; the discipline the guide teaches held.

## 1/6 -> 2/6 read + CONSTRUCTION-CEILINGED reassessment: CEILING CONFIRMED

Wave-29 projected the attack-floor + gear-first + close-fast teaches would "move 1-2 close games."
Delivered: **+1 win AND a quality upgrade** (both wins now real, non-adj, vs wave-29's lone adj win).
The decisions unlocked what the shell allows - they did NOT unlock MORE than projected:
- vs152 win = direct guide execution (gear-AM-first + attack-every-turn).
- vs136 win = the same matchup that was a 12-6 LOSS in wave-29, flipped by earlier aggression - the
  attack-floor teach's clearest single-matchup signal.
- Residual losses trace to RAW POWER / construction, exactly the ceilinged profile:
  - vs139: equipment-flooded dead draw (5 equipment, 2 bodies), died at 0.
  - vs146: run over 14 to -3 by t10 - no removal, no way to slow the opponent.
  - vs137/vs122: behind / close adj losses vs strong decks, creature-light early, no reach to close.
The wave-29 call stands and is now confirmed: **CONSTRUCTION-CEILINGED, not construction-terminal**
(the deck HAS a clock and gets there; it just runs low-power and floods on equipment). Per wave-29
notes.md's own gate ("if teaches validate and residual losses trace to raw power, THEN route to
roster owner"), that condition is now MET: teaches validated, record moved as projected, residual =
construction. The guide track is essentially done; the next lever is roster-owner construction
(trim the low-impact equipment glut - Bone Saw x4 +1/+0, Bladed Pinions x2 - toward more bodies or
a splash of interaction/reach), NOT another guide revision.

## g-batch steady state at this seat
- g4 tapped-tag reword `[tapped - cannot attack or block this turn]` renders cleanly on opponent
  creatures (vs137 Faerie Guidemother; vs139 Everquill Phoenix) - NO misread. Equipment decks block
  a lot; the blockers decisions this corpus were all sane (Outfitter blocks Knight vs136; Kor ally
  blocks Katilda + Kor Duelist blocks Luminarch vs152) or correctly declined ("no blockers"). No g4
  misread class at this seat.
- g1 convoke, g2 tutor, g3 mutate, g5 dungeon-completed: N/A to mono-white equipment. g3 verb-labeled
  loot/discard asks: this deck has NO loot/discard - nothing to cite. The only remove-from-hand asks
  are mulligan `bottom` decisions (vs136 t0; vs139 t0 x4) and they render as clean card-name lists.

## Fallbacks / parser at this seat: CLEAN (0)
**Zero fallbacks across all six deck148 seat logs this corpus** (0 deferred / unparsed / retracted /
truncated). No prose-reversal, no stale-echo, no numeric-offset. Contrast wave-29, where this seat
was the SECOND live witness of the L-18-1 planning-tax spiral (vs139 seq31 truncated_abandoned).
This wave produced NO such spiral - even in the vs139 loss and the vs122/vs137 adj losses the model
terminated its replies cleanly. Negative data point on L-18-1 frequency at this seat (see notes.md).
