# deck126 -> core-prompt / render proposals (wave 54)

Layer-routed: engine BUGS are in `wave54/seats/seat-125-126.md`. Prompt text and render shape only.

## G5 - `[named: <card>]` IS THE RIGHT RENDER IN THE WRONG PLACE FOR THE DECISION IT GOVERNS.

The tag itself is a model dedicated-line: Silverquill Silencer's chosen name is printed on the
opponent battlefield line as `Silverquill Silencer {b}{w} (3/2) [named: Exquisite Blood]`, 54
records across my two seats, and it is exactly the #193 device - one string, one place, no
substring hunting inside a composite. **Nothing about it should change.**

What is missing is the price on the row that pays it. `126v146` seq 42 and seq 61: the option row
reads `Cast Exquisite Blood {4}{b} {leaves 3 of your 8 untapped mana sources untapped} {card text:
...}` with no mention of the 3 life, and the events that follow read
`- You cast Exquisite Blood | - You lost 3 life (now 21) | - Opponent drew a card`. Every other
cost this engine surfaces is priced ON the row: `[DRAW PRICE: ... - you would be at K]`,
`{leaves N ...}`, `{paying this taps: <defender> - it cannot block on their turn}`. This one is
not, and the second of the two casts took the seat from 8 to 5 with the opponent on 5.

Proposal, in the established shape: `[NAMED BY THEIR <source>: casting this costs you 3 life and
draws them a card - you would be at K]`, on the cast row, using the same `- you would be at K`
subtraction lane C already ships for DRAW PRICE. The trigger is
`@movedto(*[chosenname]|opponentstack):life:-3 opponent && draw:1 controller`
(`borderline.txt:102765`), so the magnitude and the draw are both readable from the ability, not
guessed. This is the cheapest of my proposals and it is the one my seat's loss margin is made of.

## G6 - THE SEAT-SIDE SURFACE FOR `chooseaname` IS FINE; THE ACTOR-SIDE MENU IS THE CARRIED ITEM.

The review-carry's wave-55 item ("the `Choose one mode for Silverquill Silencer:` surface is
`auto=chooseaname`, a card-NAME menu, not modes - unannotated, 9/9") is confirmed from the other
direction at my seats: the OBSERVING seat's surface, the `[named:]` battlefield tag, renders
correctly and legibly. So the defect is scoped to the ACTOR's menu only, and the two halves should
be fixed as one item so the same fact is not described two ways on the two seats. Method note
(#F2/method headline 2): I found this by reading the matched pair - the deck146 seat chose the
name, my deck126 seat paid for it - and neither log alone shows both ends.

## G7 - THE MULLIGAN COVERAGE LINE'S CAUSE CLAUSE IS DOING REAL WORK AND SHOULD BE KEPT VERBATIM.

Two `would not cover any spell` renders on my seats, two different cause clauses, both true:
`(you have 1 land; no spell in it is castable off {B}{G} alone)` at `126v162` seq 1 and
`(2 lands are not enough for your cheapest spell at mana value 3)` at `125v126` seq 1. The first
produced the corpus's cleanest mulligan - the deck126 reply names the cause the render printed,
the seat kept the replacement, bottomed the one redundant card, and won 38-0. Recorded because
amendment #171/#203's whole worry is guides treating this line as a one-land test: the cause clause
is what makes the line safe to read literally, and it is the reason a two-land seven and a one-land
seven can now be told apart from the render alone. No change requested; a request that it not be
shortened.

## G8 - `identical_option_asks_resolved` READS 0 ON ALL TWELVE OF MY GAMEENDS WHILE 25.6% OF THIS
## SEAT'S DECISIONS SIT INSIDE RUNS OF 3+ BYTE-IDENTICAL DECLINED MENUS.

Lane D's de-duplication counter and the identical-run problem are measuring different things at
this seat, and the gap is worth one sentence in the ledger: de-duplication is about ONE ask with
repeated rows, and the run problem is about MANY asks with identical row sets across phases of one
turn. 72 of deck126's 281 decisions are in the second class, 65 of them in a single won game's last
three turns. The hold row is the intended answer and cannot reach a `blockers` window, which is
where 27 of this seat's 51 opponent-turn windows are. Not a proposal - a request that the two
numbers not be read as one when the corpus note pairs them.
