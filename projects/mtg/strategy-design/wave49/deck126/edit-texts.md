# deck126 (Sanguine Blood) - wave-49 edits, before -> after, each with its finding

Corpus `matchups-20260827-094106`; seqs are in the deck126 seat log named. Start point = the live
guide (`bin/Res/ai/baka/deck126_strategy.txt`, wave-48 revision + boundary pass); the full revised
guide is `wave49/deck126/strategy.txt` (36,788 bytes; live 33,391).

## F10 - THE WIN BUTTON: Tribute first, keyed to its row, pressable on your own turn
Finding: `...deck126-0x561500a57220-vs-ai_baka_deck152.jsonl` seq 24, 25 (t13) and 27 (t15): both
enchantments on the seat's battlefield line, the prompt's LOOP clause present ("Both halves of a
life LOOP are on YOUR battlefield"), `Cast Tribute to Hunger {2}{b} {right now: they control 7
creatures - they choose which one}` on the menu - the seat cast Wall of Omens, then Overgrown
Battlement, then "Cast nothing", its plan saying "Since no creatures are on the battlefield, wait
for the opponent to attack or cast Sorin". Seq 28 (t15 main 2) cast Tribute and won (36 / -1).
STRATEGY: Rule #2 listed Tribute fourth, without the row string, and the executor's belief was
that the trigger needed a creature of its own. Also the plan's "wait for the opponent to attack"
is exactly the passivity the section forbids.
Before: " - Declare a block with ANY wall ... - Declare a block with Pride Guardian ... - Deal 1
Damage with Staff of Nin ... - Cast Tribute to Hunger while the opponent has a creature (you gain
its toughness). - Attack with a lifelink Vampire and connect."
After: first entry " - "Cast Tribute to Hunger" with its row reading "{right now: they control N
creatures ...}" and N 1 or more: CAST IT. They sacrifice one, you gain its toughness, the loop
takes the rest. This is the win you press on your OWN turn with no creature of yours anywhere ...
You had both enchantments out and this row at "they control 7 creatures" in three windows across
two turns, cast a Wall of Omens and an Overgrown Battlement and passed once "waiting for the
opponent to attack"; in the fourth window you cast it and won on the spot." + closing sentence
"Do not wait for a better turn, do not wait for their attack, and do not build a bigger board
first: a wall cast with the win on the menu is a turn given away." Matching SITUATIONS bullet.

## F11 - THE PAIR IS TWO DIFFERENT NAMES
Finding: `...deck126-0x56448ebc9f10-vs-ai_baka_deck162.jsonl` seq 12 (t10, life 14): `Cast
Exquisite Blood` taken with `Exquisite Blood` already on the seat's battlefield line; reply plan:
"Both enchantments are now on the battlefield. Win immediately by blocking with Pride Guardian ...
or Staff of Nin ping". Sanguine Bond was in the library. The seat then held its walls waiting for
a loop it did not have and died to the draw lock at t13. PERCEPTION of its own board (two copies
of one name read as the pair); the surface was true (`Exquisite Blood #1-#2 x2` after the cast).
Before (Rule #3): "Both together end the game the moment any life changes. But the two halves are
NOT the same card:"
After: "Both together end the game the moment any life changes. THE PAIR IS TWO DIFFERENT NAMES:
one Sanguine Bond AND one Exquisite Blood on your battlefield line. "Exquisite Blood #1-#2 x2" is
one half twice, not the pair, and a second copy of either enchantment does nothing at all. Read
both names off your battlefield line before you write "both enchantments": you cast a second
Exquisite Blood with the first already there, wrote "Both enchantments are now on the
battlefield", and Sanguine Bond was still in your library. And the two halves are NOT the same
card:"

## F12 - RULE #7 ENTRY 1 at any "leaves N"; a copy already out is not entry 1
Finding: same file seq 7 (t6, five mana): menu `Cast Sorin ... leaves 1`, `Cast Exquisite Blood
... leaves 0 - casting this taps you out`, `Cast Wall of Omens ... leaves 3`, `Cast Chromatic
Lantern`, `Cast Pride Guardian`; the seat cast Wall of Omens then Pride Guardian, plan "ramp mana
... Cast Sorin". The Blood waited until t9 (seq 10). Entry 1 said "cast it" without saying "at any
leaves N", and the seat's mana-holding reflex (from Rule #1's pip lessons) won. Also ties to F11
(entry 1 must be the missing NAME).
Before: " 1. Exquisite Blood or Sanguine Bond - whichever is missing from your battlefield."
After: " 1. Exquisite Blood or Sanguine Bond - whichever NAME is missing from your battlefield
line, at any "leaves N", including "casting this taps you out". A copy of a name already on that
line is not this entry; skip it. You cast a Wall of Omens over an Exquisite Blood at five mana to
"ramp for Sorin"; the Blood was the card that mattered and it waited two turns."

## F13 - CHROMATIC LANTERN: the two entry-order breaks and the four second Lanterns, cited
Finding: Lantern cast with a Rule #7 entry 1-6 row on the same menu: `...vs-ai_baka_deck162.jsonl`
seq 5 (over Wall of Omens, t4) and `...deck126-0x55ab47d01810-vs-ai_baka_deck146.jsonl` seq 9
(over Pride Guardian, t7 - the Guardian was cast in the next window, so no cost). Second Lantern
with one on the battlefield line: `...deck126-0x55a0f7b2ff90-vs-ai_baka_deck130.jsonl` seq 34;
`...deck126-0x562af4e53780-vs-ai_baka_deck125.jsonl` seq 73, 78, 100 (a game where the seat had
nothing else to do for 45 turns). D38: 0/2 on the first half, 4 on the second. Violated-unpunished,
second corpus; the rule is unchanged, the evidence is written in and the "or cast nothing" exit is
made explicit.
Before: "... A SECOND Lantern does nothing at any time - once one is there this entry is finished
for the rest of the game, so take a wall or a combo piece instead. You have cast a second one with
the first still in play."
After: "... - you cast a Lantern over a Wall of Omens and another over a Pride Guardian this
corpus, both walls one entry higher. A SECOND Lantern does nothing at any time - once "Chromatic
Lantern" is on your battlefield line this entry is finished for the rest of the game, so take a
wall or a combo piece instead, or cast nothing. You cast four second Lanterns this corpus with
the first still in play."

## F14 - STEP 1 re-keyed to the printed parenthesis (third corpus of the same shape, #118)
Finding: `...vs-ai_baka_deck146.jsonl` seq 11 (t8, `you would be at 18`, no enchantment out):
B-row `Pride Guardian (0/3) [defender] ... may block A1 (your blocker dies, attacker lives)`,
reply `BLOCKS: B1:A1` "to gain 3 life and stop Nadaar's damage". The Guardian died; the seat had
no blocker for the next two combats (24 -> 19 -> 9) and lost at t14. Wave 48 recorded the identical
shape (`vs146` seq 11, Pride Guardian on a dies line) and did not re-word; the guide's own
anecdote is Pride Guardian on Nadaar. Third corpus -> shrink and re-key to the string.
Before: " - If EVERY attacker offered to that blocker says "your blocker dies", leave that blocker
OUT - unless an enchantment of yours is on the battlefield, or the combat header's "you would be
at N" number is 8 or less, or the blocker is a lifelink Vampire. ... You blocked a Triumphant
Adventurer (1/1, first strike, deathtouch) with Perimeter Captain at 20 life ..."
After: " - A B-row whose EVERY parenthesis reads "your blocker dies, attacker lives" stays OUT of
your BLOCKS line - "BLOCKS: none" when it is your only blocker - unless one of three things is
printed: an enchantment of yours on your battlefield line, the combat header's "you would be at
N" at 8 or less, or the blocker is a lifelink Vampire. ... Three corpora running you have put
Pride Guardian on a "your blocker dies" attacker at 18 to 21 life with no enchantment out - this
time on Nadaar, Selfless Paladin at "you would be at 18": you gained 3, the wall died, and with no
blocker left the next two combats took you from 24 to 9. A wall that lives gains that 3 in every
combat after; a wall that dies gains it once. ..." (the Adventurer anecdote dropped; the Vampire
"BLOCKS: none" anecdote kept).

## F15 - MULLIGAN section rewritten to the odds form (skill #119; the live text carried floors)
Finding: the live section still read "KEEP whenever N is 2 or more", "At "(keeping 5)" or lower a
hand with even ONE land is a keep", "If bottoming a land would leave you with fewer than THREE, no
land on that list is an answer - cross them all off" - three floors / forced keeps, the shape the
owner ruled out and #119 supersedes. This corpus's real hands: 3 mulligans taken, all single, on
a 0-land seven (`...vs-ai_baka_deck123.jsonl` seq 1), and two 1-land sevens (`...vs-ai_baka_deck152.jsonl`
seq 1, `...vs-ai_baka_deck146.jsonl` seq 1); the sixes kept at 3, 3, 5 lands; three 7-card keeps
at 5, 3, 3 lands. Bottoming: Chromatic Lantern (vs152), Sunpetal Grove from a five-land six
(vs146), Sunpetal Grove from a three-land six (vs123, keeping Chapel + Savannah + Sorin + Guardian
+ Wall + Blood - a two-land keep the live floor forbade, and the game was won at t24). Deck facts
read off `deck126.txt`: 24 lands (Bayou 4, Scrubland 4, Savannah 4, Sunpetal Grove 4, Isolated
Chapel 2, Woodland Cemetery 2, Urborg, Plains, Swamp, Forest), sixteen walls, two one-mana
spells (Perimeter Captain {W}, Pride Guardian {W}).
Before: the whole "=== MULLIGAN ===" section (floors as quoted) and the two SITUATIONS bullets
("(keeping 5) or fewer and the hand holds a land: keep it"; the bottom order).
After: odds framing (24-in-60, two-colour lands, one-mana walls, the enchantment's two black pips);
the three engine-printed numbers named; per-look branches - "Two or more lands and a coverage line
that names even one card: KEEP, at any price"; "Zero lands, or one land ... ship it on a seven and
on a six ... At a row reading "would keep 5 cards" or smaller the next look costs more than the
wait does: keep a one-land hand there if the coverage line names a card, and ship only a hand with
no land at all"; "Seven lands and no spell ships at any price; six lands and one spell is a keep if
the spell is a wall or the Tutor". Bottoming = a PREFERENCE ORDER for the hand you keep (lands
covering the cheapest spells, {B} sources first; one combo piece or Tutor; cheapest wall; second
piece; rest by cost) and a send order (duplicate wall, Lantern or Staff, most expensive spell, a
land only when lands exceed what the kept spells use; a combo piece last). Satisfiable at every N.

## Recorded, NOT edited
- Tribute at "of which 1 is a creature": 4/4 taken in the first window (`vs130` seq 6 Dwarven
  Blastminer, `vs146` seq 7 Triumphant Adventurer, `vs123` seq 54 Thraben Doomsayer, `vs125` seq
  104 Emrakul - the last one the win at t60). D36 PASS; 0 "if it becomes" replies. F5 held.
- Tribute at 0: 0 casts in ~120 windows (every row now prints `at 0 this does nothing`). D37 PASS.
- The Tribute that resolved with NO sacrifice (`vs146` seq 13, opponent controlling Nadaar and a
  Goblin token) is an ENGINE event - seat file HIGH #1 - not a guide matter.
- `vs123` seq 27 `ATTACK: A1, A2` with one A-row offered: the engine trimmed A2 (R44); the attack
  went through. No guide text.
