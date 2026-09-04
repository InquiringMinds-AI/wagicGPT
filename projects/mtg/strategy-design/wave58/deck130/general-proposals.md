# deck130 → general-guide (RENDER / CORE-PROMPT) proposals, wave 58

Three render proposals. Each is cross-deck by construction (they key on a row SHAPE, not on a
card), none restates an existing R-item in `wave57/general-strategy.md` (checked by string).

## P1 — annotate the "move a card to hand" activation with what the card then does
**Shape:** an activated ability whose effect is `moveto(hand)` (graveyard recursion, flashback-
style returns, tutor-to-hand activations). Rendered today with cost and card text only.
**Evidence:** `matchups-20260904-015059/1788504683-ai_baka_deck130-0x560b0f3dc510-vs-ai_baka_deck125.jsonl`,
seq 95, 99, 103, 105, 107, 109, 112, 116, 118, 121, 125, 128, 130, 132 — fourteen consecutive
upkeeps, turns 30-56, pilot hand `(0 cards): (none)`, 11-14 untapped sources, opponent at 4 life:
```
1. Put a card into hand with Hammer of Bogardan [cost: {2}{r}{r}{r}] {card text: "Hammer of
Bogardan deals 3 damage to any target. -- {2}{R}{R}{R}: Return Hammer of Bogardan from your
graveyard to your hand. Activate this ability only during your upkeep."}
```
`CHOICE: 0 (pass)` all fourteen times; the seat lost 0-11 on turn 57. Every CAST row on the same
seat carries `{... and 3 to the opponent at life N leaves them at M}` and a mana-left clause; this
row, the only reach the deck had left, carries neither.
**Ask:** for a `moveto(hand)` activation, append the returned card's own damage/kill clause and
the current opponent life in the existing `{...}` shape, plus the sources left after paying —
e.g. `{returns a card that deals 3 to any target; opponent at 4 - two returns reach it}
{leaves 6 of your 11 untapped mana sources untapped}`. Facts only; no recommendation.

## P2 — price a REPEATABLE activation over the payments the pilot can actually afford
**Shape:** an activated ability with a repeatable cost (sacrifice-a-token, tap-an-untapped-X, a
mana cost payable N times from the current pool) whose row prints a single-activation verdict.
**Evidence:** `1788504674-ai_baka_deck130-0x5569cb670ad0-vs-ai_baka_deck162.jsonl` seq110, pilot
at 2 life with 8 untapped sources and three Goblin tokens:
```
1. Deal 2 damage with Siege-Gang Commander targeting Fate Unraveler [opponent's battlefield]
{right now: takes 2 damage - SURVIVES (toughness 4)} [cost: {1}{r}, Sacrifice]
```
Two payments (4 of 8 sources, 2 of 3 Goblins) kill a 3/4. `SURVIVES` is true of one payment and
false of the window, and it is the only verdict on the row. Same seat, seq113 identical.
**Ask:** where the cost is payable more than once from the CURRENT board and pool, print the
affordable repeat count and the cumulative verdict alongside the per-payment one:
`{right now: takes 2 damage - SURVIVES (toughness 4); you can pay this 3 times right now -
2 payments = 4 damage - DIES}`. The count is a fact the engine already has (it gates the row's
legality); today the pilot must derive it, and does not.

## P3 — sum the damage already ON THE STACK against the pilot's own life
**Shape:** the `ON THE STACK, waiting to resolve` block, when one or more pending items deal
damage to or drain the pilot.
**Evidence:** same file, seq114, `Your life: 1`:
```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): ability: Underworld Dreams's deal 1 damage [from their Underworld Dreams] ...
  2: ability: Howling Mine's Draw 1 [from their Howling Mine] ...
  3: ability: Fate Unraveler's deal 1 damage [from their Fate Unraveler] ...
  4: ability: Underworld Dreams's deal 1 damage [from their Underworld Dreams] ...
```
Three of the four take a point off a pilot at 1. The frame forecasts a FUTURE draw step on the
next line down (`DRAW FORECAST: your next draw step draws 3 cards (1 + Howling Mine 1 + Dictate
of Kruphix 1) = 3 x 2 = 6 life to the punishers above`) but prints nothing about the damage
already committed in front of it. The window is the pilot's last chance to respond and the frame
does not say it is the last chance.
**Ask:** one line under the stack block in the DRAW FORECAST's own style —
`ON THE STACK AGAINST YOU: 3 damage pending (Underworld Dreams 1, Fate Unraveler 1, Underworld
Dreams 1). Your life: 1 - THIS KILLS YOU IF IT ALL RESOLVES.` Keep it pessimistic in the lethal
direction (count what is certain, under-claim what is conditional), per D6/D7's rule.
