# deck146 (Orzhov Dungeons) -- wave-28 findings (STEP-0 GUIDELESS DEBUT)

Corpus: matchups-20260726-050449, binary ad8930961, -T 3000, REPPENALTY=1.05.
Record: **3/6** -- all three WINS are cap-adjudications (136 adj1, 134 adj1, 137 adj1);
all three LOSSES are clean kills (93 t10 -6, 18 t8 -7, 22 t10 0). No timeouts/draws at
this seat. Own-seat decision volume: 136=42, 134=50, 137=32, 22=14, 18=7, 93=6.

Deck = Orzhov (WB) midrange: flexible removal (Vanishing Verse, Soul Shatter, Silverquill
Command, Pelakka Predation, Kaya -3) + venture creatures (Triumphant Adventurer, Nadaar,
Acererak, Barrowin) + two planeswalkers (Lolth, Kaya x2) + MDFC flood-insurance lands
(Brightclimb Pathway, Emeria's Call, Agadeem's Awakening) + dungeon completion payoffs.
Scryfall-verified all card facts below.

---

## CAMPAIGN-FIRST SURFACE 1 -- VENTURE / DUNGEON

### Render faces (three distinct shapes; two clean, one has a real gap)

**(A) Dungeon SELECTION (first venture)** -- renders as a chooseTarget framed as targeting
the venture source. deck134 seq25 (Nadaar on stack):
```
TARGET CHOICE for Nadaar, Selfless Paladin ... Pick the ONE target it will affect ...
1. Tomb of Annihilation [your zone] - "Trapped Entry - Each player loses 1 life. -- Veils
   of Fear - Each player loses 2 life unless they discard a..."
2. Lost Mine of Phandelver [your zone] - "Cave Entrance - Scry 1. -- Goblin Lair - Create
   a 1/1 red Goblin creature token. -- Mine Tunnels - Create a..."
3. Dungeon of the Mad Mage [your zone] - "Yawning Portal - You gain 1 life. -- Dungeon
   Level - Scry 1. -- Goblin Bazaar - Create a Treasure token. --..."
```
Model understood it as a dungeon choice (correct). TWO render gaps:
  * **Room text TRUNCATED at ~1-2 rooms per dungeon ("...").** Model cannot see deeper/
    completion rooms (the payoff), so it picks on partial info -- cannot see Lost Mine
    completes in 3 rooms vs Tomb's 4 / Mad Mage's 8, nor the completion rewards.
  * **Framing "TARGET CHOICE ... pick the ONE target it will affect" is off** for a dungeon
    SELECTION (choosing which dungeon to venture into, not targeting a permanent). Coped.

**(B) Room BRANCH advancement** -- deck134 seq36 (advancing within Tomb):
```
A choice is required - choose an option:
1. veils of fear
2. oubliette
```
**GAP: bare room names, ZERO effect text.** This is the actual strategic venture decision
(which path through the dungeon) and it carries no rules text -- the model must recall from
memory that veils-of-fear = each player loses 2 or discards, oubliette = discard + sac
creature/artifact/land. The SELECTION ask (A) shows room text (truncated); the BRANCH ask (B)
shows none. Inconsistent, and B is where the value decision lives. Model picked veils of fear
(reasoned it pressures the empty-board opponent) -- defensible but blind on oubliette.

**(C) Room EFFECT modal** -- deck134 seq38, Veils-of-Fear resolving on the venturer:
```
1. lose 2 life
2. discard a card
```
Clean -- effect renders AS option text. Model chose discard (correct at healthy life).

### Navigation verdict
Sane where it can see the effect (A, C); value-blind by construction at the branch face (B).
Understands venture mechanics (seq25: "no rooms entered yet, choose a dungeon to enter the
first room"). BUT **defaulted to Tomb of Annihilation (option 1) in BOTH games where it chose**
(deck134 seq25, deck22 seq11) -- option-1 bias + deck-file ordering (Tomb first) steers it to
the SLOWEST-to-complete dungeon, usually wrong for a deck whose payoffs (Nadaar team +1/+1
anthem, Barrowin reanimate) need a COMPLETED dungeon. Lost Mine (option 2) completes in 3 rooms
and makes a Goblin blocker + Treasure + a pump/shrink en route. -> guide teach.

### Completion (ledger item 'dungeon completion 309.5b')
**NOT exercised live this corpus.** Max depth = Tomb room 2 (Veils of Fear, deck134). No
dungeon completed in any game, so completion + its anthem/reanimate payoffs never triggered.
Cannot confirm the 309.5b defect bites, cannot clear it. Completion IS load-bearing here
(Nadaar/Barrowin), so it warrants a probe (stack Triumphant Adventurer + Nadaar + Lost Mine,
venture 3x, confirm the dungeon completes, leaves, and Nadaar's anthem turns on). -> notes.md.
Venture progress tracking WORKS: engine puts a counter on the dungeon ("Counter added to Tomb
of Annihilation") and moves "zone -> zone" -- venture count is tracked.

---

## CAMPAIGN-FIRST SURFACE 2 -- LOYALTY (Lolth exercised; Kaya UNEXERCISED)

### Lolth, Spider Queen -- menu renders WELL, navigated reasonably
deck134 seq21:
```
1. -3: create spiders with Lolth, Spider Queen [cost: Counters] {card text: "... 0: You
   draw a card and you lose 1 life. -3: Create two 2/1 black Spider ... -8: ... emblem ..."}
2. +0: draw card and lose life with Lolth, Spider Queen [cost: Counters] {card text: ...}
```
Carries +/- prefix, short gloss, source name, "[cost: Counters]", full card text. Legible.
Decision quality: activated -3 FIRST (seq21, correct -- two 2/1 menace/reach while ahead), +0
later (seq43). BUT **passed on Lolth entirely on turns 15 and 19** (seq26/28/30 offered +0,
chose pass; seq48 offered +0, chose pass). Leaving a walker idle for whole turns is a mild
leak -- the +0 (draw, lose 1) is near-free advantage at healthy life. Priority order (-3
before +0) correct; misses were passing free +0. -> mild guide nudge, not a headline.

### Kaya the Inexorable -- UNEXERCISED
Drawn to hand in 3 games (134/93/22, "library -> hand") but **NEVER CAST** (no "stack ->
battlefield"). In 93/22 dead before 6 mana; in 134 held at game end. Her loyalty menu **never
fired** -- render + +1/-3/-7 discipline UNVALIDATED this corpus. Lolth confirms the menu shape,
so Kaya will likely render the same, but her specific decision quality is untested -- carry as
unexercised (do not certify clean). Guide teaches her abilities from Oracle (Scryfall): +1
ghostform (protect a creature), **-3 exile target nonland permanent = premium removal**, -7
legendary-recast emblem. Starting loyalty 5 -> survives at 2 after -3.

---

## STEP-0 READ

### THE #1 LEAK is an ENGINE WINDOW GAP, not a decision (drives both blowout losses)
In the deck18 (t8,-7) and deck93 (t10,-6) losses the model **cast NOTHING all game -- it got
ONLY land-drop asks, never one cast or priority window.** deck18 seq3 (own Main phase 1, turn
3, life 19):
```
Mana available: {b} from 1 untapped source
Your hand: Triumphant Adventurer {b}{w} (1/1); Hive...; Swamp...; Vanishing Verse [instant] x4
YOUR PLAN: Play Shineshadow Snarl, then cast Triumphant Adventurer ...
Land drop: which land do you play now, if any?
1. Play Hive of the Eye Tyrant   2. Play Swamp   3. Play no land right now
```
The model HELD Triumphant Adventurer (a {B}{W} deathtouch/first-strike blocker, castable from
turn 2 once a 2nd land is down) and 3-4 Vanishing Verse (instant, exiles ANY monocolored
permanent -- deck18's board was ALL mono-white creatures = every one a legal target) while
being raced 20 -> 19 -> 14 -> 6 -> dead. Its own PLAN wanted to cast Adventurer. **It was never
asked.** deck18 turns 3/5/7 all land-only; deck93 turns 1/3/5 land-only. Zero `priority`
records, zero cast `ask` records in either file.

Mechanism hypothesis (NEEDS BINARY REPRO before any fix): the land-drop ask fires before the
2nd land is down (legalCasts empty -> land-only menu); after the land resolves, FindCardToPlay/
computeActions is apparently NOT re-invoked to offer now-affordable casts in the same main
phase, and the turn advances. Contrast: deck134 (50 records) / deck136 (42 records) surface
many cast asks -- starvation correlates with being on the back foot / short games. Single
largest STEP-0 result, unfixable by guide prose (a model never asked cannot be taught to cast).
-> notes.md headline. Per method-headline-1 corollary I am NOT certifying "plays passively":
the record shows it WANTED to cast and had no window.

### Teachable leaks (decisions it DOES face)
1. Dungeon default to Tomb (option 1) x2 -- should pick Lost Mine to complete fast.
2. Loyalty passivity -- passed free Lolth +0 on 2 turns (seq26/28/30/48).
3. Removal hoarding (where windows exist) -- treats Vanishing Verse as "hold for a threat";
   vs aggro every attacker is a legal target, holding = dying with removal in hand. Guide must
   frame Verse / Kaya-3 as proactive on-curve removal, not counterspell-style hold.

### What it does WELL
- Correctly DECLINES dead removal: Soul Shatter is an edict needing an opposing creature/pw;
  declined every empty-board turn (deck134 seq11/15/32) with clear reasoning. No misfire.
- Understands venture mechanics (seq25 rules-correct).
- Loyalty PRIORITY (impactful -3 before +0) correct.
- Silverquill Silencer ETB naming works (deck22 seq7 named "Blind-Spot Giant").
- All 3 cap-adjudicated WINS (134/136/137) were grindy value games where it out-carded the
  opponent -- the midrange plan functions when not raced off the table.

---

## THE 1 DEFER (retracted_choice, deck134 seq40) -- decomposed
NOT convoke, NOT a hard fallback. Mechanism = **stale-answer drop (menu drift)**: model replied
"CHOICE: 3 (Cast nothing right now)" to a 3-option ask, but the option set changed between ask
and reply (a trigger resolved), so index 3 no longer mapped -> retracted (choice=-1,
chosen=None), re-asked. Self-healed at seq41 (resolved cleanly, "creature gains 3/3 and you
draw"). Sanctioned ~2% self-healing drift path. The reply reasoning was rambling (second-
guessing life totals) but the MECHANISM is clean drift-retract. 1 retracted / 149 own-seat
decisions. No convoke in this deck.

---

## e4 LAND-TAG CHECK (two-color seat) -- clean, one secondary note
Deck runs NO shocklands / Godless Shrine, so the e4 shockland-ETB (c2) annotation question is
**N/A at this seat**. Actual lands render cleanly:
```
Plains (land: taps for {W})              Swamp (land: taps for {B})
Shineshadow Snarl (land: taps for {W}{B})    [WB dual, both colors shown]
Brightclimb Pathway (land: taps for {W})     [MDFC, its W land-face]
Hive of the Eye Tyrant (land: taps for {B})  [creature-land as land]
```
No prompt-noise from the tags. TWO secondary notes:
  * Shineshadow Snarl's ETB condition ("enters tapped unless you reveal a Plains/Swamp") is not
    surfaced in the tag -- engine auto-handles it, but the model can't plan around a tapped
    Snarl. Minor (no decision hinged on it observed).
  * **The "Mana available:" SUMMARY collapses Snarl's dual to one color** -- deck93 seq4 shows
    "{b}{b} from 2 untapped sources" when one source is Snarl (taps W OR B). The hand render is
    correct ({W}{B}) but the running mana summary under-reports the second color, which could
    make the model think it can't pay a {W} pip. Secondary render note -> notes.md.
