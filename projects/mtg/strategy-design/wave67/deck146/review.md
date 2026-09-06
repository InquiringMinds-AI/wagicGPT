# Wave 67 - deck 146 (Orzhov Dungeons) per-deck review

Seat files (all under `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/`), record
counts and outcomes:

| game | seat file | result | turns | recs | fallbacks | hold windows skipped |
|---|---|---|---|---|---|---|
| 146v162 | `1788708403-ai_baka_deck146-0x55ba697172c0-vs-ai_baka_deck162.jsonl` | WIN 19 / -9 | 15 | 49 | 0 | 0 |
| 146v152 | `1788708409-ai_baka_deck146-0x55e0bb49a2d0-vs-ai_baka_deck152.jsonl` | LOSS 0 / 24 | 20 | 43 | 0 | 33 |
| 146v130 | `1788708427-ai_baka_deck146-0x55ddf9b3f330-vs-ai_baka_deck130.jsonl` | WIN 14 / 0 | 33 | 60 | 0 | 46 |
| 146v126 | `1788708429-ai_baka_deck146-0x557714edd100-vs-ai_baka_deck126.jsonl` | WIN 15 / 0 | 29 | 99 | 0 | 32 |
| 146v125 | `1788708434-ai_baka_deck146-0x56285ffe51b0-vs-ai_baka_deck125.jsonl` | LOSS 0 / 51 | 46 | 144 | 0 | 314 |
| 146v123 | `1788708438-ai_baka_deck146-0x55b565a661c0-vs-ai_baka_deck123.jsonl` | LOSS 0 / 16 | 16 | 39 | 0 | 14 |

Deck record 3/6. Seat totals: 416 model replies, **0 fallbacks, 0 `unparsed_reply`, 0
`label_missing_*`, 0 reveal records, 0 `repeat_past_stop`** (the own-stop clause `right now: M=`
never rendered for this seat at all). Answer-first **413/416 = 99%**; 7 replies carried a
`[+N bytes written past the PLAN line trimmed]` marker, 25,023 bytes total.

Mulligans: **6/6 untouched sevens kept**, 3-4 counted lands each, zero ships and zero bottom asks
across the whole wave. The guide's mulligan block (its single largest section) had no error to
report in any of the six games.

---

## 1. Game by game - the deciding decisions

### 146v125 - LOSS 0 / 51, turn 46 (the flagged lopsided finish)

Deck 125 is UW control: 2x Staff of Nin, recurring Elixir of Immortality, Supreme Verdict, Final
Judgment, Path to Exile, Cancel, Essence Scatter, 2x Fall of the Gavel, Sphinx's Revelation.
**The seat declared attackers TWICE in 46 turns** (seq 31, turn 25, Nadaar alone for 3; seq 117,
turn 41, an animated Hive). Its own life went 20 -> 0 almost entirely to Staff of Nin upkeep pings
(15 of them visible in the untrimmed tail of the seq 110 log). The opponent went 20 -> 54.

Deciding decisions, in order:

- **seq 16 (turn 13) - gave the opponent a card.** Silverquill Command mode ask. Row 1 was
  `return creature and you draw`, row 2 `return creature and opponent draws`; the seat took row 2
  and wrote `PLAN: Opponent loses 1 life and draws a card.` The two rows are the same mode pointed
  in opposite directions. STRATEGY.
- **seq 19/20 (turn 15) - did it again, worse.** Only two rows, both HALF DEAD (the opponent had no
  creature to sacrifice): row 1 `you draw and sacrifice creature`, row 2 `opponent draws and
  sacrifice creature`. The seat took row 2, so it spent a card to give a control opponent a card
  and take 1 life off them. Its own reply argued the sign backwards: *"This is a 2-for-1 in card
  advantage (I spend 1 card, they draw 1)."* STRATEGY. (seq 19 also cost **866,311 ms** - see
  ENGINE HIGH-3.)
- **seq 22 / 24 (turns 17, 19) - correct passes.** Both casting menus offered only a dead Soul
  Shatter against an empty board; the two Lolths in hand needed `{3}{b}{b}` and the seat's only
  black source was the Hive. Reading them as misplays would be wrong: this was a genuine black
  screw, and the render said so exactly.
- **seq 33-130 - the manland was never turned on.** From turn 27 the seat had 9-14 untapped
  sources, an empty opponent board, and the `becomes beholder` row on the menu 52 times. It was
  **on an UNTAPPED Hive twelve of those times and was declined every one**: seq 36 and 40 (turn 27,
  9 sources), seq 47 and 48 (turn 29, 10 sources) - on both of those turns the seat cast NOTHING at
  all - then seq 58, 73, 93, 104, 109, 125, 130 at upkeep with 11-14 sources. The one it finally
  took (seq 113, turn 41) attacked at seq 117. Against an opponent that controlled zero creatures
  from turn 26 to the end, a 3/3 menace was the seat's only clock and it ran for one turn out of
  twenty. STRATEGY - and the live guide's Hive ladder actively argues for the decline (see the
  guide verdict).
- **seq 95 (t37) and seq 105/106 (t39) - one spell a turn into 14 open blue mana**, with two Fall of
  the Gavel already visible in `Their graveyard`. Each counter gained the opponent 5: 10 of their
  54 life came from countering the seat's own spells. The guide's ONE SPELL A TURN INTO OPEN BLUE
  MANA LOSES rule exists and was followed on turn 35 (three spells) but not on 37/39. STRATEGY,
  covered by existing text - no edit.
- **seq 50/51 (turn 31) - legend rule on a render that names neither the rule nor the loyalty.**
  See ENGINE MED-1. Outcome was fine (kept the fresh 4-loyalty copy) but for a reason the model did
  not have.

Honest scope: the seat has **no artifact removal that could reach a Staff of Nin** (Vanishing Verse
needs a monocolored permanent, Soul Shatter needs a creature or planeswalker), and its only answer
per the guide - Kaya's -3 - was already in the graveyard from turn 11, before the first Staff
landed on turn 22. A large part of this loss is deck-vs-deck. What was in the seat's hands was the
clock: twelve declined animations and two donated cards.

### 146v123 - LOSS 0 / 16, turn 16

Deck 123 assembled Intruder Alarm plus a Human token engine and went from a normal board to **30
Humans** between turns 13 and 15. The seat was at 24 life with the opponent at 16 on turn 15.

- **seq 30-33 (turn 15) - two Silverquill Commands, both on `return creature and sacrifice
  creature`,** against a board reading `31 are creatures`. Each edict removed one 1/1 of 31. The
  seat's own plan at seq 30 read *"opponent sacrifices Thraben Doomsayer"* - a choice that belongs
  to the opponent, who had thirty 1/1s to give instead. See ENGINE HIGH-1: on this menu the
  sacrifice half printed only `{this mode has a legal object right now}`, with no count and no
  "they choose".
- **seq 37 (turn 16) - the block.** `INCOMING THIS COMBAT: 29 attackers ... best case with every
  blocker assigned: you would be at 0; no block saves you`. The seat took the engine's own
  least-damage assignment. The render was correct and complete; the decision was already lost.

Nothing in the seat's hand answered 30 tokens (its sweeper-shaped cards are all single-target).
This is largely a deck loss; the Command mode picks were the only controllable part and they were
worth about 2 damage.

### 146v152 - LOSS 0 / 24, turn 20

An aggro race the seat blocked its way through (seq 22, 29, 33, 41 all blocks) while spending
removal on curve (seq 8 and 11 Soul Shatters taking Brutal Cathar and Sigarda). It attacked once
(seq 21). At seq 36 (**440,881 ms**, 2,338 bytes of overrun) it cast Silverquill Command and at
seq 37 took `return creature and sacrifice creature` - a real edict here, the opponent's board was
small. Life went 21 -> 12 -> 0 over turns 15-20 to a wide board it could not block. No misread
surface found; the guide's blocking ladder was followed at every block.

### 146v162 / 146v130 / 146v126 - WINS

These three show the plan working and are the reason the guide is largely a KEEP. The shape is
identical in all three: land on curve, remove on curve, attack **every** turn a body is untapped,
and use the planeswalker as removal.
- 146v162: Kaya -3 exiled Shield Sphere (seq 24) and Fog Bank (seq 39) - the two blockers - then
  attacked at seq 25, 31, 46 and won on turn 15. At **seq 43 it took `creature gains 3/3 and you
  draw`** - the same draw-direction question it got wrong twice in 146v125, answered correctly
  here. The choice is not stable without a rule.
- 146v130: attacked at seq 30, 41, 46, 58; Silencer named `Starstorm {visible now: 2 in their
  graveyard}` (seq 51), exactly what the guide's counterspell-naming rule asks for.
- 146v126: 8 attackers windows in 29 turns, 26 priority actions, 1 `identical_option_asks_resolved`.

---

## 2. Engine / interface / card items

### HIGH-1 - Silverquill Command's sacrifice half prints no edict floor, while every other edict in the corpus does
`1788708438-ai_baka_deck146-0x55b565a661c0-vs-ai_baka_deck123.jsonl` seq 31, turn 15. The
CURRENT SITUATION block reads `Opponent battlefield (39 permanents listed, of which 31 are
creatures ...)` and the menu prints:

```
4. creature gains 3/3 and sacrifice creature {this mode has a legal object right now}
7. return creature and sacrifice creature {this mode has a legal object right now}
```

The corpus already renders this fact for other edicts - e.g.
`{right now: they control 5 creatures - they choose which one - you gain at least 1 (their
smallest toughness, and 4 of their 5 creatures are at it - the pick is theirs, so plan on the
floor)}`. Silverquill Command's sacrifice half gets none of it: no count, no "they choose", no
floor. The consequence is on the record - the seat's plan one window earlier (seq 30) named the
card it expected them to sacrifice (`Thraben Doomsayer`), and it then spent BOTH Commands (seq 31
and seq 33) on that mode against thirty 1/1s. Repro: give the Command's sacrifice modes the same
`they control N creatures - they choose which one` clause the standalone edicts carry. This is the
highest-value single render fix visible from this deck.

### HIGH-2 - the "opponent draws" modes are annotated as a benefit, never as a cost
`...vs-ai_baka_deck125.jsonl` seq 16, row 2, verbatim:

```
2. return creature and opponent draws {this mode right now: they LOSE 1 life - they would be at
   19; they draw 1 (life and draws only: anything else this mode does is in the row label and the
   card text)} {this mode has a legal object right now}
```

The annotation leads with a number that is good for the seat (`they LOSE 1 life`) and reports the
card as a bare fact (`they draw 1`). The cast row's DRAW GRANT bracket (seq 19) is worse: it prices
the draw only as something that might hurt *them* - `converters on your battlefield: 0 (nothing of
yours punishes their draws, so those 1 draws take 0 off them)` - so the only framing on the screen
says the draw is worth ZERO, when what it actually is, is a card handed to the opponent. The model
read it exactly that way twice (seq 16, seq 20) and wrote `"a 2-for-1 in card advantage (I spend 1
card, they draw 1)"`. `#W47-R3` already established the pattern for this - a parenthesis naming
life THEY gain is "the PRICE of that outcome, not part of it". The same sentence is owed to a card
they draw. Repro: on any mode/row that grants the OPPONENT a draw, print the draw as a price
(`this hands them 1 card`) and, where a mirror row exists, name it (`row 1 is this same mode
pointed at you`).

### HIGH-3 - post-answer overrun is this seat's whole latency tail, and it came within 34 s of the 900 s wall
The seat's 7 overrun-trimmed replies are **exactly its 7 slowest decisions**, in order:

| bytes past the answer | file | seq | latency |
|---|---|---|---|
| 9,333 | `...vs-ai_baka_deck123` | 36 | 615,286 ms |
| 5,675 | `...vs-ai_baka_deck125` | 19 | **866,311 ms** |
| 2,376 | `...vs-ai_baka_deck125` | 120 | 60,122 ms |
| 2,338 | `...vs-ai_baka_deck152` | 36 | 440,881 ms |
| 2,269 | `...vs-ai_baka_deck126` | 55 | 222,335 ms |
| 1,873 | `...vs-ai_baka_deck125` | 110 | 57,628 ms |
| 1,159 | `...vs-ai_baka_deck162` | 42 | 243,681 ms |

Median latency over the other 409 replies is under 25 s. The 866 s record left 34 s of margin
against the 900 s curl wall, which lane AX measured as having zero retry budget. The overrun text
at seq 19 is visible in the record and is the model arguing with itself about a mode census
(`"Actually, let's re-read the menu carefully."`) after it had already written its CHOICE line.
Whatever the protocol does about overrun, this seat's evidence is that overrun bytes are the
latency, and the latency is the wall. Answer-first compliance is not the problem - it is 99%.

### MED-1 - the legend-rule ask names neither the legend rule nor either copy's loyalty
`...vs-ai_baka_deck125.jsonl` seq 51, turn 31, verbatim head and rows:

```
TARGET CHOICE for Lolth, Spider Queen - its "put a card into the graveyard" ability (this
spell/ability is already on the stack and needs a target ...)
1. Lolth, Spider Queen #1 [planeswalker] [your battlefield] - "Whenever a creature you control
   dies, put a loyalty counter on Lolth, Spider Queen. -- 0: You draw a card and..."
2. Lolth, Spider Queen #2 [planeswalker] [your battlefield] - "Whenever a creature you control
   dies, put a loyalty counter on Lolth, Spider Queen. -- 0: You draw a card and..."
```

The two rows are byte-identical apart from the handle. **Neither prints loyalty** - although the
battlefield line in the same corpus does (`Lolth, Spider Queen ... [counters: 2x loyalty]`, seq 66)
- and nothing says this is the legend rule or that the chosen row is the one that DIES. The model
answered `CHOICE: 2` and then wrote `PLAN: ... sacrificing the first (Lolth #1) to the legend rule.
Lolth #2 enters with 4 + 4 = 8 loyalty counters.` Both halves are false: the loyalties do not add,
and the copy it named as the survivor is the one it had just targeted. Repro: on a legend-rule
pick, head the ask with "legend rule: the copy you name is put into your graveyard" and print each
copy's loyalty on its row. The choice is between two numbers and the numbers are not on the screen.

### MED-2 - "NO LIVE CAST ROW ON THIS MENU" does not say what the seat can still do
`...vs-ai_baka_deck125.jsonl` seq 22 and seq 24 (turns 17, 19), rendered line:
`NO LIVE CAST ROW ON THIS MENU: all 1 cast row below carry a verdict computed from the board that
reads zero`. Both windows are correct and both are answered correctly. What the screen never says
is that the casting seam is not the whole turn: a land drop, a Hive animation, an attack and a
planeswalker activation are all still available and all live at other seams. The deck guide has to
carry that sentence itself (it does, under DRAW PRICE), which is a sign it belongs on the screen.
Low cost, and it is the seam where this deck's "I have nothing to do" turns are decided.

### LOW-1 - `Their graveyard` (lane AY's I8) rendered on every window and no decision cited it
The line is present and correct on every prompt in all six games (e.g. seq 110: `Their graveyard
(9 cards): Essence Scatter {1}{u} x2; Lightmine Field {2}{w}{w} x2; Cancel {1}{u}{u}; Supreme
Verdict {1}{u}{w}{w}; Fall of the Gavel {3}{u}{w} x2; Final Judgment {4}{w}{w}`). Across 416
replies from this seat, **no reply text names a card because it saw it in the opponent's
graveyard** - including seq 105, where the seat cast a 7-mana Emeria's Call into open UUW with two
Fall of the Gavel printed in that exact line one screen earlier. Reported as a measurement, not a
defect: the fact is on the screen, the seat's own Silencer naming does read it (`Essence Scatter
{visible now: 2 in their graveyard}`, seq 18; `Starstorm {visible now: 2 in their graveyard}`,
146v130 seq 51), so the render works where a row points at it and is inert where nothing does.

### LOW-2 - the beholder row's rung-ceiling clause is correct and load-bearing
`{rung ceiling: Hive of the Eye Tyrant's OWN mana is not counted toward this activation - tapping
it for mana leaves it tapped ...}` (seq 66, seq 113). Verified against the primitive
(`borderline.txt:53394`, `auto={3}{B}:name(Becomes beholder) ...`). At 146v125 seq 22/24 the seat's
only `{b}` source WAS the Hive, so the row was correctly absent from those menus. No defect - noted
because it is the reason two suspicious-looking do-nothing turns are not misplays.

---

## 3. Guide verdict: EDIT

The guide is mostly working and most of it stays byte-identical. 6/6 mulligan keeps, correct
Silencer naming in two games, correct removal-on-curve in three, correct block ladder in 146v152,
correct dungeon and room picks throughout, and three wins on the plan the guide describes. Two
places are paid for by decisions in the corpus, and one of them is a rule the guide currently gets
backwards.

Revised guide: `strategy.txt` in this directory. **71,482 bytes** (`stat -c %s`), inside the
41,000-71,500 band; live file was 70,997.

### EDIT 1 - Hive ladder: add CHECK 0b above CHECK 1

BEFORE (the ladder ran CHECK 0 -> CHECK 1 -> CHECK 2 -> CHECK 3, and CHECK 1 reads):
```
  CHECK 1 - WHAT PHASE IS IT? The prompt's "Phase:" line says. If it says Upkeep and CHECK 0 did
  NOT fire, the answer is PASS.
```
Three of the four checks are reasons to PASS, and CHECK 0 fires only when the animation is lethal
THIS turn. Against an opponent at 28-54 life with zero creatures, CHECK 0 can never fire, so the
ladder as written switched the deck's only remaining win condition off permanently.

AFTER (inserted immediately above CHECK 1):
```
  CHECK 0b - IS IT YOUR ONLY CLOCK? Read their battlefield line's "of which N are creatures". At
  N=0 with your own line showing 0 able to attack, the animated Hive is the only damage you can
  deal, and a 3/3 menace facing no blockers is 3 unanswered damage: ANIMATE, at any phase, unless
  the row is tagged [this land is TAPPED] or the mana it spends is mana you need for a cast you
  will actually make this turn. This outranks CHECK 1, because at N=0 CHECK 0 can never fire and
  CHECK 1 alone switches your only win condition off for the whole game. WHAT THIS COST: at
  146v125 the opponent controlled ZERO creatures for 46 turns and this seat declined the beholder
  row on an UNTAPPED Hive twelve times - seq 36 and 40 (turn 27) and seq 47 and 48 (turn 29), each
  with 9-10 untapped sources and nothing cast that turn, then seq 58, 73, 93, 104, 109, 125, 130
  at upkeep with 11-14 sources. It attacked twice in the whole game and lost 0 to 51.
```
PAID BY: `...vs-ai_baka_deck125` seq 36, 40, 47, 48 (main phase, untapped Hive, 9-10 sources,
nothing else cast either turn) and seq 58, 73, 93, 104, 109, 125, 130. CHECK 3's tapped-Hive PASS
is preserved verbatim and is explicitly re-stated inside CHECK 0b, because 40 of the 52 offers in
that game genuinely were on a tapped Hive and the row said so.

### EDIT 2 - a draw-direction rule in DECIDING SITUATIONS

BEFORE: the guide had no rule on which way to point a Silverquill Command draw mode. The nearest
text was inside cast-order entry 5 and was conditional - *"Never take an 'opponent draws' pair
while another pair is live"* - which does not reach seq 20, where BOTH offered rows were half dead.

AFTER (new bullet, immediately under the existing "pick YOUR OWN creature" bullet):
```
- A Silverquill Command mode names a DRAW and the menu offers that mode pointed both ways ("...
  and you draw" / "... and opponent draws") -> TAKE THE ONE THAT SAYS YOU DRAW, always. The row
  prints the 1 life first ("they LOSE 1 life - they would be at 19; they draw 1") and that life is
  the small half; the CARD is the big half, and pointing it at them hands a card to the player you
  are trying to run out of answers. WHAT THIS COST: 146v125 seq 16 took "return creature and
  opponent draws" with the identical you-draw mode sitting on row 1, and seq 20 took "opponent
  draws and sacrifice creature" on a HALF DEAD row, so it bought nothing but their card - the
  reply called it "a 2-for-1 in card advantage (I spend 1 card, they draw 1)", which has the sign
  backwards. No board state in this deck makes an opponent draw right.
```
PAID BY: `...vs-ai_baka_deck125` seq 16 and seq 20 (both wrong) against `...vs-ai_baka_deck162`
seq 43 (right) - the seat answers this question inconsistently and the guide never told it which
way. The conditional sentence in entry 5 is replaced by a pointer to the new bullet so the rule
has one home.

### Byte budget - what was compressed to pay for the additions

The two additions are +1,634 bytes. To stay in band, four passages were compressed with no rule
removed, and one measurement note was deleted:
1. Mulligan STEP 2's two-reason bracket paragraph - the colour-bracket text argued the same
   conclusion three times; the rule (count bracket = the one-land test; colour bracket = never a
   ship, at any scope) is intact.
2. Cast-order entry 5's "returns NOTHING" paragraph - shortened, quote kept.
3. "EVERY CARD YOU CAN CAST HAS A LINE ABOVE" - said "go back and take the first line that matches"
   twice; now once.
4. The Hive ladder's "AND THE OPTION COMING BACK" paragraph - 5 lines to 3, rule unchanged.
5. DELETED: the wave-55/57 HOLD-row measurement note ("190 windows ... The number is the
   measurement, not the story"). It carries no rule, and this seat's HOLD behaviour needed no
   correction in any of the six games.

---

## 4. Not checked / limits of this review

- I did not read the opponents' seat files end to end. I used `1788708434-...deck125-...-vs-...
  deck146` only through the shared game log rendered inside my own seat's prompts, and did not
  audit deck 125's, 123's or 152's own decisions.
- I did not adjudicate the lane predictions - that is the engine seat's job. Where a lane-flagged
  counter is zero for this seat I said so as a count, not as a verdict, and I did not check the
  corpus-wide 1233 s record (it is not in a deck146 file; this seat's max is 866,311 ms).
- I did not verify the `hold_windows_skipped` totals against stderr, and I did not check whether
  the 314 skipped windows in 146v125 hid anything: I confirmed from `AIPlayerGPT.cpp` that
  `holdHonoured` is called only at the `priority` (line 32353) and `cast` (line 35619) seams, so it
  cannot suppress an attackers window, and the 52 beholder offers in that game show the priority
  seam did keep re-opening.
- Card facts verified against the primitives only where a decision turned on them: Hive of the Eye
  Tyrant (`bin/Res/sets/primitives/borderline.txt:53391-53396`) and Silverquill Command
  (`:102739-102752`). Silverquill Command's Wagic modes are a faithful expansion of the printed
  "Choose two" - the "Target player draws a card and loses 1 life" clause really is pointable at
  either player, so seq 16 / seq 20 are strategy errors and not card-script bugs. I did not
  re-check them against Scryfall.
- No general-guide or skill proposals: nothing here generalises past this deck's cards. The
  draw-direction error is about a card only deck 146 plays, and the manland ladder is deck 146's
  own text.
