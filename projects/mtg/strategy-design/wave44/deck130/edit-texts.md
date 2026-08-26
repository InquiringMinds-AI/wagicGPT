# deck130 (Budde's Ponza) - wave-44 edit texts

Corpus: matchups-20260826-111937, 6 games, 343 model decisions, 0 fallbacks.
Base = the LIVE guide projects/mtg/bin/Res/ai/baka/deck130_strategy.txt (wave-43 edition).
Five edits, all in the Starstorm / damage-targeting / blocking lanes.

---

## EDIT 1 - rule #2 step 0 gains a DO NOT TAKE IT half, and the X-menu warning
FINDING (two decisive self-wipes, both with the annotation telling the truth):
`1787761212-ai_baka_deck130-0x55df6ecbf9d0-vs-ai_baka_deck126.jsonl` seq 44/46, turn 17: opponent at 9, this deck at
19 with Rorix Bladewing (6/5 flying haste) as its only creature - lethal in two swings. The
Starstorm row read `{X pricing: max affordable X=5 ...; smallest X that kills an OPPONENT
creature: X=1 (3 mana total) - kills THEIRS: Vampire; YOURS: none. At X=5 - kills THEIRS: Pride
Guardian x2, Wall of Omens x2, Vampire; YOURS: Rorix Bladewing}`. The pilot's plan, written
twice, was "Cast Starstorm at X=1". At the announce menu it answered `CHOICE: 1` - and option 1
was X=5. Rorix died; the deck never threatened again and lost at turn 22.
`1787761208-ai_baka_deck130-0x55894935dbc0-vs-ai_baka_deck162.jsonl` seq 81/83, turn 21: life 1, opponent 9, board
Rorix + 2 Goblins + Blastminer. Annotation: `smallest X that kills an OPPONENT creature: X=5 -
kills THEIRS: Shield Sphere; YOURS: Dwarven Blastminer, Goblin x2, Rorix Bladewing`. This one
was deliberate - it chose option 2 (X=5), its own plan line saying "kill Shield Sphere and
remove own creatures". Shield Sphere is a 0/6 `[defender]` (primitive: `abilities=defender`,
power 0). It traded its whole board and its only clock for a wall that cannot attack, and died.
CLASSIFICATION: the first is PERCEPTION at the announce menu (see engine item H1); the second
is STRATEGY. Both are guarded by the same prose.
BEFORE: step 0 was a single TAKE-IT paragraph ending "Losing your own Blastminer to kill two of
their creatures is a trade you take."
AFTER: the TAKE-IT clause is preserved verbatim in substance, then a DO NOT TAKE IT clause with
two named conditions - (a) the YOURS list at that X names Rorix Bladewing or Siege-Gang
Commander, (b) the THEIRS list at that X names only `[defender]` creatures - and a closing
paragraph on the inverted announce menu ("option 1 is NOT X=1; answer the row whose text reads
X = your number"), each carrying its own incident.

## EDIT 2 - Starstorm's card line in YOUR CARDS
FINDING: same two incidents; the one-line card entry told the pilot to "pick the smallest X"
but said nothing about holding the card once a clock is out, and it quoted the menu's
"option 1 is the LARGEST X" without drawing the conclusion.
BEFORE: `STARSTORM ... Its X menu says "option 1 is the LARGEST X"; pick the smallest X that
reaches the toughness you must kill. Because it kills your own Blastminers, goblins and
Siege-Gang too, SWEEP FIRST and deploy Siege-Gang AFTER.`
AFTER: "two or more creatures that are NOT marked [defender]"; "option 1 is therefore never
X=1 - read each row's `X = ` text and answer that row's number"; "kills your own ... AND RORIX
too"; and "once Rorix or Siege-Gang is already on the battlefield, Starstorm is a card you
hold, not a card you cast".

## EDIT 3 - the face-burn DECIDING SITUATIONS line reversed
FINDING: 9 face-damage casts this corpus, all in `1787761182-ai_baka_deck130-0x5653dcbb8350-vs-ai_baka_deck125.jsonl`, at
opponent life 25, 22, 18, 20, 17, 19, 24, 27, 24 - every one above the guide's own 6-or-less
floor, and the opponent's life was visibly RISING between them (18 -> 20, 19 -> 24, 24 -> 27).
The guide's rule #2 step 4 forbids exactly this and its CROSS-CHECK paragraph forbids it twice;
the DECIDING SITUATIONS line at the bottom AUTHORISED it. The pilot followed the bottom line.
An internal contradiction in one guide is resolved by whichever clause is nearest the decision.
CLASSIFICATION: STRATEGY / guide-internal contradiction.
BEFORE: `-> hold the card if it is Starstorm; otherwise fire it at the opponent and move on, do
not agonise.`
AFTER: "do NOT fire it at the face. Hold the card." + the 25 -> 24 measurement + the two
conditions that release it (a creature or planeswalker appears; their life is 6 or less) + the
lethal-chain exception pointer.

## EDIT 4 - blocking gains a lethal branch and a legality statement
FINDING: `1787761202-ai_baka_deck130-0x55882f2454d0-vs-ai_baka_deck146.jsonl` seq 32, turn 16: life 3, prompt LETHAL,
attackers Nadaar 4/4 "deals 4" and Goblin 2/2 "deals 2", one blocker (Dwarven Blastminer 1/1,
`may block A1 (your blocker dies, attacker lives), A2 (your blocker dies, attacker lives)`).
It blocked the Goblin, took 4, and died at -1. Blocking Nadaar takes 2 and leaves it at 1 ALIVE.
Its reply says "Nadaar is unblockable by Blastminer" - the row it was reading says the opposite.
The live BLOCKING paragraph has no rule for which attacker to chump.
CLASSIFICATION: STRATEGY (a render statement was contradicted, not missing).
BEFORE: the three-sentence BLOCKING paragraph.
AFTER: same three sentences, then "WHEN THE PROMPT SAYS LETHAL, CHUMP THE BIGGEST ATTACKER"
keyed to the `deals N` field with the 3-life 4-and-2 arithmetic, and "the `may block` list is
the LEGALITY list".

## EDIT 5 - five new DECIDING SITUATIONS lines
Recognition handles for Edits 1-4: the YOURS-names-Rorix stop, the defender-only stop, the
announce-menu row rule, and the lethal biggest-first blocker.

---

## CONSIDERED AND NOT MADE

- MULLIGAN floor ("Mulligan only a hand with zero lands, or a hand that is all lands"). Two
  2-land keeps this corpus (vs152: 2 Mountains + 2 Starstorm + 2 Siege-Gang + Lay Waste, i.e.
  five cards costing 4-5; vs162: 2 Mountains + 2 Siege-Gang + Spellbomb + Lay Waste + Stone
  Rain). Both lost, but so did four other games, and n=2 with a fair-hand corpus one wave old
  cannot move a floor. Routed to the owner in the seat file.
- The cycling table. Starstorm cycled 2/33 offers and both were legal (opponent battlefield
  "of which 0 are creatures"); Lay Waste cycled 0/26; Forgotten Cave 2/38. The rung is holding;
  no text touched.
- The Blastminer attack floor. 7/7 windows where Blastminer appeared on the attack list, it was
  left out. No text touched.
- The land-destruction curve. 21/21 destroy-targets were tagged `[opponent's battlefield]`,
  0 self-hits. EDIT 8's backstop retirement holds; no text touched.
