# deck130 (Budde's Ponza, mono-red LD) - wave-47 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (= wave-46 revision,
byte-identical, verified). Three edits: 31,190 -> 32,386 chars (+1,196). Two of the three
replace existing text rather than add to it; only STEP 0 is a net addition.
Corpus: `matchups-20260826-182155`.

This seat had ONE decision-losing rule violation in 400 decisions. Everything else the wave-46
edits armed held. The edits below are (1) removing the one line in the guide that argued for
the violation, (2) a tripwire on the carried plan that produced it, (3) folding a manual scan
into the render line that now does it for you.

---

## EDIT D47-1 - KEY CARDS: delete "pick the smallest X", the sentence that caused the miss

**Finding.** `1787786534-ai_baka_deck130-0x562ec751eb60-vs-ai_baka_deck126.jsonl` seq 37,
ANNOUNCE_X for Starstorm, turn 11, life 20 vs 14. The menu (max affordable X=5):
```
1. X = 5 {X pricing: kills THEIRS: Pride Guardian x2, Perimeter Captain; YOURS: Rorix Bladewing}
2. X = 4 {X pricing: kills THEIRS: Pride Guardian x2, Perimeter Captain; YOURS: none}
3. X = 3 {X pricing: kills THEIRS: Pride Guardian x2; YOURS: none}
4. X = 2 {X pricing: kills THEIRS: none; YOURS: none}
...
```
It answered **option 3 (X = 3)**. Row 2 is a strict superset of row 2's kills for one more mana
with `YOURS: none`. Perimeter Captain (the lifegain wall) lived.

**Root cause is an internal contradiction in my own guide.** Rule #2 STEP 1 says
*"Find the row with the LARGEST 'kills THEIRS' list whose 'YOURS:' reads 'none'"* - correct.
But the KEY CARDS entry for Starstorm, 100 lines further down, said
*"Pick the smallest X that reaches the toughness you must kill."* X=3 is exactly the smallest X
that reaches Pride Guardian's toughness 3. The pilot followed the KEY CARDS sentence. Same shape
as wave 46's Blastminer contradiction (D46-4): a rule stated correctly in one place and
contradicted in the card section.

**Before**
```
... read each row's "X = " text and
answer that row's number. Pick the smallest X that reaches the toughness you must kill. Because it
kills your own Blastminers, ...
```
**After**
```
... read each row's "X = " text and
answer that row's number. DO NOT pick the smallest X that reaches the toughness you must kill -
that sentence used to live here and it is wrong; it is how this deck answered X=3 with an X=4 row
one line above it reading "kills THEIRS: Pride Guardian x2, Perimeter Captain; YOURS: none", and
let the Captain live for one mana. The X is chosen by rule #2 STEP 1 - the LARGEST "kills THEIRS"
list whose "YOURS:" reads "none" - and by nothing else. Because it
kills your own Blastminers, ...
```

---

## EDIT D47-2 - Rule #2: STEP 0, the carried plan does not choose X

**Finding, same window.** The pilot's carried plan at seq 37 read
`YOUR PLAN (as you last stated it): Cast Starstorm at X=3 to kill both Pride Guardians ...`
and its reply is that sentence **verbatim** with `CHOICE: 3 (X = 3)`. The X was fixed one screen
earlier, at the cast row, before the per-row kill lists existed. Then the announce screen
re-showed the plan and the plan answered it. This is the wave-46 M2 shape (the pre-game mulligan
chain answering its own carried plan) relocated onto the X menu, and it is the mechanism by
which D47-1's bad sentence reached the decision.

**Added, as STEP 0 above the existing STEP 1** (6 lines)
```
       STEP 0. IF YOUR CARRIED PLAN ALREADY NAMES AN X, IGNORE IT AND READ THE ROWS. The plan
               you wrote one screen ago was written before this menu existed, and the screen
               hands it back to you as "YOUR PLAN (as you last stated it)". This deck arrived
               here with "Cast Starstorm at X=3 to kill both Pride Guardians", answered X=3
               verbatim, and never read the X=4 row above it. An X in a plan is a guess; an X
               on a row comes with the kill list attached.
```
The render half of this is ENGINE M1 in the seat file - it costs the guide 6 lines and it costs
the render one marker.

---

## EDIT D47-3 - #0 RULE: read the summary line and its N; stop counting permanents by hand

**Finding (a render improvement, not a failure).** The wave-46 HIGH item H1 shipped. The line
renders at 19 windows at this seat:
```
DRAW PUNISHERS on the battlefield: theirs - Fate Unraveler. Every card YOU draw costs you 1
life to theirs. They fire on EVERY draw - the draw step, a cycling ability, a draw spell, any
extra draw - and the loss lands as the card is drawn, before anything can be done with it.
Count that cost before choosing to draw.
```
The rule as written still told the pilot to scan `{effect:}` text on each opponent entry and to
count PERMANENTS ("ONE such permanent ... TWO OR MORE ..."). The line already sums the price,
and the price is the thing that matters: one Ob Nixilis is 2 life, two Fate Unravelers are 2
life, and the permanent count is the wrong denominator for both. The rule shrinks and gets more
accurate at the same time.

**Before** (4 lines of manual scanning + 4 lines of permanent-count thresholds)
```
Scan the "Opponent battlefield" line for any permanent whose printed {effect:} text contains the
words "draws a card" together with damage or life loss to that player. In this pool they are
UNDERWORLD DREAMS, FATE UNRAVELER and OB NIXILIS, THE HATE-TWISTED, and their text is on their
own battlefield entry - you do not have to remember them, you have to read the line.
  - ONE such permanent on their line: cycle only when your life is above 5.
  - TWO OR MORE: DO NOT DRAW AT ALL. [...]
```
**After**
```
YOU NO LONGER HAVE TO SCAN FOR THEM, AND YOU NO LONGER COUNT THEM. The board frame prints its
own summary line whenever any are out, and it prints the PRICE, already summed: [the line]
Call that number N. If the line is not on the screen, there is no punisher and this rule does
not apply this window. [...]
Decide on N and your life, not on how many permanents are named:
  - N is 1 and "Your life:" is above 5: you may cycle.
  - N is 2 or more, OR N is at or above your life minus 5: DO NOT DRAW AT ALL. [...]
```
The wave-46 3-life Starstorm-cycling story stays under it: it is the reason the rule is #0.

---

## NOT edited, and why - the rungs that held

- **#0 RULE draw punishers (P7): PASS 0 of 19.** 19 windows carried the DRAW PUNISHERS line, a
  cycling row was on the screen at most of them, and the pilot drew at **none**. The best single
  piece of evidence is a window with NO punisher: `...vs-...deck146.jsonl` seq 53, life 3, it
  cycled Forgotten Cave and wrote *"The opponent has no draw punishers on the battlefield
  (Nadaar does not trigger on draw)"* - it is running the check, and it is running it correctly
  in both directions. Nothing to change.
- **#1 RULE cycling trap / the "Choose an option for Starstorm:" menu (P10): PASS 0 of 4.** Four
  modal windows (2 Starstorm, 2 Lay Waste). All four intended a cast and took `Cast Card
  Normally`; two carried plans said *"Do not cycle cards"* / *"Do not cycle Lay Waste"* in so
  many words. The wave-45 and wave-46 failure did not recur. The rule and the ENGINE H2
  annotation both shipped in the same binary, so I am not claiming which one did it - see the
  seat file.
- **X=0 (P-R15): PASS 0 of 2.** Both announce menus printed
  `X = 0 {X pricing: X=0 - this cast does NOTHING: it deals 0 damage and the spell is spent}`
  and neither was answered.
- **STEP 2 collapsed runs (P9): PASS 0 of 2.** No answer landed on a row whose next row read
  "same kills as X=N".
- **Dwarven Blastminer gate (P11): PASS 0 of 17.** 17 land-destruction activations; every one
  at four or fewer opponent lands on the printed line and every one at 15 life or above. The
  wave-46 contradiction (D46-4) is closed.
- **Blastminer attack floor.** 13 of 36 tagged attacker rows declined, all of them the lone
  `A1. Dwarven Blastminer (1/1) [no creature they control can block this attacker]` - the floor
  working as written, because his ability costs `{T}` and an attack spends it. Kept. This is
  also why lane K's "attack rate on tagged rows >= 90%" reads 64% at this seat; see the seat
  file, it is not a defect.
- **Starstorm quick-reference (DECIDING SITUATIONS).** Its four X entries already say "LARGEST"
  and were not the sentence that misfired; unchanged.
