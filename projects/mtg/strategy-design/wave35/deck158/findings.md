# deck158 (Orcs of Mordor — RB amass) — Wave-35 guide review and revision

Rubric (owner, binding): win rate irrelevant; score adherence per decision and the quality of
divergences. Arm weighting per the brief: **arm C (Qwen3.6-35B-A3B-FP8, the standing opponent) is
the pilot this guide teaches**; arm B (122B) marks under-specification; arm A (retired 4.75-bit)
is background only. Pre-digested evidence: `wave35-review/deck158-tier-review.md` (arms A+B) and
`wave35-review/deck158-fp8-review.md` (arm C) — cited below, not re-derived.

## Cross-corpus adherence picture (from the two prior reviews)

| arm | scored | divergent | failures | notes |
|---|---|---|---|---|
| A (4.75-bit, retired) | 126 | 9 | 3 (2 guide-induced G1, 1 own) | rail-following excellent; echoes guide bugs verbatim |
| B (122B) | 125 | 3 | 0 | every divergence a justified audible |
| C (FP8, standing) | 119 | 10 | 6 decisions / 4 events, all model-owned | zero G1 obedience; new single-number-misread class |

The guide's rails are obeyed at every tier: mulligan by the engine {B} count (A-105 seq1 verbatim
quote; C-36 mulled a 0-{B} hand and bottomed correctly), amass-in-MP1 (C-116/C-105 textbook),
March both copies (B-116 seq8), Mauhur-first (B-105 seq6), Bowmasters toughness-1 branch incl.
face-branch (A-105 seq24-27; C-139 seq7/24; C-146 seq7/11), Foray printed-number trust, Assault
max-X (A-116 seq15-16; A-152 seq19-20), Feed price-reading (B-152 seq12-13; C-105 seq12), Witch-king
attack-always (B-146 seq23). These lines are FROZEN — wording unchanged in the revision.

## Mandatory item — G1, the false "their deaths feed the Army" line: RESOLVED

Old lines 117-118 claimed Vanguard, Bowmasters AND Crebain chump-first "— their deaths feed the
Army". Engine-verified FALSE for two of the three (`borderline.txt`: Vanguard `auto=_DIES_
_AMASSORC1_`; Bowmasters = ETB + opp-draw triggers, no death trigger; Crebain = ETB only).
- Arm A OBEYED it into two chump-block misplays: A-36 seq15 T7 (chumped Cathodion with Bowmasters
  at 15 life, "blocking is strictly better" — Army 7/7 unchanged) and A-152 seq10 T6 (chumped the
  Wolf at 17 life quoting line 117 verbatim — Army 2/2 unchanged).
- Arm C REFUTED it from card text: C-152 seq19 T12 quoted the line, then "Crebain doesn't have a
  death trigger. Only Easterling Vanguard does. So blocking is pointless" → BLOCKS: none, correct
  under step 3. The one place C leaned on death-amass was Vanguard (C-36 seq16/seq18) — the card
  for which it is true.
The revision states the TRUE mechanism so no pilot has to override: Vanguard is the ONLY
death-amass creature (first chump; its death pays a counter); everything else amassed on ENTRY
and dying returns nothing; chump Crebain before Bowmasters because a live Bowmasters keeps
triggering on opponent draws (the value arm A threw away at A-36 seq15). Class swept across the
whole guide: the DECIDING SITUATIONS line ("Vanguard and Bowmasters first") corrected to the
same order in the same terms.

## Best-blocker rule under pressure (brief per-seat item)

A-139 seq15+seq17 T9: at 4 life, declined Vanguard twice — "can cast, but useless without a sac
outlet" — a self-invented disqualifier contradicting the guide, then assumed lethal through four
untapped blockers and died on the crack-back. Did NOT recur at arm C: C-36 seq15 T8 cast Vanguard
on sight ("best blocker") and seq18 ran the block test with all three lines enumerated. Treated as
defense-in-depth, not a live leak: one clause + one falsity-fact added inside the Vanguard entry
("dying in combat IS the payoff; the thought 'Vanguard is useless without a sac outlet' is
FALSE"), quote-and-forbid form per the skill's sanctioned negative construct, following its
positive rule in the same block.

## Threshold-arithmetic support (arm C's single-number misreads)

All four arm-C failure events are one shape — a single number misread inside a rule-quoting trace:
- F1 (C-152 seq10+seq14, T7/T9): Elite Spellbinder (3/1) read as "toughness 3" — P/T transposed,
  twice; rule 4 quoted then misapplied → face shot with a legal toughness-1 kill on the list.
- F2 (C-105 seq11 T6): rule-4 override on a false premise — shot Contagious Nim 2/2 [infect]
  citing the FEED paragraph's infect clause, claiming 1 damage "removes" a 2/2.
- F3 (C-36 seq12 T6): Foray 6 damage → Master of Etherium 8/8, "all of them will die"; Master
  survived and ate the 6/6 Army.
- F4 (C-36 seq16-17 T8): declined the listed Olog-hai Crusher after a hand-recount miscounted
  Treasure mana, overriding TAKE-THE-CAST and the prompt's payability guarantee.
- R1 (C-36 seq18, outcome-neutral): "4 - 4 = 0 life. I survive!" — 0 treated as survival.
Guide-side support shipped (each names its number by the render substring it follows, per the
wave-33 HL2 rung — no bare variable letters):
1. New early section READING A CREATURE'S NUMBERS: toughness is the SECOND number of (P/T), with
   the transposition pair worked both ways ((3/1) dies to 1; (1/3) does not) and the
   damage>=toughness kill test ("6 damage does not kill an 8/8"). Bowmasters' bullet and the
   DECIDING SITUATIONS recap re-anchor to "the SECOND (P/T) number". [answers F1]
2. Bowmasters: "[infect] is not a reason either... an infect creature with toughness 2 or more is
   a job for Feed the Swarm, never for the shot" — closes the cross-paragraph licence F2 used.
3. Foray target choice (previously guide-silent, flagged in both prior reviews): kill test stated
   in the entry + the kills-nothing branch (damage stays marked; combat adds to it). [answers F3]
4. TAKE THE CAST: falsity-fact "the thought 'I cannot actually pay this' is ALWAYS FALSE when the
   line is in your list — Treasures and every source are already counted". [answers F4]
5. Blocking step 2: "OR MORE — EQUAL counts: at exactly 0 life you LOSE". [answers R1]
Items 1 and 5 are STOPGAPS against a render gap (see notes.md P6): removal conditions recorded
there — demote when the Bowmasters/Foray target lists print an explicit toughness verdict and the
blockers header prints the at-0-you-lose arithmetic.

## Other edits

- Assault on Osgiliath: added "gives your Orcs double strike and haste for the turn — cast it in
  MAIN PHASE 1, before combat" (primitive: `emblem transforms(... double strike / haste) ueot`).
  One clause; the card is on its third consecutive unexercised corpus (wave-33 HL12 retired it
  from the corpus ledger), so no further budget spent.
- Everything else byte-identical. This is a constrained diff: no frozen line's wording changed;
  the two new negative constructs are the two sanctioned forms (falsity-facts following their
  positive rules).

## Retroactive card-fact audit (HL1 hard gate) — ledger, shipped guide

| claim as written | adjudicating primitive line | verdict |
|---|---|---|
| March: amass 1 on entry + every Army attack; {1}{B} | `_AMASSORC1_` + `@combat(attacking) source(army...)`; mana={1}{B} | TRUE |
| Mauhur {B}{R} 2/2 menace; +1 extra per counter event on Army/Orcs | `@totalcounteradded(1/1) from(*[army;orc;goblin]...) plus(1)`; 2/2 menace | TRUE |
| Gothmog {3}{B} 3/3; amass 1; TOKENS gain deathtouch | `_AMASSORC1_` + `lord(creature[token]|myBattlefield) deathtouch`; 3/3 | TRUE |
| Foray {3}{R}: amass 2 then damage = Army power, creature-only | `counter(1/1.2)` + `damage:power target(creature|opponentbattlefield)` | TRUE |
| Swarming {2}{R}: Treasure + amass 2 | `_TREASURE_` + `_AMASSORC2_` | TRUE |
| Muster {1}{B}: draw 1, lose 1, amass 1 | `draw:1` + `life:-1` + `_AMASSORC1_` | TRUE |
| Bowmasters {1}{B} 1/1; deals exactly 1; ETB + opp-draw trigger | `damage:1 target(anytarget)`; `@drawfoeof(player)...` | TRUE |
| Vanguard {1}{B} 2/1; amasses when it DIES, not on entry | `auto=_DIES_ _AMASSORC1_`; power=2 toughness=1 | TRUE |
| Vanguard is the ONLY death-amass creature in the deck (new) | full decklist sweep vs primitives: no other `_DIES_` amass | TRUE |
| Crebain {2}{B} 1/1 flying; amass 2 on entry | `abilities=flying` + `_AMASSORC2_`; 1/1 | TRUE |
| Warg {3}{B} 3/4 menace; 4/4 while you control an Orc | `aslongas(*[orc;goblin]...) 1/0`; 3/4 menace | TRUE |
| Olog-hai {3}{R} 4/4 trample; blocks only with a Goblin/Orc | `cantblock` unless goblin/orc; 4/4 trample | TRUE |
| Sauron {3}{B}{R} 4/4; steal ueot, untapped+hasty; {1}{B}{R}: +2/+0 + drain 2 | steal `moveto(mybattlefield)` + `untap`,`haste` ueot; `life:-2 opponent` + `2/0 ueot` | TRUE |
| Witch-king {4}{B}{B} 5/3 flying; attacker forces sac of lowest-power | `_ATTACKING_ ... power=power:lowest ... sacrifice`; 5/3 flying | TRUE |
| Ugluk {2}{B}{R} 3/3; +1 per death, +2 if Orc/Goblin | two `@movedTo(...graveyard)` lines; 3/3 | TRUE |
| Grishnakh {2}{R} 1/1; amass 2 then steal nonlegendary power<=Army | `target(creature[-legendary&power<=p]...)` untap+haste ueot | TRUE |
| Assault {X}{R}{R}{R}; plain face at 0 Armies, alt face otherwise; double strike + haste (new) | `restriction`/`otherrestriction` on army count; `emblem ... double strike / haste ueot` | TRUE |
| Assault "option 1 is always the largest X" | render behaviour, validated A-116 seq15, A-152 seq19 | TRUE (render) |
| Feed {1}{B}: destroy creature OR enchantment (opponent's), lose MV life | `target=*[creature;enchantment]|opponentBattlefield` + `life:-manacost` | TRUE |
| Mulligan facts: Swamp only {B} source, no duals, 22 of 34 spells need {B}, 26 lands | decklist count: 34 spells, 22 with {B} pips, 12 Mountain + 14 Swamp | TRUE |
| Quoted render strings ("right now:", "counted by the engine:", "costs you N life", "deals N", "may block", "[tapped -", "[infect]") | grepped in all three corpora by the two prior reviews | PRESENT |

Class sweep run on the corrected false-fact class (death-trigger claims): the guide now contains
exactly two death-amass assertions (Vanguard entry, blocking step-2 paragraph) and both name
Vanguard alone; the DECIDING SITUATIONS recap matches.

Freeze verdict (two lines, per the gate): the teaches FIRED — behavioural evidence above, all
three corpora. The teaches are TRUE — ledger above, one row per claim. NOT proposing a freeze:
this revision ships five new support lines that need one corpus of validation on the FP8 pilot.

## Rotation-relevant observation (adherence/maturity-shaped, per ruling #1)

This guide is rail-heavy by design and its rails are now proven at three tiers; both prior
G1-induced failure modes die with this revision, and the remaining arm-C defect class
(single-number misreads) is tier-intrinsic noise, addressable half by these support lines and
half by render hardening (notes.md). One validating FP8 corpus with zero G1-class and reduced
misread-class events would make this seat a rotation candidate — its open items are engine/render
lane, not guide lane.
