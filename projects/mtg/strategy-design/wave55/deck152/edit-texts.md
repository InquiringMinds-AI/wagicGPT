# deck152 - wave-55 edit texts (before -> after), each with its finding

Base = the LIVE guide `bin/Res/ai/baka/deck152_strategy.txt` (wave-54 reviewer revision + boundary
pass dd0cd2d74, which re-keyed the attack rule to the header's N). Size 60,009 -> **63,865 bytes**
(band 41-71 KB). Three edits. deck152 **3-3** (W v146 29/-5 T18, v123 20/-2 T34, v130 25/-5 T22;
L v125 -11/18 T64, v126 -2/24 T13, v162 0/6 T16). **575 decision records** at the six deck152 seats,
316 of them in `152v125` alone - the corpus's largest seat and its longest game.

---

## 152-A (NEW, HIGH) - an A-line is proof the creature is untapped; the plan is not evidence

**Finding.** `152v130` **seq 34** (T23, 25 life vs **7**). Render:
`Your battlefield (10 permanents listed, of which 4 are creatures, 4 of them able to attack right
now ...)`; four A-lines, **each** reading `[no creature they control can block this attacker]`;
header `They have 0 untapped creatures able to block: every attacker you declare this turn is
unblocked unless something changes first.` A1 was `Sigarda, Champion of Light (5/5) [trample,
flying]`. The reply was `ATTACK: A2, A3, A4` with `PLAN: ... Sigarda is already tapped.`
Sigarda was not tapped - a tapped permanent renders `[tapped]` on the battlefield line and gets no
A-line at all. The seat invented a tap state and left 5 unblockable power at home on a swing the
opponent could not interact with.
Classification: **STRATEGY / trust-doctrine.** Nothing on the screen was false or missing; the model
overrode the menu with a remembered fact. Cost was zero this game (11 was already lethal from 7),
which is why it is a rule edit and not a headline.

**Before** (#2 RULE):
```
The render already
lists only creatures that CAN attack - do not re-derive eligibility.
```
**After:** the same sentence, then:
```
AN A-LINE IS PROOF THE CREATURE IS UNTAPPED AND LEGAL RIGHT NOW. If a name is printed as A1, A2,
A3 it is not tapped, not summoning-sick and not restricted - the engine checked before it wrote
the line, and your battlefield line says the same number a second time ("of which N are creatures,
N of them able to attack right now"). A creature that IS tapped is printed with "[tapped]" on the
battlefield line and gets NO A-line at all. So never leave a name home because you remember it
attacking last turn, or because your plan says it is tapped: the menu is this combat's truth and
the plan is not. WHAT THIS COST: with the header reading "They have 0 untapped creatures able to
block", the opponent on 7 life and FOUR A-lines each reading "[no creature they control can block
this attacker]", this seat sent three and wrote "Sigarda is already tapped" about the 5/5 trampling
flier printed as A1 - five power left at home for no reason on a lethal swing.
```

---

## 152-B (NEW, HIGH) - a counterspell matchup posture keyed to the `[tapped]` markers

**Finding.** `152v125` (L -11/18, **T64**, 316 records). Eight creature spells countered, read off
the game log in the final prompt: Luminarch Aspirant (Fall of the Gavel), Elite Spellbinder
(Essence Scatter), Katilda (Essence Scatter), Elite Spellbinder (Cancel), Katilda (Dream Fracture),
Augur of Autumn (Dream Fracture), Sigarda (Cancel), Intrepid Adversary (Essence Scatter).
Opponent untapped permanents at each cast, counted off the rendered
`Opponent battlefield (...)` line by absence of `[tapped]`:

| seq | turn | cast | opp untapped | opp hand | outcome |
|---|---|---|---|---|---|
| s53 | 25 | Elite Spellbinder | 6 | 5 | countered |
| s63 | 29 | Luminarch Aspirant | 6 | 6 | countered |
| s68 | 31 | Sigarda | 6 | 5 | countered |
| s79 | 33 | Elite Spellbinder | 6 | 5 | countered |
| s95 | 39 | Katilda | 5 | 7 | countered |
| **s101** | **41** | **Tovolar's Huntmaster (6/6)** | **2** | 7 | **RESOLVED** |
| s177/s179/s180 | 51 | Katilda / Augur / Sigarda | 7 / 8 / 7 | 7 | 2 of 3 countered |

The one cast made into a tapped-out opponent is the one that resolved. The seat led **34 life to
11 on turn 19** and lost at -11 on turn 65. The `[tapped]` markers are on every row of every one of
those prompts, so the number was always readable.
Classification: **STRATEGY**. Note the edit does NOT tell the seat to stop deploying - the deploy
floor (#3 RULE) is untouched; it changes WHICH row is taken when U is high.

**Before:** `MATCHUP POSTURE:` followed directly by `- vs REMOVAL/GRIND:`.
**After:** a new first entry:
```
- vs COUNTERSPELLS / OPEN MANA (the log shows Cancel, Essence Scatter, Dream Fracture or Fall of
  the Gavel countering your creatures): THE OPPONENT BATTLEFIELD LINE TELLS YOU WHEN THEY CAN DO
  IT. Every permanent of theirs that is tapped is printed "[tapped]"; count the entries with NO
  "[tapped]" on them and call that U. That is their open mana, and it is the only number in this
  matchup you have to read.
  * U of 2 or less, or their hand size at 3 or below -> this is the window. Cast your BEST
    creature now, and cast a second one in the same turn if the rows let you.
  * U of 3 or more with their hand at 5 or more -> deploy the CHEAPEST creature on the menu
    first, not the best one, and hold the expensive one for the same turn or a later window.
    A counterspell spent on a two-drop is a counterspell that is not on Sigarda.
  * NEVER hold everything. A hand of threats loses this matchup exactly as fast as a countered
    one; the deploy floor (#3 RULE) still applies, it only chooses a different row.
  WHAT THIS COST, one 65-turn game: EIGHT creature spells were countered one at a time -
  Luminarch Aspirant, two Elite Spellbinders, two Katildas, Augur of Autumn, Sigarda and Intrepid
  Adversary - every one cast into an opponent line showing five to eight untapped sources and a
  seven-card hand. The ONE turn their line showed two untapped permanents, the seat cast a 6/6 and
  it resolved. The seat led 34 life to 11 on turn 19 and lost at -11 on turn 65.
```

---

## 152-C (NEW, cross-deck) - the HOLD-priority row

**Finding.** deck152 seats: **358 HOLD renders / 16 takes**; of those, **113 windows carried
`[you declined this exact list N times already this turn]` at N>=2 and 0 were taken**, 85 of them in
`152v125` (turns 41-65, the same short menu re-asked eight to ten times a turn while the board did
not move). Inserted immediately above `DECIDING SITUATIONS (recognize -> do):`, text as in the
revised guide.
Classification: **STRATEGY** (row and trigger both render; the guide never mentioned either).

---

## NOT CHANGED, and why

- **P4 (152-A of wave 54, the `(you kill it, your attacker lives)` hold)** - only **2 partial
  attack windows** at deck152 seats this corpus and neither is that shape (`152v130` s34 is 152-A
  above; `152v126` s36 held two creatures whose every listed blocker printed a
  `(blocking trigger: they may gain 2 ...)` clause, which is the rule firing as written). Rule
  untouched.
- **P5 (mulligan)** - **PASS**. deck152's one mulligan this corpus (`152v146` s1) was a 1-land hand
  under the COUNT family, `(1 land is not enough for your cheapest spell at mana value 3)`; the
  reply named no colour. The COLOUR family did not render at any deck152 seat. Nothing to fix.
- **P6 (menace blocks / `all_assignments_illegal`)** - `all_assignments_illegal` **0 of 3** blocker
  windows at deck152 seats, and **0 menace attackers anywhere in the corpus's 21 blocker windows**.
  The affordance sentence is untouched and remains UNTESTED.
- **The boundary pass's "read N off the header" re-key** - **62 of 62** attack windows at my seats
  printed `They have N untapped creature(s) able to block`. The re-key's precondition holds; no
  safety valve needed.
- **The Sanguine Bond / Exquisite Blood loop block** - `152v126` (L -2/24) is a loop kill; the guide
  already says the deck cannot answer it and prescribes the race. The seat's t13 swing was 8 power
  into a 12-life opponent, so no lethal line existed. No edit earned.
