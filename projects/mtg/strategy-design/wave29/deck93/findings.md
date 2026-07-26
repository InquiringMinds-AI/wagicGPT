# deck93 (Relentless Rats) - wave-29 findings (SICK-TAG EXIT GATE, 5/6, up from 4/6)

Reviewer seat: deck93, mono-B Relentless Rats (24 Swamp / 36 Relentless Rats). Binary
50eeed8be. Corpus matchups-20260726-115018, -T 3000, REPPENALTY=1.05. This corpus is the
validation of the f3 summoning-sick tag (`[summoning sick - cannot attack this turn]` now
on battlefield renders). Guide deployed BYTE-IDENTICAL to wave-27/28 (cmp-verified, 5124
bytes, frozen since wave-27). This is the closing data point of the perception-vs-strategy
natural experiment.

Record decomposition (5/6): WIN vs122, LOSS vs146, WIN vs136, WIN vs137, WIN vs139, WIN
vs148. Pool rotated heavily: wave-28's 134/22/18 are OUT; 122/139/148 are new Step-0 debuts.
Persistent-opponent deltas: vs136 W->W (hold), vs137 L->W (flip, the hard matchup),
vs146 W->L (flip, opponent now guided). 109 decisions across 6 games, 0 fallbacks
(structured-field scan) - parser/binding held perfectly again at the 36-Rat stress seat.

================================================================================
THE EXIT GATE (f3) - SICK-TAG VALIDATION  ->  VERDICT: L2 PHANTOM CLASS IS DEAD
================================================================================

### Tag presence: CONFIRMED at this seat, every game
`[summoning sick - cannot attack this turn]` appears in every deck93 seat log (9-15
occurrences/game: 122=14, 146=11, 136=12, 137=14, 139=15, 148=9). Emitted inline on the
own-battlefield render exactly as N-93a proposed, mirroring the e4 land tag. Verbatim
(deck139 seq8 "Your battlefield"):

    ... Relentless Rats #1 {1}{b}{b} (3/3) (printed 2/2); Swamp #4 [tapped];
    Relentless Rats #2 {1}{b}{b} (3/3) (printed 2/2) [summoning sick - cannot attack this turn]

### PHANTOM-ATTACKER RATE TABLE (the headline)

Wave-28 (guide-only, NO annotation): 11/15 attack declarations over-listed (73%).
Wave-29 (annotation added): 1/21 over-listed (5%), and the ONE residual is a DIFFERENT
perception class (a [tapped] misread, NOT summoning-sickness - see below). The
summoning-sick phantom class is effectively 0.

| opp   | seq | turn | legal A-list      | declared        | over  | phantom | cites sick-tag |
|-------|-----|------|-------------------|-----------------|-------|---------|----------------|
| 122   | 9   | 6    | [1]               | [1]             | -     | no      | YES            |
| 122   | 13  | 8    | [1,2]             | [1,2]           | -     | no      | YES            |
| 122   | 16  | 10   | [1,2,3]           | [1,2,3]         | -     | no      | YES            |
| 122   | 21  | 12   | [1,2,3,4]         | [1,2,3,4]       | -     | no      | no             |
| 146   | 9   | 6    | [1]               | [1]             | -     | no      | YES            |
| 146   | 13  | 8    | [1,2]             | [1,2]           | -     | no      | YES            |
| 136   | 9   | 6    | [1]               | [1]             | -     | no      | YES            |
| 136   | 11  | 8    | [1,2]             | [1,2]           | -     | no      | YES            |
| 136   | 14  | 10   | [1,2,3]           | [1,2,3]         | -     | no      | YES            |
| 136   | 18  | 12   | [1,2]             | [1,2]           | -     | no      | YES            |
| 137   | 9   | 7    | [1]               | [1]             | -     | no      | YES            |
| 137   | 13  | 9    | [1,2]             | [1,2]           | -     | no      | YES            |
| 137   | 18  | 11   | [1] (RESIDUAL)    | [1,2]           | [2]   | YES     | no (tapped-misread) |
| 137   | 21  | 13   | [1,2,3,4]         | [1,2,3,4]       | -     | no      | YES            |
| 139   | 8   | 6    | [1]               | [1]             | -     | no      | YES            |
| 139   | 12  | 8    | [1,2]             | [1,2]           | -     | no      | YES            |
| 139   | 17  | 10   | [1,2,3]           | [1,2,3]         | -     | no      | YES            |
| 139   | 19  | 12   | [1,2,3,4,5]       | [1,2,3,4,5]     | -     | no      | YES            |
| 148   | 8   | 6    | [1]               | [1]             | -     | no      | YES            |
| 148   | 11  | 8    | [1,2]             | [1,2]           | -     | no      | YES            |
| 148   | 15  | 10   | [1,2]             | [1,2]           | -     | no      | no             |

Phantom rate: 11/15 (73%) -> 1/21 (5%). Summoning-sick over-list class: 0/21.

### DIRECT TAG CITATION (the clean annotation-lever signal): 18/21 replies
18 of 21 attack replies cite the summoning-sickness restriction directly in their prose.
Verbatim (deck139 seq8 reply) - the model reading the tag off the surface:

    "ATTACK: A1. I have only one creature that can attack this turn: Relentless Rats #1.
    Relentless Rats #2 entered the battlefield this turn (Turn 7), so it has summoning
    sickness and cannot attack."

This is the annotation-lever signal the wave-28 mulligan case could NOT supply (there the
model cited the GUIDE, not the tag - guide-vs-tag confounded). Here the guide teach for L2
was proven to FAIL guide-only in wave-28 (11/15), and adding the annotation - with NO guide
change - both killed the class AND produced 18/21 direct tag citations. The perception
surface, not the prose, carried the fix. Clean isolation.

### The single residual phantom is a DIFFERENT class (tapped-misread, not sick)
deck137 seq18 (turn 11) - board verbatim:

    Relentless Rats #1 (5/5); Relentless Rats #2 (5/5) [tapped - untaps and can attack next
    turn]; Relentless Rats #3 (5/5) [summoning sick - cannot attack this turn]; Relentless
    Rats #4 (5/5) [summoning sick - cannot attack this turn]

A-lines: only `A1. Relentless Rats #1 (5/5)`. Reply: "ATTACK: A1, A2 ... both untapped
Relentless Rats (#1 and #2)". The sick tag WORKED - #3 and #4 (both sick) were correctly
excluded. The error is the model calling #2 "untapped" when it is [tapped]: it latched onto
the substring "can attack next turn" in the tapped-tag wording and misread it as attackable
now. This is a tapped-tag WORDING ambiguity (ledger item, see notes.md), not a
summoning-sick failure. It also caused no harm (opp was at 11, the one legal 5/5 was part of
a winning attack; deck93 won the game). The exit-gate class (summoning-sick phantom) is dead.

================================================================================
MULLIGAN STEADY STATE (land-blindness) + N-93b DE-DUP
================================================================================

- Land-blindness stays DEAD: identical opening (3 Swamp / 3 Rats) dealt again; KEPT 6/6
  (all choice=1 "Keep this hand"). Zero "zero lands" phrase anywhere in the corpus (0/109).
  All 6 keeps cite the guide SNAP-KEEP rule verbatim ("three Swamps and three Relentless
  Rats").
- N-93b de-dup: APPLIED and VISIBLE. The wave-28 double-render (plain "Swamp; Swamp;..."
  then tagged) is GONE. The mulligan decision prompt now renders the hand ONCE, tagged:
  `Your hand: Swamp (land: taps for {B}); ...` + the descriptive "Pre-game mulligan decision
  (London mulligan)" sentence. (The plain "opening hand (6 cards): Swamp; Swamp;..." string
  now only appears in the event-history replay of later prompts, which is expected/harmless.)

================================================================================
INSTANCE HANDLES AT SCALE (round 3) -> CLEAN PASS AGAIN
================================================================================

0 fallbacks across 109 decisions / 6 games. Peak concurrent Rats #6 (deck122 seq21).
Contiguous #N renders held; handles carried correctly into A-lines/B-lines (A1..A5 = board
#1..#5, deck139 seq19). Blocker over-lists: NONE (wave-28 had 1 at vs18 seq19). The
R-DUPLICATE-NAME-INSTANCE audit is re-confirmed a third time: renders/handles/binding are
correct at 36-Rat scale; there is no handle or binding defect left. The wave-28 residual
(model adherence, L2) is now also resolved by the annotation.

================================================================================
4/6 -> 5/6 DECOMPOSITION + "5/6 HIDES TAXES" AUDIT
================================================================================

The record rose 4/6 -> 5/6, but the pool shifted so heavily that the raw number OVERSTATES
deck93's strength. Decomposition:

- vs122 WIN: POOL. New Step-0 debut (Eternal Life, 2/6 overall), guideless/weak. deck93
  curved out, opp to -11 turn 12.
- vs139 WIN: POOL. New Step-0 debut (Mutants, 2/6), guideless/weak. Opp -18 turn 12.
- vs148 WIN (adj): POOL. New Step-0 debut (Kor Army, 1/6 - the pool's weakest), guideless.
  Adjudicated deck93 favor (6 vs 1, turn 11).
- vs136 WIN: HOLD. Established guided opponent (5/6). deck93 held its wave-28 win. Real.
- vs137 WIN: FLIP (L->W), the meaningful signal. The hard fast-aggro matchup deck93 LOST in
  wave-28. deck93 won decisively (opp -24, turn 13). Contributing factors are entangled:
  deck137 got the f1 March/convoke changes this wave (its own deck behavior moved), AND
  deck93's combat perception is now correct (the sick-tag fix removes the wave-28 failure
  mode where the model believed it was swinging lethal on phantom attackers and misplayed to
  death). Cannot fully isolate deck93's improvement from deck137's changes - flag as
  plausibly-fix-assisted, not proven-fix-caused.
- vs146 LOSS: POOL (opponent improved). Was a WIN in wave-28 when deck146 was a weak Step-0
  debut. deck146 now has a guide (f2 dual-land fix) and beat deck93 fast (deck93 -9, turn 9).
  Not a deck93 regression - the opponent got stronger.

TAXES AUDIT: YES, 5/6 partly hides pool-composition tax. Three of the five wins (122, 139,
148) are against 1/6-2/6 guideless Step-0 debuts - the softest opponents in the pool.
Against the established/guided field deck93 is 2/3 this wave (W vs136, W vs137, L vs146).
The genuine, non-pool signal is: vs136 hold + vs137 flip (the fix's likely dividend) against
one loss to a now-guided faster deck. deck93 remains the structurally SLOW deck (turn-3 first
threat, needs several turns to go lethal-wide); it beats weak/guideless boards and loses to
decks that go wider faster - exactly the wave-28 L4 read, unchanged. No guide change extracts
the vs146 speed loss.

================================================================================
f-BATCH / PARSER STEADY STATE
================================================================================

- Parser precedence contradictions at this seat: NONE. 0 fallbacks; every choice bound within
  the legal option count. The one attacker over-list (137 seq18) and all correct declarations
  bound cleanly via first-wins/drop-illegal.
- f3 sick-tag - noise check: CLEAN. Tag consumed without confusion, no mis-parse, no latency
  effect, no prompt-noise. It is READ (18/21 direct citations) rather than ignored.
- e4 land tags / e5 echo / d/c batches: steady (0 fallbacks; echo shape N/A at this seat).
- The corpus-wide fallbacks (11/1,316 per brief) are all at OTHER seats - deck93 contributes 0.
