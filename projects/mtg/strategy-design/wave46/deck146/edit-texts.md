# deck146 — wave-46 edits (before -> after, each with its finding)

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck146_strategy.txt` as of master 064792bae
(wave-45 reviewer revision + boundary pass 3ca0b2622). Revised file: `wave46/deck146/strategy.txt`.
Corpus: `matchups-20260826-155842`, 6 games, 320 decisions. Card facts read off
`bin/Res/sets/primitives/mtg.txt` and `planeswalkers.txt` with `/usr/bin/grep`.
0 deletions. Guide is pure ASCII.

---

## 146-A (HIGH) — THE WHOLE CAST ORDER gains entries for the two cards it could not rank

**FINDING.** Over 73 own-turn cast asks the first-match rule holds 46 times. Nine breaks are one
shape: `Cast Silverquill Silencer` was taken **8 times** and `Cast Emeria's Call` once, and
neither card is an entry in the list. Most of those Silencer casts were good plays. A first-match
list that cannot name a castable card teaches the executor to stop using the list — amendment #36
applied to the list rather than to a single card. Repro: 146 vs152 seq 4, 18; vs125 seq 4, 7, 102;
vs123 seq 6; vs126 seq 11; vs162 seq 34.

**BEFORE**
```
 3. Nadaar, Barrowin or Triumphant Adventurer - a body that ventures.
 4. Silverquill Command (see its own rule for the modes).
 5. Pelakka Predation, when their hand size is 3 or more.
 6. Acererak the Archlich - only here, at the bottom, and only when nothing above matched.
 7. "Cast nothing right now".
```

**AFTER**
```
 3. Nadaar, Barrowin or Triumphant Adventurer - a body that ventures.
 4. SILVERQUILL SILENCER - your other real body. It is not a venture creature and it has no rule
    of its own; it belongs HERE, above the Command, whenever no body from entry 3 is listed.
 5. Silverquill Command (see its own rule for the modes).
 6. EMERIA'S CALL, when the option is actually listed (that means you can pay its seven).
 7. Pelakka Predation, when their hand size is 3 or more.
 8. Acererak the Archlich - only here, near the bottom, and only when nothing above matched.
 9. "Cast nothing right now".
EVERY CARD YOU CAN CAST HAS A LINE ABOVE. If you find yourself casting something the list does
not name, the list has not been read - go back and take the first line that matches. And "Cast
nothing right now" is the LAST line, not a tiebreaker: it is correct only when every other line
above it failed to match, never because a listed card looks situational.
```

Two cross-references were re-stamped in the same edit (skill 51): the Command rule's header now
reads "entry 5 of THE WHOLE CAST ORDER", and DEPLOY YOUR CHEAP THREATS now notes the Silencer is
entry 4.

---

## 146-B (HIGH) — the Silverquill Command rung gets an observable test

**FINDING.** 35 offers, **4 casts** (wave 45: 13 offers, 0 casts). Ten of the declines are in the
vs125 game at asks where nothing above entry 4 matched — flat violations of the guide's own "if
it is on the list and nothing above it in THE WHOLE CAST ORDER matched, take it". Repro: vs125
seq 18, 44, 48, 62, 80, 82, 85, 127, 129. The existing rule is an imperative whose trigger is a
board judgement; the executor needs a test it can read off the option list.

**BEFORE**
```
- SILVERQUILL COMMAND ({2}{W}{B}, SORCERY - your main phases only) - entry 4 of THE WHOLE CAST
  ORDER: CAST IT.
```

**AFTER**
```
- SILVERQUILL COMMAND ({2}{W}{B}, SORCERY - your main phases only) - entry 5 of THE WHOLE CAST
  ORDER: CAST IT. THE TEST IS THE OPTION LIST, NOT THE BOARD: if "Cast Silverquill Command" is
  printed and no removal spell with a live target and no planeswalker is printed above it, that
  option IS your answer this ask - write it. "The board is not right for it yet" and "I will
  find a better pair of modes later" are the two sentences that leave it in your hand; there is
  no board on which its two best modes are worth less than the turn you spent doing nothing.
```

---

## 146-C (HIGH) — a CHECK 0 above the Hive's upkeep absolute

**FINDING.** The upkeep stop worked (46 offers, 3 taken, from 78/27) — and it cost the vs125 game.
`priority` seq **50** and **51**, turn 23, phase **Upkeep**, opponent at **1 life**, Hive untapped
with 4 sources up: animating makes a 3/3 menace and wins on the spot. Both answers `pass`, because
CHECK 1 says "There is no exception to this." There was no main-phase `priority` window that turn.
Phase census across the seat: `becomes beholder` offered **31x Upkeep, 11x Blockers, 4x Main 1,
0x Main 2** against 17 Main-1 and 13 Main-2 priority windows — the guide's correct main-phase
advice is unactionable in 42 of 46 offers (engine item E-3). The guide half of the fix is a lethal
carve-out that outranks the phase rule and says out loud that the upkeep offer may be the only one.

**BEFORE**
```
  CHECK 1 - WHAT PHASE IS IT? The prompt's "Phase:" line says. If it says Upkeep, the answer is
  PASS. The animation lasts until end of turn either way, and spending {3}{B} before your draw
  step is spending it before you know what you drew. There is no exception to this.
```

**AFTER**
```
  CHECK 0 - IS THIS THE TURN IT WINS? Read the opponent's printed life, then add the power of
  every creature of yours that is UNTAPPED and could attack this turn, plus 3 for the animated
  Hive. If that total is at or above their printed life, ANIMATE NOW and swing, whatever phase
  it is - a manland is a win condition on the turn it closes, and this check outranks both
  checks below. Your Hive's activation is usually offered to you only during your UPKEEP, so
  "I will do it in my main phase instead" can mean you never get asked again this turn: when
  CHECK 0 says the game ends this turn, the upkeep offer is the only offer and you take it.
  CHECK 1 - WHAT PHASE IS IT? The prompt's "Phase:" line says. If it says Upkeep and CHECK 0 did
  NOT fire, the answer is PASS. The animation lasts until end of turn either way, and spending
  {3}{B} before your draw step is spending it before you know what you drew.
```
DECIDING SITUATIONS' beholder line was re-stated in the same edit to carry CHECK 0 first.

---

## 146-D (HIGH) — a printed-life floor on Lolth's 0

**FINDING.** Five `+0: draw card and lose life` activations this corpus, at **16, 12, 9, 5 and 2**
printed life (vs125 seq 56, 83, 98, 116, 128). deck146 died at **0** to a Staff of Nin ping.
Lolth's own ability supplied the last two points. Primitive verified:
`auto={C(0/0,0,Loyalty)}:name(+0: Draw card and lose life) draw:1 controller && life:-1 controller`;
Staff of Nin `auto={T}:damage:1 target(anytarget)` (53 pings landed on this seat this corpus).
The old wording, "take it most turns you are not desperate for that life", contains no observable.

**BEFORE**
```
  - use this first to build a board; 0 draws you a card and loses 1 life (take it most turns you
  are not desperate for that life; do not just pass). -8 is the game-ending emblem.
```

**AFTER**
```
  - use this first to build a board; 0 draws you a card and loses 1 life. -8 is the game-ending
  emblem.
  LOLTH'S 0 HAS A LIFE FLOOR AND IT IS 6. Read your own printed life first. At 6 or less, do NOT
  take the 0 - PASS, or take -3 if she has the loyalty. That "lose 1 life" is real damage you
  are dealing to yourself, it is not offset by the card, and a card drawn at 2 life is a card
  you never get to play. Any opponent permanent that pings - a Staff of Nin, a punisher
  enchantment, one unblocked 1/1 - finishes a game you handed them one point at a time.
```

---

## 146-E (MED) — the friendly-sibling loophole on the per-creature STOP

**FINDING.** `attackers` vs162 seq **27**, turn 12, life 19 vs **12**, power sent 5 (lethal check
unmet). A1 Nadaar's tag: `Fate Unraveler (3/4) (your attacker dies, their blocker lives)`. Sent.
The reply names the reason: *"Attack with Nadaar, Goblin, and Triumphant Adventurer. Triumphant
Adventurer blocks Fate Unraveler (trades)."* A3's own tag reads `(you kill it, your attacker
lives)`, so the model reasoned that a **different attacker of its own** would absorb the blocker.
An attacking creature does not block, and the defender chooses which attacker to stop. The guide
already refuses "outnumbering protects a card"; this is the same falsehood with a sibling A-line
in place of a body count.

**BEFORE** (end of the OUTNUMBERING paragraph inside the STOP)
```
  OUTNUMBERING THEM DOES NOT PROTECT A CARD. "They have one blocker and I am sending three, so
  mine is safe" is FALSE: their one blocker is spent on whichever attacker they most want dead,
  and that is the one your tag says dies.
```

**AFTER**
```
  OUTNUMBERING THEM DOES NOT PROTECT A CARD. "They have one blocker and I am sending three, so
  mine is safe" is FALSE: their one blocker is spent on whichever attacker they most want dead,
  and that is the one your tag says dies.
  AND NEITHER DOES A FRIENDLIER A-LINE. "My Triumphant Adventurer is on the same attack and its
  tag kills that blocker, so it will eat the block" is the same false sentence in a new shape.
  THEY choose which attacker their blocker stops, and they will not choose the deathtouch 1/1 -
  they will choose the card your tag says dies. Read ONLY the tag on the line you are deciding;
  what another A-line's tag says has no bearing on it. Never write a plan in which one of your
  ATTACKERS "blocks" or "trades with" one of their blockers - an attacking creature does not
  block, and it does not choose what it fights.
```
A matching DECIDING SITUATIONS line was added.

---

## 146-F (MED) — "exile is not a bounce" on Vanishing Verse

**FINDING.** The render-string rule went **22/23** (wave 45: 27/27). The one break, vs125 seq 21,
is not a reading failure — it is a rules belief: `CHOICE: 3 (Cast Vanishing Verse) PLAN: Exile
Nadaar with Vanishing Verse to cycle it back to hand and venture the dungeon again`. The option
line said `the only legal targets are YOUR OWN right now: Nadaar, Selfless Paladin`. The existing
clause forbids the act but not the belief behind it.

**BEFORE**
```
    deliberation arguing with the list, and do not exile your own permanent to "use" the card.
```
**AFTER**
```
    deliberation arguing with the list, and do not exile your own permanent to "use" the card.
    EXILE IS NOT A BOUNCE AND IT IS NOT A RE-USE. "I will Verse my own Nadaar to send it back to
    my hand and venture again" is FALSE: an exiled permanent is GONE - not to your hand, not to
    your graveyard, and its enter-the-battlefield trigger never happens again. Exiling your own
    creature costs you the creature AND the removal spell in one action; there is no board on
    which it is right.
```

---

## THRESHOLD WHYS (skill amendment 61) — new numbers only

- **Lolth-0 life floor T = 6** because below it the card is not collectable: at 5 and at 2 this
  seat took the 0 and died at 0 to a repeating 1-damage source. Six is one turn of a 5-power
  attacker plus the 1 the ability itself costs, so the floor holds under the commonest clock in
  the pool. Falsifier: a corpus in which the 0 is declined at 6-8 while the seat is not under any
  clock and then loses to card starvation.
- **Hive CHECK 0 threshold = "opponent's printed life at or below 3 + power of untapped
  attackers"** because 3 is the animated Hive's own power, read off the primitive
  (`{3}{B}: Until end of turn, Hive of the Eye Tyrant becomes a 3/3 black Beholder creature with menace`, borderline.txt). It is not
  a judgement number; it is the arithmetic of the swing.

## RECORDS, not edits
- `DO NOT CAST AGADEEM'S AWAKENING - EVER`: **0 offers this corpus** (was 21/21). UNREACHABLE this
  wave, not deleted; re-check wave 47.
- The wave-45 P2 rule (send a "(neither dies)" body only when no blocker prints `(blocking
  trigger:`) got **0 windows**. UNTESTED, not deleted.
- The mulligan STEP-1 floor got **0 windows at (keeping 5) or lower** on deck146 this corpus; the
  wave-45 violation did not recur. No wording touched.
