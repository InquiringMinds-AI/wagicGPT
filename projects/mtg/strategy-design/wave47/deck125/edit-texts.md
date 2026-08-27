# deck125 (Revelation Fracture) - wave-47 edits, before -> after

Base: `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt`, verified byte-identical to
`wave46/deck125/strategy.txt` before editing (31,631 bytes). After: 34,394 bytes (**+2,763**).
ASCII-only (`grep -c '[^ -~]'` = 0). Corpus root
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-182155/`; every seq below is from a
`...ai_baka_deck125-0x...-vs-ai_baka_deckN.jsonl` file and is quoted from the record's own `prompt`
or `chosen_text`.

---

## W1 - PATH TO EXILE: the positive half of the power rule, and "tapped is not safe"

**Finding.** 42 Path windows, 7 casts. **13 windows at life <= 12 with at least one candidate that
is neither `[defender]` nor printed power 0/1; Path taken in 1.** Twelve of the twelve misses are
one game and it is a **loss**: `vs-ai_baka_deck146` seq 11 (life 9, Nadaar 3/3), seq 48 and 49
(life 10, two Spiders 2/1), seq 61 and 62 (life 12), seq 65 and 66 (life 5), seq 67 (life 5, Hive
of the Eye Tyrant 3/3), seq 70 and 71 (**life 1**, three Path to Exile in hand), seq 73 and 74
(life 6). Dead turn 38.
**Diagnosis - STRATEGY, an ordering defect of the kind amendment 70 names.** The section states the
rule as a NEGATIVE floor ("if EVERY candidate reads 0 or 1 ... keep the Path"), and DECIDING
SITUATIONS carried only that negative line. There was no sentence anywhere that says *cast it*. The
seq-74 reply shows the second half of the mechanism in the model's own words: *"You have 3 Path to
Exile in hand but no creatures to block the spiders if they attack, and the spiders are tapped"* -
a `[tapped - cannot attack or block this turn]` tag read as safety, plus reasoning about blockers in
a deck that has none.

BEFORE (Path section):
```
Your life is 12 or less: that floor is off for creatures that can attack - exile the one with
the highest printed power. A creature tagged [defender] still never gets the Path, ...
```
AFTER:
```
YOUR LIFE IS 12 OR LESS AND ANY CANDIDATE READS PRINTED POWER 2 OR MORE: CAST THE PATH AT THE
BIGGEST OF THEM, IN THAT WINDOW. Not next turn, not after you find a sweeper, not once you have
seen it attack. Thirteen windows this corpus offered a real target at 12 life or less and you took
ONE; the other twelve are a single game you lost, at 12 then 10 then 5 then 1 life, holding THREE
Path to Exile against two 2/1 Spiders and a 3/3, answering "Cast nothing right now" every time.
TAPPED IS NOT SAFE: "[tapped - cannot attack or block this turn]" says nothing about next turn, and
it untaps. Neither is "my blockers can handle it" - you have no blockers, ever, and no reasoning
that ends in one of yours blocking is about this deck.
A creature tagged [defender] still never gets the Path, ...
```
Plus one positive line added to DECIDING SITUATIONS (previously only the negative one was there).

---

## W2 - SWEEPERS: read the engine's attacker count, stop subtracting walls

**Finding.** 107 sweeper-offering windows, 13 casts. **4 of the 13 went into boards whose every
creature was `[defender]`** - `vs-ai_baka_deck126` seq 18 (turn 14, header *"of which 3 are
creatures, 0 of them without a restriction against attacking"*: Overgrown Battlement, Wall of Omens,
Pride Guardian), seq 36 (turn 24, header 2 / 0: two Perimeter Captains), seq 77 (turn 38, header 2 /
0: two Wall of Omens), seq 84 (turn 40, header 1 / 0: Pride Guardian). Wave 46 was 2 of 10; the rate
went **up**. The 0-creature-header half held at **0 casts in 64 windows** (W25's second half PASS).
**The new header is a perfect discriminator on this seat: all 4 violations read `0 of them without a
restriction against attacking`; all 9 correct casts read 1 or more (1,1,1,1,1,1,3,86 and one at
`att=1` beside a Shield Sphere).** 13/13.
**Diagnosis - STRATEGY.** R14a shipped the number the guide was asking the pilot to derive
(`AIPlayerGPT.cpp` `battlefieldHeaderText`/`boardCreatureCanAttackNow`, whose own comment cites this
seat), and the guide went on telling it to derive. Amendment 62: one rendered field replaces a
subtraction.

BEFORE (section head + gate):
```
=== SWEEPERS - THE HEADER COUNTS THE CREATURES FOR YOU ===
... "Opponent battlefield (11 permanents listed, of which 3 are creatures)". READ THAT NUMBER FIRST
BEFORE YOU USE THAT NUMBER, SUBTRACT THE WALLS. ... take off every creature tagged [defender] and
every creature showing printed power 0 ... The number you carry into the branches below is what is
LEFT after that subtraction ...
IF THAT NUMBER IS 0, ...
```
AFTER:
```
=== SWEEPERS - THE HEADER COUNTS THE ATTACKERS FOR YOU ===
... prints TWO numbers and this section uses only the SECOND: "Opponent battlefield (11 permanents
listed, of which 3 are creatures, 0 of them without a restriction against attacking)". That second
number is the engine's own count of their creatures that can come at you - walls and Defenders are
already taken off it. Subtract nothing yourself; you got that subtraction wrong four times in one
game. (On whichever board belongs to the player whose turn it is the same number is worded "N of
them able to attack right now"; use it the same way.)
You cast four sweepers into headers reading 3, 2, 2 and 1 creatures with that second number at 0
every time - four cards and four tapped-out turns for nothing. ...
IF THAT SECOND NUMBER IS 0, ...
```
The three DECIDING SITUATIONS header lines were rewritten onto the same field. The scope sentence
is exact: I read the emitter, and the two wordings are the same count under different knowability
scopes (`liveScope`), which is why both are named.

---

## W3 - LIGHTMINE FIELD: its own stop, not a cross-reference

**Finding.** 57 Lightmine windows, **3 casts, all three at an attacker count of 0**:
`vs-ai_baka_deck162` seq 7 (turn 6, `{leaves 0 of your 4 ... - casting this taps you out}`, Dream
Fracture in hand), `vs-ai_baka_deck126` seq 56 (turn 30), `vs-ai_baka_deck146` seq 14 (turn 11).
**Diagnosis - STRATEGY.** The whole gate was the sentence *"Its gate is the same header count you
already read for your sweepers - see that list"*, and DECIDING SITUATIONS carried only the POSITIVE
line ("3 or more creatures ... cast it"). A rule delivered as a pointer plus a positive-only
deciding line has no negative branch to fire.

BEFORE: `Its gate is the same header count you already read for your sweepers - see that list.`
AFTER: a stated hard stop keyed to the same second number, with the "below 3, never" line added to
DECIDING SITUATIONS beside the existing positive one, and the three cited casts as the cost record.

---

## W4 - THE TAP-OUT GATE: read `{leaves N ...}` off the option row

**Finding.** Every `Cast ...` row now carries `{leaves N of your M untapped mana sources untapped}`
or `{... - casting this taps you out}` (R14b; 336 of 545 cast rows on this seat - the rest are X
rows, which carry `{X pricing:}` instead). **Staff of Nin: 7 casts, 6 below the cheapest counter in
hand** - `vs162` seq 30 (`leaves 2`, Cancel + Fall of the Gavel), `vs126` seq 23 (`leaves 1`, Dream
Fracture + Essence Scatter), `vs123` seq 14 (`leaves 0`, Cancel), `vs123` seq 27 (`leaves 1`,
Cancel), `vs152` seq 13 and 16 (`leaves 0` each, Fall of the Gavel). The one clean cast, `vs146`
seq 22, is the one where the hand held no counter at all. Wave 44 6/11, wave 45 3-4/7, wave 46 4/7,
now **6/7** - four waves, no movement. Across the whole seat, **12 own-turn casts (excluding
counters answering the stack) left less mana than the cheapest counterspell in hand**: Staff 6,
Supreme Verdict 2, Path 2, Lightmine 1, Final Judgment 1.
**Diagnosis - STRATEGY, and the rule's shape is the defect.** It asked for a subtraction against a
line printed BEFORE the cast; the row now prints the answer AFTER it. The Staff branch had already
been collapsed to one constant (9) in wave 46 and still failed, which rules out the arithmetic being
the only problem: the constant was on the wrong surface.

BEFORE (both copies of the gate, and the Staff bullet):
```
... the spell you are about to cast must leave your "Mana available:" line reading 5 or more
AFTERWARDS.
- CAST STAFF OF NIN ONLY WHEN YOUR "Mana available:" LINE READS 9 OR MORE. ... Staff costs 6, and
  9 minus 6 is the 3 that keeps a Cancel or a Dream Fracture live.
```
AFTER:
```
... THE OPTION ROW PRINTS THE ANSWER: every "Cast ..." line ends with "{leaves N of your M untapped
mana sources untapped}", or "casting this taps you out" which is N = 0. N is what is left the
instant the spell resolves - read it off the row, do not compute it. If any counterspell at all is
in your hand, take the row only when its N is 5 or more. ... This gate does NOT cover a counter you
are casting at something on the stack - that is the mana doing its job.
- CAST STAFF OF NIN ONLY WHEN ITS OWN OPTION ROW READS "leaves 5" OR MORE. ... SIX OF SEVEN Staff
  casts this corpus broke it - at leaves 2, 1, 1, 0, 0 and 0 ... It has failed four waves running
  while it asked you to compute a number; it now asks you to copy one.
```
The two DECIDING SITUATIONS tap-out lines were rewritten onto the same field. The exemption for
counters answering the stack is new and is what makes the rule measurable: 11 of the 23 raw
violations were counterspells doing their job.

---

## W5 - X = 3: delete a sentence that the render has made FALSE

**Finding.** The guide said, in bold, *"the ask ... does NOT print a count of your own hand - so a
rule keyed to your hand size has no field to stand on"*. Commit 1c71d5301 shipped
`Your hand (N cards):` and it renders on **413 of 413** prompts on this seat. The sentence is now a
false statement about the render, which the trust doctrine forbids outright.
The rule it defends is at ceiling and is NOT touched (amendment 35): **12 X casts, 0 above 3**;
X = 3 in 8 of 8 windows where 3 was affordable and the menu maximum in the other 4 (max 1, 1, 2, 2).
The largest offered X was taken 0 times where anything smaller existed.

BEFORE: `There is nothing to work out here: the ask prints your library, it prints their hand size,
and it does NOT print a count of your own hand - so a rule keyed to your hand size has no field to
stand on, and this deck does not use one. Three is the number.`
AFTER: `Each row now prints what it buys - "X = 3 {X pricing: X=3 - you gain 3 life and draw 3
cards}" - and the X = 0 row says outright that the cast does nothing. Read the row, take the 3, and
stop. Twelve casts this corpus, none above 3: this rule works and there is nothing to work out.`

**Not done: the wave-46 sunset.** The wave-46 seat file said to retire the flat 3 the moment the
hand count landed. I am not retiring it. The subtraction rule it would restore conformed 4/17 last
wave; the constant conforms 12/12. Retiring a rule at ceiling to reinstate one that failed is a
regression I have no evidence for. W23 is adjudicated VOID by its own terms (the render landed) and
replaced by W30 below.

---

## W6 - THE STACK ROW HAS NO `[defender]` TAG

**Finding.** 34 counter-offering windows, 28 taken. **0 went to a mana artifact** (W26 PASS, was
2/39) and 0 to a `[defender]`-*tagged* body - but one went to a wall: `vs-ai_baka_deck126` seq 66,
turn 33, `Cast Cancel ... - can target on the stack: Perimeter Captain {w} (creature 0/4) {target
text: "Defender -- Whenever a creature you control with defender blocks, you may gain 2 life."}`.
**Diagnosis - PERCEPTION, and the surface is at fault.** The battlefield line prints `[defender]`;
the stack line and the counter-target clause print `(creature 0/4)` and the card's text and **no
keyword tag at all**. The guide's rule is keyed literally to "tagged [defender]", which is
unsatisfiable on the only surface where the decision is made. Routed to engine item **MED #2**; the
guide is re-keyed onto what that surface does carry.

BEFORE: `- (creature P/T) tagged [defender], at ANY printed power: LET IT RESOLVE.`
AFTER: `- a WALL, at ANY printed power: LET IT RESOLVE. ... RECOGNISE ONE ON THE STACK BY ITS TEXT,
NOT BY A TAG: the stack line and the counter row do not print [defender] ... A spell whose {target
text: ...} BEGINS with "Defender", or whose printed power is 0, is a wall.` (Perimeter Captain added
to the named list; the DECIDING SITUATIONS twin re-keyed the same way.)

---

## W7 - EMRAKUL OUTRANKS THE TAP-OUT GATE

**Finding.** 4 Emrakul windows, 1 taken (wave 46: 1 of 5). The three declines are
`vs-ai_baka_deck152` seq 86, 92, 93, and each row read `Cast Emrakul, the Aeons Torn {17} (15/15)
[from exile] {leaves 0 of your 17 ... - casting this taps you out}` / `{leaves 1 of your 18 ...}`.
The replies carry no reasoning at all - just the standing PLAN line.
**Diagnosis - STRATEGY, a rule collision the guide never resolved.** W8 hoisted Emrakul into
DECIDING SITUATIONS in wave 46 and it still failed, because the tap-out gate in the SAME section
forbids exactly the rows Emrakul always produces. Fifteen mana is this deck's whole board, so the
two rules can never both be satisfied. Naming the precedence is the fix; hoisting again is not.
Note also that the row read **{17}**, not the printed {15} - a cost-increase effect (deck152's Elite
Spellbinder shape) taxed it, and `Cast Supreme Verdict {3}{u}{w}{w} [from exile]` on the same menu
shows the same +2. The guide's "{15}" sentence is a printed-cost statement and stays; a line telling
the pilot to read the cost off the row was added.

BEFORE (end of RULE #1's Emrakul paragraph): `... because Emrakul finishes them sooner.`
AFTER: `... because Emrakul finishes them sooner. IT ALSO OUTRANKS THE TAP-OUT GATE, and it is the
only card in this deck that does: fifteen mana is your whole board, so its row always reads "leaves
0" or "leaves 1" and a gate asking for 5 could never pass it. Take it anyway. You declined it three
times in one game reading exactly those rows. Read its cost off the row too - a tax effect makes the
printed {15} appear as {17}, and the game only prints a cast you can pay for.`

---

## Considered and NOT edited (no bytes spent)

- **RULE #0, the land drop.** 93 windows, **93 taken, 0 declines** (wave 46: 40/40). Regression
  detector only; amendment 35.
- **The draw-is-damage stop.** 13 windows offered Sphinx's Revelation under a `DRAW PUNISHERS on the
  battlefield: theirs` line; **0 casts** (wave 46: 1/1, in a loss). The rule and lane J's render are
  both doing their job.
- **The counter-on-sight list.** 28 counters spent, 0 on a mana artifact, and the four
  draw-punisher permanents in the `vs162` game were each countered on sight (seq 15 Underworld
  Dreams, seq 22 Fate Unraveler, seq 28 Underworld Dreams). That game was still lost - to Howling
  Mines and a Dictate of Kruphix that resolved in windows where no counter was in hand. There is no
  guide edit in it.
- **Elixir.** 15 activation windows, 9 taken; minimum library across them **9**, so the
  `library <= 16` trigger finally had windows and did not misfire. Not touched.
- **Mulligan.** 6 offers, 1 mulligan (`vs130`, on the exact `would not cover any spell in it`
  branch), the re-keep at `(keeping 6)`, and one bottoming (`vs130` seq 3, an Island from a 4-land
  hand, leaving 3) that is guide-legal under rule 2 (duplicate) and clears the three-land floor.
  Nothing to pay for.
