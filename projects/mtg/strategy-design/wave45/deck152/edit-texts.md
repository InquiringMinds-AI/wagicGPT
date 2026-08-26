# deck152 — wave-45 edit texts (before -> after, each with its finding)

Baseline = the LIVE guide `projects/mtg/bin/Res/ai/baka/deck152_strategy.txt`.
Corpus: 6 games (2-4), 253 decisions, **0 fallbacks**. 5 edits, 0 deletions.

**Headline: the engine printed the exact price of the attack that lost the game, and deck152 has
no rule keyed to that string.**

---

## EDIT 152-A (HIGH) — the collapsed tag and the "GANG BLOCK:" clause

FINDING (the deciding misplay, loss vs deck123 at T21). `attackers` seq 19, turn 14, life 18 vs 13.
Luminarch Aspirant had been grown to **7/7** over seven combats and was the seat's only creature:
```
A1. Luminarch Aspirant (7/7) [their untapped blockers: they have 12 untapped creatures that could
    block this one, biggest Thraben Doomsayer (2/2) (you kill it, your attacker lives)
    - GANG BLOCK: their 6 biggest together deal 7, enough to kill this attacker;
      each result above is a LONE blocker only]
```
`ATTACK: A1`. Six blockers were assigned; the log reads **`Your Luminarch Aspirant died`**. It
killed five 1/1 Human tokens and a Thraben Doomsayer that remakes them every turn; deck152 had no
threat left and died at turn 21. The clause is the wave-45 lane-A render working exactly as
shipped. deck152's guide has no rule about the collapsed form or the GANG BLOCK string at all —
deck162 has carried one since wave 44 and it was never mirrored (amendment #32: one teach, every
guide that can see the string; amendment #31: name every FORM a field renders in).
CLASSIFICATION: STRATEGY.

BEFORE: (no such rule; the guide's attack section ends at the Shield Sphere exception)
AFTER (appended to #2 RULE, after the Shield Sphere exception):
```
- THE COLLAPSED TAG AND THE "GANG BLOCK:" CLAUSE - THIS IS HOW YOUR BEST CREATURE DIES.
  On a wide board the tag stops listing their blockers one by one and reads instead:
  "they have N untapped creatures that could block this one, biggest X (you kill it, your
  attacker lives) - GANG BLOCK: their 6 biggest together deal 7, enough to kill this attacker;
  each result above is a LONE blocker only".
  That parenthesis prices ONE blocker. The GANG BLOCK clause prices what actually happens.
  READ THE GANG BLOCK CLAUSE, NOT THE PARENTHESIS. When it says a group of them is "enough to
  kill this attacker", that creature stays home ... unless the lethal check has cleared.
  "But the biggest one dies to me" is exactly the sentence the clause was written to refuse ...
  When the tag is collapsed and prints NO "GANG BLOCK:" clause, use the parenthesis it does print
  - if that parenthesis already says your attacker dies, that is a stop on its own.
  A 7/7 Luminarch Aspirant carrying seven counters is your whole board in one body. Trading it
  for six 1/1 tokens their Thraben Doomsayer remakes every turn is the worst trade in this
  matchup, and the render told you the price before you answered.
```

---

## EDIT 152-B (HIGH) — the per-creature STOP, with the lifelink loophole named and refused

FINDING (P4). Three sends on a CARD carrying `your attacker dies, their blocker lives`, one dead
lord. vs deck162 `attackers` seq 20 (A2) and seq 28 (A2, A3):
```
A2. Intrepid Adversary #1 (3/1) [lifelink] [their untapped blockers: Fate Unraveler (3/4)
    (your attacker dies, their blocker lives (lifelink: you gain 3))]
```
Reply at seq 28, verbatim: "Attack with Wolf and both Intrepid Adversaries. The Wolf kills the 3/4
Fate Unraveler. **The Intrepid Adversaries gain 3 life each via lifelink (total +6 life) and deal 6
damage to the opponent** (reducing them to 4)." Both halves cannot happen: the lifelink gain is what
the BLOCKED fight pays while the Adversary dies; the 6 face damage is the unblocked branch. Log:
`Your Intrepid Adversary died`. At seq 20 there was ONE untapped blocker and TWO attackers, and the
Fate Unraveler ate the Adversary rather than the Wolf that would have killed it — the "we outnumber
them" reasoning the guide's token carve-out licenses does not transfer to cards, because the
defender picks. This is amendment #30's rationale-loophole shape with a NEW lever (lifelink where
wave 44 found vigilance), and amendment #28's "name and refuse the argument in the logs' own words".
CLASSIFICATION: STRATEGY, with a render contribution (engine item E-1).

BEFORE: `... VIGILANCE IS NOT A LICENCE ... THE STOP IS ABOUT CARDS, NOT TOKENS. A Wolf token costs
you nothing when it dies, and one untapped blocker can only block ONE attacker: when the
stop-tagged body is a TOKEN and your attackers outnumber their untapped blockers, send it as bait`
AFTER: adds, before the vigilance clause, `LIFELINK IS NOT A PAYMENT FOR THE TRADE, AND YOU NEVER
GET BOTH HALVES ... Never write "the Adversaries gain 3 life each AND deal 6 damage to the
opponent": that sentence counts the blocked outcome and the unblocked outcome at the same time,
and it is how you lose lords`; and after it, `OUTNUMBERING THEM DOES NOT PROTECT A CARD ... THEY
choose which attacker that blocker stops, and they choose the one their block kills`; and
`ANYWHERE IN ITS LIST` in the opening sentence.

---

## EDIT 152-C (HIGH) — hoist the "BLOCKS: none" release condition above the ladder, and make the floor one comparison

FINDING (the loss vs deck146 at T10 — two ladder answers of `BLOCKS: none` in one ten-turn game).
- `blockers` seq 7, life 17, one blocker Luminarch Aspirant (2/2), both attackers Silverquill
  Silencer (3/2) tagged **"(both die)"**. Rung 2 says take it. Answer: `BLOCKS: none`. The whole
  reply is two lines and contains no reasoning about the block at all.
- `blockers` seq 12, life 11, header "Unblocked, these attackers deal up to 6 - **you would be at
  5** - NOT lethal", the only offered block is rung 3. The guide's floor is **9** ("Take this block
  when N is 9 or less"). Answer: `BLOCKS: none`, again bare. Life 11 -> 5, and it died at **-2** the
  next combat: the 3 damage that block erases is exactly the margin between -2 and +1.
The release condition ("'BLOCKS: none' is correct only when every offered block is rung 3 above
your floor") is the LAST line of a 25-line ladder, farthest from the answer, and it is phrased as a
property of the ladder rather than as a number to compute (amendments #23 and #24).
CLASSIFICATION: STRATEGY.

BEFORE (bottom of the ladder): `"BLOCKS: none" is correct only when every offered block is rung 3
above your floor.`
AFTER: that line is replaced by an arithmetic step, and a new paragraph is hoisted to the TOP of
the ladder, above the survival gate:
```
 WHEN "BLOCKS: none" IS ALLOWED - READ THIS BEFORE THE LADDER, NOT AFTER IT. You may answer
 "BLOCKS: none" in exactly one case: every offered block reads "(your blocker dies, attacker
 lives)" AND the header's "you would be at N" number is 10 or more. In every other case you are
 blocking something ... if ANY offered block reads "(neither dies)", "(you kill it, your blocker
 lives)" or "(both die)", "BLOCKS: none" is WRONG.
...
THE ONE ARITHMETIC STEP, EVERY BLOCK WINDOW: read the header's "you would be at N", compare N to
9, and answer. ... a two-line "BLOCKS: none" answer that never mentions N is an answer that
skipped the ladder.
```

---

## EDIT 152-D (MED) — do not put every counter on one body

FINDING. The wave-44 connection teach WORKED (P6 PASS: 5 placements, 0 onto a Fog-Bank-walled body,
two onto flying Sigarda while the Aspirant's own tag read "(neither dies)"). But vs deck123 the
Aspirant targeted **itself** seven combats running (log seq 18: "You targeted Luminarch Aspirant
(6/6) with Luminarch Aspirant's ability ... got a +1/+1 counter (now 7/7)") and the 7/7 then died to
the gang block in 152-A, taking the entire board with it in one card. The existing rule asks a
connection question but has nothing to say about concentration.

BEFORE: `... Stacking nine counters onto one grounded attacker facing a Fog Bank builds a 10/10
that never deals a point.`
AFTER: adds `AND DO NOT PUT EVERY COUNTER ON ONE BODY EVEN WHEN IT IS CONNECTING. Once a creature
is carrying four or more counters it is your whole board in one card, and one gang block or one
removal spell takes all of it at once. From the fifth counter on, put it on a SECOND creature
instead - two 4/4s beat one 8/8 against a board that can block with six bodies. Never put the
counter on the Aspirant itself while another creature of yours can attack.`

---

## EDIT 152-E (LOW) — the LIFE-TO-DAMAGE CONVERTER line

FINDING (loss vs deck126 at T13, no misplay identified). `attackers` seq 32 printed, for the first
time in this project:
```
LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond. While it is in play, life
ITS CONTROLLER gains also makes the other player lose that much life.
```
deck126 also had **Exquisite Blood** on the same line. Primitives: `Sanguine Bond`
`auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`; `Exquisite Blood`
`auto=@lifelostfoeof(player):life:thatmuch controller` — together an unbounded loop. The opponent's
own log (deck126 seat, its turn 14) shows it firing off a Staff of Nin ping: deck152 went **22 -> 0**
and deck126 **3 -> 26** on one turn. deck152 attacked for everything it had and left them at 3, one
point short of the 12 it needed. It owns no enchantment removal (Fateful Absence hits creatures and
planeswalkers; Brutal Cathar exiles a creature), so there was no answer on the board. The edit is
therefore posture, not a decision rule, and is filed LOW.

BEFORE: (the MATCHUP POSTURE list ends at `vs LIFEGAIN`)
AFTER: adds a `vs A LIFE-TO-DAMAGE CONVERTER` entry naming the render line, stating plainly that
this deck cannot answer an enchantment, and setting the posture: attack with everything not held
home by a stop, every turn, and spend nothing on incremental value — "a turn spent gaining 2 life
off Teferi's +1 while they are at 3 is a turn you needed for the last 3 damage."

---

## EDIT 152-F (MED, mulligan) — rewritten to the #46 odds framing

FINDING + RULING. Amendments #46/#47 (owner, 2026-08-26) require a mulligan section to state the
odds comparison and its two deck facts rather than a coverage-line floor. deck152's shipped floor
is keyed to the engine's "would cover the cost of" line, which is a *castability* test, not a land
test — and this corpus produced the case it handles worst: vs126 s1, one land with three 2-drops
and two 3-drops, coverage line naming nothing (one land pays one pip). That is #47's named judgment
case verbatim, and a coverage-line floor answers it as an automatic ship.
BEFORE: `THE FLOOR, and it overrides everything below it: if the "Playing every land in this hand
would cover the cost of:" line NAMES EVEN ONE CARD, KEEP THE HAND ... Only when it says "would not
cover any spell in it", or when the hand has two or fewer lands, is a mulligan on the table at all.`
AFTER: `MULLIGAN - THIS IS AN ODDS COMPARISON, NOT A CHECKLIST. ... THE QUESTION IS ALWAYS THE
SAME: is this seven more likely to win than a fresh six? ... You run 25 lands, so a third land by
turn three and a fourth by turn five are both likely ... TWO OR MORE LANDS: KEEP ... ONE LAND:
weigh it on what the spells cost ... ZERO LANDS: ship, always. Colour and curve are TIEBREAKERS
between hands that already clear the land count, never a reason to ship a hand that clears it.`
(The existing "THE FORBIDDEN THOUGHT" colour-screw paragraph is kept verbatim — it is the same
ruling stated in the model's own words and it held: no colour mulligan was taken this corpus.)

---

## NOT EDITED (confirmed, earning their tokens)
Deploy floor **33/33** (was 45/47). Counter-placement connection teach: **PASS**. Day/Night line
present on every daybound frame. The own-only-targets stop got **0 windows** — UNTESTED, kept
verbatim, and deck146's identical render-string rule ran 27/27, so nothing argues against it.

---

## THRESHOLD WHYS (skill amendment 61) - reviewer boundary pass, wave-45 step 4
- T = 9 ("you would be at N", the one arithmetic step at every block window) because it is one
  comparison against one printed number, hoisted above the ladder so the exit condition is read
  before the procedure it releases rather than at the bottom of 25 lines.
- T = 2 defenders printing a "(blocking trigger:" clause (the lifegain-wall gate) because the
  first payment is affordable and the second is what turns an attack into a life swing.
- T = "(keeping 5)" (the mulligan chain stop, new this pass) because the London mulligan already
  shaves a seven to six for one card, so a third look buys a fresh seven at the price of two.
- T = 4 lands (never bottom a land unless the keep still holds at least four, new this pass)
  because the keep was priced on the engine's land count; deck162 bottoming a land from a 5-land
  keep is the deviation that made this the tested form.
- T = 2 (the collapsed tag's N / the GANG BLOCK clause) because at N of 2 their bodies may block
  together and the sum, not the biggest, is what your attacker's toughness is measured against -
  the clause priced the fatal attack on the 7/7 Aspirant exactly and in advance.
