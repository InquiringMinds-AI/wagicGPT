# deck146 - wave-57 edits (before -> after), each with its finding

Base = the live guide `bin/Res/ai/baka/deck146_strategy.txt` (67,603 bytes, 779 lines).
Result = `wave57/deck146/strategy.txt` (**69,626 bytes**, +2,023). Pool band 41-71 KB: inside.
Three edits in, one stale block trimmed out.

---

## 146-A - RETIRE the "the header does not count them" block; replace it with the double-count reading and the pay-3-life rule

**Finding.** (a) D2 shipped: 6 of 6 deck146 pregame prompts holding a land-backed MDFC carry
`1 card is counted TWICE above, on purpose:` and the land tally now includes the back face
(seat file, HEADLINE 1). The retired block's OWN removal condition - *"REMOVE THIS RULE when the
mulligan header's land number already counts those three names"* - is met, and leaving it would
make the pilot add one to a number that already counts it. (b) HEADLINE 2: both of this deck's
-2 losses are 3-life MDFC arrival payments, one of them a suicide at `Your life: 1`
(`146v130` seq 20), the other turning a survivable 19-token swing lethal (`146v123` seq 8 ->
gameend -2). It paid nothing in the two games it won.

**BEFORE** (18 lines):

```
- THREE OF YOUR SPELLS ARE ALSO LANDS, AND THE HAND HEADER DOES NOT COUNT THEM. Emeria's Call,
  Agadeem's Awakening and Pelakka Predation each have a LAND on the back, and the game offers that
  face as a row reading "<land name> with <spell name> -> PLAY THIS AS A LAND". TAKE IT ONLY ON
  YOUR OWN MAIN PHASE WITH YOUR LAND DROP UNUSED - that is when playing a land is legal, and it is
  the only window where taking it is a real land drop. Taking it puts the land onto your
  battlefield for no mana and uses your land drop.
  So: READ THE HEADER'S LAND NUMBER, THEN ADD ONE FOR EACH of those three names on the hand line.
  That sum - not the header's own number - is the land count every mulligan and bottom rung below
  is written against. A "1 land" seven holding Agadeem's Awakening is a TWO-land seven.
  Emeria, Shattered Skyclave taps for {W}; Agadeem, the Undercrypt and Pelakka Caverns tap for {B};
  each enters tapped unless you pay 3 life on the menu that follows.
  WHAT THIS COST, in this deck's two losses of one corpus: at `vs152` the header read "0 lands" ...
  REMOVE THIS RULE when the mulligan header's land number already counts those three names.
```

**AFTER**: the "counted TWICE" reading (land number TRUE and unchanged, spell number one too high,
the `Mana sources` line already carries the back face's colour so a colour printed once may be
coming from that card alone), then a new bullet, **THE MENU THAT FOLLOWS AN AGADEEM OR AN EMERIA
COSTS 3 LIFE**: default `tap`; pay only when (a) the untapped land makes a cast possible IN THIS
WINDOW against the printed `Mana available: N total`, AND (b) life after paying stays above the
total power on their battlefield line; **NEVER PAY AT 3 LIFE OR LESS**; Pelakka Caverns exempt (its
row says `{it enters TAPPED ... and no menu follows}`); with both losses cited by seq.

Every number in the new text is read off the render: the `counted TWICE` sentence, the
`Mana available: 1 total` at `146v130` s20, `Your life: 1` on the same prompt, and the row's own
`{taking this row then ASKS you to pay 3 life ...}` clause.

---

## 146-B - the cleanup-discard order contradicted itself in one sentence

**Finding.** The rung list read *"In order: Emeria's Call, Agadeem's Awakening, a second
planeswalker (Emeria's Call, Agadeem's Awakening and Pelakka Predation are NOT on this rung any
more ...)"* - it names the three cards as the FIRST thing to send and excludes them in the
parenthesis of the same sentence. `146v130` seq 7 answered `PUT: 8` = Emeria's Call at ONE land,
which is what the first half of that sentence says to do.

**BEFORE**
```
                                                        In order: Emeria's Call, Agadeem's Awakening, a
  second planeswalker (Emeria's Call, Agadeem's Awakening and Pelakka Predation are NOT on this
  rung any more - they are lands, and go only under the land rung below), then a land in hand
```
**AFTER**
```
                                                        In order: a second planeswalker
  (EMERIA'S CALL, AGADEEM'S AWAKENING AND PELAKKA PREDATION ARE NOT ON THIS RUNG - they are lands
  and go only under the land rung below), then a land in hand
```

---

## 146-C - the culling windows do not repeat the pregame note (added under rung (1) of the SCAN)

**Finding.** Seat file HEADLINE 3 / E-2. At `146v130` seq 7 the discard row for Emeria's Call reads
`Emeria's Call {4}{w}{w}{w} (sorcery) {card text: Create two 4/4 white Angel Warrior ...}` with no
land word anywhere; the `counted TWICE` note exists on 6 prompts in the corpus and all 6 are
pregame. At `146v123` seq 3 the note IS present and the header reads `{W} 2, {B} 1` - and the seat
still bottomed Pelakka Predation, the card the `{B} 1` came from, with a duplicate Nadaar on the
same list.

**ADDED** (7 lines, indented under rung (1)):

```
        THE ROW WILL NOT TELL YOU. Only the PRE-GAME header carries the "counted TWICE" note; at a
        cleanup discard, at a forced discard and at a bottom ask these three print as plain
        sorceries, with their spell cost and their spell text and NOTHING about a land. Recognise
        the three names yourself, every time, at every window that culls your hand.
        WHAT THIS COST: at `vs130` seq 7 ... the PUT line sent EMERIA'S CALL ...
        At `vs123` seq 3 the bottom ask sent Pelakka Predation off a keep whose header read
        "{W} 2, {B} 1" - the {B} 1 WAS Pelakka Caverns - with a duplicate Nadaar on the same list.
```

---

## 146-D (trim, no finding) - the superseded WHAT THIS BROKE block

The 7-line wave-55 anecdote under the SCAN list is now doing the same job as 146-C's fresher,
seq-cited evidence one screen above it. Cut to 3 lines, keeping both wrong answers and the
"both were row 1" point, which is the part the SCAN rung is defending against. Saves 334 bytes and
pays for part of 146-A.
