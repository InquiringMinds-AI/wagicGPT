# Wave 73 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260907-225637-final/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games; the
125v146 game is one of the reruns — treated as an ordinary corpus game). Seat census: **266 records,
248 model decisions** (ask 187, priority 30, attackers 23, discard 4, reveal 3, blockers 1),
**0 fallbacks**, 0 `reply_truncated`, 0 `action_before_plan`, 0 `menu_pass_no_progress`.
Reasoning present on every decision. `plan_line_missing` 29 of 248 (11.7 %; wave 72: 7.7 %),
`off_protocol_bytes>0` 26. Latency p50 81.9 s / p95 163.7 s / max 358.6 s. Gameend sums for this
seat: `own_turn_windows_skipped` 107, `main_phase_windows_skipped` 59, `hold_windows_skipped` 15
(all cast, 0 priority), `hold_released_turn` 2, `ask_replays_reserved` 14 / refused 0,
`async_drops` 6, `reserve_decline_windows_noted` 3 / skipped 0, `sibling_window_asks_skipped` 0,
`stop_reached_windows_skipped` 0, `chain_windows_collapsed` 0, `repeat_annotated_takes` 0,
`blocker_forecast_rows` 22 (12 multi), `engine_reveal_floor_picks` 0.

| # | Opponent | Result | Turn | Final life | Wave-72 result vs same deck |
|---|---|---|---|---|---|
| 1 | 123 | WIN | 11 | 25 / -4 | WIN |
| 2 | 152 | **LOSS** | 18 | -1 / 11 | WIN |
| 3 | 130 | WIN | 21 | 18 / -3 | WIN |
| 4 | 126 | **WIN** | 19 | 21 / -11 | LOSS |
| 5 | 162 | **LOSS** | 14 | -6 / 20 | WIN |
| 6 | 125 | WIN | 18 | 19 / -3 | WIN |

4-2 (wave 72: 5-1). The 126 loss flipped to a win; two new losses, and **neither is variance** —
both trace to a single decision this guide either did not cover (vs 162) or actively mis-taught
(vs 152). Hands were real: all six mulligans were untouched-7 keeps, no bottom ask ever rendered.

## 0. Did the wave-72 edits pay off, and did the wave-73 engine changes land?

- **Wave-72 HIGH 1 (missing pass row) — FIXED.** All **30** of this seat's priority windows print
  `0. Pass priority`; wave 72 had 36 of 66 missing. Only 4 HOLD rows were taken all wave (deck130
  seq 8, 62; deck125 seq 21, 44) and none of them cost a cast — deck125 seq 21 held at T14 upkeep
  with a PLAN naming main-phase casts, and the Silencer still got cast at seq 22. The wave-72
  hold-as-pass shape is gone from this seat.
- **Wave-72 HIGH 2 (reveal rows without a mana cost) — FIXED.** All three reveal windows now print
  `[cost: {2}{w} (mana value 3)]` on every row (deck152 seq 13, deck130 seq 21, deck123 seq 16).
- **Wave-72 MED 3 (truncated worked example) — FIXED.** No "typo in the prompt" reasoning anywhere
  in the seat. The residue is cosmetic: deck130 seq 14's example is
  `"CHOICE: 1 (Brightclimb Pathway (menu text: Play Land))"`, which still copies an annotation the
  same sentence says to copy nothing from — the model called it "slightly contradictory" and moved on.
- **Wave-72 MED 4 (MDFC back face unlinked) — FIXED.** deck162 seq 4 and deck130 seq 30 both print
  `it is the OTHER FACE of "Brightclimb Pathway" in your hand - one card with two land faces`.
- **Wave-73 hold release — WORKS here.** deck130 seq 62 takes HOLD on the OPPONENT's turn 20
  (combat begins); the next decision is a fresh window at T21 upkeep (seq 63). Both of this seat's
  two `hold_released_turn` events are that game; no hold outlived its holder's untap.
- **Wave-72 Edit 1 (Silencer = highest remaining-copy N) — PAID, 6 for 6.** Every name taken had
  `{copies not yet in a public zone: 4 of theirs}`: deck152 seq 9 Elite Spellbinder, seq 25 Sigarda,
  deck130 seq 49 Lay Waste, deck126 seq 6 Idyllic Tutor, seq 20 Tribute to Hunger, deck125 seq 50
  Supreme Verdict.
- **Wave-72 Edit 2 (no Verse on a 0-power/[defender] body) — PAID.** Verses went to Bloodline Keeper
  (3/3 flier, deck123) and **Ranger Class** (deck152 seq 15/21) — the mono-colour enchantment case
  the edit was written for. No Verse was spent on a wall this wave despite deck126 fielding three.
- **The Sanguine Bond rule — PAID and won the rematch.** deck126 seq 18 Kaya -3 on Sanguine Bond,
  seq 28 Kaya -3 on Exquisite Blood: both halves exiled before the loop could assemble. That is
  the wave-72 loss reversed.
- **Wave-72 Edit 3 (reveal = the card that gets CAST) — UNTESTED.** No reveal list this wave
  offered a sweeper or a counterspell against a creature.
- **`sibling_window_asks_skipped` / `stop_reached_windows_skipped` / `chain_windows_collapsed` are
  all 0 across my six games** — I did not read the code to say which cause applies; engine seat.

## 1. Game by game — the deciding decisions

**vs 162 (LOSS, T14) — the whole game is ONE land drop, `deck162.jsonl` seq 4 (T1).**
Opening hand: 2 Silencers {B}{W}, Triumphant Adventurer {B}{W}, Nadaar {2}{W}, Lolth {3}{B}{B},
Plains, Brightclimb Pathway. Two white sources, five spells, **four of them needing {B}**. The
casting menu at seq 4 offered both faces of the pathway:
```
1. Grimclimb Pathway [PLAY THIS AS A LAND: ... as a land (taps for {B}) ... it is the OTHER FACE of
   "Brightclimb Pathway" in your hand ...] {it enters UNTAPPED - it makes mana this turn}
2. Brightclimb Pathway (menu text: Play Land) [... (taps for {W}) ...]
```
The model took row 2, the WHITE face, and its whole reasoning for the choice is
*"Both give {W}... I'll just pick 1... Actually, I'll go with Brightclimb"* — it never weighed the
colour. With Grimclimb + Plains the seat casts a 3/2 Silencer or the Adventurer on turn 3. With
Brightclimb + Plains it casts **nothing at all**: it drew no further land through turn 13, and the
log runs Turn 3 land drop -> T7/T9/T11 cleanup discards (seq 6/7/8) -> first spell on turn 13. Life
went 20 -> -6 to a Fate Unraveler and a Master of the Feast it could have answered on curve. The
surface was correct and complete (the wave-73 back-face link is right there), so this is a **STRATEGY
item**: the guide has a paragraph on Emeria/Agadeem/Pelakka land-backs and **nothing at all on a
two-land-face pathway**. Guide Edit 3. The model gets it right when the plan happens to mention the
colour (deck130 seq 30: *"Play Grimclimb Pathway to secure black mana for Lolth next turn"*), which
is exactly the tell that a rule, not a hint, is what is missing.

**vs 152 (LOSS, T18) — `deck152.jsonl` seq 29 (T17), and the guide caused it.**
At 5 life against a `CRACK-BACK NEXT TURN: ... for up to 13 - you would be at -8; that would KILL
you`, with **7 untapped sources** and Silencer (3/2) on board, the menu was:
```
1. Cast Nadaar, Selfless Paladin {2}{w} (3/3)   {leaves 4 of your 7 untapped mana sources untapped}
2. Cast Lolth, Spider Queen {5}{b}{b} [from exile] ... {card text: ... -3: Create two 2/1 black
   Spider creature tokens with menace and reach. ...} {taps you out}
```
It took Nadaar, attacked with the Silencer (seq 31), and at seq 32 the block forecast read
`no block saves you`. **Lolth was the line that survives**: she costs exactly the 7 available, she
enters at 4 loyalty, and -3 the same turn makes two 2/1 **reach** blockers. The next turn's actual
attack (seq 32) was Elite Spellbinder 6/4 flying + Moonrage Brute 6/6 first strike for 12; a Spider
blocks the flier (reach), a Spider or the untapped Silencer blocks the Brute, nothing gets through,
and each dead Spider puts a loyalty counter back on Lolth. The reasoning says why it did not:
> *"I can't activate her -3 because she just entered. I need... Wait, can I activate Lolth? No, she
> just entered."* … *"she enters tapped. I can't activate her -3 because she has 0 loyalty and
> entering doesn't give counters. Even if I could, it would give two 2/1 spiders. They can't block
> first strike or flying. Useless."*
Four false beliefs, three of them contradicted by the row it was reading: she enters at 4 loyalty
(`auto=counter(0/0,4,Loyalty)`, planeswalkers.txt:2166), planeswalkers never enter tapped, the
printed row says the Spiders have **reach**, and a loyalty ability is legal the turn a walker
arrives — in real MTG (CR 606.3) **and in this engine**: `MTGAbility.cpp:9227` only gates on
summoning sickness when the ability has a tap cost, and this seat itself proves it live at
**deck126 seq 11 -> seq 12** (Kaya cast on T9, `+1: target creature gets ghostform` used on T9).
The guide's cast-order entry 2 reads **"Cast her NOW; activate from next turn."** That sentence is
false, it is the only place the model could have learned it, and it quoted the guide verbatim while
reasoning about Lolth in another game (deck125 seq 13). Guide Edits 1 and 2.

Two smaller items in the same game. **seq 26 (T15 main 1)** declined the land drop
(`PLAN: Pass this turn. Next turn, play a land`) because the Snarl would enter tapped, then played
it in main 2 at seq 28 — recovered, but the guide's "Play a land every turn" needs the tapped case
named (Edit 4). **seq 13 (T7 reveal)** took Teferi (MV 4) over two Elite Spellbinders and a Brutal
Cathar; the two Spellbinders and the Cathar (as Moonrage Brute) are what killed the seat, but with
two copies of the Spellbinder in the revealed hand no single pick fixes that. Not a misplay.

**vs 125 (WIN T18).** Creatureless permission again; the Hive was the clock, and the false-sacrifice
window (HIGH 1 below) landed here and cost nothing only by luck. Notable good play: **seq 50** named
Supreme Verdict on the Silencer, and **seq 34/53** animated the Hive at the affordability the guide
sets.

**vs 126 (WIN T19, wave 72's loss).** Kaya answered both combo halves on sight (seq 18, 28). At
**seq 13** the guide's lifegain-blocker rule fired correctly: three walls printing
`(blocking trigger: they may gain 2)` / `(they gain 3 and may gain 2 more)` -> `ATTACK: none`.

**vs 123 (WIN T11), vs 130 (WIN T21).** Clean. deck130 is the one game where a planeswalker lived:
Lolth cast at seq 32 (T15), -3 at seq 53 (T19), +0 at seq 42 — four turns of loyalty left on the
table under the "activate from next turn" reading, though the game was already won.

## 2. Engine / interface / card items

### HIGH 1 — Silverquill Command's `+3/+3` TARGET picker is rendered as a FORCED SACRIFICE OF YOUR OWN CREATURE
`...vs-ai_baka_deck125.jsonl` **seq 60** (T18). At seq 59 the model chose mode row 2,
`creature gains 3/3 and you draw` — a pair with **no sacrifice half at all** (the sacrifice rows,
3/4/5, were separately tagged `{this mode's SACRIFICE half is an EDICT: they control 0 creatures -
at 0 this does nothing}` and were not taken). The very next window opens:
```
FORCED SACRIFICE OF ONE OF YOUR OWN CREATURES: your own effect (Silverquill Command) makes you
sacrifice a creature YOU control. Every row below is a creature you will LOSE off the battlefield -
it is NOT a target you attack or affect, and there is no decline: one of them dies whichever row
you pick. Pick the creature you can best AFFORD TO LOSE (usually your least useful body) ...
1. Nadaar, Selfless Paladin (3/3) [vigilance] [your battlefield] ... [you SACRIFICE this]
2. Hive of the Eye Tyrant (4/4) [menace] [your battlefield] [tapped] ... [you SACRIFICE this]
3. Barrowin of Clan Undurr (4/4) [your battlefield] ... [you SACRIFICE this]
4. Silverquill Silencer (4/3) [your battlefield] ... [you SACRIFICE this]
```
This window is the **target picker for the +3/+3** — the log at seq 61 reads
`You targeted Hive of the Eye Tyrant (4/4) with Silverquill Command's ability (creature gains 3/3
and you draw)`, the Hive is a 7/7 flier on the seq-61 board, and **nothing was sacrificed**. Every
sentence of that header is false about this window, and the instruction it gives is exactly
inverted: it told the model to pick its WORST body, for a row that BUFFS what it picks. The model
saw it and could not resolve it — *"Silverquill Command does NOT have a sacrifice mode... Why does
the prompt say 'your own effect makes you sacrifice a creature YOU control'? ... I will just follow
the prompt's instruction to pick the least useful one."* It picked the tapped Hive; the pump made a
7/7 flier that won the game, purely by accident. **The correct render exists** — the same card, the
same wave, a different mode pair: `...vs-ai_baka_deck126.jsonl` **seq 27** prints
`TARGET CHOICE for Silverquill Command - its "creature gains 3/3 and sacrifice creature" ability ...
a gain/pump lands ON the creature you pick, so picking an enemy creature HELPS the enemy`, and lists
the opponent's creatures too. So the classifier picks the forced-sacrifice branch for the
`creature gains 3/3 and you draw` pair and the target branch for `creature gains 3/3 and sacrifice
creature`. Repro: deck125 seq 59 -> 60 -> 61 against deck126 seq 25 -> 26 -> 27.
Fix: classify by the ability that is asking, not by the card name / mode-list membership; a row
tagged `[you SACRIFICE this]` must never appear on a picker whose effect is a pump.

### HIGH 2 — the land-drop seam and the casting seam disagree about which pathway faces exist
`...vs-ai_baka_deck162.jsonl` **seq 3** (T1). The dedicated `Land drop:` question offers
`1. Play Brightclimb Pathway / 2. Play Plains / 3. Play no land right now` — **the {B} face is not
on it**. The model answered `CHOICE: 1 (Play Brightclimb Pathway)`, and only then, at **seq 4**, was
the full two-face menu put to it (Grimclimb row 1, Brightclimb row 2). The land drop was therefore
decided twice, the first time on a menu that hid half the card, and the seq-3 answer carried into
seq 4 as an anchor the reasoning never revisited (*"I'll stick with 1... Actually, I'll go with
Brightclimb"*). Same double-ask at deck152 seq 17 -> 18, deck126 seq 23 -> 24 and seq 31 -> 32,
deck130 seq 29 -> 30. Fix: print both faces on the `Land drop:` menu itself (it already carries the
paragraph promising the drop is its own decision), or suppress the land-drop seam entirely for a
hand holding only a two-faced land.

### MED 3 — `[hold check: ...]` and the pass row now coexist, but the hold text still runs ~450 bytes on every casting menu
deck152 seq 29 rows 3 and 4 spend more than a third of the menu's bytes explaining what HOLD covers,
on a window where the model had already decided to act. With `0. Pass priority` now unconditional
and only 4 holds taken in 248 decisions, the block has outlived its size. Low-risk shrink; no
decision in this seat was lost to it.

### MED 4 — the worked example still copies annotations the same sentence forbids
`...vs-ai_baka_deck130.jsonl` **seq 14**: `e.g. "CHOICE: 1 (Brightclimb Pathway (menu text: Play
Land))"` under `its SHORT NAME in parentheses (the name only - copy nothing from the {...}
annotations)`. Reasoning: *"This is slightly contradictory but I'll follow the exact example
string."* The truncation defect is gone; the annotation-copying one is not. Strip a trailing
` (menu text: ...)` from the example the same way `{...}` is stripped.

### LOW 5 — `plan_line_missing` is up (7.7 % -> 11.7 %) and the shape is unchanged
23 of 29 are a plan sentence with the literal `PLAN:` label dropped (deck123 seq 24 `Create a body
to pressure and block next turn.`, deck125 seq 61, deck130 seq 30); 6 are action-line only. All
parsed, no fallback. The wave-71/72 `YOUR PLAN:` header collision is at **zero** this wave.

### LOW 6 — `main_phase_windows_skipped` 59 for this seat is plausible, not an over-count
59 skips across ~50 of my own turns, i.e. ~100 main phases, with 107 `own_turn_windows_skipped`
concentrated in the two games where the seat could cast nothing (vs 162 alone: 38 own-turn skips
and **0** main-phase skips — the meter did not fire once in the game where the seat spent eleven
turns doing nothing, which is worth the engine seat's eye as a possible under-count, not an over-count).

## 3. Guide verdict: EDIT

Live guide 19,859 bytes -> revised **19,939 bytes** (`wave73/deck146/strategy.txt`). Four edits add
rules; seven compressions pay for them (net +80 bytes).

**Edit 1 — CAST-ORDER ENTRY 2: DELETE THE FALSE RULE "activate from next turn" (the rule that earned
its bytes).**
- before: ` 2. LOLTH or KAYA if listed. Cast her NOW; activate from next turn.`
- after: ` 2. LOLTH or KAYA if listed. Cast her NOW, AND USE A LOYALTY ABILITY THE SAME TURN: a
  walker is never summoning sick and never enters tapped - she arrives at full loyalty and one
  ability is legal at once, on your own main phase. That ability is asked at its own window after
  the cast.`
- paid for by **deck152 seq 29** (the losing decision, whose reasoning is four false beliefs about
  a freshly cast walker), by **deck126 seq 11 -> seq 12** (Kaya cast and +1 activated on the same
  turn 9 — the engine allows it), and by `MTGAbility.cpp:9227` (the sickness gate applies only to
  tap-cost abilities) and `planeswalkers.txt:2166` (`counter(0/0,4,Loyalty)`). The old sentence was
  the guide teaching a rules error; deleting it is the whole point of the edit and the added bytes
  are the correction, not decoration.

**Edit 2 — THE LOLTH BULLET: the Spiders are BLOCKERS, and she can make them the turn she lands.**
- before: `- LOLTH: -3 first (two 2/1 Spiders, menace and reach). LOLTH'S 0 HAS A LIFE FLOOR OF 6 ...`
- after: `- LOLTH: -3 first, THE TURN SHE ARRIVES whenever a crack-back line is aimed at you. Her
  two 2/1 Spiders have REACH - they BLOCK A FLIER - and menace; they are two blockers this deck
  cannot buy any other way, and every creature of yours that dies puts a loyalty counter back on
  her, so chump-blocking with them refuels her. LOLTH'S 0 HAS A LIFE FLOOR OF 6 ...` plus
  `PLANESWALKERS - USE THEM THE TURN THEY LAND AND EVERY TURN AFTER ... "she only just came down"
  is never a reason to wait.`
- paid for by **deck152 seq 29** (*"they can't block first strike or flying. Useless."* — reach was
  printed on the row it was reading) and **deck130 seq 32 -> seq 53** (Lolth landed T15, first -3 on
  T19: four idle turns). The old text listed "menace and reach" as flavour inside a parenthesis; the
  seat needed to be told what reach BUYS.

**Edit 3 — A PATHWAY IS ONE CARD WITH TWO LAND FACES: TAKE THE COLOUR THE HAND IS SHORT OF (the rule
that earned its bytes).**
- added to LANDS AND MANA: `- A PATHWAY IS ONE CARD WITH TWO LAND FACES (Brightclimb taps {W},
  Grimclimb taps {B}) offered as two rows on the same menu, and both enter untapped. TAKE THE FACE
  THAT MAKES THE COLOUR YOUR HAND IS SHORT OF, not the one named in your carried plan: read the
  "[cannot pay now: ...]" brackets and "Mana sources among those lands". With no black source in
  play and a {b} spell in hand, the BLACK face is the only right row, and the white face is how this
  deck loses to its own mana.`
- paid for by **deck162 seq 4** (the game, in one row) and by **deck130 seq 30**, where the model
  took the black face for exactly this reason with no rule to lean on. The guide's LANDS section
  covered Emeria/Agadeem/Pelakka land-backs at length and said nothing about the deck's four
  pathways.

**Edit 4 — "Play a land every turn" now names the tapped case.**
- before: `Play a land every turn.` after: `PLAY A LAND EVERY TURN: a land that enters TAPPED still
  goes down, because the drop does not carry over to the next turn.`
- paid for by **deck152 seq 26** (declined the drop for a Snarl entering tapped, recovered in main 2).

**Edit 5 — the bytes.** CUT the DRAW-PRICE BRACKET block entirely: `[DRAW PRICE:` has rendered
**0 times** in this seat across waves 71, 72 and 73 (~330 bytes). Compressed, with no rule removed:
the BOTTOM ASK (still 0 renders in four waves), CLEANUP DISCARD (4 windows this wave — its five-step
scan is kept intact), the ACERERAK bullet, the room-branch line, the counter/lifegain block, the
mulligan header and count-reason quotations, the 3-life menu, the hand-header double-count note, the
Hive CHECK 2 quotations, and the legend-rule trailing clause. The `{visible now: ...}` sentence in
the Silencer bullet was kept — that tag still rendered 28 times this wave.

## 4. What I did NOT check
- Opponent seat files beyond board context; no audit of any opponent deck's play.
- The 6 `async_drops` (3 in vs 126, 3 in vs 125) — not traced to a seam.
- The 26 `off_protocol_bytes>0` replies — I classified `plan_line_missing` but did not read the
  off-protocol bytes themselves.
- The 3 `reserve_decline_windows_noted` in vs 125 and the 14 `ask_replays_reserved` — engine seat.
- BOTTOM ASK: **zero** windows for a fourth wave (all six hands were untouched-7 keeps, none went to
  a bottom). BLOCK LADDER: **one** blocker decision in six games (deck152 seq 32, a forced-lethal
  window) — still effectively untested. `[DRAW PRICE:` and `[repeat: activated ...]`: 0 renders.
- Whether the `main_phase_windows_skipped` meter's **0** in the vs-162 game is correct behaviour
  (the seat had 38 own-turn skips and made no main-phase play for eleven turns) — engine seat.
- Corpus-wide census, the lane BY/BZ/CA/CB adjudications, the 375 corpus-wide
  `main_phase_windows_skipped`, and the `chain_windows_collapsed` / `sibling_window_asks_skipped`
  zeroes — engine seat.
- I did not build a test for whether HIGH 1's inverted picker has cost a game elsewhere; here it
  produced the right answer for the wrong reason.

No `general-proposals.md` and no `skill-proposals.md`. Edits 1 and 3 are
wave68/strategy-writing-skill-v2.md's existing discipline applied — one is a false rules claim the
guide itself introduced (the skill already forbids stating a rule the engine does not implement),
the other is a surface the guide simply never named; neither is a gap in the method the skill teaches.
