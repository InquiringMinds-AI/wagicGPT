# deck152 - wave-57 edits (before -> after), each with its finding

Base = `bin/Res/ai/baka/deck152_strategy.txt` (65,628 bytes, 780 lines).
Result = `wave57/deck152/strategy.txt` (**65,648 bytes**, +20 - one retirement pays for two
additions). Pool band 41-71 KB: inside.

---

## 152-A - RETIRE the "NEVER TAKE Flip Side" paragraph; its own removal condition is met

**Finding.** `Flip Side` does not appear ANYWHERE in ANY prompt in this corpus - 0 rows, 0
occurrences, corpus-wide (wave 56: 109 rows in 90 windows). Lane D's D14 drops the toggle at the
candidate level for land-backed MDFCs; the 1,309 `only display-toggle ... auto-passing` stderr
lines are that drop firing with zero prompt cost. The paragraph said, in its own words, *"REMOVE
THIS PARAGRAPH WHEN the 'Flip Side' row stops rendering on a land menu - re-verify by counting
'Flip Side' option rows in the corpus"*. Counted: 0.

**BEFORE** (8 lines)
```
  NEVER TAKE "Flip Side" ON A LAND MENU. It is a DISPLAY toggle only: it changes nothing about
  what you can play, its own row now says so, and the face you want already has a row of its own.
  A flip costs you a whole decision and buys nothing. WHAT THIS PAID: at `vs123` seq 24 this seat
  answered the other-face row directly and cast a {4}{g}{g} Tovolar's Huntmaster on the same turn;
  across the corpus the Flip Side row was offered 109 times and taken zero times, which is right.
  REMOVE THIS PARAGRAPH WHEN the "Flip Side" row stops rendering on a land menu - re-verify by
  counting "Flip Side" option rows in the corpus; it was 109 rows / 0 takes in wave 56, and the
  docket asks for the row to be suppressed.
```
**AFTER** (3 lines)
```
  The other-face row is the ONLY route to that face and it is always on the menu when the face is
  playable; there is no toggle to hunt for. WHAT THIS PAID: 15 such rows this corpus, 7 taken,
  7 arrivals, every one on your own main phase with the drop unused.
```
(The Brutal Cathar / Tovolar's Huntmaster "Flip Side is a display toggle" bullet three lines above
is KEPT - those are daybound transform cards, a different surface, and it does not depend on the
row rendering.)

---

## 152-B - RE-KEY the HOLD paragraph. The rule failed, and its key was measuring the wrong thing

**Finding.** Seat file HEADLINE 4 / E-3. 73 windows at deck152's seats carried
`[you declined this exact list N times already this turn]` with N>=2 and the HOLD row on the menu;
66 answered `Cast nothing right now` (wave-56 P6 asked for below 15) and 7 took HOLD (P6 asked for
25%). **But 0 of the 66 had a `--- CURRENT SITUATION ---` block identical to the window before** -
the stack was draining, life was moving, permanents were arriving. HOLD would have been re-opened
by the same change in all 66, so obedience had a ceiling of zero. Corpus-wide the note fires on 801
windows at N>=2 against 163 genuinely repeated situations - a 5x over-selection - and no prompt
anywhere in the corpus is byte-identical to its predecessor.

Repro for the shape: `152v123` seqs 69-82, turn 15, N climbing 2 -> 13 on a 3-row menu while the
stack goes from 4 items to 1 and life moves 22 -> 24.

**BEFORE** (19 lines): "THE TEST IS ONE LITERAL ... that row is the wrong one: it re-opens the same
list at the next window and you will be answering it again inside the same turn" + three stacked
scoreboards (wave 55's 190 windows, this corpus's 33 of 35, "wave 56: 2 of 35 - the rule is
working").

**AFTER** (13 lines): HOLD stays, demoted to what the evidence supports -
```
THE TEST IS ONE LITERAL, AND IT IS A TIEBREAKER, NOT AN ALARM. ... HOLD is never worse: its own row
says you give up no cast, and any change to the board re-opens the window at once.
WHAT THAT LINE DOES AND DOES NOT MEAN. It counts identical OPTION LISTS, not identical boards. At
this deck's seats this corpus, 73 windows carried it with N of 2 or more, and in NOT ONE of them was
the "--- CURRENT SITUATION ---" block the same as the window before ... Never read that N line as
"nothing is happening"; it says only "this menu has not changed". READ THE SITUATION BLOCK, DECIDE,
AND THEN LET THE N LINE PICK THE ROW - never the other way round, and never let it push you into
passing a window where the board in front of you has moved.
```
The deleted claim ("you will be answering it again inside the same turn") is false in 66 of 66
measured windows, and the trust doctrine forbids a guide line the render refutes. The three
scoreboards go with it (skill proposal S4).

---

## 152-C - the pregame mana-source line counts only the face each Pathway is showing

**Finding.** `152v125` seq 1: `Mana sources among those lands, counted by the engine: {W} 1, {G} 3`
on a hand line holding `Branchloft Pathway (land: taps for {G})` and two Barkchannel Pathways -
whose other faces are Boulderloft {W} and Tidechannel {U}. deck152 played back faces **7 times**
this corpus (Boulderloft 5, Tidechannel 2). 4 of its 6 keeps hold a Pathway. Lane E states the
land/land Pathway class was deliberately untouched by D2, so this is the pilot's job for now. No
decision turned on it this corpus (deck152 kept all six hands), which is why it is a one-paragraph
rule and an E-6 render ask rather than a headline.

**ADDED**, immediately above the `ONE LAND: read the coverage line` rung:
```
- THE PRE-GAME "Mana sources among those lands" LINE COUNTS ONLY THE FACE EACH PATHWAY IS SHOWING.
  A Branchloft Pathway on your hand line is counted under {G} and NOT under {W} ... So before you
  price a keep, ADD the other colour of every Pathway on the hand line: `vs125`'s seven read
  "{W} 1, {G} 3" on a hand whose Branchloft and two Barkchannels could also have made {W} and {U}.
  The land-backed SPELLS of other decks get a "counted TWICE" note; your Pathways get none, because
  both faces are already lands and the card is counted once - correctly, and one colour short.
  This never ships a hand: it only ever ADDS colours, so it can turn a ship into a keep and never
  the reverse.
```
The last sentence is load-bearing: this deck's oldest and most-cited failure mode is shipping a
hand for a colour it could have made, and a rule about colour counts must be unable to produce a
mulligan.
