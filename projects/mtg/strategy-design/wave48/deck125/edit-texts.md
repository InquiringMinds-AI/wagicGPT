# deck125 (Revelation Fracture) - wave-48 edit texts

Base = live `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` (wave-47 W1-W7 + boundary pass).
Output = `wave48/deck125/strategy.txt` (34,338 -> 37,803 bytes). Every edit below cites the seq
that pays for it; corpus root `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-072004/`, seat
files `...-ai_baka_deck125-0x...-vs-ai_baka_deckN.jsonl`. Numbering continues wave 47 (W1-W7).

## W8 - FORCED FRUITION: a new section (the vs162 loss)
Finding: `vs162` (t29 loss). Forced Fruition resolved on their t13 with no window offered (the seat
had 4 open after Path at seq 13; Fall of the Gavel costs 5). From then on every spell the seat cast
drew it seven. With Underworld Dreams beside it from t21, each spell was 7 damage: seq 44 (t26,
life 15) cast Elixir of Immortality -> drew 7 -> 20 -> 13; seq 51 (t29, life 11) cast Cancel at
Dictate of Kruphix under TWO Fruitions + Dreams -> drew 14 -> dead. The draw-is-damage stop covers
optional draws only; nothing in the guide covered "casting anything draws seven". STRATEGY (guide
omission) against a complete render (`Forced Fruition #1-#2 x2 ... {effect (each copy of this card
does this): "Whenever an opponent casts a spell, that player draws seven cards."}` on the line).
Primitive verified: `auto=@movedTo(*[-land]|opponentstack):draw:7 opponent` - only a CAST moves a
card onto the stack, so an Elixir ACTIVATION is Fruition-free (narration confirms: seq 45's
activation drew nothing; seq 44's cast drew seven).
BEFORE: (no section)
AFTER: new section `=== FORCED FRUITION - EVERY SPELL YOU CAST DRAWS YOU SEVEN ===` inserted before
the Elixir section (three branches: Fruition + punisher -> cast nothing on own turn, counter only
when 7 x Fruitions x punishers is below life - 1; Fruition alone -> library floor 14 and refill by
Elixir activation; the answer is the counter while it is on the stack), plus one DECIDING
SITUATIONS line.

## W9 - SWEEPERS AT 0: the shapes the four casts took, and the spend-the-mana line carved out
Finding: W31 FAIL 4/8 casts (95 sweeper windows). All four at a header second number of 0:
`vs130` seq 45 (t28, 0 creatures on their board at all), `vs130` seq 72 (t40, same), `vs126` seq 46
(t22, five walls, `5 are creatures, 0 of them without a restriction`), `vs162` seq 21 (t18, main
phase 2, after the same turn's main-phase-1 window seq 20 was correctly declined). Plus W32 FAIL
1/44: `vs123` seq 40 Lightmine Field at 0. Every reply was the bare plan line - no reasoning; each
cast came after several correct declines, and in 3 of 4 the sweeper was the only cast row beside
"Cast nothing". The DECIDING SITUATIONS line "Nothing in your list answers anything ... spend it on
Staff, Elixir, Revelation rather than passing" is the pursuit the executor generalised.
BEFORE (hard line): "...This line outranks every other reason to cast a sweeper - being ahead,
having spare mana, wanting to use the turn, the plan naming a wipe. A wipe with nothing..."
AFTER: adds "It outranks the turn count too: the tenth window ... same answer as the first, main
phase 2 ... same as main phase 1, and a menu whose only cast row is the sweeper still answers
'Cast nothing right now' ... Four sweepers this corpus went into a 0, and every one took one of
those shapes..."
BEFORE (situations): "...spend it on Staff of Nin, on Elixir, or on Sphinx's Revelation rather than
passing the turn with it unused - the tap-out gate ... still binds, and so does the draw-is-damage
stop."
AFTER: "+ NEVER on Supreme Verdict, Final Judgment or Lightmine Field: those three are spent only by
the header number, and a turn that ends with mana unused is the right turn when that number is 0."
BEFORE (situations, 0 line): "...at any life total and with any spare mana. '3 creatures, 0 ...' is a
board of walls and no card of yours is worth spending on it."
AFTER: "...at any life total, with any spare mana, in either main phase, however many turns it has
been offered, and when it is the only cast row on the menu. ... '0 creatures' is an empty board; no
card of yours is worth spending on either."

## W10 - THE TAP-OUT GATE: put the number in the plan line; exempt the Elixir CAST
Finding: W33 FAIL again (10 identifiable own-turn spends below "leaves 5" with a counter in hand;
Staff sub-lane 2/6, was 6/7). The one that decided a game: `vs162` seq 27 (t20) Staff of Nin at
`leaves 2` holding Essence Scatter, Cancel and Fall of the Gavel, then seq 29 Elixir activation to
0 open; on their t21 Underworld Dreams resolved with NO window offered (the seat could not pay for
one - no record between seq 29 and seq 30, and Dreams is on the line at seq 30). Every reply on
this seat copies the guide's plan sentence verbatim (406/406 carry a PLAN or inherit one), so the
plan line is the one guide sentence guaranteed to be in front of the model at every cast ask.
Also: of the 10 violations, 6 are Elixir CASTS at leaves 0-4 (`vs152` 14, `vs126` 10 and 20,
`vs123` 5, `vs162` 3 and 26) - a {1} spend the gate was never meant for and that never cost
anything; a rule broken six times for free teaches the executor the rule is optional.
BEFORE (plan, two sites): "Answer their threats, resolve Staff of Nin, ping their face every turn."
AFTER: "Answer their threats, resolve Staff of Nin from a row reading leaves 5 or more, ping their
face every turn."
BEFORE (gate paragraph, two identical copies): "...a Sphinx's Revelation, a Staff of Nin, an Elixir
of Immortality cast OR activated, a Lightmine Field."
AFTER: "...an Elixir of Immortality ACTIVATION, a Lightmine Field. The one spend it does NOT cover is
the Elixir CAST: {1} for a card that then waits on your battlefield is always paid, at any N, the
turn it is listed; the gate governs its {2} activation."
BEFORE (Staff bullet anecdote): "SIX OF SEVEN Staff casts this corpus broke it - at leaves 2, 1, 1,
0, 0 and 0 ... it now asks you to copy one."
AFTER: "Two of six Staff casts this corpus broke it, at 'leaves 2' and 'leaves 0' ... the 'leaves 2'
one was followed, on the very next turn, by an Underworld Dreams resolving with no window offered
... The plan line now carries this number so it is in front of you at every cast ask."

## W11 - THE STACK ROW NOW CARRIES [defender]: re-key W6 to the tag, delete the false sentence
Finding: lane P shipped keyword tags on stack rows and counter-target clauses. Emitter string as
rendered: `vs126` seq 15 `can target on the stack: Perimeter Captain {w} (creature 0/4) [defender]
{target text: "Defender -- ..."}`; same at seq 30, 33, 37, 47 (Pride Guardian, Wall of Omens). The
live guide says "the stack row does not carry the [defender] tag" - now a false claim about the
render (trust doctrine: shed it) - and told the model to key to the tag "when it starts printing".
Behaviour: 0 counters spent on those 5 tagged rows (W36 PASS 0/27 counters).
BEFORE: "RECOGNISE ONE BY WHAT THE ROW DOES CARRY: a spell whose {target text: ...} BEGINS with
'Defender', or whose printed power is 0, is a wall. You spent a Cancel on ... When the stack row
starts printing [defender], key this rule to the tag instead."
AFTER: "RECOGNISE ONE BY ITS TAG: the stack line and the counter row print it - 'Perimeter Captain
{w} (creature 0/4) [defender]'. A stack row tagged [defender], or printing power 0, is a wall. Five
such rows this corpus, zero counters spent on them - keep it that way."
BEFORE (situations): "Their spell on the stack prints (creature P/T) whose {target text:} begins with
'Defender', or prints printed power 0: let it resolve. Walls never get a counter - and the stack
row does not carry the [defender] tag, so the text is where you read it."
AFTER: "Their spell on the stack is tagged [defender], or prints printed power 0: let it resolve.
Walls never get a counter."

## W12 - PATH: a textless 1/1 token is never a target above 12 life
Finding: W34 second half: `vs146` seq 9 (t11, life 20, `leaves 5`) and seq 31 (t21, life 20,
`leaves 10`) both cast Path to Exile at `Goblin (creature 1/1)` - the only candidate, no
{target text:} at all. The rule ("every candidate reads power 0 or 1 -> cast nothing above 12")
already forbids it; the token case is named so the executor stops treating "the only target" as
"a target". Zero windows at life 12 or below offered a real Path target, so W34's first half is
UNTESTED. STRATEGY.
BEFORE: "...A 0/4 wall is never a Path target."
AFTER: "...A 0/4 wall is never a Path target, and neither is a token with no {target text:} at all -
'Goblin (creature 1/1)', a Human, a Wolf: you Pathed a lone Goblin token twice at 20 life with ten
lands open."

## W13 - MULLIGAN: odds-based guidance replaces the STEP 1 threshold and the three bottoming floors
Finding (my call, under the owner's standing ruling that deck strategy is the reviewer's and that
guide text must not impose mulligan floors or forced keeps as code-like constraints): `vs126` seq
1-5. seq 1 `0 lands, 7 spells` -> mulligan (right). seq 2 `4 lands, 3 spells`, coverage names Dream
Fracture and Supreme Verdict, row `Mulligan (a keep after this one would keep 5 cards)` ->
mulligan (the KEEP clause covered it; the STEP 1 text keyed on "(keeping 5)", a string the row
does not print). seq 3 `2 lands, 5 spells`, no coverage, at the would-keep-4 row -> mulligan. seq
4 kept `3 lands, 4 spells` bottoming 3; seq 5 sent Island, Fall of the Gavel, Essence Scatter and
kept Glacial Fortress, Plains, Elixir, Path - a hand with no counter that then watched Sanguine
Bond and Exquisite Blood resolve (seq 18, 29) and lost at t25. At that ask the three floors ("never
below three lands", "never your last counterspell", "never your last Path") could not all be met
with three cards to send from 3 lands + Elixir + Path + Gavel + Scatter: the rule set was
unsatisfiable exactly where it was needed, and the executor, unable to satisfy it, chose freely.
BEFORE: STEP 1 / STEP 2 ("(keeping 5) or ANY lower number: KEEP, if the hand holds at least one
land ... do not read another rule"), the KEEP/THINK AGAIN block, and the bottoming list ending
"NEVER bottom a land when the hand you keep would hold fewer than THREE. NEVER bottom your last
counterspell and never bottom your last Path to Exile...".
AFTER: one odds framing ("what are the odds THIS hand does something in time, against the odds the
NEXT hand - one card smaller - does"), the price row quoted as the engine prints it ("would keep 5
cards"), three reasoned branches (2-5 lands with coverage: keep at any price; no coverage: weigh
draws-to-a-cast against the next look's cost, the small-hand branch keeping anything with a land
and a spell castable within two draws; all-land / all-spell: ship at any price), and a bottoming
PREFERENCE order plus the shape of the hand to keep ("its lands and at least one answer; when the
ask takes more than that, keep the lands and one answer") - satisfiable at any bottom count, with
the seq 5 hand worked as the example (send Gavel, Elixir, Path; keep three lands and Scatter).
deck125's DECIDING SITUATIONS has no mulligan line, so nothing there to change. Falsifier W45.

## Considered and NOT edited (no bytes spent)
- **X = 3 (W30)** - 8/8 at ceiling; not touched (#35).
- **Land drop (W37)** - 84/84; not touched.
- **Emrakul (W35)** - 2/2 taken at `leaves 1` and `leaves 2`; W7 held; not touched.
- **Draw-is-damage stop (W27 lineage)** - 0 Revelations in 9 windows under a `DRAW PUNISHERS` line;
  not touched.
- **Counters at land destruction (`vs130` seq 15, 39, 53)** - the guide is silent on Stone Rain /
  Molten Rain / Lay Waste; the seat countered them and won at t54. Correct play, no rule needed.
- **Elixir library floor** - min library 5 at `vs162` seq 42 (activated in that window, `library <=
  16` fired 1/1); the floor works, not touched.
- **Dream Fracture `[DRAW PRICE:]`** - the tag now reaches its row (`vs162` seq 38); the counter-on-
  sight rule already outranks the draw price for the named punishers; no line added.
