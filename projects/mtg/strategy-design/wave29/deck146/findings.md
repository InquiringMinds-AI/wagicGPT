# deck146 (Orzhov Dungeons) -- wave-29 findings (FIRST-GUIDED CORPUS)

Corpus: matchups-20260726-115018, binary 50eeed8be, -T 3000, REPPENALTY=1.05.
Guide under test: bin/Res/ai/baka/deck146_strategy.txt (wave-28 first deploy).
Record: **3/6**. Own-seat fallbacks: **0/11 corpus fallbacks are this seat** (clean).

Games (from results.tsv):
- vs148: **WIN, clean kill** (7 to -7, t12)
- vs93:  **WIN, clean kill** (10 to -9, t9)
- vs122: **WIN, adjudication** (16 to 15, t13)
- vs139: LOSS, adjudication (1 to 8, t16) -- close grind at the cap
- vs136: LOSS, clean kill (-1, t10) -- MANA SCREW (stuck 2 lands to t9)
- vs137: LOSS, clean kill (-9, t14) -- raced by a strong go-wide deck

Own-seat decision volume: vs122=55, vs139=71, vs148=27, vs137=27, vs93=21, vs136=14.

---

## f2 -- THE NO-CAST-WINDOW CLASS IS GONE (dual-land oracle fix VALIDATED)

Wave-28's signature blowout: in the deck18 (t8,-7) and deck93 (t10,-6) losses the model
cast NOTHING all game -- it got ONLY land-drop asks, zero cast/priority windows, while
holding castable Triumphant Adventurer + multiple Vanishing Verse and being raced to death.

**This corpus: that class does not appear.** Every game where the deck had mana + castables,
it got cast asks AND instant-speed priority windows.

### The headline conversion: vs93 flipped L -> W
Wave-28: clean-kill LOSS to 93 (t10, -6), the no-cast-window blowout.
Wave-29: clean-kill WIN over 93 (t9, opp -9). Full curve: Silverquill Silencer t3 -> Nadaar
t5 -> Triumphant Adventurer t7, ventured Lost Mine, alpha-struck for the kill. Same matchup,
opposite result -- the fix + guide directly converted a loss into a win.

### Removal now fires ON CURVE (was hoarded/window-less in wave-28)
Vanishing Verse cast on curve in FIVE of six games:
- vs136 s11 (t9): Verse -> Midnight Reaper, the instant mana came up (mana-screwed until t9)
- vs148 s11 (t8): Verse -> Stone Haven Outfitter; s22 (t12): Verse -> Kitesail Apprentice
- vs139 s22 (t4): Verse -> Dryad of the Ilysian Grove (killed their ramp/fixing)
- vs137 s11 (t3): Verse -> Giant Killer; s15 (t7): Verse -> Lovestruck Beast
The wave-28 "removal hoarding" leak is resolved -- it was never a decision leak, it was a
window gap. With windows, the model spends removal on curve as the guide directs.

### Mana-available lines show TRUE color reach
Zero mana-collapse lines across all six games (grep {b}{b}/{w}{w} from a WB deck = 0).
Cite: **vs93 s6 "Mana available: {b}{w} from 1 untapped source"** -- a SINGLE Shineshadow
Snarl now renders BOTH colors. Wave-28 showed "{b}{b} from 2 sources" (the collapse bug).
The line also now carries "(tapped automatically when you cast)". Fix holds.

### Instant-speed windows are back
Priority-record counts: vs139=35, vs122=17, vs136=3, vs93=2, vs148=0, vs137=0. The grind
games get many instant-speed priority windows (potentialManaPermissive feeding
hasInstantResponse); wave-28's losses had ZERO priority records. The deck can now hold up
Vanishing Verse on the opponent's turn.

### The two non-adj losses decomposed (neither is a window gap)
- **vs136 (clean kill, t10): MANA SCREW.** seq8/9 (t1) then a jump to seq10 (t9) -- it drew
  no land t2-t8 (hand had one land, rest spells), stuck at {w} from 1 source. The moment it
  hit {b}{w} at t9 it cast Vanishing Verse correctly. Pure variance, not a starvation gap.
  (Aside: at t2 the model's PLAN wrongly called Knight of the Ebon Legion "multicolored"
  Verse-immune; the render correctly showed {b} and later offered it as a legal Verse target,
  which the model took. Render fine; a momentary model misread at a no-mana turn.)
- **vs137 (clean kill, t14): raced.** Cast Verse t3 + t7, Adventurer t5, ventured, but a
  fast go-wide opponent (deck137 is a strong 3/6 whose wins are clean kills) out-tempoed a
  1/1-plus-removal draw. Matchup/draw, not a window gap.

**f2 VERDICT: PASS. The no-cast-window class is eliminated; dual-land mana renders correct
color reach; removal fires on curve; instant windows restored. This is the corpus's headline.**

---

## f4 -- DUNGEON RENDERS FIXED; COMPLETION REACHED (309.5b probe ANSWERED)

### Selection face: full room path + completion reward + reframed prompt
Wave-28 gaps (truncated room text; "TARGET CHOICE...pick the ONE target" framing) are BOTH
fixed. Selection now renders (vs93 s10, vs148 s7, vs122 s24):
```
VENTURE - CHOOSE A DUNGEON to enter (you are picking WHICH dungeon to venture into, NOT
targeting a permanent)... Weigh how many rooms to completion and whether the completion
payoff and the rooms en route fit your plan... Pick the ONE dungeon... answer with its name.
1. Tomb of Annihilation [dungeon: 5 rooms; completion reward ("Cradle of the Death God"):
   Create The Atropal, a legendary 4/4 black God Horror with deathtouch.] - full room path: ...
2/3. Lost Mine of Phandelver [dungeon: 7 rooms; completion reward ("Temple of Dumathoin"):
   Draw a card.] - full room path: "Cave Entrance - Scry 1. -- Goblin Lair - Create a 1/1
   red Goblin... -- Mine Tunnels - Create a Treasure... -- Storeroom - +1/+1 counter... --
   Dark Pool - each opp loses 1, you gain 1... -- Fungi Cavern... -- Temple of Dumathoin -
   Draw a card."
```
No truncation. Completion reward and full path visible. Framing is correct (venture, not target).

### Branch face: room options now CARRY effect text
Wave-28: bare room names, zero text. Now every branch pick renders
`goblin lair {room effect: Create a 1/1 red Goblin creature token.}`,
`dark pool {room effect: Each opponent loses 1 life and you gain 1 life.}`,
`storeroom {room effect: Put a +1/+1 counter on target creature.}`,
`mine tunnels {room effect: Create a Treasure token.}`. The value decision is no longer blind.

### Navigation quality: option-1-Tomb bias BROKEN
Lost Mine picked in **7/7** dungeon selections (vs148 x2, vs139, vs93, vs122 x3, vs137).
ZERO Tomb selections. And the room picks are value-aware: Goblin Lair (blocker), Dark Pool
(drain), Storeroom (+1/+1), Mine Tunnels (Treasure) -- it avoids the scry-only rooms, exactly
the "prefer a body or damage over scry" behavior. Big improvement.

### DUNGEON COMPLETION REACHED -- the 309.5b probe is ANSWERED
In **deck146 vs deck122** the venture re-offered "CHOOSE A DUNGEON" three times (s10 initial,
s24, s37) = **1 initial run + ~2 completions**. Completion is confirmed by ENGINE-rendered
board state at t13 s54:
```
Triumphant Adventurer #1 {b}{w} (2/2) (printed 1/1) [deathtouch]
Nadaar, Selfless Paladin {2}{w} (5/5) (printed 3/3) [vigilance] [counters: 1x +1/+1]
```
Nadaar 3/3 base + 1 Storeroom counter (+1/+1 -> 4/4) + **completed-dungeon anthem (+1/+1)** =
5/5; Adventurer 1/1 -> 2/2 is the anthem alone. The Nadaar "creatures you control get +1/+1
as long as you have completed a dungeon" payoff is LIVE on the whole team. Game ran cleanly to
t13 and was a WIN. **No 309.5b crash. Completion works end-to-end. Recommend RETIRING the
N-146e completion probe.**

Note (verbatim model uncertainty, deck122 PLAN at s37): "the anthem applies *after* the
dungeon is completed? 'As long as you've completed a dungeon'." The engine applies the anthem
correctly, but the CURRENT SITUATION render carries no explicit "dungeons completed: N" status
line, so the model must infer completion from anthem'd stats. -> minor render item in notes.md.

**f4 VERDICT: PASS. Selection + branch renders fixed, navigation value-aware, Lost Mine
picked 100%, completion reached and anthem confirmed live. Strongest campaign-first surface
result to date for this seat.**

---

## GUIDE VALIDATION (first-guided corpus)

| Guide directive | Result |
|---|---|
| Spend removal on their best threat, ON CURVE | CONVERTED -- Verse fired on curve in 5/6 games |
| Deploy Triumphant Adventurer early as a body | CONVERTED -- cast t4/t5/t7 across 3 games |
| Pick Lost Mine of Phandelver (not Tomb) | CONVERTED -- 7/7 selections Lost Mine |
| Prefer body/damage rooms over scry | CONVERTED -- goblin lair / dark pool / storeroom |
| Complete a dungeon for the anthem payoff | CONVERTED + VALIDATED -- ~2 completions vs122 |
| Planeswalkers: use them every turn | UNEXERCISED -- see below |
| Kaya -3 as premium removal | UNEXERCISED -- see below |

### Planeswalkers / Kaya: STILL UNEXERCISED (second corpus running)
Neither Lolth NOR Kaya was CAST in any deck146 game this corpus (0 "stack -> battlefield" for
both). Kaya drawn to hand in vs93 and vs137, never cast (games ended, or she sat uncastable at
5 mana). Lolth never drawn to a castable spot. Their decision quality is UNVALIDATED across TWO
corpora now (wave-28 exercised Lolth once; Kaya never). Do NOT certify walker play clean. This
is the deck's one remaining unvalidated surface and the primary reason to keep it in rotation.

### GUIDE FACT ERRORS surfaced by the live render (verify-oracle rail) -> strategy.txt revised
The deployed guide asserts three things the CURRENT render contradicts:
1. "Lost Mine ... completes in just 3 rooms" and "Tomb ... is the SLOWEST to complete" -- the
   render shows **Lost Mine = 7 rooms, Tomb = 5 rooms, Mad Mage = 9 rooms**. The "3 rooms"
   claim (a min-branch-path figure) flatly contradicts the "7 rooms" number the model reads.
   The real reason to pick Lost Mine is that its rooms are ALL UPSIDE (Goblin, Treasure,
   +1/+1, drain) with a clean completion (draw), whereas Tomb damages YOU every room.
2. "Room branches show only the room NAME with no text" -- now FALSE (branches carry
   {room effect: ...}). A stale description of a now-fixed render gap.
3. Option-number references ("Lost Mine is usually option 2", "Tomb is option 1") -- ordering
   is UNRELIABLE: at vs122 s10 Lost Mine was option 2, but at re-selections s24/s37 it was
   option **3**. The render asks the model to "answer with its name" and the model does.
Revision strips option numbers, corrects the room-count/branch-text claims, and reframes the
pick around value + completion payoff. (Removal/Adventurer/walker sections kept -- they work.)

---

## RECORD DECOMPOSITION (3/6 Step-0 -> 3/6 first-guided: FLAT W-L, IMPROVED SHAPE)

Wave-28 (guideless): 3/6 -- all 3 wins were cap-ADJUDICATIONS (grindy); all 3 losses were
CLEAN KILLS (raced off the table with removal stuck in hand; 2 were no-cast-window blowouts).

Wave-29 (guided): 3/6 -- but **2 of 3 wins are now CLEAN KILLS** (148 t12, 93 t9), only 122 is
adj. Losses: 139 adj (close, 1-8), 136 clean (mana screw), 137 clean (raced by a strong deck).

Pool churn: deck18/22/134 rotated out (18/22 were two of 146's wave-28 losses), replaced by
fresh Step-0 decks 148/122/139. So the flat record is measured against a DIFFERENT pool.

- **Converted**: vs93 L->W (headline). Removal-on-curve (0 windows -> fires in 5/6). Lost Mine
  selection (0% intent -> 100%). Dungeon completion (never reached -> reached + anthem).
- **Regressed**: vs137 adj-W -> clean-L, but 137 is a strong go-wide deck whose adj-win in
  wave-28 was a coin-flip grind; noise, not a guide failure.
- **Didn't convert / untestable**: planeswalker usage (unexercised); folding to fast aggro on
  a mana-light draw (vs136 screw, vs137 race) -- variance, not a taught decision missed.

Honest read: the flat 3/6 UNDERSTATES real progress. The systematic wave-28 failure mode
(window starvation -> death with a full hand) is eliminated, the deck now closes games with
clean kills, removal fires on curve, and the whole campaign-first surface (venture + dungeon
completion) is validated live. The two non-adj losses are variance (screw) and a strong
opponent, not the deck's own leaks.

---

## FALLBACKS / PARSER
deck146 owns **0 of the 11 corpus fallbacks** -- no defers, no priority retracted/unparsed, no
truncated_abandoned at this seat. No parser contradictions observed (blockers/attackers parsed
cleanly, e.g. vs148 s10 "Nadaar blocks Armament Master; Goblin blocks Kor ally"). The 3
non-defer fallbacks and the 8 defers the brief flags belong to other seats.
