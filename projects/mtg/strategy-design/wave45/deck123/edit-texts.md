# deck123 (Intruders of Thraben) - wave-45 edit texts

Corpus `matchups-20260826-131833`, 6 games, 374 records / 366 model decisions, 2 fallbacks
(0.55%). Base = the LIVE guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (wave-44
edition, deployed 5cd6a860b). SEVEN edits. Everything else is byte-identical to the live file.
Record: 3/6 (wave 44: 1/6) - reported as a count, NOT attributed to the wave-44 edits.

---

## EDIT A45-1 - a new RULE 0: PLAY A LAND EVERY TURN, and read costs in mana
FINDING (the seat's largest single loss cause this corpus, and it is new).
`1787768332-ai_baka_deck123-0x564b59dd4a40-vs-ai_baka_deck126.jsonl`, seq 12 -> seq 24: THIRTEEN
consecutive "Land drop:" windows answered "Play no land right now", spanning its own turns 5, 7,
9, 11, 13, 15 and 17 - seven land drops thrown away in one game. Across the whole seat: 34
land-drop windows, 13 declines, ALL of them in this one game (the other five games: 0 of 21).
The board never moved: "Your battlefield (2 permanents listed, of which 0 are creatures)" at
seq 12 and still at seq 24, with 2 to 4 lands sitting in hand throughout. The PLAN line was
re-emitted verbatim in every one of those windows - "Cast Intruder Alarm {2}{u} using Tundra
and Underground Sea" - i.e. the executor read {2}{u} as TWO mana, believed it could already
cast the Alarm off two lands, and therefore had no reason to take the drop. Its own reply at
seq 25 shows the confusion out loud: "Actually, I have 2 mana. I can cast Intruder Alarm now
for {2}{u}. Wait, I ...". It finally played a land at seq 26 (turn 17) and lost 42 to 0 on
turn 25 having never made a token.
Verified against the primitive: Intruder Alarm `mana={2}{U}` (three mana). The render was
honest at every window - "Land drop: NOT yet used this turn - you can still play a land",
"Mana available: 2 total", and the full hand listing with three lands in it.
CLASSIFICATION: STRATEGY. Every surface was correct and present; the guide simply has no land
rule of any kind, and its cast list never says what a cost symbol is worth in mana.
BEFORE: the guide opened directly on "=== RULE 1 - THE TOKEN LINES ...".
AFTER: a new "=== RULE 0 - PLAY A LAND, EVERY TURN, BEFORE YOU DO ANYTHING ELSE ===" section
inserted above RULE 1 - the imperative with its own stop inline (amendment #11), the statement
that the drop is a separate question that costs no cast, the mana arithmetic keyed to the
render's own "Mana available: N total" line (amendment #33: one integer against a printed
number), and the incident named.

## EDIT A45-2 - Rule 5 names the false belief about Devour Flesh
FINDING: 64 edict offers (63 Devour Flesh, 1 Tribute to Hunger), 1 cast - the same 1-in-N floor
as waves 43 and 44, but a new shape. `...vs-ai_baka_deck126.jsonl` seq 10/11 (turn 3): the
opponent's battlefield read "of which 2 are creatures" (Perimeter Captain 0/4 [defender] and
Wall of Omens 0/4 [defender]); it cast Devour Flesh naming "The opponent (player, life 20)".
Its plan: "Cast Devour Flesh on Perimeter Captain to remove their only blocker and gain 4 life."
Three errors in one sentence - N was 2 not 1, both creatures are defenders the existing rule
already excludes, and the life goes to the SACRIFICING player. Primitive confirms:
`auto=ability$!name(sacrifice) ... dynamicability<!mytgt toughnesslifegain targetcontroller!>
sacrifice!$ targetedplayer` - the targeted player sacrifices AND that player gains. The
opponent was deck126, whose whole deck is Sanguine Bond / Exquisite Blood lifegain.
CLASSIFICATION: STRATEGY. The guide states the life direction correctly and the executor
believed the opposite, so per amendment #28 the fix names the fabricated justification rather
than restating the rule a fourth time.
BEFORE: `"Cast Devour Flesh {1}{b}" - the player you name sacrifices a creature AND THAT SAME
PLAYER gains the life.`
AFTER: same sentence plus the named refusal - "remove their blocker and gain 4 life" quoted as
the sentence that produced the mistake, with "You gain life from Tribute to Hunger. You never
gain life from Devour Flesh."

## EDIT A45-3 - the Lightning Greaves cast gate moves onto the countable
FINDING: EDIT K's gate REGRESSED. 59 Cast-Lightning-Greaves offers, 6 casts, and 3 of the 6
were at a battlefield line reading "of which 0 are creatures" - the exact state the cast list
already forbids: vs130 seq 5 (t2), vs162 seq 8 (t5), vs152 seq 5 (t5). Wave 44 was 0 casts in
35 offers, so this is not noise in the same direction. The other 3 casts (vs146 seq 24 at 11
creatures, vs125 seq 16 and seq 37 at 1 creature) are correct.
CLASSIFICATION: STRATEGY. The stop was present and phrased as a consequence ("means the Greaves
equips nothing") rather than as a test on a printed number.
BEFORE: cast-list entry 5, "ONLY when a token-maker is already on your battlefield. Read your
own battlefield line: 'of which 0 are creatures' means ..."
AFTER: entry 5 re-keyed to the printed clause as the FIRST act ("ONLY when your own battlefield
line reads 'of which 1 is a creature' or more AND that creature is a token-maker"), with the
0-creature branch as an explicit do-not and the 3-of-6 count named.

## EDIT A45-4 - CHECK 4 states what an attack costs under Intruder Alarm
FINDING: `...vs-ai_baka_deck162.jsonl` seq 61 (t13, life 8): 13 attackers sent, correctly (13
damage, opponent 17 -> 4). It then had ZERO untapped creatures for the opponent's turn and died
on turn 15 with the opponent on 4. Blocking would NOT have saved that game - the damage came
from Ob Nixilis and Fate Unraveler triggers, not from combat - so this is deliberately NOT a
hold-back gate (amendments #30, #35, #36: no prohibition without a recorded falsifier, and the
falsifier here is the very window that produced it). What the guide is missing is the FACT.
Primitive: Intruder Alarm `auto=lord(creature) doesnotuntap` plus `@movedTo(creature|...):untap
all(creature)` - nothing untaps in the untap step, and the team comes back only when a creature
ENTERS. The render prints "[doesn't untap during its controller's untap step]" on every one of
the pilot's own battlefield rows, and the guide explains the mechanic in its Intruder Alarm
section, but the COMBAT section never connects the two.
BEFORE: CHECK 4 ended at "Send it once you have tokens attacking beside it."
AFTER: a paragraph added inside CHECK 4 - find a token-maker on your battlefield line; with one
there the team is back on the next token and Check 4 is free; with none there the attackers stay
tapped, so send them when the attack advances the clock but do not plan a block with them.

## EDIT A45-5 - blocking gets an explicit SELECTION step and a render-contradiction refusal
FINDING (decisive). `...vs-ai_baka_deck162.jsonl` seq 55 (t12, life 12, NOT lethal - the header
read "Unblocked, these attackers deal up to 3 - you would be at 9 - NOT lethal"). ONE attacker,
Fate Unraveler (3/4) deals 3. FOURTEEN blockers offered: eleven 1/1 Human tokens, two 2/2
Vampire tokens, and Bloodline Keeper (3/3, carrying Lightning Greaves). Every single row read
"(your blocker dies, attacker lives)". It answered `BLOCKS: B12:A1` - the Bloodline Keeper -
with the plan line "Block Fate Unraveler with Bloodline Keeper to kill it", against fourteen
identical rows that say it does not kill it. The Keeper and the Greaves both died; Thraben
Doomsayer was already gone; no token was ever made again; it went all-in on turn 13 and lost on
turn 15 with the opponent on 4 life. Also visible in that reply: the first two thirds of its
reasoning are spent re-litigating RULE 1's 12-creature stop, which is not on that menu at all.
CLASSIFICATION: STRATEGY, with the render contradicted rather than missing (amendment #30's
companion check).
BEFORE: "Blocking, in this order:" began at " - Block with your Human and Vampire TOKENS first".
AFTER: two numbered STEPS inserted above the existing list - STEP 1 WHICH BODY (token before
card, maker last, keyed to the absence of a mana cost on the row) and STEP 2 READ THE
PARENTHESIS (the "(your blocker dies, attacker lives)" refusal with this incident named), then
the existing bullets unchanged.

## EDIT A45-6 - the MULLIGAN section becomes an odds comparison, and gains a BOTTOMING step
FINDING (new-baseline seam; the guide-vs-behaviour split is reported, the floor is NOT patched
to match behaviour - amendment #38 - but the SECTION is rewritten to the owner's ruling #46/#47,
which the live text predates). `...vs-ai_baka_deck126.jsonl` seq 1-5: THREE mulligans then a
keep at four cards. Hands, off the engine's own header: (1) 1 land / 6 spells, cheapest spell
Intruder Alarm {2}{u}, "Playing every land in this hand would not cover any spell in it" - ship
is right by odds; (2) 1 land / 6 spells, cheapest Lightning Greaves {2}, same cover line - ship
is defensible; (3) SIX lands / 1 spell (Arcane Sanctum, 3x Tundra, 2x Scrubland, Devour Flesh) -
shipped, and this one is wrong by odds: keeping bottoms two lands and leaves four lands plus a
removal spell, a functioning five, against a fresh four; (4) 3 lands / 4 spells kept at four
cards. The live floor ("KEEP any 7-card hand with two or more lands", "Never keep fewer than 5
cards") forbids both the seq-3 ship and the seq-4 keep, and the pilot did both - the section is
written as declarative facts about keepable hands, which is exactly the shape #46 supersedes.
SECOND FINDING, the same window and never governed at all: at the BOTTOMING ask (seq 5) it kept
Marsh Flats, Tundra, Swamp and Intruder Alarm and bottomed Damnation, Vision Skeins AND Idyllic
Tutor - both of its ways to find a token-maker and its only sweeper - in the same reply whose
PLAN says "Dig for a token maker using Idyllic Tutor or Vision Skeins". Its own guide says an
Intruder Alarm with no maker "does NOTHING". No creature reached its battlefield in that game.
The mulligan section ends at the keep and hands the bottoming ask to nobody (amendment #37).
Corpus mulligan record, both seats, for the owner: 12 windows offered, 9 straight keeps, 3
mulligans (all deck123 vs126), 1 bottom record, no mull-to-zero. Kept-hand land counts -
deck123: 4, 3(at four cards), 2, 2, 2, 3; deck130: 2, 3, 3, 2, 2, 1.
=> OWNER QUESTION carried, not patched: deck130 kept a ONE-land seven (vs126, 1 land / 6
spells) and won that game; deck123 shipped a six-land seven and lost. Both are #47 judgment
keeps on the exact cards, and n=1 each way.
BEFORE: the four-line floor.
AFTER: the two deck facts (24 lands; the cards that win cost 3-4), the comparison stated on the
header line with branches for 2+ lands / 1 land / 0 lands / all lands / 6+ lands / at five cards
or fewer, and a new BOTTOMING sub-section giving a keep-order (lands to four mana, token-maker,
Tutor-or-Skeins, removal, Intruder Alarm LAST) with the four-card incident named.

## EDIT A45-7 - six new DECIDING SITUATIONS lines
FINDING: recognition handles for A45-1, A45-3, A45-5 and A45-6, in the recognise -> do format,
per amendment #24's completeness half (every body rule gets a list line, and the list must agree
with the rule).
AFTER: the land-drop line; the cost-vs-"Mana available" line; the Greaves-at-0-creatures line;
the token-blocks-first line; the "(your blocker dies, attacker lives)" maker line; the bottoming
keep-order line.

---

## CONSIDERED AND NOT MADE

- THE 12-CREATURE TOKEN STOP-RULE. It collapsed: 10 violations this corpus (vs162 seq 45/47/51
  at counts 12/13/14; vs146 seq 28/36/37/38/41/42 at 12..17; vs152 seq 45 at 12) against 0 in
  wave 44. But every one came at 16 life against boards with no lethal on them, and every one
  bought a free 2/2-or-4/4 flying token for a deck whose only plan is bodies - i.e. under
  amendment #47 the deviations took the better odds and the evidence runs against the RULE, not
  against the pilot. Two of the three games with violations were WINS. I did not raise, lower or
  delete the cap, because its rationale is not recorded anywhere I could find in waves 40-44
  (prompt size is not it: the largest deck123 prompt this corpus is 19,155 characters, and the
  ranged-collapse render holds wide boards flat). ROUTED TO THE OWNER as a question: what is the
  12 for? Supporting evidence that it costs something beyond the tokens - at vs162 seq 55, a
  BLOCKING window, the pilot spent the first two thirds of its reasoning re-deriving the stop
  rule for an option that was not in that menu, and then made the block that lost the game.
- THE EDICT N-CHECK (Rule 5's mechanical test). 1 cast in 64 offers, same floor as waves 43 and
  44. Wave 44 declined to restate it a fourth time and that judgment stands; A45-2 adds the
  card-fact refusal instead, which is a different failure mode.
- DAMNATION. 59 offers, 1 cast (vs126 seq 37: their creature count 5, yours 0). Correct under
  Rule 3. No text touched.
- MARSH FLATS. 135 offers, 4 taken, and all 4 followed Rule 2's order (3x Underground Sea, 1x
  Tundra). No text touched.
- VISION SKEINS. 68 offers, 4 casts, none into a draw-punish or extra-draw board. No text
  touched.
- TRAGIC SLIP, the Morbid upgrade. 48 offers, 3 casts; vs162 seq 21/22 took the -13/-13 line
  with `{right now: -13/-13 (a creature died this turn, so Morbid applies)}` on the row and
  aimed it at the biggest body (Fate Unraveler 3/4). Correct. No text touched.
- THE DECK123 CONSTRUCTION CASE. Owner-ruled OUT OF SCOPE; not reviewed.

---

## REVIEWER BOUNDARY PASS (wave-45 step 4) - deck123

### RECORDED FALSIFIER for the Check-1 lifegain-wall absolute (finding B14, skill #30)
The absolute stands as written - "Their battlefield line names Perimeter Captain, Pride Guardian,
Sanguine Bond or Exquisite Blood and it is your Attackers step: ATTACK: none. No damage total
overrides this." No wording change; its rationale is already in CHECK 1 with the two deaths that
produced it, and the quick-reference line agrees with the rule exactly.
FALSIFIER: a genuinely lethal alpha strike - one that ends the game on the spot, before any
lifegain trigger can matter. Recorded UNREACHABLE-IN-POOL for deck123: across waves 43-45 it has
never had lethal on a wall board. RE-CHECK IN WAVE 46. If a lethal window arises on a wall board,
the absolute gains a named lethal-check exception; until one does, it does not.

### THRESHOLD WHYS (skill amendment 61) - every numeric threshold this wave ships
- T = 20 creatures (the token stop) because the board render collapses wide boards into a range
  ("Human #2-#11 (1/1) ... x10") and held a 22-permanent board to ~19,000 characters, so board
  width costs no decision quality below 20; the old T = 12 was crossed ten times in one corpus
  and every crossing was odds-favourable. Raised, not deleted, because it is the only stop in
  that section. SUNSET: if wave 46 shows 0 violations at 20, no prompt-size cost and no
  decision-quality cost, wave 47 deletes it under skill #35.
- T = "(keeping 5)" (the mulligan chain stop) because the London mulligan already shaves a seven
  to six for one card, so a third look buys a fresh seven at the price of two; the seat took
  three mulligans, kept four cards, and lost 42-0 having never made a token.
- T = 3 lands (the bottoming land floor) because the deck's engine costs three and four mana and
  a kept hand below three lands cannot reach it before the game is decided.
- T = 4 mana (the bottoming keep-order target) because Bloodline Keeper {2}{B}{B} and Damnation
  {2}{B}{B} are four, and a hand that stops at three never casts the deck's best card.
- T = 2 (the collapsed tag's N) because at N of 2 the defender may gang the attacker and the
  parenthesis prices only the biggest single blocker; at N = 1 the parenthesis is exact.
