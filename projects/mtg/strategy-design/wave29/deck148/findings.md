# deck148 (Kor Army - equipment) findings - wave 29 STEP-0 GUIDELESS DEBUT

Corpus: matchups-20260726-115018, binary 50eeed8be, -T 3000, REPPENALTY=1.05.
Record: **1/6** - the one win is an ADJUDICATED win vs137 (ahead 15-7 at the turn cap, results.tsv
`137 148 adj1`). All card facts below verified against engine `text=`/`auto=` in
bin/Res/sets/primitives/mtg.txt + borderline.txt (wave-28 verify-oracle rail).

## Seat owned: EQUIPMENT SURFACE

### Equipment RENDER - verdict: STRONG, no gaps found
- Equip options render as one option per (equipment, target) pair, carrying the equip cost and full
  card text: vs136 seq20 `Equip with Sword of Vengeance targeting Stone Haven Outfitter [cost: {3}]
  {card text: "Equipped creature gets +2/+0 and has first strike, vigilance, trample, and haste. --
  Equip {3} ..."}`. The attach TARGET is bound INTO the option (no separate blind target sub-menu
  for equip) and the cost is explicit.
- Board render lists each equipment as an `[artifact]` line; when attached it carries
  `[attached to: <creature>]`, and the equipped creature's line shows its MODIFIED [P/T]. Witnessed
  vs139 seq31 (`Cranial Plating #2 {2} [artifact] [attached to: Armament Master]`; `Armament Master
  {w}{w} (6/2)` = 2/2 base + Cranial Plating +4 from 4 artifacts). Unattached equipment shows no
  attached-to tag (vs136 seq20: three Bone Saws + a Sword sitting bare).
- A dedicated `Artifacts in play: you N | opponent N` line exists in CURRENT SITUATION (vs136 seq20
  `Artifacts in play: you 4 | opponent 0`). This is the exact number Cranial Plating keys off
  (+1/+0 per artifact you control) - the deciding fact already rides a surface line.
- No engine gap found: swept all six seat logs for equipment falling off / illegal attach /
  double-equip. Every "fell off"/"fall off" hit (vs93 seq26, vs122, vs139) is the MODEL speculating
  during board reconstruction, not an engine event. Attach legality and attachment state render
  correctly.

### Does the model use equipment sanely? - MOSTLY YES (comprehension is good)
- Equips actively: chosen_text tally across the seat = Bone Saw equips 11, Captain's Claws 8, Sword
  of Vengeance 5, Cranial Plating 4, Bladed Pinions 1.
- Understands Armament Master's lord - 16 replies reason about it (vs137 seq14 "Armament Master ...
  will provide a +2/+2 buff to my other Kor creatures") and EXPLOITS the pump by equipping AM itself
  (vs139 seq26 `Equip with Bone Saw targeting Armament Master`; seq31 quotes the engine text "Other
  Kor creatures you control get +2/+2 for each Equipment attached to Armament Master").
- Mulligan judgment is sharp (vs137 seq3 mull: "4 lands but no creatures ... the artifacts are dead
  cards until a creature enters"; another mull on an all-tapped Kabira hand). It knows equipment is
  dead without bodies.

### The two DECISION leaks worth a guide teach (STRATEGY leaks, not perception)
1. PASSIVITY / no attack floor (the #1 fixable leak). vs136: Armament Master + Stone Haven Pilgrim
   on board by turn 3-5, but the FIRST attack is turn 11 (trajectory: casts through turns 1-9, first
   `attackers` at t11). It durdled while sliding 20->19->18->15->8 and only swung when nearly dead,
   losing 12-6. Standard aggro attack-floor-does-not-transfer leak (Step-0-bis rule 5). This deck is
   BEATDOWN and must swing every turn a body connects profitably.
2. BARE Armament Master is a BLANK - a subtle fact gap. The engine grants the +2/+2 team pump ONLY
   per equipment attached to AM (`auto=this(gear = 1) lord(...) 2/2`; there is NO gear=0 line). A
   vanilla AM (gear=0) pumps nothing and is just a 2/2. The model sometimes treats it as a static
   anthem (vs137 seq14 implies it buffs "future Kitesail Apprentices" without noting AM must itself
   be geared). Teaching "gear Armament Master FIRST - cheap equipment on IT beats gearing an
   attacker" converts a blank into the deck's best turn.

### The engine is DOUBLED vs paper (load-bearing for the guide)
- Armament Master: real oracle is +1/+1 per equipment on it; the ENGINE gives +2/+2 per equipment
  on AM, to every OTHER Kor. Two cheap Bone Saws ({1} each) on AM = +4/+4 to the whole Kor team.
  This is the deck's real payoff engine and it is STRONGER than the card reads. (Oracle-divergence
  record in notes.md; the GUIDE uses engine behavior per Step-0-bis rule 1.)

## The 1/6 decomposition - construction vs decisions (honest split)

~60% CONSTRUCTION, ~40% decision. Losses are CLOSE, not blowouts, so decisions matter, but the
shell is genuinely low-power.
- CONSTRUCTION (notes.md decklist flags, not guide length):
  - Greedy creature:equipment ratio (~20 creatures + 2 token-makers vs ~17 equipment). Equipment-
    heavy draws are dead cardboard - vs136 drew 3 Bone Saws + a Sword with ~1 creature and could not
    function. Much of the equipment is low-impact (Bone Saw x4 = +1/+0; Bladed Pinions x2).
  - NO removal, NO reach, NO card advantage beyond Stone Haven Outfitter death-draw and Emeria
    (needs 7 Plains, rarely online in a race). A fragile assemble-body+equipment+lord engine with no
    way to interact or claw back.
  - Loses close races it cannot quite finish: vs139 died at OPP 2 LIFE (gameend my_life -3, opp 2,
    t12); vs93 lost 6-1 (adj); vs136 lost 12-6. The profile a tighter aggression floor could flip.
- DECISION (guide targets these): the attack-floor leak (#1) and the gear-Armament-Master-first fact
  (#2). Both are STRATEGY leaks (board read correctly, decided suboptimally) so guide prose is right
  (wave-28 headline 1 test). Mulligan and general equip comprehension are already good - do NOT
  re-teach.

## Fallback I own: f6 - N-18e truncation safety, FIRST LIVE FIRE (kind=blockers/truncated_abandoned)

Record: 1785095144-ai_baka_deck148-...-vs-ai_baka_deck139.jsonl seq31, turn 12, phase Blockers. Sole
truncated_abandoned in the corpus, MINE (the brief's f6 / wave-28 N-18e candidate fix's first live
application). Decomposition:
- Situation: my_life 4, one legal blocker (Armament Master 6/2; other creature tapped) vs three
  attackers (Gemrazer 4/4 trample, Dryad 2/4, Pollywog 1/3 = 7 power). PROVABLY LOST - every config
  takes >=5 to a 4-life player.
- The reply OPENED with an illegal line `BLOCKS: B1:A1, B1:A2, B1:A3` (one blocker on three
  attackers), IMMEDIATELY SELF-RETRACTED it ("Wait, I cannot assign B1 to three attackers"), then
  spiraled ~2,500 words recomputing the identical lethal math ~6 times and reconstructing attachment
  state, and hit the token ceiling mid-sentence ("If the blocking creature is") with NO terminating
  `BLOCKS:` line.
- The N-18e safety did exactly its job: it did NOT honor the stale/illegal retracted opening line and
  fell to the SAFE blockers default -> choice 0 / "no blockers". Game is lost regardless, so "no
  blockers" costs nothing.
- Verdict: the safety fired CORRECTLY - it abandoned a self-contradicted, truncated commitment and
  took the safe phase default. VALIDATES the wave-28 N-18e candidate fix (finish_reason=length AND no
  terminator -> prefer safe phase default). The RESIDUAL is MODEL non-termination (L-18-1 planning
  tax): recomputing lethal ~6x in a lost position ate the whole ceiling -> routes to SKILL/CORE
  (L-18-1), NOT guide debt. See notes.md.

## Other corpus fallbacks (NOT mine, for the synthesis map)
- 8 deferred_to_heuristic - all deck137-seat convoke defers, none in my seat.
- 1 retracted_choice - deck136 seat vs93 (1785090615-...deck136-...-vs-...deck93.jsonl).
- 1 unparsed_reply - deck136 seat vs122 (1785093357-...deck136-...-vs-...deck122.jsonl).
Both belong to the deck136 reviewer.

## Parser / contradictions
None in my seat beyond the f6 record (which is the safety working, not a parser defect). No
prose-reversal, no stale-echo, no numeric-offset across the six deck148 seat logs.
