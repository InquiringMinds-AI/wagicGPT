# deck123 (Intruders of Thraben) - wave-48 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (wave-47 revision +
boundary pass, 40,627 chars). Six edits, one of them a deletion. 40,627 -> 43,086 chars.
Corpus: `matchups-20260827-072004`. Every number is read off a prompt, a reply or a primitive.

---

## EDIT A48-1 - RULE 1: the repeat row is the way to take the line; the 50 ceiling and the width paragraph go
**Finding.** Lane N's row `2. Create human with Thraben Doomsayer, repeated N times, then stop
[... e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x50)" ...]` was offered 14 times and
taken 10. Four takes ran ONCE because the N was written `N=17` or omitted
(`...deck123-0x55ca87a19a00-vs-...deck162.jsonl` seq 28, 29, 31; `...vs-...deck126.jsonl` seq 22);
the pilot had computed `N = 17` correctly each time. Five takes wrote `x50` regardless of the
arithmetic (`...vs-...deck126.jsonl` seq 14 at M=5 with L+C+3 = 26; seq 21-24 at M = 106..207
with L+C+3 = 30), past both the L+C+3 stop and the boundary pass's `M >= 50` ceiling. The
ceiling's reason - a 233,662-char attackers prompt - no longer exists: the 257-creature
attackers screen (seq 31) is 11,274 chars and one row (`A3-A257`). The live text also says
"no 'pass' row", which lane N made false (`0. Pass priority` on 63/63 single-option windows).
**Before** (operative parts)
```
  - MONOTONE CEILING, above every other line in this rule ... at 50 OR MORE, answer
    "CHOICE: 0 (pass)" ...
PASSING IS ALWAYS AVAILABLE, EVEN WHEN THE LOOP IS THE ONLY ROW ON THE SCREEN. These windows
routinely list exactly one action - the token line - and no "pass" row. ...
WHY STOPPING MATTERS EVEN WHEN YOU ARE WINNING: ... 233,662 character prompt ...
THERE IS NO CREATURE-COUNT CAP AT 12 OR AT 20 ...
"I have tapped it 0 times this turn" is not a reason ...
[DECIDING] - You just took that line and it appears again: take it again on the same test.
```
**After**
```
  - THE REPEAT ROW IS HOW YOU TAKE THE LINE. From the second activation of a turn on, the
    screen adds a second row: "... repeated N times, then stop [you name N on the CHOICE line,
    e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x50)" ...]". Take THAT row, ONCE, with
        N = (L + C + 3) - M
    and write N exactly the way the row's own example writes it ...
        CHOICE: 2 (Create human with Thraben Doomsayer x17)
    No other spelling is read. "N=17" and a bare "repeated N times, then stop" ... were each
    carried out as ONE token, three times in one turn ...
    Do NOT write x50 out of habit. Four x50 answers on one turn put 257 creatures on a board
    whose L + C + 3 was 30 ...
PASSING IS ALWAYS AVAILABLE. The last row on these windows reads "0. Pass priority (take no
action this window)"; when M has reached L + C + 3 that row is the answer ...
[DECIDING] - The "repeated N times, then stop" row is listed: take it ONCE, written "CHOICE: 2
  (Create human with Thraben Doomsayer x<N>)" with N = (L + C + 3) - M, then pass at the next
  window.
```
The `[repeat:]` description at the top of the rule now says the tag's N is per-copy, per-turn
(true), without calling it a tripwire. Net: -1,050 / +1,230 chars.

## EDIT A48-2 - INTRUDER ALARM AND IDYLLIC TUTOR: the Tutor finds only an Alarm; the auto-tap can strand the Alarm
**Finding.** Two Tutor casts with an Alarm already owned, both in lost games: `...vs-...deck146
.jsonl` seq 20-22 (turn 17, 3 life, Alarm on the battlefield AND in hand, reply *"to find a
creature"*, reveal returns a third Alarm); `...vs-...deck125.jsonl` seq 14 (turn 11, Alarm #1
cast that window, Tutor for Alarm #2, 22 maker-less turns follow). Cast-list entry 4 already
says "when no Intruder Alarm is on your battlefield or in your hand"; the seq 20 reply quoted
the fact and cast anyway - the belief "finds a creature" was never refuted by name (#77).
Second half: `...vs-...deck162.jsonl` seq 17 -> 19: the engine paid the Tutor's `{2}{w}` with
both Underground Seas (`{U} 2` -> `{U} 0`), the tutored Alarm waited a turn cycle, and `Cast
Thraben Doomsayer {1}{w}{w}` was declined twice (seq 19, 21) with three W/B sources open.
Primitive: `Idyllic Tutor ... target(<1>enchantment|reveal)`; the sixty's enchantments = Intruder
Alarm x4 (deck list).
**Before**
```
"Cast Idyllic Tutor {2}{w}" searches for an enchantment. The answer on the reveal list is
Intruder Alarm, every time. Cast the Alarm the next window it appears in your list.
```
**After**
```
"Cast Idyllic Tutor {2}{w}" searches for an ENCHANTMENT, and the only enchantment in this sixty
is Intruder Alarm. ... IT CANNOT FIND A CREATURE. "Cast Idyllic Tutor to find a creature" is a
sentence this deck wrote at 3 life ... With an Intruder Alarm on your battlefield OR in your
hand, the Tutor is a dead card: never cast it - Vision Skeins is the card that digs for a maker.
THE ALARM YOU TUTOR MAY NOT BE CASTABLE THIS TURN. The engine taps your mana for you, and it
paid a Tutor's {2}{w} with BOTH Underground Seas ... Read "sources that can make each: ... {U}
N" before you cast the Tutor: at {U} 2 or less, plan the Alarm for NEXT turn, and spend what the
Tutor leaves on a maker ...
```
Plus a DECIDING SITUATIONS line (`"Cast Idyllic Tutor" is listed and "Intruder Alarm" is on your
battlefield line or in "Your hand": do not cast it`). +1,000 chars.

## EDIT A48-3 - RULE 3 + cast list: Damnation at 0-vs-2+ becomes entry 0
**Finding.** Two game-deciding declines of the "best Damnation there is": `...vs-...deck130.jsonl`
seq 23 (turn 14, 14 life, `of which 0` vs `of which 4`, Damnation castable, cast Vision Skeins;
dead turn 17 to those four) and `...vs-...deck152.jsonl` seq 25 (turn 13, 4 life, 0 vs 4, cast
Thraben Doomsayer; dead next combat). RULE 3's sentence existed; the cast list put makers
(entries 1-2) above removal (entry 6) and the pilot obeys the list - amendment #70's shape.
**Before**
```
Your number is 0 and theirs is 2 or more: that is the best Damnation there is. Take it.
...
 1. "Cast Bloodline Keeper {2}{b}{b}" - when no token-maker is on your battlefield.
```
**After**
```
Your number is 0 and theirs is 2 or more: that is the best Damnation there is. Take it, and take
it BEFORE a token-maker and before Vision Skeins - it is entry 0 of the cast list below. At 0
against 4 this deck cast Vision Skeins ... at 4 life against 4 it cast a Thraben Doomsayer into
them and died on the next attack ...
...
 0. "Cast Damnation {2}{b}{b}" - when YOUR battlefield line reads "of which 0 are creatures"
    and THEIRS reads "of which 2" or more. Nothing else first, not a maker, not a dig.
 1. "Cast Bloodline Keeper ..."
```
DECIDING SITUATIONS Damnation line gains "Yours 0 and theirs 2 or more - cast it, above
everything else on the menu." +700 chars.

## EDIT A48-4 - BLOCKING STEP 0: a B row is a blocker; LETHAL writes the assignment first
**Finding.** `...vs-...deck152.jsonl` seq 10 (16 life, `B1. Bloodline Keeper ... may block ... A2
(you kill it, your blocker lives)`, reply `BLOCKS: none` *"We have no blockers"*) and seq 26 (4
life, header `LETHAL if it all connects ... block enough to survive`, `B1. Thraben Doomsayer -
may block A1 (your blocker dies, attacker lives)` on a `deals 4`, reply `BLOCKS: none` *"I have
no way to block"*; blocking A1 = 2 life, not -2). The lethal-assignment rule existed below the
ladder and was never reached. #77 (refute the principle), #79 (the governing line is written
first).
**Before** - the ladder opened at STEP 1 (WHICH BODY).
**After**
```
 - STEP 0 - A "B" ROW IS A BLOCKER. If any row on the screen begins "B1.", you have a blocker,
   and its "may block A1, A2" list is the legality list. "We have no blockers" and "I have no
   way to block" are false whenever that row is printed - this deck wrote both ... When the
   header says LETHAL, the FIRST line of your answer is "BLOCKS: B1:A<the attacker with the
   largest deals N>" - write that line before you write anything else ...
   A row on a token-maker reading "(you kill it, your blocker lives)" is a free kill: take it.
```
+900 chars.

## EDIT A48-5 - DECIDING SITUATIONS: the Vision Skeins dig, keyed to three printed facts
**Finding.** `...vs-...deck125.jsonl` seq 18-31: twelve `Cast nothing right now` at own-turn
cast asks with `of which 0 are creatures`, no maker in hand, no `DRAW PUNISHERS` line, 4-10 mana
open and `Cast Vision Skeins` on every menu; the Skeins went at seq 32 (turn 21). Entry 8 ("Cast
nothing ... only when no entry above appeared") and the KEY CARDS Skeins clause both existed;
neither was keyed to a recognise -> do line.
**After** (new DECIDING line)
```
- Your battlefield line reads "of which 0 are creatures", no Thraben Doomsayer or Bloodline
  Keeper is in "Your hand", no "DRAW PUNISHERS" line is on the screen, and "Cast Vision Skeins
  {1}{u}" is listed: cast it, this window. Twelve consecutive "Cast nothing right now" answers
  ... were twelve turns of not looking for the maker ...
```
+480 chars.

## EDIT A48-6 - RULE -1: the "WHAT THIS COST" paragraph is cut (sunset)
**Finding.** P6 held a second corpus: 2 mulligans (both one-land sevens), 0 third looks, 0 of 21
land-drop declines. The wave-47 file said the paragraph could go on a second clean corpus.
**Before**: the 5-line "WHAT THIS COST: one game shipped seven hands in a row ..." paragraph.
**After**: its last sentence only - "A three-land five-card hand beats every hand you have not
seen." -430 chars.

---

## NOT edited, and why
- **RULE 5 (edicts)**: failed a fourth corpus (seq 18 vs152 at N=4; seq 22 vs130 at N=0 with
  the creature on the stack). Per wave-47 P4 the guide lane is exhausted; the remedy is the
  engine annotation (seat file ENGINE M2). No new prose.
- **RULE 2 (Marsh Flats)**: 3 of 6 fetches outside the printed order, one to a basic (vs152
  seq 7). The rule is a three-line ordered list already; a fourth restatement is not the lever.
- **Cast-list entry 5 (Greaves at 0 creatures)**: 1 instance (vs130 seq 13). Rule stands.
- **The vs162 draw-step death** (9 forced draws at 2 life each from 9 life): no guide line
  changes a mandatory draw step; the missing fact is the COUNT of forced draws, which is a
  render item (general-suggestions R1).
