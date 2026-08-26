# deck123 (Intruders of Thraben) - wave-44 edit texts

Corpus: matchups-20260826-111937, 6 games, 219 model decisions, 0 fallbacks.
Base = the LIVE guide projects/mtg/bin/Res/ai/baka/deck123_strategy.txt (wave-43 edition).
Three edits. Everything else in the guide is byte-identical to the live file.

---

## EDIT A - COMBAT / attack section rebuilt as four ordered checks
FINDING: EDIT J (the wave-43 lifegain-wall gate) fired 1 of 3 times and both misses lost the
game it was written for. `1787761184-ai_baka_deck123-0x55e881efea80-vs-ai_baka_deck126.jsonl` seq 40 (turn 13) and
seq 59 (turn 15): the opponent battlefield line named Perimeter Captain (and at seq 59 a second
Captain) plus two Sanguine Bonds; the pilot declared all 12 attackers both times. Seq 40 took it
from 20 life to 8 while the opponent went 20 -> 11; seq 59 killed it outright at opponent life
11. Both replies QUOTE Perimeter Captain and Sanguine Bond by name and then out-argue the gate
with a damage sum ("we deal 30, they are at 11, we win").
SECOND FINDING, opposite direction: `1787761200-ai_baka_deck123-0x5652b79129c0-vs-ai_baka_deck162.jsonl` seq 35 - 11 attackers
offered, ONE untapped opposing creature (Master of the Feast 5/5 flier), no lifegain wall
anywhere - answered "ATTACK: none" with the reason "Master of the Feast blocks everything".
One blocker blocks one attacker. The old section's "send all of them" / "ATTACK: none" /
"send the army only when..." clauses are five conditions in prose order, and the executor
resolved them by narrative rather than by order.
CLASSIFICATION: STRATEGY both ways (every surface was honest; see engine item H2 for the half
of it that IS a render gap).
BEFORE (whole block from "=== COMBAT ===" down to "Blocking, in this order:"): the live text
opening `"Your creatures that can attack:" already excludes every creature that cannot attack.
Send all of them.` ... through `... Send it once you have tokens attacking beside it.`
AFTER: four numbered CHECKS, stop at the first that answers - Check 1 the lifegain-wall STOP
(with the multiplication stated as a number: defenders-blocking x Captains x 2 life, doubled by
each Sanguine Bond = 24 life in one combat, and the explicit refusal of the damage-sum
counter-argument), Check 2 Lightmine Field (text preserved), Check 3 the count plus the flat
statement ONE BLOCKER BLOCKS EXACTLY ONE ATTACKER, Check 4 send everything.
Numbers used are read off this corpus's own renders and off the primitives:
Perimeter Captain `auto=@combat(blocking) source(creature[defender]|mybattlefield):may life:2
controller`, Pride Guardian `@combat(blocking) source(this):life:3 controller`, Sanguine Bond
`@lifeof(player) ...:life:-thatmuch opponent`.

## EDIT B - blocking: biggest-attacker-first at lethal, and menace
FINDING: 2 lethal blocking windows this corpus, both misassigned.
`1787761186-ai_baka_deck123-0x5557c3baf5d0-vs-ai_baka_deck146.jsonl` seq 13: life 10, prompt says LETHAL, attackers 3/3/3/1,
one blocker - blocked the 1. Survived at 1 instead of at 3.
Same file seq 17: life 1, four attackers, two blockers, A4 tagged
`[menace - only a block by TWO OR MORE of your creatures counts; one creature alone does not
block it at all]`. The pilot put its 5/5 Bloodline Keeper alone on A4 - a blocker that legally
does nothing - and took 12. (That game was already unwinnable at 1 life; the assignment error
is the finding, not the loss.)
The live guide's "biggest attacker first" lives inside the life-minus-4 branch and was not
reached; nothing in the guide mentions menace at all.
CLASSIFICATION: STRATEGY. Both facts were printed on the rows the pilot was reading.
BEFORE: the blocking list began at ` - Add up the damage of every attacker with no blocker...`
AFTER: two new bullets inserted ABOVE it - "WHEN THE PROMPT SAYS LETHAL, ASSIGN TO THE BIGGEST
ATTACKERS FIRST" keyed to the render's `deals N` field and the `LETHAL if it all connects`
line, with the 10-life 3/3/3/1 arithmetic spelled out; and "MENACE" (two blockers or none).

## EDIT C - four new DECIDING SITUATIONS lines
FINDING: recognition handles for Edits A and B, in the recognise -> do format the section uses.
BEFORE: the section ended at the Lightning Greaves equip line.
AFTER: four lines appended - the wall-board ATTACK: none with "no damage total overrides this";
the one-untapped-blocker send; the lethal biggest-first blocker; the menace two-or-none.

---

## CONSIDERED AND NOT MADE

- MULLIGAN floor. The live floor is "KEEP any 7-card hand with two or more lands." It was
  exercised twice (2-land keeps vs deck130 and vs deck162) and split 1-1: the vs130 keep drew
  no third land, made 7 decisions in 13 turns and lost without casting a spell after turn 5;
  the vs162 keep WON. n=2 is not a floor change. Routed to the owner in the seat file.
- Damnation (19 offers, 0 casts) and Devour Flesh (53 offers, 0 casts). Both declines were
  correct under Rules 3 and 5 every time. This is the construction case the owner has ruled
  OUT OF SCOPE, not a guide defect; no prose written for it.
- Rule 5's N-check. 84 edict offers, 1 cast at N=2 (vs deck152 seq 11). One violation in 84 is
  the same rate as wave 43 and the rule text already states the test three times; adding a
  fourth restatement is churn.
