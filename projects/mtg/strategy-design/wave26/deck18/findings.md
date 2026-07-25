# deck18 (Kithkin, mono-W tribal anthem aggro) - wave-26 STEP-0 GUIDELESS DEBUT review

Corpus: matchups-20260725-115216, binary 77e91ef4e. deck18 record 4/6.
- WINS: vs102 (t10), vs131 (t19), vs22 (t8, fast), vs27 (t17).
- LOSSES: vs137 (adj 8-6 at cap), vs133 (t14, ground out by mono-B devotion).
Fallbacks at this seat: ZERO (consistent with brief - the lone non-defer fallback
corpus-wide was at deck133). All 6 mulligans were clean keeps.

Card facts verified against primitives (bin/Res/sets/primitives/mtg.txt). Engine
divergences from real MTG routed to notes.md; the GUIDE uses engine behavior.

## The deck as the engine actually runs it

Mono-white weenie tribal. Cheap Kithkin/Soldier bodies + stacked static buffs:
Wizened Cenn (lord: other Kithkin +1/+1), Field Marshal (lord: other Soldiers +1/+1
AND first strike), Thistledown Liege (lord: other white/blue +1/+1; itself only 1/3),
Glorious Anthem (all creatures +1/+1). Payoffs: Goldmeadow Dodger (can't be blocked by
power>=4), Cenn's Heir (grows per other attacking Kithkin), Armored Ascension (aura:
flying + big per-Plains buff = finisher), Mobilization (Soldier vigilance + {2}{W}
token maker). Clock ~turn 5-8; it is the beatdown in every corpus matchup.

The model already plays the CORE loop well: curves out, deploys a creature nearly
every turn, and attacks regularly (vs22 s6/s9/s13/s16 = a clean escalating alpha-strike
kill by t8; vs27 grinds to a win). Mulligan judgment is sound (all 6 keeps correct,
textbook 3-4 Plains + threats; no color anxiety on the {W/U} hybrids). So the guide's
job is NOT the loop - it is three specific leaks, all high-tax, two of them loss-relevant.

## LEAK 1 (loss-relevant, top priority): Goldmeadow Dodger's evasion is not applied at combat

Goldmeadow Dodger `auto=cantbeblockedby(creature[power>=4])` - unblockable by any
creature of power 4+. This is the deck's key evasion vs big-creature decks.

- vs133 s17 (attackers, 14,351 chars / 200s - the corpus's biggest single spike at
  this seat): attacking Goldmeadow Dodger (2/2) into a lone Phyrexian Obliterator
  (5/5 trample). The model spent 14k chars agonizing "the opponent can block with the
  Obliterator ... my Dodger will die," running trample math, weighing whether to
  attack at all. The Obliterator has power 5 >= 4, so it CANNOT block the Dodger - the
  Dodger connects for 2 every turn, freely. The model never once applied the evasion.
  It chose to attack (correct outcome) but for the wrong reason and at maximum tax, in
  the game it LOST by attrition.
- Cross-phase failure: at the vs133 MULLIGAN (s1) the same model wrote "two Goldmeadow
  Dodgers (evading large blockers)" - it KNOWS the fact in the abstract at mulligan,
  then forgets it at the attack seam. So the corrective must be stated as an ATTACK
  DIRECTIVE ("swing it every turn, a big blocker can't stop it"), not just a capability
  line - the fact does not transfer from mulligan to combat on its own.

## LEAK 2 (loss-relevant): the deck has no flyers/reach; the model tried an illegal flyer block

The deck has zero flying/reach creatures (Armored Ascension is the only flying source,
and only once resolved). Ground creatures cannot block flyers.

- vs137 s10 (blockers, 9,904 chars): opponent attacked with Faerie Guidemother (1/1
  FLYING, A1) + Lovestruck Beast (5/5, A2). The model reasoned 9.9k chars, concluded
  emphatically "**Block the 1/1** is the only logical choice ... a massive win," and
  its FINAL coded line was `BLOCKS: B1:A1` (Ballynock Cohort blocks the Faerie). That
  block is ILLEGAL - Ballynock Cohort has no flying/reach, cannot block a flyer. The
  engine correctly dropped the illegal assignment; result = no blockers, took all 6
  (L19->13). The model never considered that a 1/1 flyer is unblockable by its ground
  2/2. This fed the vs137 loss (adjudicated 8-6 at the cap).
- C4-GATE DECOMPOSITION (per brief): chosen_text="no blockers" vs a clear final answer
  `BLOCKS: B1:A1`. This is NOT a parser-hardening misfire. The reply's first line was
  `BLOCKS: B1:A2` (block the 5/5, legal) and its concluding line was `BLOCKS: B1:A1`
  (block the flyer, ILLEGAL). Last-BLOCKS-line precedence fired correctly and took the
  concluding line; that line was rules-illegal, so it was dropped to no-blockers. The
  drop is CORRECT-BY-RULES; the fault is model combat comprehension (flying), not the
  parser. (Incidentally no-blockers was also the best available outcome here - blocking
  the 5/5 loses the Cohort for nothing - but the model did not choose it deliberately.)
- Not systematic: at vs137 s24 the model correctly let the Faerie through ("Block Faerie
  with NO ONE, let it hit for 1") while gang-blocking the 5/5. So the blindness is
  intermittent, but it cost a full 9.9k tax and a phantom answer in the loss - guide it.

## LEAK 3 (pervasive tax, correctness-neutral): re-deriving printed P/T from base + lords

The board lines already render current stats, e.g. "Field Marshal (3/3) (printed
2/2)". The model repeatedly ignores the rendered number and recomputes from base + each
lord/anthem, spiraling. This is the classic TRUST-THE-RENDERED-NUMBER shape (deck62/
deck17/deck35/deck59 lineage) and it is the dominant tax on this stat-stacking deck:
- vs102 s14 (Armored Ascension target, 11,972 chars): recomputes every creature's size
  and lethal from base + Glorious Anthem + aura, twice, second-guessing correct numbers.
- vs131 s18 (10,923), s22 (13,327); vs133 s12 (13,895, casting a 2nd Field Marshal);
  vs27 s11 (11,636 block), s13 (7,806 anthem), s19 (10,555), s23 (13,073 token).
All landed correct choices - this is a LATENCY/tax leak, not a correctness leak, but on
a deck that adjudicated one loss at the cap (vs137) it also costs games via p90 latency,
and it is the #1 saliency signal for what to teach (Step-0-bis rule 2).

## LEAK 4 (interface-seam tax): Mobilization's token ability

Mobilization `auto={2}{W}:_SOLDIERTOKEN_` is an ACTIVATED ability, surfaced as a
priority-window option ("Create soldier with Mobilization [cost: {2}{w}]"), NOT in the
cast list. The model was confused about this:
- vs137 s13 (3,471 chars): "I cannot activate Mobilization's ability directly from the
  list provided ... the interface doesn't offer an Activate Mobilization button."
It DID find and use the token option later at priority windows (vs137 s16/s20, vs27
s23/s25/s28/s29), so no game was lost, but the interface confusion is real recurring
tax (deck49 target-sub-menu family). A one-line interface note pre-empts it.

## STRENGTH to reinforce, not just leaks: the first-strike engine

When the rendered [first strike] tag is on its own board, the model USES it well:
- vs137 s24: gang-blocked a 5/5 Lovestruck Beast with Zealous Guardian + 2 Soldier
  tokens, all 2/2 FIRST STRIKE (Field Marshal buff) - the three first strikers kill the
  5/5 before it deals damage, none die. Textbook. Worth naming as the deck's combat
  identity so the model reaches for it on purpose (it is the correct answer to most big
  attackers this deck faces).

## c1-c5 observations (per brief)

- c1 (full target enumeration): WORKING. vs102 s14 Armored Ascension enumerated all 3
  legal targets with the opponent's Zombie army listed FIRST, no "(+N more)" cap. No
  prompt-length pathology observed.
- c2 (ETB pay-or-tap annotations): N/A at this seat - deck18 plays only Plains, never
  sees its own shock/pathway ETB. (Opponents' Temple Gardens appear on their side only.)
- c3 (PT-pump delta render): N/A - all of deck18's buffs are STATIC lords/anthems, no
  activated pump seam. The "(printed X/Y)" static render is present and is exactly the
  number the model should trust (see Leak 3).
- c4 (parser hardening): one candidate examined (vs137 s10, above) - decomposed as a
  CORRECT rules-based drop of an illegal block, NOT a hardening misfire. Full-corpus
  scan of all 6 games found no other choice-vs-final-answer contradiction and no
  fallbacks at this seat. No misfire.
- c5 (may-ask renders): N/A - no may-triggers at this seat. Mobilization's token is an
  activated ability, not a may-trigger (see Leak 4).
