# deck146 (Orzhov Dungeons) — wave-62 seat review

Corpus: `matchups-20260905-133058`, binary `12be0b79c`. Six games, **6-0**, all as the deck0 seat.
293 model decisions, 1 fallback (`unparsed_reply`, 0.3%), 0 crashes, 0 hangs, 0 wall misses,
0 timeouts. HOLD row offered in 119 windows, taken 25. 222 of 293 replies (76%) wrote past the
answer line despite "Write nothing else."

## 1. Game by game

| Opp | Result | Turns | Deciding decisions |
|---|---|---|---|
| 123 | W 21 / 0 | 11 | s13 Soul Shatter on Bloodline Keeper (MV 4, their highest) on curve; s14/s15 Silencer naming Intruder Alarm (not on their line, the engine they replay); s21 alpha strike into an empty board for lethal. Clean. |
| 130 | W 22 / -5 | 15 | s5 Nadaar t5, s21 Lolth t13 → s22 -3 for Spiders; s25 the Spider blocks Rorix Bladewing (6/5 flier) — the Spider was made for exactly that. Removal never needed. |
| 152 | W 22 / -4 | 17 | s11 Soul Shatter on Augur of Autumn; s20/s21 Command taking **return + sacrifice** (a two-for-one, guide entry 5); s41 Kaya -3 on Tovolar's Huntmaster the turn she landed. |
| 125 | W 20 / -7 | 17 | s34 Emeria's Call for two 4/4 Angels; **s45 the Hive CHECK 0 fired verbatim** — the reply wrote "Angel (4) + Angel (4) + Goblin (1) + Hive (3) = 12. Opponent life is 5", animated in UPKEEP and s49 swung for the win. This is the guide's most expensive rule paying out exactly as written. |
| 162 | W 1 / -5 | 17 | s20/s21 Kaya -3 on Underworld Dreams; **s41 the Verse target was decided by the parser, not the model** (item HIGH-1); s29 gang-block on Fate Unraveler read off the printed `GANG BLOCK:` clause; won at 1 life on a three-body swing at s55. |
| 126 | W 11 / -1 | 29 | s14 Kaya -3 exiled **Sanguine Bond** and s24 exiled **Exquisite Blood** — the pair the guide calls unbeatable, answered on sight, both before they combined. Then the game stalled 12 turns on the lifegain walls (item MED-1 / guide edit). s53 model decode spiral → recovery. |

Hands were real: only vs162 mulliganed (s1 ship on the untouched seven, s2 keep, s3 bottomed Lolth
from a two-land six — inside the guide's bottom rule).

## 2. Engine / interface / card items

**HIGH-1 — a coded CHOICE line buried in trailing CoT overrides the model's committed answer, and
the overriding line is trimmed out of the record.**
Repro: `1788633094-ai_baka_deck146-0x55cd37159450-vs-ai_baka_deck162.jsonl` seq 41, turn 15,
`kind: ask`, a two-row Vanishing Verse TARGET CHOICE. Rendered rows:
```
1. Liliana's Caress [enchantment] [opponent's battlefield] - "Whenever an opponent discards a card, that player loses 2 life."
2. Ob Nixilis, the Hate-Twisted [planeswalker] [opponent's battlefield] - "Whenever an opponent draws a card, ..."
```
The reply's first line is `CHOICE: 1 (Liliana's Caress)` and its PLAN restates that target. The
record carries `choice: 2`, `chosen_text: Ob Nixilis`, `answer_replaced: true`,
`coded_answers: 2`, `latched_coded_line: 2`, `post_plan_overrun: 2803`, `reply_trimmed_bytes: 2337`.
The model then rambled ~2.8 KB past PLAN (visible tail: "If I target Liliana's Caress ... I am
dead ... Wait, let me re-read the stack") and somewhere in that ramble emitted a second coded line
that the last-wins resolver (`salvageLoopedChoice`, `AIPlayerGPT.cpp`) latched. Two problems:
- The latched line is *hypothetical reasoning*, not a self-correction. The seat's stated answer did
  not execute.
- **The line that executed is not in the record.** `reply_trimmed_bytes: 2337` cut it. The record
  says *which ordinal* was latched but not *what it said*, so the override cannot be audited from
  the corpus at all. Same shape at `...deck146...vs-ai_baka_deck126` seq 80 (`coded_answers: 2`,
  `latched_coded_line: 2`, `post_plan_overrun: 3017`, `reply_trimmed_bytes: 2551`,
  `parse_note: hold_row_named;echo_index_conflict`) — 2 of 2 answer_replaced records in this seat
  trim away the deciding line.
Weight: this was the seat's survival window. The prompt printed
`DRAW FORECAST: ... = 7 life to the punishers above - if it resolves as forecast you would be at 0;
that KILLS you`. Exiling Liliana's Caress (the model's answer) leaves Ob Nixilis up and the seat
dies to its own draw step; exiling Ob Nixilis (the parser's answer) is what took it to 1 instead of
dead. **The seat won this game because the parser overrode it.** Suggested fix direction: preserve
the latched line's text on the record whenever `answer_replaced` is true, and require a coded line
to be line-leading *and outside* the plan body before it can displace a valid first-line answer.

**HIGH-2 — the attacker A-line prices `(blocking trigger:` per blocker as a LONE-block price; a
gang block fires every one of them and nothing on the screen says the prices add.**
Repro: `1788633091-ai_baka_deck146-0x55591c582320-vs-ai_baka_deck126.jsonl` seq 32, turn 17.
Rendered:
```
A1. Silverquill Silencer (3/2) [their untapped blockers: Pride Guardian (0/3) (you kill it, your attacker lives (blocking trigger: they gain 3 and may gain 2 more)); Wall of Omens (0/4) (neither dies (blocking trigger: they may gain 2)); Perimeter Captain (0/4) (neither dies (blocking trigger: they may gain 2))]
```
The worst number any line offers is 5. The prompt's own boilerplate warns only that *power* adds in
a gang block — "two or more of theirs may block the same attacker together and add their power,
which no listed result includes" — and says nothing about the gain prices. What happened (seq 33
`events`): all three walls blocked the one Silencer and the bill was **nine**:
```
- Opponent used: Life with Perimeter Captain   - Opponent gained 2 life (now 27)
- Opponent used: Life with Perimeter Captain   - Opponent gained 2 life (now 29)
- Opponent used: Life with Perimeter Captain   - Opponent gained 2 life (now 31)
- Opponent gained 3 life (now 34)
```
Card facts verified against the primitives and Scryfall (they agree — this is a render item, not a
card bug): Perimeter Captain `auto=@combat(blocking) source(creature[defender]|mybattlefield):may
life:2 controller` / Oracle "Whenever a creature you control with defender blocks, you may gain 2
life" — it fires once **per blocking defender**, so three blockers is three payments plus Pride
Guardian's own 3. Note also that the `(blocking trigger: they may gain 2)` printed on the *Wall of
Omens* row is Perimeter Captain's trigger, not Wall of Omens' (Wall of Omens has no blocking
trigger: `auto=draw:1 controller`). The price is truthful for that one block but the label reads as
the row's own card.
Cost across this game: seqs 25, 32, 38, 41 sent priced attackers on four consecutive turns, dealt
**zero** damage, and took the opponent from 21 to 42 life. A `[GANG BLOCK PRICE: all N of their
untapped blockers on this attacker pay a combined K]` clause on the A-line would close it — the
defensive window already prints exactly this shape (see MED-2).

**MED-1 — `Silverquill Silencer` naming row shows presence but not remaining copies.**
`...vs-ai_baka_deck162` seqs 24, 28, 32: three Silencers each named `Teferi's Puzzle Box
{visible now: 1 on their battlefield}`. deck162 runs 2 copies, so the bet is not dead — but the
annotation gives the reader only "1 is already out", which reads as the guide's forbidden
"never a card already sitting on their battlefield line" while being, in fact, a live bet. The
annotation is ambiguous in the one direction that matters.

**LOW-1 — one model decode spiral, handled correctly.**
`...vs-ai_baka_deck126` seq 53, turn 25, 527 s (58.5% of deadline): the reply degenerated into
repeated tokens ("Aldo … Kra … Cra"), classed `unparsed_reply`, and the seq 54 `recovery` record
shows the heuristic casting Nadaar. No engine defect — recorded as the corpus's only fallback and
as evidence the fallback+recovery pair works.

**MED-2 — a success worth keeping: the defensive `GANG BLOCK:` clause fired and was used.**
`...vs-ai_baka_deck162` seq 29 rendered
`A1. Fate Unraveler (3/4) deals 3 [GANG BLOCK: any 2 of yours together deal 4, enough to kill this
attacker; each B-line result below is a LONE blocker only]`, whose three B-lines all read
"(your blocker dies, attacker lives)". The seat answered `BLOCKS: B1:A1, B2:A1` and killed it — a
line the 1-on-1 ladder alone would never have found. 4 of 21 blocker windows in the corpus carry
the clause. **This is the exact clause HIGH-2 is missing on the attack side.**

## 3. Guide verdict: **EDIT**

The guide is doing its job — CHECK 0 fired verbatim and won vs125; the Sanguine Bond / Exquisite
Blood answer-on-sight rule won vs126; the cast order, the dungeon choice, the pay-3-life floor
(0 payments in 6 games) and the bottom/discard rules were all followed. One rule has a loophole the
corpus paid for.

**Edit 1** — LIFEGAIN WALL section (was line 403-405).
Before:
```
  the swing PAYS them: send only the attackers that get through
  (flying, menace, or a body whose tag kills something), keep the rest home, and win with removal,
  Kaya and the dungeon drains instead.
```
After:
```
  the swing PAYS them: send only the attackers that get through (flying, menace, or a tag reading
  "[no creature they control can block this attacker]"), keep the rest home, and win with removal,
  Kaya and the dungeon drains instead. A TAG LINE THAT KILLS ONE OF THEM IS NOT AN EXCEPTION -
  THEY choose the blocker and they choose the one that pays them and lives - AND THE PRICES ADD:
  every blocker joining a gang block fires its own clause, so the tag's worst single number is a
  floor, not the bill. WHAT THIS COST: at `vs126` seq 32 one Silencer whose worst tag line read 5
  was blocked by all three walls and paid NINE (2+2+2+3); four such swings (seqs 25, 32, 38, 41)
  dealt ZERO damage and took them from 21 to 42 life.
```
Paid for by `...vs-ai_baka_deck126` seqs 25, 32, 38 and 41. The exception "a body whose tag kills
something" licensed three of those four swings (Silencer's tag reads "you kill it, your attacker
lives" against Pride Guardian at s25/s32 and against both walls at s41) — and it is the same false
sentence the guide already refuses two paragraphs above under "THE OTHER NAMES ON THE LIST DO NOT
CANCEL IT": they choose the blocker and they choose the one that pays them and survives. Seq 38 is
the naked case — **every** blocker on **both** A-lines read "(neither dies (blocking trigger: they
may gain 2))", no exception applied at all, and the seat attacked anyway for +4 and no damage.

**Edit 2** — DECIDING SITUATIONS mirror line (was line 709-711), so the recognizer matches the body.
Before: `through; hold the rest. No such clause on the tag -> their walls are free to attack`
After: `through; hold the rest - a tag line that KILLS one of their blockers is not an exception, and` /
`the prices ADD across a gang block. No such clause on the tag -> their walls are free to attack`

Revised guide: `wave62/deck146/strategy.txt`, **70,986 bytes** (was 70,343; pool band 41-71 KB, pool
max 70,999). No other line changed.

## 4. Proposals

None. The gang-block price gap is an engine/render item (HIGH-2), not a general-guide or
skill-writing item, and one deck against one opponent is too thin to move the pool-wide surfaces.

## What I did NOT check

- The 15 games deck146 is not in, and the 152v125 rerun corpus (`matchups-20260905-161801`) — read
  only for the whole-corpus `answer_replaced` scan.
- Opponent seats' translogs beyond the deck146 pairings; opponent decision quality is uncharacterised.
- The lane-{W,X,Y,Z,AA} predictions — adjudication is the engine seat's deliverable, not this one.
- The trimmed ~2.3 KB of seq 41's reply: the line that actually executed is unrecoverable from the
  corpus, so HIGH-1's mechanism is inferred from `latched_coded_line` + the last-wins resolver in
  `AIPlayerGPT.cpp`, not read directly.
- No build, no game run, no git — the revised guide is untested in play.
- Whether the seq 32 gang block was the opponent AI's deliberate choice or incidental.
