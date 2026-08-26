# deck146 — wave-45 edit texts (before -> after, each with its finding)

Baseline = the LIVE guide `projects/mtg/bin/Res/ai/baka/deck146_strategy.txt` as of this review
(it already carries the wave-44 edits AND the post-launch one-land mulligan teach c01ece06a — the
corpus ran the pre-edit guide, so the one-land teach is NOT re-reported as a finding here).
Corpus: 6 games (3-3), 327 decisions, **0 fallbacks**. 5 edits, 0 deletions.

**Headline: deck146 has no ordered cast list, and that is what lost it a game.**

---

## EDIT 146-A (HIGH) — one ordered cast list, with the planeswalker above the venture body

FINDING. deck146 vs deck162, `ask` seq 22, turn 12, life 15 vs 14, `Mana available: 5 total`.
Opponent battlefield line: `Fate Unraveler {3}{b} (3/4)` and `Dictate of Kruphix {1}{u}{u}
[enchantment]` — both legal Kaya -3 targets (primitive `{C(0/0,-3,Loyalty)}:name(-3: Exile
non-land permanent) target(*[-land]|battlefield) moveto(exile)`). The option list held
`Cast Lolth, Spider Queen {3}{b}{b}`, `Cast Kaya the Inexorable {3}{b}{w}`, and
`Cast Barrowin of Clan Undurr {2}{b}{w} (3/3) {right now: returns NOTHING - your graveyard has no
creature cards}`. Answer: Barrowin. Plan: "Use Hive of the Eye Tyrant's mana to cast Lolth next
turn or hold for Kaya if needed, but prioritize developing the board and finishing the dungeon."
At turn 14 its `Mana available` was **1**. Neither walker was ever cast; deck146 died at turn 16.
The guide already says this twice — "The moment you can afford one, CAST IT" and a DECIDING
SITUATIONS line — and lost anyway, because casting advice lives in five different sections
(#1 RULE, DEPLOY YOUR CHEAP THREATS, PLANESWALKERS, VENTURE, CLOSE THE GAME) with no order
between them, and the executor resolves that by narrative fit (amendment #27). deck162, which
carries a single ordered list, runs 17/20 on its own cast-priority rule.
CLASSIFICATION: STRATEGY.

BEFORE: (no such section; the guide goes from YOUR MANA straight to #1 RULE)

AFTER (inserted immediately before `#1 RULE`):
```
THE WHOLE CAST ORDER, ONE LIST. At every "which card do you cast now" ask on YOUR OWN turn, read
your numbered options top to bottom and take the FIRST line that matches. Stop there - do not read
the rest of the list, and do not weigh a later line against an earlier one.
 1. A removal spell (Vanishing Verse, Soul Shatter) when the opponent's battlefield line shows a
    creature or planeswalker you can hit - and for Vanishing Verse, only when its target list names
    one of THEIRS. See #1 RULE.
 2. LOLTH or KAYA, if either is on the list. A planeswalker in hand does nothing. The mana you have
    this turn is the only mana you are sure of: "I will cast her next turn" is how a bomb ends the
    game in your hand, and this deck's mana goes DOWN as often as up. Cast her now and start
    activating her next turn. Kaya's -3 exiles ANY nonland permanent - a creature, an enchantment,
    an artifact, a planeswalker - so she answers the boards Vanishing Verse cannot.
 3. Nadaar, Barrowin or Triumphant Adventurer - a body that ventures.
 4. Silverquill Command (see its own rule for the modes).
 5. Pelakka Predation, when their hand size is 3 or more.
 6. Acererak the Archlich - only here, at the bottom, and only when nothing above matched.
 7. "Cast nothing right now".
Then take the ask again if the game offers it - you can cast more than one spell in a turn.
A CREATURE THAT VENTURES IS NOT A REASON TO SKIP A PLANESWALKER. ... When a 3/3 and a
planeswalker are both on the same list, the walker is the answer. Read the option's own annotation
too - a Barrowin printed "{right now: returns NOTHING ...}" is a vanilla 3/3 this turn.
```
Plus the DECIDING SITUATIONS line gains "even when a creature (Barrowin, Nadaar, Acererak) is on
the same list and even when that creature would venture" (amendment #24: the list nearest the
decision must agree with the rule).

---

## EDIT 146-B (HIGH) — key the lifegain-wall rule to the rendered "(blocking trigger:" clause

FINDING (P1, and the render that makes it fixable). vs deck126 `attackers` seq 16, turn 8:
opponent board is two `Wall of Omens (0/4)`, Nadaar's tag reads "(neither dies)" against both, and
Nadaar was sent. Under the shipped rule ("count the 0/N defenders on their battlefield line: if
there are two or more, the swing PAYS them") that is a violation — and **it cost nothing**:
`Wall of Omens` is `abilities=defender / auto=draw:1 controller`, no block trigger at all, and
Nadaar has vigilance, so it was never even tapped out of blocking. The rule counts bodies when
only the *paying* class matters.
The wave-45 lane-B render now prints exactly which those are, in the tag, in three forms all
observed live this corpus: `Pride Guardian (0/3) (you kill it, your attacker lives (blocking
trigger: they gain 3))` (vs126 seq 7), `(blocking trigger: you gain 3)` on the defending side
(deck126 seat seq 9), and `Perimeter Captain ... (blocking trigger: they may gain 2)` (deck130
seat seq 19 — the "may" matches the primitive's `may life:2 controller`).
Secondary prediction PASSED: total block-trigger life gained against deck146 in that game was
**3**, against 12 and 13 in single turns in wave 44.
CLASSIFICATION: the tag was true; the guide's test was the wrong test.

BEFORE:
```
- OUTNUMBERING THEM DOES NOT MAKE A LIFEGAIN WALL FREE. Some 0/N defenders PAY THEM for
  blocking ... So before you send a "(neither dies)" body into their board, count the 0/N
  defenders on their battlefield line: if there are two or more, the swing PAYS them and
  the outnumber clause does not apply.
```
AFTER:
```
- A LIFEGAIN WALL IS NAMED IN THE TAG ITSELF - READ FOR IT. ... The words to look for are
  "blocking trigger:".
  THE TEST IS THAT CLAUSE, NOT A BODY COUNT. If two or more blockers on this creature's tag print
  a "(blocking trigger:" clause, the swing PAYS them ... If NO blocker on the tag prints that
  clause, their walls cost them nothing and cost you nothing - a 0/4 Wall of Omens is just a wall
  - so the ordinary attack rules apply and a vigilant attacker (Nadaar) can be sent freely.
```

---

## EDIT 146-C (HIGH) — the Hive check restated as two ordered checks, with the repeat offer named

FINDING (P3, FAIL). 78 `becomes beholder` offers, 27 taken.
- **10 taken while the battlefield line already read `Hive of the Eye Tyrant (3/3) [menace]`**
  (wave 44: 12). Repro: vs deck125 seq **98** then seq **100**, both turn 30, Main phase 1, same
  Hive, header "of which 1 is a creature".
- **18 of 27 taken in UPKEEP** (wave 44: ~19), against "In upkeep, pass."
- Six turns carry a **double activation inside one upkeep priority chain**: vs125 seq 87+88,
  124+125, 133+134, 147+148, 154+155, 160+161.
The shipped edit moved 12 -> 10 and 19 -> 18: essentially nothing. The failure mode is not that the
model cannot read the battlefield line, it is that the option returns in the same turn and reads as
a new opportunity (amendment #28's repeated-identical-row drift). The rewrite turns one paragraph
into two ordered checks with a stop, puts the cheapest check (the phase) first, and names the
repeat shape. Paired with engine item E-2, which puts the state on the option row itself.

BEFORE: `- ONE ANIMATION PER HIVE PER TURN, AND NEVER IN YOUR UPKEEP. ... Before you answer
"becomes beholder", find that Hive on your battlefield line: if it already shows (3/3), the answer
is pass.` + a separate `- Do it in your MAIN PHASE, not in your upkeep.`
AFTER: `- BEFORE YOU ANSWER "becomes beholder", RUN THESE TWO CHECKS IN ORDER AND STOP AT THE
FIRST ONE THAT ANSWERS. ... CHECK 1 - WHAT PHASE IS IT? ... If it says Upkeep, the answer is PASS
... CHECK 2 - IS IT ALREADY A CREATURE? ... AND THE OPTION COMING BACK IS NOT A NEW OPPORTUNITY.
The game will offer you "becomes beholder" again later in the same turn ... A second offer in a
turn where you already animated is always PASS.`

---

## EDIT 146-D (MED) — the per-creature STOP: precedence over the Shield Sphere exception, and the defender chooses

FINDING (P4, 0/3 on this deck; 0/7 across the seat). Three windows where a CARD's tag contained
`your attacker dies, their blocker lives`, three sends, two dead cards:
- vs162 seq 20 and seq 27 — `Silverquill Silencer (3/2) [their untapped blockers: Shield Sphere #1
  (0/6) (neither dies); Shield Sphere #2 (0/6) (neither dies); Fate Unraveler (3/4) (your attacker
  dies, their blocker lives)]`, sent both times. Log: `Your Silverquill Silencer died`. The reply
  at seq 20 reads "Attack with Silencer and Goblin to chip damage **and reduce Shield Spheres**" —
  the guide's own Shield Sphere exception is the sentence it obeyed, and nothing in the guide says
  which of the two rules wins when they land on the same A-row (amendment #6).
- vs123 seq 23 — `Nadaar, Selfless Paladin (3/3) [their untapped blockers: they have 12 untapped
  creatures that could block this one, biggest Vampire #1 (4/4) (your attacker dies, their blocker
  lives)]`, sent. Log: `Your Nadaar, Selfless Paladin died`.
The token carve-out is retained and unchanged; the two Goblin sends are not counted as violations.
CLASSIFICATION: STRATEGY (the tags were true and complete).

BEFORE: `- A "their untapped blockers" tag that contains "your attacker dies, their blocker lives"
is a per-creature STOP ... THE STOP IS ABOUT CARDS, NOT TOKENS. ... their one untapped blocker can
only block ONE attacker: send the token WITH the team and their blocker spends itself on the token`
AFTER: adds three named clauses — `THE OTHER NAMES ON THE LIST DO NOT CANCEL IT ... THEY choose
which of those three blocks, and they will choose the one that kills your card ... price the WORST
name on the list, always`; `THIS BEATS THE SHIELD SPHERE EXCEPTION ... The Shield Sphere exception
applies only when EVERY name on the tag is a Shield Sphere`; `OUTNUMBERING THEM DOES NOT PROTECT A
CARD ... their one blocker is spent on whichever attacker they most want dead`.

---

## EDIT 146-E (MED) — Silverquill Command gets an affirmative rung

FINDING. `Cast Silverquill Command` was on the option list **13 times** and cast **0 times**. Two
copies sat in hand through the 54-turn vs125 grind (wave 44 recorded the same shape). Its three
guide rules are all restrictions — a timing restriction, a target restriction, a draw-aim
restriction — and no line anywhere says when to cast it (amendment #36: every section needs an
affirmative rung; a rendered vocabulary needs full coverage, not just its dangerous half).

BEFORE: `- SILVERQUILL COMMAND ({2}{W}{B}, SORCERY - your main phases only): choose TWO of - ...`
AFTER: `- SILVERQUILL COMMAND ({2}{W}{B}, SORCERY - your main phases only): CAST IT. It is a
two-for-one and it is not a card to hold: if it is on the list and nothing above it in THE WHOLE
CAST ORDER matched, take it. There is always a playable pair of modes - with a creature of theirs
on the board, sacrifice plus draw; with a creature of yours attacking this turn, pump plus
sacrifice; with neither, draw aimed at yourself plus return a two-drop from your graveyard. "I will
wait for a better board" is how two Commands sit in your hand while you lose. It chooses TWO of ...`
Plus a DECIDING SITUATIONS line.

---

## NOT EDITED (confirmed, earning their tokens)
Dungeon = LOST MINE 8/8; room branch 14/14; "the only legal targets are YOUR OWN" 27/27;
Agadeem's Awakening 0 casts / 21 offers; the Shield Sphere attrition exception **5/5 and now
proven live** (0/6 -> 0/5 -> 0/4 on their battlefield line); all-"(you kill it, your attacker
lives)" tags sent 6/6; Acererak-vs-removal 6/6 in substance. The wave-43 S4 deletion (hold-back
gate) remains costless. The mulligan STEP-1 floor violation is an **OWNER QUESTION**, not patched.
