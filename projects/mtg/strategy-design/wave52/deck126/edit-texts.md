# wave-52 deck126 guide edits (before -> after, each with its finding)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` at corpus time (wave-51 revisions 5d0c24d28 + boundary pass 6686392d3).
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260901-223430/`. 10 edits, every one cited to a seq.

## D65 [second copy: literal for Lantern (was [already owned:) + true reason

**Finding.** GUIDE-LITERAL DRIFT (review-carry obligation). The retired `[already owned: Chromatic Lantern on your battlefield]` never appears in this corpus (0 occurrences in 42 logs); the emitter now prints `[second copy: you already control Chromatic Lantern; both stay on the battlefield - no legend rule]`. deck126 met the new literal on 25 option rows and TOOK 9 of them, 5 of those Chromatic Lantern #2 (`vs162` seq 10, `vs123` seq 41, `vs125` seq 39 and seq 41, `vs152` seq 40) - against 9/9 declines under the old literal in wave 51. The rule went blind when its recognition string changed. Card fact re-checked against `mtg.txt:19630`: Chromatic Lantern has its own `{T}:Add{G}/{R}/{U}/{B}/{W}`, so the wave-51 sentence 'a SECOND Lantern does nothing at any time' was FALSE - the lord clause does not stack but the copy is still a {3} rock; the guide now says that instead.

**BEFORE**
```
    game; the row now prints "[already owned: Chromatic Lantern on your battlefield]" - nine such
    rows this corpus, nine declined. Read the line, not the memory of it.
```

**AFTER**
```
    game; the row now prints "[second copy: you already control Chromatic Lantern; both stay on
    the battlefield - no legend rule]". THAT TAG IS NOT PERMISSION. It answers a rules question you
    did not ask (whether the copy survives) and says nothing about whether the copy is worth three
    mana and a turn. Here it is not: the part of the Lantern that matters - "Lands you control have
    {T}: Add one mana of any color" - is already switched on by the first one and does not stack, so
    the second copy is a bare {3} mana rock, and generic mana is the one thing this deck is never
    short of (a single Overgrown Battlement makes {G} for every defender you control). The corpus
    before, this row printed a different tag and you declined it nine times out of nine; under the
    new wording you took it five times out of five, at 3 to 8 spare sources, in games you lost.
    Read the NAME on your battlefield line: once "Chromatic Lantern" is printed there, this entry is
    finished for the rest of the game whatever tag the row carries.
```

## D66 [second copy: on an enchantment is not entry 1

**Finding.** `vs146` seq 12 (t14, 11 life): `Cast Exquisite Blood {4}{b} [second copy: ...] {leaves 0 ... taps you out}` taken over `Cast Tribute to Hunger {right now: they control 2 creatures - they choose which one} {leaves 2}`; dead on t17. Rule #7 entry 1 already said to skip a copy of a name on the battlefield line, but the new tag reads as permission and the reply's own PLAN asserted 'Both combo pieces are now on the battlefield' with Sanguine Bond exiled two turns earlier.

**BEFORE**
```
 1. Exquisite Blood or Sanguine Bond - whichever NAME is missing from your battlefield line, at any
    "leaves N", including "casting this taps you out". A copy of a name already on that line is
    not this entry; skip it. Nine of nine this corpus - the turn the row appeared, it was cast.
```

**AFTER**
```
 1. Exquisite Blood or Sanguine Bond - whichever NAME is missing from your battlefield line, at any
    "leaves N", including "casting this taps you out". A copy of a name already on that line is
    not this entry; skip it. Nine of nine one corpus - the turn the row appeared, it was cast.
    THE ROW SAYS WHICH IT IS. A second copy prints "[second copy: you already control Exquisite
    Blood; both stay on the battlefield - no legend rule]" - that tag means the copy is legal, NOT
    that it is worth casting; a second Blood or a second Bond changes nothing about who is winning.
    At 11 life with that tag on the row you tapped out for a second Exquisite Blood while "Cast
    Tribute to Hunger {right now: they control 2 creatures}" sat two rows below it at "leaves 2",
    and you were dead three turns later. When the tag is printed, this entry has NOT matched -
    go down the list.
```

## D67 Exquisite Blood triggers on THEIR life loss only

**Finding.** `vs125` seq 152/185/200/215/216 (t30-t38, 9 -> 1 life): both enchantments on the battlefield line since t22, and the PLAN carried into every window read 'The opponent's Staff of Nin pings in their upcoming upkeep will deal 1 damage to me (triggering Exquisite Blood), causing me to gain 1 life'. Primitive `mtg.txt:37966`: `auto=@lifelostfoeof(player):life:thatmuch controller` - the trigger is the OPPONENT losing life; damage to the seat fires nothing. The seat passed 5 windows on that false sentence and lost 0/25 at t39.

**BEFORE**
```
 - EXQUISITE BLOOD ALONE DOES NOTHING you can start yourself: it only pays you when the opponent
   is ALREADY losing life. Against an opponent with no creatures and no burn it is a blank.
```

**AFTER**
```
 - EXQUISITE BLOOD ALONE DOES NOTHING you can start yourself: it only pays you when the opponent
   is ALREADY losing life. Against an opponent with no creatures and no burn it is a blank.
   AND IT IS THE OPPONENT'S LIFE THAT STARTS IT, NEVER YOURS. Its line reads "Whenever an OPPONENT
   loses life, you gain that much life". Damage dealt to YOU triggers nothing at all. The thought
   "their Staff of Nin will ping me, that triggers Exquisite Blood, I gain 1, Sanguine Bond takes 1
   off them, and the loop wins on its own" is FALSE, and it is the most expensive false sentence
   this deck has ever written: with both enchantments on your battlefield from turn 22 you answered
   "Cast nothing right now" in every window from 9 life down to 1 while their two Staffs pinged you
   out, and lost that game at 0 against their 25.
```

## D68 both-out with no starter: keep casting, Wall of Omens digs

**Finding.** Same game. From t30 the opponent's battlefield line held 0 creatures (Tribute reads 'at 0 this does nothing', no block is possible), the seat's only Staff of Nin had been countered at seq 168 (`Opponent cast Fall of the Gavel`), and it controlled no Sorin and no Vampire - so none of Rule #2's five starters existed. `Cast Wall of Omens` (draws a card, `mtg.txt:131252`) was on the menu at seq 200/215/216 and was declined each time.

**BEFORE**
```
Any one of these wins from any life total, yours or theirs. You do not need a big number - 1 life is
enough, because the loop runs itself until the opponent is at 0. Do not wait for a better turn, do
not wait for their attack, and do not build a bigger board first: a wall cast with the win on the
menu is a turn given away.
```

**AFTER**
```
Any one of these wins from any life total, yours or theirs. You do not need a big number - 1 life is
enough, because the loop runs itself until the opponent is at 0. Do not wait for a better turn, do
not wait for their attack, and do not build a bigger board first: a wall cast with the win on the
menu is a turn given away.
BUT THE LOOP NEEDS A STARTER, AND THE STARTER IS ALWAYS A CARD OF YOURS. Every entry above is
something YOU do: a Tribute, a block, a Staff ping, a lifelink attack. Nothing the opponent does
starts it - not their attack, not their burn, and not their Staff of Nin pinging you.
So when both names are on your battlefield line and NONE of the five entries is available - their
battlefield line shows no creature (so no Tribute and no block), and you control no Staff of Nin,
no Sorin and no Vampire - you are not winning by passing, you are waiting to die. CAST SOMETHING
EVERY TURN in that state, in this order: Staff of Nin, Sorin, then Wall of Omens (it draws you a
card, which is the only digging this deck does), then any other wall. "Cast nothing right now" is
the wrong answer to five windows in a row with a Wall of Omens on the menu; that is exactly what
you answered at 9, 7, 5, 3 and 1 life in the game above.
```

## D69 read {paying this taps: <wall>} as 'cannot block on their turn'

**Finding.** 91 rows carrying `{paying this taps: Overgrown Battlement - it cannot attack this turn}` across my 6 deck126 logs, 7 taken (`vs162` seq 9, `vs123` seq 12 and 47, `vs125` seq 9/86/169, `vs130` seq 13). Every creature in this deck is a Defender, so the clause's stated consequence is vacuous while its real one (the wall is tapped through the opponent's turn) is unstated. Rule #1 already teaches the cost for the upkeep-tap window; this extends it to the cast rows where it now actually appears. The emitter wording itself is filed as an engine item, not taught as doubt.

**BEFORE**
```
Taking that upkeep option instead does two bad things at once: the green mana is gone
before your main phase ever starts, and a tapped Battlement is a 0/4 wall that cannot block on the
opponent's next turn - a wall you needed.
```

**AFTER**
```
Taking that upkeep option instead does two bad things at once: the green mana is gone
before your main phase ever starts, and a tapped Battlement is a 0/4 wall that cannot block on the
opponent's next turn - a wall you needed.
THE CAST ROWS NAME THE SAME COST, AND YOU HAVE TO TRANSLATE IT. A cast row can carry
"{paying this taps: Overgrown Battlement - it cannot attack this turn}". Every creature in this
deck is a Defender and none of them could ever attack, so the words "cannot attack" cost you
nothing; what that clause is telling you is WHICH OF YOUR WALLS WILL BE TAPPED when you take this
row, and a tapped wall does not block on their next turn. Read it as "this block is not available
next turn" and weigh it against what the spell buys. Ninety-one such rows this corpus; the seven
you took each cost one wall for one combat, and each was worth it because the spell was a Sanguine
Bond, a Sorin or a Tribute. It would not be worth it for a wall or a Chromatic Lantern.
```

## D70 <=8 carve-out is about unstopped damage

**Finding.** `vs162` seq 21 (t12, header `you would be at 7`): one attacker (`A1. Fate Unraveler (3/4) deals 3`), two Overgrown Battlements both reading `(neither dies)` assigned, three lifelink Vampires reading `(your blocker dies, attacker lives (lifelink: you gain 1))` left out. As written, 'at 8 or less everything blocks' made that a break; in substance it was right, because the two walls already stopped all 3. The rule is narrowed rather than the play corrected.

**BEFORE**
```
Once either enchantment is on your battlefield,
every blocker blocks, on any line at all, and so it does whenever the header's "you would be at N"
number is 8 or less - at that life total the damage a body stops is worth more than the body.
```

**AFTER**
```
Once either enchantment is on your battlefield,
every blocker blocks, on any line at all, and so it does whenever the header's "you would be at N"
number is 8 or less - at that life total the damage a body stops is worth more than the body.
ONE LIMIT ON THAT CARVE-OUT, AND ONLY THIS ONE: it is about damage that is still coming through.
When every attacker on the A-lines is already blocked by a blocker whose parenthesis says it lives,
no damage is left to stop, and a further blocker whose line reads "your blocker dies" only feeds
them a body. At "you would be at 7" with one attacker and two 0/4 Battlements already on it
("neither dies"), you correctly left three 1/1 lifelink Vampires out - that is this limit, and it
is right whenever no enchantment of yours is out. With an enchantment of yours out the Vampires
block anyway: 1 life each is the loop.
```

## D71 the short name outranks the index

**Finding.** `vs152` seq 45 (t18): reply `CHOICE: 3 (Play no land right now)` on a menu whose row 3 was `Play Sunpetal Grove` and row 4 `Play no land right now`; `parse_note: echo_index_conflict;name_over_index` and the NAMED row executed. Lane J worked as specified; the seat's only missed land drop in 405 decisions came from the name, not the number.

**BEFORE**
```
Your CHOICE names a numbered row, never a card from the hand line.
```

**AFTER**
```
Your CHOICE names a numbered row, never a card from the hand line.
AND THE NAME YOU WRITE IN THE PARENTHESES IS WHAT THE ENGINE EXECUTES when it disagrees with the
number in front of it. Copy the name off the row you actually mean, character for character. You
wrote "CHOICE: 3 (Play no land right now)" on a land-drop menu whose row 3 was "Play Sunpetal
Grove" and whose row 4 was "Play no land right now"; the NAME won, and Rule #0's land drop - the
one rule in this guide that has no exceptions - was skipped that turn.
```

## D72 mulligan sentence for a 3+-land no-coverage seven

**Finding.** `vs146` seq 1: a 4-land 3-spell seven, `Mana sources ... {W} 2, {B} 3, {G} 2`, `Playing every land in this hand would not cover any spell in it`. The section had no sentence for a three-or-more-land no-coverage hand (#131 gap). The model never answered it - the ask timed out at 900.0 s with an empty reply and the fallback kept the hand - so this is a completeness fix, NOT a punished break, and no win/loss is attributed to it.

**BEFORE**
```
  - Two lands and a coverage line reading "would not cover any spell in it" (two lands that make
    no {W} under a hand of {W} walls): on a seven, KEEP
```

**AFTER**
```
  - THREE OR MORE lands and a coverage line reading "would not cover any spell in it" (five-drop
    enchantments and a Staff of Nin over lands that are still one short): KEEP on a seven, at any
    price. The lands are there, the hand casts its first spell the turn the fourth or fifth land
    lands, and no smaller hand fixes that faster. This shape reached you once this corpus and the
    engine answered it for you.
  - Two lands and a coverage line reading "would not cover any spell in it" (two lands that make
    no {W} under a hand of {W} walls): on a seven, KEEP
```

## D73 situations-list re-key of the retired [already owned: literal

**Finding.** Same drift as D65; the DECIDING SITUATIONS list carried the second occurrence of the retired literal.

**BEFORE**
```
 - "Cast Chromatic Lantern" is in your menu and no Chromatic Lantern is on your battlefield line:
   cast it when nothing from entries 1-6 of Rule #7 is beside it - or when a wall is beside it but
   Sanguine Bond, Exquisite Blood or Idyllic Tutor is on your hand line and "Mana available:" reads
   4 or less. A combo piece castable on the same menu, or a Lantern already out ("[already owned:"
   on its row): take the piece, or the wall.
```

**AFTER**
```
 - "Cast Chromatic Lantern" is in your menu and no Chromatic Lantern is on your battlefield line:
   cast it when nothing from entries 1-6 of Rule #7 is beside it - or when a wall is beside it but
   Sanguine Bond, Exquisite Blood or Idyllic Tutor is on your hand line and "Mana available:" reads
   4 or less. A combo piece castable on the same menu, or a Lantern already out (its row reads
   "[second copy: you already control Chromatic Lantern; both stay on the battlefield - no legend
   rule]"): take the piece, or the wall. That tag says the copy is LEGAL, never that it is worth
   casting - a second Lantern is a bare {3} mana rock and this deck is never short of generic mana.
```

## D74 situations-list entry for both-out with no starter

**Finding.** The situations list is the layer the pilot reads under time pressure; D67/D68's rule needed its own row there. Same citation as D68.

**BEFORE**
```
 - Both Exquisite Blood and Sanguine Bond on your battlefield: take the first of these the list
   offers - "Cast Tribute to Hunger" at "they control 1" or more creatures (a wall of theirs
   counts), a block, a Staff ping, a lifelink attack. You win this turn; a wall cast instead is a
   turn given away.
```

**AFTER**
```
 - Both Exquisite Blood and Sanguine Bond on your battlefield: take the first of these the list
   offers - "Cast Tribute to Hunger" at "they control 1" or more creatures (a wall of theirs
   counts), a block, a Staff ping, a lifelink attack. You win this turn; a wall cast instead is a
   turn given away.
 - Both on your battlefield and NONE of those four available (their line shows no creature, and you
   control no Staff of Nin, no Sorin and no Vampire): the loop has no starter and nothing the
   opponent does will start it. Cast Staff of Nin, else Sorin, else Wall of Omens, else any wall -
   every turn, until one of the four appears. Do not answer "Cast nothing right now" here.
```
