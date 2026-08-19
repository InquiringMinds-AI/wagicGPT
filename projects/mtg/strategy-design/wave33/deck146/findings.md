# deck146 (WB Orzhov Dungeons) -- wave-33 findings

Corpus `matchups-20260819-070739`, binary **bdb30d8e7**, -T 3000, REPPENALTY=1.05, -j 3.
**6th corpus at this seat.** Guide under test: `bin/Res/ai/baka/deck146_strategy.txt` = the
wave-32 revision (mulligan floor + attack tax + five card-fact corrections), first corpus with
those teaches deployed.

Record **4/6** (wave-32 2/6, wave-31 2/6, wave-30 3/6). Own-seat volume **238 records / 227 real
model calls**. Own-seat fallbacks: **ZERO** (0 unparsed, 0 empty, 0 defer, 0 retracted, 0
`all_assignments_illegal`). Crash gate: **PASS** (0 signatures in all six stderr files).

| Opp | Result | Life | Turn | Cause class |
|---|---|---|---|---|
| 139 | **WIN** clean | 15 / -3 | 18 | -- |
| 152 | **WIN** clean | 12 / -1 | 16 | -- |
| 105 | **WIN** clean | 19 / 0 | 10 | **i1 poison render** (wave-32's loss-at-21-life inverted) |
| 116 | **WIN** cap adjudication 20-14 | 20 / 14 | 14 | **latency spiral** (30.4 min of seat inference) |
| 36 | LOSS clean | -1 / 13 | 13 | MATCHUP WALL (Akroma's Memorial pro-black + colourless artifact board) |
| 158 | LOSS clean | -5 / 16 | 15 | amass race (Orc army 6/6 -> 12/12) |

**Headline: the mulligan pathology is DEAD -- 2 mulligans in six games (wave-32: 19, with two
literal 0-card hands), zero false-landless replies, minimum kept hand 5 cards. And the HL1
retroactive card-fact audit of the SHIPPED guide FAILS again, on three claims, one of which
(Vanishing Verse "cannot target an artifact or a land") was made false BY the i3 fix this wave --
a crutch that outlived its defect. The seat cannot exit on a failing audit; the revision this
wave is entirely crutch-demotion + off-by-one repair, which is the shape of a converging seat.**

---

## (a) i9 / N-146k -- THE MULLIGAN CLUSTER: **DEAD. Decisive validation.**

Pregame prompts are hand-only at this seat, with the engine-computed header, in **8 of 8**
pregame asks (5 games x 1 opening ask + 3 asks in vs158). Measured against the wave-32 pathology:

| Metric | wave-32 | wave-33 | Verdict |
|---|---|---|---|
| Mulligans, 6 games | **19** (two 7-mulligan chains) | **2** (both vs158) | collapsed |
| Games with 0 mulligans | 2 / 6 | **5 / 6** | -- |
| Smallest kept hand | **0 cards** (x2) | **5 cards** (x1) | floor holds |
| `Mana available:` line in a pregame prompt | every one | **0 / 8** | surface removed |
| Battlefield facts in a pregame prompt | every one | **0 / 8** | surface removed |
| Replies calling a hand with lands "zero lands" | **5 / 14** | **0 / 8** | class dead |
| `Mana available: 0` echoed in a mulligan reply | 5 | **0** | **N-146k echo class DEAD** |

The new header is what the pilot reasons on, verbatim (vs158 seq1):

```
Your hand (7 cards), counted by the engine: 3 lands, 4 spells.
Mana sources among those lands, counted by the engine: {W} 3. A land in your hand IS a mana
source: it produces mana from the turn you play it, so count these when you judge this hand.
Cheapest spell in this hand: Vanishing Verse {b}{w} (mana value 2).
Playing every land in this hand would not cover any spell in it.
```

**Floor satisfiability is now provable against the header, and both mulligans are correct on it.**
- vs158 seq1: header `{W} 3` and no black source; every spell in the hand is `{B}{W}` or `{B}`;
  the header's own last line says *"Playing every land in this hand would not cover any spell in
  it."* Colour-screw mulligan at 7 -- correct, and reasoned FROM the header, not against it.
- vs158 seq2: header `1 land, 6 spells`, `{B} 1`, cheapest spell MV 3. One-land mulligan at 6 --
  correct, and the reply quotes the guide's keep rule by name.
- vs158 seq3: header `2 lands, 5 spells`, `{W} 2, {B} 2` (two Snarls), *"Playing every land in this
  hand would cover the cost of: Vanishing Verse, Vanishing Verse, Triumphant Adventurer."* KEPT at
  "keeping 5" -- the exact hand shape the wave-32 pilot shipped five times. The reply cites the
  floor: *"This hand hits the 'keep floor' comfortably."*

Two independent teaches fired and neither is now separable from the other: the guide's HARD FLOOR
and the i9 header. The header is doing the heavier lifting -- the wave-32 false-landless formula
("zero lands that can produce mana right now") tracked the removed `Mana available:` line word for
word, and with the line gone the formula does not appear once. **i9 at this seat: PASS.**
No leyline resolved in pregame at this seat -- the known consequence is **0 occurrences**.

## (b) i3 / N-146j -- VERSE TARGETING: falsifier **PASSES**; the positive half **UNEXERCISED**

The fix is in the binary (`TargetChooser.cpp:1066-1092`: the two `SetExclusionColor` calls are
gone, `-multicolor` now asks only `setisMultiColored(-1)`), and Vanishing Verse's primitive was
re-written to the sanctioned idiom `target=*[-multicolor;-colorless]|battlefield`
(`borderline.txt:123898`).

- **FALSIFIER (zero colourless targets): PASSES.** 21 windows offered a real target list; the 23
  distinct names across them are all coloured non-artifact permanents (Arboreal Grazer, Dryad of
  the Ilysian Grove, Pollywog Symbiote, Ranger Class, Wolf, Intrepid Adversary, Elite Spellbinder,
  Nissa, Gemrazer, Blight Mamba, Cystbearer, Elvish Piper, Pelakka Wurm, plus own-side Nadaar /
  Lolth / Spiders / the red Goblin token). **Zero artifacts, zero lands, zero Clue-token slips.**
- **POSITIVE HALF (mono artifact creatures listed): NOT EXERCISED.** A whole-seat regex over every
  prompt for a permanent rendered with `[artifact]` and a coloured mana cost returns **0** -- no
  coloured artifact permanent appeared at this seat in any of the six games. deck36's board this
  corpus is 100% generic-cost artifacts (Ornithopter {0}, Mox {0}, Cathodion {3}, Yotian Soldier
  {3}, Silver Myr {2}, Akroma's Memorial {7}, artifact lands); wave-32's witnesses (Glaze Fiend,
  Salvage Slasher, both `{1}{B}`) were never drawn. The eight `only legal targets are YOUR OWN`
  windows in vs36 are therefore **RULES-CORRECT** now, not the wave-32 wall: a colourless
  permanent is not monocolored and Verse may not touch it.
- **Take-rate.** 41 Verse option-line windows; 20 own-side-only (declined 20/20, correct), 21 with
  a real list, **7 taken (33%)**. Wave-32 was 2/4 on the same denominator. Every take exiled the
  best opposing body available (Pollywog Symbiote, Ranger Class x2, Elite Spellbinder, Wolf,
  Gemrazer, Arboreal Grazer). Six declines in vs105 held Verse against 1/1 infect bodies while
  ahead on board -- defensible, and that game was won at turn 10.

**Consequence for the guide: the wave-32 crutch is now a FALSE TEACH.** Line 54 says *"IN THIS
GAME Vanishing Verse CANNOT target an ARTIFACT or a LAND, whatever color it is"*. Post-fix the
rule is colour, not type: a monocolored artifact creature IS a legal target, a colourless one is
not. The pilot has already internalised the false version -- vs36 seq15, verbatim: *"not valid
targets per the specific restriction that it cannot hit artifacts/lands/multicolored"* -- and once
inverted it in the wrong direction, vs116 seq37: *"So Vanishing Verse is useful for hitting
lands."* Demoted in strategy.txt.

## (c) THE ACERERAK CLASS -- **STAYS DEAD** (2nd consecutive corpus)

7 windows offered `Cast Acererak`, **1 cast** (vs36 seq33, t12, at 4 life with an all-artifact
board, no legal removal target and nothing else castable = the guide's sanctioned "mana you have
nothing better to do with"). **Zero passages counting his 5 power in damage math; zero windows
where offered removal with a live target was declined for Acererak.** All nine Acererak passages
in replies are refusals or correct descriptions, e.g. vs36 seq27: *"Acererak is a bad play here
because he bounces immediately... leaving us with no board presence."* Under the whole-corner
stopping clause: WATCH, not under-spec. No guide change.

## (d) THE WAVE-32 LOSS DRIVERS -- both closed

- **Attack tax (the vs36 wave-32 loss shape).** Zero replies plan to block with a creature they
  declare as an attacker in the same reply. The teach fires explicitly and correctly: vs36 seq32
  at 4 life declared **no attackers** with four bodies available; vs158 seq19 declined the only
  attack with the line *"If I attack with Barrowin, I lose my primary blocker... PLAN: Hold back
  all creatures this turn to preserve blockers."* The wave-31-headline-3 shape did not recur.
- **N-146l poison invisibility -- FIXED, and it flipped the game.** The vs105 rematch of the
  wave-32 loss-at-21-life renders the poison line in **15/15** records from first infect damage
  onward: `Poison counters (you): 3 of 10 - you LOSE the game at 10 poison counters, whatever the
  life total is; 7 more end it.` Infect damage narrates distinctly (`Infect damage: 1 from Blight
  Mamba to you - dealt as POISON COUNTERS, not life loss`), and the blocker forecast prices it:
  `Unblocked, these attackers put up to 3 POISON COUNTERS on you - you would be at 4 of 10`.
  deck146 won at turn 10, 19 life, 3 poison. N-146l: **CLOSED at this seat.**

## (e) THE COMMIT COUNTER (counted fields, per the instrument spec)

| Field | Value |
|---|---|
| Replies carrying a `PLAN:` line | **207 / 227** |
| Replies with NO `PLAN:` line (protocol-compliant per interim `f46dd58ee`) | **20 / 227** |
| `post_plan_overrun` present | 227 / 227 |
| `post_plan_overrun` **nonzero** | **22 / 227 (9.7%)**; sum 68,556 chars; mean 3,116 |
| `commit_retracted` | **0** |
| `pruned_pairs` | **0** (never populated at this seat) |
| Fatal overruns (overrun caused a wrong committed decision) | **0** |
| Post-answer self-corrections that CHANGED the committed decision | **4 / 227 (1.8%)** |

The four self-corrections all improved the decision and all resolved to the model's final intent
(no retraction fired): vs139 s11 Verse -> Pelakka Predation; vs152 s36 Cast nothing -> Emeria's
Call (the game-winning haymaker); vs36 s33 Cast nothing -> Acererak; vs158 s24 `B1:A3, B1:A4` ->
`B1:A1` (a legality self-repair -- one blocker cannot block two attackers). So the 4.2%
corpus-wide class **does carry decision value at this seat**, and native pre-answer reasoning
would have to reproduce it. See general-suggestions.md for the evidence-only contribution.

**The spirals ride a REPRESENTATION AMBIGUITY, and it is the same one three times.** The four
worst specimens in the corpus are all at this seat:

| Spec | lat | overrun | Content |
|---|---|---|---|
| vs36 s22 | 201.0s | 15,163 | pro-black damage re-derivation + *"Wait, the prompt asks for a choice on the dungeon?"* |
| vs152 s45 | 213.8s | 12,155 | **counts explore counters by hand to work out which room it is on** |
| vs116 s35 | 214.1s | 9,091 | **"the options are weirdly skipping Sandfall Cell... perhaps Oubliette is a typo"** |
| vs116 s37 | 228.3s | 8,638 | *"is this MY discard or theirs?"* + anthem-power re-derivation |

Three of the four are the **dungeon room-branch menu**: it offers two room names and their
`{room effect}` text but never states which room the venture is currently at, nor which room each
option advances to. The model then re-derives its dungeon position from narration counters and
never reaches certainty -- 12k and 9k characters of it. Per the trust doctrine the obligation runs
one way: the model owes the surface belief, the surface owes the model truth, and an
under-specified menu is a scope failure of the same family. This is the highest-value render fix
at this seat (N-146n, notes.md). The anthem re-derivation at s37 is secondary and rides the same
reply: the board render already states `Barrowin (5/5) (printed 3/3)`, `Nadaar (4/4) (printed
3/3)` and the model recomputes all four from base anyway -- but only AFTER the discard-ownership
question had already destabilised it.

**And the spiral is not merely wasted output -- it becomes durable prompt state.** The PLAN line
is the only thing carried between decisions, and 14 replies emitted a PLAN tail over 800 chars
(max 15,164). At vs116 the 9k s35 spiral was re-injected verbatim as `YOUR PLAN (as you last
stated it)` into **both** following prompts (s36, s37), and s37 then re-ran the identical lethal
re-derivation. Post-answer scratch is contaminating the next decision, not just the token bill.

## (f) NEW HIGH -- N-146m: an option NAME that matches no option is silently resolved by INDEX

**The single worst decision at this seat, and it is a parser/protocol defect, not a play error.**
`deck146 vs deck116 seq34`, kind `ask`, latency 11.6s, no fallback. The ask is the
**dungeon-selection** menu (Tomb / Mad Mage / Lost Mine). The reply, verbatim first line:

```
CHOICE: 1 (Nadaar, Selfless Paladin #1 (5/5) [vigilance] [your battlefield])
```

-- the answer to seq33, which the s34 prompt still carried as `YOUR PLAN`. No option in the s34
list is named Nadaar. The reconciler took the **number** and committed
`chosen_text = Tomb of Annihilation`. The guide's #1 dungeon teach ("pick LOST MINE by NAME") was
followed 8 of 10 times this corpus; **this is one of the two misses and it was not a decision at
all.** Everything downstream is its cost: the Tomb room chain, the 214s veils-of-fear spiral, the
228s discard spiral, Acererak discarded to a Tomb room, and the game hitting the cap.

Class rate at this seat: **1 / 189** replies whose `CHOICE: N (name)` names something absent from
the option list (the other 27 hits are `CHOICE: 0 (pass)` on option-less priority asks -- benign).
Rare, silent, and decision-fatal. Routed engine-lane as **N-146m** (notes.md); a name/index
disagreement should re-ask or fall back, never commit the index.

## (g) HL1 HARD GATE -- RETROACTIVE CARD-FACT AUDIT OF THE SHIPPED GUIDE: **FAIL**

Every cost, P/T, loyalty, trigger condition, targeting restriction and zone claim in the deployed
guide, checked against **scryfall.com Oracle text** AND `Res/sets/primitives/*.txt` AND the live
render. Three false claims, one stale-surface teach, one wording correction.

**TRUE (re-verified against Oracle + primitive, no action):** Acererak `{2}{B}` 5/5, bounce unless
Tomb completed, attack trigger = each opponent sacrifices a creature or you get a 2/2 Zombie |
Kaya `{3}{W}{B}`, 5 loyalty, `-3: Exile target nonland permanent`, `+1` ghostform on a **nontoken
creature you control** (primitive `target(creature[-token]|mybattlefield)` -- the guide's "your
own" matches the ENGINE; Oracle allows any nontoken creature, divergence noted in notes.md) |
Lolth `{3}{B}{B}`, 4 loyalty, `-3` two 2/1 Spiders with reach and menace, `0` draw + lose 1, `-8`
emblem, loyalty when your creature dies | Triumphant Adventurer `{W}{B}` 1/1 deathtouch, first
strike during your turn, venture on attack | Nadaar `{2}{W}` 3/3 vigilance, venture on enter AND
attack | Barrowin `{2}{W}{B}` 3/3, venture on enter, attack-reanimate MV<=3 gated on a completed
dungeon | Silverquill Command all four modes incl. "aim the draw-and-lose-1 at the OPPONENT"
(primitive has both `draw:1 controller && life:-1 controller` and the `opponent` variants) |
Pelakka Predation = MV>=3, ANY card type, and still spell-only in this engine (vs116 s30 offered
only `Cast Card Normally` / `Flip Side [display toggle only]`) | Shineshadow Snarl reveal-or-tapped
| Lost Mine 7 rooms / Tomb 5 / Mad Mage 9 and every room effect, read off the live render.

**FALSE / stale (all fixed in strategy.txt):**

1. **Verse "CANNOT target an ARTIFACT or a LAND, whatever color it is"** -- made false by i3 this
   wave (see (b)). The rule is colour, not type. **A crutch that outlived its defect** and the
   pilot is already reciting it, and once inverted it.
2. **Hive of the Eye Tyrant "enters TAPPED if you already control MORE THAN TWO other lands"** --
   **off by one, and game-relevant.** Oracle: *"If you control two or more other lands, this land
   enters tapped."* Live witness, vs139 seq6: board = Plains, Swamp, **Hive [tapped]** -- two
   other lands, entered tapped. (Negative control, vs116 seq4: Swamp + Hive untapped, one other
   land.) The engine's `compare(type:land:myBattlefield)~morethan~2` counts the entering land
   itself, so engine and Oracle agree and only the guide is wrong. The wave-32 audit read the
   script literally and mis-transcribed it -- a lesson for the skill (skill.md P1).
3. **The whole `Mana available: 0 total` mulligan paragraph** (guide lines 12-15) teaches the
   pilot how to read a line that **no longer exists** in any pregame prompt (0/8 this corpus).
   Teaching against a removed surface is dead weight in the highest-stakes paragraph of the guide,
   and it invites the pilot to look for a line the engine will not show it.
4. **Emeria's Call "your other creatures gain indestructible"** -- Oracle and primitive
   (`all(creature[-angel]|myBattlefield) indestructible uynt`) say **non-Angel** creatures you
   control; the two Angels it makes are not covered. Wording corrected.
5. **Soul Shatter "the opponent SACRIFICES their biggest creature"** -- Oracle says creature **or
   planeswalker** with the greatest mana value among both. The primitive sacrifices
   `notaTarget(creature[...]|myBattlefield)` -- **creatures only** -- so the guide's "does NOTHING
   if they control no creature" is ENGINE-TRUE and stays. The engine/Oracle divergence is docketed
   (notes.md, N-146p) rather than taught.

**HL1 VERDICT: FAIL (three false claims).** Second consecutive failing audit, but the composition
has changed completely: no false claim this wave was invented, and none is a misreading of a card.
Two are *crutches and transcriptions that the engine moved out from under*, one is a stale surface
reference. That is the failure mode of a guide converging against a moving engine, not of a guide
that was never grounded.

## (h) STEADY-STATE SURFACES -- all hold

| Teach | wave-33 evidence | Verdict |
|---|---|---|
| Pick Lost Mine **by name** | **8 / 10** (vs139 s9, vs152 s7+s29, vs105 s11, vs36 s8+s26, vs116 s6+s18, vs158 s11). Both misses explained: vs105 s24 picked Tomb with the game already lethal that step (won that turn); vs116 s34 is N-146m, not a decision | HOLDS, 5th corpus |
| Room by `{room effect}`, body/drain over scry | **15 / 15** value rooms (goblin lair x7, dark pool x5, storeroom x3). **Zero** Cave Entrance (scry) | HOLDS, 5th corpus |
| Walkers: cast when affordable, act same turn | 4 casts (Kaya x2, Lolth x2), **5 activations, 0 idle walker turns** (139 s23->s24, 152 s23->s24 and s38, 139 s37->s38, 158 s22->s23 and s28) | HOLDS, 3rd corpus |
| Attack tax / hold blockers at low life | vs36 s32 (no attackers at 4 life), vs158 s19 (hold all) | **NEW, HOLDS** |
| Mulligan floor | 5-card keep at "keeping 5", 0 hands below 5 | **NEW, HOLDS** |
| Dual-land mana oracle | `colours you can make: {b}{w}` throughout; zero "I cannot afford" declines of offered casts | HOLDS, 5th corpus |
| Completion status line (N-146f) | `Dungeons completed (you): 2 - effects that check "completed a dungeon" ... are ACTIVE for you`; creatures render `(5/5) (printed 3/3)` | HOLDS |
| Acererak bounce teach | 1 cast / 7 offers, in the sanctioned condition | HOLDS, 2nd corpus |
| Agadeem's Awakening | **0 offers, 0 casts** (never drawn to a castable window) | N-146h layer 2 still **UNEXERCISED** -- probe-lane, not re-diagnosed here |

## (i) THE 4/6 -- ATTRIBUTION (per INTERIM CHANGES; do not credit the batch blind)

- **vs105 WIN: credit i1/i2 (poison render), NOT the guide.** This is the exact game deck146 lost
  at 21 life in wave-32; the only changed variable that touched it is poison legibility -- at both
  seats. deck105 fell 6/6 -> 2/6 corpus-wide this wave, which is the poison-pair validation
  landing, and this seat is one of its new losses.
- **vs116 WIN: credit the CAP, not play.** Adjudicated 20-14 at turn 14 after ~47.5 of 50 minutes
  of inference, 30.4 of them at this seat (p90 199.7s). The seat was ahead and correct throughout;
  it did not win the game, it out-survived the clock. Do not count it as a decision-quality win.
- **vs139 / vs152 WINS: credit is genuinely shared and cannot be split.** Both are games the
  wave-32 seat would plausibly have mulliganed away (its two 0-card hands were vs139 and vs158);
  the mulligan cluster's death is jointly i9 (header, engine) and the guide's HARD FLOOR, and no
  measurement here separates them. Report as joint.
- **The two losses are not guide-lane.** vs36 is a matchup wall the pilot diagnosed correctly and
  in advance (Akroma's Memorial grants the whole artifact board protection from black; every
  deck146 creature is B or WB, so its damage is prevented and deathtouch never triggers; Verse is
  rules-correctly dead against a colourless board; Soul Shatter and Kaya were never drawn). vs158
  is an amass race lost to a 12/12 Orc army with correct chump-and-hold blocking throughout.

## WAVE-34 EXIT PROJECTION: **HOLD. Exit projected wave-34, on a short and fully specified gate.**

| # | Gate | Status |
|---|---|---|
| 1 | Zero mulligans past the floor; no 0/1-card hands; no false-landless replies | **PASS** (2 mulligans, min keep 5, 0 false) |
| 2 | Attack tax fires; no block-with-my-attackers replies | **PASS** (0 occurrences, 2 explicit holds) |
| 3 | wave-32's corrected card facts hold | **PASS** (no MDFC counted as a land, no Pelakka-as-land, no Verse-hits-multicolor) |
| 4 | N-146h discharged by probe | **STILL UNEXERCISED** (0 offers; probe request stands from wave-32) |
| 5 | HL1 retroactive audit of the SHIPPED guide | **FAIL** -- 3 false claims |

The hold is called on gate 5 alone, and gate 5 is the hard gate. Everything the wave-32 revision
was written to fix worked on its first corpus, and the seat's record doubled. **Wave-34 must
confirm, on a guide whose only changes are the three audit repairs:**

1. **The audit passes on the revised guide** (no new false facts).
2. **Zero replies asserting Verse cannot hit an artifact**, and if a monocolored artifact creature
   is ever offered as a target, it is taken or declined on merit -- i3's positive half needs a
   witness this seat could not produce. If deck36 again fails to draw a coloured artifact,
   discharge it from deck36's seat or a probe, not by a seventh corpus of waiting.
3. **Zero N-146m index-only commits** (1 this corpus, decision-fatal).
4. **The dungeon room menu names its current room** (N-146n) -- if it does, the 200s+ spiral class
   at this seat should collapse; that is the falsifiable prediction.
5. Mulligan cluster, attack tax, Lost Mine, rooms, walkers and the Acererak class stay dead/clean.

If (1)-(3) come back clean the seat rotates at wave-34 with a hand-off covering N-146m, N-146n,
N-146o (Nadaar self-anthem card script), N-146p (Soul Shatter planeswalker divergence), N-146h
layer 2 and the N-152a MDFC lane.
