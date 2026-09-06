# Wave-65 per-deck review - deck123 (Intruders of Thraben, WBU token combo)

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/`, 6 games, 3-3.
Seat files are the six `*deck123-*-vs-*.jsonl`. All seq citations are this seat unless named otherwise.
Card facts checked against `projects/mtg/bin/Res/sets/primitives/mtg.txt` (Pride Guardian
`auto=@combat(blocking) source(this):life:3 controller`; Thraben Doomsayer
`auto=this(controllerlife < 6) lord(other creature|mybattlefield) 2/2`; Bloodline Keeper
`{B}:flip(backside) restriction{...vampire...morethan~4}`; Idyllic Tutor
`notaTarget(enchantment|myLibrary)`). Every render this seat leaned on matched the primitive; no
Scryfall disagreement to report.

## 1. Game by game, and the decisions that decided them

**vs deck126 - WIN, turn 10, 20 to -2** (`1788683483-...deck123-...-vs-ai_baka_deck126`).
Curve was clean (Doomsayer T6, Alarm T8). Two decisions mattered and both went against the guide:
- **seq 12 (T8 attackers)**: `ATTACK: A1` sent the lone Thraben Doomsayer into a board whose only
  creatures were `Pride Guardian #1 (0/3) (neither dies (blocking trigger: they gain 3)); Pride
  Guardian #2 (0/3) (...they gain 3)`, with `Cards you have seen in the opponent's hand: Sanguine
  Bond` on the same screen. COMBAT CHECK 1 BRANCH A says "ATTACK: none ... until removal has taken
  the LAST of them off that line". The attack gained them 6 (20 -> 26) and raised the deck's own
  L+C+3 stop for the rest of the game. STRATEGY.
- **seq 36 (T10, repeat row)**: the model computed its stop correctly *inside the same reply* and
  the engine executed the number it had just repudiated. See HIGH-1.
It still won: seq 38 `ATTACK: all` for 39 into 26.

**vs deck130 - WIN, turn 19, 10 to -78.** Fetch/colour play was correct throughout (seq 5 Marsh
Flats -> Scrubland; seq 18 Isolated Chapel taken on the "enters UNTAPPED" render). Deciding
decision **seq 29 (T16 blockers)**: `BLOCKS: B1:A1`, Bloodline Keeper on Siege-Gang Commander, off
`B1. Bloodline Keeper ... may block A1 (you kill it, your blocker lives)` under a header reading
`NOT lethal`. That is STEP 0's free kill and also the assignment STEP 0b tells it to ignore when
the header is not lethal - see LOW-2. It was the right play; the Keeper lived, transformed at seq
37, and the game ended at -78.

**vs deck125 - LOSS, turn 32, -14 to 45.** Deck125 countered the T8 Doomsayer with Essence Scatter
(game log at seq 16) and this seat never got a maker to stick; only ONE token-activation window was
ever offered in 50 records. Largely variance, but two guide-cited misplays compounded it:
- **seq 13 (T12)**: took `2. Cast Idyllic Tutor {2}{w} [finds only an enchantment card - every
  enchantment left in your library is a copy of one you already control or hold: Intruder Alarm]`
  with the Alarm already on the battlefield. The guide names that bracket verbatim as DEAD, and the
  counter-deck bait carve-out did not apply: both Bloodline Keepers on the hand line read `[cannot
  pay now: needs 4 mana, you have 3 untapped sources]`, so no maker row was on this window's menu.
  Correct answer was row 3, "Cast nothing right now". STRATEGY.
- **seq 15 (T12 cleanup, caused by seq 13's overdraw)**: `PUT: 3` discarded **Damnation** - its only
  sweeper against the Supreme Verdict / Sphinx's Revelation deck - while holding `Lightning Greaves
  (copy 1 of 2)` and `(copy 2 of 2)`. The guide's Vision Skeins discard rule names the answer ("the
  SPARE - a 'copy 2 of 2' ... never your only maker, only Alarm or only Damnation") and names
  Lightning Greaves as the leftover. The model wrote "Discard spare Damnation"; it was not spare.
  STRATEGY.
- seq 43 (T32, Emrakul annihilator 6) is LOW-1; the game was already lost (`INCOMING THIS COMBAT: 1
  attacker, 15 unblocked damage - you would be at -14; this KILLS you`).

**vs deck152 - LOSS, turn 15, -12 to 20.** Real flood/variance: from T12 on the hand was Devour
Flesh + Lightning Greaves + Idyllic Tutor with the Alarm already out and 6-7 lands, and the reads
were all correct (seq 33, seq 49: Tutor dead, Greaves "equips nothing", Devour Flesh at `they
control 3/6 creatures - they choose which one`). Two notes:
- **seq 17 (T10)**: `CHOICE: 3 (Play no land right now)` with `1. Play Tundra` and `2. Play
  Underground Sea` on the screen - a straight Rule 0 violation. Recovered only because the engine
  re-offered the unused drop at seq 19, where it played the Tundra. STRATEGY, no cost this game.
- seqs 49/50 carried `L=20, C=6, stop=29. M=0. Create 29 tokens.` onto casting menus with zero
  creatures anywhere and no Create row - the exact plan-shape leak Rule 1 forbids. It did not change
  the answer (both windows were "Cast nothing right now") but it is the failure mode the guide's
  "THIS PLAN SHAPE BELONGS TO THE TOKEN-ACTIVATION WINDOW AND TO NOTHING ELSE" paragraph exists for.

**vs deck162 - WIN, turn 12, 1 to -1089.** This is the owner-flagged game; see HIGH-2 for the
answer. Four of the corpus's eight re-asks are here (seqs 29, 68, 111 `index_name_conflict`; seq 45
`no_pass_reask`). Deck162's board was 3x Howling Mine + 2x Dictate of Kruphix + Underworld Dreams,
and the render priced it honestly at seq 73: `DRAW FORECAST: your next draw step draws 6 cards ... =
6 life LOST BY YOU ... if it resolves as forecast you would be at 3.` The seat finished at 1 life.

**vs deck146 - LOSS, turn 11, 0 to 22.** Land-light (no drop at T5-T7). The deciding decision is
**seq 40 (T10, 8 life)**:
```
Your battlefield (3 permanents listed, of which 0 are creatures, 0 of them able to attack right now, and 3 are lands): Plains; Scrubland; Tundra
CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack (tapped ones untap first), for up to 7 - you would be at 1
YOUR PLAN (as you last stated it, 1 window ago on turn 8): Turn 8: Cast Doomsayer. Board: 1 Doomsayer. ...
2. Cast Thraben Doomsayer {1}{w}{w} (2/2) {leaves 0 of your 3 untapped mana sources untapped ...}
5. Cast Idyllic Tutor {2}{w} [finds only an enchantment card - still in your library ...: Intruder Alarm]
```
Reply: `CHOICE: 5 (Cast Idyllic Tutor)` / `PLAN: ... Board: 1 Doomsayer, 1 Alarm.` The T8 Doomsayer
had been killed; the battlefield line says `0 are creatures`. The cast list is unambiguous - CL2
(Thraben Doomsayer, "when no token-maker is on your battlefield") outranks CL4 (Idyllic Tutor) - and
row 2 was on the menu. Taking row 2 also puts a body in front of the 7 the crack-back line prints.
It tapped out for a card instead, and died the next turn with zero creatures. STRATEGY, and the
game. The stale-plan half of it is MED-4.

## 2. Engine / interface / card items

### HIGH-1 - `later_answer_ignored` discards a self-marked correction on an UNBOUNDED row
File `1788683483-ai_baka_deck123-0x558592dbc5f0-vs-ai_baka_deck126.jsonl`, **seq 36**, T10.
Reply, verbatim:
```
CHOICE: 2 (Create human with Thraben Doomsayer x30)
PLAN: L=26, C=4, stop=33; M=41 now; this window: x30 (41-30=11, wait, stop is 33, 41 > 33, so pass).
Correction: M=41. Stop = L+C+3 = 26+4+3 = 33. M (41) is already above stop (33).
Therefore, I should not create any more tokens.

CHOICE: 0 (pass)
```
Record fields: `parse_note: later_answer_ignored`, `repeat_n: 30`, `latched_coded_line: 1`,
`latched_line_in_plan: false`, `coded_answers: 2`. The word CORRECTION *is* present, but two lines
below the corrected answer's original rather than on it or just above it, so AP's adjacency test
misses it and the engine performs 30 activations the model spent its own reply refusing.
This is not a symmetric cost. On every other row family a wrong first-wins read costs one card or
one window; on the repeat row it costs up to 200 activations, and this seat rode that to 266
creatures in one turn (HIGH-2). Two candidate fixes, either sufficient:
 (a) **Read the PLAN the row already demands.** The row's own text says "a count on this row
     REQUIRES a PLAN line stating your stop count, the count you are at now, and how many you
     perform this window - a count with no PLAN line is refused and re-asked". The engine demands
     those three numbers and never looks at them. When the reply's own plan states a stop and a
     current count whose difference is <= 0, refuse-and-re-ask exactly as it does for a missing
     plan. That closes seq 36 and all ten overshoot windows in the 162 game with the machinery
     already present.
 (b) On the repeat row only, widen the correction test: a later coded line preceded anywhere by
     CORRECTION/Correction supersedes.

### HIGH-2 - the repeat row is the one row family that prints no `{right now: ...}` verdict
File `1788683516-ai_baka_deck123-0x55f8cd35b2e0-vs-ai_baka_deck162.jsonl`, **seq 73**, T11, rendered
row verbatim:
```
2. Create vampire with Lord of Lineage, repeated N times, then stop [you control 68 creatures right now; you name N on the CHOICE line, e.g. "CHOICE: 2 (Create vampire with Lord of Lineage x<N>)"; the engine performs it N times, re-checking the cost each iteration and stopping early if it becomes unpayable, then returns priority to you here; N is at most 200; a count on this row REQUIRES a PLAN line stating your stop count, the count you are at now, and how many you perform this window - a count with no PLAN line is refused and re-asked]
```
M is printed. Opponent life, their blocker count, and any verdict on whether another body does
anything are not. Every other decision in this engine now computes for the model - the Damnation row
prints `destroys N of their creatures (K ...), M of yours`, the Slip row prints `- DIES`, the edict
row prints `they control N creatures`, the attack header prints `that KILLS them whatever they
block`. This row asks the model to do arithmetic instead, and the model got it wrong in 10 of 13
counted windows in one turn while printing the right stop in its own plan each time:
seqs 73 (`stop=20; M=68 now; this window: x0 / pass (stop reached: 68 >= 15+2+3=20)` -> took **x33**),
76 (M=101 -> x10), 79 (M=111 -> x21), 82 (M=132 -> x20), 85 (M=152 -> x1), 90 (M=153 -> x47),
93 (M=200 -> x30), 96 (M=230 -> **x200**), 100 (M=232 -> x21), 103 (M=253 -> x13), stopping at
seq 106 with M=266 against an opponent on 15 life with 2 creatures.
Cost: that game is 167 records against a 43-50 median for this seat's other five, and it finished at
**1 life** while the DRAW FORECAST at seq 73 said the next draw step alone would take it to 3.
Concrete fix: give the row the same clause every other row has, e.g.
`{right now: they are at 15 and control 2 possible blockers - 20 bodies already wins; you control 68}`.

**The owner's flagged question, answered from this seat: 162v123's -1089 is not an engine loop, and
the loop verdicts did print before it fired.** The chain above is the whole mechanism: 277 attackers
built on turn 11, swung on turn 12. **seq 165**:
`ATTACK TOTAL: 277 attackers listed, 1109 total combat damage to a player - declaring all of them
with none blocked puts them at -1094. At least 1100 damage lands whatever they block - they would be
at -1085; that KILLS them whatever they block.` Reply `ATTACK: A1, A2-A277`. Nothing in the
narration lied and nothing looped; the overshoot is entirely upstream, in HIGH-1 and HIGH-2.

### MED-1 - `index_name_conflict` burns a window when the short name is a unique match
File `...vs-ai_baka_deck162`, **seqs 29, 68, 111** - all three of the corpus's three
`index_name_conflict` re-asks are this seat, this one row pair. Reply at seq 68: `CHOICE: 2 (Hold
priority)`. The menu:
```
2. Cast nothing right now
3. Hold priority: pass now, and do not ask me again - this turn or later - until one of the rows above changes ...
```
"Hold priority" prefix-matches exactly one row and matches no other row on the menu. The number is
wrong and the name is unambiguous; re-asking spends a window to recover information the reply
already carried unambiguously. Suggest: when the short name prefix-matches exactly ONE row and the
number points elsewhere, resolve by name and record `index_name_resolved_by_name` instead of
re-asking. (Flagged, not asserted: AO chose re-ask deliberately; this is the measured cost.)

### MED-2 - two menu families, two numbers for the same act
File `...vs-ai_baka_deck162`, **seq 45**, reply `CHOICE: 0 (pass)` -> `<refused: no_pass_reask>`.
On the ability/priority window the last row is `0. Pass priority (take no action this window)`
(seq 73 above). On the "Casting decision (...): which card do you cast now, if any?" window the rows
run 1..N with no row 0, and the same act is the numbered `Cast nothing right now`. Accepting
`CHOICE: 0 (pass)` on a casting menu as that row - or printing the pass as row 0 on both - removes
the class. Note this is the one place where deck123's live guide is factually wrong ("PASSING IS
ALWAYS AVAILABLE: the last row reads \"0. Pass priority ...\"" - true of the window Rule 1 is about,
false of the casting menu); I did not spend guide bytes on it, see section 3.

### MED-3 - the hold row is not being taken: 182 declines to 4 holds
Across this seat's 6 games: `Cast nothing right now` **182**, `Hold priority` **4**
(deck125 0/18, deck146 0/30, deck152 1/45, deck162 1/80, deck130 0/9, deck126 2/4). Both rows
decline; only the hold suppresses the re-offer. The shape is deck152 seqs 23-31 and 36-47 - nine
consecutive `Cast nothing right now` per opponent turn on menus whose `[hold check: every row above
was also on the menu at the last window at this seam]` says nothing moved. This is deck123's
measurement for the lane's re-offer/decline churn question: G7's re-opener is not what costs the
windows here; the decline row winning 98% of the time is. The two rows are the same decision from
the model's seat, and the decline row is the one the guide's own cast list numbers (CL8). Collapsing
them - making the decline row itself carry the hold - would be worth more than tuning the re-opener.

### MED-4 - a stale carried plan sits beside a board line that refutes it, unflagged
File `...vs-ai_baka_deck146`, **seq 40**: `YOUR PLAN (...on turn 8): ... Board: 1 Doomsayer.` printed
directly above `Your battlefield (3 permanents listed, of which 0 are creatures...)`. The Doomsayer
was countered/killed on turn 8 and the game log two blocks up says so. Nothing on the screen marks
the conflict, and the model wrote `Board: 1 Doomsayer, 1 Alarm` in the reply that lost the game.
The plan text is already scanned for the 400-char carry; a one-line tag when a permanent the carried
plan names is absent from the battlefield line ("your carried plan names Thraben Doomsayer; you do
not control one") is cheap and lands exactly where AL's contextText work already runs.

### LOW-1 - forced-sacrifice ask has no material ranking, and the model pitched its win condition
File `...vs-ai_baka_deck125`, **seq 43** (Emrakul annihilator 6, 6 sacrifices). The ask advises
"pitch a spare land or a redundant/dead card, and KEEP your best spells, answers, and threats" and
lists nine lands plus `10. Intruder Alarm [enchantment]`. Reply: `CHOICE: 10 (Intruder Alarm)`.
Zero cost (`INCOMING ... you would be at -14; this KILLS you`), but the same ask on a live board
would hand away the combo. AN's "forced-sacrifice tie-break faces whoever is paid" is about the
tie-break; the *ordering* advice is prose the model did not follow. A verdict clause on each row
(`{right now: a spare land - you control 9}` vs `{right now: your only Intruder Alarm}`) fits the
engine's existing idiom and would settle it.

### LOW-2 - guide-internal, but visible only at the seam
File `...vs-ai_baka_deck130`, **seq 29**: header `NOT lethal - block only where the trade favors
you` plus the named assignment `Bloodline Keeper blocks Siege-Gang Commander`, and the B row
`B1. Bloodline Keeper (3/3) ... may block A1 (you kill it, your blocker lives)`. COMBAT STEP 0 calls
that a free kill and says take it; STEP 0b says take the header's named assignment ONLY when the
header says LETHAL. Both fire on this one row. The model blocked and it was correct. Recording it so
the contradiction is on the record; it has not yet cost anything, so it does not pay for an edit.

### Not observed
- No heuristic fallbacks, no `empty_reply`, no `transport_error`, no `deadline_pct > 100`, no
  translog gaps, no crashes or hangs in this seat's 6 games (21/21 corpus-wide natural).
- **AL's G4 question, from this seat: no decision was starved of the ceiling number.** All 43
  `CRACK-BACK NEXT TURN:` lines rendered to this seat carry an explicit `you would be at X`
  (4 of them carrying the FLOOR wording). seq 40's `for up to 7 - you would be at 1` is the one that
  mattered and it was fully priced.
- **AL's G5**: the DRAW PUNISHERS billing in the 162 game is the opponent's Underworld Dreams
  charging this seat's draws, which is correct; no own-draw-step mis-billing seen.

## 3. Guide verdict: KEEP

No edit. `bin/Res/ai/baka/deck123_strategy.txt` is 70,993 bytes, at the top of the 41-71 KB band, so
any addition must evict text an earlier corpus paid for - and every misplay in these six games is
already covered by a rule in the live file, in words, with a worked example:

| seq | misplay | rule already in the guide |
|---|---|---|
| 126 s12 | lone Doomsayer into two Pride Guardians | COMBAT CHECK 1 BRANCH A, by name |
| 125 s13 | dead Idyllic Tutor | CL4 + the Tutor bracket, that bracket quoted verbatim |
| 125 s15 | discarded Damnation over a "copy 2 of 2" Greaves | KEY CARDS / Vision Skeins discard order, Greaves named as the leftover |
| 146 s40 | Tutor over Doomsayer at 0 creatures | CL2 above CL4 |
| 152 s17 | "Play no land right now" with two Play rows | RULE 0, first line |
| 162 s73-103, 126 s36 | took a count its own plan called past the stop | RULE 1: "when (L + C + 3) - M is 0 or less, that line is CHOICE: 0 (pass)"; "Never leave a CHOICE line standing that your plan line contradicts" |

What the guide *gets*, it got: six mulligans, six keeps, no game decided there; the fetch rows were
read correctly every time (130 s5, 146 s37, 125 s9); the Isolated Chapel "enters UNTAPPED" clause was
used (152 s13/15, 162 s7); Damnation was cast once, on a `destroys 2 of their creatures (2 without a
restriction against attacking)` row (152 s16), and never on a dead one; Tragic Slip was never cast
off a `kills 0 of the N legal targets` row; Devour Flesh was correctly declined at N of 3 and 6
(152 s33, s49); the second-Alarm and equips-nothing brackets were obeyed throughout. This is a
successful guide with an adherence problem, and adherence is not bought with more text - the two
places it visibly failed are the two places the engine can decide instead of describe (HIGH-1,
HIGH-2).

The one place the live guide is factually **wrong** - "PASSING IS ALWAYS AVAILABLE: the last row
reads \"0. Pass priority (take no action this window)\"", true of the ability window Rule 1 is about
and false of the casting menu, cost measured at deck162 seq 45 - is deliberately left alone: it is
one refused window in a game this seat won, correcting it in prose costs ~450 bytes the file does
not have, and the same failure exists in every deck's seat. MED-2 fixes it once at the seam instead
of seven times in prose.

## 4. What I did NOT check
- The five lane files' predictions (AL/AM/AN/AO/AP) - engine seat's adjudication, not mine.
- The opponent seat JSONLs, except results.tsv and whatever the opponents' actions rendered into my
  own game logs; in particular I did not read deck162's own seat for the -1089 game.
- Latency, `deadline_pct`, `transport`, `async_drops`, HOLD render/take census - engine seat.
- Any deck's card list other than deck123's, and the general guide (not loaded at runtime).
- No `general-proposals.md` or `skill-proposals.md` written: HIGH-1/HIGH-2 are engine seams, not
  general-strategy or skill text, and nothing here generalises past this deck's repeat row.
