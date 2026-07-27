# deck152 -> strategy-writing skill: wave-32 proposals

Four proposals. P1 is PROMOTION-GRADE (it corrects a rung the skill shipped LAST wave and that
this seat then violated while implementing it). P2/P3 are single-seat with a clean witness. P4 is
an instrument addition.

---

## P1 (PROMOTION-GRADE, HL6 amendment) -- A FREQUENCY CLAIM IS BANNED IN **BOTH DIRECTIONS**, and the archetypal way to break the rule is while FIXING a violation of it

Wave-31 method headline 6 was written from this seat: *"never encode a FREQUENCY or RARITY claim
about a mechanic the baseline corpus did not EXERCISE."* Its worked example was this guide's
*"this rarely happens, so treat them as their day-side bodies"*, falsified by 4-of-6 flips.

**The wave-31 revision -- written by this seat, one pass later, with HL6 in front of it --
replaced that sentence with `WEREWOLVES / DAY-NIGHT - THIS HAPPENS OFTEN, PLAN FOR IT` and
`which happens regularly by mid-game - EXPECT your werewolves to flip`.** That is the same rung
violated with the sign flipped, and wave-32 supports it 1-in-3: werewolves reached this seat's
battlefield in 3 games and flipped in **1** (deck158 s34-s47; deck146 and deck116 stayed Day).
Nine games of combined evidence give 5/9 -- a coin flip, which is precisely the number no
adjective should be attached to.

Why the miss is structural rather than careless: HL6 reads as a rule about EVIDENCE ("do not
claim what you did not observe"), so a reviewer who has now OBSERVED the mechanic believes the
prohibition has lifted and reaches for the opposite adjective. It has not lifted. The defect HL6
names is not the unsupported claim, it is the ADJECTIVE ITSELF -- any frequency word is a licence
for the pilot to stop reading the board, and the pilot obeys it in whichever direction it points.
"Rarely" licenses ignoring the flip; "expect it / plan for it / this happens often" licenses
planning around a face that is not there, which at this seat means planning around a 7/7 the
board does not contain.

**THE RULE (replaces HL6's closing sentence).** A guide may state a mechanic's TRIGGER, its
OUTCOMES, and the STRING on the board that reports its current state. It may never state how
OFTEN the mechanic fires -- not "rarely", not "often", not "regularly", not "expect", not
"usually", not "treat X as the default state" -- **regardless of how many times the corpus
sampled it.** The corpus count belongs in findings.md and never in the guide.

**Reviewer procedure, cheap and mechanical.** When revising any section that a previous wave
flagged for a frequency claim, `grep` the NEW draft for the frequency vocabulary before shipping
it: `rarely|often|usually|regularly|typically|expect|most of the time|by default|almost always|
seldom`. A hit inside a mechanic description is a defect even when the direction is the one this
wave's data supports. Worked replacement, from this seat's wave-32 guide:

> *"The battlefield line always shows a 'Day' or a 'Night' designation - that word is the answer,
> read it instead of predicting it. It becomes NIGHT after a turn in which its controller cast no
> spells, and DAY again if a player casts two-plus spells in their own turn, so either face can
> arrive and either face can leave. Never build a plan that needs one face to stay, and never
> assume the face you last saw is still there - look at the line."*

Trigger, both directions, both outcomes, zero adjectives, and the pilot is pointed at the render
that already carries the truth.

---

## P2 -- A FLOOR RULE OWES ITS **SAFETY** OFF-RAMP AND ITS **SAFETY** COUNTER-ARGUMENT, NOT JUST ITS DANGER ONE

Wave-31 method headline 3 established that a role teach owes a seam-labeled mechanical rule where
the role is cheapest to abandon. This seat's deploy floor was written exactly that way and it
WORKED: the failure mode it was aimed at (holding creatures at 1-3 life out of chump-block fear)
went to zero, declines-against-the-floor stayed flat at 2, and the lowest life at any decline rose
from 1 to 11.

Both survivors, though, sit on the axis the rule never addressed -- **safety, not danger**:

- **deck158 s27** (life 20, one creature, Brutal Cathar offered, declined): *"The opponent has no
  untapped creatures that can attack this turn ... Therefore, there is no immediate threat."* The
  same reply then quotes the guide back at itself -- *"However, the strategy guide says: 'DEPLOY A
  CREATURE EVERY TURN'"* -- and answers `Cast nothing` anyway. The guide gave it a reason to
  deploy when it was LOSING and no reason when it was FINE, so the model supplied its own.
- **deck158 s44** (life 11, Elite Spellbinder offered, declined because lethal was already on the
  board; the attack was declared three decisions later and won the game). A CORRECT decline the
  guide's absolute did not license.

**THE RULE.** A floor rule stated as an absolute owes exactly two things, both written into the
rule and not appended as afterthoughts:
1. **The complete exception list, enumerated and closed** ("correct in exactly TWO situations:
   (a) ... (b) ..."). A floor with one exception invites the model to invent the second, and it
   will invent it at the moment it is most confident -- which is when it is safe or when it is
   winning.
2. **The SAFETY counter-argument, named and refuted in the rule's own voice.** Danger arguments
   ("I might die if I deploy into removal / into deathtouch") get refuted by every guide that
   writes a floor; safety arguments ("nothing can punish me this turn, so nothing is urgent") get
   refuted by none, and they are strictly more available -- every quiet turn produces one. Refute
   it by INVERTING it: *"a turn where they cannot punish you is the CHEAPEST turn you will ever
   get to add a body."*

Generalizable beyond deploy floors to any "always do X" rule (always attack, always level, always
hold interaction): enumerate the exceptions closed, and name the comfortable objection, not just
the frightened one.

---

## P3 -- SCOPE A PROHIBITION TO THE **SURFACE STRING**, NOT TO THE CARD THAT MOTIVATED IT

Wave-31 deleted this guide's false Pathway-flip teach and left a correctly-scoped prohibition:
*"A Pathway's blue back face is NOT available ... so never spend a decision on it."* Zero
Pathway-flip decisions followed -- the teach worked on its named card.

The seat then spent **two priority windows** taking the identical option on a DIFFERENT card:
deck105 **s28** `choice 3` and **s31** `choice 2`, both `Flip Side with Tovolar's Huntmaster`,
against an annotation whose last two words are literally `Ignore this option`. The prohibition
named PATHWAYS; the option string is `Flip Side`, and it rides four card classes (MDFC lands,
MDFC spells, and both transform DFCs).

**THE RULE.** When a guide prohibits or mandates a response to a RENDERED OPTION, scope the
sentence to **the option string the pilot will actually read**, and enumerate the card classes
that emit it -- never to the one card whose misplay generated the note. Procedure: take the
literal option text from the corpus, `grep` it across the seat's logs, list the distinct cards it
appears on, and write the rule at that level. Cheap check: if the prohibition names a CARD and
the render names a MENU LABEL, the rule is under-scoped by construction.

This is the guide-authoring twin of HL5 (path-scoped fixes) -- the same defect one layer up. A
render fix that lands on one emitter and a guide rule that lands on one card fail for the same
reason: neither author enumerated the surface.

---

## P4 (instrument) -- THE **ALT-WIN VISIBILITY CHECK**: for every LOSS, ask what the losing seat could SEE of the clock that killed it

This seat lost a game at **23 life with the opponent at 2** and had no way to know it was losing:
deck105 won on poison, and the corpus contains no poison total on any surface -- no
`Your poison counters: N`, no warning, and infect damage narrated in the ordinary damage form
(`- Damage: 2 dealt by Contagious Nim to you`) with the life total sitting unchanged at 23 for
the whole game. Every existing instrument reported the seat healthy. The cast-nothing split says
the deploy floor held; the attack audit says 4/4 full declarations; the fallback counter says
0/211. **A loss can be invisible to every counter the loop owns when the WIN CONDITION itself is
unrendered** -- the offer-side blind spot of the wave-31 castable-but-not-offered rung, moved
from a single option to a whole victory condition.

**THE RULE, one line per loss.** For every LOSS, name the mechanism the opponent won by, then
locate the render that reports that mechanism's PROGRESS in the losing seat's prompts. Three
outcomes: (a) the render exists and the pilot read it -> normal play analysis; (b) the render
exists and the pilot ignored it -> a guide teach; (c) **the render does not exist** -> a HIGH
render-lane note, and the reviewer must say so explicitly rather than decomposing the play. The
check costs one grep and it is mandatory whenever the loss shape is anything other than "life
reached 0": poison, mill, an alt-win threshold, a dungeon/venture counter, an emblem, an energy
or experience count. Corollary: **a loss at a HEALTHY life total is a visibility question until
proven otherwise** -- treat `my_life` high + `won: false` in the gameend record as an automatic
trigger for this check.

Second-order note for the general skill: a stopgap guide line CAN be written against a missing
render (this seat's new infect posture teaches the pilot to count the narrated damage as a
countdown from ten), but per the wave-30 stopgap-retirement rung it must be tagged in findings
with the ledger item that retires it, and the guide carrying it is **not freezable**.
