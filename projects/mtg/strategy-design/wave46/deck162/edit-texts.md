# deck162 — wave-46 edits (before -> after, each with its finding)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck162_strategy.txt` as of master 064792bae
(the wave-45 reviewer revision; the boundary pass made no guide edit here). Revised file:
`wave46/deck162/strategy.txt`. Corpus `matchups-20260826-155842`, 6 games, 129 decisions —
the smallest seat in the corpus, and the record went 5-1 -> **3-3**.
Card facts read off `bin/Res/sets/primitives/mtg.txt` with `/usr/bin/grep`: Fate Unraveler
`auto=@drawfoeof(player):damage:1 opponent` (3/4, {3}{B}); Master of the Feast `abilities=flying`,
`auto=@each my upkeep:draw:1 opponent` (5/5, {1}{B}{B}); Sanguine Bond
`auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`; Exquisite Blood
`auto=@lifelostfoeof(player):life:thatmuch controller`; Perimeter Captain
`auto=@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller` (0/4).
0 deletions. Guide is pure ASCII.

**None of the three losses trace to a wave-45 edit.** Rule 1 is at its best measured value ever
(20/21) and the 162-A LETHAL survival gate passed its first live window. All three losses trace
to things this guide has never contained.

---

## 162-A (HIGH) — RULE 0: play a land every turn

**FINDING — the mechanical cause of the vs146 loss.** `ask` seq **3** and **4** (turn 3, main 1
and main 2) and seq **5** (turn 5): `Land drop: NOT yet used this turn - you can still play a
land`, hand holding Island + Swamp + Drowned Catacomb, answer `Play no land right now` all three
times. The replies name the reason: *"Play Underworld Dreams this turn"* and *"Pass this turn.
Next turn, play a land and cast Underworld Dreams"* — the land drop is being weighed against the
cast, which the prompt's own line already says it is not. Underworld Dreams did not reach the
battlefield until **turn 9**; the seat lost at T16.
Corpus: **35 of 39** land drops taken, 3 of the 4 declines in this one game (the fourth, vs152
seq 9, is the same shape). deck146, the only deck of mine with a land rule, is **43/43**.
This guide has no land rule of any kind.

**BEFORE** — nothing; the guide opens on RULE 1.
**AFTER** — new first rule, above RULE 1:
```
=== RULE 0 - PLAY A LAND EVERY TURN YOU ARE OFFERED ONE. NOTHING OUTRANKS THIS ===
When a "Land drop:" question is put to you and one of its options plays a land, PLAY THE LAND.
The land drop is its OWN decision - the prompt says so on its own line. It costs no mana, it is
not part of your casting decision, and taking it does not spend anything you were going to cast
this turn: you play the land AND cast the spell, on the same turn, in that order. "I am casting
Underworld Dreams this turn, so I will hold the land" and "I will play it next turn when I know
what I need" are the two sentences that lose this deck games before turn six - both are false.
Your punishers cost three to five mana and every land you skip is a turn they arrive late.
Never answer "Play no land right now" or "Hold ..." while any land is playable, and when several
are offered take the one whose colour your hand needs (black for the punishers first).
```
A matching DECIDING SITUATIONS line was added.

---

## 162-B (HIGH) — a lifegain/converter price on the SEND rung

**FINDING — the deciding misplay of the worst loss in the corpus (19 to 0 in one combat).**
`attackers` vs126 seq **17**, turn 11, life 19 vs 15. A1's tag: `Vampire #1 (1/1) (you kill it,
your attacker lives (lifelink: they gain 1)); Vampire #2 (1/1) (...); Vampire #3 (1/1) (...)`.
Every name reads the free line, so the guide's S6 rung — "SEND requires that EVERY name on the
tag reads '(you kill it, your attacker lives)'" — **fired and licensed the swing**. The same
prompt printed `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond`. Fate
Unraveler was blocked by all three lifelink Vampires; they gained 3, Sanguine Bond took 3 off
deck162, and deck126's Exquisite Blood closed the loop: **deck162 19 -> 0, deck126 15 -> 31, in
that single turn.** The earlier seq **14** send is the same gap in miniature — it sent into
`Perimeter Captain (0/4) (neither dies (blocking trigger: they may gain 2))`, which is not a
"(you kill it, your attacker lives)" name at all.
This guide mentions lifelink exactly once, buried inside the *Fate Unraveler stop for the fatal
tag*, never in the send rung; and it never mentions the converter line at all. deck152, which
shares this pool and faces the same deck, has the rule.

**BEFORE** — the ATTACKING section goes straight from the SEND rung to
`ONE FATAL NAME IN THE LIST IS A STOP`.
**AFTER** — inserted between them:
```
A "(blocking trigger:" CLAUSE OR A "(lifelink:" TAIL IS ALSO A PRICE, AND SOMETIMES IT IS THE
WHOLE GAME. This deck gains no life and has no removal for enchantments, so life THEY gain is
progress you cannot undo. Two tags carry that price:
  "(neither dies (blocking trigger: they may gain 2))" - the block costs you nothing and pays
  them; a tag containing one is NOT an all-free tag, and the send rung above does not fire.
  "(you kill it, your attacker lives (lifelink: they gain 1))" - you get the kill and they get
  the life. Against 1/1 lifelink tokens their controller remakes every turn, that trade is
  theirs, not yours.
AND WHEN THE PROMPT ALSO PRINTS "LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs", EVERY
ONE OF THOSE NUMBERS IS DOUBLE AND POINTED AT YOU. That line means life THEY gain also comes
off YOUR total. Three 1/1 lifelink blockers eating one attacker is 3 life to them and 3 life
off you, and their controller may hold a second enchantment that turns that loop into your
death in a single combat. So: with that converter line on the screen, send ONLY creatures whose
A-line prints no "(blocking trigger:" and no "(lifelink:" anywhere in it. If no attacker
qualifies, the answer is ATTACK: none and you win on the draw step instead - your punishers
deal their damage whether or not you attacked, and that is the whole point of this deck.
```

---

## 162-C (MED) — the empty tag is now a named case in the ATTACKING rungs

**FINDING.** `attackers` vs123 seq **15** (opponent battlefield line: `(0 permanents listed, of
which 0 are creatures): (none)`, Fate Unraveler 3/4, opponent at 13) and seq **19** (opponent at
4) both answered `ATTACK: none`, reasoning *"Pass combat to preserve Fate Unraveler as a
blocker"* against a player with no creatures at all. Every rung in the ATTACKING section is keyed
to what the *tag says*; an A-line with **no tag at all** matches no rung, so the guide is silent
on the one attack that is free by construction. Overall attack rate fell from 12 of 12 windows to
**6 of 8**, and both declines are this shape. The render is honest — those opponents genuinely had
no legal blocker — so this is STRATEGY, not PERCEPTION.

**BEFORE**
```
Read each attacker's own "their untapped blockers" tag and answer from it. The tag is the
engine's computed result - do not argue with it, and do not answer ATTACK: none out of a
general wish to keep your board.
SEND IT when every outcome on that creature's tag reads "(you kill it, your attacker lives)".
```
**AFTER**
```
Read each attacker's own "their untapped blockers" tag and answer from it. The tag is the
engine's computed result - do not argue with it, and do not answer ATTACK: none out of a
general wish to keep your board.
NO "their untapped blockers" TAG AT ALL ON A LINE MEANS NOTHING THEY CONTROL CAN BLOCK IT.
That is free, unopposed damage and the answer is SEND, every time, at every life total - it is
the one attack in this deck that has no downside to weigh. "I will keep it back as a blocker"
is not a reason here: they have no creature that can be blocked, or none that can block this
one, which is exactly why the tag is empty. Only the low-life rule at the bottom of this
section overrides it, and only for your LAST untapped body.
SEND IT when every outcome on that creature's tag reads "(you kill it, your attacker lives)".
```

---

## 162-D (MED) — a rung 0 above the free kill

**FINDING.** `blockers` vs146 seq **14**, turn 12, life 12, NOT lethal ("you would be at 4").
One blocker (Fate Unraveler 3/4) offered three attackers: A1 Nadaar (deals 4, "your blocker dies,
attacker lives"), A2 Goblin (deals 2, "you kill it, your blocker lives"), A3 Triumphant Adventurer
(deals 2, "your blocker dies"). It took the free kill on the 2/2; **6 damage walked past; life
12 -> 4**; it died at -2 two turns later. Rule 1 of the ladder is unconditional ("Free kill, every
time") and rule 3's life-8 clause is written *below* it, so rule 3 never gets read — an unstated
precedence collision between two adjacent rungs (amendment #6).

**BEFORE**
```
Run this over the offered block lines, every combat, in this order:
1. A line ending "(you kill it, your blocker lives)" - take that block. Free kill, every time.
```
**AFTER**
```
Run this over the offered block lines, every combat, in this order:
0. HOW MUCH GETS THROUGH IF I TAKE THIS BLOCK? Before rung 1, read the header's "you would be at
   N" and read every attacker's "deals N". When ONE blocker is offered several attackers, the
   free kill is only free if what walks past you is survivable. Subtract the damage you would
   stop from the total: if the number left would put your printed life at 8 OR LESS, block the
   BIGGEST "deals N" you are allowed to block instead, even when that line kills your blocker
   and even when a free kill on a small attacker is sitting right there. A free kill on a 2/2
   while a 4/4 walks past and drops you to 4 is a loss you chose two turns early. Only when the
   damage getting past you leaves your life ABOVE 8 does rung 1 below decide anything.
1. A line ending "(you kill it, your blocker lives)" - take that block. Free kill, every time.
```
Rung 0 deliberately reuses the **8** that rung 3 and rule 4 already carry, so the guide ships one
life threshold, not two.

---

## 162-E (MED) — the keep-it-home rule generalised from Fate Unraveler to the last untapped body

**FINDING.** `attackers` vs146 seq **18**, turn 15, life **3** vs 19, sole attacker
`Master of the Feast (5/5) [flying]` with an empty blockers tag; deck146's board showed an
untapped Silverquill Silencer (3/2) and a Goblin (1/1). It swung, tapping its only blocker, and
died at **-2** on the crack-back. The existing rule names Fate Unraveler only ("At 2 life your
Fate Unraveler is also your blocker"), and Master of the Feast is separately described as "your
fastest clock". The rule was right and it did not cover the card that needed it.

**BEFORE**
```
AND A LOW LIFE TOTAL IS A REASON TO KEEP IT HOME, NOT A REASON TO SWING. At 2 life your Fate
Unraveler is also your blocker: attacking taps it out of the block you are about to need, and
the punisher damage arrives on THEIR draw step whether or not you attacked. "I have to do
something" is not a rule. The rule is the tag.
```
**AFTER**
```
AND A LOW LIFE TOTAL IS A REASON TO KEEP IT HOME, NOT A REASON TO SWING - AND THIS APPLIES TO
WHICHEVER CREATURE IS YOUR LAST UNTAPPED BODY, NOT ONLY TO FATE UNRAVELER. Count the creatures
on YOUR battlefield line that are untapped and not summoning sick. If that count is ONE and
your printed life is 8 or less, that creature does not attack, whatever its tag says and even
when its tag is empty - it is the only thing standing between you and the next combat, and an
attacker is tapped for their whole turn. Master of the Feast is the card this rule is usually
about: a 5/5 flier is your best blocker exactly when you are low, and swinging it for 5 while
they hold three ground creatures is trading your life for damage you did not need. The punisher
damage arrives on THEIR draw step whether or not you attacked; that is what buys the turns.
"I have to do something" is not a rule. The rule is the tag and the count.
```

---

## 162-F (MED) — RULE 0b: trust the "Mana available" line

**FINDING.** `ask` vs152 seq **10** is a 2-option ask that took **350 seconds**, and the reply is a
mana monologue: *"with 3 mana untapped + 1 land to play, I can play a Swamp (untapped) to get
{B}, then cast Underworld Dreams {3}{b} if we play a Swamp, or {1}{u}{u} Dictate if we play an
Island ..."* including a "need 3 generic" step. That is the only generic-mana doubt on my three
seats, and deck162 is the only one of the three without a trust-the-mana-line rule (deck152's
#1 RULE is the tested form). Small, cheap, and it is also the shape behind this deck's share of
the 2-option latency tail.

**AFTER** — new section, immediately after RULE 0:
```
=== RULE 0b - TRUST THE "Mana available" LINE. DO NOT RE-DERIVE YOUR MANA ===
"Mana available: N total (... colours you can make: {u}{b})" gives you two facts and both are
correct: N is what you can spend this window, and the colour list names which COLOURED pips you
can pay. GENERIC IS NOT A COLOUR - Underworld Dreams {B}{B}{B} needs three black sources, but
Howling Mine {2} and Fog Bank {1}{U} take any source for their generic. Never spend a decision
asking "do I have generic mana"; there is no such thing as not having it. And if a spell is
printed as a numbered "Cast ..." option, it IS payable - the game has already checked. Pick it.
```
Costs verified against the primitives: Underworld Dreams `mana={B}{B}{B}`, Howling Mine
`mana={2}`, Fog Bank `mana={1}{U}`.

---

## THRESHOLD WHYS (skill amendment 61) — new numbers only

- **Ladder rung 0 threshold T = 8** because that is the number rung 3 and rule 4 of this same
  section already carry; shipping a second, different life threshold in one ladder is how the two
  rungs come to disagree. Chosen for consistency, not re-derived. Falsifier: a corpus in which
  taking the biggest block at 9-12 life loses walls the seat needed later.
- **Last-untapped-body threshold T = 8**, same number for the same reason, and because the pool's
  common clock is a 4-6 power attacker: at 8 or less one unblocked swing is lethal or nearly so.
  Falsifier: a corpus in which the seat holds its sole body at 8 and loses the race it could have
  won by swinging.
- RULE 0 and RULE 0b carry no numeric threshold.
- Existing thresholds (Rule 1's life-8 Fog Bank exception, Rule 2's "one Howling Mine", the
  never-below-5-cards mulligan floor) are UNCHANGED; their WHYs stand from wave 45.

## RECORDS, not edits
- **Rule 2 brake: 5 of 7.** Both breaks (vs125 seq 8 and seq 16) are the exact shape the guide
  already names — a Howling Mine on the battlefield line, no punisher, a SECOND Mine offered,
  taken. Violated-and-unpunished at n=2 is not grounds to loosen or re-word (amendments 45/64);
  carried as a wave-47 prediction instead.
- **The GANG BLOCK / collapsed-tag rule (162-C of wave 45) got 0 windows** and the whole
  **MULLIGAN section got 0 windows** (deck162 took no mulligan this corpus). Both UNTESTED, both
  untouched.
- The 162-B stop rewording ("DO NOT SEND Fate Unraveler into a tag that CONTAINS 'your attacker
  dies'") also got **0 windows**. UNTESTED, untouched.
