# Wave 65 - deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/`
Seat files: `*deck162-*-vs-*.jsonl` (6 games). Record: **4-2**. Model `qwen36-35b-a3b`.
0 heuristic fallbacks in any of the six seats; 0 `deadline_pct > 95`; 0 `empty_reply` /
`unparsed_reply` / `transport_error`. Decision counts are low (18-31 per seat) - this deck durdles.

Card facts below are read off `bin/Res/sets/primitives/`: Underworld Dreams, Fate Unraveler and
Ob Nixilis, the Hate-Twisted all carry the identical `auto=@drawfoeof(player):damage:1 opponent`
(so Ob Nixilis is a third punisher **and** removal); Forced Fruition draws **seven**
(`auto=@movedTo(*[-land]|opponentstack):draw:7 opponent`), not four.

---

## 1. Game by game

The corpus separates on ONE variable: **how many punishers reached the battlefield.**

| game | result | punishers landed (seq) | draw engines landed |
|---|---|---|---|
| vs 152 | **W** t15, 4/0 | Ob Nixilis s16, Underworld Dreams s23, Fate Unraveler s28, U.Dreams#2 s29 = 4 | Mine, 2x Dictate, Master |
| vs 125 | **W** t19, 18/0 | U.Dreams s13, Fate Unraveler s17, Ob Nixilis s20, U.Dreams#2 s21, Ob Nixilis#2 s23 = 5 | 2x Mine, Dictate, Puzzle Box |
| vs 146 | **W** t17, 14/0 | U.Dreams s11, Ob Nixilis s13, Fate Unraveler s16 = 3 | Dictate, Dictate#2, Puzzle Box |
| vs 130 | **W** t15, 6/0 | Ob Nixilis s11, U.Dreams s14, Ob Nixilis#2 s22 = 3 | Mine, Dictate |
| vs 126 | **L** t17, -2/18 | Fate Unraveler s14 (**sacrificed s15**), F.U.#2 s18, U.Dreams s21, Ob Nixilis s25 | Mine |
| vs 123 | **L** t12, **-1089**/1 | Underworld Dreams s7 **= 1, all game** | 3x Mine, 2x Dictate, Master |

Every win ran 3-5 punishers. Both losses ran one (vs123) or none-for-six-turns (vs126).

### vs 123 - the -1089 game (the brief's flag)

**The loop is on the OPPONENT's side and deck 162 never had a window against it.**
`1788683516-ai_baka_deck123-...-vs-...deck162.jsonl`: deck 123 assembled Intruder Alarm
("Whenever a creature enters, untap all creatures") + Bloodline Keeper, flipped to Lord of
Lineage ({T}: create a 2/2 Vampire). Each token entering untaps the Lord, so the {T} cost is
free and the activation is unbounded. The 123 seat rode the engine's repeat row - `Create
vampire with Lord of Lineage, repeated N times, then stop` - at seq 70 ("you control 49
creatures"), 73 (68), 76 (101), 79 (111), 82 (132), 85 (152), 90 (153), 93 (200), 100 (232),
103 (253), 113 (266), 116 (276), 139 (279), 157 (301), 160 (304), **161 (318)**. Final swing:
277 attackers for 1109.

**Loop verdicts: they printed and they were correct, but only the ones the board supported.**
The deck-162 seat's seq 18 blockers prompt carries `INCOMING THIS COMBAT: 277 attackers, 1109
unblocked damage - you would be at -1094; this KILLS you`, a correct 318-vampire battlefield
render, and a correct `DRAW FORECAST (theirs) ... = 6 x 1 = 6 ... they would be at -5; that
KILLS them`. There is **no** `LETHAL COMES FIRST` clause anywhere in any deck-162 seat this
corpus (0 occurrences across all six files) - correctly, since no life LOOP was in play in this
game and `lifeLoopProvenWin` never armed. The A-row range compression handled 276 identical
attackers in one line. The model read all of it correctly and answered `BLOCKS: B1:A1` with an
explicit "I die either way" - **no misplay at seq 18.**

The narration is complete too: seq 18 shows `Create vampire with Lord of Lineage (x106)`,
`(x113)`, `(x11)` inside deck 162's own turn 11, then `(x21)/(x3)/(x16)` on turn 12 - 270 of the
272 tokens made in that span are narrated with counts. Nothing was hidden from the model.

**Where deck 162 actually lost it: seq 11 and seq 14 - mana spent on the 5th and 6th draw
engine instead of the 2nd punisher.**
- seq 11 (turn 9, its own DRAW step): `Mana available: 4 total`, `Land drop: NOT yet used`,
  hand holds `Ob Nixilis, the Hate-Twisted {3}{b}{b} [planeswalker] [no cast row now: sorcery
  speed - only in your own main phase with an empty stack]`. It flash-cast Dictate of Kruphix
  (`{leaves 1 of your 4 untapped mana sources untapped}`). 4 + the land drop = 5 = Ob Nixilis in
  main phase; instead seq 12 opens on `Mana available: 1 total` and seq 13's only cast row is
  Shield Sphere.
- seq 14 (turn 11, DRAW step) repeats it exactly: 5 untapped, Dictate #2 for 3, land drop
  unused, Ob Nixilis still in hand. Seq 16 then reads `Ob Nixilis ... [cannot pay now: needs 5
  mana, you have 3 untapped sources]`.
- The arithmetic it never ran: their draw step costs them `P x (D+1)`. At seq 14 the choice was
  +1 damage/turn (6th engine) against +5 damage/turn (2nd punisher). With Ob Nixilis down at
  turn 11 the forecast reads `5 x 2 = 10` against their **9** life - they die on their turn-12
  draw step, before the 277-attacker declaration ever happens. The game was decided two turns
  before the loop mattered. **STRATEGY** (the model read every surface correctly).

### vs 126 - the other loss

- **seq 15, forced sacrifice (Tribute to Hunger): sacrificed its only punisher.** Rows were
  `1. Shield Sphere (0/5) [defender] ... [you SACRIFICE this; they gain 5 life (its toughness)]`
  and `2. Fate Unraveler (3/4) ... [you SACRIFICE this; they gain 4 life (its toughness)]`. The
  same prompt's own line reads `DRAW PUNISHERS on the battlefield: yours - Fate Unraveler`. It
  took row 2 to save one life, went to zero punishers with a Howling Mine still handing the
  opponent free cards, and had no punisher on board again until seq 18. **STRATEGY, abetted by
  the render** - see HIGH-2 below.
- seq 27 (attackers, at 3 life): the model wrote `ATTACK: A1>W1` on line 1, deliberated inside
  the PLAN line, then wrote a marked `Correction:` and a later `ATTACK: none`. The engine
  recorded `parse_note=attack_correction_line_taken`, `answer_replaced=true`,
  `latched_line='ATTACK: none'`, `coded_answers=2`, and committed **no attackers** - Fate
  Unraveler was still untapped and offered as B2 at seq 28. **Lane AO/AP's correction rule
  worked, and it saved a blocker.**
- seq 28: `BLOCKS: none` at 2 life vs 4 lifelink attackers with Sanguine Bond + Exquisite Blood
  both on their side. The header correctly refused to give a best-case figure. Blocking 2 of 4
  still leaves 2 damage into 2 life - dead either way. **No misplay.**

### Wins (short)

vs 152: Ob Nixilis -2 twice on real threats (s17 Brutal Cathar, s24 Sigarda), `A1>W1` at Teferi
(s18) executed correctly (`parse_note=attack_walker_target`). vs 146: same shape, Ob Nixilis -2
on Barrowin of Clan Undurr (s14), won on the draw step at t17. vs 130: Ob Nixilis -2 on Dwarven
Blastminer (s12) and Siege-Gang Commander (s23) - both "makes more creatures" targets, exactly
the guide's teach. vs 125: five punishers, opponent 0 at t19 with the seat still on 18.
Mulligans were real (vs126 mulliganed to 6 at s2-s4); I did not read variance as guide effect.

---

## 2. Engine / interface / card items

### HIGH-1 - an unbounded token loop can run to 318 permanents with no engine-side bound
`1788683516-ai_baka_deck123-0x55f8cd35b2e0-vs-ai_baka_deck162.jsonl`, seq 70-161. Repeated row:
`Create vampire with Lord of Lineage, repeated N times, then stop [you control 318 creatures
right now; you name N on the ...]`. Intruder Alarm makes the {T} cost self-refunding, so the
repeat row's N has no cost gate and no board-size gate; the AI seat took it sixteen times in one
turn cycle. Consequences visible from the deck-162 seat (`...deck162-...-vs-...deck123.jsonl`
seq 18): a `327 permanents listed` battlefield render, `277 attackers, 1109 unblocked damage`,
and a final life of **-1089**. Nothing here is a *false* surface - the render and the verdicts
are all correct - but the game is decided by an activation the engine never bounds, and every
prompt after it is enormous. Suggested bound to consider (not a design ruling): cap the repeat
row's N, or refuse to re-offer a free-to-activate ability once the board state it produces stops
changing any decision-relevant quantity. This crosses decks - the engine seat should own it.

### HIGH-2 - the forced-sacrifice rows price toughness but not the permanent's ROLE
`1788683514-ai_baka_deck162-0x5644a69432c0-vs-ai_baka_deck126.jsonl` seq 15. Header: *"Pick the
creature you can best AFFORD TO LOSE (usually your least useful body, and - where the rows
differ - the one that pays the least)"*. Rows price only `they gain N life (its toughness)`, so
the tie-break **actively pointed at the punisher**: Fate Unraveler pays 4, the spent 0/5 Shield
Sphere pays 5. The same prompt already computes `DRAW PUNISHERS on the battlefield: yours - Fate
Unraveler` - the engine knows the role and does not put it on the row. Concrete fix in the
existing annotation style: tag the row, e.g. `[you SACRIFICE this; they gain 4 life (its
toughness)] {this is 1 of your 1 draw punishers - losing it takes that count to 0}`, and drop or
qualify the "pays the least" tie-break when the rows differ in role. Lane AN already touched
this seam ("forced-sacrifice tie-break faces whoever is paid"); the *price* is now right and the
*value* is still missing.

### MED-1 - a flash engine offered outside your main phase shows no reservation against your
### sorcery-speed hand
`...deck162-...-vs-...deck123.jsonl` seq 11 and seq 14. In the seat's own DRAW step the Dictate
of Kruphix row prints `{leaves 1 of your 4 untapped mana sources untapped}` and a rich `{feeds:
... draw converters in your hand: 1 - Ob Nixilis, the Hate-Twisted}` clause - so the engine
already knows a punisher is stranded in hand - while the hand line says only `[no cast row now:
sorcery speed - only in your own main phase with an empty stack]`. Nothing states what taking
this row costs the main phase. The whole game turned on that subtraction (4 untapped + 1 unused
land drop = 5 = Ob Nixilis). One clause on the row would decide it, e.g. `{after this you would
hold 1 source + your unused land drop = 2 - not enough for Ob Nixilis {3}{b}{b}, which your hand
line says can only be cast in your main phase}`. This is squarely the loop's UX mandate: the
information exists, it is just not at the decision.

### MED-2 - DRAW FORECAST prints the punisher multiplier but never its margin
Same file seq 18: `their next draw step draws 6 cards (...) = 6 x 1 = 6 life LOST BY THEM`. The
`x 1` is P. Two clauses of arithmetic the engine already has would make the deck's central
trade-off explicit at the exact window where it is decided: `- one more punisher would make this
12; one more draw engine would make it 7`. Cheap, no new state, and it is the fact both losses
turned on.

### LOW-1 - PLAN lines are used as an unbounded scratchpad; most of it is billed and discarded
seq 27 vs126: `reply_trimmed_bytes=796`, `post_answer_overrun=1968`, `post_plan_overrun=1261`.
seq 29 vs152: `+2595 bytes written past the PLAN line trimmed from this record`. The model
routinely writes 1-3 KB of deliberation inside PLAN, of which 400 characters carry. Not a
defect (the carry cap is AP's design and the first-wins/CORRECTION rules handle it correctly)
but it is real inference spend on every decision, and the trimming also costs the reviewer the
model's actual reasoning. Worth an engine-seat measurement across the corpus.

### LOW-2 - `later_answer_ignored` fired twice in this seat, both harmless
vs152 seq 29 (`ask`, coded_answers 2, latched `CHOICE: 2 (Cast Underworld Dreams)` - identical
to the first answer) and vs123 seq 18 (`blockers`, restated `BLOCKS: B1:A1`). In this seat's six
games there is **no** case where the later unmarked line would have been the better answer; the
one case where it would have been (vs126 seq 27) the model marked it `Correction:` and the
engine took it. That is a clean PASS for lane AO/AP from this seat - 1 of 1 corrections honoured,
0 of 2 ignored-later-answers regrettable.

### Not checked
I did not audit the `system` prompt/decklist rendering, the mulligan/bottom seams beyond noting
they fired, a prefix-matched HOLD render/take census (I only noted that
`identical_ask_answers_reserved` was 1 in the vs152 seat and 0 in the other five, and that one
Hold row was taken - deck 123's seat, seq 50), latency/transport statistics, `async_drops`, or
the opponents' seats beyond deck 123's loop and deck 126's board. Any per-lane
prediction adjudication is the engine seat's, not mine.

---

## 3. Guide verdict: **EDIT**

Live guide `bin/Res/ai/baka/deck162_strategy.txt` is 60,745 B. Revised guide written to
`strategy-design/wave65/deck162/strategy.txt`, **65,630 B (64.1 KB)** - inside the 41-71 KB band.
Three edits, each paid for by a seq-cited decision that changed a result. Everything else is
KEPT verbatim: Rule 0/0b/0c, Rule 1, Rule 3/4, the Ob Nixilis -2 teach and the Sanguine
Bond/Exquisite Blood bullet all produced correct play this corpus (Ob Nixilis -2 was used at
vs152 s17/s24, vs146 s14, vs130 s12/s23, vs126 s26 - six times, every one on a legitimate
target, three of them on token-makers exactly as the guide instructs).

**Edit 1 - Rule 2, "K OF 1 OR MORE" bullet.**
before: the only ordering exception was *"if a second punisher ... is ALSO in this ask's list,
cast the punisher FIRST"*.
after: adds (a) the observation that a flash engine's window is the upkeep/draw step, where a
sorcery-speed punisher **can never appear on the menu to compete**, so the rule's own escape
hatch is unreachable there; (b) a printed-numbers subtraction (`Mana available` + unused land
drop - what the row spends, vs. the cheapest punisher in hand) with the instruction to answer
"Cast nothing right now"; (c) the deck's core arithmetic `P x (D+1)` - next punisher is worth
`D+1`, next engine is worth `P`; (d) a WHAT THIS COST citing **vs123 seq 11 and seq 14**.
paid for by: vs123 seq 11, seq 14 (and the seq 16 `[cannot pay now: needs 5 mana]` that followed).

**Edit 2 - new section "FORCED SACRIFICE - THE WALL GOES, NEVER THE PUNISHER"** (before
`=== ATTACKING ===`). before: the guide had no forced-sacrifice guidance at all.
after: fixed order Shield Sphere > Fog Bank > Master of the Feast > Fate Unraveler; a spent
Shield Sphere is the best row on the menu; take the wall *even when it pays them more life*;
cross-check against the prompt's own `DRAW PUNISHERS on the battlefield: yours -` line; WHAT
THIS COST citing **vs126 seq 15**.
paid for by: vs126 seq 15 (and the 6-turn punisher gap to seq 18/21 that followed).

**Edit 3 - two DECIDING SITUATIONS bullets** (flash engine outside main phase vs. a stranded
sorcery-speed punisher; forced sacrifice offering a wall and a punisher), so both new teaches
have a one-line trigger form at the place the model scans last.

No general-strategy or skill proposals from this seat: HIGH-2, MED-1 and MED-2 are engine/render
items for the core loop, not prompt-text items, and nothing here restates or contradicts an
existing skill amendment.
