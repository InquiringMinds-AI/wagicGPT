# deck126 (Sanguine Blood, WGB wall-prison combo) — wave-61 review

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-092408/17886182NN-ai_baka_deck126-0x...-vs-ai_baka_deckXXX.jsonl`.
Record 3-3. 217 answered windows across the six seats (184 `ask`, 16 `priority`, 9 `blockers`,
6 `attackers`, 2 `discard`), 3 `recovery` records from 3 fallbacks (all `named_row_reask`, all in the
deck125 game), 0 stale drops, 0 hangs. Note (method): the record `phase` field is not the rendered
phase — the block declarations are `kind: blockers` records, not `phase: Blockers` ones.

## 1. Game by game

| opp | result | turn | final | deciding decision |
|---|---|---|---|---|
| 152 | WIN 51-0 | 18 | 51/0 | seq 14 t12 Tribute names Wolf (6/6) → 32 life; pair completed seq 19 t18; seq 21 Vampire attack ends it |
| 123 | WIN 39-0 | 13 | 39/0 | seq 15 t13 Tribute at "they control 205 creatures" with the pair out — Rule #2 pressed in its first window |
| 130 | WIN 28-0 | 17 | 28/0 | seq 20 t17, at 8 life vs 20, "Rorix Bladewing (6/5) [flying, haste] is sacrificed, you gain 5" with the pair out — textbook Rule #2/7b |
| 146 | LOSS -5/24 | 15 | -5/24 | seq 18 t14; see B below. Game was already lost by t13 (board wiped twice by Soul Shatter + Silverquill Command, Exquisite Blood exiled by Vanishing Verse t11) |
| 162 | LOSS 0/4 | 21 | 0/4 | seq 37-56: **twenty consecutive windows offering the winning Tribute, all declined**; see A |
| 125 | LOSS 0/40 | 43 | 0/40 | pair complete from seq 41 (t28); opponent had 0 creatures + Lightmine Field + 2 Staff of Nin. No life-gain starter existed — see D |

The three wins are the same decision three times: both halves on the battlefield line, take the first
life-gain row offered. The guide's central instruction is being executed correctly and fast.

## A. HIGH — the win row was declined for a price the guide told it to refuse (STRATEGY, guide item)

File `1788618257-ai_baka_deck126-0x563183d27f70-vs-ai_baka_deck162.jsonl`, seq 37-56, turn 21.
Battlefield line carries BOTH `Sanguine Bond` and `Exquisite Blood`; the prompt prints the C5 banner
("Both halves of a life LOOP are on YOUR battlefield"). Opponent life 4. Row 1 of every one of those
twenty windows:

```
1. Cast Tribute to Hunger {2}{b} {right now: they control 2 creatures - they choose which one} ...
   [DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition), and their Underworld Dreams,
   Ob Nixilis, the Hate-Twisted deal you 14 - you would be at 6]
```

Either sacrifice (Shield Sphere 0/6, Master of the Feast 5/5) gains 5-6, which the pair converts into
5-6 off an opponent on 4: the game ends. The price is survivable for the first SIX windows (seq 37-42,
life 20 down to 15); from seq 43 the row correctly reads "this KILLS you". The seat answered
"Cast nothing right now" to all twenty and died at 0 in its own draw step.

Why: the guide's DRAW-PRICE bullet (b) reads "Rule #7 entry 1 is the ONLY row worth that price".
Tribute to Hunger is entry 7b/8, never entry 1 — so the guide, read literally, forbade the winning
row. Rule #2 ("take the first of these the list offers, immediately") and the DRAW-PRICE bullet
contradicted each other and the model obeyed the bullet. Fixed in the revised guide (edits 1 and 2).

The seat's own replies show the second half of the trap: every plan from seq 37 on was "pass through
the draw step, then cast Staff of Nin in my main phase and ping to win". Its draw step was 26 life at
20 life. There was no main phase.

## B. MED — the plan asserted a pair that had been exiled three turns earlier (PERCEPTION)

`...vs-ai_baka_deck146.jsonl` seq 18, t14, 8 life. Exquisite Blood was exiled by Vanishing Verse on
turn 11 (in the log) and the battlefield line correctly lists only Chromatic Lantern and five lands.
The reply still opens `PLAN: Both Exquisite Blood and Sanguine Bond are now on the battlefield. The
loop is active.` The surface was true and complete; the model did not read it. Cost here was near
zero (the game was gone), but it is the same false-pair belief that shapes the deck125 endgame.
Guide-compliant otherwise: Rule #7 entry 1 does command the Bond there.

## 2. Engine / interface / card items

**HIGH-1 — the DRAW FORECAST is written in the future tense while it is resolving now.**
`...vs-deck162.jsonl` seq 37, phase `Draw`, ON THE STACK already lists Teferi's Puzzle Box, Howling
Mine and both Dictate of Kruphix draw triggers. The rendered line:
`DRAW FORECAST: your next draw step draws 13 cards (...) = 13 x 2 = 26 life to the punishers above.`
"your NEXT draw step" is the draw step the seat is standing in, and its 26 is more than the 20 life
printed two lines above — yet the line carries no verdict. Every other lethal figure in this prompt
family is verdicted ("you would be at -4; that would KILL you", "this KILLS you"). Repro: same seq.
Fix shape: when the forecast's cost is at or above current life, print the resulting life and the
KILLS-you verdict, and drop "next" once the phase is already Draw. The seat's entire plan was built
on reaching a main phase this line implied was still ahead of it.

**HIGH-2 — no row is ever tagged as the win while the loop banner is printed.**
Same file/seq. The prompt states the loop exists in prose, and prices row 1's cost in life, but no
row says it ENDS THE GAME. Rows already carry computed verdicts for lethality against the pilot
("this KILLS you"); the symmetric verdict for the opponent is missing. Concretely, row 1 at seq 37
could read `- they would be at 0; this WINS the game` (gain >= 5 from a 0/6 or 5/5 sacrifice, opponent
on 4). Twenty windows and three of the seat's six games turn on this one row type.

**MED-3 — the guide-visible CHOICE-latching mechanic is undocumented and reverses the old one.**
`...vs-deck125.jsonl` seq 64, 79, 87 carry `answer_replaced: true, coded_answers: 2,
latched_coded_line: 2`; the engine executed the SECOND CHOICE line every time (seq 79: the reply's
first line was `CHOICE: 1 (Cast Tribute to Hunger)` on a "they control 0 creatures - at 0 this does
nothing" row, and the recorded choice was 3, `Cast nothing right now` — the latch saved the window).
Nothing in the prompt says a later coded line supersedes an earlier one. Worth one clause in the
reply-protocol block, because the deck guides currently teach the opposite (that such a reply is
refused and the window is spent).

**MED-4 — three windows lost to out-of-range CHOICE numbers naming actions from another decision.**
`...vs-deck125.jsonl` seq 44 (`CHOICE: 5 (Attack with Vampire)`, 4-row cast menu), seq 65
(`CHOICE: 6 (Cast Sorin, Lord of Innistrad)`, 3-row menu), seq 82 (`CHOICE: 5 (Cast Staff of Nin)` —
and the only Staffs in play were the OPPONENT's two). All three refused (`fallback:
named_row_reask`), each followed by an empty options record. The existing `[RE-ASK]` text handles a
named HAND card; it does not handle "the number is not on this menu" or "the action belongs to the
attack step". A re-ask line naming the legal range and, when the named string is an attack, pointing
at the "Cast nothing right now (combat comes next this turn)" row, would recover these.

**MED-5 — the ATTACK TOTAL Lightmine Field clause names the trap but leaves the arithmetic.**
`...vs-deck125.jsonl` seq 77 (also 29, 35, 50): "Lightmine Field fires on the declaration, before any
combat damage, and can kill your attackers first - how much it deals depends on how many you declare,
so it is not folded into either number." The engine knows the count it would deal for the declaration
being offered (primitive: `damage:1 all(creature[attacking])` scaled by `foreach(creature[attacking])`);
printing "at 1 attacker it deals 1 to each - that kills Vampire #1 (1/1) before combat damage" turns a
caveat into a decision. The seat attacked into it on four separate turns and every Vampire died
pre-damage.

**LOW-6 — the `[hold check]` line invited holds that could not hold.** `...vs-deck162.jsonl` seq 55:
the seat took the hold row while row 1's DRAW PRICE resulting-life changed on every window, so the
hold re-opened immediately (`hold_windows_skipped: 6` across a game with 20 such windows). Not a
defect — the row genuinely moved — but the hold-check note reads as an offer of relief it cannot give
when the only moving text is a recomputed life total on an unchanged row.

**Card facts verified against `projects/mtg/bin/Res/sets/primitives/*.txt`:** Tribute to Hunger
(`dynamicability<!mytgt toughnesslifegain targetopponent!>` — gain = sacrificed creature's toughness),
Exquisite Blood (`@lifelostfoeof(player):life:thatmuch controller`), Sanguine Bond
(`@lifeof(player)...:life:-thatmuch opponent`), Lightmine Field (`@each blockers:foreach(creature
[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)`), Forced Fruition
(`@movedTo(*[-land]|opponentstack):draw:7 opponent`), Perimeter Captain, Overgrown Battlement. All
match the rendered rows and the guide's claims. No Scryfall disagreement was needed.

## 3. Guide verdict: EDIT

Written: `wave61/deck126/strategy.txt` (70,903 bytes; live guide is 70,974 — inside the 41-71 KB band).
Five edits, each paid for by a decision above; the bytes were paid for by deleting per-corpus
tallies (rotation bookkeeping) and three restatement bullets whose behaviour was clean this corpus
(Chromatic Lantern, mulligan, bottom/cleanup).

1. **Rule #2, after "a wall cast with the win on the menu is a turn given away"** —
   before: nothing about paying for the win row.
   after: "A PRICE ON THE WIN ROW IS NOT A REASON TO DECLINE IT ... While that number is 1 or more you
   are ALIVE after paying and the loop ends the game in the same window - so PAY IT", plus "AND YOUR
   OWN DRAW STEP IS A CLOCK" (a DRAW FORECAST at or above your life means there is no later window
   this turn). Paid for by deck162 seq 37-42.
2. **DRAW PRICE bullet (b)** — before: "Rule #7 entry 1 is the ONLY row worth that price".
   after: "ONCE BOTH HALVES ARE ON YOUR BATTLEFIELD LINE, RULE #2'S WIN ROW OUTRANKS THIS PRICE AND
   EVERY OTHER READ - including a Tribute row, which is not entry 1 ... With the pair incomplete,
   Rule #7 entry 1 is the only row worth that price". This is the sentence that lost the game.
   Paid for by deck162 seq 37-56.
3. **Rule #7, "ONE CHOICE LINE PER REPLY"** — before: "THE FIRST WORD YOU WRITE IS THE LAST WORD YOU
   GET ... is REFUSED by the engine ... a refused reply spends the whole window" (false against this
   binary). after: "IT IS YOUR LAST ONE THE ENGINE RUNS ... the engine LATCHES THE LATER ONE - three
   replies this corpus did it and the later answer executed every time". Paid for by deck125 seq
   64/79/87.
4. **Re-ask block** — added "A NUMBER OUTSIDE THE PRINTED ROWS IS THE SAME REFUSAL", with the range
   rule and "ATTACKING IS A LATER DECISION IN THIS SAME TURN". Paid for by deck125 seq 44/65/82.
5. **Rule #6, the attack stop** — before: read it only off your own narration after a Vampire has
   already died. after: the ATTACK TOTAL header names Lightmine Field before the first send, plus the
   arithmetic ("the NUMBER OF ATTACKERS to EACH attacker, so one Vampire dies to one and two die to
   two - no count of 1/1s ever connects"). Paid for by deck125 seq 29/35/50/77.

No general-guide or skill proposals: every finding here is either deck-specific or an engine-render
item already reported in section 2.

## 4. Not checked
- The opponents' seat translogs were read only for context around the deciding turns (deck162 turn
  19-21 and deck125 turn 26-40); I did not trace opponent decision quality anywhere.
- Non-deciding windows in the 71-ask deck125 game (the ~30 hold/Sorin-+1 repeats between turns 30
  and 42) were classified by their `chosen_text` only, not by reading each prompt.
- Latency, `deadline_pct`, transport fields and the wave-61 lane predictions — the engine seat's job.
- Blockers: 9 block declarations arose (deck152 x4, deck146 x2, deck162 x3) and all looked
  guide-compliant on their chosen text; I did not audit the per-attacker assignment arithmetic, the
  LIVES/dies parentheses or the "deals N" tiebreak in any of them against the rendered B-lines.
- I did not verify that Sorin's -6 or any line other than a life-gain trigger could have broken the
  deck125 Lightmine Field lock; I concluded no life-gain starter existed and stopped there.
