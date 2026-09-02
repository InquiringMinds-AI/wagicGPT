# deck152 - wave-52 edits (before -> after), each with its finding

Corpus `matchups-20260901-223430/`, deck152 seats (6 logs, 612 records, 2 fallbacks).
Record **1-5**. Hands are real and two of the five losses are matchup losses with no window
(see the seat file); no rate is attributed to any edit below. Five edits.

---

## 152-I - re-key #2a to the row's own `{paying this taps: ...}` clause
**Finding, the deck's most expensive lane this corpus.** The engine now prints the bill on the
option itself. deck152 met 10 such rows and TOOK three, all in `152v162` (L 0/5 T16):
- s11 (T11, **Main 1**) `Cast Luminarch Aspirant ... {paying this taps: Katilda, Dawnhart Prime
  - it cannot attack this turn}` -> taken; s14 attacked with **one** creature.
- s19 (T13, **Main 1**) `Cast Luminarch Aspirant ... {paying this taps: Luminarch Aspirant,
  Luminarch Aspirant - they cannot attack this turn}` -> taken; s23 attacked with **one**.
- s27 (T15, **UPKEEP**) `put 1/1 counters with Katilda, Dawnhart Prime [cost: {4}{g}{w}, Tap]
  {paying this taps: Luminarch Aspirant, Luminarch Aspirant, Luminarch Aspirant - they cannot
  attack this turn}` -> taken. Four bodies tapped before combat; s33 sent Elite Spellbinder alone.
The guide's #2a keys on the mana line's "Those sources, one per untapped card:" list. The clause
on the row is one step closer to the decision and names the creatures outright.

**Before**
```
- YOUR HUMANS ARE MANA SOURCES. Katilda gives every Human "{T}: add mana", so once your lands are
  tapped the mana line's "Those sources, one per untapped card:" list names your creatures. ...
  Before any Main-1 cast, read that list: if it names a creature you are about to attack
  with, do not cast - the row's "casting this taps you out" means it taps THEM.
```
**After**
```
- YOUR HUMANS ARE MANA SOURCES, AND THE ROW NOW TELLS YOU WHICH ONES IT WILL TAP. ... it prints
  the bill on the option itself: "{paying this taps: Katilda, Dawnhart Prime, Luminarch
  Aspirant, Luminarch Aspirant - they cannot attack this turn}". THAT CLAUSE IS AN
  UPKEEP-AND-MAIN-1 STOP. ... In Upkeep or Main 1 a row carrying that clause is PASS: attack
  first, then take the same row in Main 2, when the clause costs you nothing. ... when they
  disagree, the clause printed on the row you are taking is the one that is about THIS cast.
  WHAT THIS COST, three takes in one game: [the three above]
```
Plus a DECIDING SITUATIONS line.

---

## 152-J - ONE converter name: an unblockable A-line is the race, not a risk
**Finding.** `152v126` (L 0/19 T24). Their line carried **Sanguine Bond only** - Exquisite Blood
was never in play, verified against every prompt in that log. Sanguine Bond's primitive is
`auto=@lifeof(player) ... :life:-thatmuch opponent` / *"Whenever you gain life, target opponent
loses that much life"*: it fires on life THEY gain, so an attacker nothing can block gains them
nothing and the converter has nothing to convert. The seat answered `ATTACK: none` across two
windows holding **five** A-lines every one of which read `[no creature they control can block
this attacker]`:
- s24 (T11, 20/28): Briarbridge Tracker + Brutal Cathar #1. Reply: *"Do not attack due to
  Sanguine Bond ... consider if any evasion exists (currently none)"*.
- s45 (T17, 12/32): Brutal Cathar #1, Brutal Cathar #2, Katilda - same reply shape.
(s51 T19 declined again, correctly there: those tags DID print blocking triggers.) Then at s61
(T21, 8 life) and s67 (T23, 4 life) it sent the **priced** attackers into Perimeter Captain and
Pride Guardian and lost. Across the corpus deck152 sent 14 of 19 empty-tag A-lines; all five
misses are these two windows.

**Before**: `With ONE of the two names printed, the race is real: race it with your EVASION, not
with your whole board.`
**After**: same, plus `AND AN A-LINE READING "[no creature they control can block this attacker]"
IS THAT RACE. ... SEND every creature carrying that line, every combat, at every life total.
Only the BOTH-names board makes an unblockable attacker fatal ...; do not carry the both-names
sentence onto a one-name screen. Check their battlefield line for the SECOND name before you
write ATTACK: none.` + the WHAT THIS COST above. Plus a DECIDING SITUATIONS line.

---

## 152-K - #3 RULE deploy floor: the `[legendary:` second copy is not a body
**Finding.** The deploy floor registered 10 "breaks" this corpus and **nine** were the same
shape: a menu whose only creature row read `Cast Katilda, Dawnhart Prime {g}{w} (1/1) [legendary:
you already control Katilda, Dawnhart Prime - legend rule: casting this sends one copy to your
graveyard]` (`152v126` s32/s35/s46/s52/s54/s59/s65/s68, `152v130` s16). Declining was correct
every time; the guide's absolute was calling correct play a break. (The tenth, `152v130` s7, is
a real break and needs no new text: T5, empty board both sides, `Cast Brutal Cathar` declined
against a carried plan naming an uncastable Katilda - #3 RULE already covers it.)

**Before**: `"Cast nothing right now" on YOUR main phase is correct in exactly TWO situations:`
(a) Fateful Absence held; (b) lethal already on board.
**After**: `... exactly THREE situations:` (a), (b), and
```
  (c) every creature row on the menu is printed "[legendary: you already control <name> - legend
      rule: casting this sends one copy to your graveyard]" and the copy you control is healthy.
      That row is not a body: you pay the mana, both copies exist for an instant, and one dies.
      It becomes a real cast only when the copy on your line is about to die anyway or is worse
      than the fresh one ... Nine windows in one corpus offered a second Katilda under that tag
      with nothing else on the menu; declining was right every time.
```

---

## 152-L - the two block-window absolutes that broke
**Finding A - one blocker, one attacker (2 breaks, both re-asked).**
`152v146` s76 (T27, 1 life, LETHAL): `BLOCKS: B1:A1, B2:A4, B1:A5` -> `multiblock_reask`;
`152v162` s26 (T14): `BLOCKS: B2:A1, B2:A2` with the plan *"Elite Spellbinder blocks both Fate
Unravelers"* -> `multiblock_reask`. Both recovered on the second ask. The rule exists in the
guide; what it lacked was the counting instruction and the note that the OTHER direction (two
blockers on one attacker) is legal - `152v146` s76's own A4 row says
`no 1-on-1 result exists - only a block by TWO OR MORE of your creatures counts`.
**Finding B - the N<=9 block floor (2 breaks).** `152v146` s43 (T19, 8 life, header
`deal up to 2 - you would be at 6`, three rung-3 blocks offered) -> `no blockers`; s68 (T25,
2 life, `you would be at 1`, one rung-3 block offered) -> `no blockers`. Both are inside the
guide's own "N of 9 or less means you are blocking". The tell in both: the incoming damage was
small (2 and 1), so the *attack* looked ignorable while N did not.

**Before**: `- One blocker can be assigned to AT MOST ONE attacker. "B1:A1, B1:A4" is illegal;
pick one.` / `... AND the header's "you would be at N" number is 10 or more.`
**After**: the same lines plus `COUNT THE LEFT-HAND SIDES BEFORE YOU SEND THE LINE: each B-number
appears at most once in the whole answer... THE OTHER DIRECTION IS LEGAL AND SOMETIMES REQUIRED
...` and `THE TEST IS THE HEADER'S N, NOT THE SIZE OF THE ATTACK. "Unblocked, these attackers
deal up to 2 - you would be at 6" is N of 6, so you are blocking ...`. Plus two DECIDING
SITUATIONS lines.

---

## 152-M - first strike does not cancel a blocking trigger
**Finding.** `152v126` s61 (T21, 8 life) reply: *"if it blocks, it dies to first strike before
triggering, or if it blocks and triggers, we lose 2 life"*. Perimeter Captain's primitive is
`auto=@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller` - the
trigger is on BLOCKING, not on surviving damage - and the A-line already printed the priced
first-strike outcome `(you kill it, your attacker lives (blocking trigger: they may gain 2, and
if they do their converter takes 2 off you))`. Three first-strikers were sent on that reasoning.

**Before/After**: one paragraph added under the `blocking trigger:` section (FIRST STRIKE DOES
NOT CANCEL A BLOCKING TRIGGER ...), plus a DECIDING SITUATIONS line.

---

## Considered and NOT made
- **The T39-T54 no-attack stretch in `152v125`** (L 0/28 T54). Not a lane: the battlefield line
  read "of which 0 are creatures" in every one of those windows - Supreme Verdict / Final
  Judgment wiped the board and Essence Scatter / Fall of the Gavel countered every replacement
  (s337, s338). Real hands, real matchup.
- **The T21-T35 one-Wolf attacks in the same game**: every one of those A-lines was the deck's
  only untapped body and read `[no creature they control can block this attacker]`; it was sent
  each time. Correct.
- Katilda A-row / B-row rules, Fateful-Absence-own, Spellbinder naming: 0 breaks (see seat file).
