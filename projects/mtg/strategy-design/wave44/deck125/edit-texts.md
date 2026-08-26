# deck125 (Revelation Fracture) — wave-44 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` as of the
matchups-20260826-111937 corpus. Every card fact below was read off the primitive
(`projects/mtg/bin/Res/sets/primitives/*.txt`) and, where load-bearing, cross-checked
against Scryfall; every number is counted from the corpus.

---

## E1 (HIGH) — the 0-creature sweeper gate, hoisted and repeated

**Finding.** 118 windows offered Supreme Verdict or Final Judgment; 14 casts. THREE of
those casts happened with the header reading "of which 0 are creatures":
`...deck125-...-vs-ai_baka_deck130` seq 25, `...vs-ai_baka_deck162` seq 13,
`...vs-ai_baka_deck152` seq 48. The rule already existed — it was the LAST bullet of a
five-bullet list, and it was missing from DECIDING SITUATIONS entirely.

The deck162 instance decided the only deck125 loss. At seq 13 (turn 11, life 19) the
opponent board was `Opponent battlefield (6 permanents listed, of which 0 are creatures)`
— five lands and Ob Nixilis, the Hate-Twisted. The model cast Supreme Verdict, spending
4 of its 5 untapped sources. On the opponent's next turn Dictate of Kruphix and Howling
Mine both resolved with no counter window; from then on every card drawn cost 1 life to
Ob Nixilis and (once it landed) 1 more to Underworld Dreams. Life went 20 -> 0 by turn 18.

**Before** (SWEEPERS header):
```
creatures)". Read that number and act on it:
- 2 or more creatures: ...
```
**After**:
```
creatures)". READ THAT NUMBER FIRST, before you look at anything else in the menu.
IF THAT NUMBER IS 0, SUPREME VERDICT AND FINAL JUDGMENT ARE NOT CASTABLE PLAYS THIS TURN. Answer
"Cast nothing right now" and keep them. This line outranks every other reason to cast a sweeper -
being ahead, having spare mana, wanting to use the turn, the plan naming a wipe. A wipe with nothing
to kill is a card you threw away AND a turn you spent tapped out, and the turn you spend tapped out
is the turn their engine lands uncountered. Only when that number is 1 or more do you read on:
- 2 or more creatures: ...
```
**Before** (trailing bullet):
```
- 0 creatures: answer "Cast nothing right now" and keep the sweeper. A wipe with nothing to kill is a
  card you have thrown away.
```
**After**:
```
- 0 creatures: answer "Cast nothing right now" and keep the sweeper (the hard line at the top of
  this section).
```
**Added to DECIDING SITUATIONS** (new line, above the 1-creature line):
```
- The header says 0 creatures: cast NOTHING. Never a Supreme Verdict, never a Final Judgment, at any
  life total and with any amount of spare mana.
```

---

## E2 (HIGH) — the X menu contradicted itself; the cap now comes first

**Finding.** 14 X-announce windows across 6 games. The model took the LARGEST offered X
in **14/14**. The library cap (W8, "X <= N-2-Staffs") held in 13/14 — the one failure is
the only window where it bound: `...vs-ai_baka_deck146` seq 87, library 13, one Staff,
menu up to 12, announced **X = 12**. The hand cap (W9, "hand + X <= 7") held in **5/14**.

Root cause is inside the guide, not the model. The paragraph immediately above the cap
read as an unconditional order: *"pick the line showing the LARGEST X ... the line you
pick must literally read 'X = ' followed by the biggest number in the menu."* The cap
sentences that followed it were obeyed only when they happened to agree.

Cost at seq 87: the 12 draws were followed by 12 discards — Island, Dream Fracture, Fall
of the Gavel, **Elixir of Immortality**, **Path to Exile**, Island, Fall of the Gavel,
Tundra, Emrakul, Essence Scatter, **Final Judgment**, Cancel. Only Emrakul's graveyard
trigger (shuffling 32 cards back) kept the library alive.

**Before**:
```
When the X menu appears, pick the line showing the LARGEST X. The sentence above the menu names the
largest X you can afford, and every value listed is affordable. Reply with the OPTION number, not the
X value - the line you pick must literally read "X = " followed by the biggest number in the menu.
Before you announce X, count the cards on your own "Your hand:" line and add X to that
number. Everything above 7 is discarded at end of turn: those are cards you paid mana
for and threw away. If the opponent battlefield line shows Liliana's Caress, Underworld
Dreams or Fate Unraveler, each discarded card ALSO costs you life on the way to the
graveyard. Announce the largest X that keeps (cards in hand + X) at 7 or less, even
when the menu offers more and even when your library is deep.
```
**After**: a "WORK OUT YOUR X BEFORE YOU LOOK AT THE MENU" block that computes two
ceilings (hand and library), takes the smaller, and carries the seq-87 numbers as a
worked example. Full text in `strategy.txt`.

Four dependent rewrites came with it:
- `At N of 6 or more, cast it as above and pick the largest X the menu offers, EXCEPT...`
  -> a hand gate (`If your "Your hand:" line already lists 7 or more cards, do NOT take
  "Cast Sphinx's Revelation" at all this turn`) plus a pointer to the ceilings. At seq 87
  the hand was 8; the correct play was to not cast at all, which the old text never said.
- DECIDING SITUATIONS: `cast it, then pick the line with the largest X on the menu that
  follows` -> `announce the SMALLER of (7 minus the cards in your hand after removing the
  Revelation itself) and (your library minus 2 minus one per Staff of Nin)`.
- Two residual absolutes elsewhere in the same section were removed, because they said the
  same thing E2 was written to stop (per skill proposal S1 — a capped rule must not be
  contradicted by an uncapped one a few lines away):
  `then cast it for whatever X the menu offers` -> `then cast it, and size X by the two
  ceilings below like any other Revelation`; and `Cast it at whatever size the game allows,
  whatever that number says, when your hand holds no counter and no Path to Exile` -> `When
  your hand holds no counter and no Path to Exile you need the cards more than you need
  anything else - but the two ceilings below still bind ...`.
- The worked example now names the better play as well as the better X ("a hand of 8 means
  you should not have taken 'Cast Sphinx's Revelation' in the first place").

---

## E3 (MED) — the hand count at the X menu is off by one

**Finding.** In **14/14** X-announce windows the Sphinx's Revelation being cast is still
listed on the "Your hand:" line, and there is no ON THE STACK line in that window. The
old rule ("count the cards on your own 'Your hand:' line and add X") therefore over-counts
the hand by one every single time. Folded into E2 as `CEILING 1 ... SUBTRACT 1: the
Sphinx's Revelation you are casting right now is still listed there, and it is leaving.`
Filed as a LOW render item as well.

---

## E4 (HIGH) — Elixir activation threshold 12 -> 16

**Finding.** No decking losses this corpus (W7 PASS, 0/6). But the margin was luck, not
the rule. In `...vs-ai_baka_deck123` the Elixir option was offered in **17** priority
windows and taken **0** times while the library ground from 18 down to 12 (seq 52 -> 69).
The guide's trigger is `N is 12 or less`; the last window that OFFERED the Elixir was
seq 68 at library **13**, so the rule never fired. The library was rescued at library 12
by an Emrakul discard trigger, not by the guide. Per-game library minima this corpus:
20, 12, 13, 20, 39, 14.

The Elixir is only offered in priority windows, which arrive roughly every other turn,
and with two Staffs of Nin the deck draws three cards a turn. A threshold of 12 is inside
the sampling gap.

**Before**: `If N is 12 or less and an Elixir of Immortality of yours is on the
battlefield untapped, ACTIVATE IT IN THAT WINDOW`
**After**: `If N is 16 or less ... ACTIVATE IT IN THAT WINDOW ... Sixteen, not twelve:
with two Staffs of Nin you draw three cards a turn, and the window that offers the Elixir
is a priority window that only comes around every other turn - by the time N reads 12 the
next offer can be at 6.`

---

## E5 (HIGH) — the engine-text clause, ported from Path into the counter section

**Finding.** Wave 43's W6 fix taught "a creature whose text does something every turn is
worth the Path however small its printed power" — and it PASSED on the Path side (6/7
engine-shaped targets Pathed within two windows). The identical shape on the COUNTER side
was still governed by the flat rule *"(creature P/T) with printed power 0 or 1: LET IT
RESOLVE"*, and that rule ate three spells this corpus:

| file | seq | spell on stack, as rendered |
|---|---|---|
| `...deck125-...-vs-ai_baka_deck130` | 28 | Dwarven Blastminer {1}{r} (creature 1/1) {target text: "{2}{R}, {T}: Destroy target nonbasic land..."} |
| `...deck125-...-vs-ai_baka_deck130` | 63 | Dwarven Blastminer (same) |
| `...deck125-...-vs-ai_baka_deck152` | 39 | Luminarch Aspirant {1}{w} (creature 1/1) {target text: "At the beginning of combat on your turn, put a +1/+1 counter..."} |

Each then had to be answered with a Path later in the same game (deck130 seq 67; deck152
seq 7/24/42) — spending the guide's own "only answer to a creature that already landed"
on a spell a counter would have handled. The {target text:} needed to make the call was
present on all three option rows.

**Before**:
```
  - (creature P/T) with printed power 0 or 1: LET IT RESOLVE. It cannot kill you, and Path to
    Exile or a sweeper answers it later for less mana. ...
```
**After**: the same bullet with an `unless its {target text: ...} on that same option line
contains "{T}:", "Whenever ... attacks", "At the beginning of" or "venture"` carve-out,
naming Dwarven Blastminer, Luminarch Aspirant and Triumphant Adventurer as the shapes.
Card texts quoted are the primitives' own `text=` lines.
DECIDING SITUATIONS got the matching one-liner.

---

## E6 (LOW) — Lightmine Field: either main phase

**Finding.** Both Lightmine Field casts this corpus (`...vs-deck123` seq 13, mana 4;
`...vs-deck152` seq 43, mana 7) were in MAIN PHASE 1 at a 0-creature header. The guide's
pre-emptive escape hatch required "your second main phase", so both were technically
violations, and neither cost anything — the deck has no creatures, so the card is pure
insurance. The phase restriction is a rule the model does not follow and does not need.

**Before**: `unless it is your second main phase, nothing else in your list is worth
casting, and your "Mana available:" line still reads 4 or more`
**After**: `unless nothing else in your list is worth casting this turn and your "Mana
available:" line still reads 4 or more AFTER the mana you keep open for a counterspell:
then cast it in EITHER main phase`

---

## E7 (MED) — Staff of Nin's tap-out rule, made arithmetic

**Finding.** 11 Staff of Nin casts. **5** of them left the model unable to pay its
cheapest counterspell in hand, which is exactly what the rule forbids:

| file | seq | mana available | left after Staff | cheapest counter in hand |
|---|---|---|---|---|
| `...vs-deck123` | 19 | 7 | 1 | 3 (Cancel/Dream Fracture) |
| `...vs-deck123` | 23 | 8 | 2 | 3 |
| `...vs-deck146` | 16 | 6 | 0 | 5 (Fall of the Gavel) |
| `...vs-deck126` | 27 | 6 | 0 | 2 (Essence Scatter) |
| `...vs-deck152` | 18 | 6 | 0 | 2 |

The old text asked for a comparison against a symbol ("leaves {1}{U} untapped") rather
than a number the render prints. The new text names the number.

**Before**: `but only when doing so leaves {1}{U} still untapped. If casting the Staff
would take your "Mana available:" line below {1}{U}, ...`
**After**: `but only when doing so leaves your cheapest counterspell payable. Do the
subtraction on the render: Staff costs 6, so you need your "Mana available:" line to read
8 or more with an Essence Scatter in hand, 9 or more with a Cancel or a Dream Fracture,
11 or more with only a Fall of the Gavel, and 6 is enough only when your hand holds no
counter at all.`

---

## E8 (LOW, n=1, flagged) — the one keepable-by-count hand that is still a mulligan

**Finding.** Six mulligan decisions, one mulligan taken
(`...deck125-...-vs-ai_baka_deck152` seq 1): a 7 with `2 lands, 5 spells`, `Mana sources
among those lands, counted by the engine: {W} 2`, and `Playing every land in this hand
would cover the cost of: Path to Exile {w}` — two Plains, Emrakul, Dream Fracture
{1}{U}{U}, two Fall of the Gavel {3}{U}{W}, one Path. The guide's floor says KEEP at 2
lands and says explicitly that if the "would cover the cost of" line names even one card
the hand can act. The model shipped it, kept a 3-land 7 with two Sphinx's Revelations,
bottomed Final Judgment, and won a 52-turn game.

This is ONE instance and the win is not evidence (new-baseline discipline). The edit only
carves out the exact shape observed, so any hand outside it is still a mandatory keep.

**Added** (MULLIGAN, after the tiebreaker paragraph):
```
The ONE hand that passes the land count and is still a mulligan: the engine's "Mana sources among
those lands" line names a single colour, AND the "Playing every land in this hand would cover the
cost of:" line names only one card. That hand has two lands and five cards it cannot cast; ship it.
Any other hand with 2 to 5 lands is a keep.
```
Also routed to the owner as a floor question in the seat file — this is a mulligan-floor
deviation on the first fair-hand corpus, and one sample cannot settle it.

---

## Deliberately NOT changed

- **The counter-on-sight list**, the Path power rule and the counter power rule for
  power >= 2: 45/45 counter option rows carried {target text:}, 39/45 counter windows took
  the counter, and the six declines were the three E5 cases plus three correct ones
  (Pride Guardian 0/3, Perimeter Captain 0/4, Chromatic Lantern).
- **The Emrakul block**: no Emrakul was ever castable (the "Cast Emrakul" line never
  appeared); the block is untested this corpus, not wrong.
- **The Path floor at life > 12.** 11 windows in `...vs-deck146` declined Path on 2/1
  Spider tokens at life 34-40, which the "highest printed power" line technically demands.
  Nothing turned on it, the deck won that game from 47 turns, and loosening a removal
  floor on a null finding is how the wave-43 W1 punisher clause went wrong. Left alone;
  logged as a wave-45 watch item instead.
