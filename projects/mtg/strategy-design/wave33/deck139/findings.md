# deck139 (Temur/Sultai Mutants -- mutate) -- wave-33 review

Corpus `matchups-20260819-070739`, binary `bdb30d8e7`, -T 3000, REPPENALTY=1.05, -j 3.
Seat record **0/6** (wave-32: 2/6). L vs146 (-3, T18), vs158 (-19, T13), vs116 (-1, T13),
vs36 (-15, T11), vs152 (-16, T13), vs105 (**alive at 14 life -- opponent POISON alt-win**, T11).
Seat decisions **178 timed / 190 records**; **fallbacks 0** (wave-32: 1). Crash signatures in all
six seat `.stderr`: **ZERO** (fourth consecutive clean corpus). `all_assignments_illegal`: **0**.
All card facts verified against `Res/sets/primitives/borderline.txt` via `/usr/bin/grep`; all
render strings quoted from the translog `prompt` field; the wave-32 comparison numbers are
re-measured from `matchups-20260727-115847` with the same scripts, not copied from prose.

---

## HEADLINE

1. **i9 / N-139n is VALIDATED OUTRIGHT AND THE CLASS IS DEAD. Mulligans 9 -> 0. False
   mulligans 6/9 -> 0/0.** Six games, six `Keep this hand` on the opening seven, zero mulligans,
   zero bottom asks. The belief did **not** survive in any new phrasing on the axis the header
   covers: `zero green sources` **0 hits**, `this hand has zero lands` **0 hits**, and all six
   keep replies cite the engine's counts. The count escalation past the spent exemplar lever
   **worked**, and it is the cleanest single-fix result this seat has produced.
2. **AND THE BELIEF MIGRATED AGAIN -- to a class with NO engine-computed surface, and that
   migration is the 0/6.** The new false belief is **"my creatures are Human"**
   (*"my only creature, Pollywog Symbiote, is a Human. Therefore, I cannot mutate"*). It has
   **5 instances in 3 games and ZERO instances in the entire wave-32 corpus.** It drove the
   mutate take-rate from **6/7 (86%) to 1/6 (17%)** -- the deck's #1 RULE, off in five of six
   games. Section 2.
3. **The proximate cause is a GUIDE REGRESSION this seat shipped in wave 32.** The wave-31 guide
   carried *"**Every creature in this deck is non-Human, so any of your creatures**"* (line 31).
   The wave-32 revision **deleted that sentence** while KEEPING the membership condition
   (*"when a non-Human creature is on your battlefield, take the mutate line"*). Wave-32 corpus,
   sentence present: 0 Human claims, 6/7 mutates. Wave-33 corpus, sentence absent: 5 Human
   claims, 1/6 mutates. One deleted falsity-fact, one wave, the class back. Section 3 + skill.md P1.
4. **The 0/6 is BOTH, and the split is measurable: 1 game pure environment, 2 pure decision, 3
   mixed.** The pool moved hard (deck158 3/6 -> **6/6**, deck146 2/6 -> **4/6** -- *those two are
   exactly the seats deck139 beat in wave 32*; deck116 1/6 -> 3/6; deck105 6/6 -> 2/6, the
   asterisk discharged). Section 6.
5. **i1/i2 (the poison pair) VALIDATED at this seat with a perfect record, and the seat STILL
   lost the vs105 game -- on a decision, not a blind spot.** Poison rendered in **4/4** of the
   game's infect windows; `NOT lethal` emissions on infect windows **0/4**; the wave-32
   invisible-clock class is gone. The game was lost at s24 by assigning **one blocker to four
   attackers** against a prompt line that forbids it two lines above. Section 4.
6. **Latency tail is GONE. >195 s: 1 -> 0. The 213 s bottom-ask specimen cannot recur** -- it was
   a mulligan-belief artifact and there are no mulligans. Median 15,601 -> **14,653** ms.
   Section 7.
7. **Commit counter: 13/178 replies overran their PLAN line (7.3%); commit_retracted 0/178; ZERO
   fatal overruns.** But **6/178 (3.4%) emitted a SECOND coded answer that differed from the
   first and the engine executed the later one** -- and `commit_retracted` cannot see that class.
   Section 8 + general-suggestions.md.
8. **NEW, countable, standing self-harm: 20/20 shocklands paid untapped = 40 life over 6 games.**
   Not a regression (wave-32: 19/20) -- a leak that was invisible while the mulligan spiral owned
   the seat. It cost 8 life in a game lost by 3. Section 5.

---

## (1) i9 / N-139n -- THE ASSIGNED GATE. VERDICT: PASS, CLASS DEAD.

### (1a) The counts

| | wave-31 | wave-32 | **wave-33** |
|---|---|---|---|
| mulligans / 6 games | 15 (2.50/game) | 9 (1.50/game) | **0 (0.00/game)** |
| games at 0 mulligans | 1 | 3 | **6** |
| bottom asks | 15 corpus-wide | 3 at this seat | **0** |
| FALSE mulligans (guide's own keep criterion) | -- | **6 of 9** | **0 of 0** |
| replies containing `zero green` / `zero lands` | -- | 6 | **0** |
| mean kept hand | 5.0 | 5.8 | **7.0** |

All six keeps were CORRECT by the deployed guide's criterion (each hand: >=2 `(land:` cards,
>=1 with `{G}`, >=1 card from the early-play list). The header did not over-correct into keeping
unkeepable hands -- there was no unkeepable hand to test it on, so the FALSE-KEEP direction is
**untested**, recorded as such.

### (1b) What the header actually prints, and that it is TRUE

Verbatim, vs36 seq1:
```
Your hand (7 cards), counted by the engine: 3 lands, 4 spells.
Mana sources among those lands, counted by the engine: {W} 1, {B} 1, {R} 1, {G} 3. A land in your
hand IS a mana source: it produces mana from the turn you play it, so count these when you judge
this hand.
Cheapest spell in this hand: Arboreal Grazer {g} (mana value 1).
Playing every land in this hand would cover the cost of: Arboreal Grazer {g}, Dryad of the
Ilysian Grove {2}{g}, Arboreal Grazer {g}.
```
I checked all six headers against the hand line they sit above. **6/6 arithmetically correct**,
including the two hands with exactly ONE green source (vs158 `{G} 1` off a lone Overgrown Tomb;
vs116 `{G} 1` off a lone Breeding Pool) -- the two hands the wave-32 belief would have thrown
away. Both were kept.

The **satisfiability line** (`Playing every land in this hand would cover the cost of: ...`) is
the part that closes HL7 rung 3 by construction: it does not merely count, it demonstrates a
castable opening exists. It fired in 6/6.

### (1c) The diagnostic signature is gone

The wave-32 grep target -- *the reply RECITES the guide's land list then EXCLUDES a member of
it* -- returns **zero hits on the green-source axis**. There is no recitation, because the pilot
no longer runs the membership procedure at all: it reads `{G} 3` and stops. That is exactly the
mechanism HL6 predicted ("a count is the one form the pilot cannot reason its way around,
because there is nothing per-card left to disqualify").

**One residual, benign, recorded not proposed:** the pilot sometimes over-generalises the count
into membership in the SAFE direction. vs158 seq1, header `{W} 2, {B} 1, {R} 1, {G} 1`, reply:
*"it contains 3 lands (Overgrown Tomb, Sacred Foundry, Plains), **all of which provide green mana
({G})**"*. False about two of the three; it kept, which was correct. No decision this corpus
turned on it.

### (1d) NOT exercised

The leyline pregame path, the bottoming path, and the `having already taken N mulligans` floor
string all went **unexercised** (0 mulligans). The floor teach is UNEXERCISED, **not** superseded
-- see section 3 for why that distinction is now the seat's most expensive lesson.

---

## (2) THE 0/6's #1 DECISION DEFECT: "MY CREATURES ARE HUMAN" (5 instances, 3 games)

### (2a) Ground truth

Every mutate card in deck139 carries, in `borderline.txt`:
```
otherrestriction=type(creature[-human]|mybattlefield)~morethan~0
```
(verified on Gemrazer, Migratory Greathorn, Brokkos, Everquill Phoenix, Snapdax, Illuna.)
**The engine offers a `with its mutate cost` line ONLY when a legal non-Human host is already on
your battlefield.** And no card in the deck is a Human:

| card | `type=` | `subtype=` |
|---|---|---|
| Pollywog Symbiote | Creature | **Frog** |
| Arboreal Grazer | Creature | **Beast** |
| Migratory Greathorn | Creature | **Beast** |
| Gemrazer | Creature | **Beast** |
| Dryad of the Ilysian Grove | Enchantment Creature | **Nymph** |
| Everquill Phoenix | Creature | **Phoenix** |
| Brokkos | Legendary Creature | Nightmare Beast Elemental |
| Illuna | Legendary Creature | Beast Elemental Dinosaur |
| Snapdax | Legendary Creature | Dinosaur Cat Nightmare |
| Nethroi | Legendary Creature | Cat Nightmare Beast |
| Vadrok | Legendary Creature | Elemental Dinosaur Cat |

### (2b) The five instances, verbatim

| # | game/seq | board | reply, verbatim | cost |
|---|---|---|---|---|
| 1 | vs36 s22 | Pollywog out; **`2. Cast Everquill Phoenix with its mutate cost {2}{r}` OFFERED** | *"Option 2 (Mutate) is illegal because it requires a *non-Human* creature on my battlefield to target, and **my only creature, Pollywog Symbiote, is a Human**"* | hardcast at 9 life; dead T11 |
| 2 | vs36 s23 | same, at the `Choose an option` menu | *"I have no non-Human creature on my battlefield to serve as a host (**Pollywog Symbiote is a Human**). Therefore, the mutate option is not legally available regardless of the text on the card"* | -- |
| 3 | vs158 s24 | Grazer + Pollywog out; Greathorn mutate offered at `{1}{g}` | *"Your only creatures are **Arboreal Grazer (Human)** and **Pollywog Symbiote (Human)**, so you **cannot** mutate Greathorn this turn"* | hardcast at 4 life |
| 4 | vs158 s29 | Greathorn + Pollywog out; `2. Cast Brokkos with its mutate cost {1}{g}{g}{u/b}` offered | *"I currently have no valid non-Human target on my battlefield (**Migratory Greathorn is Human, Pollywog is Human**, lands are not creatures)"* | hardcast at 2 life; dead T13 |
| 5 | vs105 s22 | Pollywog out; `2. Cast Illuna with its mutate cost {2}{u}{u}{r/g}` offered | *"you currently have no non-Human creature on the battlefield to mutate onto (**your only creature, Pollywog Symbiote, is a Human**). Therefore, the mutate option is unavailable"* | hardcast at 14 life |

Instance 3 is self-refuting inside one game: at s23 the same model wrote *"you have no other
non-Human creatures on the battlefield (**Arboreal Grazers and Pollywog are the only ones**...)"*
-- naming them as the non-Human creatures -- then at s24 flatly typed both as Human.

### (2c) It is the SAME MECHANISM as N-139n, one class over

The wave-32 diagnostic for class-membership denial is *"the reply reproduces your list and then
excludes a member of it."* Here it excludes members of the class the ENGINE used to build the
option list. The difference from the green-source axis is the SURFACE:

- green sources: guide sentence (present) + per-item tag `(land: taps for {R}{G})` (present) +
  **engine-computed COUNT `{G} 3` (present since i9)** -> **0 misreads**.
- non-Human: guide sentence (**DELETED wave-32**) + per-item tag (**ABSENT** -- `subtype=` is a
  declared primitive field the render never emits; the hand/board line is
  `Pollywog Symbiote {1}{u} (1/3) [creature]`, no type) + count (**ABSENT**) -> **5 misreads,
  5 of 6 mutate windows lost**.
- summoning sickness on an OFFERED attacker: same shape, 1 instance (vs158 s27, *"Pollywog
  Symbiote #1 is summoning sick and cannot attack"* against `A2. Pollywog Symbiote #1 (1/3)`
  listed as able to attack, no tag on the line). `ATTACK: none` at 4 life.

**Three axes, one mechanism, and the axis with the engine-computed aggregate is the only clean
one.** This is the finding the skill lane needs (skill.md P1/P2).

### (2d) The 6th mutate decline is legitimate -- do not count it against the guide

vs146 s28/s29, 8 life, one creature, opponent showing 3: took `Cast Gemrazer {3}{g}` over
`Cast Gemrazer with its mutate cost {1}{g}{g}`, reasoning *"hardcasting the 4/4 first ensures you
have a body to block with this turn"*. That is the deployed guide's **#2 RULE** executed
correctly. No Human claim appears. Scored CORRECT.

**Mutate scorecard: 6 offer windows, 1 taken, 1/1 arrived** (vs116 s15-s20, full six-ask flow,
`[mutated pile - ONE merged creature ...]` at s21, Pollywog under, discount live). Of the 5
declines: **4 false-belief (WRONG), 1 guide-compliant (CORRECT)**. Decision-correctness **2/6**,
against wave-32's **7/7**.

---

## (3) THE ATTRIBUTION: A DELETED FALSITY-FACT, PROVEN BY COUNTERFACTUAL

`diff wave32/deck139/strategy.txt bin/Res/ai/baka/deck139_strategy.txt` -> **IDENTICAL**, so the
corpus ran on exactly the guide this seat shipped last wave.

The wave-31 guide (deployed for the wave-32 corpus) said, lines 30-31:
> *"MUTATE = casting a creature 'for its mutate cost' onto a NON-HUMAN creature you already
> control on the battlefield. **Every creature in this deck is non-Human, so any of your
> creatures** [can be the host]."*

The wave-32 revision replaced that with line 44:
> *"So when a non-Human creature is on your battlefield, take the mutate line."*

The membership CONDITION survived; the membership FACT was deleted. Measured outcome:

| | guide sentence present (w32 corpus) | guide sentence absent (w33 corpus) |
|---|---|---|
| replies asserting a deck creature is Human | **0** | **5** |
| mutate offer windows | 7 | 6 |
| mutate line taken | **6 (86%)** | **1 (17%)** |

The wave-32 audit **checked this exact fact and recorded it TRUE** (findings section 7: *"Every
creature is non-Human ... so the guide's 'any of your creatures can be the host' is TRUE"*) and
the revision dropped it anyway, under the three-part demotion of validated teaches. The class read
as dead **because the teach was holding it dead**, and nothing in the demotion rule distinguishes
"the RENDER absorbed this teach" (safe to delete, HL3's SUPERSEDED verdict) from "the render never
carried it and my own sentence is the only thing holding the line" (never safe to delete).
`subtype=` has **zero** renders in 190 records; the render absorbed nothing.

**This is a guide-lane self-inflicted regression and I am naming it as this seat's own error, not
the pool's.** Restored in strategy.txt, in falsity-fact form, plus a decision rule that does not
depend on the missing surface at all (section 9 / strategy.txt).

---

## (4) i1 / i2 -- THE POISON PAIR AT THIS SEAT: FULL PASS, AND THE GAME WAS STILL LOST

### (4a) Coverage fractions

The vs105 game had **4 infect blocker windows** (s13, s17, s21, s24).

- Poison total visible: **4/4** windows carry `Your life: N. Your poison counters: M of 10.`
  The board snapshot carries the standalone `Poison counters (you): M of 10 - you LOSE the game at
  10 poison counters, whatever the life total is; K more end it.` from the first accrual onward
  (s21-s24, **4/4** of the post-accrual records; zero-suppressed before, as designed).
- Forecast partitioned by damage type: **4/4**. Verbatim s24:
  `Unblocked, these attackers put up to 10 POISON COUNTERS on you - you would be at 13 of 10
  poison - LETHAL if it all connects: at 10 poison counters you lose the game no matter what your
  life total is. Your life total does not answer this; block the poison.`
- **`NOT lethal` emissions on infect windows: 0/4.** The wave-32 game-losing shape is gone.
- Per-attacker discrimination: **every** infect attacker line carries
  `[its combat damage to YOU is dealt as POISON COUNTERS, not life loss - your life total will not
  move and blocking is the only thing that stops it]` -- 40 renders.
- Non-combat poison narrates: `- Poison: you take 1 poison counter - now 2 of 10 (you LOSE at 10
  poison counters; 8 more end it)`. The wave-32 four-silent-events class is **dead**.
- Negative control: the three NON-infect blocker windows elsewhere (vs152 s25/s27, vs116 s33)
  print the ordinary `you would be at -4 - LETHAL` form unchanged. **No cross-contamination.**

**The pilot read all of it.** s13: *"Since I have 0 poison counters, taking even one is permanent
progress toward losing"*. s17: gang-blocked all four bodies onto a single 3/4 to take zero. s21:
*"Unblocked, they would grant you 6 poison counters, bringing your total to 8 out of 10"*. Not one
"still safe at N life" sentence exists in the game. **N-139o is CLOSED at this seat.**

### (4b) The losing decision, and it is a rules error the prompt forbids in the line above it

**vs105 seq24, T11, `kind=blockers`, life 14, poison 3 of 10, four infect attackers (3+3+2+2 = 10
poison = exactly lethal), ONE blocker: Illuna, Apex of Wishes (6/6 flying trample).**

Prompt, verbatim, immediately above the answer line:
```
Assign each blocker to AT MOST ONE attacker (a creature cannot block two attackers), but several
DIFFERENT blockers may gang-block the same attacker.
```
Reply, verbatim first line:
```
BLOCKS: Illuna blocks Cystbearer, Illuna blocks Hand of the Praetors, Illuna blocks Ichorclaw Myr #1, Illuna blocks Ichorclaw Myr #2
```
with the reasoning *"Illuna is the only creature capable of blocking all four attackers ... Since
she can legally block all four (assigning damage from each to her), she stops all poison damage."*

Parser behaviour, correct: first-wins. `choice=1`,
`chosen_text = "Illuna, Apex of Wishes blocks Cystbearer"`, **no fallback**. Three attackers went
unblocked, **7 poison connected, 3 + 7 = 10, game over at 14 life.**

Any single block prevents at most 3, so the game was **already unwinnable at s24**; the
correctable decision is upstream at s22 (4c). But the one-blocker-four-attackers shape is a live,
cheap, guide-addressable defect and it is the exact inverse of the gang-block trap the bundling
was built to fix: the model has learned that *several blockers may gang one attacker* and mirrored
it into *one blocker may split across several attackers*.

### (4c) The upstream decision, vs105 s22 -- the Human belief in its most expensive placement

At 6 mana with Pollywog (a **Frog**) on the battlefield, both lines offered, the pilot hardcast
Illuna citing *"Pollywog Symbiote is a Human"*. **The hardcast was arguably the better PLAY**
(hardcast 4 mana leaves 2 bodies; mutate 5 mana leaves 1 body, and against infect bodies are the
resource). Scored **CORRECT-BY-ACCIDENT with FALSE reasoning** -- it did not compare the two, it
eliminated one on a fabricated rule. The same fabrication cost real value at vs158 s29 and
vs36 s22, where the mutate was strictly better.

---

## (5) THE SHOCKLAND TAX -- 20/20 PAID, 40 LIFE, NEVER MEASURED BEFORE

Every `A choice is required` shockland window at this seat, both waves:

| | wave-32 | **wave-33** |
|---|---|---|
| shock windows | 20 | **20** |
| answered `pay 2 life` | 19 | **20** |
| answered `tap` (enter tapped) | 1 | **0** |
| self-inflicted life | 38 | **40 (6.7 / game)** |

**Not a regression -- a standing leak the mulligan spiral was hiding.** Its price this wave:

- **vs146: 4 payments, 8 life, lost at -3.** The opponent needed 18 turns and dealt 15; the seat
  dealt itself 8. The T9 payment (s22, at 13 life) was followed by `no attackers` and no cast at
  all -- 2 life for mana that was never spent.
- vs36: 4 payments, 8 life, at a seat that died at -15.
- vs105: 3 payments, **6 life -- the ONLY life the seat lost all game** (20 -> 14; it died to
  poison at 14). The s19 payment at 16 life also bought mana never spent that turn.
- **4 of the 20 payments (8 life) bought untapped mana that was not spent on the turn it was paid
  for**, cross-checked by reading the next casting record in each game.

The guide is silent on the pay/tap seam and the wave-32 revision's shockland clause (*"the 2 life
is an enters-the-battlefield choice about tapped-or-untapped, and the land taps for both its
colours either way"*) is TRUE and now, with the mulligan class dead, its only live effect is to
normalise the payment. Fixed in strategy.txt with a rule keyed to a countable the pilot reads.

---

## (6) THE 0/6, DECOMPOSED -- ENVIRONMENT vs DECISION, PER GAME

### (6a) The pool moved, and it moved exactly under this seat

Win table re-derived from `results.tsv` (adj = cap adjudication):

| deck | wave-32 | **wave-33** | delta |
|---|---|---|---|
| 158 | 3/6 | **6/6** | +3 |
| 146 | 2/6 | **4/6** | +2 |
| 116 | 1/6 | **3/6** | +2 |
| 36 | 4/6 | 3/6 | -1 |
| 152 | 3/6 | 3/6 | 0 |
| 105 | 6/6 (ASTERISKED) | **2/6** | -4 |
| **139** | 2/6 | **0/6** | **-2** |

Corpus health: 21 games, **0 timeouts, 0 draws, 4 cap adjudications** (up from 1).
**deck139's two wave-32 wins were vs146 and vs158 -- the two seats that gained +2 and +3.**
A seat that beats only the bottom two of a pool goes to 0/6 when the bottom two become the top
two, with no change in its own play. That is real, and it is roughly half the record.
(Note for synthesis: deck105 dropping 6/6 -> 2/6 with poison rendered and the forecast honest
confirms the wave-32 asterisk -- the perfecta was substantially a render artifact.)

### (6b) Per game

| game | result | attribution | evidence |
|---|---|---|---|
| **vs158** (-19, T13) | L | **ENVIRONMENT + DECISION** | deck158 went 6/6 pool-wide; its Orc army reached 9/9 by T12. Both Human-belief mutate declines here (s24, s29). s24 is a clear loss of value: Greathorn mutating onto a Grazer fetches a basic AND makes one big body. Also the s12 cast-floor violation. |
| **vs146** (-3, T18) | L | **DECISION (self-damage)** | 8 of the 23 life it lost was shockland payments. Mutate decline at s28 was guide-compliant. **Closest game of the six and the one with the clearest correctable cost.** |
| **vs116** (-1, T13) | L | **ENVIRONMENT** | Seat was AHEAD -- opponent at 9 life at T11, seat untouched at 14, attacking each turn with the mutated Gemrazer pile. Opponent cheated **Emrakul, the Aeons Torn (15/15, annihilator 6, protection from coloured spells)** in off Quicksilver Amulet; annihilator stripped six permanents (s27-s32); the seat double-blocked correctly at s33 and survived; died at -1 the next swing. **No decision defect found in this game.** |
| **vs36** (-15, T11) | L | **DECISION** | Human-belief hardcast at s22/s23 at 9 life against a 10/10 Master of Etherium. 8 life to shocklands. Two declined attacks (s20 defensible, s24 marginal). |
| **vs152** (-16, T13) | L | **ENVIRONMENT + DECISION** | deck152's wolf engine reached **19 creatures** by T13; 11 attackers vs 1 blocker is not a decision problem. Upstream: the s13 cast-floor violation removed a fourth wall from exactly the combat that killed it. |
| **vs105** (14 life, T11) | L | **DECISION** | Poison fully rendered and fully read. Lost to the one-blocker-four-attackers rules error at s24 (4b). |

**Split: 1 pure environment (vs116), 2 pure decision (vs146, vs105), 3 mixed with a real decision
component.** The guide is NOT exonerated by the pool shift; the mutate collapse is a guide
regression this seat authored, and the shockland tax is a guide gap.

### (6c) Cast floor -- 2 violations / 39 windows (5.1%)

This deck runs **zero instants and zero sorceries**, so the legitimate-hold branch is structurally
empty (HL10's instant-count rung) and the raw rate IS the finding. Both declines are the same
shape and both are against the floor:

- **vs158 s12, T4, 14 life**, sole cast option `Cast Arboreal Grazer {g}`: declined, *"I have no
  land cards in my hand to trigger Arboreal Grazer's ability, making it a poor play right now (it
  would just be a 0/3 body)"*.
- **vs152 s13, T6, 16 life**, sole cast option `Cast Arboreal Grazer {g}`: declined, *"I have no
  lands in my hand to fetch with Arboreal Grazer, so casting it provides no immediate value (just
  a 0/3 body)"*.

A 0/3 reach wall is not "no value" in a deck whose losses are wide boards; deck152's game ended
with three Grazers holding off eleven attackers. The floor lost to a value argument it does not
refute by name. Fixed with the specific counter-sentence.

### (6d) Attack floor -- 30/33 full participation, 0 zero-power declarations

**33 attacker windows. Creatures tagged `[deals 0 ...]` declared as attackers: 0/33** (N-139j
stays CLOSED, fourth corpus). Non-zero participation full in 30/33. The three partials:
vs36 s20 (2/4 into an untapped 9/9 -- correct decline under the floor's own survival clause),
vs36 s24 (1/3 held at 9 life against tapped blockers -- marginal, 1 damage forgone), and
**vs158 s27, which is not an attack-floor failure at all but the third instance of the
membership-denial mechanism** (2c).

---

## (7) LATENCY -- THE TAIL IS CLOSED

178 timed decisions, zero cache/reuse records.

| | w31 | w32 | **w33** |
|---|---|---|---|
| median ms | 16,867 | 15,601 | **14,653** |
| mean ms | 32,254 | 26,579 | **24,885** |
| p90 ms | 77,085 | 58,896 | **53,204** |
| max ms | 224,282 | 213,393 | **188,084** |
| decisions > 195 s | 6 | 1 | **0** |
| decisions > 100 s | -- | 9 | 10 |
| reply chars p50 / p95 / max | ~1,100 / -- / 15,217 | 925 / 6,393 / 12,845 | **915 / 7,340 / 12,145** |

**The wave-32 survivor is structurally extinct.** That specimen was vs36 s7, the BOTTOM ask,
213 s / 12,845 chars, and the wave-32 finding was that its deliberation WAS the mulligan belief.
i9 removed the mulligans, and with them the bottom asks; there is no bottom record in this corpus.
The `>195 s` count went 6 -> 1 -> 0 across three waves on that one lever. **The bottom-ask
question in the brief is answered: yes, the tail died with i9, and it died because the tail and
the belief were the same object.**

The remaining tail is a different animal and it is not a belief: the slowest three are vs152 s25
(188 s / 12,145 chars, **11 attackers x 3 blockers** damage arithmetic), vs116 s30 (178 s /
10,667, the 4th of six annihilator sacrifices), vs116 s31 (149 s / 9,264, the 5th). All three are
genuinely wide combinatorial decisions on a large board, all three answered correctly, none
retracted. p95 reply length rose 15% and that rise sits entirely in these wide-board records.
This is the length/latency profile OWNER DIRECTIVE 1 targets, and it is NOT the seat's to fix.

---

## (8) THE COMMIT COUNTER (the assigned cross-seat field)

**178 decision records.**

| field | count |
|---|---|
| replies with `post_plan_overrun > 0` | **13 / 178 (7.3%)** |
| of those, `commit_retracted = true` | **0** |
| of those, FATAL (cost the game) | **0** |
| `commit_retracted` anywhere at this seat | **0 / 178** |
| replies with NO `PLAN:` line (protocol-compliant per interim `f46dd58ee`) | **5 / 178** |
| fallbacks of any kind | **0 / 178** |

Overrun magnitudes: 6,993 / 4,984 / 4,943 / 4,803 / 4,437 / 4,372 / 2,942 / 2,107 / 1,247 / 630 /
484 / 434 / 159 chars (vs116 s10/s24/s31, vs152 s5/s18/s22/s25/s26/s27, vs36 s19/s27,
vs105 s18/s23). **Every one committed.** The wave-32 deck158 shape (a correct answer discarded,
Baka answers at 2 life) has **no instance at this seat**. This seat therefore contributes a
**NO** to the promotion trigger.

### (8a) The class the fields CANNOT see -- 6 silent re-commits

**14 replies emitted more than one line-leading coded answer; in 6 of them the LAST label differs
from the first, and the engine executed the LATER one.** `commit_retracted` is fallback-scoped by
construction (`commitRetracted()` at `AIPlayerGPT.cpp:2259` returns false unless `fallback` is
`retracted_choice` / `truncated_abandoned` / `truncated_abandoned_heuristic`), so a re-commit the
parser *accepts* is invisible to both new fields.

| game/seq | kind | first answer | executed answer | direction |
|---|---|---|---|---|
| vs146 s13 | ask | `CHOICE: 2 (Cast nothing right now)` | `CHOICE: 1 (Cast Snapdax)` | **BETTER** -- reversed a cast-floor violation |
| vs152 s21 | priority | `CHOICE: 0 (pass)` | `CHOICE: 1 (Draw 1 with Clue)` | **BETTER** -- a free card taken |
| vs152 s25 | blockers | `B1:A3, B2:A3, B3:A3` | `B1:A3, B2:A3, B3:A4` | **BETTER** -- spread a wall onto a 2/2, saved 2 life |
| vs152 s27 | blockers | `Arboreal Grazer blocks Elite Spellbinder` | `Arboreal Grazer blocks Tovolar's Packleader #1` | **BETTER** -- 7 absorbed instead of 3 |
| vs116 s30 | ask (sacrifice) | `CHOICE: 7 (Pollywog Symbiote)` | `CHOICE: 2 (Sacred Foundry)` | **BETTER** -- kept a creature, gave a land |
| vs158 s28 | blockers | `B1:A1, B2:A2, B3:A2, B4:A3` | `B1:A1, B2:A3, B3:A3` | neutral (re-shuffled a gang-block) |

**5 of 6 improved the decision and 0 of 6 degraded it.** Direct evidence for OWNER DIRECTIVE 1(b)
-- see general-suggestions.md.

---

## (9) THE OTHER STEP-1 ITEMS AT THIS SEAT

- **i7 (`(printed X/Y)` on the board snapshot):** **PASS. 106 printed-tag renders, 0 with a zero
  delta.** Every one carries a real difference (`Pollywog Symbiote (0/2) (printed 1/3)` after an
  infect counter; `Ichorclaw Myr (2/2) (printed 1/1)` under Hand of the Praetors;
  `Orc army (9/9) (printed 0/0)`). The wave-32 Moonrage-Brute class has no analogue here.
- **i8 (name-form `BLOCKS:` parse):** **PASS, 3 witnesses, all parsed.** vs36 s21
  (`BLOCKS: Dryad of the Ilysian Grove: Master of Etherium, Pollywog Symbiote: Silver Myr` -- the
  colon-inside-the-label shape that broke the wave-32 reconciler) resolved to the correct two
  assignments; vs152 s27 and vs105 s24 name-form lines both resolved. **`unparsed_reply`: 0.
  `all_assignments_illegal`: 0** -- the new fallback class did not fire at this seat, and the
  wave-32 N-139s repro shape did not recur.
- **i5 (`{0}` cost tokens):** visible and correct on real zero-cost cards from this seat's side of
  the board -- `Mox Sapphire {0}`, `Mox Jet {0}`, `Ornithopter {0}` (45 renders), zero
  confabulation about them in any reply. **One over-reach recorded:** the emitter now prints `{0}`
  on permanents that have NO mana cost rather than a cost of zero -- `Tovolar's Packleader {0}`
  (a transformed DFC back face, 3 renders) and the day/night designations `Day {0}` / `Night {0}`
  (5 renders). Decision-neutral here (all opponent-side, never castable by this seat).
  -> notes.md N-139w (LOW).
- **i6 (amass previews):** **NOT EXERCISED at this seat.** The amass annotation rides the caster's
  own offer lines; from the defender's chair the army arrives as `Orc army (7/7) (printed 0/0)
  [counters: 7x +1/+1]`, which is complete and correct. 0 occurrences, 0 defects -- report as
  no-coverage, not as a failure.
- **i3 (`[-multicolor]`), i4 (`deferFlexibleSource`):** not exercised at this seat.
- **N-139m (HARNESS lane, known-open -- CITED, NOT RE-DIAGNOSED):** trigger predicate (a mutate
  spell cast with Pollywog on this seat's battlefield) fired **once** (vs116 s15), paid the
  discounted `{g}{g}` against `{1}{G}{G}`, fired the loot **twice** (two Pollywogs, s17+s18), and
  **ARRIVED** (s21 `Gemrazer {1}{u} (4/4) [trample, reach, mutate] [mutated pile - ONE merged
  creature ...]`). 1/1 live. Nothing new; the harness-lane scoping stands.
- **N-139l (merged-pile cost pip drift):** RECURS, 4th corpus. `Gemrazer {1}{u} (4/4)` renders
  Pollywog's cost on the pile. Decision-neutral. LOW, unchanged.
- **N-139g (under-card statics):** confirmed a third time -- Pollywog is the under card at
  vs116 s20 and the discount is live in the same pile.

---

## (10) HL1 -- RETROACTIVE CARD-FACT LEDGER ON THE GUIDE I AM SHIPPING

Terminal pass over `wave33/deck139/strategy.txt`, one row per checkable claim, each adjudicated
against `borderline.txt` or a corpus render. The wave-32 ledger's TRUE rows were re-checked line
by line and all remain TRUE; rows below are the load-bearing and newly-written ones.

| claim as written | adjudicating line | verdict |
|---|---|---|
| "Every creature in this deck is NON-HUMAN" | `subtype=` of all 11 creatures (table in 2a): Beast/Frog/Nymph/Phoenix/Nightmare/Dinosaur/Cat/Elemental/Giant. No Human. | **TRUE** |
| "the engine prints 'with its mutate cost' ONLY when a legal non-Human host is already on your battlefield" | `otherrestriction=type(creature[-human]\|mybattlefield)~morethan~0` on all six mutate cards | **TRUE** |
| "one creature blocks AT MOST ONE attacker" | prompt line, present in all 20 blocker renders, verbatim | **TRUE** (render-quoted) |
| "Pollywog has no mutate and does not trigger itself" | `abilities=` empty; `auto=@movedTo(creature[mutate]\|mystack):_LOOT_` | **TRUE** |
| "Nethroi returns creatures from your graveyard - read the target list you are given" | `target(<anyamount>creature[power<=10]\|mygraveyard)`; engine has no total-power budget (N-139q) | **TRUE as written** (deliberately generic) |
| "Vadrok's trigger wants a noncreature card of MV 3 or less in your graveyard and this deck has none" | decklist: only nonland noncreature is Nissa, MV 5 | **TRUE** |
| "Everquill's trigger makes a small artifact, not a creature" | `token(Feather Eve)`, `type=Artifact` | **TRUE** |
| "Arboreal Grazer {G} 0/3 reach, ETB put a land from hand onto the battlefield tapped" | `mana={G}`, `abilities=reach`, ETB Put in Play | **TRUE** |
| "Dryad {2}{G} 2/4, extra land drop, all your lands tap for every colour" | `maxPlay(land)+1`, `lord(land\|myBattlefield) transforms((basic ...))` | **TRUE** |
| "Gemrazer 4/4 reach trample, mutate {1}{G}{G}" | `abilities=reach,trample,mutate`, `other={1}{G}{G}`, 4/4 | **TRUE** |
| "Migratory Greathorn 3/4, mutate {2}{G}, fetches a basic on mutate only" | `other={2}{G}`, 3/4, `@mutated(this)` fetch | **TRUE** |
| "Snapdax 3/5 double strike, mutate {2}{BR}{W}{W}: 4 damage AND gain 4 life" | `other={2}{BR}{W}{W}`, `life:4 controller` + `damage:4` | **TRUE** |
| "Illuna 6/6 flying trample, mutate {3}{RG}{U}{U}" | `other={3}{RG}{U}{U}`, `abilities=flying,trample,mutate` | **TRUE** |
| "Brokkos 6/6 trample, mutate {2}{UB}{G}{G}, castable from your graveyard with its mutate" | `other={2}{UB}{G}{G}`, graveyard-cast clause | **TRUE** |
| "10 poison counters lose you the game at any life total; the count is printed" | render, 4/4 windows, `Poison counters (you): M of 10` | **TRUE** (was a STOPGAP; now render-backed) |
| "the 2 life is an enters-the-battlefield choice about tapped-or-untapped" | `pay 2 life [... enters the battlefield UNTAPPED]` / `tap [decline the payment; ... enters TAPPED]` | **TRUE** |

**CLASS SWEEP (HL2 rung 3):** the false-fact class fixed this wave is *"the pilot is asked to
verify a card property the render does not print."* Grepped the whole shipping guide for every
such property: creature TYPE (fixed -- fact restored + engine-offer rule), summoning sickness on
an offered attacker (fixed -- falsity-fact added), Human-ness of the mutate HOST (same fix),
"non-Human" as a condition anywhere else (0 remaining unbacked occurrences). No other guide
sentence asks the pilot to evaluate an unrendered property.

**HL3 QUOTED-STRING SWEEP:** every string the shipping guide presents as a quotation was grepped
against this corpus's 190 `prompt` fields. All hit except one: **`having already taken`, 0 hits**
-- because there were zero mulligans, not because the emitter died (it lives at
`AIPlayerGPT.cpp:8476`). **UNEXERCISED, not superseded; the sentence stays.** After section 3 that
distinction is the seat's governing rule.

**HL6 FREQUENCY-WORD PRE-SHIP GREP** (`rarely|often|usually|regularly|typically|expect|most of the
time|by default|almost always|seldom`): the first draft produced **three hits, all fixed before
ship** -- `take them early and often` -> *"take their mutate line every time it is offered"*
(a countable instruction replaces an adverb), `usually a missing coloured pip` -> *"a missing
coloured pip does this"*, and `ANSWER "tap" by default` -> *"ANSWER 'tap' unless one of the next
two lines applies"*. **The shipping guide greps clean (exit 1).** Worth recording that two of the
three were in text I wrote THIS wave -- HL2's blind spot (a) applies to HL6 as well.

**LENGTH:** 11,818 -> 15,812 bytes (+34%, 154 -> 207 lines). Deliberate and defended: the wave's
own evidence is that this seat's last revision paid for concision with the deck's #1 rule. Under
L-18-1 (length-only watch) I am flagging the growth rather than absorbing it silently; if wave-34
validates the repair, the mulligan block is the first candidate to compress, NOT the non-Human
block.

---

## (11) ROTATION -- EXIT VERDICT: **HOLD**

The wave-32 gate was: *"if wave-33 shows the mulligan rate at or under the field with no
false-green-source mulligan, the guide freezes and the seat exits -- conditional on N-139n."*

| gate condition | status |
|---|---|
| N-139n ships and the class dies | **MET, decisively** -- 0 mulligans, 0 false, 0 recitation-then-exclusion on that axis |
| mulligan rate at or under the field | **MET** -- 0.00/game, the pool floor |
| crash gate | **MET** -- 0 signatures, 4th consecutive corpus |
| combat-latency teach proven | **MET** -- >195 s went 1 -> 0; the specimen class is structurally extinct |
| N-139m | **CITED, harness lane** -- 1/1 live arrival, not a live risk at this seat |
| HL1 audit discharged | **MET** -- ledger above, run as a terminal pass on the shipping guide |
| **revised guide validated** | **FAILED, and failed on a defect the wave-32 revision INTRODUCED** -- the deck's #1 rule went 7/7 -> 2/6 |
| **guide frozen** | **NOT MET** -- strategy.txt is revised again this wave |

**Recommendation: HOLD deck139 for wave 34. Exit is NOT defensible.** Not because of the 0/6 --
half of that is a pool that moved out from under a seat whose only two wins were against the two
decks that improved most, and one loss is a cheated 15/15 annihilator the seat had no answer to
while ahead on board. It is not defensible because **the wave-32 revision broke the deck's #1 rule
and the wave-33 repair is untested.** A seat cannot exit on the same wave it discovers its own
guide caused a 7/7 -> 2/6 collapse; the repair needs exactly one corpus.

**Wave-34 exit gate, stated falsifiably:** mutate take-rate back to >=5/6 of offered windows with
**zero** replies asserting a deck creature is Human or that an offered attacker is summoning sick;
mulligan class stays at 0; shockland pay-rate below 12/20; **then freeze and exit.** If the Human
class is still alive after a restored explicit falsity-fact, the guide layer has failed twice on
it and it escalates to the render -- that is N-139v.

**Construction hand-off, restated and now stronger:** six mutate windows in six games, five of
them a single-copy Apex with no second host in play. A third and fourth Dryad, or trimming the
five-colour Apex count in favour of Gemrazer/Greathorn, would do more for this seat than any
remaining sentence.

---

## Ledger

- **N-139n** -- **CLOSED. VALIDATED.** 0 mulligans / 0 false / 0 bottom asks; the count header and
  its satisfiability line are arithmetically correct 6/6.
- **N-139o** (poison unrendered) -- **CLOSED.** 4/4 windows, both partitions, narration, zero
  `NOT lethal` on infect. Guide stopgap RETIRED per its own removal condition, replaced by a
  read-the-number rule.
- **N-139s** (all-illegal blocker set exits silently) -- **no recurrence**; `all_assignments_illegal`
  0 at this seat; the i8 legs parse.
- **N-139v (NEW, HIGH)** -- creature TYPE is a declared primitive field (`subtype=`) the render
  never emits; the pilot filled the gap by invention 5 times and lost the deck's #1 rule.
  Guide-layer repair shipped this wave; render escalation specified in notes.md.
- **N-139w (NEW, LOW)** -- i5's zero-cost emitter prints `{0}` on permanents with NO mana cost
  (transformed DFC back faces, day/night designations), 8 renders. notes.md.
- **N-139x (NEW, MED, instrument)** -- `commit_retracted` is fallback-scoped and cannot see a
  post-answer re-commit the parser ACCEPTS; 6 such at this seat, all executed as the later answer.
  notes.md + general-suggestions.md.
- **N-139y (NEW, MED, guide-lane, now fixed)** -- one blocker assigned to four attackers against
  the prompt's own prohibition; first-wins parser correct, 7 poison connected, game lost. notes.md.
- **N-139m** -- harness lane, KNOWN-OPEN, cited. 1 live firing, 1 arrival.
- **N-139l** -- merged-pile cost pip drift, RECURS (4th corpus), decision-neutral, LOW.
- **N-139q / N-139r / N-139p / N-139t** -- unexercised this corpus; carried unchanged.
- **Shockland tax** -- 20/20 paid, 40 life, guide-lane, fixed this wave; measurement recorded so
  wave-34 has a baseline.
