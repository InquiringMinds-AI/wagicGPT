# deck152 - wave-48 edits (4), before -> after, each with its finding

Baseline: the LIVE guide `bin/Res/ai/baka/deck152_strategy.txt` (36,855 bytes = wave-47 reviewer
revision + boundary-pass B7). After: `wave48/deck152/strategy.txt`, 41,973 bytes (**+5,118**, of
which ~2,050 is a DECIDING SITUATIONS list this guide never had - amendment 24's contract, every
row a restatement of a rule the guide already carries; the rest is the four edits). 0 deletions.
ASCII only. Corpus `matchups-20260827-072004`. Card facts re-verified with `/usr/bin/grep` against
`bin/Res/sets/primitives/*.txt`: `Exquisite Blood` = `auto=@lifelostfoeof(player):life:thatmuch
controller`, `type=Enchantment`; `Sanguine Bond` = `auto=@lifeof(player) from(*[-lifefaker]|*):
life:-thatmuch opponent`; `Katilda, Dawnhart Prime` = `auto=lord(creature[human&white]|
myBattlefield) {T}:add{W}` (one line per colour) and `auto={4}{G}{W}{T}:name(Put 1/1 counters)
all(creature|mybattlefield) counter(1/1)`; `Elite Spellbinder` ETB = `target(*[-land]|opponenthand)
moveto(opponentexile)`. The sixty were checked for enchantment removal: none.

---

## 152-A (HIGH) - the converter PAIR is a loop, and the guide told the seat to race it
**Finding:** `152 vs126` attackers s25 (T14, life 20 vs 14). Their battlefield line printed
`Exquisite Blood {4}{b} [enchantment] {effect: "Whenever an opponent loses life, you gain that
much life."}` AND `Sanguine Bond {3}{b}{b} [enchantment] {effect: "Whenever you gain life, target
opponent loses that much life."}`; the converter block named Sanguine Bond only. s20 and s23 had
correctly held every `(blocking trigger:` ground attacker. s25's PLAN reads *"Opponent has
Sanguine Bond and Exquisite Blood; any life gain or damage they take from my attacks will heal
them or hurt me. I must bypass their walls ... attack with my flying creatures"* - the guide's
MATCHUP POSTURE sentence "Race it with your EVASION". `ATTACK: A1` (Elite Spellbinder 7/5 flying,
`[no creature they control can block this attacker]`). Seven damage -> Blood +7 -> Bond -7 ->
Blood +7 ... `gameend` **L0 / 32** the same combat. The deck has no enchantment removal.
CLASSIFICATION: **PERCEPTION** for the block (R12, carried; now E-5 with a lost game) **and
STRATEGY** - both names were on the line and the guide's own posture rule sent the flier.
- **before:** `... the game has become a race with a deadline you cannot see. Race it with your
  EVASION, not with your whole board.`
- **after:** `FIRST, READ THEIR BATTLEFIELD LINE FOR THE SECOND NAME.` Both names -> a LOOP
  (stated in one sentence each direction), `ATTACK: none, every combat, fliers included - "no
  creature they control can block this attacker" is exactly why it kills you`; block every
  `[lifelink]` attacker; cast the hand as blockers; WHAT THIS COST. ONE name -> the evasion race
  as before. DECIDING row added.
- **Prediction:** `ATTACK:` other than `none` with both names on their line: 0 (was 1, fatal).
  Life lost in one combat step with both names printed: 0 (was 20).

## 152-B (HIGH) - Elite Spellbinder took Sorin and left Exquisite Blood in hand
**Finding:** `152 vs126` ask s13 (T6): hand look `1. Staff of Nin / 2. Exquisite Blood / 3. Sorin
/ 4. Chromatic Lantern`; reply `CHOICE: 3 (Sorin)`. Their line then had no Bond; Exquisite Blood
resolved (s20's narration), Idyllic Tutor fetched Sanguine Bond (s25's narration), and 152-A's
combat followed. Spellbinder's exile is the only enchantment answer in the sixty and it works
before the card is cast. CLASSIFICATION: **STRATEGY** (the row printed the card's text).
- **before:** no Spellbinder card note.
- **after:** `ELITE SPELLBINDER ... When the list shows "Exquisite Blood" or "Sanguine Bond",
  exile THAT - above a planeswalker, above a creature, above anything.` + WHAT THIS COST. DECIDING
  row added.
- **Prediction:** hand-look answers leaving a loop half in their hand when one is offered: 0
  (was 1 of 1).

## 152-C (HIGH) - lethal on board, spent in Upkeep and Main 1; the mana sources were the attackers
**Finding:** `152 vs162` s22-s26 (T12, life 12 vs 14, their line `of which 0 are creatures`).
s22 Upkeep: `put 1/1 counters with Katilda [cost: {4}{g}{w}, Tap]` taken, six of ten sources.
s23: `Mana available: 2 ... Those sources, one per untapped card: Intrepid Adversary {w}; Brutal
Cathar {w}`; battlefield Katilda (4/4) `[tapped]`, Adversary (6/4), Sigarda (7/7) flying, Cathar
(5/5), Aspirant (3/3) - 25 power untapped against 14 with nothing to block. s24: `Cast Briarbridge
Tracker {2}{g} {leaves 0 of your 3 untapped mana sources untapped - casting this taps you out}`
taken; s25 battlefield: Adversary and Cathar `[tapped - cannot attack or block this turn]`,
`2 of them able to attack`. s26 `ATTACK: A1, A2` for 11; opponent 14 -> 3; next draw step 5 cards
x 2 punishers, `gameend` L0 / 3. Same shape at `152 vs130` s20-s22 (18 power vs 13, Katilda in
Upkeep, only Sigarda attacked; won two turns later). The guide's Katilda entry said "worth a whole
turn's mana only at three-plus creatures" with no phase and no word about Humans paying.
CLASSIFICATION: **STRATEGY** (the `Those sources` list names the creatures) + a render half (E-3:
the row says `taps you out`, not `taps Intrepid Adversary and Brutal Cathar`).
- **before:** no lethal-count rule; Katilda card entry with the "whole turn's mana" clause.
- **after:** new `#2a RULE - BEFORE YOU SPEND ANYTHING IN UPKEEP OR MAIN 1, RUN THE LETHAL COUNT`
  (sum of power on entries with no `[tapped` / `[summoning sick`, compared to their printed life,
  gated on their line reading `of which 0 are creatures`; outranks #3 for that turn), `YOUR HUMANS
  ARE MANA SOURCES` keyed to the `Those sources, one per untapped card:` list, and `KATILDA'S ...
  IS A MAIN-2 ACTION`; the card entry now points at #2a (amendment 96). Three DECIDING rows.
- **Prediction:** Katilda's ability taken in Upkeep or Main 1: 0 (was 8 of 9). Main-1 casts whose
  `Those sources` list names a creature while untapped power >= their life and their line reads
  0 creatures: 0 (was 2 windows, one fatal).

## 152-D (LOW) - a Clue cracked at 8 life under two punishers
**Finding:** `152 vs162` priority s29 (T14, life 8): `DRAW PUNISHERS on the battlefield: theirs -
Underworld Dreams, Ob Nixilis ... Every card YOU draw costs you 2 life`; row `1. Draw 1 with Clue
[cost: {2}, Sacrifice]` with no `[DRAW PRICE:` tag (E-4); `CHOICE: 1`. Dead on the draw step
regardless (5 draws x 2), so cost 0 here; the rule is one DECIDING row and its evidence is the
window. CLASSIFICATION: **STRATEGY** (the summary line printed the cost).
- **after:** DECIDING row `A "DRAW PUNISHERS" line is printed, a "Draw 1 with Clue" row is
  offered, and your printed life is 10 or less -> pass`.
- **Prediction:** Clue cracked with a punisher line printed at life <= 10: 0 (was 1).

---

## NOT edited, and why
- **Counter spreading (152-D wave 46): 2 of 13**, third corpus at that rate (`vs123` s24, `vs162`
  s25, both Sigarda at 6/6+ with others listed). Amendments 45/64/65: measured, carried; if it is
  2/N a fourth time the rule is inert and goes to the row (a `{already 6/6}` tag).
- **STOP tag on a card (`vs123` s15 Aspirant)**: the A-line also read `[held back, it CANNOT
  block ANY of their 1 creatures: Bloodline Keeper (flying)]`, which removes the stop's cost
  argument (it could not block anyway); sent, unblocked, won. n=1, recorded.
- **Deploy floor 35/35, Fateful Absence own-only 14/14, land drops 131/131, no-blocker rows
  24/24, converter posture 2/3 windows** - held.
- **RULE -1 / MULLIGAN:** 0 mulligans, 6 keeps at 2-4 lands; untouched.
- **Block floor:** 0 windows at `you would be at` <= 9; untouched.
