# deck146 (WB Orzhov Dungeons) -- wave-31 findings

Corpus: `matchups-20260727-025026`, binary cf7cb2e53, -T 3000, REPPENALTY=1.05. **4th corpus at
this seat.** Guide under test: `bin/Res/ai/baka/deck146_strategy.txt` (wave-29 revision, FROZEN
wave-30 and wave-31).

Record: **2/6** (wave-30 3/6, wave-29 3/6). Own-seat volume 231 records / 220 model calls.
Own-seat fallbacks: **1 unparsed_reply** (vs122 seq18).

Games (results.tsv):

| Opp | Result | Life | Turn | wave-30 |
|---|---|---|---|---|
| 152 | **WIN** clean | 9 / 0 | 16 | L(adj) |
| 122 | **WIN** clean | 16 / -5 | 12 | L(adj) |
| 139 | LOSS adj | 4 / 8 | 14 | W(clean) |
| 148 | LOSS clean | 0 / 12 | 13 | W(clean) |
| 137 | LOSS clean | 0 / 19 | 12 | W(adj) |
| 158 | LOSS clean | -1 / 5 | 19 | (new; replaces 136 L) |

**Every one of the five repeated head-to-heads INVERTED.** That shape is the key context for the
3/6 -> 2/6 read and is decomposed in (d).

---

## (a) h6 -- N-146g PLANESWALKER MAGNITUDE SKIP: **VALIDATED, clean before/after**

Wave-30's finding: dynamic-magnitude (WParsedInt) eval attached a nonsense `{right now: damage 8}`
to Lolth's Cast option and to BOTH of her loyalty options -- the number is her -8 emblem's drain
expression, irrelevant to casting her or to +0/-3.

**BEFORE (wave-30 corpus `matchups-20260726-181642`, deck146 vs122 seq20/seq21 t12), verbatim:**

```
1. Cast Lolth, Spider Queen {3}{b}{b} {right now: damage 8}
1. -3: create spiders with Lolth, Spider Queen [cost: Counters] {card text: "..."} {right now: damage 8}
2. +0: draw card and lose life with Lolth, Spider Queen [cost: Counters] {card text: "..."} {right now: damage 8}
```

**AFTER (this corpus, deck146 vs122 seq30 t10 / seq31 t10 -- same matchup, same card), verbatim:**

```
3. Cast Lolth, Spider Queen {3}{b}{b}
1. -3: create spiders with Lolth, Spider Queen [cost: Counters] {card text: "..."}
2. +0: draw card and lose life with Lolth, Spider Queen [cost: Counters] {card text: "..."}
```

Counts, which show the fix is surgical rather than a blanket disable:

- wave-30 deck146 seat: **14** `{right now: ...}` option annotations, **14 of 14 on Lolth lines**
  (Cast x2, +0 x9, -3 x3) -- 100% of this seat's magnitude annotations were the bogus set.
- wave-31 deck146 seat: **0** `{right now: ...}` annotations on any option line, walker or not.
- wave-31 corpus-wide: **49** `{right now: ...}` annotations still render, all at deck158. The eval
  is alive; it just no longer touches planeswalkers.

Kaya's loyalty menu (vs122 seq35 t12, 9 options incl. six `-3: exile non-land permanent ...
targeting X` lines) also renders completely clean -- her first-ever live render in the campaign
carries no magnitude annotation. **h6 VERDICT: PASS. Retire N-146g.**

---

## (c) KAYA AND LOLTH -- BOTH EXERCISED. THE SEAT'S LAST UNVALIDATED SURFACE IS CLOSED.

Kaya the Inexorable had gone uncast across three corpora (drawn once in wave-30, mulliganed away).
**This corpus she was cast and used, and the line was guide-perfect.**

vs122, t12, one turn after Lolth:
- seq34 t12 -- chose `Cast Kaya the Inexorable {3}{b}{w}` (option 3), declining an offered
  `Cast Vanishing Verse` in the same window. Board: opponent had Suture Priest + three Goblins;
  deck146 ahead 16-3.
- seq35 t12 -- loyalty menu offered 9 options. Chose option **4, `-3: exile non-land permanent with
  Kaya the Inexorable targeting Suture Priest`** -- the opponent's only real permanent, exiled the
  turn she landed, Kaya surviving at 2. Verbatim the guide's teach: *"Land her and immediately -3
  their best threat when you are behind; she drops to 2 and survives."*
- seq37 t12 -- alpha strike, game over at t12.

Primitive check (`Res/sets/primitives/planeswalkers.txt`):
`auto={C(0/0,-3,Loyalty)}:name(-3: Exile non-land permanent) target(*[-land]|battlefield)
moveto(exile)` -- the predicate is `|battlefield` (both sides), which is why options 5/6/8/9
offered Kaya's own Adventurer, her own Lolth, and Kaya herself. That is Oracle-correct ("target
nonland permanent") and the model correctly ignored all of them. No finding.

Lolth cast twice (vs122 seq30 t10 -> seq31 `-3: create spiders`; vs137 seq21 t11 -> seq22 `-3:
create spiders`), both times **the same turn she landed, ability activated immediately**. Zero idle
walker turns across three walker casts.

**Disposition: the Kaya/Lolth residual is DISCHARGED.** It is no longer a reason to hold the seat,
and per the brief its former unexercised state would not have held the seat anyway.

---

## (b) FREEZE VALIDATION -- the exercised teaches HELD, but the guide is NOT clean

Third consecutive corpus of correct firing on everything exercised:

| Teach | Wave-31 evidence | Verdict |
|---|---|---|
| Pick Lost Mine **by name** | **15 / 15** dungeon choices = Lost Mine (vs152 s11; vs139 s10/s23/s37/s46; vs122 s7/s20/s28; vs148 s9/s20/s38; vs158 s8/s27; vs137 s12) | HOLDS |
| Room by `{room effect}`, body/drain over scry | **23 / 23** value rooms: goblin lair x12, dark pool x6, storeroom x5. **Zero** Cave Entrance (scry) or Fungi Cavern picks | HOLDS |
| Completion status line (N-146f, wave-30 g5) | Rendered in 101 prompts across the 5 games where a dungeon completed; correctly ABSENT in vs137 (mull-to-5, no completion). N>0 gating still correct | HOLDS |
| Walkers: cast when affordable, act every turn | 3 casts, 3 same-turn activations, 0 idle turns (see (c)) | HOLDS |
| Soul Shatter not into an empty board | Cast 2x, both with opposing creatures present (vs137 s17 t9; vs152 s34 t14) | HOLDS |
| Dual-land mana oracle (f2) | 0 mono-color collapse lines. (The three `{b}{b}{b}{w}{w}{w}{w}` lines at vs158 s31/33/35 are a *floating mana pool* render, not the f2 bug) | HOLDS, 3rd corpus |

**But the freeze did NOT hold in the sense that matters, and this is the wave's main finding: the
frozen guide contains three FALSE teaches, and two of them cost games this corpus.** They were
never caught because prior reviews validated *teaches that fired* and never re-audited the guide's
own card facts against the primitives. Fixes are in `strategy.txt`.

### Misleading teach 1 -- ACERERAK FRAMED AS A STICKY BODY (game-costing)

Guide line 2 sells the deck as *"sticky value creatures that VENTURE INTO THE DUNGEON"*; line 41
lists *"Acererak (on enter)"* among them and says nothing more. Primitive
(`Res/sets/primitives/borderline.txt`):

```
text=When Acererak the Archlich enters, if you have not completed the dungeon Tomb of Annihilation,
     return Acererak the Archlich to its owner's hand and venture into the dungeon. -- Whenever
     Acererak the Archlich attacks, for each opponent, you create a 2/2 black Zombie creature token
     unless that player sacrifices a creature.
auto=if type(Tomb of Annihilation[dungeoncompleted]|myzones)~equalto~0 then name(Return to hand) moveTo(hand)
```

Acererak self-bounces unless **Tomb of Annihilation** is completed -- and the same guide correctly
tells the model never to pick Tomb. So in this deck Acererak is *structurally* a {2}{B} repeatable
venture and **can never be a body, never attack, and his attack trigger can never fire**. The guide
never says so. Consequences at this seat:

- **15 Acererak casts** across 3 games (vs139 x7, vs152 x4, vs148 x4) -- up from 7 in wave-30.
- **59 reply passages** across those games planning to attack with, block with, or count Acererak's
  5 power. Examples: vs152 s10 *"Attack with Acererak next turn if unblocked to apply pressure and
  trigger his attack ability"*; vs152 s19 *"attack with all creatures (Barrowin 4/4, Goblin 1/1,
  Acererak 5/5) to deal lethal damage (4+1+5 = 10 damage vs 14 life)"*; vs152 s27 *"My plan relies
  on Acererak being on board to attack."*
- The model DOES notice the bounce when narration shows it (vs152 s11 *"Acererak returned to my
  hand, so I cannot cast him now"*; vs152 s18 *"Wait, looking at the log: 'Your Acererak the
  Archlich: battlefield -> hand' implies he returned to hand"*) -- then re-adopts the false belief
  on the next decision. It is a durable model belief the render alone cannot cure, because **the
  Cast option line carries no card text at all** (0 of 163 `Cast ...` option lines at this seat
  carry `{card text: ...}`; only activated-ability lines do). The guide is the only channel that
  can carry this fact. See notes.md N-146i.

### Misleading teach 2 -- HIVE OF THE EYE TYRANT'S COST IS WRONG (card-fact error)

Guide line 82: *"Hive of the Eye Tyrant is a manland: pay **{1}{B}** to make it a 3/3 menace
attacker when you have spare mana."* Primitive:

```
auto={3}{B}:name(Becomes beholder) transforms((...becomes(Creature Beholder^3/3^black^menace) ueot...))
text=... -- {3}{B}: Until end of turn, Hive of the Eye Tyrant becomes a 3/3 black Beholder creature
     with menace ...
```

Real cost is **{3}{B} = four mana**, twice the guide's number. The render is honest (vs122 seq18:
`1. becomes beholder with Hive of the Eye Tyrant [cost: {3}{b}]`), so the model was never fooled
into an illegal attempt -- it animated the Hive 0 times this corpus -- but this is an objectively
false card fact shipped inside a deployed guide, exactly the class the skill's verify-oracle rail
exists to prevent. Fixed in strategy.txt.

### Misleading teach 3 -- "YOUR MDFC CARDS CAN BE PLAYED AS LANDS" IS UNDELIVERABLE (N-152a lane)

Guide line 87: *"Your MDFC cards (Emeria's Call, Agadeem's Awakening) can be played as LANDS - a
hand with one of them has an extra land"*; line 107: *"You are flooding / have spare lands in hand
-> play Emeria's Call or Agadeem's Awakening as a land."*

Per this wave's h4 (N-152a), the engine **structurally cannot** place an MDFC land's back face via
AI seams. The primitive shows the exact gate:

```
autohand={0}:restriction{can play land,compare(isflipped)~equalto~1} name(Agadeem, the Undercrypt)
             ... flip(Agadeem, the Undercrypt) forcetype(land)
restriction=compare(isflipped)~equalto~0
```

Land play requires `isflipped==1`; the front face is `isflipped==0`; the option is never offered.
**Independent second witness at a non-152 seat:** vs158 seq29 t14, `Your hand: Soul Shatter;
Agadeem's Awakening; Plains`, land-drop options were exactly `['Play Plains', 'Hold Plains - do not
play it now']` -- Agadeem's-as-a-land absent. The guide tells the model to count these as lands for
mulligan purposes and to play them as lands when flooding, and the game will never let it. Replaced
with truthful text.

---

## (d) 2/6 -- LOSS DECOMPOSITION (2 draw/variance, 1 play-quality, 1 engine)

### vs137 -- LOSS clean, 0 to 19, t12. **DRAW (mulligan to 5).** No play-quality defect.
seq1 Mulligan, seq2 Mulligan, seq3 Keep, seq4 bottom 2 (`Silverquill Command, Silverquill Command`)
= **a five-card hand**. 25 decisions, the shortest deck146 game. Every taught decision still made
correctly: Verse on curve at seq7 t3 on Edgewall Innkeeper (opponent's only creature and their
card-advantage engine), Soul Shatter seq17 t9, Lolth seq21 t11 with immediate `-3` at seq22. Life
20->19->18->16->9->7->0 against a clock a five-card hand cannot answer. **Not attributable to the
guide or to play quality.**

### vs139 -- LOSS adj, 4 to 8, t14. **MIXED: mulligan to 6 + the Acererak tax.**
Mulliganed once (bottom Soul Shatter). Held 20 life to t8 with opponent at 13; collapsed t12->t14
(13 -> 3). **7 Acererak casts = 21 mana** on ventures. Vanishing Verse offered only twice (deck139's
board was Arboreal Grazers / Dryad / Pollywog), taken once (seq24 t8 on Dryad of the Ilysian Grove
-- correct, it was the ramp engine); the seq20 t8 decline (Silverquill Silencer over
Verse-on-Pollywog-Symbiote) is defensible. Acererak tax is real but removal was genuinely thin.
**Roughly half variance, half the Acererak misread.**

### vs148 -- LOSS clean, 0 to 12, t13. **PLAY-QUALITY REGRESSION, and it is the Acererak misread.**
The game that makes the Acererak finding decisive. Kept 7. Vanishing Verse was **offered 7 times
and cast only twice** -- and **four of the five declines chose Cast Acererak instead**:

| seq | turn | Verse targets offered | model chose |
|---|---|---|---|
| 12 | 6 | `Armament Master` (only target) | Cast Acererak |
| 18 | 8 | `Armament Master, Oath of Gideon, Kor ally, Kor ally, Goblin` | Cast Acererak |
| 29 | 10 | `Oath of Gideon, Kor ally, Kor ally, Stone Haven Pilgrim, Kor ally` | Cast Acererak |
| 35 | 12 | `Oath of Gideon, Kor ally x4` | Cast Acererak |

Armament Master is deck148's lord -- primitive
`auto=this(gear = 1) lord(other creature[kor]|myBattlefield) 2/2` scaling to 18/18 at gear 9, i.e.
+2/+2 to every other Kor per Equipment. At seq18 t8 the board was `Armament Master {w}{w} (2/2)
[tapped]; Oath of Gideon; Kor ally #1; Kor ally #2`, 4 mana available, **three Vanishing Verses in
hand**. The model's reply, verbatim:

> CHOICE: 2 (Cast Acererak the Archlich)
>
> Reasoning: I have the mana ({2}{B}) to cast Acererak the Archlich right now. He is a 5/5 threat
> that forces the opponent to either sacrifice a creature (likely one of their small Kor Allies or
> the tapped Armament Master) or give me a 2/2 Zombie token. This immediately improves my board
> state and pressures them further. Vanishing Verse is a powerful tool, but holding it while I have
> a threat to deploy is passive; I should play my threat first, and if they attack next turn with a
> problematic creature, I can use Vanishing Verse then. The opponent's Armament Master is tapped and
> currently not threatening, so removing it isn't urgent yet.

Every load-bearing clause is false. Acererak is not a 5/5 threat here, he does not force a sacrifice
(that is an *attack* trigger on a creature that will never be on the battlefield to attack), and he
does not improve the board state at all -- he bounces to hand before the turn ends. The model traded
its own **#1 RULE** ("SPEND YOUR REMOVAL ON THEIR BEST THREAT, ON TIME") for a no-op, on the explicit
reasoning that the no-op was a threat. On the opponent's next turn the Kor board swung for 12 (19 ->
7 between seq20 and seq21). The Verse finally landed on Armament Master at seq27 t10, one turn too
late. **A genuine, guide-attributable play-quality regression -- not a pool shift.**

### vs158 -- LOSS clean, -1 to 5, t19. **ENGINE, not the seat.** New N-146h.
Mulligan to 6. Played well to t13: opponent driven from 20 to 5 (seq26 t12 alpha strike; narration
turn 13 `Opponent's life -5 (now 8)` then their own `Feed the Swarm` to 5). Then the game was lost
to a silent engine no-op on three consecutive turns:

- seq30 t14 -- chose `Cast Agadeem's Awakening {b}{b}{b}{x}` with 7 untapped sources.
- seq31 -- prompt now reads `Mana available: (no untapped sources) | Already in pool:
  {b}{b}{b}{w}{w}{w}{w}` with all seven lands `[tapped]`. Options
  `['Cast Card Normally', 'Flip Side [display toggle only ...]', 'Decline - do nothing']`. Chose
  `Cast Card Normally`.
- **Nothing happens.** Narration for that turn is exactly one line:
  `- Choose an option for Agadeem's Awakening: -> Cast Card Normally`. No `hand -> stack`, no
  resolution, no life loss, no creature returned. At t16 the card is **still in hand**.
- Repeated verbatim at seq32/33 t16 (narration turn 17, same single line) -- and t16 is the turn the
  model had drawn a `Silverquill Silencer` it could have cast as a 3/2 blocker against a growing Orc
  army. At seq34/35 t18 it selected Agadeem's a third time, then chose `Decline - do nothing`, having
  again locked its mana.

Three turns and ~21 mana burned with the opponent on 5 life; deck146 went 11 -> 7 -> -1 to a 5/5 Orc
army while holding two uncastable-because-tapped-out Silverquill Silencers. **The loss is
engine-caused.** Routed as N-146h (notes.md) with a missing-X-announce root-cause hypothesis.

### The 3/6 -> 2/6 drift: mostly pool/variance, with ONE real regression
All five repeated matchups inverted (W,W,W,L,L -> L,L,L,W,W). A complete sign flip across a repeated
pool is the signature of variance dominating these matchups, not of a decision-quality collapse --
and the two *new* wins (vs122 clean at t12, vs152 clean at t16) are the two best games deck146 has
played in the campaign, both featuring correct walker deployment. Mulligans hit 4 of 6 games (one to
five cards). **But the honest read is not "all variance":** vs148 is a clean play-quality regression
caused by a false guide framing, vs158 is an engine loss, and the Acererak cast rate more than
doubled (7 -> 15), raising the defect's visibility. The record moved 2 games; the *causes* moved from
"three losses are variance" (wave-30) to "two variance, one guide-caused, one engine-caused."

---

## (e) THE ONE FALLBACK -- unparsed_reply, vs122 seq18 t8 (priority)

`kind=priority`, `latency_ms=15738`, `choice=-1`, `fallback=unparsed_reply`.

Offered set was **{0 = pass, 1 = becomes beholder with Hive of the Eye Tyrant [cost: {3}{b}]}** --
one non-pass option. Prompt state: `Your life: 18 | Opponent life: 22`, `Mana available: {b}{w} from
4 untapped sources`, `Your hand: Vanishing Verse x3; Soul Shatter {2}{b}; Lolth, Spider Queen
{3}{b}{b} [planeswalker]`.

**Reply, verbatim (complete):**

> CHOICE: 4 (Cast Lolth, Spider Queen)
>
> Reasoning: My previous plan suggested attacking next turn, but I am currently in my Main Phase 1
> with a powerful planeswalker in hand. The strategy guide explicitly states: "Do not leave Lolth or
> Kaya rotting in hand... Once one is on your battlefield on your turn -> activate an ability... do
> not pass." Casting Lolth now allows me to use her -3 ability immediately to create two 2/1 Spider
> tokens with menace and reach. This significantly increases my board presence and damage output for
> the upcoming attack, making my lethal threat much more likely to succeed. I will then attack with
> all creatures (including the new Spiders) to push for lethal damage.
>
> PLAN: Cast Lolth, Spider Queen this turn. Activate her -3 ability immediately to create two 2/1
> Spider tokens with menace and reach. Attack with all creatures (Nadaar, two Triumphant Adventurers,
> Goblin, Hive of the Eye Tyrant if I have the mana, and the two new Spiders) to deal massive damage
> and attempt to win the game this turn. If the opponent survives, continue applying pressure with
> the board and planeswalker abilities.

**Classification: OFF-MENU INVENTION -- index out of range AND name absent from the offered set.**
Index 4 does not exist (legal set {0,1}); "Cast Lolth, Spider Queen" is not on the menu. The parser
read the leading CHOICE line, found no match on either channel, and correctly rejected -> Baka
answered. **No parser bug, no engine defect, no render defect** -- the rejection is the parser
working as specified, and the reply is otherwise well-formed (clean CHOICE/Reasoning/PLAN structure,
no truncation, 15.7s, not a spiral -- unlike wave-30's 210s spiral fallback).

Two things distinguish it from wave-30's unparsed and make it worth recording:

1. **Lolth was not castable.** She costs {3}{B}{B} = 5; the prompt says 4 untapped sources. The model
   did not merely mis-index an available action -- it invented an action it could not have afforded
   even in an ask window.
2. **The invention was licensed by the GUIDE.** The model quotes the guide's imperative ("Do not
   leave Lolth or Kaya rotting in hand...") as its justification for going off-menu. Second campaign
   witness of the invent-an-off-menu-option shape at this seat (wave-30 vs152 s17 was the first,
   PLAN-driven); the driver has shifted from *its own plan* to *a guide imperative*. Routed to
   skill.md as an authoring observation, NOT as a guide defect -- the guide text is conditioned
   correctly ("The moment you can **afford** one, CAST IT") and cannot be held responsible for a
   model that drops the condition.

Rate: 1 / 220 model calls = 0.45% at this seat, in line with the corpus's 0.42%.

---

## LATENCY (harness property, unchanged)

220 real calls: median 18.4s, p90 114.8s, max 219.3s, **37 calls over 60s**. Heavier tail than
wave-30 (p90 73s -> 115s) but the same -j contention curve; no fallback this corpus was
latency-caused. Not a seat finding.

## CORPUS GATES AT THIS SEAT

- **h1 / N-139h crash gate: PASS.** 0 crash, abort, assert, or core-dumped signatures across all six
  deck146 game stderr files. (`game-146v158` has one `FATAL:` line --
  `AIStats.cpp:load : can't load ai/baka/stats/ai_baka_deck158.stats` -- a missing stats file for the
  Step-0 debut deck; benign, not a crash.)
- **h3 / reveal eligibility: not exercised.** 0 reveal decisions at this seat.
- **h5 / display-toggle auto-pass: PARTIAL, worth a look.** deck146's priority count fell 24
  (wave-30) -> 18 (wave-31), consistent with the auto-pass. But the Flip-Side display toggle still
  reaches the model inside the Agadeem's card-face chooser (vs158 seq31/33/35: `'Flip Side [display
  toggle only - no game effect: ...]'`), so h5's suppression does not cover toggles that appear
  alongside real options. Noted, NOT a defect -- suppressing it there would be wrong since the other
  options are real.

---

## WAVE-32 EXIT PROJECTION: **OFF TRACK. HOLD THE SEAT.**

Wave-30 projected a wave-32 exit on the reasoning that the guide was converged and only a
draw-variance residual (Kaya) remained. **That projection rested on a false premise.** Kaya has now
been exercised and discharged, so the *stated* residual is gone -- but this corpus found the
"converged, frozen" guide carrying three false card facts, one of which (Acererak) is directly
responsible for a lost game and 59 impossible plans, and which sat in the guide through **four
corpora and three freeze validations** without being caught. A guide with a game-costing false teach
is by definition not converged, and rotating on the strength of three freezes that never audited its
card facts would be exiting on an unearned verdict.

The seat therefore has **new, non-speculative guide work** -- exactly what the unsentimental rotation
gate asks for -- and has never been validated with that work in place.

### What wave-32 must confirm (the exit gate)

1. **The Acererak fix lands.** Casts should collapse to genuinely spare-mana turns, and the decisive
   metric: **zero windows in which an offered removal spell with a live target is declined in favour
   of Cast Acererak** (this corpus: 4 such windows in one game). Expect the "attack with Acererak"
   planning passages (59 this corpus) to go to ~0.
2. **The Verse take-rate recovers.** Offered 16 / taken 4 (25%) this corpus vs 16 taken in wave-30.
   Removal-on-curve is this deck's #1 rule and is currently the weakest number at the seat.
3. **The MDFC text is truthful in practice** -- no mulligan reply counts Emeria's Call or Agadeem's
   Awakening as a land, and Agadeem's is not cast.
4. **N-146h is resolved, or the guide's workaround holds.** If the engine fix ships, the guide's
   "leave Agadeem's Awakening alone" line becomes a crutch and should be DEMOTED per the
   crutch-lifecycle doctrine; that demotion is itself a wave-32 item.
5. **The already-validated surfaces stay validated** (Lost Mine by name, room-by-effect, walker
   discipline, completion status line, f2 mana) -- three-corpus-stable, should need no attention.

If wave-32 shows (1)-(3) clean, the seat is genuinely converged and should rotate out at wave-33 with
a hand-off note covering N-146h and the N-152a MDFC lane. **Projected exit: wave-33, not wave-32.**
