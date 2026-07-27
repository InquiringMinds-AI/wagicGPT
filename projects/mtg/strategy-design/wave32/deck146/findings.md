# deck146 (WB Orzhov Dungeons) -- wave-32 findings

Corpus: `matchups-20260727-115847`, binary 7fabd9bd0, -T 3000, REPPENALTY=1.05. **5th corpus at
this seat.** Guide under test: `bin/Res/ai/baka/deck146_strategy.txt` = the wave-31 REVISION
(byte-identical to `wave31/deck146/strategy.txt`, verified by diff), first corpus with the three
corrected teaches (Acererak bounce, Hive `{3}{B}`, MDFC-lands).

Record: **2/6** (wave-31 2/6, wave-30 3/6). Own-seat volume **164 records / 153 real model calls**
(wave-31: 231/220 -- the drop is the two 0-card-hand games, below). Own-seat fallbacks: **ZERO**
(0 unparsed, 0 empty, 0 retracted, 0 defer, 0 `validation_reject_reask`). Crash gate: PASS.

| Opp | Result | Life | Turn | Cause class |
|---|---|---|---|---|
| 152 | **WIN** (adj at cap) | 16 / 12 | 16 | -- |
| 116 | **WIN** clean | 21 / -2 | 8 | -- |
| 158 | LOSS clean | -9 / 20 | 9 | **MULLIGAN SPIRAL (0-card hand)** |
| 139 | LOSS clean | 0 / 16 | 13 | **MULLIGAN SPIRAL (0-card hand)** |
| 105 | LOSS clean | **21** / 6 | 15 | **RENDER: poison never shown** |
| 36 | LOSS clean | 0 / 13 | 11 | PLAY (never blocked) + ENGINE (Verse walled) |

**Headline: the Acererak fix worked completely and the seat still went 2/6, because two
LARGER defects surfaced that no prior corpus had exposed -- a pre-game MULLIGAN SPIRAL that
produced two literal 0-card opening hands, and a Vanishing Verse targeting wall that makes the
deck's #1 rule inoperative against artifact and land boards.** Zero of the four losses is
attributable to the Acererak class.

---

## (a) THE ACERERAK FIX: **VALIDATED, DECISIVELY. The class is dead.**

Every wave-31 exit-gate metric for item (1) collapsed to zero.

| Metric | wave-31 | wave-32 | Verdict |
|---|---|---|---|
| Acererak casts | 15 (3 games) | **1** | collapsed |
| Windows: offered removal w/ live target DECLINED for Cast Acererak | **4** | **0** | **class dead** |
| Reply passages planning to attack/block/count his 5 power | **59** | **0** | **belief dead** |

**The one cast is the guide's exact sanctioned condition.** deck105 seq23 t14 (`ask`,
latency 106,660 ms), reply verbatim -- the pilot restates the corrected teach and applies it:

> **Acererak's Value:** As noted in your strategy guide, Acererak returns to your hand
> immediately upon entering because you haven't completed the *Tomb of Annihilation*. He provides
> no immediate board presence, no combat value, and no immediate threat. He is purely a "free
> venture" card.
> [...] Since I have **no other spells I can cast** (Kaya/Emeria are too expensive, no instants),
> I have "nothing better to do with" my mana.

Two further replies REFUSE to plan around him, unprompted:
- deck105 seq10 t6: *"Acererak bounces on entry if the dungeon isn't done, so he's not a permanent
  threat yet."*
- deck105 seq19 t12: *"Acererak is a bad play right now because he bounces to hand immediately
  (since we haven't completed Tomb of Annihilation), meaning he won't be on the battlefield to
  attack or block next turn."*

**Arrival trace (the wave-30 method rail applied to the card itself)** -- deck105 narration,
turn 15, verbatim: `- Your Acererak the Archlich: hand -> stack` / `stack -> battlefield` /
`battlefield -> hand`, immediately followed by the VENTURE dungeon chooser. The engine's behaviour
and the guide's description of it now agree exactly.

**Exit-gate item (1): PASS.**

## (b) i3 -- AGADEEM'S AWAKENING: **NOT EXERCISED. The second layer is UNTESTED.**

Arrival-traced per the brief; the result is a clean negative.

- Agadeem's Awakening **Cast option lines offered: 0**. **Casts attempted: 0**. **`Choose an
  option for Agadeem's Awakening` menus: 0.**
- Every appearance of the card is pre-game: it was in the mulligan hand at deck158 seq2, deck105
  seq4, deck139 seq2/seq3 and was shuffled back or bottomed each time. The only narration lines
  bearing its name in the whole seat are `library -> hand` / `hand -> library` pairs (deck158 x2,
  deck105 x1, deck139 x2).
- Why it never reached a castable window: 1-of in the deck, cost `{B}{B}{B}{X}` (triple black), and
  the two games with a long enough board (vs152 to t16, vs36 to t11) never drew it.

**Verdict: i3 UNRESOLVED-BECAUSE-UNTESTED at this seat.** The N-146h layer-2 question (does the
cast now fail LOUDLY, or resolve?) cannot be answered from this corpus and **carries forward
un-discharged**. The guide's "leave Agadeem's Awakening alone" crutch therefore **stays** -- the
crutch-lifecycle demotion (wave-31 exit-gate item 4) is NOT eligible. Discharging this needs a
probe deck (notes.md, N-146h-probe).

## (c) i13 -- CARD TEXT ON CAST LINES: **VALIDATED, FULL COVERAGE, NO LENGTH COST.**

Coverage: 67 numbered `Cast ...` option lines rendered at this seat. **40 carry
`{card text: "..."}`; the other 26 are the `Cast nothing right now` decline** (correctly bare) and
1 is a regex false-positive from a PLAN echo. **Every real Cast line carries rules text = 100%.**
Wave-31 measured 0 of 163.

Acererak's line, **verbatim** (deck105, the only two windows it was offered):

```
Cast Acererak the Archlich {2}{b} (5/5) {card text: "When Acererak the Archlich enters, if you
have not completed the dungeon Tomb of Annihilation, return Acererak the Archlich to its owner's
hand and venture into the dungeon. -- Whenever Acererak the Archlich attacks, for..."}
```

The truncation cuts the attack trigger -- the *right* clause to lose at this seat, since that
trigger can never fire. The bounce clause, the game-relevant one, survives in full.

**Length cost -- the batch's flagged risk: NOT OBSERVED.**

| Metric | wave-32 | reference |
|---|---|---|
| Prompt chars p50 / p95 / max | 7,181 / 16,934 / 21,668 | first corpus measured in chars |
| Reply chars p50 / p95 / max | 1,151 / 5,335 / 13,336 | campaign p50 band 1,088-1,107 (w25-w31) |
| Latency med / p90 / max / >60s | 18.3s / 51.5s / 197s / 13 | wave-31: 18.4s / 114.8s / 219s / 37 |

Reply lengths sit exactly on the campaign p50 band and the latency tail is **better** than
wave-31 (p90 115s -> 51s, calls over 60s 37 -> 13). Zero fallbacks. The two heaviest deliberations
(deck105 s23 at 106.7s, deck36 s17) are driven by a guide contradiction and an engine
contradiction respectively, not by prompt size. **i13: PASS, adopt permanently.**

---

## (d) N-146j (NEW, HIGH, ENGINE) -- VANISHING VERSE CANNOT TARGET ARTIFACTS OR LANDS

**The wave's most consequential discovery, and it inverts the Verse take-rate reading.**

Raw take-rate looks catastrophic: **Verse offered in 13 windows, taken 2 (15%)** vs wave-31's
16/4 (25%). Decomposed, it is not a play-quality number at all:

| Window class | n | Model behaviour |
|---|---|---|
| Option annotated `the only legal targets are YOUR OWN right now` | **9** | declined 9/9 -- **correct** |
| Real opponent-side targets offered | 4 | **took 2**, declined 2 for other removal/threats |

The 9 own-side-only windows are the defect. At **deck36 seq17/19/23, t10**, the option line reads
**verbatim**:

```
1. Cast Vanishing Verse {b}{w} - the only legal targets are YOUR OWN right now (warning: this
   would harm your own side - only correct if you are deliberately triggering something)
   {card text: "Exile target monocolored permanent."}
```

...while the opponent's board in the same prompt is, verbatim:

```
Opponent battlefield (creatures: 4): Tolarian Academy [tapped]; Mox Sapphire [artifact]; Mox Jet
[artifact]; Ornithopter (0/2) [artifact] [flying]; Glaze Fiend {1}{b} (0/1) [artifact] [flying]
[tapped ...]; Seat of the Synod #1 [artifact]; Salvage Slasher {1}{b} (1/1) [artifact] [tapped
...]; Great Furnace [artifact]; Yotian Soldier {3} (1/4) [artifact] [vigilance]; Vault of
Whispers [artifact]; Seat of the Synod #2 [artifact]
```

Glaze Fiend and Salvage Slasher are `mana={1}{B}`, `type=Artifact Creature` (`mtg.txt`) --
**monocolored black, plainly legal**. The pilot spotted the contradiction and burned a long
deliberation on it (deck36 seq17, verbatim):

> **Glaze Fiend** is {1}{B} (Black) - that is monocolored. **Salvage Slasher** is {1}{B} (Black)
> - monocolored. [...] Why are they not listed as legal targets?

**Root cause, traced to source (diagnosed, not hypothesised).** Verse's primitive is
`target=*[-multicolor]|battlefield` (`borderline.txt`). `TargetChooser.cpp:1066-1074` compiles
`[-multicolor]` to:

```cpp
cd->setisMultiColored(-1);
cd->SetExclusionColor(0);//not multicolored is monocolored not colorless, use iscolorless attribute
cd->SetExclusionColor(6);//restriction... green, red, blue, black or white colored only
```

Index 0 is `MTG_COLOR_ARTIFACT` and index 6 is `MTG_COLOR_WASTE`, and
`CardPrimitive::ConvertColorToBitMask` maps **`MTG_COLOR_WASTE` and `MTG_COLOR_LAND` to the SAME
bit** (`kColorBitMask_Land = 0x40`, CardPrimitive.cpp:530-535). The exclusion mask is therefore
`0x01 | 0x40`, and `CardDescriptor::match_and` rejects on **any** intersection
(`if ((mColorExclusions & card->colors) != 0) match = NULL;`). Net effect: **`[-multicolor]`
excludes every ARTIFACT and every LAND, whatever its actual colour.** The intent (exclude
COLORLESS) is stated in the comments; the implementation does not do that.

Corpus-wide corroboration: across all 21 games **no land ever appears in a Verse legal-targets
list**, and the only artifact that ever appears is a **Clue token** (deck152 s27/s29) -- token
creation does not set the artifact colour bit, so it slips the same filter. Two independent
confirmations of one mechanism.

**Consequences at this seat.** deck36 was lost at t11 holding **three Vanishing Verses** against a
board whose every non-land permanent was an artifact. This is not a play failure -- given the
render, declining was right every time. Routed engine-lane as **N-146j** (notes.md). It also makes
the guide's line 20 (`even a monocolored land`) and its `artifact` claim FALSE -- fixed in
strategy.txt.

---

## (e) THE MULLIGAN SPIRAL (NEW, HIGH) -- two 0-CARD opening hands in six games

**The single largest cause of the 2/6.** In two of six games the pilot mulliganed **seven times**
and kept, bottoming all seven -- a literal empty hand.

| Game | Mulligans | Kept | Result |
|---|---|---|---|
| vs158 | **7** | **0 cards** | L t9, -9 life |
| vs139 | **7** | **0 cards** | L t13 |
| vs105 | 4 | 3 cards | L t15 (poison) |
| vs116 | 1 | 6 cards | **W t8** |
| vs152 | 0 | 7 cards | **W** (adj) |
| vs36 | 0 | 7 cards | L t11 |

The two games kept at 7 are the two the seat did not lose to variance. **The render is not at
fault** -- it states the price with total clarity at every step (deck158 seq5, verbatim):

```
Pre-game mulligan decision (London mulligan). You have a fresh 7-card opening hand, and having
already taken 4 mulligans you will bottom 4 cards on a keep (keeping 3). Keep this hand, or
mulligan (shuffle back and draw 7 again, bottoming one more at the next keep)?
```

Per the perception-vs-strategy routing this is a **STRATEGY failure**: the state is rendered
correctly and read correctly, and the decision is wrong. Two distinct drivers, both curable in
guide prose.

**Driver 1 -- "a land in hand is not a mana source" (5 of 14 mulligan replies).** The pilot
repeatedly declares a land-rich hand landless. deck139 seq1, the OPENING seven, verbatim:

> Your hand: Hive of the Eye Tyrant (land: taps for {B}); Plains (land: taps for {W}); Swamp
> (land: taps for {B}); Triumphant Adventurer {b}{w} (1/1); Vanishing Verse {b}{w}; Vanishing
> Verse {b}{w}; Vanishing Verse {b}{w}
>
> CHOICE: 2 (Mulligan)
> This hand has zero lands that can produce mana *right now* (all three are lands, but you
> haven't tapped them yet, and you have no mana in your pool).

A three-land, both-colours, removal-dense keep, shipped on a false rule. Same shape at deck158
seq4 (Brightclimb + Hive: *"zero lands that can produce mana right now"*), deck139 seq3 (Snarl +
Swamp + Hive: *"It has zero lands"*), deck139 seq4 (Snarl: *"This hand has zero lands"*), deck139
seq6 (Snarl + Plains: *"This hand has zero lands"*).

The likely trigger sits in the prompt: the pre-game mulligan snapshot carries
`Mana available: 0 total (no untapped sources)` -- a battlefield fact rendered before a
battlefield exists -- and the pilot's phrasing (*"produce mana **right now**"*) echoes it almost
word for word. Routed render-lane as **N-146k** (notes.md); the guide gets a hard restatement
regardless, since the existing line ("never call a hand with lands 'no lands'") is buried in the
mulligan paragraph and lost 5 times.

**Driver 2 -- "prefer an early play" read as a hard requirement, with no floor.** deck158 seq5, at
mulligan 5 (would keep 3), hand = **five lands** (Snarl + Swamp + Plains + Brightclimb + Hive)
plus 2x Lolth:

> This hand has 5 lands but zero creatures or removal spells to interact with the game on turn 1.
> [...] The strategy guide emphasizes keeping hands with 2-5 lands *and* an early play; this hand
> fails the "early play" criteria completely.

Same at deck139 seq7 (mulligan 6, would keep 1; 4-land hand with Soul Shatter + Verse):
*"With 5 lands, you are highly likely to flood out [...] The strategy guide explicitly advises
keeping 2-5 lands with early plays; this hand has too many lands."* The guide's *preference*
clause is being applied as a *filter*, at hand sizes where no filter can be afforded. **The guide
has no mulligan floor.** Fixed in strategy.txt.

**i7 (N-139i London bottoming) is VALIDATED here as a by-product**: 7 mulligans -> the bottom ask
demanded `you took 7 mulligans and kept, so you must put 7 cards ... Name EXACTLY 7 cards`, labels
constant, exactly 7 named. The mechanic is correct and the 3-card-after-7-mulls shape did not
recur. (One rules misbelief in the bottom reply, deck158 seq8: *"the hand is cleared to draw a
fresh 7"* -- no effect on the answer, noted only.)

---

## (f) vs105 -- LOST AT 21 LIFE. **THE PILOT'S POISON COUNT IS NEVER RENDERED.** (N-146l, HIGH)

`gameend`: `{'won': False, 'my_life': 21, 'opp_life': 6, 'turn': 15}`. deck146 lost while
**gaining** life, to the infect alt-win.

Searched every deck146 vs105 prompt: **no line anywhere states the pilot's poison-counter total.**
The word "poison" appears only inside two reminder-text snippets riding option lines (the attacker
preview `A1. Corpse Cur (2/2) deals 2 [infect] {text: Infect (...to players in form of poison
counters...)}` and the same card's target-list entry). The board header, the life line and the
narration all omit it. Infect damage narrates as **ordinary damage with no life change**
(verbatim, turn 14):

```
- Phase: Combat damage
- Damage: 1 dealt by Plague Stinger to you
- Damage: 2 dealt by Corpse Cur to you
```

No `Counter added to you: poison`, no running total, and `Your life` stays 21 throughout. The
pilot was racing a clock it could not see and could not price blocks against. Routed engine/
render-lane as **N-146l** (notes.md). deck105's own seat is the natural owner of the fix; this is
an independent second witness from the opposite side of the table.

## (g) vs36 -- PLAY-QUALITY: THE ATTACK TAX (third campaign seat, wave-31 HL3a)

The only loss with a real decision defect. **Zero `blockers` decisions in the entire game** -- not
because the engine never asked, but because the pilot attacked with its only untapped bodies every
turn and had nothing left to block with. Life 20 -> 16 -> 11 -> 7 -> 3 -> 2 -> 0, every attack
unopposed.

The decisive window is **seq21 t10, at 2 life**. Options and reply, verbatim:

```
Your creatures that can attack:
A1. Silverquill Silencer (3/2)
A2. Spider #1 (2/1) [reach, menace]
A3. Spider #2 (2/1) [reach, menace]
```
> ATTACK: A1, A2, A3
>
> Reasoning: I am at 2 life [...] I must attack with everything I have to reduce their life total.
>
> PLAN: Attack with all three creatures (Silverquill Silencer and both Spiders) [...] **In the
> opponent's upcoming turn, I will block their attackers to survive: use the Spiders (with reach)
> to block Glaze Fiend and Salvage Slasher, and use Silverquill Silencer to block Yotian
> Soldier.**

The plan proposes blocking with the three creatures it is tapping in the same breath. This is not
a knowledge gap about blocking -- it is a missing link between *attack* and *tapped*. It is the
wave-31 headline-3 shape at a **third** seat (deck122, deck148, now deck146), and here the
contradiction is INSIDE one reply, which makes it the cleanest instance the campaign has. Guide
rule added; a render proposal is routed in general-suggestions.md.

## (h) STEADY-STATE SURFACES -- all hold

| Teach | Wave-32 evidence | Verdict |
|---|---|---|
| Pick Lost Mine **by name** | **6 / 6** (vs152 s9/s48; vs105 s13/s24; vs116 s10/s21) | HOLDS, 4th corpus |
| Room by `{room effect}`, body/drain over scry | **10 / 10** value rooms: goblin lair x4, dark pool x3, storeroom x2, mine tunnels x1. **Zero** Cave Entrance (scry) | HOLDS, 4th corpus |
| Walkers: cast when affordable, act same turn | 2 casts, 2 same-turn activations, 0 idle turns. Kaya vs152 s44 t14 -> s45 `-3: exile non-land permanent ... targeting Elite Spellbinder`; Lolth vs36 s13 t8 -> s14 `-3: create spiders`, `+0` at s18 | HOLDS, 2nd corpus |
| Completion status line (N-146f) | `Dungeons completed (you): 1 - effects that check "completed a dungeon" ... are ACTIVE for you.` (vs105 s23); creatures render `(5/5) (printed 3/3)` | HOLDS |
| Soul Shatter not into an empty board | Cast 1x (vs152 s18 t8), opposing creatures present | HOLDS |
| Dual-land mana oracle / i12 count-first line | `Mana available: 5 total (five untapped sources ... colours you can make: {b}{w})` -- zero mono-colour collapse, zero "I cannot afford" declines of offered casts | HOLDS, 4th corpus |
| Hive `{3}{B}` (wave-31 fix) | Renders `[cost: {3}{b}]`; animated once (vs152 s36) and attacked with it (s39) -- the **first Hive activation of the campaign** | HOLDS |

**i8 (N-122c/N-139k combat tags): PARTIAL at this seat.** The tags render (101 occurrences of
`[summoning sick - cannot attack this turn, but CAN block]`, 87 of `[tapped - cannot attack or
block this turn]`, 33 of `[tapped - attacking]`) and one reply consumes the tag correctly (deck105
s12: *"Corpse Cur is summoning sick and cannot attack, but it \*can\* block"*). But the paradox
verbalization survives twice, both with a tagged CAN-block creature present:
- deck152 s47 t14 (Intrepid Adversary tagged CAN block) -- the reply **contradicts itself in
  adjacent sentences**: *"The opponent has no untapped creatures to block. Intrepid Adversary is
  summoning sick and cannot attack, but it \*can\* block."*
- deck105 s25 t14 (Contagious Nim tagged CAN block): *"The opponent has no untapped creatures to
  block."*

Neither cost anything (both attacks were correct on their merits). The tag works at the render;
the residue is a stock verbal formula. Reported as a wording observation in skill.md, not a defect.

**Protocol observation (no defect, no fallback).** deck105 seq23 emitted `CHOICE: 2 (Cast nothing
right now)` on its first line, deliberated for 106.7s, genuinely reversed, and closed with
`CHOICE: 1 (Cast Acererak the Archlich {2}{b} (5/5))`. The record shows `choice=1` -- the parser
resolved to the model's final, reasoned intent, which is the correct outcome. Worth one line in
the ledger as an answer-first-protocol violation that the parser absorbed cleanly.

---

## (i) HL1 HARD GATE -- RETROACTIVE CARD-FACT AUDIT OF THE **REVISED** GUIDE: **FAIL**

Every mana cost, activation cost, MV, P/T, loyalty number, targeting restriction and zone claim in
all 134 lines of the deployed guide, checked against `Res/sets/primitives/*.txt` (`borderline.txt`,
`mtg.txt`, `planeswalkers.txt`) and against the live dungeon render. **The three wave-31 fixes are
TRUE. Four new false facts and one self-contradiction were found.**

**TRUE (re-verified, no action):** Acererak `{2}{B}` 5/5 + ETB bounce unless Tomb completed + the
attack trigger (quoted verbatim-correct) | Hive `{3}{B}` (`auto={3}{B}:name(Becomes beholder)`) |
Kaya `{3}{W}{B}`, `counter(0/0,5,Loyalty)`, `-3: Exile non-land permanent target(*[-land]|
battlefield)` | Lolth `{3}{B}{B}`, `counter(0/0,4,Loyalty)`, `-3` two 2/1 menace+reach Spiders,
`0` draw + lose 1, `-8` emblem, loyalty on your creature dying | Triumphant Adventurer `{W}{B}`
1/1 deathtouch + first strike on your turn + venture on attack | Nadaar `{2}{W}` 3/3 vigilance,
venture on enter AND attack, `lord(creature|myBattlefield) 1/1` gated on `pdungeoncompleted>0` |
Barrowin `{2}{W}{B}` 3/3, venture on enter, attack-reanimate `manacost<=3` gated on a completed
dungeon | Soul Shatter `{2}{B}` instant, untargeted, highest-MV | Emeria's Call `{4}{W}{W}{W}` =
seven mana, two 4/4 flying Angels | Lost Mine rooms (Goblin 1/1, Treasure, +1/+1 counter, drain 1)
and completion `Temple of Dumathoin: Draw a card` | Tomb's five rooms all cost BOTH players life/
discard/sacrifice and complete for The Atropal, a 4/4 deathtouch token | Mad Mage is the longest
(**9 rooms** vs Lost Mine 7 vs Tomb 5, read off the live dungeon-chooser render).

**FALSE / undeliverable (all fixed in strategy.txt):**

1. **Verse "any type (creature, planeswalker, artifact, enchantment, even a monocolored land)"**
   -- artifacts and lands are structurally untargetable (N-146j above). **Game-costing** (vs36).
   The guide was actively teaching the pilot to expect a play the engine forbids.
2. **Pelakka Predation "you choose a creature or planeswalker card"** -- the primitive is
   `target(*[manacost>=3]|reveal) reject`, text *"You may choose a card from it with mana value 3
   or greater."* It is **MV>=3, ANY card type**, and it cannot take a 1- or 2-drop creature. Both
   halves of the guide's claim are wrong.
3. **Pelakka Predation "Its back face is a land if you need the drop"** -- same N-152a lane as the
   MDFC teach the guide already corrects elsewhere (`restriction=compare(isflipped)~equalto~0`,
   never offered). A surviving instance of the exact class wave-31 believed it had removed.
4. **Silverquill Command "draw a card and drain 1"** -- the mode is *"Target player draws a card
   and loses 1 life"*; `auto=choice name(Creature gains 3/3 and you draw) ... draw:1 controller &&
   life:-1 controller`. Pointed at yourself it **costs you** 1 life; it does not drain them.
5. **Self-contradiction, MDFC (lines 99-100 vs 103-108).** Line 99: *"Your MDFC cards (Emeria's
   Call, Agadeem's Awakening) can be played as LANDS - a hand with one of them has an extra
   land."* Line 103: *"EMERIA'S CALL AND AGADEEM'S AWAKENING ARE NOT LANDS FOR YOU."* The wave-31
   revision appended the correction and never deleted the sentence it corrects. **The pilot hit it
   live** -- deck139 seq2, verbatim: *"It has 2 lands (Plains and the back of Agadeem's Awakening
   which counts as a land drop, though we play the spell side if we have mana, or the land side if
   we need fixing? Wait [...] The strategy guide says 'Emeria's Call and Agadeem's Awakening are
   NOT LANDS for you... Count them as SPELLS ONLY'. So this is a 1-land hand."* Resolved
   correctly, but it paid a full reasoning detour inside a mulligan chain that then span to 0
   cards.

**Two accuracy refinements (not false, but printed identity rather than realized behaviour, per
wave-31 HL2):** Shineshadow Snarl `auto=if type(*[plains;swamp]|myhand)~equalto~0 then
tap(noevent)` -- enters TAPPED unless you reveal a Plains or Swamp; Hive `auto=if
compare(type:land:myBattlefield)~morethan~2 then tap(noevent)` -- enters tapped if you already
control **more than two** lands. Both were mis-stated by the pilot at the mulligan seam (deck139
seq3 asserted Hive *"enters tapped because you control no other lands"* -- exactly backwards), so
both go in. Emeria's Call's indestructible rider is also omitted and added.

**HL1 VERDICT: the revised guide FAILED its first retroactive audit.** Per the wave-31 hard gate,
a seat whose guide fails the card-fact audit is not eligible for an exit projection.

---

## WAVE-33 EXIT PROJECTION: **OFF TRACK AGAIN. HOLD THE SEAT. Projected exit: wave-34.**

Against wave-31's five-item exit gate:

| # | Gate | Status |
|---|---|---|
| 1 | Acererak fix lands; 0 removal-declined-for-Acererak; ~0 attack-with-Acererak passages | **PASS** (0 and 0) |
| 2 | Verse take-rate recovers | **CONFOUNDED, then PASS on the real denominator** -- 2/4 on windows with a live opponent target; the 9 remaining declines are N-146j, an engine wall |
| 3 | MDFC text truthful in practice; Agadeem not cast | **PARTIAL** -- Agadeem never cast (0 offers), no reply counted an MDFC as a land, **but** the guide still contains the contradiction that made the pilot litigate it |
| 4 | N-146h resolved or the crutch holds | **UNTESTED** -- 0 exercises; crutch retained, demotion not eligible |
| 5 | Already-validated surfaces stay validated | **PASS** -- Lost Mine 6/6, rooms 10/10, walkers 2/2 same-turn, completion line, f2/i12 mana all clean |

Item 1 -- the item the whole hold was called for -- passed cleanly, with the strongest evidence
the seat has produced (0 of 59 planning passages survive, and the single cast quotes the new
teach). The seat still cannot rotate, for three independent reasons:

- **The HL1 audit FAILED on the revised guide** (four false facts + a live-hit self-contradiction).
  The hard gate is unambiguous: no exit projection on a failing audit. This is now the SECOND
  consecutive wave in which an audit of this guide found game-costing false teaches -- which is
  itself the finding: the wave-31 revision fixed the three facts it was handed and left/introduced
  five more, because it audited the lines it touched rather than the whole guide.
- **Two brand-new HIGH defects, neither yet validated with a fix in place**: the mulligan spiral
  (guide-lane, 2 of 4 losses, two 0-card hands) and the attack tax (guide-lane, the vs36 loss).
  The seat has never been observed with either teach deployed.
- **i3/N-146h is undischarged** and now needs a probe rather than another corpus of waiting.

The guide revision this wave is substantial and evidence-backed (five card-fact corrections, a
mulligan floor, an attack-tax rule). **Wave-33 must confirm:**

1. **Zero mulligans past the floor.** No keep below 5 cards; no reply calling a hand with lands
   "zero lands"; specifically zero 0-card or 1-card hands. (This corpus: two 0-card hands.)
2. **The attack tax fires.** Zero windows where the pilot attacks with its last untapped
   non-vigilant bodies while facing lethal-or-near crack-back; zero replies planning to block with
   creatures declared as attackers in the same reply. (This corpus: 1 fatal, 1 reply-internal
   contradiction.)
3. **The corrected card facts hold** -- no reply expects Verse to hit an artifact or land, no reply
   expects Pelakka Predation to take a 2-drop creature or to be played as a land, no MDFC counted
   as a land, no reasoning detour over a guide contradiction.
4. **N-146h discharged by probe** (notes.md) rather than by another corpus of absence.
5. **The Acererak class stays dead and the steady-state surfaces stay clean.**

If (1)-(3) come back clean and (4) is discharged, the seat is genuinely converged and rotates at
wave-34 with a hand-off covering N-146j, N-146k, N-146l and the N-152a MDFC lane.
