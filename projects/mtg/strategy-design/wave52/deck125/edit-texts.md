# wave-52 deck125 guide edits (before -> after, each with its finding)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` at corpus time (wave-51 revisions 5d0c24d28 + boundary pass 6686392d3).
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260901-223430/`. 7 edits, every one cited to a seq.

## W64 Fruition lethal-price stop

**Finding.** `deck125 vs162` seq 97 (t20, 3 life): the row printed `[DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition), and their Underworld Dreams deals you 7]`; the reply cast Essence Scatter, the seq-98 events show seven draws and `Underworld Dreams dealt 1 damage to you (now 2)`, and the seat was dead in that window (3 -> 2 -> 1 -> 0, gameend t20 at 0/18). The Forced Fruition section forbade the cast ('cast a counter ONLY when that price is below your life minus 1') but had no sentence naming the ROW's own printed number, and the price arrives as N separate 1-damage pings, not one hit. This is the loss.

**BEFORE**
```
   14. At 15 life you cast an Elixir under one of each, drew seven and finished at 13 for the five
   life it gained; at 11 life you cast Cancel under two Fruitions and a Dreams, drew fourteen, and
   died on the spot.
```

**AFTER**
```
   14. At 15 life you cast an Elixir under one of each, drew seven and finished at 13 for the five
   life it gained; at 11 life you cast Cancel under two Fruitions and a Dreams, drew fourteen, and
   died on the spot.
   EVERY ROW ON THAT MENU PRINTS THE PRICE, THE COUNTER ROW INCLUDED: "[DRAW PRICE: casting this
   draws YOU 7 cards (their Forced Fruition), and their Underworld Dreams deals you 7]". Read that
   last number against YOUR LIFE before anything else on the row. When it is at or above your life,
   THAT ROW KILLS YOU - answer "Cast nothing right now" even though the thing on the stack is on
   your counter-on-sight list, because a threat that resolves can still be answered next turn and a
   row that kills you cannot. The damage does not arrive as one hit you might survive: it arrives as
   seven separate 1-damage pings, one per card drawn. At 3 life, with "deals you 7" printed on the
   row, you cast Essence Scatter at a Fate Unraveler; the Unraveler was countered, the seven draws
   took you 3 -> 2 -> 1 -> 0, and you lost the game in that window.
```

## W65 sweeper-at-0 220/220

**Finding.** 220 sweeper rows at a header second number of 0 across the 6 seat logs, 220 declined (5th corpus at the ceiling). The '41 windows / 40 declined' number was two corpora stale.

**BEFORE**
```
castable card on the menu is not a reason to cast it. Forty-one windows at 0 this corpus, forty
declined; the one you cast was the Verdict that killed your own Emrakul (#1).
```

**AFTER**
```
castable card on the menu is not a reason to cast it. Two hundred and twenty rows at 0 this
corpus, two hundred and twenty declined - this is the most reliable line in the guide; the one you
ever did cast was the Verdict that killed your own Emrakul (#1).
```

## W66 low-X 0/18

**Finding.** 18 Revelation rows at `max affordable X=1`/`X=2`, 0 taken; all 10 Revelation casts came off rows reading X=3 or more (`vs146` seq 19/31/37/60/104, `vs152` seq 73/81/111, `vs130` seq 150, `vs123` seq 29). W58 PASS.

**BEFORE**
```
Sixteen windows at "max affordable X=1" or "X=2" one corpus, sixteen holds; the next corpus two
casts at 20 life, both from menus whose only other rows were "Cast nothing" and a sweeper at 0.
```

**AFTER**
```
Sixteen windows at "max affordable X=1" or "X=2" one corpus, sixteen holds; the corpus after that,
two casts at 20 life from menus whose only other rows were "Cast nothing" and a sweeper at 0; this
corpus eighteen such rows and eighteen holds, and all ten Revelations you did cast came off rows
reading X=3 or more. The rule works - keep reading the row.
```

## W67 X=3 11/11

**Finding.** 11 X menus, 11 answers of `X = 3`, off menus topping out at 5, 6, 7, 11, 13 and 16 (`vs152` seq 82 max 11, seq 112 max 13, seq 137 max 16). Old count was stale.

**BEFORE**
```
Six casts this corpus, six at 3, off menus reaching 4, 5 and 7: this rule works and there is
nothing to work out.
```

**AFTER**
```
Eleven X menus this corpus, eleven answers of 3, off menus that topped out at 5, 6, 7, 11, 13 and
16: this rule works and there is nothing to work out.
```

## W68 Elixir gate x2 + {spends K} clause

**Finding.** 2 of 11 Elixir activations broke the gate: `vs162` seq 7 (t5, 20 life, library 51, `{spends 2 of your 2 ... Essence Scatter {1}{u} in your hand needs 2}`, leaves 0) and `vs130` seq 36 (t13, 18 life, leaves 2 with a Fall of the Gavel as the only counter, price 5). Third corpus of the turn-2/turn-5 shape. The `{spends K of your M ...; <card> in your hand needs K}` clause (the wave-51 G1 ask) now prints on these rows - 434 occurrences across my 12 logs - so the guide can point at it instead of asking for arithmetic.

**BEFORE**
```
On turn 2 of a game you activated it from "Mana available: 2" to 0 at 20 life
with a library of 53 and a Scatter and a Gavel in hand: nothing bought, a counter's mana gone.
```

**AFTER**
```
On turn 2 of a game you activated it from "Mana available: 2" to 0 at 20 life
with a library of 53 and a Scatter and a Gavel in hand: nothing bought, a counter's mana gone.
You did it twice more this corpus - turn 5 from "Mana available: 2" to 0 at 20 life on a library of
51 with an Essence Scatter in hand, and turn 13 at 18 life on four sources with a Fall of the Gavel
as your only counter, leaving 2 against its price of 5. THE ROW DOES THAT SUBTRACTION FOR YOU: the
activation line prints "{spends 2 of your 4 untapped mana sources this turn; Fall of the Gavel
{3}{u}{w} in your hand needs 5}". When the number after "needs" is larger than what the row leaves,
the answer is pass.
```

## W69 [second copy: literal

**Finding.** Lane K retired `[already owned:`. 5 second-Staff casts this corpus, all off gate-legal rows (`vs146` seq 103, `vs126` seq 28, `vs152` seq 98, `vs130` seq 149, `vs123` seq 47) plus 1 declined window (`vs162` seq 91). deck125's guide had no sentence for the new tag; deck126's blind spot under the same tag (5/5 dead Lantern casts) is what makes the recognition line worth one paragraph here.

**BEFORE**
```
- Keep taking the "Deal 1 Damage with Staff of Nin ... targeting a player" option in every window it
```

**AFTER**
```
- A SECOND STAFF IS A SECOND CLOCK. When a row reads "Cast Staff of Nin {6} [second copy: you
  already control Staff of Nin; both stay on the battlefield - no legend rule]", that tag is not a
  warning: both Staffs stay, each draws you a card at your upkeep and each pings for 1, so the
  second one doubles your clock. Take it exactly when the gate above passes - five second-Staff
  casts this corpus, all five off a "leaves N" the gate allowed. The same tag on an Elixir of
  Immortality means the same thing: each copy activates on its own.
- Keep taking the "Deal 1 Damage with Staff of Nin ... targeting a player" option in every window it
```

## W70 Staff gate 4 unpunished

**Finding.** 4 Staff casts below the gate: `vs162` seq 32 (leaves 1, Essence Scatter in hand), `vs126` seq 24 (leaves 0) and seq 28 (leaves 0), `vs123` seq 32 (leaves 1). None was punished - the seat won 5 of 6. Rule kept (the corpus where it was punished cost two games); the guide's count corrected so it is not read as 'this always loses'.

**BEFORE**
```
"Leaves 1" with a Dream Fracture in hand is below the price - you took that row twice
  this corpus, and Ob Nixilis and three creatures resolved on the next opponent turns with no
  window offered.
```

**AFTER**
```
"Leaves 1" with a Dream Fracture in hand is below the price - you took that row twice
  in the corpus where it cost two games, and Ob Nixilis and three creatures resolved on the next
  opponent turns with no window offered. You took it four more times this corpus ("leaves 1" and
  "leaves 0", an Essence Scatter in hand) and none of the four was punished; the rule stands on the
  corpus where it was.
```
