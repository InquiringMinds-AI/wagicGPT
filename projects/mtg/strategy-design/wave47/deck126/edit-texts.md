# deck126 (Sanguine Blood) - wave-47 edits, before -> after

Base: `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` (27,533 bytes), verified equal to
`wave46/deck126/strategy.txt` **plus the wave-46 boundary pass C3/C5 edits** - the live file differs
from `wave46/deck126/strategy.txt` by exactly those two hunks, which is the expected baseline.
After: 30,968 bytes (**+3,435**). ASCII-only. Corpus root
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-182155/`.

---

## F1 - TRIBUTE TO HUNGER AT "EXACTLY ONE CREATURE": give it a rank

**Finding, and it is the seat's headline.** 20 Tribute-offering windows, 4 casts. **In 5 of those
windows the opponent battlefield header read "of which 1 is a creature". Tribute was taken 0 times.**

| file | seq | turn | my life | their only creature | chosen instead |
|---|---|---|---|---|---|
| `vs-ai_baka_deck146` | 6 | 5 | 20 | Silverquill Silencer (3/2) | Cast Idyllic Tutor |
| `vs-ai_baka_deck146` | 11 | 7 | 19 | Silverquill Silencer (3/2) | Cast Idyllic Tutor |
| `vs-ai_baka_deck146` | 16 | 9 | 19 | Silverquill Silencer (3/2) | Cast Idyllic Tutor |
| `vs-ai_baka_deck146` | 18 | 11 | 10 | Silverquill Silencer (3/2) | Cast Sorin |
| `vs-ai_baka_deck162` | 12 | 8 | 17 | Fate Unraveler (3/4) | Cast Sanguine Bond |

The Silencer took the seat from 20 to 10 across those four windows. The `vs162` one is inside the
**loss**: Fate Unraveler was one half of a Fate Unraveler + Underworld Dreams + Teferi's Puzzle Box
draw lock (17 -> 7 in one turn cycle), Tribute was the deck's only answer to it, and the Sanguine
Bond cast instead gained the seat nothing for the rest of the game - dead turn 12.
**Diagnosis - STRATEGY, a rank collision between two sections.** RULE #4 lists "the opponent
controls exactly ONE creature" as a cast condition but gives it no position; RULE #7 is the
first-match list the pilot actually executes, and there Tribute sits at entry **8**, below both
enchantments, the Tutor, four walls, Staff, Sorin and the Lantern, with 7b's escape gated on
`life <= 10` **or** `[flying]` **and** on not being able to pay for both. None of the five windows
satisfied 7b as written. A condition with no rank in the ordered list does not fire.
**Fix:** 7b's trigger is widened to the header's one-creature reading itself, at any life total and
regardless of whether both are payable; RULE #4's bullet says the same thing and names the header
field; entry 8 is re-scoped to "two or more"; and the DECIDING SITUATIONS line that read
"only creature is tagged [flying]" now reads the header count.

BEFORE (7b):
```
7b. THE SURVIVAL FLOOR ... When the opponent battlefield line shows exactly ONE creature, "Cast
    Tribute to Hunger" is in this menu, you cannot pay for BOTH Tribute and the combo piece out of
    the "Mana available:" line, and EITHER of these holds -
      (a) your life is 10 or less, or
      (b) that one creature is tagged [flying] ...
```
AFTER (7b):
```
7b. THE SURVIVAL FLOOR ... When the opponent battlefield HEADER reads "of which 1 is a creature" and
    "Cast Tribute to Hunger" is in this menu, cast TRIBUTE TO HUNGER first, even over Exquisite
    Blood or Sanguine Bond - at any life total, whatever that creature is, and whether or not you
    could also pay for the enchantment. One creature means THEIR choice is YOUR choice, so the edict
    is clean removal, and it is the only removal this deck has. It went 0 for 5 this corpus while
    you cast enchantments and tutors over it. The two shapes below are the same rule at its sharpest
    and are kept because each already cost you a game - (a) ... (b) ...
```
BEFORE (entry 8): `Tribute to Hunger, when nothing above it matched and it is in the menu ...`
AFTER (entry 8): `Tribute to Hunger when the header shows TWO OR MORE of their creatures and
nothing above it matched - at one creature it is 7b and it goes first, and at ZERO creatures it is
never cast at all ...`

---

## F2 - THE ZERO STOP: never cast Tribute at an empty opposing board

**Finding.** `vs-ai_baka_deck123` **seq 12, turn 7**, header `of which 0 are creatures`, opponent
board `Underground Sea [tapped]; Isolated Chapel [tapped]; Tundra [tapped]; Intruder Alarm ...` with no creature on it:
`Cast Tribute to Hunger {2}{b} {leaves 0 of your 3 untapped mana sources untapped - casting this
taps you out}`. It sacrificed nothing, gained nothing and spent the whole turn.
**Diagnosis - STRATEGY.** RULE #4's closing sentence said "with an empty opponent board, take a
different option" - a soft instruction with no field named and no place in the ordered list. The
header prints the count; the rule now reads it.

BEFORE: `The engine also lists it when the opponent has no creature at all; with an empty opponent
board, take a different option.`
AFTER: `THE ZERO STOP: the engine lists Tribute even when they have NO creature. Read the header -
"of which 0 are creatures" - and never cast it there. It sacrifices nothing, gains nothing and taps
you out for {2}{B}. You did exactly that on turn 7 of a game, for your whole three mana.`
(Plus a matching DECIDING SITUATIONS line, and the re-scoped entry 8 above.)

---

## F3 - MENACE, and the one-attacker pile-up

**Finding.** `vs-ai_baka_deck146` **seq 40** (turn 18, life 9, header *"Unblocked, these attackers
deal up to 16 - you would be at -7 - LETHAL"*) and **seq 45** (turn 20, life 5, up to 13). Both
answered `BLOCKS: B1:A1, B2:A1, B3:A1, B4:A1, B5:A1` - **all five blockers onto A1, a Silverquill
Silencer (3/2)** - while `A2. Hive of the Eye Tyrant (3/3) ... [menace - only a block by TWO OR MORE
of your creatures counts; one creature alone does not block it at all]` and `A3. Spider (2/1) ...
[menace ...]` went entirely unblocked. Five bodies bought 3 damage of the 16.
**Diagnosis - STRATEGY, and it is a blank cell.** RULE #5 STEP 2 says "Give each attacker one
blocker first, then put the leftovers on the biggest attacker" - which is exactly wrong for menace:
one blocker on a menace attacker stops NOTHING, so the "one each first" heuristic silently wastes a
body there and the pile-up is what is left. The guide has **no menace rule at all**, and the B-line
itself spells the requirement out (`against any attacker whose A-line states a two-or-more
requirement this creature alone does not block it at all, so it stops NOTHING there`). The render is
complete; the guide had nothing to say about it.

AFTER (two bullets appended to RULE #5 STEP 2):
```
 - MENACE NEEDS TWO OR IT NEEDS NONE. An A-line reading "[menace - only a block by TWO OR MORE of
   your creatures counts; one creature alone does not block it at all]" cannot be blocked by one
   body ... So either put TWO blockers on that attacker or put none there. Two 0/4 walls on a menace
   attacker is the cheapest block in this deck ...
 - AND DO NOT STACK YOUR WHOLE BOARD ON ONE ATTACKER while another blockable one goes through.
   Twice in one game you answered a five-attacker window with all five blockers on the SAME 3/2,
   stopping 3 of 16 damage while a menace Spider walked in unblocked ...
```
Plus one DECIDING SITUATIONS line.

---

## F4 - THE CARVE-OUT FLOOR: read the header before benching a blocker

**Finding.** 11 blockers windows. `BLOCKS: none` occurred **once** (wave 46: 0 of 12):
`vs-ai_baka_deck146` **seq 20**, turn 12, header `Unblocked, these attackers deal up to 3 - you
would be at 7 - NOT lethal`, sole blocker `B1. Vampire (1/1) [lifelink] - may block A1 (your blocker
dies, attacker lives (lifelink: you gain 1))`. Neither enchantment was out, so STEP 1's carve-out
matched on its face - but the header's `you would be at 7` is **7, which is 8 or less**, and the
carve-out's own exception says the blocker blocks. A lifelink body with no other job was benched and
3 damage taken at 10 life.
**Diagnosis - STRATEGY.** The carve-out's exceptions are stated once, in a subordinate clause, and
the header number is not named as something to read first. Two words fix it, plus the Vampire case,
which is materially different from a wall: a lifelink Vampire that dies still banks its life.

BEFORE: `... leave that blocker OUT - unless an enchantment of yours is on the battlefield, or the
combat header's "you would be at N" number is 8 or less.`
AFTER: `... leave that blocker OUT - unless an enchantment of yours is on the battlefield, or the
combat header's "you would be at N" number is 8 or less, or the blocker is a lifelink Vampire. Read
the header's N before you bench anything: at 8 or less the carve-out is OFF and everything blocks.
You answered "BLOCKS: none" to a header reading "you would be at 7" with a lifelink Vampire offered
- a body you had no other use for, three damage you did not have to take, and a life gain thrown
away.` (Mirrored into the DECIDING SITUATIONS blocking line.)

---

## Considered and NOT edited (no bytes spent)

- **RULE #7's Chromatic Lantern entry (D24) - it WORKS now, do not touch it.** 57 Lantern windows,
  3 casts. `vs-ai_baka_deck123` seq 7 at `{B} 1` **correct**; `vs-ai_baka_deck152` seq 12 at a
  colour clause with **no {B} entry at all** (`{W} 4, {G} 3`) **correct** - that is D24's positive
  half, `>= 1/1`, discharged; only `vs-ai_baka_deck125` seq 17 at `{B} 4` is wrong. Wave 46 was
  **5/5 wrong and 0/1 on the positive half**. And a SECOND Lantern was cast **0 times** across ~40
  windows where one was already on the battlefield line (was 1/5). Amendment 35: a rule that went
  from 5/5 wrong to 1/3 wrong on its first wave in its new form is not re-written.
- **RULE #1's Battlement discipline (D27).** `Add N green mana with Overgrown Battlement` reached
  the model in 3 windows and was taken **0 times** (wave 46: 1/16). Nothing to pay for.
- **RULE #0.** 17 land-drop windows, 17 taken, 0 declines.
- **Cast-order discipline (P-C5, the wave-46 boundary pass).** PASS. 107 cast windows, 47 answered
  `Cast nothing right now` - but **38 of those 47 had nothing on the menu but a second Chromatic
  Lantern**, which entry 7(b) forbids, and only **4** had a genuinely matched entry available
  (three Pride Guardian declines in `vs146` at life 9-10, one in `vs125`). In wave 46 the same
  measure was **24 of 24**. The roll-call and entries 8/9 did what they were added to do; the raw
  pass-rate rose because the menus were emptier, not because the discipline slipped.
- **The mulligan section.** 6 offers, **0 mulligans**, opening land counts 4/2/3/2/4/4, no bottoming
  window. Entirely UNTESTED this wave; not moved (amendment 66).
