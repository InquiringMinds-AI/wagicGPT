# deck139 (Temur/Sultai Mutants -- mutate) -- wave-32 review

Corpus `matchups-20260727-115847`, binary `7fabd9bd0`, -T 3000, REPPENALTY=1.05.
Seat record **2/6 -- and BOTH are clean KILLS** (wave-31: 1/6, and that 1 was an adjudication).
W vs146 (opp 0 life, T13), W vs158 (opp -7, T12); L vs36 (dead -6, T7), vs116 (dead -10, T13),
vs105 (**alive at 14 life -- lost to POISON**, T13), vs152 (dead -3, T11).
Pool adjudication rate this corpus: **1 / 21** (wave-31: 3/21; wave-30: 12/21).
Seat decisions **171 timed / 183 records**; fallbacks **1** (vs152 s21 unparsed_reply).
Crash signatures in the six deck139-seat stderr files: **ZERO**. `fails validation; deferring`: **ZERO**.
All card facts below verified against `Res/sets/primitives/borderline.txt` via `/usr/bin/grep`;
all render strings quoted from the translog `prompt` field.

---

## HEADLINE

1. **i7 (London bottoming) VALIDATED, at this seat and corpus-wide -- the class is dead.**
   15/15 mulligan-keeps in the corpus produced EXACTLY ONE bottom ask each, with the required
   count equal to the mulligan count, the named count equal to the required count, and the
   resulting hand size exactly 7-N. Three of the 15 are this seat's.
2. **N-139m DID NOT REPRODUCE LIVE. The trigger predicate fired SIX times and resolved 6/6.**
   Six mutate-card spells were cast with Pollywog Symbiote on this seat's battlefield, on the
   corpus binary `7fabd9bd0`. Every one paid a DISCOUNTED cost, fired Pollywog's loot, and
   ARRIVED on the battlefield as a `[mutated pile]`. The probe that proved N-139m ran on
   `6ca1f8411`, which PREDATES the convoke stale-payment fix in `7fabd9bd0`. **Deliverable: re-run
   the three failing fixtures on 7fabd9bd0 BEFORE scheduling the wave-33 fix** (section 3).
3. **The mulligan spiral is HALVED but NOT CURED, and the misread MUTATED rather than died.**
   2.50 -> **1.50 mulligans/game**. But 6 of the 9 mulligans were false by the guide's own keep
   criterion, and the driver is no longer "zero lands (despite duals)" -- it is **"zero GREEN
   mana sources"** asserted while a green dual, or a literal Forest, sits in the hand. The guide
   now enumerates the green list by name and the pilot RECITES THAT LIST while holding a member
   of it. The guide-wording lever is at its ceiling; the durable fix is representation (N-139n).
4. **i8 and i9 VALIDATED with zero misreads.** `[summoning sick - cannot attack this turn, but
   CAN block]` rendered 131x at this seat / 851x corpus-wide, old form 0x, paradox pair
   `[tapped - cannot attack or block this turn] [attacking]` **0x corpus-wide**. `[deals 0 - this
   attack deals no damage to the opponent]` rendered on every 0-power attacker: **26/26 attacker
   windows declared ZERO 0-power creatures**, against wave-31's four consecutive Grazer attacks
   with a fabricated "3 damage" claim.
5. **New #1 STRATEGY leak, and it decided the vs152 game: the DEPLOY-CHOICE when behind.**
   At 8 life, 0 creatures, facing 5, with 4 mana and both `Cast Pollywog Symbiote (1/3)` and
   `Cast Gemrazer (4/4)` offered, the pilot cast the 1/3 ENGINE. It died the next turn.
6. **Latency: the 195-224 s tail COLLAPSED, 6 -> 1.** Median 16,867 -> **15,601 ms**, mean
   32,254 -> **26,579**, p90 77,085 -> **58,896**, >195 s count **6 -> 1**. The survivor is the
   5-mulligan bottom ask.

---

## (1) i7 -- LONDON BOTTOMING VALIDATION (the assigned gate)

### (1a) This seat's three bottom records, verbatim

| game | seq | prompt says | named | resulting hand | latency |
|---|---|---|---|---|---|
| vs36 | 7 | *"you took **5** mulligans and kept, so you must put **5** cards from your hand on the BOTTOM ... Name EXACTLY 5 cards now - this is the ONLY ask for them ... You will be left with a **2-card hand**"* | `Pollywog Symbiote, Breeding Pool, Island, Plains, Forest` = **5** | seq8 `Your hand: Temple Garden ...; Stomping Ground ...` = **2** | 213,393 ms |
| vs105 | 4 | *"you took **2** mulligans ... put **2** cards ... Name EXACTLY 2 ... left with a **5-card hand**"* | `Snapdax, Apex of the Hunt, Migratory Greathorn` = **2** | 5 (pre-draw) | 146,214 ms |
| vs152 | 4 | *"you took **2** mulligans ... put **2** cards ... left with a **5-card hand**"* | `Dryad of the Ilysian Grove, Island` = **2** | 5 (pre-draw) | 13,614 ms |

Every property the i7 prediction names holds:
- **Exactly N bottoms for N mulligans.** One `kind=bottom` record per game, never re-asked.
  Wave-31's N-139i signature (engine consumes ONE card then RE-ASKS on a shrunken hand) has no
  instance anywhere in the corpus.
- **Constant labels.** The single ask enumerates the full 7-card hand as `1.`-`7.` and is
  answered once; there is no second ask whose numbering could shift. The prompt states the
  contract explicitly: *"this is the ONLY ask for them, and they will be bottomed one at a time
  in the order you give."*
- **Correct final hand size,** stated in the prompt AND realised on the board. vs36 is the
  decisive case: 7 - 5 = 2, and seq8's `Your hand:` line is exactly two cards.
- **Correct execution against the PUT line, not the prose.** vs36's reply prose named a
  DIFFERENT five cards than its `PUT:` line ("I will bottom Pollywog Symbiote, Temple Garden,
  Breeding Pool, Stomping Ground, and Island"); the engine bottomed the PUT line's five, leaving
  Temple Garden + Stomping Ground. The engine obeyed the answer, not the rambling. (The
  prose/PUT divergence is a model self-contradiction inside one 12,845-char reply, not an engine
  defect -- see 8.)

### (1b) Corpus-wide, all 15 bottoms

Scripted over all 42 seat logs: 15 games had >=1 mulligan; **15 bottom records, one per game**;
`says_mulligans == must_put == mulligan_count` in **15/15**; `named == must_put` in **15/15**
(after resolving comma-bearing card names such as `Snapdax, Apex of the Hunt`); resulting hand
size consistent with 7-N in 15/15. No game with mulligans lacked a bottom ask, and no game
without mulligans produced one. **N-139i is CLOSED.**

One cosmetic residual: at N=7 (0-card keep) the sentence *"You will be left with a N-card hand"*
is absent (deck146 vs158 s8, deck146 vs139 s8, deck116 vs158 s8). The count instruction itself is
correct in all three. -> notes.md N-139p (LOW).

---

## (2) MULLIGAN -- rate delta, and the misread that recurred

### (2a) The rate

| | wave-31 | wave-32 |
|---|---|---|
| mulligans / 6 games | **15 (2.50/game)** | **9 (1.50/game)** |
| mean kept hand | 5.0 | **5.8** |
| games at 0 mulligans | 1 | **3** (vs116, vs146, vs158) |
| worst game | 7 mulligans, 3-card keep | 5 mulligans, **2-card keep** (vs36) |

Field this corpus (mulligans/6 games): deck146 **19**, deck116 **9**, **deck139 9**, deck105 2,
deck158 2, deck152 1, deck36 2. The seat is no longer the pool's outlier -- it is mid-field, and
deck146 now owns the spiral. Excluding vs36 the seat's rate is **0.80/game**.

Per-game: vs116 **0** (kept 7), vs146 **0** (kept 7), vs158 **0** (kept 7), vs105 **2** (kept 5),
vs152 **2** (kept 5), vs36 **5** (kept 2).

### (2b) The floor was written, and it was DISOBEYED once -- by DEFERRAL, not by rejection

Guide text: *"HARD FLOOR: you may take at most TWO mulligans in a game. On your third look you
KEEP whatever you have."* vs36 took FIVE. The reply stream shows the floor being re-promised at
every step instead of executed:

- **vs36 seq3** (prompt: *"having already taken 2 mulligans you will bottom 2 cards on a keep"*):
  *"Since I have already taken 2 mulligans, I must take this one to find a functional opening
  sequence, **and I will keep whatever I draw next regardless of quality**."*
- **vs36 seq4**: mulligan again.
- **vs36 seq5** (*"having already taken 4 mulligans"*): *"**With four mulligans already taken, I
  must keep the next hand**, but I cannot keep this one."*
- **vs36 seq6**: keeps at 2 cards.

This is the Step-2 stale-plan / deferred-plan loop applied to a one-shot seam: the floor is a
COUNT the pilot restates as "the next one", and a one-shot decision cannot self-correct. The
wording defect is that the floor names an internal count ("at most TWO mulligans") that the pilot
must maintain, while the PROMPT prints the authoritative count in front of it
(`having already taken N mulligans`). Re-aimed in strategy.txt onto that printed string.

### (2c) The misread RECURRED, in a MUTATED form -- 6 false mulligans of 9

Wave-31's class was "zero lands ... despite having three dual lands". Wave-32's is **"zero GREEN
mana sources"**. Every instance below satisfies the deployed guide's keep criterion verbatim
(>=2 lands, >=1 from the named green list, >=1 card from the named early-play list):

| # | game/seq | hand (verbatim, abridged) | reply, verbatim | verdict |
|---|---|---|---|---|
| 1 | vs36 s1 | `Mountain; Stomping Ground (land: taps for {R}{G}); Island; Arboreal Grazer; Dryad; Dryad; Arboreal Grazer` | *"critically **zero sources of Green mana** ... Stomping Ground ... require a payment of life to produce Green ... the hand lacks a single card that can *produce* Green mana (like a Forest, Breeding Pool, or Overgrown Tomb)"* | FALSE mull |
| 2 | vs36 s2 | `Swamp; Migratory Greathorn; Overgrown Tomb (land: taps for {B}{G}); Nethroi; Dryad; Snapdax; Beanstalk` | *"**zero untapped green mana sources** ... there is no Forest, Stomping Ground, Breeding Pool, or any other land that taps for Green in the hand"* | FALSE mull |
| 3 | vs36 s3 | `Migratory Greathorn x3; Temple Garden; Forest; Overgrown Tomb; Beanstalk` | *"no playable spells for Turn 1 ... Since I have already taken 2 mulligans, I must take this one"* | FALSE mull (Greathorn AND Beanstalk are both on the guide's list-2) |
| 4 | vs36 s4 | `Illuna; Stomping Ground; Beanstalk x2; Gemrazer; Nethroi; Swamp` | *"**zero lands that tap for green mana** (Swamp is black only, **Stomping Ground is red/green** but you have no way to pay the life ...)"* -- self-contradictory in one sentence | FALSE mull |
| 5 | vs105 s1 | `Island; Sacred Foundry; Forest; Pollywog; Pollywog; Arboreal Grazer; Dryad` | *"3 lands but **zero** green mana sources (Forest, Stomping Ground, Temple Garden, Overgrown Tomb, Breeding Pool)"* -- **recites the guide's own list while holding item #1 of it** | FALSE mull |
| 6 | vs152 s1 | `Mountain; Plains; Arboreal Grazer; Overgrown Tomb; Dryad; Arboreal Grazer; Pollywog` | *"critically **zero green mana sources** ... Overgrown Tomb enters tapped unless I pay 2 life, and even if I pay, it provides {B} or {G}, but I have no way to pay the 2 life *and* have mana left"* | FALSE mull |

Correct mulligans (3/9): vs36 s5 (one land -- though its stated reason, *"this hand has zero
lands"*, is FALSE with a literal Forest in it), vs105 s2 (3 lands, none green), vs152 s2 (3
lands, none green).

**Mechanism, and it is NOT the exemplar this time.** The guide's revision already did what
wave-31 HL10 prescribed: it enumerated the offenders by printed name and quoted the render
substring (*"A card printed `(land: taps for {R}{G})` IS a land AND IS a green source; read the
braces, not the name"*). The pilot reads that sentence, reproduces the list, and reasons past it
via a NEW invented gate: **the shockland's "pay 2 life or enter tapped" clause is treated as a
barrier to PRODUCING mana** (instances 1, 4, 6 state it explicitly). It is a false rules belief
about a card class, not a failure to see the land. Per the perception-vs-strategy routing and the
belief-overrides-display rung (wave-23 (b)), a THIRD copy of the fact will not close it:
- the guide states it (present, losing),
- the render states it (`(land: taps for {R}{G})`, present, losing),
- so the remaining layer is **a COUNT the pilot cannot compute around** -- the exact shape that
  worked for i12's `Mana available: N total (...)`. Proposal N-139n in notes.md: a count header on
  the mulligan/bottom hand block, `Your hand (7 cards): lands 3 (green sources: 1) | creatures 4`.
The guide keeps one clause aimed at the SPECIFIC invented phrase ("the 2-life payment is an ETB
choice, not a cost of making mana") per the RESURRECTION-IN-A-NEW-GUISE rung.

### (2d) i11 (creature markers) -- VALIDATED, zero counterexamples

Every hand line carries its type marker: `Arboreal Grazer {g} (0/3) [creature]`,
`Nissa, Who Shakes the World {3}{g}{g} [planeswalker]`, lands as `(land: taps for {X})`.
**Zero "zero creatures" mulligans at this seat.** Not one of the nine mulligan replies miscounts
the creature axis; all six false mulligans are on the LAND/COLOUR axis. i11 closed the axis it
targeted and, by contrast, isolates the axis that is still open: the type marker on a land tells
the pilot it IS a land; nothing tells it HOW MANY, or how many make green.

---

## (3) N-139m -- OCCURRENCE COUNT (engine lane; notes.md only, no guide teach)

**Trigger predicate, written before reading** (wave-31 HL4.2): *a creature spell WITH MUTATE is
cast (either route) while `Pollywog Symbiote` appears in this seat's `Your battlefield:` line.*

**Corpus count at this seat: 6. Vanishes: 0. Arrivals: 6.**

| # | game | seq/turn | spell + rendered cost | printed mutate cost | discount | loot fired | ARRIVED |
|---|---|---|---|---|---|---|---|
| 1 | vs146 | s14 / T7 | `Cast Gemrazer with its mutate cost {g}{g}` | `other={1}{G}{G}` | -1 (1 Pollywog) | s15 `DISCARD ONE OF YOUR OWN CARDS` -> Pollywog from hand | **YES** s18 `Gemrazer {2}{g} (4/4) [trample, reach, mutate] [mutated pile - ... 1 card underneath]` |
| 2 | vs146 | s27 / T13 | `Cast Snapdax, Apex of the Hunt with its mutate cost {w}{w}{b/r} ... total 3 mana` | `other={2}{BR}{W}{W}` | **-2** (2 Pollywogs) | s28 + s29, two loots -> Plains, Plains | **YES** s32 `Snapdax ... (3/5) [trample, double strike, reach, mutate] [mutated pile ...]` |
| 3 | vs105 | s21 / T10 | `Cast Gemrazer with its mutate cost {g}{g}` | `{1}{G}{G}` | -1 | s23 + s24, two loots | **YES** s27 `Gemrazer {1}{u} (4/4) ... [mutated pile ...]` |
| 4 | vs105 | s27 / T10 | `Cast Gemrazer with its mutate cost {g}{g}` (2nd copy) | `{1}{G}{G}` | -1 | s29 + s30, two loots | **YES** s31/s32 host list reads `Gemrazer #1 ... [your battlefield]`, pile grew |
| 5 | vs158 | s15 / T6 | `Cast Gemrazer with its mutate cost {g}{g}` | `{1}{G}{G}` | -1 | s16 loot -> Beanstalk Giant | **YES** s19 `Gemrazer {1}{u} (4/4) ... [mutated pile ...]` |
| 6 | vs158 | s26 / T12 | `Cast Illuna, Apex of Wishes with its mutate cost {2}{u}{u}{r/g} ... total 5 mana` | `other={3}{RG}{U}{U}` | -1, **off a Pollywog that is UNDER the pile** | s27 loot -> Stomping Ground | **YES** s30 `Illuna, Apex of Wishes {1}{u} (6/6) [trample, flying, reach, mutate] [mutated pile ...]` |

Also present: the seat's own hardcast lines show the discount live (`Cast Gemrazer {1}{g}` at
vs105 s21 = `mana={3}{G}` minus 2). So the `altercost(colorless,-1)` path was exercised on BOTH
routes' PRICING; only the mutate route's CASTING was actually taken (the normal-cast route of a
mutate card with Pollywog out -- the probe's `_control2` failing case -- was offered but never
chosen, so **that half remains UNEXERCISED live**).

**RESOLVED-EFFECT check, one rung deeper than arrival** (Step-2 resolved-effect-magnitude):
- Row 2, Snapdax: narration `Snapdax: hand -> stack` -> `stack -> battlefield` -> `mutate over` ->
  target Gemrazer -> pile renders, and **`Your life: 12` (s31) -> `Your life: 16` (s32)**. The
  `auto=@mutated(this):name(Gain 4 life) life:4 controller` trigger RESOLVED at the magnitude the
  guide states. The paired `damage:4 target(*[creature;planeswalker]|opponentBattlefield)` half
  had no legal target (`Opponent battlefield (creatures: 0)`).
- Row 6, Illuna: `auto=@mutated(this):... reveal ... optionone ... transforms((...Put in play...))`
  fired -- s30 is a `kind=reveal` record and s31 answers `put in play`. Trigger RESOLVED.
- Rows 1/3/4/5, Gemrazer: `auto=@mutated(this):name(Destroy artifact or enchantment)
  target(*[artifact;enchantment]|opponentBattlefield) destroy` -- no legal target on any of the
  four boards. Correctly silent, not a failure.

**What this means, stated conservatively.** The probe's fixtures are on `6ca1f8411`; the corpus is
on `7fabd9bd0`, which lands the convoke stale-payment fix (*"doPay/isPaymentSet keying drift left
stale payment targets"*). N-139m's described mechanism -- a spell that pays, fires its cast
trigger, then leaves the stack into no zone -- is in the same family as a stale payment record. It
is plausible, and NOT established here, that `7fabd9bd0` closed it. Two other differences could
also explain the divergence: the `[AI]` fixture harness drives the cast through the scripted
menu-pump, whereas live casts ride the `DecisionManager` `planCastSpell` one-click-per-tick path;
and the fixtures' Pollywog board is minimal.

**Deliverable (replaces "schedule the fix"): re-run `pollywog_discount_gemrazer_n139h.txt`,
`pollywog_discount_dirgebat_n139h.txt` and `_control2_gemrazer_normal_discounted.txt` on
`7fabd9bd0` (or current HEAD) FIRST.** If they go green, N-139m is a fixture-vs-binary staleness
artifact and the ledger item downgrades to "register the three fixtures as regression tests". If
they stay red on a binary the live corpus passes 6/6 on, the divergence is itself the finding and
localises N-139m to the fixture harness path rather than the production cast path -- a different,
cheaper fix. Either way the wave-33 docket item changes shape. Details in notes.md.

**N-139g (under-card statics) -- ANSWERED POSITIVE, now LIVE-CONFIRMED twice.** The deployed
guide's claim *"its discount keeps working even while Pollywog is the UNDER card of a merged
pile"* was a PROMISE THE ENGINE OWES (wave-31 HL10) with 0 live witnesses. It is now PAID:
vs158 s18 puts Pollywog UNDER Gemrazer, and at s26 Illuna's mutate still renders at
`{2}{u}{u}{r/g}` = `{3}{RG}{U}{U}` minus 1; vs105 s26 puts Pollywog #1 under Gemrazer and the
s27 hardcast still renders `Cast Gemrazer {1}{g}` (= -2 off `{3}{G}`, both Pollywogs applying,
one of them buried). Promise discharged; the line stays, now as a validated teach.

---

## (4) i8 / i9 -- TAG VALIDATION (the assigned gate)

### (4a) i8 -- summoning-sick CAN-block clause. The wave-31 game-loss class is DEAD.

Render, verbatim and invariant: `[summoning sick - cannot attack this turn, but CAN block]`.
- **131 occurrences at this seat, 851 corpus-wide. Old form `[summoning sick - cannot attack this
  turn]` without the CAN-block clause: 0 corpus-wide.**
- **Paradox pair `[tapped - cannot attack or block this turn] [attacking]`: 0 corpus-wide**
  (`[tapped - attacking]` renders 157x instead). Zero paradox verbalizations in any reply.
- **Zero misreads at this seat.** The wave-31 loss shape (vs158 s26: *"The opponent has no
  untapped creatures to block (their Orc army and Sauron are tapped)"* while Sauron was
  summoning-sick-but-untapped, costing the game) has no analogue. The one `no untapped creatures
  to block` phrase this corpus (deck116 s17) is about a genuinely empty opposing board and is
  correct.
- Positive evidence the clause is being USED: the pilot reasons about summoning-sick bodies as
  attack-restricted only, never block-restricted, e.g. vs146 s19 *"Casting another creature now
  would not help me attack this turn due to summoning sickness"*.

### (4b) i9 -- zero-power attacker tags. 26/26 windows clean.

Render: `A1. Arboreal Grazer (0/3) [reach] [deals 0 - this attack deals no damage to the opponent]`.
Every attacker window at this seat, all six games:

- **26 attacker windows. 0-power creatures declared as attackers: 0.**
- **Nonzero-attacker participation: 26/26 FULL** (declared count == count of A-lines without the
  `[deals 0` tag, in every window; counted against the engine's `A#.` list, never the battlefield).
  Six windows offered ONLY 0-power attackers and were correctly answered `no attackers` per the
  guide's closed exception (vs116 s9, vs146 s10, vs158 s11, vs152 s11/s12/s14/s16).
- **The tag is DYNAMIC and the pilot reads it dynamically.** vs105 s19/s33/s37 tag
  `Dryad of the Ilysian Grove (0/2) [deals 0 ...]` -- Dryad is printed 2/4 and was reduced by two
  -1/-1 counters from infect damage. The pilot excluded her by name each time
  (*"Dryad is a 0/2 and deals no damage, so attacking with it is pointless"*, s19). That is a
  stronger validation than the static Grazer case: the annotation carried a COMPUTED value the
  pilot could not have taken off the printed card.
- The wave-31 failure it replaces: four consecutive Grazer attacks (vs137 s16/s18/s19/s21) with
  *"Attacking with all three 0/3 Grazers deals 3 damage"*. **Zero recurrences.**
- The blockers-side twin renders too: `[deals 0 - this block kills nothing, it only absorbs
  damage]`, reasoned with correctly (vs158 s22 chump-with-menace-math; vs152 s15 decline at safe
  life; vs105 s20/s34 decline a 0/2 into a 2/1).

**Both tags are now DOUBLE COVERAGE against the guide's own clauses** (the guide carries "ARBOREAL
GRAZER IS 0/3 ... deals ZERO damage" and "A creature marked [summoning sick ...] CAN STILL
BLOCK"). Per the three-part demotion rule both are demoted in strategy.txt to marker-anchored
one-liners, keeping the load-bearing token and the render string, cutting the war-story.

---

## (5) MUTATE STEADY STATE (arrival-traced) + the guide's other teaches

### (5a) Offer/take/arrival

**7 mutate-line offer windows, 6 taken, 6/6 arrived.** The single decline (vs158 s12 T4, Gemrazer
mutate offered, `Cast Dryad of the Ilysian Grove` taken instead) is a correct-let under the
guide's own ramp ordering -- the pilot's reasoning is *"Dryad ... enables an extra land drop this
turn ... and fixes my mana"*, and Gemrazer was mutated three seqs later at s15. Scored by
DECISION-CORRECTNESS (value-floor rung): **7/7 correct**, 0 errors.

### (5b) The wave-31 (2a) mutate/hardcast MISLABEL class is DEAD

Wave-31: 2 of 2 big-Apex casts took a hardcast line while the reply asserted a mutate.
Wave-32: **6 of 6 Apex/mutant casts took the `with its mutate cost` line**, and 5 of 6 replies
name the mutate explicitly and correctly (the sixth names no mutate and is a mutate). Zero
mislabels. The revised guide's verification step (*"re-read your chosen option and confirm the
words 'mutate cost' are in it"*) fired. FREEZE it verbatim.

### (5c) The full multi-ask flow parses and resolves, every time

`Cast X with its mutate cost` -> `Choose an option for X:` (`Cast Card Normally` / `mutate [cast
for the MUTATE cost: merge onto one of your non-Human creatures ...]` / decline) -> `DISCARD ONE
OF YOUR OWN CARDS` (0-2 loots) -> `mutate over` -> `TARGET CHOICE for X - its "mutate over"
ability ...` -> `[mutated pile - ONE merged creature ...; N card(s) underneath: ...]`.
Up to SIX consecutive asks per cast (vs105 s27-s32), 6/6 completed, 0 fallbacks, 0 defers.

### (5d) h-batch / render reconciliation sweep (HL10) -- no stale guide prose

Every render string the deployed guide describes was grepped against this corpus and MATCHES:
the mutate cast line, the `Choose an option for <card>:` header, the self-describing `mutate
[cast for the MUTATE cost: ...]` label, `DISCARD ONE OF YOUR OWN CARDS`, `mutate over`/`mutate
under`, `TARGET CHOICE for <card>`, and the one-line `[mutated pile ...]`. **No sentence of the
deployed guide is made false by this wave's batch.** Two NEW surfaces the guide should now point
at rather than paraphrase: the hybrid explainer (`[hybrid: each {b/r} pays with B or R - total 3
mana]`) and i13's `{card text: "..."}` on Cast lines (54 of 91 Cast option lines at this seat).

### (5e) i13 cost risk -- NOT realised at this seat; the opposite

Reply length p50 **925** chars / p95 **6,393** / max **12,845**, against wave-31's p50 ~1,100 and
a 12.2-13.3 k cluster at the tail. Latency fell on every statistic. Card text on Cast lines did
not degrade this seat; it is plausibly part of why (5b) went 6/6.

### (5f) i12 -- ONE counterexample, quoted

i12's prediction was zero "I cannot afford" declines of offered payable casts. There is one, at
this seat, in the WON game (outcome-neutral). vs146 s21, Main phase 2, render:
`Mana available: 3 total (three untapped sources, tapped automatically when you cast; colours you
can make: {g}{u}{r}{b}{w})`, options `1. Cast Pollywog Symbiote {1}{u}` / `2. Cast Dryad of the
Ilysian Grove {2}{g}` / `3. Cast nothing right now`. Chose 3, reasoning *"I have no untapped lands
left to pay for any of the cards in my hand ... my only available mana is generic or colored but
insufficient"*. Count-first line PRESENT and LOST. One instance -> reported, not a re-open.

### (5g) Cast-nothing rate, split by window composition (HL9)

**2 declines / 33 casting windows (6.1%).** Both in the won vs146 game, both with >=1 creature
offered (i.e. both are declines against the floor, not legitimate instant-holds -- this deck runs
no instants). s19 is a defensible "attack for the win instead" (it did, and won); s21 is the
false-affordability decline in 5f. Neither cost a game. The cast floor is holding.

### (5h) CASTABLE-BUT-NOT-OFFERED audit in every loss (HL9)

Signature cards checked in each of the four losses, by cross-reading `Your hand:` against the
numbered option list at every casting ask: **zero silently-absent payable casts**. vs36 (2-card
keep: Temple Garden + Stomping Ground, nothing castable ever), vs116 (no mutant in hand at any
point -- offers were Grazer 6 / Dryad 3 / Beanstalk-adventure 1 / Cast-nothing 7), vs105
(Gemrazer and Everquill both offered when payable; Everquill was discarded to a loot, not
withheld), vs152 (Gemrazer and Pollywog both offered at s20; Illuna correctly absent at 4 mana
against `{2}{G}{U}{R}`). i1's prediction holds at this seat.

---

## (6) THE RECORD, DECOMPOSED (HL7, both directions)

### (6a) Kills are the pool-invariant number, and they went 0 -> 2

| | wave-31 | wave-32 |
|---|---|---|
| record | 1/6 | **2/6** |
| clean **kills** | **0** (the 1 win was adj1, 8 v 4 at T14) | **2** (opp 0 at T13; opp -7 at T12) |
| adjudicated-ahead wins | 1 | 0 |
| pool adjudication rate | 3/21 | **1/21** |

The record moved by one; the KILL count moved by two, in a pool whose adjudication rate fell to
near zero. Both wins are the guide's own line executed end to end: vs146 = Gemrazer mutate (T7)
-> Snapdax mutate + 4 life (T13) -> lethal alpha; vs158 = Gemrazer mutate onto Pollywog (T6) ->
Illuna mutate onto the pile + its reveal trigger (T12) -> lethal with a 6/6 flier.

### (6b) The four losses

| game | cause | owner |
|---|---|---|
| **vs36** (dead T7, kept 2 cards) | The mulligan spiral, in pure form. Five mulligans, floor deferred four times, 2-card keep, dead on turn 7 having cast nothing. | **GUIDE (mulligan) + REPRESENTATION (N-139n)** |
| **vs105** (alive at **14 life**, T13) | **Lost to POISON at 14 life.** The pilot knew the rule (s19: *"I'm at 14 life, and 10 poison counters lose the game"*) but its poison total is **NEVER RENDERED** -- `--- CURRENT SITUATION ---` prints `Your life: 14 \| Opponent life: 13` and no poison field anywhere in 39 records. It blocked and raced on a life total that was not the clock that killed it. | **REPRESENTATION (N-139o, HIGH, cross-seat)** |
| **vs152** (dead -3, T11) | **The deploy-choice at 8 life** (6c). | **GUIDE (new #1 seam rule)** |
| **vs116** (dead -10, T13) | Draw variance / construction. Kept 7 (3 lands, 3 Grazers, Dryad); across 13 turns the hand held only Grazer / Dryad / Beanstalk / lands. **No mutant was offered once all game.** Killed by a 12/12 Kozilek with Annihilator 4. | **CONSTRUCTION** |

### (6c) The vs152 loss -- the wave's new #1 STRATEGY leak, in one decision

vs152 seq20, T10. `Your life: 8 | Opponent life: 26`. `Your battlefield (creatures: 0)`.
`Opponent battlefield (creatures: 5)`. `Mana available: 4 total`. Options:
```
1. Cast Pollywog Symbiote {1}{u} (1/3) {card text: "Each creature spell you cast costs {1} less ...
2. Cast Gemrazer {3}{g} (4/4) {card text: "Mutate {1}{G}{G} ...
3. Cast nothing right now
```
It chose **1**, the 1/3: *"I need to stabilize and set up my engine ... Once on the battlefield,
it reduces the cost of my future mutate spells by {1}, making my big Apex bombs much more
affordable."* The 4/4 with reach was exactly payable at 4 mana.

The next decision (s21, blockers, at 8 life against five attackers including a 3/1 FLIER) is the
seat's only fallback: the lone blocker was the 1/3 Pollywog, which has neither flying nor reach,
and the reply's `BLOCKS: Pollywog Symbiote blocks Elite Spellbinder` names an illegal assignment
-> all assignments dropped -> `unparsed_reply` -> heuristic. Dead at -3 the following turn. Had
Gemrazer been cast, the seat had a 4/4 **reach** blocker for that exact flier.

This is wave-31 HL3 exactly: a **VALUE teach** ("Pollywog is your mutate discount engine, keep it
alive, mutate onto it") applied at the level the model reasons at, evaporating at the seam where
the role is cheapest to abandon -- here the DEPLOY seam, not combat. The deployed guide raises
Pollywog's value in three separate places and gives the deploy seam no rule for WHICH body to
cast when behind. Fixed in strategy.txt as a seam-labeled, countable rule.

---

## (7) RETROACTIVE CARD-FACT AUDIT of the ENTIRE deployed guide (wave-31 HL1 HARD GATE)

Ran over every cost, MV, P/T, targeting restriction and zone claim in `deck139_strategy.txt`
against `Res/sets/primitives/borderline.txt`. **Recorded here as the gate's explicit line item.**

**TRUE (no change needed):** all eight mutate costs (`Snapdax {2}{BR}{W}{W}`, `Illuna
{3}{RG}{U}{U}`, `Nethroi {4}{GW}{B}{B}`, `Vadrok {1}{WU}{R}{R}`, `Brokkos {2}{UB}{G}{G}`,
`Gemrazer {1}{G}{G}`, `Greathorn {2}{G}`, `Everquill {3}{R}`); all eight P/T (3/5, 6/6, 5/5, 3/3,
6/6, 4/4, 3/4, 4/4) and their keyword sets; `Arboreal Grazer {G} 0/3 reach` + its ETB land-drop;
`Dryad {2}{G} 2/4` + `maxPlay(land)+1` + `lord(land|myBattlefield) transforms((basic plains forest
mountain swamp island))`; `Pollywog {1}{U} 1/3` + `altercost(colorless,-1)` + `_LOOT_`;
`Beanstalk Giant */* = land count` and `Fertile Footsteps {2}{G}` fetch-a-basic; Nissa's
double-green Forests and her `+1` making a 3/3 vigilance/haste land. **PERSISTENCE (HL2): every
creature the guide names as a body actually STAYS** -- no self-bouncer, self-saccer or
conditional-return anywhere in this deck. Every creature is non-Human (Beast / Frog / Nymph /
Giant / Phoenix / Nightmare / Dinosaur / Cat / Elemental), so the guide's "any of your creatures
can be the host" is TRUE.

**FALSE / UNSUPPORTED -- three hits, all corrected in strategy.txt:**

1. **NETHROI (FALSE NUMBER, under-sells the engine).** Guide: *"return creatures with **total
   power 10 or less** from your graveyard"*. Primitive:
   `auto=@mutated(this):name(Return creatures) target(<anyamount>creature[power<=10]|mygraveyard)
   moveTo(myBattlefield)` -- **ANY NUMBER of creatures, EACH with power <= 10**. The engine is far
   more permissive than the guide claims. (The guide matched real Oracle; the ENGINE deviates.
   Per wave-23 (c) the guide is softened to a generic oracle-true description and the deviation is
   routed to notes.md as a card-script item -- N-139q.)
2. **VADROK (UNSUPPORTED "for free", and near-dead in this deck).** Guide: *"recast a noncreature
   card of mana value 3 or less from your graveyard **for free**"*. Primitive:
   `auto=@mutated(this):may name(Cast noncreature card)
   target(*[-creature&manacost<=3]|mygraveyard) activate castcard(normal)` -- `castcard(normal)` is
   not a without-paying-its-cost token. Softened. The load-bearing fact is checkable off the
   decklist alone and is stronger: **deck139's only nonland noncreature card is Nissa (MV 5)**, so
   Vadrok's trigger can never find a target in this deck's own graveyard. Stated in the guide.
3. **"The mutate TRIGGER fires either way (over or under)" (NOT UNIFORMLY SUPPORTED).**
   Gemrazer / Greathorn / Nethroi / Vadrok / Everquill each carry BOTH `@mutated(this)` and
   `@mutated(mytgt)` hooks. **Snapdax and Illuna carry only `@mutated(this)`.** I did not read the
   engine to establish what `mytgt` resolves to, so no mechanism is asserted -- the guide drops the
   "either way" claim and states the already-mandated OVER as an absolute, which is correct under
   either reading. Asymmetry routed to notes.md (N-139r, LOW, data question).

**One GAP (not false, but the card is undescribed):** Everquill Phoenix's mutate trigger is
`token(Feather Eve)`, and `Feather Eve` in this engine is `type=Artifact`, `color=red`, with
`auto={1}{S}:name(Return a phoenix) target(*[phoenix]|mygraveyard) moveto(myBattlefield) and!(
tap(noevent) )!` -- **a sacrificeable artifact, not a 3/3 flier**. The guide never claimed
otherwise (no false teach) but listed Everquill among cards that "do something powerful". At a
{3}{R} mutate cost off 8 red sources for a recursion trinket, it is demoted to an accurate line.

**A NEW small false belief (harmless, one clause added):** the pilot twice asserts Pollywog
triggers its own loot -- vs146 s12 *"triggers its 'draw a card' ability immediately since it is a
creature spell with mutate"*, vs158 s9 *"casting it triggers its own ability (since it has
mutate)"*. Pollywog's `abilities=` field is empty; it has no mutate. Both casts were correct
anyway, but the belief mis-models when the loot fires.

---

## (8) LATENCY -- the tail vs the 195-224 s history (the assigned gate)

171 timed decisions.

| | w30 | w31 | **w32** |
|---|---|---|---|
| median ms | 17,421 | 16,867 | **15,601** |
| mean ms | 35,240 | 32,254 | **26,579** |
| p90 ms | -- | 77,085 | **58,896** |
| max ms | 209,562 | 224,282 | **213,393** |
| decisions > 195 s | 4 | 6 | **1** |
| decisions > 100 s | -- | -- | 9 |
| reply chars p50 / p95 / max | -- | ~1,100 / -- / 15,217 | **925 / 6,393 / 12,845** |

**The tail collapsed.** Wave-31's finding was that the tail is a reply-LENGTH phenomenon at the
token cap, seam-agnostic, with six 12.2-13.3 k replies; the guide's "DECIDE FAST" section was
retargeted from combat to all seams. It landed: p95 reply length is now under half wave-31's tail
cluster, and only ONE decision exceeded 195 s.

**The survivor is diagnostic** (HL9 rank-tax rule: read the low-branching rows first). The
corpus's single longest reply and this seat's slowest decision are the same record --
**vs36 s7, the BOTTOM ask, 213,393 ms / 12,845 chars, on a 7-option pick-5**. A bottoming choice
cannot be hard; the deliberation is the model rehearsing a belief it cannot settle, and it is the
SAME belief that produced the game's five mulligans (the reply argues about which lands "produce
green" and ends up naming a different five cards in prose than in its `PUT:` line). The seat's
largest time cost and its largest strategic leak are one belief. Second and third longest
(vs105 s28 158.6 s; vs105 s4 146.2 s -- also a bottom ask) point the same way.

The "DECIDE FAST" teach is therefore **PROVEN** and is demoted to a short line, with its one
remaining aim moved onto the mulligan/bottom seam.

---

## (9) PARSER / FALLBACKS

**1 fallback / 171 timed decisions (0.58%)**, up from wave-31's 0/154 but on a different shape.
- vs152 s21, `kind=blockers`, `unparsed_reply`, 8,171-char reply. The head line was compliant in
  FORM (`BLOCKS: ...`) but named an assignment the engine never offered: Pollywog Symbiote
  (1/3, no flying, no reach) onto `A5. Elite Spellbinder (3/1) deals 3 [flying]`, while B1's own
  line reads `may block A1 ..., A2 ..., A3 ..., A4 ...` -- A5 absent. All assignments illegal ->
  all dropped -> nothing left to parse. **The parser behaved correctly.** The observation is about
  the EXIT: an all-illegal assignment set exits as a silent `unparsed_reply` + heuristic, where an
  i5-style loud re-ask (illegal target removed / legality restated) is the shape the wave-32 batch
  just installed elsewhere. -> notes.md N-139s (MED).
- `validation_reject_reask` / `validation_reject_exhausted` / `truncated_abandoned` /
  `deferred_to_heuristic` / `empty_reply`: **0** at this seat; the first two are 0 corpus-wide, so
  i5 remains live-validated only by the deck198 probe.
- Corpus totals confirmed from the raw files: 1 `retracted_choice`, 2 `unparsed_reply`, 0 others.

---

## (10) ROTATION -- exit-gate status, stated honestly

The wave-31 gate, with N-139h re-scoped to N-139m per the probe:
*N-139m fixed + discounted mutate arrival-traced live; revised guide validated; guide frozen;
combat-latency teach proven.*

| gate condition | status | what is missing |
|---|---|---|
| **crash half (original N-139h)** | **MET** | 0 crash signatures in 6/6 seat stderr, third consecutive corpus; probe concurs |
| **N-139m fixed** | **UNRESOLVED -- and the live evidence points the other way** | Fixtures are red on `6ca1f8411`; the corpus is green 6/6 on `7fabd9bd0`. Cheapest next step is a fixture RE-RUN on the corpus binary, not a fix (section 3). Not MET, but no longer a HIGH live-risk item at this seat. |
| **discounted mutate arrival-traced live** | **MET** | 6/6 Pollywog-discounted mutate casts paid, looted and arrived, with two triggers resolved at magnitude (Snapdax +4 life 12->16; Illuna reveal->put in play). The wave-31 UNEXERCISED verdict is discharged. Residual: the NORMAL-cast route of a mutate card with Pollywog out was offered but never taken -- still unexercised live. |
| **revised guide validated** | **PARTIAL (3 of 4 revised teaches)** | mutate-line verification **6/6 PASS** (class dead); attack floor + 0-power exception **26/26 PASS** (class dead); DECIDE-FAST **PASS** (>195 s 6->1); mulligan floor + dual-land counting **FAIL** -- floor deferred to 5 mulligans once, 6 false mulligans of 9. |
| **guide frozen** | **NOT MET -- freeze DECLINED again** | strategy.txt is revised this wave. But the HL1 hard gate is now DISCHARGED for this seat: the retroactive full-guide card-fact audit RAN (section 7) and found three defects, all corrected. The seat is now exit-ELIGIBLE, which it was not before. |
| **combat-latency teach proven** | **MET** | median/mean/p90 all down; >195 s 6 -> 1. Residual is one bottom ask, folded into the mulligan lever. |

**Recommendation: HOLD deck139 for wave-33 -- and this should be the last guide-lane wave.** Two
conditions newly MET, two teaches converted FAIL -> PASS-and-frozen, the HL1 audit discharged, and
the remaining guide work is ONE lever (mulligan) plus ONE new seam rule (deploy-choice when
behind). If wave-33 shows the mulligan rate at or under the field with no false-green-source
mulligan, the guide freezes and the seat exits -- **conditional on N-139n (hand-count header)
shipping, because the guide layer has now failed twice on that belief and section 2c argues it
cannot win at that layer.**

**Construction hand-off, restated (unchanged from wave-31; this corpus corroborates it):** all six
mutates this corpus were again the mono-green mutators or a Dryad-fixed cast (Gemrazer x4, Illuna
once, Snapdax once). Snapdax's and Illuna's mutate lines were offered exactly ONCE EACH in six
games. vs116 offered no mutant at all in 13 turns. The double-pip Apexes remain unsupported by a
24-land five-colour base with 2 Dryads; a third and fourth Dryad, or cutting to the green/red
mutators, would do more than any guide sentence.

---

## Ledger

- **N-139m** -- **0 live occurrences / 6 trigger-predicate firings** on `7fabd9bd0`; fixture
  evidence is from `6ca1f8411`. **RE-RUN THE FIXTURES FIRST.** notes.md.
- **N-139n (NEW, HIGH)** -- mulligan/bottom hand block has no LAND or GREEN-SOURCE count; six
  false "zero green sources" mulligans against a printed `(land: taps for {R}{G})`. Guide layer
  has failed on this belief twice. notes.md.
- **N-139o (NEW, HIGH, cross-seat)** -- **poison counters are never rendered.** Lost the vs105
  game at 14 life to an invisible lose-condition resource. notes.md.
- **N-139s (NEW, MED)** -- an all-illegal blocker assignment set exits as silent
  `unparsed_reply` + heuristic rather than an i5-style loud re-ask. notes.md.
- **N-139q (NEW, MED, card-script)** -- Nethroi's `auto=` returns ANY NUMBER of creatures each
  with power<=10; Oracle is "total power 10 or less". notes.md.
- **N-139p (NEW, LOW)** -- the bottom ask omits the "You will be left with an N-card hand"
  sentence when N would be 0 (7 mulligans). notes.md.
- **N-139r (NEW, LOW)** -- `@mutated(mytgt)` hook present on five mutants, absent on Snapdax and
  Illuna. Data question; no mechanism asserted. notes.md.
- **N-139t (NEW, LOW)** -- the `DISCARD ONE OF YOUR OWN CARDS` header attributes the loot source
  inconsistently (`(Pollywog Symbiote)` at vs146 s15, `(Gemrazer)` at vs105 s29). notes.md.
- **N-139g** -- under-card statics: **ANSWERED POSITIVE, now LIVE-CONFIRMED twice** (vs158 s26,
  vs105 s27). The PROMISE-OWED line is discharged.
- **N-139i** -- London bottoming: **CLOSED**, 15/15 corpus-wide.
- **N-139j** (0-power attacker tag) -- **CLOSED**, 26/26.
- **N-139k** (summoning-sick CAN block) -- **CLOSED**, 131 renders / 0 misreads / 0 paradox pairs.
- **N-139l** (merged-pile cost pip drift, `{3}{g}` -> `{g}` / `{1}{u}`) -- **RECURS**, still
  decision-neutral. LOW, unchanged.
- **Commit-lock** -- no witness this corpus.
- **i12** -- one counterexample at this seat (vs146 s21), outcome-neutral, quoted in 5f.
