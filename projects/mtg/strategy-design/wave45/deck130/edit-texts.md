# deck130 (Budde's Ponza) - wave-45 edit texts

Corpus `matchups-20260826-131833`, 6 games, 225 records / 213 model decisions, 0 fallbacks.
Base = the LIVE guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (wave-44 edition,
deployed 5cd6a860b). FIVE edits. Everything else is byte-identical to the live file.
Record: 3/6 (wave 44: 1/6) - reported as a count, NOT attributed to the wave-44 edits.

---

## EDIT 45-1 - the cycling table's Starstorm row learns that X=0 is not cycling
FINDING (decisive in the game it appears in).
`1787768330-ai_baka_deck130-0x559cc3d27570-vs-ai_baka_deck123.jsonl` seq 19 / 20 / 21, turn 9.
Board: four Mountains, four mana, opponent battlefield "of which 0 are creatures", hand = 2
Siege-Gang Commander, 2 Rorix Bladewing, 2 Hammer of Bogardan, Starstorm. The cast row carried
`{X pricing: max affordable X=2 (4 mana total); there is no creature on the battlefield for it
to damage}`. It cast Starstorm anyway, with the plan "Cast Starstorm at X=0 (free, kills no
creatures, draws nothing, but spends the card to avoid cycling it later or holding a blank)",
then at the modal menu (seq 20) chose "1. Cast Card Normally" over the "2. cycling" row that was
sitting directly beneath it, then announced X=0 at seq 21. Primitive confirms what that costs:
Starstorm is `auto=damage:X all(creature)` with `autohand=__CYCLING__({3})` - the cast at X=0
deals nothing and draws nothing; only the cycling row draws. It had 4 mana and cycling costs
{3}. The card it would have drawn is the fifth land the rest of that game needed: it stayed on
four lands to the end, answered "Cast nothing right now" at turns 11 and 13 with two Rorix
({3}{R}{R}{R} = SIX mana) and two Siege-Gang ({3}{R}{R} = five) stranded in hand, never put a
creature on the battlefield, and lost 19 to -23 on turn 14.
The live guide's own cycling table already released Starstorm at exactly this board state
("when it reads 'of which 0 are creatures' and you hold no Forgotten Cave, CYCLE THE
STARSTORM") - the pilot reached the right release and then took the wrong row.
CLASSIFICATION: STRATEGY. Both rows were on one screen with their own names.
BEFORE: the Starstorm row of the #1 RULE cycling table ended at "With a Forgotten Cave in hand,
cycle the Cave first."
AFTER: same text plus "CASTING IT AT X=0 IS NOT CYCLING" - the two menu rows named as they
render, what each one actually does, the "spend the card / cycle it legally" reasoning named and
refused (amendment #28), the incident, and a flat "NEVER announce X=0".

## EDIT 45-2 - a RACE CHECK above the ATTACK FLOOR
FINDING (decisive). `1787768342-ai_baka_deck130-0x55f269ac9490-vs-ai_baka_deck146.jsonl`
seq 21 (t11, life 8, opponent 21) and seq 25 (t13, life 4, opponent 15). Attack list both
times: A1 Rorix Bladewing (6/5) [flying, haste] and nothing else. Opponent battlefield both
times: exactly one creature, Nadaar, Selfless Paladin (4/4) [vigilance] (primitive:
`abilities=vigilance`, base 3/3 with its own dungeon lord making it 4/4 - the render printed
"(4/4) (printed 3/3)"). It attacked both times. Vigilance means Nadaar attacks and still blocks,
so sending Rorix bought nothing defensively; the arithmetic on the screen was 4 damage a turn
into 4 life against 6 damage a turn into 15. It died on turn 14 with the opponent on 9.
Held home, Rorix blocks Nadaar and the blockers screen's own trade line says the 6/5 kills the
4/4 and lives - that removes their entire clock for free.
The live ATTACK FLOOR's only hold-back is "if their untapped creatures' printed power totals
your life or more, send only Rorix this turn and keep the Goblins home" - at one creature that
condition was TRUE (4 >= 4) and its instruction was to send the one creature it had.
CLASSIFICATION: STRATEGY, and structurally the attack-side twin of skill amendment #25 (a floor
evaluates; it never allocates the last body).
BEFORE: the COMBAT section opened on "ATTACK FLOOR - read the 'Your creatures that can attack:'
list ...".
AFTER: a "RACE CHECK" placed ABOVE the floor, armed only when the attack list holds exactly one
creature and it is Rorix or Siege-Gang, stating both divisions as reads off "Your life:",
"Opponent life:" and the opponent's printed powers, with vigilance named as a creature that
counts on both sides, and the incident's numbers.

## EDIT 45-3 - the DECIDING SITUATIONS Starstorm line is made to agree with rule #2 step 0
FINDING (amendment #24's consistency half - the same defect class the list produced for face
burn in wave 44, in a different lane). Windows this corpus where an affordable X killed TWO OR
MORE opponent creatures: SIX. Cast in 1 (vs146 seq 16/17/18, X=2, `kills THEIRS: Goblin,
Triumphant Adventurer; YOURS: none` - free, correct). Declined in 5, ALL in
`1787768348-ai_baka_deck130-0x55d604650990-vs-ai_baka_deck126.jsonl`, and all five declines were
RIGHT:
  seq 18 (t11): `At X=4 - kills THEIRS: Wall of Omens, Perimeter Captain, Overgrown Battlement
    x3; YOURS: Dwarven Blastminer` - every one of those five is printed "[defender]" on the
    opponent battlefield line, so rule #2 step 0's DO-NOT-TAKE-IT (b) applies. It cast Rorix
    instead and started the clock that won the game.
  seq 27, 31, 33 (t15/t16) and seq 38 (t17): `X=1 kills THEIRS: Vampire x2 / x3; YOURS: Dwarven
    Blastminer`, with Rorix already on the battlefield - EDIT 2's hold clause ("once Rorix or
    Siege-Gang is already on the battlefield, Starstorm is a card you hold"). It held, raced,
    and won on turn 17 with the opponent at 3.
So the wave-44 TAKE-IT half and the wave-44 hold clause now point opposite ways at the same
board, and the closing DECIDING SITUATIONS list still carries only the TAKE-IT direction
unqualified. The pilot obeyed the RULE and not the list this time; the list is the clause
nearest the decision and must not be the wrong one.
CLASSIFICATION: guide-internal inconsistency, caught before it cost a game.
BEFORE: `- Their board shows two or more creatures and Starstorm is in hand -> cast Starstorm,
and cast it BEFORE you cast Siege-Gang Commander.`
AFTER: the same line with the two carve-outs from step 0 written into its condition ("that are
NOT marked [defender]", "and NEITHER Rorix Bladewing NOR Siege-Gang Commander is on your
battlefield") plus the explicit hold branch and the five-window incident.

## EDIT 45-4 - BLOCKING distinguishes a token from a card on "(both die)"
FINDING: `1787768322-ai_baka_deck130-0x55be41810fe0-vs-ai_baka_deck152.jsonl` seq 14 (t8, life
19, NOT lethal). One attacker, Wolf (2/2) deals 2. Blockers: B1 Siege-Gang Commander (2/2) -
"may block A1 (both die)"; B2/B3/B4 Goblin (1/1) - "(your blocker dies, attacker lives)". It
answered `BLOCKS: none`. The live rule says "take ... any block where both die", so this is a
letter-violation - and it is the RIGHT play: Siege-Gang Commander is a card, it is one of only
two real bodies in the deck, and its sacrifice ability requires it alive, while the Wolf is a
token deck152 makes again. Under amendment #47 the deviation took the better odds, which is
evidence against the rule text (amendment #39: tokens and cards are different nouns in a combat
rule).
CLASSIFICATION: guide text wrong for the card case; behaviour correct.
BEFORE: `BLOCKING - take any block where your creature kills theirs and lives, and any block
where both die.`
AFTER: the "kills theirs and lives" clause kept, "(both die)" split - always with a GOBLIN
token, never with Siege-Gang Commander or Rorix Bladewing, with the reason (they are the deck's
only bodies; Siege-Gang's ability needs it alive) and the alternative (send a Goblin, or take
the damage while comfortable).

## EDIT 45-5 - the lethal chump rule gains its legality qualifier
FINDING: `1787768328-ai_baka_deck130-0x564c9d0969e0-vs-ai_baka_deck162.jsonl` seq 19 (t10,
life 8, header "Unblocked, these attackers deal up to 8 - you would be at 0 - LETHAL if it all
connects"). A1 Master of the Feast (5/5) deals 5 [flying], tagged `[NONE of your available
blockers can block this attacker - it has flying and none of your available blockers has flying
or reach]`; A2 Fate Unraveler (3/4) deals 3. One blocker, Dwarven Blastminer (1/1), "may block
A2". It blocked A2 and survived the turn at 3 life. The live rule as written -"put your blocker
in front of the LARGEST of those numbers" - names A1, which is not blockable at all; obeying its
letter loses the game on the spot. The pilot read the legality tag and did the right thing.
CLASSIFICATION: guide text wrong; behaviour correct (amendment #47).
BEFORE: "WHEN THE PROMPT SAYS LETHAL, CHUMP THE BIGGEST ATTACKER. ... Put your blocker in front
of the LARGEST of those numbers,"
AFTER: "...CHUMP THE BIGGEST ATTACKER YOU ARE ALLOWED TO BLOCK", with a FIRST step that narrows
the list by the two render strings that remove an attacker from consideration (the "[NONE of
your available blockers can block this attacker ...]" tag, and absence from your blocker's own
"may block" line), then the largest "deals N" among what is left.

## EDIT 45-6 (folded into 45-1) - four new DECIDING SITUATIONS lines
The X=0 / no-creature-to-damage stop; the "answer the cycling row, never Cast Card Normally"
line; the per-row X-pricing reading instruction (the announce rows now carry their own pricing -
see the seat file, #W45-5 PASS); and the RACE CHECK pointer for a one-creature attack list.

---

## CONSIDERED AND NOT MADE

- THE FACE-BURN RULES (rule #2 step 4, the CROSS-CHECK paragraph, and the wave-44 DECIDING
  SITUATIONS reversal). 18 windows offered a damage spell; 5 of them offered ONLY player targets
  ("legal targets right now: the opponent, you") at an opponent life of 19, and all 5 were
  declined (vs123 seq 12, 16, 19, 22, 23). 18 of 18 targeted rows actually taken went at an
  opponent permanent. Wave 44 was 9 of 9 face casts above the floor. The rule is holding; no
  text touched.
- THE CYCLING TABLE'S OTHER ROWS. Lay Waste cycled 0 times; Forgotten Cave played, not cycled.
  Only the Starstorm row needed the X=0 clause. No other row touched.
- THE BLASTMINER ATTACK FLOOR. 7 windows where Dwarven Blastminer appeared on the attack list
  (vs162 seq 9/14/18, vs146 seq 8, vs125 seq 41/45, vs126 seq 19); excluded in all 7. No text
  touched.
- THE LAND-DESTRUCTION LANE. 28 of 28 rows taken that carry an owner tag read "[opponent's
  battlefield]", 0 read "[your battlefield]"; of the 18 TARGET CHOICE asks, 15 went at a land
  and 3 at a creature, none at a player; the curve was followed (Stone Rain / Molten Rain at 3-4 lands in 5 of 6 games).
  EDIT 8's backstop retirement continues to hold. No text touched.
- THE "PICK THE SMALLEST X" LINE. One divergence: vs146 seq 17 planned X=1 ("to kill the Goblin
  and Triumphant Adventurer") and the announce at seq 18 answered X=2, which kills the same two
  ("kills THEIRS: Goblin, Triumphant Adventurer; YOURS: none"). One wasted mana, nothing else -
  the per-row annotation the wave-45 engine work shipped is what makes that safe. The rule text
  already says "Pick the smallest X that reaches the toughness you must kill"; restating it is
  churn. EDIT 45-6's per-row reading line covers the reading habit; the numeric half is a
  prediction (P11), not new prose.
- THE MULLIGAN FLOOR ("Mulligan only a hand with zero lands, or a hand that is all lands"). Six
  keeps, six games, land counts 2/3/3/2/2/1 - including a ONE-land seven (vs126, 1 land / 6
  spells - Mountain, Starstorm, Rorix, 2x Molten Rain, Pyrite Spellbomb, Talisman of Impulse,
  with the header's own "Playing every land in this hand would cover the cost of: Pyrite
  Spellbomb {1}") that the floor keeps and that WON. Under amendment #38 a floor is never moved on
  behaviour and never on n=1; the one-land keep is routed to the owner in the seat file as a #47
  judgment case, with the hand.

---

## THRESHOLD WHYS (skill amendment 61) - reviewer boundary pass, wave-45 step 4
- T = 4 of their lands (land destruction is worth a card at or below it) because Stone Rain and
  Molten Rain buy a whole turn while they are still on four lands and buy nothing once they can
  cast their hand anyway.
- T = 8 of your own lands (cycle Lay Waste above it) because past eight lands a land kill no
  longer changes what they cast that turn, so the card is worth more as a draw.
- T = 6 opponent life (the face-damage floor) because four Pyrite Spellbombs plus three Spark
  Sprays are 11 damage total, which is not a clock - the damage only pays at the number where it
  finishes them.
- T = 5 of your life (block where only your creature dies) because a Goblin is worth more as a
  blocker than as 2 points of face damage once you are inside one attack of dying.
- T = 2 (the collapsed tag's N) because at N of 2 the gang kills a 6/5 Rorix that no single
  blocker on the tag could touch.
- T = 3 lands (the bottoming land floor) because Siege-Gang wants five lands and Rorix six; a
  two-land keep never reaches either body.
