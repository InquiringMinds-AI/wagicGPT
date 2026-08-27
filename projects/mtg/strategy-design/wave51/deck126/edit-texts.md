# deck126 - wave-51 edits (each: finding -> before -> after)

Start = live `bin/Res/ai/baka/deck126_strategy.txt` (wave-50 F16-F20 + boundary pass). Full revised
guide: `wave51/deck126/strategy.txt` (40,695 -> 43,331 chars). Every edit keyed to a corpus seq in
`seats/seat-125-126.md`.

## F21 - Chromatic Lantern re-derived: above the walls exactly when a combo piece or Tutor is in hand at <= 4 sources (D55: `vs125` seq 6, `vs130` seq 7 - fourth corpus of unpunished breaks, both wins)
Finding: both Lantern-over-wall casts had a five-mana enchantment on the hand line at 3-4 sources
and the enchantment resolved the next turn (Blood t8 off `leaves 1`; Bond t9). #118's ceiling on a
violated-unpunished rule: re-key, not restate. The `[already owned:` half is closed (9/9 declined).
BEFORE (entry 7): "Chromatic Lantern - when nothing from entries 1-6 is on the menu AND no
Chromatic Lantern is on your battlefield line. It is a mana rock ... as the only real row beside
"Cast nothing right now" it has been the right cast in game after game. Do NOT count black sources
by name to decide this; the entry above it on this list decides it - you cast a Lantern over a
Pride Guardian this corpus, the wall one entry higher. A SECOND Lantern does nothing ... You cast a
second Lantern this corpus after writing "I do not have a CL on the battlefield" under a
battlefield line that printed one; read the line, not the memory of it."
AFTER: "Chromatic Lantern - when nothing from entries 1-6 is on the menu AND no Chromatic Lantern
is on your battlefield line. It is a mana rock: every land of yours then makes any colour and it
adds a mana of its own. IT MOVES ABOVE ENTRY 4 (the walls) in exactly one shape: Sanguine Bond,
Exquisite Blood or Idyllic Tutor is on your hand line AND your "Mana available:" line reads 4 or
less. Then the Lantern is the fifth mana the enchantment needs, a turn earlier than the land drop
brings it - you cast it over a Wall of Omens at 4 sources with Exquisite Blood in hand and the
Blood came down the next turn; over two walls at 3 sources with Sanguine Bond in hand and the Bond
came down the turn after; both games were won. With no combo piece in hand, or at 5 or more
sources, the wall comes first. A SECOND Lantern does nothing at any time - once "Chromatic
Lantern" is on your battlefield line this entry is finished for the rest of the game; the row now
prints "[already owned: Chromatic Lantern on your battlefield]" - nine such rows this corpus, nine
declined. Read the line, not the memory of it."
Situations bullet re-keyed the same way.

## F22 - A [defender] victim: the plan-text check + "a printed row is payable" (D52 FAIL: `vs162` seq 17)
Finding: Tribute on a Fog Bank at 13 life over a printed `Cast Sanguine Bond` row; the reply said
"remove the only blocker" (nothing of this deck attacks) and "we still lack the two black mana for
Sanguine Bond" (the row was printed = payable). F17's board rule failed its first test; #102's
plan-text form applies.
BEFORE (Rule 4, defender bullet, end): "... on turn 14 both enchantments were out against two Fog
Banks, you were at 3, and a Tribute in hand would have ended the game - you had none."
AFTER (appended): "The next corpus you spent one on a Fog Bank at 13 life with "Cast Sanguine
Bond" printed one row below it, and your plan said "remove the only blocker". IF YOUR PLAN SAYS
"remove their blocker", THE PLAN IS ALREADY WRONG: nothing of yours attacks except a Vampire, so a
wall of theirs blocks nothing you care about. And a row that is PRINTED is a row you can PAY - "we
still lack the two black mana for Sanguine Bond" beside a printed "Cast Sanguine Bond {3}{b}{b}"
row is a false sentence; the engine prints only casts it can pay for."

## F23 - Tutor is entry 2 only when the missing half is on NEITHER line; the reveal pick reads the hand line (`vs146` seq 28-29: Tutor over a printed `Cast Exquisite Blood` with Bond out; picked Bond #2)
Finding: the model believed Bond was missing with `Sanguine Bond` on its battlefield line and
`Exquisite Blood` on its hand line with a Cast row; the win waited from t18 to t22. A battlefield-
line misread (D55's report half) plus a Rule 3 step-4 pick keyed only to "what you have".
BEFORE (Rule 3 step 2): "If one is already on your battlefield and the other is not in your hand,
"Cast Idyllic Tutor {2}{w}" is your best play in that list - ahead of every wall and every other
card."
AFTER: "If one is already on your battlefield and the other is on NEITHER your hand line nor your
battlefield line, "Cast Idyllic Tutor {2}{w}" is your best play in that list - ahead of every wall
and every other card. If the other IS on your hand line with a "Cast" row, that row is the play and
the Tutor is not: you tutored with Sanguine Bond on the battlefield and "Cast Exquisite Blood"
printed on the same menu, fetched a second Bond, and the win waited two more turns."
BEFORE (step 4): "Pick the one of the two you do NOT already have; if you have NEITHER, pick
SANGUINE BOND ..."
AFTER: "Before the PUT line, read your battlefield line AND your hand line: the name printed on
either is the one you already have. Pick the OTHER one; if you have NEITHER, pick SANGUINE BOND - it
is the half that wins on its own (Rule #3). You picked a Sanguine Bond with a Sanguine Bond on your
battlefield and an Exquisite Blood in your hand."
Rule 7 entry 2 gains: "A missing name that is on your HAND line with a "Cast" row is entry 1, not
this."

## F24 - Mulligan: the two-land no-coverage hand (#131 gap; `vs146` seq 1 kept, won) and the one-land keep recorded (`vs152` seq 1)
Finding: the section had no sentence for "2 lands, would not cover any spell" (Bayou + Forest under
{W} walls); the seat kept it and drew the Scrubland on turn 4. Odds: 12 of 24 lands make {W}; the
chance one of the next three draws is one of them is about even, which a six-card hand does not
beat often enough to pay a card. The one-land keep (`vs152`, Captain x2 covered, won on Battlement
mana) is recorded as outside the rule; the rule stands.
NEW (before the zero/one-land bullet): "- Two lands and a coverage line reading "would not cover
any spell in it" (two lands that make no {W} under a hand of {W} walls): on a seven, KEEP - 12 of
your 24 lands make {W}, the odds that one of your next three draws is one of them are about even,
and a six-card hand that covers a wall is not better than that often enough to pay a card for it.
You kept exactly this hand (Bayou, Forest, three {W} walls, Tribute, Bond), drew the Scrubland on
turn 4 and won. On a six or smaller the same hand ships only when it also holds no one-mana spell."
Zero/one-land bullet gains: "You kept a one-land seven whose coverage line named two Perimeter
Captains and won on Battlement mana: the keep was outside this line and the win does not move it -
one land is one draw from nothing." Situations bullet updated. Twelve-hand check re-run (0 / 1 w/o
/ 1 with / all lands at 7, 6, 4) plus the new 2-land-no-coverage row at 7 and 6: no disagreement.

## Counts updated, no restatement
- Rule 4's "Three of four such rows this corpus" -> "+ one of one the next (Nadaar, turn 8, over
  two walls)" (D51 PASS).
- Rule 7's F19 paragraph: "+ once the next corpus ... and the engine asked again. When the window
  prints "[RE-ASK] ... is not on this list", the name you wrote is a hand card: answer the printed
  rows." (D54: the re-ask shape is now what the model sees.)

## Recorded, NOT edited
- `vs123` seq 18: Staff of Nin at `leaves 2` on t11 with no Tribute in hand; the Keeper's 25
  tokens came t12. No rule reaches a hand without the Tribute.
- `vs162` seq 21: Sorin before Tribute at 1 life (both cast the same turn) - fine.
- Cleanup discard: 0 asks (D53 UNTESTED); the F18 section is untouched.
- Engine #1 (Battlement over-tap) cost `vs152` 18 life; the guide cannot reach the planner - not
  a guide edit.
