# deck18 (Kithkin, mono-W tribal anthem aggro) - wave-27 FIRST-GUIDED VALIDATION

Corpus: matchups-20260725-193608, binary ffec79fd8, -T 3000, REPPENALTY=1.05.
deck18 record 5/6 (UP from 4/6 Step-0). Guide under validation = wave-26 initial deploy
(deck18_strategy.txt, byte-identical to wave26/deck18/strategy.txt).

- WINS: vs134 (t10), vs137 (t11), vs131 (t11), vs22 (t10), vs93 (t8).
- LOSS: vs136 (adj, life 1 vs 35, t18) - Yarok mono-B removal+lifelink value; STRUCTURAL
  matchup loss, not a guide leak (decomposed below).
- Fallbacks at this seat: ZERO (consistent with brief - the 7 non-defer fallbacks were at
  136/22/131, none at 18). All 6 mulligans clean keeps.

Second first-guided validation in the deck59 pipeline (4/6 Step-0 -> 5/6 guided). All THREE
targeted leaks VALIDATED; nothing new demands a guide-mod. Verdict NEAR-TERMINAL (rotation
projection in notes.md).

## GUIDE VALIDATION - the three targeted leaks

### LEAK 1 (Goldmeadow Dodger evasion as an ATTACK ORDER): VALIDATED
The guide's "SWING IT EVERY TURN - a big blocker cannot block it" order fired. Every turn
Dodger was on the battlefield and untapped, it attacked:
- vs134: Dodger in EVERY attack (seq6 t2, seq9 t4, seq11 t6, seq13 t8, seq18 t10) - 5/5.
- vs22: Dodger in EVERY attack (seq6 t2, seq10 t4, seq15 t6, seq18 t8, seq22 t10) - 5/5,
  and evasion REASONED (vs22 seq6/10/15/18 replies cite can't-be-blocked / power>=4).
- vs131 seq6 (t3), vs136 seq6 (t2): Dodger swings the turn it lands.
Where Dodger did not attack (vs137, vs93, later vs136) it was simply not on the board
(not drawn / removed) - never a case of Dodger present-but-held.

THE 14K IMPOSSIBLE-BLOCK SPIRAL IS GONE. Wave-26's signature leak (vs133 s17: 14,351
chars agonizing that a 5/5 Obliterator would block+kill the Dodger, evasion never applied)
has NO analog this corpus. The largest Dodger-containing attack is vs22 seq18 = 7,160
chars, and that is a WIDE-ALPHA-STRIKE LETHAL calc (Dodger + Cenn's Heir + Zealous Guardian
at t8), not a Dodger-block spiral. Cross-phase fact-transfer (mulligan->combat) that failed
in wave-26 now holds: the evasion fact reaches the attack seam.

### LEAK 2 (no-flyers/reach RACE off-case): VALIDATED (off-case EXERCISED)
The exact wave-26 losing matchup (vs137, which had the illegal Ballynock-blocks-Faerie
9,904-char spiral at Step-0 s10) recurred with the same flyer. Result this corpus:
- vs137 seq15 (t8): Faerie Guidemother // Gift of the Fae (1/1 FLYING) attacks -> model
  chose "no blockers" (1,121 chars). NO illegal ground-block attempt.
- vs137 seq20 (t10): "Faerie Guidemother // Gift of the Fae #1 (1/1) [flying]" attacks ->
  "no blockers" (875 chars). Clean.
The only two real flying attackers faced all corpus were these two Faeries; both correctly
taken to the face and raced. The Step-0 illegal-flyer-block class did not reproduce, and
the 9.9k tax at that decision collapsed to ~1k. The guide's "you cannot block flyers - do
not try, take it and RACE" line is doing its job.

### LEAK 3 (trust-the-rendered-number tax): VALIDATED - clean A/B tail collapse
Same-deck A/B (wave-26 Step-0 guideless vs wave-27 guided; pools partly overlap: 137/131/22
common):

  metric        wave-26 (guideless)   wave-27 (guided)
  N decisions   138                   127
  p50           987                   943
  p90           5,542                 5,397
  p95           10,923                7,112     (-35%)
  max           14,351                12,915
  mean          2,199                 1,909     (-13%)
  >7,000 chars  12                    9
  >10,000 chars 8                     3         (-63%)
  >12,000 chars 4                     1

The tax reduction lands where the guide targeted it - the TAIL. The re-derivation class
that dominated wave-26's top spikes (vs102 s14 11,972 / vs131 s18 10,923, s22 13,327 /
vs133 s12 13,895, s17 14,351 / vs27 s19 10,555, s23 13,073 - all "recompute Field Marshal
from base 2/2 + each lord") is GONE from the top of the wave-27 distribution. The 3
surviving >10k spikes are ALL attributable to a single lost game's desperation, not stat
re-derivation:
  - vs136 seq24 (12,915): blockers at LIFE 2 vs a lethal 3-attacker board - desperate
    all-block deliberation.
  - vs136 seq28 (10,693): upkeep at LIFE 1 vs lethal - desperate rebuild deliberation.
  - vs137 seq11 (10,046): a double-block SELF-CORRECTION + attacker-tap-status confusion
    (not re-derivation; parser decomposition below).
Caveat: pool rotated (134/93/136 are new opponents), so this is not a hermetic A/B; but a
similar decision count with the >10k spike class halved-plus, and every survivor traceable
to losing-position/combat rather than the targeted re-derivation shape, is a strong pass.

## d1 DUPLICATE-NAME INSTANCE HANDLES (R-DUPLICATE-NAME-INSTANCE) at this seat: WORKING
Duplicate `#N` handles rendered on board/attacker lines and the model bound them correctly,
no confusion, across three kinds of duplicate:
- Creatures: Cenn's Heir #1 / #2 (13 renders each). vs137 seq24 the model declared "Cenn's
  Heir, Field Marshal, Thistledown Liege, Cenn's Heir" (ATTACK: A1,A2,A3,A4) - two distinct
  Cenn's Heirs correctly treated as two first-strike attackers in the lethal calc.
- Tokens: Soldier #1 / #2. vs136 seq24 gang-block "B1:A2, B2:A3, B3:A1" mapped Zealous
  Guardian + Soldier #1 + Soldier #2 to three distinct attackers - no double-assignment.
- Enchantments: Mobilization #1 / #2. vs136 seq28 the model explicitly reasoned "I have two
  Mobilizations ... I can only make one per priority window" - correct duplicate-permanent
  comprehension.
- Adventure flyer: Faerie Guidemother // Gift of the Fae #1 (vs137 seq20) - handle rendered
  on the DFC/adventure attacker, model unaffected.
No binding error, no #N mis-reference anywhere at deck18's seat. deck18 is corroborating
evidence #2 for d1 (deck93 is the systematic 36-Rats auditor) at a creature+token+
enchantment seat.

## N-18c Mobilization discoverability: EXERCISED, now FLUENT
The Step-0 confusion (vs137 s13, 3.5k chars: "the interface doesn't offer an Activate
Mobilization button") did NOT reproduce. The model found and used "Create soldier with
Mobilization [cost {2}{w}]" at priority windows freely (vs134 seq10; vs136 seq9/11/13/16/
21/22/25/26/28/29). The guide's Mobilization interface note pre-empted the confusion. Two
large Mobilization spikes remain (vs136 seq28 10,693 / seq25 8,285) but both are at LIFE
1-2 on a lost board - losing-position deliberation, not interface confusion.

## vs136 LOSS decomposition - STRUCTURAL, not a guide leak
deck18 curved out normally (Dodger t0, Cenn's Heir t2, Mobilization t4). But by t10 (seq13)
its battlefield read "creatures: 0" - the aggro board was fully removed by deck136's mono-B
suite (Knight of the Ebon Legion, Cavalier of Night, Dread Presence + removal). Opponent
life climbed 20->24->25->29->32->35 on Cavalier of Night LIFELINK while deck18 fell
20->...->2->1. The apparent "no attackers t4-t10" gap is not a durdle: deck18 had no
creatures to attack with. The model kept rebuilding via Mobilization tokens and attacked
whenever it had a body (seq19 t12 Soldier+Zealous Guardian, seq23 t14 Zealous Guardian) -
correct beatdown behavior on a losing board. This is the weenie-aggro-vs-mono-B-removal+
lifelink structural loss; no guide line fixes "opponent has more removal + lifegain than
you have gas." NOT a teachable leak.

## c/d steady state + parser precedence: CLEAN
- c1 (full target enumeration): no prompt-length pathology; Armored Ascension never
  resolved this corpus (N/A), but the multi-attacker block enumerations were complete.
- c4/d (parser hardening precedence): one candidate. vs137 seq11 (blockers, 10,046 chars):
  the reply's FIRST line was the illegal `BLOCKS: B1:A1, B1:A2` (Field Marshal assigned to
  two attackers), the model then self-corrected mid-reply and its CONCLUDING coded line was
  the legal `BLOCKS: B1:A1`. Last-BLOCKS-line precedence took the legal final line ->
  Field Marshal blocks Flaxen Intruder (2/2 vs 1/2, survives). CORRECT-BY-PRECEDENCE, no
  fallback. c/d steady-state behavior working as designed. 0 unparsed_reply / fallback
  markers at deck18's seat all corpus.
- Residual (below-bar): whole-turn planning tax on trivial single-option decisions - vs137
  seq12 "Play Plains" 7,112 chars and vs136 seq17 "Play Plains" 5,844 chars are the model
  planning the entire turn's sequencing on a land-drop prompt. Not stat re-derivation, not
  guide-fixable; latency-budget WATCH only (new ledger L-18-1).
