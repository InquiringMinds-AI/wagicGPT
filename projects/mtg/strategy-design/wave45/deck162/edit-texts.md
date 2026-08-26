# deck162 — wave-45 edit texts (before -> after, each with its finding)

Baseline = the LIVE guide `projects/mtg/bin/Res/ai/baka/deck162_strategy.txt` (wave-44's 162-A
restoration of the affirmative attack rung and the collapsed-tag gate are already in it).
Corpus: 6 games (**5-1**), 145 decisions, **0 fallbacks**. 5 edits, 0 deletions.

**Headline: the wave-44 S6 restoration is VALIDATED — and the loss came through the one rule this
guide never received.**

---

## EDIT 162-A (HIGH) — a LETHAL survival gate above Rule 4

FINDING (the deciding decision of its only loss, vs deck152). `blockers` seq 14, turn 10:
```
Your life: 10. Unblocked, these attackers deal up to 12 - you would be at -2 - LETHAL if it all
connects (at 0 life you LOSE - 0 is not survival): block enough to survive.
A1. Wolf (6/6) deals 6
A2. Intrepid Adversary #1 (3/1) deals 3 [lifelink]
A3. Intrepid Adversary #2 (3/1) deals 3 [lifelink]
B1. Fate Unraveler (3/4) - may block A1 (your blocker dies, attacker lives),
    A2 (you kill it, your blocker lives (lifelink: they gain 3)),
    A3 (you kill it, your blocker lives (lifelink: they gain 3))
```
Answer `B1:A2` — the rung-1 free kill. That survives at **1**. Blocking A1 survives at **4**.
It went into the next combat at 2 life against 20 power and died.
deck162's Rule 4 has no arithmetic gate above the ladder at all. Its only survival clause is at
rung 3 — "Take a dying line instead only when leaving that attacker unblocked would put your
printed life at 8 or below" — which is a *margin* test on the wrong quantity: it cannot separate
"survives at 1" from "dies", and at seq 14 both readings of it point at the free kill.
deck146 and deck152 both received this gate in wave 44 (amendment #25, "put the survival gate above
the ladder — a ladder evaluates, it never allocates"). This guide did not. Amendment #32.
CLASSIFICATION: STRATEGY (the render was complete and the arithmetic was all on screen).

BEFORE: (Rule 4 goes straight from "Read both and act on them" into the numbered list)
AFTER (inserted above the numbered rules):
```
SURVIVAL COMES FIRST, AND IT IS ARITHMETIC - RUN IT BEFORE THE NUMBERED RULES BELOW.
 STEP 1. Each attacker prints its own "deals N". Add them all up - call that TOTAL.
 STEP 2. Your printed life is L. You must block enough that TOTAL minus what you stop is LESS
   than L. At 0 you lose; 0 is not survival.
 STEP 3. With FEWER blockers than attackers you do not get to pick the pretty block. Assign each
   blocker to the biggest "deals N" it is allowed to stop, whatever its parenthesis says ...
 STEP 4. If more than one assignment survives, take the one that leaves you at the HIGHEST life.
   Surviving at 4 and surviving at 1 are not the same answer ... A free kill on a 3-power
   lifelinker while a 6/6 Wolf walks past you is a LOSS you chose, even though the free kill is
   real.
 STEP 5. If NO assignment survives, block the biggest "deals N" you can and play on.
```
STEP 5 is written because both LETHAL windows on my other two seats this corpus were arithmetically
unsurvivable; a gate with no answer for that case invites a re-derivation.

---

## EDIT 162-B (HIGH) — the Fate Unraveler stop, restated against the MIXED tag and against low life

FINDING. `attackers` vs deck152 seq 17, turn 11, **life 2**:
```
A1. Fate Unraveler (3/4) [their untapped blockers: Tovolar's Huntmaster (6/6) (your attacker dies,
    their blocker lives); Wolf #2 (2/2) (you kill it, your attacker lives);
    Wolf #3 (2/2) (you kill it, your attacker lives)]
```
`ATTACK: A1`. Log: **`Your Fate Unraveler died`**. It lost the punisher its own guide names by name
AND its only untapped body, and took 13 through a lone Fog Bank the next combat.
Two things beat the rule. First, the tag is a LIST: two of three names are good for it, and the
guide's wording ("a tag that READS 'your attacker dies'") does not say what to do when the tag also
reads other things. Second, the reply never engages the tag at all — it is 300 words of mana
arithmetic about whether Teferi's Puzzle Box is castable, i.e. the attack was made under pressure
as "doing something".
This is the same defect the other two guides on this seat showed in different clothes; the shared
rule went **0 for 7** corpus-wide.
CLASSIFICATION: STRATEGY.

BEFORE: `DO NOT SEND Fate Unraveler into a tag that reads "your attacker dies, their blocker
lives", and do not send it into "(both die)" either ...`
AFTER: `... into a tag that CONTAINS "your attacker dies, their blocker lives" ANYWHERE IN ITS
LIST ...` plus two new clauses:
```
ONE FATAL NAME IN THE LIST IS A STOP, HOWEVER MANY FRIENDLY NAMES SIT BESIDE IT. ... Two of those
three are good for you and it does not matter: THEY choose which one blocks, and they choose the
Huntmaster. Price the WORST name on the list, always. SEND requires that EVERY name on the tag
reads "(you kill it, your attacker lives)".
AND A LOW LIFE TOTAL IS A REASON TO KEEP IT HOME, NOT A REASON TO SWING. At 2 life your Fate
Unraveler is also your blocker ... the punisher damage arrives on THEIR draw step whether or not
you attacked. "I have to do something" is not a rule. The rule is the tag.
```

---

## EDIT 162-C (MED) — quote the "GANG BLOCK:" clause inside the collapsed-tag gate

FINDING. `attackers` vs deck123 seq 23, turn 12: `Fate Unraveler (3/4) [their untapped blockers:
they have 14 untapped creatures that could block this one, biggest Bloodline Keeper (3/3) (you kill
it, your attacker lives) - GANG BLOCK: their 2 biggest together deal 5, enough to kill this
attacker; each result above is a LONE blocker only]`. The guide says `ATTACK: none` at N >= 2 below
the lethal check. It attacked. The opponent lone-blocked with the Keeper; Fate Unraveler survived,
killed it, and deck162 won the game.
**Odds bad, outcome good, n = 1.** Per amendment #45 a single favourable sample does not discharge
a lane and does not license loosening a gate; per #47 the deviation is graded on the odds it took,
and the engine had just told it the odds. The gate stands. What changes is that the gate now quotes
the clause the engine actually prints — wave 44 wrote the rule against the count N alone, because
the GANG BLOCK clause did not exist yet (amendment #44: adjudicate and write against the emitter's
actual string).

BEFORE:
```
  N = 1  -> the parenthesis is the whole story; use it.
  N >= 2 -> a gang block is available ... the answer for that creature is ATTACK: none.
```
AFTER: keeps both lines and appends:
```
  THE ENGINE OFTEN PRICES THE GANG FOR YOU, IN THE SAME TAG: "- GANG BLOCK: their 2 biggest
  together deal 5, enough to kill this attacker; each result above is a LONE blocker only".
  When you see that clause, it is the answer ... and the friendly parenthesis in front of it is
  the LONE-blocker case only. Do not argue with it because they might block with just the big
  one; they might not, and the clause told you what happens when they do not.
```

---

## EDIT 162-D (MED) — the bottoming rule gets deck146's land carve-out

FINDING (P9, split). `bottom` vs deck152 seq 3: the kept hand was **5 lands, 2 spells** (Underground
Sea, Drowned Catacomb, 2x Swamp, Dismal Backwater + Howling Mine + Fate Unraveler) and it bottomed
**Drowned Catacomb — a land**. The wave-44 rule is an unqualified "Never bottom a land". By
amendment #47 this deviation is graded on the odds, and the odds are plainly with it: a five-land
keep has a land to spare and both spells are wanted. deck146's parallel rule already carries the
carve-out ("unless the hand you keep would still hold at least 4 lands"). Evidence against the
rule, not against the pilot.
The Shield Sphere and punisher clauses (the wave-44 162-C fix) were both honoured and are unchanged.

BEFORE: `Never bottom a land, and never bottom SHIELD SPHERE ... Never bottom your only punisher
either. Bottom your most EXPENSIVE spell first: ...`
AFTER: reorders so the affirmative instruction comes first, then:
```
LANDS: bottom one ONLY when the hand you keep would still hold FOUR OR MORE lands. A five-land,
two-spell keep has a land to spare and the spell is worth more; a four-land keep does not, and
below that a land is never the right card to lose.
```

---

## EDIT 162-E (MED) — mulligan section rewritten to the #46 odds framing

FINDING + RULING. Amendments #46/#47 require the comparison, not a floor. deck162's shipped section
is a bare two-land floor plus tiebreakers. Its one live window (vs152 s1, one land / six spells,
cheapest play Fog Bank at two and everything that matters at three-plus) was answered correctly, so
this is a conformance-to-doctrine rewrite rather than a behaviour fix — but the deck facts it needs
are specific and worth stating: 26 lands, and nothing that advances the plan costs less than three.

BEFORE: `KEEP any 7-card hand with two or more lands ... Tiebreakers ... Your cheapest card costs
ZERO ...`
AFTER: `=== MULLIGAN - AN ODDS COMPARISON, NOT A CHECKLIST === ... The question is always: is this
seven more likely to win than a fresh six? ... You run 26 lands ... Your punishers cost three to
five, so you need to reach four or five mana on time - that is what the land count is for.
TWO OR MORE LANDS: KEEP ... ONE LAND: ship it. Your cheap plays are Shield Sphere {0} and Fog
Bank/Howling Mine at two, and none of them advances the plan ... ZERO LANDS: ship, always.`
The "having already taken 1 mulligan -> keep THIS one" clause and the never-below-5 floor are kept
verbatim.

---

## NOT EDITED (confirmed, earning their tokens)
Rule 1 punisher-first **17/20** (was 13/17). The restored affirmative attack rung: all-"(you kill
it, your attacker lives)" tags **sent 2/2**, and the deck attacked in **12 of 12** attack windows
(was 2 of 6) — the wave-43 deletion that this rung reversed is now doubly falsified and the
restoration is validated. Rule 2 engine-brake 22/26; three of the four breaks are the exact named
shape (vs125 s6/s23/s35) and **deck162 won that game** — reported directionally, not loosened,
per amendment #45. Blocking otherwise clean, including the corpus's only menace-attacker window
(no solo menace block offered or taken).

---

## THRESHOLD WHYS (skill amendment 61) - reviewer boundary pass, wave-45 step 4
- T = 2 (the collapsed tag's N, and the GANG BLOCK clause that prices it) because at N of 2 any
  two of their bodies may block the same attacker and your creature's toughness is measured
  against the SUM; at N = 1 the parenthesis is the whole story.
- T = 8 of your life (the block-always exception, and the survival release) because inside one
  attack of dying the damage a body stops is worth more than the body itself.
- T = 4 lands (never bottom a land unless the keep still holds at least four) because the keep was
  priced on the engine's land count, and this seat bottomed a land from a 5-land keep - the
  deviation was right, which is exactly why the prohibition needed the carve-out rather than
  deletion.
