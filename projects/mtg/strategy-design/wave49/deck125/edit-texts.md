# deck125 (Revelation Fracture) - wave-49 edits, before -> after, each with its finding

Corpus `matchups-20260827-094106`; every seq below is in the deck125 seat log named. Start point =
the live guide (`bin/Res/ai/baka/deck125_strategy.txt`, wave-48 revision + boundary pass); the
full revised guide is `wave49/deck125/strategy.txt` (40,013 bytes, +998 on the live 39,015 after
a 15-line duplicate paragraph was cut).

## W14 - EMRAKUL IS OFF against an edict or a converter (NEW; the vs126 loss)
Finding: `...deck125-0x562af3b37450-vs-ai_baka_deck126.jsonl` seq 117 (t60, life 48 vs 15, Staff
ticking them since t53): `Cast Emrakul, the Aeons Torn {15} ... {leaves 6 of your 21}` taken with
the opponent battlefield line showing BOTH `Sanguine Bond` and `Exquisite Blood` and the prompt's
own LOOP clause ("Both halves of a life LOOP are on THEIR battlefield ... ANY nonzero payment ...
is fatal"). The deck126 seat's next window (its seq 104) printed `Cast Tribute to Hunger {2}{b}
{right now: they control 1 creature - Emrakul, the Aeons Torn is sacrificed, you gain 15}` and took
it: 48 -> -12 in one resolution. Same shape at `...vs-ai_baka_deck123.jsonl`: Emrakul cast seq 55,
opponent's Devour Flesh took it (opp 2 -> 17 between seq 77 and 78); the seat still won with the
second Emrakul. STRATEGY: the guide's "take it, it outranks everything" had no counter-shape.
Before (section #1, after "...the game only prints a cast you can pay for."): nothing.
After: new paragraph "THE ONE BOARD WHERE THAT LINE IS OFF, and it outranks it: the opponent
battlefield line shows Sanguine Bond or Exquisite Blood, or the game log shows the opponent has
cast Tribute to Hunger or Devour Flesh ... answer "Cast nothing right now" to the Emrakul row every
time and let the Staff finish - it was finishing." Plus the matching DECIDING SITUATIONS bullet
("TAKE IT - unless the opponent battlefield line shows Sanguine Bond or Exquisite Blood, or the
log shows they cast Tribute to Hunger or Devour Flesh").

## W15 - THE STAFF GATE, sixth wave: row string first, plan line reordered, contradiction removed
Finding: Staff casts 6, breaks 3 - `...vs-ai_baka_deck152.jsonl` seq 23 (t15, "casting this taps
you out", Essence Scatter in hand; Tovolar's Huntmaster resolved next turn with NO window, 18 ->
8 -> dead: the loss), `...vs-ai_baka_deck146.jsonl` seq 30 ("taps you out", Fall of the Gavel in
hand), `...vs-ai_baka_deck130.jsonl` seq 54 ("leaves 1", Dream Fracture in hand). All three replies
echoed the plan line "resolve Staff of Nin from a row reading leaves 5 or more" verbatim while
casting off a row that read 0 or 1 - #102's shape (rule in the plan, answer breaks it). The
DECIDING SITUATIONS list also carried a bullet that contradicted the gate ("'Cast Staff of Nin' is
in your list and no answer is needed this turn: take it").
Before (plan line, 3 places): "Answer their threats, resolve Staff of Nin from a row reading leaves
5 or more, ping their face every turn."
After: "Keep a counterspell's mana open; Staff of Nin only from a row reading leaves 5 or more;
ping their face every turn." (the gate is the plan's FIRST clause, the named action carries its
qualifier as "only from").
Before (Staff bullet): "CAST STAFF OF NIN ONLY WHEN ITS OWN OPTION ROW READS "leaves 5" OR MORE
... Two of six Staff casts this corpus broke it ..."
After: "A STAFF ROW READING "casting this taps you out", OR "leaves 4" OR LESS, WITH ANY
COUNTERSPELL IN YOUR HAND, IS ANSWERED "Cast nothing right now" ... This one: three of six Staff
casts ... the first of them lost the game - a Tovolar's Huntmaster resolved on the next turn with
no window offered ..."
Before (situations): "'Cast Staff of Nin' is in your list and no answer is needed this turn: take it."
After: "'Cast Staff of Nin' is in your list: take it only from a row reading "leaves 5" or more,
or with no counterspell in your hand. Its row reads "taps you out" or "leaves 4" or less with a
counterspell in hand: cast nothing, and cast it next turn."

## W16 - SPHINX'S REVELATION re-keyed to the row's "max affordable X=N"; duplicate gate paragraph cut
Finding: five Revelations cast off rows reading `{X pricing: max affordable X=1 ...}` or `X=2` -
`...vs-ai_baka_deck146.jsonl` seq 19 (X=1), `...vs-ai_baka_deck130.jsonl` seq 31 (X=1), 38 (X=2),
47 (X=2), `...vs-ai_baka_deck152.jsonl` seq 28 (X=2, life 13) - against the "Mana available: SIX
or more" rule. No game turned on them (violated-unpunished, second corpus of the shape), but the
rule was keyed to a number the model has to compare against a cost, while the row prints the
answer. Also the tap-out-gate paragraph was duplicated verbatim in the Revelation section (15
lines) - cut, per the "shrink" step.
Before: the 15-line "THE TAP-OUT GATE, and it covers EVERY mana ..." duplicate + "Cast Sphinx's
Revelation when your "Mana available:" line reads SIX or more - that is X = 3 or better. Below
six, answer "Cast nothing right now" ..."
After: "THE TAP-OUT GATE from the Staff section covers a Revelation too: with any counterspell in
your hand, take its row only when the row's "leaves N" reads 5 or more. Its row also prints the
size it can be: "{X pricing: max affordable X=N ...}". Cast Sphinx's Revelation only from a row
whose max affordable X reads 3 or more ... Five casts this corpus went off rows reading "max
affordable X=1" or "X=2" for one or two cards: read the N off the row, and 3 is the floor."

## W17 - SWEEPERS AT 0 / LIGHTMINE BELOW 3: the row's own string named (sixth wave, #118 re-key)
Finding: `...vs-ai_baka_deck126.jsonl` seq 60 (t34, life 35 vs 20): `Cast Final Judgment ...
{right now: exiles 5 of their creatures (0 able to attack), 0 of yours}` taken at a header
reading `5 are creatures, 0 of them without a restriction against attacking` (five walls), after
~25 correct declines on the same board. `...vs-ai_baka_deck130.jsonl` seq 50 (t32): `Cast
Lightmine Field ... {right now: they control 0 creatures able to attack - deals 0 until they
have an attacker}` taken at a header second number of 2. Sweeper casts this corpus: 4, of which 2
correct (vs123 seq 13 at 323; vs152 seq 21 at 3). Lane T's `{right now:}` clause now prints the
fact on the row; the rule names the string.
Before: "Four sweepers this corpus went into a 0, and every one took one of those shapes ..."
After: "The sweeper row prints the same fact beside the header: "{right now: exiles 5 of their
creatures (0 able to attack), 0 of yours}" next to a header second number of 0 is five walls, and
"destroys 0 of their creatures" is an empty board. Read the header number; the row's first number
counts walls too. You cast a Final Judgment off exactly that ... row at 35 life with no clock on
you ..."
Before (Lightmine): "All three casts this corpus were at that number = 0, one of them on turn 6 ..."
After: "Its row says so itself: "{right now: they control 0 creatures able to attack - deals 0
until they have an attacker}" is a "Cast nothing" row ... You cast it off that "deals 0" row at a
header of 2 this corpus, after three casts at 0 the corpus before."

## W19 - PATH: the wall pattern has two printed forms
Finding: `...vs-ai_baka_deck162.jsonl` seq 24 (t16, life 13): `Cast Path to Exile ... legal
targets right now: Shield Sphere (artifact creature 0/6) [defender]` taken - the only candidate,
tagged [defender], and the guide's pattern read "(creature P/T)". Harmless in the game (the seat
was already dead to the draw lock) but a rule broken with the fact rendered.
Before: "If EVERY candidate on that clause reads printed power 0 or 1, or is tagged [defender],
answer "Cast nothing right now""
After: "... or is tagged [defender] - printed "(creature 0/4) [defender]" or "(artifact creature
0/6) [defender]" alike; you Pathed a Shield Sphere off the second form at 13 life - answer ..."

## Recorded, NOT edited (first corpus of the shape, or already covered)
- Land drop declined 4 times (`...vs-ai_baka_deck146.jsonl` seq 94; `...vs-ai_baka_deck130.jsonl`
  seq 80, 98, 100), all at 12+ lands in play with a full hand, bare `CHOICE: N (Play no land right
  now)` replies. Wave 48 was 84/84. No consequence; #0 already says the card is discarded at
  seven. Watch W44.
- Forced Fruition: own-turn casts under Fruition + punishers 0/6 (`...vs-ai_baka_deck162.jsonl`
  seq 27-32) - W38 PASS on that half; the one counter cast under the pair (seq 33, Cancel at 2 life
  under Fruition + 3 Dreams + Ob Nixilis, price 28) broke "below your life minus 1" with the seat
  already dead to the next draw step. The text covers it; not re-worded.
- Elixir activations below the gate with a counter in hand: `...vs-ai_baka_deck126.jsonl` seq 8,
  15; `...vs-ai_baka_deck130.jsonl` seq 7, 15; `...vs-ai_baka_deck146.jsonl` seq 35 (5). None cost
  a window (the spells that resolved in them were enchantments a lone Essence Scatter could not
  have countered). Counted under W41; the gate sentence is unchanged.
- Path at Goblin tokens: `...vs-ai_baka_deck130.jsonl` seq 34 and 41 (life 21 / 17, the only
  candidates `Goblin #1-#3 (creature 1/1)`; seq 31 declined the same board). W12's clause held
  1/3; second corpus at 2. Not re-worded this wave - the rule is already keyed to the printed
  "(creature 1/1)" with no {target text:}; if it fails a third corpus, #118 applies.
- Mulligan: 6 offers, 0 taken; keeps at 4, 2, 5, 5, 4, 2 lands, every coverage line naming a
  card. Nothing to teach; the W13 odds text stands.
