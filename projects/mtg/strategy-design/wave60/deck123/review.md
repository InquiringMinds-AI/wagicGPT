# wave 60 - deck123 (Intruders Of Thraben, WBU token combo) - per-deck review

Corpus `matchups-20260904-171140`, six games, all natural. Seat files are
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-171140/<epoch>-ai_baka_deck123-0x...-vs-ai_baka_deck<N>.jsonl`;
below they are cited as `[vs N] seq K`. Record 3-3. 400 decisions, 6 fallbacks (all one shape,
below), 0 hangs, 0 crashes, 0 timeouts, 0 empty/transport failures on this seat.

## 1. Game by game

| vs | result | turns | how it ended |
|---|---|---|---|
| 162 | **WIN** -118 | 14 | Alarm T8, Doomsayer T12, chain fired T12/T13, 47 attackers for 140 at 3 life |
| 130 | **WIN** -10 | 11 | Doomsayer T5, Alarm T7, Keeper T9, chain T10, 21 bodies swung T11 |
| 126 | **WIN** -76 | 18 | Doomsayer T6/T8, Alarm T10, Keeper+Greaves T16, chain T16/T17, swung T18 |
| 152 | loss 24-(-2) | 13 | mull-free but land-light; two windows burned on turn 6 (below); never assembled |
| 146 | loss 22-0 | 15 | mulliganed to 6, three-land keep, first spell turn 6; Alarm resolved T12 at 3 life, too late |
| 125 | loss 83-0 | **66** | see below - the deciding fact is not a decision |

**Deciding decisions.**

- **[vs 125]** is the corpus's most important game and it contains no misplay worth the name.
  Seven threats were cast and **every one was countered** - narration in the final prompt reads
  `Your Intruder Alarm was COUNTERED by Cancel`, `Your Bloodline Keeper was COUNTERED by Essence
  Scatter` (x3), `Your Thraben Doomsayer was COUNTERED by Cancel` (x2) / `by Essence Scatter`.
  An Alarm resolved at `seq 43` (T22) and sat on an empty board for 44 turns: **zero token
  windows were ever offered in the whole game** (I grepped every `prompt` for `Create human` /
  `Create vampire`: 0 hits) because the deck never had a creature to tap. Not an engine defect -
  a hard control matchup. The one strategic residue is Vision Skeins: cast at `seq 18, 73, 126,
  146` into a deck holding 8 counterspells, drawing the opponent two cards each time; two of the
  four were themselves countered. See §3 for why this did not become a guide edit.
- **[vs 146] seq 22** (T10, 3 life): `Cast Damnation {2}{b}{b} {right now: destroys 3 of their
  creatures (3 without a restriction against attacking), 1 of yours}`, answered
  `PLAN: Damnation: N 3, K 3, M 1; cast.` Correct, guide-shaped, and it bought the turns that got
  the Alarm down at `seq 26`. The game was lost to the mulligan (`seq 1-3`) and a turn-6 first
  spell, not to a decision. Variance.
- **[vs 152] seq 18-24** (T6): the only land in hand was Arcane Sanctum; playing it was forced and
  correct, but it enters tapped, so turn 6 opened on 2 sources with `Thraben Doomsayer {1}{w}{w}`
  in hand. The model had written `Cast Thraben Doomsayer turn 6` into its plan at `seq 18` and
  then spent **four windows** (19-24) trying to cast a card that had no row. HIGH item H1.
- **[vs 126] seq 46-57** and **[vs 162] seq 40, 52**: the chain fired and won, but nine
  activations were taken *after* the model's own stop. MED item H3.
- **[vs 162] seq 63**: `ATTACK TOTAL: 47 attackers listed, 140 total combat damage ... that KILLS
  them whatever they block`, answered `ATTACK: A1-A47`. The range-label render and the lethal
  arithmetic both did their job at the one window that ended the game.

## 2. Engine / interface / card items

### HIGH - H1. A hand card with no cast row states no reason, and the model names it anyway (5 of 6 fallbacks)
Repro: `[vs 152] seq 19`, T6, Main phase 1. `--- CURRENT SITUATION ---` renders
```
Mana available: 2 total (two untapped sources ...)
Your hand (7 cards): ... Thraben Doomsayer (copy 1 of 2 in your hand) {1}{w}{w} (2/2) [creature]; ...
```
and the menu lists Lightning Greaves / Tragic Slip / Devour Flesh / two declines. Reply:
`CHOICE: 2 (Cast Thraben Doomsayer)` -> `<refused: named_row_reask>`. Repeated verbatim at
`seq 22` after a clean re-ask. Same shape at `[vs 126] seq 5, 8` and `[vs 162] seq 17, 19`.
Five of this seat's six fallbacks are this one shape; each costs a refusal + a `recovery` record
+ a re-ask, so six windows across three games.

The hand line carries cost, type and P/T but **never why the card has no row**. I checked every
distinct `Your hand (...)` line in this seat's 400 prompts: not one carries a castability
annotation. The trust doctrine's own rule applies - a silent omission is where the model
confabulates, and it confabulated the same way five times. The engine already knows the answer
(the cast list was built from the oracle's legalCasts) and already prints far subtler facts on
the rows it does show (`{spends 2 of your 2 untapped mana sources this turn; Tragic Slip {b} in
your hand needs 1}`).

Concrete ask: annotate hand cards absent from this window's cast list with the blocking reason -
`Thraben Doomsayer {1}{w}{w} (2/2) [creature] {no cast row this window: needs 3 mana, you have 2}`
- and for a colour block, the pip: `{no cast row: needs {W}{W}, you have 1 source of {W}}`.
Note the guide has taught this in prose for two waves ("A CHOICE NAMES A NUMBERED ROW ON THIS
WINDOW", with the previous corpus's two incidents named in the text) and the count went 2 -> 5.
Prose has been tried; the surface has not.

**Working correctly, for the record:** the re-ask machinery itself is clean. Every refusal
produced a `recovery` record and then a re-ask carrying `[RE-ASK] "Cast Thraben Doomsayer" is not
on this list. Answer with a number from 1 to 4.`, and the model answered legally every time
(`[vs 126] seq 7, 10`; `[vs 152] seq 21, 24`; `[vs 162] seq 21`). Zero refusals survived a re-ask.

### HIGH - H2. The land-drop row does not say the land enters tapped
Repro: `[vs 152] seq 18`, T6 (a T5 decision by the seat's own clock).
```
Your hand (8 cards): Arcane Sanctum (land: taps for {W}{U}{B}); ...
Land drop: play Arcane Sanctum now? (playing a land costs no mana and uses up no cast: it does not reduce what you can cast this turn)
1. Play Arcane Sanctum
2. Hold Arcane Sanctum - do not play it now
```
Verified against the primitive: `mtg.txt:4934 name=Arcane Sanctum / auto=tap(noevent) /
text=Arcane Sanctum enters tapped.` Scryfall agrees. Neither the hand line nor the row says so,
and the row's parenthetical actively reads as a promise that this land is available now ("it does
not reduce what you can cast this turn" - true, and beside the point). The model wrote
`Cast Thraben Doomsayer turn 6` into the plan off that window and paid for it with H1's four
windows on the next turn.

This is not the choice-affecting kind of defect here (Sanctum was the only land), but the
land drop is where this deck's plan for the next turn is formed, and the plan is carried. Same
gap for `Isolated Chapel` (`mtg.txt:59410`, `auto=aslongas(plains,swamp|myBattlefield)
tap(noevent) <1 oneshot`) which is conditional and so genuinely needs the row to evaluate it:
`1. Play Isolated Chapel {enters untapped - you control a Swamp}` vs `{enters TAPPED - you
control no Plains or Swamp; it makes no mana until your next turn}`. Deck123 runs 2 Sanctum +
4 Chapel of 24 lands, and the pool's tapped-land count is far larger.

### MED - H3. The token repeat row prints M but not the two numbers the stop is made of
Repro: `[vs 126] seq 49`, T17.
```
1. Create vampire with Bloodline Keeper, repeated N times, then stop [you control 25 creatures right now; you name N on the CHOICE line ...]
```
Reply: `CHOICE: 2 (Create vampire with Bloodline Keeper x1)` /
`PLAN: L=21, C=1, stop=24; M=25 now; this window: pass`.
The model's own plan line has the arithmetic exactly right, says `pass`, and the CHOICE line above
it takes the row anyway. It did this **nine consecutive times** (`seq 49, 50, 51, 52, 53, 54, 56`
plus `[vs 162] seq 40, 52`), riding M from 25 to 48 against a stop of 24 - measured cost 9 windows
and 138 s of inference on this seat alone, in games already won.

The row itself carries `M` and nothing else. `L` (opponent life) and `C` (their creature count)
live 15 lines up the prompt in two different lines, and the model must re-derive the stop every
window from three separated places. The "deciding fact rides the option" principle has not been
applied to this row. Concrete ask: put the whole comparison on the row -
`[you control 25 creatures; they are at 21 life with 1 creature that can block - 25 bodies is
already more than they can survive]`. Do not remove the row (legality without constraint), just
state the verdict the engine can already compute.

### MED - H4. The loop caution's "in a row" clause resets on one decline, and the model reads that as permission
Repro: `[vs 126] seq 55` vs `seq 56`, both T17, Main phase 1, same row. At `seq 55`:
```
1. Create vampire with Bloodline Keeper [cost: Tap] [repeat: you control 46 creatures; activated this turn 26 times already. This turn will not advance while you keep taking this option; you have taken it 26 times in a row with no other action in between.]
```
Model passed. At `seq 56` the tail clause is gone -
`... activated this turn 26 times already. This turn will not advance while you keep taking this
option.]` - and the model immediately answered `CHOICE: 2 (... x2)`, taking M to 48. The 26-count
survived; the sentence that actually deterred it did not. Suggest the "no other action in
between" streak not be the load-bearing half of the caution, or that the softer form still name
the total windows the turn has consumed.

**Not a defect (checked and cleared):** the `seq 55 -> 56` re-offer is legitimate. Diffing the two
`--- CURRENT SITUATION ---` blocks, the opponent resolved a creature (their battlefield went
`of which 1 is a creature` -> `2 are creatures`) and `Intruder Alarm's Untap` went on the stack -
a real board change, so the dedupe key behaved correctly.

### LOW - H5. `Opponent life trend` is unhelpful in exactly the game that needed it
`[vs 125] seq 99` (T41): `Opponent life trend: unchanged at 20 since turn 4` is a line this deck
never acts on, while the fact that mattered - that seven consecutive spells had been countered
and the opponent held up 8+ untapped blue sources every turn - is present only as raw narration
the model must re-read and tally itself. `Their untapped sources: N (colours they could make: ...)`
is rendered, but nothing summarises the counterspell pattern. A cheap counterpart to the existing
punisher blocks would be a `SPELLS COUNTERED THIS GAME: 7 (Cancel x4, Essence Scatter x3)` line
when the count is non-zero. Filed LOW because the game was unwinnable regardless.

### LOW - H6. Guide-internal tension the render could settle
`[vs 126] seq 34`: `Cast Tribute to Hunger {2}{b} {right now: they control 1 creature - Pride
Guardian (0/3) [defender, doesn't un...] is sacrificed, they gain 3}`. Verified: `mtg.txt:88356
name=Pride Guardian / abilities=defender / auto=@combat(blocking) source(this):life:3 controller`.
The row's `[defender]` tag reads as "decline" (an edict on a body that cannot attack you), but a
lifegain wall is precisely the body this deck must remove to attack. The model chose correctly
here. If the row's verdict clause distinguished "cannot attack you" from "stops your attack",
this would stop being a judgment call.

## 3. Guide verdict: **KEEP as is**

`bin/Res/ai/baka/deck123_strategy.txt`, 70,982 bytes / 869 lines - in band (41-71 KB), at its top.
No edit. Reasons, each grounded in this corpus:

**What the guide taught and the model executed.** Rule 3's Damnation arithmetic was written and
obeyed at `[vs 146] seq 22` (`N 3, K 3, M 1; cast`) and correctly declined at `[vs 125] seq 127,
137` where the row read `destroys 0 of their creatures ... 0 of yours`. Rule 5's edict count was
obeyed at `[vs 125] seq 85` (Tribute at N=1 into a 15/15 Emrakul, +15 life, 13 -> 28) and
`[vs 126] seq 34`. Rule 2's Marsh Flats crack was taken at the first window it was listed in all
three games it appeared (`[vs 125] seq 21, 35, 112`; `[vs 126] seq 27`). Rule 0's land drop was
taken at every window a Play row existed - I found no `Play no land right now` answered off a
menu that had one. Rule 6's equip-once held (`[vs 162] seq 30`, `[vs 126] seq 39`, one each, no
re-equip). The stop's *arithmetic* was written correctly in every single token window
(`L=21, C=1, stop=24`), including the nine it then ignored. The mulligan rule held at all six
looks. This is a success verdict, and it is what the loop's cheap side quest is supposed to look
like when it is done.

**Why the two live failures are not guide edits.** Both are cases where the guide already says
the right thing, in strong terms, with the previous corpus's incidents quoted in the text, and
the model does the reasoning correctly and then acts against it:
- the named-row refusals are governed by a paragraph that names its own prior count ("twice last
  corpus") - and the count rose to five;
- the stop overshoot is governed by three separate passages including "Take the repeat row ONCE
  per turn and then pass" and "Do the subtraction BEFORE you write the CHOICE line" - and the
  model *did* the subtraction, wrote `stop=24; M=25 now; this window: pass`, and took the row.
Adding a fourth passage would be the busywork pattern. Both belong to the surface (H1, H3, H4),
which is where I have routed them.

**The one edit I considered and rejected.** A CL7 caveat on Vision Skeins against a
counterspell/control opponent, paid for by `[vs 125] seq 18, 73, 126, 146`. Rejected: at three of
those four windows Skeins was the only castable card and the mana was otherwise idle, and the
game was lost to eight counterspells that four fewer opponent cards would not have changed. The
guide is at the top of its band, so this teach would have to displace one that this corpus shows
holding. Not paid for.

No `general-proposals.md` and no `skill-proposals.md`. The routing this review used (prose has
failed twice -> move the fact onto the surface) is the existing perception/strategy doctrine
applied, not a new rule; restating it as an amendment would be noise.

## What I did NOT check

- The five opponent seats' own translogs beyond the deck123-vs-X files (I read the deck123 seat
  for all six games; I did not trace the opponents' decisions except through my seat's narration).
- Whether H1's absent-castability annotation is cheap at the render site - I did not read
  `AIPlayerGPT.cpp` / the serializer; I am reporting the gap, not costing the fix.
- Lane L/M/N/O/P/Q predictions - the engine seat adjudicates those. I incidentally observed
  ATTACK TOTAL (`[vs 162] seq 63`), the loop caution (`[vs 126] seq 55`), the `named_row_reask`
  class and its re-ask, and the `[you declined this exact list N times]` counter all rendering,
  but I did not count them against any lane's prediction.
- Any deck-construction question (mana base, 24-land count, the 3 Greaves) - out of scope here.
- I did not run the game, the suite, or PARSETEST; nothing outside `wave60/deck123/` was written.
