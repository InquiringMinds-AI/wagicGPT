# deck152 — wave-46 edits (before -> after, each with its finding)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck152_strategy.txt` as of master 064792bae.
Revised file: `wave46/deck152/strategy.txt`. Corpus `matchups-20260826-155842`, 6 games,
320 decisions. Card facts read off `bin/Res/sets/primitives/mtg.txt` with `/usr/bin/grep`:
Sanguine Bond `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent` ({3}{B}{B});
Exquisite Blood `auto=@lifelostfoeof(player):life:thatmuch controller` ({4}{B});
Perimeter Captain `auto=@combat(blocking) source(creature[defender]|mybattlefield):may life:2
controller` (0/4, {W}); Pride Guardian `auto=@combat(blocking) source(this):life:3 controller`
(0/3, {W}). 0 deletions. Guide is pure ASCII. **STEP 1 of MULLIGAN was NOT touched.**

---

## 152-A (HIGH) — the converter rule and the lifegain-wall rule get a stated precedence

**FINDING — the deciding misplay of the vs126 loss.** deck126 had Sanguine Bond on the battlefield
and a wall of Perimeter Captain / Pride Guardian / Wall of Omens. The attackers prompt printed
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` and each A-line printed its
own price (`(you kill it, your attacker lives (blocking trigger: they may gain 2))`). deck152
attacked with everything at seq **29**, **37** and **46**. Life record: seq 29 -> 32, deck152
**20 -> 11** while deck126 went **28 -> 37**; seq 37 -> 40, **11 -> 5** while they went
**31 -> 37**. It lost at **-1**. The guide contains both rules and states no precedence: the
converter rule reads "the correct posture is the most aggressive one available: attack with
everything that is not held home by a stop", which the executor took as a licence over the wall
rule. With a converter of theirs out a blocking trigger is a **2N** swing, so the wall rule must
get stricter, not weaker.

**BEFORE (wall rule)**
```
- OUTNUMBERING THEM DOES NOT MAKE A LIFEGAIN WALL FREE. PERIMETER CAPTAIN gives them 2 life
  every time ANY defender they control blocks; PRIDE GUARDIAN gives them 3 whenever it blocks.
  The tag still reads "(neither dies)" - it does not print their life gain. Count the 0/N
  defenders on their battlefield line before you swing: at two or more, the attack PAYS them
  and the outnumber clause does not apply. Send only what actually gets through (Sigarda and
  Elite Spellbinder FLY) and grind with removal instead of feeding the wall.
```
**AFTER** — the false claim "The tag ... does not print their life gain" is corrected (the wave-45
lane-A/C render does print it), the count is re-keyed from bodies on the battlefield line to
`(blocking trigger:` clauses on the A-line, and the converter multiplier is stated:
```
- OUTNUMBERING THEM DOES NOT MAKE A LIFEGAIN WALL FREE. PERIMETER CAPTAIN gives them 2 life
  every time ANY defender they control blocks; PRIDE GUARDIAN gives them 3 whenever it blocks.
  THE TAG NOW PRINTS IT, inside the same parentheses as the outcome: "(you kill it, your
  attacker lives (blocking trigger: they gain 3))", "(neither dies (blocking trigger: they may
  gain 2))". The words to look for are "blocking trigger:". Count how many blockers on that
  A-line print one: at two or more, the swing PAYS them and the outnumber clause does not apply.
  Send only what actually gets through (Sigarda and Elite Spellbinder FLY) and grind with
  removal instead of feeding the wall.
  AND WITH A CONVERTER OF THEIRS OUT, EVERY "blocking trigger:" IS DOUBLE. This is the one
  combination that has killed this deck from a winning board. A "LIFE-TO-DAMAGE CONVERTER on the
  battlefield: theirs" line plus a "(blocking trigger: they gain N)" tag means that block costs
  you 2N: N life to them AND N life off you. Three walls blocking three attackers is nine life
  gained and nine life lost - one swing can take you from 20 to 11 while THEY go up. So when
  BOTH lines are on the screen at once, the lifegain-wall rule gets STRICTER, not looser: send
  ONLY the creatures whose own A-line prints no "blocking trigger:" anywhere in it - normally
  that means your fliers and nothing else - and never send a body into a wall "to trade" while
  a converter of theirs is in play.
```

**BEFORE (converter posture, MATCHUP POSTURE)** — final sentences:
```
  THEIRS, the game has become a race with a deadline you cannot see, and the correct posture is
  the most aggressive one available: attack with everything that is not held home by a stop,
  every turn, and spend nothing on incremental value. A turn spent gaining 2 life off Teferi's +1
  while they are at 3 is a turn you needed for the last 3 damage.
```
**AFTER**
```
  THEIRS, the game has become a race with a deadline you cannot see. Race it with your EVASION,
  not with your whole board.
  RACE MEANS DAMAGE THROUGH, NOT ATTACKERS DECLARED, AND THE CONVERTER LINE NEVER LICENSES A
  SWING THE TAGS PRICE AGAINST YOU. A creature you send into a "(blocking trigger:" wall deals
  ZERO to their life and hands them N, which the converter takes off you as well - it moves the
  race backwards in both directions at once. So with a converter of THEIRS on the line: send
  every creature whose A-line prints no "(blocking trigger:" and no stop - your fliers first -
  and hold every creature whose tag prints one. Spend nothing on incremental value (a turn
  gaining 2 life off Teferi's +1 while they are at 3 is a turn you needed for the last 3
  damage), but "the most aggressive posture available" is never a reason to overrule a tag.
```

---

## 152-B (HIGH) — the block floor becomes the first line of every block answer

**FINDING.** `blockers` vs146 seq **14**: life 14, one attacker `Silverquill Silencer (6/5) deals
6 [flying]`, one offered block `B1. Elite Spellbinder (3/1) [flying] - may block A1 (your blocker
dies, attacker lives)`, header "you would be at **8** - NOT lethal". The floor is 9. Answer
`BLOCKS: none`; reply verbatim: *"I will take the 6 damage, dropping to 8 life."* It printed the
number and never compared it. deck152 ended that game at 0. The arithmetic step exists in the
guide but sits below a 25-line ladder, farthest from the answer (amendment #24). This is the
second corpus running in which a bare `BLOCKS: none` at N <= 9 cost real life.

**BEFORE**
```
BLOCK LADDER - the block window prints one parenthesis per offered block. Read it and answer from
this ladder, top down. The parenthesis is the answer; do not re-derive the combat.
 WHEN "BLOCKS: none" IS ALLOWED - READ THIS BEFORE THE LADDER, NOT AFTER IT.
```
**AFTER**
```
BLOCK LADDER - the block window prints one parenthesis per offered block. Read it and answer from
this ladder, top down. The parenthesis is the answer; do not re-derive the combat.
 FIRST LINE OF EVERY BLOCK ANSWER, BEFORE ANYTHING ELSE: find the header's "you would be at N"
 number and write it down. N of 9 or LESS means you are blocking something - there is no reading
 of this screen at N of 9 or less on which "BLOCKS: none" is the answer, and that includes the
 case where the only block offered loses your creature. "I will take the 6 damage and drop to 8"
 is that answer written out longhand, and it is the single most expensive sentence this deck
 produces: the creature you saved blocks nothing next turn if the 6 you took was the margin.
 WHEN "BLOCKS: none" IS ALLOWED - READ THIS BEFORE THE LADDER, NOT AFTER IT.
```

---

## 152-C (HIGH) — the all-friendly-parentheses gang tag, and Sigarda's flying

**FINDING (two windows).** (1) `attackers` vs123 seq **18**: `Briarbridge Tracker (3/4) ... Human
(1/1) (you kill it, your attacker lives); Bloodline Keeper (3/3) (you kill it, your attacker
lives) - GANG BLOCK: any 2 of them together deal 4, enough to kill this attacker`. Both printed
parentheses are friendly and the clause is fatal; sent. The wave-45 rule says "READ THE GANG
BLOCK CLAUSE, NOT THE PARENTHESIS" but its worked example has a friendly parenthesis *and* the
gang clause, so the case where **every** parenthesis is friendly is not obviously covered.
(2) `attackers` vs123 seq **24**: Sigarda sent on `they have 9 untapped creatures that could
block this one, biggest Lord of Lineage (5/5) (your attacker dies, their blocker lives)`; reply
reasons *"She flies and is the only creature that can get through unblocked"*.
`Your Sigarda, Champion of Light died`. The collapsed count already excludes what cannot block
her, so nine of their fliers/reach bodies were available.

**BEFORE**
```
  When the tag is collapsed and prints NO "GANG BLOCK:" clause, use the parenthesis it does print
  - if that parenthesis already says your attacker dies, that is a stop on its own.
```
**AFTER**
```
  When the tag is collapsed and prints NO "GANG BLOCK:" clause, use the parenthesis it does print
  - if that parenthesis already says your attacker dies, that is a stop on its own.
  A FRIENDLY PARENTHESIS DOES NOT CANCEL A GANG CLAUSE. "Human (1/1) (you kill it, your attacker
  lives); Bloodline Keeper (3/3) (you kill it, your attacker lives) - GANG BLOCK: any 2 of them
  together deal 4, enough to kill this attacker" is a STOP: both named results are good for you
  and the clause is still the answer, because the clause is what happens when two of them block
  at once and the parentheses are lone-blocker prices only.
  FLYING DOES NOT EXEMPT SIGARDA. She is the best card in the deck and the one this rule exists
  to save. A collapsed tag counts only the creatures that CAN block her, so "they have 9 untapped
  creatures that could block this one, biggest Lord of Lineage (5/5) (your attacker dies, their
  blocker lives)" means nine of their fliers and reach bodies are available and the biggest one
  kills her. Trample and flying change nothing about that parenthesis. She stays home.
```

---

## 152-D (MED) — counter spreading keyed to the size printed on the option row

**FINDING.** Luminarch Aspirant placements vs126 at seq **28, 30, 36, 38, 45, 47, 54** all went to
Sigarda, taking her **4/4 -> 11/11**. The wave-44/45 rule says "From the fifth counter on, put it
on a SECOND creature instead" — counters 5, 6 and 7 went to the same body. The rule asks the
executor to remember how many counters it has placed; the option row prints each creature's
CURRENT size, which is an observable it can read instead. (The rest of the rule held: **0**
counters onto a wall-blocked body, and vs162 seq 13/18 both went to the connecting Tracker.)

**BEFORE**
```
  AND DO NOT PUT EVERY COUNTER ON ONE BODY EVEN WHEN IT IS CONNECTING. Once a creature is
  carrying four or more counters it is your whole board in one card, and one gang block or one
  removal spell takes all of it at once. From the fifth counter on, put it on a SECOND creature
  instead - two 4/4s beat one 8/8 against a board that can block with six bodies. Never put the
  counter on the Aspirant itself while another creature of yours can attack.
```
**AFTER**
```
  AND DO NOT PUT EVERY COUNTER ON ONE BODY EVEN WHEN IT IS CONNECTING. Once a creature is
  carrying four or more counters it is your whole board in one card, and one gang block or one
  removal spell takes all of it at once. Two 4/4s beat one 8/8 against a board that can block
  with six bodies.
  THE TEST IS THE SIZE PRINTED ON THE OPTION ROW, and you never have to count counters to run
  it: each choice in the target list shows that creature's CURRENT size. If the creature you
  were about to pick is already printed at 6/6 or larger, PICK A DIFFERENT ONE - any other
  creature of yours on the list, preferring one that flies. Picking the biggest body on the list
  is the habit this rule exists to break; a Sigarda taken from 4/4 to 11/11 one counter at a
  time is six counters that could have been a second threat. Never put the counter on the
  Aspirant itself while another creature of yours can attack.
```

---

## 152-E (MED) — land-drop RULE 0

**FINDING.** 51 of 56 land drops taken. The five declines (vs146 s7 T5; vs130 s15 and s17 T8;
vs125 s3 T2; vs123 s19 T10) are one shape: the land drop is weighed against the spell the model
means to cast, e.g. *"Cast Intrepid Adversary. Next turn play land"* and *"Cast Katilda ... We
hold Tovolar's Huntmaster for now"*. deck146, the only one of my three decks with an explicit
land rule, is **43/43** — that is the control observation. The prompt already states the land drop
is its own decision; the guide has to say the answer.

**BEFORE** — no such rule (the only mention is "Play a land, cast the biggest creature" inside #2).
**AFTER** — new section inserted immediately above `#2 RULE`:
```
#1a RULE - THE LAND DROP IS FREE AND IT IS NEVER A CHOICE. When a "Land drop:" question is put
to you and its options include playing a land, PLAY THE LAND. It is its OWN decision: it costs
no mana, it competes with nothing, and taking it does not reduce what you can cast this turn -
the prompt says so on its own line. "I would rather cast Katilda this turn" and "I will hold
this land for the colour I need later" are the two sentences that cost this deck games; both
are false, because playing the land and then casting the spell are the same turn. The ONLY
answer that is ever right is "Play <land>", and when several lands are offered take the one
whose colour your hand needs most. Never answer "Play no land right now" or "Hold ..." while a
land is playable.
```

---

## 152-F (MED) — the bottoming land floor gets a card-count qualifier

**FINDING.** vs162: after three mulligans the seat kept a 5-land/2-spell hand at (keeping 4) and
bottomed **Branchloft Pathway, Deserted Beach, Barkchannel Pathway** — three lands — leaving two
lands, Luminarch Aspirant and Briarbridge Tracker. The wave-45 boundary-pass floor ("NEVER bottom
a land unless the hand you keep would still hold at least FOUR") forbids that outright, and the
deviation is plainly odds-favoured: a four-card hand of four lands is not a hand. The floor was
written for a six or a five and needs to say so. Contrast vs130, where the same floor worked
perfectly: a 5-land/2-spell keep at (keeping 6) bottomed a Forest, leaving four.

**BEFORE**
```
NEVER bottom a land unless the hand you keep would still hold at least FOUR - the
keep you just made was priced on the engine's land count, and bottoming a land undoes it at the
very next ask.
```
**AFTER** (appended)
```
THAT FOUR-LAND FLOOR APPLIES ONLY WHILE YOU ARE KEEPING FIVE CARDS OR MORE. Read the ask's own
sentence, "You will be left with a N-card hand". At N of 4 or fewer a four-land hand is not a
hand at all: bottom down to the best MIX you can - two lands and your two cheapest spells - and
lands are the right cards to lose to get there. The floor protects a six or a five; it must
never turn a four-card hand into four lands.
```

---

## THRESHOLD WHYS (skill amendment 61) — new numbers only

- **Counter-spreading trigger T = "6/6 or larger printed on the option row"** because the printed
  size is the only counter-related number the executor can read without memory, and 6/6 is where
  the deck's own bodies (all 1/1-4/4 printed) can only have got by taking two or more counters
  past the fourth. Falsifier: a corpus in which spreading from 6/6 leaves two bodies both too
  small to connect while the stacked body would have got through.
- **Bottoming card-count qualifier N = 4** because 4 is the first hand size at which four lands is
  the whole hand. Read straight off the ask's own "You will be left with a N-card hand" line, so
  it is an observable rather than a judgement. Falsifier: a 4-card keep that bottoms to two lands
  and loses to colour screw where a three-land keep would have cast something.
- The block floor **N = 9** and the deploy-floor rules are UNCHANGED and their WHYs stand from
  wave 45.

## RECORDS, not edits
- **MULLIGAN STEP 1 was violated once** (vs162 seq 3: mulliganed at (keeping 5) holding a land)
  and the seat **won** that game. Per skill 66 a floor is not moved on one window in either
  direction. Wording untouched; carried as prediction P10 for wave 47.
- Deploy floor **50/50** and Fateful Absence's "only legal targets are YOUR OWN" **30/30**. Both
  rules earn their tokens; no edit.
- The trample carve-out in block ladder rung 3 got **no window** for the seventh consecutive
  corpus. UNREACHABLE-IN-POOL per amendment #21, not deleted.
